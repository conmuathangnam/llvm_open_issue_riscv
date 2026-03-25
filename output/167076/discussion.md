# [clang][bytecode] assertion failure when computing offset of struct member after huge array

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/167076
**Status:** Closed
**Labels:** crash, clang:bytecode
**Closed Date:** 2025-11-27T12:06:16Z

## Body

Reproducer:
https://godbolt.org/z/adrEG39zT
```cpp
typedef __SIZE_TYPE__ Size_t;

#define bufsize ((1LL << (8 * sizeof(Size_t) - 2)) - 256)

struct S {
  short buf[bufsize];
  int a;
};

Size_t foo() { return (Size_t)(&((struct S *)0)->a); }
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/AST/RecordLayoutBuilder.cpp:3378: const clang::ASTRecordLayout& clang::ASTContext::getASTRecordLayout(const clang::RecordDecl*) const: Assertion `!D->isInvalidDecl() && "Cannot get layout of invalid decl!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -fexperimental-new-constant-interpreter <source>
1.	<source>:10:52: current parser token ';'
2.	<source>:10:14: parsing function body 'foo'
3.	<source>:10:14: in compound statement ('{}')
 #0 0x000000000424be68 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x424be68)
 #1 0x0000000004249294 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4249294)
 #2 0x000000000418d4f8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000079d41c442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x000079d41c4969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x000079d41c442476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x000079d41c4287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x000079d41c42871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x000079d41c439e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000007f97258 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7f97258)
#10 0x0000000007ea69c3 clang::interp::IntPointer::atOffset(clang::ASTContext const&, unsigned int) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7ea69c3)
#11 0x0000000007e5392f clang::interp::getField(clang::interp::InterpState&, clang::interp::CodePtr, clang::interp::Pointer const&, unsigned int) Interp.cpp:0:0
#12 0x0000000007e53a79 clang::interp::GetPtrFieldPop(clang::interp::InterpState&, clang::interp::CodePtr, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7e53a79)
#13 0x000000000812e4a7 clang::interp::Compiler<clang::interp::EvalEmitter>::VisitMemberExpr(clang::MemberExpr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x812e4a7)
#14 0x000000000813826b clang::interp::Compiler<clang::interp::EvalEmitter>::VisitUnaryOperator(clang::UnaryOperator const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x813826b)
#15 0x0000000008133e5a clang::interp::Compiler<clang::interp::EvalEmitter>::visit(clang::Expr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x8133e5a)
#16 0x0000000008128a96 clang::interp::Compiler<clang::interp::EvalEmitter>::visitExpr(clang::Expr const*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x8128a96)
#17 0x0000000007cadead clang::interp::EvalEmitter::interpretExpr(clang::Expr const*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7cadead)
#18 0x0000000007c9d3c3 clang::interp::Context::evaluateAsRValue(clang::interp::State&, clang::Expr const*, clang::APValue&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7c9d3c3)
#19 0x0000000007c08f03 EvaluateAsRValue((anonymous namespace)::EvalInfo&, clang::Expr const*, clang::APValue&) ExprConstant.cpp:0:0
#20 0x0000000007c0ca3e clang::Expr::EvaluateForOverflow(clang::ASTContext const&) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7c0ca3e)
#21 0x0000000006b98cd6 clang::Sema::CheckForIntOverflow(clang::Expr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b98cd6)
#22 0x0000000006bdb78c clang::Sema::CheckCompletedExpr(clang::Expr*, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bdb78c)
#23 0x0000000006f7e247 clang::Sema::ActOnFinishFullExpr(clang::Expr*, clang::SourceLocation, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f7e247)
#24 0x00000000072adc16 clang::Sema::BuildReturnStmt(clang::SourceLocation, clang::Expr*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x72adc16)
#25 0x00000000072ae988 clang::Sema::ActOnReturnStmt(clang::SourceLocation, clang::Expr*, clang::Scope*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x72ae988)
#26 0x0000000006a52bba clang::Parser::ParseReturnStatement() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a52bba)
#27 0x0000000006a56eea clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a56eea)
#28 0x0000000006a57f19 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a57f19)
#29 0x0000000006a606e6 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a606e6)
#30 0x0000000006a60efa clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a60efa)
#31 0x0000000006967a8b clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6967a8b)
#32 0x000000000699de3d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x699de3d)
#33 0x000000000695b23e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x695b23e)
#34 0x000000000695b9df clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x695b9df)
#35 0x0000000006963293 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6963293)
#36 0x00000000069641a5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69641a5)
#37 0x000000000694579a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x694579a)
#38 0x0000000004b97858 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b97858)
#39 0x0000000004e85d25 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e85d25)
#40 0x0000000004e06bbe clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e06bbe)
#41 0x0000000004f7e57d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f7e57d)
#42 0x0000000000dd0d90 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdd0d90)
#43 0x0000000000dc790a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#44 0x0000000000dc7a8d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#45 0x0000000004c016d9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#46 0x000000000418d994 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x418d994)
#47 0x0000000004c01cef clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#48 0x0000000004bc2742 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bc2742)
#49 0x0000000004bc36ee clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bc36ee)
#50 0x0000000004bcae45 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bcae45)
#51 0x0000000000dcd2b1 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdcd2b1)
#52 0x0000000000c7c234 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc7c234)
#53 0x000079d41c429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#54 0x000079d41c429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#55 0x0000000000dc73a5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdc73a5)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - k-arrows

Goes back to clang-20.1.0:
https://godbolt.org/z/9sTnnneKn

---

