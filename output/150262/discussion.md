# hexagon compiler runs "forever" on matrix-spec-types at O2

**Author:** androm3da
**URL:** https://github.com/llvm/llvm-project/issues/150262
**Status:** Closed
**Labels:** llvm:codegen, slow-compile, release:backport, regression:21
**Closed Date:** 2025-10-09T11:53:54Z

## Body

As of [20.1.4](https://github.com/quic/toolchain_for_hexagon/releases/tag/v20.1.4) clang would build this test case within ~20 minutes or so on my Xeon Gold 6136 CPU.

Now, it seems to run for hours - I've let it go for 7 hours and it has not finished.  I have seen similar behavior with `llc -O2 matrix-types-spec.ll`.

[matrix-types-spec.ll.txt](https://github.com/user-attachments/files/21392092/matrix-types-spec.ll.txt)

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-hexagon

Author: Brian Cain (androm3da)

<details>
As of [20.1.4](https://github.com/quic/toolchain_for_hexagon/releases/tag/v20.1.4) clang would build this test case within ~20 minutes or so on my Xeon Gold 6136 CPU.

Now, it seems to run for hours - I've let it go for 7 hours and it has not finished.  I have seen similar behavior with `llc -O2 matrix-types-spec.ll`.

[matrix-types-spec.ll.txt](https://github.com/user-attachments/files/21392092/matrix-types-spec.ll.txt)
</details>


---

### Comment 2 - aankit-ca

I've narrowed down the issue to the MachinePipeliner pass which took about 9 hours to run on a reduced version of the original example.

```
===-------------------------------------------------------------------------===
                          Pass execution timing report
===-------------------------------------------------------------------------===
  Total Execution Time: 32789.9193 seconds (32797.0895 wall clock)

   ---User Time---   --System Time--   --User+System--   ---Wall Time---  --- Name ---
  32713.6319 ( 99.8%)   4.7475 ( 99.5%)  32718.3794 ( 99.8%)  32725.4233 ( 99.8%)  Modulo Software Pipelining
  58.6751 (  0.2%)   0.0139 (  0.3%)  58.6890 (  0.2%)  58.7968 (  0.2%)  Hexagon DAG->DAG Pattern Instruction Selection
   7.9919 (  0.0%)   0.0000 (  0.0%)   7.9919 (  0.0%)   7.9921 (  0.0%)  Machine Instruction Scheduler
   1.5369 (  0.0%)   0.0000 (  0.0%)   1.5369 (  0.0%)   1.5369 (  0.0%)  Post RA top-down list latency scheduler
   0.7863 (  0.0%)   0.0000 (  0.0%)   0.7863 (  0.0%)   0.7864 (  0.0%)  Hexagon Packetizer
   0.6647 (  0.0%)   0.0000 (  0.0%)   0.6647 (  0.0%)   0.6647 (  0.0%)  Prologue/Epilogue Insertion & Frame Finalization
```

The addDependenciesBetweenSUs functions seems to be the culprit here:
https://github.com/llvm/llvm-project/blob/d30fd562e8a45c90e8b256890100442b61e0dac8/llvm/lib/CodeGen/MachinePipeliner.cpp#L1089

@kasuga-fj Can you please look into this issue?

I've captured the IR before the machine pipeliner pass that you can use to work on this:

[before_pipeliner_21.0.mir.txt](https://github.com/user-attachments/files/21842167/before_pipeliner_21.0.mir.txt)

To run:

```
llc -O2 before_pipeliner_21.0.mir -run-pass=pipeliner  -o after-pipeliner.mir
```

---

### Comment 3 - kasuga-fj

Okay, I'll take a look next week.

Here's my early comment (I haven't run the test yet):
At a glance, I don't think the root cause of the original issue is `LoopCarriedOrderDepsTracker::addLoopCarriedDepenenciesForChunks`, because [it's not merged into LLVM 20.1.4](https://github.com/llvm/llvm-project/blob/ec28b8f9cc7f2ac187d8a617a6d08d5e56f9120e/llvm/lib/CodeGen/MachinePipeliner.cpp). Without any investigation, I suspect that `SMSchedule::orderDependence` may have triggered the issue, as I encountered a similar problem when running some random tests locally. The function is a recursive one, and it looks to me that the function potentially lead infinite recursion. IIRC, I let it run for several days back then, but it never completed. 

On the other hand, I agree with that `addLoopCarriedDepenenciesForChunks` can be time-consuming, and I'm open to mitigating it if it becomes a problem.

@aankit-ca Could you check which function is actually causing the issue? Or do you have any data indicating that `addLoopCarriedDepenenciesForChunks` is the culprit?


---

### Comment 4 - kasuga-fj

Sorry, I misunderstood the issue's description... Yes, I now agree with `addLoopCarriedDepenenciesForChunks` would be the culprit.

---

### Comment 5 - aankit-ca

Yeah, in this particular case, it took about 9 hours to run

---

### Comment 6 - kasuga-fj

I've considered some approaches to fix this issue, but I could only come up with non-trivial solutions other than #154940, which are not suitable for backporting. I still believe there is still room for parameter tuning, but if it doesn't work, I'll revert c0b82df5f3484870d3728156da7d7e3720ef53ad and 6df012ab48ececd27359bdc9448ee101b39eea7a.

---

### Comment 7 - kasuga-fj

Fortunately, https://github.com/llvm/llvm-project/pull/154940 seems to fix this issue. I'll make a backport request.

@androm3da @aankit-ca I've confirmed that the issue is now resolved, but if possible, could you please double-check it?

---

### Comment 8 - kasuga-fj

/cherry-pick 22b79fb3b9d051a83520eaa9d03abad782697448

---

### Comment 9 - llvmbot

/pull-request llvm/llvm-project#162639

---

