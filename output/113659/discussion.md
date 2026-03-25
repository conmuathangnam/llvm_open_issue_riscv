# [clang] Crash in class template arguments deduction for aggregates

**Author:** Timothy-Liuxf
**URL:** https://github.com/llvm/llvm-project/issues/113659
**Status:** Closed
**Labels:** good first issue, clang:frontend, crash
**Closed Date:** 2024-11-29T19:27:00Z

## Body

The following code:

```cpp
#include <tuple>
#include <string>
#include <any>

template <class... Args>
struct B : std::tuple<Args...>, Args... {};

B b{std::tuple<std::any, std::string>{}};

```

crashes when compiled with Clang 19.1.0 (`clang++ -std=c++20`):

```bash
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-19.1.0/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-14.2.0 -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 <source>
1.	<source>:8:41: current parser token ';'
 #0 0x00000000036fdb08 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x36fdb08)
 #1 0x00000000036fbc74 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x36fbc74)
 #2 0x0000000003647be8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007a0a79c42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x0000000006a8bbbc checkDeducedTemplateArguments(clang::ASTContext&, clang::DeducedTemplateArgument const&, clang::DeducedTemplateArgument const&, bool) (.part.0) SemaTemplateDeduction.cpp:0:0
 #5 0x0000000006a8d57c (anonymous namespace)::PackDeductionScope::finish() SemaTemplateDeduction.cpp:0:0
 #6 0x0000000006af1e55 clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, bool, clang::QualType, clang::Expr::Classification, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>)>) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x6af1e55)
 #7 0x00000000068eef0d clang::Sema::AddTemplateOverloadCandidate(clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::OverloadCandidateSet&, bool, bool, bool, clang::CallExpr::ADLCallKind, clang::OverloadCandidateParamOrder, bool) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x68eef0d)
 #8 0x000000000670cb8a clang::Sema::DeduceTemplateSpecializationFromInitializer(clang::TypeSourceInfo*, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::MutableArrayRef<clang::Expr*>)::'lambda0'(bool)::operator()(bool) const::'lambda'(clang::InitListExpr*)::operator()(clang::InitListExpr*) const SemaInit.cpp:0:0
 #9 0x000000000670d168 clang::Sema::DeduceTemplateSpecializationFromInitializer(clang::TypeSourceInfo*, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::MutableArrayRef<clang::Expr*>)::'lambda0'(bool)::operator()(bool) const SemaInit.cpp:0:0
#10 0x000000000670d82a clang::Sema::DeduceTemplateSpecializationFromInitializer(clang::TypeSourceInfo*, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::MutableArrayRef<clang::Expr*>) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x670d82a)
#11 0x0000000006294f18 clang::Sema::deduceVarTypeFromInitializer(clang::VarDecl*, clang::DeclarationName, clang::QualType, clang::TypeSourceInfo*, clang::SourceRange, bool, clang::Expr*) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x6294f18)
#12 0x00000000062afa2b clang::Sema::DeduceVariableDeclarationType(clang::VarDecl*, bool, clang::Expr*) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x62afa2b)
#13 0x00000000062d9fe4 clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x62d9fe4)
#14 0x0000000005f58d4c clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x5f58d4c)
#15 0x0000000005f6cbe4 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x5f6cbe4)
#16 0x0000000005f1e817 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x5f1e817)
#17 0x0000000005f1f703 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x5f1f703)
#18 0x0000000005f27cd2 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x5f27cd2)
#19 0x0000000005f29ac8 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x5f29ac8)
#20 0x0000000005f1877a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x5f1877a)
#21 0x0000000003fce825 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x3fce825)
#22 0x0000000004274201 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x4274201)
#23 0x00000000041f699b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x41f699b)
#24 0x000000000435a843 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x435a843)
#25 0x0000000000c874dc cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0xc874dc)
#26 0x0000000000c8146d ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#27 0x0000000004017309 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#28 0x0000000003648013 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x3648013)
#29 0x0000000004017529 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#30 0x0000000003fdd947 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x3fdd947)
#31 0x0000000003fde2fd clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x3fde2fd)
#32 0x0000000003fe7d4c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x3fe7d4c)
#33 0x0000000000c847f1 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0xc847f1)
#34 0x0000000000b5c2d4 main (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0xb5c2d4)
#35 0x00007a0a79c29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#36 0x00007a0a79c29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#37 0x0000000000c80f0e _start (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0xc80f0e)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```

