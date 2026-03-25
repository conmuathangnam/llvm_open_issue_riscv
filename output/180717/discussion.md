# mlir-opt crash: getTerminator() assertion failure during verification of sparse_tensor.select with empty region

**Author:** xvv4402
**URL:** https://github.com/llvm/llvm-project/issues/180717
**Status:** Closed
**Labels:** mlir, crash
**Closed Date:** 2026-02-11T01:19:40Z

## Body

**Mlir-opt Version**: 22.1.0-rc1

**Bug Op**: sparse_tensor.select

**Bug Pass**: N/A (Crash occurs during IR Verification/Parsing)

**Detailed Description**: The mlir-opt tool crashes when parsing or verifying a sparse_tensor.select operation that contains a region with an empty block (no operations or terminator).

The SelectOp::verify() implementation calls a helper function (likely verifyNumBlockArgs) that attempts to access the block's terminator using getTerminator(). Since the block is empty and has no terminator, it triggers an assertion failure: Assertion mightHaveTerminator() failed. The verifier should check if the block is empty before attempting to retrieve a terminator.

```mlir
module {
  func.func @main() {
    %0 = arm_sme.streaming_vl <byte>
    %1 = arm_sme.streaming_vl <byte>
    %2 = llvm.inline_asm "byte_size", "=r" : () -> i64
    %3 = arm_sme.streaming_vl <byte>
    %4 = index.minu %0, %1
    %5 = arm_sme.streaming_vl <byte>
    %6 = index.remu %4, %5
    %7 = sparse_tensor.select %6 : index {
      ^bb0(%arg0: index):
    }
    return
  }
}
```
```
mlir-opt temp.mlir
```
```
mlir-opt: llvm-project-llvmorg-22.1.0-rc1/mlir/lib/IR/Block.cpp:250: mlir::Operation* mlir::Block::getTerminator(): Assertion `mightHaveTerminator()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llvmorg-22.1.0-install/bin/mlir-opt temp.mlir
 #0 0x0000584055f92e32 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #1 0x0000584055f8f5af llvm::sys::RunSignalHandlers() (llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #2 0x0000584055f8f6fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x00007a7b53045330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x00007a7b5309eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x00007a7b5309eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x00007a7b5309eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x00007a7b5304527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x00007a7b530288ff abort ./stdlib/abort.c:81:7
 #9 0x00007a7b5302881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#10 0x00007a7b5303b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#11 0x000058405dad5c71 mlir::Block::getTerminator() (llvmorg-22.1.0-install/bin/mlir-opt+0xa072c71)
