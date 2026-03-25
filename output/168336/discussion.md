# [flang][OpenMP] Custom mappers in different source files crashes the flang parser

**Author:** jfuchs-kmt
**URL:** https://github.com/llvm/llvm-project/issues/168336
**Status:** Closed
**Labels:** flang:ir, crash, flang:openmp
**Closed Date:** 2025-11-17T17:18:14Z

## Body

After the issue https://github.com/llvm/llvm-project/issues/163385 has been fixed and implemented in the PR https://github.com/llvm/llvm-project/pull/167903, a new bug has appeared that crashes the Parser when defining custom mappers in different source files under certain `USE`es of modules. I am not sure how to narrow down the issue any further as I have no experience with the internals of flang.

I am using `flang version 22.0.0git (git@github.com:llvm/llvm-project.git 75af8e86749c820f0920c207ecbcdf6bb04a0ebd)`.
The issue does not appear before https://github.com/llvm/llvm-project/pull/167903 was merged.

The following examples provokes the parser crash:
Compile with: `flang -O2 -fopenmp -fopenmp-targets=nvptx64-nvidia-cuda -fopenmp-version=52 a_mod.F90 b_mod.F90 c_mod.F90 main.F90` (I am using NVIDIA here)

a_mod.F90:
```fortran
MODULE a_mod 
    IMPLICIT NONE

    TYPE :: atyp 
        INTEGER :: foo 
    END TYPE atyp
    !$omp declare mapper(atyp :: t) map(t%foo)
END MODULE a_mod
```
b_mod.F90:
```fortran
MODULE b_mod 
    USE a_mod ! This import is mandatory in the bigger code we are testing, when this is removed the parser does NOT crash
    IMPLICIT NONE

    TYPE :: btyp 
        REAL, ALLOCATABLE :: arr(:)
    END TYPE btyp 
END MODULE b_mod
```
c_mod.F90:
```fortran
MODULE c_mod 
    USE b_mod 
    IMPLICIT NONE

    ! Mapper for btyp shall be defined in c_mod
    !$omp declare mapper(btyp :: t) map(t%arr)
END MODULE c_mod
```
main.F90:
```fortran
PROGRAM main
    IMPLICIT NONE
    ! Does not do anything 
END PROGRAM main
```

