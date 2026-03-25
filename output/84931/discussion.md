# [clang] [ICE] [crash-on-valid] comparing pointers to explicit-object member functions

**Author:** atomgalaxy
**URL:** https://github.com/llvm/llvm-project/issues/84931
**Status:** Closed
**Labels:** clang:frontend, confirmed, crash-on-valid
**Closed Date:** 2025-07-06T16:27:50Z

## Body

this test program is valid but ICEs clang-trunk:

```
// RUN: %clang_cc1 -std=c++2b -fsyntax-only -verify %s
// expected-no-diagnostics

struct S {
    void f(this S&);
    void f(this S&, int);
};
using f0_t = void (*)(S&);
constexpr f0_t f0 = &S::f;
using f1_t = void (*)(S&, int);
constexpr f1_t f1 = &S::f;
static_assert(f0 == f0); // ICE caused by this
static_assert(f0 != f1);
static_assert(f1 == f1);
```

The stack trace:

```
-- Testing: 1 tests, 1 workers --
FAIL: Clang :: SemaCXX/cxx23-explicit-object-member-ptr.cpp (1 of 1)
******************** TEST 'Clang :: SemaCXX/cxx23-explicit-object-member-ptr.cpp' FAILED ********************
Exit Code: 134

Command Output (stderr):
--
RUN: at line 1: /Users/atomicity/work/calltarget-llvm-root/bin/clang -cc1 -internal-isystem /Users/atomicity/work/calltarget-llvm-root/lib/clang/19/include -nostdsysteminc -std=c++2b -fsyntax-only -verify /Users/atomicity/work/calltarget-llvm-src/clang/test/SemaCXX/cxx23-explicit-object-member-ptr.cpp
+ /Users/atomicity/work/calltarget-llvm-root/bin/clang -cc1 -internal-isystem /Users/atomicity/work/calltarget-llvm-root/lib/clang/19/include -nostdsysteminc -std=c++2b -fsyntax-only -verify /Users/atomicity/work/calltarget-llvm-src/clang/test/SemaCXX/cxx23-explicit-object-member-ptr.cpp
Assertion failed: (V.isLValue() && "Setting LValue from a non-LValue?"), function setFrom, file ExprConstant.cpp, line 1634.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /Users/atomicity/work/calltarget-llvm-root/bin/clang -cc1 -internal-isystem /Users/atomicity/work/calltarget-llvm-root/lib/clang/19/include -nostdsysteminc -std=c++2b -fsyntax-only -verify /Users/atomicity/work/calltarget-llvm-src/clang/test/SemaCXX/cxx23-explicit-object-member-ptr.cpp
1.      /Users/atomicity/work/calltarget-llvm-src/clang/test/SemaCXX/cxx23-explicit-object-member-ptr.cpp:13:1: current parser token 'static_assert'
 #0 0x00000001031f9b1c llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/Users/atomicity/work/calltarget-llvm-root/bin/clang-19+0x102ca1b1c)
 #1 0x00000001031fa100 PrintStackTraceSignalHandler(void*) (/Users/atomicity/work/calltarget-llvm-root/bin/clang-19+0x102ca2100)
 #2 0x00000001031f7db8 llvm::sys::RunSignalHandlers() (/Users/atomicity/work/calltarget-llvm-root/bin/clang-19+0x102c9fdb8)
 #3 0x00000001031fb21c SignalHandler(int) (/Users/atomicity/work/calltarget-llvm-root/bin/clang-19+0x102ca321c)
 #4 0x0000000193ddb584 (/usr/lib/system/libsystem_platform.dylib+0x180477584)
 #5 0x0000000193daac20 (/usr/lib/system/libsystem_pthread.dylib+0x180446c20)
 #6 0x0000000193cb7a20 (/usr/lib/system/libsystem_c.dylib+0x180353a20)
 #7 0x0000000193cb6d10 (/usr/lib/system/libsystem_c.dylib+0x180352d10)
 #8 0x0000000109c83c58 (anonymous namespace)::LValue::setFrom(clang::ASTContext&, clang::APValue const&) (/Users/atomicity/work/calltarget-llvm-root/bin/clang-19+0x10972bc58)
 #9 0x0000000109d3a164 (anonymous namespace)::PointerExprEvaluator::Success(clang::APValue const&, clang::Expr const*) (/Users/atomicity/work/calltarget-llvm-root/bin/clang-19+0x1097e2164)
#10 0x0000000109d3b048 (anonymous namespace)::PointerExprEvaluator::VisitCastExpr(clang::CastExpr const*) (/Users/atomicity/work/calltarget-llvm-root/bin/clang-19+0x1097e3048)
#11 0x0000000109d369a8 clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::PointerExprEvaluator, bool>::VisitImplicitCastExpr(clang::ImplicitCastExpr const*) (/Users/atomicity/work/calltarget-llvm-root/bin/clang-19+0x1097de9a8)
#12 0x0000000109d32acc clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::PointerExprEvaluator, bool>::Visit(clang::Stmt const*) (/Users/atomicity/work/calltarget-llvm-root/bin/clang-19+0x1097daacc)
#13 0x0000000109c6d3d4 EvaluatePointer(clang::Expr const*, (anonymous namespace)::LValue&, (anonymous namespace)::EvalInfo&, bool) (/Users/atomicity/work/calltarget-llvm-root/bin/clang-19+0x1097153d4)
#14 0x0000000109d224bc bool EvaluateComparisonBinaryOperator<(anonymous namespace)::IntExprEvaluator::VisitBinaryOperator(clang::BinaryOperator const*)::$_5&, (anonymous namespace)::IntExprEvaluator::VisitBinaryOperator(clang::BinaryOperator const*)::$_6>((anonymous namespace)::EvalInfo&, clang::BinaryOperator const*, (anonymous namespace)::IntExprEvaluator::VisitBinaryOperator(clang::BinaryOperator const*)::$_5&, (anonymous namespace)::IntExprEvaluator::VisitBinaryOperator(clang::BinaryOperator const*)::$_6&&) (/Users/atomicity/work/calltarget-llvm-root/bin/clang-19+0x1097ca4bc)
#15 0x0000000109d15b58 (anonymous namespace)::IntExprEvaluator::VisitBinaryOperator(clang::BinaryOperator const*) (/Users/atomicity/work/calltarget-llvm-root/bin/clang-19+0x1097bdb58)
#16 0x0000000109d11d1c clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool>::VisitBinEQ(clang::BinaryOperator const*) (/Users/atomicity/work/calltarget-llvm-root/bin/clang-19+0x1097b9d1c)
#17 0x0000000109d04a20 clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool>::Visit(clang::Stmt const*) (/Users/atomicity/work/calltarget-llvm-root/bin/clang-19+0x1097aca20)
#18 0x0000000109c6b75c Evaluate(clang::APValue&, (anonymous namespace)::EvalInfo&, clang::Expr const*) (/Users/atomicity/work/calltarget-llvm-root/bin/clang-19+0x10971375c)
#19 0x0000000109c68604 EvaluateAsRValue((anonymous namespace)::EvalInfo&, clang::Expr const*, clang::APValue&) (/Users/atomicity/work/calltarget-llvm-root/bin/clang-19+0x109710604)
#20 0x0000000109c642d4 EvaluateAsRValue(clang::Expr const*, clang::Expr::EvalResult&, clang::ASTContext const&, (anonymous namespace)::EvalInfo&) (/Users/atomicity/work/calltarget-llvm-root/bin/clang-19+0x10970c2d4)
#21 0x0000000109c6415c clang::Expr::EvaluateAsRValue(clang::Expr::EvalResult&, clang::ASTContext const&, bool) const (/Users/atomicity/work/calltarget-llvm-root/bin/clang-19+0x10970c15c)
#22 0x000000010839b65c clang::Sema::VerifyIntegerConstantExpression(clang::Expr*, llvm::APSInt*, clang::Sema::VerifyICEDiagnoser&, clang::Sema::AllowFoldKind) (/Users/atomicity/work/calltarget-llvm-root/bin/clang-19+0x107e4365c)
#23 0x000000010839755c clang::Sema::VerifyIntegerConstantExpression(clang::Expr*, llvm::APSInt*, unsigned int, clang::Sema::AllowFoldKind) (/Users/atomicity/work/calltarget-llvm-root/bin/clang-19+0x107e3f55c)
#24 0x000000010811b188 clang::Sema::BuildStaticAssertDeclaration(clang::SourceLocation, clang::Expr*, clang::Expr*, clang::SourceLocation, bool) (/Users/atomicity/work/calltarget-llvm-root/bin/clang-19+0x107bc3188)
#25 0x000000010811af5c clang::Sema::ActOnStaticAssertDeclaration(clang::SourceLocation, clang::Expr*, clang::Expr*, clang::SourceLocation) (/Users/atomicity/work/calltarget-llvm-root/bin/clang-19+0x107bc2f5c)
#26 0x00000001078080cc clang::Parser::ParseStaticAssertDeclaration(clang::SourceLocation&) (/Users/atomicity/work/calltarget-llvm-root/bin/clang-19+0x1072b00cc)
#27 0x00000001077d39d8 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/Users/atomicity/work/calltarget-llvm-root/bin/clang-19+0x10727b9d8)
#28 0x00000001078ea63c clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/Users/atomicity/work/calltarget-llvm-root/bin/clang-19+0x10739263c)
#29 0x00000001078e8b54 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/Users/atomicity/work/calltarget-llvm-root/bin/clang-19+0x107390b54)
#30 0x00000001077ba7fc clang::ParseAST(clang::Sema&, bool, bool) (/Users/atomicity/work/calltarget-llvm-root/bin/clang-19+0x1072627fc)
#31 0x0000000104985e40 clang::ASTFrontendAction::ExecuteAction() (/Users/atomicity/work/calltarget-llvm-root/bin/clang-19+0x10442de40)
#32 0x000000010498560c clang::FrontendAction::Execute() (/Users/atomicity/work/calltarget-llvm-root/bin/clang-19+0x10442d60c)
#33 0x00000001048a5f08 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/Users/atomicity/work/calltarget-llvm-root/bin/clang-19+0x10434df08)
#34 0x0000000104aa9840 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/Users/atomicity/work/calltarget-llvm-root/bin/clang-19+0x104551840)
#35 0x000000010056f0b8 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/Users/atomicity/work/calltarget-llvm-root/bin/clang-19+0x1000170b8)
#36 0x000000010055dbf4 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) (/Users/atomicity/work/calltarget-llvm-root/bin/clang-19+0x100005bf4)
#37 0x000000010055c940 clang_main(int, char**, llvm::ToolContext const&) (/Users/atomicity/work/calltarget-llvm-root/bin/clang-19+0x100004940)
```

