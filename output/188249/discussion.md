# [clang][mlir] Crash when compiling mlir/IR/BuiltinDialectBytecode.cpp

**Author:** nuudlman
**URL:** https://github.com/llvm/llvm-project/issues/188249
**Status:** Open
**Labels:** clang, crash, needs-reduction

## Body

Crashes with Clang 21, 22. 

Backtrace with non-assertions clang 22:
```
1.      /.../build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:4:86: current parser token ')'
2.      /.../mlir/lib/IR/BuiltinDialectBytecode.cpp:28:1: parsing namespace '(anonymous)'
3.      /.../build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:1:93: parsing function body '(anonymous namespace)::readArrayAttr'
4.      /.../build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:1:93: in compound statement ('{}')
5.      /...build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:3:48: in compound statement ('{}')
6.      /.../mlir/include/mlir/Bytecode/BytecodeImplementation.h:547:6: instantiating function definition 'mlir::getChecked<mlir::ArrayAttr, llvm::SmallVector<mlir::Attribute> &>'
```
clang::ImplicitConversionSequence::getKind() const: Assertion `isInitialized() && "querying uninitialized conversion"` failed.

#186354 seems to have introduced the code that causes this crash.

<details>
<summary>
Assertion failure and backtrace with clang 21
</summary>

```
clang++: /.../llvm-project-llvmorg-21.1.4/clang/include/clang/Sema/Overload.h:716: clang::ImplicitConversionSequence::Kind clang::ImplicitConversionSequence::getKind() const: Assertion `isInitialized() && "querying uninitialized conversion"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /.../clang/21.1.4/bin/clang++ -D_DEBUG -D_GLIBCXX_ASSERTIONS -D_GLIBCXX_USE_CXX11_ABI=1 -D_GNU_SOURCE -D_LIBCPP_HARDENING_MODE=_LIBCPP_HARDENING_MODE_EXTENSIVE -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS -I/.../llvm-project/src/clang_crash/tools/mlir/lib/IR -I/.../llvm-project/src/mlir/lib/IR -I/.../llvm-project/src/clang_crash/tools/mlir/include -I/.../llvm-project/src/mlir/include -I/.../llvm-project/src/clang_crash/include -I/.../llvm-project/src/llvm/include -fPIC -fno-semantic-interposition -fvisibility-inlines-hidden -Werror=date-time -Werror=unguarded-availability-new -Wall -Wextra -Wno-unused-parameter -Wwrite-strings -Wcast-qual -Wmissing-field-initializers -pedantic -Wno-long-long -Wc++98-compat-extra-semi -Wimplicit-fallthrough -Wcovered-switch-default -Wno-noexcept-type -Wnon-virtual-dtor -Wdelete-non-virtual-dtor -Wsuggest-override -Wstring-conversion -Wno-pass-failed -Wmisleading-indentation -Wctad-maybe-unsupported -fdiagnostics-color -Xclang -fno-pch-timestamp -Wundef -Werror=mismatched-tags -Werror=global-constructors -g -std=c++17 -gsplit-dwarf -fno-exceptions -funwind-tables -fno-rtti -Winvalid-pch -Xclang -include-pch -Xclang /.../llvm-project/src/clang_crash/lib/Support/CMakeFiles/LLVMSupport.dir/cmake_pch.hxx.pch -Xclang -include -Xclang /.../llvm-project/src/clang_crash/lib/Support/CMakeFiles/LLVMSupport.dir/cmake_pch.hxx -MD -MT tools/mlir/lib/IR/CMakeFiles/obj.MLIRIR.dir/BuiltinDialectBytecode.cpp.o -MF tools/mlir/lib/IR/CMakeFiles/obj.MLIRIR.dir/BuiltinDialectBytecode.cpp.o.d -o tools/mlir/lib/IR/CMakeFiles/obj.MLIRIR.dir/BuiltinDialectBytecode.cpp.o -c /.../llvm-project/src/mlir/lib/IR/BuiltinDialectBytecode.cpp
1.      /.../llvm-project/src/clang_crash/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:4:86: current parser token ')'
2.      /.../llvm-project/src/mlir/lib/IR/BuiltinDialectBytecode.cpp:28:1: parsing namespace '(anonymous)'
3.      /.../llvm-project/src/clang_crash/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:1:93: parsing function body '(anonymous namespace)::readArrayAttr'
4.      /.../llvm-project/src/clang_crash/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:1:93: in compound statement ('{}')
5.      /.../llvm-project/src/clang_crash/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:3:48: in compound statement ('{}')
6.      /.../llvm-project/src/mlir/include/mlir/Bytecode/BytecodeImplementation.h:547:6: instantiating function definition 'mlir::getChecked<mlir::ArrayAttr, llvm::SmallVector<mlir::Attribute> &>'
  #0 0x00000000022d498e llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/.../clang/21.1.4/bin/clang+++0x22d498e)
  #1 0x00000000022d176b llvm::sys::RunSignalHandlers() (/.../clang/21.1.4/bin/clang+++0x22d176b)
  #2 0x000000000220e228 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
  #3 0x00007f4dd34da990 __restore_rt (/lib64/libpthread.so.0+0x12990)
  #4 0x00007f4dd22925af raise (/lib64/libc.so.6+0x4e5af)
  #5 0x00007f4dd2265ee5 abort (/lib64/libc.so.6+0x21ee5)
  #6 0x00007f4dd2265db9 _nl_load_domain.cold.0 (/lib64/libc.so.6+0x21db9)
  #7 0x00007f4dd228af06 (/lib64/libc.so.6+0x46f06)
  #8 0x00000000050e75fc (anonymous namespace)::CompareOverloadCandidatesForDisplay::CompareConversions(clang::OverloadCandidate const&, clang::OverloadCandidate const&) (.isra.0) SemaOverload.cpp:0:0
  #9 0x00000000050eeaab (anonymous namespace)::CompareOverloadCandidatesForDisplay::operator()(clang::OverloadCandidate const*, clang::OverloadCandidate const*) SemaOverload.cpp:0:0
 #10 0x00000000050ef04f void std::__merge_adaptive<clang::OverloadCandidate**, long, clang::OverloadCandidate**, __gnu_cxx::__ops::_Iter_comp_iter<(anonymous namespace)::CompareOverloadCandidatesForDisplay>>(clang::OverloadCandidate**, clang::OverloadCandidate**, clang::OverloadCandidate**, long, long, clang::OverloadCandidate**, __gnu_cxx::__ops::_Iter_comp_iter<(anonymous namespace)::CompareOverloadCandidatesForDisplay>) (.constprop.0) SemaOverload.cpp:0:0
 #11 0x0000000005129211 clang::OverloadCandidateSet::CompleteCandidates(clang::Sema&, clang::OverloadCandidateDisplayKind, llvm::ArrayRef<clang::Expr*>, clang::SourceLocation, llvm::function_ref<bool (clang::OverloadCandidate&)>) (/.../clang/21.1.4/bin/clang+++0x5129211)
 #12 0x000000000512bb5e clang::OverloadCandidateSet::NoteCandidates(std::pair<clang::SourceLocation, clang::PartialDiagnostic>, clang::Sema&, clang::OverloadCandidateDisplayKind, llvm::ArrayRef<clang::Expr*>, llvm::StringRef, clang::SourceLocation, llvm::function_ref<bool (clang::OverloadCandidate&)>) (/.../clang/21.1.4/bin/clang+++0x512bb5e)
 #13 0x000000000512cd24 FinishOverloadedCallExpr(clang::Sema&, clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, clang::OverloadCandidateSet*, clang::OverloadCandidate**, clang::OverloadingResult, bool) SemaOverload.cpp:0:0
 #14 0x000000000512df60 clang::Sema::BuildOverloadedCallExpr(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/.../clang/21.1.4/bin/clang+++0x512df60)
 #15 0x0000000004d58608 clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/.../clang/21.1.4/bin/clang+++0x4d58608)
 #16 0x0000000004d5a0ad clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) (/.../clang/21.1.4/bin/clang+++0x4d5a0ad)
 #17 0x00000000053a711d clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCallExpr(clang::CallExpr*) SemaTemplateInstantiate.cpp:0:0
 #18 0x000000000538b4d0 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
 #19 0x0000000005390a0a clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeSourceInfo*) (.constprop.0) SemaTemplateInstantiate.cpp:0:0
 #20 0x00000000053927da clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::QualType) SemaTemplateInstantiate.cpp:0:0
 #21 0x0000000005392e86 clang::Sema::SubstType(clang::QualType, clang::MultiLevelTemplateArgumentList const&, clang::SourceLocation, clang::DeclarationName, bool*) (/.../clang/21.1.4/bin/clang+++0x5392e86)
 #22 0x000000000522d68c clang::Sema::CheckTemplateIdType(clang::TemplateName, clang::SourceLocation, clang::TemplateArgumentListInfo&) (/.../clang/21.1.4/bin/clang+++0x522d68c)
 #23 0x00000000053b2f27 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformTemplateSpecializationType(clang::TypeLocBuilder&, clang::TemplateSpecializationTypeLoc, clang::TemplateName) SemaTemplateInstantiate.cpp:0:0
 #24 0x000000000538c1a7 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
 #25 0x00000000053b9a45 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformElaboratedType(clang::TypeLocBuilder&, clang::ElaboratedTypeLoc) SemaTemplateInstantiate.cpp:0:0
 #26 0x000000000538a930 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
 #27 0x0000000005390a0a clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeSourceInfo*) (.constprop.0) SemaTemplateInstantiate.cpp:0:0
 #28 0x0000000005398796 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformTemplateArgument(clang::TemplateArgumentLoc const&, clang::TemplateArgumentLoc&, bool) SemaTemplateInstantiate.cpp:0:0
 #29 0x00000000053b310b clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformTemplateSpecializationType(clang::TypeLocBuilder&, clang::TemplateSpecializationTypeLoc, clang::TemplateName) SemaTemplateInstantiate.cpp:0:0
 #30 0x000000000538c1a7 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
 #31 0x00000000053b9a45 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformElaboratedType(clang::TypeLocBuilder&, clang::ElaboratedTypeLoc) SemaTemplateInstantiate.cpp:0:0
 #32 0x000000000538a930 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
 #33 0x0000000005390a0a clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeSourceInfo*) (.constprop.0) SemaTemplateInstantiate.cpp:0:0
 #34 0x0000000005398796 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformTemplateArgument(clang::TemplateArgumentLoc const&, clang::TemplateArgumentLoc&, bool) SemaTemplateInstantiate.cpp:0:0
 #35 0x0000000005399f52 bool clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformTemplateArguments<clang::TemplateArgumentLoc const*>(clang::TemplateArgumentLoc const*, clang::TemplateArgumentLoc const*, clang::TemplateArgumentListInfo&, bool) (.constprop.0) SemaTemplateInstantiate.cpp:0:0
 #36 0x000000000539a8c3 clang::Sema::SubstTemplateArguments(llvm::ArrayRef<clang::TemplateArgumentLoc>, clang::MultiLevelTemplateArgumentList const&, clang::TemplateArgumentListInfo&) (/.../clang/21.1.4/bin/clang+++0x539a8c3)
 #37 0x00000000052cf1dc FinishTemplateArgumentDeduction(clang::Sema&, clang::NamedDecl*, clang::TemplateParameterList*, clang::TemplateDecl*, bool, llvm::ArrayRef<clang::TemplateArgumentLoc>, llvm::ArrayRef<clang::TemplateArgument>, llvm::SmallVectorImpl<clang::DeducedTemplateArgument>&, clang::sema::TemplateDeductionInfo&, bool) SemaTemplateDeduction.cpp:0:0
 #38 0x00000000052cfecf _ZN4llvm12function_refIFvvEE11callback_fnIZ23DeduceTemplateArgumentsIN5clang38ClassTemplatePartialSpecializationDeclEENSt9enable_ifIXsr23IsPartialSpecializationIT_E5valueENS5_23TemplateDeductionResultEE4typeERNS5_4SemaEPS9_NS_8ArrayRefINS5_16TemplateArgumentEEERNS5_4sema21TemplateDeductionInfoEEUlvE_EEvl SemaTemplateDeduction.cpp:0:0
 #39 0x000000000617ad9f clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/.../clang/21.1.4/bin/clang+++0x617ad9f)
 #40 0x00000000052c08eb _Z23DeduceTemplateArgumentsIN5clang38ClassTemplatePartialSpecializationDeclEENSt9enable_ifIXsr23IsPartialSpecializationIT_E5valueENS0_23TemplateDeductionResultEE4typeERNS0_4SemaEPS4_N4llvm8ArrayRefINS0_16TemplateArgumentEEERNS0_4sema21TemplateDeductionInfoE SemaTemplateDeduction.cpp:0:0
 #41 0x00000000053782bd getPatternForClassTemplateSpecialization(clang::Sema&, clang::SourceLocation, clang::ClassTemplateSpecializationDecl*, clang::TemplateSpecializationKind, bool) (.isra.0) SemaTemplateInstantiate.cpp:0:0
 #42 0x00000000053cd3fa clang::Sema::InstantiateClassTemplateSpecialization(clang::SourceLocation, clang::ClassTemplateSpecializationDecl*, clang::TemplateSpecializationKind, bool, bool) (/.../clang/21.1.4/bin/clang+++0x53cd3fa)
 #43 0x000000000545031e void llvm::function_ref<void ()>::callback_fn<clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*)::'lambda'()>(long) SemaType.cpp:0:0
 #44 0x000000000617ad9f clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/.../clang/21.1.4/bin/clang+++0x617ad9f)
 #45 0x000000000545c24c clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*) (/.../clang/21.1.4/bin/clang+++0x545c24c)
 #46 0x000000000545c795 clang::Sema::RequireCompleteType(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser&) (/.../clang/21.1.4/bin/clang+++0x545c795)
 #47 0x0000000004a4b9e8 clang::Sema::RequireCompleteDeclContext(clang::CXXScopeSpec&, clang::DeclContext*) (/.../clang/21.1.4/bin/clang+++0x4a4b9e8)
 #48 0x0000000005233a47 clang::Sema::CheckTypenameType(clang::ElaboratedTypeKeyword, clang::SourceLocation, clang::NestedNameSpecifierLoc, clang::IdentifierInfo const&, clang::SourceLocation, bool) (/.../clang/21.1.4/bin/clang+++0x5233a47)
 #49 0x000000000537a9bc clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::RebuildDependentNameType(clang::ElaboratedTypeKeyword, clang::SourceLocation, clang::NestedNameSpecifierLoc, clang::IdentifierInfo const*, clang::SourceLocation, bool) SemaTemplateInstantiate.cpp:0:0
 #50 0x00000000053959aa clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformDependentNameType(clang::TypeLocBuilder&, clang::DependentNameTypeLoc, bool) SemaTemplateInstantiate.cpp:0:0
 #51 0x000000000538b7a2 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
 #52 0x0000000005390a0a clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeSourceInfo*) (.constprop.0) SemaTemplateInstantiate.cpp:0:0
 #53 0x00000000053927da clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::QualType) SemaTemplateInstantiate.cpp:0:0
 #54 0x0000000005392e86 clang::Sema::SubstType(clang::QualType, clang::MultiLevelTemplateArgumentList const&, clang::SourceLocation, clang::DeclarationName, bool*) (/.../clang/21.1.4/bin/clang+++0x5392e86)
 #55 0x000000000522d68c clang::Sema::CheckTemplateIdType(clang::TemplateName, clang::SourceLocation, clang::TemplateArgumentListInfo&) (/.../clang/21.1.4/bin/clang+++0x522d68c)
 #56 0x00000000053b2f27 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformTemplateSpecializationType(clang::TypeLocBuilder&, clang::TemplateSpecializationTypeLoc, clang::TemplateName) SemaTemplateInstantiate.cpp:0:0
 #57 0x0000000005394487 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformTSIInObjectScope(clang::TypeLoc, clang::QualType, clang::NamedDecl*, clang::CXXScopeSpec&) (.isra.0) SemaTemplateInstantiate.cpp:0:0
 #58 0x0000000005395268 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformNestedNameSpecifierLoc(clang::NestedNameSpecifierLoc, clang::QualType, clang::NamedDecl*) SemaTemplateInstantiate.cpp:0:0
 #59 0x00000000053b42e0 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformDependentScopeDeclRefExpr(clang::DependentScopeDeclRefExpr*, bool, clang::TypeSourceInfo**) SemaTemplateInstantiate.cpp:0:0
 #60 0x000000000537ea10 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCondition(clang::SourceLocation, clang::VarDecl*, clang::Expr*, clang::Sema::ConditionKind) SemaTemplateInstantiate.cpp:0:0
 #61 0x00000000053ca561 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformIfStmt(clang::IfStmt*) SemaTemplateInstantiate.cpp:0:0
 #62 0x00000000053c47ea clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaTemplateInstantiate.cpp:0:0
 #63 0x00000000053ccd8f clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&) (/.../clang/21.1.4/bin/clang+++0x53ccd8f)
 #64 0x0000000005432319 clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/.../clang/21.1.4/bin/clang+++0x5432319)
 #65 0x000000000617ad9f clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/.../clang/21.1.4/bin/clang+++0x617ad9f)
 #66 0x00000000052ca08b clang::Sema::DeduceReturnType(clang::FunctionDecl*, clang::SourceLocation, bool) (/.../clang/21.1.4/bin/clang+++0x52ca08b)
 #67 0x0000000004d461d8 clang::Sema::DiagnoseUseOfDecl(clang::NamedDecl*, llvm::ArrayRef<clang::SourceLocation>, clang::ObjCInterfaceDecl const*, bool, bool, clang::ObjCInterfaceDecl*, bool) (/.../clang/21.1.4/bin/clang+++0x4d461d8)
 #68 0x000000000512d19b FinishOverloadedCallExpr(clang::Sema&, clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, clang::OverloadCandidateSet*, clang::OverloadCandidate**, clang::OverloadingResult, bool) SemaOverload.cpp:0:0
 #69 0x000000000512df60 clang::Sema::BuildOverloadedCallExpr(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/.../clang/21.1.4/bin/clang+++0x512df60)
 #70 0x0000000004d58608 clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/.../clang/21.1.4/bin/clang+++0x4d58608)
 #71 0x0000000004d5a0ad clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) (/.../clang/21.1.4/bin/clang+++0x4d5a0ad)
 #72 0x00000000048b35e7 clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult<clang::Expr*, true>) (/.../clang/21.1.4/bin/clang+++0x48b35e7)
 #73 0x00000000048ac16a clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypoCorrectionTypeBehavior, bool, bool*) (/.../clang/21.1.4/bin/clang+++0x48ac16a)
 #74 0x00000000048ac379 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypoCorrectionTypeBehavior, bool, bool*) (/.../clang/21.1.4/bin/clang+++0x48ac379)
 #75 0x00000000048ae7a6 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/.../clang/21.1.4/bin/clang+++0x48ae7a6)
 #76 0x00000000048ae838 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/.../clang/21.1.4/bin/clang+++0x48ae838)
 #77 0x00000000048b2819 clang::Parser::ParseExpression(clang::TypoCorrectionTypeBehavior) (/.../clang/21.1.4/bin/clang+++0x48b2819)
 #78 0x000000000493f100 clang::Parser::ParseReturnStatement() (/.../clang/21.1.4/bin/clang+++0x493f100)
 #79 0x00000000049440e3 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/.../clang/21.1.4/bin/clang+++0x49440e3)
 #80 0x000000000494521c clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/.../clang/21.1.4/bin/clang+++0x494521c)
 #81 0x00000000049466c3 clang::Parser::ParseCompoundStatementBody(bool) (/.../clang/21.1.4/bin/clang+++0x49466c3)
 #82 0x0000000004946ff4 void llvm::function_ref<void ()>::callback_fn<clang::Parser::ParseCompoundStatement(bool, unsigned int)::'lambda'()>(long) ParseStmt.cpp:0:0
 #83 0x000000000617ad9f clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/.../clang/21.1.4/bin/clang+++0x617ad9f)
 #84 0x000000000493d7e6 clang::Parser::ParseCompoundStatement(bool, unsigned int) (/.../clang/21.1.4/bin/clang+++0x493d7e6)
 #85 0x00000000049441cf clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/.../clang/21.1.4/bin/clang+++0x49441cf)
 #86 0x000000000494521c clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/.../clang/21.1.4/bin/clang+++0x494521c)
 #87 0x0000000004947050 clang::Parser::ParseStatement(clang::SourceLocation*, clang::Parser::ParsedStmtContext) (/.../clang/21.1.4/bin/clang+++0x4947050)
 #88 0x0000000004942e12 clang::Parser::ParseIfStatement(clang::SourceLocation*) (/.../clang/21.1.4/bin/clang+++0x4942e12)
 #89 0x00000000049440cf clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/.../clang/21.1.4/bin/clang+++0x49440cf)
 #90 0x000000000494521c clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/.../clang/21.1.4/bin/clang+++0x494521c)
 #91 0x00000000049466c3 clang::Parser::ParseCompoundStatementBody(bool) (/.../clang/21.1.4/bin/clang+++0x49466c3)
 #92 0x0000000004946ee7 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/.../clang/21.1.4/bin/clang+++0x4946ee7)
 #93 0x000000000483357a clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/.../clang/21.1.4/bin/clang+++0x483357a)
 #94 0x000000000487b929 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/.../clang/21.1.4/bin/clang+++0x487b929)
 #95 0x000000000482e029 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/.../clang/21.1.4/bin/clang+++0x482e029)
 #96 0x000000000482e7e8 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/.../clang/21.1.4/bin/clang+++0x482e7e8)
 #97 0x0000000004836c58 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/.../clang/21.1.4/bin/clang+++0x4836c58)
 #98 0x00000000048989c5 clang::Parser::ParseInnerNamespace(llvm::SmallVector<clang::Parser::InnerNamespaceInfo, 4u> const&, unsigned int, clang::SourceLocation&, clang::ParsedAttributes&, clang::BalancedDelimiterTracker&) (/.../clang/21.1.4/bin/clang+++0x48989c5)
 #99 0x000000000489959b clang::Parser::ParseNamespace(clang::DeclaratorContext, clang::SourceLocation&, clang::SourceLocation) (/.../clang/21.1.4/bin/clang+++0x489959b)
