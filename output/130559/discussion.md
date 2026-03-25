# [clang] Assertion `!(Offset & (1ULL << OffsetBits)) && "Offset is too large"' failed

**Author:** ym-xie
**URL:** https://github.com/llvm/llvm-project/issues/130559

## Body

code:
```c
#define max(X, Y) (((X) >= (Y)) ? (X) : (Y))
void main(void) {
  float x = max(
      x,
      max(x,
          max(x,
              max(x,
                  max(x,
                      max(x,
                          max(x,
                              max(x,
                                  max(x,
                                      max(x,
                                          max(x,
                                              max(x,
                                                  max(x,
                                                      max(x,
                                                          max(x,
                                                              max(x,
                                                                  max(x,
                                                                      max(x,
                                                                          max(x,
                                                                              max(x,
                                                                                  max(x,
                                                                                      max(x,
                                                                                          max(x,
                                                                                              max(x,
                                                                                                  max(x,
                                                                                                      max(x,
                                                                                                          max(x,
                                                                                                              max(x,
                                                                                                                  x))))))))))))))))))))))))))));
```
command:
```shell
$ clang 979-reduced.c
979-reduced.c:2:1: warning: return type of 'main' is not 'int' [-Wmain-return-type]
    2 | void main(void) {
      | ^
979-reduced.c:2:1: note: change return type to 'int'
    2 | void main(void) {
      | ^~~~
      | int
clang-21: /data/xieym/llm/src/llvm-project/clang/include/clang/Basic/SourceManager.h:532: static clang::SrcMgr::SLocEntry clang::SrcMgr::SLocEntry::get(clang::SourceLocation::UIntTy, const clang::SrcMgr::Expansi
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /data/xieym/llm/install/llvm-trunk/bin/clang-21 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -main-file-name 979-reduced.c -mrelo
1.      979-reduced.c:3:13: current parser token 'max'
2.      979-reduced.c:2:17: parsing function body 'main'
3.      979-reduced.c:2:17: in compound statement ('{}')
 #0 0x0000557261d57020 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x455f020)
 #1 0x0000557261d5441f llvm::sys::RunSignalHandlers() (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x455c41f)
 #2 0x0000557261d5456a SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x00007f3e6b602520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007f3e6b6569fc __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x00007f3e6b6569fc __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x00007f3e6b6569fc pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x00007f3e6b602476 gsignal ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x00007f3e6b5e87f3 abort ./stdlib/abort.c:81:7
 #9 0x00007f3e6b5e871b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#10 0x00007f3e6b5f9e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
#11 0x0000557261fbcf7a clang::SourceManager::createExpansionLocImpl(clang::SrcMgr::ExpansionInfo const&, unsigned int, int, unsigned int) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x47c4f7a)
#12 0x0000557261fbd759 clang::SourceManager::createMacroArgExpansionLoc(clang::SourceLocation, clang::SourceLocation, unsigned int) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x47c5759)
#13 0x0000557265922667 clang::TokenLexer::updateLocForMacroArgTokens(clang::SourceLocation, clang::Token*, clang::Token*) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x812a667)
#14 0x00005572659268ee clang::TokenLexer::ExpandFunctionArguments() (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x812e8ee)
#15 0x0000557265926f0f clang::TokenLexer::Init(clang::Token&, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x812ef0f)
#16 0x00005572658e5c55 clang::Preprocessor::EnterMacro(clang::Token&, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x80edc55)
#17 0x00005572658fd46e clang::Preprocessor::HandleMacroExpandedIdentifier(clang::Token&, clang::MacroDefinition const&) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x810546e)
#18 0x000055726591a822 clang::Preprocessor::HandleIdentifier(clang::Token&) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x8122822)
#19 0x000055726591cccc clang::Preprocessor::Lex(clang::Token&) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x8124ccc)
#20 0x0000557265928513 clang::MacroArgs::getPreExpArgument(unsigned int, clang::Preprocessor&) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x8130513)
#21 0x000055726592613d clang::TokenLexer::ExpandFunctionArguments() (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x812e13d)
#22 0x0000557265926f0f clang::TokenLexer::Init(clang::Token&, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x812ef0f)
#23 0x00005572658e5c55 clang::Preprocessor::EnterMacro(clang::Token&, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x80edc55)
#24 0x00005572658fd46e clang::Preprocessor::HandleMacroExpandedIdentifier(clang::Token&, clang::MacroDefinition const&) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x810546e)
#25 0x000055726591a822 clang::Preprocessor::HandleIdentifier(clang::Token&) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x8122822)
#26 0x000055726591cccc clang::Preprocessor::Lex(clang::Token&) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x8124ccc)
#27 0x0000557265928513 clang::MacroArgs::getPreExpArgument(unsigned int, clang::Preprocessor&) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x8130513)
#28 0x000055726592613d clang::TokenLexer::ExpandFunctionArguments() (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x812e13d)
#29 0x0000557265926f0f clang::TokenLexer::Init(clang::Token&, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x812ef0f)
#30 0x00005572658e5c55 clang::Preprocessor::EnterMacro(clang::Token&, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x80edc55)
#31 0x00005572658fd46e clang::Preprocessor::HandleMacroExpandedIdentifier(clang::Token&, clang::MacroDefinition const&) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x810546e)
#32 0x000055726591a822 clang::Preprocessor::HandleIdentifier(clang::Token&) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x8122822)
#33 0x000055726591cccc clang::Preprocessor::Lex(clang::Token&) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x8124ccc)
#34 0x0000557265928513 clang::MacroArgs::getPreExpArgument(unsigned int, clang::Preprocessor&) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x8130513)
#35 0x000055726592613d clang::TokenLexer::ExpandFunctionArguments() (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x812e13d)
#36 0x0000557265926f0f clang::TokenLexer::Init(clang::Token&, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x812ef0f)
#37 0x00005572658e5c55 clang::Preprocessor::EnterMacro(clang::Token&, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x80edc55)
#38 0x00005572658fd46e clang::Preprocessor::HandleMacroExpandedIdentifier(clang::Token&, clang::MacroDefinition const&) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x810546e)
#39 0x000055726591a822 clang::Preprocessor::HandleIdentifier(clang::Token&) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x8122822)
#40 0x00005572658787f2 clang::Lexer::LexIdentifierContinue(clang::Token&, char const*) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x80807f2)
#41 0x000055726587acce clang::Lexer::LexTokenInternal(clang::Token&, bool) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x8082cce)
#42 0x000055726587d193 clang::Lexer::Lex(clang::Token&) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x8085193)
#43 0x000055726591cccc clang::Preprocessor::Lex(clang::Token&) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x8124ccc)
#44 0x000055726405a4d0 clang::Parser::ConsumeToken() (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x68624d0)
#45 0x0000557264090610 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/data/xieym/llm/install/llvm-trunk/
#46 0x00005572640a63b4 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRan
#47 0x00005572640a9cb6 clang::Parser::ParseSimpleDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, bool, clang::Parser::ForRangeInit*, clang::Sourc
#48 0x00005572640aa1d3 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x68b21d3)
#49 0x00005572641592f6 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x69612f6)
#50 0x0000557264159b5a clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x6961b5a)
#51 0x000055726415aa02 clang::Parser::ParseCompoundStatementBody(bool) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x6962a02)
#52 0x000055726415dd1f clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x6965d1f)
#53 0x00005572640630a6 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x686b0a6)
#54 0x00005572640a6b7f clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x68aeb7f)
#55 0x000055726405e0ac clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x68660ac)
#56 0x000055726405e80e clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x686680e)
#57 0x0000557264067326 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x686f326)
#58 0x0000557264068365 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x6870365)
#59 0x0000557264068844 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x6870844)
#60 0x000055726405769b clang::ParseAST(clang::Sema&, bool, bool) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x685f69b)
#61 0x00005572629ff779 clang::FrontendAction::Execute() (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x5207779)
#62 0x000055726297f2f5 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x51872f5)
#63 0x0000557262ae5ed3 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x52eded3)
#64 0x000055725eaad637 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x12b5637)
#65 0x000055725eaa3c6a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#66 0x000055725eaa8274 clang_main(int, char**, llvm::ToolContext const&) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x12b0274)
#67 0x000055725e98892b main (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x119092b)
#68 0x00007f3e6b5e9d90 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:58:16
#69 0x00007f3e6b5e9e40 call_init ./csu/../csu/libc-start.c:128:20
#70 0x00007f3e6b5e9e40 __libc_start_main ./csu/../csu/libc-start.c:379:5
#71 0x000055725eaa3395 _start (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x12ab395)
clang: error: unable to execute command: Aborted (core dumped)
clang: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 21.0.0git (git@github.com:llvm/llvm-project.git c8ec8073aa5d8e87a15d101ded149de399518bc1)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /data/xieym/llm/install/llvm-trunk/bin
Build config: +assertions
clang: note: diagnostic msg:
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang: note: diagnostic msg: /data/xieym/temp/979-reduced-516ffa.c
clang: note: diagnostic msg: /data/xieym/temp/979-reduced-516ffa.sh
clang: note: diagnostic msg:

********************
```
version:
```shell
$ clang -v
clang version 21.0.0git (git@github.com:llvm/llvm-project.git c8ec8073aa5d8e87a15d101ded149de399518bc1)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /data/xieym/llm/install/llvm-trunk/bin
Build config: +assertions
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/11
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/12
Selected GCC installation: /usr/lib/gcc/x86_64-linux-gnu/12
Candidate multilib: .;@m64
Selected multilib: .;@m64
Found CUDA installation: /usr/local/cuda, version 12.3
```
godbolt link: https://godbolt.org/z/91z88cG69, gcc also ICE: https://gcc.gnu.org/bugzilla/show_bug.cgi?id=119184


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (wxie7)

