# [clang] Assertion `ParsingFilename == false && "reentered LexIncludeFilename"' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/178635
**Status:** Closed
**Labels:** clang:frontend, confirmed, crash, embed
**Closed Date:** 2026-02-13T16:58:51Z

## Body

Reproducer:
https://godbolt.org/z/nEfoxGqbe
```cpp
#if __has_embed(__has_include)
#endif
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/Lex/PreprocessorLexer.cpp:32: void clang::PreprocessorLexer::LexIncludeFilename(clang::Token&): Assertion `ParsingFilename == false && "reentered LexIncludeFilename"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<source>:1:2: current parser token 'if'
 #0 0x0000000004322d48 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4322d48)
 #1 0x00000000043201a4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x43201a4)
 #2 0x0000000004261878 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000071d18e642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x000071d18e6969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x000071d18e642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x000071d18e6287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x000071d18e62871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x000071d18e639e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x000000000853cfc7 (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x853cfc7)
#10 0x000000000853aabe clang::Preprocessor::LexHeaderName(clang::Token&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x853aabe)
#11 0x000000000850a80f EvaluateHasIncludeCommon(clang::Token&, clang::IdentifierInfo*, clang::Preprocessor&, clang::detail::SearchDirIteratorImpl<true>, clang::FileEntry const*) PPMacroExpansion.cpp:0:0
#12 0x000000000851110b clang::Preprocessor::ExpandBuiltinMacro(clang::Token&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x851110b)
#13 0x0000000008517066 clang::Preprocessor::HandleMacroExpandedIdentifier(clang::Token&, clang::MacroDefinition const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x8517066)
#14 0x000000000853761e clang::Preprocessor::HandleIdentifier(clang::Token&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x853761e)
#15 0x000000000848bf12 clang::Lexer::LexIdentifierContinue(clang::Token&, char const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x848bf12)
#16 0x000000000848d5ae clang::Lexer::LexTokenInternal(clang::Token&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x848d5ae)
#17 0x000000000848fc1e clang::Lexer::Lex(clang::Token&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x848fc1e)
#18 0x00000000085397fc clang::Preprocessor::Lex(clang::Token&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x85397fc)
#19 0x000000000853cf91 clang::PreprocessorLexer::LexIncludeFilename(clang::Token&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x853cf91)
#20 0x000000000853aabe clang::Preprocessor::LexHeaderName(clang::Token&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x853aabe)
#21 0x000000000850ba6d clang::Preprocessor::EvaluateHasEmbed(clang::Token&, clang::IdentifierInfo*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x850ba6d)
#22 0x00000000085111da clang::Preprocessor::ExpandBuiltinMacro(clang::Token&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x85111da)
#23 0x0000000008517066 clang::Preprocessor::HandleMacroExpandedIdentifier(clang::Token&, clang::MacroDefinition const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x8517066)
#24 0x000000000853761e clang::Preprocessor::HandleIdentifier(clang::Token&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x853761e)
#25 0x000000000848bf12 clang::Lexer::LexIdentifierContinue(clang::Token&, char const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x848bf12)
#26 0x000000000848d5ae clang::Lexer::LexTokenInternal(clang::Token&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x848d5ae)
#27 0x000000000848fc1e clang::Lexer::Lex(clang::Token&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x848fc1e)
#28 0x00000000085397fc clang::Preprocessor::Lex(clang::Token&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x85397fc)
#29 0x00000000084fd41b clang::Preprocessor::EvaluateDirectiveExpression(clang::IdentifierInfo*&, clang::Token&, bool&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x84fd41b)
#30 0x00000000084fde6f clang::Preprocessor::EvaluateDirectiveExpression(clang::IdentifierInfo*&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x84fde6f)
#31 0x00000000084eceff clang::Preprocessor::HandleIfDirective(clang::Token&, clang::Token const&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x84eceff)
#32 0x00000000084f4dbf clang::Preprocessor::HandleDirective(clang::Token&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x84f4dbf)
#33 0x000000000848e15c clang::Lexer::LexTokenInternal(clang::Token&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x848e15c)
#34 0x000000000848fc1e clang::Lexer::Lex(clang::Token&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x848fc1e)
#35 0x00000000085397fc clang::Preprocessor::Lex(clang::Token&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x85397fc)
#36 0x0000000006aa6202 clang::Parser::ConsumeToken() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6aa6202)
#37 0x0000000006a930b8 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a930b8)
#38 0x0000000004c911c8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c911c8)
#39 0x0000000004f8cc55 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f8cc55)
#40 0x0000000004f0c67e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f0c67e)
#41 0x0000000005087e8d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5087e8d)
#42 0x0000000000ddff4e cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xddff4e)
#43 0x0000000000dd691a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#44 0x0000000000dd6a9d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#45 0x0000000004d04c19 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#46 0x0000000004261d14 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4261d14)
#47 0x0000000004d0522f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#48 0x0000000004cc5c02 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4cc5c02)
#49 0x0000000004cc6bae clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4cc6bae)
#50 0x0000000004ccdff5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ccdff5)
#51 0x0000000000ddc2c1 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xddc2c1)
#52 0x0000000000c98c94 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc98c94)
#53 0x000071d18e629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#54 0x000071d18e629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#55 0x0000000000dd63b5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdd63b5)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/nEfoxGqbe
```cpp
#if __has_embed(__has_include)
#endif
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/Lex/PreprocessorLexer.cpp:32: void clang::PreprocessorLexer::LexIncludeFilename(clang::Token&amp;): Assertion `ParsingFilename == false &amp;&amp; "reentered LexIncludeFilename"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics &lt;source&gt;
1.	&lt;source&gt;:1:2: current parser token 'if'
 #<!-- -->0 0x0000000004322d48 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4322d48)
 #<!-- -->1 0x00000000043201a4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x43201a4)
 #<!-- -->2 0x0000000004261878 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x000071d18e642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x000071d18e6969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x000071d18e642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x000071d18e6287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x000071d18e62871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x000071d18e639e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x000000000853cfc7 (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x853cfc7)
