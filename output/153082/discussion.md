# ICE with statement expression + builtin: Assertion failed: (!isValueDependent() && "Expression evaluator can't be called on a dependent expression.")

**Author:** comex
**URL:** https://github.com/llvm/llvm-project/issues/153082

## Body

As of 1a502da7084abf03f0ff3ba852b94ca217892730, this code:
```cpp
template <typename T> void f() {
  __builtin_expect_with_probability(0, 0, ({ 0; }));
}
```
produces:
```
% ~/src/llvm/build/bin/clang++ -fsyntax-only file.cpp
Assertion failed: (!isValueDependent() && "Expression evaluator can't be called on a dependent expression."), function EvaluateAsConstantExpr, file ExprConstant.cpp, line 17122.
```
WIthout the template, the function compiles successfully.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (comex)

<details>
As of 1a502da7084abf03f0ff3ba852b94ca217892730, this code:
```cpp
template &lt;typename T&gt; void f() {
  __builtin_expect_with_probability(0, 0, ({ 0; }));
}
```
produces:
```
% ~/src/llvm/build/bin/clang++ -fsyntax-only file.cpp
Assertion failed: (!isValueDependent() &amp;&amp; "Expression evaluator can't be called on a dependent expression."), function EvaluateAsConstantExpr, file ExprConstant.cpp, line 17122.
```
WIthout the template, the function compiles successfully.
</details>


---

### Comment 2 - efriedma-quic

It's probably easier to understand like this:

```
template <typename T> void f() {
  __builtin_expect_with_probability(0, 0, sizeof(T));
}
```

(Statement expressions are also value-dependent, but for less obvious reasons.)

---

### Comment 3 - shafik

This goes back to clang-11: https://godbolt.org/z/qYf6e1nW7

Backtrace:

```console
0.	Program arguments: /opt/compiler-explorer/clang-assertions-11.0.0/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-10.2.0 -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 <source> -isystem/opt/compiler-explorer/libs/fmt/trunk/include 
1.	<source>:2:52: current parser token ')'
2.	<source>:1:32: parsing function body 'f'
3.	<source>:1:32: in compound statement ('{}')
 #0 0x00005600638c1f7e llvm::sys::PrintStackTrace(llvm::raw_ostream&) (/opt/compiler-explorer/clang-assertions-11.0.0/bin/clang+++0x31d6f7e)
 #1 0x00005600638bfd54 llvm::sys::RunSignalHandlers() (/opt/compiler-explorer/clang-assertions-11.0.0/bin/clang+++0x31d4d54)
 #2 0x00005600638bffd1 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-11.0.0/bin/clang+++0x31d4fd1)
 #3 0x000056006382b9b8 CrashRecoverySignalHandler(int) (/opt/compiler-explorer/clang-assertions-11.0.0/bin/clang+++0x31409b8)
 #4 0x000071046d242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #5 0x000071046d2969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #6 0x000071046d242476 raise (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #7 0x000071046d2287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #8 0x000071046d22871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #9 0x000071046d239e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
#10 0x000056006658d417 clang::Expr::EvaluateAsConstantExpr(clang::Expr::EvalResult&, clang::Expr::ConstExprUsage, clang::ASTContext const&, bool) const (/opt/compiler-explorer/clang-assertions-11.0.0/bin/clang+++0x5ea2417)
#11 0x00005600659745e9 clang::Sema::CheckBuiltinFunctionCall(clang::FunctionDecl*, unsigned int, clang::CallExpr*) (/opt/compiler-explorer/clang-assertions-11.0.0/bin/clang+++0x52895e9)
#12 0x0000560065c2536b clang::Sema::BuildResolvedCallExpr(clang::Expr*, clang::NamedDecl*, clang::SourceLocation, llvm::ArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, clang::CallExpr::ADLCallKind) (/opt/compiler-explorer/clang-assertions-11.0.0/bin/clang+++0x553a36b)
#13 0x0000560065c2630b clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool) (/opt/compiler-explorer/clang-assertions-11.0.0/bin/clang+++0x553b30b)
#14 0x0000560065c277f3 clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-assertions-11.0.0/bin/clang+++0x553c7f3)
#15 0x00005600658413fb clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult<clang::Expr*, true>) (.localalias) (/opt/compiler-explorer/clang-assertions-11.0.0/bin/clang+++0x51563fb)
#16 0x000056006583a72c clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) (.localalias) (/opt/compiler-explorer/clang-assertions-11.0.0/bin/clang+++0x514f72c)
#17 0x000056006583d61b clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-11.0.0/bin/clang+++0x515261b)
#18 0x000056006583f599 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-assertions-11.0.0/bin/clang+++0x5154599)
#19 0x000056006584043d clang::Parser::ParseExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-assertions-11.0.0/bin/clang+++0x515543d)
#20 0x0000560065893bcf clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-11.0.0/bin/clang+++0x51a8bcf)
#21 0x00005600658929ea clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::Parser::ParsedAttributesWithRange&) (/opt/compiler-explorer/clang-assertions-11.0.0/bin/clang+++0x51a79ea)
#22 0x0000560065892c3c clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-11.0.0/bin/clang+++0x51a7c3c)
#23 0x00005600658973b9 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-11.0.0/bin/clang+++0x51ac3b9)
#24 0x0000560065899c0a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-11.0.0/bin/clang+++0x51aec0a)
#25 0x00005600657f6e5a clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-11.0.0/bin/clang+++0x510be5a)
#26 0x00005600658a7426 clang::Parser::ParseSingleDeclarationAfterTemplate(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo const&, clang::ParsingDeclRAIIObject&, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-11.0.0/bin/clang+++0x51bc426)
#27 0x00005600658aa13d clang::Parser::ParseTemplateDeclarationOrSpecialization(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (.part.0) (/opt/compiler-explorer/clang-assertions-11.0.0/bin/clang+++0x51bf13d)
#28 0x00005600658aa46f clang::Parser::ParseDeclarationStartingWithTemplate(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-11.0.0/bin/clang+++0x51bf46f)
#29 0x000056006581aa8e clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::Parser::ParsedAttributesWithRange&, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-11.0.0/bin/clang+++0x512fa8e)
#30 0x00005600657f52cc clang::Parser::ParseExternalDeclaration(clang::Parser::ParsedAttributesWithRange&, clang::ParsingDeclSpec*) (.localalias) (/opt/compiler-explorer/clang-assertions-11.0.0/bin/clang+++0x510a2cc)
#31 0x00005600657f5c78 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, bool) (/opt/compiler-explorer/clang-assertions-11.0.0/bin/clang+++0x510ac78)
#32 0x00005600657f613c clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&) (/opt/compiler-explorer/clang-assertions-11.0.0/bin/clang+++0x510b13c)
#33 0x00005600657ea9ba clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-11.0.0/bin/clang+++0x50ff9ba)
#34 0x000056006487bd38 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-11.0.0/bin/clang+++0x4190d38)
#35 0x00005600641a8e79 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-11.0.0/bin/clang+++0x3abde79)
#36 0x000056006415f846 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-11.0.0/bin/clang+++0x3a74846)
#37 0x000056006427d790 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-11.0.0/bin/clang+++0x3b92790)
#38 0x0000560061630c5c cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-11.0.0/bin/clang+++0xf45c5c)
#39 0x000056006162dd98 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&) (/opt/compiler-explorer/clang-assertions-11.0.0/bin/clang+++0xf42d98)
#40 0x000056006401f219 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<llvm::Optional<llvm::StringRef> >, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >*, bool*) const::'lambda'()>(long) (/opt/compiler-explorer/clang-assertions-11.0.0/bin/clang+++0x3934219)
#41 0x000056006382bb3c llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-11.0.0/bin/clang+++0x3140b3c)
#42 0x000056006401fb36 clang::driver::CC1Command::Execute(llvm::ArrayRef<llvm::Optional<llvm::StringRef> >, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >*, bool*) const (.part.0) (/opt/compiler-explorer/clang-assertions-11.0.0/bin/clang+++0x3934b36)
#43 0x0000560063ff710c clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&) const (/opt/compiler-explorer/clang-assertions-11.0.0/bin/clang+++0x390c10c)
#44 0x0000560063ff7a46 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*> >&) const (/opt/compiler-explorer/clang-assertions-11.0.0/bin/clang+++0x390ca46)
#45 0x0000560064000d49 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*> >&) (/opt/compiler-explorer/clang-assertions-11.0.0/bin/clang+++0x3915d49)
#46 0x000056006157a599 main (/opt/compiler-explorer/clang-assertions-11.0.0/bin/clang+++0xe8f599)
#47 0x000071046d229d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#48 0x000071046d229e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#49 0x000056006162d8ee _start (/opt/compiler-explorer/clang-assertions-11.0.0/bin/clang+++0xf428ee)
```

---

### Comment 4 - efriedma-quic

I don't really want to mark this a duplicate because it isn't the same codepath, even if it's closely related.

---

### Comment 5 - shafik

> I don't really want to mark this a duplicate because it isn't the same codepath, even if it's closely related.

Ahh, I missed the `getAsBuiltinConstantDeclRef` difference. I wish we had an automated way of comparing backtraces.

---

