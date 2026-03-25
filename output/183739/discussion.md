# opt: [...] LoopVectorize.cpp:7335: [...] Assertion `[...] " VPlan cost model and legacy cost model disagreed"' failed.

**Author:** mikaelholmen
**URL:** https://github.com/llvm/llvm-project/issues/183739
**Status:** Open
**Labels:** vectorizers, crash, needs-reduction

## Body

llvm commit: fc69531254c
Reproduce with: ```opt -passes=loop-vectorize -mtriple=riscv64-linux-gnu -mattr=+v -scalable-vectorization=off bbi-115486_riscv.ll -S -o /dev/null```
Result:
```
opt: /repo/uabelho/main-github/llvm/lib/Transforms/Vectorize/LoopVectorize.cpp:7335: VectorizationFactor llvm::LoopVectorizationPlanner::computeBestVF(): Assertion `(BestFactor.Width == LegacyVF.Width || BestPlan.hasEarlyExit() || !Legal->getLAI()->getSymbolicStrides().empty() || UsesEVLGatherScatter || planContainsAdditionalSimplifications( getPlanFor(BestFactor.Width), CostCtx, OrigLoop, BestFactor.Width) || planContainsAdditionalSimplifications( getPlanFor(LegacyVF.Width), CostCtx, OrigLoop, LegacyVF.Width)) && " VPlan cost model and legacy cost model disagreed"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: build-all-no-miracle/bin/opt -passes=loop-vectorize -mtriple=riscv64-linux-gnu -mattr=+v -scalable-vectorization=off bbi-115486_riscv.ll -S -o /dev/null
1.	Running pass "function(loop-vectorize<no-interleave-forced-only;no-vectorize-forced-only;>)" on module "bbi-115486_riscv.ll"
2.	Running pass "loop-vectorize<no-interleave-forced-only;no-vectorize-forced-only;>" on function "f"
 #0 0x000056389749bed6 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (build-all-no-miracle/bin/opt+0x50f0ed6)
 #1 0x0000563897498b85 llvm::sys::RunSignalHandlers() (build-all-no-miracle/bin/opt+0x50edb85)
 #2 0x000056389749d0e9 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x00007fc25e0cb990 __restore_rt (/lib64/libpthread.so.0+0x12990)
 #4 0x00007fc25ba6a52f raise (/lib64/libc.so.6+0x4e52f)
 #5 0x00007fc25ba3de65 abort (/lib64/libc.so.6+0x21e65)
 #6 0x00007fc25ba3dd39 _nl_load_domain.cold.0 (/lib64/libc.so.6+0x21d39)
 #7 0x00007fc25ba62e86 (/lib64/libc.so.6+0x46e86)
 #8 0x0000563898b2f82c (build-all-no-miracle/bin/opt+0x678482c)
 #9 0x0000563898b3d26f llvm::LoopVectorizePass::processLoop(llvm::Loop*) (build-all-no-miracle/bin/opt+0x679226f)
