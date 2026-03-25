# Assertion `Idx < NumArguments && "Invalid template argument index"' failed

**Author:** gregbedwell
**URL:** https://github.com/llvm/llvm-project/issues/162855
**Status:** Closed
**Labels:** clang:frontend, regression, confirmed, crash-on-valid
**Closed Date:** 2025-10-11T02:00:37Z

## Body

Very recent regression spotted in our downstream CI testing. Not bisected yet.

https://godbolt.org/z/nWeb1zhsY

```
// ===========================================================================
template <typename...> using a = int;
template <typename...> struct b;
template <int> struct c;
template <class, int d> using e = c<d>;
template <typename> struct f;
template <template <typename, unsigned...> class g, typename h, long... i,
          typename... j>
struct b<g<h, i...>, j...> {
  template <typename... k> using l = a<e<k, i>...>;
};
b<f<int>> m;
// ---------------------------------------------------------------------------
```

```
clang++: /root/llvm-project/llvm/tools/clang/include/clang/AST/DeclTemplate.h:272: const clang::TemplateArgument& clang::TemplateArgumentList::get(unsigned int) const: Assertion `Idx < NumArguments && "Invalid template argument index"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -fno-verbose-asm -c --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<source>:12:12: current parser token ';'
2.	<source>:3:31: instantiating class definition 'b<f<int>>'
 #0 0x00000000041e14e8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41e14e8)
 #1 0x00000000041de914 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41de914)
 #2 0x0000000004123268 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007e2df1442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007e2df14969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007e2df1442476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007e2df14287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007e2df142871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007e2df1439e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000007a023ea clang::getReplacedTemplateParameter(clang::Decl*, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7a023ea)
