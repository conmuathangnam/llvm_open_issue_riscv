# [clang][modules] Assertion failed: (D && "missing definition for pattern of instantiated definition"), function hasAcceptableDefinition, file SemaType.cpp, line 9203.

**Author:** zixu-w
**URL:** https://github.com/llvm/llvm-project/issues/126973
**Status:** Closed
**Labels:** clang:frontend, clang:modules, crash-on-valid
**Closed Date:** 2025-03-16T10:29:16Z

## Body

Update to include the reproducing steps here fore better visibility:
- Get Xcode: https://developer.apple.com/xcode/
- Configure the build with `cmake -G Ninja -DCLANG_ENABLE_BOOTSTRAP=On -DBOOTSTRAP_LLVM_ENABLE_MODULES=On -DCMAKE_BUILD_TYPE=RelWithDebInfo -DLLVM_ENABLE_ASSERTIONS=On -DLLVM_ENABLE_PROJECTS="clang" -DCLANG_BOOTSTRAP_TARGETS="LLVMSupport" ../llvm`
- Build `ninja -v stage2-LLVMSupport`
- Stage 2 build crashes with the above assertion when building StringSaver.cpp

------

Crash with assertion failure when building `StringSaver.cpp` during bootstrap build, with modules enabled on macOS

```
❯ /Volumes/External2/repos/upstream-llvm-project/build-clang-bootstrap/./bin/clang++ -D_DEBUG -D_GLIBCXX_ASSERTIONS -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS -I/Volumes/External2/repos/upstream-llvm-project/build-clang-bootstrap/tools/clang/stage2-bins/lib/Support -I/Volumes/External2/repos/upstream-llvm-project/llvm/lib/Support -I/Volumes/External2/repos/upstream-llvm-project/build-clang-bootstrap/tools/clang/stage2-bins/include -I/Volumes/External2/repos/upstream-llvm-project/llvm/include -fPIC -fvisibility-inlines-hidden -fdiagnostics-color  -O2 -g -DNDEBUG -std=c++17 -arch arm64 -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX15.0.sdk -UNDEBUG  -fno-exceptions -funwind-tables -fno-rtti -MD -MT lib/Support/CMakeFiles/LLVMSupport.dir/StringSaver.cpp.o -MF lib/Support/CMakeFiles/LLVMSupport.dir/StringSaver.cpp.o.d -o lib/Support/CMakeFiles/LLVMSupport.dir/StringSaver.cpp.o -c /Volumes/External2/repos/upstream-llvm-project/llvm/lib/Support/StringSaver.cpp -fmodules -fcxx-modules -Xclang -fmodules-local-submodule-visibility -gmodules

Assertion failed: (D && "missing definition for pattern of instantiated definition"), function hasAcceptableDefinition, file SemaType.cpp, line 9203.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /Volumes/External2/repos/upstream-llvm-project/build-clang-bootstrap/./bin/clang++ -D_DEBUG -D_GLIBCXX_ASSERTIONS -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS -I/Volumes/External2/repos/upstream-llvm-project/build-clang-bootstrap/tools/clang/stage2-bins/lib/Support -I/Volumes/External2/repos/upstream-llvm-project/llvm/lib/Support -I/Volumes/External2/repos/upstream-llvm-project/build-clang-bootstrap/tools/clang/stage2-bins/include -I/Volumes/External2/repos/upstream-llvm-project/llvm/include -fPIC -fvisibility-inlines-hidden -fdiagnostics-color -O2 -g -DNDEBUG -std=c++17 -arch arm64 -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX15.0.sdk -UNDEBUG -fno-exceptions -funwind-tables -fno-rtti -MD -MT lib/Support/CMakeFiles/LLVMSupport.dir/StringSaver.cpp.o -MF lib/Support/CMakeFiles/LLVMSupport.dir/StringSaver.cpp.o.d -o lib/Support/CMakeFiles/LLVMSupport.dir/StringSaver.cpp.o -c /Volumes/External2/repos/upstream-llvm-project/llvm/lib/Support/StringSaver.cpp -fmodules -fcxx-modules -Xclang -fmodules-local-submodule-visibility -gmodules
1.      /Volumes/External2/repos/upstream-llvm-project/llvm/lib/Support/StringSaver.cpp:25:37: current parser token ')'
2.      /Volumes/External2/repos/upstream-llvm-project/llvm/lib/Support/StringSaver.cpp:23:45: parsing function body 'llvm::StringSaver::save'
3.      /Volumes/External2/repos/upstream-llvm-project/llvm/lib/Support/StringSaver.cpp:23:45: in compound statement ('{}')
Stack dump without symbol names (ensure you have llvm-symbolizer in your PATH or set the environment var `LLVM_SYMBOLIZER_PATH` to point to it):
0  clang-21                 0x0000000105099ac8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) + 56
1  clang-21                 0x0000000105097fdc llvm::sys::RunSignalHandlers() + 112
2  clang-21                 0x00000001050992c0 llvm::sys::CleanupOnSignal(unsigned long) + 272
3  clang-21                 0x0000000104ffd738 CrashRecoverySignalHandler(int) + 156
4  libsystem_platform.dylib 0x0000000194e9f624 _sigtramp + 56
5  libsystem_pthread.dylib  0x0000000194e6588c pthread_kill + 296
6  libsystem_c.dylib        0x0000000194d6eb34 abort + 124
7  libsystem_c.dylib        0x0000000194d6de44 err + 0
8  clang-21                 0x0000000107936e50 clang::Sema::RequireLiteralType(clang::SourceLocation, clang::QualType, clang::Sema::TypeDiagnoser&) (.cold.11) + 0
9  clang-21                 0x0000000106cdf37c clang::Sema::hasAcceptableDefinition(clang::NamedDecl*, clang::NamedDecl**, clang::Sema::AcceptableKind, bool) + 468
10 clang-21                 0x0000000106cdec50 clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*) + 692
11 clang-21                 0x0000000106aa35f0 clang::Sema::CompareReferenceRelationship(clang::SourceLocation, clang::QualType, clang::QualType, clang::Sema::ReferenceConversionsScope::ReferenceConversions*) + 300
12 clang-21                 0x0000000106ac6cb4 FindConversionForRefInit(clang::Sema&, clang::ImplicitConversionSequence&, clang::QualType, clang::SourceLocation, clang::Expr*, clang::QualType, bool, bool) + 976
13 clang-21                 0x0000000106ac6138 TryReferenceInit(clang::Sema&, clang::Expr*, clang::QualType, clang::SourceLocation, bool, bool) + 644
14 clang-21                 0x0000000106aa843c clang::Sema::AddMethodCandidate(clang::CXXMethodDecl*, clang::DeclAccessPair, clang::CXXRecordDecl*, clang::QualType, clang::Expr::Classification, llvm::ArrayRef<clang::Expr*>, 
clang::OverloadCandidateSet&, bool, bool, llvm::MutableArrayRef<clang::ImplicitConversionSequence>, clang::OverloadCandidateParamOrder, bool) + 1396
15 clang-21                 0x0000000106abea98 clang::Sema::BuildCallToMemberFunction(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) 
+ 2152
16 clang-21                 0x0000000106816c4c clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) + 84
17 clang-21                 0x00000001065450cc clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult<clang::Expr*, true>) + 3808
18 clang-21                 0x0000000106546968 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) + 1572
19 clang-21                 0x0000000106542f5c clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) + 244
20 clang-21                 0x0000000106542e50 clang::Parser::ParseExpression(clang::Parser::TypeCastState) + 20
21 clang-21                 0x000000010659ab58 clang::Parser::ParseReturnStatement() + 324
22 clang-21                 0x00000001065958b4 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttribute
s&, clang::ParsedAttributes&) + 3772
23 clang-21                 0x00000001065947d8 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) + 324
24 clang-21                 0x000000010659c770 clang::Parser::ParseCompoundStatementBody(bool) + 1964
25 clang-21                 0x000000010659d408 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) + 184
26 clang-21                 0x00000001065b23e8 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) + 2068
27 clang-21                 0x0000000106518bb8 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Par
ser::ForRangeInit*) + 3940
28 clang-21                 0x00000001065b1940 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) + 936
29 clang-21                 0x00000001065b1338 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) + 464
30 clang-21                 0x00000001065b07ac clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) + 1580
31 clang-21                 0x00000001065af134 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) + 1384
32 clang-21                 0x0000000106505f50 clang::ParseAST(clang::Sema&, bool, bool) + 612
33 clang-21                 0x0000000105ab8ddc clang::FrontendAction::Execute() + 48
34 clang-21                 0x0000000105a57d94 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) + 900
35 clang-21                 0x0000000105b2b8d4 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) + 480
36 clang-21                 0x0000000102c59678 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) + 4272
37 clang-21                 0x0000000102c55068 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) + 948
38 clang-21                 0x00000001058ea7b8 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::__1::optional<llvm::StringRef>>, std::__1::basic_string<char, std::__1::char
_traits<char>, std::__1::allocator<char>>*, bool*) const::$_0>(long) + 32
39 clang-21                 0x0000000104ffd458 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) + 164
40 clang-21                 0x00000001058ea240 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::__1::optional<llvm::StringRef>>, std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>>*, bo
ol*) const + 228
41 clang-21                 0x00000001058bd6c8 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const + 476
42 clang-21                 0x00000001058bd890 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::__1::pair<int, clang::driver::Command const*>>&, bool) const + 120
43 clang-21                 0x00000001058d2f8c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::__1::pair<int, clang::driver::Command const*>>&) + 328
44 clang-21                 0x0000000102c545f0 clang_main(int, char**, llvm::ToolContext const&) + 4760
45 clang-21                 0x0000000102c621c0 main + 88
46 dyld                     0x0000000194ac6b4c start + 6000
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
clang version 21.0.0git (git@github.com:llvm/llvm-project.git 1f51038036b4bc216eddcc620dcb9eca5df2bfef)
Target: arm64-apple-darwin24.4.0
Thread model: posix
InstalledDir: /Volumes/External2/repos/upstream-llvm-project/build-clang-bootstrap/bin
Build config: +assertions
clang++: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: /var/folders/t_/j6y672696bsd2yhrn8rd0wwm0000gn/T/StringSaver-92ef01.cpp
clang++: note: diagnostic msg: /var/folders/t_/j6y672696bsd2yhrn8rd0wwm0000gn/T/StringSaver-92ef01.cache
clang++: note: diagnostic msg: /var/folders/t_/j6y672696bsd2yhrn8rd0wwm0000gn/T/StringSaver-92ef01.sh
clang++: note: diagnostic msg: Crash backtrace is located in
clang++: note: diagnostic msg: /Users/zixu/Library/Logs/DiagnosticReports/clang-21_<YYYY-MM-DD-HHMMSS>_<hostname>.crash
clang++: note: diagnostic msg: (choose the .crash file that corresponds to your crash)
clang++: note: diagnostic msg: 

********************
```

