# [clang] Assertion failure with unknown header

**Author:** Heath123
**URL:** https://github.com/llvm/llvm-project/issues/156458
**Status:** Closed
**Labels:** clang:frontend, regression, crash-on-invalid, confirmed
**Closed Date:** 2025-09-04T00:27:32Z

## Body

When compiling this with clang (tested on commit a65aa32a3bf153204ff51c23d432a2497533a656):
```cpp
enum class endianness {};
namespace llvm {}

#include <unknownheader>

constexpr bool IsBigEndianHost = llvm::endianness::native == llvm::endianness::big;
```
with the following:
`clang++ test.cpp`
the following assertion is hit:
```
/home/heath/crash/test.cpp:4:10: fatal error: 'unknownheader' file not found
    4 | #include <unknownheader>
      |          ^~~~~~~~~~~~~~~
clang-22: /home/heath/llvm-project/clang/lib/Sema/SemaCXXScopeSpec.cpp:772: bool clang::Sema::BuildCXXNestedNameSpecifier(Scope *, NestedNameSpecInfo &, bool, CXXScopeSpec &, NamedDecl *, bool, bool *, bool): Assertion `!isa<TagDecl>(TD)' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /home/heath/llvm-project/build-dbg-shr-clang/bin/clang-22 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -main-file-name test.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/home/heath/llvm-project/build-dbg-shr-clang -fcoverage-compilation-dir=/home/heath/llvm-project/build-dbg-shr-clang -resource-dir /home/heath/llvm-project/build-dbg-shr-clang/lib/clang/22 -internal-isystem /usr/lib/gcc/x86_64-redhat-linux/15/../../../../include/c++/15 -internal-isystem /usr/lib/gcc/x86_64-redhat-linux/15/../../../../include/c++/15/x86_64-redhat-linux -internal-isystem /usr/lib/gcc/x86_64-redhat-linux/15/../../../../include/c++/15/backward -internal-isystem /home/heath/llvm-project/build-dbg-shr-clang/lib/clang/22/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-redhat-linux/15/../../../../x86_64-redhat-linux/include -internal-externc-isystem /include -internal-externc-isystem /usr/include -fdeprecated-macro -ferror-limit 19 -fmessage-length=226 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/test-82c079.o -x c++ /home/heath/crash/test.cpp
1.      /home/heath/crash/test.cpp:6:52: current parser token 'native'
 #0 0x00007cc6a1c1a25d llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /home/heath/llvm-project/llvm/lib/Support/Unix/Signals.inc:834:11
 #1 0x00007cc6a1c1a79b PrintStackTraceSignalHandler(void*) /home/heath/llvm-project/llvm/lib/Support/Unix/Signals.inc:918:1
 #2 0x00007cc6a1c1869f llvm::sys::RunSignalHandlers() /home/heath/llvm-project/llvm/lib/Support/Signals.cpp:104:5
 #3 0x00007cc6a1c1adc9 SignalHandler(int, siginfo_t*, void*) /home/heath/llvm-project/llvm/lib/Support/Unix/Signals.inc:426:38
 #4 0x00007cc6a1228bb0 __restore_rt (/lib64/libc.so.6+0x19bb0)
 #5 0x00007cc6a128209c __pthread_kill_implementation (/lib64/libc.so.6+0x7309c)
 #6 0x00007cc6a1228a7e gsignal (/lib64/libc.so.6+0x19a7e)
 #7 0x00007cc6a12106d0 abort (/lib64/libc.so.6+0x16d0)
 #8 0x00007cc6a1210639 __assert_perror_fail (/lib64/libc.so.6+0x1639)
 #9 0x00007cc69b620a56 clang::Sema::BuildCXXNestedNameSpecifier(clang::Scope*, clang::Sema::NestedNameSpecInfo&, bool, clang::CXXScopeSpec&, clang::NamedDecl*, bool, bool*, bool) /home/heath/llvm-project/clang/lib/Sema/SemaCXXScopeSpec.cpp:773:9
