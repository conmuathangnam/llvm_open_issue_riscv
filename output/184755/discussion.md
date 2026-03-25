# [MLIR] Crash during custom operation creation in a gpu program

**Author:** LynnBroe
**URL:** https://github.com/llvm/llvm-project/issues/184755
**Status:** Closed
**Labels:** mlir, crash
**Closed Date:** 2026-03-05T13:40:07Z

## Body

Hi, I encountered a crash in MLIR while processing an input program. See the following code example:

Git version: 47766d7f8c397da857d9f78db36c71e8a2ca1fbf

```llvm
module {
  "pdl_interp"() ({
    "pdl_interp.create_operation"(%operands, %attributes, %types) {opName = "arith.constant"} : (!pdl.range<value>, !pdl.range<attribute>, !pdl.range<type>) -> !pdl.operation
  }) : () -> ()

  func.func @mixed_dialect_sgemv(%arg0: tensor<?xf32>, %arg1: tensor<?xf32>) -> tensor<?xf32> {
    %c0 = arith.constant 0 : index
    %c4 = arith.constant 4 : index
    %c32 = arith.constant 32 : index
    %M = tensor.dim %arg0, %c0 : tensor<?xf32>
    %N = tensor.dim %arg1, %c0 : tensor<?xf32>
    %init = linalg.init_tensor [%M] : tensor<?xf32>
    %result = linalg.generic {indexing_maps = [affine_map<(d0, d1) -> (d0, d1)>, affine_map<(d0, d1) -> (d1)>, affine_map<(d0, d1) -> (d0)>], iterator_types = ["parallel", "reduction"]}
        ins(%arg0, %arg1 : tensor<?xf32>, tensor<?xf32>) outs(%init : tensor<?xf32>) {
      ^bb0(%a: f32, %x: f32, %y: f32):
        %prod = arith.mulf %a, %x : f32
        %sum = arith.addf %y, %prod : f32
        linalg.yield %sum : f32
    } -> tensor<?xf32>
    %updated = scf.for %i = %c0 to %M step %c4 iter_args(%out = %result) -> (tensor<?xf32>) {
      %slice = tensor.extract_slice %out[%i] [%c4] [1] : tensor<?xf32> to tensor<?xf32>
      %add_vec = vector.broadcast %c0 : index to vector<4xindex>
      %new_slice = vector.add %slice, %add_vec : tensor<?xf32>
      %inserted = tensor.insert_slice %new_slice into %out[%i] [%c4] [1] : tensor<?xf32> into tensor<?xf32>
      scf.yield %inserted : tensor<?xf32>
    }
  }
}
```

