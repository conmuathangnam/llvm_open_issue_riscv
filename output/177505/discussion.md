# [Flang] Multiple definition of pointers to the same procedure cause an internal error

**Author:** boo-aboutme
**URL:** https://github.com/llvm/llvm-project/issues/177505
**Status:** Open
**Labels:** flang:ir, crash

## Body

``` text
Version of flang: 22.0.0git (https://github.com/llvm/llvm-project.git 2f7e218017db69454ea84e1849fcb9a11b1f7fef)
```

Flang causes an internal error while compiling the sample program with multiple pointers to the same procedure shown below.


test.f90

``` fortran
Module m1
  external::sub
  type ty
    procedure(),pointer,nopass :: ptr5=>sub
  end type
  procedure(),pointer:: ptr6=>sub
end module

use m1
  integer::jj =4
  call ptr6(10)
  print*,"Pass"
end

subroutine sub(a)
  integer::a
  print*,"sub"
end subroutine
```

No compiler options are needed.

``` text
$ flang test.f90
flang: /work/groups/ssoft/compiler/llvm/src/llvm-main/flang/lib/Lower/CallInterface.cpp:742: void Fortran::lower::CallInterface<T>::mapPassedEntities() [with T = Fortran::lower::CalleeInterface]: Assertion `inputs.size() == func.front().getArguments().size() && "function previously created with different number of arguments"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: /work/groups/ssoft/compiler/llvm/aarch64/main-20260112-2f7e218017db/bin/flang -fc1 -triple aarch64-unknown-linux-gnu -emit-obj -fcolor-diagnostics -mrelocation-model pic -pic-level 2 -pic-is-pie -target-cpu generic -target-feature +outline-atomics -target-feature +v8a -target-feature +fp-armv8 -target-feature +neon -resource-dir /work/groups/ssoft/compiler/llvm/aarch64/main-20260112-2f7e218017db/lib/clang/22 -mframe-pointer=non-leaf-no-reserve -o /tmp/test-d40af8.o -x f95 test.f90
 #0 0x0000ffff94b7dd10 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/work/groups/ssoft/compiler/llvm/aarch64/main-20260112-2f7e218017db/bin/../lib/libLLVM.so.22.0git+0x6445d10)
 #1 0x0000ffff94b7b2b8 llvm::sys::RunSignalHandlers() (/work/groups/ssoft/compiler/llvm/aarch64/main-20260112-2f7e218017db/bin/../lib/libLLVM.so.22.0git+0x64432b8)
 #2 0x0000ffff94b7b410 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x0000ffffa5d607f0 (linux-vdso.so.1+0x7f0)
 #4 0x0000ffff8e2d4a78 __pthread_kill_implementation (/lib64/libc.so.6+0x82a78)
 #5 0x0000ffff8e28ccfc gsignal (/lib64/libc.so.6+0x3acfc)
 #6 0x0000ffff8e279274 abort (/lib64/libc.so.6+0x27274)
 #7 0x0000ffff8e286694 __assert_fail_base (/lib64/libc.so.6+0x34694)
 #8 0x0000ffff8e2866ec (/lib64/libc.so.6+0x346ec)
 #9 0x00000000063a2c28 Fortran::lower::CalleeInterface::addEntryBlockAndMapArguments() (/work/groups/ssoft/compiler/llvm/aarch64/main-20260112-2f7e218017db/bin/flang+0x63a2c28)
