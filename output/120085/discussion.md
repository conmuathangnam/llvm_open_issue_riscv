# clang crash on x86_64-linux-gnu: Assertion `getType(Base)->isPointerType() || getType(Base)->isArrayType()' failed

**Author:** bi6c
**URL:** https://github.com/llvm/llvm-project/issues/120085

## Body

Compiler Explorer: https://godbolt.org/z/c1Y8cGv93
```console
<source>:7:1: error: unknown type name 'uint32_t'
    7 | uint32_t abcdef_h[(1 << 16) / sizeof(uint32_t)]
      | ^
<source>:7:38: error: use of undeclared identifier 'uint32_t'
    7 | uint32_t abcdef_h[(1 << 16) / sizeof(uint32_t)]
      |                                      ^
<source>:7:48: error: expected ';' after top level declarator
    7 | uint32_t abcdef_h[(1 << 16) / sizeof(uint32_t)]
      |                                                ^
      |                                                ;
<source>:12:16: warning: unknown attribute 'noipa' ignored [-Wunknown-attributes]
   12 | __attribute__((noipa)) void sink (const void *p, ...) { (void)&p; }
      |                ^~~~~
clang: /root/llvm-project/clang/lib/AST/ExprConstant.cpp:1747: void {anonymous}::LValue::addUnsizedArray({anonymous}::EvalInfo&, const clang::Expr*, clang::QualType): Assertion `getType(Base)->isPointerType() || getType(Base)->isArrayType()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang -gdwarf-4 -g -o /app/output.s -fno-verbose-asm -c --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<source>:31:3: current parser token 'zero'
2.	<source>:17:1: parsing function body 'main'
3.	<source>:17:1: in compound statement ('{}')
 #0 0x0000000003c56df8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3c56df8)
 #1 0x0000000003c54b04 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3c54b04)
 #2 0x0000000003ba2068 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000071f0ade42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x000071f0ade969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x000071f0ade42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x000071f0ade287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x000071f0ade2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x000071f0ade39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000007723c02 (anonymous namespace)::LValue::addUnsizedArray((anonymous namespace)::EvalInfo&, clang::Expr const*, clang::QualType) ExprConstant.cpp:0:0
