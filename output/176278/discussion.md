# [Flang][OpenMP] Compilation error when type-list in declare reduction directive is derived type name

**Author:** ohno-fj
**URL:** https://github.com/llvm/llvm-project/issues/176278
**Status:** Open
**Labels:** flang:ir, crash, flang:openmp

## Body

```
Version of flang : 22.0.0(2f7e218017db69454ea84e1849fcb9a11b1f7fef)/AArch64
```

In the attached program (`masa_udr0009_2.f90`), specifying `derived type name` in `type-list` of `declare reduction directive` causes the compilation to terminate abnormally.
After checking `OpenMP 6.0 : 7.6.14 declare_reduction Directive`, the program appears to contain no particular errors.

The following are the test program, Flang, Gfortran and ifx compilation/execution results.

masa_udr0009_2.f90:
```fortran
program main
  type t
     integer::i=0
  end type t
!$omp declare reduction (+:t:omp_out%i=omp_out%i+omp_in%i) &
!$omp         initializer(omp_priv%i=0)
  integer::i
  type(t)::x1
  x1%i=0
!$omp parallel do reduction(+:x1)
  do i=1,10
     x1%i=x1%i+1
  end do
!$omp end parallel do
  print *,'pass'
end program main
```

```
$ flang -fopenmp masa_udr0009_2.f90
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: /work/groups/ssoft/compiler/llvm/aarch64/main-20260105-a73de982a95b/bin/flang -fc1 -triple aarch64-unknown-linux-gnu -emit-obj -mrelocation-model pic -pic-level 2 -pic-is-pie -target-cpu generic -target-feature
 +v8a -target-feature +fp-armv8 -target-feature +neon -fopenmp -resource-dir /work/groups/ssoft/compiler/llvm/aarch64/main-20260105-a73de982a95b/lib/clang/22 -mframe-pointer=non-leaf-no-reserve -o /tmp/masa_udr0009_2-ceec8b.o -x f95 masa
_udr0009_2.f90
 #0 0x0000400035f39e80 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/work/groups/ssoft/compiler/llvm/aarch64/main-latest/lib/libLLVM.so.22.0git+0x6419e80)
 #1 0x0000400035f37428 llvm::sys::RunSignalHandlers() (/work/groups/ssoft/compiler/llvm/aarch64/main-latest/lib/libLLVM.so.22.0git+0x6417428)
 #2 0x0000400035f37580 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x00004000237607a0 (linux-vdso.so.1+0x7a0)
 #4 0x000040003b0a62e4 raise (/lib64/libc.so.6+0x362e4)
 #5 0x000040003b090aac abort (/lib64/libc.so.6+0x20aac)
 #6 0x00000000066dff20 genOMP(Fortran::lower::AbstractConverter&, Fortran::lower::SymMap&, Fortran::semantics::SemanticsContext&, Fortran::lower::pft::Evaluation&, Fortran::parser::OpenMPDeclareReductionConstruct const&) (.isra.22387) Op
enMP.cpp:0:0
 #7 0x00000000066cabe8 Fortran::lower::genOpenMPDeclarativeConstruct(Fortran::lower::AbstractConverter&, Fortran::lower::SymMap&, Fortran::semantics::SemanticsContext&, Fortran::lower::pft::Evaluation&, Fortran::parser::OpenMPDeclarative
Construct const&) (/work/groups/ssoft/compiler/llvm/aarch64/main-20260105-a73de982a95b/bin/flang+0x66cabe8)
 #8 0x000000000632bd44 std::__detail::__variant::__gen_vtable_impl<std::__detail::__variant::_Multi_array<void (*)(Fortran::common::visitors<auto Fortran::lower::pft::ReferenceVariantBase<true, Fortran::parser::AllocateStmt, Fortran::par
ser::AssignmentStmt, Fortran::parser::BackspaceStmt, Fortran::parser::CallStmt, Fortran::parser::CloseStmt, Fortran::parser::ContinueStmt, Fortran::parser::CycleStmt, Fortran::parser::DeallocateStmt, Fortran::parser::EndfileStmt, Fortran
::parser::EventPostStmt, Fortran::parser::EventWaitStmt, Fortran::parser::ExitStmt, Fortran::parser::FailImageStmt, Fortran::parser::FlushStmt, Fortran::parser::FormTeamStmt, Fortran::parser::GotoStmt, Fortran::parser::IfStmt, Fortran::p
arser::InquireStmt, Fortran::parser::LockStmt, Fortran::parser::NotifyWaitStmt, Fortran::parser::NullifyStmt, Fortran::parser::OpenStmt, Fortran::parser::PointerAssignmentStmt, Fortran::parser::PrintStmt, Fortran::parser::ReadStmt, Fortr
an::parser::ReturnStmt, Fortran::parser::RewindStmt, Fortran::parser::StopStmt, Fortran::parser::SyncAllStmt, Fortran::parser::SyncImagesStmt, Fortran::parser::SyncMemoryStmt, Fortran::parser::SyncTeamStmt, Fortran::parser::UnlockStmt, F
ortran::parser::WaitStmt, Fortran::parser::WhereStmt, Fortran::parser::WriteStmt, Fortran::parser::ComputedGotoStmt, Fortran::parser::ForallStmt, Fortran::parser::ArithmeticIfStmt, Fortran::parser::AssignStmt, Fortran::parser::AssignedGo
toStmt, Fortran::parser::PauseStmt, Fortran::parser::EntryStmt, Fortran::parser::FormatStmt, Fortran::parser::AssociateStmt, Fortran::parser::EndAssociateStmt, Fortran::parser::BlockStmt, Fortran::parser::EndBlockStmt, Fortran::parser::S
electCaseStmt, Fortran::parser::CaseStmt, Fortran::parser::EndSelectStmt, Fortran::parser::ChangeTeamStmt, Fortran::parser::EndChangeTeamStmt, Fortran::parser::CriticalStmt, Fortran::parser::EndCriticalStmt, Fortran::parser::NonLabelDoSt
mt, Fortran::parser::EndDoStmt, Fortran::parser::IfThenStmt, Fortran::parser::ElseIfStmt, Fortran::parser::ElseStmt, Fortran::parser::EndIfStmt, Fortran::parser::SelectRankStmt, Fortran::parser::SelectRankCaseStmt, Fortran::parser::Selec
tTypeStmt, Fortran::parser::TypeGuardStmt, Fortran::parser::WhereConstructStmt, Fortran::parser::MaskedElsewhereStmt, Fortran::parser::ElsewhereStmt, Fortran::parser::EndWhereStmt, Fortran::parser::ForallConstructStmt, Fortran::parser::E
ndForallStmt, Fortran::parser::EndProgramStmt, Fortran::parser::EndFunctionStmt, Fortran::parser::EndSubroutineStmt, Fortran::parser::EndMpSubprogramStmt, Fortran::parser::AssociateConstruct, Fortran::parser::BlockConstruct, Fortran::par
ser::CaseConstruct, Fortran::parser::ChangeTeamConstruct, Fortran::parser::CriticalConstruct, Fortran::parser::DoConstruct, Fortran::parser::IfConstruct, Fortran::parser::SelectRankConstruct, Fortran::parser::SelectTypeConstruct, Fortran
::parser::WhereConstruct, Fortran::parser::ForallConstruct, Fortran::parser::CompilerDirective, Fortran::parser::OpenACCConstruct, Fortran::parser::OpenACCRoutineConstruct, Fortran::parser::OpenACCDeclarativeConstruct, Fortran::parser::O
penMPConstruct, Fortran::parser::OpenMPDeclarativeConstruct, Fortran::parser::OmpEndLoopDirective, Fortran::parser::CUFKernelDoConstruct>::visit<(anonymous namespace)::FirConverter::genFIR(Fortran::lower::pft::Evaluation&, bool)::'lambda
'(auto const&)>(auto&&) const::'lambda'(auto)>&&, std::variant<Fortran::common::Reference<Fortran::parser::AllocateStmt const>, Fortran::common::Reference<Fortran::parser::AssignmentStmt const>, Fortran::common::Reference<Fortran::parser
::BackspaceStmt const>, Fortran::common::Reference<Fortran::parser::CallStmt const>, Fortran::common::Reference<Fortran::parser::CloseStmt const>, Fortran::common::Reference<Fortran::parser::ContinueStmt const>, Fortran::common::Referenc
e<Fortran::parser::CycleStmt const>, Fortran::common::Reference<Fortran::parser::DeallocateStmt const>, Fortran::common::Reference<Fortran::parser::EndfileStmt const>, Fortran::common::Reference<Fortran::parser::EventPostStmt const>, For
tran::common::Reference<Fortran::parser::EventWaitStmt const>, Fortran::common::Reference<Fortran::parser::ExitStmt const>, Fortran::common::Reference<Fortran::parser::FailImageStmt const>, Fortran::common::Reference<Fortran::parser::Flu
shStmt const>, Fortran::common::Reference<Fortran::parser::FormTeamStmt const>, Fortran::common::Reference<Fortran::parser::GotoStmt const>, Fortran::common::Reference<Fortran::parser::IfStmt const>, Fortran::common::Reference<Fortran::p
arser::InquireStmt const>, Fortran::common::Reference<Fortran::parser::LockStmt const>, Fortran::common::Reference<Fortran::parser::NotifyWaitStmt const>, Fortran::common::Reference<Fortran::parser::NullifyStmt const>, Fortran::common::R
eference<Fortran::parser::OpenStmt const>, Fortran::common::Reference<Fortran::parser::PointerAssignmentStmt const>, Fortran::common::Reference<Fortran::parser::PrintStmt const>, Fortran::common::Reference<Fortran::parser::ReadStmt const
>, Fortran::common::Reference<Fortran::parser::ReturnStmt const>, Fortran::common::Reference<Fortran::parser::RewindStmt const>, Fortran::common::Reference<Fortran::parser::StopStmt const>, Fortran::common::Reference<Fortran::parser::Syn
cAllStmt const>, Fortran::common::Reference<Fortran::parser::SyncImagesStmt const>, Fortran::common::Reference<Fortran::parser::SyncMemoryStmt const>, Fortran::common::Reference<Fortran::parser::SyncTeamStmt const>, Fortran::common::Refe
rence<Fortran::parser::UnlockStmt const>, Fortran::common::Reference<Fortran::parser::WaitStmt const>, Fortran::common::Reference<Fortran::parser::WhereStmt const>, Fortran::common::Reference<Fortran::parser::WriteStmt const>, Fortran::c
ommon::Reference<Fortran::parser::ComputedGotoStmt const>, Fortran::common::Reference<Fortran::parser::ForallStmt const>, Fortran::common::Reference<Fortran::parser::ArithmeticIfStmt const>, Fortran::common::Reference<Fortran::parser::As
signStmt const>, Fortran::common::Reference<Fortran::parser::AssignedGotoStmt const>, Fortran::common::Reference<Fortran::parser::PauseStmt const>, Fortran::common::Reference<Fortran::parser::EntryStmt const>, Fortran::common::Reference<
Fortran::parser::FormatStmt const>, Fortran::common::Reference<Fortran::parser::AssociateStmt const>, Fortran::common::Reference<Fortran::parser::EndAssociateStmt const>, Fortran::common::Reference<Fortran::parser::BlockStmt const>, Fort
ran::common::Reference<Fortran::parser::EndBlockStmt const>, Fortran::common::Reference<Fortran::parser::SelectCaseStmt const>, Fortran::common::Reference<Fortran::parser::CaseStmt const>, Fortran::common::Reference<Fortran::parser::EndS
electStmt const>, Fortran::common::Reference<Fortran::parser::ChangeTeamStmt const>, Fortran::common::Reference<Fortran::parser::EndChangeTeamStmt const>, Fortran::common::Reference<Fortran::parser::CriticalStmt const>, Fortran::common::
Reference<Fortran::parser::EndCriticalStmt const>, Fortran::common::Reference<Fortran::parser::NonLabelDoStmt const>, Fortran::common::Reference<Fortran::parser::EndDoStmt const>, Fortran::common::Reference<Fortran::parser::IfThenStmt co
nst>, Fortran::common::Reference<Fortran::parser::ElseIfStmt const>, Fortran::common::Reference<Fortran::parser::ElseStmt const>, Fortran::common::Reference<Fortran::parser::EndIfStmt const>, Fortran::common::Reference<Fortran::parser::S
electRankStmt const>, Fortran::common::Reference<Fortran::parser::SelectRankCaseStmt const>, Fortran::common::Reference<Fortran::parser::SelectTypeStmt const>, Fortran::common::Reference<Fortran::parser::TypeGuardStmt const>, Fortran::co
mmon::Reference<Fortran::parser::WhereConstructStmt const>, Fortran::common::Reference<Fortran::parser::MaskedElsewhereStmt const>, Fortran::common::Reference<Fortran::parser::ElsewhereStmt const>, Fortran::common::Reference<Fortran::par
ser::EndWhereStmt const>, Fortran::common::Reference<Fortran::parser::ForallConstructStmt const>, Fortran::common::Reference<Fortran::parser::EndForallStmt const>, Fortran::common::Reference<Fortran::parser::EndProgramStmt const>, Fortra
n::common::Reference<Fortran::parser::EndFunctionStmt const>, Fortran::common::Reference<Fortran::parser::EndSubroutineStmt const>, Fortran::common::Reference<Fortran::parser::EndMpSubprogramStmt const>, Fortran::common::Reference<Fortra
n::parser::AssociateConstruct const>, Fortran::common::Reference<Fortran::parser::BlockConstruct const>, Fortran::common::Reference<Fortran::parser::CaseConstruct const>, Fortran::common::Reference<Fortran::parser::ChangeTeamConstruct co
nst>, Fortran::common::Reference<Fortran::parser::CriticalConstruct const>, Fortran::common::Reference<Fortran::parser::DoConstruct const>, Fortran::common::Reference<Fortran::parser::IfConstruct const>, Fortran::common::Reference<Fortra
n::parser::SelectRankConstruct const>, Fortran::common::Reference<Fortran::parser::SelectTypeConstruct const>, Fortran::common::Reference<Fortran::parser::WhereConstruct const>, Fortran::common::Reference<Fortran::parser::ForallConstruct
 const>, Fortran::common::Reference<Fortran::parser::CompilerDirective const>, Fortran::common::Reference<Fortran::parser::OpenACCConstruct const>, Fortran::common::Reference<Fortran::parser::OpenACCRoutineConstruct const>, Fortran::comm
on::Reference<Fortran::parser::OpenACCDeclarativeConstruct const>, Fortran::common::Reference<Fortran::parser::OpenMPConstruct const>, Fortran::common::Reference<Fortran::parser::OpenMPDeclarativeConstruct const>, Fortran::common::Refere
nce<Fortran::parser::OmpEndLoopDirective const>, Fortran::common::Reference<Fortran::parser::CUFKernelDoConstruct const>> const&)>, std::tuple<std::variant<Fortran::common::Reference<Fortran::parser::AllocateStmt const>, Fortran::common:
:Reference<Fortran::parser::AssignmentStmt const>, Fortran::common::Reference<Fortran::parser::BackspaceStmt const>, Fortran::common::Reference<Fortran::parser::CallStmt const>, Fortran::common::Reference<Fortran::parser::CloseStmt const
>, Fortran::common::Reference<Fortran::parser::ContinueStmt const>, Fortran::common::Reference<Fortran::parser::CycleStmt const>, Fortran::common::Reference<Fortran::parser::DeallocateStmt const>, Fortran::common::Reference<Fortran::pars
er::EndfileStmt const>, Fortran::common::Reference<Fortran::parser::EventPostStmt const>, Fortran::common::Reference<Fortran::parser::EventWaitStmt const>, Fortran::common::Reference<Fortran::parser::ExitStmt const>, Fortran::common::Ref
erence<Fortran::parser::FailImageStmt const>, Fortran::common::Reference<Fortran::parser::FlushStmt const>, Fortran::common::Reference<Fortran::parser::FormTeamStmt const>, Fortran::common::Reference<Fortran::parser::GotoStmt const>, For
tran::common::Reference<Fortran::parser::IfStmt const>, Fortran::common::Reference<Fortran::parser::InquireStmt const>, Fortran::common::Reference<Fortran::parser::LockStmt const>, Fortran::common::Reference<Fortran::parser::NotifyWaitSt
mt const>, Fortran::common::Reference<Fortran::parser::NullifyStmt const>, Fortran::common::Reference<Fortran::parser::OpenStmt const>, Fortran::common::Reference<Fortran::parser::PointerAssignmentStmt const>, Fortran::common::Reference<
Fortran::parser::PrintStmt const>, Fortran::common::Reference<Fortran::parser::ReadStmt const>, Fortran::common::Reference<Fortran::parser::ReturnStmt const>, Fortran::common::Reference<Fortran::parser::RewindStmt const>, Fortran::common
::Reference<Fortran::parser::StopStmt const>, Fortran::common::Reference<Fortran::parser::SyncAllStmt const>, Fortran::common::Reference<Fortran::parser::SyncImagesStmt const>, Fortran::common::Reference<Fortran::parser::SyncMemoryStmt c
onst>, Fortran::common::Reference<Fortran::parser::SyncTeamStmt const>, Fortran::common::Reference<Fortran::parser::UnlockStmt const>, Fortran::common::Reference<Fortran::parser::WaitStmt const>, Fortran::common::Reference<Fortran::parse
r::WhereStmt const>, Fortran::common::Reference<Fortran::parser::WriteStmt const>, Fortran::common::Reference<Fortran::parser::ComputedGotoStmt const>, Fortran::common::Reference<Fortran::parser::ForallStmt const>, Fortran::common::Refer
ence<Fortran::parser::ArithmeticIfStmt const>, Fortran::common::Reference<Fortran::parser::AssignStmt const>, Fortran::common::Reference<Fortran::parser::AssignedGotoStmt const>, Fortran::common::Reference<Fortran::parser::PauseStmt cons
t>, Fortran::common::Reference<Fortran::parser::EntryStmt const>, Fortran::common::Reference<Fortran::parser::FormatStmt const>, Fortran::common::Reference<Fortran::parser::AssociateStmt const>, Fortran::common::Reference<Fortran::parser
::EndAssociateStmt const>, Fortran::common::Reference<Fortran::parser::BlockStmt const>, Fortran::common::Reference<Fortran::parser::EndBlockStmt const>, Fortran::common::Reference<Fortran::parser::SelectCaseStmt const>, Fortran::common:
:Reference<Fortran::parser::CaseStmt const>, Fortran::common::Reference<Fortran::parser::EndSelectStmt const>, Fortran::common::Reference<Fortran::parser::ChangeTeamStmt const>, Fortran::common::Reference<Fortran::parser::EndChangeTeamSt
mt const>, Fortran::common::Reference<Fortran::parser::CriticalStmt const>, Fortran::common::Reference<Fortran::parser::EndCriticalStmt const>, Fortran::common::Reference<Fortran::parser::NonLabelDoStmt const>, Fortran::common::Reference
<Fortran::parser::EndDoStmt const>, Fortran::common::Reference<Fortran::parser::IfThenStmt const>, Fortran::common::Reference<Fortran::parser::ElseIfStmt const>, Fortran::common::Reference<Fortran::parser::ElseStmt const>, Fortran::commo
n::Reference<Fortran::parser::EndIfStmt const>, Fortran::common::Reference<Fortran::parser::SelectRankStmt const>, Fortran::common::Reference<Fortran::parser::SelectRankCaseStmt const>, Fortran::common::Reference<Fortran::parser::SelectT
ypeStmt const>, Fortran::common::Reference<Fortran::parser::TypeGuardStmt const>, Fortran::common::Reference<Fortran::parser::WhereConstructStmt const>, Fortran::common::Reference<Fortran::parser::MaskedElsewhereStmt const>, Fortran::com
mon::Reference<Fortran::parser::ElsewhereStmt const>, Fortran::common::Reference<Fortran::parser::EndWhereStmt const>, Fortran::common::Reference<Fortran::parser::ForallConstructStmt const>, Fortran::common::Reference<Fortran::parser::En
dForallStmt const>, Fortran::common::Reference<Fortran::parser::EndProgramStmt const>, Fortran::common::Reference<Fortran::parser::EndFunctionStmt const>, Fortran::common::Reference<Fortran::parser::EndSubroutineStmt const>, Fortran::com
mon::Reference<Fortran::parser::EndMpSubprogramStmt const>, Fortran::common::Reference<Fortran::parser::AssociateConstruct const>, Fortran::common::Reference<Fortran::parser::BlockConstruct const>, Fortran::common::Reference<Fortran::par
ser::CaseConstruct const>, Fortran::common::Reference<Fortran::parser::ChangeTeamConstruct const>, Fortran::common::Reference<Fortran::parser::CriticalConstruct const>, Fortran::common::Reference<Fortran::parser::DoConstruct const>, Fort
ran::common::Reference<Fortran::parser::IfConstruct const>, Fortran::common::Reference<Fortran::parser::SelectRankConstruct const>, Fortran::common::Reference<Fortran::parser::SelectTypeConstruct const>, Fortran::common::Reference<Fortra
n::parser::WhereConstruct const>, Fortran::common::Reference<Fortran::parser::ForallConstruct const>, Fortran::common::Reference<Fortran::parser::CompilerDirective const>, Fortran::common::Reference<Fortran::parser::OpenACCConstruct cons
t>, Fortran::common::Reference<Fortran::parser::OpenACCRoutineConstruct const>, Fortran::common::Reference<Fortran::parser::OpenACCDeclarativeConstruct const>, Fortran::common::Reference<Fortran::parser::OpenMPConstruct const>, Fortran::
common::Reference<Fortran::parser::OpenMPDeclarativeConstruct const>, Fortran::common::Reference<Fortran::parser::OmpEndLoopDirective const>, Fortran::common::Reference<Fortran::parser::CUFKernelDoConstruct const>> const&>, std::integer_
sequence<unsigned long, 91ul>>::__visit_invoke(Fortran::common::visitors<auto Fortran::lower::pft::ReferenceVariantBase<true, Fortran::parser::AllocateStmt, Fortran::parser::AssignmentStmt, Fortran::parser::BackspaceStmt, Fortran::parser
::CallStmt, Fortran::parser::CloseStmt, Fortran::parser::ContinueStmt, Fortran::parser::CycleStmt, Fortran::parser::DeallocateStmt, Fortran::parser::EndfileStmt, Fortran::parser::EventPostStmt, Fortran::parser::EventWaitStmt, Fortran::pa
rser::ExitStmt, Fortran::parser::FailImageStmt, Fortran::parser::FlushStmt, Fortran::parser::FormTeamStmt, Fortran::parser::GotoStmt, Fortran::parser::IfStmt, Fortran::parser::InquireStmt, Fortran::parser::LockStmt, Fortran::parser::Noti
fyWaitStmt, Fortran::parser::NullifyStmt, Fortran::parser::OpenStmt, Fortran::parser::PointerAssignmentStmt, Fortran::parser::PrintStmt, Fortran::parser::ReadStmt, Fortran::parser::ReturnStmt, Fortran::parser::RewindStmt, Fortran::parser
::StopStmt, Fortran::parser::SyncAllStmt, Fortran::parser::SyncImagesStmt, Fortran::parser::SyncMemoryStmt, Fortran::parser::SyncTeamStmt, Fortran::parser::UnlockStmt, Fortran::parser::WaitStmt, Fortran::parser::WhereStmt, Fortran::parse
r::WriteStmt, Fortran::parser::ComputedGotoStmt, Fortran::parser::ForallStmt, Fortran::parser::ArithmeticIfStmt, Fortran::parser::AssignStmt, Fortran::parser::AssignedGotoStmt, Fortran::parser::PauseStmt, Fortran::parser::EntryStmt, Fort
ran::parser::FormatStmt, Fortran::parser::AssociateStmt, Fortran::parser::EndAssociateStmt, Fortran::parser::BlockStmt, Fortran::parser::EndBlockStmt, Fortran::parser::SelectCaseStmt, Fortran::parser::CaseStmt, Fortran::parser::EndSelect
Stmt, Fortran::parser::ChangeTeamStmt, Fortran::parser::EndChangeTeamStmt, Fortran::parser::CriticalStmt, Fortran::parser::EndCriticalStmt, Fortran::parser::NonLabelDoStmt, Fortran::parser::EndDoStmt, Fortran::parser::IfThenStmt, Fortran
::parser::ElseIfStmt, Fortran::parser::ElseStmt, Fortran::parser::EndIfStmt, Fortran::parser::SelectRankStmt, Fortran::parser::SelectRankCaseStmt, Fortran::parser::SelectTypeStmt, Fortran::parser::TypeGuardStmt, Fortran::parser::WhereCon
structStmt, Fortran::parser::MaskedElsewhereStmt, Fortran::parser::ElsewhereStmt, Fortran::parser::EndWhereStmt, Fortran::parser::ForallConstructStmt, Fortran::parser::EndForallStmt, Fortran::parser::EndProgramStmt, Fortran::parser::EndF
unctionStmt, Fortran::parser::EndSubroutineStmt, Fortran::parser::EndMpSubprogramStmt, Fortran::parser::AssociateConstruct, Fortran::parser::BlockConstruct, Fortran::parser::CaseConstruct, Fortran::parser::ChangeTeamConstruct, Fortran::p
arser::CriticalConstruct, Fortran::parser::DoConstruct, Fortran::parser::IfConstruct, Fortran::parser::SelectRankConstruct, Fortran::parser::SelectTypeConstruct, Fortran::parser::WhereConstruct, Fortran::parser::ForallConstruct, Fortran:
:parser::CompilerDirective, Fortran::parser::OpenACCConstruct, Fortran::parser::OpenACCRoutineConstruct, Fortran::parser::OpenACCDeclarativeConstruct, Fortran::parser::OpenMPConstruct, Fortran::parser::OpenMPDeclarativeConstruct, Fortran
::parser::OmpEndLoopDirective, Fortran::parser::CUFKernelDoConstruct>::visit<(anonymous namespace)::FirConverter::genFIR(Fortran::lower::pft::Evaluation&, bool)::'lambda'(auto const&)>(auto&&) const::'lambda'(auto)>&&, std::variant<Fortr
an::common::Reference<Fortran::parser::AllocateStmt const>, Fortran::common::Reference<Fortran::parser::AssignmentStmt const>, Fortran::common::Reference<Fortran::parser::BackspaceStmt const>, Fortran::common::Reference<Fortran::parser::
CallStmt const>, Fortran::common::Reference<Fortran::parser::CloseStmt const>, Fortran::common::Reference<Fortran::parser::ContinueStmt const>, Fortran::common::Reference<Fortran::parser::CycleStmt const>, Fortran::common::Reference<Fort
ran::parser::DeallocateStmt const>, Fortran::common::Reference<Fortran::parser::EndfileStmt const>, Fortran::common::Reference<Fortran::parser::EventPostStmt const>, Fortran::common::Reference<Fortran::parser::EventWaitStmt const>, Fortr
an::common::Reference<Fortran::parser::ExitStmt const>, Fortran::common::Reference<Fortran::parser::FailImageStmt const>, Fortran::common::Reference<Fortran::parser::FlushStmt const>, Fortran::common::Reference<Fortran::parser::FormTeamS
tmt const>, Fortran::common::Reference<Fortran::parser::GotoStmt const>, Fortran::common::Reference<Fortran::parser::IfStmt const>, Fortran::common::Reference<Fortran::parser::InquireStmt const>, Fortran::common::Reference<Fortran::parse
r::LockStmt const>, Fortran::common::Reference<Fortran::parser::NotifyWaitStmt const>, Fortran::common::Reference<Fortran::parser::NullifyStmt const>, Fortran::common::Reference<Fortran::parser::OpenStmt const>, Fortran::common::Referenc
e<Fortran::parser::PointerAssignmentStmt const>, Fortran::common::Reference<Fortran::parser::PrintStmt const>, Fortran::common::Reference<Fortran::parser::ReadStmt const>, Fortran::common::Reference<Fortran::parser::ReturnStmt const>, Fo
rtran::common::Reference<Fortran::parser::RewindStmt const>, Fortran::common::Reference<Fortran::parser::StopStmt const>, Fortran::common::Reference<Fortran::parser::SyncAllStmt const>, Fortran::common::Reference<Fortran::parser::SyncIma
gesStmt const>, Fortran::common::Reference<Fortran::parser::SyncMemoryStmt const>, Fortran::common::Reference<Fortran::parser::SyncTeamStmt const>, Fortran::common::Reference<Fortran::parser::UnlockStmt const>, Fortran::common::Reference
<Fortran::parser::WaitStmt const>, Fortran::common::Reference<Fortran::parser::WhereStmt const>, Fortran::common::Reference<Fortran::parser::WriteStmt const>, Fortran::common::Reference<Fortran::parser::ComputedGotoStmt const>, Fortran::
common::Reference<Fortran::parser::ForallStmt const>, Fortran::common::Reference<Fortran::parser::ArithmeticIfStmt const>, Fortran::common::Reference<Fortran::parser::AssignStmt const>, Fortran::common::Reference<Fortran::parser::Assigne
dGotoStmt const>, Fortran::common::Reference<Fortran::parser::PauseStmt const>, Fortran::common::Reference<Fortran::parser::EntryStmt const>, Fortran::common::Reference<Fortran::parser::FormatStmt const>, Fortran::common::Reference<Fortr
an::parser::AssociateStmt const>, Fortran::common::Reference<Fortran::parser::EndAssociateStmt const>, Fortran::common::Reference<Fortran::parser::BlockStmt const>, Fortran::common::Reference<Fortran::parser::EndBlockStmt const>, Fortran
::common::Reference<Fortran::parser::SelectCaseStmt const>, Fortran::common::Reference<Fortran::parser::CaseStmt const>, Fortran::common::Reference<Fortran::parser::EndSelectStmt const>, Fortran::common::Reference<Fortran::parser::Change
TeamStmt const>, Fortran::common::Reference<Fortran::parser::EndChangeTeamStmt const>, Fortran::common::Reference<Fortran::parser::CriticalStmt const>, Fortran::common::Reference<Fortran::parser::EndCriticalStmt const>, Fortran::common::
Reference<Fortran::parser::NonLabelDoStmt const>, Fortran::common::Reference<Fortran::parser::EndDoStmt const>, Fortran::common::Reference<Fortran::parser::IfThenStmt const>, Fortran::common::Reference<Fortran::parser::ElseIfStmt const>,
 Fortran::common::Reference<Fortran::parser::ElseStmt const>, Fortran::common::Reference<Fortran::parser::EndIfStmt const>, Fortran::common::Reference<Fortran::parser::SelectRankStmt const>, Fortran::common::Reference<Fortran::parser::Se
lectRankCaseStmt const>, Fortran::common::Reference<Fortran::parser::SelectTypeStmt const>, Fortran::common::Reference<Fortran::parser::TypeGuardStmt const>, Fortran::common::Reference<Fortran::parser::WhereConstructStmt const>, Fortran:
:common::Reference<Fortran::parser::MaskedElsewhereStmt const>, Fortran::common::Reference<Fortran::parser::ElsewhereStmt const>, Fortran::common::Reference<Fortran::parser::EndWhereStmt const>, Fortran::common::Reference<Fortran::parser
::ForallConstructStmt const>, Fortran::common::Reference<Fortran::parser::EndForallStmt const>, Fortran::common::Reference<Fortran::parser::EndProgramStmt const>, Fortran::common::Reference<Fortran::parser::EndFunctionStmt const>, Fortra
n::common::Reference<Fortran::parser::EndSubroutineStmt const>, Fortran::common::Reference<Fortran::parser::EndMpSubprogramStmt const>, Fortran::common::Reference<Fortran::parser::AssociateConstruct const>, Fortran::common::Reference<For
tran::parser::BlockConstruct const>, Fortran::common::Reference<Fortran::parser::CaseConstruct const>, Fortran::common::Reference<Fortran::parser::ChangeTeamConstruct const>, Fortran::common::Reference<Fortran::parser::CriticalConstruct
const>, Fortran::common::Reference<Fortran::parser::DoConstruct const>, Fortran::common::Reference<Fortran::parser::IfConstruct const>, Fortran::common::Reference<Fortran::parser::SelectRankConstruct const>, Fortran::common::Reference<Fo
rtran::parser::SelectTypeConstruct const>, Fortran::common::Reference<Fortran::parser::WhereConstruct const>, Fortran::common::Reference<Fortran::parser::ForallConstruct const>, Fortran::common::Reference<Fortran::parser::CompilerDirecti
ve const>, Fortran::common::Reference<Fortran::parser::OpenACCConstruct const>, Fortran::common::Reference<Fortran::parser::OpenACCRoutineConstruct const>, Fortran::common::Reference<Fortran::parser::OpenACCDeclarativeConstruct const>, F
ortran::common::Reference<Fortran::parser::OpenMPConstruct const>, Fortran::common::Reference<Fortran::parser::OpenMPDeclarativeConstruct const>, Fortran::common::Reference<Fortran::parser::OmpEndLoopDirective const>, Fortran::common::Re
ference<Fortran::parser::CUFKernelDoConstruct const>> const&) Bridge.cpp:0:0
 #9 0x00000000063716e0 (anonymous namespace)::FirConverter::lowerFunc(Fortran::lower::pft::FunctionLikeUnit&) Bridge.cpp:0:0
#10 0x000000000637350c Fortran::lower::LoweringBridge::lower(Fortran::parser::Program const&, Fortran::semantics::SemanticsContext const&) (/work/groups/ssoft/compiler/llvm/aarch64/main-20260105-a73de982a95b/bin/flang+0x637350c)
#11 0x0000000006227af0 Fortran::frontend::CodeGenAction::beginSourceFileAction() (/work/groups/ssoft/compiler/llvm/aarch64/main-20260105-a73de982a95b/bin/flang+0x6227af0)
#12 0x0000000006216104 Fortran::frontend::FrontendAction::beginSourceFile(Fortran::frontend::CompilerInstance&, Fortran::frontend::FrontendInputFile const&) (/work/groups/ssoft/compiler/llvm/aarch64/main-20260105-a73de982a95b/bin/flang+0
x6216104)
#13 0x00000000062028f4 Fortran::frontend::CompilerInstance::executeAction(Fortran::frontend::FrontendAction&) (/work/groups/ssoft/compiler/llvm/aarch64/main-20260105-a73de982a95b/bin/flang+0x62028f4)
#14 0x000000000621b318 Fortran::frontend::executeCompilerInvocation(Fortran::frontend::CompilerInstance*) (/work/groups/ssoft/compiler/llvm/aarch64/main-20260105-a73de982a95b/bin/flang+0x621b318)
#15 0x00000000061fb0c0 fc1_main(llvm::ArrayRef<char const*>, char const*) (/work/groups/ssoft/compiler/llvm/aarch64/main-20260105-a73de982a95b/bin/flang+0x61fb0c0)
#16 0x00000000061faab8 main (/work/groups/ssoft/compiler/llvm/aarch64/main-20260105-a73de982a95b/bin/flang+0x61faab8)
#17 0x000040003b094404 __libc_start_main (/lib64/libc.so.6+0x24404)
#18 0x00000000061f8f3c _start (/work/groups/ssoft/compiler/llvm/aarch64/main-20260105-a73de982a95b/bin/flang+0x61f8f3c)
flang-22: error: unable to execute command: Aborted (core dumped)
flang-22: error: flang frontend command failed due to signal (use -v to see invocation)
flang version 22.0.0git (https://github.com/llvm/llvm-project.git a73de982a95b7bcb3a918987743136bd38c76e7a)
Target: aarch64-unknown-linux-gnu
Thread model: posix
InstalledDir: /work/groups/ssoft/compiler/llvm/aarch64/main-20260105-a73de982a95b/bin
Build config: +assertions
flang-22: note: diagnostic msg:
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
flang-22: note: diagnostic msg: /tmp/masa_udr0009_2-52b5c4
flang-22: note: diagnostic msg: /tmp/masa_udr0009_2-52b5c4.sh
flang-22: note: diagnostic msg:

********************
$
```

