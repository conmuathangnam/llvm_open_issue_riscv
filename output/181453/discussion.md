# mlir-opt crash: Null pointer dereference in acc.getdeviceptr custom parser

**Author:** mlirBug
**URL:** https://github.com/llvm/llvm-project/issues/181453
**Status:** Closed
**Labels:** crash, mlir:openacc
**Closed Date:** 2026-02-27T14:57:06Z

## Body

Bug Op: acc.getdeviceptr

Bug Pass: N/A (Crash occurs during Parsing)

Detailed Description:
The mlir-opt tool crashes while parsing the acc.getdeviceptr operation. The crash occurs in the helper function parseVarPtrType within OpenACC.cpp.

The parser expects to extract a valid Type from the varPtr argument. If the input IR is malformed or if the type resolution fails for any reason, the mlir::Type object remains null. The code then calls mlir::TypeAttr::get(type), which internally calls type.getContext(). Since the type is null, this causes a segmentation fault.
```mlir
module {
  func.func @main() {
    %0 = mpi.finalize : !mpi.retval
    %1 = complex.constant [0.000000e+00 : f32, 0.000000e+00 : f32] : complex<f32>
    // math.tanh does not support complex types; use a real-valued operand instead
    %real = complex.re %1 : complex<f32>
    %2 = math.tanh %real : f32
    %3 = spirv.FOrdEqual %2, %real : f32
    %4 = llvm.call_intrinsic "llvm.dso_local_equivalent"() : () -> !llvm.ptr
    %5 = acc.getdeviceptr varPtr(%4 : !llvm.ptr) -> !llvm.ptr
    %6 = acc.declare_enter dataOperands(%5 : !llvm.ptr)
    return
  }
}
```
```
mlir-opt temp.mlir
```
```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llvmorg-22.1.0-install/bin/mlir-opt temp.mlir
1.      MLIR Parser: custom op parser 'builtin.module'
2.      MLIR Parser: custom op parser 'func.func'
3.      MLIR Parser: custom op parser 'acc.getdeviceptr'
 #0 0x0000602bc19b3e32 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #1 0x0000602bc19b05af llvm::sys::RunSignalHandlers() (llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #2 0x0000602bc19b06fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x00007b11b6c45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x0000602bc95fdfb7 mlir::Type::getContext() const (llvmorg-22.1.0-install/bin/mlir-opt+0xa179fb7)
 #5 0x0000602bc951aec8 mlir::TypeAttr::get(mlir::Type) (llvmorg-22.1.0-install/bin/mlir-opt+0xa096ec8)
 #6 0x0000602bc33b1396 parseVarPtrType(mlir::OpAsmParser&, mlir::Type&, mlir::TypeAttr&) (.constprop.0) OpenACC.cpp:0:0
 #7 0x0000602bc342d730 mlir::acc::GetDevicePtrOp::parse(mlir::OpAsmParser&, mlir::OperationState&) (llvmorg-22.1.0-install/bin/mlir-opt+0x3fa9730)
 #8 0x0000602bc7b4763d (anonymous namespace)::OperationParser::parseOperation() Parser.cpp:0:0
 #9 0x0000602bc7b4a5cf (anonymous namespace)::OperationParser::parseBlockBody(mlir::Block*) Parser.cpp:0:0
#10 0x0000602bc7b4adac (anonymous namespace)::OperationParser::parseRegionBody(mlir::Region&, llvm::SMLoc, llvm::ArrayRef<mlir::OpAsmParser::Argument>, bool) Parser.cpp:0:0
#11 0x0000602bc7b4b49d (anonymous namespace)::OperationParser::parseRegion(mlir::Region&, llvm::ArrayRef<mlir::OpAsmParser::Argument>, bool) Parser.cpp:0:0
#12 0x0000602bc7b4b545 (anonymous namespace)::CustomOpAsmParser::parseOptionalRegion(mlir::Region&, llvm::ArrayRef<mlir::OpAsmParser::Argument>, bool) Parser.cpp:0:0
#13 0x0000602bc93e00ef mlir::function_interface_impl::parseFunctionOp(mlir::OpAsmParser&, mlir::OperationState&, bool, mlir::StringAttr, llvm::function_ref<mlir::Type (mlir::Builder&, llvm::ArrayRef<mlir::Type>, llvm::ArrayRef<mlir::Type>, mlir::function_interface_impl::VariadicFlag, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>&)>, mlir::StringAttr, mlir::StringAttr) (llvmorg-22.1.0-install/bin/mlir-opt+0x9f5c0ef)
#14 0x0000602bc7f53016 mlir::func::FuncOp::parse(mlir::OpAsmParser&, mlir::OperationState&) (llvmorg-22.1.0-install/bin/mlir-opt+0x8acf016)
#15 0x0000602bc7b4763d (anonymous namespace)::OperationParser::parseOperation() Parser.cpp:0:0
#16 0x0000602bc7b4a5cf (anonymous namespace)::OperationParser::parseBlockBody(mlir::Block*) Parser.cpp:0:0
#17 0x0000602bc7b4adac (anonymous namespace)::OperationParser::parseRegionBody(mlir::Region&, llvm::SMLoc, llvm::ArrayRef<mlir::OpAsmParser::Argument>, bool) Parser.cpp:0:0
#18 0x0000602bc7b4b743 (anonymous namespace)::CustomOpAsmParser::parseRegion(mlir::Region&, llvm::ArrayRef<mlir::OpAsmParser::Argument>, bool) Parser.cpp:0:0
#19 0x0000602bc9541735 mlir::ModuleOp::parse(mlir::OpAsmParser&, mlir::OperationState&) (llvmorg-22.1.0-install/bin/mlir-opt+0xa0bd735)
#20 0x0000602bc7b4763d (anonymous namespace)::OperationParser::parseOperation() Parser.cpp:0:0
#21 0x0000602bc7b4933d mlir::parseAsmSourceFile(llvm::SourceMgr const&, mlir::Block*, mlir::ParserConfig const&, mlir::AsmParserState*, mlir::AsmParserCodeCompleteContext*) (llvmorg-22.1.0-install/bin/mlir-opt+0x86c533d)
#22 0x0000602bc1a79df7 performActions(llvm::raw_ostream&, std::shared_ptr<llvm::SourceMgr> const&, mlir::MLIRContext*, mlir::MlirOptMainConfig const&) MlirOptMain.cpp:0:0
#23 0x0000602bc1a7b4ea llvm::LogicalResult llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::callback_fn<mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::'lambda'(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, llvm::raw_ostream&)>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) MlirOptMain.cpp:0:0
#24 0x0000602bc9623731 mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef) (llvmorg-22.1.0-install/bin/mlir-opt+0xa19f731)
#25 0x0000602bc1a728d0 mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&) (.part.0) MlirOptMain.cpp:0:0
#26 0x0000602bc1a7bb70 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&) (llvmorg-22.1.0-install/bin/mlir-opt+0x25f7b70)
#27 0x0000602bc1a7bda9 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&) (llvmorg-22.1.0-install/bin/mlir-opt+0x25f7da9)
#28 0x0000602bc18ad023 main (llvmorg-22.1.0-install/bin/mlir-opt+0x2429023)
#29 0x00007b11b6c2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#30 0x00007b11b6c2a28b call_init ./csu/../csu/libc-start.c:128:20
#31 0x00007b11b6c2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#32 0x0000602bc1992405 _start (llvmorg-22.1.0-install/bin/mlir-opt+0x250e405)
Segmentation fault (core dumped)
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: None (mlirBug)

<details>
Bug Op: acc.getdeviceptr

Bug Pass: N/A (Crash occurs during Parsing)

Detailed Description:
The mlir-opt tool crashes while parsing the acc.getdeviceptr operation. The crash occurs in the helper function parseVarPtrType within OpenACC.cpp.

The parser expects to extract a valid Type from the varPtr argument. If the input IR is malformed or if the type resolution fails for any reason, the mlir::Type object remains null. The code then calls mlir::TypeAttr::get(type), which internally calls type.getContext(). Since the type is null, this causes a segmentation fault.
```mlir
module {
  func.func @<!-- -->main() {
    %0 = mpi.finalize : !mpi.retval
    %1 = complex.constant [0.000000e+00 : f32, 0.000000e+00 : f32] : complex&lt;f32&gt;
    // math.tanh does not support complex types; use a real-valued operand instead
    %real = complex.re %1 : complex&lt;f32&gt;
    %2 = math.tanh %real : f32
    %3 = spirv.FOrdEqual %2, %real : f32
    %4 = llvm.call_intrinsic "llvm.dso_local_equivalent"() : () -&gt; !llvm.ptr
    %5 = acc.getdeviceptr varPtr(%4 : !llvm.ptr) -&gt; !llvm.ptr
    %6 = acc.declare_enter dataOperands(%5 : !llvm.ptr)
    return
  }
}
```
```
mlir-opt temp.mlir
```
```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llvmorg-22.1.0-install/bin/mlir-opt temp.mlir
1.      MLIR Parser: custom op parser 'builtin.module'
2.      MLIR Parser: custom op parser 'func.func'
3.      MLIR Parser: custom op parser 'acc.getdeviceptr'
 #<!-- -->0 0x0000602bc19b3e32 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #<!-- -->1 0x0000602bc19b05af llvm::sys::RunSignalHandlers() (llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #<!-- -->2 0x0000602bc19b06fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x00007b11b6c45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->4 0x0000602bc95fdfb7 mlir::Type::getContext() const (llvmorg-22.1.0-install/bin/mlir-opt+0xa179fb7)
 #<!-- -->5 0x0000602bc951aec8 mlir::TypeAttr::get(mlir::Type) (llvmorg-22.1.0-install/bin/mlir-opt+0xa096ec8)
 #<!-- -->6 0x0000602bc33b1396 parseVarPtrType(mlir::OpAsmParser&amp;, mlir::Type&amp;, mlir::TypeAttr&amp;) (.constprop.0) OpenACC.cpp:0:0
 #<!-- -->7 0x0000602bc342d730 mlir::acc::GetDevicePtrOp::parse(mlir::OpAsmParser&amp;, mlir::OperationState&amp;) (llvmorg-22.1.0-install/bin/mlir-opt+0x3fa9730)
 #<!-- -->8 0x0000602bc7b4763d (anonymous namespace)::OperationParser::parseOperation() Parser.cpp:0:0
 #<!-- -->9 0x0000602bc7b4a5cf (anonymous namespace)::OperationParser::parseBlockBody(mlir::Block*) Parser.cpp:0:0