#100 0x000000000487cf0d clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/.../clang/21.1.4/bin/clang+++0x487cf0d)
#101 0x0000000004836e12 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/.../clang/21.1.4/bin/clang+++0x4836e12)
#102 0x0000000004837cf5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/.../clang/21.1.4/bin/clang+++0x4837cf5)
#103 0x00000000048267aa clang::ParseAST(clang::Sema&, bool, bool) (/.../clang/21.1.4/bin/clang+++0x48267aa)
#104 0x0000000002f41803 clang::FrontendAction::Execute() (/.../clang/21.1.4/bin/clang+++0x2f41803)
#105 0x0000000002ec16bd clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/.../clang/21.1.4/bin/clang+++0x2ec16bd)
#106 0x0000000003033feb clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/.../clang/21.1.4/bin/clang+++0x3033feb)
#107 0x0000000000c8f1c4 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/.../clang/21.1.4/bin/clang+++0xc8f1c4)
#108 0x0000000000c84967 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#109 0x0000000002c66889 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#110 0x000000000220e6f2 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/.../clang/21.1.4/bin/clang+++0x220e6f2)
#111 0x0000000002c67970 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#112 0x0000000002c24b65 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/.../clang/21.1.4/bin/clang+++0x2c24b65)
#113 0x0000000002c25cb2 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/.../clang/21.1.4/bin/clang+++0x2c25cb2)
#114 0x0000000002c32f74 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/.../clang/21.1.4/bin/clang+++0x2c32f74)
#115 0x0000000000c8a259 clang_main(int, char**, llvm::ToolContext const&) (/.../clang/21.1.4/bin/clang+++0xc8a259)
#116 0x0000000000b898f3 main (/.../clang/21.1.4/bin/clang+++0xb898f3)
#117 0x00007f4dd227e865 __libc_start_main (/lib64/libc.so.6+0x3a865)
#118 0x0000000000c83e6e _start (/.../clang/21.1.4/bin/clang+++0xc83e6e)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
clang version 21.1.4
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /.../clang/21.1.4/bin
Build config: +assertions
clang++: note: diagnostic msg:
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: /tmp/BuiltinDialectBytecode-0f1662.cpp
clang++: note: diagnostic msg: /tmp/BuiltinDialectBytecode-0f1662.sh
clang++: note: diagnostic msg:

