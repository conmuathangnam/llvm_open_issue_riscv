# [clang][20.x regression] Crash during code completion when using PCH

**Author:** HighCommander4
**URL:** https://github.com/llvm/llvm-project/issues/139486
**Status:** Closed
**Labels:** duplicate, clang:frontend, regression:20
**Closed Date:** 2025-05-12T02:25:07Z

## Body

Given the following files:

`test.h`:
```cpp
#pragma once
class provider_t
{
  public:
    template<class T>
    void emit(T *data)
    {}
};
```

`test.cpp`:
```cpp
#include "test.h"

void test()
{
    provider_t *focus;
    void *data;
    focus->emit(&data);
}
```

Run:

```console
$ clang -cc1 -xc++-header test.h -emit-pch -o test.h.pch
$ clang -cc1 -include-pch test.h.pch -code-completion-at=test.cpp:7:17 test.cpp
```

The second command prints:

```console
Expected valid TemplateArgument
UNREACHABLE executed at llvm/src/clang/lib/Serialization/TemplateArgumentHasher.cpp:68!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: clang-dev -cc1 -include-pch test.h.pch -code-completion-at=test.cpp:7:17 test.cpp
1.      <eof> parser at end of file
2.      test.cpp:4:1: parsing function body 'test'
3.      test.cpp:4:1: in compound statement ('{}')
Stack dump without symbol names (ensure you have llvm-symbolizer in your PATH or set the environment var `LLVM_SYMBOLIZER_PATH` to point to it):
0  libLLVMSupport.so.21.0git        0x00007fabf087aadd llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) + 61
1  libLLVMSupport.so.21.0git        0x00007fabf087b05b
2  libLLVMSupport.so.21.0git        0x00007fabf0879056 llvm::sys::RunSignalHandlers() + 134
3  libLLVMSupport.so.21.0git        0x00007fabf087b80d
4  libpthread.so.0                  0x00007fabf9b2b140
5  libc.so.6                        0x00007fabeff6ad51 gsignal + 321
6  libc.so.6                        0x00007fabeff54537 abort + 291
7  libLLVMSupport.so.21.0git        0x00007fabf072fe20 llvm::install_out_of_memory_new_handler() + 0
8  libclangSerialization.so.21.0git 0x00007fabf4a20792
9  libclangSerialization.so.21.0git 0x00007fabf4a20668 clang::serialization::StableHashForTemplateArguments(llvm::ArrayRef<clang::TemplateArgument>) + 248
10 libclangSerialization.so.21.0git 0x00007fabf46c1b97 clang::ASTReader::LoadExternalSpecializationsImpl(llvm::DenseMap<clang::Decl const*, clang::serialization::reader::LazySpecializationInfoLookupTable, llvm::DenseMapInfo<clang::Decl const*, void>, llvm::detail::DenseMapPair<clang::Decl const*, clang::serialization::reader::LazySpecializationInfoLookupTable>>&, clang::Decl const*, llvm::ArrayRef<clang::TemplateArgument>) + 215
11 libclangSerialization.so.21.0git 0x00007fabf46c1d92 clang::ASTReader::LoadExternalSpecializations(clang::Decl const*, llvm::ArrayRef<clang::TemplateArgument>) + 178
12 libclangAST.so.21.0git           0x00007fabecf4f710 clang::RedeclarableTemplateDecl::loadLazySpecializationsImpl(llvm::ArrayRef<clang::TemplateArgument>, clang::TemplateParameterList*) const + 176
13 libclangAST.so.21.0git           0x00007fabecf56335 clang::RedeclarableTemplateDecl::SpecEntryTraits<clang::FunctionTemplateSpecializationInfo>::DeclType* clang::RedeclarableTemplateDecl::findSpecializationImpl<clang::FunctionTemplateSpecializationInfo, llvm::ArrayRef<clang::TemplateArgument>&>(llvm::FoldingSetVector<clang::FunctionTemplateSpecializationInfo, llvm::SmallVector<clang::FunctionTemplateSpecializationInfo*, 8u>>&, void*&, llvm::ArrayRef<clang::TemplateArgument>&) + 117
14 libclangAST.so.21.0git           0x00007fabecf4fa42 clang::FunctionTemplateDecl::findSpecialization(llvm::ArrayRef<clang::TemplateArgument>, void*&) + 66
15 libclangSema.so.21.0git          0x00007fabea9f1fc9 clang::TemplateDeclInstantiator::VisitCXXMethodDecl(clang::CXXMethodDecl*, clang::TemplateParameterList*, clang::TemplateDeclInstantiator::RewriteKind) + 153
16 libclangSema.so.21.0git          0x00007fabea9f79f3 clang::TemplateDeclInstantiator::VisitCXXMethodDecl(clang::CXXMethodDecl*) + 35
17 libclangSema.so.21.0git          0x00007fabea9d4373
18 libclangSema.so.21.0git          0x00007fabeaa7a784
19 libclangSema.so.21.0git          0x00007fabeaa7a755
20 libclangBasic.so.21.0git         0x00007fabf41da089
21 libclangBasic.so.21.0git         0x00007fabf41da29d
22 libclangBasic.so.21.0git         0x00007fabf41da0f0 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) + 96
23 libclangSema.so.21.0git          0x00007fabe98a8a98 clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) + 72
24 libclangSema.so.21.0git          0x00007fabea9fdec7 clang::Sema::SubstDecl(clang::Decl*, clang::DeclContext*, clang::MultiLevelTemplateArgumentList const&) + 215
25 libclangSema.so.21.0git          0x00007fabea82c391 clang::Sema::FinishTemplateArgumentDeduction(clang::FunctionTemplateDecl*, llvm::SmallVectorImpl<clang::DeducedTemplateArgument>&, unsigned int, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, llvm::SmallVectorImpl<clang::Sema::OriginalCallArg> const*, bool, bool, llvm::function_ref<bool ()>) + 1201
26 libclangSema.so.21.0git          0x00007fabea8ba48c
27 libclangSema.so.21.0git          0x00007fabea8ba395
28 libclangBasic.so.21.0git         0x00007fabf41da089
29 libclangBasic.so.21.0git         0x00007fabf41da29d
30 libclangBasic.so.21.0git         0x00007fabf41da0f0 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) + 96
31 libclangSema.so.21.0git          0x00007fabe98a8a98 clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) + 72
32 libclangSema.so.21.0git          0x00007fabea82ec04 clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, bool, bool, clang::QualType, clang::Expr::Classification, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>)>) + 3140
33 libclangSema.so.21.0git          0x00007fabea4e6a81
34 libclangSema.so.21.0git          0x00007fabea4e6240 clang::Sema::AddMethodTemplateCandidate(clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::CXXRecordDecl*, clang::TemplateArgumentListInfo*, clang::QualType, clang::Expr::Classification, llvm::ArrayRef<clang::Expr*>, clang::OverloadCandidateSet&, bool, bool, clang::OverloadCandidateParamOrder) + 352
35 libclangSema.so.21.0git          0x00007fabea4e5cc7 clang::Sema::AddFunctionCandidates(clang::UnresolvedSetImpl const&, llvm::ArrayRef<clang::Expr*>, clang::OverloadCandidateSet&, clang::TemplateArgumentListInfo*, bool, bool, bool) + 823
36 libclangSema.so.21.0git          0x00007fabe9a840a7 clang::SemaCodeCompletion::ProduceCallSignatureHelp(clang::Expr*, llvm::ArrayRef<clang::Expr*>, clang::SourceLocation) + 983
37 libclangParse.so.21.0git         0x00007fabead3ba59
38 libclangParse.so.21.0git         0x00007fabead3e0d5
39 libclangParse.so.21.0git         0x00007fabead3fff9
40 libclangParse.so.21.0git         0x00007fabead3e20a
41 libclangParse.so.21.0git         0x00007fabead2abe0 clang::Parser::ParseAssignmentExpression(clang::TypeCastState) + 128
42 libclangParse.so.21.0git         0x00007fabead3b555 clang::Parser::ParseExpressionList(llvm::SmallVectorImpl<clang::Expr*>&, llvm::function_ref<void ()>, bool, bool) + 277
43 libclangParse.so.21.0git         0x00007fabead2dd9a clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult<clang::Expr*, true>) + 5354
44 libclangParse.so.21.0git         0x00007fabead33861 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypeCastState, bool, bool*) + 14209
45 libclangParse.so.21.0git         0x00007fabead2c6dc clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypeCastState, bool, bool*) + 92
46 libclangParse.so.21.0git         0x00007fabead2ac81 clang::Parser::ParseAssignmentExpression(clang::TypeCastState) + 289
47 libclangParse.so.21.0git         0x00007fabead2ab2f clang::Parser::ParseExpression(clang::TypeCastState) + 31
48 libclangParse.so.21.0git         0x00007fabeaddf978 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) + 88
49 libclangParse.so.21.0git         0x00007fabeadddbeb clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) + 1995
50 libclangParse.so.21.0git         0x00007fabeaddd2aa clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) + 266
51 libclangParse.so.21.0git         0x00007fabeade7049 clang::Parser::ParseCompoundStatementBody(bool) + 1289
52 libclangParse.so.21.0git         0x00007fabeade8417 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) + 343
53 libclangParse.so.21.0git         0x00007fabeae12da0 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) + 4400
54 libclangParse.so.21.0git         0x00007fabeacbf9e5 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) + 2501
55 libclangParse.so.21.0git         0x00007fabeae11b5c clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) + 1916
56 libclangParse.so.21.0git         0x00007fabeae1103e clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) + 238
57 libclangParse.so.21.0git         0x00007fabeae108b8 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) + 4216
58 libclangParse.so.21.0git         0x00007fabeae0e65b clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) + 2171
59 libclangParse.so.21.0git         0x00007fabeae0dd50 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) + 64
60 libclangParse.so.21.0git         0x00007fabeac96011 clang::ParseAST(clang::Sema&, bool, bool) + 513
61 libclangFrontend.so.21.0git      0x00007fabf55084b3 clang::ASTFrontendAction::ExecuteAction() + 307
62 libclangFrontend.so.21.0git      0x00007fabf5507e96 clang::FrontendAction::Execute() + 54
63 libclangFrontend.so.21.0git      0x00007fabf53f6b5e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) + 974
64 libclangFrontendTool.so.21.0git  0x00007fabf9b0045e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) + 926
65 clang-dev                        0x00005645c5ab9ff0 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) + 1792
66 clang-dev                        0x00005645c5aac512
67 clang-dev                        0x00005645c5aab25d clang_main(int, char**, llvm::ToolContext const&) + 765
68 clang-dev                        0x00005645c5ae1c15 main + 101
69 libc.so.6                        0x00007fabeff55d7a __libc_start_main + 234
70 clang-dev                        0x00005645c5aaacea _start + 42
Aborted
```

