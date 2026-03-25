# [clang] crashes on pragma push_macro

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/149762
**Status:** Closed
**Labels:** clang:frontend, confirmed, crash
**Closed Date:** 2025-07-22T14:57:35Z

## Body

Reproducer:
https://godbolt.org/z/snbxaecxM
```cpp
#pragma push_macro("")
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/Lex/Preprocessor.cpp:714: clang::IdentifierInfo* clang::Preprocessor::LookUpIdentifierInfo(clang::Token&) const: Assertion `!Identifier.getRawIdentifier().empty() && "No raw identifier data!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<source>:1:2: current parser token 'pragma'
 #0 0x0000000003fc1498 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3fc1498)
 #1 0x0000000003fbe8c4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3fbe8c4)
 #2 0x0000000003f03098 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007b311ea42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007b311ea969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007b311ea42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007b311ea287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007b311ea2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007b311ea39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000007eb069d (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7eb069d)
#10 0x0000000007e98d3f clang::Preprocessor::ParsePragmaPushOrPopMacro(clang::Token&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7e98d3f)
#11 0x0000000007ea1732 clang::Preprocessor::HandlePragmaPushMacro(clang::Token&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7ea1732)
#12 0x0000000007e9bff7 clang::PragmaNamespace::HandlePragma(clang::Preprocessor&, clang::PragmaIntroducer, clang::Token&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7e9bff7)
#13 0x0000000007e9716c clang::Preprocessor::HandlePragmaDirective(clang::PragmaIntroducer) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7e9716c)
#14 0x0000000007e6ff4c clang::Preprocessor::HandleDirective(clang::Token&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7e6ff4c)
#15 0x0000000007e0dfca clang::Lexer::LexTokenInternal(clang::Token&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7e0dfca)
#16 0x0000000007e101a8 clang::Lexer::Lex(clang::Token&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7e101a8)
#17 0x0000000007eb36dc clang::Preprocessor::Lex(clang::Token&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7eb36dc)
#18 0x0000000006668682 clang::Parser::ConsumeToken() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6668682)
#19 0x0000000006666288 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6666288)
#20 0x0000000004970188 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4970188)
#21 0x0000000004c64025 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c64025)
#22 0x0000000004be0e6e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4be0e6e)
#23 0x0000000004d57b01 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d57b01)
#24 0x0000000000d9ec4f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd9ec4f)
#25 0x0000000000d9582a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#26 0x00000000049d7709 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#27 0x0000000003f03534 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f03534)
#28 0x00000000049d7d1f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#29 0x0000000004999d6d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4999d6d)
#30 0x000000000499adfe clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x499adfe)
#31 0x00000000049a3945 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49a3945)
#32 0x0000000000d9b0ff clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd9b0ff)
#33 0x0000000000c4f5b4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc4f5b4)
#34 0x00007b311ea29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#35 0x00007b311ea29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#36 0x0000000000d952d5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd952d5)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/snbxaecxM
```cpp
#pragma push_macro("")
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/Lex/Preprocessor.cpp:714: clang::IdentifierInfo* clang::Preprocessor::LookUpIdentifierInfo(clang::Token&amp;) const: Assertion `!Identifier.getRawIdentifier().empty() &amp;&amp; "No raw identifier data!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics &lt;source&gt;
1.	&lt;source&gt;:1:2: current parser token 'pragma'
 #<!-- -->0 0x0000000003fc1498 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3fc1498)
 #<!-- -->1 0x0000000003fbe8c4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3fbe8c4)
 #<!-- -->2 0x0000000003f03098 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007b311ea42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007b311ea969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007b311ea42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007b311ea287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x00007b311ea2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x00007b311ea39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000007eb069d (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7eb069d)
#<!-- -->10 0x0000000007e98d3f clang::Preprocessor::ParsePragmaPushOrPopMacro(clang::Token&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7e98d3f)
#<!-- -->11 0x0000000007ea1732 clang::Preprocessor::HandlePragmaPushMacro(clang::Token&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7ea1732)
#<!-- -->12 0x0000000007e9bff7 clang::PragmaNamespace::HandlePragma(clang::Preprocessor&amp;, clang::PragmaIntroducer, clang::Token&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7e9bff7)
#<!-- -->13 0x0000000007e9716c clang::Preprocessor::HandlePragmaDirective(clang::PragmaIntroducer) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7e9716c)
#<!-- -->14 0x0000000007e6ff4c clang::Preprocessor::HandleDirective(clang::Token&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7e6ff4c)
#<!-- -->15 0x0000000007e0dfca clang::Lexer::LexTokenInternal(clang::Token&amp;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7e0dfca)
#<!-- -->16 0x0000000007e101a8 clang::Lexer::Lex(clang::Token&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7e101a8)
#<!-- -->17 0x0000000007eb36dc clang::Preprocessor::Lex(clang::Token&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7eb36dc)
#<!-- -->18 0x0000000006668682 clang::Parser::ConsumeToken() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6668682)
#<!-- -->19 0x0000000006666288 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6666288)
#<!-- -->20 0x0000000004970188 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4970188)
#<!-- -->21 0x0000000004c64025 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c64025)
#<!-- -->22 0x0000000004be0e6e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4be0e6e)
#<!-- -->23 0x0000000004d57b01 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d57b01)
#<!-- -->24 0x0000000000d9ec4f cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd9ec4f)
#<!-- -->25 0x0000000000d9582a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->26 0x00000000049d7709 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->27 0x0000000003f03534 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f03534)
#<!-- -->28 0x00000000049d7d1f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->29 0x0000000004999d6d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4999d6d)
#<!-- -->30 0x000000000499adfe clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x499adfe)
#<!-- -->31 0x00000000049a3945 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49a3945)
#<!-- -->32 0x0000000000d9b0ff clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd9b0ff)
#<!-- -->33 0x0000000000c4f5b4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc4f5b4)
#<!-- -->34 0x00007b311ea29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->35 0x00007b311ea29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->36 0x0000000000d952d5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd952d5)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

### Comment 2 - k-arrows

Goes back to clang-3.5:
https://godbolt.org/z/hcdG3rv61

---

