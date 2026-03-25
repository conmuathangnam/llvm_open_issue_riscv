# ICE on use of pack in requires clause of lambda in pack expansion in requires clause of constructor

**Author:** Eczbek
**URL:** https://github.com/llvm/llvm-project/issues/147772
**Status:** Closed
**Labels:** clang:frontend, regression, concepts, confirmed, crash, lambda
**Closed Date:** 2025-07-10T08:17:40Z

## Body

https://godbolt.org/z/KPj3Wb3Kx

```cpp
template<int...>
struct seq {};

using arr = char[1];

struct foo {
	template<int... i>
	constexpr foo(seq<i...>) requires requires {
		arr { [](auto) requires(i, true) { return 0; }(i)... };
	} {}
};

constexpr auto bar = foo(seq<0>());

int main() {}
```

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 <source>
1.	<source>:13:35: current parser token ';'
 #0 0x0000000003c4aaf8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c4aaf8)
 #1 0x0000000003c484cc llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c484cc)
 #2 0x0000000003b988c8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000070b3dbe42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x0000000007741dff clang::TemplateArgument::isPackExpansion() const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x7741dff)
 #5 0x0000000006d5ae43 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
 #6 0x0000000006d7234b clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformBinaryOperator(clang::BinaryOperator*) SemaTemplateInstantiate.cpp:0:0
 #7 0x0000000006d59e18 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
 #8 0x0000000006d604b5 clang::Sema::SubstConstraintExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6d604b5)
 #9 0x00000000065148a5 calculateConstraintSatisfaction(clang::Sema&, clang::Expr const*, clang::NamedDecl const*, clang::SourceLocation, clang::MultiLevelTemplateArgumentList const&, clang::ConstraintSatisfaction&) SemaConcept.cpp:0:0
