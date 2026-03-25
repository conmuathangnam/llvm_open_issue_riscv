# Assertion `TheLoop->contains(BB) && "Unknown block used"' failed.

**Author:** TatyanaDoubts
**URL:** https://github.com/llvm/llvm-project/issues/174760
**Status:** Closed
**Labels:** loopoptim, crash
**Closed Date:** 2026-01-19T20:24:17Z

## Body

To reproduce run opt with the test below (-passes=loop-load-elim)
```
; ModuleID = './reduced.ll'
source_filename = "./reduced.ll"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128-ni:1-p2:32:8:8:32-ni:2"
target triple = "x86_64-unknown-linux-gnu"

define i32 @wombat() gc "statepoint-example" {
bb:
  br label %bb1

bb1:                                              ; preds = %bb2, %bb
  %phi = phi i64 [ 1, %bb ], [ %add, %bb2 ]
  br label %bb2

bb2:                                              ; preds = %bb1
  %and = and i64 %phi, 1
  %getelementptr = getelementptr nusw i32, ptr addrspace(1) null, i64 %and
  store i32 0, ptr addrspace(1) %getelementptr, align 4
  store i32 0, ptr addrspace(1) null, align 4
  %add = add i64 %phi, 1
  %icmp = icmp ult i64 %phi, 1
  br i1 %icmp, label %bb3, label %bb1

bb3:                                              ; preds = %bb2
  %load = load i32, ptr addrspace(1) %getelementptr, align 4
  ret i32 0
}
```
Reproducer:  https://godbolt.org/z/dfa7ar8dx
Stack dump:
```
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/opt -o /app/output.s -S -passes=loop-load-elim <source>
1.	Running pass "function(loop-load-elim)" on module "<source>"
2.	Running pass "loop-load-elim" on function "wombat"
 #0 0x0000000005a7f2e8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5a7f2e8)
 #1 0x0000000005a7c194 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x000074b4a6642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x000074b4a66969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x000074b4a6642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x000074b4a66287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x000074b4a662871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #7 0x000074b4a6639e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #8 0x0000000004f74f5f llvm::LoopAccessInfo::blockNeedsPredication(llvm::BasicBlock const*, llvm::Loop const*, llvm::DominatorTree const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x4f74f5f)
 #9 0x0000000004f7522a isNoWrap(llvm::PredicatedScalarEvolution&, llvm::SCEVAddRecExpr const*, llvm::Value*, llvm::Type*, llvm::Loop const*, bool, llvm::DominatorTree const&, std::optional<long>) LoopAccessAnalysis.cpp:0:0
#10 0x0000000004f7dd6b (anonymous namespace)::AccessAnalysis::createCheckForAccess(llvm::RuntimePointerChecking&, llvm::PointerIntPair<llvm::Value*, 1u, bool, llvm::PointerLikeTypeTraits<llvm::Value*>, llvm::PointerIntPairInfo<llvm::Value*, 1u, llvm::PointerLikeTypeTraits<llvm::Value*>>>, llvm::Type*, llvm::DenseMap<llvm::Value*, llvm::SCEV const*, llvm::DenseMapInfo<llvm::Value*, void>, llvm::detail::DenseMapPair<llvm::Value*, llvm::SCEV const*>> const&, llvm::DenseMap<llvm::Value*, unsigned int, llvm::DenseMapInfo<llvm::Value*, void>, llvm::detail::DenseMapPair<llvm::Value*, unsigned int>>&, llvm::Loop*, unsigned int&, unsigned int, bool) LoopAccessAnalysis.cpp:0:0
#11 0x0000000004f85566 (anonymous namespace)::AccessAnalysis::canCheckPtrAtRT(llvm::RuntimePointerChecking&, llvm::Loop*, llvm::DenseMap<llvm::Value*, llvm::SCEV const*, llvm::DenseMapInfo<llvm::Value*, void>, llvm::detail::DenseMapPair<llvm::Value*, llvm::SCEV const*>> const&, llvm::Value*&, bool) (.part.0) LoopAccessAnalysis.cpp:0:0
#12 0x0000000004f8a01e llvm::LoopAccessInfo::analyzeLoop(llvm::AAResults*, llvm::LoopInfo const*, llvm::TargetLibraryInfo const*, llvm::DominatorTree*) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x4f8a01e)
#13 0x0000000004f8ab82 llvm::LoopAccessInfo::LoopAccessInfo(llvm::Loop*, llvm::ScalarEvolution*, llvm::TargetTransformInfo const*, llvm::TargetLibraryInfo const*, llvm::AAResults*, llvm::DominatorTree*, llvm::LoopInfo*, llvm::AssumptionCache*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x4f8ab82)
#14 0x0000000004f8aeba llvm::LoopAccessInfoManager::getInfo(llvm::Loop&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x4f8aeba)
#15 0x00000000047bcc79 llvm::LoopLoadEliminationPass::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x47bcc79)
#16 0x0000000003043cae llvm::detail::PassModel<llvm::Function, llvm::LoopLoadEliminationPass, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x3043cae)
#17 0x0000000005859c61 llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5859c61)
#18 0x0000000000f1ba3e llvm::detail::PassModel<llvm::Function, llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0xf1ba3e)
#19 0x000000000585821a llvm::ModuleToFunctionPassAdaptor::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x585821a)
#20 0x000000000097ee5e llvm::detail::PassModel<llvm::Module, llvm::ModuleToFunctionPassAdaptor, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x97ee5e)
#21 0x0000000005857bd1 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5857bd1)
#22 0x0000000000989110 llvm::runPassPipeline(llvm::StringRef, llvm::Module&, llvm::TargetMachine*, llvm::TargetLibraryInfoImpl*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::StringRef, llvm::ArrayRef<llvm::PassPlugin>, llvm::ArrayRef<std::function<void (llvm::PassBuilder&)>>, llvm::opt_tool::OutputKind, llvm::opt_tool::VerifierKind, bool, bool, bool, bool, bool, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x989110)
#23 0x000000000097d038 optMain (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x97d038)
#24 0x000074b4a6629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#25 0x000074b4a6629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#26 0x0000000000973a35 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x973a35)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

## Comments

### Comment 1 - TatyanaDoubts

Hi Florian @fhahn 
It looks like the fault commit for this crash is
commit af9a4263a1a209953a1d339ef781a954e31268ff

Author: Florian Hahn <[flo@fhahn.com](mailto:flo@fhahn.com)>

Date:   Tue Nov 4 17:08:12 2025 +0000

 

    [LAA] Only use inbounds/nusw in isNoWrap if the GEP is dereferenced. (#161445)

 

    Update isNoWrap to only use the inbounds/nusw flags from GEPs that are

    guaranteed to be dereferenced on every iteration. This fixes a case

    where we incorrectly determine no dependence.

 

    I think the issue is isolated to code that evaluates the resulting

    AddRec at BTC, just using it to compute the distance between accesses

    should still be fine; if the access does not execute in a given

    iteration, there's no dependence in that iteration. But isolating the

    code is not straight-forward, so be conservative for now. The practical

    impact should be very minor (only one loop changed across a corpus with

    27k modules from large C/C++ workloads.

 

    Fixes [[LAA] Missing dependence due to AddRec wrap · Issue #160912 · llvm/llvm-project](https://github.com/llvm/llvm-project/issues/160912) .

 

    PR: [[LAA] Only use inbounds/nusw in isNoWrap if the GEP is dereferenced. by fhahn · Pull Request #161445 · llvm/llvm-project](https://github.com/llvm/llvm-project/pull/161445)

---