### Stack Trace
```bash
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: ./mlir-opt spgemv.mlir
1.	MLIR Parser: custom op parser 'builtin.module'
 #0 0x000063b4d2d8fbed llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /llvm/lib/Support/Unix/Signals.inc:880:11
 #1 0x000063b4d2d9011b PrintStackTraceSignalHandler(void*) /llvm/lib/Support/Unix/Signals.inc:962:1
 #2 0x000063b4d2d8dfb4 llvm::sys::RunSignalHandlers() /llvm/lib/Support/Signals.cpp:108:5
 #3 0x000063b4d2d908b9 SignalHandler(int, siginfo_t*, void*) /llvm/lib/Support/Unix/Signals.inc:448:38
 #4 0x000079d5da245330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #5 0x000079d5da29eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #6 0x000079d5da29eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #7 0x000079d5da29eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #8 0x000079d5da24527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #9 0x000079d5da2288ff abort ./stdlib/abort.c:81:7
#10 0x000063b4d2d68985 llvm::report_fatal_error(llvm::Twine const&, bool) /llvm/lib/Support/ErrorHandling.cpp:137:5
#11 0x000063b4e1b97e62 mlir::Operation::Operation(mlir::Location, mlir::OperationName, unsigned int, unsigned int, unsigned int, int, mlir::DictionaryAttr, mlir::OpaqueProperties, bool) /mlir/lib/IR/Operation.cpp:173:7
#12 0x000063b4e1b976eb mlir::Operation::create(mlir::Location, mlir::OperationName, mlir::TypeRange, mlir::ValueRange, mlir::DictionaryAttr, mlir::OpaqueProperties, mlir::BlockRange, unsigned int) /mlir/lib/IR/Operation.cpp:118:34
#13 0x000063b4e1b97496 mlir::Operation::create(mlir::Location, mlir::OperationName, mlir::TypeRange, mlir::ValueRange, mlir::NamedAttrList&&, mlir::OpaqueProperties, mlir::BlockRange, unsigned int) /mlir/lib/IR/Operation.cpp:75:3
#14 0x000063b4e1b9720b mlir::Operation::create(mlir::Location, mlir::OperationName, mlir::TypeRange, mlir::ValueRange, mlir::NamedAttrList&&, mlir::OpaqueProperties, mlir::BlockRange, mlir::RegionRange) /mlir/lib/IR/Operation.cpp:57:14
#15 0x000063b4e1b96fde mlir::Operation::create(mlir::OperationState const&) /mlir/lib/IR/Operation.cpp:36:7
#16 0x000063b4e1a27b91 mlir::OpBuilder::create(mlir::OperationState const&) /mlir/lib/IR/Builders.cpp:462:17
#17 0x000063b4defca843 (anonymous namespace)::OperationParser::parseGenericOperation() /mlir/lib/AsmParser/Parser.cpp:1600:14
#18 0x000063b4defc61e2 (anonymous namespace)::OperationParser::parseOperation() /mlir/lib/AsmParser/Parser.cpp:1308:8
#19 0x000063b4defd33b2 (anonymous namespace)::OperationParser::parseBlockBody(mlir::Block*) /mlir/lib/AsmParser/Parser.cpp:2478:9
#20 0x000063b4defd249a (anonymous namespace)::OperationParser::parseBlock(mlir::Block*&) /mlir/lib/AsmParser/Parser.cpp:2408:12
#21 0x000063b4defd2073 (anonymous namespace)::OperationParser::parseRegionBody(mlir::Region&, llvm::SMLoc, llvm::ArrayRef<mlir::OpAsmParser::Argument>, bool) /mlir/lib/AsmParser/Parser.cpp:2366:7
#22 0x000063b4defcf557 (anonymous namespace)::OperationParser::parseRegion(mlir::Region&, llvm::ArrayRef<mlir::OpAsmParser::Argument>, bool) /mlir/lib/AsmParser/Parser.cpp:2295:7
#23 0x000063b4defcd200 (anonymous namespace)::CustomOpAsmParser::parseRegion(mlir::Region&, llvm::ArrayRef<mlir::OpAsmParser::Argument>, bool) /mlir/lib/AsmParser/Parser.cpp:1917:16
#24 0x000063b4e1abe1bd mlir::ModuleOp::parse(mlir::OpAsmParser&, mlir::OperationState&) /build/tools/mlir/include/mlir/IR/BuiltinOps.cpp.inc:307:14
#25 0x000063b4d3113a6d llvm::ParseResult llvm::detail::UniqueFunctionBase<llvm::ParseResult, mlir::OpAsmParser&, mlir::OperationState&>::CallImpl<llvm::ParseResult (*)(mlir::OpAsmParser&, mlir::OperationState&)>(void*, mlir::OpAsmParser&, mlir::OperationState&) /llvm/include/llvm/ADT/FunctionExtras.h:212:12
#26 0x000063b4defdbc4f llvm::unique_function<llvm::ParseResult (mlir::OpAsmParser&, mlir::OperationState&)>::operator()(mlir::OpAsmParser&, mlir::OperationState&) /llvm/include/llvm/ADT/FunctionExtras.h:364:12
#27 0x000063b4defdbbf5 llvm::ParseResult llvm::function_ref<llvm::ParseResult (mlir::OpAsmParser&, mlir::OperationState&)>::callback_fn<llvm::unique_function<llvm::ParseResult (mlir::OpAsmParser&, mlir::OperationState&)>>(long, mlir::OpAsmParser&, mlir::OperationState&) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#28 0x000063b4defdbbb9 llvm::function_ref<llvm::ParseResult (mlir::OpAsmParser&, mlir::OperationState&)>::operator()(mlir::OpAsmParser&, mlir::OperationState&) const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#29 0x000063b4defcb6de (anonymous namespace)::CustomOpAsmParser::parseOperation(mlir::OperationState&) /mlir/lib/AsmParser/Parser.cpp:1653:9
#30 0x000063b4defca020 (anonymous namespace)::OperationParser::parseCustomOperation(llvm::ArrayRef<std::tuple<llvm::StringRef, unsigned int, llvm::SMLoc>>) /mlir/lib/AsmParser/Parser.cpp:2192:19
#31 0x000063b4defc61b3 (anonymous namespace)::OperationParser::parseOperation() /mlir/lib/AsmParser/Parser.cpp:1306:8
#32 0x000063b4defc5bfd (anonymous namespace)::TopLevelOperationParser::parse(mlir::Block*, mlir::Location) /mlir/lib/AsmParser/Parser.cpp:2908:20
#33 0x000063b4defc5a9e mlir::parseAsmSourceFile(llvm::SourceMgr const&, mlir::Block*, mlir::ParserConfig const&, mlir::AsmParserState*, mlir::AsmParserCodeCompleteContext*) /mlir/lib/AsmParser/Parser.cpp:2968:41
#34 0x000063b4def917ef mlir::parseSourceFile(std::shared_ptr<llvm::SourceMgr> const&, mlir::Block*, mlir::ParserConfig const&, mlir::LocationAttr*) /mlir/lib/Parser/Parser.cpp:64:10
#35 0x000063b4d2eb326f mlir::OwningOpRef<mlir::ModuleOp> mlir::detail::parseSourceFile<mlir::ModuleOp, std::shared_ptr<llvm::SourceMgr> const&>(mlir::ParserConfig const&, std::shared_ptr<llvm::SourceMgr> const&) /mlir/include/mlir/Parser/Parser.h:158:14
#36 0x000063b4d2eb3178 mlir::OwningOpRef<mlir::ModuleOp> mlir::parseSourceFile<mlir::ModuleOp>(std::shared_ptr<llvm::SourceMgr> const&, mlir::ParserConfig const&) /mlir/include/mlir/Parser/Parser.h:188:10
#37 0x000063b4d2eb23ed mlir::parseSourceFileForTool(std::shared_ptr<llvm::SourceMgr> const&, mlir::ParserConfig const&, bool) /mlir/include/mlir/Tools/ParseUtilities.h:31:12
#38 0x000063b4d2e9a46a performActions(llvm::raw_ostream&, std::shared_ptr<llvm::SourceMgr> const&, mlir::MLIRContext*, mlir::MlirOptMainConfig const&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:528:16
#39 0x000063b4d2e9a271 processBuffer(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&, mlir::DialectRegistry&, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:681:12
#40 0x000063b4d2e99f2c mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, llvm::raw_ostream&) const /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:779:12
#41 0x000063b4d2e99e36 llvm::LogicalResult llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::callback_fn<mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#42 0x000063b4e1c159da llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#43 0x000063b4e1c14d9a mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef) /mlir/lib/Support/ToolUtilities.cpp:30:12
#44 0x000063b4d2e95ae6 mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:784:26
#45 0x000063b4d2e95ee8 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:830:14
#46 0x000063b4d2e960b8 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:846:10
#47 0x000063b4d2d647d5 main /mlir/tools/mlir-opt/mlir-opt.cpp:343:33
#48 0x000079d5da22a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#49 0x000079d5da22a28b call_init ./csu/../csu/libc-start.c:128:20
#50 0x000079d5da22a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#51 0x000063b4d2d64645 _start (./mlir-opt+0x47e6645)
Aborted
```

