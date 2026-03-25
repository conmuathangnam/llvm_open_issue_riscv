# Clang crash when targetting 32 bit iOS with some ObjC++ when stack protector enabled

**Author:** Un1q32
**URL:** https://github.com/llvm/llvm-project/issues/118175
**Status:** Closed
**Labels:** llvm:codegen, crash
**Closed Date:** 2025-09-04T01:51:20Z

## Body

Happens with armv6, armv7, and armv7s but NOT armv5 or armv4t
Only happens when compiling without optimizations
Also happens with the latest Apple clang 1600.0.26.4 shipped with Xcode 16.1, but NOT with 1500.3.9.4 shipped with Xcode 15.4

```
0.	Program arguments: clang++ -target armv7-apple-ios -isysroot iossdk -Fsysroot/Library/Frameworks -c MobileCydia.mm -F/home/uniq/devel/iphoneports-cydia/iossdk/System/Library/PrivateFrameworks -I. -isystem sysroot/var/usr/include -isystem sysroot/usr/include -w -stdlib=libstdc++ -std=c++03
1.	<eof> parser at end of file
2.	Code generation
3.	Running pass 'Function Pass Manager' on module 'MobileCydia.mm'.
4.	Running pass 'ARM Assembly Printer' on function '@"\01-[Database _readCydia:]"'
 #0 0x00005be564706488 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/home/uniq/LLVM-19.1.4-Linux-X64/bin/clang-19+0x695d488)
 #1 0x00005be56470391e llvm::sys::RunSignalHandlers() (/home/uniq/LLVM-19.1.4-Linux-X64/bin/clang-19+0x695a91e)
 #2 0x00005be564660597 CrashRecoverySignalHandler(int) (.llvm.2746408055151269361) CrashRecoveryContext.cpp:0:0
 #3 0x00007c31fe84c1d0 (/usr/lib/libc.so.6+0x3d1d0)
 #4 0x00005be5658f0aed llvm::EHStreamer::computeCallSiteTable(llvm::SmallVectorImpl<llvm::EHStreamer::CallSiteEntry>&, llvm::SmallVectorImpl<llvm::EHStreamer::CallSiteRange>&, llvm::SmallVectorImpl<llvm::LandingPadInfo const*> const&, llvm::SmallVectorImpl<unsigned int> const&) EHStreamer.cpp:0:0
 #5 0x00005be5658f163b llvm::EHStreamer::emitExceptionTable() EHStreamer.cpp:0:0
 #6 0x00005be56587935e llvm::AsmPrinter::emitFunctionBody() (/home/uniq/LLVM-19.1.4-Linux-X64/bin/clang-19+0x7ad035e)
 #7 0x00005be562857536 llvm::ARMAsmPrinter::runOnMachineFunction(llvm::MachineFunction&) ARMAsmPrinter.cpp:0:0
 #8 0x00005be563be7ed9 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (/home/uniq/LLVM-19.1.4-Linux-X64/bin/clang-19+0x5e3eed9)
 #9 0x00005be5640d86af llvm::FPPassManager::runOnFunction(llvm::Function&) (/home/uniq/LLVM-19.1.4-Linux-X64/bin/clang-19+0x632f6af)
#10 0x00005be5640e24f3 llvm::FPPassManager::runOnModule(llvm::Module&) (/home/uniq/LLVM-19.1.4-Linux-X64/bin/clang-19+0x63394f3)
#11 0x00005be5640d95c7 llvm::legacy::PassManagerImpl::run(llvm::Module&) (/home/uniq/LLVM-19.1.4-Linux-X64/bin/clang-19+0x63305c7)
#12 0x00005be564fa78e3 clang::EmitBackendOutput(clang::DiagnosticsEngine&, clang::HeaderSearchOptions const&, clang::CodeGenOptions const&, clang::TargetOptions const&, clang::LangOptions const&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/home/uniq/LLVM-19.1.4-Linux-X64/bin/clang-19+0x71fe8e3)
#13 0x00005be564fbf47d clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/home/uniq/LLVM-19.1.4-Linux-X64/bin/clang-19+0x721647d)
#14 0x00005be567396b89 clang::ParseAST(clang::Sema&, bool, bool) (/home/uniq/LLVM-19.1.4-Linux-X64/bin/clang-19+0x95edb89)
#15 0x00005be56549a4c4 clang::FrontendAction::Execute() (/home/uniq/LLVM-19.1.4-Linux-X64/bin/clang-19+0x76f14c4)
#16 0x00005be5653d35f0 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/home/uniq/LLVM-19.1.4-Linux-X64/bin/clang-19+0x762a5f0)
#17 0x00005be56559d0fa clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/home/uniq/LLVM-19.1.4-Linux-X64/bin/clang-19+0x77f40fa)
#18 0x00005be56201e58f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/home/uniq/LLVM-19.1.4-Linux-X64/bin/clang-19+0x427558f)
#19 0x00005be56201a5bf ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#20 0x00005be5652251a9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::$_0>(long) Job.cpp:0:0
#21 0x00005be564660158 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/home/uniq/LLVM-19.1.4-Linux-X64/bin/clang-19+0x68b7158)
#22 0x00005be565224df4 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (/home/uniq/LLVM-19.1.4-Linux-X64/bin/clang-19+0x747bdf4)
#23 0x00005be5651e264b clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/home/uniq/LLVM-19.1.4-Linux-X64/bin/clang-19+0x743964b)
#24 0x00005be56520266b clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/home/uniq/LLVM-19.1.4-Linux-X64/bin/clang-19+0x745966b)
#25 0x00005be5620199ee clang_main(int, char**, llvm::ToolContext const&) (/home/uniq/LLVM-19.1.4-Linux-X64/bin/clang-19+0x42709ee)
#26 0x00005be56202c57a main (/home/uniq/LLVM-19.1.4-Linux-X64/bin/clang-19+0x428357a)
#27 0x00007c31fe834e08 (/usr/lib/libc.so.6+0x25e08)
#28 0x00007c31fe834ecc __libc_start_main (/usr/lib/libc.so.6+0x25ecc)
#29 0x00005be56200c265 _start (/home/uniq/LLVM-19.1.4-Linux-X64/bin/clang-19+0x4263265)
```

