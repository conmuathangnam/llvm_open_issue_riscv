# mlir-translate crashes on mlir/test/Dialect/LLVMIR/module-roundtrip.mlir

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/160717
**Status:** Closed
**Labels:** mlir:llvm, crash
**Closed Date:** 2025-11-27T05:29:39Z

## Body

Reproducible on godbolt:
https://godbolt.org/z/1xWTYsaMT

Backtrace:
```console
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/mlir-trunk/bin/mlir-translate -o /app/example.out.mlir -mlir-to-llvmir <source>
 #0 0x0000000001accbe8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/mlir-trunk/bin/mlir-translate+0x1accbe8)
 #1 0x0000000001aca064 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x00007f7e3ce42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x00000000017a52d0 llvm::Value::getContext() const (/opt/compiler-explorer/mlir-trunk/bin/mlir-translate+0x17a52d0)
 #4 0x000000000177a154 llvm::ValueAsMetadata::get(llvm::Value*) (/opt/compiler-explorer/mlir-trunk/bin/mlir-translate+0x177a154)
 #5 0x0000000000d0ccd2 convertOperationImpl(mlir::Operation&, llvm::IRBuilderBase&, mlir::LLVM::ModuleTranslation&) LLVMToLLVMIRTranslation.cpp:0:0
 #6 0x0000000000d17db6 mlir::LLVM::ModuleTranslation::convertOperation(mlir::Operation&, llvm::IRBuilderBase&, bool) (/opt/compiler-explorer/mlir-trunk/bin/mlir-translate+0xd17db6)
 #7 0x0000000000d2c34a mlir::translateModuleToLLVMIR(mlir::Operation*, llvm::LLVMContext&, llvm::StringRef, bool) (/opt/compiler-explorer/mlir-trunk/bin/mlir-translate+0xd2c34a)
 #8 0x0000000000486bad std::_Function_handler<llvm::LogicalResult (mlir::Operation*, llvm::raw_ostream&), mlir::registerToLLVMIRTranslation()::'lambda'(mlir::Operation*, llvm::raw_ostream&)>::_M_invoke(std::_Any_data const&, mlir::Operation*&&, llvm::raw_ostream&) ConvertToLLVMIR.cpp:0:0
 #9 0x000000000045520d mlir::TranslateFromMLIRRegistration::TranslateFromMLIRRegistration(llvm::StringRef, llvm::StringRef, std::function<llvm::LogicalResult (mlir::Operation*, llvm::raw_ostream&)> const&, std::function<void (mlir::DialectRegistry&)> const&)::'lambda'(std::shared_ptr<llvm::SourceMgr> const&, llvm::raw_ostream&, mlir::MLIRContext*)::operator()(std::shared_ptr<llvm::SourceMgr> const&, llvm::raw_ostream&, mlir::MLIRContext*) const Translation.cpp:0:0
#10 0x0000000000451ea7 mlir::mlirTranslateMain(int, char**, llvm::StringRef)::'lambda0'(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::raw_ostream&)::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::raw_ostream&) const (.isra.0) MlirTranslateMain.cpp:0:0
#11 0x0000000000452216 llvm::LogicalResult llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::raw_ostream&)>::callback_fn<mlir::mlirTranslateMain(int, char**, llvm::StringRef)::'lambda0'(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::raw_ostream&)>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::raw_ostream&) MlirTranslateMain.cpp:0:0
#12 0x00000000004616f9 llvm::LogicalResult llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::callback_fn<mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef)::'lambda'(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) ToolUtilities.cpp:0:0
#13 0x000000000046196e mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef) (/opt/compiler-explorer/mlir-trunk/bin/mlir-translate+0x46196e)
#14 0x0000000000462024 mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef) (/opt/compiler-explorer/mlir-trunk/bin/mlir-translate+0x462024)
#15 0x0000000000452de6 mlir::mlirTranslateMain(int, char**, llvm::StringRef) (/opt/compiler-explorer/mlir-trunk/bin/mlir-translate+0x452de6)
#16 0x0000000000419cc4 main (/opt/compiler-explorer/mlir-trunk/bin/mlir-translate+0x419cc4)
#17 0x00007f7e3ce29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#18 0x00007f7e3ce29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#19 0x0000000000450775 _start (/opt/compiler-explorer/mlir-trunk/bin/mlir-translate+0x450775)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

The above reproducer comes from the following:
https://github.com/llvm/llvm-project/blob/main/mlir/test/Dialect/LLVMIR/module-roundtrip.mlir

I observed the following assertion failure with the above reproducer:
```txt
mlir-translate: /path_to_project/llvm-project/llvm/lib/IR/Metadata.cpp:503: static llvm::ValueAsMetadata *llvm::ValueAsMetadata::get(llvm::Value *): Assertion `V && "Unexpected null Value"' failed.
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: None (k-arrows)

<details>
Reproducible on godbolt:
https://godbolt.org/z/1xWTYsaMT

Backtrace:
```console
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/mlir-trunk/bin/mlir-translate -o /app/example.out.mlir -mlir-to-llvmir &lt;source&gt;
 #<!-- -->0 0x0000000001accbe8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/mlir-trunk/bin/mlir-translate+0x1accbe8)
 #<!-- -->1 0x0000000001aca064 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->2 0x00007f7e3ce42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->3 0x00000000017a52d0 llvm::Value::getContext() const (/opt/compiler-explorer/mlir-trunk/bin/mlir-translate+0x17a52d0)
 #<!-- -->4 0x000000000177a154 llvm::ValueAsMetadata::get(llvm::Value*) (/opt/compiler-explorer/mlir-trunk/bin/mlir-translate+0x177a154)
 #<!-- -->5 0x0000000000d0ccd2 convertOperationImpl(mlir::Operation&amp;, llvm::IRBuilderBase&amp;, mlir::LLVM::ModuleTranslation&amp;) LLVMToLLVMIRTranslation.cpp:0:0
 #<!-- -->6 0x0000000000d17db6 mlir::LLVM::ModuleTranslation::convertOperation(mlir::Operation&amp;, llvm::IRBuilderBase&amp;, bool) (/opt/compiler-explorer/mlir-trunk/bin/mlir-translate+0xd17db6)
 #<!-- -->7 0x0000000000d2c34a mlir::translateModuleToLLVMIR(mlir::Operation*, llvm::LLVMContext&amp;, llvm::StringRef, bool) (/opt/compiler-explorer/mlir-trunk/bin/mlir-translate+0xd2c34a)
 #<!-- -->8 0x0000000000486bad std::_Function_handler&lt;llvm::LogicalResult (mlir::Operation*, llvm::raw_ostream&amp;), mlir::registerToLLVMIRTranslation()::'lambda'(mlir::Operation*, llvm::raw_ostream&amp;)&gt;::_M_invoke(std::_Any_data const&amp;, mlir::Operation*&amp;&amp;, llvm::raw_ostream&amp;) ConvertToLLVMIR.cpp:0:0
 #<!-- -->9 0x000000000045520d mlir::TranslateFromMLIRRegistration::TranslateFromMLIRRegistration(llvm::StringRef, llvm::StringRef, std::function&lt;llvm::LogicalResult (mlir::Operation*, llvm::raw_ostream&amp;)&gt; const&amp;, std::function&lt;void (mlir::DialectRegistry&amp;)&gt; const&amp;)::'lambda'(std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, llvm::raw_ostream&amp;, mlir::MLIRContext*)::operator()(std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, llvm::raw_ostream&amp;, mlir::MLIRContext*) const Translation.cpp:0:0
