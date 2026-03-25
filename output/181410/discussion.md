# Assertion `!Init->isValueDependent()` failed

**Author:** Endilll
**URL:** https://github.com/llvm/llvm-project/issues/181410
**Status:** Closed
**Labels:** c++20, clang:frontend, regression, crash
**Closed Date:** 2026-02-26T19:26:12Z

## Body

While reducing #181062, I stumbled upon another crash (https://godbolt.org/z/aEqoxaWcG):
```cpp
template <int>
struct integer_sequence {};

template <int>
struct array {};

template <int*>
struct MetaValuesHelper;

template <typename TupleName, TupleName kValues>
struct MetaValuesHelper<kValues> {
  template <int... Is>
  static array<stdget<Is>(kValues)...> MetaValuesFunc(integer_sequence<Is...>);
};

int kBaseIndexRegistersUsed;

array<0> u = decltype(MetaValuesHelper<&kBaseIndexRegistersUsed>::MetaValuesFunc(integer_sequence<0>{})){};
```
Clang 23.0 output:
```
<source>:11:8: error: class template partial specialization is not more specialized than the primary template [-Winvalid-partial-specialization]
   11 | struct MetaValuesHelper<kValues> {
      |        ^
<source>:8:8: note: template is declared here
    8 | struct MetaValuesHelper;
      |        ^

clang++: /root/llvm-project/llvm/tools/clang/lib/AST/Decl.cpp:2596:
clang::APValue* clang::VarDecl::evaluateValueImpl(llvm::SmallVectorImpl<std::pair<clang::SourceLocation, clang::PartialDiagnostic> >&, bool) const:
Assertion `!Init->isValueDependent()' failed.

PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 <source>
1.	<eof> parser at end of file
2.	<source>:18:10: LLVM IR generation of declaration 'u'
3.	<source>:18:10: Generating code for declaration 'u'
 #0 0x00000000043cb438 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x43cb438)
 #1 0x00000000043c8894 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x43c8894)
 #2 0x0000000004308ba8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007c23a8c42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007c23a8c969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007c23a8c42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007c23a8c287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007c23a8c2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007c23a8c39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000007d0c963 (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7d0c963)
#10 0x0000000007d0c9ea clang::VarDecl::evaluateValue() const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7d0c9ea)
#11 0x000000000479b79b clang::CodeGen::ConstantEmitter::tryEmitPrivateForVarInit(clang::VarDecl const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x479b79b)
#12 0x000000000479bb91 clang::CodeGen::ConstantEmitter::tryEmitForInitializer(clang::VarDecl const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x479bb91)
#13 0x00000000049a2d6c clang::CodeGen::CodeGenModule::EmitGlobalVarDefinition(clang::VarDecl const*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49a2d6c)
#14 0x00000000049cb091 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49cb091)
#15 0x00000000049cb8d3 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49cb8d3)
#16 0x00000000049d6e4b clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#17 0x0000000004d47b49 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#18 0x0000000004d38f34 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d38f34)
#19 0x0000000006b69ed4 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b69ed4)
#20 0x0000000004d458e8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d458e8)
#21 0x00000000050439c5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x50439c5)
#22 0x0000000004fc2cde clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4fc2cde)
#23 0x000000000513f02d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x513f02d)
#24 0x0000000000de86de cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde86de)
#25 0x0000000000ddf0aa ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#26 0x0000000000ddf22d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#27 0x0000000004db9319 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#28 0x0000000004309044 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4309044)
#29 0x0000000004db992f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#30 0x0000000004d7a322 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d7a322)
#31 0x0000000004d7b2ce clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d7b2ce)
#32 0x0000000004d82585 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d82585)
#33 0x0000000000de4a51 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde4a51)
#34 0x0000000000c9e9b4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc9e9b4)
#35 0x00007c23a8c29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#36 0x00007c23a8c29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#37 0x0000000000ddeb45 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xddeb45)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Vlad Serebrennikov (Endilll)

