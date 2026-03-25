# [clang] Assertion failure with partial template specialization, variadic template, and non-type template parameter

**Author:** davidstone
**URL:** https://github.com/llvm/llvm-project/issues/162770
**Status:** Closed
**Labels:** clang:frontend, crash
**Closed Date:** 2025-10-11T02:00:37Z

## Body

The following valid translation unit

```c++
enum e { };

template<e>
struct s {
};

template<typename>
struct specialized;

template<e x>
struct specialized<s<x>> {
	static auto make(auto) -> s<x>;
};

template<e x>
struct check {
	static constexpr auto m = requires { specialized<s<x>>::make(0); };
};

template<typename... Ts>
auto comma = (..., Ts());

auto b = comma<check<e{}>>;
```

causes clang to crash with

```console
clang++: llvm/clang/include/clang/AST/TypeBase.h:953: const ExtQualsTypeCommonBase *clang::QualType::getCommonPtr() const: Assertion `!isNull() && "Cannot retrieve a NULL type pointer"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: clang++ -gdwarf-4 -g -o /tmp/compiler-explorer-compiler202599-997-4cgv05.qyzo4/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 <source>
1.	<source>:23:27: current parser token ';'
2.	<source>:16:8: instantiating class definition 'check<0>'
 #0 0x000055a14e278c06 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (clang+++0x4adac06)
 #1 0x000055a14e276155 llvm::sys::RunSignalHandlers() (clang+++0x4ad8155)
 #2 0x000055a14e1aaaed CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007f530c63e540 (/usr/lib/libc.so.6+0x3e540)
 #4 0x00007f530c69894c (/usr/lib/libc.so.6+0x9894c)
 #5 0x00007f530c63e410 raise (/usr/lib/libc.so.6+0x3e410)
 #6 0x00007f530c62557a abort (/usr/lib/libc.so.6+0x2557a)
 #7 0x00007f530c6254e3 __assert_perror_fail (/usr/lib/libc.so.6+0x254e3)
 #8 0x000055a14e919e02 clang::ASTContext::hasSameType(clang::QualType, clang::QualType) const CGCoroutine.cpp:0:0
 #9 0x000055a150caee7e clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformSubstNonTypeTemplateParmExpr(clang::SubstNonTypeTemplateParmExpr*) SemaTemplateInstantiate.cpp:0:0