Preprocessed source: [MobileCydia-5b9246.mm](https://github.com/user-attachments/files/17965810/MobileCydia-5b9246.mm.txt)
Run script: [MobileCydia-5b9246.sh](https://github.com/user-attachments/files/17965811/MobileCydia-5b9246.sh.txt)


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-arm

Author: None (Un1q32)

<details>
Happens with armv6, armv7, and armv7s but NOT armv5 or armv4t
Only happens when compiling without optimizations
Also happens with the latest Apple clang 1600.0.26.4 shipped with Xcode 16.1, but NOT with 1500.3.9.4 shipped with Xcode 15.4

```
0.	Program arguments: clang++ -target armv7-apple-ios -isysroot iossdk -Fsysroot/Library/Frameworks -c MobileCydia.mm -F/home/uniq/devel/iphoneports-cydia/iossdk/System/Library/PrivateFrameworks -I. -isystem sysroot/var/usr/include -isystem sysroot/usr/include -w -stdlib=libstdc++ -std=c++03
1.	&lt;eof&gt; parser at end of file
2.	Code generation
3.	Running pass 'Function Pass Manager' on module 'MobileCydia.mm'.
4.	Running pass 'ARM Assembly Printer' on function '@"\01-[Database _readCydia:]"'
 #<!-- -->0 0x00005be564706488 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/home/uniq/LLVM-19.1.4-Linux-X64/bin/clang-19+0x695d488)
 #<!-- -->1 0x00005be56470391e llvm::sys::RunSignalHandlers() (/home/uniq/LLVM-19.1.4-Linux-X64/bin/clang-19+0x695a91e)
 #<!-- -->2 0x00005be564660597 CrashRecoverySignalHandler(int) (.llvm.2746408055151269361) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007c31fe84c1d0 (/usr/lib/libc.so.6+0x3d1d0)
 #<!-- -->4 0x00005be5658f0aed llvm::EHStreamer::computeCallSiteTable(llvm::SmallVectorImpl&lt;llvm::EHStreamer::CallSiteEntry&gt;&amp;, llvm::SmallVectorImpl&lt;llvm::EHStreamer::CallSiteRange&gt;&amp;, llvm::SmallVectorImpl&lt;llvm::LandingPadInfo const*&gt; const&amp;, llvm::SmallVectorImpl&lt;unsigned int&gt; const&amp;) EHStreamer.cpp:0:0
 #<!-- -->5 0x00005be5658f163b llvm::EHStreamer::emitExceptionTable() EHStreamer.cpp:0:0
 #<!-- -->6 0x00005be56587935e llvm::AsmPrinter::emitFunctionBody() (/home/uniq/LLVM-19.1.4-Linux-X64/bin/clang-19+0x7ad035e)
 #<!-- -->7 0x00005be562857536 llvm::ARMAsmPrinter::runOnMachineFunction(llvm::MachineFunction&amp;) ARMAsmPrinter.cpp:0:0
 #<!-- -->8 0x00005be563be7ed9 llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (/home/uniq/LLVM-19.1.4-Linux-X64/bin/clang-19+0x5e3eed9)
 #<!-- -->9 0x00005be5640d86af llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (/home/uniq/LLVM-19.1.4-Linux-X64/bin/clang-19+0x632f6af)
#<!-- -->10 0x00005be5640e24f3 llvm::FPPassManager::runOnModule(llvm::Module&amp;) (/home/uniq/LLVM-19.1.4-Linux-X64/bin/clang-19+0x63394f3)
#<!-- -->11 0x00005be5640d95c7 llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/home/uniq/LLVM-19.1.4-Linux-X64/bin/clang-19+0x63305c7)
#<!-- -->12 0x00005be564fa78e3 clang::EmitBackendOutput(clang::DiagnosticsEngine&amp;, clang::HeaderSearchOptions const&amp;, clang::CodeGenOptions const&amp;, clang::TargetOptions const&amp;, clang::LangOptions const&amp;, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;, std::unique_ptr&lt;llvm::raw_pwrite_stream, std::default_delete&lt;llvm::raw_pwrite_stream&gt;&gt;, clang::BackendConsumer*) (/home/uniq/LLVM-19.1.4-Linux-X64/bin/clang-19+0x71fe8e3)
#<!-- -->13 0x00005be564fbf47d clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&amp;) (/home/uniq/LLVM-19.1.4-Linux-X64/bin/clang-19+0x721647d)
#<!-- -->14 0x00005be567396b89 clang::ParseAST(clang::Sema&amp;, bool, bool) (/home/uniq/LLVM-19.1.4-Linux-X64/bin/clang-19+0x95edb89)
#<!-- -->15 0x00005be56549a4c4 clang::FrontendAction::Execute() (/home/uniq/LLVM-19.1.4-Linux-X64/bin/clang-19+0x76f14c4)
#<!-- -->16 0x00005be5653d35f0 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/home/uniq/LLVM-19.1.4-Linux-X64/bin/clang-19+0x762a5f0)
#<!-- -->17 0x00005be56559d0fa clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/home/uniq/LLVM-19.1.4-Linux-X64/bin/clang-19+0x77f40fa)
#<!-- -->18 0x00005be56201e58f cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/home/uniq/LLVM-19.1.4-Linux-X64/bin/clang-19+0x427558f)
#<!-- -->19 0x00005be56201a5bf ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->20 0x00005be5652251a9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::$_0&gt;(long) Job.cpp:0:0
#<!-- -->21 0x00005be564660158 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/home/uniq/LLVM-19.1.4-Linux-X64/bin/clang-19+0x68b7158)
#<!-- -->22 0x00005be565224df4 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (/home/uniq/LLVM-19.1.4-Linux-X64/bin/clang-19+0x747bdf4)
#<!-- -->23 0x00005be5651e264b clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/home/uniq/LLVM-19.1.4-Linux-X64/bin/clang-19+0x743964b)
#<!-- -->24 0x00005be56520266b clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/home/uniq/LLVM-19.1.4-Linux-X64/bin/clang-19+0x745966b)
#<!-- -->25 0x00005be5620199ee clang_main(int, char**, llvm::ToolContext const&amp;) (/home/uniq/LLVM-19.1.4-Linux-X64/bin/clang-19+0x42709ee)
#<!-- -->26 0x00005be56202c57a main (/home/uniq/LLVM-19.1.4-Linux-X64/bin/clang-19+0x428357a)
#<!-- -->27 0x00007c31fe834e08 (/usr/lib/libc.so.6+0x25e08)
#<!-- -->28 0x00007c31fe834ecc __libc_start_main (/usr/lib/libc.so.6+0x25ecc)
#<!-- -->29 0x00005be56200c265 _start (/home/uniq/LLVM-19.1.4-Linux-X64/bin/clang-19+0x4263265)
```

Preprocessed source: [MobileCydia-5b9246.mm](https://github.com/user-attachments/files/17965810/MobileCydia-5b9246.mm.txt)
Run script: [MobileCydia-5b9246.sh](https://github.com/user-attachments/files/17965811/MobileCydia-5b9246.sh.txt)

</details>


---

### Comment 2 - EugeneZelenko

Could you please try 19 or `main` branch (LLVM, not Apple)? https://godbolt.org should be helpful.


---

### Comment 3 - Un1q32

The preprocessed source, backtrace, and run script are both made with 19.1.4, the linux tarball on the release page, I only mentioned Apple's clang to give an idea on how long the bug has existed.

---

### Comment 4 - Un1q32

https://godbolt.org/z/Grh4oc44e
You can see here it compiles correctly with clang 15, but fails with 16 and up.

---

### Comment 5 - davemgreen

I don't know a lot about objective C, but there is a reduced version here: https://godbolt.org/z/Pzx8jzPqx. I would guess this is caused by the notreturn on the call.

---

### Comment 6 - Un1q32

I did a git bisect and found that d656ae28095726830f9beb8dbd4d69f5144ef821 is bad.  Also the crash doesn't happen when passing `-fno-stack-protector`

---

### Comment 7 - Un1q32

This was fixed by #147411

---

