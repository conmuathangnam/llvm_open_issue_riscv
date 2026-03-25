# Error Compiling UnrealEngine with AngelScript

**Author:** affamu
**URL:** https://github.com/llvm/llvm-project/issues/157219

## Body

```
[113/193] Compile Module.Slate.3.cpp
free(): invalid size
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /home/affamu/UnrealAngel/Engine/Extras/ThirdPartyNotUE/SDKs/HostLinux/Linux_x64/v25_clang-18.1.0-rockylinux8/x86_64-unknown-linux-gnu/bin/clang++ @../Intermediate/Build/Linux/x64/ShaderCompileWorker/Development/Slate/Module.Slate.3.cpp.o.rsp
1.      <eof> parser at end of file
 #0 0x0000000001f6d5f8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/home/affamu/UnrealAngel/Engine/Extras/ThirdPartyNotUE/SDKs/HostLinux/Linux_x64/v25_clang-18.1.0-rockylinux8/x86_64-unknown-linux-gnu/bin/clang+++0x1f6d5f8)
 #1 0x0000000001f6b574 llvm::sys::CleanupOnSignal(unsigned long) (/home/affamu/UnrealAngel/Engine/Extras/ThirdPartyNotUE/SDKs/HostLinux/Linux_x64/v25_clang-18.1.0-rockylinux8/x86_64-unknown-linux-gnu/bin/clang+++0x1f6b574)
 #2 0x0000000001ec7fc8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007fd2be43e540 (/usr/lib/libc.so.6+0x3e540)
 #4 0x00007fd2be49894c (/usr/lib/libc.so.6+0x9894c)
 #5 0x00007fd2be43e410 raise (/usr/lib/libc.so.6+0x3e410)
 #6 0x00007fd2be42557a abort (/usr/lib/libc.so.6+0x2557a)
 #7 0x00007fd2be426613 (/usr/lib/libc.so.6+0x26613)
 #8 0x00007fd2be4a2d65 (/usr/lib/libc.so.6+0xa2d65)
 #9 0x00007fd2be4a2d84 (/usr/lib/libc.so.6+0xa2d84)
#10 0x0000000001ca4ad3 llvm::MCContext::~MCContext() (/home/affamu/UnrealAngel/Engine/Extras/ThirdPartyNotUE/SDKs/HostLinux/Linux_x64/v25_clang-18.1.0-rockylinux8/x86_64-unknown-linux-gnu/bin/clang+++0x1ca4ad3)
#11 0x00000000015ad69c llvm::MachineModuleInfoWrapperPass::~MachineModuleInfoWrapperPass() (/home/affamu/UnrealAngel/Engine/Extras/ThirdPartyNotUE/SDKs/HostLinux/Linux_x64/v25_clang-18.1.0-rockylinux8/x86_64-unknown-linux-gnu/bin/clang+++0x15ad69c)
#12 0x0000000001aa33ae llvm::PMTopLevelManager::~PMTopLevelManager() (/home/affamu/UnrealAngel/Engine/Extras/ThirdPartyNotUE/SDKs/HostLinux/Linux_x64/v25_clang-18.1.0-rockylinux8/x86_64-unknown-linux-gnu/bin/clang+++0x1aa33ae)
#13 0x0000000001aa3f49 llvm::legacy::PassManagerImpl::~PassManagerImpl() (/home/affamu/UnrealAngel/Engine/Extras/ThirdPartyNotUE/SDKs/HostLinux/Linux_x64/v25_clang-18.1.0-rockylinux8/x86_64-unknown-linux-gnu/bin/clang+++0x1aa3f49)
#14 0x0000000001aa1d31 llvm::legacy::PassManager::~PassManager() (/home/affamu/UnrealAngel/Engine/Extras/ThirdPartyNotUE/SDKs/HostLinux/Linux_x64/v25_clang-18.1.0-rockylinux8/x86_64-unknown-linux-gnu/bin/clang+++0x1aa1d31)
#15 0x000000000219ed36 clang::EmitBackendOutput(clang::DiagnosticsEngine&, clang::HeaderSearchOptions const&, clang::CodeGenOptions const&, clang::TargetOptions const&, clang::LangOptions const&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/home/affamu/UnrealAngel/Engine/Extras/ThirdPartyNotUE/SDKs/HostLinux/Linux_x64/v25_clang-18.1.0-rockylinux8/x86_64-unknown-linux-gnu/bin/clang+++0x219ed36)
#16 0x00000000026a76ba clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/home/affamu/UnrealAngel/Engine/Extras/ThirdPartyNotUE/SDKs/HostLinux/Linux_x64/v25_clang-18.1.0-rockylinux8/x86_64-unknown-linux-gnu/bin/clang+++0x26a76ba)
#17 0x0000000003f1c201 clang::ParseAST(clang::Sema&, bool, bool) (/home/affamu/UnrealAngel/Engine/Extras/ThirdPartyNotUE/SDKs/HostLinux/Linux_x64/v25_clang-18.1.0-rockylinux8/x86_64-unknown-linux-gnu/bin/clang+++0x3f1c201)
#18 0x00000000026a6d55 clang::CodeGenAction::ExecuteAction() (/home/affamu/UnrealAngel/Engine/Extras/ThirdPartyNotUE/SDKs/HostLinux/Linux_x64/v25_clang-18.1.0-rockylinux8/x86_64-unknown-linux-gnu/bin/clang+++0x26a6d55)
#19 0x00000000028ee0b1 clang::FrontendAction::Execute() (/home/affamu/UnrealAngel/Engine/Extras/ThirdPartyNotUE/SDKs/HostLinux/Linux_x64/v25_clang-18.1.0-rockylinux8/x86_64-unknown-linux-gnu/bin/clang+++0x28ee0b1)
#20 0x000000000287a13a clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/home/affamu/UnrealAngel/Engine/Extras/ThirdPartyNotUE/SDKs/HostLinux/Linux_x64/v25_clang-18.1.0-rockylinux8/x86_64-unknown-linux-gnu/bin/clang+++0x287a13a)
#21 0x00000000029bd0b3 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/home/affamu/UnrealAngel/Engine/Extras/ThirdPartyNotUE/SDKs/HostLinux/Linux_x64/v25_clang-18.1.0-rockylinux8/x86_64-unknown-linux-gnu/bin/clang+++0x29bd0b3)
#22 0x0000000000b0433c cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/home/affamu/UnrealAngel/Engine/Extras/ThirdPartyNotUE/SDKs/HostLinux/Linux_x64/v25_clang-18.1.0-rockylinux8/x86_64-unknown-linux-gnu/bin/clang+++0xb0433c)
#23 0x0000000000afcf28 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#24 0x00000000026e82f9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#25 0x0000000001ec83f3 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/home/affamu/UnrealAngel/Engine/Extras/ThirdPartyNotUE/SDKs/HostLinux/Linux_x64/v25_clang-18.1.0-rockylinux8/x86_64-unknown-linux-gnu/bin/clang+++0x1ec83f3)
#26 0x00000000026e8a7e clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.276) Job.cpp:0:0
#27 0x00000000026b5de4 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/home/affamu/UnrealAngel/Engine/Extras/ThirdPartyNotUE/SDKs/HostLinux/Linux_x64/v25_clang-18.1.0-rockylinux8/x86_64-unknown-linux-gnu/bin/clang+++0x26b5de4)
#28 0x00000000026b67a3 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/home/affamu/UnrealAngel/Engine/Extras/ThirdPartyNotUE/SDKs/HostLinux/Linux_x64/v25_clang-18.1.0-rockylinux8/x86_64-unknown-linux-gnu/bin/clang+++0x26b67a3)
#29 0x00000000026be1cc clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/home/affamu/UnrealAngel/Engine/Extras/ThirdPartyNotUE/SDKs/HostLinux/Linux_x64/v25_clang-18.1.0-rockylinux8/x86_64-unknown-linux-gnu/bin/clang+++0x26be1cc)
#30 0x0000000000b01c7e clang_main(int, char**, llvm::ToolContext const&) (/home/affamu/UnrealAngel/Engine/Extras/ThirdPartyNotUE/SDKs/HostLinux/Linux_x64/v25_clang-18.1.0-rockylinux8/x86_64-unknown-linux-gnu/bin/clang+++0xb01c7e)
#31 0x0000000000a67c13 main (/home/affamu/UnrealAngel/Engine/Extras/ThirdPartyNotUE/SDKs/HostLinux/Linux_x64/v25_clang-18.1.0-rockylinux8/x86_64-unknown-linux-gnu/bin/clang+++0xa67c13)
#32 0x00007fd2be427675 (/usr/lib/libc.so.6+0x27675)
#33 0x00007fd2be427729 __libc_start_main (/usr/lib/libc.so.6+0x27729)
#34 0x0000000000afc7de _start (/home/affamu/UnrealAngel/Engine/Extras/ThirdPartyNotUE/SDKs/HostLinux/Linux_x64/v25_clang-18.1.0-rockylinux8/x86_64-unknown-linux-gnu/bin/clang+++0xafc7de)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
clang version 18.1.0rc (github.com/llvm/llvm-project 461274b81d8641eab64d494accddc81d7db8a09e)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /home/affamu/UnrealAngel/Engine/Extras/ThirdPartyNotUE/SDKs/HostLinux/Linux_x64/v25_clang-18.1.0-rockylinux8/x86_64-unknown-linux-gnu/bin
clang++: note: diagnostic msg:
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: /home/affamu/.epic/UnrealBuildAccelerator/sessions/250905_232312/temp/Module-b6dba6.cpp
clang++: note: diagnostic msg: /home/affamu/.epic/UnrealBuildAccelerator/sessions/250905_232312/temp/Module-b6dba6.sh
clang++: note: diagnostic msg:

********************
[114/193] Compile Module.UELibSampleRate.cpp
```

