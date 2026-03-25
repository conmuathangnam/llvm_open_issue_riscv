# [AArch64][SVE] crash when using vector types with register constraints in a ternary operator

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/169027
**Status:** Open
**Labels:** backend:AArch64, SVE, crash

## Body

Reproducer:
https://godbolt.org/z/KWP5ed9zc
```cpp
typedef int vnx4si __attribute__((vector_size(32)));

void foo(int val) {
  register vnx4si x asm("z0");
  vnx4si z = {val, val, val, val, val, val, val, val};
  x = (vnx4si){0, 0, 0, 0, 0, 0, 0, 0} ? z : z;
  asm volatile("" ::"w"(x));
}
```

Backtrace:
```console
fatal error: error in backend: Cannot implicitly convert a scalable size to a fixed-width size in `TypeSize::operator ScalarTy()`
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -g -o /app/output.s -fno-verbose-asm -S -target aarch64-linux-gnu --gcc-toolchain=/opt/compiler-explorer/arm64/gcc-15.2.0/aarch64-unknown-linux-gnu --sysroot=/opt/compiler-explorer/arm64/gcc-15.2.0/aarch64-unknown-linux-gnu/aarch64-unknown-linux-gnu/sysroot -fcolor-diagnostics -fno-crash-diagnostics -march=armv8.2-a+sve <source>
1.	<eof> parser at end of file
2.	Code generation
3.	Running pass 'Function Pass Manager' on module '<source>'.
4.	Running pass 'AArch64 Instruction Selection' on function '@_Z3fooi'
 #0 0x0000000003c97ca8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c97ca8)
 #1 0x0000000003c9567c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c9567c)
 #2 0x0000000003bdbaa3 llvm::CrashRecoveryContext::HandleExit(int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3bdbaa3)
 #3 0x0000000003c8cebe llvm::sys::Process::Exit(int, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c8cebe)
 #4 0x0000000000dca0db LLVMErrorHandler(void*, char const*, bool) cc1_main.cpp:0:0
 #5 0x0000000003be5d03 llvm::report_fatal_error(llvm::Twine const&, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3be5d03)
 #6 0x0000000003be5e99 (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3be5e99)
 #7 0x0000000005040675 (anonymous namespace)::DAGCombiner::visitEXTRACT_VECTOR_ELT(llvm::SDNode*) DAGCombiner.cpp:0:0
 #8 0x0000000005079ed6 (anonymous namespace)::DAGCombiner::visit(llvm::SDNode*) DAGCombiner.cpp:0:0
 #9 0x000000000507ba35 (anonymous namespace)::DAGCombiner::combine(llvm::SDNode*) DAGCombiner.cpp:0:0
#10 0x000000000507c9b0 (anonymous namespace)::DAGCombiner::Run(llvm::CombineLevel) DAGCombiner.cpp:0:0
#11 0x000000000507fb44 llvm::SelectionDAG::Combine(llvm::CombineLevel, llvm::BatchAAResults*, llvm::CodeGenOptLevel) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x507fb44)
#12 0x00000000051a8f90 llvm::SelectionDAGISel::CodeGenAndEmitDAG() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x51a8f90)
#13 0x00000000051ab6d1 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x51ab6d1)
#14 0x00000000051ad67e llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x51ad67e)
#15 0x000000000519c621 llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x519c621)
#16 0x00000000030aa20d llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x30aa20d)
#17 0x00000000035fa682 llvm::FPPassManager::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x35fa682)
#18 0x00000000035fa911 llvm::FPPassManager::runOnModule(llvm::Module&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x35fa911)
#19 0x00000000035fc187 llvm::legacy::PassManagerImpl::run(llvm::Module&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x35fc187)
#20 0x0000000003f27da0 clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3f27da0)
#21 0x000000000455eacb clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x455eacb)
#22 0x000000000617719c clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x617719c)
#23 0x000000000455f625 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x455f625)
#24 0x00000000048625aa clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x48625aa)
#25 0x00000000047e13bb clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x47e13bb)
#26 0x00000000049585ab clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x49585ab)
#27 0x0000000000dcc1e5 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdcc1e5)
#28 0x0000000000dc409b ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#29 0x0000000000dc413d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#30 0x00000000045cc819 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#31 0x0000000003bdb9a3 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3bdb9a3)
#32 0x00000000045cca39 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#33 0x000000000458f472 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x458f472)
#34 0x0000000004590351 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4590351)
#35 0x00000000045990cc clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45990cc)
#36 0x0000000000dc8b69 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdc8b69)
#37 0x0000000000c73b24 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc73b24)
#38 0x00007f5658229d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#39 0x00007f5658229e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#40 0x0000000000dc3b35 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdc3b35)
clang++: error: clang frontend command failed with exit code 70 (use -v to see invocation)
Compiler returned: 70
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-aarch64

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/KWP5ed9zc
```cpp
typedef int vnx4si __attribute__((vector_size(32)));