Originally reported at https://github.com/clangd/clangd/issues/2387.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Nathan Ridge (HighCommander4)

<details>
Given the following files:

`test.h`:
```cpp
#pragma once
class provider_t
{
  public:
    template&lt;class T&gt;
    void emit(T *data)
    {}
};
```

`test.cpp`:
```cpp
#include "test.h"

void test()
{
    provider_t *focus;
    void *data;
    focus-&gt;emit(&amp;data);
}
```

Run:

```console
$ clang -cc1 -xc++-header test.h -emit-pch -o test.h.pch
$ clang -cc1 -include-pch test.h.pch -code-completion-at=test.cpp:7:17 test.cpp
```

The second command prints:

```console
Expected valid TemplateArgument
UNREACHABLE executed at llvm/src/clang/lib/Serialization/TemplateArgumentHasher.cpp:68!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: clang-dev -cc1 -include-pch test.h.pch -code-completion-at=test.cpp:7:17 test.cpp
1.      &lt;eof&gt; parser at end of file
2.      test.cpp:4:1: parsing function body 'test'
3.      test.cpp:4:1: in compound statement ('{}')
Stack dump without symbol names (ensure you have llvm-symbolizer in your PATH or set the environment var `LLVM_SYMBOLIZER_PATH` to point to it):
0  libLLVMSupport.so.21.0git        0x00007fabf087aadd llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) + 61
1  libLLVMSupport.so.21.0git        0x00007fabf087b05b
2  libLLVMSupport.so.21.0git        0x00007fabf0879056 llvm::sys::RunSignalHandlers() + 134
3  libLLVMSupport.so.21.0git        0x00007fabf087b80d
4  libpthread.so.0                  0x00007fabf9b2b140
5  libc.so.6                        0x00007fabeff6ad51 gsignal + 321
6  libc.so.6                        0x00007fabeff54537 abort + 291
7  libLLVMSupport.so.21.0git        0x00007fabf072fe20 llvm::install_out_of_memory_new_handler() + 0
8  libclangSerialization.so.21.0git 0x00007fabf4a20792
9  libclangSerialization.so.21.0git 0x00007fabf4a20668 clang::serialization::StableHashForTemplateArguments(llvm::ArrayRef&lt;clang::TemplateArgument&gt;) + 248
10 libclangSerialization.so.21.0git 0x00007fabf46c1b97 clang::ASTReader::LoadExternalSpecializationsImpl(llvm::DenseMap&lt;clang::Decl const*, clang::serialization::reader::LazySpecializationInfoLookupTable, llvm::DenseMapInfo&lt;clang::Decl const*, void&gt;, llvm::detail::DenseMapPair&lt;clang::Decl const*, clang::serialization::reader::LazySpecializationInfoLookupTable&gt;&gt;&amp;, clang::Decl const*, llvm::ArrayRef&lt;clang::TemplateArgument&gt;) + 215
11 libclangSerialization.so.21.0git 0x00007fabf46c1d92 clang::ASTReader::LoadExternalSpecializations(clang::Decl const*, llvm::ArrayRef&lt;clang::TemplateArgument&gt;) + 178
12 libclangAST.so.21.0git           0x00007fabecf4f710 clang::RedeclarableTemplateDecl::loadLazySpecializationsImpl(llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::TemplateParameterList*) const + 176
13 libclangAST.so.21.0git           0x00007fabecf56335 clang::RedeclarableTemplateDecl::SpecEntryTraits&lt;clang::FunctionTemplateSpecializationInfo&gt;::DeclType* clang::RedeclarableTemplateDecl::findSpecializationImpl&lt;clang::FunctionTemplateSpecializationInfo, llvm::ArrayRef&lt;clang::TemplateArgument&gt;&amp;&gt;(llvm::FoldingSetVector&lt;clang::FunctionTemplateSpecializationInfo, llvm::SmallVector&lt;clang::FunctionTemplateSpecializationInfo*, 8u&gt;&gt;&amp;, void*&amp;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;&amp;) + 117
14 libclangAST.so.21.0git           0x00007fabecf4fa42 clang::FunctionTemplateDecl::findSpecialization(llvm::ArrayRef&lt;clang::TemplateArgument&gt;, void*&amp;) + 66
15 libclangSema.so.21.0git          0x00007fabea9f1fc9 clang::TemplateDeclInstantiator::VisitCXXMethodDecl(clang::CXXMethodDecl*, clang::TemplateParameterList*, clang::TemplateDeclInstantiator::RewriteKind) + 153
16 libclangSema.so.21.0git          0x00007fabea9f79f3 clang::TemplateDeclInstantiator::VisitCXXMethodDecl(clang::CXXMethodDecl*) + 35
17 libclangSema.so.21.0git          0x00007fabea9d4373
18 libclangSema.so.21.0git          0x00007fabeaa7a784
19 libclangSema.so.21.0git          0x00007fabeaa7a755
20 libclangBasic.so.21.0git         0x00007fabf41da089
21 libclangBasic.so.21.0git         0x00007fabf41da29d
22 libclangBasic.so.21.0git         0x00007fabf41da0f0 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) + 96
23 libclangSema.so.21.0git          0x00007fabe98a8a98 clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) + 72
24 libclangSema.so.21.0git          0x00007fabea9fdec7 clang::Sema::SubstDecl(clang::Decl*, clang::DeclContext*, clang::MultiLevelTemplateArgumentList const&amp;) + 215
25 libclangSema.so.21.0git          0x00007fabea82c391 clang::Sema::FinishTemplateArgumentDeduction(clang::FunctionTemplateDecl*, llvm::SmallVectorImpl&lt;clang::DeducedTemplateArgument&gt;&amp;, unsigned int, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, llvm::SmallVectorImpl&lt;clang::Sema::OriginalCallArg&gt; const*, bool, bool, llvm::function_ref&lt;bool ()&gt;) + 1201
26 libclangSema.so.21.0git          0x00007fabea8ba48c
27 libclangSema.so.21.0git          0x00007fabea8ba395
28 libclangBasic.so.21.0git         0x00007fabf41da089
29 libclangBasic.so.21.0git         0x00007fabf41da29d
30 libclangBasic.so.21.0git         0x00007fabf41da0f0 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) + 96
31 libclangSema.so.21.0git          0x00007fabe98a8a98 clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) + 72
32 libclangSema.so.21.0git          0x00007fabea82ec04 clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, bool, bool, bool, clang::QualType, clang::Expr::Classification, llvm::function_ref&lt;bool (llvm::ArrayRef&lt;clang::QualType&gt;)&gt;) + 3140
33 libclangSema.so.21.0git          0x00007fabea4e6a81
34 libclangSema.so.21.0git          0x00007fabea4e6240 clang::Sema::AddMethodTemplateCandidate(clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::CXXRecordDecl*, clang::TemplateArgumentListInfo*, clang::QualType, clang::Expr::Classification, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::OverloadCandidateSet&amp;, bool, bool, clang::OverloadCandidateParamOrder) + 352
35 libclangSema.so.21.0git          0x00007fabea4e5cc7 clang::Sema::AddFunctionCandidates(clang::UnresolvedSetImpl const&amp;, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::OverloadCandidateSet&amp;, clang::TemplateArgumentListInfo*, bool, bool, bool) + 823
36 libclangSema.so.21.0git          0x00007fabe9a840a7 clang::SemaCodeCompletion::ProduceCallSignatureHelp(clang::Expr*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation) + 983
37 libclangParse.so.21.0git         0x00007fabead3ba59
38 libclangParse.so.21.0git         0x00007fabead3e0d5
39 libclangParse.so.21.0git         0x00007fabead3fff9
40 libclangParse.so.21.0git         0x00007fabead3e20a
41 libclangParse.so.21.0git         0x00007fabead2abe0 clang::Parser::ParseAssignmentExpression(clang::TypeCastState) + 128
42 libclangParse.so.21.0git         0x00007fabead3b555 clang::Parser::ParseExpressionList(llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, llvm::function_ref&lt;void ()&gt;, bool, bool) + 277
43 libclangParse.so.21.0git         0x00007fabead2dd9a clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult&lt;clang::Expr*, true&gt;) + 5354
44 libclangParse.so.21.0git         0x00007fabead33861 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&amp;, clang::TypeCastState, bool, bool*) + 14209
45 libclangParse.so.21.0git         0x00007fabead2c6dc clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypeCastState, bool, bool*) + 92
46 libclangParse.so.21.0git         0x00007fabead2ac81 clang::Parser::ParseAssignmentExpression(clang::TypeCastState) + 289
47 libclangParse.so.21.0git         0x00007fabead2ab2f clang::Parser::ParseExpression(clang::TypeCastState) + 31
48 libclangParse.so.21.0git         0x00007fabeaddf978 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) + 88
49 libclangParse.so.21.0git         0x00007fabeadddbeb clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) + 1995
50 libclangParse.so.21.0git         0x00007fabeaddd2aa clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) + 266
51 libclangParse.so.21.0git         0x00007fabeade7049 clang::Parser::ParseCompoundStatementBody(bool) + 1289
52 libclangParse.so.21.0git         0x00007fabeade8417 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) + 343
53 libclangParse.so.21.0git         0x00007fabeae12da0 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) + 4400
54 libclangParse.so.21.0git         0x00007fabeacbf9e5 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) + 2501
55 libclangParse.so.21.0git         0x00007fabeae11b5c clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) + 1916
56 libclangParse.so.21.0git         0x00007fabeae1103e clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) + 238
57 libclangParse.so.21.0git         0x00007fabeae108b8 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) + 4216
58 libclangParse.so.21.0git         0x00007fabeae0e65b clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) + 2171
59 libclangParse.so.21.0git         0x00007fabeae0dd50 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) + 64
60 libclangParse.so.21.0git         0x00007fabeac96011 clang::ParseAST(clang::Sema&amp;, bool, bool) + 513
61 libclangFrontend.so.21.0git      0x00007fabf55084b3 clang::ASTFrontendAction::ExecuteAction() + 307
62 libclangFrontend.so.21.0git      0x00007fabf5507e96 clang::FrontendAction::Execute() + 54
63 libclangFrontend.so.21.0git      0x00007fabf53f6b5e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) + 974
64 libclangFrontendTool.so.21.0git  0x00007fabf9b0045e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) + 926
65 clang-dev                        0x00005645c5ab9ff0 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) + 1792
66 clang-dev                        0x00005645c5aac512
67 clang-dev                        0x00005645c5aab25d clang_main(int, char**, llvm::ToolContext const&amp;) + 765
68 clang-dev                        0x00005645c5ae1c15 main + 101
69 libc.so.6                        0x00007fabeff55d7a __libc_start_main + 234
70 clang-dev                        0x00005645c5aaacea _start + 42
Aborted
```
</details>