```
$ cat /tmp/masa_udr0009_2-52b5c4
#line "./masa_udr0009_2.f90" 1
      program main
      type t
      integer::i=0
      end type t
!$omp declare reduction (+:t:omp_out%i=omp_out%i+omp_in%i) initializer(o&
!$omp&mp_priv%i=0)
      integer::i
      type(t)::x1
      x1%i=0
!$omp parallel do reduction(+:x1)
      do i=1,10
      x1%i=x1%i+1
      end do
!$omp end parallel do
      print *,'pass'
      end program main
$
```

```
$ cat /tmp/masa_udr0009_2-52b5c4.sh
# Crash reproducer for clang version 22.0.0git (https://github.com/llvm/llvm-project.git a73de982a95b7bcb3a918987743136bd38c76e7a)
# Driver args: "-fopenmp" "masa_udr0009_2.f90"
# Original command:  "/work/groups/ssoft/compiler/llvm/aarch64/main-20260105-a73de982a95b/bin/flang" "-fc1" "-triple" "aarch64-unknown-linux-gnu" "-emit-obj" "-mrelocation-model" "pic" "-pic-level" "2" "-pic-is-pie" "-target-cpu" "generi
c" "-target-feature" "+v8a" "-target-feature" "+fp-armv8" "-target-feature" "+neon" "-fopenmp" "-resource-dir" "/work/groups/ssoft/compiler/llvm/aarch64/main-20260105-a73de982a95b/lib/clang/22" "-mframe-pointer=non-leaf-no-reserve" "-o"
"/tmp/masa_udr0009_2-ceec8b.o" "-x" "f95" "masa_udr0009_2.f90"
 "/work/groups/ssoft/compiler/llvm/aarch64/main-20260105-a73de982a95b/bin/flang" "-fc1" "-triple" "aarch64-unknown-linux-gnu" "-emit-obj" "-mrelocation-model" "pic" "-pic-level" "2" "-pic-is-pie" "-target-cpu" "generic" "-target-feature"
 "+v8a" "-target-feature" "+fp-armv8" "-target-feature" "+neon" "-fopenmp" "-mframe-pointer=non-leaf-no-reserve" "-x" "f95" "masa_udr0009_2-52b5c4"
$
```

