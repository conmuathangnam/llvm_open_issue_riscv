# clangd Crashes while editing `std::find` arguments.

**Author:** anny-moose
**URL:** https://github.com/llvm/llvm-project/issues/139366
**Status:** Closed
**Labels:** duplicate, clangd, clang:frontend, confirmed, crash
**Closed Date:** 2025-05-21T07:23:32Z

## Body

Crash backtrace:

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.	<eof> parser at end of file
1.	/home/dmytro/Dev/app.hpp:12:43: parsing function body 'App::deleteObject'
2.	/home/dmytro/Dev/app.hpp:12:43: in compound statement ('{}')
I[11:01:00.448] <-- textDocument/inlayHint(19)
I[11:01:00.449] Failed to find compilation database for /home/dmytro/Dev/app.hpp
I[11:01:00.449] ASTWorker building file /home/dmytro/Dev/app.hpp version 125 with command clangd fallback
[/home/dmytro/Dev]
/usr/lib64/llvm20/bin/clang -resource-dir=/usr/lib64/llvm20/bin/../../../lib/clang/20 -- /home/dmytro/Dev/app.hpp
I[11:01:00.450] --> textDocument/clangd.fileStatus
I[11:01:00.455] --> textDocument/publishDiagnostics
I[11:01:00.455] --> textDocument/inactiveRegions
I[11:01:00.455] --> reply:textDocument/inlayHint(19) 7 ms
I[11:01:00.455] --> textDocument/clangd.fileStatus
 #0 0x00007f8c8402914a llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/lib64/libLLVM.so.20.1+0x22914a)
 #1 0x00007f8c840260d4 llvm::sys::RunSignalHandlers() (/lib64/libLLVM.so.20.1+0x2260d4)
 #2 0x00007f8c840298bb (/lib64/libLLVM.so.20.1+0x2298bb)
 #3 0x00007f8c83827c30 __restore_rt (/lib64/libc.so.6+0x19c30)
 #4 0x00007f8c8ed424cd (/lib64/libclang-cpp.so.20.1+0x27424cd)
 #5 0x00007f8c8ed42048 clang::serialization::StableHashForTemplateArguments(llvm::ArrayRef<clang::TemplateArgument>) (/lib64/libclang-cpp.so.20.1+0x2742048)
 #6 0x00007f8c8ec0cb8d clang::ASTReader::LoadExternalSpecializationsImpl(llvm::DenseMap<clang::Decl const*, clang::serialization::reader::LazySpecializationInfoLookupTable, llvm::DenseMapInfo<clang::Decl const*, void>, llvm::detail::DenseMapPair<clang::Decl const*, clang::serialization::reader::LazySpecializationInfoLookupTable>>&, clang::Decl const*, llvm::ArrayRef<clang::TemplateArgument>) (/lib64/libclang-cpp.so.20.1+0x260cb8d)
 #7 0x00007f8c8ec0d184 non-virtual thunk to clang::ASTReader::LoadExternalSpecializations(clang::Decl const*, llvm::ArrayRef<clang::TemplateArgument>) (/lib64/libclang-cpp.so.20.1+0x260d184)
 #8 0x00007f8c8cb75237 clang::FunctionTemplateDecl::findSpecialization(llvm::ArrayRef<clang::TemplateArgument>, void*&) (/lib64/libclang-cpp.so.20.1+0x575237)
 #9 0x00007f8c8dd036a3 clang::TemplateDeclInstantiator::VisitFunctionDecl(clang::FunctionDecl*, clang::TemplateParameterList*, clang::TemplateDeclInstantiator::RewriteKind) (/lib64/libclang-cpp.so.20.1+0x17036a3)