#10 0x00000000063a2dc8 Fortran::lower::CalleeInterface::addEntryBlockAndMapArguments() (/work/groups/ssoft/compiler/llvm/aarch64/main-20260112-2f7e218017db/bin/flang+0x63a2dc8)
#11 0x000000000637dd94 (anonymous namespace)::FirConverter::startNewFunction(Fortran::lower::pft::FunctionLikeUnit&) Bridge.cpp:0:0
#12 0x000000000637fe5c (anonymous namespace)::FirConverter::lowerFunc(Fortran::lower::pft::FunctionLikeUnit&) Bridge.cpp:0:0
#13 0x0000000006381e54 Fortran::lower::LoweringBridge::lower(Fortran::parser::Program const&, Fortran::semantics::SemanticsContext const&) (/work/groups/ssoft/compiler/llvm/aarch64/main-20260112-2f7e218017db/bin/flang+0x6381e54)
#14 0x00000000062357f0 Fortran::frontend::CodeGenAction::beginSourceFileAction() (/work/groups/ssoft/compiler/llvm/aarch64/main-20260112-2f7e218017db/bin/flang+0x62357f0)
#15 0x0000000006222fcc Fortran::frontend::FrontendAction::beginSourceFile(Fortran::frontend::CompilerInstance&, Fortran::frontend::FrontendInputFile const&) (/work/groups/ssoft/compiler/llvm/aarch64/main-20260112-2f7e218017db/bin/flang+0x6222fcc)
#16 0x000000000620f764 Fortran::frontend::CompilerInstance::executeAction(Fortran::frontend::FrontendAction&) (/work/groups/ssoft/compiler/llvm/aarch64/main-20260112-2f7e218017db/bin/flang+0x620f764)
#17 0x000000000622951c Fortran::frontend::executeCompilerInvocation(Fortran::frontend::CompilerInstance*) (/work/groups/ssoft/compiler/llvm/aarch64/main-20260112-2f7e218017db/bin/flang+0x622951c)
#18 0x0000000006207f18 fc1_main(llvm::ArrayRef<char const*>, char const*) (/work/groups/ssoft/compiler/llvm/aarch64/main-20260112-2f7e218017db/bin/flang+0x6207f18)
#19 0x0000000006207910 main (/work/groups/ssoft/compiler/llvm/aarch64/main-20260112-2f7e218017db/bin/flang+0x6207910)
#20 0x0000ffff8e279540 __libc_start_call_main (/lib64/libc.so.6+0x27540)
#21 0x0000ffff8e279618 __libc_start_main@GLIBC_2.17 (/lib64/libc.so.6+0x27618)
#22 0x0000000006205d94 _start (/work/groups/ssoft/compiler/llvm/aarch64/main-20260112-2f7e218017db/bin/flang+0x6205d94)
flang-22: error: unable to execute command: Aborted (core dumped)
flang-22: error: flang frontend command failed due to signal (use -v to see invocation)
flang version 22.0.0git (https://github.com/llvm/llvm-project.git 2f7e218017db69454ea84e1849fcb9a11b1f7fef)
Target: aarch64-unknown-linux-gnu
Thread model: posix
InstalledDir: /work/groups/ssoft/compiler/llvm/aarch64/main-20260112-2f7e218017db/bin
Build config: +assertions
flang-22: note: diagnostic msg:
```


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-flang-ir

Author: Manabu Matsuyama (boo-aboutme)

<details>
``` text
Version of flang: 22.0.0git (https://github.com/llvm/llvm-project.git 2f7e218017db69454ea84e1849fcb9a11b1f7fef)
```

Flang causes an internal error while compiling the sample program with multiple pointers to the same procedure shown below.


test.f90

``` fortran
Module m1
  external::sub
  type ty
    procedure(),pointer,nopass :: ptr5=&gt;sub
  end type
  procedure(),pointer:: ptr6=&gt;sub
end module

use m1
  integer::jj =4
  call ptr6(10)
  print*,"Pass"
end

subroutine sub(a)
  integer::a
  print*,"sub"
end subroutine
```

No compiler options are needed.

``` text
$ flang test.f90
flang: /work/groups/ssoft/compiler/llvm/src/llvm-main/flang/lib/Lower/CallInterface.cpp:742: void Fortran::lower::CallInterface&lt;T&gt;::mapPassedEntities() [with T = Fortran::lower::CalleeInterface]: Assertion `inputs.size() == func.front().getArguments().size() &amp;&amp; "function previously created with different number of arguments"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: /work/groups/ssoft/compiler/llvm/aarch64/main-20260112-2f7e218017db/bin/flang -fc1 -triple aarch64-unknown-linux-gnu -emit-obj -fcolor-diagnostics -mrelocation-model pic -pic-level 2 -pic-is-pie -target-cpu generic -target-feature +outline-atomics -target-feature +v8a -target-feature +fp-armv8 -target-feature +neon -resource-dir /work/groups/ssoft/compiler/llvm/aarch64/main-20260112-2f7e218017db/lib/clang/22 -mframe-pointer=non-leaf-no-reserve -o /tmp/test-d40af8.o -x f95 test.f90
 #<!-- -->0 0x0000ffff94b7dd10 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/work/groups/ssoft/compiler/llvm/aarch64/main-20260112-2f7e218017db/bin/../lib/libLLVM.so.22.0git+0x6445d10)
 #<!-- -->1 0x0000ffff94b7b2b8 llvm::sys::RunSignalHandlers() (/work/groups/ssoft/compiler/llvm/aarch64/main-20260112-2f7e218017db/bin/../lib/libLLVM.so.22.0git+0x64432b8)
 #<!-- -->2 0x0000ffff94b7b410 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x0000ffffa5d607f0 (linux-vdso.so.1+0x7f0)
 #<!-- -->4 0x0000ffff8e2d4a78 __pthread_kill_implementation (/lib64/libc.so.6+0x82a78)
 #<!-- -->5 0x0000ffff8e28ccfc gsignal (/lib64/libc.so.6+0x3acfc)
 #<!-- -->6 0x0000ffff8e279274 abort (/lib64/libc.so.6+0x27274)
 #<!-- -->7 0x0000ffff8e286694 __assert_fail_base (/lib64/libc.so.6+0x34694)
 #<!-- -->8 0x0000ffff8e2866ec (/lib64/libc.so.6+0x346ec)
 #<!-- -->9 0x00000000063a2c28 Fortran::lower::CalleeInterface::addEntryBlockAndMapArguments() (/work/groups/ssoft/compiler/llvm/aarch64/main-20260112-2f7e218017db/bin/flang+0x63a2c28)
#<!-- -->10 0x00000000063a2dc8 Fortran::lower::CalleeInterface::addEntryBlockAndMapArguments() (/work/groups/ssoft/compiler/llvm/aarch64/main-20260112-2f7e218017db/bin/flang+0x63a2dc8)
#<!-- -->11 0x000000000637dd94 (anonymous namespace)::FirConverter::startNewFunction(Fortran::lower::pft::FunctionLikeUnit&amp;) Bridge.cpp:0:0
#<!-- -->12 0x000000000637fe5c (anonymous namespace)::FirConverter::lowerFunc(Fortran::lower::pft::FunctionLikeUnit&amp;) Bridge.cpp:0:0
#<!-- -->13 0x0000000006381e54 Fortran::lower::LoweringBridge::lower(Fortran::parser::Program const&amp;, Fortran::semantics::SemanticsContext const&amp;) (/work/groups/ssoft/compiler/llvm/aarch64/main-20260112-2f7e218017db/bin/flang+0x6381e54)
#<!-- -->14 0x00000000062357f0 Fortran::frontend::CodeGenAction::beginSourceFileAction() (/work/groups/ssoft/compiler/llvm/aarch64/main-20260112-2f7e218017db/bin/flang+0x62357f0)
#<!-- -->15 0x0000000006222fcc Fortran::frontend::FrontendAction::beginSourceFile(Fortran::frontend::CompilerInstance&amp;, Fortran::frontend::FrontendInputFile const&amp;) (/work/groups/ssoft/compiler/llvm/aarch64/main-20260112-2f7e218017db/bin/flang+0x6222fcc)
#<!-- -->16 0x000000000620f764 Fortran::frontend::CompilerInstance::executeAction(Fortran::frontend::FrontendAction&amp;) (/work/groups/ssoft/compiler/llvm/aarch64/main-20260112-2f7e218017db/bin/flang+0x620f764)
#<!-- -->17 0x000000000622951c Fortran::frontend::executeCompilerInvocation(Fortran::frontend::CompilerInstance*) (/work/groups/ssoft/compiler/llvm/aarch64/main-20260112-2f7e218017db/bin/flang+0x622951c)
#<!-- -->18 0x0000000006207f18 fc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*) (/work/groups/ssoft/compiler/llvm/aarch64/main-20260112-2f7e218017db/bin/flang+0x6207f18)
#<!-- -->19 0x0000000006207910 main (/work/groups/ssoft/compiler/llvm/aarch64/main-20260112-2f7e218017db/bin/flang+0x6207910)
#<!-- -->20 0x0000ffff8e279540 __libc_start_call_main (/lib64/libc.so.6+0x27540)
#<!-- -->21 0x0000ffff8e279618 __libc_start_main@<!-- -->GLIBC_2.17 (/lib64/libc.so.6+0x27618)
#<!-- -->22 0x0000000006205d94 _start (/work/groups/ssoft/compiler/llvm/aarch64/main-20260112-2f7e218017db/bin/flang+0x6205d94)
flang-22: error: unable to execute command: Aborted (core dumped)
flang-22: error: flang frontend command failed due to signal (use -v to see invocation)
flang version 22.0.0git (https://github.com/llvm/llvm-project.git 2f7e218017db69454ea84e1849fcb9a11b1f7fef)
Target: aarch64-unknown-linux-gnu
Thread model: posix
InstalledDir: /work/groups/ssoft/compiler/llvm/aarch64/main-20260112-2f7e218017db/bin
Build config: +assertions
flang-22: note: diagnostic msg:
```

</details>


---

### Comment 2 - ShashwathiNavada

The crash happens when the procedure pointer references an external subprogram. Under constraint C103, a subprogram with the external attribute should be valid. I am currently investigating the issue.

---