#10 0x000055a150ca6ff5 (anonymous namespace)::TemplateInstantiator::TransformTemplateArgument(clang::TemplateArgumentLoc const&, clang::TemplateArgumentLoc&, bool) SemaTemplateInstantiate.cpp:0:0
#11 0x000055a150cc7f4a clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformTemplateSpecializationType(clang::TypeLocBuilder&, clang::TemplateSpecializationTypeLoc, clang::QualType, clang::NamedDecl*, bool) SemaTemplateInstantiate.cpp:0:0
#12 0x000055a150cd04bf (anonymous namespace)::TemplateInstantiator::TransformTemplateSpecializationType(clang::TypeLocBuilder&, clang::TemplateSpecializationTypeLoc) SemaTemplateInstantiate.cpp:0:0
#13 0x000055a150c9e94f clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
#14 0x000055a150c9f7af clang::Sema::SubstFunctionDeclType(clang::TypeSourceInfo*, clang::MultiLevelTemplateArgumentList const&, clang::SourceLocation, clang::DeclarationName, clang::CXXRecordDecl*, clang::Qualifiers, bool) (clang+++0x75017af)
#15 0x000055a150d01078 clang::TemplateDeclInstantiator::SubstFunctionType(clang::FunctionDecl*, llvm::SmallVectorImpl<clang::ParmVarDecl*>&) (clang+++0x7563078)
#16 0x000055a150cfdc16 clang::TemplateDeclInstantiator::VisitCXXMethodDecl(clang::CXXMethodDecl*, clang::TemplateParameterList*, clang::TemplateDeclInstantiator::RewriteKind) (clang+++0x755fc16)
#17 0x000055a150d4d704 void llvm::function_ref<void ()>::callback_fn<clang::Sema::SubstDecl(clang::Decl*, clang::DeclContext*, clang::MultiLevelTemplateArgumentList const&)::$_0>(long) SemaTemplateInstantiateDecl.cpp:0:0
#18 0x000055a14e7de0ad clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (clang+++0x50400ad)
#19 0x000055a150d086f3 clang::Sema::SubstDecl(clang::Decl*, clang::DeclContext*, clang::MultiLevelTemplateArgumentList const&) (clang+++0x756a6f3)
#20 0x000055a150bf9d6c clang::Sema::FinishTemplateArgumentDeduction(clang::FunctionTemplateDecl*, llvm::SmallVectorImpl<clang::DeducedTemplateArgument>&, unsigned int, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, llvm::SmallVectorImpl<clang::Sema::OriginalCallArg> const*, bool, bool, bool, llvm::function_ref<bool (bool)>) (clang+++0x745bd6c)
#21 0x000055a150c50b03 void llvm::function_ref<void ()>::callback_fn<clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, bool, bool, clang::QualType, clang::Expr::Classification, bool, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>, bool)>)::$_2>(long) SemaTemplateDeduction.cpp:0:0
#22 0x000055a14e7de0ad clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (clang+++0x50400ad)
#23 0x000055a150bfd69f clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, bool, bool, clang::QualType, clang::Expr::Classification, bool, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>, bool)>) (clang+++0x745f69f)
#24 0x000055a150ad56b8 AddTemplateOverloadCandidateImmediately(clang::Sema&, clang::OverloadCandidateSet&, clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, bool, bool, bool, clang::CallExpr::ADLCallKind, clang::OverloadCandidateParamOrder, bool) SemaOverload.cpp:0:0
#25 0x000055a150ae2abc clang::OverloadCandidateSet::InjectNonDeducedTemplateCandidates(clang::Sema&) (clang+++0x7344abc)
#26 0x000055a150acfa71 clang::OverloadCandidateSet::BestViableFunction(clang::Sema&, clang::SourceLocation, clang::OverloadCandidate*&) (clang+++0x7331a71)
#27 0x000055a150aeb88e clang::Sema::BuildOverloadedCallExpr(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (clang+++0x734d88e)
#28 0x000055a15078a0f3 clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (clang+++0x6fec0f3)
#29 0x000055a1507a208f clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) (clang+++0x700408f)
#30 0x000055a150cb7e99 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCallExpr(clang::CallExpr*) SemaTemplateInstantiate.cpp:0:0
#31 0x000055a150cc0a07 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformRequiresExpr(clang::RequiresExpr*) SemaTemplateInstantiate.cpp:0:0
#32 0x000055a150cb01d0 (anonymous namespace)::TemplateInstantiator::TransformRequiresExpr(clang::RequiresExpr*) SemaTemplateInstantiate.cpp:0:0
#33 0x000055a150ca477c clang::Sema::SubstInitializer(clang::Expr*, clang::MultiLevelTemplateArgumentList const&, bool) (clang+++0x750677c)
#34 0x000055a150d06b58 clang::Sema::InstantiateVariableInitializer(clang::VarDecl*, clang::VarDecl*, clang::MultiLevelTemplateArgumentList const&) (clang+++0x7568b58)
#35 0x000055a150cf8fc5 clang::Sema::BuildVariableInstantiation(clang::VarDecl*, clang::VarDecl*, clang::MultiLevelTemplateArgumentList const&, llvm::SmallVector<clang::Sema::LateInstantiatedAttribute, 1u>*, clang::DeclContext*, clang::LocalInstantiationScope*, bool, clang::VarTemplateSpecializationDecl*) (clang+++0x755afc5)
#36 0x000055a150cf8620 clang::TemplateDeclInstantiator::VisitVarDecl(clang::VarDecl*, bool, llvm::ArrayRef<clang::BindingDecl*>*) (clang+++0x755a620)
#37 0x000055a150ca311d clang::Sema::InstantiateClass(clang::SourceLocation, clang::CXXRecordDecl*, clang::CXXRecordDecl*, clang::MultiLevelTemplateArgumentList const&, clang::TemplateSpecializationKind, bool) (clang+++0x750511d)
#38 0x000055a150ca572a clang::Sema::InstantiateClassTemplateSpecialization(clang::SourceLocation, clang::ClassTemplateSpecializationDecl*, clang::TemplateSpecializationKind, bool, bool) (clang+++0x750772a)
#39 0x000055a150d8018e void llvm::function_ref<void ()>::callback_fn<clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*)::$_0>(long) SemaType.cpp:0:0
#40 0x000055a14e7de0ad clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (clang+++0x50400ad)
#41 0x000055a150d67858 clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*) (clang+++0x75c9858)
#42 0x000055a150d6709f clang::Sema::RequireCompleteType(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser&) (clang+++0x75c909f)
#43 0x000055a1508a225e clang::Sema::BuildCXXTypeConstructExpr(clang::TypeSourceInfo*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, bool) (clang+++0x710425e)
#44 0x000055a150cb8b41 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCXXUnresolvedConstructExpr(clang::CXXUnresolvedConstructExpr*) SemaTemplateInstantiate.cpp:0:0
#45 0x000055a150cbad52 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCXXFoldExpr(clang::CXXFoldExpr*) SemaTemplateInstantiate.cpp:0:0
#46 0x000055a150ca477c clang::Sema::SubstInitializer(clang::Expr*, clang::MultiLevelTemplateArgumentList const&, bool) (clang+++0x750677c)
#47 0x000055a150d06b58 clang::Sema::InstantiateVariableInitializer(clang::VarDecl*, clang::VarDecl*, clang::MultiLevelTemplateArgumentList const&) (clang+++0x7568b58)
#48 0x000055a150cf8fc5 clang::Sema::BuildVariableInstantiation(clang::VarDecl*, clang::VarDecl*, clang::MultiLevelTemplateArgumentList const&, llvm::SmallVector<clang::Sema::LateInstantiatedAttribute, 1u>*, clang::DeclContext*, clang::LocalInstantiationScope*, bool, clang::VarTemplateSpecializationDecl*) (clang+++0x755afc5)
#49 0x000055a150d0838e clang::TemplateDeclInstantiator::VisitVarTemplateSpecializationDecl(clang::VarTemplateDecl*, clang::VarDecl*, llvm::ArrayRef<clang::TemplateArgument>, clang::VarTemplateSpecializationDecl*) (clang+++0x756a38e)
#50 0x000055a150d0d2d7 clang::Sema::BuildVarTemplateInstantiation(clang::VarTemplateDecl*, clang::VarDecl*, clang::TemplateArgumentList const*, llvm::SmallVectorImpl<clang::TemplateArgument>&, clang::SourceLocation, llvm::SmallVector<clang::Sema::LateInstantiatedAttribute, 1u>*, clang::LocalInstantiationScope*) (clang+++0x756f2d7)
#51 0x000055a150b9d5dd clang::Sema::CheckVarTemplateId(clang::VarTemplateDecl*, clang::SourceLocation, clang::SourceLocation, clang::TemplateArgumentListInfo const&, bool) (clang+++0x73ff5dd)
#52 0x000055a150b9e2df clang::Sema::CheckVarTemplateId(clang::CXXScopeSpec const&, clang::DeclarationNameInfo const&, clang::VarTemplateDecl*, clang::NamedDecl*, clang::SourceLocation, clang::TemplateArgumentListInfo const*) (clang+++0x74002df)
#53 0x000055a150b9f04e clang::Sema::BuildTemplateIdExpr(clang::CXXScopeSpec const&, clang::SourceLocation, clang::LookupResult&, bool, clang::TemplateArgumentListInfo const*) (clang+++0x740104e)
#54 0x000055a150789625 clang::Sema::ActOnIdExpression(clang::Scope*, clang::CXXScopeSpec&, clang::SourceLocation, clang::UnqualifiedId&, bool, bool, clang::CorrectionCandidateCallback*, bool, clang::Token*) (clang+++0x6feb625)
#55 0x000055a15038a77b clang::Parser::tryParseCXXIdExpression(clang::CXXScopeSpec&, bool, clang::Token&) (clang+++0x6bec77b)
#56 0x000055a15038bf57 clang::Parser::ParseCXXIdExpression(bool) (clang+++0x6bedf57)
#57 0x000055a15037baa7 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypoCorrectionTypeBehavior, bool, bool*) (clang+++0x6bddaa7)
#58 0x000055a15037b4b4 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypoCorrectionTypeBehavior, bool, bool*) (clang+++0x6bdd4b4)
#59 0x000055a150376752 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (clang+++0x6bd8752)
#60 0x000055a15033c633 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (clang+++0x6b9e633)
#61 0x000055a150339919 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (clang+++0x6b9b919)
#62 0x000055a1504152fa clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (clang+++0x6c772fa)
#63 0x000055a150414adb clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (clang+++0x6c76adb)
#64 0x000055a1504139e8 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (clang+++0x6c759e8)
#65 0x000055a150411c63 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (clang+++0x6c73c63)
#66 0x000055a1503208be clang::ParseAST(clang::Sema&, bool, bool) (clang+++0x6b828be)
#67 0x000055a14f140214 clang::FrontendAction::Execute() (clang+++0x59a2214)
#68 0x000055a14f0a391d clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (clang+++0x590591d)
#69 0x000055a14f1edac2 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (clang+++0x5a4fac2)
#70 0x000055a14c76e773 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (clang+++0x2fd0773)
#71 0x000055a14c76a465 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#72 0x000055a14c76cad8 int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::$_0>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#73 0x000055a14ee84489 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::$_0>(long) Job.cpp:0:0
#74 0x000055a14e1aa71e llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (clang+++0x4a0c71e)
#75 0x000055a14ee83e94 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (clang+++0x56e5e94)
#76 0x000055a14ee453b8 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (clang+++0x56a73b8)
#77 0x000055a14ee455f7 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (clang+++0x56a75f7)
#78 0x000055a14ee61259 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (clang+++0x56c3259)
#79 0x000055a14c769cf8 clang_main(int, char**, llvm::ToolContext const&) (clang+++0x2fcbcf8)
#80 0x000055a14c77a6d6 main (clang+++0x2fdc6d6)
#81 0x00007f530c627675 (/usr/lib/libc.so.6+0x27675)
#82 0x00007f530c627729 __libc_start_main (/usr/lib/libc.so.6+0x27729)
#83 0x000055a14c767de5 _start (clang+++0x2fc9de5)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

