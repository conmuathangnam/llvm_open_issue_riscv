# [flang] crashes during parsing of module file when module PDT is abstract

**Author:** edvinolo
**URL:** https://github.com/llvm/llvm-project/issues/174859
**Status:** Closed
**Labels:** flang:frontend, crash
**Closed Date:** 2026-01-08T16:59:08Z

## Body

Reproducer:
https://gcc.godbolt.org/z/4Tdxq4csE

m.f90:
```fortran
module m
  type, abstract :: t(k)
    integer, kind :: k
  end type
end
```
test.f90:
```fortran
program test
  use m
end program test
```

Backtrace:
```
fatal internal error: CHECK(!parseState.anyErrorRecovery() || parseState.messages().AnyFatalError()) failed at /root/llvm-project/flang/lib/Parser/parsing.cpp(289)
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /cefs/ca/caa1d3d56e12d10c8047a543_clang-llvmflang-trunk-20260106/bin/flang -fc1 -triple x86_64-unknown-linux-gnu -emit-obj -mrelocation-model pic -pic-level 2 -pic-is-pie -target-cpu x86-64 -resource-dir /cefs/ca/caa1d3d56e12d10c8047a543_clang-llvmflang-trunk-20260106/lib/clang/22 -mframe-pointer=all -o CMakeFiles/test.dir/test.f90.o -x f95 /app/test.f90
 #0 0x0000000004564ec8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/cefs/ca/caa1d3d56e12d10c8047a543_clang-llvmflang-trunk-20260106/bin/flang+0x4564ec8)
 #1 0x0000000004562094 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x00007da90a642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x00007da90a6969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x00007da90a642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x00007da90a6287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x000000000679d899 (/cefs/ca/caa1d3d56e12d10c8047a543_clang-llvmflang-trunk-20260106/bin/flang+0x679d899)
 #7 0x00000000064117c5 Fortran::parser::Parsing::Parse(llvm::raw_ostream&) (/cefs/ca/caa1d3d56e12d10c8047a543_clang-llvmflang-trunk-20260106/bin/flang+0x64117c5)
 #8 0x000000000581a74c Fortran::semantics::ModFileReader::Read(Fortran::parser::CharBlock, std::optional<bool>, Fortran::semantics::Scope*, bool) (/cefs/ca/caa1d3d56e12d10c8047a543_clang-llvmflang-trunk-20260106/bin/flang+0x581a74c)
 #9 0x00000000050355d5 Fortran::semantics::ModuleVisitor::FindModule(Fortran::parser::Name const&, std::optional<bool>, Fortran::semantics::Scope*) (/cefs/ca/caa1d3d56e12d10c8047a543_clang-llvmflang-trunk-20260106/bin/flang+0x50355d5)
#10 0x00000000050358bc Fortran::semantics::ModuleVisitor::Pre(Fortran::parser::UseStmt const&) (/cefs/ca/caa1d3d56e12d10c8047a543_clang-llvmflang-trunk-20260106/bin/flang+0x50358bc)
#11 0x000000000509d785 Fortran::semantics::ResolveNamesVisitor::Pre(Fortran::parser::SpecificationPart const&) (/cefs/ca/caa1d3d56e12d10c8047a543_clang-llvmflang-trunk-20260106/bin/flang+0x509d785)
#12 0x00000000050a96d7 Fortran::semantics::ResolveNamesVisitor::ResolveSpecificationParts(Fortran::semantics::ProgramTree&) (.part.0) resolve-names.cpp:0:0
#13 0x00000000050b5fe3 Fortran::semantics::ResolveNamesVisitor::Pre(Fortran::parser::ProgramUnit const&) (/cefs/ca/caa1d3d56e12d10c8047a543_clang-llvmflang-trunk-20260106/bin/flang+0x50b5fe3)
#14 0x00000000050b760c Fortran::semantics::ResolveNames(Fortran::semantics::SemanticsContext&, Fortran::parser::Program const&, Fortran::semantics::Scope&) (/cefs/ca/caa1d3d56e12d10c8047a543_clang-llvmflang-trunk-20260106/bin/flang+0x50b760c)
#15 0x00000000051392ee Fortran::semantics::Semantics::Perform() (/cefs/ca/caa1d3d56e12d10c8047a543_clang-llvmflang-trunk-20260106/bin/flang+0x51392ee)
#16 0x00000000045c99b8 Fortran::frontend::FrontendAction::runSemanticChecks() (/cefs/ca/caa1d3d56e12d10c8047a543_clang-llvmflang-trunk-20260106/bin/flang+0x45c99b8)
#17 0x00000000048764c0 Fortran::frontend::CodeGenAction::beginSourceFileAction() (/cefs/ca/caa1d3d56e12d10c8047a543_clang-llvmflang-trunk-20260106/bin/flang+0x48764c0)
#18 0x00000000045c8615 Fortran::frontend::FrontendAction::beginSourceFile(Fortran::frontend::CompilerInstance&, Fortran::frontend::FrontendInputFile const&) (/cefs/ca/caa1d3d56e12d10c8047a543_clang-llvmflang-trunk-20260106/bin/flang+0x45c8615)
#19 0x00000000045b58df Fortran::frontend::CompilerInstance::executeAction(Fortran::frontend::FrontendAction&) (/cefs/ca/caa1d3d56e12d10c8047a543_clang-llvmflang-trunk-20260106/bin/flang+0x45b58df)
#20 0x00000000045d1f53 Fortran::frontend::executeCompilerInvocation(Fortran::frontend::CompilerInstance*) (/cefs/ca/caa1d3d56e12d10c8047a543_clang-llvmflang-trunk-20260106/bin/flang+0x45d1f53)
#21 0x00000000027faec2 fc1_main(llvm::ArrayRef<char const*>, char const*) (/cefs/ca/caa1d3d56e12d10c8047a543_clang-llvmflang-trunk-20260106/bin/flang+0x27faec2)
#22 0x0000000002699051 main (/cefs/ca/caa1d3d56e12d10c8047a543_clang-llvmflang-trunk-20260106/bin/flang+0x2699051)
#23 0x00007da90a629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#24 0x00007da90a629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#25 0x00000000027f9985 _start (/cefs/ca/caa1d3d56e12d10c8047a543_clang-llvmflang-trunk-20260106/bin/flang+0x27f9985)
flang-22: error: unable to execute command: Aborted (core dumped)
flang-22: error: flang frontend command failed due to signal (use -v to see invocation)
flang version 22.0.0git (https://github.com/llvm/llvm-project.git f9181a82fd00699ef2f840f2b7f8e9d54b2b270d)
Target: x86_64-unknown-linux-gnu
Thread model: posix
```

