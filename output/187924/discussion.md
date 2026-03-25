# [X86][CodeView][APX] fatal error: error in backend: unknown codeview register R16-R31

**Author:** Andarwinux
**URL:** https://github.com/llvm/llvm-project/issues/187924
**Status:** Open
**Labels:** backend:X86, crash

## Body

https://aaa.godbolt.org/z/bvdYjcxET

```
fatal error: error in backend: unknown codeview register R16
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -O3 -march=novalake --target=x86_64-pc-windows-gnu -gcodeview <source>
1.	<eof> parser at end of file
2.	Code generation
3.	Running pass 'Function Pass Manager' on module '<source>'.
4.	Running pass 'X86 Assembly Printer' on function '@test'
 #0 0x0000000003e6e848 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3e6e848)
 #1 0x0000000003e6c22c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3e6c22c)
 #2 0x0000000003daf013 llvm::CrashRecoveryContext::HandleExit(int) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3daf013)
 #3 0x0000000003e6394e llvm::sys::Process::Exit(int, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3e6394e)
 #4 0x0000000000deaf8b LLVMErrorHandler(void*, char const*, bool) cc1_main.cpp:0:0
 #5 0x0000000003db7ab3 llvm::report_fatal_error(llvm::Twine const&, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3db7ab3)
 #6 0x0000000003b663bc llvm::MCRegisterInfo::getCodeViewRegNum(llvm::MCRegister) const (/opt/compiler-explorer/clang-trunk/bin/clang+0x3b663bc)
 #7 0x0000000004f093b2 llvm::CodeViewDebug::calculateRanges(llvm::CodeViewDebug::LocalVariable&, llvm::SmallVector<llvm::DbgValueHistoryMap::Entry, 4u> const&) (/opt/compiler-explorer/clang-trunk/bin/clang+0x4f093b2)
 #8 0x0000000004f0cd50 llvm::CodeViewDebug::collectVariableInfo(llvm::DISubprogram const*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x4f0cd50)
 #9 0x0000000004f0dad1 llvm::CodeViewDebug::endFunctionImpl(llvm::MachineFunction const*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x4f0dad1)
#10 0x0000000004e880bb llvm::DebugHandlerBase::endFunction(llvm::MachineFunction const*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x4e880bb)
#11 0x0000000004e80d12 llvm::AsmPrinter::emitFunctionBody() (/opt/compiler-explorer/clang-trunk/bin/clang+0x4e80d12)
#12 0x0000000002812393 llvm::X86AsmPrinter::runOnMachineFunction(llvm::MachineFunction&) (/opt/compiler-explorer/clang-trunk/bin/clang+0x2812393)
#13 0x000000000323faed llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-trunk/bin/clang+0x323faed)
#14 0x00000000037a7872 llvm::FPPassManager::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-trunk/bin/clang+0x37a7872)
#15 0x00000000037a7b01 llvm::FPPassManager::runOnModule(llvm::Module&) (/opt/compiler-explorer/clang-trunk/bin/clang+0x37a7b01)
#16 0x00000000037a9377 llvm::legacy::PassManagerImpl::run(llvm::Module&) (/opt/compiler-explorer/clang-trunk/bin/clang+0x37a9377)
#17 0x0000000004224a0e clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x4224a0e)
#18 0x00000000048677a9 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/opt/compiler-explorer/clang-trunk/bin/clang+0x48677a9)
#19 0x0000000006491e9c clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+0x6491e9c)
#20 0x0000000004868305 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+0x4868305)
#21 0x0000000004b97b2a clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+0x4b97b2a)
#22 0x0000000004b1363b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-trunk/bin/clang+0x4b1363b)
#23 0x0000000004c9370b clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x4c9370b)
#24 0x0000000000ded185 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+0xded185)
#25 0x0000000000de4e4b ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#26 0x0000000000de4eed int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#27 0x00000000048d5519 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#28 0x0000000003daef13 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3daef13)
#29 0x00000000048d5739 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#30 0x0000000004897782 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+0x4897782)
#31 0x0000000004898661 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+0x4898661)
#32 0x00000000048a117c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-trunk/bin/clang+0x48a117c)
#33 0x0000000000de9961 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-trunk/bin/clang+0xde9961)
#34 0x0000000000c982ea main (/opt/compiler-explorer/clang-trunk/bin/clang+0xc982ea)
#35 0x0000762d39629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#36 0x0000762d39629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#37 0x0000000000de48e5 _start (/opt/compiler-explorer/clang-trunk/bin/clang+0xde48e5)
clang: error: clang frontend command failed with exit code 70 (use -v to see invocation)
Compiler returned: 70
```


## Comments

### Comment 1 - Andarwinux

@phoebewang Should EGPR generation be disabled on Windows until Microsoft ships support for APX?

---

### Comment 2 - phoebewang

EGPR is the key feature of APX. Disabling it may mislead users who want to try with it. I think emitting an error when codeview is used would be better.

---