#10 0x000000000779c152 (anonymous namespace)::PointerExprEvaluator::VisitCastExpr(clang::CastExpr const*) ExprConstant.cpp:0:0
#11 0x0000000007750aeb clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::PointerExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#12 0x000000000779b768 (anonymous namespace)::PointerExprEvaluator::VisitCastExpr(clang::CastExpr const*) ExprConstant.cpp:0:0
#13 0x0000000007750aeb clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::PointerExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#14 0x0000000007752a2b EvaluatePointer(clang::Expr const*, (anonymous namespace)::LValue&, (anonymous namespace)::EvalInfo&, bool) ExprConstant.cpp:0:0
#15 0x000000000773ba5d Evaluate(clang::APValue&, (anonymous namespace)::EvalInfo&, clang::Expr const*) ExprConstant.cpp:0:0
#16 0x00000000077584a6 EvaluateInPlace(clang::APValue&, (anonymous namespace)::EvalInfo&, (anonymous namespace)::LValue const&, clang::Expr const*, bool) ExprConstant.cpp:0:0
#17 0x0000000007758e8d EvaluateCallArg(clang::ParmVarDecl const*, clang::Expr const*, (anonymous namespace)::CallRef, (anonymous namespace)::EvalInfo&, bool) ExprConstant.cpp:0:0
#18 0x00000000077590b2 EvaluateArgs(llvm::ArrayRef<clang::Expr const*>, (anonymous namespace)::CallRef, (anonymous namespace)::EvalInfo&, clang::FunctionDecl const*, bool) ExprConstant.cpp:0:0
#19 0x0000000007739154 (anonymous namespace)::VoidExprEvaluator::VisitCallExpr(clang::CallExpr const*) ExprConstant.cpp:0:0
#20 0x0000000007739b65 clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::VoidExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#21 0x000000000773c1c0 Evaluate(clang::APValue&, (anonymous namespace)::EvalInfo&, clang::Expr const*) ExprConstant.cpp:0:0
#22 0x000000000773e53f (anonymous namespace)::DataRecursiveIntBinOpEvaluator::process((anonymous namespace)::DataRecursiveIntBinOpEvaluator::EvalResult&) ExprConstant.cpp:0:0
#23 0x00000000077a04b3 (anonymous namespace)::IntExprEvaluator::VisitBinaryOperator(clang::BinaryOperator const*) ExprConstant.cpp:0:0
#24 0x000000000774d985 clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#25 0x000000000773bc16 Evaluate(clang::APValue&, (anonymous namespace)::EvalInfo&, clang::Expr const*) ExprConstant.cpp:0:0
#26 0x00000000077444eb EvaluateAsRValue((anonymous namespace)::EvalInfo&, clang::Expr const*, clang::APValue&) ExprConstant.cpp:0:0
#27 0x0000000007745c1c clang::Expr::EvaluateForOverflow(clang::ASTContext const&) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x7745c1c)
#28 0x0000000006803644 clang::Sema::CheckForIntOverflow(clang::Expr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6803644)
#29 0x00000000068337ad clang::Sema::CheckCompletedExpr(clang::Expr*, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x68337ad)
#30 0x0000000006bcda3c clang::Sema::ActOnFinishFullExpr(clang::Expr*, clang::SourceLocation, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6bcda3c)
#31 0x0000000006ede90e clang::Sema::ActOnExprStmt(clang::ActionResult<clang::Expr*, true>, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6ede90e)
#32 0x00000000066cf2b4 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x66cf2b4)
#33 0x00000000066c55ae clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x66c55ae)
#34 0x00000000066c6500 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x66c6500)
#35 0x00000000066c73c5 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x66c73c5)
#36 0x00000000066c8bca clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x66c8bca)
#37 0x00000000065db123 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x65db123)
#38 0x000000000660fadd clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x660fadd)
#39 0x00000000065ceece clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x65ceece)
#40 0x00000000065cf689 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x65cf689)
#41 0x00000000065d6e03 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x65d6e03)
#42 0x00000000065d7ccd clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x65d7ccd)
#43 0x00000000065ca20a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x65ca20a)
#44 0x00000000045e0668 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x45e0668)
#45 0x0000000004899a89 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4899a89)
#46 0x000000000481c48e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x481c48e)
#47 0x00000000049862ce clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x49862ce)
#48 0x0000000000ce1c7f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xce1c7f)
#49 0x0000000000cd971a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#50 0x0000000004624ad9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#51 0x0000000003ba2514 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3ba2514)
#52 0x00000000046250cf clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#53 0x00000000045ea3dd clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x45ea3dd)
#54 0x00000000045eb4cd clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x45eb4cd)
#55 0x00000000045f2a45 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x45f2a45)
#56 0x0000000000cdeac3 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xcdeac3)
#57 0x0000000000bac364 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xbac364)
#58 0x000071f0ade29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#59 0x000071f0ade29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#60 0x0000000000cd91c5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xcd91c5)
clang: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
Code:
```c

#define AB_D "ab\0d"
#define ABCDEF_H "abcdef\0h"
#define ABCDEFGHIJKLMN_P "abcdefghijklmn\0p"

char ab_d[] = AB_D;
uint32_t abcdef_h[(1 << 16) / sizeof(uint32_t)]
char abcdef_h[] ;

extern int strncmp (const char*, const char*, __SIZE_TYPE__);

__attribute__((noipa)) void sink (const void *p, ...) { (void)&p; }

#define strncmp(a, b, n) (sink (a, b), strncmp (a, b, n))

int main (void)
{
  int zero = 0;

  zero += strncmp (ab_d, AB_D, 1);
  zero += strncmp (ab_d, AB_D, 2);
  zero += strncmp (ab_d, AB_D, 3);
  zero += strncmp (ab_d, AB_D, 4);
  zero += strncmp (ab_d, AB_D, 5);

  zero += strncmp (ab_d, ABCDEF_H, 1);
  zero += strncmp (ab_d, ABCDEF_H, 2);

  zero += strncmp (abcdef_h, AB_D, 2);

  zero += strncmp (abcdef_h, ABCDEF_H, 2);
  zero += strncmp (abcdef_h, ABCDEF_H, 3);
  zero += strncmp (abcdef_h, ABCDEF_H, 4);
  zero += strncmp (abcdef_h, ABCDEF_H, 5);
  zero += strncmp (abcdef_h, ABCDEF_H, 6);
  zero += strncmp (abcdef_h, ABCDEF_H, 7);
  zero += strncmp (abcdef_h, ABCDEF_H, 8);
  zero += strncmp (abcdef_h, ABCDEF_H, 9);

  if (zero != 0)
    __builtin_abort ();

  int neg = 0;

  neg -= strncmp (ab_d, ABCDEF_H, 3) < 0;
  neg -= strncmp (ab_d, ABCDEF_H, 4) < 0;
  neg -= strncmp (ab_d, ABCDEF_H, 5) < 0;
  neg -= strncmp (ab_d, ABCDEF_H, 6) < 0;
  neg -= strncmp (ab_d, ABCDEF_H, 7) < 0;
  neg -= strncmp (ab_d, ABCDEF_H, 8) < 0;
  neg -= strncmp (ab_d, ABCDEF_H, 9) < 0;

  if (neg != -7)
    __builtin_abort ();
}
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (bi6c)

<details>
Compiler Explorer: https://godbolt.org/z/c1Y8cGv93
```console
&lt;source&gt;:7:1: error: unknown type name 'uint32_t'
    7 | uint32_t abcdef_h[(1 &lt;&lt; 16) / sizeof(uint32_t)]
      | ^
