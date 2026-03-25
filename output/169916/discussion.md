# [clang] Assertion failure in InitListExpr::getInit() during union compound literal parsing with multiple designated initializers

**Author:** AbyssStaror
**URL:** https://github.com/llvm/llvm-project/issues/169916
**Status:** Closed
**Labels:** duplicate, clang:frontend, crash-on-invalid
**Closed Date:** 2025-12-09T03:03:02Z

## Body

### Link for quick verification: https://godbolt.org/z/66vEcaWfb
```c
union u_type {
    int inner;
    int c;
};

int main() {
    (union u_type){ .c = 1, .inner = 2 }; 
}
```
When attempting to initialize multiple members of a union simultaneously, the compiler went crash.

## The stack dump:
```
clang-21: /workspace/install/llvm/src/llvm-project/clang/include/clang/AST/Expr.h:5287: clang::Expr* clang::InitListExpr::getInit(unsigned int): Assertion `Init < getNumInits() && "Initializer access out of range!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /workspace/install/llvm/build_21.1.4/bin/clang-21 -cc1 -triple x86_64-unknown-linux-gnu -O3 -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -main-file-name 34848.fuzz -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=none -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/workspace/exps -fcoverage-compilation-dir=/workspace/exps -resource-dir /workspace/install/llvm/build_21.1.4/lib/clang/21 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/x86_64-linux-gnu/c++/13 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/backward -internal-isystem /workspace/install/llvm/build_21.1.4/lib/clang/21/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -std=c++20 -fdeprecated-macro -ferror-limit 19 -fmessage-length=167 -fgnuc-version=4.2.1 -fno-implicit-modules -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -vectorize-loops -vectorize-slp -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/34848-a2cd0b.o -x c++ /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_choose_3_1_exp/cases/34848.fuzz
1.      /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_choose_3_1_exp/cases/34848.fuzz:29:49: current parser token ')'
2.      /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_choose_3_1_exp/cases/34848.fuzz:26:12: parsing function body 'main'
3.      /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_choose_3_1_exp/cases/34848.fuzz:26:12: in compound statement ('{}')
 #0 0x00005e5572a87c2a llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9691c2a)
 #1 0x00005e5572a88457 PrintStackTraceSignalHandler(void*) Signals.cpp:0:0
 #2 0x00005e5572a81f57 llvm::sys::RunSignalHandlers() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x968bf57)
 #3 0x00005e5572a821c7 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #4 0x000072371d508330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #5 0x000072371d561b2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #6 0x000072371d561b2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #7 0x000072371d561b2c pthread_kill ./nptl/pthread_kill.c:89:10
 #8 0x000072371d50827e raise ./signal/../sysdeps/posix/raise.c:27:6
 #9 0x000072371d4eb8ff abort ./stdlib/abort.c:81:7
