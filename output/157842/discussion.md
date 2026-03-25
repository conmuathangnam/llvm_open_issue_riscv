# [clang] Assertion `(Params || Tok.is(tok::eod)) && "expected success or to be at the end of the directive"' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/157842
**Status:** Closed
**Labels:** clang:frontend, crash-on-invalid, confirmed, embed
**Closed Date:** 2025-09-11T08:49:44Z

## Body

Reproducer:
https://godbolt.org/z/K5bsxeWh8
```cpp
#if __has_embed (__FILE__ limit (-1))
#endif
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/Lex/PPMacroExpansion.cpp:1265: clang::EmbedResult clang::Preprocessor::EvaluateHasEmbed(clang::Token&, clang::IdentifierInfo*): Assertion `(Params || Tok.is(tok::eod)) && "expected success or to be at the end of the directive"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<source>:1:2: current parser token 'if'
 #0 0x0000000004083a38 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4083a38)
 #1 0x0000000004080e64 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4080e64)
 #2 0x0000000003fc51e8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000070d5e7842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x000070d5e78969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x000070d5e7842476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x000070d5e78287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x000070d5e782871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x000070d5e7839e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000007f4a3d9 clang::Preprocessor::EvaluateHasEmbed(clang::Token&, clang::IdentifierInfo*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7f4a3d9)
#10 0x0000000007f4f2f3 clang::Preprocessor::ExpandBuiltinMacro(clang::Token&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7f4f2f3)
#11 0x0000000007f552a6 clang::Preprocessor::HandleMacroExpandedIdentifier(clang::Token&, clang::MacroDefinition const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7f552a6)
#12 0x0000000007f754fa clang::Preprocessor::HandleIdentifier(clang::Token&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7f754fa)
#13 0x0000000007ecdd72 clang::Lexer::LexIdentifierContinue(clang::Token&, char const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7ecdd72)
#14 0x0000000007ecf40e clang::Lexer::LexTokenInternal(clang::Token&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7ecf40e)
#15 0x0000000007ed1a2e clang::Lexer::Lex(clang::Token&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7ed1a2e)
#16 0x0000000007f77efc clang::Preprocessor::Lex(clang::Token&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7f77efc)
#17 0x0000000007f3b48b clang::Preprocessor::EvaluateDirectiveExpression(clang::IdentifierInfo*&, clang::Token&, bool&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7f3b48b)
#18 0x0000000007f3bd2f clang::Preprocessor::EvaluateDirectiveExpression(clang::IdentifierInfo*&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7f3bd2f)
#19 0x0000000007f2d48f clang::Preprocessor::HandleIfDirective(clang::Token&, clang::Token const&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7f2d48f)
#20 0x0000000007f32c99 clang::Preprocessor::HandleDirective(clang::Token&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7f32c99)
#21 0x0000000007ecf76a clang::Lexer::LexTokenInternal(clang::Token&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7ecf76a)
#22 0x0000000007ed1a2e clang::Lexer::Lex(clang::Token&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7ed1a2e)
#23 0x0000000007f77efc clang::Preprocessor::Lex(clang::Token&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7f77efc)
#24 0x000000000672eaf2 clang::Parser::ConsumeToken() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x672eaf2)
#25 0x000000000671b588 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x671b588)
#26 0x00000000049bbfa8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49bbfa8)
#27 0x0000000004cb65c5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4cb65c5)
#28 0x0000000004c31b8e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c31b8e)
#29 0x0000000004dabd81 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4dabd81)
#30 0x0000000000db57bf cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb57bf)
#31 0x0000000000dac46a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#32 0x0000000004a25019 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#33 0x0000000003fc5684 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3fc5684)
#34 0x0000000004a2562f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#35 0x00000000049e619d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49e619d)
#36 0x00000000049e722e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49e722e)
#37 0x00000000049ef1c5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49ef1c5)
#38 0x0000000000db1c75 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb1c75)
#39 0x0000000000c647d4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc647d4)
#40 0x000070d5e7829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#41 0x000070d5e7829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#42 0x0000000000dabf15 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdabf15)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/K5bsxeWh8
```cpp
#if __has_embed (__FILE__ limit (-1))
#endif
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/Lex/PPMacroExpansion.cpp:1265: clang::EmbedResult clang::Preprocessor::EvaluateHasEmbed(clang::Token&amp;, clang::IdentifierInfo*): Assertion `(Params || Tok.is(tok::eod)) &amp;&amp; "expected success or to be at the end of the directive"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics &lt;source&gt;
1.	&lt;source&gt;:1:2: current parser token 'if'
 #<!-- -->0 0x0000000004083a38 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4083a38)
 #<!-- -->1 0x0000000004080e64 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4080e64)
 #<!-- -->2 0x0000000003fc51e8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x000070d5e7842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x000070d5e78969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x000070d5e7842476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x000070d5e78287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x000070d5e782871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x000070d5e7839e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000007f4a3d9 clang::Preprocessor::EvaluateHasEmbed(clang::Token&amp;, clang::IdentifierInfo*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7f4a3d9)
