# [MLIR] vector::ShuffleOp::inferReturnTypes crashes when processing shuffle operation

**Author:** kitty-1998
**URL:** https://github.com/llvm/llvm-project/issues/185587
**Status:** Closed
**Labels:** mlir:vectorops, crash
**Closed Date:** 2026-03-11T11:00:07Z

## Body

I discovered a crash bug while using MLIR to process programs. Here's a code example for reproduction:

Git version: da8d1b52d0d344068a8bd3e46a8af3a692d2efc2

```llvm
module {
  func.func @ComputeSobel_vectorized(%pixels: vector<9xi8>, %fScale: f32) -> i8 attributes {dialect.test = #dialect.test<"custom">} {
    %c0 = arith.constant 0 : index
    %c9 = arith.constant 9 : index
    %c1 = arith.constant 1 : index
    %mem = memref.alloca() : memref<9xi8>
    vector.store %pixels, %mem[%c0] : memref<9xi8>, vector<9xi8>
    %out = memref.alloca() : memref<1xi8>
    scf.parallel (%i) = (%c0) to (%c9) step (%c1) {
      %val = memref.load %mem[%i] : memref<9xi8>
      %next = arith.addi %val, %val : i8
      memref.store %next, %out[%c0] : memref<1xi8>
      scf.yield
    }
    %result = memref.load %out[%c0] : memref<1xi8>
    %shuffle = vector.shuffle %result, %result [0] : i8, i8
    return %shuffle : i8
  }
}
```


