# ICE on clang: Assertion `!Calls.empty() && "Missing lambda call operator!"'

**Author:** jcelerier
**URL:** https://github.com/llvm/llvm-project/issues/138018
**Status:** Closed
**Labels:** clang:frontend, crash, lambda
**Closed Date:** 2025-05-02T06:42:31Z

## Body

> clang HEAD from a couple hours ago (git@github.com:llvm/llvm-project e79d8f6892aa138bfef65334817c24d3d9881855) crashes:
> 
> [kabang_avnd-8e80cb.zip](https://github.com/user-attachments/files/19841836/kabang_avnd-8e80cb.zip) 

 _Originally posted by @jcelerier in [#136635](https://github.com/llvm/llvm-project/issues/136635#issuecomment-2819810879)_


Reduced:
```cpp
struct foo { };

template <typename T>
struct vec { };

auto structure_to_typelist(const auto& s) noexcept {
  return []<template <typename...> typename T, typename... Args>(T<Args...>) {
    return 0;
  }(vec<int>{});
}

template <typename T>
using helper2 = decltype(structure_to_typelist(T{}));
auto tl_ok2 = helper2<foo>{};

```

## Comments

### Comment 1 - jcelerier

```
2.	D:/dev/avendish/tests/test_introspection_rec.cpp:10:6: instantiating function definition 'structure_to_typelist<foo>'
Exception Code: 0xC0000005
 #0 0x00007ff841d0d4b1 clang::CXXRecordDecl::getLambdaStaticInvoker() const (D:\msys64\clang64\bin\libclang-cpp.dll+0x44d4b1)
 #1 0x00007ff842017d09 clang::CXXRecordDecl::getLambdaCallOperator() const (D:\msys64\clang64\bin\libclang-cpp.dll+0x757d09)
 #2 0x00007ff841ab2451 clang::Sema::getTemplateInstantiationArgs(clang::NamedDecl const*, clang::DeclContext const*, bool, std::__1::optional<llvm::ArrayRef<clang::TemplateArgument>>, bool, clang::FunctionDecl const*, bool, bool, bool) (D:\msys64\clang64\bin\libclang-cpp.dll+0x1f2451)
 #3 0x00007ff841a96bb9 clang::Sema::CheckTemplateIdType(clang::TemplateName, clang::SourceLocation, clang::TemplateArgumentListInfo&) (D:\msys64\clang64\bin\libclang-cpp.dll+0x1d6bb9)
 #4 0x00007ff841a6596f clang::FunctionProtoType::FunctionProtoType(clang::QualType, llvm::ArrayRef<clang::QualType>, clang::QualType, clang::FunctionProtoType::ExtProtoInfo const&) (D:\msys64\clang64\bin\libclang-cpp.dll+0x1a596f)
 #5 0x00007ff841a654c1 clang::FunctionProtoType::FunctionProtoType(clang::QualType, llvm::ArrayRef<clang::QualType>, clang::QualType, clang::FunctionProtoType::ExtProtoInfo const&) (D:\msys64\clang64\bin\libclang-cpp.dll+0x1a54c1)
 #6 0x00007ff842043c6d std::__1::optional<llvm::APSInt>& std::__1::optional<llvm::APSInt>::operator=[abi:nn200100]<llvm::APSInt&, void>(llvm::APSInt&) (D:\msys64\clang64\bin\libclang-cpp.dll+0x783c6d)
 #7 0x00007ff8420429b0 std::__1::optional<llvm::APSInt>& std::__1::optional<llvm::APSInt>::operator=[abi:nn200100]<llvm::APSInt&, void>(llvm::APSInt&) (D:\msys64\clang64\bin\libclang-cpp.dll+0x7829b0)
 #8 0x00007ff841a670aa clang::FunctionProtoType::FunctionProtoType(clang::QualType, llvm::ArrayRef<clang::QualType>, clang::QualType, clang::FunctionProtoType::ExtProtoInfo const&) (D:\msys64\clang64\bin\libclang-cpp.dll+0x1a70aa)
 #9 0x00007ff8420910f7 clang::Sema::BuiltinEnumUnderlyingType(clang::QualType, clang::SourceLocation) (D:\msys64\clang64\bin\libclang-cpp.dll+0x7d10f7)
#10 0x00007ff841a719f2 clang::VarDecl::isUsableInConstantExpressions(clang::ASTContext const&) const (D:\msys64\clang64\bin\libclang-cpp.dll+0x1b19f2)
#11 0x00007ff841a7834b clang::Sema::ActOnParenListExpr(clang::SourceLocation, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>) (D:\msys64\clang64\bin\libclang-cpp.dll+0x1b834b)
#12 0x00007ff841dfa6f9 clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&) (D:\msys64\clang64\bin\libclang-cpp.dll+0x53a6f9)
#13 0x00007ff841df9ced clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&) (D:\msys64\clang64\bin\libclang-cpp.dll+0x539ced)
#14 0x00007ff841df92b9 clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&) (D:\msys64\clang64\bin\libclang-cpp.dll+0x5392b9)
#15 0x00007ff841df6669 clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (D:\msys64\clang64\bin\libclang-cpp.dll+0x536669)
#16 0x00007ff8422319c9 clang::Sema::PerformQualificationConversion(clang::Expr*, clang::QualType, clang::ExprValueKind, clang::CheckedConversionKind) (D:\msys64\clang64\bin\libclang-cpp.dll+0x9719c9)
#17 0x00007ff841cb5be1 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (D:\msys64\clang64\bin\libclang-cpp.dll+0x3f5be1)
#18 0x00007ff841cb5b81 clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (D:\msys64\clang64\bin\libclang-cpp.dll+0x3f5b81)
#19 0x00007ff841fcba90 clang::Sema::DeduceReturnType(clang::FunctionDecl*, clang::SourceLocation, bool) (D:\msys64\clang64\bin\libclang-cpp.dll+0x70ba90)
#20 0x00007ff841aac375 clang::Sema::DiagnoseUseOfDecl(clang::NamedDecl*, llvm::ArrayRef<clang::SourceLocation>, clang::ObjCInterfaceDecl const*, bool, bool, clang::ObjCInterfaceDecl*, bool) (D:\msys64\clang64\bin\libclang-cpp.dll+0x1ec375)
#21 0x00007ff841969f9c clang::Sema::BuildOverloadedCallExpr(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (D:\msys64\clang64\bin\libclang-cpp.dll+0xa9f9c)
#22 0x00007ff8419652f9 clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) (D:\msys64\clang64\bin\libclang-cpp.dll+0xa52f9)
#23 0x00007ff841a78555 clang::Sema::ActOnParenListExpr(clang::SourceLocation, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>) (D:\msys64\clang64\bin\libclang-cpp.dll+0x1b8555)
#24 0x00007ff841a7a490 clang::Expr::isDefaultArgument() const (D:\msys64\clang64\bin\libclang-cpp.dll+0x1ba490)
#25 0x00007ff841ac8288 clang::NestedNameSpecifier::Create(clang::ASTContext const&, clang::NestedNameSpecifier*, clang::NamespaceDecl const*) (D:\msys64\clang64\bin\libclang-cpp.dll+0x208288)
#26 0x00007ff841aa49b8 clang::Sema::SubstType(clang::QualType, clang::MultiLevelTemplateArgumentList const&, clang::SourceLocation, clang::DeclarationName, bool*) (D:\msys64\clang64\bin\libclang-cpp.dll+0x1e49b8)
#27 0x00007ff841a972c7 clang::Sema::CheckTemplateIdType(clang::TemplateName, clang::SourceLocation, clang::TemplateArgumentListInfo&) (D:\msys64\clang64\bin\libclang-cpp.dll+0x1d72c7)
#28 0x00007ff8419691e4 clang::Sema::ActOnTemplateIdType(clang::Scope*, clang::CXXScopeSpec&, clang::SourceLocation, clang::OpaquePtr<clang::TemplateName>, clang::IdentifierInfo const*, clang::SourceLocation, clang::SourceLocation, llvm::MutableArrayRef<clang::ParsedTemplateArgument>, clang::SourceLocation, bool, bool, clang::ImplicitTypenameContext) (D:\msys64\clang64\bin\libclang-cpp.dll+0xa91e4)
#29 0x00007ff841961c72 clang::Parser::AnnotateTemplateIdTokenAsType(clang::CXXScopeSpec&, clang::ImplicitTypenameContext, bool) (D:\msys64\clang64\bin\libclang-cpp.dll+0xa1c72)
#30 0x00007ff84195ed1f clang::Parser::TryAnnotateTypeOrScopeToken(clang::ImplicitTypenameContext) (D:\msys64\clang64\bin\libclang-cpp.dll+0x9ed1f)
#31 0x00007ff84195c3fb clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) (D:\msys64\clang64\bin\libclang-cpp.dll+0x9c3fb)
#32 0x00007ff841ddfc77 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (D:\msys64\clang64\bin\libclang-cpp.dll+0x51fc77)
#33 0x00007ff841e5e058 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (D:\msys64\clang64\bin\libclang-cpp.dll+0x59e058)
#34 0x00007ff841e5b210 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (D:\msys64\clang64\bin\libclang-cpp.dll+0x59b210)
#35 0x00007ff841e591f5 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (D:\msys64\clang64\bin\libclang-cpp.dll+0x5991f5)
#36 0x00007ff84209b161 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (D:\msys64\clang64\bin\libclang-cpp.dll+0x7db161)
#37 0x00007ff84209ae25 clang::ParseAST(clang::Sema&, bool, bool) (D:\msys64\clang64\bin\libclang-cpp.dll+0x7dae25)
#38 0x00007ff84214bd8f clang::FrontendAction::Execute() (D:\msys64\clang64\bin\libclang-cpp.dll+0x88bd8f)
#39 0x00007ff84214b9a1 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (D:\msys64\clang64\bin\libclang-cpp.dll+0x88b9a1)
#40 0x00007ff84214b3d2 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (D:\msys64\clang64\bin\libclang-cpp.dll+0x88b3d2)
#41 0x00007ff7487743c2 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (D:\msys64\clang64\bin\c++.exe+0x43c2)
#42 0x00007ff748773b96 llvm::raw_ostream::operator<<(llvm::StringRef) (D:\msys64\clang64\bin\c++.exe+0x3b96)
#43 0x00007ff8436c72bd void std::__1::vector<std::__1::optional<std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>>>, std::__1::allocator<std::__1::optional<std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>>>>>::__assign_with_size[abi:nn200100]<std::__1::optional<std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>>>*, std::__1::optional<std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>>>*>(std::__1::optional<std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>>>*, std::__1::optional<std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>>>*, long long) (D:\msys64\clang64\bin\libclang-cpp.dll+0x1e072bd)
#44 0x00007ff83a7fcfc4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (D:\msys64\clang64\bin\libLLVM-20.dll+0xedcfc4)
#45 0x00007ff8421f1977 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::__1::optional<llvm::StringRef>>, std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>>*, bool*) const (D:\msys64\clang64\bin\libclang-cpp.dll+0x931977)
#46 0x00007ff8421b1fa3 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (D:\msys64\clang64\bin\libclang-cpp.dll+0x8f1fa3)
#47 0x00007ff8421b1c10 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::__1::pair<int, clang::driver::Command const*>>&, bool) const (D:\msys64\clang64\bin\libclang-cpp.dll+0x8f1c10)
#48 0x00007ff8421b193b clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::__1::pair<int, clang::driver::Command const*>>&) (D:\msys64\clang64\bin\libclang-cpp.dll+0x8f193b)
#49 0x00007ff748771c0b clang_main(int, char**, llvm::ToolContext const&) (D:\msys64\clang64\bin\c++.exe+0x1c0b)
#50 0x00007ff748771075 main (D:\msys64\clang64\bin\c++.exe+0x1075)
#51 0x00007ff74877ab99 WinMainCRTStartup (D:\msys64\clang64\bin\c++.exe+0xab99)
#52 0x00007ff74877abf6 mainCRTStartup (D:\msys64\clang64\bin\c++.exe+0xabf6)
#53 0x00007ff8ea9ee8d7 (C:\WINDOWS\System32\KERNEL32.DLL+0x2e8d7)
#54 0x00007ff8eb9914fc (C:\WINDOWS\SYSTEM32\ntdll.dll+0xb14fc)
c++: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 20.1.3
Target: x86_64-w64-windows-gnu
Thread model: posix
InstalledDir: D:/msys64/clang64/bin
c++: note: diagnostic msg: 
********************
```

---

### Comment 2 - jcelerier

It works if either: 
- the lambda is extracted as a struct
- calling ` decltype(structure_to_typelist(T{}));` with a concrete type instead of a template argument

---

### Comment 3 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Jean-Michaël Celerier (jcelerier)

<details>
&gt; clang HEAD from a couple hours ago (git@<!-- -->github.com:llvm/llvm-project e79d8f6892aa138bfef65334817c24d3d9881855) crashes:
&gt; 
&gt; [kabang_avnd-8e80cb.zip](https://github.com/user-attachments/files/19841836/kabang_avnd-8e80cb.zip) 

 _Originally posted by @<!-- -->jcelerier in [#<!-- -->136635](https://github.com/llvm/llvm-project/issues/136635#issuecomment-2819810879)_


Reduced:
```cpp
struct foo { };

template &lt;typename T&gt;
struct vec { };

auto structure_to_typelist(const auto&amp; s) noexcept {
  return []&lt;template &lt;typename...&gt; typename T, typename... Args&gt;(T&lt;Args...&gt;) {
    return 0;
  }(vec&lt;int&gt;{});
}

template &lt;typename T&gt;
using helper2 = decltype(structure_to_typelist(T{}));
auto tl_ok2 = helper2&lt;foo&gt;{};

```
</details>


---

### Comment 4 - shafik

Looks like it started in clang-19: https://godbolt.org/z/43dd7KeeE

Assertions builds are better for bug reports:

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
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++2b <source>
1.	<source>:14:15: at annotation token
2.	<source>:6:6: instantiating function definition 'structure_to_typelist<foo>'
 #0 0x0000000003f1fcd8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f1fcd8)
 #1 0x0000000003f1d964 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f1d964)
 #2 0x0000000003e62518 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000079c399842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x000079c3998969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x000079c399842476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x000079c3998287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x000079c39982871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x000079c399839e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x00000000076a623b getLambdaCallOperatorHelper(clang::CXXRecordDecl const&) (.part.0) DeclCXX.cpp:0:0
#10 0x00000000076a6c6b clang::CXXRecordDecl::getLambdaCallOperator() const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x76a6c6b)
#11 0x00000000071115a9 clang::Sema::getTemplateInstantiationArgs(clang::NamedDecl const*, clang::DeclContext const*, bool, std::optional<llvm::ArrayRef<clang::TemplateArgument>>, bool, clang::FunctionDecl const*, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x71115a9)
#12 0x0000000006fceba5 clang::Sema::CheckTemplateArgumentList(clang::TemplateDecl*, clang::SourceLocation, clang::TemplateArgumentListInfo&, clang::DefaultArguments const&, bool, clang::Sema::CheckTemplateArgumentInfo&, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6fceba5)
#13 0x0000000006fd26f5 clang::Sema::CheckTemplateIdType(clang::TemplateName, clang::SourceLocation, clang::TemplateArgumentListInfo&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6fd26f5)
#14 0x000000000714b28c clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformTemplateSpecializationType(clang::TypeLocBuilder&, clang::TemplateSpecializationTypeLoc, clang::TemplateName) SemaTemplateInstantiate.cpp:0:0
#15 0x000000000713a2b3 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
#16 0x0000000007159674 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformElaboratedType(clang::TypeLocBuilder&, clang::ElaboratedTypeLoc) SemaTemplateInstantiate.cpp:0:0
#17 0x00000000071394f9 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
#18 0x000000000713feea clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeSourceInfo*) SemaTemplateInstantiate.cpp:0:0
#19 0x000000000714e95e clang::Sema::SubstType(clang::TypeSourceInfo*, clang::MultiLevelTemplateArgumentList const&, clang::SourceLocation, clang::DeclarationName, bool) (.constprop.0) SemaTemplateInstantiate.cpp:0:0
#20 0x000000000715c37a clang::Sema::SubstParmVarDecl(clang::ParmVarDecl*, clang::MultiLevelTemplateArgumentList const&, int, clang::UnsignedOrNone, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x715c37a)
#21 0x000000000715d356 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformFunctionTypeParams(clang::SourceLocation, llvm::ArrayRef<clang::ParmVarDecl*>, clang::QualType const*, clang::FunctionType::ExtParameterInfo const*, llvm::SmallVectorImpl<clang::QualType>&, llvm::SmallVectorImpl<clang::ParmVarDecl*>*, clang::Sema::ExtParameterInfoBuilder&, unsigned int*) SemaTemplateInstantiate.cpp:0:0
#22 0x00000000071383bc clang::QualType clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformFunctionProtoType<clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformFunctionProtoType(clang::TypeLocBuilder&, clang::FunctionProtoTypeLoc)::'lambda'(clang::FunctionProtoType::ExceptionSpecInfo&, bool&)>(clang::TypeLocBuilder&, clang::FunctionProtoTypeLoc, clang::CXXRecordDecl*, clang::Qualifiers, clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformFunctionProtoType(clang::TypeLocBuilder&, clang::FunctionProtoTypeLoc)::'lambda'(clang::FunctionProtoType::ExceptionSpecInfo&, bool&)) (.constprop.0) SemaTemplateInstantiate.cpp:0:0
#23 0x0000000007139667 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
#24 0x000000000712684b clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformLambdaExpr(clang::LambdaExpr*) SemaTemplateInstantiate.cpp:0:0
#25 0x0000000007127fa2 (anonymous namespace)::TemplateInstantiator::TransformLambdaExpr(clang::LambdaExpr*) SemaTemplateInstantiate.cpp:0:0
#26 0x000000000712852f clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#27 0x0000000007134ed6 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCallExpr(clang::CallExpr*) SemaTemplateInstantiate.cpp:0:0
#28 0x00000000071283a8 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#29 0x000000000712a535 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformInitializer(clang::Expr*, bool) (.part.0) SemaTemplateInstantiate.cpp:0:0
#30 0x000000000712a8dc clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformReturnStmt(clang::ReturnStmt*) SemaTemplateInstantiate.cpp:0:0
#31 0x00000000071689ce clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaTemplateInstantiate.cpp:0:0
#32 0x0000000007171ce4 clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7171ce4)
#33 0x00000000071d2207 clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x71d2207)
#34 0x0000000007e63f71 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7e63f71)
#35 0x000000000706115a clang::Sema::DeduceReturnType(clang::FunctionDecl*, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x706115a)
#36 0x0000000006a8de83 clang::Sema::DiagnoseUseOfDecl(clang::NamedDecl*, llvm::ArrayRef<clang::SourceLocation>, clang::ObjCInterfaceDecl const*, bool, bool, clang::ObjCInterfaceDecl*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a8de83)
#37 0x0000000006ed64c4 FinishOverloadedCallExpr(clang::Sema&, clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, clang::OverloadCandidateSet*, clang::OverloadCandidate**, clang::OverloadingResult, bool) SemaOverload.cpp:0:0
#38 0x0000000006ed73c0 clang::Sema::BuildOverloadedCallExpr(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ed73c0)
#39 0x0000000006ad61f6 clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ad61f6)
#40 0x0000000006ad707e clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ad707e)
#41 0x0000000007134fdb clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCallExpr(clang::CallExpr*) SemaTemplateInstantiate.cpp:0:0
#42 0x00000000071283a8 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#43 0x000000000713929c clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
#44 0x000000000713feea clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeSourceInfo*) SemaTemplateInstantiate.cpp:0:0
#45 0x000000000714000c clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::QualType) SemaTemplateInstantiate.cpp:0:0
#46 0x0000000007141d31 clang::Sema::SubstType(clang::QualType, clang::MultiLevelTemplateArgumentList const&, clang::SourceLocation, clang::DeclarationName, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7141d31)
#47 0x0000000006fd2adc clang::Sema::CheckTemplateIdType(clang::TemplateName, clang::SourceLocation, clang::TemplateArgumentListInfo&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6fd2adc)
#48 0x0000000006fd5527 clang::Sema::ActOnTemplateIdType(clang::Scope*, clang::CXXScopeSpec&, clang::SourceLocation, clang::OpaquePtr<clang::TemplateName>, clang::IdentifierInfo const*, clang::SourceLocation, clang::SourceLocation, llvm::MutableArrayRef<clang::ParsedTemplateArgument>, clang::SourceLocation, bool, bool, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6fd5527)
#49 0x000000000669b61b clang::Parser::AnnotateTemplateIdTokenAsType(clang::CXXScopeSpec&, clang::ImplicitTypenameContext, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x669b61b)
#50 0x000000000658c570 clang::Parser::TryAnnotateTypeOrScopeTokenAfterScopeSpec(clang::CXXScopeSpec&, bool, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x658c570)
#51 0x000000000658ef3e clang::Parser::TryAnnotateTypeOrScopeToken(clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x658ef3e)
#52 0x0000000006605afe clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6605afe)
#53 0x0000000006607a67 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6607a67)
#54 0x0000000006607af9 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6607af9)
#55 0x00000000065c0538 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65c0538)
#56 0x00000000065cf579 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65cf579)
#57 0x000000000658d9fe clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x658d9fe)
#58 0x000000000658e1b9 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x658e1b9)
#59 0x0000000006595ec3 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6595ec3)
#60 0x0000000006596ddd clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6596ddd)
#61 0x0000000006588e7a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6588e7a)
#62 0x00000000048bf6a8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48bf6a8)
#63 0x0000000004bb3eb5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bb3eb5)
#64 0x0000000004b3253e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b3253e)
#65 0x0000000004ca82b9 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ca82b9)
#66 0x0000000000da2d2f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda2d2f)
#67 0x0000000000d99cda ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#68 0x0000000004925f49 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#69 0x0000000003e629b4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3e629b4)
#70 0x000000000492655f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#71 0x00000000048e89ad clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48e89ad)
#72 0x00000000048e9a3e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48e9a3e)
#73 0x00000000048f1895 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48f1895)
#74 0x0000000000d9fa78 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd9fa78)
#75 0x0000000000c25334 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc25334)
#76 0x000079c399829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#77 0x000079c399829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#78 0x0000000000d99785 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd99785)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

---

### Comment 5 - shafik

Maybe related: https://github.com/llvm/llvm-project/issues/137014

CC @zyn0217 @cor3ntin 

---