&lt;source&gt;:7:38: error: use of undeclared identifier 'uint32_t'
    7 | uint32_t abcdef_h[(1 &lt;&lt; 16) / sizeof(uint32_t)]
      |                                      ^
&lt;source&gt;:7:48: error: expected ';' after top level declarator
    7 | uint32_t abcdef_h[(1 &lt;&lt; 16) / sizeof(uint32_t)]
      |                                                ^
      |                                                ;
&lt;source&gt;:12:16: warning: unknown attribute 'noipa' ignored [-Wunknown-attributes]
   12 | __attribute__((noipa)) void sink (const void *p, ...) { (void)&amp;p; }
      |                ^~~~~
clang: /root/llvm-project/clang/lib/AST/ExprConstant.cpp:1747: void {anonymous}::LValue::addUnsizedArray({anonymous}::EvalInfo&amp;, const clang::Expr*, clang::QualType): Assertion `getType(Base)-&gt;isPointerType() || getType(Base)-&gt;isArrayType()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang -gdwarf-4 -g -o /app/output.s -fno-verbose-asm -c --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics &lt;source&gt;
1.	&lt;source&gt;:31:3: current parser token 'zero'
2.	&lt;source&gt;:17:1: parsing function body 'main'
3.	&lt;source&gt;:17:1: in compound statement ('{}')
 #<!-- -->0 0x0000000003c56df8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3c56df8)
 #<!-- -->1 0x0000000003c54b04 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3c54b04)
 #<!-- -->2 0x0000000003ba2068 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x000071f0ade42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x000071f0ade969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x000071f0ade42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x000071f0ade287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x000071f0ade2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x000071f0ade39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000007723c02 (anonymous namespace)::LValue::addUnsizedArray((anonymous namespace)::EvalInfo&amp;, clang::Expr const*, clang::QualType) ExprConstant.cpp:0:0
#<!-- -->10 0x000000000779c152 (anonymous namespace)::PointerExprEvaluator::VisitCastExpr(clang::CastExpr const*) ExprConstant.cpp:0:0
#<!-- -->11 0x0000000007750aeb clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::PointerExprEvaluator, bool&gt;::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#<!-- -->12 0x000000000779b768 (anonymous namespace)::PointerExprEvaluator::VisitCastExpr(clang::CastExpr const*) ExprConstant.cpp:0:0
#<!-- -->13 0x0000000007750aeb clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::PointerExprEvaluator, bool&gt;::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#<!-- -->14 0x0000000007752a2b EvaluatePointer(clang::Expr const*, (anonymous namespace)::LValue&amp;, (anonymous namespace)::EvalInfo&amp;, bool) ExprConstant.cpp:0:0
#<!-- -->15 0x000000000773ba5d Evaluate(clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Expr const*) ExprConstant.cpp:0:0
#<!-- -->16 0x00000000077584a6 EvaluateInPlace(clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;, (anonymous namespace)::LValue const&amp;, clang::Expr const*, bool) ExprConstant.cpp:0:0
#<!-- -->17 0x0000000007758e8d EvaluateCallArg(clang::ParmVarDecl const*, clang::Expr const*, (anonymous namespace)::CallRef, (anonymous namespace)::EvalInfo&amp;, bool) ExprConstant.cpp:0:0
#<!-- -->18 0x00000000077590b2 EvaluateArgs(llvm::ArrayRef&lt;clang::Expr const*&gt;, (anonymous namespace)::CallRef, (anonymous namespace)::EvalInfo&amp;, clang::FunctionDecl const*, bool) ExprConstant.cpp:0:0
#<!-- -->19 0x0000000007739154 (anonymous namespace)::VoidExprEvaluator::VisitCallExpr(clang::CallExpr const*) ExprConstant.cpp:0:0
#<!-- -->20 0x0000000007739b65 clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::VoidExprEvaluator, bool&gt;::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#<!-- -->21 0x000000000773c1c0 Evaluate(clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Expr const*) ExprConstant.cpp:0:0
#<!-- -->22 0x000000000773e53f (anonymous namespace)::DataRecursiveIntBinOpEvaluator::process((anonymous namespace)::DataRecursiveIntBinOpEvaluator::EvalResult&amp;) ExprConstant.cpp:0:0
#<!-- -->23 0x00000000077a04b3 (anonymous namespace)::IntExprEvaluator::VisitBinaryOperator(clang::BinaryOperator const*) ExprConstant.cpp:0:0
#<!-- -->24 0x000000000774d985 clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool&gt;::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#<!-- -->25 0x000000000773bc16 Evaluate(clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Expr const*) ExprConstant.cpp:0:0
#<!-- -->26 0x00000000077444eb EvaluateAsRValue((anonymous namespace)::EvalInfo&amp;, clang::Expr const*, clang::APValue&amp;) ExprConstant.cpp:0:0
#<!-- -->27 0x0000000007745c1c clang::Expr::EvaluateForOverflow(clang::ASTContext const&amp;) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x7745c1c)
#<!-- -->28 0x0000000006803644 clang::Sema::CheckForIntOverflow(clang::Expr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6803644)
#<!-- -->29 0x00000000068337ad clang::Sema::CheckCompletedExpr(clang::Expr*, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x68337ad)
#<!-- -->30 0x0000000006bcda3c clang::Sema::ActOnFinishFullExpr(clang::Expr*, clang::SourceLocation, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6bcda3c)
#<!-- -->31 0x0000000006ede90e clang::Sema::ActOnExprStmt(clang::ActionResult&lt;clang::Expr*, true&gt;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6ede90e)
#<!-- -->32 0x00000000066cf2b4 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x66cf2b4)
#<!-- -->33 0x00000000066c55ae clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x66c55ae)
#<!-- -->34 0x00000000066c6500 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x66c6500)
#<!-- -->35 0x00000000066c73c5 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x66c73c5)
#<!-- -->36 0x00000000066c8bca clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x66c8bca)
#<!-- -->37 0x00000000065db123 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x65db123)
#<!-- -->38 0x000000000660fadd clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x660fadd)
#<!-- -->39 0x00000000065ceece clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x65ceece)
#<!-- -->40 0x00000000065cf689 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x65cf689)
#<!-- -->41 0x00000000065d6e03 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x65d6e03)
#<!-- -->42 0x00000000065d7ccd clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x65d7ccd)
#<!-- -->43 0x00000000065ca20a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x65ca20a)
#<!-- -->44 0x00000000045e0668 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x45e0668)
#<!-- -->45 0x0000000004899a89 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4899a89)
#<!-- -->46 0x000000000481c48e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x481c48e)
#<!-- -->47 0x00000000049862ce clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x49862ce)
#<!-- -->48 0x0000000000ce1c7f cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xce1c7f)
#<!-- -->49 0x0000000000cd971a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->50 0x0000000004624ad9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->51 0x0000000003ba2514 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3ba2514)
#<!-- -->52 0x00000000046250cf clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->53 0x00000000045ea3dd clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x45ea3dd)
#<!-- -->54 0x00000000045eb4cd clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x45eb4cd)
#<!-- -->55 0x00000000045f2a45 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x45f2a45)
#<!-- -->56 0x0000000000cdeac3 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xcdeac3)
#<!-- -->57 0x0000000000bac364 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xbac364)
#<!-- -->58 0x000071f0ade29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->59 0x000071f0ade29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->60 0x0000000000cd91c5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xcd91c5)
clang: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
Code:
```c