#### Stack Trace
```bash
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: mlir-opt SobelFilter.mlir -gpu-kernel-outlining -lower-affine --convert-scf-to-cf --convert-gpu-to-nvvm -allow-unregistered-dialect
1.	MLIR Parser: custom op parser 'builtin.module'
2.	MLIR Parser: custom op parser 'func.func'
3.	MLIR Parser: custom op parser 'vector.shuffle'
 #0 0x00006025f9136dbd llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /llvm/lib/Support/Unix/Signals.inc:880:11
 #1 0x00006025f91372eb PrintStackTraceSignalHandler(void*) /llvm/lib/Support/Unix/Signals.inc:962:1
 #2 0x00006025f9135184 llvm::sys::RunSignalHandlers() /llvm/lib/Support/Signals.cpp:108:5
 #3 0x00006025f9137a89 SignalHandler(int, siginfo_t*, void*) /llvm/lib/Support/Unix/Signals.inc:448:38
 #4 0x000070f944c45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #5 0x000070f944c9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #6 0x000070f944c9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #7 0x000070f944c9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #8 0x000070f944c4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #9 0x000070f944c288ff abort ./stdlib/abort.c:81:7
#10 0x000070f944c2881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#11 0x000070f944c3b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#12 0x00006025f9308504 decltype(auto) llvm::cast<mlir::VectorType, mlir::Type>(mlir::Type const&) /llvm/include/llvm/Support/Casting.h:561:43
#13 0x0000602605a13207 mlir::vector::ShuffleOp::inferReturnTypes(mlir::MLIRContext*, std::optional<mlir::Location>, mlir::vector::ShuffleOpAdaptor, llvm::SmallVectorImpl<mlir::Type>&) /mlir/lib/Dialect/Vector/IR/VectorOps.cpp:3278:17
#14 0x0000602605a8f1da mlir::vector::ShuffleOp::inferReturnTypes(mlir::MLIRContext*, std::optional<mlir::Location>, mlir::ValueRange, mlir::DictionaryAttr, mlir::OpaqueProperties, mlir::RegionRange, llvm::SmallVectorImpl<mlir::Type>&) /build_new/tools/mlir/include/mlir/Dialect/Vector/IR/VectorOps.cpp.inc:14440:10
#15 0x0000602605a91f29 mlir::vector::ShuffleOp::parse(mlir::OpAsmParser&, mlir::OperationState&) /build_new/tools/mlir/include/mlir/Dialect/Vector/IR/VectorOps.cpp.inc:14407:22
#16 0x00006025f94bc13d llvm::ParseResult llvm::detail::UniqueFunctionBase<llvm::ParseResult, mlir::OpAsmParser&, mlir::OperationState&>::CallImpl<llvm::ParseResult (*)(mlir::OpAsmParser&, mlir::OperationState&)>(void*, mlir::OpAsmParser&, mlir::OperationState&) /llvm/include/llvm/ADT/FunctionExtras.h:212:12
#17 0x00006026053fe19f llvm::unique_function<llvm::ParseResult (mlir::OpAsmParser&, mlir::OperationState&)>::operator()(mlir::OpAsmParser&, mlir::OperationState&) /llvm/include/llvm/ADT/FunctionExtras.h:364:12
#18 0x00006026053fe145 llvm::ParseResult llvm::function_ref<llvm::ParseResult (mlir::OpAsmParser&, mlir::OperationState&)>::callback_fn<llvm::unique_function<llvm::ParseResult (mlir::OpAsmParser&, mlir::OperationState&)>>(long, mlir::OpAsmParser&, mlir::OperationState&) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#19 0x00006026053fe109 llvm::function_ref<llvm::ParseResult (mlir::OpAsmParser&, mlir::OperationState&)>::operator()(mlir::OpAsmParser&, mlir::OperationState&) const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#20 0x00006026053edc3e (anonymous namespace)::CustomOpAsmParser::parseOperation(mlir::OperationState&) /mlir/lib/AsmParser/Parser.cpp:1653:9
#21 0x00006026053ec580 (anonymous namespace)::OperationParser::parseCustomOperation(llvm::ArrayRef<std::tuple<llvm::StringRef, unsigned int, llvm::SMLoc>>) /mlir/lib/AsmParser/Parser.cpp:2192:19
#22 0x00006026053e8713 (anonymous namespace)::OperationParser::parseOperation() /mlir/lib/AsmParser/Parser.cpp:1306:8
#23 0x00006026053f5902 (anonymous namespace)::OperationParser::parseBlockBody(mlir::Block*) /mlir/lib/AsmParser/Parser.cpp:2478:9
#24 0x00006026053f49ea (anonymous namespace)::OperationParser::parseBlock(mlir::Block*&) /mlir/lib/AsmParser/Parser.cpp:2408:12
#25 0x00006026053f45c3 (anonymous namespace)::OperationParser::parseRegionBody(mlir::Region&, llvm::SMLoc, llvm::ArrayRef<mlir::OpAsmParser::Argument>, bool) /mlir/lib/AsmParser/Parser.cpp:2366:7
#26 0x00006026053f1aa7 (anonymous namespace)::OperationParser::parseRegion(mlir::Region&, llvm::ArrayRef<mlir::OpAsmParser::Argument>, bool) /mlir/lib/AsmParser/Parser.cpp:2295:7
#27 0x00006026053ef760 (anonymous namespace)::CustomOpAsmParser::parseRegion(mlir::Region&, llvm::ArrayRef<mlir::OpAsmParser::Argument>, bool) /mlir/lib/AsmParser/Parser.cpp:1917:16
#28 0x00006026053ef844 (anonymous namespace)::CustomOpAsmParser::parseOptionalRegion(mlir::Region&, llvm::ArrayRef<mlir::OpAsmParser::Argument>, bool) /mlir/lib/AsmParser/Parser.cpp:1928:12
#29 0x0000602607cf793c mlir::function_interface_impl::parseFunctionOp(mlir::OpAsmParser&, mlir::OperationState&, bool, mlir::StringAttr, llvm::function_ref<mlir::Type (mlir::Builder&, llvm::ArrayRef<mlir::Type>, llvm::ArrayRef<mlir::Type>, mlir::function_interface_impl::VariadicFlag, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>&)>, mlir::StringAttr, mlir::StringAttr) /mlir/lib/Interfaces/FunctionImplementation.cpp:154:14
#30 0x00006026059da52e mlir::func::FuncOp::parse(mlir::OpAsmParser&, mlir::OperationState&) /mlir/lib/Dialect/Func/IR/FuncOps.cpp:203:10
#31 0x00006025f94bc13d llvm::ParseResult llvm::detail::UniqueFunctionBase<llvm::ParseResult, mlir::OpAsmParser&, mlir::OperationState&>::CallImpl<llvm::ParseResult (*)(mlir::OpAsmParser&, mlir::OperationState&)>(void*, mlir::OpAsmParser&, mlir::OperationState&) /llvm/include/llvm/ADT/FunctionExtras.h:212:12
#32 0x00006026053fe19f llvm::unique_function<llvm::ParseResult (mlir::OpAsmParser&, mlir::OperationState&)>::operator()(mlir::OpAsmParser&, mlir::OperationState&) /llvm/include/llvm/ADT/FunctionExtras.h:364:12
#33 0x00006026053fe145 llvm::ParseResult llvm::function_ref<llvm::ParseResult (mlir::OpAsmParser&, mlir::OperationState&)>::callback_fn<llvm::unique_function<llvm::ParseResult (mlir::OpAsmParser&, mlir::OperationState&)>>(long, mlir::OpAsmParser&, mlir::OperationState&) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#34 0x00006026053fe109 llvm::function_ref<llvm::ParseResult (mlir::OpAsmParser&, mlir::OperationState&)>::operator()(mlir::OpAsmParser&, mlir::OperationState&) const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#35 0x00006026053edc3e (anonymous namespace)::CustomOpAsmParser::parseOperation(mlir::OperationState&) /mlir/lib/AsmParser/Parser.cpp:1653:9
#36 0x00006026053ec580 (anonymous namespace)::OperationParser::parseCustomOperation(llvm::ArrayRef<std::tuple<llvm::StringRef, unsigned int, llvm::SMLoc>>) /mlir/lib/AsmParser/Parser.cpp:2192:19
#37 0x00006026053e8713 (anonymous namespace)::OperationParser::parseOperation() /mlir/lib/AsmParser/Parser.cpp:1306:8
#38 0x00006026053f5902 (anonymous namespace)::OperationParser::parseBlockBody(mlir::Block*) /mlir/lib/AsmParser/Parser.cpp:2478:9
#39 0x00006026053f49ea (anonymous namespace)::OperationParser::parseBlock(mlir::Block*&) /mlir/lib/AsmParser/Parser.cpp:2408:12
#40 0x00006026053f45c3 (anonymous namespace)::OperationParser::parseRegionBody(mlir::Region&, llvm::SMLoc, llvm::ArrayRef<mlir::OpAsmParser::Argument>, bool) /mlir/lib/AsmParser/Parser.cpp:2366:7
#41 0x00006026053f1aa7 (anonymous namespace)::OperationParser::parseRegion(mlir::Region&, llvm::ArrayRef<mlir::OpAsmParser::Argument>, bool) /mlir/lib/AsmParser/Parser.cpp:2295:7
#42 0x00006026053ef760 (anonymous namespace)::CustomOpAsmParser::parseRegion(mlir::Region&, llvm::ArrayRef<mlir::OpAsmParser::Argument>, bool) /mlir/lib/AsmParser/Parser.cpp:1917:16
#43 0x0000602607ef158d mlir::ModuleOp::parse(mlir::OpAsmParser&, mlir::OperationState&) /build_new/tools/mlir/include/mlir/IR/BuiltinOps.cpp.inc:307:14
#44 0x00006025f94bc13d llvm::ParseResult llvm::detail::UniqueFunctionBase<llvm::ParseResult, mlir::OpAsmParser&, mlir::OperationState&>::CallImpl<llvm::ParseResult (*)(mlir::OpAsmParser&, mlir::OperationState&)>(void*, mlir::OpAsmParser&, mlir::OperationState&) /llvm/include/llvm/ADT/FunctionExtras.h:212:12
#45 0x00006026053fe19f llvm::unique_function<llvm::ParseResult (mlir::OpAsmParser&, mlir::OperationState&)>::operator()(mlir::OpAsmParser&, mlir::OperationState&) /llvm/include/llvm/ADT/FunctionExtras.h:364:12
#46 0x00006026053fe145 llvm::ParseResult llvm::function_ref<llvm::ParseResult (mlir::OpAsmParser&, mlir::OperationState&)>::callback_fn<llvm::unique_function<llvm::ParseResult (mlir::OpAsmParser&, mlir::OperationState&)>>(long, mlir::OpAsmParser&, mlir::OperationState&) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#47 0x00006026053fe109 llvm::function_ref<llvm::ParseResult (mlir::OpAsmParser&, mlir::OperationState&)>::operator()(mlir::OpAsmParser&, mlir::OperationState&) const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#48 0x00006026053edc3e (anonymous namespace)::CustomOpAsmParser::parseOperation(mlir::OperationState&) /mlir/lib/AsmParser/Parser.cpp:1653:9
#49 0x00006026053ec580 (anonymous namespace)::OperationParser::parseCustomOperation(llvm::ArrayRef<std::tuple<llvm::StringRef, unsigned int, llvm::SMLoc>>) /mlir/lib/AsmParser/Parser.cpp:2192:19
#50 0x00006026053e8713 (anonymous namespace)::OperationParser::parseOperation() /mlir/lib/AsmParser/Parser.cpp:1306:8
#51 0x00006026053e815d (anonymous namespace)::TopLevelOperationParser::parse(mlir::Block*, mlir::Location) /mlir/lib/AsmParser/Parser.cpp:2908:20
#52 0x00006026053e7ffe mlir::parseAsmSourceFile(llvm::SourceMgr const&, mlir::Block*, mlir::ParserConfig const&, mlir::AsmParserState*, mlir::AsmParserCodeCompleteContext*) /mlir/lib/AsmParser/Parser.cpp:2968:41
#53 0x00006026053b3d1f mlir::parseSourceFile(std::shared_ptr<llvm::SourceMgr> const&, mlir::Block*, mlir::ParserConfig const&, mlir::LocationAttr*) /mlir/lib/Parser/Parser.cpp:64:10
#54 0x00006025f925a54f mlir::OwningOpRef<mlir::ModuleOp> mlir::detail::parseSourceFile<mlir::ModuleOp, std::shared_ptr<llvm::SourceMgr> const&>(mlir::ParserConfig const&, std::shared_ptr<llvm::SourceMgr> const&) /mlir/include/mlir/Parser/Parser.h:158:14
#55 0x00006025f925a458 mlir::OwningOpRef<mlir::ModuleOp> mlir::parseSourceFile<mlir::ModuleOp>(std::shared_ptr<llvm::SourceMgr> const&, mlir::ParserConfig const&) /mlir/include/mlir/Parser/Parser.h:188:10
#56 0x00006025f92596cd mlir::parseSourceFileForTool(std::shared_ptr<llvm::SourceMgr> const&, mlir::ParserConfig const&, bool) /mlir/include/mlir/Tools/ParseUtilities.h:31:12
#57 0x00006025f924174a performActions(llvm::raw_ostream&, std::shared_ptr<llvm::SourceMgr> const&, mlir::MLIRContext*, mlir::MlirOptMainConfig const&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:528:16
#58 0x00006025f9241551 processBuffer(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&, mlir::DialectRegistry&, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:681:12
#59 0x00006025f924120c mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, llvm::raw_ostream&) const /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:779:12
#60 0x00006025f9241116 llvm::LogicalResult llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::callback_fn<mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#61 0x00006026080496fa llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#62 0x0000602608048aba mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef) /mlir/lib/Support/ToolUtilities.cpp:30:12
#63 0x00006025f923cdc6 mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:784:26
#64 0x00006025f923d1c8 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:830:14
#65 0x00006025f923d398 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:846:10
#66 0x00006025f910b8d5 main /mlir/tools/mlir-opt/mlir-opt.cpp:343:33
#67 0x000070f944c2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#68 0x000070f944c2a28b call_init ./csu/../csu/libc-start.c:128:20
#69 0x000070f944c2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#70 0x00006025f910b745 _start (./mlir-opt+0x481e745)
Aborted
```

