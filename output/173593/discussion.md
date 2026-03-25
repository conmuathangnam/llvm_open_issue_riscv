# [flang] crashes when passing an assumed-rank dummy argument as an actual argument

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/173593
**Status:** Closed
**Labels:** flang:frontend, crash
**Closed Date:** 2025-12-31T20:43:10Z

## Body

Reproducer:
https://godbolt.org/z/cah191ePj
```f90
contains
  subroutine s(x)
    integer :: x(..)
    call ss((x))
  end subroutine

  subroutine ss(y)
    integer :: y(..)
  end subroutine
end
```

Backtrace:
```console
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /cefs/c6/c62ecd9f1e1d1aea4caddd78_clang-llvmflang-trunk-20251225/bin/flang -fc1 -triple x86_64-unknown-linux-gnu -S -mrelocation-model pic -pic-level 2 -pic-is-pie -target-cpu x86-64 -mllvm -x86-asm-syntax=intel -resource-dir /cefs/c6/c62ecd9f1e1d1aea4caddd78_clang-llvmflang-trunk-20251225/lib/clang/22 -mframe-pointer=all -o /app/output.s -x f95 /app/example.f90
 #0 0x000000000454ccd8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/cefs/c6/c62ecd9f1e1d1aea4caddd78_clang-llvmflang-trunk-20251225/bin/flang+0x454ccd8)
 #1 0x0000000004549ea4 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x0000744981a42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x000000000588ff02 (anonymous namespace)::ElementalOpConversion::matchAndRewrite(hlfir::ElementalOp, hlfir::ElementalOpAdaptor, mlir::ConversionPatternRewriter&) const BufferizeHLFIR.cpp:0:0
 #4 0x000000000588aa5d llvm::LogicalResult mlir::ConversionPattern::dispatchTo1To1<mlir::OpConversionPattern<hlfir::ElementalOp>, hlfir::ElementalOp>(mlir::OpConversionPattern<hlfir::ElementalOp> const&, hlfir::ElementalOp, hlfir::ElementalOp::GenericAdaptor<llvm::ArrayRef<mlir::ValueRange>>, mlir::ConversionPatternRewriter&) (/cefs/c6/c62ecd9f1e1d1aea4caddd78_clang-llvmflang-trunk-20251225/bin/flang+0x588aa5d)
 #5 0x000000000588aafb mlir::OpConversionPattern<hlfir::ElementalOp>::matchAndRewrite(mlir::Operation*, llvm::ArrayRef<mlir::ValueRange>, mlir::ConversionPatternRewriter&) const (/cefs/c6/c62ecd9f1e1d1aea4caddd78_clang-llvmflang-trunk-20251225/bin/flang+0x588aafb)
 #6 0x00000000087a9882 mlir::ConversionPattern::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&) const (/cefs/c6/c62ecd9f1e1d1aea4caddd78_clang-llvmflang-trunk-20251225/bin/flang+0x87a9882)
 #7 0x00000000087e8a74 mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&, llvm::function_ref<bool (mlir::Pattern const&)>, llvm::function_ref<void (mlir::Pattern const&)>, llvm::function_ref<llvm::LogicalResult (mlir::Pattern const&)>) (/cefs/c6/c62ecd9f1e1d1aea4caddd78_clang-llvmflang-trunk-20251225/bin/flang+0x87e8a74)
 #8 0x00000000087ac140 (anonymous namespace)::OperationLegalizer::legalize(mlir::Operation*) DialectConversion.cpp:0:0
 #9 0x00000000087ac22b mlir::OperationConverter::convert(mlir::Operation*, bool) (/cefs/c6/c62ecd9f1e1d1aea4caddd78_clang-llvmflang-trunk-20251225/bin/flang+0x87ac22b)
#10 0x00000000087b0faa mlir::OperationConverter::applyConversion(llvm::ArrayRef<mlir::Operation*>) (/cefs/c6/c62ecd9f1e1d1aea4caddd78_clang-llvmflang-trunk-20251225/bin/flang+0x87b0faa)
#11 0x00000000087b278c applyConversion(llvm::ArrayRef<mlir::Operation*>, mlir::ConversionTarget const&, mlir::FrozenRewritePatternSet const&, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode) DialectConversion.cpp:0:0
#12 0x00000000087b29b5 mlir::applyFullConversion(mlir::Operation*, mlir::ConversionTarget const&, mlir::FrozenRewritePatternSet const&, mlir::ConversionConfig) (/cefs/c6/c62ecd9f1e1d1aea4caddd78_clang-llvmflang-trunk-20251225/bin/flang+0x87b29b5)
#13 0x0000000005887b00 (anonymous namespace)::BufferizeHLFIR::runOnOperation() BufferizeHLFIR.cpp:0:0
#14 0x000000000a3bd6b9 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) (/cefs/c6/c62ecd9f1e1d1aea4caddd78_clang-llvmflang-trunk-20251225/bin/flang+0xa3bd6b9)
#15 0x000000000a3bd929 mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) (/cefs/c6/c62ecd9f1e1d1aea4caddd78_clang-llvmflang-trunk-20251225/bin/flang+0xa3bd929)
#16 0x000000000a3be569 mlir::PassManager::run(mlir::Operation*) (/cefs/c6/c62ecd9f1e1d1aea4caddd78_clang-llvmflang-trunk-20251225/bin/flang+0xa3be569)
#17 0x000000000485d305 Fortran::frontend::CodeGenAction::generateLLVMIR() (/cefs/c6/c62ecd9f1e1d1aea4caddd78_clang-llvmflang-trunk-20251225/bin/flang+0x485d305)
#18 0x0000000004862eb8 Fortran::frontend::CodeGenAction::executeAction() (/cefs/c6/c62ecd9f1e1d1aea4caddd78_clang-llvmflang-trunk-20251225/bin/flang+0x4862eb8)
#19 0x00000000045b0a8d Fortran::frontend::FrontendAction::execute() (/cefs/c6/c62ecd9f1e1d1aea4caddd78_clang-llvmflang-trunk-20251225/bin/flang+0x45b0a8d)
#20 0x000000000459d47f Fortran::frontend::CompilerInstance::executeAction(Fortran::frontend::FrontendAction&) (/cefs/c6/c62ecd9f1e1d1aea4caddd78_clang-llvmflang-trunk-20251225/bin/flang+0x459d47f)
#21 0x00000000045b99c3 Fortran::frontend::executeCompilerInvocation(Fortran::frontend::CompilerInstance*) (/cefs/c6/c62ecd9f1e1d1aea4caddd78_clang-llvmflang-trunk-20251225/bin/flang+0x45b99c3)
#22 0x00000000027f4332 fc1_main(llvm::ArrayRef<char const*>, char const*) (/cefs/c6/c62ecd9f1e1d1aea4caddd78_clang-llvmflang-trunk-20251225/bin/flang+0x27f4332)
#23 0x0000000002693011 main (/cefs/c6/c62ecd9f1e1d1aea4caddd78_clang-llvmflang-trunk-20251225/bin/flang+0x2693011)
#24 0x0000744981a29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#25 0x0000744981a29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#26 0x00000000027f2de5 _start (/cefs/c6/c62ecd9f1e1d1aea4caddd78_clang-llvmflang-trunk-20251225/bin/flang+0x27f2de5)
flang-22: error: unable to execute command: Segmentation fault (core dumped)
flang-22: error: flang frontend command failed due to signal (use -v to see invocation)
flang version 22.0.0git (https://github.com/llvm/llvm-project.git 6785c40bfd9b59bf7cc410a86962e49caea29f9f)
Target: x86_64-unknown-linux-gnu
Thread model: posix
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-flang-ir

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/cah191ePj
```f90
contains
  subroutine s(x)
    integer :: x(..)
    call ss((x))
  end subroutine

  subroutine ss(y)
    integer :: y(..)
  end subroutine
