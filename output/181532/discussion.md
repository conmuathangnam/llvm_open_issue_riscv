# mlir-opt crash: APInt::ashrInPlace assertion in arith-int-range-narrowing with $i0$ types

**Author:** xvv4402
**URL:** https://github.com/llvm/llvm-project/issues/181532
**Status:** Closed
**Labels:** crash, mlir:arith, mlir:vector
**Closed Date:** 2026-03-04T11:34:21Z

## Body

Bug Pass: --arith-int-range-narrowing

Bug Op: arith.trunci

Description:The mlir-opt tool crashes during integer range analysis when it encounters an arith.trunci operation truncating to a zero-width integer (i0).The analysis calls mlir::intrange::truncRange, which eventually invokes llvm::APInt::ashrInPlace (arithmetic shift right). The LLVM APInt implementation asserts that the shift amount must be less than or equal to the bit width. When the bit width is $0$, the internal logic for sign-extension or range bound calculation performs an invalid shift, triggering the process abort.

```mlir
module {
  func.func @main() {
    %0 = pdl.type : i32
    %1 = vector.constant_mask [1] : vector<1xi1>
    %2 = arith.trunci %1 : vector<1xi1> to vector<1xi0>
    return
  }
}
```
```
mlir-opt --arith-int-range-narrowing test.mlir
```
```
mlir-opt: llvm-project-llvmorg-22.1.0-rc1/llvm/include/llvm/ADT/APInt.h:836: void llvm::APInt::ashrInPlace(unsigned int): Assertion `ShiftAmt <= BitWidth && "Invalid shift amount"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llvmorg-22.1.0-install/bin/mlir-opt --arith-int-range-narrowing test.mlir
 #0 0x0000624210b87e32 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #1 0x0000624210b845af llvm::sys::RunSignalHandlers() (llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #2 0x0000624210b846fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x000079a463045330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x000079a46309eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x000079a46309eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x000079a46309eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x000079a46304527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x000079a4630288ff abort ./stdlib/abort.c:81:7
 #9 0x000079a46302881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#10 0x000079a46303b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#11 0x00006242182aa9ff mlir::intrange::truncRange(mlir::ConstantIntRanges const&, unsigned int) (llvmorg-22.1.0-install/bin/mlir-opt+0x9c529ff)
#12 0x00006242182a0213 mlir::arith::TruncIOp::inferResultRanges(llvm::ArrayRef<mlir::ConstantIntRanges>, llvm::function_ref<void (mlir::Value, mlir::ConstantIntRanges const&)>) (llvmorg-22.1.0-install/bin/mlir-opt+0x9c48213)
#13 0x000062421828686a mlir::detail::InferIntRangeInterfaceInterfaceTraits::Model<mlir::arith::TruncIOp>::inferResultRanges(mlir::detail::InferIntRangeInterfaceInterfaceTraits::Concept const*, mlir::Operation*, llvm::ArrayRef<mlir::ConstantIntRanges>, llvm::function_ref<void (mlir::Value, mlir::ConstantIntRanges const&)>) (llvmorg-22.1.0-install/bin/mlir-opt+0x9c2e86a)
#14 0x000062421867fd08 mlir::intrange::detail::defaultInferResultRanges(mlir::InferIntRangeInterface, llvm::ArrayRef<mlir::IntegerValueRange>, llvm::function_ref<void (mlir::Value, mlir::IntegerValueRange const&)>) (llvmorg-22.1.0-install/bin/mlir-opt+0xa027d08)
#15 0x000062421858b94e mlir::dataflow::IntegerRangeAnalysis::visitOperation(mlir::Operation*, llvm::ArrayRef<mlir::dataflow::IntegerValueRangeLattice const*>, llvm::ArrayRef<mlir::dataflow::IntegerValueRangeLattice*>) (llvmorg-22.1.0-install/bin/mlir-opt+0x9f3394e)
#16 0x000062421859c450 mlir::dataflow::AbstractSparseForwardDataFlowAnalysis::visitOperation(mlir::Operation*) (.part.0) SparseAnalysis.cpp:0:0
#17 0x00006242185a11bb mlir::dataflow::AbstractSparseForwardDataFlowAnalysis::initializeRecursively(mlir::Operation*) (llvmorg-22.1.0-install/bin/mlir-opt+0x9f491bb)
#18 0x00006242185a1833 mlir::dataflow::AbstractSparseForwardDataFlowAnalysis::initializeRecursively(mlir::Operation*) (llvmorg-22.1.0-install/bin/mlir-opt+0x9f49833)
#19 0x00006242185a1833 mlir::dataflow::AbstractSparseForwardDataFlowAnalysis::initializeRecursively(mlir::Operation*) (llvmorg-22.1.0-install/bin/mlir-opt+0x9f49833)
#20 0x000062421851d0fd mlir::DataFlowSolver::initializeAndRun(mlir::Operation*) (llvmorg-22.1.0-install/bin/mlir-opt+0x9ec50fd)
#21 0x0000624216e7744e (anonymous namespace)::IntRangeNarrowingPass::runOnOperation() IntRangeOptimizations.cpp:0:0
#22 0x0000624218427756 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) (llvmorg-22.1.0-install/bin/mlir-opt+0x9dcf756)
#23 0x0000624218427a8e mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) (llvmorg-22.1.0-install/bin/mlir-opt+0x9dcfa8e)
#24 0x00006242184281e2 mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) (llvmorg-22.1.0-install/bin/mlir-opt+0x9dd01e2)
#25 0x0000624218429400 mlir::PassManager::run(mlir::Operation*) (llvmorg-22.1.0-install/bin/mlir-opt+0x9dd1400)
#26 0x0000624210c4e767 performActions(llvm::raw_ostream&, std::shared_ptr<llvm::SourceMgr> const&, mlir::MLIRContext*, mlir::MlirOptMainConfig const&) MlirOptMain.cpp:0:0
#27 0x0000624210c4f4ea llvm::LogicalResult llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::callback_fn<mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::'lambda'(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, llvm::raw_ostream&)>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) MlirOptMain.cpp:0:0
#28 0x00006242187f7731 mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef) (llvmorg-22.1.0-install/bin/mlir-opt+0xa19f731)
#29 0x0000624210c468d0 mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&) (.part.0) MlirOptMain.cpp:0:0
#30 0x0000624210c4fb70 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&) (llvmorg-22.1.0-install/bin/mlir-opt+0x25f7b70)
#31 0x0000624210c4fda9 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&) (llvmorg-22.1.0-install/bin/mlir-opt+0x25f7da9)
#32 0x0000624210a81023 main (llvmorg-22.1.0-install/bin/mlir-opt+0x2429023)
#33 0x000079a46302a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#34 0x000079a46302a28b call_init ./csu/../csu/libc-start.c:128:20
#35 0x000079a46302a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#36 0x0000624210b66405 _start (llvmorg-22.1.0-install/bin/mlir-opt+0x250e405)
Aborted (core dumped)
```

