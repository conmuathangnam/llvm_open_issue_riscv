# [MLIR] CallGraph crashes on func.call with nested region in transform context

**Author:** sairam2661
**URL:** https://github.com/llvm/llvm-project/issues/159734
**Status:** Closed
**Labels:** crash, mlir:transform_dialect
**Closed Date:** 2025-10-06T14:58:02Z

## Body

Test commit: [5a86dc996c26299de63effc927075dcbfb924167](https://github.com/llvm/llvm-project/pull/157520/commits/5a86dc996c26299de63effc927075dcbfb924167)

**Description**:
The CallGraph construction crashes with an assertion failure while it verifies the `transform.with_named_sequence` attribute on the module containing a `func.call` operation with an invalid nested region. 

**Steps to reproduce**
- Minimal MLIR program (test.mlir)
```
"builtin.module"() ({
  "transform.named_sequence"() <{
    function_type = (!transform.any_op) -> (),
    sym_name = "__transform_main"
  }> ({
  ^bb0(%arg0: !transform.any_op):
    "func.call"() <{
      function_type = () -> (),
      sym_name = "lambda_function"
    }> ({
    ^bb0():
      "func.return"() : () -> ()
    }) : () -> ()
    "transform.yield"() : () -> ()
  }) : () -> ()
}) {transform.with_named_sequence} : () -> ()
```

**Command**
```
mlir-opt test.mlir
```

**Output**
```
mlir-opt: /workdir/llvm-project/llvm/include/llvm/Support/Casting.h:656: decltype(auto) llvm::dyn_cast(From &) [To = mlir::Value, From = mlir::CallInterfaceCallable]: Assertion `detail::isPresent(Val) && "dyn_cast on a non-existent value"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.      Program arguments: /workdir/llvm-project/build/bin/mlir-opt test.mlir
 #0 0x000055922514fa1b backtrace (/workdir/llvm-project/build/bin/mlir-opt+0xf5f1a1b)
 #1 0x00005592253d4cfc llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /workdir/llvm-project/llvm/lib/Support/Unix/Signals.inc:834:13
 #2 0x00005592253cf17f llvm::sys::RunSignalHandlers() /workdir/llvm-project/llvm/lib/Support/Signals.cpp:105:18
 #3 0x00005592253d7316 SignalHandler(int, siginfo_t*, void*) /workdir/llvm-project/llvm/lib/Support/Unix/Signals.inc:426:38
 #4 0x00007f24c724c520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #5 0x00007f24c72a09fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #6 0x00007f24c724c476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #7 0x00007f24c72327f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #8 0x00007f24c723271b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #9 0x00007f24c7243e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
#10 0x0000559237198923 decltype(auto) llvm::dyn_cast<mlir::Value, mlir::CallInterfaceCallable>(mlir::CallInterfaceCallable&) /workdir/llvm-project/llvm/include/llvm/Support/Casting.h:656:3
#11 0x0000559237198923 mlir::call_interface_impl::resolveCallable(mlir::CallOpInterface, mlir::SymbolTableCollection*) /workdir/llvm-project/mlir/lib/Interfaces/CallInterfaces.cpp:190:24
#12 0x0000559226908954 mlir::detail::CallOpInterfaceTrait<mlir::func::CallOp>::resolveCallableInTable(mlir::SymbolTableCollection*) /workdir/llvm-project/build/tools/mlir/include/mlir/Interfaces/CallInterfaces.h.inc:156:7
#13 0x0000559226908954 mlir::detail::CallOpInterfaceInterfaceTraits::Model<mlir::func::CallOp>::resolveCallableInTable(mlir::detail::CallOpInterfaceInterfaceTraits::Concept const*, mlir::Operation*, mlir::SymbolTableCollection*) /workdir/llvm-project/build/tools/mlir/include/mlir/Interfaces/CallInterfaces.h.inc:303:56
#14 0x000055922f215da8 mlir::CallGraph::resolveCallable(mlir::CallOpInterface, mlir::SymbolTableCollection&) const /workdir/llvm-project/mlir/lib/Analysis/CallGraph.cpp:150:12
#15 0x000055922f214c24 computeCallGraph(mlir::Operation*, mlir::CallGraph&, mlir::SymbolTableCollection&, mlir::CallGraphNode*, bool) /workdir/llvm-project/mlir/lib/Analysis/CallGraph.cpp:82:19
#16 0x000055922f214f11 computeCallGraph(mlir::Operation*, mlir::CallGraph&, mlir::SymbolTableCollection&, mlir::CallGraphNode*, bool) /workdir/llvm-project/mlir/lib/Analysis/CallGraph.cpp:0:7
#17 0x000055922f214f11 computeCallGraph(mlir::Operation*, mlir::CallGraph&, mlir::SymbolTableCollection&, mlir::CallGraphNode*, bool) /workdir/llvm-project/mlir/lib/Analysis/CallGraph.cpp:0:7
#18 0x000055922f214771 mlir::CallGraph::CallGraph(mlir::Operation*) /workdir/llvm-project/mlir/lib/Analysis/CallGraph.cpp:0:3
#19 0x000055922db4f7b0 mlir::transform::TransformDialect::verifyOperationAttribute(mlir::Operation*, mlir::NamedAttribute) /workdir/llvm-project/mlir/lib/Dialect/Transform/IR/TransformDialect.cpp:144:10
#20 0x000055922f85cd28 llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#21 0x000055922f85cd28 llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#22 0x000055922f85cd28 (anonymous namespace)::OperationVerifier::verifyOnEntrance(mlir::Operation&) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:167:11
#23 0x000055922f85cd28 auto (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&)::$_2::operator()<mlir::Operation>(mlir::Operation*) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:296:45
#24 0x000055922f85cd28 auto (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&)::$_1::operator()<(anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&)::$_2>((anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&)::$_2&&, llvm::PointerUnion<mlir::Operation*, mlir::Block*>) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:276:16
#25 0x000055922f85cd28 (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:295:16
#26 0x000055922f85cd28 (anonymous namespace)::OperationVerifier::verifyOpAndDominance(mlir::Operation&) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:81:14
#27 0x000055922f86224d llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#28 0x000055922f86224d llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#29 0x000055922f86224d (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3::operator()(mlir::Operation*) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:225:9
#30 0x000055922f86224d auto void mlir::parallelForEach<mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3>(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3&&)::'lambda'(mlir::Operation**&&)::operator()<mlir::Operation*&>(mlir::Operation**&&) const /workdir/llvm-project/mlir/include/mlir/IR/Threading.h:120:12
#31 0x000055922f85d941 llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#32 0x000055922f85d941 llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#33 0x000055922f85d941 llvm::LogicalResult mlir::failableParallelForEach<mlir::Operation**, void mlir::parallelForEach<mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3>(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3&&)::'lambda'(mlir::Operation**&&)>(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3&&) /workdir/llvm-project/mlir/include/mlir/IR/Threading.h:46:11
#34 0x000055922f85d941 void mlir::parallelForEach<mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3>(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3&&) /workdir/llvm-project/mlir/include/mlir/IR/Threading.h:119:9
#35 0x000055922f85d941 void mlir::parallelForEach<llvm::SmallVector<mlir::Operation*, 6u>&, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3>(mlir::MLIRContext*, llvm::SmallVector<mlir::Operation*, 6u>&, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3&&) /workdir/llvm-project/mlir/include/mlir/IR/Threading.h:131:3
#36 0x000055922f85d941 (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:224:3
#37 0x000055922f85d941 auto (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&)::$_0::operator()<mlir::Operation>(mlir::Operation*) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:287:53
#38 0x000055922f85d941 auto (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&)::$_1::operator()<(anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&)::$_0>((anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&)::$_0&&, llvm::PointerUnion<mlir::Operation*, mlir::Block*>) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:276:16
#39 0x000055922f85d941 (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:287:15
#40 0x000055922f85d941 (anonymous namespace)::OperationVerifier::verifyOpAndDominance(mlir::Operation&) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:81:14
#41 0x000055922f85b9f3 mlir::verify(mlir::Operation*, bool) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:426:1
#42 0x0000559236f4dd69 llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#43 0x0000559236f4dd69 llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#44 0x0000559236f4dd69 (anonymous namespace)::OperationParser::finalize() /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:931:48
#45 0x0000559236f4dd69 (anonymous namespace)::TopLevelOperationParser::parse(mlir::Block*, mlir::Location) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:2840:20
#46 0x0000559236f4dd69 mlir::parseAsmSourceFile(llvm::SourceMgr const&, mlir::Block*, mlir::ParserConfig const&, mlir::AsmParserState*, mlir::AsmParserCodeCompleteContext*) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:2894:41
#47 0x000055922f2b61de mlir::parseSourceFile(std::shared_ptr<llvm::SourceMgr> const&, mlir::Block*, mlir::ParserConfig const&, mlir::LocationAttr*) /workdir/llvm-project/mlir/lib/Parser/Parser.cpp:47:1
#48 0x000055922f2b3425 llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#49 0x000055922f2b3425 llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#50 0x000055922f2b3425 mlir::OwningOpRef<mlir::ModuleOp> mlir::detail::parseSourceFile<mlir::ModuleOp, std::shared_ptr<llvm::SourceMgr> const&>(mlir::ParserConfig const&, std::shared_ptr<llvm::SourceMgr> const&) /workdir/llvm-project/mlir/include/mlir/Parser/Parser.h:159:7
#51 0x000055922f2b2cd8 std::enable_if<__and_<std::__not_<std::__is_tuple_like<mlir::ModuleOp> >, std::is_move_constructible<mlir::ModuleOp>, std::is_move_assignable<mlir::ModuleOp> >::value, void>::type std::swap<mlir::ModuleOp>(mlir::ModuleOp&, mlir::ModuleOp&) /usr/bin/../lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/bits/move.h:205:11
#52 0x000055922f2b2cd8 mlir::OwningOpRef<mlir::ModuleOp>::release() /workdir/llvm-project/mlir/include/mlir/IR/OwningOpRef.h:69:5
#53 0x000055922f2b2cd8 mlir::OwningOpRef<mlir::ModuleOp>::operator mlir::OwningOpRef<mlir::Operation*>() && /workdir/llvm-project/mlir/include/mlir/IR/OwningOpRef.h:64:51
#54 0x000055922f2b2cd8 mlir::parseSourceFileForTool(std::shared_ptr<llvm::SourceMgr> const&, mlir::ParserConfig const&, bool) /workdir/llvm-project/mlir/include/mlir/Tools/ParseUtilities.h:31:12
#55 0x000055922f2b16ff performActions(llvm::raw_ostream&, std::shared_ptr<llvm::SourceMgr> const&, mlir::MLIRContext*, mlir::MlirOptMainConfig const&) /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:0:33
#56 0x000055922f2b0fe4 processBuffer(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, mlir::MlirOptMainConfig const&, mlir::DialectRegistry&, llvm::ThreadPoolInterface*) /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:551:12
#57 0x000055922f2b0fe4 mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_3::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::raw_ostream&) const /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:635:12
#58 0x000055922f2b0fe4 llvm::LogicalResult llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::raw_ostream&)>::callback_fn<mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_3>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::raw_ostream&) /workdir/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#59 0x000055922f4e9eac std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >::~unique_ptr() /usr/bin/../lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/bits/unique_ptr.h:360:6
#60 0x000055922f4e9eac llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::raw_ostream&)>::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::raw_ostream&) const /workdir/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#61 0x000055922f4e94c3 std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >::~unique_ptr() /usr/bin/../lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/bits/unique_ptr.h:360:6
#62 0x000055922f4e94c3 mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef) /workdir/llvm-project/mlir/lib/Support/ToolUtilities.cpp:27:5
#63 0x000055922f29fb1d mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&) /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:638:10
#64 0x000055922f2a0405 llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#65 0x000055922f2a0405 llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#66 0x000055922f2a0405 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&) /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:679:7
#67 0x000055922f2a0b53 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&) /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:695:10
#68 0x00005592251d4987 llvm::LogicalResult::succeeded() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:40:45
#69 0x00005592251d4987 mlir::asMainReturnCode(llvm::LogicalResult) /workdir/llvm-project/mlir/include/mlir/Tools/mlir-opt/MlirOptMain.h:355:12
#70 0x00005592251d4987 main /workdir/llvm-project/mlir/tools/mlir-opt/mlir-opt.cpp:339:10
#71 0x00007f24c7233d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#72 0x00007f24c7233e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#73 0x0000559225113ee5 _start (/workdir/llvm-project/build/bin/mlir-opt+0xf5b5ee5)
Aborted (core dumped)
```


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: None (sairam2661)

<details>
Test commit: [5a86dc996c26299de63effc927075dcbfb924167](https://github.com/llvm/llvm-project/pull/157520/commits/5a86dc996c26299de63effc927075dcbfb924167)

**Description**:
The CallGraph construction crashes with an assertion failure while it verifies the `transform.with_named_sequence` attribute on the module containing a `func.call` operation with an invalid nested region. 

**Steps to reproduce**
- Minimal MLIR program (test.mlir)
```
"builtin.module"() ({
  "transform.named_sequence"() &lt;{
    function_type = (!transform.any_op) -&gt; (),
    sym_name = "__transform_main"
  }&gt; ({
  ^bb0(%arg0: !transform.any_op):
    "func.call"() &lt;{
      function_type = () -&gt; (),
      sym_name = "lambda_function"
    }&gt; ({
    ^bb0():
      "func.return"() : () -&gt; ()
    }) : () -&gt; ()
    "transform.yield"() : () -&gt; ()
  }) : () -&gt; ()
}) {transform.with_named_sequence} : () -&gt; ()
```

**Command**
```
mlir-opt test.mlir
```

**Output**
```
mlir-opt: /workdir/llvm-project/llvm/include/llvm/Support/Casting.h:656: decltype(auto) llvm::dyn_cast(From &amp;) [To = mlir::Value, From = mlir::CallInterfaceCallable]: Assertion `detail::isPresent(Val) &amp;&amp; "dyn_cast on a non-existent value"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.      Program arguments: /workdir/llvm-project/build/bin/mlir-opt test.mlir
 #<!-- -->0 0x000055922514fa1b backtrace (/workdir/llvm-project/build/bin/mlir-opt+0xf5f1a1b)
 #<!-- -->1 0x00005592253d4cfc llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /workdir/llvm-project/llvm/lib/Support/Unix/Signals.inc:834:13
 #<!-- -->2 0x00005592253cf17f llvm::sys::RunSignalHandlers() /workdir/llvm-project/llvm/lib/Support/Signals.cpp:105:18
 #<!-- -->3 0x00005592253d7316 SignalHandler(int, siginfo_t*, void*) /workdir/llvm-project/llvm/lib/Support/Unix/Signals.inc:426:38
 #<!-- -->4 0x00007f24c724c520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->5 0x00007f24c72a09fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->6 0x00007f24c724c476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->7 0x00007f24c72327f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->8 0x00007f24c723271b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->9 0x00007f24c7243e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
#<!-- -->10 0x0000559237198923 decltype(auto) llvm::dyn_cast&lt;mlir::Value, mlir::CallInterfaceCallable&gt;(mlir::CallInterfaceCallable&amp;) /workdir/llvm-project/llvm/include/llvm/Support/Casting.h:656:3
#<!-- -->11 0x0000559237198923 mlir::call_interface_impl::resolveCallable(mlir::CallOpInterface, mlir::SymbolTableCollection*) /workdir/llvm-project/mlir/lib/Interfaces/CallInterfaces.cpp:190:24
#<!-- -->12 0x0000559226908954 mlir::detail::CallOpInterfaceTrait&lt;mlir::func::CallOp&gt;::resolveCallableInTable(mlir::SymbolTableCollection*) /workdir/llvm-project/build/tools/mlir/include/mlir/Interfaces/CallInterfaces.h.inc:156:7
#<!-- -->13 0x0000559226908954 mlir::detail::CallOpInterfaceInterfaceTraits::Model&lt;mlir::func::CallOp&gt;::resolveCallableInTable(mlir::detail::CallOpInterfaceInterfaceTraits::Concept const*, mlir::Operation*, mlir::SymbolTableCollection*) /workdir/llvm-project/build/tools/mlir/include/mlir/Interfaces/CallInterfaces.h.inc:303:56
#<!-- -->14 0x000055922f215da8 mlir::CallGraph::resolveCallable(mlir::CallOpInterface, mlir::SymbolTableCollection&amp;) const /workdir/llvm-project/mlir/lib/Analysis/CallGraph.cpp:150:12
#<!-- -->15 0x000055922f214c24 computeCallGraph(mlir::Operation*, mlir::CallGraph&amp;, mlir::SymbolTableCollection&amp;, mlir::CallGraphNode*, bool) /workdir/llvm-project/mlir/lib/Analysis/CallGraph.cpp:82:19
#<!-- -->16 0x000055922f214f11 computeCallGraph(mlir::Operation*, mlir::CallGraph&amp;, mlir::SymbolTableCollection&amp;, mlir::CallGraphNode*, bool) /workdir/llvm-project/mlir/lib/Analysis/CallGraph.cpp:0:7
#<!-- -->17 0x000055922f214f11 computeCallGraph(mlir::Operation*, mlir::CallGraph&amp;, mlir::SymbolTableCollection&amp;, mlir::CallGraphNode*, bool) /workdir/llvm-project/mlir/lib/Analysis/CallGraph.cpp:0:7
#<!-- -->18 0x000055922f214771 mlir::CallGraph::CallGraph(mlir::Operation*) /workdir/llvm-project/mlir/lib/Analysis/CallGraph.cpp:0:3
#<!-- -->19 0x000055922db4f7b0 mlir::transform::TransformDialect::verifyOperationAttribute(mlir::Operation*, mlir::NamedAttribute) /workdir/llvm-project/mlir/lib/Dialect/Transform/IR/TransformDialect.cpp:144:10
#<!-- -->20 0x000055922f85cd28 llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#<!-- -->21 0x000055922f85cd28 llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#<!-- -->22 0x000055922f85cd28 (anonymous namespace)::OperationVerifier::verifyOnEntrance(mlir::Operation&amp;) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:167:11
#<!-- -->23 0x000055922f85cd28 auto (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;)::$_2::operator()&lt;mlir::Operation&gt;(mlir::Operation*) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:296:45
#<!-- -->24 0x000055922f85cd28 auto (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;)::$_1::operator()&lt;(anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;)::$_2&gt;((anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;)::$_2&amp;&amp;, llvm::PointerUnion&lt;mlir::Operation*, mlir::Block*&gt;) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:276:16
#<!-- -->25 0x000055922f85cd28 (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:295:16
#<!-- -->26 0x000055922f85cd28 (anonymous namespace)::OperationVerifier::verifyOpAndDominance(mlir::Operation&amp;) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:81:14
#<!-- -->27 0x000055922f86224d llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#<!-- -->28 0x000055922f86224d llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#<!-- -->29 0x000055922f86224d (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3::operator()(mlir::Operation*) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:225:9
#<!-- -->30 0x000055922f86224d auto void mlir::parallelForEach&lt;mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&gt;(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&amp;&amp;)::'lambda'(mlir::Operation**&amp;&amp;)::operator()&lt;mlir::Operation*&amp;&gt;(mlir::Operation**&amp;&amp;) const /workdir/llvm-project/mlir/include/mlir/IR/Threading.h:120:12
#<!-- -->31 0x000055922f85d941 llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#<!-- -->32 0x000055922f85d941 llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#<!-- -->33 0x000055922f85d941 llvm::LogicalResult mlir::failableParallelForEach&lt;mlir::Operation**, void mlir::parallelForEach&lt;mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&gt;(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&amp;&amp;)::'lambda'(mlir::Operation**&amp;&amp;)&gt;(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&amp;&amp;) /workdir/llvm-project/mlir/include/mlir/IR/Threading.h:46:11
#<!-- -->34 0x000055922f85d941 void mlir::parallelForEach&lt;mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&gt;(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&amp;&amp;) /workdir/llvm-project/mlir/include/mlir/IR/Threading.h:119:9
#<!-- -->35 0x000055922f85d941 void mlir::parallelForEach&lt;llvm::SmallVector&lt;mlir::Operation*, 6u&gt;&amp;, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&gt;(mlir::MLIRContext*, llvm::SmallVector&lt;mlir::Operation*, 6u&gt;&amp;, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&amp;&amp;) /workdir/llvm-project/mlir/include/mlir/IR/Threading.h:131:3
#<!-- -->36 0x000055922f85d941 (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:224:3
#<!-- -->37 0x000055922f85d941 auto (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;)::$_0::operator()&lt;mlir::Operation&gt;(mlir::Operation*) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:287:53
#<!-- -->38 0x000055922f85d941 auto (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;)::$_1::operator()&lt;(anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;)::$_0&gt;((anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;)::$_0&amp;&amp;, llvm::PointerUnion&lt;mlir::Operation*, mlir::Block*&gt;) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:276:16
#<!-- -->39 0x000055922f85d941 (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:287:15
#<!-- -->40 0x000055922f85d941 (anonymous namespace)::OperationVerifier::verifyOpAndDominance(mlir::Operation&amp;) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:81:14
#<!-- -->41 0x000055922f85b9f3 mlir::verify(mlir::Operation*, bool) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:426:1
#<!-- -->42 0x0000559236f4dd69 llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#<!-- -->43 0x0000559236f4dd69 llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#<!-- -->44 0x0000559236f4dd69 (anonymous namespace)::OperationParser::finalize() /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:931:48
#<!-- -->45 0x0000559236f4dd69 (anonymous namespace)::TopLevelOperationParser::parse(mlir::Block*, mlir::Location) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:2840:20
#<!-- -->46 0x0000559236f4dd69 mlir::parseAsmSourceFile(llvm::SourceMgr const&amp;, mlir::Block*, mlir::ParserConfig const&amp;, mlir::AsmParserState*, mlir::AsmParserCodeCompleteContext*) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:2894:41
#<!-- -->47 0x000055922f2b61de mlir::parseSourceFile(std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::Block*, mlir::ParserConfig const&amp;, mlir::LocationAttr*) /workdir/llvm-project/mlir/lib/Parser/Parser.cpp:47:1
#<!-- -->48 0x000055922f2b3425 llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#<!-- -->49 0x000055922f2b3425 llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#<!-- -->50 0x000055922f2b3425 mlir::OwningOpRef&lt;mlir::ModuleOp&gt; mlir::detail::parseSourceFile&lt;mlir::ModuleOp, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;&gt;(mlir::ParserConfig const&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;) /workdir/llvm-project/mlir/include/mlir/Parser/Parser.h:159:7
#<!-- -->51 0x000055922f2b2cd8 std::enable_if&lt;__and_&lt;std::__not_&lt;std::__is_tuple_like&lt;mlir::ModuleOp&gt; &gt;, std::is_move_constructible&lt;mlir::ModuleOp&gt;, std::is_move_assignable&lt;mlir::ModuleOp&gt; &gt;::value, void&gt;::type std::swap&lt;mlir::ModuleOp&gt;(mlir::ModuleOp&amp;, mlir::ModuleOp&amp;) /usr/bin/../lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/bits/move.h:205:11
#<!-- -->52 0x000055922f2b2cd8 mlir::OwningOpRef&lt;mlir::ModuleOp&gt;::release() /workdir/llvm-project/mlir/include/mlir/IR/OwningOpRef.h:69:5
#<!-- -->53 0x000055922f2b2cd8 mlir::OwningOpRef&lt;mlir::ModuleOp&gt;::operator mlir::OwningOpRef&lt;mlir::Operation*&gt;() &amp;&amp; /workdir/llvm-project/mlir/include/mlir/IR/OwningOpRef.h:64:51
#<!-- -->54 0x000055922f2b2cd8 mlir::parseSourceFileForTool(std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::ParserConfig const&amp;, bool) /workdir/llvm-project/mlir/include/mlir/Tools/ParseUtilities.h:31:12
#<!-- -->55 0x000055922f2b16ff performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:0:33
#<!-- -->56 0x000055922f2b0fe4 processBuffer(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, mlir::MlirOptMainConfig const&amp;, mlir::DialectRegistry&amp;, llvm::ThreadPoolInterface*) /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:551:12
#<!-- -->57 0x000055922f2b0fe4 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_3::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;) const /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:635:12
#<!-- -->58 0x000055922f2b0fe4 llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_3&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;) /workdir/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->59 0x000055922f4e9eac std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;::~unique_ptr() /usr/bin/../lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/bits/unique_ptr.h:360:6
#<!-- -->60 0x000055922f4e9eac llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;)&gt;::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;) const /workdir/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->61 0x000055922f4e94c3 std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;::~unique_ptr() /usr/bin/../lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/bits/unique_ptr.h:360:6
#<!-- -->62 0x000055922f4e94c3 mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) /workdir/llvm-project/mlir/lib/Support/ToolUtilities.cpp:27:5
#<!-- -->63 0x000055922f29fb1d mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:638:10
#<!-- -->64 0x000055922f2a0405 llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#<!-- -->65 0x000055922f2a0405 llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#<!-- -->66 0x000055922f2a0405 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:679:7
#<!-- -->67 0x000055922f2a0b53 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:695:10
#<!-- -->68 0x00005592251d4987 llvm::LogicalResult::succeeded() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:40:45
#<!-- -->69 0x00005592251d4987 mlir::asMainReturnCode(llvm::LogicalResult) /workdir/llvm-project/mlir/include/mlir/Tools/mlir-opt/MlirOptMain.h:355:12
#<!-- -->70 0x00005592251d4987 main /workdir/llvm-project/mlir/tools/mlir-opt/mlir-opt.cpp:339:10
#<!-- -->71 0x00007f24c7233d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->72 0x00007f24c7233e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->73 0x0000559225113ee5 _start (/workdir/llvm-project/build/bin/mlir-opt+0xf5b5ee5)
Aborted (core dumped)
```

</details>


---

### Comment 2 - ftynse

This likely has the same cause as https://github.com/llvm/llvm-project/issues/159646.

---

