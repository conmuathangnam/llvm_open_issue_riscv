# [Flang] crashes when using ieee function with assumed-rank arrays

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/159555
**Status:** Closed
**Labels:** flang:frontend, crash
**Closed Date:** 2025-09-23T22:45:19Z

## Body

Reproducer:
https://godbolt.org/z/P6oEsor1x
```f90
real :: r1(10), r2(10)
call s(r1, r2)
contains
  subroutine s(x, y)
    use, intrinsic :: ieee_arithmetic
    real :: x(..), y(..)
    print *, ieee_rem(x, y)
  end subroutine
end
```

Backtrace:
```console
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /cefs/93/93e8d16aa784b1659a61a5b2_clang-llvmflang-trunk-20250918/bin/flang -fc1 -triple x86_64-unknown-linux-gnu -S -mrelocation-model pic -pic-level 2 -pic-is-pie -target-cpu x86-64 -mllvm -x86-asm-syntax=intel -resource-dir /cefs/93/93e8d16aa784b1659a61a5b2_clang-llvmflang-trunk-20250918/lib/clang/22 -mframe-pointer=all -o /app/output.s -x f95 /app/example.f90
 #0 0x0000000004238cc8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/cefs/93/93e8d16aa784b1659a61a5b2_clang-llvmflang-trunk-20250918/bin/flang+0x4238cc8)
 #1 0x0000000004235e94 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x00007736e2a42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x00000000054e19c2 (anonymous namespace)::ElementalOpConversion::matchAndRewrite(hlfir::ElementalOp, hlfir::ElementalOpAdaptor, mlir::ConversionPatternRewriter&) const BufferizeHLFIR.cpp:0:0
 #4 0x00000000054dc55d llvm::LogicalResult mlir::ConversionPattern::dispatchTo1To1<mlir::OpConversionPattern<hlfir::ElementalOp>, hlfir::ElementalOp>(mlir::OpConversionPattern<hlfir::ElementalOp> const&, hlfir::ElementalOp, hlfir::ElementalOp::GenericAdaptor<llvm::ArrayRef<mlir::ValueRange>>, mlir::ConversionPatternRewriter&) (/cefs/93/93e8d16aa784b1659a61a5b2_clang-llvmflang-trunk-20250918/bin/flang+0x54dc55d)
 #5 0x00000000054dc5fb mlir::OpConversionPattern<hlfir::ElementalOp>::matchAndRewrite(mlir::Operation*, llvm::ArrayRef<mlir::ValueRange>, mlir::ConversionPatternRewriter&) const (/cefs/93/93e8d16aa784b1659a61a5b2_clang-llvmflang-trunk-20250918/bin/flang+0x54dc5fb)
 #6 0x000000000813c3f2 mlir::ConversionPattern::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&) const (/cefs/93/93e8d16aa784b1659a61a5b2_clang-llvmflang-trunk-20250918/bin/flang+0x813c3f2)
 #7 0x0000000008187e54 mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&, llvm::function_ref<bool (mlir::Pattern const&)>, llvm::function_ref<void (mlir::Pattern const&)>, llvm::function_ref<llvm::LogicalResult (mlir::Pattern const&)>) (/cefs/93/93e8d16aa784b1659a61a5b2_clang-llvmflang-trunk-20250918/bin/flang+0x8187e54)
 #8 0x0000000008141562 (anonymous namespace)::OperationLegalizer::legalize(mlir::Operation*) DialectConversion.cpp:0:0
 #9 0x0000000008141699 mlir::OperationConverter::convert(mlir::Operation*) (/cefs/93/93e8d16aa784b1659a61a5b2_clang-llvmflang-trunk-20250918/bin/flang+0x8141699)
#10 0x0000000008141d40 mlir::OperationConverter::convertOperations(llvm::ArrayRef<mlir::Operation*>) (/cefs/93/93e8d16aa784b1659a61a5b2_clang-llvmflang-trunk-20250918/bin/flang+0x8141d40)
#11 0x0000000008143e26 applyConversion(llvm::ArrayRef<mlir::Operation*>, mlir::ConversionTarget const&, mlir::FrozenRewritePatternSet const&, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode) DialectConversion.cpp:0:0
#12 0x0000000008144055 mlir::applyFullConversion(mlir::Operation*, mlir::ConversionTarget const&, mlir::FrozenRewritePatternSet const&, mlir::ConversionConfig) (/cefs/93/93e8d16aa784b1659a61a5b2_clang-llvmflang-trunk-20250918/bin/flang+0x8144055)
#13 0x00000000054d95b0 (anonymous namespace)::BufferizeHLFIR::runOnOperation() BufferizeHLFIR.cpp:0:0
#14 0x0000000009e6b6a1 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) (/cefs/93/93e8d16aa784b1659a61a5b2_clang-llvmflang-trunk-20250918/bin/flang+0x9e6b6a1)
#15 0x0000000009e6bb19 mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) (/cefs/93/93e8d16aa784b1659a61a5b2_clang-llvmflang-trunk-20250918/bin/flang+0x9e6bb19)
#16 0x0000000009e6c579 mlir::PassManager::run(mlir::Operation*) (/cefs/93/93e8d16aa784b1659a61a5b2_clang-llvmflang-trunk-20250918/bin/flang+0x9e6c579)
#17 0x00000000045378c1 Fortran::frontend::CodeGenAction::generateLLVMIR() (/cefs/93/93e8d16aa784b1659a61a5b2_clang-llvmflang-trunk-20250918/bin/flang+0x45378c1)
#18 0x000000000453cf38 Fortran::frontend::CodeGenAction::executeAction() (/cefs/93/93e8d16aa784b1659a61a5b2_clang-llvmflang-trunk-20250918/bin/flang+0x453cf38)
#19 0x00000000042974bd Fortran::frontend::FrontendAction::execute() (/cefs/93/93e8d16aa784b1659a61a5b2_clang-llvmflang-trunk-20250918/bin/flang+0x42974bd)
#20 0x000000000428444f Fortran::frontend::CompilerInstance::executeAction(Fortran::frontend::FrontendAction&) (/cefs/93/93e8d16aa784b1659a61a5b2_clang-llvmflang-trunk-20250918/bin/flang+0x428444f)
#21 0x00000000042a044b Fortran::frontend::executeCompilerInvocation(Fortran::frontend::CompilerInstance*) (/cefs/93/93e8d16aa784b1659a61a5b2_clang-llvmflang-trunk-20250918/bin/flang+0x42a044b)
#22 0x0000000002597ce2 fc1_main(llvm::ArrayRef<char const*>, char const*) (/cefs/93/93e8d16aa784b1659a61a5b2_clang-llvmflang-trunk-20250918/bin/flang+0x2597ce2)
#23 0x000000000242ee91 main (/cefs/93/93e8d16aa784b1659a61a5b2_clang-llvmflang-trunk-20250918/bin/flang+0x242ee91)
#24 0x00007736e2a29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#25 0x00007736e2a29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#26 0x00000000025966d5 _start (/cefs/93/93e8d16aa784b1659a61a5b2_clang-llvmflang-trunk-20250918/bin/flang+0x25966d5)
flang-22: error: unable to execute command: Segmentation fault (core dumped)
flang-22: error: flang frontend command failed due to signal (use -v to see invocation)
flang version 22.0.0git (https://github.com/llvm/llvm-project.git 38f2a1cb9b25ef2967661865458d6a919ce82aaa)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /cefs/93/93e8d16aa784b1659a61a5b2_clang-llvmflang-trunk-20250918/bin
flang-22: note: diagnostic msg: 
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-flang-ir

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/P6oEsor1x
```f90
real :: r1(10), r2(10)
call s(r1, r2)
contains
  subroutine s(x, y)
    use, intrinsic :: ieee_arithmetic
    real :: x(..), y(..)
    print *, ieee_rem(x, y)
  end subroutine
