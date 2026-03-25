# [clang] crashes where builtin feature check macro lacks a parenthesized identifier

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/178098
**Status:** Closed
**Labels:** clang:frontend, crash-on-invalid, confirmed
**Closed Date:** 2026-01-28T13:44:50Z

## Body

Reproducer:
https://godbolt.org/z/sG6eTEP3e
```cpp
__has_cpp_attribute(clang::
```

Backtrace:
```console
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<eof> parser at end of file
 #0 0x000000000430e638 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x430e638)
 #1 0x000000000430ba94 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x430ba94)
 #2 0x000000000424d858 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007fd1aee42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00000000084695b1 clang::Lexer::Lex(clang::Token&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x84695b1)
 #5 0x0000000008512a9c clang::Preprocessor::Lex(clang::Token&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x8512a9c)
 #6 0x00000000084e42c6 EvaluateFeatureLikeBuiltinMacro(llvm::raw_svector_ostream&, clang::Token&, clang::IdentifierInfo*, clang::Preprocessor&, bool, llvm::function_ref<int (clang::Token&, bool&)>) PPMacroExpansion.cpp:0:0
 #7 0x00000000084e951e clang::Preprocessor::ExpandBuiltinMacro(clang::Token&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x84e951e)
 #8 0x00000000084f0306 clang::Preprocessor::HandleMacroExpandedIdentifier(clang::Token&, clang::MacroDefinition const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x84f0306)
 #9 0x00000000085108be clang::Preprocessor::HandleIdentifier(clang::Token&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x85108be)
#10 0x0000000008465922 clang::Lexer::LexIdentifierContinue(clang::Token&, char const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x8465922)
#11 0x0000000008466fbe clang::Lexer::LexTokenInternal(clang::Token&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x8466fbe)
#12 0x000000000846962e clang::Lexer::Lex(clang::Token&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x846962e)
#13 0x0000000008512a9c clang::Preprocessor::Lex(clang::Token&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x8512a9c)
#14 0x0000000006a8a322 clang::Parser::ConsumeToken() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a8a322)
#15 0x0000000006a771d8 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a771d8)
#16 0x0000000004c77cd8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c77cd8)
#17 0x0000000004f72e35 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f72e35)
#18 0x0000000004ef285e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ef285e)
#19 0x000000000506e01d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x506e01d)
#20 0x0000000000dde97e cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdde97e)
#21 0x0000000000dd534a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#22 0x0000000000dd54cd int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#23 0x0000000004ceb719 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#24 0x000000000424dcf4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x424dcf4)
#25 0x0000000004cebd2f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#26 0x0000000004cac702 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4cac702)
#27 0x0000000004cad6ae clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4cad6ae)
#28 0x0000000004cb4af5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4cb4af5)
#29 0x0000000000ddacf1 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xddacf1)
#30 0x0000000000c97d44 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc97d44)
#31 0x00007fd1aee29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#32 0x00007fd1aee29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#33 0x0000000000dd4de5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdd4de5)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/sG6eTEP3e
```cpp
__has_cpp_attribute(clang::
```

Backtrace:
```console
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
 #<!-- -->0 0x000000000430e638 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x430e638)
 #<!-- -->1 0x000000000430ba94 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x430ba94)
 #<!-- -->2 0x000000000424d858 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007fd1aee42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00000000084695b1 clang::Lexer::Lex(clang::Token&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x84695b1)
 #<!-- -->5 0x0000000008512a9c clang::Preprocessor::Lex(clang::Token&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x8512a9c)
 #<!-- -->6 0x00000000084e42c6 EvaluateFeatureLikeBuiltinMacro(llvm::raw_svector_ostream&amp;, clang::Token&amp;, clang::IdentifierInfo*, clang::Preprocessor&amp;, bool, llvm::function_ref&lt;int (clang::Token&amp;, bool&amp;)&gt;) PPMacroExpansion.cpp:0:0
 #<!-- -->7 0x00000000084e951e clang::Preprocessor::ExpandBuiltinMacro(clang::Token&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x84e951e)
 #<!-- -->8 0x00000000084f0306 clang::Preprocessor::HandleMacroExpandedIdentifier(clang::Token&amp;, clang::MacroDefinition const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x84f0306)
 #<!-- -->9 0x00000000085108be clang::Preprocessor::HandleIdentifier(clang::Token&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x85108be)
#<!-- -->10 0x0000000008465922 clang::Lexer::LexIdentifierContinue(clang::Token&amp;, char const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x8465922)
#<!-- -->11 0x0000000008466fbe clang::Lexer::LexTokenInternal(clang::Token&amp;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x8466fbe)
#<!-- -->12 0x000000000846962e clang::Lexer::Lex(clang::Token&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x846962e)
#<!-- -->13 0x0000000008512a9c clang::Preprocessor::Lex(clang::Token&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x8512a9c)
#<!-- -->14 0x0000000006a8a322 clang::Parser::ConsumeToken() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a8a322)
#<!-- -->15 0x0000000006a771d8 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a771d8)
#<!-- -->16 0x0000000004c77cd8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c77cd8)
#<!-- -->17 0x0000000004f72e35 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f72e35)
#<!-- -->18 0x0000000004ef285e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ef285e)
#<!-- -->19 0x000000000506e01d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x506e01d)
#<!-- -->20 0x0000000000dde97e cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdde97e)
#<!-- -->21 0x0000000000dd534a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->22 0x0000000000dd54cd int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->23 0x0000000004ceb719 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->24 0x000000000424dcf4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x424dcf4)
#<!-- -->25 0x0000000004cebd2f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->26 0x0000000004cac702 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4cac702)
#<!-- -->27 0x0000000004cad6ae clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4cad6ae)
#<!-- -->28 0x0000000004cb4af5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4cb4af5)
#<!-- -->29 0x0000000000ddacf1 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xddacf1)
#<!-- -->30 0x0000000000c97d44 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc97d44)
#<!-- -->31 0x00007fd1aee29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->32 0x00007fd1aee29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->33 0x0000000000dd4de5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdd4de5)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```
</details>


---

### Comment 2 - k-arrows

Goes back to clang-3.9:
https://godbolt.org/z/MrMdehhGP

---