Bisection pinned to a9e249f64e800fbb20a3b26c0cfb68c1a1aee5e1

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-modules

Author: Zixu Wang (zixu-w)

<details>
Crash with assertion failure when building `StringSaver.cpp` during bootstrap build, with modules enabled on macOS

```
❯ /Volumes/External2/repos/upstream-llvm-project/build-clang-bootstrap/./bin/clang++ -D_DEBUG -D_GLIBCXX_ASSERTIONS -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS -I/Volumes/External2/repos/upstream-llvm-project/build-clang-bootstrap/tools/clang/stage2-bins/lib/Support -I/Volumes/External2/repos/upstream-llvm-project/llvm/lib/Support -I/Volumes/External2/repos/upstream-llvm-project/build-clang-bootstrap/tools/clang/stage2-bins/include -I/Volumes/External2/repos/upstream-llvm-project/llvm/include -fPIC -fvisibility-inlines-hidden -fdiagnostics-color  -O2 -g -DNDEBUG -std=c++17 -arch arm64 -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX15.0.sdk -UNDEBUG  -fno-exceptions -funwind-tables -fno-rtti -MD -MT lib/Support/CMakeFiles/LLVMSupport.dir/StringSaver.cpp.o -MF lib/Support/CMakeFiles/LLVMSupport.dir/StringSaver.cpp.o.d -o lib/Support/CMakeFiles/LLVMSupport.dir/StringSaver.cpp.o -c /Volumes/External2/repos/upstream-llvm-project/llvm/lib/Support/StringSaver.cpp -fmodules -fcxx-modules -Xclang -fmodules-local-submodule-visibility -gmodules

Assertion failed: (D &amp;&amp; "missing definition for pattern of instantiated definition"), function hasAcceptableDefinition, file SemaType.cpp, line 9203.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /Volumes/External2/repos/upstream-llvm-project/build-clang-bootstrap/./bin/clang++ -D_DEBUG -D_GLIBCXX_ASSERTIONS -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS -I/Volumes/External2/repos/upstream-llvm-project/build-clang-bootstrap/tools/clang/stage2-bins/lib/Support -I/Volumes/External2/repos/upstream-llvm-project/llvm/lib/Support -I/Volumes/External2/repos/upstream-llvm-project/build-clang-bootstrap/tools/clang/stage2-bins/include -I/Volumes/External2/repos/upstream-llvm-project/llvm/include -fPIC -fvisibility-inlines-hidden -fdiagnostics-color -O2 -g -DNDEBUG -std=c++17 -arch arm64 -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX15.0.sdk -UNDEBUG -fno-exceptions -funwind-tables -fno-rtti -MD -MT lib/Support/CMakeFiles/LLVMSupport.dir/StringSaver.cpp.o -MF lib/Support/CMakeFiles/LLVMSupport.dir/StringSaver.cpp.o.d -o lib/Support/CMakeFiles/LLVMSupport.dir/StringSaver.cpp.o -c /Volumes/External2/repos/upstream-llvm-project/llvm/lib/Support/StringSaver.cpp -fmodules -fcxx-modules -Xclang -fmodules-local-submodule-visibility -gmodules
1.      /Volumes/External2/repos/upstream-llvm-project/llvm/lib/Support/StringSaver.cpp:25:37: current parser token ')'
2.      /Volumes/External2/repos/upstream-llvm-project/llvm/lib/Support/StringSaver.cpp:23:45: parsing function body 'llvm::StringSaver::save'
3.      /Volumes/External2/repos/upstream-llvm-project/llvm/lib/Support/StringSaver.cpp:23:45: in compound statement ('{}')
Stack dump without symbol names (ensure you have llvm-symbolizer in your PATH or set the environment var `LLVM_SYMBOLIZER_PATH` to point to it):
0  clang-21                 0x0000000105099ac8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) + 56
1  clang-21                 0x0000000105097fdc llvm::sys::RunSignalHandlers() + 112
2  clang-21                 0x00000001050992c0 llvm::sys::CleanupOnSignal(unsigned long) + 272
3  clang-21                 0x0000000104ffd738 CrashRecoverySignalHandler(int) + 156
4  libsystem_platform.dylib 0x0000000194e9f624 _sigtramp + 56
5  libsystem_pthread.dylib  0x0000000194e6588c pthread_kill + 296
6  libsystem_c.dylib        0x0000000194d6eb34 abort + 124
7  libsystem_c.dylib        0x0000000194d6de44 err + 0
8  clang-21                 0x0000000107936e50 clang::Sema::RequireLiteralType(clang::SourceLocation, clang::QualType, clang::Sema::TypeDiagnoser&amp;) (.cold.11) + 0
9  clang-21                 0x0000000106cdf37c clang::Sema::hasAcceptableDefinition(clang::NamedDecl*, clang::NamedDecl**, clang::Sema::AcceptableKind, bool) + 468
10 clang-21                 0x0000000106cdec50 clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*) + 692
11 clang-21                 0x0000000106aa35f0 clang::Sema::CompareReferenceRelationship(clang::SourceLocation, clang::QualType, clang::QualType, clang::Sema::ReferenceConversionsScope::ReferenceConversions*) + 300
12 clang-21                 0x0000000106ac6cb4 FindConversionForRefInit(clang::Sema&amp;, clang::ImplicitConversionSequence&amp;, clang::QualType, clang::SourceLocation, clang::Expr*, clang::QualType, bool, bool) + 976
13 clang-21                 0x0000000106ac6138 TryReferenceInit(clang::Sema&amp;, clang::Expr*, clang::QualType, clang::SourceLocation, bool, bool) + 644
14 clang-21                 0x0000000106aa843c clang::Sema::AddMethodCandidate(clang::CXXMethodDecl*, clang::DeclAccessPair, clang::CXXRecordDecl*, clang::QualType, clang::Expr::Classification, llvm::ArrayRef&lt;clang::Expr*&gt;, 
clang::OverloadCandidateSet&amp;, bool, bool, llvm::MutableArrayRef&lt;clang::ImplicitConversionSequence&gt;, clang::OverloadCandidateParamOrder, bool) + 1396
15 clang-21                 0x0000000106abea98 clang::Sema::BuildCallToMemberFunction(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, bool) 
+ 2152
16 clang-21                 0x0000000106816c4c clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*) + 84
17 clang-21                 0x00000001065450cc clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult&lt;clang::Expr*, true&gt;) + 3808
18 clang-21                 0x0000000106546968 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) + 1572
19 clang-21                 0x0000000106542f5c clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) + 244
20 clang-21                 0x0000000106542e50 clang::Parser::ParseExpression(clang::Parser::TypeCastState) + 20
21 clang-21                 0x000000010659ab58 clang::Parser::ParseReturnStatement() + 324
22 clang-21                 0x00000001065958b4 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttribute
s&amp;, clang::ParsedAttributes&amp;) + 3772
23 clang-21                 0x00000001065947d8 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) + 324
24 clang-21                 0x000000010659c770 clang::Parser::ParseCompoundStatementBody(bool) + 1964
25 clang-21                 0x000000010659d408 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) + 184
26 clang-21                 0x00000001065b23e8 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) + 2068
27 clang-21                 0x0000000106518bb8 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Par
ser::ForRangeInit*) + 3940
28 clang-21                 0x00000001065b1940 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) + 936
29 clang-21                 0x00000001065b1338 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) + 464
30 clang-21                 0x00000001065b07ac clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) + 1580
31 clang-21                 0x00000001065af134 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) + 1384
32 clang-21                 0x0000000106505f50 clang::ParseAST(clang::Sema&amp;, bool, bool) + 612
33 clang-21                 0x0000000105ab8ddc clang::FrontendAction::Execute() + 48
34 clang-21                 0x0000000105a57d94 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) + 900
35 clang-21                 0x0000000105b2b8d4 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) + 480
36 clang-21                 0x0000000102c59678 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) + 4272
37 clang-21                 0x0000000102c55068 ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) + 948
38 clang-21                 0x00000001058ea7b8 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::__1::optional&lt;llvm::StringRef&gt;&gt;, std::__1::basic_string&lt;char, std::__1::char
_traits&lt;char&gt;, std::__1::allocator&lt;char&gt;&gt;*, bool*) const::$_0&gt;(long) + 32
39 clang-21                 0x0000000104ffd458 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) + 164
40 clang-21                 0x00000001058ea240 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::__1::optional&lt;llvm::StringRef&gt;&gt;, std::__1::basic_string&lt;char, std::__1::char_traits&lt;char&gt;, std::__1::allocator&lt;char&gt;&gt;*, bo
ol*) const + 228
41 clang-21                 0x00000001058bd6c8 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const + 476
42 clang-21                 0x00000001058bd890 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::__1::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const + 120
43 clang-21                 0x00000001058d2f8c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::__1::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) + 328
44 clang-21                 0x0000000102c545f0 clang_main(int, char**, llvm::ToolContext const&amp;) + 4760
45 clang-21                 0x0000000102c621c0 main + 88
46 dyld                     0x0000000194ac6b4c start + 6000
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
clang version 21.0.0git (git@<!-- -->github.com:llvm/llvm-project.git 1f51038036b4bc216eddcc620dcb9eca5df2bfef)
Target: arm64-apple-darwin24.4.0
Thread model: posix
InstalledDir: /Volumes/External2/repos/upstream-llvm-project/build-clang-bootstrap/bin
Build config: +assertions
clang++: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: /var/folders/t_/j6y672696bsd2yhrn8rd0wwm0000gn/T/StringSaver-92ef01.cpp
clang++: note: diagnostic msg: /var/folders/t_/j6y672696bsd2yhrn8rd0wwm0000gn/T/StringSaver-92ef01.cache
clang++: note: diagnostic msg: /var/folders/t_/j6y672696bsd2yhrn8rd0wwm0000gn/T/StringSaver-92ef01.sh
clang++: note: diagnostic msg: Crash backtrace is located in
clang++: note: diagnostic msg: /Users/zixu/Library/Logs/DiagnosticReports/clang-21_&lt;YYYY-MM-DD-HHMMSS&gt;_&lt;hostname&gt;.crash
clang++: note: diagnostic msg: (choose the .crash file that corresponds to your crash)
clang++: note: diagnostic msg: 

********************
```

