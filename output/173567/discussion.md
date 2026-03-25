# mlir-opt crashes in mlir/lib/Dialect/Utils/ReshapeOpsUtils.cpp:454

**Author:** Emilyaxe
**URL:** https://github.com/llvm/llvm-project/issues/173567
**Status:** Closed
**Labels:** mlir:tensor, crash
**Closed Date:** 2025-12-30T10:35:54Z

## Body

When running opt on the following IR, mlir-opt crashes. 


version 2c02e4c7909a

test.mlir

```
func.func @tensor.collapse_shape(%arg0: tensor<1x?xf32>) -> tensor<?x10xf32> {
  %0 = tensor.collapse_shape %arg0 [[0, 1], []] : tensor<1x?xf32> into tensor<?x10xf32>
  return %0 : tensor<?x10xf32>
}

```


commads:

mlir-opt test.mlir

stacktrace

```
mlir-opt: llvm-project/mlir/lib/Dialect/Utils/ReshapeOpsUtils.cpp:454: SmallVector<AffineMap, 4> mlir::getSymbolLessAffineMaps(ArrayRef<ReassociationExprs>): Assertion `!exprs.empty()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: ./build/bin/mlir-opt test.mlir
 #0 0x000055b767383358 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (./build/bin/mlir-opt+0x1e97358)
 #1 0x000055b767380a15 llvm::sys::RunSignalHandlers() (./build/bin/mlir-opt+0x1e94a15)
 #2 0x000055b767384396 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x00007f39c1aa2420 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x14420)
 #4 0x00007f39c10fb00b raise /build/glibc-B3wQXB/glibc-2.31/signal/../sysdeps/unix/sysv/linux/raise.c:51:1
 #5 0x00007f39c10da859 abort /build/glibc-B3wQXB/glibc-2.31/stdlib/abort.c:81:7
 #6 0x00007f39c10da729 get_sysdep_segment_value /build/glibc-B3wQXB/glibc-2.31/intl/loadmsgcat.c:509:8
 #7 0x00007f39c10da729 _nl_load_domain /build/glibc-B3wQXB/glibc-2.31/intl/loadmsgcat.c:970:34
 #8 0x00007f39c10ebfd6 (/lib/x86_64-linux-gnu/libc.so.6+0x33fd6)
 #9 0x000055b76f938418 mlir::getSymbolLessAffineMaps(llvm::ArrayRef<llvm::SmallVector<mlir::AffineExpr, 2u>>) (./build/bin/mlir-opt+0xa44c418)