#10 0x00007cc69b620ee7 clang::Sema::ActOnCXXNestedNameSpecifier(clang::Scope*, clang::Sema::NestedNameSpecInfo&, bool, clang::CXXScopeSpec&, bool*, bool) /home/heath/llvm-project/clang/lib/Sema/SemaCXXScopeSpec.cpp:807:3
#11 0x00007cc69c7979be clang::Parser::ParseOptionalCXXScopeSpecifier(clang::CXXScopeSpec&, clang::OpaquePtr<clang::QualType>, bool, bool, bool*, bool, clang::IdentifierInfo const**, bool, bool, bool) /home/heath/llvm-project/clang/lib/Parse/ParseExprCXX.cpp:466:19
#12 0x00007cc69c86294b clang::Parser::TryAnnotateTypeOrScopeToken(clang::ImplicitTypenameContext) /home/heath/llvm-project/clang/lib/Parse/Parser.cpp:2011:9
#13 0x00007cc69c780217 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypoCorrectionTypeBehavior, bool, bool*) /home/heath/llvm-project/clang/lib/Parse/ParseExpr.cpp:905:13
#14 0x00007cc69c77cbbc clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) /home/heath/llvm-project/clang/lib/Parse/ParseExpr.cpp:567:20
#15 0x00007cc69c77b5e6 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) /home/heath/llvm-project/clang/lib/Parse/ParseExpr.cpp:90:7
#16 0x00007cc69c741b33 clang::Parser::ParseInitializer() /home/heath/llvm-project/clang/include/clang/Parse/Parser.h:5220:14
#17 0x00007cc69c72767d clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) /home/heath/llvm-project/clang/lib/Parse/ParseDecl.cpp:2613:25
#18 0x00007cc69c725e84 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) /home/heath/llvm-project/clang/lib/Parse/ParseDecl.cpp:2352:9
#19 0x00007cc69c85eca8 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) /home/heath/llvm-project/clang/lib/Parse/Parser.cpp:1187:10
#20 0x00007cc69c85e25b clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) /home/heath/llvm-project/clang/lib/Parse/Parser.cpp:1209:12
#21 0x00007cc69c85db54 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) /home/heath/llvm-project/clang/lib/Parse/Parser.cpp:1032:14
#22 0x00007cc69c85b9d8 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) /home/heath/llvm-project/clang/lib/Parse/Parser.cpp:745:12
#23 0x00007cc69c6fe98c clang::ParseAST(clang::Sema&, bool, bool) /home/heath/llvm-project/clang/lib/Parse/ParseAST.cpp:170:16
#24 0x00007cc6a717a2c9 clang::ASTFrontendAction::ExecuteAction() /home/heath/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1430:1
#25 0x00007cc6ab2c260d clang::CodeGenAction::ExecuteAction() /home/heath/llvm-project/clang/lib/CodeGen/CodeGenAction.cpp:1114:5
#26 0x00007cc6a7179ca6 clang::FrontendAction::Execute() /home/heath/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1312:7
#27 0x00007cc6a7070323 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) /home/heath/llvm-project/clang/lib/Frontend/CompilerInstance.cpp:1066:23
#28 0x00007cc6acd4147a clang::ExecuteCompilerInvocation(clang::CompilerInstance*) /home/heath/llvm-project/clang/lib/FrontendTool/ExecuteCompilerInvocation.cpp:302:8
#29 0x00000000002479f7 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) /home/heath/llvm-project/clang/tools/driver/cc1_main.cpp:297:13
#30 0x000000000023b01e ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) /home/heath/llvm-project/clang/tools/driver/driver.cpp:223:5
#31 0x0000000000239e37 clang_main(int, char**, llvm::ToolContext const&) /home/heath/llvm-project/clang/tools/driver/driver.cpp:264:5
#32 0x000000000026da55 main /home/heath/llvm-project/build-dbg-shr-clang/tools/clang/tools/driver/clang-driver.cpp:17:3
#33 0x00007cc6a1212575 __libc_start_call_main (/lib64/libc.so.6+0x3575)
#34 0x00007cc6a1212628 __libc_start_main@GLIBC_2.2.5 (/lib64/libc.so.6+0x3628)
#35 0x00000000002398f5 _start (/home/heath/llvm-project/build-dbg-shr-clang/bin/clang-22+0x2398f5)
clang++: error: unable to execute command: Aborted (core dumped)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 22.0.0git (https://github.com/Heath123/llvm-project a65aa32a3bf153204ff51c23d432a2497533a656)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /home/heath/llvm-project/build-dbg-shr-clang/bin
Build config: +unoptimized, +assertions
clang++: note: diagnostic msg: Error generating preprocessed source(s).
```

## Comments

### Comment 1 - Fznamznon

Confirmed https://godbolt.org/z/rd3bb9zY8 . It is an assertion failure, so no crash in release mode. Seems to be a recent regression on main. Clang 21 doesn't have it.

---

### Comment 2 - Fznamznon

It seems without header we step into typo correction patch and exit from ``BuildCXXNestedNameSpecifier`` earlier than the failed assertion. However missing header is a fatal error and typo correction doesn't work if there has been a fatal error we come to this assertion
https://github.com/llvm/llvm-project/blob/b16930204b230240d834f667c8f32b12ca4ad198/clang/lib/Sema/SemaCXXScopeSpec.cpp#L772
with an EnumDecl
```
EnumDecl 0x5555667d1960 <t.cpp:1:1, col:24> col:12 class endianness 'int'
```
The assertion was added by https://github.com/llvm/llvm-project/pull/147835 , so cc @mizvekov 

---

### Comment 3 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: circuit10 (Heath123)

<details>
When compiling this with clang (tested on commit a65aa32a3bf153204ff51c23d432a2497533a656):
```cpp
enum class endianness {};
namespace llvm {}

#include &lt;unknownheader&gt;

constexpr bool IsBigEndianHost = llvm::endianness::native == llvm::endianness::big;
```
with the following:
`clang++ test.cpp`
the following assertion is hit:
```
/home/heath/crash/test.cpp:4:10: fatal error: 'unknownheader' file not found
    4 | #include &lt;unknownheader&gt;
      |          ^~~~~~~~~~~~~~~
clang-22: /home/heath/llvm-project/clang/lib/Sema/SemaCXXScopeSpec.cpp:772: bool clang::Sema::BuildCXXNestedNameSpecifier(Scope *, NestedNameSpecInfo &amp;, bool, CXXScopeSpec &amp;, NamedDecl *, bool, bool *, bool): Assertion `!isa&lt;TagDecl&gt;(TD)' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /home/heath/llvm-project/build-dbg-shr-clang/bin/clang-22 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -main-file-name test.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/home/heath/llvm-project/build-dbg-shr-clang -fcoverage-compilation-dir=/home/heath/llvm-project/build-dbg-shr-clang -resource-dir /home/heath/llvm-project/build-dbg-shr-clang/lib/clang/22 -internal-isystem /usr/lib/gcc/x86_64-redhat-linux/15/../../../../include/c++/15 -internal-isystem /usr/lib/gcc/x86_64-redhat-linux/15/../../../../include/c++/15/x86_64-redhat-linux -internal-isystem /usr/lib/gcc/x86_64-redhat-linux/15/../../../../include/c++/15/backward -internal-isystem /home/heath/llvm-project/build-dbg-shr-clang/lib/clang/22/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-redhat-linux/15/../../../../x86_64-redhat-linux/include -internal-externc-isystem /include -internal-externc-isystem /usr/include -fdeprecated-macro -ferror-limit 19 -fmessage-length=226 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/test-82c079.o -x c++ /home/heath/crash/test.cpp
1.      /home/heath/crash/test.cpp:6:52: current parser token 'native'
 #<!-- -->0 0x00007cc6a1c1a25d llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /home/heath/llvm-project/llvm/lib/Support/Unix/Signals.inc:834:11
 #<!-- -->1 0x00007cc6a1c1a79b PrintStackTraceSignalHandler(void*) /home/heath/llvm-project/llvm/lib/Support/Unix/Signals.inc:918:1
 #<!-- -->2 0x00007cc6a1c1869f llvm::sys::RunSignalHandlers() /home/heath/llvm-project/llvm/lib/Support/Signals.cpp:104:5
 #<!-- -->3 0x00007cc6a1c1adc9 SignalHandler(int, siginfo_t*, void*) /home/heath/llvm-project/llvm/lib/Support/Unix/Signals.inc:426:38
 #<!-- -->4 0x00007cc6a1228bb0 __restore_rt (/lib64/libc.so.6+0x19bb0)
 #<!-- -->5 0x00007cc6a128209c __pthread_kill_implementation (/lib64/libc.so.6+0x7309c)
 #<!-- -->6 0x00007cc6a1228a7e gsignal (/lib64/libc.so.6+0x19a7e)
 #<!-- -->7 0x00007cc6a12106d0 abort (/lib64/libc.so.6+0x16d0)
 #<!-- -->8 0x00007cc6a1210639 __assert_perror_fail (/lib64/libc.so.6+0x1639)
 #<!-- -->9 0x00007cc69b620a56 clang::Sema::BuildCXXNestedNameSpecifier(clang::Scope*, clang::Sema::NestedNameSpecInfo&amp;, bool, clang::CXXScopeSpec&amp;, clang::NamedDecl*, bool, bool*, bool) /home/heath/llvm-project/clang/lib/Sema/SemaCXXScopeSpec.cpp:773:9
#<!-- -->10 0x00007cc69b620ee7 clang::Sema::ActOnCXXNestedNameSpecifier(clang::Scope*, clang::Sema::NestedNameSpecInfo&amp;, bool, clang::CXXScopeSpec&amp;, bool*, bool) /home/heath/llvm-project/clang/lib/Sema/SemaCXXScopeSpec.cpp:807:3
#<!-- -->11 0x00007cc69c7979be clang::Parser::ParseOptionalCXXScopeSpecifier(clang::CXXScopeSpec&amp;, clang::OpaquePtr&lt;clang::QualType&gt;, bool, bool, bool*, bool, clang::IdentifierInfo const**, bool, bool, bool) /home/heath/llvm-project/clang/lib/Parse/ParseExprCXX.cpp:466:19
#<!-- -->12 0x00007cc69c86294b clang::Parser::TryAnnotateTypeOrScopeToken(clang::ImplicitTypenameContext) /home/heath/llvm-project/clang/lib/Parse/Parser.cpp:2011:9
#<!-- -->13 0x00007cc69c780217 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&amp;, clang::TypoCorrectionTypeBehavior, bool, bool*) /home/heath/llvm-project/clang/lib/Parse/ParseExpr.cpp:905:13
#<!-- -->14 0x00007cc69c77cbbc clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) /home/heath/llvm-project/clang/lib/Parse/ParseExpr.cpp:567:20
#<!-- -->15 0x00007cc69c77b5e6 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) /home/heath/llvm-project/clang/lib/Parse/ParseExpr.cpp:90:7
#<!-- -->16 0x00007cc69c741b33 clang::Parser::ParseInitializer() /home/heath/llvm-project/clang/include/clang/Parse/Parser.h:5220:14
#<!-- -->17 0x00007cc69c72767d clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) /home/heath/llvm-project/clang/lib/Parse/ParseDecl.cpp:2613:25
#<!-- -->18 0x00007cc69c725e84 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) /home/heath/llvm-project/clang/lib/Parse/ParseDecl.cpp:2352:9
#<!-- -->19 0x00007cc69c85eca8 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) /home/heath/llvm-project/clang/lib/Parse/Parser.cpp:1187:10
#<!-- -->20 0x00007cc69c85e25b clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) /home/heath/llvm-project/clang/lib/Parse/Parser.cpp:1209:12
#<!-- -->21 0x00007cc69c85db54 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) /home/heath/llvm-project/clang/lib/Parse/Parser.cpp:1032:14
#<!-- -->22 0x00007cc69c85b9d8 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) /home/heath/llvm-project/clang/lib/Parse/Parser.cpp:745:12
#<!-- -->23 0x00007cc69c6fe98c clang::ParseAST(clang::Sema&amp;, bool, bool) /home/heath/llvm-project/clang/lib/Parse/ParseAST.cpp:170:16
#<!-- -->24 0x00007cc6a717a2c9 clang::ASTFrontendAction::ExecuteAction() /home/heath/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1430:1
#<!-- -->25 0x00007cc6ab2c260d clang::CodeGenAction::ExecuteAction() /home/heath/llvm-project/clang/lib/CodeGen/CodeGenAction.cpp:1114:5
#<!-- -->26 0x00007cc6a7179ca6 clang::FrontendAction::Execute() /home/heath/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1312:7
#<!-- -->27 0x00007cc6a7070323 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) /home/heath/llvm-project/clang/lib/Frontend/CompilerInstance.cpp:1066:23
#<!-- -->28 0x00007cc6acd4147a clang::ExecuteCompilerInvocation(clang::CompilerInstance*) /home/heath/llvm-project/clang/lib/FrontendTool/ExecuteCompilerInvocation.cpp:302:8
#<!-- -->29 0x00000000002479f7 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) /home/heath/llvm-project/clang/tools/driver/cc1_main.cpp:297:13
#<!-- -->30 0x000000000023b01e ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) /home/heath/llvm-project/clang/tools/driver/driver.cpp:223:5
#<!-- -->31 0x0000000000239e37 clang_main(int, char**, llvm::ToolContext const&amp;) /home/heath/llvm-project/clang/tools/driver/driver.cpp:264:5
#<!-- -->32 0x000000000026da55 main /home/heath/llvm-project/build-dbg-shr-clang/tools/clang/tools/driver/clang-driver.cpp:17:3
#<!-- -->33 0x00007cc6a1212575 __libc_start_call_main (/lib64/libc.so.6+0x3575)
#<!-- -->34 0x00007cc6a1212628 __libc_start_main@<!-- -->GLIBC_2.2.5 (/lib64/libc.so.6+0x3628)
#<!-- -->35 0x00000000002398f5 _start (/home/heath/llvm-project/build-dbg-shr-clang/bin/clang-22+0x2398f5)
clang++: error: unable to execute command: Aborted (core dumped)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 22.0.0git (https://github.com/Heath123/llvm-project a65aa32a3bf153204ff51c23d432a2497533a656)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /home/heath/llvm-project/build-dbg-shr-clang/bin
Build config: +unoptimized, +assertions
clang++: note: diagnostic msg: Error generating preprocessed source(s).
```
</details>


---

