# Clang crashes in CastInst::Create when using __builtin_fmaxf16 with __bf16 type

**Author:** AbyssStaror
**URL:** https://github.com/llvm/llvm-project/issues/170434

## Body

### Link for quick verification: https://godbolt.org/z/YaE7KbYEr
Reduced code:
```c
__bf16 fmaxf16_bfloat16(__bf16 a) {
    return __builtin_fmaxf16(a, a);  // Apply intrinsic with __bf16
}
```
Clang encounters an internal compiler error (ICE) when compiling code that uses the` __builtin_fmaxf16`intrinsic with `__bf16`(bfloat16) type arguments. The compiler fails during LLVM IR generation with an assertion failure in `CastInst::Create`. `__builtin_fmaxf16`expects `_Float16t`ype arguments, but the code is passing `__bf16`arguments. When Clang attempts to perform implicit type conversion between these incompatible floating-point types during IR generation, it triggers an invalid cast assertion.

## Stack dump:
```
clang-21: /workspace/install/llvm/src/llvm-project/llvm/lib/IR/Instructions.cpp:3037: static llvm::CastInst* llvm::CastInst::Create(llvm::Instruction::CastOps, llvm::Value*, llvm::Type*, const llvm::Twine&, llvm::InsertPosition): Assertion `castIsValid(op, S, Ty) && "Invalid cast!"' failed.
Stack dump:
0.      Program arguments: /workspace/install/llvm/build_21.1.4/bin/clang-21 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -main-file-name 49630.fuzz -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/workspace/exps -fcoverage-compilation-dir=/workspace/exps -resource-dir /workspace/install/llvm/build_21.1.4/lib/clang/21 -internal-isystem /workspace/install/llvm/build_21.1.4/lib/clang/21/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -ferror-limit 19 -fmessage-length=137 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/49630-f3f840.o -x c /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_input_2_feature_1_exp/cases/49630.fuzz
1.      /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_input_2_feature_1_exp/cases/49630.fuzz:10:1: current parser token 'float'
2.      /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_input_2_feature_1_exp/cases/49630.fuzz:6:8: LLVM IR generation of declaration 'fmaxf16_bfloat16'
3.      /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_input_2_feature_1_exp/cases/49630.fuzz:6:8: Generating code for declaration 'fmaxf16_bfloat16'
 #0 0x0000587e3af94c2a llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9691c2a)
 #1 0x0000587e3af95457 PrintStackTraceSignalHandler(void*) Signals.cpp:0:0
 #2 0x0000587e3af8ef57 llvm::sys::RunSignalHandlers() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x968bf57)
 #3 0x0000587e3af8f1c7 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #4 0x00007ae1baccc330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #5 0x00007ae1bad25b2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #6 0x00007ae1bad25b2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #7 0x00007ae1bad25b2c pthread_kill ./nptl/pthread_kill.c:89:10
 #8 0x00007ae1baccc27e raise ./signal/../sysdeps/posix/raise.c:27:6
 #9 0x00007ae1bacaf8ff abort ./stdlib/abort.c:81:7
