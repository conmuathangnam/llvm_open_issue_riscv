# Crash assertion failure when using __typeof with __attribute__((noreturn)) and __asm__ alias in C2X mode

**Author:** AbyssStaror
**URL:** https://github.com/llvm/llvm-project/issues/173598

## Body

### Link for quick verification: https://godbolt.org/z/f1nh3xob9
```c
#include <string.h>
__typeof (memcmp) memcmp __asm__ ("memory_compare") __attribute__((noreturn));
```
Go back to `clang 15.0.0 (assertions)`. And this issue is specifically reproducible when using -std=c2x.
Compiler crashes with an assertion failure in `SemaDecl.cpp` when a function is declared using `__typeof` combined with an `__asm__` label and a `noreturn` attribute.

## Stack dump
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/Sema/SemaDecl.cpp:9538: clang::FunctionDecl* CreateNewFunctionDecl(clang::Sema&, clang::Declarator&, clang::DeclContext*, clang::QualType&, clang::TypeSourceInfo*, clang::StorageClass, bool&): Assertion `(HasPrototype || !SemaRef.getLangOpts().requiresStrictPrototypes()) && "Strict prototypes are required"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x c -std=c2x <source>
1.	<source>:2:78: current parser token ';'
 #0 0x00000000042ecab8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42ecab8)
 #1 0x00000000042e9ee4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42e9ee4)
 #2 0x000000000422ec88 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000070a427042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x000070a4270969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x000070a427042476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x000070a4270287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x000070a42702871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x000070a427039e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000006e169d8 clang::Sema::ActOnFunctionDeclarator(clang::Scope*, clang::Declarator&, clang::DeclContext*, clang::TypeSourceInfo*, clang::LookupResult&, llvm::MutableArrayRef<clang::TemplateParameterList*>, bool&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e169d8)
#10 0x0000000006e1b5a1 clang::Sema::HandleDeclarator(clang::Scope*, clang::Declarator&, llvm::MutableArrayRef<clang::TemplateParameterList*>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e1b5a1)
#11 0x0000000006e1c4a0 clang::Sema::ActOnDeclarator(clang::Scope*, clang::Declarator&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e1c4a0)
#12 0x0000000006a532c3 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a532c3)
#13 0x0000000006a610da clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a610da)
#14 0x0000000006a1e78b clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a1e78b)
#15 0x0000000006a1ef7f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a1ef7f)
#16 0x0000000006a26daa clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a26daa)
#17 0x0000000006a27d45 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a27d45)
#18 0x0000000006a08dca clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a08dca)
#19 0x0000000004c4dde8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c4dde8)
#20 0x0000000004f446e5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f446e5)
#21 0x0000000004ec497e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ec497e)
#22 0x000000000503ea6d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x503ea6d)
#23 0x0000000000de5b4c cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde5b4c)
#24 0x0000000000ddc59a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#25 0x0000000000ddc71d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#26 0x0000000004cc0739 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#27 0x000000000422f124 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x422f124)
#28 0x0000000004cc0d4f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#29 0x0000000004c81832 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c81832)
#30 0x0000000004c827de clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c827de)
#31 0x0000000004c89c35 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c89c35)
#32 0x0000000000de1f41 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde1f41)
#33 0x0000000000c8cf94 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc8cf94)
#34 0x000070a427029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#35 0x000070a427029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#36 0x0000000000ddc035 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xddc035)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (AbyssStaror)

<details>
### Link for quick verification: https://godbolt.org/z/f1nh3xob9
```c
#include &lt;string.h&gt;
__typeof (memcmp) memcmp __asm__ ("memory_compare") __attribute__((noreturn));
```
Go back to `clang 15.0.0 (assertions)`. And this issue is specifically reproducible when using -std=c2x.
Compiler crashes with an assertion failure in `SemaDecl.cpp` when a function is declared using `__typeof` combined with an `__asm__` label and a `noreturn` attribute.

## Stack dump
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/Sema/SemaDecl.cpp:9538: clang::FunctionDecl* CreateNewFunctionDecl(clang::Sema&amp;, clang::Declarator&amp;, clang::DeclContext*, clang::QualType&amp;, clang::TypeSourceInfo*, clang::StorageClass, bool&amp;): Assertion `(HasPrototype || !SemaRef.getLangOpts().requiresStrictPrototypes()) &amp;&amp; "Strict prototypes are required"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x c -std=c2x &lt;source&gt;
1.	&lt;source&gt;:2:78: current parser token ';'
 #<!-- -->0 0x00000000042ecab8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42ecab8)
 #<!-- -->1 0x00000000042e9ee4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42e9ee4)
 #<!-- -->2 0x000000000422ec88 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x000070a427042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x000070a4270969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x000070a427042476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x000070a4270287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x000070a42702871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x000070a427039e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000006e169d8 clang::Sema::ActOnFunctionDeclarator(clang::Scope*, clang::Declarator&amp;, clang::DeclContext*, clang::TypeSourceInfo*, clang::LookupResult&amp;, llvm::MutableArrayRef&lt;clang::TemplateParameterList*&gt;, bool&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e169d8)