#12 0x000058405c3b6b19 llvm::LogicalResult verifyNumBlockArgs<mlir::sparse_tensor::UnaryOp>(mlir::sparse_tensor::UnaryOp*, mlir::Region&, char const*, mlir::TypeRange, mlir::Type) SparseTensorDialect.cpp:0:0
#13 0x000058405c3b741a mlir::sparse_tensor::SelectOp::verify() (llvmorg-22.1.0-install/bin/mlir-opt+0x895441a)
#14 0x000058405c3d2359 mlir::Op<mlir::sparse_tensor::SelectOp, mlir::OpTrait::OneRegion, mlir::OpTrait::OneResult, mlir::OpTrait::OneTypedResult<mlir::Type>::Impl, mlir::OpTrait::ZeroSuccessors, mlir::OpTrait::OneOperand, mlir::OpTrait::OpInvariants, mlir::ConditionallySpeculatable::Trait, mlir::OpTrait::AlwaysSpeculatableImplTrait, mlir::MemoryEffectOpInterface::Trait, mlir::OpTrait::SameOperandsAndResultType, mlir::InferTypeOpInterface::Trait>::verifyInvariants(mlir::Operation*) (llvmorg-22.1.0-install/bin/mlir-opt+0x896f359)
#15 0x000058405c3de827 mlir::RegisteredOperationName::Model<mlir::sparse_tensor::SelectOp>::verifyInvariants(mlir::Operation*) (llvmorg-22.1.0-install/bin/mlir-opt+0x897b827)
#16 0x000058405dbe576e (anonymous namespace)::OperationVerifier::verifyOnEntrance(mlir::Operation&) (.isra.0) Verifier.cpp:0:0
#17 0x000058405dbe7818 (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&) Verifier.cpp:0:0
#18 0x000058405dbe6ac9 (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&) Verifier.cpp:0:0
#19 0x000058405dbe7799 (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&) Verifier.cpp:0:0
#20 0x000058405dbe6ac9 (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&) Verifier.cpp:0:0
#21 0x000058405dbe7799 (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&) Verifier.cpp:0:0
#22 0x000058405dbe86c8 mlir::verify(mlir::Operation*, bool) (llvmorg-22.1.0-install/bin/mlir-opt+0xa1856c8)
#23 0x000058405c12940a mlir::parseAsmSourceFile(llvm::SourceMgr const&, mlir::Block*, mlir::ParserConfig const&, mlir::AsmParserState*, mlir::AsmParserCodeCompleteContext*) (llvmorg-22.1.0-install/bin/mlir-opt+0x86c640a)
#24 0x0000584056058df7 performActions(llvm::raw_ostream&, std::shared_ptr<llvm::SourceMgr> const&, mlir::MLIRContext*, mlir::MlirOptMainConfig const&) MlirOptMain.cpp:0:0
#25 0x000058405605a4ea llvm::LogicalResult llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::callback_fn<mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::'lambda'(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, llvm::raw_ostream&)>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) MlirOptMain.cpp:0:0
#26 0x000058405dc02731 mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef) (llvmorg-22.1.0-install/bin/mlir-opt+0xa19f731)
#27 0x00005840560518d0 mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&) (.part.0) MlirOptMain.cpp:0:0
#28 0x000058405605ab70 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&) (llvmorg-22.1.0-install/bin/mlir-opt+0x25f7b70)
#29 0x000058405605ada9 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&) (llvmorg-22.1.0-install/bin/mlir-opt+0x25f7da9)
#30 0x0000584055e8c023 main (llvmorg-22.1.0-install/bin/mlir-opt+0x2429023)
#31 0x00007a7b5302a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#32 0x00007a7b5302a28b call_init ./csu/../csu/libc-start.c:128:20
#33 0x00007a7b5302a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#34 0x0000584055f71405 _start (llvmorg-22.1.0-install/bin/mlir-opt+0x250e405)
Aborted (core dumped)
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: None (xvv4402)

<details>
**Mlir-opt Version**: 22.1.0-rc1

**Bug Op**: sparse_tensor.select

**Bug Pass**: N/A (Crash occurs during IR Verification/Parsing)

**Detailed Description**: The mlir-opt tool crashes when parsing or verifying a sparse_tensor.select operation that contains a region with an empty block (no operations or terminator).

The SelectOp::verify() implementation calls a helper function (likely verifyNumBlockArgs) that attempts to access the block's terminator using getTerminator(). Since the block is empty and has no terminator, it triggers an assertion failure: Assertion mightHaveTerminator() failed. The verifier should check if the block is empty before attempting to retrieve a terminator.

```mlir
module {
  func.func @<!-- -->main() {
    %0 = arm_sme.streaming_vl &lt;byte&gt;
    %1 = arm_sme.streaming_vl &lt;byte&gt;
    %2 = llvm.inline_asm "byte_size", "=r" : () -&gt; i64
    %3 = arm_sme.streaming_vl &lt;byte&gt;
    %4 = index.minu %0, %1
    %5 = arm_sme.streaming_vl &lt;byte&gt;
    %6 = index.remu %4, %5
    %7 = sparse_tensor.select %6 : index {
      ^bb0(%arg0: index):
    }
    return
  }
}
```
```
mlir-opt temp.mlir
```
```
mlir-opt: llvm-project-llvmorg-22.1.0-rc1/mlir/lib/IR/Block.cpp:250: mlir::Operation* mlir::Block::getTerminator(): Assertion `mightHaveTerminator()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llvmorg-22.1.0-install/bin/mlir-opt temp.mlir
 #<!-- -->0 0x0000584055f92e32 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #<!-- -->1 0x0000584055f8f5af llvm::sys::RunSignalHandlers() (llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #<!-- -->2 0x0000584055f8f6fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x00007a7b53045330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->4 0x00007a7b5309eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->5 0x00007a7b5309eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->6 0x00007a7b5309eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->7 0x00007a7b5304527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->8 0x00007a7b530288ff abort ./stdlib/abort.c:81:7
 #<!-- -->9 0x00007a7b5302881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->10 0x00007a7b5303b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->11 0x000058405dad5c71 mlir::Block::getTerminator() (llvmorg-22.1.0-install/bin/mlir-opt+0xa072c71)
