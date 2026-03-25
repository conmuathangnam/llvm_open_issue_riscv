# [VPlan] f521.wrf_r asserts in loop vectorizer " VPlan cost model and legacy cost model disagreed"

**Author:** anoopkg6
**URL:** https://github.com/llvm/llvm-project/issues/177780
**Status:** Closed
**Labels:** vectorizers, crash
**Closed Date:** 2026-02-08T16:16:06Z

## Body

f521.wrf_r benchmark asserts in loop vectorizer with message " VPlan cost model and legacy cost model disagreed" like #177772. They might be related but it shows up in different commit.

```
commit a51e2ef0fe73dd9ab6e608304ddf2b489c350cf4 (HEAD)
Author: Florian Hahn <flo@fhahn.com>
Date:   Tue Nov 25 14:46:30 2025 +0000

    [VPlan] Treat VPVector(End)PointerRecipe as single-scalar, if ops are. (#169249)

    VPVector(End)PointerRecipes are single-scalar if all their operands are.
    This should be effectively NFC currently, but it should re-enable cost
    checking for some more VPWidenMemoryRecipe after
    https://github.com/llvm/llvm-project/pull/157387 as discovered by
    John Brawn.
```

This issue can be reproduced with -O3.

```
% opt -O3 wrf_r_reduced.ll
opt: /home/anoopk/fllvm/llvm-project/llvm/lib/Transforms/Vectorize/LoopVectorize.cpp:7276: llvm::VectorizationFactor llvm::LoopVectorizationPlanner::computeBestVF(): Assertion `(BestFactor.Width == LegacyVF.Width || BestPlan.hasEarlyExit() || !Legal->getLAI()->getSymbolicStrides().empty() || UsesEVLGatherScatter || planContainsAdditionalSimplifications( getPlanFor(BestFactor.Width), CostCtx, OrigLoop, BestFactor.Width) || planContainsAdditionalSimplifications( getPlanFor(LegacyVF.Width), CostCtx, OrigLoop, LegacyVF.Width)) && " VPlan cost model and legacy cost model disagreed"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: opt -O3 wrf_r_reduced.ll
1.      Running pass "function<eager-inv>(drop-unnecessary-assumes,float2int,lower-constant-intrinsics,chr,loop(loop-rotate<header-duplication;no-prepare-for-lto>,loop-deletion),loop-distribute,inject-tli-mappings,loop-vectorize<no-interleave-forced-only;no-vectorize-forced-only;>,drop-unnecessary-assumes,infer-alignment,loop-load-elim,instcombine<max-iterations=1;no-verify-fixpoint>,simplifycfg<bonus-inst-threshold=1;forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;switch-to-lookup;no-keep-loops;hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,slp-vectorizer,vector-combine,instcombine<max-iterations=1;no-verify-fixpoint>,loop-unroll<O3>,transform-warning,sroa<preserve-cfg>,infer-alignment,instcombine<max-iterations=1;no-verify-fixpoint>,loop-mssa(licm<allowspeculation>),alignment-from-assumptions,loop-sink,instsimplify,div-rem-pairs,tailcallelim,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;speculate-unpredictables>)" on module "wrf_r_reduced.ll"
2.      Running pass "loop-vectorize<no-interleave-forced-only;no-vectorize-forced-only;>" on function "_QMmodule_cu_tiedtkePcutype"
 #0 0x00000000046cdc64 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/home/anoopk/fllvm/llvm-project/build/bin/opt+0x46cdc64)
 #1 0x00000000046caa74 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x000003ffec6fe4e0 (linux-vdso.so.1+0x4e0)
 #3 0x000003ff99db07d0 __pthread_kill_implementation (/lib64/libc.so.6+0xb07d0)
 #4 0x000003ff99d550c0 gsignal (/lib64/libc.so.6+0x550c0)
 #5 0x000003ff99d34242 abort (/lib64/libc.so.6+0x34242)
 #6 0x000003ff99d4bdcc __assert_fail_base (/lib64/libc.so.6+0x4bdcc)
 #7 0x000003ff99d4c0e4 __assert_fail (/lib64/libc.so.6+0x4c0e4)
 #8 0x0000000001f0ccd8 llvm::LoopVectorizationPlanner::computeBestVF() (/home/anoopk/fllvm/llvm-project/build/bin/opt+0x1f0ccd8)
 #9 0x0000000001f0eb08 llvm::LoopVectorizePass::processLoop(llvm::Loop*) (/home/anoopk/fllvm/llvm-project/build/bin/opt+0x1f0eb08)