#10 0x00007f8c8dd0eaba clang::Sema::SubstDecl(clang::Decl*, clang::DeclContext*, clang::MultiLevelTemplateArgumentList const&) (/lib64/libclang-cpp.so.20.1+0x170eaba)
#11 0x00007f8c8dbc6f1e clang::Sema::FinishTemplateArgumentDeduction(clang::FunctionTemplateDecl*, llvm::SmallVectorImpl<clang::DeducedTemplateArgument>&, unsigned int, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, llvm::SmallVectorImpl<clang::Sema::OriginalCallArg> const*, bool, bool, llvm::function_ref<bool ()>) (/lib64/libclang-cpp.so.20.1+0x15c6f1e)
#12 0x00007f8c8dbca2bf clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, bool, bool, clang::QualType, clang::Expr::Classification, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>)>) (/lib64/libclang-cpp.so.20.1+0x15ca2bf)
#13 0x00007f8c8da72100 clang::Sema::AddTemplateOverloadCandidate(clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::OverloadCandidateSet&, bool, bool, bool, clang::CallExpr::ADLCallKind, clang::OverloadCandidateParamOrder, bool) (/lib64/libclang-cpp.so.20.1+0x1472100)
#14 0x00007f8c8da85f1b (/lib64/libclang-cpp.so.20.1+0x1485f1b)
#15 0x00007f8c8da85da4 clang::Sema::AddOverloadedCallCandidates(clang::UnresolvedLookupExpr*, llvm::ArrayRef<clang::Expr*>, clang::OverloadCandidateSet&, bool) (/lib64/libclang-cpp.so.20.1+0x1485da4)
#16 0x00007f8c8d51e156 clang::SemaCodeCompletion::ProduceCallSignatureHelp(clang::Expr*, llvm::ArrayRef<clang::Expr*>, clang::SourceLocation) (/lib64/libclang-cpp.so.20.1+0xf1e156)
#17 0x00007f8c8c8802de (/lib64/libclang-cpp.so.20.1+0x2802de)
#18 0x00007f8c8c86b6d7 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/lib64/libclang-cpp.so.20.1+0x26b6d7)
#19 0x00007f8c8c87cf4a clang::Parser::ParseExpressionList(llvm::SmallVectorImpl<clang::Expr*>&, llvm::function_ref<void ()>, bool, bool, bool*) (/lib64/libclang-cpp.so.20.1+0x27cf4a)
#20 0x00007f8c8c86e195 clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult<clang::Expr*, true>) (/lib64/libclang-cpp.so.20.1+0x26e195)
#21 0x00007f8c8c870d5e clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) (/lib64/libclang-cpp.so.20.1+0x270d5e)
#22 0x00007f8c8c87242d clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) (/lib64/libclang-cpp.so.20.1+0x27242d)
#23 0x00007f8c8c86b787 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/lib64/libclang-cpp.so.20.1+0x26b787)
#24 0x00007f8c8c82fe6e clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/lib64/libclang-cpp.so.20.1+0x22fe6e)
#25 0x00007f8c8c82c571 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/lib64/libclang-cpp.so.20.1+0x22c571)
#26 0x00007f8c8c82b6ae clang::Parser::ParseSimpleDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, bool, clang::Parser::ForRangeInit*, clang::SourceLocation*) (/lib64/libclang-cpp.so.20.1+0x22b6ae)
#27 0x00007f8c8c82b070 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/lib64/libclang-cpp.so.20.1+0x22b070)
#28 0x00007f8c8c8e3942 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/lib64/libclang-cpp.so.20.1+0x2e3942)
#29 0x00007f8c8c8e2c60 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/lib64/libclang-cpp.so.20.1+0x2e2c60)
#30 0x00007f8c8c8ee90e clang::Parser::ParseCompoundStatementBody(bool) (/lib64/libclang-cpp.so.20.1+0x2ee90e)
#31 0x00007f8c8c8efeec clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/lib64/libclang-cpp.so.20.1+0x2efeec)
#32 0x00007f8c8c90e5d1 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/lib64/libclang-cpp.so.20.1+0x30e5d1)
#33 0x00007f8c8c82d85d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/lib64/libclang-cpp.so.20.1+0x22d85d)
#34 0x00007f8c8c90d373 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/lib64/libclang-cpp.so.20.1+0x30d373)
#35 0x00007f8c8c90cb6d clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/lib64/libclang-cpp.so.20.1+0x30cb6d)
#36 0x00007f8c8c90baae clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/lib64/libclang-cpp.so.20.1+0x30baae)
#37 0x00007f8c8c9099a6 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/lib64/libclang-cpp.so.20.1+0x3099a6)
#38 0x00007f8c8c812dee clang::ParseAST(clang::Sema&, bool, bool) (/lib64/libclang-cpp.so.20.1+0x212dee)
#39 0x00007f8c8ef1642f clang::FrontendAction::Execute() (/lib64/libclang-cpp.so.20.1+0x291642f)
#40 0x000056337ba8f96d (/usr/bin/clangd+0x1be96d)
#41 0x000056337ba91df8 (/usr/bin/clangd+0x1c0df8)
#42 0x000056337ba80dbf (/usr/bin/clangd+0x1afdbf)
#43 0x000056337bc88e21 (/usr/bin/clangd+0x3b7e21)
#44 0x000056337bdf7d65 (/usr/bin/clangd+0x526d65)
#45 0x00007f8c8387f1d4 start_thread (/lib64/libc.so.6+0x711d4)
#46 0x00007f8c83901cec __GI___clone3 (/lib64/libc.so.6+0xf3cec)
Signalled during preamble action: CodeComplete
  Filename: app.hpp
  Directory: /home/dmytro/Dev
  Command Line: /usr/lib64/llvm20/bin/clang -resource-dir=/usr/lib64/llvm20/bin/../../../lib/clang/20 -- /home/dmytro/Dev/app.hpp