```
$ export OMP_NUM_THREADS=2; gfortran -fopenmp masa_udr0009_2.f90; ./a.out
 pass
$
```

```
$ export OMP_NUM_THREADS=2; ifx -qopenmp masa_udr0009_2.f90; ./a.out
 pass
$
```



## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-flang-ir

Author: ohno-fj (ohno-fj)

<details>
```
Version of flang : 22.0.0(2f7e218017db69454ea84e1849fcb9a11b1f7fef)/AArch64
```

In the attached program (`masa_udr0009_2.f90`), specifying `derived type name` in `type-list` of `declare reduction directive` causes the compilation to terminate abnormally.
After checking `OpenMP 6.0 : 7.6.14 declare_reduction Directive`, the program appears to contain no particular errors.

The following are the test program, Flang, Gfortran and ifx compilation/execution results.

masa_udr0009_2.f90:
```fortran
program main
  type t
     integer::i=0
  end type t
!$omp declare reduction (+:t:omp_out%i=omp_out%i+omp_in%i) &amp;
!$omp         initializer(omp_priv%i=0)
  integer::i
  type(t)::x1
  x1%i=0
!$omp parallel do reduction(+:x1)
  do i=1,10
     x1%i=x1%i+1
  end do
!$omp end parallel do
  print *,'pass'
end program main
```

