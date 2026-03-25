# mlir-opt crash: getTerminator() assertion failure during sparse_tensor.reduce verification

**Author:** xvv4402
**URL:** https://github.com/llvm/llvm-project/issues/180720
**Status:** Closed
**Labels:** mlir, crash
**Closed Date:** 2026-02-11T01:08:46Z

## Body

**Bug Op**: sparse_tensor.reduce

**Bug Pass**: N/A (General Verification)

**Detailed Description**: The verifier for sparse_tensor.reduce crashes when encountering a block that it deems malformed (e.g., trailing operations after a yield or an empty block). The failure occurs because the dialect's internal helper verifyNumBlockArgs calls getTerminator() without first checking if the block is empty or contains a terminator, triggering a core IR assertion.

```mlir
module {
  func.func @main() {
    %0 = "emitc.constant"() <{value = 0 : i32}> : () -> i32
    %1 = smt.forall ["x"] {
      ^bb0(%arg0: !smt.int):
      %c1 = smt.int.constant 1
      %2 = smt.int.add %arg0, %c1
      %3 = smt.eq %2, %arg0 : !smt.int
      smt.yield %3 : !smt.bool
    }
    %4 = emitc.bitwise_not %0 : (i32) -> i32
    %5 = gpu.all_reduce  add %0 uniform {
    } : (i32) -> i32
    %6 = sparse_tensor.reduce %0, %5, %0 : i32 {
      ^bb0(%arg0: i32, %arg1: i32):
      %7 = arith.addi %arg0, %arg1 : i32
      sparse_tensor.yield %7 : i32
      %11 = arith.sitofp %7 : i32 to f32
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
 #0 0x00005ea3d3e1fe32 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #1 0x00005ea3d3e1c5af llvm::sys::RunSignalHandlers() (llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #2 0x00005ea3d3e1c6fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x000079a8ce045330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x000079a8ce09eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x000079a8ce09eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x000079a8ce09eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x000079a8ce04527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x000079a8ce0288ff abort ./stdlib/abort.c:81:7
 #9 0x000079a8ce02881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#10 0x000079a8ce03b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#11 0x00005ea3db962c71 mlir::Block::getTerminator() (llvmorg-22.1.0-install/bin/mlir-opt+0xa072c71)
#12 0x00005ea3da243b19 llvm::LogicalResult verifyNumBlockArgs<mlir::sparse_tensor::UnaryOp>(mlir::sparse_tensor::UnaryOp*, mlir::Region&, char const*, mlir::TypeRange, mlir::Type) SparseTensorDialect.cpp:0:0
#13 0x00005ea3da244327 mlir::sparse_tensor::ReduceOp::verify() (llvmorg-22.1.0-install/bin/mlir-opt+0x8954327)
#14 0x00005ea3da25edae mlir::Op<mlir::sparse_tensor::ReduceOp, mlir::OpTrait::OneRegion, mlir::OpTrait::OneResult, mlir::OpTrait::OneTypedResult<mlir::Type>::Impl, mlir::OpTrait::ZeroSuccessors, mlir::OpTrait::NOperands<3u>::Impl, mlir::OpTrait::OpInvariants, mlir::ConditionallySpeculatable::Trait, mlir::OpTrait::AlwaysSpeculatableImplTrait, mlir::MemoryEffectOpInterface::Trait, mlir::OpTrait::SameOperandsAndResultType, mlir::InferTypeOpInterface::Trait>::verifyInvariants(mlir::Operation*) (llvmorg-22.1.0-install/bin/mlir-opt+0x896edae)
#15 0x00005ea3da26ba67 mlir::RegisteredOperationName::Model<mlir::sparse_tensor::ReduceOp>::verifyInvariants(mlir::Operation*) (llvmorg-22.1.0-install/bin/mlir-opt+0x897ba67)
#16 0x00005ea3dba7276e (anonymous namespace)::OperationVerifier::verifyOnEntrance(mlir::Operation&) (.isra.0) Verifier.cpp:0:0
#17 0x00005ea3dba74818 (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&) Verifier.cpp:0:0
#18 0x00005ea3dba73ac9 (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&) Verifier.cpp:0:0
#19 0x00005ea3dba74799 (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&) Verifier.cpp:0:0
#20 0x00005ea3dba73ac9 (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&) Verifier.cpp:0:0
#21 0x00005ea3dba74799 (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&) Verifier.cpp:0:0
#22 0x00005ea3dba756c8 mlir::verify(mlir::Operation*, bool) (llvmorg-22.1.0-install/bin/mlir-opt+0xa1856c8)
#23 0x00005ea3d9fb640a mlir::parseAsmSourceFile(llvm::SourceMgr const&, mlir::Block*, mlir::ParserConfig const&, mlir::AsmParserState*, mlir::AsmParserCodeCompleteContext*) (llvmorg-22.1.0-install/bin/mlir-opt+0x86c640a)
#24 0x00005ea3d3ee5df7 performActions(llvm::raw_ostream&, std::shared_ptr<llvm::SourceMgr> const&, mlir::MLIRContext*, mlir::MlirOptMainConfig const&) MlirOptMain.cpp:0:0
#25 0x00005ea3d3ee74ea llvm::LogicalResult llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::callback_fn<mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::'lambda'(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, llvm::raw_ostream&)>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) MlirOptMain.cpp:0:0
#26 0x00005ea3dba8f731 mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef) (llvmorg-22.1.0-install/bin/mlir-opt+0xa19f731)
#27 0x00005ea3d3ede8d0 mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&) (.part.0) MlirOptMain.cpp:0:0
#28 0x00005ea3d3ee7b70 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&) (llvmorg-22.1.0-install/bin/mlir-opt+0x25f7b70)
#29 0x00005ea3d3ee7da9 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&) (llvmorg-22.1.0-install/bin/mlir-opt+0x25f7da9)
#30 0x00005ea3d3d19023 main (llvmorg-22.1.0-install/bin/mlir-opt+0x2429023)
#31 0x000079a8ce02a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#32 0x000079a8ce02a28b call_init ./csu/../csu/libc-start.c:128:20
#33 0x000079a8ce02a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#34 0x00005ea3d3dfe405 _start (llvmorg-22.1.0-install/bin/mlir-opt+0x250e405)
Aborted (core dumped)
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: None (xvv4402)

<details>
**Bug Op**: sparse_tensor.reduce

**Bug Pass**: N/A (General Verification)

**Detailed Description**: The verifier for sparse_tensor.reduce crashes when encountering a block that it deems malformed (e.g., trailing operations after a yield or an empty block). The failure occurs because the dialect's internal helper verifyNumBlockArgs calls getTerminator() without first checking if the block is empty or contains a terminator, triggering a core IR assertion.

```mlir
module {
  func.func @<!-- -->main() {
    %0 = "emitc.constant"() &lt;{value = 0 : i32}&gt; : () -&gt; i32
    %1 = smt.forall ["x"] {
      ^bb0(%arg0: !smt.int):
      %c1 = smt.int.constant 1
      %2 = smt.int.add %arg0, %c1
      %3 = smt.eq %2, %arg0 : !smt.int
      smt.yield %3 : !smt.bool
    }
    %4 = emitc.bitwise_not %0 : (i32) -&gt; i32
    %5 = gpu.all_reduce  add %0 uniform {
    } : (i32) -&gt; i32
    %6 = sparse_tensor.reduce %0, %5, %0 : i32 {
      ^bb0(%arg0: i32, %arg1: i32):
      %7 = arith.addi %arg0, %arg1 : i32
      sparse_tensor.yield %7 : i32
      %11 = arith.sitofp %7 : i32 to f32
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
 #<!-- -->0 0x00005ea3d3e1fe32 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #<!-- -->1 0x00005ea3d3e1c5af llvm::sys::RunSignalHandlers() (llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #<!-- -->2 0x00005ea3d3e1c6fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x000079a8ce045330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->4 0x000079a8ce09eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->5 0x000079a8ce09eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->6 0x000079a8ce09eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->7 0x000079a8ce04527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->8 0x000079a8ce0288ff abort ./stdlib/abort.c:81:7
 #<!-- -->9 0x000079a8ce02881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->10 0x000079a8ce03b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->11 0x00005ea3db962c71 mlir::Block::getTerminator() (llvmorg-22.1.0-install/bin/mlir-opt+0xa072c71)
#<!-- -->12 0x00005ea3da243b19 llvm::LogicalResult verifyNumBlockArgs&lt;mlir::sparse_tensor::UnaryOp&gt;(mlir::sparse_tensor::UnaryOp*, mlir::Region&amp;, char const*, mlir::TypeRange, mlir::Type) SparseTensorDialect.cpp:0:0
#<!-- -->13 0x00005ea3da244327 mlir::sparse_tensor::ReduceOp::verify() (llvmorg-22.1.0-install/bin/mlir-opt+0x8954327)
#<!-- -->14 0x00005ea3da25edae mlir::Op&lt;mlir::sparse_tensor::ReduceOp, mlir::OpTrait::OneRegion, mlir::OpTrait::OneResult, mlir::OpTrait::OneTypedResult&lt;mlir::Type&gt;::Impl, mlir::OpTrait::ZeroSuccessors, mlir::OpTrait::NOperands&lt;3u&gt;::Impl, mlir::OpTrait::OpInvariants, mlir::ConditionallySpeculatable::Trait, mlir::OpTrait::AlwaysSpeculatableImplTrait, mlir::MemoryEffectOpInterface::Trait, mlir::OpTrait::SameOperandsAndResultType, mlir::InferTypeOpInterface::Trait&gt;::verifyInvariants(mlir::Operation*) (llvmorg-22.1.0-install/bin/mlir-opt+0x896edae)
#<!-- -->15 0x00005ea3da26ba67 mlir::RegisteredOperationName::Model&lt;mlir::sparse_tensor::ReduceOp&gt;::verifyInvariants(mlir::Operation*) (llvmorg-22.1.0-install/bin/mlir-opt+0x897ba67)
#<!-- -->16 0x00005ea3dba7276e (anonymous namespace)::OperationVerifier::verifyOnEntrance(mlir::Operation&amp;) (.isra.0) Verifier.cpp:0:0
#<!-- -->17 0x00005ea3dba74818 (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;) Verifier.cpp:0:0
#<!-- -->18 0x00005ea3dba73ac9 (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;) Verifier.cpp:0:0
#<!-- -->19 0x00005ea3dba74799 (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;) Verifier.cpp:0:0
#<!-- -->20 0x00005ea3dba73ac9 (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;) Verifier.cpp:0:0
#<!-- -->21 0x00005ea3dba74799 (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;) Verifier.cpp:0:0
#<!-- -->22 0x00005ea3dba756c8 mlir::verify(mlir::Operation*, bool) (llvmorg-22.1.0-install/bin/mlir-opt+0xa1856c8)
#<!-- -->23 0x00005ea3d9fb640a mlir::parseAsmSourceFile(llvm::SourceMgr const&amp;, mlir::Block*, mlir::ParserConfig const&amp;, mlir::AsmParserState*, mlir::AsmParserCodeCompleteContext*) (llvmorg-22.1.0-install/bin/mlir-opt+0x86c640a)
#<!-- -->24 0x00005ea3d3ee5df7 performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) MlirOptMain.cpp:0:0
#<!-- -->25 0x00005ea3d3ee74ea llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::'lambda'(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, llvm::raw_ostream&amp;)&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) MlirOptMain.cpp:0:0
#<!-- -->26 0x00005ea3dba8f731 mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) (llvmorg-22.1.0-install/bin/mlir-opt+0xa19f731)
#<!-- -->27 0x00005ea3d3ede8d0 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) (.part.0) MlirOptMain.cpp:0:0
#<!-- -->28 0x00005ea3d3ee7b70 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) (llvmorg-22.1.0-install/bin/mlir-opt+0x25f7b70)
#<!-- -->29 0x00005ea3d3ee7da9 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) (llvmorg-22.1.0-install/bin/mlir-opt+0x25f7da9)
#<!-- -->30 0x00005ea3d3d19023 main (llvmorg-22.1.0-install/bin/mlir-opt+0x2429023)
#<!-- -->31 0x000079a8ce02a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->32 0x000079a8ce02a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->33 0x000079a8ce02a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->34 0x00005ea3d3dfe405 _start (llvmorg-22.1.0-install/bin/mlir-opt+0x250e405)
Aborted (core dumped)
```
</details>


---