#<!-- -->10 0x0000000000451ea7 mlir::mlirTranslateMain(int, char**, llvm::StringRef)::'lambda0'(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::raw_ostream&amp;)::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::raw_ostream&amp;) const (.isra.0) MlirTranslateMain.cpp:0:0
#<!-- -->11 0x0000000000452216 llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::mlirTranslateMain(int, char**, llvm::StringRef)::'lambda0'(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::raw_ostream&amp;)&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::raw_ostream&amp;) MlirTranslateMain.cpp:0:0
#<!-- -->12 0x00000000004616f9 llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef)::'lambda'(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) ToolUtilities.cpp:0:0
#<!-- -->13 0x000000000046196e mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) (/opt/compiler-explorer/mlir-trunk/bin/mlir-translate+0x46196e)
#<!-- -->14 0x0000000000462024 mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) (/opt/compiler-explorer/mlir-trunk/bin/mlir-translate+0x462024)
#<!-- -->15 0x0000000000452de6 mlir::mlirTranslateMain(int, char**, llvm::StringRef) (/opt/compiler-explorer/mlir-trunk/bin/mlir-translate+0x452de6)
#<!-- -->16 0x0000000000419cc4 main (/opt/compiler-explorer/mlir-trunk/bin/mlir-translate+0x419cc4)
#<!-- -->17 0x00007f7e3ce29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->18 0x00007f7e3ce29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->19 0x0000000000450775 _start (/opt/compiler-explorer/mlir-trunk/bin/mlir-translate+0x450775)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