```
$ flang -fopenmp masa_udr0009_2.f90
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: /work/groups/ssoft/compiler/llvm/aarch64/main-20260105-a73de982a95b/bin/flang -fc1 -triple aarch64-unknown-linux-gnu -emit-obj -mrelocation-model pic -pic-level 2 -pic-is-pie -target-cpu generic -target-feature
 +v8a -target-feature +fp-armv8 -target-feature +neon -fopenmp -resource-dir /work/groups/ssoft/compiler/llvm/aarch64/main-20260105-a73de982a95b/lib/clang/22 -mframe-pointer=non-leaf-no-reserve -o /tmp/masa_udr0009_2-ceec8b.o -x f95 masa
_udr0009_2.f90
 #<!-- -->0 0x0000400035f39e80 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/work/groups/ssoft/compiler/llvm/aarch64/main-latest/lib/libLLVM.so.22.0git+0x6419e80)
 #<!-- -->1 0x0000400035f37428 llvm::sys::RunSignalHandlers() (/work/groups/ssoft/compiler/llvm/aarch64/main-latest/lib/libLLVM.so.22.0git+0x6417428)
 #<!-- -->2 0x0000400035f37580 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x00004000237607a0 (linux-vdso.so.1+0x7a0)
 #<!-- -->4 0x000040003b0a62e4 raise (/lib64/libc.so.6+0x362e4)
 #<!-- -->5 0x000040003b090aac abort (/lib64/libc.so.6+0x20aac)
 #<!-- -->6 0x00000000066dff20 genOMP(Fortran::lower::AbstractConverter&amp;, Fortran::lower::SymMap&amp;, Fortran::semantics::SemanticsContext&amp;, Fortran::lower::pft::Evaluation&amp;, Fortran::parser::OpenMPDeclareReductionConstruct const&amp;) (.isra.22387) Op
enMP.cpp:0:0
 #<!-- -->7 0x00000000066cabe8 Fortran::lower::genOpenMPDeclarativeConstruct(Fortran::lower::AbstractConverter&amp;, Fortran::lower::SymMap&amp;, Fortran::semantics::SemanticsContext&amp;, Fortran::lower::pft::Evaluation&amp;, Fortran::parser::OpenMPDeclarative
Construct const&amp;) (/work/groups/ssoft/compiler/llvm/aarch64/main-20260105-a73de982a95b/bin/flang+0x66cabe8)
 #<!-- -->8 0x000000000632bd44 std::__detail::__variant::__gen_vtable_impl&lt;std::__detail::__variant::_Multi_array&lt;void (*)(Fortran::common::visitors&lt;auto Fortran::lower::pft::ReferenceVariantBase&lt;true, Fortran::parser::AllocateStmt, Fortran::par
ser::AssignmentStmt, Fortran::parser::BackspaceStmt, Fortran::parser::CallStmt, Fortran::parser::CloseStmt, Fortran::parser::ContinueStmt, Fortran::parser::CycleStmt, Fortran::parser::DeallocateStmt, Fortran::parser::EndfileStmt, Fortran
::parser::EventPostStmt, Fortran::parser::EventWaitStmt, Fortran::parser::ExitStmt, Fortran::parser::FailImageStmt, Fortran::parser::FlushStmt, Fortran::parser::FormTeamStmt, Fortran::parser::GotoStmt, Fortran::parser::IfStmt, Fortran::p
arser::InquireStmt, Fortran::parser::LockStmt, Fortran::parser::NotifyWaitStmt, Fortran::parser::NullifyStmt, Fortran::parser::OpenStmt, Fortran::parser::PointerAssignmentStmt, Fortran::parser::PrintStmt, Fortran::parser::ReadStmt, Fortr
an::parser::ReturnStmt, Fortran::parser::RewindStmt, Fortran::parser::StopStmt, Fortran::parser::SyncAllStmt, Fortran::parser::SyncImagesStmt, Fortran::parser::SyncMemoryStmt, Fortran::parser::SyncTeamStmt, Fortran::parser::UnlockStmt, F
ortran::parser::WaitStmt, Fortran::parser::WhereStmt, Fortran::parser::WriteStmt, Fortran::parser::ComputedGotoStmt, Fortran::parser::ForallStmt, Fortran::parser::ArithmeticIfStmt, Fortran::parser::AssignStmt, Fortran::parser::AssignedGo
toStmt, Fortran::parser::PauseStmt, Fortran::parser::EntryStmt, Fortran::parser::FormatStmt, Fortran::parser::AssociateStmt, Fortran::parser::EndAssociateStmt, Fortran::parser::BlockStmt, Fortran::parser::EndBlockStmt, Fortran::parser::S
electCaseStmt, Fortran::parser::CaseStmt, Fortran::parser::EndSelectStmt, Fortran::parser::ChangeTeamStmt, Fortran::parser::EndChangeTeamStmt, Fortran::parser::CriticalStmt, Fortran::parser::EndCriticalStmt, Fortran::parser::NonLabelDoSt
mt, Fortran::parser::EndDoStmt, Fortran::parser::IfThenStmt, Fortran::parser::ElseIfStmt, Fortran::parser::ElseStmt, Fortran::parser::EndIfStmt, Fortran::parser::SelectRankStmt, Fortran::parser::SelectRankCaseStmt, Fortran::parser::Selec
tTypeStmt, Fortran::parser::TypeGuardStmt, Fortran::parser::WhereConstructStmt, Fortran::parser::MaskedElsewhereStmt, Fortran::parser::ElsewhereStmt, Fortran::parser::EndWhereStmt, Fortran::parser::ForallConstructStmt, Fortran::parser::E
ndForallStmt, Fortran::parser::EndProgramStmt, Fortran::parser::EndFunctionStmt, Fortran::parser::EndSubroutineStmt, Fortran::parser::EndMpSubprogramStmt, Fortran::parser::AssociateConstruct, Fortran::parser::BlockConstruct, Fortran::par
ser::CaseConstruct, Fortran::parser::ChangeTeamConstruct, Fortran::parser::CriticalConstruct, Fortran::parser::DoConstruct, Fortran::parser::IfConstruct, Fortran::parser::SelectRankConstruct, Fortran::parser::SelectTypeConstruct, Fortran
::parser::WhereConstruct, Fortran::parser::ForallConstruct, Fortran::parser::CompilerDirective, Fortran::parser::OpenACCConstruct, Fortran::parser::OpenACCRoutineConstruct, Fortran::parser::OpenACCDeclarativeConstruct, Fortran::parser::O
penMPConstruct, Fortran::parser::OpenMPDeclarativeConstruct, Fortran::parser::OmpEndLoopDirective, Fortran::parser::CUFKernelDoConstruct&gt;::visit&lt;(anonymous namespace)::FirConverter::genFIR(Fortran::lower::pft::Evaluation&amp;, bool)::'lambda
'(auto const&amp;)&gt;(auto&amp;&amp;) const::'lambda'(auto)&gt;&amp;&amp;, std::variant&lt;Fortran::common::Reference&lt;Fortran::parser::AllocateStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::AssignmentStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser
::BackspaceStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::CallStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::CloseStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::ContinueStmt const&gt;, Fortran::common::Referenc
e&lt;Fortran::parser::CycleStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::DeallocateStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::EndfileStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::EventPostStmt const&gt;, For
tran::common::Reference&lt;Fortran::parser::EventWaitStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::ExitStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::FailImageStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::Flu
shStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::FormTeamStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::GotoStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::IfStmt const&gt;, Fortran::common::Reference&lt;Fortran::p
arser::InquireStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::LockStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::NotifyWaitStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::NullifyStmt const&gt;, Fortran::common::R
eference&lt;Fortran::parser::OpenStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::PointerAssignmentStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::PrintStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::ReadStmt const
&gt;, Fortran::common::Reference&lt;Fortran::parser::ReturnStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::RewindStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::StopStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::Syn
cAllStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::SyncImagesStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::SyncMemoryStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::SyncTeamStmt const&gt;, Fortran::common::Refe
rence&lt;Fortran::parser::UnlockStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::WaitStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::WhereStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::WriteStmt const&gt;, Fortran::c
ommon::Reference&lt;Fortran::parser::ComputedGotoStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::ForallStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::ArithmeticIfStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::As
signStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::AssignedGotoStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::PauseStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::EntryStmt const&gt;, Fortran::common::Reference&lt;
Fortran::parser::FormatStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::AssociateStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::EndAssociateStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::BlockStmt const&gt;, Fort
ran::common::Reference&lt;Fortran::parser::EndBlockStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::SelectCaseStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::CaseStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::EndS
electStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::ChangeTeamStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::EndChangeTeamStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::CriticalStmt const&gt;, Fortran::common::
Reference&lt;Fortran::parser::EndCriticalStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::NonLabelDoStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::EndDoStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::IfThenStmt co
nst&gt;, Fortran::common::Reference&lt;Fortran::parser::ElseIfStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::ElseStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::EndIfStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::S
electRankStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::SelectRankCaseStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::SelectTypeStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::TypeGuardStmt const&gt;, Fortran::co
mmon::Reference&lt;Fortran::parser::WhereConstructStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::MaskedElsewhereStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::ElsewhereStmt const&gt;, Fortran::common::Reference&lt;Fortran::par
ser::EndWhereStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::ForallConstructStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::EndForallStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::EndProgramStmt const&gt;, Fortra
n::common::Reference&lt;Fortran::parser::EndFunctionStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::EndSubroutineStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::EndMpSubprogramStmt const&gt;, Fortran::common::Reference&lt;Fortra
n::parser::AssociateConstruct const&gt;, Fortran::common::Reference&lt;Fortran::parser::BlockConstruct const&gt;, Fortran::common::Reference&lt;Fortran::parser::CaseConstruct const&gt;, Fortran::common::Reference&lt;Fortran::parser::ChangeTeamConstruct co
nst&gt;, Fortran::common::Reference&lt;Fortran::parser::CriticalConstruct const&gt;, Fortran::common::Reference&lt;Fortran::parser::DoConstruct const&gt;, Fortran::common::Reference&lt;Fortran::parser::IfConstruct const&gt;, Fortran::common::Reference&lt;Fortra
n::parser::SelectRankConstruct const&gt;, Fortran::common::Reference&lt;Fortran::parser::SelectTypeConstruct const&gt;, Fortran::common::Reference&lt;Fortran::parser::WhereConstruct const&gt;, Fortran::common::Reference&lt;Fortran::parser::ForallConstruct
 const&gt;, Fortran::common::Reference&lt;Fortran::parser::CompilerDirective const&gt;, Fortran::common::Reference&lt;Fortran::parser::OpenACCConstruct const&gt;, Fortran::common::Reference&lt;Fortran::parser::OpenACCRoutineConstruct const&gt;, Fortran::comm
on::Reference&lt;Fortran::parser::OpenACCDeclarativeConstruct const&gt;, Fortran::common::Reference&lt;Fortran::parser::OpenMPConstruct const&gt;, Fortran::common::Reference&lt;Fortran::parser::OpenMPDeclarativeConstruct const&gt;, Fortran::common::Refere
nce&lt;Fortran::parser::OmpEndLoopDirective const&gt;, Fortran::common::Reference&lt;Fortran::parser::CUFKernelDoConstruct const&gt;&gt; const&amp;)&gt;, std::tuple&lt;std::variant&lt;Fortran::common::Reference&lt;Fortran::parser::AllocateStmt const&gt;, Fortran::common:
:Reference&lt;Fortran::parser::AssignmentStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::BackspaceStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::CallStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::CloseStmt const
&gt;, Fortran::common::Reference&lt;Fortran::parser::ContinueStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::CycleStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::DeallocateStmt const&gt;, Fortran::common::Reference&lt;Fortran::pars
er::EndfileStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::EventPostStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::EventWaitStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::ExitStmt const&gt;, Fortran::common::Ref
erence&lt;Fortran::parser::FailImageStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::FlushStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::FormTeamStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::GotoStmt const&gt;, For
tran::common::Reference&lt;Fortran::parser::IfStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::InquireStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::LockStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::NotifyWaitSt
mt const&gt;, Fortran::common::Reference&lt;Fortran::parser::NullifyStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::OpenStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::PointerAssignmentStmt const&gt;, Fortran::common::Reference&lt;
Fortran::parser::PrintStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::ReadStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::ReturnStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::RewindStmt const&gt;, Fortran::common
::Reference&lt;Fortran::parser::StopStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::SyncAllStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::SyncImagesStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::SyncMemoryStmt c
onst&gt;, Fortran::common::Reference&lt;Fortran::parser::SyncTeamStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::UnlockStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::WaitStmt const&gt;, Fortran::common::Reference&lt;Fortran::parse
r::WhereStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::WriteStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::ComputedGotoStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::ForallStmt const&gt;, Fortran::common::Refer
ence&lt;Fortran::parser::ArithmeticIfStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::AssignStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::AssignedGotoStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::PauseStmt cons
t&gt;, Fortran::common::Reference&lt;Fortran::parser::EntryStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::FormatStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::AssociateStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser
::EndAssociateStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::BlockStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::EndBlockStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::SelectCaseStmt const&gt;, Fortran::common:
:Reference&lt;Fortran::parser::CaseStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::EndSelectStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::ChangeTeamStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::EndChangeTeamSt
mt const&gt;, Fortran::common::Reference&lt;Fortran::parser::CriticalStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::EndCriticalStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::NonLabelDoStmt const&gt;, Fortran::common::Reference
&lt;Fortran::parser::EndDoStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::IfThenStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::ElseIfStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::ElseStmt const&gt;, Fortran::commo
n::Reference&lt;Fortran::parser::EndIfStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::SelectRankStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::SelectRankCaseStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::SelectT
ypeStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::TypeGuardStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::WhereConstructStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::MaskedElsewhereStmt const&gt;, Fortran::com
mon::Reference&lt;Fortran::parser::ElsewhereStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::EndWhereStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::ForallConstructStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::En
dForallStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::EndProgramStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::EndFunctionStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::EndSubroutineStmt const&gt;, Fortran::com
mon::Reference&lt;Fortran::parser::EndMpSubprogramStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::AssociateConstruct const&gt;, Fortran::common::Reference&lt;Fortran::parser::BlockConstruct const&gt;, Fortran::common::Reference&lt;Fortran::par
ser::CaseConstruct const&gt;, Fortran::common::Reference&lt;Fortran::parser::ChangeTeamConstruct const&gt;, Fortran::common::Reference&lt;Fortran::parser::CriticalConstruct const&gt;, Fortran::common::Reference&lt;Fortran::parser::DoConstruct const&gt;, Fort
ran::common::Reference&lt;Fortran::parser::IfConstruct const&gt;, Fortran::common::Reference&lt;Fortran::parser::SelectRankConstruct const&gt;, Fortran::common::Reference&lt;Fortran::parser::SelectTypeConstruct const&gt;, Fortran::common::Reference&lt;Fortra
n::parser::WhereConstruct const&gt;, Fortran::common::Reference&lt;Fortran::parser::ForallConstruct const&gt;, Fortran::common::Reference&lt;Fortran::parser::CompilerDirective const&gt;, Fortran::common::Reference&lt;Fortran::parser::OpenACCConstruct cons
t&gt;, Fortran::common::Reference&lt;Fortran::parser::OpenACCRoutineConstruct const&gt;, Fortran::common::Reference&lt;Fortran::parser::OpenACCDeclarativeConstruct const&gt;, Fortran::common::Reference&lt;Fortran::parser::OpenMPConstruct const&gt;, Fortran::
common::Reference&lt;Fortran::parser::OpenMPDeclarativeConstruct const&gt;, Fortran::common::Reference&lt;Fortran::parser::OmpEndLoopDirective const&gt;, Fortran::common::Reference&lt;Fortran::parser::CUFKernelDoConstruct const&gt;&gt; const&amp;&gt;, std::integer_
sequence&lt;unsigned long, 91ul&gt;&gt;::__visit_invoke(Fortran::common::visitors&lt;auto Fortran::lower::pft::ReferenceVariantBase&lt;true, Fortran::parser::AllocateStmt, Fortran::parser::AssignmentStmt, Fortran::parser::BackspaceStmt, Fortran::parser
::CallStmt, Fortran::parser::CloseStmt, Fortran::parser::ContinueStmt, Fortran::parser::CycleStmt, Fortran::parser::DeallocateStmt, Fortran::parser::EndfileStmt, Fortran::parser::EventPostStmt, Fortran::parser::EventWaitStmt, Fortran::pa
rser::ExitStmt, Fortran::parser::FailImageStmt, Fortran::parser::FlushStmt, Fortran::parser::FormTeamStmt, Fortran::parser::GotoStmt, Fortran::parser::IfStmt, Fortran::parser::InquireStmt, Fortran::parser::LockStmt, Fortran::parser::Noti
fyWaitStmt, Fortran::parser::NullifyStmt, Fortran::parser::OpenStmt, Fortran::parser::PointerAssignmentStmt, Fortran::parser::PrintStmt, Fortran::parser::ReadStmt, Fortran::parser::ReturnStmt, Fortran::parser::RewindStmt, Fortran::parser
::StopStmt, Fortran::parser::SyncAllStmt, Fortran::parser::SyncImagesStmt, Fortran::parser::SyncMemoryStmt, Fortran::parser::SyncTeamStmt, Fortran::parser::UnlockStmt, Fortran::parser::WaitStmt, Fortran::parser::WhereStmt, Fortran::parse
r::WriteStmt, Fortran::parser::ComputedGotoStmt, Fortran::parser::ForallStmt, Fortran::parser::ArithmeticIfStmt, Fortran::parser::AssignStmt, Fortran::parser::AssignedGotoStmt, Fortran::parser::PauseStmt, Fortran::parser::EntryStmt, Fort
ran::parser::FormatStmt, Fortran::parser::AssociateStmt, Fortran::parser::EndAssociateStmt, Fortran::parser::BlockStmt, Fortran::parser::EndBlockStmt, Fortran::parser::SelectCaseStmt, Fortran::parser::CaseStmt, Fortran::parser::EndSelect
Stmt, Fortran::parser::ChangeTeamStmt, Fortran::parser::EndChangeTeamStmt, Fortran::parser::CriticalStmt, Fortran::parser::EndCriticalStmt, Fortran::parser::NonLabelDoStmt, Fortran::parser::EndDoStmt, Fortran::parser::IfThenStmt, Fortran
::parser::ElseIfStmt, Fortran::parser::ElseStmt, Fortran::parser::EndIfStmt, Fortran::parser::SelectRankStmt, Fortran::parser::SelectRankCaseStmt, Fortran::parser::SelectTypeStmt, Fortran::parser::TypeGuardStmt, Fortran::parser::WhereCon
structStmt, Fortran::parser::MaskedElsewhereStmt, Fortran::parser::ElsewhereStmt, Fortran::parser::EndWhereStmt, Fortran::parser::ForallConstructStmt, Fortran::parser::EndForallStmt, Fortran::parser::EndProgramStmt, Fortran::parser::EndF
unctionStmt, Fortran::parser::EndSubroutineStmt, Fortran::parser::EndMpSubprogramStmt, Fortran::parser::AssociateConstruct, Fortran::parser::BlockConstruct, Fortran::parser::CaseConstruct, Fortran::parser::ChangeTeamConstruct, Fortran::p
arser::CriticalConstruct, Fortran::parser::DoConstruct, Fortran::parser::IfConstruct, Fortran::parser::SelectRankConstruct, Fortran::parser::SelectTypeConstruct, Fortran::parser::WhereConstruct, Fortran::parser::ForallConstruct, Fortran:
:parser::CompilerDirective, Fortran::parser::OpenACCConstruct, Fortran::parser::OpenACCRoutineConstruct, Fortran::parser::OpenACCDeclarativeConstruct, Fortran::parser::OpenMPConstruct, Fortran::parser::OpenMPDeclarativeConstruct, Fortran
::parser::OmpEndLoopDirective, Fortran::parser::CUFKernelDoConstruct&gt;::visit&lt;(anonymous namespace)::FirConverter::genFIR(Fortran::lower::pft::Evaluation&amp;, bool)::'lambda'(auto const&amp;)&gt;(auto&amp;&amp;) const::'lambda'(auto)&gt;&amp;&amp;, std::variant&lt;Fortr
an::common::Reference&lt;Fortran::parser::AllocateStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::AssignmentStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::BackspaceStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::
CallStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::CloseStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::ContinueStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::CycleStmt const&gt;, Fortran::common::Reference&lt;Fort
ran::parser::DeallocateStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::EndfileStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::EventPostStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::EventWaitStmt const&gt;, Fortr
an::common::Reference&lt;Fortran::parser::ExitStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::FailImageStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::FlushStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::FormTeamS
tmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::GotoStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::IfStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::InquireStmt const&gt;, Fortran::common::Reference&lt;Fortran::parse
r::LockStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::NotifyWaitStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::NullifyStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::OpenStmt const&gt;, Fortran::common::Referenc
e&lt;Fortran::parser::PointerAssignmentStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::PrintStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::ReadStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::ReturnStmt const&gt;, Fo
rtran::common::Reference&lt;Fortran::parser::RewindStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::StopStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::SyncAllStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::SyncIma
gesStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::SyncMemoryStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::SyncTeamStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::UnlockStmt const&gt;, Fortran::common::Reference
&lt;Fortran::parser::WaitStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::WhereStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::WriteStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::ComputedGotoStmt const&gt;, Fortran::
common::Reference&lt;Fortran::parser::ForallStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::ArithmeticIfStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::AssignStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::Assigne
dGotoStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::PauseStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::EntryStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::FormatStmt const&gt;, Fortran::common::Reference&lt;Fortr
an::parser::AssociateStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::EndAssociateStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::BlockStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::EndBlockStmt const&gt;, Fortran
::common::Reference&lt;Fortran::parser::SelectCaseStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::CaseStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::EndSelectStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::Change
TeamStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::EndChangeTeamStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::CriticalStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::EndCriticalStmt const&gt;, Fortran::common::
Reference&lt;Fortran::parser::NonLabelDoStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::EndDoStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::IfThenStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::ElseIfStmt const&gt;,
 Fortran::common::Reference&lt;Fortran::parser::ElseStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::EndIfStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::SelectRankStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::Se
lectRankCaseStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::SelectTypeStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::TypeGuardStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::WhereConstructStmt const&gt;, Fortran:
:common::Reference&lt;Fortran::parser::MaskedElsewhereStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::ElsewhereStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::EndWhereStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser
::ForallConstructStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::EndForallStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::EndProgramStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::EndFunctionStmt const&gt;, Fortra
n::common::Reference&lt;Fortran::parser::EndSubroutineStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::EndMpSubprogramStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::AssociateConstruct const&gt;, Fortran::common::Reference&lt;For
tran::parser::BlockConstruct const&gt;, Fortran::common::Reference&lt;Fortran::parser::CaseConstruct const&gt;, Fortran::common::Reference&lt;Fortran::parser::ChangeTeamConstruct const&gt;, Fortran::common::Reference&lt;Fortran::parser::CriticalConstruct
const&gt;, Fortran::common::Reference&lt;Fortran::parser::DoConstruct const&gt;, Fortran::common::Reference&lt;Fortran::parser::IfConstruct const&gt;, Fortran::common::Reference&lt;Fortran::parser::SelectRankConstruct const&gt;, Fortran::common::Reference&lt;Fo
rtran::parser::SelectTypeConstruct const&gt;, Fortran::common::Reference&lt;Fortran::parser::WhereConstruct const&gt;, Fortran::common::Reference&lt;Fortran::parser::ForallConstruct const&gt;, Fortran::common::Reference&lt;Fortran::parser::CompilerDirecti
ve const&gt;, Fortran::common::Reference&lt;Fortran::parser::OpenACCConstruct const&gt;, Fortran::common::Reference&lt;Fortran::parser::OpenACCRoutineConstruct const&gt;, Fortran::common::Reference&lt;Fortran::parser::OpenACCDeclarativeConstruct const&gt;, F
ortran::common::Reference&lt;Fortran::parser::OpenMPConstruct const&gt;, Fortran::common::Reference&lt;Fortran::parser::OpenMPDeclarativeConstruct const&gt;, Fortran::common::Reference&lt;Fortran::parser::OmpEndLoopDirective const&gt;, Fortran::common::Re
ference&lt;Fortran::parser::CUFKernelDoConstruct const&gt;&gt; const&amp;) Bridge.cpp:0:0
 #<!-- -->9 0x00000000063716e0 (anonymous namespace)::FirConverter::lowerFunc(Fortran::lower::pft::FunctionLikeUnit&amp;) Bridge.cpp:0:0
#<!-- -->10 0x000000000637350c Fortran::lower::LoweringBridge::lower(Fortran::parser::Program const&amp;, Fortran::semantics::SemanticsContext const&amp;) (/work/groups/ssoft/compiler/llvm/aarch64/main-20260105-a73de982a95b/bin/flang+0x637350c)
#<!-- -->11 0x0000000006227af0 Fortran::frontend::CodeGenAction::beginSourceFileAction() (/work/groups/ssoft/compiler/llvm/aarch64/main-20260105-a73de982a95b/bin/flang+0x6227af0)
#<!-- -->12 0x0000000006216104 Fortran::frontend::FrontendAction::beginSourceFile(Fortran::frontend::CompilerInstance&amp;, Fortran::frontend::FrontendInputFile const&amp;) (/work/groups/ssoft/compiler/llvm/aarch64/main-20260105-a73de982a95b/bin/flang+0
x6216104)
#<!-- -->13 0x00000000062028f4 Fortran::frontend::CompilerInstance::executeAction(Fortran::frontend::FrontendAction&amp;) (/work/groups/ssoft/compiler/llvm/aarch64/main-20260105-a73de982a95b/bin/flang+0x62028f4)
#<!-- -->14 0x000000000621b318 Fortran::frontend::executeCompilerInvocation(Fortran::frontend::CompilerInstance*) (/work/groups/ssoft/compiler/llvm/aarch64/main-20260105-a73de982a95b/bin/flang+0x621b318)
#<!-- -->15 0x00000000061fb0c0 fc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*) (/work/groups/ssoft/compiler/llvm/aarch64/main-20260105-a73de982a95b/bin/flang+0x61fb0c0)
#<!-- -->16 0x00000000061faab8 main (/work/groups/ssoft/compiler/llvm/aarch64/main-20260105-a73de982a95b/bin/flang+0x61faab8)
#<!-- -->17 0x000040003b094404 __libc_start_main (/lib64/libc.so.6+0x24404)
#<!-- -->18 0x00000000061f8f3c _start (/work/groups/ssoft/compiler/llvm/aarch64/main-20260105-a73de982a95b/bin/flang+0x61f8f3c)
flang-22: error: unable to execute command: Aborted (core dumped)
flang-22: error: flang frontend command failed due to signal (use -v to see invocation)
flang version 22.0.0git (https://github.com/llvm/llvm-project.git a73de982a95b7bcb3a918987743136bd38c76e7a)
Target: aarch64-unknown-linux-gnu
Thread model: posix
InstalledDir: /work/groups/ssoft/compiler/llvm/aarch64/main-20260105-a73de982a95b/bin
Build config: +assertions
flang-22: note: diagnostic msg:
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
flang-22: note: diagnostic msg: /tmp/masa_udr0009_2-52b5c4
flang-22: note: diagnostic msg: /tmp/masa_udr0009_2-52b5c4.sh
flang-22: note: diagnostic msg:

********************
$
```

