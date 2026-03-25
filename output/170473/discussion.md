# Clang compiler crash when using large unsigned long constant as vector index

**Author:** AbyssStaror
**URL:** https://github.com/llvm/llvm-project/issues/170473
**Status:** Closed
**Labels:** duplicate, clang:codegen, crash
**Closed Date:** 2025-12-04T02:24:53Z

## Body

### Link for quick verification: https://godbolt.org/z/qTGhzsjv6
```c
typedef int v1si __attribute__ ((vector_size (4))); 

v1si test_vector = {1};

int get_last_element() {
    return test_vector[~0UL]; 
}
```
When compiling code that uses `~0UL`(all bits set to 1 for unsigned long) as an index for a vector type, Clang crashes with an assertion failure in `APSInt::getExtValue()`. The crash occurs during constant expression evaluation in the LLVM IR generation phase.
The crash is specific to the extremely large value of` ~0UL`. Using moderately large but still out-of-bounds values like `__INT_MAX__`does not trigger the crash, suggesting the issue is specifically with values that exceed `int64_t` representation.
## Stack dump:
```
clang-21: /workspace/install/llvm/src/llvm-project/llvm/include/llvm/ADT/APSInt.h:100: int64_t llvm::APSInt::getExtValue() const: Assertion `isRepresentableByInt64() && "Too many bits for int64_t"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /workspace/install/llvm/build_21.1.4/bin/clang-21 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -main-file-name 13974.fuzz -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/workspace/exps -fcoverage-compilation-dir=/workspace/exps -resource-dir /workspace/install/llvm/build_21.1.4/lib/clang/21 -internal-isystem /workspace/install/llvm/build_21.1.4/lib/clang/21/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -ferror-limit 19 -fmessage-length=137 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/13974-5bc6b6.o -x c /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_1_exp/cases/13974.fuzz
1.      /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_1_exp/cases/13974.fuzz:9:1: current parser token 'int'
2.      /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_1_exp/cases/13974.fuzz:5:5: LLVM IR generation of declaration 'get_last_element'
3.      /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_1_exp/cases/13974.fuzz:5:5: Generating code for declaration 'get_last_element'
 #0 0x000058527f5d3c2a llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9691c2a)
 #1 0x000058527f5d4457 PrintStackTraceSignalHandler(void*) Signals.cpp:0:0
 #2 0x000058527f5cdf57 llvm::sys::RunSignalHandlers() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x968bf57)
 #3 0x000058527f5ce1c7 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #4 0x00007b8c2bdd5330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #5 0x00007b8c2be2eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #6 0x00007b8c2be2eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #7 0x00007b8c2be2eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #8 0x00007b8c2bdd527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #9 0x00007b8c2bdb88ff abort ./stdlib/abort.c:81:7