<details>
While reducing #<!-- -->181062, I stumbled upon another crash (https://godbolt.org/z/aEqoxaWcG):
```cpp
template &lt;int&gt;
struct integer_sequence {};

template &lt;int&gt;
struct array {};

template &lt;int*&gt;
struct MetaValuesHelper;

template &lt;typename TupleName, TupleName kValues&gt;
struct MetaValuesHelper&lt;kValues&gt; {
  template &lt;int... Is&gt;
  static array&lt;stdget&lt;Is&gt;(kValues)...&gt; MetaValuesFunc(integer_sequence&lt;Is...&gt;);
};

int kBaseIndexRegistersUsed;

array&lt;0&gt; u = decltype(MetaValuesHelper&lt;&amp;kBaseIndexRegistersUsed&gt;::MetaValuesFunc(integer_sequence&lt;0&gt;{})){};
```
Clang 23.0 output:
```
&lt;source&gt;:11:8: error: class template partial specialization is not more specialized than the primary template [-Winvalid-partial-specialization]
   11 | struct MetaValuesHelper&lt;kValues&gt; {
      |        ^
&lt;source&gt;:8:8: note: template is declared here
    8 | struct MetaValuesHelper;
      |        ^

clang++: /root/llvm-project/llvm/tools/clang/lib/AST/Decl.cpp:2596:
clang::APValue* clang::VarDecl::evaluateValueImpl(llvm::SmallVectorImpl&lt;std::pair&lt;clang::SourceLocation, clang::PartialDiagnostic&gt; &gt;&amp;, bool) const:
Assertion `!Init-&gt;isValueDependent()' failed.

PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	&lt;source&gt;:18:10: LLVM IR generation of declaration 'u'
3.	&lt;source&gt;:18:10: Generating code for declaration 'u'
 #<!-- -->0 0x00000000043cb438 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x43cb438)
 #<!-- -->1 0x00000000043c8894 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x43c8894)
 #<!-- -->2 0x0000000004308ba8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007c23a8c42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007c23a8c969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007c23a8c42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007c23a8c287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x00007c23a8c2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x00007c23a8c39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000007d0c963 (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7d0c963)
#<!-- -->10 0x0000000007d0c9ea clang::VarDecl::evaluateValue() const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7d0c9ea)
#<!-- -->11 0x000000000479b79b clang::CodeGen::ConstantEmitter::tryEmitPrivateForVarInit(clang::VarDecl const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x479b79b)
#<!-- -->12 0x000000000479bb91 clang::CodeGen::ConstantEmitter::tryEmitForInitializer(clang::VarDecl const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x479bb91)
#<!-- -->13 0x00000000049a2d6c clang::CodeGen::CodeGenModule::EmitGlobalVarDefinition(clang::VarDecl const*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49a2d6c)
#<!-- -->14 0x00000000049cb091 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49cb091)
#<!-- -->15 0x00000000049cb8d3 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49cb8d3)
#<!-- -->16 0x00000000049d6e4b clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#<!-- -->17 0x0000000004d47b49 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#<!-- -->18 0x0000000004d38f34 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d38f34)
#<!-- -->19 0x0000000006b69ed4 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b69ed4)
#<!-- -->20 0x0000000004d458e8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d458e8)
#<!-- -->21 0x00000000050439c5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x50439c5)
#<!-- -->22 0x0000000004fc2cde clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4fc2cde)
#<!-- -->23 0x000000000513f02d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x513f02d)
#<!-- -->24 0x0000000000de86de cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde86de)
#<!-- -->25 0x0000000000ddf0aa ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->26 0x0000000000ddf22d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->27 0x0000000004db9319 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->28 0x0000000004309044 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4309044)
#<!-- -->29 0x0000000004db992f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->30 0x0000000004d7a322 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d7a322)
#<!-- -->31 0x0000000004d7b2ce clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d7b2ce)
#<!-- -->32 0x0000000004d82585 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d82585)
#<!-- -->33 0x0000000000de4a51 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde4a51)
#<!-- -->34 0x0000000000c9e9b4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc9e9b4)
#<!-- -->35 0x00007c23a8c29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->36 0x00007c23a8c29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->37 0x0000000000ddeb45 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xddeb45)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-c-20

Author: Vlad Serebrennikov (Endilll)

