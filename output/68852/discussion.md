# clang ICE when dealing with function call in an NTTP inside a function return type

**Author:** RungeCC
**URL:** https://github.com/llvm/llvm-project/issues/68852
**Status:** Closed
**Labels:** clang:frontend, confirmed, crash
**Closed Date:** 2025-05-06T20:51:07Z

## Body

[Example](https://godbolt.org/z/od7ecbrM4) code(also this, https://godbolt.org/z/58a7encqh):

```c++
#include <concepts>
#include <functional>
#include <type_traits>
#include <utility>

template <class c>
concept constexpr_value_storable = std::is_object_v<std::remove_cvref_t<c>>;

struct never {};

template <auto v, class T = std::remove_cvref_t<decltype(v)>>
struct constexpr_value {
  static inline constexpr auto value = v;
  constexpr constexpr_value() = default;

  template <class... Ts>
    requires(requires { v(Ts::value...); } &&
             constexpr_value_storable<decltype(v(Ts::value...))>)
  constexpr auto call(Ts...) -> constexpr_value<v(Ts::value...)> {
    return {};
  }

  template <class... Ts>
    requires(requires { v(Ts::value...); } &&
             not constexpr_value_storable<decltype(v(Ts::value...))>)
  constexpr auto call(Ts...) -> constexpr_value<never{}> {
    return {};
  }
};

template <auto v> constexpr static inline auto c_ = constexpr_value<v>{};

int main() { auto k = c_<1>; }
```

clang version:

```
clang version 18.0.0 (https://github.com/llvm/llvm-project.git 7cc1bfaf371c4a816cf4e62fe31d8515bf8f6fbd)
Target: x86_64-unknown-linux-gnu
Thread model: posix
```

and flag:

```
-std=c++2b
```

outputs:

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++2b <source>
1.	<source>:33:28: current parser token ';'
2.	<source>:33:12: parsing function body 'main'
3.	<source>:33:12: in compound statement ('{}')
4.	<source>:12:8: instantiating class definition 'constexpr_value<1>'
 #0 0x000000000342d0c8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x342d0c8)
 #1 0x000000000342b1f4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x342b1f4)
 #2 0x00000000033793f8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007f00ee80c420 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x14420)
 #4 0x0000000006acb55c ClassifyUnnamed(clang::ASTContext&, clang::QualType) ExprClassification.cpp:0:0
 #5 0x0000000006acb7b6 ClassifyInternal(clang::ASTContext&, clang::Expr const*) ExprClassification.cpp:0:0
 #6 0x0000000006acc0ee clang::Expr::ClassifyImpl(clang::ASTContext&, clang::SourceLocation*) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6acc0ee)
 #7 0x00000000064e6658 clang::Sema::DeduceAutoType(clang::TypeLoc, clang::Expr*, clang::QualType&, clang::sema::TemplateDeductionInfo&, bool, bool, clang::TemplateSpecCandidateSet*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x64e6658)
 #8 0x00000000063f6489 clang::Sema::CheckTemplateArgument(clang::NonTypeTemplateParmDecl*, clang::QualType, clang::Expr*, clang::TemplateArgument&, clang::TemplateArgument&, clang::Sema::CheckTemplateArgumentKind) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x63f6489)
 #9 0x00000000063f87c6 clang::Sema::CheckTemplateArgument(clang::NamedDecl*, clang::TemplateArgumentLoc&, clang::NamedDecl*, clang::SourceLocation, clang::SourceLocation, unsigned int, llvm::SmallVectorImpl<clang::TemplateArgument>&, llvm::SmallVectorImpl<clang::TemplateArgument>&, clang::Sema::CheckTemplateArgumentKind) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x63f87c6)
#10 0x00000000063f9e6c clang::Sema::CheckTemplateArgumentList(clang::TemplateDecl*, clang::SourceLocation, clang::TemplateArgumentListInfo&, bool, llvm::SmallVectorImpl<clang::TemplateArgument>&, llvm::SmallVectorImpl<clang::TemplateArgument>&, bool, bool*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x63f9e6c)
#11 0x0000000006406079 clang::Sema::CheckTemplateIdType(clang::TemplateName, clang::SourceLocation, clang::TemplateArgumentListInfo&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6406079)
#12 0x0000000006547d55 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformTemplateSpecializationType(clang::TypeLocBuilder&, clang::TemplateSpecializationTypeLoc, clang::TemplateName) SemaTemplateInstantiate.cpp:0:0
#13 0x000000000653de06 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
#14 0x000000000654fd30 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformElaboratedType(clang::TypeLocBuilder&, clang::ElaboratedTypeLoc) SemaTemplateInstantiate.cpp:0:0
#15 0x000000000653c810 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
#16 0x0000000006558c17 clang::Sema::SubstFunctionDeclType(clang::TypeSourceInfo*, clang::MultiLevelTemplateArgumentList const&, clang::SourceLocation, clang::DeclarationName, clang::CXXRecordDecl*, clang::Qualifiers, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6558c17)
#17 0x000000000656cc03 clang::TemplateDeclInstantiator::SubstFunctionType(clang::FunctionDecl*, llvm::SmallVectorImpl<clang::ParmVarDecl*>&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x656cc03)
#18 0x00000000065b8623 clang::TemplateDeclInstantiator::VisitCXXMethodDecl(clang::CXXMethodDecl*, clang::TemplateParameterList*, clang::TemplateDeclInstantiator::RewriteKind) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x65b8623)
#19 0x00000000065be62a clang::TemplateDeclInstantiator::VisitFunctionTemplateDecl(clang::FunctionTemplateDecl*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x65be62a)
#20 0x000000000654ab25 clang::Sema::InstantiateClass(clang::SourceLocation, clang::CXXRecordDecl*, clang::CXXRecordDecl*, clang::MultiLevelTemplateArgumentList const&, clang::TemplateSpecializationKind, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x654ab25)
#21 0x0000000006567075 clang::Sema::InstantiateClassTemplateSpecialization(clang::SourceLocation, clang::ClassTemplateSpecializationDecl*, clang::TemplateSpecializationKind, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6567075)
#22 0x00000000065e2f2f void llvm::function_ref<void ()>::callback_fn<clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*)::'lambda'()>(long) SemaType.cpp:0:0
#23 0x0000000005b95d81 clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5b95d81)
#24 0x00000000065e820d clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x65e820d)
#25 0x00000000065e8295 clang::Sema::RequireCompleteType(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x65e8295)
#26 0x00000000060a127e clang::Sema::BuildCXXTypeConstructExpr(clang::TypeSourceInfo*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x60a127e)
#27 0x000000000654962d clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCXXUnresolvedConstructExpr(clang::CXXUnresolvedConstructExpr*) SemaTemplateInstantiate.cpp:0:0
#28 0x00000000065313e1 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#29 0x00000000065320c5 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformInitializer(clang::Expr*, bool) SemaTemplateInstantiate.cpp:0:0
#30 0x000000000653396a clang::Sema::SubstInitializer(clang::Expr*, clang::MultiLevelTemplateArgumentList const&, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x653396a)
#31 0x000000000656e374 clang::Sema::InstantiateVariableInitializer(clang::VarDecl*, clang::VarDecl*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x656e374)
#32 0x00000000065a9812 clang::Sema::BuildVariableInstantiation(clang::VarDecl*, clang::VarDecl*, clang::MultiLevelTemplateArgumentList const&, llvm::SmallVector<clang::Sema::LateInstantiatedAttribute, 16u>*, clang::DeclContext*, clang::LocalInstantiationScope*, bool, clang::VarTemplateSpecializationDecl*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x65a9812)
#33 0x00000000065aa0ba clang::TemplateDeclInstantiator::VisitVarTemplateSpecializationDecl(clang::VarTemplateDecl*, clang::VarDecl*, clang::TemplateArgumentListInfo const&, llvm::ArrayRef<clang::TemplateArgument>, clang::VarTemplateSpecializationDecl*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x65aa0ba)
#34 0x00000000065b4ac2 clang::Sema::BuildVarTemplateInstantiation(clang::VarTemplateDecl*, clang::VarDecl*, clang::TemplateArgumentList const&, clang::TemplateArgumentListInfo const&, llvm::SmallVectorImpl<clang::TemplateArgument>&, clang::SourceLocation, llvm::SmallVector<clang::Sema::LateInstantiatedAttribute, 16u>*, clang::LocalInstantiationScope*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x65b4ac2)
#35 0x000000000640b7a7 clang::Sema::CheckVarTemplateId(clang::VarTemplateDecl*, clang::SourceLocation, clang::SourceLocation, clang::TemplateArgumentListInfo const&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x640b7a7)
#36 0x000000000640bddf clang::Sema::BuildTemplateIdExpr(clang::CXXScopeSpec const&, clang::SourceLocation, clang::LookupResult&, bool, clang::TemplateArgumentListInfo const*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x640bddf)
#37 0x0000000005fd1f79 clang::Sema::ActOnIdExpression(clang::Scope*, clang::CXXScopeSpec&, clang::SourceLocation, clang::UnqualifiedId&, bool, bool, clang::CorrectionCandidateCallback*, bool, clang::Token*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5fd1f79)
#38 0x0000000005ac5448 clang::Parser::tryParseCXXIdExpression(clang::CXXScopeSpec&, bool, clang::Token&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5ac5448)
#39 0x0000000005ac563a clang::Parser::ParseCXXIdExpression(bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5ac563a)
#40 0x0000000005aa7e78 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5aa7e78)
#41 0x0000000005aa873c clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5aa873c)
#42 0x0000000005aaa61a clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5aaa61a)
#43 0x0000000005aaa7b9 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5aaa7b9)
#44 0x0000000005a67211 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5a67211)
#45 0x0000000005a7be10 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5a7be10)
#46 0x0000000005a7d1f2 clang::Parser::ParseSimpleDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, bool, clang::Parser::ForRangeInit*, clang::SourceLocation*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5a7d1f2)
#47 0x0000000005a7d6ff clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5a7d6ff)
#48 0x0000000005b29e29 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5b29e29)
#49 0x0000000005b2a7b8 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5b2a7b8)
#50 0x0000000005b2b5b9 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5b2b5b9)
#51 0x0000000005b2d80a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5b2d80a)
#52 0x0000000005a48f78 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5a48f78)
#53 0x0000000005a7bf59 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5a7bf59)
#54 0x0000000005a42679 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5a42679)
#55 0x0000000005a4318f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (.part.0) Parser.cpp:0:0
#56 0x0000000005a4bf62 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5a4bf62)
#57 0x0000000005a4d127 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5a4d127)
#58 0x0000000005a3c79a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5a3c79a)
#59 0x000000000463081d clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x463081d)
#60 0x0000000003ed2fc1 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3ed2fc1)
#61 0x0000000003e51c3b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3e51c3b)
#62 0x0000000003fb2963 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3fb2963)
#63 0x0000000000bfd196 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xbfd196)
#64 0x0000000000bf5c1d ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#65 0x0000000003c9f0c9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#66 0x0000000003379823 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3379823)
#67 0x0000000003c9f2e9 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#68 0x0000000003c67507 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c67507)
#69 0x0000000003c67ebd clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c67ebd)
#70 0x0000000003c71c6c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c71c6c)
#71 0x0000000000bfa5b1 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xbfa5b1)
#72 0x0000000000af6541 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xaf6541)
#73 0x00007f00ee2b0083 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x24083)
#74 0x0000000000bf541e _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xbf541e)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```

Notably, clang17 accepts this codes correctly. Also, when the function call appear in the function body but not in the return type, clang works well(https://godbolt.org/z/MsrTz78a1).

## Comments

### Comment 1 - RungeCC

Another hint, if the `v(Ts::value...)` is valid for some `Ts...`, clang also works well(https://godbolt.org/z/1ob316q7v). Maybe clang is a little bit too radical in deciding the function signature?

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Runge (RungeCC)

<details>
[Example](https://godbolt.org/z/od7ecbrM4) code(also this, https://godbolt.org/z/58a7encqh):

```c++
#include &lt;concepts&gt;
#include &lt;functional&gt;
#include &lt;type_traits&gt;
#include &lt;utility&gt;

template &lt;class c&gt;
concept constexpr_value_storable = std::is_object_v&lt;std::remove_cvref_t&lt;c&gt;&gt;;

struct never {};

template &lt;auto v, class T = std::remove_cvref_t&lt;decltype(v)&gt;&gt;
struct constexpr_value {
  static inline constexpr auto value = v;
  constexpr constexpr_value() = default;

  template &lt;class... Ts&gt;
    requires(requires { v(Ts::value...); } &amp;&amp;
             constexpr_value_storable&lt;decltype(v(Ts::value...))&gt;)
  constexpr auto call(Ts...) -&gt; constexpr_value&lt;v(Ts::value...)&gt; {
    return {};
  }

  template &lt;class... Ts&gt;
    requires(requires { v(Ts::value...); } &amp;&amp;
             not constexpr_value_storable&lt;decltype(v(Ts::value...))&gt;)
  constexpr auto call(Ts...) -&gt; constexpr_value&lt;never{}&gt; {
    return {};
  }
};

template &lt;auto v&gt; constexpr static inline auto c_ = constexpr_value&lt;v&gt;{};

int main() { auto k = c_&lt;1&gt;; }
```

clang version:

```
clang version 18.0.0 (https://github.com/llvm/llvm-project.git 7cc1bfaf371c4a816cf4e62fe31d8515bf8f6fbd)
Target: x86_64-unknown-linux-gnu
Thread model: posix
```

and flag:

```
-std=c++2b
```

outputs:

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++2b &lt;source&gt;
1.	&lt;source&gt;:33:28: current parser token ';'
2.	&lt;source&gt;:33:12: parsing function body 'main'
3.	&lt;source&gt;:33:12: in compound statement ('{}')
4.	&lt;source&gt;:12:8: instantiating class definition 'constexpr_value&lt;1&gt;'
 #<!-- -->0 0x000000000342d0c8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x342d0c8)
 #<!-- -->1 0x000000000342b1f4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x342b1f4)
 #<!-- -->2 0x00000000033793f8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007f00ee80c420 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x14420)
 #<!-- -->4 0x0000000006acb55c ClassifyUnnamed(clang::ASTContext&amp;, clang::QualType) ExprClassification.cpp:0:0
 #<!-- -->5 0x0000000006acb7b6 ClassifyInternal(clang::ASTContext&amp;, clang::Expr const*) ExprClassification.cpp:0:0
 #<!-- -->6 0x0000000006acc0ee clang::Expr::ClassifyImpl(clang::ASTContext&amp;, clang::SourceLocation*) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6acc0ee)
 #<!-- -->7 0x00000000064e6658 clang::Sema::DeduceAutoType(clang::TypeLoc, clang::Expr*, clang::QualType&amp;, clang::sema::TemplateDeductionInfo&amp;, bool, bool, clang::TemplateSpecCandidateSet*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x64e6658)
 #<!-- -->8 0x00000000063f6489 clang::Sema::CheckTemplateArgument(clang::NonTypeTemplateParmDecl*, clang::QualType, clang::Expr*, clang::TemplateArgument&amp;, clang::TemplateArgument&amp;, clang::Sema::CheckTemplateArgumentKind) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x63f6489)
 #<!-- -->9 0x00000000063f87c6 clang::Sema::CheckTemplateArgument(clang::NamedDecl*, clang::TemplateArgumentLoc&amp;, clang::NamedDecl*, clang::SourceLocation, clang::SourceLocation, unsigned int, llvm::SmallVectorImpl&lt;clang::TemplateArgument&gt;&amp;, llvm::SmallVectorImpl&lt;clang::TemplateArgument&gt;&amp;, clang::Sema::CheckTemplateArgumentKind) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x63f87c6)
#<!-- -->10 0x00000000063f9e6c clang::Sema::CheckTemplateArgumentList(clang::TemplateDecl*, clang::SourceLocation, clang::TemplateArgumentListInfo&amp;, bool, llvm::SmallVectorImpl&lt;clang::TemplateArgument&gt;&amp;, llvm::SmallVectorImpl&lt;clang::TemplateArgument&gt;&amp;, bool, bool*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x63f9e6c)
#<!-- -->11 0x0000000006406079 clang::Sema::CheckTemplateIdType(clang::TemplateName, clang::SourceLocation, clang::TemplateArgumentListInfo&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6406079)
#<!-- -->12 0x0000000006547d55 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformTemplateSpecializationType(clang::TypeLocBuilder&amp;, clang::TemplateSpecializationTypeLoc, clang::TemplateName) SemaTemplateInstantiate.cpp:0:0
#<!-- -->13 0x000000000653de06 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeLocBuilder&amp;, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
#<!-- -->14 0x000000000654fd30 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformElaboratedType(clang::TypeLocBuilder&amp;, clang::ElaboratedTypeLoc) SemaTemplateInstantiate.cpp:0:0
#<!-- -->15 0x000000000653c810 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeLocBuilder&amp;, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
#<!-- -->16 0x0000000006558c17 clang::Sema::SubstFunctionDeclType(clang::TypeSourceInfo*, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceLocation, clang::DeclarationName, clang::CXXRecordDecl*, clang::Qualifiers, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6558c17)
#<!-- -->17 0x000000000656cc03 clang::TemplateDeclInstantiator::SubstFunctionType(clang::FunctionDecl*, llvm::SmallVectorImpl&lt;clang::ParmVarDecl*&gt;&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x656cc03)
#<!-- -->18 0x00000000065b8623 clang::TemplateDeclInstantiator::VisitCXXMethodDecl(clang::CXXMethodDecl*, clang::TemplateParameterList*, clang::TemplateDeclInstantiator::RewriteKind) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x65b8623)
#<!-- -->19 0x00000000065be62a clang::TemplateDeclInstantiator::VisitFunctionTemplateDecl(clang::FunctionTemplateDecl*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x65be62a)
#<!-- -->20 0x000000000654ab25 clang::Sema::InstantiateClass(clang::SourceLocation, clang::CXXRecordDecl*, clang::CXXRecordDecl*, clang::MultiLevelTemplateArgumentList const&amp;, clang::TemplateSpecializationKind, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x654ab25)
#<!-- -->21 0x0000000006567075 clang::Sema::InstantiateClassTemplateSpecialization(clang::SourceLocation, clang::ClassTemplateSpecializationDecl*, clang::TemplateSpecializationKind, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6567075)
#<!-- -->22 0x00000000065e2f2f void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*)::'lambda'()&gt;(long) SemaType.cpp:0:0
#<!-- -->23 0x0000000005b95d81 clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5b95d81)
#<!-- -->24 0x00000000065e820d clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x65e820d)
#<!-- -->25 0x00000000065e8295 clang::Sema::RequireCompleteType(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x65e8295)
#<!-- -->26 0x00000000060a127e clang::Sema::BuildCXXTypeConstructExpr(clang::TypeSourceInfo*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x60a127e)
#<!-- -->27 0x000000000654962d clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCXXUnresolvedConstructExpr(clang::CXXUnresolvedConstructExpr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->28 0x00000000065313e1 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->29 0x00000000065320c5 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformInitializer(clang::Expr*, bool) SemaTemplateInstantiate.cpp:0:0
#<!-- -->30 0x000000000653396a clang::Sema::SubstInitializer(clang::Expr*, clang::MultiLevelTemplateArgumentList const&amp;, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x653396a)
#<!-- -->31 0x000000000656e374 clang::Sema::InstantiateVariableInitializer(clang::VarDecl*, clang::VarDecl*, clang::MultiLevelTemplateArgumentList const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x656e374)
#<!-- -->32 0x00000000065a9812 clang::Sema::BuildVariableInstantiation(clang::VarDecl*, clang::VarDecl*, clang::MultiLevelTemplateArgumentList const&amp;, llvm::SmallVector&lt;clang::Sema::LateInstantiatedAttribute, 16u&gt;*, clang::DeclContext*, clang::LocalInstantiationScope*, bool, clang::VarTemplateSpecializationDecl*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x65a9812)
#<!-- -->33 0x00000000065aa0ba clang::TemplateDeclInstantiator::VisitVarTemplateSpecializationDecl(clang::VarTemplateDecl*, clang::VarDecl*, clang::TemplateArgumentListInfo const&amp;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::VarTemplateSpecializationDecl*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x65aa0ba)
#<!-- -->34 0x00000000065b4ac2 clang::Sema::BuildVarTemplateInstantiation(clang::VarTemplateDecl*, clang::VarDecl*, clang::TemplateArgumentList const&amp;, clang::TemplateArgumentListInfo const&amp;, llvm::SmallVectorImpl&lt;clang::TemplateArgument&gt;&amp;, clang::SourceLocation, llvm::SmallVector&lt;clang::Sema::LateInstantiatedAttribute, 16u&gt;*, clang::LocalInstantiationScope*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x65b4ac2)
#<!-- -->35 0x000000000640b7a7 clang::Sema::CheckVarTemplateId(clang::VarTemplateDecl*, clang::SourceLocation, clang::SourceLocation, clang::TemplateArgumentListInfo const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x640b7a7)
#<!-- -->36 0x000000000640bddf clang::Sema::BuildTemplateIdExpr(clang::CXXScopeSpec const&amp;, clang::SourceLocation, clang::LookupResult&amp;, bool, clang::TemplateArgumentListInfo const*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x640bddf)
#<!-- -->37 0x0000000005fd1f79 clang::Sema::ActOnIdExpression(clang::Scope*, clang::CXXScopeSpec&amp;, clang::SourceLocation, clang::UnqualifiedId&amp;, bool, bool, clang::CorrectionCandidateCallback*, bool, clang::Token*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5fd1f79)
#<!-- -->38 0x0000000005ac5448 clang::Parser::tryParseCXXIdExpression(clang::CXXScopeSpec&amp;, bool, clang::Token&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5ac5448)
#<!-- -->39 0x0000000005ac563a clang::Parser::ParseCXXIdExpression(bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5ac563a)
#<!-- -->40 0x0000000005aa7e78 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5aa7e78)
#<!-- -->41 0x0000000005aa873c clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5aa873c)
#<!-- -->42 0x0000000005aaa61a clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5aaa61a)
#<!-- -->43 0x0000000005aaa7b9 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5aaa7b9)
#<!-- -->44 0x0000000005a67211 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5a67211)
#<!-- -->45 0x0000000005a7be10 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5a7be10)
#<!-- -->46 0x0000000005a7d1f2 clang::Parser::ParseSimpleDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, bool, clang::Parser::ForRangeInit*, clang::SourceLocation*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5a7d1f2)
#<!-- -->47 0x0000000005a7d6ff clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5a7d6ff)
#<!-- -->48 0x0000000005b29e29 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5b29e29)
#<!-- -->49 0x0000000005b2a7b8 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5b2a7b8)
#<!-- -->50 0x0000000005b2b5b9 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5b2b5b9)
#<!-- -->51 0x0000000005b2d80a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5b2d80a)
#<!-- -->52 0x0000000005a48f78 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5a48f78)
#<!-- -->53 0x0000000005a7bf59 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5a7bf59)
#<!-- -->54 0x0000000005a42679 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5a42679)
#<!-- -->55 0x0000000005a4318f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (.part.0) Parser.cpp:0:0
#<!-- -->56 0x0000000005a4bf62 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5a4bf62)
#<!-- -->57 0x0000000005a4d127 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5a4d127)
#<!-- -->58 0x0000000005a3c79a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5a3c79a)
#<!-- -->59 0x000000000463081d clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x463081d)
#<!-- -->60 0x0000000003ed2fc1 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3ed2fc1)
#<!-- -->61 0x0000000003e51c3b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3e51c3b)
#<!-- -->62 0x0000000003fb2963 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3fb2963)
#<!-- -->63 0x0000000000bfd196 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xbfd196)
#<!-- -->64 0x0000000000bf5c1d ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->65 0x0000000003c9f0c9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->66 0x0000000003379823 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3379823)
#<!-- -->67 0x0000000003c9f2e9 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->68 0x0000000003c67507 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c67507)
#<!-- -->69 0x0000000003c67ebd clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c67ebd)
#<!-- -->70 0x0000000003c71c6c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c71c6c)
#<!-- -->71 0x0000000000bfa5b1 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xbfa5b1)
#<!-- -->72 0x0000000000af6541 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xaf6541)
#<!-- -->73 0x00007f00ee2b0083 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x24083)
#<!-- -->74 0x0000000000bf541e _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xbf541e)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```

Notably, clang17 accepts this codes correctly. Also, when the function call appear in the function body but not in the return type, clang works well(https://godbolt.org/z/MsrTz78a1).
</details>


---

### Comment 3 - RungeCC

I think this case is `crash-on-invalid`(IFNDR case). Since template `auto call(Ts...) -> constexpr_value<1(Ts::value...)>` will never be well-formed, according to [temp.res.general#8.1](https://timsong-cpp.github.io/cppwp/n4868/temp.res.general#8.1):

> The program is ill-formed, no diagnostic required, if:
> - no valid specialization can be generated for a template or a substatement of a [constexpr if](https://timsong-cpp.github.io/cppwp/n4868/stmt.if#def:constexpr_if) statement within a template and the template is not instantiated, or
> - ...

---

### Comment 4 - shafik

Confirmed: https://godbolt.org/z/9o5MGjfoz

with assertion build.

Assertion:

```console
clang++: /root/llvm-project/clang/include/clang/AST/Type.h:7630:
const T* clang::Type::castAs() const [with T = clang::FunctionType]:
Assertion `isa<T>(CanonicalType)' failed.
```

Backtrace:

```console
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++2b <source>
1.	<source>:33:28: current parser token ';'
2.	<source>:33:12: parsing function body 'main'
3.	<source>:33:12: in compound statement ('{}')
4.	<source>:12:8: instantiating class definition 'constexpr_value<1>'
 #0 0x0000000003793138 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3793138)
 #1 0x0000000003790e1c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3790e1c)
 #2 0x00000000036d9988 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007f255744d420 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x14420)
 #4 0x00007f2556f1000b raise (/lib/x86_64-linux-gnu/libc.so.6+0x4300b)
 #5 0x00007f2556eef859 abort (/lib/x86_64-linux-gnu/libc.so.6+0x22859)
 #6 0x00007f2556eef729 (/lib/x86_64-linux-gnu/libc.so.6+0x22729)
 #7 0x00007f2556f00fd6 (/lib/x86_64-linux-gnu/libc.so.6+0x33fd6)
 #8 0x00000000070282df clang::CallExpr::getCallReturnType(clang::ASTContext const&) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x70282df)
 #9 0x0000000007030c6b ClassifyInternal(clang::ASTContext&, clang::Expr const*) ExprClassification.cpp:0:0
#10 0x0000000007031d64 clang::Expr::ClassifyImpl(clang::ASTContext&, clang::SourceLocation*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7031d64)
#11 0x00000000069eb595 clang::Sema::DeduceAutoType(clang::TypeLoc, clang::Expr*, clang::QualType&, clang::sema::TemplateDeductionInfo&, bool, bool, clang::TemplateSpecCandidateSet*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69eb595)
#12 0x00000000068d33bc clang::Sema::CheckTemplateArgument(clang::NonTypeTemplateParmDecl*, clang::QualType, clang::Expr*, clang::TemplateArgument&, clang::TemplateArgument&, clang::Sema::CheckTemplateArgumentKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68d33bc)
#13 0x00000000068dca55 clang::Sema::CheckTemplateArgument(clang::NamedDecl*, clang::TemplateArgumentLoc&, clang::NamedDecl*, clang::SourceLocation, clang::SourceLocation, unsigned int, llvm::SmallVectorImpl<clang::TemplateArgument>&, llvm::SmallVectorImpl<clang::TemplateArgument>&, clang::Sema::CheckTemplateArgumentKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68dca55)
#14 0x00000000068e0ca8 clang::Sema::CheckTemplateArgumentList(clang::TemplateDecl*, clang::SourceLocation, clang::TemplateArgumentListInfo&, bool, llvm::SmallVectorImpl<clang::TemplateArgument>&, llvm::SmallVectorImpl<clang::TemplateArgument>&, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68e0ca8)
#15 0x00000000068e4369 clang::Sema::CheckTemplateIdType(clang::TemplateName, clang::SourceLocation, clang::TemplateArgumentListInfo&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68e4369)
#16 0x0000000006a4c12d clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformTemplateSpecializationType(clang::TypeLocBuilder&, clang::TemplateSpecializationTypeLoc, clang::TemplateName) SemaTemplateInstantiate.cpp:0:0
#17 0x0000000006a3f868 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
#18 0x0000000006a543ed clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformElaboratedType(clang::TypeLocBuilder&, clang::ElaboratedTypeLoc) SemaTemplateInstantiate.cpp:0:0
#19 0x0000000006a3ec8e clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
#20 0x0000000006a5c06a clang::Sema::SubstFunctionDeclType(clang::TypeSourceInfo*, clang::MultiLevelTemplateArgumentList const&, clang::SourceLocation, clang::DeclarationName, clang::CXXRecordDecl*, clang::Qualifiers, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a5c06a)
#21 0x0000000006a7620c clang::TemplateDeclInstantiator::SubstFunctionType(clang::FunctionDecl*, llvm::SmallVectorImpl<clang::ParmVarDecl*>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a7620c)
#22 0x0000000006ac480a clang::TemplateDeclInstantiator::VisitCXXMethodDecl(clang::CXXMethodDecl*, clang::TemplateParameterList*, clang::TemplateDeclInstantiator::RewriteKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ac480a)
#23 0x0000000006ac87d0 clang::TemplateDeclInstantiator::VisitFunctionTemplateDecl(clang::FunctionTemplateDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ac87d0)
#24 0x0000000006a4e263 clang::Sema::InstantiateClass(clang::SourceLocation, clang::CXXRecordDecl*, clang::CXXRecordDecl*, clang::MultiLevelTemplateArgumentList const&, clang::TemplateSpecializationKind, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a4e263)
#25 0x0000000006a6c4eb clang::Sema::InstantiateClassTemplateSpecialization(clang::SourceLocation, clang::ClassTemplateSpecializationDecl*, clang::TemplateSpecializationKind, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a6c4eb)
#26 0x0000000006af581f void llvm::function_ref<void ()>::callback_fn<clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*)::'lambda'()>(long) SemaType.cpp:0:0
#27 0x000000000606bd91 clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x606bd91)
#28 0x0000000006afed37 clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6afed37)
#29 0x0000000006aff085 clang::Sema::RequireCompleteType(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6aff085)
#30 0x000000000658ec67 clang::Sema::BuildCXXTypeConstructExpr(clang::TypeSourceInfo*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x658ec67)
#31 0x0000000006a508ed clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCXXUnresolvedConstructExpr(clang::CXXUnresolvedConstructExpr*) SemaTemplateInstantiate.cpp:0:0
#32 0x0000000006a30ecc clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#33 0x0000000006a317d8 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformInitializer(clang::Expr*, bool) (.part.0) SemaTemplateInstantiate.cpp:0:0
#34 0x0000000006a31d61 clang::Sema::SubstInitializer(clang::Expr*, clang::MultiLevelTemplateArgumentList const&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a31d61)
#35 0x0000000006a780ae clang::Sema::InstantiateVariableInitializer(clang::VarDecl*, clang::VarDecl*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a780ae)
#36 0x0000000006ab7f0d clang::Sema::BuildVariableInstantiation(clang::VarDecl*, clang::VarDecl*, clang::MultiLevelTemplateArgumentList const&, llvm::SmallVector<clang::Sema::LateInstantiatedAttribute, 16u>*, clang::DeclContext*, clang::LocalInstantiationScope*, bool, clang::VarTemplateSpecializationDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ab7f0d)
#37 0x0000000006ab8783 clang::TemplateDeclInstantiator::VisitVarTemplateSpecializationDecl(clang::VarTemplateDecl*, clang::VarDecl*, clang::TemplateArgumentListInfo const&, llvm::ArrayRef<clang::TemplateArgument>, clang::VarTemplateSpecializationDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ab8783)
#38 0x0000000006ac1170 clang::Sema::BuildVarTemplateInstantiation(clang::VarTemplateDecl*, clang::VarDecl*, clang::TemplateArgumentList const&, clang::TemplateArgumentListInfo const&, llvm::SmallVectorImpl<clang::TemplateArgument>&, clang::SourceLocation, llvm::SmallVector<clang::Sema::LateInstantiatedAttribute, 16u>*, clang::LocalInstantiationScope*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ac1170)
#39 0x00000000068e3b27 clang::Sema::CheckVarTemplateId(clang::VarTemplateDecl*, clang::SourceLocation, clang::SourceLocation, clang::TemplateArgumentListInfo const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68e3b27)
#40 0x00000000068e3f86 clang::Sema::CheckVarTemplateId(clang::CXXScopeSpec const&, clang::DeclarationNameInfo const&, clang::VarTemplateDecl*, clang::SourceLocation, clang::TemplateArgumentListInfo const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68e3f86)
#41 0x00000000068e89e4 clang::Sema::BuildTemplateIdExpr(clang::CXXScopeSpec const&, clang::SourceLocation, clang::LookupResult&, bool, clang::TemplateArgumentListInfo const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68e89e4)
#42 0x00000000064a918c clang::Sema::ActOnIdExpression(clang::Scope*, clang::CXXScopeSpec&, clang::SourceLocation, clang::UnqualifiedId&, bool, bool, clang::CorrectionCandidateCallback*, bool, clang::Token*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64a918c)
#43 0x0000000005f9f576 clang::Parser::tryParseCXXIdExpression(clang::CXXScopeSpec&, bool, clang::Token&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5f9f576)
#44 0x0000000005f9f7b4 clang::Parser::ParseCXXIdExpression(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5f9f7b4)
#45 0x0000000005f8079d clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5f8079d)
#46 0x0000000005f810c5 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5f810c5)
#47 0x0000000005f82dfa clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5f82dfa)
#48 0x0000000005f82f99 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5f82f99)
#49 0x0000000005f436fd clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5f436fd)
#50 0x0000000005f50e10 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5f50e10)
#51 0x0000000005f59305 clang::Parser::ParseSimpleDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, bool, clang::Parser::ForRangeInit*, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5f59305)
#52 0x0000000005f597a7 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5f597a7)
#53 0x0000000005ff9960 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5ff9960)
#54 0x0000000005ffa657 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5ffa657)
#55 0x0000000005ffb579 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5ffb579)
#56 0x0000000005ffce9a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5ffce9a)
#57 0x0000000005f2a071 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5f2a071)
#58 0x0000000005f51518 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5f51518)
#59 0x0000000005f1e26b clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5f1e26b)
#60 0x0000000005f1e9f1 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5f1e9f1)
#61 0x0000000005f25cc6 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5f25cc6)
#62 0x0000000005f26b6d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5f26b6d)
#63 0x0000000005f19b8a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5f19b8a)
#64 0x000000000400cb58 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x400cb58)
#65 0x0000000004273199 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4273199)
#66 0x00000000041f36fe clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41f36fe)
#67 0x000000000435247e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x435247e)
#68 0x0000000000bf71e6 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xbf71e6)
#69 0x0000000000beeaaa ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#70 0x000000000404de79 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#71 0x00000000036d9e34 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x36d9e34)
#72 0x000000000404e46f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#73 0x00000000040163e5 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x40163e5)
#74 0x0000000004016e4d clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4016e4d)
#75 0x000000000401ed75 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x401ed75)
#76 0x0000000000bf468c clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xbf468c)
#77 0x0000000000aee061 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xaee061)
#78 0x00007f2556ef1083 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x24083)
#79 0x0000000000bee58e _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xbee58e)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

---

### Comment 5 - RungeCC

Reduce to (https://godbolt.org/z/dKx75P61W):

```c++
template <auto v>
struct hold {};

template <auto v>
constexpr void ice() {

  [&]<auto r>(){
    hold<v(r)> {};
  };

}

int main() {
  ice<1>();
}
```

Or this (https://godbolt.org/z/rW1Esaz3a):

```c++
template <auto v>
struct ICE {

  template <auto r>
  constexpr auto call() -> ICE<v(r)>;

};

using R = decltype(ICE<1>{});
```

Perhaps due to missing some instantiation checks?

---

