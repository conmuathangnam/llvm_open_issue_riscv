# [clang] fatal error: error in backend: IO sandbox violation

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/172922
**Status:** Closed
**Labels:** clang:frontend, crash-on-invalid
**Closed Date:** 2025-12-20T01:43:08Z

## Body

Reproducer:
https://godbolt.org/z/83Ynr4Pvo
```cpp
#pragma clang module build bounds_safety
#pragma clang module endbuild
```

Backtrace:
```console
fatal error: error in backend: IO sandbox violation
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<source>:1:2: current parser token 'pragma'
 #0 0x000000000429e098 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x429e098)
 #1 0x000000000429af44 llvm::sys::RunSignalHandlers() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x429af44)
 #2 0x000000000429b4c4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x429b4c4)
 #3 0x00000000041e0766 llvm::CrashRecoveryContext::HandleExit(int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41e0766)
 #4 0x0000000004292d3e llvm::sys::Process::Exit(int, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4292d3e)
 #5 0x0000000000dd7640 LLVMErrorHandler(void*, char const*, bool) cc1_main.cpp:0:0
 #6 0x00000000041eaa43 llvm::report_fatal_error(llvm::Twine const&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41eaa43)
 #7 0x00000000041eabd0 llvm::reportFatalInternalError(llvm::StringRef) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41eabd0)
 #8 0x000000000428c18d llvm::sys::fs::openFile(llvm::Twine const&, int&, llvm::sys::fs::CreationDisposition, llvm::sys::fs::FileAccess, llvm::sys::fs::OpenFlags, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x428c18d)
 #9 0x000000000428fd6f createUniqueEntity(llvm::Twine const&, int&, llvm::SmallVectorImpl<char>&, bool, FSEntity, llvm::sys::fs::OpenFlags, unsigned int) Path.cpp:0:0
#10 0x0000000004290002 llvm::sys::fs::createTemporaryFile(llvm::Twine const&, llvm::StringRef, int&, llvm::SmallVectorImpl<char>&, FSEntity, llvm::sys::fs::OpenFlags) Path.cpp:0:0
#11 0x00000000042901c0 llvm::sys::fs::createTemporaryFile(llvm::Twine const&, llvm::StringRef, llvm::SmallVectorImpl<char>&, llvm::sys::fs::OpenFlags) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42901c0)
#12 0x0000000004e644e6 clang::CompilerInstance::createModuleFromSource(clang::SourceLocation, llvm::StringRef, llvm::StringRef) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e644e6)
#13 0x00000000083b9d51 clang::Preprocessor::HandlePragmaModuleBuild(clang::Token&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x83b9d51)
#14 0x00000000083b5f9a clang::PragmaNamespace::HandlePragma(clang::Preprocessor&, clang::PragmaIntroducer, clang::Token&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x83b5f9a)
#15 0x00000000083b5f9a clang::PragmaNamespace::HandlePragma(clang::Preprocessor&, clang::PragmaIntroducer, clang::Token&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x83b5f9a)
#16 0x00000000083b5f9a clang::PragmaNamespace::HandlePragma(clang::Preprocessor&, clang::PragmaIntroducer, clang::Token&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x83b5f9a)
#17 0x00000000083b0e2c clang::Preprocessor::HandlePragmaDirective(clang::PragmaIntroducer) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x83b0e2c)
#18 0x0000000008389816 clang::Preprocessor::HandleDirective(clang::Token&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x8389816)
#19 0x000000000832514a clang::Lexer::LexTokenInternal(clang::Token&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x832514a)
#20 0x000000000832740e clang::Lexer::Lex(clang::Token&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x832740e)
#21 0x00000000083cde5c clang::Preprocessor::Lex(clang::Token&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x83cde5c)
#22 0x0000000006994692 clang::Parser::ConsumeToken() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6994692)
#23 0x0000000006981708 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6981708)
#24 0x0000000004bf5da8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bf5da8)
#25 0x0000000004eea105 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4eea105)
#26 0x0000000004e6a3be clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e6a3be)
#27 0x0000000004fe41ed clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4fe41ed)
#28 0x0000000000dd9ef2 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdd9ef2)
#29 0x0000000000dd091a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#30 0x0000000000dd0a9d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#31 0x0000000004c67a89 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#32 0x00000000041e06a4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41e06a4)
#33 0x0000000004c680b6 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#34 0x0000000004c29732 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c29732)
#35 0x0000000004c2a6de clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c2a6de)
#36 0x0000000004c319b5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c319b5)
#37 0x0000000000dd62c1 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdd62c1)
#38 0x0000000000c813d4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc813d4)
#39 0x0000743ee6e29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#40 0x0000743ee6e29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#41 0x0000000000dd03b5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdd03b5)
clang++: error: clang frontend command failed with exit code 70 (use -v to see invocation)
Compiler returned: 70
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/83Ynr4Pvo
```cpp
#pragma clang module build bounds_safety
#pragma clang module endbuild
```

Backtrace:
```console
fatal error: error in backend: IO sandbox violation
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics &lt;source&gt;
1.	&lt;source&gt;:1:2: current parser token 'pragma'
 #<!-- -->0 0x000000000429e098 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x429e098)
 #<!-- -->1 0x000000000429af44 llvm::sys::RunSignalHandlers() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x429af44)
 #<!-- -->2 0x000000000429b4c4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x429b4c4)
 #<!-- -->3 0x00000000041e0766 llvm::CrashRecoveryContext::HandleExit(int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41e0766)
 #<!-- -->4 0x0000000004292d3e llvm::sys::Process::Exit(int, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4292d3e)
 #<!-- -->5 0x0000000000dd7640 LLVMErrorHandler(void*, char const*, bool) cc1_main.cpp:0:0
 #<!-- -->6 0x00000000041eaa43 llvm::report_fatal_error(llvm::Twine const&amp;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41eaa43)
 #<!-- -->7 0x00000000041eabd0 llvm::reportFatalInternalError(llvm::StringRef) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41eabd0)
 #<!-- -->8 0x000000000428c18d llvm::sys::fs::openFile(llvm::Twine const&amp;, int&amp;, llvm::sys::fs::CreationDisposition, llvm::sys::fs::FileAccess, llvm::sys::fs::OpenFlags, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x428c18d)
 #<!-- -->9 0x000000000428fd6f createUniqueEntity(llvm::Twine const&amp;, int&amp;, llvm::SmallVectorImpl&lt;char&gt;&amp;, bool, FSEntity, llvm::sys::fs::OpenFlags, unsigned int) Path.cpp:0:0