Used command: `mlir-opt spgemv.mlir`

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: KHSDev (LynnBroe)

<details>
Hi, I encountered a crash in MLIR while processing an input program. See the following code example:

Git version: 47766d7f8c397da857d9f78db36c71e8a2ca1fbf

```llvm
module {
  "pdl_interp"() ({
    "pdl_interp.create_operation"(%operands, %attributes, %types) {opName = "arith.constant"} : (!pdl.range&lt;value&gt;, !pdl.range&lt;attribute&gt;, !pdl.range&lt;type&gt;) -&gt; !pdl.operation
  }) : () -&gt; ()

  func.func @<!-- -->mixed_dialect_sgemv(%arg0: tensor&lt;?xf32&gt;, %arg1: tensor&lt;?xf32&gt;) -&gt; tensor&lt;?xf32&gt; {
    %c0 = arith.constant 0 : index
    %c4 = arith.constant 4 : index
    %c32 = arith.constant 32 : index
    %M = tensor.dim %arg0, %c0 : tensor&lt;?xf32&gt;
    %N = tensor.dim %arg1, %c0 : tensor&lt;?xf32&gt;
    %init = linalg.init_tensor [%M] : tensor&lt;?xf32&gt;
    %result = linalg.generic {indexing_maps = [affine_map&lt;(d0, d1) -&gt; (d0, d1)&gt;, affine_map&lt;(d0, d1) -&gt; (d1)&gt;, affine_map&lt;(d0, d1) -&gt; (d0)&gt;], iterator_types = ["parallel", "reduction"]}
        ins(%arg0, %arg1 : tensor&lt;?xf32&gt;, tensor&lt;?xf32&gt;) outs(%init : tensor&lt;?xf32&gt;) {
      ^bb0(%a: f32, %x: f32, %y: f32):
        %prod = arith.mulf %a, %x : f32
        %sum = arith.addf %y, %prod : f32
        linalg.yield %sum : f32
    } -&gt; tensor&lt;?xf32&gt;
    %updated = scf.for %i = %c0 to %M step %c4 iter_args(%out = %result) -&gt; (tensor&lt;?xf32&gt;) {
      %slice = tensor.extract_slice %out[%i] [%c4] [1] : tensor&lt;?xf32&gt; to tensor&lt;?xf32&gt;
      %add_vec = vector.broadcast %c0 : index to vector&lt;4xindex&gt;
      %new_slice = vector.add %slice, %add_vec : tensor&lt;?xf32&gt;
      %inserted = tensor.insert_slice %new_slice into %out[%i] [%c4] [1] : tensor&lt;?xf32&gt; into tensor&lt;?xf32&gt;
      scf.yield %inserted : tensor&lt;?xf32&gt;
    }
  }
}
```

