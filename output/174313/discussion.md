# Crash when compiling from a Zed task while saving

**Author:** jmsaunde
**URL:** https://github.com/llvm/llvm-project/issues/174313
**Status:** Open
**Labels:** clang:codegen, crash, needs-reduction

## Body

I was doing a compilation using a keyboard shortcut I had configured as follows in Zed: 
```        "action::Sequence", ["workspace::SaveAll", "workspace::Save", "debugger::Rerun"],```

Effectively, save everything then run the debugger. The debugger run has an initial stage that will run the compilation which is how clang got invoked.

I was messing around with trying to get it to save things before running the builds and it wasn't working right for me. I suspect the sequence actions used within Zed involve some degree of asynchronicity. With only the `workspace::SaveAll` part (meaning without the `workspace::Save` part), the build would start before the file write had happened and not rebuild.

Since I think this is unrelated to the code being compiled I haven't included the requested source and build script. I made sure it didn't repro running the script and I've kept it. I'm willing to share privately with a compiler dev but not more publicly.

``` #0 0x00007f9d86ccbba9 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/lib64/libLLVM.so.21.1+0x50cbba9)
 #1 0x00007f9d86ccb919 llvm::sys::CleanupOnSignal(unsigned long) (/lib64/libLLVM.so.21.1+0x50cb919)
 #2 0x00007f9d86c3af95 (/lib64/libLLVM.so.21.1+0x503af95)
 #3 0x00007f9d86c3b1e6 (/lib64/libLLVM.so.21.1+0x503b1e6)
 #4 0x00007f9d81627290 __restore_rt (/lib64/libc.so.6+0x1a290)
 #5 0x00007f9d85eacba1 (/lib64/libLLVM.so.21.1+0x42acba1)
 #6 0x00007f9d8c125957 clang::FileManager::getBufferForFile(clang::FileEntryRef, bool, bool, std::optional<long>, bool) (/lib64/libclang-cpp.so.21.1+0x1b25957)
 #7 0x00007f9d8c125370 (/lib64/libclang-cpp.so.21.1+0x1b25370)
 #8 0x00007f9d8d45ff4b clang::CodeGen::CGDebugInfo::computeChecksum(clang::FileID, llvm::SmallString<64u>&) const (/lib64/libclang-cpp.so.21.1+0x2e5ff4b)
 #9 0x00007f9d8d45c66a clang::CodeGen::CGDebugInfo::CreateCompileUnit() (/lib64/libclang-cpp.so.21.1+0x2e5c66a)
#10 0x00007f9d8d45c2b4 clang::CodeGen::CGDebugInfo::CGDebugInfo(clang::CodeGen::CodeGenModule&) (/lib64/libclang-cpp.so.21.1+0x2e5c2b4)
#11 0x00007f9d8c5da65e clang::CodeGen::CodeGenModule::CodeGenModule(clang::ASTContext&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, clang::HeaderSearchOptions const&, clang::PreprocessorOptions const&, clang::CodeGenOptions const&, llvm::Module&, clang::DiagnosticsEngine&, clang::CoverageSourceInfo*) (/lib64/libclang-cpp.so.21.1+0x1fda65e)
#12 0x00007f9d8c5d966a (/lib64/libclang-cpp.so.21.1+0x1fd966a)
#13 0x00007f9d8d606955 clang::BackendConsumer::Initialize(clang::ASTContext&) (/lib64/libclang-cpp.so.21.1+0x3006955)
#14 0x00007f9d8c3caff5 clang::FrontendAction::BeginSourceFile(clang::CompilerInstance&, clang::FrontendInputFile const&) (/lib64/libclang-cpp.so.21.1+0x1dcaff5)
#15 0x00007f9d8c3c82c1 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/lib64/libclang-cpp.so.21.1+0x1dc82c1)
#16 0x00007f9d8c3c7c75 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/lib64/libclang-cpp.so.21.1+0x1dc7c75)
#17 0x000055ab709328d6 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/usr/bin/clang+++0x198d6)
#18 0x000055ab70929a4e (/usr/bin/clang+++0x10a4e)
#19 0x00007f9d8d8f721d (/lib64/libclang-cpp.so.21.1+0x32f721d)
#20 0x00007f9d86c3af30 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/lib64/libLLVM.so.21.1+0x503af30)
#21 0x00007f9d8c5dc875 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (/lib64/libclang-cpp.so.21.1+0x1fdc875)
#22 0x00007f9d8c5457be clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/lib64/libclang-cpp.so.21.1+0x1f457be)
#23 0x00007f9d8c5455b2 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/lib64/libclang-cpp.so.21.1+0x1f455b2)
#24 0x00007f9d8c544f26 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/lib64/libclang-cpp.so.21.1+0x1f44f26)
#25 0x000055ab7092bf04 clang_main(int, char**, llvm::ToolContext const&) (/usr/bin/clang+++0x12f04)
#26 0x000055ab7092a782 main (/usr/bin/clang+++0x11782)
#27 0x00007f9d816105b5 __libc_start_call_main (/lib64/libc.so.6+0x35b5)
#28 0x00007f9d81610668 __libc_start_main@GLIBC_2.2.5 (/lib64/libc.so.6+0x3668)
#29 0x000055ab70934f85 _start (/usr/bin/clang+++0x1bf85)
clang++: error: clang frontend command failed with exit code 135 (use -v to see invocation)
clang version 21.1.7 (Fedora 21.1.7-1.fc43)
Target: x86_64-redhat-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
Configuration file: /etc/clang/x86_64-redhat-linux-gnu-clang++.cfg
```