This is clang-trunk, 9277a32305c1083653ffaa7955cd26deffc10988

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Gašper Ažman (atomgalaxy)

<details>
this test program is valid but ICEs clang-trunk:

```
// RUN: %clang_cc1 -std=c++2b -fsyntax-only -verify %s
// expected-no-diagnostics

struct S {
    void f(this S&amp;);
    void f(this S&amp;, int);
};
using f0_t = void (*)(S&amp;);
constexpr f0_t f0 = &amp;S::f;
using f1_t = void (*)(S&amp;, int);
constexpr f1_t f1 = &amp;S::f;
static_assert(f0 == f0); // ICE caused by this
static_assert(f0 != f1);
static_assert(f1 == f1);
```

The stack trace:

```
-- Testing: 1 tests, 1 workers --
FAIL: Clang :: SemaCXX/cxx23-explicit-object-member-ptr.cpp (1 of 1)
******************** TEST 'Clang :: SemaCXX/cxx23-explicit-object-member-ptr.cpp' FAILED ********************
Exit Code: 134

Command Output (stderr):
--
RUN: at line 1: /Users/atomicity/work/calltarget-llvm-root/bin/clang -cc1 -internal-isystem /Users/atomicity/work/calltarget-llvm-root/lib/clang/19/include -nostdsysteminc -std=c++2b -fsyntax-only -verify /Users/atomicity/work/calltarget-llvm-src/clang/test/SemaCXX/cxx23-explicit-object-member-ptr.cpp
+ /Users/atomicity/work/calltarget-llvm-root/bin/clang -cc1 -internal-isystem /Users/atomicity/work/calltarget-llvm-root/lib/clang/19/include -nostdsysteminc -std=c++2b -fsyntax-only -verify /Users/atomicity/work/calltarget-llvm-src/clang/test/SemaCXX/cxx23-explicit-object-member-ptr.cpp
Assertion failed: (V.isLValue() &amp;&amp; "Setting LValue from a non-LValue?"), function setFrom, file ExprConstant.cpp, line 1634.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /Users/atomicity/work/calltarget-llvm-root/bin/clang -cc1 -internal-isystem /Users/atomicity/work/calltarget-llvm-root/lib/clang/19/include -nostdsysteminc -std=c++2b -fsyntax-only -verify /Users/atomicity/work/calltarget-llvm-src/clang/test/SemaCXX/cxx23-explicit-object-member-ptr.cpp
1.      /Users/atomicity/work/calltarget-llvm-src/clang/test/SemaCXX/cxx23-explicit-object-member-ptr.cpp:13:1: current parser token 'static_assert'
 #<!-- -->0 0x00000001031f9b1c llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/Users/atomicity/work/calltarget-llvm-root/bin/clang-19+0x102ca1b1c)
 #<!-- -->1 0x00000001031fa100 PrintStackTraceSignalHandler(void*) (/Users/atomicity/work/calltarget-llvm-root/bin/clang-19+0x102ca2100)
 #<!-- -->2 0x00000001031f7db8 llvm::sys::RunSignalHandlers() (/Users/atomicity/work/calltarget-llvm-root/bin/clang-19+0x102c9fdb8)
 #<!-- -->3 0x00000001031fb21c SignalHandler(int) (/Users/atomicity/work/calltarget-llvm-root/bin/clang-19+0x102ca321c)
 #<!-- -->4 0x0000000193ddb584 (/usr/lib/system/libsystem_platform.dylib+0x180477584)
 #<!-- -->5 0x0000000193daac20 (/usr/lib/system/libsystem_pthread.dylib+0x180446c20)
 #<!-- -->6 0x0000000193cb7a20 (/usr/lib/system/libsystem_c.dylib+0x180353a20)
 #<!-- -->7 0x0000000193cb6d10 (/usr/lib/system/libsystem_c.dylib+0x180352d10)
 #<!-- -->8 0x0000000109c83c58 (anonymous namespace)::LValue::setFrom(clang::ASTContext&amp;, clang::APValue const&amp;) (/Users/atomicity/work/calltarget-llvm-root/bin/clang-19+0x10972bc58)
 #<!-- -->9 0x0000000109d3a164 (anonymous namespace)::PointerExprEvaluator::Success(clang::APValue const&amp;, clang::Expr const*) (/Users/atomicity/work/calltarget-llvm-root/bin/clang-19+0x1097e2164)
#<!-- -->10 0x0000000109d3b048 (anonymous namespace)::PointerExprEvaluator::VisitCastExpr(clang::CastExpr const*) (/Users/atomicity/work/calltarget-llvm-root/bin/clang-19+0x1097e3048)
#<!-- -->11 0x0000000109d369a8 clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::PointerExprEvaluator, bool&gt;::VisitImplicitCastExpr(clang::ImplicitCastExpr const*) (/Users/atomicity/work/calltarget-llvm-root/bin/clang-19+0x1097de9a8)
#<!-- -->12 0x0000000109d32acc clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::PointerExprEvaluator, bool&gt;::Visit(clang::Stmt const*) (/Users/atomicity/work/calltarget-llvm-root/bin/clang-19+0x1097daacc)
#<!-- -->13 0x0000000109c6d3d4 EvaluatePointer(clang::Expr const*, (anonymous namespace)::LValue&amp;, (anonymous namespace)::EvalInfo&amp;, bool) (/Users/atomicity/work/calltarget-llvm-root/bin/clang-19+0x1097153d4)
#<!-- -->14 0x0000000109d224bc bool EvaluateComparisonBinaryOperator&lt;(anonymous namespace)::IntExprEvaluator::VisitBinaryOperator(clang::BinaryOperator const*)::$_5&amp;, (anonymous namespace)::IntExprEvaluator::VisitBinaryOperator(clang::BinaryOperator const*)::$_6&gt;((anonymous namespace)::EvalInfo&amp;, clang::BinaryOperator const*, (anonymous namespace)::IntExprEvaluator::VisitBinaryOperator(clang::BinaryOperator const*)::$_5&amp;, (anonymous namespace)::IntExprEvaluator::VisitBinaryOperator(clang::BinaryOperator const*)::$_6&amp;&amp;) (/Users/atomicity/work/calltarget-llvm-root/bin/clang-19+0x1097ca4bc)
#<!-- -->15 0x0000000109d15b58 (anonymous namespace)::IntExprEvaluator::VisitBinaryOperator(clang::BinaryOperator const*) (/Users/atomicity/work/calltarget-llvm-root/bin/clang-19+0x1097bdb58)
#<!-- -->16 0x0000000109d11d1c clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool&gt;::VisitBinEQ(clang::BinaryOperator const*) (/Users/atomicity/work/calltarget-llvm-root/bin/clang-19+0x1097b9d1c)
#<!-- -->17 0x0000000109d04a20 clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool&gt;::Visit(clang::Stmt const*) (/Users/atomicity/work/calltarget-llvm-root/bin/clang-19+0x1097aca20)
#<!-- -->18 0x0000000109c6b75c Evaluate(clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Expr const*) (/Users/atomicity/work/calltarget-llvm-root/bin/clang-19+0x10971375c)
#<!-- -->19 0x0000000109c68604 EvaluateAsRValue((anonymous namespace)::EvalInfo&amp;, clang::Expr const*, clang::APValue&amp;) (/Users/atomicity/work/calltarget-llvm-root/bin/clang-19+0x109710604)
#<!-- -->20 0x0000000109c642d4 EvaluateAsRValue(clang::Expr const*, clang::Expr::EvalResult&amp;, clang::ASTContext const&amp;, (anonymous namespace)::EvalInfo&amp;) (/Users/atomicity/work/calltarget-llvm-root/bin/clang-19+0x10970c2d4)
#<!-- -->21 0x0000000109c6415c clang::Expr::EvaluateAsRValue(clang::Expr::EvalResult&amp;, clang::ASTContext const&amp;, bool) const (/Users/atomicity/work/calltarget-llvm-root/bin/clang-19+0x10970c15c)
#<!-- -->22 0x000000010839b65c clang::Sema::VerifyIntegerConstantExpression(clang::Expr*, llvm::APSInt*, clang::Sema::VerifyICEDiagnoser&amp;, clang::Sema::AllowFoldKind) (/Users/atomicity/work/calltarget-llvm-root/bin/clang-19+0x107e4365c)
#<!-- -->23 0x000000010839755c clang::Sema::VerifyIntegerConstantExpression(clang::Expr*, llvm::APSInt*, unsigned int, clang::Sema::AllowFoldKind) (/Users/atomicity/work/calltarget-llvm-root/bin/clang-19+0x107e3f55c)
#<!-- -->24 0x000000010811b188 clang::Sema::BuildStaticAssertDeclaration(clang::SourceLocation, clang::Expr*, clang::Expr*, clang::SourceLocation, bool) (/Users/atomicity/work/calltarget-llvm-root/bin/clang-19+0x107bc3188)
#<!-- -->25 0x000000010811af5c clang::Sema::ActOnStaticAssertDeclaration(clang::SourceLocation, clang::Expr*, clang::Expr*, clang::SourceLocation) (/Users/atomicity/work/calltarget-llvm-root/bin/clang-19+0x107bc2f5c)
#<!-- -->26 0x00000001078080cc clang::Parser::ParseStaticAssertDeclaration(clang::SourceLocation&amp;) (/Users/atomicity/work/calltarget-llvm-root/bin/clang-19+0x1072b00cc)
#<!-- -->27 0x00000001077d39d8 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/Users/atomicity/work/calltarget-llvm-root/bin/clang-19+0x10727b9d8)
#<!-- -->28 0x00000001078ea63c clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/Users/atomicity/work/calltarget-llvm-root/bin/clang-19+0x10739263c)
#<!-- -->29 0x00000001078e8b54 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/Users/atomicity/work/calltarget-llvm-root/bin/clang-19+0x107390b54)
#<!-- -->30 0x00000001077ba7fc clang::ParseAST(clang::Sema&amp;, bool, bool) (/Users/atomicity/work/calltarget-llvm-root/bin/clang-19+0x1072627fc)
#<!-- -->31 0x0000000104985e40 clang::ASTFrontendAction::ExecuteAction() (/Users/atomicity/work/calltarget-llvm-root/bin/clang-19+0x10442de40)
#<!-- -->32 0x000000010498560c clang::FrontendAction::Execute() (/Users/atomicity/work/calltarget-llvm-root/bin/clang-19+0x10442d60c)
#<!-- -->33 0x00000001048a5f08 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/Users/atomicity/work/calltarget-llvm-root/bin/clang-19+0x10434df08)
#<!-- -->34 0x0000000104aa9840 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/Users/atomicity/work/calltarget-llvm-root/bin/clang-19+0x104551840)
#<!-- -->35 0x000000010056f0b8 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/Users/atomicity/work/calltarget-llvm-root/bin/clang-19+0x1000170b8)
#<!-- -->36 0x000000010055dbf4 ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) (/Users/atomicity/work/calltarget-llvm-root/bin/clang-19+0x100005bf4)
#<!-- -->37 0x000000010055c940 clang_main(int, char**, llvm::ToolContext const&amp;) (/Users/atomicity/work/calltarget-llvm-root/bin/clang-19+0x100004940)
```

