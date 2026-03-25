# [Clang][Regression:21] Crash on constrained variadic constructor during CTAD for alias template

**Author:** MagentaTreehouse
**URL:** https://github.com/llvm/llvm-project/issues/167514

## Body

The following C++20 code causes a crash since Clang 21:
```c++
template <class, class>
concept C = true;

template <class... Ts>
struct Tuple {
    template <C<Ts>... ArgTs>
    Tuple(ArgTs &&...) {}
};

template <class... Ts>
Tuple(Ts...) -> Tuple<Ts...>;

template <class... Ts>
using Result = Tuple<Ts...>;

Result x{0, 1};
```

See https://compiler-explorer.com/z/6db4r7s1G.

Assertion:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/Sema/SemaTemplateInstantiate.cpp:1432:
clang::TemplateArgument {anonymous}::TemplateInstantiator::getTemplateArgumentPackPatternForRewrite(const clang::TemplateArgument&):
Assertion `TA.pack_size() == 1 && TA.pack_begin()->isPackExpansion() && "unexpected pack arguments in template rewrite"' failed.
```

<details>

<summary>Stack dump</summary>

```console
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 <source>
1.	<source>:16:15: current parser token ';'
 #0 0x000000000424f278 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x424f278)
 #1 0x000000000424c6a4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x424c6a4)
 #2 0x0000000004190908 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000073309b442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x000073309b4969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x000073309b442476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x000073309b4287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x000073309b42871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x000073309b439e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000007477848 (anonymous namespace)::TemplateInstantiator::getTemplateArgumentPackPatternForRewrite(clang::TemplateArgument const&) (.isra.0) SemaTemplateInstantiate.cpp:0:0
#10 0x00000000074a9202 (anonymous namespace)::TemplateInstantiator::TransformTemplateTypeParmType(clang::TypeLocBuilder&, clang::TemplateTypeParmTypeLoc, bool) SemaTemplateInstantiate.cpp:0:0
#11 0x0000000007495354 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
#12 0x0000000007499767 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeSourceInfo*) SemaTemplateInstantiate.cpp:0:0
#13 0x00000000074a176c (anonymous namespace)::TemplateInstantiator::TransformTemplateArgument(clang::TemplateArgumentLoc const&, clang::TemplateArgumentLoc&, bool) SemaTemplateInstantiate.cpp:0:0
#14 0x00000000074a3267 bool clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformTemplateArguments<clang::TemplateArgumentLoc const*>(clang::TemplateArgumentLoc const*, clang::TemplateArgumentLoc const*, clang::TemplateArgumentListInfo&, bool) (.constprop.0) SemaTemplateInstantiate.cpp:0:0
#15 0x00000000074a3b68 clang::Sema::SubstTypeConstraint(clang::TemplateTypeParmDecl*, clang::TypeConstraint const*, clang::MultiLevelTemplateArgumentList const&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x74a3b68)
#16 0x000000000742734d (anonymous namespace)::transformTemplateParameter(clang::Sema&, clang::DeclContext*, clang::NamedDecl*, clang::MultiLevelTemplateArgumentList&, unsigned int, unsigned int, bool) (.constprop.0) SemaTemplateDeductionGuide.cpp:0:0
#17 0x000000000742a524 (anonymous namespace)::BuildDeductionGuideForTypeAlias(clang::Sema&, clang::TypeAliasTemplateDecl*, clang::FunctionTemplateDecl*, clang::SourceLocation) SemaTemplateDeductionGuide.cpp:0:0
#18 0x00000000074704c0 clang::Sema::DeclareImplicitDeductionGuides(clang::TemplateDecl*, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x74704c0)
#19 0x000000000706287e LookupDirect(clang::Sema&, clang::LookupResult&, clang::DeclContext const*) SemaLookup.cpp:0:0
#20 0x0000000007062b91 clang::Sema::LookupQualifiedName(clang::LookupResult&, clang::DeclContext*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7062b91)
#21 0x000000000702a2a5 clang::Sema::DeduceTemplateSpecializationFromInitializer(clang::TypeSourceInfo*, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::MutableArrayRef<clang::Expr*>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x702a2a5)
#22 0x0000000006d14e25 clang::Sema::deduceVarTypeFromInitializer(clang::VarDecl*, clang::DeclarationName, clang::QualType, clang::TypeSourceInfo*, clang::SourceRange, bool, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d14e25)
#23 0x0000000006d19fe7 clang::Sema::DeduceVariableDeclarationType(clang::VarDecl*, bool, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d19fe7)
#24 0x0000000006d4ac1a clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d4ac1a)
#25 0x00000000069962c4 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69962c4)
#26 0x00000000069a3c1a clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69a3c1a)
#27 0x00000000069619de clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69619de)
#28 0x000000000696217f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x696217f)
#29 0x0000000006969a33 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6969a33)
#30 0x000000000696a945 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x696a945)
#31 0x000000000694bf3a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x694bf3a)
#32 0x0000000004b9bd38 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b9bd38)
#33 0x0000000004e8a285 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e8a285)
#34 0x0000000004e0b08e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e0b08e)
#35 0x0000000004f8242d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f8242d)
#36 0x0000000000dd3d00 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdd3d00)
#37 0x0000000000dca87a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#38 0x0000000000dca9fd int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#39 0x0000000004c05bb9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#40 0x0000000004190da4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4190da4)
#41 0x0000000004c061cf clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#42 0x0000000004bc6c22 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bc6c22)
#43 0x0000000004bc7bce clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bc7bce)
#44 0x0000000004bcf325 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bcf325)
#45 0x0000000000dd0221 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdd0221)
#46 0x0000000000c7e7b4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc7e7b4)
#47 0x000073309b429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#48 0x000073309b429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#49 0x0000000000dca315 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdca315)
```