will result in the stack trace:
```
fatal internal error: CHECK(!parseState.anyErrorRecovery() || parseState.messages().AnyFatalError()) failed at /home/jfuchs/repos/llvm-project/flang/lib/Parser/parsing.cpp(290)
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/llvm/llvm-project/last_2025-11-14/bin/flang -fc1 -triple x86_64-unknown-linux-gnu -emit-llvm-bc -fcolor-diagnostics -mrelocation-model pic -pic-level 2 -pic-is-pie -target-cpu x86-64 -vectorize-loops -vectorize-slp -fopenmp -fopenmp-version=52 -resource-dir /opt/llvm/llvm-project/last_2025-11-14/lib/clang/22 --offload-targets=nvptx64-nvidia-cuda -mframe-pointer=none -O2 -o /tmp/c_mod-c5b504.bc -x f95 c_mod.F90
 #0 0x000062584e36baa8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/llvm/llvm-project/last_2025-11-14/bin/flang+0x1b0daa8)
 #1 0x000062584e3693d5 llvm::sys::RunSignalHandlers() (/opt/llvm/llvm-project/last_2025-11-14/bin/flang+0x1b0b3d5)
 #2 0x000062584e36c871 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x00007aca7b845330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x00007aca7b89eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x00007aca7b89eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x00007aca7b89eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x00007aca7b84527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x00007aca7b8288ff abort ./stdlib/abort.c:81:7
 #9 0x000062585039da16 (/opt/llvm/llvm-project/last_2025-11-14/bin/flang+0x3b3fa16)
#10 0x0000625850058ecc (/opt/llvm/llvm-project/last_2025-11-14/bin/flang+0x37faecc)
#11 0x000062584f45c618 Fortran::semantics::ModFileReader::Read(Fortran::parser::CharBlock, std::optional<bool>, Fortran::semantics::Scope*, bool) (/opt/llvm/llvm-project/last_2025-11-14/bin/flang+0x2bfe618)
#12 0x000062584ecba01e Fortran::semantics::ModuleVisitor::FindModule(Fortran::parser::Name const&, std::optional<bool>, Fortran::semantics::Scope*) (/opt/llvm/llvm-project/last_2025-11-14/bin/flang+0x245c01e)
#13 0x000062584ecb9f48 Fortran::semantics::ModuleVisitor::Pre(Fortran::parser::UseStmt const&) (/opt/llvm/llvm-project/last_2025-11-14/bin/flang+0x245bf48)
#14 0x000062584ed208b9 void Fortran::parser::detail::ParseTreeVisitorLookupScope::Walk<Fortran::parser::UseStmt, Fortran::semantics::ResolveNamesVisitor>(Fortran::common::Indirection<Fortran::parser::UseStmt, false> const&, Fortran::semantics::ResolveNamesVisitor&) (/opt/llvm/llvm-project/last_2025-11-14/bin/flang+0x24c28b9)
#15 0x000062584ecdef46 Fortran::semantics::ResolveNamesVisitor::Pre(Fortran::parser::SpecificationPart const&) (/opt/llvm/llvm-project/last_2025-11-14/bin/flang+0x2480f46)
#16 0x000062584ece2665 Fortran::semantics::ResolveNamesVisitor::ResolveSpecificationParts(Fortran::semantics::ProgramTree&) (/opt/llvm/llvm-project/last_2025-11-14/bin/flang+0x2484665)
#17 0x000062584ece2500 Fortran::semantics::ResolveNamesVisitor::Pre(Fortran::parser::ProgramUnit const&) (/opt/llvm/llvm-project/last_2025-11-14/bin/flang+0x2484500)
#18 0x000062584ed33c38 std::enable_if<WrapperTrait<Fortran::parser::Program>, void>::type Fortran::parser::detail::ParseTreeVisitorLookupScope::Walk<Fortran::parser::Program, Fortran::semantics::ResolveNamesVisitor>(Fortran::parser::Program const&, Fortran::semantics::ResolveNamesVisitor&) (/opt/llvm/llvm-project/last_2025-11-14/bin/flang+0x24d5c38)
#19 0x000062584ece47ff Fortran::semantics::ResolveNames(Fortran::semantics::SemanticsContext&, Fortran::parser::Program const&, Fortran::semantics::Scope&) (/opt/llvm/llvm-project/last_2025-11-14/bin/flang+0x24867ff)
#20 0x000062584ed61275 Fortran::semantics::Semantics::Perform() (/opt/llvm/llvm-project/last_2025-11-14/bin/flang+0x2503275)
#21 0x000062584e3a5948 Fortran::frontend::FrontendAction::runSemanticChecks() (/opt/llvm/llvm-project/last_2025-11-14/bin/flang+0x1b47948)
#22 0x000062584e5acd2f Fortran::frontend::CodeGenAction::beginSourceFileAction() (/opt/llvm/llvm-project/last_2025-11-14/bin/flang+0x1d4ed2f)
#23 0x000062584e3a4e88 Fortran::frontend::FrontendAction::beginSourceFile(Fortran::frontend::CompilerInstance&, Fortran::frontend::FrontendInputFile const&) (/opt/llvm/llvm-project/last_2025-11-14/bin/flang+0x1b46e88)
#24 0x000062584e38dcf3 Fortran::frontend::CompilerInstance::executeAction(Fortran::frontend::FrontendAction&) (/opt/llvm/llvm-project/last_2025-11-14/bin/flang+0x1b2fcf3)
#25 0x000062584e3a9546 Fortran::frontend::executeCompilerInvocation(Fortran::frontend::CompilerInstance*) (/opt/llvm/llvm-project/last_2025-11-14/bin/flang+0x1b4b546)
#26 0x000062584de7bb9f fc1_main(llvm::ArrayRef<char const*>, char const*) (/opt/llvm/llvm-project/last_2025-11-14/bin/flang+0x161db9f)
#27 0x000062584de7ae6b main (/opt/llvm/llvm-project/last_2025-11-14/bin/flang+0x161ce6b)
#28 0x00007aca7b82a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#29 0x00007aca7b82a28b call_init ./csu/../csu/libc-start.c:128:20
#30 0x00007aca7b82a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#31 0x000062584de79f85 _start (/opt/llvm/llvm-project/last_2025-11-14/bin/flang+0x161bf85)
flang-22: error: unable to execute command: Aborted (core dumped)
warning: OpenMP support for version 52 in flang is still incomplete
warning: OpenMP support for version 52 in flang is still incomplete
warning: OpenMP support for version 52 in flang is still incomplete
flang-22: error: flang frontend command failed due to signal (use -v to see invocation)
flang version 22.0.0git (git@github.com:llvm/llvm-project.git 75af8e86749c820f0920c207ecbcdf6bb04a0ebd)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /opt/llvm/llvm-project/last_2025-11-14/bin
flang-22: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
flang-22: note: diagnostic msg: /tmp/a_mod-sm_120-fb9ba0
flang-22: note: diagnostic msg: /tmp/a_mod-96d2dd
flang-22: note: diagnostic msg: /tmp/b_mod-sm_120-b184b1
flang-22: note: diagnostic msg: /tmp/b_mod-aa9f31
flang-22: note: diagnostic msg: /tmp/c_mod-sm_120-8b8aea
flang-22: note: diagnostic msg: /tmp/c_mod-3b32ac
flang-22: note: diagnostic msg: /tmp/main-sm_120-fb5e67
flang-22: note: diagnostic msg: /tmp/main-4b0751
flang-22: note: diagnostic msg: /tmp/a_mod-sm_120-fb9ba0.sh
flang-22: note: diagnostic msg: 

********************
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-flang-frontend

Author: None (jfuchs-kmt)

<details>
After the issue https://github.com/llvm/llvm-project/issues/163385 has been fixed and implemented in the PR https://github.com/llvm/llvm-project/pull/167903, a new bug has appeared that crashes the Parser when defining custom mappers in different source files under certain `USE`es of modules. I am not sure how to narrow down the issue any further as I have no experience with the internals of flang.

I am using `flang version 22.0.0git (git@<!-- -->github.com:llvm/llvm-project.git 75af8e86749c820f0920c207ecbcdf6bb04a0ebd)`.
The issue does not appear before https://github.com/llvm/llvm-project/pull/167903 was merged.

The following examples provokes the parser crash:
Compile with: `flang -O2 -fopenmp -fopenmp-targets=nvptx64-nvidia-cuda -fopenmp-version=52 a_mod.F90 b_mod.F90 c_mod.F90 main.F90` (I am using NVIDIA here)

a_mod.F90:
```fortran
MODULE a_mod 
    IMPLICIT NONE

    TYPE :: atyp 
        INTEGER :: foo 
    END TYPE atyp
    !$omp declare mapper(atyp :: t) map(t%foo)
