# [MLIR] `gpu.launch` custom parser crash (invalid types?)

**Author:** RJerrica
**URL:** https://github.com/llvm/llvm-project/issues/176530
**Status:** Closed
**Labels:** mlir:gpu, crash
**Closed Date:** 2026-01-22T21:58:05Z

## Body

Hi, I ran into a crash while building a program using the GPU Dialect. Please see the program below for details.

### Input Program
```llvm
module attributes {gpu.container_module} {
  gpu.module @volumemodule {
    func.func @main() {
      %0 = gpu.launch blocks(%bx, %by, %bz) in (%gx = %c1, %gy = %c2, %gz = %c1) threads(%tx, %ty, %tz) in (%lx = %c4, %ly = %c5, %lz = %c1) {
        gpu.barrier
        gpu.terminator
      }
      return
    }
    func.func private @constant() -> (i32, i32, i32, i32, i32, i32) attributes {gpu.kernel} {
      %c1 = arith.constant 1 : i32
      %c2 = arith.constant 2 : i32
      %c4 = arith.constant 4 : i32
      %c5 = arith.constant 5 : i32
      return %c1, %c2, %c1, %c4, %c5, %c1 : i32, i32, i32, i32, i32, i32
    }
  }
}
```

### Stack Trace
```bash
mlir-opt: /projects/llvm-project/mlir/lib/IR/TypeRange.cpp:20: mlir::TypeRange::TypeRange(ArrayRef<Type>): Assertion `llvm::all_of(types, [](Type t) { return t; }) && "attempting to construct a TypeRange with null types"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: mlir-opt input.mlir
1.	MLIR Parser: custom op parser 'builtin.module'
2.	MLIR Parser: custom op parser 'gpu.module'
3.	MLIR Parser: custom op parser 'func.func'
4.	MLIR Parser: custom op parser 'gpu.launch'
 #0 0x000055ef8936461d llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /projects/llvm-project/llvm/lib/Support/Unix/Signals.inc:834:11
 #1 0x000055ef89364b4b PrintStackTraceSignalHandler(void*) /projects/llvm-project/llvm/lib/Support/Unix/Signals.inc:916:1
 #2 0x000055ef89362a1f llvm::sys::RunSignalHandlers() /projects/llvm-project/llvm/lib/Support/Signals.cpp:104:5
 #3 0x000055ef893651e9 SignalHandler(int, siginfo_t*, void*) /projects/llvm-project/llvm/lib/Support/Unix/Signals.inc:426:38
 #4 0x00007fea4ba45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #5 0x00007fea4ba9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #6 0x00007fea4ba9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #7 0x00007fea4ba9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #8 0x00007fea4ba4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #9 0x00007fea4ba288ff abort ./stdlib/abort.c:81:7