```
$ cat /tmp/masa_udr0009_2-52b5c4
#line "./masa_udr0009_2.f90" 1
      program main
      type t
      integer::i=0
      end type t
!$omp declare reduction (+:t:omp_out%i=omp_out%i+omp_in%i) initializer(o&amp;
!$omp&amp;mp_priv%i=0)
      integer::i
      type(t)::x1
      x1%i=0
!$omp parallel do reduction(+:x1)
      do i=1,10
      x1%i=x1%i+1
      end do
!$omp end parallel do
      print *,'pass'
      end program main
$
```

```
$ cat /tmp/masa_udr0009_2-52b5c4.sh
# Crash reproducer for clang version 22.0.0git (https://github.com/llvm/llvm-project.git a73de982a95b7bcb3a918987743136bd38c76e7a)
# Driver args: "-fopenmp" "masa_udr0009_2.f90"
# Original command:  "/work/groups/ssoft/compiler/llvm/aarch64/main-20260105-a73de982a95b/bin/flang" "-fc1" "-triple" "aarch64-unknown-linux-gnu" "-emit-obj" "-mrelocation-model" "pic" "-pic-level" "2" "-pic-is-pie" "-target-cpu" "generi
c" "-target-feature" "+v8a" "-target-feature" "+fp-armv8" "-target-feature" "+neon" "-fopenmp" "-resource-dir" "/work/groups/ssoft/compiler/llvm/aarch64/main-20260105-a73de982a95b/lib/clang/22" "-mframe-pointer=non-leaf-no-reserve" "-o"
"/tmp/masa_udr0009_2-ceec8b.o" "-x" "f95" "masa_udr0009_2.f90"
 "/work/groups/ssoft/compiler/llvm/aarch64/main-20260105-a73de982a95b/bin/flang" "-fc1" "-triple" "aarch64-unknown-linux-gnu" "-emit-obj" "-mrelocation-model" "pic" "-pic-level" "2" "-pic-is-pie" "-target-cpu" "generic" "-target-feature"
 "+v8a" "-target-feature" "+fp-armv8" "-target-feature" "+neon" "-fopenmp" "-mframe-pointer=non-leaf-no-reserve" "-x" "f95" "masa_udr0009_2-52b5c4"
$
```

