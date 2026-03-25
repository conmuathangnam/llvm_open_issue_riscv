# [clang]  Crash clang::StmtVisitorBase with constrained template constructor and deduction guide for derived class

**Author:** DimitrijeDobrota
**URL:** https://github.com/llvm/llvm-project/issues/141603

## Body

The following code doesn't make sense as I've tried to reduce it as much as possible. 

It compiles fine with gcc 15.1 but crashes clang 19.1.0, as can be seen in [godbolt](https://godbolt.org/z/qajfnjE1h)

```c++

// any concept will do the trick
template<class T> concept IsOption = sizeof(T) >= 0;

struct group_base
{
  // if 'IsOption' is changed to 'class' code compiles
  template<IsOption Opt, IsOption... Opts>
  group_base(Opt&& opt, Opts&&... opts)
  {
  }
};

template<class Record>
struct group : group_base
{
  using group_base::group_base;
};

template<IsOption Opt, IsOption... Opts>
group(Opt&& opt, Opts&&... opts) -> group<Opt>;

int main() {
    group{3};
}
```

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-19.1.0/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-14.2.0 -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 -O0 <source>
1.	<source>:24:13: current parser token ';'
2.	<source>:23:12: parsing function body 'main'
3.	<source>:23:12: in compound statement ('{}')
 #0 0x00000000036fdb08 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x36fdb08)
 #1 0x00000000036fbc74 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x36fbc74)
 #2 0x0000000003647be8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000078d636e42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
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
#16 0x00000000068eef0d clang::Sema::AddTemplateOverloadCandidate(clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::OverloadCandidateSet&, bool, bool, bool, clang::CallExpr::ADLCallKind, clang::OverloadCandidateParamOrder, bool) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x68eef0d)
#17 0x000000000670cec2 clang::Sema::DeduceTemplateSpecializationFromInitializer(clang::TypeSourceInfo*, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::MutableArrayRef<clang::Expr*>)::'lambda0'(bool)::operator()(bool) const SemaInit.cpp:0:0
#18 0x000000000670d82a clang::Sema::DeduceTemplateSpecializationFromInitializer(clang::TypeSourceInfo*, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::MutableArrayRef<clang::Expr*>) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x670d82a)
#19 0x00000000065dcc6b clang::Sema::BuildCXXTypeConstructExpr(clang::TypeSourceInfo*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x65dcc6b)
#20 0x00000000066709fd clang::Sema::ActOnCXXTypeConstructExpr(clang::OpaquePtr<clang::QualType>, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x66709fd)
#21 0x0000000005faeb75 clang::Parser::ParseCXXTypeConstructExpression(clang::DeclSpec const&) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x5faeb75)
#22 0x0000000005f9e519 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x5f9e519)
#23 0x0000000005f9f09a clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x5f9f09a)
#24 0x0000000005f9f239 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x5f9f239)
#25 0x0000000005fa41f9 clang::Parser::ParseExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x5fa41f9)
#26 0x000000000602f627 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x602f627)
#27 0x0000000006024c1a clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x6024c1a)
#28 0x000000000602560d clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x602560d)
#29 0x0000000006026891 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x6026891)
#30 0x0000000006028bda clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x6028bda)
#31 0x0000000005f24740 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x5f24740)
#32 0x0000000005f6d07e clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x5f6d07e)
#33 0x0000000005f1e817 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x5f1e817)
#34 0x0000000005f1f703 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x5f1f703)
#35 0x0000000005f27cd2 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x5f27cd2)
#36 0x0000000005f29ac8 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x5f29ac8)
#37 0x0000000005f1877a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x5f1877a)
#38 0x0000000003fce825 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x3fce825)
#39 0x0000000004274201 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x4274201)
#40 0x00000000041f699b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x41f699b)
#41 0x000000000435a843 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x435a843)
#42 0x0000000000c874dc cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0xc874dc)
#43 0x0000000000c8146d ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#44 0x0000000004017309 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#45 0x0000000003648013 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x3648013)
#46 0x0000000004017529 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#47 0x0000000003fdd947 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x3fdd947)
#48 0x0000000003fde2fd clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x3fde2fd)
#49 0x0000000003fe7d4c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x3fe7d4c)
#50 0x0000000000c847f1 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0xc847f1)
#51 0x0000000000b5c2d4 main (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0xb5c2d4)
#52 0x000078d636e29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#53 0x000078d636e29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#54 0x0000000000c80f0e _start (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0xc80f0e)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Dimitrije Dobrota (DimitrijeDobrota)

<details>
The following code doesn't make sense as I've tried to reduce it as much as possible. 

It compiles fine with gcc 15.1 but crashes clang 19.1.0, as can be seen in [godbolt](https://godbolt.org/z/qajfnjE1h)

```c++

// any concept will do the trick
template&lt;class T&gt; concept IsOption = sizeof(T) &gt;= 0;

struct group_base
{
  // if 'IsOption' is changed to 'class' code compiles
  template&lt;IsOption Opt, IsOption... Opts&gt;
  group_base(Opt&amp;&amp; opt, Opts&amp;&amp;... opts)
  {
  }
};

template&lt;class Record&gt;
struct group : group_base
{
  using group_base::group_base;
};

template&lt;IsOption Opt, IsOption... Opts&gt;
group(Opt&amp;&amp; opt, Opts&amp;&amp;... opts) -&gt; group&lt;Opt&gt;;

int main() {
    group{3};
}
```

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-19.1.0/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-14.2.0 -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 -O0 &lt;source&gt;
1.	&lt;source&gt;:24:13: current parser token ';'
2.	&lt;source&gt;:23:12: parsing function body 'main'
3.	&lt;source&gt;:23:12: in compound statement ('{}')
 #<!-- -->0 0x00000000036fdb08 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x36fdb08)
 #<!-- -->1 0x00000000036fbc74 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x36fbc74)
 #<!-- -->2 0x0000000003647be8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x000078d636e42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
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
#<!-- -->16 0x00000000068eef0d clang::Sema::AddTemplateOverloadCandidate(clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::OverloadCandidateSet&amp;, bool, bool, bool, clang::CallExpr::ADLCallKind, clang::OverloadCandidateParamOrder, bool) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x68eef0d)
#<!-- -->17 0x000000000670cec2 clang::Sema::DeduceTemplateSpecializationFromInitializer(clang::TypeSourceInfo*, clang::InitializedEntity const&amp;, clang::InitializationKind const&amp;, llvm::MutableArrayRef&lt;clang::Expr*&gt;)::'lambda0'(bool)::operator()(bool) const SemaInit.cpp:0:0
#<!-- -->18 0x000000000670d82a clang::Sema::DeduceTemplateSpecializationFromInitializer(clang::TypeSourceInfo*, clang::InitializedEntity const&amp;, clang::InitializationKind const&amp;, llvm::MutableArrayRef&lt;clang::Expr*&gt;) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x670d82a)
#<!-- -->19 0x00000000065dcc6b clang::Sema::BuildCXXTypeConstructExpr(clang::TypeSourceInfo*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x65dcc6b)
#<!-- -->20 0x00000000066709fd clang::Sema::ActOnCXXTypeConstructExpr(clang::OpaquePtr&lt;clang::QualType&gt;, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x66709fd)
#<!-- -->21 0x0000000005faeb75 clang::Parser::ParseCXXTypeConstructExpression(clang::DeclSpec const&amp;) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x5faeb75)
#<!-- -->22 0x0000000005f9e519 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x5f9e519)
#<!-- -->23 0x0000000005f9f09a clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x5f9f09a)
#<!-- -->24 0x0000000005f9f239 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x5f9f239)
#<!-- -->25 0x0000000005fa41f9 clang::Parser::ParseExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x5fa41f9)
#<!-- -->26 0x000000000602f627 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x602f627)
#<!-- -->27 0x0000000006024c1a clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x6024c1a)
#<!-- -->28 0x000000000602560d clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x602560d)
#<!-- -->29 0x0000000006026891 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x6026891)
#<!-- -->30 0x0000000006028bda clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x6028bda)
#<!-- -->31 0x0000000005f24740 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x5f24740)
#<!-- -->32 0x0000000005f6d07e clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x5f6d07e)
#<!-- -->33 0x0000000005f1e817 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x5f1e817)
#<!-- -->34 0x0000000005f1f703 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x5f1f703)
#<!-- -->35 0x0000000005f27cd2 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x5f27cd2)
#<!-- -->36 0x0000000005f29ac8 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x5f29ac8)
#<!-- -->37 0x0000000005f1877a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x5f1877a)
#<!-- -->38 0x0000000003fce825 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x3fce825)
#<!-- -->39 0x0000000004274201 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x4274201)
#<!-- -->40 0x00000000041f699b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x41f699b)
#<!-- -->41 0x000000000435a843 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x435a843)
#<!-- -->42 0x0000000000c874dc cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0xc874dc)
#<!-- -->43 0x0000000000c8146d ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->44 0x0000000004017309 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->45 0x0000000003648013 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x3648013)
#<!-- -->46 0x0000000004017529 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->47 0x0000000003fdd947 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x3fdd947)
#<!-- -->48 0x0000000003fde2fd clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x3fde2fd)
#<!-- -->49 0x0000000003fe7d4c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0x3fe7d4c)
#<!-- -->50 0x0000000000c847f1 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0xc847f1)
#<!-- -->51 0x0000000000b5c2d4 main (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0xb5c2d4)
#<!-- -->52 0x000078d636e29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->53 0x000078d636e29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->54 0x0000000000c80f0e _start (/opt/compiler-explorer/clang-19.1.0/bin/clang+++0xc80f0e)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```
</details>


---

### Comment 2 - EugeneZelenko

Still crashes in `main`: https://godbolt.org/z/6o79ET73E

---

### Comment 3 - shafik

This goes back to clang-10: https://godbolt.org/z/7veYbe7x7

---

### Comment 4 - shafik

This one also goes back to clang-10: https://github.com/llvm/llvm-project/issues/131481

but the backtrace is different.

CC @zyn0217 

---