END MODULE a_mod
```
b_mod.F90:
```fortran
MODULE b_mod 
    USE a_mod ! This import is mandatory in the bigger code we are testing, when this is removed the parser does NOT crash
    IMPLICIT NONE

    TYPE :: btyp 
        REAL, ALLOCATABLE :: arr(:)
    END TYPE btyp 
END MODULE b_mod
```
c_mod.F90:
```fortran
MODULE c_mod 
    USE b_mod 
    IMPLICIT NONE

    ! Mapper for btyp shall be defined in c_mod
    !$omp declare mapper(btyp :: t) map(t%arr)
END MODULE c_mod
```
main.F90:
```fortran
PROGRAM main
    IMPLICIT NONE
    ! Does not do anything 
END PROGRAM main
```

will result in the stack trace:
```
fatal internal error: CHECK(!parseState.anyErrorRecovery() || parseState.messages().AnyFatalError()) failed at /home/jfuchs/repos/llvm-project/flang/lib/Parser/parsing.cpp(290)
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/llvm/llvm-project/last_2025-11-14/bin/flang -fc1 -triple x86_64-unknown-linux-gnu -emit-llvm-bc -fcolor-diagnostics -mrelocation-model pic -pic-level 2 -pic-is-pie -target-cpu x86-64 -vectorize-loops -vectorize-slp -fopenmp -fopenmp-version=52 -resource-dir /opt/llvm/llvm-project/last_2025-11-14/lib/clang/22 --offload-targets=nvptx64-nvidia-cuda -mframe-pointer=none -O2 -o /tmp/c_mod-c5b504.bc -x f95 c_mod.F90
 #<!-- -->0 0x000062584e36baa8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/llvm/llvm-project/last_2025-11-14/bin/flang+0x1b0daa8)
 #<!-- -->1 0x000062584e3693d5 llvm::sys::RunSignalHandlers() (/opt/llvm/llvm-project/last_2025-11-14/bin/flang+0x1b0b3d5)
 #<!-- -->2 0x000062584e36c871 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x00007aca7b845330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->4 0x00007aca7b89eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->5 0x00007aca7b89eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->6 0x00007aca7b89eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->7 0x00007aca7b84527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->8 0x00007aca7b8288ff abort ./stdlib/abort.c:81:7
 #<!-- -->9 0x000062585039da16 (/opt/llvm/llvm-project/last_2025-11-14/bin/flang+0x3b3fa16)
