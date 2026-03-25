# mlir-opt crash: arith.addui_extended folder assertion when encountering ub.poison

**Author:** xvv4402
**URL:** https://github.com/llvm/llvm-project/issues/181534
**Status:** Closed
**Labels:** crash, mlir:arith
**Closed Date:** 2026-02-26T19:23:32Z

## Body

Bug Pass: --bubble-down-memory-space-casts (triggers Greedy Pattern Rewriting/Folding)

Bug Op: arith.addui_extended

Description:
The mlir-opt tool crashes during operation folding when arith.addui_extended receives a ub.poison operand. The constant folder for this operation attempts to cast the input attributes to TypedAttr (specifically expecting integer-like attributes) to calculate the sum and overflow bit. Since ub.PoisonAttr does not satisfy this cast, the llvm::cast fails, triggering an assertion.


```mlir
module {
  func.func @main() {
    %1 = "emitc.variable"() <{value = 0 : i32}> : () -> !emitc.lvalue<i32>
    %2 = ub.poison : i32
    %3 = spirv.BitwiseXor %2, %2 : i32
    %4 = spirv.SRem %3, %2 : i32
    %5 = gpu.subgroup_reduce  add %4 uniform : (i32) -> i32
    %6, %7 = arith.addui_extended %5, %2 : i32, i1
    vector.print %6 : i32
    return
  }
}
```
```
mlir-opt --bubble-down-memory-space-casts test.mlir
```
```
mlir-opt: /mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-22.1.0-rc1/llvm/include/llvm/Support/Casting.h:566: decltype(auto) llvm::cast(From&) [with To = mlir::TypedAttr; From = mlir::Attribute]: Assertion `isa<To>(Val) && "cast<Ty>() argument of incompatible type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: /mnt/sde1/sbw/mlir/tool/llvmorg-22.1.0-install/bin/mlir-opt --bubble-down-memory-space-casts /mnt/sdd1/sbw/mlir/data/genProgram_0204_filtered/genProgram_0204_valid/051202_9612_7.mlir
 #0 0x000063f6742e7e32 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/mnt/sde1/sbw/mlir/tool/llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #1 0x000063f6742e45af llvm::sys::RunSignalHandlers() (/mnt/sde1/sbw/mlir/tool/llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #2 0x000063f6742e46fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x000073e8b6a45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x000073e8b6a9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x000073e8b6a9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x000073e8b6a9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x000073e8b6a4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x000073e8b6a288ff abort ./stdlib/abort.c:81:7
 #9 0x000073e8b6a2881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#10 0x000073e8b6a3b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#11 0x000063f67b99805e mlir::arith::AddUIExtendedOp::fold(mlir::arith::AddUIExtendedOpGenericAdaptor<llvm::ArrayRef<mlir::Attribute>>, llvm::SmallVectorImpl<mlir::OpFoldResult>&) (/mnt/sde1/sbw/mlir/tool/llvmorg-22.1.0-install/bin/mlir-opt+0x9be005e)
