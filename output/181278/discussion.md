# [Flang] Internal error when compiling a structure constructor

**Author:** boo-aboutme
**URL:** https://github.com/llvm/llvm-project/issues/181278
**Status:** Open
**Labels:** crash, flang

## Body

``` text
Version of flang: 23.0.0git (https://github.com/llvm/llvm-project.git a563e6bb7ed485e0ab88717bb4a8c5e8522d9792)
Target: aarch64-unknown-linux-gnu
```

Flang causes an internal error while compiling the sample program shown below.

test.f90

``` fortran
program main
  type ty0(k)
    integer,kind::k
    integer::ii
  end type
  type ty(k,l)
   integer,kind::k
   integer,len ::l
   type(ty0(2))::cmp(1)= [ty0(2)(4)]
  end type
  type(ty(2,4))::obj
end program
```

No compiler options are needed.

``` text
$ flang test.f90
flang: /work/groups/ssoft/compiler/llvm/src/llvm-main/flang/lib/Lower/ConvertConstant.cpp:535: mlir::Value genInlinedStructureCtorLitImpl(Fortran::lower::AbstractConverter&, mlir::Location, const Fortran::evaluate::StructureConstructor&, mlir::Type): Assertion `componentParentType && "failed to retrieve component parent type"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: /work/groups/ssoft/compiler/llvm/aarch64/main-20260209-a563e6bb7ed4/bin/flang -fc1 -triple aarch64-unknown-linux-gnu -emit-obj -mrelocation-model pic -pic-level 2 -pic-is-pie -target-cpu generic -target-feature +outline-atomics -target-feature +v8a -target-feature +fp-armv8 -target-feature +neon -resource-dir /work/groups/ssoft/compiler/llvm/aarch64/main-20260209-a563e6bb7ed4/lib/clang/23 -mframe-pointer=non-leaf-no-reserve -o /work/home/boo/Spack/tmp/test-418bc7.o -x f95 test.f90
 #0 0x0000ffff7565d1a0 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/work/groups/ssoft/compiler/llvm/aarch64/main-20260209-a563e6bb7ed4/bin/../lib/libLLVM.so.23.0git+0x638a1a0)
 #1 0x0000ffff7565a770 llvm::sys::RunSignalHandlers() (/work/groups/ssoft/compiler/llvm/aarch64/main-20260209-a563e6bb7ed4/bin/../lib/libLLVM.so.23.0git+0x6387770)
 #2 0x0000ffff7565a8c8 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x0000ffff86c957f0 (linux-vdso.so.1+0x7f0)
 #4 0x0000ffff6eea3a78 __pthread_kill_implementation (/lib64/libc.so.6+0x82a78)
 #5 0x0000ffff6ee5bcfc gsignal (/lib64/libc.so.6+0x3acfc)
 #6 0x0000ffff6ee48274 abort (/lib64/libc.so.6+0x27274)
 #7 0x0000ffff6ee55694 __assert_fail_base (/lib64/libc.so.6+0x34694)
 #8 0x0000ffff6ee556ec (/lib64/libc.so.6+0x346ec)
 #9 0x00000000066f1ea8 genInlinedStructureCtorLitImpl(Fortran::lower::AbstractConverter&, mlir::Location, Fortran::evaluate::StructureConstructor const&, mlir::Type) ConvertConstant.cpp:0:0
#10 0x00000000066f54b4 mlir::Value genInlinedArrayLit<Fortran::evaluate::SomeKind<(Fortran::common::TypeCategory)6>>(Fortran::lower::AbstractConverter&, mlir::Location, mlir::Type, Fortran::evaluate::Constant<Fortran::evaluate::SomeKind<(Fortran::common::TypeCategory)6>> const&) ConvertConstant.cpp:0:0
(omitted below)
```

Fujitsu Fortran compiler can compile the program:

``` text
$ frt --version
frt (FRT) 4.12.1 20250711
Copyright FUJITSU LIMITED 2019-2025
$ frt test.f90
Fortran diagnostic messages: program name(main)
  jwd2004i-i  "test.f90", line 11: 'obj' is set but never used.
```

GNU Fortran doesn't cause any internal errors, but it claims there is an error in the program.

``` text
$ gfortran test.f90
test.f90:9:33:

    9 |    type(ty0(2))::cmp(1)= [ty0(2)(4)]
      |                                 1
Error: Syntax error in array constructor at (1)
```


