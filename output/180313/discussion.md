# [clang][C23] crashes when initializing constexpr int* with floating-point

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/180313
**Status:** Closed
**Labels:** good first issue, c23, clang:frontend, crash-on-invalid, confirmed, regression:19
**Closed Date:** 2026-02-17T14:02:21Z

## Body

Reproducer:
https://godbolt.org/z/34r8Whsx4
```c
constexpr int *foo = 0.;
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/AST/APValue.cpp:1020: bool clang::APValue::isNullPointer() const: Assertion `isLValue() && "Invalid usage"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x c -std=c23 <source>
1.	<source>:1:24: current parser token ';'
 #0 0x000000000435ba08 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x435ba08)
 #1 0x0000000004358e64 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4358e64)
 #2 0x00000000042991d8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007b4b7e842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007b4b7e8969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007b4b7e842476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007b4b7e8287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007b4b7e82871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007b4b7e839e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000007a1b3f3 (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7a1b3f3)
#10 0x0000000007a1f3b3 (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7a1f3b3)
#11 0x00000000071c1b3a clang::InitializationSequence::Perform(clang::Sema&, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::MutableArrayRef<clang::Expr*>, clang::QualType*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x71c1b3a)
#12 0x0000000006ee5545 clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ee5545)
#13 0x0000000006b30041 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b30041)
#14 0x0000000006b3d2aa clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b3d2aa)
#15 0x0000000006afa8db clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6afa8db)
#16 0x0000000006afb0cf clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6afb0cf)
#17 0x0000000006b030d1 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b030d1)
#18 0x0000000006b03fe5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b03fe5)
#19 0x0000000006b043d0 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b043d0)
#20 0x0000000006ae4cc3 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ae4cc3)
#21 0x0000000004cd2d18 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4cd2d18)
#22 0x0000000004fcfef5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4fcfef5)
#23 0x0000000004f4f72e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f4f72e)
#24 0x00000000050cb2cd clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x50cb2cd)
#25 0x0000000000de429e cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde429e)
#26 0x0000000000ddac6a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#27 0x0000000000ddaded int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#28 0x0000000004d46929 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#29 0x0000000004299674 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4299674)
#30 0x0000000004d46f3f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#31 0x0000000004d07732 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d07732)
#32 0x0000000004d086de clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d086de)
#33 0x0000000004d0fb25 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d0fb25)
#34 0x0000000000de0611 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde0611)
#35 0x0000000000c9b9c4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc9b9c4)
#36 0x00007b4b7e829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#37 0x00007b4b7e829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#38 0x0000000000dda705 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdda705)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/34r8Whsx4
```c
constexpr int *foo = 0.;
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/AST/APValue.cpp:1020: bool clang::APValue::isNullPointer() const: Assertion `isLValue() &amp;&amp; "Invalid usage"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x c -std=c23 &lt;source&gt;
1.	&lt;source&gt;:1:24: current parser token ';'
 #<!-- -->0 0x000000000435ba08 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x435ba08)
 #<!-- -->1 0x0000000004358e64 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4358e64)
 #<!-- -->2 0x00000000042991d8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007b4b7e842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007b4b7e8969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007b4b7e842476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007b4b7e8287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x00007b4b7e82871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x00007b4b7e839e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000007a1b3f3 (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7a1b3f3)
#<!-- -->10 0x0000000007a1f3b3 (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7a1f3b3)
#<!-- -->11 0x00000000071c1b3a clang::InitializationSequence::Perform(clang::Sema&amp;, clang::InitializedEntity const&amp;, clang::InitializationKind const&amp;, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::QualType*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x71c1b3a)
#<!-- -->12 0x0000000006ee5545 clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ee5545)
#<!-- -->13 0x0000000006b30041 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b30041)
#<!-- -->14 0x0000000006b3d2aa clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b3d2aa)
#<!-- -->15 0x0000000006afa8db clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6afa8db)
#<!-- -->16 0x0000000006afb0cf clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6afb0cf)
#<!-- -->17 0x0000000006b030d1 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b030d1)
#<!-- -->18 0x0000000006b03fe5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b03fe5)
#<!-- -->19 0x0000000006b043d0 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b043d0)
#<!-- -->20 0x0000000006ae4cc3 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ae4cc3)
#<!-- -->21 0x0000000004cd2d18 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4cd2d18)
#<!-- -->22 0x0000000004fcfef5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4fcfef5)
#<!-- -->23 0x0000000004f4f72e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f4f72e)
#<!-- -->24 0x00000000050cb2cd clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x50cb2cd)
#<!-- -->25 0x0000000000de429e cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde429e)
#<!-- -->26 0x0000000000ddac6a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->27 0x0000000000ddaded int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->28 0x0000000004d46929 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->29 0x0000000004299674 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4299674)
#<!-- -->30 0x0000000004d46f3f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->31 0x0000000004d07732 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d07732)
#<!-- -->32 0x0000000004d086de clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d086de)
#<!-- -->33 0x0000000004d0fb25 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d0fb25)
#<!-- -->34 0x0000000000de0611 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde0611)
#<!-- -->35 0x0000000000c9b9c4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc9b9c4)
#<!-- -->36 0x00007b4b7e829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->37 0x00007b4b7e829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->38 0x0000000000dda705 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdda705)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