[Info  - 11:01:01 AM] Connection to server got closed. Server will restart.
```


app.hpp
```Cpp
#include <vector>
#include <algorithm>

class App {
private:
    std::vector<int> arr;

public:
    void deleteObject(int object);
};

inline void App::deleteObject(int object) {
    auto it = std::find(arr.begin(), arr.end(), object);
}
```

The crash happens while typing out the line `auto it = std::find(arr.begin(), arr.end(), object);`, specifically when typing out the arguments (arr.begin(), arr.end(), object).

Steps to reproduce:
1. Remove the last argument (`object`) in `auto it = std::find(arr.begin(), arr.end(), object);`
2. You should have `auto it = std::find(arr.begin(), arr.end(), |);`, where `|` indicates the cursor
3. Press `o`

Expected behavior:
clangd should provide code completion (for `object`)

Actual behavior:
clangd crashes as soon as you start typing

This crash seems to happen with vectors of any type.

Additional info:
OS: Fedora 42
Editor: VSCode (+clangd extension)
Output of `clangd --version`:
```
clangd version 20.1.3 (Fedora 20.1.3-1.fc42)
Features: linux
Platform: x86_64-unknown-linux-gnu; target=x86_64-redhat-linux-gnu
```
Output of `clang --version`: 
```
clang version 20.1.3 (Fedora 20.1.3-1.fc42)
Target: x86_64-redhat-linux-gnu
Thread model: posix
InstalledDir: /usr/lib64/llvm20/bin
Configuration file: /etc/clang/x86_64-redhat-linux-gnu-clang.cfg
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: dzmtrzz (dzmtrzz)

<details>
Crash backtrace:

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.	&lt;eof&gt; parser at end of file
1.	/home/dmytro/Dev/app.hpp:12:43: parsing function body 'App::deleteObject'
2.	/home/dmytro/Dev/app.hpp:12:43: in compound statement ('{}')
I[11:01:00.448] &lt;-- textDocument/inlayHint(19)
I[11:01:00.449] Failed to find compilation database for /home/dmytro/Dev/app.hpp
I[11:01:00.449] ASTWorker building file /home/dmytro/Dev/app.hpp version 125 with command clangd fallback
[/home/dmytro/Dev]
/usr/lib64/llvm20/bin/clang -resource-dir=/usr/lib64/llvm20/bin/../../../lib/clang/20 -- /home/dmytro/Dev/app.hpp
I[11:01:00.450] --&gt; textDocument/clangd.fileStatus
I[11:01:00.455] --&gt; textDocument/publishDiagnostics
I[11:01:00.455] --&gt; textDocument/inactiveRegions
I[11:01:00.455] --&gt; reply:textDocument/inlayHint(19) 7 ms
I[11:01:00.455] --&gt; textDocument/clangd.fileStatus
 #<!-- -->0 0x00007f8c8402914a llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/lib64/libLLVM.so.20.1+0x22914a)
 #<!-- -->1 0x00007f8c840260d4 llvm::sys::RunSignalHandlers() (/lib64/libLLVM.so.20.1+0x2260d4)
 #<!-- -->2 0x00007f8c840298bb (/lib64/libLLVM.so.20.1+0x2298bb)
 #<!-- -->3 0x00007f8c83827c30 __restore_rt (/lib64/libc.so.6+0x19c30)
 #<!-- -->4 0x00007f8c8ed424cd (/lib64/libclang-cpp.so.20.1+0x27424cd)
 #<!-- -->5 0x00007f8c8ed42048 clang::serialization::StableHashForTemplateArguments(llvm::ArrayRef&lt;clang::TemplateArgument&gt;) (/lib64/libclang-cpp.so.20.1+0x2742048)
 #<!-- -->6 0x00007f8c8ec0cb8d clang::ASTReader::LoadExternalSpecializationsImpl(llvm::DenseMap&lt;clang::Decl const*, clang::serialization::reader::LazySpecializationInfoLookupTable, llvm::DenseMapInfo&lt;clang::Decl const*, void&gt;, llvm::detail::DenseMapPair&lt;clang::Decl const*, clang::serialization::reader::LazySpecializationInfoLookupTable&gt;&gt;&amp;, clang::Decl const*, llvm::ArrayRef&lt;clang::TemplateArgument&gt;) (/lib64/libclang-cpp.so.20.1+0x260cb8d)
 #<!-- -->7 0x00007f8c8ec0d184 non-virtual thunk to clang::ASTReader::LoadExternalSpecializations(clang::Decl const*, llvm::ArrayRef&lt;clang::TemplateArgument&gt;) (/lib64/libclang-cpp.so.20.1+0x260d184)
 #<!-- -->8 0x00007f8c8cb75237 clang::FunctionTemplateDecl::findSpecialization(llvm::ArrayRef&lt;clang::TemplateArgument&gt;, void*&amp;) (/lib64/libclang-cpp.so.20.1+0x575237)
 #<!-- -->9 0x00007f8c8dd036a3 clang::TemplateDeclInstantiator::VisitFunctionDecl(clang::FunctionDecl*, clang::TemplateParameterList*, clang::TemplateDeclInstantiator::RewriteKind) (/lib64/libclang-cpp.so.20.1+0x17036a3)
