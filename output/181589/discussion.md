# mlir-opt crash: Null pointer dereference in acc.getdeviceptr custom parser

**Author:** Subway2023
**URL:** https://github.com/llvm/llvm-project/issues/181589
**Status:** Closed
**Labels:** crash, mlir:openacc
**Closed Date:** 2026-02-27T14:57:07Z

## Body

Bug Op: acc.getdeviceptr

Context: MLIR Parsing (Custom Op Parser)

Description: The mlir-opt tool crashes during parsing of the acc.getdeviceptr operation. The crash occurs within the parseVarPtrType helper function in the OpenACC dialect. The parser attempts to create a TypeAttr from a Type variable that has not been successfully initialized (i.e., it is null). When mlir::TypeAttr::get() is called on this null type, it triggers a null pointer dereference while attempting to access the MLIR context.


```mlir
module {
  func.func @main() {
    %4 = llvm.call_intrinsic "llvm.dso_local_equivalent"() : () -> !llvm.ptr
    %5 = acc.getdeviceptr varPtr(%4 : !llvm.ptr) -> !llvm.ptr
    %6 = acc.declare_enter dataOperands(%5 : !llvm.ptr)
    return
  }
}
```
```
mlir-opt test.mlir
```
```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llvmorg-22.1.0-install/bin/mlir-opt test.mlir
1.      MLIR Parser: custom op parser 'builtin.module'
2.      MLIR Parser: custom op parser 'func.func'
3.      MLIR Parser: custom op parser 'acc.getdeviceptr'
 #0 0x000059cb4f01be32 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #1 0x000059cb4f0185af llvm::sys::RunSignalHandlers() (llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #2 0x000059cb4f0186fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x0000711a55c45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x000059cb56c65fb7 mlir::Type::getContext() const (llvmorg-22.1.0-install/bin/mlir-opt+0xa179fb7)
 #5 0x000059cb56b82ec8 mlir::TypeAttr::get(mlir::Type) (llvmorg-22.1.0-install/bin/mlir-opt+0xa096ec8)
 #6 0x000059cb50a19396 parseVarPtrType(mlir::OpAsmParser&, mlir::Type&, mlir::TypeAttr&) (.constprop.0) OpenACC.cpp:0:0
 #7 0x000059cb50a95730 mlir::acc::GetDevicePtrOp::parse(mlir::OpAsmParser&, mlir::OperationState&) (llvmorg-22.1.0-install/bin/mlir-opt+0x3fa9730)
 #8 0x000059cb551af63d (anonymous namespace)::OperationParser::parseOperation() Parser.cpp:0:0
 #9 0x000059cb551b25cf (anonymous namespace)::OperationParser::parseBlockBody(mlir::Block*) Parser.cpp:0:0
#10 0x000059cb551b2dac (anonymous namespace)::OperationParser::parseRegionBody(mlir::Region&, llvm::SMLoc, llvm::ArrayRef<mlir::OpAsmParser::Argument>, bool) Parser.cpp:0:0
#11 0x000059cb551b349d (anonymous namespace)::OperationParser::parseRegion(mlir::Region&, llvm::ArrayRef<mlir::OpAsmParser::Argument>, bool) Parser.cpp:0:0
#12 0x000059cb551b3545 (anonymous namespace)::CustomOpAsmParser::parseOptionalRegion(mlir::Region&, llvm::ArrayRef<mlir::OpAsmParser::Argument>, bool) Parser.cpp:0:0
#13 0x000059cb56a480ef mlir::function_interface_impl::parseFunctionOp(mlir::OpAsmParser&, mlir::OperationState&, bool, mlir::StringAttr, llvm::function_ref<mlir::Type (mlir::Builder&, llvm::ArrayRef<mlir::Type>, llvm::ArrayRef<mlir::Type>, mlir::function_interface_impl::VariadicFlag, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>&)>, mlir::StringAttr, mlir::StringAttr) (llvmorg-22.1.0-install/bin/mlir-opt+0x9f5c0ef)
#14 0x000059cb555bb016 mlir::func::FuncOp::parse(mlir::OpAsmParser&, mlir::OperationState&) (llvmorg-22.1.0-install/bin/mlir-opt+0x8acf016)
#15 0x000059cb551af63d (anonymous namespace)::OperationParser::parseOperation() Parser.cpp:0:0
#16 0x000059cb551b25cf (anonymous namespace)::OperationParser::parseBlockBody(mlir::Block*) Parser.cpp:0:0
#17 0x000059cb551b2dac (anonymous namespace)::OperationParser::parseRegionBody(mlir::Region&, llvm::SMLoc, llvm::ArrayRef<mlir::OpAsmParser::Argument>, bool) Parser.cpp:0:0
#18 0x000059cb551b3743 (anonymous namespace)::CustomOpAsmParser::parseRegion(mlir::Region&, llvm::ArrayRef<mlir::OpAsmParser::Argument>, bool) Parser.cpp:0:0
#19 0x000059cb56ba9735 mlir::ModuleOp::parse(mlir::OpAsmParser&, mlir::OperationState&) (llvmorg-22.1.0-install/bin/mlir-opt+0xa0bd735)
#20 0x000059cb551af63d (anonymous namespace)::OperationParser::parseOperation() Parser.cpp:0:0
#21 0x000059cb551b133d mlir::parseAsmSourceFile(llvm::SourceMgr const&, mlir::Block*, mlir::ParserConfig const&, mlir::AsmParserState*, mlir::AsmParserCodeCompleteContext*) (llvmorg-22.1.0-install/bin/mlir-opt+0x86c533d)
#22 0x000059cb4f0e1df7 performActions(llvm::raw_ostream&, std::shared_ptr<llvm::SourceMgr> const&, mlir::MLIRContext*, mlir::MlirOptMainConfig const&) MlirOptMain.cpp:0:0
#23 0x000059cb4f0e34ea llvm::LogicalResult llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::callback_fn<mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::'lambda'(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, llvm::raw_ostream&)>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) MlirOptMain.cpp:0:0
#24 0x000059cb56c8b731 mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef) (llvmorg-22.1.0-install/bin/mlir-opt+0xa19f731)
#25 0x000059cb4f0da8d0 mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&) (.part.0) MlirOptMain.cpp:0:0
#26 0x000059cb4f0e3b70 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&) (llvmorg-22.1.0-install/bin/mlir-opt+0x25f7b70)
#27 0x000059cb4f0e3da9 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&) (llvmorg-22.1.0-install/bin/mlir-opt+0x25f7da9)
#28 0x000059cb4ef15023 main (llvmorg-22.1.0-install/bin/mlir-opt+0x2429023)
#29 0x0000711a55c2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#30 0x0000711a55c2a28b call_init ./csu/../csu/libc-start.c:128:20
#31 0x0000711a55c2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#32 0x000059cb4effa405 _start (llvmorg-22.1.0-install/bin/mlir-opt+0x250e405)
Segmentation fault (core dumped)
```


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: None (Subway2023)

