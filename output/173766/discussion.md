# [flang] crashes on derived type I/O when sequence type and interface have same name

**Author:** edvinolo
**URL:** https://github.com/llvm/llvm-project/issues/173766
**Status:** Closed
**Labels:** flang:frontend, crash
**Closed Date:** 2025-12-31T20:42:52Z

## Body

Reproducer:
https://gcc.godbolt.org/z/nb7WEczr5

``` fortran
module m_1
implicit none

type :: s
    sequence
    integer :: i
end type

interface s
end interface

interface write(formatted)
    module procedure :: write_formatted
end interface

contains

subroutine write_formatted(t, unit, iotype, v_list, iostat, iomsg)
    type(s), intent(in) :: t
    integer, intent(in) :: unit
    character(len=*), intent(in) :: iotype
    integer, intent(in) :: v_list(:)
    integer, intent(out) :: iostat
    character(len=*), intent(inout) :: iomsg
end subroutine

subroutine f
    type(s) :: t
    print *, t
end subroutine

end module m_1
```

Backtrace:
```
fatal internal error: CHECK(typeSymbol_.has<DerivedTypeDetails>()) failed at /root/llvm-project/flang/lib/Semantics/type.cpp(28)
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /cefs/d9/d98d5df9f22bfabe4705b57b_clang-llvmflang-trunk-20251228/bin/flang -fc1 -triple x86_64-unknown-linux-gnu -S -mrelocation-model pic -pic-level 2 -pic-is-pie -target-cpu x86-64 -mllvm -x86-asm-syntax=intel -resource-dir /cefs/d9/d98d5df9f22bfabe4705b57b_clang-llvmflang-trunk-20251228/lib/clang/22 -mframe-pointer=all -o /app/output.s -x f95 /app/example.f90
 #0 0x000000000454e268 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/cefs/d9/d98d5df9f22bfabe4705b57b_clang-llvmflang-trunk-20251228/bin/flang+0x454e268)
 #1 0x000000000454b434 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x000077831ae42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x000077831ae969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x000077831ae42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x000077831ae287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x0000000006771009 (/cefs/d9/d98d5df9f22bfabe4705b57b_clang-llvmflang-trunk-20251228/bin/flang+0x6771009)
 #7 0x000000000512dd0f (/cefs/d9/d98d5df9f22bfabe4705b57b_clang-llvmflang-trunk-20251228/bin/flang+0x512dd0f)
 #8 0x00000000050af051 Fortran::semantics::CollectNonTbpDefinedIoGenericInterfaces(Fortran::semantics::Scope const&, bool) (/cefs/d9/d98d5df9f22bfabe4705b57b_clang-llvmflang-trunk-20251228/bin/flang+0x50af051)
 #9 0x00000000050aef5b Fortran::semantics::CollectNonTbpDefinedIoGenericInterfaces(Fortran::semantics::Scope const&, bool) (/cefs/d9/d98d5df9f22bfabe4705b57b_clang-llvmflang-trunk-20251228/bin/flang+0x50aef5b)
#10 0x0000000004cac424 genOutputItemList(Fortran::lower::AbstractConverter&, mlir::Value, std::__cxx11::list<Fortran::parser::OutputItem, std::allocator<Fortran::parser::OutputItem>> const&, bool, bool, mlir::Value&, bool) IO.cpp:0:0
#11 0x0000000004cb11a1 mlir::Value genDataTransferStmt<false, false, Fortran::parser::PrintStmt>(Fortran::lower::AbstractConverter&, Fortran::parser::PrintStmt const&) IO.cpp:0:0
#12 0x00000000049e923b (anonymous namespace)::FirConverter::genFIR(Fortran::lower::pft::Evaluation&, bool) (.constprop.0) Bridge.cpp:0:0
#13 0x00000000049ee15c (anonymous namespace)::FirConverter::lowerFunc(Fortran::lower::pft::FunctionLikeUnit&) Bridge.cpp:0:0
#14 0x00000000049eff38 Fortran::lower::LoweringBridge::lower(Fortran::parser::Program const&, Fortran::semantics::SemanticsContext const&) (/cefs/d9/d98d5df9f22bfabe4705b57b_clang-llvmflang-trunk-20251228/bin/flang+0x49eff38)
#15 0x000000000485c20c Fortran::frontend::CodeGenAction::beginSourceFileAction() (/cefs/d9/d98d5df9f22bfabe4705b57b_clang-llvmflang-trunk-20251228/bin/flang+0x485c20c)
#16 0x00000000045b1605 Fortran::frontend::FrontendAction::beginSourceFile(Fortran::frontend::CompilerInstance&, Fortran::frontend::FrontendInputFile const&) (/cefs/d9/d98d5df9f22bfabe4705b57b_clang-llvmflang-trunk-20251228/bin/flang+0x45b1605)
#17 0x000000000459e9ff Fortran::frontend::CompilerInstance::executeAction(Fortran::frontend::FrontendAction&) (/cefs/d9/d98d5df9f22bfabe4705b57b_clang-llvmflang-trunk-20251228/bin/flang+0x459e9ff)
#18 0x00000000045baf53 Fortran::frontend::executeCompilerInvocation(Fortran::frontend::CompilerInstance*) (/cefs/d9/d98d5df9f22bfabe4705b57b_clang-llvmflang-trunk-20251228/bin/flang+0x45baf53)
#19 0x00000000027f4332 fc1_main(llvm::ArrayRef<char const*>, char const*) (/cefs/d9/d98d5df9f22bfabe4705b57b_clang-llvmflang-trunk-20251228/bin/flang+0x27f4332)
#20 0x0000000002693041 main (/cefs/d9/d98d5df9f22bfabe4705b57b_clang-llvmflang-trunk-20251228/bin/flang+0x2693041)
#21 0x000077831ae29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#22 0x000077831ae29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#23 0x00000000027f2de5 _start (/cefs/d9/d98d5df9f22bfabe4705b57b_clang-llvmflang-trunk-20251228/bin/flang+0x27f2de5)
flang-22: error: unable to execute command: Aborted (core dumped)
flang-22: error: flang frontend command failed due to signal (use -v to see invocation)
flang version 22.0.0git (https://github.com/llvm/llvm-project.git f59e2b20ead28738ea6350a922ed1867f5d47139)
Target: x86_64-unknown-linux-gnu
Thread model: posix
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-flang-frontend

Author: Edvin Olofsson (edvinolo)

<details>
Reproducer:
https://gcc.godbolt.org/z/nb7WEczr5

``` fortran
module m_1
implicit none

