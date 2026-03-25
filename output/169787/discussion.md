# clang: trunk: Assertion `Result.isAbsent() && "local created multiple times"' failed in ExprConstant.cpp

**Author:** AbyssStaror
**URL:** https://github.com/llvm/llvm-project/issues/169787

## Body

### Link for quick verification: https://godbolt.org/z/TdGEsnn68
```c
int main() {
    static int large_array[1024] = { [0 ... 1023] = (int[]){1, 2}[0] }; 

    return large_array[512]; 
}
```
## The stack dump is :
```
clang-21: /workspace/install/llvm/src/llvm-project/clang/lib/AST/ExprConstant.cpp:1976: clang::APValue& {anonymous}::CallStackFrame::createLocal(clang::APValue::LValueBase, const void*, clang::QualType, {anonymous}::ScopeKind): Assertion `Result.isAbsent() && "local created multiple times"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /workspace/install/llvm/build_21.1.4/bin/clang-21 -cc1 -triple x86_64-unknown-linux-gnu -O3 -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -main-file-name 3948.fuzz -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=none -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/workspace/exps -fcoverage-compilation-dir=/workspace/exps -resource-dir /workspace/install/llvm/build_21.1.4/lib/clang/21 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/x86_64-linux-gnu/c++/13 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/backward -internal-isystem /workspace/install/llvm/build_21.1.4/lib/clang/21/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -fdeprecated-macro -ferror-limit 19 -fmessage-length=138 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -vectorize-loops -vectorize-slp -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/3948-e06a63.o -x c++ /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_choose_3_1_exp/cases/3948.fuzz
1.      /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_choose_3_1_exp/cases/3948.fuzz:9:71: current parser token ';'
2.      /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_choose_3_1_exp/cases/3948.fuzz:8:12: parsing function body 'main'
3.      /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_choose_3_1_exp/cases/3948.fuzz:8:12: in compound statement ('{}')
 #0 0x00006210fabfcc2a llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9691c2a)
 #1 0x00006210fabfd457 PrintStackTraceSignalHandler(void*) Signals.cpp:0:0
 #2 0x00006210fabf6f57 llvm::sys::RunSignalHandlers() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x968bf57)
 #3 0x00006210fabf71c7 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #4 0x000079a06ddcb330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #5 0x000079a06de24b2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #6 0x000079a06de24b2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #7 0x000079a06de24b2c pthread_kill ./nptl/pthread_kill.c:89:10
 #8 0x000079a06ddcb27e raise ./signal/../sysdeps/posix/raise.c:27:6
 #9 0x000079a06ddae8ff abort ./stdlib/abort.c:81:7
