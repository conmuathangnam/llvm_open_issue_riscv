# Clang ICEs in SemaTemplateDeduction.cpp since version 4.0

**Author:** mariete1223
**URL:** https://github.com/llvm/llvm-project/issues/146506

## Body


## Assertion

```
clang++: /root/llvm-project/llvm/tools/clang/lib/Sema/SemaTemplateDeduction.cpp:868: unsigned int {anonymous}::PackDeductionScope::addPacks(clang::TemplateArgument): Assertion `!Packs.empty() && "Pack expansion without unexpanded packs?"' failed.
```

## Stack dump

```
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x c++ <source>
1.	<source>:10:5: current parser token '{'
2.	<source>:4:1: parsing struct/union/class body 'remove_separators<>'
 #0 0x0000000003f78848 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f78848)
 #1 0x0000000003f764d4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f764d4)
 #2 0x0000000003ebb048 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x0000710809a42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x0000710809a969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x0000710809a42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x0000710809a287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x0000710809a2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x0000710809a39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x000000000709d351 (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x709d351)
#10 0x000000000709ddb9 DeduceTemplateSpecArguments(clang::Sema&, clang::TemplateParameterList*, clang::QualType, clang::QualType, clang::sema::TemplateDeductionInfo&, bool, llvm::SmallVectorImpl<clang::DeducedTemplateArgument>&, bool*) SemaTemplateDeduction.cpp:0:0
#11 0x0000000007097ddb DeduceTemplateArgumentsByTypeMatch(clang::Sema&, clang::TemplateParameterList*, clang::QualType, clang::QualType, clang::sema::TemplateDeductionInfo&, llvm::SmallVectorImpl<clang::DeducedTemplateArgument>&, unsigned int, PartialOrderingKind, bool, bool*) SemaTemplateDeduction.cpp:0:0
#12 0x00000000070a0d05 clang::ClassTemplatePartialSpecializationDecl* getMoreSpecialized<clang::ClassTemplatePartialSpecializationDecl, clang::ClassTemplateDecl>(clang::Sema&, clang::QualType, clang::QualType, clang::ClassTemplatePartialSpecializationDecl*, clang::ClassTemplateDecl*, clang::sema::TemplateDeductionInfo&) SemaTemplateDeduction.cpp:0:0
#13 0x00000000070a1551 clang::Sema::isMoreSpecializedThanPrimary(clang::ClassTemplatePartialSpecializationDecl*, clang::sema::TemplateDeductionInfo&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x70a1551)
#14 0x0000000006fe265d void checkTemplatePartialSpecialization<clang::ClassTemplatePartialSpecializationDecl>(clang::Sema&, clang::ClassTemplatePartialSpecializationDecl*) SemaTemplate.cpp:0:0
#15 0x00000000070129d1 clang::Sema::ActOnClassTemplateSpecialization(clang::Scope*, unsigned int, clang::TagUseKind, clang::SourceLocation, clang::SourceLocation, clang::CXXScopeSpec&, clang::TemplateIdAnnotation&, clang::ParsedAttributesView const&, llvm::MutableArrayRef<clang::TemplateParameterList*>, clang::SkipBodyInfo*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x70129d1)
#16 0x0000000006696aab clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6696aab)
#17 0x000000000666b4e0 clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x666b4e0)
#18 0x0000000006690ba1 clang::Parser::ParseCXXClassMemberDeclaration(clang::AccessSpecifier, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::ParsingDeclRAIIObject*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6690ba1)
#19 0x00000000067470a5 clang::Parser::ParseDeclarationAfterTemplate(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo&, clang::ParsingDeclRAIIObject&, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67470a5)
#20 0x000000000675492f clang::Parser::ParseTemplateDeclarationOrSpecialization(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (.part.0) ParseTemplate.cpp:0:0
#21 0x0000000006690fed clang::Parser::ParseCXXClassMemberDeclaration(clang::AccessSpecifier, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::ParsingDeclRAIIObject*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6690fed)
#22 0x0000000006693161 clang::Parser::ParseCXXClassMemberDeclarationWithPragmas(clang::AccessSpecifier&, clang::ParsedAttributes&, clang::TypeSpecifierType, clang::Decl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6693161)
#23 0x0000000006694fcf clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&, unsigned int, clang::Decl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6694fcf)
#24 0x0000000006697f10 clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6697f10)
#25 0x000000000666b4e0 clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x666b4e0)
#26 0x0000000006746cbc clang::Parser::ParseDeclarationAfterTemplate(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo&, clang::ParsingDeclRAIIObject&, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6746cbc)
#27 0x000000000675492f clang::Parser::ParseTemplateDeclarationOrSpecialization(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (.part.0) ParseTemplate.cpp:0:0
#28 0x0000000006754cda clang::Parser::ParseDeclarationStartingWithTemplate(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6754cda)
#29 0x00000000066721b0 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66721b0)
#30 0x000000000662f85c clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x662f85c)
#31 0x000000000663077d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x663077d)
#32 0x0000000006622aea clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6622aea)
#33 0x000000000492ed48 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x492ed48)
#34 0x0000000004c28c65 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c28c65)
#35 0x0000000004ba5ffe clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ba5ffe)
#36 0x0000000004d1b441 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d1b441)
#37 0x0000000000d8e81f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd8e81f)
#38 0x0000000000d853fa ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#39 0x0000000004995b49 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#40 0x0000000003ebb4e4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3ebb4e4)
#41 0x000000000499615f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#42 0x0000000004957f8d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4957f8d)
#43 0x000000000495901e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x495901e)
#44 0x00000000049614d5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49614d5)
#45 0x0000000000d8accf clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd8accf)
#46 0x0000000000c41924 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc41924)
#47 0x0000710809a29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#48 0x0000710809a29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#49 0x0000000000d84ea5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd84ea5)
```

## Program

```
template <class...>
struct remove_separators;
template <int T>
struct remove_separators<>
{
    template <class...>
    struct helper;
    template <class Sep>
    struct helper<Sep>
    {
        static constexpr bool value = false;
    };
}
```


## To quickly reproduce:

https://godbolt.org/z/55G4Y59cv

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (mariete1223)

<details>

## Assertion

```
clang++: /root/llvm-project/llvm/tools/clang/lib/Sema/SemaTemplateDeduction.cpp:868: unsigned int {anonymous}::PackDeductionScope::addPacks(clang::TemplateArgument): Assertion `!Packs.empty() &amp;&amp; "Pack expansion without unexpanded packs?"' failed.
```