#10 0x000055b76f5fbe54 mlir::tensor::CollapseShapeOp::verify() (./build/bin/mlir-opt+0xa10fe54)
#11 0x000055b76f5bdb2e mlir::Op<mlir::tensor::CollapseShapeOp, mlir::OpTrait::ZeroRegions, mlir::OpTrait::OneResult, mlir::OpTrait::OneTypedResult<mlir::TensorType>::Impl, mlir::OpTrait::ZeroSuccessors, mlir::OpTrait::OneOperand, mlir::OpTrait::OpInvariants, mlir::BytecodeOpInterface::Trait, mlir::OpAsmOpInterface::Trait, mlir::ConditionallySpeculatable::Trait, mlir::OpTrait::AlwaysSpeculatableImplTrait, mlir::MemoryEffectOpInterface::Trait>::verifyInvariants(mlir::Operation*) (./build/bin/mlir-opt+0xa0d1b2e)
#12 0x000055b76f5bc16e mlir::RegisteredOperationName::Model<mlir::tensor::CollapseShapeOp>::verifyInvariants(mlir::Operation*) (./build/bin/mlir-opt+0xa0d016e)
#13 0x000055b76fd47a7f (anonymous namespace)::OperationVerifier::verifyOpAndDominance(mlir::Operation&) Verifier.cpp:0:0
#14 0x000055b76fd47c50 (anonymous namespace)::OperationVerifier::verifyOpAndDominance(mlir::Operation&) Verifier.cpp:0:0
#15 0x000055b76fd47146 mlir::verify(mlir::Operation*, bool) (./build/bin/mlir-opt+0xa85b146)
#16 0x000055b76e3e7df5 mlir::parseAsmSourceFile(llvm::SourceMgr const&, mlir::Block*, mlir::ParserConfig const&, mlir::AsmParserState*, mlir::AsmParserCodeCompleteContext*) (./build/bin/mlir-opt+0x8efbdf5)
#17 0x000055b76e3cc4d3 mlir::parseSourceFile(std::shared_ptr<llvm::SourceMgr> const&, mlir::Block*, mlir::ParserConfig const&, mlir::LocationAttr*) (./build/bin/mlir-opt+0x8ee04d3)
#18 0x000055b767427afe mlir::parseSourceFileForTool(std::shared_ptr<llvm::SourceMgr> const&, mlir::ParserConfig const&, bool) (./build/bin/mlir-opt+0x1f3bafe)
#19 0x000055b767426827 performActions(llvm::raw_ostream&, std::shared_ptr<llvm::SourceMgr> const&, mlir::MLIRContext*, mlir::MlirOptMainConfig const&) MlirOptMain.cpp:0:0
#20 0x000055b7674265a6 llvm::LogicalResult llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::callback_fn<mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) MlirOptMain.cpp:0:0
#21 0x000055b76fd58855 mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef) (./build/bin/mlir-opt+0xa86c855)
#22 0x000055b76741c166 mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&) (./build/bin/mlir-opt+0x1f30166)
#23 0x000055b76741c4a2 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&) (./build/bin/mlir-opt+0x1f304a2)
#24 0x000055b76741c805 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&) (./build/bin/mlir-opt+0x1f30805)
#25 0x000055b7673699b3 main (./build/bin/mlir-opt+0x1e7d9b3)
#26 0x00007f39c10dc083 __libc_start_main /build/glibc-B3wQXB/glibc-2.31/csu/../csu/libc-start.c:342:3
#27 0x000055b7673694ee _start (./build/bin/mlir-opt+0x1e7d4ee)
```


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: None (Emilyaxe)

<details>
When running opt on the following IR, mlir-opt crashes. 


version 2c02e4c7909a

test.mlir

```
func.func @<!-- -->tensor.collapse_shape(%arg0: tensor&lt;1x?xf32&gt;) -&gt; tensor&lt;?x10xf32&gt; {
  %0 = tensor.collapse_shape %arg0 [[0, 1], []] : tensor&lt;1x?xf32&gt; into tensor&lt;?x10xf32&gt;
  return %0 : tensor&lt;?x10xf32&gt;
}