#<!-- -->10 0x0000625850058ecc (/opt/llvm/llvm-project/last_2025-11-14/bin/flang+0x37faecc)
#<!-- -->11 0x000062584f45c618 Fortran::semantics::ModFileReader::Read(Fortran::parser::CharBlock, std::optional&lt;bool&gt;, Fortran::semantics::Scope*, bool) (/opt/llvm/llvm-project/last_2025-11-14/bin/flang+0x2bfe618)
#<!-- -->12 0x000062584ecba01e Fortran::semantics::ModuleVisitor::FindModule(Fortran::parser::Name const&amp;, std::optional&lt;bool&gt;, Fortran::semantics::Scope*) (/opt/llvm/llvm-project/last_2025-11-14/bin/flang+0x245c01e)
#<!-- -->13 0x000062584ecb9f48 Fortran::semantics::ModuleVisitor::Pre(Fortran::parser::UseStmt const&amp;) (/opt/llvm/llvm-project/last_2025-11-14/bin/flang+0x245bf48)
#<!-- -->14 0x000062584ed208b9 void Fortran::parser::detail::ParseTreeVisitorLookupScope::Walk&lt;Fortran::parser::UseStmt, Fortran::semantics::ResolveNamesVisitor&gt;(Fortran::common::Indirection&lt;Fortran::parser::UseStmt, false&gt; const&amp;, Fortran::semantics::ResolveNamesVisitor&amp;) (/opt/llvm/llvm-project/last_2025-11-14/bin/flang+0x24c28b9)
#<!-- -->15 0x000062584ecdef46 Fortran::semantics::ResolveNamesVisitor::Pre(Fortran::parser::SpecificationPart const&amp;) (/opt/llvm/llvm-project/last_2025-11-14/bin/flang+0x2480f46)
#<!-- -->16 0x000062584ece2665 Fortran::semantics::ResolveNamesVisitor::ResolveSpecificationParts(Fortran::semantics::ProgramTree&amp;) (/opt/llvm/llvm-project/last_2025-11-14/bin/flang+0x2484665)
#<!-- -->17 0x000062584ece2500 Fortran::semantics::ResolveNamesVisitor::Pre(Fortran::parser::ProgramUnit const&amp;) (/opt/llvm/llvm-project/last_2025-11-14/bin/flang+0x2484500)
#<!-- -->18 0x000062584ed33c38 std::enable_if&lt;WrapperTrait&lt;Fortran::parser::Program&gt;, void&gt;::type Fortran::parser::detail::ParseTreeVisitorLookupScope::Walk&lt;Fortran::parser::Program, Fortran::semantics::ResolveNamesVisitor&gt;(Fortran::parser::Program const&amp;, Fortran::semantics::ResolveNamesVisitor&amp;) (/opt/llvm/llvm-project/last_2025-11-14/bin/flang+0x24d5c38)
#<!-- -->19 0x000062584ece47ff Fortran::semantics::ResolveNames(Fortran::semantics::SemanticsContext&amp;, Fortran::parser::Program const&amp;, Fortran::semantics::Scope&amp;) (/opt/llvm/llvm-project/last_2025-11-14/bin/flang+0x24867ff)
#<!-- -->20 0x000062584ed61275 Fortran::semantics::Semantics::Perform() (/opt/llvm/llvm-project/last_2025-11-14/bin/flang+0x2503275)
#<!-- -->21 0x000062584e3a5948 Fortran::frontend::FrontendAction::runSemanticChecks() (/opt/llvm/llvm-project/last_2025-11-14/bin/flang+0x1b47948)
#<!-- -->22 0x000062584e5acd2f Fortran::frontend::CodeGenAction::beginSourceFileAction() (/opt/llvm/llvm-project/last_2025-11-14/bin/flang+0x1d4ed2f)
#<!-- -->23 0x000062584e3a4e88 Fortran::frontend::FrontendAction::beginSourceFile(Fortran::frontend::CompilerInstance&amp;, Fortran::frontend::FrontendInputFile const&amp;) (/opt/llvm/llvm-project/last_2025-11-14/bin/flang+0x1b46e88)
#<!-- -->24 0x000062584e38dcf3 Fortran::frontend::CompilerInstance::executeAction(Fortran::frontend::FrontendAction&amp;) (/opt/llvm/llvm-project/last_2025-11-14/bin/flang+0x1b2fcf3)
#<!-- -->25 0x000062584e3a9546 Fortran::frontend::executeCompilerInvocation(Fortran::frontend::CompilerInstance*) (/opt/llvm/llvm-project/last_2025-11-14/bin/flang+0x1b4b546)
#<!-- -->26 0x000062584de7bb9f fc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*) (/opt/llvm/llvm-project/last_2025-11-14/bin/flang+0x161db9f)
#<!-- -->27 0x000062584de7ae6b main (/opt/llvm/llvm-project/last_2025-11-14/bin/flang+0x161ce6b)
#<!-- -->28 0x00007aca7b82a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->29 0x00007aca7b82a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->30 0x00007aca7b82a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->31 0x000062584de79f85 _start (/opt/llvm/llvm-project/last_2025-11-14/bin/flang+0x161bf85)
flang-22: error: unable to execute command: Aborted (core dumped)
warning: OpenMP support for version 52 in flang is still incomplete
warning: OpenMP support for version 52 in flang is still incomplete
warning: OpenMP support for version 52 in flang is still incomplete
flang-22: error: flang frontend command failed due to signal (use -v to see invocation)
flang version 22.0.0git (git@<!-- -->github.com:llvm/llvm-project.git 75af8e86749c820f0920c207ecbcdf6bb04a0ebd)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /opt/llvm/llvm-project/last_2025-11-14/bin
flang-22: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
flang-22: note: diagnostic msg: /tmp/a_mod-sm_120-fb9ba0
flang-22: note: diagnostic msg: /tmp/a_mod-96d2dd
flang-22: note: diagnostic msg: /tmp/b_mod-sm_120-b184b1
flang-22: note: diagnostic msg: /tmp/b_mod-aa9f31
flang-22: note: diagnostic msg: /tmp/c_mod-sm_120-8b8aea
flang-22: note: diagnostic msg: /tmp/c_mod-3b32ac
flang-22: note: diagnostic msg: /tmp/main-sm_120-fb5e67
flang-22: note: diagnostic msg: /tmp/main-4b0751
flang-22: note: diagnostic msg: /tmp/a_mod-sm_120-fb9ba0.sh
flang-22: note: diagnostic msg: 

