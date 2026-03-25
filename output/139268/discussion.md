# [OpenMP] Assertion `llvm::isUIntN(BitWidth, val) && "Value is not an N-bit unsigned value"' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/139268

## Body

Reproducible on Godbolt:
https://godbolt.org/z/xzM7oEf33
```cpp
void f(void) {
#pragma omp unroll partial(0xFFFFFFFFF)
  for (int i = 0; i < 10; i++)
    ;
}
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (k-arrows)

<details>
Reproducible on Godbolt:
https://godbolt.org/z/xzM7oEf33
```cpp
void f(void) {
#pragma omp unroll partial(0xFFFFFFFFF)
  for (int i = 0; i &lt; 10; i++)
    ;
}
```
</details>


---

### Comment 2 - EugeneZelenko

@k-arrows: Please do not forget to set `Type` to `Bug` for chashes.

---

### Comment 3 - shafik

Assertion:

```console
clang++: /root/llvm-project/llvm/include/llvm/ADT/APInt.h:127:
llvm::APInt::APInt(unsigned int, uint64_t, bool, bool):
Assertion `llvm::isUIntN(BitWidth, val) && "Value is not an N-bit unsigned value"' failed.
```

Bcktrace:

```console
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -fopenmp <source>
1.	<source>:5:1: current parser token '}'
2.	<source>:1:14: parsing function body 'f'
3.	<source>:1:14: in compound statement ('{}')
 #0 0x0000000003f860b8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f860b8)
 #1 0x0000000003f83d44 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f83d44)
 #2 0x0000000003ec8c08 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007fc1c5842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007fc1c58969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007fc1c5842476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007fc1c58287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007fc1c582871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007fc1c5839e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000006e02e99 clang::SemaOpenMP::ActOnOpenMPUnrollDirective(llvm::ArrayRef<clang::OMPClause*>, clang::Stmt*, clang::SourceLocation, clang::SourceLocation)::'lambda'()::operator()() const SemaOpenMP.cpp:0:0
#10 0x0000000006f079f5 clang::SemaOpenMP::ActOnOpenMPUnrollDirective(llvm::ArrayRef<clang::OMPClause*>, clang::Stmt*, clang::SourceLocation, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f079f5)
#11 0x0000000006e91d47 clang::SemaOpenMP::ActOnOpenMPExecutableDirective(llvm::omp::Directive, clang::DeclarationNameInfo const&, llvm::omp::Directive, llvm::ArrayRef<clang::OMPClause*>, clang::Stmt*, clang::SourceLocation, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e91d47)
#12 0x00000000066d2834 clang::Parser::ParseOpenMPExecutableDirective(clang::Parser::ParsedStmtContext, llvm::omp::Directive, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66d2834)
#13 0x00000000066e1d95 clang::Parser::ParseOpenMPDeclarativeOrExecutableDirective(clang::Parser::ParsedStmtContext, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66e1d95)
#14 0x00000000066ffff2 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66ffff2)
#15 0x0000000006701e9d clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6701e9d)
#16 0x0000000006709bb3 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6709bb3)
#17 0x000000000670a37a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x670a37a)
#18 0x0000000006611983 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6611983)
#19 0x000000000664802d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x664802d)
#20 0x00000000066054be clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66054be)
#21 0x0000000006605c79 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6605c79)
#22 0x000000000660d59a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x660d59a)
#23 0x000000000660e53d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x660e53d)
#24 0x000000000660e9f0 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x660e9f0)
#25 0x00000000066008e3 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66008e3)
#26 0x0000000004929f18 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4929f18)
#27 0x0000000004c1dde5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c1dde5)
#28 0x0000000004b9cf4e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b9cf4e)
#29 0x0000000004d11b49 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d11b49)
#30 0x0000000000da7b4f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda7b4f)
#31 0x0000000000d9dd1a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#32 0x0000000004990ae9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#33 0x0000000003ec90a4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3ec90a4)
#34 0x00000000049910ff clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#35 0x000000000495354d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x495354d)
#36 0x00000000049545de clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49545de)
#37 0x000000000495c435 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x495c435)
#38 0x0000000000da3ab8 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda3ab8)
#39 0x0000000000c293c4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc293c4)
#40 0x00007fc1c5829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#41 0x00007fc1c5829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#42 0x0000000000d9d7c5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd9d7c5)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

---

### Comment 4 - AaronBallman

The issue is here: https://github.com/llvm/llvm-project/blob/8e9227a8c636ae380010029f516206e76a694b8e/clang/lib/Sema/SemaOpenMP.cpp#L14916

We're not checking: 1) whether the width of the constant value can fit within the width of the iteration type, 2) whether the value can fit in a `uint64_t`, and 3) whether the iteration type is signed or not when constructing the `APInt` a few lines below. So a few new diagnostics need to be introduced for (1) and (2), and test coverage needs to be added for the various ways this can fail.

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

Author: None (k-arrows)

<details>
Reproducible on Godbolt:
https://godbolt.org/z/xzM7oEf33
```cpp
void f(void) {
#pragma omp unroll partial(0xFFFFFFFFF)
  for (int i = 0; i &lt; 10; i++)
    ;
}
```
</details>


---

### Comment 7 - albus-droid

I'd like to work on this

---

### Comment 8 - shafik

> I'd like to work on this

We have been experimenting with a new process, once you have opened a PR then we will assign the issue to you.

---

### Comment 9 - albus-droid

@shafik I opened a pull request

---

