# `__builtin_assume_aligned` crash on invalid arguments

**Author:** bi6c
**URL:** https://github.com/llvm/llvm-project/issues/110914
**Status:** Closed
**Labels:** clang:frontend, crash-on-invalid, confirmed, regression:19
**Closed Date:** 2024-12-19T10:34:57Z

## Body

Compiler Explorer: https://godbolt.org/z/3vbqjEbqb
```console
<source>:7:49: error: incompatible integer to pointer conversion passing 'int' to parameter of type 'const void *' [-Wint-conversion]
    7 |     unsigned char *y = __builtin_assume_aligned(arg, 16);
      |                                                 ^~~
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -fsanitize=undefined -fsanitize=address --rtlib=compiler-rt -g -lgcc_s -w -O2 <source>
1.	<eof> parser at end of file
2.	<source>:4:6: LLVM IR generation of declaration 'foo'
3.	<source>:4:6: Generating code for declaration 'foo'
 #0 0x0000000003932808 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3932808)
 #1 0x0000000003930974 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3930974)
 #2 0x0000000003882fb8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007786dca42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x0000000003cd953c clang::CodeGen::CodeGenFunction::emitAlignmentAssumptionCheck(llvm::Value*, clang::QualType, clang::SourceLocation, clang::SourceLocation, llvm::Value*, llvm::Value*, llvm::Value*, llvm::Instruction*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3cd953c)
 #5 0x0000000003cd9b83 clang::CodeGen::CodeGenFunction::emitAlignmentAssumption(llvm::Value*, clang::QualType, clang::SourceLocation, clang::SourceLocation, llvm::Value*, llvm::Value*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3cd9b83)
 #6 0x0000000003cda2a4 clang::CodeGen::CodeGenFunction::emitAlignmentAssumption(llvm::Value*, clang::Expr const*, clang::SourceLocation, llvm::Value*, llvm::Value*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3cda2a4)
 #7 0x0000000003ffb847 clang::CodeGen::CodeGenFunction::EmitBuiltinExpr(clang::GlobalDecl, unsigned int, clang::CallExpr const*, clang::CodeGen::ReturnValueSlot) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3ffb847)
 #8 0x00000000040dd556 clang::CodeGen::CodeGenFunction::EmitCallExpr(clang::CallExpr const*, clang::CodeGen::ReturnValueSlot, llvm::CallBase**) (/opt/compiler-explorer/clang-trunk/bin/clang+0x40dd556)
 #9 0x000000000412a4b9 (anonymous namespace)::ScalarExprEmitter::VisitCallExpr(clang::CallExpr const*) CGExprScalar.cpp:0:0
#10 0x0000000004120adc clang::StmtVisitorBase<std::add_pointer, (anonymous namespace)::ScalarExprEmitter, llvm::Value*>::Visit(clang::Stmt*) CGExprScalar.cpp:0:0
#11 0x0000000004122cbb (anonymous namespace)::ScalarExprEmitter::Visit(clang::Expr*) CGExprScalar.cpp:0:0
#12 0x000000000412776b (anonymous namespace)::ScalarExprEmitter::VisitCastExpr(clang::CastExpr*) CGExprScalar.cpp:0:0
#13 0x00000000041212d2 clang::StmtVisitorBase<std::add_pointer, (anonymous namespace)::ScalarExprEmitter, llvm::Value*>::Visit(clang::Stmt*) CGExprScalar.cpp:0:0
#14 0x0000000004125e22 clang::CodeGen::CodeGenFunction::EmitScalarExpr(clang::Expr const*, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+0x4125e22)
#15 0x000000000407f33b clang::CodeGen::CodeGenFunction::EmitScalarInit(clang::Expr const*, clang::ValueDecl const*, clang::CodeGen::LValue, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+0x407f33b)
#16 0x0000000004080663 clang::CodeGen::CodeGenFunction::EmitExprAsInit(clang::Expr const*, clang::ValueDecl const*, clang::CodeGen::LValue, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+0x4080663)
#17 0x000000000408e9fb clang::CodeGen::CodeGenFunction::EmitAutoVarInit(clang::CodeGen::CodeGenFunction::AutoVarEmission const&) (/opt/compiler-explorer/clang-trunk/bin/clang+0x408e9fb)
#18 0x000000000408f6dc clang::CodeGen::CodeGenFunction::EmitVarDecl(clang::VarDecl const&) (.part.0) CGDecl.cpp:0:0
#19 0x000000000408f7a8 clang::CodeGen::CodeGenFunction::EmitDecl(clang::Decl const&) (/opt/compiler-explorer/clang-trunk/bin/clang+0x408f7a8)
#20 0x0000000003c5fe2f clang::CodeGen::CodeGenFunction::EmitDeclStmt(clang::DeclStmt const&) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3c5fe2f)
#21 0x0000000003c7517a clang::CodeGen::CodeGenFunction::EmitSimpleStmt(clang::Stmt const*, llvm::ArrayRef<clang::Attr const*>) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3c7517a)
#22 0x0000000003c6d742 clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef<clang::Attr const*>) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3c6d742)
#23 0x0000000003c74c29 clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&, bool, clang::CodeGen::AggValueSlot) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3c74c29)
#24 0x0000000003cdc36a clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3cdc36a)
#25 0x0000000003ce6f85 clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3ce6f85)
#26 0x0000000003d44beb clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3d44beb)
#27 0x0000000003d40095 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3d40095)
#28 0x0000000003d40ac0 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3d40ac0)
#29 0x0000000003d4a687 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#30 0x000000000425401e (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#31 0x000000000423e469 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/compiler-explorer/clang-trunk/bin/clang+0x423e469)
#32 0x0000000006215ac4 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+0x6215ac4)
#33 0x0000000004251b35 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+0x4251b35)
#34 0x0000000004517f71 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+0x4517f71)
#35 0x0000000004497a8b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-trunk/bin/clang+0x4497a8b)
#36 0x00000000045fc2e3 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x45fc2e3)
#37 0x0000000000cdf87c cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+0xcdf87c)
#38 0x0000000000cd82dd ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#39 0x000000000429a0a9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#40 0x00000000038833e3 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-trunk/bin/clang+0x38833e3)
#41 0x000000000429a2c9 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#42 0x000000000426173d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+0x426173d)
#43 0x000000000426273d clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+0x426273d)
#44 0x000000000426c26c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-trunk/bin/clang+0x426c26c)
#45 0x0000000000cdc5d1 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-trunk/bin/clang+0xcdc5d1)
#46 0x0000000000baf834 main (/opt/compiler-explorer/clang-trunk/bin/clang+0xbaf834)
#47 0x00007786dca29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#48 0x00007786dca29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#49 0x0000000000cd7d7e _start (/opt/compiler-explorer/clang-trunk/bin/clang+0xcd7d7e)
clang: error: clang frontend command failed with exit code 139 (use -v to see invocation)
ASM generation compiler returned: 139
<source>:7:49: error: incompatible integer to pointer conversion passing 'int' to parameter of type 'const void *' [-Wint-conversion]
    7 |     unsigned char *y = __builtin_assume_aligned(arg, 16);
      |                                                 ^~~
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk-20241001/bin/clang-20 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -dumpdir /app/output.s- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name example.c -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=none -fmath-errno -ffp-contract=on -fno-rounding-math -fno-verbose-asm -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debug-info-kind=constructor -dwarf-version=4 -debugger-tuning=gdb -fdebug-compilation-dir=/app -fcoverage-compilation-dir=/app -resource-dir /opt/compiler-explorer/clang-trunk-20241001/lib/clang/20 -internal-isystem /opt/compiler-explorer/clang-trunk-20241001/lib/clang/20/include -internal-isystem /usr/local/include -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/15.0.0/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -O2 -w -ferror-limit 19 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -vectorize-loops -vectorize-slp -fsanitize=address,alignment,array-bounds,bool,builtin,enum,float-cast-overflow,function,integer-divide-by-zero,nonnull-attribute,null,object-size,pointer-overflow,return,returns-nonnull-attribute,shift-base,shift-exponent,signed-integer-overflow,unreachable,vla-bound,vptr -fsanitize-recover=alignment,array-bounds,bool,builtin,enum,float-cast-overflow,function,integer-divide-by-zero,nonnull-attribute,null,object-size,pointer-overflow,returns-nonnull-attribute,shift-base,shift-exponent,signed-integer-overflow,vla-bound,vptr -fsanitize-system-ignorelist=/opt/compiler-explorer/clang-trunk-20241001/lib/clang/20/share/asan_ignorelist.txt -fno-sanitize-memory-param-retval -fsanitize-address-use-after-scope -fsanitize-address-globals-dead-stripping -fno-assume-sane-operator-new -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/example-a27a44.o -x c <source>
1.	<eof> parser at end of file
2.	<source>:4:6: LLVM IR generation of declaration 'foo'
3.	<source>:4:6: Generating code for declaration 'foo'
 #0 0x0000000003932808 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x3932808)
 #1 0x00000000039303ec SignalHandler(int) Signals.cpp:0:0
 #2 0x00007de5a0842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x0000000003cd953c clang::CodeGen::CodeGenFunction::emitAlignmentAssumptionCheck(llvm::Value*, clang::QualType, clang::SourceLocation, clang::SourceLocation, llvm::Value*, llvm::Value*, llvm::Value*, llvm::Instruction*) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x3cd953c)
 #4 0x0000000003cd9b83 clang::CodeGen::CodeGenFunction::emitAlignmentAssumption(llvm::Value*, clang::QualType, clang::SourceLocation, clang::SourceLocation, llvm::Value*, llvm::Value*) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x3cd9b83)
 #5 0x0000000003cda2a4 clang::CodeGen::CodeGenFunction::emitAlignmentAssumption(llvm::Value*, clang::Expr const*, clang::SourceLocation, llvm::Value*, llvm::Value*) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x3cda2a4)
 #6 0x0000000003ffb847 clang::CodeGen::CodeGenFunction::EmitBuiltinExpr(clang::GlobalDecl, unsigned int, clang::CallExpr const*, clang::CodeGen::ReturnValueSlot) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x3ffb847)
 #7 0x00000000040dd556 clang::CodeGen::CodeGenFunction::EmitCallExpr(clang::CallExpr const*, clang::CodeGen::ReturnValueSlot, llvm::CallBase**) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x40dd556)
 #8 0x000000000412a4b9 (anonymous namespace)::ScalarExprEmitter::VisitCallExpr(clang::CallExpr const*) CGExprScalar.cpp:0:0
 #9 0x0000000004120adc clang::StmtVisitorBase<std::add_pointer, (anonymous namespace)::ScalarExprEmitter, llvm::Value*>::Visit(clang::Stmt*) CGExprScalar.cpp:0:0
#10 0x0000000004122cbb (anonymous namespace)::ScalarExprEmitter::Visit(clang::Expr*) CGExprScalar.cpp:0:0
#11 0x000000000412776b (anonymous namespace)::ScalarExprEmitter::VisitCastExpr(clang::CastExpr*) CGExprScalar.cpp:0:0
#12 0x00000000041212d2 clang::StmtVisitorBase<std::add_pointer, (anonymous namespace)::ScalarExprEmitter, llvm::Value*>::Visit(clang::Stmt*) CGExprScalar.cpp:0:0
#13 0x0000000004125e22 clang::CodeGen::CodeGenFunction::EmitScalarExpr(clang::Expr const*, bool) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x4125e22)
#14 0x000000000407f33b clang::CodeGen::CodeGenFunction::EmitScalarInit(clang::Expr const*, clang::ValueDecl const*, clang::CodeGen::LValue, bool) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x407f33b)
#15 0x0000000004080663 clang::CodeGen::CodeGenFunction::EmitExprAsInit(clang::Expr const*, clang::ValueDecl const*, clang::CodeGen::LValue, bool) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x4080663)
#16 0x000000000408e9fb clang::CodeGen::CodeGenFunction::EmitAutoVarInit(clang::CodeGen::CodeGenFunction::AutoVarEmission const&) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x408e9fb)
#17 0x000000000408f6dc clang::CodeGen::CodeGenFunction::EmitVarDecl(clang::VarDecl const&) (.part.0) CGDecl.cpp:0:0
#18 0x000000000408f7a8 clang::CodeGen::CodeGenFunction::EmitDecl(clang::Decl const&) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x408f7a8)
#19 0x0000000003c5fe2f clang::CodeGen::CodeGenFunction::EmitDeclStmt(clang::DeclStmt const&) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x3c5fe2f)
#20 0x0000000003c7517a clang::CodeGen::CodeGenFunction::EmitSimpleStmt(clang::Stmt const*, llvm::ArrayRef<clang::Attr const*>) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x3c7517a)
#21 0x0000000003c6d742 clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef<clang::Attr const*>) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x3c6d742)
#22 0x0000000003c74c29 clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&, bool, clang::CodeGen::AggValueSlot) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x3c74c29)
#23 0x0000000003cdc36a clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x3cdc36a)
#24 0x0000000003ce6f85 clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x3ce6f85)
#25 0x0000000003d44beb clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x3d44beb)
#26 0x0000000003d40095 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x3d40095)
#27 0x0000000003d40ac0 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x3d40ac0)
#28 0x0000000003d4a687 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#29 0x000000000425401e (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#30 0x000000000423e469 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x423e469)
#31 0x0000000006215ac4 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x6215ac4)
#32 0x0000000004251b35 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x4251b35)
#33 0x0000000004517f71 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x4517f71)
#34 0x0000000004497a8b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x4497a8b)
#35 0x00000000045fc2e3 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x45fc2e3)
#36 0x0000000000cdf87c cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0xcdf87c)
#37 0x0000000000cd82dd ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#38 0x0000000000cdc44f clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0xcdc44f)
#39 0x0000000000baf834 main (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0xbaf834)
#40 0x00007de5a0829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#41 0x00007de5a0829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#42 0x0000000000cd7d7e _start (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0xcd7d7e)
clang: error: unable to execute command: Segmentation fault (core dumped)
clang: error: clang frontend command failed due to signal (use -v to see invocation)
Execution build compiler returned: 254
```
Code:
```c

#include <stdio.h>

void foo(int arg) {
    int x = 0;

    unsigned char *y = __builtin_assume_aligned(arg, 16);

    for (int i = 0; i < 8; ++i) {
        x = y[i];
        printf("Value of x in the loop: %d\n", x);
    }
    
    return;
}

```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: None (bi6c)

