# clang crashes with assertion failure in llvm::Align when __atomic_always_lock_free is called with size 0

**Author:** AbyssStaror
**URL:** https://github.com/llvm/llvm-project/issues/170139
**Status:** Open
**Labels:** clang:codegen, confirmed, crash, regression:19

## Body

## Link for quick verification: https://godbolt.org/z/vv1YMceov
```c
struct A { int a; }; 
int main() {
    struct A obj;
    __atomic_always_lock_free(0, 0); 
    return 0;
}
```
When I call the `__atomic_always_lock_free` function with 0 as the first argument, and the compiler crashes. And The `__atomic_always_lock_free` function expects:

- First parameter: Object size (in bytes) as size_t

- Second parameter: Optional pointer for alignment considerations

## The stack dump:
```
clang-21: /workspace/install/llvm/src/llvm-project/llvm/include/llvm/Support/Alignment.h:77: llvm::Align::Align(uint64_t): Assertion `Value > 0 && "Value must not be 0"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /workspace/install/llvm/build_21.1.4/bin/clang-21 -cc1 -triple x86_64-unknown-linux-gnu -O3 -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -main-file-name 46471.fuzz -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=none -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/workspace/exps -fcoverage-compilation-dir=/workspace/exps -resource-dir /workspace/install/llvm/build_21.1.4/lib/clang/21 -internal-isystem /workspace/install/llvm/build_21.1.4/lib/clang/21/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -ferror-limit 19 -fmessage-length=167 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -vectorize-loops -vectorize-slp -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/46471-bb02b6.o -x c /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_choose_3_1_exp/cases/46471.fuzz
1.      <eof> parser at end of file
2.      /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_choose_3_1_exp/cases/46471.fuzz:2:5: LLVM IR generation of declaration 'main'
3.      /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_choose_3_1_exp/cases/46471.fuzz:2:5: Generating code for declaration 'main'
 #0 0x00005905d10ccc2a llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9691c2a)
 #1 0x00005905d10cd457 PrintStackTraceSignalHandler(void*) Signals.cpp:0:0
 #2 0x00005905d10c6f57 llvm::sys::RunSignalHandlers() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x968bf57)
 #3 0x00005905d10c71c7 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #4 0x00007dbeb8189330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #5 0x00007dbeb81e2b2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #6 0x00007dbeb81e2b2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #7 0x00007dbeb81e2b2c pthread_kill ./nptl/pthread_kill.c:89:10
 #8 0x00007dbeb818927e raise ./signal/../sysdeps/posix/raise.c:27:6
 #9 0x00007dbeb816c8ff abort ./stdlib/abort.c:81:7