#<!-- -->10 0x0000000007f4f2f3 clang::Preprocessor::ExpandBuiltinMacro(clang::Token&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7f4f2f3)
#<!-- -->11 0x0000000007f552a6 clang::Preprocessor::HandleMacroExpandedIdentifier(clang::Token&amp;, clang::MacroDefinition const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7f552a6)
#<!-- -->12 0x0000000007f754fa clang::Preprocessor::HandleIdentifier(clang::Token&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7f754fa)
#<!-- -->13 0x0000000007ecdd72 clang::Lexer::LexIdentifierContinue(clang::Token&amp;, char const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7ecdd72)
#<!-- -->14 0x0000000007ecf40e clang::Lexer::LexTokenInternal(clang::Token&amp;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7ecf40e)
#<!-- -->15 0x0000000007ed1a2e clang::Lexer::Lex(clang::Token&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7ed1a2e)
#<!-- -->16 0x0000000007f77efc clang::Preprocessor::Lex(clang::Token&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7f77efc)
#<!-- -->17 0x0000000007f3b48b clang::Preprocessor::EvaluateDirectiveExpression(clang::IdentifierInfo*&amp;, clang::Token&amp;, bool&amp;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7f3b48b)
#<!-- -->18 0x0000000007f3bd2f clang::Preprocessor::EvaluateDirectiveExpression(clang::IdentifierInfo*&amp;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7f3bd2f)
#<!-- -->19 0x0000000007f2d48f clang::Preprocessor::HandleIfDirective(clang::Token&amp;, clang::Token const&amp;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7f2d48f)
#<!-- -->20 0x0000000007f32c99 clang::Preprocessor::HandleDirective(clang::Token&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7f32c99)
#<!-- -->21 0x0000000007ecf76a clang::Lexer::LexTokenInternal(clang::Token&amp;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7ecf76a)
#<!-- -->22 0x0000000007ed1a2e clang::Lexer::Lex(clang::Token&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7ed1a2e)
#<!-- -->23 0x0000000007f77efc clang::Preprocessor::Lex(clang::Token&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7f77efc)
#<!-- -->24 0x000000000672eaf2 clang::Parser::ConsumeToken() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x672eaf2)
#<!-- -->25 0x000000000671b588 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x671b588)
#<!-- -->26 0x00000000049bbfa8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49bbfa8)
#<!-- -->27 0x0000000004cb65c5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4cb65c5)
#<!-- -->28 0x0000000004c31b8e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c31b8e)
#<!-- -->29 0x0000000004dabd81 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4dabd81)
#<!-- -->30 0x0000000000db57bf cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb57bf)
#<!-- -->31 0x0000000000dac46a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->32 0x0000000004a25019 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->33 0x0000000003fc5684 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3fc5684)
#<!-- -->34 0x0000000004a2562f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->35 0x00000000049e619d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49e619d)
#<!-- -->36 0x00000000049e722e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49e722e)
#<!-- -->37 0x00000000049ef1c5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49ef1c5)
#<!-- -->38 0x0000000000db1c75 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb1c75)
#<!-- -->39 0x0000000000c647d4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc647d4)
#<!-- -->40 0x000070d5e7829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->41 0x000070d5e7829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->42 0x0000000000dabf15 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdabf15)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

### Comment 2 - k-arrows

Goes back to clang-19.1.0:
https://godbolt.org/z/zf6v857Pd

---

