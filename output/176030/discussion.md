# [Flang][OpenMP] crash when private var set between teams and distribute clause

**Author:** Jason-Van-Beusekom
**URL:** https://github.com/llvm/llvm-project/issues/176030
**Status:** Closed
**Labels:** crash, mlir:openmp, flang:openmp
**Closed Date:** 2026-01-21T11:56:38Z

## Body

```fortran
program minimal_repro
  implicit none

  integer :: i, m
  integer :: res(10) = 0

!$omp target teams map(from:m,res) private(m)
  m = 5
!$omp distribute parallel do
  do i = 1, 10
    res(i) = 5 + i
  end do
!$omp end distribute parallel do
!$omp end target teams

print *, "Test completed successfully m =", m, " res=", res

end program minimal_repro
``` 

When compiling the above program I get the below segfault:
```
[vanbeuse@pe32 vanbeuse]$ flang -fopenmp -fopenmp-targets=amdgcn-amd-amdhsa minimal_repro.F90 
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: /ptmp/vanbeuse/clang_install_release/bin/flang -fc1 -triple x86_64-unknown-linux-gnu -emit-llvm-bc -fcolor-diagnostics -mrelocation-model pic -pic-level 2 -pic-is-pie -target-cpu x86-64 -fopenmp -resource-dir /ptmp/vanbeuse/clang_install_release/lib/clang/22 --offload-targets=amdgcn-amd-amdhsa -mframe-pointer=all -o /tmp/minimal_repro-8300a3.bc -x f95 minimal_repro.F90
 #0 0x00007f03c3260e7a llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/ptmp/vanbeuse/clang_install_release/bin/../lib/libLLVMSupport.so.22.0git+0x260e7a)
 #1 0x00007f03c3261f56 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x00007f03c223ebf0 __restore_rt (/lib64/libc.so.6+0x3ebf0)
 #3 0x00007f03b3330fc2 llvm::OpenMPIRBuilder::calculateCanonicalLoopTripCount(llvm::OpenMPIRBuilder::LocationDescription const&, llvm::Value*, llvm::Value*, llvm::Value*, bool, bool, llvm::Twine const&) (/ptmp/vanbeuse/clang_install_release/bin/../lib/../lib/libLLVMFrontendOpenMP.so.22.0git+0x95fc2)
 #4 0x00007f03b056b1f6 convertOmpTarget(mlir::Operation&, llvm::IRBuilderBase&, mlir::LLVM::ModuleTranslation&) OpenMPToLLVMIRTranslation.cpp:0:0
 #5 0x00007f03b05636bc convertHostOrTargetOperation(mlir::Operation*, llvm::IRBuilderBase&, mlir::LLVM::ModuleTranslation&) OpenMPToLLVMIRTranslation.cpp:0:0
 #6 0x00007f03b75e3701 mlir::LLVM::ModuleTranslation::convertOperation(mlir::Operation&, llvm::IRBuilderBase&, bool) (/ptmp/vanbeuse/clang_install_release/bin/../lib/../lib/libMLIRTargetLLVMIRExport.so.22.0git+0x2e701)
 #7 0x00007f03b75e4834 mlir::LLVM::ModuleTranslation::convertBlockImpl(mlir::Block&, bool, llvm::IRBuilderBase&, bool) (/ptmp/vanbeuse/clang_install_release/bin/../lib/../lib/libMLIRTargetLLVMIRExport.so.22.0git+0x2f834)
 #8 0x00007f03b75e985e mlir::LLVM::ModuleTranslation::convertOneFunction(mlir::LLVM::LLVMFuncOp) (/ptmp/vanbeuse/clang_install_release/bin/../lib/../lib/libMLIRTargetLLVMIRExport.so.22.0git+0x3485e)
 #9 0x00007f03b75ec99c mlir::LLVM::ModuleTranslation::convertFunctions() (/ptmp/vanbeuse/clang_install_release/bin/../lib/../lib/libMLIRTargetLLVMIRExport.so.22.0git+0x3799c)
#10 0x00007f03b75f0d48 mlir::translateModuleToLLVMIR(mlir::Operation*, llvm::LLVMContext&, llvm::StringRef, bool) (/ptmp/vanbeuse/clang_install_release/bin/../lib/../lib/libMLIRTargetLLVMIRExport.so.22.0git+0x3bd48)
#11 0x00007f03c6a23399 Fortran::frontend::CodeGenAction::generateLLVMIR() (/ptmp/vanbeuse/clang_install_release/bin/../lib/libflangFrontend.so.22.0git+0x223399)
#12 0x00007f03c6a2951c Fortran::frontend::CodeGenAction::executeAction() (/ptmp/vanbeuse/clang_install_release/bin/../lib/libflangFrontend.so.22.0git+0x22951c)
#13 0x00007f03c6a1aedc Fortran::frontend::FrontendAction::execute() (/ptmp/vanbeuse/clang_install_release/bin/../lib/libflangFrontend.so.22.0git+0x21aedc)
#14 0x00007f03c684bd72 Fortran::frontend::CompilerInstance::executeAction(Fortran::frontend::FrontendAction&) (/ptmp/vanbeuse/clang_install_release/bin/../lib/libflangFrontend.so.22.0git+0x4bd72)
#15 0x00007f03c7323d36 Fortran::frontend::executeCompilerInvocation(Fortran::frontend::CompilerInstance*) (/ptmp/vanbeuse/clang_install_release/bin/../lib/libflangFrontendTool.so.22.0git+0x5d36)
#16 0x0000000000205b65 fc1_main(llvm::ArrayRef<char const*>, char const*) (/ptmp/vanbeuse/clang_install_release/bin/flang+0x205b65)
#17 0x0000000000204f73 main (/ptmp/vanbeuse/clang_install_release/bin/flang+0x204f73)
#18 0x00007f03c22295d0 __libc_start_call_main (/lib64/libc.so.6+0x295d0)
#19 0x00007f03c2229680 __libc_start_main@GLIBC_2.2.5 (/lib64/libc.so.6+0x29680)
#20 0x0000000000203d15 _start (/ptmp/vanbeuse/clang_install_release/bin/flang+0x203d15)
flang-22: error: unable to execute command: Segmentation fault (core dumped)
flang-22: error: flang frontend command failed due to signal (use -v to see invocation)
flang version 22.0.0git (https://github.com/llvm/llvm-project.git 53f84636eb86e6c64c1ec405f70b3dd2b27f4ddc)
```

This is due to the function `extractOnlyOmpNestedDir` in `flang/lib/Lower/OpenMP/OpenMP.cpp`, returning null due to the insertion of a store between the `omp.teams` and `omp.parallel` instructions. 