#10 0x00007dbeb816c81b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#11 0x00007dbeb817f517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#12 0x00005905cc5b0a5e (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x4b75a5e)
#13 0x00005905d61d78ef (anonymous namespace)::IntExprEvaluator::VisitBuiltinCallExpr(clang::CallExpr const*, unsigned int) ExprConstant.cpp:0:0
#14 0x00005905d61d8261 (anonymous namespace)::IntExprEvaluator::VisitCallExpr(clang::CallExpr const*) ExprConstant.cpp:0:0
#15 0x00005905d61db819 clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#16 0x00005905d61eb627 Evaluate(clang::APValue&, (anonymous namespace)::EvalInfo&, clang::Expr const*) ExprConstant.cpp:0:0
#17 0x00005905d61f4d5d EvaluateAsRValue((anonymous namespace)::EvalInfo&, clang::Expr const*, clang::APValue&) ExprConstant.cpp:0:0
#18 0x00005905d61f5e9f EvaluateAsRValue(clang::Expr const*, clang::Expr::EvalResult&, clang::ASTContext const&, (anonymous namespace)::EvalInfo&) ExprConstant.cpp:0:0
#19 0x00005905d61f806b clang::Expr::EvaluateAsRValue(clang::Expr::EvalResult&, clang::ASTContext const&, bool) const (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xe7bd06b)
#20 0x00005905d1d1de28 clang::CodeGen::CodeGenFunction::EmitBuiltinExpr(clang::GlobalDecl, unsigned int, clang::CallExpr const*, clang::CodeGen::ReturnValueSlot) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa2e2e28)
#21 0x00005905d1625721 clang::CodeGen::CodeGenFunction::EmitCallExpr(clang::CallExpr const*, clang::CodeGen::ReturnValueSlot, llvm::CallBase**) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9bea721)
#22 0x00005905d16cfa1e (anonymous namespace)::ScalarExprEmitter::VisitCallExpr(clang::CallExpr const*) CGExprScalar.cpp:0:0
#23 0x00005905d16b8fef clang::StmtVisitorBase<std::add_pointer, (anonymous namespace)::ScalarExprEmitter, llvm::Value*>::Visit(clang::Stmt*) CGExprScalar.cpp:0:0
#24 0x00005905d16c0fb6 clang::CodeGen::CodeGenFunction::EmitScalarExpr(clang::Expr const*, bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9c85fb6)
#25 0x00005905d15f7141 clang::CodeGen::CodeGenFunction::EmitAnyExpr(clang::Expr const*, clang::CodeGen::AggValueSlot, bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9bbc141)
#26 0x00005905d161fe90 clang::CodeGen::CodeGenFunction::EmitIgnoredExpr(clang::Expr const*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9be4e90)
#27 0x00005905d18585c3 clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef<clang::Attr const*>) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9e1d5c3)
#28 0x00005905d186201d clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&, bool, clang::CodeGen::AggValueSlot) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9e2701d)
#29 0x00005905d18fc0ff clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9ec10ff)
#30 0x00005905d191c3f9 clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9ee13f9)
#31 0x00005905d19c35e5 clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9f885e5)
#32 0x00005905d19c907d clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9f8e07d)
#33 0x00005905d19ce278 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9f93278)
#34 0x00005905d19cf36a clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9f9436a)
#35 0x00005905d1ee8be0 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#36 0x00005905d1ed2475 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa497475)
#37 0x00005905d45ac9b2 clang::ParseAST(clang::Sema&, bool, bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb719b2)
#38 0x00005905d22f492b clang::ASTFrontendAction::ExecuteAction() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa8b992b)
#39 0x00005905d1ee2360 clang::CodeGenAction::ExecuteAction() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa4a7360)
#40 0x00005905d22fc9cf clang::FrontendAction::Execute() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa8c19cf)
#41 0x00005905d2242f46 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa807f46)
#42 0x00005905d2484f7b clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xaa49f7b)
#43 0x00005905cc1f3523 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47b8523)
#44 0x00005905cc1e3c58 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#45 0x00005905cc1e9b28 clang_main(int, char**, llvm::ToolContext const&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47aeb28)
#46 0x00005905cc044b4a main (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x4609b4a)
#47 0x00007dbeb816e1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#48 0x00007dbeb816e28b call_init ./csu/../csu/libc-start.c:128:20
#49 0x00007dbeb816e28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#50 0x00005905cc1e1f55 _start (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47a6f55)
clang: error: unable to execute command: Aborted (core dumped)
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


@llvm/issue-subscribers-clang-codegen

Author: None (AbyssStaror)

<details>
## Link for quick verification: https://godbolt.org/z/vv1YMceov
```c
struct A { int a; }; 
int main() {
    struct A obj;
    __atomic_always_lock_free(0, 0); 
    return 0;
}
```
When I call the `__atomic_always_lock_free` function with 0 as the first argument, and the compiler crashes. And The `__atomic_always_lock_free` function expects:

- First parameter: Object size (in bytes) as size_t

- Second parameter: Optional pointer for alignment considerations

