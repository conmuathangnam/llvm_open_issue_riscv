# Illegal Instruction Fault From Improper CPU Feature Detection

**Author:** StDymphna
**URL:** https://github.com/llvm/llvm-project/issues/130509
**Status:** Closed
**Labels:** llvm:codegen, miscompilation
**Closed Date:** 2025-11-24T22:09:43Z

## Body

**LLVM Versions Tested**: 18, 19, 20, current tip

### Issue Summary
LLVM components incorrectly detect supported CPU instructions, resulting in the generation of illegal instructions, resulting in faulting binaries.

### Detailed Description
The compiler infrastructure appears to use static CPU model mappings to determine available instruction sets, instead of querying what the CPU actually supports (via `/proc/cpuinfo` or hwcap on AArch64). This causes particular problems on:

1. ~~Arm-V9 CPUs from Qualcomm SoCs that do not implement SVE despite the Arm-V9 specification requiring it~~ CPU IPs that are individually speced to require SVE, SVE2, etc., but that do not (notably(some/many/all?) Qualcomm SoCs).
2. Potentially any system where the Linux kernel is not configured with `CONFIG_ARM64_SVE=Y`

But is not necessarily limited to AArch64 or the above.

This affects instruction selection/codegen/runtime dispatching for all of LLVM like Clang, Flang, OpenMP, ORC JIT, etc.

### Reproduction Steps
Below is a relatively minimal test case using ORC JIT that demonstrates the issue. A Termux environment on Android devices using Qualcomm chips is likely the easiest target for reproduction. This could also be reproduced with a vectorizable loop in C code, with Clang, using the "-march=native" flag.