#<!-- -->10 0x0000602bc7b4adac (anonymous namespace)::OperationParser::parseRegionBody(mlir::Region&amp;, llvm::SMLoc, llvm::ArrayRef&lt;mlir::OpAsmParser::Argument&gt;, bool) Parser.cpp:0:0
#<!-- -->11 0x0000602bc7b4b49d (anonymous namespace)::OperationParser::parseRegion(mlir::Region&amp;, llvm::ArrayRef&lt;mlir::OpAsmParser::Argument&gt;, bool) Parser.cpp:0:0
#<!-- -->12 0x0000602bc7b4b545 (anonymous namespace)::CustomOpAsmParser::parseOptionalRegion(mlir::Region&amp;, llvm::ArrayRef&lt;mlir::OpAsmParser::Argument&gt;, bool) Parser.cpp:0:0
#<!-- -->13 0x0000602bc93e00ef mlir::function_interface_impl::parseFunctionOp(mlir::OpAsmParser&amp;, mlir::OperationState&amp;, bool, mlir::StringAttr, llvm::function_ref&lt;mlir::Type (mlir::Builder&amp;, llvm::ArrayRef&lt;mlir::Type&gt;, llvm::ArrayRef&lt;mlir::Type&gt;, mlir::function_interface_impl::VariadicFlag, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;&amp;)&gt;, mlir::StringAttr, mlir::StringAttr) (llvmorg-22.1.0-install/bin/mlir-opt+0x9f5c0ef)
#<!-- -->14 0x0000602bc7f53016 mlir::func::FuncOp::parse(mlir::OpAsmParser&amp;, mlir::OperationState&amp;) (llvmorg-22.1.0-install/bin/mlir-opt+0x8acf016)
#<!-- -->15 0x0000602bc7b4763d (anonymous namespace)::OperationParser::parseOperation() Parser.cpp:0:0
#<!-- -->16 0x0000602bc7b4a5cf (anonymous namespace)::OperationParser::parseBlockBody(mlir::Block*) Parser.cpp:0:0
#<!-- -->17 0x0000602bc7b4adac (anonymous namespace)::OperationParser::parseRegionBody(mlir::Region&amp;, llvm::SMLoc, llvm::ArrayRef&lt;mlir::OpAsmParser::Argument&gt;, bool) Parser.cpp:0:0
#<!-- -->18 0x0000602bc7b4b743 (anonymous namespace)::CustomOpAsmParser::parseRegion(mlir::Region&amp;, llvm::ArrayRef&lt;mlir::OpAsmParser::Argument&gt;, bool) Parser.cpp:0:0
#<!-- -->19 0x0000602bc9541735 mlir::ModuleOp::parse(mlir::OpAsmParser&amp;, mlir::OperationState&amp;) (llvmorg-22.1.0-install/bin/mlir-opt+0xa0bd735)
#<!-- -->20 0x0000602bc7b4763d (anonymous namespace)::OperationParser::parseOperation() Parser.cpp:0:0
#<!-- -->21 0x0000602bc7b4933d mlir::parseAsmSourceFile(llvm::SourceMgr const&amp;, mlir::Block*, mlir::ParserConfig const&amp;, mlir::AsmParserState*, mlir::AsmParserCodeCompleteContext*) (llvmorg-22.1.0-install/bin/mlir-opt+0x86c533d)
#<!-- -->22 0x0000602bc1a79df7 performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) MlirOptMain.cpp:0:0
#<!-- -->23 0x0000602bc1a7b4ea llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::'lambda'(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, llvm::raw_ostream&amp;)&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) MlirOptMain.cpp:0:0
#<!-- -->24 0x0000602bc9623731 mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) (llvmorg-22.1.0-install/bin/mlir-opt+0xa19f731)
#<!-- -->25 0x0000602bc1a728d0 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) (.part.0) MlirOptMain.cpp:0:0
#<!-- -->26 0x0000602bc1a7bb70 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) (llvmorg-22.1.0-install/bin/mlir-opt+0x25f7b70)
#<!-- -->27 0x0000602bc1a7bda9 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) (llvmorg-22.1.0-install/bin/mlir-opt+0x25f7da9)
#<!-- -->28 0x0000602bc18ad023 main (llvmorg-22.1.0-install/bin/mlir-opt+0x2429023)
#<!-- -->29 0x00007b11b6c2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->30 0x00007b11b6c2a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->31 0x00007b11b6c2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->32 0x0000602bc1992405 _start (llvmorg-22.1.0-install/bin/mlir-opt+0x250e405)
Segmentation fault (core dumped)
```
</details>


---