#<!-- -->10 0x0000000004290002 llvm::sys::fs::createTemporaryFile(llvm::Twine const&amp;, llvm::StringRef, int&amp;, llvm::SmallVectorImpl&lt;char&gt;&amp;, FSEntity, llvm::sys::fs::OpenFlags) Path.cpp:0:0
#<!-- -->11 0x00000000042901c0 llvm::sys::fs::createTemporaryFile(llvm::Twine const&amp;, llvm::StringRef, llvm::SmallVectorImpl&lt;char&gt;&amp;, llvm::sys::fs::OpenFlags) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42901c0)
#<!-- -->12 0x0000000004e644e6 clang::CompilerInstance::createModuleFromSource(clang::SourceLocation, llvm::StringRef, llvm::StringRef) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e644e6)
#<!-- -->13 0x00000000083b9d51 clang::Preprocessor::HandlePragmaModuleBuild(clang::Token&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x83b9d51)
#<!-- -->14 0x00000000083b5f9a clang::PragmaNamespace::HandlePragma(clang::Preprocessor&amp;, clang::PragmaIntroducer, clang::Token&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x83b5f9a)
#<!-- -->15 0x00000000083b5f9a clang::PragmaNamespace::HandlePragma(clang::Preprocessor&amp;, clang::PragmaIntroducer, clang::Token&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x83b5f9a)
#<!-- -->16 0x00000000083b5f9a clang::PragmaNamespace::HandlePragma(clang::Preprocessor&amp;, clang::PragmaIntroducer, clang::Token&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x83b5f9a)
#<!-- -->17 0x00000000083b0e2c clang::Preprocessor::HandlePragmaDirective(clang::PragmaIntroducer) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x83b0e2c)
#<!-- -->18 0x0000000008389816 clang::Preprocessor::HandleDirective(clang::Token&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x8389816)
#<!-- -->19 0x000000000832514a clang::Lexer::LexTokenInternal(clang::Token&amp;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x832514a)
#<!-- -->20 0x000000000832740e clang::Lexer::Lex(clang::Token&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x832740e)
#<!-- -->21 0x00000000083cde5c clang::Preprocessor::Lex(clang::Token&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x83cde5c)
#<!-- -->22 0x0000000006994692 clang::Parser::ConsumeToken() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6994692)
#<!-- -->23 0x0000000006981708 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6981708)
#<!-- -->24 0x0000000004bf5da8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bf5da8)
#<!-- -->25 0x0000000004eea105 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4eea105)
#<!-- -->26 0x0000000004e6a3be clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e6a3be)
#<!-- -->27 0x0000000004fe41ed clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4fe41ed)
#<!-- -->28 0x0000000000dd9ef2 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdd9ef2)
#<!-- -->29 0x0000000000dd091a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->30 0x0000000000dd0a9d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->31 0x0000000004c67a89 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->32 0x00000000041e06a4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41e06a4)
#<!-- -->33 0x0000000004c680b6 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->34 0x0000000004c29732 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c29732)
#<!-- -->35 0x0000000004c2a6de clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c2a6de)
#<!-- -->36 0x0000000004c319b5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c319b5)
#<!-- -->37 0x0000000000dd62c1 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdd62c1)
#<!-- -->38 0x0000000000c813d4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc813d4)
#<!-- -->39 0x0000743ee6e29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->40 0x0000743ee6e29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->41 0x0000000000dd03b5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdd03b5)
clang++: error: clang frontend command failed with exit code 70 (use -v to see invocation)
Compiler returned: 70
```
</details>


---

### Comment 2 - k-arrows

This is trunk only:
https://godbolt.org/z/zoTKoz144

---

### Comment 3 - shafik

CC @jansvoboda11 This error comes from 8e999e3d7857c

---

### Comment 4 - jansvoboda11

Thanks for reporting this. I'll try to have this fixed by the end of the day.

---

### Comment 5 - jansvoboda11

I disabled the sandbox for now in https://github.com/llvm/llvm-project/pull/173074. I'll make sure this test case works before re-enabling.

---