```


commads:

mlir-opt test.mlir

stacktrace

```
mlir-opt: llvm-project/mlir/lib/Dialect/Utils/ReshapeOpsUtils.cpp:454: SmallVector&lt;AffineMap, 4&gt; mlir::getSymbolLessAffineMaps(ArrayRef&lt;ReassociationExprs&gt;): Assertion `!exprs.empty()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: ./build/bin/mlir-opt test.mlir
 #<!-- -->0 0x000055b767383358 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (./build/bin/mlir-opt+0x1e97358)
 #<!-- -->1 0x000055b767380a15 llvm::sys::RunSignalHandlers() (./build/bin/mlir-opt+0x1e94a15)
 #<!-- -->2 0x000055b767384396 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x00007f39c1aa2420 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x14420)
 #<!-- -->4 0x00007f39c10fb00b raise /build/glibc-B3wQXB/glibc-2.31/signal/../sysdeps/unix/sysv/linux/raise.c:51:1
 #<!-- -->5 0x00007f39c10da859 abort /build/glibc-B3wQXB/glibc-2.31/stdlib/abort.c:81:7
 #<!-- -->6 0x00007f39c10da729 get_sysdep_segment_value /build/glibc-B3wQXB/glibc-2.31/intl/loadmsgcat.c:509:8
 #<!-- -->7 0x00007f39c10da729 _nl_load_domain /build/glibc-B3wQXB/glibc-2.31/intl/loadmsgcat.c:970:34
 #<!-- -->8 0x00007f39c10ebfd6 (/lib/x86_64-linux-gnu/libc.so.6+0x33fd6)
 #<!-- -->9 0x000055b76f938418 mlir::getSymbolLessAffineMaps(llvm::ArrayRef&lt;llvm::SmallVector&lt;mlir::AffineExpr, 2u&gt;&gt;) (./build/bin/mlir-opt+0xa44c418)
#<!-- -->10 0x000055b76f5fbe54 mlir::tensor::CollapseShapeOp::verify() (./build/bin/mlir-opt+0xa10fe54)
#<!-- -->11 0x000055b76f5bdb2e mlir::Op&lt;mlir::tensor::CollapseShapeOp, mlir::OpTrait::ZeroRegions, mlir::OpTrait::OneResult, mlir::OpTrait::OneTypedResult&lt;mlir::TensorType&gt;::Impl, mlir::OpTrait::ZeroSuccessors, mlir::OpTrait::OneOperand, mlir::OpTrait::OpInvariants, mlir::BytecodeOpInterface::Trait, mlir::OpAsmOpInterface::Trait, mlir::ConditionallySpeculatable::Trait, mlir::OpTrait::AlwaysSpeculatableImplTrait, mlir::MemoryEffectOpInterface::Trait&gt;::verifyInvariants(mlir::Operation*) (./build/bin/mlir-opt+0xa0d1b2e)
#<!-- -->12 0x000055b76f5bc16e mlir::RegisteredOperationName::Model&lt;mlir::tensor::CollapseShapeOp&gt;::verifyInvariants(mlir::Operation*) (./build/bin/mlir-opt+0xa0d016e)
#<!-- -->13 0x000055b76fd47a7f (anonymous namespace)::OperationVerifier::verifyOpAndDominance(mlir::Operation&amp;) Verifier.cpp:0:0
#<!-- -->14 0x000055b76fd47c50 (anonymous namespace)::OperationVerifier::verifyOpAndDominance(mlir::Operation&amp;) Verifier.cpp:0:0
#<!-- -->15 0x000055b76fd47146 mlir::verify(mlir::Operation*, bool) (./build/bin/mlir-opt+0xa85b146)
#<!-- -->16 0x000055b76e3e7df5 mlir::parseAsmSourceFile(llvm::SourceMgr const&amp;, mlir::Block*, mlir::ParserConfig const&amp;, mlir::AsmParserState*, mlir::AsmParserCodeCompleteContext*) (./build/bin/mlir-opt+0x8efbdf5)
#<!-- -->17 0x000055b76e3cc4d3 mlir::parseSourceFile(std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::Block*, mlir::ParserConfig const&amp;, mlir::LocationAttr*) (./build/bin/mlir-opt+0x8ee04d3)
#<!-- -->18 0x000055b767427afe mlir::parseSourceFileForTool(std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::ParserConfig const&amp;, bool) (./build/bin/mlir-opt+0x1f3bafe)
#<!-- -->19 0x000055b767426827 performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) MlirOptMain.cpp:0:0
#<!-- -->20 0x000055b7674265a6 llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) MlirOptMain.cpp:0:0
#<!-- -->21 0x000055b76fd58855 mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) (./build/bin/mlir-opt+0xa86c855)
#<!-- -->22 0x000055b76741c166 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) (./build/bin/mlir-opt+0x1f30166)
#<!-- -->23 0x000055b76741c4a2 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) (./build/bin/mlir-opt+0x1f304a2)
#<!-- -->24 0x000055b76741c805 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) (./build/bin/mlir-opt+0x1f30805)
#<!-- -->25 0x000055b7673699b3 main (./build/bin/mlir-opt+0x1e7d9b3)
#<!-- -->26 0x00007f39c10dc083 __libc_start_main /build/glibc-B3wQXB/glibc-2.31/csu/../csu/libc-start.c:342:3
#<!-- -->27 0x000055b7673694ee _start (./build/bin/mlir-opt+0x1e7d4ee)
```

</details>


---

### Comment 2 - akparmar004

could you please assign this to me?

---

