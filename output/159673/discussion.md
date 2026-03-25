# [MLIR][IRDL] Assertion crash with self-referencing IRDL types and duplicate function symbols

**Author:** sairam2661
**URL:** https://github.com/llvm/llvm-project/issues/159673
**Status:** Closed
**Labels:** mlir, crash
**Closed Date:** 2026-03-01T10:07:38Z

## Body

Test commit: [5a86dc996c26299de63effc927075dcbfb924167](https://github.com/llvm/llvm-project/pull/157520/commits/5a86dc996c26299de63effc927075dcbfb924167)

**Description**:
MLIR crashes with `Assertion 'inserted.second && "expected region to contain uniquely named symbol operations"' failed` when a module contains both the IRDL self-referencing parametric types and some duplicate function symbols. Interestingly, I was not able to reproduce this using them individually - the crash only occurs when both are present.

**Steps to reproduce**
- Minimal MLIR program (test.mlir)

```mlir
"builtin.module"() ({
  "irdl.dialect"() <{sym_name = "testd"}> ({
    "irdl.type"() <{sym_name = "self_referencing"}> ({
      %8 = "irdl.any"() : () -> !irdl.attribute
      %9 = "irdl.parametric"(%8) <{base_type = @testd::@self_referencing}> : (!irdl.attribute) -> !irdl.attribute
      "irdl.parameters"(%9) <{names = ["foo"]}> : (!irdl.attribute) -> ()
    }) : () -> ()
  }) : () -> ()
  "func.func"() <{function_type = () -> (), sym_name = "test"}> ({
    "func.return"() : () -> ()
  }) : () -> ()
  "func.func"() <{function_type = () -> (), sym_name = "test"}> ({
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
mlir-opt: /workdir/llvm-project/mlir/lib/IR/SymbolTable.cpp:137: mlir::SymbolTable::SymbolTable(mlir::Operation *): Assertion `inserted.second && "expected region to contain uniquely named symbol operations"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.      Program arguments: /workdir/llvm-project/build/bin/mlir-opt test.mlir
 #0 0x000055832ca94a1b backtrace (/workdir/llvm-project/build/bin/mlir-opt+0xf5f1a1b)
 #1 0x000055832cd19cfc llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /workdir/llvm-project/llvm/lib/Support/Unix/Signals.inc:834:13
 #2 0x000055832cd1417f llvm::sys::RunSignalHandlers() /workdir/llvm-project/llvm/lib/Support/Signals.cpp:105:18
 #3 0x000055832cd1c316 SignalHandler(int, siginfo_t*, void*) /workdir/llvm-project/llvm/lib/Support/Unix/Signals.inc:426:38
 #4 0x00007fcd1acad520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #5 0x00007fcd1ad019fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #6 0x00007fcd1acad476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #7 0x00007fcd1ac937f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #8 0x00007fcd1ac9371b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #9 0x00007fcd1aca4e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
#10 0x00005583371730b4 mlir::SymbolTable::SymbolTable(mlir::Operation*) /workdir/llvm-project/mlir/lib/IR/SymbolTable.cpp:136:5
#11 0x000055833717f709 std::__uniq_ptr_impl<mlir::SymbolTable, std::default_delete<mlir::SymbolTable> >::__uniq_ptr_impl(mlir::SymbolTable*) /usr/bin/../lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/bits/unique_ptr.h:155:56
#12 0x000055833717f709 std::__uniq_ptr_data<mlir::SymbolTable, std::default_delete<mlir::SymbolTable>, true, true>::__uniq_ptr_data(mlir::SymbolTable*) /usr/bin/../lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/bits/unique_ptr.h:210:40
#13 0x000055833717f709 std::unique_ptr<mlir::SymbolTable, std::default_delete<mlir::SymbolTable> >::unique_ptr<std::default_delete<mlir::SymbolTable>, void>(mlir::SymbolTable*) /usr/bin/../lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/bits/unique_ptr.h:282:4
#14 0x000055833717f709 std::_MakeUniq<mlir::SymbolTable>::__single_object std::make_unique<mlir::SymbolTable, mlir::Operation*&>(mlir::Operation*&) /usr/bin/../lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/bits/unique_ptr.h:962:14
#15 0x000055833717f709 mlir::SymbolTableCollection::getSymbolTable(mlir::Operation*) /workdir/llvm-project/mlir/lib/IR/SymbolTable.cpp:997:24
#16 0x000055833717f04e mlir::SymbolTableCollection::lookupSymbolIn(mlir::Operation*, mlir::StringAttr) /workdir/llvm-project/mlir/lib/IR/SymbolTable.cpp:0:10
#17 0x0000558337178494 lookupSymbolInImpl(mlir::Operation*, mlir::SymbolRefAttr, llvm::SmallVectorImpl<mlir::Operation*>&, llvm::function_ref<mlir::Operation* (mlir::Operation*, mlir::StringAttr)>) /workdir/llvm-project/mlir/lib/IR/SymbolTable.cpp:418:8
#18 0x000055833717f42a mlir::SymbolTableCollection::lookupSymbolIn(mlir::Operation*, mlir::SymbolRefAttr, llvm::SmallVectorImpl<mlir::Operation*>&) /workdir/llvm-project/mlir/lib/IR/SymbolTable.cpp:975:1
#19 0x000055833717f255 llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#20 0x000055833717f255 llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#21 0x000055833717f255 mlir::SymbolTableCollection::lookupSymbolIn(mlir::Operation*, mlir::SymbolRefAttr) /workdir/llvm-project/mlir/lib/IR/SymbolTable.cpp:960:7
#22 0x000055832e85e002 checkSymbolIsTypeOrAttribute(mlir::SymbolTableCollection&, mlir::Operation*, mlir::SymbolRefAttr) /workdir/llvm-project/mlir/lib/Dialect/IRDL/IR/IRDL.cpp:261:8
#23 0x000055832e85e54d mlir::irdl::ParametricOp::verifySymbolUses(mlir::SymbolTableCollection&) /workdir/llvm-project/mlir/lib/Dialect/IRDL/IR/IRDL.cpp:288:1
#24 0x000055832e8fe576 mlir::detail::SymbolUserOpInterfaceInterfaceTraits::Model<mlir::irdl::ParametricOp>::verifySymbolUses(mlir::detail::SymbolUserOpInterfaceInterfaceTraits::Concept const*, mlir::Operation*, mlir::SymbolTableCollection&) /workdir/llvm-project/build/tools/mlir/include/mlir/IR/SymbolInterfaces.h.inc:583:3
#25 0x0000558337186b6e llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#26 0x0000558337186b6e llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#27 0x0000558337186b6e mlir::WalkResult::WalkResult(llvm::LogicalResult) /workdir/llvm-project/mlir/include/mlir/Support/WalkResult.h:37:16
#28 0x0000558337186b6e mlir::detail::verifySymbolTable(mlir::Operation*)::$_3::operator()(mlir::Operation*) const /workdir/llvm-project/mlir/lib/IR/SymbolTable.cpp:515:14
#29 0x0000558337186b6e std::optional<mlir::WalkResult> llvm::function_ref<std::optional<mlir::WalkResult> (mlir::Operation*)>::callback_fn<mlir::detail::verifySymbolTable(mlir::Operation*)::$_3>(long, mlir::Operation*) /workdir/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#30 0x0000558337179fdb walkSymbolTable(llvm::MutableArrayRef<mlir::Region>, llvm::function_ref<std::optional<mlir::WalkResult> (mlir::Operation*)>) /workdir/llvm-project/mlir/lib/IR/SymbolTable.cpp:86:42
#31 0x00005583371794e8 mlir::detail::verifySymbolTable(mlir::Operation*) /workdir/llvm-project/mlir/lib/IR/SymbolTable.cpp:520:7
#32 0x000055832e8efa0e llvm::LogicalResult::succeeded() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:40:45
#33 0x000055832e8efa0e llvm::succeeded(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:67:61
#34 0x000055832e8efa0e llvm::LogicalResult mlir::op_definition_impl::verifyRegionTraits<mlir::OpTrait::OneRegion<mlir::irdl::DialectOp>, mlir::OpTrait::ZeroResults<mlir::irdl::DialectOp>, mlir::OpTrait::ZeroSuccessors<mlir::irdl::DialectOp>, mlir::OpTrait::ZeroOperands<mlir::irdl::DialectOp>, mlir::OpTrait::NoTerminator<mlir::irdl::DialectOp>, mlir::OpTrait::OpInvariants<mlir::irdl::DialectOp>, mlir::BytecodeOpInterface::Trait<mlir::irdl::DialectOp>, mlir::OpTrait::IsIsolatedFromAbove<mlir::irdl::DialectOp>, mlir::SymbolOpInterface::Trait<mlir::irdl::DialectOp>, mlir::OpTrait::SymbolTable<mlir::irdl::DialectOp> >(mlir::Operation*) /workdir/llvm-project/mlir/include/mlir/IR/OpDefinition.h:1661:19
#35 0x000055832e8efa0e mlir::Op<mlir::irdl::DialectOp, mlir::OpTrait::OneRegion, mlir::OpTrait::ZeroResults, mlir::OpTrait::ZeroSuccessors, mlir::OpTrait::ZeroOperands, mlir::OpTrait::NoTerminator, mlir::OpTrait::OpInvariants, mlir::BytecodeOpInterface::Trait, mlir::OpTrait::IsIsolatedFromAbove, mlir::SymbolOpInterface::Trait, mlir::OpTrait::SymbolTable>::verifyRegionInvariants(mlir::Operation*) /workdir/llvm-project/mlir/include/mlir/IR/OpDefinition.h:2063:16
#36 0x000055832e8ecaa1 mlir::RegisteredOperationName::Model<mlir::irdl::DialectOp>::verifyRegionInvariants(mlir::Operation*) /workdir/llvm-project/mlir/include/mlir/IR/OperationSupport.h:0:14
#37 0x00005583371a2f6e llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#38 0x00005583371a2f6e llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#39 0x00005583371a2f6e (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:236:25
#40 0x00005583371a2f6e auto (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&)::$_0::operator()<mlir::Operation>(mlir::Operation*) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:287:53
#41 0x00005583371a2f6e auto (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&)::$_1::operator()<(anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&)::$_0>((anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&)::$_0&&, llvm::PointerUnion<mlir::Operation*, mlir::Block*>) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:276:16
#42 0x00005583371a2f6e (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:287:15
#43 0x00005583371a2f6e (anonymous namespace)::OperationVerifier::verifyOpAndDominance(mlir::Operation&) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:81:14
#44 0x00005583371a724d llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#45 0x00005583371a724d llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#46 0x00005583371a724d (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3::operator()(mlir::Operation*) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:225:9
#47 0x00005583371a724d auto void mlir::parallelForEach<mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3>(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3&&)::'lambda'(mlir::Operation**&&)::operator()<mlir::Operation*&>(mlir::Operation**&&) const /workdir/llvm-project/mlir/include/mlir/IR/Threading.h:120:12
#48 0x00005583371a2941 llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#49 0x00005583371a2941 llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#50 0x00005583371a2941 llvm::LogicalResult mlir::failableParallelForEach<mlir::Operation**, void mlir::parallelForEach<mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3>(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3&&)::'lambda'(mlir::Operation**&&)>(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3&&) /workdir/llvm-project/mlir/include/mlir/IR/Threading.h:46:11
#51 0x00005583371a2941 void mlir::parallelForEach<mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3>(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3&&) /workdir/llvm-project/mlir/include/mlir/IR/Threading.h:119:9
#52 0x00005583371a2941 void mlir::parallelForEach<llvm::SmallVector<mlir::Operation*, 6u>&, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3>(mlir::MLIRContext*, llvm::SmallVector<mlir::Operation*, 6u>&, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3&&) /workdir/llvm-project/mlir/include/mlir/IR/Threading.h:131:3
#53 0x00005583371a2941 (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:224:3
#54 0x00005583371a2941 auto (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&)::$_0::operator()<mlir::Operation>(mlir::Operation*) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:287:53
#55 0x00005583371a2941 auto (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&)::$_1::operator()<(anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&)::$_0>((anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&)::$_0&&, llvm::PointerUnion<mlir::Operation*, mlir::Block*>) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:276:16
#56 0x00005583371a2941 (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:287:15
#57 0x00005583371a2941 (anonymous namespace)::OperationVerifier::verifyOpAndDominance(mlir::Operation&) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:81:14
#58 0x00005583371a724d llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#59 0x00005583371a724d llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#60 0x00005583371a724d (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3::operator()(mlir::Operation*) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:225:9
#61 0x00005583371a724d auto void mlir::parallelForEach<mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3>(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3&&)::'lambda'(mlir::Operation**&&)::operator()<mlir::Operation*&>(mlir::Operation**&&) const /workdir/llvm-project/mlir/include/mlir/IR/Threading.h:120:12
#62 0x00005583371a2941 llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#63 0x00005583371a2941 llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#64 0x00005583371a2941 llvm::LogicalResult mlir::failableParallelForEach<mlir::Operation**, void mlir::parallelForEach<mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3>(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3&&)::'lambda'(mlir::Operation**&&)>(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3&&) /workdir/llvm-project/mlir/include/mlir/IR/Threading.h:46:11
#65 0x00005583371a2941 void mlir::parallelForEach<mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3>(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3&&) /workdir/llvm-project/mlir/include/mlir/IR/Threading.h:119:9
#66 0x00005583371a2941 void mlir::parallelForEach<llvm::SmallVector<mlir::Operation*, 6u>&, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3>(mlir::MLIRContext*, llvm::SmallVector<mlir::Operation*, 6u>&, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3&&) /workdir/llvm-project/mlir/include/mlir/IR/Threading.h:131:3
#67 0x00005583371a2941 (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:224:3
#68 0x00005583371a2941 auto (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&)::$_0::operator()<mlir::Operation>(mlir::Operation*) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:287:53
#69 0x00005583371a2941 auto (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&)::$_1::operator()<(anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&)::$_0>((anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&)::$_0&&, llvm::PointerUnion<mlir::Operation*, mlir::Block*>) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:276:16
#70 0x00005583371a2941 (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:287:15
#71 0x00005583371a2941 (anonymous namespace)::OperationVerifier::verifyOpAndDominance(mlir::Operation&) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:81:14
#72 0x00005583371a09f3 mlir::verify(mlir::Operation*, bool) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:426:1
#73 0x000055833e892d69 llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#74 0x000055833e892d69 llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#75 0x000055833e892d69 (anonymous namespace)::OperationParser::finalize() /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:931:48
#76 0x000055833e892d69 (anonymous namespace)::TopLevelOperationParser::parse(mlir::Block*, mlir::Location) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:2840:20
#77 0x000055833e892d69 mlir::parseAsmSourceFile(llvm::SourceMgr const&, mlir::Block*, mlir::ParserConfig const&, mlir::AsmParserState*, mlir::AsmParserCodeCompleteContext*) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:2894:41
#78 0x0000558336bfb1de mlir::parseSourceFile(std::shared_ptr<llvm::SourceMgr> const&, mlir::Block*, mlir::ParserConfig const&, mlir::LocationAttr*) /workdir/llvm-project/mlir/lib/Parser/Parser.cpp:47:1
#79 0x0000558336bf8425 llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#80 0x0000558336bf8425 llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#81 0x0000558336bf8425 mlir::OwningOpRef<mlir::ModuleOp> mlir::detail::parseSourceFile<mlir::ModuleOp, std::shared_ptr<llvm::SourceMgr> const&>(mlir::ParserConfig const&, std::shared_ptr<llvm::SourceMgr> const&) /workdir/llvm-project/mlir/include/mlir/Parser/Parser.h:159:7
#82 0x0000558336bf7cd8 std::enable_if<__and_<std::__not_<std::__is_tuple_like<mlir::ModuleOp> >, std::is_move_constructible<mlir::ModuleOp>, std::is_move_assignable<mlir::ModuleOp> >::value, void>::type std::swap<mlir::ModuleOp>(mlir::ModuleOp&, mlir::ModuleOp&) /usr/bin/../lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/bits/move.h:205:11
#83 0x0000558336bf7cd8 mlir::OwningOpRef<mlir::ModuleOp>::release() /workdir/llvm-project/mlir/include/mlir/IR/OwningOpRef.h:69:5
#84 0x0000558336bf7cd8 mlir::OwningOpRef<mlir::ModuleOp>::operator mlir::OwningOpRef<mlir::Operation*>() && /workdir/llvm-project/mlir/include/mlir/IR/OwningOpRef.h:64:51
#85 0x0000558336bf7cd8 mlir::parseSourceFileForTool(std::shared_ptr<llvm::SourceMgr> const&, mlir::ParserConfig const&, bool) /workdir/llvm-project/mlir/include/mlir/Tools/ParseUtilities.h:31:12
#86 0x0000558336bf66ff performActions(llvm::raw_ostream&, std::shared_ptr<llvm::SourceMgr> const&, mlir::MLIRContext*, mlir::MlirOptMainConfig const&) /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:0:33
#87 0x0000558336bf5fe4 processBuffer(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, mlir::MlirOptMainConfig const&, mlir::DialectRegistry&, llvm::ThreadPoolInterface*) /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:551:12
#88 0x0000558336bf5fe4 mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_3::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::raw_ostream&) const /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:635:12
#89 0x0000558336bf5fe4 llvm::LogicalResult llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::raw_ostream&)>::callback_fn<mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_3>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::raw_ostream&) /workdir/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#90 0x0000558336e2eeac std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >::~unique_ptr() /usr/bin/../lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/bits/unique_ptr.h:360:6
#91 0x0000558336e2eeac llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::raw_ostream&)>::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::raw_ostream&) const /workdir/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#92 0x0000558336e2f4f7 llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#93 0x0000558336e2f4f7 llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#94 0x0000558336e2f4f7 mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef)::$_0::operator()(llvm::StringRef) const /workdir/llvm-project/mlir/lib/Support/ToolUtilities.cpp:86:9
#95 0x0000558336e2e7cb void llvm::interleave<llvm::StringRef const*, mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef)::$_0, void llvm::interleave<llvm::SmallVector<llvm::StringRef, 8u>, mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef)::$_0, llvm::raw_ostream, llvm::StringRef>(llvm::SmallVector<llvm::StringRef, 8u> const&, llvm::raw_ostream&, mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef)::$_0, llvm::StringRef const&)::'lambda'(), void>(llvm::SmallVector<llvm::StringRef, 8u>, llvm::SmallVector<llvm::StringRef, 8u>, mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef)::$_0, llvm::raw_ostream) /workdir/llvm-project/llvm/include/llvm/ADT/STLExtras.h:0:3
#96 0x0000558336e2e7cb void llvm::interleave<llvm::SmallVector<llvm::StringRef, 8u>, mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef)::$_0, llvm::raw_ostream, llvm::StringRef>(llvm::SmallVector<llvm::StringRef, 8u> const&, llvm::raw_ostream&, mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef)::$_0, llvm::StringRef const&) /workdir/llvm-project/llvm/include/llvm/ADT/STLExtras.h:2238:3
#97 0x0000558336e2e7cb mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef) /workdir/llvm-project/mlir/lib/Support/ToolUtilities.cpp:89:3
#98 0x0000558336be4b1d mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&) /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:638:10
#99 0x0000558336be5405 llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#100 0x0000558336be5405 llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#101 0x0000558336be5405 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&) /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:679:7
#102 0x0000558336be5b53 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&) /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:695:10
#103 0x000055832cb19987 llvm::LogicalResult::succeeded() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:40:45
#104 0x000055832cb19987 mlir::asMainReturnCode(llvm::LogicalResult) /workdir/llvm-project/mlir/include/mlir/Tools/mlir-opt/MlirOptMain.h:355:12
#105 0x000055832cb19987 main /workdir/llvm-project/mlir/tools/mlir-opt/mlir-opt.cpp:339:10
#106 0x00007fcd1ac94d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#107 0x00007fcd1ac94e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#108 0x000055832ca58ee5 _start (/workdir/llvm-project/build/bin/mlir-opt+0xf5b5ee5)
Aborted (core dumped)
```


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: None (sairam2661)

<details>
Test commit: [5a86dc996c26299de63effc927075dcbfb924167](https://github.com/llvm/llvm-project/pull/157520/commits/5a86dc996c26299de63effc927075dcbfb924167)

**Description**:
MLIR crashes with `Assertion 'inserted.second &amp;&amp; "expected region to contain uniquely named symbol operations"' failed` when a module contains both the IRDL self-referencing parametric types and some duplicate function symbols. Interestingly, I was not able to reproduce this using them individually - the crash only occurs when both are present.

**Steps to reproduce**
- Minimal MLIR program (test.mlir)
```
"builtin.module"() ({
  "irdl.dialect"() &lt;{sym_name = "testd"}&gt; ({
    "irdl.type"() &lt;{sym_name = "self_referencing"}&gt; ({
      %8 = "irdl.any"() : () -&gt; !irdl.attribute
      %9 = "irdl.parametric"(%8) &lt;{base_type = @<!-- -->testd::@<!-- -->self_referencing}&gt; : (!irdl.attribute) -&gt; !irdl.attribute
      "irdl.parameters"(%9) &lt;{names = ["foo"]}&gt; : (!irdl.attribute) -&gt; ()
    }) : () -&gt; ()
  }) : () -&gt; ()
  "func.func"() &lt;{function_type = () -&gt; (), sym_name = "test"}&gt; ({
    "func.return"() : () -&gt; ()
  }) : () -&gt; ()
  "func.func"() &lt;{function_type = () -&gt; (), sym_name = "test"}&gt; ({
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
mlir-opt: /workdir/llvm-project/mlir/lib/IR/SymbolTable.cpp:137: mlir::SymbolTable::SymbolTable(mlir::Operation *): Assertion `inserted.second &amp;&amp; "expected region to contain uniquely named symbol operations"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.      Program arguments: /workdir/llvm-project/build/bin/mlir-opt test.mlir
 #<!-- -->0 0x000055832ca94a1b backtrace (/workdir/llvm-project/build/bin/mlir-opt+0xf5f1a1b)
 #<!-- -->1 0x000055832cd19cfc llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /workdir/llvm-project/llvm/lib/Support/Unix/Signals.inc:834:13
 #<!-- -->2 0x000055832cd1417f llvm::sys::RunSignalHandlers() /workdir/llvm-project/llvm/lib/Support/Signals.cpp:105:18
 #<!-- -->3 0x000055832cd1c316 SignalHandler(int, siginfo_t*, void*) /workdir/llvm-project/llvm/lib/Support/Unix/Signals.inc:426:38
 #<!-- -->4 0x00007fcd1acad520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->5 0x00007fcd1ad019fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->6 0x00007fcd1acad476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->7 0x00007fcd1ac937f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->8 0x00007fcd1ac9371b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->9 0x00007fcd1aca4e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
#<!-- -->10 0x00005583371730b4 mlir::SymbolTable::SymbolTable(mlir::Operation*) /workdir/llvm-project/mlir/lib/IR/SymbolTable.cpp:136:5
#<!-- -->11 0x000055833717f709 std::__uniq_ptr_impl&lt;mlir::SymbolTable, std::default_delete&lt;mlir::SymbolTable&gt; &gt;::__uniq_ptr_impl(mlir::SymbolTable*) /usr/bin/../lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/bits/unique_ptr.h:155:56
#<!-- -->12 0x000055833717f709 std::__uniq_ptr_data&lt;mlir::SymbolTable, std::default_delete&lt;mlir::SymbolTable&gt;, true, true&gt;::__uniq_ptr_data(mlir::SymbolTable*) /usr/bin/../lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/bits/unique_ptr.h:210:40
#<!-- -->13 0x000055833717f709 std::unique_ptr&lt;mlir::SymbolTable, std::default_delete&lt;mlir::SymbolTable&gt; &gt;::unique_ptr&lt;std::default_delete&lt;mlir::SymbolTable&gt;, void&gt;(mlir::SymbolTable*) /usr/bin/../lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/bits/unique_ptr.h:282:4
#<!-- -->14 0x000055833717f709 std::_MakeUniq&lt;mlir::SymbolTable&gt;::__single_object std::make_unique&lt;mlir::SymbolTable, mlir::Operation*&amp;&gt;(mlir::Operation*&amp;) /usr/bin/../lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/bits/unique_ptr.h:962:14
#<!-- -->15 0x000055833717f709 mlir::SymbolTableCollection::getSymbolTable(mlir::Operation*) /workdir/llvm-project/mlir/lib/IR/SymbolTable.cpp:997:24
#<!-- -->16 0x000055833717f04e mlir::SymbolTableCollection::lookupSymbolIn(mlir::Operation*, mlir::StringAttr) /workdir/llvm-project/mlir/lib/IR/SymbolTable.cpp:0:10
#<!-- -->17 0x0000558337178494 lookupSymbolInImpl(mlir::Operation*, mlir::SymbolRefAttr, llvm::SmallVectorImpl&lt;mlir::Operation*&gt;&amp;, llvm::function_ref&lt;mlir::Operation* (mlir::Operation*, mlir::StringAttr)&gt;) /workdir/llvm-project/mlir/lib/IR/SymbolTable.cpp:418:8
#<!-- -->18 0x000055833717f42a mlir::SymbolTableCollection::lookupSymbolIn(mlir::Operation*, mlir::SymbolRefAttr, llvm::SmallVectorImpl&lt;mlir::Operation*&gt;&amp;) /workdir/llvm-project/mlir/lib/IR/SymbolTable.cpp:975:1
#<!-- -->19 0x000055833717f255 llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#<!-- -->20 0x000055833717f255 llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#<!-- -->21 0x000055833717f255 mlir::SymbolTableCollection::lookupSymbolIn(mlir::Operation*, mlir::SymbolRefAttr) /workdir/llvm-project/mlir/lib/IR/SymbolTable.cpp:960:7
#<!-- -->22 0x000055832e85e002 checkSymbolIsTypeOrAttribute(mlir::SymbolTableCollection&amp;, mlir::Operation*, mlir::SymbolRefAttr) /workdir/llvm-project/mlir/lib/Dialect/IRDL/IR/IRDL.cpp:261:8
#<!-- -->23 0x000055832e85e54d mlir::irdl::ParametricOp::verifySymbolUses(mlir::SymbolTableCollection&amp;) /workdir/llvm-project/mlir/lib/Dialect/IRDL/IR/IRDL.cpp:288:1
#<!-- -->24 0x000055832e8fe576 mlir::detail::SymbolUserOpInterfaceInterfaceTraits::Model&lt;mlir::irdl::ParametricOp&gt;::verifySymbolUses(mlir::detail::SymbolUserOpInterfaceInterfaceTraits::Concept const*, mlir::Operation*, mlir::SymbolTableCollection&amp;) /workdir/llvm-project/build/tools/mlir/include/mlir/IR/SymbolInterfaces.h.inc:583:3
#<!-- -->25 0x0000558337186b6e llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#<!-- -->26 0x0000558337186b6e llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#<!-- -->27 0x0000558337186b6e mlir::WalkResult::WalkResult(llvm::LogicalResult) /workdir/llvm-project/mlir/include/mlir/Support/WalkResult.h:37:16
#<!-- -->28 0x0000558337186b6e mlir::detail::verifySymbolTable(mlir::Operation*)::$_3::operator()(mlir::Operation*) const /workdir/llvm-project/mlir/lib/IR/SymbolTable.cpp:515:14
#<!-- -->29 0x0000558337186b6e std::optional&lt;mlir::WalkResult&gt; llvm::function_ref&lt;std::optional&lt;mlir::WalkResult&gt; (mlir::Operation*)&gt;::callback_fn&lt;mlir::detail::verifySymbolTable(mlir::Operation*)::$_3&gt;(long, mlir::Operation*) /workdir/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->30 0x0000558337179fdb walkSymbolTable(llvm::MutableArrayRef&lt;mlir::Region&gt;, llvm::function_ref&lt;std::optional&lt;mlir::WalkResult&gt; (mlir::Operation*)&gt;) /workdir/llvm-project/mlir/lib/IR/SymbolTable.cpp:86:42
#<!-- -->31 0x00005583371794e8 mlir::detail::verifySymbolTable(mlir::Operation*) /workdir/llvm-project/mlir/lib/IR/SymbolTable.cpp:520:7
#<!-- -->32 0x000055832e8efa0e llvm::LogicalResult::succeeded() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:40:45
#<!-- -->33 0x000055832e8efa0e llvm::succeeded(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:67:61
#<!-- -->34 0x000055832e8efa0e llvm::LogicalResult mlir::op_definition_impl::verifyRegionTraits&lt;mlir::OpTrait::OneRegion&lt;mlir::irdl::DialectOp&gt;, mlir::OpTrait::ZeroResults&lt;mlir::irdl::DialectOp&gt;, mlir::OpTrait::ZeroSuccessors&lt;mlir::irdl::DialectOp&gt;, mlir::OpTrait::ZeroOperands&lt;mlir::irdl::DialectOp&gt;, mlir::OpTrait::NoTerminator&lt;mlir::irdl::DialectOp&gt;, mlir::OpTrait::OpInvariants&lt;mlir::irdl::DialectOp&gt;, mlir::BytecodeOpInterface::Trait&lt;mlir::irdl::DialectOp&gt;, mlir::OpTrait::IsIsolatedFromAbove&lt;mlir::irdl::DialectOp&gt;, mlir::SymbolOpInterface::Trait&lt;mlir::irdl::DialectOp&gt;, mlir::OpTrait::SymbolTable&lt;mlir::irdl::DialectOp&gt; &gt;(mlir::Operation*) /workdir/llvm-project/mlir/include/mlir/IR/OpDefinition.h:1661:19
#<!-- -->35 0x000055832e8efa0e mlir::Op&lt;mlir::irdl::DialectOp, mlir::OpTrait::OneRegion, mlir::OpTrait::ZeroResults, mlir::OpTrait::ZeroSuccessors, mlir::OpTrait::ZeroOperands, mlir::OpTrait::NoTerminator, mlir::OpTrait::OpInvariants, mlir::BytecodeOpInterface::Trait, mlir::OpTrait::IsIsolatedFromAbove, mlir::SymbolOpInterface::Trait, mlir::OpTrait::SymbolTable&gt;::verifyRegionInvariants(mlir::Operation*) /workdir/llvm-project/mlir/include/mlir/IR/OpDefinition.h:2063:16
#<!-- -->36 0x000055832e8ecaa1 mlir::RegisteredOperationName::Model&lt;mlir::irdl::DialectOp&gt;::verifyRegionInvariants(mlir::Operation*) /workdir/llvm-project/mlir/include/mlir/IR/OperationSupport.h:0:14
#<!-- -->37 0x00005583371a2f6e llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#<!-- -->38 0x00005583371a2f6e llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#<!-- -->39 0x00005583371a2f6e (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:236:25
#<!-- -->40 0x00005583371a2f6e auto (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;)::$_0::operator()&lt;mlir::Operation&gt;(mlir::Operation*) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:287:53
#<!-- -->41 0x00005583371a2f6e auto (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;)::$_1::operator()&lt;(anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;)::$_0&gt;((anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;)::$_0&amp;&amp;, llvm::PointerUnion&lt;mlir::Operation*, mlir::Block*&gt;) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:276:16
#<!-- -->42 0x00005583371a2f6e (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:287:15
#<!-- -->43 0x00005583371a2f6e (anonymous namespace)::OperationVerifier::verifyOpAndDominance(mlir::Operation&amp;) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:81:14
#<!-- -->44 0x00005583371a724d llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#<!-- -->45 0x00005583371a724d llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#<!-- -->46 0x00005583371a724d (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3::operator()(mlir::Operation*) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:225:9
#<!-- -->47 0x00005583371a724d auto void mlir::parallelForEach&lt;mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&gt;(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&amp;&amp;)::'lambda'(mlir::Operation**&amp;&amp;)::operator()&lt;mlir::Operation*&amp;&gt;(mlir::Operation**&amp;&amp;) const /workdir/llvm-project/mlir/include/mlir/IR/Threading.h:120:12
#<!-- -->48 0x00005583371a2941 llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#<!-- -->49 0x00005583371a2941 llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#<!-- -->50 0x00005583371a2941 llvm::LogicalResult mlir::failableParallelForEach&lt;mlir::Operation**, void mlir::parallelForEach&lt;mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&gt;(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&amp;&amp;)::'lambda'(mlir::Operation**&amp;&amp;)&gt;(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&amp;&amp;) /workdir/llvm-project/mlir/include/mlir/IR/Threading.h:46:11
#<!-- -->51 0x00005583371a2941 void mlir::parallelForEach&lt;mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&gt;(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&amp;&amp;) /workdir/llvm-project/mlir/include/mlir/IR/Threading.h:119:9
#<!-- -->52 0x00005583371a2941 void mlir::parallelForEach&lt;llvm::SmallVector&lt;mlir::Operation*, 6u&gt;&amp;, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&gt;(mlir::MLIRContext*, llvm::SmallVector&lt;mlir::Operation*, 6u&gt;&amp;, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&amp;&amp;) /workdir/llvm-project/mlir/include/mlir/IR/Threading.h:131:3
#<!-- -->53 0x00005583371a2941 (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:224:3
#<!-- -->54 0x00005583371a2941 auto (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;)::$_0::operator()&lt;mlir::Operation&gt;(mlir::Operation*) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:287:53
#<!-- -->55 0x00005583371a2941 auto (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;)::$_1::operator()&lt;(anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;)::$_0&gt;((anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;)::$_0&amp;&amp;, llvm::PointerUnion&lt;mlir::Operation*, mlir::Block*&gt;) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:276:16
#<!-- -->56 0x00005583371a2941 (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:287:15
#<!-- -->57 0x00005583371a2941 (anonymous namespace)::OperationVerifier::verifyOpAndDominance(mlir::Operation&amp;) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:81:14
#<!-- -->58 0x00005583371a724d llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#<!-- -->59 0x00005583371a724d llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#<!-- -->60 0x00005583371a724d (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3::operator()(mlir::Operation*) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:225:9
#<!-- -->61 0x00005583371a724d auto void mlir::parallelForEach&lt;mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&gt;(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&amp;&amp;)::'lambda'(mlir::Operation**&amp;&amp;)::operator()&lt;mlir::Operation*&amp;&gt;(mlir::Operation**&amp;&amp;) const /workdir/llvm-project/mlir/include/mlir/IR/Threading.h:120:12
#<!-- -->62 0x00005583371a2941 llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#<!-- -->63 0x00005583371a2941 llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#<!-- -->64 0x00005583371a2941 llvm::LogicalResult mlir::failableParallelForEach&lt;mlir::Operation**, void mlir::parallelForEach&lt;mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&gt;(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&amp;&amp;)::'lambda'(mlir::Operation**&amp;&amp;)&gt;(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&amp;&amp;) /workdir/llvm-project/mlir/include/mlir/IR/Threading.h:46:11
#<!-- -->65 0x00005583371a2941 void mlir::parallelForEach&lt;mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&gt;(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&amp;&amp;) /workdir/llvm-project/mlir/include/mlir/IR/Threading.h:119:9
#<!-- -->66 0x00005583371a2941 void mlir::parallelForEach&lt;llvm::SmallVector&lt;mlir::Operation*, 6u&gt;&amp;, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&gt;(mlir::MLIRContext*, llvm::SmallVector&lt;mlir::Operation*, 6u&gt;&amp;, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&amp;&amp;) /workdir/llvm-project/mlir/include/mlir/IR/Threading.h:131:3
#<!-- -->67 0x00005583371a2941 (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:224:3
#<!-- -->68 0x00005583371a2941 auto (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;)::$_0::operator()&lt;mlir::Operation&gt;(mlir::Operation*) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:287:53
#<!-- -->69 0x00005583371a2941 auto (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;)::$_1::operator()&lt;(anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;)::$_0&gt;((anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;)::$_0&amp;&amp;, llvm::PointerUnion&lt;mlir::Operation*, mlir::Block*&gt;) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:276:16
#<!-- -->70 0x00005583371a2941 (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:287:15
#<!-- -->71 0x00005583371a2941 (anonymous namespace)::OperationVerifier::verifyOpAndDominance(mlir::Operation&amp;) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:81:14
#<!-- -->72 0x00005583371a09f3 mlir::verify(mlir::Operation*, bool) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:426:1
#<!-- -->73 0x000055833e892d69 llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#<!-- -->74 0x000055833e892d69 llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#<!-- -->75 0x000055833e892d69 (anonymous namespace)::OperationParser::finalize() /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:931:48
#<!-- -->76 0x000055833e892d69 (anonymous namespace)::TopLevelOperationParser::parse(mlir::Block*, mlir::Location) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:2840:20
#<!-- -->77 0x000055833e892d69 mlir::parseAsmSourceFile(llvm::SourceMgr const&amp;, mlir::Block*, mlir::ParserConfig const&amp;, mlir::AsmParserState*, mlir::AsmParserCodeCompleteContext*) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:2894:41
#<!-- -->78 0x0000558336bfb1de mlir::parseSourceFile(std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::Block*, mlir::ParserConfig const&amp;, mlir::LocationAttr*) /workdir/llvm-project/mlir/lib/Parser/Parser.cpp:47:1
#<!-- -->79 0x0000558336bf8425 llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#<!-- -->80 0x0000558336bf8425 llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#<!-- -->81 0x0000558336bf8425 mlir::OwningOpRef&lt;mlir::ModuleOp&gt; mlir::detail::parseSourceFile&lt;mlir::ModuleOp, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;&gt;(mlir::ParserConfig const&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;) /workdir/llvm-project/mlir/include/mlir/Parser/Parser.h:159:7
#<!-- -->82 0x0000558336bf7cd8 std::enable_if&lt;__and_&lt;std::__not_&lt;std::__is_tuple_like&lt;mlir::ModuleOp&gt; &gt;, std::is_move_constructible&lt;mlir::ModuleOp&gt;, std::is_move_assignable&lt;mlir::ModuleOp&gt; &gt;::value, void&gt;::type std::swap&lt;mlir::ModuleOp&gt;(mlir::ModuleOp&amp;, mlir::ModuleOp&amp;) /usr/bin/../lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/bits/move.h:205:11
#<!-- -->83 0x0000558336bf7cd8 mlir::OwningOpRef&lt;mlir::ModuleOp&gt;::release() /workdir/llvm-project/mlir/include/mlir/IR/OwningOpRef.h:69:5
#<!-- -->84 0x0000558336bf7cd8 mlir::OwningOpRef&lt;mlir::ModuleOp&gt;::operator mlir::OwningOpRef&lt;mlir::Operation*&gt;() &amp;&amp; /workdir/llvm-project/mlir/include/mlir/IR/OwningOpRef.h:64:51
#<!-- -->85 0x0000558336bf7cd8 mlir::parseSourceFileForTool(std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::ParserConfig const&amp;, bool) /workdir/llvm-project/mlir/include/mlir/Tools/ParseUtilities.h:31:12
#<!-- -->86 0x0000558336bf66ff performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:0:33
#<!-- -->87 0x0000558336bf5fe4 processBuffer(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, mlir::MlirOptMainConfig const&amp;, mlir::DialectRegistry&amp;, llvm::ThreadPoolInterface*) /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:551:12
#<!-- -->88 0x0000558336bf5fe4 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_3::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;) const /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:635:12
#<!-- -->89 0x0000558336bf5fe4 llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_3&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;) /workdir/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->90 0x0000558336e2eeac std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;::~unique_ptr() /usr/bin/../lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/bits/unique_ptr.h:360:6
#<!-- -->91 0x0000558336e2eeac llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;)&gt;::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;) const /workdir/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->92 0x0000558336e2f4f7 llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#<!-- -->93 0x0000558336e2f4f7 llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#<!-- -->94 0x0000558336e2f4f7 mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef)::$_0::operator()(llvm::StringRef) const /workdir/llvm-project/mlir/lib/Support/ToolUtilities.cpp:86:9
#<!-- -->95 0x0000558336e2e7cb void llvm::interleave&lt;llvm::StringRef const*, mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef)::$_0, void llvm::interleave&lt;llvm::SmallVector&lt;llvm::StringRef, 8u&gt;, mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef)::$_0, llvm::raw_ostream, llvm::StringRef&gt;(llvm::SmallVector&lt;llvm::StringRef, 8u&gt; const&amp;, llvm::raw_ostream&amp;, mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef)::$_0, llvm::StringRef const&amp;)::'lambda'(), void&gt;(llvm::SmallVector&lt;llvm::StringRef, 8u&gt;, llvm::SmallVector&lt;llvm::StringRef, 8u&gt;, mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef)::$_0, llvm::raw_ostream) /workdir/llvm-project/llvm/include/llvm/ADT/STLExtras.h:0:3
#<!-- -->96 0x0000558336e2e7cb void llvm::interleave&lt;llvm::SmallVector&lt;llvm::StringRef, 8u&gt;, mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef)::$_0, llvm::raw_ostream, llvm::StringRef&gt;(llvm::SmallVector&lt;llvm::StringRef, 8u&gt; const&amp;, llvm::raw_ostream&amp;, mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef)::$_0, llvm::StringRef const&amp;) /workdir/llvm-project/llvm/include/llvm/ADT/STLExtras.h:2238:3
#<!-- -->97 0x0000558336e2e7cb mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) /workdir/llvm-project/mlir/lib/Support/ToolUtilities.cpp:89:3
#<!-- -->98 0x0000558336be4b1d mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:638:10
#<!-- -->99 0x0000558336be5405 llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#<!-- -->100 0x0000558336be5405 llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#<!-- -->101 0x0000558336be5405 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:679:7
#<!-- -->102 0x0000558336be5b53 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:695:10
#<!-- -->103 0x000055832cb19987 llvm::LogicalResult::succeeded() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:40:45
#<!-- -->104 0x000055832cb19987 mlir::asMainReturnCode(llvm::LogicalResult) /workdir/llvm-project/mlir/include/mlir/Tools/mlir-opt/MlirOptMain.h:355:12
#<!-- -->105 0x000055832cb19987 main /workdir/llvm-project/mlir/tools/mlir-opt/mlir-opt.cpp:339:10
#<!-- -->106 0x00007fcd1ac94d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->107 0x00007fcd1ac94e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->108 0x000055832ca58ee5 _start (/workdir/llvm-project/build/bin/mlir-opt+0xf5b5ee5)
Aborted (core dumped)
```

</details>


---

