# Lexical analysis fails on malformed _Pragma directive after #include

**Author:** mariete1223
**URL:** https://github.com/llvm/llvm-project/issues/138094
**Status:** Closed
**Labels:** c, clang:frontend, crash-on-invalid, confirmed
**Closed Date:** 2025-05-05T11:29:43Z

## Body

## Summary
The crash occurs when an invalid _Pragma directive appears on the same line as a #include.

## Assertion 
No assertion given in Assertion build clang

## Stack dump

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /root/llvm-project/build-assert/bin/clang-21 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -main-file-name prueba.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/root/llvm-project/build-assert -fcoverage-compilation-dir=/root/llvm-project/build-assert -resource-dir /root/llvm-project/build-assert/lib/clang/21 -internal-isystem /root/llvm-project/build-assert/lib/clang/21/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/9/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -std=c2x -ferror-limit 19 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/prueba-ce8fe6.o -x c prueba.cpp
1.      prueba.cpp:1:2: current parser token 'include'
 #0 0x00005557aed8327e llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /root/llvm-project/llvm/lib/Support/Unix/Signals.inc:804:22
 #1 0x00005557aed83694 PrintStackTraceSignalHandler(void*) /root/llvm-project/llvm/lib/Support/Unix/Signals.inc:880:1
 #2 0x00005557aed80bf2 llvm::sys::RunSignalHandlers() /root/llvm-project/llvm/lib/Support/Signals.cpp:105:20
 #3 0x00005557aed82b0a SignalHandler(int, siginfo_t*, void*) /root/llvm-project/llvm/lib/Support/Unix/Signals.inc:418:13
 #4 0x00007fe35c9d8420 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x14420)
 #5 0x00005557b417817c llvm::ArrayRef<clang::dependency_directives_scan::Directive>::empty() const /root/llvm-project/llvm/include/llvm/ADT/ArrayRef.h:163:33
 #6 0x00005557b4177b32 clang::Lexer::isDependencyDirectivesLexer() const /root/llvm-project/clang/include/clang/Lex/Lexer.h:215:74
 #7 0x00005557b41737c6 clang::Lexer::Lex(clang::Token&) /root/llvm-project/clang/lib/Lex/Lexer.cpp:3703:3
 #8 0x00005557b2121b9f clang::Preprocessor::CLK_Lexer(clang::Preprocessor&, clang::Token&) /root/llvm-project/clang/include/clang/Lex/Preprocessor.h:3029:3
 #9 0x00005557b4211dde clang::Preprocessor::Lex(clang::Token&) /root/llvm-project/clang/lib/Lex/Preprocessor.cpp:870:10
