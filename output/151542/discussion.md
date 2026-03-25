# [clang][OpenMP] Assertion `(LE->getType().getTypePtr() == BO->getType().getTypePtr() || RE->getType().getTypePtr() == BO->getType().getTypePtr()) && "Either LHS or RHS have base decl inside"' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/151542

## Body

Reproducer:
https://godbolt.org/z/eorMcoebz
```cpp
struct S {
  int x;
  int *ptr;
};

void f ()
{
  S *s = new S;
  int S::* xp = &S::x;
  int* S::* ptrp = &S::ptr;

  #pragma omp target map(s->*xp, s->*ptrp, (s->*ptrp)[:64])
}
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/Sema/SemaOpenMP.cpp:21140: bool {anonymous}::MapBaseChecker::VisitBinaryOperator(clang::BinaryOperator*): Assertion `(LE->getType().getTypePtr() == BO->getType().getTypePtr() || RE->getType().getTypePtr() == BO->getType().getTypePtr()) && "Either LHS or RHS have base decl inside"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -fopenmp <source>
1.	<source>:12:60: at annotation token
2.	<source>:7:1: parsing function body 'f'
3.	<source>:7:1: in compound statement ('{}')
 #0 0x0000000003fdb0d8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3fdb0d8)
 #1 0x0000000003fd8504 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3fd8504)
 #2 0x0000000003f1ccf8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007bfccb842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007bfccb8969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007bfccb842476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007bfccb8287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007bfccb82871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007bfccb839e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000006e5fb1b (anonymous namespace)::MapBaseChecker::VisitBinaryOperator(clang::BinaryOperator*) SemaOpenMP.cpp:0:0
#10 0x0000000006e5fb57 clang::StmtVisitorBase<std::add_pointer, (anonymous namespace)::MapBaseChecker, bool>::Visit(clang::Stmt*) SemaOpenMP.cpp:0:0
#11 0x0000000006e60e45 checkMapClauseExpressionBase(clang::Sema&, clang::Expr*, llvm::SmallVector<clang::OMPClauseMappableExprCommon::MappableComponent, 8u>&, llvm::omp::Clause, llvm::omp::Directive, bool) SemaOpenMP.cpp:0:0
#12 0x0000000006e92214 checkMappableExpressionList(clang::Sema&, (anonymous namespace)::DSAStackTy*, llvm::omp::Clause, (anonymous namespace)::MappableVarListInfo&, clang::SourceLocation, clang::CXXScopeSpec&, clang::DeclarationNameInfo, llvm::ArrayRef<clang::Expr*>, clang::OpenMPMapClauseKind, llvm::ArrayRef<clang::OpenMPMapModifierKind>, bool, bool) SemaOpenMP.cpp:0:0
#13 0x0000000006e946c4 clang::SemaOpenMP::ActOnOpenMPMapClause(clang::Expr*, llvm::ArrayRef<clang::OpenMPMapModifierKind>, llvm::ArrayRef<clang::SourceLocation>, clang::CXXScopeSpec&, clang::DeclarationNameInfo&, clang::OpenMPMapClauseKind, bool, clang::SourceLocation, clang::SourceLocation, llvm::ArrayRef<clang::Expr*>, clang::OMPVarListLocTy const&, bool, llvm::ArrayRef<clang::Expr*>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e946c4)
#14 0x0000000006ecd5f1 clang::SemaOpenMP::ActOnOpenMPVarListClause(llvm::omp::Clause, llvm::ArrayRef<clang::Expr*>, clang::OMPVarListLocTy const&, clang::SemaOpenMP::OpenMPVarListDataTy&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ecd5f1)
#15 0x0000000006765db9 clang::Parser::ParseOpenMPVarListClause(llvm::omp::Directive, llvm::omp::Clause, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6765db9)
#16 0x0000000006766261 clang::Parser::ParseOpenMPClause(llvm::omp::Directive, llvm::omp::Clause, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6766261)
#17 0x0000000006767b8b clang::Parser::ParseOpenMPExecutableDirective(clang::Parser::ParsedStmtContext, llvm::omp::Directive, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6767b8b)
#18 0x0000000006777a04 clang::Parser::ParseOpenMPDeclarativeOrExecutableDirective(clang::Parser::ParsedStmtContext, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6777a04)
#19 0x0000000006796374 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6796374)
#20 0x0000000006797fd9 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6797fd9)
#21 0x00000000067a010b clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67a010b)
#22 0x00000000067a095a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67a095a)
#23 0x0000000006698063 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6698063)
#24 0x00000000066cde4d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66cde4d)
#25 0x000000000668ae3e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x668ae3e)
#26 0x000000000668b5cf clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x668b5cf)
#27 0x00000000066932da clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66932da)
#28 0x0000000006694265 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6694265)
#29 0x0000000006685fda clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6685fda)
#30 0x0000000004987388 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4987388)
#31 0x0000000004c7aa05 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c7aa05)
#32 0x0000000004bf780e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bf780e)
#33 0x0000000004d6e731 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d6e731)
#34 0x0000000000daa41f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdaa41f)
#35 0x0000000000da106a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#36 0x00000000049eeaa9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#37 0x0000000003f1d194 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f1d194)
#38 0x00000000049ef0bf clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#39 0x00000000049b16ed clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49b16ed)
#40 0x00000000049b277e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49b277e)
#41 0x00000000049ba7b5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49ba7b5)
#42 0x0000000000da68cf clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda68cf)
#43 0x0000000000c5b7d4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc5b7d4)
#44 0x00007bfccb829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#45 0x00007bfccb829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#46 0x0000000000da0b15 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda0b15)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - k-arrows