#10 0x00000000065168fc CheckConstraintSatisfaction(clang::Sema&, clang::NamedDecl const*, llvm::ArrayRef<clang::AssociatedConstraint>, llvm::SmallVectorImpl<clang::Expr*>&, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&) SemaConcept.cpp:0:0
#11 0x0000000006519ba7 clang::Sema::CheckConstraintSatisfaction(clang::NamedDecl const*, llvm::ArrayRef<clang::AssociatedConstraint>, llvm::SmallVectorImpl<clang::Expr*>&, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&) (.constprop.0) SemaConcept.cpp:0:0
#12 0x000000000651a75e clang::Sema::CheckFunctionTemplateConstraints(clang::SourceLocation, clang::FunctionDecl*, llvm::ArrayRef<clang::TemplateArgument>, clang::ConstraintSatisfaction&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x651a75e)
#13 0x0000000006cfa49e clang::Sema::FinishTemplateArgumentDeduction(clang::FunctionTemplateDecl*, llvm::SmallVectorImpl<clang::DeducedTemplateArgument>&, unsigned int, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, llvm::SmallVectorImpl<clang::Sema::OriginalCallArg> const*, bool, bool, llvm::function_ref<bool (bool)>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6cfa49e)
#14 0x0000000006cfaf97 void llvm::function_ref<void ()>::callback_fn<clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, bool, bool, clang::QualType, clang::Expr::Classification, bool, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>, bool)>)::'lambda1'()>(long) SemaTemplateDeduction.cpp:0:0
#15 0x00000000079d8e51 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x79d8e51)
#16 0x0000000006cf15cb clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, bool, bool, clang::QualType, clang::Expr::Classification, bool, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>, bool)>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6cf15cb)
#17 0x0000000006aed375 AddMethodTemplateCandidateImmediately(clang::Sema&, clang::OverloadCandidateSet&, clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::CXXRecordDecl*, clang::TemplateArgumentListInfo*, clang::QualType, clang::Expr::Classification, llvm::ArrayRef<clang::Expr*>, bool, bool, clang::OverloadCandidateParamOrder) SemaOverload.cpp:0:0
#18 0x0000000006aefcf3 clang::OverloadCandidateSet::InjectNonDeducedTemplateCandidates(clang::Sema&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6aefcf3)
#19 0x0000000006aefda7 clang::OverloadCandidateSet::BestViableFunction(clang::Sema&, clang::SourceLocation, clang::OverloadCandidate*&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6aefda7)
#20 0x0000000006b0485c clang::Sema::BuildCallToObjectOfClassType(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6b0485c)
#21 0x0000000006788788 clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6788788)
#22 0x000000000678ca3c clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x678ca3c)
#23 0x0000000006d74f13 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCallExpr(clang::CallExpr*) SemaTemplateInstantiate.cpp:0:0
#24 0x0000000006d5a01a clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#25 0x0000000006d716bb clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExprs(clang::Expr* const*, unsigned int, bool, llvm::SmallVectorImpl<clang::Expr*>&, bool*) SemaTemplateInstantiate.cpp:0:0
#26 0x0000000006d74d8c clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformInitListExpr(clang::InitListExpr*) SemaTemplateInstantiate.cpp:0:0
#27 0x0000000006d5a647 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#28 0x0000000006d71fc5 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformInitializer(clang::Expr*, bool) SemaTemplateInstantiate.cpp:0:0
#29 0x0000000006d715e7 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExprs(clang::Expr* const*, unsigned int, bool, llvm::SmallVectorImpl<clang::Expr*>&, bool*) SemaTemplateInstantiate.cpp:0:0
#30 0x0000000006d758c6 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCXXUnresolvedConstructExpr(clang::CXXUnresolvedConstructExpr*) SemaTemplateInstantiate.cpp:0:0
#31 0x0000000006d59fde clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#32 0x0000000006d829d7 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformRequiresExpr(clang::RequiresExpr*) SemaTemplateInstantiate.cpp:0:0
#33 0x0000000006d5a3e6 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#34 0x0000000006d604b5 clang::Sema::SubstConstraintExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6d604b5)
#35 0x00000000065148a5 calculateConstraintSatisfaction(clang::Sema&, clang::Expr const*, clang::NamedDecl const*, clang::SourceLocation, clang::MultiLevelTemplateArgumentList const&, clang::ConstraintSatisfaction&) SemaConcept.cpp:0:0
#36 0x00000000065168fc CheckConstraintSatisfaction(clang::Sema&, clang::NamedDecl const*, llvm::ArrayRef<clang::AssociatedConstraint>, llvm::SmallVectorImpl<clang::Expr*>&, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&) SemaConcept.cpp:0:0
#37 0x0000000006519ba7 clang::Sema::CheckConstraintSatisfaction(clang::NamedDecl const*, llvm::ArrayRef<clang::AssociatedConstraint>, llvm::SmallVectorImpl<clang::Expr*>&, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&) (.constprop.0) SemaConcept.cpp:0:0
#38 0x000000000651ac7a clang::Sema::CheckFunctionTemplateConstraints(clang::SourceLocation, clang::FunctionDecl*, llvm::ArrayRef<clang::TemplateArgument>, clang::ConstraintSatisfaction&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x651ac7a)
#39 0x0000000006cf9f30 clang::Sema::FinishTemplateArgumentDeduction(clang::FunctionTemplateDecl*, llvm::SmallVectorImpl<clang::DeducedTemplateArgument>&, unsigned int, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, llvm::SmallVectorImpl<clang::Sema::OriginalCallArg> const*, bool, bool, llvm::function_ref<bool (bool)>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6cf9f30)
#40 0x0000000006cfaf97 void llvm::function_ref<void ()>::callback_fn<clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, bool, bool, clang::QualType, clang::Expr::Classification, bool, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>, bool)>)::'lambda1'()>(long) SemaTemplateDeduction.cpp:0:0
#41 0x00000000079d8e51 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x79d8e51)
#42 0x0000000006cf15cb clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, bool, bool, clang::QualType, clang::Expr::Classification, bool, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>, bool)>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6cf15cb)
#43 0x0000000006aef3dc AddTemplateOverloadCandidateImmediately(clang::Sema&, clang::OverloadCandidateSet&, clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, bool, bool, bool, clang::CallExpr::ADLCallKind, clang::OverloadCandidateParamOrder, bool) SemaOverload.cpp:0:0
#44 0x0000000006af096d clang::Sema::AddTemplateOverloadCandidate(clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::OverloadCandidateSet&, bool, bool, bool, clang::CallExpr::ADLCallKind, clang::OverloadCandidateParamOrder, bool) (.constprop.0) SemaOverload.cpp:0:0
#45 0x0000000006af2c41 IsUserDefinedConversion(clang::Sema&, clang::Expr*, clang::QualType, clang::UserDefinedConversionSequence&, clang::OverloadCandidateSet&, clang::Sema::AllowedExplicit, bool) (.constprop.0) SemaOverload.cpp:0:0
#46 0x0000000006af3035 TryUserDefinedConversion(clang::Sema&, clang::Expr*, clang::QualType, bool, clang::Sema::AllowedExplicit, bool, bool, bool, bool) (.constprop.0) SemaOverload.cpp:0:0
#47 0x0000000006af366c TryImplicitConversion(clang::Sema&, clang::Expr*, clang::QualType, bool, clang::Sema::AllowedExplicit, bool, bool, bool, bool) SemaOverload.cpp:0:0
#48 0x0000000006af3aba TryReferenceInit(clang::Sema&, clang::Expr*, clang::QualType, clang::SourceLocation, bool, bool) SemaOverload.cpp:0:0
#49 0x0000000006ae9cde TryCopyInitialization(clang::Sema&, clang::Expr*, clang::QualType, bool, bool, bool, bool) SemaOverload.cpp:0:0
#50 0x0000000006aeec54 clang::Sema::AddOverloadCandidate(clang::FunctionDecl*, clang::DeclAccessPair, llvm::ArrayRef<clang::Expr*>, clang::OverloadCandidateSet&, bool, bool, bool, bool, clang::CallExpr::ADLCallKind, llvm::MutableArrayRef<clang::ImplicitConversionSequence>, clang::OverloadCandidateParamOrder, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6aeec54)
#51 0x00000000068937bc ResolveConstructorOverload(clang::Sema&, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::OverloadCandidateSet&, clang::QualType, clang::DeclContextLookupResult, clang::OverloadCandidate*&, bool, bool, bool, bool, bool, bool) SemaInit.cpp:0:0
#52 0x00000000068a7776 TryConstructorInitialization(clang::Sema&, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::MutableArrayRef<clang::Expr*>, clang::QualType, clang::QualType, clang::InitializationSequence&, bool, bool) SemaInit.cpp:0:0
#53 0x00000000068b983e TryConstructorOrParenListInitialization(clang::Sema&, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::MutableArrayRef<clang::Expr*>, clang::QualType, clang::InitializationSequence&, bool) SemaInit.cpp:0:0
#54 0x00000000068aaa31 clang::InitializationSequence::InitializeFrom(clang::Sema&, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::MutableArrayRef<clang::Expr*>, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x68aaa31)
#55 0x00000000064400e4 TryStaticImplicitCast(clang::Sema&, clang::ActionResult<clang::Expr*, true>&, clang::QualType, clang::CheckedConversionKind, (anonymous namespace)::CastOperation::OpRangeType, unsigned int&, clang::CastKind&, bool) SemaCast.cpp:0:0
#56 0x0000000006446000 TryStaticCast(clang::Sema&, clang::ActionResult<clang::Expr*, true>&, clang::QualType, clang::CheckedConversionKind, (anonymous namespace)::CastOperation::OpRangeType, unsigned int&, clang::CastKind&, llvm::SmallVector<clang::CXXBaseSpecifier*, 4u>&, bool) SemaCast.cpp:0:0
#57 0x000000000644f24e (anonymous namespace)::CastOperation::CheckCXXCStyleCast(bool, bool) SemaCast.cpp:0:0
#58 0x000000000644f641 clang::Sema::BuildCXXFunctionalCastExpr(clang::TypeSourceInfo*, clang::QualType, clang::SourceLocation, clang::Expr*, clang::SourceLocation) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x644f641)
#59 0x00000000068163c0 clang::Sema::BuildCXXTypeConstructExpr(clang::TypeSourceInfo*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x68163c0)
#60 0x0000000006816a59 clang::Sema::ActOnCXXTypeConstructExpr(clang::OpaquePtr<clang::QualType>, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6816a59)
#61 0x0000000006296289 clang::Parser::ParseCXXTypeConstructExpression(clang::DeclSpec const&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6296289)
#62 0x0000000006286534 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6286534)
#63 0x0000000006286bb5 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6286bb5)
#64 0x00000000062870da clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62870da)
#65 0x00000000062872c9 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62872c9)
#66 0x000000000623e60d clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x623e60d)
#67 0x000000000625402c clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x625402c)
#68 0x00000000062011cf clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62011cf)
#69 0x0000000006201fbe clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6201fbe)
#70 0x000000000620aeef clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x620aeef)
#71 0x000000000620c728 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x620c728)
#72 0x00000000061fab1a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61fab1a)
#73 0x000000000459896d clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x459896d)
#74 0x00000000048a955a clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x48a955a)
#75 0x000000000482485b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x482485b)
#76 0x000000000499ab43 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x499ab43)
#77 0x0000000000da4575 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xda4575)
#78 0x0000000000d9c45d ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#79 0x000000000460a579 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#80 0x0000000003b98ce3 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3b98ce3)
#81 0x000000000460a799 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#82 0x00000000045cdaed clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45cdaed)
#83 0x00000000045ceaa1 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45ceaa1)
#84 0x00000000045dbfac clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45dbfac)
#85 0x0000000000da0d91 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xda0d91)
#86 0x0000000000c50f14 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc50f14)
#87 0x000070b3dbe29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#88 0x000070b3dbe29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#89 0x0000000000d9bef5 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xd9bef5)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (Eczbek)

