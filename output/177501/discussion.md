# [Flang][OpenMP] Compilation error when a character type is specified in REDUCTION clause corresponding to declare_reduction directive

**Author:** ohno-fj
**URL:** https://github.com/llvm/llvm-project/issues/177501
**Status:** Closed
**Labels:** flang:ir, crash, flang:openmp
**Closed Date:** 2026-02-10T11:31:26Z

## Body

```
Version of flang : 23.0.0(eb773961aac45593ce126ed258d82c3c3abbaa93)/AArch64
```

In the attached program (`masa_udr_deferred_002_21.f90`), when a character type is specified in `REDUCTION clause` corresponding to `declare_reduction directive`, a compilation terminates abnormally.
After checking `OpenMP 6.0 : 7.6.14 declare_reduction Directive`, the program appears to contain no particular errors.

The following are the test program, Flang, Gfortran and ifx compilation/execution results.

masa_udr_deferred_002_21.f90:
```fortran
program main
  character(len=1),allocatable::k1
!$omp declare reduction (a:character(len=1):omp_out=max(omp_out,omp_in)) &
!$omp initializer(omp_priv='1')
!$omp parallel sections reduction(a:k1)
  k1=max(k1,'2')
!$omp end parallel sections
end program main
```

```
$ flang -fopenmp masa_udr_deferred_002_21.f90
flang: /work/groups/ssoft/compiler/llvm/src/llvm-main/flang/include/flang/Optimizer/Builder/HLFIRTools.h:56: hlfir::Entity::Entity(mlir::Value): Assertion `isFortranEntity(value) && "must be a value representing a Fortran value or variab
le like"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: /work/groups/ssoft/compiler/llvm/aarch64/main-20260119-eb773961aac4/bin/flang -fc1 -triple aarch64-unknown-linux-gnu -emit-obj -mrelocation-model pic -pic-level 2 -pic-is-pie -target-cpu generic -target-feature
 +v8a -target-feature +fp-armv8 -target-feature +neon -fopenmp -resource-dir /work/groups/ssoft/compiler/llvm/aarch64/main-20260119-eb773961aac4/lib/clang/23 -mframe-pointer=non-leaf-no-reserve -o /tmp/masa_udr_deferred_002_21-99ed9a.o -
x f95 masa_udr_deferred_002_21.f90
 #0 0x000040001a8f2360 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/work/groups/ssoft/compiler/llvm/aarch64/main-latest/lib/libLLVM.so.23.0git+0x6312360)
 #1 0x000040001a8ef930 llvm::sys::RunSignalHandlers() (/work/groups/ssoft/compiler/llvm/aarch64/main-latest/lib/libLLVM.so.23.0git+0x630f930)
 #2 0x000040001a8efa88 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x00004000081607a0 (linux-vdso.so.1+0x7a0)
 #4 0x000040001fa562e4 raise (/lib64/libc.so.6+0x362e4)
 #5 0x000040001fa40aac abort (/lib64/libc.so.6+0x20aac)
 #6 0x000040001fa4fc24 __assert_fail_base (/lib64/libc.so.6+0x2fc24)
 #7 0x000040001fa4fc98 __assert_perror_fail (/lib64/libc.so.6+0x2fc98)
 #8 0x00000000067c414c (anonymous namespace)::PopulateInitAndCleanupRegionsHelper::populateByRefInitAndCleanupRegions() PrivateReductionUtils.cpp:0:0
 #9 0x00000000067c5328 Fortran::lower::populateByRefInitAndCleanupRegions(Fortran::lower::AbstractConverter&, mlir::Location, mlir::Type, mlir::Value, mlir::Block*, mlir::Value, mlir::Value, mlir::Region&, Fortran::lower::DeclOperationKi