#10 0x00007ae1bacaf81b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#11 0x00007ae1bacc2517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#12 0x0000587e3a46b8dd (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x8b688dd)
#13 0x0000587e3a8e7e69 llvm::IRBuilderBase::CreateCast(llvm::Instruction::CastOps, llvm::Value*, llvm::Type*, llvm::Twine const&, llvm::MDNode*, llvm::FMFSource) (.isra.0) InstCombineCasts.cpp:0:0
#14 0x0000587e3a8e9573 llvm::IRBuilderBase::CreateFPTruncFMF(llvm::Value*, llvm::Type*, llvm::FMFSource, llvm::Twine const&, llvm::MDNode*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x8fe6573)
#15 0x0000587e3b577e70 (anonymous namespace)::ScalarExprEmitter::EmitScalarConversion(llvm::Value*, clang::QualType, clang::QualType, clang::SourceLocation, (anonymous namespace)::ScalarExprEmitter::ScalarConversionOpts) CGExprScalar.cpp:0:0
#16 0x0000587e3b594792 (anonymous namespace)::ScalarExprEmitter::VisitCastExpr(clang::CastExpr*) CGExprScalar.cpp:0:0
#17 0x0000587e3b581368 clang::StmtVisitorBase<std::add_pointer, (anonymous namespace)::ScalarExprEmitter, llvm::Value*>::Visit(clang::Stmt*) CGExprScalar.cpp:0:0
#18 0x0000587e3b588fb6 clang::CodeGen::CodeGenFunction::EmitScalarExpr(clang::Expr const*, bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9c85fb6)
#19 0x0000587e3bbce5cb emitBinaryMaybeConstrainedFPBuiltin(clang::CodeGen::CodeGenFunction&, clang::CallExpr const*, unsigned int, unsigned int) CGBuiltin.cpp:0:0
#20 0x0000587e3bbe6915 clang::CodeGen::CodeGenFunction::EmitBuiltinExpr(clang::GlobalDecl, unsigned int, clang::CallExpr const*, clang::CodeGen::ReturnValueSlot) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa2e3915)
#21 0x0000587e3b4ed721 clang::CodeGen::CodeGenFunction::EmitCallExpr(clang::CallExpr const*, clang::CodeGen::ReturnValueSlot, llvm::CallBase**) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9bea721)
#22 0x0000587e3b597a1e (anonymous namespace)::ScalarExprEmitter::VisitCallExpr(clang::CallExpr const*) CGExprScalar.cpp:0:0
#23 0x0000587e3b580fef clang::StmtVisitorBase<std::add_pointer, (anonymous namespace)::ScalarExprEmitter, llvm::Value*>::Visit(clang::Stmt*) CGExprScalar.cpp:0:0
#24 0x0000587e3b58363a (anonymous namespace)::ScalarExprEmitter::Visit(clang::Expr*) CGExprScalar.cpp:0:0
#25 0x0000587e3b59476c (anonymous namespace)::ScalarExprEmitter::VisitCastExpr(clang::CastExpr*) CGExprScalar.cpp:0:0
#26 0x0000587e3b581368 clang::StmtVisitorBase<std::add_pointer, (anonymous namespace)::ScalarExprEmitter, llvm::Value*>::Visit(clang::Stmt*) CGExprScalar.cpp:0:0
#27 0x0000587e3b588fb6 clang::CodeGen::CodeGenFunction::EmitScalarExpr(clang::Expr const*, bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9c85fb6)
#28 0x0000587e3b715249 clang::CodeGen::CodeGenFunction::EmitReturnStmt(clang::ReturnStmt const&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9e12249)
#29 0x0000587e3b720703 clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef<clang::Attr const*>) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9e1d703)
#30 0x0000587e3b72a01d clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&, bool, clang::CodeGen::AggValueSlot) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9e2701d)
#31 0x0000587e3b7c40ff clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9ec10ff)
#32 0x0000587e3b7e43f9 clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9ee13f9)
#33 0x0000587e3b88b5e5 clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9f885e5)
#34 0x0000587e3b89107d clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9f8e07d)
#35 0x0000587e3b896278 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9f93278)
#36 0x0000587e3b89736a clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9f9436a)
#37 0x0000587e3bdb0be0 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#38 0x0000587e3bd9a475 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa497475)
#39 0x0000587e3e4749b2 clang::ParseAST(clang::Sema&, bool, bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb719b2)
#40 0x0000587e3c1bc92b clang::ASTFrontendAction::ExecuteAction() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa8b992b)
#41 0x0000587e3bdaa360 clang::CodeGenAction::ExecuteAction() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa4a7360)
#42 0x0000587e3c1c49cf clang::FrontendAction::Execute() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa8c19cf)
#43 0x0000587e3c10af46 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa807f46)
#44 0x0000587e3c34cf7b clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xaa49f7b)
#45 0x0000587e360bb523 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47b8523)
#46 0x0000587e360abc58 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#47 0x0000587e360b1b28 clang_main(int, char**, llvm::ToolContext const&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47aeb28)
#48 0x0000587e35f0cb4a main (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x4609b4a)
#49 0x00007ae1bacb11ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#50 0x00007ae1bacb128b call_init ./csu/../csu/libc-start.c:128:20
#51 0x00007ae1bacb128b __libc_start_main ./csu/../csu/libc-start.c:347:5
#52 0x0000587e360a9f55 _start (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47a6f55)
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