```
$ export OMP_NUM_THREADS=2; gfortran -fopenmp masa_udr0009_2.f90; ./a.out
 pass
$
```

```
$ export OMP_NUM_THREADS=2; ifx -qopenmp masa_udr0009_2.f90; ./a.out
 pass
$
```


</details>


---

### Comment 2 - kparzysz

The problem is at https://github.com/llvm/llvm-project/blob/main/flang/lib/Lower/OpenMP/OpenMP.cpp#L3829.  It expects the identifier to be a procedure designator, but here it is an intrinsic operator.

---

### Comment 3 - MattPD

Could the ICE in this issue be the same problem I'm running into?

My repro: https://flang.godbolt.org/z/osYboboGh - seems similar as it results in an ICE involving `genOpenMPDeclarativeConstruct` when using declare reduction with initializers on a derived type.

An even smaller version: https://flang.godbolt.org/z/EEK4MrM49
```f90
program crash
  type t
     integer :: x
  end type t
  !$omp declare reduction(+:t: omp_out = omp_out) initializer(omp_priv = t(0))
  type(t) :: a
  !$omp parallel reduction(+:a)
  !$omp end parallel
end program crash
```

My ICE:
```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /cefs/5f/5f270e9efb8d313a52164fd1_clang-llvmflang-trunk-20260204/bin/flang -fc1 -triple x86_64-unknown-linux-gnu -emit-obj -mrelocation-model pic -pic-level 2 -pic-is-pie -target-cpu x86-64 -vectorize-loops -vectorize-slp -fversion-loops-for-stride -debug-info-kind=standalone -fopenmp -resource-dir /cefs/5f/5f270e9efb8d313a52164fd1_clang-llvmflang-trunk-20260204/lib/clang/23 -mframe-pointer=none -O3 -o /tmp/example-e1ff76.o -x f95 /app/example.f90
 #0 0x00000000045e75c8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/cefs/5f/5f270e9efb8d313a52164fd1_clang-llvmflang-trunk-20260204/bin/flang+0x45e75c8)
 #1 0x00000000045e47b4 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x0000762c21242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x0000762c212969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x0000762c21242476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x0000762c212287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x00000000026afc17 Fortran::lower::genOpenMPDeclarativeConstruct(Fortran::lower::AbstractConverter&, Fortran::lower::SymMap&, Fortran::semantics::SemanticsContext&, Fortran::lower::pft::Evaluation&, Fortran::parser::OpenMPDeclarativeConstruct const&) (.cold) OpenMP.cpp:0:0
 #7 0x0000000004e120d6 Fortran::lower::genOpenMPDeclarativeConstruct(Fortran::lower::AbstractConverter&, Fortran::lower::SymMap&, Fortran::semantics::SemanticsContext&, Fortran::lower::pft::Evaluation&, Fortran::parser::OpenMPDeclarativeConstruct const&)
#8 0x0000000004a8dd1d void Fortran::common::log2visit::Log2VisitHelper<...>::visit<(anonymous namespace)::FirConverter::genFIR(Fortran::lower::pft::Evaluation&, bool)::'lambda0'(auto const&)>(auto&&) const::'lambda'(auto)>, std::variant<...> const&) Bridge.cpp:0:0
#9 0x0000000004a95139 (anonymous namespace)::FirConverter::lowerFunc(Fortran::lower::pft::FunctionLikeUnit&) Bridge.cpp:0:0
#10 0x0000000004a97234 Fortran::lower::LoweringBridge::lower(Fortran::parser::Program const&, Fortran::semantics::SemanticsContext const&) (/cefs/5f/5f270e9efb8d313a52164fd1_clang-llvmflang-trunk-20260204/bin/flang+0x4a97234)
#11 0x00000000048fd69c Fortran::frontend::CodeGenAction::beginSourceFileAction() (/cefs/5f/5f270e9efb8d313a52164fd1_clang-llvmflang-trunk-20260204/bin/flang+0x48fd69c)
#12 0x000000000464b825 Fortran::frontend::FrontendAction::beginSourceFile(Fortran::frontend::CompilerInstance&, Fortran::frontend::FrontendInputFile const&) (/cefs/5f/5f270e9efb8d313a52164fd1_clang-llvmflang-trunk-20260204/bin/flang+0x464b825)
#13 0x00000000046387bf Fortran::frontend::CompilerInstance::executeAction(Fortran::frontend::FrontendAction&) (/cefs/5f/5f270e9efb8d313a52164fd1_clang-llvmflang-trunk-20260204/bin/flang+0x46387bf)
#14 0x0000000004656c2f Fortran::frontend::executeCompilerInvocation(Fortran::frontend::CompilerInstance*) (/cefs/5f/5f270e9efb8d313a52164fd1_clang-llvmflang-trunk-20260204/bin/flang+0x4656c2f)
#15 0x000000000283e705 fc1_main(llvm::ArrayRef<char const*>, char const*) (/cefs/5f/5f270e9efb8d313a52164fd1_clang-llvmflang-trunk-20260204/bin/flang+0x283e705)
#16 0x00000000026df511 main (/cefs/5f/5f270e9efb8d313a52164fd1_clang-llvmflang-trunk-20260204/bin/flang+0x26df511)
#17 0x0000762c21229d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#18 0x0000762c21229e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#19 0x000000000283d1c5 _start (/cefs/5f/5f270e9efb8d313a52164fd1_clang-llvmflang-trunk-20260204/bin/flang+0x283d1c5)
flang-23: error: unable to execute command: Aborted (core dumped)
flang-23: error: flang frontend command failed due to signal (use -v to see invocation)
flang version 23.0.0git (https://github.com/llvm/llvm-project.git 1ffe78811e4a502f5ac8a57d4931e3e3b80aaf96)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /cefs/5f/5f270e9efb8d313a52164fd1_clang-llvmflang-trunk-20260204/bin
flang-23: note: diagnostic msg: 
********************
```


