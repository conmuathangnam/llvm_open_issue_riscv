# [Clang] Crash on CTAD for alias template declared with respect to another alias template

**Author:** MagentaTreehouse
**URL:** https://github.com/llvm/llvm-project/issues/129620
**Status:** Closed
**Labels:** clang:frontend, crash
**Closed Date:** 2025-03-22T14:56:00Z

## Body

```c++
template <class... Ts>
struct A {
    constexpr A(Ts...) {}
};

template <class... Ts>
using Foo = A<Ts...>;

template <class T>
using Bar = Foo<T, T>;

Bar a{0, 0};
```

See https://compiler-explorer.com/z/rzhxc7nPv.

Assertion:
```console
clang++: /root/llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:1474:
static clang::TemplateArgument {anonymous}::TemplateInstantiator::getTemplateArgumentPackPatternForRewrite(const clang::TemplateArgument&):
Assertion `TA.pack_size() == 1 && "unexpected pack arguments in template rewrite"' failed.
```

<details>

<summary>Stack dump</summary>

```console
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 <source>
1.	<source>:12:12: current parser token ';'
 #0 0x0000000003e53898 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3e53898)
 #1 0x0000000003e51554 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3e51554)
 #2 0x0000000003d9de88 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007b90cb442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007b90cb4969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007b90cb442476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007b90cb4287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007b90cb42871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007b90cb439e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000006f462e3 (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f462e3)
#10 0x0000000006f4fa50 clang::declvisitor::Base<std::add_pointer, clang::TemplateDeclInstantiator, clang::Decl*>::Visit(clang::Decl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f4fa50)
#11 0x0000000006f7f52d clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
#12 0x0000000006f83baa clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeSourceInfo*) SemaTemplateInstantiate.cpp:0:0
#13 0x0000000006f83ccc clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::QualType) SemaTemplateInstantiate.cpp:0:0
#14 0x0000000006f84840 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformSubstTemplateTypeParmType(clang::TypeLocBuilder&, clang::SubstTemplateTypeParmTypeLoc) SemaTemplateInstantiate.cpp:0:0
#15 0x0000000006f7f42d clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
#16 0x0000000006f83baa clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeSourceInfo*) SemaTemplateInstantiate.cpp:0:0
#17 0x0000000006fa0bbd (anonymous namespace)::TemplateInstantiator::TransformTemplateArgument(clang::TemplateArgumentLoc const&, clang::TemplateArgumentLoc&, bool) SemaTemplateInstantiate.cpp:0:0
#18 0x0000000006fa294f clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformTemplateSpecializationType(clang::TypeLocBuilder&, clang::TemplateSpecializationTypeLoc, clang::TemplateName) SemaTemplateInstantiate.cpp:0:0
#19 0x0000000006f7ff36 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
#20 0x0000000006f80781 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformTypeTraitExpr(clang::TypeTraitExpr*) SemaTemplateInstantiate.cpp:0:0
#21 0x0000000006f880ff clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#22 0x0000000006fb23d2 clang::Sema::SubstExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6fb23d2)
#23 0x0000000006efde0c (anonymous namespace)::buildAssociatedConstraints(clang::Sema&, clang::FunctionTemplateDecl*, clang::TypeAliasTemplateDecl*, llvm::ArrayRef<clang::DeducedTemplateArgument>, unsigned int, clang::Expr*) SemaTemplateDeductionGuide.cpp:0:0
#24 0x0000000006f018d8 (anonymous namespace)::BuildDeductionGuideForTypeAlias(clang::Sema&, clang::TypeAliasTemplateDecl*, clang::FunctionTemplateDecl*, clang::SourceLocation) SemaTemplateDeductionGuide.cpp:0:0
#25 0x0000000006f43cc5 clang::Sema::DeclareImplicitDeductionGuides(clang::TemplateDecl*, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f43cc5)
#26 0x0000000006b2fd17 LookupDirect(clang::Sema&, clang::LookupResult&, clang::DeclContext const*) SemaLookup.cpp:0:0
#27 0x0000000006b1b012 clang::Sema::LookupQualifiedName(clang::LookupResult&, clang::DeclContext*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b1b012)
#28 0x0000000006af9c2e clang::Sema::DeduceTemplateSpecializationFromInitializer(clang::TypeSourceInfo*, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::MutableArrayRef<clang::Expr*>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6af9c2e)
#29 0x000000000677e43d clang::Sema::deduceVarTypeFromInitializer(clang::VarDecl*, clang::DeclarationName, clang::QualType, clang::TypeSourceInfo*, clang::SourceRange, bool, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x677e43d)
#30 0x0000000006783207 clang::Sema::DeduceVariableDeclarationType(clang::VarDecl*, bool, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6783207)
#31 0x00000000067a19a2 clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67a19a2)
#32 0x0000000006430345 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6430345)
#33 0x000000000643f8c9 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x643f8c9)
#34 0x00000000063ff73e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x63ff73e)
#35 0x00000000063ffef9 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x63ffef9)
#36 0x00000000064076d3 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64076d3)
#37 0x00000000064085ad clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64085ad)
#38 0x00000000063faa3a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x63faa3a)
#39 0x0000000004812598 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4812598)
#40 0x0000000004ada245 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ada245)
#41 0x0000000004a5d92e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4a5d92e)
#42 0x0000000004bc809e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bc809e)
#43 0x0000000000d4ab2f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd4ab2f)
#44 0x0000000000d4244a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#45 0x000000000485a4c9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#46 0x0000000003d9e334 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3d9e334)
#47 0x000000000485aabf clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#48 0x000000000481d6dd clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x481d6dd)
#49 0x000000000481e75e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x481e75e)
#50 0x00000000048266f5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48266f5)
#51 0x0000000000d47923 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd47923)
#52 0x0000000000c11464 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc11464)
#53 0x00007b90cb429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#54 0x00007b90cb429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#55 0x0000000000d41ef5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd41ef5)
```

</details>

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (MagentaTreehouse)

<details>
```c++
template &lt;class... Ts&gt;
struct A {
    constexpr A(Ts...) {}
};

