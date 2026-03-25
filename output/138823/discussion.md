# ICE with concept pack expansion + std::swap in C++20 constraint expressions with -std=c++20 since clang 20

**Author:** mariete1223
**URL:** https://github.com/llvm/llvm-project/issues/138823
**Status:** Closed
**Labels:** clang:frontend, crash-on-invalid, concepts, confirmed, regression:20
**Closed Date:** 2025-07-11T00:33:05Z

## Body

It may be related to issue #138820 because Stack dump start similar, but they end up differing, so i brought this issue, it appers since clang 20 with -std=c++20 flag


## Stack dump

```
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-20.1.0/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-14.2.0 -fcolor-diagnostics -fno-crash-diagnostics -x c++ -std=c++20 <source>
1.	<source>:24:13: current parser token ')'
2.	<source>:22:1: parsing function body 'main'
3.	<source>:22:1: in compound statement ('{}')
 #0 0x0000000003a59568 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x3a59568)
 #1 0x0000000003a576ac llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x3a576ac)
 #2 0x00000000039a69e8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007dae43242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x000000000742ef29 clang::Expr::IgnoreParenImpCasts() (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x742ef29)
 #5 0x00000000065c1b26 clang::ActionResult<clang::Expr*, true> calculateConstraintSatisfaction<calculateConstraintSatisfaction(clang::Sema&, clang::NamedDecl const*, clang::SourceLocation, clang::MultiLevelTemplateArgumentList const&, clang::Expr const*, clang::ConstraintSatisfaction&)::ConstraintEvaluator>(clang::Sema&, clang::Expr const*, clang::ConstraintSatisfaction&, calculateConstraintSatisfaction(clang::Sema&, clang::NamedDecl const*, clang::SourceLocation, clang::MultiLevelTemplateArgumentList const&, clang::Expr const*, clang::ConstraintSatisfaction&)::ConstraintEvaluator const&) SemaConcept.cpp:0:0
 #6 0x00000000065c484a CheckConstraintSatisfaction(clang::Sema&, clang::NamedDecl const*, llvm::ArrayRef<clang::Expr const*>, llvm::SmallVectorImpl<clang::Expr*>&, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&) SemaConcept.cpp:0:0
 #7 0x00000000065c4d97 clang::Sema::CheckConstraintSatisfaction(clang::NamedDecl const*, llvm::ArrayRef<clang::Expr const*>, llvm::SmallVectorImpl<clang::Expr*>&, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x65c4d97)
 #8 0x0000000006cebc68 clang::Sema::CheckConceptTemplateId(clang::CXXScopeSpec const&, clang::SourceLocation, clang::DeclarationNameInfo const&, clang::NamedDecl*, clang::ConceptDecl*, clang::TemplateArgumentListInfo const*) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x6cebc68)
 #9 0x0000000006e611c0 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformConceptSpecializationExpr(clang::ConceptSpecializationExpr*) SemaTemplateInstantiate.cpp:0:0
#10 0x0000000006e486f3 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#11 0x0000000006e73555 clang::Sema::SubstConstraintExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x6e73555)
#12 0x00000000065c2701 clang::ActionResult<clang::Expr*, true> calculateConstraintSatisfaction<calculateConstraintSatisfaction(clang::Sema&, clang::NamedDecl const*, clang::SourceLocation, clang::MultiLevelTemplateArgumentList const&, clang::Expr const*, clang::ConstraintSatisfaction&)::ConstraintEvaluator>(clang::Sema&, clang::Expr const*, clang::ConstraintSatisfaction&, calculateConstraintSatisfaction(clang::Sema&, clang::NamedDecl const*, clang::SourceLocation, clang::MultiLevelTemplateArgumentList const&, clang::Expr const*, clang::ConstraintSatisfaction&)::ConstraintEvaluator const&) SemaConcept.cpp:0:0
#13 0x00000000065c1b88 clang::ActionResult<clang::Expr*, true> calculateConstraintSatisfaction<calculateConstraintSatisfaction(clang::Sema&, clang::NamedDecl const*, clang::SourceLocation, clang::MultiLevelTemplateArgumentList const&, clang::Expr const*, clang::ConstraintSatisfaction&)::ConstraintEvaluator>(clang::Sema&, clang::Expr const*, clang::ConstraintSatisfaction&, calculateConstraintSatisfaction(clang::Sema&, clang::NamedDecl const*, clang::SourceLocation, clang::MultiLevelTemplateArgumentList const&, clang::Expr const*, clang::ConstraintSatisfaction&)::ConstraintEvaluator const&) SemaConcept.cpp:0:0
#14 0x00000000065c484a CheckConstraintSatisfaction(clang::Sema&, clang::NamedDecl const*, llvm::ArrayRef<clang::Expr const*>, llvm::SmallVectorImpl<clang::Expr*>&, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&) SemaConcept.cpp:0:0
#15 0x00000000065c4d97 clang::Sema::CheckConstraintSatisfaction(clang::NamedDecl const*, llvm::ArrayRef<clang::Expr const*>, llvm::SmallVectorImpl<clang::Expr*>&, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x65c4d97)
#16 0x0000000006cebc68 clang::Sema::CheckConceptTemplateId(clang::CXXScopeSpec const&, clang::SourceLocation, clang::DeclarationNameInfo const&, clang::NamedDecl*, clang::ConceptDecl*, clang::TemplateArgumentListInfo const*) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x6cebc68)
#17 0x0000000006e611c0 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformConceptSpecializationExpr(clang::ConceptSpecializationExpr*) SemaTemplateInstantiate.cpp:0:0
#18 0x0000000006e486f3 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#19 0x0000000006e56909 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCXXFoldExpr(clang::CXXFoldExpr*) SemaTemplateInstantiate.cpp:0:0
#20 0x0000000006e485a3 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#21 0x0000000006e73555 clang::Sema::SubstConstraintExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x6e73555)
#22 0x00000000065c2701 clang::ActionResult<clang::Expr*, true> calculateConstraintSatisfaction<calculateConstraintSatisfaction(clang::Sema&, clang::NamedDecl const*, clang::SourceLocation, clang::MultiLevelTemplateArgumentList const&, clang::Expr const*, clang::ConstraintSatisfaction&)::ConstraintEvaluator>(clang::Sema&, clang::Expr const*, clang::ConstraintSatisfaction&, calculateConstraintSatisfaction(clang::Sema&, clang::NamedDecl const*, clang::SourceLocation, clang::MultiLevelTemplateArgumentList const&, clang::Expr const*, clang::ConstraintSatisfaction&)::ConstraintEvaluator const&) SemaConcept.cpp:0:0
#23 0x00000000065c484a CheckConstraintSatisfaction(clang::Sema&, clang::NamedDecl const*, llvm::ArrayRef<clang::Expr const*>, llvm::SmallVectorImpl<clang::Expr*>&, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&) SemaConcept.cpp:0:0
#24 0x00000000065c4d97 clang::Sema::CheckConstraintSatisfaction(clang::NamedDecl const*, llvm::ArrayRef<clang::Expr const*>, llvm::SmallVectorImpl<clang::Expr*>&, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x65c4d97)
#25 0x00000000065c595f clang::Sema::CheckInstantiatedFunctionTemplateConstraints(clang::SourceLocation, clang::FunctionDecl*, llvm::ArrayRef<clang::TemplateArgument>, clang::ConstraintSatisfaction&) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x65c595f)
#26 0x0000000006dc5976 clang::Sema::FinishTemplateArgumentDeduction(clang::FunctionTemplateDecl*, llvm::SmallVectorImpl<clang::DeducedTemplateArgument>&, unsigned int, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, llvm::SmallVectorImpl<clang::Sema::OriginalCallArg> const*, bool, bool, llvm::function_ref<bool ()>) (.constprop.0) SemaTemplateDeduction.cpp:0:0
#27 0x0000000006dc5dc7 void llvm::function_ref<void ()>::callback_fn<clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, bool, bool, clang::QualType, clang::Expr::Classification, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>)>)::'lambda1'()>(long) SemaTemplateDeduction.cpp:0:0
#28 0x0000000007a160c1 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x7a160c1)
#29 0x0000000006dbb4d7 clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, bool, bool, clang::QualType, clang::Expr::Classification, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>)>) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x6dbb4d7)
#30 0x0000000006bd91a6 clang::Sema::AddTemplateOverloadCandidate(clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::OverloadCandidateSet&, bool, bool, bool, clang::CallExpr::ADLCallKind, clang::OverloadCandidateParamOrder, bool) (.constprop.1) SemaOverload.cpp:0:0
#31 0x0000000006bdb444 clang::Sema::AddOverloadedCallCandidates(clang::UnresolvedLookupExpr*, llvm::ArrayRef<clang::Expr*>, clang::OverloadCandidateSet&, bool) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x6bdb444)
#32 0x0000000006bdb6ee clang::Sema::buildOverloadedCallSet(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::OverloadCandidateSet*, clang::ActionResult<clang::Expr*, true>*) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x6bdb6ee)
#33 0x0000000006bdd56e clang::Sema::BuildOverloadedCallExpr(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x6bdd56e)
#34 0x00000000068225d8 clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x68225d8)
#35 0x0000000006826c2c clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x6826c2c)
#36 0x0000000006351d06 clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult<clang::Expr*, true>) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x6351d06)
#37 0x000000000634a231 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x634a231)
#38 0x000000000634c46a clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x634c46a)
#39 0x000000000634c639 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x634c639)
#40 0x00000000063518d9 clang::Parser::ParseExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x63518d9)
#41 0x00000000063df167 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x63df167)
#42 0x00000000063d28f7 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x63d28f7)
#43 0x00000000063d32ed clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x63d32ed)
#44 0x00000000063d4604 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x63d4604)
#45 0x00000000063d57ba clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x63d57ba)
#46 0x00000000062d0cf6 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x62d0cf6)
#47 0x000000000631afbf clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x631afbf)
#48 0x00000000062c92c7 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x62c92c7)
#49 0x00000000062ca0de clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x62ca0de)
#50 0x00000000062d39d2 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x62d39d2)
#51 0x00000000062d5268 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x62d5268)
#52 0x00000000062c38ca clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x62c38ca)
#53 0x00000000043aa295 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x43aa295)
#54 0x0000000004682ca1 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x4682ca1)
#55 0x000000000460260b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x460260b)
#56 0x000000000476e173 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x476e173)
#57 0x0000000000d116c1 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0xd116c1)
#58 0x0000000000d09eed ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#59 0x00000000043f5b49 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#60 0x00000000039a6e13 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x39a6e13)
#61 0x00000000043f5d69 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#62 0x00000000043b9ebd clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x43b9ebd)
#63 0x00000000043bae51 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x43bae51)
#64 0x00000000043c51ec clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x43c51ec)
#65 0x0000000000d0e391 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0xd0e391)
#66 0x0000000000bd3bc4 main (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0xbd3bc4)
#67 0x00007dae43229d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#68 0x00007dae43229e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#69 0x0000000000d09985 _start (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0xd09985)
```

