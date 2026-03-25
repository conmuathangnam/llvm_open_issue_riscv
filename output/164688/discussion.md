# [clang] Crash when explicitly specializing a member function template with `template<> auto A::B()`

**Author:** Attacker23
**URL:** https://github.com/llvm/llvm-project/issues/164688
**Status:** Closed
**Labels:** duplicate, clang:frontend, confirmed, crash
**Closed Date:** 2025-12-30T07:31:13Z

## Body

The following code causes **crash** on x86-64 clang since 6.0.0:

```cpp

class A {
    template <typename T>
    T B(); 
};

template <> auto A::B() { return 1; }

```
The code is probably ill‑formed according to the standard (missing template argument in explicit specialization).
**GCC** rejects it, **MSVC/EDG** accept it, but **Clang** crashes 😅

Please check https://godbolt.org/z/7Gab5r8eq for more info

Assertion:

```console
clang++: /root/llvm-project/llvm/tools/clang/lib/CodeGen/CGDebugInfo.cpp:4021:
clang::QualType UnwrapTypeForDebugInfo(clang::QualType, const clang::ASTContext&):
Assertion `!DT.isNull() && "Undeduced types shouldn't reach here."' failed.
```

Compiler Output:

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<eof> parser at end of file
2.	<source>:6:21: LLVM IR generation of declaration 'A::B'
3.	<source>:6:21: Generating code for declaration 'A::B'
 #0 0x0000000003ccfe98 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3ccfe98)
 #1 0x0000000003ccd86c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3ccd86c)
 #2 0x0000000003c13c48 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007fc26f242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x0000000003f6ca27 UnwrapTypeForDebugInfo(clang::QualType, clang::ASTContext const&) CGDebugInfo.cpp:0:0
 #5 0x0000000003fa7a9f clang::CodeGen::CGDebugInfo::getOrCreateType(clang::QualType, llvm::DIFile*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3fa7a9f)
 #6 0x0000000003face59 clang::CodeGen::CGDebugInfo::CollectTemplateParams(std::optional<clang::CodeGen::CGDebugInfo::TemplateArgs>, llvm::DIFile*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3face59)
 #7 0x0000000003fb9c8d clang::CodeGen::CGDebugInfo::collectFunctionDeclProps(clang::GlobalDecl, llvm::DIFile*, llvm::StringRef&, llvm::StringRef&, llvm::DIScope*&, llvm::MDTupleTypedArrayWrapper<llvm::DINode>&, llvm::DINode::DIFlags&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3fb9c8d)
 #8 0x0000000003fbf71f clang::CodeGen::CGDebugInfo::emitFunctionStart(clang::GlobalDecl, clang::SourceLocation, clang::SourceLocation, clang::QualType, llvm::Function*, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3fbf71f)
 #9 0x00000000041e573d clang::CodeGen::CodeGenFunction::StartFunction(clang::GlobalDecl, clang::QualType, llvm::Function*, clang::CodeGen::CGFunctionInfo const&, clang::CodeGen::FunctionArgList const&, clang::SourceLocation, clang::SourceLocation) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x41e573d)
#10 0x00000000041e928e clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x41e928e)
#11 0x00000000042473ab clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x42473ab)
#12 0x00000000042430d4 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x42430d4)
#13 0x0000000004243a23 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4243a23)
#14 0x000000000424d27f clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#15 0x000000000459d9e1 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#16 0x00000000045876d9 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45876d9)
#17 0x0000000006207064 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6207064)
#18 0x000000000459aa75 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x459aa75)
#19 0x00000000048980fa clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x48980fa)
#20 0x0000000004817d3b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4817d3b)
#21 0x000000000498e5fb clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x498e5fb)
#22 0x0000000000dd30c5 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdd30c5)
#23 0x0000000000dcb01b ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#24 0x0000000000dcb0bd int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#25 0x0000000004608359 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#26 0x0000000003c14063 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c14063)
#27 0x0000000004608579 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#28 0x00000000045cb772 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45cb772)
#29 0x00000000045cc651 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45cc651)
#30 0x00000000045d518c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45d518c)
#31 0x0000000000dcfb19 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdcfb19)
#32 0x0000000000c7b2c4 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc7b2c4)
#33 0x00007fc26f229d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#34 0x00007fc26f229e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#35 0x0000000000dcaab5 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdcaab5)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: None (Attacker23)

<details>
The following code causes **crash** on x86-64 clang since 6.0.0:

```cpp

