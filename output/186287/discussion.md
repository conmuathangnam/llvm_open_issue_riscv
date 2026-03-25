# [Flang][OpenMP] Declare reduction directive using an undefined derived type terminates abnormally during compilation

**Author:** ohno-fj
**URL:** https://github.com/llvm/llvm-project/issues/186287
**Status:** Open
**Labels:** flang:frontend, crash, flang:openmp

## Body

```
Version of flang : 23.0.0(ae282974b8a40e20a4c55dfffba84d24db843d74)/AArch64
```

The attached program (`err_sngg002e_2.f90`) is incorrect.
In this program, although the derived type is not defined, `declare reduction directive` is written using that derived type.
In this case, the compilation terminates abnormally.
Error checking should be performed for this program.

The following are the test program, Flang, Gfortran and ifx compilation/execution results.

err_sngg002e_2.f90:
```fortran
program main
  implicit none
!  type x
!     integer::xx
!  end type x

!$omp declare reduction(gen:x:omp_out=gen(omp_in,omp_out))
  type(x)    ::t1
!$omp parallel   reduction(gen:t1)
!$omp endparallel

end program main
```

```
$ flang -fopenmp err_sngg002e_2.f90
flang: /work/groups/ssoft/compiler/llvm/src/llvm-main/flang/lib/Semantics/check-omp-structure.cpp:3847: void Fortran::semantics::OmpStructureChecker::CheckReductionObjectTypes(const Fortran::parser::OmpObjectList&, const Fortran::parser:
:OmpReductionIdentifier&): Assertion `IsProcedurePointer(*symbol) && "Unexpected symbol properties"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: /work/groups/ssoft/compiler/llvm/aarch64/main-20260309-ae282974b8a4/bin/flang -fc1 -triple aarch64-unknown-linux-gnu -emit-obj -mrelocation-model pic -pic-level 2 -pic-is-pie -target-cpu generic -target-feature
 +v8a -target-feature +fp-armv8 -target-feature +neon -fopenmp -resource-dir /work/groups/ssoft/compiler/llvm/aarch64/main-20260309-ae282974b8a4/lib/clang/23 -mframe-pointer=non-leaf-no-reserve -o /tmp/err_sngg002e_2-a829e3.o -x f95 err_
sngg002e_2.f90
 #0 0x000040001e0a8638 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/work/groups/ssoft/compiler/llvm/aarch64/main-latest/lib/libLLVM.so.23.0git+0x6588638)
 #1 0x000040001e0a5958 llvm::sys::RunSignalHandlers() (/work/groups/ssoft/compiler/llvm/aarch64/main-latest/lib/libLLVM.so.23.0git+0x6585958)
 #2 0x000040001e0a5ab0 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x000040000afb07a0 (linux-vdso.so.1+0x7a0)
 #4 0x00004000233962e4 raise (/lib64/libc.so.6+0x362e4)
 #5 0x0000400023380aac abort (/lib64/libc.so.6+0x20aac)
 #6 0x000040002338fc24 __assert_fail_base (/lib64/libc.so.6+0x2fc24)
 #7 0x000040002338fc98 __assert_perror_fail (/lib64/libc.so.6+0x2fc98)
 #8 0x0000000006d29f0c Fortran::semantics::OmpStructureChecker::CheckReductionObjectTypes(Fortran::parser::OmpObjectList const&, Fortran::parser::OmpReductionIdentifier const&) (/work/groups/ssoft/compiler/llvm/aarch64/main-20260309-ae28
2974b8a4/bin/flang+0x6d29f0c)
 #9 0x0000000006d5c1b0 Fortran::semantics::OmpStructureChecker::Enter(Fortran::parser::OmpClause::Reduction const&) (/work/groups/ssoft/compiler/llvm/aarch64/main-20260309-ae282974b8a4/bin/flang+0x6d5c1b0)
  - Omitted -