The above reproducer comes from the following:
https://github.com/llvm/llvm-project/blob/main/mlir/test/Dialect/LLVMIR/module-roundtrip.mlir

I observed the following assertion failure with the above reproducer:
```txt
mlir-translate: /path_to_project/llvm-project/llvm/lib/IR/Metadata.cpp:503: static llvm::ValueAsMetadata *llvm::ValueAsMetadata::get(llvm::Value *): Assertion `V &amp;&amp; "Unexpected null Value"' failed.
```
</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-mlir

Author: None (k-arrows)

<details>
Reproducible on godbolt:
https://godbolt.org/z/1xWTYsaMT

Backtrace:
```console
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/mlir-trunk/bin/mlir-translate -o /app/example.out.mlir -mlir-to-llvmir &lt;source&gt;
 #<!-- -->0 0x0000000001accbe8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/mlir-trunk/bin/mlir-translate+0x1accbe8)
 #<!-- -->1 0x0000000001aca064 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->2 0x00007f7e3ce42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->3 0x00000000017a52d0 llvm::Value::getContext() const (/opt/compiler-explorer/mlir-trunk/bin/mlir-translate+0x17a52d0)
 #<!-- -->4 0x000000000177a154 llvm::ValueAsMetadata::get(llvm::Value*) (/opt/compiler-explorer/mlir-trunk/bin/mlir-translate+0x177a154)
 #<!-- -->5 0x0000000000d0ccd2 convertOperationImpl(mlir::Operation&amp;, llvm::IRBuilderBase&amp;, mlir::LLVM::ModuleTranslation&amp;) LLVMToLLVMIRTranslation.cpp:0:0
 #<!-- -->6 0x0000000000d17db6 mlir::LLVM::ModuleTranslation::convertOperation(mlir::Operation&amp;, llvm::IRBuilderBase&amp;, bool) (/opt/compiler-explorer/mlir-trunk/bin/mlir-translate+0xd17db6)
 #<!-- -->7 0x0000000000d2c34a mlir::translateModuleToLLVMIR(mlir::Operation*, llvm::LLVMContext&amp;, llvm::StringRef, bool) (/opt/compiler-explorer/mlir-trunk/bin/mlir-translate+0xd2c34a)
 #<!-- -->8 0x0000000000486bad std::_Function_handler&lt;llvm::LogicalResult (mlir::Operation*, llvm::raw_ostream&amp;), mlir::registerToLLVMIRTranslation()::'lambda'(mlir::Operation*, llvm::raw_ostream&amp;)&gt;::_M_invoke(std::_Any_data const&amp;, mlir::Operation*&amp;&amp;, llvm::raw_ostream&amp;) ConvertToLLVMIR.cpp:0:0
 #<!-- -->9 0x000000000045520d mlir::TranslateFromMLIRRegistration::TranslateFromMLIRRegistration(llvm::StringRef, llvm::StringRef, std::function&lt;llvm::LogicalResult (mlir::Operation*, llvm::raw_ostream&amp;)&gt; const&amp;, std::function&lt;void (mlir::DialectRegistry&amp;)&gt; const&amp;)::'lambda'(std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, llvm::raw_ostream&amp;, mlir::MLIRContext*)::operator()(std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, llvm::raw_ostream&amp;, mlir::MLIRContext*) const Translation.cpp:0:0
