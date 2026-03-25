# ICE in QualType::getCommonPtr() using is_nothrow_invocable_v with unnamed lambda since clang 17 with -std=c++20

**Author:** mariete1223
**URL:** https://github.com/llvm/llvm-project/issues/140462

## Body

## Assertion

```
clang++: /root/llvm-project/llvm/tools/clang/include/clang/AST/Type.h:952: const clang::ExtQualsTypeCommonBase* clang::QualType::getCommonPtr() const: Assertion `!isNull() && "Cannot retrieve a NULL type pointer"' failed.
```


## Stack dump

```
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x c++ -std=c++20 <source>
1.	<source>:7:7: current parser token ')'
2.	<source>:4:1: parsing function body 'main'
3.	<source>:4:1: in compound statement ('{}')
4.	<source>:5:5: lambda expression parsing
5.	/opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../include/c++/16.0.0/type_traits:3722:25: instantiating variable definition 'std::is_nothrow_invocable_v<(lambda at <source>:6:9)>'
6.	/opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../include/c++/16.0.0/type_traits:3338:12: instantiating class definition 'std::is_nothrow_invocable<(lambda at <source>:6:9)>'
7.	/opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../include/c++/16.0.0/type_traits:203:12: instantiating class definition 'std::__and_<std::__is_invocable_impl<std::__invoke_result<(lambda at <source>:6:9)>, void>, std::__call_is_nothrow<std::__invoke_result<(lambda at <source>:6:9)>, (lambda at <source>:6:9)>>'
8.	/opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../include/c++/16.0.0/type_traits:3257:12: instantiating class definition 'std::__call_is_nothrow<std::__invoke_result<(lambda at <source>:6:9)>, (lambda at <source>:6:9)>'
9.	/opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../include/c++/16.0.0/type_traits:3251:20: instantiating function definition 'std::__call_is_nt<(lambda at <source>:6:9)>'
10.	/opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../include/c++/16.0.0/type_traits:3251:20: LLVM IR generation of declaration 'std::__call_is_nt'
11.	/opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../include/c++/16.0.0/type_traits:3251:20: Mangling declaration 'std::__call_is_nt'
  #0 0x0000000003f9b2a8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f9b2a8)
  #1 0x0000000003f98f34 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f98f34)
  #2 0x0000000003eddb68 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
  #3 0x00007a32f3a42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
  #4 0x00007a32f3a969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
  #5 0x00007a32f3a42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
  #6 0x00007a32f3a287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
  #7 0x00007a32f3a2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
  #8 0x00007a32f3a39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
  #9 0x0000000007b808e6 (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
 #10 0x0000000007b6d534 (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
 #11 0x0000000007b6de1b (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
 #12 0x0000000007b6ea67 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) ItaniumMangle.cpp:0:0
 #13 0x0000000007b712aa (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) ItaniumMangle.cpp:0:0
 #14 0x0000000007b891c8 (anonymous namespace)::CXXNameMangler::mangleTemplateArg(clang::TemplateArgument, bool) ItaniumMangle.cpp:0:0
 #15 0x0000000007b8672f (anonymous namespace)::CXXNameMangler::mangleTemplateArgs(clang::TemplateName, clang::TemplateArgumentList const&) ItaniumMangle.cpp:0:0
 #16 0x0000000007b6df16 (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
 #17 0x0000000007b6ea67 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) ItaniumMangle.cpp:0:0
 #18 0x0000000007b7f8b1 (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
 #19 0x0000000007b809f7 (anonymous namespace)::CXXNameMangler::mangle(clang::GlobalDecl) ItaniumMangle.cpp:0:0
 #20 0x0000000007b83081 (anonymous namespace)::ItaniumMangleContextImpl::mangleCXXName(clang::GlobalDecl, llvm::raw_ostream&) ItaniumMangle.cpp:0:0
 #21 0x0000000007ba7d93 clang::MangleContext::mangleName(clang::GlobalDecl, llvm::raw_ostream&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7ba7d93)
 #22 0x0000000004515024 getMangledNameImpl(clang::CodeGen::CodeGenModule&, clang::GlobalDecl, clang::NamedDecl const*, bool) CodeGenModule.cpp:0:0
 #23 0x000000000452c8ad clang::CodeGen::CodeGenModule::getMangledName(clang::GlobalDecl) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x452c8ad)
 #24 0x0000000004562817 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4562817)
 #25 0x000000000456d5d3 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
 #26 0x000000000494a9e6 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
 #27 0x000000000493a614 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x493a614)
 #28 0x00000000072963a5 clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x72963a5)
 #29 0x0000000006b710ae void llvm::function_ref<void ()>::callback_fn<clang::Sema::MarkFunctionReferenced(clang::SourceLocation, clang::FunctionDecl*, bool)::'lambda'()>(long) SemaExpr.cpp:0:0
 #30 0x0000000007f3a981 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7f3a981)
 #31 0x0000000006b6f1ef clang::Sema::MarkFunctionReferenced(clang::SourceLocation, clang::FunctionDecl*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b6f1ef)
 #32 0x0000000006b6f86d MarkExprReferenced(clang::Sema&, clang::SourceLocation, clang::Decl*, clang::Expr*, bool, llvm::DenseMap<clang::VarDecl const*, int, llvm::DenseMapInfo<clang::VarDecl const*, void>, llvm::detail::DenseMapPair<clang::VarDecl const*, int>>&) SemaExpr.cpp:0:0
 #33 0x0000000006b5fbd9 clang::Sema::BuildDeclRefExpr(clang::ValueDecl*, clang::QualType, clang::ExprValueKind, clang::DeclarationNameInfo const&, clang::NestedNameSpecifierLoc, clang::NamedDecl*, clang::SourceLocation, clang::TemplateArgumentListInfo const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b5fbd9)
 #34 0x0000000006f63c0d clang::Sema::FixOverloadedFunctionReference(clang::Expr*, clang::DeclAccessPair, clang::FunctionDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f63c0d)
 #35 0x0000000006f9b50e FinishOverloadedCallExpr(clang::Sema&, clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, clang::OverloadCandidateSet*, clang::OverloadCandidate**, clang::OverloadingResult, bool) SemaOverload.cpp:0:0
 #36 0x0000000006f9c5f0 clang::Sema::BuildOverloadedCallExpr(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f9c5f0)
 #37 0x0000000006b94c86 clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b94c86)
 #38 0x0000000006b95e8e clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b95e8e)
 #39 0x00000000071fa29b clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCallExpr(clang::CallExpr*) SemaTemplateInstantiate.cpp:0:0
 #40 0x00000000071ed6d8 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
 #41 0x00000000071ed73c clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
 #42 0x000000000720d713 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformTemplateArgument(clang::TemplateArgumentLoc const&, clang::TemplateArgumentLoc&, bool) SemaTemplateInstantiate.cpp:0:0
 #43 0x000000000720df48 (anonymous namespace)::TemplateInstantiator::TransformTemplateArgument(clang::TemplateArgumentLoc const&, clang::TemplateArgumentLoc&, bool) SemaTemplateInstantiate.cpp:0:0
 #44 0x00000000072109f8 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformTemplateSpecializationType(clang::TypeLocBuilder&, clang::TemplateSpecializationTypeLoc, clang::TemplateName) SemaTemplateInstantiate.cpp:0:0
 #45 0x00000000071ff733 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
 #46 0x000000000721ecc4 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformElaboratedType(clang::TypeLocBuilder&, clang::ElaboratedTypeLoc) SemaTemplateInstantiate.cpp:0:0
 #47 0x00000000071fe982 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
 #48 0x000000000720543a clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeSourceInfo*) SemaTemplateInstantiate.cpp:0:0
 #49 0x0000000007213fae clang::Sema::SubstType(clang::TypeSourceInfo*, clang::MultiLevelTemplateArgumentList const&, clang::SourceLocation, clang::DeclarationName, bool) (.constprop.0) SemaTemplateInstantiate.cpp:0:0
 #50 0x00000000072144e5 clang::Sema::SubstBaseSpecifiers(clang::CXXRecordDecl*, clang::CXXRecordDecl*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x72144e5)
 #51 0x0000000007214f14 clang::Sema::InstantiateClass(clang::SourceLocation, clang::CXXRecordDecl*, clang::CXXRecordDecl*, clang::MultiLevelTemplateArgumentList const&, clang::TemplateSpecializationKind, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7214f14)
 #52 0x00000000072379d8 clang::Sema::InstantiateClassTemplateSpecialization(clang::SourceLocation, clang::ClassTemplateSpecializationDecl*, clang::TemplateSpecializationKind, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x72379d8)
 #53 0x00000000072b567e void llvm::function_ref<void ()>::callback_fn<clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*)::'lambda'()>(long) SemaType.cpp:0:0
 #54 0x0000000007f3a981 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7f3a981)
 #55 0x00000000072be84a clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x72be84a)
 #56 0x00000000072bee45 clang::Sema::RequireCompleteType(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x72bee45)
 #57 0x00000000068512fc clang::Sema::RequireCompleteDeclContext(clang::CXXScopeSpec&, clang::DeclContext*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68512fc)
 #58 0x0000000006d9415f clang::Sema::LookupParsedName(clang::LookupResult&, clang::Scope*, clang::CXXScopeSpec*, clang::QualType, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d9415f)
 #59 0x0000000006b664f0 clang::Sema::BuildQualifiedDeclarationNameExpr(clang::CXXScopeSpec&, clang::DeclarationNameInfo const&, bool, clang::TypeSourceInfo**) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b664f0)
 #60 0x00000000072193f6 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformDependentScopeDeclRefExpr(clang::DependentScopeDeclRefExpr*, bool, clang::TypeSourceInfo**) SemaTemplateInstantiate.cpp:0:0
 #61 0x00000000071ed7a3 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
 #62 0x00000000071ef885 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformInitializer(clang::Expr*, bool) (.part.0) SemaTemplateInstantiate.cpp:0:0
 #63 0x00000000071ef228 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExprs(clang::Expr* const*, unsigned int, bool, llvm::SmallVectorImpl<clang::Expr*>&, bool*) SemaTemplateInstantiate.cpp:0:0
 #64 0x0000000007213372 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCXXUnresolvedConstructExpr(clang::CXXUnresolvedConstructExpr*) SemaTemplateInstantiate.cpp:0:0
 #65 0x00000000071ed69c clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
 #66 0x000000000720d713 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformTemplateArgument(clang::TemplateArgumentLoc const&, clang::TemplateArgumentLoc&, bool) SemaTemplateInstantiate.cpp:0:0
 #67 0x000000000720df48 (anonymous namespace)::TemplateInstantiator::TransformTemplateArgument(clang::TemplateArgumentLoc const&, clang::TemplateArgumentLoc&, bool) SemaTemplateInstantiate.cpp:0:0
 #68 0x00000000072109f8 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformTemplateSpecializationType(clang::TypeLocBuilder&, clang::TemplateSpecializationTypeLoc, clang::TemplateName) SemaTemplateInstantiate.cpp:0:0
 #69 0x00000000071ff733 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
 #70 0x000000000721ecc4 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformElaboratedType(clang::TypeLocBuilder&, clang::ElaboratedTypeLoc) SemaTemplateInstantiate.cpp:0:0
 #71 0x00000000071fe982 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
 #72 0x000000000720543a clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeSourceInfo*) SemaTemplateInstantiate.cpp:0:0
 #73 0x000000000720d7bb clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformTemplateArgument(clang::TemplateArgumentLoc const&, clang::TemplateArgumentLoc&, bool) SemaTemplateInstantiate.cpp:0:0
 #74 0x0000000007210d22 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformTemplateSpecializationType(clang::TypeLocBuilder&, clang::TemplateSpecializationTypeLoc, clang::TemplateName) SemaTemplateInstantiate.cpp:0:0
 #75 0x00000000071ff733 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
 #76 0x000000000721ecc4 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformElaboratedType(clang::TypeLocBuilder&, clang::ElaboratedTypeLoc) SemaTemplateInstantiate.cpp:0:0
 #77 0x00000000071fe982 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
 #78 0x000000000720543a clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeSourceInfo*) SemaTemplateInstantiate.cpp:0:0
 #79 0x000000000720555c clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::QualType) SemaTemplateInstantiate.cpp:0:0
 #80 0x0000000007207381 clang::Sema::SubstType(clang::QualType, clang::MultiLevelTemplateArgumentList const&, clang::SourceLocation, clang::DeclarationName, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7207381)
 #81 0x00000000071231af clang::Sema::SubstituteExplicitTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo&, llvm::SmallVectorImpl<clang::DeducedTemplateArgument>&, llvm::SmallVectorImpl<clang::QualType>&, clang::QualType*, clang::sema::TemplateDeductionInfo&) (.constprop.0) SemaTemplateDeduction.cpp:0:0
 #82 0x0000000007123bb0 void llvm::function_ref<void ()>::callback_fn<clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, bool, bool, clang::QualType, clang::Expr::Classification, bool, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>)>)::'lambda'()>(long) SemaTemplateDeduction.cpp:0:0
 #83 0x0000000007f3a981 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7f3a981)
 #84 0x0000000007158f6c clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, bool, bool, clang::QualType, clang::Expr::Classification, bool, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>)>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7158f6c)
 #85 0x0000000006f832ab AddTemplateOverloadCandidateImmediately(clang::Sema&, clang::OverloadCandidateSet&, clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, bool, bool, bool, clang::CallExpr::ADLCallKind, clang::OverloadCandidateParamOrder, bool) SemaOverload.cpp:0:0
 #86 0x0000000006f85736 clang::Sema::AddTemplateOverloadCandidate(clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::OverloadCandidateSet&, bool, bool, bool, clang::CallExpr::ADLCallKind, clang::OverloadCandidateParamOrder, bool) (.constprop.1) SemaOverload.cpp:0:0
 #87 0x0000000006f8593b AddOverloadedCallCandidate(clang::Sema&, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::OverloadCandidateSet&, bool, bool) SemaOverload.cpp:0:0
 #88 0x0000000006f870e1 clang::Sema::AddOverloadedCallCandidates(clang::UnresolvedLookupExpr*, llvm::ArrayRef<clang::Expr*>, clang::OverloadCandidateSet&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f870e1)
 #89 0x0000000006f873de clang::Sema::buildOverloadedCallSet(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::OverloadCandidateSet*, clang::ActionResult<clang::Expr*, true>*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f873de)
 #90 0x0000000006f9c4c0 clang::Sema::BuildOverloadedCallExpr(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f9c4c0)
 #91 0x0000000006b94c86 clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b94c86)
 #92 0x0000000006b95e8e clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b95e8e)
 #93 0x00000000071fa29b clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCallExpr(clang::CallExpr*) SemaTemplateInstantiate.cpp:0:0
 #94 0x00000000071ed6d8 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
 #95 0x00000000071fe717 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
 #96 0x000000000720543a clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeSourceInfo*) SemaTemplateInstantiate.cpp:0:0
 #97 0x0000000007213fae clang::Sema::SubstType(clang::TypeSourceInfo*, clang::MultiLevelTemplateArgumentList const&, clang::SourceLocation, clang::DeclarationName, bool) (.constprop.0) SemaTemplateInstantiate.cpp:0:0
 #98 0x00000000072144e5 clang::Sema::SubstBaseSpecifiers(clang::CXXRecordDecl*, clang::CXXRecordDecl*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x72144e5)
 #99 0x0000000007214f14 clang::Sema::InstantiateClass(clang::SourceLocation, clang::CXXRecordDecl*, clang::CXXRecordDecl*, clang::MultiLevelTemplateArgumentList const&, clang::TemplateSpecializationKind, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7214f14)