## Comments

### Comment 1 - frederick-vs-ja

Could you reduce the reproducer?

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: Jordan Saunders (jmsaunde)

<details>
I was doing a compilation using a keyboard shortcut I had configured as follows in Zed: 
```        "action::Sequence", ["workspace::SaveAll", "workspace::Save", "debugger::Rerun"],```

Effectively, save everything then run the debugger. The debugger run has an initial stage that will run the compilation which is how clang got invoked.

I was messing around with trying to get it to save things before running the builds and it wasn't working right for me. I suspect the sequence actions used within Zed involve some degree of asynchronicity. With only the `workspace::SaveAll` part (meaning without the `workspace::Save` part), the build would start before the file write had happened and not rebuild.

Since I think this is unrelated to the code being compiled I haven't included the requested source and build script. I made sure it didn't repro running the script and I've kept it. I'm willing to share privately with a compiler dev but not more publicly.

``` #<!-- -->0 0x00007f9d86ccbba9 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/lib64/libLLVM.so.21.1+0x50cbba9)
 #<!-- -->1 0x00007f9d86ccb919 llvm::sys::CleanupOnSignal(unsigned long) (/lib64/libLLVM.so.21.1+0x50cb919)
 #<!-- -->2 0x00007f9d86c3af95 (/lib64/libLLVM.so.21.1+0x503af95)
 #<!-- -->3 0x00007f9d86c3b1e6 (/lib64/libLLVM.so.21.1+0x503b1e6)
 #<!-- -->4 0x00007f9d81627290 __restore_rt (/lib64/libc.so.6+0x1a290)
 #<!-- -->5 0x00007f9d85eacba1 (/lib64/libLLVM.so.21.1+0x42acba1)
 #<!-- -->6 0x00007f9d8c125957 clang::FileManager::getBufferForFile(clang::FileEntryRef, bool, bool, std::optional&lt;long&gt;, bool) (/lib64/libclang-cpp.so.21.1+0x1b25957)
 #<!-- -->7 0x00007f9d8c125370 (/lib64/libclang-cpp.so.21.1+0x1b25370)
 #<!-- -->8 0x00007f9d8d45ff4b clang::CodeGen::CGDebugInfo::computeChecksum(clang::FileID, llvm::SmallString&lt;64u&gt;&amp;) const (/lib64/libclang-cpp.so.21.1+0x2e5ff4b)
 #<!-- -->9 0x00007f9d8d45c66a clang::CodeGen::CGDebugInfo::CreateCompileUnit() (/lib64/libclang-cpp.so.21.1+0x2e5c66a)
#<!-- -->10 0x00007f9d8d45c2b4 clang::CodeGen::CGDebugInfo::CGDebugInfo(clang::CodeGen::CodeGenModule&amp;) (/lib64/libclang-cpp.so.21.1+0x2e5c2b4)
#<!-- -->11 0x00007f9d8c5da65e clang::CodeGen::CodeGenModule::CodeGenModule(clang::ASTContext&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;, clang::HeaderSearchOptions const&amp;, clang::PreprocessorOptions const&amp;, clang::CodeGenOptions const&amp;, llvm::Module&amp;, clang::DiagnosticsEngine&amp;, clang::CoverageSourceInfo*) (/lib64/libclang-cpp.so.21.1+0x1fda65e)
#<!-- -->12 0x00007f9d8c5d966a (/lib64/libclang-cpp.so.21.1+0x1fd966a)
#<!-- -->13 0x00007f9d8d606955 clang::BackendConsumer::Initialize(clang::ASTContext&amp;) (/lib64/libclang-cpp.so.21.1+0x3006955)
#<!-- -->14 0x00007f9d8c3caff5 clang::FrontendAction::BeginSourceFile(clang::CompilerInstance&amp;, clang::FrontendInputFile const&amp;) (/lib64/libclang-cpp.so.21.1+0x1dcaff5)
#<!-- -->15 0x00007f9d8c3c82c1 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/lib64/libclang-cpp.so.21.1+0x1dc82c1)
#<!-- -->16 0x00007f9d8c3c7c75 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/lib64/libclang-cpp.so.21.1+0x1dc7c75)
#<!-- -->17 0x000055ab709328d6 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/usr/bin/clang+++0x198d6)
#<!-- -->18 0x000055ab70929a4e (/usr/bin/clang+++0x10a4e)
#<!-- -->19 0x00007f9d8d8f721d (/lib64/libclang-cpp.so.21.1+0x32f721d)
#<!-- -->20 0x00007f9d86c3af30 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/lib64/libLLVM.so.21.1+0x503af30)
#<!-- -->21 0x00007f9d8c5dc875 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (/lib64/libclang-cpp.so.21.1+0x1fdc875)
#<!-- -->22 0x00007f9d8c5457be clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/lib64/libclang-cpp.so.21.1+0x1f457be)
#<!-- -->23 0x00007f9d8c5455b2 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/lib64/libclang-cpp.so.21.1+0x1f455b2)
#<!-- -->24 0x00007f9d8c544f26 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/lib64/libclang-cpp.so.21.1+0x1f44f26)
#<!-- -->25 0x000055ab7092bf04 clang_main(int, char**, llvm::ToolContext const&amp;) (/usr/bin/clang+++0x12f04)
#<!-- -->26 0x000055ab7092a782 main (/usr/bin/clang+++0x11782)
#<!-- -->27 0x00007f9d816105b5 __libc_start_call_main (/lib64/libc.so.6+0x35b5)
#<!-- -->28 0x00007f9d81610668 __libc_start_main@<!-- -->GLIBC_2.2.5 (/lib64/libc.so.6+0x3668)
#<!-- -->29 0x000055ab70934f85 _start (/usr/bin/clang+++0x1bf85)
clang++: error: clang frontend command failed with exit code 135 (use -v to see invocation)
clang version 21.1.7 (Fedora 21.1.7-1.fc43)
Target: x86_64-redhat-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
Configuration file: /etc/clang/x86_64-redhat-linux-gnu-clang++.cfg
```
</details>


---

### Comment 3 - jmsaunde

> Could you reduce the reproducer?

The reproducer generated by clang does not reproduce the issue locally for me. I don't think the issue has anything to do with the code being compiled.

I think the problem is that the file is being modified (maybe deleted? but before being recreated) while it's trying to compute the checksum for the debug info. My read of this is that it is a race condition.

---