<details>
Compiler Explorer: https://godbolt.org/z/3vbqjEbqb
```console
&lt;source&gt;:7:49: error: incompatible integer to pointer conversion passing 'int' to parameter of type 'const void *' [-Wint-conversion]
    7 |     unsigned char *y = __builtin_assume_aligned(arg, 16);
      |                                                 ^~~
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -fsanitize=undefined -fsanitize=address --rtlib=compiler-rt -g -lgcc_s -w -O2 &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	&lt;source&gt;:4:6: LLVM IR generation of declaration 'foo'
3.	&lt;source&gt;:4:6: Generating code for declaration 'foo'
 #<!-- -->0 0x0000000003932808 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3932808)
 #<!-- -->1 0x0000000003930974 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3930974)
 #<!-- -->2 0x0000000003882fb8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007786dca42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x0000000003cd953c clang::CodeGen::CodeGenFunction::emitAlignmentAssumptionCheck(llvm::Value*, clang::QualType, clang::SourceLocation, clang::SourceLocation, llvm::Value*, llvm::Value*, llvm::Value*, llvm::Instruction*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3cd953c)
 #<!-- -->5 0x0000000003cd9b83 clang::CodeGen::CodeGenFunction::emitAlignmentAssumption(llvm::Value*, clang::QualType, clang::SourceLocation, clang::SourceLocation, llvm::Value*, llvm::Value*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3cd9b83)
 #<!-- -->6 0x0000000003cda2a4 clang::CodeGen::CodeGenFunction::emitAlignmentAssumption(llvm::Value*, clang::Expr const*, clang::SourceLocation, llvm::Value*, llvm::Value*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3cda2a4)
 #<!-- -->7 0x0000000003ffb847 clang::CodeGen::CodeGenFunction::EmitBuiltinExpr(clang::GlobalDecl, unsigned int, clang::CallExpr const*, clang::CodeGen::ReturnValueSlot) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3ffb847)
 #<!-- -->8 0x00000000040dd556 clang::CodeGen::CodeGenFunction::EmitCallExpr(clang::CallExpr const*, clang::CodeGen::ReturnValueSlot, llvm::CallBase**) (/opt/compiler-explorer/clang-trunk/bin/clang+0x40dd556)
 #<!-- -->9 0x000000000412a4b9 (anonymous namespace)::ScalarExprEmitter::VisitCallExpr(clang::CallExpr const*) CGExprScalar.cpp:0:0
#<!-- -->10 0x0000000004120adc clang::StmtVisitorBase&lt;std::add_pointer, (anonymous namespace)::ScalarExprEmitter, llvm::Value*&gt;::Visit(clang::Stmt*) CGExprScalar.cpp:0:0
#<!-- -->11 0x0000000004122cbb (anonymous namespace)::ScalarExprEmitter::Visit(clang::Expr*) CGExprScalar.cpp:0:0
#<!-- -->12 0x000000000412776b (anonymous namespace)::ScalarExprEmitter::VisitCastExpr(clang::CastExpr*) CGExprScalar.cpp:0:0
#<!-- -->13 0x00000000041212d2 clang::StmtVisitorBase&lt;std::add_pointer, (anonymous namespace)::ScalarExprEmitter, llvm::Value*&gt;::Visit(clang::Stmt*) CGExprScalar.cpp:0:0
#<!-- -->14 0x0000000004125e22 clang::CodeGen::CodeGenFunction::EmitScalarExpr(clang::Expr const*, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+0x4125e22)
#<!-- -->15 0x000000000407f33b clang::CodeGen::CodeGenFunction::EmitScalarInit(clang::Expr const*, clang::ValueDecl const*, clang::CodeGen::LValue, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+0x407f33b)
#<!-- -->16 0x0000000004080663 clang::CodeGen::CodeGenFunction::EmitExprAsInit(clang::Expr const*, clang::ValueDecl const*, clang::CodeGen::LValue, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+0x4080663)
#<!-- -->17 0x000000000408e9fb clang::CodeGen::CodeGenFunction::EmitAutoVarInit(clang::CodeGen::CodeGenFunction::AutoVarEmission const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x408e9fb)
#<!-- -->18 0x000000000408f6dc clang::CodeGen::CodeGenFunction::EmitVarDecl(clang::VarDecl const&amp;) (.part.0) CGDecl.cpp:0:0
#<!-- -->19 0x000000000408f7a8 clang::CodeGen::CodeGenFunction::EmitDecl(clang::Decl const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x408f7a8)
#<!-- -->20 0x0000000003c5fe2f clang::CodeGen::CodeGenFunction::EmitDeclStmt(clang::DeclStmt const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3c5fe2f)
#<!-- -->21 0x0000000003c7517a clang::CodeGen::CodeGenFunction::EmitSimpleStmt(clang::Stmt const*, llvm::ArrayRef&lt;clang::Attr const*&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3c7517a)
#<!-- -->22 0x0000000003c6d742 clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef&lt;clang::Attr const*&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3c6d742)
#<!-- -->23 0x0000000003c74c29 clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&amp;, bool, clang::CodeGen::AggValueSlot) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3c74c29)
#<!-- -->24 0x0000000003cdc36a clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3cdc36a)
#<!-- -->25 0x0000000003ce6f85 clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3ce6f85)
#<!-- -->26 0x0000000003d44beb clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3d44beb)
#<!-- -->27 0x0000000003d40095 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3d40095)
#<!-- -->28 0x0000000003d40ac0 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3d40ac0)
#<!-- -->29 0x0000000003d4a687 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#<!-- -->30 0x000000000425401e (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#<!-- -->31 0x000000000423e469 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/compiler-explorer/clang-trunk/bin/clang+0x423e469)
#<!-- -->32 0x0000000006215ac4 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+0x6215ac4)
#<!-- -->33 0x0000000004251b35 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+0x4251b35)
#<!-- -->34 0x0000000004517f71 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+0x4517f71)
#<!-- -->35 0x0000000004497a8b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x4497a8b)
#<!-- -->36 0x00000000045fc2e3 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x45fc2e3)
#<!-- -->37 0x0000000000cdf87c cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+0xcdf87c)
#<!-- -->38 0x0000000000cd82dd ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->39 0x000000000429a0a9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->40 0x00000000038833e3 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x38833e3)
#<!-- -->41 0x000000000429a2c9 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->42 0x000000000426173d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+0x426173d)
#<!-- -->43 0x000000000426273d clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+0x426273d)
#<!-- -->44 0x000000000426c26c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x426c26c)
#<!-- -->45 0x0000000000cdc5d1 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0xcdc5d1)
#<!-- -->46 0x0000000000baf834 main (/opt/compiler-explorer/clang-trunk/bin/clang+0xbaf834)
#<!-- -->47 0x00007786dca29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->48 0x00007786dca29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->49 0x0000000000cd7d7e _start (/opt/compiler-explorer/clang-trunk/bin/clang+0xcd7d7e)
clang: error: clang frontend command failed with exit code 139 (use -v to see invocation)
ASM generation compiler returned: 139
&lt;source&gt;:7:49: error: incompatible integer to pointer conversion passing 'int' to parameter of type 'const void *' [-Wint-conversion]
    7 |     unsigned char *y = __builtin_assume_aligned(arg, 16);
      |                                                 ^~~
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk-20241001/bin/clang-20 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -dumpdir /app/output.s- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name example.c -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=none -fmath-errno -ffp-contract=on -fno-rounding-math -fno-verbose-asm -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debug-info-kind=constructor -dwarf-version=4 -debugger-tuning=gdb -fdebug-compilation-dir=/app -fcoverage-compilation-dir=/app -resource-dir /opt/compiler-explorer/clang-trunk-20241001/lib/clang/20 -internal-isystem /opt/compiler-explorer/clang-trunk-20241001/lib/clang/20/include -internal-isystem /usr/local/include -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/15.0.0/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -O2 -w -ferror-limit 19 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -vectorize-loops -vectorize-slp -fsanitize=address,alignment,array-bounds,bool,builtin,enum,float-cast-overflow,function,integer-divide-by-zero,nonnull-attribute,null,object-size,pointer-overflow,return,returns-nonnull-attribute,shift-base,shift-exponent,signed-integer-overflow,unreachable,vla-bound,vptr -fsanitize-recover=alignment,array-bounds,bool,builtin,enum,float-cast-overflow,function,integer-divide-by-zero,nonnull-attribute,null,object-size,pointer-overflow,returns-nonnull-attribute,shift-base,shift-exponent,signed-integer-overflow,vla-bound,vptr -fsanitize-system-ignorelist=/opt/compiler-explorer/clang-trunk-20241001/lib/clang/20/share/asan_ignorelist.txt -fno-sanitize-memory-param-retval -fsanitize-address-use-after-scope -fsanitize-address-globals-dead-stripping -fno-assume-sane-operator-new -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/example-a27a44.o -x c &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	&lt;source&gt;:4:6: LLVM IR generation of declaration 'foo'
3.	&lt;source&gt;:4:6: Generating code for declaration 'foo'
 #<!-- -->0 0x0000000003932808 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x3932808)
 #<!-- -->1 0x00000000039303ec SignalHandler(int) Signals.cpp:0:0
 #<!-- -->2 0x00007de5a0842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->3 0x0000000003cd953c clang::CodeGen::CodeGenFunction::emitAlignmentAssumptionCheck(llvm::Value*, clang::QualType, clang::SourceLocation, clang::SourceLocation, llvm::Value*, llvm::Value*, llvm::Value*, llvm::Instruction*) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x3cd953c)
 #<!-- -->4 0x0000000003cd9b83 clang::CodeGen::CodeGenFunction::emitAlignmentAssumption(llvm::Value*, clang::QualType, clang::SourceLocation, clang::SourceLocation, llvm::Value*, llvm::Value*) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x3cd9b83)
 #<!-- -->5 0x0000000003cda2a4 clang::CodeGen::CodeGenFunction::emitAlignmentAssumption(llvm::Value*, clang::Expr const*, clang::SourceLocation, llvm::Value*, llvm::Value*) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x3cda2a4)
 #<!-- -->6 0x0000000003ffb847 clang::CodeGen::CodeGenFunction::EmitBuiltinExpr(clang::GlobalDecl, unsigned int, clang::CallExpr const*, clang::CodeGen::ReturnValueSlot) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x3ffb847)
 #<!-- -->7 0x00000000040dd556 clang::CodeGen::CodeGenFunction::EmitCallExpr(clang::CallExpr const*, clang::CodeGen::ReturnValueSlot, llvm::CallBase**) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x40dd556)
 #<!-- -->8 0x000000000412a4b9 (anonymous namespace)::ScalarExprEmitter::VisitCallExpr(clang::CallExpr const*) CGExprScalar.cpp:0:0
 #<!-- -->9 0x0000000004120adc clang::StmtVisitorBase&lt;std::add_pointer, (anonymous namespace)::ScalarExprEmitter, llvm::Value*&gt;::Visit(clang::Stmt*) CGExprScalar.cpp:0:0
#<!-- -->10 0x0000000004122cbb (anonymous namespace)::ScalarExprEmitter::Visit(clang::Expr*) CGExprScalar.cpp:0:0
#<!-- -->11 0x000000000412776b (anonymous namespace)::ScalarExprEmitter::VisitCastExpr(clang::CastExpr*) CGExprScalar.cpp:0:0
#<!-- -->12 0x00000000041212d2 clang::StmtVisitorBase&lt;std::add_pointer, (anonymous namespace)::ScalarExprEmitter, llvm::Value*&gt;::Visit(clang::Stmt*) CGExprScalar.cpp:0:0
#<!-- -->13 0x0000000004125e22 clang::CodeGen::CodeGenFunction::EmitScalarExpr(clang::Expr const*, bool) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x4125e22)
#<!-- -->14 0x000000000407f33b clang::CodeGen::CodeGenFunction::EmitScalarInit(clang::Expr const*, clang::ValueDecl const*, clang::CodeGen::LValue, bool) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x407f33b)
#<!-- -->15 0x0000000004080663 clang::CodeGen::CodeGenFunction::EmitExprAsInit(clang::Expr const*, clang::ValueDecl const*, clang::CodeGen::LValue, bool) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x4080663)
#<!-- -->16 0x000000000408e9fb clang::CodeGen::CodeGenFunction::EmitAutoVarInit(clang::CodeGen::CodeGenFunction::AutoVarEmission const&amp;) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x408e9fb)
#<!-- -->17 0x000000000408f6dc clang::CodeGen::CodeGenFunction::EmitVarDecl(clang::VarDecl const&amp;) (.part.0) CGDecl.cpp:0:0
#<!-- -->18 0x000000000408f7a8 clang::CodeGen::CodeGenFunction::EmitDecl(clang::Decl const&amp;) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x408f7a8)
#<!-- -->19 0x0000000003c5fe2f clang::CodeGen::CodeGenFunction::EmitDeclStmt(clang::DeclStmt const&amp;) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x3c5fe2f)
#<!-- -->20 0x0000000003c7517a clang::CodeGen::CodeGenFunction::EmitSimpleStmt(clang::Stmt const*, llvm::ArrayRef&lt;clang::Attr const*&gt;) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x3c7517a)
#<!-- -->21 0x0000000003c6d742 clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef&lt;clang::Attr const*&gt;) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x3c6d742)
#<!-- -->22 0x0000000003c74c29 clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&amp;, bool, clang::CodeGen::AggValueSlot) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x3c74c29)
#<!-- -->23 0x0000000003cdc36a clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x3cdc36a)
#<!-- -->24 0x0000000003ce6f85 clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&amp;) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x3ce6f85)
#<!-- -->25 0x0000000003d44beb clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x3d44beb)
#<!-- -->26 0x0000000003d40095 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x3d40095)
#<!-- -->27 0x0000000003d40ac0 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x3d40ac0)
#<!-- -->28 0x0000000003d4a687 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#<!-- -->29 0x000000000425401e (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#<!-- -->30 0x000000000423e469 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x423e469)
#<!-- -->31 0x0000000006215ac4 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x6215ac4)
#<!-- -->32 0x0000000004251b35 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x4251b35)
#<!-- -->33 0x0000000004517f71 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x4517f71)
#<!-- -->34 0x0000000004497a8b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x4497a8b)
#<!-- -->35 0x00000000045fc2e3 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x45fc2e3)
#<!-- -->36 0x0000000000cdf87c cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0xcdf87c)
#<!-- -->37 0x0000000000cd82dd ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->38 0x0000000000cdc44f clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0xcdc44f)
#<!-- -->39 0x0000000000baf834 main (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0xbaf834)
#<!-- -->40 0x00007de5a0829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->41 0x00007de5a0829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->42 0x0000000000cd7d7e _start (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0xcd7d7e)
clang: error: unable to execute command: Segmentation fault (core dumped)
clang: error: clang frontend command failed due to signal (use -v to see invocation)
Execution build compiler returned: 254
```
Code:
```c

#include &lt;stdio.h&gt;

void foo(int arg) {
    int x = 0;

    unsigned char *y = __builtin_assume_aligned(arg, 16);

    for (int i = 0; i &lt; 8; ++i) {
        x = y[i];
        printf("Value of x in the loop: %d\n", x);
    }
    
    return;
}

```
</details>


