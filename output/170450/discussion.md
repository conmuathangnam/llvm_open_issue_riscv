# [AArch64] crash on __scoped_atomic_uinc_wrap

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/170450
**Status:** Closed
**Labels:** backend:AArch64, crash
**Closed Date:** 2025-12-09T14:22:43Z

## Body

Reproducer:
https://godbolt.org/z/K1b3YT95e
```cpp
void foo(unsigned int *b) {
  *b = __scoped_atomic_uinc_wrap(b, ~0U, __ATOMIC_RELAXED, __MEMORY_SCOPE_DEVICE);
}
```

Backtrace:
```console
fatal error: error in backend: Cannot select: 0x14e14400: i32,ch = AtomicLoadUIncWrap<(load store monotonic (s32) on %ir.5)> 0x14e14390:1, 0x14e141d0, 0x14e14390, example.cpp:2:8
  0x14e141d0: i64,ch = load<(dereferenceable load (s64) from %ir.2)> 0x14e14160, FrameIndex:i64<0>, undef:i64, example.cpp:2:34
  0x14e14390: i32,ch = load<(dereferenceable load (s32) from %ir.3)> 0x14e14320, FrameIndex:i64<1>, undef:i64, example.cpp:2:8
In function: _Z3fooPj
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -g -o /app/output.s -fno-verbose-asm -S -target aarch64-linux-gnu --gcc-toolchain=/opt/compiler-explorer/arm64/gcc-15.2.0/aarch64-unknown-linux-gnu --sysroot=/opt/compiler-explorer/arm64/gcc-15.2.0/aarch64-unknown-linux-gnu/aarch64-unknown-linux-gnu/sysroot -fcolor-diagnostics -fno-crash-diagnostics -march=armv8.2-a <source>
1.	<eof> parser at end of file
2.	Code generation
3.	Running pass 'Function Pass Manager' on module '<source>'.
4.	Running pass 'AArch64 Instruction Selection' on function '@_Z3fooPj'
 #0 0x0000000003cdab88 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3cdab88)
 #1 0x0000000003cd855c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3cd855c)
 #2 0x0000000003c1e983 llvm::CrashRecoveryContext::HandleExit(int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c1e983)
 #3 0x0000000003ccfd9e llvm::sys::Process::Exit(int, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3ccfd9e)
 #4 0x0000000000dcb98b LLVMErrorHandler(void*, char const*, bool) cc1_main.cpp:0:0
 #5 0x0000000003c28be3 llvm::report_fatal_error(llvm::Twine const&, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c28be3)
 #6 0x00000000051dd1ea llvm::SelectionDAGISel::CannotYetSelect(llvm::SDNode*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x51dd1ea)
 #7 0x00000000051e2f9d llvm::SelectionDAGISel::SelectCodeCommon(llvm::SDNode*, unsigned char const*, unsigned int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x51e2f9d)
 #8 0x0000000000f77097 (anonymous namespace)::AArch64DAGToDAGISel::Select(llvm::SDNode*) AArch64ISelDAGToDAG.cpp:0:0
 #9 0x00000000051da2d4 llvm::SelectionDAGISel::DoInstructionSelection() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x51da2d4)
#10 0x00000000051e68b2 llvm::SelectionDAGISel::CodeGenAndEmitDAG() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x51e68b2)
#11 0x00000000051e91e1 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x51e91e1)
#12 0x00000000051eb195 llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x51eb195)
#13 0x00000000051d9e71 llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x51d9e71)
#14 0x00000000030ef21d llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x30ef21d)
#15 0x0000000003641652 llvm::FPPassManager::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3641652)
#16 0x00000000036418e1 llvm::FPPassManager::runOnModule(llvm::Module&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x36418e1)
#17 0x0000000003643157 llvm::legacy::PassManagerImpl::run(llvm::Module&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3643157)
#18 0x0000000003f6bd50 clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3f6bd50)
#19 0x0000000004598c2b clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4598c2b)
#20 0x00000000061c5e6c clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61c5e6c)
#21 0x0000000004599785 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4599785)
#22 0x000000000489ae2a clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x489ae2a)
#23 0x0000000004819d0b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4819d0b)
#24 0x000000000498f96b clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x498f96b)
#25 0x0000000000dcda95 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdcda95)
#26 0x0000000000dc594b ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#27 0x0000000000dc59ed int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#28 0x0000000004606ab9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#29 0x0000000003c1e883 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c1e883)
#30 0x0000000004606cd9 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#31 0x00000000045c9662 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45c9662)
#32 0x00000000045ca541 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45ca541)
#33 0x00000000045d312c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45d312c)
#34 0x0000000000dca419 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdca419)
#35 0x0000000000c74c74 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc74c74)
#36 0x0000712af6429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#37 0x0000712af6429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#38 0x0000000000dc53e5 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdc53e5)
clang++: error: clang frontend command failed with exit code 70 (use -v to see invocation)
Compiler returned: 70
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-aarch64

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/K1b3YT95e
```cpp
void foo(unsigned int *b) {
  *b = __scoped_atomic_uinc_wrap(b, ~0U, __ATOMIC_RELAXED, __MEMORY_SCOPE_DEVICE);
}
```

Backtrace:
```console
fatal error: error in backend: Cannot select: 0x14e14400: i32,ch = AtomicLoadUIncWrap&lt;(load store monotonic (s32) on %ir.5)&gt; 0x14e14390:1, 0x14e141d0, 0x14e14390, example.cpp:2:8
  0x14e141d0: i64,ch = load&lt;(dereferenceable load (s64) from %ir.2)&gt; 0x14e14160, FrameIndex:i64&lt;0&gt;, undef:i64, example.cpp:2:34
  0x14e14390: i32,ch = load&lt;(dereferenceable load (s32) from %ir.3)&gt; 0x14e14320, FrameIndex:i64&lt;1&gt;, undef:i64, example.cpp:2:8