#16 0x0000000006c1d6d4 Fortran::semantics::Semantics::Perform() (/work/groups/ssoft/compiler/llvm/aarch64/main-20260309-ae282974b8a4/bin/flang+0x6c1d6d4)
#17 0x00000000063d8250 Fortran::frontend::FrontendAction::runSemanticChecks() (/work/groups/ssoft/compiler/llvm/aarch64/main-20260309-ae282974b8a4/bin/flang+0x63d8250)
#18 0x00000000063ecf6c Fortran::frontend::CodeGenAction::beginSourceFileAction() (/work/groups/ssoft/compiler/llvm/aarch64/main-20260309-ae282974b8a4/bin/flang+0x63ecf6c)
#19 0x00000000063da704 Fortran::frontend::FrontendAction::beginSourceFile(Fortran::frontend::CompilerInstance&, Fortran::frontend::FrontendInputFile const&) (/work/groups/ssoft/compiler/llvm/aarch64/main-20260309-ae282974b8a4/bin/flang+0
x63da704)
#20 0x00000000063c701c Fortran::frontend::CompilerInstance::executeAction(Fortran::frontend::FrontendAction&) (/work/groups/ssoft/compiler/llvm/aarch64/main-20260309-ae282974b8a4/bin/flang+0x63c701c)
#21 0x00000000063e0c6c Fortran::frontend::executeCompilerInvocation(Fortran::frontend::CompilerInstance*) (/work/groups/ssoft/compiler/llvm/aarch64/main-20260309-ae282974b8a4/bin/flang+0x63e0c6c)
#22 0x00000000063bf938 fc1_main(llvm::ArrayRef<char const*>, char const*) (/work/groups/ssoft/compiler/llvm/aarch64/main-20260309-ae282974b8a4/bin/flang+0x63bf938)
#23 0x00000000063bf144 main (/work/groups/ssoft/compiler/llvm/aarch64/main-20260309-ae282974b8a4/bin/flang+0x63bf144)
#24 0x0000400023384404 __libc_start_main (/lib64/libc.so.6+0x24404)
#25 0x00000000063bd1bc _start (/work/groups/ssoft/compiler/llvm/aarch64/main-20260309-ae282974b8a4/bin/flang+0x63bd1bc)
flang-23: error: unable to execute command: Aborted (core dumped)
flang-23: error: flang frontend command failed with exit code -2 (use -v to see invocation)
flang version 23.0.0git (https://github.com/llvm/llvm-project.git ae282974b8a40e20a4c55dfffba84d24db843d74)
Target: aarch64-unknown-linux-gnu
Thread model: posix
InstalledDir: /work/groups/ssoft/compiler/llvm/aarch64/main-20260309-ae282974b8a4/bin
Build config: +assertions
flang-23: note: diagnostic msg:
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
flang-23: note: diagnostic msg: /tmp/err_sngg002e_2-02346f
flang-23: note: diagnostic msg: /tmp/err_sngg002e_2-02346f.sh
flang-23: note: diagnostic msg:

********************
$
```

`$ cat /tmp/err_sngg002e_2-02346f`

```fortran
#line "./err_sngg002e_2.f90" 1
      program main
      implicit none




!$omp declare reduction(gen:x:omp_out=gen(omp_in,omp_out))
      type(x) ::t1
!$omp parallel   reduction(gen:t1)
!$omp endparallel

      end program main
$
```

```
$ cat /tmp/err_sngg002e_2-02346f.sh
# Crash reproducer for clang version 23.0.0git (https://github.com/llvm/llvm-project.git ae282974b8a40e20a4c55dfffba84d24db843d74)
# Driver args: "-fopenmp" "err_sngg002e_2.f90"
# Original command:  "/work/groups/ssoft/compiler/llvm/aarch64/main-20260309-ae282974b8a4/bin/flang" "-fc1" "-triple" "aarch64-unknown-linux-gnu" "-emit-obj" "-mrelocation-model" "pic" "-pic-level" "2" "-pic-is-pie" "-target-cpu" "generi
c" "-target-feature" "+v8a" "-target-feature" "+fp-armv8" "-target-feature" "+neon" "-fopenmp" "-resource-dir" "/work/groups/ssoft/compiler/llvm/aarch64/main-20260309-ae282974b8a4/lib/clang/23" "-mframe-pointer=non-leaf-no-reserve" "-o"
"/tmp/err_sngg002e_2-a829e3.o" "-x" "f95" "err_sngg002e_2.f90"
 "/work/groups/ssoft/compiler/llvm/aarch64/main-20260309-ae282974b8a4/bin/flang" "-fc1" "-triple" "aarch64-unknown-linux-gnu" "-emit-obj" "-mrelocation-model" "pic" "-pic-level" "2" "-pic-is-pie" "-target-cpu" "generic" "-target-feature"
 "+v8a" "-target-feature" "+fp-armv8" "-target-feature" "+neon" "-fopenmp" "-mframe-pointer=non-leaf-no-reserve" "-x" "f95" "err_sngg002e_2-02346f"
$
```

```
$ gfortran -fopenmp err_sngg002e_2.f90
err_sngg002e_2.f90:7:7:

    7 | !$omp declare reduction(gen:x:omp_out=gen(omp_in,omp_out))
      |       1
Error: Unclassifiable OpenMP directive at (1)
err_sngg002e_2.f90:8:10:

    8 |   type(x)    ::t1
      |          1
Error: Derived type ‘x’ at (1) is being used before it is defined
err_sngg002e_2.f90:9:27:

    9 | !$omp parallel   reduction(gen:t1)
      |                           1
Error: !$OMP DECLARE REDUCTION gen not found at (1)
err_sngg002e_2.f90:9:33:

    9 | !$omp parallel   reduction(gen:t1)
      |                                 1
Error: Symbol ‘t1’ at (1) has no IMPLICIT type
$
```

```
$ ifx -qopenmp err_sngg002e_2.f90
err_sngg002e_2.f90(7): error #8863: Derived type is not found or is inaccessible.
!$omp declare reduction(gen:x:omp_out=gen(omp_in,omp_out))
------^
err_sngg002e_2.f90(8): error #6457: This derived type name has not been declared.   [X]
  type(x)    ::t1
-------^
err_sngg002e_2.f90(9): error #6407: This symbolic name is not an intrinsic function name or an intrinsic s\
ubroutine name.   [GEN]
!$omp parallel   reduction(gen:t1)
---------------------------^
err_sngg002e_2.f90(9): error #7709: The symbolic name must be one of the intrinsic functions MAX, MIN, IAN\
D, IOR, or IEOR.   [GEN]
!$omp parallel   reduction(gen:t1)
---------------------------^
err_sngg002e_2.f90(9): error #6404: This name does not have a type, and must have an explicit type.   [T1]
!$omp parallel   reduction(gen:t1)
-------------------------------^
compilation aborted for err_sngg002e_2.f90 (code 1)
$
```


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-flang-frontend

Author: ohno-fj (ohno-fj)

<details>
```
Version of flang : 23.0.0(ae282974b8a40e20a4c55dfffba84d24db843d74)/AArch64
```

The attached program (`err_sngg002e_2.f90`) is incorrect.
In this program, although the derived type is not defined, `declare reduction directive` is written using that derived type.
In this case, the compilation terminates abnormally.
Error checking should be performed for this program.

The following are the test program, Flang, Gfortran and ifx compilation/execution results.

err_sngg002e_2.f90:
```fortran
program main
  implicit none
!  type x
!     integer::xx
!  end type x

!$omp declare reduction(gen:x:omp_out=gen(omp_in,omp_out))
  type(x)    ::t1
!$omp parallel   reduction(gen:t1)
!$omp endparallel

end program main
```

```
$ flang -fopenmp err_sngg002e_2.f90
flang: /work/groups/ssoft/compiler/llvm/src/llvm-main/flang/lib/Semantics/check-omp-structure.cpp:3847: void Fortran::semantics::OmpStructureChecker::CheckReductionObjectTypes(const Fortran::parser::OmpObjectList&amp;, const Fortran::parser:
:OmpReductionIdentifier&amp;): Assertion `IsProcedurePointer(*symbol) &amp;&amp; "Unexpected symbol properties"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: /work/groups/ssoft/compiler/llvm/aarch64/main-20260309-ae282974b8a4/bin/flang -fc1 -triple aarch64-unknown-linux-gnu -emit-obj -mrelocation-model pic -pic-level 2 -pic-is-pie -target-cpu generic -target-feature
 +v8a -target-feature +fp-armv8 -target-feature +neon -fopenmp -resource-dir /work/groups/ssoft/compiler/llvm/aarch64/main-20260309-ae282974b8a4/lib/clang/23 -mframe-pointer=non-leaf-no-reserve -o /tmp/err_sngg002e_2-a829e3.o -x f95 err_
sngg002e_2.f90
 #<!-- -->0 0x000040001e0a8638 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/work/groups/ssoft/compiler/llvm/aarch64/main-latest/lib/libLLVM.so.23.0git+0x6588638)
 #<!-- -->1 0x000040001e0a5958 llvm::sys::RunSignalHandlers() (/work/groups/ssoft/compiler/llvm/aarch64/main-latest/lib/libLLVM.so.23.0git+0x6585958)
 #<!-- -->2 0x000040001e0a5ab0 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x000040000afb07a0 (linux-vdso.so.1+0x7a0)
 #<!-- -->4 0x00004000233962e4 raise (/lib64/libc.so.6+0x362e4)
 #<!-- -->5 0x0000400023380aac abort (/lib64/libc.so.6+0x20aac)
 #<!-- -->6 0x000040002338fc24 __assert_fail_base (/lib64/libc.so.6+0x2fc24)
 #<!-- -->7 0x000040002338fc98 __assert_perror_fail (/lib64/libc.so.6+0x2fc98)
 #<!-- -->8 0x0000000006d29f0c Fortran::semantics::OmpStructureChecker::CheckReductionObjectTypes(Fortran::parser::OmpObjectList const&amp;, Fortran::parser::OmpReductionIdentifier const&amp;) (/work/groups/ssoft/compiler/llvm/aarch64/main-20260309-ae28
2974b8a4/bin/flang+0x6d29f0c)
 #<!-- -->9 0x0000000006d5c1b0 Fortran::semantics::OmpStructureChecker::Enter(Fortran::parser::OmpClause::Reduction const&amp;) (/work/groups/ssoft/compiler/llvm/aarch64/main-20260309-ae282974b8a4/bin/flang+0x6d5c1b0)
  - Omitted -
#<!-- -->16 0x0000000006c1d6d4 Fortran::semantics::Semantics::Perform() (/work/groups/ssoft/compiler/llvm/aarch64/main-20260309-ae282974b8a4/bin/flang+0x6c1d6d4)
#<!-- -->17 0x00000000063d8250 Fortran::frontend::FrontendAction::runSemanticChecks() (/work/groups/ssoft/compiler/llvm/aarch64/main-20260309-ae282974b8a4/bin/flang+0x63d8250)
#<!-- -->18 0x00000000063ecf6c Fortran::frontend::CodeGenAction::beginSourceFileAction() (/work/groups/ssoft/compiler/llvm/aarch64/main-20260309-ae282974b8a4/bin/flang+0x63ecf6c)
#<!-- -->19 0x00000000063da704 Fortran::frontend::FrontendAction::beginSourceFile(Fortran::frontend::CompilerInstance&amp;, Fortran::frontend::FrontendInputFile const&amp;) (/work/groups/ssoft/compiler/llvm/aarch64/main-20260309-ae282974b8a4/bin/flang+0
x63da704)
#<!-- -->20 0x00000000063c701c Fortran::frontend::CompilerInstance::executeAction(Fortran::frontend::FrontendAction&amp;) (/work/groups/ssoft/compiler/llvm/aarch64/main-20260309-ae282974b8a4/bin/flang+0x63c701c)
#<!-- -->21 0x00000000063e0c6c Fortran::frontend::executeCompilerInvocation(Fortran::frontend::CompilerInstance*) (/work/groups/ssoft/compiler/llvm/aarch64/main-20260309-ae282974b8a4/bin/flang+0x63e0c6c)
#<!-- -->22 0x00000000063bf938 fc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*) (/work/groups/ssoft/compiler/llvm/aarch64/main-20260309-ae282974b8a4/bin/flang+0x63bf938)
#<!-- -->23 0x00000000063bf144 main (/work/groups/ssoft/compiler/llvm/aarch64/main-20260309-ae282974b8a4/bin/flang+0x63bf144)
#<!-- -->24 0x0000400023384404 __libc_start_main (/lib64/libc.so.6+0x24404)
#<!-- -->25 0x00000000063bd1bc _start (/work/groups/ssoft/compiler/llvm/aarch64/main-20260309-ae282974b8a4/bin/flang+0x63bd1bc)
flang-23: error: unable to execute command: Aborted (core dumped)
flang-23: error: flang frontend command failed with exit code -2 (use -v to see invocation)
flang version 23.0.0git (https://github.com/llvm/llvm-project.git ae282974b8a40e20a4c55dfffba84d24db843d74)
Target: aarch64-unknown-linux-gnu
Thread model: posix
InstalledDir: /work/groups/ssoft/compiler/llvm/aarch64/main-20260309-ae282974b8a4/bin
Build config: +assertions
flang-23: note: diagnostic msg:
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
flang-23: note: diagnostic msg: /tmp/err_sngg002e_2-02346f
flang-23: note: diagnostic msg: /tmp/err_sngg002e_2-02346f.sh
flang-23: note: diagnostic msg:

********************
$
```

```
$ cat /tmp/err_sngg002e_2-02346f
#line "./err_sngg002e_2.f90" 1
      program main
      implicit none




!$omp declare reduction(gen:x:omp_out=gen(omp_in,omp_out))
      type(x) ::t1
!$omp parallel   reduction(gen:t1)
!$omp endparallel

      end program main
$
```

```
$ cat /tmp/err_sngg002e_2-02346f.sh
# Crash reproducer for clang version 23.0.0git (https://github.com/llvm/llvm-project.git ae282974b8a40e20a4c55dfffba84d24db843d74)
# Driver args: "-fopenmp" "err_sngg002e_2.f90"
# Original command:  "/work/groups/ssoft/compiler/llvm/aarch64/main-20260309-ae282974b8a4/bin/flang" "-fc1" "-triple" "aarch64-unknown-linux-gnu" "-emit-obj" "-mrelocation-model" "pic" "-pic-level" "2" "-pic-is-pie" "-target-cpu" "generi
c" "-target-feature" "+v8a" "-target-feature" "+fp-armv8" "-target-feature" "+neon" "-fopenmp" "-resource-dir" "/work/groups/ssoft/compiler/llvm/aarch64/main-20260309-ae282974b8a4/lib/clang/23" "-mframe-pointer=non-leaf-no-reserve" "-o"
"/tmp/err_sngg002e_2-a829e3.o" "-x" "f95" "err_sngg002e_2.f90"
 "/work/groups/ssoft/compiler/llvm/aarch64/main-20260309-ae282974b8a4/bin/flang" "-fc1" "-triple" "aarch64-unknown-linux-gnu" "-emit-obj" "-mrelocation-model" "pic" "-pic-level" "2" "-pic-is-pie" "-target-cpu" "generic" "-target-feature"
 "+v8a" "-target-feature" "+fp-armv8" "-target-feature" "+neon" "-fopenmp" "-mframe-pointer=non-leaf-no-reserve" "-x" "f95" "err_sngg002e_2-02346f"
$
```

```
$ gfortran -fopenmp err_sngg002e_2.f90
err_sngg002e_2.f90:7:7:

    7 | !$omp declare reduction(gen:x:omp_out=gen(omp_in,omp_out))
      |       1
Error: Unclassifiable OpenMP directive at (1)
err_sngg002e_2.f90:8:10:

    8 |   type(x)    ::t1
      |          1
Error: Derived type ‘x’ at (1) is being used before it is defined
err_sngg002e_2.f90:9:27:

    9 | !$omp parallel   reduction(gen:t1)
      |                           1
Error: !$OMP DECLARE REDUCTION gen not found at (1)
err_sngg002e_2.f90:9:33:

    9 | !$omp parallel   reduction(gen:t1)
      |                                 1
Error: Symbol ‘t1’ at (1) has no IMPLICIT type
$
```

```
$ ifx -qopenmp err_sngg002e_2.f90
err_sngg002e_2.f90(7): error #<!-- -->8863: Derived type is not found or is inaccessible.
!$omp declare reduction(gen:x:omp_out=gen(omp_in,omp_out))
------^
err_sngg002e_2.f90(8): error #<!-- -->6457: This derived type name has not been declared.   [X]
  type(x)    ::t1
-------^
err_sngg002e_2.f90(9): error #<!-- -->6407: This symbolic name is not an intrinsic function name or an intrinsic s\
ubroutine name.   [GEN]
!$omp parallel   reduction(gen:t1)
---------------------------^
err_sngg002e_2.f90(9): error #<!-- -->7709: The symbolic name must be one of the intrinsic functions MAX, MIN, IAN\
D, IOR, or IEOR.   [GEN]
!$omp parallel   reduction(gen:t1)
---------------------------^
err_sngg002e_2.f90(9): error #<!-- -->6404: This name does not have a type, and must have an explicit type.   [T1]
!$omp parallel   reduction(gen:t1)
-------------------------------^
compilation aborted for err_sngg002e_2.f90 (code 1)
$
```

</details>


---