Bisection pinned to a9e249f64e800fbb20a3b26c0cfb68c1a1aee5e1
</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Zixu Wang (zixu-w)

<details>
Crash with assertion failure when building `StringSaver.cpp` during bootstrap build, with modules enabled on macOS

```
❯ /Volumes/External2/repos/upstream-llvm-project/build-clang-bootstrap/./bin/clang++ -D_DEBUG -D_GLIBCXX_ASSERTIONS -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS -I/Volumes/External2/repos/upstream-llvm-project/build-clang-bootstrap/tools/clang/stage2-bins/lib/Support -I/Volumes/External2/repos/upstream-llvm-project/llvm/lib/Support -I/Volumes/External2/repos/upstream-llvm-project/build-clang-bootstrap/tools/clang/stage2-bins/include -I/Volumes/External2/repos/upstream-llvm-project/llvm/include -fPIC -fvisibility-inlines-hidden -fdiagnostics-color  -O2 -g -DNDEBUG -std=c++17 -arch arm64 -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX15.0.sdk -UNDEBUG  -fno-exceptions -funwind-tables -fno-rtti -MD -MT lib/Support/CMakeFiles/LLVMSupport.dir/StringSaver.cpp.o -MF lib/Support/CMakeFiles/LLVMSupport.dir/StringSaver.cpp.o.d -o lib/Support/CMakeFiles/LLVMSupport.dir/StringSaver.cpp.o -c /Volumes/External2/repos/upstream-llvm-project/llvm/lib/Support/StringSaver.cpp -fmodules -fcxx-modules -Xclang -fmodules-local-submodule-visibility -gmodules

Assertion failed: (D &amp;&amp; "missing definition for pattern of instantiated definition"), function hasAcceptableDefinition, file SemaType.cpp, line 9203.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /Volumes/External2/repos/upstream-llvm-project/build-clang-bootstrap/./bin/clang++ -D_DEBUG -D_GLIBCXX_ASSERTIONS -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS -I/Volumes/External2/repos/upstream-llvm-project/build-clang-bootstrap/tools/clang/stage2-bins/lib/Support -I/Volumes/External2/repos/upstream-llvm-project/llvm/lib/Support -I/Volumes/External2/repos/upstream-llvm-project/build-clang-bootstrap/tools/clang/stage2-bins/include -I/Volumes/External2/repos/upstream-llvm-project/llvm/include -fPIC -fvisibility-inlines-hidden -fdiagnostics-color -O2 -g -DNDEBUG -std=c++17 -arch arm64 -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX15.0.sdk -UNDEBUG -fno-exceptions -funwind-tables -fno-rtti -MD -MT lib/Support/CMakeFiles/LLVMSupport.dir/StringSaver.cpp.o -MF lib/Support/CMakeFiles/LLVMSupport.dir/StringSaver.cpp.o.d -o lib/Support/CMakeFiles/LLVMSupport.dir/StringSaver.cpp.o -c /Volumes/External2/repos/upstream-llvm-project/llvm/lib/Support/StringSaver.cpp -fmodules -fcxx-modules -Xclang -fmodules-local-submodule-visibility -gmodules
1.      /Volumes/External2/repos/upstream-llvm-project/llvm/lib/Support/StringSaver.cpp:25:37: current parser token ')'
2.      /Volumes/External2/repos/upstream-llvm-project/llvm/lib/Support/StringSaver.cpp:23:45: parsing function body 'llvm::StringSaver::save'
3.      /Volumes/External2/repos/upstream-llvm-project/llvm/lib/Support/StringSaver.cpp:23:45: in compound statement ('{}')
Stack dump without symbol names (ensure you have llvm-symbolizer in your PATH or set the environment var `LLVM_SYMBOLIZER_PATH` to point to it):
0  clang-21                 0x0000000105099ac8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) + 56
1  clang-21                 0x0000000105097fdc llvm::sys::RunSignalHandlers() + 112
2  clang-21                 0x00000001050992c0 llvm::sys::CleanupOnSignal(unsigned long) + 272
3  clang-21                 0x0000000104ffd738 CrashRecoverySignalHandler(int) + 156
4  libsystem_platform.dylib 0x0000000194e9f624 _sigtramp + 56
5  libsystem_pthread.dylib  0x0000000194e6588c pthread_kill + 296
6  libsystem_c.dylib        0x0000000194d6eb34 abort + 124
7  libsystem_c.dylib        0x0000000194d6de44 err + 0
8  clang-21                 0x0000000107936e50 clang::Sema::RequireLiteralType(clang::SourceLocation, clang::QualType, clang::Sema::TypeDiagnoser&amp;) (.cold.11) + 0
9  clang-21                 0x0000000106cdf37c clang::Sema::hasAcceptableDefinition(clang::NamedDecl*, clang::NamedDecl**, clang::Sema::AcceptableKind, bool) + 468
10 clang-21                 0x0000000106cdec50 clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*) + 692
11 clang-21                 0x0000000106aa35f0 clang::Sema::CompareReferenceRelationship(clang::SourceLocation, clang::QualType, clang::QualType, clang::Sema::ReferenceConversionsScope::ReferenceConversions*) + 300
12 clang-21                 0x0000000106ac6cb4 FindConversionForRefInit(clang::Sema&amp;, clang::ImplicitConversionSequence&amp;, clang::QualType, clang::SourceLocation, clang::Expr*, clang::QualType, bool, bool) + 976
13 clang-21                 0x0000000106ac6138 TryReferenceInit(clang::Sema&amp;, clang::Expr*, clang::QualType, clang::SourceLocation, bool, bool) + 644
14 clang-21                 0x0000000106aa843c clang::Sema::AddMethodCandidate(clang::CXXMethodDecl*, clang::DeclAccessPair, clang::CXXRecordDecl*, clang::QualType, clang::Expr::Classification, llvm::ArrayRef&lt;clang::Expr*&gt;, 
clang::OverloadCandidateSet&amp;, bool, bool, llvm::MutableArrayRef&lt;clang::ImplicitConversionSequence&gt;, clang::OverloadCandidateParamOrder, bool) + 1396
15 clang-21                 0x0000000106abea98 clang::Sema::BuildCallToMemberFunction(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, bool) 
+ 2152
16 clang-21                 0x0000000106816c4c clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*) + 84
17 clang-21                 0x00000001065450cc clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult&lt;clang::Expr*, true&gt;) + 3808
18 clang-21                 0x0000000106546968 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) + 1572
19 clang-21                 0x0000000106542f5c clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) + 244
20 clang-21                 0x0000000106542e50 clang::Parser::ParseExpression(clang::Parser::TypeCastState) + 20
21 clang-21                 0x000000010659ab58 clang::Parser::ParseReturnStatement() + 324
22 clang-21                 0x00000001065958b4 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttribute
s&amp;, clang::ParsedAttributes&amp;) + 3772
23 clang-21                 0x00000001065947d8 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) + 324
24 clang-21                 0x000000010659c770 clang::Parser::ParseCompoundStatementBody(bool) + 1964
25 clang-21                 0x000000010659d408 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) + 184
26 clang-21                 0x00000001065b23e8 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) + 2068
27 clang-21                 0x0000000106518bb8 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Par
ser::ForRangeInit*) + 3940
28 clang-21                 0x00000001065b1940 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) + 936
29 clang-21                 0x00000001065b1338 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) + 464
30 clang-21                 0x00000001065b07ac clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) + 1580
31 clang-21                 0x00000001065af134 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) + 1384
32 clang-21                 0x0000000106505f50 clang::ParseAST(clang::Sema&amp;, bool, bool) + 612
33 clang-21                 0x0000000105ab8ddc clang::FrontendAction::Execute() + 48
34 clang-21                 0x0000000105a57d94 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) + 900
35 clang-21                 0x0000000105b2b8d4 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) + 480
36 clang-21                 0x0000000102c59678 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) + 4272
37 clang-21                 0x0000000102c55068 ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) + 948
38 clang-21                 0x00000001058ea7b8 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::__1::optional&lt;llvm::StringRef&gt;&gt;, std::__1::basic_string&lt;char, std::__1::char
_traits&lt;char&gt;, std::__1::allocator&lt;char&gt;&gt;*, bool*) const::$_0&gt;(long) + 32
39 clang-21                 0x0000000104ffd458 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) + 164
40 clang-21                 0x00000001058ea240 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::__1::optional&lt;llvm::StringRef&gt;&gt;, std::__1::basic_string&lt;char, std::__1::char_traits&lt;char&gt;, std::__1::allocator&lt;char&gt;&gt;*, bo
ol*) const + 228
41 clang-21                 0x00000001058bd6c8 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const + 476
42 clang-21                 0x00000001058bd890 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::__1::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const + 120
43 clang-21                 0x00000001058d2f8c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::__1::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) + 328
44 clang-21                 0x0000000102c545f0 clang_main(int, char**, llvm::ToolContext const&amp;) + 4760
45 clang-21                 0x0000000102c621c0 main + 88
46 dyld                     0x0000000194ac6b4c start + 6000
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
clang version 21.0.0git (git@<!-- -->github.com:llvm/llvm-project.git 1f51038036b4bc216eddcc620dcb9eca5df2bfef)
Target: arm64-apple-darwin24.4.0
Thread model: posix
InstalledDir: /Volumes/External2/repos/upstream-llvm-project/build-clang-bootstrap/bin
Build config: +assertions
clang++: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: /var/folders/t_/j6y672696bsd2yhrn8rd0wwm0000gn/T/StringSaver-92ef01.cpp
clang++: note: diagnostic msg: /var/folders/t_/j6y672696bsd2yhrn8rd0wwm0000gn/T/StringSaver-92ef01.cache
clang++: note: diagnostic msg: /var/folders/t_/j6y672696bsd2yhrn8rd0wwm0000gn/T/StringSaver-92ef01.sh
clang++: note: diagnostic msg: Crash backtrace is located in
clang++: note: diagnostic msg: /Users/zixu/Library/Logs/DiagnosticReports/clang-21_&lt;YYYY-MM-DD-HHMMSS&gt;_&lt;hostname&gt;.crash
clang++: note: diagnostic msg: (choose the .crash file that corresponds to your crash)
clang++: note: diagnostic msg: 

********************
```

