# ICE in `TemplateInstantiator::TransformTemplateTypeParmType`

**Author:** chfast
**URL:** https://github.com/llvm/llvm-project/issues/63823
**Status:** Closed
**Labels:** clang:frontend, crash-on-invalid, confirmed
**Closed Date:** 2024-12-11T18:50:01Z

## Body

This is one of the reduced version of fixed https://github.com/llvm/llvm-project/issues/62615.

This code still crashes the compiler with

Assertion:

```console
clang/lib/Sema/SemaTemplateInstantiate.cpp:2224:
clang::QualType {anonymous}::TemplateInstantiator::TransformTemplateTypeParmType(clang::TypeLocBuilder&, clang::TemplateTypeParmTypeLoc, bool):
Assertion `Arg.getKind() == TemplateArgument::Type && "Template argument kind mismatch"' failed.
```

Backtrace:

```console
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 <source>
1.	<source>:66:30: current parser token ')'
2.	<source>:62:16: instantiating variable definition 'tuple_size_v<__tuple<(lambda at <source>:46:16)>>'
3.	<source>:24:8: instantiating class definition '__mexpand<__tuple<(lambda at <source>:46:16)>>'
  #0 0x000055a2fc1e86cf llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3c706cf)
  #1 0x000055a2fc1e643c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3c6e43c)
  #2 0x000055a2fc130478 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
  #3 0x00007fb8eb8d1420 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x14420)
  #4 0x00007fb8eb39e00b raise (/lib/x86_64-linux-gnu/libc.so.6+0x4300b)
  #5 0x00007fb8eb37d859 abort (/lib/x86_64-linux-gnu/libc.so.6+0x22859)
  #6 0x00007fb8eb37d729 (/lib/x86_64-linux-gnu/libc.so.6+0x22729)
  #7 0x00007fb8eb38efd6 (/lib/x86_64-linux-gnu/libc.so.6+0x33fd6)
  #8 0x000055a2ff4d84e7 (anonymous namespace)::TemplateInstantiator::TransformTemplateTypeParmType(clang::TypeLocBuilder&, clang::TemplateTypeParmTypeLoc, bool) SemaTemplateInstantiate.cpp:0:0
  #9 0x000055a2ff4fa44f clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
 #10 0x000055a2ff50029f clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeSourceInfo*) SemaTemplateInstantiate.cpp:0:0
 #11 0x000055a2ff5003d9 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::QualType) SemaTemplateInstantiate.cpp:0:0
 #12 0x000055a2ff4fa2de clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
 #13 0x000055a2ff517bca clang::Sema::SubstFunctionDeclType(clang::TypeSourceInfo*, clang::MultiLevelTemplateArgumentList const&, clang::SourceLocation, clang::DeclarationName, clang::CXXRecordDecl*, clang::Qualifiers, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f9fbca)
 #14 0x000055a2ff531ed9 clang::TemplateDeclInstantiator::SubstFunctionType(clang::FunctionDecl*, llvm::SmallVectorImpl<clang::ParmVarDecl*>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6fb9ed9)
 #15 0x000055a2ff5814da clang::TemplateDeclInstantiator::VisitCXXMethodDecl(clang::CXXMethodDecl*, clang::TemplateParameterList*, std::optional<clang::ASTTemplateArgumentListInfo const*>, clang::TemplateDeclInstantiator::RewriteKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x70094da)
 #16 0x000055a2ff583218 void llvm::function_ref<void ()>::callback_fn<clang::Sema::SubstDecl(clang::Decl*, clang::DeclContext*, clang::MultiLevelTemplateArgumentList const&)::'lambda'()>(long) SemaTemplateInstantiateDecl.cpp:0:0
 #17 0x000055a2feaf83a5 clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65803a5)
 #18 0x000055a2ff531d51 clang::Sema::SubstDecl(clang::Decl*, clang::DeclContext*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6fb9d51)
 #19 0x000055a2ff4ab04d clang::Sema::FinishTemplateArgumentDeduction(clang::FunctionTemplateDecl*, llvm::SmallVectorImpl<clang::DeducedTemplateArgument>&, unsigned int, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, llvm::SmallVectorImpl<clang::Sema::OriginalCallArg> const*, bool, llvm::function_ref<bool ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f3304d)
 #20 0x000055a2ff4ad01e void llvm::function_ref<void ()>::callback_fn<clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, bool, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>)>)::'lambda1'()>(long) SemaTemplateDeduction.cpp:0:0
 #21 0x000055a2feaf83a5 clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65803a5)
 #22 0x000055a2ff4b7279 clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, bool, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>)>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f3f279)
 #23 0x000055a2ff2bde31 clang::Sema::AddMethodTemplateCandidate(clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::CXXRecordDecl*, clang::TemplateArgumentListInfo*, clang::QualType, clang::Expr::Classification, llvm::ArrayRef<clang::Expr*>, clang::OverloadCandidateSet&, bool, bool, clang::OverloadCandidateParamOrder) (.constprop.0) SemaOverload.cpp:0:0
 #24 0x000055a2ff2be287 clang::Sema::AddMethodCandidate(clang::DeclAccessPair, clang::QualType, clang::Expr::Classification, llvm::ArrayRef<clang::Expr*>, clang::OverloadCandidateSet&, bool, clang::OverloadCandidateParamOrder) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d46287)
 #25 0x000055a2ff2d9e63 clang::Sema::BuildCallToObjectOfClassType(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d61e63)
 #26 0x000055a2fef5f5a5 clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69e75a5)
 #27 0x000055a2fef643bb clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69ec3bb)
 #28 0x000055a2ff4f4850 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCallExpr(clang::CallExpr*) SemaTemplateInstantiate.cpp:0:0
 #29 0x000055a2ff4ea67a clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
 #30 0x000055a2ff4f8027 (anonymous namespace)::TemplateInstantiator::TransformExprRequirement(clang::concepts::ExprRequirement*) SemaTemplateInstantiate.cpp:0:0
 #31 0x000055a2ff516139 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformRequiresExpr(clang::RequiresExpr*) SemaTemplateInstantiate.cpp:0:0
 #32 0x000055a2ff51704e (anonymous namespace)::TemplateInstantiator::TransformRequiresExpr(clang::RequiresExpr*) SemaTemplateInstantiate.cpp:0:0
 #33 0x000055a2ff4ea1d3 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
 #34 0x000055a2ff527aec clang::Sema::SubstConstraintExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6fafaec)
 #35 0x000055a2fec517df calculateConstraintSatisfaction(clang::Sema&, clang::NamedDecl const*, clang::SourceLocation, clang::MultiLevelTemplateArgumentList const&, clang::Expr const*, clang::ConstraintSatisfaction&)::'lambda'(clang::Expr const*)::operator()(clang::Expr const*) const SemaConcept.cpp:0:0
 #36 0x000055a2fec52ebb clang::ActionResult<clang::Expr*, true> calculateConstraintSatisfaction<calculateConstraintSatisfaction(clang::Sema&, clang::NamedDecl const*, clang::SourceLocation, clang::MultiLevelTemplateArgumentList const&, clang::Expr const*, clang::ConstraintSatisfaction&)::'lambda'(clang::Expr const*)>(clang::Sema&, clang::Expr const*, clang::ConstraintSatisfaction&, calculateConstraintSatisfaction(clang::Sema&, clang::NamedDecl const*, clang::SourceLocation, clang::MultiLevelTemplateArgumentList const&, clang::Expr const*, clang::ConstraintSatisfaction&)::'lambda'(clang::Expr const*)&&) SemaConcept.cpp:0:0
 #37 0x000055a2fec536f2 CheckConstraintSatisfaction(clang::Sema&, clang::NamedDecl const*, llvm::ArrayRef<clang::Expr const*>, llvm::SmallVectorImpl<clang::Expr*>&, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&) SemaConcept.cpp:0:0
 #38 0x000055a2fec53d34 clang::Sema::CheckConstraintSatisfaction(clang::NamedDecl const*, llvm::ArrayRef<clang::Expr const*>, llvm::SmallVectorImpl<clang::Expr*>&, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66dbd34)
 #39 0x000055a2ff393d54 clang::Sema::CheckConceptTemplateId(clang::CXXScopeSpec const&, clang::SourceLocation, clang::DeclarationNameInfo const&, clang::NamedDecl*, clang::ConceptDecl*, clang::TemplateArgumentListInfo const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e1bd54)
 #40 0x000055a2ff50655c clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformConceptSpecializationExpr(clang::ConceptSpecializationExpr*) SemaTemplateInstantiate.cpp:0:0
 #41 0x000055a2ff4ea5b5 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
 #42 0x000055a2ff527aec clang::Sema::SubstConstraintExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6fafaec)
 #43 0x000055a2fec517df calculateConstraintSatisfaction(clang::Sema&, clang::NamedDecl const*, clang::SourceLocation, clang::MultiLevelTemplateArgumentList const&, clang::Expr const*, clang::ConstraintSatisfaction&)::'lambda'(clang::Expr const*)::operator()(clang::Expr const*) const SemaConcept.cpp:0:0
 #44 0x000055a2fec52ebb clang::ActionResult<clang::Expr*, true> calculateConstraintSatisfaction<calculateConstraintSatisfaction(clang::Sema&, clang::NamedDecl const*, clang::SourceLocation, clang::MultiLevelTemplateArgumentList const&, clang::Expr const*, clang::ConstraintSatisfaction&)::'lambda'(clang::Expr const*)>(clang::Sema&, clang::Expr const*, clang::ConstraintSatisfaction&, calculateConstraintSatisfaction(clang::Sema&, clang::NamedDecl const*, clang::SourceLocation, clang::MultiLevelTemplateArgumentList const&, clang::Expr const*, clang::ConstraintSatisfaction&)::'lambda'(clang::Expr const*)&&) SemaConcept.cpp:0:0
 #45 0x000055a2fec536f2 CheckConstraintSatisfaction(clang::Sema&, clang::NamedDecl const*, llvm::ArrayRef<clang::Expr const*>, llvm::SmallVectorImpl<clang::Expr*>&, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&) SemaConcept.cpp:0:0
 #46 0x000055a2fec53d34 clang::Sema::CheckConstraintSatisfaction(clang::NamedDecl const*, llvm::ArrayRef<clang::Expr const*>, llvm::SmallVectorImpl<clang::Expr*>&, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66dbd34)
 #47 0x000055a2fec54cf4 clang::Sema::CheckInstantiatedFunctionTemplateConstraints(clang::SourceLocation, clang::FunctionDecl*, llvm::ArrayRef<clang::TemplateArgument>, clang::ConstraintSatisfaction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66dccf4)
 #48 0x000055a2ff4ac197 clang::Sema::FinishTemplateArgumentDeduction(clang::FunctionTemplateDecl*, llvm::SmallVectorImpl<clang::DeducedTemplateArgument>&, unsigned int, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, llvm::SmallVectorImpl<clang::Sema::OriginalCallArg> const*, bool, llvm::function_ref<bool ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f34197)
 #49 0x000055a2ff4ad01e void llvm::function_ref<void ()>::callback_fn<clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, bool, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>)>)::'lambda1'()>(long) SemaTemplateDeduction.cpp:0:0
 #50 0x000055a2feaf83a5 clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65803a5)
 #51 0x000055a2ff4b7279 clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, bool, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>)>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f3f279)
 #52 0x000055a2ff2bde31 clang::Sema::AddMethodTemplateCandidate(clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::CXXRecordDecl*, clang::TemplateArgumentListInfo*, clang::QualType, clang::Expr::Classification, llvm::ArrayRef<clang::Expr*>, clang::OverloadCandidateSet&, bool, bool, clang::OverloadCandidateParamOrder) (.constprop.0) SemaOverload.cpp:0:0
 #53 0x000055a2ff2be287 clang::Sema::AddMethodCandidate(clang::DeclAccessPair, clang::QualType, clang::Expr::Classification, llvm::ArrayRef<clang::Expr*>, clang::OverloadCandidateSet&, bool, clang::OverloadCandidateParamOrder) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d46287)
 #54 0x000055a2ff2d9e63 clang::Sema::BuildCallToObjectOfClassType(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d61e63)
 #55 0x000055a2fef5f5a5 clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69e75a5)
 #56 0x000055a2fef643bb clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69ec3bb)
 #57 0x000055a2ff4f4850 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCallExpr(clang::CallExpr*) SemaTemplateInstantiate.cpp:0:0
 #58 0x000055a2ff4ea67a clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
 #59 0x000055a2ff4fad91 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
 #60 0x000055a2ff50029f clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeSourceInfo*) SemaTemplateInstantiate.cpp:0:0
 #61 0x000055a2ff5003d9 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::QualType) SemaTemplateInstantiate.cpp:0:0
 #62 0x000055a2ff501f23 clang::Sema::SubstType(clang::QualType, clang::MultiLevelTemplateArgumentList const&, clang::SourceLocation, clang::DeclarationName) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f89f23)
 #63 0x000055a2ff395802 clang::Sema::CheckTemplateIdType(clang::TemplateName, clang::SourceLocation, clang::TemplateArgumentListInfo&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e1d802)
 #64 0x000055a2ff507af5 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformTemplateSpecializationType(clang::TypeLocBuilder&, clang::TemplateSpecializationTypeLoc, clang::TemplateName) SemaTemplateInstantiate.cpp:0:0
 #65 0x000055a2ff4fb2c8 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
 #66 0x000055a2ff50ffb0 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformElaboratedType(clang::TypeLocBuilder&, clang::ElaboratedTypeLoc) SemaTemplateInstantiate.cpp:0:0
 #67 0x000055a2ff4faccd clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
 #68 0x000055a2ff50029f clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeSourceInfo*) SemaTemplateInstantiate.cpp:0:0
 #69 0x000055a2ff508b0e clang::Sema::SubstType(clang::TypeSourceInfo*, clang::MultiLevelTemplateArgumentList const&, clang::SourceLocation, clang::DeclarationName, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f90b0e)
 #70 0x000055a2ff580a35 clang::TemplateDeclInstantiator::InstantiateTypedefNameDecl(clang::TypedefNameDecl*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7008a35)
 #71 0x000055a2ff583583 clang::TemplateDeclInstantiator::VisitTypeAliasTemplateDecl(clang::TypeAliasTemplateDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x700b583)
 #72 0x000055a2ff509d23 clang::Sema::InstantiateClass(clang::SourceLocation, clang::CXXRecordDecl*, clang::CXXRecordDecl*, clang::MultiLevelTemplateArgumentList const&, clang::TemplateSpecializationKind, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f91d23)
 #73 0x000055a2ff527d93 clang::Sema::InstantiateClassTemplateSpecialization(clang::SourceLocation, clang::ClassTemplateSpecializationDecl*, clang::TemplateSpecializationKind, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6fafd93)
 #74 0x000055a2ff5b5403 void llvm::function_ref<void ()>::callback_fn<clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*)::'lambda'()>(long) SemaType.cpp:0:0
 #75 0x000055a2feaf83a5 clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65803a5)
 #76 0x000055a2ff5be957 clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7046957)
 #77 0x000055a2ff5beca9 clang::Sema::RequireCompleteType(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7046ca9)
 #78 0x000055a2feb3c7ac clang::Sema::RequireCompleteDeclContext(clang::CXXScopeSpec&, clang::DeclContext*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65c47ac)
 #79 0x000055a2ff3795c6 clang::Sema::LookupTemplateName(clang::LookupResult&, clang::Scope*, clang::CXXScopeSpec&, clang::QualType, bool, bool&, clang::Sema::RequiredTemplateKind, clang::Sema::AssumedTemplateKind*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e015c6)
 #80 0x000055a2ff37ab03 clang::Sema::isTemplateName(clang::Scope*, clang::CXXScopeSpec&, bool, clang::UnqualifiedId const&, clang::OpaquePtr<clang::QualType>, bool, clang::OpaquePtr<clang::TemplateName>&, bool&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e02b03)
 #81 0x000055a2ff3800a6 clang::Sema::ActOnTemplateName(clang::Scope*, clang::CXXScopeSpec&, clang::SourceLocation, clang::UnqualifiedId const&, clang::OpaquePtr<clang::QualType>, bool, clang::OpaquePtr<clang::TemplateName>&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e080a6)
 #82 0x000055a2ff4d8f2a (anonymous namespace)::TemplateInstantiator::TransformTemplateName(clang::CXXScopeSpec&, clang::TemplateName, clang::SourceLocation, clang::QualType, clang::NamedDecl*, bool) SemaTemplateInstantiate.cpp:0:0
 #83 0x000055a2ff50482f clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformTemplateArgument(clang::TemplateArgumentLoc const&, clang::TemplateArgumentLoc&, bool) SemaTemplateInstantiate.cpp:0:0
 #84 0x000055a2ff507c78 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformTemplateSpecializationType(clang::TypeLocBuilder&, clang::TemplateSpecializationTypeLoc, clang::TemplateName) SemaTemplateInstantiate.cpp:0:0
 #85 0x000055a2ff4fb2c8 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
 #86 0x000055a2ff50ffb0 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformElaboratedType(clang::TypeLocBuilder&, clang::ElaboratedTypeLoc) SemaTemplateInstantiate.cpp:0:0
 #87 0x000055a2ff4faccd clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
 #88 0x000055a2ff50029f clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeSourceInfo*) SemaTemplateInstantiate.cpp:0:0
 #89 0x000055a2ff5003d9 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::QualType) SemaTemplateInstantiate.cpp:0:0
 #90 0x000055a2ff501f23 clang::Sema::SubstType(clang::QualType, clang::MultiLevelTemplateArgumentList const&, clang::SourceLocation, clang::DeclarationName) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f89f23)
 #91 0x000055a2ff395802 clang::Sema::CheckTemplateIdType(clang::TemplateName, clang::SourceLocation, clang::TemplateArgumentListInfo&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e1d802)
 #92 0x000055a2ff507af5 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformTemplateSpecializationType(clang::TypeLocBuilder&, clang::TemplateSpecializationTypeLoc, clang::TemplateName) SemaTemplateInstantiate.cpp:0:0
 #93 0x000055a2ff4fb2c8 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
 #94 0x000055a2ff50ffb0 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformElaboratedType(clang::TypeLocBuilder&, clang::ElaboratedTypeLoc) SemaTemplateInstantiate.cpp:0:0
 #95 0x000055a2ff4faccd clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
 #96 0x000055a2ff50029f clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeSourceInfo*) SemaTemplateInstantiate.cpp:0:0
 #97 0x000055a2ff5003d9 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::QualType) SemaTemplateInstantiate.cpp:0:0
 #98 0x000055a2ff501f23 clang::Sema::SubstType(clang::QualType, clang::MultiLevelTemplateArgumentList const&, clang::SourceLocation, clang::DeclarationName) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f89f23)
 #99 0x000055a2ff395802 clang::Sema::CheckTemplateIdType(clang::TemplateName, clang::SourceLocation, clang::TemplateArgumentListInfo&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e1d802)