---

### Comment 4 - MattPD

One problem (there's more, that's the first in line): genOMP (incorrectly) assumes OmpReductionIdentifier is always a ProcedureDesignator.

Reduced repro:
```f90
! reduction_ice.f90
program crash
  type t
     integer :: x
  end type t
  !$omp declare reduction(+:t: omp_out = omp_in) initializer(omp_priv = t(0))
  type(t) :: a
  !$omp parallel reduction(+:a)
  !$omp end parallel
end program crash
```

After we build the compiler (either latest commit or the older releases mentioned in the GitHub issue) we can regenerate ICE:
```sh
$ ${LLVM_BUILD_DIR?}/flangdev/bin/flang -O0 -fopenmp reduction_ice.f90 >out.txt 2>&1 || true

$ awk '
>   /Stack dump:/{p=1}
>   p==1{print}
>   /#20 /{exit}
> ' out.txt

Stack dump:
0.      Program arguments: /path/to/llvm/build-dual/flangdev/bin/flang -fc1 -triple x86_64-unknown-linux-gnu -emit-obj -mrelocation-model pic -pic-level 2 -pic-is-pie -target-cpu x86-64 -fopenmp -resource-dir /path/to/llvm/build-dual/flangdev/lib/clang/23 -mframe-pointer=all -O0 -o /tmp/reduction_ice-02364e.o -x f95 /path/to/f90/reduction_ice.f90
 #0 0x00007fe9bbc6d96b llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /path/to/llvm/src/llvm-project-dual/llvm/lib/Support/Unix/Signals.inc:880:13
 #1 0x00007fe9bbc6df13 PrintStackTraceSignalHandler(void*) /path/to/llvm/src/llvm-project-dual/llvm/lib/Support/Unix/Signals.inc:962:1
 #2 0x00007fe9bbc6b313 llvm::sys::RunSignalHandlers() /path/to/llvm/src/llvm-project-dual/llvm/lib/Support/Signals.cpp:109:18
 #3 0x00007fe9bbc6e777 SignalHandler(int, siginfo_t*, void*) /path/to/llvm/src/llvm-project-dual/llvm/lib/Support/Unix/Signals.inc:448:38
 #4 0x00007fe9bb257900 __restore_rt (/lib64/libc.so.6+0x57900)
 #5 0x00007fe9bb2a8dfc __pthread_kill_implementation (/lib64/libc.so.6+0xa8dfc)
 #6 0x00007fe9bb257842 gsignal (/lib64/libc.so.6+0x57842)
 #7 0x00007fe9bb23f5cf abort (/lib64/libc.so.6+0x3f5cf)
 #8 0x00007fe9bbba58ba llvm::report_fatal_error(llvm::Twine const&, bool) /path/to/llvm/src/llvm-project-dual/llvm/lib/Support/ErrorHandling.cpp:137:5
 #9 0x00007fe9bbba56f9 (/path/to/llvm/build-dual/flangdev/bin/../lib/libLLVMSupport.so.23.0git+0x1a56f9)
#10 0x00007fe9ba117969 appendCombiner(Fortran::parser::OpenMPDeclareReductionConstruct const&, llvm::SmallVector<Fortran::lower::omp::Clause, 0u>&, Fortran::semantics::SemanticsContext&) /path/to/llvm/src/llvm-project-dual/flang/lib/Lower/OpenMP/OpenMP.cpp:3939:3
#11 0x00007fe9ba117969 genOMP(Fortran::lower::AbstractConverter&, Fortran::lower::SymMap&, Fortran::semantics::SemanticsContext&, Fortran::lower::pft::Evaluation&, Fortran::parser::OpenMPDeclareReductionConstruct const&) /path/to/llvm/src/llvm-project-dual/flang/lib/Lower/OpenMP/OpenMP.cpp:3955:7
```