Other information:
If line 287 ``` parseState.set_deferMessages(options_.isModuleFile);```  of  ```flang/lib/Parser/parsing.cpp``` is removed, then the crash no longer occurs, but flang returns an error: ```Cannot parse module file for module 'm': Module file is corrupt: ./m.mod```

I also noted in the generated module file that the line for ```k``` looks like ```integer(4),kind,abstract::k```. Is the ```abstract``` attribute supposed to be there?

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-flang-frontend

Author: Edvin Olofsson (edvinolo)

<details>
Reproducer:
https://gcc.godbolt.org/z/4Tdxq4csE

m.f90:
```fortran
module m
  type, abstract :: t(k)
    integer, kind :: k
  end type
end
```
test.f90:
```fortran
program test
  use m
end program test
```

Backtrace:
```
fatal internal error: CHECK(!parseState.anyErrorRecovery() || parseState.messages().AnyFatalError()) failed at /root/llvm-project/flang/lib/Parser/parsing.cpp(289)
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /cefs/ca/caa1d3d56e12d10c8047a543_clang-llvmflang-trunk-20260106/bin/flang -fc1 -triple x86_64-unknown-linux-gnu -emit-obj -mrelocation-model pic -pic-level 2 -pic-is-pie -target-cpu x86-64 -resource-dir /cefs/ca/caa1d3d56e12d10c8047a543_clang-llvmflang-trunk-20260106/lib/clang/22 -mframe-pointer=all -o CMakeFiles/test.dir/test.f90.o -x f95 /app/test.f90
 #<!-- -->0 0x0000000004564ec8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/cefs/ca/caa1d3d56e12d10c8047a543_clang-llvmflang-trunk-20260106/bin/flang+0x4564ec8)
 #<!-- -->1 0x0000000004562094 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->2 0x00007da90a642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->3 0x00007da90a6969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->4 0x00007da90a642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->5 0x00007da90a6287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->6 0x000000000679d899 (/cefs/ca/caa1d3d56e12d10c8047a543_clang-llvmflang-trunk-20260106/bin/flang+0x679d899)
 #<!-- -->7 0x00000000064117c5 Fortran::parser::Parsing::Parse(llvm::raw_ostream&amp;) (/cefs/ca/caa1d3d56e12d10c8047a543_clang-llvmflang-trunk-20260106/bin/flang+0x64117c5)
 #<!-- -->8 0x000000000581a74c Fortran::semantics::ModFileReader::Read(Fortran::parser::CharBlock, std::optional&lt;bool&gt;, Fortran::semantics::Scope*, bool) (/cefs/ca/caa1d3d56e12d10c8047a543_clang-llvmflang-trunk-20260106/bin/flang+0x581a74c)
 #<!-- -->9 0x00000000050355d5 Fortran::semantics::ModuleVisitor::FindModule(Fortran::parser::Name const&amp;, std::optional&lt;bool&gt;, Fortran::semantics::Scope*) (/cefs/ca/caa1d3d56e12d10c8047a543_clang-llvmflang-trunk-20260106/bin/flang+0x50355d5)
#<!-- -->10 0x00000000050358bc Fortran::semantics::ModuleVisitor::Pre(Fortran::parser::UseStmt const&amp;) (/cefs/ca/caa1d3d56e12d10c8047a543_clang-llvmflang-trunk-20260106/bin/flang+0x50358bc)
#<!-- -->11 0x000000000509d785 Fortran::semantics::ResolveNamesVisitor::Pre(Fortran::parser::SpecificationPart const&amp;) (/cefs/ca/caa1d3d56e12d10c8047a543_clang-llvmflang-trunk-20260106/bin/flang+0x509d785)
#<!-- -->12 0x00000000050a96d7 Fortran::semantics::ResolveNamesVisitor::ResolveSpecificationParts(Fortran::semantics::ProgramTree&amp;) (.part.0) resolve-names.cpp:0:0
#<!-- -->13 0x00000000050b5fe3 Fortran::semantics::ResolveNamesVisitor::Pre(Fortran::parser::ProgramUnit const&amp;) (/cefs/ca/caa1d3d56e12d10c8047a543_clang-llvmflang-trunk-20260106/bin/flang+0x50b5fe3)
#<!-- -->14 0x00000000050b760c Fortran::semantics::ResolveNames(Fortran::semantics::SemanticsContext&amp;, Fortran::parser::Program const&amp;, Fortran::semantics::Scope&amp;) (/cefs/ca/caa1d3d56e12d10c8047a543_clang-llvmflang-trunk-20260106/bin/flang+0x50b760c)
#<!-- -->15 0x00000000051392ee Fortran::semantics::Semantics::Perform() (/cefs/ca/caa1d3d56e12d10c8047a543_clang-llvmflang-trunk-20260106/bin/flang+0x51392ee)
#<!-- -->16 0x00000000045c99b8 Fortran::frontend::FrontendAction::runSemanticChecks() (/cefs/ca/caa1d3d56e12d10c8047a543_clang-llvmflang-trunk-20260106/bin/flang+0x45c99b8)
#<!-- -->17 0x00000000048764c0 Fortran::frontend::CodeGenAction::beginSourceFileAction() (/cefs/ca/caa1d3d56e12d10c8047a543_clang-llvmflang-trunk-20260106/bin/flang+0x48764c0)
#<!-- -->18 0x00000000045c8615 Fortran::frontend::FrontendAction::beginSourceFile(Fortran::frontend::CompilerInstance&amp;, Fortran::frontend::FrontendInputFile const&amp;) (/cefs/ca/caa1d3d56e12d10c8047a543_clang-llvmflang-trunk-20260106/bin/flang+0x45c8615)
#<!-- -->19 0x00000000045b58df Fortran::frontend::CompilerInstance::executeAction(Fortran::frontend::FrontendAction&amp;) (/cefs/ca/caa1d3d56e12d10c8047a543_clang-llvmflang-trunk-20260106/bin/flang+0x45b58df)
#<!-- -->20 0x00000000045d1f53 Fortran::frontend::executeCompilerInvocation(Fortran::frontend::CompilerInstance*) (/cefs/ca/caa1d3d56e12d10c8047a543_clang-llvmflang-trunk-20260106/bin/flang+0x45d1f53)
#<!-- -->21 0x00000000027faec2 fc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*) (/cefs/ca/caa1d3d56e12d10c8047a543_clang-llvmflang-trunk-20260106/bin/flang+0x27faec2)
#<!-- -->22 0x0000000002699051 main (/cefs/ca/caa1d3d56e12d10c8047a543_clang-llvmflang-trunk-20260106/bin/flang+0x2699051)
#<!-- -->23 0x00007da90a629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->24 0x00007da90a629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->25 0x00000000027f9985 _start (/cefs/ca/caa1d3d56e12d10c8047a543_clang-llvmflang-trunk-20260106/bin/flang+0x27f9985)
flang-22: error: unable to execute command: Aborted (core dumped)
flang-22: error: flang frontend command failed due to signal (use -v to see invocation)
flang version 22.0.0git (https://github.com/llvm/llvm-project.git f9181a82fd00699ef2f840f2b7f8e9d54b2b270d)
Target: x86_64-unknown-linux-gnu
Thread model: posix
```

Other information:
If line 287 ``` parseState.set_deferMessages(options_.isModuleFile);```  of  ```flang/lib/Parser/parsing.cpp``` is removed, then the crash no longer occurs, but flang returns an error: ```Cannot parse module file for module 'm': Module file is corrupt: ./m.mod```

I also noted in the generated module file that the line for ```k``` looks like ```integer(4),kind,abstract::k```. Is the ```abstract``` attribute supposed to be there?
</details>


---

