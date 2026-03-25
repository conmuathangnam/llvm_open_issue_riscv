# [clang][SYCL] Unreacheable executed for SYCL device compilation

**Author:** Fznamznon
**URL:** https://github.com/llvm/llvm-project/issues/167358
**Status:** Open
**Labels:** clang:codegen, crash, SYCL

## Body

For the following sample (https://godbolt.org/z/W4d7G148K):
```
template <typename KN, typename KT>
[[clang::sycl_kernel_entry_point(KN)]] void k(KT) {}

int main() {
    k<struct K>([](){});
    return 0;
}
```

Compiled with:
```
clang -cc1 t.cpp -fsycl-is-device -emit-llvm
```

results in the following assertion hit
```
Unknown kernel calling convention
UNREACHABLE executed at /root/llvm-project/llvm/tools/clang/lib/CodeGen/TargetInfo.cpp:129!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -Xclang -fsycl-is-device <source>
1.	<eof> parser at end of file
2.	Per-file LLVM IR generation
 #0 0x000000000424f278 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x424f278)
 #1 0x000000000424c6a4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x424c6a4)
 #2 0x0000000004190908 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007deb48a42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007deb48a969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007deb48a42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007deb48a287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x000000000419c4ea (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x419c4ea)
 #8 0x00000000049bc8a0 clang::CodeGen::TargetCodeGenInfo::createEnqueuedBlockKernel(clang::CodeGen::CodeGenFunction&, llvm::Function*, llvm::Type*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49bc8a0)
 #9 0x0000000004a92f60 clang::CodeGen::CodeGenTypes::arrangeLLVMFunctionInfo(clang::CanQual<clang::Type>, clang::CodeGen::FnInfoOpts, llvm::ArrayRef<clang::CanQual<clang::Type>>, clang::FunctionType::ExtInfo, llvm::ArrayRef<clang::FunctionType::ExtParameterInfo>, clang::CodeGen::RequiredArgs) (.constprop.0) CGCall.cpp:0:0
#10 0x0000000004a93912 clang::CodeGen::CodeGenTypes::arrangeDeviceKernelCallerDeclaration(clang::QualType, clang::CodeGen::FunctionArgList const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4a93912)
#11 0x000000000487652e clang::CodeGen::CodeGenModule::EmitSYCLKernelCaller(clang::FunctionDecl const*, clang::ASTContext&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x487652e)
#12 0x000000000483cd2c clang::CodeGen::CodeGenModule::EmitDeferred() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x483cd2c)
#13 0x000000000483f1e3 clang::CodeGen::CodeGenModule::Release() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x483f1e3)
#14 0x0000000004b9f54e (anonymous namespace)::CodeGeneratorImpl::HandleTranslationUnit(clang::ASTContext&) ModuleBuilder.cpp:0:0
#15 0x0000000004b9b555 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b9b555)
#16 0x000000000694c15c clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x694c15c)
#17 0x0000000004b9bd38 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b9bd38)
#18 0x0000000004e8a285 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e8a285)
#19 0x0000000004e0b08e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e0b08e)
#20 0x0000000004f8242d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f8242d)
#21 0x0000000000dd3d00 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdd3d00)
#22 0x0000000000dca87a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#23 0x0000000000dca9fd int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#24 0x0000000004c05bb9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#25 0x0000000004190da4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4190da4)
#26 0x0000000004c061cf clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#27 0x0000000004bc6c22 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bc6c22)
#28 0x0000000004bc7bce clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bc7bce)
#29 0x0000000004bcf325 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bcf325)
#30 0x0000000000dd0221 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdd0221)
#31 0x0000000000c7e7b4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc7e7b4)
#32 0x00007deb48a29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#33 0x00007deb48a29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#34 0x0000000000dca315 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdca315)
```
this happens when a target supplied is not a correct offloading target. It should be either accepted or rejected with a message saying that the target is not a valid offloading target for sycl device compilation.

This issue is annoying because each time the target is forgotten in the command line, the host target is substituted and it is usually not an offloading target.

## Comments

### Comment 1 - Fznamznon

FYI @elizabethandrews 

---

### Comment 2 - tahonermann

@Fznamznon, I updated the issue description to adjust the compiler command line needed to reproduce the issue in recent Clang builds. I'm not able to reproduce the issue using a `-cc1` command line that doesn't explicitly specify a target; the default device target seems to be spirv when run on x86-64 Linux. I'm guessing this was a recent change; I haven't looked for a relevant git commit. We clearly need to issue an appropriate diagnostic to avoid the crash, but can you validate that the default target concern is no longer an issue?

---

### Comment 3 - Fznamznon

> We clearly need to issue an appropriate diagnostic to avoid the crash, but can you validate that the default target concern is no longer an issue?

No, it is still an issue.
With clang built with commit 711a2954799e597c71b86aed8c93167765a5255f on top, using the command `clang -cc1 t.cpp -fsycl-is-device -emit-llvm` on my Linux x86_64 machine I get the following
```
Unknown kernel calling convention
UNREACHABLE executed at source/llorg/llvm-project/clang/lib/CodeGen/TargetInfo.cpp:129!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: source/llorg/llvm-project/build/bin/clang -cc1 t.cpp -fsycl-is-device -emit-llvm
1.      <eof> parser at end of file
2.      Per-file LLVM IR generation
 #0 0x00000000021b5f21 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (source/llorg/llvm-project/build/bin/clang+0x21b5f21)
 #1 0x00000000021b2d95 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x00007f7d00ee5db0 __restore_rt (/lib64/libc.so.6+0x59db0)
 #3 0x00007f7d00f3242c __pthread_kill_implementation (/lib64/libc.so.6+0xa642c)
 #4 0x00007f7d00ee5d06 gsignal (/lib64/libc.so.6+0x59d06)
 #5 0x00007f7d00eb87d3 abort (/lib64/libc.so.6+0x2c7d3)
 #6 0x000000000210c81a (source/llorg/llvm-project/build/bin/clang+0x210c81a)
 #7 0x00000000028ca1b7 (source/llorg/llvm-project/build/bin/clang+0x28ca1b7)
 #8 0x00000000029a5e12 clang::CodeGen::CodeGenTypes::arrangeLLVMFunctionInfo(clang::CanQual<clang::Type>, clang::CodeGen::FnInfoOpts, llvm::ArrayRef<clang::CanQual<clang::Type>>, clang::FunctionType::ExtInfo, llvm::ArrayRef<clang::FunctionType::ExtParameterInfo>, clang::CodeGen::RequiredArgs) (.constprop.0) CGCall.cpp:0:0
 #9 0x00000000029a67df clang::CodeGen::CodeGenTypes::arrangeDeviceKernelCallerDeclaration(clang::QualType, clang::CodeGen::FunctionArgList const&) (source/llorg/llvm-project/build/bin/clang+0x29a67df)
#10 0x00000000027870e1 clang::CodeGen::CodeGenModule::EmitSYCLKernelCaller(clang::FunctionDecl const*, clang::ASTContext&) (source/llorg/llvm-project/build/bin/clang+0x27870e1)
#11 0x000000000274e6d4 clang::CodeGen::CodeGenModule::EmitDeferred() (source/llorg/llvm-project/build/bin/clang+0x274e6d4)
#12 0x0000000002750a31 clang::CodeGen::CodeGenModule::Release() (source/llorg/llvm-project/build/bin/clang+0x2750a31)
#13 0x0000000002aafc6e (anonymous namespace)::CodeGeneratorImpl::HandleTranslationUnit(clang::ASTContext&) ModuleBuilder.cpp:0:0
#14 0x0000000002aad36d clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (source/llorg/llvm-project/build/bin/clang+0x2aad36d)
#15 0x00000000045dbc9c clang::ParseAST(clang::Sema&, bool, bool) (source/llorg/llvm-project/build/bin/clang+0x45dbc9c)
#16 0x0000000002dcaf75 clang::FrontendAction::Execute() (source/llorg/llvm-project/build/bin/clang+0x2dcaf75)
#17 0x0000000002d4c0b5 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (source/llorg/llvm-project/build/bin/clang+0x2d4c0b5)
#18 0x0000000002eb8b83 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (source/llorg/llvm-project/build/bin/clang+0x2eb8b83)
#19 0x0000000000c1f827 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (source/llorg/llvm-project/build/bin/clang+0xc1f827)
#20 0x0000000000c165ba ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#21 0x0000000000c1a2b5 clang_main(int, char**, llvm::ToolContext const&) (source/llorg/llvm-project/build/bin/clang+0xc1a2b5)
#22 0x0000000000b36b54 main (source/llorg/llvm-project/build/bin/clang+0xb36b54)
#23 0x00007f7d00ed0e50 __libc_start_call_main (/lib64/libc.so.6+0x44e50)
#24 0x00007f7d00ed0efc __libc_start_main@GLIBC_2.2.5 (/lib64/libc.so.6+0x44efc)
#25 0x0000000000c15845 _start (source/llorg/llvm-project/build/bin/clang+0xc15845)
Aborted (core dumped)
```

I updated the command back because invoking driver makes no sense for debugging clang.
I think I forgot the sycl flag in the original command, so my bad.

---

### Comment 4 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: Mariya Podchishchaeva (Fznamznon)

<details>
For the following sample (https://godbolt.org/z/W4d7G148K):
```
template &lt;typename KN, typename KT&gt;
[[clang::sycl_kernel_entry_point(KN)]] void k(KT) {}

int main() {
    k&lt;struct K&gt;([](){});
    return 0;
}
```

Compiled with:
```
clang -cc1 t.cpp -fsycl-is-device -emit-llvm
```

results in the following assertion hit
```
Unknown kernel calling convention
UNREACHABLE executed at /root/llvm-project/llvm/tools/clang/lib/CodeGen/TargetInfo.cpp:129!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -Xclang -fsycl-is-device &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	Per-file LLVM IR generation
 #<!-- -->0 0x000000000424f278 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x424f278)
 #<!-- -->1 0x000000000424c6a4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x424c6a4)
 #<!-- -->2 0x0000000004190908 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007deb48a42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007deb48a969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007deb48a42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007deb48a287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x000000000419c4ea (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x419c4ea)
 #<!-- -->8 0x00000000049bc8a0 clang::CodeGen::TargetCodeGenInfo::createEnqueuedBlockKernel(clang::CodeGen::CodeGenFunction&amp;, llvm::Function*, llvm::Type*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49bc8a0)
 #<!-- -->9 0x0000000004a92f60 clang::CodeGen::CodeGenTypes::arrangeLLVMFunctionInfo(clang::CanQual&lt;clang::Type&gt;, clang::CodeGen::FnInfoOpts, llvm::ArrayRef&lt;clang::CanQual&lt;clang::Type&gt;&gt;, clang::FunctionType::ExtInfo, llvm::ArrayRef&lt;clang::FunctionType::ExtParameterInfo&gt;, clang::CodeGen::RequiredArgs) (.constprop.0) CGCall.cpp:0:0
#<!-- -->10 0x0000000004a93912 clang::CodeGen::CodeGenTypes::arrangeDeviceKernelCallerDeclaration(clang::QualType, clang::CodeGen::FunctionArgList const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4a93912)
#<!-- -->11 0x000000000487652e clang::CodeGen::CodeGenModule::EmitSYCLKernelCaller(clang::FunctionDecl const*, clang::ASTContext&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x487652e)
#<!-- -->12 0x000000000483cd2c clang::CodeGen::CodeGenModule::EmitDeferred() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x483cd2c)
#<!-- -->13 0x000000000483f1e3 clang::CodeGen::CodeGenModule::Release() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x483f1e3)
#<!-- -->14 0x0000000004b9f54e (anonymous namespace)::CodeGeneratorImpl::HandleTranslationUnit(clang::ASTContext&amp;) ModuleBuilder.cpp:0:0
#<!-- -->15 0x0000000004b9b555 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b9b555)
#<!-- -->16 0x000000000694c15c clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x694c15c)
#<!-- -->17 0x0000000004b9bd38 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b9bd38)
#<!-- -->18 0x0000000004e8a285 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e8a285)
#<!-- -->19 0x0000000004e0b08e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e0b08e)
#<!-- -->20 0x0000000004f8242d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f8242d)
#<!-- -->21 0x0000000000dd3d00 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdd3d00)
#<!-- -->22 0x0000000000dca87a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->23 0x0000000000dca9fd int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->24 0x0000000004c05bb9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->25 0x0000000004190da4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4190da4)
#<!-- -->26 0x0000000004c061cf clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->27 0x0000000004bc6c22 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bc6c22)
#<!-- -->28 0x0000000004bc7bce clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bc7bce)
#<!-- -->29 0x0000000004bcf325 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bcf325)
#<!-- -->30 0x0000000000dd0221 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdd0221)
#<!-- -->31 0x0000000000c7e7b4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc7e7b4)
#<!-- -->32 0x00007deb48a29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->33 0x00007deb48a29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->34 0x0000000000dca315 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdca315)
```
this happens when a target supplied is not a correct offloading target. It should be either accepted or rejected with a message saying that the target is not a valid offloading target for sycl device compilation.

This issue is annoying because each time the target is forgotten in the command line, the host target is substituted and it is usually not an offloading target.
</details>


---

### Comment 5 - elizabethandrews

I can reproduce this locally without providing a triple as well.

---

