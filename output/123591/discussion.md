# Clang frontend command failed with exit code 139

**Author:** ghost
**URL:** https://github.com/llvm/llvm-project/issues/123591
**Status:** Closed
**Labels:** c++17, clang:frontend, crash-on-invalid, confirmed, regression:20
**Closed Date:** 2025-03-22T14:55:59Z

## Body

Hi, 

I've been experimenting with ways to model an AST in modern C++. During one of these experiments, my compiler crashed on me. Per request of the compiler itself, I've attached the output and the associated culprit files below. 

The issue seems to arise from a leftover `template <typename T>`  declaration:
```
template <typename T>  // <<< right here, sorry!
using AstNode = std::variant<AstProgram, AstDecl, AstExpr, AstStmt>;
```
I accidentally left it in place when I was restructuring the program. Once I remove the declaration, the program still doesn't compile (that's correct), but the compiler doesn't crash anymore.

---

[main-8471ef.tar.gz](https://github.com/user-attachments/files/18476429/main-8471ef.tar.gz)

```
Stack dump:
0.	Program arguments: clang++ -std=c++20 -Wall -Wextra -Wconversion -Wpedantic -funsigned-char -fsanitize=address,undefined -fno-omit-frame-pointer -g3 -glldb -O0 -c -o target/main.o src/main.cc
1.	src/main.cc:69:112: current parser token ';'
2.	src/main.cc:68:1: parsing function body 'main'
3.	src/main.cc:68:1: in compound statement ('{}')
 #0 0x00007fd90b8b0a8a llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.1+0xeb0a8a)
 #1 0x00007fd90b8ae634 llvm::sys::RunSignalHandlers() (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.1+0xeae634)
 #2 0x00007fd90b7f44f4 (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.1+0xdf44f4)
 #3 0x00007fd90a449da0 (/lib/x86_64-linux-gnu/libc.so.6+0x3fda0)
 #4 0x00007fd913a850e4 clang::TemplateArgument::isPackExpansion() const (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x12850e4)
 #5 0x00007fd914631acb clang::Sema::getFullyPackExpandedSize(clang::TemplateArgument) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1e31acb)
 #6 0x00007fd9145bf020 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1dbf020)
 #7 0x00007fd9145c8bcb (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1dc8bcb)
 #8 0x00007fd91459a425 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1d9a425)
 #9 0x00007fd9145d2ab0 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1dd2ab0)
#10 0x00007fd9145d9867 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1dd9867)
#11 0x00007fd9145d6d6d (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1dd6d6d)
#12 0x00007fd91459348b (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1d9348b)
#13 0x00007fd91459a3a7 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1d9a3a7)
#14 0x00007fd91459a232 clang::Sema::SubstTemplateArgument(clang::TemplateArgumentLoc const&, clang::MultiLevelTemplateArgumentList const&, clang::TemplateArgumentLoc&, clang::SourceLocation, clang::DeclarationName const&) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1d9a232)
#15 0x00007fd91445daab (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1c5daab)
#16 0x00007fd91445d57e clang::Sema::SubstDefaultTemplateArgumentIfAvailable(clang::TemplateDecl*, clang::SourceLocation, clang::SourceLocation, clang::Decl*, llvm::ArrayRef<clang::TemplateArgument>, llvm::ArrayRef<clang::TemplateArgument>, bool&) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1c5d57e)
#17 0x00007fd9144e3c61 clang::Sema::FinishTemplateArgumentDeduction(clang::FunctionTemplateDecl*, llvm::SmallVectorImpl<clang::DeducedTemplateArgument>&, unsigned int, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, llvm::SmallVectorImpl<clang::Sema::OriginalCallArg> const*, bool, llvm::function_ref<bool ()>) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1ce3c61)
#18 0x00007fd91454001b (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1d4001b)
#19 0x00007fd913d93671 clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1593671)
#20 0x00007fd9144e5c1b clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, bool, clang::QualType, clang::Expr::Classification, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>)>) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1ce5c1b)
#21 0x00007fd9143cb434 clang::Sema::AddTemplateOverloadCandidate(clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::OverloadCandidateSet&, bool, bool, bool, clang::CallExpr::ADLCallKind, clang::OverloadCandidateParamOrder, bool) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1bcb434)
#22 0x00007fd9142a0847 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1aa0847)
#23 0x00007fd914293c39 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1a93c39)
#24 0x00007fd9142930d9 clang::Sema::DeduceTemplateSpecializationFromInitializer(clang::TypeSourceInfo*, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::MutableArrayRef<clang::Expr*>) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1a930d9)
#25 0x00007fd913f4a1f2 clang::Sema::deduceVarTypeFromInitializer(clang::VarDecl*, clang::DeclarationName, clang::QualType, clang::TypeSourceInfo*, clang::SourceRange, bool, clang::Expr*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x174a1f2)
#26 0x00007fd913f4a920 clang::Sema::DeduceVariableDeclarationType(clang::VarDecl*, bool, clang::Expr*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x174a920)
#27 0x00007fd913f4b03c clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x174b03c)
#28 0x00007fd91344e3c1 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0xc4e3c1)
#29 0x00007fd91344b0c9 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0xc4b0c9)
#30 0x00007fd91344a4f5 clang::Parser::ParseSimpleDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, bool, clang::Parser::ForRangeInit*, clang::SourceLocation*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0xc4a4f5)
#31 0x00007fd913449e79 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0xc49e79)
#32 0x00007fd9134e9779 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0xce9779)
#33 0x00007fd9134e7a6a clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0xce7a6a)
#34 0x00007fd9134f05b0 clang::Parser::ParseCompoundStatementBody(bool) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0xcf05b0)
#35 0x00007fd9134f16f6 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0xcf16f6)
#36 0x00007fd91350b8e3 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0xd0b8e3)
#37 0x00007fd91344c46d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0xc4c46d)
#38 0x00007fd91350a8be clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0xd0a8be)
#39 0x00007fd91350a25f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0xd0a25f)
#40 0x00007fd91350944c clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0xd0944c)
#41 0x00007fd9135078d7 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0xd078d7)
#42 0x00007fd91343457e clang::ParseAST(clang::Sema&, bool, bool) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0xc3457e)
#43 0x00007fd915547379 clang::FrontendAction::Execute() (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x2d47379)
#44 0x00007fd9154b5c74 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x2cb5c74)
#45 0x00007fd9155c57fe clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x2dc57fe)
#46 0x000055891017bc61 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/usr/lib/llvm-19/bin/clang+0x11c61)
#47 0x0000558910178b35 (/usr/lib/llvm-19/bin/clang+0xeb35)
#48 0x00007fd91513509d (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x293509d)
#49 0x00007fd90b7f4290 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.1+0xdf4290)
#50 0x00007fd915134b31 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x2934b31)
#51 0x00007fd9150f9ec8 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x28f9ec8)
#52 0x00007fd9150fa13e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x28fa13e)
#53 0x00007fd9151178fc clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x29178fc)
#54 0x0000558910178574 clang_main(int, char**, llvm::ToolContext const&) (/usr/lib/llvm-19/bin/clang+0xe574)
#55 0x00005589101864ba main (/usr/lib/llvm-19/bin/clang+0x1c4ba)
#56 0x00007fd90a433d68 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#57 0x00007fd90a433e25 call_init ./csu/../csu/libc-start.c:128:20
#58 0x00007fd90a433e25 __libc_start_main ./csu/../csu/libc-start.c:347:5
#59 0x0000558910176731 _start (/usr/lib/llvm-19/bin/clang+0xc731)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Debian clang version 19.1.6 (1+b1)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/lib/llvm-19/bin
clang++: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: /tmp/main-8471ef.cpp
clang++: note: diagnostic msg: /tmp/main-8471ef.sh
clang++: note: diagnostic msg: 

********************
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (dvora124-wq)

<details>
Hi, 

I've been experimenting with ways to model an AST in modern C++. During one of these experiments, my compiler crashed on me. Per request of the compiler itself, I've attached the output and the associated culprit files below. 

The issue seems to arise from a leftover `template &lt;typename T&gt;`  declaration:
```
template &lt;typename T&gt;  // &lt;&lt;&lt; right here, sorry!
using AstNode = std::variant&lt;AstProgram, AstDecl, AstExpr, AstStmt&gt;;
```
I accidentally left it in place when I was restructuring the program. Once I remove the declaration, the program still doesn't compile (that's correct), but the compiler doesn't crash anymore.

---

[main-8471ef.tar.gz](https://github.com/user-attachments/files/18476429/main-8471ef.tar.gz)

```
Stack dump:
0.	Program arguments: clang++ -std=c++20 -Wall -Wextra -Wconversion -Wpedantic -funsigned-char -fsanitize=address,undefined -fno-omit-frame-pointer -g3 -glldb -O0 -c -o target/main.o src/main.cc
1.	src/main.cc:69:112: current parser token ';'
2.	src/main.cc:68:1: parsing function body 'main'
3.	src/main.cc:68:1: in compound statement ('{}')
 #<!-- -->0 0x00007fd90b8b0a8a llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.1+0xeb0a8a)
 #<!-- -->1 0x00007fd90b8ae634 llvm::sys::RunSignalHandlers() (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.1+0xeae634)
 #<!-- -->2 0x00007fd90b7f44f4 (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.1+0xdf44f4)
 #<!-- -->3 0x00007fd90a449da0 (/lib/x86_64-linux-gnu/libc.so.6+0x3fda0)
 #<!-- -->4 0x00007fd913a850e4 clang::TemplateArgument::isPackExpansion() const (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x12850e4)
 #<!-- -->5 0x00007fd914631acb clang::Sema::getFullyPackExpandedSize(clang::TemplateArgument) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1e31acb)
 #<!-- -->6 0x00007fd9145bf020 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1dbf020)
 #<!-- -->7 0x00007fd9145c8bcb (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1dc8bcb)
 #<!-- -->8 0x00007fd91459a425 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1d9a425)
 #<!-- -->9 0x00007fd9145d2ab0 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1dd2ab0)
#<!-- -->10 0x00007fd9145d9867 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1dd9867)
#<!-- -->11 0x00007fd9145d6d6d (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1dd6d6d)
#<!-- -->12 0x00007fd91459348b (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1d9348b)
#<!-- -->13 0x00007fd91459a3a7 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1d9a3a7)
#<!-- -->14 0x00007fd91459a232 clang::Sema::SubstTemplateArgument(clang::TemplateArgumentLoc const&amp;, clang::MultiLevelTemplateArgumentList const&amp;, clang::TemplateArgumentLoc&amp;, clang::SourceLocation, clang::DeclarationName const&amp;) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1d9a232)
#<!-- -->15 0x00007fd91445daab (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1c5daab)
#<!-- -->16 0x00007fd91445d57e clang::Sema::SubstDefaultTemplateArgumentIfAvailable(clang::TemplateDecl*, clang::SourceLocation, clang::SourceLocation, clang::Decl*, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, bool&amp;) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1c5d57e)
#<!-- -->17 0x00007fd9144e3c61 clang::Sema::FinishTemplateArgumentDeduction(clang::FunctionTemplateDecl*, llvm::SmallVectorImpl&lt;clang::DeducedTemplateArgument&gt;&amp;, unsigned int, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, llvm::SmallVectorImpl&lt;clang::Sema::OriginalCallArg&gt; const*, bool, llvm::function_ref&lt;bool ()&gt;) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1ce3c61)
#<!-- -->18 0x00007fd91454001b (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1d4001b)
#<!-- -->19 0x00007fd913d93671 clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1593671)
#<!-- -->20 0x00007fd9144e5c1b clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, bool, bool, clang::QualType, clang::Expr::Classification, llvm::function_ref&lt;bool (llvm::ArrayRef&lt;clang::QualType&gt;)&gt;) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1ce5c1b)
#<!-- -->21 0x00007fd9143cb434 clang::Sema::AddTemplateOverloadCandidate(clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::OverloadCandidateSet&amp;, bool, bool, bool, clang::CallExpr::ADLCallKind, clang::OverloadCandidateParamOrder, bool) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1bcb434)
#<!-- -->22 0x00007fd9142a0847 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1aa0847)
#<!-- -->23 0x00007fd914293c39 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1a93c39)
#<!-- -->24 0x00007fd9142930d9 clang::Sema::DeduceTemplateSpecializationFromInitializer(clang::TypeSourceInfo*, clang::InitializedEntity const&amp;, clang::InitializationKind const&amp;, llvm::MutableArrayRef&lt;clang::Expr*&gt;) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1a930d9)
#<!-- -->25 0x00007fd913f4a1f2 clang::Sema::deduceVarTypeFromInitializer(clang::VarDecl*, clang::DeclarationName, clang::QualType, clang::TypeSourceInfo*, clang::SourceRange, bool, clang::Expr*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x174a1f2)
#<!-- -->26 0x00007fd913f4a920 clang::Sema::DeduceVariableDeclarationType(clang::VarDecl*, bool, clang::Expr*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x174a920)
#<!-- -->27 0x00007fd913f4b03c clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x174b03c)
#<!-- -->28 0x00007fd91344e3c1 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0xc4e3c1)
#<!-- -->29 0x00007fd91344b0c9 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0xc4b0c9)
#<!-- -->30 0x00007fd91344a4f5 clang::Parser::ParseSimpleDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, bool, clang::Parser::ForRangeInit*, clang::SourceLocation*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0xc4a4f5)
#<!-- -->31 0x00007fd913449e79 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0xc49e79)
#<!-- -->32 0x00007fd9134e9779 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0xce9779)
#<!-- -->33 0x00007fd9134e7a6a clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0xce7a6a)
#<!-- -->34 0x00007fd9134f05b0 clang::Parser::ParseCompoundStatementBody(bool) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0xcf05b0)
#<!-- -->35 0x00007fd9134f16f6 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0xcf16f6)
#<!-- -->36 0x00007fd91350b8e3 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0xd0b8e3)
#<!-- -->37 0x00007fd91344c46d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0xc4c46d)
#<!-- -->38 0x00007fd91350a8be clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0xd0a8be)
#<!-- -->39 0x00007fd91350a25f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0xd0a25f)
#<!-- -->40 0x00007fd91350944c clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0xd0944c)
#<!-- -->41 0x00007fd9135078d7 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0xd078d7)
#<!-- -->42 0x00007fd91343457e clang::ParseAST(clang::Sema&amp;, bool, bool) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0xc3457e)
#<!-- -->43 0x00007fd915547379 clang::FrontendAction::Execute() (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x2d47379)
#<!-- -->44 0x00007fd9154b5c74 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x2cb5c74)
#<!-- -->45 0x00007fd9155c57fe clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x2dc57fe)
#<!-- -->46 0x000055891017bc61 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/usr/lib/llvm-19/bin/clang+0x11c61)
#<!-- -->47 0x0000558910178b35 (/usr/lib/llvm-19/bin/clang+0xeb35)
#<!-- -->48 0x00007fd91513509d (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x293509d)
#<!-- -->49 0x00007fd90b7f4290 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.1+0xdf4290)
#<!-- -->50 0x00007fd915134b31 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x2934b31)
#<!-- -->51 0x00007fd9150f9ec8 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x28f9ec8)
#<!-- -->52 0x00007fd9150fa13e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x28fa13e)
#<!-- -->53 0x00007fd9151178fc clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x29178fc)
#<!-- -->54 0x0000558910178574 clang_main(int, char**, llvm::ToolContext const&amp;) (/usr/lib/llvm-19/bin/clang+0xe574)
#<!-- -->55 0x00005589101864ba main (/usr/lib/llvm-19/bin/clang+0x1c4ba)
#<!-- -->56 0x00007fd90a433d68 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->57 0x00007fd90a433e25 call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->58 0x00007fd90a433e25 __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->59 0x0000558910176731 _start (/usr/lib/llvm-19/bin/clang+0xc731)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Debian clang version 19.1.6 (1+b1)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/lib/llvm-19/bin
clang++: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: /tmp/main-8471ef.cpp
clang++: note: diagnostic msg: /tmp/main-8471ef.sh
clang++: note: diagnostic msg: 

********************
```
</details>


---

### Comment 2 - Endilll

Confirmed on 21.0 (trunk): https://godbolt.org/z/7M3aPs4bq
Reduced by C-Reduce:
```cpp
template < int >
using enable_if_t = int;

template < typename... _Types >
struct variant {
  template < enable_if_t< sizeof...(_Types) > >
  variant();
};

template <int>
using AstNode = variant<>;

AstNode tree;
```
Clang 21.0 crash:
```
clang++: /root/llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:1474:
static clang::TemplateArgument {anonymous}::TemplateInstantiator::getTemplateArgumentPackPatternForRewrite(const clang::TemplateArgument&):
Assertion `TA.pack_size() == 1 && "unexpected pack arguments in template rewrite"' failed.

PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++17 <source>
1.	<source>:13:13: current parser token ';'
 #0 0x0000000003e84598 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3e84598)
 #1 0x0000000003e82224 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3e82224)
 #2 0x0000000003dc70b8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x0000746995042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007469950969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x0000746995042476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007469950287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x000074699502871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x0000746995039e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000006ffa653 (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ffa653)
#10 0x0000000007003f40 clang::declvisitor::Base<std::add_pointer, clang::TemplateDeclInstantiator, clang::Decl*>::Visit(clang::Decl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7003f40)
#11 0x0000000007033d0d clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
#12 0x000000000703839a clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeSourceInfo*) SemaTemplateInstantiate.cpp:0:0
#13 0x000000000705575d (anonymous namespace)::TemplateInstantiator::TransformTemplateArgument(clang::TemplateArgumentLoc const&, clang::TemplateArgumentLoc&, bool) SemaTemplateInstantiate.cpp:0:0
#14 0x0000000007056685 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformSizeOfPackExpr(clang::SizeOfPackExpr*) SemaTemplateInstantiate.cpp:0:0
#15 0x000000000703c930 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#16 0x000000000703c3ba clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#17 0x00000000070556c7 (anonymous namespace)::TemplateInstantiator::TransformTemplateArgument(clang::TemplateArgumentLoc const&, clang::TemplateArgumentLoc&, bool) SemaTemplateInstantiate.cpp:0:0
#18 0x00000000070573d0 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformTemplateSpecializationType(clang::TypeLocBuilder&, clang::TemplateSpecializationTypeLoc, clang::TemplateName) SemaTemplateInstantiate.cpp:0:0
#19 0x0000000007034714 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
#20 0x000000000705d2f2 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformElaboratedType(clang::TypeLocBuilder&, clang::ElaboratedTypeLoc) SemaTemplateInstantiate.cpp:0:0
#21 0x0000000007033837 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
#22 0x000000000703839a clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeSourceInfo*) SemaTemplateInstantiate.cpp:0:0
#23 0x000000000705a096 clang::Sema::SubstType(clang::TypeSourceInfo*, clang::MultiLevelTemplateArgumentList const&, clang::SourceLocation, clang::DeclarationName, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x705a096)
#24 0x000000000709c9ed clang::TemplateDeclInstantiator::VisitNonTypeTemplateParmDecl(clang::NonTypeTemplateParmDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x709c9ed)
#25 0x00000000070fae14 void llvm::function_ref<void ()>::callback_fn<clang::Sema::SubstDecl(clang::Decl*, clang::DeclContext*, clang::MultiLevelTemplateArgumentList const&)::'lambda'()>(long) SemaTemplateInstantiateDecl.cpp:0:0
#26 0x0000000007d770b1 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7d770b1)
#27 0x000000000709999a clang::Sema::SubstDecl(clang::Decl*, clang::DeclContext*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x709999a)
#28 0x0000000006fad65a clang::NonTypeTemplateParmDecl* (anonymous namespace)::transformTemplateParam<clang::NonTypeTemplateParmDecl>(clang::Sema&, clang::DeclContext*, clang::NonTypeTemplateParmDecl*, clang::MultiLevelTemplateArgumentList&, unsigned int, unsigned int) SemaTemplateDeductionGuide.cpp:0:0
#29 0x0000000006fb0a00 (anonymous namespace)::transformTemplateParameter(clang::Sema&, clang::DeclContext*, clang::NamedDecl*, clang::MultiLevelTemplateArgumentList&, unsigned int, unsigned int, bool) (.constprop.0) SemaTemplateDeductionGuide.cpp:0:0
#30 0x0000000006fb49fc (anonymous namespace)::BuildDeductionGuideForTypeAlias(clang::Sema&, clang::TypeAliasTemplateDecl*, clang::FunctionTemplateDecl*, clang::SourceLocation) SemaTemplateDeductionGuide.cpp:0:0
#31 0x0000000006ff7e2d clang::Sema::DeclareImplicitDeductionGuides(clang::TemplateDecl*, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ff7e2d)
#32 0x0000000006bdccaf LookupDirect(clang::Sema&, clang::LookupResult&, clang::DeclContext const*) SemaLookup.cpp:0:0
#33 0x0000000006bc7c32 clang::Sema::LookupQualifiedName(clang::LookupResult&, clang::DeclContext*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bc7c32)
#34 0x0000000006ba612e clang::Sema::DeduceTemplateSpecializationFromInitializer(clang::TypeSourceInfo*, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::MutableArrayRef<clang::Expr*>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ba612e)
#35 0x0000000006826a7d clang::Sema::deduceVarTypeFromInitializer(clang::VarDecl*, clang::DeclarationName, clang::QualType, clang::TypeSourceInfo*, clang::SourceRange, bool, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6826a7d)
#36 0x000000000682b887 clang::Sema::DeduceVariableDeclarationType(clang::VarDecl*, bool, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x682b887)
#37 0x0000000006847161 clang::Sema::ActOnUninitializedDecl(clang::Decl*) (.part.0) SemaDecl.cpp:0:0
#38 0x00000000064d3f68 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64d3f68)
#39 0x00000000064e3b39 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64e3b39)
#40 0x00000000064a345e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64a345e)
#41 0x00000000064a3c19 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64a3c19)
#42 0x00000000064ab47a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64ab47a)
#43 0x00000000064ac3ed clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64ac3ed)
#44 0x000000000649e96a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x649e96a)
#45 0x000000000483fd48 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x483fd48)
#46 0x0000000004b0f825 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b0f825)
#47 0x0000000004a9196e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4a9196e)
#48 0x0000000004bff54e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bff54e)
#49 0x0000000000d4f6ff cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd4f6ff)
#50 0x0000000000d46eba ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#51 0x00000000048882a9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#52 0x0000000003dc7554 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3dc7554)
#53 0x00000000048888bf clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#54 0x000000000484b32d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x484b32d)
#55 0x000000000484c3be clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x484c3be)
#56 0x0000000004853a95 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4853a95)
#57 0x0000000000d4c4f8 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd4c4f8)
#58 0x0000000000c13dd4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc13dd4)
#59 0x0000746995029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#60 0x0000746995029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#61 0x0000000000d46965 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd46965)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
```

---

### Comment 3 - llvmbot


@llvm/issue-subscribers-c-17

Author: None (dvora124-wq)

<details>
Hi, 

I've been experimenting with ways to model an AST in modern C++. During one of these experiments, my compiler crashed on me. Per request of the compiler itself, I've attached the output and the associated culprit files below. 

The issue seems to arise from a leftover `template &lt;typename T&gt;`  declaration:
```
template &lt;typename T&gt;  // &lt;&lt;&lt; right here, sorry!
using AstNode = std::variant&lt;AstProgram, AstDecl, AstExpr, AstStmt&gt;;
```
I accidentally left it in place when I was restructuring the program. Once I remove the declaration, the program still doesn't compile (that's correct), but the compiler doesn't crash anymore.

