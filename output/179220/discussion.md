# [clang] Assertion `getLangOpts().CPlusPlusModules && ModuleTok.is(tok::kw_module)' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/179220

## Body

Reproducer:
https://github.com/k-arrows/clang-assert-tests/blob/main/cases/00131.cpp

https://godbolt.org/z/ohWv95s56
```cpp
#module
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/Lex/PPDirectives.cpp:4360: void clang::Preprocessor::HandleCXXModuleDirective(clang::Token): Assertion `getLangOpts().CPlusPlusModules && ModuleTok.is(tok::kw_module)' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<source>:1:2: current parser token 'module'
 #0 0x000000000432e038 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x432e038)
 #1 0x000000000432b494 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x432b494)
 #2 0x000000000426cb68 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x0000764199e42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x0000764199e969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x0000764199e42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x0000764199e287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x0000764199e2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x0000764199e39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x00000000084fcc12 clang::Preprocessor::HandleCXXModuleDirective(clang::Token) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x84fcc12)
#10 0x000000000850803c clang::Preprocessor::HandleDirective(clang::Token&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x850803c)
#11 0x00000000084a15ec clang::Lexer::LexTokenInternal(clang::Token&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x84a15ec)
#12 0x00000000084a30ae clang::Lexer::Lex(clang::Token&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x84a30ae)
#13 0x000000000854cc6c clang::Preprocessor::Lex(clang::Token&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x854cc6c)
#14 0x0000000006ab8632 clang::Parser::ConsumeToken() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ab8632)
#15 0x0000000006aa54e8 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6aa54e8)
#16 0x0000000004c9de08 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c9de08)
#17 0x0000000004f9a625 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f9a625)
#18 0x0000000004f19dbe clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f19dbe)
#19 0x000000000509574d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x509574d)
#20 0x0000000000de13ce cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde13ce)
#21 0x0000000000dd7d9a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#22 0x0000000000dd7f1d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#23 0x0000000004d11849 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#24 0x000000000426d004 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x426d004)
#25 0x0000000004d11e5f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#26 0x0000000004cd2832 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4cd2832)
#27 0x0000000004cd37de clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4cd37de)
#28 0x0000000004cdac25 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4cdac25)
#29 0x0000000000ddd741 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xddd741)
#30 0x0000000000c99734 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc99734)
#31 0x0000764199e29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#32 0x0000764199e29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#33 0x0000000000dd7835 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdd7835)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (k-arrows)

<details>
Reproducer:
https://github.com/k-arrows/clang-assert-tests/blob/main/cases/00131.cpp

https://godbolt.org/z/ohWv95s56
```cpp
#module
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/Lex/PPDirectives.cpp:4360: void clang::Preprocessor::HandleCXXModuleDirective(clang::Token): Assertion `getLangOpts().CPlusPlusModules &amp;&amp; ModuleTok.is(tok::kw_module)' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics &lt;source&gt;
1.	&lt;source&gt;:1:2: current parser token 'module'
 #<!-- -->0 0x000000000432e038 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x432e038)
 #<!-- -->1 0x000000000432b494 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x432b494)
 #<!-- -->2 0x000000000426cb68 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x0000764199e42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x0000764199e969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x0000764199e42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x0000764199e287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x0000764199e2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x0000764199e39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x00000000084fcc12 clang::Preprocessor::HandleCXXModuleDirective(clang::Token) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x84fcc12)
#<!-- -->10 0x000000000850803c clang::Preprocessor::HandleDirective(clang::Token&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x850803c)
#<!-- -->11 0x00000000084a15ec clang::Lexer::LexTokenInternal(clang::Token&amp;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x84a15ec)
#<!-- -->12 0x00000000084a30ae clang::Lexer::Lex(clang::Token&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x84a30ae)
#<!-- -->13 0x000000000854cc6c clang::Preprocessor::Lex(clang::Token&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x854cc6c)
#<!-- -->14 0x0000000006ab8632 clang::Parser::ConsumeToken() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ab8632)
#<!-- -->15 0x0000000006aa54e8 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6aa54e8)
#<!-- -->16 0x0000000004c9de08 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c9de08)
#<!-- -->17 0x0000000004f9a625 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f9a625)
#<!-- -->18 0x0000000004f19dbe clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f19dbe)
#<!-- -->19 0x000000000509574d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x509574d)
#<!-- -->20 0x0000000000de13ce cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde13ce)
#<!-- -->21 0x0000000000dd7d9a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->22 0x0000000000dd7f1d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->23 0x0000000004d11849 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->24 0x000000000426d004 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x426d004)
#<!-- -->25 0x0000000004d11e5f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->26 0x0000000004cd2832 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4cd2832)
#<!-- -->27 0x0000000004cd37de clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4cd37de)
#<!-- -->28 0x0000000004cdac25 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4cdac25)
#<!-- -->29 0x0000000000ddd741 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xddd741)
#<!-- -->30 0x0000000000c99734 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc99734)
#<!-- -->31 0x0000764199e29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->32 0x0000764199e29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->33 0x0000000000dd7835 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdd7835)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

### Comment 2 - k-arrows

does not crash with clang-21:
https://godbolt.org/z/bxrn1Wcdc

---

### Comment 3 - shafik

@yronglin this bisects to 1da403937eb9f48b2de9c27ba1aa0eba50bfdf5f

---