#<!-- -->10 0x000000000853aabe clang::Preprocessor::LexHeaderName(clang::Token&amp;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x853aabe)
#<!-- -->11 0x000000000850a80f EvaluateHasIncludeCommon(clang::Token&amp;, clang::IdentifierInfo*, clang::Preprocessor&amp;, clang::detail::SearchDirIteratorImpl&lt;true&gt;, clang::FileEntry const*) PPMacroExpansion.cpp:0:0
#<!-- -->12 0x000000000851110b clang::Preprocessor::ExpandBuiltinMacro(clang::Token&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x851110b)
#<!-- -->13 0x0000000008517066 clang::Preprocessor::HandleMacroExpandedIdentifier(clang::Token&amp;, clang::MacroDefinition const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x8517066)
#<!-- -->14 0x000000000853761e clang::Preprocessor::HandleIdentifier(clang::Token&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x853761e)
#<!-- -->15 0x000000000848bf12 clang::Lexer::LexIdentifierContinue(clang::Token&amp;, char const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x848bf12)
#<!-- -->16 0x000000000848d5ae clang::Lexer::LexTokenInternal(clang::Token&amp;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x848d5ae)
#<!-- -->17 0x000000000848fc1e clang::Lexer::Lex(clang::Token&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x848fc1e)
#<!-- -->18 0x00000000085397fc clang::Preprocessor::Lex(clang::Token&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x85397fc)
#<!-- -->19 0x000000000853cf91 clang::PreprocessorLexer::LexIncludeFilename(clang::Token&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x853cf91)
#<!-- -->20 0x000000000853aabe clang::Preprocessor::LexHeaderName(clang::Token&amp;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x853aabe)
#<!-- -->21 0x000000000850ba6d clang::Preprocessor::EvaluateHasEmbed(clang::Token&amp;, clang::IdentifierInfo*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x850ba6d)
#<!-- -->22 0x00000000085111da clang::Preprocessor::ExpandBuiltinMacro(clang::Token&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x85111da)
#<!-- -->23 0x0000000008517066 clang::Preprocessor::HandleMacroExpandedIdentifier(clang::Token&amp;, clang::MacroDefinition const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x8517066)
#<!-- -->24 0x000000000853761e clang::Preprocessor::HandleIdentifier(clang::Token&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x853761e)
#<!-- -->25 0x000000000848bf12 clang::Lexer::LexIdentifierContinue(clang::Token&amp;, char const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x848bf12)
#<!-- -->26 0x000000000848d5ae clang::Lexer::LexTokenInternal(clang::Token&amp;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x848d5ae)
#<!-- -->27 0x000000000848fc1e clang::Lexer::Lex(clang::Token&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x848fc1e)
#<!-- -->28 0x00000000085397fc clang::Preprocessor::Lex(clang::Token&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x85397fc)
#<!-- -->29 0x00000000084fd41b clang::Preprocessor::EvaluateDirectiveExpression(clang::IdentifierInfo*&amp;, clang::Token&amp;, bool&amp;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x84fd41b)
#<!-- -->30 0x00000000084fde6f clang::Preprocessor::EvaluateDirectiveExpression(clang::IdentifierInfo*&amp;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x84fde6f)
#<!-- -->31 0x00000000084eceff clang::Preprocessor::HandleIfDirective(clang::Token&amp;, clang::Token const&amp;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x84eceff)
#<!-- -->32 0x00000000084f4dbf clang::Preprocessor::HandleDirective(clang::Token&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x84f4dbf)
#<!-- -->33 0x000000000848e15c clang::Lexer::LexTokenInternal(clang::Token&amp;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x848e15c)
#<!-- -->34 0x000000000848fc1e clang::Lexer::Lex(clang::Token&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x848fc1e)
#<!-- -->35 0x00000000085397fc clang::Preprocessor::Lex(clang::Token&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x85397fc)
#<!-- -->36 0x0000000006aa6202 clang::Parser::ConsumeToken() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6aa6202)
#<!-- -->37 0x0000000006a930b8 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a930b8)
#<!-- -->38 0x0000000004c911c8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c911c8)
#<!-- -->39 0x0000000004f8cc55 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f8cc55)
#<!-- -->40 0x0000000004f0c67e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f0c67e)
#<!-- -->41 0x0000000005087e8d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5087e8d)
#<!-- -->42 0x0000000000ddff4e cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xddff4e)
#<!-- -->43 0x0000000000dd691a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->44 0x0000000000dd6a9d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->45 0x0000000004d04c19 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->46 0x0000000004261d14 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4261d14)
#<!-- -->47 0x0000000004d0522f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->48 0x0000000004cc5c02 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4cc5c02)
#<!-- -->49 0x0000000004cc6bae clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4cc6bae)
#<!-- -->50 0x0000000004ccdff5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ccdff5)
#<!-- -->51 0x0000000000ddc2c1 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xddc2c1)
#<!-- -->52 0x0000000000c98c94 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc98c94)
#<!-- -->53 0x000071d18e629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->54 0x000071d18e629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->55 0x0000000000dd63b5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdd63b5)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

### Comment 2 - k-arrows

Goes back to clang-19:
https://godbolt.org/z/4zYT3sjG7

---

### Comment 3 - k-arrows

The above reproducer is available at:
https://github.com/k-arrows/clang-assert-tests/blob/main/cases/00119.cpp

---

### Comment 4 - shafik

@a-tarasyuk since you were just fixing code in this area

https://github.com/llvm/llvm-project/issues/175088

---

### Comment 5 - shafik

@k-arrows it would super helpful if you have reports that hit similar types of code you mention the past reports as well. 

---

### Comment 6 - shafik

@Fznamznon this actually goes all the way back to the original commit: 41c6e4379204ffc00948edd33d59ba5ebbceaba2

---

### Comment 7 - a-tarasyuk

@shafik I think `__has_embed` added one more case; however, nested `__has_include` https://godbolt.org/z/T3bqv3YvP, also causes the assertion failure, and this goes back to 9.0.0

---

