# Assertion `Arg.getKind() == TemplateArgument::Pack && "Missing argument pack"' failed

**Author:** alexfh
**URL:** https://github.com/llvm/llvm-project/issues/164327

## Body

I got this assertion failure while trying to reduce a test case for another crash. I don't know if the code is valid: https://gcc.godbolt.org/z/jz53bqKh5

```
template <typename> struct FieldId;
template <FieldId field> constexpr auto FieldIdToInnerValue() {
  return field.template ToInnerValue<field>();
}
template <FieldId> struct PathImpl;
template <typename T> struct PathFieldId {
  template <typename Arg> constexpr PathFieldId(Arg) : value() {}
  T value;
};
template <PathFieldId...> constexpr auto PathImplHelper();
enum class FieldNumber;
template <PathFieldId... fields> auto Path = PathImplHelper<fields...>();
template <> struct FieldId<FieldNumber> {
  constexpr FieldId(FieldNumber) : number() {}
  FieldNumber number;
};
FieldId(FieldNumber) -> FieldId<FieldNumber>;
template <typename T> PathFieldId(T t) -> PathFieldId<decltype(t)>;
template <FieldId... fields>
PathImpl<fields...> *ConcatPath(PathImpl<fields...> *);
template <FieldId field> PathImpl<field> *SinglePath();
template <PathFieldId... fields> constexpr auto PathImplHelper() {
  return ConcatPath(SinglePath<fields.value>()...);
}
template <auto... fields> struct FieldId<const PathImpl<fields...> *> {
  constexpr FieldId(PathImpl<fields...> *) : path() {}
  template <auto field> static constexpr auto ToInnerValue() {
    return field.path;
  }
  PathImpl<fields...> *path;
};
template <auto... fields>
FieldId(PathImpl<fields...> *) -> FieldId<const PathImpl<fields...> *>;
template <auto> struct UnpackedField {
  static constexpr bool is_path = false;
};
template <auto... fields, PathImpl<fields> *path> struct UnpackedField<path>;
constexpr auto Field() { return FieldNumber{}; }
template <FieldId field> constexpr bool IsPath() {
  return UnpackedField<FieldIdToInnerValue<field>()>::is_path;
}
template <typename, typename, typename F>
void ForEachSimplePathImpl(F callable) {
  constexpr auto kNewField = Field();
  callable(Path<kNewField>);
}
template <typename F> void ForEachSimplePath(F callable) {
  ForEachSimplePathImpl<int, int>(callable);
}
template <FieldId> struct FieldNameBuilder;
template <FieldId field>
  requires(!IsPath<field>())
struct FieldNameBuilder<field> {
  template <typename> static void Get();
};
template <typename, FieldId field> void FieldName() {
  FieldNameBuilder<field>::template Get<int>;
}
struct ProtoSpec {
  ProtoSpec() {
    ForEachSimplePath([](auto path) { RegisterField(path); });
  }
  static void RegisterField(auto path) {
    constexpr auto kPath = decltype(path){};
    FieldName<int, kPath>();
  }
};
```