---

### Comment 2 - HighCommander4

I bisected this to https://github.com/llvm/llvm-project/commit/20e904950967c12.

cc @ChuanqiXu9 as the author of the regressing patch.

(This regression window was brought to you by [manyclangs](https://github.com/elfshaker/manyclangs)).

---

### Comment 3 - HighCommander4

And there is a potential diagnosis in [this comment](https://github.com/clangd/clangd/issues/2387#issuecomment-2870427641):

> it seems to me that this code from the commit you've posted has undefined behaviour because of the multiple `std::forward` uses on the same `ProfileArgs`:
> 
> ```
> template <class EntryType, typename... ProfileArguments>
> typename RedeclarableTemplateDecl::SpecEntryTraits<EntryType>::DeclType *
> RedeclarableTemplateDecl::findSpecializationImpl(
>     llvm::FoldingSetVector<EntryType> &Specs, void *&InsertPos,
>     ProfileArguments &&...ProfileArgs) {
> 
>   if (auto *Found = findSpecializationLocally(
>           Specs, InsertPos, std::forward<ProfileArguments>(ProfileArgs)...))
>     return Found;
> 
>   if (!loadLazySpecializationsImpl(
>           std::forward<ProfileArguments>(ProfileArgs)...))
>     return nullptr;
> 
>   return findSpecializationLocally(
>       Specs, InsertPos, std::forward<ProfileArguments>(ProfileArgs)...);
> }
> ```



---

### Comment 4 - HighCommander4

> Run:
> 
> $ clang -cc1 -xc++-header test.h -emit-pch -o test.h.pch
> $ clang -cc1 -include-pch test.h.pch -code-completion-at=test.cpp:7:17 test.cpp

(This also affects clangd, which takes roughly the same codepaths as these commands when code completion or signature help is invoked at the specified location.)

---

### Comment 5 - ChuanqiXu9

I feel this is duplicated with https://github.com/llvm/llvm-project/issues/139019

---

