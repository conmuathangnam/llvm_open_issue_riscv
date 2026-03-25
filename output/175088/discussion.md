# [clang] Assertion `Tmp.isNot(tok::eof) && "EOF seen while discarding directive tokens"' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/175088
**Status:** Closed
**Labels:** clang:frontend, regression, crash-on-invalid, confirmed, embed
**Closed Date:** 2026-01-26T12:39:11Z

## Body

Reproducer:
https://godbolt.org/z/nzovT3Yrb
```cpp
#if __has_embed("" if_empty
```

The above reproducer is available at:
https://github.com/k-arrows/clang-assert-tests/blob/main/cases/00064.cpp

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/Lex/PPDirectives.cpp:91: clang::SourceRange clang::Preprocessor::DiscardUntilEndOfDirective(clang::Token&): Assertion `Tmp.isNot(tok::eof) && "EOF seen while discarding directive tokens"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<source>:1:2: current parser token 'if'
 #0 0x00000000042ffb48 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42ffb48)
 #1 0x00000000042fcf74 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42fcf74)
 #2 0x000000000423fd48 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x0000753cf7642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x0000753cf76969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x0000753cf7642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x0000753cf76287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x0000753cf762871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x0000753cf7639e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x000000000845af5c (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x845af5c)
#10 0x000000000845cc35 clang::Preprocessor::LexEmbedParameters(clang::Token&, bool)::'lambda5'(llvm::SmallVectorImpl<clang::Token>&)::operator()(llvm::SmallVectorImpl<clang::Token>&) const (.constprop.0) PPDirectives.cpp:0:0
#11 0x0000000008462ab2 clang::Preprocessor::LexEmbedParameters(clang::Token&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x8462ab2)
#12 0x00000000084861fd clang::Preprocessor::EvaluateHasEmbed(clang::Token&, clang::IdentifierInfo*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x84861fd)
#13 0x000000000848b8fa clang::Preprocessor::ExpandBuiltinMacro(clang::Token&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x848b8fa)
#14 0x00000000084918a6 clang::Preprocessor::HandleMacroExpandedIdentifier(clang::Token&, clang::MacroDefinition const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x84918a6)
#15 0x00000000084b1c54 clang::Preprocessor::HandleIdentifier(clang::Token&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x84b1c54)
#16 0x000000000840a5ea clang::Lexer::LexIdentifierContinue(clang::Token&, char const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x840a5ea)
#17 0x000000000840aa9e clang::Lexer::LexTokenInternal(clang::Token&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x840aa9e)
#18 0x000000000840d0be clang::Lexer::Lex(clang::Token&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x840d0be)
#19 0x00000000084b468c clang::Preprocessor::Lex(clang::Token&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x84b468c)
#20 0x0000000008477efb clang::Preprocessor::EvaluateDirectiveExpression(clang::IdentifierInfo*&, clang::Token&, bool&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x8477efb)
#21 0x000000000847893f clang::Preprocessor::EvaluateDirectiveExpression(clang::IdentifierInfo*&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x847893f)
#22 0x000000000846830f clang::Preprocessor::HandleIfDirective(clang::Token&, clang::Token const&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x846830f)
#23 0x000000000846f7c9 clang::Preprocessor::HandleDirective(clang::Token&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x846f7c9)
#24 0x000000000840adfa clang::Lexer::LexTokenInternal(clang::Token&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x840adfa)
#25 0x000000000840d0be clang::Lexer::Lex(clang::Token&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x840d0be)
#26 0x00000000084b468c clang::Preprocessor::Lex(clang::Token&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x84b468c)
#27 0x0000000006a47d72 clang::Parser::ConsumeToken() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a47d72)
#28 0x0000000006a34e38 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a34e38)
#29 0x0000000004c657e8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c657e8)
#30 0x0000000004f5db65 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f5db65)
#31 0x0000000004eddc6e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4eddc6e)
#32 0x000000000505829d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x505829d)
#33 0x0000000000de87de cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde87de)
#34 0x0000000000ddf1aa ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#35 0x0000000000ddf32d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#36 0x0000000004cd84e9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#37 0x00000000042401e4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42401e4)
#38 0x0000000004cd8aff clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#39 0x0000000004c99492 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c99492)
#40 0x0000000004c9a43e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c9a43e)
#41 0x0000000004ca1895 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ca1895)
#42 0x0000000000de4b51 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde4b51)
#43 0x0000000000c8e8a4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc8e8a4)
#44 0x0000753cf7629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#45 0x0000753cf7629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#46 0x0000000000ddec45 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xddec45)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

This seems a regression on trunk:
https://godbolt.org/z/9hYz61jGc

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/nzovT3Yrb
```cpp
#if __has_embed("" if_empty
```

The above reproducer is available at:
https://github.com/k-arrows/clang-assert-tests/blob/main/cases/00064.cpp

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/Lex/PPDirectives.cpp:91: clang::SourceRange clang::Preprocessor::DiscardUntilEndOfDirective(clang::Token&amp;): Assertion `Tmp.isNot(tok::eof) &amp;&amp; "EOF seen while discarding directive tokens"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics &lt;source&gt;
1.	&lt;source&gt;:1:2: current parser token 'if'
 #<!-- -->0 0x00000000042ffb48 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42ffb48)
 #<!-- -->1 0x00000000042fcf74 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42fcf74)
 #<!-- -->2 0x000000000423fd48 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x0000753cf7642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x0000753cf76969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x0000753cf7642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x0000753cf76287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x0000753cf762871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x0000753cf7639e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x000000000845af5c (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x845af5c)