## The stack dump:
```
clang-21: /workspace/install/llvm/src/llvm-project/llvm/include/llvm/Support/Alignment.h:77: llvm::Align::Align(uint64_t): Assertion `Value &gt; 0 &amp;&amp; "Value must not be 0"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /workspace/install/llvm/build_21.1.4/bin/clang-21 -cc1 -triple x86_64-unknown-linux-gnu -O3 -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -main-file-name 46471.fuzz -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=none -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/workspace/exps -fcoverage-compilation-dir=/workspace/exps -resource-dir /workspace/install/llvm/build_21.1.4/lib/clang/21 -internal-isystem /workspace/install/llvm/build_21.1.4/lib/clang/21/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -ferror-limit 19 -fmessage-length=167 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -vectorize-loops -vectorize-slp -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/46471-bb02b6.o -x c /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_choose_3_1_exp/cases/46471.fuzz
1.      &lt;eof&gt; parser at end of file
2.      /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_choose_3_1_exp/cases/46471.fuzz:2:5: LLVM IR generation of declaration 'main'
3.      /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_choose_3_1_exp/cases/46471.fuzz:2:5: Generating code for declaration 'main'
 #<!-- -->0 0x00005905d10ccc2a llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9691c2a)
 #<!-- -->1 0x00005905d10cd457 PrintStackTraceSignalHandler(void*) Signals.cpp:0:0
 #<!-- -->2 0x00005905d10c6f57 llvm::sys::RunSignalHandlers() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x968bf57)
 #<!-- -->3 0x00005905d10c71c7 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->4 0x00007dbeb8189330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->5 0x00007dbeb81e2b2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->6 0x00007dbeb81e2b2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->7 0x00007dbeb81e2b2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->8 0x00007dbeb818927e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x00007dbeb816c8ff abort ./stdlib/abort.c:81:7
