# Clang trunk crash: infinite macro expansion leads to stack overflow during macro argument expansion

**Author:** mariete1223
**URL:** https://github.com/llvm/llvm-project/issues/137863
**Status:** Closed
**Labels:** duplicate, clang:frontend, crash
**Closed Date:** 2025-04-30T00:00:55Z

## Body

## Summary

Clang (trunk version) crashes when compiling a C program with heavily nested recursive macro expansions.
Instead of cleanly rejecting the expansion due to excessive depth, the compiler crashes deep inside the preprocessor (TokenLexer::ExpandFunctionArguments) causing a fatal error.

## Stack dump

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x c -ferror-limit=100 <source>
1.	<source>:16:9 <Spelling=<source>:9:13>: current parser token 'CAT7'
2.	<source>:11:13: parsing function body 'main'
3.	<source>:11:13: in compound statement ('{}')
 #0 0x0000000003f1c8c8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f1c8c8)
 #1 0x0000000003f1a554 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f1a554)
 #2 0x0000000003e5f5e6 llvm::CrashRecoveryContext::HandleExit(int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3e5f5e6)
 #3 0x0000000003f11c7e llvm::sys::Process::Exit(int, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f11c7e)
 #4 0x0000000000da0220 LLVMErrorHandler(void*, char const*, bool) cc1_main.cpp:0:0
 #5 0x0000000003e6a2c3 llvm::report_fatal_error(llvm::Twine const&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3e6a2c3)
 #6 0x0000000003e6a428 (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3e6a428)
 #7 0x0000000004174efb clang::SourceManager::createExpansionLocImpl(clang::SrcMgr::ExpansionInfo const&, unsigned int, int, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4174efb)
 #8 0x000000000417567b clang::SourceManager::createMacroArgExpansionLoc(clang::SourceLocation, clang::SourceLocation, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x417567b)
 #9 0x0000000007df022c clang::TokenLexer::updateLocForMacroArgTokens(clang::SourceLocation, clang::Token*, clang::Token*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7df022c)
