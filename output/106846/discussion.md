# performance regression in clang-19 when using computed goto

**Author:** mikulas-patocka
**URL:** https://github.com/llvm/llvm-project/issues/106846
**Status:** Closed
**Labels:** llvm:codegen, release:backport, missed-optimization, regression:19
**Closed Date:** 2025-03-10T11:34:09Z

## Body

Hi

I noticed that the Ajla programming language ( https://www.ajla-lang.cz/ ) runs slower when being compiled by clang-19 compared to clang-18 or gcc-14. I looked at the assembler, and it turns out that the bytecode interpreter (the file "ipret.c") is not compiled as efficiently as it could be. In particular, clang-19 joins all the "goto *next_label" statements in a function into just one "jmp *" instruction. That reduces code size, but it also makes branch prediction inefficient, because the CPU cannot learn that a single instruction jumps to multiple targets.

I created this example that shows the issue:
http://www.jikos.cz/~mikulas/testcases/clang/computed-goto.c
(use: clang-19 -O2 computed-goto.c && time ./a.out)

The results (in seconds) are here:
http://www.jikos.cz/~mikulas/testcases/clang/computed-goto.txt

We can see that the worst slowdown happens on Sandy Bridge. Zen 4 shows no slowdown, so it seems that it has smart indirect branch predictor that can predict multiple jump targets from a single instruction.

## Comments

### Comment 1 - nikic

This is caused by https://github.com/llvm/llvm-project/pull/78582, we get separate indirect gotos with `-mllvm -tail-dup-pred-size=30`. cc @DianQK Possibly we should not apply the limitation to INDIRECTBR, which is more profitable to tail-duplicate than other instructions. 

---

### Comment 2 - mikulas-patocka

-mllvm -tail-dup-pred-size=30 helps the testcase, but not Ajla. If you compile Ajla with clang-19 -O2 -mllvm -tail-dup-pred-size=30 and do
`$ objdump -d ipret.o | grep 'jmp    \*'`
it shows that there is just one indirect jump in the whole file.

---

### Comment 3 - nikic

What about `-mllvm -tail-dup-pred-size=1000`? The value needs to be about as large as the number of instructions / indirect jumps.

---

### Comment 4 - mikulas-patocka

Yes - there are 2284 instructions in the Ajla interpreter. So, I set tail-dup-pred-size=3000 and I get the same output as with clang-18 - 2466 indirect jumps in ipret.o.

---

### Comment 5 - pinskia

Note GCC (RTL) has a specific pass which duplicates the computed gotos (as GCC merges all computed goto into one BB for performance reasons) which was added in GCC 4.0 and then improved for GCC 7.1.0 (to also unduplicate some related instructions). This pass happens late in the pipeline after register allocation. This is just explaining GCC's solution to the problem and nothing more.

---

### Comment 6 - dianqk

> This is caused by #78582, we get separate indirect gotos with `-mllvm -tail-dup-pred-size=30`. cc @DianQK Possibly we should not apply the limitation to INDIRECTBR, which is more profitable to tail-duplicate than other instructions.

It looks like makes sense. Thanks!
I will check it later.

---

### Comment 7 - dianqk

The `switch` instruction also becomes an INDIRECTBR machine instruction, and I think we should apply the limitation to PHI.
If I remember correctly, both https://github.com/llvm/llvm-project/issues/78578 and https://github.com/llvm/llvm-project/pull/79993#issuecomment-1936822679 contain a massive PHI. Duplicating the BB that contains the massive PHI causes the CFG to become exceptionally complex, leading to issues with compile time and runtime performance.

---

### Comment 8 - dianqk

@mikulas-patocka Can you provide an example of Ajla? I may need to investigate this with the help of it.

BTW, whether using INDIRECTBR directly or coming over from SWITCH, we introduce a lot of PHIs.

---

### Comment 9 - mikulas-patocka

The Ajla compiler is written in Ajla, you can have a look at files in newlib/compiler/. I usually use self-compilation as a benchmark.

Run './configure && make' with CFLAGS='-O2 -DDEBUG_ENV'. The DEBUG_ENV macro enables envirnment variables that are useful for debugging.

Set the environment variable 'export CG=none' - this will disable the machine code generator and use only the interpreter.

Run the script ./scripts/update.sh - this will compile the Ajla compiler itself and it will re-generate the file builtin.pcd.
The results (on i7-2640M, Sandy Bridge, 2 cores, 4 threads):
gcc-14:  22.8s
clang-18:  22.2s
clang-19:  26.8s
The results (on 7840U, Zen 4, 8 cores, 16 threads):
gcc-14:  4.1s
clang-18:  4.1s
clang-19:  4.4s

If you want some simple benchmark, copy this piece of code to a file loop.ajla
```
fn main
[
        var sum := 0;
        for i := 0 to ston(args[0]) do
                sum += 1;
        write(h[1], ntos(sum) + nl);
]
```
and run it with 'CG=none ./ajla --nosave loop.ajla 1000000000'
The results (on i7-2640M):
gcc-14:  17.3s
clang-18:  15.3s
clang-19:  39.0s
On 7840U there is too much jitter in this test.

---

### Comment 10 - dianqk

By comparing the results from gcc, I can observe the following:

- In the initial OOM example, which creates multiple consecutive switch statements, gcc did not apply tail duplication.
- For `computed-goto.c` and `ipret.c`, it appears that clang applied more tail duplication (perhaps unnecessarily?).
- The compile time for `ipret.c` with `clang -O3` is unacceptably long, mainly due to Branch Probability Basic Block Placement (which is definitely a separate issue).

Next, I'll look into some gcc code to see if I can find anything useful. Perhaps gcc is simply differentiating between `goto` and `switch` at the source level? :) Although I think they should be interchangeable in this scenario.

<details><summary>Detailed commands</summary>
<p>

```
$ time -f '%M' gcc -O3 -c oom_manual2.c -S
129608
$ cat oom_manual2.s | grep "jmp.*%" | wc -l
26
$ wc -l oom_manual2.s
18494 oom_manual2.s

$ time -f '%M' clang -O3 -c oom_manual2.c -mllvm -tail-dup-pred-size=3000 -mllvm -tail-dup-succ-size=3000 -S
836248
$ cat oom_manual2.s | grep "jmp.*%" | wc -l
3081
$ wc -l oom_manual2.s
50527 oom_manual2.s


$ time -f '%M' gcc -O3 -S -c computed-goto.c
47556
$ cat computed-goto.s | grep "jmp.*%" | wc -l
32
$ wc -l computed-goto.s
1184 computed-goto.s

$ time -f '%M' clang -O3 -mllvm -tail-dup-pred-size=3000 -mllvm -tail-dup-succ-size=3000 -S -c computed-goto.c
119084
$ cat computed-goto.s | grep "jmp.*%" | wc -l
51
$ wc -l computed-goto.s
1717 computed-goto.s
```

</p>
</details> 

---

### Comment 11 - dianqk

> Possibly we should not apply the limitation to INDIRECTBR, which is more profitable to tail-duplicate than other instructions.

A quick note:  
Based on the comments below, this is undoubtedly what GCC does: https://github.com/gcc-mirror/gcc/blob/7f67acf60c5429895d7c9e5df81796753e2913e0/gcc/rtlanal.cc#L3615-L3621.

```cc
/* Return true if INSN is an indirect jump (aka computed jump).

   Tablejumps and casesi insns are not considered indirect jumps;
   we can recognize them by a (use (label_ref)).  */
```

I’ll go with this approach, but from the machine instructions: https://llvm.godbolt.org/z/jcje1xPaW, I haven’t found any *fundamental* difference between computed goto and table jumps. I plan to investigate further and then file this PR.

---

### Comment 12 - nelhage

It looks like this issue is also impacting CPython: https://github.com/python/cpython/issues/129987#issuecomment-2654504283

I measured a ~4% performance improvement on x86_64 by using an `__asm__ volatile("")` hack to force the jumps to remain deduplicated; I haven't tested with `-mllvm -tail-dup-pred-size=5000` yet but I expect a similar improvement.

---

### Comment 13 - dianqk

> It looks like this issue is also impacting CPython: [python/cpython#129987 (comment)](https://github.com/python/cpython/issues/129987#issuecomment-2654504283)
> 
> I measured a ~4% performance improvement on x86_64 by using an `__asm__ volatile("")` hack to force the jumps to remain deduplicated; I haven't tested with `-mllvm -tail-dup-pred-size=5000` yet but I expect a similar improvement.

Does #114990 or #116072 fix it?

---

### Comment 14 - nelhage

I haven't tested, but I do notice that https://github.com/llvm/llvm-project/pull/116072 cites CPython as their motivation so I suspect we're looking at the same issue. Is there somewhere I can download a nightly or otherwise test without doing a source build myself?

---

### Comment 15 - dianqk

> I haven't tested, but I do notice that [#116072](https://github.com/llvm/llvm-project/pull/116072) cites CPython as their motivation so I suspect we're looking at the same issue. Is there somewhere I can download a nightly or otherwise test without doing a source build myself?

I'll test it. LLVM 20 might not be available in your distribution.

---

### Comment 16 - dianqk

I can confirm that #116072 does not resolve the issue, whereas #114990 does.

```sh
# main branch
$ objdump -S --disassemble=_PyEval_EvalFrameDefault Python/ceval.o | grep -cF 'DISPATCH()'
27
# 114990
$ objdump -S --disassemble=_PyEval_EvalFrameDefault Python/ceval.o | grep -cF 'DISPATCH()'
234
```

---

### Comment 17 - dianqk

> I can confirm that [#116072](https://github.com/llvm/llvm-project/pull/116072) does not resolve the issue, whereas [#114990](https://github.com/llvm/llvm-project/pull/114990) does.

@nikic @fhahn I propose to merge #114990 and backport it. This issue has already affected multiple projects, and Python is a widely used one. In fact, this PR just reverts the computed goto part of #78582.

---

### Comment 18 - nelhage

I've done some more benchmarking here.

 I'd [previously](https://github.com/llvm/llvm-project/issues/106846#issuecomment-2655130829) estimated the cost here as ~4% for CPython; my [current data](https://github.com/nelhage/cpython-interp-perf/#results) puts the number at closer to 10% (!).

I have also benchmarked https://github.com/llvm/llvm-project/pull/114990, and confirmed that it fixes the performance regression, as well as fixing the tail-duplication logic (as tested by @DianQK above).

---

### Comment 19 - Fidget-Spinner

As much as I would like the tail call interpreter to beat the computed goto one, fixing the computed goto interpreter is more important as it affects more systems :). Thanks Nelson for your investigation of this, and DianQK for the patch!

---

### Comment 20 - zeux

FWIW this also affects Luau interpreter (https://github.com/luau-lang/luau); on a Zen 4 system, running the Luau benchmark suite (which reports geomean deltas) with `luau -O2` and using clang-18 built binary as a baseline I get:

```
'luau-19 -O2' change is -15.590% negative on average
'luau-19-fix -O2' change is 2.966% positive on average
```

luau-19 is built with clang-19, luau-19-fix is built with additional settings `-mllvm -tail-dup-pred-size=256 -mllvm -tail-dup-succ-size=256`.

The degradation with clang-20 is less significant but still severe, perhaps the change to handling blocks without phi helps here but does not help enough:

```
'luau-20 -O2' change is -8.727% negative on average
'luau-20-fix -O2' change is 3.834% positive on average
```

---

### Comment 21 - dianqk

I apologize for the disruption this regression has caused. Thanks to like https://llvm-compile-time-tracker.com and https://github.com/dtcxzyw/llvm-opt-benchmark, LLVM has been able to catch many issues early. I'm exploring ways to catch issues similar to this one earlier.

---

### Comment 22 - dianqk

/cherry-pick dd21aacd76e36d4db157a5d7a7b5370d456426e6

---

### Comment 23 - llvmbot

/pull-request llvm/llvm-project#130585

---

### Comment 24 - fhahn

@mikulas-patocka @efriedma-quic could you double-check that current `main` does not regress Ajla perf after https://github.com/llvm/llvm-project/pull/150911 landed?

---

### Comment 25 - mikulas-patocka

> [@mikulas-patocka](https://github.com/mikulas-patocka) [@efriedma-quic](https://github.com/efriedma-quic) could you double-check that current `main` does not regress Ajla perf after [#150911](https://github.com/llvm/llvm-project/pull/150911) landed?

Yes - it's much better than it used to be. The time to compile Ajla with clang is 3m33s (compared to 1m26s with gcc). There's no performance difference when Ajla compiles itself. For a microbenchmark that just loops and add numbers, clang is actually faster.

However, the generated code is still suboptimal, for example this is a piece of code in the interpreter in the function u_run that adds two numbers. On gcc, we get this code - the code is optimal, there is no useless instruction:
```
   24801:       0f b6 4d 03             movzbl 0x3(%rbp),%ecx           # load 3 indices from the bytecode stream
   24805:       0f b6 55 02             movzbl 0x2(%rbp),%edx
   24809:       0f b6 75 04             movzbl 0x4(%rbp),%esi
   2480d:       0f b6 3c 0b             movzbl (%rbx,%rcx,1),%edi       # test tags on both input variables
   24811:       40 0a 3c 13             or     (%rbx,%rdx,1),%dil
   24815:       0f 85 54 da ff ff       jne    2226f <u_run+0x1cddf>    # exit if at least one of them is tagged
   2481b:       48 8b 14 d3             mov    (%rbx,%rdx,8),%rdx       # add the input variables
   2481f:       48 03 14 cb             add    (%rbx,%rcx,8),%rdx
   24823:       0f 80 46 da ff ff       jo     2226f <u_run+0x1cddf>    # exit if there is overflow
   24829:       48 89 14 f3             mov    %rdx,(%rbx,%rsi,8)
   2482d:       0f b7 45 06             movzwl 0x6(%rbp),%eax           # load the next opcode
   24831:       48 83 c5 06             add    $0x6,%rbp                # increment the bytecode pointer
   24835:       ff 24 c5 00 00 00 00    jmp    *0x0(,%rax,8)            # jump to the next instruction
```

on clang from current git, we get this:
```
    bff0:       41 0f b6 4e 02          movzbl 0x2(%r14),%ecx
    bff5:       41 0f b6 46 03          movzbl 0x3(%r14),%eax
    bffa:       48 8b 74 24 28          mov    0x28(%rsp),%rsi
    bfff:       0f b6 14 06             movzbl (%rsi,%rax,1),%edx
    c003:       4c 8b 3d 00 00 00 00    mov    0x0(%rip),%r15
    c00a:       0a 14 0e                or     (%rsi,%rcx,1),%dl
    c00d:       0f 85 75 1d 00 00       jne    dd88 <u_run+0xdd88>
    c013:       41 0f b6 56 04          movzbl 0x4(%r14),%edx
    c018:       48 8b 74 24 28          mov    0x28(%rsp),%rsi
    c01d:       48 8b 0c ce             mov    (%rsi,%rcx,8),%rcx
    c021:       48 8b 04 c6             mov    (%rsi,%rax,8),%rax
    c025:       48 01 c1                add    %rax,%rcx
    c028:       0f 80 b9 01 00 00       jo     c1e7 <u_run+0xc1e7>
    c02e:       48 89 0c d6             mov    %rcx,(%rsi,%rdx,8)
    c032:       4d 89 f7                mov    %r14,%r15
    c035:       49 83 c7 06             add    $0x6,%r15
    c039:       4d 89 fe                mov    %r15,%r14
    c03c:       41 0f b7 07             movzwl (%r15),%eax
    c040:       48 8d 0d 00 00 00 00    lea    0x0(%rip),%rcx
    c047:       ff 24 c1                jmp    *(%rcx,%rax,8)
```


---

### Comment 26 - fhahn

> > [@mikulas-patocka](https://github.com/mikulas-patocka) [@efriedma-quic](https://github.com/efriedma-quic) could you double-check that current `main` does not regress Ajla perf after [#150911](https://github.com/llvm/llvm-project/pull/150911) landed?
> 
> Yes - it's much better than it used to be. The time to compile Ajla with clang is 3m33s (compared to 1m26s with gcc). There's no performance difference when Ajla compiles itself. For a microbenchmark that just loops and add numbers, clang is actually faster.
> 
> However, the generated code is still suboptimal, for example this is a piece of code in the interpreter in the function u_run that adds two numbers. On gcc, we get this code - the code is optimal, there is no useless instruction:


Sure there are probably other unrelated optimizations we could do. Just to double-check, IIUC from your response, https://github.com/llvm/llvm-project/pull/150911 specifically did not impact performance  negatively?



---

### Comment 27 - mikulas-patocka

Hi

I did some more thorough testing and I found out that there is a regression in clang-22 from git. I tested clang 18 to clang-22 - clang-18, clang-19, clang-20, clang-21, clang-22 were installed from Debian Sid. clang-22-git is the current clang version from git: b16fe132c68834a601b29c0eb53c7ec25a3cd627. The tests are done on i7-2640M.

Ajla microbenchmark, adding numbers from 1 to 1000000000:
```
clang-18:        9.67
clang-19:       12.70
clang-20:       12.70
clang-21:        9.75
clang-22:        9.87
clang-22-git:   10.84
gcc-15:         12.49
gcc-15-no-cmov:  9.85
```

Compiling the Ajla compiler with itself:
```
clang-18:       29.29
clang-19:       34.24
clang-20:       34.87
clang-21:       31.39
clang-22:       31.11
clang-22-git:   31.36
gcc-15:         31.52
gcc-15-no-cmov: 30.12
```

On the microbenchmark, clang-22-git performs slightly slower than clang-22. The reason is that clang-22-git generates pointless register moves (as shown in my previous comment). clang-22 doesn't.

clang-18 generates the fastest code, all the following clang versions perform worse.

I was analyzing the gcc-vs-clang performance drop. It is caused by the fact that gcc generates a CMOV instruction in the JMP_FALSE opcode - this creates data dependence inside the CPU and the CPU cannot fetch further data from the bytecode until the dependence is resolved. When i insert a dummy asm statement into the code (so that gcc can't join the branches and generate CMOV), performance improved.

---