```
clang++: /root/llvm-project/llvm/tools/clang/lib/Sema/SemaTemplateInstantiate.cpp:2240: clang::ExprResult {anonymous}::TemplateInstantiator::TransformTemplateParmRefExpr(clang::DeclRefExpr*, clang::NonTypeTemplateParmDecl*): Assertion `Arg.getKind() == TemplateArgument::Pack && "Missing argument pack"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fverbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 <source>
1.	<eof> parser at end of file
2.	<source>:47:28: instantiating function definition 'ForEachSimplePath<(lambda at <source>:61:23)>'
3.	<source>:43:6: instantiating function definition 'ForEachSimplePathImpl<int, int, (lambda at <source>:61:23)>'
4.	<source>:63:15: instantiating function definition 'ProtoSpec::RegisterField<PathImpl<FieldId<FieldNumber>{0}> *>'
5.	<source>:56:41: instantiating function definition 'FieldName<int, FieldId<const PathImpl<FieldId<FieldNumber>{0}> *>{nullptr}>'
6.	<source>:39:41: instantiating function definition 'IsPath<FieldId<const PathImpl<FieldId<FieldNumber>{0}> *>{nullptr}>'
  #0 0x00000000041f5e08 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41f5e08)
  #1 0x00000000041f3234 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41f3234)
  #2 0x0000000004137498 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
  #3 0x00007b5ab3642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
  #4 0x00007b5ab36969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
  #5 0x00007b5ab3642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
  #6 0x00007b5ab36287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
  #7 0x00007b5ab362871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
  #8 0x00007b5ab3639e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
  #9 0x000000000744df14 (anonymous namespace)::TemplateInstantiator::TransformDeclRefExpr(clang::DeclRefExpr*) SemaTemplateInstantiate.cpp:0:0
 #10 0x000000000741d71e clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
 #11 0x00000000074346e7 (anonymous namespace)::TemplateInstantiator::TransformTemplateArgument(clang::TemplateArgumentLoc const&, clang::TemplateArgumentLoc&, bool) SemaTemplateInstantiate.cpp:0:0
 #12 0x0000000007449077 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformTemplateSpecializationType(clang::TypeLocBuilder&, clang::TemplateSpecializationTypeLoc, clang::QualType, clang::NamedDecl*, bool) SemaTemplateInstantiate.cpp:0:0
 #13 0x000000000742840d clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
 #14 0x000000000742bf41 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformPointerType(clang::TypeLocBuilder&, clang::PointerTypeLoc) SemaTemplateInstantiate.cpp:0:0
 #15 0x0000000007429103 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
 #16 0x000000000742c847 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeSourceInfo*) SemaTemplateInstantiate.cpp:0:0
 #17 0x000000000742c95c clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::QualType) SemaTemplateInstantiate.cpp:0:0
 #18 0x000000000742cffa clang::Sema::SubstType(clang::QualType, clang::MultiLevelTemplateArgumentList const&, clang::SourceLocation, clang::DeclarationName, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x742cffa)
 #19 0x00000000072ee1fe clang::Sema::CheckTemplateArgument(clang::NamedDecl*, clang::TemplateArgumentLoc&, clang::NamedDecl*, clang::SourceLocation, clang::SourceLocation, unsigned int, clang::Sema::CheckTemplateArgumentInfo&, clang::Sema::CheckTemplateArgumentKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x72ee1fe)
 #20 0x000000000735dc4f ConvertDeducedTemplateArguments(clang::Sema&, clang::NamedDecl*, clang::TemplateParameterList*, bool, llvm::SmallVectorImpl<clang::DeducedTemplateArgument>&, clang::sema::TemplateDeductionInfo&, clang::Sema::CheckTemplateArgumentInfo&, clang::LocalInstantiationScope*, unsigned int, bool*) SemaTemplateDeduction.cpp:0:0
 #21 0x000000000735fd87 FinishTemplateArgumentDeduction(clang::Sema&, clang::NamedDecl*, clang::TemplateParameterList*, clang::TemplateDecl*, bool, llvm::ArrayRef<clang::TemplateArgumentLoc>, llvm::ArrayRef<clang::TemplateArgument>, llvm::SmallVectorImpl<clang::DeducedTemplateArgument>&, clang::sema::TemplateDeductionInfo&, bool) SemaTemplateDeduction.cpp:0:0
 #22 0x0000000007360c12 _ZN4llvm12function_refIFvvEE11callback_fnIZ23DeduceTemplateArgumentsIN5clang38ClassTemplatePartialSpecializationDeclEENSt9enable_ifIXsr23IsPartialSpecializationIT_E5valueENS5_23TemplateDeductionResultEE4typeERNS5_4SemaEPS9_NS_8ArrayRefINS5_16TemplateArgumentEEERNS5_4sema21TemplateDeductionInfoEEUlvE_EEvl SemaTemplateDeduction.cpp:0:0
 #23 0x000000000821a121 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x821a121)
 #24 0x000000000736b29a _Z23DeduceTemplateArgumentsIN5clang38ClassTemplatePartialSpecializationDeclEENSt9enable_ifIXsr23IsPartialSpecializationIT_E5valueENS0_23TemplateDeductionResultEE4typeERNS0_4SemaEPS4_N4llvm8ArrayRefINS0_16TemplateArgumentEEERNS0_4sema21TemplateDeductionInfoE SemaTemplateDeduction.cpp:0:0
 #25 0x0000000007415a3d getPatternForClassTemplateSpecialization(clang::Sema&, clang::SourceLocation, clang::ClassTemplateSpecializationDecl*, clang::TemplateSpecializationKind, bool) (.constprop.0) SemaTemplateInstantiate.cpp:0:0
 #26 0x000000000749dd30 clang::Sema::InstantiateClassTemplateSpecialization(clang::SourceLocation, clang::ClassTemplateSpecializationDecl*, clang::TemplateSpecializationKind, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x749dd30)
 #27 0x000000000751e38e void llvm::function_ref<void ()>::callback_fn<clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*)::'lambda'()>(long) SemaType.cpp:0:0
 #28 0x000000000821a121 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x821a121)
 #29 0x0000000007529da3 clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7529da3)
 #30 0x000000000752a23f clang::Sema::RequireCompleteType(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x752a23f)
 #31 0x0000000006b005b3 clang::Sema::RequireCompleteDeclContext(clang::CXXScopeSpec&, clang::DeclContext*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b005b3)
 #32 0x0000000006ff5e64 clang::Sema::LookupParsedName(clang::LookupResult&, clang::Scope*, clang::CXXScopeSpec*, clang::QualType, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ff5e64)
 #33 0x0000000006e40700 clang::Sema::BuildQualifiedDeclarationNameExpr(clang::CXXScopeSpec&, clang::DeclarationNameInfo const&, bool, clang::TypeSourceInfo**) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e40700)
 #34 0x000000000744ac74 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformDependentScopeDeclRefExpr(clang::DependentScopeDeclRefExpr*, bool, clang::TypeSourceInfo**) SemaTemplateInstantiate.cpp:0:0
 #35 0x000000000741d732 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
 #36 0x000000000743d285 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformInitializer(clang::Expr*, bool) (.part.0) SemaTemplateInstantiate.cpp:0:0
 #37 0x000000000743d60c clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformReturnStmt(clang::ReturnStmt*) SemaTemplateInstantiate.cpp:0:0
 #38 0x000000000745aa0e clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaTemplateInstantiate.cpp:0:0
 #39 0x0000000007463dad clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7463dad)
 #40 0x000000000750007f clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x750007f)
 #41 0x0000000006e4ac8e void llvm::function_ref<void ()>::callback_fn<clang::Sema::MarkFunctionReferenced(clang::SourceLocation, clang::FunctionDecl*, bool)::'lambda'()>(long) SemaExpr.cpp:0:0
 #42 0x000000000821a121 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x821a121)
 #43 0x0000000006e4902f clang::Sema::MarkFunctionReferenced(clang::SourceLocation, clang::FunctionDecl*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e4902f)
 #44 0x0000000006e49655 MarkExprReferenced(clang::Sema&, clang::SourceLocation, clang::Decl*, clang::Expr*, bool, llvm::DenseMap<clang::VarDecl const*, int, llvm::DenseMapInfo<clang::VarDecl const*, void>, llvm::detail::DenseMapPair<clang::VarDecl const*, int>>&) SemaExpr.cpp:0:0
 #45 0x0000000006e39f29 clang::Sema::BuildDeclRefExpr(clang::ValueDecl*, clang::QualType, clang::ExprValueKind, clang::DeclarationNameInfo const&, clang::NestedNameSpecifierLoc, clang::NamedDecl*, clang::SourceLocation, clang::TemplateArgumentListInfo const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e39f29)
 #46 0x00000000071c6784 clang::Sema::FixOverloadedFunctionReference(clang::Expr*, clang::DeclAccessPair, clang::FunctionDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x71c6784)
 #47 0x000000000720003e FinishOverloadedCallExpr(clang::Sema&, clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, clang::OverloadCandidateSet*, clang::OverloadCandidate**, clang::OverloadingResult, bool) SemaOverload.cpp:0:0
 #48 0x00000000072010d0 clang::Sema::BuildOverloadedCallExpr(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x72010d0)
 #49 0x0000000006e6ac1a clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e6ac1a)
 #50 0x0000000006e7009c clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e7009c)
 #51 0x0000000007440803 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCallExpr(clang::CallExpr*) SemaTemplateInstantiate.cpp:0:0
 #52 0x000000000741d667 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
 #53 0x0000000007464381 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformUnaryOperator(clang::UnaryOperator*) SemaTemplateInstantiate.cpp:0:0
 #54 0x000000000741db5d clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
 #55 0x00000000074573d7 clang::Sema::SubstConstraintExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x74573d7)
 #56 0x0000000006c3636d (anonymous namespace)::ConstraintSatisfactionChecker::Evaluate(clang::AtomicConstraint const&, clang::MultiLevelTemplateArgumentList const&) SemaConcept.cpp:0:0
 #57 0x0000000006c3ac25 CheckConstraintSatisfaction(clang::Sema&, clang::NamedDecl const*, llvm::ArrayRef<clang::AssociatedConstraint>, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&, clang::Expr**, clang::ConceptReference const*) SemaConcept.cpp:0:0
 #58 0x0000000006c3b3bd clang::Sema::CheckConstraintSatisfaction(llvm::PointerUnion<clang::NamedDecl const*, clang::concepts::NestedRequirement const*>, llvm::ArrayRef<clang::AssociatedConstraint>, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&, clang::ConceptReference const*, clang::Expr**) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c3b3bd)
 #59 0x000000000734d8c1 CheckDeducedArgumentConstraints(clang::Sema&, clang::NamedDecl*, llvm::ArrayRef<clang::TemplateArgument>, llvm::ArrayRef<clang::TemplateArgument>, clang::sema::TemplateDeductionInfo&) SemaTemplateDeduction.cpp:0:0
 #60 0x000000000736089d FinishTemplateArgumentDeduction(clang::Sema&, clang::NamedDecl*, clang::TemplateParameterList*, clang::TemplateDecl*, bool, llvm::ArrayRef<clang::TemplateArgumentLoc>, llvm::ArrayRef<clang::TemplateArgument>, llvm::SmallVectorImpl<clang::DeducedTemplateArgument>&, clang::sema::TemplateDeductionInfo&, bool) SemaTemplateDeduction.cpp:0:0
 #61 0x0000000007360c12 _ZN4llvm12function_refIFvvEE11callback_fnIZ23DeduceTemplateArgumentsIN5clang38ClassTemplatePartialSpecializationDeclEENSt9enable_ifIXsr23IsPartialSpecializationIT_E5valueENS5_23TemplateDeductionResultEE4typeERNS5_4SemaEPS9_NS_8ArrayRefINS5_16TemplateArgumentEEERNS5_4sema21TemplateDeductionInfoEEUlvE_EEvl SemaTemplateDeduction.cpp:0:0
 #62 0x000000000821a121 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x821a121)
 #63 0x000000000736b29a _Z23DeduceTemplateArgumentsIN5clang38ClassTemplatePartialSpecializationDeclEENSt9enable_ifIXsr23IsPartialSpecializationIT_E5valueENS0_23TemplateDeductionResultEE4typeERNS0_4SemaEPS4_N4llvm8ArrayRefINS0_16TemplateArgumentEEERNS0_4sema21TemplateDeductionInfoE SemaTemplateDeduction.cpp:0:0
 #64 0x0000000007415a3d getPatternForClassTemplateSpecialization(clang::Sema&, clang::SourceLocation, clang::ClassTemplateSpecializationDecl*, clang::TemplateSpecializationKind, bool) (.constprop.0) SemaTemplateInstantiate.cpp:0:0
 #65 0x000000000749dd30 clang::Sema::InstantiateClassTemplateSpecialization(clang::SourceLocation, clang::ClassTemplateSpecializationDecl*, clang::TemplateSpecializationKind, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x749dd30)
 #66 0x000000000751e38e void llvm::function_ref<void ()>::callback_fn<clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*)::'lambda'()>(long) SemaType.cpp:0:0
 #67 0x000000000821a121 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x821a121)
 #68 0x0000000007529da3 clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7529da3)
 #69 0x000000000752a23f clang::Sema::RequireCompleteType(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x752a23f)
 #70 0x0000000006b005b3 clang::Sema::RequireCompleteDeclContext(clang::CXXScopeSpec&, clang::DeclContext*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b005b3)
 #71 0x00000000072deb8a clang::Sema::LookupTemplateName(clang::LookupResult&, clang::Scope*, clang::CXXScopeSpec&, clang::QualType, bool, clang::Sema::RequiredTemplateKind, clang::Sema::AssumedTemplateKind*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x72deb8a)
 #72 0x00000000072ff13f clang::Sema::BuildQualifiedTemplateIdExpr(clang::CXXScopeSpec&, clang::SourceLocation, clang::DeclarationNameInfo const&, clang::TemplateArgumentListInfo const*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x72ff13f)
 #73 0x000000000744ad24 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformDependentScopeDeclRefExpr(clang::DependentScopeDeclRefExpr*, bool, clang::TypeSourceInfo**) SemaTemplateInstantiate.cpp:0:0
 #74 0x000000000741d732 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
 #75 0x000000000745a357 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformStmt(clang::Stmt*, clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::StmtDiscardKind) SemaTemplateInstantiate.cpp:0:0
 #76 0x000000000745aa0e clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaTemplateInstantiate.cpp:0:0
 #77 0x0000000007463dad clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7463dad)
 #78 0x000000000750007f clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x750007f)
 #79 0x00000000074fdcb6 clang::Sema::PerformPendingInstantiations(bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x74fdcb6)
 #80 0x000000000750042c clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x750042c)
 #81 0x00000000074fdcb6 clang::Sema::PerformPendingInstantiations(bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x74fdcb6)
 #82 0x000000000750042c clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x750042c)
 #83 0x00000000074fdcb6 clang::Sema::PerformPendingInstantiations(bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x74fdcb6)
 #84 0x000000000750042c clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x750042c)
 #85 0x00000000074fdcb6 clang::Sema::PerformPendingInstantiations(bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x74fdcb6)
 #86 0x0000000006a90ad2 clang::Sema::ActOnEndOfTranslationUnitFragment(clang::TUFragmentKind) (.part.0) Sema.cpp:0:0
 #87 0x0000000006a90e52 clang::Sema::ActOnEndOfTranslationUnit() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a90e52)
 #88 0x00000000068f9fd0 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68f9fd0)
 #89 0x00000000068db57a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68db57a)
 #90 0x0000000004b3e668 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b3e668)
 #91 0x0000000004e2bcf5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e2bcf5)
 #92 0x0000000004dacd8e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4dacd8e)
 #93 0x0000000004f2377d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f2377d)
 #94 0x0000000000dc3800 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdc3800)
 #95 0x0000000000dba36a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
 #96 0x0000000000dba4ed int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
 #97 0x0000000004ba7819 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
 #98 0x0000000004137934 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4137934)
 #99 0x0000000004ba7e2f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#100 0x0000000004b69142 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b69142)