which can be compiled with GCC 14.2.

View in Compiler Explorer:

<https://gcc.godbolt.org/z/1jca4WMEr>

## Comments

### Comment 1 - chrchr-github

Reduced:
~~~c++
template<class... Args>
struct S {};
struct T{};
struct U{};

template<class... Args>
struct B : S<Args...>, Args... {};

B b{S<T, U>{}};
~~~
https://gcc.godbolt.org/z/44sfTreYM

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Timothy Liu (Timothy-Liuxf)

<details>
The following code:

```cpp
#include &lt;tuple&gt;
#include &lt;string&gt;
#include &lt;any&gt;

template &lt;class... Args&gt;
struct B : std::tuple&lt;Args...&gt;, Args... {};

B b{std::tuple&lt;std::any, std::string&gt;{}};

```

crashes when compiled with Clang 19.1.0 (`clang++ -std=c++20`):

```bash
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-19.1.0/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-14.2.0 -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 &lt;source&gt;
1.	&lt;source&gt;:8:41: current parser token ';'
 #<!-- -->0 0x00000000036fdb08 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x36fdb08)
 #<!-- -->1 0x00000000036fbc74 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x36fbc74)
 #<!-- -->2 0x0000000003647be8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007a0a79c42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x0000000006a8bbbc checkDeducedTemplateArguments(clang::ASTContext&amp;, clang::DeducedTemplateArgument const&amp;, clang::DeducedTemplateArgument const&amp;, bool) (.part.0) SemaTemplateDeduction.cpp:0:0
 #<!-- -->5 0x0000000006a8d57c (anonymous namespace)::PackDeductionScope::finish() SemaTemplateDeduction.cpp:0:0
 #<!-- -->6 0x0000000006af1e55 clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, bool, bool, clang::QualType, clang::Expr::Classification, llvm::function_ref&lt;bool (llvm::ArrayRef&lt;clang::QualType&gt;)&gt;) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x6af1e55)
 #<!-- -->7 0x00000000068eef0d clang::Sema::AddTemplateOverloadCandidate(clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::OverloadCandidateSet&amp;, bool, bool, bool, clang::CallExpr::ADLCallKind, clang::OverloadCandidateParamOrder, bool) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x68eef0d)
 #<!-- -->8 0x000000000670cb8a clang::Sema::DeduceTemplateSpecializationFromInitializer(clang::TypeSourceInfo*, clang::InitializedEntity const&amp;, clang::InitializationKind const&amp;, llvm::MutableArrayRef&lt;clang::Expr*&gt;)::'lambda0'(bool)::operator()(bool) const::'lambda'(clang::InitListExpr*)::operator()(clang::InitListExpr*) const SemaInit.cpp:0:0
 #<!-- -->9 0x000000000670d168 clang::Sema::DeduceTemplateSpecializationFromInitializer(clang::TypeSourceInfo*, clang::InitializedEntity const&amp;, clang::InitializationKind const&amp;, llvm::MutableArrayRef&lt;clang::Expr*&gt;)::'lambda0'(bool)::operator()(bool) const SemaInit.cpp:0:0
#<!-- -->10 0x000000000670d82a clang::Sema::DeduceTemplateSpecializationFromInitializer(clang::TypeSourceInfo*, clang::InitializedEntity const&amp;, clang::InitializationKind const&amp;, llvm::MutableArrayRef&lt;clang::Expr*&gt;) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x670d82a)
#<!-- -->11 0x0000000006294f18 clang::Sema::deduceVarTypeFromInitializer(clang::VarDecl*, clang::DeclarationName, clang::QualType, clang::TypeSourceInfo*, clang::SourceRange, bool, clang::Expr*) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x6294f18)
#<!-- -->12 0x00000000062afa2b clang::Sema::DeduceVariableDeclarationType(clang::VarDecl*, bool, clang::Expr*) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x62afa2b)
#<!-- -->13 0x00000000062d9fe4 clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x62d9fe4)
#<!-- -->14 0x0000000005f58d4c clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x5f58d4c)
#<!-- -->15 0x0000000005f6cbe4 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x5f6cbe4)
#<!-- -->16 0x0000000005f1e817 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x5f1e817)
#<!-- -->17 0x0000000005f1f703 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x5f1f703)
#<!-- -->18 0x0000000005f27cd2 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x5f27cd2)
#<!-- -->19 0x0000000005f29ac8 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x5f29ac8)
#<!-- -->20 0x0000000005f1877a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x5f1877a)
#<!-- -->21 0x0000000003fce825 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x3fce825)
#<!-- -->22 0x0000000004274201 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x4274201)
#<!-- -->23 0x00000000041f699b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x41f699b)
#<!-- -->24 0x000000000435a843 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x435a843)
#<!-- -->25 0x0000000000c874dc cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0xc874dc)
#<!-- -->26 0x0000000000c8146d ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->27 0x0000000004017309 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->28 0x0000000003648013 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x3648013)
#<!-- -->29 0x0000000004017529 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->30 0x0000000003fdd947 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x3fdd947)
#<!-- -->31 0x0000000003fde2fd clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x3fde2fd)
#<!-- -->32 0x0000000003fe7d4c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x3fe7d4c)
#<!-- -->33 0x0000000000c847f1 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0xc847f1)
#<!-- -->34 0x0000000000b5c2d4 main (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0xb5c2d4)
#<!-- -->35 0x00007a0a79c29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->36 0x00007a0a79c29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->37 0x0000000000c80f0e _start (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0xc80f0e)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```