#10 0x00005557b009b857 clang::Preprocessor::LexUnexpandedToken(clang::Token&) /root/llvm-project/clang/include/clang/Lex/Preprocessor.h:1803:27
#11 0x00005557b41bb13e clang::Preprocessor::DiscardUntilEndOfDirective(clang::Token&) /root/llvm-project/clang/lib/Lex/PPDirectives.cpp:89:15
#12 0x00005557b2235466 clang::Preprocessor::DiscardUntilEndOfDirective() /root/llvm-project/clang/include/clang/Lex/Preprocessor.h:2345:3
#13 0x00005557b41bdd48 clang::Preprocessor::CheckEndOfDirective(char const*, bool) /root/llvm-project/clang/lib/Lex/PPDirectives.cpp:466:37
#14 0x00005557b41c49ab clang::Preprocessor::HandleIncludeDirective(clang::SourceLocation, clang::Token&, clang::detail::SearchDirIteratorImpl<true>, clang::FileEntry const*) /root/llvm-project/clang/lib/Lex/PPDirectives.cpp:2087:79
#15 0x00005557b41c1b01 clang::Preprocessor::HandleDirective(clang::Token&) /root/llvm-project/clang/lib/Lex/PPDirectives.cpp:1334:36
#16 0x00005557b41765ba clang::Lexer::LexTokenInternal(clang::Token&, bool) /root/llvm-project/clang/lib/Lex/Lexer.cpp:4513:7
#17 0x00005557b41738b8 clang::Lexer::Lex(clang::Token&) /root/llvm-project/clang/lib/Lex/Lexer.cpp:3728:40
#18 0x00005557b2121b9f clang::Preprocessor::CLK_Lexer(clang::Preprocessor&, clang::Token&) /root/llvm-project/clang/include/clang/Lex/Preprocessor.h:3029:3
#19 0x00005557b4211dde clang::Preprocessor::Lex(clang::Token&) /root/llvm-project/clang/lib/Lex/Preprocessor.cpp:870:10
#20 0x00005557b212469a clang::Parser::ConsumeToken() /root/llvm-project/clang/include/clang/Parse/Parser.h:553:12
#21 0x00005557b2117cdb clang::Parser::Initialize() /root/llvm-project/clang/lib/Parse/Parser.cpp:583:1
#22 0x00005557b2113630 clang::ParseAST(clang::Sema&, bool, bool) /root/llvm-project/clang/lib/Parse/ParseAST.cpp:165:28
#23 0x00005557b0042366 clang::ASTFrontendAction::ExecuteAction() /root/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1186:11
#24 0x00005557afcff958 clang::CodeGenAction::ExecuteAction() /root/llvm-project/clang/lib/CodeGen/CodeGenAction.cpp:1102:5
#25 0x00005557b0041c21 clang::FrontendAction::Execute() /root/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1076:38
#26 0x00005557aff52e8b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) /root/llvm-project/clang/lib/Frontend/CompilerInstance.cpp:1056:42
#27 0x00005557b020d57d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) /root/llvm-project/clang/lib/FrontendTool/ExecuteCompilerInvocation.cpp:265:38
#28 0x00005557aa2c18c4 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) /root/llvm-project/clang/tools/driver/cc1_main.cpp:290:40
#29 0x00005557aa2b240b ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) /root/llvm-project/clang/tools/driver/driver.cpp:218:20
#30 0x00005557aa2b2950 clang_main(int, char**, llvm::ToolContext const&) /root/llvm-project/clang/tools/driver/driver.cpp:259:26
#31 0x00005557aa2ee950 main /root/llvm-project/build-assert/tools/clang/tools/driver/clang-driver.cpp:17:20
#32 0x00007fe35c486083 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x24083)
#33 0x00005557aa2b176e _start (/root/llvm-project/build-assert/bin/clang-21+0x38e676e)
```

## Program

```
#include <fcntl.h> _Pragma(
```

## To quickly reproduce

It happens since clang 3.1.0 until current version

https://gcc.godbolt.org/z/T7Kn6nGnd


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (mariete1223)

<details>
## Summary
The crash occurs when an invalid _Pragma directive appears on the same line as a #include.

## Assertion 
No assertion given in Assertion build clang

## Stack dump

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /root/llvm-project/build-assert/bin/clang-21 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -main-file-name prueba.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/root/llvm-project/build-assert -fcoverage-compilation-dir=/root/llvm-project/build-assert -resource-dir /root/llvm-project/build-assert/lib/clang/21 -internal-isystem /root/llvm-project/build-assert/lib/clang/21/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/9/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -std=c2x -ferror-limit 19 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/prueba-ce8fe6.o -x c prueba.cpp
1.      prueba.cpp:1:2: current parser token 'include'
 #<!-- -->0 0x00005557aed8327e llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /root/llvm-project/llvm/lib/Support/Unix/Signals.inc:804:22
 #<!-- -->1 0x00005557aed83694 PrintStackTraceSignalHandler(void*) /root/llvm-project/llvm/lib/Support/Unix/Signals.inc:880:1
 #<!-- -->2 0x00005557aed80bf2 llvm::sys::RunSignalHandlers() /root/llvm-project/llvm/lib/Support/Signals.cpp:105:20
 #<!-- -->3 0x00005557aed82b0a SignalHandler(int, siginfo_t*, void*) /root/llvm-project/llvm/lib/Support/Unix/Signals.inc:418:13
 #<!-- -->4 0x00007fe35c9d8420 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x14420)
 #<!-- -->5 0x00005557b417817c llvm::ArrayRef&lt;clang::dependency_directives_scan::Directive&gt;::empty() const /root/llvm-project/llvm/include/llvm/ADT/ArrayRef.h:163:33
 #<!-- -->6 0x00005557b4177b32 clang::Lexer::isDependencyDirectivesLexer() const /root/llvm-project/clang/include/clang/Lex/Lexer.h:215:74
 #<!-- -->7 0x00005557b41737c6 clang::Lexer::Lex(clang::Token&amp;) /root/llvm-project/clang/lib/Lex/Lexer.cpp:3703:3
 #<!-- -->8 0x00005557b2121b9f clang::Preprocessor::CLK_Lexer(clang::Preprocessor&amp;, clang::Token&amp;) /root/llvm-project/clang/include/clang/Lex/Preprocessor.h:3029:3
 #<!-- -->9 0x00005557b4211dde clang::Preprocessor::Lex(clang::Token&amp;) /root/llvm-project/clang/lib/Lex/Preprocessor.cpp:870:10
#<!-- -->10 0x00005557b009b857 clang::Preprocessor::LexUnexpandedToken(clang::Token&amp;) /root/llvm-project/clang/include/clang/Lex/Preprocessor.h:1803:27
#<!-- -->11 0x00005557b41bb13e clang::Preprocessor::DiscardUntilEndOfDirective(clang::Token&amp;) /root/llvm-project/clang/lib/Lex/PPDirectives.cpp:89:15
#<!-- -->12 0x00005557b2235466 clang::Preprocessor::DiscardUntilEndOfDirective() /root/llvm-project/clang/include/clang/Lex/Preprocessor.h:2345:3
#<!-- -->13 0x00005557b41bdd48 clang::Preprocessor::CheckEndOfDirective(char const*, bool) /root/llvm-project/clang/lib/Lex/PPDirectives.cpp:466:37
#<!-- -->14 0x00005557b41c49ab clang::Preprocessor::HandleIncludeDirective(clang::SourceLocation, clang::Token&amp;, clang::detail::SearchDirIteratorImpl&lt;true&gt;, clang::FileEntry const*) /root/llvm-project/clang/lib/Lex/PPDirectives.cpp:2087:79
#<!-- -->15 0x00005557b41c1b01 clang::Preprocessor::HandleDirective(clang::Token&amp;) /root/llvm-project/clang/lib/Lex/PPDirectives.cpp:1334:36
#<!-- -->16 0x00005557b41765ba clang::Lexer::LexTokenInternal(clang::Token&amp;, bool) /root/llvm-project/clang/lib/Lex/Lexer.cpp:4513:7
#<!-- -->17 0x00005557b41738b8 clang::Lexer::Lex(clang::Token&amp;) /root/llvm-project/clang/lib/Lex/Lexer.cpp:3728:40
#<!-- -->18 0x00005557b2121b9f clang::Preprocessor::CLK_Lexer(clang::Preprocessor&amp;, clang::Token&amp;) /root/llvm-project/clang/include/clang/Lex/Preprocessor.h:3029:3
#<!-- -->19 0x00005557b4211dde clang::Preprocessor::Lex(clang::Token&amp;) /root/llvm-project/clang/lib/Lex/Preprocessor.cpp:870:10
#<!-- -->20 0x00005557b212469a clang::Parser::ConsumeToken() /root/llvm-project/clang/include/clang/Parse/Parser.h:553:12
#<!-- -->21 0x00005557b2117cdb clang::Parser::Initialize() /root/llvm-project/clang/lib/Parse/Parser.cpp:583:1
#<!-- -->22 0x00005557b2113630 clang::ParseAST(clang::Sema&amp;, bool, bool) /root/llvm-project/clang/lib/Parse/ParseAST.cpp:165:28
#<!-- -->23 0x00005557b0042366 clang::ASTFrontendAction::ExecuteAction() /root/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1186:11
#<!-- -->24 0x00005557afcff958 clang::CodeGenAction::ExecuteAction() /root/llvm-project/clang/lib/CodeGen/CodeGenAction.cpp:1102:5
#<!-- -->25 0x00005557b0041c21 clang::FrontendAction::Execute() /root/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1076:38
#<!-- -->26 0x00005557aff52e8b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) /root/llvm-project/clang/lib/Frontend/CompilerInstance.cpp:1056:42
#<!-- -->27 0x00005557b020d57d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) /root/llvm-project/clang/lib/FrontendTool/ExecuteCompilerInvocation.cpp:265:38
#<!-- -->28 0x00005557aa2c18c4 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) /root/llvm-project/clang/tools/driver/cc1_main.cpp:290:40
#<!-- -->29 0x00005557aa2b240b ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) /root/llvm-project/clang/tools/driver/driver.cpp:218:20
#<!-- -->30 0x00005557aa2b2950 clang_main(int, char**, llvm::ToolContext const&amp;) /root/llvm-project/clang/tools/driver/driver.cpp:259:26
#<!-- -->31 0x00005557aa2ee950 main /root/llvm-project/build-assert/tools/clang/tools/driver/clang-driver.cpp:17:20
#<!-- -->32 0x00007fe35c486083 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x24083)
#<!-- -->33 0x00005557aa2b176e _start (/root/llvm-project/build-assert/bin/clang-21+0x38e676e)
```

