# [MLIR] Transform dialect crashes when func.func transform is applied to non-function operations

**Author:** sairam2661
**URL:** https://github.com/llvm/llvm-project/issues/159646
**Status:** Closed
**Labels:** crash, mlir:transform_dialect
**Closed Date:** 2025-10-06T14:58:02Z

## Body

Test commit: [5a86dc996c26299de63effc927075dcbfb924167](https://github.com/llvm/llvm-project/pull/157520/commits/5a86dc996c26299de63effc927075dcbfb924167)

**Description**:
The Transform dialect crashes with assertion failure `index < numRegions && "invalid region index"` when the `func.func` transform is applied to non-function operations. From the trace, it looks like its happening during the verification when it builds a call graph (the code tries to access callable regions on operations that don't have the expected function structure).

**Steps to reproduce**
- Minimal MLIR program (test.mlir)
```
"builtin.module"() ({
  %0 = "test.match_anchor_1"() : () -> i32
  
  "transform.named_sequence"()<{
    function_type = (!transform.any_op) -> (), 
    sym_name = "__transform_main"
  }> ({
  ^bb0(%arg0: !transform.any_op):
    %1 = "transform.structured.match"(%arg0)<{ops = ["test.match_anchor_1"]}> : (!transform.any_op) -> !transform.any_op
    %2 = "func.func"(%1)<{number = 0 : i64}> : (!transform.any_op) -> !transform.any_value
    "transform.test_consume_operand"(%2) : (!transform.any_value) -> ()
    "transform.yield"() : () -> ()
  }) : () -> ()
}) { transform.with_named_sequence } : () -> ()
```

**Command**
```
mlir-opt test.mlir
```

**Output**
```
mlir-opt: /workdir/llvm-project/mlir/include/mlir/IR/Operation.h:687: mlir::Region &mlir::Operation::getRegion(unsigned int): Assertion `index < numRegions && "invalid region index"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.      Program arguments: /workdir/llvm-project/build/bin/mlir-opt test.mlir
 #0 0x000055e879e0ba1b backtrace (/workdir/llvm-project/build/bin/mlir-opt+0xf5f1a1b)
 #1 0x000055e87a090cfc llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /workdir/llvm-project/llvm/lib/Support/Unix/Signals.inc:834:13
 #2 0x000055e87a08b17f llvm::sys::RunSignalHandlers() /workdir/llvm-project/llvm/lib/Support/Signals.cpp:105:18
 #3 0x000055e87a093316 SignalHandler(int, siginfo_t*, void*) /workdir/llvm-project/llvm/lib/Support/Unix/Signals.inc:426:38
 #4 0x00007f64a9a13520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #5 0x00007f64a9a679fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #6 0x00007f64a9a13476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #7 0x00007f64a99f97f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #8 0x00007f64a99f971b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #9 0x00007f64a9a0ae96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
#10 0x000055e87a11c22c mlir::Operation::getRegion(unsigned int) /workdir/llvm-project/mlir/include/mlir/IR/Operation.h:687:5
#11 0x000055e87b0191f5 llvm::detail::PunnedPointer<llvm::ilist_node_base<true, void>*>::asInt() const /workdir/llvm-project/llvm/include/llvm/ADT/PointerIntPair.h:41:5
#12 0x000055e87b0191f5 llvm::detail::PunnedPointer<llvm::ilist_node_base<true, void>*>::operator long() const /workdir/llvm-project/llvm/include/llvm/ADT/PointerIntPair.h:45:48
#13 0x000055e87b0191f5 llvm::PointerIntPair<llvm::ilist_node_base<true, void>*, 1u, unsigned int, llvm::PointerLikeTypeTraits<llvm::ilist_node_base<true, void>*>, llvm::PointerIntPairInfo<llvm::ilist_node_base<true, void>*, 1u, llvm::PointerLikeTypeTraits<llvm::ilist_node_base<true, void>*> > >::getPointer() const /workdir/llvm-project/llvm/include/llvm/ADT/PointerIntPair.h:94:58
#14 0x000055e87b0191f5 llvm::ilist_detail::node_base_prevnext<llvm::ilist_node_base<true, void>, true>::getPrev() const /workdir/llvm-project/llvm/include/llvm/ADT/ilist_node_base.h:41:54
#15 0x000055e87b0191f5 llvm::ilist_node_impl<llvm::ilist_detail::node_options<mlir::Block, true, false, void, false, void> >::getPrev() const /workdir/llvm-project/llvm/include/llvm/ADT/ilist_node.h:123:59
#16 0x000055e87b0191f5 llvm::ilist_sentinel<llvm::ilist_detail::node_options<mlir::Block, true, false, void, false, void> >::empty() const /workdir/llvm-project/llvm/include/llvm/ADT/ilist_node.h:319:45
#17 0x000055e87b0191f5 llvm::simple_ilist<mlir::Block>::empty() const /workdir/llvm-project/llvm/include/llvm/ADT/simple_ilist.h:139:54
#18 0x000055e87b0191f5 mlir::Region::empty() /workdir/llvm-project/mlir/include/mlir/IR/Region.h:60:32
#19 0x000055e87b0191f5 mlir::detail::FunctionOpInterfaceTrait<mlir::func::FuncOp>::empty() /workdir/llvm-project/build/tools/mlir/include/mlir/Interfaces/FunctionInterfaces.h.inc:806:45
#20 0x000055e87b5cd06e mlir::func::FuncOp::getCallableRegion() /workdir/llvm-project/build/tools/mlir/include/mlir/Dialect/Func/IR/FuncOps.h.inc:1443:48
#21 0x000055e87b5cc392 mlir::detail::CallableOpInterfaceInterfaceTraits::Model<mlir::func::FuncOp>::getCallableRegion(mlir::detail::CallableOpInterfaceInterfaceTraits::Concept const*, mlir::Operation*) /workdir/llvm-project/build/tools/mlir/include/mlir/Interfaces/CallInterfaces.h.inc:393:3
#22 0x000055e883ed0cee computeCallGraph(mlir::Operation*, mlir::CallGraph&, mlir::SymbolTableCollection&, mlir::CallGraphNode*, bool) /workdir/llvm-project/mlir/lib/Analysis/CallGraph.cpp:88:15
#23 0x000055e883ed0f11 computeCallGraph(mlir::Operation*, mlir::CallGraph&, mlir::SymbolTableCollection&, mlir::CallGraphNode*, bool) /workdir/llvm-project/mlir/lib/Analysis/CallGraph.cpp:0:7
#24 0x000055e883ed0f11 computeCallGraph(mlir::Operation*, mlir::CallGraph&, mlir::SymbolTableCollection&, mlir::CallGraphNode*, bool) /workdir/llvm-project/mlir/lib/Analysis/CallGraph.cpp:0:7
#25 0x000055e883ed075b mlir::CallGraph::CallGraph(mlir::Operation*) /workdir/llvm-project/mlir/lib/Analysis/CallGraph.cpp:108:3
#26 0x000055e88280b7b0 mlir::transform::TransformDialect::verifyOperationAttribute(mlir::Operation*, mlir::NamedAttribute) /workdir/llvm-project/mlir/lib/Dialect/Transform/IR/TransformDialect.cpp:144:10
#27 0x000055e884518d28 llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#28 0x000055e884518d28 llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#29 0x000055e884518d28 (anonymous namespace)::OperationVerifier::verifyOnEntrance(mlir::Operation&) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:167:11
#30 0x000055e884518d28 auto (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&)::$_2::operator()<mlir::Operation>(mlir::Operation*) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:296:45
#31 0x000055e884518d28 auto (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&)::$_1::operator()<(anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&)::$_2>((anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&)::$_2&&, llvm::PointerUnion<mlir::Operation*, mlir::Block*>) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:276:16
#32 0x000055e884518d28 (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:295:16
#33 0x000055e884518d28 (anonymous namespace)::OperationVerifier::verifyOpAndDominance(mlir::Operation&) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:81:14
#34 0x000055e88451e24d llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#35 0x000055e88451e24d llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#36 0x000055e88451e24d (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3::operator()(mlir::Operation*) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:225:9
#37 0x000055e88451e24d auto void mlir::parallelForEach<mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3>(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3&&)::'lambda'(mlir::Operation**&&)::operator()<mlir::Operation*&>(mlir::Operation**&&) const /workdir/llvm-project/mlir/include/mlir/IR/Threading.h:120:12
#38 0x000055e884519941 llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#39 0x000055e884519941 llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#40 0x000055e884519941 llvm::LogicalResult mlir::failableParallelForEach<mlir::Operation**, void mlir::parallelForEach<mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3>(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3&&)::'lambda'(mlir::Operation**&&)>(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3&&) /workdir/llvm-project/mlir/include/mlir/IR/Threading.h:46:11
#41 0x000055e884519941 void mlir::parallelForEach<mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3>(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3&&) /workdir/llvm-project/mlir/include/mlir/IR/Threading.h:119:9
#42 0x000055e884519941 void mlir::parallelForEach<llvm::SmallVector<mlir::Operation*, 6u>&, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3>(mlir::MLIRContext*, llvm::SmallVector<mlir::Operation*, 6u>&, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3&&) /workdir/llvm-project/mlir/include/mlir/IR/Threading.h:131:3
#43 0x000055e884519941 (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:224:3
#44 0x000055e884519941 auto (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&)::$_0::operator()<mlir::Operation>(mlir::Operation*) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:287:53
#45 0x000055e884519941 auto (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&)::$_1::operator()<(anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&)::$_0>((anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&)::$_0&&, llvm::PointerUnion<mlir::Operation*, mlir::Block*>) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:276:16
#46 0x000055e884519941 (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:287:15
#47 0x000055e884519941 (anonymous namespace)::OperationVerifier::verifyOpAndDominance(mlir::Operation&) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:81:14
#48 0x000055e8845179f3 mlir::verify(mlir::Operation*, bool) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:426:1
#49 0x000055e88bc09d69 llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#50 0x000055e88bc09d69 llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#51 0x000055e88bc09d69 (anonymous namespace)::OperationParser::finalize() /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:931:48
#52 0x000055e88bc09d69 (anonymous namespace)::TopLevelOperationParser::parse(mlir::Block*, mlir::Location) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:2840:20
#53 0x000055e88bc09d69 mlir::parseAsmSourceFile(llvm::SourceMgr const&, mlir::Block*, mlir::ParserConfig const&, mlir::AsmParserState*, mlir::AsmParserCodeCompleteContext*) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:2894:41
#54 0x000055e883f721de mlir::parseSourceFile(std::shared_ptr<llvm::SourceMgr> const&, mlir::Block*, mlir::ParserConfig const&, mlir::LocationAttr*) /workdir/llvm-project/mlir/lib/Parser/Parser.cpp:47:1
#55 0x000055e883f6f425 llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#56 0x000055e883f6f425 llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#57 0x000055e883f6f425 mlir::OwningOpRef<mlir::ModuleOp> mlir::detail::parseSourceFile<mlir::ModuleOp, std::shared_ptr<llvm::SourceMgr> const&>(mlir::ParserConfig const&, std::shared_ptr<llvm::SourceMgr> const&) /workdir/llvm-project/mlir/include/mlir/Parser/Parser.h:159:7
#58 0x000055e883f6ecd8 std::enable_if<__and_<std::__not_<std::__is_tuple_like<mlir::ModuleOp> >, std::is_move_constructible<mlir::ModuleOp>, std::is_move_assignable<mlir::ModuleOp> >::value, void>::type std::swap<mlir::ModuleOp>(mlir::ModuleOp&, mlir::ModuleOp&) /usr/bin/../lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/bits/move.h:205:11
#59 0x000055e883f6ecd8 mlir::OwningOpRef<mlir::ModuleOp>::release() /workdir/llvm-project/mlir/include/mlir/IR/OwningOpRef.h:69:5
#60 0x000055e883f6ecd8 mlir::OwningOpRef<mlir::ModuleOp>::operator mlir::OwningOpRef<mlir::Operation*>() && /workdir/llvm-project/mlir/include/mlir/IR/OwningOpRef.h:64:51
#61 0x000055e883f6ecd8 mlir::parseSourceFileForTool(std::shared_ptr<llvm::SourceMgr> const&, mlir::ParserConfig const&, bool) /workdir/llvm-project/mlir/include/mlir/Tools/ParseUtilities.h:31:12
#62 0x000055e883f6d6ff performActions(llvm::raw_ostream&, std::shared_ptr<llvm::SourceMgr> const&, mlir::MLIRContext*, mlir::MlirOptMainConfig const&) /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:0:33
#63 0x000055e883f6cfe4 processBuffer(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, mlir::MlirOptMainConfig const&, mlir::DialectRegistry&, llvm::ThreadPoolInterface*) /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:551:12
#64 0x000055e883f6cfe4 mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_3::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::raw_ostream&) const /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:635:12
#65 0x000055e883f6cfe4 llvm::LogicalResult llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::raw_ostream&)>::callback_fn<mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_3>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::raw_ostream&) /workdir/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#66 0x000055e8841a5eac std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >::~unique_ptr() /usr/bin/../lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/bits/unique_ptr.h:360:6
#67 0x000055e8841a5eac llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::raw_ostream&)>::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::raw_ostream&) const /workdir/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#68 0x000055e8841a54c3 std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >::~unique_ptr() /usr/bin/../lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/bits/unique_ptr.h:360:6
#69 0x000055e8841a54c3 mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef) /workdir/llvm-project/mlir/lib/Support/ToolUtilities.cpp:27:5
#70 0x000055e883f5bb1d mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&) /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:638:10
#71 0x000055e883f5c405 llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#72 0x000055e883f5c405 llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#73 0x000055e883f5c405 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&) /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:679:7
#74 0x000055e883f5cb53 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&) /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:695:10
#75 0x000055e879e90987 llvm::LogicalResult::succeeded() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:40:45
#76 0x000055e879e90987 mlir::asMainReturnCode(llvm::LogicalResult) /workdir/llvm-project/mlir/include/mlir/Tools/mlir-opt/MlirOptMain.h:355:12
#77 0x000055e879e90987 main /workdir/llvm-project/mlir/tools/mlir-opt/mlir-opt.cpp:339:10
#78 0x00007f64a99fad90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#79 0x00007f64a99fae40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#80 0x000055e879dcfee5 _start (/workdir/llvm-project/build/bin/mlir-opt+0xf5b5ee5)
Aborted (core dumped)
```


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: None (sairam2661)

<details>
Test commit: [5a86dc996c26299de63effc927075dcbfb924167](https://github.com/llvm/llvm-project/pull/157520/commits/5a86dc996c26299de63effc927075dcbfb924167)

**Description**:
The Transform dialect crashes with assertion failure `index &lt; numRegions &amp;&amp; "invalid region index"` when the `func.func` transform is applied to non-function operations. From the trace, it looks like its happening during the verification when it builds a call graph (the code tries to access callable regions on operations that don't have the expected function structure).

**Steps to reproduce**
- Minimal MLIR program (test.mlir)
```
"builtin.module"() ({
  %0 = "test.match_anchor_1"() : () -&gt; i32
  
  "transform.named_sequence"()&lt;{
    function_type = (!transform.any_op) -&gt; (), 
    sym_name = "__transform_main"
  }&gt; ({
  ^bb0(%arg0: !transform.any_op):
    %1 = "transform.structured.match"(%arg0)&lt;{ops = ["test.match_anchor_1"]}&gt; : (!transform.any_op) -&gt; !transform.any_op
    %2 = "func.func"(%1)&lt;{number = 0 : i64}&gt; : (!transform.any_op) -&gt; !transform.any_value
    "transform.test_consume_operand"(%2) : (!transform.any_value) -&gt; ()
    "transform.yield"() : () -&gt; ()
  }) : () -&gt; ()
}) { transform.with_named_sequence } : () -&gt; ()
```

**Command**
```
mlir-opt test.mlir
```

**Output**
```
mlir-opt: /workdir/llvm-project/mlir/include/mlir/IR/Operation.h:687: mlir::Region &amp;mlir::Operation::getRegion(unsigned int): Assertion `index &lt; numRegions &amp;&amp; "invalid region index"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.      Program arguments: /workdir/llvm-project/build/bin/mlir-opt test.mlir
 #<!-- -->0 0x000055e879e0ba1b backtrace (/workdir/llvm-project/build/bin/mlir-opt+0xf5f1a1b)
 #<!-- -->1 0x000055e87a090cfc llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /workdir/llvm-project/llvm/lib/Support/Unix/Signals.inc:834:13
 #<!-- -->2 0x000055e87a08b17f llvm::sys::RunSignalHandlers() /workdir/llvm-project/llvm/lib/Support/Signals.cpp:105:18
 #<!-- -->3 0x000055e87a093316 SignalHandler(int, siginfo_t*, void*) /workdir/llvm-project/llvm/lib/Support/Unix/Signals.inc:426:38
 #<!-- -->4 0x00007f64a9a13520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->5 0x00007f64a9a679fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->6 0x00007f64a9a13476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->7 0x00007f64a99f97f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->8 0x00007f64a99f971b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->9 0x00007f64a9a0ae96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
#<!-- -->10 0x000055e87a11c22c mlir::Operation::getRegion(unsigned int) /workdir/llvm-project/mlir/include/mlir/IR/Operation.h:687:5
#<!-- -->11 0x000055e87b0191f5 llvm::detail::PunnedPointer&lt;llvm::ilist_node_base&lt;true, void&gt;*&gt;::asInt() const /workdir/llvm-project/llvm/include/llvm/ADT/PointerIntPair.h:41:5
#<!-- -->12 0x000055e87b0191f5 llvm::detail::PunnedPointer&lt;llvm::ilist_node_base&lt;true, void&gt;*&gt;::operator long() const /workdir/llvm-project/llvm/include/llvm/ADT/PointerIntPair.h:45:48
#<!-- -->13 0x000055e87b0191f5 llvm::PointerIntPair&lt;llvm::ilist_node_base&lt;true, void&gt;*, 1u, unsigned int, llvm::PointerLikeTypeTraits&lt;llvm::ilist_node_base&lt;true, void&gt;*&gt;, llvm::PointerIntPairInfo&lt;llvm::ilist_node_base&lt;true, void&gt;*, 1u, llvm::PointerLikeTypeTraits&lt;llvm::ilist_node_base&lt;true, void&gt;*&gt; &gt; &gt;::getPointer() const /workdir/llvm-project/llvm/include/llvm/ADT/PointerIntPair.h:94:58
#<!-- -->14 0x000055e87b0191f5 llvm::ilist_detail::node_base_prevnext&lt;llvm::ilist_node_base&lt;true, void&gt;, true&gt;::getPrev() const /workdir/llvm-project/llvm/include/llvm/ADT/ilist_node_base.h:41:54
#<!-- -->15 0x000055e87b0191f5 llvm::ilist_node_impl&lt;llvm::ilist_detail::node_options&lt;mlir::Block, true, false, void, false, void&gt; &gt;::getPrev() const /workdir/llvm-project/llvm/include/llvm/ADT/ilist_node.h:123:59
#<!-- -->16 0x000055e87b0191f5 llvm::ilist_sentinel&lt;llvm::ilist_detail::node_options&lt;mlir::Block, true, false, void, false, void&gt; &gt;::empty() const /workdir/llvm-project/llvm/include/llvm/ADT/ilist_node.h:319:45
#<!-- -->17 0x000055e87b0191f5 llvm::simple_ilist&lt;mlir::Block&gt;::empty() const /workdir/llvm-project/llvm/include/llvm/ADT/simple_ilist.h:139:54
#<!-- -->18 0x000055e87b0191f5 mlir::Region::empty() /workdir/llvm-project/mlir/include/mlir/IR/Region.h:60:32
#<!-- -->19 0x000055e87b0191f5 mlir::detail::FunctionOpInterfaceTrait&lt;mlir::func::FuncOp&gt;::empty() /workdir/llvm-project/build/tools/mlir/include/mlir/Interfaces/FunctionInterfaces.h.inc:806:45
#<!-- -->20 0x000055e87b5cd06e mlir::func::FuncOp::getCallableRegion() /workdir/llvm-project/build/tools/mlir/include/mlir/Dialect/Func/IR/FuncOps.h.inc:1443:48
#<!-- -->21 0x000055e87b5cc392 mlir::detail::CallableOpInterfaceInterfaceTraits::Model&lt;mlir::func::FuncOp&gt;::getCallableRegion(mlir::detail::CallableOpInterfaceInterfaceTraits::Concept const*, mlir::Operation*) /workdir/llvm-project/build/tools/mlir/include/mlir/Interfaces/CallInterfaces.h.inc:393:3
#<!-- -->22 0x000055e883ed0cee computeCallGraph(mlir::Operation*, mlir::CallGraph&amp;, mlir::SymbolTableCollection&amp;, mlir::CallGraphNode*, bool) /workdir/llvm-project/mlir/lib/Analysis/CallGraph.cpp:88:15
#<!-- -->23 0x000055e883ed0f11 computeCallGraph(mlir::Operation*, mlir::CallGraph&amp;, mlir::SymbolTableCollection&amp;, mlir::CallGraphNode*, bool) /workdir/llvm-project/mlir/lib/Analysis/CallGraph.cpp:0:7
#<!-- -->24 0x000055e883ed0f11 computeCallGraph(mlir::Operation*, mlir::CallGraph&amp;, mlir::SymbolTableCollection&amp;, mlir::CallGraphNode*, bool) /workdir/llvm-project/mlir/lib/Analysis/CallGraph.cpp:0:7
#<!-- -->25 0x000055e883ed075b mlir::CallGraph::CallGraph(mlir::Operation*) /workdir/llvm-project/mlir/lib/Analysis/CallGraph.cpp:108:3
#<!-- -->26 0x000055e88280b7b0 mlir::transform::TransformDialect::verifyOperationAttribute(mlir::Operation*, mlir::NamedAttribute) /workdir/llvm-project/mlir/lib/Dialect/Transform/IR/TransformDialect.cpp:144:10
#<!-- -->27 0x000055e884518d28 llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#<!-- -->28 0x000055e884518d28 llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#<!-- -->29 0x000055e884518d28 (anonymous namespace)::OperationVerifier::verifyOnEntrance(mlir::Operation&amp;) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:167:11
#<!-- -->30 0x000055e884518d28 auto (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;)::$_2::operator()&lt;mlir::Operation&gt;(mlir::Operation*) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:296:45
#<!-- -->31 0x000055e884518d28 auto (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;)::$_1::operator()&lt;(anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;)::$_2&gt;((anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;)::$_2&amp;&amp;, llvm::PointerUnion&lt;mlir::Operation*, mlir::Block*&gt;) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:276:16
#<!-- -->32 0x000055e884518d28 (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:295:16
#<!-- -->33 0x000055e884518d28 (anonymous namespace)::OperationVerifier::verifyOpAndDominance(mlir::Operation&amp;) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:81:14
#<!-- -->34 0x000055e88451e24d llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#<!-- -->35 0x000055e88451e24d llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#<!-- -->36 0x000055e88451e24d (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3::operator()(mlir::Operation*) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:225:9
#<!-- -->37 0x000055e88451e24d auto void mlir::parallelForEach&lt;mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&gt;(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&amp;&amp;)::'lambda'(mlir::Operation**&amp;&amp;)::operator()&lt;mlir::Operation*&amp;&gt;(mlir::Operation**&amp;&amp;) const /workdir/llvm-project/mlir/include/mlir/IR/Threading.h:120:12
#<!-- -->38 0x000055e884519941 llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#<!-- -->39 0x000055e884519941 llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#<!-- -->40 0x000055e884519941 llvm::LogicalResult mlir::failableParallelForEach&lt;mlir::Operation**, void mlir::parallelForEach&lt;mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&gt;(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&amp;&amp;)::'lambda'(mlir::Operation**&amp;&amp;)&gt;(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&amp;&amp;) /workdir/llvm-project/mlir/include/mlir/IR/Threading.h:46:11
#<!-- -->41 0x000055e884519941 void mlir::parallelForEach&lt;mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&gt;(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&amp;&amp;) /workdir/llvm-project/mlir/include/mlir/IR/Threading.h:119:9
#<!-- -->42 0x000055e884519941 void mlir::parallelForEach&lt;llvm::SmallVector&lt;mlir::Operation*, 6u&gt;&amp;, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&gt;(mlir::MLIRContext*, llvm::SmallVector&lt;mlir::Operation*, 6u&gt;&amp;, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&amp;&amp;) /workdir/llvm-project/mlir/include/mlir/IR/Threading.h:131:3
#<!-- -->43 0x000055e884519941 (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:224:3
#<!-- -->44 0x000055e884519941 auto (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;)::$_0::operator()&lt;mlir::Operation&gt;(mlir::Operation*) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:287:53
#<!-- -->45 0x000055e884519941 auto (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;)::$_1::operator()&lt;(anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;)::$_0&gt;((anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;)::$_0&amp;&amp;, llvm::PointerUnion&lt;mlir::Operation*, mlir::Block*&gt;) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:276:16
#<!-- -->46 0x000055e884519941 (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:287:15
#<!-- -->47 0x000055e884519941 (anonymous namespace)::OperationVerifier::verifyOpAndDominance(mlir::Operation&amp;) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:81:14
#<!-- -->48 0x000055e8845179f3 mlir::verify(mlir::Operation*, bool) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:426:1
#<!-- -->49 0x000055e88bc09d69 llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#<!-- -->50 0x000055e88bc09d69 llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#<!-- -->51 0x000055e88bc09d69 (anonymous namespace)::OperationParser::finalize() /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:931:48
#<!-- -->52 0x000055e88bc09d69 (anonymous namespace)::TopLevelOperationParser::parse(mlir::Block*, mlir::Location) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:2840:20
#<!-- -->53 0x000055e88bc09d69 mlir::parseAsmSourceFile(llvm::SourceMgr const&amp;, mlir::Block*, mlir::ParserConfig const&amp;, mlir::AsmParserState*, mlir::AsmParserCodeCompleteContext*) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:2894:41
#<!-- -->54 0x000055e883f721de mlir::parseSourceFile(std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::Block*, mlir::ParserConfig const&amp;, mlir::LocationAttr*) /workdir/llvm-project/mlir/lib/Parser/Parser.cpp:47:1
#<!-- -->55 0x000055e883f6f425 llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#<!-- -->56 0x000055e883f6f425 llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#<!-- -->57 0x000055e883f6f425 mlir::OwningOpRef&lt;mlir::ModuleOp&gt; mlir::detail::parseSourceFile&lt;mlir::ModuleOp, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;&gt;(mlir::ParserConfig const&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;) /workdir/llvm-project/mlir/include/mlir/Parser/Parser.h:159:7
#<!-- -->58 0x000055e883f6ecd8 std::enable_if&lt;__and_&lt;std::__not_&lt;std::__is_tuple_like&lt;mlir::ModuleOp&gt; &gt;, std::is_move_constructible&lt;mlir::ModuleOp&gt;, std::is_move_assignable&lt;mlir::ModuleOp&gt; &gt;::value, void&gt;::type std::swap&lt;mlir::ModuleOp&gt;(mlir::ModuleOp&amp;, mlir::ModuleOp&amp;) /usr/bin/../lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/bits/move.h:205:11
#<!-- -->59 0x000055e883f6ecd8 mlir::OwningOpRef&lt;mlir::ModuleOp&gt;::release() /workdir/llvm-project/mlir/include/mlir/IR/OwningOpRef.h:69:5
#<!-- -->60 0x000055e883f6ecd8 mlir::OwningOpRef&lt;mlir::ModuleOp&gt;::operator mlir::OwningOpRef&lt;mlir::Operation*&gt;() &amp;&amp; /workdir/llvm-project/mlir/include/mlir/IR/OwningOpRef.h:64:51
#<!-- -->61 0x000055e883f6ecd8 mlir::parseSourceFileForTool(std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::ParserConfig const&amp;, bool) /workdir/llvm-project/mlir/include/mlir/Tools/ParseUtilities.h:31:12
#<!-- -->62 0x000055e883f6d6ff performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:0:33
#<!-- -->63 0x000055e883f6cfe4 processBuffer(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, mlir::MlirOptMainConfig const&amp;, mlir::DialectRegistry&amp;, llvm::ThreadPoolInterface*) /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:551:12
#<!-- -->64 0x000055e883f6cfe4 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_3::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;) const /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:635:12
#<!-- -->65 0x000055e883f6cfe4 llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_3&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;) /workdir/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->66 0x000055e8841a5eac std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;::~unique_ptr() /usr/bin/../lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/bits/unique_ptr.h:360:6
#<!-- -->67 0x000055e8841a5eac llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;)&gt;::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;) const /workdir/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->68 0x000055e8841a54c3 std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;::~unique_ptr() /usr/bin/../lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/bits/unique_ptr.h:360:6
#<!-- -->69 0x000055e8841a54c3 mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) /workdir/llvm-project/mlir/lib/Support/ToolUtilities.cpp:27:5
#<!-- -->70 0x000055e883f5bb1d mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:638:10
#<!-- -->71 0x000055e883f5c405 llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#<!-- -->72 0x000055e883f5c405 llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#<!-- -->73 0x000055e883f5c405 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:679:7
#<!-- -->74 0x000055e883f5cb53 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:695:10
#<!-- -->75 0x000055e879e90987 llvm::LogicalResult::succeeded() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:40:45
#<!-- -->76 0x000055e879e90987 mlir::asMainReturnCode(llvm::LogicalResult) /workdir/llvm-project/mlir/include/mlir/Tools/mlir-opt/MlirOptMain.h:355:12
#<!-- -->77 0x000055e879e90987 main /workdir/llvm-project/mlir/tools/mlir-opt/mlir-opt.cpp:339:10
#<!-- -->78 0x00007f64a99fad90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->79 0x00007f64a99fae40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->80 0x000055e879dcfee5 _start (/workdir/llvm-project/build/bin/mlir-opt+0xf5b5ee5)
Aborted (core dumped)
```

</details>


---

### Comment 2 - ftynse

`func.func` is not a transform, it's a function ;) But it has nothing to do in a transform sequence and that should be handled appropriately.

---

### Comment 3 - ftynse

This comes from `CallGraph` computation being confused by a malformed `func.func` operation. The verifier of `transform.with_named_sequence`, which constructs the `CallGraph`, seems to be running before the verifier of individual nested operations, but the construction of `CallGraph` likely assumes operations to be valid.

---

### Comment 4 - ftynse

Specifically, `CallGraph` construction calls `isExternal` on a function op that transitively, by means of `FunctionOpInterface` and `CallableInterface`, assumes that the function op has exactly one region as checked by the verifier of `FunctionOpInterface`. An sort of inefficient solution would be to manually trigger that verification in the verifier of `transform.with_named_sequence`. The question is whether there is another situation unrelated to this verifier where a `CallGraph` maybe constructed for yet-unverified nested IR.

---

