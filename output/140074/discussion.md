# [OpenMP] Assertion `!isNull() && "Cannot retrieve a NULL type pointer"' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/140074

## Body

Reproducer:
https://godbolt.org/z/PzTa7re3n
```cpp
void f(int a, _BitInt(931) b) {
#pragma omp for collapse(2)
  for (int i = a; i < b; i++)
    ;
}
```

The original reproducer of this issue is the following:
https://github.com/gcc-mirror/gcc/blob/master/libgomp/testsuite/libgomp.c/bitint-1.c

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/include/clang/AST/Type.h:952: const clang::ExtQualsTypeCommonBase* clang::QualType::getCommonPtr() const: Assertion `!isNull() && "Cannot retrieve a NULL type pointer"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -fopenmp <source>
1.	<source>:5:1: current parser token '}'
2.	<source>:1:31: parsing function body 'f'
3.	<source>:1:31: in compound statement ('{}')
 #0 0x0000000003f9b2a8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f9b2a8)
 #1 0x0000000003f98f34 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f98f34)
 #2 0x0000000003eddb68 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000070824a842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x000070824a8969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x000070824a842476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x000070824a8287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x000070824a82871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x000070824a839e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000006f7c6a7 IsStandardConversion(clang::Sema&, clang::Expr*, clang::QualType, bool, clang::StandardConversionSequence&, bool, bool) SemaOverload.cpp:0:0
#10 0x0000000006f7d6bd TryImplicitConversion(clang::Sema&, clang::Expr*, clang::QualType, bool, clang::Sema::AllowedExplicit, bool, bool, bool, bool) SemaOverload.cpp:0:0
#11 0x0000000006f8d28b clang::Sema::PerformImplicitConversion(clang::Expr*, clang::QualType, clang::AssignmentAction, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f8d28b)
#12 0x0000000006eb46bb (anonymous namespace)::calculateNumIters(clang::Sema&, clang::Scope*, clang::SourceLocation, clang::Expr*, clang::Expr*, clang::Expr*, clang::QualType, bool, bool, llvm::MapVector<clang::Expr const*, clang::DeclRefExpr*, llvm::DenseMap<clang::Expr const*, unsigned int, llvm::DenseMapInfo<clang::Expr const*, void>, llvm::detail::DenseMapPair<clang::Expr const*, unsigned int>>, llvm::SmallVector<std::pair<clang::Expr const*, clang::DeclRefExpr*>, 0u>>&) SemaOpenMP.cpp:0:0
#13 0x0000000006eb5684 (anonymous namespace)::OpenMPIterationSpaceChecker::buildNumIterations(clang::Scope*, llvm::ArrayRef<(anonymous namespace)::LoopIterationSpace>, bool, llvm::MapVector<clang::Expr const*, clang::DeclRefExpr*, llvm::DenseMap<clang::Expr const*, unsigned int, llvm::DenseMapInfo<clang::Expr const*, void>, llvm::detail::DenseMapPair<clang::Expr const*, unsigned int>>, llvm::SmallVector<std::pair<clang::Expr const*, clang::DeclRefExpr*>, 0u>>&) const SemaOpenMP.cpp:0:0
#14 0x0000000006eb631f checkOpenMPIterationSpace(llvm::omp::Directive, clang::Stmt*, clang::Sema&, (anonymous namespace)::DSAStackTy&, unsigned int, unsigned int, unsigned int, clang::Expr*, clang::Expr*, llvm::SmallDenseMap<clang::ValueDecl const*, clang::Expr const*, 4u, llvm::DenseMapInfo<clang::ValueDecl const*, void>, llvm::detail::DenseMapPair<clang::ValueDecl const*, clang::Expr const*>>&, llvm::MutableArrayRef<(anonymous namespace)::LoopIterationSpace>, llvm::MapVector<clang::Expr const*, clang::DeclRefExpr*, llvm::DenseMap<clang::Expr const*, unsigned int, llvm::DenseMapInfo<clang::Expr const*, void>, llvm::detail::DenseMapPair<clang::Expr const*, unsigned int>>, llvm::SmallVector<std::pair<clang::Expr const*, clang::DeclRefExpr*>, 0u>>&, llvm::SmallPtrSetImpl<clang::Decl const*> const&) (.isra.0) SemaOpenMP.cpp:0:0
#15 0x0000000006ebbae8 bool llvm::function_ref<bool (unsigned int, clang::Stmt*)>::callback_fn<checkOpenMPLoop(llvm::omp::Directive, clang::Expr*, clang::Expr*, clang::Stmt*, clang::Sema&, (anonymous namespace)::DSAStackTy&, llvm::SmallDenseMap<clang::ValueDecl const*, clang::Expr const*, 4u, llvm::DenseMapInfo<clang::ValueDecl const*, void>, llvm::detail::DenseMapPair<clang::ValueDecl const*, clang::Expr const*>>&, clang::OMPLoopBasedDirective::HelperExprs&)::'lambda'(unsigned int, clang::Stmt*)>(long, unsigned int, clang::Stmt*) SemaOpenMP.cpp:0:0
#16 0x0000000007c595b5 clang::OMPLoopBasedDirective::doForAllLoops(clang::Stmt*, bool, unsigned int, llvm::function_ref<bool (unsigned int, clang::Stmt*)>, llvm::function_ref<void (clang::OMPLoopTransformationDirective*)>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7c595b5)
#17 0x0000000006eac526 checkOpenMPLoop(llvm::omp::Directive, clang::Expr*, clang::Expr*, clang::Stmt*, clang::Sema&, (anonymous namespace)::DSAStackTy&, llvm::SmallDenseMap<clang::ValueDecl const*, clang::Expr const*, 4u, llvm::DenseMapInfo<clang::ValueDecl const*, void>, llvm::detail::DenseMapPair<clang::ValueDecl const*, clang::Expr const*>>&, clang::OMPLoopBasedDirective::HelperExprs&) (.part.0) SemaOpenMP.cpp:0:0
#18 0x0000000006eae925 clang::SemaOpenMP::ActOnOpenMPForDirective(llvm::ArrayRef<clang::OMPClause*>, clang::Stmt*, clang::SourceLocation, clang::SourceLocation, llvm::SmallDenseMap<clang::ValueDecl const*, clang::Expr const*, 4u, llvm::DenseMapInfo<clang::ValueDecl const*, void>, llvm::detail::DenseMapPair<clang::ValueDecl const*, clang::Expr const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6eae925)
#19 0x0000000006ed4874 clang::SemaOpenMP::ActOnOpenMPExecutableDirective(llvm::omp::Directive, clang::DeclarationNameInfo const&, llvm::omp::Directive, llvm::ArrayRef<clang::OMPClause*>, clang::Stmt*, clang::SourceLocation, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ed4874)
#20 0x000000000671137c clang::Parser::ParseOpenMPExecutableDirective(clang::Parser::ParsedStmtContext, llvm::omp::Directive, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x671137c)
#21 0x0000000006720c06 clang::Parser::ParseOpenMPDeclarativeOrExecutableDirective(clang::Parser::ParsedStmtContext, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6720c06)
#22 0x000000000673f0b2 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x673f0b2)
#23 0x0000000006740f2d clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6740f2d)
#24 0x0000000006748c53 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6748c53)
#25 0x000000000674941a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x674941a)
#26 0x0000000006643433 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6643433)
#27 0x0000000006679dcd clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6679dcd)
#28 0x0000000006636f7e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6636f7e)
#29 0x0000000006637739 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6637739)
#30 0x000000000663f04a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x663f04a)
#31 0x000000000663ffed clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x663ffed)
#32 0x00000000066404a0 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66404a0)
#33 0x0000000006632383 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6632383)
#34 0x0000000004946b78 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4946b78)
#35 0x0000000004c3eb45 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c3eb45)
#36 0x0000000004bbd78e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bbd78e)
#37 0x0000000004d31629 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d31629)
#38 0x0000000000dace0f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdace0f)
#39 0x0000000000da2fda ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#40 0x00000000049ade69 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#41 0x0000000003ede004 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3ede004)
#42 0x00000000049ae47f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#43 0x000000000497024d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x497024d)
#44 0x00000000049712de clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49712de)
#45 0x00000000049795c5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49795c5)
#46 0x0000000000da8d78 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda8d78)
#47 0x0000000000c2e774 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc2e774)
#48 0x000070824a829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#49 0x000070824a829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#50 0x0000000000da2a85 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda2a85)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/PzTa7re3n
```cpp
void f(int a, _BitInt(931) b) {
#pragma omp for collapse(2)
  for (int i = a; i &lt; b; i++)
    ;
}
```
</details>


---

### Comment 2 - k-arrows

Goes back to clang-16: https://godbolt.org/z/fqGdqT69e

---

