# [clang] Assertion failure in Type.h when using vector comparison as the first operand for the conditional operator

**Author:** AbyssStaror
**URL:** https://github.com/llvm/llvm-project/issues/170432
**Status:** Closed
**Labels:** duplicate, clang:frontend, regression, crash-on-invalid
**Closed Date:** 2025-12-05T01:17:01Z

## Body

### Link for quick verification: https://godbolt.org/z/6MoP9xEK4
```c
typedef int v2di __attribute__((vector_size(8)));
#define VECT_VAR ((v2di){1, 3}) != ((v2di){5, 7}) ? NULL : "true"

#include <stdio.h>

int main() {
    printf("%s\n", VECT_VAR);
    return 0;
}
```
Clang crashes with an assertion failure when compiling code that uses a vector comparison as the condition in a conditional operator where one branch is a NULL pointer.

## Stack dump:
```
clang-21: /workspace/install/llvm/src/llvm-project/clang/include/clang/AST/Type.h:952: const clang::ExtQualsTypeCommonBase* clang::QualType::getCommonPtr() const: Assertion `!isNull() && "Cannot retrieve a NULL type pointer"' failed.
Stack dump:
0.      Program arguments: /workspace/install/llvm/build_21.1.4/bin/clang-21 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -main-file-name 10502.fuzz -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/workspace/exps -fcoverage-compilation-dir=/workspace/exps -resource-dir /workspace/install/llvm/build_21.1.4/lib/clang/21 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/x86_64-linux-gnu/c++/13 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/backward -internal-isystem /workspace/install/llvm/build_21.1.4/lib/clang/21/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -std=c++20 -fdeprecated-macro -ferror-limit 19 -fmessage-length=125 -fgnuc-version=4.2.1 -fno-implicit-modules -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/10502-24695d.o -x c++ /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_input_2_feature_1_exp/cases/10502.fuzz
1.      /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_input_2_feature_1_exp/cases/10502.fuzz:12:40: current parser token ')'
2.      /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_input_2_feature_1_exp/cases/10502.fuzz:10:12: parsing function body 'main'
3.      /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_input_2_feature_1_exp/cases/10502.fuzz:10:12: in compound statement ('{}')
4.      /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_input_2_feature_1_exp/cases/10502.fuzz:11:57: in compound statement ('{}')
 #0 0x00005868eb322c2a llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9691c2a)
 #1 0x00005868eb323457 PrintStackTraceSignalHandler(void*) Signals.cpp:0:0
 #2 0x00005868eb31cf57 llvm::sys::RunSignalHandlers() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x968bf57)
 #3 0x00005868eb31d1c7 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #4 0x00007e649b0cb330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #5 0x00007e649b124b2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #6 0x00007e649b124b2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #7 0x00007e649b124b2c pthread_kill ./nptl/pthread_kill.c:89:10
 #8 0x00007e649b0cb27e raise ./signal/../sysdeps/posix/raise.c:27:6
 #9 0x00007e649b0ae8ff abort ./stdlib/abort.c:81:7
