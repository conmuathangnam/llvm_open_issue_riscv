# Clang crash in StandardConversionSequence::isPerfect when parsing floor() call with _Atomic double argument

**Author:** AbyssStaror
**URL:** https://github.com/llvm/llvm-project/issues/170433
**Status:** Closed
**Labels:** clang:frontend, confirmed, crash-on-valid, regression:21
**Closed Date:** 2026-01-25T16:57:27Z

## Body

### Link for quick verification: https://godbolt.org/z/EGfq73McE
Reduced code: 
```c
_Atomic double atomic_value = 42.5;

double use_floor_and_division() {
    long cast_result = (long) floor(atomic_value);
    return cast_result;
}
```
- The crash occurs during the parsing of the `floor(atomic_value)`function call.

- The assertion fails in `StandardConversionSequence::isPerfect`during overload resolution.

- The issue appears to be related to handling the implicit conversion from `_Atomic double`to `double`when passing the argument to `floor()`


## Stack dump:
```
clang-21: /workspace/install/llvm/src/llvm-project/clang/include/clang/Sema/Overload.h:446: bool clang::StandardConversionSequence::isPerfect(const clang::ASTContext&) const: Assertion `C.hasSameUnqualifiedType(Decay(getFromType()), Decay(getToType(2)))' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /workspace/install/llvm/build_21.1.4/bin/clang-21 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -main-file-name 29195.fuzz -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/workspace/exps -fcoverage-compilation-dir=/workspace/exps -resource-dir /workspace/install/llvm/build_21.1.4/lib/clang/21 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/x86_64-linux-gnu/c++/13 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/backward -internal-isystem /workspace/install/llvm/build_21.1.4/lib/clang/21/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -std=c++20 -fdeprecated-macro -ferror-limit 19 -fmessage-length=137 -fgnuc-version=4.2.1 -fno-implicit-modules -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/29195-776d58.o -x c++ /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_input_2_feature_1_exp/cases/29195.fuzz
1.      /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_input_2_feature_1_exp/cases/29195.fuzz:12:49: current parser token ')'
2.      /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_input_2_feature_1_exp/cases/29195.fuzz:7:33: parsing function body 'use_floor_and_division'
3.      /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_input_2_feature_1_exp/cases/29195.fuzz:7:33: in compound statement ('{}')
 #0 0x00005a6e75feec2a llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9691c2a)
 #1 0x00005a6e75fef457 PrintStackTraceSignalHandler(void*) Signals.cpp:0:0
 #2 0x00005a6e75fe8f57 llvm::sys::RunSignalHandlers() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x968bf57)
 #3 0x00005a6e75fe91c7 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #4 0x000074725b6ba330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #5 0x000074725b713b2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #6 0x000074725b713b2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #7 0x000074725b713b2c pthread_kill ./nptl/pthread_kill.c:89:10
 #8 0x000074725b6ba27e raise ./signal/../sysdeps/posix/raise.c:27:6
 #9 0x000074725b69d8ff abort ./stdlib/abort.c:81:7