#define AB_D "ab\0d"
#define ABCDEF_H "abcdef\0h"
#define ABCDEFGHIJKLMN_P "abcdefghijklmn\0p"

char ab_d[] = AB_D;
uint32_t abcdef_h[(1 &lt;&lt; 16) / sizeof(uint32_t)]
char abcdef_h[] ;

extern int strncmp (const char*, const char*, __SIZE_TYPE__);

__attribute__((noipa)) void sink (const void *p, ...) { (void)&amp;p; }

#define strncmp(a, b, n) (sink (a, b), strncmp (a, b, n))

int main (void)
{
  int zero = 0;

  zero += strncmp (ab_d, AB_D, 1);
  zero += strncmp (ab_d, AB_D, 2);
  zero += strncmp (ab_d, AB_D, 3);
  zero += strncmp (ab_d, AB_D, 4);
  zero += strncmp (ab_d, AB_D, 5);

  zero += strncmp (ab_d, ABCDEF_H, 1);
  zero += strncmp (ab_d, ABCDEF_H, 2);

  zero += strncmp (abcdef_h, AB_D, 2);

  zero += strncmp (abcdef_h, ABCDEF_H, 2);
  zero += strncmp (abcdef_h, ABCDEF_H, 3);
  zero += strncmp (abcdef_h, ABCDEF_H, 4);
  zero += strncmp (abcdef_h, ABCDEF_H, 5);
  zero += strncmp (abcdef_h, ABCDEF_H, 6);
  zero += strncmp (abcdef_h, ABCDEF_H, 7);
  zero += strncmp (abcdef_h, ABCDEF_H, 8);
  zero += strncmp (abcdef_h, ABCDEF_H, 9);

  if (zero != 0)
    __builtin_abort ();

  int neg = 0;

  neg -= strncmp (ab_d, ABCDEF_H, 3) &lt; 0;
  neg -= strncmp (ab_d, ABCDEF_H, 4) &lt; 0;
  neg -= strncmp (ab_d, ABCDEF_H, 5) &lt; 0;
  neg -= strncmp (ab_d, ABCDEF_H, 6) &lt; 0;
  neg -= strncmp (ab_d, ABCDEF_H, 7) &lt; 0;
  neg -= strncmp (ab_d, ABCDEF_H, 8) &lt; 0;
  neg -= strncmp (ab_d, ABCDEF_H, 9) &lt; 0;

  if (neg != -7)
    __builtin_abort ();
}
```
</details>


---

### Comment 2 - shafik

Goes back to clang-7: https://godbolt.org/z/zhcjhPrKx

may be related: https://github.com/llvm/llvm-project/issues/69468

---

