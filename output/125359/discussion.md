# ICE on lambda expression with concept constrained template argument

**Author:** Fedr
**URL:** https://github.com/llvm/llvm-project/issues/125359
**Status:** Closed
**Labels:** duplicate, clang:frontend, concepts, crash, lambda
**Closed Date:** 2025-02-01T21:17:13Z

## Body

This program
```c++
template<class T>
concept C = true;

static_assert([]<int I>() -> decltype([]<C auto = I>{ return true; })
    { return {}; }.template operator()<{}>()());
```
is accepted in GCC, but Clang crashes:
```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-19.1.0/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-14.2.0 -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 -Wall -Wextra -pedantic-errors -stdlib=libc++ <source>
1.	<source>:5:45: current parser token ')'
 #0 0x00000000036fdb08 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x36fdb08)
 #1 0x00000000036fbc74 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x36fbc74)
 #2 0x0000000003647be8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007ee200442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x0000000007281d4d clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
 #5 0x000000000724bbfe Evaluate(clang::APValue&, (anonymous namespace)::EvalInfo&, clang::Expr const*) ExprConstant.cpp:0:0
 #6 0x0000000007262bee EvaluateInPlace(clang::APValue&, (anonymous namespace)::EvalInfo&, (anonymous namespace)::LValue const&, clang::Expr const*, bool) ExprConstant.cpp:0:0
 #7 0x000000000726ada9 clang::Expr::EvaluateAsConstantExpr(clang::Expr::EvalResult&, clang::ASTContext const&, clang::Expr::ConstantExprKind) const (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x726ada9)
 #8 0x000000000621686e clang::ActionResult<clang::Expr*, true> calculateConstraintSatisfaction<calculateConstraintSatisfaction(clang::Sema&, clang::NamedDecl const*, clang::SourceLocation, clang::MultiLevelTemplateArgumentList const&, clang::Expr const*, clang::ConstraintSatisfaction&)::ConstraintEvaluator>(clang::Sema&, clang::Expr const*, clang::ConstraintSatisfaction&, calculateConstraintSatisfaction(clang::Sema&, clang::NamedDecl const*, clang::SourceLocation, clang::MultiLevelTemplateArgumentList const&, clang::Expr const*, clang::ConstraintSatisfaction&)::ConstraintEvaluator const&) SemaConcept.cpp:0:0
 #9 0x000000000621835a CheckConstraintSatisfaction(clang::Sema&, clang::NamedDecl const*, llvm::ArrayRef<clang::Expr const*>, llvm::SmallVectorImpl<clang::Expr*>&, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&) SemaConcept.cpp:0:0
#10 0x00000000062188a7 clang::Sema::CheckConstraintSatisfaction(clang::NamedDecl const*, llvm::ArrayRef<clang::Expr const*>, llvm::SmallVectorImpl<clang::Expr*>&, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x62188a7)
#11 0x0000000006219467 clang::Sema::CheckInstantiatedFunctionTemplateConstraints(clang::SourceLocation, clang::FunctionDecl*, llvm::ArrayRef<clang::TemplateArgument>, clang::ConstraintSatisfaction&) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x6219467)
#12 0x0000000006ada2fa clang::Sema::FinishTemplateArgumentDeduction(clang::FunctionTemplateDecl*, llvm::SmallVectorImpl<clang::DeducedTemplateArgument>&, unsigned int, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, llvm::SmallVectorImpl<clang::Sema::OriginalCallArg> const*, bool, llvm::function_ref<bool ()>) (.constprop.0) SemaTemplateDeduction.cpp:0:0
#13 0x0000000006adae4a void llvm::function_ref<void ()>::callback_fn<clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, bool, clang::QualType, clang::Expr::Classification, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>)>)::'lambda1'()>(long) SemaTemplateDeduction.cpp:0:0
#14 0x0000000006097151 clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x6097151)
#15 0x0000000006af201f clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, bool, clang::QualType, clang::Expr::Classification, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>)>) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x6af201f)
#16 0x00000000068f68ef clang::Sema::AddMethodTemplateCandidate(clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::CXXRecordDecl*, clang::TemplateArgumentListInfo*, clang::QualType, clang::Expr::Classification, llvm::ArrayRef<clang::Expr*>, clang::OverloadCandidateSet&, bool, bool, clang::OverloadCandidateParamOrder) (.constprop.0) SemaOverload.cpp:0:0
#17 0x00000000068f6d23 clang::Sema::AddMethodCandidate(clang::DeclAccessPair, clang::QualType, clang::Expr::Classification, llvm::ArrayRef<clang::Expr*>, clang::OverloadCandidateSet&, bool, clang::OverloadCandidateParamOrder) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x68f6d23)
#18 0x0000000006907473 clang::Sema::BuildCallToObjectOfClassType(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x6907473)
#19 0x0000000006512c8f clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x6512c8f)
#20 0x00000000065186fc clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x65186fc)
#21 0x0000000005fa512b clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult<clang::Expr*, true>) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x5fa512b)
#22 0x0000000005f9ce71 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x5f9ce71)
#23 0x0000000005fabac9 clang::Parser::ParseConstantExpressionInExprEvalContext(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x5fabac9)
#24 0x0000000005f81cbf clang::Parser::ParseStaticAssertDeclaration(clang::SourceLocation&) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x5f81cbf)
#25 0x0000000005f71028 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x5f71028)
#26 0x0000000005f27cfa clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x5f27cfa)
#27 0x0000000005f29ac8 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x5f29ac8)
#28 0x0000000005f1877a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x5f1877a)
#29 0x0000000003fce825 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x3fce825)
#30 0x0000000004274201 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x4274201)
#31 0x00000000041f699b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x41f699b)
#32 0x000000000435a843 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x435a843)
#33 0x0000000000c874dc cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0xc874dc)
#34 0x0000000000c8146d ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#35 0x0000000004017309 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#36 0x0000000003648013 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x3648013)
#37 0x0000000004017529 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#38 0x0000000003fdd947 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x3fdd947)
#39 0x0000000003fde2fd clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x3fde2fd)
#40 0x0000000003fe7d4c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x3fe7d4c)
#41 0x0000000000c847f1 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0xc847f1)
#42 0x0000000000b5c2d4 main (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0xb5c2d4)
#43 0x00007ee200429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#44 0x00007ee200429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#45 0x0000000000c80f0e _start (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0xc80f0e)
```
Online demo: https://gcc.godbolt.org/z/74aWojehv