#### Command for Reproduction
```bash
mlir-opt SobelFilter.mlir -gpu-kernel-outlining -lower-affine --convert-scf-to-cf --convert-gpu-to-nvvm -allow-unregistered-dialect
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: kitty-apple (kitty-1998)

<details>
I discovered a crash bug while using MLIR to process programs. Here's a code example for reproduction:

Git version: da8d1b52d0d344068a8bd3e46a8af3a692d2efc2

```llvm
module {
  func.func @<!-- -->ComputeSobel_vectorized(%pixels: vector&lt;9xi8&gt;, %fScale: f32) -&gt; i8 attributes {dialect.test = #dialect.test&lt;"custom"&gt;} {
    %c0 = arith.constant 0 : index
    %c9 = arith.constant 9 : index
    %c1 = arith.constant 1 : index
    %mem = memref.alloca() : memref&lt;9xi8&gt;
    vector.store %pixels, %mem[%c0] : memref&lt;9xi8&gt;, vector&lt;9xi8&gt;
    %out = memref.alloca() : memref&lt;1xi8&gt;
    scf.parallel (%i) = (%c0) to (%c9) step (%c1) {
      %val = memref.load %mem[%i] : memref&lt;9xi8&gt;
      %next = arith.addi %val, %val : i8
      memref.store %next, %out[%c0] : memref&lt;1xi8&gt;
      scf.yield
    }
    %result = memref.load %out[%c0] : memref&lt;1xi8&gt;
    %shuffle = vector.shuffle %result, %result [0] : i8, i8
    return %shuffle : i8
  }
}
```


#### Stack Trace
```bash
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: mlir-opt SobelFilter.mlir -gpu-kernel-outlining -lower-affine --convert-scf-to-cf --convert-gpu-to-nvvm -allow-unregistered-dialect
1.	MLIR Parser: custom op parser 'builtin.module'
2.	MLIR Parser: custom op parser 'func.func'
3.	MLIR Parser: custom op parser 'vector.shuffle'
 #<!-- -->0 0x00006025f9136dbd llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /llvm/lib/Support/Unix/Signals.inc:880:11
 #<!-- -->1 0x00006025f91372eb PrintStackTraceSignalHandler(void*) /llvm/lib/Support/Unix/Signals.inc:962:1
 #<!-- -->2 0x00006025f9135184 llvm::sys::RunSignalHandlers() /llvm/lib/Support/Signals.cpp:108:5
 #<!-- -->3 0x00006025f9137a89 SignalHandler(int, siginfo_t*, void*) /llvm/lib/Support/Unix/Signals.inc:448:38
 #<!-- -->4 0x000070f944c45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->5 0x000070f944c9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->6 0x000070f944c9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->7 0x000070f944c9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->8 0x000070f944c4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x000070f944c288ff abort ./stdlib/abort.c:81:7