which can be compiled with GCC 14.2.

View in Compiler Explorer:

&lt;https://gcc.godbolt.org/z/1jca4WMEr&gt;
</details>


---

### Comment 3 - zyn0217

When performing a template argument deduction against the synthesized deduction guide `auto B(S<Args...>, Args...) -> B<Args...>`, we will exercise the code path for the empty trailing packs

https://github.com/llvm/llvm-project/blob/3a115279f888b6489902df60c3fe2e76447d375c/clang/lib/Sema/SemaTemplateDeduction.cpp#L1043-L1045

`TemplateArgument::getEmptyPack()` creates a `TemplateArgument` of kind `Pack`, where `Args.Args` is null and `Args.NumArgs` is 0.

Later in `checkDeducedTemplateArguments()` called by `PackDeductionScope::finish()`, we seem to overlook the fact that the value of `Y` (which is the empty NewPack) can be null and we will then be increasing a potential nullptr `YA`:

https://github.com/llvm/llvm-project/blob/3a115279f888b6489902df60c3fe2e76447d375c/clang/lib/Sema/SemaTemplateDeduction.cpp#L376-L389

This is wrong. We shouldn't bump `YA` unless we can ensure it's non-null.

---

### Comment 4 - llvmbot


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

### Comment 5 - llvmbot


@llvm/issue-subscribers-good-first-issue

Author: Timothy Liu (Timothy-Liuxf)

<details>
The following code:

```cpp
#include &lt;tuple&gt;
#include &lt;string&gt;
#include &lt;any&gt;

template &lt;class... Args&gt;
struct B : std::tuple&lt;Args...&gt;, Args... {};

B b{std::tuple&lt;std::any, std::string&gt;{}};

```

crashes when compiled with Clang 19.1.0 (`clang++ -std=c++20`):