#100 0x000055a2ff507af5 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformTemplateSpecializationType(clang::TypeLocBuilder&, clang::TemplateSpecializationTypeLoc, clang::TemplateName) SemaTemplateInstantiate.cpp:0:0
#101 0x000055a2ff4fb2c8 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
#102 0x000055a2ff50ffb0 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformElaboratedType(clang::TypeLocBuilder&, clang::ElaboratedTypeLoc) SemaTemplateInstantiate.cpp:0:0
#103 0x000055a2ff4faccd clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
#104 0x000055a2ff50029f clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeSourceInfo*) SemaTemplateInstantiate.cpp:0:0
#105 0x000055a2ff504718 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformTemplateArgument(clang::TemplateArgumentLoc const&, clang::TemplateArgumentLoc&, bool) SemaTemplateInstantiate.cpp:0:0
#106 0x000055a2ff505f68 bool clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformTemplateArguments<clang::TemplateArgumentLoc const*>(clang::TemplateArgumentLoc const*, clang::TemplateArgumentLoc const*, clang::TemplateArgumentListInfo&, bool) (.constprop.0) SemaTemplateInstantiate.cpp:0:0
#107 0x000055a2ff50cccc clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformUnresolvedLookupExpr(clang::UnresolvedLookupExpr*) SemaTemplateInstantiate.cpp:0:0
#108 0x000055a2ff4ea237 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#109 0x000055a2ff4eafb8 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformInitializer(clang::Expr*, bool) (.part.0) SemaTemplateInstantiate.cpp:0:0
#110 0x000055a2ff4eb765 clang::Sema::SubstInitializer(clang::Expr*, clang::MultiLevelTemplateArgumentList const&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f73765)
#111 0x000055a2ff533352 clang::Sema::InstantiateVariableInitializer(clang::VarDecl*, clang::VarDecl*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6fbb352)
#112 0x000055a2ff5337b6 clang::Sema::CompleteVarTemplateSpecializationDecl(clang::VarTemplateSpecializationDecl*, clang::VarDecl*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6fbb7b6)
#113 0x000055a2ff57605e clang::Sema::InstantiateVariableDefinition(clang::SourceLocation, clang::VarDecl*, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ffe05e)
#114 0x000055a2feaf83a5 clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65803a5)
#115 0x000055a2fef06f02 DoMarkVarDeclReferenced(clang::Sema&, clang::SourceLocation, clang::VarDecl*, clang::Expr*, llvm::DenseMap<clang::VarDecl const*, int, llvm::DenseMapInfo<clang::VarDecl const*, void>, llvm::detail::DenseMapPair<clang::VarDecl const*, int>>&) SemaExpr.cpp:0:0
#116 0x000055a2fef07511 MarkExprReferenced(clang::Sema&, clang::SourceLocation, clang::Decl*, clang::Expr*, bool, llvm::DenseMap<clang::VarDecl const*, int, llvm::DenseMapInfo<clang::VarDecl const*, void>, llvm::detail::DenseMapPair<clang::VarDecl const*, int>>&) SemaExpr.cpp:0:0
#117 0x000055a2fef08a31 clang::Sema::BuildDeclRefExpr(clang::ValueDecl*, clang::QualType, clang::ExprValueKind, clang::DeclarationNameInfo const&, clang::NestedNameSpecifierLoc, clang::NamedDecl*, clang::SourceLocation, clang::TemplateArgumentListInfo const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6990a31)
#118 0x000055a2fef09230 clang::Sema::BuildDeclRefExpr(clang::ValueDecl*, clang::QualType, clang::ExprValueKind, clang::DeclarationNameInfo const&, clang::CXXScopeSpec const*, clang::NamedDecl*, clang::SourceLocation, clang::TemplateArgumentListInfo const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6991230)
#119 0x000055a2fef0e757 clang::Sema::BuildDeclarationNameExpr(clang::CXXScopeSpec const&, clang::DeclarationNameInfo const&, clang::NamedDecl*, clang::NamedDecl*, clang::TemplateArgumentListInfo const*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6996757)
#120 0x000055a2ff394c64 clang::Sema::CheckVarTemplateId(clang::CXXScopeSpec const&, clang::DeclarationNameInfo const&, clang::VarTemplateDecl*, clang::SourceLocation, clang::TemplateArgumentListInfo const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e1cc64)
#121 0x000055a2ff39a40e clang::Sema::BuildTemplateIdExpr(clang::CXXScopeSpec const&, clang::SourceLocation, clang::LookupResult&, bool, clang::TemplateArgumentListInfo const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e2240e)
#122 0x000055a2fef4789d clang::Sema::ActOnIdExpression(clang::Scope*, clang::CXXScopeSpec&, clang::SourceLocation, clang::UnqualifiedId&, bool, bool, clang::CorrectionCandidateCallback*, bool, clang::Token*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69cf89d)
#123 0x000055a2fea2577a clang::Parser::tryParseCXXIdExpression(clang::CXXScopeSpec&, bool, clang::Token&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64ad77a)
#124 0x000055a2fea259db clang::Parser::ParseCXXIdExpression(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64ad9db)
#125 0x000055a2fea05a7c clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x648da7c)
#126 0x000055a2fea0613c clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x648e13c)
#127 0x000055a2fea12d6b clang::Parser::ParseConstantExpressionInExprEvalContext(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x649ad6b)
#128 0x000055a2fe9eca9b clang::Parser::ParseStaticAssertDeclaration(clang::SourceLocation&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6474a9b)
#129 0x000055a2fe9de177 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6466177)
#130 0x000055a2fe9a8d54 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6430d54)
#131 0x000055a2fe9aa316 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6432316)
#132 0x000055a2fe99e5ea clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64265ea)
#133 0x000055a2fd4706b8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ef86b8)
#134 0x000055a2fccb2f19 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x473af19)
#135 0x000055a2fcc37b36 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x46bfb36)
#136 0x000055a2fcd981c6 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48201c6)
#137 0x000055a2f968d094 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x1115094)
#138 0x000055a2f9688dba ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#139 0x000055a2fca915cd void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#140 0x000055a2fc130980 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3bb8980)
#141 0x000055a2fca91bef clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#142 0x000055a2fca587cc clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x44e07cc)
#143 0x000055a2fca5925d clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x44e125d)
#144 0x000055a2fca6133d clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x44e933d)
#145 0x000055a2f968b30a clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x111330a)
#146 0x000055a2f9591705 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x1019705)
#147 0x00007fb8eb37f083 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x24083)
#148 0x000055a2f9683b9e _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x110bb9e)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

