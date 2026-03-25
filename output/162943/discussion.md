# new ICE with clang 21.1.1

**Author:** sthalik
**URL:** https://github.com/llvm/llvm-project/issues/162943

## Body

Hey,

I'm getting a new ICE when parsing a file frrom a particular project. Here's console output and preprocessed files.

```
FAILED: external/magnum/src/Magnum/GL/CMakeFiles/MagnumGL.dir/Shader.cpp.obj 
c:\msys64\clang64\bin\clang++.exe -DCORRADE_IS_DEBUG_BUILD -DFlextGL_EXPORTS -DIMGUI_DISABLE_OBSOLETE_FUNCTIONS -DIMGUI_DISABLE_OBSOLETE_KEYIO -DJSON_HAS_THREE_WAY_COMPARISON=1 -DMagnumGL_EXPORTS -DNOMINMAX -DUNICODE -DWIN32_LEAN_AND_MEAN -D_CRT_SECURE_NO_WARNINGS -D_HAS_EXCEPTIONS=0 -D_LIBCPP_HARDENING_MODE=_LIBCPP_HARDENING_MODE_EXTENSIVE -D_LIBCPP_REMOVE_TRANSITIVE_INCLUDES -D_UNICODE -isystem D:/dev/floormat/external/magnum/src -isystem D:/dev/floormat/build/clang-asan/external/magnum/src -isystem D:/dev/floormat/external/corrade/src -isystem D:/dev/floormat/build/clang-asan/external/corrade/src -march=x86-64-v2 -mtune=native -mavx2 -maes -g -gcolumn-info -gdwarf-aranges -fno-rtti -O0 -fstack-protector-all -g -std=c++23 -fvisibility=hidden -fvisibility-inlines-hidden -fansi-escape-codes -fcolor-diagnostics -fsanitize=address,bounds,undefined -fassume-nothrow-exception-dtor -Wno-ambiguous-reversed-operator -Wno-reserved-macro-identifier -Wstrict-aliasing -Werror=strict-aliasing -Wno-float-equal -fuse-cxa-atexit -Wno-error -Wno-undef -Wno-cast-function-type -Wno-ignored-attributes -Wno-unused-function -Wno-unused-but-set-variable -Wno-error=return-type -MD -MT external/magnum/src/Magnum/GL/CMakeFiles/MagnumGL.dir/Shader.cpp.obj -MF external\magnum\src\Magnum\GL\CMakeFiles\MagnumGL.dir\Shader.cpp.obj.d -o external/magnum/src/Magnum/GL/CMakeFiles/MagnumGL.dir/Shader.cpp.obj -c D:/dev/floormat/external/magnum/src/Magnum/GL/Shader.cpp
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: c:\\msys64\\clang64\\bin\\clang++.exe -DCORRADE_IS_DEBUG_BUILD -DFlextGL_EXPORTS -DIMGUI_DISABLE_OBSOLETE_FUNCTIONS -DIMGUI_DISABLE_OBSOLETE_KEYIO -DJSON_HAS_THREE_WAY_COMPARISON=1 -DMagnumGL_EXPORTS -DNOMINMAX -DUNICODE -DWIN32_LEAN_AND_MEAN -D_CRT_SECURE_NO_WARNINGS -D_HAS_EXCEPTIONS=0 -D_LIBCPP_HARDENING_MODE=_LIBCPP_HARDENING_MODE_EXTENSIVE -D_LIBCPP_REMOVE_TRANSITIVE_INCLUDES -D_UNICODE -isystem D:/dev/floormat/external/magnum/src -isystem D:/dev/floormat/build/clang-asan/external/magnum/src -isystem D:/dev/floormat/external/corrade/src -isystem D:/dev/floormat/build/clang-asan/external/corrade/src -march=x86-64-v2 -mtune=native -mavx2 -maes -g -gcolumn-info -gdwarf-aranges -fno-rtti -O0 -fstack-protector-all -g -std=c++23 -fvisibility=hidden -fvisibility-inlines-hidden -fansi-escape-codes -fcolor-diagnostics -fsanitize=address,bounds,undefined -fassume-nothrow-exception-dtor -Wno-ambiguous-reversed-operator -Wno-reserved-macro-identifier -Wstrict-aliasing -Werror=strict-aliasing -Wno-float-equal -fuse-cxa-atexit -Wno-error -Wno-undef -Wno-cast-function-type -Wno-ignored-attributes -Wno-unused-function -Wno-unused-but-set-variable -Wno-error=return-type -MD -MT external/magnum/src/Magnum/GL/CMakeFiles/MagnumGL.dir/Shader.cpp.obj -MF external\\magnum\\src\\Magnum\\GL\\CMakeFiles\\MagnumGL.dir\\Shader.cpp.obj.d -o external/magnum/src/Magnum/GL/CMakeFiles/MagnumGL.dir/Shader.cpp.obj -c D:/dev/floormat/external/magnum/src/Magnum/GL/Shader.cpp
1.	D:/dev/floormat/external/magnum/src/Magnum/GL/Shader.cpp:809:69: current parser token ';'
2.	D:/dev/floormat/external/magnum/src/Magnum/GL/Shader.cpp:56:1: parsing namespace 'Magnum'
3.	D:/dev/floormat/external/magnum/src/Magnum/GL/Shader.cpp:56:20: parsing namespace 'Magnum::GL'
4.	D:/dev/floormat/external/magnum/src/Magnum/GL/Shader.cpp:809:52: parsing function body 'Magnum::GL::Shader::sources'
5.	D:/dev/floormat/external/magnum/src/Magnum/GL/Shader.cpp:809:52: in compound statement ('{}')
Exception Code: 0xC000001D
 #0 0x00007ffee025ed53 clang::ConversionFixItGenerator::tryToFixConversion(clang::Expr const*, clang::QualType, clang::QualType, clang::Sema&) (c:\msys64\clang64\bin\libclang-cpp.dll+0x83ed53)
 #1 0x00007ffee025e48e clang::OverloadCandidateSet::CompleteCandidates(clang::Sema&, clang::OverloadCandidateDisplayKind, llvm::ArrayRef<clang::Expr*>, clang::SourceLocation, llvm::function_ref<bool (clang::OverloadCandidate&)>) (c:\msys64\clang64\bin\libclang-cpp.dll+0x83e48e)
 #2 0x00007ffee025e0ad clang::OverloadCandidateSet::CompleteCandidates(clang::Sema&, clang::OverloadCandidateDisplayKind, llvm::ArrayRef<clang::Expr*>, clang::SourceLocation, llvm::function_ref<bool (clang::OverloadCandidate&)>) (c:\msys64\clang64\bin\libclang-cpp.dll+0x83e0ad)
 #3 0x00007ffee025dbdf clang::OverloadCandidateSet::CompleteCandidates(clang::Sema&, clang::OverloadCandidateDisplayKind, llvm::ArrayRef<clang::Expr*>, clang::SourceLocation, llvm::function_ref<bool (clang::OverloadCandidate&)>) (c:\msys64\clang64\bin\libclang-cpp.dll+0x83dbdf)
 #4 0x00007ffee019fe4d clang::OverloadCandidateSet::NoteCandidates(std::__1::pair<clang::SourceLocation, clang::PartialDiagnostic>, clang::Sema&, clang::OverloadCandidateDisplayKind, llvm::ArrayRef<clang::Expr*>, llvm::StringRef, clang::SourceLocation, llvm::function_ref<bool (clang::OverloadCandidate&)>) (c:\msys64\clang64\bin\libclang-cpp.dll+0x77fe4d)
 #5 0x00007ffee0375f25 llvm::APSInt::operator==(long long) const (c:\msys64\clang64\bin\libclang-cpp.dll+0x955f25)
 #6 0x00007ffedfb3684b clang::Sema::BuildOverloadedCallExpr(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (c:\msys64\clang64\bin\libclang-cpp.dll+0x11684b)
 #7 0x00007ffedfac40a6 clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) (c:\msys64\clang64\bin\libclang-cpp.dll+0xa40a6)
 #8 0x00007ffedfc675f3 clang::CXXConversionDecl::Create(clang::ASTContext&, clang::CXXRecordDecl*, clang::SourceLocation, clang::DeclarationNameInfo const&, clang::QualType, clang::TypeSourceInfo*, bool, bool, clang::ExplicitSpecifier, clang::ConstexprSpecKind, clang::SourceLocation, clang::AssociatedConstraint const&) (c:\msys64\clang64\bin\libclang-cpp.dll+0x2475f3)
 #9 0x00007ffedfbca213 clang::VarDecl::getInstantiatedFromStaticDataMember() const (c:\msys64\clang64\bin\libclang-cpp.dll+0x1aa213)
#10 0x00007ffedfbff239 clang::Sema::SubstTemplateArgument(clang::TemplateArgumentLoc const&, clang::MultiLevelTemplateArgumentList const&, clang::TemplateArgumentLoc&, clang::SourceLocation, clang::DeclarationName const&) (c:\msys64\clang64\bin\libclang-cpp.dll+0x1df239)
#11 0x00007ffedfef4728 clang::Sema::SubstDefaultTemplateArgumentIfAvailable(clang::TemplateDecl*, clang::SourceLocation, clang::SourceLocation, clang::Decl*, llvm::ArrayRef<clang::TemplateArgument>, llvm::ArrayRef<clang::TemplateArgument>, bool&) (c:\msys64\clang64\bin\libclang-cpp.dll+0x4d4728)
#12 0x00007ffedfeef35e clang::FunctionDecl::isOutOfLine() const (c:\msys64\clang64\bin\libclang-cpp.dll+0x4cf35e)
#13 0x00007ffedfe11401 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (c:\msys64\clang64\bin\libclang-cpp.dll+0x3f1401)
#14 0x00007ffedfe113a1 clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (c:\msys64\clang64\bin\libclang-cpp.dll+0x3f13a1)
#15 0x00007ffedfd0b5d7 clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, bool, bool, clang::QualType, clang::Expr::Classification, bool, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>, bool)>) (c:\msys64\clang64\bin\libclang-cpp.dll+0x2eb5d7)
#16 0x00007ffedfd0291d clang::OverloadCandidateSet::BestViableFunction(clang::Sema&, clang::SourceLocation, clang::OverloadCandidate*&) (c:\msys64\clang64\bin\libclang-cpp.dll+0x2e291d)
#17 0x00007ffee0154d43 clang::Sema::DiagnoseAssignmentResult(clang::AssignConvertType, clang::SourceLocation, clang::QualType, clang::QualType, clang::Expr*, clang::AssignmentAction, bool*) (c:\msys64\clang64\bin\libclang-cpp.dll+0x734d43)
#18 0x00007ffee00ece5d clang::Sema::BuildDeclarationNameExpr(clang::CXXScopeSpec const&, clang::DeclarationNameInfo const&, clang::NamedDecl*, clang::NamedDecl*, clang::TemplateArgumentListInfo const*, bool) (c:\msys64\clang64\bin\libclang-cpp.dll+0x6cce5d)
#19 0x00007ffedffa0856 clang::InitializationSequence::InitializationSequence(clang::Sema&, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::MutableArrayRef<clang::Expr*>, bool, bool) (c:\msys64\clang64\bin\libclang-cpp.dll+0x580856)
#20 0x00007ffedfb96094 clang::Sema::BuildCXXTypeConstructExpr(clang::TypeSourceInfo*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, bool) (c:\msys64\clang64\bin\libclang-cpp.dll+0x176094)
#21 0x00007ffedfbe9e61 clang::computeDependence(clang::DependentScopeDeclRefExpr*) (c:\msys64\clang64\bin\libclang-cpp.dll+0x1c9e61)
#22 0x00007ffedfbca213 clang::VarDecl::getInstantiatedFromStaticDataMember() const (c:\msys64\clang64\bin\libclang-cpp.dll+0x1aa213)
#23 0x00007ffedfbff239 clang::Sema::SubstTemplateArgument(clang::TemplateArgumentLoc const&, clang::MultiLevelTemplateArgumentList const&, clang::TemplateArgumentLoc&, clang::SourceLocation, clang::DeclarationName const&) (c:\msys64\clang64\bin\libclang-cpp.dll+0x1df239)
#24 0x00007ffedfef4728 clang::Sema::SubstDefaultTemplateArgumentIfAvailable(clang::TemplateDecl*, clang::SourceLocation, clang::SourceLocation, clang::Decl*, llvm::ArrayRef<clang::TemplateArgument>, llvm::ArrayRef<clang::TemplateArgument>, bool&) (c:\msys64\clang64\bin\libclang-cpp.dll+0x4d4728)
#25 0x00007ffedfeef35e clang::FunctionDecl::isOutOfLine() const (c:\msys64\clang64\bin\libclang-cpp.dll+0x4cf35e)
#26 0x00007ffedfe11401 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (c:\msys64\clang64\bin\libclang-cpp.dll+0x3f1401)
#27 0x00007ffedfe113a1 clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (c:\msys64\clang64\bin\libclang-cpp.dll+0x3f13a1)
#28 0x00007ffedfd0b5d7 clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, bool, bool, clang::QualType, clang::Expr::Classification, bool, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>, bool)>) (c:\msys64\clang64\bin\libclang-cpp.dll+0x2eb5d7)
#29 0x00007ffedfb2f630 clang::Sema::AddTemplateOverloadCandidate(clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::OverloadCandidateSet&, bool, bool, bool, clang::CallExpr::ADLCallKind, clang::OverloadCandidateParamOrder, bool) (c:\msys64\clang64\bin\libclang-cpp.dll+0x10f630)
#30 0x00007ffee0105c67 clang::computeDependence(clang::ImplicitValueInitExpr*) (c:\msys64\clang64\bin\libclang-cpp.dll+0x6e5c67)
#31 0x00007ffedfb1afa7 clang::Sema::PerformCopyInitialization(clang::InitializedEntity const&, clang::SourceLocation, clang::ActionResult<clang::Expr*, true>, bool, bool) (c:\msys64\clang64\bin\libclang-cpp.dll+0xfafa7)
#32 0x00007ffedfafec66 clang::Sema::ActOnReturnStmt(clang::SourceLocation, clang::Expr*, clang::Scope*) (c:\msys64\clang64\bin\libclang-cpp.dll+0xdec66)
#33 0x00007ffedfab4209 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (c:\msys64\clang64\bin\libclang-cpp.dll+0x94209)
#34 0x00007ffee0006328 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (c:\msys64\clang64\bin\libclang-cpp.dll+0x5e6328)
#35 0x00007ffee0002380 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (c:\msys64\clang64\bin\libclang-cpp.dll+0x5e2380)
#36 0x00007ffee00001d0 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (c:\msys64\clang64\bin\libclang-cpp.dll+0x5e01d0)
#37 0x00007ffedfffe205 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (c:\msys64\clang64\bin\libclang-cpp.dll+0x5de205)
#38 0x00007ffedfffdd01 clang::Parser::ParseInnerNamespace(llvm::SmallVector<clang::Parser::InnerNamespaceInfo, 4u> const&, unsigned int, clang::SourceLocation&, clang::ParsedAttributes&, clang::BalancedDelimiterTracker&) (c:\msys64\clang64\bin\libclang-cpp.dll+0x5ddd01)
#39 0x00007ffedfffc745 clang::Parser::ParseNamespace(clang::DeclaratorContext, clang::SourceLocation&, clang::SourceLocation) (c:\msys64\clang64\bin\libclang-cpp.dll+0x5dc745)
#40 0x00007ffedffff24a clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (c:\msys64\clang64\bin\libclang-cpp.dll+0x5df24a)
#41 0x00007ffedfffe0f4 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (c:\msys64\clang64\bin\libclang-cpp.dll+0x5de0f4)
#42 0x00007ffedfffdd01 clang::Parser::ParseInnerNamespace(llvm::SmallVector<clang::Parser::InnerNamespaceInfo, 4u> const&, unsigned int, clang::SourceLocation&, clang::ParsedAttributes&, clang::BalancedDelimiterTracker&) (c:\msys64\clang64\bin\libclang-cpp.dll+0x5ddd01)
#43 0x00007ffedfffc745 clang::Parser::ParseNamespace(clang::DeclaratorContext, clang::SourceLocation&, clang::SourceLocation) (c:\msys64\clang64\bin\libclang-cpp.dll+0x5dc745)
#44 0x00007ffedffff24a clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (c:\msys64\clang64\bin\libclang-cpp.dll+0x5df24a)
#45 0x00007ffedfffe0f4 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (c:\msys64\clang64\bin\libclang-cpp.dll+0x5de0f4)
#46 0x00007ffee0238d31 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (c:\msys64\clang64\bin\libclang-cpp.dll+0x818d31)
#47 0x00007ffee02389f4 clang::ParseAST(clang::Sema&, bool, bool) (c:\msys64\clang64\bin\libclang-cpp.dll+0x8189f4)
#48 0x00007ffee026170f clang::FrontendAction::Execute() (c:\msys64\clang64\bin\libclang-cpp.dll+0x84170f)
#49 0x00007ffee026127b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (c:\msys64\clang64\bin\libclang-cpp.dll+0x84127b)
#50 0x00007ffee0260d02 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (c:\msys64\clang64\bin\libclang-cpp.dll+0x840d02)
#51 0x00007ff7ad60442d cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (c:\msys64\clang64\bin\clang++.exe+0x442d)
#52 0x00007ff7ad603c06 llvm::raw_ostream::operator<<(llvm::StringRef) (c:\msys64\clang64\bin\clang++.exe+0x3c06)
#53 0x00007ffee15b463d void std::__1::vector<std::__1::optional<std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>>>, std::__1::allocator<std::__1::optional<std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>>>>>::__assign_with_size[abi:nn210101]<std::__1::optional<std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>>>*, std::__1::optional<std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>>>*>(std::__1::optional<std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>>>*, std::__1::optional<std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>>>*, long long) (c:\msys64\clang64\bin\libclang-cpp.dll+0x1b9463d)
#54 0x00007ffee3b1c6c5 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (c:\msys64\clang64\bin\libLLVM-21.dll+0xf1c6c5)
#55 0x00007ffee0385a57 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::__1::optional<llvm::StringRef>>, std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>>*, bool*) const (c:\msys64\clang64\bin\libclang-cpp.dll+0x965a57)
#56 0x00007ffee033f1a3 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (c:\msys64\clang64\bin\libclang-cpp.dll+0x91f1a3)
#57 0x00007ffee033ee20 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::__1::pair<int, clang::driver::Command const*>>&, bool) const (c:\msys64\clang64\bin\libclang-cpp.dll+0x91ee20)
#58 0x00007ffee033eb4b clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::__1::pair<int, clang::driver::Command const*>>&) (c:\msys64\clang64\bin\libclang-cpp.dll+0x91eb4b)
#59 0x00007ff7ad601bfc clang_main(int, char**, llvm::ToolContext const&) (c:\msys64\clang64\bin\clang++.exe+0x1bfc)
#60 0x00007ff7ad601075 main (c:\msys64\clang64\bin\clang++.exe+0x1075)
#61 0x00007ff7ad60a96e WinMainCRTStartup (c:\msys64\clang64\bin\clang++.exe+0xa96e)
#62 0x00007ff7ad60ac26 mainCRTStartup (c:\msys64\clang64\bin\clang++.exe+0xac26)
#63 0x00007fffc47b7374 (C:\Windows\System32\KERNEL32.DLL+0x17374)
#64 0x00007fffc621cc91 (C:\Windows\SYSTEM32\ntdll.dll+0x4cc91)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 21.1.1
Target: x86_64-w64-windows-gnu
Thread model: posix
InstalledDir: C:/msys64/clang64/bin
clang++: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: d:/temp/Shader-7c45b0.cpp
clang++: note: diagnostic msg: d:/temp/Shader-7c45b0.sh
clang++: note: diagnostic msg: 

********************

[Shader.cpp.txt](https://github.com/user-attachments/files/22859060/Shader.cpp.txt)
[Shader.sh.txt](https://github.com/user-attachments/files/22859058/Shader.sh.txt)

```

