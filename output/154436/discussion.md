# Assertion failed: (Capacity - Index == TypeLoc::getFullDataSizeForType(T) && "incorrect data size provided to CreateTypeSourceInfo!"), function pushImpl, file TypeLocBuilder.cpp, line 183

**Author:** seanm
**URL:** https://github.com/llvm/llvm-project/issues/154436
**Status:** Closed
**Labels:** clang:frontend, regression, confirmed, crash
**Closed Date:** 2025-08-20T19:24:42Z

## Body

Building [ITK](https://github.com/InsightSoftwareConsortium/ITK/) using today's clang (6127e46ff86bc660c0de5e7ece764005c91a1aaa) asserts:

```
Assertion failed: (Capacity - Index == TypeLoc::getFullDataSizeForType(T) && "incorrect data size provided to CreateTypeSourceInfo!"), function pushImpl, file TypeLocBuilder.cpp, line 183.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /Users/builder/llvm/llvm-install/bin/clang++ -I/Users/builder/external/ITK/Modules/ThirdParty/Eigen3/src/itkeigen/.. -I/Users/builder/external/ITK-ClangMain-dbg-arm64/Modules/ThirdParty/Eigen3/src -I/Users/builder/external/ITK-ClangMain-dbg-arm64/Modules/ThirdParty/KWSys/src -I/Users/builder/external/ITK/Modules/ThirdParty/VNL/src/vxl/v3p/netlib -I/Users/builder/external/ITK/Modules/ThirdParty/VNL/src/vxl/vcl -I/Users/builder/external/ITK/Modules/ThirdParty/VNL/src/vxl/core -I/Users/builder/external/ITK-ClangMain-dbg-arm64/Modules/ThirdParty/VNL/src/vxl/v3p/netlib -I/Users/builder/external/ITK-ClangMain-dbg-arm64/Modules/ThirdParty/VNL/src/vxl/vcl -I/Users/builder/external/ITK-ClangMain-dbg-arm64/Modules/ThirdParty/VNL/src/vxl/core -I/Users/builder/external/ITK-ClangMain-dbg-arm64/Modules/Core/Common -I/Users/builder/external/ITK/Modules/Core/Common/include -I/Users/builder/external/ITK/Modules/Core/ImageAdaptors/include -I/Users/builder/external/ITK-ClangMain-dbg-arm64/Modules/ThirdParty/Netlib -I/Users/builder/external/ITK/Modules/Numerics/Statistics/include -I/Users/builder/external/ITK/Modules/Filtering/ImageFilterBase/include -I/Users/builder/external/ITK/Modules/Core/Transform/include -I/Users/builder/external/ITK/Modules/Core/ImageFunction/include -I/Users/builder/external/ITK/Modules/Filtering/ImageGrid/include -I/Users/builder/external/ITK/Modules/Filtering/FFT/include -I/Users/builder/external/ITK/Modules/Filtering/ImageCompose/include -I/Users/builder/external/ITK-ClangMain-dbg-arm64/Modules/ThirdParty/ZLIB/src -I/Users/builder/external/ITK-ClangMain-dbg-arm64/Modules/ThirdParty/MetaIO/src/MetaIO/src -I/Users/builder/external/ITK/Modules/ThirdParty/MetaIO/src/MetaIO/src -I/Users/builder/external/ITK/Modules/Core/SpatialObjects/include -I/Users/builder/external/ITK/Modules/Filtering/ImageStatistics/include -I/Users/builder/external/ITK/Modules/Filtering/Path/include -I/Users/builder/external/ITK/Modules/Filtering/ImageIntensity/include -I/Users/builder/external/ITK/Modules/Filtering/Thresholding/include -I/Users/builder/external/ITK/Modules/Filtering/Convolution/include -I/Users/builder/external/ITK/Modules/Filtering/ImageSources/include -I/Users/builder/external/ITK/Modules/Filtering/Smoothing/include -Weverything -Wno-padded -Wno-missing-noreturn -Wno-unused-macros -Wno-missing-prototypes -Wno-sign-conversion -Wno-conversion -Wno-unreachable-code-break -Wno-reserved-id-macro -Wno-double-promotion -Wno-documentation -Wno-documentation-unknown-command -Wno-deprecated-declarations -Wno-switch-default -Wno-weak-vtables -Wno-c++98-compat-pedantic -Wno-global-constructors -Wno-unreachable-code -Wno-float-equal -Wno-exit-time-destructors -Wno-switch-enum -Wno-covered-switch-default -Wno-missing-variable-declarations -Wno-undef -Wno-weak-template-vtables -Wno-conditional-uninitialized -Wno-unused-member-function -Wno-old-style-cast -Wno-over-aligned -Wno-disabled-macro-expansion -Wno-undefined-func-template -Wno-shift-sign-overflow -Wno-zero-as-null-pointer-constant -Wno-unused-template -Wno-alloca -Wno-used-but-marked-unused -Wno-gnu-zero-variadic-macro-arguments -Wno-reserved-identifier -fstack-protector-all -ftrapv -ftrivial-auto-var-init=pattern -std=c++23 -D_LIBCPP_HARDENING_MODE=_LIBCPP_HARDENING_MODE_DEBUG -Wall -Wcast-align -Wdisabled-optimization -Wextra -Wformat=2 -Winvalid-pch -Wno-format-nonliteral -Wpointer-arith -Wshadow -Wunused -Wwrite-strings -Wno-strict-overflow -Wno-deprecated -Wno-invalid-offsetof -Wno-undefined-var-template -Woverloaded-virtual -Wctad-maybe-unsupported -g -std=c++17 -arch arm64 -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX26.0.sdk -mmacosx-version-min=26.0 -fPIE -MD -MT Modules/Filtering/Smoothing/CMakeFiles/ITKSmoothingHeaderTest1.dir/test/ITKSmoothingHeaderTest1.cxx.o -MF CMakeFiles/ITKSmoothingHeaderTest1.dir/test/ITKSmoothingHeaderTest1.cxx.o.d -o CMakeFiles/ITKSmoothingHeaderTest1.dir/test/ITKSmoothingHeaderTest1.cxx.o -c /Users/builder/external/ITK-ClangMain-dbg-arm64/Modules/Filtering/Smoothing/test/ITKSmoothingHeaderTest1.cxx
1.	/Users/builder/external/ITK/Modules/Core/Common/include/itkShapedNeighborhoodIterator.h:186:5: current parser token '{'
2.	/Users/builder/external/ITK/Modules/Core/Common/include/itkShapedNeighborhoodIterator.h:25:1: parsing namespace 'itk'
3.	/Users/builder/external/ITK/Modules/Core/Common/include/itkShapedNeighborhoodIterator.h:149:1: parsing struct/union/class body 'itk::ShapedNeighborhoodIterator'
[ 40%] Built target hdf5-static
 #0 0x000000010534fa74 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/Users/builder/llvm/llvm-install/bin/clang-22+0x1025c7a74)
 #1 0x000000010534d9c0 llvm::sys::RunSignalHandlers() (/Users/builder/llvm/llvm-install/bin/clang-22+0x1025c59c0)
 #2 0x000000010534f29c llvm::sys::CleanupOnSignal(unsigned long) (/Users/builder/llvm/llvm-install/bin/clang-22+0x1025c729c)
 #3 0x00000001052af94c CrashRecoverySignalHandler(int) (/Users/builder/llvm/llvm-install/bin/clang-22+0x10252794c)
 #4 0x000000018f5f0704 (/usr/lib/system/libsystem_platform.dylib+0x1804d4704)
 #5 0x000000018f5e6848 (/usr/lib/system/libsystem_pthread.dylib+0x1804ca848)
 #6 0x000000018f4ec808 (/usr/lib/system/libsystem_c.dylib+0x1803d0808)
 #7 0x000000018f4eba3c (/usr/lib/system/libsystem_c.dylib+0x1803cfa3c)
 #8 0x0000000107c4fe58 clang::TypeLocBuilder::pushImpl(clang::QualType, unsigned long, unsigned int) (.cold.11) (/Users/builder/llvm/llvm-install/bin/clang-22+0x104ec7e58)
 #9 0x0000000106f923ec clang::TypeLocBuilder::pushImpl(clang::QualType, unsigned long, unsigned int) (/Users/builder/llvm/llvm-install/bin/clang-22+0x10420a3ec)
#10 0x0000000106968fd8 clang::TypeLocBuilder::pushTypeSpec(clang::QualType) (/Users/builder/llvm/llvm-install/bin/clang-22+0x103be0fd8)
#11 0x0000000106ab3314 clang::Sema::BuildMemInitializer(clang::Decl*, clang::Scope*, clang::CXXScopeSpec&, clang::IdentifierInfo*, clang::OpaquePtr<clang::QualType>, clang::DeclSpec const&, clang::SourceLocation, clang::Expr*, clang::SourceLocation) (/Users/builder/llvm/llvm-install/bin/clang-22+0x103d2b314)
#12 0x000000010683bea8 clang::Parser::ParseMemInitializer(clang::Decl*) (/Users/builder/llvm/llvm-install/bin/clang-22+0x103ab3ea8)
#13 0x000000010683b568 clang::Parser::ParseConstructorInitializer(clang::Decl*) (/Users/builder/llvm/llvm-install/bin/clang-22+0x103ab3568)
#14 0x00000001068078ac clang::Parser::ParseLexedMethodDef(clang::Parser::LexedMethod&) (/Users/builder/llvm/llvm-install/bin/clang-22+0x103a7f8ac)
#15 0x0000000106806a50 clang::Parser::ParseLexedMethodDefs(clang::Parser::ParsingClass&) (/Users/builder/llvm/llvm-install/bin/clang-22+0x103a7ea50)
#16 0x0000000106806a50 clang::Parser::ParseLexedMethodDefs(clang::Parser::ParsingClass&) (/Users/builder/llvm/llvm-install/bin/clang-22+0x103a7ea50)
#17 0x0000000106836120 clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&, unsigned int, clang::Decl*) (/Users/builder/llvm/llvm-install/bin/clang-22+0x103aae120)
#18 0x00000001068339f8 clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&) (/Users/builder/llvm/llvm-install/bin/clang-22+0x103aab9f8)
#19 0x0000000106819790 clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/Users/builder/llvm/llvm-install/bin/clang-22+0x103a91790)
#20 0x00000001068a4f20 clang::Parser::ParseDeclarationAfterTemplate(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo&, clang::ParsingDeclRAIIObject&, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (/Users/builder/llvm/llvm-install/bin/clang-22+0x103b1cf20)
#21 0x00000001068a4568 clang::Parser::ParseTemplateDeclarationOrSpecialization(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (/Users/builder/llvm/llvm-install/bin/clang-22+0x103b1c568)
#22 0x00000001068a3e80 clang::Parser::ParseDeclarationStartingWithTemplate(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&) (/Users/builder/llvm/llvm-install/bin/clang-22+0x103b1be80)
#23 0x0000000106814c6c clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/Users/builder/llvm/llvm-install/bin/clang-22+0x103a8cc6c)
#24 0x00000001068b1f10 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/Users/builder/llvm/llvm-install/bin/clang-22+0x103b29f10)
#25 0x000000010682dac8 clang::Parser::ParseInnerNamespace(llvm::SmallVector<clang::Parser::InnerNamespaceInfo, 4u> const&, unsigned int, clang::SourceLocation&, clang::ParsedAttributes&, clang::BalancedDelimiterTracker&) (/Users/builder/llvm/llvm-install/bin/clang-22+0x103aa5ac8)
#26 0x000000010682d4d0 clang::Parser::ParseNamespace(clang::DeclaratorContext, clang::SourceLocation&, clang::SourceLocation) (/Users/builder/llvm/llvm-install/bin/clang-22+0x103aa54d0)
#27 0x0000000106814dd0 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/Users/builder/llvm/llvm-install/bin/clang-22+0x103a8cdd0)
#28 0x00000001068b1f10 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/Users/builder/llvm/llvm-install/bin/clang-22+0x103b29f10)
#29 0x00000001068b0d8c clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/Users/builder/llvm/llvm-install/bin/clang-22+0x103b28d8c)
#30 0x000000010680393c clang::ParseAST(clang::Sema&, bool, bool) (/Users/builder/llvm/llvm-install/bin/clang-22+0x103a7b93c)
#31 0x0000000105d82bd8 clang::FrontendAction::Execute() (/Users/builder/llvm/llvm-install/bin/clang-22+0x102ffabd8)
#32 0x0000000105d235d0 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/Users/builder/llvm/llvm-install/bin/clang-22+0x102f9b5d0)
#33 0x0000000105df9d84 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/Users/builder/llvm/llvm-install/bin/clang-22+0x103071d84)
#34 0x0000000102d8d7ac cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/Users/builder/llvm/llvm-install/bin/clang-22+0x1000057ac)
#35 0x0000000102d8b9e0 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) (/Users/builder/llvm/llvm-install/bin/clang-22+0x1000039e0)
#36 0x0000000105bc0198 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::__1::optional<llvm::StringRef>>, std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>>*, bool*) const::$_0>(long) (/Users/builder/llvm/llvm-install/bin/clang-22+0x102e38198)
#37 0x00000001052af65c llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/Users/builder/llvm/llvm-install/bin/clang-22+0x10252765c)
#38 0x0000000105bbfc30 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::__1::optional<llvm::StringRef>>, std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>>*, bool*) const (/Users/builder/llvm/llvm-install/bin/clang-22+0x102e37c30)
#39 0x0000000105b93788 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/Users/builder/llvm/llvm-install/bin/clang-22+0x102e0b788)
#40 0x0000000105b93954 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::__1::pair<int, clang::driver::Command const*>>&, bool) const (/Users/builder/llvm/llvm-install/bin/clang-22+0x102e0b954)
#41 0x0000000105ba72f0 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::__1::pair<int, clang::driver::Command const*>>&) (/Users/builder/llvm/llvm-install/bin/clang-22+0x102e1f2f0)
#42 0x0000000102d8add8 clang_main(int, char**, llvm::ToolContext const&) (/Users/builder/llvm/llvm-install/bin/clang-22+0x100002dd8)
#43 0x0000000102d96a58 main (/Users/builder/llvm/llvm-install/bin/clang-22+0x10000ea58)
#44 0x000000018f229924
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
clang version 22.0.0git (https://github.com/llvm/llvm-project.git 6127e46ff86bc660c0de5e7ece764005c91a1aaa)
Target: arm64-apple-darwin25.0.0
Thread model: posix
InstalledDir: /Users/builder/llvm/llvm-install/bin
Build config: +assertions
[ 40%] Built target itklbfgs
[ 40%] Built target ITKDoubleConversion-all
[ 40%] Built target ITKKWSys-all
[ 40%] Built target itktestlib
clang++: note: diagnostic msg:
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: /var/folders/_0/fc9yww793jzbdm1vtncnfshh0000gn/T/ITKSmoothingHeaderTest1-e37434.cpp
clang++: note: diagnostic msg: /var/folders/_0/fc9yww793jzbdm1vtncnfshh0000gn/T/ITKSmoothingHeaderTest1-e37434.sh
clang++: note: diagnostic msg: Crash backtrace is located in
clang++: note: diagnostic msg: /Users/builder/Library/Logs/DiagnosticReports/clang-22_<YYYY-MM-DD-HHMMSS>_<hostname>.crash
clang++: note: diagnostic msg: (choose the .crash file that corresponds to your crash)
clang++: note: diagnostic msg:

********************
make[2]: *** [Modules/Filtering/Smoothing/CMakeFiles/ITKSmoothingHeaderTest1.dir/test/ITKSmoothingHeaderTest1.cxx.o] Error 1
make[1]: *** [Modules/Filtering/Smoothing/CMakeFiles/ITKSmoothingHeaderTest1.dir/all] Error 2
make[1]: *** Waiting for unfinished jobs....
```

Attached are the files it requested.

[ITKSmoothingHeaderTest1-e37434.zip](https://github.com/user-attachments/files/21873054/ITKSmoothingHeaderTest1-e37434.zip)

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Sean McBride (seanm)

<details>
Building [ITK](https://github.com/InsightSoftwareConsortium/ITK/) using today's clang (6127e46ff86bc660c0de5e7ece764005c91a1aaa) asserts:

```
Assertion failed: (Capacity - Index == TypeLoc::getFullDataSizeForType(T) &amp;&amp; "incorrect data size provided to CreateTypeSourceInfo!"), function pushImpl, file TypeLocBuilder.cpp, line 183.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /Users/builder/llvm/llvm-install/bin/clang++ -I/Users/builder/external/ITK/Modules/ThirdParty/Eigen3/src/itkeigen/.. -I/Users/builder/external/ITK-ClangMain-dbg-arm64/Modules/ThirdParty/Eigen3/src -I/Users/builder/external/ITK-ClangMain-dbg-arm64/Modules/ThirdParty/KWSys/src -I/Users/builder/external/ITK/Modules/ThirdParty/VNL/src/vxl/v3p/netlib -I/Users/builder/external/ITK/Modules/ThirdParty/VNL/src/vxl/vcl -I/Users/builder/external/ITK/Modules/ThirdParty/VNL/src/vxl/core -I/Users/builder/external/ITK-ClangMain-dbg-arm64/Modules/ThirdParty/VNL/src/vxl/v3p/netlib -I/Users/builder/external/ITK-ClangMain-dbg-arm64/Modules/ThirdParty/VNL/src/vxl/vcl -I/Users/builder/external/ITK-ClangMain-dbg-arm64/Modules/ThirdParty/VNL/src/vxl/core -I/Users/builder/external/ITK-ClangMain-dbg-arm64/Modules/Core/Common -I/Users/builder/external/ITK/Modules/Core/Common/include -I/Users/builder/external/ITK/Modules/Core/ImageAdaptors/include -I/Users/builder/external/ITK-ClangMain-dbg-arm64/Modules/ThirdParty/Netlib -I/Users/builder/external/ITK/Modules/Numerics/Statistics/include -I/Users/builder/external/ITK/Modules/Filtering/ImageFilterBase/include -I/Users/builder/external/ITK/Modules/Core/Transform/include -I/Users/builder/external/ITK/Modules/Core/ImageFunction/include -I/Users/builder/external/ITK/Modules/Filtering/ImageGrid/include -I/Users/builder/external/ITK/Modules/Filtering/FFT/include -I/Users/builder/external/ITK/Modules/Filtering/ImageCompose/include -I/Users/builder/external/ITK-ClangMain-dbg-arm64/Modules/ThirdParty/ZLIB/src -I/Users/builder/external/ITK-ClangMain-dbg-arm64/Modules/ThirdParty/MetaIO/src/MetaIO/src -I/Users/builder/external/ITK/Modules/ThirdParty/MetaIO/src/MetaIO/src -I/Users/builder/external/ITK/Modules/Core/SpatialObjects/include -I/Users/builder/external/ITK/Modules/Filtering/ImageStatistics/include -I/Users/builder/external/ITK/Modules/Filtering/Path/include -I/Users/builder/external/ITK/Modules/Filtering/ImageIntensity/include -I/Users/builder/external/ITK/Modules/Filtering/Thresholding/include -I/Users/builder/external/ITK/Modules/Filtering/Convolution/include -I/Users/builder/external/ITK/Modules/Filtering/ImageSources/include -I/Users/builder/external/ITK/Modules/Filtering/Smoothing/include -Weverything -Wno-padded -Wno-missing-noreturn -Wno-unused-macros -Wno-missing-prototypes -Wno-sign-conversion -Wno-conversion -Wno-unreachable-code-break -Wno-reserved-id-macro -Wno-double-promotion -Wno-documentation -Wno-documentation-unknown-command -Wno-deprecated-declarations -Wno-switch-default -Wno-weak-vtables -Wno-c++98-compat-pedantic -Wno-global-constructors -Wno-unreachable-code -Wno-float-equal -Wno-exit-time-destructors -Wno-switch-enum -Wno-covered-switch-default -Wno-missing-variable-declarations -Wno-undef -Wno-weak-template-vtables -Wno-conditional-uninitialized -Wno-unused-member-function -Wno-old-style-cast -Wno-over-aligned -Wno-disabled-macro-expansion -Wno-undefined-func-template -Wno-shift-sign-overflow -Wno-zero-as-null-pointer-constant -Wno-unused-template -Wno-alloca -Wno-used-but-marked-unused -Wno-gnu-zero-variadic-macro-arguments -Wno-reserved-identifier -fstack-protector-all -ftrapv -ftrivial-auto-var-init=pattern -std=c++23 -D_LIBCPP_HARDENING_MODE=_LIBCPP_HARDENING_MODE_DEBUG -Wall -Wcast-align -Wdisabled-optimization -Wextra -Wformat=2 -Winvalid-pch -Wno-format-nonliteral -Wpointer-arith -Wshadow -Wunused -Wwrite-strings -Wno-strict-overflow -Wno-deprecated -Wno-invalid-offsetof -Wno-undefined-var-template -Woverloaded-virtual -Wctad-maybe-unsupported -g -std=c++17 -arch arm64 -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX26.0.sdk -mmacosx-version-min=26.0 -fPIE -MD -MT Modules/Filtering/Smoothing/CMakeFiles/ITKSmoothingHeaderTest1.dir/test/ITKSmoothingHeaderTest1.cxx.o -MF CMakeFiles/ITKSmoothingHeaderTest1.dir/test/ITKSmoothingHeaderTest1.cxx.o.d -o CMakeFiles/ITKSmoothingHeaderTest1.dir/test/ITKSmoothingHeaderTest1.cxx.o -c /Users/builder/external/ITK-ClangMain-dbg-arm64/Modules/Filtering/Smoothing/test/ITKSmoothingHeaderTest1.cxx
1.	/Users/builder/external/ITK/Modules/Core/Common/include/itkShapedNeighborhoodIterator.h:186:5: current parser token '{'
2.	/Users/builder/external/ITK/Modules/Core/Common/include/itkShapedNeighborhoodIterator.h:25:1: parsing namespace 'itk'
3.	/Users/builder/external/ITK/Modules/Core/Common/include/itkShapedNeighborhoodIterator.h:149:1: parsing struct/union/class body 'itk::ShapedNeighborhoodIterator'
[ 40%] Built target hdf5-static
 #<!-- -->0 0x000000010534fa74 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/Users/builder/llvm/llvm-install/bin/clang-22+0x1025c7a74)
 #<!-- -->1 0x000000010534d9c0 llvm::sys::RunSignalHandlers() (/Users/builder/llvm/llvm-install/bin/clang-22+0x1025c59c0)
 #<!-- -->2 0x000000010534f29c llvm::sys::CleanupOnSignal(unsigned long) (/Users/builder/llvm/llvm-install/bin/clang-22+0x1025c729c)
 #<!-- -->3 0x00000001052af94c CrashRecoverySignalHandler(int) (/Users/builder/llvm/llvm-install/bin/clang-22+0x10252794c)
 #<!-- -->4 0x000000018f5f0704 (/usr/lib/system/libsystem_platform.dylib+0x1804d4704)
 #<!-- -->5 0x000000018f5e6848 (/usr/lib/system/libsystem_pthread.dylib+0x1804ca848)
 #<!-- -->6 0x000000018f4ec808 (/usr/lib/system/libsystem_c.dylib+0x1803d0808)
 #<!-- -->7 0x000000018f4eba3c (/usr/lib/system/libsystem_c.dylib+0x1803cfa3c)
 #<!-- -->8 0x0000000107c4fe58 clang::TypeLocBuilder::pushImpl(clang::QualType, unsigned long, unsigned int) (.cold.11) (/Users/builder/llvm/llvm-install/bin/clang-22+0x104ec7e58)
 #<!-- -->9 0x0000000106f923ec clang::TypeLocBuilder::pushImpl(clang::QualType, unsigned long, unsigned int) (/Users/builder/llvm/llvm-install/bin/clang-22+0x10420a3ec)
#<!-- -->10 0x0000000106968fd8 clang::TypeLocBuilder::pushTypeSpec(clang::QualType) (/Users/builder/llvm/llvm-install/bin/clang-22+0x103be0fd8)
#<!-- -->11 0x0000000106ab3314 clang::Sema::BuildMemInitializer(clang::Decl*, clang::Scope*, clang::CXXScopeSpec&amp;, clang::IdentifierInfo*, clang::OpaquePtr&lt;clang::QualType&gt;, clang::DeclSpec const&amp;, clang::SourceLocation, clang::Expr*, clang::SourceLocation) (/Users/builder/llvm/llvm-install/bin/clang-22+0x103d2b314)
#<!-- -->12 0x000000010683bea8 clang::Parser::ParseMemInitializer(clang::Decl*) (/Users/builder/llvm/llvm-install/bin/clang-22+0x103ab3ea8)
#<!-- -->13 0x000000010683b568 clang::Parser::ParseConstructorInitializer(clang::Decl*) (/Users/builder/llvm/llvm-install/bin/clang-22+0x103ab3568)
#<!-- -->14 0x00000001068078ac clang::Parser::ParseLexedMethodDef(clang::Parser::LexedMethod&amp;) (/Users/builder/llvm/llvm-install/bin/clang-22+0x103a7f8ac)
#<!-- -->15 0x0000000106806a50 clang::Parser::ParseLexedMethodDefs(clang::Parser::ParsingClass&amp;) (/Users/builder/llvm/llvm-install/bin/clang-22+0x103a7ea50)
#<!-- -->16 0x0000000106806a50 clang::Parser::ParseLexedMethodDefs(clang::Parser::ParsingClass&amp;) (/Users/builder/llvm/llvm-install/bin/clang-22+0x103a7ea50)
#<!-- -->17 0x0000000106836120 clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&amp;, unsigned int, clang::Decl*) (/Users/builder/llvm/llvm-install/bin/clang-22+0x103aae120)
#<!-- -->18 0x00000001068339f8 clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&amp;) (/Users/builder/llvm/llvm-install/bin/clang-22+0x103aab9f8)
#<!-- -->19 0x0000000106819790 clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/Users/builder/llvm/llvm-install/bin/clang-22+0x103a91790)
#<!-- -->20 0x00000001068a4f20 clang::Parser::ParseDeclarationAfterTemplate(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo&amp;, clang::ParsingDeclRAIIObject&amp;, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::AccessSpecifier) (/Users/builder/llvm/llvm-install/bin/clang-22+0x103b1cf20)
#<!-- -->21 0x00000001068a4568 clang::Parser::ParseTemplateDeclarationOrSpecialization(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::AccessSpecifier) (/Users/builder/llvm/llvm-install/bin/clang-22+0x103b1c568)
#<!-- -->22 0x00000001068a3e80 clang::Parser::ParseDeclarationStartingWithTemplate(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;) (/Users/builder/llvm/llvm-install/bin/clang-22+0x103b1be80)
#<!-- -->23 0x0000000106814c6c clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/Users/builder/llvm/llvm-install/bin/clang-22+0x103a8cc6c)
#<!-- -->24 0x00000001068b1f10 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/Users/builder/llvm/llvm-install/bin/clang-22+0x103b29f10)
#<!-- -->25 0x000000010682dac8 clang::Parser::ParseInnerNamespace(llvm::SmallVector&lt;clang::Parser::InnerNamespaceInfo, 4u&gt; const&amp;, unsigned int, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::BalancedDelimiterTracker&amp;) (/Users/builder/llvm/llvm-install/bin/clang-22+0x103aa5ac8)
#<!-- -->26 0x000000010682d4d0 clang::Parser::ParseNamespace(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::SourceLocation) (/Users/builder/llvm/llvm-install/bin/clang-22+0x103aa54d0)
#<!-- -->27 0x0000000106814dd0 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/Users/builder/llvm/llvm-install/bin/clang-22+0x103a8cdd0)
#<!-- -->28 0x00000001068b1f10 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/Users/builder/llvm/llvm-install/bin/clang-22+0x103b29f10)
#<!-- -->29 0x00000001068b0d8c clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/Users/builder/llvm/llvm-install/bin/clang-22+0x103b28d8c)
#<!-- -->30 0x000000010680393c clang::ParseAST(clang::Sema&amp;, bool, bool) (/Users/builder/llvm/llvm-install/bin/clang-22+0x103a7b93c)
#<!-- -->31 0x0000000105d82bd8 clang::FrontendAction::Execute() (/Users/builder/llvm/llvm-install/bin/clang-22+0x102ffabd8)
#<!-- -->32 0x0000000105d235d0 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/Users/builder/llvm/llvm-install/bin/clang-22+0x102f9b5d0)
#<!-- -->33 0x0000000105df9d84 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/Users/builder/llvm/llvm-install/bin/clang-22+0x103071d84)
#<!-- -->34 0x0000000102d8d7ac cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/Users/builder/llvm/llvm-install/bin/clang-22+0x1000057ac)
#<!-- -->35 0x0000000102d8b9e0 ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) (/Users/builder/llvm/llvm-install/bin/clang-22+0x1000039e0)
#<!-- -->36 0x0000000105bc0198 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::__1::optional&lt;llvm::StringRef&gt;&gt;, std::__1::basic_string&lt;char, std::__1::char_traits&lt;char&gt;, std::__1::allocator&lt;char&gt;&gt;*, bool*) const::$_0&gt;(long) (/Users/builder/llvm/llvm-install/bin/clang-22+0x102e38198)
#<!-- -->37 0x00000001052af65c llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/Users/builder/llvm/llvm-install/bin/clang-22+0x10252765c)
#<!-- -->38 0x0000000105bbfc30 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::__1::optional&lt;llvm::StringRef&gt;&gt;, std::__1::basic_string&lt;char, std::__1::char_traits&lt;char&gt;, std::__1::allocator&lt;char&gt;&gt;*, bool*) const (/Users/builder/llvm/llvm-install/bin/clang-22+0x102e37c30)
#<!-- -->39 0x0000000105b93788 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/Users/builder/llvm/llvm-install/bin/clang-22+0x102e0b788)
#<!-- -->40 0x0000000105b93954 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::__1::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/Users/builder/llvm/llvm-install/bin/clang-22+0x102e0b954)
#<!-- -->41 0x0000000105ba72f0 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::__1::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/Users/builder/llvm/llvm-install/bin/clang-22+0x102e1f2f0)
#<!-- -->42 0x0000000102d8add8 clang_main(int, char**, llvm::ToolContext const&amp;) (/Users/builder/llvm/llvm-install/bin/clang-22+0x100002dd8)
#<!-- -->43 0x0000000102d96a58 main (/Users/builder/llvm/llvm-install/bin/clang-22+0x10000ea58)
#<!-- -->44 0x000000018f229924
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
clang version 22.0.0git (https://github.com/llvm/llvm-project.git 6127e46ff86bc660c0de5e7ece764005c91a1aaa)
Target: arm64-apple-darwin25.0.0
Thread model: posix
InstalledDir: /Users/builder/llvm/llvm-install/bin
Build config: +assertions
[ 40%] Built target itklbfgs
[ 40%] Built target ITKDoubleConversion-all
[ 40%] Built target ITKKWSys-all
[ 40%] Built target itktestlib
clang++: note: diagnostic msg:
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: /var/folders/_0/fc9yww793jzbdm1vtncnfshh0000gn/T/ITKSmoothingHeaderTest1-e37434.cpp
clang++: note: diagnostic msg: /var/folders/_0/fc9yww793jzbdm1vtncnfshh0000gn/T/ITKSmoothingHeaderTest1-e37434.sh
clang++: note: diagnostic msg: Crash backtrace is located in
clang++: note: diagnostic msg: /Users/builder/Library/Logs/DiagnosticReports/clang-22_&lt;YYYY-MM-DD-HHMMSS&gt;_&lt;hostname&gt;.crash
clang++: note: diagnostic msg: (choose the .crash file that corresponds to your crash)
clang++: note: diagnostic msg:

********************
make[2]: *** [Modules/Filtering/Smoothing/CMakeFiles/ITKSmoothingHeaderTest1.dir/test/ITKSmoothingHeaderTest1.cxx.o] Error 1
make[1]: *** [Modules/Filtering/Smoothing/CMakeFiles/ITKSmoothingHeaderTest1.dir/all] Error 2
make[1]: *** Waiting for unfinished jobs....
```

Attached are the files it requested.

[ITKSmoothingHeaderTest1-e37434.zip](https://github.com/user-attachments/files/21873054/ITKSmoothingHeaderTest1-e37434.zip)
</details>


---

### Comment 2 - seanm

creduce reduced this to:

```c++
template < typename a  >
class b
  : a
  using c = a;
  using typename c::e;
  struct d :e
d()
      : e()
    {
```

then invoke clang++ trivially with no params `

```
% /Users/sean/llvm/llvm-install/bin/clang++ ITKFastMarchingHeaderTest1-c6c4f5.cpp
```


---

### Comment 3 - tbaederr

See https://godbolt.org/z/E8W8GKGqq

---

### Comment 4 - AaronBallman

Reduced further to something a bit closer to valid:
```
template <typename a>
class b {
  using typename a::e;
  struct d {
    d() : e() {}
  };
};
```
https://godbolt.org/z/EW4r9xqcf

and it only fails on trunk. CC @mizvekov 

---

### Comment 5 - seanm

I am bisecting... 10 steps to go...

---

### Comment 6 - seanm

Meh, my bisecting keeps hitting a compiler error:

```
/Users/sean/llvm/llvm-project/clang/lib/Frontend/Rewrite/RewriteObjC.cpp:2361:52: error: no member named 'getTagDeclType' in 'clang::ASTContext'
 2361 |   QualType argT = Context->getPointerType(Context->getTagDeclType(RD));
      |                                           ~~~~~~~  ^
```

I've skipped so many revisions, but it keeps saying `Bisecting: 14 revisions left to test after this (roughly 4 steps)`

As I don't need Obj-C for this case, is there a build option I can turn off in CMake that might allow me to not even need to compile that file?

---

### Comment 7 - mizvekov

@seanm Don't worry about bisecting it, I am posting a fix soon.

---

### Comment 8 - mizvekov

> As I don't need Obj-C for this case, is there I build option I can turn off in CMake that might allow me to not even need to compile that file?

You don't need to continue bisecting since I already have a fix and we know which commit introduced the regression.

But just FYI, building the objc rewriter is controlled by the `CLANG_ENABLE_OBJC_REWRITER` cmake config variable.

---

### Comment 9 - seanm

Confirmed fixed. Thanks!

---

