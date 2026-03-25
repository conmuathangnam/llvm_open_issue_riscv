# [X86] crash on __attribute__((nvptx_kernel))

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/166060
**Status:** Open
**Labels:** clang:codegen, crash

## Body

Related issue: https://github.com/llvm/llvm-project/issues/157028

Reproducer:
https://godbolt.org/z/1xf6caqPG
```cpp
__attribute__((nvptx_kernel)) void foo(int *ret) {
  *ret = 1;
}
```

Backtrace:
```console
Unknown kernel calling convention
UNREACHABLE executed at /root/llvm-project/llvm/tools/clang/lib/CodeGen/TargetInfo.cpp:129!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<eof> parser at end of file
2.	<source>:1:36: LLVM IR generation of declaration 'foo'
3.	<source>:1:36: Generating code for declaration 'foo'
 #0 0x00000000042387e8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42387e8)
 #1 0x0000000004235c14 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4235c14)
 #2 0x0000000004179e68 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007a0d09442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007a0d094969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007a0d09442476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007a0d094287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x0000000004185a4a (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4185a4a)
 #8 0x00000000049a44e0 clang::CodeGen::TargetCodeGenInfo::createEnqueuedBlockKernel(clang::CodeGen::CodeGenFunction&, llvm::Function*, llvm::Type*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49a44e0)
 #9 0x0000000004a7b6a0 clang::CodeGen::CodeGenTypes::arrangeLLVMFunctionInfo(clang::CanQual<clang::Type>, clang::CodeGen::FnInfoOpts, llvm::ArrayRef<clang::CanQual<clang::Type>>, clang::FunctionType::ExtInfo, llvm::ArrayRef<clang::FunctionType::ExtParameterInfo>, clang::CodeGen::RequiredArgs) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4a7b6a0)
#10 0x0000000004a7d21e arrangeLLVMFunctionInfo(clang::CodeGen::CodeGenTypes&, bool, llvm::SmallVectorImpl<clang::CanQual<clang::Type>>&, clang::CanQual<clang::FunctionProtoType>) CGCall.cpp:0:0
#11 0x0000000004a80e1e clang::CodeGen::CodeGenTypes::arrangeFunctionDeclaration(clang::GlobalDecl) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4a80e1e)
#12 0x000000000481cf16 clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x481cf16)
#13 0x0000000004817d34 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4817d34)
#14 0x0000000004818bd0 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4818bd0)
#15 0x00000000048235b3 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#16 0x0000000004b87f29 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#17 0x0000000004b77044 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b77044)
#18 0x000000000692f354 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x692f354)
#19 0x0000000004b83928 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b83928)
#20 0x0000000004e71515 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e71515)
#21 0x0000000004df231e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4df231e)
#22 0x0000000004f69ced clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f69ced)
#23 0x0000000000dd0670 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdd0670)
#24 0x0000000000dc71da ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#25 0x0000000000dc735d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#26 0x0000000004bed1c9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#27 0x000000000417a304 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x417a304)
#28 0x0000000004bed7df clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#29 0x0000000004bae572 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bae572)
#30 0x0000000004baf51e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4baf51e)
#31 0x0000000004bb6c55 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bb6c55)
#32 0x0000000000dccb99 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdccb99)
#33 0x0000000000c7bb64 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc7bb64)
#34 0x00007a0d09429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#35 0x00007a0d09429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#36 0x0000000000dc6c75 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdc6c75)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - k-arrows

Goes back to clang-21.1.0:
https://godbolt.org/z/s99d9qoz3

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: None (k-arrows)

<details>
Related issue: https://github.com/llvm/llvm-project/issues/157028

Reproducer:
https://godbolt.org/z/1xf6caqPG
```cpp
__attribute__((nvptx_kernel)) void foo(int *ret) {
  *ret = 1;
}
```

Backtrace:
```console
Unknown kernel calling convention
UNREACHABLE executed at /root/llvm-project/llvm/tools/clang/lib/CodeGen/TargetInfo.cpp:129!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	&lt;source&gt;:1:36: LLVM IR generation of declaration 'foo'
3.	&lt;source&gt;:1:36: Generating code for declaration 'foo'
 #<!-- -->0 0x00000000042387e8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42387e8)
 #<!-- -->1 0x0000000004235c14 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4235c14)
 #<!-- -->2 0x0000000004179e68 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007a0d09442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007a0d094969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007a0d09442476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007a0d094287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x0000000004185a4a (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4185a4a)
 #<!-- -->8 0x00000000049a44e0 clang::CodeGen::TargetCodeGenInfo::createEnqueuedBlockKernel(clang::CodeGen::CodeGenFunction&amp;, llvm::Function*, llvm::Type*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49a44e0)
 #<!-- -->9 0x0000000004a7b6a0 clang::CodeGen::CodeGenTypes::arrangeLLVMFunctionInfo(clang::CanQual&lt;clang::Type&gt;, clang::CodeGen::FnInfoOpts, llvm::ArrayRef&lt;clang::CanQual&lt;clang::Type&gt;&gt;, clang::FunctionType::ExtInfo, llvm::ArrayRef&lt;clang::FunctionType::ExtParameterInfo&gt;, clang::CodeGen::RequiredArgs) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4a7b6a0)
#<!-- -->10 0x0000000004a7d21e arrangeLLVMFunctionInfo(clang::CodeGen::CodeGenTypes&amp;, bool, llvm::SmallVectorImpl&lt;clang::CanQual&lt;clang::Type&gt;&gt;&amp;, clang::CanQual&lt;clang::FunctionProtoType&gt;) CGCall.cpp:0:0
#<!-- -->11 0x0000000004a80e1e clang::CodeGen::CodeGenTypes::arrangeFunctionDeclaration(clang::GlobalDecl) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4a80e1e)
#<!-- -->12 0x000000000481cf16 clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x481cf16)
#<!-- -->13 0x0000000004817d34 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4817d34)
#<!-- -->14 0x0000000004818bd0 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4818bd0)
#<!-- -->15 0x00000000048235b3 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#<!-- -->16 0x0000000004b87f29 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#<!-- -->17 0x0000000004b77044 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b77044)
#<!-- -->18 0x000000000692f354 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x692f354)
#<!-- -->19 0x0000000004b83928 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b83928)
#<!-- -->20 0x0000000004e71515 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e71515)
#<!-- -->21 0x0000000004df231e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4df231e)
#<!-- -->22 0x0000000004f69ced clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f69ced)
#<!-- -->23 0x0000000000dd0670 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdd0670)
#<!-- -->24 0x0000000000dc71da ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->25 0x0000000000dc735d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->26 0x0000000004bed1c9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->27 0x000000000417a304 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x417a304)
#<!-- -->28 0x0000000004bed7df clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->29 0x0000000004bae572 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bae572)
#<!-- -->30 0x0000000004baf51e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4baf51e)
#<!-- -->31 0x0000000004bb6c55 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bb6c55)
#<!-- -->32 0x0000000000dccb99 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdccb99)
#<!-- -->33 0x0000000000c7bb64 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc7bb64)
#<!-- -->34 0x00007a0d09429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->35 0x00007a0d09429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->36 0x0000000000dc6c75 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdc6c75)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

### Comment 3 - ecnelises

Suspicious commit: 86d1d6b2c0c1f03e82cb8e360f2672c6f0ea39d5

hi @sarnex is the deletion of NVPTX (and other cases) in `getDeviceKernelCallingConv` intended?

---