Broken in 018ae02785da5a35bea8a6eb04f6398c052228f9 @mizvekov 

See it live after the next nightly clang update: https://godbolt.org/z/zY4svWz1a

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: David Stone (davidstone)

<details>
The following valid translation unit

```c++
enum e { };

template&lt;e&gt;
struct s {
};

template&lt;typename&gt;
struct specialized;

template&lt;e x&gt;
struct specialized&lt;s&lt;x&gt;&gt; {
	static auto make(auto) -&gt; s&lt;x&gt;;
};

template&lt;e x&gt;
struct check {
	static constexpr auto m = requires { specialized&lt;s&lt;x&gt;&gt;::make(0); };
};

template&lt;typename... Ts&gt;
auto comma = (..., Ts());

auto b = comma&lt;check&lt;e{}&gt;&gt;;
```

causes clang to crash with

```console
clang++: llvm/clang/include/clang/AST/TypeBase.h:953: const ExtQualsTypeCommonBase *clang::QualType::getCommonPtr() const: Assertion `!isNull() &amp;&amp; "Cannot retrieve a NULL type pointer"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: clang++ -gdwarf-4 -g -o /tmp/compiler-explorer-compiler202599-997-4cgv05.qyzo4/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 &lt;source&gt;
1.	&lt;source&gt;:23:27: current parser token ';'
2.	&lt;source&gt;:16:8: instantiating class definition 'check&lt;0&gt;'
 #<!-- -->0 0x000055a14e278c06 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (clang+++0x4adac06)
 #<!-- -->1 0x000055a14e276155 llvm::sys::RunSignalHandlers() (clang+++0x4ad8155)
 #<!-- -->2 0x000055a14e1aaaed CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007f530c63e540 (/usr/lib/libc.so.6+0x3e540)
 #<!-- -->4 0x00007f530c69894c (/usr/lib/libc.so.6+0x9894c)
 #<!-- -->5 0x00007f530c63e410 raise (/usr/lib/libc.so.6+0x3e410)
 #<!-- -->6 0x00007f530c62557a abort (/usr/lib/libc.so.6+0x2557a)
 #<!-- -->7 0x00007f530c6254e3 __assert_perror_fail (/usr/lib/libc.so.6+0x254e3)
 #<!-- -->8 0x000055a14e919e02 clang::ASTContext::hasSameType(clang::QualType, clang::QualType) const CGCoroutine.cpp:0:0
 #<!-- -->9 0x000055a150caee7e clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformSubstNonTypeTemplateParmExpr(clang::SubstNonTypeTemplateParmExpr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->10 0x000055a150ca6ff5 (anonymous namespace)::TemplateInstantiator::TransformTemplateArgument(clang::TemplateArgumentLoc const&amp;, clang::TemplateArgumentLoc&amp;, bool) SemaTemplateInstantiate.cpp:0:0