#<!-- -->10 0x0000000006e1b5a1 clang::Sema::HandleDeclarator(clang::Scope*, clang::Declarator&amp;, llvm::MutableArrayRef&lt;clang::TemplateParameterList*&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e1b5a1)
#<!-- -->11 0x0000000006e1c4a0 clang::Sema::ActOnDeclarator(clang::Scope*, clang::Declarator&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e1c4a0)
#<!-- -->12 0x0000000006a532c3 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a532c3)
#<!-- -->13 0x0000000006a610da clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a610da)
#<!-- -->14 0x0000000006a1e78b clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a1e78b)
#<!-- -->15 0x0000000006a1ef7f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a1ef7f)
#<!-- -->16 0x0000000006a26daa clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a26daa)
#<!-- -->17 0x0000000006a27d45 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a27d45)
#<!-- -->18 0x0000000006a08dca clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a08dca)
#<!-- -->19 0x0000000004c4dde8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c4dde8)
#<!-- -->20 0x0000000004f446e5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f446e5)
#<!-- -->21 0x0000000004ec497e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ec497e)
#<!-- -->22 0x000000000503ea6d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x503ea6d)
#<!-- -->23 0x0000000000de5b4c cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde5b4c)
#<!-- -->24 0x0000000000ddc59a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->25 0x0000000000ddc71d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->26 0x0000000004cc0739 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->27 0x000000000422f124 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x422f124)
#<!-- -->28 0x0000000004cc0d4f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->29 0x0000000004c81832 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c81832)
#<!-- -->30 0x0000000004c827de clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c827de)
#<!-- -->31 0x0000000004c89c35 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c89c35)
#<!-- -->32 0x0000000000de1f41 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde1f41)
#<!-- -->33 0x0000000000c8cf94 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc8cf94)
#<!-- -->34 0x000070a427029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->35 0x000070a427029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->36 0x0000000000ddc035 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xddc035)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

### Comment 2 - DhanashreePetare

Greetings @AbyssStaror  , I am interested to work on the issue. Can you please assign it to me. Thankyou.



---

### Comment 3 - shafik

> Greetings [@AbyssStaror](https://github.com/AbyssStaror) , I am interested to work on the issue. Can you please assign it to me. Thankyou.

We don't assign issues until after a PR is created and we think the PR is on the right track. Considering it is a regression waiting until a git bisect is done would be sensible usually.

---

### Comment 4 - DhanashreePetare

Okay @shafik, I will try to implement and create a PR for the same.

---

### Comment 5 - DhanashreePetare

@AbyssStaror I have tried implementing it. Please review it and let me know about any further changes if required.

---

### Comment 6 - AbyssStaror

@DhanashreePetare Thanks for the fix!
For LLVM, changes need to be submitted as a PR against the upstream llvm/llvm-project repository so that they can go through review and CI.
Could you please open a PR there using this commit?

---

### Comment 7 - DhanashreePetare

@AbyssStaror, the PR is already with the upstream llvm/llvm-project repo and the PR went through CI checks as well. Still I tried again just to be sure and got the same thing that the PR already exist. Am I missing on something, if so..sorry for that, can you please explain if I am wrong anywhere.

---

### Comment 8 - shafik

I am not sure why the PR does not show up properly in the issue but it is here: https://github.com/llvm/llvm-project/pull/174092

---

### Comment 9 - DhanashreePetare

Yeah @shafik, I went through your review and will be implementing it. 
Quick doubts: 
1. You suggested 2 ways (A) add the minimal isNull() guard only, or (B) refactor into a small helper that examines [D] and [R], I should go with which option, according to me the 2nd one promises clarity. 
2. I saw you requested review from other reviewers, should I wait for their reviews as well and then implement everything altogether or implement the above suggestion by you firstly?

---