It is a modified example from https://stackoverflow.com/q/77431308/7325599

## Comments

### Comment 1 - Fedr

Sorry, duplicate of https://github.com/llvm/llvm-project/issues/97953

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Fedor Chelnokov (Fedr)

<details>
This program
```c++
template&lt;class T&gt;
concept C = true;

static_assert([]&lt;int I&gt;() -&gt; decltype([]&lt;C auto = I&gt;{ return true; })
    { return {}; }.template operator()&lt;{}&gt;()());
```
is accepted in GCC, but Clang crashes:
```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-19.1.0/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-14.2.0 -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 -Wall -Wextra -pedantic-errors -stdlib=libc++ &lt;source&gt;
1.	&lt;source&gt;:5:45: current parser token ')'
 #<!-- -->0 0x00000000036fdb08 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x36fdb08)
 #<!-- -->1 0x00000000036fbc74 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x36fbc74)
 #<!-- -->2 0x0000000003647be8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007ee200442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x0000000007281d4d clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool&gt;::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
 #<!-- -->5 0x000000000724bbfe Evaluate(clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Expr const*) ExprConstant.cpp:0:0
 #<!-- -->6 0x0000000007262bee EvaluateInPlace(clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;, (anonymous namespace)::LValue const&amp;, clang::Expr const*, bool) ExprConstant.cpp:0:0
 #<!-- -->7 0x000000000726ada9 clang::Expr::EvaluateAsConstantExpr(clang::Expr::EvalResult&amp;, clang::ASTContext const&amp;, clang::Expr::ConstantExprKind) const (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x726ada9)
 #<!-- -->8 0x000000000621686e clang::ActionResult&lt;clang::Expr*, true&gt; calculateConstraintSatisfaction&lt;calculateConstraintSatisfaction(clang::Sema&amp;, clang::NamedDecl const*, clang::SourceLocation, clang::MultiLevelTemplateArgumentList const&amp;, clang::Expr const*, clang::ConstraintSatisfaction&amp;)::ConstraintEvaluator&gt;(clang::Sema&amp;, clang::Expr const*, clang::ConstraintSatisfaction&amp;, calculateConstraintSatisfaction(clang::Sema&amp;, clang::NamedDecl const*, clang::SourceLocation, clang::MultiLevelTemplateArgumentList const&amp;, clang::Expr const*, clang::ConstraintSatisfaction&amp;)::ConstraintEvaluator const&amp;) SemaConcept.cpp:0:0
 #<!-- -->9 0x000000000621835a CheckConstraintSatisfaction(clang::Sema&amp;, clang::NamedDecl const*, llvm::ArrayRef&lt;clang::Expr const*&gt;, llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceRange, clang::ConstraintSatisfaction&amp;) SemaConcept.cpp:0:0
#<!-- -->10 0x00000000062188a7 clang::Sema::CheckConstraintSatisfaction(clang::NamedDecl const*, llvm::ArrayRef&lt;clang::Expr const*&gt;, llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceRange, clang::ConstraintSatisfaction&amp;) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x62188a7)
#<!-- -->11 0x0000000006219467 clang::Sema::CheckInstantiatedFunctionTemplateConstraints(clang::SourceLocation, clang::FunctionDecl*, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::ConstraintSatisfaction&amp;) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x6219467)
#<!-- -->12 0x0000000006ada2fa clang::Sema::FinishTemplateArgumentDeduction(clang::FunctionTemplateDecl*, llvm::SmallVectorImpl&lt;clang::DeducedTemplateArgument&gt;&amp;, unsigned int, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, llvm::SmallVectorImpl&lt;clang::Sema::OriginalCallArg&gt; const*, bool, llvm::function_ref&lt;bool ()&gt;) (.constprop.0) SemaTemplateDeduction.cpp:0:0
#<!-- -->13 0x0000000006adae4a void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, bool, bool, clang::QualType, clang::Expr::Classification, llvm::function_ref&lt;bool (llvm::ArrayRef&lt;clang::QualType&gt;)&gt;)::'lambda1'()&gt;(long) SemaTemplateDeduction.cpp:0:0
#<!-- -->14 0x0000000006097151 clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x6097151)
#<!-- -->15 0x0000000006af201f clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, bool, bool, clang::QualType, clang::Expr::Classification, llvm::function_ref&lt;bool (llvm::ArrayRef&lt;clang::QualType&gt;)&gt;) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x6af201f)
#<!-- -->16 0x00000000068f68ef clang::Sema::AddMethodTemplateCandidate(clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::CXXRecordDecl*, clang::TemplateArgumentListInfo*, clang::QualType, clang::Expr::Classification, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::OverloadCandidateSet&amp;, bool, bool, clang::OverloadCandidateParamOrder) (.constprop.0) SemaOverload.cpp:0:0
#<!-- -->17 0x00000000068f6d23 clang::Sema::AddMethodCandidate(clang::DeclAccessPair, clang::QualType, clang::Expr::Classification, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::OverloadCandidateSet&amp;, bool, clang::OverloadCandidateParamOrder) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x68f6d23)
#<!-- -->18 0x0000000006907473 clang::Sema::BuildCallToObjectOfClassType(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x6907473)
#<!-- -->19 0x0000000006512c8f clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x6512c8f)
#<!-- -->20 0x00000000065186fc clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x65186fc)
#<!-- -->21 0x0000000005fa512b clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult&lt;clang::Expr*, true&gt;) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x5fa512b)
#<!-- -->22 0x0000000005f9ce71 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x5f9ce71)
#<!-- -->23 0x0000000005fabac9 clang::Parser::ParseConstantExpressionInExprEvalContext(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x5fabac9)
#<!-- -->24 0x0000000005f81cbf clang::Parser::ParseStaticAssertDeclaration(clang::SourceLocation&amp;) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x5f81cbf)
#<!-- -->25 0x0000000005f71028 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x5f71028)
#<!-- -->26 0x0000000005f27cfa clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x5f27cfa)
#<!-- -->27 0x0000000005f29ac8 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x5f29ac8)
#<!-- -->28 0x0000000005f1877a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x5f1877a)
#<!-- -->29 0x0000000003fce825 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x3fce825)
#<!-- -->30 0x0000000004274201 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x4274201)
#<!-- -->31 0x00000000041f699b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x41f699b)
#<!-- -->32 0x000000000435a843 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x435a843)
#<!-- -->33 0x0000000000c874dc cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0xc874dc)
#<!-- -->34 0x0000000000c8146d ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->35 0x0000000004017309 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->36 0x0000000003648013 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x3648013)
#<!-- -->37 0x0000000004017529 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->38 0x0000000003fdd947 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x3fdd947)
#<!-- -->39 0x0000000003fde2fd clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x3fde2fd)
#<!-- -->40 0x0000000003fe7d4c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x3fe7d4c)
#<!-- -->41 0x0000000000c847f1 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0xc847f1)
#<!-- -->42 0x0000000000b5c2d4 main (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0xb5c2d4)
#<!-- -->43 0x00007ee200429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->44 0x00007ee200429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->45 0x0000000000c80f0e _start (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0xc80f0e)
```
Online demo: https://gcc.godbolt.org/z/74aWojehv

It is a modified example from https://stackoverflow.com/q/77431308/7325599
</details>


---

