# Fatal error regression:  Assertion `SubregToRegSrcInsts.empty() && "can this happen?"' failed on neoverse-v1 targets.

**Author:** scamp-nvidia
**URL:** https://github.com/llvm/llvm-project/issues/151592
**Status:** Closed
**Labels:** llvm:codegen, crash
**Closed Date:** 2025-08-05T14:09:48Z

## Body

The following snippet of code fails when compiled at "-O2" and with a mcpu target of neoverse-v1. 

```
test3.F90:
module repro_module
  implicit none
  private
  public :: run_reproducer
  integer, parameter :: rk = kind(1.0d0)
  ! Abstract container for some grid values
  type :: grid_data_t
    integer :: dummy
  end type grid_data_t
  ! Abstract descriptor with 2D integer array
  type :: desc_t
    integer, allocatable :: val(:,:)
  end type desc_t
  ! Abstract data holder with a complex array and descriptor
  type :: data_t
    complex(rk), allocatable :: buffer(:)
    type(desc_t) :: desc
  end type data_t
contains
  subroutine run_reproducer()
    implicit none
    type(data_t)     :: obj
    type(grid_data_t):: grid

    allocate(obj%buffer(100))
    allocate(obj%desc%val(1,1))
    obj%desc%val(1,1) = 10
    call outer_kernel(obj, grid)
  end subroutine run_reproducer
  subroutine outer_kernel(obj, grid)
    implicit none
    type(data_t),      intent(in) :: obj
    type(grid_data_t), intent(in) :: grid

    call inner_kernel(obj)
  contains
    subroutine inner_kernel(obj)
      type(data_t), intent(in) :: obj
      integer :: n, i, idx, loop_index
      complex(rk) :: local_buffer(size(obj%buffer))
      do loop_index = 1, 10
        idx = 1
        n = obj%desc%val(idx, 1)
        i = compute_index(idx, loop_index, grid)
        local_buffer = cmplx(0.0_rk, 1.0_rk)
        do i = 1, n
          local_buffer(i) = min(real(local_buffer(i), rk), 1.0_rk)
        end do
      end do
    end subroutine inner_kernel
  end subroutine outer_kernel
  function compute_index(a, b, grid) result(index_val)
    integer, intent(in)       :: a, b
    type(grid_data_t), intent(in) :: grid
    integer :: index_val
    index_val = 42
  end function compute_index
end module repro_module
```
Compiling with a recent Flang checkout: 
```
scamp:$ flang -O2  -c test3.f90 -mcpu=neoverse-v1 -v
flang version 22.0.0git (https://github.com/llvm/llvm-project 3e579d93ab50952628a51bda05f3a39f6a5a631c)
Target: aarch64-unknown-linux-gnu
Thread model: posix

Build config: +assertions
Found candidate GCC installation: /usr/lib/gcc/aarch64-redhat-linux/11
Selected GCC installation: /usr/lib/gcc/aarch64-redhat-linux/11
Candidate multilib: .;@m64
Selected multilib: .;@m64
"flang" -fc1 -triple aarch64-unknown-linux-gnu -emit-obj -fcolor-diagnostics -mrelocation-model pic -pic-level 2 -pic-is-pie -target-cpu neoverse-v1 -target-feature +outline-atomics -target-feature +v8.4a -target-feature +aes -target-feature +bf16 -target-feature +ccdp -target-feature +ccidx -target-feature +ccpp -target-feature +complxnum -target-feature +crc -target-feature +dotprod -target-feature +fp-armv8 -target-feature +fp16fml -target-feature +fullfp16 -target-feature +i8mm -target-feature +jsconv -target-feature +lse -target-feature +neon -target-feature +pauth -target-feature +perfmon -target-feature +rand -target-feature +ras -target-feature +rcpc -target-feature +rdm -target-feature +sha2 -target-feature +sha3 -target-feature +sm4 -target-feature +spe -target-feature +ssbs -target-feature +sve -floop-interchange -vectorize-loops -vectorize-slp -resource-dir clang/22 -mframe-pointer=non-leaf -O2 -o test3.o -x f95 test3.f90
flang: /proj/build/llvm/Linux_aarch64/llvm/lib/CodeGen/RegisterCoalescer.cpp:2375: bool {anonymous}::RegisterCoalescer::joinCopy(llvm::MachineInstr*, bool&, llvm::SmallPtrSetImpl<llvm::MachineInstr*>&): Assertion `SubregToRegSrcInsts.empty() && "can this happen?"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.      Program arguments: flang -fc1 -triple aarch64-unknown-linux-gnu -emit-obj -fcolor-diagnostics -mrelocation-model pic -pic-level 2 -pic-is-pie -target-cpu neoverse-v1 -target-feature +outline-atomics -target-feature +v8.4a -target-feature +aes -target-feature +bf16 -target-feature +ccdp -target-feature +ccidx -target-feature +ccpp -target-feature +complxnum -target-feature +crc -target-feature +dotprod -target-feature +fp-armv8 -target-feature +fp16fml -target-feature +fullfp16 -target-feature +i8mm -target-feature +jsconv -target-feature +lse -target-feature +neon -target-feature +pauth -target-feature +perfmon -target-feature +rand -target-feature +ras -target-feature +rcpc -target-feature +rdm -target-feature +sha2 -target-feature +sha3 -target-feature +sm4 -target-feature +spe -target-feature +ssbs -target-feature +sve -floop-interchange -vectorize-loops -vectorize-slp -resource-dir clang/22 -mframe-pointer=non-leaf -O2 -o test3.o -x f95 test3.f90
1.      Running pass 'Function Pass Manager' on module 'FIRModule'.
2.      Running pass 'Register Coalescer' on function '@_QMrepro_moduleFouter_kernelPinner_kernel'
 #0 0x0000000004d1ae58 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) 
 #1 0x0000000004d18230 llvm::sys::RunSignalHandlers()
 #2 0x0000000004d183b4 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x0000ffffa75b3830 (linux-vdso.so.1+0x830)
 #4 0x0000ffffa71b4454 __pthread_kill_implementation (/lib64/libc.so.6+0x92454)
 #5 0x0000ffffa716b320 gsignal (/lib64/libc.so.6+0x49320)
 #6 0x0000ffffa7152224 abort (/lib64/libc.so.6+0x30224)
 #7 0x0000ffffa7164aa0 __assert_fail_base (/lib64/libc.so.6+0x42aa0)
 #8 0x0000ffffa7164b18 (/lib64/libc.so.6+0x42b18)
 #9 0x0000000007925fc0 (anonymous namespace)::RegisterCoalescer::joinCopy(llvm::MachineInstr*, bool&, llvm::SmallPtrSetImpl<llvm::MachineInstr*>&) (.constprop.0) RegisterCoalescer.cpp:0:0
