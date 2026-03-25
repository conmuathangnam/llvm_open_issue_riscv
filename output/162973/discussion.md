# [clang] fatal error: error in backend: Cannot implicitly convert a scalable size to a fixed-width size in `TypeSize::operator ScalarTy()`

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/162973
**Status:** Open
**Labels:** clang:codegen, crash

## Body

Reproducer:
https://godbolt.org/z/oPx7rb5xz
```c
void foo(int, ...);

struct S {
  int a, b, c;
};

void bar(void) {
  _Atomic struct S x = (struct S){0, 1, 2};
  foo(0, x = (struct S){0, 1, 2});
}
```

Backtrace:
```console
fatal error: error in backend: Cannot implicitly convert a scalable size to a fixed-width size in `TypeSize::operator ScalarTy()`
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<eof> parser at end of file
2.	<source>:7:6: LLVM IR generation of declaration 'bar'
3.	<source>:7:6: Generating code for declaration 'bar'
 #0 0x0000000003caee98 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3caee98)
 #1 0x0000000003cac86c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3cac86c)
 #2 0x0000000003bf37d3 llvm::CrashRecoveryContext::HandleExit(int) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3bf37d3)
 #3 0x0000000003ca40ae llvm::sys::Process::Exit(int, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3ca40ae)
 #4 0x0000000000dca91b LLVMErrorHandler(void*, char const*, bool) cc1_main.cpp:0:0
 #5 0x0000000003bfd563 llvm::report_fatal_error(llvm::Twine const&, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3bfd563)
 #6 0x0000000003bfd6f9 (/opt/compiler-explorer/clang-trunk/bin/clang+0x3bfd6f9)
 #7 0x0000000003fd966e (/opt/compiler-explorer/clang-trunk/bin/clang+0x3fd966e)
 #8 0x0000000003fdf20b clang::CodeGen::CodeGenFunction::EmitLValueForField(clang::CodeGen::LValue, clang::FieldDecl const*, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3fdf20b)
 #9 0x0000000003fe3ab5 clang::CodeGen::CodeGenFunction::EmitLValueForFieldInitialization(clang::CodeGen::LValue, clang::FieldDecl const*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3fe3ab5)
#10 0x0000000003ffea0d (anonymous namespace)::AggExprEmitter::VisitCXXParenListOrInitListExpr(clang::Expr*, llvm::ArrayRef<clang::Expr*>, clang::FieldDecl*, clang::Expr*) CGExprAgg.cpp:0:0
#11 0x0000000003ffbdf8 (anonymous namespace)::AggExprEmitter::Visit(clang::Expr*) CGExprAgg.cpp:0:0
#12 0x0000000003ffce19 clang::CodeGen::CodeGenFunction::EmitAggExpr(clang::Expr const*, clang::CodeGen::AggValueSlot) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3ffce19)
#13 0x0000000003ffc9c2 (anonymous namespace)::AggExprEmitter::Visit(clang::Expr*) CGExprAgg.cpp:0:0
#14 0x0000000003ffa6ec (anonymous namespace)::AggExprEmitter::VisitCastExpr(clang::CastExpr*) CGExprAgg.cpp:0:0
#15 0x0000000003ffbda8 (anonymous namespace)::AggExprEmitter::Visit(clang::Expr*) CGExprAgg.cpp:0:0
#16 0x0000000003ffce19 clang::CodeGen::CodeGenFunction::EmitAggExpr(clang::Expr const*, clang::CodeGen::AggValueSlot) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3ffce19)
#17 0x0000000003ffb723 (anonymous namespace)::AggExprEmitter::VisitCastExpr(clang::CastExpr*) CGExprAgg.cpp:0:0
#18 0x0000000003ffbda8 (anonymous namespace)::AggExprEmitter::Visit(clang::Expr*) CGExprAgg.cpp:0:0
#19 0x0000000003ffd16e (anonymous namespace)::AggExprEmitter::VisitBinAssign(clang::BinaryOperator const*) CGExprAgg.cpp:0:0
#20 0x0000000003ffc68b (anonymous namespace)::AggExprEmitter::Visit(clang::Expr*) CGExprAgg.cpp:0:0
#21 0x0000000003ffce19 clang::CodeGen::CodeGenFunction::EmitAggExpr(clang::Expr const*, clang::CodeGen::AggValueSlot) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3ffce19)
#22 0x0000000003fb53b2 clang::CodeGen::CodeGenFunction::EmitAnyExpr(clang::Expr const*, clang::CodeGen::AggValueSlot, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3fb53b2)
#23 0x0000000003fb67eb clang::CodeGen::CodeGenFunction::EmitAnyExprToTemp(clang::Expr const*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3fb67eb)
#24 0x000000000447bda8 clang::CodeGen::CodeGenFunction::EmitCallArg(clang::CodeGen::CallArgList&, clang::Expr const*, clang::QualType) (/opt/compiler-explorer/clang-trunk/bin/clang+0x447bda8)
#25 0x00000000044866f5 clang::CodeGen::CodeGenFunction::EmitCallArgs(clang::CodeGen::CallArgList&, clang::CodeGen::CodeGenFunction::PrototypeWrapper, llvm::iterator_range<clang::Stmt::CastIterator<clang::Expr, clang::Expr const* const, clang::Stmt const* const>>, clang::CodeGen::CodeGenFunction::AbstractCallee, unsigned int, clang::CodeGen::CodeGenFunction::EvaluationOrder) (/opt/compiler-explorer/clang-trunk/bin/clang+0x44866f5)
#26 0x0000000003fd6922 clang::CodeGen::CodeGenFunction::EmitCall(clang::QualType, clang::CodeGen::CGCallee const&, clang::CallExpr const*, clang::CodeGen::ReturnValueSlot, llvm::Value*, llvm::CallBase**, clang::CodeGen::CGFunctionInfo const**) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3fd6922)
#27 0x0000000003fd7b96 clang::CodeGen::CodeGenFunction::EmitCallExpr(clang::CallExpr const*, clang::CodeGen::ReturnValueSlot, llvm::CallBase**) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3fd7b96)
#28 0x000000000404f059 (anonymous namespace)::ScalarExprEmitter::VisitCallExpr(clang::CallExpr const*) CGExprScalar.cpp:0:0
#29 0x000000000404448b clang::StmtVisitorBase<std::add_pointer, (anonymous namespace)::ScalarExprEmitter, llvm::Value*>::Visit(clang::Stmt*) CGExprScalar.cpp:0:0
#30 0x0000000004049d62 clang::CodeGen::CodeGenFunction::EmitScalarExpr(clang::Expr const*, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+0x4049d62)
#31 0x0000000003fb556e clang::CodeGen::CodeGenFunction::EmitAnyExpr(clang::Expr const*, clang::CodeGen::AggValueSlot, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3fb556e)
#32 0x0000000003fc38e8 clang::CodeGen::CodeGenFunction::EmitIgnoredExpr(clang::Expr const*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3fc38e8)
#33 0x00000000041494eb clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef<clang::Attr const*>) (/opt/compiler-explorer/clang-trunk/bin/clang+0x41494eb)
#34 0x0000000004150ae9 clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&, bool, clang::CodeGen::AggValueSlot) (/opt/compiler-explorer/clang-trunk/bin/clang+0x4150ae9)
#35 0x00000000041b2e55 clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x41b2e55)
#36 0x00000000041c3f33 clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&) (/opt/compiler-explorer/clang-trunk/bin/clang+0x41c3f33)
#37 0x0000000004221f9b clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x4221f9b)
#38 0x000000000421dc74 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x421dc74)
#39 0x000000000421e573 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/compiler-explorer/clang-trunk/bin/clang+0x421e573)
#40 0x0000000004227e6f clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#41 0x0000000004577f41 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#42 0x0000000004561c29 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/compiler-explorer/clang-trunk/bin/clang+0x4561c29)
#43 0x00000000061dd004 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+0x61dd004)
#44 0x0000000004574fd5 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+0x4574fd5)
#45 0x000000000487038a clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+0x487038a)
#46 0x00000000047f0d6b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-trunk/bin/clang+0x47f0d6b)
#47 0x000000000496630b clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x496630b)
#48 0x0000000000dcc975 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+0xdcc975)
#49 0x0000000000dc48cb ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#50 0x0000000000dc496d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#51 0x00000000045e2839 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#52 0x0000000003bf36d3 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3bf36d3)
#53 0x00000000045e2a59 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#54 0x00000000045a5ce2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+0x45a5ce2)
#55 0x00000000045a6bc1 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+0x45a6bc1)
#56 0x00000000045af6fc clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-trunk/bin/clang+0x45af6fc)
#57 0x0000000000dc93c9 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-trunk/bin/clang+0xdc93c9)
#58 0x0000000000c75434 main (/opt/compiler-explorer/clang-trunk/bin/clang+0xc75434)
#59 0x000070222f029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#60 0x000070222f029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#61 0x0000000000dc4365 _start (/opt/compiler-explorer/clang-trunk/bin/clang+0xdc4365)
clang: error: clang frontend command failed with exit code 70 (use -v to see invocation)
Compiler returned: 70
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/oPx7rb5xz
```c
void foo(int, ...);