<details>
https://godbolt.org/z/KPj3Wb3Kx

```cpp
template&lt;int...&gt;
struct seq {};

using arr = char[1];

struct foo {
	template&lt;int... i&gt;
	constexpr foo(seq&lt;i...&gt;) requires requires {
		arr { [](auto) requires(i, true) { return 0; }(i)... };
	} {}
};

constexpr auto bar = foo(seq&lt;0&gt;());

int main() {}
```

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 &lt;source&gt;
1.	&lt;source&gt;:13:35: current parser token ';'
 #<!-- -->0 0x0000000003c4aaf8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c4aaf8)
 #<!-- -->1 0x0000000003c484cc llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c484cc)
 #<!-- -->2 0x0000000003b988c8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x000070b3dbe42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x0000000007741dff clang::TemplateArgument::isPackExpansion() const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x7741dff)
 #<!-- -->5 0x0000000006d5ae43 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->6 0x0000000006d7234b clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformBinaryOperator(clang::BinaryOperator*) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->7 0x0000000006d59e18 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->8 0x0000000006d604b5 clang::Sema::SubstConstraintExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6d604b5)
 #<!-- -->9 0x00000000065148a5 calculateConstraintSatisfaction(clang::Sema&amp;, clang::Expr const*, clang::NamedDecl const*, clang::SourceLocation, clang::MultiLevelTemplateArgumentList const&amp;, clang::ConstraintSatisfaction&amp;) SemaConcept.cpp:0:0