void foo(int val) {
  register vnx4si x asm("z0");
  vnx4si z = {val, val, val, val, val, val, val, val};
  x = (vnx4si){0, 0, 0, 0, 0, 0, 0, 0} ? z : z;
  asm volatile("" ::"w"(x));
}
```

Backtrace:
```console
fatal error: error in backend: Cannot implicitly convert a scalable size to a fixed-width size in `TypeSize::operator ScalarTy()`
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -g -o /app/output.s -fno-verbose-asm -S -target aarch64-linux-gnu --gcc-toolchain=/opt/compiler-explorer/arm64/gcc-15.2.0/aarch64-unknown-linux-gnu --sysroot=/opt/compiler-explorer/arm64/gcc-15.2.0/aarch64-unknown-linux-gnu/aarch64-unknown-linux-gnu/sysroot -fcolor-diagnostics -fno-crash-diagnostics -march=armv8.2-a+sve &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	Code generation
3.	Running pass 'Function Pass Manager' on module '&lt;source&gt;'.
4.	Running pass 'AArch64 Instruction Selection' on function '@<!-- -->_Z3fooi'
 #<!-- -->0 0x0000000003c97ca8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c97ca8)
 #<!-- -->1 0x0000000003c9567c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c9567c)
 #<!-- -->2 0x0000000003bdbaa3 llvm::CrashRecoveryContext::HandleExit(int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3bdbaa3)
 #<!-- -->3 0x0000000003c8cebe llvm::sys::Process::Exit(int, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c8cebe)
 #<!-- -->4 0x0000000000dca0db LLVMErrorHandler(void*, char const*, bool) cc1_main.cpp:0:0
 #<!-- -->5 0x0000000003be5d03 llvm::report_fatal_error(llvm::Twine const&amp;, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3be5d03)
 #<!-- -->6 0x0000000003be5e99 (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3be5e99)
 #<!-- -->7 0x0000000005040675 (anonymous namespace)::DAGCombiner::visitEXTRACT_VECTOR_ELT(llvm::SDNode*) DAGCombiner.cpp:0:0
 #<!-- -->8 0x0000000005079ed6 (anonymous namespace)::DAGCombiner::visit(llvm::SDNode*) DAGCombiner.cpp:0:0
 #<!-- -->9 0x000000000507ba35 (anonymous namespace)::DAGCombiner::combine(llvm::SDNode*) DAGCombiner.cpp:0:0
#<!-- -->10 0x000000000507c9b0 (anonymous namespace)::DAGCombiner::Run(llvm::CombineLevel) DAGCombiner.cpp:0:0
#<!-- -->11 0x000000000507fb44 llvm::SelectionDAG::Combine(llvm::CombineLevel, llvm::BatchAAResults*, llvm::CodeGenOptLevel) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x507fb44)
#<!-- -->12 0x00000000051a8f90 llvm::SelectionDAGISel::CodeGenAndEmitDAG() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x51a8f90)
#<!-- -->13 0x00000000051ab6d1 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x51ab6d1)
#<!-- -->14 0x00000000051ad67e llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x51ad67e)
#<!-- -->15 0x000000000519c621 llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x519c621)
#<!-- -->16 0x00000000030aa20d llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x30aa20d)
#<!-- -->17 0x00000000035fa682 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x35fa682)
#<!-- -->18 0x00000000035fa911 llvm::FPPassManager::runOnModule(llvm::Module&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x35fa911)
#<!-- -->19 0x00000000035fc187 llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x35fc187)
#<!-- -->20 0x0000000003f27da0 clang::emitBackendOutput(clang::CompilerInstance&amp;, clang::CodeGenOptions&amp;, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;, std::unique_ptr&lt;llvm::raw_pwrite_stream, std::default_delete&lt;llvm::raw_pwrite_stream&gt;&gt;, clang::BackendConsumer*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3f27da0)
#<!-- -->21 0x000000000455eacb clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x455eacb)
#<!-- -->22 0x000000000617719c clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x617719c)
#<!-- -->23 0x000000000455f625 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x455f625)
#<!-- -->24 0x00000000048625aa clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x48625aa)
#<!-- -->25 0x00000000047e13bb clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x47e13bb)
#<!-- -->26 0x00000000049585ab clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x49585ab)
#<!-- -->27 0x0000000000dcc1e5 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdcc1e5)
#<!-- -->28 0x0000000000dc409b ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->29 0x0000000000dc413d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->30 0x00000000045cc819 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->31 0x0000000003bdb9a3 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3bdb9a3)
#<!-- -->32 0x00000000045cca39 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->33 0x000000000458f472 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x458f472)
#<!-- -->34 0x0000000004590351 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4590351)
#<!-- -->35 0x00000000045990cc clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45990cc)
#<!-- -->36 0x0000000000dc8b69 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdc8b69)
#<!-- -->37 0x0000000000c73b24 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc73b24)
#<!-- -->38 0x00007f5658229d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->39 0x00007f5658229e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->40 0x0000000000dc3b35 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdc3b35)
clang++: error: clang frontend command failed with exit code 70 (use -v to see invocation)
Compiler returned: 70
```
</details>


---