## Stack dump

```
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x c++ &lt;source&gt;
1.	&lt;source&gt;:10:5: current parser token '{'
2.	&lt;source&gt;:4:1: parsing struct/union/class body 'remove_separators&lt;&gt;'
 #<!-- -->0 0x0000000003f78848 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f78848)
 #<!-- -->1 0x0000000003f764d4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f764d4)
 #<!-- -->2 0x0000000003ebb048 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x0000710809a42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x0000710809a969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x0000710809a42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x0000710809a287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x0000710809a2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x0000710809a39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x000000000709d351 (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x709d351)
#<!-- -->10 0x000000000709ddb9 DeduceTemplateSpecArguments(clang::Sema&amp;, clang::TemplateParameterList*, clang::QualType, clang::QualType, clang::sema::TemplateDeductionInfo&amp;, bool, llvm::SmallVectorImpl&lt;clang::DeducedTemplateArgument&gt;&amp;, bool*) SemaTemplateDeduction.cpp:0:0
#<!-- -->11 0x0000000007097ddb DeduceTemplateArgumentsByTypeMatch(clang::Sema&amp;, clang::TemplateParameterList*, clang::QualType, clang::QualType, clang::sema::TemplateDeductionInfo&amp;, llvm::SmallVectorImpl&lt;clang::DeducedTemplateArgument&gt;&amp;, unsigned int, PartialOrderingKind, bool, bool*) SemaTemplateDeduction.cpp:0:0
#<!-- -->12 0x00000000070a0d05 clang::ClassTemplatePartialSpecializationDecl* getMoreSpecialized&lt;clang::ClassTemplatePartialSpecializationDecl, clang::ClassTemplateDecl&gt;(clang::Sema&amp;, clang::QualType, clang::QualType, clang::ClassTemplatePartialSpecializationDecl*, clang::ClassTemplateDecl*, clang::sema::TemplateDeductionInfo&amp;) SemaTemplateDeduction.cpp:0:0
#<!-- -->13 0x00000000070a1551 clang::Sema::isMoreSpecializedThanPrimary(clang::ClassTemplatePartialSpecializationDecl*, clang::sema::TemplateDeductionInfo&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x70a1551)
#<!-- -->14 0x0000000006fe265d void checkTemplatePartialSpecialization&lt;clang::ClassTemplatePartialSpecializationDecl&gt;(clang::Sema&amp;, clang::ClassTemplatePartialSpecializationDecl*) SemaTemplate.cpp:0:0
#<!-- -->15 0x00000000070129d1 clang::Sema::ActOnClassTemplateSpecialization(clang::Scope*, unsigned int, clang::TagUseKind, clang::SourceLocation, clang::SourceLocation, clang::CXXScopeSpec&amp;, clang::TemplateIdAnnotation&amp;, clang::ParsedAttributesView const&amp;, llvm::MutableArrayRef&lt;clang::TemplateParameterList*&gt;, clang::SkipBodyInfo*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x70129d1)
#<!-- -->16 0x0000000006696aab clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6696aab)
#<!-- -->17 0x000000000666b4e0 clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x666b4e0)
#<!-- -->18 0x0000000006690ba1 clang::Parser::ParseCXXClassMemberDeclaration(clang::AccessSpecifier, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::ParsingDeclRAIIObject*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6690ba1)
#<!-- -->19 0x00000000067470a5 clang::Parser::ParseDeclarationAfterTemplate(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo&amp;, clang::ParsingDeclRAIIObject&amp;, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67470a5)
#<!-- -->20 0x000000000675492f clang::Parser::ParseTemplateDeclarationOrSpecialization(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::AccessSpecifier) (.part.0) ParseTemplate.cpp:0:0
#<!-- -->21 0x0000000006690fed clang::Parser::ParseCXXClassMemberDeclaration(clang::AccessSpecifier, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::ParsingDeclRAIIObject*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6690fed)
#<!-- -->22 0x0000000006693161 clang::Parser::ParseCXXClassMemberDeclarationWithPragmas(clang::AccessSpecifier&amp;, clang::ParsedAttributes&amp;, clang::TypeSpecifierType, clang::Decl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6693161)
#<!-- -->23 0x0000000006694fcf clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&amp;, unsigned int, clang::Decl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6694fcf)
#<!-- -->24 0x0000000006697f10 clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6697f10)
#<!-- -->25 0x000000000666b4e0 clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x666b4e0)
#<!-- -->26 0x0000000006746cbc clang::Parser::ParseDeclarationAfterTemplate(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo&amp;, clang::ParsingDeclRAIIObject&amp;, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6746cbc)
#<!-- -->27 0x000000000675492f clang::Parser::ParseTemplateDeclarationOrSpecialization(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::AccessSpecifier) (.part.0) ParseTemplate.cpp:0:0
#<!-- -->28 0x0000000006754cda clang::Parser::ParseDeclarationStartingWithTemplate(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6754cda)
#<!-- -->29 0x00000000066721b0 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66721b0)
#<!-- -->30 0x000000000662f85c clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x662f85c)
#<!-- -->31 0x000000000663077d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x663077d)
#<!-- -->32 0x0000000006622aea clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6622aea)
#<!-- -->33 0x000000000492ed48 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x492ed48)
#<!-- -->34 0x0000000004c28c65 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c28c65)
#<!-- -->35 0x0000000004ba5ffe clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ba5ffe)
#<!-- -->36 0x0000000004d1b441 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d1b441)
#<!-- -->37 0x0000000000d8e81f cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd8e81f)
#<!-- -->38 0x0000000000d853fa ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->39 0x0000000004995b49 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->40 0x0000000003ebb4e4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3ebb4e4)
#<!-- -->41 0x000000000499615f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->42 0x0000000004957f8d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4957f8d)
#<!-- -->43 0x000000000495901e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x495901e)
#<!-- -->44 0x00000000049614d5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49614d5)
#<!-- -->45 0x0000000000d8accf clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd8accf)
#<!-- -->46 0x0000000000c41924 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc41924)
#<!-- -->47 0x0000710809a29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->48 0x0000710809a29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->49 0x0000000000d84ea5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd84ea5)
```

## Program

```
template &lt;class...&gt;
struct remove_separators;
template &lt;int T&gt;
struct remove_separators&lt;&gt;
{
    template &lt;class...&gt;
    struct helper;
    template &lt;class Sep&gt;
    struct helper&lt;Sep&gt;
    {
        static constexpr bool value = false;
    };
}
```


## To quickly reproduce:

https://godbolt.org/z/55G4Y59cv
</details>


---