Code:

```cpp
template <class> _declval;
template <class From, class> using _copy_cvref_t = From;
template <class Ty> using _unwrap_t = decltype(_declval<Ty>);
template <class Self, class Ty>
using _element_t = _unwrap_t<_copy_cvref_t<Self, Ty>>;
template <class... Ts,
          auto GenImpl =
              []<class...> { []<class Self, _element_t<Self, Ts>> {}; }>
auto _make_impl() {
  GenImpl();
}
static_assert(_make_impl<>);
```

https://godbolt.org/z/3cb4K6Wbn

## Comments

### Comment 1 - llvmbot

@llvm/issue-subscribers-clang-frontend

---

### Comment 2 - shafik

Confirmed

---

### Comment 3 - chfast

Still happens in current trunk (19).

---

### Comment 4 - Endilll

While reducing #77250, I stumbled upon this assertion (https://godbolt.org/z/99K9bdExr):
```cpp
auto __make_tuple = []< class _Cvref, class _Fun >(_Cvref, _Fun) -> void
                      requires requires { _Fun{}(); } {};

template < class _Fun, class... _As >
using __call_result_t = decltype(__make_tuple(_As{}...));

struct __sexpr_uncurry_fn {
  void operator()();
};

using T = __call_result_t< decltype(__make_tuple), int, __sexpr_uncurry_fn >;
```
Clang 20.0 crash:
```
clang++: /root/llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:2514:
clang::QualType {anonymous}::TemplateInstantiator::TransformTemplateTypeParmType(clang::TypeLocBuilder&, clang::TemplateTypeParmTypeLoc, bool):
Assertion `Arg.getKind() == TemplateArgument::Type && "Template argument kind mismatch"' failed.

PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 <source>
1.	<source>:11:11: at annotation token
 #0 0x0000000003abe608 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3abe608)
 #1 0x0000000003abc2fc llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3abc2fc)
 #2 0x0000000003a052f8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000073cd6a042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x000073cd6a0969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x000073cd6a042476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x000073cd6a0287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x000073cd6a02871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x000073cd6a039e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000007148d5c (anonymous namespace)::TemplateInstantiator::TransformTemplateTypeParmType(clang::TypeLocBuilder&, clang::TemplateTypeParmTypeLoc, bool) SemaTemplateInstantiate.cpp:0:0
#10 0x000000000716e7e4 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
#11 0x00000000071749fa clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeSourceInfo*) SemaTemplateInstantiate.cpp:0:0
#12 0x0000000007181f75 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformTypeWithDeducedTST(clang::TypeSourceInfo*) SemaTemplateInstantiate.cpp:0:0
#13 0x00000000071850ef clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCXXUnresolvedConstructExpr(clang::CXXUnresolvedConstructExpr*) SemaTemplateInstantiate.cpp:0:0
#14 0x000000000715d409 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#15 0x000000000716984e clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCallExpr(clang::CallExpr*) SemaTemplateInstantiate.cpp:0:0
#16 0x000000000715d445 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#17 0x0000000007192c08 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformRequiresExpr(clang::RequiresExpr*) SemaTemplateInstantiate.cpp:0:0
#18 0x0000000007194a2e (anonymous namespace)::TemplateInstantiator::TransformRequiresExpr(clang::RequiresExpr*) SemaTemplateInstantiate.cpp:0:0
#19 0x000000000715d829 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#20 0x000000000716cc94 clang::Sema::SubstConstraintExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x716cc94)
#21 0x00000000067b9e8f clang::ActionResult<clang::Expr*, true> calculateConstraintSatisfaction<calculateConstraintSatisfaction(clang::Sema&, clang::NamedDecl const*, clang::SourceLocation, clang::MultiLevelTemplateArgumentList const&, clang::Expr const*, clang::ConstraintSatisfaction&)::ConstraintEvaluator>(clang::Sema&, clang::Expr const*, clang::ConstraintSatisfaction&, calculateConstraintSatisfaction(clang::Sema&, clang::NamedDecl const*, clang::SourceLocation, clang::MultiLevelTemplateArgumentList const&, clang::Expr const*, clang::ConstraintSatisfaction&)::ConstraintEvaluator const&) SemaConcept.cpp:0:0
#22 0x00000000067bbe7e CheckConstraintSatisfaction(clang::Sema&, clang::NamedDecl const*, llvm::ArrayRef<clang::Expr const*>, llvm::SmallVectorImpl<clang::Expr*>&, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&) SemaConcept.cpp:0:0
#23 0x00000000067bc4dd clang::Sema::CheckConstraintSatisfaction(clang::NamedDecl const*, llvm::ArrayRef<clang::Expr const*>, llvm::SmallVectorImpl<clang::Expr*>&, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67bc4dd)
#24 0x00000000067bd52b clang::Sema::CheckInstantiatedFunctionTemplateConstraints(clang::SourceLocation, clang::FunctionDecl*, llvm::ArrayRef<clang::TemplateArgument>, clang::ConstraintSatisfaction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67bd52b)
#25 0x00000000070a6702 clang::Sema::FinishTemplateArgumentDeduction(clang::FunctionTemplateDecl*, llvm::SmallVectorImpl<clang::DeducedTemplateArgument>&, unsigned int, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, llvm::SmallVectorImpl<clang::Sema::OriginalCallArg> const*, bool, llvm::function_ref<bool ()>) (.constprop.0) SemaTemplateDeduction.cpp:0:0
#26 0x00000000070a710a void llvm::function_ref<void ()>::callback_fn<clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, bool, clang::QualType, clang::Expr::Classification, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>)>)::'lambda1'()>(long) SemaTemplateDeduction.cpp:0:0
#27 0x0000000006640431 clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6640431)
#28 0x00000000070b2212 clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, bool, clang::QualType, clang::Expr::Classification, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>)>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x70b2212)
#29 0x0000000006ec639d clang::Sema::AddMethodTemplateCandidate(clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::CXXRecordDecl*, clang::TemplateArgumentListInfo*, clang::QualType, clang::Expr::Classification, llvm::ArrayRef<clang::Expr*>, clang::OverloadCandidateSet&, bool, bool, clang::OverloadCandidateParamOrder) (.constprop.0) SemaOverload.cpp:0:0
#30 0x0000000006ec6d21 clang::Sema::AddMethodCandidate(clang::DeclAccessPair, clang::QualType, clang::Expr::Classification, llvm::ArrayRef<clang::Expr*>, clang::OverloadCandidateSet&, bool, clang::OverloadCandidateParamOrder) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ec6d21)
#31 0x0000000006ed72e2 clang::Sema::BuildCallToObjectOfClassType(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ed72e2)
#32 0x0000000006ac4ccd clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ac4ccd)
#33 0x0000000006ac6cec clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ac6cec)
#34 0x0000000007169920 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCallExpr(clang::CallExpr*) SemaTemplateInstantiate.cpp:0:0
#35 0x000000000715d445 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#36 0x000000000716e047 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
#37 0x00000000071749fa clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeSourceInfo*) SemaTemplateInstantiate.cpp:0:0
#38 0x0000000007174b1c clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::QualType) SemaTemplateInstantiate.cpp:0:0
#39 0x00000000071767db clang::Sema::SubstType(clang::QualType, clang::MultiLevelTemplateArgumentList const&, clang::SourceLocation, clang::DeclarationName) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x71767db)
#40 0x0000000006fad6e6 clang::Sema::CheckTemplateIdType(clang::TemplateName, clang::SourceLocation, clang::TemplateArgumentListInfo&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6fad6e6)
#41 0x0000000006fafaa7 clang::Sema::ActOnTemplateIdType(clang::Scope*, clang::CXXScopeSpec&, clang::SourceLocation, clang::OpaquePtr<clang::TemplateName>, clang::IdentifierInfo const*, clang::SourceLocation, clang::SourceLocation, llvm::MutableArrayRef<clang::ParsedTemplateArgument>, clang::SourceLocation, bool, bool, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6fafaa7)
#42 0x00000000065d9e4b clang::Parser::AnnotateTemplateIdTokenAsType(clang::CXXScopeSpec&, clang::ImplicitTypenameContext, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65d9e4b)
#43 0x0000000006515492 clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6515492)
#44 0x0000000006518212 clang::Parser::ParseSpecifierQualifierList(clang::DeclSpec&, clang::ImplicitTypenameContext, clang::AccessSpecifier, clang::Parser::DeclSpecContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6518212)
#45 0x000000000651f1f0 clang::Parser::ParseTypeName(clang::SourceRange*, clang::DeclaratorContext, clang::AccessSpecifier, clang::Decl**, clang::ParsedAttributes*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x651f1f0)
#46 0x000000000652cb59 clang::Parser::ParseAliasDeclarationAfterDeclarator(clang::Parser::ParsedTemplateInfo const&, clang::SourceLocation, clang::Parser::UsingDeclarator&, clang::SourceLocation&, clang::AccessSpecifier, clang::ParsedAttributes&, clang::Decl**) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x652cb59)
#47 0x000000000653664e clang::Parser::ParseUsingDeclaration(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo const&, clang::SourceLocation, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x653664e)
#48 0x00000000065373a5 clang::Parser::ParseUsingDirectiveOrDeclaration(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo const&, clang::SourceLocation&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65373a5)
#49 0x000000000651bbdf clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x651bbdf)
#50 0x00000000064d89e1 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64d89e1)
#51 0x00000000064d986d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64d986d)
#52 0x00000000064ccd3a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64ccd3a)
#53 0x00000000043d11d8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x43d11d8)
#54 0x000000000465d109 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x465d109)
#55 0x00000000045e170e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x45e170e)
#56 0x00000000047465de clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x47465de)
#57 0x0000000000c9299f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc9299f)
#58 0x0000000000c8bb2a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#59 0x0000000004414819 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#60 0x0000000003a057a4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3a057a4)
#61 0x0000000004414e0f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#62 0x00000000043da9d5 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x43da9d5)
#63 0x00000000043db43d clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x43db43d)
#64 0x00000000043e3055 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x43e3055)
#65 0x0000000000c8fcd5 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc8fcd5)
#66 0x0000000000b63ba4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xb63ba4)
#67 0x000073cd6a029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#68 0x000073cd6a029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#69 0x0000000000c8b5de _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc8b5de)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

---

### Comment 5 - MagentaTreehouse

This is no longer crashing in trunk.

---

