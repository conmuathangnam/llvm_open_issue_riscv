# [clang] crash on __attribute__((sycl_kernel))

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/161077
**Status:** Closed
**Labels:** clang:frontend, crash
**Closed Date:** 2025-10-22T14:47:31Z

## Body

Reproducer:
https://godbolt.org/z/Y553rnnWK
```cpp
__attribute__((sycl_kernel)) void foo(int *ret) {
  *ret = 1;
}
```

Backtrace:
```console
Unknown kernel calling convention
UNREACHABLE executed at /root/llvm-project/llvm/tools/clang/lib/CodeGen/TargetInfo.cpp:127!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<eof> parser at end of file
2.	<source>:1:35: LLVM IR generation of declaration 'foo'
3.	<source>:1:35: Generating code for declaration 'foo'
 #0 0x00000000041b0358 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41b0358)
 #1 0x00000000041ad784 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41ad784)
 #2 0x00000000040f1ab8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000078e2c1a42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x000078e2c1a969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x000078e2c1a42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x000078e2c1a287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00000000040fd18a (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x40fd18a)
 #8 0x000000000490ec60 clang::CodeGen::TargetCodeGenInfo::setOCLKernelStubCallingConvention(clang::FunctionType const*&) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x490ec60)
 #9 0x00000000049e3180 clang::CodeGen::CodeGenTypes::arrangeLLVMFunctionInfo(clang::CanQual<clang::Type>, clang::CodeGen::FnInfoOpts, llvm::ArrayRef<clang::CanQual<clang::Type>>, clang::FunctionType::ExtInfo, llvm::ArrayRef<clang::FunctionType::ExtParameterInfo>, clang::CodeGen::RequiredArgs) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49e3180)
#10 0x00000000049e638e arrangeLLVMFunctionInfo(clang::CodeGen::CodeGenTypes&, bool, llvm::SmallVectorImpl<clang::CanQual<clang::Type>>&, clang::CanQual<clang::FunctionProtoType>) CGCall.cpp:0:0
#11 0x00000000049e9fee clang::CodeGen::CodeGenTypes::arrangeFunctionDeclaration(clang::GlobalDecl) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49e9fee)
#12 0x0000000004785ca6 clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4785ca6)
#13 0x0000000004780ad4 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4780ad4)
#14 0x0000000004781970 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4781970)
#15 0x000000000478cac3 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#16 0x0000000004af1360 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#17 0x0000000004ae03b4 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ae03b4)
#18 0x0000000006878414 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6878414)
#19 0x0000000004aecd68 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4aecd68)
#20 0x0000000004dd8b75 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4dd8b75)
#21 0x0000000004d54a0e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d54a0e)
#22 0x0000000004ecf74d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ecf74d)
#23 0x0000000000db8ce0 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb8ce0)
#24 0x0000000000daf84a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#25 0x0000000000daf9cd int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#26 0x0000000004b55ea9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#27 0x00000000040f1f54 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x40f1f54)
#28 0x0000000004b564bf clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#29 0x0000000004b17822 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b17822)
#30 0x0000000004b187ce clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b187ce)
#31 0x0000000004b1ff05 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b1ff05)
#32 0x0000000000db5209 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb5209)
#33 0x0000000000c66b74 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc66b74)
#34 0x000078e2c1a29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#35 0x000078e2c1a29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#36 0x0000000000daf2e5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdaf2e5)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/Y553rnnWK
```cpp
__attribute__((sycl_kernel)) void foo(int *ret) {
  *ret = 1;
}
```

Backtrace:
```console
Unknown kernel calling convention
UNREACHABLE executed at /root/llvm-project/llvm/tools/clang/lib/CodeGen/TargetInfo.cpp:127!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	&lt;source&gt;:1:35: LLVM IR generation of declaration 'foo'
3.	&lt;source&gt;:1:35: Generating code for declaration 'foo'
 #<!-- -->0 0x00000000041b0358 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41b0358)
 #<!-- -->1 0x00000000041ad784 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41ad784)
 #<!-- -->2 0x00000000040f1ab8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x000078e2c1a42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x000078e2c1a969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x000078e2c1a42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x000078e2c1a287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x00000000040fd18a (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x40fd18a)
 #<!-- -->8 0x000000000490ec60 clang::CodeGen::TargetCodeGenInfo::setOCLKernelStubCallingConvention(clang::FunctionType const*&amp;) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x490ec60)
 #<!-- -->9 0x00000000049e3180 clang::CodeGen::CodeGenTypes::arrangeLLVMFunctionInfo(clang::CanQual&lt;clang::Type&gt;, clang::CodeGen::FnInfoOpts, llvm::ArrayRef&lt;clang::CanQual&lt;clang::Type&gt;&gt;, clang::FunctionType::ExtInfo, llvm::ArrayRef&lt;clang::FunctionType::ExtParameterInfo&gt;, clang::CodeGen::RequiredArgs) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49e3180)