## Comments

### Comment 1 - sthalik

[Shader.sh.txt](https://github.com/user-attachments/files/22859072/Shader.sh.txt)

[Shader.cpp.txt](https://github.com/user-attachments/files/22859073/Shader.cpp.txt)

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Stanislaw Halik (sthalik)

<details>
Hey,

I'm getting a new ICE when parsing a file frrom a particular project. Here's console output and preprocessed files.

```
FAILED: external/magnum/src/Magnum/GL/CMakeFiles/MagnumGL.dir/Shader.cpp.obj 
c:\msys64\clang64\bin\clang++.exe -DCORRADE_IS_DEBUG_BUILD -DFlextGL_EXPORTS -DIMGUI_DISABLE_OBSOLETE_FUNCTIONS -DIMGUI_DISABLE_OBSOLETE_KEYIO -DJSON_HAS_THREE_WAY_COMPARISON=1 -DMagnumGL_EXPORTS -DNOMINMAX -DUNICODE -DWIN32_LEAN_AND_MEAN -D_CRT_SECURE_NO_WARNINGS -D_HAS_EXCEPTIONS=0 -D_LIBCPP_HARDENING_MODE=_LIBCPP_HARDENING_MODE_EXTENSIVE -D_LIBCPP_REMOVE_TRANSITIVE_INCLUDES -D_UNICODE -isystem D:/dev/floormat/external/magnum/src -isystem D:/dev/floormat/build/clang-asan/external/magnum/src -isystem D:/dev/floormat/external/corrade/src -isystem D:/dev/floormat/build/clang-asan/external/corrade/src -march=x86-64-v2 -mtune=native -mavx2 -maes -g -gcolumn-info -gdwarf-aranges -fno-rtti -O0 -fstack-protector-all -g -std=c++23 -fvisibility=hidden -fvisibility-inlines-hidden -fansi-escape-codes -fcolor-diagnostics -fsanitize=address,bounds,undefined -fassume-nothrow-exception-dtor -Wno-ambiguous-reversed-operator -Wno-reserved-macro-identifier -Wstrict-aliasing -Werror=strict-aliasing -Wno-float-equal -fuse-cxa-atexit -Wno-error -Wno-undef -Wno-cast-function-type -Wno-ignored-attributes -Wno-unused-function -Wno-unused-but-set-variable -Wno-error=return-type -MD -MT external/magnum/src/Magnum/GL/CMakeFiles/MagnumGL.dir/Shader.cpp.obj -MF external\magnum\src\Magnum\GL\CMakeFiles\MagnumGL.dir\Shader.cpp.obj.d -o external/magnum/src/Magnum/GL/CMakeFiles/MagnumGL.dir/Shader.cpp.obj -c D:/dev/floormat/external/magnum/src/Magnum/GL/Shader.cpp
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: c:\\msys64\\clang64\\bin\\clang++.exe -DCORRADE_IS_DEBUG_BUILD -DFlextGL_EXPORTS -DIMGUI_DISABLE_OBSOLETE_FUNCTIONS -DIMGUI_DISABLE_OBSOLETE_KEYIO -DJSON_HAS_THREE_WAY_COMPARISON=1 -DMagnumGL_EXPORTS -DNOMINMAX -DUNICODE -DWIN32_LEAN_AND_MEAN -D_CRT_SECURE_NO_WARNINGS -D_HAS_EXCEPTIONS=0 -D_LIBCPP_HARDENING_MODE=_LIBCPP_HARDENING_MODE_EXTENSIVE -D_LIBCPP_REMOVE_TRANSITIVE_INCLUDES -D_UNICODE -isystem D:/dev/floormat/external/magnum/src -isystem D:/dev/floormat/build/clang-asan/external/magnum/src -isystem D:/dev/floormat/external/corrade/src -isystem D:/dev/floormat/build/clang-asan/external/corrade/src -march=x86-64-v2 -mtune=native -mavx2 -maes -g -gcolumn-info -gdwarf-aranges -fno-rtti -O0 -fstack-protector-all -g -std=c++23 -fvisibility=hidden -fvisibility-inlines-hidden -fansi-escape-codes -fcolor-diagnostics -fsanitize=address,bounds,undefined -fassume-nothrow-exception-dtor -Wno-ambiguous-reversed-operator -Wno-reserved-macro-identifier -Wstrict-aliasing -Werror=strict-aliasing -Wno-float-equal -fuse-cxa-atexit -Wno-error -Wno-undef -Wno-cast-function-type -Wno-ignored-attributes -Wno-unused-function -Wno-unused-but-set-variable -Wno-error=return-type -MD -MT external/magnum/src/Magnum/GL/CMakeFiles/MagnumGL.dir/Shader.cpp.obj -MF external\\magnum\\src\\Magnum\\GL\\CMakeFiles\\MagnumGL.dir\\Shader.cpp.obj.d -o external/magnum/src/Magnum/GL/CMakeFiles/MagnumGL.dir/Shader.cpp.obj -c D:/dev/floormat/external/magnum/src/Magnum/GL/Shader.cpp
1.	D:/dev/floormat/external/magnum/src/Magnum/GL/Shader.cpp:809:69: current parser token ';'
2.	D:/dev/floormat/external/magnum/src/Magnum/GL/Shader.cpp:56:1: parsing namespace 'Magnum'
3.	D:/dev/floormat/external/magnum/src/Magnum/GL/Shader.cpp:56:20: parsing namespace 'Magnum::GL'
4.	D:/dev/floormat/external/magnum/src/Magnum/GL/Shader.cpp:809:52: parsing function body 'Magnum::GL::Shader::sources'
5.	D:/dev/floormat/external/magnum/src/Magnum/GL/Shader.cpp:809:52: in compound statement ('{}')
Exception Code: 0xC000001D
 #<!-- -->0 0x00007ffee025ed53 clang::ConversionFixItGenerator::tryToFixConversion(clang::Expr const*, clang::QualType, clang::QualType, clang::Sema&amp;) (c:\msys64\clang64\bin\libclang-cpp.dll+0x83ed53)
 #<!-- -->1 0x00007ffee025e48e clang::OverloadCandidateSet::CompleteCandidates(clang::Sema&amp;, clang::OverloadCandidateDisplayKind, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, llvm::function_ref&lt;bool (clang::OverloadCandidate&amp;)&gt;) (c:\msys64\clang64\bin\libclang-cpp.dll+0x83e48e)
 #<!-- -->2 0x00007ffee025e0ad clang::OverloadCandidateSet::CompleteCandidates(clang::Sema&amp;, clang::OverloadCandidateDisplayKind, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, llvm::function_ref&lt;bool (clang::OverloadCandidate&amp;)&gt;) (c:\msys64\clang64\bin\libclang-cpp.dll+0x83e0ad)
 #<!-- -->3 0x00007ffee025dbdf clang::OverloadCandidateSet::CompleteCandidates(clang::Sema&amp;, clang::OverloadCandidateDisplayKind, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, llvm::function_ref&lt;bool (clang::OverloadCandidate&amp;)&gt;) (c:\msys64\clang64\bin\libclang-cpp.dll+0x83dbdf)
 #<!-- -->4 0x00007ffee019fe4d clang::OverloadCandidateSet::NoteCandidates(std::__1::pair&lt;clang::SourceLocation, clang::PartialDiagnostic&gt;, clang::Sema&amp;, clang::OverloadCandidateDisplayKind, llvm::ArrayRef&lt;clang::Expr*&gt;, llvm::StringRef, clang::SourceLocation, llvm::function_ref&lt;bool (clang::OverloadCandidate&amp;)&gt;) (c:\msys64\clang64\bin\libclang-cpp.dll+0x77fe4d)
 #<!-- -->5 0x00007ffee0375f25 llvm::APSInt::operator==(long long) const (c:\msys64\clang64\bin\libclang-cpp.dll+0x955f25)
 #<!-- -->6 0x00007ffedfb3684b clang::Sema::BuildOverloadedCallExpr(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, bool) (c:\msys64\clang64\bin\libclang-cpp.dll+0x11684b)
 #<!-- -->7 0x00007ffedfac40a6 clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*) (c:\msys64\clang64\bin\libclang-cpp.dll+0xa40a6)
 #<!-- -->8 0x00007ffedfc675f3 clang::CXXConversionDecl::Create(clang::ASTContext&amp;, clang::CXXRecordDecl*, clang::SourceLocation, clang::DeclarationNameInfo const&amp;, clang::QualType, clang::TypeSourceInfo*, bool, bool, clang::ExplicitSpecifier, clang::ConstexprSpecKind, clang::SourceLocation, clang::AssociatedConstraint const&amp;) (c:\msys64\clang64\bin\libclang-cpp.dll+0x2475f3)
 #<!-- -->9 0x00007ffedfbca213 clang::VarDecl::getInstantiatedFromStaticDataMember() const (c:\msys64\clang64\bin\libclang-cpp.dll+0x1aa213)
#<!-- -->10 0x00007ffedfbff239 clang::Sema::SubstTemplateArgument(clang::TemplateArgumentLoc const&amp;, clang::MultiLevelTemplateArgumentList const&amp;, clang::TemplateArgumentLoc&amp;, clang::SourceLocation, clang::DeclarationName const&amp;) (c:\msys64\clang64\bin\libclang-cpp.dll+0x1df239)
#<!-- -->11 0x00007ffedfef4728 clang::Sema::SubstDefaultTemplateArgumentIfAvailable(clang::TemplateDecl*, clang::SourceLocation, clang::SourceLocation, clang::Decl*, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, bool&amp;) (c:\msys64\clang64\bin\libclang-cpp.dll+0x4d4728)
#<!-- -->12 0x00007ffedfeef35e clang::FunctionDecl::isOutOfLine() const (c:\msys64\clang64\bin\libclang-cpp.dll+0x4cf35e)
#<!-- -->13 0x00007ffedfe11401 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (c:\msys64\clang64\bin\libclang-cpp.dll+0x3f1401)
#<!-- -->14 0x00007ffedfe113a1 clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (c:\msys64\clang64\bin\libclang-cpp.dll+0x3f13a1)
#<!-- -->15 0x00007ffedfd0b5d7 clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, bool, bool, bool, clang::QualType, clang::Expr::Classification, bool, llvm::function_ref&lt;bool (llvm::ArrayRef&lt;clang::QualType&gt;, bool)&gt;) (c:\msys64\clang64\bin\libclang-cpp.dll+0x2eb5d7)
#<!-- -->16 0x00007ffedfd0291d clang::OverloadCandidateSet::BestViableFunction(clang::Sema&amp;, clang::SourceLocation, clang::OverloadCandidate*&amp;) (c:\msys64\clang64\bin\libclang-cpp.dll+0x2e291d)
#<!-- -->17 0x00007ffee0154d43 clang::Sema::DiagnoseAssignmentResult(clang::AssignConvertType, clang::SourceLocation, clang::QualType, clang::QualType, clang::Expr*, clang::AssignmentAction, bool*) (c:\msys64\clang64\bin\libclang-cpp.dll+0x734d43)
#<!-- -->18 0x00007ffee00ece5d clang::Sema::BuildDeclarationNameExpr(clang::CXXScopeSpec const&amp;, clang::DeclarationNameInfo const&amp;, clang::NamedDecl*, clang::NamedDecl*, clang::TemplateArgumentListInfo const*, bool) (c:\msys64\clang64\bin\libclang-cpp.dll+0x6cce5d)
#<!-- -->19 0x00007ffedffa0856 clang::InitializationSequence::InitializationSequence(clang::Sema&amp;, clang::InitializedEntity const&amp;, clang::InitializationKind const&amp;, llvm::MutableArrayRef&lt;clang::Expr*&gt;, bool, bool) (c:\msys64\clang64\bin\libclang-cpp.dll+0x580856)
#<!-- -->20 0x00007ffedfb96094 clang::Sema::BuildCXXTypeConstructExpr(clang::TypeSourceInfo*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, bool) (c:\msys64\clang64\bin\libclang-cpp.dll+0x176094)
#<!-- -->21 0x00007ffedfbe9e61 clang::computeDependence(clang::DependentScopeDeclRefExpr*) (c:\msys64\clang64\bin\libclang-cpp.dll+0x1c9e61)
#<!-- -->22 0x00007ffedfbca213 clang::VarDecl::getInstantiatedFromStaticDataMember() const (c:\msys64\clang64\bin\libclang-cpp.dll+0x1aa213)
#<!-- -->23 0x00007ffedfbff239 clang::Sema::SubstTemplateArgument(clang::TemplateArgumentLoc const&amp;, clang::MultiLevelTemplateArgumentList const&amp;, clang::TemplateArgumentLoc&amp;, clang::SourceLocation, clang::DeclarationName const&amp;) (c:\msys64\clang64\bin\libclang-cpp.dll+0x1df239)
#<!-- -->24 0x00007ffedfef4728 clang::Sema::SubstDefaultTemplateArgumentIfAvailable(clang::TemplateDecl*, clang::SourceLocation, clang::SourceLocation, clang::Decl*, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, bool&amp;) (c:\msys64\clang64\bin\libclang-cpp.dll+0x4d4728)
#<!-- -->25 0x00007ffedfeef35e clang::FunctionDecl::isOutOfLine() const (c:\msys64\clang64\bin\libclang-cpp.dll+0x4cf35e)
#<!-- -->26 0x00007ffedfe11401 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (c:\msys64\clang64\bin\libclang-cpp.dll+0x3f1401)
#<!-- -->27 0x00007ffedfe113a1 clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (c:\msys64\clang64\bin\libclang-cpp.dll+0x3f13a1)
#<!-- -->28 0x00007ffedfd0b5d7 clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, bool, bool, bool, clang::QualType, clang::Expr::Classification, bool, llvm::function_ref&lt;bool (llvm::ArrayRef&lt;clang::QualType&gt;, bool)&gt;) (c:\msys64\clang64\bin\libclang-cpp.dll+0x2eb5d7)
#<!-- -->29 0x00007ffedfb2f630 clang::Sema::AddTemplateOverloadCandidate(clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::OverloadCandidateSet&amp;, bool, bool, bool, clang::CallExpr::ADLCallKind, clang::OverloadCandidateParamOrder, bool) (c:\msys64\clang64\bin\libclang-cpp.dll+0x10f630)
#<!-- -->30 0x00007ffee0105c67 clang::computeDependence(clang::ImplicitValueInitExpr*) (c:\msys64\clang64\bin\libclang-cpp.dll+0x6e5c67)
#<!-- -->31 0x00007ffedfb1afa7 clang::Sema::PerformCopyInitialization(clang::InitializedEntity const&amp;, clang::SourceLocation, clang::ActionResult&lt;clang::Expr*, true&gt;, bool, bool) (c:\msys64\clang64\bin\libclang-cpp.dll+0xfafa7)
#<!-- -->32 0x00007ffedfafec66 clang::Sema::ActOnReturnStmt(clang::SourceLocation, clang::Expr*, clang::Scope*) (c:\msys64\clang64\bin\libclang-cpp.dll+0xdec66)
#<!-- -->33 0x00007ffedfab4209 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (c:\msys64\clang64\bin\libclang-cpp.dll+0x94209)
#<!-- -->34 0x00007ffee0006328 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (c:\msys64\clang64\bin\libclang-cpp.dll+0x5e6328)
#<!-- -->35 0x00007ffee0002380 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (c:\msys64\clang64\bin\libclang-cpp.dll+0x5e2380)
#<!-- -->36 0x00007ffee00001d0 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (c:\msys64\clang64\bin\libclang-cpp.dll+0x5e01d0)
#<!-- -->37 0x00007ffedfffe205 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (c:\msys64\clang64\bin\libclang-cpp.dll+0x5de205)
#<!-- -->38 0x00007ffedfffdd01 clang::Parser::ParseInnerNamespace(llvm::SmallVector&lt;clang::Parser::InnerNamespaceInfo, 4u&gt; const&amp;, unsigned int, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::BalancedDelimiterTracker&amp;) (c:\msys64\clang64\bin\libclang-cpp.dll+0x5ddd01)
#<!-- -->39 0x00007ffedfffc745 clang::Parser::ParseNamespace(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::SourceLocation) (c:\msys64\clang64\bin\libclang-cpp.dll+0x5dc745)
#<!-- -->40 0x00007ffedffff24a clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (c:\msys64\clang64\bin\libclang-cpp.dll+0x5df24a)
#<!-- -->41 0x00007ffedfffe0f4 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (c:\msys64\clang64\bin\libclang-cpp.dll+0x5de0f4)
#<!-- -->42 0x00007ffedfffdd01 clang::Parser::ParseInnerNamespace(llvm::SmallVector&lt;clang::Parser::InnerNamespaceInfo, 4u&gt; const&amp;, unsigned int, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::BalancedDelimiterTracker&amp;) (c:\msys64\clang64\bin\libclang-cpp.dll+0x5ddd01)
#<!-- -->43 0x00007ffedfffc745 clang::Parser::ParseNamespace(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::SourceLocation) (c:\msys64\clang64\bin\libclang-cpp.dll+0x5dc745)
#<!-- -->44 0x00007ffedffff24a clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (c:\msys64\clang64\bin\libclang-cpp.dll+0x5df24a)
#<!-- -->45 0x00007ffedfffe0f4 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (c:\msys64\clang64\bin\libclang-cpp.dll+0x5de0f4)
#<!-- -->46 0x00007ffee0238d31 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (c:\msys64\clang64\bin\libclang-cpp.dll+0x818d31)
#<!-- -->47 0x00007ffee02389f4 clang::ParseAST(clang::Sema&amp;, bool, bool) (c:\msys64\clang64\bin\libclang-cpp.dll+0x8189f4)
#<!-- -->48 0x00007ffee026170f clang::FrontendAction::Execute() (c:\msys64\clang64\bin\libclang-cpp.dll+0x84170f)
#<!-- -->49 0x00007ffee026127b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (c:\msys64\clang64\bin\libclang-cpp.dll+0x84127b)
#<!-- -->50 0x00007ffee0260d02 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (c:\msys64\clang64\bin\libclang-cpp.dll+0x840d02)
#<!-- -->51 0x00007ff7ad60442d cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (c:\msys64\clang64\bin\clang++.exe+0x442d)
#<!-- -->52 0x00007ff7ad603c06 llvm::raw_ostream::operator&lt;&lt;(llvm::StringRef) (c:\msys64\clang64\bin\clang++.exe+0x3c06)
#<!-- -->53 0x00007ffee15b463d void std::__1::vector&lt;std::__1::optional&lt;std::__1::basic_string&lt;char, std::__1::char_traits&lt;char&gt;, std::__1::allocator&lt;char&gt;&gt;&gt;, std::__1::allocator&lt;std::__1::optional&lt;std::__1::basic_string&lt;char, std::__1::char_traits&lt;char&gt;, std::__1::allocator&lt;char&gt;&gt;&gt;&gt;&gt;::__assign_with_size[abi:nn210101]&lt;std::__1::optional&lt;std::__1::basic_string&lt;char, std::__1::char_traits&lt;char&gt;, std::__1::allocator&lt;char&gt;&gt;&gt;*, std::__1::optional&lt;std::__1::basic_string&lt;char, std::__1::char_traits&lt;char&gt;, std::__1::allocator&lt;char&gt;&gt;&gt;*&gt;(std::__1::optional&lt;std::__1::basic_string&lt;char, std::__1::char_traits&lt;char&gt;, std::__1::allocator&lt;char&gt;&gt;&gt;*, std::__1::optional&lt;std::__1::basic_string&lt;char, std::__1::char_traits&lt;char&gt;, std::__1::allocator&lt;char&gt;&gt;&gt;*, long long) (c:\msys64\clang64\bin\libclang-cpp.dll+0x1b9463d)
#<!-- -->54 0x00007ffee3b1c6c5 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (c:\msys64\clang64\bin\libLLVM-21.dll+0xf1c6c5)
#<!-- -->55 0x00007ffee0385a57 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::__1::optional&lt;llvm::StringRef&gt;&gt;, std::__1::basic_string&lt;char, std::__1::char_traits&lt;char&gt;, std::__1::allocator&lt;char&gt;&gt;*, bool*) const (c:\msys64\clang64\bin\libclang-cpp.dll+0x965a57)
#<!-- -->56 0x00007ffee033f1a3 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (c:\msys64\clang64\bin\libclang-cpp.dll+0x91f1a3)
#<!-- -->57 0x00007ffee033ee20 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::__1::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (c:\msys64\clang64\bin\libclang-cpp.dll+0x91ee20)
#<!-- -->58 0x00007ffee033eb4b clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::__1::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (c:\msys64\clang64\bin\libclang-cpp.dll+0x91eb4b)
#<!-- -->59 0x00007ff7ad601bfc clang_main(int, char**, llvm::ToolContext const&amp;) (c:\msys64\clang64\bin\clang++.exe+0x1bfc)
#<!-- -->60 0x00007ff7ad601075 main (c:\msys64\clang64\bin\clang++.exe+0x1075)
#<!-- -->61 0x00007ff7ad60a96e WinMainCRTStartup (c:\msys64\clang64\bin\clang++.exe+0xa96e)
#<!-- -->62 0x00007ff7ad60ac26 mainCRTStartup (c:\msys64\clang64\bin\clang++.exe+0xac26)
#<!-- -->63 0x00007fffc47b7374 (C:\Windows\System32\KERNEL32.DLL+0x17374)
#<!-- -->64 0x00007fffc621cc91 (C:\Windows\SYSTEM32\ntdll.dll+0x4cc91)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 21.1.1
Target: x86_64-w64-windows-gnu
Thread model: posix
InstalledDir: C:/msys64/clang64/bin
clang++: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: d:/temp/Shader-7c45b0.cpp
clang++: note: diagnostic msg: d:/temp/Shader-7c45b0.sh
clang++: note: diagnostic msg: 

********************

[Shader.cpp.txt](https://github.com/user-attachments/files/22859060/Shader.cpp.txt)
[Shader.sh.txt](https://github.com/user-attachments/files/22859058/Shader.sh.txt)

```
</details>


---

### Comment 3 - sthalik

Here's the same crash, reduced, but with a different source file:

reduced file: [foo.i.txt](https://github.com/user-attachments/files/23594601/foo.i.txt)

```
clang version 21.1.5
Target: x86_64-w64-windows-gnu
Thread model: posix
InstalledDir: C:/msys64/clang64/bin
```
```
clang++.exe -fsyntax-only -xc++ -std=c++20 -emit-llvm -c foo.i -o foo.bc
```
```console
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: C:\\msys64\\clang64\\bin\\clang++.exe -fsyntax-only -xc++ -std=c++20 -emit-llvm foo.i
1.      foo.i:1991:69: current parser token ';'
2.      foo.i:1948:1: parsing namespace 'Corrade'
3.      foo.i:1984:1: parsing namespace 'Corrade::PluginManager'
4.      foo.i:1990:1: parsing function body 'Corrade::PluginManager::PluginMetadata::depends'
5.      foo.i:1990:1: in compound statement ('{}')
Exception Code: 0xC000001D
 #0 0x00007ffff7115533 clang::ConversionFixItGenerator::tryToFixConversion(clang::Expr const*, clang::QualType, clang::QualType, clang::Sema&) (C:\msys64\clang64\bin\libclang-cpp.dll+0x855533)
 #1 0x00007ffff7114c6e clang::OverloadCandidateSet::CompleteCandidates(clang::Sema&, clang::OverloadCandidateDisplayKind, llvm::ArrayRef<clang::Expr*>, clang::SourceLocation, llvm::function_ref<bool (clang::OverloadCandidate&)>) (C:\msys64\clang64\bin\libclang-cpp.dll+0x854c6e)
 #2 0x00007ffff711488d clang::OverloadCandidateSet::CompleteCandidates(clang::Sema&, clang::OverloadCandidateDisplayKind, llvm::ArrayRef<clang::Expr*>, clang::SourceLocation, llvm::function_ref<bool (clang::OverloadCandidate&)>) (C:\msys64\clang64\bin\libclang-cpp.dll+0x85488d)
 #3 0x00007ffff71143bf clang::OverloadCandidateSet::CompleteCandidates(clang::Sema&, clang::OverloadCandidateDisplayKind, llvm::ArrayRef<clang::Expr*>, clang::SourceLocation, llvm::function_ref<bool (clang::OverloadCandidate&)>) (C:\msys64\clang64\bin\libclang-cpp.dll+0x8543bf)
 #4 0x00007ffff7054bed clang::OverloadCandidateSet::NoteCandidates(std::__1::pair<clang::SourceLocation, clang::PartialDiagnostic>, clang::Sema&, clang::OverloadCandidateDisplayKind, llvm::ArrayRef<clang::Expr*>, llvm::StringRef, clang::SourceLocation, llvm::function_ref<bool (clang::OverloadCandidate&)>) (C:\msys64\clang64\bin\libclang-cpp.dll+0x794bed)
 #5 0x00007ffff722a8ba clang::Parser::isDeclarationStatement(bool) (C:\msys64\clang64\bin\libclang-cpp.dll+0x96a8ba)
 #6 0x00007ffff69d6dfb clang::Sema::BuildOverloadedCallExpr(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (C:\msys64\clang64\bin\libclang-cpp.dll+0x116dfb)
 #7 0x00007ffff69641a6 clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) (C:\msys64\clang64\bin\libclang-cpp.dll+0xa41a6)
 #8 0x00007ffff6b07876 clang::CXXConversionDecl::Create(clang::ASTContext&, clang::CXXRecordDecl*, clang::SourceLocation, clang::DeclarationNameInfo const&, clang::QualType, clang::TypeSourceInfo*, bool, bool, clang::ExplicitSpecifier, clang::ConstexprSpecKind, clang::SourceLocation, clang::AssociatedConstraint const&) (C:\msys64\clang64\bin\libclang-cpp.dll+0x247876)
 #9 0x00007ffff6a6a243 clang::VarDecl::getInstantiatedFromStaticDataMember() const (C:\msys64\clang64\bin\libclang-cpp.dll+0x1aa243)
#10 0x00007ffff6a9fb19 clang::Sema::SubstTemplateArgument(clang::TemplateArgumentLoc const&, clang::MultiLevelTemplateArgumentList const&, clang::TemplateArgumentLoc&, clang::SourceLocation, clang::DeclarationName const&) (C:\msys64\clang64\bin\libclang-cpp.dll+0x1dfb19)
#11 0x00007ffff6da0d5d clang::Sema::SubstDefaultTemplateArgumentIfAvailable(clang::TemplateDecl*, clang::SourceLocation, clang::SourceLocation, clang::Decl*, llvm::ArrayRef<clang::TemplateArgument>, llvm::ArrayRef<clang::TemplateArgument>, bool&) (C:\msys64\clang64\bin\libclang-cpp.dll+0x4e0d5d)
#12 0x00007ffff6d9bb72 clang::DynamicRecursiveASTVisitorBase<false>::TraverseTemplateName(clang::TemplateName) (C:\msys64\clang64\bin\libclang-cpp.dll+0x4dbb72)
#13 0x00007ffff6cbb5b1 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (C:\msys64\clang64\bin\libclang-cpp.dll+0x3fb5b1)
#14 0x00007ffff6cbb551 clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (C:\msys64\clang64\bin\libclang-cpp.dll+0x3fb551)
#15 0x00007ffff6bb17e7 clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, bool, bool, clang::QualType, clang::Expr::Classification, bool, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>, bool)>) (C:\msys64\clang64\bin\libclang-cpp.dll+0x2f17e7)
#16 0x00007ffff6ba8abd clang::OverloadCandidateSet::BestViableFunction(clang::Sema&, clang::SourceLocation, clang::OverloadCandidate*&) (C:\msys64\clang64\bin\libclang-cpp.dll+0x2e8abd)
#17 0x00007ffff6ffeed0 clang::Sema::DeclareImplicitMoveAssignment(clang::CXXRecordDecl*) (C:\msys64\clang64\bin\libclang-cpp.dll+0x73eed0)
#18 0x00007ffff6fa042d clang::Sema::BuildDeclarationNameExpr(clang::CXXScopeSpec const&, clang::DeclarationNameInfo const&, clang::NamedDecl*, clang::NamedDecl*, clang::TemplateArgumentListInfo const*, bool) (C:\msys64\clang64\bin\libclang-cpp.dll+0x6e042d)
#19 0x00007ffff6e4cf36 clang::InitializationSequence::InitializationSequence(clang::Sema&, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::MutableArrayRef<clang::Expr*>, bool, bool) (C:\msys64\clang64\bin\libclang-cpp.dll+0x58cf36)
#20 0x00007ffff6a365c4 clang::Sema::BuildCXXTypeConstructExpr(clang::TypeSourceInfo*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, bool) (C:\msys64\clang64\bin\libclang-cpp.dll+0x1765c4)
#21 0x00007ffff6a8a741 clang::computeDependence(clang::DependentScopeDeclRefExpr*) (C:\msys64\clang64\bin\libclang-cpp.dll+0x1ca741)
#22 0x00007ffff6a6a243 clang::VarDecl::getInstantiatedFromStaticDataMember() const (C:\msys64\clang64\bin\libclang-cpp.dll+0x1aa243)
#23 0x00007ffff6a9fb19 clang::Sema::SubstTemplateArgument(clang::TemplateArgumentLoc const&, clang::MultiLevelTemplateArgumentList const&, clang::TemplateArgumentLoc&, clang::SourceLocation, clang::DeclarationName const&) (C:\msys64\clang64\bin\libclang-cpp.dll+0x1dfb19)
#24 0x00007ffff6da0d5d clang::Sema::SubstDefaultTemplateArgumentIfAvailable(clang::TemplateDecl*, clang::SourceLocation, clang::SourceLocation, clang::Decl*, llvm::ArrayRef<clang::TemplateArgument>, llvm::ArrayRef<clang::TemplateArgument>, bool&) (C:\msys64\clang64\bin\libclang-cpp.dll+0x4e0d5d)
#25 0x00007ffff6d9bb72 clang::DynamicRecursiveASTVisitorBase<false>::TraverseTemplateName(clang::TemplateName) (C:\msys64\clang64\bin\libclang-cpp.dll+0x4dbb72)
#26 0x00007ffff6cbb5b1 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (C:\msys64\clang64\bin\libclang-cpp.dll+0x3fb5b1)
#27 0x00007ffff6cbb551 clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (C:\msys64\clang64\bin\libclang-cpp.dll+0x3fb551)
#28 0x00007ffff6bb17e7 clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, bool, bool, clang::QualType, clang::Expr::Classification, bool, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>, bool)>) (C:\msys64\clang64\bin\libclang-cpp.dll+0x2f17e7)
#29 0x00007ffff69cfbe0 clang::Sema::AddTemplateOverloadCandidate(clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::OverloadCandidateSet&, bool, bool, bool, clang::CallExpr::ADLCallKind, clang::OverloadCandidateParamOrder, bool) (C:\msys64\clang64\bin\libclang-cpp.dll+0x10fbe0)
#30 0x00007ffff6fb5247 clang::computeDependence(clang::ImplicitValueInitExpr*) (C:\msys64\clang64\bin\libclang-cpp.dll+0x6f5247)
#31 0x00007ffff69bb4b7 clang::Sema::PerformCopyInitialization(clang::InitializedEntity const&, clang::SourceLocation, clang::ActionResult<clang::Expr*, true>, bool, bool) (C:\msys64\clang64\bin\libclang-cpp.dll+0xfb4b7)
#32 0x00007ffff699f566 clang::Sema::ActOnReturnStmt(clang::SourceLocation, clang::Expr*, clang::Scope*) (C:\msys64\clang64\bin\libclang-cpp.dll+0xdf566)
#33 0x00007ffff69542e9 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (C:\msys64\clang64\bin\libclang-cpp.dll+0x942e9)
#34 0x00007ffff6eaf798 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (C:\msys64\clang64\bin\libclang-cpp.dll+0x5ef798)
#35 0x00007ffff6eab7f0 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (C:\msys64\clang64\bin\libclang-cpp.dll+0x5eb7f0)
#36 0x00007ffff6ea9640 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (C:\msys64\clang64\bin\libclang-cpp.dll+0x5e9640)
#37 0x00007ffff6ea7675 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (C:\msys64\clang64\bin\libclang-cpp.dll+0x5e7675)
#38 0x00007ffff6ea7171 clang::Parser::ParseInnerNamespace(llvm::SmallVector<clang::Parser::InnerNamespaceInfo, 4u> const&, unsigned int, clang::SourceLocation&, clang::ParsedAttributes&, clang::BalancedDelimiterTracker&) (C:\msys64\clang64\bin\libclang-cpp.dll+0x5e7171)
#39 0x00007ffff6ea5bb5 clang::Parser::ParseNamespace(clang::DeclaratorContext, clang::SourceLocation&, clang::SourceLocation) (C:\msys64\clang64\bin\libclang-cpp.dll+0x5e5bb5)
#40 0x00007ffff6ea86ba clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (C:\msys64\clang64\bin\libclang-cpp.dll+0x5e86ba)
#41 0x00007ffff6ea7564 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (C:\msys64\clang64\bin\libclang-cpp.dll+0x5e7564)
#42 0x00007ffff6ea7171 clang::Parser::ParseInnerNamespace(llvm::SmallVector<clang::Parser::InnerNamespaceInfo, 4u> const&, unsigned int, clang::SourceLocation&, clang::ParsedAttributes&, clang::BalancedDelimiterTracker&) (C:\msys64\clang64\bin\libclang-cpp.dll+0x5e7171)
#43 0x00007ffff6ea5bb5 clang::Parser::ParseNamespace(clang::DeclaratorContext, clang::SourceLocation&, clang::SourceLocation) (C:\msys64\clang64\bin\libclang-cpp.dll+0x5e5bb5)
#44 0x00007ffff6ea86ba clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (C:\msys64\clang64\bin\libclang-cpp.dll+0x5e86ba)
#45 0x00007ffff6ea7564 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (C:\msys64\clang64\bin\libclang-cpp.dll+0x5e7564)
#46 0x00007ffff70f1191 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (C:\msys64\clang64\bin\libclang-cpp.dll+0x831191)
#47 0x00007ffff70f0e54 clang::ParseAST(clang::Sema&, bool, bool) (C:\msys64\clang64\bin\libclang-cpp.dll+0x830e54)
#48 0x00007ffff7117bbf clang::FrontendAction::Execute() (C:\msys64\clang64\bin\libclang-cpp.dll+0x857bbf)
#49 0x00007ffff711772b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (C:\msys64\clang64\bin\libclang-cpp.dll+0x85772b)
#50 0x00007ffff71171b2 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (C:\msys64\clang64\bin\libclang-cpp.dll+0x8571b2)
#51 0x00007ff6ddf6442d cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (C:\msys64\clang64\bin\clang++.exe+0x442d)    
#52 0x00007ff6ddf63c06 llvm::raw_ostream::operator<<(llvm::StringRef) (C:\msys64\clang64\bin\clang++.exe+0x3c06)
#53 0x00007ffff840e17d void std::__1::vector<std::__1::optional<std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>>>, std::__1::allocator<std::__1::optional<std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>>>>>::__assign_with_size[abi:nn210104]<std::__1::optional<std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>>>*, std::__1::optional<std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>>>*>(std::__1::optional<std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>>>*, std::__1::optional<std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>>>*, long long) (C:\msys64\clang64\bin\libclang-cpp.dll+0x1b4e17d)
#54 0x00007fff7b62fc25 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (C:\msys64\clang64\bin\libLLVM-21.dll+0xf1fc25)
#55 0x00007ffff7235777 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::__1::optional<llvm::StringRef>>, std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>>*, bool*) const (C:\msys64\clang64\bin\libclang-cpp.dll+0x975777)
#56 0x00007ffff71fa163 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (C:\msys64\clang64\bin\libclang-cpp.dll+0x93a163)
#57 0x00007ffff71f9de0 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::__1::pair<int, clang::driver::Command const*>>&, bool) const (C:\msys64\clang64\bin\libclang-cpp.dll+0x939de0)
#58 0x00007ffff71f9b0b clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::__1::pair<int, clang::driver::Command const*>>&) (C:\msys64\clang64\bin\libclang-cpp.dll+0x939b0b)
#59 0x00007ff6ddf61bfc clang_main(int, char**, llvm::ToolContext const&) (C:\msys64\clang64\bin\clang++.exe+0x1bfc)
#60 0x00007ff6ddf61075 main (C:\msys64\clang64\bin\clang++.exe+0x1075)
#61 0x00007ff6ddf6a96e WinMainCRTStartup (C:\msys64\clang64\bin\clang++.exe+0xa96e)
#62 0x00007ff6ddf6ac26 mainCRTStartup (C:\msys64\clang64\bin\clang++.exe+0xac26)
#63 0x00007ff8a752e8d7 (C:\WINDOWS\System32\KERNEL32.DLL+0x2e8d7)
#64 0x00007ff8a892c53c (C:\WINDOWS\SYSTEM32\ntdll.dll+0x8c53c)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 21.1.5
Target: x86_64-w64-windows-gnu
Thread model: posix
InstalledDir: C:/msys64/clang64/bin
clang++: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: D:/temp/foo-ba34d3.cpp
clang++: note: diagnostic msg: D:/temp/foo-ba34d3.sh
clang++: note: diagnostic msg:

********************

```

---

### Comment 4 - sthalik

FWIW, here's the stack trace with all symbols present.

[foo-stacktrace.txt](https://github.com/user-attachments/files/23612158/foo-stacktrace.txt)

---

### Comment 5 - shafik

CC @AaronBallman 

---