template &lt;class... Ts&gt;
using Foo = A&lt;Ts...&gt;;

template &lt;class T&gt;
using Bar = Foo&lt;T, T&gt;;

Bar a{0, 0};
```

See https://compiler-explorer.com/z/rzhxc7nPv.

Assertion:
```console
clang++: /root/llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:1474:
static clang::TemplateArgument {anonymous}::TemplateInstantiator::getTemplateArgumentPackPatternForRewrite(const clang::TemplateArgument&amp;):
Assertion `TA.pack_size() == 1 &amp;&amp; "unexpected pack arguments in template rewrite"' failed.
```

&lt;details&gt;

&lt;summary&gt;Stack dump&lt;/summary&gt;

```console
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 &lt;source&gt;
1.	&lt;source&gt;:12:12: current parser token ';'
 #<!-- -->0 0x0000000003e53898 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3e53898)
 #<!-- -->1 0x0000000003e51554 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3e51554)
 #<!-- -->2 0x0000000003d9de88 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007b90cb442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007b90cb4969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007b90cb442476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007b90cb4287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x00007b90cb42871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x00007b90cb439e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000006f462e3 (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f462e3)
#<!-- -->10 0x0000000006f4fa50 clang::declvisitor::Base&lt;std::add_pointer, clang::TemplateDeclInstantiator, clang::Decl*&gt;::Visit(clang::Decl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f4fa50)
#<!-- -->11 0x0000000006f7f52d clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeLocBuilder&amp;, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
#<!-- -->12 0x0000000006f83baa clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeSourceInfo*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->13 0x0000000006f83ccc clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::QualType) SemaTemplateInstantiate.cpp:0:0
#<!-- -->14 0x0000000006f84840 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformSubstTemplateTypeParmType(clang::TypeLocBuilder&amp;, clang::SubstTemplateTypeParmTypeLoc) SemaTemplateInstantiate.cpp:0:0
#<!-- -->15 0x0000000006f7f42d clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeLocBuilder&amp;, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
#<!-- -->16 0x0000000006f83baa clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeSourceInfo*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->17 0x0000000006fa0bbd (anonymous namespace)::TemplateInstantiator::TransformTemplateArgument(clang::TemplateArgumentLoc const&amp;, clang::TemplateArgumentLoc&amp;, bool) SemaTemplateInstantiate.cpp:0:0
#<!-- -->18 0x0000000006fa294f clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformTemplateSpecializationType(clang::TypeLocBuilder&amp;, clang::TemplateSpecializationTypeLoc, clang::TemplateName) SemaTemplateInstantiate.cpp:0:0
#<!-- -->19 0x0000000006f7ff36 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeLocBuilder&amp;, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
#<!-- -->20 0x0000000006f80781 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformTypeTraitExpr(clang::TypeTraitExpr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->21 0x0000000006f880ff clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->22 0x0000000006fb23d2 clang::Sema::SubstExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6fb23d2)
#<!-- -->23 0x0000000006efde0c (anonymous namespace)::buildAssociatedConstraints(clang::Sema&amp;, clang::FunctionTemplateDecl*, clang::TypeAliasTemplateDecl*, llvm::ArrayRef&lt;clang::DeducedTemplateArgument&gt;, unsigned int, clang::Expr*) SemaTemplateDeductionGuide.cpp:0:0
#<!-- -->24 0x0000000006f018d8 (anonymous namespace)::BuildDeductionGuideForTypeAlias(clang::Sema&amp;, clang::TypeAliasTemplateDecl*, clang::FunctionTemplateDecl*, clang::SourceLocation) SemaTemplateDeductionGuide.cpp:0:0
#<!-- -->25 0x0000000006f43cc5 clang::Sema::DeclareImplicitDeductionGuides(clang::TemplateDecl*, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f43cc5)
#<!-- -->26 0x0000000006b2fd17 LookupDirect(clang::Sema&amp;, clang::LookupResult&amp;, clang::DeclContext const*) SemaLookup.cpp:0:0
#<!-- -->27 0x0000000006b1b012 clang::Sema::LookupQualifiedName(clang::LookupResult&amp;, clang::DeclContext*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b1b012)
#<!-- -->28 0x0000000006af9c2e clang::Sema::DeduceTemplateSpecializationFromInitializer(clang::TypeSourceInfo*, clang::InitializedEntity const&amp;, clang::InitializationKind const&amp;, llvm::MutableArrayRef&lt;clang::Expr*&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6af9c2e)
#<!-- -->29 0x000000000677e43d clang::Sema::deduceVarTypeFromInitializer(clang::VarDecl*, clang::DeclarationName, clang::QualType, clang::TypeSourceInfo*, clang::SourceRange, bool, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x677e43d)
#<!-- -->30 0x0000000006783207 clang::Sema::DeduceVariableDeclarationType(clang::VarDecl*, bool, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6783207)
#<!-- -->31 0x00000000067a19a2 clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67a19a2)
#<!-- -->32 0x0000000006430345 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6430345)
#<!-- -->33 0x000000000643f8c9 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x643f8c9)
#<!-- -->34 0x00000000063ff73e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x63ff73e)
#<!-- -->35 0x00000000063ffef9 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x63ffef9)
#<!-- -->36 0x00000000064076d3 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64076d3)
#<!-- -->37 0x00000000064085ad clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64085ad)
#<!-- -->38 0x00000000063faa3a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x63faa3a)
#<!-- -->39 0x0000000004812598 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4812598)
#<!-- -->40 0x0000000004ada245 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ada245)
#<!-- -->41 0x0000000004a5d92e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4a5d92e)
#<!-- -->42 0x0000000004bc809e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bc809e)
#<!-- -->43 0x0000000000d4ab2f cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd4ab2f)
#<!-- -->44 0x0000000000d4244a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->45 0x000000000485a4c9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->46 0x0000000003d9e334 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3d9e334)
#<!-- -->47 0x000000000485aabf clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->48 0x000000000481d6dd clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x481d6dd)
#<!-- -->49 0x000000000481e75e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x481e75e)
#<!-- -->50 0x00000000048266f5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48266f5)
#<!-- -->51 0x0000000000d47923 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd47923)
#<!-- -->52 0x0000000000c11464 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc11464)
#<!-- -->53 0x00007b90cb429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->54 0x00007b90cb429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->55 0x0000000000d41ef5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd41ef5)
```

&lt;/details&gt;
</details>


---

### Comment 2 - shafik

CC @hokein 

---

