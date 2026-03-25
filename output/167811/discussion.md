# clang 16 parse error related to construction from init list

**Author:** correaa
**URL:** https://github.com/llvm/llvm-project/issues/167811
**Status:** Closed
**Labels:** clang:frontend, obsolete, crash
**Closed Date:** 2025-11-13T15:59:48Z

## Body

```cpp
	constexpr array(std::initializer_list<typename static_array<T, D>::value_type> ilv)
	: static_{
		  (ilv.size() == 0) ? array<T, D>()
							: array<T, D>(ilv.begin(), ilv.end())
	  } {
	}
```

All the details are here, in C++20 mode:
https://gitlab.com/correaa/boost-multi/-/jobs/12070724795

Also happens also with C++23 mode and libc++:
https://gitlab.com/correaa/boost-multi/-/jobs/12070724806

It doesn't happen in C++17 mode: 
https://gitlab.com/correaa/boost-multi/-/jobs/12070724787

Message dump:
```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /usr/bin/clang++-16 -I/builds/correaa/boost-multi/include -g -std=c++20 -ftrivial-auto-var-init=pattern -Wall -Wextra -Wno-error=#warnings -Wno-c++98-compat -Wno-c++98-compat-bind-to-temporary-copy -Wno-c++98-compat-pedantic -Wno-poison-system-directories -Wno-large-by-value-copy -Wstrict-aliasing -Wstrict-aliasing=0 -Wstrict-aliasing=1 -Wstrict-aliasing=2 -Wstrict-overflow -Wstrict-overflow=0 -Wstrict-overflow=1 -Wstrict-overflow=2 -Wstrict-overflow=3 -Wstrict-overflow=4 -Wstrict-overflow=5 -Wno-undefined-reinterpret-cast -Wunknown-pragmas -Wno-c++17-extensions -Wno-padded -Wno-pre-c++17-compat -Werror -MD -MT test/CMakeFiles/allocator.cpp.x.dir/allocator.cpp.o -MF CMakeFiles/allocator.cpp.x.dir/allocator.cpp.o.d -o CMakeFiles/allocator.cpp.x.dir/allocator.cpp.o -c /builds/correaa/boost-multi/test/allocator.cpp
1.	/builds/correaa/boost-multi/test/allocator.cpp:624:4: current parser token ';'
2.	/builds/correaa/boost-multi/test/allocator.cpp:32:20: parsing function body 'main'
3.	/builds/correaa/boost-multi/test/allocator.cpp:32:20: in compound statement ('{}')
4.	/builds/correaa/boost-multi/test/allocator.cpp:620:2: in compound statement ('{}')
5.	/builds/correaa/boost-multi/include/boost/multi/array.hpp:1189:12: instantiating function definition 'boost::multi::array<int, 1>::array'
Stack dump without symbol names (ensure you have llvm-symbolizer in your PATH or set the environment var `LLVM_SYMBOLIZER_PATH` to point to it):
0  libLLVM-16.so.1    0x00007f3e06a63d3f llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) + 63
1  libLLVM-16.so.1    0x00007f3e06a62069 llvm::sys::RunSignalHandlers() + 89
2  libLLVM-16.so.1    0x00007f3e0699cd17
3  libc.so.6          0x00007f3e05649330
4  libclang-cpp.so.16 0x00007f3e0df6413b
5  libclang-cpp.so.16 0x00007f3e0df3c272
6  libclang-cpp.so.16 0x00007f3e0df38357
7  libclang-cpp.so.16 0x00007f3e0df37e29 clang::Expr::EvaluateAsConstantExpr(clang::Expr::EvalResult&, clang::ASTContext const&, clang::Expr::ConstantExprKind) const + 1001
8  libclang-cpp.so.16 0x00007f3e0e50a0d0
9  libclang-cpp.so.16 0x00007f3e0e504e89
10 libclang-cpp.so.16 0x00007f3e0e504b17 clang::Sema::CheckConstraintSatisfaction(clang::NamedDecl const*, llvm::ArrayRef<clang::Expr const*>, llvm::SmallVectorImpl<clang::Expr*>&, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&) + 823
11 libclang-cpp.so.16 0x00007f3e0e505f77 clang::Sema::CheckFunctionConstraints(clang::FunctionDecl const*, clang::ConstraintSatisfaction&, clang::SourceLocation, bool) + 1111
12 libclang-cpp.so.16 0x00007f3e0e68e2a5 clang::Sema::DiagnoseUseOfDecl(clang::NamedDecl*, llvm::ArrayRef<clang::SourceLocation>, clang::ObjCInterfaceDecl const*, bool, bool, clang::ObjCInterfaceDecl*, bool) + 997
13 libclang-cpp.so.16 0x00007f3e0e85c249
14 libclang-cpp.so.16 0x00007f3e0e857ad5 clang::InitializationSequence::Perform(clang::Sema&, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::MutableArrayRef<clang::Expr*>, clang::QualType*) + 8437
15 libclang-cpp.so.16 0x00007f3e0e7adcab clang::Sema::BuildCXXTypeConstructExpr(clang::TypeSourceInfo*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, bool) + 2267
16 libclang-cpp.so.16 0x00007f3e0eafb6f9
17 libclang-cpp.so.16 0x00007f3e0eaf7d81
18 libclang-cpp.so.16 0x00007f3e0eaf568d
19 libclang-cpp.so.16 0x00007f3e0eafdda2
20 libclang-cpp.so.16 0x00007f3e0eaf286e clang::Sema::SubstInitializer(clang::Expr*, clang::MultiLevelTemplateArgumentList const&, bool) + 78
21 libclang-cpp.so.16 0x00007f3e0eb35fde clang::Sema::InstantiateMemInitializers(clang::CXXConstructorDecl*, clang::CXXConstructorDecl const*, clang::MultiLevelTemplateArgumentList const&) + 654
22 libclang-cpp.so.16 0x00007f3e0eb355e0 clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) + 2592
23 libclang-cpp.so.16 0x00007f3e0e7840c7
24 libclang-cpp.so.16 0x00007f3e0e3ce0f0 clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) + 64
25 libclang-cpp.so.16 0x00007f3e0e6cd726 clang::Sema::MarkFunctionReferenced(clang::SourceLocation, clang::FunctionDecl*, bool) + 774
26 libclang-cpp.so.16 0x00007f3e0e61bf66 clang::Sema::BuildCXXConstructExpr(clang::SourceLocation, clang::QualType, clang::CXXConstructorDecl*, bool, llvm::MutableArrayRef<clang::Expr*>, bool, bool, bool, bool, unsigned int, clang::SourceRange) + 86
27 libclang-cpp.so.16 0x00007f3e0e61bd7c clang::Sema::BuildCXXConstructExpr(clang::SourceLocation, clang::QualType, clang::NamedDecl*, clang::CXXConstructorDecl*, llvm::MutableArrayRef<clang::Expr*>, bool, bool, bool, bool, unsigned int, clang::SourceRange) + 348
28 libclang-cpp.so.16 0x00007f3e0e85c02c
29 libclang-cpp.so.16 0x00007f3e0e857ad5 clang::InitializationSequence::Perform(clang::Sema&, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::MutableArrayRef<clang::Expr*>, clang::QualType*) + 8437
30 libclang-cpp.so.16 0x00007f3e0e863bfb
31 libclang-cpp.so.16 0x00007f3e0e862edf
32 libclang-cpp.so.16 0x00007f3e0e860fbd
33 libclang-cpp.so.16 0x00007f3e0e84c468
34 libclang-cpp.so.16 0x00007f3e0e8571e0 clang::InitializationSequence::Perform(clang::Sema&, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::MutableArrayRef<clang::Expr*>, clang::QualType*) + 6144
35 libclang-cpp.so.16 0x00007f3e0e85e48d clang::Sema::PerformCopyInitialization(clang::InitializedEntity const&, clang::SourceLocation, clang::ActionResult<clang::Expr*, true>, bool, bool) + 509
36 libclang-cpp.so.16 0x00007f3e0e6aba2f clang::Sema::GatherArgumentsForCall(clang::SourceLocation, clang::FunctionDecl*, clang::FunctionProtoType const*, unsigned int, llvm::ArrayRef<clang::Expr*>, llvm::SmallVectorImpl<clang::Expr*>&, clang::Sema::VariadicCallType, bool, bool) + 1551
37 libclang-cpp.so.16 0x00007f3e0e61c588 clang::Sema::CompleteConstructorCall(clang::CXXConstructorDecl*, clang::QualType, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, llvm::SmallVectorImpl<clang::Expr*>&, bool, bool) + 296
38 libclang-cpp.so.16 0x00007f3e0e85be4f
39 libclang-cpp.so.16 0x00007f3e0e857ad5 clang::InitializationSequence::Perform(clang::Sema&, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::MutableArrayRef<clang::Expr*>, clang::QualType*) + 8437
40 libclang-cpp.so.16 0x00007f3e0e572cfa clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) + 2266
41 libclang-cpp.so.16 0x00007f3e0dc6321d clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) + 4589
42 libclang-cpp.so.16 0x00007f3e0dc603f8 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::SourceLocation*, clang::Parser::ForRangeInit*) + 1208
43 libclang-cpp.so.16 0x00007f3e0dc5f95b clang::Parser::ParseSimpleDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, bool, clang::Parser::ForRangeInit*, clang::SourceLocation*) + 907
44 libclang-cpp.so.16 0x00007f3e0dc5f4d2 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) + 1234
45 libclang-cpp.so.16 0x00007f3e0dcf0460 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) + 6800
46 libclang-cpp.so.16 0x00007f3e0dcee79e clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) + 334
47 libclang-cpp.so.16 0x00007f3e0dcf7042 clang::Parser::ParseCompoundStatementBody(bool) + 1602
48 libclang-cpp.so.16 0x00007f3e0dcefc21 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) + 4689
49 libclang-cpp.so.16 0x00007f3e0dcee79e clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) + 334
50 libclang-cpp.so.16 0x00007f3e0dcf7042 clang::Parser::ParseCompoundStatementBody(bool) + 1602
51 libclang-cpp.so.16 0x00007f3e0dcf8117 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) + 183
52 libclang-cpp.so.16 0x00007f3e0dd12730 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) + 3520
53 libclang-cpp.so.16 0x00007f3e0dc6114f clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::SourceLocation*, clang::Parser::ForRangeInit*) + 4623
54 libclang-cpp.so.16 0x00007f3e0dd117dc clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) + 1052
55 libclang-cpp.so.16 0x00007f3e0dd111c8 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) + 456
56 libclang-cpp.so.16 0x00007f3e0dd10464 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) + 2036
57 libclang-cpp.so.16 0x00007f3e0dd0e70f clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) + 1199
58 libclang-cpp.so.16 0x00007f3e0dc4cd3e clang::ParseAST(clang::Sema&, bool, bool) + 686
59 libclang-cpp.so.16 0x00007f3e0f8b2dcc clang::FrontendAction::Execute() + 92
60 libclang-cpp.so.16 0x00007f3e0f8328b4 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) + 724
61 libclang-cpp.so.16 0x00007f3e0f92c8d3 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) + 675
62 clang++-16         0x0000557b0fed7c3d cc1_main(llvm::ArrayRef<char const*>, char const*, void*) + 2333
63 clang++-16         0x0000557b0fed4d00
64 libclang-cpp.so.16 0x00007f3e0f50ce0b
65 libLLVM-16.so.1    0x00007f3e0699ca67 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) + 151
66 libclang-cpp.so.16 0x00007f3e0f50c44c clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const + 396
67 libclang-cpp.so.16 0x00007f3e0f4d7783 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const + 883
68 libclang-cpp.so.16 0x00007f3e0f4d79cf clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const + 159
69 libclang-cpp.so.16 0x00007f3e0f4f20f0 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) + 352
70 clang++-16         0x0000557b0fed44bf clang_main(int, char**) + 11759
71 libc.so.6          0x00007f3e0562e1ca
72 libc.so.6          0x00007f3e0562e28b __libc_start_main + 139
73 clang++-16         0x0000557b0fed12f5 _start + 37
clang: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Ubuntu clang version 16.0.6 (23ubuntu4)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
clang: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang: note: diagnostic msg: /tmp/allocator-72d48c.cpp
clang: note: diagnostic msg: /tmp/allocator-72d48c.sh
clang: note: diagnostic msg: 

********************
```

