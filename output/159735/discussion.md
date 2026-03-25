# [MLIR] MLIR parser crashes on mixed string/integer literals in dense tensor attribute

**Author:** sairam2661
**URL:** https://github.com/llvm/llvm-project/issues/159735
**Status:** Closed
**Labels:** mlir, crash
**Closed Date:** 2026-01-23T22:48:35Z

## Body

Test commit: [5a86dc996c26299de63effc927075dcbfb924167](https://github.com/llvm/llvm-project/pull/157520/commits/5a86dc996c26299de63effc927075dcbfb924167)

**Description**:
The parser crashes with an assertion failure in `getIntAttrElements()` when a dense tensor attribute contains mixed string and integer literals but is typed as an integer tensor.

**Steps to reproduce**
- Minimal MLIR program (test.mlir)
```
"builtin.module"() ({
  "tosa.const"() <{values = dense<["string", 1]> : tensor<2xi32>}> : () -> tensor<2xi32>
}) : () -> ()
```

**Command**
```
mlir-opt test.mlir
```

**Output**
```
mlir-opt: /workdir/llvm-project/mlir/lib/AsmParser/AttributeParser.cpp:642: llvm::ParseResult (anonymous namespace)::TensorLiteralParser::getIntAttrElements(llvm::SMLoc, mlir::Type, std::vector<APInt> &): Assertion `token.isAny(Token::integer, Token::kw_true, Token::kw_false) && "unexpected token type"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.      Program arguments: /workdir/llvm-project/build/bin/mlir-opt test.mlir
 #0 0x000055d502b1ea1b backtrace (/workdir/llvm-project/build/bin/mlir-opt+0xf5f1a1b)
 #1 0x000055d502da3cfc llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /workdir/llvm-project/llvm/lib/Support/Unix/Signals.inc:834:13
 #2 0x000055d502d9e17f llvm::sys::RunSignalHandlers() /workdir/llvm-project/llvm/lib/Support/Signals.cpp:105:18
 #3 0x000055d502da6316 SignalHandler(int, siginfo_t*, void*) /workdir/llvm-project/llvm/lib/Support/Unix/Signals.inc:426:38
 #4 0x00007fa80a503520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #5 0x00007fa80a5579fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #6 0x00007fa80a503476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #7 0x00007fa80a4e97f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #8 0x00007fa80a4e971b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #9 0x00007fa80a4fae96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
#10 0x000055d5148faf47 (anonymous namespace)::TensorLiteralParser::getIntAttrElements(llvm::SMLoc, mlir::Type, std::vector<llvm::APInt, std::allocator<llvm::APInt> >&) /workdir/llvm-project/mlir/lib/AsmParser/AttributeParser.cpp:641:5
#11 0x000055d5148faf47 (anonymous namespace)::TensorLiteralParser::getAttr(llvm::SMLoc, mlir::ShapedType) /workdir/llvm-project/mlir/lib/AsmParser/AttributeParser.cpp:589:16
#12 0x000055d5148f1caf mlir::detail::Parser::parseDenseElementsAttr(mlir::Type) /workdir/llvm-project/mlir/lib/AsmParser/AttributeParser.cpp:973:24
#13 0x000055d5148f052f mlir::detail::Parser::parseAttribute(mlir::Type) /workdir/llvm-project/mlir/lib/AsmParser/AttributeParser.cpp:0:12
#14 0x000055d514901859 mlir::Attribute::operator!() const /workdir/llvm-project/mlir/include/mlir/IR/Attributes.h:48:40
#15 0x000055d514901859 mlir::detail::Parser::parseAttributeDict(mlir::NamedAttrList&)::$_1::operator()() const /workdir/llvm-project/mlir/lib/AsmParser/AttributeParser.cpp:329:9
#16 0x000055d514901859 llvm::ParseResult llvm::function_ref<llvm::ParseResult ()>::callback_fn<mlir::detail::Parser::parseAttributeDict(mlir::NamedAttrList&)::$_1>(long) /workdir/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#17 0x000055d514915d3d llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#18 0x000055d514915d3d llvm::ParseResult::operator bool() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:124:53
#19 0x000055d514915d3d mlir::detail::Parser::parseCommaSeparatedList(mlir::AsmParser::Delimiter, llvm::function_ref<llvm::ParseResult ()>, llvm::StringRef) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:138:7
#20 0x000055d5148f2fe9 mlir::detail::Parser::parseAttributeDict(mlir::NamedAttrList&) /workdir/llvm-project/mlir/lib/AsmParser/AttributeParser.cpp:335:10
#21 0x000055d5148f0ae8 llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#22 0x000055d5148f0ae8 llvm::ParseResult::operator bool() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:124:53
#23 0x000055d5148f0ae8 mlir::detail::Parser::parseAttribute(mlir::Type) /workdir/llvm-project/mlir/lib/AsmParser/AttributeParser.cpp:109:9
#24 0x000055d51493746d (anonymous namespace)::OperationParser::parseGenericOperationAfterOpName(mlir::OperationState&, std::optional<llvm::ArrayRef<mlir::OpAsmParser::UnresolvedOperand> >, std::optional<llvm::ArrayRef<mlir::Block*> >, std::optional<llvm::MutableArrayRef<std::unique_ptr<mlir::Region, std::default_delete<mlir::Region> > > >, std::optional<llvm::ArrayRef<mlir::NamedAttribute> >, std::optional<mlir::Attribute>, std::optional<mlir::FunctionType>) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:0:29
#25 0x000055d51492705e (anonymous namespace)::OperationParser::parseGenericOperation() /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:1527:7
#26 0x000055d51491ec83 (anonymous namespace)::OperationParser::parseOperation() /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:1270:10
#27 0x000055d514941a7b llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#28 0x000055d514941a7b llvm::ParseResult::operator bool() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:124:53
#29 0x000055d514941a7b (anonymous namespace)::OperationParser::parseBlockBody(mlir::Block*) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:2404:9
#30 0x000055d51493fe27 (anonymous namespace)::OperationParser::parseBlock(mlir::Block*&) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:2334:12
#31 0x000055d51493954d llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#32 0x000055d51493954d llvm::ParseResult::operator bool() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:124:53
#33 0x000055d51493954d (anonymous namespace)::OperationParser::parseRegionBody(mlir::Region&, llvm::SMLoc, llvm::ArrayRef<mlir::OpAsmParser::Argument>, bool) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:2292:7
#34 0x000055d51493954d (anonymous namespace)::OperationParser::parseRegion(mlir::Region&, llvm::ArrayRef<mlir::OpAsmParser::Argument>, bool) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:2221:7
#35 0x000055d51493767b llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#36 0x000055d51493767b llvm::ParseResult::operator bool() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:124:53
#37 0x000055d51493767b (anonymous namespace)::OperationParser::parseGenericOperationAfterOpName(mlir::OperationState&, std::optional<llvm::ArrayRef<mlir::OpAsmParser::UnresolvedOperand> >, std::optional<llvm::ArrayRef<mlir::Block*> >, std::optional<llvm::MutableArrayRef<std::unique_ptr<mlir::Region, std::default_delete<mlir::Region> > > >, std::optional<llvm::ArrayRef<mlir::NamedAttribute> >, std::optional<mlir::Attribute>, std::optional<mlir::FunctionType>) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:1430:13
#38 0x000055d51492705e (anonymous namespace)::OperationParser::parseGenericOperation() /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:1527:7
#39 0x000055d51491ec83 (anonymous namespace)::OperationParser::parseOperation() /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:1270:10
#40 0x000055d51491c06c llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#41 0x000055d51491c06c llvm::ParseResult::operator bool() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:124:53
#42 0x000055d51491c06c (anonymous namespace)::TopLevelOperationParser::parse(mlir::Block*, mlir::Location) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:2834:11
#43 0x000055d51491c06c mlir::parseAsmSourceFile(llvm::SourceMgr const&, mlir::Block*, mlir::ParserConfig const&, mlir::AsmParserState*, mlir::AsmParserCodeCompleteContext*) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:2894:41
#44 0x000055d50cc851de mlir::parseSourceFile(std::shared_ptr<llvm::SourceMgr> const&, mlir::Block*, mlir::ParserConfig const&, mlir::LocationAttr*) /workdir/llvm-project/mlir/lib/Parser/Parser.cpp:47:1
#45 0x000055d50cc82425 llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#46 0x000055d50cc82425 llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#47 0x000055d50cc82425 mlir::OwningOpRef<mlir::ModuleOp> mlir::detail::parseSourceFile<mlir::ModuleOp, std::shared_ptr<llvm::SourceMgr> const&>(mlir::ParserConfig const&, std::shared_ptr<llvm::SourceMgr> const&) /workdir/llvm-project/mlir/include/mlir/Parser/Parser.h:159:7
#48 0x000055d50cc81cd8 std::enable_if<__and_<std::__not_<std::__is_tuple_like<mlir::ModuleOp> >, std::is_move_constructible<mlir::ModuleOp>, std::is_move_assignable<mlir::ModuleOp> >::value, void>::type std::swap<mlir::ModuleOp>(mlir::ModuleOp&, mlir::ModuleOp&) /usr/bin/../lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/bits/move.h:205:11
#49 0x000055d50cc81cd8 mlir::OwningOpRef<mlir::ModuleOp>::release() /workdir/llvm-project/mlir/include/mlir/IR/OwningOpRef.h:69:5
#50 0x000055d50cc81cd8 mlir::OwningOpRef<mlir::ModuleOp>::operator mlir::OwningOpRef<mlir::Operation*>() && /workdir/llvm-project/mlir/include/mlir/IR/OwningOpRef.h:64:51
#51 0x000055d50cc81cd8 mlir::parseSourceFileForTool(std::shared_ptr<llvm::SourceMgr> const&, mlir::ParserConfig const&, bool) /workdir/llvm-project/mlir/include/mlir/Tools/ParseUtilities.h:31:12
#52 0x000055d50cc806ff performActions(llvm::raw_ostream&, std::shared_ptr<llvm::SourceMgr> const&, mlir::MLIRContext*, mlir::MlirOptMainConfig const&) /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:0:33
#53 0x000055d50cc7ffe4 processBuffer(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, mlir::MlirOptMainConfig const&, mlir::DialectRegistry&, llvm::ThreadPoolInterface*) /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:551:12
#54 0x000055d50cc7ffe4 mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_3::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::raw_ostream&) const /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:635:12
#55 0x000055d50cc7ffe4 llvm::LogicalResult llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::raw_ostream&)>::callback_fn<mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_3>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::raw_ostream&) /workdir/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#56 0x000055d50ceb8eac std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >::~unique_ptr() /usr/bin/../lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/bits/unique_ptr.h:360:6
#57 0x000055d50ceb8eac llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::raw_ostream&)>::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::raw_ostream&) const /workdir/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#58 0x000055d50ceb84c3 std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >::~unique_ptr() /usr/bin/../lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/bits/unique_ptr.h:360:6
#59 0x000055d50ceb84c3 mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef) /workdir/llvm-project/mlir/lib/Support/ToolUtilities.cpp:27:5
#60 0x000055d50cc6eb1d mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&) /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:638:10
#61 0x000055d50cc6f405 llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#62 0x000055d50cc6f405 llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#63 0x000055d50cc6f405 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&) /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:679:7
#64 0x000055d50cc6fb53 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&) /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:695:10
#65 0x000055d502ba3987 llvm::LogicalResult::succeeded() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:40:45
#66 0x000055d502ba3987 mlir::asMainReturnCode(llvm::LogicalResult) /workdir/llvm-project/mlir/include/mlir/Tools/mlir-opt/MlirOptMain.h:355:12
#67 0x000055d502ba3987 main /workdir/llvm-project/mlir/tools/mlir-opt/mlir-opt.cpp:339:10
#68 0x00007fa80a4ead90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#69 0x00007fa80a4eae40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#70 0x000055d502ae2ee5 _start (/workdir/llvm-project/build/bin/mlir-opt+0xf5b5ee5)
Aborted (core dumped)
```


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: None (sairam2661)

<details>
Test commit: [5a86dc996c26299de63effc927075dcbfb924167](https://github.com/llvm/llvm-project/pull/157520/commits/5a86dc996c26299de63effc927075dcbfb924167)

**Description**:
The parser crashes with an assertion failure in `getIntAttrElements()` when a dense tensor attribute contains mixed string and integer literals but is typed as an integer tensor.

**Steps to reproduce**
- Minimal MLIR program (test.mlir)
```
"builtin.module"() ({
  "tosa.const"() &lt;{values = dense&lt;["string", 1]&gt; : tensor&lt;2xi32&gt;}&gt; : () -&gt; tensor&lt;2xi32&gt;
}) : () -&gt; ()
```

**Command**
```
mlir-opt test.mlir
```

**Output**
```
mlir-opt: /workdir/llvm-project/mlir/lib/AsmParser/AttributeParser.cpp:642: llvm::ParseResult (anonymous namespace)::TensorLiteralParser::getIntAttrElements(llvm::SMLoc, mlir::Type, std::vector&lt;APInt&gt; &amp;): Assertion `token.isAny(Token::integer, Token::kw_true, Token::kw_false) &amp;&amp; "unexpected token type"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.      Program arguments: /workdir/llvm-project/build/bin/mlir-opt test.mlir
 #<!-- -->0 0x000055d502b1ea1b backtrace (/workdir/llvm-project/build/bin/mlir-opt+0xf5f1a1b)
 #<!-- -->1 0x000055d502da3cfc llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /workdir/llvm-project/llvm/lib/Support/Unix/Signals.inc:834:13
 #<!-- -->2 0x000055d502d9e17f llvm::sys::RunSignalHandlers() /workdir/llvm-project/llvm/lib/Support/Signals.cpp:105:18
 #<!-- -->3 0x000055d502da6316 SignalHandler(int, siginfo_t*, void*) /workdir/llvm-project/llvm/lib/Support/Unix/Signals.inc:426:38
 #<!-- -->4 0x00007fa80a503520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->5 0x00007fa80a5579fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->6 0x00007fa80a503476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->7 0x00007fa80a4e97f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->8 0x00007fa80a4e971b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->9 0x00007fa80a4fae96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
#<!-- -->10 0x000055d5148faf47 (anonymous namespace)::TensorLiteralParser::getIntAttrElements(llvm::SMLoc, mlir::Type, std::vector&lt;llvm::APInt, std::allocator&lt;llvm::APInt&gt; &gt;&amp;) /workdir/llvm-project/mlir/lib/AsmParser/AttributeParser.cpp:641:5
#<!-- -->11 0x000055d5148faf47 (anonymous namespace)::TensorLiteralParser::getAttr(llvm::SMLoc, mlir::ShapedType) /workdir/llvm-project/mlir/lib/AsmParser/AttributeParser.cpp:589:16
#<!-- -->12 0x000055d5148f1caf mlir::detail::Parser::parseDenseElementsAttr(mlir::Type) /workdir/llvm-project/mlir/lib/AsmParser/AttributeParser.cpp:973:24
#<!-- -->13 0x000055d5148f052f mlir::detail::Parser::parseAttribute(mlir::Type) /workdir/llvm-project/mlir/lib/AsmParser/AttributeParser.cpp:0:12
#<!-- -->14 0x000055d514901859 mlir::Attribute::operator!() const /workdir/llvm-project/mlir/include/mlir/IR/Attributes.h:48:40
#<!-- -->15 0x000055d514901859 mlir::detail::Parser::parseAttributeDict(mlir::NamedAttrList&amp;)::$_1::operator()() const /workdir/llvm-project/mlir/lib/AsmParser/AttributeParser.cpp:329:9
#<!-- -->16 0x000055d514901859 llvm::ParseResult llvm::function_ref&lt;llvm::ParseResult ()&gt;::callback_fn&lt;mlir::detail::Parser::parseAttributeDict(mlir::NamedAttrList&amp;)::$_1&gt;(long) /workdir/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->17 0x000055d514915d3d llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#<!-- -->18 0x000055d514915d3d llvm::ParseResult::operator bool() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:124:53
#<!-- -->19 0x000055d514915d3d mlir::detail::Parser::parseCommaSeparatedList(mlir::AsmParser::Delimiter, llvm::function_ref&lt;llvm::ParseResult ()&gt;, llvm::StringRef) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:138:7
#<!-- -->20 0x000055d5148f2fe9 mlir::detail::Parser::parseAttributeDict(mlir::NamedAttrList&amp;) /workdir/llvm-project/mlir/lib/AsmParser/AttributeParser.cpp:335:10
#<!-- -->21 0x000055d5148f0ae8 llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#<!-- -->22 0x000055d5148f0ae8 llvm::ParseResult::operator bool() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:124:53
#<!-- -->23 0x000055d5148f0ae8 mlir::detail::Parser::parseAttribute(mlir::Type) /workdir/llvm-project/mlir/lib/AsmParser/AttributeParser.cpp:109:9
#<!-- -->24 0x000055d51493746d (anonymous namespace)::OperationParser::parseGenericOperationAfterOpName(mlir::OperationState&amp;, std::optional&lt;llvm::ArrayRef&lt;mlir::OpAsmParser::UnresolvedOperand&gt; &gt;, std::optional&lt;llvm::ArrayRef&lt;mlir::Block*&gt; &gt;, std::optional&lt;llvm::MutableArrayRef&lt;std::unique_ptr&lt;mlir::Region, std::default_delete&lt;mlir::Region&gt; &gt; &gt; &gt;, std::optional&lt;llvm::ArrayRef&lt;mlir::NamedAttribute&gt; &gt;, std::optional&lt;mlir::Attribute&gt;, std::optional&lt;mlir::FunctionType&gt;) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:0:29
#<!-- -->25 0x000055d51492705e (anonymous namespace)::OperationParser::parseGenericOperation() /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:1527:7
#<!-- -->26 0x000055d51491ec83 (anonymous namespace)::OperationParser::parseOperation() /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:1270:10
#<!-- -->27 0x000055d514941a7b llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#<!-- -->28 0x000055d514941a7b llvm::ParseResult::operator bool() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:124:53
#<!-- -->29 0x000055d514941a7b (anonymous namespace)::OperationParser::parseBlockBody(mlir::Block*) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:2404:9
#<!-- -->30 0x000055d51493fe27 (anonymous namespace)::OperationParser::parseBlock(mlir::Block*&amp;) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:2334:12
#<!-- -->31 0x000055d51493954d llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#<!-- -->32 0x000055d51493954d llvm::ParseResult::operator bool() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:124:53
#<!-- -->33 0x000055d51493954d (anonymous namespace)::OperationParser::parseRegionBody(mlir::Region&amp;, llvm::SMLoc, llvm::ArrayRef&lt;mlir::OpAsmParser::Argument&gt;, bool) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:2292:7
#<!-- -->34 0x000055d51493954d (anonymous namespace)::OperationParser::parseRegion(mlir::Region&amp;, llvm::ArrayRef&lt;mlir::OpAsmParser::Argument&gt;, bool) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:2221:7
#<!-- -->35 0x000055d51493767b llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#<!-- -->36 0x000055d51493767b llvm::ParseResult::operator bool() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:124:53
#<!-- -->37 0x000055d51493767b (anonymous namespace)::OperationParser::parseGenericOperationAfterOpName(mlir::OperationState&amp;, std::optional&lt;llvm::ArrayRef&lt;mlir::OpAsmParser::UnresolvedOperand&gt; &gt;, std::optional&lt;llvm::ArrayRef&lt;mlir::Block*&gt; &gt;, std::optional&lt;llvm::MutableArrayRef&lt;std::unique_ptr&lt;mlir::Region, std::default_delete&lt;mlir::Region&gt; &gt; &gt; &gt;, std::optional&lt;llvm::ArrayRef&lt;mlir::NamedAttribute&gt; &gt;, std::optional&lt;mlir::Attribute&gt;, std::optional&lt;mlir::FunctionType&gt;) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:1430:13
#<!-- -->38 0x000055d51492705e (anonymous namespace)::OperationParser::parseGenericOperation() /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:1527:7
#<!-- -->39 0x000055d51491ec83 (anonymous namespace)::OperationParser::parseOperation() /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:1270:10
#<!-- -->40 0x000055d51491c06c llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#<!-- -->41 0x000055d51491c06c llvm::ParseResult::operator bool() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:124:53
#<!-- -->42 0x000055d51491c06c (anonymous namespace)::TopLevelOperationParser::parse(mlir::Block*, mlir::Location) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:2834:11
#<!-- -->43 0x000055d51491c06c mlir::parseAsmSourceFile(llvm::SourceMgr const&amp;, mlir::Block*, mlir::ParserConfig const&amp;, mlir::AsmParserState*, mlir::AsmParserCodeCompleteContext*) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:2894:41
#<!-- -->44 0x000055d50cc851de mlir::parseSourceFile(std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::Block*, mlir::ParserConfig const&amp;, mlir::LocationAttr*) /workdir/llvm-project/mlir/lib/Parser/Parser.cpp:47:1
#<!-- -->45 0x000055d50cc82425 llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#<!-- -->46 0x000055d50cc82425 llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#<!-- -->47 0x000055d50cc82425 mlir::OwningOpRef&lt;mlir::ModuleOp&gt; mlir::detail::parseSourceFile&lt;mlir::ModuleOp, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;&gt;(mlir::ParserConfig const&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;) /workdir/llvm-project/mlir/include/mlir/Parser/Parser.h:159:7
#<!-- -->48 0x000055d50cc81cd8 std::enable_if&lt;__and_&lt;std::__not_&lt;std::__is_tuple_like&lt;mlir::ModuleOp&gt; &gt;, std::is_move_constructible&lt;mlir::ModuleOp&gt;, std::is_move_assignable&lt;mlir::ModuleOp&gt; &gt;::value, void&gt;::type std::swap&lt;mlir::ModuleOp&gt;(mlir::ModuleOp&amp;, mlir::ModuleOp&amp;) /usr/bin/../lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/bits/move.h:205:11
#<!-- -->49 0x000055d50cc81cd8 mlir::OwningOpRef&lt;mlir::ModuleOp&gt;::release() /workdir/llvm-project/mlir/include/mlir/IR/OwningOpRef.h:69:5
#<!-- -->50 0x000055d50cc81cd8 mlir::OwningOpRef&lt;mlir::ModuleOp&gt;::operator mlir::OwningOpRef&lt;mlir::Operation*&gt;() &amp;&amp; /workdir/llvm-project/mlir/include/mlir/IR/OwningOpRef.h:64:51
#<!-- -->51 0x000055d50cc81cd8 mlir::parseSourceFileForTool(std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::ParserConfig const&amp;, bool) /workdir/llvm-project/mlir/include/mlir/Tools/ParseUtilities.h:31:12
#<!-- -->52 0x000055d50cc806ff performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:0:33
#<!-- -->53 0x000055d50cc7ffe4 processBuffer(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, mlir::MlirOptMainConfig const&amp;, mlir::DialectRegistry&amp;, llvm::ThreadPoolInterface*) /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:551:12
#<!-- -->54 0x000055d50cc7ffe4 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_3::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;) const /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:635:12
#<!-- -->55 0x000055d50cc7ffe4 llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_3&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;) /workdir/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->56 0x000055d50ceb8eac std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;::~unique_ptr() /usr/bin/../lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/bits/unique_ptr.h:360:6
#<!-- -->57 0x000055d50ceb8eac llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;)&gt;::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;) const /workdir/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->58 0x000055d50ceb84c3 std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;::~unique_ptr() /usr/bin/../lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/bits/unique_ptr.h:360:6
#<!-- -->59 0x000055d50ceb84c3 mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) /workdir/llvm-project/mlir/lib/Support/ToolUtilities.cpp:27:5
#<!-- -->60 0x000055d50cc6eb1d mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:638:10
#<!-- -->61 0x000055d50cc6f405 llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#<!-- -->62 0x000055d50cc6f405 llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#<!-- -->63 0x000055d50cc6f405 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:679:7
#<!-- -->64 0x000055d50cc6fb53 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:695:10
#<!-- -->65 0x000055d502ba3987 llvm::LogicalResult::succeeded() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:40:45
#<!-- -->66 0x000055d502ba3987 mlir::asMainReturnCode(llvm::LogicalResult) /workdir/llvm-project/mlir/include/mlir/Tools/mlir-opt/MlirOptMain.h:355:12
#<!-- -->67 0x000055d502ba3987 main /workdir/llvm-project/mlir/tools/mlir-opt/mlir-opt.cpp:339:10
#<!-- -->68 0x00007fa80a4ead90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->69 0x00007fa80a4eae40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->70 0x000055d502ae2ee5 _start (/workdir/llvm-project/build/bin/mlir-opt+0xf5b5ee5)
Aborted (core dumped)
```

</details>


---

### Comment 2 - veera-sivarajan

Looks like this is fixed in trunk: https://godbolt.org/z/6s51qj6v4

---

### Comment 3 - foxtran

@EugeneZelenko, can it be closed?

---

