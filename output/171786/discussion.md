# clang crash when getting really creative with variadic macros

**Author:** pantoniou
**URL:** https://github.com/llvm/llvm-project/issues/171786

## Body

When macro expansion gets very complicated via nested variadic macro expansion and _Generic clang crashes.
For the same file gcc works: "$ gcc --version gcc (Ubuntu 13.3.0-6ubuntu2~24.04) 13.3.0" It does take a long time though.

```console
0.	Program arguments: /usr/bin/clang -D_GNU_SOURCE -I/home/panto/work/libfyaml/include -I/home/panto/work/libfyaml/src/valgrind -I/home/panto/work/libfyaml/src/lib -I/home/panto/work/libfyaml/src/util -I/home/panto/work/libfyaml/src/generic -I/home/panto/work/libfyaml/src/allocator -I/home/panto/work/libfyaml/src/reflection -I/home/panto/work/libfyaml/build -O2 -g -DNDEBUG -Wall -Wsign-compare -Wno-unused-function -Wno-tautological-constant-out-of-range-compare -std=gnu2x -MD -MT CMakeFiles/libfyaml-test.dir/test/libfyaml-test-generic.c.o -MF CMakeFiles/libfyaml-test.dir/test/libfyaml-test-generic.c.o.d -o CMakeFiles/libfyaml-test.dir/test/libfyaml-test-generic.c.o -c /home/panto/work/libfyaml/test/libfyaml-test-generic.c
1.	/home/panto/work/libfyaml/test/libfyaml-test-generic.c:3421:6 <Spelling=/home/panto/work/libfyaml/src/generic/fy-generic.h:2495:2>: current parser token 'fy_local_mapping_create_value'
2.	/home/panto/work/libfyaml/test/libfyaml-test-generic.c:3358:1: parsing function body 'local_ops_retry_fn'
3.	/home/panto/work/libfyaml/test/libfyaml-test-generic.c:3358:1: in compound statement ('{}')
Stack dump without symbol names (ensure you have llvm-symbolizer in your PATH or set the environment var `LLVM_SYMBOLIZER_PATH` to point to it):
0  libLLVM.so.18.1      0x00007ba08f7a63bf llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) + 63
1  libLLVM.so.18.1      0x00007ba08f7a44f9 llvm::sys::RunSignalHandlers() + 89
2  libLLVM.so.18.1      0x00007ba08f6efff3
3  libLLVM.so.18.1      0x00007ba08f6effa2
4  libLLVM.so.18.1      0x00007ba08f7a0c70 llvm::sys::Process::Exit(int, bool) + 48
5  clang                0x000063f3607f891b
6  libLLVM.so.18.1      0x00007ba08f6fe01c llvm::report_fatal_error(llvm::Twine const&, bool) + 284
7  libLLVM.so.18.1      0x00007ba08f6fdefd
8  libclang-cpp.so.18.1 0x00007ba096c601dc clang::SourceManager::createExpansionLocImpl(clang::SrcMgr::ExpansionInfo const&, unsigned int, int, unsigned int) + 620
9  libclang-cpp.so.18.1 0x00007ba096c5ff54 clang::SourceManager::createMacroArgExpansionLoc(clang::SourceLocation, clang::SourceLocation, unsigned int) + 68
10 libclang-cpp.so.18.1 0x00007ba096d96bdc clang::TokenLexer::updateLocForMacroArgTokens(clang::SourceLocation, clang::Token*, clang::Token*) + 1148
11 libclang-cpp.so.18.1 0x00007ba096d9510c clang::TokenLexer::ExpandFunctionArguments() + 3244
12 libclang-cpp.so.18.1 0x00007ba096d943d7 clang::TokenLexer::Init(clang::Token&, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) + 311
13 libclang-cpp.so.18.1 0x00007ba096d691ce clang::Preprocessor::EnterMacro(clang::Token&, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) + 158
14 libclang-cpp.so.18.1 0x00007ba096d6fbdc clang::Preprocessor::HandleMacroExpandedIdentifier(clang::Token&, clang::MacroDefinition const&) + 2908
15 libclang-cpp.so.18.1 0x00007ba096d8f1c4 clang::Preprocessor::HandleIdentifier(clang::Token&) + 468
16 libclang-cpp.so.18.1 0x00007ba096d8f5ed clang::Preprocessor::Lex(clang::Token&) + 45
17 libclang-cpp.so.18.1 0x00007ba096d31163 clang::MacroArgs::getPreExpArgument(unsigned int, clang::Preprocessor&) + 291
18 libclang-cpp.so.18.1 0x00007ba096d94f1a clang::TokenLexer::ExpandFunctionArguments() + 2746
19 libclang-cpp.so.18.1 0x00007ba096d943d7 clang::TokenLexer::Init(clang::Token&, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) + 311
20 libclang-cpp.so.18.1 0x00007ba096d691ce clang::Preprocessor::EnterMacro(clang::Token&, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) + 158
21 libclang-cpp.so.18.1 0x00007ba096d6fbdc clang::Preprocessor::HandleMacroExpandedIdentifier(clang::Token&, clang::MacroDefinition const&) + 2908
22 libclang-cpp.so.18.1 0x00007ba096d8f1c4 clang::Preprocessor::HandleIdentifier(clang::Token&) + 468
23 libclang-cpp.so.18.1 0x00007ba096d8f5ed clang::Preprocessor::Lex(clang::Token&) + 45
24 libclang-cpp.so.18.1 0x00007ba096d31163 clang::MacroArgs::getPreExpArgument(unsigned int, clang::Preprocessor&) + 291
25 libclang-cpp.so.18.1 0x00007ba096d94f1a clang::TokenLexer::ExpandFunctionArguments() + 2746
26 libclang-cpp.so.18.1 0x00007ba096d943d7 clang::TokenLexer::Init(clang::Token&, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) + 311
27 libclang-cpp.so.18.1 0x00007ba096d691ce clang::Preprocessor::EnterMacro(clang::Token&, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) + 158
28 libclang-cpp.so.18.1 0x00007ba096d6fbdc clang::Preprocessor::HandleMacroExpandedIdentifier(clang::Token&, clang::MacroDefinition const&) + 2908
29 libclang-cpp.so.18.1 0x00007ba096d8f1c4 clang::Preprocessor::HandleIdentifier(clang::Token&) + 468
30 libclang-cpp.so.18.1 0x00007ba096d8f5ed clang::Preprocessor::Lex(clang::Token&) + 45
31 libclang-cpp.so.18.1 0x00007ba096d31163 clang::MacroArgs::getPreExpArgument(unsigned int, clang::Preprocessor&) + 291
32 libclang-cpp.so.18.1 0x00007ba096d94f1a clang::TokenLexer::ExpandFunctionArguments() + 2746
33 libclang-cpp.so.18.1 0x00007ba096d943d7 clang::TokenLexer::Init(clang::Token&, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) + 311
34 libclang-cpp.so.18.1 0x00007ba096d691ce clang::Preprocessor::EnterMacro(clang::Token&, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) + 158
35 libclang-cpp.so.18.1 0x00007ba096d6fbdc clang::Preprocessor::HandleMacroExpandedIdentifier(clang::Token&, clang::MacroDefinition const&) + 2908
36 libclang-cpp.so.18.1 0x00007ba096d8f1c4 clang::Preprocessor::HandleIdentifier(clang::Token&) + 468
37 libclang-cpp.so.18.1 0x00007ba096d8f5ed clang::Preprocessor::Lex(clang::Token&) + 45
38 libclang-cpp.so.18.1 0x00007ba096d31163 clang::MacroArgs::getPreExpArgument(unsigned int, clang::Preprocessor&) + 291
39 libclang-cpp.so.18.1 0x00007ba096d94f1a clang::TokenLexer::ExpandFunctionArguments() + 2746
40 libclang-cpp.so.18.1 0x00007ba096d943d7 clang::TokenLexer::Init(clang::Token&, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) + 311
41 libclang-cpp.so.18.1 0x00007ba096d691ce clang::Preprocessor::EnterMacro(clang::Token&, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) + 158
42 libclang-cpp.so.18.1 0x00007ba096d6fbdc clang::Preprocessor::HandleMacroExpandedIdentifier(clang::Token&, clang::MacroDefinition const&) + 2908
43 libclang-cpp.so.18.1 0x00007ba096d8f1c4 clang::Preprocessor::HandleIdentifier(clang::Token&) + 468
44 libclang-cpp.so.18.1 0x00007ba096d8f5ed clang::Preprocessor::Lex(clang::Token&) + 45
45 libclang-cpp.so.18.1 0x00007ba096d31163 clang::MacroArgs::getPreExpArgument(unsigned int, clang::Preprocessor&) + 291
46 libclang-cpp.so.18.1 0x00007ba096d94f1a clang::TokenLexer::ExpandFunctionArguments() + 2746
47 libclang-cpp.so.18.1 0x00007ba096d943d7 clang::TokenLexer::Init(clang::Token&, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) + 311
48 libclang-cpp.so.18.1 0x00007ba096d691ce clang::Preprocessor::EnterMacro(clang::Token&, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) + 158
49 libclang-cpp.so.18.1 0x00007ba096d6fbdc clang::Preprocessor::HandleMacroExpandedIdentifier(clang::Token&, clang::MacroDefinition const&) + 2908
50 libclang-cpp.so.18.1 0x00007ba096d8f1c4 clang::Preprocessor::HandleIdentifier(clang::Token&) + 468
51 libclang-cpp.so.18.1 0x00007ba096d8f5ed clang::Preprocessor::Lex(clang::Token&) + 45
52 libclang-cpp.so.18.1 0x00007ba096d31163 clang::MacroArgs::getPreExpArgument(unsigned int, clang::Preprocessor&) + 291
53 libclang-cpp.so.18.1 0x00007ba096d94f1a clang::TokenLexer::ExpandFunctionArguments() + 2746
54 libclang-cpp.so.18.1 0x00007ba096d943d7 clang::TokenLexer::Init(clang::Token&, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) + 311
55 libclang-cpp.so.18.1 0x00007ba096d691ce clang::Preprocessor::EnterMacro(clang::Token&, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) + 158
56 libclang-cpp.so.18.1 0x00007ba096d6fbdc clang::Preprocessor::HandleMacroExpandedIdentifier(clang::Token&, clang::MacroDefinition const&) + 2908
57 libclang-cpp.so.18.1 0x00007ba096d8f1c4 clang::Preprocessor::HandleIdentifier(clang::Token&) + 468
58 libclang-cpp.so.18.1 0x00007ba096d8f5ed clang::Preprocessor::Lex(clang::Token&) + 45
59 libclang-cpp.so.18.1 0x00007ba096de6241 clang::Parser::ParseParenExpression(clang::Parser::ParenParseOption&, bool, bool, clang::OpaquePtr<clang::QualType>&, clang::SourceLocation&) + 241
60 libclang-cpp.so.18.1 0x00007ba096de541c clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) + 10604
61 libclang-cpp.so.18.1 0x00007ba096ddec24 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) + 276
62 libclang-cpp.so.18.1 0x00007ba096de80c5 clang::Parser::ParseGenericSelectionExpression() + 1349
63 libclang-cpp.so.18.1 0x00007ba096de4289 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) + 6105
64 libclang-cpp.so.18.1 0x00007ba096ddec24 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) + 276
65 libclang-cpp.so.18.1 0x00007ba096de6d59 clang::Parser::ParseParenExpression(clang::Parser::ParenParseOption&, bool, bool, clang::OpaquePtr<clang::QualType>&, clang::SourceLocation&) + 3081
66 libclang-cpp.so.18.1 0x00007ba096de541c clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) + 10604
67 libclang-cpp.so.18.1 0x00007ba096ddec24 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) + 276
68 libclang-cpp.so.18.1 0x00007ba096ddf202 clang::Parser::ParseRHSOfBinaryExpression(clang::ActionResult<clang::Expr*, true>, clang::prec::Level) + 1282
69 libclang-cpp.so.18.1 0x00007ba096ddeccc clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) + 444
70 libclang-cpp.so.18.1 0x00007ba096de6d59 clang::Parser::ParseParenExpression(clang::Parser::ParenParseOption&, bool, bool, clang::OpaquePtr<clang::QualType>&, clang::SourceLocation&) + 3081
71 libclang-cpp.so.18.1 0x00007ba096de541c clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) + 10604
72 libclang-cpp.so.18.1 0x00007ba096ddec24 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) + 276
73 libclang-cpp.so.18.1 0x00007ba096e002f1 clang::Parser::ParseInitializerWithPotentialDesignator(clang::Parser::DesignatorCompletionInfo) + 3745
74 libclang-cpp.so.18.1 0x00007ba096e00b92 clang::Parser::ParseBraceInitializer() + 1234
75 libclang-cpp.so.18.1 0x00007ba096dec08a clang::Parser::ParseCompoundLiteralExpression(clang::OpaquePtr<clang::QualType>, clang::SourceLocation, clang::SourceLocation) + 234
76 libclang-cpp.so.18.1 0x00007ba096de71af clang::Parser::ParseParenExpression(clang::Parser::ParenParseOption&, bool, bool, clang::OpaquePtr<clang::QualType>&, clang::SourceLocation&) + 4191
77 libclang-cpp.so.18.1 0x00007ba096de541c clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) + 10604
78 libclang-cpp.so.18.1 0x00007ba096ddec24 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) + 276
79 libclang-cpp.so.18.1 0x00007ba096de6d59 clang::Parser::ParseParenExpression(clang::Parser::ParenParseOption&, bool, bool, clang::OpaquePtr<clang::QualType>&, clang::SourceLocation&) + 3081
80 libclang-cpp.so.18.1 0x00007ba096de541c clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) + 10604
81 libclang-cpp.so.18.1 0x00007ba096ddf67e clang::Parser::ParseRHSOfBinaryExpression(clang::ActionResult<clang::Expr*, true>, clang::prec::Level) + 2430
82 libclang-cpp.so.18.1 0x00007ba096ddeccc clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) + 444
83 libclang-cpp.so.18.1 0x00007ba096ddeaf2 clang::Parser::ParseExpression(clang::Parser::TypeCastState) + 18
84 libclang-cpp.so.18.1 0x00007ba096e41239 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) + 73
85 libclang-cpp.so.18.1 0x00007ba096e3f471 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) + 1073
86 libclang-cpp.so.18.1 0x00007ba096e3edfc clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) + 332
87 libclang-cpp.so.18.1 0x00007ba096e47aa2 clang::Parser::ParseCompoundStatementBody(bool) + 1618
88 libclang-cpp.so.18.1 0x00007ba096e48b77 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) + 183
89 libclang-cpp.so.18.1 0x00007ba096e640c7 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) + 3751
90 libclang-cpp.so.18.1 0x00007ba096dad2a1 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::SourceLocation*, clang::Parser::ForRangeInit*) + 4689
91 libclang-cpp.so.18.1 0x00007ba096e6307f clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) + 1087
92 libclang-cpp.so.18.1 0x00007ba096e62a39 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) + 489
93 libclang-cpp.so.18.1 0x00007ba096e61c65 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) + 2197
94 libclang-cpp.so.18.1 0x00007ba096e5ff7b clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) + 1499
95 libclang-cpp.so.18.1 0x00007ba096d9747e clang::ParseAST(clang::Sema&, bool, bool) + 766
96 libclang-cpp.so.18.1 0x00007ba098c0662c clang::FrontendAction::Execute() + 92
97 libclang-cpp.so.18.1 0x00007ba098b830b4 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) + 708
98 libclang-cpp.so.18.1 0x00007ba098c8263d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) + 765
99 clang                0x000063f3607f842e cc1_main(llvm::ArrayRef<char const*>, char const*, void*) + 3694
100 clang                0x000063f3607f5894
101 libclang-cpp.so.18.1 0x00007ba098833972
102 libLLVM.so.18.1      0x00007ba08f6eff77 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) + 151
103 libclang-cpp.so.18.1 0x00007ba098833237 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const + 407
104 libclang-cpp.so.18.1 0x00007ba0987fb518 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const + 888
105 libclang-cpp.so.18.1 0x00007ba0987fb77f clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const + 159
106 libclang-cpp.so.18.1 0x00007ba098817c20 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) + 352
107 clang                0x000063f3607f51ec clang_main(int, char**, llvm::ToolContext const&) + 11180
108 clang                0x000063f360802383 main + 131
109 libc.so.6            0x00007ba08e22a1ca
110 libc.so.6            0x00007ba08e22a28b __libc_start_main + 139
111 clang                0x000063f3607f2255 _start + 37
clang: error: clang frontend command failed with exit code 70 (use -v to see invocation)
Ubuntu clang version 18.1.3 (1ubuntu1)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
clang: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang: note: diagnostic msg: /tmp/libfyaml-test-generic-affb2c.c
clang: note: diagnostic msg: /tmp/libfyaml-test-generic-affb2c.sh
clang: note: diagnostic msg: 

********************
```

