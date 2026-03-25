# Clang crashes with UNREACHABLE when applying __builtin_fabs to vector type

**Author:** AbyssStaror
**URL:** https://github.com/llvm/llvm-project/issues/170466

## Body

### Link for quick verification: https://godbolt.org/z/KT17hMdMW
```c
typedef double V __attribute__((vector_size (8)));

V foo(V x) {
    return __builtin_fabs(x);
}
```
When compiling code that applies `__builtin_fabs` to a vector type containing only one element (`vector_size(8) with double`), Clang crashes with an `UNREACHABLE` execution in `SemaChecking.cpp`.

## Stack dump:
```
Type not integer, floating, or complex
UNREACHABLE executed at /workspace/install/llvm/src/llvm-project/clang/lib/Sema/SemaChecking.cpp:9110!
Stack dump:
0.      Program arguments: /workspace/install/llvm/build_21.1.4/bin/clang-21 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -main-file-name 12222.fuzz -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/workspace/exps -fcoverage-compilation-dir=/workspace/exps -resource-dir /workspace/install/llvm/build_21.1.4/lib/clang/21 -internal-isystem /workspace/install/llvm/build_21.1.4/lib/clang/21/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -ferror-limit 19 -fmessage-length=137 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/12222-ad45f9.o -x c /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_1_exp/cases/12222.fuzz
1.      /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_1_exp/cases/12222.fuzz:6:28: current parser token ')'
2.      /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_1_exp/cases/12222.fuzz:5:12: parsing function body 'foo'
3.      /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_1_exp/cases/12222.fuzz:5:12: in compound statement ('{}')
 #0 0x00005aa65fcbfc2a llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9691c2a)
 #1 0x00005aa65fcc0457 PrintStackTraceSignalHandler(void*) Signals.cpp:0:0
 #2 0x00005aa65fcb9f57 llvm::sys::RunSignalHandlers() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x968bf57)
 #3 0x00005aa65fcba1c7 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #4 0x000079681b057330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #5 0x000079681b0b0b2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #6 0x000079681b0b0b2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #7 0x000079681b0b0b2c pthread_kill ./nptl/pthread_kill.c:89:10
 #8 0x000079681b05727e raise ./signal/../sysdeps/posix/raise.c:27:6
 #9 0x000079681b03a8ff abort ./stdlib/abort.c:81:7
#10 0x00005aa65fbb76de llvm::llvm_unreachable_internal(char const*, char const*, unsigned int) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x95896de)
#11 0x00005aa6634ab659 (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xce7d659)
#12 0x00005aa6634e0e77 clang::Sema::CheckAbsoluteValueFunction(clang::CallExpr const*, clang::FunctionDecl const*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xceb2e77)
#13 0x00005aa663521707 clang::Sema::CheckFunctionCall(clang::FunctionDecl*, clang::CallExpr*, clang::FunctionProtoType const*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcef3707)
#14 0x00005aa663a7403d clang::Sema::BuildResolvedCallExpr(clang::Expr*, clang::NamedDecl*, clang::SourceLocation, llvm::ArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, clang::CallExpr::ADLCallKind) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xd44603d)
#15 0x00005aa66393bf20 clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xd30df20)
#16 0x00005aa66393dace clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xd30face)
#17 0x00005aa66325419d clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult<clang::Expr*, true>) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc2619d)
#18 0x00005aa66324e92b clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypoCorrectionTypeBehavior, bool, bool*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc2092b)
#19 0x00005aa6632520d1 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc240d1)
#20 0x00005aa663252194 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc24194)
#21 0x00005aa663253a15 clang::Parser::ParseExpression(clang::TypoCorrectionTypeBehavior) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc25a15)
#22 0x00005aa663306b00 clang::Parser::ParseReturnStatement() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xccd8b00)
#23 0x00005aa66330d879 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xccdf879)
#24 0x00005aa66330f099 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcce1099)
#25 0x00005aa663310e9e clang::Parser::ParseCompoundStatementBody(bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcce2e9e)
#26 0x00005aa6633118c2 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcce38c2)
#27 0x00005aa6631b5337 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb87337)
#28 0x00005aa66320ed18 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcbe0d18)
#29 0x00005aa6631ab307 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb7d307)
#30 0x00005aa6631abe9e clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb7de9e)
#31 0x00005aa6631b7f98 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb89f98)
#32 0x00005aa6631b959d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb8b59d)
#33 0x00005aa66319f9d0 clang::ParseAST(clang::Sema&, bool, bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb719d0)
#34 0x00005aa660ee792b clang::ASTFrontendAction::ExecuteAction() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa8b992b)
#35 0x00005aa660ad5360 clang::CodeGenAction::ExecuteAction() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa4a7360)
#36 0x00005aa660eef9cf clang::FrontendAction::Execute() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa8c19cf)
#37 0x00005aa660e35f46 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa807f46)
#38 0x00005aa661077f7b clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xaa49f7b)
#39 0x00005aa65ade6523 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47b8523)
#40 0x00005aa65add6c58 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#41 0x00005aa65addcb28 clang_main(int, char**, llvm::ToolContext const&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47aeb28)
#42 0x00005aa65ac37b4a main (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x4609b4a)
#43 0x000079681b03c1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#44 0x000079681b03c28b call_init ./csu/../csu/libc-start.c:128:20
#45 0x000079681b03c28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#46 0x00005aa65add4f55 _start (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47a6f55)
clang: error: unable to execute command: Aborted (core dumped)
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

Started to crash since Clang 3.9. [Godbolt link](https://godbolt.org/z/MWs5TWzWM).

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (AbyssStaror)

<details>
### Link for quick verification: https://godbolt.org/z/KT17hMdMW
```c
typedef double V __attribute__((vector_size (8)));