<details>
code:
```c
#define max(X, Y) (((X) &gt;= (Y)) ? (X) : (Y))
void main(void) {
  float x = max(
      x,
      max(x,
          max(x,
              max(x,
                  max(x,
                      max(x,
                          max(x,
                              max(x,
                                  max(x,
                                      max(x,
                                          max(x,
                                              max(x,
                                                  max(x,
                                                      max(x,
                                                          max(x,
                                                              max(x,
                                                                  max(x,
                                                                      max(x,
                                                                          max(x,
                                                                              max(x,
                                                                                  max(x,
                                                                                      max(x,
                                                                                          max(x,
                                                                                              max(x,
                                                                                                  max(x,
                                                                                                      max(x,
                                                                                                          max(x,
                                                                                                              max(x,
                                                                                                                  x))))))))))))))))))))))))))));
```
command:
```shell
$ clang 979-reduced.c
979-reduced.c:2:1: warning: return type of 'main' is not 'int' [-Wmain-return-type]
    2 | void main(void) {
      | ^
979-reduced.c:2:1: note: change return type to 'int'
    2 | void main(void) {
      | ^~~~
      | int
clang-21: /data/xieym/llm/src/llvm-project/clang/include/clang/Basic/SourceManager.h:532: static clang::SrcMgr::SLocEntry clang::SrcMgr::SLocEntry::get(clang::SourceLocation::UIntTy, const clang::SrcMgr::Expansi
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /data/xieym/llm/install/llvm-trunk/bin/clang-21 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -main-file-name 979-reduced.c -mrelo
1.      979-reduced.c:3:13: current parser token 'max'
2.      979-reduced.c:2:17: parsing function body 'main'
3.      979-reduced.c:2:17: in compound statement ('{}')
 #<!-- -->0 0x0000557261d57020 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x455f020)
 #<!-- -->1 0x0000557261d5441f llvm::sys::RunSignalHandlers() (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x455c41f)
 #<!-- -->2 0x0000557261d5456a SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x00007f3e6b602520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007f3e6b6569fc __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->5 0x00007f3e6b6569fc __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->6 0x00007f3e6b6569fc pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->7 0x00007f3e6b602476 gsignal ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->8 0x00007f3e6b5e87f3 abort ./stdlib/abort.c:81:7
 #<!-- -->9 0x00007f3e6b5e871b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->10 0x00007f3e6b5f9e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
#<!-- -->11 0x0000557261fbcf7a clang::SourceManager::createExpansionLocImpl(clang::SrcMgr::ExpansionInfo const&amp;, unsigned int, int, unsigned int) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x47c4f7a)
#<!-- -->12 0x0000557261fbd759 clang::SourceManager::createMacroArgExpansionLoc(clang::SourceLocation, clang::SourceLocation, unsigned int) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x47c5759)
#<!-- -->13 0x0000557265922667 clang::TokenLexer::updateLocForMacroArgTokens(clang::SourceLocation, clang::Token*, clang::Token*) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x812a667)
#<!-- -->14 0x00005572659268ee clang::TokenLexer::ExpandFunctionArguments() (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x812e8ee)
#<!-- -->15 0x0000557265926f0f clang::TokenLexer::Init(clang::Token&amp;, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x812ef0f)
#<!-- -->16 0x00005572658e5c55 clang::Preprocessor::EnterMacro(clang::Token&amp;, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x80edc55)
#<!-- -->17 0x00005572658fd46e clang::Preprocessor::HandleMacroExpandedIdentifier(clang::Token&amp;, clang::MacroDefinition const&amp;) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x810546e)
#<!-- -->18 0x000055726591a822 clang::Preprocessor::HandleIdentifier(clang::Token&amp;) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x8122822)
#<!-- -->19 0x000055726591cccc clang::Preprocessor::Lex(clang::Token&amp;) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x8124ccc)
#<!-- -->20 0x0000557265928513 clang::MacroArgs::getPreExpArgument(unsigned int, clang::Preprocessor&amp;) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x8130513)
#<!-- -->21 0x000055726592613d clang::TokenLexer::ExpandFunctionArguments() (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x812e13d)
#<!-- -->22 0x0000557265926f0f clang::TokenLexer::Init(clang::Token&amp;, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x812ef0f)
#<!-- -->23 0x00005572658e5c55 clang::Preprocessor::EnterMacro(clang::Token&amp;, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x80edc55)
#<!-- -->24 0x00005572658fd46e clang::Preprocessor::HandleMacroExpandedIdentifier(clang::Token&amp;, clang::MacroDefinition const&amp;) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x810546e)
#<!-- -->25 0x000055726591a822 clang::Preprocessor::HandleIdentifier(clang::Token&amp;) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x8122822)
#<!-- -->26 0x000055726591cccc clang::Preprocessor::Lex(clang::Token&amp;) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x8124ccc)
#<!-- -->27 0x0000557265928513 clang::MacroArgs::getPreExpArgument(unsigned int, clang::Preprocessor&amp;) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x8130513)
#<!-- -->28 0x000055726592613d clang::TokenLexer::ExpandFunctionArguments() (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x812e13d)
#<!-- -->29 0x0000557265926f0f clang::TokenLexer::Init(clang::Token&amp;, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x812ef0f)
#<!-- -->30 0x00005572658e5c55 clang::Preprocessor::EnterMacro(clang::Token&amp;, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x80edc55)
#<!-- -->31 0x00005572658fd46e clang::Preprocessor::HandleMacroExpandedIdentifier(clang::Token&amp;, clang::MacroDefinition const&amp;) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x810546e)
#<!-- -->32 0x000055726591a822 clang::Preprocessor::HandleIdentifier(clang::Token&amp;) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x8122822)
#<!-- -->33 0x000055726591cccc clang::Preprocessor::Lex(clang::Token&amp;) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x8124ccc)
#<!-- -->34 0x0000557265928513 clang::MacroArgs::getPreExpArgument(unsigned int, clang::Preprocessor&amp;) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x8130513)
#<!-- -->35 0x000055726592613d clang::TokenLexer::ExpandFunctionArguments() (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x812e13d)
#<!-- -->36 0x0000557265926f0f clang::TokenLexer::Init(clang::Token&amp;, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x812ef0f)
#<!-- -->37 0x00005572658e5c55 clang::Preprocessor::EnterMacro(clang::Token&amp;, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x80edc55)
#<!-- -->38 0x00005572658fd46e clang::Preprocessor::HandleMacroExpandedIdentifier(clang::Token&amp;, clang::MacroDefinition const&amp;) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x810546e)
#<!-- -->39 0x000055726591a822 clang::Preprocessor::HandleIdentifier(clang::Token&amp;) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x8122822)
#<!-- -->40 0x00005572658787f2 clang::Lexer::LexIdentifierContinue(clang::Token&amp;, char const*) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x80807f2)
#<!-- -->41 0x000055726587acce clang::Lexer::LexTokenInternal(clang::Token&amp;, bool) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x8082cce)
#<!-- -->42 0x000055726587d193 clang::Lexer::Lex(clang::Token&amp;) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x8085193)
#<!-- -->43 0x000055726591cccc clang::Preprocessor::Lex(clang::Token&amp;) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x8124ccc)
#<!-- -->44 0x000055726405a4d0 clang::Parser::ConsumeToken() (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x68624d0)
#<!-- -->45 0x0000557264090610 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) (/data/xieym/llm/install/llvm-trunk/
#<!-- -->46 0x00005572640a63b4 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRan
#<!-- -->47 0x00005572640a9cb6 clang::Parser::ParseSimpleDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, bool, clang::Parser::ForRangeInit*, clang::Sourc
#<!-- -->48 0x00005572640aa1d3 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x68b21d3)
#<!-- -->49 0x00005572641592f6 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x69612f6)
#<!-- -->50 0x0000557264159b5a clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x6961b5a)
#<!-- -->51 0x000055726415aa02 clang::Parser::ParseCompoundStatementBody(bool) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x6962a02)
#<!-- -->52 0x000055726415dd1f clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x6965d1f)
#<!-- -->53 0x00005572640630a6 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x686b0a6)
#<!-- -->54 0x00005572640a6b7f clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x68aeb7f)
#<!-- -->55 0x000055726405e0ac clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x68660ac)
#<!-- -->56 0x000055726405e80e clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x686680e)
#<!-- -->57 0x0000557264067326 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x686f326)
#<!-- -->58 0x0000557264068365 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x6870365)
#<!-- -->59 0x0000557264068844 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x6870844)
#<!-- -->60 0x000055726405769b clang::ParseAST(clang::Sema&amp;, bool, bool) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x685f69b)
#<!-- -->61 0x00005572629ff779 clang::FrontendAction::Execute() (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x5207779)
#<!-- -->62 0x000055726297f2f5 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x51872f5)
#<!-- -->63 0x0000557262ae5ed3 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x52eded3)
#<!-- -->64 0x000055725eaad637 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x12b5637)
#<!-- -->65 0x000055725eaa3c6a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->66 0x000055725eaa8274 clang_main(int, char**, llvm::ToolContext const&amp;) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x12b0274)
#<!-- -->67 0x000055725e98892b main (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x119092b)
#<!-- -->68 0x00007f3e6b5e9d90 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:58:16
#<!-- -->69 0x00007f3e6b5e9e40 call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->70 0x00007f3e6b5e9e40 __libc_start_main ./csu/../csu/libc-start.c:379:5
#<!-- -->71 0x000055725eaa3395 _start (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x12ab395)
clang: error: unable to execute command: Aborted (core dumped)
clang: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 21.0.0git (git@<!-- -->github.com:llvm/llvm-project.git c8ec8073aa5d8e87a15d101ded149de399518bc1)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /data/xieym/llm/install/llvm-trunk/bin
Build config: +assertions
clang: note: diagnostic msg:
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang: note: diagnostic msg: /data/xieym/temp/979-reduced-516ffa.c
clang: note: diagnostic msg: /data/xieym/temp/979-reduced-516ffa.sh
clang: note: diagnostic msg:

********************
```
version:
```shell
$ clang -v
clang version 21.0.0git (git@<!-- -->github.com:llvm/llvm-project.git c8ec8073aa5d8e87a15d101ded149de399518bc1)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /data/xieym/llm/install/llvm-trunk/bin
Build config: +assertions
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/11
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/12
Selected GCC installation: /usr/lib/gcc/x86_64-linux-gnu/12
Candidate multilib: .;@<!-- -->m64
Selected multilib: .;@<!-- -->m64
Found CUDA installation: /usr/local/cuda, version 12.3
```
godbolt link: https://godbolt.org/z/91z88cG69, gcc also ICE: https://gcc.gnu.org/bugzilla/show_bug.cgi?id=119184