#10 0x00007b8c2bdb881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#11 0x00007b8c2bdcb517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#12 0x000058527fc8c2a1 llvm::APSInt::getExtValue() const (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9d4a2a1)
#13 0x00005852847080e6 (anonymous namespace)::LValueExprEvaluator::VisitArraySubscriptExpr(clang::ArraySubscriptExpr const*) ExprConstant.cpp:0:0
#14 0x00005852846c78ca clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::LValueExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#15 0x00005852846ca949 EvaluateLValue(clang::Expr const*, (anonymous namespace)::LValue&, (anonymous namespace)::EvalInfo&, bool) ExprConstant.cpp:0:0
#16 0x0000585284776733 (anonymous namespace)::IntExprEvaluator::VisitCastExpr(clang::CastExpr const*) ExprConstant.cpp:0:0
#17 0x00005852846e119b clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#18 0x00005852846f2627 Evaluate(clang::APValue&, (anonymous namespace)::EvalInfo&, clang::Expr const*) ExprConstant.cpp:0:0
#19 0x00005852846fbd5d EvaluateAsRValue((anonymous namespace)::EvalInfo&, clang::Expr const*, clang::APValue&) ExprConstant.cpp:0:0
#20 0x00005852846fce9f EvaluateAsRValue(clang::Expr const*, clang::Expr::EvalResult&, clang::ASTContext const&, (anonymous namespace)::EvalInfo&) ExprConstant.cpp:0:0
#21 0x00005852846ff06b clang::Expr::EvaluateAsRValue(clang::Expr::EvalResult&, clang::ASTContext const&, bool) const (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xe7bd06b)
#22 0x00005852846ff16b clang::Expr::isEvaluatable(clang::ASTContext const&, clang::Expr::SideEffectsKind) const (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xe7bd16b)
#23 0x000058527fd54599 clang::CodeGen::CodeGenFunction::EmitReturnStmt(clang::ReturnStmt const&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9e12599)
#24 0x000058527fd5f703 clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef<clang::Attr const*>) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9e1d703)
#25 0x000058527fd6901d clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&, bool, clang::CodeGen::AggValueSlot) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9e2701d)
#26 0x000058527fe030ff clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9ec10ff)
#27 0x000058527fe233f9 clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9ee13f9)
#28 0x000058527feca5e5 clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9f885e5)
#29 0x000058527fed007d clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9f8e07d)
#30 0x000058527fed5278 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9f93278)
#31 0x000058527fed636a clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9f9436a)
#32 0x00005852803efbe0 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#33 0x00005852803d9475 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa497475)
#34 0x0000585282ab39b2 clang::ParseAST(clang::Sema&, bool, bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb719b2)
#35 0x00005852807fb92b clang::ASTFrontendAction::ExecuteAction() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa8b992b)
#36 0x00005852803e9360 clang::CodeGenAction::ExecuteAction() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa4a7360)
#37 0x00005852808039cf clang::FrontendAction::Execute() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa8c19cf)
#38 0x0000585280749f46 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa807f46)
#39 0x000058528098bf7b clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xaa49f7b)
#40 0x000058527a6fa523 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47b8523)
#41 0x000058527a6eac58 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#42 0x000058527a6f0b28 clang_main(int, char**, llvm::ToolContext const&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47aeb28)
#43 0x000058527a54bb4a main (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x4609b4a)
#44 0x00007b8c2bdba1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#45 0x00007b8c2bdba28b call_init ./csu/../csu/libc-start.c:128:20
#46 0x00007b8c2bdba28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#47 0x000058527a6e8f55 _start (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47a6f55)
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

### Comment 1 - k-arrows

The program and backtrace presented here are similar to those in https://github.com/llvm/llvm-project/issues/154713

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: None (AbyssStaror)

<details>
### Link for quick verification: https://godbolt.org/z/qTGhzsjv6
```c
typedef int v1si __attribute__ ((vector_size (4))); 

v1si test_vector = {1};

int get_last_element() {
    return test_vector[~0UL]; 
}
```
When compiling code that uses `~0UL`(all bits set to 1 for unsigned long) as an index for a vector type, Clang crashes with an assertion failure in `APSInt::getExtValue()`. The crash occurs during constant expression evaluation in the LLVM IR generation phase.
The crash is specific to the extremely large value of` ~0UL`. Using moderately large but still out-of-bounds values like `__INT_MAX__`does not trigger the crash, suggesting the issue is specifically with values that exceed `int64_t` representation.
## Stack dump:
```
clang-21: /workspace/install/llvm/src/llvm-project/llvm/include/llvm/ADT/APSInt.h:100: int64_t llvm::APSInt::getExtValue() const: Assertion `isRepresentableByInt64() &amp;&amp; "Too many bits for int64_t"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /workspace/install/llvm/build_21.1.4/bin/clang-21 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -main-file-name 13974.fuzz -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/workspace/exps -fcoverage-compilation-dir=/workspace/exps -resource-dir /workspace/install/llvm/build_21.1.4/lib/clang/21 -internal-isystem /workspace/install/llvm/build_21.1.4/lib/clang/21/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -ferror-limit 19 -fmessage-length=137 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/13974-5bc6b6.o -x c /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_1_exp/cases/13974.fuzz
1.      /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_1_exp/cases/13974.fuzz:9:1: current parser token 'int'
2.      /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_1_exp/cases/13974.fuzz:5:5: LLVM IR generation of declaration 'get_last_element'
3.      /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_1_exp/cases/13974.fuzz:5:5: Generating code for declaration 'get_last_element'
 #<!-- -->0 0x000058527f5d3c2a llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9691c2a)
 #<!-- -->1 0x000058527f5d4457 PrintStackTraceSignalHandler(void*) Signals.cpp:0:0
 #<!-- -->2 0x000058527f5cdf57 llvm::sys::RunSignalHandlers() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x968bf57)
 #<!-- -->3 0x000058527f5ce1c7 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->4 0x00007b8c2bdd5330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->5 0x00007b8c2be2eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->6 0x00007b8c2be2eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->7 0x00007b8c2be2eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->8 0x00007b8c2bdd527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x00007b8c2bdb88ff abort ./stdlib/abort.c:81:7
