# [flang] Crash with coarray teams

**Author:** eugeneepshteyn
**URL:** https://github.com/llvm/llvm-project/issues/171048
**Status:** Open
**Labels:** flang:ir, crash

## Body

These llvm-test-suite tests are currently disabled, but if one tries to compile them, one gets a crash.

Tests: llvm-test-suite/Fortran/gfortran/regression: pr84784.f90, team_change_1.f90, team_end_1.f90

Crash:
```
$ flang -c -fcoarray llvm-test-suite/Fortran/gfortran/regression/pr84784.f90 
warning: Support for multi image Fortran features is still experimental and in development.
flang: llvm-project/llvm/include/llvm/Support/Casting.h:572: decltype(auto) llvm::cast(From *) [To = mif::ChangeTeamOp, From = mlir::Operation]: Assertion `isa<To>(Val) && "cast<Ty>() argument of incompatible type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /home/eepshteyn/compilers/flang-upstream/bin/flang -fc1 -triple x86_64-unknown-linux-gnu -emit-obj -fcolor-diagnostics -mrelocation-model pic -pic-level 2 -pic-is-pie -target-cpu x86-64 -fcoarray -resource-dir /home/eepshteyn/compilers/flang-upstream/lib/clang/22 -mframe-pointer=all -o pr84784.o -x f95 llvm-test-suite/Fortran/gfortran/regression/pr84784.f90
 #0 0x00005e0a25d46de1 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) llvm-project/llvm/lib/Support/Unix/Signals.inc:834:11
 #1 0x00005e0a25d4735b PrintStackTraceSignalHandler(void*) llvm-project/llvm/lib/Support/Unix/Signals.inc:916:1
 #2 0x00005e0a25d45076 llvm::sys::RunSignalHandlers() llvm-project/llvm/lib/Support/Signals.cpp:104:5
 #3 0x00005e0a25d47add SignalHandler(int, siginfo_t*, void*) llvm-project/llvm/lib/Support/Unix/Signals.inc:426:38
 #4 0x000072219de45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #5 0x000072219de9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #6 0x000072219de9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #7 0x000072219de9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #8 0x000072219de4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #9 0x000072219de288ff abort ./stdlib/abort.c:81:7