type :: s
    sequence
    integer :: i
end type

interface s
end interface

interface write(formatted)
    module procedure :: write_formatted
end interface

contains

subroutine write_formatted(t, unit, iotype, v_list, iostat, iomsg)
    type(s), intent(in) :: t
    integer, intent(in) :: unit
    character(len=*), intent(in) :: iotype
    integer, intent(in) :: v_list(:)
    integer, intent(out) :: iostat
    character(len=*), intent(inout) :: iomsg
end subroutine

subroutine f
    type(s) :: t
    print *, t
end subroutine

end module m_1
```

Backtrace:
```
fatal internal error: CHECK(typeSymbol_.has&lt;DerivedTypeDetails&gt;()) failed at /root/llvm-project/flang/lib/Semantics/type.cpp(28)
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /cefs/d9/d98d5df9f22bfabe4705b57b_clang-llvmflang-trunk-20251228/bin/flang -fc1 -triple x86_64-unknown-linux-gnu -S -mrelocation-model pic -pic-level 2 -pic-is-pie -target-cpu x86-64 -mllvm -x86-asm-syntax=intel -resource-dir /cefs/d9/d98d5df9f22bfabe4705b57b_clang-llvmflang-trunk-20251228/lib/clang/22 -mframe-pointer=all -o /app/output.s -x f95 /app/example.f90
 #<!-- -->0 0x000000000454e268 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/cefs/d9/d98d5df9f22bfabe4705b57b_clang-llvmflang-trunk-20251228/bin/flang+0x454e268)
 #<!-- -->1 0x000000000454b434 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->2 0x000077831ae42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->3 0x000077831ae969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->4 0x000077831ae42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->5 0x000077831ae287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->6 0x0000000006771009 (/cefs/d9/d98d5df9f22bfabe4705b57b_clang-llvmflang-trunk-20251228/bin/flang+0x6771009)
 #<!-- -->7 0x000000000512dd0f (/cefs/d9/d98d5df9f22bfabe4705b57b_clang-llvmflang-trunk-20251228/bin/flang+0x512dd0f)
 #<!-- -->8 0x00000000050af051 Fortran::semantics::CollectNonTbpDefinedIoGenericInterfaces(Fortran::semantics::Scope const&amp;, bool) (/cefs/d9/d98d5df9f22bfabe4705b57b_clang-llvmflang-trunk-20251228/bin/flang+0x50af051)
 #<!-- -->9 0x00000000050aef5b Fortran::semantics::CollectNonTbpDefinedIoGenericInterfaces(Fortran::semantics::Scope const&amp;, bool) (/cefs/d9/d98d5df9f22bfabe4705b57b_clang-llvmflang-trunk-20251228/bin/flang+0x50aef5b)
