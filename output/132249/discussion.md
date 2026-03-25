# [clang] clang frontend crash with friend class declaration and overloaded `operator ==`

**Author:** Rush10233
**URL:** https://github.com/llvm/llvm-project/issues/132249
**Status:** Closed
**Labels:** good first issue, clang:frontend, confirmed, crash
**Closed Date:** 2025-04-03T01:11:28Z

## Body

The following code causes clang to crash:

```
struct foo {
};

struct bar {
    friend bool operator==(const bar& a, const bar& b);
    friend class foo;
};

bool operator==(const bar& a, const bar& b) = default;
```

[https://godbolt.org/z/Wv4scnaTG](https://godbolt.org/z/Wv4scnaTG)

Stack Dump:

```
<source>:9:47: warning: defaulted comparison operators are a C++20 extension [-Wc++20-extensions]
    9 | bool operator==(const bar& a, const bar& b) = default;
      |                                               ^
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<eof> parser at end of file
 #0 0x0000000003b7abe8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3b7abe8)
 #1 0x0000000003b78d2c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3b78d2c)
 #2 0x0000000003ac85a8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007e2311642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x0000000000c167b0 clang::Sema::CheckExplicitlyDefaultedComparison(clang::Scope*, clang::FunctionDecl*, clang::Sema::DefaultedComparisonKind) (.cold) SemaDeclCXX.cpp:0:0
 #5 0x00000000065642e9 clang::Sema::SetDeclDefaulted(clang::Decl*, clang::SourceLocation) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x65642e9)
 #6 0x00000000060e2751 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x60e2751)
 #7 0x000000000612cfdf clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x612cfdf)
 #8 0x00000000060da427 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x60da427)
 #9 0x00000000060db23e clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x60db23e)
#10 0x00000000060e4f3a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x60e4f3a)
#11 0x00000000060e67d8 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x60e67d8)
#12 0x00000000060d4b5a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x60d4b5a)
#13 0x00000000044f8d8d clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x44f8d8d)
#14 0x00000000047ddc31 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x47ddc31)
#15 0x000000000475c1bb clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x475c1bb)
#16 0x00000000048cddb3 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x48cddb3)
#17 0x0000000000d65be1 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xd65be1)
#18 0x0000000000d5e3dd ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#19 0x0000000004546089 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#20 0x0000000003ac89c3 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3ac89c3)
#21 0x00000000045462a9 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#22 0x00000000045095cd clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45095cd)
#23 0x000000000450a591 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x450a591)
#24 0x000000000451492c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x451492c)
#25 0x0000000000d629b1 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xd629b1)
#26 0x0000000000c23264 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc23264)
#27 0x00007e2311629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#28 0x00007e2311629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#29 0x0000000000d5de75 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xd5de75)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (Rush10233)

<details>
The following code causes clang to crash:

```
struct foo {
};

struct bar {
    friend bool operator==(const bar&amp; a, const bar&amp; b);
    friend class foo;
};

bool operator==(const bar&amp; a, const bar&amp; b) = default;
```

[https://godbolt.org/z/Wv4scnaTG](https://godbolt.org/z/Wv4scnaTG)

Stack Dump:

```
&lt;source&gt;:9:47: warning: defaulted comparison operators are a C++20 extension [-Wc++20-extensions]
    9 | bool operator==(const bar&amp; a, const bar&amp; b) = default;
      |                                               ^
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
 #<!-- -->0 0x0000000003b7abe8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3b7abe8)
 #<!-- -->1 0x0000000003b78d2c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3b78d2c)
 #<!-- -->2 0x0000000003ac85a8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007e2311642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x0000000000c167b0 clang::Sema::CheckExplicitlyDefaultedComparison(clang::Scope*, clang::FunctionDecl*, clang::Sema::DefaultedComparisonKind) (.cold) SemaDeclCXX.cpp:0:0
 #<!-- -->5 0x00000000065642e9 clang::Sema::SetDeclDefaulted(clang::Decl*, clang::SourceLocation) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x65642e9)
 #<!-- -->6 0x00000000060e2751 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x60e2751)
 #<!-- -->7 0x000000000612cfdf clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x612cfdf)
 #<!-- -->8 0x00000000060da427 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x60da427)
 #<!-- -->9 0x00000000060db23e clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x60db23e)
#<!-- -->10 0x00000000060e4f3a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x60e4f3a)
#<!-- -->11 0x00000000060e67d8 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x60e67d8)
#<!-- -->12 0x00000000060d4b5a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x60d4b5a)
#<!-- -->13 0x00000000044f8d8d clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x44f8d8d)
#<!-- -->14 0x00000000047ddc31 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x47ddc31)
#<!-- -->15 0x000000000475c1bb clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x475c1bb)
#<!-- -->16 0x00000000048cddb3 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x48cddb3)
#<!-- -->17 0x0000000000d65be1 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xd65be1)
#<!-- -->18 0x0000000000d5e3dd ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->19 0x0000000004546089 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->20 0x0000000003ac89c3 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3ac89c3)
#<!-- -->21 0x00000000045462a9 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->22 0x00000000045095cd clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45095cd)
#<!-- -->23 0x000000000450a591 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x450a591)
#<!-- -->24 0x000000000451492c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x451492c)
#<!-- -->25 0x0000000000d629b1 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xd629b1)
#<!-- -->26 0x0000000000c23264 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc23264)
#<!-- -->27 0x00007e2311629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->28 0x00007e2311629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->29 0x0000000000d5de75 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xd5de75)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```
</details>


---

### Comment 2 - Rush10233

Please note that this one goes back to clang-14:

[https://godbolt.org/z/xvoa37dvG](https://godbolt.org/z/xvoa37dvG)

---

### Comment 3 - MagentaTreehouse

Debugger says: "`clang::FriendDecl::getFriendDecl(...)` returned nullptr."
https://github.com/llvm/llvm-project/blob/49f06075a6d298bd13564c9bffcf51281bed4962/clang/lib/Sema/SemaDeclCXX.cpp#L9039-L9043


---

### Comment 4 - shafik

> Debugger says: "`clang::FriendDecl::getFriendDecl(...)` returned nullptr."

Sounds like a straight forward addition of check should fix this.

---

### Comment 5 - llvmbot


Hi!

This issue may be a good introductory issue for people new to working on LLVM. If you would like to work on this issue, your first steps are:

1. Check that no other contributor has already been assigned to this issue. If you believe that no one is actually working on it despite an assignment, ping the person. After one week without a response, the assignee may be changed.
1. In the comments of this issue, request for it to be assigned to you, or just create a [pull request](https://github.com/llvm/llvm-project/pulls) after following the steps below. [Mention](https://docs.github.com/en/issues/tracking-your-work-with-issues/linking-a-pull-request-to-an-issue) this issue in the description of the pull request.
1. Fix the issue locally.
1. [Run the test suite](https://llvm.org/docs/TestingGuide.html#unit-and-regression-tests) locally. Remember that the subdirectories under `test/` create fine-grained testing targets, so you can e.g. use `make check-clang-ast` to only run Clang's AST tests.
1. Create a Git commit.
1. Run [`git clang-format HEAD~1`](https://clang.llvm.org/docs/ClangFormat.html#git-integration) to format your changes.
1. Open a [pull request](https://github.com/llvm/llvm-project/pulls) to the [upstream repository](https://github.com/llvm/llvm-project) on GitHub. Detailed instructions can be found [in GitHub's documentation](https://docs.github.com/en/pull-requests/collaborating-with-pull-requests/proposing-changes-to-your-work-with-pull-requests/creating-a-pull-request). [Mention](https://docs.github.com/en/issues/tracking-your-work-with-issues/linking-a-pull-request-to-an-issue) this issue in the description of the pull request.

If you have any further questions about this issue, don't hesitate to ask via a comment in the thread below.



---

### Comment 6 - llvmbot


@llvm/issue-subscribers-good-first-issue

Author: None (Rush10233)

<details>
The following code causes clang to crash:

```
struct foo {
};

struct bar {
    friend bool operator==(const bar&amp; a, const bar&amp; b);
    friend class foo;
};

bool operator==(const bar&amp; a, const bar&amp; b) = default;
```

[https://godbolt.org/z/Wv4scnaTG](https://godbolt.org/z/Wv4scnaTG)

Stack Dump:

```
&lt;source&gt;:9:47: warning: defaulted comparison operators are a C++20 extension [-Wc++20-extensions]
    9 | bool operator==(const bar&amp; a, const bar&amp; b) = default;
      |                                               ^
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
 #<!-- -->0 0x0000000003b7abe8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3b7abe8)
 #<!-- -->1 0x0000000003b78d2c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3b78d2c)
 #<!-- -->2 0x0000000003ac85a8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007e2311642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x0000000000c167b0 clang::Sema::CheckExplicitlyDefaultedComparison(clang::Scope*, clang::FunctionDecl*, clang::Sema::DefaultedComparisonKind) (.cold) SemaDeclCXX.cpp:0:0
 #<!-- -->5 0x00000000065642e9 clang::Sema::SetDeclDefaulted(clang::Decl*, clang::SourceLocation) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x65642e9)
 #<!-- -->6 0x00000000060e2751 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x60e2751)
 #<!-- -->7 0x000000000612cfdf clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x612cfdf)
 #<!-- -->8 0x00000000060da427 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x60da427)
 #<!-- -->9 0x00000000060db23e clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x60db23e)
#<!-- -->10 0x00000000060e4f3a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x60e4f3a)
#<!-- -->11 0x00000000060e67d8 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x60e67d8)
#<!-- -->12 0x00000000060d4b5a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x60d4b5a)
#<!-- -->13 0x00000000044f8d8d clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x44f8d8d)
#<!-- -->14 0x00000000047ddc31 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x47ddc31)
#<!-- -->15 0x000000000475c1bb clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x475c1bb)
#<!-- -->16 0x00000000048cddb3 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x48cddb3)
#<!-- -->17 0x0000000000d65be1 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xd65be1)
#<!-- -->18 0x0000000000d5e3dd ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->19 0x0000000004546089 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->20 0x0000000003ac89c3 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3ac89c3)
#<!-- -->21 0x00000000045462a9 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->22 0x00000000045095cd clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45095cd)
#<!-- -->23 0x000000000450a591 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x450a591)
#<!-- -->24 0x000000000451492c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x451492c)
#<!-- -->25 0x0000000000d629b1 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xd629b1)
#<!-- -->26 0x0000000000c23264 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc23264)
#<!-- -->27 0x00007e2311629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->28 0x00007e2311629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->29 0x0000000000d5de75 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xd5de75)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```
</details>


---

### Comment 7 - BruceAko

I would like to work on this issue :)
Could you please assign it to me? @shafik 

---

### Comment 8 - shafik

@BruceAko we are trying out a new approach to assigning issues. We would like to see a PR first and then we will assign the issue.

In the meantime, if you have any questions let us know.

---

### Comment 9 - BruceAko

> [@BruceAko](https://github.com/BruceAko) we are trying out a new approach to assigning issues. We would like to see a PR first and then we will assign the issue.
> 
> In the meantime, if you have any questions let us know.

Understood! I'll start working on the PR and reach out if any questions come up. Thanks!

---