#10 0x000072219de2881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#11 0x000072219de3b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#12 0x00005e0a26d9a248 decltype(auto) llvm::cast<mif::ChangeTeamOp, mlir::Operation>(mlir::Operation*) llvm-project/llvm/include/llvm/Support/Casting.h:573:39
#13 0x00005e0a26d902fa std::enable_if<true, mif::ChangeTeamOp>::type mlir::OpTrait::HasParent<mif::ChangeTeamOp>::Impl<mif::EndTeamOp>::getParentOp<mif::ChangeTeamOp>() llvm-project/flang/../mlir/include/mlir/IR/OpDefinition.h:1321:14
#14 0x00005e0a26d8db44 Fortran::lower::genEndChangeTeamStmt(Fortran::lower::AbstractConverter&, Fortran::lower::pft::Evaluation&, Fortran::parser::EndChangeTeamStmt const&) llvm-project/flang/lib/Lower/MultiImageFortran.cpp:203:40
#15 0x00005e0a26377a5d (anonymous namespace)::FirConverter::genFIR(Fortran::parser::EndChangeTeamStmt const&) llvm-project/flang/lib/Lower/Bridge.cpp:4107:3
#16 0x00005e0a26377a20 auto (anonymous namespace)::FirConverter::genFIR(Fortran::lower::pft::Evaluation&, bool)::'lambda'(auto const&)::operator()<Fortran::parser::EndChangeTeamStmt>(auto const&) const llvm-project/flang/lib/Lower/Bridge.cpp:6104:54
#17 0x00005e0a26376037 auto auto Fortran::lower::pft::ReferenceVariantBase<true, Fortran::parser::AllocateStmt, Fortran::parser::AssignmentStmt, Fortran::parser::BackspaceStmt, Fortran::parser::CallStmt, Fortran::parser::CloseStmt, Fortran::parser::ContinueStmt, Fortran::parser::CycleStmt, Fortran::parser::DeallocateStmt, Fortran::parser::EndfileStmt, Fortran::parser::EventPostStmt, Fortran::parser::EventWaitStmt, Fortran::parser::ExitStmt, Fortran::parser::FailImageStmt, Fortran::parser::FlushStmt, Fortran::parser::FormTeamStmt, Fortran::parser::GotoStmt, Fortran::parser::IfStmt, Fortran::parser::InquireStmt, Fortran::parser::LockStmt, Fortran::parser::NotifyWaitStmt, Fortran::parser::NullifyStmt, Fortran::parser::OpenStmt, Fortran::parser::PointerAssignmentStmt, Fortran::parser::PrintStmt, Fortran::parser::ReadStmt, Fortran::parser::ReturnStmt, Fortran::parser::RewindStmt, Fortran::parser::StopStmt, Fortran::parser::SyncAllStmt, Fortran::parser::SyncImagesStmt, Fortran::parser::SyncMemoryStmt, Fortran::parser::SyncTeamStmt, Fortran::parser::UnlockStmt, Fortran::parser::WaitStmt, Fortran::parser::WhereStmt, Fortran::parser::WriteStmt, Fortran::parser::ComputedGotoStmt, Fortran::parser::ForallStmt, Fortran::parser::ArithmeticIfStmt, Fortran::parser::AssignStmt, Fortran::parser::AssignedGotoStmt, Fortran::parser::PauseStmt, Fortran::parser::EntryStmt, Fortran::parser::FormatStmt, Fortran::parser::AssociateStmt, Fortran::parser::EndAssociateStmt, Fortran::parser::BlockStmt, Fortran::parser::EndBlockStmt, Fortran::parser::SelectCaseStmt, Fortran::parser::CaseStmt, Fortran::parser::EndSelectStmt, Fortran::parser::ChangeTeamStmt, Fortran::parser::EndChangeTeamStmt, Fortran::parser::CriticalStmt, Fortran::parser::EndCriticalStmt, Fortran::parser::NonLabelDoStmt, Fortran::parser::EndDoStmt, Fortran::parser::IfThenStmt, Fortran::parser::ElseIfStmt, Fortran::parser::ElseStmt, Fortran::parser::EndIfStmt, Fortran::parser::SelectRankStmt, Fortran::parser::SelectRankCaseStmt, Fortran::parser::SelectTypeStmt, Fortran::parser::TypeGuardStmt, Fortran::parser::WhereConstructStmt, Fortran::parser::MaskedElsewhereStmt, Fortran::parser::ElsewhereStmt, Fortran::parser::EndWhereStmt, Fortran::parser::ForallConstructStmt, Fortran::parser::EndForallStmt, Fortran::parser::EndProgramStmt, Fortran::parser::EndFunctionStmt, Fortran::parser::EndSubroutineStmt, Fortran::parser::EndMpSubprogramStmt, Fortran::parser::AssociateConstruct, Fortran::parser::BlockConstruct, Fortran::parser::CaseConstruct, Fortran::parser::ChangeTeamConstruct, Fortran::parser::CriticalConstruct, Fortran::parser::DoConstruct, Fortran::parser::IfConstruct, Fortran::parser::SelectRankConstruct, Fortran::parser::SelectTypeConstruct, Fortran::parser::WhereConstruct, Fortran::parser::ForallConstruct, Fortran::parser::CompilerDirective, Fortran::parser::OpenACCConstruct, Fortran::parser::OpenACCRoutineConstruct, Fortran::parser::OpenACCDeclarativeConstruct, Fortran::parser::OpenMPConstruct, Fortran::parser::OpenMPDeclarativeConstruct, Fortran::parser::OmpEndLoopDirective, Fortran::parser::CUFKernelDoConstruct>::visit<(anonymous namespace)::FirConverter::genFIR(Fortran::lower::pft::Evaluation&, bool)::'lambda'(auto const&)>(auto&&) const::'lambda'(auto)::operator()<Fortran::common::Reference<Fortran::parser::EndChangeTeamStmt const>>(auto) const llvm-project/flang/include/flang/Lower/PFTBuilder.h:80:49
```

flang version: 315c904e3e1c00ea1ec7f0757e4c538ec2513624, x86_64-unknown-linux-gnu

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-flang-ir

Author: Eugene Epshteyn (eugeneepshteyn)

<details>
These llvm-test-suite tests are currently disabled, but if one tries to compile them, one gets a crash.

Tests: llvm-test-suite/Fortran/gfortran/regression: pr84784.f90, team_change_1.f90, team_end_1.f90

Crash:
```
$ flang -c -fcoarray llvm-test-suite/Fortran/gfortran/regression/pr84784.f90 
warning: Support for multi image Fortran features is still experimental and in development.
flang: llvm-project/llvm/include/llvm/Support/Casting.h:572: decltype(auto) llvm::cast(From *) [To = mif::ChangeTeamOp, From = mlir::Operation]: Assertion `isa&lt;To&gt;(Val) &amp;&amp; "cast&lt;Ty&gt;() argument of incompatible type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /home/eepshteyn/compilers/flang-upstream/bin/flang -fc1 -triple x86_64-unknown-linux-gnu -emit-obj -fcolor-diagnostics -mrelocation-model pic -pic-level 2 -pic-is-pie -target-cpu x86-64 -fcoarray -resource-dir /home/eepshteyn/compilers/flang-upstream/lib/clang/22 -mframe-pointer=all -o pr84784.o -x f95 llvm-test-suite/Fortran/gfortran/regression/pr84784.f90
 #<!-- -->0 0x00005e0a25d46de1 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) llvm-project/llvm/lib/Support/Unix/Signals.inc:834:11
 #<!-- -->1 0x00005e0a25d4735b PrintStackTraceSignalHandler(void*) llvm-project/llvm/lib/Support/Unix/Signals.inc:916:1
 #<!-- -->2 0x00005e0a25d45076 llvm::sys::RunSignalHandlers() llvm-project/llvm/lib/Support/Signals.cpp:104:5
 #<!-- -->3 0x00005e0a25d47add SignalHandler(int, siginfo_t*, void*) llvm-project/llvm/lib/Support/Unix/Signals.inc:426:38
 #<!-- -->4 0x000072219de45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->5 0x000072219de9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->6 0x000072219de9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->7 0x000072219de9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->8 0x000072219de4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x000072219de288ff abort ./stdlib/abort.c:81:7
#<!-- -->10 0x000072219de2881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->11 0x000072219de3b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->12 0x00005e0a26d9a248 decltype(auto) llvm::cast&lt;mif::ChangeTeamOp, mlir::Operation&gt;(mlir::Operation*) llvm-project/llvm/include/llvm/Support/Casting.h:573:39
#<!-- -->13 0x00005e0a26d902fa std::enable_if&lt;true, mif::ChangeTeamOp&gt;::type mlir::OpTrait::HasParent&lt;mif::ChangeTeamOp&gt;::Impl&lt;mif::EndTeamOp&gt;::getParentOp&lt;mif::ChangeTeamOp&gt;() llvm-project/flang/../mlir/include/mlir/IR/OpDefinition.h:1321:14
#<!-- -->14 0x00005e0a26d8db44 Fortran::lower::genEndChangeTeamStmt(Fortran::lower::AbstractConverter&amp;, Fortran::lower::pft::Evaluation&amp;, Fortran::parser::EndChangeTeamStmt const&amp;) llvm-project/flang/lib/Lower/MultiImageFortran.cpp:203:40
#<!-- -->15 0x00005e0a26377a5d (anonymous namespace)::FirConverter::genFIR(Fortran::parser::EndChangeTeamStmt const&amp;) llvm-project/flang/lib/Lower/Bridge.cpp:4107:3
#<!-- -->16 0x00005e0a26377a20 auto (anonymous namespace)::FirConverter::genFIR(Fortran::lower::pft::Evaluation&amp;, bool)::'lambda'(auto const&amp;)::operator()&lt;Fortran::parser::EndChangeTeamStmt&gt;(auto const&amp;) const llvm-project/flang/lib/Lower/Bridge.cpp:6104:54
#<!-- -->17 0x00005e0a26376037 auto auto Fortran::lower::pft::ReferenceVariantBase&lt;true, Fortran::parser::AllocateStmt, Fortran::parser::AssignmentStmt, Fortran::parser::BackspaceStmt, Fortran::parser::CallStmt, Fortran::parser::CloseStmt, Fortran::parser::ContinueStmt, Fortran::parser::CycleStmt, Fortran::parser::DeallocateStmt, Fortran::parser::EndfileStmt, Fortran::parser::EventPostStmt, Fortran::parser::EventWaitStmt, Fortran::parser::ExitStmt, Fortran::parser::FailImageStmt, Fortran::parser::FlushStmt, Fortran::parser::FormTeamStmt, Fortran::parser::GotoStmt, Fortran::parser::IfStmt, Fortran::parser::InquireStmt, Fortran::parser::LockStmt, Fortran::parser::NotifyWaitStmt, Fortran::parser::NullifyStmt, Fortran::parser::OpenStmt, Fortran::parser::PointerAssignmentStmt, Fortran::parser::PrintStmt, Fortran::parser::ReadStmt, Fortran::parser::ReturnStmt, Fortran::parser::RewindStmt, Fortran::parser::StopStmt, Fortran::parser::SyncAllStmt, Fortran::parser::SyncImagesStmt, Fortran::parser::SyncMemoryStmt, Fortran::parser::SyncTeamStmt, Fortran::parser::UnlockStmt, Fortran::parser::WaitStmt, Fortran::parser::WhereStmt, Fortran::parser::WriteStmt, Fortran::parser::ComputedGotoStmt, Fortran::parser::ForallStmt, Fortran::parser::ArithmeticIfStmt, Fortran::parser::AssignStmt, Fortran::parser::AssignedGotoStmt, Fortran::parser::PauseStmt, Fortran::parser::EntryStmt, Fortran::parser::FormatStmt, Fortran::parser::AssociateStmt, Fortran::parser::EndAssociateStmt, Fortran::parser::BlockStmt, Fortran::parser::EndBlockStmt, Fortran::parser::SelectCaseStmt, Fortran::parser::CaseStmt, Fortran::parser::EndSelectStmt, Fortran::parser::ChangeTeamStmt, Fortran::parser::EndChangeTeamStmt, Fortran::parser::CriticalStmt, Fortran::parser::EndCriticalStmt, Fortran::parser::NonLabelDoStmt, Fortran::parser::EndDoStmt, Fortran::parser::IfThenStmt, Fortran::parser::ElseIfStmt, Fortran::parser::ElseStmt, Fortran::parser::EndIfStmt, Fortran::parser::SelectRankStmt, Fortran::parser::SelectRankCaseStmt, Fortran::parser::SelectTypeStmt, Fortran::parser::TypeGuardStmt, Fortran::parser::WhereConstructStmt, Fortran::parser::MaskedElsewhereStmt, Fortran::parser::ElsewhereStmt, Fortran::parser::EndWhereStmt, Fortran::parser::ForallConstructStmt, Fortran::parser::EndForallStmt, Fortran::parser::EndProgramStmt, Fortran::parser::EndFunctionStmt, Fortran::parser::EndSubroutineStmt, Fortran::parser::EndMpSubprogramStmt, Fortran::parser::AssociateConstruct, Fortran::parser::BlockConstruct, Fortran::parser::CaseConstruct, Fortran::parser::ChangeTeamConstruct, Fortran::parser::CriticalConstruct, Fortran::parser::DoConstruct, Fortran::parser::IfConstruct, Fortran::parser::SelectRankConstruct, Fortran::parser::SelectTypeConstruct, Fortran::parser::WhereConstruct, Fortran::parser::ForallConstruct, Fortran::parser::CompilerDirective, Fortran::parser::OpenACCConstruct, Fortran::parser::OpenACCRoutineConstruct, Fortran::parser::OpenACCDeclarativeConstruct, Fortran::parser::OpenMPConstruct, Fortran::parser::OpenMPDeclarativeConstruct, Fortran::parser::OmpEndLoopDirective, Fortran::parser::CUFKernelDoConstruct&gt;::visit&lt;(anonymous namespace)::FirConverter::genFIR(Fortran::lower::pft::Evaluation&amp;, bool)::'lambda'(auto const&amp;)&gt;(auto&amp;&amp;) const::'lambda'(auto)::operator()&lt;Fortran::common::Reference&lt;Fortran::parser::EndChangeTeamStmt const&gt;&gt;(auto) const llvm-project/flang/include/flang/Lower/PFTBuilder.h:80:49
```

flang version: 315c904e3e1c00ea1ec7f0757e4c538ec2513624, x86_64-unknown-linux-gnu
</details>


---

### Comment 2 - JDPailleux

Hi @eugeneepshteyn , noted ! 
Btw, just for your information, we are also working on adding tests to the LLVM test suite using Caffeine.

---

### Comment 3 - ktras

@eugeneepshteyn PR https://github.com/llvm/llvm-project/pull/172259 has been merged to `main`. Can you please confirm the PR fixes this bug?

---

