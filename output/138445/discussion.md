# [OpenMP] clang++: /root/llvm-project/llvm/include/llvm/ADT/APSInt.h:99: int64_t llvm::APSInt::getExtValue() const: Assertion `isRepresentableByInt64() && "Too many bits for int64_t"' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/138445
**Status:** Closed
**Labels:** good first issue, clang:frontend, crash, clang:openmp
**Closed Date:** 2025-05-12T19:34:52Z

## Body

Reproducible on Godbolt:
https://godbolt.org/z/477nWWrb4
```cpp
void f(void) {
#pragma omp simd collapse(0xFFFFFFFFFFFFFFFF)
  for (int i = 0; i < 10; i++)
    ;
}
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-openmp

Author: None (k-arrows)

<details>
Reproducible on Godbolt:
https://godbolt.org/z/477nWWrb4
```cpp
void f(void) {
#pragma omp simd collapse(0xFFFFFFFFFFFFFFFF)
  for (int i = 0; i &lt; 10; i++)
    ;
}
```
</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (k-arrows)

<details>
Reproducible on Godbolt:
https://godbolt.org/z/477nWWrb4
```cpp
void f(void) {
#pragma omp simd collapse(0xFFFFFFFFFFFFFFFF)
  for (int i = 0; i &lt; 10; i++)
    ;
}
```
</details>


---

### Comment 3 - llvmbot


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

### Comment 4 - llvmbot


@llvm/issue-subscribers-good-first-issue

Author: None (k-arrows)

<details>
Reproducible on Godbolt:
https://godbolt.org/z/477nWWrb4
```cpp
void f(void) {
#pragma omp simd collapse(0xFFFFFFFFFFFFFFFF)
  for (int i = 0; i &lt; 10; i++)
    ;
}
```
</details>


---

### Comment 5 - AmrDeveloper

Interested to work on it

---

### Comment 6 - shafik

Adding backtrace for better searchability:

```console
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -fopenmp <source>
1.	<source>:2:46: at annotation token
2.	<source>:1:14: parsing function body 'f'
3.	<source>:1:14: in compound statement ('{}')
 #0 0x0000000003f7c0a8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f7c0a8)
 #1 0x0000000003f79d34 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f79d34)
 #2 0x0000000003ebebf8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x0000749cbf842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x0000749cbf8969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x0000749cbf842476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x0000749cbf8287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x0000749cbf82871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x0000749cbf839e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x00000000043c7e23 llvm::APSInt::getExtValue() const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x43c7e23)
#10 0x0000000006e11ad8 clang::SemaOpenMP::VerifyPositiveIntegerConstantInClause(clang::Expr*, llvm::omp::Clause, bool, bool) (.part.0) SemaOpenMP.cpp:0:0
#11 0x0000000006e11f7e clang::SemaOpenMP::ActOnOpenMPCollapseClause(clang::Expr*, clang::SourceLocation, clang::SourceLocation, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e11f7e)
#12 0x00000000066b9673 clang::Parser::ParseOpenMPSingleExprClause(llvm::omp::Clause, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66b9673)
#13 0x00000000066c0c57 clang::Parser::ParseOpenMPClause(llvm::omp::Directive, llvm::omp::Clause, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66c0c57)
#14 0x00000000066c25e3 clang::Parser::ParseOpenMPExecutableDirective(clang::Parser::ParsedStmtContext, llvm::omp::Directive, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66c25e3)
#15 0x00000000066d1e65 clang::Parser::ParseOpenMPDeclarativeOrExecutableDirective(clang::Parser::ParsedStmtContext, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66d1e65)
#16 0x00000000066f0122 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66f0122)
#17 0x00000000066f1fcd clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66f1fcd)
#18 0x00000000066f9ce3 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66f9ce3)
#19 0x00000000066fa4aa clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66fa4aa)
#20 0x0000000006601a53 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6601a53)
#21 0x00000000066380fd clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66380fd)
#22 0x00000000065f558e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65f558e)
#23 0x00000000065f5d49 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65f5d49)
#24 0x00000000065fd66a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65fd66a)
#25 0x00000000065fe60d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65fe60d)
#26 0x00000000065feac0 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65feac0)
#27 0x00000000065f09b3 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65f09b3)
#28 0x000000000491ebe8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x491ebe8)
#29 0x0000000004c11f15 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c11f15)
#30 0x0000000004b9131e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b9131e)
#31 0x0000000004d05d39 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d05d39)
#32 0x0000000000da788f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda788f)
#33 0x0000000000d9da5a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#34 0x00000000049854a9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#35 0x0000000003ebf094 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3ebf094)
#36 0x0000000004985abf clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#37 0x0000000004947f0d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4947f0d)
#38 0x0000000004948f9e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4948f9e)
#39 0x0000000004950df5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4950df5)
#40 0x0000000000da37f8 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda37f8)
#41 0x0000000000c28b74 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc28b74)
#42 0x0000749cbf829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#43 0x0000749cbf829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#44 0x0000000000d9d505 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd9d505)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

---