#<!-- -->10 0x000070f944c2881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->11 0x000070f944c3b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->12 0x00006025f9308504 decltype(auto) llvm::cast&lt;mlir::VectorType, mlir::Type&gt;(mlir::Type const&amp;) /llvm/include/llvm/Support/Casting.h:561:43
#<!-- -->13 0x0000602605a13207 mlir::vector::ShuffleOp::inferReturnTypes(mlir::MLIRContext*, std::optional&lt;mlir::Location&gt;, mlir::vector::ShuffleOpAdaptor, llvm::SmallVectorImpl&lt;mlir::Type&gt;&amp;) /mlir/lib/Dialect/Vector/IR/VectorOps.cpp:3278:17
#<!-- -->14 0x0000602605a8f1da mlir::vector::ShuffleOp::inferReturnTypes(mlir::MLIRContext*, std::optional&lt;mlir::Location&gt;, mlir::ValueRange, mlir::DictionaryAttr, mlir::OpaqueProperties, mlir::RegionRange, llvm::SmallVectorImpl&lt;mlir::Type&gt;&amp;) /build_new/tools/mlir/include/mlir/Dialect/Vector/IR/VectorOps.cpp.inc:14440:10
#<!-- -->15 0x0000602605a91f29 mlir::vector::ShuffleOp::parse(mlir::OpAsmParser&amp;, mlir::OperationState&amp;) /build_new/tools/mlir/include/mlir/Dialect/Vector/IR/VectorOps.cpp.inc:14407:22
#<!-- -->16 0x00006025f94bc13d llvm::ParseResult llvm::detail::UniqueFunctionBase&lt;llvm::ParseResult, mlir::OpAsmParser&amp;, mlir::OperationState&amp;&gt;::CallImpl&lt;llvm::ParseResult (*)(mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;(void*, mlir::OpAsmParser&amp;, mlir::OperationState&amp;) /llvm/include/llvm/ADT/FunctionExtras.h:212:12
#<!-- -->17 0x00006026053fe19f llvm::unique_function&lt;llvm::ParseResult (mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;::operator()(mlir::OpAsmParser&amp;, mlir::OperationState&amp;) /llvm/include/llvm/ADT/FunctionExtras.h:364:12
#<!-- -->18 0x00006026053fe145 llvm::ParseResult llvm::function_ref&lt;llvm::ParseResult (mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;::callback_fn&lt;llvm::unique_function&lt;llvm::ParseResult (mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;&gt;(long, mlir::OpAsmParser&amp;, mlir::OperationState&amp;) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->19 0x00006026053fe109 llvm::function_ref&lt;llvm::ParseResult (mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;::operator()(mlir::OpAsmParser&amp;, mlir::OperationState&amp;) const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->20 0x00006026053edc3e (anonymous namespace)::CustomOpAsmParser::parseOperation(mlir::OperationState&amp;) /mlir/lib/AsmParser/Parser.cpp:1653:9
#<!-- -->21 0x00006026053ec580 (anonymous namespace)::OperationParser::parseCustomOperation(llvm::ArrayRef&lt;std::tuple&lt;llvm::StringRef, unsigned int, llvm::SMLoc&gt;&gt;) /mlir/lib/AsmParser/Parser.cpp:2192:19
#<!-- -->22 0x00006026053e8713 (anonymous namespace)::OperationParser::parseOperation() /mlir/lib/AsmParser/Parser.cpp:1306:8
#<!-- -->23 0x00006026053f5902 (anonymous namespace)::OperationParser::parseBlockBody(mlir::Block*) /mlir/lib/AsmParser/Parser.cpp:2478:9
#<!-- -->24 0x00006026053f49ea (anonymous namespace)::OperationParser::parseBlock(mlir::Block*&amp;) /mlir/lib/AsmParser/Parser.cpp:2408:12
#<!-- -->25 0x00006026053f45c3 (anonymous namespace)::OperationParser::parseRegionBody(mlir::Region&amp;, llvm::SMLoc, llvm::ArrayRef&lt;mlir::OpAsmParser::Argument&gt;, bool) /mlir/lib/AsmParser/Parser.cpp:2366:7
#<!-- -->26 0x00006026053f1aa7 (anonymous namespace)::OperationParser::parseRegion(mlir::Region&amp;, llvm::ArrayRef&lt;mlir::OpAsmParser::Argument&gt;, bool) /mlir/lib/AsmParser/Parser.cpp:2295:7
#<!-- -->27 0x00006026053ef760 (anonymous namespace)::CustomOpAsmParser::parseRegion(mlir::Region&amp;, llvm::ArrayRef&lt;mlir::OpAsmParser::Argument&gt;, bool) /mlir/lib/AsmParser/Parser.cpp:1917:16
#<!-- -->28 0x00006026053ef844 (anonymous namespace)::CustomOpAsmParser::parseOptionalRegion(mlir::Region&amp;, llvm::ArrayRef&lt;mlir::OpAsmParser::Argument&gt;, bool) /mlir/lib/AsmParser/Parser.cpp:1928:12
#<!-- -->29 0x0000602607cf793c mlir::function_interface_impl::parseFunctionOp(mlir::OpAsmParser&amp;, mlir::OperationState&amp;, bool, mlir::StringAttr, llvm::function_ref&lt;mlir::Type (mlir::Builder&amp;, llvm::ArrayRef&lt;mlir::Type&gt;, llvm::ArrayRef&lt;mlir::Type&gt;, mlir::function_interface_impl::VariadicFlag, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;&amp;)&gt;, mlir::StringAttr, mlir::StringAttr) /mlir/lib/Interfaces/FunctionImplementation.cpp:154:14
#<!-- -->30 0x00006026059da52e mlir::func::FuncOp::parse(mlir::OpAsmParser&amp;, mlir::OperationState&amp;) /mlir/lib/Dialect/Func/IR/FuncOps.cpp:203:10
#<!-- -->31 0x00006025f94bc13d llvm::ParseResult llvm::detail::UniqueFunctionBase&lt;llvm::ParseResult, mlir::OpAsmParser&amp;, mlir::OperationState&amp;&gt;::CallImpl&lt;llvm::ParseResult (*)(mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;(void*, mlir::OpAsmParser&amp;, mlir::OperationState&amp;) /llvm/include/llvm/ADT/FunctionExtras.h:212:12
#<!-- -->32 0x00006026053fe19f llvm::unique_function&lt;llvm::ParseResult (mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;::operator()(mlir::OpAsmParser&amp;, mlir::OperationState&amp;) /llvm/include/llvm/ADT/FunctionExtras.h:364:12
#<!-- -->33 0x00006026053fe145 llvm::ParseResult llvm::function_ref&lt;llvm::ParseResult (mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;::callback_fn&lt;llvm::unique_function&lt;llvm::ParseResult (mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;&gt;(long, mlir::OpAsmParser&amp;, mlir::OperationState&amp;) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->34 0x00006026053fe109 llvm::function_ref&lt;llvm::ParseResult (mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;::operator()(mlir::OpAsmParser&amp;, mlir::OperationState&amp;) const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->35 0x00006026053edc3e (anonymous namespace)::CustomOpAsmParser::parseOperation(mlir::OperationState&amp;) /mlir/lib/AsmParser/Parser.cpp:1653:9
#<!-- -->36 0x00006026053ec580 (anonymous namespace)::OperationParser::parseCustomOperation(llvm::ArrayRef&lt;std::tuple&lt;llvm::StringRef, unsigned int, llvm::SMLoc&gt;&gt;) /mlir/lib/AsmParser/Parser.cpp:2192:19
#<!-- -->37 0x00006026053e8713 (anonymous namespace)::OperationParser::parseOperation() /mlir/lib/AsmParser/Parser.cpp:1306:8
#<!-- -->38 0x00006026053f5902 (anonymous namespace)::OperationParser::parseBlockBody(mlir::Block*) /mlir/lib/AsmParser/Parser.cpp:2478:9
#<!-- -->39 0x00006026053f49ea (anonymous namespace)::OperationParser::parseBlock(mlir::Block*&amp;) /mlir/lib/AsmParser/Parser.cpp:2408:12
#<!-- -->40 0x00006026053f45c3 (anonymous namespace)::OperationParser::parseRegionBody(mlir::Region&amp;, llvm::SMLoc, llvm::ArrayRef&lt;mlir::OpAsmParser::Argument&gt;, bool) /mlir/lib/AsmParser/Parser.cpp:2366:7
#<!-- -->41 0x00006026053f1aa7 (anonymous namespace)::OperationParser::parseRegion(mlir::Region&amp;, llvm::ArrayRef&lt;mlir::OpAsmParser::Argument&gt;, bool) /mlir/lib/AsmParser/Parser.cpp:2295:7
#<!-- -->42 0x00006026053ef760 (anonymous namespace)::CustomOpAsmParser::parseRegion(mlir::Region&amp;, llvm::ArrayRef&lt;mlir::OpAsmParser::Argument&gt;, bool) /mlir/lib/AsmParser/Parser.cpp:1917:16
#<!-- -->43 0x0000602607ef158d mlir::ModuleOp::parse(mlir::OpAsmParser&amp;, mlir::OperationState&amp;) /build_new/tools/mlir/include/mlir/IR/BuiltinOps.cpp.inc:307:14
#<!-- -->44 0x00006025f94bc13d llvm::ParseResult llvm::detail::UniqueFunctionBase&lt;llvm::ParseResult, mlir::OpAsmParser&amp;, mlir::OperationState&amp;&gt;::CallImpl&lt;llvm::ParseResult (*)(mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;(void*, mlir::OpAsmParser&amp;, mlir::OperationState&amp;) /llvm/include/llvm/ADT/FunctionExtras.h:212:12
#<!-- -->45 0x00006026053fe19f llvm::unique_function&lt;llvm::ParseResult (mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;::operator()(mlir::OpAsmParser&amp;, mlir::OperationState&amp;) /llvm/include/llvm/ADT/FunctionExtras.h:364:12
#<!-- -->46 0x00006026053fe145 llvm::ParseResult llvm::function_ref&lt;llvm::ParseResult (mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;::callback_fn&lt;llvm::unique_function&lt;llvm::ParseResult (mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;&gt;(long, mlir::OpAsmParser&amp;, mlir::OperationState&amp;) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->47 0x00006026053fe109 llvm::function_ref&lt;llvm::ParseResult (mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;::operator()(mlir::OpAsmParser&amp;, mlir::OperationState&amp;) const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->48 0x00006026053edc3e (anonymous namespace)::CustomOpAsmParser::parseOperation(mlir::OperationState&amp;) /mlir/lib/AsmParser/Parser.cpp:1653:9
#<!-- -->49 0x00006026053ec580 (anonymous namespace)::OperationParser::parseCustomOperation(llvm::ArrayRef&lt;std::tuple&lt;llvm::StringRef, unsigned int, llvm::SMLoc&gt;&gt;) /mlir/lib/AsmParser/Parser.cpp:2192:19
#<!-- -->50 0x00006026053e8713 (anonymous namespace)::OperationParser::parseOperation() /mlir/lib/AsmParser/Parser.cpp:1306:8
#<!-- -->51 0x00006026053e815d (anonymous namespace)::TopLevelOperationParser::parse(mlir::Block*, mlir::Location) /mlir/lib/AsmParser/Parser.cpp:2908:20
#<!-- -->52 0x00006026053e7ffe mlir::parseAsmSourceFile(llvm::SourceMgr const&amp;, mlir::Block*, mlir::ParserConfig const&amp;, mlir::AsmParserState*, mlir::AsmParserCodeCompleteContext*) /mlir/lib/AsmParser/Parser.cpp:2968:41
#<!-- -->53 0x00006026053b3d1f mlir::parseSourceFile(std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::Block*, mlir::ParserConfig const&amp;, mlir::LocationAttr*) /mlir/lib/Parser/Parser.cpp:64:10
#<!-- -->54 0x00006025f925a54f mlir::OwningOpRef&lt;mlir::ModuleOp&gt; mlir::detail::parseSourceFile&lt;mlir::ModuleOp, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;&gt;(mlir::ParserConfig const&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;) /mlir/include/mlir/Parser/Parser.h:158:14
#<!-- -->55 0x00006025f925a458 mlir::OwningOpRef&lt;mlir::ModuleOp&gt; mlir::parseSourceFile&lt;mlir::ModuleOp&gt;(std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::ParserConfig const&amp;) /mlir/include/mlir/Parser/Parser.h:188:10
#<!-- -->56 0x00006025f92596cd mlir::parseSourceFileForTool(std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::ParserConfig const&amp;, bool) /mlir/include/mlir/Tools/ParseUtilities.h:31:12
#<!-- -->57 0x00006025f924174a performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:528:16
#<!-- -->58 0x00006025f9241551 processBuffer(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&amp;, mlir::DialectRegistry&amp;, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:681:12
#<!-- -->59 0x00006025f924120c mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, llvm::raw_ostream&amp;) const /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:779:12
#<!-- -->60 0x00006025f9241116 llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->61 0x00006026080496fa llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->62 0x0000602608048aba mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) /mlir/lib/Support/ToolUtilities.cpp:30:12
#<!-- -->63 0x00006025f923cdc6 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:784:26
#<!-- -->64 0x00006025f923d1c8 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:830:14
#<!-- -->65 0x00006025f923d398 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:846:10
#<!-- -->66 0x00006025f910b8d5 main /mlir/tools/mlir-opt/mlir-opt.cpp:343:33
#<!-- -->67 0x000070f944c2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->68 0x000070f944c2a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->69 0x000070f944c2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->70 0x00006025f910b745 _start (./mlir-opt+0x481e745)
Aborted
```

#### Command for Reproduction
```bash
mlir-opt SobelFilter.mlir -gpu-kernel-outlining -lower-affine --convert-scf-to-cf --convert-gpu-to-nvvm -allow-unregistered-dialect
```
</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-mlir-vectorops

Author: kitty-apple (kitty-1998)

<details>
I discovered a crash bug while using MLIR to process programs. Here's a code example for reproduction:

Git version: da8d1b52d0d344068a8bd3e46a8af3a692d2efc2

```llvm
module {
  func.func @<!-- -->ComputeSobel_vectorized(%pixels: vector&lt;9xi8&gt;, %fScale: f32) -&gt; i8 attributes {dialect.test = #dialect.test&lt;"custom"&gt;} {
    %c0 = arith.constant 0 : index
    %c9 = arith.constant 9 : index
    %c1 = arith.constant 1 : index
    %mem = memref.alloca() : memref&lt;9xi8&gt;
    vector.store %pixels, %mem[%c0] : memref&lt;9xi8&gt;, vector&lt;9xi8&gt;
    %out = memref.alloca() : memref&lt;1xi8&gt;
    scf.parallel (%i) = (%c0) to (%c9) step (%c1) {
      %val = memref.load %mem[%i] : memref&lt;9xi8&gt;
      %next = arith.addi %val, %val : i8
      memref.store %next, %out[%c0] : memref&lt;1xi8&gt;
      scf.yield
    }
    %result = memref.load %out[%c0] : memref&lt;1xi8&gt;
    %shuffle = vector.shuffle %result, %result [0] : i8, i8
    return %shuffle : i8
  }
}
```


#### Stack Trace
```bash
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: mlir-opt SobelFilter.mlir -gpu-kernel-outlining -lower-affine --convert-scf-to-cf --convert-gpu-to-nvvm -allow-unregistered-dialect
1.	MLIR Parser: custom op parser 'builtin.module'
2.	MLIR Parser: custom op parser 'func.func'
3.	MLIR Parser: custom op parser 'vector.shuffle'
 #<!-- -->0 0x00006025f9136dbd llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /llvm/lib/Support/Unix/Signals.inc:880:11
 #<!-- -->1 0x00006025f91372eb PrintStackTraceSignalHandler(void*) /llvm/lib/Support/Unix/Signals.inc:962:1
 #<!-- -->2 0x00006025f9135184 llvm::sys::RunSignalHandlers() /llvm/lib/Support/Signals.cpp:108:5
 #<!-- -->3 0x00006025f9137a89 SignalHandler(int, siginfo_t*, void*) /llvm/lib/Support/Unix/Signals.inc:448:38
 #<!-- -->4 0x000070f944c45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->5 0x000070f944c9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->6 0x000070f944c9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->7 0x000070f944c9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->8 0x000070f944c4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x000070f944c288ff abort ./stdlib/abort.c:81:7
