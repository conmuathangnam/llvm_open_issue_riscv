# [flang] fatal internal error: CHECK(localSymbol->has<UseDetails>())

**Author:** mathomp4
**URL:** https://github.com/llvm/llvm-project/issues/168099
**Status:** Closed
**Labels:** flang:frontend, crash
**Closed Date:** 2025-11-19T16:54:23Z

## Body

This is sort of a follow-on to #116388. Once that was fixed in `main` (see https://github.com/llvm/llvm-project/pull/162205 by @valerydmit) we encountered another failure. This one is a single file failure as described below.

This code passes with nagfor, gfortran-15, ifx and lfortran (godbolt link: https://godbolt.org/z/551deeqd9)


```
> flang pair.F90

fatal internal error: CHECK(localSymbol->has<UseDetails>()) failed at /discover/nobackup/projects/gmao/SIteam/build-llvm-flang/tmp/llvm-src/llvm-project-main/flang/lib/Semantics/resolve-names.cpp(3942)
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /gpfsm/dnb05/projects/p50/comp/SLES-15/llvm-flang/2025-10-14/bin/flang -fc1 -triple x86_64-unknown-linux-gnu -emit-obj -fcolor-diagnostics -mrelocation-model pic -pic-level 2 -pic-is-pie -target-cpu x86-64 -resource-dir /gpfsm/dnb05/projects/p50/comp/SLES-15/llvm-flang/2025-10-14/lib/clang/22 -mframe-pointer=all -o /gpfsm/dnb34/tdirs/login/discover36.93889.mathomp4/pair-be2f17.o -x f95 pair.F90
 #0 0x0000000001bb608b llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/gpfsm/dnb05/projects/p50/comp/SLES-15/llvm-flang/2025-10-14/bin/flang+0x1bb608b)
 #1 0x0000000001bb2f4f SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x000015420a7ea910 __restore_rt (/lib64/libpthread.so.0+0x16910)
 #3 0x000015420a0d0d2b raise (/lib64/libc.so.6+0x4ad2b)
 #4 0x000015420a0d23e5 abort (/lib64/libc.so.6+0x4c3e5)
 #5 0x0000000003bd27a9 (/gpfsm/dnb05/projects/p50/comp/SLES-15/llvm-flang/2025-10-14/bin/flang+0x3bd27a9)
 #6 0x0000000002569abf Fortran::semantics::ModuleVisitor::DoAddUse(Fortran::parser::CharBlock, Fortran::parser::CharBlock, Fortran::semantics::Symbol&, Fortran::semantics::Symbol const&) (/gpfsm/dnb05/projects/p50/comp/SLES-15/llvm-flang/2025-10-14/bin/flang+0x2569abf)
 #7 0x000000000256aa63 Fortran::semantics::ModuleVisitor::Post(Fortran::parser::UseStmt const&) (/gpfsm/dnb05/projects/p50/comp/SLES-15/llvm-flang/2025-10-14/bin/flang+0x256aa63)
 #8 0x00000000025b4002 Fortran::semantics::ResolveNamesVisitor::Pre(Fortran::parser::SpecificationPart const&) (/gpfsm/dnb05/projects/p50/comp/SLES-15/llvm-flang/2025-10-14/bin/flang+0x25b4002)
 #9 0x00000000025b4ba2 std::enable_if<TupleTrait<Fortran::parser::SpecificationPart>, void>::type Fortran::parser::detail::ParseTreeVisitorLookupScope::Walk<Fortran::parser::SpecificationPart, Fortran::semantics::ResolveNamesVisitor>(Fortran::parser::SpecificationPart const&, Fortran::semantics::ResolveNamesVisitor&) (/gpfsm/dnb05/projects/p50/comp/SLES-15/llvm-flang/2025-10-14/bin/flang+0x25b4ba2)
#10 0x00000000025da78d Fortran::semantics::ResolveNamesVisitor::ResolveSpecificationParts(Fortran::semantics::ProgramTree&) (.part.0) resolve-names.cpp:0:0
#11 0x00000000025db903 Fortran::semantics::ResolveNamesVisitor::Pre(Fortran::parser::ProgramUnit const&) (/gpfsm/dnb05/projects/p50/comp/SLES-15/llvm-flang/2025-10-14/bin/flang+0x25db903)
#12 0x00000000025dd094 Fortran::semantics::ResolveNames(Fortran::semantics::SemanticsContext&, Fortran::parser::Program const&, Fortran::semantics::Scope&) (/gpfsm/dnb05/projects/p50/comp/SLES-15/llvm-flang/2025-10-14/bin/flang+0x25dd094)
#13 0x000000000266525e Fortran::semantics::Semantics::Perform() (/gpfsm/dnb05/projects/p50/comp/SLES-15/llvm-flang/2025-10-14/bin/flang+0x266525e)
#14 0x0000000001bf969e Fortran::frontend::FrontendAction::runSemanticChecks() (/gpfsm/dnb05/projects/p50/comp/SLES-15/llvm-flang/2025-10-14/bin/flang+0x1bf969e)
#15 0x0000000001eb7fca Fortran::frontend::CodeGenAction::beginSourceFileAction() (/gpfsm/dnb05/projects/p50/comp/SLES-15/llvm-flang/2025-10-14/bin/flang+0x1eb7fca)
#16 0x0000000001bf7a6d Fortran::frontend::FrontendAction::beginSourceFile(Fortran::frontend::CompilerInstance&, Fortran::frontend::FrontendInputFile const&) (/gpfsm/dnb05/projects/p50/comp/SLES-15/llvm-flang/2025-10-14/bin/flang+0x1bf7a6d)
#17 0x0000000001be4ebc Fortran::frontend::CompilerInstance::executeAction(Fortran::frontend::FrontendAction&) (/gpfsm/dnb05/projects/p50/comp/SLES-15/llvm-flang/2025-10-14/bin/flang+0x1be4ebc)
#18 0x0000000001c017bc Fortran::frontend::executeCompilerInvocation(Fortran::frontend::CompilerInstance*) (/gpfsm/dnb05/projects/p50/comp/SLES-15/llvm-flang/2025-10-14/bin/flang+0x1c017bc)
#19 0x000000000161c014 fc1_main(llvm::ArrayRef<char const*>, char const*) (/gpfsm/dnb05/projects/p50/comp/SLES-15/llvm-flang/2025-10-14/bin/flang+0x161c014)
#20 0x0000000001505919 main (/gpfsm/dnb05/projects/p50/comp/SLES-15/llvm-flang/2025-10-14/bin/flang+0x1505919)
#21 0x000015420a0bb24d __libc_start_main (/lib64/libc.so.6+0x3524d)
#22 0x000000000161aaaa _start /home/abuild/rpmbuild/BUILD/glibc-2.31/csu/../sysdeps/x86_64/start.S:122:0
flang-22: error: unable to execute command: Aborted
flang-22: error: flang frontend command failed due to signal (use -v to see invocation)
flang version 22.0.0git (git@github.com:GEOS-ESM/build-llvm-flang.git db43ed9aa5de6bdb57428340514eb95d07fb28ab)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /gpfsm/dnb05/projects/p50/comp/SLES-15/llvm-flang/2025-10-14/bin
flang-22: note: diagnostic msg:
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
flang-22: note: diagnostic msg: /gpfsm/dnb34/tdirs/login/discover36.93889.mathomp4/pair-8726cf
flang-22: note: diagnostic msg: /gpfsm/dnb34/tdirs/login/discover36.93889.mathomp4/pair-8726cf.sh
flang-22: note: diagnostic msg:

********************
```

As directed, I'm attaching the files that can reproduce this. They are in a tarfile because, well, I can't seem to attach F90 files.

There are three files:
```
Flang-Crash-2025Nov14/
├── FromFlang
│   ├── pair-8726cf
│   └── pair-8726cf.sh
└── pair.F90

2 directories, 3 files
```

The `pair.F90` is our original repro, and the files in `FromFlang` are from 

[Flang-Crash-2025Nov14.tar.gz](https://github.com/user-attachments/files/23553478/Flang-Crash-2025Nov14.tar.gz)

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-flang-frontend

Author: Matt Thompson (mathomp4)

<details>
This is sort of a follow-on to #<!-- -->116388. Once that was fixed in `main` (see https://github.com/llvm/llvm-project/pull/162205 by @<!-- -->valerydmit) we encountered another failure. This one is a single file failure as described below.

This code passes with nagfor, gfortran-15, ifx and lfortran (godbolt link: https://godbolt.org/z/551deeqd9)


```
&gt; flang pair.F90

fatal internal error: CHECK(localSymbol-&gt;has&lt;UseDetails&gt;()) failed at /discover/nobackup/projects/gmao/SIteam/build-llvm-flang/tmp/llvm-src/llvm-project-main/flang/lib/Semantics/resolve-names.cpp(3942)
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /gpfsm/dnb05/projects/p50/comp/SLES-15/llvm-flang/2025-10-14/bin/flang -fc1 -triple x86_64-unknown-linux-gnu -emit-obj -fcolor-diagnostics -mrelocation-model pic -pic-level 2 -pic-is-pie -target-cpu x86-64 -resource-dir /gpfsm/dnb05/projects/p50/comp/SLES-15/llvm-flang/2025-10-14/lib/clang/22 -mframe-pointer=all -o /gpfsm/dnb34/tdirs/login/discover36.93889.mathomp4/pair-be2f17.o -x f95 pair.F90
 #<!-- -->0 0x0000000001bb608b llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/gpfsm/dnb05/projects/p50/comp/SLES-15/llvm-flang/2025-10-14/bin/flang+0x1bb608b)
 #<!-- -->1 0x0000000001bb2f4f SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->2 0x000015420a7ea910 __restore_rt (/lib64/libpthread.so.0+0x16910)
 #<!-- -->3 0x000015420a0d0d2b raise (/lib64/libc.so.6+0x4ad2b)
 #<!-- -->4 0x000015420a0d23e5 abort (/lib64/libc.so.6+0x4c3e5)
 #<!-- -->5 0x0000000003bd27a9 (/gpfsm/dnb05/projects/p50/comp/SLES-15/llvm-flang/2025-10-14/bin/flang+0x3bd27a9)
 #<!-- -->6 0x0000000002569abf Fortran::semantics::ModuleVisitor::DoAddUse(Fortran::parser::CharBlock, Fortran::parser::CharBlock, Fortran::semantics::Symbol&amp;, Fortran::semantics::Symbol const&amp;) (/gpfsm/dnb05/projects/p50/comp/SLES-15/llvm-flang/2025-10-14/bin/flang+0x2569abf)
 #<!-- -->7 0x000000000256aa63 Fortran::semantics::ModuleVisitor::Post(Fortran::parser::UseStmt const&amp;) (/gpfsm/dnb05/projects/p50/comp/SLES-15/llvm-flang/2025-10-14/bin/flang+0x256aa63)
 #<!-- -->8 0x00000000025b4002 Fortran::semantics::ResolveNamesVisitor::Pre(Fortran::parser::SpecificationPart const&amp;) (/gpfsm/dnb05/projects/p50/comp/SLES-15/llvm-flang/2025-10-14/bin/flang+0x25b4002)
 #<!-- -->9 0x00000000025b4ba2 std::enable_if&lt;TupleTrait&lt;Fortran::parser::SpecificationPart&gt;, void&gt;::type Fortran::parser::detail::ParseTreeVisitorLookupScope::Walk&lt;Fortran::parser::SpecificationPart, Fortran::semantics::ResolveNamesVisitor&gt;(Fortran::parser::SpecificationPart const&amp;, Fortran::semantics::ResolveNamesVisitor&amp;) (/gpfsm/dnb05/projects/p50/comp/SLES-15/llvm-flang/2025-10-14/bin/flang+0x25b4ba2)
#<!-- -->10 0x00000000025da78d Fortran::semantics::ResolveNamesVisitor::ResolveSpecificationParts(Fortran::semantics::ProgramTree&amp;) (.part.0) resolve-names.cpp:0:0
#<!-- -->11 0x00000000025db903 Fortran::semantics::ResolveNamesVisitor::Pre(Fortran::parser::ProgramUnit const&amp;) (/gpfsm/dnb05/projects/p50/comp/SLES-15/llvm-flang/2025-10-14/bin/flang+0x25db903)
#<!-- -->12 0x00000000025dd094 Fortran::semantics::ResolveNames(Fortran::semantics::SemanticsContext&amp;, Fortran::parser::Program const&amp;, Fortran::semantics::Scope&amp;) (/gpfsm/dnb05/projects/p50/comp/SLES-15/llvm-flang/2025-10-14/bin/flang+0x25dd094)
#<!-- -->13 0x000000000266525e Fortran::semantics::Semantics::Perform() (/gpfsm/dnb05/projects/p50/comp/SLES-15/llvm-flang/2025-10-14/bin/flang+0x266525e)
#<!-- -->14 0x0000000001bf969e Fortran::frontend::FrontendAction::runSemanticChecks() (/gpfsm/dnb05/projects/p50/comp/SLES-15/llvm-flang/2025-10-14/bin/flang+0x1bf969e)
#<!-- -->15 0x0000000001eb7fca Fortran::frontend::CodeGenAction::beginSourceFileAction() (/gpfsm/dnb05/projects/p50/comp/SLES-15/llvm-flang/2025-10-14/bin/flang+0x1eb7fca)
#<!-- -->16 0x0000000001bf7a6d Fortran::frontend::FrontendAction::beginSourceFile(Fortran::frontend::CompilerInstance&amp;, Fortran::frontend::FrontendInputFile const&amp;) (/gpfsm/dnb05/projects/p50/comp/SLES-15/llvm-flang/2025-10-14/bin/flang+0x1bf7a6d)
#<!-- -->17 0x0000000001be4ebc Fortran::frontend::CompilerInstance::executeAction(Fortran::frontend::FrontendAction&amp;) (/gpfsm/dnb05/projects/p50/comp/SLES-15/llvm-flang/2025-10-14/bin/flang+0x1be4ebc)
#<!-- -->18 0x0000000001c017bc Fortran::frontend::executeCompilerInvocation(Fortran::frontend::CompilerInstance*) (/gpfsm/dnb05/projects/p50/comp/SLES-15/llvm-flang/2025-10-14/bin/flang+0x1c017bc)
#<!-- -->19 0x000000000161c014 fc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*) (/gpfsm/dnb05/projects/p50/comp/SLES-15/llvm-flang/2025-10-14/bin/flang+0x161c014)
#<!-- -->20 0x0000000001505919 main (/gpfsm/dnb05/projects/p50/comp/SLES-15/llvm-flang/2025-10-14/bin/flang+0x1505919)
#<!-- -->21 0x000015420a0bb24d __libc_start_main (/lib64/libc.so.6+0x3524d)
#<!-- -->22 0x000000000161aaaa _start /home/abuild/rpmbuild/BUILD/glibc-2.31/csu/../sysdeps/x86_64/start.S:122:0
flang-22: error: unable to execute command: Aborted
flang-22: error: flang frontend command failed due to signal (use -v to see invocation)
flang version 22.0.0git (git@<!-- -->github.com:GEOS-ESM/build-llvm-flang.git db43ed9aa5de6bdb57428340514eb95d07fb28ab)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /gpfsm/dnb05/projects/p50/comp/SLES-15/llvm-flang/2025-10-14/bin
flang-22: note: diagnostic msg:
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
flang-22: note: diagnostic msg: /gpfsm/dnb34/tdirs/login/discover36.93889.mathomp4/pair-8726cf
flang-22: note: diagnostic msg: /gpfsm/dnb34/tdirs/login/discover36.93889.mathomp4/pair-8726cf.sh
flang-22: note: diagnostic msg:

********************
```

As directed, I'm attaching the files that can reproduce this. They are in a tarfile because, well, I can't seem to attach F90 files.

There are three files:
```
Flang-Crash-2025Nov14/
├── FromFlang
│   ├── pair-8726cf
│   └── pair-8726cf.sh
└── pair.F90

2 directories, 3 files
```

The `pair.F90` is our original repro, and the files in `FromFlang` are from 

[Flang-Crash-2025Nov14.tar.gz](https://github.com/user-attachments/files/23553478/Flang-Crash-2025Nov14.tar.gz)
</details>


---

### Comment 2 - klausler

Here is the offending source file:
```
module foo
   type :: Variable
   end type Variable
   type :: ExtDataFileStream
   end type ExtDataFileStream
   type :: ExtDataRule
   end type ExtDataRule
end module foo

module foo2
   use foo
   implicit none

   type :: pair
   end type pair

   interface pair
      module procedure m_newPair
   end interface pair

contains
      function m_newPair(key, value) result(p)
         type (pair) :: p
         character(len=*) , intent(in) :: key
         class (Variable) , intent(in) :: value
      end function m_newPair

end module foo2


module foo3
   use foo
   type :: pair
   end type pair


end module Foo3

module foo4
   use foo

   type :: pair
   end type pair

end module Foo4

module MAPL_ExtDataConfig
   use foo2
   use foo3
   use foo4
end module MAPL_ExtDataConfig
```

Note that the derived type `pair` has multiple definitions arriving via USE association in the scope of module `MAPI_ExtDataConfig`.  Like most compilers, we accept this so long as the derived type isn't actually referenced.  The crash comes from the code that is preparing the symbol table to emit an error message if that were to happen.

---

### Comment 3 - mathomp4

@klausler Dang. Fast work! Once it gets in I'll do another build of `main` to test it out. (Note we [have a workaround](https://github.com/GEOS-ESM/MAPL/pull/4174) for the code that triggered this but we figured we'd send this to you since other compilers didn't complain.)

---