#<!-- -->10 0x00007dbeb816c81b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->11 0x00007dbeb817f517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->12 0x00005905cc5b0a5e (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x4b75a5e)
#<!-- -->13 0x00005905d61d78ef (anonymous namespace)::IntExprEvaluator::VisitBuiltinCallExpr(clang::CallExpr const*, unsigned int) ExprConstant.cpp:0:0
#<!-- -->14 0x00005905d61d8261 (anonymous namespace)::IntExprEvaluator::VisitCallExpr(clang::CallExpr const*) ExprConstant.cpp:0:0
#<!-- -->15 0x00005905d61db819 clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool&gt;::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#<!-- -->16 0x00005905d61eb627 Evaluate(clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Expr const*) ExprConstant.cpp:0:0
#<!-- -->17 0x00005905d61f4d5d EvaluateAsRValue((anonymous namespace)::EvalInfo&amp;, clang::Expr const*, clang::APValue&amp;) ExprConstant.cpp:0:0
#<!-- -->18 0x00005905d61f5e9f EvaluateAsRValue(clang::Expr const*, clang::Expr::EvalResult&amp;, clang::ASTContext const&amp;, (anonymous namespace)::EvalInfo&amp;) ExprConstant.cpp:0:0
#<!-- -->19 0x00005905d61f806b clang::Expr::EvaluateAsRValue(clang::Expr::EvalResult&amp;, clang::ASTContext const&amp;, bool) const (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xe7bd06b)
#<!-- -->20 0x00005905d1d1de28 clang::CodeGen::CodeGenFunction::EmitBuiltinExpr(clang::GlobalDecl, unsigned int, clang::CallExpr const*, clang::CodeGen::ReturnValueSlot) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa2e2e28)
#<!-- -->21 0x00005905d1625721 clang::CodeGen::CodeGenFunction::EmitCallExpr(clang::CallExpr const*, clang::CodeGen::ReturnValueSlot, llvm::CallBase**) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9bea721)
#<!-- -->22 0x00005905d16cfa1e (anonymous namespace)::ScalarExprEmitter::VisitCallExpr(clang::CallExpr const*) CGExprScalar.cpp:0:0
#<!-- -->23 0x00005905d16b8fef clang::StmtVisitorBase&lt;std::add_pointer, (anonymous namespace)::ScalarExprEmitter, llvm::Value*&gt;::Visit(clang::Stmt*) CGExprScalar.cpp:0:0
#<!-- -->24 0x00005905d16c0fb6 clang::CodeGen::CodeGenFunction::EmitScalarExpr(clang::Expr const*, bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9c85fb6)
#<!-- -->25 0x00005905d15f7141 clang::CodeGen::CodeGenFunction::EmitAnyExpr(clang::Expr const*, clang::CodeGen::AggValueSlot, bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9bbc141)
#<!-- -->26 0x00005905d161fe90 clang::CodeGen::CodeGenFunction::EmitIgnoredExpr(clang::Expr const*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9be4e90)
#<!-- -->27 0x00005905d18585c3 clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef&lt;clang::Attr const*&gt;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9e1d5c3)
#<!-- -->28 0x00005905d186201d clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&amp;, bool, clang::CodeGen::AggValueSlot) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9e2701d)
#<!-- -->29 0x00005905d18fc0ff clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9ec10ff)
#<!-- -->30 0x00005905d191c3f9 clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9ee13f9)
#<!-- -->31 0x00005905d19c35e5 clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9f885e5)
#<!-- -->32 0x00005905d19c907d clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9f8e07d)
#<!-- -->33 0x00005905d19ce278 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9f93278)
#<!-- -->34 0x00005905d19cf36a clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9f9436a)
#<!-- -->35 0x00005905d1ee8be0 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#<!-- -->36 0x00005905d1ed2475 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa497475)
#<!-- -->37 0x00005905d45ac9b2 clang::ParseAST(clang::Sema&amp;, bool, bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb719b2)
#<!-- -->38 0x00005905d22f492b clang::ASTFrontendAction::ExecuteAction() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa8b992b)
#<!-- -->39 0x00005905d1ee2360 clang::CodeGenAction::ExecuteAction() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa4a7360)
#<!-- -->40 0x00005905d22fc9cf clang::FrontendAction::Execute() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa8c19cf)
#<!-- -->41 0x00005905d2242f46 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa807f46)
#<!-- -->42 0x00005905d2484f7b clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xaa49f7b)
#<!-- -->43 0x00005905cc1f3523 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47b8523)
#<!-- -->44 0x00005905cc1e3c58 ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->45 0x00005905cc1e9b28 clang_main(int, char**, llvm::ToolContext const&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47aeb28)
#<!-- -->46 0x00005905cc044b4a main (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x4609b4a)
#<!-- -->47 0x00007dbeb816e1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->48 0x00007dbeb816e28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->49 0x00007dbeb816e28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->50 0x00005905cc1e1f55 _start (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47a6f55)
clang: error: unable to execute command: Aborted (core dumped)
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

### Comment 2 - EugeneZelenko

@AbyssStaror : If you plan to file more issues in future, it makes sense to request triage role ([examples](https://github.com/llvm/llvm-project/issues/?q=is%3Aissue%20state%3Aclosed%20label%3Ainfra%3Acommit-access-request%20Triage)), so you'll be able to label issues yourself (preferably at creation, to avoid unnecessary notifications).

---

### Comment 3 - AbyssStaror

> [@AbyssStaror](https://github.com/AbyssStaror) : If you plan to file more issues in future, it makes sense to request triage role ([examples](https://github.com/llvm/llvm-project/issues/?q=is%3Aissue%20state%3Aclosed%20label%3Ainfra%3Acommit-access-request%20Triage)), so you'll be able to label issues yourself (preferably at creation, to avoid unnecessary notifications).

OK, I will request triage role.

---

### Comment 4 - shafik

Related: https://github.com/llvm/llvm-project/issues/120082

---

### Comment 5 - shafik

@jyknight this bisects to 511e93b96ee74438cbd643cec63281aff0663933

---

### Comment 6 - AbyssStaror

`bool gnu_tm_available = __atomic_is_lock_free(0, NULL);`
The code also triggers the same crash.

---