********************
```

</details>

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: Isaac Nudelman (nuudlman)

<details>
Crashes with Clang 21, 22. 

Backtrace with non-assertions clang 22:
```
1.      /.../build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:4:86: current parser token ')'
2.      /.../mlir/lib/IR/BuiltinDialectBytecode.cpp:28:1: parsing namespace '(anonymous)'
3.      /.../build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:1:93: parsing function body '(anonymous namespace)::readArrayAttr'
4.      /.../build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:1:93: in compound statement ('{}')
5.      /...build/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:3:48: in compound statement ('{}')
6.      /.../mlir/include/mlir/Bytecode/BytecodeImplementation.h:547:6: instantiating function definition 'mlir::getChecked&lt;mlir::ArrayAttr, llvm::SmallVector&lt;mlir::Attribute&gt; &amp;&gt;'
```
clang::ImplicitConversionSequence::getKind() const: Assertion `isInitialized() &amp;&amp; "querying uninitialized conversion"` failed.

#<!-- -->186354 seems to have introduced the code that causes this crash.

&lt;details&gt;
&lt;summary&gt;
Assertion failure and backtrace with clang 21
&lt;/summary&gt;

```
clang++: /.../llvm-project-llvmorg-21.1.4/clang/include/clang/Sema/Overload.h:716: clang::ImplicitConversionSequence::Kind clang::ImplicitConversionSequence::getKind() const: Assertion `isInitialized() &amp;&amp; "querying uninitialized conversion"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /.../clang/21.1.4/bin/clang++ -D_DEBUG -D_GLIBCXX_ASSERTIONS -D_GLIBCXX_USE_CXX11_ABI=1 -D_GNU_SOURCE -D_LIBCPP_HARDENING_MODE=_LIBCPP_HARDENING_MODE_EXTENSIVE -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS -I/.../llvm-project/src/clang_crash/tools/mlir/lib/IR -I/.../llvm-project/src/mlir/lib/IR -I/.../llvm-project/src/clang_crash/tools/mlir/include -I/.../llvm-project/src/mlir/include -I/.../llvm-project/src/clang_crash/include -I/.../llvm-project/src/llvm/include -fPIC -fno-semantic-interposition -fvisibility-inlines-hidden -Werror=date-time -Werror=unguarded-availability-new -Wall -Wextra -Wno-unused-parameter -Wwrite-strings -Wcast-qual -Wmissing-field-initializers -pedantic -Wno-long-long -Wc++98-compat-extra-semi -Wimplicit-fallthrough -Wcovered-switch-default -Wno-noexcept-type -Wnon-virtual-dtor -Wdelete-non-virtual-dtor -Wsuggest-override -Wstring-conversion -Wno-pass-failed -Wmisleading-indentation -Wctad-maybe-unsupported -fdiagnostics-color -Xclang -fno-pch-timestamp -Wundef -Werror=mismatched-tags -Werror=global-constructors -g -std=c++17 -gsplit-dwarf -fno-exceptions -funwind-tables -fno-rtti -Winvalid-pch -Xclang -include-pch -Xclang /.../llvm-project/src/clang_crash/lib/Support/CMakeFiles/LLVMSupport.dir/cmake_pch.hxx.pch -Xclang -include -Xclang /.../llvm-project/src/clang_crash/lib/Support/CMakeFiles/LLVMSupport.dir/cmake_pch.hxx -MD -MT tools/mlir/lib/IR/CMakeFiles/obj.MLIRIR.dir/BuiltinDialectBytecode.cpp.o -MF tools/mlir/lib/IR/CMakeFiles/obj.MLIRIR.dir/BuiltinDialectBytecode.cpp.o.d -o tools/mlir/lib/IR/CMakeFiles/obj.MLIRIR.dir/BuiltinDialectBytecode.cpp.o -c /.../llvm-project/src/mlir/lib/IR/BuiltinDialectBytecode.cpp
1.      /.../llvm-project/src/clang_crash/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:4:86: current parser token ')'
2.      /.../llvm-project/src/mlir/lib/IR/BuiltinDialectBytecode.cpp:28:1: parsing namespace '(anonymous)'
3.      /.../llvm-project/src/clang_crash/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:1:93: parsing function body '(anonymous namespace)::readArrayAttr'
4.      /.../llvm-project/src/clang_crash/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:1:93: in compound statement ('{}')
5.      /.../llvm-project/src/clang_crash/tools/mlir/include/mlir/IR/BuiltinDialectBytecode.cpp.inc:3:48: in compound statement ('{}')
6.      /.../llvm-project/src/mlir/include/mlir/Bytecode/BytecodeImplementation.h:547:6: instantiating function definition 'mlir::getChecked&lt;mlir::ArrayAttr, llvm::SmallVector&lt;mlir::Attribute&gt; &amp;&gt;'
  #<!-- -->0 0x00000000022d498e llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/.../clang/21.1.4/bin/clang+++0x22d498e)
  #<!-- -->1 0x00000000022d176b llvm::sys::RunSignalHandlers() (/.../clang/21.1.4/bin/clang+++0x22d176b)
  #<!-- -->2 0x000000000220e228 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
  #<!-- -->3 0x00007f4dd34da990 __restore_rt (/lib64/libpthread.so.0+0x12990)
  #<!-- -->4 0x00007f4dd22925af raise (/lib64/libc.so.6+0x4e5af)
  #<!-- -->5 0x00007f4dd2265ee5 abort (/lib64/libc.so.6+0x21ee5)
  #<!-- -->6 0x00007f4dd2265db9 _nl_load_domain.cold.0 (/lib64/libc.so.6+0x21db9)
  #<!-- -->7 0x00007f4dd228af06 (/lib64/libc.so.6+0x46f06)
  #<!-- -->8 0x00000000050e75fc (anonymous namespace)::CompareOverloadCandidatesForDisplay::CompareConversions(clang::OverloadCandidate const&amp;, clang::OverloadCandidate const&amp;) (.isra.0) SemaOverload.cpp:0:0
  #<!-- -->9 0x00000000050eeaab (anonymous namespace)::CompareOverloadCandidatesForDisplay::operator()(clang::OverloadCandidate const*, clang::OverloadCandidate const*) SemaOverload.cpp:0:0
 #<!-- -->10 0x00000000050ef04f void std::__merge_adaptive&lt;clang::OverloadCandidate**, long, clang::OverloadCandidate**, __gnu_cxx::__ops::_Iter_comp_iter&lt;(anonymous namespace)::CompareOverloadCandidatesForDisplay&gt;&gt;(clang::OverloadCandidate**, clang::OverloadCandidate**, clang::OverloadCandidate**, long, long, clang::OverloadCandidate**, __gnu_cxx::__ops::_Iter_comp_iter&lt;(anonymous namespace)::CompareOverloadCandidatesForDisplay&gt;) (.constprop.0) SemaOverload.cpp:0:0
 #<!-- -->11 0x0000000005129211 clang::OverloadCandidateSet::CompleteCandidates(clang::Sema&amp;, clang::OverloadCandidateDisplayKind, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, llvm::function_ref&lt;bool (clang::OverloadCandidate&amp;)&gt;) (/.../clang/21.1.4/bin/clang+++0x5129211)
 #<!-- -->12 0x000000000512bb5e clang::OverloadCandidateSet::NoteCandidates(std::pair&lt;clang::SourceLocation, clang::PartialDiagnostic&gt;, clang::Sema&amp;, clang::OverloadCandidateDisplayKind, llvm::ArrayRef&lt;clang::Expr*&gt;, llvm::StringRef, clang::SourceLocation, llvm::function_ref&lt;bool (clang::OverloadCandidate&amp;)&gt;) (/.../clang/21.1.4/bin/clang+++0x512bb5e)
 #<!-- -->13 0x000000000512cd24 FinishOverloadedCallExpr(clang::Sema&amp;, clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, clang::OverloadCandidateSet*, clang::OverloadCandidate**, clang::OverloadingResult, bool) SemaOverload.cpp:0:0
 #<!-- -->14 0x000000000512df60 clang::Sema::BuildOverloadedCallExpr(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, bool) (/.../clang/21.1.4/bin/clang+++0x512df60)
 #<!-- -->15 0x0000000004d58608 clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, bool) (/.../clang/21.1.4/bin/clang+++0x4d58608)
 #<!-- -->16 0x0000000004d5a0ad clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*) (/.../clang/21.1.4/bin/clang+++0x4d5a0ad)
 #<!-- -->17 0x00000000053a711d clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCallExpr(clang::CallExpr*) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->18 0x000000000538b4d0 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeLocBuilder&amp;, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->19 0x0000000005390a0a clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeSourceInfo*) (.constprop.0) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->20 0x00000000053927da clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::QualType) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->21 0x0000000005392e86 clang::Sema::SubstType(clang::QualType, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceLocation, clang::DeclarationName, bool*) (/.../clang/21.1.4/bin/clang+++0x5392e86)
 #<!-- -->22 0x000000000522d68c clang::Sema::CheckTemplateIdType(clang::TemplateName, clang::SourceLocation, clang::TemplateArgumentListInfo&amp;) (/.../clang/21.1.4/bin/clang+++0x522d68c)
 #<!-- -->23 0x00000000053b2f27 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformTemplateSpecializationType(clang::TypeLocBuilder&amp;, clang::TemplateSpecializationTypeLoc, clang::TemplateName) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->24 0x000000000538c1a7 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeLocBuilder&amp;, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->25 0x00000000053b9a45 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformElaboratedType(clang::TypeLocBuilder&amp;, clang::ElaboratedTypeLoc) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->26 0x000000000538a930 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeLocBuilder&amp;, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->27 0x0000000005390a0a clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeSourceInfo*) (.constprop.0) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->28 0x0000000005398796 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformTemplateArgument(clang::TemplateArgumentLoc const&amp;, clang::TemplateArgumentLoc&amp;, bool) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->29 0x00000000053b310b clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformTemplateSpecializationType(clang::TypeLocBuilder&amp;, clang::TemplateSpecializationTypeLoc, clang::TemplateName) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->30 0x000000000538c1a7 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeLocBuilder&amp;, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->31 0x00000000053b9a45 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformElaboratedType(clang::TypeLocBuilder&amp;, clang::ElaboratedTypeLoc) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->32 0x000000000538a930 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeLocBuilder&amp;, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->33 0x0000000005390a0a clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeSourceInfo*) (.constprop.0) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->34 0x0000000005398796 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformTemplateArgument(clang::TemplateArgumentLoc const&amp;, clang::TemplateArgumentLoc&amp;, bool) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->35 0x0000000005399f52 bool clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformTemplateArguments&lt;clang::TemplateArgumentLoc const*&gt;(clang::TemplateArgumentLoc const*, clang::TemplateArgumentLoc const*, clang::TemplateArgumentListInfo&amp;, bool) (.constprop.0) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->36 0x000000000539a8c3 clang::Sema::SubstTemplateArguments(llvm::ArrayRef&lt;clang::TemplateArgumentLoc&gt;, clang::MultiLevelTemplateArgumentList const&amp;, clang::TemplateArgumentListInfo&amp;) (/.../clang/21.1.4/bin/clang+++0x539a8c3)
 #<!-- -->37 0x00000000052cf1dc FinishTemplateArgumentDeduction(clang::Sema&amp;, clang::NamedDecl*, clang::TemplateParameterList*, clang::TemplateDecl*, bool, llvm::ArrayRef&lt;clang::TemplateArgumentLoc&gt;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, llvm::SmallVectorImpl&lt;clang::DeducedTemplateArgument&gt;&amp;, clang::sema::TemplateDeductionInfo&amp;, bool) SemaTemplateDeduction.cpp:0:0
 #<!-- -->38 0x00000000052cfecf _ZN4llvm12function_refIFvvEE11callback_fnIZ23DeduceTemplateArgumentsIN5clang38ClassTemplatePartialSpecializationDeclEENSt9enable_ifIXsr23IsPartialSpecializationIT_E5valueENS5_23TemplateDeductionResultEE4typeERNS5_4SemaEPS9_NS_8ArrayRefINS5_16TemplateArgumentEEERNS5_4sema21TemplateDeductionInfoEEUlvE_EEvl SemaTemplateDeduction.cpp:0:0
 #<!-- -->39 0x000000000617ad9f clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/.../clang/21.1.4/bin/clang+++0x617ad9f)
 #<!-- -->40 0x00000000052c08eb _Z23DeduceTemplateArgumentsIN5clang38ClassTemplatePartialSpecializationDeclEENSt9enable_ifIXsr23IsPartialSpecializationIT_E5valueENS0_23TemplateDeductionResultEE4typeERNS0_4SemaEPS4_N4llvm8ArrayRefINS0_16TemplateArgumentEEERNS0_4sema21TemplateDeductionInfoE SemaTemplateDeduction.cpp:0:0
 #<!-- -->41 0x00000000053782bd getPatternForClassTemplateSpecialization(clang::Sema&amp;, clang::SourceLocation, clang::ClassTemplateSpecializationDecl*, clang::TemplateSpecializationKind, bool) (.isra.0) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->42 0x00000000053cd3fa clang::Sema::InstantiateClassTemplateSpecialization(clang::SourceLocation, clang::ClassTemplateSpecializationDecl*, clang::TemplateSpecializationKind, bool, bool) (/.../clang/21.1.4/bin/clang+++0x53cd3fa)
 #<!-- -->43 0x000000000545031e void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*)::'lambda'()&gt;(long) SemaType.cpp:0:0
 #<!-- -->44 0x000000000617ad9f clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/.../clang/21.1.4/bin/clang+++0x617ad9f)
 #<!-- -->45 0x000000000545c24c clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*) (/.../clang/21.1.4/bin/clang+++0x545c24c)
 #<!-- -->46 0x000000000545c795 clang::Sema::RequireCompleteType(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser&amp;) (/.../clang/21.1.4/bin/clang+++0x545c795)
 #<!-- -->47 0x0000000004a4b9e8 clang::Sema::RequireCompleteDeclContext(clang::CXXScopeSpec&amp;, clang::DeclContext*) (/.../clang/21.1.4/bin/clang+++0x4a4b9e8)
 #<!-- -->48 0x0000000005233a47 clang::Sema::CheckTypenameType(clang::ElaboratedTypeKeyword, clang::SourceLocation, clang::NestedNameSpecifierLoc, clang::IdentifierInfo const&amp;, clang::SourceLocation, bool) (/.../clang/21.1.4/bin/clang+++0x5233a47)
 #<!-- -->49 0x000000000537a9bc clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::RebuildDependentNameType(clang::ElaboratedTypeKeyword, clang::SourceLocation, clang::NestedNameSpecifierLoc, clang::IdentifierInfo const*, clang::SourceLocation, bool) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->50 0x00000000053959aa clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformDependentNameType(clang::TypeLocBuilder&amp;, clang::DependentNameTypeLoc, bool) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->51 0x000000000538b7a2 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeLocBuilder&amp;, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->52 0x0000000005390a0a clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeSourceInfo*) (.constprop.0) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->53 0x00000000053927da clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::QualType) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->54 0x0000000005392e86 clang::Sema::SubstType(clang::QualType, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceLocation, clang::DeclarationName, bool*) (/.../clang/21.1.4/bin/clang+++0x5392e86)
 #<!-- -->55 0x000000000522d68c clang::Sema::CheckTemplateIdType(clang::TemplateName, clang::SourceLocation, clang::TemplateArgumentListInfo&amp;) (/.../clang/21.1.4/bin/clang+++0x522d68c)
 #<!-- -->56 0x00000000053b2f27 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformTemplateSpecializationType(clang::TypeLocBuilder&amp;, clang::TemplateSpecializationTypeLoc, clang::TemplateName) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->57 0x0000000005394487 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformTSIInObjectScope(clang::TypeLoc, clang::QualType, clang::NamedDecl*, clang::CXXScopeSpec&amp;) (.isra.0) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->58 0x0000000005395268 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformNestedNameSpecifierLoc(clang::NestedNameSpecifierLoc, clang::QualType, clang::NamedDecl*) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->59 0x00000000053b42e0 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformDependentScopeDeclRefExpr(clang::DependentScopeDeclRefExpr*, bool, clang::TypeSourceInfo**) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->60 0x000000000537ea10 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCondition(clang::SourceLocation, clang::VarDecl*, clang::Expr*, clang::Sema::ConditionKind) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->61 0x00000000053ca561 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformIfStmt(clang::IfStmt*) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->62 0x00000000053c47ea clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->63 0x00000000053ccd8f clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&amp;) (/.../clang/21.1.4/bin/clang+++0x53ccd8f)
 #<!-- -->64 0x0000000005432319 clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/.../clang/21.1.4/bin/clang+++0x5432319)
 #<!-- -->65 0x000000000617ad9f clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/.../clang/21.1.4/bin/clang+++0x617ad9f)
 #<!-- -->66 0x00000000052ca08b clang::Sema::DeduceReturnType(clang::FunctionDecl*, clang::SourceLocation, bool) (/.../clang/21.1.4/bin/clang+++0x52ca08b)
 #<!-- -->67 0x0000000004d461d8 clang::Sema::DiagnoseUseOfDecl(clang::NamedDecl*, llvm::ArrayRef&lt;clang::SourceLocation&gt;, clang::ObjCInterfaceDecl const*, bool, bool, clang::ObjCInterfaceDecl*, bool) (/.../clang/21.1.4/bin/clang+++0x4d461d8)
 #<!-- -->68 0x000000000512d19b FinishOverloadedCallExpr(clang::Sema&amp;, clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, clang::OverloadCandidateSet*, clang::OverloadCandidate**, clang::OverloadingResult, bool) SemaOverload.cpp:0:0
 #<!-- -->69 0x000000000512df60 clang::Sema::BuildOverloadedCallExpr(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, bool) (/.../clang/21.1.4/bin/clang+++0x512df60)
 #<!-- -->70 0x0000000004d58608 clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, bool) (/.../clang/21.1.4/bin/clang+++0x4d58608)
 #<!-- -->71 0x0000000004d5a0ad clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*) (/.../clang/21.1.4/bin/clang+++0x4d5a0ad)
 #<!-- -->72 0x00000000048b35e7 clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult&lt;clang::Expr*, true&gt;) (/.../clang/21.1.4/bin/clang+++0x48b35e7)
 #<!-- -->73 0x00000000048ac16a clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&amp;, clang::TypoCorrectionTypeBehavior, bool, bool*) (/.../clang/21.1.4/bin/clang+++0x48ac16a)
 #<!-- -->74 0x00000000048ac379 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&amp;, clang::TypoCorrectionTypeBehavior, bool, bool*) (/.../clang/21.1.4/bin/clang+++0x48ac379)
 #<!-- -->75 0x00000000048ae7a6 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/.../clang/21.1.4/bin/clang+++0x48ae7a6)
 #<!-- -->76 0x00000000048ae838 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/.../clang/21.1.4/bin/clang+++0x48ae838)
 #<!-- -->77 0x00000000048b2819 clang::Parser::ParseExpression(clang::TypoCorrectionTypeBehavior) (/.../clang/21.1.4/bin/clang+++0x48b2819)
 #<!-- -->78 0x000000000493f100 clang::Parser::ParseReturnStatement() (/.../clang/21.1.4/bin/clang+++0x493f100)
 #<!-- -->79 0x00000000049440e3 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/.../clang/21.1.4/bin/clang+++0x49440e3)
 #<!-- -->80 0x000000000494521c clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/.../clang/21.1.4/bin/clang+++0x494521c)
 #<!-- -->81 0x00000000049466c3 clang::Parser::ParseCompoundStatementBody(bool) (/.../clang/21.1.4/bin/clang+++0x49466c3)
 #<!-- -->82 0x0000000004946ff4 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::Parser::ParseCompoundStatement(bool, unsigned int)::'lambda'()&gt;(long) ParseStmt.cpp:0:0
 #<!-- -->83 0x000000000617ad9f clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/.../clang/21.1.4/bin/clang+++0x617ad9f)
 #<!-- -->84 0x000000000493d7e6 clang::Parser::ParseCompoundStatement(bool, unsigned int) (/.../clang/21.1.4/bin/clang+++0x493d7e6)
 #<!-- -->85 0x00000000049441cf clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/.../clang/21.1.4/bin/clang+++0x49441cf)
 #<!-- -->86 0x000000000494521c clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/.../clang/21.1.4/bin/clang+++0x494521c)
 #<!-- -->87 0x0000000004947050 clang::Parser::ParseStatement(clang::SourceLocation*, clang::Parser::ParsedStmtContext) (/.../clang/21.1.4/bin/clang+++0x4947050)
 #<!-- -->88 0x0000000004942e12 clang::Parser::ParseIfStatement(clang::SourceLocation*) (/.../clang/21.1.4/bin/clang+++0x4942e12)
 #<!-- -->89 0x00000000049440cf clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/.../clang/21.1.4/bin/clang+++0x49440cf)
 #<!-- -->90 0x000000000494521c clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/.../clang/21.1.4/bin/clang+++0x494521c)
 #<!-- -->91 0x00000000049466c3 clang::Parser::ParseCompoundStatementBody(bool) (/.../clang/21.1.4/bin/clang+++0x49466c3)
 #<!-- -->92 0x0000000004946ee7 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/.../clang/21.1.4/bin/clang+++0x4946ee7)
 #<!-- -->93 0x000000000483357a clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/.../clang/21.1.4/bin/clang+++0x483357a)
 #<!-- -->94 0x000000000487b929 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/.../clang/21.1.4/bin/clang+++0x487b929)
 #<!-- -->95 0x000000000482e029 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/.../clang/21.1.4/bin/clang+++0x482e029)
 #<!-- -->96 0x000000000482e7e8 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/.../clang/21.1.4/bin/clang+++0x482e7e8)
 #<!-- -->97 0x0000000004836c58 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/.../clang/21.1.4/bin/clang+++0x4836c58)
 #<!-- -->98 0x00000000048989c5 clang::Parser::ParseInnerNamespace(llvm::SmallVector&lt;clang::Parser::InnerNamespaceInfo, 4u&gt; const&amp;, unsigned int, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::BalancedDelimiterTracker&amp;) (/.../clang/21.1.4/bin/clang+++0x48989c5)
 #<!-- -->99 0x000000000489959b clang::Parser::ParseNamespace(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::SourceLocation) (/.../clang/21.1.4/bin/clang+++0x489959b)
#<!-- -->100 0x000000000487cf0d clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/.../clang/21.1.4/bin/clang+++0x487cf0d)
#<!-- -->101 0x0000000004836e12 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/.../clang/21.1.4/bin/clang+++0x4836e12)
#<!-- -->102 0x0000000004837cf5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/.../clang/21.1.4/bin/clang+++0x4837cf5)
#<!-- -->103 0x00000000048267aa clang::ParseAST(clang::Sema&amp;, bool, bool) (/.../clang/21.1.4/bin/clang+++0x48267aa)
#<!-- -->104 0x0000000002f41803 clang::FrontendAction::Execute() (/.../clang/21.1.4/bin/clang+++0x2f41803)
#<!-- -->105 0x0000000002ec16bd clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/.../clang/21.1.4/bin/clang+++0x2ec16bd)
#<!-- -->106 0x0000000003033feb clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/.../clang/21.1.4/bin/clang+++0x3033feb)
#<!-- -->107 0x0000000000c8f1c4 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/.../clang/21.1.4/bin/clang+++0xc8f1c4)
#<!-- -->108 0x0000000000c84967 ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->109 0x0000000002c66889 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->110 0x000000000220e6f2 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/.../clang/21.1.4/bin/clang+++0x220e6f2)
#<!-- -->111 0x0000000002c67970 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->112 0x0000000002c24b65 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/.../clang/21.1.4/bin/clang+++0x2c24b65)
#<!-- -->113 0x0000000002c25cb2 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/.../clang/21.1.4/bin/clang+++0x2c25cb2)
#<!-- -->114 0x0000000002c32f74 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/.../clang/21.1.4/bin/clang+++0x2c32f74)
#<!-- -->115 0x0000000000c8a259 clang_main(int, char**, llvm::ToolContext const&amp;) (/.../clang/21.1.4/bin/clang+++0xc8a259)
#<!-- -->116 0x0000000000b898f3 main (/.../clang/21.1.4/bin/clang+++0xb898f3)
#<!-- -->117 0x00007f4dd227e865 __libc_start_main (/lib64/libc.so.6+0x3a865)
#<!-- -->118 0x0000000000c83e6e _start (/.../clang/21.1.4/bin/clang+++0xc83e6e)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
clang version 21.1.4
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /.../clang/21.1.4/bin
Build config: +assertions
clang++: note: diagnostic msg:
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: /tmp/BuiltinDialectBytecode-0f1662.cpp
clang++: note: diagnostic msg: /tmp/BuiltinDialectBytecode-0f1662.sh
clang++: note: diagnostic msg:

********************
```

&lt;/details&gt;
</details>


---

### Comment 2 - nuudlman

Doesn't crash with Clang 23 (3286de333b691d3d815939ce5618f85be71b1575) or with Clang 20.1.7

---

### Comment 3 - nuudlman

Running creduce on it, but it looks like it will take a few hours.

---

