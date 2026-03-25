# mlir-opt crash: std::optional assertion failure in IntegerRangeAnalysis during --arith-int-range-narrowing

**Author:** xvv4402
**URL:** https://github.com/llvm/llvm-project/issues/180312
**Status:** Closed
**Labels:** crash, mlir:dataflow
**Closed Date:** 2026-02-27T10:47:11Z

## Body

**Bug Op**: tensor.generate / affine.for (Region-based Interface Ops)

**Bug Pass**: --arith-int-range-narrowing

**Detailed Description**: The mlir-opt tool crashes with a std::optional "not engaged" assertion failure. The crash is located in IntegerRangeAnalysis::visitNonControlFlowArguments. It occurs when the data-flow analysis attempts to retrieve range information for block arguments within region-based operations (like tensor.generate or affine.for

```mlir
module {
  func.func @main() {
    %0 = gpu.global_id  x
    %1 = gpu.block_id  x
    %2 = tensor.generate %1 {
      ^bb0(%arg0: index):
      %3 = arith.addi %arg0, %0 : index
      %4 = arith.index_cast %3 : index to i32
      tensor.yield %4 : i32
    } : tensor<?xi32>
    %5 = tensor.dim %2, %0 : tensor<?xi32>
    %6 = affine.for %arg0 = 0 to %5 iter_args(%arg1 = %0) -> (index) {
      affine.yield %arg0 : index
    }
    return
  }
}
```
```
mlir-opt --arith-int-range-narrowing temp.mlir
```
```
/usr/include/c++/13/optional:479: constexpr _Tp& std::_Optional_base_impl<_Tp, _Dp>::_M_get() [with _Tp = llvm::SmallVector<mlir::OpFoldResult>; _Dp = std::_Optional_base<llvm::SmallVector<mlir::OpFoldResult>, false, false>]: Assertion 'this->_M_is_engaged()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llvmorg-22.1.0-install/bin/mlir-opt --arith-int-range-narrowing temp.mlir
 #0 0x00005e87e8a89e32 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #1 0x00005e87e8a865af llvm::sys::RunSignalHandlers() (llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #2 0x00005e87e8a866fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x00007d77d1045330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x00007d77d109eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x00007d77d109eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x00007d77d109eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x00007d77d104527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x00007d77d10288ff abort ./stdlib/abort.c:81:7
 #9 0x00007d77d14df90d (/lib/x86_64-linux-gnu/libstdc++.so.6+0xdf90d)
#10 0x00005e87f048fc0e mlir::dataflow::IntegerRangeAnalysis::visitNonControlFlowArguments(mlir::Operation*, mlir::RegionSuccessor const&, llvm::ArrayRef<mlir::dataflow::IntegerValueRangeLattice*>, unsigned int) (llvmorg-22.1.0-install/bin/mlir-opt+0x9f35c0e)
#11 0x00005e87f04a0066 mlir::dataflow::AbstractSparseForwardDataFlowAnalysis::visitRegionSuccessors(mlir::ProgramPoint*, mlir::RegionBranchOpInterface, mlir::RegionSuccessor, llvm::ArrayRef<mlir::dataflow::AbstractSparseLattice*>) (llvmorg-22.1.0-install/bin/mlir-opt+0x9f46066)
#12 0x00005e87f04a2ce6 mlir::dataflow::AbstractSparseForwardDataFlowAnalysis::visitBlock(mlir::Block*) (llvmorg-22.1.0-install/bin/mlir-opt+0x9f48ce6)
#13 0x00005e87f04a30d8 mlir::dataflow::AbstractSparseForwardDataFlowAnalysis::visit(mlir::ProgramPoint*) (llvmorg-22.1.0-install/bin/mlir-opt+0x9f490d8)
#14 0x00005e87f041f3ac mlir::DataFlowSolver::initializeAndRun(mlir::Operation*) (llvmorg-22.1.0-install/bin/mlir-opt+0x9ec53ac)
#15 0x00005e87eed7944e (anonymous namespace)::IntRangeNarrowingPass::runOnOperation() IntRangeOptimizations.cpp:0:0
#16 0x00005e87f0329756 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) (llvmorg-22.1.0-install/bin/mlir-opt+0x9dcf756)
```

