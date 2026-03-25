# [clang][bytecode] crash when array index is past the end of array

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/165090
**Status:** Closed
**Labels:** crash, clang:bytecode
**Closed Date:** 2025-10-27T14:51:37Z

## Body

Reproducer:
https://godbolt.org/z/nhvha45Ej
```c
void foo(int, ...);

void bar(void) {
  int a[2][3][4][5];
  foo(__LINE__, &a[0][4]);
}
```

Backtrace:
```console
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -fexperimental-new-constant-interpreter <source>
1.	<source>:6:1: current parser token '}'
2.	<source>:3:16: parsing function body 'bar'
3.	<source>:3:16: in compound statement ('{}')
 #0 0x0000000003cf5858 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3cf5858)
 #1 0x0000000003cf322c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3cf322c)
 #2 0x0000000003c39108 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x0000771a19242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x000000000771df3e clang::interp::Pointer::toAPValue(clang::ASTContext const&) const (/opt/compiler-explorer/clang-trunk/bin/clang+0x771df3e)
 #5 0x00000000075695d5 bool clang::interp::EvalEmitter::emitRet<(clang::interp::PrimType)13>(clang::interp::SourceInfo) (/opt/compiler-explorer/clang-trunk/bin/clang+0x75695d5)
 #6 0x00000000075c7fd8 clang::interp::EvalEmitter::emitRet(clang::interp::PrimType, clang::interp::SourceInfo) (/opt/compiler-explorer/clang-trunk/bin/clang+0x75c7fd8)
 #7 0x000000000793d0c7 clang::interp::Compiler<clang::interp::EvalEmitter>::visitExpr(clang::Expr const*, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+0x793d0c7)
 #8 0x000000000754c1a3 clang::interp::EvalEmitter::interpretExpr(clang::Expr const*, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+0x754c1a3)
 #9 0x000000000753d1d6 clang::interp::Context::evaluateAsRValue(clang::interp::State&, clang::Expr const*, clang::APValue&) (/opt/compiler-explorer/clang-trunk/bin/clang+0x753d1d6)
#10 0x00000000074a908a EvaluateAsRValue((anonymous namespace)::EvalInfo&, clang::Expr const*, clang::APValue&) ExprConstant.cpp:0:0
#11 0x00000000074ad390 clang::Expr::EvaluateForOverflow(clang::ASTContext const&) const (/opt/compiler-explorer/clang-trunk/bin/clang+0x74ad390)
#12 0x00000000064b47d6 clang::Sema::CheckForIntOverflow(clang::Expr const*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x64b47d6)
#13 0x00000000064f442b clang::Sema::CheckCompletedExpr(clang::Expr*, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+0x64f442b)
#14 0x000000000688f239 clang::Sema::ActOnFinishFullExpr(clang::Expr*, clang::SourceLocation, bool, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+0x688f239)
#15 0x0000000006bb67ee clang::Sema::ActOnExprStmt(clang::ActionResult<clang::Expr*, true>, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+0x6bb67ee)
#16 0x000000000636f2ac clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-trunk/bin/clang+0x636f2ac)
#17 0x0000000006366009 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::LabelDecl*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x6366009)
#18 0x00000000063669e6 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x63669e6)
#19 0x000000000636fe20 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-trunk/bin/clang+0x636fe20)
#20 0x000000000637085a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-trunk/bin/clang+0x637085a)
#21 0x000000000625ac9b clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x625ac9b)
#22 0x00000000062a75af clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x62a75af)
#23 0x00000000062540df clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+0x62540df)
#24 0x0000000006254817 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+0x6254817)
#25 0x000000000625d1ef clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x625d1ef)
#26 0x000000000625e9f7 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-trunk/bin/clang+0x625e9f7)
#27 0x00000000062378ea clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+0x62378ea)
#28 0x00000000045c1525 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+0x45c1525)
#29 0x00000000048beb5a clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+0x48beb5a)
#30 0x000000000483ec8b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-trunk/bin/clang+0x483ec8b)
#31 0x00000000049b5d4b clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x49b5d4b)
#32 0x0000000000ddd7f5 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+0xddd7f5)
#33 0x0000000000dd574b ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#34 0x0000000000dd57ed int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#35 0x000000000462edd9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#36 0x0000000003c39523 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3c39523)
#37 0x000000000462eff9 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#38 0x00000000045f2232 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+0x45f2232)
#39 0x00000000045f3111 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+0x45f3111)
#40 0x00000000045fbc4c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-trunk/bin/clang+0x45fbc4c)
#41 0x0000000000dda249 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-trunk/bin/clang+0xdda249)
#42 0x0000000000c84da4 main (/opt/compiler-explorer/clang-trunk/bin/clang+0xc84da4)
#43 0x0000771a19229d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#44 0x0000771a19229e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#45 0x0000000000dd51e5 _start (/opt/compiler-explorer/clang-trunk/bin/clang+0xdd51e5)
clang: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```

## Comments

### Comment 1 - k-arrows

Goes back to clang-18.1.0:
https://godbolt.org/z/1dvovx6rc

---