```cpp
#include <iostream>
#include <vector>
#include <string>

#include "llvm/ExecutionEngine/Orc/LLJIT.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/PassManager.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/TargetParser/Host.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Analysis/TargetTransformInfo.h"
#include "llvm/Target/TargetMachine.h"

using namespace llvm;
using namespace llvm::orc;

// Diagnostic handler to suppress remarks
class SilenceRemarksHandler : public DiagnosticHandler {
public:
  bool handleDiagnostics(const DiagnosticInfo &DI) override {
    // Ignore remarks, pass through other diagnostics
    if (DI.getSeverity() == DS_Remark) {
      return true;
    }
    return false;
  }
};

std::unique_ptr<Module> createVectorModule(LLVMContext &Context) {
  auto M = std::make_unique<Module>("VecTest", Context);
  
  auto *FloatTy = Type::getFloatTy(Context);
  auto *FloatPtrTy = PointerType::get(FloatTy, 0);
  auto *Int32Ty = Type::getInt32Ty(Context);
  
  FunctionType *FT = FunctionType::get(
      Type::getVoidTy(Context),
      {FloatPtrTy, FloatPtrTy, FloatPtrTy, Int32Ty},
      false);
  
  Function *F = Function::Create(FT, Function::ExternalLinkage, "vector_op", M.get());
  F->addFnAttr(Attribute::NoUnwind);
  
  auto Args = F->arg_begin();
  Value *A = &*Args++;
  Value *B = &*Args++;
  Value *Result = &*Args++;
  Value *Length = &*Args++;
  
  BasicBlock *EntryBB = BasicBlock::Create(Context, "entry", F);
  BasicBlock *LoopBB = BasicBlock::Create(Context, "loop", F);
  BasicBlock *ExitBB = BasicBlock::Create(Context, "exit", F);
  
  IRBuilder<> Builder(Context);
  
  Builder.SetInsertPoint(EntryBB);
  Value *IndexAlloca = Builder.CreateAlloca(Int32Ty, nullptr, "i");
  Builder.CreateStore(ConstantInt::get(Int32Ty, 0), IndexAlloca);
  Builder.CreateBr(LoopBB);
  
  Builder.SetInsertPoint(LoopBB);
  Value *Index = Builder.CreateLoad(Int32Ty, IndexAlloca, "idx");
  Value *LoopCond = Builder.CreateICmpSLT(Index, Length, "cond");
  
  Value *APtr = Builder.CreateGEP(FloatTy, A, Index, "a_ptr");
  Value *BPtr = Builder.CreateGEP(FloatTy, B, Index, "b_ptr");
  Value *ResultPtr = Builder.CreateGEP(FloatTy, Result, Index, "result_ptr");
  
  MDNode *AccessGroup = MDNode::get(Context, {});
  
  Value *AVal = Builder.CreateLoad(FloatTy, APtr, "a_val");
  Value *BVal = Builder.CreateLoad(FloatTy, BPtr, "b_val");
  
  cast<Instruction>(AVal)->setMetadata("llvm.mem.parallel_loop_access", AccessGroup);
  cast<Instruction>(BVal)->setMetadata("llvm.mem.parallel_loop_access", AccessGroup);
  
  Value *Square = Builder.CreateFMul(AVal, AVal, "square");
  Value *AddResult = Builder.CreateFAdd(Square, BVal, "add");
  
  auto *StoreInst = Builder.CreateStore(AddResult, ResultPtr);
  StoreInst->setMetadata("llvm.mem.parallel_loop_access", AccessGroup);
  
  Value *NextIndex = Builder.CreateAdd(Index, ConstantInt::get(Int32Ty, 1), "next_idx");
  Builder.CreateStore(NextIndex, IndexAlloca);
  
  // Loop metadata to force vectorization
  MDNode *ForcedVec = MDNode::get(Context, {
      MDString::get(Context, "llvm.loop.vectorize.enable"),
      ConstantAsMetadata::get(ConstantInt::get(Type::getInt1Ty(Context), 1))
  });
  
  MDNode *LoopID = MDNode::get(Context, {MDNode::get(Context, {}), ForcedVec});
  LoopID->replaceOperandWith(0, LoopID);
  
  Builder.CreateCondBr(LoopCond, LoopBB, ExitBB)->setMetadata("llvm.loop", LoopID);
  Builder.SetInsertPoint(ExitBB);
  Builder.CreateRetVoid();
  
  verifyFunction(*F);
  return M;
}

// Apply optimization passes to force vectorization
void optimizeModule(Module &M, TargetMachine *TM) {
  PassBuilder PB;
  
  LoopAnalysisManager LAM;
  FunctionAnalysisManager FAM;
  CGSCCAnalysisManager CGAM;
  ModuleAnalysisManager MAM;
  
  FAM.registerPass([&] { return TM->getTargetIRAnalysis(); });
  
  PB.registerModuleAnalyses(MAM);
  PB.registerCGSCCAnalyses(CGAM);
  PB.registerFunctionAnalyses(FAM);
  PB.registerLoopAnalyses(LAM);
  PB.crossRegisterProxies(LAM, FAM, CGAM, MAM);
  
  ModulePassManager MPM = PB.buildPerModuleDefaultPipeline(OptimizationLevel::O3);
  
  MPM.run(M, MAM);
}

int main(int argc, char** argv) {
  // Parse command line arguments
  bool useNoSVE = false;
  for (int i = 1; i < argc; i++) {
    if (std::string(argv[i]) == "--use-nosve") {
      useNoSVE = true;
    }
  }

  InitializeNativeTarget();
  InitializeNativeTargetAsmPrinter();
  InitializeNativeTargetAsmParser();
  
  // Silence remarks
  LLVMContext Context;
  Context.setDiagnosticHandler(std::make_unique<SilenceRemarksHandler>());
  
  auto JTMB = cantFail(JITTargetMachineBuilder::detectHost());
  JTMB.setCodeGenOptLevel(CodeGenOptLevel::Aggressive);
  
  if (useNoSVE) {
    JTMB.addFeatures(std::vector<std::string>{"-sve"});
  }
  
  std::unique_ptr<TargetMachine> TM(cantFail(JTMB.createTargetMachine()));
  auto M = createVectorModule(Context);
  M->setDataLayout(TM->createDataLayout());
  
  // Apply optimization passes to ensure and force vectorization
  optimizeModule(*M, TM.get());
  
  // Set-up JIT compiled function
  auto JIT = cantFail(LLJITBuilder().setJITTargetMachineBuilder(std::move(JTMB)).create());
  cantFail(JIT->addIRModule(ThreadSafeModule(std::move(M), std::make_unique<LLVMContext>())));
  auto VecOpAddr = cantFail(JIT->lookup("vector_op"));
  auto *VectorOp = (void(*)(float*, float*, float*, int))VecOpAddr.getValue();
  const int Length = 1024;
  std::vector<float> A(Length), B(Length), Result(Length);
  for (int i = 0; i < Length; i++) {
    A[i] = i;
    B[i] = i * 2;
  }
  
  // Execute JIT-compiled function
  // It should fault with an illegal instruction on such devices
  VectorOp(A.data(), B.data(), Result.data(), Length);
  
  // Will only reach here if execution succeeds
  std::cout << "Result[10]: " << Result[10] << std::endl;
  
  return 0;
}
```

When executed normally, the program generates illegal instructions on hardware that meets the specified conditions. It will also accept an argument `--use-nosve` to add `-sve` to the JIT's features list which should cause it not crash.

### Additional Context
Attempting to workaround this issue locally revealed frustrating inconsistentencies in how LLVM CPU features are specified across different LLVM interfaces:
- `-march=`
- `-mcpu=`
- `-Xclang -target-feature`
- `llvm::orc::JITTargetMachineBuilder::addFeatures()`

Each of these accepts a different set of feature flags with inconsistent naming conventions and limited documentation.

## Comments

### Comment 1 - efriedma-quic

See https://github.com/llvm/llvm-project/blob/42d49a77241df73a17cb442973702fc460e7fb90/llvm/lib/TargetParser/Host.cpp#L2088

---

### Comment 2 - StDymphna