---

### Comment 2 - shafik

This looks like a clang-19 regression and if we use assertions build it looks like it is crashes in the front-end: https://godbolt.org/z/rdf8MoTno

Assertion:

```console
clang++: /root/llvm-project/clang/lib/AST/ExprConstant.cpp:9201:
bool EvaluatePointer(const clang::Expr*, {anonymous}::LValue&, {anonymous}::EvalInfo&, bool): 
Assertion `E->isPRValue() && E->getType()->hasPointerRepresentation()' failed.
```

Backtrace:

```console
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-19.1.0/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-14.2.0 -fcolor-diagnostics -fno-crash-diagnostics -x c <source>
1.	<eof> parser at end of file
2.	<source>:4:6: LLVM IR generation of declaration 'foo'
3.	<source>:4:6: Generating code for declaration 'foo'
 #0 0x0000000003ae9978 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x3ae9978)
 #1 0x0000000003ae766c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x3ae766c)
 #2 0x0000000003a308f8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000076e678442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x000076e6784969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x000076e678442476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x000076e6784287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x000076e67842871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x000076e678439e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x00000000078b07ea EvaluatePointer(clang::Expr const*, (anonymous namespace)::LValue&, (anonymous namespace)::EvalInfo&, bool) ExprConstant.cpp:0:0
