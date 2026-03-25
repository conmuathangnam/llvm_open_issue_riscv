# [Clang] [CodeGen] Assertion when trying to pass a prvalue of matrix type to a function

**Author:** Sirraide
**URL:** https://github.com/llvm/llvm-project/issues/118604

## Body

This currently asserts on trunk (https://godbolt.org/z/5PzGbK39v):
```c++
using m5x5 = double __attribute__((matrix_type(5, 5)));
void f(m5x5 t) {}
void g() { f(m5x5()); }
```

The actual crash is in `llvm::ConstantExpr::getCast`, but this is probably Clang giving it bad input.

Assertion:
```console
clang++: /root/llvm-project/llvm/lib/IR/Constants.cpp:2219: 
static llvm::Constant* llvm::ConstantExpr::getCast(
    unsigned int, llvm::Constant*, llvm::Type*, bool): 
Assertion `CastInst::castIsValid(opc, C, Ty) && "Invalid constantexpr cast!"' failed.
```

Stack trace:
```console
#0 0x0000000003bf9868 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3bf9868)
 #1 0x0000000003bf756c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3bf756c)
 #2 0x0000000003b44a48 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007ed970e42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007ed970e969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007ed970e42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007ed970e287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007ed970e2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007ed970e39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000003450fbb llvm::ConstantExpr::getCast(unsigned int, llvm::Constant*, llvm::Type*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3450fbb)
#10 0x0000000000d36ce1 llvm::IRBuilderBase::CreateCast(llvm::Instruction::CastOps, llvm::Value*, llvm::Type*, llvm::Twine const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd36ce1)
#11 0x0000000004374f17 clang::CodeGen::CodeGenFunction::EmitCall(clang::CodeGen::CGFunctionInfo const&, clang::CodeGen::CGCallee const&, clang::CodeGen::ReturnValueSlot, clang::CodeGen::CallArgList const&, llvm::CallBase**, bool, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4374f17)
#12 0x00000000043ffa96 clang::CodeGen::CodeGenFunction::EmitCall(clang::QualType, clang::CodeGen::CGCallee const&, clang::CallExpr const*, clang::CodeGen::ReturnValueSlot, llvm::Value*, llvm::CallBase**, clang::CodeGen::CGFunctionInfo const**) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x43ffa96)
#13 0x0000000004400455 clang::CodeGen::CodeGenFunction::EmitCallExpr(clang::CallExpr const*, clang::CodeGen::ReturnValueSlot, llvm::CallBase**) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4400455)
#14 0x0000000004457d34 (anonymous namespace)::ScalarExprEmitter::VisitCallExpr(clang::CallExpr const*) CGExprScalar.cpp:0:0
#15 0x000000000444bbeb clang::StmtVisitorBase<std::add_pointer, (anonymous namespace)::ScalarExprEmitter, llvm::Value*>::Visit(clang::Stmt*) CGExprScalar.cpp:0:0
#16 0x000000000445187c clang::CodeGen::CodeGenFunction::EmitScalarExpr(clang::Expr const*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x445187c)
#17 0x00000000043dabfe clang::CodeGen::CodeGenFunction::EmitAnyExpr(clang::Expr const*, clang::CodeGen::AggValueSlot, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x43dabfe)
#18 0x00000000043fe985 clang::CodeGen::CodeGenFunction::EmitIgnoredExpr(clang::Expr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x43fe985)
#19 0x0000000003f609d5 clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef<clang::Attr const*>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f609d5)
#20 0x0000000003f68714 clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&, bool, clang::CodeGen::AggValueSlot) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f68714)
#21 0x0000000003fd1764 clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3fd1764)
#22 0x0000000003fe472c clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3fe472c)
#23 0x000000000404c06a clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x404c06a)
#24 0x00000000040472f5 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x40472f5)
#25 0x00000000040476db clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x40476db)
#26 0x0000000004051ea3 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#27 0x000000000457fd56 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#28 0x0000000004570a58 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4570a58)
#29 0x0000000006522794 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6522794)
#30 0x000000000457d908 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x457d908)
#31 0x0000000004838839 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4838839)
#32 0x00000000047b86fe clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x47b86fe)
#33 0x000000000492374e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x492374e)
#34 0x0000000000caeaef cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xcaeaef)
#35 0x0000000000ca668a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#36 0x00000000045c0b59 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#37 0x0000000003b44ef4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3b44ef4)
#38 0x00000000045c114f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#39 0x00000000045873ed clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x45873ed)
#40 0x00000000045884dd clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x45884dd)
#41 0x000000000458f895 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x458f895)
#42 0x0000000000cab933 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xcab933)
#43 0x0000000000b801d4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xb801d4)
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: None (Sirraide)

<details>
This currently asserts on trunk (https://godbolt.org/z/5PzGbK39v):
```c++
using m5x5 = double __attribute__((matrix_type(5, 5)));
void f(m5x5 t) {}
void g() { f(m5x5()); }
```

The actual crash is in `llvm::ConstantExpr::getCast`, but this is probably Clang giving it bad input.

Assertion:
```console
clang++: /root/llvm-project/llvm/lib/IR/Constants.cpp:2219: 
static llvm::Constant* llvm::ConstantExpr::getCast(
    unsigned int, llvm::Constant*, llvm::Type*, bool): 
Assertion `CastInst::castIsValid(opc, C, Ty) &amp;&amp; "Invalid constantexpr cast!"' failed.
```

Stack trace:
```console
#<!-- -->0 0x0000000003bf9868 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3bf9868)
 #<!-- -->1 0x0000000003bf756c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3bf756c)
 #<!-- -->2 0x0000000003b44a48 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007ed970e42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007ed970e969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007ed970e42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007ed970e287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x00007ed970e2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x00007ed970e39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000003450fbb llvm::ConstantExpr::getCast(unsigned int, llvm::Constant*, llvm::Type*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3450fbb)
#<!-- -->10 0x0000000000d36ce1 llvm::IRBuilderBase::CreateCast(llvm::Instruction::CastOps, llvm::Value*, llvm::Type*, llvm::Twine const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd36ce1)
#<!-- -->11 0x0000000004374f17 clang::CodeGen::CodeGenFunction::EmitCall(clang::CodeGen::CGFunctionInfo const&amp;, clang::CodeGen::CGCallee const&amp;, clang::CodeGen::ReturnValueSlot, clang::CodeGen::CallArgList const&amp;, llvm::CallBase**, bool, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4374f17)
#<!-- -->12 0x00000000043ffa96 clang::CodeGen::CodeGenFunction::EmitCall(clang::QualType, clang::CodeGen::CGCallee const&amp;, clang::CallExpr const*, clang::CodeGen::ReturnValueSlot, llvm::Value*, llvm::CallBase**, clang::CodeGen::CGFunctionInfo const**) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x43ffa96)
#<!-- -->13 0x0000000004400455 clang::CodeGen::CodeGenFunction::EmitCallExpr(clang::CallExpr const*, clang::CodeGen::ReturnValueSlot, llvm::CallBase**) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4400455)
#<!-- -->14 0x0000000004457d34 (anonymous namespace)::ScalarExprEmitter::VisitCallExpr(clang::CallExpr const*) CGExprScalar.cpp:0:0
#<!-- -->15 0x000000000444bbeb clang::StmtVisitorBase&lt;std::add_pointer, (anonymous namespace)::ScalarExprEmitter, llvm::Value*&gt;::Visit(clang::Stmt*) CGExprScalar.cpp:0:0
#<!-- -->16 0x000000000445187c clang::CodeGen::CodeGenFunction::EmitScalarExpr(clang::Expr const*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x445187c)
#<!-- -->17 0x00000000043dabfe clang::CodeGen::CodeGenFunction::EmitAnyExpr(clang::Expr const*, clang::CodeGen::AggValueSlot, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x43dabfe)
#<!-- -->18 0x00000000043fe985 clang::CodeGen::CodeGenFunction::EmitIgnoredExpr(clang::Expr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x43fe985)
#<!-- -->19 0x0000000003f609d5 clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef&lt;clang::Attr const*&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f609d5)
#<!-- -->20 0x0000000003f68714 clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&amp;, bool, clang::CodeGen::AggValueSlot) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f68714)
#<!-- -->21 0x0000000003fd1764 clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3fd1764)
#<!-- -->22 0x0000000003fe472c clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3fe472c)
#<!-- -->23 0x000000000404c06a clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x404c06a)
#<!-- -->24 0x00000000040472f5 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x40472f5)
#<!-- -->25 0x00000000040476db clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x40476db)
#<!-- -->26 0x0000000004051ea3 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#<!-- -->27 0x000000000457fd56 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#<!-- -->28 0x0000000004570a58 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4570a58)
#<!-- -->29 0x0000000006522794 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6522794)
#<!-- -->30 0x000000000457d908 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x457d908)
#<!-- -->31 0x0000000004838839 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4838839)
#<!-- -->32 0x00000000047b86fe clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x47b86fe)
#<!-- -->33 0x000000000492374e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x492374e)
#<!-- -->34 0x0000000000caeaef cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xcaeaef)
#<!-- -->35 0x0000000000ca668a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->36 0x00000000045c0b59 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->37 0x0000000003b44ef4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3b44ef4)
#<!-- -->38 0x00000000045c114f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->39 0x00000000045873ed clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x45873ed)
#<!-- -->40 0x00000000045884dd clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x45884dd)
#<!-- -->41 0x000000000458f895 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x458f895)
#<!-- -->42 0x0000000000cab933 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xcab933)
#<!-- -->43 0x0000000000b801d4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xb801d4)
```
</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (Sirraide)

<details>
This currently asserts on trunk (https://godbolt.org/z/5PzGbK39v):
```c++
using m5x5 = double __attribute__((matrix_type(5, 5)));
void f(m5x5 t) {}
void g() { f(m5x5()); }
```

The actual crash is in `llvm::ConstantExpr::getCast`, but this is probably Clang giving it bad input.

Assertion:
```console
clang++: /root/llvm-project/llvm/lib/IR/Constants.cpp:2219: 
static llvm::Constant* llvm::ConstantExpr::getCast(
    unsigned int, llvm::Constant*, llvm::Type*, bool): 
Assertion `CastInst::castIsValid(opc, C, Ty) &amp;&amp; "Invalid constantexpr cast!"' failed.
```

Stack trace:
```console
#<!-- -->0 0x0000000003bf9868 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3bf9868)
 #<!-- -->1 0x0000000003bf756c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3bf756c)
 #<!-- -->2 0x0000000003b44a48 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007ed970e42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007ed970e969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007ed970e42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007ed970e287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x00007ed970e2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x00007ed970e39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000003450fbb llvm::ConstantExpr::getCast(unsigned int, llvm::Constant*, llvm::Type*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3450fbb)
#<!-- -->10 0x0000000000d36ce1 llvm::IRBuilderBase::CreateCast(llvm::Instruction::CastOps, llvm::Value*, llvm::Type*, llvm::Twine const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd36ce1)
#<!-- -->11 0x0000000004374f17 clang::CodeGen::CodeGenFunction::EmitCall(clang::CodeGen::CGFunctionInfo const&amp;, clang::CodeGen::CGCallee const&amp;, clang::CodeGen::ReturnValueSlot, clang::CodeGen::CallArgList const&amp;, llvm::CallBase**, bool, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4374f17)
#<!-- -->12 0x00000000043ffa96 clang::CodeGen::CodeGenFunction::EmitCall(clang::QualType, clang::CodeGen::CGCallee const&amp;, clang::CallExpr const*, clang::CodeGen::ReturnValueSlot, llvm::Value*, llvm::CallBase**, clang::CodeGen::CGFunctionInfo const**) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x43ffa96)
#<!-- -->13 0x0000000004400455 clang::CodeGen::CodeGenFunction::EmitCallExpr(clang::CallExpr const*, clang::CodeGen::ReturnValueSlot, llvm::CallBase**) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4400455)
#<!-- -->14 0x0000000004457d34 (anonymous namespace)::ScalarExprEmitter::VisitCallExpr(clang::CallExpr const*) CGExprScalar.cpp:0:0
#<!-- -->15 0x000000000444bbeb clang::StmtVisitorBase&lt;std::add_pointer, (anonymous namespace)::ScalarExprEmitter, llvm::Value*&gt;::Visit(clang::Stmt*) CGExprScalar.cpp:0:0
#<!-- -->16 0x000000000445187c clang::CodeGen::CodeGenFunction::EmitScalarExpr(clang::Expr const*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x445187c)
#<!-- -->17 0x00000000043dabfe clang::CodeGen::CodeGenFunction::EmitAnyExpr(clang::Expr const*, clang::CodeGen::AggValueSlot, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x43dabfe)
#<!-- -->18 0x00000000043fe985 clang::CodeGen::CodeGenFunction::EmitIgnoredExpr(clang::Expr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x43fe985)
#<!-- -->19 0x0000000003f609d5 clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef&lt;clang::Attr const*&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f609d5)
#<!-- -->20 0x0000000003f68714 clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&amp;, bool, clang::CodeGen::AggValueSlot) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f68714)
#<!-- -->21 0x0000000003fd1764 clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3fd1764)
#<!-- -->22 0x0000000003fe472c clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3fe472c)
#<!-- -->23 0x000000000404c06a clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x404c06a)
#<!-- -->24 0x00000000040472f5 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x40472f5)
#<!-- -->25 0x00000000040476db clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x40476db)
#<!-- -->26 0x0000000004051ea3 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#<!-- -->27 0x000000000457fd56 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#<!-- -->28 0x0000000004570a58 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4570a58)
#<!-- -->29 0x0000000006522794 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6522794)
#<!-- -->30 0x000000000457d908 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x457d908)
#<!-- -->31 0x0000000004838839 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4838839)
#<!-- -->32 0x00000000047b86fe clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x47b86fe)
#<!-- -->33 0x000000000492374e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x492374e)
#<!-- -->34 0x0000000000caeaef cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xcaeaef)
#<!-- -->35 0x0000000000ca668a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->36 0x00000000045c0b59 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->37 0x0000000003b44ef4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3b44ef4)
#<!-- -->38 0x00000000045c114f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->39 0x00000000045873ed clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x45873ed)
#<!-- -->40 0x00000000045884dd clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x45884dd)
#<!-- -->41 0x000000000458f895 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x458f895)
#<!-- -->42 0x0000000000cab933 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xcab933)
#<!-- -->43 0x0000000000b801d4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xb801d4)
```
</details>


---