#<!-- -->10 0x00007f8c8dd0eaba clang::Sema::SubstDecl(clang::Decl*, clang::DeclContext*, clang::MultiLevelTemplateArgumentList const&amp;) (/lib64/libclang-cpp.so.20.1+0x170eaba)
#<!-- -->11 0x00007f8c8dbc6f1e clang::Sema::FinishTemplateArgumentDeduction(clang::FunctionTemplateDecl*, llvm::SmallVectorImpl&lt;clang::DeducedTemplateArgument&gt;&amp;, unsigned int, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, llvm::SmallVectorImpl&lt;clang::Sema::OriginalCallArg&gt; const*, bool, bool, llvm::function_ref&lt;bool ()&gt;) (/lib64/libclang-cpp.so.20.1+0x15c6f1e)
#<!-- -->12 0x00007f8c8dbca2bf clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, bool, bool, bool, clang::QualType, clang::Expr::Classification, llvm::function_ref&lt;bool (llvm::ArrayRef&lt;clang::QualType&gt;)&gt;) (/lib64/libclang-cpp.so.20.1+0x15ca2bf)
#<!-- -->13 0x00007f8c8da72100 clang::Sema::AddTemplateOverloadCandidate(clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::OverloadCandidateSet&amp;, bool, bool, bool, clang::CallExpr::ADLCallKind, clang::OverloadCandidateParamOrder, bool) (/lib64/libclang-cpp.so.20.1+0x1472100)
#<!-- -->14 0x00007f8c8da85f1b (/lib64/libclang-cpp.so.20.1+0x1485f1b)
#<!-- -->15 0x00007f8c8da85da4 clang::Sema::AddOverloadedCallCandidates(clang::UnresolvedLookupExpr*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::OverloadCandidateSet&amp;, bool) (/lib64/libclang-cpp.so.20.1+0x1485da4)
#<!-- -->16 0x00007f8c8d51e156 clang::SemaCodeCompletion::ProduceCallSignatureHelp(clang::Expr*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation) (/lib64/libclang-cpp.so.20.1+0xf1e156)
#<!-- -->17 0x00007f8c8c8802de (/lib64/libclang-cpp.so.20.1+0x2802de)
#<!-- -->18 0x00007f8c8c86b6d7 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/lib64/libclang-cpp.so.20.1+0x26b6d7)
#<!-- -->19 0x00007f8c8c87cf4a clang::Parser::ParseExpressionList(llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, llvm::function_ref&lt;void ()&gt;, bool, bool, bool*) (/lib64/libclang-cpp.so.20.1+0x27cf4a)
#<!-- -->20 0x00007f8c8c86e195 clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult&lt;clang::Expr*, true&gt;) (/lib64/libclang-cpp.so.20.1+0x26e195)
#<!-- -->21 0x00007f8c8c870d5e clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) (/lib64/libclang-cpp.so.20.1+0x270d5e)
#<!-- -->22 0x00007f8c8c87242d clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) (/lib64/libclang-cpp.so.20.1+0x27242d)
#<!-- -->23 0x00007f8c8c86b787 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/lib64/libclang-cpp.so.20.1+0x26b787)
#<!-- -->24 0x00007f8c8c82fe6e clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) (/lib64/libclang-cpp.so.20.1+0x22fe6e)
#<!-- -->25 0x00007f8c8c82c571 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/lib64/libclang-cpp.so.20.1+0x22c571)
#<!-- -->26 0x00007f8c8c82b6ae clang::Parser::ParseSimpleDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, bool, clang::Parser::ForRangeInit*, clang::SourceLocation*) (/lib64/libclang-cpp.so.20.1+0x22b6ae)
#<!-- -->27 0x00007f8c8c82b070 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/lib64/libclang-cpp.so.20.1+0x22b070)
#<!-- -->28 0x00007f8c8c8e3942 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/lib64/libclang-cpp.so.20.1+0x2e3942)
#<!-- -->29 0x00007f8c8c8e2c60 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/lib64/libclang-cpp.so.20.1+0x2e2c60)
#<!-- -->30 0x00007f8c8c8ee90e clang::Parser::ParseCompoundStatementBody(bool) (/lib64/libclang-cpp.so.20.1+0x2ee90e)
#<!-- -->31 0x00007f8c8c8efeec clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/lib64/libclang-cpp.so.20.1+0x2efeec)
#<!-- -->32 0x00007f8c8c90e5d1 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/lib64/libclang-cpp.so.20.1+0x30e5d1)
#<!-- -->33 0x00007f8c8c82d85d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/lib64/libclang-cpp.so.20.1+0x22d85d)
#<!-- -->34 0x00007f8c8c90d373 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/lib64/libclang-cpp.so.20.1+0x30d373)
#<!-- -->35 0x00007f8c8c90cb6d clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/lib64/libclang-cpp.so.20.1+0x30cb6d)
#<!-- -->36 0x00007f8c8c90baae clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/lib64/libclang-cpp.so.20.1+0x30baae)
#<!-- -->37 0x00007f8c8c9099a6 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/lib64/libclang-cpp.so.20.1+0x3099a6)
#<!-- -->38 0x00007f8c8c812dee clang::ParseAST(clang::Sema&amp;, bool, bool) (/lib64/libclang-cpp.so.20.1+0x212dee)
#<!-- -->39 0x00007f8c8ef1642f clang::FrontendAction::Execute() (/lib64/libclang-cpp.so.20.1+0x291642f)
#<!-- -->40 0x000056337ba8f96d (/usr/bin/clangd+0x1be96d)
#<!-- -->41 0x000056337ba91df8 (/usr/bin/clangd+0x1c0df8)
#<!-- -->42 0x000056337ba80dbf (/usr/bin/clangd+0x1afdbf)
#<!-- -->43 0x000056337bc88e21 (/usr/bin/clangd+0x3b7e21)
#<!-- -->44 0x000056337bdf7d65 (/usr/bin/clangd+0x526d65)
#<!-- -->45 0x00007f8c8387f1d4 start_thread (/lib64/libc.so.6+0x711d4)
#<!-- -->46 0x00007f8c83901cec __GI___clone3 (/lib64/libc.so.6+0xf3cec)
Signalled during preamble action: CodeComplete
  Filename: app.hpp
  Directory: /home/dmytro/Dev
  Command Line: /usr/lib64/llvm20/bin/clang -resource-dir=/usr/lib64/llvm20/bin/../../../lib/clang/20 -- /home/dmytro/Dev/app.hpp