#101 0x0000000004b6a0ee clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b6a0ee)
#102 0x0000000004b71825 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b71825)
#103 0x0000000000dbfd29 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdbfd29)
#104 0x0000000000c6fab4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc6fab4)
#105 0x00007b5ab3629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#106 0x00007b5ab3629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#107 0x0000000000db9e05 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb9e05)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Alexander Kornienko (alexfh)

<details>
I got this assertion failure while trying to reduce a test case for another crash. I don't know if the code is valid: https://gcc.godbolt.org/z/jz53bqKh5

```
template &lt;typename&gt; struct FieldId;
template &lt;FieldId field&gt; constexpr auto FieldIdToInnerValue() {
  return field.template ToInnerValue&lt;field&gt;();
}
template &lt;FieldId&gt; struct PathImpl;
template &lt;typename T&gt; struct PathFieldId {
  template &lt;typename Arg&gt; constexpr PathFieldId(Arg) : value() {}
  T value;
};
template &lt;PathFieldId...&gt; constexpr auto PathImplHelper();
enum class FieldNumber;
template &lt;PathFieldId... fields&gt; auto Path = PathImplHelper&lt;fields...&gt;();
template &lt;&gt; struct FieldId&lt;FieldNumber&gt; {
  constexpr FieldId(FieldNumber) : number() {}
  FieldNumber number;
};
FieldId(FieldNumber) -&gt; FieldId&lt;FieldNumber&gt;;
template &lt;typename T&gt; PathFieldId(T t) -&gt; PathFieldId&lt;decltype(t)&gt;;
template &lt;FieldId... fields&gt;
PathImpl&lt;fields...&gt; *ConcatPath(PathImpl&lt;fields...&gt; *);
template &lt;FieldId field&gt; PathImpl&lt;field&gt; *SinglePath();
template &lt;PathFieldId... fields&gt; constexpr auto PathImplHelper() {
  return ConcatPath(SinglePath&lt;fields.value&gt;()...);
}
template &lt;auto... fields&gt; struct FieldId&lt;const PathImpl&lt;fields...&gt; *&gt; {
  constexpr FieldId(PathImpl&lt;fields...&gt; *) : path() {}
  template &lt;auto field&gt; static constexpr auto ToInnerValue() {
    return field.path;
  }
  PathImpl&lt;fields...&gt; *path;
};
template &lt;auto... fields&gt;
FieldId(PathImpl&lt;fields...&gt; *) -&gt; FieldId&lt;const PathImpl&lt;fields...&gt; *&gt;;
template &lt;auto&gt; struct UnpackedField {
  static constexpr bool is_path = false;
};
template &lt;auto... fields, PathImpl&lt;fields&gt; *path&gt; struct UnpackedField&lt;path&gt;;
constexpr auto Field() { return FieldNumber{}; }
template &lt;FieldId field&gt; constexpr bool IsPath() {
  return UnpackedField&lt;FieldIdToInnerValue&lt;field&gt;()&gt;::is_path;
}
template &lt;typename, typename, typename F&gt;
void ForEachSimplePathImpl(F callable) {
  constexpr auto kNewField = Field();
  callable(Path&lt;kNewField&gt;);
}
template &lt;typename F&gt; void ForEachSimplePath(F callable) {
  ForEachSimplePathImpl&lt;int, int&gt;(callable);
}
template &lt;FieldId&gt; struct FieldNameBuilder;
template &lt;FieldId field&gt;
  requires(!IsPath&lt;field&gt;())
struct FieldNameBuilder&lt;field&gt; {
  template &lt;typename&gt; static void Get();
};
template &lt;typename, FieldId field&gt; void FieldName() {
  FieldNameBuilder&lt;field&gt;::template Get&lt;int&gt;;
}
struct ProtoSpec {
  ProtoSpec() {
    ForEachSimplePath([](auto path) { RegisterField(path); });
  }
  static void RegisterField(auto path) {
    constexpr auto kPath = decltype(path){};
    FieldName&lt;int, kPath&gt;();
  }
};
```

