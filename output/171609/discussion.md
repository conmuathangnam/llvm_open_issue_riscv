# [clang][bytecode] crash on __builtin_memcpy

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/171609
**Status:** Closed
**Labels:** crash, clang:bytecode
**Closed Date:** 2026-01-02T09:01:58Z

## Body

Here is the C reproducer:
https://godbolt.org/z/T9W978fzW
```c
void f() { __builtin_memcpy(f, f, 1); }
```

Backtrace:
```console
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x c -fexperimental-new-constant-interpreter <source>
1.	<eof> parser at end of file
2.	<source>:1:6: LLVM IR generation of declaration 'f'
3.	<source>:1:6: Generating code for declaration 'f'
 #0 0x0000000003d0fc28 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3d0fc28)
 #1 0x0000000003d0d5fc llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3d0d5fc)
 #2 0x0000000003c54678 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000079bc1f442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00000000075034d0 clang::interp::Descriptor::getType() const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x75034d0)
 #5 0x0000000000c70d86 clang::interp::getElemType(clang::interp::Pointer const&) (.cold) InterpBuiltin.cpp:0:0
 #6 0x0000000007631292 clang::interp::interp__builtin_memcpy(clang::interp::InterpState&, clang::interp::CodePtr, clang::interp::InterpFrame const*, clang::CallExpr const*, unsigned int) (.isra.0) InterpBuiltin.cpp:0:0
 #7 0x0000000007651c3f clang::interp::InterpretBuiltin(clang::interp::InterpState&, clang::interp::CodePtr, clang::CallExpr const*, unsigned int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x7651c3f)
 #8 0x0000000007916850 clang::interp::Compiler<clang::interp::EvalEmitter>::VisitBuiltinCallExpr(clang::CallExpr const*, unsigned int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x7916850)
 #9 0x0000000007916a36 clang::interp::Compiler<clang::interp::EvalEmitter>::VisitCallExpr(clang::CallExpr const*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x7916a36)
#10 0x000000000791de1b clang::interp::Compiler<clang::interp::EvalEmitter>::visit(clang::Expr const*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x791de1b)
#11 0x000000000791366f clang::interp::Compiler<clang::interp::EvalEmitter>::visitExpr(clang::Expr const*, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x791366f)
#12 0x000000000750a303 clang::interp::EvalEmitter::interpretExpr(clang::Expr const*, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x750a303)
#13 0x00000000074fd1b6 clang::interp::Context::evaluateAsRValue(clang::interp::State&, clang::Expr const*, clang::APValue&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x74fd1b6)
#14 0x000000000746333a EvaluateAsRValue((anonymous namespace)::EvalInfo&, clang::Expr const*, clang::APValue&) ExprConstant.cpp:0:0
#15 0x0000000007468aa0 clang::Expr::EvaluateAsRValue(clang::Expr::EvalResult&, clang::ASTContext const&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x7468aa0)
#16 0x0000000004497f60 clang::CodeGen::CodeGenFunction::EmitBuiltinExpr(clang::GlobalDecl, unsigned int, clang::CallExpr const*, clang::CodeGen::ReturnValueSlot) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4497f60)
#17 0x00000000040393fb clang::CodeGen::CodeGenFunction::EmitCallExpr(clang::CallExpr const*, clang::CodeGen::ReturnValueSlot, llvm::CallBase**) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x40393fb)
#18 0x00000000040acc59 (anonymous namespace)::ScalarExprEmitter::VisitCallExpr(clang::CallExpr const*) CGExprScalar.cpp:0:0
#19 0x00000000040a260b clang::StmtVisitorBase<std::add_pointer, (anonymous namespace)::ScalarExprEmitter, llvm::Value*>::Visit(clang::Stmt*) CGExprScalar.cpp:0:0
#20 0x00000000040a6f02 clang::CodeGen::CodeGenFunction::EmitScalarExpr(clang::Expr const*, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x40a6f02)
#21 0x00000000040139ce clang::CodeGen::CodeGenFunction::EmitAnyExpr(clang::Expr const*, clang::CodeGen::AggValueSlot, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x40139ce)
#22 0x0000000004028fa8 clang::CodeGen::CodeGenFunction::EmitIgnoredExpr(clang::Expr const*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4028fa8)
#23 0x00000000041accfb clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef<clang::Attr const*>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x41accfb)
#24 0x00000000041b429b clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&, bool, clang::CodeGen::AggValueSlot) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x41b429b)
#25 0x0000000004213a45 clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4213a45)
#26 0x0000000004223d13 clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4223d13)
#27 0x00000000042818db clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x42818db)
#28 0x000000000427cb84 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x427cb84)
#29 0x000000000427d4e3 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x427d4e3)
#30 0x00000000042877af clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#31 0x00000000045cdf11 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#32 0x00000000045b8e19 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45b8e19)
#33 0x000000000620e114 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x620e114)
#34 0x00000000045cc1b5 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45cc1b5)
#35 0x00000000048dae6a clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x48dae6a)
#36 0x000000000485968b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x485968b)
#37 0x00000000049d0cdb clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x49d0cdb)
#38 0x0000000000dc9605 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdc9605)
#39 0x0000000000dc14bb ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#40 0x0000000000dc155d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#41 0x0000000004646999 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#42 0x0000000003c54a93 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c54a93)
#43 0x0000000004646bb9 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#44 0x0000000004609542 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4609542)
#45 0x000000000460a421 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x460a421)
#46 0x000000000461300c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x461300c)
#47 0x0000000000dc5f89 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdc5f89)
#48 0x0000000000c77cb4 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc77cb4)
#49 0x000079bc1f429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#50 0x000079bc1f429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#51 0x0000000000dc0f55 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdc0f55)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```

## Comments

### Comment 1 - k-arrows

Goes back to clang-20.1.0:
https://godbolt.org/z/rb1bPhGdM

---