This is clang-trunk, 9277a32305c1083653ffaa7955cd26deffc10988
</details>


---

### Comment 2 - shafik

May be a duplicate: https://github.com/llvm/llvm-project/issues/30358

---

### Comment 3 - atomgalaxy

I guess it ... could be? But the difference this one is not contrived :). Could be different cause, too.

---

### Comment 4 - AaronBallman

We're trying to perform an lvalue to rvalue conversion on a member pointer. The corresponding AST is:
```
`-StaticAssertDecl <line:9:1, col:23> col:1
  |-BinaryOperator <col:15, col:21> 'bool' '=='
  | |-ImplicitCastExpr <col:15> 'f0_t':'void (*)(S &)' <LValueToRValue>
  | | `-DeclRefExpr <col:15> 'const f0_t':'void (*const)(S &)' lvalue Var 0xc7b82b8 'f0' 'const f0_t':'void (*const)(S &)' non_odr_use_constant
  | `-ImplicitCastExpr <col:21> 'f0_t':'void (*)(S &)' <LValueToRValue>
  |   `-DeclRefExpr <col:21> 'const f0_t':'void (*const)(S &)' lvalue Var 0xc7b82b8 'f0' 'const f0_t':'void (*const)(S &)' non_odr_use_constant
  `-<<<NULL>>>
```
which looks reasonable. I think the issue is that the initializer for the `VarDecl` has a member function pointer type, but the typedef type is a function pointer type:
```
|-TypeAliasDecl <line:5:1, col:25> col:7 referenced f0_t 'void (*)(S &)'
| `-PointerType 'void (*)(S &)'
|   `-ParenType 'void (S &)' sugar
|     `-FunctionProtoType 'void (S &)' cdecl
|       |-BuiltinType 'void'
|       `-LValueReferenceType 'S &'
|         `-ElaboratedType 'S' sugar
|           `-RecordType 'S'
|             `-CXXRecord 'S'
|-VarDecl <line:6:1, col:25> col:16 referenced f0 'const f0_t':'void (*const)(S &)' constexpr cinit
| |-value: MemberPointer <todo>
| `-UnaryOperator <col:21, col:25> 'void (S::*)(S &)' prefix '&' cannot overflow
|   `-DeclRefExpr <col:22, col:25> 'void (S &)' lvalue CXXMethod 0xc7b7cb8 'f' 'void (S &)'
|     `-NestedNameSpecifier TypeSpec 'S'
```
CC @cor3ntin 

---

### Comment 5 - cor3ntin

Fixed in 19

---

