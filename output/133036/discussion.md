# [llvm][CodeGen] Assertion `!hasInterval(Reg) && "Interval already exists!"' failed

**Author:** kawashima-fj
**URL:** https://github.com/llvm/llvm-project/issues/133036
**Status:** Closed
**Labels:** llvm:codegen, crash-on-valid
**Closed Date:** 2025-04-08T00:19:03Z

## Body

After the commit b09b9ac1081d19c8021df8e55e96cd1325f0eed0, (PR #129204), the following assertion failure started to appear.

```console
$ llc -aarch64-enable-pipeliner test.ll
llc: llvm/include/llvm/CodeGen/LiveIntervals.h:150: llvm::LiveInterval& llvm::LiveIntervals::createEmptyInterval(llvm::Register): Assertion `!hasInterval(Reg) && "Interval already exists!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.      Program arguments: llc -aarch64-enable-pipeliner test.ll
1.      Running pass 'Function Pass Manager' on module 'test.ll'.
2.      Running pass 'Modulo Software Pipelining' on function '@foo_'
 #0 0x0000ffff87bbe738 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (lib/libLLVMSupport.so.21.0git+0x24e738)
 #1 0x0000ffff87bbbe20 llvm::sys::RunSignalHandlers() (lib/libLLVMSupport.so.21.0git+0x24be20)
 #2 0x0000ffff87bbbf88 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x0000ffff8c4107a0 (linux-vdso.so.1+0x7a0)
 #4 0x0000ffff87597608 (/lib/aarch64-linux-gnu/libc.so.6+0x87608)
 #5 0x0000ffff8754cb3c raise (/lib/aarch64-linux-gnu/libc.so.6+0x3cb3c)
 #6 0x0000ffff87537e00 abort (/lib/aarch64-linux-gnu/libc.so.6+0x27e00)
 #7 0x0000ffff87545cc0 (/lib/aarch64-linux-gnu/libc.so.6+0x35cc0)
 #8 0x0000ffff87545d30 __assert_perror_fail (/lib/aarch64-linux-gnu/libc.so.6+0x35d30)
 #9 0x0000ffff89f21cf8 llvm::ModuloScheduleExpander::calculateIntervals() (lib/libLLVMCodeGen.so.21.0git+0x771cf8)
#10 0x0000ffff89f3684c llvm::ModuloScheduleExpander::generatePipelinedLoop() (lib/libLLVMCodeGen.so.21.0git+0x78684c)
#11 0x0000ffff89e67390 llvm::SwingSchedulerDAG::schedule() (lib/libLLVMCodeGen.so.21.0git+0x6b7390)
#12 0x0000ffff89e67c64 llvm::MachinePipeliner::swingModuloScheduler(llvm::MachineLoop&) (lib/libLLVMCodeGen.so.21.0git+0x6b7c64)
#13 0x0000ffff89e68168 llvm::MachinePipeliner::scheduleLoop(llvm::MachineLoop&) (lib/libLLVMCodeGen.so.21.0git+0x6b8168)
#14 0x0000ffff89e6848c llvm::MachinePipeliner::runOnMachineFunction(llvm::MachineFunction&) (lib/libLLVMCodeGen.so.21.0git+0x6b848c)
#15 0x0000ffff89dc8f30 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (.part.0) MachineFunctionPass.cpp:0:0
#16 0x0000ffff88174378 llvm::FPPassManager::runOnFunction(llvm::Function&) (lib/libLLVMCore.so.21.0git+0x494378)
#17 0x0000ffff8817457c llvm::FPPassManager::runOnModule(llvm::Module&) (lib/libLLVMCore.so.21.0git+0x49457c)
#18 0x0000ffff881751e0 llvm::legacy::PassManagerImpl::run(llvm::Module&) (lib/libLLVMCore.so.21.0git+0x4951e0)
#19 0x0000aaaaddc5e474 compileModule(char**, llvm::LLVMContext&) llc.cpp:0:0
#20 0x0000aaaaddc5ef10 main (bin/llc+0x2ef10)
#21 0x0000ffff875384c4 (/lib/aarch64-linux-gnu/libc.so.6+0x284c4)
#22 0x0000ffff87538598 __libc_start_main (/lib/aarch64-linux-gnu/libc.so.6+0x28598)
#23 0x0000aaaaddc53130 _start (bin/llc+0x23130)
```

The input IR is [test.ll](https://github.com/user-attachments/files/19460743/test.ll.txt).
This IR is genereted by compiling the following Fortran program with `flang --target=aarch64-unknown-linux-gnu -S -emit-llvm -O2 -mcpu=neoverse-v1`

```fortran
subroutine foo(a, b, n)
  implicit none
  real, intent(out) :: a(n)
  real, intent(in) :: b(n)
  integer, intent(in) :: n
  integer :: i
  do i = 1, n - 5
    a(i) = b(i+3) + b(i) + b(i+2) + b(i+1) + b(i+4) + b(i+5)
  end do
end subroutine
```

@huaatian Could you take a look?


## Comments

### Comment 1 - huaatian

OK, I'll handle this issue.

---

### Comment 2 - huaatian

My colleague also found this problem, and we have already submitted a patch. You can try it out to see the effect.
https://github.com/llvm/llvm-project/pull/132352

---

### Comment 3 - kawashima-fj

@huaatian Thanks! I confirmed #132352 fixes the assertion failure when compiling the reproducer attached in this issue and my original Fortran program. I'll wait for the merge.

---

### Comment 4 - kawashima-fj

Fixed by #132352

---