#10 0x000079a06ddae81b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#11 0x000079a06ddc1517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#12 0x00006210ffccd1e0 (anonymous namespace)::CallStackFrame::createLocal(clang::APValue::LValueBase, void const*, clang::QualType, (anonymous namespace)::ScopeKind) ExprConstant.cpp:0:0
#13 0x00006210ffd617bd (anonymous namespace)::LValueExprEvaluator::VisitMaterializeTemporaryExpr(clang::MaterializeTemporaryExpr const*) ExprConstant.cpp:0:0
#14 0x00006210ffcf01d8 clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::LValueExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#15 0x00006210ffcf3949 EvaluateLValue(clang::Expr const*, (anonymous namespace)::LValue&, (anonymous namespace)::EvalInfo&, bool) ExprConstant.cpp:0:0
#16 0x00006210ffcf8aa2 (anonymous namespace)::PointerExprEvaluator::VisitCastExpr(clang::CastExpr const*) ExprConstant.cpp:0:0
#17 0x00006210ffcfa8ef clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::PointerExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#18 0x00006210ffcfd449 EvaluatePointer(clang::Expr const*, (anonymous namespace)::LValue&, (anonymous namespace)::EvalInfo&, bool) ExprConstant.cpp:0:0
#19 0x00006210ffd311ac (anonymous namespace)::LValueExprEvaluator::VisitArraySubscriptExpr(clang::ArraySubscriptExpr const*) ExprConstant.cpp:0:0
#20 0x00006210ffcf08ca clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::LValueExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#21 0x00006210ffcf3949 EvaluateLValue(clang::Expr const*, (anonymous namespace)::LValue&, (anonymous namespace)::EvalInfo&, bool) ExprConstant.cpp:0:0
#22 0x00006210ffd9f733 (anonymous namespace)::IntExprEvaluator::VisitCastExpr(clang::CastExpr const*) ExprConstant.cpp:0:0
#23 0x00006210ffd0a19b clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#24 0x00006210ffd1b627 Evaluate(clang::APValue&, (anonymous namespace)::EvalInfo&, clang::Expr const*) ExprConstant.cpp:0:0
#25 0x00006210ffd4138b EvaluateInPlace(clang::APValue&, (anonymous namespace)::EvalInfo&, (anonymous namespace)::LValue const&, clang::Expr const*, bool) ExprConstant.cpp:0:0
#26 0x00006210ffd4316c (anonymous namespace)::ArrayExprEvaluator::VisitCXXParenListOrInitListExpr(clang::Expr const*, llvm::ArrayRef<clang::Expr*>, clang::Expr const*, clang::QualType) ExprConstant.cpp:0:0
#27 0x00006210ffd43bd4 (anonymous namespace)::ArrayExprEvaluator::VisitInitListExpr(clang::InitListExpr const*, clang::QualType) ExprConstant.cpp:0:0
#28 0x00006210ffd91b1e clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::ArrayExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#29 0x00006210ffd932b2 clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::ArrayExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#30 0x00006210ffd95934 EvaluateArray(clang::Expr const*, (anonymous namespace)::LValue const&, clang::APValue&, (anonymous namespace)::EvalInfo&) ExprConstant.cpp:0:0
#31 0x00006210ffd414a9 EvaluateInPlace(clang::APValue&, (anonymous namespace)::EvalInfo&, (anonymous namespace)::LValue const&, clang::Expr const*, bool) ExprConstant.cpp:0:0
#32 0x00006210ffd4f9a4 clang::Expr::EvaluateAsInitializer(clang::APValue&, clang::ASTContext const&, clang::VarDecl const*, llvm::SmallVectorImpl<std::pair<clang::SourceLocation, clang::PartialDiagnostic>>&, bool) const (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xe7e49a4)
#33 0x00006210ffa42a6e clang::VarDecl::evaluateValueImpl(llvm::SmallVectorImpl<std::pair<clang::SourceLocation, clang::PartialDiagnostic>>&, bool) const (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xe4d7a6e)
#34 0x00006210ffa42ca9 clang::VarDecl::checkForConstantInitialization(llvm::SmallVectorImpl<std::pair<clang::SourceLocation, clang::PartialDiagnostic>>&) const (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xe4d7ca9)
#35 0x00006210fe60d53b clang::Sema::CheckCompleteVariableDeclaration(clang::VarDecl*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xd0a253b)
#36 0x00006210fe655d6c clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xd0ead6c)
#37 0x00006210fe129a32 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcbbea32)
#38 0x00006210fe14ad1b clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcbdfd1b)
#39 0x00006210fe14ca1f clang::Parser::ParseSimpleDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, bool, clang::Parser::ForRangeInit*, clang::SourceLocation*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcbe1a1f)
#40 0x00006210fe14d069 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcbe2069)
#41 0x00006210fe24b46c clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcce046c)
#42 0x00006210fe24c099 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcce1099)
#43 0x00006210fe24de9e clang::Parser::ParseCompoundStatementBody(bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcce2e9e)
#44 0x00006210fe24e8c2 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcce38c2)
#45 0x00006210fe0f2337 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb87337)
#46 0x00006210fe14bd18 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcbe0d18)
#47 0x00006210fe0e8307 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb7d307)
#48 0x00006210fe0e8e9e clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb7de9e)
#49 0x00006210fe0f4f98 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb89f98)
#50 0x00006210fe0f659d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb8b59d)
#51 0x00006210fe0dc9d0 clang::ParseAST(clang::Sema&, bool, bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb719d0)
#52 0x00006210fbe2492b clang::ASTFrontendAction::ExecuteAction() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa8b992b)
#53 0x00006210fba12360 clang::CodeGenAction::ExecuteAction() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa4a7360)
#54 0x00006210fbe2c9cf clang::FrontendAction::Execute() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa8c19cf)
#55 0x00006210fbd72f46 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa807f46)
#56 0x00006210fbfb4f7b clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xaa49f7b)
#57 0x00006210f5d23523 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47b8523)
#58 0x00006210f5d13c58 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#59 0x00006210f5d19b28 clang_main(int, char**, llvm::ToolContext const&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47aeb28)
#60 0x00006210f5b74b4a main (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x4609b4a)
#61 0x000079a06ddb01ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#62 0x000079a06ddb028b call_init ./csu/../csu/libc-start.c:128:20
#63 0x000079a06ddb028b __libc_start_main ./csu/../csu/libc-start.c:347:5
#64 0x00006210f5d11f55 _start (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47a6f55)
clang++: error: unable to execute command: Aborted (core dumped)
```

## The version:
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

Also crashes in C mode [Godbolt link](https://godbolt.org/z/93vvr658Y).
```C
int main() {
  constexpr int a[2] = { [0 ... 1] = (const int){0} };
}
```

I _think_ this is https://github.com/llvm/llvm-project/labels/crash-on-valid although it uses an GNU extension.

Also reproducible with a small array. [Godbolt link](https://godbolt.org/z/93vvr658Y).
```C++
int main() {
  static int large_array[2] = { [0 ... 1] = (const int[]){1}[0] };
}
```

[Used to work in Clang 3.1](https://godbolt.org/z/54dYdcdco) but crashed later.

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (AbyssStaror)

<details>
### Link for quick verification: https://godbolt.org/z/TdGEsnn68
```c
int main() {
    static int large_array[1024] = { [0 ... 1023] = (int[]){1, 2}[0] }; 

    return large_array[512]; 
}
```
## The stack dump is :
```
clang-21: /workspace/install/llvm/src/llvm-project/clang/lib/AST/ExprConstant.cpp:1976: clang::APValue&amp; {anonymous}::CallStackFrame::createLocal(clang::APValue::LValueBase, const void*, clang::QualType, {anonymous}::ScopeKind): Assertion `Result.isAbsent() &amp;&amp; "local created multiple times"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /workspace/install/llvm/build_21.1.4/bin/clang-21 -cc1 -triple x86_64-unknown-linux-gnu -O3 -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -main-file-name 3948.fuzz -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=none -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/workspace/exps -fcoverage-compilation-dir=/workspace/exps -resource-dir /workspace/install/llvm/build_21.1.4/lib/clang/21 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/x86_64-linux-gnu/c++/13 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/backward -internal-isystem /workspace/install/llvm/build_21.1.4/lib/clang/21/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -fdeprecated-macro -ferror-limit 19 -fmessage-length=138 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -vectorize-loops -vectorize-slp -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/3948-e06a63.o -x c++ /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_choose_3_1_exp/cases/3948.fuzz
1.      /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_choose_3_1_exp/cases/3948.fuzz:9:71: current parser token ';'
2.      /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_choose_3_1_exp/cases/3948.fuzz:8:12: parsing function body 'main'
3.      /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_choose_3_1_exp/cases/3948.fuzz:8:12: in compound statement ('{}')
 #<!-- -->0 0x00006210fabfcc2a llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9691c2a)
 #<!-- -->1 0x00006210fabfd457 PrintStackTraceSignalHandler(void*) Signals.cpp:0:0
 #<!-- -->2 0x00006210fabf6f57 llvm::sys::RunSignalHandlers() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x968bf57)
 #<!-- -->3 0x00006210fabf71c7 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->4 0x000079a06ddcb330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->5 0x000079a06de24b2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->6 0x000079a06de24b2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->7 0x000079a06de24b2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->8 0x000079a06ddcb27e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x000079a06ddae8ff abort ./stdlib/abort.c:81:7
#<!-- -->10 0x000079a06ddae81b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->11 0x000079a06ddc1517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->12 0x00006210ffccd1e0 (anonymous namespace)::CallStackFrame::createLocal(clang::APValue::LValueBase, void const*, clang::QualType, (anonymous namespace)::ScopeKind) ExprConstant.cpp:0:0
#<!-- -->13 0x00006210ffd617bd (anonymous namespace)::LValueExprEvaluator::VisitMaterializeTemporaryExpr(clang::MaterializeTemporaryExpr const*) ExprConstant.cpp:0:0
#<!-- -->14 0x00006210ffcf01d8 clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::LValueExprEvaluator, bool&gt;::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#<!-- -->15 0x00006210ffcf3949 EvaluateLValue(clang::Expr const*, (anonymous namespace)::LValue&amp;, (anonymous namespace)::EvalInfo&amp;, bool) ExprConstant.cpp:0:0
#<!-- -->16 0x00006210ffcf8aa2 (anonymous namespace)::PointerExprEvaluator::VisitCastExpr(clang::CastExpr const*) ExprConstant.cpp:0:0
#<!-- -->17 0x00006210ffcfa8ef clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::PointerExprEvaluator, bool&gt;::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#<!-- -->18 0x00006210ffcfd449 EvaluatePointer(clang::Expr const*, (anonymous namespace)::LValue&amp;, (anonymous namespace)::EvalInfo&amp;, bool) ExprConstant.cpp:0:0
#<!-- -->19 0x00006210ffd311ac (anonymous namespace)::LValueExprEvaluator::VisitArraySubscriptExpr(clang::ArraySubscriptExpr const*) ExprConstant.cpp:0:0
#<!-- -->20 0x00006210ffcf08ca clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::LValueExprEvaluator, bool&gt;::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#<!-- -->21 0x00006210ffcf3949 EvaluateLValue(clang::Expr const*, (anonymous namespace)::LValue&amp;, (anonymous namespace)::EvalInfo&amp;, bool) ExprConstant.cpp:0:0
#<!-- -->22 0x00006210ffd9f733 (anonymous namespace)::IntExprEvaluator::VisitCastExpr(clang::CastExpr const*) ExprConstant.cpp:0:0
#<!-- -->23 0x00006210ffd0a19b clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool&gt;::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#<!-- -->24 0x00006210ffd1b627 Evaluate(clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Expr const*) ExprConstant.cpp:0:0
#<!-- -->25 0x00006210ffd4138b EvaluateInPlace(clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;, (anonymous namespace)::LValue const&amp;, clang::Expr const*, bool) ExprConstant.cpp:0:0
#<!-- -->26 0x00006210ffd4316c (anonymous namespace)::ArrayExprEvaluator::VisitCXXParenListOrInitListExpr(clang::Expr const*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::Expr const*, clang::QualType) ExprConstant.cpp:0:0
#<!-- -->27 0x00006210ffd43bd4 (anonymous namespace)::ArrayExprEvaluator::VisitInitListExpr(clang::InitListExpr const*, clang::QualType) ExprConstant.cpp:0:0
#<!-- -->28 0x00006210ffd91b1e clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::ArrayExprEvaluator, bool&gt;::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#<!-- -->29 0x00006210ffd932b2 clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::ArrayExprEvaluator, bool&gt;::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#<!-- -->30 0x00006210ffd95934 EvaluateArray(clang::Expr const*, (anonymous namespace)::LValue const&amp;, clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;) ExprConstant.cpp:0:0
#<!-- -->31 0x00006210ffd414a9 EvaluateInPlace(clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;, (anonymous namespace)::LValue const&amp;, clang::Expr const*, bool) ExprConstant.cpp:0:0
#<!-- -->32 0x00006210ffd4f9a4 clang::Expr::EvaluateAsInitializer(clang::APValue&amp;, clang::ASTContext const&amp;, clang::VarDecl const*, llvm::SmallVectorImpl&lt;std::pair&lt;clang::SourceLocation, clang::PartialDiagnostic&gt;&gt;&amp;, bool) const (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xe7e49a4)
#<!-- -->33 0x00006210ffa42a6e clang::VarDecl::evaluateValueImpl(llvm::SmallVectorImpl&lt;std::pair&lt;clang::SourceLocation, clang::PartialDiagnostic&gt;&gt;&amp;, bool) const (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xe4d7a6e)
#<!-- -->34 0x00006210ffa42ca9 clang::VarDecl::checkForConstantInitialization(llvm::SmallVectorImpl&lt;std::pair&lt;clang::SourceLocation, clang::PartialDiagnostic&gt;&gt;&amp;) const (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xe4d7ca9)
#<!-- -->35 0x00006210fe60d53b clang::Sema::CheckCompleteVariableDeclaration(clang::VarDecl*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xd0a253b)
#<!-- -->36 0x00006210fe655d6c clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xd0ead6c)
#<!-- -->37 0x00006210fe129a32 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcbbea32)
#<!-- -->38 0x00006210fe14ad1b clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcbdfd1b)
#<!-- -->39 0x00006210fe14ca1f clang::Parser::ParseSimpleDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, bool, clang::Parser::ForRangeInit*, clang::SourceLocation*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcbe1a1f)
#<!-- -->40 0x00006210fe14d069 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcbe2069)
#<!-- -->41 0x00006210fe24b46c clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcce046c)
#<!-- -->42 0x00006210fe24c099 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcce1099)
#<!-- -->43 0x00006210fe24de9e clang::Parser::ParseCompoundStatementBody(bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcce2e9e)
#<!-- -->44 0x00006210fe24e8c2 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcce38c2)
#<!-- -->45 0x00006210fe0f2337 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb87337)
#<!-- -->46 0x00006210fe14bd18 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcbe0d18)
#<!-- -->47 0x00006210fe0e8307 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb7d307)
#<!-- -->48 0x00006210fe0e8e9e clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb7de9e)
#<!-- -->49 0x00006210fe0f4f98 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb89f98)
#<!-- -->50 0x00006210fe0f659d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb8b59d)
#<!-- -->51 0x00006210fe0dc9d0 clang::ParseAST(clang::Sema&amp;, bool, bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb719d0)
#<!-- -->52 0x00006210fbe2492b clang::ASTFrontendAction::ExecuteAction() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa8b992b)
#<!-- -->53 0x00006210fba12360 clang::CodeGenAction::ExecuteAction() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa4a7360)
#<!-- -->54 0x00006210fbe2c9cf clang::FrontendAction::Execute() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa8c19cf)
#<!-- -->55 0x00006210fbd72f46 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa807f46)
#<!-- -->56 0x00006210fbfb4f7b clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xaa49f7b)
#<!-- -->57 0x00006210f5d23523 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47b8523)
#<!-- -->58 0x00006210f5d13c58 ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->59 0x00006210f5d19b28 clang_main(int, char**, llvm::ToolContext const&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47aeb28)
#<!-- -->60 0x00006210f5b74b4a main (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x4609b4a)
#<!-- -->61 0x000079a06ddb01ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->62 0x000079a06ddb028b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->63 0x000079a06ddb028b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->64 0x00006210f5d11f55 _start (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47a6f55)
clang++: error: unable to execute command: Aborted (core dumped)
```

## The version:
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