#10 0x0000000007bb79c7 clang::SubstNonTypeTemplateParmPackExpr::getParameterPack() const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7bb79c7)
#11 0x0000000007ee635f clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::StmtProfiler, void>::Visit(clang::Stmt const*) StmtProfile.cpp:0:0
#12 0x0000000007ee7ee1 (anonymous namespace)::StmtProfiler::VisitStmt(clang::Stmt const*) StmtProfile.cpp:0:0
#13 0x0000000007ee6fc8 clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::StmtProfiler, void>::Visit(clang::Stmt const*) StmtProfile.cpp:0:0
#14 0x0000000007eee4b0 clang::Stmt::Profile(llvm::FoldingSetNodeID&, clang::ASTContext const&, bool, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7eee4b0)
#15 0x0000000007ef4bc6 clang::TemplateArgument::Profile(llvm::FoldingSetNodeID&, clang::ASTContext const&) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7ef4bc6)
#16 0x0000000007f45962 clang::TemplateSpecializationType::Profile(llvm::FoldingSetNodeID&, clang::ElaboratedTypeKeyword, clang::TemplateName, llvm::ArrayRef<clang::TemplateArgument>, clang::QualType, clang::ASTContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7f45962)
#17 0x00000000077e5972 clang::ASTContext::getCanonicalTemplateSpecializationType(clang::ElaboratedTypeKeyword, clang::TemplateName, llvm::ArrayRef<clang::TemplateArgument>) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x77e5972)
#18 0x00000000072ea044 clang::Sema::CheckTemplateIdType(clang::ElaboratedTypeKeyword, clang::TemplateName, clang::SourceLocation, clang::TemplateArgumentListInfo&, clang::Scope*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x72ea044)
#19 0x00000000074368e4 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformTemplateSpecializationType(clang::TypeLocBuilder&, clang::TemplateSpecializationTypeLoc, clang::QualType, clang::NamedDecl*, bool) SemaTemplateInstantiate.cpp:0:0
#20 0x000000000740e9e5 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
#21 0x000000000741350a clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeSourceInfo*) SemaTemplateInstantiate.cpp:0:0
#22 0x000000000741362c clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::QualType) SemaTemplateInstantiate.cpp:0:0
#23 0x0000000007413cca clang::Sema::SubstType(clang::QualType, clang::MultiLevelTemplateArgumentList const&, clang::SourceLocation, clang::DeclarationName, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7413cca)
#24 0x00000000072ea1cb clang::Sema::CheckTemplateIdType(clang::ElaboratedTypeKeyword, clang::TemplateName, clang::SourceLocation, clang::TemplateArgumentListInfo&, clang::Scope*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x72ea1cb)
#25 0x00000000074368e4 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformTemplateSpecializationType(clang::TypeLocBuilder&, clang::TemplateSpecializationTypeLoc, clang::QualType, clang::NamedDecl*, bool) SemaTemplateInstantiate.cpp:0:0
#26 0x000000000740e9e5 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
#27 0x000000000741350a clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeSourceInfo*) SemaTemplateInstantiate.cpp:0:0
#28 0x000000000741c2fb clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformTemplateArgument(clang::TemplateArgumentLoc const&, clang::TemplateArgumentLoc&, bool) SemaTemplateInstantiate.cpp:0:0
#29 0x000000000741ca38 (anonymous namespace)::TemplateInstantiator::TransformTemplateArgument(clang::TemplateArgumentLoc const&, clang::TemplateArgumentLoc&, bool) SemaTemplateInstantiate.cpp:0:0
#30 0x000000000742ef59 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::PreparePackForExpansion(clang::TemplateArgumentLoc, bool, clang::TemplateArgumentLoc&, clang::UnexpandedInfo&) (.constprop.0) SemaTemplateInstantiate.cpp:0:0
#31 0x0000000007436190 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformTemplateSpecializationType(clang::TypeLocBuilder&, clang::TemplateSpecializationTypeLoc, clang::QualType, clang::NamedDecl*, bool) SemaTemplateInstantiate.cpp:0:0
#32 0x000000000740e9e5 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
#33 0x000000000741350a clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeSourceInfo*) SemaTemplateInstantiate.cpp:0:0
#34 0x0000000007418aed clang::Sema::SubstType(clang::TypeSourceInfo*, clang::MultiLevelTemplateArgumentList const&, clang::SourceLocation, clang::DeclarationName, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7418aed)
#35 0x00000000074b87f1 clang::TemplateDeclInstantiator::InstantiateTypedefNameDecl(clang::TypedefNameDecl*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x74b87f1)
#36 0x00000000074bfe98 clang::TemplateDeclInstantiator::InstantiateTypeAliasTemplateDecl(clang::TypeAliasTemplateDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x74bfe98)
#37 0x00000000074c011f clang::TemplateDeclInstantiator::VisitTypeAliasTemplateDecl(clang::TypeAliasTemplateDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x74c011f)
#38 0x000000000741a353 clang::Sema::InstantiateClass(clang::SourceLocation, clang::CXXRecordDecl*, clang::CXXRecordDecl*, clang::MultiLevelTemplateArgumentList const&, clang::TemplateSpecializationKind, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x741a353)
#39 0x000000000744bf18 clang::Sema::InstantiateClassTemplateSpecialization(clang::SourceLocation, clang::ClassTemplateSpecializationDecl*, clang::TemplateSpecializationKind, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x744bf18)
#40 0x00000000074cb95e void llvm::function_ref<void ()>::callback_fn<clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*)::'lambda'()>(long) SemaType.cpp:0:0
#41 0x00000000081a6d91 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x81a6d91)
#42 0x00000000074d7373 clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x74d7373)
#43 0x00000000074d780f clang::Sema::RequireCompleteType(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x74d780f)
#44 0x00000000074d7e75 clang::Sema::RequireCompleteType(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x74d7e75)
#45 0x0000000006ca6332 clang::Sema::ActOnUninitializedDecl(clang::Decl*) (.part.0) SemaDecl.cpp:0:0
#46 0x000000000690713b clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x690713b)
#47 0x0000000006914e59 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6914e59)
#48 0x00000000068d27be clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68d27be)
#49 0x00000000068d2f5f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68d2f5f)
#50 0x00000000068da813 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68da813)
#51 0x00000000068db725 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68db725)
#52 0x00000000068bcd8a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68bcd8a)
#53 0x0000000004b25988 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b25988)
#54 0x0000000004e11975 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e11975)
#55 0x0000000004d930ae clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d930ae)
#56 0x0000000004f08c2d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f08c2d)
#57 0x0000000000dbf5b0 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdbf5b0)
#58 0x0000000000db611a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#59 0x0000000000db629d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#60 0x0000000004b8eb29 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#61 0x0000000004123704 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4123704)
#62 0x0000000004b8f13f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#63 0x0000000004b50442 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b50442)
#64 0x0000000004b513ee clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b513ee)
#65 0x0000000004b58b25 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b58b25)
#66 0x0000000000dbbad9 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdbbad9)
#67 0x0000000000c6bad4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc6bad4)
#68 0x00007e2df1429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#69 0x00007e2df1429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#70 0x0000000000db5bb5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb5bb5)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Greg Bedwell (gregbedwell)

<details>
Very recent regression spotted in our downstream CI testing. Not bisected yet.

https://godbolt.org/z/nWeb1zhsY