## Program 

```
#include <tuple>
#include <concepts>
template<class T>
concept IsSwappable = requires(T& t, T&& u) {
{std::swap(t, u)};
};
template<class... Ts>
concept IsTupleSwappable = IsSwappable<Ts> && requires {
{std::swap(std::declval<Ts>(), std::declval<Ts>())};
...
};
template<class... Ts>
concept ABSwappable = IsTupleSwappable<Ts...> && requires {
std::swap(std::declval<Ts>()...);
...
};
void callABSwap(ABSwappable auto&&... abSwappable)
{}

int main()
{
std::tuple<int, int> t{1, 2};
callABSwap(t);
}
```


## To quickly reproduce

https://gcc.godbolt.org/z/5GbYr85n8

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (mariete1223)

<details>
It may be related to issue #<!-- -->138820 because Stack dump start similar, but they end up differing, so i brought this issue, it appers since clang 20 with -std=c++20 flag


## Stack dump

```
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-20.1.0/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-14.2.0 -fcolor-diagnostics -fno-crash-diagnostics -x c++ -std=c++20 &lt;source&gt;
1.	&lt;source&gt;:24:13: current parser token ')'
2.	&lt;source&gt;:22:1: parsing function body 'main'
3.	&lt;source&gt;:22:1: in compound statement ('{}')
 #<!-- -->0 0x0000000003a59568 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x3a59568)
 #<!-- -->1 0x0000000003a576ac llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x3a576ac)
 #<!-- -->2 0x00000000039a69e8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007dae43242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x000000000742ef29 clang::Expr::IgnoreParenImpCasts() (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x742ef29)
 #<!-- -->5 0x00000000065c1b26 clang::ActionResult&lt;clang::Expr*, true&gt; calculateConstraintSatisfaction&lt;calculateConstraintSatisfaction(clang::Sema&amp;, clang::NamedDecl const*, clang::SourceLocation, clang::MultiLevelTemplateArgumentList const&amp;, clang::Expr const*, clang::ConstraintSatisfaction&amp;)::ConstraintEvaluator&gt;(clang::Sema&amp;, clang::Expr const*, clang::ConstraintSatisfaction&amp;, calculateConstraintSatisfaction(clang::Sema&amp;, clang::NamedDecl const*, clang::SourceLocation, clang::MultiLevelTemplateArgumentList const&amp;, clang::Expr const*, clang::ConstraintSatisfaction&amp;)::ConstraintEvaluator const&amp;) SemaConcept.cpp:0:0
 #<!-- -->6 0x00000000065c484a CheckConstraintSatisfaction(clang::Sema&amp;, clang::NamedDecl const*, llvm::ArrayRef&lt;clang::Expr const*&gt;, llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceRange, clang::ConstraintSatisfaction&amp;) SemaConcept.cpp:0:0
 #<!-- -->7 0x00000000065c4d97 clang::Sema::CheckConstraintSatisfaction(clang::NamedDecl const*, llvm::ArrayRef&lt;clang::Expr const*&gt;, llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceRange, clang::ConstraintSatisfaction&amp;) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x65c4d97)
 #<!-- -->8 0x0000000006cebc68 clang::Sema::CheckConceptTemplateId(clang::CXXScopeSpec const&amp;, clang::SourceLocation, clang::DeclarationNameInfo const&amp;, clang::NamedDecl*, clang::ConceptDecl*, clang::TemplateArgumentListInfo const*) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x6cebc68)
 #<!-- -->9 0x0000000006e611c0 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformConceptSpecializationExpr(clang::ConceptSpecializationExpr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->10 0x0000000006e486f3 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->11 0x0000000006e73555 clang::Sema::SubstConstraintExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&amp;) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x6e73555)
#<!-- -->12 0x00000000065c2701 clang::ActionResult&lt;clang::Expr*, true&gt; calculateConstraintSatisfaction&lt;calculateConstraintSatisfaction(clang::Sema&amp;, clang::NamedDecl const*, clang::SourceLocation, clang::MultiLevelTemplateArgumentList const&amp;, clang::Expr const*, clang::ConstraintSatisfaction&amp;)::ConstraintEvaluator&gt;(clang::Sema&amp;, clang::Expr const*, clang::ConstraintSatisfaction&amp;, calculateConstraintSatisfaction(clang::Sema&amp;, clang::NamedDecl const*, clang::SourceLocation, clang::MultiLevelTemplateArgumentList const&amp;, clang::Expr const*, clang::ConstraintSatisfaction&amp;)::ConstraintEvaluator const&amp;) SemaConcept.cpp:0:0
#<!-- -->13 0x00000000065c1b88 clang::ActionResult&lt;clang::Expr*, true&gt; calculateConstraintSatisfaction&lt;calculateConstraintSatisfaction(clang::Sema&amp;, clang::NamedDecl const*, clang::SourceLocation, clang::MultiLevelTemplateArgumentList const&amp;, clang::Expr const*, clang::ConstraintSatisfaction&amp;)::ConstraintEvaluator&gt;(clang::Sema&amp;, clang::Expr const*, clang::ConstraintSatisfaction&amp;, calculateConstraintSatisfaction(clang::Sema&amp;, clang::NamedDecl const*, clang::SourceLocation, clang::MultiLevelTemplateArgumentList const&amp;, clang::Expr const*, clang::ConstraintSatisfaction&amp;)::ConstraintEvaluator const&amp;) SemaConcept.cpp:0:0
#<!-- -->14 0x00000000065c484a CheckConstraintSatisfaction(clang::Sema&amp;, clang::NamedDecl const*, llvm::ArrayRef&lt;clang::Expr const*&gt;, llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceRange, clang::ConstraintSatisfaction&amp;) SemaConcept.cpp:0:0
#<!-- -->15 0x00000000065c4d97 clang::Sema::CheckConstraintSatisfaction(clang::NamedDecl const*, llvm::ArrayRef&lt;clang::Expr const*&gt;, llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceRange, clang::ConstraintSatisfaction&amp;) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x65c4d97)
#<!-- -->16 0x0000000006cebc68 clang::Sema::CheckConceptTemplateId(clang::CXXScopeSpec const&amp;, clang::SourceLocation, clang::DeclarationNameInfo const&amp;, clang::NamedDecl*, clang::ConceptDecl*, clang::TemplateArgumentListInfo const*) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x6cebc68)
#<!-- -->17 0x0000000006e611c0 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformConceptSpecializationExpr(clang::ConceptSpecializationExpr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->18 0x0000000006e486f3 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->19 0x0000000006e56909 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCXXFoldExpr(clang::CXXFoldExpr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->20 0x0000000006e485a3 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->21 0x0000000006e73555 clang::Sema::SubstConstraintExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&amp;) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x6e73555)
#<!-- -->22 0x00000000065c2701 clang::ActionResult&lt;clang::Expr*, true&gt; calculateConstraintSatisfaction&lt;calculateConstraintSatisfaction(clang::Sema&amp;, clang::NamedDecl const*, clang::SourceLocation, clang::MultiLevelTemplateArgumentList const&amp;, clang::Expr const*, clang::ConstraintSatisfaction&amp;)::ConstraintEvaluator&gt;(clang::Sema&amp;, clang::Expr const*, clang::ConstraintSatisfaction&amp;, calculateConstraintSatisfaction(clang::Sema&amp;, clang::NamedDecl const*, clang::SourceLocation, clang::MultiLevelTemplateArgumentList const&amp;, clang::Expr const*, clang::ConstraintSatisfaction&amp;)::ConstraintEvaluator const&amp;) SemaConcept.cpp:0:0
#<!-- -->23 0x00000000065c484a CheckConstraintSatisfaction(clang::Sema&amp;, clang::NamedDecl const*, llvm::ArrayRef&lt;clang::Expr const*&gt;, llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceRange, clang::ConstraintSatisfaction&amp;) SemaConcept.cpp:0:0
#<!-- -->24 0x00000000065c4d97 clang::Sema::CheckConstraintSatisfaction(clang::NamedDecl const*, llvm::ArrayRef&lt;clang::Expr const*&gt;, llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceRange, clang::ConstraintSatisfaction&amp;) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x65c4d97)
#<!-- -->25 0x00000000065c595f clang::Sema::CheckInstantiatedFunctionTemplateConstraints(clang::SourceLocation, clang::FunctionDecl*, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::ConstraintSatisfaction&amp;) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x65c595f)
#<!-- -->26 0x0000000006dc5976 clang::Sema::FinishTemplateArgumentDeduction(clang::FunctionTemplateDecl*, llvm::SmallVectorImpl&lt;clang::DeducedTemplateArgument&gt;&amp;, unsigned int, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, llvm::SmallVectorImpl&lt;clang::Sema::OriginalCallArg&gt; const*, bool, bool, llvm::function_ref&lt;bool ()&gt;) (.constprop.0) SemaTemplateDeduction.cpp:0:0
#<!-- -->27 0x0000000006dc5dc7 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, bool, bool, bool, clang::QualType, clang::Expr::Classification, llvm::function_ref&lt;bool (llvm::ArrayRef&lt;clang::QualType&gt;)&gt;)::'lambda1'()&gt;(long) SemaTemplateDeduction.cpp:0:0
#<!-- -->28 0x0000000007a160c1 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x7a160c1)
#<!-- -->29 0x0000000006dbb4d7 clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, bool, bool, bool, clang::QualType, clang::Expr::Classification, llvm::function_ref&lt;bool (llvm::ArrayRef&lt;clang::QualType&gt;)&gt;) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x6dbb4d7)
#<!-- -->30 0x0000000006bd91a6 clang::Sema::AddTemplateOverloadCandidate(clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::OverloadCandidateSet&amp;, bool, bool, bool, clang::CallExpr::ADLCallKind, clang::OverloadCandidateParamOrder, bool) (.constprop.1) SemaOverload.cpp:0:0
#<!-- -->31 0x0000000006bdb444 clang::Sema::AddOverloadedCallCandidates(clang::UnresolvedLookupExpr*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::OverloadCandidateSet&amp;, bool) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x6bdb444)
#<!-- -->32 0x0000000006bdb6ee clang::Sema::buildOverloadedCallSet(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::OverloadCandidateSet*, clang::ActionResult&lt;clang::Expr*, true&gt;*) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x6bdb6ee)
#<!-- -->33 0x0000000006bdd56e clang::Sema::BuildOverloadedCallExpr(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x6bdd56e)
#<!-- -->34 0x00000000068225d8 clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x68225d8)
#<!-- -->35 0x0000000006826c2c clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x6826c2c)
#<!-- -->36 0x0000000006351d06 clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult&lt;clang::Expr*, true&gt;) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x6351d06)
#<!-- -->37 0x000000000634a231 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x634a231)
#<!-- -->38 0x000000000634c46a clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x634c46a)
#<!-- -->39 0x000000000634c639 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x634c639)
#<!-- -->40 0x00000000063518d9 clang::Parser::ParseExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x63518d9)
#<!-- -->41 0x00000000063df167 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x63df167)
#<!-- -->42 0x00000000063d28f7 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x63d28f7)
#<!-- -->43 0x00000000063d32ed clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x63d32ed)
#<!-- -->44 0x00000000063d4604 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x63d4604)
#<!-- -->45 0x00000000063d57ba clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x63d57ba)
#<!-- -->46 0x00000000062d0cf6 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x62d0cf6)
#<!-- -->47 0x000000000631afbf clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x631afbf)
#<!-- -->48 0x00000000062c92c7 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x62c92c7)
#<!-- -->49 0x00000000062ca0de clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x62ca0de)
#<!-- -->50 0x00000000062d39d2 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x62d39d2)
#<!-- -->51 0x00000000062d5268 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x62d5268)
#<!-- -->52 0x00000000062c38ca clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x62c38ca)
#<!-- -->53 0x00000000043aa295 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x43aa295)
#<!-- -->54 0x0000000004682ca1 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x4682ca1)
#<!-- -->55 0x000000000460260b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x460260b)
#<!-- -->56 0x000000000476e173 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x476e173)
#<!-- -->57 0x0000000000d116c1 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0xd116c1)
#<!-- -->58 0x0000000000d09eed ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->59 0x00000000043f5b49 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->60 0x00000000039a6e13 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x39a6e13)
#<!-- -->61 0x00000000043f5d69 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->62 0x00000000043b9ebd clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x43b9ebd)
#<!-- -->63 0x00000000043bae51 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x43bae51)
#<!-- -->64 0x00000000043c51ec clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x43c51ec)
#<!-- -->65 0x0000000000d0e391 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0xd0e391)
#<!-- -->66 0x0000000000bd3bc4 main (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0xbd3bc4)
#<!-- -->67 0x00007dae43229d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->68 0x00007dae43229e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->69 0x0000000000d09985 _start (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0xd09985)
```