#10 0x000072371d4eb81b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#11 0x000072371d4fe517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#12 0x00005e557305851c clang::InitListExpr::getInit(unsigned int) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9c6251c)
#13 0x00005e557693bfe5 (anonymous namespace)::InitListChecker::CheckDesignatedInitializer(clang::InitializedEntity const&, clang::InitListExpr*, clang::DesignatedInitExpr*, unsigned int, clang::QualType&, clang::DeclContext::specific_decl_iterator<clang::FieldDecl>*, llvm::APSInt*, unsigned int&, clang::InitListExpr*, unsigned int&, bool, bool) (.constprop.0) SemaInit.cpp:0:0
#14 0x00005e557693cbc6 (anonymous namespace)::InitListChecker::CheckStructUnionTypes(clang::InitializedEntity const&, clang::InitListExpr*, clang::QualType, llvm::iterator_range<clang::CXXBaseSpecifier const*>, clang::DeclContext::specific_decl_iterator<clang::FieldDecl>, bool, unsigned int&, clang::InitListExpr*, unsigned int&, bool) SemaInit.cpp:0:0
#15 0x00005e557693f25b (anonymous namespace)::InitListChecker::CheckListElementTypes(clang::InitializedEntity const&, clang::InitListExpr*, clang::QualType&, bool, unsigned int&, clang::InitListExpr*, unsigned int&, bool) (.constprop.0) SemaInit.cpp:0:0
#16 0x00005e5576940475 (anonymous namespace)::InitListChecker::CheckExplicitInitList(clang::InitializedEntity const&, clang::InitListExpr*, clang::QualType&, clang::InitListExpr*, bool) (.constprop.0) SemaInit.cpp:0:0
#17 0x00005e5576925b7d (anonymous namespace)::InitListChecker::InitListChecker(clang::Sema&, clang::InitializedEntity const&, clang::InitListExpr*, clang::QualType&, bool, bool, bool, llvm::SmallVectorImpl<clang::QualType>*) SemaInit.cpp:0:0
#18 0x00005e557692b069 clang::InitializationSequence::Perform(clang::Sema&, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::MutableArrayRef<clang::Expr*>, clang::QualType*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xd535069)
#19 0x00005e55766dff65 clang::Sema::BuildCompoundLiteralExpr(clang::SourceLocation, clang::TypeSourceInfo*, clang::SourceLocation, clang::Expr*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xd2e9f65)
#20 0x00005e55766e0e01 clang::Sema::ActOnCompoundLiteral(clang::SourceLocation, clang::OpaquePtr<clang::QualType>, clang::SourceLocation, clang::Expr*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xd2eae01)
#21 0x00005e5576021fa0 clang::Parser::ParseCompoundLiteralExpression(clang::OpaquePtr<clang::QualType>, clang::SourceLocation, clang::SourceLocation) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc2bfa0)
#22 0x00005e5576023fa0 clang::Parser::ParseParenExpression(clang::ParenParseOption&, bool, clang::ParenExprKind, clang::TypoCorrectionTypeBehavior, clang::OpaquePtr<clang::QualType>&, clang::SourceLocation&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc2dfa0)
#23 0x00005e55760172da clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypoCorrectionTypeBehavior, bool, bool*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc212da)
#24 0x00005e557601a0d1 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc240d1)
#25 0x00005e557601a194 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc24194)
#26 0x00005e557601ba15 clang::Parser::ParseExpression(clang::TypoCorrectionTypeBehavior) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc25a15)
#27 0x00005e5576022b95 clang::Parser::ParseParenExpression(clang::ParenParseOption&, bool, clang::ParenExprKind, clang::TypoCorrectionTypeBehavior, clang::OpaquePtr<clang::QualType>&, clang::SourceLocation&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc2cb95)
#28 0x00005e55760172da clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypoCorrectionTypeBehavior, bool, bool*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc212da)
#29 0x00005e557601a0d1 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc240d1)
#30 0x00005e55760171af clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypoCorrectionTypeBehavior, bool, bool*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc211af)
#31 0x00005e557601a0d1 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc240d1)
#32 0x00005e557601a194 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc24194)
#33 0x00005e557601a6cd clang::Parser::ParseExpressionList(llvm::SmallVectorImpl<clang::Expr*>&, llvm::function_ref<void ()>, bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc246cd)
#34 0x00005e557601c90e clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult<clang::Expr*, true>) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc2690e)
#35 0x00005e557601692b clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypoCorrectionTypeBehavior, bool, bool*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc2092b)
#36 0x00005e557601a0d1 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc240d1)
#37 0x00005e557601a194 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc24194)
#38 0x00005e557601ba15 clang::Parser::ParseExpression(clang::TypoCorrectionTypeBehavior) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc25a15)
#39 0x00005e55760d4257 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xccde257)
#40 0x00005e55760d6487 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcce0487)
#41 0x00005e55760d7099 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcce1099)
#42 0x00005e55760d8e9e clang::Parser::ParseCompoundStatementBody(bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcce2e9e)
#43 0x00005e55760d98c2 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcce38c2)
#44 0x00005e5575f7d337 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb87337)
#45 0x00005e5575fd6d18 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcbe0d18)
#46 0x00005e5575f73307 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb7d307)
#47 0x00005e5575f73e9e clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb7de9e)
#48 0x00005e5575f7ff98 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb89f98)
#49 0x00005e5575f8159d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb8b59d)
#50 0x00005e5575f679d0 clang::ParseAST(clang::Sema&, bool, bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb719d0)
#51 0x00005e5573caf92b clang::ASTFrontendAction::ExecuteAction() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa8b992b)
#52 0x00005e557389d360 clang::CodeGenAction::ExecuteAction() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa4a7360)
#53 0x00005e5573cb79cf clang::FrontendAction::Execute() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa8c19cf)
#54 0x00005e5573bfdf46 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa807f46)
#55 0x00005e5573e3ff7b clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xaa49f7b)
#56 0x00005e556dbae523 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47b8523)
#57 0x00005e556db9ec58 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#58 0x00005e556dba4b28 clang_main(int, char**, llvm::ToolContext const&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47aeb28)
#59 0x00005e556d9ffb4a main (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x4609b4a)
#60 0x000072371d4ed1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#61 0x000072371d4ed28b call_init ./csu/../csu/libc-start.c:128:20
#62 0x000072371d4ed28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#63 0x00005e556db9cf55 _start (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47a6f55)
clang++: error: unable to execute command: Aborted (core dumped)
```

## The Clang version:
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

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (AbyssStaror)

<details>
### Link for quick verification: https://godbolt.org/z/7a85Kchsv
```c
#include &lt;stdio.h&gt;