#<!-- -->10 0x0000000000451ea7 mlir::mlirTranslateMain(int, char**, llvm::StringRef)::'lambda0'(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::raw_ostream&amp;)::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::raw_ostream&amp;) const (.isra.0) MlirTranslateMain.cpp:0:0
#<!-- -->11 0x0000000000452216 llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::mlirTranslateMain(int, char**, llvm::StringRef)::'lambda0'(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::raw_ostream&amp;)&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::raw_ostream&amp;) MlirTranslateMain.cpp:0:0
#<!-- -->12 0x00000000004616f9 llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef)::'lambda'(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) ToolUtilities.cpp:0:0
#<!-- -->13 0x000000000046196e mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) (/opt/compiler-explorer/mlir-trunk/bin/mlir-translate+0x46196e)
#<!-- -->14 0x0000000000462024 mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) (/opt/compiler-explorer/mlir-trunk/bin/mlir-translate+0x462024)
#<!-- -->15 0x0000000000452de6 mlir::mlirTranslateMain(int, char**, llvm::StringRef) (/opt/compiler-explorer/mlir-trunk/bin/mlir-translate+0x452de6)
#<!-- -->16 0x0000000000419cc4 main (/opt/compiler-explorer/mlir-trunk/bin/mlir-translate+0x419cc4)
#<!-- -->17 0x00007f7e3ce29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->18 0x00007f7e3ce29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->19 0x0000000000450775 _start (/opt/compiler-explorer/mlir-trunk/bin/mlir-translate+0x450775)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

The above reproducer comes from the following:
https://github.com/llvm/llvm-project/blob/main/mlir/test/Dialect/LLVMIR/module-roundtrip.mlir