struct S {
  int a, b, c;
};

void bar(void) {
  _Atomic struct S x = (struct S){0, 1, 2};
  foo(0, x = (struct S){0, 1, 2});
}
```

Backtrace:
```console
fatal error: error in backend: Cannot implicitly convert a scalable size to a fixed-width size in `TypeSize::operator ScalarTy()`
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	&lt;source&gt;:7:6: LLVM IR generation of declaration 'bar'
3.	&lt;source&gt;:7:6: Generating code for declaration 'bar'
 #<!-- -->0 0x0000000003caee98 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3caee98)
 #<!-- -->1 0x0000000003cac86c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3cac86c)
 #<!-- -->2 0x0000000003bf37d3 llvm::CrashRecoveryContext::HandleExit(int) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3bf37d3)
 #<!-- -->3 0x0000000003ca40ae llvm::sys::Process::Exit(int, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3ca40ae)
 #<!-- -->4 0x0000000000dca91b LLVMErrorHandler(void*, char const*, bool) cc1_main.cpp:0:0
 #<!-- -->5 0x0000000003bfd563 llvm::report_fatal_error(llvm::Twine const&amp;, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3bfd563)
 #<!-- -->6 0x0000000003bfd6f9 (/opt/compiler-explorer/clang-trunk/bin/clang+0x3bfd6f9)
 #<!-- -->7 0x0000000003fd966e (/opt/compiler-explorer/clang-trunk/bin/clang+0x3fd966e)
 #<!-- -->8 0x0000000003fdf20b clang::CodeGen::CodeGenFunction::EmitLValueForField(clang::CodeGen::LValue, clang::FieldDecl const*, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3fdf20b)
 #<!-- -->9 0x0000000003fe3ab5 clang::CodeGen::CodeGenFunction::EmitLValueForFieldInitialization(clang::CodeGen::LValue, clang::FieldDecl const*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3fe3ab5)
