# [flang][LoongArch] Runtime error in spec2017 527.cam4_r with size=ref and optimization levels "-O3"/"-Ofast"

**Author:** azuresky01
**URL:** https://github.com/llvm/llvm-project/issues/136971
**Status:** Closed
**Labels:** miscompilation, backend:loongarch
**Closed Date:** 2025-05-15T13:55:00Z

## Body

Tested OS: Debian sid/experimental loong64 installed with llvm 20.1.3

Optimization setting:

`COPTIMIZE      = -g -Ofast -march=la464 -flto=auto`
`FOPTIMIZE      = -g -Ofast -march=la464 -flto=auto`

When I test spec2017 527.cam4_r with above optimization setting and ref size, I met the following error:

```
run_base_refrate_mytest_llvm_20d1d3-m64.0000 -f compare.cmd -E -e compare.err -o compare.stdout'; no non-empty output files exist
  Command returned exit code 1
****************************************
Contents of cam4_r_base.mytest_llvm_20d1d3-m64.err
****************************************
Fortran STOP
IEEE arithmetic exceptions signaled: DIVBYZERO INEXACT INVALID OVERFLOW UNDERFLOW

****************************************
*** Miscompare of cam4_validate.txt
```

The content in miscomparison file is:

```
0001:   PASS:  4  points. 
        PASS:  0  points. 
              ^
```

The above error still appears when I change the optimization level of Fortran part to `-O3`. If the optimization level of Fortran part is `-O2` or lower, the program can run smoothly.

The above error only appears with ref size, with test/train sizes the program can run smoothly at optimization level of Fortran part `-Ofast`. 


## Comments

### Comment 1 - azuresky01

@heiher @wangleiat @SixWeining

---

### Comment 2 - SixWeining

Thanks for reporting the issue.

@tangaac Do you ever meet this issue? Is is caused by our recent optimizations?

@llvm/pr-subscribers-loongarch 

---

### Comment 3 - tangaac

