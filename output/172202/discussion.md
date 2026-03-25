# [clang][bytecode] crash on __builtin_memchr

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/172202
**Status:** Closed
**Labels:** crash, clang:bytecode
**Closed Date:** 2026-01-02T11:00:46Z

## Body

Reproducer:
https://godbolt.org/z/fWqYP979x
```cpp
void f()  { __builtin_memchr(f, 0, 1); }
```

Backtrace:
```console
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x c -fexperimental-new-constant-interpreter -ferror-limit=999999999 <source>
1.	<eof> parser at end of file
2.	<source>:1:6: LLVM IR generation of declaration 'f'
3.	<source>:1:6: Generating code for declaration 'f'
 #0 0x00000000042bc068 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42bc068)
 #1 0x00000000042b9494 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42b9494)
 #2 0x00000000041fe278 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007c9c7fa42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x0000000007e6c303 clang::interp::interp__builtin_memchr(clang::interp::InterpState&, clang::interp::CodePtr, clang::CallExpr const*, unsigned int) InterpBuiltin.cpp:0:0
 #5 0x0000000007e86c3d clang::interp::InterpretBuiltin(clang::interp::InterpState&, clang::interp::CodePtr, clang::CallExpr const*, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7e86c3d)
 #6 0x00000000081bdfcb clang::interp::Compiler<clang::interp::EvalEmitter>::VisitBuiltinCallExpr(clang::CallExpr const*, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x81bdfcb)
 #7 0x00000000081be35e clang::interp::Compiler<clang::interp::EvalEmitter>::VisitCallExpr(clang::CallExpr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x81be35e)
 #8 0x00000000081c594a clang::interp::Compiler<clang::interp::EvalEmitter>::visit(clang::Expr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x81c594a)
 #9 0x00000000081b9d86 clang::interp::Compiler<clang::interp::EvalEmitter>::visitExpr(clang::Expr const*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x81b9d86)
#10 0x0000000007d3aa8d clang::interp::EvalEmitter::interpretExpr(clang::Expr const*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7d3aa8d)
#11 0x0000000007d2a393 clang::interp::Context::evaluateAsRValue(clang::interp::State&, clang::Expr const*, clang::APValue&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7d2a393)
#12 0x0000000007c90213 EvaluateAsRValue((anonymous namespace)::EvalInfo&, clang::Expr const*, clang::APValue&) ExprConstant.cpp:0:0
#13 0x0000000007c954e7 clang::Expr::EvaluateAsRValue(clang::Expr::EvalResult&, clang::ASTContext const&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7c954e7)
#14 0x0000000004ace590 clang::CodeGen::CodeGenFunction::EmitBuiltinExpr(clang::GlobalDecl, unsigned int, clang::CallExpr const*, clang::CodeGen::ReturnValueSlot) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ace590)
#15 0x000000000462d26e clang::CodeGen::CodeGenFunction::EmitCallExpr(clang::CallExpr const*, clang::CodeGen::ReturnValueSlot, llvm::CallBase**) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x462d26e)
#16 0x00000000046a49b4 (anonymous namespace)::ScalarExprEmitter::VisitCallExpr(clang::CallExpr const*) CGExprScalar.cpp:0:0
#17 0x0000000004699040 clang::StmtVisitorBase<std::add_pointer, (anonymous namespace)::ScalarExprEmitter, llvm::Value*>::Visit(clang::Stmt*) CGExprScalar.cpp:0:0
#18 0x000000000469d7dc clang::CodeGen::CodeGenFunction::EmitScalarExpr(clang::Expr const*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x469d7dc)
#19 0x000000000460153e clang::CodeGen::CodeGenFunction::EmitAnyExpr(clang::Expr const*, clang::CodeGen::AggValueSlot, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x460153e)
#20 0x00000000046189dd clang::CodeGen::CodeGenFunction::EmitIgnoredExpr(clang::Expr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x46189dd)
#21 0x00000000047ba3e7 clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef<clang::Attr const*>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x47ba3e7)
#22 0x00000000047c296b clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&, bool, clang::CodeGen::AggValueSlot) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x47c296b)
#23 0x000000000482a56e clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x482a56e)
#24 0x000000000483b964 clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x483b964)
#25 0x00000000048a9770 clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48a9770)
#26 0x00000000048a4414 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48a4414)
#27 0x00000000048a4bf3 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48a4bf3)
#28 0x00000000048afce3 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#29 0x0000000004c0f099 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#30 0x0000000004bffbc4 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bffbc4)
#31 0x00000000069bc8a4 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69bc8a4)
#32 0x0000000004c0c4a8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c0c4a8)
#33 0x0000000004f01b45 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f01b45)
#34 0x0000000004e81dbe clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e81dbe)
#35 0x0000000004ffbbfd clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ffbbfd)
#36 0x0000000000dcf4cc cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdcf4cc)
#37 0x0000000000dc5f2a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#38 0x0000000000dc60ad int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#39 0x0000000004c7f0d9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#40 0x00000000041fe714 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41fe714)
#41 0x0000000004c7f6ef clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#42 0x0000000004c3fee2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c3fee2)
#43 0x0000000004c40e8e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c40e8e)
#44 0x0000000004c482c5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c482c5)
#45 0x0000000000dcb8d1 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdcb8d1)
#46 0x0000000000c77584 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc77584)
#47 0x00007c9c7fa29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#48 0x00007c9c7fa29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#49 0x0000000000dc59c5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdc59c5)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```

## Comments

### Comment 1 - k-arrows

This crash was discovered while investigating whether crashes similar to https://github.com/llvm/llvm-project/issues/171609 occur in other built-in functions.

---