#100 0x00000000072379d8 clang::Sema::InstantiateClassTemplateSpecialization(clang::SourceLocation, clang::ClassTemplateSpecializationDecl*, clang::TemplateSpecializationKind, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x72379d8)
#101 0x00000000072b567e void llvm::function_ref<void ()>::callback_fn<clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*)::'lambda'()>(long) SemaType.cpp:0:0
#102 0x0000000007f3a981 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7f3a981)
#103 0x00000000072be84a clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x72be84a)
#104 0x00000000072bee45 clang::Sema::RequireCompleteType(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x72bee45)
#105 0x00000000068512fc clang::Sema::RequireCompleteDeclContext(clang::CXXScopeSpec&, clang::DeclContext*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68512fc)
#106 0x000000000708dd51 clang::Sema::CheckTypenameType(clang::ElaboratedTypeKeyword, clang::SourceLocation, clang::NestedNameSpecifierLoc, clang::IdentifierInfo const&, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x708dd51)
#107 0x00000000071db2ee clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::RebuildDependentNameType(clang::ElaboratedTypeKeyword, clang::SourceLocation, clang::NestedNameSpecifierLoc, clang::IdentifierInfo const*, clang::SourceLocation, bool) SemaTemplateInstantiate.cpp:0:0
#108 0x000000000721292a clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformDependentNameType(clang::TypeLocBuilder&, clang::DependentNameTypeLoc, bool) SemaTemplateInstantiate.cpp:0:0
#109 0x00000000071fe853 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
#110 0x000000000720543a clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeSourceInfo*) SemaTemplateInstantiate.cpp:0:0
#111 0x0000000007213fae clang::Sema::SubstType(clang::TypeSourceInfo*, clang::MultiLevelTemplateArgumentList const&, clang::SourceLocation, clang::DeclarationName, bool) (.constprop.0) SemaTemplateInstantiate.cpp:0:0
#112 0x00000000072144e5 clang::Sema::SubstBaseSpecifiers(clang::CXXRecordDecl*, clang::CXXRecordDecl*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x72144e5)
#113 0x0000000007214f14 clang::Sema::InstantiateClass(clang::SourceLocation, clang::CXXRecordDecl*, clang::CXXRecordDecl*, clang::MultiLevelTemplateArgumentList const&, clang::TemplateSpecializationKind, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7214f14)
#114 0x00000000072379d8 clang::Sema::InstantiateClassTemplateSpecialization(clang::SourceLocation, clang::ClassTemplateSpecializationDecl*, clang::TemplateSpecializationKind, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x72379d8)
#115 0x00000000072b567e void llvm::function_ref<void ()>::callback_fn<clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*)::'lambda'()>(long) SemaType.cpp:0:0
#116 0x0000000007f3a981 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7f3a981)
#117 0x00000000072be84a clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x72be84a)
#118 0x00000000072bee45 clang::Sema::RequireCompleteType(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x72bee45)
#119 0x00000000068512fc clang::Sema::RequireCompleteDeclContext(clang::CXXScopeSpec&, clang::DeclContext*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68512fc)
#120 0x0000000006d9415f clang::Sema::LookupParsedName(clang::LookupResult&, clang::Scope*, clang::CXXScopeSpec*, clang::QualType, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d9415f)
#121 0x0000000006b664f0 clang::Sema::BuildQualifiedDeclarationNameExpr(clang::CXXScopeSpec&, clang::DeclarationNameInfo const&, bool, clang::TypeSourceInfo**) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b664f0)
#122 0x00000000072193f6 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformDependentScopeDeclRefExpr(clang::DependentScopeDeclRefExpr*, bool, clang::TypeSourceInfo**) SemaTemplateInstantiate.cpp:0:0
#123 0x00000000071ed7a3 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#124 0x00000000071ef885 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformInitializer(clang::Expr*, bool) (.part.0) SemaTemplateInstantiate.cpp:0:0
#125 0x00000000071efe9b clang::Sema::SubstInitializer(clang::Expr*, clang::MultiLevelTemplateArgumentList const&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x71efe9b)
#126 0x0000000007244096 clang::Sema::InstantiateVariableInitializer(clang::VarDecl*, clang::VarDecl*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7244096)
#127 0x000000000724450e clang::Sema::CompleteVarTemplateSpecializationDecl(clang::VarTemplateSpecializationDecl*, clang::VarDecl*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x724450e)
#128 0x0000000007294088 clang::Sema::InstantiateVariableDefinition(clang::SourceLocation, clang::VarDecl*, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7294088)
#129 0x0000000007f3a981 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7f3a981)
#130 0x0000000006b5f5c3 DoMarkVarDeclReferenced(clang::Sema&, clang::SourceLocation, clang::VarDecl*, clang::Expr*, llvm::DenseMap<clang::VarDecl const*, int, llvm::DenseMapInfo<clang::VarDecl const*, void>, llvm::detail::DenseMapPair<clang::VarDecl const*, int>>&) SemaExpr.cpp:0:0
#131 0x0000000006b6f7de MarkExprReferenced(clang::Sema&, clang::SourceLocation, clang::Decl*, clang::Expr*, bool, llvm::DenseMap<clang::VarDecl const*, int, llvm::DenseMapInfo<clang::VarDecl const*, void>, llvm::detail::DenseMapPair<clang::VarDecl const*, int>>&) SemaExpr.cpp:0:0
#132 0x0000000006b5fbd9 clang::Sema::BuildDeclRefExpr(clang::ValueDecl*, clang::QualType, clang::ExprValueKind, clang::DeclarationNameInfo const&, clang::NestedNameSpecifierLoc, clang::NamedDecl*, clang::SourceLocation, clang::TemplateArgumentListInfo const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b5fbd9)
#133 0x0000000006b6038f clang::Sema::BuildDeclRefExpr(clang::ValueDecl*, clang::QualType, clang::ExprValueKind, clang::DeclarationNameInfo const&, clang::CXXScopeSpec const*, clang::NamedDecl*, clang::SourceLocation, clang::TemplateArgumentListInfo const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b6038f)
#134 0x0000000006b65d43 clang::Sema::BuildDeclarationNameExpr(clang::CXXScopeSpec const&, clang::DeclarationNameInfo const&, clang::NamedDecl*, clang::NamedDecl*, clang::TemplateArgumentListInfo const*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b65d43)
#135 0x000000000709a7ab clang::Sema::BuildTemplateIdExpr(clang::CXXScopeSpec const&, clang::SourceLocation, clang::LookupResult&, bool, clang::TemplateArgumentListInfo const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x709a7ab)
#136 0x0000000006b67e78 clang::Sema::ActOnIdExpression(clang::Scope*, clang::CXXScopeSpec&, clang::SourceLocation, clang::UnqualifiedId&, bool, bool, clang::CorrectionCandidateCallback*, bool, clang::Token*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b67e78)
#137 0x00000000066ceea8 clang::Parser::tryParseCXXIdExpression(clang::CXXScopeSpec&, bool, clang::Token&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66ceea8)
#138 0x00000000066cf1ca clang::Parser::ParseCXXIdExpression(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66cf1ca)
#139 0x00000000066b1a72 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66b1a72)
#140 0x00000000066b122e clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66b122e)
#141 0x00000000066bf14e clang::Parser::ParseConstantExpressionInExprEvalContext(clang::TypeCastState) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66bf14e)
#142 0x0000000006695869 clang::Parser::tryParseExceptionSpecification(bool, clang::SourceRange&, llvm::SmallVectorImpl<clang::OpaquePtr<clang::QualType>>&, llvm::SmallVectorImpl<clang::SourceRange>&, clang::ActionResult<clang::Expr*, true>&, llvm::SmallVector<clang::Token, 4u>*&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6695869)
#143 0x00000000066d28b2 clang::Parser::ParseLambdaExpressionAfterIntroducer(clang::LambdaIntroducer&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66d28b2)
#144 0x00000000066d35fb clang::Parser::ParseLambdaExpression() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66d35fb)
#145 0x00000000066b2795 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66b2795)
#146 0x00000000066b3157 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66b3157)
#147 0x00000000066b31e9 clang::Parser::ParseAssignmentExpression(clang::TypeCastState) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66b31e9)
#148 0x00000000066b7bc9 clang::Parser::ParseExpression(clang::TypeCastState) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66b7bc9)
#149 0x0000000006748559 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6748559)
#150 0x00000000067400eb clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67400eb)
#151 0x0000000006740f2d clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6740f2d)
#152 0x0000000006748c53 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6748c53)
#153 0x000000000674941a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x674941a)
#154 0x0000000006643433 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6643433)
#155 0x0000000006679dcd clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6679dcd)
#156 0x0000000006636f7e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6636f7e)
#157 0x0000000006637739 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6637739)
#158 0x000000000663f04a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x663f04a)
#159 0x000000000663ffed clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x663ffed)
#160 0x00000000066323ba clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66323ba)
#161 0x0000000004946b78 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4946b78)
#162 0x0000000004c3eb45 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c3eb45)
#163 0x0000000004bbd78e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bbd78e)
#164 0x0000000004d31629 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d31629)
#165 0x0000000000dace0f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdace0f)
#166 0x0000000000da2fda ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#167 0x00000000049ade69 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#168 0x0000000003ede004 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3ede004)
#169 0x00000000049ae47f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#170 0x000000000497024d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x497024d)
#171 0x00000000049712de clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49712de)
#172 0x00000000049795c5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49795c5)
#173 0x0000000000da8d78 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda8d78)
#174 0x0000000000c2e774 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc2e774)
#175 0x00007a32f3a29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#176 0x00007a32f3a29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#177 0x0000000000da2a85 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda2a85)
```

## Program

```
#include <iostream>
using namespace std;
auto main() -> int
{
    []() noexcept(is_nothrow_invocable_v<decltype(
        [](){}
    )>);
}
```

## To quickly reproduce

https://godbolt.org/z/9af5Phv5P

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (mariete1223)

<details>
## Assertion

```
clang++: /root/llvm-project/llvm/tools/clang/include/clang/AST/Type.h:952: const clang::ExtQualsTypeCommonBase* clang::QualType::getCommonPtr() const: Assertion `!isNull() &amp;&amp; "Cannot retrieve a NULL type pointer"' failed.
```


