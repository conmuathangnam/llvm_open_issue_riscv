# [clang/TemplateInstantiator] crash (segfault) in TransformTemplateName

**Author:** kedartal
**URL:** https://github.com/llvm/llvm-project/issues/155281
**Status:** Closed
**Labels:** clang:frontend, crash
**Closed Date:** 2025-08-26T03:08:42Z

## Body

segfault compiling (`-fsyntax-only`) [this source](https://github.com/stan-dev/math/blob/develop/stan/math/prim/fun/vector_seq_view.hpp)
(similar to https://github.com/llvm/llvm-project/issues/155260)

Again no backtrace file created, but can be reproduced from [vector_seq_view-35fab3.zip](https://github.com/user-attachments/files/21975657/vector_seq_view-35fab3.zip)

```
/Users/tal/local/llvm/latest/bin/clang++ -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX15.5.sdk -I./external/stan.git/lib/stan_math -I./external/stan.git/lib/stan_math/lib/eigen_3.4.0 -I ./external/stan.git/lib/stan_math/lib/boost_1.84.0  -fsyntax-only ./external/stan.git/lib/stan_math/stan/math/prim/fun/vector_seq_view.hpp
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /Users/tal/local/llvm/latest/bin/clang++ -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX15.5.sdk -I./external/stan.git/lib/stan_math -I./external/stan.git/lib/stan_math/lib/eigen_3.4.0 -I ./external/stan.git/lib/stan_math/lib/boost_1.84.0 -fsyntax-only ./external/stan.git/lib/stan_math/stan/math/prim/fun/vector_seq_view.hpp
1.	./external/stan.git/lib/stan_math/stan/math/prim/fun/vector_seq_view.hpp:75:69: current parser token '{'
2.	./external/stan.git/lib/stan_math/stan/math/prim/fun/vector_seq_view.hpp:8:1: parsing namespace 'stan'
 #0 0x0000000106a0be0c llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x101aa7e0c)
 #1 0x0000000106a093bc llvm::sys::RunSignalHandlers() (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x101aa53bc)
 #2 0x0000000106a0b304 llvm::sys::CleanupOnSignal(unsigned long) (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x101aa7304)
 #3 0x000000010694e12c CrashRecoverySignalHandler(int) (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x1019ea12c)
 #4 0x000000018ffc56a4 (/usr/lib/system/libsystem_platform.dylib+0x1804ad6a4)
 #5 0x0000000108b8f128 (anonymous namespace)::TemplateInstantiator::TransformTemplateName(clang::NestedNameSpecifierLoc&, clang::SourceLocation, clang::TemplateName, clang::SourceLocation, clang::QualType, clang::NamedDecl*, bool) (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x103c2b128)
 #6 0x0000000108b8f128 (anonymous namespace)::TemplateInstantiator::TransformTemplateName(clang::NestedNameSpecifierLoc&, clang::SourceLocation, clang::TemplateName, clang::SourceLocation, clang::QualType, clang::NamedDecl*, bool) (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x103c2b128)
 #7 0x0000000108b8ef88 (anonymous namespace)::TemplateInstantiator::TransformTemplateName(clang::NestedNameSpecifierLoc&, clang::SourceLocation, clang::TemplateName, clang::SourceLocation, clang::QualType, clang::NamedDecl*, bool) (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x103c2af88)
 #8 0x0000000108b8bec0 (anonymous namespace)::TemplateInstantiator::TransformTemplateArgument(clang::TemplateArgumentLoc const&, clang::TemplateArgumentLoc&, bool) (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x103c27ec0)
 #9 0x0000000108baf53c (anonymous namespace)::TemplateInstantiator::TransformTemplateSpecializationType(clang::TypeLocBuilder&, clang::TemplateSpecializationTypeLoc) (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x103c4b53c)
#10 0x0000000108b83230 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeSourceInfo*) (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x103c1f230)
#11 0x0000000108baf0c8 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformSubstTemplateTypeParmType(clang::TypeLocBuilder&, clang::SubstTemplateTypeParmTypeLoc) (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x103c4b0c8)
#12 0x0000000108b8e094 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformNestedNameSpecifierLoc(clang::NestedNameSpecifierLoc, clang::QualType, clang::NamedDecl*) (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x103c2a094)
#13 0x0000000108b9dd7c clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformDependentScopeDeclRefExpr(clang::DependentScopeDeclRefExpr*, bool, clang::TypeSourceInfo**) (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x103c39d7c)
#14 0x0000000108b91bfc (anonymous namespace)::TemplateInstantiator::TransformSubstNonTypeTemplateParmExpr(clang::SubstNonTypeTemplateParmExpr*) (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x103c2dbfc)
#15 0x0000000108b8bb64 (anonymous namespace)::TemplateInstantiator::TransformTemplateArgument(clang::TemplateArgumentLoc const&, clang::TemplateArgumentLoc&, bool) (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x103c27b64)
#16 0x0000000108baf53c (anonymous namespace)::TemplateInstantiator::TransformTemplateSpecializationType(clang::TypeLocBuilder&, clang::TemplateSpecializationTypeLoc) (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x103c4b53c)
#17 0x0000000108b8e094 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformNestedNameSpecifierLoc(clang::NestedNameSpecifierLoc, clang::QualType, clang::NamedDecl*) (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x103c2a094)
#18 0x0000000108ba8c44 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformDependentNameType(clang::TypeLocBuilder&, clang::DependentNameTypeLoc, bool, clang::QualType, clang::NamedDecl*) (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x103c44c44)
#19 0x0000000108b83230 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeSourceInfo*) (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x103c1f230)
#20 0x0000000108b8baec (anonymous namespace)::TemplateInstantiator::TransformTemplateArgument(clang::TemplateArgumentLoc const&, clang::TemplateArgumentLoc&, bool) (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x103c27aec)
#21 0x0000000108b8c100 bool clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformTemplateArguments<clang::TemplateArgumentLoc const*>(clang::TemplateArgumentLoc const*, clang::TemplateArgumentLoc const*, clang::TemplateArgumentListInfo&, bool) (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x103c28100)
#22 0x0000000108b43540 FinishTemplateArgumentDeduction(clang::Sema&, clang::NamedDecl*, clang::TemplateParameterList*, clang::TemplateDecl*, bool, llvm::ArrayRef<clang::TemplateArgumentLoc>, llvm::ArrayRef<clang::TemplateArgument>, llvm::SmallVectorImpl<clang::DeducedTemplateArgument>&, clang::sema::TemplateDeductionInfo&, bool) (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x103bdf540)
#23 0x0000000108b45cd4 FinishTemplateArgumentDeduction(clang::Sema&, clang::NamedDecl*, clang::TemplateParameterList*, clang::TemplateDecl*, bool, llvm::ArrayRef<clang::TemplateArgument>, llvm::ArrayRef<clang::TemplateArgument>, llvm::SmallVectorImpl<clang::DeducedTemplateArgument>&, clang::sema::TemplateDeductionInfo&, bool) (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x103be1cd4)
#24 0x0000000108b457fc bool isAtLeastAsSpecializedAs<clang::ClassTemplatePartialSpecializationDecl>(clang::Sema&, clang::QualType, clang::QualType, clang::ClassTemplatePartialSpecializationDecl*, clang::TemplateDecl*, clang::sema::TemplateDeductionInfo&) (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x103be17fc)
#25 0x0000000108b0d240 clang::Sema::isMoreSpecializedThanPrimary(clang::ClassTemplatePartialSpecializationDecl*, clang::sema::TemplateDeductionInfo&) (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x103ba9240)
#26 0x0000000108a80b54 clang::Sema::CheckTemplatePartialSpecialization(clang::ClassTemplatePartialSpecializationDecl*) (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x103b1cb54)
#27 0x0000000108a90708 clang::Sema::ActOnClassTemplateSpecialization(clang::Scope*, unsigned int, clang::TagUseKind, clang::SourceLocation, clang::SourceLocation, clang::CXXScopeSpec&, clang::TemplateIdAnnotation&, clang::ParsedAttributesView const&, llvm::MutableArrayRef<clang::TemplateParameterList*>, clang::SkipBodyInfo*) (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x103b2c708)
#28 0x000000010833aa60 clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&) (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x1033d6a60)
#29 0x0000000108315b24 clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x1033b1b24)
#30 0x00000001083d9bec clang::Parser::ParseDeclarationAfterTemplate(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo&, clang::ParsingDeclRAIIObject&, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x103475bec)
#31 0x00000001083d8c90 clang::Parser::ParseTemplateDeclarationOrSpecialization(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x103474c90)
#32 0x00000001083d84ec clang::Parser::ParseDeclarationStartingWithTemplate(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&) (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x1034744ec)
#33 0x000000010830f4ac clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x1033ab4ac)
#34 0x00000001083ea540 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x103486540)
#35 0x0000000108331b54 clang::Parser::ParseInnerNamespace(llvm::SmallVector<clang::Parser::InnerNamespaceInfo, 4u> const&, unsigned int, clang::SourceLocation&, clang::ParsedAttributes&, clang::BalancedDelimiterTracker&) (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x1033cdb54)
#36 0x0000000108331200 clang::Parser::ParseNamespace(clang::DeclaratorContext, clang::SourceLocation&, clang::SourceLocation) (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x1033cd200)
#37 0x000000010830f6fc clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x1033ab6fc)
#38 0x00000001083ea540 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x103486540)
#39 0x00000001083e8f40 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x103484f40)
#40 0x00000001082f62a0 clang::ParseAST(clang::Sema&, bool, bool) (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x1033922a0)
#41 0x000000010767c238 clang::FrontendAction::Execute() (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x102718238)
#42 0x00000001075f546c clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x10269146c)
#43 0x00000001076ff1cc clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x10279b1cc)
#44 0x0000000104f6a458 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x100006458)
#45 0x0000000104f67f00 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x100003f00)
#46 0x000000010742a068 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::__1::optional<llvm::StringRef>>, std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>>*, bool*) const::$_0>(long) (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x1024c6068)
#47 0x000000010694de2c llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x1019e9e2c)
#48 0x0000000107429a9c clang::driver::CC1Command::Execute(llvm::ArrayRef<std::__1::optional<llvm::StringRef>>, std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>>*, bool*) const (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x1024c5a9c)
#49 0x00000001073eba18 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x102487a18)
#50 0x000000010740823c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::__1::pair<int, clang::driver::Command const*>>&) (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x1024a423c)
#51 0x0000000104f66b8c clang_main(int, char**, llvm::ToolContext const&) (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x100002b8c)
#52 0x0000000104f75398 main (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x100011398)
#53 0x000000018fbeab98
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
clang version 22.0.0git (https://github.com/llvm/llvm-project.git 9801a0f62e49cbd81ce8352ab140af7c240e51ba)
Target: arm64-apple-darwin24.6.0
Thread model: posix
InstalledDir: /Users/tal/local/llvm/20250825.140242/bin
clang++: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: /var/folders/20/qtz798xj7pv9k3tfl9_w20mm0000gn/T/vector_seq_view-35fab3.hh
clang++: note: diagnostic msg: /var/folders/20/qtz798xj7pv9k3tfl9_w20mm0000gn/T/vector_seq_view-35fab3.sh
clang++: note: diagnostic msg: Crash backtrace is located in
clang++: note: diagnostic msg: /Users/tal/Library/Logs/DiagnosticReports/clang-22_<YYYY-MM-DD-HHMMSS>_<hostname>.crash
clang++: note: diagnostic msg: (choose the .crash file that corresponds to your crash)
clang++: note: diagnostic msg: 

********************
```

@mizvekov probably also related to https://github.com/llvm/llvm-project/pull/147835 ?

## Comments

### Comment 1 - kedartal

@mizvekov this happens after the fix of https://github.com/llvm/llvm-project/pull/155272 
```
/Users/tal/local/llvm/latest/bin/clang++ --version
clang version 22.0.0git (https://github.com/llvm/llvm-project.git 9801a0f62e49cbd81ce8352ab140af7c240e51ba)
Target: arm64-apple-darwin24.6.0
Thread model: posix
```

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Tal Kedar (kedartal)

<details>
segfault compiling (`-fsyntax-only`) [this source](https://github.com/stan-dev/math/blob/develop/stan/math/prim/fun/vector_seq_view.hpp)
(similar to https://github.com/llvm/llvm-project/issues/155260)

Again no backtrace file created, but can be reproduced from [vector_seq_view-35fab3.zip](https://github.com/user-attachments/files/21975657/vector_seq_view-35fab3.zip)

```
/Users/tal/local/llvm/latest/bin/clang++ -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX15.5.sdk -I./external/stan.git/lib/stan_math -I./external/stan.git/lib/stan_math/lib/eigen_3.4.0 -I ./external/stan.git/lib/stan_math/lib/boost_1.84.0  -fsyntax-only ./external/stan.git/lib/stan_math/stan/math/prim/fun/vector_seq_view.hpp
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /Users/tal/local/llvm/latest/bin/clang++ -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX15.5.sdk -I./external/stan.git/lib/stan_math -I./external/stan.git/lib/stan_math/lib/eigen_3.4.0 -I ./external/stan.git/lib/stan_math/lib/boost_1.84.0 -fsyntax-only ./external/stan.git/lib/stan_math/stan/math/prim/fun/vector_seq_view.hpp
1.	./external/stan.git/lib/stan_math/stan/math/prim/fun/vector_seq_view.hpp:75:69: current parser token '{'
2.	./external/stan.git/lib/stan_math/stan/math/prim/fun/vector_seq_view.hpp:8:1: parsing namespace 'stan'
 #<!-- -->0 0x0000000106a0be0c llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x101aa7e0c)
 #<!-- -->1 0x0000000106a093bc llvm::sys::RunSignalHandlers() (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x101aa53bc)
 #<!-- -->2 0x0000000106a0b304 llvm::sys::CleanupOnSignal(unsigned long) (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x101aa7304)
 #<!-- -->3 0x000000010694e12c CrashRecoverySignalHandler(int) (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x1019ea12c)
 #<!-- -->4 0x000000018ffc56a4 (/usr/lib/system/libsystem_platform.dylib+0x1804ad6a4)
 #<!-- -->5 0x0000000108b8f128 (anonymous namespace)::TemplateInstantiator::TransformTemplateName(clang::NestedNameSpecifierLoc&amp;, clang::SourceLocation, clang::TemplateName, clang::SourceLocation, clang::QualType, clang::NamedDecl*, bool) (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x103c2b128)
 #<!-- -->6 0x0000000108b8f128 (anonymous namespace)::TemplateInstantiator::TransformTemplateName(clang::NestedNameSpecifierLoc&amp;, clang::SourceLocation, clang::TemplateName, clang::SourceLocation, clang::QualType, clang::NamedDecl*, bool) (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x103c2b128)
 #<!-- -->7 0x0000000108b8ef88 (anonymous namespace)::TemplateInstantiator::TransformTemplateName(clang::NestedNameSpecifierLoc&amp;, clang::SourceLocation, clang::TemplateName, clang::SourceLocation, clang::QualType, clang::NamedDecl*, bool) (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x103c2af88)
 #<!-- -->8 0x0000000108b8bec0 (anonymous namespace)::TemplateInstantiator::TransformTemplateArgument(clang::TemplateArgumentLoc const&amp;, clang::TemplateArgumentLoc&amp;, bool) (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x103c27ec0)
 #<!-- -->9 0x0000000108baf53c (anonymous namespace)::TemplateInstantiator::TransformTemplateSpecializationType(clang::TypeLocBuilder&amp;, clang::TemplateSpecializationTypeLoc) (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x103c4b53c)
#<!-- -->10 0x0000000108b83230 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeSourceInfo*) (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x103c1f230)
#<!-- -->11 0x0000000108baf0c8 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformSubstTemplateTypeParmType(clang::TypeLocBuilder&amp;, clang::SubstTemplateTypeParmTypeLoc) (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x103c4b0c8)
#<!-- -->12 0x0000000108b8e094 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformNestedNameSpecifierLoc(clang::NestedNameSpecifierLoc, clang::QualType, clang::NamedDecl*) (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x103c2a094)
#<!-- -->13 0x0000000108b9dd7c clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformDependentScopeDeclRefExpr(clang::DependentScopeDeclRefExpr*, bool, clang::TypeSourceInfo**) (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x103c39d7c)
#<!-- -->14 0x0000000108b91bfc (anonymous namespace)::TemplateInstantiator::TransformSubstNonTypeTemplateParmExpr(clang::SubstNonTypeTemplateParmExpr*) (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x103c2dbfc)
#<!-- -->15 0x0000000108b8bb64 (anonymous namespace)::TemplateInstantiator::TransformTemplateArgument(clang::TemplateArgumentLoc const&amp;, clang::TemplateArgumentLoc&amp;, bool) (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x103c27b64)
#<!-- -->16 0x0000000108baf53c (anonymous namespace)::TemplateInstantiator::TransformTemplateSpecializationType(clang::TypeLocBuilder&amp;, clang::TemplateSpecializationTypeLoc) (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x103c4b53c)
#<!-- -->17 0x0000000108b8e094 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformNestedNameSpecifierLoc(clang::NestedNameSpecifierLoc, clang::QualType, clang::NamedDecl*) (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x103c2a094)
#<!-- -->18 0x0000000108ba8c44 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformDependentNameType(clang::TypeLocBuilder&amp;, clang::DependentNameTypeLoc, bool, clang::QualType, clang::NamedDecl*) (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x103c44c44)
#<!-- -->19 0x0000000108b83230 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeSourceInfo*) (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x103c1f230)
#<!-- -->20 0x0000000108b8baec (anonymous namespace)::TemplateInstantiator::TransformTemplateArgument(clang::TemplateArgumentLoc const&amp;, clang::TemplateArgumentLoc&amp;, bool) (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x103c27aec)
#<!-- -->21 0x0000000108b8c100 bool clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformTemplateArguments&lt;clang::TemplateArgumentLoc const*&gt;(clang::TemplateArgumentLoc const*, clang::TemplateArgumentLoc const*, clang::TemplateArgumentListInfo&amp;, bool) (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x103c28100)
#<!-- -->22 0x0000000108b43540 FinishTemplateArgumentDeduction(clang::Sema&amp;, clang::NamedDecl*, clang::TemplateParameterList*, clang::TemplateDecl*, bool, llvm::ArrayRef&lt;clang::TemplateArgumentLoc&gt;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, llvm::SmallVectorImpl&lt;clang::DeducedTemplateArgument&gt;&amp;, clang::sema::TemplateDeductionInfo&amp;, bool) (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x103bdf540)
#<!-- -->23 0x0000000108b45cd4 FinishTemplateArgumentDeduction(clang::Sema&amp;, clang::NamedDecl*, clang::TemplateParameterList*, clang::TemplateDecl*, bool, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, llvm::SmallVectorImpl&lt;clang::DeducedTemplateArgument&gt;&amp;, clang::sema::TemplateDeductionInfo&amp;, bool) (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x103be1cd4)
#<!-- -->24 0x0000000108b457fc bool isAtLeastAsSpecializedAs&lt;clang::ClassTemplatePartialSpecializationDecl&gt;(clang::Sema&amp;, clang::QualType, clang::QualType, clang::ClassTemplatePartialSpecializationDecl*, clang::TemplateDecl*, clang::sema::TemplateDeductionInfo&amp;) (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x103be17fc)
#<!-- -->25 0x0000000108b0d240 clang::Sema::isMoreSpecializedThanPrimary(clang::ClassTemplatePartialSpecializationDecl*, clang::sema::TemplateDeductionInfo&amp;) (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x103ba9240)
#<!-- -->26 0x0000000108a80b54 clang::Sema::CheckTemplatePartialSpecialization(clang::ClassTemplatePartialSpecializationDecl*) (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x103b1cb54)
#<!-- -->27 0x0000000108a90708 clang::Sema::ActOnClassTemplateSpecialization(clang::Scope*, unsigned int, clang::TagUseKind, clang::SourceLocation, clang::SourceLocation, clang::CXXScopeSpec&amp;, clang::TemplateIdAnnotation&amp;, clang::ParsedAttributesView const&amp;, llvm::MutableArrayRef&lt;clang::TemplateParameterList*&gt;, clang::SkipBodyInfo*) (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x103b2c708)
#<!-- -->28 0x000000010833aa60 clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&amp;) (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x1033d6a60)
#<!-- -->29 0x0000000108315b24 clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x1033b1b24)
#<!-- -->30 0x00000001083d9bec clang::Parser::ParseDeclarationAfterTemplate(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo&amp;, clang::ParsingDeclRAIIObject&amp;, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::AccessSpecifier) (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x103475bec)
#<!-- -->31 0x00000001083d8c90 clang::Parser::ParseTemplateDeclarationOrSpecialization(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::AccessSpecifier) (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x103474c90)
#<!-- -->32 0x00000001083d84ec clang::Parser::ParseDeclarationStartingWithTemplate(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;) (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x1034744ec)
#<!-- -->33 0x000000010830f4ac clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x1033ab4ac)
#<!-- -->34 0x00000001083ea540 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x103486540)
#<!-- -->35 0x0000000108331b54 clang::Parser::ParseInnerNamespace(llvm::SmallVector&lt;clang::Parser::InnerNamespaceInfo, 4u&gt; const&amp;, unsigned int, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::BalancedDelimiterTracker&amp;) (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x1033cdb54)
#<!-- -->36 0x0000000108331200 clang::Parser::ParseNamespace(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::SourceLocation) (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x1033cd200)
#<!-- -->37 0x000000010830f6fc clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x1033ab6fc)
#<!-- -->38 0x00000001083ea540 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x103486540)
#<!-- -->39 0x00000001083e8f40 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x103484f40)
#<!-- -->40 0x00000001082f62a0 clang::ParseAST(clang::Sema&amp;, bool, bool) (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x1033922a0)
#<!-- -->41 0x000000010767c238 clang::FrontendAction::Execute() (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x102718238)
#<!-- -->42 0x00000001075f546c clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x10269146c)
#<!-- -->43 0x00000001076ff1cc clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x10279b1cc)
#<!-- -->44 0x0000000104f6a458 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x100006458)
#<!-- -->45 0x0000000104f67f00 ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x100003f00)
#<!-- -->46 0x000000010742a068 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::__1::optional&lt;llvm::StringRef&gt;&gt;, std::__1::basic_string&lt;char, std::__1::char_traits&lt;char&gt;, std::__1::allocator&lt;char&gt;&gt;*, bool*) const::$_0&gt;(long) (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x1024c6068)
#<!-- -->47 0x000000010694de2c llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x1019e9e2c)
#<!-- -->48 0x0000000107429a9c clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::__1::optional&lt;llvm::StringRef&gt;&gt;, std::__1::basic_string&lt;char, std::__1::char_traits&lt;char&gt;, std::__1::allocator&lt;char&gt;&gt;*, bool*) const (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x1024c5a9c)
#<!-- -->49 0x00000001073eba18 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x102487a18)
#<!-- -->50 0x000000010740823c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::__1::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x1024a423c)
#<!-- -->51 0x0000000104f66b8c clang_main(int, char**, llvm::ToolContext const&amp;) (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x100002b8c)
#<!-- -->52 0x0000000104f75398 main (/Users/tal/local/llvm/20250825.140242/bin/clang-22+0x100011398)
#<!-- -->53 0x000000018fbeab98
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
clang version 22.0.0git (https://github.com/llvm/llvm-project.git 9801a0f62e49cbd81ce8352ab140af7c240e51ba)
Target: arm64-apple-darwin24.6.0
Thread model: posix
InstalledDir: /Users/tal/local/llvm/20250825.140242/bin
clang++: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: /var/folders/20/qtz798xj7pv9k3tfl9_w20mm0000gn/T/vector_seq_view-35fab3.hh
clang++: note: diagnostic msg: /var/folders/20/qtz798xj7pv9k3tfl9_w20mm0000gn/T/vector_seq_view-35fab3.sh
clang++: note: diagnostic msg: Crash backtrace is located in
clang++: note: diagnostic msg: /Users/tal/Library/Logs/DiagnosticReports/clang-22_&lt;YYYY-MM-DD-HHMMSS&gt;_&lt;hostname&gt;.crash
clang++: note: diagnostic msg: (choose the .crash file that corresponds to your crash)
clang++: note: diagnostic msg: 

********************
```

@<!-- -->mizvekov probably also related to https://github.com/llvm/llvm-project/pull/147835 ?
</details>


---

