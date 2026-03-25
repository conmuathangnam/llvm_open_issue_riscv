# [OpenACC] UNREACHABLE executed at /root/llvm-project/llvm/tools/clang/lib/Sema/SemaOpenACCClause.cpp:1305!

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/140339
**Status:** Closed
**Labels:** clang:frontend, crash, clang:openacc
**Closed Date:** 2025-05-19T16:57:58Z

## Body

Reproducer:
https://godbolt.org/z/d3xM5dYsv
```cpp
void foo() {
#pragma acc serial loop
  for (int i = 0; i < 10; i++) {
#pragma acc loop
    for (int j = 0; j < 10; j++) {
#pragma acc loop vector(length : 5)
      for (int k = 0; k < 10; k++)
        ;
    }
  }
}
```

The original reproducer of this issue is the following:
https://github.com/gcc-mirror/gcc/blob/master/gcc/testsuite/c-c%2B%2B-common/goacc/pr92793-1.c

Backtrace:
```console
Non compute construct in active compute construct
UNREACHABLE executed at /root/llvm-project/llvm/tools/clang/lib/Sema/SemaOpenACCClause.cpp:1305!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -fopenacc -fclangir <source>
1.	<source>:6:36: at annotation token
2.	<source>:1:12: parsing function body 'foo'
3.	<source>:1:12: in compound statement ('{}')
4.	<source>:3:32: in compound statement ('{}')
5.	<source>:5:34: in compound statement ('{}')
 #0 0x0000000003f99d48 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f99d48)
 #1 0x0000000003f979d4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f979d4)
 #2 0x0000000003edc608 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000076f392c42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x000076f392c969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x000076f392c42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x000076f392c287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x0000000003ee7d4a (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3ee7d4a)
 #8 0x0000000006e0df1c (anonymous namespace)::SemaOpenACCClauseVisitor::VisitVectorClause(clang::SemaOpenACC::OpenACCParsedClause&) SemaOpenACCClause.cpp:0:0
 #9 0x0000000006e1706b (anonymous namespace)::SemaOpenACCClauseVisitor::Visit(clang::SemaOpenACC::OpenACCParsedClause&) SemaOpenACCClause.cpp:0:0
#10 0x0000000006e184de clang::SemaOpenACC::ActOnClause(llvm::ArrayRef<clang::OpenACCClause const*>, clang::SemaOpenACC::OpenACCParsedClause&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e184de)
#11 0x000000000664629f clang::Parser::ParseOpenACCClauseParams(llvm::ArrayRef<clang::OpenACCClause const*>, clang::OpenACCDirectiveKind, clang::OpenACCClauseKind, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x664629f)
#12 0x00000000066486e6 clang::Parser::ParseOpenACCClauseList(clang::OpenACCDirectiveKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66486e6)
#13 0x000000000664905e clang::Parser::ParseOpenACCDirective() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x664905e)
#14 0x000000000664b4fc clang::Parser::ParseOpenACCDirectiveStmt() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x664b4fc)
#15 0x000000000673a7e2 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x673a7e2)
#16 0x000000000673bc9d clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x673bc9d)
#17 0x00000000067439c3 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67439c3)
#18 0x00000000067440b4 void llvm::function_ref<void ()>::callback_fn<clang::Parser::ParseCompoundStatement(bool, unsigned int)::'lambda'()>(long) ParseStmt.cpp:0:0
#19 0x0000000007f348d1 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7f348d1)
#20 0x0000000006735535 clang::Parser::ParseCompoundStatement(bool, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6735535)
#21 0x0000000006739d97 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6739d97)
#22 0x000000000673bc9d clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x673bc9d)
#23 0x000000000673ca38 clang::Parser::ParseStatement(clang::SourceLocation*, clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x673ca38)
#24 0x000000000673dd8a clang::Parser::ParseForStatement(clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x673dd8a)
#25 0x000000000673a657 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x673a657)
#26 0x000000000673bc9d clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x673bc9d)
#27 0x000000000673ca38 clang::Parser::ParseStatement(clang::SourceLocation*, clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x673ca38)
#28 0x000000000664b5e7 clang::Parser::ParseOpenACCDirectiveStmt() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x664b5e7)
#29 0x000000000673a7e2 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x673a7e2)
#30 0x000000000673bc9d clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x673bc9d)
#31 0x00000000067439c3 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67439c3)
#32 0x00000000067440b4 void llvm::function_ref<void ()>::callback_fn<clang::Parser::ParseCompoundStatement(bool, unsigned int)::'lambda'()>(long) ParseStmt.cpp:0:0
#33 0x0000000007f348d1 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7f348d1)
#34 0x0000000006735535 clang::Parser::ParseCompoundStatement(bool, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6735535)
#35 0x0000000006739d97 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6739d97)
#36 0x000000000673bc9d clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x673bc9d)
#37 0x000000000673ca38 clang::Parser::ParseStatement(clang::SourceLocation*, clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x673ca38)
#38 0x000000000673dd8a clang::Parser::ParseForStatement(clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x673dd8a)
#39 0x000000000673a657 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x673a657)
#40 0x000000000673bc9d clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x673bc9d)
#41 0x000000000673ca38 clang::Parser::ParseStatement(clang::SourceLocation*, clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x673ca38)
#42 0x000000000664b5e7 clang::Parser::ParseOpenACCDirectiveStmt() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x664b5e7)
#43 0x000000000673a7e2 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x673a7e2)
#44 0x000000000673bc9d clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x673bc9d)
#45 0x00000000067439c3 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67439c3)
#46 0x000000000674418a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x674418a)
#47 0x000000000663e1a3 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x663e1a3)
#48 0x0000000006674b3d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6674b3d)
#49 0x0000000006631cee clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6631cee)
#50 0x00000000066324a9 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66324a9)
#51 0x0000000006639dba clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6639dba)
#52 0x000000000663ad5d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x663ad5d)
#53 0x000000000663b210 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x663b210)
#54 0x000000000662d0f3 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x662d0f3)
#55 0x0000000004944fd8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4944fd8)
#56 0x0000000004c3cea5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c3cea5)
#57 0x0000000004bbbb3e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bbbb3e)
#58 0x0000000004d30a49 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d30a49)
#59 0x0000000000dac29f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdac29f)
#60 0x0000000000da246a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#61 0x00000000049ac2c9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#62 0x0000000003edcaa4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3edcaa4)
#63 0x00000000049ac8df clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#64 0x000000000496e6ad clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x496e6ad)
#65 0x000000000496f73e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x496f73e)
#66 0x0000000004977a25 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4977a25)
#67 0x0000000000da8208 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda8208)
#68 0x0000000000c2df44 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc2df44)
#69 0x000076f392c29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#70 0x000076f392c29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#71 0x0000000000da1f15 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda1f15)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/d3xM5dYsv
```cpp
void foo() {
#pragma acc serial loop
  for (int i = 0; i &lt; 10; i++) {
#pragma acc loop
    for (int j = 0; j &lt; 10; j++) {
#pragma acc loop vector(length : 5)
      for (int k = 0; k &lt; 10; k++)
        ;
    }
  }
}
```
</details>


---

### Comment 2 - shafik

CC @erichkeane 

---

### Comment 3 - erichkeane

Hmm... I thought I explicitly tested this/made sure we couldn't get into this state.  I'll work on this ASAP, I'd expect the 'active' compute construct to only be set for certain things, and cleared properly, so I'm a bit surprised the above happened.

Anyway, looking at it now.  Thanks a bunch for the bug!

---

### Comment 4 - k-arrows

Thank you for taking the time to look into this bug report.
There’s absolutely no rush — please feel free to address it at your own pace.

---