> See
> 
> [llvm-project/llvm/lib/TargetParser/Host.cpp](https://github.com/llvm/llvm-project/blob/42d49a77241df73a17cb442973702fc460e7fb90/llvm/lib/TargetParser/Host.cpp#L2088)
> 
> Line 2088 in [42d49a7](/llvm/llvm-project/commit/42d49a77241df73a17cb442973702fc460e7fb90)
> 
>  // LLVM has decided some AArch64 CPUs have all the instructions they _may_

Does this comment apply to more than the cryptography instructions it surrounds? Because the reproducer is not SVE AES.

 Regardless, if that's the design philosophy, it should be trapping the illegal instruction signals and handling fallback.

---

### Comment 3 - efriedma-quic

See #95694 for context.  At the time, the discussion only mentioned crypto, but I guess sve/sve2 have a similar issue.  The goal is for getHostCPUName+getHostCPUFeatures to accurately characterize what features are available, so stuff like the JIT just works.

---

### Comment 4 - davemgreen

It would probably be better to go via hwcaps than parsing cpu features in /proc/cpuinfo like we do at the moment, and trust what features the kernel says are enabled.

---

### Comment 5 - z3ntu

For linking back to there, this seems to cause the following issue in Mesa: https://gitlab.freedesktop.org/mesa/mesa/-/issues/13314

---

### Comment 6 - efriedma-quic

Posted #160410; I'd appreciate help testing for both Linux and Windows (CC @dpaoliello ).  If not, I'll probably figure out the testing myself at some point, but not sure when I'll get to it.

---

### Comment 7 - DavidSpickett

This is not a challenge to the validity of the bug, or the original poster's analysis as such, but I want to add some context that folks may find useful in understanding the issue. Or can use to understand similar bugs in their own projects.

Context: I work for Arm, but I too may make mistakes reading the specifications so please check my citations and the latest architecture manual if you are going to rely on these details.

This issue states:
> Arm-V9 CPUs from Qualcomm SoCs that do not implement SVE despite the Arm-V9 specification requiring it

The Armv9-A specification does not require it. Though I see how you would get that from the architecture manual's wording, it does not say it is `MANDATORY` which is the key term the manual uses in the sections that describe the sets of extensions that make up each version.

https://developer.arm.com/documentation/ddi0487/latest/ (document version `L.b` as of today)

> A2.3.1 The Armv9.0 architecture extension
> 
> An implementation is Armv9.0 compliant if all of the following apply:
> • It is Armv8.5 compliant.
> • It includes all of the Armv9.0 architectural features that are mandatory.

FEAT_SVE is not given a subsection here but FEAT_SVE2 is.

> FEAT_SVE2, Scalable Vector Extension version 2
>
> All Armv9-A systems that support standard operating systems with rich application environments also provide
hardware support for SVE2 instructions.
> 
> FEAT_SVE2 is OPTIONAL from Armv9.0.
> 
> If FEAT_SVE2 is implemented, then FEAT_SVE is implemented.

What "support standard operating systems with rich application environments" means is not clear and Arm has not chosen to define it.

What I can say is that if it were mandatory for Armv9-A as a whole, the term `MANDATORY` would be used specifically.

At this time, FEAT_SVE is not mandatory at any architecture level:

> FEAT_SVE, Scalable Vector Extension
> <...>
> FEAT_SVE is OPTIONAL from Armv8.2.

The practical effect of this is the same as the issue has described, but those are the details if you need them.

---

### Comment 8 - StDymphna

@DavidSpickett 

I do recall checking that document, and I obviously messed up in reading it somehow; however, while _no architecture level_ requires SVE, et al., I do believe a lot of ARM's CPU IP offerings _do require_ them. They are listed as supported features on respective pages and documentations and not as a configurable option, e.g.:

https://developer.arm.com/documentation/102826/0600/?lang=en

https://developer.arm.com/documentation/101593/0101/The-Cortex-X3--core/Supported-standards-and-specifications?lang=en

https://developer.arm.com/documentation/101590/0103/The-Cortex-A715--core/Cortex-A715--core-features?lang=en

https://developer.arm.com/documentation/101800/0201/The-Cortex-A710--core/Cortex-A710--core-features?lang=en

If you agree in that assertion, I will ammend the issue to specify that certain CPUs have been produced that do not meet their individual specs (at least by Qualcomm, I do not know if others have made similar non-compliant products).

---

### Comment 9 - DavidSpickett

> I do recall checking that document, and I obviously messed up in reading it somehow

I've read those sections a whole lot for other tasks and dealt with this specific corner of v9 before. So it's totally understandable you'd interpret it that way, I also did when it was first published.

> If you agree in that assertion, I will ammend the issue to specify that certain CPUs have been produced that do not meet their individual specs (at least by Qualcomm, I do not know if others have made similar non-compliant products).

I can't comment on an implementation of a specific core, just the architecture in the abstract. You are welcome to add your own experiences to the issue of course.

(regardless of what you add or not, this is a valid issue for LLVM, I'm just adding the architecture background to it)

---

### Comment 10 - efriedma-quic

The reason this is a problem for Qualcomm cores, specifically, is that LLVM detects them as Arm Cortex cores which have SVE.

The whole armv9 thing is also a mess, but not related to this (there's a different issue report discussing that).

---

### Comment 11 - efriedma-quic

The JIT side of the issue should be fixed by #160410.  The clang issues are now tracked under #168998.

---

