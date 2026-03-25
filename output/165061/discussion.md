# [clang][bytecode] crash on __builtin_memcmp

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/165061
**Status:** Closed
**Labels:** crash, clang:bytecode
**Closed Date:** 2025-10-27T09:43:02Z

## Body

Reproducer:
https://godbolt.org/z/orMnasT43
```c
int a;

void foo(void) { a *= __builtin_memcmp(0, 0, 2); }
```

Backtrace:
```console
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x c -fexperimental-new-constant-interpreter <source>
1.	<source>:3:50: current parser token '}'
2.	<source>:3:16: parsing function body 'foo'
3.	<source>:3:16: in compound statement ('{}')
 #0 0x0000000003cecc38 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3cecc38)
 #1 0x0000000003cea60c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3cea60c)
 #2 0x0000000003c30708 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000074c795e42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x0000000007536310 clang::interp::Descriptor::getType() const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x7536310)
 #5 0x00000000076475c4 clang::interp::getElemType(clang::interp::Pointer const&) InterpBuiltin.cpp:0:0
 #6 0x000000000766673d clang::interp::interp__builtin_memcmp(clang::interp::InterpState&, clang::interp::CodePtr, clang::interp::InterpFrame const*, clang::CallExpr const*, unsigned int) (.isra.0) InterpBuiltin.cpp:0:0
 #7 0x000000000767eccc clang::interp::InterpretBuiltin(clang::interp::InterpState&, clang::interp::CodePtr, clang::CallExpr const*, unsigned int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x767eccc)
 #8 0x00000000079427c0 clang::interp::Compiler<clang::interp::EvalEmitter>::VisitBuiltinCallExpr(clang::CallExpr const*, unsigned int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x79427c0)
 #9 0x00000000079429a6 clang::interp::Compiler<clang::interp::EvalEmitter>::VisitCallExpr(clang::CallExpr const*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x79429a6)
#10 0x000000000794919b clang::interp::Compiler<clang::interp::EvalEmitter>::visit(clang::Expr const*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x794919b)
#11 0x0000000007920c05 clang::interp::Compiler<clang::interp::EvalEmitter>::VisitCompoundAssignOperator(clang::CompoundAssignOperator const*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x7920c05)
#12 0x000000000794919b clang::interp::Compiler<clang::interp::EvalEmitter>::visit(clang::Expr const*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x794919b)
#13 0x000000000793f90f clang::interp::Compiler<clang::interp::EvalEmitter>::visitExpr(clang::Expr const*, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x793f90f)
#14 0x000000000753d983 clang::interp::EvalEmitter::interpretExpr(clang::Expr const*, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x753d983)
#15 0x000000000752e9b6 clang::interp::Context::evaluateAsRValue(clang::interp::State&, clang::Expr const*, clang::APValue&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x752e9b6)
#16 0x000000000749b60a EvaluateAsRValue((anonymous namespace)::EvalInfo&, clang::Expr const*, clang::APValue&) ExprConstant.cpp:0:0
#17 0x000000000749f910 clang::Expr::EvaluateForOverflow(clang::ASTContext const&) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x749f910)
#18 0x00000000064a6886 clang::Sema::CheckForIntOverflow(clang::Expr const*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x64a6886)
#19 0x00000000064e652b clang::Sema::CheckCompletedExpr(clang::Expr*, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x64e652b)
#20 0x0000000006881339 clang::Sema::ActOnFinishFullExpr(clang::Expr*, clang::SourceLocation, bool, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6881339)
#21 0x0000000006ba91ae clang::Sema::ActOnExprStmt(clang::ActionResult<clang::Expr*, true>, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6ba91ae)
#22 0x00000000063612bc clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x63612bc)
#23 0x0000000006358019 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::LabelDecl*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6358019)
#24 0x00000000063589f6 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x63589f6)
#25 0x0000000006361e30 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6361e30)
#26 0x000000000636286a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x636286a)
#27 0x000000000624ccab clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x624ccab)
#28 0x00000000062995bf clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62995bf)
#29 0x00000000062460ef clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62460ef)
#30 0x0000000006246827 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6246827)
#31 0x000000000624f1ff clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x624f1ff)
#32 0x0000000006250a07 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6250a07)
#33 0x00000000062298fa clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62298fa)
#34 0x00000000045b80d5 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45b80d5)
#35 0x00000000048b4c1a clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x48b4c1a)
#36 0x0000000004834d4b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4834d4b)
#37 0x00000000049abe0b clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x49abe0b)
#38 0x0000000000ddd175 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xddd175)
#39 0x0000000000dd50cb ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#40 0x0000000000dd516d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#41 0x0000000004625989 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#42 0x0000000003c30b23 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c30b23)
#43 0x0000000004625ba9 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#44 0x00000000045e8de2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45e8de2)
#45 0x00000000045e9cc1 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45e9cc1)
#46 0x00000000045f27fc clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45f27fc)
#47 0x0000000000dd9bc9 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdd9bc9)
#48 0x0000000000c84984 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc84984)
#49 0x000074c795e29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#50 0x000074c795e29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#51 0x0000000000dd4b65 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdd4b65)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```

## Comments

### Comment 1 - k-arrows

Goes back to clang-20.1.0:
https://godbolt.org/z/M6vxocrz9

---

