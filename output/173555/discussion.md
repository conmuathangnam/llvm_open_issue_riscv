# mlir-opt crashes in mlir::vector::OuterProductOp::verify()

**Author:** Emilyaxe
**URL:** https://github.com/llvm/llvm-project/issues/173555
**Status:** Closed
**Labels:** crash, mlir:vector
**Closed Date:** 2026-01-08T01:29:44Z

## Body


When running opt on the following IR, mlir-opt crashes. 


version 2c02e4c7909a

test.mlir

```
func.func @vector_outerproduct_f32(%lhs : vector<[4]xf32>, %rhs : vector<[4]xf32>, %acc : vector<[4]x[4]xf32>) -> vector<[4]x[4]xf32> {
  %result = vector.outerproduct %lhs, %rhs, %acc {kind = "kshr"} : vector<[4]xf32>, vector<[4]xf32>
  return %result : vector<[4]x[4]xf32>
}

#map1 = affine_map<(d0, d1, d2) -> (d0, d1)>
```


commads:

mlir-opt test.mlir

stacktrace

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: ./build/bin/mlir-opt test.mlir
 #0 0x000055bf9b016358 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (./build/bin/mlir-opt+0x1e97358)
 #1 0x000055bf9b013a15 llvm::sys::RunSignalHandlers() (./build/bin/mlir-opt+0x1e94a15)
 #2 0x000055bf9b017396 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x00007ff7dc389420 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x14420)
 #4 0x000055bfa24283dc mlir::vector::OuterProductOp::verify() (./build/bin/mlir-opt+0x92a93dc)
 #5 0x000055bfa24cd033 mlir::Op<mlir::vector::OuterProductOp, mlir::OpTrait::ZeroRegions, mlir::OpTrait::OneResult, mlir::OpTrait::OneTypedResult<mlir::VectorType>::Impl, mlir::OpTrait::ZeroSuccessors, mlir::OpTrait::AtLeastNOperands<2u>::Impl, mlir::OpTrait::OpInvariants, mlir::BytecodeOpInterface::Trait, mlir::ConditionallySpeculatable::Trait, mlir::OpTrait::AlwaysSpeculatableImplTrait, mlir::MemoryEffectOpInterface::Trait, mlir::vector::MaskableOpInterface::Trait>::verifyInvariants(mlir::Operation*) (./build/bin/mlir-opt+0x934e033)
 #6 0x000055bfa24cb64e mlir::RegisteredOperationName::Model<mlir::vector::OuterProductOp>::verifyInvariants(mlir::Operation*) (./build/bin/mlir-opt+0x934c64e)
 #7 0x000055bfa39daa7f (anonymous namespace)::OperationVerifier::verifyOpAndDominance(mlir::Operation&) Verifier.cpp:0:0
 #8 0x000055bfa39dac50 (anonymous namespace)::OperationVerifier::verifyOpAndDominance(mlir::Operation&) Verifier.cpp:0:0
 #9 0x000055bfa39da146 mlir::verify(mlir::Operation*, bool) (./build/bin/mlir-opt+0xa85b146)
#10 0x000055bfa207adf5 mlir::parseAsmSourceFile(llvm::SourceMgr const&, mlir::Block*, mlir::ParserConfig const&, mlir::AsmParserState*, mlir::AsmParserCodeCompleteContext*) (./build/bin/mlir-opt+0x8efbdf5)
#11 0x000055bfa205f4d3 mlir::parseSourceFile(std::shared_ptr<llvm::SourceMgr> const&, mlir::Block*, mlir::ParserConfig const&, mlir::LocationAttr*) (./build/bin/mlir-opt+0x8ee04d3)
#12 0x000055bf9b0baafe mlir::parseSourceFileForTool(std::shared_ptr<llvm::SourceMgr> const&, mlir::ParserConfig const&, bool) (./build/bin/mlir-opt+0x1f3bafe)
#13 0x000055bf9b0b9827 performActions(llvm::raw_ostream&, std::shared_ptr<llvm::SourceMgr> const&, mlir::MLIRContext*, mlir::MlirOptMainConfig const&) MlirOptMain.cpp:0:0
#14 0x000055bf9b0b95a6 llvm::LogicalResult llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::callback_fn<mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) MlirOptMain.cpp:0:0
#15 0x000055bfa39eb855 mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef) (./build/bin/mlir-opt+0xa86c855)
#16 0x000055bf9b0af166 mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&) (./build/bin/mlir-opt+0x1f30166)
#17 0x000055bf9b0af4a2 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&) (./build/bin/mlir-opt+0x1f304a2)
#18 0x000055bf9b0af805 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&) (./build/bin/mlir-opt+0x1f30805)
#19 0x000055bf9affc9b3 main (./build/bin/mlir-opt+0x1e7d9b3)
#20 0x00007ff7db9c3083 __libc_start_main /build/glibc-B3wQXB/glibc-2.
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
func.func @<!-- -->vector_outerproduct_f32(%lhs : vector&lt;[4]xf32&gt;, %rhs : vector&lt;[4]xf32&gt;, %acc : vector&lt;[4]x[4]xf32&gt;) -&gt; vector&lt;[4]x[4]xf32&gt; {
  %result = vector.outerproduct %lhs, %rhs, %acc {kind = "kshr"} : vector&lt;[4]xf32&gt;, vector&lt;[4]xf32&gt;
  return %result : vector&lt;[4]x[4]xf32&gt;
}