#10 0x00000000078f97bd (anonymous namespace)::PointerExprEvaluator::VisitBuiltinCallExpr(clang::CallExpr const*, unsigned int) ExprConstant.cpp:0:0
#11 0x00000000078fb032 (anonymous namespace)::PointerExprEvaluator::VisitCallExpr(clang::CallExpr const*) ExprConstant.cpp:0:0
#12 0x00000000078aec26 clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::PointerExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#13 0x00000000078b081b EvaluatePointer(clang::Expr const*, (anonymous namespace)::LValue&, (anonymous namespace)::EvalInfo&, bool) ExprConstant.cpp:0:0
#14 0x0000000007899cdd Evaluate(clang::APValue&, (anonymous namespace)::EvalInfo&, clang::Expr const*) ExprConstant.cpp:0:0
#15 0x00000000078a2afb EvaluateAsRValue((anonymous namespace)::EvalInfo&, clang::Expr const*, clang::APValue&) ExprConstant.cpp:0:0
#16 0x00000000078a5121 clang::Expr::EvaluateAsRValue(clang::Expr::EvalResult&, clang::ASTContext const&, bool) const (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x78a5121)
#17 0x000000000419e678 clang::CodeGen::CodeGenFunction::EmitBuiltinExpr(clang::GlobalDecl, unsigned int, clang::CallExpr const*, clang::CodeGen::ReturnValueSlot) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x419e678)
#18 0x0000000004283dcc clang::CodeGen::CodeGenFunction::EmitCallExpr(clang::CallExpr const*, clang::CodeGen::ReturnValueSlot) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x4283dcc)
#19 0x00000000042d4c62 (anonymous namespace)::ScalarExprEmitter::VisitCallExpr(clang::CallExpr const*) CGExprScalar.cpp:0:0
#20 0x00000000042cb3e0 clang::StmtVisitorBase<std::add_pointer, (anonymous namespace)::ScalarExprEmitter, llvm::Value*>::Visit(clang::Stmt*) CGExprScalar.cpp:0:0
#21 0x00000000042cd3ab (anonymous namespace)::ScalarExprEmitter::Visit(clang::Expr*) CGExprScalar.cpp:0:0
#22 0x00000000042d1d43 (anonymous namespace)::ScalarExprEmitter::VisitCastExpr(clang::CastExpr*) CGExprScalar.cpp:0:0
#23 0x00000000042cb837 clang::StmtVisitorBase<std::add_pointer, (anonymous namespace)::ScalarExprEmitter, llvm::Value*>::Visit(clang::Stmt*) CGExprScalar.cpp:0:0
#24 0x00000000042d10bc clang::CodeGen::CodeGenFunction::EmitScalarExpr(clang::Expr const*, bool) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x42d10bc)
#25 0x000000000422515f clang::CodeGen::CodeGenFunction::EmitScalarInit(clang::Expr const*, clang::ValueDecl const*, clang::CodeGen::LValue, bool) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x422515f)
#26 0x0000000004226994 clang::CodeGen::CodeGenFunction::EmitExprAsInit(clang::Expr const*, clang::ValueDecl const*, clang::CodeGen::LValue, bool) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x4226994)
#27 0x00000000042330e1 clang::CodeGen::CodeGenFunction::EmitAutoVarInit(clang::CodeGen::CodeGenFunction::AutoVarEmission const&) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x42330e1)
#28 0x0000000004234f96 clang::CodeGen::CodeGenFunction::EmitVarDecl(clang::VarDecl const&) (.part.0) CGDecl.cpp:0:0
#29 0x00000000042351e6 clang::CodeGen::CodeGenFunction::EmitDecl(clang::Decl const&) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x42351e6)
#30 0x0000000003e3bb77 clang::CodeGen::CodeGenFunction::EmitDeclStmt(clang::DeclStmt const&) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x3e3bb77)
#31 0x0000000003e522f9 clang::CodeGen::CodeGenFunction::EmitSimpleStmt(clang::Stmt const*, llvm::ArrayRef<clang::Attr const*>) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x3e522f9)
#32 0x0000000003e4a025 clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef<clang::Attr const*>) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x3e4a025)
#33 0x0000000003e51d14 clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&, bool, clang::CodeGen::AggValueSlot) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x3e51d14)
#34 0x0000000003eb8b6e clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x3eb8b6e)
#35 0x0000000003ecda4d clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x3ecda4d)
#36 0x0000000003f2de52 clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x3f2de52)
#37 0x0000000003f28d55 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x3f28d55)
#38 0x0000000003f292fb clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x3f292fb)
#39 0x0000000003f336b3 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#40 0x00000000043fb906 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#41 0x00000000043ebe68 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x43ebe68)
#42 0x00000000064cc324 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x64cc324)
#43 0x00000000043f9448 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x43f9448)
#44 0x0000000004684f59 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x4684f59)
#45 0x0000000004609f8e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x4609f8e)
#46 0x000000000476e2be clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x476e2be)
#47 0x0000000000c82b7f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0xc82b7f)
#48 0x0000000000c7bcda ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#49 0x000000000443cba9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#50 0x0000000003a30da4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x3a30da4)
#51 0x000000000443d19f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#52 0x0000000004402c05 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x4402c05)
#53 0x000000000440366d clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x440366d)
#54 0x000000000440b075 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x440b075)
#55 0x0000000000c7fe95 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0xc7fe95)
#56 0x0000000000b547a4 main (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0xb547a4)
#57 0x000076e678429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#58 0x000076e678429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#59 0x0000000000c7b78e _start (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0xc7b78e)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

---

### Comment 3 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (bi6c)