### Comment 3 - llvmbot


@llvm/issue-subscribers-backend-x86

Author: None (Andarwinux)

<details>
https://aaa.godbolt.org/z/bvdYjcxET

```
fatal error: error in backend: unknown codeview register R16
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -O3 -march=novalake --target=x86_64-pc-windows-gnu -gcodeview &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	Code generation
3.	Running pass 'Function Pass Manager' on module '&lt;source&gt;'.
4.	Running pass 'X86 Assembly Printer' on function '@<!-- -->test'
 #<!-- -->0 0x0000000003e6e848 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3e6e848)
 #<!-- -->1 0x0000000003e6c22c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3e6c22c)
 #<!-- -->2 0x0000000003daf013 llvm::CrashRecoveryContext::HandleExit(int) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3daf013)
 #<!-- -->3 0x0000000003e6394e llvm::sys::Process::Exit(int, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3e6394e)
 #<!-- -->4 0x0000000000deaf8b LLVMErrorHandler(void*, char const*, bool) cc1_main.cpp:0:0
 #<!-- -->5 0x0000000003db7ab3 llvm::report_fatal_error(llvm::Twine const&amp;, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3db7ab3)
 #<!-- -->6 0x0000000003b663bc llvm::MCRegisterInfo::getCodeViewRegNum(llvm::MCRegister) const (/opt/compiler-explorer/clang-trunk/bin/clang+0x3b663bc)
 #<!-- -->7 0x0000000004f093b2 llvm::CodeViewDebug::calculateRanges(llvm::CodeViewDebug::LocalVariable&amp;, llvm::SmallVector&lt;llvm::DbgValueHistoryMap::Entry, 4u&gt; const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x4f093b2)
 #<!-- -->8 0x0000000004f0cd50 llvm::CodeViewDebug::collectVariableInfo(llvm::DISubprogram const*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x4f0cd50)
 #<!-- -->9 0x0000000004f0dad1 llvm::CodeViewDebug::endFunctionImpl(llvm::MachineFunction const*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x4f0dad1)
#<!-- -->10 0x0000000004e880bb llvm::DebugHandlerBase::endFunction(llvm::MachineFunction const*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x4e880bb)
#<!-- -->11 0x0000000004e80d12 llvm::AsmPrinter::emitFunctionBody() (/opt/compiler-explorer/clang-trunk/bin/clang+0x4e80d12)
#<!-- -->12 0x0000000002812393 llvm::X86AsmPrinter::runOnMachineFunction(llvm::MachineFunction&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x2812393)
#<!-- -->13 0x000000000323faed llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x323faed)
#<!-- -->14 0x00000000037a7872 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x37a7872)
#<!-- -->15 0x00000000037a7b01 llvm::FPPassManager::runOnModule(llvm::Module&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x37a7b01)
#<!-- -->16 0x00000000037a9377 llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x37a9377)
#<!-- -->17 0x0000000004224a0e clang::emitBackendOutput(clang::CompilerInstance&amp;, clang::CodeGenOptions&amp;, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;, std::unique_ptr&lt;llvm::raw_pwrite_stream, std::default_delete&lt;llvm::raw_pwrite_stream&gt;&gt;, clang::BackendConsumer*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x4224a0e)
#<!-- -->18 0x00000000048677a9 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x48677a9)
#<!-- -->19 0x0000000006491e9c clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+0x6491e9c)
#<!-- -->20 0x0000000004868305 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+0x4868305)
#<!-- -->21 0x0000000004b97b2a clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+0x4b97b2a)
#<!-- -->22 0x0000000004b1363b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x4b1363b)
#<!-- -->23 0x0000000004c9370b clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x4c9370b)
#<!-- -->24 0x0000000000ded185 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+0xded185)
#<!-- -->25 0x0000000000de4e4b ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->26 0x0000000000de4eed int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->27 0x00000000048d5519 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->28 0x0000000003daef13 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3daef13)
#<!-- -->29 0x00000000048d5739 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->30 0x0000000004897782 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+0x4897782)
#<!-- -->31 0x0000000004898661 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+0x4898661)
#<!-- -->32 0x00000000048a117c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x48a117c)
#<!-- -->33 0x0000000000de9961 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0xde9961)
#<!-- -->34 0x0000000000c982ea main (/opt/compiler-explorer/clang-trunk/bin/clang+0xc982ea)
#<!-- -->35 0x0000762d39629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->36 0x0000762d39629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->37 0x0000000000de48e5 _start (/opt/compiler-explorer/clang-trunk/bin/clang+0xde48e5)
clang: error: clang frontend command failed with exit code 70 (use -v to see invocation)
Compiler returned: 70
```

</details>


---

### Comment 4 - efriedma-quic

CC @vzarytovskii

---

### Comment 5 - vzarytovskii

> CC @vzarytovskii

Thanks for the ping, we're working on the APX support in dia, but can't currently share anything specific. We will release a public announcement when it will be ready. 

---