## Comments

### Comment 1 - EugeneZelenko

Could you please try 21 or `main` branch? https://godbolt.org should be helpful. Only most recent release is maintained.

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Alfredo Correa (correaa)

<details>
```cpp
	constexpr array(std::initializer_list&lt;typename static_array&lt;T, D&gt;::value_type&gt; ilv)
	: static_{
		  (ilv.size() == 0) ? array&lt;T, D&gt;()
							: array&lt;T, D&gt;(ilv.begin(), ilv.end())
	  } {
	}
```

All the details are here, in C++20 mode:
https://gitlab.com/correaa/boost-multi/-/jobs/12070724795

Also happens also with C++23 mode and libc++:
https://gitlab.com/correaa/boost-multi/-/jobs/12070724806

It doesn't happen in C++17 mode: 
https://gitlab.com/correaa/boost-multi/-/jobs/12070724787

Message dump:
```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /usr/bin/clang++-16 -I/builds/correaa/boost-multi/include -g -std=c++20 -ftrivial-auto-var-init=pattern -Wall -Wextra -Wno-error=#warnings -Wno-c++98-compat -Wno-c++98-compat-bind-to-temporary-copy -Wno-c++98-compat-pedantic -Wno-poison-system-directories -Wno-large-by-value-copy -Wstrict-aliasing -Wstrict-aliasing=0 -Wstrict-aliasing=1 -Wstrict-aliasing=2 -Wstrict-overflow -Wstrict-overflow=0 -Wstrict-overflow=1 -Wstrict-overflow=2 -Wstrict-overflow=3 -Wstrict-overflow=4 -Wstrict-overflow=5 -Wno-undefined-reinterpret-cast -Wunknown-pragmas -Wno-c++17-extensions -Wno-padded -Wno-pre-c++17-compat -Werror -MD -MT test/CMakeFiles/allocator.cpp.x.dir/allocator.cpp.o -MF CMakeFiles/allocator.cpp.x.dir/allocator.cpp.o.d -o CMakeFiles/allocator.cpp.x.dir/allocator.cpp.o -c /builds/correaa/boost-multi/test/allocator.cpp
1.	/builds/correaa/boost-multi/test/allocator.cpp:624:4: current parser token ';'
2.	/builds/correaa/boost-multi/test/allocator.cpp:32:20: parsing function body 'main'
3.	/builds/correaa/boost-multi/test/allocator.cpp:32:20: in compound statement ('{}')
4.	/builds/correaa/boost-multi/test/allocator.cpp:620:2: in compound statement ('{}')
5.	/builds/correaa/boost-multi/include/boost/multi/array.hpp:1189:12: instantiating function definition 'boost::multi::array&lt;int, 1&gt;::array'
Stack dump without symbol names (ensure you have llvm-symbolizer in your PATH or set the environment var `LLVM_SYMBOLIZER_PATH` to point to it):
0  libLLVM-16.so.1    0x00007f3e06a63d3f llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) + 63
1  libLLVM-16.so.1    0x00007f3e06a62069 llvm::sys::RunSignalHandlers() + 89
2  libLLVM-16.so.1    0x00007f3e0699cd17
3  libc.so.6          0x00007f3e05649330
4  libclang-cpp.so.16 0x00007f3e0df6413b
5  libclang-cpp.so.16 0x00007f3e0df3c272
6  libclang-cpp.so.16 0x00007f3e0df38357
7  libclang-cpp.so.16 0x00007f3e0df37e29 clang::Expr::EvaluateAsConstantExpr(clang::Expr::EvalResult&amp;, clang::ASTContext const&amp;, clang::Expr::ConstantExprKind) const + 1001
8  libclang-cpp.so.16 0x00007f3e0e50a0d0
9  libclang-cpp.so.16 0x00007f3e0e504e89
10 libclang-cpp.so.16 0x00007f3e0e504b17 clang::Sema::CheckConstraintSatisfaction(clang::NamedDecl const*, llvm::ArrayRef&lt;clang::Expr const*&gt;, llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceRange, clang::ConstraintSatisfaction&amp;) + 823
11 libclang-cpp.so.16 0x00007f3e0e505f77 clang::Sema::CheckFunctionConstraints(clang::FunctionDecl const*, clang::ConstraintSatisfaction&amp;, clang::SourceLocation, bool) + 1111
12 libclang-cpp.so.16 0x00007f3e0e68e2a5 clang::Sema::DiagnoseUseOfDecl(clang::NamedDecl*, llvm::ArrayRef&lt;clang::SourceLocation&gt;, clang::ObjCInterfaceDecl const*, bool, bool, clang::ObjCInterfaceDecl*, bool) + 997
13 libclang-cpp.so.16 0x00007f3e0e85c249
14 libclang-cpp.so.16 0x00007f3e0e857ad5 clang::InitializationSequence::Perform(clang::Sema&amp;, clang::InitializedEntity const&amp;, clang::InitializationKind const&amp;, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::QualType*) + 8437
15 libclang-cpp.so.16 0x00007f3e0e7adcab clang::Sema::BuildCXXTypeConstructExpr(clang::TypeSourceInfo*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, bool) + 2267
16 libclang-cpp.so.16 0x00007f3e0eafb6f9
17 libclang-cpp.so.16 0x00007f3e0eaf7d81
18 libclang-cpp.so.16 0x00007f3e0eaf568d
19 libclang-cpp.so.16 0x00007f3e0eafdda2
20 libclang-cpp.so.16 0x00007f3e0eaf286e clang::Sema::SubstInitializer(clang::Expr*, clang::MultiLevelTemplateArgumentList const&amp;, bool) + 78
21 libclang-cpp.so.16 0x00007f3e0eb35fde clang::Sema::InstantiateMemInitializers(clang::CXXConstructorDecl*, clang::CXXConstructorDecl const*, clang::MultiLevelTemplateArgumentList const&amp;) + 654
22 libclang-cpp.so.16 0x00007f3e0eb355e0 clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) + 2592
23 libclang-cpp.so.16 0x00007f3e0e7840c7
24 libclang-cpp.so.16 0x00007f3e0e3ce0f0 clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) + 64
25 libclang-cpp.so.16 0x00007f3e0e6cd726 clang::Sema::MarkFunctionReferenced(clang::SourceLocation, clang::FunctionDecl*, bool) + 774
26 libclang-cpp.so.16 0x00007f3e0e61bf66 clang::Sema::BuildCXXConstructExpr(clang::SourceLocation, clang::QualType, clang::CXXConstructorDecl*, bool, llvm::MutableArrayRef&lt;clang::Expr*&gt;, bool, bool, bool, bool, unsigned int, clang::SourceRange) + 86
27 libclang-cpp.so.16 0x00007f3e0e61bd7c clang::Sema::BuildCXXConstructExpr(clang::SourceLocation, clang::QualType, clang::NamedDecl*, clang::CXXConstructorDecl*, llvm::MutableArrayRef&lt;clang::Expr*&gt;, bool, bool, bool, bool, unsigned int, clang::SourceRange) + 348
28 libclang-cpp.so.16 0x00007f3e0e85c02c
29 libclang-cpp.so.16 0x00007f3e0e857ad5 clang::InitializationSequence::Perform(clang::Sema&amp;, clang::InitializedEntity const&amp;, clang::InitializationKind const&amp;, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::QualType*) + 8437
30 libclang-cpp.so.16 0x00007f3e0e863bfb
31 libclang-cpp.so.16 0x00007f3e0e862edf
32 libclang-cpp.so.16 0x00007f3e0e860fbd
33 libclang-cpp.so.16 0x00007f3e0e84c468
34 libclang-cpp.so.16 0x00007f3e0e8571e0 clang::InitializationSequence::Perform(clang::Sema&amp;, clang::InitializedEntity const&amp;, clang::InitializationKind const&amp;, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::QualType*) + 6144
35 libclang-cpp.so.16 0x00007f3e0e85e48d clang::Sema::PerformCopyInitialization(clang::InitializedEntity const&amp;, clang::SourceLocation, clang::ActionResult&lt;clang::Expr*, true&gt;, bool, bool) + 509
36 libclang-cpp.so.16 0x00007f3e0e6aba2f clang::Sema::GatherArgumentsForCall(clang::SourceLocation, clang::FunctionDecl*, clang::FunctionProtoType const*, unsigned int, llvm::ArrayRef&lt;clang::Expr*&gt;, llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, clang::Sema::VariadicCallType, bool, bool) + 1551
37 libclang-cpp.so.16 0x00007f3e0e61c588 clang::Sema::CompleteConstructorCall(clang::CXXConstructorDecl*, clang::QualType, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, bool, bool) + 296
38 libclang-cpp.so.16 0x00007f3e0e85be4f
39 libclang-cpp.so.16 0x00007f3e0e857ad5 clang::InitializationSequence::Perform(clang::Sema&amp;, clang::InitializedEntity const&amp;, clang::InitializationKind const&amp;, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::QualType*) + 8437
40 libclang-cpp.so.16 0x00007f3e0e572cfa clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) + 2266
41 libclang-cpp.so.16 0x00007f3e0dc6321d clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) + 4589
42 libclang-cpp.so.16 0x00007f3e0dc603f8 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) + 1208
43 libclang-cpp.so.16 0x00007f3e0dc5f95b clang::Parser::ParseSimpleDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, bool, clang::Parser::ForRangeInit*, clang::SourceLocation*) + 907
44 libclang-cpp.so.16 0x00007f3e0dc5f4d2 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) + 1234
45 libclang-cpp.so.16 0x00007f3e0dcf0460 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) + 6800
46 libclang-cpp.so.16 0x00007f3e0dcee79e clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) + 334
47 libclang-cpp.so.16 0x00007f3e0dcf7042 clang::Parser::ParseCompoundStatementBody(bool) + 1602
48 libclang-cpp.so.16 0x00007f3e0dcefc21 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) + 4689
49 libclang-cpp.so.16 0x00007f3e0dcee79e clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) + 334
50 libclang-cpp.so.16 0x00007f3e0dcf7042 clang::Parser::ParseCompoundStatementBody(bool) + 1602
51 libclang-cpp.so.16 0x00007f3e0dcf8117 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) + 183
52 libclang-cpp.so.16 0x00007f3e0dd12730 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) + 3520
53 libclang-cpp.so.16 0x00007f3e0dc6114f clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) + 4623
54 libclang-cpp.so.16 0x00007f3e0dd117dc clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) + 1052
55 libclang-cpp.so.16 0x00007f3e0dd111c8 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) + 456
56 libclang-cpp.so.16 0x00007f3e0dd10464 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) + 2036
57 libclang-cpp.so.16 0x00007f3e0dd0e70f clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) + 1199
58 libclang-cpp.so.16 0x00007f3e0dc4cd3e clang::ParseAST(clang::Sema&amp;, bool, bool) + 686
59 libclang-cpp.so.16 0x00007f3e0f8b2dcc clang::FrontendAction::Execute() + 92
60 libclang-cpp.so.16 0x00007f3e0f8328b4 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) + 724
61 libclang-cpp.so.16 0x00007f3e0f92c8d3 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) + 675
62 clang++-16         0x0000557b0fed7c3d cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) + 2333
63 clang++-16         0x0000557b0fed4d00
64 libclang-cpp.so.16 0x00007f3e0f50ce0b
65 libLLVM-16.so.1    0x00007f3e0699ca67 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) + 151
66 libclang-cpp.so.16 0x00007f3e0f50c44c clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const + 396
67 libclang-cpp.so.16 0x00007f3e0f4d7783 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const + 883
68 libclang-cpp.so.16 0x00007f3e0f4d79cf clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const + 159
69 libclang-cpp.so.16 0x00007f3e0f4f20f0 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) + 352
70 clang++-16         0x0000557b0fed44bf clang_main(int, char**) + 11759
71 libc.so.6          0x00007f3e0562e1ca
72 libc.so.6          0x00007f3e0562e28b __libc_start_main + 139
73 clang++-16         0x0000557b0fed12f5 _start + 37
clang: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Ubuntu clang version 16.0.6 (23ubuntu4)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
clang: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang: note: diagnostic msg: /tmp/allocator-72d48c.cpp
clang: note: diagnostic msg: /tmp/allocator-72d48c.sh
clang: note: diagnostic msg: 

********************
```
</details>


