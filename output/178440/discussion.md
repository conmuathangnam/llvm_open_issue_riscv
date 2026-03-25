# Clang crashes while diagnosing invalid in-class member initializer

**Author:** wusikijeronii
**URL:** https://github.com/llvm/llvm-project/issues/178440

## Body

Clang crashes during diagnostic formatting when compiling invalid code (in-class member initializer with function pointer to non-existing symbol / wrong namespace).
Instead of emitting a diagnostic, clang crashes in Diagnostic::FormatDiagnostic.

Version:
clang version 21.1.8
Target: x86_64-w64-windows-gnu
Thread model: posix

Command:
clang++ -fsyntax-only context.hpp

<details>
<summary>Stackstrace</summary>

```sh
wusik@LAPTOP-4PA9J0V4:app3d clang++ -fsyntax-only  modules/acul/include/acul/detail/simd/context.hpp 
modules/acul/include/acul/detail/simd/context.hpp:13:9: error: unknown type name 'PFN_fill_line_buffer'; did you mean
      'fs::detail::PFN_fill_line_buffer'?
   13 |         PFN_fill_line_buffer fill_line_buffer = &nosimd::fill_line_buffer;
      |         ^~~~~~~~~~~~~~~~~~~~
      |         fs::detail::PFN_fill_line_buffer
modules/acul/include/acul/detail/simd/../../io/fs/detail/file_simd_fn.hpp:23:11: note: 'fs::detail::PFN_fill_line_buffer' declared here
   23 |     using PFN_fill_line_buffer = void (*)(const char *data, size_t size, string_view_pool<char> &dst);
      |           ^
modules/acul/include/acul/detail/simd/context.hpp:13:50: error: no member named 'fill_line_buffer' in namespace 'acul::detail::nosimd';
      did you mean simply 'fill_line_buffer'?
   13 |         PFN_fill_line_buffer fill_line_buffer = &nosimd::fill_line_buffer;
      |                                                  ^~~~~~~~~~~~~~~~~~~~~~~~
      |                                                  fill_line_buffer
modules/acul/include/acul/detail/simd/context.hpp:13:30: note: 'fill_line_buffer' declared here
   13 |         PFN_fill_line_buffer fill_line_buffer = &nosimd::fill_line_buffer;
      |                              ^
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: C:\\msys64\\clang64\\bin\\clang++.exe -fsyntax-only modules/acul/include/acul/detail/simd/context.hpp
1.      <eof> parser at end of file
2.      modules/acul/include/acul/detail/simd/context.hpp:7:1: parsing namespace 'acul'
3.      modules/acul/include/acul/detail/simd/context.hpp:9:23: parsing struct/union/class body 'acul::detail::simd_context'
Exception Code: 0xC0000005
 #0 0x00007ffc8e930581 clang::TemplateSpecializationType const* clang::Type::getAs<clang::TemplateSpecializationType>() const (C:\msys64\clang64\bin\libclang-cpp.dll+0x1f0581)
 #1 0x00007ffc8f2c4d7f clang::FormatUTFCodeUnitAsCodepoint(unsigned int, clang::QualType) (C:\msys64\clang64\bin\libclang-cpp.dll+0xb84d7f)
 #2 0x00007ffc8f08d0d4 clang::FormatASTNodeDiagnosticArgument(clang::DiagnosticsEngine::ArgumentKind, long long, llvm::StringRef, llvm::StringRef, llvm::ArrayRef<std::__1::pair<clang::DiagnosticsEngine::ArgumentKind, long long>>, llvm::SmallVectorImpl<char>&, void*, llvm::ArrayRef<long long>) (C:\msys64\clang64\bin\libclang-cpp.dll+0x94d0d4)
 #3 0x00007ffc8f0a8296 clang::Diagnostic::FormatDiagnostic(char const*, char const*, llvm::SmallVectorImpl<char>&) const (C:\msys64\clang64\bin\libclang-cpp.dll+0x968296)
 #4 0x00007ffc8f0a72f4 clang::Diagnostic::FormatDiagnostic(llvm::SmallVectorImpl<char>&) const (C:\msys64\clang64\bin\libclang-cpp.dll+0x9672f4)
 #5 0x00007ffc8f0a6c73 clang::TextDiagnosticPrinter::HandleDiagnostic(clang::DiagnosticsEngine::Level, clang::Diagnostic const&) (C:\msys64\clang64\bin\libclang-cpp.dll+0x966c73)
 #6 0x00007ffc8e86cee1 clang::DiagnosticsEngine::EmitDiagnostic(clang::DiagnosticBuilder const&, bool) (C:\msys64\clang64\bin\libclang-cpp.dll+0x12cee1)
 #7 0x00007ffc8e9ddbb5 clang::Sema::EmitDiagnostic(unsigned int, clang::DiagnosticBuilder const&) (C:\msys64\clang64\bin\libclang-cpp.dll+0x29dbb5)
 #8 0x00007ffc8e9d97cc clang::SemaBase::ImmediateDiagBuilder::~ImmediateDiagBuilder() (C:\msys64\clang64\bin\libclang-cpp.dll+0x2997cc) 
 #9 0x00007ffc8e9d76fc clang::SemaBase::SemaDiagnosticBuilder::~SemaDiagnosticBuilder() (C:\msys64\clang64\bin\libclang-cpp.dll+0x2976fc)
#10 0x00007ffc8f090862 clang::InitializationSequence::Diagnose(clang::Sema&, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::ArrayRef<clang::Expr*>) (C:\msys64\clang64\bin\libclang-cpp.dll+0x950862)
#11 0x00007ffc8ecc809e clang::InitializationSequence::Perform(clang::Sema&, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::MutableArrayRef<clang::Expr*>, clang::QualType*) (C:\msys64\clang64\bin\libclang-cpp.dll+0x58809e)
#12 0x00007ffc8ed1168c clang::Sema::ConvertMemberDefaultInitExpression(clang::FieldDecl*, clang::Expr*, clang::SourceLocation) (C:\msys64\clang64\bin\libclang-cpp.dll+0x5d168c)
#13 0x00007ffc8ed1148d clang::Sema::ActOnFinishCXXInClassMemberInitializer(clang::Decl*, clang::SourceLocation, clang::ActionResult<clang::Expr*, true>) (C:\msys64\clang64\bin\libclang-cpp.dll+0x5d148d)
#14 0x00007ffc8ed10fa5 clang::Parser::ParseLexedMemberInitializer(clang::Parser::LateParsedMemberInitializer&) (C:\msys64\clang64\bin\libclang-cpp.dll+0x5d0fa5)
#15 0x00007ffc8e7ccdea clang::Parser::ParseLexedMemberInitializers(clang::Parser::ParsingClass&) (C:\msys64\clang64\bin\libclang-cpp.dll+0x8cdea)
#16 0x00007ffc8e7c95d8 clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&, unsigned int, clang::Decl*) (C:\msys64\clang64\bin\libclang-cpp.dll+0x895d8)
#17 0x00007ffc8ed956be clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&) (C:\msys64\clang64\bin\libclang-cpp.dll+0x6556be)
#18 0x00007ffc8e88e503 clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (C:\msys64\clang64\bin\libclang-cpp.dll+0x14e503)
#19 0x00007ffc8ed1fa6e clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (C:\msys64\clang64\bin\libclang-cpp.dll+0x5dfa6e)
#20 0x00007ffc8ed1db45 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (C:\msys64\clang64\bin\libclang-cpp.dll+0x5ddb45)
#21 0x00007ffc8ed1d641 clang::Parser::ParseInnerNamespace(llvm::SmallVector<clang::Parser::InnerNamespaceInfo, 4u> const&, unsigned int, clang::SourceLocation&, clang::ParsedAttributes&, clang::BalancedDelimiterTracker&) (C:\msys64\clang64\bin\libclang-cpp.dll+0x5dd641)  
#22 0x00007ffc8ed1d761 clang::Parser::ParseInnerNamespace(llvm::SmallVector<clang::Parser::InnerNamespaceInfo, 4u> const&, unsigned int, clang::SourceLocation&, clang::ParsedAttributes&, clang::BalancedDelimiterTracker&) (C:\msys64\clang64\bin\libclang-cpp.dll+0x5dd761)  
#23 0x00007ffc8ed1c085 clang::Parser::ParseNamespace(clang::DeclaratorContext, clang::SourceLocation&, clang::SourceLocation) (C:\msys64\clang64\bin\libclang-cpp.dll+0x5dc085)
#24 0x00007ffc8ed1eb8a clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (C:\msys64\clang64\bin\libclang-cpp.dll+0x5deb8a)
#25 0x00007ffc8ed1da34 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (C:\msys64\clang64\bin\libclang-cpp.dll+0x5dda34)
#26 0x00007ffc8ef58451 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (C:\msys64\clang64\bin\libclang-cpp.dll+0x818451)
#27 0x00007ffc8ef58114 clang::ParseAST(clang::Sema&, bool, bool) (C:\msys64\clang64\bin\libclang-cpp.dll+0x818114)
#28 0x00007ffc8ef80e2f clang::FrontendAction::Execute() (C:\msys64\clang64\bin\libclang-cpp.dll+0x840e2f)
#29 0x00007ffc8ef8099b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (C:\msys64\clang64\bin\libclang-cpp.dll+0x84099b) 
#30 0x00007ffc8ef80422 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (C:\msys64\clang64\bin\libclang-cpp.dll+0x840422)     
#31 0x00007ff65b52442d cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (C:\msys64\clang64\bin\clang++.exe+0x442d)
#32 0x00007ff65b523c06 llvm::raw_ostream::operator<<(llvm::StringRef) (C:\msys64\clang64\bin\clang++.exe+0x3c06)
#33 0x00007ffc9027e7ed void std::__1::vector<std::__1::optional<std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>>>, std::__1::allocator<std::__1::optional<std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>>>>>::__assign_with_size[abi:nn210108]<std::__1::optional<std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>>>*, std::__1::optional<std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>>>*>(std::__1::optional<std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>>>*, std::__1::optional<std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>>>*, long long) (C:\msys64\clang64\bin\libclang-cpp.dll+0x1b3e7ed)
#34 0x00007ffbea85bf05 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (C:\msys64\clang64\bin\libLLVM-21.dll+0xf1bf05)
#35 0x00007ffc8f0a43b7 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::__1::optional<llvm::StringRef>>, std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>>*, bool*) const (C:\msys64\clang64\bin\libclang-cpp.dll+0x9643b7)
#36 0x00007ffc8f05e153 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (C:\msys64\clang64\bin\libclang-cpp.dll+0x91e153)
#37 0x00007ffc8f05ddd0 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::__1::pair<int, clang::driver::Command const*>>&, bool) const (C:\msys64\clang64\bin\libclang-cpp.dll+0x91ddd0)
#38 0x00007ffc8f05dafb clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::__1::pair<int, clang::driver::Command const*>>&) (C:\msys64\clang64\bin\libclang-cpp.dll+0x91dafb)
#39 0x00007ff65b521bfc clang_main(int, char**, llvm::ToolContext const&) (C:\msys64\clang64\bin\clang++.exe+0x1bfc)
#40 0x00007ff65b521075 main (C:\msys64\clang64\bin\clang++.exe+0x1075)
#41 0x00007ff65b52a96e WinMainCRTStartup (C:\msys64\clang64\bin\clang++.exe+0xa96e)
#42 0x00007ff65b52ac56 mainCRTStartup (C:\msys64\clang64\bin\clang++.exe+0xac56)
#43 0x00007ffd3adbe8d7 (C:\WINDOWS\System32\KERNEL32.DLL+0x2e8d7)
#44 0x00007ffd3be8c53c (C:\WINDOWS\SYSTEM32\ntdll.dll+0x8c53c)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 21.1.8
Target: x86_64-w64-windows-gnu
Thread model: posix
InstalledDir: C:/msys64/clang64/bin
clang++: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: C:/Users/wusik/AppData/Local/Temp/context-0f0fe5.hh
clang++: note: diagnostic msg: C:/Users/wusik/AppData/Local/Temp/context-0f0fe5.sh
clang++: note: diagnostic msg:

********************
```