#10 0x00007e649b0ae81b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#11 0x00007e649b0c1517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#12 0x00005868eb6e3aa9 (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9a52aa9)
#13 0x00005868ef0f060c clang::Sema::CheckVectorConditionalTypes(clang::ActionResult<clang::Expr*, true>&, clang::ActionResult<clang::Expr*, true>&, clang::ActionResult<clang::Expr*, true>&, clang::SourceLocation) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xd45f60c)
#14 0x00005868ef0fdaaf clang::Sema::CXXCheckConditionalOperands(clang::ActionResult<clang::Expr*, true>&, clang::ActionResult<clang::Expr*, true>&, clang::ActionResult<clang::Expr*, true>&, clang::ExprValueKind&, clang::ExprObjectKind&, clang::SourceLocation) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xd46caaf)
#15 0x00005868eeffcd7b clang::Sema::CheckConditionalOperands(clang::ActionResult<clang::Expr*, true>&, clang::ActionResult<clang::Expr*, true>&, clang::ActionResult<clang::Expr*, true>&, clang::ExprValueKind&, clang::ExprObjectKind&, clang::SourceLocation) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xd36bd7b)
#16 0x00005868eeffed4a clang::Sema::ActOnConditionalOp(clang::SourceLocation, clang::SourceLocation, clang::Expr*, clang::Expr*, clang::Expr*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xd36dd4a)
#17 0x00005868ee8bb258 clang::Parser::ParseRHSOfBinaryExpression(clang::ActionResult<clang::Expr*, true>, clang::prec::Level) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc2a258)
#18 0x00005868ee8b51ac clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc241ac)
#19 0x00005868ee8b531a clang::Parser::ParseSimpleExpressionList(llvm::SmallVectorImpl<clang::Expr*>&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc2431a)
#20 0x00005868ee8be4ef clang::Parser::ParseParenExpression(clang::ParenParseOption&, bool, clang::ParenExprKind, clang::TypoCorrectionTypeBehavior, clang::OpaquePtr<clang::QualType>&, clang::SourceLocation&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc2d4ef)
#21 0x00005868ee8b22da clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypoCorrectionTypeBehavior, bool, bool*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc212da)
#22 0x00005868ee8b50d1 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc240d1)
#23 0x00005868ee8be41d clang::Parser::ParseParenExpression(clang::ParenParseOption&, bool, clang::ParenExprKind, clang::TypoCorrectionTypeBehavior, clang::OpaquePtr<clang::QualType>&, clang::SourceLocation&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc2d41d)
#24 0x00005868ee8b22da clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypoCorrectionTypeBehavior, bool, bool*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc212da)
#25 0x00005868ee8b50d1 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc240d1)
#26 0x00005868ee8b20ec clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypoCorrectionTypeBehavior, bool, bool*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc210ec)
#27 0x00005868ee8b50d1 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc240d1)
#28 0x00005868ee8b5194 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc24194)
#29 0x00005868ee8b56cd clang::Parser::ParseExpressionList(llvm::SmallVectorImpl<clang::Expr*>&, llvm::function_ref<void ()>, bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc246cd)
#30 0x00005868ee8b790e clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult<clang::Expr*, true>) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc2690e)
#31 0x00005868ee8b192b clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypoCorrectionTypeBehavior, bool, bool*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc2092b)
#32 0x00005868ee8b50d1 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc240d1)
#33 0x00005868ee8b5194 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc24194)
#34 0x00005868ee8b6a15 clang::Parser::ParseExpression(clang::TypoCorrectionTypeBehavior) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc25a15)
#35 0x00005868ee96f257 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xccde257)
#36 0x00005868ee971487 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcce0487)
#37 0x00005868ee972099 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcce1099)
#38 0x00005868ee973e9e clang::Parser::ParseCompoundStatementBody(bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcce2e9e)
#39 0x00005868ee9747d0 void llvm::function_ref<void ()>::callback_fn<clang::Parser::ParseCompoundStatement(bool, unsigned int)::'lambda'()>(long) ParseStmt.cpp:0:0
#40 0x00005868f0d3642b clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xf0a542b)
#41 0x00005868ee967e0e clang::Parser::ParseCompoundStatement(bool, unsigned int) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xccd6e0e)
#42 0x00005868ee9709c7 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xccdf9c7)
#43 0x00005868ee972099 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcce1099)
#44 0x00005868ee974ad1 clang::Parser::ParseStatement(clang::SourceLocation*, clang::Parser::ParsedStmtContext) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcce3ad1)
#45 0x00005868ee978fcf clang::Parser::ParseIfStatement(clang::SourceLocation*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcce7fcf)
#46 0x00005868ee970855 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xccdf855)
#47 0x00005868ee972099 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcce1099)
#48 0x00005868ee973e9e clang::Parser::ParseCompoundStatementBody(bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcce2e9e)
#49 0x00005868ee9748c2 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcce38c2)
#50 0x00005868ee818337 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb87337)
#51 0x00005868ee871d18 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcbe0d18)
#52 0x00005868ee80e307 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb7d307)
#53 0x00005868ee80ee9e clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb7de9e)
#54 0x00005868ee81af98 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb89f98)
#55 0x00005868ee81c59d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb8b59d)
#56 0x00005868ee8029d0 clang::ParseAST(clang::Sema&, bool, bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb719d0)
#57 0x00005868ec54a92b clang::ASTFrontendAction::ExecuteAction() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa8b992b)
#58 0x00005868ec138360 clang::CodeGenAction::ExecuteAction() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa4a7360)
#59 0x00005868ec5529cf clang::FrontendAction::Execute() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa8c19cf)
#60 0x00005868ec498f46 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa807f46)
#61 0x00005868ec6daf7b clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xaa49f7b)
#62 0x00005868e6449523 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47b8523)
#63 0x00005868e6439c58 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#64 0x00005868e643fb28 clang_main(int, char**, llvm::ToolContext const&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47aeb28)
#65 0x00005868e629ab4a main (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x4609b4a)
#66 0x00007e649b0b01ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#67 0x00007e649b0b028b call_init ./csu/../csu/libc-start.c:128:20
#68 0x00007e649b0b028b __libc_start_main ./csu/../csu/libc-start.c:347:5
#69 0x00005868e6437f55 _start (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47a6f55)
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