Goes back to clang-11.0.0:
https://godbolt.org/z/G8K9Tbjd6

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/eorMcoebz
```cpp
struct S {
  int x;
  int *ptr;
};

void f ()
{
  S *s = new S;
  int S::* xp = &amp;S::x;
  int* S::* ptrp = &amp;S::ptr;

  #pragma omp target map(s-&gt;*xp, s-&gt;*ptrp, (s-&gt;*ptrp)[:64])
}
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/Sema/SemaOpenMP.cpp:21140: bool {anonymous}::MapBaseChecker::VisitBinaryOperator(clang::BinaryOperator*): Assertion `(LE-&gt;getType().getTypePtr() == BO-&gt;getType().getTypePtr() || RE-&gt;getType().getTypePtr() == BO-&gt;getType().getTypePtr()) &amp;&amp; "Either LHS or RHS have base decl inside"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -fopenmp &lt;source&gt;
1.	&lt;source&gt;:12:60: at annotation token
2.	&lt;source&gt;:7:1: parsing function body 'f'
3.	&lt;source&gt;:7:1: in compound statement ('{}')
 #<!-- -->0 0x0000000003fdb0d8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3fdb0d8)
 #<!-- -->1 0x0000000003fd8504 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3fd8504)
 #<!-- -->2 0x0000000003f1ccf8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007bfccb842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007bfccb8969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007bfccb842476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007bfccb8287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x00007bfccb82871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x00007bfccb839e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000006e5fb1b (anonymous namespace)::MapBaseChecker::VisitBinaryOperator(clang::BinaryOperator*) SemaOpenMP.cpp:0:0