```
// ===========================================================================
template &lt;typename...&gt; using a = int;
template &lt;typename...&gt; struct b;
template &lt;int&gt; struct c;
template &lt;class, int d&gt; using e = c&lt;d&gt;;
template &lt;typename&gt; struct f;
template &lt;template &lt;typename, unsigned...&gt; class g, typename h, long... i,
          typename... j&gt;
struct b&lt;g&lt;h, i...&gt;, j...&gt; {
  template &lt;typename... k&gt; using l = a&lt;e&lt;k, i&gt;...&gt;;
};
b&lt;f&lt;int&gt;&gt; m;
// ---------------------------------------------------------------------------
```

```
clang++: /root/llvm-project/llvm/tools/clang/include/clang/AST/DeclTemplate.h:272: const clang::TemplateArgument&amp; clang::TemplateArgumentList::get(unsigned int) const: Assertion `Idx &lt; NumArguments &amp;&amp; "Invalid template argument index"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -fno-verbose-asm -c --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics &lt;source&gt;
1.	&lt;source&gt;:12:12: current parser token ';'
2.	&lt;source&gt;:3:31: instantiating class definition 'b&lt;f&lt;int&gt;&gt;'
 #<!-- -->0 0x00000000041e14e8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41e14e8)
 #<!-- -->1 0x00000000041de914 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41de914)
 #<!-- -->2 0x0000000004123268 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007e2df1442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007e2df14969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007e2df1442476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007e2df14287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x00007e2df142871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x00007e2df1439e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000007a023ea clang::getReplacedTemplateParameter(clang::Decl*, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7a023ea)