## Stack dump

```
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x c++ -std=c++20 &lt;source&gt;
1.	&lt;source&gt;:7:7: current parser token ')'
2.	&lt;source&gt;:4:1: parsing function body 'main'
3.	&lt;source&gt;:4:1: in compound statement ('{}')
4.	&lt;source&gt;:5:5: lambda expression parsing
5.	/opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../include/c++/16.0.0/type_traits:3722:25: instantiating variable definition 'std::is_nothrow_invocable_v&lt;(lambda at &lt;source&gt;:6:9)&gt;'
6.	/opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../include/c++/16.0.0/type_traits:3338:12: instantiating class definition 'std::is_nothrow_invocable&lt;(lambda at &lt;source&gt;:6:9)&gt;'
7.	/opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../include/c++/16.0.0/type_traits:203:12: instantiating class definition 'std::__and_&lt;std::__is_invocable_impl&lt;std::__invoke_result&lt;(lambda at &lt;source&gt;:6:9)&gt;, void&gt;, std::__call_is_nothrow&lt;std::__invoke_result&lt;(lambda at &lt;source&gt;:6:9)&gt;, (lambda at &lt;source&gt;:6:9)&gt;&gt;'
8.	/opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../include/c++/16.0.0/type_traits:3257:12: instantiating class definition 'std::__call_is_nothrow&lt;std::__invoke_result&lt;(lambda at &lt;source&gt;:6:9)&gt;, (lambda at &lt;source&gt;:6:9)&gt;'
9.	/opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../include/c++/16.0.0/type_traits:3251:20: instantiating function definition 'std::__call_is_nt&lt;(lambda at &lt;source&gt;:6:9)&gt;'
10.	/opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../include/c++/16.0.0/type_traits:3251:20: LLVM IR generation of declaration 'std::__call_is_nt'
11.	/opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../include/c++/16.0.0/type_traits:3251:20: Mangling declaration 'std::__call_is_nt'
  #<!-- -->0 0x0000000003f9b2a8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f9b2a8)
  #<!-- -->1 0x0000000003f98f34 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f98f34)
  #<!-- -->2 0x0000000003eddb68 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
  #<!-- -->3 0x00007a32f3a42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
  #<!-- -->4 0x00007a32f3a969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
  #<!-- -->5 0x00007a32f3a42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
  #<!-- -->6 0x00007a32f3a287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
  #<!-- -->7 0x00007a32f3a2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
  #<!-- -->8 0x00007a32f3a39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
  #<!-- -->9 0x0000000007b808e6 (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
 #<!-- -->10 0x0000000007b6d534 (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector&lt;llvm::StringRef, 4u&gt; const*) ItaniumMangle.cpp:0:0
 #<!-- -->11 0x0000000007b6de1b (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector&lt;llvm::StringRef, 4u&gt; const*) ItaniumMangle.cpp:0:0
 #<!-- -->12 0x0000000007b6ea67 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) ItaniumMangle.cpp:0:0
 #<!-- -->13 0x0000000007b712aa (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) ItaniumMangle.cpp:0:0
 #<!-- -->14 0x0000000007b891c8 (anonymous namespace)::CXXNameMangler::mangleTemplateArg(clang::TemplateArgument, bool) ItaniumMangle.cpp:0:0
 #<!-- -->15 0x0000000007b8672f (anonymous namespace)::CXXNameMangler::mangleTemplateArgs(clang::TemplateName, clang::TemplateArgumentList const&amp;) ItaniumMangle.cpp:0:0
 #<!-- -->16 0x0000000007b6df16 (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector&lt;llvm::StringRef, 4u&gt; const*) ItaniumMangle.cpp:0:0
 #<!-- -->17 0x0000000007b6ea67 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) ItaniumMangle.cpp:0:0
 #<!-- -->18 0x0000000007b7f8b1 (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
 #<!-- -->19 0x0000000007b809f7 (anonymous namespace)::CXXNameMangler::mangle(clang::GlobalDecl) ItaniumMangle.cpp:0:0
 #<!-- -->20 0x0000000007b83081 (anonymous namespace)::ItaniumMangleContextImpl::mangleCXXName(clang::GlobalDecl, llvm::raw_ostream&amp;) ItaniumMangle.cpp:0:0
 #<!-- -->21 0x0000000007ba7d93 clang::MangleContext::mangleName(clang::GlobalDecl, llvm::raw_ostream&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7ba7d93)
 #<!-- -->22 0x0000000004515024 getMangledNameImpl(clang::CodeGen::CodeGenModule&amp;, clang::GlobalDecl, clang::NamedDecl const*, bool) CodeGenModule.cpp:0:0
 #<!-- -->23 0x000000000452c8ad clang::CodeGen::CodeGenModule::getMangledName(clang::GlobalDecl) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x452c8ad)
 #<!-- -->24 0x0000000004562817 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4562817)
 #<!-- -->25 0x000000000456d5d3 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
 #<!-- -->26 0x000000000494a9e6 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
 #<!-- -->27 0x000000000493a614 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x493a614)
 #<!-- -->28 0x00000000072963a5 clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x72963a5)
 #<!-- -->29 0x0000000006b710ae void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::Sema::MarkFunctionReferenced(clang::SourceLocation, clang::FunctionDecl*, bool)::'lambda'()&gt;(long) SemaExpr.cpp:0:0
 #<!-- -->30 0x0000000007f3a981 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7f3a981)
 #<!-- -->31 0x0000000006b6f1ef clang::Sema::MarkFunctionReferenced(clang::SourceLocation, clang::FunctionDecl*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b6f1ef)
 #<!-- -->32 0x0000000006b6f86d MarkExprReferenced(clang::Sema&amp;, clang::SourceLocation, clang::Decl*, clang::Expr*, bool, llvm::DenseMap&lt;clang::VarDecl const*, int, llvm::DenseMapInfo&lt;clang::VarDecl const*, void&gt;, llvm::detail::DenseMapPair&lt;clang::VarDecl const*, int&gt;&gt;&amp;) SemaExpr.cpp:0:0
 #<!-- -->33 0x0000000006b5fbd9 clang::Sema::BuildDeclRefExpr(clang::ValueDecl*, clang::QualType, clang::ExprValueKind, clang::DeclarationNameInfo const&amp;, clang::NestedNameSpecifierLoc, clang::NamedDecl*, clang::SourceLocation, clang::TemplateArgumentListInfo const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b5fbd9)
 #<!-- -->34 0x0000000006f63c0d clang::Sema::FixOverloadedFunctionReference(clang::Expr*, clang::DeclAccessPair, clang::FunctionDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f63c0d)
 #<!-- -->35 0x0000000006f9b50e FinishOverloadedCallExpr(clang::Sema&amp;, clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, clang::OverloadCandidateSet*, clang::OverloadCandidate**, clang::OverloadingResult, bool) SemaOverload.cpp:0:0
 #<!-- -->36 0x0000000006f9c5f0 clang::Sema::BuildOverloadedCallExpr(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f9c5f0)
 #<!-- -->37 0x0000000006b94c86 clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b94c86)
 #<!-- -->38 0x0000000006b95e8e clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b95e8e)
 #<!-- -->39 0x00000000071fa29b clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCallExpr(clang::CallExpr*) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->40 0x00000000071ed6d8 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->41 0x00000000071ed73c clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->42 0x000000000720d713 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformTemplateArgument(clang::TemplateArgumentLoc const&amp;, clang::TemplateArgumentLoc&amp;, bool) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->43 0x000000000720df48 (anonymous namespace)::TemplateInstantiator::TransformTemplateArgument(clang::TemplateArgumentLoc const&amp;, clang::TemplateArgumentLoc&amp;, bool) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->44 0x00000000072109f8 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformTemplateSpecializationType(clang::TypeLocBuilder&amp;, clang::TemplateSpecializationTypeLoc, clang::TemplateName) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->45 0x00000000071ff733 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeLocBuilder&amp;, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->46 0x000000000721ecc4 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformElaboratedType(clang::TypeLocBuilder&amp;, clang::ElaboratedTypeLoc) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->47 0x00000000071fe982 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeLocBuilder&amp;, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->48 0x000000000720543a clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeSourceInfo*) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->49 0x0000000007213fae clang::Sema::SubstType(clang::TypeSourceInfo*, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceLocation, clang::DeclarationName, bool) (.constprop.0) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->50 0x00000000072144e5 clang::Sema::SubstBaseSpecifiers(clang::CXXRecordDecl*, clang::CXXRecordDecl*, clang::MultiLevelTemplateArgumentList const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x72144e5)
 #<!-- -->51 0x0000000007214f14 clang::Sema::InstantiateClass(clang::SourceLocation, clang::CXXRecordDecl*, clang::CXXRecordDecl*, clang::MultiLevelTemplateArgumentList const&amp;, clang::TemplateSpecializationKind, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7214f14)
 #<!-- -->52 0x00000000072379d8 clang::Sema::InstantiateClassTemplateSpecialization(clang::SourceLocation, clang::ClassTemplateSpecializationDecl*, clang::TemplateSpecializationKind, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x72379d8)
 #<!-- -->53 0x00000000072b567e void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*)::'lambda'()&gt;(long) SemaType.cpp:0:0
 #<!-- -->54 0x0000000007f3a981 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7f3a981)
 #<!-- -->55 0x00000000072be84a clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x72be84a)
 #<!-- -->56 0x00000000072bee45 clang::Sema::RequireCompleteType(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x72bee45)
 #<!-- -->57 0x00000000068512fc clang::Sema::RequireCompleteDeclContext(clang::CXXScopeSpec&amp;, clang::DeclContext*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68512fc)
 #<!-- -->58 0x0000000006d9415f clang::Sema::LookupParsedName(clang::LookupResult&amp;, clang::Scope*, clang::CXXScopeSpec*, clang::QualType, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d9415f)
 #<!-- -->59 0x0000000006b664f0 clang::Sema::BuildQualifiedDeclarationNameExpr(clang::CXXScopeSpec&amp;, clang::DeclarationNameInfo const&amp;, bool, clang::TypeSourceInfo**) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b664f0)
 #<!-- -->60 0x00000000072193f6 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformDependentScopeDeclRefExpr(clang::DependentScopeDeclRefExpr*, bool, clang::TypeSourceInfo**) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->61 0x00000000071ed7a3 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->62 0x00000000071ef885 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformInitializer(clang::Expr*, bool) (.part.0) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->63 0x00000000071ef228 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExprs(clang::Expr* const*, unsigned int, bool, llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, bool*) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->64 0x0000000007213372 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCXXUnresolvedConstructExpr(clang::CXXUnresolvedConstructExpr*) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->65 0x00000000071ed69c clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->66 0x000000000720d713 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformTemplateArgument(clang::TemplateArgumentLoc const&amp;, clang::TemplateArgumentLoc&amp;, bool) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->67 0x000000000720df48 (anonymous namespace)::TemplateInstantiator::TransformTemplateArgument(clang::TemplateArgumentLoc const&amp;, clang::TemplateArgumentLoc&amp;, bool) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->68 0x00000000072109f8 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformTemplateSpecializationType(clang::TypeLocBuilder&amp;, clang::TemplateSpecializationTypeLoc, clang::TemplateName) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->69 0x00000000071ff733 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeLocBuilder&amp;, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->70 0x000000000721ecc4 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformElaboratedType(clang::TypeLocBuilder&amp;, clang::ElaboratedTypeLoc) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->71 0x00000000071fe982 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeLocBuilder&amp;, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->72 0x000000000720543a clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeSourceInfo*) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->73 0x000000000720d7bb clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformTemplateArgument(clang::TemplateArgumentLoc const&amp;, clang::TemplateArgumentLoc&amp;, bool) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->74 0x0000000007210d22 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformTemplateSpecializationType(clang::TypeLocBuilder&amp;, clang::TemplateSpecializationTypeLoc, clang::TemplateName) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->75 0x00000000071ff733 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeLocBuilder&amp;, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->76 0x000000000721ecc4 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformElaboratedType(clang::TypeLocBuilder&amp;, clang::ElaboratedTypeLoc) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->77 0x00000000071fe982 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeLocBuilder&amp;, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->78 0x000000000720543a clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeSourceInfo*) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->79 0x000000000720555c clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::QualType) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->80 0x0000000007207381 clang::Sema::SubstType(clang::QualType, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceLocation, clang::DeclarationName, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7207381)
 #<!-- -->81 0x00000000071231af clang::Sema::SubstituteExplicitTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo&amp;, llvm::SmallVectorImpl&lt;clang::DeducedTemplateArgument&gt;&amp;, llvm::SmallVectorImpl&lt;clang::QualType&gt;&amp;, clang::QualType*, clang::sema::TemplateDeductionInfo&amp;) (.constprop.0) SemaTemplateDeduction.cpp:0:0
 #<!-- -->82 0x0000000007123bb0 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, bool, bool, bool, clang::QualType, clang::Expr::Classification, bool, llvm::function_ref&lt;bool (llvm::ArrayRef&lt;clang::QualType&gt;)&gt;)::'lambda'()&gt;(long) SemaTemplateDeduction.cpp:0:0
 #<!-- -->83 0x0000000007f3a981 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7f3a981)
 #<!-- -->84 0x0000000007158f6c clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, bool, bool, bool, clang::QualType, clang::Expr::Classification, bool, llvm::function_ref&lt;bool (llvm::ArrayRef&lt;clang::QualType&gt;)&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7158f6c)
 #<!-- -->85 0x0000000006f832ab AddTemplateOverloadCandidateImmediately(clang::Sema&amp;, clang::OverloadCandidateSet&amp;, clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, bool, bool, bool, clang::CallExpr::ADLCallKind, clang::OverloadCandidateParamOrder, bool) SemaOverload.cpp:0:0
 #<!-- -->86 0x0000000006f85736 clang::Sema::AddTemplateOverloadCandidate(clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::OverloadCandidateSet&amp;, bool, bool, bool, clang::CallExpr::ADLCallKind, clang::OverloadCandidateParamOrder, bool) (.constprop.1) SemaOverload.cpp:0:0
 #<!-- -->87 0x0000000006f8593b AddOverloadedCallCandidate(clang::Sema&amp;, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::OverloadCandidateSet&amp;, bool, bool) SemaOverload.cpp:0:0
 #<!-- -->88 0x0000000006f870e1 clang::Sema::AddOverloadedCallCandidates(clang::UnresolvedLookupExpr*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::OverloadCandidateSet&amp;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f870e1)
 #<!-- -->89 0x0000000006f873de clang::Sema::buildOverloadedCallSet(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::OverloadCandidateSet*, clang::ActionResult&lt;clang::Expr*, true&gt;*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f873de)
 #<!-- -->90 0x0000000006f9c4c0 clang::Sema::BuildOverloadedCallExpr(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f9c4c0)
 #<!-- -->91 0x0000000006b94c86 clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b94c86)
 #<!-- -->92 0x0000000006b95e8e clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b95e8e)
 #<!-- -->93 0x00000000071fa29b clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCallExpr(clang::CallExpr*) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->94 0x00000000071ed6d8 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->95 0x00000000071fe717 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeLocBuilder&amp;, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->96 0x000000000720543a clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeSourceInfo*) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->97 0x0000000007213fae clang::Sema::SubstType(clang::TypeSourceInfo*, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceLocation, clang::DeclarationName, bool) (.constprop.0) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->98 0x00000000072144e5 clang::Sema::SubstBaseSpecifiers(clang::CXXRecordDecl*, clang::CXXRecordDecl*, clang::MultiLevelTemplateArgumentList const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x72144e5)
 #<!-- -->99 0x0000000007214f14 clang::Sema::InstantiateClass(clang::SourceLocation, clang::CXXRecordDecl*, clang::CXXRecordDecl*, clang::MultiLevelTemplateArgumentList const&amp;, clang::TemplateSpecializationKind, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7214f14)
#<!-- -->100 0x00000000072379d8 clang::Sema::InstantiateClassTemplateSpecialization(clang::SourceLocation, clang::ClassTemplateSpecializationDecl*, clang::TemplateSpecializationKind, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x72379d8)
#<!-- -->101 0x00000000072b567e void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*)::'lambda'()&gt;(long) SemaType.cpp:0:0
#<!-- -->102 0x0000000007f3a981 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7f3a981)
#<!-- -->103 0x00000000072be84a clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x72be84a)
#<!-- -->104 0x00000000072bee45 clang::Sema::RequireCompleteType(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x72bee45)
#<!-- -->105 0x00000000068512fc clang::Sema::RequireCompleteDeclContext(clang::CXXScopeSpec&amp;, clang::DeclContext*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68512fc)
#<!-- -->106 0x000000000708dd51 clang::Sema::CheckTypenameType(clang::ElaboratedTypeKeyword, clang::SourceLocation, clang::NestedNameSpecifierLoc, clang::IdentifierInfo const&amp;, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x708dd51)
#<!-- -->107 0x00000000071db2ee clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::RebuildDependentNameType(clang::ElaboratedTypeKeyword, clang::SourceLocation, clang::NestedNameSpecifierLoc, clang::IdentifierInfo const*, clang::SourceLocation, bool) SemaTemplateInstantiate.cpp:0:0
#<!-- -->108 0x000000000721292a clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformDependentNameType(clang::TypeLocBuilder&amp;, clang::DependentNameTypeLoc, bool) SemaTemplateInstantiate.cpp:0:0
#<!-- -->109 0x00000000071fe853 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeLocBuilder&amp;, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
#<!-- -->110 0x000000000720543a clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeSourceInfo*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->111 0x0000000007213fae clang::Sema::SubstType(clang::TypeSourceInfo*, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceLocation, clang::DeclarationName, bool) (.constprop.0) SemaTemplateInstantiate.cpp:0:0
#<!-- -->112 0x00000000072144e5 clang::Sema::SubstBaseSpecifiers(clang::CXXRecordDecl*, clang::CXXRecordDecl*, clang::MultiLevelTemplateArgumentList const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x72144e5)
#<!-- -->113 0x0000000007214f14 clang::Sema::InstantiateClass(clang::SourceLocation, clang::CXXRecordDecl*, clang::CXXRecordDecl*, clang::MultiLevelTemplateArgumentList const&amp;, clang::TemplateSpecializationKind, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7214f14)
#<!-- -->114 0x00000000072379d8 clang::Sema::InstantiateClassTemplateSpecialization(clang::SourceLocation, clang::ClassTemplateSpecializationDecl*, clang::TemplateSpecializationKind, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x72379d8)
#<!-- -->115 0x00000000072b567e void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*)::'lambda'()&gt;(long) SemaType.cpp:0:0
#<!-- -->116 0x0000000007f3a981 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7f3a981)
#<!-- -->117 0x00000000072be84a clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x72be84a)
#<!-- -->118 0x00000000072bee45 clang::Sema::RequireCompleteType(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x72bee45)
#<!-- -->119 0x00000000068512fc clang::Sema::RequireCompleteDeclContext(clang::CXXScopeSpec&amp;, clang::DeclContext*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68512fc)
#<!-- -->120 0x0000000006d9415f clang::Sema::LookupParsedName(clang::LookupResult&amp;, clang::Scope*, clang::CXXScopeSpec*, clang::QualType, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d9415f)
#<!-- -->121 0x0000000006b664f0 clang::Sema::BuildQualifiedDeclarationNameExpr(clang::CXXScopeSpec&amp;, clang::DeclarationNameInfo const&amp;, bool, clang::TypeSourceInfo**) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b664f0)
#<!-- -->122 0x00000000072193f6 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformDependentScopeDeclRefExpr(clang::DependentScopeDeclRefExpr*, bool, clang::TypeSourceInfo**) SemaTemplateInstantiate.cpp:0:0
#<!-- -->123 0x00000000071ed7a3 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->124 0x00000000071ef885 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformInitializer(clang::Expr*, bool) (.part.0) SemaTemplateInstantiate.cpp:0:0
#<!-- -->125 0x00000000071efe9b clang::Sema::SubstInitializer(clang::Expr*, clang::MultiLevelTemplateArgumentList const&amp;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x71efe9b)
#<!-- -->126 0x0000000007244096 clang::Sema::InstantiateVariableInitializer(clang::VarDecl*, clang::VarDecl*, clang::MultiLevelTemplateArgumentList const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7244096)
#<!-- -->127 0x000000000724450e clang::Sema::CompleteVarTemplateSpecializationDecl(clang::VarTemplateSpecializationDecl*, clang::VarDecl*, clang::MultiLevelTemplateArgumentList const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x724450e)
#<!-- -->128 0x0000000007294088 clang::Sema::InstantiateVariableDefinition(clang::SourceLocation, clang::VarDecl*, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7294088)
#<!-- -->129 0x0000000007f3a981 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7f3a981)
#<!-- -->130 0x0000000006b5f5c3 DoMarkVarDeclReferenced(clang::Sema&amp;, clang::SourceLocation, clang::VarDecl*, clang::Expr*, llvm::DenseMap&lt;clang::VarDecl const*, int, llvm::DenseMapInfo&lt;clang::VarDecl const*, void&gt;, llvm::detail::DenseMapPair&lt;clang::VarDecl const*, int&gt;&gt;&amp;) SemaExpr.cpp:0:0
#<!-- -->131 0x0000000006b6f7de MarkExprReferenced(clang::Sema&amp;, clang::SourceLocation, clang::Decl*, clang::Expr*, bool, llvm::DenseMap&lt;clang::VarDecl const*, int, llvm::DenseMapInfo&lt;clang::VarDecl const*, void&gt;, llvm::detail::DenseMapPair&lt;clang::VarDecl const*, int&gt;&gt;&amp;) SemaExpr.cpp:0:0
#<!-- -->132 0x0000000006b5fbd9 clang::Sema::BuildDeclRefExpr(clang::ValueDecl*, clang::QualType, clang::ExprValueKind, clang::DeclarationNameInfo const&amp;, clang::NestedNameSpecifierLoc, clang::NamedDecl*, clang::SourceLocation, clang::TemplateArgumentListInfo const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b5fbd9)
#<!-- -->133 0x0000000006b6038f clang::Sema::BuildDeclRefExpr(clang::ValueDecl*, clang::QualType, clang::ExprValueKind, clang::DeclarationNameInfo const&amp;, clang::CXXScopeSpec const*, clang::NamedDecl*, clang::SourceLocation, clang::TemplateArgumentListInfo const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b6038f)
#<!-- -->134 0x0000000006b65d43 clang::Sema::BuildDeclarationNameExpr(clang::CXXScopeSpec const&amp;, clang::DeclarationNameInfo const&amp;, clang::NamedDecl*, clang::NamedDecl*, clang::TemplateArgumentListInfo const*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b65d43)
#<!-- -->135 0x000000000709a7ab clang::Sema::BuildTemplateIdExpr(clang::CXXScopeSpec const&amp;, clang::SourceLocation, clang::LookupResult&amp;, bool, clang::TemplateArgumentListInfo const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x709a7ab)
#<!-- -->136 0x0000000006b67e78 clang::Sema::ActOnIdExpression(clang::Scope*, clang::CXXScopeSpec&amp;, clang::SourceLocation, clang::UnqualifiedId&amp;, bool, bool, clang::CorrectionCandidateCallback*, bool, clang::Token*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b67e78)
#<!-- -->137 0x00000000066ceea8 clang::Parser::tryParseCXXIdExpression(clang::CXXScopeSpec&amp;, bool, clang::Token&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66ceea8)
#<!-- -->138 0x00000000066cf1ca clang::Parser::ParseCXXIdExpression(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66cf1ca)
#<!-- -->139 0x00000000066b1a72 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&amp;, clang::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66b1a72)
#<!-- -->140 0x00000000066b122e clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&amp;, clang::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66b122e)
#<!-- -->141 0x00000000066bf14e clang::Parser::ParseConstantExpressionInExprEvalContext(clang::TypeCastState) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66bf14e)
#<!-- -->142 0x0000000006695869 clang::Parser::tryParseExceptionSpecification(bool, clang::SourceRange&amp;, llvm::SmallVectorImpl&lt;clang::OpaquePtr&lt;clang::QualType&gt;&gt;&amp;, llvm::SmallVectorImpl&lt;clang::SourceRange&gt;&amp;, clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, llvm::SmallVector&lt;clang::Token, 4u&gt;*&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6695869)
#<!-- -->143 0x00000000066d28b2 clang::Parser::ParseLambdaExpressionAfterIntroducer(clang::LambdaIntroducer&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66d28b2)
#<!-- -->144 0x00000000066d35fb clang::Parser::ParseLambdaExpression() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66d35fb)
#<!-- -->145 0x00000000066b2795 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&amp;, clang::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66b2795)
#<!-- -->146 0x00000000066b3157 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66b3157)
#<!-- -->147 0x00000000066b31e9 clang::Parser::ParseAssignmentExpression(clang::TypeCastState) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66b31e9)
#<!-- -->148 0x00000000066b7bc9 clang::Parser::ParseExpression(clang::TypeCastState) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66b7bc9)
#<!-- -->149 0x0000000006748559 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6748559)
#<!-- -->150 0x00000000067400eb clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67400eb)
#<!-- -->151 0x0000000006740f2d clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6740f2d)
#<!-- -->152 0x0000000006748c53 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6748c53)
#<!-- -->153 0x000000000674941a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x674941a)
#<!-- -->154 0x0000000006643433 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6643433)
#<!-- -->155 0x0000000006679dcd clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6679dcd)
#<!-- -->156 0x0000000006636f7e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6636f7e)
#<!-- -->157 0x0000000006637739 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6637739)
#<!-- -->158 0x000000000663f04a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x663f04a)
#<!-- -->159 0x000000000663ffed clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x663ffed)
#<!-- -->160 0x00000000066323ba clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66323ba)
#<!-- -->161 0x0000000004946b78 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4946b78)
#<!-- -->162 0x0000000004c3eb45 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c3eb45)
#<!-- -->163 0x0000000004bbd78e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bbd78e)
#<!-- -->164 0x0000000004d31629 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d31629)
#<!-- -->165 0x0000000000dace0f cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdace0f)
#<!-- -->166 0x0000000000da2fda ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->167 0x00000000049ade69 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->168 0x0000000003ede004 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3ede004)
#<!-- -->169 0x00000000049ae47f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->170 0x000000000497024d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x497024d)
#<!-- -->171 0x00000000049712de clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49712de)
#<!-- -->172 0x00000000049795c5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49795c5)
#<!-- -->173 0x0000000000da8d78 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda8d78)
#<!-- -->174 0x0000000000c2e774 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc2e774)
#<!-- -->175 0x00007a32f3a29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->176 0x00007a32f3a29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->177 0x0000000000da2a85 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda2a85)
```

## Program

```
#include &lt;iostream&gt;
using namespace std;
auto main() -&gt; int
{
    []() noexcept(is_nothrow_invocable_v&lt;decltype(
        [](){}
    )&gt;);
}
```

## To quickly reproduce

https://godbolt.org/z/9af5Phv5P
</details>


---