[Info  - 11:01:01 AM] Connection to server got closed. Server will restart.
```


app.hpp
```Cpp
#include &lt;vector&gt;
#include &lt;algorithm&gt;

class App {
private:
    std::vector&lt;int&gt; arr;

public:
    void deleteObject(int object);
};

inline void App::deleteObject(int object) {
    auto it = std::find(arr.begin(), arr.end(), object);
}
```

The crash happens while typing out the line `auto it = std::find(arr.begin(), arr.end(), object);`, specifically when typing out the arguments (arr.begin(), arr.end(), object).

Steps to reproduce:
1. Remove the last argument (`object`) in `auto it = std::find(arr.begin(), arr.end(), object);`
2. You should have `auto it = std::find(arr.begin(), arr.end(), |);`, where `|` indicates the cursor
3. Press `o`

Expected behavior:
clangd should provide code completion (for `object`)

Actual behavior:
clangd crashes as soon as you start typing

This crash seems to happen with vectors of any type.

Additional info:
OS: Fedora 42
Editor: VSCode (+clangd extension)
Output of `clangd --version`:
```
clangd version 20.1.3 (Fedora 20.1.3-1.fc42)
Features: linux
Platform: x86_64-unknown-linux-gnu; target=x86_64-redhat-linux-gnu
```
Output of `clang --version`: 
```
clang version 20.1.3 (Fedora 20.1.3-1.fc42)
Target: x86_64-redhat-linux-gnu
Thread model: posix
InstalledDir: /usr/lib64/llvm20/bin
Configuration file: /etc/clang/x86_64-redhat-linux-gnu-clang.cfg
```
</details>


---

### Comment 2 - zyn0217

@dzmtrzz which libstdc++/libc++ version are you using?

---

### Comment 3 - anny-moose

> [@dzmtrzz](https://github.com/dzmtrzz) which libstdc++/libc++ version are you using?

On my system:
`_GLIBCXX_RELEASE`  is defined as `15`,
`__GLIBCXX__` is defined as `20250425`,
The gcc version is `gcc-15.1.1-1.fc42`,
I am also using `glibc 2.41` (in case that matters)

`strings /usr/lib/libstdc++.so.6 | grep GLIBCXX` outputs the following:
```
GLIBCXX_3.4
GLIBCXX_3.4.1
GLIBCXX_3.4.2
GLIBCXX_3.4.3
GLIBCXX_3.4.4
GLIBCXX_3.4.5
GLIBCXX_3.4.6
GLIBCXX_3.4.7
GLIBCXX_3.4.8
GLIBCXX_3.4.9
GLIBCXX_3.4.10
GLIBCXX_3.4.11
GLIBCXX_3.4.12
GLIBCXX_3.4.13
GLIBCXX_3.4.14
GLIBCXX_3.4.15
GLIBCXX_3.4.16
GLIBCXX_3.4.17
GLIBCXX_3.4.18
GLIBCXX_3.4.19
GLIBCXX_3.4.20
GLIBCXX_3.4.21
GLIBCXX_3.4.22
GLIBCXX_3.4.23
GLIBCXX_3.4.24
GLIBCXX_3.4.25
GLIBCXX_3.4.26
GLIBCXX_3.4.27
GLIBCXX_3.4.28
GLIBCXX_3.4.29
GLIBCXX_3.4.30
GLIBCXX_3.4.31
GLIBCXX_3.4.32
GLIBCXX_3.4.33
GLIBCXX_3.4.34
GLIBCXX_TUNABLES
GLIBCXX_DEBUG_MESSAGE_LENGTH
```

If there is anything else i can add, please tell me.

---

### Comment 4 - anny-moose

Here's a simpler version that also crashes the LSP under the same circumstances.

a.cpp
```cpp
#include <algorithm>
#include <vector>

