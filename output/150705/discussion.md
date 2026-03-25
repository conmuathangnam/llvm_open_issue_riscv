# [clang] Assertion for invalid program in constant evaluation for virtual function call via member pointer into sibling class.

**Author:** keinflue
**URL:** https://github.com/llvm/llvm-project/issues/150705
**Status:** Closed
**Labels:** clang:frontend, crash
**Closed Date:** 2025-08-05T21:44:17Z

## Body

Current clang trunk crashes on the following program with an assertion (see https://godbolt.org/z/sxbeK4qdc):
```cpp
struct A { };
struct B : A { };
struct C : A {
    constexpr virtual void foo() {}
};

consteval void test() {
    auto p = &C::foo;
    auto q = static_cast<void (A::*)()>(p);
    B b;
    A& a = b;
    (a.*q)();
}

int main() { test(); }
```

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++23 -pedantic-errors <source>
1.	<eof> parser at end of file
2.	<source>:15:12: parsing function body 'main'
 #0 0x0000000003ca4b28 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3ca4b28)
 #1 0x0000000003ca24fc llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3ca24fc)
 #2 0x0000000003bf2828 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000077be12642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x0000000007442bb8 CastToDerivedClass((anonymous namespace)::EvalInfo&, clang::Expr const*, (anonymous namespace)::LValue&, clang::RecordDecl const*, unsigned int) ExprConstant.cpp:0:0
 #5 0x000000000744f1c6 HandleVirtualDispatch((anonymous namespace)::EvalInfo&, clang::Expr const*, (anonymous namespace)::LValue&, clang::CXXMethodDecl const*, llvm::SmallVectorImpl<clang::QualType>&) ExprConstant.cpp:0:0
 #6 0x000000000749caa2 (anonymous namespace)::VoidExprEvaluator::VisitCallExpr(clang::CallExpr const*) ExprConstant.cpp:0:0
 #7 0x00000000074c4d79 clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::VoidExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
 #8 0x000000000745744d Evaluate(clang::APValue&, (anonymous namespace)::EvalInfo&, clang::Expr const*) ExprConstant.cpp:0:0
 #9 0x000000000745e9d8 EvaluateIgnoredValue((anonymous namespace)::EvalInfo&, clang::Expr const*) ExprConstant.cpp:0:0
#10 0x00000000074682c0 EvaluateStmt((anonymous namespace)::StmtResult&, (anonymous namespace)::EvalInfo&, clang::Stmt const*, clang::SwitchCase const*) (.part.0) ExprConstant.cpp:0:0
#11 0x0000000007467318 EvaluateStmt((anonymous namespace)::StmtResult&, (anonymous namespace)::EvalInfo&, clang::Stmt const*, clang::SwitchCase const*) (.part.0) ExprConstant.cpp:0:0
#12 0x000000000746bec8 HandleFunctionCall(clang::SourceLocation, clang::FunctionDecl const*, (anonymous namespace)::LValue const*, clang::Expr const*, llvm::ArrayRef<clang::Expr const*>, (anonymous namespace)::CallRef, clang::Stmt const*, (anonymous namespace)::EvalInfo&, clang::APValue&, (anonymous namespace)::LValue const*) (.isra.0) ExprConstant.cpp:0:0
#13 0x000000000749c8c4 (anonymous namespace)::VoidExprEvaluator::VisitCallExpr(clang::CallExpr const*) ExprConstant.cpp:0:0
#14 0x00000000074c4ccc clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::VoidExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#15 0x00000000074c52fc clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::VoidExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#16 0x000000000745744d Evaluate(clang::APValue&, (anonymous namespace)::EvalInfo&, clang::Expr const*) ExprConstant.cpp:0:0
#17 0x000000000746e6f6 EvaluateInPlace(clang::APValue&, (anonymous namespace)::EvalInfo&, (anonymous namespace)::LValue const&, clang::Expr const*, bool) (.constprop.0) ExprConstant.cpp:0:0
#18 0x00000000074c3876 clang::Expr::EvaluateAsConstantExpr(clang::Expr::EvalResult&, clang::ASTContext const&, clang::Expr::ConstantExprKind) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x74c3876)
#19 0x00000000067b7380 clang::Sema::PopExpressionEvaluationContext() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x67b7380)
#20 0x0000000006618e42 clang::Sema::ActOnFinishFunctionBody(clang::Decl*, clang::Stmt*, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6618e42)
#21 0x0000000006386fdf clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6386fdf)
#22 0x000000000625e9b8 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x625e9b8)
#23 0x00000000062aae6f clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62aae6f)
#24 0x000000000625779f clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x625779f)
#25 0x0000000006258587 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6258587)
#26 0x000000000626117f clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x626117f)
#27 0x0000000006262990 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6262990)
#28 0x000000000625110a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x625110a)
#29 0x00000000045eee5d clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45eee5d)
#30 0x00000000048f82ba clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x48f82ba)
#31 0x000000000487359b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x487359b)
#32 0x00000000049ec3f3 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x49ec3f3)
#33 0x0000000000dbaf25 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdbaf25)
#34 0x0000000000db2e7d ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#35 0x0000000004660aa9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#36 0x0000000003bf2c43 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3bf2c43)
#37 0x0000000004660cc9 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#38 0x000000000462416d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x462416d)
#39 0x0000000004625121 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4625121)
#40 0x00000000046318bc clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x46318bc)
#41 0x0000000000db7741 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdb7741)
#42 0x0000000000c67bc4 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc67bc4)
#43 0x000077be12629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#44 0x000077be12629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#45 0x0000000000db2915 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdb2915)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```

The program should be ill-formed, because the most derived object `b` does not contain the member `foo` which the member pointer `a` references, causing the call to have undefined behavior.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (keinflue)

<details>
Current clang trunk crashes on the following program with an assertion (see https://godbolt.org/z/sxbeK4qdc):
```cpp
struct A { };
struct B : A { };
struct C : A {
    constexpr virtual void foo() {}
};

consteval void test() {
    auto p = &amp;C::foo;
    auto q = static_cast&lt;void (A::*)()&gt;(p);
    B b;
    A&amp; a = b;
    (a.*q)();
}

int main() { test(); }
```

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++23 -pedantic-errors &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	&lt;source&gt;:15:12: parsing function body 'main'
 #<!-- -->0 0x0000000003ca4b28 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3ca4b28)
 #<!-- -->1 0x0000000003ca24fc llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3ca24fc)
 #<!-- -->2 0x0000000003bf2828 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x000077be12642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x0000000007442bb8 CastToDerivedClass((anonymous namespace)::EvalInfo&amp;, clang::Expr const*, (anonymous namespace)::LValue&amp;, clang::RecordDecl const*, unsigned int) ExprConstant.cpp:0:0
 #<!-- -->5 0x000000000744f1c6 HandleVirtualDispatch((anonymous namespace)::EvalInfo&amp;, clang::Expr const*, (anonymous namespace)::LValue&amp;, clang::CXXMethodDecl const*, llvm::SmallVectorImpl&lt;clang::QualType&gt;&amp;) ExprConstant.cpp:0:0
 #<!-- -->6 0x000000000749caa2 (anonymous namespace)::VoidExprEvaluator::VisitCallExpr(clang::CallExpr const*) ExprConstant.cpp:0:0
 #<!-- -->7 0x00000000074c4d79 clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::VoidExprEvaluator, bool&gt;::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
 #<!-- -->8 0x000000000745744d Evaluate(clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Expr const*) ExprConstant.cpp:0:0
 #<!-- -->9 0x000000000745e9d8 EvaluateIgnoredValue((anonymous namespace)::EvalInfo&amp;, clang::Expr const*) ExprConstant.cpp:0:0
#<!-- -->10 0x00000000074682c0 EvaluateStmt((anonymous namespace)::StmtResult&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Stmt const*, clang::SwitchCase const*) (.part.0) ExprConstant.cpp:0:0
#<!-- -->11 0x0000000007467318 EvaluateStmt((anonymous namespace)::StmtResult&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Stmt const*, clang::SwitchCase const*) (.part.0) ExprConstant.cpp:0:0
#<!-- -->12 0x000000000746bec8 HandleFunctionCall(clang::SourceLocation, clang::FunctionDecl const*, (anonymous namespace)::LValue const*, clang::Expr const*, llvm::ArrayRef&lt;clang::Expr const*&gt;, (anonymous namespace)::CallRef, clang::Stmt const*, (anonymous namespace)::EvalInfo&amp;, clang::APValue&amp;, (anonymous namespace)::LValue const*) (.isra.0) ExprConstant.cpp:0:0
#<!-- -->13 0x000000000749c8c4 (anonymous namespace)::VoidExprEvaluator::VisitCallExpr(clang::CallExpr const*) ExprConstant.cpp:0:0
#<!-- -->14 0x00000000074c4ccc clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::VoidExprEvaluator, bool&gt;::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#<!-- -->15 0x00000000074c52fc clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::VoidExprEvaluator, bool&gt;::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#<!-- -->16 0x000000000745744d Evaluate(clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Expr const*) ExprConstant.cpp:0:0
#<!-- -->17 0x000000000746e6f6 EvaluateInPlace(clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;, (anonymous namespace)::LValue const&amp;, clang::Expr const*, bool) (.constprop.0) ExprConstant.cpp:0:0
#<!-- -->18 0x00000000074c3876 clang::Expr::EvaluateAsConstantExpr(clang::Expr::EvalResult&amp;, clang::ASTContext const&amp;, clang::Expr::ConstantExprKind) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x74c3876)
#<!-- -->19 0x00000000067b7380 clang::Sema::PopExpressionEvaluationContext() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x67b7380)
#<!-- -->20 0x0000000006618e42 clang::Sema::ActOnFinishFunctionBody(clang::Decl*, clang::Stmt*, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6618e42)
#<!-- -->21 0x0000000006386fdf clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6386fdf)
#<!-- -->22 0x000000000625e9b8 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x625e9b8)
#<!-- -->23 0x00000000062aae6f clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62aae6f)
#<!-- -->24 0x000000000625779f clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x625779f)
#<!-- -->25 0x0000000006258587 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6258587)
#<!-- -->26 0x000000000626117f clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x626117f)
#<!-- -->27 0x0000000006262990 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6262990)
#<!-- -->28 0x000000000625110a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x625110a)
#<!-- -->29 0x00000000045eee5d clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45eee5d)
#<!-- -->30 0x00000000048f82ba clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x48f82ba)
#<!-- -->31 0x000000000487359b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x487359b)
#<!-- -->32 0x00000000049ec3f3 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x49ec3f3)
#<!-- -->33 0x0000000000dbaf25 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdbaf25)
#<!-- -->34 0x0000000000db2e7d ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->35 0x0000000004660aa9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->36 0x0000000003bf2c43 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3bf2c43)
#<!-- -->37 0x0000000004660cc9 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->38 0x000000000462416d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x462416d)
#<!-- -->39 0x0000000004625121 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4625121)
#<!-- -->40 0x00000000046318bc clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x46318bc)
#<!-- -->41 0x0000000000db7741 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdb7741)
#<!-- -->42 0x0000000000c67bc4 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc67bc4)
#<!-- -->43 0x000077be12629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->44 0x000077be12629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->45 0x0000000000db2915 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdb2915)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```

The program should be ill-formed, because the most derived object `b` does not contain the member `foo` which the member pointer `a` references, causing the call to have undefined behavior.
</details>


---