Started to crash since Clang 10. [Godbolt link](https://godbolt.org/z/oYWTMYrc9).

Type mismatch seems unrelated. `((v2di){1, 3}) != ((v2di){5, 7}) ? "true" : "true"` also causes crashing.

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (AbyssStaror)

<details>
### Link for quick verification: https://godbolt.org/z/6MoP9xEK4
```c
typedef int v2di __attribute__((vector_size(8)));
#define VECT_VAR ((v2di){1, 3}) != ((v2di){5, 7}) ? NULL : "true"

#include &lt;stdio.h&gt;

int main() {
    printf("%s\n", VECT_VAR);
    return 0;
}
```
Clang crashes with an assertion failure when compiling code that uses a vector comparison as the condition in a conditional operator where one branch is a NULL pointer.

## Stack dump:
```
clang-21: /workspace/install/llvm/src/llvm-project/clang/include/clang/AST/Type.h:952: const clang::ExtQualsTypeCommonBase* clang::QualType::getCommonPtr() const: Assertion `!isNull() &amp;&amp; "Cannot retrieve a NULL type pointer"' failed.
Stack dump:
0.      Program arguments: /workspace/install/llvm/build_21.1.4/bin/clang-21 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -main-file-name 10502.fuzz -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/workspace/exps -fcoverage-compilation-dir=/workspace/exps -resource-dir /workspace/install/llvm/build_21.1.4/lib/clang/21 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/x86_64-linux-gnu/c++/13 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/backward -internal-isystem /workspace/install/llvm/build_21.1.4/lib/clang/21/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -std=c++20 -fdeprecated-macro -ferror-limit 19 -fmessage-length=125 -fgnuc-version=4.2.1 -fno-implicit-modules -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/10502-24695d.o -x c++ /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_input_2_feature_1_exp/cases/10502.fuzz
1.      /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_input_2_feature_1_exp/cases/10502.fuzz:12:40: current parser token ')'
2.      /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_input_2_feature_1_exp/cases/10502.fuzz:10:12: parsing function body 'main'
3.      /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_input_2_feature_1_exp/cases/10502.fuzz:10:12: in compound statement ('{}')
4.      /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_input_2_feature_1_exp/cases/10502.fuzz:11:57: in compound statement ('{}')
 #<!-- -->0 0x00005868eb322c2a llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9691c2a)
 #<!-- -->1 0x00005868eb323457 PrintStackTraceSignalHandler(void*) Signals.cpp:0:0
 #<!-- -->2 0x00005868eb31cf57 llvm::sys::RunSignalHandlers() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x968bf57)
 #<!-- -->3 0x00005868eb31d1c7 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->4 0x00007e649b0cb330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->5 0x00007e649b124b2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->6 0x00007e649b124b2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->7 0x00007e649b124b2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->8 0x00007e649b0cb27e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x00007e649b0ae8ff abort ./stdlib/abort.c:81:7
#<!-- -->10 0x00007e649b0ae81b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->11 0x00007e649b0c1517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->12 0x00005868eb6e3aa9 (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9a52aa9)
#<!-- -->13 0x00005868ef0f060c clang::Sema::CheckVectorConditionalTypes(clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, clang::SourceLocation) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xd45f60c)
#<!-- -->14 0x00005868ef0fdaaf clang::Sema::CXXCheckConditionalOperands(clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, clang::ExprValueKind&amp;, clang::ExprObjectKind&amp;, clang::SourceLocation) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xd46caaf)
#<!-- -->15 0x00005868eeffcd7b clang::Sema::CheckConditionalOperands(clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, clang::ExprValueKind&amp;, clang::ExprObjectKind&amp;, clang::SourceLocation) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xd36bd7b)
#<!-- -->16 0x00005868eeffed4a clang::Sema::ActOnConditionalOp(clang::SourceLocation, clang::SourceLocation, clang::Expr*, clang::Expr*, clang::Expr*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xd36dd4a)
#<!-- -->17 0x00005868ee8bb258 clang::Parser::ParseRHSOfBinaryExpression(clang::ActionResult&lt;clang::Expr*, true&gt;, clang::prec::Level) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc2a258)
#<!-- -->18 0x00005868ee8b51ac clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc241ac)
#<!-- -->19 0x00005868ee8b531a clang::Parser::ParseSimpleExpressionList(llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc2431a)
#<!-- -->20 0x00005868ee8be4ef clang::Parser::ParseParenExpression(clang::ParenParseOption&amp;, bool, clang::ParenExprKind, clang::TypoCorrectionTypeBehavior, clang::OpaquePtr&lt;clang::QualType&gt;&amp;, clang::SourceLocation&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc2d4ef)
#<!-- -->21 0x00005868ee8b22da clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&amp;, clang::TypoCorrectionTypeBehavior, bool, bool*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc212da)
#<!-- -->22 0x00005868ee8b50d1 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc240d1)
#<!-- -->23 0x00005868ee8be41d clang::Parser::ParseParenExpression(clang::ParenParseOption&amp;, bool, clang::ParenExprKind, clang::TypoCorrectionTypeBehavior, clang::OpaquePtr&lt;clang::QualType&gt;&amp;, clang::SourceLocation&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc2d41d)
#<!-- -->24 0x00005868ee8b22da clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&amp;, clang::TypoCorrectionTypeBehavior, bool, bool*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc212da)
#<!-- -->25 0x00005868ee8b50d1 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc240d1)
#<!-- -->26 0x00005868ee8b20ec clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&amp;, clang::TypoCorrectionTypeBehavior, bool, bool*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc210ec)
#<!-- -->27 0x00005868ee8b50d1 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc240d1)
#<!-- -->28 0x00005868ee8b5194 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc24194)
#<!-- -->29 0x00005868ee8b56cd clang::Parser::ParseExpressionList(llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, llvm::function_ref&lt;void ()&gt;, bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc246cd)
#<!-- -->30 0x00005868ee8b790e clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult&lt;clang::Expr*, true&gt;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc2690e)
#<!-- -->31 0x00005868ee8b192b clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&amp;, clang::TypoCorrectionTypeBehavior, bool, bool*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc2092b)
#<!-- -->32 0x00005868ee8b50d1 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc240d1)
#<!-- -->33 0x00005868ee8b5194 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc24194)
#<!-- -->34 0x00005868ee8b6a15 clang::Parser::ParseExpression(clang::TypoCorrectionTypeBehavior) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc25a15)
#<!-- -->35 0x00005868ee96f257 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xccde257)
#<!-- -->36 0x00005868ee971487 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcce0487)
#<!-- -->37 0x00005868ee972099 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcce1099)
#<!-- -->38 0x00005868ee973e9e clang::Parser::ParseCompoundStatementBody(bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcce2e9e)
#<!-- -->39 0x00005868ee9747d0 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::Parser::ParseCompoundStatement(bool, unsigned int)::'lambda'()&gt;(long) ParseStmt.cpp:0:0
#<!-- -->40 0x00005868f0d3642b clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xf0a542b)
#<!-- -->41 0x00005868ee967e0e clang::Parser::ParseCompoundStatement(bool, unsigned int) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xccd6e0e)
#<!-- -->42 0x00005868ee9709c7 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xccdf9c7)
#<!-- -->43 0x00005868ee972099 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcce1099)
#<!-- -->44 0x00005868ee974ad1 clang::Parser::ParseStatement(clang::SourceLocation*, clang::Parser::ParsedStmtContext) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcce3ad1)
#<!-- -->45 0x00005868ee978fcf clang::Parser::ParseIfStatement(clang::SourceLocation*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcce7fcf)
#<!-- -->46 0x00005868ee970855 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xccdf855)
#<!-- -->47 0x00005868ee972099 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcce1099)
#<!-- -->48 0x00005868ee973e9e clang::Parser::ParseCompoundStatementBody(bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcce2e9e)
#<!-- -->49 0x00005868ee9748c2 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcce38c2)
#<!-- -->50 0x00005868ee818337 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb87337)
#<!-- -->51 0x00005868ee871d18 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcbe0d18)
#<!-- -->52 0x00005868ee80e307 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb7d307)
#<!-- -->53 0x00005868ee80ee9e clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb7de9e)
#<!-- -->54 0x00005868ee81af98 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb89f98)
#<!-- -->55 0x00005868ee81c59d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb8b59d)
#<!-- -->56 0x00005868ee8029d0 clang::ParseAST(clang::Sema&amp;, bool, bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb719d0)
#<!-- -->57 0x00005868ec54a92b clang::ASTFrontendAction::ExecuteAction() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa8b992b)
#<!-- -->58 0x00005868ec138360 clang::CodeGenAction::ExecuteAction() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa4a7360)
#<!-- -->59 0x00005868ec5529cf clang::FrontendAction::Execute() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa8c19cf)
#<!-- -->60 0x00005868ec498f46 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa807f46)
#<!-- -->61 0x00005868ec6daf7b clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xaa49f7b)
#<!-- -->62 0x00005868e6449523 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47b8523)
#<!-- -->63 0x00005868e6439c58 ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->64 0x00005868e643fb28 clang_main(int, char**, llvm::ToolContext const&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47aeb28)
#<!-- -->65 0x00005868e629ab4a main (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x4609b4a)
#<!-- -->66 0x00007e649b0b01ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->67 0x00007e649b0b028b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->68 0x00007e649b0b028b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->69 0x00005868e6437f55 _start (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47a6f55)
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

### Comment 3 - AbyssStaror

> Started to crash since Clang 10. [Godbolt link](https://godbolt.org/z/oYWTMYrc9).
> 
> Type mismatch seems unrelated. `((v2di){1, 3}) != ((v2di){5, 7}) ? "true" : "true"` also causes crashing.


`((v2di){1, 3}) != ((v2di){5, 7}) ? "true" : "true"` triggers the crash that is mostly like [issue 170431](https://github.com/llvm/llvm-project/issues/170431) in frontend.

---