void findObject(int object, std::vector<int> arr) {
    auto it = std::find(arr.begin(), arr.end(), object);
}
```

---

### Comment 5 - zyn0217

Maybe related: https://github.com/llvm/llvm-project/issues/139019

---

### Comment 6 - HighCommander4

> Maybe related: [#139019](https://github.com/llvm/llvm-project/issues/139019)

Yeah, based on the stacktrace, very likely a duplicate.

---

### Comment 7 - llvmbot


@llvm/issue-subscribers-clangd

Author: dzmtrzz (dzmtrzz)

<details>
Crash backtrace:

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.	&lt;eof&gt; parser at end of file
1.	/home/dmytro/Dev/app.hpp:12:43: parsing function body 'App::deleteObject'
2.	/home/dmytro/Dev/app.hpp:12:43: in compound statement ('{}')
I[11:01:00.448] &lt;-- textDocument/inlayHint(19)
I[11:01:00.449] Failed to find compilation database for /home/dmytro/Dev/app.hpp
I[11:01:00.449] ASTWorker building file /home/dmytro/Dev/app.hpp version 125 with command clangd fallback
[/home/dmytro/Dev]
/usr/lib64/llvm20/bin/clang -resource-dir=/usr/lib64/llvm20/bin/../../../lib/clang/20 -- /home/dmytro/Dev/app.hpp
I[11:01:00.450] --&gt; textDocument/clangd.fileStatus
I[11:01:00.455] --&gt; textDocument/publishDiagnostics
I[11:01:00.455] --&gt; textDocument/inactiveRegions
I[11:01:00.455] --&gt; reply:textDocument/inlayHint(19) 7 ms
I[11:01:00.455] --&gt; textDocument/clangd.fileStatus
 #<!-- -->0 0x00007f8c8402914a llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/lib64/libLLVM.so.20.1+0x22914a)
 #<!-- -->1 0x00007f8c840260d4 llvm::sys::RunSignalHandlers() (/lib64/libLLVM.so.20.1+0x2260d4)
 #<!-- -->2 0x00007f8c840298bb (/lib64/libLLVM.so.20.1+0x2298bb)
 #<!-- -->3 0x00007f8c83827c30 __restore_rt (/lib64/libc.so.6+0x19c30)
 #<!-- -->4 0x00007f8c8ed424cd (/lib64/libclang-cpp.so.20.1+0x27424cd)
 #<!-- -->5 0x00007f8c8ed42048 clang::serialization::StableHashForTemplateArguments(llvm::ArrayRef&lt;clang::TemplateArgument&gt;) (/lib64/libclang-cpp.so.20.1+0x2742048)
 #<!-- -->6 0x00007f8c8ec0cb8d clang::ASTReader::LoadExternalSpecializationsImpl(llvm::DenseMap&lt;clang::Decl const*, clang::serialization::reader::LazySpecializationInfoLookupTable, llvm::DenseMapInfo&lt;clang::Decl const*, void&gt;, llvm::detail::DenseMapPair&lt;clang::Decl const*, clang::serialization::reader::LazySpecializationInfoLookupTable&gt;&gt;&amp;, clang::Decl const*, llvm::ArrayRef&lt;clang::TemplateArgument&gt;) (/lib64/libclang-cpp.so.20.1+0x260cb8d)
 #<!-- -->7 0x00007f8c8ec0d184 non-virtual thunk to clang::ASTReader::LoadExternalSpecializations(clang::Decl const*, llvm::ArrayRef&lt;clang::TemplateArgument&gt;) (/lib64/libclang-cpp.so.20.1+0x260d184)
 #<!-- -->8 0x00007f8c8cb75237 clang::FunctionTemplateDecl::findSpecialization(llvm::ArrayRef&lt;clang::TemplateArgument&gt;, void*&amp;) (/lib64/libclang-cpp.so.20.1+0x575237)
 #<!-- -->9 0x00007f8c8dd036a3 clang::TemplateDeclInstantiator::VisitFunctionDecl(clang::FunctionDecl*, clang::TemplateParameterList*, clang::TemplateDeclInstantiator::RewriteKind) (/lib64/libclang-cpp.so.20.1+0x17036a3)
#<!-- -->10 0x00007f8c8dd0eaba clang::Sema::SubstDecl(clang::Decl*, clang::DeclContext*, clang::MultiLevelTemplateArgumentList const&amp;) (/lib64/libclang-cpp.so.20.1+0x170eaba)
#<!-- -->11 0x00007f8c8dbc6f1e clang::Sema::FinishTemplateArgumentDeduction(clang::FunctionTemplateDecl*, llvm::SmallVectorImpl&lt;clang::DeducedTemplateArgument&gt;&amp;, unsigned int, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, llvm::SmallVectorImpl&lt;clang::Sema::OriginalCallArg&gt; const*, bool, bool, llvm::function_ref&lt;bool ()&gt;) (/lib64/libclang-cpp.so.20.1+0x15c6f1e)
#<!-- -->12 0x00007f8c8dbca2bf clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, bool, bool, bool, clang::QualType, clang::Expr::Classification, llvm::function_ref&lt;bool (llvm::ArrayRef&lt;clang::QualType&gt;)&gt;) (/lib64/libclang-cpp.so.20.1+0x15ca2bf)
#<!-- -->13 0x00007f8c8da72100 clang::Sema::AddTemplateOverloadCandidate(clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::OverloadCandidateSet&amp;, bool, bool, bool, clang::CallExpr::ADLCallKind, clang::OverloadCandidateParamOrder, bool) (/lib64/libclang-cpp.so.20.1+0x1472100)
#<!-- -->14 0x00007f8c8da85f1b (/lib64/libclang-cpp.so.20.1+0x1485f1b)
#<!-- -->15 0x00007f8c8da85da4 clang::Sema::AddOverloadedCallCandidates(clang::UnresolvedLookupExpr*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::OverloadCandidateSet&amp;, bool) (/lib64/libclang-cpp.so.20.1+0x1485da4)
#<!-- -->16 0x00007f8c8d51e156 clang::SemaCodeCompletion::ProduceCallSignatureHelp(clang::Expr*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation) (/lib64/libclang-cpp.so.20.1+0xf1e156)
#<!-- -->17 0x00007f8c8c8802de (/lib64/libclang-cpp.so.20.1+0x2802de)
#<!-- -->18 0x00007f8c8c86b6d7 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/lib64/libclang-cpp.so.20.1+0x26b6d7)
#<!-- -->19 0x00007f8c8c87cf4a clang::Parser::ParseExpressionList(llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, llvm::function_ref&lt;void ()&gt;, bool, bool, bool*) (/lib64/libclang-cpp.so.20.1+0x27cf4a)
#<!-- -->20 0x00007f8c8c86e195 clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult&lt;clang::Expr*, true&gt;) (/lib64/libclang-cpp.so.20.1+0x26e195)
#<!-- -->21 0x00007f8c8c870d5e clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) (/lib64/libclang-cpp.so.20.1+0x270d5e)
#<!-- -->22 0x00007f8c8c87242d clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) (/lib64/libclang-cpp.so.20.1+0x27242d)
#<!-- -->23 0x00007f8c8c86b787 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/lib64/libclang-cpp.so.20.1+0x26b787)
#<!-- -->24 0x00007f8c8c82fe6e clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) (/lib64/libclang-cpp.so.20.1+0x22fe6e)
#<!-- -->25 0x00007f8c8c82c571 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/lib64/libclang-cpp.so.20.1+0x22c571)
#<!-- -->26 0x00007f8c8c82b6ae clang::Parser::ParseSimpleDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, bool, clang::Parser::ForRangeInit*, clang::SourceLocation*) (/lib64/libclang-cpp.so.20.1+0x22b6ae)
#<!-- -->27 0x00007f8c8c82b070 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/lib64/libclang-cpp.so.20.1+0x22b070)
#<!-- -->28 0x00007f8c8c8e3942 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/lib64/libclang-cpp.so.20.1+0x2e3942)
#<!-- -->29 0x00007f8c8c8e2c60 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/lib64/libclang-cpp.so.20.1+0x2e2c60)
#<!-- -->30 0x00007f8c8c8ee90e clang::Parser::ParseCompoundStatementBody(bool) (/lib64/libclang-cpp.so.20.1+0x2ee90e)
#<!-- -->31 0x00007f8c8c8efeec clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/lib64/libclang-cpp.so.20.1+0x2efeec)
#<!-- -->32 0x00007f8c8c90e5d1 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/lib64/libclang-cpp.so.20.1+0x30e5d1)
#<!-- -->33 0x00007f8c8c82d85d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/lib64/libclang-cpp.so.20.1+0x22d85d)
#<!-- -->34 0x00007f8c8c90d373 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/lib64/libclang-cpp.so.20.1+0x30d373)
#<!-- -->35 0x00007f8c8c90cb6d clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/lib64/libclang-cpp.so.20.1+0x30cb6d)
#<!-- -->36 0x00007f8c8c90baae clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/lib64/libclang-cpp.so.20.1+0x30baae)
#<!-- -->37 0x00007f8c8c9099a6 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/lib64/libclang-cpp.so.20.1+0x3099a6)
#<!-- -->38 0x00007f8c8c812dee clang::ParseAST(clang::Sema&amp;, bool, bool) (/lib64/libclang-cpp.so.20.1+0x212dee)
#<!-- -->39 0x00007f8c8ef1642f clang::FrontendAction::Execute() (/lib64/libclang-cpp.so.20.1+0x291642f)
#<!-- -->40 0x000056337ba8f96d (/usr/bin/clangd+0x1be96d)
#<!-- -->41 0x000056337ba91df8 (/usr/bin/clangd+0x1c0df8)
#<!-- -->42 0x000056337ba80dbf (/usr/bin/clangd+0x1afdbf)
#<!-- -->43 0x000056337bc88e21 (/usr/bin/clangd+0x3b7e21)
#<!-- -->44 0x000056337bdf7d65 (/usr/bin/clangd+0x526d65)
#<!-- -->45 0x00007f8c8387f1d4 start_thread (/lib64/libc.so.6+0x711d4)
#<!-- -->46 0x00007f8c83901cec __GI___clone3 (/lib64/libc.so.6+0xf3cec)
Signalled during preamble action: CodeComplete
  Filename: app.hpp
  Directory: /home/dmytro/Dev
  Command Line: /usr/lib64/llvm20/bin/clang -resource-dir=/usr/lib64/llvm20/bin/../../../lib/clang/20 -- /home/dmytro/Dev/app.hpp
[Info  - 11:01:01 AM] Connection to server got closed. Server will restart.
```