<details>
Compiler Explorer: https://godbolt.org/z/3vbqjEbqb
```console
&lt;source&gt;:7:49: error: incompatible integer to pointer conversion passing 'int' to parameter of type 'const void *' [-Wint-conversion]
    7 |     unsigned char *y = __builtin_assume_aligned(arg, 16);
      |                                                 ^~~
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -fsanitize=undefined -fsanitize=address --rtlib=compiler-rt -g -lgcc_s -w -O2 &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	&lt;source&gt;:4:6: LLVM IR generation of declaration 'foo'
3.	&lt;source&gt;:4:6: Generating code for declaration 'foo'
 #<!-- -->0 0x0000000003932808 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3932808)
 #<!-- -->1 0x0000000003930974 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3930974)
 #<!-- -->2 0x0000000003882fb8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007786dca42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x0000000003cd953c clang::CodeGen::CodeGenFunction::emitAlignmentAssumptionCheck(llvm::Value*, clang::QualType, clang::SourceLocation, clang::SourceLocation, llvm::Value*, llvm::Value*, llvm::Value*, llvm::Instruction*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3cd953c)
 #<!-- -->5 0x0000000003cd9b83 clang::CodeGen::CodeGenFunction::emitAlignmentAssumption(llvm::Value*, clang::QualType, clang::SourceLocation, clang::SourceLocation, llvm::Value*, llvm::Value*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3cd9b83)
 #<!-- -->6 0x0000000003cda2a4 clang::CodeGen::CodeGenFunction::emitAlignmentAssumption(llvm::Value*, clang::Expr const*, clang::SourceLocation, llvm::Value*, llvm::Value*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3cda2a4)
 #<!-- -->7 0x0000000003ffb847 clang::CodeGen::CodeGenFunction::EmitBuiltinExpr(clang::GlobalDecl, unsigned int, clang::CallExpr const*, clang::CodeGen::ReturnValueSlot) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3ffb847)
 #<!-- -->8 0x00000000040dd556 clang::CodeGen::CodeGenFunction::EmitCallExpr(clang::CallExpr const*, clang::CodeGen::ReturnValueSlot, llvm::CallBase**) (/opt/compiler-explorer/clang-trunk/bin/clang+0x40dd556)
 #<!-- -->9 0x000000000412a4b9 (anonymous namespace)::ScalarExprEmitter::VisitCallExpr(clang::CallExpr const*) CGExprScalar.cpp:0:0
#<!-- -->10 0x0000000004120adc clang::StmtVisitorBase&lt;std::add_pointer, (anonymous namespace)::ScalarExprEmitter, llvm::Value*&gt;::Visit(clang::Stmt*) CGExprScalar.cpp:0:0
#<!-- -->11 0x0000000004122cbb (anonymous namespace)::ScalarExprEmitter::Visit(clang::Expr*) CGExprScalar.cpp:0:0
#<!-- -->12 0x000000000412776b (anonymous namespace)::ScalarExprEmitter::VisitCastExpr(clang::CastExpr*) CGExprScalar.cpp:0:0
#<!-- -->13 0x00000000041212d2 clang::StmtVisitorBase&lt;std::add_pointer, (anonymous namespace)::ScalarExprEmitter, llvm::Value*&gt;::Visit(clang::Stmt*) CGExprScalar.cpp:0:0
#<!-- -->14 0x0000000004125e22 clang::CodeGen::CodeGenFunction::EmitScalarExpr(clang::Expr const*, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+0x4125e22)
#<!-- -->15 0x000000000407f33b clang::CodeGen::CodeGenFunction::EmitScalarInit(clang::Expr const*, clang::ValueDecl const*, clang::CodeGen::LValue, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+0x407f33b)
#<!-- -->16 0x0000000004080663 clang::CodeGen::CodeGenFunction::EmitExprAsInit(clang::Expr const*, clang::ValueDecl const*, clang::CodeGen::LValue, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+0x4080663)
#<!-- -->17 0x000000000408e9fb clang::CodeGen::CodeGenFunction::EmitAutoVarInit(clang::CodeGen::CodeGenFunction::AutoVarEmission const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x408e9fb)
#<!-- -->18 0x000000000408f6dc clang::CodeGen::CodeGenFunction::EmitVarDecl(clang::VarDecl const&amp;) (.part.0) CGDecl.cpp:0:0
#<!-- -->19 0x000000000408f7a8 clang::CodeGen::CodeGenFunction::EmitDecl(clang::Decl const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x408f7a8)
#<!-- -->20 0x0000000003c5fe2f clang::CodeGen::CodeGenFunction::EmitDeclStmt(clang::DeclStmt const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3c5fe2f)
#<!-- -->21 0x0000000003c7517a clang::CodeGen::CodeGenFunction::EmitSimpleStmt(clang::Stmt const*, llvm::ArrayRef&lt;clang::Attr const*&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3c7517a)
#<!-- -->22 0x0000000003c6d742 clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef&lt;clang::Attr const*&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3c6d742)
#<!-- -->23 0x0000000003c74c29 clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&amp;, bool, clang::CodeGen::AggValueSlot) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3c74c29)
#<!-- -->24 0x0000000003cdc36a clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3cdc36a)
#<!-- -->25 0x0000000003ce6f85 clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3ce6f85)
#<!-- -->26 0x0000000003d44beb clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3d44beb)
#<!-- -->27 0x0000000003d40095 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3d40095)
#<!-- -->28 0x0000000003d40ac0 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3d40ac0)
#<!-- -->29 0x0000000003d4a687 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#<!-- -->30 0x000000000425401e (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#<!-- -->31 0x000000000423e469 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/compiler-explorer/clang-trunk/bin/clang+0x423e469)
#<!-- -->32 0x0000000006215ac4 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+0x6215ac4)
#<!-- -->33 0x0000000004251b35 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+0x4251b35)
#<!-- -->34 0x0000000004517f71 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+0x4517f71)
#<!-- -->35 0x0000000004497a8b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x4497a8b)
#<!-- -->36 0x00000000045fc2e3 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x45fc2e3)
#<!-- -->37 0x0000000000cdf87c cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+0xcdf87c)
#<!-- -->38 0x0000000000cd82dd ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->39 0x000000000429a0a9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->40 0x00000000038833e3 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x38833e3)
#<!-- -->41 0x000000000429a2c9 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->42 0x000000000426173d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+0x426173d)
#<!-- -->43 0x000000000426273d clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+0x426273d)
#<!-- -->44 0x000000000426c26c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x426c26c)
#<!-- -->45 0x0000000000cdc5d1 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0xcdc5d1)
#<!-- -->46 0x0000000000baf834 main (/opt/compiler-explorer/clang-trunk/bin/clang+0xbaf834)
#<!-- -->47 0x00007786dca29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->48 0x00007786dca29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->49 0x0000000000cd7d7e _start (/opt/compiler-explorer/clang-trunk/bin/clang+0xcd7d7e)
clang: error: clang frontend command failed with exit code 139 (use -v to see invocation)
ASM generation compiler returned: 139
&lt;source&gt;:7:49: error: incompatible integer to pointer conversion passing 'int' to parameter of type 'const void *' [-Wint-conversion]
    7 |     unsigned char *y = __builtin_assume_aligned(arg, 16);
      |                                                 ^~~
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk-20241001/bin/clang-20 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -dumpdir /app/output.s- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name example.c -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=none -fmath-errno -ffp-contract=on -fno-rounding-math -fno-verbose-asm -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debug-info-kind=constructor -dwarf-version=4 -debugger-tuning=gdb -fdebug-compilation-dir=/app -fcoverage-compilation-dir=/app -resource-dir /opt/compiler-explorer/clang-trunk-20241001/lib/clang/20 -internal-isystem /opt/compiler-explorer/clang-trunk-20241001/lib/clang/20/include -internal-isystem /usr/local/include -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/15.0.0/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -O2 -w -ferror-limit 19 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -vectorize-loops -vectorize-slp -fsanitize=address,alignment,array-bounds,bool,builtin,enum,float-cast-overflow,function,integer-divide-by-zero,nonnull-attribute,null,object-size,pointer-overflow,return,returns-nonnull-attribute,shift-base,shift-exponent,signed-integer-overflow,unreachable,vla-bound,vptr -fsanitize-recover=alignment,array-bounds,bool,builtin,enum,float-cast-overflow,function,integer-divide-by-zero,nonnull-attribute,null,object-size,pointer-overflow,returns-nonnull-attribute,shift-base,shift-exponent,signed-integer-overflow,vla-bound,vptr -fsanitize-system-ignorelist=/opt/compiler-explorer/clang-trunk-20241001/lib/clang/20/share/asan_ignorelist.txt -fno-sanitize-memory-param-retval -fsanitize-address-use-after-scope -fsanitize-address-globals-dead-stripping -fno-assume-sane-operator-new -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/example-a27a44.o -x c &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	&lt;source&gt;:4:6: LLVM IR generation of declaration 'foo'
3.	&lt;source&gt;:4:6: Generating code for declaration 'foo'
 #<!-- -->0 0x0000000003932808 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x3932808)
 #<!-- -->1 0x00000000039303ec SignalHandler(int) Signals.cpp:0:0
 #<!-- -->2 0x00007de5a0842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->3 0x0000000003cd953c clang::CodeGen::CodeGenFunction::emitAlignmentAssumptionCheck(llvm::Value*, clang::QualType, clang::SourceLocation, clang::SourceLocation, llvm::Value*, llvm::Value*, llvm::Value*, llvm::Instruction*) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x3cd953c)
 #<!-- -->4 0x0000000003cd9b83 clang::CodeGen::CodeGenFunction::emitAlignmentAssumption(llvm::Value*, clang::QualType, clang::SourceLocation, clang::SourceLocation, llvm::Value*, llvm::Value*) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x3cd9b83)
 #<!-- -->5 0x0000000003cda2a4 clang::CodeGen::CodeGenFunction::emitAlignmentAssumption(llvm::Value*, clang::Expr const*, clang::SourceLocation, llvm::Value*, llvm::Value*) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x3cda2a4)
 #<!-- -->6 0x0000000003ffb847 clang::CodeGen::CodeGenFunction::EmitBuiltinExpr(clang::GlobalDecl, unsigned int, clang::CallExpr const*, clang::CodeGen::ReturnValueSlot) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x3ffb847)
 #<!-- -->7 0x00000000040dd556 clang::CodeGen::CodeGenFunction::EmitCallExpr(clang::CallExpr const*, clang::CodeGen::ReturnValueSlot, llvm::CallBase**) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x40dd556)
 #<!-- -->8 0x000000000412a4b9 (anonymous namespace)::ScalarExprEmitter::VisitCallExpr(clang::CallExpr const*) CGExprScalar.cpp:0:0
 #<!-- -->9 0x0000000004120adc clang::StmtVisitorBase&lt;std::add_pointer, (anonymous namespace)::ScalarExprEmitter, llvm::Value*&gt;::Visit(clang::Stmt*) CGExprScalar.cpp:0:0
#<!-- -->10 0x0000000004122cbb (anonymous namespace)::ScalarExprEmitter::Visit(clang::Expr*) CGExprScalar.cpp:0:0
#<!-- -->11 0x000000000412776b (anonymous namespace)::ScalarExprEmitter::VisitCastExpr(clang::CastExpr*) CGExprScalar.cpp:0:0
#<!-- -->12 0x00000000041212d2 clang::StmtVisitorBase&lt;std::add_pointer, (anonymous namespace)::ScalarExprEmitter, llvm::Value*&gt;::Visit(clang::Stmt*) CGExprScalar.cpp:0:0
#<!-- -->13 0x0000000004125e22 clang::CodeGen::CodeGenFunction::EmitScalarExpr(clang::Expr const*, bool) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x4125e22)
#<!-- -->14 0x000000000407f33b clang::CodeGen::CodeGenFunction::EmitScalarInit(clang::Expr const*, clang::ValueDecl const*, clang::CodeGen::LValue, bool) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x407f33b)
#<!-- -->15 0x0000000004080663 clang::CodeGen::CodeGenFunction::EmitExprAsInit(clang::Expr const*, clang::ValueDecl const*, clang::CodeGen::LValue, bool) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x4080663)
#<!-- -->16 0x000000000408e9fb clang::CodeGen::CodeGenFunction::EmitAutoVarInit(clang::CodeGen::CodeGenFunction::AutoVarEmission const&amp;) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x408e9fb)
#<!-- -->17 0x000000000408f6dc clang::CodeGen::CodeGenFunction::EmitVarDecl(clang::VarDecl const&amp;) (.part.0) CGDecl.cpp:0:0
#<!-- -->18 0x000000000408f7a8 clang::CodeGen::CodeGenFunction::EmitDecl(clang::Decl const&amp;) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x408f7a8)
#<!-- -->19 0x0000000003c5fe2f clang::CodeGen::CodeGenFunction::EmitDeclStmt(clang::DeclStmt const&amp;) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x3c5fe2f)
#<!-- -->20 0x0000000003c7517a clang::CodeGen::CodeGenFunction::EmitSimpleStmt(clang::Stmt const*, llvm::ArrayRef&lt;clang::Attr const*&gt;) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x3c7517a)
#<!-- -->21 0x0000000003c6d742 clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef&lt;clang::Attr const*&gt;) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x3c6d742)
#<!-- -->22 0x0000000003c74c29 clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&amp;, bool, clang::CodeGen::AggValueSlot) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x3c74c29)
#<!-- -->23 0x0000000003cdc36a clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x3cdc36a)
#<!-- -->24 0x0000000003ce6f85 clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&amp;) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x3ce6f85)
#<!-- -->25 0x0000000003d44beb clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x3d44beb)
#<!-- -->26 0x0000000003d40095 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x3d40095)
#<!-- -->27 0x0000000003d40ac0 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x3d40ac0)
#<!-- -->28 0x0000000003d4a687 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#<!-- -->29 0x000000000425401e (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#<!-- -->30 0x000000000423e469 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x423e469)
#<!-- -->31 0x0000000006215ac4 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x6215ac4)
#<!-- -->32 0x0000000004251b35 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x4251b35)
#<!-- -->33 0x0000000004517f71 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x4517f71)
#<!-- -->34 0x0000000004497a8b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x4497a8b)
#<!-- -->35 0x00000000045fc2e3 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x45fc2e3)
#<!-- -->36 0x0000000000cdf87c cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0xcdf87c)
#<!-- -->37 0x0000000000cd82dd ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->38 0x0000000000cdc44f clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0xcdc44f)
#<!-- -->39 0x0000000000baf834 main (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0xbaf834)
#<!-- -->40 0x00007de5a0829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->41 0x00007de5a0829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->42 0x0000000000cd7d7e _start (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0xcd7d7e)
clang: error: unable to execute command: Segmentation fault (core dumped)
clang: error: clang frontend command failed due to signal (use -v to see invocation)
Execution build compiler returned: 254
```
Code:
```c

#include &lt;stdio.h&gt;

void foo(int arg) {
    int x = 0;

    unsigned char *y = __builtin_assume_aligned(arg, 16);

    for (int i = 0; i &lt; 8; ++i) {
        x = y[i];
        printf("Value of x in the loop: %d\n", x);
    }
    
    return;
}

```
</details>


---

### Comment 4 - Endilll