#<!-- -->10 0x0000000006e5fb57 clang::StmtVisitorBase&lt;std::add_pointer, (anonymous namespace)::MapBaseChecker, bool&gt;::Visit(clang::Stmt*) SemaOpenMP.cpp:0:0
#<!-- -->11 0x0000000006e60e45 checkMapClauseExpressionBase(clang::Sema&amp;, clang::Expr*, llvm::SmallVector&lt;clang::OMPClauseMappableExprCommon::MappableComponent, 8u&gt;&amp;, llvm::omp::Clause, llvm::omp::Directive, bool) SemaOpenMP.cpp:0:0
#<!-- -->12 0x0000000006e92214 checkMappableExpressionList(clang::Sema&amp;, (anonymous namespace)::DSAStackTy*, llvm::omp::Clause, (anonymous namespace)::MappableVarListInfo&amp;, clang::SourceLocation, clang::CXXScopeSpec&amp;, clang::DeclarationNameInfo, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::OpenMPMapClauseKind, llvm::ArrayRef&lt;clang::OpenMPMapModifierKind&gt;, bool, bool) SemaOpenMP.cpp:0:0
#<!-- -->13 0x0000000006e946c4 clang::SemaOpenMP::ActOnOpenMPMapClause(clang::Expr*, llvm::ArrayRef&lt;clang::OpenMPMapModifierKind&gt;, llvm::ArrayRef&lt;clang::SourceLocation&gt;, clang::CXXScopeSpec&amp;, clang::DeclarationNameInfo&amp;, clang::OpenMPMapClauseKind, bool, clang::SourceLocation, clang::SourceLocation, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::OMPVarListLocTy const&amp;, bool, llvm::ArrayRef&lt;clang::Expr*&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e946c4)
#<!-- -->14 0x0000000006ecd5f1 clang::SemaOpenMP::ActOnOpenMPVarListClause(llvm::omp::Clause, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::OMPVarListLocTy const&amp;, clang::SemaOpenMP::OpenMPVarListDataTy&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ecd5f1)
#<!-- -->15 0x0000000006765db9 clang::Parser::ParseOpenMPVarListClause(llvm::omp::Directive, llvm::omp::Clause, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6765db9)
#<!-- -->16 0x0000000006766261 clang::Parser::ParseOpenMPClause(llvm::omp::Directive, llvm::omp::Clause, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6766261)
#<!-- -->17 0x0000000006767b8b clang::Parser::ParseOpenMPExecutableDirective(clang::Parser::ParsedStmtContext, llvm::omp::Directive, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6767b8b)
#<!-- -->18 0x0000000006777a04 clang::Parser::ParseOpenMPDeclarativeOrExecutableDirective(clang::Parser::ParsedStmtContext, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6777a04)
#<!-- -->19 0x0000000006796374 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6796374)
#<!-- -->20 0x0000000006797fd9 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6797fd9)
#<!-- -->21 0x00000000067a010b clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67a010b)
#<!-- -->22 0x00000000067a095a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67a095a)
#<!-- -->23 0x0000000006698063 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6698063)
#<!-- -->24 0x00000000066cde4d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66cde4d)
#<!-- -->25 0x000000000668ae3e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x668ae3e)
#<!-- -->26 0x000000000668b5cf clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x668b5cf)
#<!-- -->27 0x00000000066932da clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66932da)
#<!-- -->28 0x0000000006694265 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6694265)
#<!-- -->29 0x0000000006685fda clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6685fda)
#<!-- -->30 0x0000000004987388 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4987388)
#<!-- -->31 0x0000000004c7aa05 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c7aa05)
#<!-- -->32 0x0000000004bf780e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bf780e)
#<!-- -->33 0x0000000004d6e731 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d6e731)
#<!-- -->34 0x0000000000daa41f cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdaa41f)
#<!-- -->35 0x0000000000da106a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->36 0x00000000049eeaa9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->37 0x0000000003f1d194 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f1d194)
#<!-- -->38 0x00000000049ef0bf clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->39 0x00000000049b16ed clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49b16ed)
#<!-- -->40 0x00000000049b277e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49b277e)
#<!-- -->41 0x00000000049ba7b5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49ba7b5)
#<!-- -->42 0x0000000000da68cf clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda68cf)
#<!-- -->43 0x0000000000c5b7d4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc5b7d4)
#<!-- -->44 0x00007bfccb829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->45 0x00007bfccb829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->46 0x0000000000da0b15 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda0b15)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