```
clang++: /root/llvm-project/llvm/tools/clang/lib/Sema/SemaTemplateInstantiate.cpp:2240: clang::ExprResult {anonymous}::TemplateInstantiator::TransformTemplateParmRefExpr(clang::DeclRefExpr*, clang::NonTypeTemplateParmDecl*): Assertion `Arg.getKind() == TemplateArgument::Pack &amp;&amp; "Missing argument pack"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fverbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	&lt;source&gt;:47:28: instantiating function definition 'ForEachSimplePath&lt;(lambda at &lt;source&gt;:61:23)&gt;'
3.	&lt;source&gt;:43:6: instantiating function definition 'ForEachSimplePathImpl&lt;int, int, (lambda at &lt;source&gt;:61:23)&gt;'
4.	&lt;source&gt;:63:15: instantiating function definition 'ProtoSpec::RegisterField&lt;PathImpl&lt;FieldId&lt;FieldNumber&gt;{0}&gt; *&gt;'
5.	&lt;source&gt;:56:41: instantiating function definition 'FieldName&lt;int, FieldId&lt;const PathImpl&lt;FieldId&lt;FieldNumber&gt;{0}&gt; *&gt;{nullptr}&gt;'
6.	&lt;source&gt;:39:41: instantiating function definition 'IsPath&lt;FieldId&lt;const PathImpl&lt;FieldId&lt;FieldNumber&gt;{0}&gt; *&gt;{nullptr}&gt;'
  #<!-- -->0 0x00000000041f5e08 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41f5e08)
  #<!-- -->1 0x00000000041f3234 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41f3234)
  #<!-- -->2 0x0000000004137498 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
  #<!-- -->3 0x00007b5ab3642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
  #<!-- -->4 0x00007b5ab36969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
  #<!-- -->5 0x00007b5ab3642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
  #<!-- -->6 0x00007b5ab36287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
  #<!-- -->7 0x00007b5ab362871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
  #<!-- -->8 0x00007b5ab3639e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
  #<!-- -->9 0x000000000744df14 (anonymous namespace)::TemplateInstantiator::TransformDeclRefExpr(clang::DeclRefExpr*) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->10 0x000000000741d71e clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->11 0x00000000074346e7 (anonymous namespace)::TemplateInstantiator::TransformTemplateArgument(clang::TemplateArgumentLoc const&amp;, clang::TemplateArgumentLoc&amp;, bool) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->12 0x0000000007449077 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformTemplateSpecializationType(clang::TypeLocBuilder&amp;, clang::TemplateSpecializationTypeLoc, clang::QualType, clang::NamedDecl*, bool) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->13 0x000000000742840d clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeLocBuilder&amp;, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->14 0x000000000742bf41 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformPointerType(clang::TypeLocBuilder&amp;, clang::PointerTypeLoc) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->15 0x0000000007429103 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeLocBuilder&amp;, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->16 0x000000000742c847 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeSourceInfo*) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->17 0x000000000742c95c clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::QualType) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->18 0x000000000742cffa clang::Sema::SubstType(clang::QualType, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceLocation, clang::DeclarationName, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x742cffa)
 #<!-- -->19 0x00000000072ee1fe clang::Sema::CheckTemplateArgument(clang::NamedDecl*, clang::TemplateArgumentLoc&amp;, clang::NamedDecl*, clang::SourceLocation, clang::SourceLocation, unsigned int, clang::Sema::CheckTemplateArgumentInfo&amp;, clang::Sema::CheckTemplateArgumentKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x72ee1fe)
 #<!-- -->20 0x000000000735dc4f ConvertDeducedTemplateArguments(clang::Sema&amp;, clang::NamedDecl*, clang::TemplateParameterList*, bool, llvm::SmallVectorImpl&lt;clang::DeducedTemplateArgument&gt;&amp;, clang::sema::TemplateDeductionInfo&amp;, clang::Sema::CheckTemplateArgumentInfo&amp;, clang::LocalInstantiationScope*, unsigned int, bool*) SemaTemplateDeduction.cpp:0:0
 #<!-- -->21 0x000000000735fd87 FinishTemplateArgumentDeduction(clang::Sema&amp;, clang::NamedDecl*, clang::TemplateParameterList*, clang::TemplateDecl*, bool, llvm::ArrayRef&lt;clang::TemplateArgumentLoc&gt;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, llvm::SmallVectorImpl&lt;clang::DeducedTemplateArgument&gt;&amp;, clang::sema::TemplateDeductionInfo&amp;, bool) SemaTemplateDeduction.cpp:0:0
 #<!-- -->22 0x0000000007360c12 _ZN4llvm12function_refIFvvEE11callback_fnIZ23DeduceTemplateArgumentsIN5clang38ClassTemplatePartialSpecializationDeclEENSt9enable_ifIXsr23IsPartialSpecializationIT_E5valueENS5_23TemplateDeductionResultEE4typeERNS5_4SemaEPS9_NS_8ArrayRefINS5_16TemplateArgumentEEERNS5_4sema21TemplateDeductionInfoEEUlvE_EEvl SemaTemplateDeduction.cpp:0:0
 #<!-- -->23 0x000000000821a121 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x821a121)
 #<!-- -->24 0x000000000736b29a _Z23DeduceTemplateArgumentsIN5clang38ClassTemplatePartialSpecializationDeclEENSt9enable_ifIXsr23IsPartialSpecializationIT_E5valueENS0_23TemplateDeductionResultEE4typeERNS0_4SemaEPS4_N4llvm8ArrayRefINS0_16TemplateArgumentEEERNS0_4sema21TemplateDeductionInfoE SemaTemplateDeduction.cpp:0:0
 #<!-- -->25 0x0000000007415a3d getPatternForClassTemplateSpecialization(clang::Sema&amp;, clang::SourceLocation, clang::ClassTemplateSpecializationDecl*, clang::TemplateSpecializationKind, bool) (.constprop.0) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->26 0x000000000749dd30 clang::Sema::InstantiateClassTemplateSpecialization(clang::SourceLocation, clang::ClassTemplateSpecializationDecl*, clang::TemplateSpecializationKind, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x749dd30)
 #<!-- -->27 0x000000000751e38e void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*)::'lambda'()&gt;(long) SemaType.cpp:0:0
 #<!-- -->28 0x000000000821a121 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x821a121)
 #<!-- -->29 0x0000000007529da3 clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7529da3)
 #<!-- -->30 0x000000000752a23f clang::Sema::RequireCompleteType(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x752a23f)
 #<!-- -->31 0x0000000006b005b3 clang::Sema::RequireCompleteDeclContext(clang::CXXScopeSpec&amp;, clang::DeclContext*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b005b3)
 #<!-- -->32 0x0000000006ff5e64 clang::Sema::LookupParsedName(clang::LookupResult&amp;, clang::Scope*, clang::CXXScopeSpec*, clang::QualType, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ff5e64)
 #<!-- -->33 0x0000000006e40700 clang::Sema::BuildQualifiedDeclarationNameExpr(clang::CXXScopeSpec&amp;, clang::DeclarationNameInfo const&amp;, bool, clang::TypeSourceInfo**) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e40700)
 #<!-- -->34 0x000000000744ac74 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformDependentScopeDeclRefExpr(clang::DependentScopeDeclRefExpr*, bool, clang::TypeSourceInfo**) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->35 0x000000000741d732 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->36 0x000000000743d285 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformInitializer(clang::Expr*, bool) (.part.0) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->37 0x000000000743d60c clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformReturnStmt(clang::ReturnStmt*) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->38 0x000000000745aa0e clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->39 0x0000000007463dad clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7463dad)
 #<!-- -->40 0x000000000750007f clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x750007f)
 #<!-- -->41 0x0000000006e4ac8e void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::Sema::MarkFunctionReferenced(clang::SourceLocation, clang::FunctionDecl*, bool)::'lambda'()&gt;(long) SemaExpr.cpp:0:0
 #<!-- -->42 0x000000000821a121 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x821a121)
 #<!-- -->43 0x0000000006e4902f clang::Sema::MarkFunctionReferenced(clang::SourceLocation, clang::FunctionDecl*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e4902f)
 #<!-- -->44 0x0000000006e49655 MarkExprReferenced(clang::Sema&amp;, clang::SourceLocation, clang::Decl*, clang::Expr*, bool, llvm::DenseMap&lt;clang::VarDecl const*, int, llvm::DenseMapInfo&lt;clang::VarDecl const*, void&gt;, llvm::detail::DenseMapPair&lt;clang::VarDecl const*, int&gt;&gt;&amp;) SemaExpr.cpp:0:0
 #<!-- -->45 0x0000000006e39f29 clang::Sema::BuildDeclRefExpr(clang::ValueDecl*, clang::QualType, clang::ExprValueKind, clang::DeclarationNameInfo const&amp;, clang::NestedNameSpecifierLoc, clang::NamedDecl*, clang::SourceLocation, clang::TemplateArgumentListInfo const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e39f29)
 #<!-- -->46 0x00000000071c6784 clang::Sema::FixOverloadedFunctionReference(clang::Expr*, clang::DeclAccessPair, clang::FunctionDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x71c6784)
 #<!-- -->47 0x000000000720003e FinishOverloadedCallExpr(clang::Sema&amp;, clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, clang::OverloadCandidateSet*, clang::OverloadCandidate**, clang::OverloadingResult, bool) SemaOverload.cpp:0:0
 #<!-- -->48 0x00000000072010d0 clang::Sema::BuildOverloadedCallExpr(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x72010d0)
 #<!-- -->49 0x0000000006e6ac1a clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e6ac1a)
 #<!-- -->50 0x0000000006e7009c clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e7009c)
 #<!-- -->51 0x0000000007440803 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCallExpr(clang::CallExpr*) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->52 0x000000000741d667 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->53 0x0000000007464381 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformUnaryOperator(clang::UnaryOperator*) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->54 0x000000000741db5d clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->55 0x00000000074573d7 clang::Sema::SubstConstraintExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x74573d7)
 #<!-- -->56 0x0000000006c3636d (anonymous namespace)::ConstraintSatisfactionChecker::Evaluate(clang::AtomicConstraint const&amp;, clang::MultiLevelTemplateArgumentList const&amp;) SemaConcept.cpp:0:0
 #<!-- -->57 0x0000000006c3ac25 CheckConstraintSatisfaction(clang::Sema&amp;, clang::NamedDecl const*, llvm::ArrayRef&lt;clang::AssociatedConstraint&gt;, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceRange, clang::ConstraintSatisfaction&amp;, clang::Expr**, clang::ConceptReference const*) SemaConcept.cpp:0:0
 #<!-- -->58 0x0000000006c3b3bd clang::Sema::CheckConstraintSatisfaction(llvm::PointerUnion&lt;clang::NamedDecl const*, clang::concepts::NestedRequirement const*&gt;, llvm::ArrayRef&lt;clang::AssociatedConstraint&gt;, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceRange, clang::ConstraintSatisfaction&amp;, clang::ConceptReference const*, clang::Expr**) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c3b3bd)
 #<!-- -->59 0x000000000734d8c1 CheckDeducedArgumentConstraints(clang::Sema&amp;, clang::NamedDecl*, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::sema::TemplateDeductionInfo&amp;) SemaTemplateDeduction.cpp:0:0
 #<!-- -->60 0x000000000736089d FinishTemplateArgumentDeduction(clang::Sema&amp;, clang::NamedDecl*, clang::TemplateParameterList*, clang::TemplateDecl*, bool, llvm::ArrayRef&lt;clang::TemplateArgumentLoc&gt;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, llvm::SmallVectorImpl&lt;clang::DeducedTemplateArgument&gt;&amp;, clang::sema::TemplateDeductionInfo&amp;, bool) SemaTemplateDeduction.cpp:0:0
 #<!-- -->61 0x0000000007360c12 _ZN4llvm12function_refIFvvEE11callback_fnIZ23DeduceTemplateArgumentsIN5clang38ClassTemplatePartialSpecializationDeclEENSt9enable_ifIXsr23IsPartialSpecializationIT_E5valueENS5_23TemplateDeductionResultEE4typeERNS5_4SemaEPS9_NS_8ArrayRefINS5_16TemplateArgumentEEERNS5_4sema21TemplateDeductionInfoEEUlvE_EEvl SemaTemplateDeduction.cpp:0:0
 #<!-- -->62 0x000000000821a121 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x821a121)
 #<!-- -->63 0x000000000736b29a _Z23DeduceTemplateArgumentsIN5clang38ClassTemplatePartialSpecializationDeclEENSt9enable_ifIXsr23IsPartialSpecializationIT_E5valueENS0_23TemplateDeductionResultEE4typeERNS0_4SemaEPS4_N4llvm8ArrayRefINS0_16TemplateArgumentEEERNS0_4sema21TemplateDeductionInfoE SemaTemplateDeduction.cpp:0:0
 #<!-- -->64 0x0000000007415a3d getPatternForClassTemplateSpecialization(clang::Sema&amp;, clang::SourceLocation, clang::ClassTemplateSpecializationDecl*, clang::TemplateSpecializationKind, bool) (.constprop.0) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->65 0x000000000749dd30 clang::Sema::InstantiateClassTemplateSpecialization(clang::SourceLocation, clang::ClassTemplateSpecializationDecl*, clang::TemplateSpecializationKind, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x749dd30)
 #<!-- -->66 0x000000000751e38e void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*)::'lambda'()&gt;(long) SemaType.cpp:0:0
 #<!-- -->67 0x000000000821a121 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x821a121)
 #<!-- -->68 0x0000000007529da3 clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7529da3)
 #<!-- -->69 0x000000000752a23f clang::Sema::RequireCompleteType(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x752a23f)
 #<!-- -->70 0x0000000006b005b3 clang::Sema::RequireCompleteDeclContext(clang::CXXScopeSpec&amp;, clang::DeclContext*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b005b3)
 #<!-- -->71 0x00000000072deb8a clang::Sema::LookupTemplateName(clang::LookupResult&amp;, clang::Scope*, clang::CXXScopeSpec&amp;, clang::QualType, bool, clang::Sema::RequiredTemplateKind, clang::Sema::AssumedTemplateKind*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x72deb8a)
 #<!-- -->72 0x00000000072ff13f clang::Sema::BuildQualifiedTemplateIdExpr(clang::CXXScopeSpec&amp;, clang::SourceLocation, clang::DeclarationNameInfo const&amp;, clang::TemplateArgumentListInfo const*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x72ff13f)
 #<!-- -->73 0x000000000744ad24 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformDependentScopeDeclRefExpr(clang::DependentScopeDeclRefExpr*, bool, clang::TypeSourceInfo**) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->74 0x000000000741d732 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->75 0x000000000745a357 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformStmt(clang::Stmt*, clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::StmtDiscardKind) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->76 0x000000000745aa0e clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->77 0x0000000007463dad clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7463dad)
 #<!-- -->78 0x000000000750007f clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x750007f)
 #<!-- -->79 0x00000000074fdcb6 clang::Sema::PerformPendingInstantiations(bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x74fdcb6)
 #<!-- -->80 0x000000000750042c clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x750042c)
 #<!-- -->81 0x00000000074fdcb6 clang::Sema::PerformPendingInstantiations(bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x74fdcb6)
 #<!-- -->82 0x000000000750042c clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x750042c)
 #<!-- -->83 0x00000000074fdcb6 clang::Sema::PerformPendingInstantiations(bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x74fdcb6)
 #<!-- -->84 0x000000000750042c clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x750042c)
 #<!-- -->85 0x00000000074fdcb6 clang::Sema::PerformPendingInstantiations(bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x74fdcb6)
 #<!-- -->86 0x0000000006a90ad2 clang::Sema::ActOnEndOfTranslationUnitFragment(clang::TUFragmentKind) (.part.0) Sema.cpp:0:0
 #<!-- -->87 0x0000000006a90e52 clang::Sema::ActOnEndOfTranslationUnit() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a90e52)
 #<!-- -->88 0x00000000068f9fd0 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68f9fd0)
 #<!-- -->89 0x00000000068db57a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68db57a)
 #<!-- -->90 0x0000000004b3e668 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b3e668)
 #<!-- -->91 0x0000000004e2bcf5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e2bcf5)
 #<!-- -->92 0x0000000004dacd8e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4dacd8e)
 #<!-- -->93 0x0000000004f2377d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f2377d)
 #<!-- -->94 0x0000000000dc3800 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdc3800)
 #<!-- -->95 0x0000000000dba36a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
 #<!-- -->96 0x0000000000dba4ed int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
 #<!-- -->97 0x0000000004ba7819 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
 #<!-- -->98 0x0000000004137934 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4137934)
 #<!-- -->99 0x0000000004ba7e2f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->100 0x0000000004b69142 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b69142)
#<!-- -->101 0x0000000004b6a0ee clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b6a0ee)
#<!-- -->102 0x0000000004b71825 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b71825)
#<!-- -->103 0x0000000000dbfd29 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdbfd29)
#<!-- -->104 0x0000000000c6fab4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc6fab4)
#<!-- -->105 0x00007b5ab3629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->106 0x00007b5ab3629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->107 0x0000000000db9e05 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb9e05)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

### Comment 2 - shafik

Goes back to clang-12, no one accepts this, so I would say invalid: https://godbolt.org/z/YG8hdxh7W

---