#<!-- -->10 0x00000000065168fc CheckConstraintSatisfaction(clang::Sema&amp;, clang::NamedDecl const*, llvm::ArrayRef&lt;clang::AssociatedConstraint&gt;, llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceRange, clang::ConstraintSatisfaction&amp;) SemaConcept.cpp:0:0
#<!-- -->11 0x0000000006519ba7 clang::Sema::CheckConstraintSatisfaction(clang::NamedDecl const*, llvm::ArrayRef&lt;clang::AssociatedConstraint&gt;, llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceRange, clang::ConstraintSatisfaction&amp;) (.constprop.0) SemaConcept.cpp:0:0
#<!-- -->12 0x000000000651a75e clang::Sema::CheckFunctionTemplateConstraints(clang::SourceLocation, clang::FunctionDecl*, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::ConstraintSatisfaction&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x651a75e)
#<!-- -->13 0x0000000006cfa49e clang::Sema::FinishTemplateArgumentDeduction(clang::FunctionTemplateDecl*, llvm::SmallVectorImpl&lt;clang::DeducedTemplateArgument&gt;&amp;, unsigned int, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, llvm::SmallVectorImpl&lt;clang::Sema::OriginalCallArg&gt; const*, bool, bool, llvm::function_ref&lt;bool (bool)&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6cfa49e)
#<!-- -->14 0x0000000006cfaf97 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, bool, bool, bool, clang::QualType, clang::Expr::Classification, bool, llvm::function_ref&lt;bool (llvm::ArrayRef&lt;clang::QualType&gt;, bool)&gt;)::'lambda1'()&gt;(long) SemaTemplateDeduction.cpp:0:0
#<!-- -->15 0x00000000079d8e51 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x79d8e51)
#<!-- -->16 0x0000000006cf15cb clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, bool, bool, bool, clang::QualType, clang::Expr::Classification, bool, llvm::function_ref&lt;bool (llvm::ArrayRef&lt;clang::QualType&gt;, bool)&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6cf15cb)
#<!-- -->17 0x0000000006aed375 AddMethodTemplateCandidateImmediately(clang::Sema&amp;, clang::OverloadCandidateSet&amp;, clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::CXXRecordDecl*, clang::TemplateArgumentListInfo*, clang::QualType, clang::Expr::Classification, llvm::ArrayRef&lt;clang::Expr*&gt;, bool, bool, clang::OverloadCandidateParamOrder) SemaOverload.cpp:0:0
#<!-- -->18 0x0000000006aefcf3 clang::OverloadCandidateSet::InjectNonDeducedTemplateCandidates(clang::Sema&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6aefcf3)
#<!-- -->19 0x0000000006aefda7 clang::OverloadCandidateSet::BestViableFunction(clang::Sema&amp;, clang::SourceLocation, clang::OverloadCandidate*&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6aefda7)
#<!-- -->20 0x0000000006b0485c clang::Sema::BuildCallToObjectOfClassType(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6b0485c)
#<!-- -->21 0x0000000006788788 clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6788788)
#<!-- -->22 0x000000000678ca3c clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x678ca3c)
#<!-- -->23 0x0000000006d74f13 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCallExpr(clang::CallExpr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->24 0x0000000006d5a01a clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->25 0x0000000006d716bb clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExprs(clang::Expr* const*, unsigned int, bool, llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, bool*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->26 0x0000000006d74d8c clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformInitListExpr(clang::InitListExpr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->27 0x0000000006d5a647 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->28 0x0000000006d71fc5 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformInitializer(clang::Expr*, bool) SemaTemplateInstantiate.cpp:0:0
#<!-- -->29 0x0000000006d715e7 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExprs(clang::Expr* const*, unsigned int, bool, llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, bool*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->30 0x0000000006d758c6 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCXXUnresolvedConstructExpr(clang::CXXUnresolvedConstructExpr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->31 0x0000000006d59fde clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->32 0x0000000006d829d7 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformRequiresExpr(clang::RequiresExpr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->33 0x0000000006d5a3e6 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->34 0x0000000006d604b5 clang::Sema::SubstConstraintExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6d604b5)
#<!-- -->35 0x00000000065148a5 calculateConstraintSatisfaction(clang::Sema&amp;, clang::Expr const*, clang::NamedDecl const*, clang::SourceLocation, clang::MultiLevelTemplateArgumentList const&amp;, clang::ConstraintSatisfaction&amp;) SemaConcept.cpp:0:0
#<!-- -->36 0x00000000065168fc CheckConstraintSatisfaction(clang::Sema&amp;, clang::NamedDecl const*, llvm::ArrayRef&lt;clang::AssociatedConstraint&gt;, llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceRange, clang::ConstraintSatisfaction&amp;) SemaConcept.cpp:0:0
#<!-- -->37 0x0000000006519ba7 clang::Sema::CheckConstraintSatisfaction(clang::NamedDecl const*, llvm::ArrayRef&lt;clang::AssociatedConstraint&gt;, llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceRange, clang::ConstraintSatisfaction&amp;) (.constprop.0) SemaConcept.cpp:0:0
#<!-- -->38 0x000000000651ac7a clang::Sema::CheckFunctionTemplateConstraints(clang::SourceLocation, clang::FunctionDecl*, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::ConstraintSatisfaction&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x651ac7a)
#<!-- -->39 0x0000000006cf9f30 clang::Sema::FinishTemplateArgumentDeduction(clang::FunctionTemplateDecl*, llvm::SmallVectorImpl&lt;clang::DeducedTemplateArgument&gt;&amp;, unsigned int, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, llvm::SmallVectorImpl&lt;clang::Sema::OriginalCallArg&gt; const*, bool, bool, llvm::function_ref&lt;bool (bool)&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6cf9f30)
#<!-- -->40 0x0000000006cfaf97 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, bool, bool, bool, clang::QualType, clang::Expr::Classification, bool, llvm::function_ref&lt;bool (llvm::ArrayRef&lt;clang::QualType&gt;, bool)&gt;)::'lambda1'()&gt;(long) SemaTemplateDeduction.cpp:0:0
#<!-- -->41 0x00000000079d8e51 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x79d8e51)
#<!-- -->42 0x0000000006cf15cb clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, bool, bool, bool, clang::QualType, clang::Expr::Classification, bool, llvm::function_ref&lt;bool (llvm::ArrayRef&lt;clang::QualType&gt;, bool)&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6cf15cb)
#<!-- -->43 0x0000000006aef3dc AddTemplateOverloadCandidateImmediately(clang::Sema&amp;, clang::OverloadCandidateSet&amp;, clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, bool, bool, bool, clang::CallExpr::ADLCallKind, clang::OverloadCandidateParamOrder, bool) SemaOverload.cpp:0:0
#<!-- -->44 0x0000000006af096d clang::Sema::AddTemplateOverloadCandidate(clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::OverloadCandidateSet&amp;, bool, bool, bool, clang::CallExpr::ADLCallKind, clang::OverloadCandidateParamOrder, bool) (.constprop.0) SemaOverload.cpp:0:0
#<!-- -->45 0x0000000006af2c41 IsUserDefinedConversion(clang::Sema&amp;, clang::Expr*, clang::QualType, clang::UserDefinedConversionSequence&amp;, clang::OverloadCandidateSet&amp;, clang::Sema::AllowedExplicit, bool) (.constprop.0) SemaOverload.cpp:0:0
#<!-- -->46 0x0000000006af3035 TryUserDefinedConversion(clang::Sema&amp;, clang::Expr*, clang::QualType, bool, clang::Sema::AllowedExplicit, bool, bool, bool, bool) (.constprop.0) SemaOverload.cpp:0:0
#<!-- -->47 0x0000000006af366c TryImplicitConversion(clang::Sema&amp;, clang::Expr*, clang::QualType, bool, clang::Sema::AllowedExplicit, bool, bool, bool, bool) SemaOverload.cpp:0:0
#<!-- -->48 0x0000000006af3aba TryReferenceInit(clang::Sema&amp;, clang::Expr*, clang::QualType, clang::SourceLocation, bool, bool) SemaOverload.cpp:0:0
#<!-- -->49 0x0000000006ae9cde TryCopyInitialization(clang::Sema&amp;, clang::Expr*, clang::QualType, bool, bool, bool, bool) SemaOverload.cpp:0:0
#<!-- -->50 0x0000000006aeec54 clang::Sema::AddOverloadCandidate(clang::FunctionDecl*, clang::DeclAccessPair, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::OverloadCandidateSet&amp;, bool, bool, bool, bool, clang::CallExpr::ADLCallKind, llvm::MutableArrayRef&lt;clang::ImplicitConversionSequence&gt;, clang::OverloadCandidateParamOrder, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6aeec54)
#<!-- -->51 0x00000000068937bc ResolveConstructorOverload(clang::Sema&amp;, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::OverloadCandidateSet&amp;, clang::QualType, clang::DeclContextLookupResult, clang::OverloadCandidate*&amp;, bool, bool, bool, bool, bool, bool) SemaInit.cpp:0:0
#<!-- -->52 0x00000000068a7776 TryConstructorInitialization(clang::Sema&amp;, clang::InitializedEntity const&amp;, clang::InitializationKind const&amp;, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::QualType, clang::QualType, clang::InitializationSequence&amp;, bool, bool) SemaInit.cpp:0:0
#<!-- -->53 0x00000000068b983e TryConstructorOrParenListInitialization(clang::Sema&amp;, clang::InitializedEntity const&amp;, clang::InitializationKind const&amp;, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::QualType, clang::InitializationSequence&amp;, bool) SemaInit.cpp:0:0
#<!-- -->54 0x00000000068aaa31 clang::InitializationSequence::InitializeFrom(clang::Sema&amp;, clang::InitializedEntity const&amp;, clang::InitializationKind const&amp;, llvm::MutableArrayRef&lt;clang::Expr*&gt;, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x68aaa31)
#<!-- -->55 0x00000000064400e4 TryStaticImplicitCast(clang::Sema&amp;, clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, clang::QualType, clang::CheckedConversionKind, (anonymous namespace)::CastOperation::OpRangeType, unsigned int&amp;, clang::CastKind&amp;, bool) SemaCast.cpp:0:0
#<!-- -->56 0x0000000006446000 TryStaticCast(clang::Sema&amp;, clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, clang::QualType, clang::CheckedConversionKind, (anonymous namespace)::CastOperation::OpRangeType, unsigned int&amp;, clang::CastKind&amp;, llvm::SmallVector&lt;clang::CXXBaseSpecifier*, 4u&gt;&amp;, bool) SemaCast.cpp:0:0
#<!-- -->57 0x000000000644f24e (anonymous namespace)::CastOperation::CheckCXXCStyleCast(bool, bool) SemaCast.cpp:0:0
#<!-- -->58 0x000000000644f641 clang::Sema::BuildCXXFunctionalCastExpr(clang::TypeSourceInfo*, clang::QualType, clang::SourceLocation, clang::Expr*, clang::SourceLocation) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x644f641)
#<!-- -->59 0x00000000068163c0 clang::Sema::BuildCXXTypeConstructExpr(clang::TypeSourceInfo*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x68163c0)
#<!-- -->60 0x0000000006816a59 clang::Sema::ActOnCXXTypeConstructExpr(clang::OpaquePtr&lt;clang::QualType&gt;, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6816a59)
#<!-- -->61 0x0000000006296289 clang::Parser::ParseCXXTypeConstructExpression(clang::DeclSpec const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6296289)
#<!-- -->62 0x0000000006286534 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&amp;, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6286534)
#<!-- -->63 0x0000000006286bb5 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&amp;, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6286bb5)
#<!-- -->64 0x00000000062870da clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62870da)
#<!-- -->65 0x00000000062872c9 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62872c9)
#<!-- -->66 0x000000000623e60d clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x623e60d)
#<!-- -->67 0x000000000625402c clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x625402c)
#<!-- -->68 0x00000000062011cf clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62011cf)
#<!-- -->69 0x0000000006201fbe clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6201fbe)
#<!-- -->70 0x000000000620aeef clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x620aeef)
#<!-- -->71 0x000000000620c728 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x620c728)
#<!-- -->72 0x00000000061fab1a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61fab1a)
#<!-- -->73 0x000000000459896d clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x459896d)
#<!-- -->74 0x00000000048a955a clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x48a955a)
#<!-- -->75 0x000000000482485b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x482485b)
#<!-- -->76 0x000000000499ab43 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x499ab43)
#<!-- -->77 0x0000000000da4575 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xda4575)
#<!-- -->78 0x0000000000d9c45d ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->79 0x000000000460a579 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->80 0x0000000003b98ce3 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3b98ce3)
#<!-- -->81 0x000000000460a799 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->82 0x00000000045cdaed clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45cdaed)
#<!-- -->83 0x00000000045ceaa1 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45ceaa1)
#<!-- -->84 0x00000000045dbfac clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45dbfac)
#<!-- -->85 0x0000000000da0d91 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xda0d91)
#<!-- -->86 0x0000000000c50f14 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc50f14)
#<!-- -->87 0x000070b3dbe29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->88 0x000070b3dbe29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->89 0x0000000000d9bef5 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xd9bef5)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```