end
```

Backtrace:
```console
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /cefs/93/93e8d16aa784b1659a61a5b2_clang-llvmflang-trunk-20250918/bin/flang -fc1 -triple x86_64-unknown-linux-gnu -S -mrelocation-model pic -pic-level 2 -pic-is-pie -target-cpu x86-64 -mllvm -x86-asm-syntax=intel -resource-dir /cefs/93/93e8d16aa784b1659a61a5b2_clang-llvmflang-trunk-20250918/lib/clang/22 -mframe-pointer=all -o /app/output.s -x f95 /app/example.f90
 #<!-- -->0 0x0000000004238cc8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/cefs/93/93e8d16aa784b1659a61a5b2_clang-llvmflang-trunk-20250918/bin/flang+0x4238cc8)
 #<!-- -->1 0x0000000004235e94 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->2 0x00007736e2a42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->3 0x00000000054e19c2 (anonymous namespace)::ElementalOpConversion::matchAndRewrite(hlfir::ElementalOp, hlfir::ElementalOpAdaptor, mlir::ConversionPatternRewriter&amp;) const BufferizeHLFIR.cpp:0:0
 #<!-- -->4 0x00000000054dc55d llvm::LogicalResult mlir::ConversionPattern::dispatchTo1To1&lt;mlir::OpConversionPattern&lt;hlfir::ElementalOp&gt;, hlfir::ElementalOp&gt;(mlir::OpConversionPattern&lt;hlfir::ElementalOp&gt; const&amp;, hlfir::ElementalOp, hlfir::ElementalOp::GenericAdaptor&lt;llvm::ArrayRef&lt;mlir::ValueRange&gt;&gt;, mlir::ConversionPatternRewriter&amp;) (/cefs/93/93e8d16aa784b1659a61a5b2_clang-llvmflang-trunk-20250918/bin/flang+0x54dc55d)
 #<!-- -->5 0x00000000054dc5fb mlir::OpConversionPattern&lt;hlfir::ElementalOp&gt;::matchAndRewrite(mlir::Operation*, llvm::ArrayRef&lt;mlir::ValueRange&gt;, mlir::ConversionPatternRewriter&amp;) const (/cefs/93/93e8d16aa784b1659a61a5b2_clang-llvmflang-trunk-20250918/bin/flang+0x54dc5fb)
 #<!-- -->6 0x000000000813c3f2 mlir::ConversionPattern::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;) const (/cefs/93/93e8d16aa784b1659a61a5b2_clang-llvmflang-trunk-20250918/bin/flang+0x813c3f2)
 #<!-- -->7 0x0000000008187e54 mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;) (/cefs/93/93e8d16aa784b1659a61a5b2_clang-llvmflang-trunk-20250918/bin/flang+0x8187e54)
 #<!-- -->8 0x0000000008141562 (anonymous namespace)::OperationLegalizer::legalize(mlir::Operation*) DialectConversion.cpp:0:0
 #<!-- -->9 0x0000000008141699 mlir::OperationConverter::convert(mlir::Operation*) (/cefs/93/93e8d16aa784b1659a61a5b2_clang-llvmflang-trunk-20250918/bin/flang+0x8141699)
