# [LoopVectorize] Assertion `Val->isVectorTy() && "This must be a vector type"' failed.

**Author:** TatyanaDoubts
**URL:** https://github.com/llvm/llvm-project/issues/170626
**Status:** Closed
**Labels:** vectorizers, crash
**Closed Date:** 2025-12-15T09:46:29Z

## Body

To reproduce run opt with the test below (-passes=loop-vectorize):
```
; ModuleID = './reduced.ll'
source_filename = "./reduced.ll"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128-ni:1-p2:32:8:8:32-ni:2"
target triple = "x86_64-unknown-linux-gnu"

define void @wombat(i32 %arg) gc "statepoint-example" {
bb:
  br label %bb1

bb1:                                              ; preds = %bb2, %bb
  br label %bb3

bb2:                                              ; preds = %bb3
  %phi = phi i32 [ %phi5, %bb3 ]
  br label %bb1

bb3:                                              ; preds = %bb3, %bb1
  %phi4 = phi i32 [ %arg, %bb1 ], [ %add, %bb3 ]
  %phi5 = phi i32 [ 0, %bb1 ], [ %zext, %bb3 ]
  %trunc = trunc i32 %phi4 to i8
  %udiv = udiv i8 %trunc, 0
  %zext = zext i8 %udiv to i32
  %add = add i32 %phi4, 1
  %icmp = icmp ugt i32 %phi4, 1
  br i1 %icmp, label %bb2, label %bb3, !prof !0
}

!0 = !{!"branch_weights", i32 1, i32 0}
```
Reproducer: https://godbolt.org/z/9nMjd3ejx

Stack dump:
```
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/opt -o /app/output.s -S -passes=loop-vectorize <source>
1.	Running pass "function(loop-vectorize<no-interleave-forced-only;no-vectorize-forced-only;>)" on module "<source>"
2.	Running pass "loop-vectorize<no-interleave-forced-only;no-vectorize-forced-only;>" on function "wombat"
 #0 0x00000000059e25a8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x59e25a8)
 #1 0x00000000059df454 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x0000717c5de42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x0000717c5de969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x0000717c5de42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x0000717c5de287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x0000717c5de2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #7 0x0000717c5de39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #8 0x000000000230cea6 llvm::X86TTIImpl::getVectorInstrCost(unsigned int, llvm::Type*, llvm::TargetTransformInfo::TargetCostKind, unsigned int, llvm::Value const*, llvm::Value const*) const (.constprop.0) X86TargetTransformInfo.cpp:0:0
 #9 0x0000000005082b92 llvm::TargetTransformInfo::getIndexedVectorInstrCostFromEnd(unsigned int, llvm::Type*, llvm::TargetTransformInfo::TargetCostKind, unsigned int) const (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5082b92)
#10 0x00000000038c8bbf llvm::VPInstruction::computeCost(llvm::ElementCount, llvm::VPCostContext&) const (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x38c8bbf)
#11 0x00000000038bc986 llvm::VPRecipeBase::cost(llvm::ElementCount, llvm::VPCostContext&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x38bc986)
#12 0x000000000386fd44 llvm::VPBasicBlock::cost(llvm::ElementCount, llvm::VPCostContext&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x386fd44)
#13 0x000000000371ea4d llvm::LoopVectorizePass::processLoop(llvm::Loop*) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x371ea4d)
#14 0x000000000371f040 llvm::LoopVectorizePass::runImpl(llvm::Function&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x371f040)
#15 0x000000000371f76b llvm::LoopVectorizePass::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x371f76b)
#16 0x00000000030002ce llvm::detail::PassModel<llvm::Function, llvm::LoopVectorizePass, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x30002ce)
#17 0x00000000057c0391 llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x57c0391)
#18 0x0000000000f0c2ae llvm::detail::PassModel<llvm::Function, llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0xf0c2ae)
#19 0x00000000057bea5a llvm::ModuleToFunctionPassAdaptor::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x57bea5a)
#20 0x00000000009783ce llvm::detail::PassModel<llvm::Module, llvm::ModuleToFunctionPassAdaptor, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x9783ce)
#21 0x00000000057be411 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x57be411)
#22 0x000000000098261a llvm::runPassPipeline(llvm::StringRef, llvm::Module&, llvm::TargetMachine*, llvm::TargetLibraryInfoImpl*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::StringRef, llvm::ArrayRef<llvm::PassPlugin>, llvm::ArrayRef<std::function<void (llvm::PassBuilder&)>>, llvm::opt_tool::OutputKind, llvm::opt_tool::VerifierKind, bool, bool, bool, bool, bool, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x98261a)
#23 0x00000000009765d8 optMain (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x9765d8)
#24 0x0000717c5de29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#25 0x0000717c5de29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#26 0x000000000096d085 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x96d085)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

## Comments

### Comment 1 - TatyanaDoubts

The suspected fault commit for this bug is
commit 72e51d389f66d9cc6b55fd74b56fbbd087672a43
Author: Florian Hahn <flo@fhahn.com>
Date:   Wed Nov 26 19:31:24 2025 +0000

    Reapply "[LV] Use ExtractLane(LastActiveLane, V) live outs when tail-folding. (#149042)"

    This reverts commit a6edeedbfa308876d6f2b1648729d52970bb07e6.

    The following fixes have landed, addressing issues causing the original
    revert:
    * https://github.com/llvm/llvm-project/pull/169298
    * https://github.com/llvm/llvm-project/pull/167897
    * https://github.com/llvm/llvm-project/pull/168949

    Original message:
    Building on top of https://github.com/llvm/llvm-project/pull/148817,
    introduce a new abstract LastActiveLane opcode that gets lowered to
    Not(Mask) → FirstActiveLane(NotMask) → Sub(result, 1).

    When folding the tail, update all extracts for uses outside the loop the
    extract the value of the last actice lane.

    See also https://github.com/llvm/llvm-project/issues/148603

    PR: https://github.com/llvm/llvm-project/pull/149042


---

### Comment 2 - lukel97

I can't reproduce this on 96881c1226a532f94231ddb35c514109fa254df1, and on compiler explorer it also seems to be fixed with opt (assertions trunk), so presume it's been fixed in the meantime. Can you reopen this issue if it still persists on your end?

---

