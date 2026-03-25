# [clang][bytecode] crash on strcat using pointer arithmetic

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/173175
**Status:** Closed
**Labels:** crash, clang:bytecode
**Closed Date:** 2025-12-24T08:30:18Z

## Body

Reproducer:
https://godbolt.org/z/qvsdoPjfK
```cpp
char *strcat(char *, const char *);
void foo(int, ...);
#define foo(...) foo(0, __VA_ARGS__)

extern char *c;

#define T(x) foo(0, (x))

void bar() { T(strcat(c, "42" + 3)); }
```

Backtrace:
```console
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x c -fexperimental-new-constant-interpreter <source>
1.	<source>:9:14 <Spelling=<source>:9:34>: current parser token ')'
2.	<source>:9:12: parsing function body 'bar'
3.	<source>:9:12: in compound statement ('{}')
 #0 0x0000000003ceafd8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3ceafd8)
 #1 0x0000000003ce89ac llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3ce89ac)
 #2 0x0000000003c2ff08 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007c4e39042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007c4e3919da05 (/lib/x86_64-linux-gnu/libc.so.6+0x19da05)
 #5 0x00000000074c1c8b bool llvm::function_ref<bool (clang::interp::Pointer const&)>::callback_fn<clang::interp::Context::evaluateStrlen(clang::interp::State&, clang::Expr const*, unsigned long&)::'lambda'(clang::interp::Pointer const&)>(long, clang::interp::Pointer const&) Context.cpp:0:0
 #6 0x00000000074ef97f bool clang::interp::EvalEmitter::emitRet<(clang::interp::PrimType)13>(clang::interp::SourceInfo) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x74ef97f)
 #7 0x000000000754a8a8 clang::interp::EvalEmitter::emitRet(clang::interp::PrimType, clang::interp::SourceInfo) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x754a8a8)
 #8 0x00000000078dd083 clang::interp::Compiler<clang::interp::EvalEmitter>::visitExpr(clang::Expr const*, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x78dd083)
 #9 0x00000000074d241b clang::interp::EvalEmitter::interpretAsPointer(clang::Expr const*, llvm::function_ref<bool (clang::interp::Pointer const&)>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x74d241b)
#10 0x00000000074c4e48 clang::interp::Context::evaluateStrlen(clang::interp::State&, clang::Expr const*, unsigned long&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x74c4e48)
#11 0x0000000007445c53 clang::Expr::tryEvaluateStrLen(unsigned long&, clang::ASTContext&) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x7445c53)
#12 0x000000000642f76b clang::Sema::checkFortifiedBuiltinMemoryFunction(clang::FunctionDecl*, clang::CallExpr*)::'lambda2'(unsigned int)::operator()(unsigned int) const (.constprop.0) SemaChecking.cpp:0:0
#13 0x0000000006448370 clang::Sema::checkFortifiedBuiltinMemoryFunction(clang::FunctionDecl*, clang::CallExpr*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6448370)
#14 0x0000000006783f27 clang::Sema::BuildResolvedCallExpr(clang::Expr*, clang::NamedDecl*, clang::SourceLocation, llvm::ArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, clang::CallExpr::ADLCallKind) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6783f27)
#15 0x000000000677f0ee clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x677f0ee)
#16 0x00000000067843fc clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x67843fc)
#17 0x0000000006279dad clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult<clang::Expr*, true>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6279dad)
#18 0x000000000627275a clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x627275a)
#19 0x00000000062749fa clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62749fa)
#20 0x0000000006274be9 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6274be9)
#21 0x0000000006279939 clang::Parser::ParseExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6279939)
#22 0x000000000627de05 clang::Parser::ParseParenExpression(clang::ParenParseOption&, bool, clang::ParenExprKind, clang::TypoCorrectionTypeBehavior, clang::OpaquePtr<clang::QualType>&, clang::SourceLocation&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x627de05)
#23 0x00000000062736ce clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62736ce)
#24 0x00000000062749fa clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62749fa)
#25 0x0000000006274be9 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6274be9)
#26 0x0000000006274e7e clang::Parser::ParseExpressionList(llvm::SmallVectorImpl<clang::Expr*>&, llvm::function_ref<void ()>, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6274e7e)
#27 0x000000000627a382 clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult<clang::Expr*, true>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x627a382)
#28 0x000000000627275a clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x627275a)
#29 0x00000000062749fa clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62749fa)
#30 0x0000000006274be9 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6274be9)
#31 0x0000000006279939 clang::Parser::ParseExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6279939)
#32 0x0000000006309da7 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6309da7)
#33 0x0000000006301e02 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::LabelDecl*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6301e02)
#34 0x0000000006302806 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6302806)
#35 0x000000000630a579 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x630a579)
#36 0x000000000630b01a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x630b01a)
#37 0x00000000061f602b clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61f602b)
#38 0x00000000062427ff clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62427ff)
#39 0x00000000061ef417 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61ef417)
#40 0x00000000061efbb7 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61efbb7)
#41 0x00000000061f857f clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61f857f)
#42 0x00000000061f9d87 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61f9d87)
#43 0x00000000061d2bca clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61d2bca)
#44 0x00000000045af6e5 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45af6e5)
#45 0x00000000048bd62a clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x48bd62a)
#46 0x000000000483b7bb clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x483b7bb)
#47 0x00000000049b399b clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x49b399b)
#48 0x0000000000dd8ebd cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdd8ebd)
#49 0x0000000000dd0d3b ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#50 0x0000000000dd0ddd int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#51 0x0000000004628b79 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#52 0x0000000003c30323 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c30323)
#53 0x0000000004628d99 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#54 0x00000000045ec9e2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45ec9e2)
#55 0x00000000045ed8c1 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45ed8c1)
#56 0x00000000045f608c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45f608c)
#57 0x0000000000dd5809 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdd5809)
#58 0x0000000000c86984 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc86984)
#59 0x00007c4e39029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#60 0x00007c4e39029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#61 0x0000000000dd07d5 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdd07d5)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```

## Comments

### Comment 1 - k-arrows

This is trunk only:
https://godbolt.org/z/7sbafas5d

---

### Comment 2 - ojhunt

Yup, I found this the other day but couldn't work out a repro, will extract the fix from my PR now that we have a test case that hits the logic :D

---

### Comment 3 - ojhunt

Oh interesting, this only repros when explicitly targeting c, not c++ - @tbaederr any idea what might cause that?


---