Bisection pinned to a9e249f64e800fbb20a3b26c0cfb68c1a1aee5e1
</details>


---

### Comment 3 - zixu-w

Trying to come up with a smaller and isolated reproducer that doesn't involve building LLVM itself (StringSaver).

Also this seems to be related to libc++ in the macOS SDK, so you would need an Xcode to reproduce: https://developer.apple.com/xcode/

---

### Comment 4 - zixu-w

Reproducing steps:
- Get Xcode: https://developer.apple.com/xcode/
- Configure the build with `cmake -G Ninja -DCLANG_ENABLE_BOOTSTRAP=On -DBOOTSTRAP_LLVM_ENABLE_MODULES=On -DCMAKE_BUILD_TYPE=RelWithDebInfo -DLLVM_ENABLE_ASSERTIONS=On -DLLVM_ENABLE_PROJECTS="clang" -DCLANG_BOOTSTRAP_TARGETS="LLVMSupport" ../llvm`
- Build `ninja -v stage2-LLVMSupport`
- Stage 2 build crashes with the above assertion when building `StringSaver.cpp`

---

### Comment 5 - zixu-w

From the backtrace, it seems to be checking this
```
StringRef StringSaver::save(const Twine &S) {
  SmallString<128> Storage;
  return save(S.toStringRef(Storage));
         ~~~~~~~~~~~~~~~~~~~~~~~~~~~^  // StringSaver.cpp:25:37: current parser token ')'
}
```