#<!-- -->10 0x0000000004cac424 genOutputItemList(Fortran::lower::AbstractConverter&amp;, mlir::Value, std::__cxx11::list&lt;Fortran::parser::OutputItem, std::allocator&lt;Fortran::parser::OutputItem&gt;&gt; const&amp;, bool, bool, mlir::Value&amp;, bool) IO.cpp:0:0
#<!-- -->11 0x0000000004cb11a1 mlir::Value genDataTransferStmt&lt;false, false, Fortran::parser::PrintStmt&gt;(Fortran::lower::AbstractConverter&amp;, Fortran::parser::PrintStmt const&amp;) IO.cpp:0:0
#<!-- -->12 0x00000000049e923b (anonymous namespace)::FirConverter::genFIR(Fortran::lower::pft::Evaluation&amp;, bool) (.constprop.0) Bridge.cpp:0:0
#<!-- -->13 0x00000000049ee15c (anonymous namespace)::FirConverter::lowerFunc(Fortran::lower::pft::FunctionLikeUnit&amp;) Bridge.cpp:0:0
#<!-- -->14 0x00000000049eff38 Fortran::lower::LoweringBridge::lower(Fortran::parser::Program const&amp;, Fortran::semantics::SemanticsContext const&amp;) (/cefs/d9/d98d5df9f22bfabe4705b57b_clang-llvmflang-trunk-20251228/bin/flang+0x49eff38)
#<!-- -->15 0x000000000485c20c Fortran::frontend::CodeGenAction::beginSourceFileAction() (/cefs/d9/d98d5df9f22bfabe4705b57b_clang-llvmflang-trunk-20251228/bin/flang+0x485c20c)
#<!-- -->16 0x00000000045b1605 Fortran::frontend::FrontendAction::beginSourceFile(Fortran::frontend::CompilerInstance&amp;, Fortran::frontend::FrontendInputFile const&amp;) (/cefs/d9/d98d5df9f22bfabe4705b57b_clang-llvmflang-trunk-20251228/bin/flang+0x45b1605)
#<!-- -->17 0x000000000459e9ff Fortran::frontend::CompilerInstance::executeAction(Fortran::frontend::FrontendAction&amp;) (/cefs/d9/d98d5df9f22bfabe4705b57b_clang-llvmflang-trunk-20251228/bin/flang+0x459e9ff)
#<!-- -->18 0x00000000045baf53 Fortran::frontend::executeCompilerInvocation(Fortran::frontend::CompilerInstance*) (/cefs/d9/d98d5df9f22bfabe4705b57b_clang-llvmflang-trunk-20251228/bin/flang+0x45baf53)
#<!-- -->19 0x00000000027f4332 fc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*) (/cefs/d9/d98d5df9f22bfabe4705b57b_clang-llvmflang-trunk-20251228/bin/flang+0x27f4332)
#<!-- -->20 0x0000000002693041 main (/cefs/d9/d98d5df9f22bfabe4705b57b_clang-llvmflang-trunk-20251228/bin/flang+0x2693041)
#<!-- -->21 0x000077831ae29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->22 0x000077831ae29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->23 0x00000000027f2de5 _start (/cefs/d9/d98d5df9f22bfabe4705b57b_clang-llvmflang-trunk-20251228/bin/flang+0x27f2de5)
flang-22: error: unable to execute command: Aborted (core dumped)
flang-22: error: flang frontend command failed due to signal (use -v to see invocation)
flang version 22.0.0git (https://github.com/llvm/llvm-project.git f59e2b20ead28738ea6350a922ed1867f5d47139)
Target: x86_64-unknown-linux-gnu
Thread model: posix
```
</details>


---