#<!-- -->10 0x0000000007bb79c7 clang::SubstNonTypeTemplateParmPackExpr::getParameterPack() const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7bb79c7)
#<!-- -->11 0x0000000007ee635f clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::StmtProfiler, void&gt;::Visit(clang::Stmt const*) StmtProfile.cpp:0:0
#<!-- -->12 0x0000000007ee7ee1 (anonymous namespace)::StmtProfiler::VisitStmt(clang::Stmt const*) StmtProfile.cpp:0:0
#<!-- -->13 0x0000000007ee6fc8 clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::StmtProfiler, void&gt;::Visit(clang::Stmt const*) StmtProfile.cpp:0:0
#<!-- -->14 0x0000000007eee4b0 clang::Stmt::Profile(llvm::FoldingSetNodeID&amp;, clang::ASTContext const&amp;, bool, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7eee4b0)
#<!-- -->15 0x0000000007ef4bc6 clang::TemplateArgument::Profile(llvm::FoldingSetNodeID&amp;, clang::ASTContext const&amp;) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7ef4bc6)
#<!-- -->16 0x0000000007f45962 clang::TemplateSpecializationType::Profile(llvm::FoldingSetNodeID&amp;, clang::ElaboratedTypeKeyword, clang::TemplateName, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::QualType, clang::ASTContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7f45962)
#<!-- -->17 0x00000000077e5972 clang::ASTContext::getCanonicalTemplateSpecializationType(clang::ElaboratedTypeKeyword, clang::TemplateName, llvm::ArrayRef&lt;clang::TemplateArgument&gt;) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x77e5972)
#<!-- -->18 0x00000000072ea044 clang::Sema::CheckTemplateIdType(clang::ElaboratedTypeKeyword, clang::TemplateName, clang::SourceLocation, clang::TemplateArgumentListInfo&amp;, clang::Scope*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x72ea044)
#<!-- -->19 0x00000000074368e4 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformTemplateSpecializationType(clang::TypeLocBuilder&amp;, clang::TemplateSpecializationTypeLoc, clang::QualType, clang::NamedDecl*, bool) SemaTemplateInstantiate.cpp:0:0
#<!-- -->20 0x000000000740e9e5 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeLocBuilder&amp;, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
#<!-- -->21 0x000000000741350a clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeSourceInfo*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->22 0x000000000741362c clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::QualType) SemaTemplateInstantiate.cpp:0:0
#<!-- -->23 0x0000000007413cca clang::Sema::SubstType(clang::QualType, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceLocation, clang::DeclarationName, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7413cca)
#<!-- -->24 0x00000000072ea1cb clang::Sema::CheckTemplateIdType(clang::ElaboratedTypeKeyword, clang::TemplateName, clang::SourceLocation, clang::TemplateArgumentListInfo&amp;, clang::Scope*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x72ea1cb)
#<!-- -->25 0x00000000074368e4 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformTemplateSpecializationType(clang::TypeLocBuilder&amp;, clang::TemplateSpecializationTypeLoc, clang::QualType, clang::NamedDecl*, bool) SemaTemplateInstantiate.cpp:0:0
#<!-- -->26 0x000000000740e9e5 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeLocBuilder&amp;, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
#<!-- -->27 0x000000000741350a clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeSourceInfo*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->28 0x000000000741c2fb clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformTemplateArgument(clang::TemplateArgumentLoc const&amp;, clang::TemplateArgumentLoc&amp;, bool) SemaTemplateInstantiate.cpp:0:0
#<!-- -->29 0x000000000741ca38 (anonymous namespace)::TemplateInstantiator::TransformTemplateArgument(clang::TemplateArgumentLoc const&amp;, clang::TemplateArgumentLoc&amp;, bool) SemaTemplateInstantiate.cpp:0:0
#<!-- -->30 0x000000000742ef59 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::PreparePackForExpansion(clang::TemplateArgumentLoc, bool, clang::TemplateArgumentLoc&amp;, clang::UnexpandedInfo&amp;) (.constprop.0) SemaTemplateInstantiate.cpp:0:0
#<!-- -->31 0x0000000007436190 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformTemplateSpecializationType(clang::TypeLocBuilder&amp;, clang::TemplateSpecializationTypeLoc, clang::QualType, clang::NamedDecl*, bool) SemaTemplateInstantiate.cpp:0:0
#<!-- -->32 0x000000000740e9e5 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeLocBuilder&amp;, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
#<!-- -->33 0x000000000741350a clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeSourceInfo*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->34 0x0000000007418aed clang::Sema::SubstType(clang::TypeSourceInfo*, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceLocation, clang::DeclarationName, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7418aed)
#<!-- -->35 0x00000000074b87f1 clang::TemplateDeclInstantiator::InstantiateTypedefNameDecl(clang::TypedefNameDecl*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x74b87f1)
#<!-- -->36 0x00000000074bfe98 clang::TemplateDeclInstantiator::InstantiateTypeAliasTemplateDecl(clang::TypeAliasTemplateDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x74bfe98)
#<!-- -->37 0x00000000074c011f clang::TemplateDeclInstantiator::VisitTypeAliasTemplateDecl(clang::TypeAliasTemplateDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x74c011f)
#<!-- -->38 0x000000000741a353 clang::Sema::InstantiateClass(clang::SourceLocation, clang::CXXRecordDecl*, clang::CXXRecordDecl*, clang::MultiLevelTemplateArgumentList const&amp;, clang::TemplateSpecializationKind, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x741a353)
#<!-- -->39 0x000000000744bf18 clang::Sema::InstantiateClassTemplateSpecialization(clang::SourceLocation, clang::ClassTemplateSpecializationDecl*, clang::TemplateSpecializationKind, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x744bf18)
#<!-- -->40 0x00000000074cb95e void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*)::'lambda'()&gt;(long) SemaType.cpp:0:0
#<!-- -->41 0x00000000081a6d91 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x81a6d91)
#<!-- -->42 0x00000000074d7373 clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x74d7373)
#<!-- -->43 0x00000000074d780f clang::Sema::RequireCompleteType(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x74d780f)
#<!-- -->44 0x00000000074d7e75 clang::Sema::RequireCompleteType(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x74d7e75)
#<!-- -->45 0x0000000006ca6332 clang::Sema::ActOnUninitializedDecl(clang::Decl*) (.part.0) SemaDecl.cpp:0:0
#<!-- -->46 0x000000000690713b clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x690713b)
#<!-- -->47 0x0000000006914e59 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6914e59)
#<!-- -->48 0x00000000068d27be clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68d27be)
#<!-- -->49 0x00000000068d2f5f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68d2f5f)
#<!-- -->50 0x00000000068da813 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68da813)
#<!-- -->51 0x00000000068db725 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68db725)
#<!-- -->52 0x00000000068bcd8a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68bcd8a)
#<!-- -->53 0x0000000004b25988 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b25988)
#<!-- -->54 0x0000000004e11975 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e11975)
#<!-- -->55 0x0000000004d930ae clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d930ae)
#<!-- -->56 0x0000000004f08c2d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f08c2d)
#<!-- -->57 0x0000000000dbf5b0 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdbf5b0)
#<!-- -->58 0x0000000000db611a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->59 0x0000000000db629d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->60 0x0000000004b8eb29 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->61 0x0000000004123704 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4123704)
#<!-- -->62 0x0000000004b8f13f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->63 0x0000000004b50442 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b50442)
#<!-- -->64 0x0000000004b513ee clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b513ee)
#<!-- -->65 0x0000000004b58b25 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b58b25)
#<!-- -->66 0x0000000000dbbad9 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdbbad9)
#<!-- -->67 0x0000000000c6bad4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc6bad4)
#<!-- -->68 0x00007e2df1429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->69 0x00007e2df1429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->70 0x0000000000db5bb5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb5bb5)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

### Comment 2 - AaronBallman

CC @mizvekov @erichkeane 

---

### Comment 3 - gregbedwell

Bisects to 018ae02785da5a35bea8a6eb04f6398c052228f9

---