## Program

```
#include &lt;fcntl.h&gt; _Pragma(
```

## To quickly reproduce

It happens since clang 3.1.0 until current version

https://gcc.godbolt.org/z/T7Kn6nGnd

</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-c

Author: None (mariete1223)

<details>
## Summary
The crash occurs when an invalid _Pragma directive appears on the same line as a #include.

## Assertion 
No assertion given in Assertion build clang

## Stack dump

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /root/llvm-project/build-assert/bin/clang-21 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -main-file-name prueba.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/root/llvm-project/build-assert -fcoverage-compilation-dir=/root/llvm-project/build-assert -resource-dir /root/llvm-project/build-assert/lib/clang/21 -internal-isystem /root/llvm-project/build-assert/lib/clang/21/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/9/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -std=c2x -ferror-limit 19 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/prueba-ce8fe6.o -x c prueba.cpp
1.      prueba.cpp:1:2: current parser token 'include'
 #<!-- -->0 0x00005557aed8327e llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /root/llvm-project/llvm/lib/Support/Unix/Signals.inc:804:22
 #<!-- -->1 0x00005557aed83694 PrintStackTraceSignalHandler(void*) /root/llvm-project/llvm/lib/Support/Unix/Signals.inc:880:1
 #<!-- -->2 0x00005557aed80bf2 llvm::sys::RunSignalHandlers() /root/llvm-project/llvm/lib/Support/Signals.cpp:105:20
 #<!-- -->3 0x00005557aed82b0a SignalHandler(int, siginfo_t*, void*) /root/llvm-project/llvm/lib/Support/Unix/Signals.inc:418:13
 #<!-- -->4 0x00007fe35c9d8420 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x14420)
 #<!-- -->5 0x00005557b417817c llvm::ArrayRef&lt;clang::dependency_directives_scan::Directive&gt;::empty() const /root/llvm-project/llvm/include/llvm/ADT/ArrayRef.h:163:33
 #<!-- -->6 0x00005557b4177b32 clang::Lexer::isDependencyDirectivesLexer() const /root/llvm-project/clang/include/clang/Lex/Lexer.h:215:74
 #<!-- -->7 0x00005557b41737c6 clang::Lexer::Lex(clang::Token&amp;) /root/llvm-project/clang/lib/Lex/Lexer.cpp:3703:3
 #<!-- -->8 0x00005557b2121b9f clang::Preprocessor::CLK_Lexer(clang::Preprocessor&amp;, clang::Token&amp;) /root/llvm-project/clang/include/clang/Lex/Preprocessor.h:3029:3
 #<!-- -->9 0x00005557b4211dde clang::Preprocessor::Lex(clang::Token&amp;) /root/llvm-project/clang/lib/Lex/Preprocessor.cpp:870:10