#<!-- -->10 0x000070f944c2881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->11 0x000070f944c3b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->12 0x00006025f9308504 decltype(auto) llvm::cast&lt;mlir::VectorType, mlir::Type&gt;(mlir::Type const&amp;) /llvm/include/llvm/Support/Casting.h:561:43
#<!-- -->13 0x0000602605a13207 mlir::vector::ShuffleOp::inferReturnTypes(mlir::MLIRContext*, std::optional&lt;mlir::Location&gt;, mlir::vector::ShuffleOpAdaptor, llvm::SmallVectorImpl&lt;mlir::Type&gt;&amp;) /mlir/lib/Dialect/Vector/IR/VectorOps.cpp:3278:17
#<!-- -->14 0x0000602605a8f1da mlir::vector::ShuffleOp::inferReturnTypes(mlir::MLIRContext*, std::optional&lt;mlir::Location&gt;, mlir::ValueRange, mlir::DictionaryAttr, mlir::OpaqueProperties, mlir::RegionRange, llvm::SmallVectorImpl&lt;mlir::Type&gt;&amp;) /build_new/tools/mlir/include/mlir/Dialect/Vector/IR/VectorOps.cpp.inc:14440:10
#<!-- -->15 0x0000602605a91f29 mlir::vector::ShuffleOp::parse(mlir::OpAsmParser&amp;, mlir::OperationState&amp;) /build_new/tools/mlir/include/mlir/Dialect/Vector/IR/VectorOps.cpp.inc:14407:22
#<!-- -->16 0x00006025f94bc13d llvm::ParseResult llvm::detail::UniqueFunctionBase&lt;llvm::ParseResult, mlir::OpAsmParser&amp;, mlir::OperationState&amp;&gt;::CallImpl&lt;llvm::ParseResult (*)(mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;(void*, mlir::OpAsmParser&amp;, mlir::OperationState&amp;) /llvm/include/llvm/ADT/FunctionExtras.h:212:12
#<!-- -->17 0x00006026053fe19f llvm::unique_function&lt;llvm::ParseResult (mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;::operator()(mlir::OpAsmParser&amp;, mlir::OperationState&amp;) /llvm/include/llvm/ADT/FunctionExtras.h:364:12
#<!-- -->18 0x00006026053fe145 llvm::ParseResult llvm::function_ref&lt;llvm::ParseResult (mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;::callback_fn&lt;llvm::unique_function&lt;llvm::ParseResult (mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;&gt;(long, mlir::OpAsmParser&amp;, mlir::OperationState&amp;) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->19 0x00006026053fe109 llvm::function_ref&lt;llvm::ParseResult (mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;::operator()(mlir::OpAsmParser&amp;, mlir::OperationState&amp;) const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->20 0x00006026053edc3e (anonymous namespace)::CustomOpAsmParser::parseOperation(mlir::OperationState&amp;) /mlir/lib/AsmParser/Parser.cpp:1653:9
#<!-- -->21 0x00006026053ec580 (anonymous namespace)::OperationParser::parseCustomOperation(llvm::ArrayRef&lt;std::tuple&lt;llvm::StringRef, unsigned int, llvm::SMLoc&gt;&gt;) /mlir/lib/AsmParser/Parser.cpp:2192:19
#<!-- -->22 0x00006026053e8713 (anonymous namespace)::OperationParser::parseOperation() /mlir/lib/AsmParser/Parser.cpp:1306:8
#<!-- -->23 0x00006026053f5902 (anonymous namespace)::OperationParser::parseBlockBody(mlir::Block*) /mlir/lib/AsmParser/Parser.cpp:2478:9
#<!-- -->24 0x00006026053f49ea (anonymous namespace)::OperationParser::parseBlock(mlir::Block*&amp;) /mlir/lib/AsmParser/Parser.cpp:2408:12
#<!-- -->25 0x00006026053f45c3 (anonymous namespace)::OperationParser::parseRegionBody(mlir::Region&amp;, llvm::SMLoc, llvm::ArrayRef&lt;mlir::OpAsmParser::Argument&gt;, bool) /mlir/lib/AsmParser/Parser.cpp:2366:7
#<!-- -->26 0x00006026053f1aa7 (anonymous namespace)::OperationParser::parseRegion(mlir::Region&amp;, llvm::ArrayRef&lt;mlir::OpAsmParser::Argument&gt;, bool) /mlir/lib/AsmParser/Parser.cpp:2295:7
#<!-- -->27 0x00006026053ef760 (anonymous namespace)::CustomOpAsmParser::parseRegion(mlir::Region&amp;, llvm::ArrayRef&lt;mlir::OpAsmParser::Argument&gt;, bool) /mlir/lib/AsmParser/Parser.cpp:1917:16
#<!-- -->28 0x00006026053ef844 (anonymous namespace)::CustomOpAsmParser::parseOptionalRegion(mlir::Region&amp;, llvm::ArrayRef&lt;mlir::OpAsmParser::Argument&gt;, bool) /mlir/lib/AsmParser/Parser.cpp:1928:12
#<!-- -->29 0x0000602607cf793c mlir::function_interface_impl::parseFunctionOp(mlir::OpAsmParser&amp;, mlir::OperationState&amp;, bool, mlir::StringAttr, llvm::function_ref&lt;mlir::Type (mlir::Builder&amp;, llvm::ArrayRef&lt;mlir::Type&gt;, llvm::ArrayRef&lt;mlir::Type&gt;, mlir::function_interface_impl::VariadicFlag, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;&amp;)&gt;, mlir::StringAttr, mlir::StringAttr) /mlir/lib/Interfaces/FunctionImplementation.cpp:154:14
#<!-- -->30 0x00006026059da52e mlir::func::FuncOp::parse(mlir::OpAsmParser&amp;, mlir::OperationState&amp;) /mlir/lib/Dialect/Func/IR/FuncOps.cpp:203:10
#<!-- -->31 0x00006025f94bc13d llvm::ParseResult llvm::detail::UniqueFunctionBase&lt;llvm::ParseResult, mlir::OpAsmParser&amp;, mlir::OperationState&amp;&gt;::CallImpl&lt;llvm::ParseResult (*)(mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;(void*, mlir::OpAsmParser&amp;, mlir::OperationState&amp;) /llvm/include/llvm/ADT/FunctionExtras.h:212:12
#<!-- -->32 0x00006026053fe19f llvm::unique_function&lt;llvm::ParseResult (mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;::operator()(mlir::OpAsmParser&amp;, mlir::OperationState&amp;) /llvm/include/llvm/ADT/FunctionExtras.h:364:12
#<!-- -->33 0x00006026053fe145 llvm::ParseResult llvm::function_ref&lt;llvm::ParseResult (mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;::callback_fn&lt;llvm::unique_function&lt;llvm::ParseResult (mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;&gt;(long, mlir::OpAsmParser&amp;, mlir::OperationState&amp;) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->34 0x00006026053fe109 llvm::function_ref&lt;llvm::ParseResult (mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;::operator()(mlir::OpAsmParser&amp;, mlir::OperationState&amp;) const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->35 0x00006026053edc3e (anonymous namespace)::CustomOpAsmParser::parseOperation(mlir::OperationState&amp;) /mlir/lib/AsmParser/Parser.cpp:1653:9
#<!-- -->36 0x00006026053ec580 (anonymous namespace)::OperationParser::parseCustomOperation(llvm::ArrayRef&lt;std::tuple&lt;llvm::StringRef, unsigned int, llvm::SMLoc&gt;&gt;) /mlir/lib/AsmParser/Parser.cpp:2192:19
#<!-- -->37 0x00006026053e8713 (anonymous namespace)::OperationParser::parseOperation() /mlir/lib/AsmParser/Parser.cpp:1306:8
#<!-- -->38 0x00006026053f5902 (anonymous namespace)::OperationParser::parseBlockBody(mlir::Block*) /mlir/lib/AsmParser/Parser.cpp:2478:9
#<!-- -->39 0x00006026053f49ea (anonymous namespace)::OperationParser::parseBlock(mlir::Block*&amp;) /mlir/lib/AsmParser/Parser.cpp:2408:12
#<!-- -->40 0x00006026053f45c3 (anonymous namespace)::OperationParser::parseRegionBody(mlir::Region&amp;, llvm::SMLoc, llvm::ArrayRef&lt;mlir::OpAsmParser::Argument&gt;, bool) /mlir/lib/AsmParser/Parser.cpp:2366:7
#<!-- -->41 0x00006026053f1aa7 (anonymous namespace)::OperationParser::parseRegion(mlir::Region&amp;, llvm::ArrayRef&lt;mlir::OpAsmParser::Argument&gt;, bool) /mlir/lib/AsmParser/Parser.cpp:2295:7
#<!-- -->42 0x00006026053ef760 (anonymous namespace)::CustomOpAsmParser::parseRegion(mlir::Region&amp;, llvm::ArrayRef&lt;mlir::OpAsmParser::Argument&gt;, bool) /mlir/lib/AsmParser/Parser.cpp:1917:16
#<!-- -->43 0x0000602607ef158d mlir::ModuleOp::parse(mlir::OpAsmParser&amp;, mlir::OperationState&amp;) /build_new/tools/mlir/include/mlir/IR/BuiltinOps.cpp.inc:307:14
#<!-- -->44 0x00006025f94bc13d llvm::ParseResult llvm::detail::UniqueFunctionBase&lt;llvm::ParseResult, mlir::OpAsmParser&amp;, mlir::OperationState&amp;&gt;::CallImpl&lt;llvm::ParseResult (*)(mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;(void*, mlir::OpAsmParser&amp;, mlir::OperationState&amp;) /llvm/include/llvm/ADT/FunctionExtras.h:212:12
#<!-- -->45 0x00006026053fe19f llvm::unique_function&lt;llvm::ParseResult (mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;::operator()(mlir::OpAsmParser&amp;, mlir::OperationState&amp;) /llvm/include/llvm/ADT/FunctionExtras.h:364:12
#<!-- -->46 0x00006026053fe145 llvm::ParseResult llvm::function_ref&lt;llvm::ParseResult (mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;::callback_fn&lt;llvm::unique_function&lt;llvm::ParseResult (mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;&gt;(long, mlir::OpAsmParser&amp;, mlir::OperationState&amp;) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->47 0x00006026053fe109 llvm::function_ref&lt;llvm::ParseResult (mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;::operator()(mlir::OpAsmParser&amp;, mlir::OperationState&amp;) const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->48 0x00006026053edc3e (anonymous namespace)::CustomOpAsmParser::parseOperation(mlir::OperationState&amp;) /mlir/lib/AsmParser/Parser.cpp:1653:9
#<!-- -->49 0x00006026053ec580 (anonymous namespace)::OperationParser::parseCustomOperation(llvm::ArrayRef&lt;std::tuple&lt;llvm::StringRef, unsigned int, llvm::SMLoc&gt;&gt;) /mlir/lib/AsmParser/Parser.cpp:2192:19
#<!-- -->50 0x00006026053e8713 (anonymous namespace)::OperationParser::parseOperation() /mlir/lib/AsmParser/Parser.cpp:1306:8
#<!-- -->51 0x00006026053e815d (anonymous namespace)::TopLevelOperationParser::parse(mlir::Block*, mlir::Location) /mlir/lib/AsmParser/Parser.cpp:2908:20
#<!-- -->52 0x00006026053e7ffe mlir::parseAsmSourceFile(llvm::SourceMgr const&amp;, mlir::Block*, mlir::ParserConfig const&amp;, mlir::AsmParserState*, mlir::AsmParserCodeCompleteContext*) /mlir/lib/AsmParser/Parser.cpp:2968:41
#<!-- -->53 0x00006026053b3d1f mlir::parseSourceFile(std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::Block*, mlir::ParserConfig const&amp;, mlir::LocationAttr*) /mlir/lib/Parser/Parser.cpp:64:10
#<!-- -->54 0x00006025f925a54f mlir::OwningOpRef&lt;mlir::ModuleOp&gt; mlir::detail::parseSourceFile&lt;mlir::ModuleOp, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;&gt;(mlir::ParserConfig const&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;) /mlir/include/mlir/Parser/Parser.h:158:14
#<!-- -->55 0x00006025f925a458 mlir::OwningOpRef&lt;mlir::ModuleOp&gt; mlir::parseSourceFile&lt;mlir::ModuleOp&gt;(std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::ParserConfig const&amp;) /mlir/include/mlir/Parser/Parser.h:188:10
#<!-- -->56 0x00006025f92596cd mlir::parseSourceFileForTool(std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::ParserConfig const&amp;, bool) /mlir/include/mlir/Tools/ParseUtilities.h:31:12
#<!-- -->57 0x00006025f924174a performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:528:16
#<!-- -->58 0x00006025f9241551 processBuffer(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&amp;, mlir::DialectRegistry&amp;, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:681:12
#<!-- -->59 0x00006025f924120c mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, llvm::raw_ostream&amp;) const /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:779:12
#<!-- -->60 0x00006025f9241116 llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->61 0x00006026080496fa llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->62 0x0000602608048aba mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) /mlir/lib/Support/ToolUtilities.cpp:30:12
#<!-- -->63 0x00006025f923cdc6 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:784:26
#<!-- -->64 0x00006025f923d1c8 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:830:14
#<!-- -->65 0x00006025f923d398 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:846:10
#<!-- -->66 0x00006025f910b8d5 main /mlir/tools/mlir-opt/mlir-opt.cpp:343:33
#<!-- -->67 0x000070f944c2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->68 0x000070f944c2a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->69 0x000070f944c2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->70 0x00006025f910b745 _start (./mlir-opt+0x481e745)
Aborted
```

#### Command for Reproduction
```bash
mlir-opt SobelFilter.mlir -gpu-kernel-outlining -lower-affine --convert-scf-to-cf --convert-gpu-to-nvvm -allow-unregistered-dialect
```
</details>


---