V foo(V x) {
    return __builtin_fabs(x);
}
```
When compiling code that applies `__builtin_fabs` to a vector type containing only one element (`vector_size(8) with double`), Clang crashes with an `UNREACHABLE` execution in `SemaChecking.cpp`.

## Stack dump:
```
Type not integer, floating, or complex
UNREACHABLE executed at /workspace/install/llvm/src/llvm-project/clang/lib/Sema/SemaChecking.cpp:9110!
Stack dump:
0.      Program arguments: /workspace/install/llvm/build_21.1.4/bin/clang-21 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -main-file-name 12222.fuzz -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/workspace/exps -fcoverage-compilation-dir=/workspace/exps -resource-dir /workspace/install/llvm/build_21.1.4/lib/clang/21 -internal-isystem /workspace/install/llvm/build_21.1.4/lib/clang/21/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -ferror-limit 19 -fmessage-length=137 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/12222-ad45f9.o -x c /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_1_exp/cases/12222.fuzz
1.      /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_1_exp/cases/12222.fuzz:6:28: current parser token ')'
2.      /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_1_exp/cases/12222.fuzz:5:12: parsing function body 'foo'
3.      /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_1_exp/cases/12222.fuzz:5:12: in compound statement ('{}')
 #<!-- -->0 0x00005aa65fcbfc2a llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9691c2a)
 #<!-- -->1 0x00005aa65fcc0457 PrintStackTraceSignalHandler(void*) Signals.cpp:0:0
 #<!-- -->2 0x00005aa65fcb9f57 llvm::sys::RunSignalHandlers() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x968bf57)
 #<!-- -->3 0x00005aa65fcba1c7 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->4 0x000079681b057330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->5 0x000079681b0b0b2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->6 0x000079681b0b0b2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->7 0x000079681b0b0b2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->8 0x000079681b05727e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x000079681b03a8ff abort ./stdlib/abort.c:81:7