[context-0f0fe5.sh](https://github.com/user-attachments/files/24912895/context-0f0fe5.sh)

</details>


After the crash, clang automatically generated two files:
- context-0f0fe5.sh (run script with full compiler invocation)
- context-0f0fe5.hh (preprocessed source)

I am attaching context-0f0fe5.sh, since it contains the full invocation and flags used to reproduce the issue.
The preprocessed source file (context-0f0fe5.hh) is large (~300k lines, expanded includes and system headers), so I did not attach it by default, but I can provide it if needed.
If you need any further information from my side, please let me know.



## Comments

### Comment 1 - EugeneZelenko

Could you please try 22 release candidate or `main` branch? https://godbolt.org should be helpful. Only most recent release is maintained.

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Wusiki Jeronii (wusikijeronii)

<details>
Clang crashes during diagnostic formatting when compiling invalid code (in-class member initializer with function pointer to non-existing symbol / wrong namespace).
Instead of emitting a diagnostic, clang crashes in Diagnostic::FormatDiagnostic.

Version:
clang version 21.1.8
Target: x86_64-w64-windows-gnu
Thread model: posix

Command:
clang++ -fsyntax-only context.hpp

&lt;details&gt;
&lt;summary&gt;Stackstrace&lt;/summary&gt;

```sh
wusik@<!-- -->LAPTOP-4PA9J0V4:app3d clang++ -fsyntax-only  modules/acul/include/acul/detail/simd/context.hpp 
modules/acul/include/acul/detail/simd/context.hpp:13:9: error: unknown type name 'PFN_fill_line_buffer'; did you mean
      'fs::detail::PFN_fill_line_buffer'?
   13 |         PFN_fill_line_buffer fill_line_buffer = &amp;nosimd::fill_line_buffer;
      |         ^~~~~~~~~~~~~~~~~~~~
      |         fs::detail::PFN_fill_line_buffer
modules/acul/include/acul/detail/simd/../../io/fs/detail/file_simd_fn.hpp:23:11: note: 'fs::detail::PFN_fill_line_buffer' declared here
   23 |     using PFN_fill_line_buffer = void (*)(const char *data, size_t size, string_view_pool&lt;char&gt; &amp;dst);
      |           ^
modules/acul/include/acul/detail/simd/context.hpp:13:50: error: no member named 'fill_line_buffer' in namespace 'acul::detail::nosimd';
      did you mean simply 'fill_line_buffer'?
   13 |         PFN_fill_line_buffer fill_line_buffer = &amp;nosimd::fill_line_buffer;
      |                                                  ^~~~~~~~~~~~~~~~~~~~~~~~
      |                                                  fill_line_buffer
modules/acul/include/acul/detail/simd/context.hpp:13:30: note: 'fill_line_buffer' declared here
   13 |         PFN_fill_line_buffer fill_line_buffer = &amp;nosimd::fill_line_buffer;
      |                              ^
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: C:\\msys64\\clang64\\bin\\clang++.exe -fsyntax-only modules/acul/include/acul/detail/simd/context.hpp
1.      &lt;eof&gt; parser at end of file
2.      modules/acul/include/acul/detail/simd/context.hpp:7:1: parsing namespace 'acul'
3.      modules/acul/include/acul/detail/simd/context.hpp:9:23: parsing struct/union/class body 'acul::detail::simd_context'
Exception Code: 0xC0000005
 #<!-- -->0 0x00007ffc8e930581 clang::TemplateSpecializationType const* clang::Type::getAs&lt;clang::TemplateSpecializationType&gt;() const (C:\msys64\clang64\bin\libclang-cpp.dll+0x1f0581)
 #<!-- -->1 0x00007ffc8f2c4d7f clang::FormatUTFCodeUnitAsCodepoint(unsigned int, clang::QualType) (C:\msys64\clang64\bin\libclang-cpp.dll+0xb84d7f)
 #<!-- -->2 0x00007ffc8f08d0d4 clang::FormatASTNodeDiagnosticArgument(clang::DiagnosticsEngine::ArgumentKind, long long, llvm::StringRef, llvm::StringRef, llvm::ArrayRef&lt;std::__1::pair&lt;clang::DiagnosticsEngine::ArgumentKind, long long&gt;&gt;, llvm::SmallVectorImpl&lt;char&gt;&amp;, void*, llvm::ArrayRef&lt;long long&gt;) (C:\msys64\clang64\bin\libclang-cpp.dll+0x94d0d4)
 #<!-- -->3 0x00007ffc8f0a8296 clang::Diagnostic::FormatDiagnostic(char const*, char const*, llvm::SmallVectorImpl&lt;char&gt;&amp;) const (C:\msys64\clang64\bin\libclang-cpp.dll+0x968296)
 #<!-- -->4 0x00007ffc8f0a72f4 clang::Diagnostic::FormatDiagnostic(llvm::SmallVectorImpl&lt;char&gt;&amp;) const (C:\msys64\clang64\bin\libclang-cpp.dll+0x9672f4)
 #<!-- -->5 0x00007ffc8f0a6c73 clang::TextDiagnosticPrinter::HandleDiagnostic(clang::DiagnosticsEngine::Level, clang::Diagnostic const&amp;) (C:\msys64\clang64\bin\libclang-cpp.dll+0x966c73)
 #<!-- -->6 0x00007ffc8e86cee1 clang::DiagnosticsEngine::EmitDiagnostic(clang::DiagnosticBuilder const&amp;, bool) (C:\msys64\clang64\bin\libclang-cpp.dll+0x12cee1)
 #<!-- -->7 0x00007ffc8e9ddbb5 clang::Sema::EmitDiagnostic(unsigned int, clang::DiagnosticBuilder const&amp;) (C:\msys64\clang64\bin\libclang-cpp.dll+0x29dbb5)
 #<!-- -->8 0x00007ffc8e9d97cc clang::SemaBase::ImmediateDiagBuilder::~ImmediateDiagBuilder() (C:\msys64\clang64\bin\libclang-cpp.dll+0x2997cc) 
 #<!-- -->9 0x00007ffc8e9d76fc clang::SemaBase::SemaDiagnosticBuilder::~SemaDiagnosticBuilder() (C:\msys64\clang64\bin\libclang-cpp.dll+0x2976fc)
#<!-- -->10 0x00007ffc8f090862 clang::InitializationSequence::Diagnose(clang::Sema&amp;, clang::InitializedEntity const&amp;, clang::InitializationKind const&amp;, llvm::ArrayRef&lt;clang::Expr*&gt;) (C:\msys64\clang64\bin\libclang-cpp.dll+0x950862)
#<!-- -->11 0x00007ffc8ecc809e clang::InitializationSequence::Perform(clang::Sema&amp;, clang::InitializedEntity const&amp;, clang::InitializationKind const&amp;, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::QualType*) (C:\msys64\clang64\bin\libclang-cpp.dll+0x58809e)
#<!-- -->12 0x00007ffc8ed1168c clang::Sema::ConvertMemberDefaultInitExpression(clang::FieldDecl*, clang::Expr*, clang::SourceLocation) (C:\msys64\clang64\bin\libclang-cpp.dll+0x5d168c)
#<!-- -->13 0x00007ffc8ed1148d clang::Sema::ActOnFinishCXXInClassMemberInitializer(clang::Decl*, clang::SourceLocation, clang::ActionResult&lt;clang::Expr*, true&gt;) (C:\msys64\clang64\bin\libclang-cpp.dll+0x5d148d)
#<!-- -->14 0x00007ffc8ed10fa5 clang::Parser::ParseLexedMemberInitializer(clang::Parser::LateParsedMemberInitializer&amp;) (C:\msys64\clang64\bin\libclang-cpp.dll+0x5d0fa5)
#<!-- -->15 0x00007ffc8e7ccdea clang::Parser::ParseLexedMemberInitializers(clang::Parser::ParsingClass&amp;) (C:\msys64\clang64\bin\libclang-cpp.dll+0x8cdea)
#<!-- -->16 0x00007ffc8e7c95d8 clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&amp;, unsigned int, clang::Decl*) (C:\msys64\clang64\bin\libclang-cpp.dll+0x895d8)
#<!-- -->17 0x00007ffc8ed956be clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&amp;) (C:\msys64\clang64\bin\libclang-cpp.dll+0x6556be)
#<!-- -->18 0x00007ffc8e88e503 clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (C:\msys64\clang64\bin\libclang-cpp.dll+0x14e503)
#<!-- -->19 0x00007ffc8ed1fa6e clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (C:\msys64\clang64\bin\libclang-cpp.dll+0x5dfa6e)
#<!-- -->20 0x00007ffc8ed1db45 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (C:\msys64\clang64\bin\libclang-cpp.dll+0x5ddb45)
#<!-- -->21 0x00007ffc8ed1d641 clang::Parser::ParseInnerNamespace(llvm::SmallVector&lt;clang::Parser::InnerNamespaceInfo, 4u&gt; const&amp;, unsigned int, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::BalancedDelimiterTracker&amp;) (C:\msys64\clang64\bin\libclang-cpp.dll+0x5dd641)  
#<!-- -->22 0x00007ffc8ed1d761 clang::Parser::ParseInnerNamespace(llvm::SmallVector&lt;clang::Parser::InnerNamespaceInfo, 4u&gt; const&amp;, unsigned int, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::BalancedDelimiterTracker&amp;) (C:\msys64\clang64\bin\libclang-cpp.dll+0x5dd761)  
#<!-- -->23 0x00007ffc8ed1c085 clang::Parser::ParseNamespace(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::SourceLocation) (C:\msys64\clang64\bin\libclang-cpp.dll+0x5dc085)
#<!-- -->24 0x00007ffc8ed1eb8a clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (C:\msys64\clang64\bin\libclang-cpp.dll+0x5deb8a)
#<!-- -->25 0x00007ffc8ed1da34 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (C:\msys64\clang64\bin\libclang-cpp.dll+0x5dda34)
#<!-- -->26 0x00007ffc8ef58451 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (C:\msys64\clang64\bin\libclang-cpp.dll+0x818451)
#<!-- -->27 0x00007ffc8ef58114 clang::ParseAST(clang::Sema&amp;, bool, bool) (C:\msys64\clang64\bin\libclang-cpp.dll+0x818114)
#<!-- -->28 0x00007ffc8ef80e2f clang::FrontendAction::Execute() (C:\msys64\clang64\bin\libclang-cpp.dll+0x840e2f)
#<!-- -->29 0x00007ffc8ef8099b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (C:\msys64\clang64\bin\libclang-cpp.dll+0x84099b) 
#<!-- -->30 0x00007ffc8ef80422 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (C:\msys64\clang64\bin\libclang-cpp.dll+0x840422)     
#<!-- -->31 0x00007ff65b52442d cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (C:\msys64\clang64\bin\clang++.exe+0x442d)
#<!-- -->32 0x00007ff65b523c06 llvm::raw_ostream::operator&lt;&lt;(llvm::StringRef) (C:\msys64\clang64\bin\clang++.exe+0x3c06)
#<!-- -->33 0x00007ffc9027e7ed void std::__1::vector&lt;std::__1::optional&lt;std::__1::basic_string&lt;char, std::__1::char_traits&lt;char&gt;, std::__1::allocator&lt;char&gt;&gt;&gt;, std::__1::allocator&lt;std::__1::optional&lt;std::__1::basic_string&lt;char, std::__1::char_traits&lt;char&gt;, std::__1::allocator&lt;char&gt;&gt;&gt;&gt;&gt;::__assign_with_size[abi:nn210108]&lt;std::__1::optional&lt;std::__1::basic_string&lt;char, std::__1::char_traits&lt;char&gt;, std::__1::allocator&lt;char&gt;&gt;&gt;*, std::__1::optional&lt;std::__1::basic_string&lt;char, std::__1::char_traits&lt;char&gt;, std::__1::allocator&lt;char&gt;&gt;&gt;*&gt;(std::__1::optional&lt;std::__1::basic_string&lt;char, std::__1::char_traits&lt;char&gt;, std::__1::allocator&lt;char&gt;&gt;&gt;*, std::__1::optional&lt;std::__1::basic_string&lt;char, std::__1::char_traits&lt;char&gt;, std::__1::allocator&lt;char&gt;&gt;&gt;*, long long) (C:\msys64\clang64\bin\libclang-cpp.dll+0x1b3e7ed)
#<!-- -->34 0x00007ffbea85bf05 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (C:\msys64\clang64\bin\libLLVM-21.dll+0xf1bf05)
#<!-- -->35 0x00007ffc8f0a43b7 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::__1::optional&lt;llvm::StringRef&gt;&gt;, std::__1::basic_string&lt;char, std::__1::char_traits&lt;char&gt;, std::__1::allocator&lt;char&gt;&gt;*, bool*) const (C:\msys64\clang64\bin\libclang-cpp.dll+0x9643b7)
#<!-- -->36 0x00007ffc8f05e153 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (C:\msys64\clang64\bin\libclang-cpp.dll+0x91e153)
#<!-- -->37 0x00007ffc8f05ddd0 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::__1::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (C:\msys64\clang64\bin\libclang-cpp.dll+0x91ddd0)
#<!-- -->38 0x00007ffc8f05dafb clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::__1::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (C:\msys64\clang64\bin\libclang-cpp.dll+0x91dafb)
#<!-- -->39 0x00007ff65b521bfc clang_main(int, char**, llvm::ToolContext const&amp;) (C:\msys64\clang64\bin\clang++.exe+0x1bfc)
#<!-- -->40 0x00007ff65b521075 main (C:\msys64\clang64\bin\clang++.exe+0x1075)
#<!-- -->41 0x00007ff65b52a96e WinMainCRTStartup (C:\msys64\clang64\bin\clang++.exe+0xa96e)
#<!-- -->42 0x00007ff65b52ac56 mainCRTStartup (C:\msys64\clang64\bin\clang++.exe+0xac56)
#<!-- -->43 0x00007ffd3adbe8d7 (C:\WINDOWS\System32\KERNEL32.DLL+0x2e8d7)
#<!-- -->44 0x00007ffd3be8c53c (C:\WINDOWS\SYSTEM32\ntdll.dll+0x8c53c)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 21.1.8
Target: x86_64-w64-windows-gnu
Thread model: posix
InstalledDir: C:/msys64/clang64/bin
clang++: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: C:/Users/wusik/AppData/Local/Temp/context-0f0fe5.hh
clang++: note: diagnostic msg: C:/Users/wusik/AppData/Local/Temp/context-0f0fe5.sh
clang++: note: diagnostic msg:

********************
```

[context-0f0fe5.sh](https://github.com/user-attachments/files/24912895/context-0f0fe5.sh)

&lt;/details&gt;


After the crash, clang automatically generated two files:
- context-0f0fe5.sh (run script with full compiler invocation)
- context-0f0fe5.hh (preprocessed source)

I am attaching context-0f0fe5.sh, since it contains the full invocation and flags used to reproduce the issue.
The preprocessed source file (context-0f0fe5.hh) is large (~300k lines, expanded includes and system headers), so I did not attach it by default, but I can provide it if needed.
If you need any further information from my side, please let me know.


</details>


---

### Comment 3 - wusikijeronii

Hello, again.
I've checked the main branch.
<details>
<summary>The new stacktrace</summary>

```sh
wusik@LAPTOP-4PA9J0V4:app3d /c/Users/wusik/Documents/code/tests/llvm-project/build/bin/clang++.exe \
  -fsyntax-only \
  --sysroot=C:/msys64/clang64 \
  -stdlib=libc++ \
  modules/acul/include/acul/detail/simd/context.hpp
modules/acul/include/acul/detail/simd/context.hpp:13:50: error: no member named 'fill_line_buffer' in namespace 'acul::detail::nosimd';
      did you mean simply 'fill_line_buffer'?
   13 |         PFN_fill_line_buffer fill_line_buffer = &nosimd::fill_line_buffer;
      |                                                  ^~~~~~~~~~~~~~~~~~~~~~~~
      |                                                  fill_line_buffer
modules/acul/include/acul/detail/simd/context.hpp:13:30: note: 'fill_line_buffer' declared here
   13 |         PFN_fill_line_buffer fill_line_buffer = &nosimd::fill_line_buffer;
      |                              ^
Assertion failed: !D1 == !D2, file C:/Users/wusik/Documents/code/tests/llvm-project/clang/lib/AST/Type.cpp, line 5431
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: C:\\Users\\wusik\\Documents\\code\\tests\\llvm-project\\build\\bin\\clang++.exe -fsyntax-only --sysroot=C:/msys64/clang64 -stdlib=libc++ modules/acul/include/acul/detail/simd/context.hpp
1.      <eof> parser at end of file
2.      modules/acul/include/acul/detail/simd/context.hpp:7:1: parsing namespace 'acul'
3.      modules/acul/include/acul/detail/simd/context.hpp:9:23: parsing struct/union/class body 'acul::detail::simd_context'
Exception Code: 0xC000001D
 #0 0x00007ff7b35c53a6 HandleAbort (C:\Users\wusik\Documents\code\tests\llvm-project\build\bin\clang++.exe+0x4253a6)
 #1 0x00007ffd39011989 (C:\WINDOWS\System32\ucrtbase.dll+0xc1989)
 #2 0x00007ffd38ff4ab1 (C:\WINDOWS\System32\ucrtbase.dll+0xa4ab1)
 #3 0x00007ffd38fb1049 (C:\WINDOWS\System32\ucrtbase.dll+0x61049)
 #4 0x00007ffd39012ae1 (C:\WINDOWS\System32\ucrtbase.dll+0xc2ae1)
 #5 0x00007ff7b3772de4 _assert D:/W/B/src/mingw-w64/mingw-w64-crt/misc/_assert.c:27:3
 #6 0x00007ff7b4246344 clang::TemplateSpecializationType const* clang::Type::getAs<clang::TemplateSpecializationType>() const (C:\Users\wusik\Documents\code\tests\llvm-project\build\bin\clang++.exe+0x10a6344)
 #7 0x00007ff7b5af23b3 (anonymous namespace)::TemplateDiff::GetTemplateSpecializationType(clang::ASTContext&, clang::QualType) (C:\Users\wusik\Documents\code\tests\llvm-project\build\bin\clang++.exe+0x29523b3)
 #8 0x00007ff7b5af05df clang::FormatASTNodeDiagnosticArgument(clang::DiagnosticsEngine::ArgumentKind, long long, llvm::StringRef, llvm::StringRef, llvm::ArrayRef<std::__1::pair<clang::DiagnosticsEngine::ArgumentKind, long long>>, llvm::SmallVectorImpl<char>&, void*, llvm::ArrayRef<long long>) (C:\Users\wusik\Documents\code\tests\llvm-project\build\bin\clang++.exe+0x29505df)
 #9 0x00007ff7b366185a clang::Diagnostic::FormatDiagnostic(char const*, char const*, llvm::SmallVectorImpl<char>&) const (C:\Users\wusik\Documents\code\tests\llvm-project\build\bin\clang++.exe+0x4c185a)
#10 0x00007ff7b3660cbb clang::Diagnostic::FormatDiagnostic(llvm::SmallVectorImpl<char>&) const (C:\Users\wusik\Documents\code\tests\llvm-project\build\bin\clang++.exe+0x4c0cbb)
#11 0x00007ff7b376c5cb clang::TextDiagnosticPrinter::HandleDiagnostic(clang::DiagnosticsEngine::Level, clang::Diagnostic const&) (C:\Users\wusik\Documents\code\tests\llvm-project\build\bin\clang++.exe+0x5cc5cb)
#12 0x00007ff7b366094a clang::DiagnosticsEngine::ProcessDiag(clang::DiagnosticBuilder const&) (C:\Users\wusik\Documents\code\tests\llvm-project\build\bin\clang++.exe+0x4c094a)
#13 0x00007ff7b4473313 clang::Sema::EmitDiagnostic(unsigned int, clang::DiagnosticBuilder const&) (C:\Users\wusik\Documents\code\tests\llvm-project\build\bin\clang++.exe+0x12d3313)
#14 0x00007ff7b58603bb clang::SemaBase::ImmediateDiagBuilder::~ImmediateDiagBuilder() (C:\Users\wusik\Documents\code\tests\llvm-project\build\bin\clang++.exe+0x26c03bb)
#15 0x00007ff7b4473eac clang::SemaBase::SemaDiagnosticBuilder::~SemaDiagnosticBuilder() (C:\Users\wusik\Documents\code\tests\llvm-project\build\bin\clang++.exe+0x12d3eac)
#16 0x00007ff7b5b3046a clang::InitializationSequence::Diagnose(clang::Sema&, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::ArrayRef<clang::Expr*>) (C:\Users\wusik\Documents\code\tests\llvm-project\build\bin\clang++.exe+0x299046a)
#17 0x00007ff7b5b2b245 clang::InitializationSequence::Perform(clang::Sema&, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::MutableArrayRef<clang::Expr*>, clang::QualType*) (C:\Users\wusik\Documents\code\tests\llvm-project\build\bin\clang++.exe+0x298b245)
#18 0x00007ff7b5bbfd57 clang::Sema::ConvertMemberDefaultInitExpression(clang::FieldDecl*, clang::Expr*, clang::SourceLocation) (C:\Users\wusik\Documents\code\tests\llvm-project\build\bin\clang++.exe+0x2a1fd57)
#19 0x00007ff7b5bbfea6 clang::Sema::ActOnFinishCXXInClassMemberInitializer(clang::Decl*, clang::SourceLocation, clang::ActionResult<clang::Expr*, true>) (C:\Users\wusik\Documents\code\tests\llvm-project\build\bin\clang++.exe+0x2a1fea6)
#20 0x00007ff7b7a9c3d8 clang::Parser::ParseLexedMemberInitializer(clang::Parser::LateParsedMemberInitializer&) (C:\Users\wusik\Documents\code\tests\llvm-project\build\bin\clang++.exe+0x48fc3d8)
#21 0x00007ff7b7a9a40a clang::Parser::ParseLexedMemberInitializers(clang::Parser::ParsingClass&) (C:\Users\wusik\Documents\code\tests\llvm-project\build\bin\clang++.exe+0x48fa40a)
#22 0x00007ff7b79ecbab clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&, unsigned int, clang::Decl*) (C:\Users\wusik\Documents\code\tests\llvm-project\build\bin\clang++.exe+0x484cbab)
#23 0x00007ff7b79e9f27 clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&) (C:\Users\wusik\Documents\code\tests\llvm-project\build\bin\clang++.exe+0x4849f27)
#24 0x00007ff7b7a19720 clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (C:\Users\wusik\Documents\code\tests\llvm-project\build\bin\clang++.exe+0x4879720)
#25 0x00007ff7b74ffc58 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (C:\Users\wusik\Documents\code\tests\llvm-project\build\bin\clang++.exe+0x435fc58)
#26 0x00007ff7b74ff875 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (C:\Users\wusik\Documents\code\tests\llvm-project\build\bin\clang++.exe+0x435f875)
#27 0x00007ff7b74fe6bf clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (C:\Users\wusik\Documents\code\tests\llvm-project\build\bin\clang++.exe+0x435e6bf)
#28 0x00007ff7b79de43e clang::Parser::ParseInnerNamespace(llvm::SmallVector<clang::Parser::InnerNamespaceInfo, 4u> const&, unsigned int, clang::SourceLocation&, clang::ParsedAttributes&, clang::BalancedDelimiterTracker&) (C:\Users\wusik\Documents\code\tests\llvm-project\build\bin\clang++.exe+0x483e43e)
#29 0x00007ff7b79de562 clang::Parser::ParseInnerNamespace(llvm::SmallVector<clang::Parser::InnerNamespaceInfo, 4u> const&, unsigned int, clang::SourceLocation&, clang::ParsedAttributes&, clang::BalancedDelimiterTracker&) (C:\Users\wusik\Documents\code\tests\llvm-project\build\bin\clang++.exe+0x483e562)
#30 0x00007ff7b79dd63a clang::Parser::ParseNamespace(clang::DeclaratorContext, clang::SourceLocation&, clang::SourceLocation) (C:\Users\wusik\Documents\code\tests\llvm-project\build\bin\clang++.exe+0x483d63a)
#31 0x00007ff7b7a130bd clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (C:\Users\wusik\Documents\code\tests\llvm-project\build\bin\clang++.exe+0x48730bd)
#32 0x00007ff7b74fe107 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (C:\Users\wusik\Documents\code\tests\llvm-project\build\bin\clang++.exe+0x435e107)
#33 0x00007ff7b74fc105 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (C:\Users\wusik\Documents\code\tests\llvm-project\build\bin\clang++.exe+0x435c105)
#34 0x00007ff7b6019b0e clang::ParseAST(clang::Sema&, bool, bool) (C:\Users\wusik\Documents\code\tests\llvm-project\build\bin\clang++.exe+0x2e79b0e)
#35 0x00007ff7b45a5af4 clang::FrontendAction::Execute() (C:\Users\wusik\Documents\code\tests\llvm-project\build\bin\clang++.exe+0x1405af4)
#36 0x00007ff7b36e450d clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (C:\Users\wusik\Documents\code\tests\llvm-project\build\bin\clang++.exe+0x54450d)
#37 0x00007ff7b376efa6 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (C:\Users\wusik\Documents\code\tests\llvm-project\build\bin\clang++.exe+0x5cefa6)
#38 0x00007ff7b31a6bb5 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (C:\Users\wusik\Documents\code\tests\llvm-project\build\bin\clang++.exe+0x6bb5)
#39 0x00007ff7b31a38b5 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) (C:\Users\wusik\Documents\code\tests\llvm-project\build\bin\clang++.exe+0x38b5)
#40 0x00007ff7b31a592d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::$_0>(long long, llvm::SmallVectorImpl<char const*>&) (C:\Users\wusik\Documents\code\tests\llvm-project\build\bin\clang++.exe+0x592d)
#41 0x00007ff7b42d517d void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::__1::optional<llvm::StringRef>>, std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>>*, bool*) const::$_0>(long long) (C:\Users\wusik\Documents\code\tests\llvm-project\build\bin\clang++.exe+0x113517d)
#42 0x00007ff7b35a562f llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (C:\Users\wusik\Documents\code\tests\llvm-project\build\bin\clang++.exe+0x40562f)
#43 0x00007ff7b42d4801 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::__1::optional<llvm::StringRef>>, std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>>*, bool*) const (C:\Users\wusik\Documents\code\tests\llvm-project\build\bin\clang++.exe+0x1134801)
#44 0x00007ff7b36930c1 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (C:\Users\wusik\Documents\code\tests\llvm-project\build\bin\clang++.exe+0x4f30c1)
#45 0x00007ff7b369330d clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::__1::pair<int, clang::driver::Command const*>>&, bool) const (C:\Users\wusik\Documents\code\tests\llvm-project\build\bin\clang++.exe+0x4f330d)
#46 0x00007ff7b36b2439 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::__1::pair<int, clang::driver::Command const*>>&) (C:\Users\wusik\Documents\code\tests\llvm-project\build\bin\clang++.exe+0x512439)
#47 0x00007ff7b31a2f7f clang_main(int, char**, llvm::ToolContext const&) (C:\Users\wusik\Documents\code\tests\llvm-project\build\bin\clang++.exe+0x2f7f)
#48 0x00007ff7b31b218d main (C:\Users\wusik\Documents\code\tests\llvm-project\build\bin\clang++.exe+0x1218d)
#49 0x00007ff7b31a10fe __tmainCRTStartup D:/W/B/src/mingw-w64/mingw-w64-crt/crt/crtexe.c:261:10
#50 0x00007ff7b31a13e6 .l_start D:/W/B/src/mingw-w64/mingw-w64-crt/crt/crtexe.c:131:3
#51 0x00007ffd3adbe8d7 (C:\WINDOWS\System32\KERNEL32.DLL+0x2e8d7)
#52 0x00007ffd3be8c53c (C:\WINDOWS\SYSTEM32\ntdll.dll+0x8c53c)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 23.0.0git (https://github.com/llvm/llvm-project.git 562c0b002c155098cf95e08b6f3e85b656f5eee9)
Target: x86_64-w64-windows-gnu
Thread model: posix
InstalledDir: C:/Users/wusik/Documents/code/tests/llvm-project/build/bin
Build config: +assertions
clang++: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: C:/Users/wusik/AppData/Local/Temp/context-110692.hh
clang++: note: diagnostic msg: C:/Users/wusik/AppData/Local/Temp/context-110692.sh
clang++: note: diagnostic msg:

********************
```

</details>


---

### Comment 4 - wusikijeronii

I think I've identified what's causing the crash. The issue only occurs when cstdint and "api.hpp" are included simultaneously. If either of them is omitted, the crash does not occur. Here is the content of the api.hpp file:
```cpp
#ifndef APPLIB_API_H
#define APPLIB_API_H

#ifdef _WIN32
    #define API_EXPORT __declspec(dllexport)
    #define API_IMPORT __declspec(dllimport)
#else
    #define API_EXPORT __attribute__((visibility("default")))
    #define API_IMPORT __attribute__((visibility("default")))
#endif // _WIN32

#ifdef APPLIB_BUILD
    #define APPLIB_API API_EXPORT
#else
    #define APPLIB_API API_IMPORT
#endif

#endif // APPLIB_API_H
```
Additional observation:

I initially suspected that the crash is caused by including <cstdint> together with my api.hpp, because the crash only occurred when both headers were present. If either cstdint or api.hpp was omitted, the crash did not reproduce in some tests.
However, after further testing, this does not seem to be the root cause. The crash still reproduces when api.hpp is included together with other project headers, and it does not reproduce in a minimal standalone example with only cstdint + api.hpp.
So api.hpp itself does not appear to be the direct cause.
One important detail: in the generated preprocessed source (~300k lines), the vast majority of the content comes from expanded standard headers, primarily <cstdint> and its dependencies. My actual project code is only a very small fraction of that file.
Therefore, the issue is likely triggered by a specific interaction between the invalid code (in-class member initializer / wrong namespace) and the diagnostic formatting logic, with cstdint just being part of the expanded context, not the root cause.


---