## Program 

```
#include &lt;tuple&gt;
#include &lt;concepts&gt;
template&lt;class T&gt;
concept IsSwappable = requires(T&amp; t, T&amp;&amp; u) {
{std::swap(t, u)};
};
template&lt;class... Ts&gt;
concept IsTupleSwappable = IsSwappable&lt;Ts&gt; &amp;&amp; requires {
{std::swap(std::declval&lt;Ts&gt;(), std::declval&lt;Ts&gt;())};
...
};
template&lt;class... Ts&gt;
concept ABSwappable = IsTupleSwappable&lt;Ts...&gt; &amp;&amp; requires {
std::swap(std::declval&lt;Ts&gt;()...);
...
};
void callABSwap(ABSwappable auto&amp;&amp;... abSwappable)
{}

int main()
{
std::tuple&lt;int, int&gt; t{1, 2};
callABSwap(t);
}
```


## To quickly reproduce

https://gcc.godbolt.org/z/5GbYr85n8
</details>


---

### Comment 2 - zwuis

Reduced code:

```cpp
template <typename T> void foo();

template <class... Ts>
concept ConceptA = requires { foo<Ts>(); };

template <class... Ts>
concept ConceptB = ConceptA<Ts...>;

void bar(ConceptB auto &&...);

void test() { bar(1); }
```

Compiler Explorer: <https://godbolt.org/z/TK7Ed8s9o>

---

### Comment 3 - shafik

@cor3ntin this bisects to 0dedd6fe1409ab02945bb5f903a23335aa79b590

---

