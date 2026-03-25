# [MLIR][Parser] `getLineNumber` crashed in `parseRegionBody` call from GPU dialect

**Author:** RJerrica
**URL:** https://github.com/llvm/llvm-project/issues/176426
**Status:** Closed
**Labels:** mlir:gpu, crash
**Closed Date:** 2026-02-10T03:16:44Z

## Body

Hi, I found a crash bug when using gpu dialect, please see the following input program.

### Input Program
```llvm
module {
  func.func @elementwise_add_f32_host_launch(%a: memref<?xf32>, %b: memref<?xf32>, %c: memref<?xf32>, %N: i32) {
    %c0 = arith.constant 0 : index
    %c1 = arith.constant 1 : index
    %c256 = arith.constant 256 : index
    %N_idx = arith.index_cast %N : i32 to index
    %grid_dim = arith.ceildivui %N_idx, %c256 : index
    %block_dim = arith.constant 256 : index
    gpu.launch blocks(%bx) in (%grid_x = %grid_dim, %grid_y = %c1, %grid_z = %c1)
                threads(%tx) in (%block_x = %block_dim, %block_y = %c1, %block_z = %c1) {
      %bid_x = gpu.block_id x
      %tid_x = gpu.thread_id x
      %idx = arith.muli %bid_x, %block_dim : index
      %idx_0 = arith.addi %idx, %tid_x : index
      %cond = arith.cmpi slt, %idx_0, %N_idx : index
      scf.if %cond {
        %a_val = memref.load %a[%idx_0] : memref<?xf32>
        %b_val = memref.load %b[%idx_0] : memref<?xf32>
        %c_val = arith.addf %a_val, %b_val : f32
        memref.store %c_val, %c[%idx_0] : memref<?xf32>
      }
      gpu.terminator
    }
    return
  }
}
```

### Stack Trace
```bash
mlir-opt: /projects/llvm-project/llvm/lib/Support/SourceMgr.cpp:129: unsigned int llvm::SourceMgr::SrcBuffer::getLineNumberSpecialized(const char *) const [T = unsigned short]: Assertion `Ptr >= BufStart && Ptr <= Buffer->getBufferEnd()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: mlir-opt input.mlir
1.      MLIR Parser: custom op parser 'builtin.module'
2.      MLIR Parser: custom op parser 'func.func'
3.      MLIR Parser: custom op parser 'gpu.launch'
 #0 0x0000558d34ec261d llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /projects/llvm-project/llvm/lib/Support/Unix/Signals.inc:834:11
 #1 0x0000558d34ec2b4b PrintStackTraceSignalHandler(void*) /projects/llvm-project/llvm/lib/Support/Unix/Signals.inc:916:1
 #2 0x0000558d34ec0a1f llvm::sys::RunSignalHandlers() /projects/llvm-project/llvm/lib/Support/Signals.cpp:104:5
 #3 0x0000558d34ec31e9 SignalHandler(int, siginfo_t*, void*) /projects/llvm-project/llvm/lib/Support/Unix/Signals.inc:426:38
 #4 0x00007faa70e45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #5 0x00007faa70e9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #6 0x00007faa70e9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #7 0x00007faa70e9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #8 0x00007faa70e4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #9 0x00007faa70e288ff abort ./stdlib/abort.c:81:7