Started to crash since Clang 17. [Godbolt link](https://godbolt.org/z/KT61W4G74).

It's curious that Clang doesn't crash on this in C++ modes. Perhaps there's something wrong in the frontend.

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (AbyssStaror)

<details>
### Link for quick verification: https://godbolt.org/z/YaE7KbYEr
Reduced code:
```c
__bf16 fmaxf16_bfloat16(__bf16 a) {
    return __builtin_fmaxf16(a, a);  // Apply intrinsic with __bf16
}
```
Clang encounters an internal compiler error (ICE) when compiling code that uses the` __builtin_fmaxf16`intrinsic with `__bf16`(bfloat16) type arguments. The compiler fails during LLVM IR generation with an assertion failure in `CastInst::Create`. `__builtin_fmaxf16`expects `_Float16t`ype arguments, but the code is passing `__bf16`arguments. When Clang attempts to perform implicit type conversion between these incompatible floating-point types during IR generation, it triggers an invalid cast assertion.

## Stack dump:
```
clang-21: /workspace/install/llvm/src/llvm-project/llvm/lib/IR/Instructions.cpp:3037: static llvm::CastInst* llvm::CastInst::Create(llvm::Instruction::CastOps, llvm::Value*, llvm::Type*, const llvm::Twine&amp;, llvm::InsertPosition): Assertion `castIsValid(op, S, Ty) &amp;&amp; "Invalid cast!"' failed.
Stack dump:
0.      Program arguments: /workspace/install/llvm/build_21.1.4/bin/clang-21 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -main-file-name 49630.fuzz -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/workspace/exps -fcoverage-compilation-dir=/workspace/exps -resource-dir /workspace/install/llvm/build_21.1.4/lib/clang/21 -internal-isystem /workspace/install/llvm/build_21.1.4/lib/clang/21/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -ferror-limit 19 -fmessage-length=137 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/49630-f3f840.o -x c /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_input_2_feature_1_exp/cases/49630.fuzz
1.      /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_input_2_feature_1_exp/cases/49630.fuzz:10:1: current parser token 'float'
2.      /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_input_2_feature_1_exp/cases/49630.fuzz:6:8: LLVM IR generation of declaration 'fmaxf16_bfloat16'
3.      /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_input_2_feature_1_exp/cases/49630.fuzz:6:8: Generating code for declaration 'fmaxf16_bfloat16'
 #<!-- -->0 0x0000587e3af94c2a llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9691c2a)
 #<!-- -->1 0x0000587e3af95457 PrintStackTraceSignalHandler(void*) Signals.cpp:0:0
 #<!-- -->2 0x0000587e3af8ef57 llvm::sys::RunSignalHandlers() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x968bf57)
 #<!-- -->3 0x0000587e3af8f1c7 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->4 0x00007ae1baccc330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->5 0x00007ae1bad25b2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->6 0x00007ae1bad25b2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->7 0x00007ae1bad25b2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->8 0x00007ae1baccc27e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x00007ae1bacaf8ff abort ./stdlib/abort.c:81:7
#<!-- -->10 0x00007ae1bacaf81b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->11 0x00007ae1bacc2517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->12 0x0000587e3a46b8dd (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x8b688dd)
#<!-- -->13 0x0000587e3a8e7e69 llvm::IRBuilderBase::CreateCast(llvm::Instruction::CastOps, llvm::Value*, llvm::Type*, llvm::Twine const&amp;, llvm::MDNode*, llvm::FMFSource) (.isra.0) InstCombineCasts.cpp:0:0
#<!-- -->14 0x0000587e3a8e9573 llvm::IRBuilderBase::CreateFPTruncFMF(llvm::Value*, llvm::Type*, llvm::FMFSource, llvm::Twine const&amp;, llvm::MDNode*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x8fe6573)
#<!-- -->15 0x0000587e3b577e70 (anonymous namespace)::ScalarExprEmitter::EmitScalarConversion(llvm::Value*, clang::QualType, clang::QualType, clang::SourceLocation, (anonymous namespace)::ScalarExprEmitter::ScalarConversionOpts) CGExprScalar.cpp:0:0
#<!-- -->16 0x0000587e3b594792 (anonymous namespace)::ScalarExprEmitter::VisitCastExpr(clang::CastExpr*) CGExprScalar.cpp:0:0
#<!-- -->17 0x0000587e3b581368 clang::StmtVisitorBase&lt;std::add_pointer, (anonymous namespace)::ScalarExprEmitter, llvm::Value*&gt;::Visit(clang::Stmt*) CGExprScalar.cpp:0:0
#<!-- -->18 0x0000587e3b588fb6 clang::CodeGen::CodeGenFunction::EmitScalarExpr(clang::Expr const*, bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9c85fb6)
#<!-- -->19 0x0000587e3bbce5cb emitBinaryMaybeConstrainedFPBuiltin(clang::CodeGen::CodeGenFunction&amp;, clang::CallExpr const*, unsigned int, unsigned int) CGBuiltin.cpp:0:0
#<!-- -->20 0x0000587e3bbe6915 clang::CodeGen::CodeGenFunction::EmitBuiltinExpr(clang::GlobalDecl, unsigned int, clang::CallExpr const*, clang::CodeGen::ReturnValueSlot) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa2e3915)
#<!-- -->21 0x0000587e3b4ed721 clang::CodeGen::CodeGenFunction::EmitCallExpr(clang::CallExpr const*, clang::CodeGen::ReturnValueSlot, llvm::CallBase**) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9bea721)
#<!-- -->22 0x0000587e3b597a1e (anonymous namespace)::ScalarExprEmitter::VisitCallExpr(clang::CallExpr const*) CGExprScalar.cpp:0:0
#<!-- -->23 0x0000587e3b580fef clang::StmtVisitorBase&lt;std::add_pointer, (anonymous namespace)::ScalarExprEmitter, llvm::Value*&gt;::Visit(clang::Stmt*) CGExprScalar.cpp:0:0
#<!-- -->24 0x0000587e3b58363a (anonymous namespace)::ScalarExprEmitter::Visit(clang::Expr*) CGExprScalar.cpp:0:0
#<!-- -->25 0x0000587e3b59476c (anonymous namespace)::ScalarExprEmitter::VisitCastExpr(clang::CastExpr*) CGExprScalar.cpp:0:0
#<!-- -->26 0x0000587e3b581368 clang::StmtVisitorBase&lt;std::add_pointer, (anonymous namespace)::ScalarExprEmitter, llvm::Value*&gt;::Visit(clang::Stmt*) CGExprScalar.cpp:0:0
#<!-- -->27 0x0000587e3b588fb6 clang::CodeGen::CodeGenFunction::EmitScalarExpr(clang::Expr const*, bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9c85fb6)
#<!-- -->28 0x0000587e3b715249 clang::CodeGen::CodeGenFunction::EmitReturnStmt(clang::ReturnStmt const&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9e12249)
#<!-- -->29 0x0000587e3b720703 clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef&lt;clang::Attr const*&gt;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9e1d703)
#<!-- -->30 0x0000587e3b72a01d clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&amp;, bool, clang::CodeGen::AggValueSlot) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9e2701d)
#<!-- -->31 0x0000587e3b7c40ff clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9ec10ff)
#<!-- -->32 0x0000587e3b7e43f9 clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9ee13f9)
#<!-- -->33 0x0000587e3b88b5e5 clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9f885e5)
#<!-- -->34 0x0000587e3b89107d clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9f8e07d)
#<!-- -->35 0x0000587e3b896278 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9f93278)
#<!-- -->36 0x0000587e3b89736a clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9f9436a)
#<!-- -->37 0x0000587e3bdb0be0 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#<!-- -->38 0x0000587e3bd9a475 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa497475)
#<!-- -->39 0x0000587e3e4749b2 clang::ParseAST(clang::Sema&amp;, bool, bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb719b2)
#<!-- -->40 0x0000587e3c1bc92b clang::ASTFrontendAction::ExecuteAction() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa8b992b)
#<!-- -->41 0x0000587e3bdaa360 clang::CodeGenAction::ExecuteAction() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa4a7360)
#<!-- -->42 0x0000587e3c1c49cf clang::FrontendAction::Execute() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa8c19cf)
#<!-- -->43 0x0000587e3c10af46 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa807f46)
#<!-- -->44 0x0000587e3c34cf7b clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xaa49f7b)
#<!-- -->45 0x0000587e360bb523 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47b8523)
#<!-- -->46 0x0000587e360abc58 ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->47 0x0000587e360b1b28 clang_main(int, char**, llvm::ToolContext const&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47aeb28)
#<!-- -->48 0x0000587e35f0cb4a main (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x4609b4a)
#<!-- -->49 0x00007ae1bacb11ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->50 0x00007ae1bacb128b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->51 0x00007ae1bacb128b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->52 0x0000587e360a9f55 _start (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47a6f55)
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

### Comment 3 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: None (AbyssStaror)

<details>
### Link for quick verification: https://godbolt.org/z/YaE7KbYEr
Reduced code:
```c
__bf16 fmaxf16_bfloat16(__bf16 a) {
    return __builtin_fmaxf16(a, a);  // Apply intrinsic with __bf16
}
```
Clang encounters an internal compiler error (ICE) when compiling code that uses the` __builtin_fmaxf16`intrinsic with `__bf16`(bfloat16) type arguments. The compiler fails during LLVM IR generation with an assertion failure in `CastInst::Create`. `__builtin_fmaxf16`expects `_Float16t`ype arguments, but the code is passing `__bf16`arguments. When Clang attempts to perform implicit type conversion between these incompatible floating-point types during IR generation, it triggers an invalid cast assertion.

## Stack dump:
```
clang-21: /workspace/install/llvm/src/llvm-project/llvm/lib/IR/Instructions.cpp:3037: static llvm::CastInst* llvm::CastInst::Create(llvm::Instruction::CastOps, llvm::Value*, llvm::Type*, const llvm::Twine&amp;, llvm::InsertPosition): Assertion `castIsValid(op, S, Ty) &amp;&amp; "Invalid cast!"' failed.
Stack dump:
0.      Program arguments: /workspace/install/llvm/build_21.1.4/bin/clang-21 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -main-file-name 49630.fuzz -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/workspace/exps -fcoverage-compilation-dir=/workspace/exps -resource-dir /workspace/install/llvm/build_21.1.4/lib/clang/21 -internal-isystem /workspace/install/llvm/build_21.1.4/lib/clang/21/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -ferror-limit 19 -fmessage-length=137 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/49630-f3f840.o -x c /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_input_2_feature_1_exp/cases/49630.fuzz
1.      /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_input_2_feature_1_exp/cases/49630.fuzz:10:1: current parser token 'float'
2.      /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_input_2_feature_1_exp/cases/49630.fuzz:6:8: LLVM IR generation of declaration 'fmaxf16_bfloat16'
3.      /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_input_2_feature_1_exp/cases/49630.fuzz:6:8: Generating code for declaration 'fmaxf16_bfloat16'
 #<!-- -->0 0x0000587e3af94c2a llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9691c2a)
 #<!-- -->1 0x0000587e3af95457 PrintStackTraceSignalHandler(void*) Signals.cpp:0:0
 #<!-- -->2 0x0000587e3af8ef57 llvm::sys::RunSignalHandlers() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x968bf57)
 #<!-- -->3 0x0000587e3af8f1c7 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->4 0x00007ae1baccc330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->5 0x00007ae1bad25b2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->6 0x00007ae1bad25b2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->7 0x00007ae1bad25b2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->8 0x00007ae1baccc27e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x00007ae1bacaf8ff abort ./stdlib/abort.c:81:7
#<!-- -->10 0x00007ae1bacaf81b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->11 0x00007ae1bacc2517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->12 0x0000587e3a46b8dd (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x8b688dd)
#<!-- -->13 0x0000587e3a8e7e69 llvm::IRBuilderBase::CreateCast(llvm::Instruction::CastOps, llvm::Value*, llvm::Type*, llvm::Twine const&amp;, llvm::MDNode*, llvm::FMFSource) (.isra.0) InstCombineCasts.cpp:0:0
#<!-- -->14 0x0000587e3a8e9573 llvm::IRBuilderBase::CreateFPTruncFMF(llvm::Value*, llvm::Type*, llvm::FMFSource, llvm::Twine const&amp;, llvm::MDNode*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x8fe6573)
#<!-- -->15 0x0000587e3b577e70 (anonymous namespace)::ScalarExprEmitter::EmitScalarConversion(llvm::Value*, clang::QualType, clang::QualType, clang::SourceLocation, (anonymous namespace)::ScalarExprEmitter::ScalarConversionOpts) CGExprScalar.cpp:0:0
#<!-- -->16 0x0000587e3b594792 (anonymous namespace)::ScalarExprEmitter::VisitCastExpr(clang::CastExpr*) CGExprScalar.cpp:0:0
#<!-- -->17 0x0000587e3b581368 clang::StmtVisitorBase&lt;std::add_pointer, (anonymous namespace)::ScalarExprEmitter, llvm::Value*&gt;::Visit(clang::Stmt*) CGExprScalar.cpp:0:0
#<!-- -->18 0x0000587e3b588fb6 clang::CodeGen::CodeGenFunction::EmitScalarExpr(clang::Expr const*, bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9c85fb6)
#<!-- -->19 0x0000587e3bbce5cb emitBinaryMaybeConstrainedFPBuiltin(clang::CodeGen::CodeGenFunction&amp;, clang::CallExpr const*, unsigned int, unsigned int) CGBuiltin.cpp:0:0
#<!-- -->20 0x0000587e3bbe6915 clang::CodeGen::CodeGenFunction::EmitBuiltinExpr(clang::GlobalDecl, unsigned int, clang::CallExpr const*, clang::CodeGen::ReturnValueSlot) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa2e3915)
#<!-- -->21 0x0000587e3b4ed721 clang::CodeGen::CodeGenFunction::EmitCallExpr(clang::CallExpr const*, clang::CodeGen::ReturnValueSlot, llvm::CallBase**) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9bea721)
#<!-- -->22 0x0000587e3b597a1e (anonymous namespace)::ScalarExprEmitter::VisitCallExpr(clang::CallExpr const*) CGExprScalar.cpp:0:0
#<!-- -->23 0x0000587e3b580fef clang::StmtVisitorBase&lt;std::add_pointer, (anonymous namespace)::ScalarExprEmitter, llvm::Value*&gt;::Visit(clang::Stmt*) CGExprScalar.cpp:0:0
#<!-- -->24 0x0000587e3b58363a (anonymous namespace)::ScalarExprEmitter::Visit(clang::Expr*) CGExprScalar.cpp:0:0
#<!-- -->25 0x0000587e3b59476c (anonymous namespace)::ScalarExprEmitter::VisitCastExpr(clang::CastExpr*) CGExprScalar.cpp:0:0
#<!-- -->26 0x0000587e3b581368 clang::StmtVisitorBase&lt;std::add_pointer, (anonymous namespace)::ScalarExprEmitter, llvm::Value*&gt;::Visit(clang::Stmt*) CGExprScalar.cpp:0:0
#<!-- -->27 0x0000587e3b588fb6 clang::CodeGen::CodeGenFunction::EmitScalarExpr(clang::Expr const*, bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9c85fb6)
#<!-- -->28 0x0000587e3b715249 clang::CodeGen::CodeGenFunction::EmitReturnStmt(clang::ReturnStmt const&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9e12249)
#<!-- -->29 0x0000587e3b720703 clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef&lt;clang::Attr const*&gt;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9e1d703)
#<!-- -->30 0x0000587e3b72a01d clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&amp;, bool, clang::CodeGen::AggValueSlot) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9e2701d)
#<!-- -->31 0x0000587e3b7c40ff clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9ec10ff)
#<!-- -->32 0x0000587e3b7e43f9 clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9ee13f9)
#<!-- -->33 0x0000587e3b88b5e5 clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9f885e5)
#<!-- -->34 0x0000587e3b89107d clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9f8e07d)
#<!-- -->35 0x0000587e3b896278 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9f93278)
#<!-- -->36 0x0000587e3b89736a clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9f9436a)
#<!-- -->37 0x0000587e3bdb0be0 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#<!-- -->38 0x0000587e3bd9a475 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa497475)
#<!-- -->39 0x0000587e3e4749b2 clang::ParseAST(clang::Sema&amp;, bool, bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb719b2)
#<!-- -->40 0x0000587e3c1bc92b clang::ASTFrontendAction::ExecuteAction() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa8b992b)
#<!-- -->41 0x0000587e3bdaa360 clang::CodeGenAction::ExecuteAction() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa4a7360)
#<!-- -->42 0x0000587e3c1c49cf clang::FrontendAction::Execute() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa8c19cf)
#<!-- -->43 0x0000587e3c10af46 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa807f46)
#<!-- -->44 0x0000587e3c34cf7b clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xaa49f7b)
#<!-- -->45 0x0000587e360bb523 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47b8523)
#<!-- -->46 0x0000587e360abc58 ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->47 0x0000587e360b1b28 clang_main(int, char**, llvm::ToolContext const&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47aeb28)
#<!-- -->48 0x0000587e35f0cb4a main (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x4609b4a)
#<!-- -->49 0x00007ae1bacb11ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->50 0x00007ae1bacb128b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->51 0x00007ae1bacb128b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->52 0x0000587e360a9f55 _start (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47a6f55)
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

### Comment 4 - shafik

@codemzs this regression bisects to your PR: e62175736551abf40a3410bc246f58e650eb8158

---