nd, Fortran::semantics::Symbol const*, bool, bool) (/work/groups/ssoft/compiler/llvm/aarch64/main-20260119-eb773961aac4/bin/flang+0x67c5328)
#10 0x00000000067cae88 mlir::omp::DeclareReductionOp Fortran::lower::omp::ReductionProcessor::createDeclareReductionHelper<mlir::omp::DeclareReductionOp>(Fortran::lower::AbstractConverter&, llvm::StringRef, mlir::Type, mlir::Location, bo
ol, std::function<void (fir::FirOpBuilder&, mlir::Location, mlir::Type, mlir::Value, mlir::Value, bool)>, std::function<mlir::Value (fir::FirOpBuilder&, mlir::Location, mlir::Type, mlir::Value)>) (/work/groups/ssoft/compiler/llvm/aarch64
/main-20260119-eb773961aac4/bin/flang+0x67cae88)
#11 0x00000000066f2784 genOMP(Fortran::lower::AbstractConverter&, Fortran::lower::SymMap&, Fortran::semantics::SemanticsContext&, Fortran::lower::pft::Evaluation&, Fortran::parser::OpenMPDeclareReductionConstruct const&) (.isra.22386) Op
enMP.cpp:0:0
#12 0x00000000066dccd8 Fortran::lower::genOpenMPDeclarativeConstruct(Fortran::lower::AbstractConverter&, Fortran::lower::SymMap&, Fortran::semantics::SemanticsContext&, Fortran::lower::pft::Evaluation&, Fortran::parser::OpenMPDeclarative
Construct const&) (/work/groups/ssoft/compiler/llvm/aarch64/main-20260119-eb773961aac4/bin/flang+0x66dccd8)
#13 0x000000000633efac std::__detail::__variant::__gen_vtable_impl<std::__detail::__variant::_Multi_array<void (*)(Fortran::common::visitors<auto Fortran::lower::pft::ReferenceVariantBase<true, Fortran::parser::AllocateStmt, Fortran::par
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
0'(auto const&)>(auto&&) const::'lambda'(auto)>&&, std::variant<Fortran::common::Reference<Fortran::parser::AllocateStmt const>, Fortran::common::Reference<Fortran::parser::AssignmentStmt const>, Fortran::common::Reference<Fortran::parse
r::BackspaceStmt const>, Fortran::common::Reference<Fortran::parser::CallStmt const>, Fortran::common::Reference<Fortran::parser::CloseStmt const>, Fortran::common::Reference<Fortran::parser::ContinueStmt const>, Fortran::common::Referen
ce<Fortran::parser::CycleStmt const>, Fortran::common::Reference<Fortran::parser::DeallocateStmt const>, Fortran::common::Reference<Fortran::parser::EndfileStmt const>, Fortran::common::Reference<Fortran::parser::EventPostStmt const>, Fo
rtran::common::Reference<Fortran::parser::EventWaitStmt const>, Fortran::common::Reference<Fortran::parser::ExitStmt const>, Fortran::common::Reference<Fortran::parser::FailImageStmt const>, Fortran::common::Reference<Fortran::parser::Fl
ushStmt const>, Fortran::common::Reference<Fortran::parser::FormTeamStmt const>, Fortran::common::Reference<Fortran::parser::GotoStmt const>, Fortran::common::Reference<Fortran::parser::IfStmt const>, Fortran::common::Reference<Fortran::
parser::InquireStmt const>, Fortran::common::Reference<Fortran::parser::LockStmt const>, Fortran::common::Reference<Fortran::parser::NotifyWaitStmt const>, Fortran::common::Reference<Fortran::parser::NullifyStmt const>, Fortran::common::
Reference<Fortran::parser::OpenStmt const>, Fortran::common::Reference<Fortran::parser::PointerAssignmentStmt const>, Fortran::common::Reference<Fortran::parser::PrintStmt const>, Fortran::common::Reference<Fortran::parser::ReadStmt cons
t>, Fortran::common::Reference<Fortran::parser::ReturnStmt const>, Fortran::common::Reference<Fortran::parser::RewindStmt const>, Fortran::common::Reference<Fortran::parser::StopStmt const>, Fortran::common::Reference<Fortran::parser::Sy
ncAllStmt const>, Fortran::common::Reference<Fortran::parser::SyncImagesStmt const>, Fortran::common::Reference<Fortran::parser::SyncMemoryStmt const>, Fortran::common::Reference<Fortran::parser::SyncTeamStmt const>, Fortran::common::Ref
erence<Fortran::parser::UnlockStmt const>, Fortran::common::Reference<Fortran::parser::WaitStmt const>, Fortran::common::Reference<Fortran::parser::WhereStmt const>, Fortran::common::Reference<Fortran::parser::WriteStmt const>, Fortran::
common::Reference<Fortran::parser::ComputedGotoStmt const>, Fortran::common::Reference<Fortran::parser::ForallStmt const>, Fortran::common::Reference<Fortran::parser::ArithmeticIfStmt const>, Fortran::common::Reference<Fortran::parser::A
ssignStmt const>, Fortran::common::Reference<Fortran::parser::AssignedGotoStmt const>, Fortran::common::Reference<Fortran::parser::PauseStmt const>, Fortran::common::Reference<Fortran::parser::EntryStmt const>, Fortran::common::Reference
<Fortran::parser::FormatStmt const>, Fortran::common::Reference<Fortran::parser::AssociateStmt const>, Fortran::common::Reference<Fortran::parser::EndAssociateStmt const>, Fortran::common::Reference<Fortran::parser::BlockStmt const>, For
tran::common::Reference<Fortran::parser::EndBlockStmt const>, Fortran::common::Reference<Fortran::parser::SelectCaseStmt const>, Fortran::common::Reference<Fortran::parser::CaseStmt const>, Fortran::common::Reference<Fortran::parser::End
SelectStmt const>, Fortran::common::Reference<Fortran::parser::ChangeTeamStmt const>, Fortran::common::Reference<Fortran::parser::EndChangeTeamStmt const>, Fortran::common::Reference<Fortran::parser::CriticalStmt const>, Fortran::common:
:Reference<Fortran::parser::EndCriticalStmt const>, Fortran::common::Reference<Fortran::parser::NonLabelDoStmt const>, Fortran::common::Reference<Fortran::parser::EndDoStmt const>, Fortran::common::Reference<Fortran::parser::IfThenStmt c
onst>, Fortran::common::Reference<Fortran::parser::ElseIfStmt const>, Fortran::common::Reference<Fortran::parser::ElseStmt const>, Fortran::common::Reference<Fortran::parser::EndIfStmt const>, Fortran::common::Reference<Fortran::parser::
SelectRankStmt const>, Fortran::common::Reference<Fortran::parser::SelectRankCaseStmt const>, Fortran::common::Reference<Fortran::parser::SelectTypeStmt const>, Fortran::common::Reference<Fortran::parser::TypeGuardStmt const>, Fortran::c
ommon::Reference<Fortran::parser::WhereConstructStmt const>, Fortran::common::Reference<Fortran::parser::MaskedElsewhereStmt const>, Fortran::common::Reference<Fortran::parser::ElsewhereStmt const>, Fortran::common::Reference<Fortran::pa
rser::EndWhereStmt const>, Fortran::common::Reference<Fortran::parser::ForallConstructStmt const>, Fortran::common::Reference<Fortran::parser::EndForallStmt const>, Fortran::common::Reference<Fortran::parser::EndProgramStmt const>, Fortr
an::common::Reference<Fortran::parser::EndFunctionStmt const>, Fortran::common::Reference<Fortran::parser::EndSubroutineStmt const>, Fortran::common::Reference<Fortran::parser::EndMpSubprogramStmt const>, Fortran::common::Reference<Fortr
an::parser::AssociateConstruct const>, Fortran::common::Reference<Fortran::parser::BlockConstruct const>, Fortran::common::Reference<Fortran::parser::CaseConstruct const>, Fortran::common::Reference<Fortran::parser::ChangeTeamConstruct c
onst>, Fortran::common::Reference<Fortran::parser::CriticalConstruct const>, Fortran::common::Reference<Fortran::parser::DoConstruct const>, Fortran::common::Reference<Fortran::parser::IfConstruct const>, Fortran::common::Reference<Fortr
an::parser::SelectRankConstruct const>, Fortran::common::Reference<Fortran::parser::SelectTypeConstruct const>, Fortran::common::Reference<Fortran::parser::WhereConstruct const>, Fortran::common::Reference<Fortran::parser::ForallConstruc
t const>, Fortran::common::Reference<Fortran::parser::CompilerDirective const>, Fortran::common::Reference<Fortran::parser::OpenACCConstruct const>, Fortran::common::Reference<Fortran::parser::OpenACCRoutineConstruct const>, Fortran::com
mon::Reference<Fortran::parser::OpenACCDeclarativeConstruct const>, Fortran::common::Reference<Fortran::parser::OpenMPConstruct const>, Fortran::common::Reference<Fortran::parser::OpenMPDeclarativeConstruct const>, Fortran::common::Refer
ence<Fortran::parser::OmpEndLoopDirective const>, Fortran::common::Reference<Fortran::parser::CUFKernelDoConstruct const>> const&)>, std::tuple<std::variant<Fortran::common::Reference<Fortran::parser::AllocateStmt const>, Fortran::common
::Reference<Fortran::parser::AssignmentStmt const>, Fortran::common::Reference<Fortran::parser::BackspaceStmt const>, Fortran::common::Reference<Fortran::parser::CallStmt const>, Fortran::common::Reference<Fortran::parser::CloseStmt cons
t>, Fortran::common::Reference<Fortran::parser::ContinueStmt const>, Fortran::common::Reference<Fortran::parser::CycleStmt const>, Fortran::common::Reference<Fortran::parser::DeallocateStmt const>, Fortran::common::Reference<Fortran::par
ser::EndfileStmt const>, Fortran::common::Reference<Fortran::parser::EventPostStmt const>, Fortran::common::Reference<Fortran::parser::EventWaitStmt const>, Fortran::common::Reference<Fortran::parser::ExitStmt const>, Fortran::common::Re
ference<Fortran::parser::FailImageStmt const>, Fortran::common::Reference<Fortran::parser::FlushStmt const>, Fortran::common::Reference<Fortran::parser::FormTeamStmt const>, Fortran::common::Reference<Fortran::parser::GotoStmt const>, Fo
rtran::common::Reference<Fortran::parser::IfStmt const>, Fortran::common::Reference<Fortran::parser::InquireStmt const>, Fortran::common::Reference<Fortran::parser::LockStmt const>, Fortran::common::Reference<Fortran::parser::NotifyWaitS
tmt const>, Fortran::common::Reference<Fortran::parser::NullifyStmt const>, Fortran::common::Reference<Fortran::parser::OpenStmt const>, Fortran::common::Reference<Fortran::parser::PointerAssignmentStmt const>, Fortran::common::Reference
<Fortran::parser::PrintStmt const>, Fortran::common::Reference<Fortran::parser::ReadStmt const>, Fortran::common::Reference<Fortran::parser::ReturnStmt const>, Fortran::common::Reference<Fortran::parser::RewindStmt const>, Fortran::commo
n::Reference<Fortran::parser::StopStmt const>, Fortran::common::Reference<Fortran::parser::SyncAllStmt const>, Fortran::common::Reference<Fortran::parser::SyncImagesStmt const>, Fortran::common::Reference<Fortran::parser::SyncMemoryStmt
const>, Fortran::common::Reference<Fortran::parser::SyncTeamStmt const>, Fortran::common::Reference<Fortran::parser::UnlockStmt const>, Fortran::common::Reference<Fortran::parser::WaitStmt const>, Fortran::common::Reference<Fortran::pars
er::WhereStmt const>, Fortran::common::Reference<Fortran::parser::WriteStmt const>, Fortran::common::Reference<Fortran::parser::ComputedGotoStmt const>, Fortran::common::Reference<Fortran::parser::ForallStmt const>, Fortran::common::Refe
rence<Fortran::parser::ArithmeticIfStmt const>, Fortran::common::Reference<Fortran::parser::AssignStmt const>, Fortran::common::Reference<Fortran::parser::AssignedGotoStmt const>, Fortran::common::Reference<Fortran::parser::PauseStmt con
st>, Fortran::common::Reference<Fortran::parser::EntryStmt const>, Fortran::common::Reference<Fortran::parser::FormatStmt const>, Fortran::common::Reference<Fortran::parser::AssociateStmt const>, Fortran::common::Reference<Fortran::parse
r::EndAssociateStmt const>, Fortran::common::Reference<Fortran::parser::BlockStmt const>, Fortran::common::Reference<Fortran::parser::EndBlockStmt const>, Fortran::common::Reference<Fortran::parser::SelectCaseStmt const>, Fortran::common
::Reference<Fortran::parser::CaseStmt const>, Fortran::common::Reference<Fortran::parser::EndSelectStmt const>, Fortran::common::Reference<Fortran::parser::ChangeTeamStmt const>, Fortran::common::Reference<Fortran::parser::EndChangeTeamS
tmt const>, Fortran::common::Reference<Fortran::parser::CriticalStmt const>, Fortran::common::Reference<Fortran::parser::EndCriticalStmt const>, Fortran::common::Reference<Fortran::parser::NonLabelDoStmt const>, Fortran::common::Referenc
e<Fortran::parser::EndDoStmt const>, Fortran::common::Reference<Fortran::parser::IfThenStmt const>, Fortran::common::Reference<Fortran::parser::ElseIfStmt const>, Fortran::common::Reference<Fortran::parser::ElseStmt const>, Fortran::comm
on::Reference<Fortran::parser::EndIfStmt const>, Fortran::common::Reference<Fortran::parser::SelectRankStmt const>, Fortran::common::Reference<Fortran::parser::SelectRankCaseStmt const>, Fortran::common::Reference<Fortran::parser::Select
TypeStmt const>, Fortran::common::Reference<Fortran::parser::TypeGuardStmt const>, Fortran::common::Reference<Fortran::parser::WhereConstructStmt const>, Fortran::common::Reference<Fortran::parser::MaskedElsewhereStmt const>, Fortran::co
mmon::Reference<Fortran::parser::ElsewhereStmt const>, Fortran::common::Reference<Fortran::parser::EndWhereStmt const>, Fortran::common::Reference<Fortran::parser::ForallConstructStmt const>, Fortran::common::Reference<Fortran::parser::E
ndForallStmt const>, Fortran::common::Reference<Fortran::parser::EndProgramStmt const>, Fortran::common::Reference<Fortran::parser::EndFunctionStmt const>, Fortran::common::Reference<Fortran::parser::EndSubroutineStmt const>, Fortran::co
mmon::Reference<Fortran::parser::EndMpSubprogramStmt const>, Fortran::common::Reference<Fortran::parser::AssociateConstruct const>, Fortran::common::Reference<Fortran::parser::BlockConstruct const>, Fortran::common::Reference<Fortran::pa
rser::CaseConstruct const>, Fortran::common::Reference<Fortran::parser::ChangeTeamConstruct const>, Fortran::common::Reference<Fortran::parser::CriticalConstruct const>, Fortran::common::Reference<Fortran::parser::DoConstruct const>, For
tran::common::Reference<Fortran::parser::IfConstruct const>, Fortran::common::Reference<Fortran::parser::SelectRankConstruct const>, Fortran::common::Reference<Fortran::parser::SelectTypeConstruct const>, Fortran::common::Reference<Fortr
an::parser::WhereConstruct const>, Fortran::common::Reference<Fortran::parser::ForallConstruct const>, Fortran::common::Reference<Fortran::parser::CompilerDirective const>, Fortran::common::Reference<Fortran::parser::OpenACCConstruct con
st>, Fortran::common::Reference<Fortran::parser::OpenACCRoutineConstruct const>, Fortran::common::Reference<Fortran::parser::OpenACCDeclarativeConstruct const>, Fortran::common::Reference<Fortran::parser::OpenMPConstruct const>, Fortran:
:common::Reference<Fortran::parser::OpenMPDeclarativeConstruct const>, Fortran::common::Reference<Fortran::parser::OmpEndLoopDirective const>, Fortran::common::Reference<Fortran::parser::CUFKernelDoConstruct const>> const&>, std::integer
_sequence<unsigned long, 91ul>>::__visit_invoke(Fortran::common::visitors<auto Fortran::lower::pft::ReferenceVariantBase<true, Fortran::parser::AllocateStmt, Fortran::parser::AssignmentStmt, Fortran::parser::BackspaceStmt, Fortran::parse
r::CallStmt, Fortran::parser::CloseStmt, Fortran::parser::ContinueStmt, Fortran::parser::CycleStmt, Fortran::parser::DeallocateStmt, Fortran::parser::EndfileStmt, Fortran::parser::EventPostStmt, Fortran::parser::EventWaitStmt, Fortran::p
arser::ExitStmt, Fortran::parser::FailImageStmt, Fortran::parser::FlushStmt, Fortran::parser::FormTeamStmt, Fortran::parser::GotoStmt, Fortran::parser::IfStmt, Fortran::parser::InquireStmt, Fortran::parser::LockStmt, Fortran::parser::Not
ifyWaitStmt, Fortran::parser::NullifyStmt, Fortran::parser::OpenStmt, Fortran::parser::PointerAssignmentStmt, Fortran::parser::PrintStmt, Fortran::parser::ReadStmt, Fortran::parser::ReturnStmt, Fortran::parser::RewindStmt, Fortran::parse
r::StopStmt, Fortran::parser::SyncAllStmt, Fortran::parser::SyncImagesStmt, Fortran::parser::SyncMemoryStmt, Fortran::parser::SyncTeamStmt, Fortran::parser::UnlockStmt, Fortran::parser::WaitStmt, Fortran::parser::WhereStmt, Fortran::pars
er::WriteStmt, Fortran::parser::ComputedGotoStmt, Fortran::parser::ForallStmt, Fortran::parser::ArithmeticIfStmt, Fortran::parser::AssignStmt, Fortran::parser::AssignedGotoStmt, Fortran::parser::PauseStmt, Fortran::parser::EntryStmt, For
tran::parser::FormatStmt, Fortran::parser::AssociateStmt, Fortran::parser::EndAssociateStmt, Fortran::parser::BlockStmt, Fortran::parser::EndBlockStmt, Fortran::parser::SelectCaseStmt, Fortran::parser::CaseStmt, Fortran::parser::EndSelec
tStmt, Fortran::parser::ChangeTeamStmt, Fortran::parser::EndChangeTeamStmt, Fortran::parser::CriticalStmt, Fortran::parser::EndCriticalStmt, Fortran::parser::NonLabelDoStmt, Fortran::parser::EndDoStmt, Fortran::parser::IfThenStmt, Fortra
n::parser::ElseIfStmt, Fortran::parser::ElseStmt, Fortran::parser::EndIfStmt, Fortran::parser::SelectRankStmt, Fortran::parser::SelectRankCaseStmt, Fortran::parser::SelectTypeStmt, Fortran::parser::TypeGuardStmt, Fortran::parser::WhereCo
nstructStmt, Fortran::parser::MaskedElsewhereStmt, Fortran::parser::ElsewhereStmt, Fortran::parser::EndWhereStmt, Fortran::parser::ForallConstructStmt, Fortran::parser::EndForallStmt, Fortran::parser::EndProgramStmt, Fortran::parser::End
FunctionStmt, Fortran::parser::EndSubroutineStmt, Fortran::parser::EndMpSubprogramStmt, Fortran::parser::AssociateConstruct, Fortran::parser::BlockConstruct, Fortran::parser::CaseConstruct, Fortran::parser::ChangeTeamConstruct, Fortran::
parser::CriticalConstruct, Fortran::parser::DoConstruct, Fortran::parser::IfConstruct, Fortran::parser::SelectRankConstruct, Fortran::parser::SelectTypeConstruct, Fortran::parser::WhereConstruct, Fortran::parser::ForallConstruct, Fortran
::parser::CompilerDirective, Fortran::parser::OpenACCConstruct, Fortran::parser::OpenACCRoutineConstruct, Fortran::parser::OpenACCDeclarativeConstruct, Fortran::parser::OpenMPConstruct, Fortran::parser::OpenMPDeclarativeConstruct, Fortra
n::parser::OmpEndLoopDirective, Fortran::parser::CUFKernelDoConstruct>::visit<(anonymous namespace)::FirConverter::genFIR(Fortran::lower::pft::Evaluation&, bool)::'lambda0'(auto const&)>(auto&&) const::'lambda'(auto)>&&, std::variant<For
tran::common::Reference<Fortran::parser::AllocateStmt const>, Fortran::common::Reference<Fortran::parser::AssignmentStmt const>, Fortran::common::Reference<Fortran::parser::BackspaceStmt const>, Fortran::common::Reference<Fortran::parser
::CallStmt const>, Fortran::common::Reference<Fortran::parser::CloseStmt const>, Fortran::common::Reference<Fortran::parser::ContinueStmt const>, Fortran::common::Reference<Fortran::parser::CycleStmt const>, Fortran::common::Reference<Fo
rtran::parser::DeallocateStmt const>, Fortran::common::Reference<Fortran::parser::EndfileStmt const>, Fortran::common::Reference<Fortran::parser::EventPostStmt const>, Fortran::common::Reference<Fortran::parser::EventWaitStmt const>, For
tran::common::Reference<Fortran::parser::ExitStmt const>, Fortran::common::Reference<Fortran::parser::FailImageStmt const>, Fortran::common::Reference<Fortran::parser::FlushStmt const>, Fortran::common::Reference<Fortran::parser::FormTea
mStmt const>, Fortran::common::Reference<Fortran::parser::GotoStmt const>, Fortran::common::Reference<Fortran::parser::IfStmt const>, Fortran::common::Reference<Fortran::parser::InquireStmt const>, Fortran::common::Reference<Fortran::par
ser::LockStmt const>, Fortran::common::Reference<Fortran::parser::NotifyWaitStmt const>, Fortran::common::Reference<Fortran::parser::NullifyStmt const>, Fortran::common::Reference<Fortran::parser::OpenStmt const>, Fortran::common::Refere
nce<Fortran::parser::PointerAssignmentStmt const>, Fortran::common::Reference<Fortran::parser::PrintStmt const>, Fortran::common::Reference<Fortran::parser::ReadStmt const>, Fortran::common::Reference<Fortran::parser::ReturnStmt const>,
Fortran::common::Reference<Fortran::parser::RewindStmt const>, Fortran::common::Reference<Fortran::parser::StopStmt const>, Fortran::common::Reference<Fortran::parser::SyncAllStmt const>, Fortran::common::Reference<Fortran::parser::SyncI
magesStmt const>, Fortran::common::Reference<Fortran::parser::SyncMemoryStmt const>, Fortran::common::Reference<Fortran::parser::SyncTeamStmt const>, Fortran::common::Reference<Fortran::parser::UnlockStmt const>, Fortran::common::Referen
ce<Fortran::parser::WaitStmt const>, Fortran::common::Reference<Fortran::parser::WhereStmt const>, Fortran::common::Reference<Fortran::parser::WriteStmt const>, Fortran::common::Reference<Fortran::parser::ComputedGotoStmt const>, Fortran
::common::Reference<Fortran::parser::ForallStmt const>, Fortran::common::Reference<Fortran::parser::ArithmeticIfStmt const>, Fortran::common::Reference<Fortran::parser::AssignStmt const>, Fortran::common::Reference<Fortran::parser::Assig
nedGotoStmt const>, Fortran::common::Reference<Fortran::parser::PauseStmt const>, Fortran::common::Reference<Fortran::parser::EntryStmt const>, Fortran::common::Reference<Fortran::parser::FormatStmt const>, Fortran::common::Reference<For
tran::parser::AssociateStmt const>, Fortran::common::Reference<Fortran::parser::EndAssociateStmt const>, Fortran::common::Reference<Fortran::parser::BlockStmt const>, Fortran::common::Reference<Fortran::parser::EndBlockStmt const>, Fortr
an::common::Reference<Fortran::parser::SelectCaseStmt const>, Fortran::common::Reference<Fortran::parser::CaseStmt const>, Fortran::common::Reference<Fortran::parser::EndSelectStmt const>, Fortran::common::Reference<Fortran::parser::Chan
geTeamStmt const>, Fortran::common::Reference<Fortran::parser::EndChangeTeamStmt const>, Fortran::common::Reference<Fortran::parser::CriticalStmt const>, Fortran::common::Reference<Fortran::parser::EndCriticalStmt const>, Fortran::common
::Reference<Fortran::parser::NonLabelDoStmt const>, Fortran::common::Reference<Fortran::parser::EndDoStmt const>, Fortran::common::Reference<Fortran::parser::IfThenStmt const>, Fortran::common::Reference<Fortran::parser::ElseIfStmt const
>, Fortran::common::Reference<Fortran::parser::ElseStmt const>, Fortran::common::Reference<Fortran::parser::EndIfStmt const>, Fortran::common::Reference<Fortran::parser::SelectRankStmt const>, Fortran::common::Reference<Fortran::parser::
SelectRankCaseStmt const>, Fortran::common::Reference<Fortran::parser::SelectTypeStmt const>, Fortran::common::Reference<Fortran::parser::TypeGuardStmt const>, Fortran::common::Reference<Fortran::parser::WhereConstructStmt const>, Fortra
n::common::Reference<Fortran::parser::MaskedElsewhereStmt const>, Fortran::common::Reference<Fortran::parser::ElsewhereStmt const>, Fortran::common::Reference<Fortran::parser::EndWhereStmt const>, Fortran::common::Reference<Fortran::pars
er::ForallConstructStmt const>, Fortran::common::Reference<Fortran::parser::EndForallStmt const>, Fortran::common::Reference<Fortran::parser::EndProgramStmt const>, Fortran::common::Reference<Fortran::parser::EndFunctionStmt const>, Fort
ran::common::Reference<Fortran::parser::EndSubroutineStmt const>, Fortran::common::Reference<Fortran::parser::EndMpSubprogramStmt const>, Fortran::common::Reference<Fortran::parser::AssociateConstruct const>, Fortran::common::Reference<F
ortran::parser::BlockConstruct const>, Fortran::common::Reference<Fortran::parser::CaseConstruct const>, Fortran::common::Reference<Fortran::parser::ChangeTeamConstruct const>, Fortran::common::Reference<Fortran::parser::CriticalConstruc
t const>, Fortran::common::Reference<Fortran::parser::DoConstruct const>, Fortran::common::Reference<Fortran::parser::IfConstruct const>, Fortran::common::Reference<Fortran::parser::SelectRankConstruct const>, Fortran::common::Reference<
Fortran::parser::SelectTypeConstruct const>, Fortran::common::Reference<Fortran::parser::WhereConstruct const>, Fortran::common::Reference<Fortran::parser::ForallConstruct const>, Fortran::common::Reference<Fortran::parser::CompilerDirec
tive const>, Fortran::common::Reference<Fortran::parser::OpenACCConstruct const>, Fortran::common::Reference<Fortran::parser::OpenACCRoutineConstruct const>, Fortran::common::Reference<Fortran::parser::OpenACCDeclarativeConstruct const>,
 Fortran::common::Reference<Fortran::parser::OpenMPConstruct const>, Fortran::common::Reference<Fortran::parser::OpenMPDeclarativeConstruct const>, Fortran::common::Reference<Fortran::parser::OmpEndLoopDirective const>, Fortran::common::
Reference<Fortran::parser::CUFKernelDoConstruct const>> const&) Bridge.cpp:0:0
#14 0x0000000006384c0c (anonymous namespace)::FirConverter::lowerFunc(Fortran::lower::pft::FunctionLikeUnit&) Bridge.cpp:0:0
#15 0x0000000006386e0c Fortran::lower::LoweringBridge::lower(Fortran::parser::Program const&, Fortran::semantics::SemanticsContext const&) (/work/groups/ssoft/compiler/llvm/aarch64/main-20260119-eb773961aac4/bin/flang+0x6386e0c)
#16 0x0000000006239ec0 Fortran::frontend::CodeGenAction::beginSourceFileAction() (/work/groups/ssoft/compiler/llvm/aarch64/main-20260119-eb773961aac4/bin/flang+0x6239ec0)
#17 0x0000000006227694 Fortran::frontend::FrontendAction::beginSourceFile(Fortran::frontend::CompilerInstance&, Fortran::frontend::FrontendInputFile const&) (/work/groups/ssoft/compiler/llvm/aarch64/main-20260119-eb773961aac4/bin/flang+0
x6227694)
#18 0x0000000006213e2c Fortran::frontend::CompilerInstance::executeAction(Fortran::frontend::FrontendAction&) (/work/groups/ssoft/compiler/llvm/aarch64/main-20260119-eb773961aac4/bin/flang+0x6213e2c)
#19 0x000000000622dbe4 Fortran::frontend::executeCompilerInvocation(Fortran::frontend::CompilerInstance*) (/work/groups/ssoft/compiler/llvm/aarch64/main-20260119-eb773961aac4/bin/flang+0x622dbe4)
#20 0x000000000620c5e0 fc1_main(llvm::ArrayRef<char const*>, char const*) (/work/groups/ssoft/compiler/llvm/aarch64/main-20260119-eb773961aac4/bin/flang+0x620c5e0)
#21 0x000000000620bfd8 main (/work/groups/ssoft/compiler/llvm/aarch64/main-20260119-eb773961aac4/bin/flang+0x620bfd8)
#22 0x000040001fa44404 __libc_start_main (/lib64/libc.so.6+0x24404)
#23 0x000000000620a45c _start (/work/groups/ssoft/compiler/llvm/aarch64/main-20260119-eb773961aac4/bin/flang+0x620a45c)
flang-23: error: unable to execute command: Aborted (core dumped)
flang-23: error: flang frontend command failed due to signal (use -v to see invocation)
flang version 23.0.0git (https://github.com/llvm/llvm-project.git eb773961aac45593ce126ed258d82c3c3abbaa93)
Target: aarch64-unknown-linux-gnu
Thread model: posix
InstalledDir: /work/groups/ssoft/compiler/llvm/aarch64/main-20260119-eb773961aac4/bin
Build config: +assertions
flang-23: note: diagnostic msg:
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
flang-23: note: diagnostic msg: /tmp/masa_udr_deferred_002_21-1ecd05
flang-23: note: diagnostic msg: /tmp/masa_udr_deferred_002_21-1ecd05.sh
flang-23: note: diagnostic msg:

********************
$
```

```
$ cat /tmp/masa_udr_deferred_002_21-1ecd05
#line "./masa_udr_deferred_002_21.f90" 1
      program main
      character(len=1),allocatable::k1
!$omp declare reduction (a:character(len=1):omp_out=max(omp_out,omp_in))&
!$omp& initializer(omp_priv='1')
!$omp parallel sections reduction(a:k1)
      k1=max(k1,'2')
!$omp end parallel sections
      end program main
$
```

```
$ cat /tmp/masa_udr_deferred_002_21-1ecd05.sh
# Crash reproducer for clang version 23.0.0git (https://github.com/llvm/llvm-project.git eb773961aac45593ce126ed258d82c3c3abbaa93)
# Driver args: "-fopenmp" "masa_udr_deferred_002_21.f90"
# Original command:  "/work/groups/ssoft/compiler/llvm/aarch64/main-20260119-eb773961aac4/bin/flang" "-fc1" "-triple" "aarch64-unknown-linux-gnu" "-emit-obj" "-mrelocation-model" "pic" "-pic-level" "2" "-pic-is-pie" "-target-cpu" "generi
c" "-target-feature" "+v8a" "-target-feature" "+fp-armv8" "-target-feature" "+neon" "-fopenmp" "-resource-dir" "/work/groups/ssoft/compiler/llvm/aarch64/main-20260119-eb773961aac4/lib/clang/23" "-mframe-pointer=non-leaf-no-reserve" "-o"
"/tmp/masa_udr_deferred_002_21-99ed9a.o" "-x" "f95" "masa_udr_deferred_002_21.f90"
 "/work/groups/ssoft/compiler/llvm/aarch64/main-20260119-eb773961aac4/bin/flang" "-fc1" "-triple" "aarch64-unknown-linux-gnu" "-emit-obj" "-mrelocation-model" "pic" "-pic-level" "2" "-pic-is-pie" "-target-cpu" "generic" "-target-feature"
 "+v8a" "-target-feature" "+fp-armv8" "-target-feature" "+neon" "-fopenmp" "-mframe-pointer=non-leaf-no-reserve" "-x" "f95" "masa_udr_deferred_002_21-1ecd05"
$
```

```
$ gfortran -fopenmp masa_udr_deferred_002_21.f90
masa_udr_deferred_002_21.f90:5:39:

    5 | !$omp parallel sections reduction(a:k1)
      |                                       1
internal compiler error: in fold_binary_loc, at fold-const.cc:11259
0xafb5b7 fold_binary_loc(unsigned int, tree_code, tree_node*, tree_node*, tree_node*)
        ../.././gcc/fold-const.cc:11259
0xafb62b fold_build2_loc(unsigned int, tree_code, tree_node*, tree_node*, tree_node*)
        ../.././gcc/fold-const.cc:14211
0x92021b alloc_scalar_allocatable_for_assignment
        ../.././gcc/fortran/trans-expr.cc:11752
0x92021b gfc_trans_assignment_1
        ../.././gcc/fortran/trans-expr.cc:12594
0x942eaf gfc_trans_omp_array_reduction_or_udr
        ../.././gcc/fortran/trans-openmp.cc:2266
0x943763 gfc_trans_omp_reduction_list
        ../.././gcc/fortran/trans-openmp.cc:2388
0x94ab27 gfc_trans_omp_clauses
        ../.././gcc/fortran/trans-openmp.cc:2726
0x9510bb gfc_trans_omp_parallel_sections
        ../.././gcc/fortran/trans-openmp.cc:7333
0x8d4447 trans_code
        ../.././gcc/fortran/trans.cc:2660
0x903ef7 gfc_generate_function_code(gfc_namespace*)
        ../.././gcc/fortran/trans-decl.cc:7880
0x879cef translate_all_program_units
        ../.././gcc/fortran/parse.cc:7099
0x879cef gfc_parse_file()
        ../.././gcc/fortran/parse.cc:7413
0x8d172b gfc_be_parse_file
        ../.././gcc/fortran/f95-lang.cc:241
Please submit a full bug report, with preprocessed source (by using -freport-bug).
Please include the complete backtrace with any bug report.
See <https://gcc.gnu.org/bugs/> for instructions.
$
```

```
$ ifx -qopenmp masa_udr_deferred_002_21.f90
          #0 0x000000000312d792
          #1 0x0000000003191e57
          #2 0x0000000003191f80
          #3 0x00001501cec026f0
          #4 0x000000000240baa0
          #5 0x000000000240a703
          #6 0x000000000240a51b
          #7 0x00000000022a54f7
          #8 0x00000000022a774f
          #9 0x00000000021c12e4
         #10 0x00000000021c101c
         #11 0x00000000022a919a
         #12 0x00000000022a9d54
         #13 0x00000000022a93ad
         #14 0x00000000021bbf71
         #15 0x00000000021bbe4d
         #16 0x00000000021b7163
         #17 0x00000000021b6eed
         #18 0x00000000021bd791
         #19 0x00000000021bcd8d
         #20 0x00000000027ee7fa
         #21 0x00000000027ed32a
         #22 0x00000000027ecfad
         #23 0x00000000027ee7fa
         #24 0x00000000030ce29b
         #25 0x00000000030cc544
         #26 0x000000000307863e
         #27 0x000000000324df17
         #28 0x00001501cebed590
         #29 0x00001501cebed640 __libc_start_main + 128
         #30 0x0000000002eb06ea

masa_udr_deferred_002_21.f90: error #5633: **Internal compiler error: segmentation violation signal raised** Please report this error along with the circumstances in which it occurred in a Software Problem Report.  Note: File and line given may not be explicit cause of this error.
compilation aborted for masa_udr_deferred_002_21.f90 (code 3)

$
```



## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-flang-ir

Author: ohno-fj (ohno-fj)

<details>
```
Version of flang : 23.0.0(eb773961aac45593ce126ed258d82c3c3abbaa93)/AArch64
```

In the attached program (`masa_udr_deferred_002_21.f90`), when a character type is specified in `REDUCTION clause` corresponding to `declare_reduction directive`, a compilation terminates abnormally.
After checking `OpenMP 6.0 : 7.6.14 declare_reduction Directive`, the program appears to contain no particular errors.

The following are the test program, Flang, Gfortran and ifx compilation/execution results.

masa_udr_deferred_002_21.f90:
```fortran
program main
  character(len=1),allocatable::k1
!$omp declare reduction (a:character(len=1):omp_out=max(omp_out,omp_in)) &amp;
!$omp initializer(omp_priv='1')
!$omp parallel sections reduction(a:k1)
  k1=max(k1,'2')
!$omp end parallel sections
end program main
```

```
$ flang -fopenmp masa_udr_deferred_002_21.f90
flang: /work/groups/ssoft/compiler/llvm/src/llvm-main/flang/include/flang/Optimizer/Builder/HLFIRTools.h:56: hlfir::Entity::Entity(mlir::Value): Assertion `isFortranEntity(value) &amp;&amp; "must be a value representing a Fortran value or variab
le like"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: /work/groups/ssoft/compiler/llvm/aarch64/main-20260119-eb773961aac4/bin/flang -fc1 -triple aarch64-unknown-linux-gnu -emit-obj -mrelocation-model pic -pic-level 2 -pic-is-pie -target-cpu generic -target-feature
 +v8a -target-feature +fp-armv8 -target-feature +neon -fopenmp -resource-dir /work/groups/ssoft/compiler/llvm/aarch64/main-20260119-eb773961aac4/lib/clang/23 -mframe-pointer=non-leaf-no-reserve -o /tmp/masa_udr_deferred_002_21-99ed9a.o -
x f95 masa_udr_deferred_002_21.f90
 #<!-- -->0 0x000040001a8f2360 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/work/groups/ssoft/compiler/llvm/aarch64/main-latest/lib/libLLVM.so.23.0git+0x6312360)
 #<!-- -->1 0x000040001a8ef930 llvm::sys::RunSignalHandlers() (/work/groups/ssoft/compiler/llvm/aarch64/main-latest/lib/libLLVM.so.23.0git+0x630f930)
 #<!-- -->2 0x000040001a8efa88 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x00004000081607a0 (linux-vdso.so.1+0x7a0)
 #<!-- -->4 0x000040001fa562e4 raise (/lib64/libc.so.6+0x362e4)
 #<!-- -->5 0x000040001fa40aac abort (/lib64/libc.so.6+0x20aac)
 #<!-- -->6 0x000040001fa4fc24 __assert_fail_base (/lib64/libc.so.6+0x2fc24)
 #<!-- -->7 0x000040001fa4fc98 __assert_perror_fail (/lib64/libc.so.6+0x2fc98)
 #<!-- -->8 0x00000000067c414c (anonymous namespace)::PopulateInitAndCleanupRegionsHelper::populateByRefInitAndCleanupRegions() PrivateReductionUtils.cpp:0:0
 #<!-- -->9 0x00000000067c5328 Fortran::lower::populateByRefInitAndCleanupRegions(Fortran::lower::AbstractConverter&amp;, mlir::Location, mlir::Type, mlir::Value, mlir::Block*, mlir::Value, mlir::Value, mlir::Region&amp;, Fortran::lower::DeclOperationKi
nd, Fortran::semantics::Symbol const*, bool, bool) (/work/groups/ssoft/compiler/llvm/aarch64/main-20260119-eb773961aac4/bin/flang+0x67c5328)
#<!-- -->10 0x00000000067cae88 mlir::omp::DeclareReductionOp Fortran::lower::omp::ReductionProcessor::createDeclareReductionHelper&lt;mlir::omp::DeclareReductionOp&gt;(Fortran::lower::AbstractConverter&amp;, llvm::StringRef, mlir::Type, mlir::Location, bo
ol, std::function&lt;void (fir::FirOpBuilder&amp;, mlir::Location, mlir::Type, mlir::Value, mlir::Value, bool)&gt;, std::function&lt;mlir::Value (fir::FirOpBuilder&amp;, mlir::Location, mlir::Type, mlir::Value)&gt;) (/work/groups/ssoft/compiler/llvm/aarch64
/main-20260119-eb773961aac4/bin/flang+0x67cae88)
#<!-- -->11 0x00000000066f2784 genOMP(Fortran::lower::AbstractConverter&amp;, Fortran::lower::SymMap&amp;, Fortran::semantics::SemanticsContext&amp;, Fortran::lower::pft::Evaluation&amp;, Fortran::parser::OpenMPDeclareReductionConstruct const&amp;) (.isra.22386) Op
enMP.cpp:0:0
#<!-- -->12 0x00000000066dccd8 Fortran::lower::genOpenMPDeclarativeConstruct(Fortran::lower::AbstractConverter&amp;, Fortran::lower::SymMap&amp;, Fortran::semantics::SemanticsContext&amp;, Fortran::lower::pft::Evaluation&amp;, Fortran::parser::OpenMPDeclarative
Construct const&amp;) (/work/groups/ssoft/compiler/llvm/aarch64/main-20260119-eb773961aac4/bin/flang+0x66dccd8)
#<!-- -->13 0x000000000633efac std::__detail::__variant::__gen_vtable_impl&lt;std::__detail::__variant::_Multi_array&lt;void (*)(Fortran::common::visitors&lt;auto Fortran::lower::pft::ReferenceVariantBase&lt;true, Fortran::parser::AllocateStmt, Fortran::par
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
0'(auto const&amp;)&gt;(auto&amp;&amp;) const::'lambda'(auto)&gt;&amp;&amp;, std::variant&lt;Fortran::common::Reference&lt;Fortran::parser::AllocateStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::AssignmentStmt const&gt;, Fortran::common::Reference&lt;Fortran::parse
r::BackspaceStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::CallStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::CloseStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::ContinueStmt const&gt;, Fortran::common::Referen
ce&lt;Fortran::parser::CycleStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::DeallocateStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::EndfileStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::EventPostStmt const&gt;, Fo
rtran::common::Reference&lt;Fortran::parser::EventWaitStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::ExitStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::FailImageStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::Fl
ushStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::FormTeamStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::GotoStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::IfStmt const&gt;, Fortran::common::Reference&lt;Fortran::
parser::InquireStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::LockStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::NotifyWaitStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::NullifyStmt const&gt;, Fortran::common::
Reference&lt;Fortran::parser::OpenStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::PointerAssignmentStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::PrintStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::ReadStmt cons
t&gt;, Fortran::common::Reference&lt;Fortran::parser::ReturnStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::RewindStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::StopStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::Sy
ncAllStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::SyncImagesStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::SyncMemoryStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::SyncTeamStmt const&gt;, Fortran::common::Ref
erence&lt;Fortran::parser::UnlockStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::WaitStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::WhereStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::WriteStmt const&gt;, Fortran::
common::Reference&lt;Fortran::parser::ComputedGotoStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::ForallStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::ArithmeticIfStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::A
ssignStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::AssignedGotoStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::PauseStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::EntryStmt const&gt;, Fortran::common::Reference
&lt;Fortran::parser::FormatStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::AssociateStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::EndAssociateStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::BlockStmt const&gt;, For
tran::common::Reference&lt;Fortran::parser::EndBlockStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::SelectCaseStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::CaseStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::End
SelectStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::ChangeTeamStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::EndChangeTeamStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::CriticalStmt const&gt;, Fortran::common:
:Reference&lt;Fortran::parser::EndCriticalStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::NonLabelDoStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::EndDoStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::IfThenStmt c
onst&gt;, Fortran::common::Reference&lt;Fortran::parser::ElseIfStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::ElseStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::EndIfStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::
SelectRankStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::SelectRankCaseStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::SelectTypeStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::TypeGuardStmt const&gt;, Fortran::c
ommon::Reference&lt;Fortran::parser::WhereConstructStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::MaskedElsewhereStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::ElsewhereStmt const&gt;, Fortran::common::Reference&lt;Fortran::pa
rser::EndWhereStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::ForallConstructStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::EndForallStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::EndProgramStmt const&gt;, Fortr
an::common::Reference&lt;Fortran::parser::EndFunctionStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::EndSubroutineStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::EndMpSubprogramStmt const&gt;, Fortran::common::Reference&lt;Fortr
an::parser::AssociateConstruct const&gt;, Fortran::common::Reference&lt;Fortran::parser::BlockConstruct const&gt;, Fortran::common::Reference&lt;Fortran::parser::CaseConstruct const&gt;, Fortran::common::Reference&lt;Fortran::parser::ChangeTeamConstruct c
onst&gt;, Fortran::common::Reference&lt;Fortran::parser::CriticalConstruct const&gt;, Fortran::common::Reference&lt;Fortran::parser::DoConstruct const&gt;, Fortran::common::Reference&lt;Fortran::parser::IfConstruct const&gt;, Fortran::common::Reference&lt;Fortr
an::parser::SelectRankConstruct const&gt;, Fortran::common::Reference&lt;Fortran::parser::SelectTypeConstruct const&gt;, Fortran::common::Reference&lt;Fortran::parser::WhereConstruct const&gt;, Fortran::common::Reference&lt;Fortran::parser::ForallConstruc
t const&gt;, Fortran::common::Reference&lt;Fortran::parser::CompilerDirective const&gt;, Fortran::common::Reference&lt;Fortran::parser::OpenACCConstruct const&gt;, Fortran::common::Reference&lt;Fortran::parser::OpenACCRoutineConstruct const&gt;, Fortran::com
mon::Reference&lt;Fortran::parser::OpenACCDeclarativeConstruct const&gt;, Fortran::common::Reference&lt;Fortran::parser::OpenMPConstruct const&gt;, Fortran::common::Reference&lt;Fortran::parser::OpenMPDeclarativeConstruct const&gt;, Fortran::common::Refer
ence&lt;Fortran::parser::OmpEndLoopDirective const&gt;, Fortran::common::Reference&lt;Fortran::parser::CUFKernelDoConstruct const&gt;&gt; const&amp;)&gt;, std::tuple&lt;std::variant&lt;Fortran::common::Reference&lt;Fortran::parser::AllocateStmt const&gt;, Fortran::common
::Reference&lt;Fortran::parser::AssignmentStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::BackspaceStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::CallStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::CloseStmt cons
t&gt;, Fortran::common::Reference&lt;Fortran::parser::ContinueStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::CycleStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::DeallocateStmt const&gt;, Fortran::common::Reference&lt;Fortran::par
ser::EndfileStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::EventPostStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::EventWaitStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::ExitStmt const&gt;, Fortran::common::Re
ference&lt;Fortran::parser::FailImageStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::FlushStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::FormTeamStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::GotoStmt const&gt;, Fo
rtran::common::Reference&lt;Fortran::parser::IfStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::InquireStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::LockStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::NotifyWaitS
tmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::NullifyStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::OpenStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::PointerAssignmentStmt const&gt;, Fortran::common::Reference
&lt;Fortran::parser::PrintStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::ReadStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::ReturnStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::RewindStmt const&gt;, Fortran::commo
n::Reference&lt;Fortran::parser::StopStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::SyncAllStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::SyncImagesStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::SyncMemoryStmt
const&gt;, Fortran::common::Reference&lt;Fortran::parser::SyncTeamStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::UnlockStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::WaitStmt const&gt;, Fortran::common::Reference&lt;Fortran::pars
er::WhereStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::WriteStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::ComputedGotoStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::ForallStmt const&gt;, Fortran::common::Refe
rence&lt;Fortran::parser::ArithmeticIfStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::AssignStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::AssignedGotoStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::PauseStmt con
st&gt;, Fortran::common::Reference&lt;Fortran::parser::EntryStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::FormatStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::AssociateStmt const&gt;, Fortran::common::Reference&lt;Fortran::parse
r::EndAssociateStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::BlockStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::EndBlockStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::SelectCaseStmt const&gt;, Fortran::common
::Reference&lt;Fortran::parser::CaseStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::EndSelectStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::ChangeTeamStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::EndChangeTeamS
tmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::CriticalStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::EndCriticalStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::NonLabelDoStmt const&gt;, Fortran::common::Referenc
e&lt;Fortran::parser::EndDoStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::IfThenStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::ElseIfStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::ElseStmt const&gt;, Fortran::comm
on::Reference&lt;Fortran::parser::EndIfStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::SelectRankStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::SelectRankCaseStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::Select
TypeStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::TypeGuardStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::WhereConstructStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::MaskedElsewhereStmt const&gt;, Fortran::co
mmon::Reference&lt;Fortran::parser::ElsewhereStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::EndWhereStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::ForallConstructStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::E
ndForallStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::EndProgramStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::EndFunctionStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::EndSubroutineStmt const&gt;, Fortran::co
mmon::Reference&lt;Fortran::parser::EndMpSubprogramStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::AssociateConstruct const&gt;, Fortran::common::Reference&lt;Fortran::parser::BlockConstruct const&gt;, Fortran::common::Reference&lt;Fortran::pa
rser::CaseConstruct const&gt;, Fortran::common::Reference&lt;Fortran::parser::ChangeTeamConstruct const&gt;, Fortran::common::Reference&lt;Fortran::parser::CriticalConstruct const&gt;, Fortran::common::Reference&lt;Fortran::parser::DoConstruct const&gt;, For
tran::common::Reference&lt;Fortran::parser::IfConstruct const&gt;, Fortran::common::Reference&lt;Fortran::parser::SelectRankConstruct const&gt;, Fortran::common::Reference&lt;Fortran::parser::SelectTypeConstruct const&gt;, Fortran::common::Reference&lt;Fortr
an::parser::WhereConstruct const&gt;, Fortran::common::Reference&lt;Fortran::parser::ForallConstruct const&gt;, Fortran::common::Reference&lt;Fortran::parser::CompilerDirective const&gt;, Fortran::common::Reference&lt;Fortran::parser::OpenACCConstruct con
st&gt;, Fortran::common::Reference&lt;Fortran::parser::OpenACCRoutineConstruct const&gt;, Fortran::common::Reference&lt;Fortran::parser::OpenACCDeclarativeConstruct const&gt;, Fortran::common::Reference&lt;Fortran::parser::OpenMPConstruct const&gt;, Fortran:
:common::Reference&lt;Fortran::parser::OpenMPDeclarativeConstruct const&gt;, Fortran::common::Reference&lt;Fortran::parser::OmpEndLoopDirective const&gt;, Fortran::common::Reference&lt;Fortran::parser::CUFKernelDoConstruct const&gt;&gt; const&amp;&gt;, std::integer
_sequence&lt;unsigned long, 91ul&gt;&gt;::__visit_invoke(Fortran::common::visitors&lt;auto Fortran::lower::pft::ReferenceVariantBase&lt;true, Fortran::parser::AllocateStmt, Fortran::parser::AssignmentStmt, Fortran::parser::BackspaceStmt, Fortran::parse
r::CallStmt, Fortran::parser::CloseStmt, Fortran::parser::ContinueStmt, Fortran::parser::CycleStmt, Fortran::parser::DeallocateStmt, Fortran::parser::EndfileStmt, Fortran::parser::EventPostStmt, Fortran::parser::EventWaitStmt, Fortran::p
arser::ExitStmt, Fortran::parser::FailImageStmt, Fortran::parser::FlushStmt, Fortran::parser::FormTeamStmt, Fortran::parser::GotoStmt, Fortran::parser::IfStmt, Fortran::parser::InquireStmt, Fortran::parser::LockStmt, Fortran::parser::Not
ifyWaitStmt, Fortran::parser::NullifyStmt, Fortran::parser::OpenStmt, Fortran::parser::PointerAssignmentStmt, Fortran::parser::PrintStmt, Fortran::parser::ReadStmt, Fortran::parser::ReturnStmt, Fortran::parser::RewindStmt, Fortran::parse
r::StopStmt, Fortran::parser::SyncAllStmt, Fortran::parser::SyncImagesStmt, Fortran::parser::SyncMemoryStmt, Fortran::parser::SyncTeamStmt, Fortran::parser::UnlockStmt, Fortran::parser::WaitStmt, Fortran::parser::WhereStmt, Fortran::pars
er::WriteStmt, Fortran::parser::ComputedGotoStmt, Fortran::parser::ForallStmt, Fortran::parser::ArithmeticIfStmt, Fortran::parser::AssignStmt, Fortran::parser::AssignedGotoStmt, Fortran::parser::PauseStmt, Fortran::parser::EntryStmt, For
tran::parser::FormatStmt, Fortran::parser::AssociateStmt, Fortran::parser::EndAssociateStmt, Fortran::parser::BlockStmt, Fortran::parser::EndBlockStmt, Fortran::parser::SelectCaseStmt, Fortran::parser::CaseStmt, Fortran::parser::EndSelec
tStmt, Fortran::parser::ChangeTeamStmt, Fortran::parser::EndChangeTeamStmt, Fortran::parser::CriticalStmt, Fortran::parser::EndCriticalStmt, Fortran::parser::NonLabelDoStmt, Fortran::parser::EndDoStmt, Fortran::parser::IfThenStmt, Fortra
n::parser::ElseIfStmt, Fortran::parser::ElseStmt, Fortran::parser::EndIfStmt, Fortran::parser::SelectRankStmt, Fortran::parser::SelectRankCaseStmt, Fortran::parser::SelectTypeStmt, Fortran::parser::TypeGuardStmt, Fortran::parser::WhereCo
nstructStmt, Fortran::parser::MaskedElsewhereStmt, Fortran::parser::ElsewhereStmt, Fortran::parser::EndWhereStmt, Fortran::parser::ForallConstructStmt, Fortran::parser::EndForallStmt, Fortran::parser::EndProgramStmt, Fortran::parser::End
FunctionStmt, Fortran::parser::EndSubroutineStmt, Fortran::parser::EndMpSubprogramStmt, Fortran::parser::AssociateConstruct, Fortran::parser::BlockConstruct, Fortran::parser::CaseConstruct, Fortran::parser::ChangeTeamConstruct, Fortran::
parser::CriticalConstruct, Fortran::parser::DoConstruct, Fortran::parser::IfConstruct, Fortran::parser::SelectRankConstruct, Fortran::parser::SelectTypeConstruct, Fortran::parser::WhereConstruct, Fortran::parser::ForallConstruct, Fortran
::parser::CompilerDirective, Fortran::parser::OpenACCConstruct, Fortran::parser::OpenACCRoutineConstruct, Fortran::parser::OpenACCDeclarativeConstruct, Fortran::parser::OpenMPConstruct, Fortran::parser::OpenMPDeclarativeConstruct, Fortra
n::parser::OmpEndLoopDirective, Fortran::parser::CUFKernelDoConstruct&gt;::visit&lt;(anonymous namespace)::FirConverter::genFIR(Fortran::lower::pft::Evaluation&amp;, bool)::'lambda0'(auto const&amp;)&gt;(auto&amp;&amp;) const::'lambda'(auto)&gt;&amp;&amp;, std::variant&lt;For
tran::common::Reference&lt;Fortran::parser::AllocateStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::AssignmentStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::BackspaceStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser
::CallStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::CloseStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::ContinueStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::CycleStmt const&gt;, Fortran::common::Reference&lt;Fo
rtran::parser::DeallocateStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::EndfileStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::EventPostStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::EventWaitStmt const&gt;, For
tran::common::Reference&lt;Fortran::parser::ExitStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::FailImageStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::FlushStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::FormTea
mStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::GotoStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::IfStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::InquireStmt const&gt;, Fortran::common::Reference&lt;Fortran::par
ser::LockStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::NotifyWaitStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::NullifyStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::OpenStmt const&gt;, Fortran::common::Refere
nce&lt;Fortran::parser::PointerAssignmentStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::PrintStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::ReadStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::ReturnStmt const&gt;,
Fortran::common::Reference&lt;Fortran::parser::RewindStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::StopStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::SyncAllStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::SyncI
magesStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::SyncMemoryStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::SyncTeamStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::UnlockStmt const&gt;, Fortran::common::Referen
ce&lt;Fortran::parser::WaitStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::WhereStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::WriteStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::ComputedGotoStmt const&gt;, Fortran
::common::Reference&lt;Fortran::parser::ForallStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::ArithmeticIfStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::AssignStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::Assig
nedGotoStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::PauseStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::EntryStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::FormatStmt const&gt;, Fortran::common::Reference&lt;For
tran::parser::AssociateStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::EndAssociateStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::BlockStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::EndBlockStmt const&gt;, Fortr
an::common::Reference&lt;Fortran::parser::SelectCaseStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::CaseStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::EndSelectStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::Chan
geTeamStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::EndChangeTeamStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::CriticalStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::EndCriticalStmt const&gt;, Fortran::common
::Reference&lt;Fortran::parser::NonLabelDoStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::EndDoStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::IfThenStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::ElseIfStmt const
&gt;, Fortran::common::Reference&lt;Fortran::parser::ElseStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::EndIfStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::SelectRankStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::
SelectRankCaseStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::SelectTypeStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::TypeGuardStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::WhereConstructStmt const&gt;, Fortra
n::common::Reference&lt;Fortran::parser::MaskedElsewhereStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::ElsewhereStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::EndWhereStmt const&gt;, Fortran::common::Reference&lt;Fortran::pars
er::ForallConstructStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::EndForallStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::EndProgramStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::EndFunctionStmt const&gt;, Fort
ran::common::Reference&lt;Fortran::parser::EndSubroutineStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::EndMpSubprogramStmt const&gt;, Fortran::common::Reference&lt;Fortran::parser::AssociateConstruct const&gt;, Fortran::common::Reference&lt;F
ortran::parser::BlockConstruct const&gt;, Fortran::common::Reference&lt;Fortran::parser::CaseConstruct const&gt;, Fortran::common::Reference&lt;Fortran::parser::ChangeTeamConstruct const&gt;, Fortran::common::Reference&lt;Fortran::parser::CriticalConstruc
t const&gt;, Fortran::common::Reference&lt;Fortran::parser::DoConstruct const&gt;, Fortran::common::Reference&lt;Fortran::parser::IfConstruct const&gt;, Fortran::common::Reference&lt;Fortran::parser::SelectRankConstruct const&gt;, Fortran::common::Reference&lt;
Fortran::parser::SelectTypeConstruct const&gt;, Fortran::common::Reference&lt;Fortran::parser::WhereConstruct const&gt;, Fortran::common::Reference&lt;Fortran::parser::ForallConstruct const&gt;, Fortran::common::Reference&lt;Fortran::parser::CompilerDirec
tive const&gt;, Fortran::common::Reference&lt;Fortran::parser::OpenACCConstruct const&gt;, Fortran::common::Reference&lt;Fortran::parser::OpenACCRoutineConstruct const&gt;, Fortran::common::Reference&lt;Fortran::parser::OpenACCDeclarativeConstruct const&gt;,
 Fortran::common::Reference&lt;Fortran::parser::OpenMPConstruct const&gt;, Fortran::common::Reference&lt;Fortran::parser::OpenMPDeclarativeConstruct const&gt;, Fortran::common::Reference&lt;Fortran::parser::OmpEndLoopDirective const&gt;, Fortran::common::
Reference&lt;Fortran::parser::CUFKernelDoConstruct const&gt;&gt; const&amp;) Bridge.cpp:0:0
#<!-- -->14 0x0000000006384c0c (anonymous namespace)::FirConverter::lowerFunc(Fortran::lower::pft::FunctionLikeUnit&amp;) Bridge.cpp:0:0
#<!-- -->15 0x0000000006386e0c Fortran::lower::LoweringBridge::lower(Fortran::parser::Program const&amp;, Fortran::semantics::SemanticsContext const&amp;) (/work/groups/ssoft/compiler/llvm/aarch64/main-20260119-eb773961aac4/bin/flang+0x6386e0c)
#<!-- -->16 0x0000000006239ec0 Fortran::frontend::CodeGenAction::beginSourceFileAction() (/work/groups/ssoft/compiler/llvm/aarch64/main-20260119-eb773961aac4/bin/flang+0x6239ec0)
#<!-- -->17 0x0000000006227694 Fortran::frontend::FrontendAction::beginSourceFile(Fortran::frontend::CompilerInstance&amp;, Fortran::frontend::FrontendInputFile const&amp;) (/work/groups/ssoft/compiler/llvm/aarch64/main-20260119-eb773961aac4/bin/flang+0
x6227694)
#<!-- -->18 0x0000000006213e2c Fortran::frontend::CompilerInstance::executeAction(Fortran::frontend::FrontendAction&amp;) (/work/groups/ssoft/compiler/llvm/aarch64/main-20260119-eb773961aac4/bin/flang+0x6213e2c)
#<!-- -->19 0x000000000622dbe4 Fortran::frontend::executeCompilerInvocation(Fortran::frontend::CompilerInstance*) (/work/groups/ssoft/compiler/llvm/aarch64/main-20260119-eb773961aac4/bin/flang+0x622dbe4)
#<!-- -->20 0x000000000620c5e0 fc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*) (/work/groups/ssoft/compiler/llvm/aarch64/main-20260119-eb773961aac4/bin/flang+0x620c5e0)
#<!-- -->21 0x000000000620bfd8 main (/work/groups/ssoft/compiler/llvm/aarch64/main-20260119-eb773961aac4/bin/flang+0x620bfd8)
#<!-- -->22 0x000040001fa44404 __libc_start_main (/lib64/libc.so.6+0x24404)
#<!-- -->23 0x000000000620a45c _start (/work/groups/ssoft/compiler/llvm/aarch64/main-20260119-eb773961aac4/bin/flang+0x620a45c)
flang-23: error: unable to execute command: Aborted (core dumped)
flang-23: error: flang frontend command failed due to signal (use -v to see invocation)
flang version 23.0.0git (https://github.com/llvm/llvm-project.git eb773961aac45593ce126ed258d82c3c3abbaa93)
Target: aarch64-unknown-linux-gnu
Thread model: posix
InstalledDir: /work/groups/ssoft/compiler/llvm/aarch64/main-20260119-eb773961aac4/bin
Build config: +assertions
flang-23: note: diagnostic msg:
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
flang-23: note: diagnostic msg: /tmp/masa_udr_deferred_002_21-1ecd05
flang-23: note: diagnostic msg: /tmp/masa_udr_deferred_002_21-1ecd05.sh
flang-23: note: diagnostic msg:

********************
$
```

```
$ cat /tmp/masa_udr_deferred_002_21-1ecd05
#line "./masa_udr_deferred_002_21.f90" 1
      program main
      character(len=1),allocatable::k1
!$omp declare reduction (a:character(len=1):omp_out=max(omp_out,omp_in))&amp;
!$omp&amp; initializer(omp_priv='1')
!$omp parallel sections reduction(a:k1)
      k1=max(k1,'2')
!$omp end parallel sections
      end program main
$
```

```
$ cat /tmp/masa_udr_deferred_002_21-1ecd05.sh
# Crash reproducer for clang version 23.0.0git (https://github.com/llvm/llvm-project.git eb773961aac45593ce126ed258d82c3c3abbaa93)
# Driver args: "-fopenmp" "masa_udr_deferred_002_21.f90"
# Original command:  "/work/groups/ssoft/compiler/llvm/aarch64/main-20260119-eb773961aac4/bin/flang" "-fc1" "-triple" "aarch64-unknown-linux-gnu" "-emit-obj" "-mrelocation-model" "pic" "-pic-level" "2" "-pic-is-pie" "-target-cpu" "generi
c" "-target-feature" "+v8a" "-target-feature" "+fp-armv8" "-target-feature" "+neon" "-fopenmp" "-resource-dir" "/work/groups/ssoft/compiler/llvm/aarch64/main-20260119-eb773961aac4/lib/clang/23" "-mframe-pointer=non-leaf-no-reserve" "-o"
"/tmp/masa_udr_deferred_002_21-99ed9a.o" "-x" "f95" "masa_udr_deferred_002_21.f90"
 "/work/groups/ssoft/compiler/llvm/aarch64/main-20260119-eb773961aac4/bin/flang" "-fc1" "-triple" "aarch64-unknown-linux-gnu" "-emit-obj" "-mrelocation-model" "pic" "-pic-level" "2" "-pic-is-pie" "-target-cpu" "generic" "-target-feature"
 "+v8a" "-target-feature" "+fp-armv8" "-target-feature" "+neon" "-fopenmp" "-mframe-pointer=non-leaf-no-reserve" "-x" "f95" "masa_udr_deferred_002_21-1ecd05"
$
```

```
$ gfortran -fopenmp masa_udr_deferred_002_21.f90
masa_udr_deferred_002_21.f90:5:39:

    5 | !$omp parallel sections reduction(a:k1)
      |                                       1
internal compiler error: in fold_binary_loc, at fold-const.cc:11259
0xafb5b7 fold_binary_loc(unsigned int, tree_code, tree_node*, tree_node*, tree_node*)
        ../.././gcc/fold-const.cc:11259
0xafb62b fold_build2_loc(unsigned int, tree_code, tree_node*, tree_node*, tree_node*)
        ../.././gcc/fold-const.cc:14211
0x92021b alloc_scalar_allocatable_for_assignment
        ../.././gcc/fortran/trans-expr.cc:11752
0x92021b gfc_trans_assignment_1
        ../.././gcc/fortran/trans-expr.cc:12594
0x942eaf gfc_trans_omp_array_reduction_or_udr
        ../.././gcc/fortran/trans-openmp.cc:2266
0x943763 gfc_trans_omp_reduction_list
        ../.././gcc/fortran/trans-openmp.cc:2388
0x94ab27 gfc_trans_omp_clauses
        ../.././gcc/fortran/trans-openmp.cc:2726
0x9510bb gfc_trans_omp_parallel_sections
        ../.././gcc/fortran/trans-openmp.cc:7333
0x8d4447 trans_code
        ../.././gcc/fortran/trans.cc:2660
0x903ef7 gfc_generate_function_code(gfc_namespace*)
        ../.././gcc/fortran/trans-decl.cc:7880
0x879cef translate_all_program_units
        ../.././gcc/fortran/parse.cc:7099
0x879cef gfc_parse_file()
        ../.././gcc/fortran/parse.cc:7413
0x8d172b gfc_be_parse_file
        ../.././gcc/fortran/f95-lang.cc:241
Please submit a full bug report, with preprocessed source (by using -freport-bug).
Please include the complete backtrace with any bug report.
See &lt;https://gcc.gnu.org/bugs/&gt; for instructions.
$
```

```
$ ifx -qopenmp masa_udr_deferred_002_21.f90
          #<!-- -->0 0x000000000312d792
          #<!-- -->1 0x0000000003191e57
          #<!-- -->2 0x0000000003191f80
          #<!-- -->3 0x00001501cec026f0
          #<!-- -->4 0x000000000240baa0
          #<!-- -->5 0x000000000240a703
          #<!-- -->6 0x000000000240a51b
          #<!-- -->7 0x00000000022a54f7
          #<!-- -->8 0x00000000022a774f
          #<!-- -->9 0x00000000021c12e4
         #<!-- -->10 0x00000000021c101c
         #<!-- -->11 0x00000000022a919a
         #<!-- -->12 0x00000000022a9d54
         #<!-- -->13 0x00000000022a93ad
         #<!-- -->14 0x00000000021bbf71
         #<!-- -->15 0x00000000021bbe4d
         #<!-- -->16 0x00000000021b7163
         #<!-- -->17 0x00000000021b6eed
         #<!-- -->18 0x00000000021bd791
         #<!-- -->19 0x00000000021bcd8d
         #<!-- -->20 0x00000000027ee7fa
         #<!-- -->21 0x00000000027ed32a
         #<!-- -->22 0x00000000027ecfad
         #<!-- -->23 0x00000000027ee7fa
         #<!-- -->24 0x00000000030ce29b
         #<!-- -->25 0x00000000030cc544
         #<!-- -->26 0x000000000307863e
         #<!-- -->27 0x000000000324df17
         #<!-- -->28 0x00001501cebed590
         #<!-- -->29 0x00001501cebed640 __libc_start_main + 128
         #<!-- -->30 0x0000000002eb06ea

masa_udr_deferred_002_21.f90: error #<!-- -->5633: **Internal compiler error: segmentation violation signal raised** Please report this error along with the circumstances in which it occurred in a Software Problem Report.  Note: File and line given may not be explicit cause of this error.
compilation aborted for masa_udr_deferred_002_21.f90 (code 3)

$
```


</details>


---

### Comment 2 - KrxGu

Hi, I can take this issue. I’m able to reproduce the crash with assertions enabled. The failure happens during OpenMP declare-reduction lowering in the by-ref init/cleanup region generation (in flang/lib/Lower/Support/PrivateReductionUtils.cpp), where an HLFIR Entity assertion is triggered for the CHARACTER reduction case. I plan to implement proper handling for the CHARACTER representation used here (likely fir.boxchar), add a focused regression test under flang/test/Lower/OpenMP/, and then open a PR.

If available, please assign this issue to me.

---

