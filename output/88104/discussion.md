# [clang] lambda with template parameter crashed in unevaluated context

**Author:** jcsxky
**URL:** https://github.com/llvm/llvm-project/issues/88104
**Status:** Closed
**Labels:** clang:frontend, crash, lambda
**Closed Date:** 2025-12-09T02:47:02Z

## Body

```cpp
template<auto F> constexpr auto x = F();
template<class> constexpr int a() { return 1; }

template <class>
struct A {
    template<typename U>
    using Func = decltype(
    []<class T>(T) {
        return x<[] constexpr { return a<T>(); }>;
    }.template operator()<unsigned long long>('2')
    );
};
A<short>::Func<int> y;
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Qizhi Hu (jcsxky)

<details>
```cpp
template&lt;auto F&gt; constexpr auto x = F();
template&lt;class&gt; constexpr int a() { return 1; }

template &lt;class&gt;
struct A {
    template&lt;typename U&gt;
    using Func = decltype(
    []&lt;class T&gt;(T) {
        return x&lt;[] constexpr { return a&lt;T&gt;(); }&gt;;
    }.template operator()&lt;unsigned long long&gt;('2')
    );
};
A&lt;short&gt;::Func&lt;int&gt; y;
```
</details>


---

### Comment 2 - shafik

Confirmed: https://godbolt.org/z/49d4dz3Pf

Assertion:

```console
clang++: /root/llvm-project/clang/lib/AST/Decl.cpp:2561:
clang::APValue* clang::VarDecl::evaluateValueImpl(llvm::SmallVectorImpl<std::pair<clang::SourceLocation, clang::PartialDiagnostic> >&, bool) const:
Assertion `!Init->isValueDependent()' failed.
```

Backtrace:

```console
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++23 <source>
1.	<source>:13:11: current parser token 'Func'
2.	<source>:5:8: instantiating class definition 'A<short>'
3.	<source>:1:33: Generating code for declaration 'x'
 #0 0x0000000003933728 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3933728)
 #1 0x000000000393140c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x393140c)
 #2 0x00000000038779b8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007f40b3a42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007f40b3a969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007f40b3a42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007f40b3a287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007f40b3a2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007f40b3a39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000007359e43 (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7359e43)
#10 0x0000000007359ecb clang::VarDecl::evaluateValue() const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7359ecb)
#11 0x0000000003c3835b clang::CodeGen::ConstantEmitter::tryEmitPrivateForVarInit(clang::VarDecl const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3c3835b)
#12 0x0000000003c386f1 clang::CodeGen::ConstantEmitter::tryEmitForInitializer(clang::VarDecl const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3c386f1)
#13 0x0000000003d0ce97 clang::CodeGen::CodeGenModule::EmitGlobalVarDefinition(clang::VarDecl const*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3d0ce97)
#14 0x0000000003d2fba9 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3d2fba9)
#15 0x0000000003d30c10 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3d30c10)
#16 0x0000000003d3a6fb clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#17 0x0000000006de2eb8 clang::Sema::InstantiateVariableDefinition(clang::SourceLocation, clang::VarDecl*, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6de2eb8)
#18 0x00000000063352f1 clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x63352f1)
#19 0x000000000674d703 DoMarkVarDeclReferenced(clang::Sema&, clang::SourceLocation, clang::VarDecl*, clang::Expr*, llvm::DenseMap<clang::VarDecl const*, int, llvm::DenseMapInfo<clang::VarDecl const*, void>, llvm::detail::DenseMapPair<clang::VarDecl const*, int>>&) SemaExpr.cpp:0:0
#20 0x0000000006778f91 MarkExprReferenced(clang::Sema&, clang::SourceLocation, clang::Decl*, clang::Expr*, bool, llvm::DenseMap<clang::VarDecl const*, int, llvm::DenseMapInfo<clang::VarDecl const*, void>, llvm::detail::DenseMapPair<clang::VarDecl const*, int>>&) SemaExpr.cpp:0:0
#21 0x00000000067795c9 clang::Sema::BuildDeclRefExpr(clang::ValueDecl*, clang::QualType, clang::ExprValueKind, clang::DeclarationNameInfo const&, clang::NestedNameSpecifierLoc, clang::NamedDecl*, clang::SourceLocation, clang::TemplateArgumentListInfo const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67795c9)
#22 0x0000000006779dbf clang::Sema::BuildDeclRefExpr(clang::ValueDecl*, clang::QualType, clang::ExprValueKind, clang::DeclarationNameInfo const&, clang::CXXScopeSpec const*, clang::NamedDecl*, clang::SourceLocation, clang::TemplateArgumentListInfo const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6779dbf)
#23 0x000000000677fee4 clang::Sema::BuildDeclarationNameExpr(clang::CXXScopeSpec const&, clang::DeclarationNameInfo const&, clang::NamedDecl*, clang::NamedDecl*, clang::TemplateArgumentListInfo const*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x677fee4)
#24 0x0000000006be9a68 clang::Sema::BuildTemplateIdExpr(clang::CXXScopeSpec const&, clang::SourceLocation, clang::LookupResult&, bool, clang::TemplateArgumentListInfo const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6be9a68)
#25 0x0000000006d77769 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformUnresolvedLookupExpr(clang::UnresolvedLookupExpr*) SemaTemplateInstantiate.cpp:0:0
#26 0x0000000006d54969 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#27 0x0000000006d56170 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformInitializer(clang::Expr*, bool) (.part.0) SemaTemplateInstantiate.cpp:0:0
#28 0x0000000006d5654c clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformReturnStmt(clang::ReturnStmt*) SemaTemplateInstantiate.cpp:0:0
#29 0x0000000006d8e194 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaTemplateInstantiate.cpp:0:0
#30 0x0000000006d53a25 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformLambdaExpr(clang::LambdaExpr*) SemaTemplateInstantiate.cpp:0:0
#31 0x0000000006d5473f clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#32 0x0000000006d881cb clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCXXDependentScopeMemberExpr(clang::CXXDependentScopeMemberExpr*) SemaTemplateInstantiate.cpp:0:0
#33 0x0000000006d5443f clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#34 0x0000000006d5fd01 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCallExpr(clang::CallExpr*) SemaTemplateInstantiate.cpp:0:0
#35 0x0000000006d54526 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#36 0x0000000006d6385a clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
#37 0x0000000006d6989a clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeSourceInfo*) SemaTemplateInstantiate.cpp:0:0
#38 0x0000000006d7381a clang::Sema::SubstType(clang::TypeSourceInfo*, clang::MultiLevelTemplateArgumentList const&, clang::SourceLocation, clang::DeclarationName, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d7381a)
#39 0x0000000006dedd91 clang::TemplateDeclInstantiator::InstantiateTypedefNameDecl(clang::TypedefNameDecl*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6dedd91)
#40 0x0000000006df05fe clang::TemplateDeclInstantiator::VisitTypeAliasTemplateDecl(clang::TypeAliasTemplateDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6df05fe)
#41 0x0000000006d749cb clang::Sema::InstantiateClass(clang::SourceLocation, clang::CXXRecordDecl*, clang::CXXRecordDecl*, clang::MultiLevelTemplateArgumentList const&, clang::TemplateSpecializationKind, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d749cb)
#42 0x0000000006d940d2 clang::Sema::InstantiateClassTemplateSpecialization(clang::SourceLocation, clang::ClassTemplateSpecializationDecl*, clang::TemplateSpecializationKind, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d940d2)
#43 0x0000000006e1f43f void llvm::function_ref<void ()>::callback_fn<clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*)::'lambda'()>(long) SemaType.cpp:0:0
#44 0x00000000063352f1 clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x63352f1)
#45 0x0000000006e299c7 clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e299c7)
#46 0x0000000006e29d15 clang::Sema::RequireCompleteType(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e29d15)
#47 0x000000000637cffc clang::Sema::RequireCompleteDeclContext(clang::CXXScopeSpec&, clang::DeclContext*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x637cffc)
#48 0x0000000006bc218e clang::Sema::LookupTemplateName(clang::LookupResult&, clang::Scope*, clang::CXXScopeSpec&, clang::QualType, bool, bool&, clang::Sema::RequiredTemplateKind, clang::Sema::AssumedTemplateKind*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bc218e)
#49 0x0000000006bc36c7 clang::Sema::isTemplateName(clang::Scope*, clang::CXXScopeSpec&, bool, clang::UnqualifiedId const&, clang::OpaquePtr<clang::QualType>, bool, clang::OpaquePtr<clang::TemplateName>&, bool&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bc36c7)
#50 0x000000000625fbf1 clang::Parser::ParseOptionalCXXScopeSpecifier(clang::CXXScopeSpec&, clang::OpaquePtr<clang::QualType>, bool, bool, bool*, bool, clang::IdentifierInfo**, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x625fbf1)
#51 0x00000000061d0c23 clang::Parser::TryAnnotateCXXScopeToken(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x61d0c23)
#52 0x0000000006217ff0 clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&, clang::Parser::ParsedTemplateInfo const&, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6217ff0)
#53 0x00000000061d11e7 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x61d11e7)
#54 0x00000000061d1be3 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x61d1be3)
#55 0x00000000061d9056 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x61d9056)
#56 0x00000000061d9efd clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x61d9efd)
#57 0x00000000061ccbda clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x61ccbda)
#58 0x00000000041de908 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41de908)
#59 0x000000000445ccf9 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x445ccf9)
#60 0x00000000043e65be clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x43e65be)
#61 0x000000000453fcfe clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x453fcfe)
#62 0x0000000000c4367c cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc4367c)
#63 0x0000000000c3c90a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#64 0x0000000004221839 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#65 0x0000000003877e64 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3877e64)
#66 0x0000000004221e2f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#67 0x00000000041e8225 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41e8225)
#68 0x00000000041e8c8d clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41e8c8d)
#69 0x00000000041f0a05 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41f0a05)
#70 0x0000000000c40b15 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc40b15)
#71 0x0000000000b23be4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xb23be4)
#72 0x00007f40b3a29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#73 0x00007f40b3a29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#74 0x0000000000c3c3fe _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc3c3fe)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

Looks similar to: https://github.com/llvm/llvm-project/issues/86279

CC @cor3ntin 

---

### Comment 3 - zyn0217

Simplified version

```cpp
template <auto F> constexpr auto x = F();

template <class = int>
struct S {
  using Type = decltype([]<class T>(T) { return x<[] { return T(); }>; });
};

S<>::Type y;

```

This used to compile in clang 15 but was broken since 16.

https://gcc.godbolt.org/z/eM8de5sc8

---

### Comment 4 - zyn0217

I'm almost 100% sure that there are two issues behind this problem: one is the dependency of the lambda expression, that the lambda as the NTTP argument is supposed to be dependent at the time the outer class template is instantiated, but it doesn't work that way, which results in the current crash in codegen; although this isn't a codegen bug because we have guards in `Sema::CheckVarTemplateId` that prevent dependent variable templates from getting instantiated/evaluated.

https://github.com/llvm/llvm-project/blob/26852565a5f609e6b466f43c2f690ce3047d04c7/clang/lib/Sema/SemaTemplate.cpp#L5308-L5312

The second issue hides in `ActOnUninitializedDecl`, I think. If we had fixed the first problem, then the type of `y` would have been dependent at the time `y`'s initializer being required. Which soon hits the assertion in codegen that a dependent type is impossible to emit.

---

### Comment 5 - shafik

Fixed in trunk: https://godbolt.org/z/49d4dz3Pf

CC @zyn0217 

---