#<!-- -->10 0x00005aa65fbb76de llvm::llvm_unreachable_internal(char const*, char const*, unsigned int) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x95896de)
#<!-- -->11 0x00005aa6634ab659 (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xce7d659)
#<!-- -->12 0x00005aa6634e0e77 clang::Sema::CheckAbsoluteValueFunction(clang::CallExpr const*, clang::FunctionDecl const*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xceb2e77)
#<!-- -->13 0x00005aa663521707 clang::Sema::CheckFunctionCall(clang::FunctionDecl*, clang::CallExpr*, clang::FunctionProtoType const*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcef3707)
#<!-- -->14 0x00005aa663a7403d clang::Sema::BuildResolvedCallExpr(clang::Expr*, clang::NamedDecl*, clang::SourceLocation, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, clang::CallExpr::ADLCallKind) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xd44603d)
#<!-- -->15 0x00005aa66393bf20 clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xd30df20)
#<!-- -->16 0x00005aa66393dace clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xd30face)
#<!-- -->17 0x00005aa66325419d clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult&lt;clang::Expr*, true&gt;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc2619d)
#<!-- -->18 0x00005aa66324e92b clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&amp;, clang::TypoCorrectionTypeBehavior, bool, bool*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc2092b)
#<!-- -->19 0x00005aa6632520d1 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc240d1)
#<!-- -->20 0x00005aa663252194 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc24194)
#<!-- -->21 0x00005aa663253a15 clang::Parser::ParseExpression(clang::TypoCorrectionTypeBehavior) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc25a15)
#<!-- -->22 0x00005aa663306b00 clang::Parser::ParseReturnStatement() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xccd8b00)
#<!-- -->23 0x00005aa66330d879 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xccdf879)
#<!-- -->24 0x00005aa66330f099 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcce1099)
#<!-- -->25 0x00005aa663310e9e clang::Parser::ParseCompoundStatementBody(bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcce2e9e)
#<!-- -->26 0x00005aa6633118c2 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcce38c2)
#<!-- -->27 0x00005aa6631b5337 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb87337)
#<!-- -->28 0x00005aa66320ed18 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcbe0d18)
#<!-- -->29 0x00005aa6631ab307 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb7d307)
#<!-- -->30 0x00005aa6631abe9e clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb7de9e)
#<!-- -->31 0x00005aa6631b7f98 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb89f98)
#<!-- -->32 0x00005aa6631b959d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb8b59d)
#<!-- -->33 0x00005aa66319f9d0 clang::ParseAST(clang::Sema&amp;, bool, bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb719d0)
#<!-- -->34 0x00005aa660ee792b clang::ASTFrontendAction::ExecuteAction() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa8b992b)
#<!-- -->35 0x00005aa660ad5360 clang::CodeGenAction::ExecuteAction() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa4a7360)
#<!-- -->36 0x00005aa660eef9cf clang::FrontendAction::Execute() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa8c19cf)
#<!-- -->37 0x00005aa660e35f46 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa807f46)
#<!-- -->38 0x00005aa661077f7b clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xaa49f7b)
#<!-- -->39 0x00005aa65ade6523 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47b8523)
#<!-- -->40 0x00005aa65add6c58 ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->41 0x00005aa65addcb28 clang_main(int, char**, llvm::ToolContext const&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47aeb28)
#<!-- -->42 0x00005aa65ac37b4a main (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x4609b4a)
#<!-- -->43 0x000079681b03c1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->44 0x000079681b03c28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->45 0x000079681b03c28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->46 0x00005aa65add4f55 _start (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47a6f55)
clang: error: unable to execute command: Aborted (core dumped)
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

Are these bugs generated via fuzzing? If so please, state this in the description, if you can label then also label it so. Also we need you to specify how far back the bug goes, so we can see if this is a recent regression or not.

---

### Comment 4 - AbyssStaror

relate to https://github.com/llvm/llvm-project/issues/96627 https://github.com/llvm/llvm-project/issues/111304

---