#10 0x0000000007df1b2b clang::TokenLexer::ExpandFunctionArguments() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7df1b2b)
#11 0x0000000007df224f clang::TokenLexer::Init(clang::Token&, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7df224f)
#12 0x0000000007dade51 clang::Preprocessor::EnterMacro(clang::Token&, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7dade51)
#13 0x0000000007dc6319 clang::Preprocessor::HandleMacroExpandedIdentifier(clang::Token&, clang::MacroDefinition const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7dc6319)
#14 0x0000000007de5b78 clang::Preprocessor::HandleIdentifier(clang::Token&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7de5b78)
#15 0x0000000007de801c clang::Preprocessor::Lex(clang::Token&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7de801c)
#16 0x0000000007df384b clang::MacroArgs::getPreExpArgument(unsigned int, clang::Preprocessor&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7df384b)
#17 0x0000000007df143d clang::TokenLexer::ExpandFunctionArguments() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7df143d)
#18 0x0000000007df224f clang::TokenLexer::Init(clang::Token&, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7df224f)
#19 0x0000000007dade51 clang::Preprocessor::EnterMacro(clang::Token&, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7dade51)
#20 0x0000000007dc6319 clang::Preprocessor::HandleMacroExpandedIdentifier(clang::Token&, clang::MacroDefinition const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7dc6319)
#21 0x0000000007de5b78 clang::Preprocessor::HandleIdentifier(clang::Token&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7de5b78)
#22 0x0000000007de801c clang::Preprocessor::Lex(clang::Token&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7de801c)
#23 0x0000000007df384b clang::MacroArgs::getPreExpArgument(unsigned int, clang::Preprocessor&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7df384b)
#24 0x0000000007df143d clang::TokenLexer::ExpandFunctionArguments() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7df143d)
#25 0x0000000007df224f clang::TokenLexer::Init(clang::Token&, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7df224f)
#26 0x0000000007dade51 clang::Preprocessor::EnterMacro(clang::Token&, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7dade51)
#27 0x0000000007dc6319 clang::Preprocessor::HandleMacroExpandedIdentifier(clang::Token&, clang::MacroDefinition const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7dc6319)
#28 0x0000000007de5b78 clang::Preprocessor::HandleIdentifier(clang::Token&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7de5b78)
#29 0x0000000007de801c clang::Preprocessor::Lex(clang::Token&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7de801c)
#30 0x0000000007df384b clang::MacroArgs::getPreExpArgument(unsigned int, clang::Preprocessor&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7df384b)
#31 0x0000000007df143d clang::TokenLexer::ExpandFunctionArguments() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7df143d)
#32 0x0000000007df224f clang::TokenLexer::Init(clang::Token&, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7df224f)
#33 0x0000000007dade51 clang::Preprocessor::EnterMacro(clang::Token&, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7dade51)
#34 0x0000000007dc6319 clang::Preprocessor::HandleMacroExpandedIdentifier(clang::Token&, clang::MacroDefinition const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7dc6319)
#35 0x0000000007de5b78 clang::Preprocessor::HandleIdentifier(clang::Token&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7de5b78)
#36 0x0000000007de801c clang::Preprocessor::Lex(clang::Token&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7de801c)
#37 0x0000000007df384b clang::MacroArgs::getPreExpArgument(unsigned int, clang::Preprocessor&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7df384b)
#38 0x0000000007df143d clang::TokenLexer::ExpandFunctionArguments() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7df143d)
#39 0x0000000007df224f clang::TokenLexer::Init(clang::Token&, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7df224f)
#40 0x0000000007dade51 clang::Preprocessor::EnterMacro(clang::Token&, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7dade51)
#41 0x0000000007dc6319 clang::Preprocessor::HandleMacroExpandedIdentifier(clang::Token&, clang::MacroDefinition const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7dc6319)
#42 0x0000000007de5b78 clang::Preprocessor::HandleIdentifier(clang::Token&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7de5b78)
#43 0x0000000007de801c clang::Preprocessor::Lex(clang::Token&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7de801c)
#44 0x0000000007df384b clang::MacroArgs::getPreExpArgument(unsigned int, clang::Preprocessor&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7df384b)
#45 0x0000000007df143d clang::TokenLexer::ExpandFunctionArguments() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7df143d)
#46 0x0000000007df224f clang::TokenLexer::Init(clang::Token&, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7df224f)
#47 0x0000000007dade51 clang::Preprocessor::EnterMacro(clang::Token&, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7dade51)
#48 0x0000000007dc6319 clang::Preprocessor::HandleMacroExpandedIdentifier(clang::Token&, clang::MacroDefinition const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7dc6319)
#49 0x0000000007de5b78 clang::Preprocessor::HandleIdentifier(clang::Token&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7de5b78)
#50 0x0000000007de801c clang::Preprocessor::Lex(clang::Token&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7de801c)
#51 0x0000000007df384b clang::MacroArgs::getPreExpArgument(unsigned int, clang::Preprocessor&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7df384b)
#52 0x0000000007df143d clang::TokenLexer::ExpandFunctionArguments() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7df143d)
#53 0x0000000007df224f clang::TokenLexer::Init(clang::Token&, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7df224f)
#54 0x0000000007dade51 clang::Preprocessor::EnterMacro(clang::Token&, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7dade51)
#55 0x0000000007dc6319 clang::Preprocessor::HandleMacroExpandedIdentifier(clang::Token&, clang::MacroDefinition const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7dc6319)
#56 0x0000000007de5b78 clang::Preprocessor::HandleIdentifier(clang::Token&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7de5b78)
#57 0x0000000007de801c clang::Preprocessor::Lex(clang::Token&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7de801c)
#58 0x0000000007df384b clang::MacroArgs::getPreExpArgument(unsigned int, clang::Preprocessor&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7df384b)
#59 0x0000000007df143d clang::TokenLexer::ExpandFunctionArguments() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7df143d)
#60 0x0000000007df224f clang::TokenLexer::Init(clang::Token&, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7df224f)
#61 0x0000000007dade51 clang::Preprocessor::EnterMacro(clang::Token&, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7dade51)
#62 0x0000000007dc6319 clang::Preprocessor::HandleMacroExpandedIdentifier(clang::Token&, clang::MacroDefinition const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7dc6319)
#63 0x0000000007de5b78 clang::Preprocessor::HandleIdentifier(clang::Token&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7de5b78)
#64 0x0000000007de801c clang::Preprocessor::Lex(clang::Token&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7de801c)
#65 0x0000000006589ee6 clang::Parser::ExpectAndConsumeSemi(unsigned int, llvm::StringRef) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6589ee6)
#66 0x0000000006687c06 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6687c06)
#67 0x000000000667f642 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x667f642)
#68 0x000000000668056d clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x668056d)
#69 0x0000000006688273 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6688273)
#70 0x0000000006688a3a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6688a3a)
#71 0x0000000006591833 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6591833)
#72 0x00000000065c78fd clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65c78fd)
#73 0x000000000658536e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x658536e)
#74 0x0000000006585b29 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6585b29)
#75 0x000000000658d44a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x658d44a)
#76 0x000000000658e3ed clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x658e3ed)
#77 0x00000000065807ca clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65807ca)
#78 0x00000000048bc388 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48bc388)
#79 0x0000000004bb0125 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bb0125)
#80 0x0000000004b2e5ce clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b2e5ce)
#81 0x0000000004c9f999 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c9f999)
#82 0x0000000000da28df cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda28df)
#83 0x0000000000d998ea ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#84 0x0000000004922c19 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#85 0x0000000003e5f524 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3e5f524)
#86 0x000000000492322f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#87 0x00000000048e568d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48e568d)
#88 0x00000000048e671e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48e671e)
#89 0x00000000048ee575 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48ee575)
#90 0x0000000000d9f628 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd9f628)
#91 0x0000000000c25054 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc25054)
#92 0x00007701c8029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#93 0x00007701c8029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#94 0x0000000000d99395 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd99395)
clang++: error: clang frontend command failed with exit code 70 (use -v to see invocation)
Compiler returned: 70
```

## Program

```
#include <stdlib.h>
#define CAT(x, y) x ## y
#define CAT2(x, y) CAT(x, y)
#define CAT3(x, y) CAT2( CAT2(x, y), CAT2(x, y) )
#define CAT4(x, y) CAT3( CAT3(x, y), CAT2(x, y) )
#define CAT5(x, y) CAT4( CAT4(x, y), CAT2(x, y) )
#define CAT6(x, y) CAT5( CAT5(x, y), CAT2(x, y) )
#define CAT7(x, y) CAT6( CAT6(x, y), CAT2(x, y) )
#define F() CAT7( F, CAT7( F, CAT7( F, CAT7( F, CAT7( F, CAT7( F, END_F ) ) ) ) ) )
#define END_F 0
int main()  {
        int* d = malloc(7 * sizeof(int));
        int* e = malloc(7 * sizeof(int));
        d = (int*)((unsigned long)d % 32);
        e = (int*)((unsigned long)e % 32);
        F()(d, e, 9);
```

## To quickly reproduce

https://gcc.godbolt.org/z/Kh5xdajWs

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (mariete1223)

<details>
## Summary

Clang (trunk version) crashes when compiling a C program with heavily nested recursive macro expansions.
Instead of cleanly rejecting the expansion due to excessive depth, the compiler crashes deep inside the preprocessor (TokenLexer::ExpandFunctionArguments) causing a fatal error.

## Stack dump

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x c -ferror-limit=100 &lt;source&gt;
1.	&lt;source&gt;:16:9 &lt;Spelling=&lt;source&gt;:9:13&gt;: current parser token 'CAT7'
2.	&lt;source&gt;:11:13: parsing function body 'main'
3.	&lt;source&gt;:11:13: in compound statement ('{}')
 #<!-- -->0 0x0000000003f1c8c8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f1c8c8)
 #<!-- -->1 0x0000000003f1a554 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f1a554)
 #<!-- -->2 0x0000000003e5f5e6 llvm::CrashRecoveryContext::HandleExit(int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3e5f5e6)
 #<!-- -->3 0x0000000003f11c7e llvm::sys::Process::Exit(int, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f11c7e)
 #<!-- -->4 0x0000000000da0220 LLVMErrorHandler(void*, char const*, bool) cc1_main.cpp:0:0
 #<!-- -->5 0x0000000003e6a2c3 llvm::report_fatal_error(llvm::Twine const&amp;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3e6a2c3)
 #<!-- -->6 0x0000000003e6a428 (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3e6a428)
 #<!-- -->7 0x0000000004174efb clang::SourceManager::createExpansionLocImpl(clang::SrcMgr::ExpansionInfo const&amp;, unsigned int, int, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4174efb)
 #<!-- -->8 0x000000000417567b clang::SourceManager::createMacroArgExpansionLoc(clang::SourceLocation, clang::SourceLocation, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x417567b)
 #<!-- -->9 0x0000000007df022c clang::TokenLexer::updateLocForMacroArgTokens(clang::SourceLocation, clang::Token*, clang::Token*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7df022c)
#<!-- -->10 0x0000000007df1b2b clang::TokenLexer::ExpandFunctionArguments() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7df1b2b)
#<!-- -->11 0x0000000007df224f clang::TokenLexer::Init(clang::Token&amp;, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7df224f)
#<!-- -->12 0x0000000007dade51 clang::Preprocessor::EnterMacro(clang::Token&amp;, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7dade51)
#<!-- -->13 0x0000000007dc6319 clang::Preprocessor::HandleMacroExpandedIdentifier(clang::Token&amp;, clang::MacroDefinition const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7dc6319)
#<!-- -->14 0x0000000007de5b78 clang::Preprocessor::HandleIdentifier(clang::Token&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7de5b78)
#<!-- -->15 0x0000000007de801c clang::Preprocessor::Lex(clang::Token&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7de801c)
#<!-- -->16 0x0000000007df384b clang::MacroArgs::getPreExpArgument(unsigned int, clang::Preprocessor&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7df384b)
#<!-- -->17 0x0000000007df143d clang::TokenLexer::ExpandFunctionArguments() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7df143d)
#<!-- -->18 0x0000000007df224f clang::TokenLexer::Init(clang::Token&amp;, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7df224f)
#<!-- -->19 0x0000000007dade51 clang::Preprocessor::EnterMacro(clang::Token&amp;, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7dade51)
#<!-- -->20 0x0000000007dc6319 clang::Preprocessor::HandleMacroExpandedIdentifier(clang::Token&amp;, clang::MacroDefinition const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7dc6319)
#<!-- -->21 0x0000000007de5b78 clang::Preprocessor::HandleIdentifier(clang::Token&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7de5b78)
#<!-- -->22 0x0000000007de801c clang::Preprocessor::Lex(clang::Token&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7de801c)
#<!-- -->23 0x0000000007df384b clang::MacroArgs::getPreExpArgument(unsigned int, clang::Preprocessor&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7df384b)
#<!-- -->24 0x0000000007df143d clang::TokenLexer::ExpandFunctionArguments() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7df143d)
#<!-- -->25 0x0000000007df224f clang::TokenLexer::Init(clang::Token&amp;, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7df224f)
#<!-- -->26 0x0000000007dade51 clang::Preprocessor::EnterMacro(clang::Token&amp;, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7dade51)
#<!-- -->27 0x0000000007dc6319 clang::Preprocessor::HandleMacroExpandedIdentifier(clang::Token&amp;, clang::MacroDefinition const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7dc6319)
#<!-- -->28 0x0000000007de5b78 clang::Preprocessor::HandleIdentifier(clang::Token&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7de5b78)
#<!-- -->29 0x0000000007de801c clang::Preprocessor::Lex(clang::Token&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7de801c)
#<!-- -->30 0x0000000007df384b clang::MacroArgs::getPreExpArgument(unsigned int, clang::Preprocessor&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7df384b)
#<!-- -->31 0x0000000007df143d clang::TokenLexer::ExpandFunctionArguments() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7df143d)
#<!-- -->32 0x0000000007df224f clang::TokenLexer::Init(clang::Token&amp;, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7df224f)
#<!-- -->33 0x0000000007dade51 clang::Preprocessor::EnterMacro(clang::Token&amp;, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7dade51)
#<!-- -->34 0x0000000007dc6319 clang::Preprocessor::HandleMacroExpandedIdentifier(clang::Token&amp;, clang::MacroDefinition const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7dc6319)
#<!-- -->35 0x0000000007de5b78 clang::Preprocessor::HandleIdentifier(clang::Token&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7de5b78)
#<!-- -->36 0x0000000007de801c clang::Preprocessor::Lex(clang::Token&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7de801c)
#<!-- -->37 0x0000000007df384b clang::MacroArgs::getPreExpArgument(unsigned int, clang::Preprocessor&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7df384b)
#<!-- -->38 0x0000000007df143d clang::TokenLexer::ExpandFunctionArguments() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7df143d)
#<!-- -->39 0x0000000007df224f clang::TokenLexer::Init(clang::Token&amp;, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7df224f)
#<!-- -->40 0x0000000007dade51 clang::Preprocessor::EnterMacro(clang::Token&amp;, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7dade51)
#<!-- -->41 0x0000000007dc6319 clang::Preprocessor::HandleMacroExpandedIdentifier(clang::Token&amp;, clang::MacroDefinition const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7dc6319)
#<!-- -->42 0x0000000007de5b78 clang::Preprocessor::HandleIdentifier(clang::Token&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7de5b78)
#<!-- -->43 0x0000000007de801c clang::Preprocessor::Lex(clang::Token&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7de801c)
#<!-- -->44 0x0000000007df384b clang::MacroArgs::getPreExpArgument(unsigned int, clang::Preprocessor&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7df384b)
#<!-- -->45 0x0000000007df143d clang::TokenLexer::ExpandFunctionArguments() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7df143d)
#<!-- -->46 0x0000000007df224f clang::TokenLexer::Init(clang::Token&amp;, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7df224f)
#<!-- -->47 0x0000000007dade51 clang::Preprocessor::EnterMacro(clang::Token&amp;, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7dade51)
#<!-- -->48 0x0000000007dc6319 clang::Preprocessor::HandleMacroExpandedIdentifier(clang::Token&amp;, clang::MacroDefinition const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7dc6319)
#<!-- -->49 0x0000000007de5b78 clang::Preprocessor::HandleIdentifier(clang::Token&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7de5b78)
#<!-- -->50 0x0000000007de801c clang::Preprocessor::Lex(clang::Token&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7de801c)
#<!-- -->51 0x0000000007df384b clang::MacroArgs::getPreExpArgument(unsigned int, clang::Preprocessor&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7df384b)
#<!-- -->52 0x0000000007df143d clang::TokenLexer::ExpandFunctionArguments() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7df143d)
#<!-- -->53 0x0000000007df224f clang::TokenLexer::Init(clang::Token&amp;, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7df224f)
#<!-- -->54 0x0000000007dade51 clang::Preprocessor::EnterMacro(clang::Token&amp;, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7dade51)
#<!-- -->55 0x0000000007dc6319 clang::Preprocessor::HandleMacroExpandedIdentifier(clang::Token&amp;, clang::MacroDefinition const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7dc6319)
#<!-- -->56 0x0000000007de5b78 clang::Preprocessor::HandleIdentifier(clang::Token&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7de5b78)
#<!-- -->57 0x0000000007de801c clang::Preprocessor::Lex(clang::Token&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7de801c)
#<!-- -->58 0x0000000007df384b clang::MacroArgs::getPreExpArgument(unsigned int, clang::Preprocessor&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7df384b)
#<!-- -->59 0x0000000007df143d clang::TokenLexer::ExpandFunctionArguments() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7df143d)
#<!-- -->60 0x0000000007df224f clang::TokenLexer::Init(clang::Token&amp;, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7df224f)
#<!-- -->61 0x0000000007dade51 clang::Preprocessor::EnterMacro(clang::Token&amp;, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7dade51)
#<!-- -->62 0x0000000007dc6319 clang::Preprocessor::HandleMacroExpandedIdentifier(clang::Token&amp;, clang::MacroDefinition const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7dc6319)
#<!-- -->63 0x0000000007de5b78 clang::Preprocessor::HandleIdentifier(clang::Token&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7de5b78)
#<!-- -->64 0x0000000007de801c clang::Preprocessor::Lex(clang::Token&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7de801c)
#<!-- -->65 0x0000000006589ee6 clang::Parser::ExpectAndConsumeSemi(unsigned int, llvm::StringRef) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6589ee6)
#<!-- -->66 0x0000000006687c06 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6687c06)
#<!-- -->67 0x000000000667f642 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x667f642)
#<!-- -->68 0x000000000668056d clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x668056d)
#<!-- -->69 0x0000000006688273 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6688273)
#<!-- -->70 0x0000000006688a3a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6688a3a)
#<!-- -->71 0x0000000006591833 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6591833)
#<!-- -->72 0x00000000065c78fd clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65c78fd)
#<!-- -->73 0x000000000658536e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x658536e)
#<!-- -->74 0x0000000006585b29 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6585b29)
#<!-- -->75 0x000000000658d44a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x658d44a)
#<!-- -->76 0x000000000658e3ed clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x658e3ed)
#<!-- -->77 0x00000000065807ca clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65807ca)
#<!-- -->78 0x00000000048bc388 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48bc388)
#<!-- -->79 0x0000000004bb0125 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bb0125)
#<!-- -->80 0x0000000004b2e5ce clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b2e5ce)
#<!-- -->81 0x0000000004c9f999 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c9f999)
#<!-- -->82 0x0000000000da28df cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda28df)
#<!-- -->83 0x0000000000d998ea ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->84 0x0000000004922c19 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->85 0x0000000003e5f524 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3e5f524)
#<!-- -->86 0x000000000492322f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->87 0x00000000048e568d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48e568d)
#<!-- -->88 0x00000000048e671e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48e671e)
#<!-- -->89 0x00000000048ee575 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48ee575)
#<!-- -->90 0x0000000000d9f628 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd9f628)
#<!-- -->91 0x0000000000c25054 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc25054)
#<!-- -->92 0x00007701c8029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->93 0x00007701c8029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->94 0x0000000000d99395 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd99395)
clang++: error: clang frontend command failed with exit code 70 (use -v to see invocation)
Compiler returned: 70
```

## Program

```
#include &lt;stdlib.h&gt;
#define CAT(x, y) x ## y
#define CAT2(x, y) CAT(x, y)
#define CAT3(x, y) CAT2( CAT2(x, y), CAT2(x, y) )
#define CAT4(x, y) CAT3( CAT3(x, y), CAT2(x, y) )
#define CAT5(x, y) CAT4( CAT4(x, y), CAT2(x, y) )
#define CAT6(x, y) CAT5( CAT5(x, y), CAT2(x, y) )
#define CAT7(x, y) CAT6( CAT6(x, y), CAT2(x, y) )
#define F() CAT7( F, CAT7( F, CAT7( F, CAT7( F, CAT7( F, CAT7( F, END_F ) ) ) ) ) )
#define END_F 0
int main()  {
        int* d = malloc(7 * sizeof(int));
        int* e = malloc(7 * sizeof(int));
        d = (int*)((unsigned long)d % 32);
        e = (int*)((unsigned long)e % 32);
        F()(d, e, 9);
```

## To quickly reproduce

https://gcc.godbolt.org/z/Kh5xdajWs
</details>


---

### Comment 2 - shafik

Duplicate of: https://github.com/llvm/llvm-project/issues/23843

---

