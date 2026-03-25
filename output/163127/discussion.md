# [clang][bytecode] crashes when accessing typeid(int)

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/163127
**Status:** Closed
**Labels:** confirmed, crash, clang:bytecode
**Closed Date:** 2025-10-16T07:47:43Z

## Body

Reproducer:
https://godbolt.org/z/9GrjWGTbG
```cpp
namespace std {
class type_info {};
} // namespace std

bool foo() { return ((void **)&typeid(int))[0]; }
```

Backtrace:
```console
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -fexperimental-new-constant-interpreter <source>
1.	<eof> parser at end of file
2.	<source>:5:6: LLVM IR generation of declaration 'foo'
3.	<source>:5:6: Generating code for declaration 'foo'
 #0 0x0000000003cc5cb8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3cc5cb8)
 #1 0x0000000003cc368c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3cc368c)
 #2 0x0000000003c0a0d8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x0000747c61042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x000000000754d716 bool clang::interp::ArrayElemPtrPop<(clang::interp::PrimType)4, clang::interp::Integral<32u, true>>(clang::interp::InterpState&, clang::interp::CodePtr) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x754d716)
 #5 0x000000000788c1c5 clang::interp::Compiler<clang::interp::EvalEmitter>::VisitArraySubscriptExpr(clang::ArraySubscriptExpr const*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x788c1c5)
 #6 0x00000000078ae7eb clang::interp::Compiler<clang::interp::EvalEmitter>::visit(clang::Expr const*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x78ae7eb)
 #7 0x00000000078af2fd clang::interp::Compiler<clang::interp::EvalEmitter>::VisitCastExpr(clang::CastExpr const*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x78af2fd)
 #8 0x00000000078ae7eb clang::interp::Compiler<clang::interp::EvalEmitter>::visit(clang::Expr const*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x78ae7eb)
 #9 0x00000000078aed6f clang::interp::Compiler<clang::interp::EvalEmitter>::VisitCastExpr(clang::CastExpr const*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x78aed6f)
#10 0x00000000078ae7eb clang::interp::Compiler<clang::interp::EvalEmitter>::visit(clang::Expr const*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x78ae7eb)
#11 0x00000000078a4c6f clang::interp::Compiler<clang::interp::EvalEmitter>::visitExpr(clang::Expr const*, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x78a4c6f)
#12 0x00000000074cd6b3 clang::interp::EvalEmitter::interpretExpr(clang::Expr const*, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x74cd6b3)
#13 0x00000000074be766 clang::interp::Context::evaluateAsRValue(clang::interp::State&, clang::Expr const*, clang::APValue&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x74be766)
#14 0x000000000743315a EvaluateAsRValue((anonymous namespace)::EvalInfo&, clang::Expr const*, clang::APValue&) ExprConstant.cpp:0:0
#15 0x0000000007437f18 clang::Expr::EvaluateAsRValue(clang::Expr::EvalResult&, clang::ASTContext const&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x7437f18)
#16 0x0000000007438099 clang::Expr::isEvaluatable(clang::ASTContext const&, clang::Expr::SideEffectsKind) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x7438099)
#17 0x00000000041589cf clang::CodeGen::CodeGenFunction::EmitReturnStmt(clang::ReturnStmt const&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x41589cf)
#18 0x0000000004161eb3 clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef<clang::Attr const*>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4161eb3)
#19 0x0000000004168d59 clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&, bool, clang::CodeGen::AggValueSlot) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4168d59)
#20 0x00000000041cafd5 clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x41cafd5)
#21 0x00000000041dc1d3 clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x41dc1d3)
#22 0x000000000423a24b clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x423a24b)
#23 0x0000000004235f24 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4235f24)
#24 0x0000000004236823 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4236823)
#25 0x000000000424011f clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#26 0x00000000045904b1 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#27 0x000000000457a1a9 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x457a1a9)
#28 0x00000000061f7ae4 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61f7ae4)
#29 0x000000000458d545 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x458d545)
#30 0x000000000488946a clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x488946a)
#31 0x000000000480989b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x480989b)
#32 0x000000000497f4ab clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x497f4ab)
#33 0x0000000000dcf675 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdcf675)
#34 0x0000000000dc75cb ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#35 0x0000000000dc766d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#36 0x00000000045fae09 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#37 0x0000000003c0a4f3 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c0a4f3)
#38 0x00000000045fb029 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#39 0x00000000045be222 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45be222)
#40 0x00000000045bf101 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45bf101)
#41 0x00000000045c7c3c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45c7c3c)
#42 0x0000000000dcc0c9 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdcc0c9)
#43 0x0000000000c77c54 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc77c54)
#44 0x0000747c61029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#45 0x0000747c61029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#46 0x0000000000dc7065 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdc7065)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```

## Comments

### Comment 1 - k-arrows

Goes back to clang-21.1.0:
https://godbolt.org/z/E1aq9W4jT

---

### Comment 2 - term-est

It seems that we are derefing a nullptr during `interp::ArrayElemPtrPop`
Problematic line is at `Interp.h`

```cpp
if (Ptr.getFieldDesc()->isArray() && Ptr.getIndex() == 0) 
```

where `getFieldDesc` returns a `nullptr` for `Storage::Typeid` kind

```cpp
  /// Accessor for information about the declaration site.
  const Descriptor *getDeclDesc() const {
    if (isIntegralPointer())
      return Int.Desc;
    if (isFunctionPointer() || isTypeidPointer())
      return nullptr;
  }
```

as TypeId does not have a descriptor. I think we need to treat that case as a "not an array" and continue. 

---

