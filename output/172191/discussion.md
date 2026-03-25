# [clang][bytecode] Assertion `!ItemTypes.empty()' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/172191
**Status:** Closed
**Labels:** crash, clang:bytecode
**Closed Date:** 2026-01-02T08:48:43Z

## Body

Reproducer:
https://godbolt.org/z/vchK9eKM4
```cpp
typedef __SIZE_TYPE__ size_t;

int main(int argc, char *argv[]) {

  if ((__extension__(
          0 ? __extension__({ (1 ? 0 : (__builtin_constant_p("plane"))); })
            : 0)) == 0) {
  }

  return (0);
}
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/AST/ByteCode/InterpStack.h:40: T clang::interp::InterpStack::pop() [with T = clang::interp::Pointer]: Assertion `!ItemTypes.empty()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -fexperimental-new-constant-interpreter <source>
1.	<source>:7:23: current parser token ')'
2.	<source>:3:34: parsing function body 'main'
3.	<source>:3:34: in compound statement ('{}')
 #0 0x00000000042bb738 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42bb738)
 #1 0x00000000042b8b64 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42b8b64)
 #2 0x00000000041fd948 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007a9f41e42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007a9f41e969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007a9f41e42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007a9f41e287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007a9f41e2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007a9f41e39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000007d419fe clang::interp::Pointer clang::interp::InterpStack::pop<clang::interp::Pointer>() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7d419fe)
#10 0x0000000007deeb8c clang::interp::EvalEmitter::speculate(clang::CallExpr const*, unsigned int const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7deeb8c)
#11 0x00000000081bd7d9 clang::interp::Compiler<clang::interp::EvalEmitter>::VisitBuiltinCallExpr(clang::CallExpr const*, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x81bd7d9)
#12 0x00000000081bdc2e clang::interp::Compiler<clang::interp::EvalEmitter>::VisitCallExpr(clang::CallExpr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x81bdc2e)
#13 0x00000000081ca9a7 clang::interp::Compiler<clang::interp::EvalEmitter>::VisitAbstractConditionalOperator(clang::AbstractConditionalOperator const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x81ca9a7)
#14 0x00000000081cbf6b clang::interp::Compiler<clang::interp::EvalEmitter>::VisitStmtExpr(clang::StmtExpr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x81cbf6b)
#15 0x00000000081c964b clang::interp::Compiler<clang::interp::EvalEmitter>::VisitUnaryOperator(clang::UnaryOperator const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x81c964b)
#16 0x00000000081ca970 clang::interp::Compiler<clang::interp::EvalEmitter>::VisitAbstractConditionalOperator(clang::AbstractConditionalOperator const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x81ca970)
#17 0x00000000081c964b clang::interp::Compiler<clang::interp::EvalEmitter>::VisitUnaryOperator(clang::UnaryOperator const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x81c964b)
#18 0x00000000081c521a clang::interp::Compiler<clang::interp::EvalEmitter>::visit(clang::Expr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x81c521a)
#19 0x00000000081b9656 clang::interp::Compiler<clang::interp::EvalEmitter>::visitExpr(clang::Expr const*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x81b9656)
#20 0x0000000007d3a35d clang::interp::EvalEmitter::interpretExpr(clang::Expr const*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7d3a35d)
#21 0x0000000007d29c63 clang::interp::Context::evaluateAsRValue(clang::interp::State&, clang::Expr const*, clang::APValue&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7d29c63)
#22 0x0000000007c8fae3 EvaluateAsRValue((anonymous namespace)::EvalInfo&, clang::Expr const*, clang::APValue&) ExprConstant.cpp:0:0
#23 0x0000000007c9438f clang::Expr::isCXX11ConstantExpr(clang::ASTContext const&, clang::APValue*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7c9438f)
#24 0x0000000007c94850 EvaluateCPlusPlus11IntegralConstantExpr(clang::ASTContext const&, clang::Expr const*, llvm::APSInt*) ExprConstant.cpp:0:0
#25 0x0000000007c96f1f clang::Expr::getIntegerConstantExpr(clang::ASTContext const&) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7c96f1f)
#26 0x0000000006c52f57 AnalyzeImplicitConversions(clang::Sema&, clang::Expr*, clang::SourceLocation, bool) (.constprop.0) SemaChecking.cpp:0:0
#27 0x0000000006c53b95 clang::Sema::CheckCompletedExpr(clang::Expr*, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c53b95)
#28 0x0000000006ff8147 clang::Sema::ActOnFinishFullExpr(clang::Expr*, clang::SourceLocation, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ff8147)
#29 0x0000000006f447d0 clang::Sema::ActOnCondition(clang::Scope*, clang::SourceLocation, clang::Expr*, clang::Sema::ConditionKind, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f447d0)
#30 0x0000000006a63a7c clang::Parser::ParseCXXCondition(clang::ActionResult<clang::Stmt*, true>*, clang::SourceLocation, clang::Sema::ConditionKind, bool, clang::Parser::ForRangeInfo*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a63a7c)
#31 0x0000000006acc42b clang::Parser::ParseParenExprOrCondition(clang::ActionResult<clang::Stmt*, true>*, clang::Sema::ConditionResult&, clang::SourceLocation, clang::Sema::ConditionKind, clang::SourceLocation&, clang::SourceLocation&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6acc42b)
#32 0x0000000006ad1226 clang::Parser::ParseIfStatement(clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ad1226)
#33 0x0000000006aced6d clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6aced6d)
#34 0x0000000006acfc66 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6acfc66)
#35 0x0000000006ad85ae clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ad85ae)
#36 0x0000000006ad8dfa clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ad8dfa)
#37 0x00000000069dea4b clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69dea4b)
#38 0x0000000006a14f0d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a14f0d)
#39 0x00000000069d1b9b clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69d1b9b)
#40 0x00000000069d238f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69d238f)
#41 0x00000000069da1ca clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69da1ca)
#42 0x00000000069db165 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69db165)
#43 0x00000000069bc1da clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69bc1da)
#44 0x0000000004c0bb78 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c0bb78)
#45 0x0000000004f01215 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f01215)
#46 0x0000000004e8148e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e8148e)
#47 0x0000000004ffb2cd clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ffb2cd)
#48 0x0000000000dcf2cc cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdcf2cc)
#49 0x0000000000dc5d2a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#50 0x0000000000dc5ead int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#51 0x0000000004c7e7a9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#52 0x00000000041fdde4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41fdde4)
#53 0x0000000004c7edbf clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#54 0x0000000004c3f5b2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c3f5b2)
#55 0x0000000004c4055e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c4055e)
#56 0x0000000004c47995 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c47995)
#57 0x0000000000dcb6d1 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdcb6d1)
#58 0x0000000000c77384 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc77384)
#59 0x00007a9f41e29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#60 0x00007a9f41e29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#61 0x0000000000dc57c5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdc57c5)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - k-arrows

Goes back to clang-21.1.0:
https://godbolt.org/z/9bn7b5qxn

---