#map1 = affine_map&lt;(d0, d1, d2) -&gt; (d0, d1)&gt;
```


commads:

mlir-opt test.mlir

stacktrace

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: ./build/bin/mlir-opt test.mlir
 #<!-- -->0 0x000055bf9b016358 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (./build/bin/mlir-opt+0x1e97358)
 #<!-- -->1 0x000055bf9b013a15 llvm::sys::RunSignalHandlers() (./build/bin/mlir-opt+0x1e94a15)
 #<!-- -->2 0x000055bf9b017396 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x00007ff7dc389420 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x14420)
 #<!-- -->4 0x000055bfa24283dc mlir::vector::OuterProductOp::verify() (./build/bin/mlir-opt+0x92a93dc)
 #<!-- -->5 0x000055bfa24cd033 mlir::Op&lt;mlir::vector::OuterProductOp, mlir::OpTrait::ZeroRegions, mlir::OpTrait::OneResult, mlir::OpTrait::OneTypedResult&lt;mlir::VectorType&gt;::Impl, mlir::OpTrait::ZeroSuccessors, mlir::OpTrait::AtLeastNOperands&lt;2u&gt;::Impl, mlir::OpTrait::OpInvariants, mlir::BytecodeOpInterface::Trait, mlir::ConditionallySpeculatable::Trait, mlir::OpTrait::AlwaysSpeculatableImplTrait, mlir::MemoryEffectOpInterface::Trait, mlir::vector::MaskableOpInterface::Trait&gt;::verifyInvariants(mlir::Operation*) (./build/bin/mlir-opt+0x934e033)
 #<!-- -->6 0x000055bfa24cb64e mlir::RegisteredOperationName::Model&lt;mlir::vector::OuterProductOp&gt;::verifyInvariants(mlir::Operation*) (./build/bin/mlir-opt+0x934c64e)
 #<!-- -->7 0x000055bfa39daa7f (anonymous namespace)::OperationVerifier::verifyOpAndDominance(mlir::Operation&amp;) Verifier.cpp:0:0
 #<!-- -->8 0x000055bfa39dac50 (anonymous namespace)::OperationVerifier::verifyOpAndDominance(mlir::Operation&amp;) Verifier.cpp:0:0
 #<!-- -->9 0x000055bfa39da146 mlir::verify(mlir::Operation*, bool) (./build/bin/mlir-opt+0xa85b146)
#<!-- -->10 0x000055bfa207adf5 mlir::parseAsmSourceFile(llvm::SourceMgr const&amp;, mlir::Block*, mlir::ParserConfig const&amp;, mlir::AsmParserState*, mlir::AsmParserCodeCompleteContext*) (./build/bin/mlir-opt+0x8efbdf5)
#<!-- -->11 0x000055bfa205f4d3 mlir::parseSourceFile(std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::Block*, mlir::ParserConfig const&amp;, mlir::LocationAttr*) (./build/bin/mlir-opt+0x8ee04d3)
#<!-- -->12 0x000055bf9b0baafe mlir::parseSourceFileForTool(std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::ParserConfig const&amp;, bool) (./build/bin/mlir-opt+0x1f3bafe)
#<!-- -->13 0x000055bf9b0b9827 performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) MlirOptMain.cpp:0:0
#<!-- -->14 0x000055bf9b0b95a6 llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) MlirOptMain.cpp:0:0
#<!-- -->15 0x000055bfa39eb855 mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) (./build/bin/mlir-opt+0xa86c855)
#<!-- -->16 0x000055bf9b0af166 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) (./build/bin/mlir-opt+0x1f30166)
#<!-- -->17 0x000055bf9b0af4a2 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) (./build/bin/mlir-opt+0x1f304a2)
#<!-- -->18 0x000055bf9b0af805 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) (./build/bin/mlir-opt+0x1f30805)
#<!-- -->19 0x000055bf9affc9b3 main (./build/bin/mlir-opt+0x1e7d9b3)
#<!-- -->20 0x00007ff7db9c3083 __libc_start_main /build/glibc-B3wQXB/glibc-2.
```


</details>


---