<details>
Bug Op: acc.getdeviceptr

Context: MLIR Parsing (Custom Op Parser)

Description: The mlir-opt tool crashes during parsing of the acc.getdeviceptr operation. The crash occurs within the parseVarPtrType helper function in the OpenACC dialect. The parser attempts to create a TypeAttr from a Type variable that has not been successfully initialized (i.e., it is null). When mlir::TypeAttr::get() is called on this null type, it triggers a null pointer dereference while attempting to access the MLIR context.


```mlir
module {
  func.func @<!-- -->main() {
    %4 = llvm.call_intrinsic "llvm.dso_local_equivalent"() : () -&gt; !llvm.ptr
    %5 = acc.getdeviceptr varPtr(%4 : !llvm.ptr) -&gt; !llvm.ptr
    %6 = acc.declare_enter dataOperands(%5 : !llvm.ptr)
    return
  }
}
```
```
mlir-opt test.mlir
```
```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llvmorg-22.1.0-install/bin/mlir-opt test.mlir
1.      MLIR Parser: custom op parser 'builtin.module'
2.      MLIR Parser: custom op parser 'func.func'
3.      MLIR Parser: custom op parser 'acc.getdeviceptr'
 #<!-- -->0 0x000059cb4f01be32 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #<!-- -->1 0x000059cb4f0185af llvm::sys::RunSignalHandlers() (llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #<!-- -->2 0x000059cb4f0186fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x0000711a55c45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->4 0x000059cb56c65fb7 mlir::Type::getContext() const (llvmorg-22.1.0-install/bin/mlir-opt+0xa179fb7)
 #<!-- -->5 0x000059cb56b82ec8 mlir::TypeAttr::get(mlir::Type) (llvmorg-22.1.0-install/bin/mlir-opt+0xa096ec8)
 #<!-- -->6 0x000059cb50a19396 parseVarPtrType(mlir::OpAsmParser&amp;, mlir::Type&amp;, mlir::TypeAttr&amp;) (.constprop.0) OpenACC.cpp:0:0
 #<!-- -->7 0x000059cb50a95730 mlir::acc::GetDevicePtrOp::parse(mlir::OpAsmParser&amp;, mlir::OperationState&amp;) (llvmorg-22.1.0-install/bin/mlir-opt+0x3fa9730)
 #<!-- -->8 0x000059cb551af63d (anonymous namespace)::OperationParser::parseOperation() Parser.cpp:0:0
 #<!-- -->9 0x000059cb551b25cf (anonymous namespace)::OperationParser::parseBlockBody(mlir::Block*) Parser.cpp:0:0
#<!-- -->10 0x000059cb551b2dac (anonymous namespace)::OperationParser::parseRegionBody(mlir::Region&amp;, llvm::SMLoc, llvm::ArrayRef&lt;mlir::OpAsmParser::Argument&gt;, bool) Parser.cpp:0:0
#<!-- -->11 0x000059cb551b349d (anonymous namespace)::OperationParser::parseRegion(mlir::Region&amp;, llvm::ArrayRef&lt;mlir::OpAsmParser::Argument&gt;, bool) Parser.cpp:0:0
#<!-- -->12 0x000059cb551b3545 (anonymous namespace)::CustomOpAsmParser::parseOptionalRegion(mlir::Region&amp;, llvm::ArrayRef&lt;mlir::OpAsmParser::Argument&gt;, bool) Parser.cpp:0:0
#<!-- -->13 0x000059cb56a480ef mlir::function_interface_impl::parseFunctionOp(mlir::OpAsmParser&amp;, mlir::OperationState&amp;, bool, mlir::StringAttr, llvm::function_ref&lt;mlir::Type (mlir::Builder&amp;, llvm::ArrayRef&lt;mlir::Type&gt;, llvm::ArrayRef&lt;mlir::Type&gt;, mlir::function_interface_impl::VariadicFlag, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;&amp;)&gt;, mlir::StringAttr, mlir::StringAttr) (llvmorg-22.1.0-install/bin/mlir-opt+0x9f5c0ef)
#<!-- -->14 0x000059cb555bb016 mlir::func::FuncOp::parse(mlir::OpAsmParser&amp;, mlir::OperationState&amp;) (llvmorg-22.1.0-install/bin/mlir-opt+0x8acf016)
#<!-- -->15 0x000059cb551af63d (anonymous namespace)::OperationParser::parseOperation() Parser.cpp:0:0
#<!-- -->16 0x000059cb551b25cf (anonymous namespace)::OperationParser::parseBlockBody(mlir::Block*) Parser.cpp:0:0
#<!-- -->17 0x000059cb551b2dac (anonymous namespace)::OperationParser::parseRegionBody(mlir::Region&amp;, llvm::SMLoc, llvm::ArrayRef&lt;mlir::OpAsmParser::Argument&gt;, bool) Parser.cpp:0:0
#<!-- -->18 0x000059cb551b3743 (anonymous namespace)::CustomOpAsmParser::parseRegion(mlir::Region&amp;, llvm::ArrayRef&lt;mlir::OpAsmParser::Argument&gt;, bool) Parser.cpp:0:0
#<!-- -->19 0x000059cb56ba9735 mlir::ModuleOp::parse(mlir::OpAsmParser&amp;, mlir::OperationState&amp;) (llvmorg-22.1.0-install/bin/mlir-opt+0xa0bd735)
#<!-- -->20 0x000059cb551af63d (anonymous namespace)::OperationParser::parseOperation() Parser.cpp:0:0
#<!-- -->21 0x000059cb551b133d mlir::parseAsmSourceFile(llvm::SourceMgr const&amp;, mlir::Block*, mlir::ParserConfig const&amp;, mlir::AsmParserState*, mlir::AsmParserCodeCompleteContext*) (llvmorg-22.1.0-install/bin/mlir-opt+0x86c533d)
#<!-- -->22 0x000059cb4f0e1df7 performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) MlirOptMain.cpp:0:0
#<!-- -->23 0x000059cb4f0e34ea llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::'lambda'(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, llvm::raw_ostream&amp;)&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) MlirOptMain.cpp:0:0
#<!-- -->24 0x000059cb56c8b731 mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) (llvmorg-22.1.0-install/bin/mlir-opt+0xa19f731)
#<!-- -->25 0x000059cb4f0da8d0 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) (.part.0) MlirOptMain.cpp:0:0
#<!-- -->26 0x000059cb4f0e3b70 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) (llvmorg-22.1.0-install/bin/mlir-opt+0x25f7b70)
#<!-- -->27 0x000059cb4f0e3da9 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) (llvmorg-22.1.0-install/bin/mlir-opt+0x25f7da9)
#<!-- -->28 0x000059cb4ef15023 main (llvmorg-22.1.0-install/bin/mlir-opt+0x2429023)
#<!-- -->29 0x0000711a55c2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->30 0x0000711a55c2a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->31 0x0000711a55c2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->32 0x000059cb4effa405 _start (llvmorg-22.1.0-install/bin/mlir-opt+0x250e405)
Segmentation fault (core dumped)
```

</details>


---

### Comment 2 - EugeneZelenko

@Subway2023: Please run fuzzer with `main` branch (and periodically update it). Looks like bug fixes for such problems are not often backported to release branch.

---

