# [clang/TemplateInstantiator] crash (segfault) in TransformDependentTemplateSpecializationType

**Author:** kedartal
**URL:** https://github.com/llvm/llvm-project/issues/155260
**Status:** Closed
**Labels:** clang:frontend, crash
**Closed Date:** 2025-08-25T17:45:52Z

## Body

segfault when compiling [this source](https://github.com/stan-dev/math/blob/develop/stan/math/prim/meta/plain_type.hpp)

There's no backtrace file created in `/Users/tal/Library/Logs/DiagnosticReports/` on my system, but the crash reproduces easily with [`/var/folders/20/qtz798xj7pv9k3tfl9_w20mm0000gn/T/plain_type-7ba0c5.hh`](https://github.com/user-attachments/files/21972339/plain_type-7ba0c5.zip)
 
The same code compiles fine with commit `c188e1db6a4c469f5ad92c4ce088fd113335410f` (~ August 1st 2025), anecdotal evidence suggests that the issue was introduced at least a week ago.

```
/Users/tal/local/llvm/latest/bin/clang++ -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX15.5.sdk -I./external/stan.git/lib/stan_math -I./external/stan.git/lib/stan_math/lib/eigen_3.4.0 -S ./external/stan.git/lib/stan_math/stan/math/prim/meta/plain_type.hpp
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /Users/tal/local/llvm/latest/bin/clang++ -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX15.5.sdk -I./external/stan.git/lib/stan_math -I./external/stan.git/lib/stan_math/lib/eigen_3.4.0 -S ./external/stan.git/lib/stan_math/stan/math/prim/meta/plain_type.hpp
1.	./external/stan.git/lib/stan_math/stan/math/prim/meta/plain_type.hpp:52:5: current parser token ':'
2.	./external/stan.git/lib/stan_math/stan/math/prim/meta/plain_type.hpp:9:1: parsing namespace 'stan'
3.	./external/stan.git/lib/stan_math/stan/math/prim/meta/plain_type.hpp:44:1: parsing namespace 'stan::internal'
 #0 0x0000000101be5830 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x101aa1830)
 #1 0x0000000101be2de0 llvm::sys::RunSignalHandlers() (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x101a9ede0)
 #2 0x0000000101be4d28 llvm::sys::CleanupOnSignal(unsigned long) (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x101aa0d28)
 #3 0x0000000101b27c08 CrashRecoverySignalHandler(int) (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x1019e3c08)
 #4 0x000000018ffc56a4 (/usr/lib/system/libsystem_platform.dylib+0x1804ad6a4)
 #5 0x0000000103d87320 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformDependentTemplateSpecializationType(clang::TypeLocBuilder&, clang::DependentTemplateSpecializationTypeLoc, clang::QualType, clang::NamedDecl*, bool) (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x103c43320)
 #6 0x0000000103d87320 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformDependentTemplateSpecializationType(clang::TypeLocBuilder&, clang::DependentTemplateSpecializationTypeLoc, clang::QualType, clang::NamedDecl*, bool) (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x103c43320)
 #7 0x0000000103d6d72c clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformNestedNameSpecifierLoc(clang::NestedNameSpecifierLoc, clang::QualType, clang::NamedDecl*) (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x103c2972c)
 #8 0x0000000103d87ea8 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformDependentNameType(clang::TypeLocBuilder&, clang::DependentNameTypeLoc, bool, clang::QualType, clang::NamedDecl*) (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x103c43ea8)
 #9 0x0000000103d623a0 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeSourceInfo*) (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x103c1e3a0)
#10 0x0000000103d6acf0 (anonymous namespace)::TemplateInstantiator::TransformTemplateArgument(clang::TemplateArgumentLoc const&, clang::TemplateArgumentLoc&, bool) (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x103c26cf0)
#11 0x0000000103d7df44 bool clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformTemplateArguments<clang::TemplateArgumentLocInventIterator<(anonymous namespace)::TemplateInstantiator, clang::TemplateArgument const*>>(clang::TemplateArgumentLocInventIterator<(anonymous namespace)::TemplateInstantiator, clang::TemplateArgument const*>, clang::TemplateArgumentLocInventIterator<(anonymous namespace)::TemplateInstantiator, clang::TemplateArgument const*>, clang::TemplateArgumentListInfo&, bool) (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x103c39f44)
#12 0x0000000103d8e720 (anonymous namespace)::TemplateInstantiator::TransformTemplateSpecializationType(clang::TypeLocBuilder&, clang::TemplateSpecializationTypeLoc) (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x103c4a720)
#13 0x0000000103d6ccfc clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformNestedNameSpecifierLoc(clang::NestedNameSpecifierLoc, clang::QualType, clang::NamedDecl*) (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x103c28cfc)
#14 0x0000000103d87ea8 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformDependentNameType(clang::TypeLocBuilder&, clang::DependentNameTypeLoc, bool, clang::QualType, clang::NamedDecl*) (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x103c43ea8)
#15 0x0000000103d623a0 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeSourceInfo*) (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x103c1e3a0)
#16 0x0000000103d6acf0 (anonymous namespace)::TemplateInstantiator::TransformTemplateArgument(clang::TemplateArgumentLoc const&, clang::TemplateArgumentLoc&, bool) (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x103c26cf0)
#17 0x0000000103d6b304 bool clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformTemplateArguments<clang::TemplateArgumentLoc const*>(clang::TemplateArgumentLoc const*, clang::TemplateArgumentLoc const*, clang::TemplateArgumentListInfo&, bool) (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x103c27304)
#18 0x0000000103d22aa8 FinishTemplateArgumentDeduction(clang::Sema&, clang::NamedDecl*, clang::TemplateParameterList*, clang::TemplateDecl*, bool, llvm::ArrayRef<clang::TemplateArgumentLoc>, llvm::ArrayRef<clang::TemplateArgument>, llvm::SmallVectorImpl<clang::DeducedTemplateArgument>&, clang::sema::TemplateDeductionInfo&, bool) (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x103bdeaa8)
#19 0x0000000103d24e4c FinishTemplateArgumentDeduction(clang::Sema&, clang::NamedDecl*, clang::TemplateParameterList*, clang::TemplateDecl*, bool, llvm::ArrayRef<clang::TemplateArgument>, llvm::ArrayRef<clang::TemplateArgument>, llvm::SmallVectorImpl<clang::DeducedTemplateArgument>&, clang::sema::TemplateDeductionInfo&, bool) (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x103be0e4c)
#20 0x0000000103d24974 bool isAtLeastAsSpecializedAs<clang::ClassTemplatePartialSpecializationDecl>(clang::Sema&, clang::QualType, clang::QualType, clang::ClassTemplatePartialSpecializationDecl*, clang::TemplateDecl*, clang::sema::TemplateDeductionInfo&) (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x103be0974)
#21 0x0000000103cec758 clang::Sema::isMoreSpecializedThanPrimary(clang::ClassTemplatePartialSpecializationDecl*, clang::sema::TemplateDeductionInfo&) (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x103ba8758)
#22 0x0000000103c5fd40 clang::Sema::CheckTemplatePartialSpecialization(clang::ClassTemplatePartialSpecializationDecl*) (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x103b1bd40)
#23 0x0000000103c6f8f0 clang::Sema::ActOnClassTemplateSpecialization(clang::Scope*, unsigned int, clang::TagUseKind, clang::SourceLocation, clang::SourceLocation, clang::CXXScopeSpec&, clang::TemplateIdAnnotation&, clang::ParsedAttributesView const&, llvm::MutableArrayRef<clang::TemplateParameterList*>, clang::SkipBodyInfo*) (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x103b2b8f0)
#24 0x0000000103514c3c clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&) (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x1033d0c3c)
#25 0x00000001034efd08 clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x1033abd08)
#26 0x00000001035b3b2c clang::Parser::ParseDeclarationAfterTemplate(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo&, clang::ParsingDeclRAIIObject&, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x10346fb2c)
#27 0x00000001035b2bd0 clang::Parser::ParseTemplateDeclarationOrSpecialization(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x10346ebd0)
#28 0x00000001035b242c clang::Parser::ParseDeclarationStartingWithTemplate(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&) (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x10346e42c)
#29 0x00000001034e9690 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x1033a5690)
#30 0x00000001035c4438 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x103480438)
#31 0x000000010350bd30 clang::Parser::ParseInnerNamespace(llvm::SmallVector<clang::Parser::InnerNamespaceInfo, 4u> const&, unsigned int, clang::SourceLocation&, clang::ParsedAttributes&, clang::BalancedDelimiterTracker&) (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x1033c7d30)
#32 0x000000010350b3dc clang::Parser::ParseNamespace(clang::DeclaratorContext, clang::SourceLocation&, clang::SourceLocation) (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x1033c73dc)
#33 0x00000001034e98e0 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x1033a58e0)
#34 0x00000001035c4438 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x103480438)
#35 0x000000010350bd30 clang::Parser::ParseInnerNamespace(llvm::SmallVector<clang::Parser::InnerNamespaceInfo, 4u> const&, unsigned int, clang::SourceLocation&, clang::ParsedAttributes&, clang::BalancedDelimiterTracker&) (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x1033c7d30)
#36 0x000000010350b3dc clang::Parser::ParseNamespace(clang::DeclaratorContext, clang::SourceLocation&, clang::SourceLocation) (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x1033c73dc)
#37 0x00000001034e98e0 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x1033a58e0)
#38 0x00000001035c4438 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x103480438)
#39 0x00000001035c2e38 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x10347ee38)
#40 0x00000001034d048c clang::ParseAST(clang::Sema&, bool, bool) (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x10338c48c)
#41 0x0000000102858020 clang::FrontendAction::Execute() (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x102714020)
#42 0x00000001027d12b0 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x10268d2b0)
#43 0x00000001028daf64 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x102796f64)
#44 0x000000010014a450 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x100006450)
#45 0x0000000100147efc ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x100003efc)
#46 0x0000000102605e44 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::__1::optional<llvm::StringRef>>, std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>>*, bool*) const::$_0>(long) (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x1024c1e44)
#47 0x0000000101b27908 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x1019e3908)
#48 0x0000000102605878 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::__1::optional<llvm::StringRef>>, std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>>*, bool*) const (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x1024c1878)
#49 0x00000001025c7814 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x102483814)
#50 0x00000001025e4030 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::__1::pair<int, clang::driver::Command const*>>&) (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x1024a0030)
#51 0x0000000100146b88 clang_main(int, char**, llvm::ToolContext const&) (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x100002b88)
#52 0x0000000100155388 main (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x100011388)
#53 0x000000018fbeab98
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
clang version 22.0.0git (https://github.com/llvm/llvm-project.git af0f85ce0be02f9af028ca8a99248aca11f1b456)
Target: arm64-apple-darwin24.6.0
Thread model: posix
InstalledDir: /Users/tal/local/llvm/20250824.210110/bin
clang++: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: /var/folders/20/qtz798xj7pv9k3tfl9_w20mm0000gn/T/plain_type-7ba0c5.hh
clang++: note: diagnostic msg: /var/folders/20/qtz798xj7pv9k3tfl9_w20mm0000gn/T/plain_type-7ba0c5.sh
clang++: note: diagnostic msg: Crash backtrace is located in
clang++: note: diagnostic msg: /Users/tal/Library/Logs/DiagnosticReports/clang-22_<YYYY-MM-DD-HHMMSS>_<hostname>.crash
clang++: note: diagnostic msg: (choose the .crash file that corresponds to your crash)
clang++: note: diagnostic msg: 

********************
```


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Tal Kedar (kedartal)

<details>
segfault when compiling [this source](https://github.com/stan-dev/math/blob/develop/stan/math/prim/meta/plain_type.hpp)

There's no backtrace file created in `/Users/tal/Library/Logs/DiagnosticReports/` on my system, but the crash reproduces easily with [`/var/folders/20/qtz798xj7pv9k3tfl9_w20mm0000gn/T/plain_type-7ba0c5.hh`](https://github.com/user-attachments/files/21972339/plain_type-7ba0c5.zip)
 
The same code compiles fine with commit `c188e1db6a4c469f5ad92c4ce088fd113335410f` (~ August 1st 2025), anecdotal evidence suggests that the issue was introduced at least a week ago.

```
/Users/tal/local/llvm/latest/bin/clang++ -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX15.5.sdk -I./external/stan.git/lib/stan_math -I./external/stan.git/lib/stan_math/lib/eigen_3.4.0 -S ./external/stan.git/lib/stan_math/stan/math/prim/meta/plain_type.hpp
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /Users/tal/local/llvm/latest/bin/clang++ -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX15.5.sdk -I./external/stan.git/lib/stan_math -I./external/stan.git/lib/stan_math/lib/eigen_3.4.0 -S ./external/stan.git/lib/stan_math/stan/math/prim/meta/plain_type.hpp
1.	./external/stan.git/lib/stan_math/stan/math/prim/meta/plain_type.hpp:52:5: current parser token ':'
2.	./external/stan.git/lib/stan_math/stan/math/prim/meta/plain_type.hpp:9:1: parsing namespace 'stan'
3.	./external/stan.git/lib/stan_math/stan/math/prim/meta/plain_type.hpp:44:1: parsing namespace 'stan::internal'
 #<!-- -->0 0x0000000101be5830 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x101aa1830)
 #<!-- -->1 0x0000000101be2de0 llvm::sys::RunSignalHandlers() (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x101a9ede0)
 #<!-- -->2 0x0000000101be4d28 llvm::sys::CleanupOnSignal(unsigned long) (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x101aa0d28)
 #<!-- -->3 0x0000000101b27c08 CrashRecoverySignalHandler(int) (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x1019e3c08)
 #<!-- -->4 0x000000018ffc56a4 (/usr/lib/system/libsystem_platform.dylib+0x1804ad6a4)
 #<!-- -->5 0x0000000103d87320 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformDependentTemplateSpecializationType(clang::TypeLocBuilder&amp;, clang::DependentTemplateSpecializationTypeLoc, clang::QualType, clang::NamedDecl*, bool) (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x103c43320)
 #<!-- -->6 0x0000000103d87320 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformDependentTemplateSpecializationType(clang::TypeLocBuilder&amp;, clang::DependentTemplateSpecializationTypeLoc, clang::QualType, clang::NamedDecl*, bool) (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x103c43320)
 #<!-- -->7 0x0000000103d6d72c clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformNestedNameSpecifierLoc(clang::NestedNameSpecifierLoc, clang::QualType, clang::NamedDecl*) (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x103c2972c)
 #<!-- -->8 0x0000000103d87ea8 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformDependentNameType(clang::TypeLocBuilder&amp;, clang::DependentNameTypeLoc, bool, clang::QualType, clang::NamedDecl*) (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x103c43ea8)
 #<!-- -->9 0x0000000103d623a0 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeSourceInfo*) (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x103c1e3a0)
#<!-- -->10 0x0000000103d6acf0 (anonymous namespace)::TemplateInstantiator::TransformTemplateArgument(clang::TemplateArgumentLoc const&amp;, clang::TemplateArgumentLoc&amp;, bool) (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x103c26cf0)
#<!-- -->11 0x0000000103d7df44 bool clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformTemplateArguments&lt;clang::TemplateArgumentLocInventIterator&lt;(anonymous namespace)::TemplateInstantiator, clang::TemplateArgument const*&gt;&gt;(clang::TemplateArgumentLocInventIterator&lt;(anonymous namespace)::TemplateInstantiator, clang::TemplateArgument const*&gt;, clang::TemplateArgumentLocInventIterator&lt;(anonymous namespace)::TemplateInstantiator, clang::TemplateArgument const*&gt;, clang::TemplateArgumentListInfo&amp;, bool) (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x103c39f44)
#<!-- -->12 0x0000000103d8e720 (anonymous namespace)::TemplateInstantiator::TransformTemplateSpecializationType(clang::TypeLocBuilder&amp;, clang::TemplateSpecializationTypeLoc) (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x103c4a720)
#<!-- -->13 0x0000000103d6ccfc clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformNestedNameSpecifierLoc(clang::NestedNameSpecifierLoc, clang::QualType, clang::NamedDecl*) (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x103c28cfc)
#<!-- -->14 0x0000000103d87ea8 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformDependentNameType(clang::TypeLocBuilder&amp;, clang::DependentNameTypeLoc, bool, clang::QualType, clang::NamedDecl*) (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x103c43ea8)
#<!-- -->15 0x0000000103d623a0 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeSourceInfo*) (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x103c1e3a0)
#<!-- -->16 0x0000000103d6acf0 (anonymous namespace)::TemplateInstantiator::TransformTemplateArgument(clang::TemplateArgumentLoc const&amp;, clang::TemplateArgumentLoc&amp;, bool) (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x103c26cf0)
#<!-- -->17 0x0000000103d6b304 bool clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformTemplateArguments&lt;clang::TemplateArgumentLoc const*&gt;(clang::TemplateArgumentLoc const*, clang::TemplateArgumentLoc const*, clang::TemplateArgumentListInfo&amp;, bool) (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x103c27304)
#<!-- -->18 0x0000000103d22aa8 FinishTemplateArgumentDeduction(clang::Sema&amp;, clang::NamedDecl*, clang::TemplateParameterList*, clang::TemplateDecl*, bool, llvm::ArrayRef&lt;clang::TemplateArgumentLoc&gt;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, llvm::SmallVectorImpl&lt;clang::DeducedTemplateArgument&gt;&amp;, clang::sema::TemplateDeductionInfo&amp;, bool) (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x103bdeaa8)
#<!-- -->19 0x0000000103d24e4c FinishTemplateArgumentDeduction(clang::Sema&amp;, clang::NamedDecl*, clang::TemplateParameterList*, clang::TemplateDecl*, bool, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, llvm::SmallVectorImpl&lt;clang::DeducedTemplateArgument&gt;&amp;, clang::sema::TemplateDeductionInfo&amp;, bool) (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x103be0e4c)
#<!-- -->20 0x0000000103d24974 bool isAtLeastAsSpecializedAs&lt;clang::ClassTemplatePartialSpecializationDecl&gt;(clang::Sema&amp;, clang::QualType, clang::QualType, clang::ClassTemplatePartialSpecializationDecl*, clang::TemplateDecl*, clang::sema::TemplateDeductionInfo&amp;) (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x103be0974)
#<!-- -->21 0x0000000103cec758 clang::Sema::isMoreSpecializedThanPrimary(clang::ClassTemplatePartialSpecializationDecl*, clang::sema::TemplateDeductionInfo&amp;) (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x103ba8758)
#<!-- -->22 0x0000000103c5fd40 clang::Sema::CheckTemplatePartialSpecialization(clang::ClassTemplatePartialSpecializationDecl*) (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x103b1bd40)
#<!-- -->23 0x0000000103c6f8f0 clang::Sema::ActOnClassTemplateSpecialization(clang::Scope*, unsigned int, clang::TagUseKind, clang::SourceLocation, clang::SourceLocation, clang::CXXScopeSpec&amp;, clang::TemplateIdAnnotation&amp;, clang::ParsedAttributesView const&amp;, llvm::MutableArrayRef&lt;clang::TemplateParameterList*&gt;, clang::SkipBodyInfo*) (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x103b2b8f0)
#<!-- -->24 0x0000000103514c3c clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&amp;) (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x1033d0c3c)
#<!-- -->25 0x00000001034efd08 clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x1033abd08)
#<!-- -->26 0x00000001035b3b2c clang::Parser::ParseDeclarationAfterTemplate(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo&amp;, clang::ParsingDeclRAIIObject&amp;, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::AccessSpecifier) (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x10346fb2c)
#<!-- -->27 0x00000001035b2bd0 clang::Parser::ParseTemplateDeclarationOrSpecialization(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::AccessSpecifier) (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x10346ebd0)
#<!-- -->28 0x00000001035b242c clang::Parser::ParseDeclarationStartingWithTemplate(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;) (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x10346e42c)
#<!-- -->29 0x00000001034e9690 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x1033a5690)
#<!-- -->30 0x00000001035c4438 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x103480438)
#<!-- -->31 0x000000010350bd30 clang::Parser::ParseInnerNamespace(llvm::SmallVector&lt;clang::Parser::InnerNamespaceInfo, 4u&gt; const&amp;, unsigned int, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::BalancedDelimiterTracker&amp;) (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x1033c7d30)
#<!-- -->32 0x000000010350b3dc clang::Parser::ParseNamespace(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::SourceLocation) (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x1033c73dc)
#<!-- -->33 0x00000001034e98e0 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x1033a58e0)
#<!-- -->34 0x00000001035c4438 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x103480438)
#<!-- -->35 0x000000010350bd30 clang::Parser::ParseInnerNamespace(llvm::SmallVector&lt;clang::Parser::InnerNamespaceInfo, 4u&gt; const&amp;, unsigned int, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::BalancedDelimiterTracker&amp;) (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x1033c7d30)
#<!-- -->36 0x000000010350b3dc clang::Parser::ParseNamespace(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::SourceLocation) (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x1033c73dc)
#<!-- -->37 0x00000001034e98e0 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x1033a58e0)
#<!-- -->38 0x00000001035c4438 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x103480438)
#<!-- -->39 0x00000001035c2e38 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x10347ee38)
#<!-- -->40 0x00000001034d048c clang::ParseAST(clang::Sema&amp;, bool, bool) (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x10338c48c)
#<!-- -->41 0x0000000102858020 clang::FrontendAction::Execute() (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x102714020)
#<!-- -->42 0x00000001027d12b0 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x10268d2b0)
#<!-- -->43 0x00000001028daf64 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x102796f64)
#<!-- -->44 0x000000010014a450 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x100006450)
#<!-- -->45 0x0000000100147efc ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x100003efc)
#<!-- -->46 0x0000000102605e44 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::__1::optional&lt;llvm::StringRef&gt;&gt;, std::__1::basic_string&lt;char, std::__1::char_traits&lt;char&gt;, std::__1::allocator&lt;char&gt;&gt;*, bool*) const::$_0&gt;(long) (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x1024c1e44)
#<!-- -->47 0x0000000101b27908 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x1019e3908)
#<!-- -->48 0x0000000102605878 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::__1::optional&lt;llvm::StringRef&gt;&gt;, std::__1::basic_string&lt;char, std::__1::char_traits&lt;char&gt;, std::__1::allocator&lt;char&gt;&gt;*, bool*) const (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x1024c1878)
#<!-- -->49 0x00000001025c7814 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x102483814)
#<!-- -->50 0x00000001025e4030 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::__1::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x1024a0030)
#<!-- -->51 0x0000000100146b88 clang_main(int, char**, llvm::ToolContext const&amp;) (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x100002b88)
#<!-- -->52 0x0000000100155388 main (/Users/tal/local/llvm/20250824.210110/bin/clang-22+0x100011388)
#<!-- -->53 0x000000018fbeab98
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
clang version 22.0.0git (https://github.com/llvm/llvm-project.git af0f85ce0be02f9af028ca8a99248aca11f1b456)
Target: arm64-apple-darwin24.6.0
Thread model: posix
InstalledDir: /Users/tal/local/llvm/20250824.210110/bin
clang++: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: /var/folders/20/qtz798xj7pv9k3tfl9_w20mm0000gn/T/plain_type-7ba0c5.hh
clang++: note: diagnostic msg: /var/folders/20/qtz798xj7pv9k3tfl9_w20mm0000gn/T/plain_type-7ba0c5.sh
clang++: note: diagnostic msg: Crash backtrace is located in
clang++: note: diagnostic msg: /Users/tal/Library/Logs/DiagnosticReports/clang-22_&lt;YYYY-MM-DD-HHMMSS&gt;_&lt;hostname&gt;.crash
clang++: note: diagnostic msg: (choose the .crash file that corresponds to your crash)
clang++: note: diagnostic msg: 

********************
```

</details>


---

### Comment 2 - kedartal

@mizvekov Could this be related to https://github.com/llvm/llvm-project/pull/147835 ?

---