app.hpp
```Cpp
#include &lt;vector&gt;
#include &lt;algorithm&gt;

class App {
private:
    std::vector&lt;int&gt; arr;

public:
    void deleteObject(int object);
};

inline void App::deleteObject(int object) {
    auto it = std::find(arr.begin(), arr.end(), object);
}
```

The crash happens while typing out the line `auto it = std::find(arr.begin(), arr.end(), object);`, specifically when typing out the arguments (arr.begin(), arr.end(), object).

Steps to reproduce:
1. Remove the last argument (`object`) in `auto it = std::find(arr.begin(), arr.end(), object);`
2. You should have `auto it = std::find(arr.begin(), arr.end(), |);`, where `|` indicates the cursor
3. Press `o`

Expected behavior:
clangd should provide code completion (for `object`)

Actual behavior:
clangd crashes as soon as you start typing

This crash seems to happen with vectors of any type.

Additional info:
OS: Fedora 42
Editor: VSCode (+clangd extension)
Output of `clangd --version`:
```
clangd version 20.1.3 (Fedora 20.1.3-1.fc42)
Features: linux
Platform: x86_64-unknown-linux-gnu; target=x86_64-redhat-linux-gnu
```
Output of `clang --version`: 
```
clang version 20.1.3 (Fedora 20.1.3-1.fc42)
Target: x86_64-redhat-linux-gnu
Thread model: posix
InstalledDir: /usr/lib64/llvm20/bin
Configuration file: /etc/clang/x86_64-redhat-linux-gnu-clang.cfg
```
</details>


---

