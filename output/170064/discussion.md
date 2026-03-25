# [clang][bytecode] Assertion `ItemTypes.back() == toPrimType<T>()' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/170064
**Status:** Closed
**Labels:** crash, clang:bytecode
**Closed Date:** 2025-12-01T10:33:34Z

## Body

Reproducer:
https://godbolt.org/z/xfoPhfKeq
```c
#define ATTR(...) __attribute__((__VA_ARGS__))
#define ALIGN(N) ATTR(aligned(N))

#define Assert(expr) typedef int _Assert[1 - 2 * !(expr)]

#define A3(expect, expr, align)                                                \
  do {                                                                         \
    Assert(expect == __builtin_has_attribute(expr, aligned(align)));           \
  } while (0)

typedef struct S64 {
  char a[64];
} S64;
typedef ALIGN(64) S64 I64;
typedef ALIGN(32) I64 A32_I64[3];

void foo(int i) { A3(0, ((I64){}, (A32_I64){}), 32); }
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/AST/ByteCode/InterpStack.h:64: T& clang::interp::InterpStack::peek() const [with T = clang::interp::Pointer]: Assertion `ItemTypes.back() == toPrimType<T>()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x c -fexperimental-new-constant-interpreter <source>
1.	<source>:17:19 <Spelling=<source>:8:68>: current parser token ';'
2.	<source>:17:17: parsing function body 'foo'
3.	<source>:17:17: in compound statement ('{}')
4.	<source>:17:19 <Spelling=<source>:7:6>: in compound statement ('{}')
 #0 0x0000000004279728 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4279728)
 #1 0x0000000004276b54 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4276b54)
 #2 0x00000000041ba888 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x0000757005042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007570050969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x0000757005042476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007570050287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x000075700502871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x0000757005039e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000007cdd284 (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7cdd284)