One reduction of this points to a Clang 16 regression.
Reduced by C-Reduce (https://godbolt.org/z/vMTT8Kfqe):
```cpp
int *y = __builtin_assume_aligned(0, 1);
```

---

### Comment 5 - Endilll

Another reduction, specifically for 19 regression (https://godbolt.org/z/hs7qhK4oe):
```cpp
void foo() {
  int *y = __builtin_assume_aligned(1, 1);
}
```

I guess we need to better check inputs of this builtin.

---

### Comment 6 - shafik

@ostannard git bisect links this change to the regression: ef395a492aa931f428e99e1c0a93d4ad2fb0fcfa

Can you please investigate.


---

### Comment 7 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: None (bi6c)

<details>
Compiler Explorer: https://godbolt.org/z/3vbqjEbqb
```console
&lt;source&gt;:7:49: error: incompatible integer to pointer conversion passing 'int' to parameter of type 'const void *' [-Wint-conversion]
    7 |     unsigned char *y = __builtin_assume_aligned(arg, 16);
      |                                                 ^~~
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -fsanitize=undefined -fsanitize=address --rtlib=compiler-rt -g -lgcc_s -w -O2 &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	&lt;source&gt;:4:6: LLVM IR generation of declaration 'foo'
3.	&lt;source&gt;:4:6: Generating code for declaration 'foo'
 #<!-- -->0 0x0000000003932808 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3932808)
 #<!-- -->1 0x0000000003930974 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3930974)
 #<!-- -->2 0x0000000003882fb8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007786dca42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x0000000003cd953c clang::CodeGen::CodeGenFunction::emitAlignmentAssumptionCheck(llvm::Value*, clang::QualType, clang::SourceLocation, clang::SourceLocation, llvm::Value*, llvm::Value*, llvm::Value*, llvm::Instruction*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3cd953c)
 #<!-- -->5 0x0000000003cd9b83 clang::CodeGen::CodeGenFunction::emitAlignmentAssumption(llvm::Value*, clang::QualType, clang::SourceLocation, clang::SourceLocation, llvm::Value*, llvm::Value*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3cd9b83)
 #<!-- -->6 0x0000000003cda2a4 clang::CodeGen::CodeGenFunction::emitAlignmentAssumption(llvm::Value*, clang::Expr const*, clang::SourceLocation, llvm::Value*, llvm::Value*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3cda2a4)
 #<!-- -->7 0x0000000003ffb847 clang::CodeGen::CodeGenFunction::EmitBuiltinExpr(clang::GlobalDecl, unsigned int, clang::CallExpr const*, clang::CodeGen::ReturnValueSlot) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3ffb847)
 #<!-- -->8 0x00000000040dd556 clang::CodeGen::CodeGenFunction::EmitCallExpr(clang::CallExpr const*, clang::CodeGen::ReturnValueSlot, llvm::CallBase**) (/opt/compiler-explorer/clang-trunk/bin/clang+0x40dd556)
 #<!-- -->9 0x000000000412a4b9 (anonymous namespace)::ScalarExprEmitter::VisitCallExpr(clang::CallExpr const*) CGExprScalar.cpp:0:0
#<!-- -->10 0x0000000004120adc clang::StmtVisitorBase&lt;std::add_pointer, (anonymous namespace)::ScalarExprEmitter, llvm::Value*&gt;::Visit(clang::Stmt*) CGExprScalar.cpp:0:0
#<!-- -->11 0x0000000004122cbb (anonymous namespace)::ScalarExprEmitter::Visit(clang::Expr*) CGExprScalar.cpp:0:0
#<!-- -->12 0x000000000412776b (anonymous namespace)::ScalarExprEmitter::VisitCastExpr(clang::CastExpr*) CGExprScalar.cpp:0:0
#<!-- -->13 0x00000000041212d2 clang::StmtVisitorBase&lt;std::add_pointer, (anonymous namespace)::ScalarExprEmitter, llvm::Value*&gt;::Visit(clang::Stmt*) CGExprScalar.cpp:0:0
#<!-- -->14 0x0000000004125e22 clang::CodeGen::CodeGenFunction::EmitScalarExpr(clang::Expr const*, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+0x4125e22)
#<!-- -->15 0x000000000407f33b clang::CodeGen::CodeGenFunction::EmitScalarInit(clang::Expr const*, clang::ValueDecl const*, clang::CodeGen::LValue, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+0x407f33b)
#<!-- -->16 0x0000000004080663 clang::CodeGen::CodeGenFunction::EmitExprAsInit(clang::Expr const*, clang::ValueDecl const*, clang::CodeGen::LValue, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+0x4080663)
#<!-- -->17 0x000000000408e9fb clang::CodeGen::CodeGenFunction::EmitAutoVarInit(clang::CodeGen::CodeGenFunction::AutoVarEmission const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x408e9fb)
#<!-- -->18 0x000000000408f6dc clang::CodeGen::CodeGenFunction::EmitVarDecl(clang::VarDecl const&amp;) (.part.0) CGDecl.cpp:0:0
#<!-- -->19 0x000000000408f7a8 clang::CodeGen::CodeGenFunction::EmitDecl(clang::Decl const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x408f7a8)
#<!-- -->20 0x0000000003c5fe2f clang::CodeGen::CodeGenFunction::EmitDeclStmt(clang::DeclStmt const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3c5fe2f)
#<!-- -->21 0x0000000003c7517a clang::CodeGen::CodeGenFunction::EmitSimpleStmt(clang::Stmt const*, llvm::ArrayRef&lt;clang::Attr const*&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3c7517a)
#<!-- -->22 0x0000000003c6d742 clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef&lt;clang::Attr const*&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3c6d742)
#<!-- -->23 0x0000000003c74c29 clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&amp;, bool, clang::CodeGen::AggValueSlot) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3c74c29)
#<!-- -->24 0x0000000003cdc36a clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3cdc36a)
#<!-- -->25 0x0000000003ce6f85 clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3ce6f85)
#<!-- -->26 0x0000000003d44beb clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3d44beb)
#<!-- -->27 0x0000000003d40095 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3d40095)
#<!-- -->28 0x0000000003d40ac0 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3d40ac0)
#<!-- -->29 0x0000000003d4a687 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#<!-- -->30 0x000000000425401e (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#<!-- -->31 0x000000000423e469 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/compiler-explorer/clang-trunk/bin/clang+0x423e469)
#<!-- -->32 0x0000000006215ac4 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+0x6215ac4)
#<!-- -->33 0x0000000004251b35 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+0x4251b35)
#<!-- -->34 0x0000000004517f71 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+0x4517f71)
#<!-- -->35 0x0000000004497a8b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x4497a8b)
#<!-- -->36 0x00000000045fc2e3 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x45fc2e3)
#<!-- -->37 0x0000000000cdf87c cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+0xcdf87c)
#<!-- -->38 0x0000000000cd82dd ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->39 0x000000000429a0a9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->40 0x00000000038833e3 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x38833e3)
#<!-- -->41 0x000000000429a2c9 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->42 0x000000000426173d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+0x426173d)
#<!-- -->43 0x000000000426273d clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+0x426273d)
#<!-- -->44 0x000000000426c26c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x426c26c)
#<!-- -->45 0x0000000000cdc5d1 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0xcdc5d1)
#<!-- -->46 0x0000000000baf834 main (/opt/compiler-explorer/clang-trunk/bin/clang+0xbaf834)
#<!-- -->47 0x00007786dca29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->48 0x00007786dca29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->49 0x0000000000cd7d7e _start (/opt/compiler-explorer/clang-trunk/bin/clang+0xcd7d7e)
clang: error: clang frontend command failed with exit code 139 (use -v to see invocation)
ASM generation compiler returned: 139
&lt;source&gt;:7:49: error: incompatible integer to pointer conversion passing 'int' to parameter of type 'const void *' [-Wint-conversion]
    7 |     unsigned char *y = __builtin_assume_aligned(arg, 16);
      |                                                 ^~~
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk-20241001/bin/clang-20 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -dumpdir /app/output.s- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name example.c -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=none -fmath-errno -ffp-contract=on -fno-rounding-math -fno-verbose-asm -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debug-info-kind=constructor -dwarf-version=4 -debugger-tuning=gdb -fdebug-compilation-dir=/app -fcoverage-compilation-dir=/app -resource-dir /opt/compiler-explorer/clang-trunk-20241001/lib/clang/20 -internal-isystem /opt/compiler-explorer/clang-trunk-20241001/lib/clang/20/include -internal-isystem /usr/local/include -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/15.0.0/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -O2 -w -ferror-limit 19 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -vectorize-loops -vectorize-slp -fsanitize=address,alignment,array-bounds,bool,builtin,enum,float-cast-overflow,function,integer-divide-by-zero,nonnull-attribute,null,object-size,pointer-overflow,return,returns-nonnull-attribute,shift-base,shift-exponent,signed-integer-overflow,unreachable,vla-bound,vptr -fsanitize-recover=alignment,array-bounds,bool,builtin,enum,float-cast-overflow,function,integer-divide-by-zero,nonnull-attribute,null,object-size,pointer-overflow,returns-nonnull-attribute,shift-base,shift-exponent,signed-integer-overflow,vla-bound,vptr -fsanitize-system-ignorelist=/opt/compiler-explorer/clang-trunk-20241001/lib/clang/20/share/asan_ignorelist.txt -fno-sanitize-memory-param-retval -fsanitize-address-use-after-scope -fsanitize-address-globals-dead-stripping -fno-assume-sane-operator-new -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/example-a27a44.o -x c &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	&lt;source&gt;:4:6: LLVM IR generation of declaration 'foo'
3.	&lt;source&gt;:4:6: Generating code for declaration 'foo'
 #<!-- -->0 0x0000000003932808 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x3932808)
 #<!-- -->1 0x00000000039303ec SignalHandler(int) Signals.cpp:0:0
 #<!-- -->2 0x00007de5a0842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->3 0x0000000003cd953c clang::CodeGen::CodeGenFunction::emitAlignmentAssumptionCheck(llvm::Value*, clang::QualType, clang::SourceLocation, clang::SourceLocation, llvm::Value*, llvm::Value*, llvm::Value*, llvm::Instruction*) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x3cd953c)
 #<!-- -->4 0x0000000003cd9b83 clang::CodeGen::CodeGenFunction::emitAlignmentAssumption(llvm::Value*, clang::QualType, clang::SourceLocation, clang::SourceLocation, llvm::Value*, llvm::Value*) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x3cd9b83)
 #<!-- -->5 0x0000000003cda2a4 clang::CodeGen::CodeGenFunction::emitAlignmentAssumption(llvm::Value*, clang::Expr const*, clang::SourceLocation, llvm::Value*, llvm::Value*) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x3cda2a4)
 #<!-- -->6 0x0000000003ffb847 clang::CodeGen::CodeGenFunction::EmitBuiltinExpr(clang::GlobalDecl, unsigned int, clang::CallExpr const*, clang::CodeGen::ReturnValueSlot) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x3ffb847)
 #<!-- -->7 0x00000000040dd556 clang::CodeGen::CodeGenFunction::EmitCallExpr(clang::CallExpr const*, clang::CodeGen::ReturnValueSlot, llvm::CallBase**) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x40dd556)
 #<!-- -->8 0x000000000412a4b9 (anonymous namespace)::ScalarExprEmitter::VisitCallExpr(clang::CallExpr const*) CGExprScalar.cpp:0:0
 #<!-- -->9 0x0000000004120adc clang::StmtVisitorBase&lt;std::add_pointer, (anonymous namespace)::ScalarExprEmitter, llvm::Value*&gt;::Visit(clang::Stmt*) CGExprScalar.cpp:0:0
#<!-- -->10 0x0000000004122cbb (anonymous namespace)::ScalarExprEmitter::Visit(clang::Expr*) CGExprScalar.cpp:0:0
#<!-- -->11 0x000000000412776b (anonymous namespace)::ScalarExprEmitter::VisitCastExpr(clang::CastExpr*) CGExprScalar.cpp:0:0
#<!-- -->12 0x00000000041212d2 clang::StmtVisitorBase&lt;std::add_pointer, (anonymous namespace)::ScalarExprEmitter, llvm::Value*&gt;::Visit(clang::Stmt*) CGExprScalar.cpp:0:0
#<!-- -->13 0x0000000004125e22 clang::CodeGen::CodeGenFunction::EmitScalarExpr(clang::Expr const*, bool) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x4125e22)
#<!-- -->14 0x000000000407f33b clang::CodeGen::CodeGenFunction::EmitScalarInit(clang::Expr const*, clang::ValueDecl const*, clang::CodeGen::LValue, bool) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x407f33b)
#<!-- -->15 0x0000000004080663 clang::CodeGen::CodeGenFunction::EmitExprAsInit(clang::Expr const*, clang::ValueDecl const*, clang::CodeGen::LValue, bool) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x4080663)
#<!-- -->16 0x000000000408e9fb clang::CodeGen::CodeGenFunction::EmitAutoVarInit(clang::CodeGen::CodeGenFunction::AutoVarEmission const&amp;) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x408e9fb)
#<!-- -->17 0x000000000408f6dc clang::CodeGen::CodeGenFunction::EmitVarDecl(clang::VarDecl const&amp;) (.part.0) CGDecl.cpp:0:0
#<!-- -->18 0x000000000408f7a8 clang::CodeGen::CodeGenFunction::EmitDecl(clang::Decl const&amp;) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x408f7a8)
#<!-- -->19 0x0000000003c5fe2f clang::CodeGen::CodeGenFunction::EmitDeclStmt(clang::DeclStmt const&amp;) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x3c5fe2f)
#<!-- -->20 0x0000000003c7517a clang::CodeGen::CodeGenFunction::EmitSimpleStmt(clang::Stmt const*, llvm::ArrayRef&lt;clang::Attr const*&gt;) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x3c7517a)
#<!-- -->21 0x0000000003c6d742 clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef&lt;clang::Attr const*&gt;) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x3c6d742)
#<!-- -->22 0x0000000003c74c29 clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&amp;, bool, clang::CodeGen::AggValueSlot) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x3c74c29)
#<!-- -->23 0x0000000003cdc36a clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x3cdc36a)
#<!-- -->24 0x0000000003ce6f85 clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&amp;) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x3ce6f85)
#<!-- -->25 0x0000000003d44beb clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x3d44beb)
#<!-- -->26 0x0000000003d40095 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x3d40095)
#<!-- -->27 0x0000000003d40ac0 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x3d40ac0)
#<!-- -->28 0x0000000003d4a687 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#<!-- -->29 0x000000000425401e (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#<!-- -->30 0x000000000423e469 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x423e469)
#<!-- -->31 0x0000000006215ac4 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x6215ac4)
#<!-- -->32 0x0000000004251b35 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x4251b35)
#<!-- -->33 0x0000000004517f71 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x4517f71)
#<!-- -->34 0x0000000004497a8b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x4497a8b)
#<!-- -->35 0x00000000045fc2e3 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x45fc2e3)
#<!-- -->36 0x0000000000cdf87c cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0xcdf87c)
#<!-- -->37 0x0000000000cd82dd ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->38 0x0000000000cdc44f clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0xcdc44f)
#<!-- -->39 0x0000000000baf834 main (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0xbaf834)
#<!-- -->40 0x00007de5a0829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->41 0x00007de5a0829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->42 0x0000000000cd7d7e _start (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0xcd7d7e)
clang: error: unable to execute command: Segmentation fault (core dumped)
clang: error: clang frontend command failed due to signal (use -v to see invocation)
Execution build compiler returned: 254
```
Code:
```c

#include &lt;stdio.h&gt;

void foo(int arg) {
    int x = 0;

    unsigned char *y = __builtin_assume_aligned(arg, 16);

    for (int i = 0; i &lt; 8; ++i) {
        x = y[i];
        printf("Value of x in the loop: %d\n", x);
    }
    
    return;
}

```
</details>


---

### Comment 8 - ostannard

This also occurs before ef395a492aa931f428e99e1c0a93d4ad2fb0fcfa with `-Wno-int-conversion`, so this is an older bug, and that patch made it easier to hit. The same applies to #111293.

---

### Comment 9 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (bi6c)

<details>
Compiler Explorer: https://godbolt.org/z/3vbqjEbqb
```console
&lt;source&gt;:7:49: error: incompatible integer to pointer conversion passing 'int' to parameter of type 'const void *' [-Wint-conversion]
    7 |     unsigned char *y = __builtin_assume_aligned(arg, 16);
      |                                                 ^~~
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -fsanitize=undefined -fsanitize=address --rtlib=compiler-rt -g -lgcc_s -w -O2 &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	&lt;source&gt;:4:6: LLVM IR generation of declaration 'foo'
3.	&lt;source&gt;:4:6: Generating code for declaration 'foo'
 #<!-- -->0 0x0000000003932808 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3932808)
 #<!-- -->1 0x0000000003930974 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3930974)
 #<!-- -->2 0x0000000003882fb8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007786dca42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x0000000003cd953c clang::CodeGen::CodeGenFunction::emitAlignmentAssumptionCheck(llvm::Value*, clang::QualType, clang::SourceLocation, clang::SourceLocation, llvm::Value*, llvm::Value*, llvm::Value*, llvm::Instruction*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3cd953c)
 #<!-- -->5 0x0000000003cd9b83 clang::CodeGen::CodeGenFunction::emitAlignmentAssumption(llvm::Value*, clang::QualType, clang::SourceLocation, clang::SourceLocation, llvm::Value*, llvm::Value*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3cd9b83)
 #<!-- -->6 0x0000000003cda2a4 clang::CodeGen::CodeGenFunction::emitAlignmentAssumption(llvm::Value*, clang::Expr const*, clang::SourceLocation, llvm::Value*, llvm::Value*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3cda2a4)
 #<!-- -->7 0x0000000003ffb847 clang::CodeGen::CodeGenFunction::EmitBuiltinExpr(clang::GlobalDecl, unsigned int, clang::CallExpr const*, clang::CodeGen::ReturnValueSlot) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3ffb847)
 #<!-- -->8 0x00000000040dd556 clang::CodeGen::CodeGenFunction::EmitCallExpr(clang::CallExpr const*, clang::CodeGen::ReturnValueSlot, llvm::CallBase**) (/opt/compiler-explorer/clang-trunk/bin/clang+0x40dd556)
 #<!-- -->9 0x000000000412a4b9 (anonymous namespace)::ScalarExprEmitter::VisitCallExpr(clang::CallExpr const*) CGExprScalar.cpp:0:0
#<!-- -->10 0x0000000004120adc clang::StmtVisitorBase&lt;std::add_pointer, (anonymous namespace)::ScalarExprEmitter, llvm::Value*&gt;::Visit(clang::Stmt*) CGExprScalar.cpp:0:0
#<!-- -->11 0x0000000004122cbb (anonymous namespace)::ScalarExprEmitter::Visit(clang::Expr*) CGExprScalar.cpp:0:0
#<!-- -->12 0x000000000412776b (anonymous namespace)::ScalarExprEmitter::VisitCastExpr(clang::CastExpr*) CGExprScalar.cpp:0:0
#<!-- -->13 0x00000000041212d2 clang::StmtVisitorBase&lt;std::add_pointer, (anonymous namespace)::ScalarExprEmitter, llvm::Value*&gt;::Visit(clang::Stmt*) CGExprScalar.cpp:0:0
#<!-- -->14 0x0000000004125e22 clang::CodeGen::CodeGenFunction::EmitScalarExpr(clang::Expr const*, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+0x4125e22)
#<!-- -->15 0x000000000407f33b clang::CodeGen::CodeGenFunction::EmitScalarInit(clang::Expr const*, clang::ValueDecl const*, clang::CodeGen::LValue, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+0x407f33b)
#<!-- -->16 0x0000000004080663 clang::CodeGen::CodeGenFunction::EmitExprAsInit(clang::Expr const*, clang::ValueDecl const*, clang::CodeGen::LValue, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+0x4080663)
#<!-- -->17 0x000000000408e9fb clang::CodeGen::CodeGenFunction::EmitAutoVarInit(clang::CodeGen::CodeGenFunction::AutoVarEmission const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x408e9fb)
#<!-- -->18 0x000000000408f6dc clang::CodeGen::CodeGenFunction::EmitVarDecl(clang::VarDecl const&amp;) (.part.0) CGDecl.cpp:0:0
#<!-- -->19 0x000000000408f7a8 clang::CodeGen::CodeGenFunction::EmitDecl(clang::Decl const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x408f7a8)
#<!-- -->20 0x0000000003c5fe2f clang::CodeGen::CodeGenFunction::EmitDeclStmt(clang::DeclStmt const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3c5fe2f)
#<!-- -->21 0x0000000003c7517a clang::CodeGen::CodeGenFunction::EmitSimpleStmt(clang::Stmt const*, llvm::ArrayRef&lt;clang::Attr const*&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3c7517a)
#<!-- -->22 0x0000000003c6d742 clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef&lt;clang::Attr const*&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3c6d742)
#<!-- -->23 0x0000000003c74c29 clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&amp;, bool, clang::CodeGen::AggValueSlot) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3c74c29)
#<!-- -->24 0x0000000003cdc36a clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3cdc36a)
#<!-- -->25 0x0000000003ce6f85 clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3ce6f85)
#<!-- -->26 0x0000000003d44beb clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3d44beb)
#<!-- -->27 0x0000000003d40095 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3d40095)
#<!-- -->28 0x0000000003d40ac0 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3d40ac0)
#<!-- -->29 0x0000000003d4a687 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#<!-- -->30 0x000000000425401e (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#<!-- -->31 0x000000000423e469 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/compiler-explorer/clang-trunk/bin/clang+0x423e469)
#<!-- -->32 0x0000000006215ac4 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+0x6215ac4)
#<!-- -->33 0x0000000004251b35 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+0x4251b35)
#<!-- -->34 0x0000000004517f71 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+0x4517f71)
#<!-- -->35 0x0000000004497a8b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x4497a8b)
#<!-- -->36 0x00000000045fc2e3 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x45fc2e3)
#<!-- -->37 0x0000000000cdf87c cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+0xcdf87c)
#<!-- -->38 0x0000000000cd82dd ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->39 0x000000000429a0a9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->40 0x00000000038833e3 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x38833e3)
#<!-- -->41 0x000000000429a2c9 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->42 0x000000000426173d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+0x426173d)
#<!-- -->43 0x000000000426273d clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+0x426273d)
#<!-- -->44 0x000000000426c26c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x426c26c)
#<!-- -->45 0x0000000000cdc5d1 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0xcdc5d1)
#<!-- -->46 0x0000000000baf834 main (/opt/compiler-explorer/clang-trunk/bin/clang+0xbaf834)
#<!-- -->47 0x00007786dca29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->48 0x00007786dca29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->49 0x0000000000cd7d7e _start (/opt/compiler-explorer/clang-trunk/bin/clang+0xcd7d7e)
clang: error: clang frontend command failed with exit code 139 (use -v to see invocation)
ASM generation compiler returned: 139
&lt;source&gt;:7:49: error: incompatible integer to pointer conversion passing 'int' to parameter of type 'const void *' [-Wint-conversion]
    7 |     unsigned char *y = __builtin_assume_aligned(arg, 16);
      |                                                 ^~~
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk-20241001/bin/clang-20 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -dumpdir /app/output.s- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name example.c -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=none -fmath-errno -ffp-contract=on -fno-rounding-math -fno-verbose-asm -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debug-info-kind=constructor -dwarf-version=4 -debugger-tuning=gdb -fdebug-compilation-dir=/app -fcoverage-compilation-dir=/app -resource-dir /opt/compiler-explorer/clang-trunk-20241001/lib/clang/20 -internal-isystem /opt/compiler-explorer/clang-trunk-20241001/lib/clang/20/include -internal-isystem /usr/local/include -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/15.0.0/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -O2 -w -ferror-limit 19 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -vectorize-loops -vectorize-slp -fsanitize=address,alignment,array-bounds,bool,builtin,enum,float-cast-overflow,function,integer-divide-by-zero,nonnull-attribute,null,object-size,pointer-overflow,return,returns-nonnull-attribute,shift-base,shift-exponent,signed-integer-overflow,unreachable,vla-bound,vptr -fsanitize-recover=alignment,array-bounds,bool,builtin,enum,float-cast-overflow,function,integer-divide-by-zero,nonnull-attribute,null,object-size,pointer-overflow,returns-nonnull-attribute,shift-base,shift-exponent,signed-integer-overflow,vla-bound,vptr -fsanitize-system-ignorelist=/opt/compiler-explorer/clang-trunk-20241001/lib/clang/20/share/asan_ignorelist.txt -fno-sanitize-memory-param-retval -fsanitize-address-use-after-scope -fsanitize-address-globals-dead-stripping -fno-assume-sane-operator-new -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/example-a27a44.o -x c &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	&lt;source&gt;:4:6: LLVM IR generation of declaration 'foo'
3.	&lt;source&gt;:4:6: Generating code for declaration 'foo'
 #<!-- -->0 0x0000000003932808 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x3932808)
 #<!-- -->1 0x00000000039303ec SignalHandler(int) Signals.cpp:0:0
 #<!-- -->2 0x00007de5a0842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->3 0x0000000003cd953c clang::CodeGen::CodeGenFunction::emitAlignmentAssumptionCheck(llvm::Value*, clang::QualType, clang::SourceLocation, clang::SourceLocation, llvm::Value*, llvm::Value*, llvm::Value*, llvm::Instruction*) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x3cd953c)
 #<!-- -->4 0x0000000003cd9b83 clang::CodeGen::CodeGenFunction::emitAlignmentAssumption(llvm::Value*, clang::QualType, clang::SourceLocation, clang::SourceLocation, llvm::Value*, llvm::Value*) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x3cd9b83)
 #<!-- -->5 0x0000000003cda2a4 clang::CodeGen::CodeGenFunction::emitAlignmentAssumption(llvm::Value*, clang::Expr const*, clang::SourceLocation, llvm::Value*, llvm::Value*) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x3cda2a4)
 #<!-- -->6 0x0000000003ffb847 clang::CodeGen::CodeGenFunction::EmitBuiltinExpr(clang::GlobalDecl, unsigned int, clang::CallExpr const*, clang::CodeGen::ReturnValueSlot) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x3ffb847)
 #<!-- -->7 0x00000000040dd556 clang::CodeGen::CodeGenFunction::EmitCallExpr(clang::CallExpr const*, clang::CodeGen::ReturnValueSlot, llvm::CallBase**) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x40dd556)
 #<!-- -->8 0x000000000412a4b9 (anonymous namespace)::ScalarExprEmitter::VisitCallExpr(clang::CallExpr const*) CGExprScalar.cpp:0:0
 #<!-- -->9 0x0000000004120adc clang::StmtVisitorBase&lt;std::add_pointer, (anonymous namespace)::ScalarExprEmitter, llvm::Value*&gt;::Visit(clang::Stmt*) CGExprScalar.cpp:0:0
#<!-- -->10 0x0000000004122cbb (anonymous namespace)::ScalarExprEmitter::Visit(clang::Expr*) CGExprScalar.cpp:0:0
#<!-- -->11 0x000000000412776b (anonymous namespace)::ScalarExprEmitter::VisitCastExpr(clang::CastExpr*) CGExprScalar.cpp:0:0
#<!-- -->12 0x00000000041212d2 clang::StmtVisitorBase&lt;std::add_pointer, (anonymous namespace)::ScalarExprEmitter, llvm::Value*&gt;::Visit(clang::Stmt*) CGExprScalar.cpp:0:0
#<!-- -->13 0x0000000004125e22 clang::CodeGen::CodeGenFunction::EmitScalarExpr(clang::Expr const*, bool) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x4125e22)
#<!-- -->14 0x000000000407f33b clang::CodeGen::CodeGenFunction::EmitScalarInit(clang::Expr const*, clang::ValueDecl const*, clang::CodeGen::LValue, bool) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x407f33b)
#<!-- -->15 0x0000000004080663 clang::CodeGen::CodeGenFunction::EmitExprAsInit(clang::Expr const*, clang::ValueDecl const*, clang::CodeGen::LValue, bool) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x4080663)
#<!-- -->16 0x000000000408e9fb clang::CodeGen::CodeGenFunction::EmitAutoVarInit(clang::CodeGen::CodeGenFunction::AutoVarEmission const&amp;) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x408e9fb)
#<!-- -->17 0x000000000408f6dc clang::CodeGen::CodeGenFunction::EmitVarDecl(clang::VarDecl const&amp;) (.part.0) CGDecl.cpp:0:0
#<!-- -->18 0x000000000408f7a8 clang::CodeGen::CodeGenFunction::EmitDecl(clang::Decl const&amp;) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x408f7a8)
#<!-- -->19 0x0000000003c5fe2f clang::CodeGen::CodeGenFunction::EmitDeclStmt(clang::DeclStmt const&amp;) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x3c5fe2f)
#<!-- -->20 0x0000000003c7517a clang::CodeGen::CodeGenFunction::EmitSimpleStmt(clang::Stmt const*, llvm::ArrayRef&lt;clang::Attr const*&gt;) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x3c7517a)
#<!-- -->21 0x0000000003c6d742 clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef&lt;clang::Attr const*&gt;) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x3c6d742)
#<!-- -->22 0x0000000003c74c29 clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&amp;, bool, clang::CodeGen::AggValueSlot) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x3c74c29)
#<!-- -->23 0x0000000003cdc36a clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x3cdc36a)
#<!-- -->24 0x0000000003ce6f85 clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&amp;) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x3ce6f85)
#<!-- -->25 0x0000000003d44beb clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x3d44beb)
#<!-- -->26 0x0000000003d40095 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x3d40095)
#<!-- -->27 0x0000000003d40ac0 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x3d40ac0)
#<!-- -->28 0x0000000003d4a687 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#<!-- -->29 0x000000000425401e (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#<!-- -->30 0x000000000423e469 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x423e469)
#<!-- -->31 0x0000000006215ac4 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x6215ac4)
#<!-- -->32 0x0000000004251b35 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x4251b35)
#<!-- -->33 0x0000000004517f71 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x4517f71)
#<!-- -->34 0x0000000004497a8b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x4497a8b)
#<!-- -->35 0x00000000045fc2e3 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0x45fc2e3)
#<!-- -->36 0x0000000000cdf87c cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0xcdf87c)
#<!-- -->37 0x0000000000cd82dd ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->38 0x0000000000cdc44f clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0xcdc44f)
#<!-- -->39 0x0000000000baf834 main (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0xbaf834)
#<!-- -->40 0x00007de5a0829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->41 0x00007de5a0829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->42 0x0000000000cd7d7e _start (/opt/compiler-explorer/clang-trunk-20241001/bin/clang-20+0xcd7d7e)
clang: error: unable to execute command: Segmentation fault (core dumped)
clang: error: clang frontend command failed due to signal (use -v to see invocation)
Execution build compiler returned: 254
```
Code:
```c

#include &lt;stdio.h&gt;

void foo(int arg) {
    int x = 0;

    unsigned char *y = __builtin_assume_aligned(arg, 16);

    for (int i = 0; i &lt; 8; ++i) {
        x = y[i];
        printf("Value of x in the loop: %d\n", x);
    }
    
    return;
}

```
</details>


---