#<!-- -->10 0x00000000049e638e arrangeLLVMFunctionInfo(clang::CodeGen::CodeGenTypes&amp;, bool, llvm::SmallVectorImpl&lt;clang::CanQual&lt;clang::Type&gt;&gt;&amp;, clang::CanQual&lt;clang::FunctionProtoType&gt;) CGCall.cpp:0:0
#<!-- -->11 0x00000000049e9fee clang::CodeGen::CodeGenTypes::arrangeFunctionDeclaration(clang::GlobalDecl) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49e9fee)
#<!-- -->12 0x0000000004785ca6 clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4785ca6)
#<!-- -->13 0x0000000004780ad4 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4780ad4)
#<!-- -->14 0x0000000004781970 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4781970)
#<!-- -->15 0x000000000478cac3 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#<!-- -->16 0x0000000004af1360 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#<!-- -->17 0x0000000004ae03b4 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ae03b4)
#<!-- -->18 0x0000000006878414 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6878414)
#<!-- -->19 0x0000000004aecd68 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4aecd68)
#<!-- -->20 0x0000000004dd8b75 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4dd8b75)
#<!-- -->21 0x0000000004d54a0e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d54a0e)
#<!-- -->22 0x0000000004ecf74d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ecf74d)
#<!-- -->23 0x0000000000db8ce0 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb8ce0)
#<!-- -->24 0x0000000000daf84a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->25 0x0000000000daf9cd int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->26 0x0000000004b55ea9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->27 0x00000000040f1f54 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x40f1f54)
#<!-- -->28 0x0000000004b564bf clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->29 0x0000000004b17822 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b17822)
#<!-- -->30 0x0000000004b187ce clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b187ce)
#<!-- -->31 0x0000000004b1ff05 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b1ff05)
#<!-- -->32 0x0000000000db5209 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb5209)
#<!-- -->33 0x0000000000c66b74 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc66b74)
#<!-- -->34 0x000078e2c1a29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->35 0x000078e2c1a29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->36 0x0000000000daf2e5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdaf2e5)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

### Comment 2 - k-arrows

Goes back to clang-21.1.0:
https://godbolt.org/z/ofPrn65Gb

---

### Comment 3 - k-arrows

This issue has been fixed. I will close this issue.

---

### Comment 4 - k-arrows

Reopened. Since the following test case has not been fixed yet, I should wait for the PR to be merged.
https://godbolt.org/z/Kc3b13q6M
```cpp
__attribute__((device_kernel)) void foo(int *ret) {
  *ret = 1;
}
```

---

### Comment 5 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/Y553rnnWK
```cpp
__attribute__((sycl_kernel)) void foo(int *ret) {
  *ret = 1;
}
```

Backtrace:
```console
Unknown kernel calling convention
UNREACHABLE executed at /root/llvm-project/llvm/tools/clang/lib/CodeGen/TargetInfo.cpp:127!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	&lt;source&gt;:1:35: LLVM IR generation of declaration 'foo'
3.	&lt;source&gt;:1:35: Generating code for declaration 'foo'
 #<!-- -->0 0x00000000041b0358 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41b0358)
 #<!-- -->1 0x00000000041ad784 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41ad784)
 #<!-- -->2 0x00000000040f1ab8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x000078e2c1a42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x000078e2c1a969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x000078e2c1a42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x000078e2c1a287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x00000000040fd18a (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x40fd18a)
 #<!-- -->8 0x000000000490ec60 clang::CodeGen::TargetCodeGenInfo::setOCLKernelStubCallingConvention(clang::FunctionType const*&amp;) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x490ec60)
 #<!-- -->9 0x00000000049e3180 clang::CodeGen::CodeGenTypes::arrangeLLVMFunctionInfo(clang::CanQual&lt;clang::Type&gt;, clang::CodeGen::FnInfoOpts, llvm::ArrayRef&lt;clang::CanQual&lt;clang::Type&gt;&gt;, clang::FunctionType::ExtInfo, llvm::ArrayRef&lt;clang::FunctionType::ExtParameterInfo&gt;, clang::CodeGen::RequiredArgs) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49e3180)
#<!-- -->10 0x00000000049e638e arrangeLLVMFunctionInfo(clang::CodeGen::CodeGenTypes&amp;, bool, llvm::SmallVectorImpl&lt;clang::CanQual&lt;clang::Type&gt;&gt;&amp;, clang::CanQual&lt;clang::FunctionProtoType&gt;) CGCall.cpp:0:0
#<!-- -->11 0x00000000049e9fee clang::CodeGen::CodeGenTypes::arrangeFunctionDeclaration(clang::GlobalDecl) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49e9fee)
#<!-- -->12 0x0000000004785ca6 clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4785ca6)
#<!-- -->13 0x0000000004780ad4 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4780ad4)
#<!-- -->14 0x0000000004781970 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4781970)
#<!-- -->15 0x000000000478cac3 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#<!-- -->16 0x0000000004af1360 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#<!-- -->17 0x0000000004ae03b4 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ae03b4)
#<!-- -->18 0x0000000006878414 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6878414)
#<!-- -->19 0x0000000004aecd68 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4aecd68)
#<!-- -->20 0x0000000004dd8b75 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4dd8b75)
#<!-- -->21 0x0000000004d54a0e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d54a0e)
#<!-- -->22 0x0000000004ecf74d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ecf74d)
#<!-- -->23 0x0000000000db8ce0 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb8ce0)
#<!-- -->24 0x0000000000daf84a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->25 0x0000000000daf9cd int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->26 0x0000000004b55ea9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->27 0x00000000040f1f54 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x40f1f54)
#<!-- -->28 0x0000000004b564bf clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->29 0x0000000004b17822 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b17822)
#<!-- -->30 0x0000000004b187ce clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b187ce)
#<!-- -->31 0x0000000004b1ff05 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b1ff05)
#<!-- -->32 0x0000000000db5209 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb5209)
#<!-- -->33 0x0000000000c66b74 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc66b74)
#<!-- -->34 0x000078e2c1a29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->35 0x000078e2c1a29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->36 0x0000000000daf2e5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdaf2e5)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

### Comment 6 - sarnex

The PR is merged @k-arrows can you confirm the issue is fixed?

---