#<!-- -->11 0x000055a150cc7f4a clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformTemplateSpecializationType(clang::TypeLocBuilder&amp;, clang::TemplateSpecializationTypeLoc, clang::QualType, clang::NamedDecl*, bool) SemaTemplateInstantiate.cpp:0:0
#<!-- -->12 0x000055a150cd04bf (anonymous namespace)::TemplateInstantiator::TransformTemplateSpecializationType(clang::TypeLocBuilder&amp;, clang::TemplateSpecializationTypeLoc) SemaTemplateInstantiate.cpp:0:0
#<!-- -->13 0x000055a150c9e94f clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeLocBuilder&amp;, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
#<!-- -->14 0x000055a150c9f7af clang::Sema::SubstFunctionDeclType(clang::TypeSourceInfo*, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceLocation, clang::DeclarationName, clang::CXXRecordDecl*, clang::Qualifiers, bool) (clang+++0x75017af)
#<!-- -->15 0x000055a150d01078 clang::TemplateDeclInstantiator::SubstFunctionType(clang::FunctionDecl*, llvm::SmallVectorImpl&lt;clang::ParmVarDecl*&gt;&amp;) (clang+++0x7563078)
#<!-- -->16 0x000055a150cfdc16 clang::TemplateDeclInstantiator::VisitCXXMethodDecl(clang::CXXMethodDecl*, clang::TemplateParameterList*, clang::TemplateDeclInstantiator::RewriteKind) (clang+++0x755fc16)
#<!-- -->17 0x000055a150d4d704 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::Sema::SubstDecl(clang::Decl*, clang::DeclContext*, clang::MultiLevelTemplateArgumentList const&amp;)::$_0&gt;(long) SemaTemplateInstantiateDecl.cpp:0:0
#<!-- -->18 0x000055a14e7de0ad clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (clang+++0x50400ad)
#<!-- -->19 0x000055a150d086f3 clang::Sema::SubstDecl(clang::Decl*, clang::DeclContext*, clang::MultiLevelTemplateArgumentList const&amp;) (clang+++0x756a6f3)
#<!-- -->20 0x000055a150bf9d6c clang::Sema::FinishTemplateArgumentDeduction(clang::FunctionTemplateDecl*, llvm::SmallVectorImpl&lt;clang::DeducedTemplateArgument&gt;&amp;, unsigned int, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, llvm::SmallVectorImpl&lt;clang::Sema::OriginalCallArg&gt; const*, bool, bool, bool, llvm::function_ref&lt;bool (bool)&gt;) (clang+++0x745bd6c)
#<!-- -->21 0x000055a150c50b03 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, bool, bool, bool, clang::QualType, clang::Expr::Classification, bool, llvm::function_ref&lt;bool (llvm::ArrayRef&lt;clang::QualType&gt;, bool)&gt;)::$_2&gt;(long) SemaTemplateDeduction.cpp:0:0
#<!-- -->22 0x000055a14e7de0ad clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (clang+++0x50400ad)
#<!-- -->23 0x000055a150bfd69f clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, bool, bool, bool, clang::QualType, clang::Expr::Classification, bool, llvm::function_ref&lt;bool (llvm::ArrayRef&lt;clang::QualType&gt;, bool)&gt;) (clang+++0x745f69f)
#<!-- -->24 0x000055a150ad56b8 AddTemplateOverloadCandidateImmediately(clang::Sema&amp;, clang::OverloadCandidateSet&amp;, clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, bool, bool, bool, clang::CallExpr::ADLCallKind, clang::OverloadCandidateParamOrder, bool) SemaOverload.cpp:0:0
#<!-- -->25 0x000055a150ae2abc clang::OverloadCandidateSet::InjectNonDeducedTemplateCandidates(clang::Sema&amp;) (clang+++0x7344abc)
#<!-- -->26 0x000055a150acfa71 clang::OverloadCandidateSet::BestViableFunction(clang::Sema&amp;, clang::SourceLocation, clang::OverloadCandidate*&amp;) (clang+++0x7331a71)
#<!-- -->27 0x000055a150aeb88e clang::Sema::BuildOverloadedCallExpr(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, bool) (clang+++0x734d88e)
#<!-- -->28 0x000055a15078a0f3 clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, bool) (clang+++0x6fec0f3)
#<!-- -->29 0x000055a1507a208f clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*) (clang+++0x700408f)
#<!-- -->30 0x000055a150cb7e99 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCallExpr(clang::CallExpr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->31 0x000055a150cc0a07 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformRequiresExpr(clang::RequiresExpr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->32 0x000055a150cb01d0 (anonymous namespace)::TemplateInstantiator::TransformRequiresExpr(clang::RequiresExpr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->33 0x000055a150ca477c clang::Sema::SubstInitializer(clang::Expr*, clang::MultiLevelTemplateArgumentList const&amp;, bool) (clang+++0x750677c)
#<!-- -->34 0x000055a150d06b58 clang::Sema::InstantiateVariableInitializer(clang::VarDecl*, clang::VarDecl*, clang::MultiLevelTemplateArgumentList const&amp;) (clang+++0x7568b58)
#<!-- -->35 0x000055a150cf8fc5 clang::Sema::BuildVariableInstantiation(clang::VarDecl*, clang::VarDecl*, clang::MultiLevelTemplateArgumentList const&amp;, llvm::SmallVector&lt;clang::Sema::LateInstantiatedAttribute, 1u&gt;*, clang::DeclContext*, clang::LocalInstantiationScope*, bool, clang::VarTemplateSpecializationDecl*) (clang+++0x755afc5)
#<!-- -->36 0x000055a150cf8620 clang::TemplateDeclInstantiator::VisitVarDecl(clang::VarDecl*, bool, llvm::ArrayRef&lt;clang::BindingDecl*&gt;*) (clang+++0x755a620)
#<!-- -->37 0x000055a150ca311d clang::Sema::InstantiateClass(clang::SourceLocation, clang::CXXRecordDecl*, clang::CXXRecordDecl*, clang::MultiLevelTemplateArgumentList const&amp;, clang::TemplateSpecializationKind, bool) (clang+++0x750511d)
#<!-- -->38 0x000055a150ca572a clang::Sema::InstantiateClassTemplateSpecialization(clang::SourceLocation, clang::ClassTemplateSpecializationDecl*, clang::TemplateSpecializationKind, bool, bool) (clang+++0x750772a)
#<!-- -->39 0x000055a150d8018e void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*)::$_0&gt;(long) SemaType.cpp:0:0
#<!-- -->40 0x000055a14e7de0ad clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (clang+++0x50400ad)
#<!-- -->41 0x000055a150d67858 clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*) (clang+++0x75c9858)
#<!-- -->42 0x000055a150d6709f clang::Sema::RequireCompleteType(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser&amp;) (clang+++0x75c909f)
#<!-- -->43 0x000055a1508a225e clang::Sema::BuildCXXTypeConstructExpr(clang::TypeSourceInfo*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, bool) (clang+++0x710425e)
#<!-- -->44 0x000055a150cb8b41 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCXXUnresolvedConstructExpr(clang::CXXUnresolvedConstructExpr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->45 0x000055a150cbad52 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCXXFoldExpr(clang::CXXFoldExpr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->46 0x000055a150ca477c clang::Sema::SubstInitializer(clang::Expr*, clang::MultiLevelTemplateArgumentList const&amp;, bool) (clang+++0x750677c)
#<!-- -->47 0x000055a150d06b58 clang::Sema::InstantiateVariableInitializer(clang::VarDecl*, clang::VarDecl*, clang::MultiLevelTemplateArgumentList const&amp;) (clang+++0x7568b58)
#<!-- -->48 0x000055a150cf8fc5 clang::Sema::BuildVariableInstantiation(clang::VarDecl*, clang::VarDecl*, clang::MultiLevelTemplateArgumentList const&amp;, llvm::SmallVector&lt;clang::Sema::LateInstantiatedAttribute, 1u&gt;*, clang::DeclContext*, clang::LocalInstantiationScope*, bool, clang::VarTemplateSpecializationDecl*) (clang+++0x755afc5)
#<!-- -->49 0x000055a150d0838e clang::TemplateDeclInstantiator::VisitVarTemplateSpecializationDecl(clang::VarTemplateDecl*, clang::VarDecl*, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::VarTemplateSpecializationDecl*) (clang+++0x756a38e)
#<!-- -->50 0x000055a150d0d2d7 clang::Sema::BuildVarTemplateInstantiation(clang::VarTemplateDecl*, clang::VarDecl*, clang::TemplateArgumentList const*, llvm::SmallVectorImpl&lt;clang::TemplateArgument&gt;&amp;, clang::SourceLocation, llvm::SmallVector&lt;clang::Sema::LateInstantiatedAttribute, 1u&gt;*, clang::LocalInstantiationScope*) (clang+++0x756f2d7)
#<!-- -->51 0x000055a150b9d5dd clang::Sema::CheckVarTemplateId(clang::VarTemplateDecl*, clang::SourceLocation, clang::SourceLocation, clang::TemplateArgumentListInfo const&amp;, bool) (clang+++0x73ff5dd)
#<!-- -->52 0x000055a150b9e2df clang::Sema::CheckVarTemplateId(clang::CXXScopeSpec const&amp;, clang::DeclarationNameInfo const&amp;, clang::VarTemplateDecl*, clang::NamedDecl*, clang::SourceLocation, clang::TemplateArgumentListInfo const*) (clang+++0x74002df)
#<!-- -->53 0x000055a150b9f04e clang::Sema::BuildTemplateIdExpr(clang::CXXScopeSpec const&amp;, clang::SourceLocation, clang::LookupResult&amp;, bool, clang::TemplateArgumentListInfo const*) (clang+++0x740104e)
#<!-- -->54 0x000055a150789625 clang::Sema::ActOnIdExpression(clang::Scope*, clang::CXXScopeSpec&amp;, clang::SourceLocation, clang::UnqualifiedId&amp;, bool, bool, clang::CorrectionCandidateCallback*, bool, clang::Token*) (clang+++0x6feb625)
#<!-- -->55 0x000055a15038a77b clang::Parser::tryParseCXXIdExpression(clang::CXXScopeSpec&amp;, bool, clang::Token&amp;) (clang+++0x6bec77b)
#<!-- -->56 0x000055a15038bf57 clang::Parser::ParseCXXIdExpression(bool) (clang+++0x6bedf57)
#<!-- -->57 0x000055a15037baa7 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&amp;, clang::TypoCorrectionTypeBehavior, bool, bool*) (clang+++0x6bddaa7)
#<!-- -->58 0x000055a15037b4b4 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&amp;, clang::TypoCorrectionTypeBehavior, bool, bool*) (clang+++0x6bdd4b4)
#<!-- -->59 0x000055a150376752 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (clang+++0x6bd8752)
#<!-- -->60 0x000055a15033c633 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) (clang+++0x6b9e633)
#<!-- -->61 0x000055a150339919 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (clang+++0x6b9b919)
#<!-- -->62 0x000055a1504152fa clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (clang+++0x6c772fa)
#<!-- -->63 0x000055a150414adb clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (clang+++0x6c76adb)
#<!-- -->64 0x000055a1504139e8 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (clang+++0x6c759e8)
#<!-- -->65 0x000055a150411c63 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (clang+++0x6c73c63)
#<!-- -->66 0x000055a1503208be clang::ParseAST(clang::Sema&amp;, bool, bool) (clang+++0x6b828be)
#<!-- -->67 0x000055a14f140214 clang::FrontendAction::Execute() (clang+++0x59a2214)
#<!-- -->68 0x000055a14f0a391d clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (clang+++0x590591d)
#<!-- -->69 0x000055a14f1edac2 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (clang+++0x5a4fac2)
#<!-- -->70 0x000055a14c76e773 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (clang+++0x2fd0773)
#<!-- -->71 0x000055a14c76a465 ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->72 0x000055a14c76cad8 int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::$_0&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->73 0x000055a14ee84489 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::$_0&gt;(long) Job.cpp:0:0
#<!-- -->74 0x000055a14e1aa71e llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (clang+++0x4a0c71e)
#<!-- -->75 0x000055a14ee83e94 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (clang+++0x56e5e94)
#<!-- -->76 0x000055a14ee453b8 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (clang+++0x56a73b8)
#<!-- -->77 0x000055a14ee455f7 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (clang+++0x56a75f7)
#<!-- -->78 0x000055a14ee61259 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (clang+++0x56c3259)
#<!-- -->79 0x000055a14c769cf8 clang_main(int, char**, llvm::ToolContext const&amp;) (clang+++0x2fcbcf8)
#<!-- -->80 0x000055a14c77a6d6 main (clang+++0x2fdc6d6)
#<!-- -->81 0x00007f530c627675 (/usr/lib/libc.so.6+0x27675)
#<!-- -->82 0x00007f530c627729 __libc_start_main (/usr/lib/libc.so.6+0x27729)
#<!-- -->83 0x000055a14c767de5 _start (clang+++0x2fc9de5)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

Broken in 018ae02785da5a35bea8a6eb04f6398c052228f9 @<!-- -->mizvekov 

See it live after the next nightly clang update: https://godbolt.org/z/zY4svWz1a
</details>


---