#<!-- -->10 0x0000000003ffea0d (anonymous namespace)::AggExprEmitter::VisitCXXParenListOrInitListExpr(clang::Expr*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::FieldDecl*, clang::Expr*) CGExprAgg.cpp:0:0
#<!-- -->11 0x0000000003ffbdf8 (anonymous namespace)::AggExprEmitter::Visit(clang::Expr*) CGExprAgg.cpp:0:0
#<!-- -->12 0x0000000003ffce19 clang::CodeGen::CodeGenFunction::EmitAggExpr(clang::Expr const*, clang::CodeGen::AggValueSlot) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3ffce19)
#<!-- -->13 0x0000000003ffc9c2 (anonymous namespace)::AggExprEmitter::Visit(clang::Expr*) CGExprAgg.cpp:0:0
#<!-- -->14 0x0000000003ffa6ec (anonymous namespace)::AggExprEmitter::VisitCastExpr(clang::CastExpr*) CGExprAgg.cpp:0:0
#<!-- -->15 0x0000000003ffbda8 (anonymous namespace)::AggExprEmitter::Visit(clang::Expr*) CGExprAgg.cpp:0:0
#<!-- -->16 0x0000000003ffce19 clang::CodeGen::CodeGenFunction::EmitAggExpr(clang::Expr const*, clang::CodeGen::AggValueSlot) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3ffce19)
#<!-- -->17 0x0000000003ffb723 (anonymous namespace)::AggExprEmitter::VisitCastExpr(clang::CastExpr*) CGExprAgg.cpp:0:0
#<!-- -->18 0x0000000003ffbda8 (anonymous namespace)::AggExprEmitter::Visit(clang::Expr*) CGExprAgg.cpp:0:0
#<!-- -->19 0x0000000003ffd16e (anonymous namespace)::AggExprEmitter::VisitBinAssign(clang::BinaryOperator const*) CGExprAgg.cpp:0:0
#<!-- -->20 0x0000000003ffc68b (anonymous namespace)::AggExprEmitter::Visit(clang::Expr*) CGExprAgg.cpp:0:0
#<!-- -->21 0x0000000003ffce19 clang::CodeGen::CodeGenFunction::EmitAggExpr(clang::Expr const*, clang::CodeGen::AggValueSlot) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3ffce19)
#<!-- -->22 0x0000000003fb53b2 clang::CodeGen::CodeGenFunction::EmitAnyExpr(clang::Expr const*, clang::CodeGen::AggValueSlot, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3fb53b2)
#<!-- -->23 0x0000000003fb67eb clang::CodeGen::CodeGenFunction::EmitAnyExprToTemp(clang::Expr const*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3fb67eb)
#<!-- -->24 0x000000000447bda8 clang::CodeGen::CodeGenFunction::EmitCallArg(clang::CodeGen::CallArgList&amp;, clang::Expr const*, clang::QualType) (/opt/compiler-explorer/clang-trunk/bin/clang+0x447bda8)
#<!-- -->25 0x00000000044866f5 clang::CodeGen::CodeGenFunction::EmitCallArgs(clang::CodeGen::CallArgList&amp;, clang::CodeGen::CodeGenFunction::PrototypeWrapper, llvm::iterator_range&lt;clang::Stmt::CastIterator&lt;clang::Expr, clang::Expr const* const, clang::Stmt const* const&gt;&gt;, clang::CodeGen::CodeGenFunction::AbstractCallee, unsigned int, clang::CodeGen::CodeGenFunction::EvaluationOrder) (/opt/compiler-explorer/clang-trunk/bin/clang+0x44866f5)
#<!-- -->26 0x0000000003fd6922 clang::CodeGen::CodeGenFunction::EmitCall(clang::QualType, clang::CodeGen::CGCallee const&amp;, clang::CallExpr const*, clang::CodeGen::ReturnValueSlot, llvm::Value*, llvm::CallBase**, clang::CodeGen::CGFunctionInfo const**) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3fd6922)
#<!-- -->27 0x0000000003fd7b96 clang::CodeGen::CodeGenFunction::EmitCallExpr(clang::CallExpr const*, clang::CodeGen::ReturnValueSlot, llvm::CallBase**) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3fd7b96)
#<!-- -->28 0x000000000404f059 (anonymous namespace)::ScalarExprEmitter::VisitCallExpr(clang::CallExpr const*) CGExprScalar.cpp:0:0
#<!-- -->29 0x000000000404448b clang::StmtVisitorBase&lt;std::add_pointer, (anonymous namespace)::ScalarExprEmitter, llvm::Value*&gt;::Visit(clang::Stmt*) CGExprScalar.cpp:0:0
#<!-- -->30 0x0000000004049d62 clang::CodeGen::CodeGenFunction::EmitScalarExpr(clang::Expr const*, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+0x4049d62)
#<!-- -->31 0x0000000003fb556e clang::CodeGen::CodeGenFunction::EmitAnyExpr(clang::Expr const*, clang::CodeGen::AggValueSlot, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3fb556e)
#<!-- -->32 0x0000000003fc38e8 clang::CodeGen::CodeGenFunction::EmitIgnoredExpr(clang::Expr const*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3fc38e8)
#<!-- -->33 0x00000000041494eb clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef&lt;clang::Attr const*&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x41494eb)
#<!-- -->34 0x0000000004150ae9 clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&amp;, bool, clang::CodeGen::AggValueSlot) (/opt/compiler-explorer/clang-trunk/bin/clang+0x4150ae9)
#<!-- -->35 0x00000000041b2e55 clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x41b2e55)
#<!-- -->36 0x00000000041c3f33 clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x41c3f33)
#<!-- -->37 0x0000000004221f9b clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x4221f9b)
#<!-- -->38 0x000000000421dc74 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x421dc74)
#<!-- -->39 0x000000000421e573 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/compiler-explorer/clang-trunk/bin/clang+0x421e573)
#<!-- -->40 0x0000000004227e6f clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#<!-- -->41 0x0000000004577f41 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#<!-- -->42 0x0000000004561c29 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/compiler-explorer/clang-trunk/bin/clang+0x4561c29)
#<!-- -->43 0x00000000061dd004 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+0x61dd004)
#<!-- -->44 0x0000000004574fd5 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+0x4574fd5)
#<!-- -->45 0x000000000487038a clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+0x487038a)
#<!-- -->46 0x00000000047f0d6b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x47f0d6b)
#<!-- -->47 0x000000000496630b clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x496630b)
#<!-- -->48 0x0000000000dcc975 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+0xdcc975)
#<!-- -->49 0x0000000000dc48cb ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->50 0x0000000000dc496d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->51 0x00000000045e2839 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->52 0x0000000003bf36d3 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3bf36d3)
#<!-- -->53 0x00000000045e2a59 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->54 0x00000000045a5ce2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+0x45a5ce2)
#<!-- -->55 0x00000000045a6bc1 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+0x45a6bc1)
#<!-- -->56 0x00000000045af6fc clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x45af6fc)
#<!-- -->57 0x0000000000dc93c9 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0xdc93c9)
#<!-- -->58 0x0000000000c75434 main (/opt/compiler-explorer/clang-trunk/bin/clang+0xc75434)
#<!-- -->59 0x000070222f029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->60 0x000070222f029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->61 0x0000000000dc4365 _start (/opt/compiler-explorer/clang-trunk/bin/clang+0xdc4365)
clang: error: clang frontend command failed with exit code 70 (use -v to see invocation)
Compiler returned: 70
```
</details>


---

### Comment 2 - MacDue

I think the scalable type error is a red herring, if you compile with assertions you can see a `cast<StructType>` fails:  https://godbolt.org/z/54ahhjrnd

<details>
<pre>
clang: /root/llvm-project/llvm/include/llvm/Support/Casting.h:573: decltype(auto) llvm::cast(From*) [with To = llvm::StructType; From = llvm::Type]: Assertion `isa<To>(Val) && "cast<Ty>() argument of incompatible type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<eof> parser at end of file
2.	<source>:7:6: LLVM IR generation of declaration 'bar'
3.	<source>:7:6: Generating code for declaration 'bar'
 #0 0x00000000041eba08 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x41eba08)
 #1 0x00000000041e8e34 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x41e8e34)
 #2 0x000000000412d788 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007ca0f4442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007ca0f44969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007ca0f4442476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007ca0f44287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007ca0f442871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007ca0f4439e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x000000000454e019 emitAddrOfFieldStorage(clang::CodeGen::CodeGenFunction&, clang::CodeGen::Address, clang::FieldDecl const*, bool) CGExpr.cpp:0:0