</details>


---

### Comment 2 - shafik

BTW, assertion build are in general more useful. They usually crash earlier and are more indicative of the underlying issue.

This is a clang trunk regression: https://godbolt.org/z/Gaf1nrd6W

Crash: https://godbolt.org/z/W8hE1a9a1

Assertion:

```console
clang++: /root/llvm-project/llvm/tools/clang/lib/Sema/SemaTemplateInstantiate.cpp:2228: 
clang::ExprResult {anonymous}::TemplateInstantiator::TransformTemplateParmRefExpr(clang::DeclRefExpr*, clang::NonTypeTemplateParmDecl*):
Assertion `Arg.getKind() == TemplateArgument::Pack && "Missing argument pack"' failed.
```

Backtrace:

```console
#0 0x0000000003f8a9a8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f8a9a8)
 #1 0x0000000003f87dd4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f87dd4)
 #2 0x0000000003ecc6f8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007236f5c42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007236f5c969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007236f5c42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007236f5c287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007236f5c2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007236f5c39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x000000000719a1a9 (anonymous namespace)::TemplateInstantiator::TransformDeclRefExpr(clang::DeclRefExpr*) SemaTemplateInstantiate.cpp:0:0
#10 0x0000000007165eb9 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#11 0x000000000718f2f8 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformBinaryOperator(clang::BinaryOperator*) SemaTemplateInstantiate.cpp:0:0
#12 0x0000000007165b7a clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#13 0x000000000716f5d2 clang::Sema::SubstConstraintExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x716f5d2)
#14 0x0000000006936d4e calculateConstraintSatisfaction(clang::Sema&, clang::Expr const*, clang::NamedDecl const*, clang::SourceLocation, clang::MultiLevelTemplateArgumentList const&, clang::ConstraintSatisfaction&) SemaConcept.cpp:0:0
#15 0x0000000006938d49 CheckConstraintSatisfaction(clang::Sema&, clang::NamedDecl const*, llvm::ArrayRef<clang::AssociatedConstraint>, llvm::SmallVectorImpl<clang::Expr*>&, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&) SemaConcept.cpp:0:0
#16 0x0000000006939375 clang::Sema::CheckConstraintSatisfaction(clang::NamedDecl const*, llvm::ArrayRef<clang::AssociatedConstraint>, llvm::SmallVectorImpl<clang::Expr*>&, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6939375)
#17 0x000000000693ad5b clang::Sema::CheckFunctionTemplateConstraints(clang::SourceLocation, clang::FunctionDecl*, llvm::ArrayRef<clang::TemplateArgument>, clang::ConstraintSatisfaction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x693ad5b)
#18 0x00000000070fd273 clang::Sema::FinishTemplateArgumentDeduction(clang::FunctionTemplateDecl*, llvm::SmallVectorImpl<clang::DeducedTemplateArgument>&, unsigned int, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, llvm::SmallVectorImpl<clang::Sema::OriginalCallArg> const*, bool, bool, llvm::function_ref<bool (bool)>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x70fd273)
#19 0x00000000070fd947 void llvm::function_ref<void ()>::callback_fn<clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, bool, bool, clang::QualType, clang::Expr::Classification, bool, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>, bool)>)::'lambda1'()>(long) SemaTemplateDeduction.cpp:0:0
#20 0x0000000007ec6ac1 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7ec6ac1)
#21 0x00000000070d5d25 clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, bool, bool, clang::QualType, clang::Expr::Classification, bool, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>, bool)>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x70d5d25)
#22 0x0000000006f03f75 AddMethodTemplateCandidateImmediately(clang::Sema&, clang::OverloadCandidateSet&, clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::CXXRecordDecl*, clang::TemplateArgumentListInfo*, clang::QualType, clang::Expr::Classification, llvm::ArrayRef<clang::Expr*>, bool, bool, clang::OverloadCandidateParamOrder) SemaOverload.cpp:0:0
#23 0x0000000006f06a73 clang::OverloadCandidateSet::InjectNonDeducedTemplateCandidates(clang::Sema&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f06a73)
#24 0x0000000006f06ba7 clang::OverloadCandidateSet::BestViableFunction(clang::Sema&, clang::SourceLocation, clang::OverloadCandidate*&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f06ba7)
#25 0x0000000006f251bf clang::Sema::BuildCallToObjectOfClassType(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f251bf)
#26 0x0000000006b9ba75 clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b9ba75)
#27 0x0000000006b9c6de clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b9c6de)
#28 0x00000000071926c0 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCallExpr(clang::CallExpr*) SemaTemplateInstantiate.cpp:0:0
#29 0x0000000007165e02 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#30 0x000000000718e714 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExprs(clang::Expr* const*, unsigned int, bool, llvm::SmallVectorImpl<clang::Expr*>&, bool*) SemaTemplateInstantiate.cpp:0:0
#31 0x00000000071924ec clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformInitListExpr(clang::InitListExpr*) SemaTemplateInstantiate.cpp:0:0
#32 0x0000000007165f72 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#33 0x000000000718ed15 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformInitializer(clang::Expr*, bool) (.part.0) SemaTemplateInstantiate.cpp:0:0
#34 0x000000000718e5a0 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExprs(clang::Expr* const*, unsigned int, bool, llvm::SmallVectorImpl<clang::Expr*>&, bool*) SemaTemplateInstantiate.cpp:0:0
#35 0x0000000007193252 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCXXUnresolvedConstructExpr(clang::CXXUnresolvedConstructExpr*) SemaTemplateInstantiate.cpp:0:0
#36 0x0000000007165db8 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#37 0x000000000718b13b clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformRequiresExpr(clang::RequiresExpr*) SemaTemplateInstantiate.cpp:0:0
#38 0x000000000718d616 (anonymous namespace)::TemplateInstantiator::TransformRequiresExpr(clang::RequiresExpr*) SemaTemplateInstantiate.cpp:0:0
#39 0x000000000716616e clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#40 0x000000000716f5d2 clang::Sema::SubstConstraintExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x716f5d2)
#41 0x0000000006936d4e calculateConstraintSatisfaction(clang::Sema&, clang::Expr const*, clang::NamedDecl const*, clang::SourceLocation, clang::MultiLevelTemplateArgumentList const&, clang::ConstraintSatisfaction&) SemaConcept.cpp:0:0
#42 0x0000000006938d49 CheckConstraintSatisfaction(clang::Sema&, clang::NamedDecl const*, llvm::ArrayRef<clang::AssociatedConstraint>, llvm::SmallVectorImpl<clang::Expr*>&, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&) SemaConcept.cpp:0:0
#43 0x0000000006939375 clang::Sema::CheckConstraintSatisfaction(clang::NamedDecl const*, llvm::ArrayRef<clang::AssociatedConstraint>, llvm::SmallVectorImpl<clang::Expr*>&, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6939375)
#44 0x000000000693b259 clang::Sema::CheckFunctionTemplateConstraints(clang::SourceLocation, clang::FunctionDecl*, llvm::ArrayRef<clang::TemplateArgument>, clang::ConstraintSatisfaction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x693b259)
#45 0x00000000070fc448 clang::Sema::FinishTemplateArgumentDeduction(clang::FunctionTemplateDecl*, llvm::SmallVectorImpl<clang::DeducedTemplateArgument>&, unsigned int, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, llvm::SmallVectorImpl<clang::Sema::OriginalCallArg> const*, bool, bool, llvm::function_ref<bool (bool)>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x70fc448)
#46 0x00000000070fd947 void llvm::function_ref<void ()>::callback_fn<clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, bool, bool, clang::QualType, clang::Expr::Classification, bool, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>, bool)>)::'lambda1'()>(long) SemaTemplateDeduction.cpp:0:0
#47 0x0000000007ec6ac1 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7ec6ac1)
#48 0x00000000070d5d25 clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, bool, bool, clang::QualType, clang::Expr::Classification, bool, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>, bool)>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x70d5d25)
#49 0x0000000006f0602b AddTemplateOverloadCandidateImmediately(clang::Sema&, clang::OverloadCandidateSet&, clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, bool, bool, bool, clang::CallExpr::ADLCallKind, clang::OverloadCandidateParamOrder, bool) SemaOverload.cpp:0:0
#50 0x0000000006f08158 clang::Sema::AddTemplateOverloadCandidate(clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::OverloadCandidateSet&, bool, bool, bool, clang::CallExpr::ADLCallKind, clang::OverloadCandidateParamOrder, bool) (.constprop.0) SemaOverload.cpp:0:0
#51 0x0000000006f0b232 IsUserDefinedConversion(clang::Sema&, clang::Expr*, clang::QualType, clang::UserDefinedConversionSequence&, clang::OverloadCandidateSet&, clang::Sema::AllowedExplicit, bool) SemaOverload.cpp:0:0
#52 0x0000000006f0ba54 TryUserDefinedConversion(clang::Sema&, clang::Expr*, clang::QualType, bool, clang::Sema::AllowedExplicit, bool, bool, bool, bool) (.constprop.0) SemaOverload.cpp:0:0
#53 0x0000000006effe44 TryImplicitConversion(clang::Sema&, clang::Expr*, clang::QualType, bool, clang::Sema::AllowedExplicit, bool, bool, bool, bool) SemaOverload.cpp:0:0
#54 0x0000000006f07a5f TryReferenceInit(clang::Sema&, clang::Expr*, clang::QualType, clang::SourceLocation, bool, bool) SemaOverload.cpp:0:0
#55 0x0000000006f000e7 TryCopyInitialization(clang::Sema&, clang::Expr*, clang::QualType, bool, bool, bool, bool) SemaOverload.cpp:0:0
#56 0x0000000006f0569a clang::Sema::AddOverloadCandidate(clang::FunctionDecl*, clang::DeclAccessPair, llvm::ArrayRef<clang::Expr*>, clang::OverloadCandidateSet&, bool, bool, bool, bool, clang::CallExpr::ADLCallKind, llvm::MutableArrayRef<clang::ImplicitConversionSequence>, clang::OverloadCandidateParamOrder, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f0569a)
#57 0x0000000006cb941e ResolveConstructorOverload(clang::Sema&, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::OverloadCandidateSet&, clang::QualType, clang::DeclContextLookupResult, clang::OverloadCandidate*&, bool, bool, bool, bool, bool, bool) SemaInit.cpp:0:0
#58 0x0000000006cc7717 TryConstructorInitialization(clang::Sema&, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::MutableArrayRef<clang::Expr*>, clang::QualType, clang::QualType, clang::InitializationSequence&, bool, bool) SemaInit.cpp:0:0
#59 0x0000000006cd5d5e TryConstructorOrParenListInitialization(clang::Sema&, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::MutableArrayRef<clang::Expr*>, clang::QualType, clang::InitializationSequence&, bool) SemaInit.cpp:0:0
#60 0x0000000006ccb441 clang::InitializationSequence::InitializeFrom(clang::Sema&, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::MutableArrayRef<clang::Expr*>, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ccb441)
#61 0x000000000685ee89 TryStaticImplicitCast(clang::Sema&, clang::ActionResult<clang::Expr*, true>&, clang::QualType, clang::CheckedConversionKind, (anonymous namespace)::CastOperation::OpRangeType, unsigned int&, clang::CastKind&, bool) SemaCast.cpp:0:0
#62 0x0000000006864b5a TryStaticCast(clang::Sema&, clang::ActionResult<clang::Expr*, true>&, clang::QualType, clang::CheckedConversionKind, (anonymous namespace)::CastOperation::OpRangeType, unsigned int&, clang::CastKind&, llvm::SmallVector<clang::CXXBaseSpecifier*, 4u>&, bool) SemaCast.cpp:0:0
#63 0x000000000686be1b (anonymous namespace)::CastOperation::CheckCXXCStyleCast(bool, bool) SemaCast.cpp:0:0
#64 0x000000000686c65e clang::Sema::BuildCXXFunctionalCastExpr(clang::TypeSourceInfo*, clang::QualType, clang::SourceLocation, clang::Expr*, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x686c65e)
#65 0x0000000006c40765 clang::Sema::BuildCXXTypeConstructExpr(clang::TypeSourceInfo*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c40765)
#66 0x0000000006c41059 clang::Sema::ActOnCXXTypeConstructExpr(clang::OpaquePtr<clang::QualType>, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c41059)
#67 0x00000000066d2f25 clang::Parser::ParseCXXTypeConstructExpression(clang::DeclSpec const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66d2f25)
#68 0x00000000066b7540 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66b7540)
#69 0x00000000066b615e clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66b615e)
#70 0x00000000066b8087 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66b8087)
#71 0x00000000066b8119 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66b8119)
#72 0x0000000006670618 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6670618)
#73 0x000000000667ef59 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x667ef59)
#74 0x000000000663c7de clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x663c7de)
#75 0x000000000663cf89 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x663cf89)
#76 0x0000000006644c1a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6644c1a)
#77 0x0000000006645bcd clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6645bcd)
#78 0x000000000663795a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x663795a)
#79 0x000000000493fba8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x493fba8)
#80 0x0000000004c38ad5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c38ad5)
#81 0x0000000004bb5dae clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bb5dae)
#82 0x0000000004d2a0a1 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d2a0a1)
#83 0x0000000000d91b8f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd91b8f)
#84 0x0000000000d8876a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#85 0x00000000049a6759 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#86 0x0000000003eccb94 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3eccb94)
#87 0x00000000049a6d6f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#88 0x0000000004968d3d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4968d3d)
#89 0x0000000004969dce clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4969dce)
#90 0x0000000004972965 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4972965)
#91 0x0000000000d8e03f clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd8e03f)
#92 0x0000000000c43564 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc43564)
#93 0x00007236f5c29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#94 0x00007236f5c29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#95 0x0000000000d88215 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd88215)
```

---

### Comment 3 - shafik

@zyn0217 git bisect points to e04e140adb600add79b414f137a13af9d89c8c0d

---

