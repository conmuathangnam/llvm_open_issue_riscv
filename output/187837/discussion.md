# [Flang][OpenMP] crash in flang/lib/Optimizer/OpenMP/MapInfoFinalization.cpp

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/187837
**Status:** Open
**Labels:** crash, flang:openmp

## Body

Reproducer:
https://godbolt.org/z/jhE1nMeKr
```f90
type t1
  integer, allocatable :: A
end type t1

type t2
  type(t1) :: B
end type

type(t2) :: var1, var2

!$omp target map(to: var1%B) map(tofrom: var2%B)
!$omp end target

end
```

Backtrace:
```console
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /cefs/ab/ab981df5894b5c52b9313cab_clang-llvmflang-trunk-20260311/bin/flang -fc1 -triple x86_64-unknown-linux-gnu -S -mrelocation-model pic -pic-level 2 -pic-is-pie -target-cpu x86-64 -mllvm -x86-asm-syntax=intel -fopenmp -resource-dir /cefs/ab/ab981df5894b5c52b9313cab_clang-llvmflang-trunk-20260311/lib/clang/23 -mframe-pointer=all -o /app/output.s -x f95 /app/example.f90
 #0 0x00000000046f33b8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/cefs/ab/ab981df5894b5c52b9313cab_clang-llvmflang-trunk-20260311/bin/flang+0x46f33b8)
 #1 0x00000000046f04f4 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x000071f401642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x000000000a8da8b8 mlir::getForwardSlice(mlir::Value, llvm::SetVector<mlir::Operation*, llvm::SmallVector<mlir::Operation*, 0u>, llvm::DenseSet<mlir::Operation*, llvm::DenseMapInfo<mlir::Operation*, void>>, 0u>*, mlir::SliceOptions const&) (/cefs/ab/ab981df5894b5c52b9313cab_clang-llvmflang-trunk-20260311/bin/flang+0xa8da8b8)
 #4 0x0000000005c77e42 (anonymous namespace)::MapInfoFinalizationPass::runOnOperation()::'lambda'(mlir::Operation*)::operator()(mlir::Operation*) const::'lambda'(mlir::omp::MapInfoOp)::operator()(mlir::omp::MapInfoOp) const (.isra.0) MapInfoFinalization.cpp:0:0
 #5 0x0000000005c79ca1 _ZN4llvm12function_refIFN4mlir10WalkResultEPNS1_9OperationEEE11callback_fnIZNS1_6detail4walkILNS1_9WalkOrderE1ENS1_15ForwardIteratorEZZN12_GLOBAL__N_123MapInfoFinalizationPass14runOnOperationEvENKUlS4_E_clES4_EUlNS1_3omp9MapInfoOpEE_SG_S2_EENSt9enable_ifIXaantsrSt11disjunctionIJSt7is_sameIT2_S4_ESK_ISL_PNS1_6RegionEESK_ISL_PNS1_5BlockEEEE5valuesrSK_IT3_S2_E5valueESU_E4typeES4_OT1_EUlS4_E_EES2_lS4_ MapInfoFinalization.cpp:0:0
 #6 0x0000000005c79e34 void llvm::function_ref<void (mlir::Operation*)>::callback_fn<(anonymous namespace)::MapInfoFinalizationPass::runOnOperation()::'lambda'(mlir::Operation*)>(long, mlir::Operation*) MapInfoFinalization.cpp:0:0
 #7 0x0000000005c73b72 (anonymous namespace)::MapInfoFinalizationPass::runOnOperation() MapInfoFinalization.cpp:0:0
 #8 0x000000000a896819 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) (/cefs/ab/ab981df5894b5c52b9313cab_clang-llvmflang-trunk-20260311/bin/flang+0xa896819)
 #9 0x000000000a896a89 mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) (/cefs/ab/ab981df5894b5c52b9313cab_clang-llvmflang-trunk-20260311/bin/flang+0xa896a89)
#10 0x000000000a8976c9 mlir::PassManager::run(mlir::Operation*) (/cefs/ab/ab981df5894b5c52b9313cab_clang-llvmflang-trunk-20260311/bin/flang+0xa8976c9)
#11 0x0000000004a1b73c Fortran::frontend::CodeGenAction::beginSourceFileAction() (/cefs/ab/ab981df5894b5c52b9313cab_clang-llvmflang-trunk-20260311/bin/flang+0x4a1b73c)
#12 0x000000000475d5e5 Fortran::frontend::FrontendAction::beginSourceFile(Fortran::frontend::CompilerInstance&, Fortran::frontend::FrontendInputFile const&) (/cefs/ab/ab981df5894b5c52b9313cab_clang-llvmflang-trunk-20260311/bin/flang+0x475d5e5)
#13 0x000000000474a5ff Fortran::frontend::CompilerInstance::executeAction(Fortran::frontend::FrontendAction&) (/cefs/ab/ab981df5894b5c52b9313cab_clang-llvmflang-trunk-20260311/bin/flang+0x474a5ff)
#14 0x00000000047689ef Fortran::frontend::executeCompilerInvocation(Fortran::frontend::CompilerInstance*) (/cefs/ab/ab981df5894b5c52b9313cab_clang-llvmflang-trunk-20260311/bin/flang+0x47689ef)
#15 0x00000000028aff25 fc1_main(llvm::ArrayRef<char const*>, char const*) (/cefs/ab/ab981df5894b5c52b9313cab_clang-llvmflang-trunk-20260311/bin/flang+0x28aff25)
#16 0x000000000275dd3c main (/cefs/ab/ab981df5894b5c52b9313cab_clang-llvmflang-trunk-20260311/bin/flang+0x275dd3c)
#17 0x000071f401629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#18 0x000071f401629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#19 0x00000000028ae875 _start (/cefs/ab/ab981df5894b5c52b9313cab_clang-llvmflang-trunk-20260311/bin/flang+0x28ae875)
flang-23: error: unable to execute command: Segmentation fault (core dumped)
flang-23: error: flang frontend command failed with exit code -2 (use -v to see invocation)
flang version 23.0.0git (https://github.com/llvm/llvm-project.git 78beeb764372625c5ba942e7ee59f0adefd93a37)
Target: x86_64-unknown-linux-gnu
Thread model: posix
```

With assertion-enabled flang, compiling the above reproducer results in the following assertion failure:
```txt
flang: /path_to_project/llvm-project/flang/lib/Optimizer/OpenMP/MapInfoFinalization.cpp:987: auto (anonymous namespace)::MapInfoFinalizationPass::runOnOperation()::(anonymous class)::operator()(mlir::Operation *)::(anonymous class)::operator()(mlir::omp::MapInfoOp) const: Assertion `mapVarIdx >= 0 && mapVarIdx < static_cast<int64_t>(mapClauseOwner.getMapVars().size())' failed.
```