class A {
    template &lt;typename T&gt;
    T B(); 
};

template &lt;&gt; auto A::B() { return 1; }

```
The code is probably ill‑formed according to the standard (missing template argument in explicit specialization).
**GCC** rejects it, **MSVC/EDG** accept it, but **Clang** crashes 😅

Please check https://godbolt.org/z/T7T5fh1MW for more info

Compiler Output:

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	&lt;source&gt;:6:21: LLVM IR generation of declaration 'A::B'
3.	&lt;source&gt;:6:21: Generating code for declaration 'A::B'
 #<!-- -->0 0x0000000003ccfe98 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3ccfe98)
 #<!-- -->1 0x0000000003ccd86c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3ccd86c)
 #<!-- -->2 0x0000000003c13c48 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007fc26f242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x0000000003f6ca27 UnwrapTypeForDebugInfo(clang::QualType, clang::ASTContext const&amp;) CGDebugInfo.cpp:0:0
 #<!-- -->5 0x0000000003fa7a9f clang::CodeGen::CGDebugInfo::getOrCreateType(clang::QualType, llvm::DIFile*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3fa7a9f)
 #<!-- -->6 0x0000000003face59 clang::CodeGen::CGDebugInfo::CollectTemplateParams(std::optional&lt;clang::CodeGen::CGDebugInfo::TemplateArgs&gt;, llvm::DIFile*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3face59)
 #<!-- -->7 0x0000000003fb9c8d clang::CodeGen::CGDebugInfo::collectFunctionDeclProps(clang::GlobalDecl, llvm::DIFile*, llvm::StringRef&amp;, llvm::StringRef&amp;, llvm::DIScope*&amp;, llvm::MDTupleTypedArrayWrapper&lt;llvm::DINode&gt;&amp;, llvm::DINode::DIFlags&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3fb9c8d)
 #<!-- -->8 0x0000000003fbf71f clang::CodeGen::CGDebugInfo::emitFunctionStart(clang::GlobalDecl, clang::SourceLocation, clang::SourceLocation, clang::QualType, llvm::Function*, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3fbf71f)
 #<!-- -->9 0x00000000041e573d clang::CodeGen::CodeGenFunction::StartFunction(clang::GlobalDecl, clang::QualType, llvm::Function*, clang::CodeGen::CGFunctionInfo const&amp;, clang::CodeGen::FunctionArgList const&amp;, clang::SourceLocation, clang::SourceLocation) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x41e573d)
#<!-- -->10 0x00000000041e928e clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x41e928e)
#<!-- -->11 0x00000000042473ab clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x42473ab)
#<!-- -->12 0x00000000042430d4 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x42430d4)
#<!-- -->13 0x0000000004243a23 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4243a23)
#<!-- -->14 0x000000000424d27f clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#<!-- -->15 0x000000000459d9e1 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#<!-- -->16 0x00000000045876d9 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45876d9)
#<!-- -->17 0x0000000006207064 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6207064)
#<!-- -->18 0x000000000459aa75 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x459aa75)
#<!-- -->19 0x00000000048980fa clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x48980fa)
#<!-- -->20 0x0000000004817d3b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4817d3b)
#<!-- -->21 0x000000000498e5fb clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x498e5fb)
#<!-- -->22 0x0000000000dd30c5 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdd30c5)
#<!-- -->23 0x0000000000dcb01b ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->24 0x0000000000dcb0bd int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->25 0x0000000004608359 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->26 0x0000000003c14063 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c14063)
#<!-- -->27 0x0000000004608579 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->28 0x00000000045cb772 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45cb772)
#<!-- -->29 0x00000000045cc651 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45cc651)
#<!-- -->30 0x00000000045d518c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45d518c)
#<!-- -->31 0x0000000000dcfb19 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdcfb19)
#<!-- -->32 0x0000000000c7b2c4 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc7b2c4)
#<!-- -->33 0x00007fc26f229d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->34 0x00007fc26f229e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->35 0x0000000000dcaab5 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdcaab5)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```
</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (Attacker23)