---

### Comment 3 - correaa

> Could you please try 21 or `main` branch? https://godbolt.org should be helpful. Only most recent release is maintained.

Sure, with all others compilers it works: https://github.com/correaa/boost-multi/actions/runs/19318812991/job/55255694156#step:8:230

https://gitlab.com/correaa/boost-multi/-/pipelines/2155821554

Here it is another CI run with more details https://github.com/correaa/boost-multi/actions/runs/19318812991/job/55255694156#step:8:230

The code already have some specific path to workarounds problems with std::ranges, but it doesn't seem to be related.

---

### Comment 4 - correaa

Here is a reproduction of the problem in Compiler Explorer clang 16 (also there is a clang 21 panel)
https://godbolt.org/z/h7h4zoW9o

EDIT: distilled it to this: https://godbolt.org/z/fnn4or8n8

---

### Comment 5 - EugeneZelenko

21 seems to compile fine. Same for `main`: https://godbolt.org/z/TP51EvcqP

---

### Comment 6 - correaa

clang 15 also compiles fine 9as shown in my last CE link)

---

### Comment 7 - correaa

sorry, this was a different bug

<s>Thanks, I discovered that these marked lines ** are producing the problem, so it has to do with `requires` or with `std::sentinel_for`.

```
#if defined(__cpp_lib_ranges) && (__cpp_lib_ranges >= 201911L)  //  && !defined(_MSC_VER)
* 	// TODO(correaa) add sentinel version
* 	template<class It, class Sentinel, class = typename std::iterator_traits<std::decay_t<It>>::difference_type>
* 	constexpr explicit static_array(It const& first, Sentinel const& last)
*		requires std::sentinel_for<Sentinel, It>
* 	: static_array(first, last, allocator_type{}) {}
#else
	template<class It, class = typename std::iterator_traits<std::decay_t<It>>::difference_type>
	constexpr explicit static_array(It const& first, It const& last) : static_array(first, last, allocator_type{}) {}
#endif
``
`</s>




---