```bash
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-19.1.0/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-14.2.0 -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 &lt;source&gt;
1.	&lt;source&gt;:8:41: current parser token ';'
 #<!-- -->0 0x00000000036fdb08 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x36fdb08)
 #<!-- -->1 0x00000000036fbc74 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x36fbc74)
 #<!-- -->2 0x0000000003647be8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007a0a79c42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x0000000006a8bbbc checkDeducedTemplateArguments(clang::ASTContext&amp;, clang::DeducedTemplateArgument const&amp;, clang::DeducedTemplateArgument const&amp;, bool) (.part.0) SemaTemplateDeduction.cpp:0:0
 #<!-- -->5 0x0000000006a8d57c (anonymous namespace)::PackDeductionScope::finish() SemaTemplateDeduction.cpp:0:0
 #<!-- -->6 0x0000000006af1e55 clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, bool, bool, clang::QualType, clang::Expr::Classification, llvm::function_ref&lt;bool (llvm::ArrayRef&lt;clang::QualType&gt;)&gt;) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x6af1e55)
 #<!-- -->7 0x00000000068eef0d clang::Sema::AddTemplateOverloadCandidate(clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::OverloadCandidateSet&amp;, bool, bool, bool, clang::CallExpr::ADLCallKind, clang::OverloadCandidateParamOrder, bool) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x68eef0d)
 #<!-- -->8 0x000000000670cb8a clang::Sema::DeduceTemplateSpecializationFromInitializer(clang::TypeSourceInfo*, clang::InitializedEntity const&amp;, clang::InitializationKind const&amp;, llvm::MutableArrayRef&lt;clang::Expr*&gt;)::'lambda0'(bool)::operator()(bool) const::'lambda'(clang::InitListExpr*)::operator()(clang::InitListExpr*) const SemaInit.cpp:0:0
 #<!-- -->9 0x000000000670d168 clang::Sema::DeduceTemplateSpecializationFromInitializer(clang::TypeSourceInfo*, clang::InitializedEntity const&amp;, clang::InitializationKind const&amp;, llvm::MutableArrayRef&lt;clang::Expr*&gt;)::'lambda0'(bool)::operator()(bool) const SemaInit.cpp:0:0
#<!-- -->10 0x000000000670d82a clang::Sema::DeduceTemplateSpecializationFromInitializer(clang::TypeSourceInfo*, clang::InitializedEntity const&amp;, clang::InitializationKind const&amp;, llvm::MutableArrayRef&lt;clang::Expr*&gt;) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x670d82a)
#<!-- -->11 0x0000000006294f18 clang::Sema::deduceVarTypeFromInitializer(clang::VarDecl*, clang::DeclarationName, clang::QualType, clang::TypeSourceInfo*, clang::SourceRange, bool, clang::Expr*) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x6294f18)
#<!-- -->12 0x00000000062afa2b clang::Sema::DeduceVariableDeclarationType(clang::VarDecl*, bool, clang::Expr*) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x62afa2b)
#<!-- -->13 0x00000000062d9fe4 clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x62d9fe4)
#<!-- -->14 0x0000000005f58d4c clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x5f58d4c)
#<!-- -->15 0x0000000005f6cbe4 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x5f6cbe4)
#<!-- -->16 0x0000000005f1e817 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x5f1e817)
#<!-- -->17 0x0000000005f1f703 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x5f1f703)
#<!-- -->18 0x0000000005f27cd2 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x5f27cd2)
#<!-- -->19 0x0000000005f29ac8 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x5f29ac8)
#<!-- -->20 0x0000000005f1877a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x5f1877a)
#<!-- -->21 0x0000000003fce825 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x3fce825)
#<!-- -->22 0x0000000004274201 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x4274201)
#<!-- -->23 0x00000000041f699b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x41f699b)
#<!-- -->24 0x000000000435a843 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x435a843)
#<!-- -->25 0x0000000000c874dc cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0xc874dc)
#<!-- -->26 0x0000000000c8146d ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->27 0x0000000004017309 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->28 0x0000000003648013 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x3648013)
#<!-- -->29 0x0000000004017529 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->30 0x0000000003fdd947 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x3fdd947)
#<!-- -->31 0x0000000003fde2fd clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x3fde2fd)
#<!-- -->32 0x0000000003fe7d4c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x3fe7d4c)
#<!-- -->33 0x0000000000c847f1 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0xc847f1)
#<!-- -->34 0x0000000000b5c2d4 main (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0xb5c2d4)
#<!-- -->35 0x00007a0a79c29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->36 0x00007a0a79c29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->37 0x0000000000c80f0e _start (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0xc80f0e)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```

which can be compiled with GCC 14.2.

View in Compiler Explorer:

&lt;https://gcc.godbolt.org/z/1jca4WMEr&gt;
</details>


---