#10 0x00007faa70e2881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#11 0x00007faa70e3b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#12 0x0000558d34fb58ef unsigned int llvm::SourceMgr::SrcBuffer::getLineNumberSpecialized<unsigned short>(char const*) const /projects/llvm-project/llvm/lib/Support/SourceMgr.cpp:130:23
#13 0x0000558d34fb2aef llvm::SourceMgr::SrcBuffer::getLineNumber(char const*) const /projects/llvm-project/llvm/lib/Support/SourceMgr.cpp:147:5
#14 0x0000558d403be7fd mlir::Lexer::getEncodedSourceLocation(llvm::SMLoc) /projects/llvm-project/mlir/lib/AsmParser/Lexer.cpp:69:12
#15 0x0000558d403bb60d mlir::detail::Parser::getEncodedSourceLocation(llvm::SMLoc) /projects/llvm-project/mlir/lib/AsmParser/Parser.h:95:22
#16 0x0000558d403cf5c4 (anonymous namespace)::OperationParser::parseRegionBody(mlir::Region&, llvm::SMLoc, llvm::ArrayRef<mlir::OpAsmParser::Argument>, bool) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2315:28
#17 0x0000558d403cccc7 (anonymous namespace)::OperationParser::parseRegion(mlir::Region&, llvm::ArrayRef<mlir::OpAsmParser::Argument>, bool) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2257:7
#18 0x0000558d403ca970 (anonymous namespace)::CustomOpAsmParser::parseRegion(mlir::Region&, llvm::ArrayRef<mlir::OpAsmParser::Argument>, bool) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:1879:16
#19 0x0000558d40fbb7bb mlir::gpu::LaunchOp::parse(mlir::OpAsmParser&, mlir::OperationState&) /projects/llvm-project/mlir/lib/Dialect/GPU/IR/GPUDialect.cpp:1160:14
#20 0x0000558d3522acbd llvm::ParseResult llvm::detail::UniqueFunctionBase<llvm::ParseResult, mlir::OpAsmParser&, mlir::OperationState&>::CallImpl<llvm::ParseResult (*)(mlir::OpAsmParser&, mlir::OperationState&)>(void*, mlir::OpAsmParser&, mlir::OperationState&) /projects/llvm-project/llvm/include/llvm/ADT/FunctionExtras.h:213:12
#21 0x0000558d403d935f llvm::unique_function<llvm::ParseResult (mlir::OpAsmParser&, mlir::OperationState&)>::operator()(mlir::OpAsmParser&, mlir::OperationState&) /projects/llvm-project/llvm/include/llvm/ADT/FunctionExtras.h:365:12
#22 0x0000558d403d9305 llvm::ParseResult llvm::function_ref<llvm::ParseResult (mlir::OpAsmParser&, mlir::OperationState&)>::callback_fn<llvm::unique_function<llvm::ParseResult (mlir::OpAsmParser&, mlir::OperationState&)>>(long, mlir::OpAsmParser&, mlir::OperationState&) /projects/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#23 0x0000558d403d92c9 llvm::function_ref<llvm::ParseResult (mlir::OpAsmParser&, mlir::OperationState&)>::operator()(mlir::OpAsmParser&, mlir::OperationState&) const /projects/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#24 0x0000558d403c8e2e (anonymous namespace)::CustomOpAsmParser::parseOperation(mlir::OperationState&) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:1615:9
#25 0x0000558d403c7760 (anonymous namespace)::OperationParser::parseCustomOperation(llvm::ArrayRef<std::tuple<llvm::StringRef, unsigned int, llvm::SMLoc>>) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2154:19
#26 0x0000558d403c3923 (anonymous namespace)::OperationParser::parseOperation() /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:1268:8
#27 0x0000558d403d0ac2 (anonymous namespace)::OperationParser::parseBlockBody(mlir::Block*) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2440:9
#28 0x0000558d403cfb8a (anonymous namespace)::OperationParser::parseBlock(mlir::Block*&) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2370:12
#29 0x0000558d403cf753 (anonymous namespace)::OperationParser::parseRegionBody(mlir::Region&, llvm::SMLoc, llvm::ArrayRef<mlir::OpAsmParser::Argument>, bool) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2328:7
#30 0x0000558d403cccc7 (anonymous namespace)::OperationParser::parseRegion(mlir::Region&, llvm::ArrayRef<mlir::OpAsmParser::Argument>, bool) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2257:7
#31 0x0000558d403ca970 (anonymous namespace)::CustomOpAsmParser::parseRegion(mlir::Region&, llvm::ArrayRef<mlir::OpAsmParser::Argument>, bool) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:1879:16
#32 0x0000558d403caa54 (anonymous namespace)::CustomOpAsmParser::parseOptionalRegion(mlir::Region&, llvm::ArrayRef<mlir::OpAsmParser::Argument>, bool) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:1890:12
#33 0x0000558d42a9916c mlir::function_interface_impl::parseFunctionOp(mlir::OpAsmParser&, mlir::OperationState&, bool, mlir::StringAttr, llvm::function_ref<mlir::Type (mlir::Builder&, llvm::ArrayRef<mlir::Type>, llvm::ArrayRef<mlir::Type>, mlir::function_interface_impl::VariadicFlag, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>&)>, mlir::StringAttr, mlir::StringAttr) /projects/llvm-project/mlir/lib/Interfaces/FunctionImplementation.cpp:154:14
#34 0x0000558d409aa29e mlir::func::FuncOp::parse(mlir::OpAsmParser&, mlir::OperationState&) /projects/llvm-project/mlir/lib/Dialect/Func/IR/FuncOps.cpp:202:10
#35 0x0000558d3522acbd llvm::ParseResult llvm::detail::UniqueFunctionBase<llvm::ParseResult, mlir::OpAsmParser&, mlir::OperationState&>::CallImpl<llvm::ParseResult (*)(mlir::OpAsmParser&, mlir::OperationState&)>(void*, mlir::OpAsmParser&, mlir::OperationState&) /projects/llvm-project/llvm/include/llvm/ADT/FunctionExtras.h:213:12
#36 0x0000558d403d935f llvm::unique_function<llvm::ParseResult (mlir::OpAsmParser&, mlir::OperationState&)>::operator()(mlir::OpAsmParser&, mlir::OperationState&) /projects/llvm-project/llvm/include/llvm/ADT/FunctionExtras.h:365:12
#37 0x0000558d403d9305 llvm::ParseResult llvm::function_ref<llvm::ParseResult (mlir::OpAsmParser&, mlir::OperationState&)>::callback_fn<llvm::unique_function<llvm::ParseResult (mlir::OpAsmParser&, mlir::OperationState&)>>(long, mlir::OpAsmParser&, mlir::OperationState&) /projects/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#38 0x0000558d403d92c9 llvm::function_ref<llvm::ParseResult (mlir::OpAsmParser&, mlir::OperationState&)>::operator()(mlir::OpAsmParser&, mlir::OperationState&) const /projects/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#39 0x0000558d403c8e2e (anonymous namespace)::CustomOpAsmParser::parseOperation(mlir::OperationState&) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:1615:9
#40 0x0000558d403c7760 (anonymous namespace)::OperationParser::parseCustomOperation(llvm::ArrayRef<std::tuple<llvm::StringRef, unsigned int, llvm::SMLoc>>) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2154:19
#41 0x0000558d403c3923 (anonymous namespace)::OperationParser::parseOperation() /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:1268:8
#42 0x0000558d403d0ac2 (anonymous namespace)::OperationParser::parseBlockBody(mlir::Block*) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2440:9
#43 0x0000558d403cfb8a (anonymous namespace)::OperationParser::parseBlock(mlir::Block*&) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2370:12
#44 0x0000558d403cf753 (anonymous namespace)::OperationParser::parseRegionBody(mlir::Region&, llvm::SMLoc, llvm::ArrayRef<mlir::OpAsmParser::Argument>, bool) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2328:7
#45 0x0000558d403cccc7 (anonymous namespace)::OperationParser::parseRegion(mlir::Region&, llvm::ArrayRef<mlir::OpAsmParser::Argument>, bool) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2257:7
#46 0x0000558d403ca970 (anonymous namespace)::CustomOpAsmParser::parseRegion(mlir::Region&, llvm::ArrayRef<mlir::OpAsmParser::Argument>, bool) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:1879:16
#47 0x0000558d42c80aad mlir::ModuleOp::parse(mlir::OpAsmParser&, mlir::OperationState&) /projects/llvm-project/build_all/tools/mlir/include/mlir/IR/BuiltinOps.cpp.inc:304:14
#48 0x0000558d3522acbd llvm::ParseResult llvm::detail::UniqueFunctionBase<llvm::ParseResult, mlir::OpAsmParser&, mlir::OperationState&>::CallImpl<llvm::ParseResult (*)(mlir::OpAsmParser&, mlir::OperationState&)>(void*, mlir::OpAsmParser&, mlir::OperationState&) /projects/llvm-project/llvm/include/llvm/ADT/FunctionExtras.h:213:12
#49 0x0000558d403d935f llvm::unique_function<llvm::ParseResult (mlir::OpAsmParser&, mlir::OperationState&)>::operator()(mlir::OpAsmParser&, mlir::OperationState&) /projects/llvm-project/llvm/include/llvm/ADT/FunctionExtras.h:365:12
#50 0x0000558d403d9305 llvm::ParseResult llvm::function_ref<llvm::ParseResult (mlir::OpAsmParser&, mlir::OperationState&)>::callback_fn<llvm::unique_function<llvm::ParseResult (mlir::OpAsmParser&, mlir::OperationState&)>>(long, mlir::OpAsmParser&, mlir::OperationState&) /projects/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#51 0x0000558d403d92c9 llvm::function_ref<llvm::ParseResult (mlir::OpAsmParser&, mlir::OperationState&)>::operator()(mlir::OpAsmParser&, mlir::OperationState&) const /projects/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#52 0x0000558d403c8e2e (anonymous namespace)::CustomOpAsmParser::parseOperation(mlir::OperationState&) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:1615:9
#53 0x0000558d403c7760 (anonymous namespace)::OperationParser::parseCustomOperation(llvm::ArrayRef<std::tuple<llvm::StringRef, unsigned int, llvm::SMLoc>>) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2154:19
#54 0x0000558d403c3923 (anonymous namespace)::OperationParser::parseOperation() /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:1268:8
#55 0x0000558d403c336d (anonymous namespace)::TopLevelOperationParser::parse(mlir::Block*, mlir::Location) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2870:20
#56 0x0000558d403c320e mlir::parseAsmSourceFile(llvm::SourceMgr const&, mlir::Block*, mlir::ParserConfig const&, mlir::AsmParserState*, mlir::AsmParserCodeCompleteContext*) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2930:41
#57 0x0000558d4039434f mlir::parseSourceFile(std::shared_ptr<llvm::SourceMgr> const&, mlir::Block*, mlir::ParserConfig const&, mlir::LocationAttr*) /projects/llvm-project/mlir/lib/Parser/Parser.cpp:64:10
#58 0x0000558d34fdcc0f mlir::OwningOpRef<mlir::ModuleOp> mlir::detail::parseSourceFile<mlir::ModuleOp, std::shared_ptr<llvm::SourceMgr> const&>(mlir::ParserConfig const&, std::shared_ptr<llvm::SourceMgr> const&) /projects/llvm-project/mlir/include/mlir/Parser/Parser.h:158:14
#59 0x0000558d34fdcb18 mlir::OwningOpRef<mlir::ModuleOp> mlir::parseSourceFile<mlir::ModuleOp>(std::shared_ptr<llvm::SourceMgr> const&, mlir::ParserConfig const&) /projects/llvm-project/mlir/include/mlir/Parser/Parser.h:188:10
#60 0x0000558d34fdbdbd mlir::parseSourceFileForTool(std::shared_ptr<llvm::SourceMgr> const&, mlir::ParserConfig const&, bool) /projects/llvm-project/mlir/include/mlir/Tools/ParseUtilities.h:31:12
#61 0x0000558d34fc2d6a performActions(llvm::raw_ostream&, std::shared_ptr<llvm::SourceMgr> const&, mlir::MLIRContext*, mlir::MlirOptMainConfig const&) /projects/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:523:16
#62 0x0000558d34fc2b71 processBuffer(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&, mlir::DialectRegistry&, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /projects/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:673:12
#63 0x0000558d34fc282c mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, llvm::raw_ostream&) const /projects/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:762:12
#64 0x0000558d34fc2736 llvm::LogicalResult llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::callback_fn<mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) /projects/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#65 0x0000558d42dcbcba llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) const /projects/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#66 0x0000558d42dcb07a mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef) /projects/llvm-project/mlir/lib/Support/ToolUtilities.cpp:30:12
#67 0x0000558d34fbe126 mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&) /projects/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:767:26
#68 0x0000558d34fbe525 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&) /projects/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:813:14
#69 0x0000558d34fbe6f8 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&) /projects/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:829:10
#70 0x0000558d34e97935 main /projects/llvm-project/mlir/tools/mlir-opt/mlir-opt.cpp:347:33
#71 0x00007faa70e2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#72 0x00007faa70e2a28b call_init ./csu/../csu/libc-start.c:128:20
#73 0x00007faa70e2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#74 0x0000558d34e97505 _start (/mnt/raid/projects/llvm-project/build_all/bin/mlir-opt+0x435c505)
Aborted
```

I used the following command to reproduce the issue.

```bash
mlir-opt input.mlir
```

My MLIR version is:
```bash
mlir-opt --version
LLVM (http://llvm.org/):
  LLVM version 22.0.0git
  DEBUG build with assertions.
```


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: None (RJerrica)

<details>
Hi, I found a crash bug when using gpu dialect, please see the following input program.

### Input Program
```llvm
module {
  func.func @<!-- -->elementwise_add_f32_host_launch(%a: memref&lt;?xf32&gt;, %b: memref&lt;?xf32&gt;, %c: memref&lt;?xf32&gt;, %N: i32) {
    %c0 = arith.constant 0 : index
    %c1 = arith.constant 1 : index
    %c256 = arith.constant 256 : index
    %N_idx = arith.index_cast %N : i32 to index
    %grid_dim = arith.ceildivui %N_idx, %c256 : index
    %block_dim = arith.constant 256 : index
    gpu.launch blocks(%bx) in (%grid_x = %grid_dim, %grid_y = %c1, %grid_z = %c1)
                threads(%tx) in (%block_x = %block_dim, %block_y = %c1, %block_z = %c1) {
      %bid_x = gpu.block_id x
      %tid_x = gpu.thread_id x
      %idx = arith.muli %bid_x, %block_dim : index
      %idx_0 = arith.addi %idx, %tid_x : index
      %cond = arith.cmpi slt, %idx_0, %N_idx : index
      scf.if %cond {
        %a_val = memref.load %a[%idx_0] : memref&lt;?xf32&gt;
        %b_val = memref.load %b[%idx_0] : memref&lt;?xf32&gt;
        %c_val = arith.addf %a_val, %b_val : f32
        memref.store %c_val, %c[%idx_0] : memref&lt;?xf32&gt;
      }
      gpu.terminator
    }
    return
  }
}
```

### Stack Trace
```bash
mlir-opt: /projects/llvm-project/llvm/lib/Support/SourceMgr.cpp:129: unsigned int llvm::SourceMgr::SrcBuffer::getLineNumberSpecialized(const char *) const [T = unsigned short]: Assertion `Ptr &gt;= BufStart &amp;&amp; Ptr &lt;= Buffer-&gt;getBufferEnd()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: mlir-opt input.mlir
1.      MLIR Parser: custom op parser 'builtin.module'
2.      MLIR Parser: custom op parser 'func.func'
3.      MLIR Parser: custom op parser 'gpu.launch'
 #<!-- -->0 0x0000558d34ec261d llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /projects/llvm-project/llvm/lib/Support/Unix/Signals.inc:834:11
 #<!-- -->1 0x0000558d34ec2b4b PrintStackTraceSignalHandler(void*) /projects/llvm-project/llvm/lib/Support/Unix/Signals.inc:916:1
 #<!-- -->2 0x0000558d34ec0a1f llvm::sys::RunSignalHandlers() /projects/llvm-project/llvm/lib/Support/Signals.cpp:104:5
 #<!-- -->3 0x0000558d34ec31e9 SignalHandler(int, siginfo_t*, void*) /projects/llvm-project/llvm/lib/Support/Unix/Signals.inc:426:38
 #<!-- -->4 0x00007faa70e45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->5 0x00007faa70e9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->6 0x00007faa70e9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->7 0x00007faa70e9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->8 0x00007faa70e4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x00007faa70e288ff abort ./stdlib/abort.c:81:7
#<!-- -->10 0x00007faa70e2881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->11 0x00007faa70e3b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->12 0x0000558d34fb58ef unsigned int llvm::SourceMgr::SrcBuffer::getLineNumberSpecialized&lt;unsigned short&gt;(char const*) const /projects/llvm-project/llvm/lib/Support/SourceMgr.cpp:130:23
#<!-- -->13 0x0000558d34fb2aef llvm::SourceMgr::SrcBuffer::getLineNumber(char const*) const /projects/llvm-project/llvm/lib/Support/SourceMgr.cpp:147:5
#<!-- -->14 0x0000558d403be7fd mlir::Lexer::getEncodedSourceLocation(llvm::SMLoc) /projects/llvm-project/mlir/lib/AsmParser/Lexer.cpp:69:12
#<!-- -->15 0x0000558d403bb60d mlir::detail::Parser::getEncodedSourceLocation(llvm::SMLoc) /projects/llvm-project/mlir/lib/AsmParser/Parser.h:95:22
#<!-- -->16 0x0000558d403cf5c4 (anonymous namespace)::OperationParser::parseRegionBody(mlir::Region&amp;, llvm::SMLoc, llvm::ArrayRef&lt;mlir::OpAsmParser::Argument&gt;, bool) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2315:28
#<!-- -->17 0x0000558d403cccc7 (anonymous namespace)::OperationParser::parseRegion(mlir::Region&amp;, llvm::ArrayRef&lt;mlir::OpAsmParser::Argument&gt;, bool) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2257:7
#<!-- -->18 0x0000558d403ca970 (anonymous namespace)::CustomOpAsmParser::parseRegion(mlir::Region&amp;, llvm::ArrayRef&lt;mlir::OpAsmParser::Argument&gt;, bool) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:1879:16
#<!-- -->19 0x0000558d40fbb7bb mlir::gpu::LaunchOp::parse(mlir::OpAsmParser&amp;, mlir::OperationState&amp;) /projects/llvm-project/mlir/lib/Dialect/GPU/IR/GPUDialect.cpp:1160:14
#<!-- -->20 0x0000558d3522acbd llvm::ParseResult llvm::detail::UniqueFunctionBase&lt;llvm::ParseResult, mlir::OpAsmParser&amp;, mlir::OperationState&amp;&gt;::CallImpl&lt;llvm::ParseResult (*)(mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;(void*, mlir::OpAsmParser&amp;, mlir::OperationState&amp;) /projects/llvm-project/llvm/include/llvm/ADT/FunctionExtras.h:213:12
#<!-- -->21 0x0000558d403d935f llvm::unique_function&lt;llvm::ParseResult (mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;::operator()(mlir::OpAsmParser&amp;, mlir::OperationState&amp;) /projects/llvm-project/llvm/include/llvm/ADT/FunctionExtras.h:365:12
#<!-- -->22 0x0000558d403d9305 llvm::ParseResult llvm::function_ref&lt;llvm::ParseResult (mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;::callback_fn&lt;llvm::unique_function&lt;llvm::ParseResult (mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;&gt;(long, mlir::OpAsmParser&amp;, mlir::OperationState&amp;) /projects/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->23 0x0000558d403d92c9 llvm::function_ref&lt;llvm::ParseResult (mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;::operator()(mlir::OpAsmParser&amp;, mlir::OperationState&amp;) const /projects/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->24 0x0000558d403c8e2e (anonymous namespace)::CustomOpAsmParser::parseOperation(mlir::OperationState&amp;) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:1615:9
#<!-- -->25 0x0000558d403c7760 (anonymous namespace)::OperationParser::parseCustomOperation(llvm::ArrayRef&lt;std::tuple&lt;llvm::StringRef, unsigned int, llvm::SMLoc&gt;&gt;) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2154:19
#<!-- -->26 0x0000558d403c3923 (anonymous namespace)::OperationParser::parseOperation() /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:1268:8
#<!-- -->27 0x0000558d403d0ac2 (anonymous namespace)::OperationParser::parseBlockBody(mlir::Block*) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2440:9
#<!-- -->28 0x0000558d403cfb8a (anonymous namespace)::OperationParser::parseBlock(mlir::Block*&amp;) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2370:12
#<!-- -->29 0x0000558d403cf753 (anonymous namespace)::OperationParser::parseRegionBody(mlir::Region&amp;, llvm::SMLoc, llvm::ArrayRef&lt;mlir::OpAsmParser::Argument&gt;, bool) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2328:7
#<!-- -->30 0x0000558d403cccc7 (anonymous namespace)::OperationParser::parseRegion(mlir::Region&amp;, llvm::ArrayRef&lt;mlir::OpAsmParser::Argument&gt;, bool) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2257:7
#<!-- -->31 0x0000558d403ca970 (anonymous namespace)::CustomOpAsmParser::parseRegion(mlir::Region&amp;, llvm::ArrayRef&lt;mlir::OpAsmParser::Argument&gt;, bool) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:1879:16
#<!-- -->32 0x0000558d403caa54 (anonymous namespace)::CustomOpAsmParser::parseOptionalRegion(mlir::Region&amp;, llvm::ArrayRef&lt;mlir::OpAsmParser::Argument&gt;, bool) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:1890:12
#<!-- -->33 0x0000558d42a9916c mlir::function_interface_impl::parseFunctionOp(mlir::OpAsmParser&amp;, mlir::OperationState&amp;, bool, mlir::StringAttr, llvm::function_ref&lt;mlir::Type (mlir::Builder&amp;, llvm::ArrayRef&lt;mlir::Type&gt;, llvm::ArrayRef&lt;mlir::Type&gt;, mlir::function_interface_impl::VariadicFlag, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;&amp;)&gt;, mlir::StringAttr, mlir::StringAttr) /projects/llvm-project/mlir/lib/Interfaces/FunctionImplementation.cpp:154:14
#<!-- -->34 0x0000558d409aa29e mlir::func::FuncOp::parse(mlir::OpAsmParser&amp;, mlir::OperationState&amp;) /projects/llvm-project/mlir/lib/Dialect/Func/IR/FuncOps.cpp:202:10
#<!-- -->35 0x0000558d3522acbd llvm::ParseResult llvm::detail::UniqueFunctionBase&lt;llvm::ParseResult, mlir::OpAsmParser&amp;, mlir::OperationState&amp;&gt;::CallImpl&lt;llvm::ParseResult (*)(mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;(void*, mlir::OpAsmParser&amp;, mlir::OperationState&amp;) /projects/llvm-project/llvm/include/llvm/ADT/FunctionExtras.h:213:12
#<!-- -->36 0x0000558d403d935f llvm::unique_function&lt;llvm::ParseResult (mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;::operator()(mlir::OpAsmParser&amp;, mlir::OperationState&amp;) /projects/llvm-project/llvm/include/llvm/ADT/FunctionExtras.h:365:12
#<!-- -->37 0x0000558d403d9305 llvm::ParseResult llvm::function_ref&lt;llvm::ParseResult (mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;::callback_fn&lt;llvm::unique_function&lt;llvm::ParseResult (mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;&gt;(long, mlir::OpAsmParser&amp;, mlir::OperationState&amp;) /projects/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->38 0x0000558d403d92c9 llvm::function_ref&lt;llvm::ParseResult (mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;::operator()(mlir::OpAsmParser&amp;, mlir::OperationState&amp;) const /projects/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->39 0x0000558d403c8e2e (anonymous namespace)::CustomOpAsmParser::parseOperation(mlir::OperationState&amp;) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:1615:9
#<!-- -->40 0x0000558d403c7760 (anonymous namespace)::OperationParser::parseCustomOperation(llvm::ArrayRef&lt;std::tuple&lt;llvm::StringRef, unsigned int, llvm::SMLoc&gt;&gt;) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2154:19
#<!-- -->41 0x0000558d403c3923 (anonymous namespace)::OperationParser::parseOperation() /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:1268:8
#<!-- -->42 0x0000558d403d0ac2 (anonymous namespace)::OperationParser::parseBlockBody(mlir::Block*) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2440:9
#<!-- -->43 0x0000558d403cfb8a (anonymous namespace)::OperationParser::parseBlock(mlir::Block*&amp;) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2370:12
#<!-- -->44 0x0000558d403cf753 (anonymous namespace)::OperationParser::parseRegionBody(mlir::Region&amp;, llvm::SMLoc, llvm::ArrayRef&lt;mlir::OpAsmParser::Argument&gt;, bool) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2328:7
#<!-- -->45 0x0000558d403cccc7 (anonymous namespace)::OperationParser::parseRegion(mlir::Region&amp;, llvm::ArrayRef&lt;mlir::OpAsmParser::Argument&gt;, bool) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2257:7
#<!-- -->46 0x0000558d403ca970 (anonymous namespace)::CustomOpAsmParser::parseRegion(mlir::Region&amp;, llvm::ArrayRef&lt;mlir::OpAsmParser::Argument&gt;, bool) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:1879:16
#<!-- -->47 0x0000558d42c80aad mlir::ModuleOp::parse(mlir::OpAsmParser&amp;, mlir::OperationState&amp;) /projects/llvm-project/build_all/tools/mlir/include/mlir/IR/BuiltinOps.cpp.inc:304:14
#<!-- -->48 0x0000558d3522acbd llvm::ParseResult llvm::detail::UniqueFunctionBase&lt;llvm::ParseResult, mlir::OpAsmParser&amp;, mlir::OperationState&amp;&gt;::CallImpl&lt;llvm::ParseResult (*)(mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;(void*, mlir::OpAsmParser&amp;, mlir::OperationState&amp;) /projects/llvm-project/llvm/include/llvm/ADT/FunctionExtras.h:213:12
#<!-- -->49 0x0000558d403d935f llvm::unique_function&lt;llvm::ParseResult (mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;::operator()(mlir::OpAsmParser&amp;, mlir::OperationState&amp;) /projects/llvm-project/llvm/include/llvm/ADT/FunctionExtras.h:365:12
#<!-- -->50 0x0000558d403d9305 llvm::ParseResult llvm::function_ref&lt;llvm::ParseResult (mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;::callback_fn&lt;llvm::unique_function&lt;llvm::ParseResult (mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;&gt;(long, mlir::OpAsmParser&amp;, mlir::OperationState&amp;) /projects/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->51 0x0000558d403d92c9 llvm::function_ref&lt;llvm::ParseResult (mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;::operator()(mlir::OpAsmParser&amp;, mlir::OperationState&amp;) const /projects/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->52 0x0000558d403c8e2e (anonymous namespace)::CustomOpAsmParser::parseOperation(mlir::OperationState&amp;) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:1615:9
#<!-- -->53 0x0000558d403c7760 (anonymous namespace)::OperationParser::parseCustomOperation(llvm::ArrayRef&lt;std::tuple&lt;llvm::StringRef, unsigned int, llvm::SMLoc&gt;&gt;) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2154:19
#<!-- -->54 0x0000558d403c3923 (anonymous namespace)::OperationParser::parseOperation() /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:1268:8
#<!-- -->55 0x0000558d403c336d (anonymous namespace)::TopLevelOperationParser::parse(mlir::Block*, mlir::Location) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2870:20
#<!-- -->56 0x0000558d403c320e mlir::parseAsmSourceFile(llvm::SourceMgr const&amp;, mlir::Block*, mlir::ParserConfig const&amp;, mlir::AsmParserState*, mlir::AsmParserCodeCompleteContext*) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2930:41
#<!-- -->57 0x0000558d4039434f mlir::parseSourceFile(std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::Block*, mlir::ParserConfig const&amp;, mlir::LocationAttr*) /projects/llvm-project/mlir/lib/Parser/Parser.cpp:64:10
#<!-- -->58 0x0000558d34fdcc0f mlir::OwningOpRef&lt;mlir::ModuleOp&gt; mlir::detail::parseSourceFile&lt;mlir::ModuleOp, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;&gt;(mlir::ParserConfig const&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;) /projects/llvm-project/mlir/include/mlir/Parser/Parser.h:158:14
#<!-- -->59 0x0000558d34fdcb18 mlir::OwningOpRef&lt;mlir::ModuleOp&gt; mlir::parseSourceFile&lt;mlir::ModuleOp&gt;(std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::ParserConfig const&amp;) /projects/llvm-project/mlir/include/mlir/Parser/Parser.h:188:10
#<!-- -->60 0x0000558d34fdbdbd mlir::parseSourceFileForTool(std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::ParserConfig const&amp;, bool) /projects/llvm-project/mlir/include/mlir/Tools/ParseUtilities.h:31:12
#<!-- -->61 0x0000558d34fc2d6a performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) /projects/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:523:16
#<!-- -->62 0x0000558d34fc2b71 processBuffer(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&amp;, mlir::DialectRegistry&amp;, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /projects/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:673:12
#<!-- -->63 0x0000558d34fc282c mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, llvm::raw_ostream&amp;) const /projects/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:762:12
#<!-- -->64 0x0000558d34fc2736 llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) /projects/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->65 0x0000558d42dcbcba llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) const /projects/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->66 0x0000558d42dcb07a mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) /projects/llvm-project/mlir/lib/Support/ToolUtilities.cpp:30:12
#<!-- -->67 0x0000558d34fbe126 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) /projects/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:767:26
#<!-- -->68 0x0000558d34fbe525 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) /projects/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:813:14
#<!-- -->69 0x0000558d34fbe6f8 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) /projects/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:829:10
#<!-- -->70 0x0000558d34e97935 main /projects/llvm-project/mlir/tools/mlir-opt/mlir-opt.cpp:347:33
#<!-- -->71 0x00007faa70e2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->72 0x00007faa70e2a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->73 0x00007faa70e2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->74 0x0000558d34e97505 _start (/mnt/raid/projects/llvm-project/build_all/bin/mlir-opt+0x435c505)
Aborted
```

I used the following command to trigger it:
```bash
mlir-opt input.mlir
```

My MLIR version is:
```bash
mlir-opt --version
LLVM (http://llvm.org/):
  LLVM version 22.0.0git
  DEBUG build with assertions.
```

</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-mlir-core

Author: None (RJerrica)

<details>
Hi, I found a crash bug when using gpu dialect, please see the following input program.

### Input Program
```llvm
module {
  func.func @<!-- -->elementwise_add_f32_host_launch(%a: memref&lt;?xf32&gt;, %b: memref&lt;?xf32&gt;, %c: memref&lt;?xf32&gt;, %N: i32) {
    %c0 = arith.constant 0 : index
    %c1 = arith.constant 1 : index
    %c256 = arith.constant 256 : index
    %N_idx = arith.index_cast %N : i32 to index
    %grid_dim = arith.ceildivui %N_idx, %c256 : index
    %block_dim = arith.constant 256 : index
    gpu.launch blocks(%bx) in (%grid_x = %grid_dim, %grid_y = %c1, %grid_z = %c1)
                threads(%tx) in (%block_x = %block_dim, %block_y = %c1, %block_z = %c1) {
      %bid_x = gpu.block_id x
      %tid_x = gpu.thread_id x
      %idx = arith.muli %bid_x, %block_dim : index
      %idx_0 = arith.addi %idx, %tid_x : index
      %cond = arith.cmpi slt, %idx_0, %N_idx : index
      scf.if %cond {
        %a_val = memref.load %a[%idx_0] : memref&lt;?xf32&gt;
        %b_val = memref.load %b[%idx_0] : memref&lt;?xf32&gt;
        %c_val = arith.addf %a_val, %b_val : f32
        memref.store %c_val, %c[%idx_0] : memref&lt;?xf32&gt;
      }
      gpu.terminator
    }
    return
  }
}
```

### Stack Trace
```bash
mlir-opt: /projects/llvm-project/llvm/lib/Support/SourceMgr.cpp:129: unsigned int llvm::SourceMgr::SrcBuffer::getLineNumberSpecialized(const char *) const [T = unsigned short]: Assertion `Ptr &gt;= BufStart &amp;&amp; Ptr &lt;= Buffer-&gt;getBufferEnd()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: mlir-opt input.mlir
1.      MLIR Parser: custom op parser 'builtin.module'
2.      MLIR Parser: custom op parser 'func.func'
3.      MLIR Parser: custom op parser 'gpu.launch'
 #<!-- -->0 0x0000558d34ec261d llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /projects/llvm-project/llvm/lib/Support/Unix/Signals.inc:834:11
 #<!-- -->1 0x0000558d34ec2b4b PrintStackTraceSignalHandler(void*) /projects/llvm-project/llvm/lib/Support/Unix/Signals.inc:916:1
 #<!-- -->2 0x0000558d34ec0a1f llvm::sys::RunSignalHandlers() /projects/llvm-project/llvm/lib/Support/Signals.cpp:104:5
 #<!-- -->3 0x0000558d34ec31e9 SignalHandler(int, siginfo_t*, void*) /projects/llvm-project/llvm/lib/Support/Unix/Signals.inc:426:38
 #<!-- -->4 0x00007faa70e45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->5 0x00007faa70e9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->6 0x00007faa70e9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->7 0x00007faa70e9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->8 0x00007faa70e4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x00007faa70e288ff abort ./stdlib/abort.c:81:7
#<!-- -->10 0x00007faa70e2881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->11 0x00007faa70e3b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->12 0x0000558d34fb58ef unsigned int llvm::SourceMgr::SrcBuffer::getLineNumberSpecialized&lt;unsigned short&gt;(char const*) const /projects/llvm-project/llvm/lib/Support/SourceMgr.cpp:130:23
#<!-- -->13 0x0000558d34fb2aef llvm::SourceMgr::SrcBuffer::getLineNumber(char const*) const /projects/llvm-project/llvm/lib/Support/SourceMgr.cpp:147:5
#<!-- -->14 0x0000558d403be7fd mlir::Lexer::getEncodedSourceLocation(llvm::SMLoc) /projects/llvm-project/mlir/lib/AsmParser/Lexer.cpp:69:12
#<!-- -->15 0x0000558d403bb60d mlir::detail::Parser::getEncodedSourceLocation(llvm::SMLoc) /projects/llvm-project/mlir/lib/AsmParser/Parser.h:95:22
#<!-- -->16 0x0000558d403cf5c4 (anonymous namespace)::OperationParser::parseRegionBody(mlir::Region&amp;, llvm::SMLoc, llvm::ArrayRef&lt;mlir::OpAsmParser::Argument&gt;, bool) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2315:28
#<!-- -->17 0x0000558d403cccc7 (anonymous namespace)::OperationParser::parseRegion(mlir::Region&amp;, llvm::ArrayRef&lt;mlir::OpAsmParser::Argument&gt;, bool) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2257:7
#<!-- -->18 0x0000558d403ca970 (anonymous namespace)::CustomOpAsmParser::parseRegion(mlir::Region&amp;, llvm::ArrayRef&lt;mlir::OpAsmParser::Argument&gt;, bool) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:1879:16
#<!-- -->19 0x0000558d40fbb7bb mlir::gpu::LaunchOp::parse(mlir::OpAsmParser&amp;, mlir::OperationState&amp;) /projects/llvm-project/mlir/lib/Dialect/GPU/IR/GPUDialect.cpp:1160:14
#<!-- -->20 0x0000558d3522acbd llvm::ParseResult llvm::detail::UniqueFunctionBase&lt;llvm::ParseResult, mlir::OpAsmParser&amp;, mlir::OperationState&amp;&gt;::CallImpl&lt;llvm::ParseResult (*)(mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;(void*, mlir::OpAsmParser&amp;, mlir::OperationState&amp;) /projects/llvm-project/llvm/include/llvm/ADT/FunctionExtras.h:213:12
#<!-- -->21 0x0000558d403d935f llvm::unique_function&lt;llvm::ParseResult (mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;::operator()(mlir::OpAsmParser&amp;, mlir::OperationState&amp;) /projects/llvm-project/llvm/include/llvm/ADT/FunctionExtras.h:365:12
#<!-- -->22 0x0000558d403d9305 llvm::ParseResult llvm::function_ref&lt;llvm::ParseResult (mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;::callback_fn&lt;llvm::unique_function&lt;llvm::ParseResult (mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;&gt;(long, mlir::OpAsmParser&amp;, mlir::OperationState&amp;) /projects/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->23 0x0000558d403d92c9 llvm::function_ref&lt;llvm::ParseResult (mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;::operator()(mlir::OpAsmParser&amp;, mlir::OperationState&amp;) const /projects/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->24 0x0000558d403c8e2e (anonymous namespace)::CustomOpAsmParser::parseOperation(mlir::OperationState&amp;) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:1615:9
#<!-- -->25 0x0000558d403c7760 (anonymous namespace)::OperationParser::parseCustomOperation(llvm::ArrayRef&lt;std::tuple&lt;llvm::StringRef, unsigned int, llvm::SMLoc&gt;&gt;) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2154:19
#<!-- -->26 0x0000558d403c3923 (anonymous namespace)::OperationParser::parseOperation() /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:1268:8
#<!-- -->27 0x0000558d403d0ac2 (anonymous namespace)::OperationParser::parseBlockBody(mlir::Block*) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2440:9
#<!-- -->28 0x0000558d403cfb8a (anonymous namespace)::OperationParser::parseBlock(mlir::Block*&amp;) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2370:12
#<!-- -->29 0x0000558d403cf753 (anonymous namespace)::OperationParser::parseRegionBody(mlir::Region&amp;, llvm::SMLoc, llvm::ArrayRef&lt;mlir::OpAsmParser::Argument&gt;, bool) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2328:7
#<!-- -->30 0x0000558d403cccc7 (anonymous namespace)::OperationParser::parseRegion(mlir::Region&amp;, llvm::ArrayRef&lt;mlir::OpAsmParser::Argument&gt;, bool) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2257:7
#<!-- -->31 0x0000558d403ca970 (anonymous namespace)::CustomOpAsmParser::parseRegion(mlir::Region&amp;, llvm::ArrayRef&lt;mlir::OpAsmParser::Argument&gt;, bool) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:1879:16
#<!-- -->32 0x0000558d403caa54 (anonymous namespace)::CustomOpAsmParser::parseOptionalRegion(mlir::Region&amp;, llvm::ArrayRef&lt;mlir::OpAsmParser::Argument&gt;, bool) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:1890:12
#<!-- -->33 0x0000558d42a9916c mlir::function_interface_impl::parseFunctionOp(mlir::OpAsmParser&amp;, mlir::OperationState&amp;, bool, mlir::StringAttr, llvm::function_ref&lt;mlir::Type (mlir::Builder&amp;, llvm::ArrayRef&lt;mlir::Type&gt;, llvm::ArrayRef&lt;mlir::Type&gt;, mlir::function_interface_impl::VariadicFlag, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;&amp;)&gt;, mlir::StringAttr, mlir::StringAttr) /projects/llvm-project/mlir/lib/Interfaces/FunctionImplementation.cpp:154:14
#<!-- -->34 0x0000558d409aa29e mlir::func::FuncOp::parse(mlir::OpAsmParser&amp;, mlir::OperationState&amp;) /projects/llvm-project/mlir/lib/Dialect/Func/IR/FuncOps.cpp:202:10
#<!-- -->35 0x0000558d3522acbd llvm::ParseResult llvm::detail::UniqueFunctionBase&lt;llvm::ParseResult, mlir::OpAsmParser&amp;, mlir::OperationState&amp;&gt;::CallImpl&lt;llvm::ParseResult (*)(mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;(void*, mlir::OpAsmParser&amp;, mlir::OperationState&amp;) /projects/llvm-project/llvm/include/llvm/ADT/FunctionExtras.h:213:12
#<!-- -->36 0x0000558d403d935f llvm::unique_function&lt;llvm::ParseResult (mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;::operator()(mlir::OpAsmParser&amp;, mlir::OperationState&amp;) /projects/llvm-project/llvm/include/llvm/ADT/FunctionExtras.h:365:12
#<!-- -->37 0x0000558d403d9305 llvm::ParseResult llvm::function_ref&lt;llvm::ParseResult (mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;::callback_fn&lt;llvm::unique_function&lt;llvm::ParseResult (mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;&gt;(long, mlir::OpAsmParser&amp;, mlir::OperationState&amp;) /projects/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->38 0x0000558d403d92c9 llvm::function_ref&lt;llvm::ParseResult (mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;::operator()(mlir::OpAsmParser&amp;, mlir::OperationState&amp;) const /projects/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->39 0x0000558d403c8e2e (anonymous namespace)::CustomOpAsmParser::parseOperation(mlir::OperationState&amp;) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:1615:9
#<!-- -->40 0x0000558d403c7760 (anonymous namespace)::OperationParser::parseCustomOperation(llvm::ArrayRef&lt;std::tuple&lt;llvm::StringRef, unsigned int, llvm::SMLoc&gt;&gt;) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2154:19
#<!-- -->41 0x0000558d403c3923 (anonymous namespace)::OperationParser::parseOperation() /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:1268:8
#<!-- -->42 0x0000558d403d0ac2 (anonymous namespace)::OperationParser::parseBlockBody(mlir::Block*) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2440:9
#<!-- -->43 0x0000558d403cfb8a (anonymous namespace)::OperationParser::parseBlock(mlir::Block*&amp;) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2370:12
#<!-- -->44 0x0000558d403cf753 (anonymous namespace)::OperationParser::parseRegionBody(mlir::Region&amp;, llvm::SMLoc, llvm::ArrayRef&lt;mlir::OpAsmParser::Argument&gt;, bool) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2328:7
#<!-- -->45 0x0000558d403cccc7 (anonymous namespace)::OperationParser::parseRegion(mlir::Region&amp;, llvm::ArrayRef&lt;mlir::OpAsmParser::Argument&gt;, bool) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2257:7
#<!-- -->46 0x0000558d403ca970 (anonymous namespace)::CustomOpAsmParser::parseRegion(mlir::Region&amp;, llvm::ArrayRef&lt;mlir::OpAsmParser::Argument&gt;, bool) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:1879:16
#<!-- -->47 0x0000558d42c80aad mlir::ModuleOp::parse(mlir::OpAsmParser&amp;, mlir::OperationState&amp;) /projects/llvm-project/build_all/tools/mlir/include/mlir/IR/BuiltinOps.cpp.inc:304:14
#<!-- -->48 0x0000558d3522acbd llvm::ParseResult llvm::detail::UniqueFunctionBase&lt;llvm::ParseResult, mlir::OpAsmParser&amp;, mlir::OperationState&amp;&gt;::CallImpl&lt;llvm::ParseResult (*)(mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;(void*, mlir::OpAsmParser&amp;, mlir::OperationState&amp;) /projects/llvm-project/llvm/include/llvm/ADT/FunctionExtras.h:213:12
#<!-- -->49 0x0000558d403d935f llvm::unique_function&lt;llvm::ParseResult (mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;::operator()(mlir::OpAsmParser&amp;, mlir::OperationState&amp;) /projects/llvm-project/llvm/include/llvm/ADT/FunctionExtras.h:365:12
#<!-- -->50 0x0000558d403d9305 llvm::ParseResult llvm::function_ref&lt;llvm::ParseResult (mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;::callback_fn&lt;llvm::unique_function&lt;llvm::ParseResult (mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;&gt;(long, mlir::OpAsmParser&amp;, mlir::OperationState&amp;) /projects/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->51 0x0000558d403d92c9 llvm::function_ref&lt;llvm::ParseResult (mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;::operator()(mlir::OpAsmParser&amp;, mlir::OperationState&amp;) const /projects/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->52 0x0000558d403c8e2e (anonymous namespace)::CustomOpAsmParser::parseOperation(mlir::OperationState&amp;) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:1615:9
#<!-- -->53 0x0000558d403c7760 (anonymous namespace)::OperationParser::parseCustomOperation(llvm::ArrayRef&lt;std::tuple&lt;llvm::StringRef, unsigned int, llvm::SMLoc&gt;&gt;) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2154:19
#<!-- -->54 0x0000558d403c3923 (anonymous namespace)::OperationParser::parseOperation() /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:1268:8
#<!-- -->55 0x0000558d403c336d (anonymous namespace)::TopLevelOperationParser::parse(mlir::Block*, mlir::Location) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2870:20
#<!-- -->56 0x0000558d403c320e mlir::parseAsmSourceFile(llvm::SourceMgr const&amp;, mlir::Block*, mlir::ParserConfig const&amp;, mlir::AsmParserState*, mlir::AsmParserCodeCompleteContext*) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2930:41
#<!-- -->57 0x0000558d4039434f mlir::parseSourceFile(std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::Block*, mlir::ParserConfig const&amp;, mlir::LocationAttr*) /projects/llvm-project/mlir/lib/Parser/Parser.cpp:64:10
#<!-- -->58 0x0000558d34fdcc0f mlir::OwningOpRef&lt;mlir::ModuleOp&gt; mlir::detail::parseSourceFile&lt;mlir::ModuleOp, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;&gt;(mlir::ParserConfig const&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;) /projects/llvm-project/mlir/include/mlir/Parser/Parser.h:158:14
#<!-- -->59 0x0000558d34fdcb18 mlir::OwningOpRef&lt;mlir::ModuleOp&gt; mlir::parseSourceFile&lt;mlir::ModuleOp&gt;(std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::ParserConfig const&amp;) /projects/llvm-project/mlir/include/mlir/Parser/Parser.h:188:10
#<!-- -->60 0x0000558d34fdbdbd mlir::parseSourceFileForTool(std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::ParserConfig const&amp;, bool) /projects/llvm-project/mlir/include/mlir/Tools/ParseUtilities.h:31:12
#<!-- -->61 0x0000558d34fc2d6a performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) /projects/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:523:16
#<!-- -->62 0x0000558d34fc2b71 processBuffer(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&amp;, mlir::DialectRegistry&amp;, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /projects/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:673:12
#<!-- -->63 0x0000558d34fc282c mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, llvm::raw_ostream&amp;) const /projects/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:762:12
#<!-- -->64 0x0000558d34fc2736 llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) /projects/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->65 0x0000558d42dcbcba llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) const /projects/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->66 0x0000558d42dcb07a mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) /projects/llvm-project/mlir/lib/Support/ToolUtilities.cpp:30:12
#<!-- -->67 0x0000558d34fbe126 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) /projects/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:767:26
#<!-- -->68 0x0000558d34fbe525 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) /projects/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:813:14
#<!-- -->69 0x0000558d34fbe6f8 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) /projects/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:829:10
#<!-- -->70 0x0000558d34e97935 main /projects/llvm-project/mlir/tools/mlir-opt/mlir-opt.cpp:347:33
#<!-- -->71 0x00007faa70e2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->72 0x00007faa70e2a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->73 0x00007faa70e2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->74 0x0000558d34e97505 _start (/mnt/raid/projects/llvm-project/build_all/bin/mlir-opt+0x435c505)
Aborted
```

I used the following command to reproduce the issue.

```bash
mlir-opt input.mlir
```

My MLIR version is:
```bash
mlir-opt --version
LLVM (http://llvm.org/):
  LLVM version 22.0.0git
  DEBUG build with assertions.
```

</details>


---

### Comment 3 - llvmbot


@llvm/issue-subscribers-mlir-gpu

Author: None (RJerrica)

<details>
Hi, I found a crash bug when using gpu dialect, please see the following input program.

### Input Program
```llvm
module {
  func.func @<!-- -->elementwise_add_f32_host_launch(%a: memref&lt;?xf32&gt;, %b: memref&lt;?xf32&gt;, %c: memref&lt;?xf32&gt;, %N: i32) {
    %c0 = arith.constant 0 : index
    %c1 = arith.constant 1 : index
    %c256 = arith.constant 256 : index
    %N_idx = arith.index_cast %N : i32 to index
    %grid_dim = arith.ceildivui %N_idx, %c256 : index
    %block_dim = arith.constant 256 : index
    gpu.launch blocks(%bx) in (%grid_x = %grid_dim, %grid_y = %c1, %grid_z = %c1)
                threads(%tx) in (%block_x = %block_dim, %block_y = %c1, %block_z = %c1) {
      %bid_x = gpu.block_id x
      %tid_x = gpu.thread_id x
      %idx = arith.muli %bid_x, %block_dim : index
      %idx_0 = arith.addi %idx, %tid_x : index
      %cond = arith.cmpi slt, %idx_0, %N_idx : index
      scf.if %cond {
        %a_val = memref.load %a[%idx_0] : memref&lt;?xf32&gt;
        %b_val = memref.load %b[%idx_0] : memref&lt;?xf32&gt;
        %c_val = arith.addf %a_val, %b_val : f32
        memref.store %c_val, %c[%idx_0] : memref&lt;?xf32&gt;
      }
      gpu.terminator
    }
    return
  }
}
```

### Stack Trace
```bash
mlir-opt: /projects/llvm-project/llvm/lib/Support/SourceMgr.cpp:129: unsigned int llvm::SourceMgr::SrcBuffer::getLineNumberSpecialized(const char *) const [T = unsigned short]: Assertion `Ptr &gt;= BufStart &amp;&amp; Ptr &lt;= Buffer-&gt;getBufferEnd()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: mlir-opt input.mlir
1.      MLIR Parser: custom op parser 'builtin.module'
2.      MLIR Parser: custom op parser 'func.func'
3.      MLIR Parser: custom op parser 'gpu.launch'
 #<!-- -->0 0x0000558d34ec261d llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /projects/llvm-project/llvm/lib/Support/Unix/Signals.inc:834:11
 #<!-- -->1 0x0000558d34ec2b4b PrintStackTraceSignalHandler(void*) /projects/llvm-project/llvm/lib/Support/Unix/Signals.inc:916:1
 #<!-- -->2 0x0000558d34ec0a1f llvm::sys::RunSignalHandlers() /projects/llvm-project/llvm/lib/Support/Signals.cpp:104:5
 #<!-- -->3 0x0000558d34ec31e9 SignalHandler(int, siginfo_t*, void*) /projects/llvm-project/llvm/lib/Support/Unix/Signals.inc:426:38
 #<!-- -->4 0x00007faa70e45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->5 0x00007faa70e9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->6 0x00007faa70e9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->7 0x00007faa70e9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->8 0x00007faa70e4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x00007faa70e288ff abort ./stdlib/abort.c:81:7
#<!-- -->10 0x00007faa70e2881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->11 0x00007faa70e3b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->12 0x0000558d34fb58ef unsigned int llvm::SourceMgr::SrcBuffer::getLineNumberSpecialized&lt;unsigned short&gt;(char const*) const /projects/llvm-project/llvm/lib/Support/SourceMgr.cpp:130:23
#<!-- -->13 0x0000558d34fb2aef llvm::SourceMgr::SrcBuffer::getLineNumber(char const*) const /projects/llvm-project/llvm/lib/Support/SourceMgr.cpp:147:5
#<!-- -->14 0x0000558d403be7fd mlir::Lexer::getEncodedSourceLocation(llvm::SMLoc) /projects/llvm-project/mlir/lib/AsmParser/Lexer.cpp:69:12
#<!-- -->15 0x0000558d403bb60d mlir::detail::Parser::getEncodedSourceLocation(llvm::SMLoc) /projects/llvm-project/mlir/lib/AsmParser/Parser.h:95:22
#<!-- -->16 0x0000558d403cf5c4 (anonymous namespace)::OperationParser::parseRegionBody(mlir::Region&amp;, llvm::SMLoc, llvm::ArrayRef&lt;mlir::OpAsmParser::Argument&gt;, bool) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2315:28
#<!-- -->17 0x0000558d403cccc7 (anonymous namespace)::OperationParser::parseRegion(mlir::Region&amp;, llvm::ArrayRef&lt;mlir::OpAsmParser::Argument&gt;, bool) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2257:7
#<!-- -->18 0x0000558d403ca970 (anonymous namespace)::CustomOpAsmParser::parseRegion(mlir::Region&amp;, llvm::ArrayRef&lt;mlir::OpAsmParser::Argument&gt;, bool) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:1879:16
#<!-- -->19 0x0000558d40fbb7bb mlir::gpu::LaunchOp::parse(mlir::OpAsmParser&amp;, mlir::OperationState&amp;) /projects/llvm-project/mlir/lib/Dialect/GPU/IR/GPUDialect.cpp:1160:14
#<!-- -->20 0x0000558d3522acbd llvm::ParseResult llvm::detail::UniqueFunctionBase&lt;llvm::ParseResult, mlir::OpAsmParser&amp;, mlir::OperationState&amp;&gt;::CallImpl&lt;llvm::ParseResult (*)(mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;(void*, mlir::OpAsmParser&amp;, mlir::OperationState&amp;) /projects/llvm-project/llvm/include/llvm/ADT/FunctionExtras.h:213:12
#<!-- -->21 0x0000558d403d935f llvm::unique_function&lt;llvm::ParseResult (mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;::operator()(mlir::OpAsmParser&amp;, mlir::OperationState&amp;) /projects/llvm-project/llvm/include/llvm/ADT/FunctionExtras.h:365:12
#<!-- -->22 0x0000558d403d9305 llvm::ParseResult llvm::function_ref&lt;llvm::ParseResult (mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;::callback_fn&lt;llvm::unique_function&lt;llvm::ParseResult (mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;&gt;(long, mlir::OpAsmParser&amp;, mlir::OperationState&amp;) /projects/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->23 0x0000558d403d92c9 llvm::function_ref&lt;llvm::ParseResult (mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;::operator()(mlir::OpAsmParser&amp;, mlir::OperationState&amp;) const /projects/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->24 0x0000558d403c8e2e (anonymous namespace)::CustomOpAsmParser::parseOperation(mlir::OperationState&amp;) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:1615:9
#<!-- -->25 0x0000558d403c7760 (anonymous namespace)::OperationParser::parseCustomOperation(llvm::ArrayRef&lt;std::tuple&lt;llvm::StringRef, unsigned int, llvm::SMLoc&gt;&gt;) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2154:19
#<!-- -->26 0x0000558d403c3923 (anonymous namespace)::OperationParser::parseOperation() /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:1268:8
#<!-- -->27 0x0000558d403d0ac2 (anonymous namespace)::OperationParser::parseBlockBody(mlir::Block*) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2440:9
#<!-- -->28 0x0000558d403cfb8a (anonymous namespace)::OperationParser::parseBlock(mlir::Block*&amp;) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2370:12
#<!-- -->29 0x0000558d403cf753 (anonymous namespace)::OperationParser::parseRegionBody(mlir::Region&amp;, llvm::SMLoc, llvm::ArrayRef&lt;mlir::OpAsmParser::Argument&gt;, bool) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2328:7
#<!-- -->30 0x0000558d403cccc7 (anonymous namespace)::OperationParser::parseRegion(mlir::Region&amp;, llvm::ArrayRef&lt;mlir::OpAsmParser::Argument&gt;, bool) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2257:7
#<!-- -->31 0x0000558d403ca970 (anonymous namespace)::CustomOpAsmParser::parseRegion(mlir::Region&amp;, llvm::ArrayRef&lt;mlir::OpAsmParser::Argument&gt;, bool) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:1879:16
#<!-- -->32 0x0000558d403caa54 (anonymous namespace)::CustomOpAsmParser::parseOptionalRegion(mlir::Region&amp;, llvm::ArrayRef&lt;mlir::OpAsmParser::Argument&gt;, bool) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:1890:12
#<!-- -->33 0x0000558d42a9916c mlir::function_interface_impl::parseFunctionOp(mlir::OpAsmParser&amp;, mlir::OperationState&amp;, bool, mlir::StringAttr, llvm::function_ref&lt;mlir::Type (mlir::Builder&amp;, llvm::ArrayRef&lt;mlir::Type&gt;, llvm::ArrayRef&lt;mlir::Type&gt;, mlir::function_interface_impl::VariadicFlag, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;&amp;)&gt;, mlir::StringAttr, mlir::StringAttr) /projects/llvm-project/mlir/lib/Interfaces/FunctionImplementation.cpp:154:14
#<!-- -->34 0x0000558d409aa29e mlir::func::FuncOp::parse(mlir::OpAsmParser&amp;, mlir::OperationState&amp;) /projects/llvm-project/mlir/lib/Dialect/Func/IR/FuncOps.cpp:202:10
#<!-- -->35 0x0000558d3522acbd llvm::ParseResult llvm::detail::UniqueFunctionBase&lt;llvm::ParseResult, mlir::OpAsmParser&amp;, mlir::OperationState&amp;&gt;::CallImpl&lt;llvm::ParseResult (*)(mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;(void*, mlir::OpAsmParser&amp;, mlir::OperationState&amp;) /projects/llvm-project/llvm/include/llvm/ADT/FunctionExtras.h:213:12
#<!-- -->36 0x0000558d403d935f llvm::unique_function&lt;llvm::ParseResult (mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;::operator()(mlir::OpAsmParser&amp;, mlir::OperationState&amp;) /projects/llvm-project/llvm/include/llvm/ADT/FunctionExtras.h:365:12
#<!-- -->37 0x0000558d403d9305 llvm::ParseResult llvm::function_ref&lt;llvm::ParseResult (mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;::callback_fn&lt;llvm::unique_function&lt;llvm::ParseResult (mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;&gt;(long, mlir::OpAsmParser&amp;, mlir::OperationState&amp;) /projects/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->38 0x0000558d403d92c9 llvm::function_ref&lt;llvm::ParseResult (mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;::operator()(mlir::OpAsmParser&amp;, mlir::OperationState&amp;) const /projects/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->39 0x0000558d403c8e2e (anonymous namespace)::CustomOpAsmParser::parseOperation(mlir::OperationState&amp;) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:1615:9
#<!-- -->40 0x0000558d403c7760 (anonymous namespace)::OperationParser::parseCustomOperation(llvm::ArrayRef&lt;std::tuple&lt;llvm::StringRef, unsigned int, llvm::SMLoc&gt;&gt;) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2154:19
#<!-- -->41 0x0000558d403c3923 (anonymous namespace)::OperationParser::parseOperation() /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:1268:8
#<!-- -->42 0x0000558d403d0ac2 (anonymous namespace)::OperationParser::parseBlockBody(mlir::Block*) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2440:9
#<!-- -->43 0x0000558d403cfb8a (anonymous namespace)::OperationParser::parseBlock(mlir::Block*&amp;) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2370:12
#<!-- -->44 0x0000558d403cf753 (anonymous namespace)::OperationParser::parseRegionBody(mlir::Region&amp;, llvm::SMLoc, llvm::ArrayRef&lt;mlir::OpAsmParser::Argument&gt;, bool) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2328:7
#<!-- -->45 0x0000558d403cccc7 (anonymous namespace)::OperationParser::parseRegion(mlir::Region&amp;, llvm::ArrayRef&lt;mlir::OpAsmParser::Argument&gt;, bool) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2257:7
#<!-- -->46 0x0000558d403ca970 (anonymous namespace)::CustomOpAsmParser::parseRegion(mlir::Region&amp;, llvm::ArrayRef&lt;mlir::OpAsmParser::Argument&gt;, bool) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:1879:16
#<!-- -->47 0x0000558d42c80aad mlir::ModuleOp::parse(mlir::OpAsmParser&amp;, mlir::OperationState&amp;) /projects/llvm-project/build_all/tools/mlir/include/mlir/IR/BuiltinOps.cpp.inc:304:14
#<!-- -->48 0x0000558d3522acbd llvm::ParseResult llvm::detail::UniqueFunctionBase&lt;llvm::ParseResult, mlir::OpAsmParser&amp;, mlir::OperationState&amp;&gt;::CallImpl&lt;llvm::ParseResult (*)(mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;(void*, mlir::OpAsmParser&amp;, mlir::OperationState&amp;) /projects/llvm-project/llvm/include/llvm/ADT/FunctionExtras.h:213:12
#<!-- -->49 0x0000558d403d935f llvm::unique_function&lt;llvm::ParseResult (mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;::operator()(mlir::OpAsmParser&amp;, mlir::OperationState&amp;) /projects/llvm-project/llvm/include/llvm/ADT/FunctionExtras.h:365:12
#<!-- -->50 0x0000558d403d9305 llvm::ParseResult llvm::function_ref&lt;llvm::ParseResult (mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;::callback_fn&lt;llvm::unique_function&lt;llvm::ParseResult (mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;&gt;(long, mlir::OpAsmParser&amp;, mlir::OperationState&amp;) /projects/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->51 0x0000558d403d92c9 llvm::function_ref&lt;llvm::ParseResult (mlir::OpAsmParser&amp;, mlir::OperationState&amp;)&gt;::operator()(mlir::OpAsmParser&amp;, mlir::OperationState&amp;) const /projects/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->52 0x0000558d403c8e2e (anonymous namespace)::CustomOpAsmParser::parseOperation(mlir::OperationState&amp;) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:1615:9
#<!-- -->53 0x0000558d403c7760 (anonymous namespace)::OperationParser::parseCustomOperation(llvm::ArrayRef&lt;std::tuple&lt;llvm::StringRef, unsigned int, llvm::SMLoc&gt;&gt;) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2154:19
#<!-- -->54 0x0000558d403c3923 (anonymous namespace)::OperationParser::parseOperation() /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:1268:8
#<!-- -->55 0x0000558d403c336d (anonymous namespace)::TopLevelOperationParser::parse(mlir::Block*, mlir::Location) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2870:20
#<!-- -->56 0x0000558d403c320e mlir::parseAsmSourceFile(llvm::SourceMgr const&amp;, mlir::Block*, mlir::ParserConfig const&amp;, mlir::AsmParserState*, mlir::AsmParserCodeCompleteContext*) /projects/llvm-project/mlir/lib/AsmParser/Parser.cpp:2930:41
#<!-- -->57 0x0000558d4039434f mlir::parseSourceFile(std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::Block*, mlir::ParserConfig const&amp;, mlir::LocationAttr*) /projects/llvm-project/mlir/lib/Parser/Parser.cpp:64:10
#<!-- -->58 0x0000558d34fdcc0f mlir::OwningOpRef&lt;mlir::ModuleOp&gt; mlir::detail::parseSourceFile&lt;mlir::ModuleOp, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;&gt;(mlir::ParserConfig const&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;) /projects/llvm-project/mlir/include/mlir/Parser/Parser.h:158:14
#<!-- -->59 0x0000558d34fdcb18 mlir::OwningOpRef&lt;mlir::ModuleOp&gt; mlir::parseSourceFile&lt;mlir::ModuleOp&gt;(std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::ParserConfig const&amp;) /projects/llvm-project/mlir/include/mlir/Parser/Parser.h:188:10
#<!-- -->60 0x0000558d34fdbdbd mlir::parseSourceFileForTool(std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::ParserConfig const&amp;, bool) /projects/llvm-project/mlir/include/mlir/Tools/ParseUtilities.h:31:12
#<!-- -->61 0x0000558d34fc2d6a performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) /projects/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:523:16
#<!-- -->62 0x0000558d34fc2b71 processBuffer(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&amp;, mlir::DialectRegistry&amp;, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /projects/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:673:12
#<!-- -->63 0x0000558d34fc282c mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, llvm::raw_ostream&amp;) const /projects/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:762:12
#<!-- -->64 0x0000558d34fc2736 llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) /projects/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->65 0x0000558d42dcbcba llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) const /projects/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->66 0x0000558d42dcb07a mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) /projects/llvm-project/mlir/lib/Support/ToolUtilities.cpp:30:12
#<!-- -->67 0x0000558d34fbe126 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) /projects/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:767:26
#<!-- -->68 0x0000558d34fbe525 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) /projects/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:813:14
#<!-- -->69 0x0000558d34fbe6f8 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) /projects/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:829:10
#<!-- -->70 0x0000558d34e97935 main /projects/llvm-project/mlir/tools/mlir-opt/mlir-opt.cpp:347:33
#<!-- -->71 0x00007faa70e2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->72 0x00007faa70e2a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->73 0x00007faa70e2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->74 0x0000558d34e97505 _start (/mnt/raid/projects/llvm-project/build_all/bin/mlir-opt+0x435c505)
Aborted
```

I used the following command to reproduce the issue.

```bash
mlir-opt input.mlir
```

My MLIR version is:
```bash
mlir-opt --version
LLVM (http://llvm.org/):
  LLVM version 22.0.0git
  DEBUG build with assertions.
```

</details>


---