union u_type {
    int inner;
    int c;
};

void foo(union u_type *u) {}

int main() {
    foo(&amp;((union u_type){ .c = 1, .inner = 2 })); 
}
```
When attempting to initialize multiple members of a union simultaneously, the compiler went crash.

## The stack dump:
```
clang-21: /workspace/install/llvm/src/llvm-project/clang/include/clang/AST/Expr.h:5287: clang::Expr* clang::InitListExpr::getInit(unsigned int): Assertion `Init &lt; getNumInits() &amp;&amp; "Initializer access out of range!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /workspace/install/llvm/build_21.1.4/bin/clang-21 -cc1 -triple x86_64-unknown-linux-gnu -O3 -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -main-file-name 34848.fuzz -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=none -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/workspace/exps -fcoverage-compilation-dir=/workspace/exps -resource-dir /workspace/install/llvm/build_21.1.4/lib/clang/21 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/x86_64-linux-gnu/c++/13 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/backward -internal-isystem /workspace/install/llvm/build_21.1.4/lib/clang/21/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -std=c++20 -fdeprecated-macro -ferror-limit 19 -fmessage-length=167 -fgnuc-version=4.2.1 -fno-implicit-modules -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -vectorize-loops -vectorize-slp -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/34848-a2cd0b.o -x c++ /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_choose_3_1_exp/cases/34848.fuzz
1.      /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_choose_3_1_exp/cases/34848.fuzz:29:49: current parser token ')'
2.      /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_choose_3_1_exp/cases/34848.fuzz:26:12: parsing function body 'main'
3.      /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_choose_3_1_exp/cases/34848.fuzz:26:12: in compound statement ('{}')
 #<!-- -->0 0x00005e5572a87c2a llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9691c2a)
 #<!-- -->1 0x00005e5572a88457 PrintStackTraceSignalHandler(void*) Signals.cpp:0:0
 #<!-- -->2 0x00005e5572a81f57 llvm::sys::RunSignalHandlers() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x968bf57)
 #<!-- -->3 0x00005e5572a821c7 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->4 0x000072371d508330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->5 0x000072371d561b2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->6 0x000072371d561b2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->7 0x000072371d561b2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->8 0x000072371d50827e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x000072371d4eb8ff abort ./stdlib/abort.c:81:7
#<!-- -->10 0x000072371d4eb81b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->11 0x000072371d4fe517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->12 0x00005e557305851c clang::InitListExpr::getInit(unsigned int) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9c6251c)
#<!-- -->13 0x00005e557693bfe5 (anonymous namespace)::InitListChecker::CheckDesignatedInitializer(clang::InitializedEntity const&amp;, clang::InitListExpr*, clang::DesignatedInitExpr*, unsigned int, clang::QualType&amp;, clang::DeclContext::specific_decl_iterator&lt;clang::FieldDecl&gt;*, llvm::APSInt*, unsigned int&amp;, clang::InitListExpr*, unsigned int&amp;, bool, bool) (.constprop.0) SemaInit.cpp:0:0
#<!-- -->14 0x00005e557693cbc6 (anonymous namespace)::InitListChecker::CheckStructUnionTypes(clang::InitializedEntity const&amp;, clang::InitListExpr*, clang::QualType, llvm::iterator_range&lt;clang::CXXBaseSpecifier const*&gt;, clang::DeclContext::specific_decl_iterator&lt;clang::FieldDecl&gt;, bool, unsigned int&amp;, clang::InitListExpr*, unsigned int&amp;, bool) SemaInit.cpp:0:0
#<!-- -->15 0x00005e557693f25b (anonymous namespace)::InitListChecker::CheckListElementTypes(clang::InitializedEntity const&amp;, clang::InitListExpr*, clang::QualType&amp;, bool, unsigned int&amp;, clang::InitListExpr*, unsigned int&amp;, bool) (.constprop.0) SemaInit.cpp:0:0
#<!-- -->16 0x00005e5576940475 (anonymous namespace)::InitListChecker::CheckExplicitInitList(clang::InitializedEntity const&amp;, clang::InitListExpr*, clang::QualType&amp;, clang::InitListExpr*, bool) (.constprop.0) SemaInit.cpp:0:0
#<!-- -->17 0x00005e5576925b7d (anonymous namespace)::InitListChecker::InitListChecker(clang::Sema&amp;, clang::InitializedEntity const&amp;, clang::InitListExpr*, clang::QualType&amp;, bool, bool, bool, llvm::SmallVectorImpl&lt;clang::QualType&gt;*) SemaInit.cpp:0:0
#<!-- -->18 0x00005e557692b069 clang::InitializationSequence::Perform(clang::Sema&amp;, clang::InitializedEntity const&amp;, clang::InitializationKind const&amp;, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::QualType*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xd535069)
#<!-- -->19 0x00005e55766dff65 clang::Sema::BuildCompoundLiteralExpr(clang::SourceLocation, clang::TypeSourceInfo*, clang::SourceLocation, clang::Expr*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xd2e9f65)
#<!-- -->20 0x00005e55766e0e01 clang::Sema::ActOnCompoundLiteral(clang::SourceLocation, clang::OpaquePtr&lt;clang::QualType&gt;, clang::SourceLocation, clang::Expr*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xd2eae01)
#<!-- -->21 0x00005e5576021fa0 clang::Parser::ParseCompoundLiteralExpression(clang::OpaquePtr&lt;clang::QualType&gt;, clang::SourceLocation, clang::SourceLocation) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc2bfa0)
#<!-- -->22 0x00005e5576023fa0 clang::Parser::ParseParenExpression(clang::ParenParseOption&amp;, bool, clang::ParenExprKind, clang::TypoCorrectionTypeBehavior, clang::OpaquePtr&lt;clang::QualType&gt;&amp;, clang::SourceLocation&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc2dfa0)
#<!-- -->23 0x00005e55760172da clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&amp;, clang::TypoCorrectionTypeBehavior, bool, bool*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc212da)
#<!-- -->24 0x00005e557601a0d1 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc240d1)
#<!-- -->25 0x00005e557601a194 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc24194)
#<!-- -->26 0x00005e557601ba15 clang::Parser::ParseExpression(clang::TypoCorrectionTypeBehavior) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc25a15)
#<!-- -->27 0x00005e5576022b95 clang::Parser::ParseParenExpression(clang::ParenParseOption&amp;, bool, clang::ParenExprKind, clang::TypoCorrectionTypeBehavior, clang::OpaquePtr&lt;clang::QualType&gt;&amp;, clang::SourceLocation&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc2cb95)
#<!-- -->28 0x00005e55760172da clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&amp;, clang::TypoCorrectionTypeBehavior, bool, bool*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc212da)
#<!-- -->29 0x00005e557601a0d1 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc240d1)
#<!-- -->30 0x00005e55760171af clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&amp;, clang::TypoCorrectionTypeBehavior, bool, bool*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc211af)
#<!-- -->31 0x00005e557601a0d1 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc240d1)
#<!-- -->32 0x00005e557601a194 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc24194)
#<!-- -->33 0x00005e557601a6cd clang::Parser::ParseExpressionList(llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, llvm::function_ref&lt;void ()&gt;, bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc246cd)
#<!-- -->34 0x00005e557601c90e clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult&lt;clang::Expr*, true&gt;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc2690e)
#<!-- -->35 0x00005e557601692b clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&amp;, clang::TypoCorrectionTypeBehavior, bool, bool*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc2092b)
#<!-- -->36 0x00005e557601a0d1 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc240d1)
#<!-- -->37 0x00005e557601a194 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc24194)
#<!-- -->38 0x00005e557601ba15 clang::Parser::ParseExpression(clang::TypoCorrectionTypeBehavior) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc25a15)
#<!-- -->39 0x00005e55760d4257 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xccde257)
#<!-- -->40 0x00005e55760d6487 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcce0487)
#<!-- -->41 0x00005e55760d7099 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcce1099)
#<!-- -->42 0x00005e55760d8e9e clang::Parser::ParseCompoundStatementBody(bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcce2e9e)
#<!-- -->43 0x00005e55760d98c2 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcce38c2)
#<!-- -->44 0x00005e5575f7d337 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb87337)
#<!-- -->45 0x00005e5575fd6d18 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcbe0d18)
#<!-- -->46 0x00005e5575f73307 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb7d307)
#<!-- -->47 0x00005e5575f73e9e clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb7de9e)
#<!-- -->48 0x00005e5575f7ff98 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb89f98)
#<!-- -->49 0x00005e5575f8159d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb8b59d)
#<!-- -->50 0x00005e5575f679d0 clang::ParseAST(clang::Sema&amp;, bool, bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb719d0)
#<!-- -->51 0x00005e5573caf92b clang::ASTFrontendAction::ExecuteAction() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa8b992b)
#<!-- -->52 0x00005e557389d360 clang::CodeGenAction::ExecuteAction() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa4a7360)
#<!-- -->53 0x00005e5573cb79cf clang::FrontendAction::Execute() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa8c19cf)
#<!-- -->54 0x00005e5573bfdf46 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa807f46)
#<!-- -->55 0x00005e5573e3ff7b clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xaa49f7b)
#<!-- -->56 0x00005e556dbae523 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47b8523)
#<!-- -->57 0x00005e556db9ec58 ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->58 0x00005e556dba4b28 clang_main(int, char**, llvm::ToolContext const&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47aeb28)
#<!-- -->59 0x00005e556d9ffb4a main (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x4609b4a)
#<!-- -->60 0x000072371d4ed1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->61 0x000072371d4ed28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->62 0x000072371d4ed28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->63 0x00005e556db9cf55 _start (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47a6f55)
clang++: error: unable to execute command: Aborted (core dumped)
```

## The Clang version:
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

### Comment 2 - shafik

The backtrace is not identical but the diagnostics are and they both go back to clang-17, so likely the same root cause. If we discover the fix for the dup does not fix this we can reopen.

Again, checking for duplicates w/ fuzzing output is required.

---

### Comment 3 - AbyssStaror

> The backtrace is not identical but the diagnostics are and they both go back to clang-17, so likely the same root cause. If we discover the fix for the dup does not fix this we can reopen.
> 
> Again, checking for duplicates w/ fuzzing output is required.

Thanks for pointing this out, and my apologies for the duplicate submission. I'll make sure to carefully verify the outputs before submitting any future bug reports.


---