#10 0x000074725b69d81b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#11 0x000074725b6b0517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#12 0x00005a6e7a1b7314 clang::OverloadCandidateSet::PerfectViableFunction(clang::Sema&, clang::SourceLocation, clang::OverloadCandidate*&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xd85a314)
#13 0x00005a6e7a200c72 clang::OverloadCandidateSet::BestViableFunction(clang::Sema&, clang::SourceLocation, clang::OverloadCandidate*&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xd8a3c72)
#14 0x00005a6e7a212163 clang::Sema::BuildOverloadedCallExpr(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xd8b5163)
#15 0x00005a6e79c6bda8 clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xd30eda8)
#16 0x00005a6e79c6cace clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xd30face)
#17 0x00005a6e7958319d clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult<clang::Expr*, true>) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc2619d)
#18 0x00005a6e7957d92b clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypoCorrectionTypeBehavior, bool, bool*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc2092b)
#19 0x00005a6e795810d1 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc240d1)
#20 0x00005a6e7958a41d clang::Parser::ParseParenExpression(clang::ParenParseOption&, bool, clang::ParenExprKind, clang::TypoCorrectionTypeBehavior, clang::OpaquePtr<clang::QualType>&, clang::SourceLocation&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc2d41d)
#21 0x00005a6e7957e2da clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypoCorrectionTypeBehavior, bool, bool*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc212da)
#22 0x00005a6e795810d1 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc240d1)
#23 0x00005a6e79581194 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc24194)
#24 0x00005a6e7951b9f6 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcbbe9f6)
#25 0x00005a6e7953cd1b clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcbdfd1b)
#26 0x00005a6e7953ea1f clang::Parser::ParseSimpleDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, bool, clang::Parser::ForRangeInit*, clang::SourceLocation*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcbe1a1f)
#27 0x00005a6e7953f069 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcbe2069)
#28 0x00005a6e7963d46c clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcce046c)
#29 0x00005a6e7963e099 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcce1099)
#30 0x00005a6e7963fe9e clang::Parser::ParseCompoundStatementBody(bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcce2e9e)
#31 0x00005a6e796408c2 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcce38c2)
#32 0x00005a6e794e4337 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb87337)
#33 0x00005a6e7953dd18 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcbe0d18)
#34 0x00005a6e794da307 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb7d307)
#35 0x00005a6e794dae9e clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb7de9e)
#36 0x00005a6e794e6f98 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb89f98)
#37 0x00005a6e794e859d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb8b59d)
#38 0x00005a6e794ce9d0 clang::ParseAST(clang::Sema&, bool, bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb719d0)
#39 0x00005a6e7721692b clang::ASTFrontendAction::ExecuteAction() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa8b992b)
#40 0x00005a6e76e04360 clang::CodeGenAction::ExecuteAction() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa4a7360)
#41 0x00005a6e7721e9cf clang::FrontendAction::Execute() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa8c19cf)
#42 0x00005a6e77164f46 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa807f46)
#43 0x00005a6e773a6f7b clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xaa49f7b)
#44 0x00005a6e71115523 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47b8523)
#45 0x00005a6e71105c58 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#46 0x00005a6e7110bb28 clang_main(int, char**, llvm::ToolContext const&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47aeb28)
#47 0x00005a6e70f66b4a main (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x4609b4a)
#48 0x000074725b69f1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#49 0x000074725b69f28b call_init ./csu/../csu/libc-start.c:128:20
#50 0x000074725b69f28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#51 0x00005a6e71103f55 _start (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47a6f55)
clang++: error: unable to execute command: Aborted (core dumped)
```
## Clang version:
```
clang version 21.1.4 (https://github.com/llvm/llvm-project.git 222fc11f2b8f25f6a0f4976272ef1bb7bf49521d)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /workspace/install/llvm/build_21.1.4/bin
Build config: +assertions
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/13
Selected GCC installation: /usr/lib/gcc/x86_64-linux-gnu/13
Candidate multilib: .;@m64
Selected multilib: .;@m64
```

## Comments

### Comment 1 - frederick-vs-ja

Reduced to
```C++
_Atomic double atomic_value = 0.0;

void fun(double);
template<class I>
void fun(I);

using T = decltype(::fun(atomic_value));
```

Started to crash since Clang 21. [Godbolt link](https://godbolt.org/z/6vWr6Eef5).

Looks like https://github.com/llvm/llvm-project/labels/crash-on-valid to me despite using `_Atomic` from C11 as an extension.

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (AbyssStaror)

<details>
### Link for quick verification: https://godbolt.org/z/EGfq73McE
Reduced code: 
```c
_Atomic double atomic_value = 42.5;

double use_floor_and_division() {
    long cast_result = (long) floor(atomic_value);
    return cast_result;
}
```
- The crash occurs during the parsing of the `floor(atomic_value)`function call.

- The assertion fails in `StandardConversionSequence::isPerfect`during overload resolution.

- The issue appears to be related to handling the implicit conversion from `_Atomic double`to `double`when passing the argument to `floor()`


## Stack dump:
```
clang-21: /workspace/install/llvm/src/llvm-project/clang/include/clang/Sema/Overload.h:446: bool clang::StandardConversionSequence::isPerfect(const clang::ASTContext&amp;) const: Assertion `C.hasSameUnqualifiedType(Decay(getFromType()), Decay(getToType(2)))' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /workspace/install/llvm/build_21.1.4/bin/clang-21 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -main-file-name 29195.fuzz -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/workspace/exps -fcoverage-compilation-dir=/workspace/exps -resource-dir /workspace/install/llvm/build_21.1.4/lib/clang/21 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/x86_64-linux-gnu/c++/13 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/backward -internal-isystem /workspace/install/llvm/build_21.1.4/lib/clang/21/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -std=c++20 -fdeprecated-macro -ferror-limit 19 -fmessage-length=137 -fgnuc-version=4.2.1 -fno-implicit-modules -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/29195-776d58.o -x c++ /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_input_2_feature_1_exp/cases/29195.fuzz
1.      /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_input_2_feature_1_exp/cases/29195.fuzz:12:49: current parser token ')'
2.      /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_input_2_feature_1_exp/cases/29195.fuzz:7:33: parsing function body 'use_floor_and_division'
3.      /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_input_2_feature_1_exp/cases/29195.fuzz:7:33: in compound statement ('{}')
 #<!-- -->0 0x00005a6e75feec2a llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9691c2a)
 #<!-- -->1 0x00005a6e75fef457 PrintStackTraceSignalHandler(void*) Signals.cpp:0:0
 #<!-- -->2 0x00005a6e75fe8f57 llvm::sys::RunSignalHandlers() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x968bf57)
 #<!-- -->3 0x00005a6e75fe91c7 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->4 0x000074725b6ba330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->5 0x000074725b713b2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->6 0x000074725b713b2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->7 0x000074725b713b2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->8 0x000074725b6ba27e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x000074725b69d8ff abort ./stdlib/abort.c:81:7
#<!-- -->10 0x000074725b69d81b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->11 0x000074725b6b0517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->12 0x00005a6e7a1b7314 clang::OverloadCandidateSet::PerfectViableFunction(clang::Sema&amp;, clang::SourceLocation, clang::OverloadCandidate*&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xd85a314)
#<!-- -->13 0x00005a6e7a200c72 clang::OverloadCandidateSet::BestViableFunction(clang::Sema&amp;, clang::SourceLocation, clang::OverloadCandidate*&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xd8a3c72)
#<!-- -->14 0x00005a6e7a212163 clang::Sema::BuildOverloadedCallExpr(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xd8b5163)
#<!-- -->15 0x00005a6e79c6bda8 clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xd30eda8)
#<!-- -->16 0x00005a6e79c6cace clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xd30face)
#<!-- -->17 0x00005a6e7958319d clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult&lt;clang::Expr*, true&gt;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc2619d)
#<!-- -->18 0x00005a6e7957d92b clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&amp;, clang::TypoCorrectionTypeBehavior, bool, bool*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc2092b)
#<!-- -->19 0x00005a6e795810d1 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc240d1)
#<!-- -->20 0x00005a6e7958a41d clang::Parser::ParseParenExpression(clang::ParenParseOption&amp;, bool, clang::ParenExprKind, clang::TypoCorrectionTypeBehavior, clang::OpaquePtr&lt;clang::QualType&gt;&amp;, clang::SourceLocation&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc2d41d)
#<!-- -->21 0x00005a6e7957e2da clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&amp;, clang::TypoCorrectionTypeBehavior, bool, bool*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc212da)
#<!-- -->22 0x00005a6e795810d1 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc240d1)
#<!-- -->23 0x00005a6e79581194 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc24194)
#<!-- -->24 0x00005a6e7951b9f6 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcbbe9f6)
#<!-- -->25 0x00005a6e7953cd1b clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcbdfd1b)
#<!-- -->26 0x00005a6e7953ea1f clang::Parser::ParseSimpleDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, bool, clang::Parser::ForRangeInit*, clang::SourceLocation*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcbe1a1f)
#<!-- -->27 0x00005a6e7953f069 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcbe2069)
#<!-- -->28 0x00005a6e7963d46c clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcce046c)
#<!-- -->29 0x00005a6e7963e099 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcce1099)
#<!-- -->30 0x00005a6e7963fe9e clang::Parser::ParseCompoundStatementBody(bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcce2e9e)
#<!-- -->31 0x00005a6e796408c2 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcce38c2)
#<!-- -->32 0x00005a6e794e4337 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb87337)
#<!-- -->33 0x00005a6e7953dd18 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcbe0d18)
#<!-- -->34 0x00005a6e794da307 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb7d307)
#<!-- -->35 0x00005a6e794dae9e clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb7de9e)
#<!-- -->36 0x00005a6e794e6f98 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb89f98)
#<!-- -->37 0x00005a6e794e859d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb8b59d)
#<!-- -->38 0x00005a6e794ce9d0 clang::ParseAST(clang::Sema&amp;, bool, bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb719d0)
#<!-- -->39 0x00005a6e7721692b clang::ASTFrontendAction::ExecuteAction() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa8b992b)
#<!-- -->40 0x00005a6e76e04360 clang::CodeGenAction::ExecuteAction() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa4a7360)
#<!-- -->41 0x00005a6e7721e9cf clang::FrontendAction::Execute() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa8c19cf)
#<!-- -->42 0x00005a6e77164f46 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa807f46)
#<!-- -->43 0x00005a6e773a6f7b clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xaa49f7b)
#<!-- -->44 0x00005a6e71115523 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47b8523)
#<!-- -->45 0x00005a6e71105c58 ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->46 0x00005a6e7110bb28 clang_main(int, char**, llvm::ToolContext const&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47aeb28)
#<!-- -->47 0x00005a6e70f66b4a main (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x4609b4a)
#<!-- -->48 0x000074725b69f1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->49 0x000074725b69f28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->50 0x000074725b69f28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->51 0x00005a6e71103f55 _start (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47a6f55)
clang++: error: unable to execute command: Aborted (core dumped)
```
## Clang version:
```
clang version 21.1.4 (https://github.com/llvm/llvm-project.git 222fc11f2b8f25f6a0f4976272ef1bb7bf49521d)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /workspace/install/llvm/build_21.1.4/bin
Build config: +assertions
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/13
Selected GCC installation: /usr/lib/gcc/x86_64-linux-gnu/13
Candidate multilib: .;@<!-- -->m64
Selected multilib: .;@<!-- -->m64
```
</details>


---

### Comment 3 - shafik

@frederick-vs-ja the Regressions project should be visible to you, please add that project to regressions.

---

### Comment 4 - shafik

@cor3ntin this bisect to 8c5a307bd8d406e6167a5cd3ce3c74e2e3bfb2a6

---

### Comment 5 - keinflue

Reduction:
```cpp
void f(double);

template <class = int>
void f(double);

_Atomic double atomic_value = 42.5;

void test() {
    f(atomic_value);
}
```
https://godbolt.org/z/5sGq7qv7T

---