********************
```
</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-flang-ir

Author: None (jfuchs-kmt)

<details>
After the issue https://github.com/llvm/llvm-project/issues/163385 has been fixed and implemented in the PR https://github.com/llvm/llvm-project/pull/167903, a new bug has appeared that crashes the Parser when defining custom mappers in different source files under certain `USE`es of modules. I am not sure how to narrow down the issue any further as I have no experience with the internals of flang.

I am using `flang version 22.0.0git (git@<!-- -->github.com:llvm/llvm-project.git 75af8e86749c820f0920c207ecbcdf6bb04a0ebd)`.
The issue does not appear before https://github.com/llvm/llvm-project/pull/167903 was merged.

The following examples provokes the parser crash:
Compile with: `flang -O2 -fopenmp -fopenmp-targets=nvptx64-nvidia-cuda -fopenmp-version=52 a_mod.F90 b_mod.F90 c_mod.F90 main.F90` (I am using NVIDIA here)

a_mod.F90:
```fortran
MODULE a_mod 
    IMPLICIT NONE

    TYPE :: atyp 
        INTEGER :: foo 
    END TYPE atyp
    !$omp declare mapper(atyp :: t) map(t%foo)
END MODULE a_mod
```
b_mod.F90:
```fortran
MODULE b_mod 
    USE a_mod ! This import is mandatory in the bigger code we are testing, when this is removed the parser does NOT crash
    IMPLICIT NONE

    TYPE :: btyp 
        REAL, ALLOCATABLE :: arr(:)
    END TYPE btyp 
END MODULE b_mod
```
c_mod.F90:
```fortran
MODULE c_mod 
    USE b_mod 
    IMPLICIT NONE

    ! Mapper for btyp shall be defined in c_mod
    !$omp declare mapper(btyp :: t) map(t%arr)