end
```

Backtrace:
```console
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /cefs/c6/c62ecd9f1e1d1aea4caddd78_clang-llvmflang-trunk-20251225/bin/flang -fc1 -triple x86_64-unknown-linux-gnu -S -mrelocation-model pic -pic-level 2 -pic-is-pie -target-cpu x86-64 -mllvm -x86-asm-syntax=intel -resource-dir /cefs/c6/c62ecd9f1e1d1aea4caddd78_clang-llvmflang-trunk-20251225/lib/clang/22 -mframe-pointer=all -o /app/output.s -x f95 /app/example.f90
 #<!-- -->0 0x000000000454ccd8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/cefs/c6/c62ecd9f1e1d1aea4caddd78_clang-llvmflang-trunk-20251225/bin/flang+0x454ccd8)
 #<!-- -->1 0x0000000004549ea4 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->2 0x0000744981a42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->3 0x000000000588ff02 (anonymous namespace)::ElementalOpConversion::matchAndRewrite(hlfir::ElementalOp, hlfir::ElementalOpAdaptor, mlir::ConversionPatternRewriter&amp;) const BufferizeHLFIR.cpp:0:0
 #<!-- -->4 0x000000000588aa5d llvm::LogicalResult mlir::ConversionPattern::dispatchTo1To1&lt;mlir::OpConversionPattern&lt;hlfir::ElementalOp&gt;, hlfir::ElementalOp&gt;(mlir::OpConversionPattern&lt;hlfir::ElementalOp&gt; const&amp;, hlfir::ElementalOp, hlfir::ElementalOp::GenericAdaptor&lt;llvm::ArrayRef&lt;mlir::ValueRange&gt;&gt;, mlir::ConversionPatternRewriter&amp;) (/cefs/c6/c62ecd9f1e1d1aea4caddd78_clang-llvmflang-trunk-20251225/bin/flang+0x588aa5d)
 #<!-- -->5 0x000000000588aafb mlir::OpConversionPattern&lt;hlfir::ElementalOp&gt;::matchAndRewrite(mlir::Operation*, llvm::ArrayRef&lt;mlir::ValueRange&gt;, mlir::ConversionPatternRewriter&amp;) const (/cefs/c6/c62ecd9f1e1d1aea4caddd78_clang-llvmflang-trunk-20251225/bin/flang+0x588aafb)
 #<!-- -->6 0x00000000087a9882 mlir::ConversionPattern::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;) const (/cefs/c6/c62ecd9f1e1d1aea4caddd78_clang-llvmflang-trunk-20251225/bin/flang+0x87a9882)
 #<!-- -->7 0x00000000087e8a74 mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;) (/cefs/c6/c62ecd9f1e1d1aea4caddd78_clang-llvmflang-trunk-20251225/bin/flang+0x87e8a74)
 #<!-- -->8 0x00000000087ac140 (anonymous namespace)::OperationLegalizer::legalize(mlir::Operation*) DialectConversion.cpp:0:0
 #<!-- -->9 0x00000000087ac22b mlir::OperationConverter::convert(mlir::Operation*, bool) (/cefs/c6/c62ecd9f1e1d1aea4caddd78_clang-llvmflang-trunk-20251225/bin/flang+0x87ac22b)