#12 0x000063f67b9da5b7 llvm::LogicalResult llvm::detail::UniqueFunctionBase<llvm::LogicalResult, mlir::Operation*, llvm::ArrayRef<mlir::Attribute>, llvm::SmallVectorImpl<mlir::OpFoldResult>&>::CallImpl<mlir::Op<mlir::arith::AddUIExtendedOp, mlir::OpTrait::ZeroRegions, mlir::OpTrait::NResults<2u>::Impl, mlir::OpTrait::ZeroSuccessors, mlir::OpTrait::NOperands<2u>::Impl, mlir::OpTrait::OpInvariants, mlir::ConditionallySpeculatable::Trait, mlir::OpTrait::AlwaysSpeculatableImplTrait, mlir::MemoryEffectOpInterface::Trait, mlir::OpTrait::IsCommutative, mlir::VectorUnrollOpInterface::Trait, mlir::OpTrait::Elementwise, mlir::OpTrait::Scalarizable, mlir::OpTrait::Vectorizable, mlir::OpTrait::Tensorizable, mlir::OpAsmOpInterface::Trait>::getFoldHookFn()::'lambda'(mlir::Operation*, llvm::ArrayRef<mlir::Attribute>, llvm::SmallVectorImpl<mlir::OpFoldResult>&) const>(void*, mlir::Operation*, llvm::ArrayRef<mlir::Attribute>, llvm::SmallVectorImpl<mlir::OpFoldResult>&) (/mnt/sde1/sbw/mlir/tool/llvmorg-22.1.0-install/bin/mlir-opt+0x9c225b7)
#13 0x000063f67b9f8551 mlir::RegisteredOperationName::Model<mlir::arith::AddUIExtendedOp>::foldHook(mlir::Operation*, llvm::ArrayRef<mlir::Attribute>, llvm::SmallVectorImpl<mlir::OpFoldResult>&) (/mnt/sde1/sbw/mlir/tool/llvmorg-22.1.0-install/bin/mlir-opt+0x9c40551)
#14 0x000063f67befd074 mlir::Operation::fold(llvm::ArrayRef<mlir::Attribute>, llvm::SmallVectorImpl<mlir::OpFoldResult>&) (/mnt/sde1/sbw/mlir/tool/llvmorg-22.1.0-install/bin/mlir-opt+0xa145074)
#15 0x000063f67befd4e3 mlir::Operation::fold(llvm::SmallVectorImpl<mlir::OpFoldResult>&) (/mnt/sde1/sbw/mlir/tool/llvmorg-22.1.0-install/bin/mlir-opt+0xa1454e3)
#16 0x000063f67bad9603 (anonymous namespace)::GreedyPatternRewriteDriver::processWorklist() GreedyPatternRewriteDriver.cpp:0:0
#17 0x000063f67bade060 mlir::applyPatternsGreedily(mlir::Region&, mlir::FrozenRewritePatternSet const&, mlir::GreedyRewriteConfig, bool*) (/mnt/sde1/sbw/mlir/tool/llvmorg-22.1.0-install/bin/mlir-opt+0x9d26060)
#18 0x000063f67ba3b249 (anonymous namespace)::BubbleDownMemorySpaceCasts::runOnOperation() BubbleDownMemorySpaceCasts.cpp:0:0
#19 0x000063f67bb87756 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) (/mnt/sde1/sbw/mlir/tool/llvmorg-22.1.0-install/bin/mlir-opt+0x9dcf756)
#20 0x000063f67bb87a8e mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) (/mnt/sde1/sbw/mlir/tool/llvmorg-22.1.0-install/bin/mlir-opt+0x9dcfa8e)
#21 0x000063f67bb881e2 mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) (/mnt/sde1/sbw/mlir/tool/llvmorg-22.1.0-install/bin/mlir-opt+0x9dd01e2)
#22 0x000063f67bb89400 mlir::PassManager::run(mlir::Operation*) (/mnt/sde1/sbw/mlir/tool/llvmorg-22.1.0-install/bin/mlir-opt+0x9dd1400)
#23 0x000063f6743ae767 performActions(llvm::raw_ostream&, std::shared_ptr<llvm::SourceMgr> const&, mlir::MLIRContext*, mlir::MlirOptMainConfig const&) MlirOptMain.cpp:0:0
#24 0x000063f6743af4ea llvm::LogicalResult llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::callback_fn<mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::'lambda'(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, llvm::raw_ostream&)>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) MlirOptMain.cpp:0:0
#25 0x000063f67bf57731 mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef) (/mnt/sde1/sbw/mlir/tool/llvmorg-22.1.0-install/bin/mlir-opt+0xa19f731)
#26 0x000063f6743a68d0 mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&) (.part.0) MlirOptMain.cpp:0:0
#27 0x000063f6743afb70 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&) (/mnt/sde1/sbw/mlir/tool/llvmorg-22.1.0-install/bin/mlir-opt+0x25f7b70)
#28 0x000063f6743afda9 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&) (/mnt/sde1/sbw/mlir/tool/llvmorg-22.1.0-install/bin/mlir-opt+0x25f7da9)
#29 0x000063f6741e1023 main (/mnt/sde1/sbw/mlir/tool/llvmorg-22.1.0-install/bin/mlir-opt+0x2429023)
#30 0x000073e8b6a2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#31 0x000073e8b6a2a28b call_init ./csu/../csu/libc-start.c:128:20
#32 0x000073e8b6a2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#33 0x000063f6742c6405 _start (/mnt/sde1/sbw/mlir/tool/llvmorg-22.1.0-install/bin/mlir-opt+0x250e405)
Aborted (core dumped)
```

## Comments

### Comment 1 - EugeneZelenko

@xvv4402: Please run fuzzer on current `main` (and update it periodically. Looks like MLIR issues are not backported to release branch often, so you may report already fixed ones.

---