#10 0x00007fea4ba2881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#11 0x00007fea4ba3b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#12 0x000055ef97237692 mlir::TypeRange::TypeRange(llvm::ArrayRef<mlir::Type>) /projects/llvm-project/mlir/lib/IR/TypeRange.cpp:21:1
#13 0x000055ef971fea66 mlir::TypeRange::TypeRange<llvm::SmallVector<mlir::Type, 4u> const&, void>(llvm::SmallVector<mlir::Type, 4u> const&) /projects/llvm-project/mlir/include/mlir/IR/TypeRange.h:51:60
#14 0x000055ef971f4cc8 mlir::Operation::create(mlir::OperationState const&) /projects/llvm-project/mlir/lib/IR/Operation.cpp:36:55
#15 0x000055ef97089601 mlir::OpBuilder::create(mlir::OperationState const&) /projects/llvm-project/mlir/lib/IR/Builders.cpp:458:17
#16 0x000055ef9486980d (anonymous namespace)::OperationParser::parseCustomOperation(llvm::ArrayRef<std::tuple<llvm::StringRef, unsigned int, llvm::SMLoc>>) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2165:14
#17 0x000055ef94865923 (anonymous namespace)::OperationParser::parseOperation() /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:1268:8
#18 0x000055ef94872ac2 (anonymous namespace)::OperationParser::parseBlockBody(mlir::Block*) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2440:9
#19 0x000055ef94871b8a (anonymous namespace)::OperationParser::parseBlock(mlir::Block*&) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2370:12
#20 0x000055ef94871753 (anonymous namespace)::OperationParser::parseRegionBody(mlir::Region&, llvm::SMLoc, llvm::ArrayRef<mlir::OpAsmParser::Argument>, bool) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2328:7
#21 0x000055ef9486ecc7 (anonymous namespace)::OperationParser::parseRegion(mlir::Region&, llvm::ArrayRef<mlir::OpAsmParser::Argument>, bool) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2257:7
#22 0x000055ef9486c970 (anonymous namespace)::CustomOpAsmParser::parseRegion(mlir::Region&, llvm::ArrayRef<mlir::OpAsmParser::Argument>, bool) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:1879:16
#23 0x000055ef9486ca54 (anonymous namespace)::CustomOpAsmParser::parseOptionalRegion(mlir::Region&, llvm::ArrayRef<mlir::OpAsmParser::Argument>, bool) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:1890:12
#24 0x000055ef96f3b16c mlir::function_interface_impl::parseFunctionOp(mlir::OpAsmParser&, mlir::OperationState&, bool, mlir::StringAttr, llvm::function_ref<mlir::Type (mlir::Builder&, llvm::ArrayRef<mlir::Type>, llvm::ArrayRef<mlir::Type>, mlir::function_interface_impl::VariadicFlag, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>&)>, mlir::StringAttr, mlir::StringAttr) /projects/llvm-project/mlir/lib/Interfaces/FunctionImplementation.cpp:154:14
#25 0x000055ef94e4c29e mlir::func::FuncOp::parse(mlir::OpAsmParser&, mlir::OperationState&) /projects/llvm-project/mlir/lib/Dialect/Func/IR/FuncOps.cpp:202:10
#26 0x000055ef896cccbd llvm::ParseResult llvm::detail::UniqueFunctionBase<llvm::ParseResult, mlir::OpAsmParser&, mlir::OperationState&>::CallImpl<llvm::ParseResult (*)(mlir::OpAsmParser&, mlir::OperationState&)>(void*, mlir::OpAsmParser&, mlir::OperationState&) /projects/llvm-project/llvm/include/llvm/ADT/FunctionExtras.h:213:12
#27 0x000055ef9487b35f llvm::unique_function<llvm::ParseResult (mlir::OpAsmParser&, mlir::OperationState&)>::operator()(mlir::OpAsmParser&, mlir::OperationState&) /projects/llvm-project/llvm/include/llvm/ADT/FunctionExtras.h:365:12
#28 0x000055ef9487b305 llvm::ParseResult llvm::function_ref<llvm::ParseResult (mlir::OpAsmParser&, mlir::OperationState&)>::callback_fn<llvm::unique_function<llvm::ParseResult (mlir::OpAsmParser&, mlir::OperationState&)>>(long, mlir::OpAsmParser&, mlir::OperationState&) /projects/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#29 0x000055ef9487b2c9 llvm::function_ref<llvm::ParseResult (mlir::OpAsmParser&, mlir::OperationState&)>::operator()(mlir::OpAsmParser&, mlir::OperationState&) const /projects/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#30 0x000055ef9486ae2e (anonymous namespace)::CustomOpAsmParser::parseOperation(mlir::OperationState&) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:1615:9
#31 0x000055ef94869760 (anonymous namespace)::OperationParser::parseCustomOperation(llvm::ArrayRef<std::tuple<llvm::StringRef, unsigned int, llvm::SMLoc>>) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2154:19
#32 0x000055ef94865923 (anonymous namespace)::OperationParser::parseOperation() /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:1268:8
#33 0x000055ef94872ac2 (anonymous namespace)::OperationParser::parseBlockBody(mlir::Block*) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2440:9
#34 0x000055ef94871b8a (anonymous namespace)::OperationParser::parseBlock(mlir::Block*&) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2370:12
#35 0x000055ef94871753 (anonymous namespace)::OperationParser::parseRegionBody(mlir::Region&, llvm::SMLoc, llvm::ArrayRef<mlir::OpAsmParser::Argument>, bool) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2328:7
#36 0x000055ef9486ecc7 (anonymous namespace)::OperationParser::parseRegion(mlir::Region&, llvm::ArrayRef<mlir::OpAsmParser::Argument>, bool) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2257:7
#37 0x000055ef9486c970 (anonymous namespace)::CustomOpAsmParser::parseRegion(mlir::Region&, llvm::ArrayRef<mlir::OpAsmParser::Argument>, bool) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:1879:16
#38 0x000055ef954c0dcf mlir::gpu::GPUModuleOp::parse(mlir::OpAsmParser&, mlir::OperationState&) /projects/llvm-project/build_all/tools/mlir/include/mlir/Dialect/GPU/IR/GPUOps.cpp.inc:11193:14
#39 0x000055ef896cccbd llvm::ParseResult llvm::detail::UniqueFunctionBase<llvm::ParseResult, mlir::OpAsmParser&, mlir::OperationState&>::CallImpl<llvm::ParseResult (*)(mlir::OpAsmParser&, mlir::OperationState&)>(void*, mlir::OpAsmParser&, mlir::OperationState&) /projects/llvm-project/llvm/include/llvm/ADT/FunctionExtras.h:213:12
#40 0x000055ef9487b35f llvm::unique_function<llvm::ParseResult (mlir::OpAsmParser&, mlir::OperationState&)>::operator()(mlir::OpAsmParser&, mlir::OperationState&) /projects/llvm-project/llvm/include/llvm/ADT/FunctionExtras.h:365:12
#41 0x000055ef9487b305 llvm::ParseResult llvm::function_ref<llvm::ParseResult (mlir::OpAsmParser&, mlir::OperationState&)>::callback_fn<llvm::unique_function<llvm::ParseResult (mlir::OpAsmParser&, mlir::OperationState&)>>(long, mlir::OpAsmParser&, mlir::OperationState&) /projects/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#42 0x000055ef9487b2c9 llvm::function_ref<llvm::ParseResult (mlir::OpAsmParser&, mlir::OperationState&)>::operator()(mlir::OpAsmParser&, mlir::OperationState&) const /projects/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#43 0x000055ef9486ae2e (anonymous namespace)::CustomOpAsmParser::parseOperation(mlir::OperationState&) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:1615:9
#44 0x000055ef94869760 (anonymous namespace)::OperationParser::parseCustomOperation(llvm::ArrayRef<std::tuple<llvm::StringRef, unsigned int, llvm::SMLoc>>) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2154:19
#45 0x000055ef94865923 (anonymous namespace)::OperationParser::parseOperation() /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:1268:8
#46 0x000055ef94872ac2 (anonymous namespace)::OperationParser::parseBlockBody(mlir::Block*) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2440:9
#47 0x000055ef94871b8a (anonymous namespace)::OperationParser::parseBlock(mlir::Block*&) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2370:12
#48 0x000055ef94871753 (anonymous namespace)::OperationParser::parseRegionBody(mlir::Region&, llvm::SMLoc, llvm::ArrayRef<mlir::OpAsmParser::Argument>, bool) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2328:7
#49 0x000055ef9486ecc7 (anonymous namespace)::OperationParser::parseRegion(mlir::Region&, llvm::ArrayRef<mlir::OpAsmParser::Argument>, bool) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2257:7
#50 0x000055ef9486c970 (anonymous namespace)::CustomOpAsmParser::parseRegion(mlir::Region&, llvm::ArrayRef<mlir::OpAsmParser::Argument>, bool) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:1879:16
#51 0x000055ef97122aad mlir::ModuleOp::parse(mlir::OpAsmParser&, mlir::OperationState&) /projects/llvm-project/build_all/tools/mlir/include/mlir/IR/BuiltinOps.cpp.inc:304:14
#52 0x000055ef896cccbd llvm::ParseResult llvm::detail::UniqueFunctionBase<llvm::ParseResult, mlir::OpAsmParser&, mlir::OperationState&>::CallImpl<llvm::ParseResult (*)(mlir::OpAsmParser&, mlir::OperationState&)>(void*, mlir::OpAsmParser&, mlir::OperationState&) /projects/llvm-project/llvm/include/llvm/ADT/FunctionExtras.h:213:12
#53 0x000055ef9487b35f llvm::unique_function<llvm::ParseResult (mlir::OpAsmParser&, mlir::OperationState&)>::operator()(mlir::OpAsmParser&, mlir::OperationState&) /projects/llvm-project/llvm/include/llvm/ADT/FunctionExtras.h:365:12
#54 0x000055ef9487b305 llvm::ParseResult llvm::function_ref<llvm::ParseResult (mlir::OpAsmParser&, mlir::OperationState&)>::callback_fn<llvm::unique_function<llvm::ParseResult (mlir::OpAsmParser&, mlir::OperationState&)>>(long, mlir::OpAsmParser&, mlir::OperationState&) /projects/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#55 0x000055ef9487b2c9 llvm::function_ref<llvm::ParseResult (mlir::OpAsmParser&, mlir::OperationState&)>::operator()(mlir::OpAsmParser&, mlir::OperationState&) const /projects/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#56 0x000055ef9486ae2e (anonymous namespace)::CustomOpAsmParser::parseOperation(mlir::OperationState&) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:1615:9
#57 0x000055ef94869760 (anonymous namespace)::OperationParser::parseCustomOperation(llvm::ArrayRef<std::tuple<llvm::StringRef, unsigned int, llvm::SMLoc>>) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2154:19
#58 0x000055ef94865923 (anonymous namespace)::OperationParser::parseOperation() /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:1268:8
#59 0x000055ef9486536d (anonymous namespace)::TopLevelOperationParser::parse(mlir::Block*, mlir::Location) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2870:20
#60 0x000055ef9486520e mlir::parseAsmSourceFile(llvm::SourceMgr const&, mlir::Block*, mlir::ParserConfig const&, mlir::AsmParserState*, mlir::AsmParserCodeCompleteContext*) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2930:41
#61 0x000055ef9483634f mlir::parseSourceFile(std::shared_ptr<llvm::SourceMgr> const&, mlir::Block*, mlir::ParserConfig const&, mlir::LocationAttr*) /projects/llvm-project/mlir/lib/Parser/Parser.cpp:64:10
#62 0x000055ef8947ec0f mlir::OwningOpRef<mlir::ModuleOp> mlir::detail::parseSourceFile<mlir::ModuleOp, std::shared_ptr<llvm::SourceMgr> const&>(mlir::ParserConfig const&, std::shared_ptr<llvm::SourceMgr> const&) /projects/llvm-project/mlir/include/mlir/Parser/Parser.h:158:14
#63 0x000055ef8947eb18 mlir::OwningOpRef<mlir::ModuleOp> mlir::parseSourceFile<mlir::ModuleOp>(std::shared_ptr<llvm::SourceMgr> const&, mlir::ParserConfig const&) /projects/llvm-project/mlir/include/mlir/Parser/Parser.h:188:10
#64 0x000055ef8947ddbd mlir::parseSourceFileForTool(std::shared_ptr<llvm::SourceMgr> const&, mlir::ParserConfig const&, bool) /projects/llvm-project/mlir/include/mlir/Tools/ParseUtilities.h:31:12
#65 0x000055ef89464d6a performActions(llvm::raw_ostream&, std::shared_ptr<llvm::SourceMgr> const&, mlir::MLIRContext*, mlir::MlirOptMainConfig const&) /projects/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:523:16
#66 0x000055ef89464b71 processBuffer(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&, mlir::DialectRegistry&, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /projects/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:673:12
#67 0x000055ef8946482c mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, llvm::raw_ostream&) const /projects/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:762:12
#68 0x000055ef89464736 llvm::LogicalResult llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::callback_fn<mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) /projects/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#69 0x000055ef9726dcba llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) const /projects/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#70 0x000055ef9726d07a mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef) /projects/llvm-project/mlir/lib/Support/ToolUtilities.cpp:30:12
#71 0x000055ef89460126 mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&) /projects/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:767:26
#72 0x000055ef89460525 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&) /projects/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:813:14
#73 0x000055ef894606f8 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&) /projects/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:829:10
#74 0x000055ef89339935 main /projects/llvm-project/mlir/tools/mlir-opt/mlir-opt.cpp:347:33
#75 0x00007fea4ba2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#76 0x00007fea4ba2a28b call_init ./csu/../csu/libc-start.c:128:20
#77 0x00007fea4ba2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#78 0x000055ef89339505 _start (/mnt/raid/projects/llvm-project/build_all/bin/mlir-opt+0x435c505)
Aborted
```

### Used Command
```bash
mlir-opt input.mlir
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: None (RJerrica)