#<!-- -->12 0x000058405c3b6b19 llvm::LogicalResult verifyNumBlockArgs&lt;mlir::sparse_tensor::UnaryOp&gt;(mlir::sparse_tensor::UnaryOp*, mlir::Region&amp;, char const*, mlir::TypeRange, mlir::Type) SparseTensorDialect.cpp:0:0
#<!-- -->13 0x000058405c3b741a mlir::sparse_tensor::SelectOp::verify() (llvmorg-22.1.0-install/bin/mlir-opt+0x895441a)
#<!-- -->14 0x000058405c3d2359 mlir::Op&lt;mlir::sparse_tensor::SelectOp, mlir::OpTrait::OneRegion, mlir::OpTrait::OneResult, mlir::OpTrait::OneTypedResult&lt;mlir::Type&gt;::Impl, mlir::OpTrait::ZeroSuccessors, mlir::OpTrait::OneOperand, mlir::OpTrait::OpInvariants, mlir::ConditionallySpeculatable::Trait, mlir::OpTrait::AlwaysSpeculatableImplTrait, mlir::MemoryEffectOpInterface::Trait, mlir::OpTrait::SameOperandsAndResultType, mlir::InferTypeOpInterface::Trait&gt;::verifyInvariants(mlir::Operation*) (llvmorg-22.1.0-install/bin/mlir-opt+0x896f359)
#<!-- -->15 0x000058405c3de827 mlir::RegisteredOperationName::Model&lt;mlir::sparse_tensor::SelectOp&gt;::verifyInvariants(mlir::Operation*) (llvmorg-22.1.0-install/bin/mlir-opt+0x897b827)
#<!-- -->16 0x000058405dbe576e (anonymous namespace)::OperationVerifier::verifyOnEntrance(mlir::Operation&amp;) (.isra.0) Verifier.cpp:0:0
#<!-- -->17 0x000058405dbe7818 (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;) Verifier.cpp:0:0
#<!-- -->18 0x000058405dbe6ac9 (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;) Verifier.cpp:0:0
#<!-- -->19 0x000058405dbe7799 (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;) Verifier.cpp:0:0
#<!-- -->20 0x000058405dbe6ac9 (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;) Verifier.cpp:0:0
#<!-- -->21 0x000058405dbe7799 (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;) Verifier.cpp:0:0
#<!-- -->22 0x000058405dbe86c8 mlir::verify(mlir::Operation*, bool) (llvmorg-22.1.0-install/bin/mlir-opt+0xa1856c8)
#<!-- -->23 0x000058405c12940a mlir::parseAsmSourceFile(llvm::SourceMgr const&amp;, mlir::Block*, mlir::ParserConfig const&amp;, mlir::AsmParserState*, mlir::AsmParserCodeCompleteContext*) (llvmorg-22.1.0-install/bin/mlir-opt+0x86c640a)
#<!-- -->24 0x0000584056058df7 performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) MlirOptMain.cpp:0:0
#<!-- -->25 0x000058405605a4ea llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::'lambda'(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, llvm::raw_ostream&amp;)&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) MlirOptMain.cpp:0:0
#<!-- -->26 0x000058405dc02731 mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) (llvmorg-22.1.0-install/bin/mlir-opt+0xa19f731)
#<!-- -->27 0x00005840560518d0 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) (.part.0) MlirOptMain.cpp:0:0
#<!-- -->28 0x000058405605ab70 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) (llvmorg-22.1.0-install/bin/mlir-opt+0x25f7b70)
#<!-- -->29 0x000058405605ada9 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) (llvmorg-22.1.0-install/bin/mlir-opt+0x25f7da9)
#<!-- -->30 0x0000584055e8c023 main (llvmorg-22.1.0-install/bin/mlir-opt+0x2429023)
#<!-- -->31 0x00007a7b5302a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->32 0x00007a7b5302a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->33 0x00007a7b5302a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->34 0x0000584055f71405 _start (llvmorg-22.1.0-install/bin/mlir-opt+0x250e405)
Aborted (core dumped)
```
</details>


---

### Comment 2 - EugeneZelenko

@xvv4402: Please also provide reproducers on https://godbolt.org for `main` and current release (22). Later is when 22 will be available there.

---

### Comment 3 - CoTinker

Fixed by #180741.

---