<details>
The following code causes **crash** on x86-64 clang since 6.0.0:

```cpp

class A {
    template &lt;typename T&gt;
    T B(); 
};

template &lt;&gt; auto A::B() { return 1; }

```
The code is probably ill‑formed according to the standard (missing template argument in explicit specialization).
**GCC** rejects it, **MSVC/EDG** accept it, but **Clang** crashes 😅

Please check https://godbolt.org/z/T7T5fh1MW for more info

Compiler Output:

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	&lt;source&gt;:6:21: LLVM IR generation of declaration 'A::B'
3.	&lt;source&gt;:6:21: Generating code for declaration 'A::B'
 #<!-- -->0 0x0000000003ccfe98 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3ccfe98)
 #<!-- -->1 0x0000000003ccd86c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3ccd86c)
 #<!-- -->2 0x0000000003c13c48 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007fc26f242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x0000000003f6ca27 UnwrapTypeForDebugInfo(clang::QualType, clang::ASTContext const&amp;) CGDebugInfo.cpp:0:0
 #<!-- -->5 0x0000000003fa7a9f clang::CodeGen::CGDebugInfo::getOrCreateType(clang::QualType, llvm::DIFile*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3fa7a9f)
 #<!-- -->6 0x0000000003face59 clang::CodeGen::CGDebugInfo::CollectTemplateParams(std::optional&lt;clang::CodeGen::CGDebugInfo::TemplateArgs&gt;, llvm::DIFile*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3face59)
 #<!-- -->7 0x0000000003fb9c8d clang::CodeGen::CGDebugInfo::collectFunctionDeclProps(clang::GlobalDecl, llvm::DIFile*, llvm::StringRef&amp;, llvm::StringRef&amp;, llvm::DIScope*&amp;, llvm::MDTupleTypedArrayWrapper&lt;llvm::DINode&gt;&amp;, llvm::DINode::DIFlags&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3fb9c8d)
 #<!-- -->8 0x0000000003fbf71f clang::CodeGen::CGDebugInfo::emitFunctionStart(clang::GlobalDecl, clang::SourceLocation, clang::SourceLocation, clang::QualType, llvm::Function*, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3fbf71f)
 #<!-- -->9 0x00000000041e573d clang::CodeGen::CodeGenFunction::StartFunction(clang::GlobalDecl, clang::QualType, llvm::Function*, clang::CodeGen::CGFunctionInfo const&amp;, clang::CodeGen::FunctionArgList const&amp;, clang::SourceLocation, clang::SourceLocation) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x41e573d)
#<!-- -->10 0x00000000041e928e clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x41e928e)
#<!-- -->11 0x00000000042473ab clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x42473ab)
#<!-- -->12 0x00000000042430d4 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x42430d4)
#<!-- -->13 0x0000000004243a23 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4243a23)
#<!-- -->14 0x000000000424d27f clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#<!-- -->15 0x000000000459d9e1 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#<!-- -->16 0x00000000045876d9 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45876d9)
#<!-- -->17 0x0000000006207064 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6207064)
#<!-- -->18 0x000000000459aa75 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x459aa75)
#<!-- -->19 0x00000000048980fa clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x48980fa)
#<!-- -->20 0x0000000004817d3b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4817d3b)
#<!-- -->21 0x000000000498e5fb clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x498e5fb)
#<!-- -->22 0x0000000000dd30c5 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdd30c5)
#<!-- -->23 0x0000000000dcb01b ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->24 0x0000000000dcb0bd int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->25 0x0000000004608359 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->26 0x0000000003c14063 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c14063)
#<!-- -->27 0x0000000004608579 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->28 0x00000000045cb772 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45cb772)
#<!-- -->29 0x00000000045cc651 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45cc651)
#<!-- -->30 0x00000000045d518c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45d518c)
#<!-- -->31 0x0000000000dcfb19 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdcfb19)
#<!-- -->32 0x0000000000c7b2c4 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc7b2c4)
#<!-- -->33 0x00007fc26f229d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->34 0x00007fc26f229e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->35 0x0000000000dcaab5 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdcaab5)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```
</details>


---

