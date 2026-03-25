# Compiler crash, fails to compile

**Author:** troldal
**URL:** https://github.com/llvm/llvm-project/issues/136432
**Status:** Closed
**Labels:** clang:frontend, confirmed, crash, lambda, regression:19
**Closed Date:** 2025-05-02T06:42:30Z

## Body

Clang crashes when compiling my code. As instructed in the failure message, here is the backtrace, preprocessed source, and associated run script:

[Flash-90296d.zip](https://github.com/user-attachments/files/19821407/Flash-90296d.zip)

## Comments

### Comment 1 - Alcaro

You sure don't do half measures in your bug reports, do you? That's over nine megabytes.

Here's a reduced version.
```c++
namespace std
{
    template<typename _Functor, typename _ArgTypes>
    struct invoke_result
    { 
        using type = decltype(_Functor()(_ArgTypes()));
    };

    template<typename _Fn, typename _Args>
    using invoke_result_t = typename invoke_result<_Fn, _Args>::type;
}

template<typename T1, typename T2>
struct eee {
    int b() const { return 5; }
};

template<typename T, typename E>
auto append(const eee<T, E>& val)
{
    return [val]<template<typename,typename> class TExpectedOut>(const TExpectedOut<int, int>& tuple)
    {
        return 5;
    };
}
eee<int, int> CreateEOS(int arg) { return {}; }

void flash()
{
    auto apply = []<typename TError>(const eee<int, TError>& tuple) {
         append(CreateEOS(tuple.b()))(tuple);
    };
    std::invoke_result_t<decltype(apply), eee<int, int>>* f;
}
```
https://godbolt.org/z/qGEvxrhf9

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Kenneth Troldal Balslev (troldal)

<details>
Clang crashes when compiling my code. As instructed in the failure message, here is the backtrace, preprocessed source, and associated run script:

[Flash-90296d.zip](https://github.com/user-attachments/files/19821407/Flash-90296d.zip)
</details>


---

### Comment 3 - troldal

Sorry about that; I honestly didn't notice that it was a 9 MB attachment. The failure message instructed to send the whole lot, so I did.  If it happens again, I'll boil it down to the essentials.

---

### Comment 4 - shafik

Looks like a clang-19 regression: https://godbolt.org/z/Kox7z8EqW

Assertion:

```console
clang++: /root/llvm-project/llvm/tools/clang/lib/AST/DeclCXX.cpp:1699:
clang::NamedDecl* getLambdaCallOperatorHelper(const clang::CXXRecordDecl&):
Assertion `!Calls.empty() && "Missing lambda call operator!"' failed.
```

Backtrace:

```console
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 <source>
1.	<source>:33:10: at annotation token
2.	<source>:29:1: parsing function body 'flash'
3.	<source>:29:1: in compound statement ('{}')
4.	<source>:4:12: instantiating class definition 'std::invoke_result<(lambda at <source>:30:18), eee<int, int>>'
5.	<source>:30:18: instantiating function definition 'flash()::(anonymous class)::operator()<int>'
6.	<source>:19:6: instantiating function definition 'append<int, int>'
  #0 0x0000000003ef1728 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3ef1728)
  #1 0x0000000003eef3b4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3eef3b4)
  #2 0x0000000003e33ef8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
  #3 0x000073e000e42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
  #4 0x000073e000e969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
  #5 0x000073e000e42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
  #6 0x000073e000e287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
  #7 0x000073e000e2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
  #8 0x000073e000e39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
  #9 0x0000000007653ceb getLambdaCallOperatorHelper(clang::CXXRecordDecl const&) (.part.0) DeclCXX.cpp:0:0
 #10 0x000000000765471b clang::CXXRecordDecl::getLambdaCallOperator() const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x765471b)
 #11 0x00000000070c0329 clang::Sema::getTemplateInstantiationArgs(clang::NamedDecl const*, clang::DeclContext const*, bool, std::optional<llvm::ArrayRef<clang::TemplateArgument>>, bool, clang::FunctionDecl const*, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x70c0329)
 #12 0x0000000006f7f245 clang::Sema::CheckTemplateArgumentList(clang::TemplateDecl*, clang::SourceLocation, clang::TemplateArgumentListInfo&, clang::DefaultArguments const&, bool, clang::Sema::CheckTemplateArgumentInfo&, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f7f245)
 #13 0x0000000006f82d95 clang::Sema::CheckTemplateIdType(clang::TemplateName, clang::SourceLocation, clang::TemplateArgumentListInfo&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f82d95)
 #14 0x00000000070fb5ec clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformTemplateSpecializationType(clang::TypeLocBuilder&, clang::TemplateSpecializationTypeLoc, clang::TemplateName) SemaTemplateInstantiate.cpp:0:0
 #15 0x00000000070e91fb clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
 #16 0x00000000071085c4 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformElaboratedType(clang::TypeLocBuilder&, clang::ElaboratedTypeLoc) SemaTemplateInstantiate.cpp:0:0
 #17 0x00000000070e8444 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
 #18 0x00000000070ea69d clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformQualifiedType(clang::TypeLocBuilder&, clang::QualifiedTypeLoc) SemaTemplateInstantiate.cpp:0:0
 #19 0x00000000070e7d4c clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
 #20 0x00000000070ee67d clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformReferenceType(clang::TypeLocBuilder&, clang::ReferenceTypeLoc) SemaTemplateInstantiate.cpp:0:0
 #21 0x00000000070e87cb clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
 #22 0x00000000070eee3a clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeSourceInfo*) SemaTemplateInstantiate.cpp:0:0
 #23 0x00000000070fd8ae clang::Sema::SubstType(clang::TypeSourceInfo*, clang::MultiLevelTemplateArgumentList const&, clang::SourceLocation, clang::DeclarationName, bool) (.constprop.0) SemaTemplateInstantiate.cpp:0:0
 #24 0x000000000710b2ca clang::Sema::SubstParmVarDecl(clang::ParmVarDecl*, clang::MultiLevelTemplateArgumentList const&, int, clang::UnsignedOrNone, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x710b2ca)
 #25 0x000000000710c2a6 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformFunctionTypeParams(clang::SourceLocation, llvm::ArrayRef<clang::ParmVarDecl*>, clang::QualType const*, clang::FunctionType::ExtParameterInfo const*, llvm::SmallVectorImpl<clang::QualType>&, llvm::SmallVectorImpl<clang::ParmVarDecl*>*, clang::Sema::ExtParameterInfoBuilder&, unsigned int*) SemaTemplateInstantiate.cpp:0:0
 #26 0x00000000070e730c clang::QualType clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformFunctionProtoType<clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformFunctionProtoType(clang::TypeLocBuilder&, clang::FunctionProtoTypeLoc)::'lambda'(clang::FunctionProtoType::ExceptionSpecInfo&, bool&)>(clang::TypeLocBuilder&, clang::FunctionProtoTypeLoc, clang::CXXRecordDecl*, clang::Qualifiers, clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformFunctionProtoType(clang::TypeLocBuilder&, clang::FunctionProtoTypeLoc)::'lambda'(clang::FunctionProtoType::ExceptionSpecInfo&, bool&)) (.constprop.0) SemaTemplateInstantiate.cpp:0:0
 #27 0x00000000070e85b7 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
 #28 0x00000000070d571b clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformLambdaExpr(clang::LambdaExpr*) SemaTemplateInstantiate.cpp:0:0
 #29 0x00000000070d6e42 (anonymous namespace)::TemplateInstantiator::TransformLambdaExpr(clang::LambdaExpr*) SemaTemplateInstantiate.cpp:0:0
 #30 0x00000000070d73c1 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
 #31 0x00000000070d93e5 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformInitializer(clang::Expr*, bool) (.part.0) SemaTemplateInstantiate.cpp:0:0
 #32 0x00000000070d978c clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformReturnStmt(clang::ReturnStmt*) SemaTemplateInstantiate.cpp:0:0
 #33 0x000000000711791e clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaTemplateInstantiate.cpp:0:0
 #34 0x0000000007120c24 clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7120c24)
 #35 0x00000000071810d7 clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x71810d7)
 #36 0x0000000007e0cba1 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7e0cba1)
 #37 0x00000000070114ba clang::Sema::DeduceReturnType(clang::FunctionDecl*, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x70114ba)
 #38 0x0000000006a45a13 clang::Sema::DiagnoseUseOfDecl(clang::NamedDecl*, llvm::ArrayRef<clang::SourceLocation>, clang::ObjCInterfaceDecl const*, bool, bool, clang::ObjCInterfaceDecl*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a45a13)
 #39 0x0000000006e8c9b4 FinishOverloadedCallExpr(clang::Sema&, clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, clang::OverloadCandidateSet*, clang::OverloadCandidate**, clang::OverloadingResult, bool) SemaOverload.cpp:0:0
 #40 0x0000000006e8d84f clang::Sema::BuildOverloadedCallExpr(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e8d84f)
 #41 0x0000000006a8da86 clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a8da86)
 #42 0x0000000006a8e90e clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a8e90e)
 #43 0x00000000070e3f2b clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCallExpr(clang::CallExpr*) SemaTemplateInstantiate.cpp:0:0
 #44 0x00000000070d723a clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
 #45 0x00000000070e3e26 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCallExpr(clang::CallExpr*) SemaTemplateInstantiate.cpp:0:0
 #46 0x00000000070d723a clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
 #47 0x0000000007117087 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformStmt(clang::Stmt*, clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::StmtDiscardKind) SemaTemplateInstantiate.cpp:0:0
 #48 0x000000000711791e clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaTemplateInstantiate.cpp:0:0
 #49 0x0000000007120c24 clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7120c24)
 #50 0x00000000071810d7 clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x71810d7)
 #51 0x0000000007e0cba1 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7e0cba1)
 #52 0x00000000070114ba clang::Sema::DeduceReturnType(clang::FunctionDecl*, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x70114ba)
 #53 0x0000000006a45a13 clang::Sema::DiagnoseUseOfDecl(clang::NamedDecl*, llvm::ArrayRef<clang::SourceLocation>, clang::ObjCInterfaceDecl const*, bool, bool, clang::ObjCInterfaceDecl*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a45a13)
 #54 0x0000000006e51ce7 CreateFunctionRefExpr(clang::Sema&, clang::FunctionDecl*, clang::NamedDecl*, clang::Expr const*, bool, clang::SourceLocation, clang::DeclarationNameLoc const&) SemaOverload.cpp:0:0
 #55 0x0000000006e93c46 clang::Sema::BuildCallToObjectOfClassType(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e93c46)
 #56 0x0000000006a8e00e clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a8e00e)
 #57 0x0000000006a8e90e clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a8e90e)
 #58 0x00000000070e3f2b clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCallExpr(clang::CallExpr*) SemaTemplateInstantiate.cpp:0:0
 #59 0x00000000070d723a clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
 #60 0x00000000070e81e7 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
 #61 0x00000000070eee3a clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeSourceInfo*) SemaTemplateInstantiate.cpp:0:0
 #62 0x00000000070fc9b6 clang::Sema::SubstType(clang::TypeSourceInfo*, clang::MultiLevelTemplateArgumentList const&, clang::SourceLocation, clang::DeclarationName, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x70fc9b6)
 #63 0x000000000718aea1 clang::TemplateDeclInstantiator::InstantiateTypedefNameDecl(clang::TypedefNameDecl*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x718aea1)
 #64 0x000000000718b5e4 clang::TemplateDeclInstantiator::VisitTypeAliasDecl(clang::TypeAliasDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x718b5e4)
 #65 0x00000000070fe9ab clang::Sema::InstantiateClass(clang::SourceLocation, clang::CXXRecordDecl*, clang::CXXRecordDecl*, clang::MultiLevelTemplateArgumentList const&, clang::TemplateSpecializationKind, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x70fe9ab)
 #66 0x00000000071212b0 clang::Sema::InstantiateClassTemplateSpecialization(clang::SourceLocation, clang::ClassTemplateSpecializationDecl*, clang::TemplateSpecializationKind, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x71212b0)
 #67 0x000000000719fbce void llvm::function_ref<void ()>::callback_fn<clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*)::'lambda'()>(long) SemaType.cpp:0:0
 #68 0x0000000007e0cba1 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7e0cba1)
 #69 0x00000000071a89ba clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x71a89ba)
 #70 0x00000000071a8fb5 clang::Sema::RequireCompleteType(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x71a8fb5)
 #71 0x000000000675210c clang::Sema::RequireCompleteDeclContext(clang::CXXScopeSpec&, clang::DeclContext*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x675210c)
 #72 0x0000000006f7aab1 clang::Sema::CheckTypenameType(clang::ElaboratedTypeKeyword, clang::SourceLocation, clang::NestedNameSpecifierLoc, clang::IdentifierInfo const&, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f7aab1)
 #73 0x00000000070c4dce clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::RebuildDependentNameType(clang::ElaboratedTypeKeyword, clang::SourceLocation, clang::NestedNameSpecifierLoc, clang::IdentifierInfo const*, clang::SourceLocation, bool) SemaTemplateInstantiate.cpp:0:0
 #74 0x00000000070fc22a clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformDependentNameType(clang::TypeLocBuilder&, clang::DependentNameTypeLoc, bool) SemaTemplateInstantiate.cpp:0:0
 #75 0x00000000070e8315 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
 #76 0x00000000070eee3a clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeSourceInfo*) SemaTemplateInstantiate.cpp:0:0
 #77 0x00000000070eef5c clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::QualType) SemaTemplateInstantiate.cpp:0:0
 #78 0x00000000070f0c81 clang::Sema::SubstType(clang::QualType, clang::MultiLevelTemplateArgumentList const&, clang::SourceLocation, clang::DeclarationName, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x70f0c81)
 #79 0x0000000006f8317c clang::Sema::CheckTemplateIdType(clang::TemplateName, clang::SourceLocation, clang::TemplateArgumentListInfo&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f8317c)
 #80 0x0000000006f85bc7 clang::Sema::ActOnTemplateIdType(clang::Scope*, clang::CXXScopeSpec&, clang::SourceLocation, clang::OpaquePtr<clang::TemplateName>, clang::IdentifierInfo const*, clang::SourceLocation, clang::SourceLocation, llvm::MutableArrayRef<clang::ParsedTemplateArgument>, clang::SourceLocation, bool, bool, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f85bc7)
 #81 0x000000000665430b clang::Parser::AnnotateTemplateIdTokenAsType(clang::CXXScopeSpec&, clang::ImplicitTypenameContext, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x665430b)
 #82 0x0000000006545430 clang::Parser::TryAnnotateTypeOrScopeTokenAfterScopeSpec(clang::CXXScopeSpec&, bool, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6545430)
 #83 0x0000000006547f4e clang::Parser::TryAnnotateTypeOrScopeToken(clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6547f4e)
 #84 0x000000000666616b clang::Parser::isCXXDeclarationSpecifier(clang::ImplicitTypenameContext, clang::Parser::TPResult, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x666616b)
 #85 0x000000000666b9f7 clang::Parser::isCXXDeclarationStatement(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x666b9f7)
 #86 0x0000000006640592 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6640592)
 #87 0x00000000066418fd clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66418fd)
 #88 0x0000000006649593 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6649593)
 #89 0x0000000006649d5a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6649d5a)
 #90 0x0000000006552db3 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6552db3)
 #91 0x0000000006588dfd clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6588dfd)
 #92 0x000000000654692e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x654692e)
 #93 0x00000000065470e9 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65470e9)
 #94 0x000000000654e9da clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x654e9da)
 #95 0x000000000654f96d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x654f96d)
 #96 0x0000000006541d8a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6541d8a)
 #97 0x000000000488f1c8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x488f1c8)
 #98 0x0000000004b81c05 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b81c05)
 #99 0x0000000004b00bde clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b00bde)
#100 0x0000000004c70e6e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c70e6e)
#101 0x0000000000d9decf cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd9decf)
#102 0x0000000000d955da ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#103 0x00000000048f57c9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#104 0x0000000003e34394 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3e34394)
#105 0x00000000048f5ddf clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#106 0x00000000048b82fd clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48b82fd)
#107 0x00000000048b938e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48b938e)
#108 0x00000000048c11b5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48c11b5)
#109 0x0000000000d9ac18 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd9ac18)
#110 0x0000000000c229f4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc229f4)
#111 0x000073e000e29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#112 0x000073e000e29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#113 0x0000000000d95085 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd95085)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

---

### Comment 5 - shafik

@zyn0217 git bisect points to 843cc474faefad1d639f4c44c1cf3ad7dbda76c8

---