Maybe trying to resolve overloads for call to `save`: 
```
9  clang-21                 0x0000000106cdf37c clang::Sema::hasAcceptableDefinition(clang::NamedDecl*, clang::NamedDecl**, clang::Sema::AcceptableKind, bool) + 468
10 clang-21                 0x0000000106cdec50 clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*) + 692
11 clang-21                 0x0000000106aa35f0 clang::Sema::CompareReferenceRelationship(clang::SourceLocation, clang::QualType, clang::QualType, clang::Sema::ReferenceConversionsScope::ReferenceConversions*) + 300
12 clang-21                 0x0000000106ac6cb4 FindConversionForRefInit(clang::Sema&, clang::ImplicitConversionSequence&, clang::QualType, clang::SourceLocation, clang::Expr*, clang::QualType, bool, bool) + 976
13 clang-21                 0x0000000106ac6138 TryReferenceInit(clang::Sema&, clang::Expr*, clang::QualType, clang::SourceLocation, bool, bool) + 644
14 clang-21                 0x0000000106aa843c clang::Sema::AddMethodCandidate(...
15 clang-21                 0x0000000106abea98 clang::Sema::BuildCallToMemberFunction(... 
16 clang-21                 0x0000000106816c4c clang::Sema::ActOnCallExpr(...
```