### Stack Trace
```bash
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: ./mlir-opt spgemv.mlir
1.	MLIR Parser: custom op parser 'builtin.module'
 #<!-- -->0 0x000063b4d2d8fbed llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /llvm/lib/Support/Unix/Signals.inc:880:11
 #<!-- -->1 0x000063b4d2d9011b PrintStackTraceSignalHandler(void*) /llvm/lib/Support/Unix/Signals.inc:962:1
 #<!-- -->2 0x000063b4d2d8dfb4 llvm::sys::RunSignalHandlers() /llvm/lib/Support/Signals.cpp:108:5
 #<!-- -->3 0x000063b4d2d908b9 SignalHandler(int, siginfo_t*, void*) /llvm/lib/Support/Unix/Signals.inc:448:38
 #<!-- -->4 0x000079d5da245330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->5 0x000079d5da29eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->6 0x000079d5da29eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->7 0x000079d5da29eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->8 0x000079d5da24527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x000079d5da2288ff abort ./stdlib/abort.c:81:7
#<!-- -->10 0x000063b4d2d68985 llvm::report_fatal_error(llvm::Twine const&amp;, bool) /llvm/lib/Support/ErrorHandling.cpp:137:5
#<!-- -->11 0x000063b4e1b97e62 mlir::Operation::Operation(mlir::Location, mlir::OperationName, unsigned int, unsigned int, unsigned int, int, mlir::DictionaryAttr, mlir::OpaqueProperties, bool) /mlir/lib/IR/Operation.cpp:173:7
#<!-- -->12 0x000063b4e1b976eb mlir::Operation::create(mlir::Location, mlir::OperationName, mlir::TypeRange, mlir::ValueRange, mlir::DictionaryAttr, mlir::OpaqueProperties, mlir::BlockRange, unsigned int) /mlir/lib/IR/Operation.cpp:118:34
#<!-- -->13 0x000063b4e1b97496 mlir::Operation::create(mlir::Location, mlir::OperationName, mlir::TypeRange, mlir::ValueRange, mlir::NamedAttrList&amp;&amp;, mlir::OpaqueProperties, mlir::BlockRange, unsigned int) /mlir/lib/IR/Operation.cpp:75:3
#<!-- -->14 0x000063b4e1b9720b mlir::Operation::create(mlir::Location, mlir::OperationName, mlir::TypeRange, mlir::ValueRange, mlir::NamedAttrList&amp;&amp;, mlir::OpaqueProperties, mlir::BlockRange, mlir::RegionRange) /mlir/lib/IR/Operation.cpp:57:14
#<!-- -->15 0x000063b4e1b96fde mlir::Operation::create(mlir::OperationState const&amp;) /mlir/lib/IR/Operation.cpp:36:7
#<!-- -->16 0x000063b4e1a27b91 mlir::OpBuilder::create(mlir::OperationState const&amp;) /mlir/lib/IR/Builders.cpp:462:17
#<!-- -->17 0x000063b4defca843 (anonymous namespace)::OperationParser::parseGenericOperation() /mlir/lib/AsmParser/Parser.cpp:1600:14
#<!-- -->18 0x000063b4defc61e2 (anonymous namespace)::OperationParser::parseOperation() /mlir/lib/AsmParser/Parser.cpp:1308:8
#<!-- -->19 0x000063b4defd33b2 (anonymous namespace)::OperationParser::parseBlockBody(mlir::Block*) /mlir/lib/AsmParser/Parser.cpp:2478:9
#<!-- -->20 0x000063b4defd249a (anonymous namespace)::OperationParser::parseBlock(mlir::Block*&amp;) /mlir/lib/AsmParser/Parser.cpp:2408:12
#<!-- -->21 0x000063b4defd2073 (anonymous namespace)::OperationParser::parseRegionBody(mlir::Region&amp;, llvm::SMLoc, llvm::ArrayRef&lt;mlir::OpAsmParser::Argument&gt;, bool) /mlir/lib/AsmParser/Parser.cpp:2366:7
#<!-- -->22 0x000063b4defcf557 (anonymous namespace)::OperationParser::parseRegion(mlir::Region&amp;, llvm::ArrayRef&lt;mlir::OpAsmParser::Argument&gt;, bool) /mlir/lib/AsmParser/Parser.cpp:2295:7
#<!-- -->23 0x000063b4defcd200 (anonymous namespace)::CustomOpAsmParser::parseRegion(mlir::Region&amp;, llvm::ArrayRef&lt;mlir::OpAsmParser::Argument&gt;, bool) /mlir/lib/AsmParser/Parser.cpp:1917:16
#<!-- -->24 0x000063b4e1abe1bd mlir::ModuleOp::parse(mlir::OpAsmParser&amp;, mlir::OperationState&amp;) /build/tools/mlir/include/mlir/IR/BuiltinOps.cpp.inc:307:14
#<!-- -->25 0x000063b4d3113a6d llvm::ParseResult llvm::detail::UniqueFunctionBase&lt;llvm::ParseResult, mlir::OpAsmParser&amp;, mlir::OperationState&amp;&gt;::CallImpl&lt;llvm::ParseResult (*)(mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;(void*, mlir::OpAsmParser&amp;, mlir::OperationState&amp;) /llvm/include/llvm/ADT/FunctionExtras.h:212:12
#<!-- -->26 0x000063b4defdbc4f llvm::unique_function&lt;llvm::ParseResult (mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;::operator()(mlir::OpAsmParser&amp;, mlir::OperationState&amp;) /llvm/include/llvm/ADT/FunctionExtras.h:364:12
#<!-- -->27 0x000063b4defdbbf5 llvm::ParseResult llvm::function_ref&lt;llvm::ParseResult (mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;::callback_fn&lt;llvm::unique_function&lt;llvm::ParseResult (mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;&gt;(long, mlir::OpAsmParser&amp;, mlir::OperationState&amp;) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->28 0x000063b4defdbbb9 llvm::function_ref&lt;llvm::ParseResult (mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;::operator()(mlir::OpAsmParser&amp;, mlir::OperationState&amp;) const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->29 0x000063b4defcb6de (anonymous namespace)::CustomOpAsmParser::parseOperation(mlir::OperationState&amp;) /mlir/lib/AsmParser/Parser.cpp:1653:9
#<!-- -->30 0x000063b4defca020 (anonymous namespace)::OperationParser::parseCustomOperation(llvm::ArrayRef&lt;std::tuple&lt;llvm::StringRef, unsigned int, llvm::SMLoc&gt;&gt;) /mlir/lib/AsmParser/Parser.cpp:2192:19
#<!-- -->31 0x000063b4defc61b3 (anonymous namespace)::OperationParser::parseOperation() /mlir/lib/AsmParser/Parser.cpp:1306:8
#<!-- -->32 0x000063b4defc5bfd (anonymous namespace)::TopLevelOperationParser::parse(mlir::Block*, mlir::Location) /mlir/lib/AsmParser/Parser.cpp:2908:20
#<!-- -->33 0x000063b4defc5a9e mlir::parseAsmSourceFile(llvm::SourceMgr const&amp;, mlir::Block*, mlir::ParserConfig const&amp;, mlir::AsmParserState*, mlir::AsmParserCodeCompleteContext*) /mlir/lib/AsmParser/Parser.cpp:2968:41
#<!-- -->34 0x000063b4def917ef mlir::parseSourceFile(std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::Block*, mlir::ParserConfig const&amp;, mlir::LocationAttr*) /mlir/lib/Parser/Parser.cpp:64:10
#<!-- -->35 0x000063b4d2eb326f mlir::OwningOpRef&lt;mlir::ModuleOp&gt; mlir::detail::parseSourceFile&lt;mlir::ModuleOp, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;&gt;(mlir::ParserConfig const&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;) /mlir/include/mlir/Parser/Parser.h:158:14
#<!-- -->36 0x000063b4d2eb3178 mlir::OwningOpRef&lt;mlir::ModuleOp&gt; mlir::parseSourceFile&lt;mlir::ModuleOp&gt;(std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::ParserConfig const&amp;) /mlir/include/mlir/Parser/Parser.h:188:10
#<!-- -->37 0x000063b4d2eb23ed mlir::parseSourceFileForTool(std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::ParserConfig const&amp;, bool) /mlir/include/mlir/Tools/ParseUtilities.h:31:12
#<!-- -->38 0x000063b4d2e9a46a performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:528:16
#<!-- -->39 0x000063b4d2e9a271 processBuffer(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&amp;, mlir::DialectRegistry&amp;, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:681:12
#<!-- -->40 0x000063b4d2e99f2c mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, llvm::raw_ostream&amp;) const /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:779:12
#<!-- -->41 0x000063b4d2e99e36 llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->42 0x000063b4e1c159da llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->43 0x000063b4e1c14d9a mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) /mlir/lib/Support/ToolUtilities.cpp:30:12
#<!-- -->44 0x000063b4d2e95ae6 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:784:26
#<!-- -->45 0x000063b4d2e95ee8 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:830:14
#<!-- -->46 0x000063b4d2e960b8 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:846:10
#<!-- -->47 0x000063b4d2d647d5 main /mlir/tools/mlir-opt/mlir-opt.cpp:343:33
#<!-- -->48 0x000079d5da22a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->49 0x000079d5da22a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->50 0x000079d5da22a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->51 0x000063b4d2d64645 _start (./mlir-opt+0x47e6645)
Aborted
```

Used command: `mlir-opt spgemv.mlir`
</details>


---

### Comment 2 - CoTinker

Add `-allow-unregistered-dialect` option.

---

### Comment 3 - joker-eph

Closing, the full error message should be explicit about what to do here?

---