Now, let's focus on the entry #11 in the above stack dump.
To debug this, we can make a simple change in <https://github.com/llvm/llvm-project/blob/main/flang/lib/Lower/OpenMP/OpenMP.cpp>.

We find the function corresponding to the aforementioned entry #11, i.e.:
```cpp
static void genOMP(lower::AbstractConverter &converter, lower::SymMap &symTable,
                   semantics::SemanticsContext &semaCtx,
                   lower::pft::Evaluation &eval,
                   const parser::OpenMPDeclareReductionConstruct &construct)
```

We replace this line:

```c++
  const auto &designator = std::get<parser::ProcedureDesignator>(identifier.u);
```

with these lines (so as to avoid assuming `ProcedureDesignator` at the outset and instead checking):

```c++
  const auto *designator_ptr =
      std::get_if<parser::ProcedureDesignator>(&identifier.u);
  if (!designator_ptr) {
    llvm::errs() << "[OMP] ERROR: identifier.u is not ProcedureDesignator\n";
    llvm::errs().flush();
    llvm::report_fatal_error("unexpected reduction identifier form");
  }
  const auto &designator = *designator_ptr;
```

After rebuilding the compiler and re-running the compilation on the reduced repro, we now have:
```sh
# grep for salient output
grep -F "[OMP]" out.txt

[OMP] ERROR: identifier.u is not ProcedureDesignator
```

Thus, unconditionally doing `std::get<parser::ProcedureDesignator>(identifier.u);` is one of the problems.
I'm going to see if it makes sense to create a simple patch just for this issue alone, although that's not the only one: After (a quick and dirty) workaround (to get the `reductionName` for `+`) we'll get (closer) to the core issue:

```
[OMP] reductionName: +
error: loc("reduction_ice.f90":7:9): flang/lib/Lower/Support/ReductionProcessor.cpp:210: not yet implemented: Reduction of some types is not supported
LLVM ERROR: aborting
```

This corresponds to the following in `ReductionProcessor::getReductionInitValue`, <https://github.com/llvm/llvm-project/blob/2e989bd59596a7e3ea4bdd600ad8af373d99e68d/flang/lib/Lower/Support/ReductionProcessor.cpp#L210>
```
TODO(loc, "Reduction of some types is not supported");
```

Which finally corresponds to the NYI support. That being said, still may be worth to have a minimal patch for the unconditional `ProcedureDesignator` assumption first.

---

### Comment 5 - kparzysz

Yes, those are independent issues.

For the NYI problem, we should be getting the initial value from the initializer region in DeclareReductionOp.  The function with the TODO seems to only apply to predefined types/reduction operators.

---

### Comment 6 - MattPD

All right, opened PR to deal with the first issue: [flang][OpenMP] Fix crash in declare reduction with intrinsic operators, #182978

Edit: Merged; continuing with the next problem.

---

### Comment 7 - MattPD

Should have another patch soon, hopefully later today

---

### Comment 8 - MattPD

Opened PR to fix the next issue: [flang][OpenMP] Support user-defined declare reduction with derived types #184897 - code reviews welcome!

I've also verified it fixes the original problem in this issue:
```
$ ${LLVM_BUILD_DIR?}/flangdev/bin/flang -O3 -fopenmp /ptmp/$USER/f90/masa_udr0009_2.f90
$ ./a.out
 pass
```

It doesn't fully fix my (related) problem, yet, as declare reduction _without_ an initializer clause is not yet supported. One thing at a time, though; I believe it's also going to be easier to review if these are addressed subsequently.


---