> Thanks for reporting the issue.
> 
> [@tangaac](https://github.com/tangaac) Do you ever meet this issue? Is is caused by our recent optimizations?
> 
> [@llvm/pr-subscribers-loongarch](https://github.com/orgs/llvm/teams/pr-subscribers-loongarch)

Never met, I suspect it's not caused by our recent optimizations.

---

### Comment 4 - azuresky01

This issue and https://github.com/llvm/llvm-project/issues/137000:
 
* I did more tests on Debian sid/experimental x86_64/aarch64 installed with llvm 20.1.3. Both are OK with optimization setting `-Ofast`, so this two issues should be loongarch64 specific problems.

* And more tests on AOSC OS (12.1.3) loongarch64 with llvm 20.1.3 as well, I observed the same issues.

@SixWeining @tangaac

---

### Comment 5 - SixWeining

> This issue and [#137000](https://github.com/llvm/llvm-project/issues/137000):
> 
> * I did more tests on Debian sid/experimental x86_64/aarch64 installed with llvm 20.1.3. Both are OK with optimization setting `-Ofast`, so this two issues should be loongarch64 specific problems.
> * And more tests on AOSC OS (12.1.3) loongarch64 with llvm 20.1.3 as well, I observed the same issues.
> 
> [@SixWeining](https://github.com/SixWeining) [@tangaac](https://github.com/tangaac)

Thanks, we're trying to reproduce. BTW, have you tried the main branch? Or do you think it's a regression issue?

---

### Comment 6 - azuresky01

> > This issue and [#137000](https://github.com/llvm/llvm-project/issues/137000):
> > 
> > * I did more tests on Debian sid/experimental x86_64/aarch64 installed with llvm 20.1.3. Both are OK with optimization setting `-Ofast`, so this two issues should be loongarch64 specific problems.
> > * And more tests on AOSC OS (12.1.3) loongarch64 with llvm 20.1.3 as well, I observed the same issues.
> > 
> > [@SixWeining](https://github.com/SixWeining) [@tangaac](https://github.com/tangaac)
> 
> Thanks, we're trying to reproduce. BTW, have you tried the main branch? Or do you think it's a regression issue?

No, I have not yet tried the main branch. LLVM 20.1.4 will be out very soon, I will try that version on Debian when it is available. I think it should not be a regression issue, at least `-O3` should run correctly? Anyway, I appreciate very much your guys' work in this period. In LLVM 19 almost half of Fortran related SPEC2017 tests could not even compile. 

For your convenience, here I also provide the Portability setting I used in these two tests:

527.cam4_r,627.cam4_s: 
```
   CPORTABILITY  = -DSPEC_LP64 -DSPEC_CASE_FLAG -Wno-error=implicit-int
   FPORTABILITY  = -DSPEC_LP64 -DSPEC_CASE_FLAG
```

416.gamess:
```
PORTABILITY    = -DSPEC_CPU_LP64
```

---

### Comment 7 - SixWeining

What's version of SPEC2017 you used? Is it `1.1.9`?

---

### Comment 8 - azuresky01

> What's version of SPEC2017 you used? Is it `1.1.9`?

Yes. And CPU is 3A6000.

---

### Comment 9 - SixWeining

I have reproduced the issue with 20.1.3 while the main branch pass. I will bisect the commit that fix the issue.

---

### Comment 10 - azuresky01

> I have reproduced the issue with 20.1.3 while the main branch pass. I will bisect the commit that fix the issue.

I have tried the main branch [version 21.0.0git (https://github.com/llvm/llvm-project.git 173ec728d287f989053bd77521b3497b069c98a2)] and LLVM 20.1.4 (both are compiled from the source). Unfortunately with LLVM 20.1.4 I observed the same behavior as that with LLVM 20.1.3. And I confirm that LLVM 21.0.0git does fix the issue https://github.com/llvm/llvm-project/issues/136971 of spec2017 527.cam4_r. Is it possible to backport the commit that fix this issue to LLVM 20 series?

By the way, it seems that the issue https://github.com/llvm/llvm-project/issues/137000 of spec2006 416.gamess still remains with LLVM 21.0.0git. Actually it is worse since at `-Ofast` optimization level of Fortran part, tests with test/trains sizes also have miscomparison errors.

---

### Comment 11 - SixWeining

I found that https://github.com/llvm/llvm-project/pull/130686 fixed the issue for #136971. I'm not sure why x86-64/aarch64 are ok.

And, I find it is ok if I remove the `-flto=auto` option even without #130686.

For https://github.com/llvm/llvm-project/issues/137000, I'm still debugging. But I find that with assertion enabled, flang will crash when build 416.gamess:
```
flang: llvm-project/llvm/lib/Transforms/Vectorize/LoopVectorize.cpp:7730: llvm::VectorizationFactor llvm::LoopVectorizationPlanner::computeBestVF(): Assertion `(BestFactor.Width == LegacyVF.Width || BestPlan.hasEarlyExit() || planContainsAdditionalSimplifications(getPlanFor(BestFactor.Width), CostCtx, OrigLoop) || planContainsAdditionalSimplifications(getPlanFor(LegacyVF.Width), CostCtx, OrigLoop)) && " VPlan cost model and legacy cost model disagreed"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.      Program arguments: llvm-project/_build/bin/flang -fc1 -triple loongarch64-unknown-linux-gnu -emit-llvm-bc -flto=full -mrelocation-model pic -pic-level 2 -pic-is-pie -target-cpu la464 -target-feature +64bit -target-feature +f -target-feature +d -target-feature +lsx -target-feature +lasx -target-feature +ual -vectorize-loops -vectorize-slp -fversion-loops-for-stride -resource-dir llvm-project/_build/lib/clang/21 -mframe-pointer=none -O3 -o chgpen.fppized.o -x f95 chgpen.fppized.f
1.      Running pass "function<eager-inv>(float2int,lower-constant-intrinsics,chr,loop(loop-rotate<header-duplication;prepare-for-lto>,loop-deletion),loop-distribute,inject-tli-mappings,loop-vectorize<no-interleave-forced-only;no-vectorize-forced-only;>,infer-alignment,loop-load-elim,instcombine<max-iterations=1;no-verify-fixpoint>,simplifycfg<bonus-inst-threshold=1;forward-switch-cond;switch-range-to-icmp;switch-to-lookup;no-keep-loops;hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,slp-vectorizer,vector-combine,instcombine<max-iterations=1;no-verify-fixpoint>,loop-unroll<O3>,transform-warning,sroa<preserve-cfg>,infer-alignment,instcombine<max-iterations=1;no-verify-fixpoint>,loop-mssa(licm<allowspeculation>),alignment-from-assumptions,loop-sink,instsimplify,div-rem-pairs,tailcallelim,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-lookup;keep-loops;no-hoist-common-insts;hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;speculate-unpredictables>)" on module "FIRModule" 
2.      Running pass "loop-vectorize<no-interleave-forced-only;no-vectorize-forced-only;>" on function "cgpinp_" 
```

---

### Comment 12 - azuresky01

> I found that [#130686](https://github.com/llvm/llvm-project/pull/130686) fixed the issue for [#136971](https://github.com/llvm/llvm-project/issues/136971). I'm not sure why x86-64/aarch64 are ok.
> 
> And, I find it is ok if I remove the `-flto=auto` option even without [#130686](https://github.com/llvm/llvm-project/pull/130686).
> 

I can also confirm it is OK with LLVM 20.1.4 if `-flto=auto` option is removed.  So the issue of https://github.com/llvm/llvm-project/issues/136971 is related to conversion of floating point values into integers.  

---

### Comment 13 - SixWeining

> I can also confirm it is OK with LLVM 20.1.4 if `-flto=auto` option is removed. So the issue of [#136971](https://github.com/llvm/llvm-project/issues/136971) is related to conversion of floating point values into integers.

Seem so. But I'm not sure how `-flto=atuo` is related with floating point conversion.

---

### Comment 14 - SixWeining

This issue can be fixed by https://github.com/llvm/llvm-project/pull/137129 which has been cherry-picked to llvm20: https://github.com/llvm/llvm-project/pull/139851.

---

### Comment 15 - azuresky01

Great! Thanks very much for your guys' nice work. Now SPEC2017 tests should be OK on LoongArch with LLVM 20. 

Update: I just tested 527.cam4_r with LLVM 20.1.5 plus https://github.com/llvm/llvm-project/pull/139851, it runs smoothly. So the issue has been solved. 

---