END MODULE c_mod
```
main.F90:
```fortran
PROGRAM main
    IMPLICIT NONE
    ! Does not do anything 
END PROGRAM main
```

will result in the stack trace:
```
fatal internal error: CHECK(!parseState.anyErrorRecovery() || parseState.messages().AnyFatalError()) failed at /home/jfuchs/repos/llvm-project/flang/lib/Parser/parsing.cpp(290)
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/llvm/llvm-project/last_2025-11-14/bin/flang -fc1 -triple x86_64-unknown-linux-gnu -emit-llvm-bc -fcolor-diagnostics -mrelocation-model pic -pic-level 2 -pic-is-pie -target-cpu x86-64 -vectorize-loops -vectorize-slp -fopenmp -fopenmp-version=52 -resource-dir /opt/llvm/llvm-project/last_2025-11-14/lib/clang/22 --offload-targets=nvptx64-nvidia-cuda -mframe-pointer=none -O2 -o /tmp/c_mod-c5b504.bc -x f95 c_mod.F90
 #<!-- -->0 0x000062584e36baa8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/llvm/llvm-project/last_2025-11-14/bin/flang+0x1b0daa8)
 #<!-- -->1 0x000062584e3693d5 llvm::sys::RunSignalHandlers() (/opt/llvm/llvm-project/last_2025-11-14/bin/flang+0x1b0b3d5)
 #<!-- -->2 0x000062584e36c871 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x00007aca7b845330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->4 0x00007aca7b89eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->5 0x00007aca7b89eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->6 0x00007aca7b89eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->7 0x00007aca7b84527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->8 0x00007aca7b8288ff abort ./stdlib/abort.c:81:7
 #<!-- -->9 0x000062585039da16 (/opt/llvm/llvm-project/last_2025-11-14/bin/flang+0x3b3fa16)