#10 0x00000000079263a4 (anonymous namespace)::RegisterCoalescer::copyCoalesceWorkList(llvm::MutableArrayRef<llvm::MachineInstr*>) RegisterCoalescer.cpp:0:0
#11 0x0000000007926730 (anonymous namespace)::RegisterCoalescer::copyCoalesceInMBB(llvm::MachineBasicBlock*) RegisterCoalescer.cpp:0:0
#12 0x0000000007929a90 (anonymous namespace)::RegisterCoalescer::run(llvm::MachineFunction&) RegisterCoalescer.cpp:0:0
#13 0x000000000792ab7c (anonymous namespace)::RegisterCoalescerLegacy::runOnMachineFunction(llvm::MachineFunction&) RegisterCoalescer.cpp:0:0
#14 0x000000000772e69c llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (.part.0) MachineFunctionPass.cpp:0:0
#15 0x000000000a254f08 llvm::FPPassManager::runOnFunction(llvm::Function&) 
#16 0x000000000a2551c8 llvm::FPPassManager::runOnModule(llvm::Module&)
#17 0x000000000a255a5c llvm::legacy::PassManagerImpl::run(llvm::Module&) 
#18 0x0000000004d6cc10 Fortran::frontend::CodeGenAction::executeAction() 
#19 0x0000000004d532d4 Fortran::frontend::FrontendAction::execute() 
#20 0x0000000004d3d684 Fortran::frontend::CompilerInstance::executeAction(Fortran::frontend::FrontendAction&) 
#21 0x0000000004d5a828 Fortran::frontend::executeCompilerInvocation(Fortran::frontend::CompilerInstance*) 
#22 0x0000000004714684 fc1_main(llvm::ArrayRef<char const*>, char const*) 
#23 0x000000000471425c main 
#24 0x0000ffffa7157a78 __libc_start_call_main (/lib64/libc.so.6+0x35a78)
#25 0x0000ffffa7157b5c __libc_start_main@GLIBC_2.17 (/lib64/libc.so.6+0x35b5c)
#26 0x00000000047128f4 _start 
flang-22: error: unable to execute command: Aborted (core dumped)
flang-22: error: flang frontend command failed due to signal (use -v to see invocation)
flang version 22.0.0git (https://github.com/llvm/llvm-project 3e579d93ab50952628a51bda05f3a39f6a5a631c)
Target: aarch64-unknown-linux-gnu
Thread model: posix
Build config: +assertions
flang-22: note: diagnostic msg:
********************
```
The code compiles correctly at less than "-O2" or for other "-mcpu" targets. For example "neoverse-v2" and "neoverse-n1" compile to completion. 
Note that the code appears to have compiled correctly upto llvm hash 1d6e68e63aa28783ad0de7d0b46238ce95849f2f and definitely fails at hash 88c23ada9a037bca94927796b4cefab54d11a7d1. We don't have LLVM Flang builds for any hashes inbetween, but that's the range where this regression appears to have been introduced.

## Comments

### Comment 1 - scamp-nvidia

I believe #134408 may be responsible here. Can you please take a look @sdesmalen-arm? Thanks! 

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-backend-aarch64

Author: None (scamp-nvidia)

<details>
The following snippet of code fails when compiled at "-O2" and with a mcpu target of neoverse-v1. 

```
test3.F90:
module repro_module
  implicit none
  private
  public :: run_reproducer
  integer, parameter :: rk = kind(1.0d0)
  ! Abstract container for some grid values
  type :: grid_data_t
    integer :: dummy
  end type grid_data_t
  ! Abstract descriptor with 2D integer array
  type :: desc_t
    integer, allocatable :: val(:,:)
  end type desc_t
  ! Abstract data holder with a complex array and descriptor
  type :: data_t
    complex(rk), allocatable :: buffer(:)
    type(desc_t) :: desc
  end type data_t
contains
  subroutine run_reproducer()
    implicit none
    type(data_t)     :: obj
    type(grid_data_t):: grid

    allocate(obj%buffer(100))
    allocate(obj%desc%val(1,1))
    obj%desc%val(1,1) = 10
    call outer_kernel(obj, grid)
  end subroutine run_reproducer
  subroutine outer_kernel(obj, grid)
    implicit none
    type(data_t),      intent(in) :: obj
    type(grid_data_t), intent(in) :: grid

    call inner_kernel(obj)
  contains
    subroutine inner_kernel(obj)
      type(data_t), intent(in) :: obj
      integer :: n, i, idx, loop_index
      complex(rk) :: local_buffer(size(obj%buffer))
      do loop_index = 1, 10
        idx = 1
        n = obj%desc%val(idx, 1)
        i = compute_index(idx, loop_index, grid)
        local_buffer = cmplx(0.0_rk, 1.0_rk)
        do i = 1, n
          local_buffer(i) = min(real(local_buffer(i), rk), 1.0_rk)
        end do
      end do
    end subroutine inner_kernel
  end subroutine outer_kernel
  function compute_index(a, b, grid) result(index_val)
    integer, intent(in)       :: a, b
    type(grid_data_t), intent(in) :: grid
    integer :: index_val
    index_val = 42
  end function compute_index
end module repro_module
```
Compiling with a recent Flang checkout: 
```
scamp@<!-- -->c7g-16xlarge-amzn1:/local/home/scamp$ flang -O2  -c test3.f90 -mcpu=neoverse-v1 -v
flang version 22.0.0git (https://github.com/llvm/llvm-project 3e579d93ab50952628a51bda05f3a39f6a5a631c)
Target: aarch64-unknown-linux-gnu
Thread model: posix
InstalledDir: /proj/nv/llvm/Linux_aarch64/llvm-3576/bin
Build config: +assertions
Found candidate GCC installation: /usr/lib/gcc/aarch64-redhat-linux/11
Selected GCC installation: /usr/lib/gcc/aarch64-redhat-linux/11
Candidate multilib: .;@<!-- -->m64
Selected multilib: .;@<!-- -->m64
 "/proj/nv/llvm/Linux_aarch64/llvm-3576/bin/flang" -fc1 -triple aarch64-unknown-linux-gnu -emit-obj -fcolor-diagnostics -mrelocation-model pic -pic-level 2 -pic-is-pie -target-cpu neoverse-v1 -target-feature +outline-atomics -target-feature +v8.4a -target-feature +aes -target-feature +bf16 -target-feature +ccdp -target-feature +ccidx -target-feature +ccpp -target-feature +complxnum -target-feature +crc -target-feature +dotprod -target-feature +fp-armv8 -target-feature +fp16fml -target-feature +fullfp16 -target-feature +i8mm -target-feature +jsconv -target-feature +lse -target-feature +neon -target-feature +pauth -target-feature +perfmon -target-feature +rand -target-feature +ras -target-feature +rcpc -target-feature +rdm -target-feature +sha2 -target-feature +sha3 -target-feature +sm4 -target-feature +spe -target-feature +ssbs -target-feature +sve -floop-interchange -vectorize-loops -vectorize-slp -resource-dir /proj/nv/llvm/Linux_aarch64/llvm-3576/lib/clang/22 -mframe-pointer=non-leaf -O2 -o test3.o -x f95 test3.f90
flang: /proj/build/llvm/Linux_aarch64/llvm/lib/CodeGen/RegisterCoalescer.cpp:2375: bool {anonymous}::RegisterCoalescer::joinCopy(llvm::MachineInstr*, bool&amp;, llvm::SmallPtrSetImpl&lt;llvm::MachineInstr*&gt;&amp;): Assertion `SubregToRegSrcInsts.empty() &amp;&amp; "can this happen?"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.      Program arguments: /proj/nv/llvm/Linux_aarch64/llvm-3576/bin/flang -fc1 -triple aarch64-unknown-linux-gnu -emit-obj -fcolor-diagnostics -mrelocation-model pic -pic-level 2 -pic-is-pie -target-cpu neoverse-v1 -target-feature +outline-atomics -target-feature +v8.4a -target-feature +aes -target-feature +bf16 -target-feature +ccdp -target-feature +ccidx -target-feature +ccpp -target-feature +complxnum -target-feature +crc -target-feature +dotprod -target-feature +fp-armv8 -target-feature +fp16fml -target-feature +fullfp16 -target-feature +i8mm -target-feature +jsconv -target-feature +lse -target-feature +neon -target-feature +pauth -target-feature +perfmon -target-feature +rand -target-feature +ras -target-feature +rcpc -target-feature +rdm -target-feature +sha2 -target-feature +sha3 -target-feature +sm4 -target-feature +spe -target-feature +ssbs -target-feature +sve -floop-interchange -vectorize-loops -vectorize-slp -resource-dir /proj/nv/llvm/Linux_aarch64/llvm-3576/lib/clang/22 -mframe-pointer=non-leaf -O2 -o test3.o -x f95 test3.f90
1.      Running pass 'Function Pass Manager' on module 'FIRModule'.
2.      Running pass 'Register Coalescer' on function '@<!-- -->_QMrepro_moduleFouter_kernelPinner_kernel'
 #<!-- -->0 0x0000000004d1ae58 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/proj/nv/llvm/Linux_aarch64/llvm-3576/bin/flang+0x4d1ae58)
 #<!-- -->1 0x0000000004d18230 llvm::sys::RunSignalHandlers() (/proj/nv/llvm/Linux_aarch64/llvm-3576/bin/flang+0x4d18230)
 #<!-- -->2 0x0000000004d183b4 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x0000ffffa75b3830 (linux-vdso.so.1+0x830)
 #<!-- -->4 0x0000ffffa71b4454 __pthread_kill_implementation (/lib64/libc.so.6+0x92454)
 #<!-- -->5 0x0000ffffa716b320 gsignal (/lib64/libc.so.6+0x49320)
 #<!-- -->6 0x0000ffffa7152224 abort (/lib64/libc.so.6+0x30224)
 #<!-- -->7 0x0000ffffa7164aa0 __assert_fail_base (/lib64/libc.so.6+0x42aa0)
 #<!-- -->8 0x0000ffffa7164b18 (/lib64/libc.so.6+0x42b18)
 #<!-- -->9 0x0000000007925fc0 (anonymous namespace)::RegisterCoalescer::joinCopy(llvm::MachineInstr*, bool&amp;, llvm::SmallPtrSetImpl&lt;llvm::MachineInstr*&gt;&amp;) (.constprop.0) RegisterCoalescer.cpp:0:0
#<!-- -->10 0x00000000079263a4 (anonymous namespace)::RegisterCoalescer::copyCoalesceWorkList(llvm::MutableArrayRef&lt;llvm::MachineInstr*&gt;) RegisterCoalescer.cpp:0:0
#<!-- -->11 0x0000000007926730 (anonymous namespace)::RegisterCoalescer::copyCoalesceInMBB(llvm::MachineBasicBlock*) RegisterCoalescer.cpp:0:0
#<!-- -->12 0x0000000007929a90 (anonymous namespace)::RegisterCoalescer::run(llvm::MachineFunction&amp;) RegisterCoalescer.cpp:0:0
#<!-- -->13 0x000000000792ab7c (anonymous namespace)::RegisterCoalescerLegacy::runOnMachineFunction(llvm::MachineFunction&amp;) RegisterCoalescer.cpp:0:0
#<!-- -->14 0x000000000772e69c llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (.part.0) MachineFunctionPass.cpp:0:0
#<!-- -->15 0x000000000a254f08 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (/proj/nv/llvm/Linux_aarch64/llvm-3576/bin/flang+0xa254f08)
#<!-- -->16 0x000000000a2551c8 llvm::FPPassManager::runOnModule(llvm::Module&amp;) (/proj/nv/llvm/Linux_aarch64/llvm-3576/bin/flang+0xa2551c8)
#<!-- -->17 0x000000000a255a5c llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/proj/nv/llvm/Linux_aarch64/llvm-3576/bin/flang+0xa255a5c)
#<!-- -->18 0x0000000004d6cc10 Fortran::frontend::CodeGenAction::executeAction() (/proj/nv/llvm/Linux_aarch64/llvm-3576/bin/flang+0x4d6cc10)
#<!-- -->19 0x0000000004d532d4 Fortran::frontend::FrontendAction::execute() (/proj/nv/llvm/Linux_aarch64/llvm-3576/bin/flang+0x4d532d4)
#<!-- -->20 0x0000000004d3d684 Fortran::frontend::CompilerInstance::executeAction(Fortran::frontend::FrontendAction&amp;) (/proj/nv/llvm/Linux_aarch64/llvm-3576/bin/flang+0x4d3d684)
#<!-- -->21 0x0000000004d5a828 Fortran::frontend::executeCompilerInvocation(Fortran::frontend::CompilerInstance*) (/proj/nv/llvm/Linux_aarch64/llvm-3576/bin/flang+0x4d5a828)
#<!-- -->22 0x0000000004714684 fc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*) (/proj/nv/llvm/Linux_aarch64/llvm-3576/bin/flang+0x4714684)
#<!-- -->23 0x000000000471425c main (/proj/nv/llvm/Linux_aarch64/llvm-3576/bin/flang+0x471425c)
#<!-- -->24 0x0000ffffa7157a78 __libc_start_call_main (/lib64/libc.so.6+0x35a78)
#<!-- -->25 0x0000ffffa7157b5c __libc_start_main@<!-- -->GLIBC_2.17 (/lib64/libc.so.6+0x35b5c)
#<!-- -->26 0x00000000047128f4 _start (/proj/nv/llvm/Linux_aarch64/llvm-3576/bin/flang+0x47128f4)
flang-22: error: unable to execute command: Aborted (core dumped)
flang-22: error: flang frontend command failed due to signal (use -v to see invocation)
flang version 22.0.0git (https://github.com/llvm/llvm-project 3e579d93ab50952628a51bda05f3a39f6a5a631c)
Target: aarch64-unknown-linux-gnu
Thread model: posix
InstalledDir: /proj/nv/llvm/Linux_aarch64/llvm-3576/bin
Build config: +assertions
flang-22: note: diagnostic msg:
********************
```
The code compiles correctly at less than "-O2" or for other "-mcpu" targets. For example "neoverse-v2" and "neoverse-n1" compile to completion. 
Note that the code appears to have compiled correctly upto llvm hash 1d6e68e63aa28783ad0de7d0b46238ce95849f2f and definitely fails at hash 88c23ada9a037bca94927796b4cefab54d11a7d1. We don't have LLVM Flang builds for any hashes inbetween, but that's the range where this regression appears to have been introduced.
</details>


---

### Comment 3 - davemgreen

There is an .ll example in https://godbolt.org/z/enYhP11an in case it is helpful (although godbolt maybe hasn't caught up with trunk yet).

---

### Comment 4 - scamp-nvidia

#134408 resolved this issue. 

---

