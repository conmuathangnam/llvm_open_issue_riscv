# [clang] Assertion failure in `Sema::getPackSubstitutedTemplateArgument` during constraint checking (ArrayRef OOB)

**Author:** matt-gb
**URL:** https://github.com/llvm/llvm-project/issues/177245
**Status:** Closed
**Labels:** clang:frontend, crash, c++26, regression:22
**Closed Date:** 2026-01-23T09:16:44Z

## Body

https://godbolt.org/z/YKoe6vv9M

This code crashes clang 22 in C++26 mode (`-std=c++2c` or `-std=c++26`). Seen with clang 22.1.0-rc1 (built locally from 8164f1a) and godbolt's clang-trunk. Release builds segfault, assertion-enabled builds hit a bounds check in `ArrayRef.h`.

Clang 21.1.0 and GCC 15.2 both accept it with no warnings.

Bisected to e9972de (#161671).

#### Repro (reduced from some code in stdexec 24.09):
```cpp
// crash.cpp

template <class _Fun, class... _As>
concept __callable = requires (_Fun __fun, _As...) { __fun(); };

template <class... _Args>
struct __mdispatch {
  template <class... _Ts>
    requires (__callable<_Args, _Ts...> && ...)
  void operator()();
};

static_assert(!__callable<__mdispatch<int>>);
```
```bash
clang-22 -std=c++26 -fsyntax-only crash.cpp # No crash with -std=c++23/2b
```

#### Clang 22.1.0-rc1 built with options:
```
cmake -S llvm -B build-debug-rc1 -G Ninja \
  -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_C_COMPILER=/usr/bin/clang-19 \
  -DCMAKE_CXX_COMPILER=/usr/bin/clang++-19 \
  -DLLVM_ENABLE_PROJECTS=clang \
  -DLLVM_TARGETS_TO_BUILD=X86
```

#### Crash output:

<details>

```bash
clang: /llvm-project/llvm/include/llvm/ADT/ArrayRef.h:248: const T &llvm::ArrayRef<clang::TemplateArgument>::operator[](size_t) const [T = clang::TemplateArgument]: Assertion `Index < Length && "Invalid index!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /llvm-project/build-debug-rc1/bin/clang -std=c++26 -fsyntax-only crash.cpp
1.      crash.cpp:11:44: current parser token ')'
  #0 0x000000000379588d llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /llvm-project/llvm/lib/Support/Unix/Signals.inc:842:11
  #1 0x0000000003795dbb PrintStackTraceSignalHandler(void*) /llvm-project/llvm/lib/Support/Unix/Signals.inc:924:1
  #2 0x0000000003793c74 llvm::sys::RunSignalHandlers() /llvm-project/llvm/lib/Support/Signals.cpp:108:5
  #3 0x000000000379517e llvm::sys::CleanupOnSignal(unsigned long) /llvm-project/llvm/lib/Support/Unix/Signals.inc:377:1
  #4 0x00000000036b94e2 (anonymous namespace)::CrashRecoveryContextImpl::HandleCrash(int, unsigned long) /llvm-project/llvm/lib/Support/CrashRecoveryContext.cpp:0:7
  #5 0x00000000036b9896 CrashRecoverySignalHandler(int) /llvm-project/llvm/lib/Support/CrashRecoveryContext.cpp:391:1
  #6 0x00007f6a26c3ebf0 __restore_rt (/lib64/libc.so.6+0x3ebf0)
  #7 0x00007f6a26c8bedc __pthread_kill_implementation (/lib64/libc.so.6+0x8bedc)
  #8 0x00007f6a26c3eb46 gsignal (/lib64/libc.so.6+0x3eb46)
  #9 0x00007f6a26c28833 abort (/lib64/libc.so.6+0x28833)
 #10 0x00007f6a26c2875b _nl_load_domain.cold (/lib64/libc.so.6+0x2875b)
 #11 0x00007f6a26c37886 (/lib64/libc.so.6+0x37886)
 #12 0x0000000003b1cf48 llvm::ArrayRef<clang::TemplateArgument>::operator[](unsigned long) const /llvm-project/llvm/include/llvm/ADT/ArrayRef.h:0:7
 #13 0x000000000722826e clang::Sema::getPackSubstitutedTemplateArgument(clang::TemplateArgument) const /llvm-project/clang/include/clang/Sema/Sema.h:11767:11
 #14 0x0000000007144396 (anonymous namespace)::HashParameterMapping::VisitTemplateTypeParmType(clang::TemplateTypeParmType*) /llvm-project/clang/lib/Sema/SemaConcept.cpp:295:11
 #15 0x00000000071441c0 clang::RecursiveASTVisitor<(anonymous namespace)::HashParameterMapping>::WalkUpFromTemplateTypeParmType(clang::TemplateTypeParmType*) /llvm-project/build-debug-rc1/tools/clang/include/clang/AST/TypeNodes.inc:80:1
 #16 0x00000000071417a7 clang::RecursiveASTVisitor<(anonymous namespace)::HashParameterMapping>::TraverseTemplateTypeParmType(clang::TemplateTypeParmType*, bool) /llvm-project/clang/include/clang/AST/RecursiveASTVisitor.h:1139:1
 #17 0x000000000713ddf6 clang::RecursiveASTVisitor<(anonymous namespace)::HashParameterMapping>::TraverseType(clang::QualType, bool) /llvm-project/build-debug-rc1/tools/clang/include/clang/AST/TypeNodes.inc:80:1
 #18 0x000000000713d179 clang::RecursiveASTVisitor<(anonymous namespace)::HashParameterMapping>::TraverseTemplateArgument(clang::TemplateArgument const&) /llvm-project/clang/include/clang/AST/RecursiveASTVisitor.h:889:5
 #19 0x000000000713d0e4 (anonymous namespace)::HashParameterMapping::TraverseTemplateArgument(clang::TemplateArgument const&) /llvm-project/clang/lib/Sema/SemaConcept.cpp:378:5
 #20 0x000000000713e595 clang::RecursiveASTVisitor<(anonymous namespace)::HashParameterMapping>::TraverseTemplateArguments(llvm::ArrayRef<clang::TemplateArgument>) /llvm-project/clang/include/clang/AST/RecursiveASTVisitor.h:951:5
 #21 0x000000000713d20d clang::RecursiveASTVisitor<(anonymous namespace)::HashParameterMapping>::TraverseTemplateArgument(clang::TemplateArgument const&) /llvm-project/clang/include/clang/AST/RecursiveASTVisitor.h:900:5
 #22 0x000000000713d0e4 (anonymous namespace)::HashParameterMapping::TraverseTemplateArgument(clang::TemplateArgument const&) /llvm-project/clang/lib/Sema/SemaConcept.cpp:378:5
 #23 0x000000000713c903 (anonymous namespace)::HashParameterMapping::VisitConstraint(clang::NormalizedConstraintWithParamMapping const&) /llvm-project/clang/lib/Sema/SemaConcept.cpp:422:23
 #24 0x000000000713b7a9 (anonymous namespace)::ConstraintSatisfactionChecker::Evaluate(clang::AtomicConstraint const&, clang::MultiLevelTemplateArgumentList const&) /llvm-project/clang/lib/Sema/SemaConcept.cpp:749:3
 #25 0x000000000713b2b5 (anonymous namespace)::ConstraintSatisfactionChecker::Evaluate(clang::NormalizedConstraint const&, clang::MultiLevelTemplateArgumentList const&) /llvm-project/clang/lib/Sema/SemaConcept.cpp:1095:12
 #26 0x000000000713bf94 (anonymous namespace)::ConstraintSatisfactionChecker::Evaluate(clang::ConceptIdConstraint const&, clang::MultiLevelTemplateArgumentList const&) /llvm-project/clang/lib/Sema/SemaConcept.cpp:1001:18
 #27 0x000000000713b2e3 (anonymous namespace)::ConstraintSatisfactionChecker::Evaluate(clang::NormalizedConstraint const&, clang::MultiLevelTemplateArgumentList const&) /llvm-project/clang/lib/Sema/SemaConcept.cpp:1102:12
 #28 0x00000000071af8f7 (anonymous namespace)::ConstraintSatisfactionChecker::EvaluateSlow(clang::FoldExpandedConstraint const&, clang::MultiLevelTemplateArgumentList const&) /llvm-project/clang/lib/Sema/SemaConcept.cpp:840:14
 #29 0x000000000713bcc5 (anonymous namespace)::ConstraintSatisfactionChecker::Evaluate(clang::FoldExpandedConstraint const&, clang::MultiLevelTemplateArgumentList const&) /llvm-project/clang/lib/Sema/SemaConcept.cpp:888:18
 #30 0x000000000713b2cc (anonymous namespace)::ConstraintSatisfactionChecker::Evaluate(clang::NormalizedConstraint const&, clang::MultiLevelTemplateArgumentList const&) /llvm-project/clang/lib/Sema/SemaConcept.cpp:1098:12
 #31 0x000000000713458d CheckConstraintSatisfaction(clang::Sema&, clang::NamedDecl const*, llvm::ArrayRef<clang::AssociatedConstraint>, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&, clang::Expr**, clang::ConceptReference const*) /llvm-project/clang/lib/Sema/SemaConcept.cpp:1169:25
 #32 0x0000000007134217 clang::Sema::CheckConstraintSatisfaction(llvm::PointerUnion<clang::NamedDecl const*, clang::concepts::NestedRequirement const*>, llvm::ArrayRef<clang::AssociatedConstraint>, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&, clang::ConceptReference const*, clang::Expr**) /llvm-project/clang/lib/Sema/SemaConcept.cpp:1230:7
 #33 0x0000000007136867 CheckFunctionConstraintsWithoutInstantiation(clang::Sema&, clang::SourceLocation, clang::FunctionTemplateDecl*, llvm::ArrayRef<clang::TemplateArgument>, clang::ConstraintSatisfaction&) /llvm-project/clang/lib/Sema/SemaConcept.cpp:1678:3
 #34 0x00000000071361ee clang::Sema::CheckFunctionTemplateConstraints(clang::SourceLocation, clang::FunctionDecl*, llvm::ArrayRef<clang::TemplateArgument>, clang::ConstraintSatisfaction&) /llvm-project/clang/lib/Sema/SemaConcept.cpp:1690:5
 #35 0x0000000007b97e0d clang::Sema::FinishTemplateArgumentDeduction(clang::FunctionTemplateDecl*, llvm::SmallVectorImpl<clang::DeducedTemplateArgument>&, unsigned int, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, llvm::SmallVectorImpl<clang::Sema::OriginalCallArg> const*, bool, bool, bool, llvm::function_ref<bool (bool)>) /llvm-project/clang/lib/Sema/SemaTemplateDeduction.cpp:3981:9
 #36 0x0000000007c237da clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, bool, bool, clang::QualType, clang::Expr::Classification, bool, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>, bool)>)::$_2::operator()() const /llvm-project/clang/lib/Sema/SemaTemplateDeduction.cpp:4719:14
 #37 0x0000000007c236c5 void llvm::function_ref<void ()>::callback_fn<clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, bool, bool, clang::QualType, clang::Expr::Classification, bool, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>, bool)>)::$_2>(long) /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
 #38 0x00000000022c8ce9 llvm::function_ref<void ()>::operator()() const /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
 #39 0x000000000923ad73 clang::runWithSufficientStackSpace(llvm::function_ref<void ()>, llvm::function_ref<void ()>) /llvm-project/clang/include/clang/Basic/Stack.h:55:3
 #40 0x000000000923abe0 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) /llvm-project/clang/lib/Basic/StackExhaustionHandler.cpp:21:1
 #41 0x0000000006fad508 clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) /llvm-project/clang/lib/Sema/Sema.cpp:628:1
 #42 0x0000000007b9a5a5 clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, bool, bool, clang::QualType, clang::Expr::Classification, bool, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>, bool)>) /llvm-project/clang/lib/Sema/SemaTemplateDeduction.cpp:4729:12
 #43 0x00000000079a8faf AddMethodTemplateCandidateImmediately(clang::Sema&, clang::OverloadCandidateSet&, clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::CXXRecordDecl*, clang::TemplateArgumentListInfo*, clang::QualType, clang::Expr::Classification, llvm::ArrayRef<clang::Expr*>, bool, bool, clang::OverloadCandidateParamOrder) /llvm-project/clang/lib/Sema/SemaOverload.cpp:7976:31
 #44 0x00000000079b56d4 AddTemplateOverloadCandidate(clang::Sema&, clang::OverloadCandidateSet&, clang::DeferredMethodTemplateOverloadCandidate&) /llvm-project/clang/lib/Sema/SemaOverload.cpp:11322:1
 #45 0x00000000079b5489 clang::OverloadCandidateSet::InjectNonDeducedTemplateCandidates(clang::Sema&) /llvm-project/clang/lib/Sema/SemaOverload.cpp:11357:7
 #46 0x00000000079a3d3f clang::OverloadCandidateSet::BestViableFunction(clang::Sema&, clang::SourceLocation, clang::OverloadCandidate*&) /llvm-project/clang/lib/Sema/SemaOverload.cpp:11445:3
 #47 0x00000000079c999f clang::Sema::BuildCallToObjectOfClassType(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation) /llvm-project/clang/lib/Sema/SemaOverload.cpp:16509:3
 #48 0x00000000074324df clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) /llvm-project/clang/lib/Sema/SemaExpr.cpp:6714:14
 #49 0x000000000744b48f clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) /llvm-project/clang/lib/Sema/SemaExpr.cpp:6598:7
 #50 0x0000000007d58f82 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::RebuildCallExpr(clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) /llvm-project/clang/lib/Sema/TreeTransform.h:2934:22
 #51 0x0000000007d44461 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCallExpr(clang::CallExpr*) /llvm-project/clang/lib/Sema/TreeTransform.h:13615:23
 #52 0x0000000007cbe92e clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) /llvm-project/build-debug-rc1/tools/clang/include/clang/AST/StmtNodes.inc:614:1
 #53 0x0000000007d50862 (anonymous namespace)::TemplateInstantiator::TransformExprRequirement(clang::concepts::ExprRequirement*) /llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:2639:31
 #54 0x0000000007d4efa9 (anonymous namespace)::TemplateInstantiator::TransformRequiresExprRequirements(llvm::ArrayRef<clang::concepts::Requirement*>, llvm::SmallVectorImpl<clang::concepts::Requirement*>&) /llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:1798:22
 #55 0x0000000007d4e91e clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformRequiresExpr(clang::RequiresExpr*) /llvm-project/clang/lib/Sema/TreeTransform.h:15214:7
 #56 0x0000000007d3ce34 (anonymous namespace)::TemplateInstantiator::TransformRequiresExpr(clang::RequiresExpr*) /llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:1770:40
 #57 0x0000000007cbdad3 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) /llvm-project/build-debug-rc1/tools/clang/include/clang/AST/StmtNodes.inc:116:1
 #58 0x0000000007cbd80d clang::Sema::SubstExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&) /llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:4358:23
 #59 0x0000000007cbf445 clang::Sema::SubstConstraintExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&) /llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:4377:10
 #60 0x00000000071af1de (anonymous namespace)::ConstraintSatisfactionChecker::EvaluateAtomicConstraint(clang::Expr const*, clang::MultiLevelTemplateArgumentList const&) /llvm-project/clang/lib/Sema/SemaConcept.cpp:537:11
 #61 0x000000000713caeb (anonymous namespace)::ConstraintSatisfactionChecker::EvaluateSlow(clang::AtomicConstraint const&, clang::MultiLevelTemplateArgumentList const&) /llvm-project/clang/lib/Sema/SemaConcept.cpp:676:38
 #62 0x000000000713b925 (anonymous namespace)::ConstraintSatisfactionChecker::Evaluate(clang::AtomicConstraint const&, clang::MultiLevelTemplateArgumentList const&) /llvm-project/clang/lib/Sema/SemaConcept.cpp:762:18
 #63 0x000000000713b2b5 (anonymous namespace)::ConstraintSatisfactionChecker::Evaluate(clang::NormalizedConstraint const&, clang::MultiLevelTemplateArgumentList const&) /llvm-project/clang/lib/Sema/SemaConcept.cpp:1095:12
 #64 0x000000000713bf94 (anonymous namespace)::ConstraintSatisfactionChecker::Evaluate(clang::ConceptIdConstraint const&, clang::MultiLevelTemplateArgumentList const&) /llvm-project/clang/lib/Sema/SemaConcept.cpp:1001:18
 #65 0x000000000713b2e3 (anonymous namespace)::ConstraintSatisfactionChecker::Evaluate(clang::NormalizedConstraint const&, clang::MultiLevelTemplateArgumentList const&) /llvm-project/clang/lib/Sema/SemaConcept.cpp:1102:12
 #66 0x000000000713458d CheckConstraintSatisfaction(clang::Sema&, clang::NamedDecl const*, llvm::ArrayRef<clang::AssociatedConstraint>, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&, clang::Expr**, clang::ConceptReference const*) /llvm-project/clang/lib/Sema/SemaConcept.cpp:1169:25
 #67 0x0000000007134217 clang::Sema::CheckConstraintSatisfaction(llvm::PointerUnion<clang::NamedDecl const*, clang::concepts::NestedRequirement const*>, llvm::ArrayRef<clang::AssociatedConstraint>, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&, clang::ConceptReference const*, clang::Expr**) /llvm-project/clang/lib/Sema/SemaConcept.cpp:1230:7
 #68 0x0000000007af8ccf clang::Sema::CheckConceptTemplateId(clang::CXXScopeSpec const&, clang::SourceLocation, clang::DeclarationNameInfo const&, clang::NamedDecl*, clang::TemplateDecl*, clang::TemplateArgumentListInfo const*, bool) /llvm-project/clang/lib/Sema/SemaTemplate.cpp:4909:11
 #69 0x0000000007af9031 clang::Sema::BuildTemplateIdExpr(clang::CXXScopeSpec const&, clang::SourceLocation, clang::LookupResult&, bool, clang::TemplateArgumentListInfo const*) /llvm-project/clang/lib/Sema/SemaTemplate.cpp:4966:12
 #70 0x0000000007431d2e clang::Sema::ActOnIdExpression(clang::Scope*, clang::CXXScopeSpec&, clang::SourceLocation, clang::UnqualifiedId&, bool, bool, clang::CorrectionCandidateCallback*, bool, clang::Token*) /llvm-project/clang/lib/Sema/SemaExpr.cpp:2914:12
 #71 0x0000000006e98bbc clang::Parser::tryParseCXXIdExpression(clang::CXXScopeSpec&, bool, clang::Token&) /llvm-project/clang/lib/Parse/ParseExprCXX.cpp:620:17
 #72 0x0000000006e9a452 clang::Parser::ParseCXXIdExpression(bool) /llvm-project/clang/lib/Parse/ParseExprCXX.cpp:671:7
 #73 0x0000000006e85940 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypoCorrectionTypeBehavior, bool, bool*) /llvm-project/clang/lib/Parse/ParseExpr.cpp:1415:11
 #74 0x0000000006e837b8 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypoCorrectionTypeBehavior, bool, bool*) /llvm-project/clang/lib/Parse/ParseExpr.cpp:908:18
 #75 0x0000000006e800fc clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) /llvm-project/clang/lib/Parse/ParseExpr.cpp:567:20
 #76 0x0000000006e84b37 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypoCorrectionTypeBehavior, bool, bool*) /llvm-project/clang/lib/Parse/ParseExpr.cpp:1158:11
 #77 0x0000000006e800fc clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) /llvm-project/clang/lib/Parse/ParseExpr.cpp:567:20
 #78 0x0000000006e8252f clang::Parser::ParseConstantExpressionInExprEvalContext(clang::TypoCorrectionTypeBehavior) /llvm-project/clang/lib/Parse/ParseExpr.cpp:127:18
 #79 0x0000000006e67540 clang::Parser::ParseStaticAssertDeclaration(clang::SourceLocation&) /llvm-project/clang/lib/Parse/ParseDeclCXX.cpp:954:25
 #80 0x0000000006e39dea clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) /llvm-project/clang/lib/Parse/ParseDecl.cpp:1918:16
 #81 0x0000000006e03c36 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) /llvm-project/clang/lib/Parse/Parser.cpp:944:14
 #82 0x0000000006e02054 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) /llvm-project/clang/lib/Parse/Parser.cpp:743:12
 #83 0x0000000006deeb3c clang::ParseAST(clang::Sema&, bool, bool) /llvm-project/clang/lib/Parse/ParseAST.cpp:170:16
 #84 0x0000000004980b99 clang::ASTFrontendAction::ExecuteAction() /llvm-project/clang/lib/Frontend/FrontendAction.cpp:1435:1
 #85 0x0000000004980576 clang::FrontendAction::Execute() /llvm-project/clang/lib/Frontend/FrontendAction.cpp:1317:7
 #86 0x0000000004893353 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) /llvm-project/clang/lib/Frontend/CompilerInstance.cpp:1006:23
 #87 0x0000000004b5eba5 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) /llvm-project/clang/lib/FrontendTool/ExecuteCompilerInvocation.cpp:310:8
 #88 0x0000000001846ae6 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) /llvm-project/clang/tools/driver/cc1_main.cpp:304:13
 #89 0x000000000183a318 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) /llvm-project/clang/tools/driver/driver.cpp:226:5
 #90 0x000000000183adf1 clang_main(int, char**, llvm::ToolContext const&)::$_0::operator()(llvm::SmallVectorImpl<char const*>&) const /llvm-project/clang/tools/driver/driver.cpp:376:12
 #91 0x000000000183ad9d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::$_0>(long, llvm::SmallVectorImpl<char const*>&) /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
 #92 0x00000000046d6581 llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::operator()(llvm::SmallVectorImpl<char const*>&) const /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
 #93 0x00000000046d2f88 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::$_0::operator()() const /llvm-project/clang/lib/Driver/Job.cpp:442:34
 #94 0x00000000046d2f55 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::$_0>(long) /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
 #95 0x00000000022c8ce9 llvm::function_ref<void ()>::operator()() const /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
 #96 0x00000000036b92ff llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) /llvm-project/llvm/lib/Support/CrashRecoveryContext.cpp:427:3
 #97 0x00000000046d2918 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const /llvm-project/clang/lib/Driver/Job.cpp:442:7
 #98 0x000000000466ab62 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const /llvm-project/clang/lib/Driver/Compilation.cpp:196:15
 #99 0x000000000466ad46 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const /llvm-project/clang/lib/Driver/Compilation.cpp:246:13
#100 0x0000000004686ae1 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) /llvm-project/clang/lib/Driver/Driver.cpp:2265:7
#101 0x0000000001839e26 clang_main(int, char**, llvm::ToolContext const&) /llvm-project/clang/tools/driver/driver.cpp:414:9
#102 0x000000000186c955 main /llvm-project/build-debug-rc1/tools/clang/tools/driver/clang-driver.cpp:17:3
#103 0x00007f6a26c295d0 __libc_start_call_main (/lib64/libc.so.6+0x295d0)
#104 0x00007f6a26c29680 __libc_start_main@GLIBC_2.2.5 (/lib64/libc.so.6+0x29680)
#105 0x0000000001838b75 _start (/llvm-project/build-debug-rc1/bin/clang+0x1838b75)
clang: error: clang frontend command failed with exit code 134 (use -v to see invocation)
clang version 22.1.0-rc1 (https://github.com/llvm/llvm-project.git 8164f1a0c17b192e133817436bdb07598b7402a3)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /llvm-project/build-debug-rc1/bin
Build config: +unoptimized, +assertions
```
</details>

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (matt-gb)

<details>
https://godbolt.org/z/YKoe6vv9M

This code crashes clang 22 in C++26 mode (`-std=c++2c` or `-std=c++26`). Seen with clang 22.1.0-rc1 (built locally from 8164f1a) and godbolt's clang-trunk. Release builds segfault, assertion-enabled builds hit a bounds check in `ArrayRef.h`.

Clang 21.1.0 and GCC 15.2 both accept it with no warnings.

Bisected to e9972de (#<!-- -->161671).

#### Repro (reduced from some code in stdexec 24.09):
```cpp
// crash.cpp

template &lt;class _Fun, class... _As&gt;
concept __callable = requires (_Fun __fun, _As...) { __fun(); };

template &lt;class... _Args&gt;
struct __mdispatch {
  template &lt;class... _Ts&gt;
    requires (__callable&lt;_Args, _Ts...&gt; &amp;&amp; ...)
  void operator()();
};

static_assert(!__callable&lt;__mdispatch&lt;int&gt;&gt;);
```
```bash
clang-22 -std=c++26 -fsyntax-only crash.cpp # No crash with -std=c++23/2b
```

#### Clang 22.1.0-rc1 built with options:
```
cmake -S llvm -B build-debug-rc1 -G Ninja \
  -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_C_COMPILER=/usr/bin/clang-19 \
  -DCMAKE_CXX_COMPILER=/usr/bin/clang++-19 \
  -DLLVM_ENABLE_PROJECTS=clang \
  -DLLVM_TARGETS_TO_BUILD=X86
```

#### Crash output:

&lt;details&gt;

```bash
clang: /llvm-project/llvm/include/llvm/ADT/ArrayRef.h:248: const T &amp;llvm::ArrayRef&lt;clang::TemplateArgument&gt;::operator[](size_t) const [T = clang::TemplateArgument]: Assertion `Index &lt; Length &amp;&amp; "Invalid index!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /llvm-project/build-debug-rc1/bin/clang -std=c++26 -fsyntax-only crash.cpp
1.      crash.cpp:11:44: current parser token ')'
  #<!-- -->0 0x000000000379588d llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /llvm-project/llvm/lib/Support/Unix/Signals.inc:842:11
  #<!-- -->1 0x0000000003795dbb PrintStackTraceSignalHandler(void*) /llvm-project/llvm/lib/Support/Unix/Signals.inc:924:1
  #<!-- -->2 0x0000000003793c74 llvm::sys::RunSignalHandlers() /llvm-project/llvm/lib/Support/Signals.cpp:108:5
  #<!-- -->3 0x000000000379517e llvm::sys::CleanupOnSignal(unsigned long) /llvm-project/llvm/lib/Support/Unix/Signals.inc:377:1
  #<!-- -->4 0x00000000036b94e2 (anonymous namespace)::CrashRecoveryContextImpl::HandleCrash(int, unsigned long) /llvm-project/llvm/lib/Support/CrashRecoveryContext.cpp:0:7
  #<!-- -->5 0x00000000036b9896 CrashRecoverySignalHandler(int) /llvm-project/llvm/lib/Support/CrashRecoveryContext.cpp:391:1
  #<!-- -->6 0x00007f6a26c3ebf0 __restore_rt (/lib64/libc.so.6+0x3ebf0)
  #<!-- -->7 0x00007f6a26c8bedc __pthread_kill_implementation (/lib64/libc.so.6+0x8bedc)
  #<!-- -->8 0x00007f6a26c3eb46 gsignal (/lib64/libc.so.6+0x3eb46)
  #<!-- -->9 0x00007f6a26c28833 abort (/lib64/libc.so.6+0x28833)
 #<!-- -->10 0x00007f6a26c2875b _nl_load_domain.cold (/lib64/libc.so.6+0x2875b)
 #<!-- -->11 0x00007f6a26c37886 (/lib64/libc.so.6+0x37886)
 #<!-- -->12 0x0000000003b1cf48 llvm::ArrayRef&lt;clang::TemplateArgument&gt;::operator[](unsigned long) const /llvm-project/llvm/include/llvm/ADT/ArrayRef.h:0:7
 #<!-- -->13 0x000000000722826e clang::Sema::getPackSubstitutedTemplateArgument(clang::TemplateArgument) const /llvm-project/clang/include/clang/Sema/Sema.h:11767:11
 #<!-- -->14 0x0000000007144396 (anonymous namespace)::HashParameterMapping::VisitTemplateTypeParmType(clang::TemplateTypeParmType*) /llvm-project/clang/lib/Sema/SemaConcept.cpp:295:11
 #<!-- -->15 0x00000000071441c0 clang::RecursiveASTVisitor&lt;(anonymous namespace)::HashParameterMapping&gt;::WalkUpFromTemplateTypeParmType(clang::TemplateTypeParmType*) /llvm-project/build-debug-rc1/tools/clang/include/clang/AST/TypeNodes.inc:80:1
 #<!-- -->16 0x00000000071417a7 clang::RecursiveASTVisitor&lt;(anonymous namespace)::HashParameterMapping&gt;::TraverseTemplateTypeParmType(clang::TemplateTypeParmType*, bool) /llvm-project/clang/include/clang/AST/RecursiveASTVisitor.h:1139:1
 #<!-- -->17 0x000000000713ddf6 clang::RecursiveASTVisitor&lt;(anonymous namespace)::HashParameterMapping&gt;::TraverseType(clang::QualType, bool) /llvm-project/build-debug-rc1/tools/clang/include/clang/AST/TypeNodes.inc:80:1
 #<!-- -->18 0x000000000713d179 clang::RecursiveASTVisitor&lt;(anonymous namespace)::HashParameterMapping&gt;::TraverseTemplateArgument(clang::TemplateArgument const&amp;) /llvm-project/clang/include/clang/AST/RecursiveASTVisitor.h:889:5
 #<!-- -->19 0x000000000713d0e4 (anonymous namespace)::HashParameterMapping::TraverseTemplateArgument(clang::TemplateArgument const&amp;) /llvm-project/clang/lib/Sema/SemaConcept.cpp:378:5
 #<!-- -->20 0x000000000713e595 clang::RecursiveASTVisitor&lt;(anonymous namespace)::HashParameterMapping&gt;::TraverseTemplateArguments(llvm::ArrayRef&lt;clang::TemplateArgument&gt;) /llvm-project/clang/include/clang/AST/RecursiveASTVisitor.h:951:5
 #<!-- -->21 0x000000000713d20d clang::RecursiveASTVisitor&lt;(anonymous namespace)::HashParameterMapping&gt;::TraverseTemplateArgument(clang::TemplateArgument const&amp;) /llvm-project/clang/include/clang/AST/RecursiveASTVisitor.h:900:5
 #<!-- -->22 0x000000000713d0e4 (anonymous namespace)::HashParameterMapping::TraverseTemplateArgument(clang::TemplateArgument const&amp;) /llvm-project/clang/lib/Sema/SemaConcept.cpp:378:5
 #<!-- -->23 0x000000000713c903 (anonymous namespace)::HashParameterMapping::VisitConstraint(clang::NormalizedConstraintWithParamMapping const&amp;) /llvm-project/clang/lib/Sema/SemaConcept.cpp:422:23
 #<!-- -->24 0x000000000713b7a9 (anonymous namespace)::ConstraintSatisfactionChecker::Evaluate(clang::AtomicConstraint const&amp;, clang::MultiLevelTemplateArgumentList const&amp;) /llvm-project/clang/lib/Sema/SemaConcept.cpp:749:3
 #<!-- -->25 0x000000000713b2b5 (anonymous namespace)::ConstraintSatisfactionChecker::Evaluate(clang::NormalizedConstraint const&amp;, clang::MultiLevelTemplateArgumentList const&amp;) /llvm-project/clang/lib/Sema/SemaConcept.cpp:1095:12
 #<!-- -->26 0x000000000713bf94 (anonymous namespace)::ConstraintSatisfactionChecker::Evaluate(clang::ConceptIdConstraint const&amp;, clang::MultiLevelTemplateArgumentList const&amp;) /llvm-project/clang/lib/Sema/SemaConcept.cpp:1001:18
 #<!-- -->27 0x000000000713b2e3 (anonymous namespace)::ConstraintSatisfactionChecker::Evaluate(clang::NormalizedConstraint const&amp;, clang::MultiLevelTemplateArgumentList const&amp;) /llvm-project/clang/lib/Sema/SemaConcept.cpp:1102:12
 #<!-- -->28 0x00000000071af8f7 (anonymous namespace)::ConstraintSatisfactionChecker::EvaluateSlow(clang::FoldExpandedConstraint const&amp;, clang::MultiLevelTemplateArgumentList const&amp;) /llvm-project/clang/lib/Sema/SemaConcept.cpp:840:14
 #<!-- -->29 0x000000000713bcc5 (anonymous namespace)::ConstraintSatisfactionChecker::Evaluate(clang::FoldExpandedConstraint const&amp;, clang::MultiLevelTemplateArgumentList const&amp;) /llvm-project/clang/lib/Sema/SemaConcept.cpp:888:18
 #<!-- -->30 0x000000000713b2cc (anonymous namespace)::ConstraintSatisfactionChecker::Evaluate(clang::NormalizedConstraint const&amp;, clang::MultiLevelTemplateArgumentList const&amp;) /llvm-project/clang/lib/Sema/SemaConcept.cpp:1098:12
 #<!-- -->31 0x000000000713458d CheckConstraintSatisfaction(clang::Sema&amp;, clang::NamedDecl const*, llvm::ArrayRef&lt;clang::AssociatedConstraint&gt;, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceRange, clang::ConstraintSatisfaction&amp;, clang::Expr**, clang::ConceptReference const*) /llvm-project/clang/lib/Sema/SemaConcept.cpp:1169:25
 #<!-- -->32 0x0000000007134217 clang::Sema::CheckConstraintSatisfaction(llvm::PointerUnion&lt;clang::NamedDecl const*, clang::concepts::NestedRequirement const*&gt;, llvm::ArrayRef&lt;clang::AssociatedConstraint&gt;, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceRange, clang::ConstraintSatisfaction&amp;, clang::ConceptReference const*, clang::Expr**) /llvm-project/clang/lib/Sema/SemaConcept.cpp:1230:7
 #<!-- -->33 0x0000000007136867 CheckFunctionConstraintsWithoutInstantiation(clang::Sema&amp;, clang::SourceLocation, clang::FunctionTemplateDecl*, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::ConstraintSatisfaction&amp;) /llvm-project/clang/lib/Sema/SemaConcept.cpp:1678:3
 #<!-- -->34 0x00000000071361ee clang::Sema::CheckFunctionTemplateConstraints(clang::SourceLocation, clang::FunctionDecl*, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::ConstraintSatisfaction&amp;) /llvm-project/clang/lib/Sema/SemaConcept.cpp:1690:5
 #<!-- -->35 0x0000000007b97e0d clang::Sema::FinishTemplateArgumentDeduction(clang::FunctionTemplateDecl*, llvm::SmallVectorImpl&lt;clang::DeducedTemplateArgument&gt;&amp;, unsigned int, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, llvm::SmallVectorImpl&lt;clang::Sema::OriginalCallArg&gt; const*, bool, bool, bool, llvm::function_ref&lt;bool (bool)&gt;) /llvm-project/clang/lib/Sema/SemaTemplateDeduction.cpp:3981:9
 #<!-- -->36 0x0000000007c237da clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, bool, bool, bool, clang::QualType, clang::Expr::Classification, bool, llvm::function_ref&lt;bool (llvm::ArrayRef&lt;clang::QualType&gt;, bool)&gt;)::$_2::operator()() const /llvm-project/clang/lib/Sema/SemaTemplateDeduction.cpp:4719:14
 #<!-- -->37 0x0000000007c236c5 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, bool, bool, bool, clang::QualType, clang::Expr::Classification, bool, llvm::function_ref&lt;bool (llvm::ArrayRef&lt;clang::QualType&gt;, bool)&gt;)::$_2&gt;(long) /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
 #<!-- -->38 0x00000000022c8ce9 llvm::function_ref&lt;void ()&gt;::operator()() const /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
 #<!-- -->39 0x000000000923ad73 clang::runWithSufficientStackSpace(llvm::function_ref&lt;void ()&gt;, llvm::function_ref&lt;void ()&gt;) /llvm-project/clang/include/clang/Basic/Stack.h:55:3
 #<!-- -->40 0x000000000923abe0 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) /llvm-project/clang/lib/Basic/StackExhaustionHandler.cpp:21:1
 #<!-- -->41 0x0000000006fad508 clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) /llvm-project/clang/lib/Sema/Sema.cpp:628:1
 #<!-- -->42 0x0000000007b9a5a5 clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, bool, bool, bool, clang::QualType, clang::Expr::Classification, bool, llvm::function_ref&lt;bool (llvm::ArrayRef&lt;clang::QualType&gt;, bool)&gt;) /llvm-project/clang/lib/Sema/SemaTemplateDeduction.cpp:4729:12
 #<!-- -->43 0x00000000079a8faf AddMethodTemplateCandidateImmediately(clang::Sema&amp;, clang::OverloadCandidateSet&amp;, clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::CXXRecordDecl*, clang::TemplateArgumentListInfo*, clang::QualType, clang::Expr::Classification, llvm::ArrayRef&lt;clang::Expr*&gt;, bool, bool, clang::OverloadCandidateParamOrder) /llvm-project/clang/lib/Sema/SemaOverload.cpp:7976:31
 #<!-- -->44 0x00000000079b56d4 AddTemplateOverloadCandidate(clang::Sema&amp;, clang::OverloadCandidateSet&amp;, clang::DeferredMethodTemplateOverloadCandidate&amp;) /llvm-project/clang/lib/Sema/SemaOverload.cpp:11322:1
 #<!-- -->45 0x00000000079b5489 clang::OverloadCandidateSet::InjectNonDeducedTemplateCandidates(clang::Sema&amp;) /llvm-project/clang/lib/Sema/SemaOverload.cpp:11357:7
 #<!-- -->46 0x00000000079a3d3f clang::OverloadCandidateSet::BestViableFunction(clang::Sema&amp;, clang::SourceLocation, clang::OverloadCandidate*&amp;) /llvm-project/clang/lib/Sema/SemaOverload.cpp:11445:3
 #<!-- -->47 0x00000000079c999f clang::Sema::BuildCallToObjectOfClassType(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation) /llvm-project/clang/lib/Sema/SemaOverload.cpp:16509:3
 #<!-- -->48 0x00000000074324df clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, bool) /llvm-project/clang/lib/Sema/SemaExpr.cpp:6714:14
 #<!-- -->49 0x000000000744b48f clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*) /llvm-project/clang/lib/Sema/SemaExpr.cpp:6598:7
 #<!-- -->50 0x0000000007d58f82 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::RebuildCallExpr(clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*) /llvm-project/clang/lib/Sema/TreeTransform.h:2934:22
 #<!-- -->51 0x0000000007d44461 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCallExpr(clang::CallExpr*) /llvm-project/clang/lib/Sema/TreeTransform.h:13615:23
 #<!-- -->52 0x0000000007cbe92e clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) /llvm-project/build-debug-rc1/tools/clang/include/clang/AST/StmtNodes.inc:614:1
 #<!-- -->53 0x0000000007d50862 (anonymous namespace)::TemplateInstantiator::TransformExprRequirement(clang::concepts::ExprRequirement*) /llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:2639:31
 #<!-- -->54 0x0000000007d4efa9 (anonymous namespace)::TemplateInstantiator::TransformRequiresExprRequirements(llvm::ArrayRef&lt;clang::concepts::Requirement*&gt;, llvm::SmallVectorImpl&lt;clang::concepts::Requirement*&gt;&amp;) /llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:1798:22
 #<!-- -->55 0x0000000007d4e91e clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformRequiresExpr(clang::RequiresExpr*) /llvm-project/clang/lib/Sema/TreeTransform.h:15214:7
 #<!-- -->56 0x0000000007d3ce34 (anonymous namespace)::TemplateInstantiator::TransformRequiresExpr(clang::RequiresExpr*) /llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:1770:40
 #<!-- -->57 0x0000000007cbdad3 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) /llvm-project/build-debug-rc1/tools/clang/include/clang/AST/StmtNodes.inc:116:1
 #<!-- -->58 0x0000000007cbd80d clang::Sema::SubstExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&amp;) /llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:4358:23
 #<!-- -->59 0x0000000007cbf445 clang::Sema::SubstConstraintExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&amp;) /llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:4377:10
 #<!-- -->60 0x00000000071af1de (anonymous namespace)::ConstraintSatisfactionChecker::EvaluateAtomicConstraint(clang::Expr const*, clang::MultiLevelTemplateArgumentList const&amp;) /llvm-project/clang/lib/Sema/SemaConcept.cpp:537:11
 #<!-- -->61 0x000000000713caeb (anonymous namespace)::ConstraintSatisfactionChecker::EvaluateSlow(clang::AtomicConstraint const&amp;, clang::MultiLevelTemplateArgumentList const&amp;) /llvm-project/clang/lib/Sema/SemaConcept.cpp:676:38
 #<!-- -->62 0x000000000713b925 (anonymous namespace)::ConstraintSatisfactionChecker::Evaluate(clang::AtomicConstraint const&amp;, clang::MultiLevelTemplateArgumentList const&amp;) /llvm-project/clang/lib/Sema/SemaConcept.cpp:762:18
 #<!-- -->63 0x000000000713b2b5 (anonymous namespace)::ConstraintSatisfactionChecker::Evaluate(clang::NormalizedConstraint const&amp;, clang::MultiLevelTemplateArgumentList const&amp;) /llvm-project/clang/lib/Sema/SemaConcept.cpp:1095:12
 #<!-- -->64 0x000000000713bf94 (anonymous namespace)::ConstraintSatisfactionChecker::Evaluate(clang::ConceptIdConstraint const&amp;, clang::MultiLevelTemplateArgumentList const&amp;) /llvm-project/clang/lib/Sema/SemaConcept.cpp:1001:18
 #<!-- -->65 0x000000000713b2e3 (anonymous namespace)::ConstraintSatisfactionChecker::Evaluate(clang::NormalizedConstraint const&amp;, clang::MultiLevelTemplateArgumentList const&amp;) /llvm-project/clang/lib/Sema/SemaConcept.cpp:1102:12
 #<!-- -->66 0x000000000713458d CheckConstraintSatisfaction(clang::Sema&amp;, clang::NamedDecl const*, llvm::ArrayRef&lt;clang::AssociatedConstraint&gt;, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceRange, clang::ConstraintSatisfaction&amp;, clang::Expr**, clang::ConceptReference const*) /llvm-project/clang/lib/Sema/SemaConcept.cpp:1169:25
 #<!-- -->67 0x0000000007134217 clang::Sema::CheckConstraintSatisfaction(llvm::PointerUnion&lt;clang::NamedDecl const*, clang::concepts::NestedRequirement const*&gt;, llvm::ArrayRef&lt;clang::AssociatedConstraint&gt;, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceRange, clang::ConstraintSatisfaction&amp;, clang::ConceptReference const*, clang::Expr**) /llvm-project/clang/lib/Sema/SemaConcept.cpp:1230:7
 #<!-- -->68 0x0000000007af8ccf clang::Sema::CheckConceptTemplateId(clang::CXXScopeSpec const&amp;, clang::SourceLocation, clang::DeclarationNameInfo const&amp;, clang::NamedDecl*, clang::TemplateDecl*, clang::TemplateArgumentListInfo const*, bool) /llvm-project/clang/lib/Sema/SemaTemplate.cpp:4909:11
 #<!-- -->69 0x0000000007af9031 clang::Sema::BuildTemplateIdExpr(clang::CXXScopeSpec const&amp;, clang::SourceLocation, clang::LookupResult&amp;, bool, clang::TemplateArgumentListInfo const*) /llvm-project/clang/lib/Sema/SemaTemplate.cpp:4966:12
 #<!-- -->70 0x0000000007431d2e clang::Sema::ActOnIdExpression(clang::Scope*, clang::CXXScopeSpec&amp;, clang::SourceLocation, clang::UnqualifiedId&amp;, bool, bool, clang::CorrectionCandidateCallback*, bool, clang::Token*) /llvm-project/clang/lib/Sema/SemaExpr.cpp:2914:12
 #<!-- -->71 0x0000000006e98bbc clang::Parser::tryParseCXXIdExpression(clang::CXXScopeSpec&amp;, bool, clang::Token&amp;) /llvm-project/clang/lib/Parse/ParseExprCXX.cpp:620:17
 #<!-- -->72 0x0000000006e9a452 clang::Parser::ParseCXXIdExpression(bool) /llvm-project/clang/lib/Parse/ParseExprCXX.cpp:671:7
 #<!-- -->73 0x0000000006e85940 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&amp;, clang::TypoCorrectionTypeBehavior, bool, bool*) /llvm-project/clang/lib/Parse/ParseExpr.cpp:1415:11
 #<!-- -->74 0x0000000006e837b8 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&amp;, clang::TypoCorrectionTypeBehavior, bool, bool*) /llvm-project/clang/lib/Parse/ParseExpr.cpp:908:18
 #<!-- -->75 0x0000000006e800fc clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) /llvm-project/clang/lib/Parse/ParseExpr.cpp:567:20
 #<!-- -->76 0x0000000006e84b37 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&amp;, clang::TypoCorrectionTypeBehavior, bool, bool*) /llvm-project/clang/lib/Parse/ParseExpr.cpp:1158:11
 #<!-- -->77 0x0000000006e800fc clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) /llvm-project/clang/lib/Parse/ParseExpr.cpp:567:20
 #<!-- -->78 0x0000000006e8252f clang::Parser::ParseConstantExpressionInExprEvalContext(clang::TypoCorrectionTypeBehavior) /llvm-project/clang/lib/Parse/ParseExpr.cpp:127:18
 #<!-- -->79 0x0000000006e67540 clang::Parser::ParseStaticAssertDeclaration(clang::SourceLocation&amp;) /llvm-project/clang/lib/Parse/ParseDeclCXX.cpp:954:25
 #<!-- -->80 0x0000000006e39dea clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) /llvm-project/clang/lib/Parse/ParseDecl.cpp:1918:16
 #<!-- -->81 0x0000000006e03c36 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) /llvm-project/clang/lib/Parse/Parser.cpp:944:14
 #<!-- -->82 0x0000000006e02054 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) /llvm-project/clang/lib/Parse/Parser.cpp:743:12
 #<!-- -->83 0x0000000006deeb3c clang::ParseAST(clang::Sema&amp;, bool, bool) /llvm-project/clang/lib/Parse/ParseAST.cpp:170:16
 #<!-- -->84 0x0000000004980b99 clang::ASTFrontendAction::ExecuteAction() /llvm-project/clang/lib/Frontend/FrontendAction.cpp:1435:1
 #<!-- -->85 0x0000000004980576 clang::FrontendAction::Execute() /llvm-project/clang/lib/Frontend/FrontendAction.cpp:1317:7
 #<!-- -->86 0x0000000004893353 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) /llvm-project/clang/lib/Frontend/CompilerInstance.cpp:1006:23
 #<!-- -->87 0x0000000004b5eba5 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) /llvm-project/clang/lib/FrontendTool/ExecuteCompilerInvocation.cpp:310:8
 #<!-- -->88 0x0000000001846ae6 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) /llvm-project/clang/tools/driver/cc1_main.cpp:304:13
 #<!-- -->89 0x000000000183a318 ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) /llvm-project/clang/tools/driver/driver.cpp:226:5
 #<!-- -->90 0x000000000183adf1 clang_main(int, char**, llvm::ToolContext const&amp;)::$_0::operator()(llvm::SmallVectorImpl&lt;char const*&gt;&amp;) const /llvm-project/clang/tools/driver/driver.cpp:376:12
 #<!-- -->91 0x000000000183ad9d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::$_0&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
 #<!-- -->92 0x00000000046d6581 llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::operator()(llvm::SmallVectorImpl&lt;char const*&gt;&amp;) const /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
 #<!-- -->93 0x00000000046d2f88 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::$_0::operator()() const /llvm-project/clang/lib/Driver/Job.cpp:442:34
 #<!-- -->94 0x00000000046d2f55 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::$_0&gt;(long) /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
 #<!-- -->95 0x00000000022c8ce9 llvm::function_ref&lt;void ()&gt;::operator()() const /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
 #<!-- -->96 0x00000000036b92ff llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) /llvm-project/llvm/lib/Support/CrashRecoveryContext.cpp:427:3
 #<!-- -->97 0x00000000046d2918 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const /llvm-project/clang/lib/Driver/Job.cpp:442:7
 #<!-- -->98 0x000000000466ab62 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const /llvm-project/clang/lib/Driver/Compilation.cpp:196:15
 #<!-- -->99 0x000000000466ad46 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const /llvm-project/clang/lib/Driver/Compilation.cpp:246:13
#<!-- -->100 0x0000000004686ae1 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) /llvm-project/clang/lib/Driver/Driver.cpp:2265:7
#<!-- -->101 0x0000000001839e26 clang_main(int, char**, llvm::ToolContext const&amp;) /llvm-project/clang/tools/driver/driver.cpp:414:9
#<!-- -->102 0x000000000186c955 main /llvm-project/build-debug-rc1/tools/clang/tools/driver/clang-driver.cpp:17:3
#<!-- -->103 0x00007f6a26c295d0 __libc_start_call_main (/lib64/libc.so.6+0x295d0)
#<!-- -->104 0x00007f6a26c29680 __libc_start_main@<!-- -->GLIBC_2.2.5 (/lib64/libc.so.6+0x29680)
#<!-- -->105 0x0000000001838b75 _start (/llvm-project/build-debug-rc1/bin/clang+0x1838b75)
clang: error: clang frontend command failed with exit code 134 (use -v to see invocation)
clang version 22.1.0-rc1 (https://github.com/llvm/llvm-project.git 8164f1a0c17b192e133817436bdb07598b7402a3)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /llvm-project/build-debug-rc1/bin
Build config: +unoptimized, +assertions
```
&lt;/details&gt;
</details>


---

