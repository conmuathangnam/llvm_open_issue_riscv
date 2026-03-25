# [clang] Assertion `!Filename.empty()' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/159898
**Status:** Closed
**Labels:** clang:frontend, confirmed, crash, embed
**Closed Date:** 2025-09-22T09:49:31Z

## Body

Reproducer:
https://godbolt.org/z/bjTMvhK3f
```cpp
#if __has_embed("")
#endif
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/Lex/PPMacroExpansion.cpp:1289: clang::EmbedResult clang::Preprocessor::EvaluateHasEmbed(clang::Token&, clang::IdentifierInfo*): Assertion `!Filename.empty()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<source>:1:2: current parser token 'if'
 #0 0x0000000004187908 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4187908)
 #1 0x0000000004184d34 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4184d34)
 #2 0x00000000040c93c8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007b04d4842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007b04d48969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007b04d4842476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007b04d48287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007b04d482871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007b04d4839e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000008027a19 (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x8027a19)
#10 0x000000000802c953 clang::Preprocessor::ExpandBuiltinMacro(clang::Token&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x802c953)
#11 0x00000000080328d6 clang::Preprocessor::HandleMacroExpandedIdentifier(clang::Token&, clang::MacroDefinition const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x80328d6)
#12 0x0000000008052b0a clang::Preprocessor::HandleIdentifier(clang::Token&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x8052b0a)
#13 0x0000000007fab762 clang::Lexer::LexIdentifierContinue(clang::Token&, char const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7fab762)
#14 0x0000000007facdfe clang::Lexer::LexTokenInternal(clang::Token&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7facdfe)
#15 0x0000000007faf41e clang::Lexer::Lex(clang::Token&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7faf41e)
#16 0x000000000805550c clang::Preprocessor::Lex(clang::Token&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x805550c)
#17 0x0000000008018c5b clang::Preprocessor::EvaluateDirectiveExpression(clang::IdentifierInfo*&, clang::Token&, bool&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x8018c5b)
#18 0x00000000080194ff clang::Preprocessor::EvaluateDirectiveExpression(clang::IdentifierInfo*&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x80194ff)
#19 0x000000000800ac5f clang::Preprocessor::HandleIfDirective(clang::Token&, clang::Token const&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x800ac5f)
#20 0x0000000008010469 clang::Preprocessor::HandleDirective(clang::Token&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x8010469)
#21 0x0000000007fad15a clang::Lexer::LexTokenInternal(clang::Token&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7fad15a)
#22 0x0000000007faf41e clang::Lexer::Lex(clang::Token&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7faf41e)
#23 0x000000000805550c clang::Preprocessor::Lex(clang::Token&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x805550c)
#24 0x000000000684b812 clang::Parser::ConsumeToken() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x684b812)
#25 0x00000000068382a8 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68382a8)
#26 0x0000000004ac1e68 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ac1e68)
#27 0x0000000004dad415 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4dad415)
#28 0x0000000004d292ee clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d292ee)
#29 0x0000000004ea2d5d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ea2d5d)
#30 0x0000000000db5f70 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb5f70)
#31 0x0000000000daca9a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#32 0x0000000000dacc1d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#33 0x0000000004b2ae99 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#34 0x00000000040c9864 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x40c9864)
#35 0x0000000004b2b4af clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#36 0x0000000004aec8e2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4aec8e2)
#37 0x0000000004aed88e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4aed88e)
#38 0x0000000004af4fb5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4af4fb5)
#39 0x0000000000db2471 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb2471)
#40 0x0000000000c63fb4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc63fb4)
#41 0x00007b04d4829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#42 0x00007b04d4829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#43 0x0000000000dac535 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdac535)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/bjTMvhK3f
```cpp
#if __has_embed("")
#endif
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/Lex/PPMacroExpansion.cpp:1289: clang::EmbedResult clang::Preprocessor::EvaluateHasEmbed(clang::Token&amp;, clang::IdentifierInfo*): Assertion `!Filename.empty()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics &lt;source&gt;
1.	&lt;source&gt;:1:2: current parser token 'if'
 #<!-- -->0 0x0000000004187908 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4187908)
 #<!-- -->1 0x0000000004184d34 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4184d34)
 #<!-- -->2 0x00000000040c93c8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007b04d4842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007b04d48969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007b04d4842476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007b04d48287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x00007b04d482871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x00007b04d4839e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000008027a19 (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x8027a19)
#<!-- -->10 0x000000000802c953 clang::Preprocessor::ExpandBuiltinMacro(clang::Token&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x802c953)
#<!-- -->11 0x00000000080328d6 clang::Preprocessor::HandleMacroExpandedIdentifier(clang::Token&amp;, clang::MacroDefinition const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x80328d6)
#<!-- -->12 0x0000000008052b0a clang::Preprocessor::HandleIdentifier(clang::Token&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x8052b0a)
#<!-- -->13 0x0000000007fab762 clang::Lexer::LexIdentifierContinue(clang::Token&amp;, char const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7fab762)
#<!-- -->14 0x0000000007facdfe clang::Lexer::LexTokenInternal(clang::Token&amp;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7facdfe)
#<!-- -->15 0x0000000007faf41e clang::Lexer::Lex(clang::Token&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7faf41e)
#<!-- -->16 0x000000000805550c clang::Preprocessor::Lex(clang::Token&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x805550c)
#<!-- -->17 0x0000000008018c5b clang::Preprocessor::EvaluateDirectiveExpression(clang::IdentifierInfo*&amp;, clang::Token&amp;, bool&amp;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x8018c5b)
#<!-- -->18 0x00000000080194ff clang::Preprocessor::EvaluateDirectiveExpression(clang::IdentifierInfo*&amp;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x80194ff)
#<!-- -->19 0x000000000800ac5f clang::Preprocessor::HandleIfDirective(clang::Token&amp;, clang::Token const&amp;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x800ac5f)
#<!-- -->20 0x0000000008010469 clang::Preprocessor::HandleDirective(clang::Token&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x8010469)
#<!-- -->21 0x0000000007fad15a clang::Lexer::LexTokenInternal(clang::Token&amp;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7fad15a)
#<!-- -->22 0x0000000007faf41e clang::Lexer::Lex(clang::Token&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7faf41e)
#<!-- -->23 0x000000000805550c clang::Preprocessor::Lex(clang::Token&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x805550c)
#<!-- -->24 0x000000000684b812 clang::Parser::ConsumeToken() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x684b812)
#<!-- -->25 0x00000000068382a8 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68382a8)
#<!-- -->26 0x0000000004ac1e68 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ac1e68)
#<!-- -->27 0x0000000004dad415 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4dad415)
#<!-- -->28 0x0000000004d292ee clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d292ee)
#<!-- -->29 0x0000000004ea2d5d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ea2d5d)
#<!-- -->30 0x0000000000db5f70 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb5f70)
#<!-- -->31 0x0000000000daca9a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->32 0x0000000000dacc1d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->33 0x0000000004b2ae99 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->34 0x00000000040c9864 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x40c9864)
#<!-- -->35 0x0000000004b2b4af clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->36 0x0000000004aec8e2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4aec8e2)
#<!-- -->37 0x0000000004aed88e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4aed88e)
#<!-- -->38 0x0000000004af4fb5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4af4fb5)
#<!-- -->39 0x0000000000db2471 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb2471)
#<!-- -->40 0x0000000000c63fb4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc63fb4)
#<!-- -->41 0x00007b04d4829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->42 0x00007b04d4829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->43 0x0000000000dac535 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdac535)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

### Comment 2 - shafik

CC @Fznamznon @AaronBallman 

---