---

### Comment 6 - zixu-w

Some preliminary findings:
I dumped out the decl causing the assertion, which is `basic_string` from libc++:
```
ClassTemplateSpecializationDecl 0x122dcf8f8 </Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/
SDKs/MacOSX15.2.sdk/usr/inlude/c++/v1/__fwd/string.h:45:1, line:46:28> /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/
SDKs/MacOSX15.2.sdk/usr/include/c++/v1/string:709:7
imported in std_private_string_string_fwd hidden <undeserialized declarations>
class basic_string definition implicit_instantiation
|-DefinitionData standard_layout has_user_declared_ctor can_const_default_init
| |-DefaultConstructor exists non_trivial user_provided defaulted_is_constexpr
| |-CopyConstructor non_trivial user_declared has_const_param needs_overload_resolution implicit_has_const_param
| |-MoveConstructor exists non_trivial user_declared
| |-CopyAssignment non_trivial has_const_param user_declared needs_overload_resolution implicit_has_const_param
| |-MoveAssignment exists non_trivial user_declared
| `-Destructor non_trivial user_declared
|-TemplateArgument type 'char'
| `-BuiltinType 0x12106a4b0 'char'
|-TemplateArgument type 'std::char_traits<char>'
| `-RecordType 0x122dcfb40 'std::char_traits<char>' imported
|   `-ClassTemplateSpecialization 0x122e92e08 'char_traits'
|-TemplateArgument type 'std::allocator<char>'
| `-RecordType 0x122dcfc60 'std::allocator<char>' imported
|   `-ClassTemplateSpecialization 0x122dcfb68 'allocator'
|-OwnerAttr 0x122dd9800 <<invalid sloc>> Inherited Implicit
`-PreferredNameAttr 0x122dd9840 </Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/
SDKs/MacOSX15.2.sdk/usr/include/c++/v1/__config:1538:54, col:74> string
```

`.../usr/include/c++/v1/__fwd/string.h:45:1 - 46:28` is the forward declaration:
```
template <class _CharT, class _Traits = char_traits<_CharT>, class _Allocator = allocator<_CharT> >
class _LIBCPP_TEMPLATE_VIS basic_string;
```
and `.../usr/include/c++/v1/string:709:1` is the definition:
```
template <class _CharT, class _Traits, class _Allocator>
class basic_string {
...
```

This takes the [`CXXRecordDecl` path](https://github.com/llvm/llvm-project/blob/c6d95c441a29a45782ff72d6cb82839b86fd0e4a/clang/lib/Sema/SemaType.cpp#L9174) in `hasAcceptableDefinition`.

The `Pattern` obtained from this specialization decl is
```
CXXRecordDecl 0x122dcf658 </Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/
MacOSX15.2.sdk/usr/include/c++/v1/__fwd/string.h:46:1, col:28> col:28
imported in std_private_string_string_fwd hidden class basic_string
`-OwnerAttr 0x122dcf700 <<invalid sloc>> Implicit
```
and at that point `DefinitionData` for this decl is `nullptr` so [`RD->getDefinition()`](https://github.com/llvm/llvm-project/blob/c6d95c441a29a45782ff72d6cb82839b86fd0e4a/clang/include/clang/AST/DeclCXX.h#L568) goes to call [`dataPtr()`](https://github.com/llvm/llvm-project/blob/c6d95c441a29a45782ff72d6cb82839b86fd0e4a/clang/include/clang/AST/DeclCXX.h#L457) and that's where this crash interacts with the redecl chain which is affected by the original commit. So somehow moving the `markIncompleteDeclChain()` to the end of `finishPendingActions` messed up the decl chain for `basic_string`, and thus it failed to find the definition of `basic_string`.

Inside `finishPendingActions`, there are a couple of places where `CXXRecordDecl` is handled. With some very simple experiments of moving the block of `markIncompleteDeclChain` around, I found that by moving it out of the initial while loop like the original commit did, but still keep it before further handing of Tag/Record decls (for example at https://github.com/llvm/llvm-project/blob/c6d95c441a29a45782ff72d6cb82839b86fd0e4a/clang/lib/Serialization/ASTReader.cpp#L10347), this crash goes away. In fact, move it just 10 lines from the end where the original commit put it [before the handling of `PendingAddedClassMembers`](https://github.com/llvm/llvm-project/blob/c6d95c441a29a45782ff72d6cb82839b86fd0e4a/clang/lib/Serialization/ASTReader.cpp#L10478) also fixed the crash.

While it needs further investigation and better understanding of the root cause, my guess is that `markIncompleteDeclChain` is interfering with the other Tag/Record decl pending action steps and left the redecl chain in a weird state. Move marking incomplete decl chain back up 10 lines looks like a very simple fix, but I'm not really comfortable to commit that without understanding if there might be other potential unfixed issues with the same root cause, or if this will cause new issues.

---

### Comment 7 - mpark

@zixu-w Do the places you've experimented putting `markIncompleteDeclChain` that fixes the crash, also address the unit test in [#121245](https://github.com/llvm/llvm-project/pull/121245)?

---

### Comment 8 - zixu-w

> [@zixu-w](https://github.com/zixu-w) Do the places you've experimented putting `markIncompleteDeclChain` that fixes the crash, also address the unit test in [#121245](https://github.com/llvm/llvm-project/pull/121245)?

I haven't tried that. I'll pull in the test case from #121245 and play around tonight.

---

### Comment 9 - zixu-w

@mpark I reproduced the test failure with `clang/Module/pr121245.cpp`, and tried moving `markIncompleteDeclChain` around. Putting it after `PendingBodies` and before `PendingAddedClassMembers` is enough to fix `clang/Module/pr121245.cpp` while also fixing this crash.

`clang/Module/pr121245.cpp` would still fail with unreachable if `markIncompleteDeclChain` is put anywhere before `PendingBodies`.

---

### Comment 10 - mpark

@zixu-w okay, great. thank you! I'm looking into this as well

---

### Comment 11 - mpark

This issue has been fixed by #129982.

---

