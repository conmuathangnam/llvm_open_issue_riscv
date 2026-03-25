# Increased spilling on rva22u64 after bidirectional scheduling/register pressure tracking

**Author:** lukel97
**URL:** https://github.com/llvm/llvm-project/issues/119222

## Body

After #115445 we seem to have significantly more spills in this function from x264 when compiling for rva22u64 -O3 (without RVV): https://godbolt.org/z/fcKdrjqxG

Unfortunately this seems to be the cause of a 1.8% regression on 525.x264_r from LLVM 19.1.5 to today: https://lnt.lukelau.me/db_default/v4/nts/profile/13/56/54

I don't think this necessarily warrants a revert given that the patch significantly improved performance for other benchmarks, at least with RVV, but I thought it would be worth flagging anyway. 

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-risc-v

Author: Luke Lau (lukel97)

<details>
After #<!-- -->115445 we seem to have significantly more spills in this function from x264 when compiling for rva22u64 -O3 (without RVV): https://godbolt.org/z/fcKdrjqxG

Unfortunately this seems to be the cause of a 1.8% regression on 525.x264_r from LLVM 19.1.5 to today: https://lnt.lukelau.me/db_default/v4/nts/profile/13/56/54

I don't think this necessarily warrants a revert given that the patch significantly improved performance for other benchmarks, at least with RVV, but I thought it would be worth flagging anyway. 
</details>


---

### Comment 2 - wangpc-pp

That's weird! We surprisingly have worse register pressure after enabling tracking register pressure! 😂😂😂
I will take a look to find out if it is because of bidirectional scheduling.

---

### Comment 3 - lukel97

> I will take a look to find out if it is because of bidirectional scheduling.

Yeah, I would hope it's the bidirectional scheduling and not the register pressure tracking!

I just did a run on 525.x264_r before and after 9122c5235ec85ce0c0ad337e862b006e7b349d84, it looks like it actually regressed by 5% https://lnt.lukelau.me/db_default/v4/nts/59?compare_to=60. I guess we must have clawed back that extra 3.2% in the meantime.

I'll kick off a run now for the other SPEC benchmarks too.

---

### Comment 4 - lukel97

Here's a simpler kernel that also seems to have much more spilling: https://godbolt.org/z/a3qf7zb4s

---

### Comment 5 - wangpc-pp

I am 100% sure it is because of bi-directional scheduling now. These spills will disappear if we specify `-mllvm -misched-bottomup=true` to force bottom-up scheduling (the previous default behaviour). I will take a detailed look tomorrow.

---

### Comment 6 - lukel97

Looks like nothing else in SPEC regressed, it was just x264: https://lnt.lukelau.me/db_default/v4/nts/61?compare_to=62

---

### Comment 7 - wangpc-pp

In short:
* If we disable latency heuristic via `-Xclang -target-feature -Xclang +disable-latency-sched-heuristic`, we can remove all the spills: https://godbolt.org/z/3j9P3nGqo
* If we also disable load/store clustering `-mllvm -riscv-misched-load-store-clustering=false`, we can generate the same code: https://godbolt.org/z/bcs75sEf1

It seems this is an example that enabling latency heuristic causes more spills.
Of course, there must be some problems in `GenericScheduler::pickNodeBidirectional`, but I haven't gone into that detailedly.

---

### Comment 8 - lukel97

> If we disable latency heuristic via -Xclang -target-feature -Xclang +disable-latency-sched-heuristic, we can remove all the spills:

I wonder if this is this the same issue as https://github.com/llvm/llvm-project/issues/107532?

---

### Comment 9 - wangpc-pp

> > If we disable latency heuristic via -Xclang -target-feature -Xclang +disable-latency-sched-heuristic, we can remove all the spills:
> 
> I wonder if this is this the same issue as https://github.com/llvm/llvm-project/issues/107532?

Thanks for reminding! I suspect that may be the cause, I will have a test after dinner. 🤪

---

### Comment 10 - wangpc-pp

> > > If we disable latency heuristic via -Xclang -target-feature -Xclang +disable-latency-sched-heuristic, we can remove all the spills:
> > 
> > 
> > I wonder if this is this the same issue as [#107532](https://github.com/llvm/llvm-project/issues/107532)?
> 
> Thanks for reminding! I suspect that may be the cause, I will have a test after dinner. 🤪

Nope... It is still generating a lot of (but less) spills after I changed `MicroOpBufferSize` to 1.

---

### Comment 11 - wangpc-pp

Summary:
1. If we change to `bidirectional` scheduling, we will use `pickNodeBidirectional` to pick up best candidate from `top`/`bot` queues.
  a. Pick the best candidate from `bot`.
  b. Pick the best candidate from `top`.
  c. Compare the bot test and the top test, and get the best one from both directions.
2. The logic is almost the same compared to `topdown`-only and `bottomup`-only scheduling, but there is a subtle difference: we don't call `setPolicy` when doing `topdown`/`bottomup` scheduling.
https://github.com/llvm/llvm-project/blob/93fab6e362a3124af76c6e82f90ca1385aea6d1f/llvm/lib/CodeGen/MachineScheduler.cpp#L3747-L3771
https://github.com/llvm/llvm-project/blob/93fab6e362a3124af76c6e82f90ca1385aea6d1f/llvm/lib/CodeGen/MachineScheduler.cpp#L3674-L3681
3. In `setPolicy`, we will decide whether we should reduce latency (set `Policy.ReduceLatency` to true):
https://github.com/llvm/llvm-project/blob/93fab6e362a3124af76c6e82f90ca1385aea6d1f/llvm/lib/CodeGen/MachineScheduler.cpp#L3035-L3068
That is the reason why disabling latency heuristic helps.
4. `Policy.ReduceLatency` will impact scheduling heuristics in `tryCandidate`:
https://github.com/llvm/llvm-project/blob/93fab6e362a3124af76c6e82f90ca1385aea6d1f/llvm/lib/CodeGen/MachineScheduler.cpp#L3603-L3628
5. Because of this heuristic, we will pick up different SUs and this leads to different SUs in `Available`/`Pending` queues:
![Image](https://github.com/user-attachments/assets/ad7f0845-f377-41a1-a40c-f2a285b0a8ce)
6. The reg pressure heuristic will only see the pressure diff of currect instruction, which may lead to a locally optimal result and this x264 kernel is an example.
7. If we call `setPolicy` when doing `topdown`/`bottomup` scheduling, we will also generate a lot of spills.

In a word, this is a result of several coherent heuristics, which may not be so common in real applications or when compiling with real CPU models.
We need a new heuristic to avoid this situation (but I haven't figure out how to do it), and current feasible solutions are:
* Using `topdown`/`bottomup` scheduling.
* Disable latency heuristic when doing `bidirectional` scheduling.

We can also try to remove calls to `setPolicy` in `pickNodeBidirectional` for PreRA scheduling, but it is almost the same as disabling latency heuristic and loses some opportunities to try resources heuristic. And in contrast, we can also call `setPolicy` when doing `topdown`/`bottomup` scheduling to try more heuristics (I digged the history and I haven't found why we didn't do this before...), but the change may impact too many things so I will leave it be.

---