#<!-- -->10 0x00007b8c2bdb881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->11 0x00007b8c2bdcb517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->12 0x000058527fc8c2a1 llvm::APSInt::getExtValue() const (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9d4a2a1)
#<!-- -->13 0x00005852847080e6 (anonymous namespace)::LValueExprEvaluator::VisitArraySubscriptExpr(clang::ArraySubscriptExpr const*) ExprConstant.cpp:0:0
#<!-- -->14 0x00005852846c78ca clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::LValueExprEvaluator, bool&gt;::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#<!-- -->15 0x00005852846ca949 EvaluateLValue(clang::Expr const*, (anonymous namespace)::LValue&amp;, (anonymous namespace)::EvalInfo&amp;, bool) ExprConstant.cpp:0:0
#<!-- -->16 0x0000585284776733 (anonymous namespace)::IntExprEvaluator::VisitCastExpr(clang::CastExpr const*) ExprConstant.cpp:0:0
#<!-- -->17 0x00005852846e119b clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool&gt;::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#<!-- -->18 0x00005852846f2627 Evaluate(clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Expr const*) ExprConstant.cpp:0:0
#<!-- -->19 0x00005852846fbd5d EvaluateAsRValue((anonymous namespace)::EvalInfo&amp;, clang::Expr const*, clang::APValue&amp;) ExprConstant.cpp:0:0
#<!-- -->20 0x00005852846fce9f EvaluateAsRValue(clang::Expr const*, clang::Expr::EvalResult&amp;, clang::ASTContext const&amp;, (anonymous namespace)::EvalInfo&amp;) ExprConstant.cpp:0:0
#<!-- -->21 0x00005852846ff06b clang::Expr::EvaluateAsRValue(clang::Expr::EvalResult&amp;, clang::ASTContext const&amp;, bool) const (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xe7bd06b)
#<!-- -->22 0x00005852846ff16b clang::Expr::isEvaluatable(clang::ASTContext const&amp;, clang::Expr::SideEffectsKind) const (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xe7bd16b)
#<!-- -->23 0x000058527fd54599 clang::CodeGen::CodeGenFunction::EmitReturnStmt(clang::ReturnStmt const&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9e12599)
#<!-- -->24 0x000058527fd5f703 clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef&lt;clang::Attr const*&gt;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9e1d703)
#<!-- -->25 0x000058527fd6901d clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&amp;, bool, clang::CodeGen::AggValueSlot) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9e2701d)
#<!-- -->26 0x000058527fe030ff clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9ec10ff)
#<!-- -->27 0x000058527fe233f9 clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9ee13f9)
#<!-- -->28 0x000058527feca5e5 clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9f885e5)
#<!-- -->29 0x000058527fed007d clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9f8e07d)
#<!-- -->30 0x000058527fed5278 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9f93278)
#<!-- -->31 0x000058527fed636a clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9f9436a)
#<!-- -->32 0x00005852803efbe0 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#<!-- -->33 0x00005852803d9475 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa497475)
#<!-- -->34 0x0000585282ab39b2 clang::ParseAST(clang::Sema&amp;, bool, bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb719b2)
#<!-- -->35 0x00005852807fb92b clang::ASTFrontendAction::ExecuteAction() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa8b992b)
#<!-- -->36 0x00005852803e9360 clang::CodeGenAction::ExecuteAction() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa4a7360)
#<!-- -->37 0x00005852808039cf clang::FrontendAction::Execute() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa8c19cf)
#<!-- -->38 0x0000585280749f46 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa807f46)
#<!-- -->39 0x000058528098bf7b clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xaa49f7b)
#<!-- -->40 0x000058527a6fa523 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47b8523)
#<!-- -->41 0x000058527a6eac58 ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->42 0x000058527a6f0b28 clang_main(int, char**, llvm::ToolContext const&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47aeb28)
#<!-- -->43 0x000058527a54bb4a main (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x4609b4a)
#<!-- -->44 0x00007b8c2bdba1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->45 0x00007b8c2bdba28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->46 0x00007b8c2bdba28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->47 0x000058527a6e8f55 _start (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47a6f55)
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

I am going to close this a duplicate, if the problems end up having different causes we can reopen this one then.

---