#10 0x000000000454e830 clang::CodeGen::CodeGenFunction::EmitLValueForField(clang::CodeGen::LValue, clang::FieldDecl const*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x454e830)
#11 0x0000000004554265 clang::CodeGen::CodeGenFunction::EmitLValueForFieldInitialization(clang::CodeGen::LValue, clang::FieldDecl const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4554265)
#12 0x0000000004572cc9 (anonymous namespace)::AggExprEmitter::VisitCXXParenListOrInitListExpr(clang::Expr*, llvm::ArrayRef<clang::Expr*>, clang::FieldDecl*, clang::Expr*) CGExprAgg.cpp:0:0
#13 0x000000000457038d (anonymous namespace)::AggExprEmitter::Visit(clang::Expr*) CGExprAgg.cpp:0:0
#14 0x0000000004570c0e clang::CodeGen::CodeGenFunction::EmitAggExpr(clang::Expr const*, clang::CodeGen::AggValueSlot) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4570c0e)
#15 0x0000000004571c5e (anonymous namespace)::AggExprEmitter::VisitCompoundLiteralExpr(clang::CompoundLiteralExpr*) CGExprAgg.cpp:0:0
#16 0x000000000456ff8a (anonymous namespace)::AggExprEmitter::Visit(clang::Expr*) CGExprAgg.cpp:0:0
#17 0x0000000004574557 (anonymous namespace)::AggExprEmitter::VisitCastExpr(clang::CastExpr*) CGExprAgg.cpp:0:0
#18 0x000000000456ff11 (anonymous namespace)::AggExprEmitter::Visit(clang::Expr*) CGExprAgg.cpp:0:0
#19 0x0000000004570c0e clang::CodeGen::CodeGenFunction::EmitAggExpr(clang::Expr const*, clang::CodeGen::AggValueSlot) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4570c0e)
#20 0x00000000045755d5 (anonymous namespace)::AggExprEmitter::VisitCastExpr(clang::CastExpr*) CGExprAgg.cpp:0:0
#21 0x000000000456ff11 (anonymous namespace)::AggExprEmitter::Visit(clang::Expr*) CGExprAgg.cpp:0:0
#22 0x0000000004570ff9 (anonymous namespace)::AggExprEmitter::VisitBinAssign(clang::BinaryOperator const*) CGExprAgg.cpp:0:0
#23 0x00000000045705cb (anonymous namespace)::AggExprEmitter::Visit(clang::Expr*) CGExprAgg.cpp:0:0
#24 0x0000000004570c0e clang::CodeGen::CodeGenFunction::EmitAggExpr(clang::Expr const*, clang::CodeGen::AggValueSlot) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4570c0e)
#25 0x0000000004528f93 clang::CodeGen::CodeGenFunction::EmitAnyExpr(clang::Expr const*, clang::CodeGen::AggValueSlot, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4528f93)
#26 0x00000000045291ac clang::CodeGen::CodeGenFunction::EmitAnyExprToTemp(clang::Expr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x45291ac)
#27 0x0000000004a317a3 clang::CodeGen::CodeGenFunction::EmitCallArg(clang::CodeGen::CallArgList&, clang::Expr const*, clang::QualType) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4a317a3)
#28 0x0000000004a3cd85 clang::CodeGen::CodeGenFunction::EmitCallArgs(clang::CodeGen::CallArgList&, clang::CodeGen::CodeGenFunction::PrototypeWrapper, llvm::iterator_range<clang::Stmt::CastIterator<clang::Expr, clang::Expr const* const, clang::Stmt const* const>>, clang::CodeGen::CodeGenFunction::AbstractCallee, unsigned int, clang::CodeGen::CodeGenFunction::EvaluationOrder) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4a3cd85)
#29 0x000000000455dfb3 clang::CodeGen::CodeGenFunction::EmitCall(clang::QualType, clang::CodeGen::CGCallee const&, clang::CallExpr const*, clang::CodeGen::ReturnValueSlot, llvm::Value*, llvm::CallBase**, clang::CodeGen::CGFunctionInfo const**) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x455dfb3)
#30 0x000000000455f00d clang::CodeGen::CodeGenFunction::EmitCallExpr(clang::CallExpr const*, clang::CodeGen::ReturnValueSlot, llvm::CallBase**) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x455f00d)
#31 0x00000000045cc954 (anonymous namespace)::ScalarExprEmitter::VisitCallExpr(clang::CallExpr const*) CGExprScalar.cpp:0:0
#32 0x00000000045c13b0 clang::StmtVisitorBase<std::add_pointer, (anonymous namespace)::ScalarExprEmitter, llvm::Value*>::Visit(clang::Stmt*) CGExprScalar.cpp:0:0
#33 0x00000000045c5b5c clang::CodeGen::CodeGenFunction::EmitScalarExpr(clang::Expr const*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x45c5b5c)
#34 0x0000000004528f1e clang::CodeGen::CodeGenFunction::EmitAnyExpr(clang::Expr const*, clang::CodeGen::AggValueSlot, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4528f1e)
#35 0x000000000455a50d clang::CodeGen::CodeGenFunction::EmitIgnoredExpr(clang::Expr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x455a50d)
#36 0x00000000046da357 clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef<clang::Attr const*>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x46da357)
#37 0x00000000046e25ec clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&, bool, clang::CodeGen::AggValueSlot) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x46e25ec)
#38 0x000000000474ab6e clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x474ab6e)
#39 0x000000000475cbb4 clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x475cbb4)
#40 0x00000000047c9830 clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x47c9830)
#41 0x00000000047c4714 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x47c4714)
#42 0x00000000047c55b0 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x47c55b0)
#43 0x00000000047cfd83 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#44 0x0000000004b35049 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#45 0x0000000004b24174 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4b24174)
#46 0x00000000068c8ec4 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x68c8ec4)
#47 0x0000000004b30a48 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4b30a48)
#48 0x0000000004e1c8d5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4e1c8d5)
#49 0x0000000004d9df1e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4d9df1e)
#50 0x0000000004f13b8d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4f13b8d)
#51 0x0000000000dc14d0 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xdc14d0)
#52 0x0000000000db803a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#53 0x0000000000db81bd int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#54 0x0000000004b99bd9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#55 0x000000000412dc24 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x412dc24)
#56 0x0000000004b9a1ef clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#57 0x0000000004b5b502 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4b5b502)
#58 0x0000000004b5c4ae clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4b5c4ae)
#59 0x0000000004b63be5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4b63be5)
#60 0x0000000000dbd9f9 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xdbd9f9)
#61 0x0000000000c6d754 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xc6d754)
#62 0x00007ca0f4429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#63 0x00007ca0f4429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#64 0x0000000000db7ad5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xdb7ad5)
</pre>
</details>


---