#<!-- -->10 0x00000000087b0faa mlir::OperationConverter::applyConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;) (/cefs/c6/c62ecd9f1e1d1aea4caddd78_clang-llvmflang-trunk-20251225/bin/flang+0x87b0faa)
#<!-- -->11 0x00000000087b278c applyConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;, mlir::ConversionTarget const&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode) DialectConversion.cpp:0:0
#<!-- -->12 0x00000000087b29b5 mlir::applyFullConversion(mlir::Operation*, mlir::ConversionTarget const&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::ConversionConfig) (/cefs/c6/c62ecd9f1e1d1aea4caddd78_clang-llvmflang-trunk-20251225/bin/flang+0x87b29b5)
#<!-- -->13 0x0000000005887b00 (anonymous namespace)::BufferizeHLFIR::runOnOperation() BufferizeHLFIR.cpp:0:0
#<!-- -->14 0x000000000a3bd6b9 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) (/cefs/c6/c62ecd9f1e1d1aea4caddd78_clang-llvmflang-trunk-20251225/bin/flang+0xa3bd6b9)
#<!-- -->15 0x000000000a3bd929 mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) (/cefs/c6/c62ecd9f1e1d1aea4caddd78_clang-llvmflang-trunk-20251225/bin/flang+0xa3bd929)
#<!-- -->16 0x000000000a3be569 mlir::PassManager::run(mlir::Operation*) (/cefs/c6/c62ecd9f1e1d1aea4caddd78_clang-llvmflang-trunk-20251225/bin/flang+0xa3be569)
#<!-- -->17 0x000000000485d305 Fortran::frontend::CodeGenAction::generateLLVMIR() (/cefs/c6/c62ecd9f1e1d1aea4caddd78_clang-llvmflang-trunk-20251225/bin/flang+0x485d305)
#<!-- -->18 0x0000000004862eb8 Fortran::frontend::CodeGenAction::executeAction() (/cefs/c6/c62ecd9f1e1d1aea4caddd78_clang-llvmflang-trunk-20251225/bin/flang+0x4862eb8)
#<!-- -->19 0x00000000045b0a8d Fortran::frontend::FrontendAction::execute() (/cefs/c6/c62ecd9f1e1d1aea4caddd78_clang-llvmflang-trunk-20251225/bin/flang+0x45b0a8d)
#<!-- -->20 0x000000000459d47f Fortran::frontend::CompilerInstance::executeAction(Fortran::frontend::FrontendAction&amp;) (/cefs/c6/c62ecd9f1e1d1aea4caddd78_clang-llvmflang-trunk-20251225/bin/flang+0x459d47f)
#<!-- -->21 0x00000000045b99c3 Fortran::frontend::executeCompilerInvocation(Fortran::frontend::CompilerInstance*) (/cefs/c6/c62ecd9f1e1d1aea4caddd78_clang-llvmflang-trunk-20251225/bin/flang+0x45b99c3)
#<!-- -->22 0x00000000027f4332 fc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*) (/cefs/c6/c62ecd9f1e1d1aea4caddd78_clang-llvmflang-trunk-20251225/bin/flang+0x27f4332)
#<!-- -->23 0x0000000002693011 main (/cefs/c6/c62ecd9f1e1d1aea4caddd78_clang-llvmflang-trunk-20251225/bin/flang+0x2693011)
#<!-- -->24 0x0000744981a29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->25 0x0000744981a29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->26 0x00000000027f2de5 _start (/cefs/c6/c62ecd9f1e1d1aea4caddd78_clang-llvmflang-trunk-20251225/bin/flang+0x27f2de5)
flang-22: error: unable to execute command: Segmentation fault (core dumped)
flang-22: error: flang frontend command failed due to signal (use -v to see invocation)
flang version 22.0.0git (https://github.com/llvm/llvm-project.git 6785c40bfd9b59bf7cc410a86962e49caea29f9f)
Target: x86_64-unknown-linux-gnu
Thread model: posix
```
</details>


---

### Comment 2 - eugeneepshteyn

Because of extra parens around `x` in `call ss((x))`, the assumed rank array should be passed as an expression. This seems to violate C840 in Fortran 2023.

I believe flang should return error here and not invoke lowering.

gfortran outputs the following error:
```
repro-1.f90:4:13:

    4 |     call ss((x))
      |             1
Error: Assumed-rank variable x at (1) may only be used as actual argument
```

---

### Comment 3 - llvmbot


@llvm/issue-subscribers-flang-frontend

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/cah191ePj
```f90
contains
  subroutine s(x)
    integer :: x(..)
    call ss((x))
  end subroutine

  subroutine ss(y)
    integer :: y(..)
  end subroutine
end
```

Backtrace:
```console
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /cefs/c6/c62ecd9f1e1d1aea4caddd78_clang-llvmflang-trunk-20251225/bin/flang -fc1 -triple x86_64-unknown-linux-gnu -S -mrelocation-model pic -pic-level 2 -pic-is-pie -target-cpu x86-64 -mllvm -x86-asm-syntax=intel -resource-dir /cefs/c6/c62ecd9f1e1d1aea4caddd78_clang-llvmflang-trunk-20251225/lib/clang/22 -mframe-pointer=all -o /app/output.s -x f95 /app/example.f90
 #<!-- -->0 0x000000000454ccd8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/cefs/c6/c62ecd9f1e1d1aea4caddd78_clang-llvmflang-trunk-20251225/bin/flang+0x454ccd8)
 #<!-- -->1 0x0000000004549ea4 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->2 0x0000744981a42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->3 0x000000000588ff02 (anonymous namespace)::ElementalOpConversion::matchAndRewrite(hlfir::ElementalOp, hlfir::ElementalOpAdaptor, mlir::ConversionPatternRewriter&amp;) const BufferizeHLFIR.cpp:0:0
 #<!-- -->4 0x000000000588aa5d llvm::LogicalResult mlir::ConversionPattern::dispatchTo1To1&lt;mlir::OpConversionPattern&lt;hlfir::ElementalOp&gt;, hlfir::ElementalOp&gt;(mlir::OpConversionPattern&lt;hlfir::ElementalOp&gt; const&amp;, hlfir::ElementalOp, hlfir::ElementalOp::GenericAdaptor&lt;llvm::ArrayRef&lt;mlir::ValueRange&gt;&gt;, mlir::ConversionPatternRewriter&amp;) (/cefs/c6/c62ecd9f1e1d1aea4caddd78_clang-llvmflang-trunk-20251225/bin/flang+0x588aa5d)
 #<!-- -->5 0x000000000588aafb mlir::OpConversionPattern&lt;hlfir::ElementalOp&gt;::matchAndRewrite(mlir::Operation*, llvm::ArrayRef&lt;mlir::ValueRange&gt;, mlir::ConversionPatternRewriter&amp;) const (/cefs/c6/c62ecd9f1e1d1aea4caddd78_clang-llvmflang-trunk-20251225/bin/flang+0x588aafb)
 #<!-- -->6 0x00000000087a9882 mlir::ConversionPattern::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;) const (/cefs/c6/c62ecd9f1e1d1aea4caddd78_clang-llvmflang-trunk-20251225/bin/flang+0x87a9882)
 #<!-- -->7 0x00000000087e8a74 mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;) (/cefs/c6/c62ecd9f1e1d1aea4caddd78_clang-llvmflang-trunk-20251225/bin/flang+0x87e8a74)
 #<!-- -->8 0x00000000087ac140 (anonymous namespace)::OperationLegalizer::legalize(mlir::Operation*) DialectConversion.cpp:0:0
 #<!-- -->9 0x00000000087ac22b mlir::OperationConverter::convert(mlir::Operation*, bool) (/cefs/c6/c62ecd9f1e1d1aea4caddd78_clang-llvmflang-trunk-20251225/bin/flang+0x87ac22b)
#<!-- -->10 0x00000000087b0faa mlir::OperationConverter::applyConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;) (/cefs/c6/c62ecd9f1e1d1aea4caddd78_clang-llvmflang-trunk-20251225/bin/flang+0x87b0faa)
#<!-- -->11 0x00000000087b278c applyConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;, mlir::ConversionTarget const&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode) DialectConversion.cpp:0:0
#<!-- -->12 0x00000000087b29b5 mlir::applyFullConversion(mlir::Operation*, mlir::ConversionTarget const&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::ConversionConfig) (/cefs/c6/c62ecd9f1e1d1aea4caddd78_clang-llvmflang-trunk-20251225/bin/flang+0x87b29b5)
#<!-- -->13 0x0000000005887b00 (anonymous namespace)::BufferizeHLFIR::runOnOperation() BufferizeHLFIR.cpp:0:0
#<!-- -->14 0x000000000a3bd6b9 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) (/cefs/c6/c62ecd9f1e1d1aea4caddd78_clang-llvmflang-trunk-20251225/bin/flang+0xa3bd6b9)
#<!-- -->15 0x000000000a3bd929 mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) (/cefs/c6/c62ecd9f1e1d1aea4caddd78_clang-llvmflang-trunk-20251225/bin/flang+0xa3bd929)
#<!-- -->16 0x000000000a3be569 mlir::PassManager::run(mlir::Operation*) (/cefs/c6/c62ecd9f1e1d1aea4caddd78_clang-llvmflang-trunk-20251225/bin/flang+0xa3be569)
#<!-- -->17 0x000000000485d305 Fortran::frontend::CodeGenAction::generateLLVMIR() (/cefs/c6/c62ecd9f1e1d1aea4caddd78_clang-llvmflang-trunk-20251225/bin/flang+0x485d305)
#<!-- -->18 0x0000000004862eb8 Fortran::frontend::CodeGenAction::executeAction() (/cefs/c6/c62ecd9f1e1d1aea4caddd78_clang-llvmflang-trunk-20251225/bin/flang+0x4862eb8)
#<!-- -->19 0x00000000045b0a8d Fortran::frontend::FrontendAction::execute() (/cefs/c6/c62ecd9f1e1d1aea4caddd78_clang-llvmflang-trunk-20251225/bin/flang+0x45b0a8d)
#<!-- -->20 0x000000000459d47f Fortran::frontend::CompilerInstance::executeAction(Fortran::frontend::FrontendAction&amp;) (/cefs/c6/c62ecd9f1e1d1aea4caddd78_clang-llvmflang-trunk-20251225/bin/flang+0x459d47f)
#<!-- -->21 0x00000000045b99c3 Fortran::frontend::executeCompilerInvocation(Fortran::frontend::CompilerInstance*) (/cefs/c6/c62ecd9f1e1d1aea4caddd78_clang-llvmflang-trunk-20251225/bin/flang+0x45b99c3)
#<!-- -->22 0x00000000027f4332 fc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*) (/cefs/c6/c62ecd9f1e1d1aea4caddd78_clang-llvmflang-trunk-20251225/bin/flang+0x27f4332)
#<!-- -->23 0x0000000002693011 main (/cefs/c6/c62ecd9f1e1d1aea4caddd78_clang-llvmflang-trunk-20251225/bin/flang+0x2693011)
#<!-- -->24 0x0000744981a29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->25 0x0000744981a29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->26 0x00000000027f2de5 _start (/cefs/c6/c62ecd9f1e1d1aea4caddd78_clang-llvmflang-trunk-20251225/bin/flang+0x27f2de5)
flang-22: error: unable to execute command: Segmentation fault (core dumped)
flang-22: error: flang frontend command failed due to signal (use -v to see invocation)
flang version 22.0.0git (https://github.com/llvm/llvm-project.git 6785c40bfd9b59bf7cc410a86962e49caea29f9f)
Target: x86_64-unknown-linux-gnu
Thread model: posix
```
</details>


---