#10 0x0000563898b4816b llvm::LoopVectorizePass::runImpl(llvm::Function&) (build-all-no-miracle/bin/opt+0x679d16b)
#11 0x0000563898b48a72 llvm::LoopVectorizePass::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (build-all-no-miracle/bin/opt+0x679da72)
#12 0x0000563898a00d9d llvm::detail::PassModel<llvm::Function, llvm::LoopVectorizePass, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) PassBuilderPipelines.cpp:0:0
#13 0x00005638976c7f15 llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (build-all-no-miracle/bin/opt+0x531cf15)
#14 0x00005638989fe2bd llvm::detail::PassModel<llvm::Function, llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) PassBuilderPipelines.cpp:0:0
#15 0x00005638976cc82e llvm::ModuleToFunctionPassAdaptor::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (build-all-no-miracle/bin/opt+0x532182e)
#16 0x000056389898bb5d llvm::detail::PassModel<llvm::Module, llvm::ModuleToFunctionPassAdaptor, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) NewPMDriver.cpp:0:0
#17 0x00005638976c6c05 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (build-all-no-miracle/bin/opt+0x531bc05)
#18 0x0000563898984061 llvm::runPassPipeline(llvm::StringRef, llvm::Module&, llvm::TargetMachine*, llvm::TargetLibraryInfoImpl*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::StringRef, llvm::ArrayRef<llvm::PassPlugin>, llvm::ArrayRef<std::function<void (llvm::PassBuilder&)>>, llvm::opt_tool::OutputKind, llvm::opt_tool::VerifierKind, bool, bool, bool, bool, bool, bool, bool, bool) (build-all-no-miracle/bin/opt+0x65d9061)
#19 0x000056389743b5a7 optMain (build-all-no-miracle/bin/opt+0x50905a7)
#20 0x00007fc25ba567e5 __libc_start_main (/lib64/libc.so.6+0x3a7e5)
#21 0x00005638974387ee _start (build-all-no-miracle/bin/opt+0x508d7ee)
Abort (core dumped)
```
[bbi-115486_riscv.ll.gz](https://github.com/user-attachments/files/25604845/bbi-115486_riscv.ll.gz)
https://godbolt.org/z/v1j69vW84

Originally found for our out-of-tree target, then I fiddled a lot to get something similar to crash for riscv.

## Comments

### Comment 1 - mikaelholmen

This starts failing with 2753e1dedfff
```[RISCV] Set the reciprocal throughtput cost for division to TTI::TCC_Expensive (#177516)```
but I suspect that patch just exposes a LoopVectorizer issue.

My original problem started showing up with a51e2ef0fe7
```[VPlan] Treat VPVector(End)PointerRecipe as single-scalar, if ops are. (#169249)```
but I also suspect that just happened to expose an existing problem.

---

### Comment 2 - mikaelholmen

What does the "needs-reduction" label mean?

For sure it would be nicer with a smaller repro, but I want to point out that the bbi-115486_riscv.ll repro is what I get from llvm-reduce.

---

### Comment 3 - yrouban

I have a bit shorter reproducer https://godbolt.org/z/7qdEfYe6K

Reproduce with: `opt -passes=loop-vectorize loop-vectorize-old-new-vplan-disagreed.ll -disable-output`

loop-vectorize-old-new-vplan-disagreed.ll:
```llvm
target triple = "x86_64-unknown-linux-gnu"

define void @test(i32 %elem, ptr addrspace(1) %nowhere) "target-features"="+avx" {
entry:
  %0 = ashr i32 %elem, 24
  %1 = mul nsw i32 %0, 123
  %2 = call i32 @llvm.smax.i32(i32 %1, i32 15745)
  %3 = trunc i32 %2 to i8
  br label %loop

loop:
  %indvars.iv = phi i64 [ %indvars.iv.next, %loop ], [ 1, %entry ]
  %elem_gep = getelementptr i8, ptr addrspace(1) %nowhere, i64 %indvars.iv
  store i8 %3, ptr addrspace(1) %elem_gep, align 1
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 73; passes with 72
  br i1 %exitcond.not, label %bb3, label %loop

bb3:
  ret void
}

; Function Attrs: nocallback nocreateundeforpoison nofree nosync nounwind speculatable willreturn memory(none)
declare i32 @llvm.smax.i32(i32, i32) nocallback nocreateundeforpoison nofree nosync nounwind speculatable willreturn memory(none)

```

---

### Comment 4 - KennethHilmersson

I see 2 different problems.

1.  Upstream commit 045fdda39d43 switched VPlan memory op
    costing from TTI::getOperandInfo on the IR operand to
    Ctx.getOperandInfo on the VPlan value, which can see
    VPlan simplifications. Upstream commit 53cf22f3a1 then
    improved SCEV enough to simplify loop-invariant store
    values to constants, exposing a cost mismatch between
    VPlan and the legacy model.
(the above reproducer by [yrouban](https://github.com/yrouban))

To match this something like this is needed in the legacy model:
```c++
         MemIntrinsicCostAttributes(IID, VectorTy, Alignment, AS), CostKind);
   } else {
     TTI::OperandValueInfo OpInfo = TTI::getOperandInfo(I->getOperand(0));
+    // Match VPlan cost model: if SCEV can simplify the stored value to a
+    // constant, use that for operand info.
+    if (auto *SI = dyn_cast<StoreInst>(I)) {
+      Value *SV = SI->getValueOperand();
+      if (PSE.getSE()->isSCEVable(SV->getType()))
+        if (auto *C = dyn_cast<SCEVConstant>(PSE.getSE()->getSCEV(SV)))
+          OpInfo = TTI::getOperandInfo(C->getValue());
+    }
     Cost += TTI.getMemoryOpCost(I->getOpcode(), VectorTy, Alignment, AS,
                                 CostKind, OpInfo, I);
   }
```

2. Introduced in commit d606eae2ced1:
It changed VPWidenRecipe to stop using legacy cost for div/rem operations and
added select cost computation to VPInstruction. It then added logic to SKIP
safe divisor selects in the legacy model.
(the RiscV reproducer by [mikaelholmen](https://github.com/mikaelholmen))

This is more complicated to match in the legacy model since selects does not scale with VF for 
scalable vectors on AArch64:
```c++
          case Instruction::Select: {
            bool IsSafeDivisorSel = false;
            if (auto *WR =
                    dyn_cast_or_null<VPWidenRecipe>(VPI->getSingleUser())) {
              unsigned Opc = WR->getOpcode();
              IsSafeDivisorSel = Opc == Instruction::UDiv ||
                                 Opc == Instruction::SDiv ||
                                 Opc == Instruction::URem ||
                                 Opc == Instruction::SRem;
            }
            if (IsSafeDivisorSel) {
              auto SelectCost = VPI->cost(VF, CostCtx);
              // Some targets return non-scaling select costs which breaks
              // cost-per-lane calculations for scalable vectors.
              if (VF.isScalable() && VF.getKnownMinValue() > 2) {
                ElementCount RefVF = ElementCount::getScalable(2);
                if (SelectCost == VPI->cost(RefVF, CostCtx))
                  break;
              }
              C += SelectCost;
            } else {
              C += VPI->cost(VF, CostCtx);
            }
            break;
```
@fhahn @david-arm


---

### Comment 5 - fhahn

sorry for the delay, taking a look

---