---

[main-8471ef.tar.gz](https://github.com/user-attachments/files/18476429/main-8471ef.tar.gz)

```
Stack dump:
0.	Program arguments: clang++ -std=c++20 -Wall -Wextra -Wconversion -Wpedantic -funsigned-char -fsanitize=address,undefined -fno-omit-frame-pointer -g3 -glldb -O0 -c -o target/main.o src/main.cc
1.	src/main.cc:69:112: current parser token ';'
2.	src/main.cc:68:1: parsing function body 'main'
3.	src/main.cc:68:1: in compound statement ('{}')
 #<!-- -->0 0x00007fd90b8b0a8a llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.1+0xeb0a8a)
 #<!-- -->1 0x00007fd90b8ae634 llvm::sys::RunSignalHandlers() (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.1+0xeae634)
 #<!-- -->2 0x00007fd90b7f44f4 (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.1+0xdf44f4)
 #<!-- -->3 0x00007fd90a449da0 (/lib/x86_64-linux-gnu/libc.so.6+0x3fda0)
 #<!-- -->4 0x00007fd913a850e4 clang::TemplateArgument::isPackExpansion() const (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x12850e4)
 #<!-- -->5 0x00007fd914631acb clang::Sema::getFullyPackExpandedSize(clang::TemplateArgument) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1e31acb)
 #<!-- -->6 0x00007fd9145bf020 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1dbf020)
 #<!-- -->7 0x00007fd9145c8bcb (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1dc8bcb)
 #<!-- -->8 0x00007fd91459a425 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1d9a425)
 #<!-- -->9 0x00007fd9145d2ab0 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1dd2ab0)
#<!-- -->10 0x00007fd9145d9867 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1dd9867)
#<!-- -->11 0x00007fd9145d6d6d (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1dd6d6d)
#<!-- -->12 0x00007fd91459348b (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1d9348b)
#<!-- -->13 0x00007fd91459a3a7 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1d9a3a7)
#<!-- -->14 0x00007fd91459a232 clang::Sema::SubstTemplateArgument(clang::TemplateArgumentLoc const&amp;, clang::MultiLevelTemplateArgumentList const&amp;, clang::TemplateArgumentLoc&amp;, clang::SourceLocation, clang::DeclarationName const&amp;) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1d9a232)
#<!-- -->15 0x00007fd91445daab (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1c5daab)
#<!-- -->16 0x00007fd91445d57e clang::Sema::SubstDefaultTemplateArgumentIfAvailable(clang::TemplateDecl*, clang::SourceLocation, clang::SourceLocation, clang::Decl*, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, bool&amp;) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1c5d57e)
#<!-- -->17 0x00007fd9144e3c61 clang::Sema::FinishTemplateArgumentDeduction(clang::FunctionTemplateDecl*, llvm::SmallVectorImpl&lt;clang::DeducedTemplateArgument&gt;&amp;, unsigned int, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, llvm::SmallVectorImpl&lt;clang::Sema::OriginalCallArg&gt; const*, bool, llvm::function_ref&lt;bool ()&gt;) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1ce3c61)
#<!-- -->18 0x00007fd91454001b (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1d4001b)
#<!-- -->19 0x00007fd913d93671 clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1593671)
#<!-- -->20 0x00007fd9144e5c1b clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, bool, bool, clang::QualType, clang::Expr::Classification, llvm::function_ref&lt;bool (llvm::ArrayRef&lt;clang::QualType&gt;)&gt;) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1ce5c1b)
#<!-- -->21 0x00007fd9143cb434 clang::Sema::AddTemplateOverloadCandidate(clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::OverloadCandidateSet&amp;, bool, bool, bool, clang::CallExpr::ADLCallKind, clang::OverloadCandidateParamOrder, bool) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1bcb434)
#<!-- -->22 0x00007fd9142a0847 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1aa0847)
#<!-- -->23 0x00007fd914293c39 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1a93c39)
#<!-- -->24 0x00007fd9142930d9 clang::Sema::DeduceTemplateSpecializationFromInitializer(clang::TypeSourceInfo*, clang::InitializedEntity const&amp;, clang::InitializationKind const&amp;, llvm::MutableArrayRef&lt;clang::Expr*&gt;) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1a930d9)
#<!-- -->25 0x00007fd913f4a1f2 clang::Sema::deduceVarTypeFromInitializer(clang::VarDecl*, clang::DeclarationName, clang::QualType, clang::TypeSourceInfo*, clang::SourceRange, bool, clang::Expr*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x174a1f2)
#<!-- -->26 0x00007fd913f4a920 clang::Sema::DeduceVariableDeclarationType(clang::VarDecl*, bool, clang::Expr*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x174a920)
#<!-- -->27 0x00007fd913f4b03c clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x174b03c)
#<!-- -->28 0x00007fd91344e3c1 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0xc4e3c1)
#<!-- -->29 0x00007fd91344b0c9 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0xc4b0c9)
#<!-- -->30 0x00007fd91344a4f5 clang::Parser::ParseSimpleDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, bool, clang::Parser::ForRangeInit*, clang::SourceLocation*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0xc4a4f5)
#<!-- -->31 0x00007fd913449e79 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0xc49e79)
#<!-- -->32 0x00007fd9134e9779 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0xce9779)
#<!-- -->33 0x00007fd9134e7a6a clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0xce7a6a)
#<!-- -->34 0x00007fd9134f05b0 clang::Parser::ParseCompoundStatementBody(bool) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0xcf05b0)
#<!-- -->35 0x00007fd9134f16f6 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0xcf16f6)
#<!-- -->36 0x00007fd91350b8e3 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0xd0b8e3)
#<!-- -->37 0x00007fd91344c46d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0xc4c46d)
#<!-- -->38 0x00007fd91350a8be clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0xd0a8be)
#<!-- -->39 0x00007fd91350a25f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0xd0a25f)
#<!-- -->40 0x00007fd91350944c clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0xd0944c)
#<!-- -->41 0x00007fd9135078d7 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0xd078d7)
#<!-- -->42 0x00007fd91343457e clang::ParseAST(clang::Sema&amp;, bool, bool) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0xc3457e)
#<!-- -->43 0x00007fd915547379 clang::FrontendAction::Execute() (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x2d47379)
#<!-- -->44 0x00007fd9154b5c74 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x2cb5c74)
#<!-- -->45 0x00007fd9155c57fe clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x2dc57fe)
#<!-- -->46 0x000055891017bc61 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/usr/lib/llvm-19/bin/clang+0x11c61)
#<!-- -->47 0x0000558910178b35 (/usr/lib/llvm-19/bin/clang+0xeb35)
#<!-- -->48 0x00007fd91513509d (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x293509d)
#<!-- -->49 0x00007fd90b7f4290 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.1+0xdf4290)
#<!-- -->50 0x00007fd915134b31 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x2934b31)
#<!-- -->51 0x00007fd9150f9ec8 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x28f9ec8)
#<!-- -->52 0x00007fd9150fa13e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x28fa13e)
#<!-- -->53 0x00007fd9151178fc clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x29178fc)
#<!-- -->54 0x0000558910178574 clang_main(int, char**, llvm::ToolContext const&amp;) (/usr/lib/llvm-19/bin/clang+0xe574)
#<!-- -->55 0x00005589101864ba main (/usr/lib/llvm-19/bin/clang+0x1c4ba)
#<!-- -->56 0x00007fd90a433d68 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->57 0x00007fd90a433e25 call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->58 0x00007fd90a433e25 __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->59 0x0000558910176731 _start (/usr/lib/llvm-19/bin/clang+0xc731)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Debian clang version 19.1.6 (1+b1)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/lib/llvm-19/bin
clang++: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: /tmp/main-8471ef.cpp
clang++: note: diagnostic msg: /tmp/main-8471ef.sh
clang++: note: diagnostic msg: 

********************
```
</details>


---

### Comment 4 - Endilll

Clang 18 is rejecting, Clang 19 is rejecting for another reason: https://godbolt.org/z/feE1advM6
Other compilers reject, too: https://godbolt.org/z/KnGdGWr4q

---

### Comment 5 - Endilll

Combination of CTAD and aliases reminds me of #130604

---

### Comment 6 - shafik

CC @hokein 

---

### Comment 7 - shafik

@zyn0217 git bisect points to fd4f94ddbf0c0f0c9d0185e6036fe51de5ab2ef3 as the cause for this regression

---

### Comment 8 - zyn0217

Here is a similar and valid example that has caused a crash since version 19:

```cpp
template < typename... _Types >
struct variant {
  template <int N = sizeof...(_Types)>
  variant(_Types...);
};

template <class T>
using AstNode = variant<T>;

AstNode tree(42);
```

https://godbolt.org/z/4rnG8fPah

we don't seem to handle non-pack substitutions into parameter packs very well

---

