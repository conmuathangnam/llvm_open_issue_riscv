# [MLIR] Test dialect element count verification crashes on dynamic shapes with getNumElements() assertion

**Author:** sairam2661
**URL:** https://github.com/llvm/llvm-project/issues/159740
**Status:** Closed
**Labels:** mlir, crash
**Closed Date:** 2026-03-01T10:59:44Z

## Body

Test commit: [5a86dc996c26299de63effc927075dcbfb924167](https://github.com/llvm/llvm-project/pull/157520/commits/5a86dc996c26299de63effc927075dcbfb924167)

**Description**:
The test dialect crashes when it tries to verify the element count equality between operands and results that have dynamic shapes. 

**Steps to reproduce**
- Minimal MLIR program (test.mlir)
```
"builtin.module"() ({
  "func.func"() <{
    function_type = (tensor<2xi32>) -> (),
    sym_name = "test"
  }> ({
  ^bb0(%arg0: tensor<2xi32>):
    %0 = "test.operand0_and_result_have_same_element_count"(%arg0, %arg0) : 
         (tensor<2xi32>, tensor<2xi32>) -> tensor<?x?xf32>
    "func.return"() : () -> ()
  }) : () -> ()
}) : () -> ()
```

**Command**
```
mlir-opt test.mlir
```

**Output**
```
mlir-opt: /workdir/llvm-project/build/tools/mlir/include/mlir/IR/BuiltinTypeInterfaces.h.inc:436: int64_t mlir::ShapedType::getNumElements() const: Assertion `hasStaticShape() && "cannot get element count of dynamic shaped type"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.      Program arguments: /workdir/llvm-project/build/bin/mlir-opt test.mlir
 #0 0x0000556c4d9f0a1b backtrace (/workdir/llvm-project/build/bin/mlir-opt+0xf5f1a1b)
 #1 0x0000556c4dc75cfc llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /workdir/llvm-project/llvm/lib/Support/Unix/Signals.inc:834:13
 #2 0x0000556c4dc7017f llvm::sys::RunSignalHandlers() /workdir/llvm-project/llvm/lib/Support/Signals.cpp:105:18
 #3 0x0000556c4dc78316 SignalHandler(int, siginfo_t*, void*) /workdir/llvm-project/llvm/lib/Support/Unix/Signals.inc:426:38
 #4 0x00007f442ace5520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #5 0x00007f442ad399fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #6 0x00007f442ace5476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #7 0x00007f442accb7f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #8 0x00007f442accb71b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #9 0x00007f442acdce96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
#10 0x0000556c4ed2c586 mlir::ShapedType::getNumElements() const /workdir/llvm-project/build/tools/mlir/include/mlir/IR/BuiltinTypeInterfaces.h.inc:436:7
#11 0x0000556c58d2545a test::OperandZeroAndResultHaveSameElementCount::verifyInvariantsImpl() /workdir/llvm-project/build/tools/mlir/test/lib/Dialect/Test/TestOps.cpp.inc:54821:106
#12 0x0000556c58d5c2bd mlir::OpTrait::OpInvariants<test::OperandZeroAndResultHaveSameElementCount>::verifyTrait(mlir::Operation*) /workdir/llvm-project/mlir/include/mlir/IR/OpDefinition.h:440:5
#13 0x0000556c58d5c2bd llvm::LogicalResult mlir::op_definition_impl::verifyTrait<mlir::OpTrait::OpInvariants<test::OperandZeroAndResultHaveSameElementCount> >(mlir::Operation*) /workdir/llvm-project/mlir/include/mlir/IR/OpDefinition.h:1637:12
#14 0x0000556c58d5c2bd llvm::LogicalResult mlir::op_definition_impl::verifyTraits<mlir::OpTrait::ZeroRegions<test::OperandZeroAndResultHaveSameElementCount>, mlir::OpTrait::OneResult<test::OperandZeroAndResultHaveSameElementCount>, mlir::OpTrait::OneTypedResult<mlir::ShapedType>::Impl<test::OperandZeroAndResultHaveSameElementCount>, mlir::OpTrait::ZeroSuccessors<test::OperandZeroAndResultHaveSameElementCount>, mlir::OpTrait::NOperands<2u>::Impl<test::OperandZeroAndResultHaveSameElementCount>, mlir::OpTrait::OpInvariants<test::OperandZeroAndResultHaveSameElementCount> >(mlir::Operation*) /workdir/llvm-project/mlir/include/mlir/IR/OpDefinition.h:1645:29
#15 0x0000556c58d5c13e llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#16 0x0000556c58d5c13e llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#17 0x0000556c58d5c13e mlir::Op<test::OperandZeroAndResultHaveSameElementCount, mlir::OpTrait::ZeroRegions, mlir::OpTrait::OneResult, mlir::OpTrait::OneTypedResult<mlir::ShapedType>::Impl, mlir::OpTrait::ZeroSuccessors, mlir::OpTrait::NOperands<2u>::Impl, mlir::OpTrait::OpInvariants>::verifyInvariants(mlir::Operation*) /workdir/llvm-project/mlir/include/mlir/IR/OpDefinition.h:2052:9
#18 0x0000556c58d5bb21 mlir::RegisteredOperationName::Model<test::OperandZeroAndResultHaveSameElementCount>::verifyInvariants(mlir::Operation*) /workdir/llvm-project/mlir/include/mlir/IR/OperationSupport.h:0:14
#19 0x0000556c580fde8d llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#20 0x0000556c580fde8d llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#21 0x0000556c580fde8d (anonymous namespace)::OperationVerifier::verifyOnEntrance(mlir::Operation&) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:175:25
#22 0x0000556c580fde8d auto (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&)::$_2::operator()<mlir::Operation>(mlir::Operation*) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:296:45
#23 0x0000556c580fde8d auto (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&)::$_1::operator()<(anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&)::$_2>((anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&)::$_2&&, llvm::PointerUnion<mlir::Operation*, mlir::Block*>) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:276:16
#24 0x0000556c580fde8d (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:295:16
#25 0x0000556c580fde8d (anonymous namespace)::OperationVerifier::verifyOpAndDominance(mlir::Operation&) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:81:14
#26 0x0000556c5810324d llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#27 0x0000556c5810324d llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#28 0x0000556c5810324d (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3::operator()(mlir::Operation*) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:225:9
#29 0x0000556c5810324d auto void mlir::parallelForEach<mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3>(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3&&)::'lambda'(mlir::Operation**&&)::operator()<mlir::Operation*&>(mlir::Operation**&&) const /workdir/llvm-project/mlir/include/mlir/IR/Threading.h:120:12
#30 0x0000556c580fe941 llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#31 0x0000556c580fe941 llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#32 0x0000556c580fe941 llvm::LogicalResult mlir::failableParallelForEach<mlir::Operation**, void mlir::parallelForEach<mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3>(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3&&)::'lambda'(mlir::Operation**&&)>(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3&&) /workdir/llvm-project/mlir/include/mlir/IR/Threading.h:46:11
#33 0x0000556c580fe941 void mlir::parallelForEach<mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3>(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3&&) /workdir/llvm-project/mlir/include/mlir/IR/Threading.h:119:9
#34 0x0000556c580fe941 void mlir::parallelForEach<llvm::SmallVector<mlir::Operation*, 6u>&, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3>(mlir::MLIRContext*, llvm::SmallVector<mlir::Operation*, 6u>&, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3&&) /workdir/llvm-project/mlir/include/mlir/IR/Threading.h:131:3
#35 0x0000556c580fe941 (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:224:3
#36 0x0000556c580fe941 auto (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&)::$_0::operator()<mlir::Operation>(mlir::Operation*) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:287:53
#37 0x0000556c580fe941 auto (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&)::$_1::operator()<(anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&)::$_0>((anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&)::$_0&&, llvm::PointerUnion<mlir::Operation*, mlir::Block*>) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:276:16
#38 0x0000556c580fe941 (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:287:15
#39 0x0000556c580fe941 (anonymous namespace)::OperationVerifier::verifyOpAndDominance(mlir::Operation&) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:81:14
#40 0x0000556c5810324d llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#41 0x0000556c5810324d llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#42 0x0000556c5810324d (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3::operator()(mlir::Operation*) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:225:9
#43 0x0000556c5810324d auto void mlir::parallelForEach<mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3>(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3&&)::'lambda'(mlir::Operation**&&)::operator()<mlir::Operation*&>(mlir::Operation**&&) const /workdir/llvm-project/mlir/include/mlir/IR/Threading.h:120:12
#44 0x0000556c580fe941 llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#45 0x0000556c580fe941 llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#46 0x0000556c580fe941 llvm::LogicalResult mlir::failableParallelForEach<mlir::Operation**, void mlir::parallelForEach<mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3>(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3&&)::'lambda'(mlir::Operation**&&)>(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3&&) /workdir/llvm-project/mlir/include/mlir/IR/Threading.h:46:11
#47 0x0000556c580fe941 void mlir::parallelForEach<mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3>(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3&&) /workdir/llvm-project/mlir/include/mlir/IR/Threading.h:119:9
#48 0x0000556c580fe941 void mlir::parallelForEach<llvm::SmallVector<mlir::Operation*, 6u>&, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3>(mlir::MLIRContext*, llvm::SmallVector<mlir::Operation*, 6u>&, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3&&) /workdir/llvm-project/mlir/include/mlir/IR/Threading.h:131:3
#49 0x0000556c580fe941 (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:224:3
#50 0x0000556c580fe941 auto (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&)::$_0::operator()<mlir::Operation>(mlir::Operation*) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:287:53
#51 0x0000556c580fe941 auto (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&)::$_1::operator()<(anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&)::$_0>((anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&)::$_0&&, llvm::PointerUnion<mlir::Operation*, mlir::Block*>) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:276:16
#52 0x0000556c580fe941 (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:287:15
#53 0x0000556c580fe941 (anonymous namespace)::OperationVerifier::verifyOpAndDominance(mlir::Operation&) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:81:14
#54 0x0000556c580fc9f3 mlir::verify(mlir::Operation*, bool) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:426:1
#55 0x0000556c5f7eed69 llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#56 0x0000556c5f7eed69 llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#57 0x0000556c5f7eed69 (anonymous namespace)::OperationParser::finalize() /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:931:48
#58 0x0000556c5f7eed69 (anonymous namespace)::TopLevelOperationParser::parse(mlir::Block*, mlir::Location) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:2840:20
#59 0x0000556c5f7eed69 mlir::parseAsmSourceFile(llvm::SourceMgr const&, mlir::Block*, mlir::ParserConfig const&, mlir::AsmParserState*, mlir::AsmParserCodeCompleteContext*) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:2894:41
#60 0x0000556c57b571de mlir::parseSourceFile(std::shared_ptr<llvm::SourceMgr> const&, mlir::Block*, mlir::ParserConfig const&, mlir::LocationAttr*) /workdir/llvm-project/mlir/lib/Parser/Parser.cpp:47:1
#61 0x0000556c57b54425 llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#62 0x0000556c57b54425 llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#63 0x0000556c57b54425 mlir::OwningOpRef<mlir::ModuleOp> mlir::detail::parseSourceFile<mlir::ModuleOp, std::shared_ptr<llvm::SourceMgr> const&>(mlir::ParserConfig const&, std::shared_ptr<llvm::SourceMgr> const&) /workdir/llvm-project/mlir/include/mlir/Parser/Parser.h:159:7
#64 0x0000556c57b53cd8 std::enable_if<__and_<std::__not_<std::__is_tuple_like<mlir::ModuleOp> >, std::is_move_constructible<mlir::ModuleOp>, std::is_move_assignable<mlir::ModuleOp> >::value, void>::type std::swap<mlir::ModuleOp>(mlir::ModuleOp&, mlir::ModuleOp&) /usr/bin/../lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/bits/move.h:205:11
#65 0x0000556c57b53cd8 mlir::OwningOpRef<mlir::ModuleOp>::release() /workdir/llvm-project/mlir/include/mlir/IR/OwningOpRef.h:69:5
#66 0x0000556c57b53cd8 mlir::OwningOpRef<mlir::ModuleOp>::operator mlir::OwningOpRef<mlir::Operation*>() && /workdir/llvm-project/mlir/include/mlir/IR/OwningOpRef.h:64:51
#67 0x0000556c57b53cd8 mlir::parseSourceFileForTool(std::shared_ptr<llvm::SourceMgr> const&, mlir::ParserConfig const&, bool) /workdir/llvm-project/mlir/include/mlir/Tools/ParseUtilities.h:31:12
#68 0x0000556c57b526ff performActions(llvm::raw_ostream&, std::shared_ptr<llvm::SourceMgr> const&, mlir::MLIRContext*, mlir::MlirOptMainConfig const&) /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:0:33
#69 0x0000556c57b51fe4 processBuffer(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, mlir::MlirOptMainConfig const&, mlir::DialectRegistry&, llvm::ThreadPoolInterface*) /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:551:12
#70 0x0000556c57b51fe4 mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_3::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::raw_ostream&) const /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:635:12
#71 0x0000556c57b51fe4 llvm::LogicalResult llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::raw_ostream&)>::callback_fn<mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_3>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::raw_ostream&) /workdir/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#72 0x0000556c57d8aeac std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >::~unique_ptr() /usr/bin/../lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/bits/unique_ptr.h:360:6
#73 0x0000556c57d8aeac llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::raw_ostream&)>::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::raw_ostream&) const /workdir/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#74 0x0000556c57d8b4f7 llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#75 0x0000556c57d8b4f7 llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#76 0x0000556c57d8b4f7 mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef)::$_0::operator()(llvm::StringRef) const /workdir/llvm-project/mlir/lib/Support/ToolUtilities.cpp:86:9
#77 0x0000556c57d8a7cb void llvm::interleave<llvm::StringRef const*, mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef)::$_0, void llvm::interleave<llvm::SmallVector<llvm::StringRef, 8u>, mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef)::$_0, llvm::raw_ostream, llvm::StringRef>(llvm::SmallVector<llvm::StringRef, 8u> const&, llvm::raw_ostream&, mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef)::$_0, llvm::StringRef const&)::'lambda'(), void>(llvm::SmallVector<llvm::StringRef, 8u>, llvm::SmallVector<llvm::StringRef, 8u>, mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef)::$_0, llvm::raw_ostream) /workdir/llvm-project/llvm/include/llvm/ADT/STLExtras.h:0:3
#78 0x0000556c57d8a7cb void llvm::interleave<llvm::SmallVector<llvm::StringRef, 8u>, mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef)::$_0, llvm::raw_ostream, llvm::StringRef>(llvm::SmallVector<llvm::StringRef, 8u> const&, llvm::raw_ostream&, mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef)::$_0, llvm::StringRef const&) /workdir/llvm-project/llvm/include/llvm/ADT/STLExtras.h:2238:3
#79 0x0000556c57d8a7cb mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef) /workdir/llvm-project/mlir/lib/Support/ToolUtilities.cpp:89:3
#80 0x0000556c57b40b1d mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&) /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:638:10
#81 0x0000556c57b41405 llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#82 0x0000556c57b41405 llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#83 0x0000556c57b41405 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&) /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:679:7
#84 0x0000556c57b41b53 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&) /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:695:10
#85 0x0000556c4da75987 llvm::LogicalResult::succeeded() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:40:45
#86 0x0000556c4da75987 mlir::asMainReturnCode(llvm::LogicalResult) /workdir/llvm-project/mlir/include/mlir/Tools/mlir-opt/MlirOptMain.h:355:12
#87 0x0000556c4da75987 main /workdir/llvm-project/mlir/tools/mlir-opt/mlir-opt.cpp:339:10
#88 0x00007f442acccd90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#89 0x00007f442accce40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#90 0x0000556c4d9b4ee5 _start (/workdir/llvm-project/build/bin/mlir-opt+0xf5b5ee5)
Aborted (core dumped)
```


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: None (sairam2661)

<details>
Test commit: [5a86dc996c26299de63effc927075dcbfb924167](https://github.com/llvm/llvm-project/pull/157520/commits/5a86dc996c26299de63effc927075dcbfb924167)

**Description**:
The test dialect crashes when it tries to verify the element count equality between operands and results that have dynamic shapes. 

**Steps to reproduce**
- Minimal MLIR program (test.mlir)
```
"builtin.module"() ({
  "func.func"() &lt;{
    function_type = (tensor&lt;2xi32&gt;) -&gt; (),
    sym_name = "test"
  }&gt; ({
  ^bb0(%arg0: tensor&lt;2xi32&gt;):
    %0 = "test.operand0_and_result_have_same_element_count"(%arg0, %arg0) : 
         (tensor&lt;2xi32&gt;, tensor&lt;2xi32&gt;) -&gt; tensor&lt;?x?xf32&gt;
    "func.return"() : () -&gt; ()
  }) : () -&gt; ()
}) : () -&gt; ()
```

**Command**
```
mlir-opt test.mlir
```

**Output**
```
mlir-opt: /workdir/llvm-project/build/tools/mlir/include/mlir/IR/BuiltinTypeInterfaces.h.inc:436: int64_t mlir::ShapedType::getNumElements() const: Assertion `hasStaticShape() &amp;&amp; "cannot get element count of dynamic shaped type"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.      Program arguments: /workdir/llvm-project/build/bin/mlir-opt test.mlir
 #<!-- -->0 0x0000556c4d9f0a1b backtrace (/workdir/llvm-project/build/bin/mlir-opt+0xf5f1a1b)
 #<!-- -->1 0x0000556c4dc75cfc llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /workdir/llvm-project/llvm/lib/Support/Unix/Signals.inc:834:13
 #<!-- -->2 0x0000556c4dc7017f llvm::sys::RunSignalHandlers() /workdir/llvm-project/llvm/lib/Support/Signals.cpp:105:18
 #<!-- -->3 0x0000556c4dc78316 SignalHandler(int, siginfo_t*, void*) /workdir/llvm-project/llvm/lib/Support/Unix/Signals.inc:426:38
 #<!-- -->4 0x00007f442ace5520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->5 0x00007f442ad399fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->6 0x00007f442ace5476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->7 0x00007f442accb7f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->8 0x00007f442accb71b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->9 0x00007f442acdce96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
#<!-- -->10 0x0000556c4ed2c586 mlir::ShapedType::getNumElements() const /workdir/llvm-project/build/tools/mlir/include/mlir/IR/BuiltinTypeInterfaces.h.inc:436:7
#<!-- -->11 0x0000556c58d2545a test::OperandZeroAndResultHaveSameElementCount::verifyInvariantsImpl() /workdir/llvm-project/build/tools/mlir/test/lib/Dialect/Test/TestOps.cpp.inc:54821:106
#<!-- -->12 0x0000556c58d5c2bd mlir::OpTrait::OpInvariants&lt;test::OperandZeroAndResultHaveSameElementCount&gt;::verifyTrait(mlir::Operation*) /workdir/llvm-project/mlir/include/mlir/IR/OpDefinition.h:440:5
#<!-- -->13 0x0000556c58d5c2bd llvm::LogicalResult mlir::op_definition_impl::verifyTrait&lt;mlir::OpTrait::OpInvariants&lt;test::OperandZeroAndResultHaveSameElementCount&gt; &gt;(mlir::Operation*) /workdir/llvm-project/mlir/include/mlir/IR/OpDefinition.h:1637:12
#<!-- -->14 0x0000556c58d5c2bd llvm::LogicalResult mlir::op_definition_impl::verifyTraits&lt;mlir::OpTrait::ZeroRegions&lt;test::OperandZeroAndResultHaveSameElementCount&gt;, mlir::OpTrait::OneResult&lt;test::OperandZeroAndResultHaveSameElementCount&gt;, mlir::OpTrait::OneTypedResult&lt;mlir::ShapedType&gt;::Impl&lt;test::OperandZeroAndResultHaveSameElementCount&gt;, mlir::OpTrait::ZeroSuccessors&lt;test::OperandZeroAndResultHaveSameElementCount&gt;, mlir::OpTrait::NOperands&lt;2u&gt;::Impl&lt;test::OperandZeroAndResultHaveSameElementCount&gt;, mlir::OpTrait::OpInvariants&lt;test::OperandZeroAndResultHaveSameElementCount&gt; &gt;(mlir::Operation*) /workdir/llvm-project/mlir/include/mlir/IR/OpDefinition.h:1645:29
#<!-- -->15 0x0000556c58d5c13e llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#<!-- -->16 0x0000556c58d5c13e llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#<!-- -->17 0x0000556c58d5c13e mlir::Op&lt;test::OperandZeroAndResultHaveSameElementCount, mlir::OpTrait::ZeroRegions, mlir::OpTrait::OneResult, mlir::OpTrait::OneTypedResult&lt;mlir::ShapedType&gt;::Impl, mlir::OpTrait::ZeroSuccessors, mlir::OpTrait::NOperands&lt;2u&gt;::Impl, mlir::OpTrait::OpInvariants&gt;::verifyInvariants(mlir::Operation*) /workdir/llvm-project/mlir/include/mlir/IR/OpDefinition.h:2052:9
#<!-- -->18 0x0000556c58d5bb21 mlir::RegisteredOperationName::Model&lt;test::OperandZeroAndResultHaveSameElementCount&gt;::verifyInvariants(mlir::Operation*) /workdir/llvm-project/mlir/include/mlir/IR/OperationSupport.h:0:14
#<!-- -->19 0x0000556c580fde8d llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#<!-- -->20 0x0000556c580fde8d llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#<!-- -->21 0x0000556c580fde8d (anonymous namespace)::OperationVerifier::verifyOnEntrance(mlir::Operation&amp;) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:175:25
#<!-- -->22 0x0000556c580fde8d auto (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;)::$_2::operator()&lt;mlir::Operation&gt;(mlir::Operation*) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:296:45
#<!-- -->23 0x0000556c580fde8d auto (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;)::$_1::operator()&lt;(anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;)::$_2&gt;((anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;)::$_2&amp;&amp;, llvm::PointerUnion&lt;mlir::Operation*, mlir::Block*&gt;) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:276:16
#<!-- -->24 0x0000556c580fde8d (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:295:16
#<!-- -->25 0x0000556c580fde8d (anonymous namespace)::OperationVerifier::verifyOpAndDominance(mlir::Operation&amp;) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:81:14
#<!-- -->26 0x0000556c5810324d llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#<!-- -->27 0x0000556c5810324d llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#<!-- -->28 0x0000556c5810324d (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3::operator()(mlir::Operation*) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:225:9
#<!-- -->29 0x0000556c5810324d auto void mlir::parallelForEach&lt;mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&gt;(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&amp;&amp;)::'lambda'(mlir::Operation**&amp;&amp;)::operator()&lt;mlir::Operation*&amp;&gt;(mlir::Operation**&amp;&amp;) const /workdir/llvm-project/mlir/include/mlir/IR/Threading.h:120:12
#<!-- -->30 0x0000556c580fe941 llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#<!-- -->31 0x0000556c580fe941 llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#<!-- -->32 0x0000556c580fe941 llvm::LogicalResult mlir::failableParallelForEach&lt;mlir::Operation**, void mlir::parallelForEach&lt;mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&gt;(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&amp;&amp;)::'lambda'(mlir::Operation**&amp;&amp;)&gt;(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&amp;&amp;) /workdir/llvm-project/mlir/include/mlir/IR/Threading.h:46:11
#<!-- -->33 0x0000556c580fe941 void mlir::parallelForEach&lt;mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&gt;(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&amp;&amp;) /workdir/llvm-project/mlir/include/mlir/IR/Threading.h:119:9
#<!-- -->34 0x0000556c580fe941 void mlir::parallelForEach&lt;llvm::SmallVector&lt;mlir::Operation*, 6u&gt;&amp;, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&gt;(mlir::MLIRContext*, llvm::SmallVector&lt;mlir::Operation*, 6u&gt;&amp;, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&amp;&amp;) /workdir/llvm-project/mlir/include/mlir/IR/Threading.h:131:3
#<!-- -->35 0x0000556c580fe941 (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:224:3
#<!-- -->36 0x0000556c580fe941 auto (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;)::$_0::operator()&lt;mlir::Operation&gt;(mlir::Operation*) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:287:53
#<!-- -->37 0x0000556c580fe941 auto (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;)::$_1::operator()&lt;(anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;)::$_0&gt;((anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;)::$_0&amp;&amp;, llvm::PointerUnion&lt;mlir::Operation*, mlir::Block*&gt;) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:276:16
#<!-- -->38 0x0000556c580fe941 (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:287:15
#<!-- -->39 0x0000556c580fe941 (anonymous namespace)::OperationVerifier::verifyOpAndDominance(mlir::Operation&amp;) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:81:14
#<!-- -->40 0x0000556c5810324d llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#<!-- -->41 0x0000556c5810324d llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#<!-- -->42 0x0000556c5810324d (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3::operator()(mlir::Operation*) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:225:9
#<!-- -->43 0x0000556c5810324d auto void mlir::parallelForEach&lt;mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&gt;(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&amp;&amp;)::'lambda'(mlir::Operation**&amp;&amp;)::operator()&lt;mlir::Operation*&amp;&gt;(mlir::Operation**&amp;&amp;) const /workdir/llvm-project/mlir/include/mlir/IR/Threading.h:120:12
#<!-- -->44 0x0000556c580fe941 llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#<!-- -->45 0x0000556c580fe941 llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#<!-- -->46 0x0000556c580fe941 llvm::LogicalResult mlir::failableParallelForEach&lt;mlir::Operation**, void mlir::parallelForEach&lt;mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&gt;(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&amp;&amp;)::'lambda'(mlir::Operation**&amp;&amp;)&gt;(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&amp;&amp;) /workdir/llvm-project/mlir/include/mlir/IR/Threading.h:46:11
#<!-- -->47 0x0000556c580fe941 void mlir::parallelForEach&lt;mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&gt;(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&amp;&amp;) /workdir/llvm-project/mlir/include/mlir/IR/Threading.h:119:9
#<!-- -->48 0x0000556c580fe941 void mlir::parallelForEach&lt;llvm::SmallVector&lt;mlir::Operation*, 6u&gt;&amp;, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&gt;(mlir::MLIRContext*, llvm::SmallVector&lt;mlir::Operation*, 6u&gt;&amp;, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&amp;&amp;) /workdir/llvm-project/mlir/include/mlir/IR/Threading.h:131:3
#<!-- -->49 0x0000556c580fe941 (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:224:3
#<!-- -->50 0x0000556c580fe941 auto (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;)::$_0::operator()&lt;mlir::Operation&gt;(mlir::Operation*) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:287:53
#<!-- -->51 0x0000556c580fe941 auto (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;)::$_1::operator()&lt;(anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;)::$_0&gt;((anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;)::$_0&amp;&amp;, llvm::PointerUnion&lt;mlir::Operation*, mlir::Block*&gt;) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:276:16
#<!-- -->52 0x0000556c580fe941 (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:287:15
#<!-- -->53 0x0000556c580fe941 (anonymous namespace)::OperationVerifier::verifyOpAndDominance(mlir::Operation&amp;) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:81:14
#<!-- -->54 0x0000556c580fc9f3 mlir::verify(mlir::Operation*, bool) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:426:1
#<!-- -->55 0x0000556c5f7eed69 llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#<!-- -->56 0x0000556c5f7eed69 llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#<!-- -->57 0x0000556c5f7eed69 (anonymous namespace)::OperationParser::finalize() /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:931:48
#<!-- -->58 0x0000556c5f7eed69 (anonymous namespace)::TopLevelOperationParser::parse(mlir::Block*, mlir::Location) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:2840:20
#<!-- -->59 0x0000556c5f7eed69 mlir::parseAsmSourceFile(llvm::SourceMgr const&amp;, mlir::Block*, mlir::ParserConfig const&amp;, mlir::AsmParserState*, mlir::AsmParserCodeCompleteContext*) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:2894:41
#<!-- -->60 0x0000556c57b571de mlir::parseSourceFile(std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::Block*, mlir::ParserConfig const&amp;, mlir::LocationAttr*) /workdir/llvm-project/mlir/lib/Parser/Parser.cpp:47:1
#<!-- -->61 0x0000556c57b54425 llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#<!-- -->62 0x0000556c57b54425 llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#<!-- -->63 0x0000556c57b54425 mlir::OwningOpRef&lt;mlir::ModuleOp&gt; mlir::detail::parseSourceFile&lt;mlir::ModuleOp, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;&gt;(mlir::ParserConfig const&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;) /workdir/llvm-project/mlir/include/mlir/Parser/Parser.h:159:7
#<!-- -->64 0x0000556c57b53cd8 std::enable_if&lt;__and_&lt;std::__not_&lt;std::__is_tuple_like&lt;mlir::ModuleOp&gt; &gt;, std::is_move_constructible&lt;mlir::ModuleOp&gt;, std::is_move_assignable&lt;mlir::ModuleOp&gt; &gt;::value, void&gt;::type std::swap&lt;mlir::ModuleOp&gt;(mlir::ModuleOp&amp;, mlir::ModuleOp&amp;) /usr/bin/../lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/bits/move.h:205:11
#<!-- -->65 0x0000556c57b53cd8 mlir::OwningOpRef&lt;mlir::ModuleOp&gt;::release() /workdir/llvm-project/mlir/include/mlir/IR/OwningOpRef.h:69:5
#<!-- -->66 0x0000556c57b53cd8 mlir::OwningOpRef&lt;mlir::ModuleOp&gt;::operator mlir::OwningOpRef&lt;mlir::Operation*&gt;() &amp;&amp; /workdir/llvm-project/mlir/include/mlir/IR/OwningOpRef.h:64:51
#<!-- -->67 0x0000556c57b53cd8 mlir::parseSourceFileForTool(std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::ParserConfig const&amp;, bool) /workdir/llvm-project/mlir/include/mlir/Tools/ParseUtilities.h:31:12
#<!-- -->68 0x0000556c57b526ff performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:0:33
#<!-- -->69 0x0000556c57b51fe4 processBuffer(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, mlir::MlirOptMainConfig const&amp;, mlir::DialectRegistry&amp;, llvm::ThreadPoolInterface*) /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:551:12
#<!-- -->70 0x0000556c57b51fe4 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_3::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;) const /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:635:12
#<!-- -->71 0x0000556c57b51fe4 llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_3&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;) /workdir/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->72 0x0000556c57d8aeac std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;::~unique_ptr() /usr/bin/../lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/bits/unique_ptr.h:360:6
#<!-- -->73 0x0000556c57d8aeac llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;)&gt;::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;) const /workdir/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->74 0x0000556c57d8b4f7 llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#<!-- -->75 0x0000556c57d8b4f7 llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#<!-- -->76 0x0000556c57d8b4f7 mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef)::$_0::operator()(llvm::StringRef) const /workdir/llvm-project/mlir/lib/Support/ToolUtilities.cpp:86:9
#<!-- -->77 0x0000556c57d8a7cb void llvm::interleave&lt;llvm::StringRef const*, mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef)::$_0, void llvm::interleave&lt;llvm::SmallVector&lt;llvm::StringRef, 8u&gt;, mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef)::$_0, llvm::raw_ostream, llvm::StringRef&gt;(llvm::SmallVector&lt;llvm::StringRef, 8u&gt; const&amp;, llvm::raw_ostream&amp;, mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef)::$_0, llvm::StringRef const&amp;)::'lambda'(), void&gt;(llvm::SmallVector&lt;llvm::StringRef, 8u&gt;, llvm::SmallVector&lt;llvm::StringRef, 8u&gt;, mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef)::$_0, llvm::raw_ostream) /workdir/llvm-project/llvm/include/llvm/ADT/STLExtras.h:0:3
#<!-- -->78 0x0000556c57d8a7cb void llvm::interleave&lt;llvm::SmallVector&lt;llvm::StringRef, 8u&gt;, mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef)::$_0, llvm::raw_ostream, llvm::StringRef&gt;(llvm::SmallVector&lt;llvm::StringRef, 8u&gt; const&amp;, llvm::raw_ostream&amp;, mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef)::$_0, llvm::StringRef const&amp;) /workdir/llvm-project/llvm/include/llvm/ADT/STLExtras.h:2238:3
#<!-- -->79 0x0000556c57d8a7cb mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) /workdir/llvm-project/mlir/lib/Support/ToolUtilities.cpp:89:3
#<!-- -->80 0x0000556c57b40b1d mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:638:10
#<!-- -->81 0x0000556c57b41405 llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#<!-- -->82 0x0000556c57b41405 llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#<!-- -->83 0x0000556c57b41405 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:679:7
#<!-- -->84 0x0000556c57b41b53 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:695:10
#<!-- -->85 0x0000556c4da75987 llvm::LogicalResult::succeeded() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:40:45
#<!-- -->86 0x0000556c4da75987 mlir::asMainReturnCode(llvm::LogicalResult) /workdir/llvm-project/mlir/include/mlir/Tools/mlir-opt/MlirOptMain.h:355:12
#<!-- -->87 0x0000556c4da75987 main /workdir/llvm-project/mlir/tools/mlir-opt/mlir-opt.cpp:339:10
#<!-- -->88 0x00007f442acccd90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->89 0x00007f442accce40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->90 0x0000556c4d9b4ee5 _start (/workdir/llvm-project/build/bin/mlir-opt+0xf5b5ee5)
Aborted (core dumped)
```

</details>


---