#10 0x0000000007d55e3c bool clang::interp::InitElem<(clang::interp::PrimType)0, clang::interp::Integral<8u, true>>(clang::interp::InterpState&, clang::interp::CodePtr, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7d55e3c)
#11 0x0000000008124686 clang::interp::Compiler<clang::interp::EvalEmitter>::visitZeroArrayInitializer(clang::QualType, clang::Expr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x8124686)
#12 0x000000000812acae clang::interp::Compiler<clang::interp::EvalEmitter>::VisitImplicitValueInitExpr(clang::ImplicitValueInitExpr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x812acae)
#13 0x0000000008154aeb clang::interp::Compiler<clang::interp::EvalEmitter>::discard(clang::Expr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x8154aeb)
#14 0x0000000008162fec clang::interp::Compiler<clang::interp::EvalEmitter>::visitInitList(llvm::ArrayRef<clang::Expr const*>, clang::Expr const*, clang::Expr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x8162fec)
#15 0x0000000008154aeb clang::interp::Compiler<clang::interp::EvalEmitter>::discard(clang::Expr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x8154aeb)
#16 0x000000000814e6a8 clang::interp::Compiler<clang::interp::EvalEmitter>::VisitCompoundLiteralExpr(clang::CompoundLiteralExpr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x814e6a8)
#17 0x000000000815c2b3 clang::interp::Compiler<clang::interp::EvalEmitter>::VisitCastExpr(clang::CastExpr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x815c2b3)
#18 0x0000000008154aeb clang::interp::Compiler<clang::interp::EvalEmitter>::discard(clang::Expr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x8154aeb)
#19 0x000000000815ea5b clang::interp::Compiler<clang::interp::EvalEmitter>::VisitBinaryOperator(clang::BinaryOperator const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x815ea5b)
#20 0x000000000815c0ea clang::interp::Compiler<clang::interp::EvalEmitter>::visit(clang::Expr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x815c0ea)
#21 0x000000000814edf4 clang::interp::Compiler<clang::interp::EvalEmitter>::visitCallArgs(llvm::ArrayRef<clang::Expr const*>, clang::FunctionDecl const*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x814edf4)
#22 0x0000000008155027 clang::interp::Compiler<clang::interp::EvalEmitter>::VisitCallExpr(clang::CallExpr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x8155027)
#23 0x000000000815c0ea clang::interp::Compiler<clang::interp::EvalEmitter>::visit(clang::Expr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x815c0ea)
#24 0x000000000815eb8e clang::interp::Compiler<clang::interp::EvalEmitter>::VisitBinaryOperator(clang::BinaryOperator const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x815eb8e)
#25 0x000000000815c0ea clang::interp::Compiler<clang::interp::EvalEmitter>::visit(clang::Expr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x815c0ea)
#26 0x000000000811e4e3 clang::interp::Compiler<clang::interp::EvalEmitter>::visitBool(clang::Expr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x811e4e3)
#27 0x0000000008160a43 clang::interp::Compiler<clang::interp::EvalEmitter>::VisitUnaryOperator(clang::UnaryOperator const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x8160a43)
#28 0x000000000815c0ea clang::interp::Compiler<clang::interp::EvalEmitter>::visit(clang::Expr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x815c0ea)
#29 0x000000000815eb8e clang::interp::Compiler<clang::interp::EvalEmitter>::VisitBinaryOperator(clang::BinaryOperator const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x815eb8e)
#30 0x000000000815c0ea clang::interp::Compiler<clang::interp::EvalEmitter>::visit(clang::Expr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x815c0ea)
#31 0x000000000815eb8e clang::interp::Compiler<clang::interp::EvalEmitter>::VisitBinaryOperator(clang::BinaryOperator const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x815eb8e)
#32 0x000000000815c0ea clang::interp::Compiler<clang::interp::EvalEmitter>::visit(clang::Expr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x815c0ea)
#33 0x0000000008150556 clang::interp::Compiler<clang::interp::EvalEmitter>::visitExpr(clang::Expr const*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x8150556)
#34 0x0000000007cd2fad clang::interp::EvalEmitter::interpretExpr(clang::Expr const*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7cd2fad)
#35 0x0000000007cc28b3 clang::interp::Context::evaluateAsRValue(clang::interp::State&, clang::Expr const*, clang::APValue&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7cc28b3)
#36 0x0000000007c296a3 EvaluateAsRValue((anonymous namespace)::EvalInfo&, clang::Expr const*, clang::APValue&) ExprConstant.cpp:0:0
#37 0x0000000007c2e977 clang::Expr::EvaluateAsRValue(clang::Expr::EvalResult&, clang::ASTContext const&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7c2e977)
#38 0x0000000006ed0ca1 clang::Sema::VerifyIntegerConstantExpression(clang::Expr*, llvm::APSInt*, clang::Sema::VerifyICEDiagnoser&, clang::AllowFoldKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ed0ca1)
#39 0x00000000075b59a1 clang::Sema::BuildArrayType(clang::QualType, clang::ArraySizeModifier, clang::Expr*, unsigned int, clang::SourceRange, clang::DeclarationName) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x75b59a1)
#40 0x00000000075d284b GetFullTypeForDeclarator((anonymous namespace)::TypeProcessingState&, clang::QualType, clang::TypeSourceInfo*) SemaType.cpp:0:0
#41 0x00000000075d84cf clang::Sema::GetTypeForDeclarator(clang::Declarator&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x75d84cf)
#42 0x0000000006d6fdfc clang::Sema::HandleDeclarator(clang::Scope*, clang::Declarator&, llvm::MutableArrayRef<clang::TemplateParameterList*>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d6fdfc)
#43 0x0000000006d71140 clang::Sema::ActOnDeclarator(clang::Scope*, clang::Declarator&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d71140)
#44 0x00000000069aa743 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69aa743)
#45 0x00000000069b84ba clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69b84ba)
#46 0x00000000069c11cf clang::Parser::ParseSimpleDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, bool, clang::Parser::ForRangeInit*, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69c11cf)
#47 0x00000000069c169c clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69c169c)
#48 0x0000000006a7341a clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a7341a)
#49 0x0000000006a73c76 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a73c76)
#50 0x0000000006a7c3fe clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a7c3fe)
#51 0x0000000006a7cb74 void llvm::function_ref<void ()>::callback_fn<clang::Parser::ParseCompoundStatement(bool, unsigned int)::'lambda'()>(long) ParseStmt.cpp:0:0
#52 0x0000000008322bb1 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x8322bb1)
#53 0x0000000006a6d795 clang::Parser::ParseCompoundStatement(bool, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a6d795)
#54 0x0000000006a723b7 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a723b7)
#55 0x0000000006a73c76 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a73c76)
#56 0x0000000006a74c9b clang::Parser::ParseStatement(clang::SourceLocation*, clang::Parser::ParsedStmtContext, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a74c9b)
#57 0x0000000006a774c4 clang::Parser::ParseDoStatement(clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a774c4)
#58 0x0000000006a72cb4 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a72cb4)
#59 0x0000000006a73c76 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a73c76)
#60 0x0000000006a7c3fe clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a7c3fe)
#61 0x0000000006a7cc4a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a7cc4a)
#62 0x0000000006982aab clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6982aab)
#63 0x00000000069b8ecd clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69b8ecd)
#64 0x000000000697625e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x697625e)
#65 0x00000000069769ff clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69769ff)
#66 0x000000000697e2b3 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x697e2b3)
#67 0x000000000697f1c5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x697f1c5)
#68 0x00000000069607aa clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69607aa)
#69 0x0000000004bc8be8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bc8be8)
#70 0x0000000004eb4455 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4eb4455)
#71 0x0000000004e3460e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e3460e)
#72 0x0000000004fad48d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4fad48d)
#73 0x0000000000dc7780 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdc7780)
#74 0x0000000000dbe1fa ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#75 0x0000000000dbe37d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#76 0x0000000004c313d9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#77 0x00000000041bad24 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41bad24)
#78 0x0000000004c319ef clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#79 0x0000000004bf21e2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bf21e2)
#80 0x0000000004bf318e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bf318e)
#81 0x0000000004bfa5c5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bfa5c5)
#82 0x0000000000dc3ba1 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdc3ba1)
#83 0x0000000000c72574 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc72574)
#84 0x0000757005029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#85 0x0000757005029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#86 0x0000000000dbdc95 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdbdc95)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - k-arrows

Goes back to clang-20.1.0:
https://godbolt.org/z/rnEbrzqGT

---

