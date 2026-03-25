# [flang][openmp] nint intrinsic can't be compiled for Nvidia GPUs

**Author:** VeeEM
**URL:** https://github.com/llvm/llvm-project/issues/182378
**Status:** Open
**Labels:** crash, backend:NVPTX

## Body

nint is implemented as a call to the llvm lround intrinsic, which is not implemented for Nvidia GPUs.

```fortran
subroutine nint_test1(i, a)
  integer :: i
  real :: a
  !$omp declare target
  i = nint(a)
end subroutine

subroutine nint_test2(i, a)
  integer(8) :: i
  real(8) :: a
  !$omp declare target
  i = nint(a, 8)
end subroutine
```
```
$ flang -fopenmp -c --offload-arch=sm_89 nint.f90
```
```
error: no libcall available for lround
LLVM ERROR: Cannot select: t29: ch = NVPTXISD::CALL t27, Constant:i32<0>, Constant:i32<0>, Constant:i32<1>, Constant:i32<1>, undef:i64, Constant:i32<0>
In function: nint_test1_
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /home/acc/Documents/flang_root/install/bin/flang -fc1 -triple nvptx64-nvidia-cuda -S -fcolor-diagnostics -mrelocation-model pic -pic-level 2 -target-cpu sm_89 -target-feature +ptx87 -fopenmp -resource-dir /home/acc/Documents/flang_root/install/lib/clang/23 -fopenmp-host-ir-file-path /tmp/nint-a8548c.bc -fopenmp-is-target-device -mframe-pointer=all -o /tmp/nint-sm_89-e698c4.s -x f95 nint.f90
1.	Running pass 'Function Pass Manager' on module 'FIRModule'.
2.	Running pass 'NVPTX DAG->DAG Pattern Instruction Selection' on function '@nint_test1_'
 #0 0x000055add6119ce8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/home/acc/Documents/flang_root/install/bin/flang+0x22f3ce8)
 #1 0x000055add6116e67 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x000014ae8be4aa70 (/usr/lib/x86_64-linux-gnu/libc.so.6+0x40a70)
 #3 0x000014ae8bea13bc (/usr/lib/x86_64-linux-gnu/libc.so.6+0x973bc)
 #4 0x000014ae8be4a942 raise (/usr/lib/x86_64-linux-gnu/libc.so.6+0x40942)
 #5 0x000014ae8be324ac abort (/usr/lib/x86_64-linux-gnu/libc.so.6+0x284ac)
 #6 0x000055add536ef01 llvm::install_bad_alloc_error_handler(void (*)(void*, char const*, bool), void*) (.cold) ErrorHandling.cpp:0:0
 #7 0x000055add93b051f llvm::SelectionDAGISel::CannotYetSelect(llvm::SDNode*) (/home/acc/Documents/flang_root/install/bin/flang+0x558a51f)
 #8 0x000055add93b3a0a llvm::SelectionDAGISel::SelectCodeCommon(llvm::SDNode*, unsigned char const*, unsigned int, unsigned char const*) (/home/acc/Documents/flang_root/install/bin/flang+0x558da0a)
 #9 0x000055add93a9f77 llvm::SelectionDAGISel::DoInstructionSelection() (/home/acc/Documents/flang_root/install/bin/flang+0x5583f77)
#10 0x000055add93be40e llvm::SelectionDAGISel::CodeGenAndEmitDAG() (/home/acc/Documents/flang_root/install/bin/flang+0x559840e)
#11 0x000055add93c1dd9 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&) (/home/acc/Documents/flang_root/install/bin/flang+0x559bdd9)
#12 0x000055add93c36a8 llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&) (/home/acc/Documents/flang_root/install/bin/flang+0x559d6a8)
#13 0x000055add93bbd00 llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&) (/home/acc/Documents/flang_root/install/bin/flang+0x5595d00)
#14 0x000055add979b6d7 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (.part.0) MachineFunctionPass.cpp:0:0
#15 0x000055addc685f18 llvm::FPPassManager::runOnFunction(llvm::Function&) (/home/acc/Documents/flang_root/install/bin/flang+0x885ff18)
#16 0x000055addc6862f5 llvm::FPPassManager::runOnModule(llvm::Module&) (/home/acc/Documents/flang_root/install/bin/flang+0x88602f5)
#17 0x000055addc686df8 llvm::legacy::PassManagerImpl::run(llvm::Module&) (/home/acc/Documents/flang_root/install/bin/flang+0x8860df8)
#18 0x000055add645c508 Fortran::frontend::CodeGenAction::executeAction() (/home/acc/Documents/flang_root/install/bin/flang+0x2636508)
#19 0x000055add616262c Fortran::frontend::FrontendAction::execute() (/home/acc/Documents/flang_root/install/bin/flang+0x233c62c)
#20 0x000055add614af04 Fortran::frontend::CompilerInstance::executeAction(Fortran::frontend::FrontendAction&) (/home/acc/Documents/flang_root/install/bin/flang+0x2324f04)
#21 0x000055add616e4a9 Fortran::frontend::executeCompilerInvocation(Fortran::frontend::CompilerInstance*) (/home/acc/Documents/flang_root/install/bin/flang+0x23484a9)
#22 0x000055add59f7e88 fc1_main(llvm::ArrayRef<char const*>, char const*) (/home/acc/Documents/flang_root/install/bin/flang+0x1bd1e88)
#23 0x000055add58ccf02 main (/home/acc/Documents/flang_root/install/bin/flang+0x1aa6f02)
#24 0x000014ae8be33f75 (/usr/lib/x86_64-linux-gnu/libc.so.6+0x29f75)
#25 0x000014ae8be34027 __libc_start_main (/usr/lib/x86_64-linux-gnu/libc.so.6+0x2a027)
#26 0x000055add59f6cb1 _start (/home/acc/Documents/flang_root/install/bin/flang+0x1bd0cb1)
flang-23: error: unable to execute command: Aborted
flang-23: error: flang frontend command failed due to signal (use -v to see invocation)
flang version 23.0.0git (https://github.com/llvm/llvm-project.git 8ccc40ee0ecfe1acb83a47539d4d824e167132cb)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /home/acc/Documents/flang_root/install/bin
Build config: +assertions
flang-23: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
flang-23: note: diagnostic msg: /tmp/nint-sm_89-2e6734
flang-23: note: diagnostic msg: /tmp/nint-0ec9a9
flang-23: note: diagnostic msg: /tmp/nint-sm_89-2e6734.sh
flang-23: note: diagnostic msg: 

********************
```

## Comments

### Comment 1 - MitchBriles

Can I try to fix this?

---

### Comment 2 - VeeEM

Fine by me :)

---

### Comment 3 - hakostra

To me this looks like a duplicate of issue #147027 and #147023 and the closed issue #179347 - since they essentially all are related to the lacking lowering of math intrinsics in Flang.

---

### Comment 4 - VeeEM

Those were all missing a translation from the mlir math dialect to libdevice calls. nint is implemented in a different way as a call to 32- and 64-bit lround llvm intrinsics, which fails for Nvidia GPUs because they are not implemented for the NVPTX backend.

---