[Module-b6dba6.cpp](https://github.com/user-attachments/files/22184384/Module-b6dba6.cpp)
[Module-b6dba6.sh](https://github.com/user-attachments/files/22184385/Module-b6dba6.sh)

## Comments

### Comment 1 - EugeneZelenko

Could you please try 21 or `main` branch? Only most recent release is maintained.

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-llvm-mc

Author: None (affamu)

<details>
```
[113/193] Compile Module.Slate.3.cpp
free(): invalid size
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /home/affamu/UnrealAngel/Engine/Extras/ThirdPartyNotUE/SDKs/HostLinux/Linux_x64/v25_clang-18.1.0-rockylinux8/x86_64-unknown-linux-gnu/bin/clang++ @../Intermediate/Build/Linux/x64/ShaderCompileWorker/Development/Slate/Module.Slate.3.cpp.o.rsp
1.      &lt;eof&gt; parser at end of file
 #<!-- -->0 0x0000000001f6d5f8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/home/affamu/UnrealAngel/Engine/Extras/ThirdPartyNotUE/SDKs/HostLinux/Linux_x64/v25_clang-18.1.0-rockylinux8/x86_64-unknown-linux-gnu/bin/clang+++0x1f6d5f8)
 #<!-- -->1 0x0000000001f6b574 llvm::sys::CleanupOnSignal(unsigned long) (/home/affamu/UnrealAngel/Engine/Extras/ThirdPartyNotUE/SDKs/HostLinux/Linux_x64/v25_clang-18.1.0-rockylinux8/x86_64-unknown-linux-gnu/bin/clang+++0x1f6b574)
 #<!-- -->2 0x0000000001ec7fc8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007fd2be43e540 (/usr/lib/libc.so.6+0x3e540)
 #<!-- -->4 0x00007fd2be49894c (/usr/lib/libc.so.6+0x9894c)
 #<!-- -->5 0x00007fd2be43e410 raise (/usr/lib/libc.so.6+0x3e410)
 #<!-- -->6 0x00007fd2be42557a abort (/usr/lib/libc.so.6+0x2557a)
 #<!-- -->7 0x00007fd2be426613 (/usr/lib/libc.so.6+0x26613)
 #<!-- -->8 0x00007fd2be4a2d65 (/usr/lib/libc.so.6+0xa2d65)
 #<!-- -->9 0x00007fd2be4a2d84 (/usr/lib/libc.so.6+0xa2d84)
#<!-- -->10 0x0000000001ca4ad3 llvm::MCContext::~MCContext() (/home/affamu/UnrealAngel/Engine/Extras/ThirdPartyNotUE/SDKs/HostLinux/Linux_x64/v25_clang-18.1.0-rockylinux8/x86_64-unknown-linux-gnu/bin/clang+++0x1ca4ad3)
#<!-- -->11 0x00000000015ad69c llvm::MachineModuleInfoWrapperPass::~MachineModuleInfoWrapperPass() (/home/affamu/UnrealAngel/Engine/Extras/ThirdPartyNotUE/SDKs/HostLinux/Linux_x64/v25_clang-18.1.0-rockylinux8/x86_64-unknown-linux-gnu/bin/clang+++0x15ad69c)
#<!-- -->12 0x0000000001aa33ae llvm::PMTopLevelManager::~PMTopLevelManager() (/home/affamu/UnrealAngel/Engine/Extras/ThirdPartyNotUE/SDKs/HostLinux/Linux_x64/v25_clang-18.1.0-rockylinux8/x86_64-unknown-linux-gnu/bin/clang+++0x1aa33ae)
#<!-- -->13 0x0000000001aa3f49 llvm::legacy::PassManagerImpl::~PassManagerImpl() (/home/affamu/UnrealAngel/Engine/Extras/ThirdPartyNotUE/SDKs/HostLinux/Linux_x64/v25_clang-18.1.0-rockylinux8/x86_64-unknown-linux-gnu/bin/clang+++0x1aa3f49)
#<!-- -->14 0x0000000001aa1d31 llvm::legacy::PassManager::~PassManager() (/home/affamu/UnrealAngel/Engine/Extras/ThirdPartyNotUE/SDKs/HostLinux/Linux_x64/v25_clang-18.1.0-rockylinux8/x86_64-unknown-linux-gnu/bin/clang+++0x1aa1d31)
#<!-- -->15 0x000000000219ed36 clang::EmitBackendOutput(clang::DiagnosticsEngine&amp;, clang::HeaderSearchOptions const&amp;, clang::CodeGenOptions const&amp;, clang::TargetOptions const&amp;, clang::LangOptions const&amp;, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;, std::unique_ptr&lt;llvm::raw_pwrite_stream, std::default_delete&lt;llvm::raw_pwrite_stream&gt;&gt;, clang::BackendConsumer*) (/home/affamu/UnrealAngel/Engine/Extras/ThirdPartyNotUE/SDKs/HostLinux/Linux_x64/v25_clang-18.1.0-rockylinux8/x86_64-unknown-linux-gnu/bin/clang+++0x219ed36)
#<!-- -->16 0x00000000026a76ba clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&amp;) (/home/affamu/UnrealAngel/Engine/Extras/ThirdPartyNotUE/SDKs/HostLinux/Linux_x64/v25_clang-18.1.0-rockylinux8/x86_64-unknown-linux-gnu/bin/clang+++0x26a76ba)
#<!-- -->17 0x0000000003f1c201 clang::ParseAST(clang::Sema&amp;, bool, bool) (/home/affamu/UnrealAngel/Engine/Extras/ThirdPartyNotUE/SDKs/HostLinux/Linux_x64/v25_clang-18.1.0-rockylinux8/x86_64-unknown-linux-gnu/bin/clang+++0x3f1c201)
#<!-- -->18 0x00000000026a6d55 clang::CodeGenAction::ExecuteAction() (/home/affamu/UnrealAngel/Engine/Extras/ThirdPartyNotUE/SDKs/HostLinux/Linux_x64/v25_clang-18.1.0-rockylinux8/x86_64-unknown-linux-gnu/bin/clang+++0x26a6d55)
#<!-- -->19 0x00000000028ee0b1 clang::FrontendAction::Execute() (/home/affamu/UnrealAngel/Engine/Extras/ThirdPartyNotUE/SDKs/HostLinux/Linux_x64/v25_clang-18.1.0-rockylinux8/x86_64-unknown-linux-gnu/bin/clang+++0x28ee0b1)
#<!-- -->20 0x000000000287a13a clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/home/affamu/UnrealAngel/Engine/Extras/ThirdPartyNotUE/SDKs/HostLinux/Linux_x64/v25_clang-18.1.0-rockylinux8/x86_64-unknown-linux-gnu/bin/clang+++0x287a13a)
#<!-- -->21 0x00000000029bd0b3 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/home/affamu/UnrealAngel/Engine/Extras/ThirdPartyNotUE/SDKs/HostLinux/Linux_x64/v25_clang-18.1.0-rockylinux8/x86_64-unknown-linux-gnu/bin/clang+++0x29bd0b3)
#<!-- -->22 0x0000000000b0433c cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/home/affamu/UnrealAngel/Engine/Extras/ThirdPartyNotUE/SDKs/HostLinux/Linux_x64/v25_clang-18.1.0-rockylinux8/x86_64-unknown-linux-gnu/bin/clang+++0xb0433c)
#<!-- -->23 0x0000000000afcf28 ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->24 0x00000000026e82f9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->25 0x0000000001ec83f3 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/home/affamu/UnrealAngel/Engine/Extras/ThirdPartyNotUE/SDKs/HostLinux/Linux_x64/v25_clang-18.1.0-rockylinux8/x86_64-unknown-linux-gnu/bin/clang+++0x1ec83f3)
#<!-- -->26 0x00000000026e8a7e clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.276) Job.cpp:0:0
#<!-- -->27 0x00000000026b5de4 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/home/affamu/UnrealAngel/Engine/Extras/ThirdPartyNotUE/SDKs/HostLinux/Linux_x64/v25_clang-18.1.0-rockylinux8/x86_64-unknown-linux-gnu/bin/clang+++0x26b5de4)
#<!-- -->28 0x00000000026b67a3 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/home/affamu/UnrealAngel/Engine/Extras/ThirdPartyNotUE/SDKs/HostLinux/Linux_x64/v25_clang-18.1.0-rockylinux8/x86_64-unknown-linux-gnu/bin/clang+++0x26b67a3)
#<!-- -->29 0x00000000026be1cc clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/home/affamu/UnrealAngel/Engine/Extras/ThirdPartyNotUE/SDKs/HostLinux/Linux_x64/v25_clang-18.1.0-rockylinux8/x86_64-unknown-linux-gnu/bin/clang+++0x26be1cc)
#<!-- -->30 0x0000000000b01c7e clang_main(int, char**, llvm::ToolContext const&amp;) (/home/affamu/UnrealAngel/Engine/Extras/ThirdPartyNotUE/SDKs/HostLinux/Linux_x64/v25_clang-18.1.0-rockylinux8/x86_64-unknown-linux-gnu/bin/clang+++0xb01c7e)
#<!-- -->31 0x0000000000a67c13 main (/home/affamu/UnrealAngel/Engine/Extras/ThirdPartyNotUE/SDKs/HostLinux/Linux_x64/v25_clang-18.1.0-rockylinux8/x86_64-unknown-linux-gnu/bin/clang+++0xa67c13)
#<!-- -->32 0x00007fd2be427675 (/usr/lib/libc.so.6+0x27675)
#<!-- -->33 0x00007fd2be427729 __libc_start_main (/usr/lib/libc.so.6+0x27729)
#<!-- -->34 0x0000000000afc7de _start (/home/affamu/UnrealAngel/Engine/Extras/ThirdPartyNotUE/SDKs/HostLinux/Linux_x64/v25_clang-18.1.0-rockylinux8/x86_64-unknown-linux-gnu/bin/clang+++0xafc7de)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
clang version 18.1.0rc (github.com/llvm/llvm-project 461274b81d8641eab64d494accddc81d7db8a09e)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /home/affamu/UnrealAngel/Engine/Extras/ThirdPartyNotUE/SDKs/HostLinux/Linux_x64/v25_clang-18.1.0-rockylinux8/x86_64-unknown-linux-gnu/bin
clang++: note: diagnostic msg:
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: /home/affamu/.epic/UnrealBuildAccelerator/sessions/250905_232312/temp/Module-b6dba6.cpp
clang++: note: diagnostic msg: /home/affamu/.epic/UnrealBuildAccelerator/sessions/250905_232312/temp/Module-b6dba6.sh
clang++: note: diagnostic msg:

********************
[114/193] Compile Module.UELibSampleRate.cpp
```

[Module-b6dba6.cpp](https://github.com/user-attachments/files/22184384/Module-b6dba6.cpp)
[Module-b6dba6.sh](https://github.com/user-attachments/files/22184385/Module-b6dba6.sh)
</details>


---