#10 0x0000000001f11cd2 llvm::LoopVectorizePass::runImpl(llvm::Function&) (/home/anoopk/fllvm/llvm-project/build/bin/opt+0x1f11cd2)
#11 0x0000000001f1226c llvm::LoopVectorizePass::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/home/anoopk/fllvm/llvm-project/build/bin/opt+0x1f1226c)
#12 0x0000000001655caa llvm::detail::PassModel<llvm::Function, llvm::LoopVectorizePass, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/home/anoopk/fllvm/llvm-project/build/bin/opt+0x1655caa)
#13 0x00000000043e9736 llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/home/anoopk/fllvm/llvm-project/build/bin/opt+0x43e9736)
#14 0x000000000165755a llvm::detail::PassModel<llvm::Function, llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/home/anoopk/fllvm/llvm-project/build/bin/opt+0x165755a)
#15 0x00000000043ea17c llvm::ModuleToFunctionPassAdaptor::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/home/anoopk/fllvm/llvm-project/build/bin/opt+0x43ea17c)
#16 0x0000000001421b9a llvm::detail::PassModel<llvm::Module, llvm::ModuleToFunctionPassAdaptor, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/home/anoopk/fllvm/llvm-project/build/bin/opt+0x1421b9a)
#17 0x00000000043ec526 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/home/anoopk/fllvm/llvm-project/build/bin/opt+0x43ec526)
#18 0x000000000142de8e llvm::runPassPipeline(llvm::StringRef, llvm::Module&, llvm::TargetMachine*, llvm::TargetLibraryInfoImpl*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::StringRef, llvm::ArrayRef<llvm::PassPlugin>, llvm::ArrayRef<std::function<void (llvm::PassBuilder&)>>, llvm::opt_tool::OutputKind, llvm::opt_tool::VerifierKind, bool, bool, bool, bool, bool, bool, bool, bool) (/home/anoopk/fllvm/llvm-project/build/bin/opt+0x142de8e)
#19 0x000000000141fe8c optMain (/home/anoopk/fllvm/llvm-project/build/bin/opt+0x141fe8c)
#20 0x00000000013359d2 main (/home/anoopk/fllvm/llvm-project/build/bin/opt+0x13359d2)
#21 0x000003ff99d348cc __libc_start_call_main (/lib64/libc.so.6+0x348cc)
#22 0x000003ff99d349c6 __libc_start_main@GLIBC_2.2 (/lib64/libc.so.6+0x349c6)
#23 0x0000000001414ada _start (/home/anoopk/fllvm/llvm-project/build/bin/opt+0x1
```

[wrf_r.zip](https://github.com/user-attachments/files/24838115/wrf_r.zip)
wrf_r_reduced.ll is produced by llvm-reduced on asserting function ir in wrf_r_test.ll and original fortran function is in wrf_r_test.f90 from f521.wrf_r.

## Comments

### Comment 1 - anoopkg6

@uweigand @dominik-steenken 

---

### Comment 2 - EugeneZelenko

@anoopkg6: Please use code block syntax for stack traces, code snippets, etc.

---

### Comment 3 - anoopkg6

Thanks. I will do it next time.

---

### Comment 4 - ChaseYalon

I think the issue is with the way the legacy cost model assumes that the pointer math is vectorized but VPlan thinks it is scalar.

---

### Comment 5 - ChaseYalon

I will try and fix it later today

---