<details>
Hi, I ran into a crash while building a program using the GPU Dialect. Please see the program below for details.

### Input Program
```llvm
module attributes {gpu.container_module} {
  gpu.module @<!-- -->volumemodule {
    func.func @<!-- -->main() {
      %0 = gpu.launch blocks(%bx, %by, %bz) in (%gx = %c1, %gy = %c2, %gz = %c1) threads(%tx, %ty, %tz) in (%lx = %c4, %ly = %c5, %lz = %c1) {
        gpu.barrier
        gpu.terminator
      }
      return
    }
    func.func private @<!-- -->constant() -&gt; (i32, i32, i32, i32, i32, i32) attributes {gpu.kernel} {
      %c1 = arith.constant 1 : i32
      %c2 = arith.constant 2 : i32
      %c4 = arith.constant 4 : i32
      %c5 = arith.constant 5 : i32
      return %c1, %c2, %c1, %c4, %c5, %c1 : i32, i32, i32, i32, i32, i32
    }
  }
}
```

### Stack Trace
```bash
mlir-opt: /projects/llvm-project/mlir/lib/IR/TypeRange.cpp:20: mlir::TypeRange::TypeRange(ArrayRef&lt;Type&gt;): Assertion `llvm::all_of(types, [](Type t) { return t; }) &amp;&amp; "attempting to construct a TypeRange with null types"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: mlir-opt input.mlir
1.	MLIR Parser: custom op parser 'builtin.module'
2.	MLIR Parser: custom op parser 'gpu.module'
3.	MLIR Parser: custom op parser 'func.func'
4.	MLIR Parser: custom op parser 'gpu.launch'
 #<!-- -->0 0x000055ef8936461d llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /projects/llvm-project/llvm/lib/Support/Unix/Signals.inc:834:11
 #<!-- -->1 0x000055ef89364b4b PrintStackTraceSignalHandler(void*) /projects/llvm-project/llvm/lib/Support/Unix/Signals.inc:916:1
 #<!-- -->2 0x000055ef89362a1f llvm::sys::RunSignalHandlers() /projects/llvm-project/llvm/lib/Support/Signals.cpp:104:5
 #<!-- -->3 0x000055ef893651e9 SignalHandler(int, siginfo_t*, void*) /projects/llvm-project/llvm/lib/Support/Unix/Signals.inc:426:38
 #<!-- -->4 0x00007fea4ba45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->5 0x00007fea4ba9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->6 0x00007fea4ba9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->7 0x00007fea4ba9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->8 0x00007fea4ba4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x00007fea4ba288ff abort ./stdlib/abort.c:81:7
