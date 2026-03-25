# [clang][bytecode] Crash in `clang::interp::CheckArray` on valid code

**Author:** BobIsOnFire
**URL:** https://github.com/llvm/llvm-project/issues/165006
**Status:** Closed
**Labels:** crash, clang:bytecode
**Closed Date:** 2025-11-10T07:17:09Z

## Body

```c++
struct sockaddr
{
  char sa_data[8];
};

struct in_addr
{
  unsigned int s_addr;
};

struct sockaddr_in
{
  unsigned short int sin_port;
  struct in_addr sin_addr;
};

unsigned int get_addr(sockaddr addr) {
    return ((sockaddr_in *)&addr)->sin_addr.s_addr;
}
```

```
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -fexperimental-new-constant-interpreter <source>
1.	<eof> parser at end of file
2.	<source>:17:14: LLVM IR generation of declaration 'get_addr'
3.	<source>:17:14: Generating code for declaration 'get_addr'
 #0 0x0000000004214d18 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4214d18)
 #1 0x0000000004212144 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4212144)
 #2 0x0000000004156348 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000077718b242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x0000000007dfae76 clang::interp::CheckArray(clang::interp::InterpState&, clang::interp::CodePtr, clang::interp::Pointer const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7dfae76)
 #5 0x0000000007e07c16 clang::interp::getField(clang::interp::InterpState&, clang::interp::CodePtr, clang::interp::Pointer const&, unsigned int) Interp.cpp:0:0
 #6 0x0000000007e07ec9 clang::interp::GetPtrFieldPop(clang::interp::InterpState&, clang::interp::CodePtr, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7e07ec9)
 #7 0x00000000080caad7 clang::interp::Compiler<clang::interp::EvalEmitter>::VisitMemberExpr(clang::MemberExpr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x80caad7)
 #8 0x00000000080d04ca clang::interp::Compiler<clang::interp::EvalEmitter>::visit(clang::Expr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x80d04ca)
 #9 0x00000000080c50c6 clang::interp::Compiler<clang::interp::EvalEmitter>::visitExpr(clang::Expr const*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x80c50c6)
#10 0x0000000007c657bd clang::interp::EvalEmitter::interpretExpr(clang::Expr const*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7c657bd)
#11 0x0000000007c54c33 clang::interp::Context::evaluateAsRValue(clang::interp::State&, clang::Expr const*, clang::APValue&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7c54c33)
#12 0x0000000007bc2ae3 EvaluateAsRValue((anonymous namespace)::EvalInfo&, clang::Expr const*, clang::APValue&) ExprConstant.cpp:0:0
#13 0x0000000007bc8e9e EvaluateAsInt(clang::Expr const*, clang::Expr::EvalResult&, clang::ASTContext const&, clang::Expr::SideEffectsKind, (anonymous namespace)::EvalInfo&) ExprConstant.cpp:0:0
#14 0x0000000007bc9240 clang::Expr::EvaluateAsInt(clang::Expr::EvalResult&, clang::ASTContext const&, clang::Expr::SideEffectsKind, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7bc9240)
#15 0x00000000045f4233 (anonymous namespace)::ScalarExprEmitter::VisitMemberExpr(clang::MemberExpr*) CGExprScalar.cpp:0:0
#16 0x00000000045ed15c clang::StmtVisitorBase<std::add_pointer, (anonymous namespace)::ScalarExprEmitter, llvm::Value*>::Visit(clang::Stmt*) CGExprScalar.cpp:0:0
#17 0x00000000045ee98b (anonymous namespace)::ScalarExprEmitter::Visit(clang::Expr*) CGExprScalar.cpp:0:0
#18 0x00000000045f45b3 (anonymous namespace)::ScalarExprEmitter::VisitCastExpr(clang::CastExpr*) CGExprScalar.cpp:0:0
#19 0x00000000045ecb51 clang::StmtVisitorBase<std::add_pointer, (anonymous namespace)::ScalarExprEmitter, llvm::Value*>::Visit(clang::Stmt*) CGExprScalar.cpp:0:0
#20 0x00000000045f0f7c clang::CodeGen::CodeGenFunction::EmitScalarExpr(clang::Expr const*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x45f0f7c)
#21 0x00000000046fd1cc clang::CodeGen::CodeGenFunction::EmitReturnStmt(clang::ReturnStmt const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x46fd1cc)
#22 0x000000000470666e clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef<clang::Attr const*>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x470666e)
#23 0x000000000470e65c clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&, bool, clang::CodeGen::AggValueSlot) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x470e65c)
#24 0x0000000004776c2e clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4776c2e)
#25 0x0000000004788c84 clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4788c84)
#26 0x00000000047f5d80 clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x47f5d80)
#27 0x00000000047f0c44 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x47f0c44)
#28 0x00000000047f1ae0 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x47f1ae0)
#29 0x00000000047fc2f3 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#30 0x0000000004b61ae9 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#31 0x0000000004b50c04 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b50c04)
#32 0x00000000069011c4 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69011c4)
#33 0x0000000004b5d4e8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b5d4e8)
#34 0x0000000004e4a685 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e4a685)
#35 0x0000000004dcb73e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4dcb73e)
#36 0x0000000004f41c2d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f41c2d)
#37 0x0000000000dce380 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdce380)
#38 0x0000000000dc4eea ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#39 0x0000000000dc506d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#40 0x0000000004bc6a79 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#41 0x00000000041567e4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41567e4)
#42 0x0000000004bc708f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#43 0x0000000004b87fd2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b87fd2)
#44 0x0000000004b88f7e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b88f7e)
#45 0x0000000004b90695 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b90695)
#46 0x0000000000dca8a9 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdca8a9)
#47 0x0000000000c79c64 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc79c64)
#48 0x000077718b229d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#49 0x000077718b229e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#50 0x0000000000dc4985 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdc4985)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```

https://godbolt.org/z/zhYYfh7Ev

## Comments

### Comment 1 - BobIsOnFire

Looks similar to #134796

---

### Comment 2 - tbaederr

The problematic pointer here is a result of a problematic bitcast, i.e. this would be fixed by a proper implementation of https://github.com/llvm/llvm-project/pull/163809.

---