</details>

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (MagentaTreehouse)

<details>
The following C++20 code causes a crash since Clang 21:
```c++
template &lt;class, class&gt;
concept C = true;

template &lt;class... Ts&gt;
struct Tuple {
    template &lt;C&lt;Ts&gt;... ArgTs&gt;
    Tuple(ArgTs &amp;&amp;...) {}
};

template &lt;class... Ts&gt;
Tuple(Ts...) -&gt; Tuple&lt;Ts...&gt;;

template &lt;class... Ts&gt;
using Result = Tuple&lt;Ts...&gt;;

Result x{0, 1};
```

See https://compiler-explorer.com/z/6db4r7s1G.

Assertion:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/Sema/SemaTemplateInstantiate.cpp:1432:
clang::TemplateArgument {anonymous}::TemplateInstantiator::getTemplateArgumentPackPatternForRewrite(const clang::TemplateArgument&amp;):
Assertion `TA.pack_size() == 1 &amp;&amp; TA.pack_begin()-&gt;isPackExpansion() &amp;&amp; "unexpected pack arguments in template rewrite"' failed.
```

&lt;details&gt;

&lt;summary&gt;Stack dump&lt;/summary&gt;

```console
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 &lt;source&gt;
1.	&lt;source&gt;:16:15: current parser token ';'
 #<!-- -->0 0x000000000424f278 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x424f278)
 #<!-- -->1 0x000000000424c6a4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x424c6a4)
 #<!-- -->2 0x0000000004190908 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x000073309b442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x000073309b4969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x000073309b442476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x000073309b4287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x000073309b42871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x000073309b439e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000007477848 (anonymous namespace)::TemplateInstantiator::getTemplateArgumentPackPatternForRewrite(clang::TemplateArgument const&amp;) (.isra.0) SemaTemplateInstantiate.cpp:0:0
#<!-- -->10 0x00000000074a9202 (anonymous namespace)::TemplateInstantiator::TransformTemplateTypeParmType(clang::TypeLocBuilder&amp;, clang::TemplateTypeParmTypeLoc, bool) SemaTemplateInstantiate.cpp:0:0
#<!-- -->11 0x0000000007495354 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeLocBuilder&amp;, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
#<!-- -->12 0x0000000007499767 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeSourceInfo*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->13 0x00000000074a176c (anonymous namespace)::TemplateInstantiator::TransformTemplateArgument(clang::TemplateArgumentLoc const&amp;, clang::TemplateArgumentLoc&amp;, bool) SemaTemplateInstantiate.cpp:0:0
#<!-- -->14 0x00000000074a3267 bool clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformTemplateArguments&lt;clang::TemplateArgumentLoc const*&gt;(clang::TemplateArgumentLoc const*, clang::TemplateArgumentLoc const*, clang::TemplateArgumentListInfo&amp;, bool) (.constprop.0) SemaTemplateInstantiate.cpp:0:0
#<!-- -->15 0x00000000074a3b68 clang::Sema::SubstTypeConstraint(clang::TemplateTypeParmDecl*, clang::TypeConstraint const*, clang::MultiLevelTemplateArgumentList const&amp;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x74a3b68)
#<!-- -->16 0x000000000742734d (anonymous namespace)::transformTemplateParameter(clang::Sema&amp;, clang::DeclContext*, clang::NamedDecl*, clang::MultiLevelTemplateArgumentList&amp;, unsigned int, unsigned int, bool) (.constprop.0) SemaTemplateDeductionGuide.cpp:0:0
#<!-- -->17 0x000000000742a524 (anonymous namespace)::BuildDeductionGuideForTypeAlias(clang::Sema&amp;, clang::TypeAliasTemplateDecl*, clang::FunctionTemplateDecl*, clang::SourceLocation) SemaTemplateDeductionGuide.cpp:0:0
#<!-- -->18 0x00000000074704c0 clang::Sema::DeclareImplicitDeductionGuides(clang::TemplateDecl*, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x74704c0)
#<!-- -->19 0x000000000706287e LookupDirect(clang::Sema&amp;, clang::LookupResult&amp;, clang::DeclContext const*) SemaLookup.cpp:0:0
#<!-- -->20 0x0000000007062b91 clang::Sema::LookupQualifiedName(clang::LookupResult&amp;, clang::DeclContext*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7062b91)
#<!-- -->21 0x000000000702a2a5 clang::Sema::DeduceTemplateSpecializationFromInitializer(clang::TypeSourceInfo*, clang::InitializedEntity const&amp;, clang::InitializationKind const&amp;, llvm::MutableArrayRef&lt;clang::Expr*&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x702a2a5)
#<!-- -->22 0x0000000006d14e25 clang::Sema::deduceVarTypeFromInitializer(clang::VarDecl*, clang::DeclarationName, clang::QualType, clang::TypeSourceInfo*, clang::SourceRange, bool, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d14e25)
#<!-- -->23 0x0000000006d19fe7 clang::Sema::DeduceVariableDeclarationType(clang::VarDecl*, bool, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d19fe7)
#<!-- -->24 0x0000000006d4ac1a clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d4ac1a)
#<!-- -->25 0x00000000069962c4 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69962c4)
#<!-- -->26 0x00000000069a3c1a clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69a3c1a)
#<!-- -->27 0x00000000069619de clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69619de)
#<!-- -->28 0x000000000696217f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x696217f)
#<!-- -->29 0x0000000006969a33 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6969a33)
#<!-- -->30 0x000000000696a945 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x696a945)
#<!-- -->31 0x000000000694bf3a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x694bf3a)
#<!-- -->32 0x0000000004b9bd38 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b9bd38)
#<!-- -->33 0x0000000004e8a285 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e8a285)
#<!-- -->34 0x0000000004e0b08e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e0b08e)
#<!-- -->35 0x0000000004f8242d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f8242d)
#<!-- -->36 0x0000000000dd3d00 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdd3d00)
#<!-- -->37 0x0000000000dca87a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->38 0x0000000000dca9fd int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->39 0x0000000004c05bb9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->40 0x0000000004190da4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4190da4)
#<!-- -->41 0x0000000004c061cf clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->42 0x0000000004bc6c22 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bc6c22)
#<!-- -->43 0x0000000004bc7bce clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bc7bce)
#<!-- -->44 0x0000000004bcf325 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bcf325)
#<!-- -->45 0x0000000000dd0221 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdd0221)
#<!-- -->46 0x0000000000c7e7b4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc7e7b4)
#<!-- -->47 0x000073309b429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->48 0x000073309b429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->49 0x0000000000dca315 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdca315)
```

&lt;/details&gt;
</details>


---

### Comment 2 - MagentaTreehouse

Bisects to [fd4f94d](https://github.com/llvm/llvm-project/commit/fd4f94ddbf0c0f0c9d0185e6036fe51de5ab2ef3) @zyn0217 .

---