I observed the following assertion failure with the above reproducer:
```txt
mlir-translate: /path_to_project/llvm-project/llvm/lib/IR/Metadata.cpp:503: static llvm::ValueAsMetadata *llvm::ValueAsMetadata::get(llvm::Value *): Assertion `V &amp;&amp; "Unexpected null Value"' failed.
```
</details>


---

### Comment 3 - llvmbot


@llvm/issue-subscribers-mlir-llvm

Author: None (k-arrows)

<details>
Reproducible on godbolt:
https://godbolt.org/z/1xWTYsaMT

Backtrace:
```console
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/mlir-trunk/bin/mlir-translate -o /app/example.out.mlir -mlir-to-llvmir &lt;source&gt;
 #<!-- -->0 0x0000000001accbe8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/mlir-trunk/bin/mlir-translate+0x1accbe8)
 #<!-- -->1 0x0000000001aca064 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->2 0x00007f7e3ce42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->3 0x00000000017a52d0 llvm::Value::getContext() const (/opt/compiler-explorer/mlir-trunk/bin/mlir-translate+0x17a52d0)
 #<!-- -->4 0x000000000177a154 llvm::ValueAsMetadata::get(llvm::Value*) (/opt/compiler-explorer/mlir-trunk/bin/mlir-translate+0x177a154)
 #<!-- -->5 0x0000000000d0ccd2 convertOperationImpl(mlir::Operation&amp;, llvm::IRBuilderBase&amp;, mlir::LLVM::ModuleTranslation&amp;) LLVMToLLVMIRTranslation.cpp:0:0
 #<!-- -->6 0x0000000000d17db6 mlir::LLVM::ModuleTranslation::convertOperation(mlir::Operation&amp;, llvm::IRBuilderBase&amp;, bool) (/opt/compiler-explorer/mlir-trunk/bin/mlir-translate+0xd17db6)
 #<!-- -->7 0x0000000000d2c34a mlir::translateModuleToLLVMIR(mlir::Operation*, llvm::LLVMContext&amp;, llvm::StringRef, bool) (/opt/compiler-explorer/mlir-trunk/bin/mlir-translate+0xd2c34a)
 #<!-- -->8 0x0000000000486bad std::_Function_handler&lt;llvm::LogicalResult (mlir::Operation*, llvm::raw_ostream&amp;), mlir::registerToLLVMIRTranslation()::'lambda'(mlir::Operation*, llvm::raw_ostream&amp;)&gt;::_M_invoke(std::_Any_data const&amp;, mlir::Operation*&amp;&amp;, llvm::raw_ostream&amp;) ConvertToLLVMIR.cpp:0:0
 #<!-- -->9 0x000000000045520d mlir::TranslateFromMLIRRegistration::TranslateFromMLIRRegistration(llvm::StringRef, llvm::StringRef, std::function&lt;llvm::LogicalResult (mlir::Operation*, llvm::raw_ostream&amp;)&gt; const&amp;, std::function&lt;void (mlir::DialectRegistry&amp;)&gt; const&amp;)::'lambda'(std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, llvm::raw_ostream&amp;, mlir::MLIRContext*)::operator()(std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, llvm::raw_ostream&amp;, mlir::MLIRContext*) const Translation.cpp:0:0
#<!-- -->10 0x0000000000451ea7 mlir::mlirTranslateMain(int, char**, llvm::StringRef)::'lambda0'(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::raw_ostream&amp;)::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::raw_ostream&amp;) const (.isra.0) MlirTranslateMain.cpp:0:0
#<!-- -->11 0x0000000000452216 llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::mlirTranslateMain(int, char**, llvm::StringRef)::'lambda0'(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::raw_ostream&amp;)&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::raw_ostream&amp;) MlirTranslateMain.cpp:0:0
#<!-- -->12 0x00000000004616f9 llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef)::'lambda'(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) ToolUtilities.cpp:0:0
#<!-- -->13 0x000000000046196e mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) (/opt/compiler-explorer/mlir-trunk/bin/mlir-translate+0x46196e)
#<!-- -->14 0x0000000000462024 mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) (/opt/compiler-explorer/mlir-trunk/bin/mlir-translate+0x462024)
#<!-- -->15 0x0000000000452de6 mlir::mlirTranslateMain(int, char**, llvm::StringRef) (/opt/compiler-explorer/mlir-trunk/bin/mlir-translate+0x452de6)
#<!-- -->16 0x0000000000419cc4 main (/opt/compiler-explorer/mlir-trunk/bin/mlir-translate+0x419cc4)
#<!-- -->17 0x00007f7e3ce29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->18 0x00007f7e3ce29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->19 0x0000000000450775 _start (/opt/compiler-explorer/mlir-trunk/bin/mlir-translate+0x450775)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

The above reproducer comes from the following:
https://github.com/llvm/llvm-project/blob/main/mlir/test/Dialect/LLVMIR/module-roundtrip.mlir

I observed the following assertion failure with the above reproducer:
```txt
mlir-translate: /path_to_project/llvm-project/llvm/lib/IR/Metadata.cpp:503: static llvm::ValueAsMetadata *llvm::ValueAsMetadata::get(llvm::Value *): Assertion `V &amp;&amp; "Unexpected null Value"' failed.
```
</details>


---