#<!-- -->10 0x000000000845cc35 clang::Preprocessor::LexEmbedParameters(clang::Token&amp;, bool)::'lambda5'(llvm::SmallVectorImpl&lt;clang::Token&gt;&amp;)::operator()(llvm::SmallVectorImpl&lt;clang::Token&gt;&amp;) const (.constprop.0) PPDirectives.cpp:0:0
#<!-- -->11 0x0000000008462ab2 clang::Preprocessor::LexEmbedParameters(clang::Token&amp;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x8462ab2)
#<!-- -->12 0x00000000084861fd clang::Preprocessor::EvaluateHasEmbed(clang::Token&amp;, clang::IdentifierInfo*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x84861fd)
#<!-- -->13 0x000000000848b8fa clang::Preprocessor::ExpandBuiltinMacro(clang::Token&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x848b8fa)
#<!-- -->14 0x00000000084918a6 clang::Preprocessor::HandleMacroExpandedIdentifier(clang::Token&amp;, clang::MacroDefinition const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x84918a6)
#<!-- -->15 0x00000000084b1c54 clang::Preprocessor::HandleIdentifier(clang::Token&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x84b1c54)
#<!-- -->16 0x000000000840a5ea clang::Lexer::LexIdentifierContinue(clang::Token&amp;, char const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x840a5ea)
#<!-- -->17 0x000000000840aa9e clang::Lexer::LexTokenInternal(clang::Token&amp;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x840aa9e)
#<!-- -->18 0x000000000840d0be clang::Lexer::Lex(clang::Token&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x840d0be)
#<!-- -->19 0x00000000084b468c clang::Preprocessor::Lex(clang::Token&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x84b468c)
#<!-- -->20 0x0000000008477efb clang::Preprocessor::EvaluateDirectiveExpression(clang::IdentifierInfo*&amp;, clang::Token&amp;, bool&amp;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x8477efb)
#<!-- -->21 0x000000000847893f clang::Preprocessor::EvaluateDirectiveExpression(clang::IdentifierInfo*&amp;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x847893f)
#<!-- -->22 0x000000000846830f clang::Preprocessor::HandleIfDirective(clang::Token&amp;, clang::Token const&amp;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x846830f)
#<!-- -->23 0x000000000846f7c9 clang::Preprocessor::HandleDirective(clang::Token&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x846f7c9)
#<!-- -->24 0x000000000840adfa clang::Lexer::LexTokenInternal(clang::Token&amp;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x840adfa)
#<!-- -->25 0x000000000840d0be clang::Lexer::Lex(clang::Token&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x840d0be)
#<!-- -->26 0x00000000084b468c clang::Preprocessor::Lex(clang::Token&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x84b468c)
#<!-- -->27 0x0000000006a47d72 clang::Parser::ConsumeToken() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a47d72)
#<!-- -->28 0x0000000006a34e38 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a34e38)
#<!-- -->29 0x0000000004c657e8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c657e8)
#<!-- -->30 0x0000000004f5db65 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f5db65)
#<!-- -->31 0x0000000004eddc6e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4eddc6e)
#<!-- -->32 0x000000000505829d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x505829d)
#<!-- -->33 0x0000000000de87de cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde87de)
#<!-- -->34 0x0000000000ddf1aa ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->35 0x0000000000ddf32d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->36 0x0000000004cd84e9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->37 0x00000000042401e4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42401e4)
#<!-- -->38 0x0000000004cd8aff clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->39 0x0000000004c99492 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c99492)
#<!-- -->40 0x0000000004c9a43e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c9a43e)
#<!-- -->41 0x0000000004ca1895 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ca1895)
#<!-- -->42 0x0000000000de4b51 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde4b51)
#<!-- -->43 0x0000000000c8e8a4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc8e8a4)
#<!-- -->44 0x0000753cf7629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->45 0x0000753cf7629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->46 0x0000000000ddec45 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xddec45)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

This seems a regression on trunk:
https://godbolt.org/z/9hYz61jGc
</details>


---

### Comment 2 - shafik

@a-tarasyuk this bisects to 71ad307d231c8048327c7a74c1bb38f0b86fde15

---

### Comment 3 - a-tarasyuk

@shafik, should this be reverted, or is it okay to proceed with https://github.com/llvm/llvm-project/pull/175104?

---

### Comment 4 - shafik

> [@shafik](https://github.com/shafik), should this be reverted, or is it okay to proceed with [#175104](https://github.com/llvm/llvm-project/pull/175104)?

@AaronBallman it feels since this is invalid code, so we should just proceed w/ the fix. 

---

### Comment 5 - AaronBallman

I think the fix should be straightforward; I'd fix forward and then port to the release branch.

---

### Comment 6 - a-tarasyuk

@AaronBallman the fix is here https://github.com/llvm/llvm-project/pull/175104

---