In function: _Z3fooPj
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -g -o /app/output.s -fno-verbose-asm -S -target aarch64-linux-gnu --gcc-toolchain=/opt/compiler-explorer/arm64/gcc-15.2.0/aarch64-unknown-linux-gnu --sysroot=/opt/compiler-explorer/arm64/gcc-15.2.0/aarch64-unknown-linux-gnu/aarch64-unknown-linux-gnu/sysroot -fcolor-diagnostics -fno-crash-diagnostics -march=armv8.2-a &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	Code generation
3.	Running pass 'Function Pass Manager' on module '&lt;source&gt;'.
4.	Running pass 'AArch64 Instruction Selection' on function '@<!-- -->_Z3fooPj'
 #<!-- -->0 0x0000000003cdab88 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3cdab88)
 #<!-- -->1 0x0000000003cd855c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3cd855c)
 #<!-- -->2 0x0000000003c1e983 llvm::CrashRecoveryContext::HandleExit(int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c1e983)
 #<!-- -->3 0x0000000003ccfd9e llvm::sys::Process::Exit(int, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3ccfd9e)
 #<!-- -->4 0x0000000000dcb98b LLVMErrorHandler(void*, char const*, bool) cc1_main.cpp:0:0
 #<!-- -->5 0x0000000003c28be3 llvm::report_fatal_error(llvm::Twine const&amp;, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c28be3)
 #<!-- -->6 0x00000000051dd1ea llvm::SelectionDAGISel::CannotYetSelect(llvm::SDNode*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x51dd1ea)
 #<!-- -->7 0x00000000051e2f9d llvm::SelectionDAGISel::SelectCodeCommon(llvm::SDNode*, unsigned char const*, unsigned int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x51e2f9d)
 #<!-- -->8 0x0000000000f77097 (anonymous namespace)::AArch64DAGToDAGISel::Select(llvm::SDNode*) AArch64ISelDAGToDAG.cpp:0:0
 #<!-- -->9 0x00000000051da2d4 llvm::SelectionDAGISel::DoInstructionSelection() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x51da2d4)
#<!-- -->10 0x00000000051e68b2 llvm::SelectionDAGISel::CodeGenAndEmitDAG() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x51e68b2)
#<!-- -->11 0x00000000051e91e1 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x51e91e1)
#<!-- -->12 0x00000000051eb195 llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x51eb195)
#<!-- -->13 0x00000000051d9e71 llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x51d9e71)
#<!-- -->14 0x00000000030ef21d llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x30ef21d)
#<!-- -->15 0x0000000003641652 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3641652)
#<!-- -->16 0x00000000036418e1 llvm::FPPassManager::runOnModule(llvm::Module&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x36418e1)
#<!-- -->17 0x0000000003643157 llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3643157)
#<!-- -->18 0x0000000003f6bd50 clang::emitBackendOutput(clang::CompilerInstance&amp;, clang::CodeGenOptions&amp;, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;, std::unique_ptr&lt;llvm::raw_pwrite_stream, std::default_delete&lt;llvm::raw_pwrite_stream&gt;&gt;, clang::BackendConsumer*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3f6bd50)
#<!-- -->19 0x0000000004598c2b clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4598c2b)
#<!-- -->20 0x00000000061c5e6c clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61c5e6c)
#<!-- -->21 0x0000000004599785 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4599785)
#<!-- -->22 0x000000000489ae2a clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x489ae2a)
#<!-- -->23 0x0000000004819d0b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4819d0b)
#<!-- -->24 0x000000000498f96b clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x498f96b)
#<!-- -->25 0x0000000000dcda95 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdcda95)
#<!-- -->26 0x0000000000dc594b ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->27 0x0000000000dc59ed int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->28 0x0000000004606ab9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->29 0x0000000003c1e883 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c1e883)
#<!-- -->30 0x0000000004606cd9 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->31 0x00000000045c9662 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45c9662)
#<!-- -->32 0x00000000045ca541 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45ca541)
#<!-- -->33 0x00000000045d312c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45d312c)
#<!-- -->34 0x0000000000dca419 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdca419)
#<!-- -->35 0x0000000000c74c74 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc74c74)
#<!-- -->36 0x0000712af6429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->37 0x0000712af6429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->38 0x0000000000dc53e5 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdc53e5)
clang++: error: clang frontend command failed with exit code 70 (use -v to see invocation)
Compiler returned: 70
```
</details>


---

### Comment 2 - davemgreen

This should probably be expanded pre-isel again, similar to #167760. Probably in shouldExpandAtomicCmpXchgInIR this time.

---