#<!-- -->10 0x00005557b009b857 clang::Preprocessor::LexUnexpandedToken(clang::Token&amp;) /root/llvm-project/clang/include/clang/Lex/Preprocessor.h:1803:27
#<!-- -->11 0x00005557b41bb13e clang::Preprocessor::DiscardUntilEndOfDirective(clang::Token&amp;) /root/llvm-project/clang/lib/Lex/PPDirectives.cpp:89:15
#<!-- -->12 0x00005557b2235466 clang::Preprocessor::DiscardUntilEndOfDirective() /root/llvm-project/clang/include/clang/Lex/Preprocessor.h:2345:3
#<!-- -->13 0x00005557b41bdd48 clang::Preprocessor::CheckEndOfDirective(char const*, bool) /root/llvm-project/clang/lib/Lex/PPDirectives.cpp:466:37
#<!-- -->14 0x00005557b41c49ab clang::Preprocessor::HandleIncludeDirective(clang::SourceLocation, clang::Token&amp;, clang::detail::SearchDirIteratorImpl&lt;true&gt;, clang::FileEntry const*) /root/llvm-project/clang/lib/Lex/PPDirectives.cpp:2087:79
#<!-- -->15 0x00005557b41c1b01 clang::Preprocessor::HandleDirective(clang::Token&amp;) /root/llvm-project/clang/lib/Lex/PPDirectives.cpp:1334:36
#<!-- -->16 0x00005557b41765ba clang::Lexer::LexTokenInternal(clang::Token&amp;, bool) /root/llvm-project/clang/lib/Lex/Lexer.cpp:4513:7
#<!-- -->17 0x00005557b41738b8 clang::Lexer::Lex(clang::Token&amp;) /root/llvm-project/clang/lib/Lex/Lexer.cpp:3728:40
#<!-- -->18 0x00005557b2121b9f clang::Preprocessor::CLK_Lexer(clang::Preprocessor&amp;, clang::Token&amp;) /root/llvm-project/clang/include/clang/Lex/Preprocessor.h:3029:3
#<!-- -->19 0x00005557b4211dde clang::Preprocessor::Lex(clang::Token&amp;) /root/llvm-project/clang/lib/Lex/Preprocessor.cpp:870:10
#<!-- -->20 0x00005557b212469a clang::Parser::ConsumeToken() /root/llvm-project/clang/include/clang/Parse/Parser.h:553:12
#<!-- -->21 0x00005557b2117cdb clang::Parser::Initialize() /root/llvm-project/clang/lib/Parse/Parser.cpp:583:1
#<!-- -->22 0x00005557b2113630 clang::ParseAST(clang::Sema&amp;, bool, bool) /root/llvm-project/clang/lib/Parse/ParseAST.cpp:165:28
#<!-- -->23 0x00005557b0042366 clang::ASTFrontendAction::ExecuteAction() /root/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1186:11
#<!-- -->24 0x00005557afcff958 clang::CodeGenAction::ExecuteAction() /root/llvm-project/clang/lib/CodeGen/CodeGenAction.cpp:1102:5
#<!-- -->25 0x00005557b0041c21 clang::FrontendAction::Execute() /root/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1076:38
#<!-- -->26 0x00005557aff52e8b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) /root/llvm-project/clang/lib/Frontend/CompilerInstance.cpp:1056:42
#<!-- -->27 0x00005557b020d57d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) /root/llvm-project/clang/lib/FrontendTool/ExecuteCompilerInvocation.cpp:265:38
#<!-- -->28 0x00005557aa2c18c4 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) /root/llvm-project/clang/tools/driver/cc1_main.cpp:290:40
#<!-- -->29 0x00005557aa2b240b ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) /root/llvm-project/clang/tools/driver/driver.cpp:218:20
#<!-- -->30 0x00005557aa2b2950 clang_main(int, char**, llvm::ToolContext const&amp;) /root/llvm-project/clang/tools/driver/driver.cpp:259:26
#<!-- -->31 0x00005557aa2ee950 main /root/llvm-project/build-assert/tools/clang/tools/driver/clang-driver.cpp:17:20
#<!-- -->32 0x00007fe35c486083 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x24083)
#<!-- -->33 0x00005557aa2b176e _start (/root/llvm-project/build-assert/bin/clang-21+0x38e676e)
```

## Program

```
#include &lt;fcntl.h&gt; _Pragma(
```

## To quickly reproduce

It happens since clang 3.1.0 until current version

https://gcc.godbolt.org/z/T7Kn6nGnd

</details>


---