### Comment 2 - k-arrows

Goes back to clang-19:
https://godbolt.org/z/c8r9v37YM

---

### Comment 3 - llvmbot


Hi!

This issue may be a good introductory issue for people new to working on LLVM. If you would like to work on this issue, your first steps are:

1. Check that no other contributor is working on this issue. If someone is assigned to the issue or claimed to be working on it, ping the person. After one week without a response, the assignee may be changed.
1. Leave a comment indicating that you are working on the issue, or just create a [pull request](https://github.com/llvm/llvm-project/pulls) after following the steps below. [Mention](https://docs.github.com/en/issues/tracking-your-work-with-issues/linking-a-pull-request-to-an-issue) this issue in the description of the pull request.
1. Fix the issue locally.
1. [Run the test suite](https://llvm.org/docs/TestingGuide.html#unit-and-regression-tests) locally. Remember that the subdirectories under `test/` create fine-grained testing targets, so you can e.g. use `make check-clang-ast` to only run Clang's AST tests.
1. Create a Git commit.
1. Run [`git clang-format HEAD~1`](https://clang.llvm.org/docs/ClangFormat.html#git-integration) to format your changes.
1. Open a [pull request](https://github.com/llvm/llvm-project/pulls) to the [upstream repository](https://github.com/llvm/llvm-project) on GitHub. Detailed instructions can be found [in GitHub's documentation](https://docs.github.com/en/pull-requests/collaborating-with-pull-requests/proposing-changes-to-your-work-with-pull-requests/creating-a-pull-request). [Mention](https://docs.github.com/en/issues/tracking-your-work-with-issues/linking-a-pull-request-to-an-issue) this issue in the description of the pull request.

If you have any further questions about this issue, don't hesitate to ask via a comment in the thread below.



---

### Comment 4 - llvmbot


@llvm/issue-subscribers-good-first-issue

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/34r8Whsx4
```c
constexpr int *foo = 0.;
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/AST/APValue.cpp:1020: bool clang::APValue::isNullPointer() const: Assertion `isLValue() &amp;&amp; "Invalid usage"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x c -std=c23 &lt;source&gt;
1.	&lt;source&gt;:1:24: current parser token ';'
 #<!-- -->0 0x000000000435ba08 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x435ba08)
 #<!-- -->1 0x0000000004358e64 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4358e64)
 #<!-- -->2 0x00000000042991d8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007b4b7e842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007b4b7e8969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007b4b7e842476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007b4b7e8287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x00007b4b7e82871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x00007b4b7e839e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000007a1b3f3 (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7a1b3f3)
#<!-- -->10 0x0000000007a1f3b3 (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7a1f3b3)
#<!-- -->11 0x00000000071c1b3a clang::InitializationSequence::Perform(clang::Sema&amp;, clang::InitializedEntity const&amp;, clang::InitializationKind const&amp;, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::QualType*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x71c1b3a)
#<!-- -->12 0x0000000006ee5545 clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ee5545)
#<!-- -->13 0x0000000006b30041 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b30041)
#<!-- -->14 0x0000000006b3d2aa clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b3d2aa)
#<!-- -->15 0x0000000006afa8db clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6afa8db)
#<!-- -->16 0x0000000006afb0cf clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6afb0cf)
#<!-- -->17 0x0000000006b030d1 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b030d1)
#<!-- -->18 0x0000000006b03fe5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b03fe5)
#<!-- -->19 0x0000000006b043d0 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b043d0)
#<!-- -->20 0x0000000006ae4cc3 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ae4cc3)
#<!-- -->21 0x0000000004cd2d18 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4cd2d18)
#<!-- -->22 0x0000000004fcfef5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4fcfef5)
#<!-- -->23 0x0000000004f4f72e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f4f72e)
#<!-- -->24 0x00000000050cb2cd clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x50cb2cd)
#<!-- -->25 0x0000000000de429e cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde429e)
#<!-- -->26 0x0000000000ddac6a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->27 0x0000000000ddaded int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->28 0x0000000004d46929 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->29 0x0000000004299674 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4299674)
#<!-- -->30 0x0000000004d46f3f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->31 0x0000000004d07732 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d07732)
#<!-- -->32 0x0000000004d086de clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d086de)
#<!-- -->33 0x0000000004d0fb25 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d0fb25)
#<!-- -->34 0x0000000000de0611 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde0611)
#<!-- -->35 0x0000000000c9b9c4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc9b9c4)
#<!-- -->36 0x00007b4b7e829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->37 0x00007b4b7e829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->38 0x0000000000dda705 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdda705)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

### Comment 5 - samrudhnelli

Hey @tbaederr i would like to work on this, could you assign this to me..

---