</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-bug

Author: None (wxie7)

<details>
code:
```c
#define max(X, Y) (((X) &gt;= (Y)) ? (X) : (Y))
void main(void) {
  float x = max(
      x,
      max(x,
          max(x,
              max(x,
                  max(x,
                      max(x,
                          max(x,
                              max(x,
                                  max(x,
                                      max(x,
                                          max(x,
                                              max(x,
                                                  max(x,
                                                      max(x,
                                                          max(x,
                                                              max(x,
                                                                  max(x,
                                                                      max(x,
                                                                          max(x,
                                                                              max(x,
                                                                                  max(x,
                                                                                      max(x,
                                                                                          max(x,
                                                                                              max(x,
                                                                                                  max(x,
                                                                                                      max(x,
                                                                                                          max(x,
                                                                                                              max(x,
                                                                                                                  x))))))))))))))))))))))))))));
```
command:
```shell
$ clang 979-reduced.c
979-reduced.c:2:1: warning: return type of 'main' is not 'int' [-Wmain-return-type]
    2 | void main(void) {
      | ^
979-reduced.c:2:1: note: change return type to 'int'
    2 | void main(void) {
      | ^~~~
      | int
clang-21: /data/xieym/llm/src/llvm-project/clang/include/clang/Basic/SourceManager.h:532: static clang::SrcMgr::SLocEntry clang::SrcMgr::SLocEntry::get(clang::SourceLocation::UIntTy, const clang::SrcMgr::Expansi
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /data/xieym/llm/install/llvm-trunk/bin/clang-21 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -main-file-name 979-reduced.c -mrelo
1.      979-reduced.c:3:13: current parser token 'max'
2.      979-reduced.c:2:17: parsing function body 'main'
3.      979-reduced.c:2:17: in compound statement ('{}')
 #<!-- -->0 0x0000557261d57020 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x455f020)
 #<!-- -->1 0x0000557261d5441f llvm::sys::RunSignalHandlers() (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x455c41f)
 #<!-- -->2 0x0000557261d5456a SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x00007f3e6b602520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007f3e6b6569fc __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->5 0x00007f3e6b6569fc __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->6 0x00007f3e6b6569fc pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->7 0x00007f3e6b602476 gsignal ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->8 0x00007f3e6b5e87f3 abort ./stdlib/abort.c:81:7
 #<!-- -->9 0x00007f3e6b5e871b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->10 0x00007f3e6b5f9e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
#<!-- -->11 0x0000557261fbcf7a clang::SourceManager::createExpansionLocImpl(clang::SrcMgr::ExpansionInfo const&amp;, unsigned int, int, unsigned int) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x47c4f7a)
#<!-- -->12 0x0000557261fbd759 clang::SourceManager::createMacroArgExpansionLoc(clang::SourceLocation, clang::SourceLocation, unsigned int) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x47c5759)
#<!-- -->13 0x0000557265922667 clang::TokenLexer::updateLocForMacroArgTokens(clang::SourceLocation, clang::Token*, clang::Token*) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x812a667)
#<!-- -->14 0x00005572659268ee clang::TokenLexer::ExpandFunctionArguments() (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x812e8ee)
#<!-- -->15 0x0000557265926f0f clang::TokenLexer::Init(clang::Token&amp;, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x812ef0f)
#<!-- -->16 0x00005572658e5c55 clang::Preprocessor::EnterMacro(clang::Token&amp;, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x80edc55)
#<!-- -->17 0x00005572658fd46e clang::Preprocessor::HandleMacroExpandedIdentifier(clang::Token&amp;, clang::MacroDefinition const&amp;) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x810546e)
#<!-- -->18 0x000055726591a822 clang::Preprocessor::HandleIdentifier(clang::Token&amp;) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x8122822)
#<!-- -->19 0x000055726591cccc clang::Preprocessor::Lex(clang::Token&amp;) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x8124ccc)
#<!-- -->20 0x0000557265928513 clang::MacroArgs::getPreExpArgument(unsigned int, clang::Preprocessor&amp;) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x8130513)
#<!-- -->21 0x000055726592613d clang::TokenLexer::ExpandFunctionArguments() (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x812e13d)
#<!-- -->22 0x0000557265926f0f clang::TokenLexer::Init(clang::Token&amp;, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x812ef0f)
#<!-- -->23 0x00005572658e5c55 clang::Preprocessor::EnterMacro(clang::Token&amp;, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x80edc55)
#<!-- -->24 0x00005572658fd46e clang::Preprocessor::HandleMacroExpandedIdentifier(clang::Token&amp;, clang::MacroDefinition const&amp;) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x810546e)
#<!-- -->25 0x000055726591a822 clang::Preprocessor::HandleIdentifier(clang::Token&amp;) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x8122822)
#<!-- -->26 0x000055726591cccc clang::Preprocessor::Lex(clang::Token&amp;) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x8124ccc)
#<!-- -->27 0x0000557265928513 clang::MacroArgs::getPreExpArgument(unsigned int, clang::Preprocessor&amp;) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x8130513)
#<!-- -->28 0x000055726592613d clang::TokenLexer::ExpandFunctionArguments() (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x812e13d)
#<!-- -->29 0x0000557265926f0f clang::TokenLexer::Init(clang::Token&amp;, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x812ef0f)
#<!-- -->30 0x00005572658e5c55 clang::Preprocessor::EnterMacro(clang::Token&amp;, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x80edc55)
#<!-- -->31 0x00005572658fd46e clang::Preprocessor::HandleMacroExpandedIdentifier(clang::Token&amp;, clang::MacroDefinition const&amp;) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x810546e)
#<!-- -->32 0x000055726591a822 clang::Preprocessor::HandleIdentifier(clang::Token&amp;) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x8122822)
#<!-- -->33 0x000055726591cccc clang::Preprocessor::Lex(clang::Token&amp;) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x8124ccc)
#<!-- -->34 0x0000557265928513 clang::MacroArgs::getPreExpArgument(unsigned int, clang::Preprocessor&amp;) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x8130513)
#<!-- -->35 0x000055726592613d clang::TokenLexer::ExpandFunctionArguments() (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x812e13d)
#<!-- -->36 0x0000557265926f0f clang::TokenLexer::Init(clang::Token&amp;, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x812ef0f)
#<!-- -->37 0x00005572658e5c55 clang::Preprocessor::EnterMacro(clang::Token&amp;, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x80edc55)
#<!-- -->38 0x00005572658fd46e clang::Preprocessor::HandleMacroExpandedIdentifier(clang::Token&amp;, clang::MacroDefinition const&amp;) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x810546e)
#<!-- -->39 0x000055726591a822 clang::Preprocessor::HandleIdentifier(clang::Token&amp;) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x8122822)
#<!-- -->40 0x00005572658787f2 clang::Lexer::LexIdentifierContinue(clang::Token&amp;, char const*) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x80807f2)
#<!-- -->41 0x000055726587acce clang::Lexer::LexTokenInternal(clang::Token&amp;, bool) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x8082cce)
#<!-- -->42 0x000055726587d193 clang::Lexer::Lex(clang::Token&amp;) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x8085193)
#<!-- -->43 0x000055726591cccc clang::Preprocessor::Lex(clang::Token&amp;) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x8124ccc)
#<!-- -->44 0x000055726405a4d0 clang::Parser::ConsumeToken() (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x68624d0)
#<!-- -->45 0x0000557264090610 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) (/data/xieym/llm/install/llvm-trunk/
#<!-- -->46 0x00005572640a63b4 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRan
#<!-- -->47 0x00005572640a9cb6 clang::Parser::ParseSimpleDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, bool, clang::Parser::ForRangeInit*, clang::Sourc
#<!-- -->48 0x00005572640aa1d3 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x68b21d3)
#<!-- -->49 0x00005572641592f6 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x69612f6)
#<!-- -->50 0x0000557264159b5a clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x6961b5a)
#<!-- -->51 0x000055726415aa02 clang::Parser::ParseCompoundStatementBody(bool) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x6962a02)
#<!-- -->52 0x000055726415dd1f clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x6965d1f)
#<!-- -->53 0x00005572640630a6 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x686b0a6)
#<!-- -->54 0x00005572640a6b7f clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x68aeb7f)
#<!-- -->55 0x000055726405e0ac clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x68660ac)
#<!-- -->56 0x000055726405e80e clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x686680e)
#<!-- -->57 0x0000557264067326 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x686f326)
#<!-- -->58 0x0000557264068365 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x6870365)
#<!-- -->59 0x0000557264068844 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x6870844)
#<!-- -->60 0x000055726405769b clang::ParseAST(clang::Sema&amp;, bool, bool) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x685f69b)
#<!-- -->61 0x00005572629ff779 clang::FrontendAction::Execute() (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x5207779)
#<!-- -->62 0x000055726297f2f5 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x51872f5)
#<!-- -->63 0x0000557262ae5ed3 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x52eded3)
#<!-- -->64 0x000055725eaad637 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x12b5637)
#<!-- -->65 0x000055725eaa3c6a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->66 0x000055725eaa8274 clang_main(int, char**, llvm::ToolContext const&amp;) (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x12b0274)
#<!-- -->67 0x000055725e98892b main (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x119092b)
#<!-- -->68 0x00007f3e6b5e9d90 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:58:16
#<!-- -->69 0x00007f3e6b5e9e40 call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->70 0x00007f3e6b5e9e40 __libc_start_main ./csu/../csu/libc-start.c:379:5
#<!-- -->71 0x000055725eaa3395 _start (/data/xieym/llm/install/llvm-trunk/bin/clang-21+0x12ab395)
clang: error: unable to execute command: Aborted (core dumped)
clang: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 21.0.0git (git@<!-- -->github.com:llvm/llvm-project.git c8ec8073aa5d8e87a15d101ded149de399518bc1)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /data/xieym/llm/install/llvm-trunk/bin
Build config: +assertions
clang: note: diagnostic msg:
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang: note: diagnostic msg: /data/xieym/temp/979-reduced-516ffa.c
clang: note: diagnostic msg: /data/xieym/temp/979-reduced-516ffa.sh
clang: note: diagnostic msg:

********************
```
version:
```shell
$ clang -v
clang version 21.0.0git (git@<!-- -->github.com:llvm/llvm-project.git c8ec8073aa5d8e87a15d101ded149de399518bc1)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /data/xieym/llm/install/llvm-trunk/bin
Build config: +assertions
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/11
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/12
Selected GCC installation: /usr/lib/gcc/x86_64-linux-gnu/12
Candidate multilib: .;@<!-- -->m64
Selected multilib: .;@<!-- -->m64
Found CUDA installation: /usr/local/cuda, version 12.3
```
godbolt link: https://godbolt.org/z/91z88cG69, gcc also ICE: https://gcc.gnu.org/bugzilla/show_bug.cgi?id=119184

</details>


---

### Comment 3 - frederick-vs-ja

I _think_ this bug is still triggered when using `int` as the return type (so this ought to be crash-on-valid), while the `void` return type looks like red herring.

---

### Comment 4 - ym-xie

> I _think_ this bug is still triggered when using `int` as the return type (so this ought to be crash-on-valid), while the `void` return type looks like red herring.

After changing void to int, the detailed call stack is not displayed: 
```shell
fatal error: translation unit is too large for Clang to process: ran out of source locations
clang: error: clang frontend command failed with exit code 70 (use -v to see invocation)
clang version 21.0.0git (git@github.com:llvm/llvm-project.git c8ec8073aa5d8e87a15d101ded149de399518bc1)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /data/xieym/llm/install/llvm-trunk/bin
Build config: +assertions
clang: note: diagnostic msg:
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang: note: diagnostic msg: /data/xieym/temp/979-reduced-615735.c
clang: note: diagnostic msg: /data/xieym/temp/979-reduced-615735.sh
clang: note: diagnostic msg:

********************
```

---