#<!-- -->10 0x0000625850058ecc (/opt/llvm/llvm-project/last_2025-11-14/bin/flang+0x37faecc)
#<!-- -->11 0x000062584f45c618 Fortran::semantics::ModFileReader::Read(Fortran::parser::CharBlock, std::optional&lt;bool&gt;, Fortran::semantics::Scope*, bool) (/opt/llvm/llvm-project/last_2025-11-14/bin/flang+0x2bfe618)
#<!-- -->12 0x000062584ecba01e Fortran::semantics::ModuleVisitor::FindModule(Fortran::parser::Name const&amp;, std::optional&lt;bool&gt;, Fortran::semantics::Scope*) (/opt/llvm/llvm-project/last_2025-11-14/bin/flang+0x245c01e)
#<!-- -->13 0x000062584ecb9f48 Fortran::semantics::ModuleVisitor::Pre(Fortran::parser::UseStmt const&amp;) (/opt/llvm/llvm-project/last_2025-11-14/bin/flang+0x245bf48)
#<!-- -->14 0x000062584ed208b9 void Fortran::parser::detail::ParseTreeVisitorLookupScope::Walk&lt;Fortran::parser::UseStmt, Fortran::semantics::ResolveNamesVisitor&gt;(Fortran::common::Indirection&lt;Fortran::parser::UseStmt, false&gt; const&amp;, Fortran::semantics::ResolveNamesVisitor&amp;) (/opt/llvm/llvm-project/last_2025-11-14/bin/flang+0x24c28b9)
#<!-- -->15 0x000062584ecdef46 Fortran::semantics::ResolveNamesVisitor::Pre(Fortran::parser::SpecificationPart const&amp;) (/opt/llvm/llvm-project/last_2025-11-14/bin/flang+0x2480f46)
#<!-- -->16 0x000062584ece2665 Fortran::semantics::ResolveNamesVisitor::ResolveSpecificationParts(Fortran::semantics::ProgramTree&amp;) (/opt/llvm/llvm-project/last_2025-11-14/bin/flang+0x2484665)
#<!-- -->17 0x000062584ece2500 Fortran::semantics::ResolveNamesVisitor::Pre(Fortran::parser::ProgramUnit const&amp;) (/opt/llvm/llvm-project/last_2025-11-14/bin/flang+0x2484500)
#<!-- -->18 0x000062584ed33c38 std::enable_if&lt;WrapperTrait&lt;Fortran::parser::Program&gt;, void&gt;::type Fortran::parser::detail::ParseTreeVisitorLookupScope::Walk&lt;Fortran::parser::Program, Fortran::semantics::ResolveNamesVisitor&gt;(Fortran::parser::Program const&amp;, Fortran::semantics::ResolveNamesVisitor&amp;) (/opt/llvm/llvm-project/last_2025-11-14/bin/flang+0x24d5c38)
#<!-- -->19 0x000062584ece47ff Fortran::semantics::ResolveNames(Fortran::semantics::SemanticsContext&amp;, Fortran::parser::Program const&amp;, Fortran::semantics::Scope&amp;) (/opt/llvm/llvm-project/last_2025-11-14/bin/flang+0x24867ff)
#<!-- -->20 0x000062584ed61275 Fortran::semantics::Semantics::Perform() (/opt/llvm/llvm-project/last_2025-11-14/bin/flang+0x2503275)
#<!-- -->21 0x000062584e3a5948 Fortran::frontend::FrontendAction::runSemanticChecks() (/opt/llvm/llvm-project/last_2025-11-14/bin/flang+0x1b47948)
#<!-- -->22 0x000062584e5acd2f Fortran::frontend::CodeGenAction::beginSourceFileAction() (/opt/llvm/llvm-project/last_2025-11-14/bin/flang+0x1d4ed2f)
#<!-- -->23 0x000062584e3a4e88 Fortran::frontend::FrontendAction::beginSourceFile(Fortran::frontend::CompilerInstance&amp;, Fortran::frontend::FrontendInputFile const&amp;) (/opt/llvm/llvm-project/last_2025-11-14/bin/flang+0x1b46e88)
#<!-- -->24 0x000062584e38dcf3 Fortran::frontend::CompilerInstance::executeAction(Fortran::frontend::FrontendAction&amp;) (/opt/llvm/llvm-project/last_2025-11-14/bin/flang+0x1b2fcf3)
#<!-- -->25 0x000062584e3a9546 Fortran::frontend::executeCompilerInvocation(Fortran::frontend::CompilerInstance*) (/opt/llvm/llvm-project/last_2025-11-14/bin/flang+0x1b4b546)
#<!-- -->26 0x000062584de7bb9f fc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*) (/opt/llvm/llvm-project/last_2025-11-14/bin/flang+0x161db9f)
#<!-- -->27 0x000062584de7ae6b main (/opt/llvm/llvm-project/last_2025-11-14/bin/flang+0x161ce6b)
#<!-- -->28 0x00007aca7b82a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->29 0x00007aca7b82a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->30 0x00007aca7b82a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->31 0x000062584de79f85 _start (/opt/llvm/llvm-project/last_2025-11-14/bin/flang+0x161bf85)
flang-22: error: unable to execute command: Aborted (core dumped)
warning: OpenMP support for version 52 in flang is still incomplete
warning: OpenMP support for version 52 in flang is still incomplete
warning: OpenMP support for version 52 in flang is still incomplete
flang-22: error: flang frontend command failed due to signal (use -v to see invocation)
flang version 22.0.0git (git@<!-- -->github.com:llvm/llvm-project.git 75af8e86749c820f0920c207ecbcdf6bb04a0ebd)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /opt/llvm/llvm-project/last_2025-11-14/bin
flang-22: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
flang-22: note: diagnostic msg: /tmp/a_mod-sm_120-fb9ba0
flang-22: note: diagnostic msg: /tmp/a_mod-96d2dd
flang-22: note: diagnostic msg: /tmp/b_mod-sm_120-b184b1
flang-22: note: diagnostic msg: /tmp/b_mod-aa9f31
flang-22: note: diagnostic msg: /tmp/c_mod-sm_120-8b8aea
flang-22: note: diagnostic msg: /tmp/c_mod-3b32ac
flang-22: note: diagnostic msg: /tmp/main-sm_120-fb5e67
flang-22: note: diagnostic msg: /tmp/main-4b0751
flang-22: note: diagnostic msg: /tmp/a_mod-sm_120-fb9ba0.sh
flang-22: note: diagnostic msg: 

********************
```
</details>


---

### Comment 3 - jfuchs-kmt

@TIFitis Awesome, thanks for the quick fix!

---

### Comment 4 - TIFitis

@jfuchs-kmt Thank you, and, thanks to you for taking the time to report these 👍🏽

---