<details>
While reducing #<!-- -->181062, I stumbled upon another crash (https://godbolt.org/z/aEqoxaWcG):
```cpp
template &lt;int&gt;
struct integer_sequence {};

template &lt;int&gt;
struct array {};

template &lt;int*&gt;
struct MetaValuesHelper;

template &lt;typename TupleName, TupleName kValues&gt;
struct MetaValuesHelper&lt;kValues&gt; {
  template &lt;int... Is&gt;
  static array&lt;stdget&lt;Is&gt;(kValues)...&gt; MetaValuesFunc(integer_sequence&lt;Is...&gt;);
};

int kBaseIndexRegistersUsed;

array&lt;0&gt; u = decltype(MetaValuesHelper&lt;&amp;kBaseIndexRegistersUsed&gt;::MetaValuesFunc(integer_sequence&lt;0&gt;{})){};
```
Clang 23.0 output:
```
&lt;source&gt;:11:8: error: class template partial specialization is not more specialized than the primary template [-Winvalid-partial-specialization]
   11 | struct MetaValuesHelper&lt;kValues&gt; {
      |        ^
&lt;source&gt;:8:8: note: template is declared here
    8 | struct MetaValuesHelper;
      |        ^

clang++: /root/llvm-project/llvm/tools/clang/lib/AST/Decl.cpp:2596:
clang::APValue* clang::VarDecl::evaluateValueImpl(llvm::SmallVectorImpl&lt;std::pair&lt;clang::SourceLocation, clang::PartialDiagnostic&gt; &gt;&amp;, bool) const:
Assertion `!Init-&gt;isValueDependent()' failed.

PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	&lt;source&gt;:18:10: LLVM IR generation of declaration 'u'
3.	&lt;source&gt;:18:10: Generating code for declaration 'u'
 #<!-- -->0 0x00000000043cb438 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x43cb438)
 #<!-- -->1 0x00000000043c8894 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x43c8894)
 #<!-- -->2 0x0000000004308ba8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007c23a8c42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007c23a8c969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007c23a8c42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007c23a8c287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x00007c23a8c2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x00007c23a8c39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000007d0c963 (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7d0c963)
#<!-- -->10 0x0000000007d0c9ea clang::VarDecl::evaluateValue() const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7d0c9ea)
#<!-- -->11 0x000000000479b79b clang::CodeGen::ConstantEmitter::tryEmitPrivateForVarInit(clang::VarDecl const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x479b79b)
#<!-- -->12 0x000000000479bb91 clang::CodeGen::ConstantEmitter::tryEmitForInitializer(clang::VarDecl const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x479bb91)
#<!-- -->13 0x00000000049a2d6c clang::CodeGen::CodeGenModule::EmitGlobalVarDefinition(clang::VarDecl const*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49a2d6c)
#<!-- -->14 0x00000000049cb091 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49cb091)
#<!-- -->15 0x00000000049cb8d3 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49cb8d3)
#<!-- -->16 0x00000000049d6e4b clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#<!-- -->17 0x0000000004d47b49 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#<!-- -->18 0x0000000004d38f34 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d38f34)
#<!-- -->19 0x0000000006b69ed4 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b69ed4)
#<!-- -->20 0x0000000004d458e8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d458e8)
#<!-- -->21 0x00000000050439c5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x50439c5)
#<!-- -->22 0x0000000004fc2cde clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4fc2cde)
#<!-- -->23 0x000000000513f02d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x513f02d)
#<!-- -->24 0x0000000000de86de cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde86de)
#<!-- -->25 0x0000000000ddf0aa ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->26 0x0000000000ddf22d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->27 0x0000000004db9319 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->28 0x0000000004309044 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4309044)
#<!-- -->29 0x0000000004db992f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->30 0x0000000004d7a322 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d7a322)
#<!-- -->31 0x0000000004d7b2ce clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d7b2ce)
#<!-- -->32 0x0000000004d82585 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d82585)
#<!-- -->33 0x0000000000de4a51 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde4a51)
#<!-- -->34 0x0000000000c9e9b4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc9e9b4)
#<!-- -->35 0x00007c23a8c29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->36 0x00007c23a8c29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->37 0x0000000000ddeb45 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xddeb45)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

### Comment 3 - Endilll

@mizvekov I haven't tested whether it was caused by https://github.com/llvm/llvm-project/pull/161029, too, but might be worth to look into.

---