#<!-- -->10 0x00007fea4ba2881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->11 0x00007fea4ba3b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->12 0x000055ef97237692 mlir::TypeRange::TypeRange(llvm::ArrayRef&lt;mlir::Type&gt;) /projects/llvm-project/mlir/lib/IR/TypeRange.cpp:21:1
#<!-- -->13 0x000055ef971fea66 mlir::TypeRange::TypeRange&lt;llvm::SmallVector&lt;mlir::Type, 4u&gt; const&amp;, void&gt;(llvm::SmallVector&lt;mlir::Type, 4u&gt; const&amp;) /projects/llvm-project/mlir/include/mlir/IR/TypeRange.h:51:60
#<!-- -->14 0x000055ef971f4cc8 mlir::Operation::create(mlir::OperationState const&amp;) /projects/llvm-project/mlir/lib/IR/Operation.cpp:36:55
#<!-- -->15 0x000055ef97089601 mlir::OpBuilder::create(mlir::OperationState const&amp;) /projects/llvm-project/mlir/lib/IR/Builders.cpp:458:17
#<!-- -->16 0x000055ef9486980d (anonymous namespace)::OperationParser::parseCustomOperation(llvm::ArrayRef&lt;std::tuple&lt;llvm::StringRef, unsigned int, llvm::SMLoc&gt;&gt;) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2165:14
#<!-- -->17 0x000055ef94865923 (anonymous namespace)::OperationParser::parseOperation() /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:1268:8
#<!-- -->18 0x000055ef94872ac2 (anonymous namespace)::OperationParser::parseBlockBody(mlir::Block*) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2440:9
#<!-- -->19 0x000055ef94871b8a (anonymous namespace)::OperationParser::parseBlock(mlir::Block*&amp;) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2370:12
#<!-- -->20 0x000055ef94871753 (anonymous namespace)::OperationParser::parseRegionBody(mlir::Region&amp;, llvm::SMLoc, llvm::ArrayRef&lt;mlir::OpAsmParser::Argument&gt;, bool) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2328:7
#<!-- -->21 0x000055ef9486ecc7 (anonymous namespace)::OperationParser::parseRegion(mlir::Region&amp;, llvm::ArrayRef&lt;mlir::OpAsmParser::Argument&gt;, bool) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2257:7
#<!-- -->22 0x000055ef9486c970 (anonymous namespace)::CustomOpAsmParser::parseRegion(mlir::Region&amp;, llvm::ArrayRef&lt;mlir::OpAsmParser::Argument&gt;, bool) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:1879:16
#<!-- -->23 0x000055ef9486ca54 (anonymous namespace)::CustomOpAsmParser::parseOptionalRegion(mlir::Region&amp;, llvm::ArrayRef&lt;mlir::OpAsmParser::Argument&gt;, bool) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:1890:12
#<!-- -->24 0x000055ef96f3b16c mlir::function_interface_impl::parseFunctionOp(mlir::OpAsmParser&amp;, mlir::OperationState&amp;, bool, mlir::StringAttr, llvm::function_ref&lt;mlir::Type (mlir::Builder&amp;, llvm::ArrayRef&lt;mlir::Type&gt;, llvm::ArrayRef&lt;mlir::Type&gt;, mlir::function_interface_impl::VariadicFlag, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;&amp;)&gt;, mlir::StringAttr, mlir::StringAttr) /projects/llvm-project/mlir/lib/Interfaces/FunctionImplementation.cpp:154:14
#<!-- -->25 0x000055ef94e4c29e mlir::func::FuncOp::parse(mlir::OpAsmParser&amp;, mlir::OperationState&amp;) /projects/llvm-project/mlir/lib/Dialect/Func/IR/FuncOps.cpp:202:10
#<!-- -->26 0x000055ef896cccbd llvm::ParseResult llvm::detail::UniqueFunctionBase&lt;llvm::ParseResult, mlir::OpAsmParser&amp;, mlir::OperationState&amp;&gt;::CallImpl&lt;llvm::ParseResult (*)(mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;(void*, mlir::OpAsmParser&amp;, mlir::OperationState&amp;) /projects/llvm-project/llvm/include/llvm/ADT/FunctionExtras.h:213:12
#<!-- -->27 0x000055ef9487b35f llvm::unique_function&lt;llvm::ParseResult (mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;::operator()(mlir::OpAsmParser&amp;, mlir::OperationState&amp;) /projects/llvm-project/llvm/include/llvm/ADT/FunctionExtras.h:365:12
#<!-- -->28 0x000055ef9487b305 llvm::ParseResult llvm::function_ref&lt;llvm::ParseResult (mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;::callback_fn&lt;llvm::unique_function&lt;llvm::ParseResult (mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;&gt;(long, mlir::OpAsmParser&amp;, mlir::OperationState&amp;) /projects/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->29 0x000055ef9487b2c9 llvm::function_ref&lt;llvm::ParseResult (mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;::operator()(mlir::OpAsmParser&amp;, mlir::OperationState&amp;) const /projects/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->30 0x000055ef9486ae2e (anonymous namespace)::CustomOpAsmParser::parseOperation(mlir::OperationState&amp;) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:1615:9
#<!-- -->31 0x000055ef94869760 (anonymous namespace)::OperationParser::parseCustomOperation(llvm::ArrayRef&lt;std::tuple&lt;llvm::StringRef, unsigned int, llvm::SMLoc&gt;&gt;) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2154:19
#<!-- -->32 0x000055ef94865923 (anonymous namespace)::OperationParser::parseOperation() /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:1268:8
#<!-- -->33 0x000055ef94872ac2 (anonymous namespace)::OperationParser::parseBlockBody(mlir::Block*) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2440:9
#<!-- -->34 0x000055ef94871b8a (anonymous namespace)::OperationParser::parseBlock(mlir::Block*&amp;) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2370:12
#<!-- -->35 0x000055ef94871753 (anonymous namespace)::OperationParser::parseRegionBody(mlir::Region&amp;, llvm::SMLoc, llvm::ArrayRef&lt;mlir::OpAsmParser::Argument&gt;, bool) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2328:7
#<!-- -->36 0x000055ef9486ecc7 (anonymous namespace)::OperationParser::parseRegion(mlir::Region&amp;, llvm::ArrayRef&lt;mlir::OpAsmParser::Argument&gt;, bool) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2257:7
#<!-- -->37 0x000055ef9486c970 (anonymous namespace)::CustomOpAsmParser::parseRegion(mlir::Region&amp;, llvm::ArrayRef&lt;mlir::OpAsmParser::Argument&gt;, bool) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:1879:16
#<!-- -->38 0x000055ef954c0dcf mlir::gpu::GPUModuleOp::parse(mlir::OpAsmParser&amp;, mlir::OperationState&amp;) /projects/llvm-project/build_all/tools/mlir/include/mlir/Dialect/GPU/IR/GPUOps.cpp.inc:11193:14
#<!-- -->39 0x000055ef896cccbd llvm::ParseResult llvm::detail::UniqueFunctionBase&lt;llvm::ParseResult, mlir::OpAsmParser&amp;, mlir::OperationState&amp;&gt;::CallImpl&lt;llvm::ParseResult (*)(mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;(void*, mlir::OpAsmParser&amp;, mlir::OperationState&amp;) /projects/llvm-project/llvm/include/llvm/ADT/FunctionExtras.h:213:12
#<!-- -->40 0x000055ef9487b35f llvm::unique_function&lt;llvm::ParseResult (mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;::operator()(mlir::OpAsmParser&amp;, mlir::OperationState&amp;) /projects/llvm-project/llvm/include/llvm/ADT/FunctionExtras.h:365:12
#<!-- -->41 0x000055ef9487b305 llvm::ParseResult llvm::function_ref&lt;llvm::ParseResult (mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;::callback_fn&lt;llvm::unique_function&lt;llvm::ParseResult (mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;&gt;(long, mlir::OpAsmParser&amp;, mlir::OperationState&amp;) /projects/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->42 0x000055ef9487b2c9 llvm::function_ref&lt;llvm::ParseResult (mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;::operator()(mlir::OpAsmParser&amp;, mlir::OperationState&amp;) const /projects/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->43 0x000055ef9486ae2e (anonymous namespace)::CustomOpAsmParser::parseOperation(mlir::OperationState&amp;) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:1615:9
#<!-- -->44 0x000055ef94869760 (anonymous namespace)::OperationParser::parseCustomOperation(llvm::ArrayRef&lt;std::tuple&lt;llvm::StringRef, unsigned int, llvm::SMLoc&gt;&gt;) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2154:19
#<!-- -->45 0x000055ef94865923 (anonymous namespace)::OperationParser::parseOperation() /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:1268:8
#<!-- -->46 0x000055ef94872ac2 (anonymous namespace)::OperationParser::parseBlockBody(mlir::Block*) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2440:9
#<!-- -->47 0x000055ef94871b8a (anonymous namespace)::OperationParser::parseBlock(mlir::Block*&amp;) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2370:12
#<!-- -->48 0x000055ef94871753 (anonymous namespace)::OperationParser::parseRegionBody(mlir::Region&amp;, llvm::SMLoc, llvm::ArrayRef&lt;mlir::OpAsmParser::Argument&gt;, bool) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2328:7
#<!-- -->49 0x000055ef9486ecc7 (anonymous namespace)::OperationParser::parseRegion(mlir::Region&amp;, llvm::ArrayRef&lt;mlir::OpAsmParser::Argument&gt;, bool) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2257:7
#<!-- -->50 0x000055ef9486c970 (anonymous namespace)::CustomOpAsmParser::parseRegion(mlir::Region&amp;, llvm::ArrayRef&lt;mlir::OpAsmParser::Argument&gt;, bool) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:1879:16
#<!-- -->51 0x000055ef97122aad mlir::ModuleOp::parse(mlir::OpAsmParser&amp;, mlir::OperationState&amp;) /projects/llvm-project/build_all/tools/mlir/include/mlir/IR/BuiltinOps.cpp.inc:304:14
#<!-- -->52 0x000055ef896cccbd llvm::ParseResult llvm::detail::UniqueFunctionBase&lt;llvm::ParseResult, mlir::OpAsmParser&amp;, mlir::OperationState&amp;&gt;::CallImpl&lt;llvm::ParseResult (*)(mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;(void*, mlir::OpAsmParser&amp;, mlir::OperationState&amp;) /projects/llvm-project/llvm/include/llvm/ADT/FunctionExtras.h:213:12
#<!-- -->53 0x000055ef9487b35f llvm::unique_function&lt;llvm::ParseResult (mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;::operator()(mlir::OpAsmParser&amp;, mlir::OperationState&amp;) /projects/llvm-project/llvm/include/llvm/ADT/FunctionExtras.h:365:12
#<!-- -->54 0x000055ef9487b305 llvm::ParseResult llvm::function_ref&lt;llvm::ParseResult (mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;::callback_fn&lt;llvm::unique_function&lt;llvm::ParseResult (mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;&gt;(long, mlir::OpAsmParser&amp;, mlir::OperationState&amp;) /projects/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->55 0x000055ef9487b2c9 llvm::function_ref&lt;llvm::ParseResult (mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;::operator()(mlir::OpAsmParser&amp;, mlir::OperationState&amp;) const /projects/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->56 0x000055ef9486ae2e (anonymous namespace)::CustomOpAsmParser::parseOperation(mlir::OperationState&amp;) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:1615:9
#<!-- -->57 0x000055ef94869760 (anonymous namespace)::OperationParser::parseCustomOperation(llvm::ArrayRef&lt;std::tuple&lt;llvm::StringRef, unsigned int, llvm::SMLoc&gt;&gt;) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2154:19
#<!-- -->58 0x000055ef94865923 (anonymous namespace)::OperationParser::parseOperation() /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:1268:8
#<!-- -->59 0x000055ef9486536d (anonymous namespace)::TopLevelOperationParser::parse(mlir::Block*, mlir::Location) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2870:20
#<!-- -->60 0x000055ef9486520e mlir::parseAsmSourceFile(llvm::SourceMgr const&amp;, mlir::Block*, mlir::ParserConfig const&amp;, mlir::AsmParserState*, mlir::AsmParserCodeCompleteContext*) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2930:41
#<!-- -->61 0x000055ef9483634f mlir::parseSourceFile(std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::Block*, mlir::ParserConfig const&amp;, mlir::LocationAttr*) /projects/llvm-project/mlir/lib/Parser/Parser.cpp:64:10
#<!-- -->62 0x000055ef8947ec0f mlir::OwningOpRef&lt;mlir::ModuleOp&gt; mlir::detail::parseSourceFile&lt;mlir::ModuleOp, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;&gt;(mlir::ParserConfig const&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;) /projects/llvm-project/mlir/include/mlir/Parser/Parser.h:158:14
#<!-- -->63 0x000055ef8947eb18 mlir::OwningOpRef&lt;mlir::ModuleOp&gt; mlir::parseSourceFile&lt;mlir::ModuleOp&gt;(std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::ParserConfig const&amp;) /projects/llvm-project/mlir/include/mlir/Parser/Parser.h:188:10
#<!-- -->64 0x000055ef8947ddbd mlir::parseSourceFileForTool(std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::ParserConfig const&amp;, bool) /projects/llvm-project/mlir/include/mlir/Tools/ParseUtilities.h:31:12
#<!-- -->65 0x000055ef89464d6a performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) /projects/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:523:16
#<!-- -->66 0x000055ef89464b71 processBuffer(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&amp;, mlir::DialectRegistry&amp;, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /projects/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:673:12
#<!-- -->67 0x000055ef8946482c mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, llvm::raw_ostream&amp;) const /projects/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:762:12
#<!-- -->68 0x000055ef89464736 llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) /projects/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->69 0x000055ef9726dcba llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) const /projects/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->70 0x000055ef9726d07a mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) /projects/llvm-project/mlir/lib/Support/ToolUtilities.cpp:30:12
#<!-- -->71 0x000055ef89460126 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) /projects/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:767:26
#<!-- -->72 0x000055ef89460525 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) /projects/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:813:14
#<!-- -->73 0x000055ef894606f8 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) /projects/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:829:10
#<!-- -->74 0x000055ef89339935 main /projects/llvm-project/mlir/tools/mlir-opt/mlir-opt.cpp:347:33
#<!-- -->75 0x00007fea4ba2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->76 0x00007fea4ba2a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->77 0x00007fea4ba2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->78 0x000055ef89339505 _start (/mnt/raid/projects/llvm-project/build_all/bin/mlir-opt+0x435c505)
Aborted
```

### Used Command
```bash
mlir-opt input.mlir
```
</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-mlir-gpu

Author: None (RJerrica)

<details>
Hi, I ran into a crash while building a program using the GPU Dialect. Please see the program below for details.

### Input Program
```llvm
module attributes {gpu.container_module} {
  gpu.module @<!-- -->volumemodule {
    func.func @<!-- -->main() {
      %0 = gpu.launch blocks(%bx, %by, %bz) in (%gx = %c1, %gy = %c2, %gz = %c1) threads(%tx, %ty, %tz) in (%lx = %c4, %ly = %c5, %lz = %c1) {
        gpu.barrier
        gpu.terminator
      }
      return
    }
    func.func private @<!-- -->constant() -&gt; (i32, i32, i32, i32, i32, i32) attributes {gpu.kernel} {
      %c1 = arith.constant 1 : i32
      %c2 = arith.constant 2 : i32
      %c4 = arith.constant 4 : i32
      %c5 = arith.constant 5 : i32
      return %c1, %c2, %c1, %c4, %c5, %c1 : i32, i32, i32, i32, i32, i32
    }
  }
}
```

### Stack Trace
```bash
mlir-opt: /projects/llvm-project/mlir/lib/IR/TypeRange.cpp:20: mlir::TypeRange::TypeRange(ArrayRef&lt;Type&gt;): Assertion `llvm::all_of(types, [](Type t) { return t; }) &amp;&amp; "attempting to construct a TypeRange with null types"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: mlir-opt input.mlir
1.	MLIR Parser: custom op parser 'builtin.module'
2.	MLIR Parser: custom op parser 'gpu.module'
3.	MLIR Parser: custom op parser 'func.func'
4.	MLIR Parser: custom op parser 'gpu.launch'
 #<!-- -->0 0x000055ef8936461d llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /projects/llvm-project/llvm/lib/Support/Unix/Signals.inc:834:11
 #<!-- -->1 0x000055ef89364b4b PrintStackTraceSignalHandler(void*) /projects/llvm-project/llvm/lib/Support/Unix/Signals.inc:916:1
 #<!-- -->2 0x000055ef89362a1f llvm::sys::RunSignalHandlers() /projects/llvm-project/llvm/lib/Support/Signals.cpp:104:5
 #<!-- -->3 0x000055ef893651e9 SignalHandler(int, siginfo_t*, void*) /projects/llvm-project/llvm/lib/Support/Unix/Signals.inc:426:38
 #<!-- -->4 0x00007fea4ba45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->5 0x00007fea4ba9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->6 0x00007fea4ba9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->7 0x00007fea4ba9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->8 0x00007fea4ba4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x00007fea4ba288ff abort ./stdlib/abort.c:81:7
#<!-- -->10 0x00007fea4ba2881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->11 0x00007fea4ba3b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->12 0x000055ef97237692 mlir::TypeRange::TypeRange(llvm::ArrayRef&lt;mlir::Type&gt;) /projects/llvm-project/mlir/lib/IR/TypeRange.cpp:21:1
#<!-- -->13 0x000055ef971fea66 mlir::TypeRange::TypeRange&lt;llvm::SmallVector&lt;mlir::Type, 4u&gt; const&amp;, void&gt;(llvm::SmallVector&lt;mlir::Type, 4u&gt; const&amp;) /projects/llvm-project/mlir/include/mlir/IR/TypeRange.h:51:60
#<!-- -->14 0x000055ef971f4cc8 mlir::Operation::create(mlir::OperationState const&amp;) /projects/llvm-project/mlir/lib/IR/Operation.cpp:36:55
#<!-- -->15 0x000055ef97089601 mlir::OpBuilder::create(mlir::OperationState const&amp;) /projects/llvm-project/mlir/lib/IR/Builders.cpp:458:17
#<!-- -->16 0x000055ef9486980d (anonymous namespace)::OperationParser::parseCustomOperation(llvm::ArrayRef&lt;std::tuple&lt;llvm::StringRef, unsigned int, llvm::SMLoc&gt;&gt;) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2165:14
#<!-- -->17 0x000055ef94865923 (anonymous namespace)::OperationParser::parseOperation() /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:1268:8
#<!-- -->18 0x000055ef94872ac2 (anonymous namespace)::OperationParser::parseBlockBody(mlir::Block*) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2440:9
#<!-- -->19 0x000055ef94871b8a (anonymous namespace)::OperationParser::parseBlock(mlir::Block*&amp;) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2370:12
#<!-- -->20 0x000055ef94871753 (anonymous namespace)::OperationParser::parseRegionBody(mlir::Region&amp;, llvm::SMLoc, llvm::ArrayRef&lt;mlir::OpAsmParser::Argument&gt;, bool) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2328:7
#<!-- -->21 0x000055ef9486ecc7 (anonymous namespace)::OperationParser::parseRegion(mlir::Region&amp;, llvm::ArrayRef&lt;mlir::OpAsmParser::Argument&gt;, bool) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2257:7
#<!-- -->22 0x000055ef9486c970 (anonymous namespace)::CustomOpAsmParser::parseRegion(mlir::Region&amp;, llvm::ArrayRef&lt;mlir::OpAsmParser::Argument&gt;, bool) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:1879:16
#<!-- -->23 0x000055ef9486ca54 (anonymous namespace)::CustomOpAsmParser::parseOptionalRegion(mlir::Region&amp;, llvm::ArrayRef&lt;mlir::OpAsmParser::Argument&gt;, bool) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:1890:12
#<!-- -->24 0x000055ef96f3b16c mlir::function_interface_impl::parseFunctionOp(mlir::OpAsmParser&amp;, mlir::OperationState&amp;, bool, mlir::StringAttr, llvm::function_ref&lt;mlir::Type (mlir::Builder&amp;, llvm::ArrayRef&lt;mlir::Type&gt;, llvm::ArrayRef&lt;mlir::Type&gt;, mlir::function_interface_impl::VariadicFlag, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;&amp;)&gt;, mlir::StringAttr, mlir::StringAttr) /projects/llvm-project/mlir/lib/Interfaces/FunctionImplementation.cpp:154:14
#<!-- -->25 0x000055ef94e4c29e mlir::func::FuncOp::parse(mlir::OpAsmParser&amp;, mlir::OperationState&amp;) /projects/llvm-project/mlir/lib/Dialect/Func/IR/FuncOps.cpp:202:10
#<!-- -->26 0x000055ef896cccbd llvm::ParseResult llvm::detail::UniqueFunctionBase&lt;llvm::ParseResult, mlir::OpAsmParser&amp;, mlir::OperationState&amp;&gt;::CallImpl&lt;llvm::ParseResult (*)(mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;(void*, mlir::OpAsmParser&amp;, mlir::OperationState&amp;) /projects/llvm-project/llvm/include/llvm/ADT/FunctionExtras.h:213:12
#<!-- -->27 0x000055ef9487b35f llvm::unique_function&lt;llvm::ParseResult (mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;::operator()(mlir::OpAsmParser&amp;, mlir::OperationState&amp;) /projects/llvm-project/llvm/include/llvm/ADT/FunctionExtras.h:365:12
#<!-- -->28 0x000055ef9487b305 llvm::ParseResult llvm::function_ref&lt;llvm::ParseResult (mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;::callback_fn&lt;llvm::unique_function&lt;llvm::ParseResult (mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;&gt;(long, mlir::OpAsmParser&amp;, mlir::OperationState&amp;) /projects/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->29 0x000055ef9487b2c9 llvm::function_ref&lt;llvm::ParseResult (mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;::operator()(mlir::OpAsmParser&amp;, mlir::OperationState&amp;) const /projects/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->30 0x000055ef9486ae2e (anonymous namespace)::CustomOpAsmParser::parseOperation(mlir::OperationState&amp;) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:1615:9
#<!-- -->31 0x000055ef94869760 (anonymous namespace)::OperationParser::parseCustomOperation(llvm::ArrayRef&lt;std::tuple&lt;llvm::StringRef, unsigned int, llvm::SMLoc&gt;&gt;) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2154:19
#<!-- -->32 0x000055ef94865923 (anonymous namespace)::OperationParser::parseOperation() /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:1268:8
#<!-- -->33 0x000055ef94872ac2 (anonymous namespace)::OperationParser::parseBlockBody(mlir::Block*) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2440:9
#<!-- -->34 0x000055ef94871b8a (anonymous namespace)::OperationParser::parseBlock(mlir::Block*&amp;) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2370:12
#<!-- -->35 0x000055ef94871753 (anonymous namespace)::OperationParser::parseRegionBody(mlir::Region&amp;, llvm::SMLoc, llvm::ArrayRef&lt;mlir::OpAsmParser::Argument&gt;, bool) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2328:7
#<!-- -->36 0x000055ef9486ecc7 (anonymous namespace)::OperationParser::parseRegion(mlir::Region&amp;, llvm::ArrayRef&lt;mlir::OpAsmParser::Argument&gt;, bool) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2257:7
#<!-- -->37 0x000055ef9486c970 (anonymous namespace)::CustomOpAsmParser::parseRegion(mlir::Region&amp;, llvm::ArrayRef&lt;mlir::OpAsmParser::Argument&gt;, bool) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:1879:16
#<!-- -->38 0x000055ef954c0dcf mlir::gpu::GPUModuleOp::parse(mlir::OpAsmParser&amp;, mlir::OperationState&amp;) /projects/llvm-project/build_all/tools/mlir/include/mlir/Dialect/GPU/IR/GPUOps.cpp.inc:11193:14
#<!-- -->39 0x000055ef896cccbd llvm::ParseResult llvm::detail::UniqueFunctionBase&lt;llvm::ParseResult, mlir::OpAsmParser&amp;, mlir::OperationState&amp;&gt;::CallImpl&lt;llvm::ParseResult (*)(mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;(void*, mlir::OpAsmParser&amp;, mlir::OperationState&amp;) /projects/llvm-project/llvm/include/llvm/ADT/FunctionExtras.h:213:12
#<!-- -->40 0x000055ef9487b35f llvm::unique_function&lt;llvm::ParseResult (mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;::operator()(mlir::OpAsmParser&amp;, mlir::OperationState&amp;) /projects/llvm-project/llvm/include/llvm/ADT/FunctionExtras.h:365:12
#<!-- -->41 0x000055ef9487b305 llvm::ParseResult llvm::function_ref&lt;llvm::ParseResult (mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;::callback_fn&lt;llvm::unique_function&lt;llvm::ParseResult (mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;&gt;(long, mlir::OpAsmParser&amp;, mlir::OperationState&amp;) /projects/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->42 0x000055ef9487b2c9 llvm::function_ref&lt;llvm::ParseResult (mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;::operator()(mlir::OpAsmParser&amp;, mlir::OperationState&amp;) const /projects/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->43 0x000055ef9486ae2e (anonymous namespace)::CustomOpAsmParser::parseOperation(mlir::OperationState&amp;) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:1615:9
#<!-- -->44 0x000055ef94869760 (anonymous namespace)::OperationParser::parseCustomOperation(llvm::ArrayRef&lt;std::tuple&lt;llvm::StringRef, unsigned int, llvm::SMLoc&gt;&gt;) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2154:19
#<!-- -->45 0x000055ef94865923 (anonymous namespace)::OperationParser::parseOperation() /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:1268:8
#<!-- -->46 0x000055ef94872ac2 (anonymous namespace)::OperationParser::parseBlockBody(mlir::Block*) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2440:9
#<!-- -->47 0x000055ef94871b8a (anonymous namespace)::OperationParser::parseBlock(mlir::Block*&amp;) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2370:12
#<!-- -->48 0x000055ef94871753 (anonymous namespace)::OperationParser::parseRegionBody(mlir::Region&amp;, llvm::SMLoc, llvm::ArrayRef&lt;mlir::OpAsmParser::Argument&gt;, bool) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2328:7
#<!-- -->49 0x000055ef9486ecc7 (anonymous namespace)::OperationParser::parseRegion(mlir::Region&amp;, llvm::ArrayRef&lt;mlir::OpAsmParser::Argument&gt;, bool) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2257:7
#<!-- -->50 0x000055ef9486c970 (anonymous namespace)::CustomOpAsmParser::parseRegion(mlir::Region&amp;, llvm::ArrayRef&lt;mlir::OpAsmParser::Argument&gt;, bool) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:1879:16
#<!-- -->51 0x000055ef97122aad mlir::ModuleOp::parse(mlir::OpAsmParser&amp;, mlir::OperationState&amp;) /projects/llvm-project/build_all/tools/mlir/include/mlir/IR/BuiltinOps.cpp.inc:304:14
#<!-- -->52 0x000055ef896cccbd llvm::ParseResult llvm::detail::UniqueFunctionBase&lt;llvm::ParseResult, mlir::OpAsmParser&amp;, mlir::OperationState&amp;&gt;::CallImpl&lt;llvm::ParseResult (*)(mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;(void*, mlir::OpAsmParser&amp;, mlir::OperationState&amp;) /projects/llvm-project/llvm/include/llvm/ADT/FunctionExtras.h:213:12
#<!-- -->53 0x000055ef9487b35f llvm::unique_function&lt;llvm::ParseResult (mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;::operator()(mlir::OpAsmParser&amp;, mlir::OperationState&amp;) /projects/llvm-project/llvm/include/llvm/ADT/FunctionExtras.h:365:12
#<!-- -->54 0x000055ef9487b305 llvm::ParseResult llvm::function_ref&lt;llvm::ParseResult (mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;::callback_fn&lt;llvm::unique_function&lt;llvm::ParseResult (mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;&gt;(long, mlir::OpAsmParser&amp;, mlir::OperationState&amp;) /projects/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->55 0x000055ef9487b2c9 llvm::function_ref&lt;llvm::ParseResult (mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;::operator()(mlir::OpAsmParser&amp;, mlir::OperationState&amp;) const /projects/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->56 0x000055ef9486ae2e (anonymous namespace)::CustomOpAsmParser::parseOperation(mlir::OperationState&amp;) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:1615:9
#<!-- -->57 0x000055ef94869760 (anonymous namespace)::OperationParser::parseCustomOperation(llvm::ArrayRef&lt;std::tuple&lt;llvm::StringRef, unsigned int, llvm::SMLoc&gt;&gt;) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2154:19
#<!-- -->58 0x000055ef94865923 (anonymous namespace)::OperationParser::parseOperation() /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:1268:8
#<!-- -->59 0x000055ef9486536d (anonymous namespace)::TopLevelOperationParser::parse(mlir::Block*, mlir::Location) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2870:20
#<!-- -->60 0x000055ef9486520e mlir::parseAsmSourceFile(llvm::SourceMgr const&amp;, mlir::Block*, mlir::ParserConfig const&amp;, mlir::AsmParserState*, mlir::AsmParserCodeCompleteContext*) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2930:41
#<!-- -->61 0x000055ef9483634f mlir::parseSourceFile(std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::Block*, mlir::ParserConfig const&amp;, mlir::LocationAttr*) /projects/llvm-project/mlir/lib/Parser/Parser.cpp:64:10
#<!-- -->62 0x000055ef8947ec0f mlir::OwningOpRef&lt;mlir::ModuleOp&gt; mlir::detail::parseSourceFile&lt;mlir::ModuleOp, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;&gt;(mlir::ParserConfig const&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;) /projects/llvm-project/mlir/include/mlir/Parser/Parser.h:158:14
#<!-- -->63 0x000055ef8947eb18 mlir::OwningOpRef&lt;mlir::ModuleOp&gt; mlir::parseSourceFile&lt;mlir::ModuleOp&gt;(std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::ParserConfig const&amp;) /projects/llvm-project/mlir/include/mlir/Parser/Parser.h:188:10
#<!-- -->64 0x000055ef8947ddbd mlir::parseSourceFileForTool(std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::ParserConfig const&amp;, bool) /projects/llvm-project/mlir/include/mlir/Tools/ParseUtilities.h:31:12
#<!-- -->65 0x000055ef89464d6a performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) /projects/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:523:16
#<!-- -->66 0x000055ef89464b71 processBuffer(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&amp;, mlir::DialectRegistry&amp;, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /projects/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:673:12
#<!-- -->67 0x000055ef8946482c mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, llvm::raw_ostream&amp;) const /projects/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:762:12
#<!-- -->68 0x000055ef89464736 llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) /projects/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->69 0x000055ef9726dcba llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) const /projects/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->70 0x000055ef9726d07a mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) /projects/llvm-project/mlir/lib/Support/ToolUtilities.cpp:30:12
#<!-- -->71 0x000055ef89460126 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) /projects/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:767:26
#<!-- -->72 0x000055ef89460525 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) /projects/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:813:14
#<!-- -->73 0x000055ef894606f8 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) /projects/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:829:10
#<!-- -->74 0x000055ef89339935 main /projects/llvm-project/mlir/tools/mlir-opt/mlir-opt.cpp:347:33
#<!-- -->75 0x00007fea4ba2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->76 0x00007fea4ba2a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->77 0x00007fea4ba2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->78 0x000055ef89339505 _start (/mnt/raid/projects/llvm-project/build_all/bin/mlir-opt+0x435c505)
Aborted
```

### Used Command
```bash
mlir-opt input.mlir
```
</details>


---