[libfyaml-test-generic-affb2c.c](https://github.com/user-attachments/files/24099217/libfyaml-test-generic-affb2c.c)

[libfyaml-test-generic-affb2c.sh](https://github.com/user-attachments/files/24099218/libfyaml-test-generic-affb2c.sh)

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Pantelis Antoniou (pantoniou)

<details>
When macro expansion gets very complicated via nested variadic macro expansion and _Generic clang crashes.
For the same file gcc works: "$ gcc --version gcc (Ubuntu 13.3.0-6ubuntu2~24.04) 13.3.0" It does take a long time though.

0.	Program arguments: /usr/bin/clang -D_GNU_SOURCE -I/home/panto/work/libfyaml/include -I/home/panto/work/libfyaml/src/valgrind -I/home/panto/work/libfyaml/src/lib -I/home/panto/work/libfyaml/src/util -I/home/panto/work/libfyaml/src/generic -I/home/panto/work/libfyaml/src/allocator -I/home/panto/work/libfyaml/src/reflection -I/home/panto/work/libfyaml/build -O2 -g -DNDEBUG -Wall -Wsign-compare -Wno-unused-function -Wno-tautological-constant-out-of-range-compare -std=gnu2x -MD -MT CMakeFiles/libfyaml-test.dir/test/libfyaml-test-generic.c.o -MF CMakeFiles/libfyaml-test.dir/test/libfyaml-test-generic.c.o.d -o CMakeFiles/libfyaml-test.dir/test/libfyaml-test-generic.c.o -c /home/panto/work/libfyaml/test/libfyaml-test-generic.c
1.	/home/panto/work/libfyaml/test/libfyaml-test-generic.c:3421:6 &lt;Spelling=/home/panto/work/libfyaml/src/generic/fy-generic.h:2495:2&gt;: current parser token 'fy_local_mapping_create_value'
2.	/home/panto/work/libfyaml/test/libfyaml-test-generic.c:3358:1: parsing function body 'local_ops_retry_fn'
3.	/home/panto/work/libfyaml/test/libfyaml-test-generic.c:3358:1: in compound statement ('{}')
Stack dump without symbol names (ensure you have llvm-symbolizer in your PATH or set the environment var `LLVM_SYMBOLIZER_PATH` to point to it):
0  libLLVM.so.18.1      0x00007ba08f7a63bf llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) + 63
1  libLLVM.so.18.1      0x00007ba08f7a44f9 llvm::sys::RunSignalHandlers() + 89
2  libLLVM.so.18.1      0x00007ba08f6efff3
3  libLLVM.so.18.1      0x00007ba08f6effa2
4  libLLVM.so.18.1      0x00007ba08f7a0c70 llvm::sys::Process::Exit(int, bool) + 48
5  clang                0x000063f3607f891b
6  libLLVM.so.18.1      0x00007ba08f6fe01c llvm::report_fatal_error(llvm::Twine const&amp;, bool) + 284
7  libLLVM.so.18.1      0x00007ba08f6fdefd
8  libclang-cpp.so.18.1 0x00007ba096c601dc clang::SourceManager::createExpansionLocImpl(clang::SrcMgr::ExpansionInfo const&amp;, unsigned int, int, unsigned int) + 620
9  libclang-cpp.so.18.1 0x00007ba096c5ff54 clang::SourceManager::createMacroArgExpansionLoc(clang::SourceLocation, clang::SourceLocation, unsigned int) + 68
10 libclang-cpp.so.18.1 0x00007ba096d96bdc clang::TokenLexer::updateLocForMacroArgTokens(clang::SourceLocation, clang::Token*, clang::Token*) + 1148
11 libclang-cpp.so.18.1 0x00007ba096d9510c clang::TokenLexer::ExpandFunctionArguments() + 3244
12 libclang-cpp.so.18.1 0x00007ba096d943d7 clang::TokenLexer::Init(clang::Token&amp;, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) + 311
13 libclang-cpp.so.18.1 0x00007ba096d691ce clang::Preprocessor::EnterMacro(clang::Token&amp;, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) + 158
14 libclang-cpp.so.18.1 0x00007ba096d6fbdc clang::Preprocessor::HandleMacroExpandedIdentifier(clang::Token&amp;, clang::MacroDefinition const&amp;) + 2908
15 libclang-cpp.so.18.1 0x00007ba096d8f1c4 clang::Preprocessor::HandleIdentifier(clang::Token&amp;) + 468
16 libclang-cpp.so.18.1 0x00007ba096d8f5ed clang::Preprocessor::Lex(clang::Token&amp;) + 45
17 libclang-cpp.so.18.1 0x00007ba096d31163 clang::MacroArgs::getPreExpArgument(unsigned int, clang::Preprocessor&amp;) + 291
18 libclang-cpp.so.18.1 0x00007ba096d94f1a clang::TokenLexer::ExpandFunctionArguments() + 2746
19 libclang-cpp.so.18.1 0x00007ba096d943d7 clang::TokenLexer::Init(clang::Token&amp;, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) + 311
20 libclang-cpp.so.18.1 0x00007ba096d691ce clang::Preprocessor::EnterMacro(clang::Token&amp;, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) + 158
21 libclang-cpp.so.18.1 0x00007ba096d6fbdc clang::Preprocessor::HandleMacroExpandedIdentifier(clang::Token&amp;, clang::MacroDefinition const&amp;) + 2908
22 libclang-cpp.so.18.1 0x00007ba096d8f1c4 clang::Preprocessor::HandleIdentifier(clang::Token&amp;) + 468
23 libclang-cpp.so.18.1 0x00007ba096d8f5ed clang::Preprocessor::Lex(clang::Token&amp;) + 45
24 libclang-cpp.so.18.1 0x00007ba096d31163 clang::MacroArgs::getPreExpArgument(unsigned int, clang::Preprocessor&amp;) + 291
25 libclang-cpp.so.18.1 0x00007ba096d94f1a clang::TokenLexer::ExpandFunctionArguments() + 2746
26 libclang-cpp.so.18.1 0x00007ba096d943d7 clang::TokenLexer::Init(clang::Token&amp;, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) + 311
27 libclang-cpp.so.18.1 0x00007ba096d691ce clang::Preprocessor::EnterMacro(clang::Token&amp;, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) + 158
28 libclang-cpp.so.18.1 0x00007ba096d6fbdc clang::Preprocessor::HandleMacroExpandedIdentifier(clang::Token&amp;, clang::MacroDefinition const&amp;) + 2908
29 libclang-cpp.so.18.1 0x00007ba096d8f1c4 clang::Preprocessor::HandleIdentifier(clang::Token&amp;) + 468
30 libclang-cpp.so.18.1 0x00007ba096d8f5ed clang::Preprocessor::Lex(clang::Token&amp;) + 45
31 libclang-cpp.so.18.1 0x00007ba096d31163 clang::MacroArgs::getPreExpArgument(unsigned int, clang::Preprocessor&amp;) + 291
32 libclang-cpp.so.18.1 0x00007ba096d94f1a clang::TokenLexer::ExpandFunctionArguments() + 2746
33 libclang-cpp.so.18.1 0x00007ba096d943d7 clang::TokenLexer::Init(clang::Token&amp;, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) + 311
34 libclang-cpp.so.18.1 0x00007ba096d691ce clang::Preprocessor::EnterMacro(clang::Token&amp;, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) + 158
35 libclang-cpp.so.18.1 0x00007ba096d6fbdc clang::Preprocessor::HandleMacroExpandedIdentifier(clang::Token&amp;, clang::MacroDefinition const&amp;) + 2908
36 libclang-cpp.so.18.1 0x00007ba096d8f1c4 clang::Preprocessor::HandleIdentifier(clang::Token&amp;) + 468
37 libclang-cpp.so.18.1 0x00007ba096d8f5ed clang::Preprocessor::Lex(clang::Token&amp;) + 45
38 libclang-cpp.so.18.1 0x00007ba096d31163 clang::MacroArgs::getPreExpArgument(unsigned int, clang::Preprocessor&amp;) + 291
39 libclang-cpp.so.18.1 0x00007ba096d94f1a clang::TokenLexer::ExpandFunctionArguments() + 2746
40 libclang-cpp.so.18.1 0x00007ba096d943d7 clang::TokenLexer::Init(clang::Token&amp;, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) + 311
41 libclang-cpp.so.18.1 0x00007ba096d691ce clang::Preprocessor::EnterMacro(clang::Token&amp;, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) + 158
42 libclang-cpp.so.18.1 0x00007ba096d6fbdc clang::Preprocessor::HandleMacroExpandedIdentifier(clang::Token&amp;, clang::MacroDefinition const&amp;) + 2908
43 libclang-cpp.so.18.1 0x00007ba096d8f1c4 clang::Preprocessor::HandleIdentifier(clang::Token&amp;) + 468
44 libclang-cpp.so.18.1 0x00007ba096d8f5ed clang::Preprocessor::Lex(clang::Token&amp;) + 45
45 libclang-cpp.so.18.1 0x00007ba096d31163 clang::MacroArgs::getPreExpArgument(unsigned int, clang::Preprocessor&amp;) + 291
46 libclang-cpp.so.18.1 0x00007ba096d94f1a clang::TokenLexer::ExpandFunctionArguments() + 2746
47 libclang-cpp.so.18.1 0x00007ba096d943d7 clang::TokenLexer::Init(clang::Token&amp;, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) + 311
48 libclang-cpp.so.18.1 0x00007ba096d691ce clang::Preprocessor::EnterMacro(clang::Token&amp;, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) + 158
49 libclang-cpp.so.18.1 0x00007ba096d6fbdc clang::Preprocessor::HandleMacroExpandedIdentifier(clang::Token&amp;, clang::MacroDefinition const&amp;) + 2908
50 libclang-cpp.so.18.1 0x00007ba096d8f1c4 clang::Preprocessor::HandleIdentifier(clang::Token&amp;) + 468
51 libclang-cpp.so.18.1 0x00007ba096d8f5ed clang::Preprocessor::Lex(clang::Token&amp;) + 45
52 libclang-cpp.so.18.1 0x00007ba096d31163 clang::MacroArgs::getPreExpArgument(unsigned int, clang::Preprocessor&amp;) + 291
53 libclang-cpp.so.18.1 0x00007ba096d94f1a clang::TokenLexer::ExpandFunctionArguments() + 2746
54 libclang-cpp.so.18.1 0x00007ba096d943d7 clang::TokenLexer::Init(clang::Token&amp;, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) + 311
55 libclang-cpp.so.18.1 0x00007ba096d691ce clang::Preprocessor::EnterMacro(clang::Token&amp;, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) + 158
56 libclang-cpp.so.18.1 0x00007ba096d6fbdc clang::Preprocessor::HandleMacroExpandedIdentifier(clang::Token&amp;, clang::MacroDefinition const&amp;) + 2908
57 libclang-cpp.so.18.1 0x00007ba096d8f1c4 clang::Preprocessor::HandleIdentifier(clang::Token&amp;) + 468
58 libclang-cpp.so.18.1 0x00007ba096d8f5ed clang::Preprocessor::Lex(clang::Token&amp;) + 45
59 libclang-cpp.so.18.1 0x00007ba096de6241 clang::Parser::ParseParenExpression(clang::Parser::ParenParseOption&amp;, bool, bool, clang::OpaquePtr&lt;clang::QualType&gt;&amp;, clang::SourceLocation&amp;) + 241
60 libclang-cpp.so.18.1 0x00007ba096de541c clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) + 10604
61 libclang-cpp.so.18.1 0x00007ba096ddec24 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) + 276
62 libclang-cpp.so.18.1 0x00007ba096de80c5 clang::Parser::ParseGenericSelectionExpression() + 1349
63 libclang-cpp.so.18.1 0x00007ba096de4289 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) + 6105
64 libclang-cpp.so.18.1 0x00007ba096ddec24 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) + 276
65 libclang-cpp.so.18.1 0x00007ba096de6d59 clang::Parser::ParseParenExpression(clang::Parser::ParenParseOption&amp;, bool, bool, clang::OpaquePtr&lt;clang::QualType&gt;&amp;, clang::SourceLocation&amp;) + 3081
66 libclang-cpp.so.18.1 0x00007ba096de541c clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) + 10604
67 libclang-cpp.so.18.1 0x00007ba096ddec24 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) + 276
68 libclang-cpp.so.18.1 0x00007ba096ddf202 clang::Parser::ParseRHSOfBinaryExpression(clang::ActionResult&lt;clang::Expr*, true&gt;, clang::prec::Level) + 1282
69 libclang-cpp.so.18.1 0x00007ba096ddeccc clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) + 444
70 libclang-cpp.so.18.1 0x00007ba096de6d59 clang::Parser::ParseParenExpression(clang::Parser::ParenParseOption&amp;, bool, bool, clang::OpaquePtr&lt;clang::QualType&gt;&amp;, clang::SourceLocation&amp;) + 3081
71 libclang-cpp.so.18.1 0x00007ba096de541c clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) + 10604
72 libclang-cpp.so.18.1 0x00007ba096ddec24 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) + 276
73 libclang-cpp.so.18.1 0x00007ba096e002f1 clang::Parser::ParseInitializerWithPotentialDesignator(clang::Parser::DesignatorCompletionInfo) + 3745
74 libclang-cpp.so.18.1 0x00007ba096e00b92 clang::Parser::ParseBraceInitializer() + 1234
75 libclang-cpp.so.18.1 0x00007ba096dec08a clang::Parser::ParseCompoundLiteralExpression(clang::OpaquePtr&lt;clang::QualType&gt;, clang::SourceLocation, clang::SourceLocation) + 234
76 libclang-cpp.so.18.1 0x00007ba096de71af clang::Parser::ParseParenExpression(clang::Parser::ParenParseOption&amp;, bool, bool, clang::OpaquePtr&lt;clang::QualType&gt;&amp;, clang::SourceLocation&amp;) + 4191
77 libclang-cpp.so.18.1 0x00007ba096de541c clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) + 10604
78 libclang-cpp.so.18.1 0x00007ba096ddec24 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) + 276
79 libclang-cpp.so.18.1 0x00007ba096de6d59 clang::Parser::ParseParenExpression(clang::Parser::ParenParseOption&amp;, bool, bool, clang::OpaquePtr&lt;clang::QualType&gt;&amp;, clang::SourceLocation&amp;) + 3081
80 libclang-cpp.so.18.1 0x00007ba096de541c clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) + 10604
81 libclang-cpp.so.18.1 0x00007ba096ddf67e clang::Parser::ParseRHSOfBinaryExpression(clang::ActionResult&lt;clang::Expr*, true&gt;, clang::prec::Level) + 2430
82 libclang-cpp.so.18.1 0x00007ba096ddeccc clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) + 444
83 libclang-cpp.so.18.1 0x00007ba096ddeaf2 clang::Parser::ParseExpression(clang::Parser::TypeCastState) + 18
84 libclang-cpp.so.18.1 0x00007ba096e41239 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) + 73
85 libclang-cpp.so.18.1 0x00007ba096e3f471 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) + 1073
86 libclang-cpp.so.18.1 0x00007ba096e3edfc clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) + 332
87 libclang-cpp.so.18.1 0x00007ba096e47aa2 clang::Parser::ParseCompoundStatementBody(bool) + 1618
88 libclang-cpp.so.18.1 0x00007ba096e48b77 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) + 183
89 libclang-cpp.so.18.1 0x00007ba096e640c7 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) + 3751
90 libclang-cpp.so.18.1 0x00007ba096dad2a1 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) + 4689
91 libclang-cpp.so.18.1 0x00007ba096e6307f clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) + 1087
92 libclang-cpp.so.18.1 0x00007ba096e62a39 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) + 489
93 libclang-cpp.so.18.1 0x00007ba096e61c65 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) + 2197
94 libclang-cpp.so.18.1 0x00007ba096e5ff7b clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) + 1499
95 libclang-cpp.so.18.1 0x00007ba096d9747e clang::ParseAST(clang::Sema&amp;, bool, bool) + 766
96 libclang-cpp.so.18.1 0x00007ba098c0662c clang::FrontendAction::Execute() + 92
97 libclang-cpp.so.18.1 0x00007ba098b830b4 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) + 708
98 libclang-cpp.so.18.1 0x00007ba098c8263d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) + 765
99 clang                0x000063f3607f842e cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) + 3694
100 clang                0x000063f3607f5894
101 libclang-cpp.so.18.1 0x00007ba098833972
102 libLLVM.so.18.1      0x00007ba08f6eff77 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) + 151
103 libclang-cpp.so.18.1 0x00007ba098833237 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const + 407
104 libclang-cpp.so.18.1 0x00007ba0987fb518 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const + 888
105 libclang-cpp.so.18.1 0x00007ba0987fb77f clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const + 159
106 libclang-cpp.so.18.1 0x00007ba098817c20 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) + 352
107 clang                0x000063f3607f51ec clang_main(int, char**, llvm::ToolContext const&amp;) + 11180
108 clang                0x000063f360802383 main + 131
109 libc.so.6            0x00007ba08e22a1ca
110 libc.so.6            0x00007ba08e22a28b __libc_start_main + 139
111 clang                0x000063f3607f2255 _start + 37
clang: error: clang frontend command failed with exit code 70 (use -v to see invocation)
Ubuntu clang version 18.1.3 (1ubuntu1)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
clang: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang: note: diagnostic msg: /tmp/libfyaml-test-generic-affb2c.c
clang: note: diagnostic msg: /tmp/libfyaml-test-generic-affb2c.sh
clang: note: diagnostic msg: 

********************


[libfyaml-test-generic-affb2c.c](https://github.com/user-attachments/files/24099217/libfyaml-test-generic-affb2c.c)

[libfyaml-test-generic-affb2c.sh](https://github.com/user-attachments/files/24099218/libfyaml-test-generic-affb2c.sh)
</details>


---

### Comment 2 - EugeneZelenko

Could you please try 21 or `main` branch? https://godbolt.org should be helpful. Only most recent release is maintained.


---

### Comment 3 - kiran-isaac

Running your script on the main branch, i get "fatal error: translation unit is too large for Clang to process: ran out of source locations". Please reduce

---