#<!-- -->10 0x0000000008141d40 mlir::OperationConverter::convertOperations(llvm::ArrayRef&lt;mlir::Operation*&gt;) (/cefs/93/93e8d16aa784b1659a61a5b2_clang-llvmflang-trunk-20250918/bin/flang+0x8141d40)
#<!-- -->11 0x0000000008143e26 applyConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;, mlir::ConversionTarget const&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode) DialectConversion.cpp:0:0
#<!-- -->12 0x0000000008144055 mlir::applyFullConversion(mlir::Operation*, mlir::ConversionTarget const&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::ConversionConfig) (/cefs/93/93e8d16aa784b1659a61a5b2_clang-llvmflang-trunk-20250918/bin/flang+0x8144055)
#<!-- -->13 0x00000000054d95b0 (anonymous namespace)::BufferizeHLFIR::runOnOperation() BufferizeHLFIR.cpp:0:0
#<!-- -->14 0x0000000009e6b6a1 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) (/cefs/93/93e8d16aa784b1659a61a5b2_clang-llvmflang-trunk-20250918/bin/flang+0x9e6b6a1)
#<!-- -->15 0x0000000009e6bb19 mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) (/cefs/93/93e8d16aa784b1659a61a5b2_clang-llvmflang-trunk-20250918/bin/flang+0x9e6bb19)
#<!-- -->16 0x0000000009e6c579 mlir::PassManager::run(mlir::Operation*) (/cefs/93/93e8d16aa784b1659a61a5b2_clang-llvmflang-trunk-20250918/bin/flang+0x9e6c579)
#<!-- -->17 0x00000000045378c1 Fortran::frontend::CodeGenAction::generateLLVMIR() (/cefs/93/93e8d16aa784b1659a61a5b2_clang-llvmflang-trunk-20250918/bin/flang+0x45378c1)
#<!-- -->18 0x000000000453cf38 Fortran::frontend::CodeGenAction::executeAction() (/cefs/93/93e8d16aa784b1659a61a5b2_clang-llvmflang-trunk-20250918/bin/flang+0x453cf38)
#<!-- -->19 0x00000000042974bd Fortran::frontend::FrontendAction::execute() (/cefs/93/93e8d16aa784b1659a61a5b2_clang-llvmflang-trunk-20250918/bin/flang+0x42974bd)
#<!-- -->20 0x000000000428444f Fortran::frontend::CompilerInstance::executeAction(Fortran::frontend::FrontendAction&amp;) (/cefs/93/93e8d16aa784b1659a61a5b2_clang-llvmflang-trunk-20250918/bin/flang+0x428444f)
#<!-- -->21 0x00000000042a044b Fortran::frontend::executeCompilerInvocation(Fortran::frontend::CompilerInstance*) (/cefs/93/93e8d16aa784b1659a61a5b2_clang-llvmflang-trunk-20250918/bin/flang+0x42a044b)
#<!-- -->22 0x0000000002597ce2 fc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*) (/cefs/93/93e8d16aa784b1659a61a5b2_clang-llvmflang-trunk-20250918/bin/flang+0x2597ce2)
#<!-- -->23 0x000000000242ee91 main (/cefs/93/93e8d16aa784b1659a61a5b2_clang-llvmflang-trunk-20250918/bin/flang+0x242ee91)
#<!-- -->24 0x00007736e2a29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->25 0x00007736e2a29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->26 0x00000000025966d5 _start (/cefs/93/93e8d16aa784b1659a61a5b2_clang-llvmflang-trunk-20250918/bin/flang+0x25966d5)
flang-22: error: unable to execute command: Segmentation fault (core dumped)
flang-22: error: flang frontend command failed due to signal (use -v to see invocation)
flang version 22.0.0git (https://github.com/llvm/llvm-project.git 38f2a1cb9b25ef2967661865458d6a919ce82aaa)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /cefs/93/93e8d16aa784b1659a61a5b2_clang-llvmflang-trunk-20250918/bin
flang-22: note: diagnostic msg: 
```
</details>


---

### Comment 2 - klausler

Assumed-rank dummy arrays are almost never acceptable actual arguments to intrinsic procedures, including this one; will add a missing check.

---

### Comment 3 - llvmbot


@llvm/issue-subscribers-flang-frontend

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/P6oEsor1x
```f90
real :: r1(10), r2(10)
call s(r1, r2)
contains
  subroutine s(x, y)
    use, intrinsic :: ieee_arithmetic
    real :: x(..), y(..)
    print *, ieee_rem(x, y)
  end subroutine
end
```

Backtrace:
```console
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /cefs/93/93e8d16aa784b1659a61a5b2_clang-llvmflang-trunk-20250918/bin/flang -fc1 -triple x86_64-unknown-linux-gnu -S -mrelocation-model pic -pic-level 2 -pic-is-pie -target-cpu x86-64 -mllvm -x86-asm-syntax=intel -resource-dir /cefs/93/93e8d16aa784b1659a61a5b2_clang-llvmflang-trunk-20250918/lib/clang/22 -mframe-pointer=all -o /app/output.s -x f95 /app/example.f90
 #<!-- -->0 0x0000000004238cc8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/cefs/93/93e8d16aa784b1659a61a5b2_clang-llvmflang-trunk-20250918/bin/flang+0x4238cc8)
 #<!-- -->1 0x0000000004235e94 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->2 0x00007736e2a42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->3 0x00000000054e19c2 (anonymous namespace)::ElementalOpConversion::matchAndRewrite(hlfir::ElementalOp, hlfir::ElementalOpAdaptor, mlir::ConversionPatternRewriter&amp;) const BufferizeHLFIR.cpp:0:0
 #<!-- -->4 0x00000000054dc55d llvm::LogicalResult mlir::ConversionPattern::dispatchTo1To1&lt;mlir::OpConversionPattern&lt;hlfir::ElementalOp&gt;, hlfir::ElementalOp&gt;(mlir::OpConversionPattern&lt;hlfir::ElementalOp&gt; const&amp;, hlfir::ElementalOp, hlfir::ElementalOp::GenericAdaptor&lt;llvm::ArrayRef&lt;mlir::ValueRange&gt;&gt;, mlir::ConversionPatternRewriter&amp;) (/cefs/93/93e8d16aa784b1659a61a5b2_clang-llvmflang-trunk-20250918/bin/flang+0x54dc55d)
 #<!-- -->5 0x00000000054dc5fb mlir::OpConversionPattern&lt;hlfir::ElementalOp&gt;::matchAndRewrite(mlir::Operation*, llvm::ArrayRef&lt;mlir::ValueRange&gt;, mlir::ConversionPatternRewriter&amp;) const (/cefs/93/93e8d16aa784b1659a61a5b2_clang-llvmflang-trunk-20250918/bin/flang+0x54dc5fb)
 #<!-- -->6 0x000000000813c3f2 mlir::ConversionPattern::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;) const (/cefs/93/93e8d16aa784b1659a61a5b2_clang-llvmflang-trunk-20250918/bin/flang+0x813c3f2)
 #<!-- -->7 0x0000000008187e54 mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;) (/cefs/93/93e8d16aa784b1659a61a5b2_clang-llvmflang-trunk-20250918/bin/flang+0x8187e54)
 #<!-- -->8 0x0000000008141562 (anonymous namespace)::OperationLegalizer::legalize(mlir::Operation*) DialectConversion.cpp:0:0
 #<!-- -->9 0x0000000008141699 mlir::OperationConverter::convert(mlir::Operation*) (/cefs/93/93e8d16aa784b1659a61a5b2_clang-llvmflang-trunk-20250918/bin/flang+0x8141699)
#<!-- -->10 0x0000000008141d40 mlir::OperationConverter::convertOperations(llvm::ArrayRef&lt;mlir::Operation*&gt;) (/cefs/93/93e8d16aa784b1659a61a5b2_clang-llvmflang-trunk-20250918/bin/flang+0x8141d40)
#<!-- -->11 0x0000000008143e26 applyConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;, mlir::ConversionTarget const&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode) DialectConversion.cpp:0:0
#<!-- -->12 0x0000000008144055 mlir::applyFullConversion(mlir::Operation*, mlir::ConversionTarget const&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::ConversionConfig) (/cefs/93/93e8d16aa784b1659a61a5b2_clang-llvmflang-trunk-20250918/bin/flang+0x8144055)
#<!-- -->13 0x00000000054d95b0 (anonymous namespace)::BufferizeHLFIR::runOnOperation() BufferizeHLFIR.cpp:0:0
#<!-- -->14 0x0000000009e6b6a1 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) (/cefs/93/93e8d16aa784b1659a61a5b2_clang-llvmflang-trunk-20250918/bin/flang+0x9e6b6a1)
#<!-- -->15 0x0000000009e6bb19 mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) (/cefs/93/93e8d16aa784b1659a61a5b2_clang-llvmflang-trunk-20250918/bin/flang+0x9e6bb19)
#<!-- -->16 0x0000000009e6c579 mlir::PassManager::run(mlir::Operation*) (/cefs/93/93e8d16aa784b1659a61a5b2_clang-llvmflang-trunk-20250918/bin/flang+0x9e6c579)
#<!-- -->17 0x00000000045378c1 Fortran::frontend::CodeGenAction::generateLLVMIR() (/cefs/93/93e8d16aa784b1659a61a5b2_clang-llvmflang-trunk-20250918/bin/flang+0x45378c1)
#<!-- -->18 0x000000000453cf38 Fortran::frontend::CodeGenAction::executeAction() (/cefs/93/93e8d16aa784b1659a61a5b2_clang-llvmflang-trunk-20250918/bin/flang+0x453cf38)
#<!-- -->19 0x00000000042974bd Fortran::frontend::FrontendAction::execute() (/cefs/93/93e8d16aa784b1659a61a5b2_clang-llvmflang-trunk-20250918/bin/flang+0x42974bd)
#<!-- -->20 0x000000000428444f Fortran::frontend::CompilerInstance::executeAction(Fortran::frontend::FrontendAction&amp;) (/cefs/93/93e8d16aa784b1659a61a5b2_clang-llvmflang-trunk-20250918/bin/flang+0x428444f)
#<!-- -->21 0x00000000042a044b Fortran::frontend::executeCompilerInvocation(Fortran::frontend::CompilerInstance*) (/cefs/93/93e8d16aa784b1659a61a5b2_clang-llvmflang-trunk-20250918/bin/flang+0x42a044b)
#<!-- -->22 0x0000000002597ce2 fc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*) (/cefs/93/93e8d16aa784b1659a61a5b2_clang-llvmflang-trunk-20250918/bin/flang+0x2597ce2)
#<!-- -->23 0x000000000242ee91 main (/cefs/93/93e8d16aa784b1659a61a5b2_clang-llvmflang-trunk-20250918/bin/flang+0x242ee91)
#<!-- -->24 0x00007736e2a29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->25 0x00007736e2a29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->26 0x00000000025966d5 _start (/cefs/93/93e8d16aa784b1659a61a5b2_clang-llvmflang-trunk-20250918/bin/flang+0x25966d5)
flang-22: error: unable to execute command: Segmentation fault (core dumped)
flang-22: error: flang frontend command failed due to signal (use -v to see invocation)
flang version 22.0.0git (https://github.com/llvm/llvm-project.git 38f2a1cb9b25ef2967661865458d6a919ce82aaa)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /cefs/93/93e8d16aa784b1659a61a5b2_clang-llvmflang-trunk-20250918/bin
flang-22: note: diagnostic msg: 
```
</details>


---

