# Clang crash in CodeGen with templated member enum and dependent underlying type

**Author:** vrukesh
**URL:** https://github.com/llvm/llvm-project/issues/176752
**Status:** Open
**Labels:** clang:codegen, crash

## Body

**What code / commands /steps will reproduce the problem?**

Compile the sample code:

```c++
template<typename T>
struct bar { enum city : int; };

template<typename T>
enum bar<T>::city : T { london };

int main() { 
    bar<int>::city e = bar<int>::london;
    return e == london;
}
```

Compiler Explorer link: https://godbolt.org/z/4W4z8sv61

**What is the expected result?**
Compilation should fail, and there should be clear diagnostic about the member enum declaration/definition.


**What happens instead?**
**g++ (GCC) compilation fails.**

```
<source>:5:21: error: different underlying type in enum 'enum bar<T>::city' [-Wtemplate-body]
    5 | enum bar<T>::city : T { london };
      |                     ^
<source>:2:26: note: previous definition here
    2 | struct bar { enum city : int; };
      |                          ^~~
<source>: In function 'int main()':
<source>:9:17: error: 'london' was not declared in this scope
    9 |     return e == london;
      |                 ^~~~~~
Compiler returned: 1
```

**Clang frontend segfaults in CodeGen (`clang::CodeGenFunction::EmitLoadOfScalar`).**

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++17 <source>
1.	<eof> parser at end of file
2.	<source>:7:5: LLVM IR generation of declaration 'main'
3.	<source>:7:5: Generating code for declaration 'main'
4.	<source>:5:25: Mangling declaration 'bar::london'
 #0 0x0000000003d563b8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3d563b8)
 #1 0x0000000003d53dac llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3d53dac)
 #2 0x0000000003c99588 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x0000768fb5842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00000000077d5019 (anonymous namespace)::CXXNameMangler::mangle(clang::GlobalDecl) ItaniumMangle.cpp:0:0
 #5 0x00000000077d99c7 (anonymous namespace)::ItaniumMangleContextImpl::mangleCXXName(clang::GlobalDecl, llvm::raw_ostream&) ItaniumMangle.cpp:0:0
 #6 0x000000000428fe1b getMangledNameImpl(clang::CodeGen::CodeGenModule&, clang::GlobalDecl, clang::NamedDecl const*, bool) CodeGenModule.cpp:0:0
 #7 0x00000000042a07b3 clang::CodeGen::CodeGenModule::getMangledName(clang::GlobalDecl) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x42a07b3)
 #8 0x00000000042a1247 clang::CodeGen::CodeGenModule::GetAddrOfTemplateParamObject(clang::TemplateParamObjectDecl const*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x42a1247)
 #9 0x0000000004089dd5 clang::CodeGen::CodeGenFunction::EmitDeclRefLValue(clang::DeclRefExpr const*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4089dd5)
#10 0x000000000409384b clang::CodeGen::CodeGenFunction::EmitLValueHelper(clang::Expr const*, clang::CodeGen::KnownNonNull_t) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x409384b)
#11 0x0000000004094257 void llvm::function_ref<void ()>::callback_fn<clang::CodeGen::CodeGenFunction::EmitLValue(clang::Expr const*, clang::CodeGen::KnownNonNull_t)::'lambda'()>(long) CGExpr.cpp:0:0
#12 0x0000000007c6ca11 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x7c6ca11)
#13 0x000000000408fbbb clang::CodeGen::CodeGenFunction::EmitCheckedLValue(clang::Expr const*, clang::CodeGen::CodeGenFunction::TypeCheckKind) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x408fbbb)
#14 0x00000000040f25a0 (anonymous namespace)::ScalarExprEmitter::EmitLoadOfLValue(clang::Expr const*) CGExprScalar.cpp:0:0
#15 0x00000000040ed7d1 clang::StmtVisitorBase<std::add_pointer, (anonymous namespace)::ScalarExprEmitter, llvm::Value*>::Visit(clang::Stmt*) CGExprScalar.cpp:0:0
#16 0x00000000040fbcd6 (anonymous namespace)::ScalarExprEmitter::EmitBinOps(clang::BinaryOperator const*, clang::QualType) CGExprScalar.cpp:0:0
#17 0x00000000040fc095 (anonymous namespace)::ScalarExprEmitter::EmitCompare(clang::BinaryOperator const*, llvm::CmpInst::Predicate, llvm::CmpInst::Predicate, llvm::CmpInst::Predicate, bool) CGExprScalar.cpp:0:0
#18 0x00000000040ecca5 clang::StmtVisitorBase<std::add_pointer, (anonymous namespace)::ScalarExprEmitter, llvm::Value*>::Visit(clang::Stmt*) CGExprScalar.cpp:0:0
#19 0x00000000040f0b42 clang::CodeGen::CodeGenFunction::EmitScalarExpr(clang::Expr const*, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x40f0b42)
#20 0x00000000041f777b clang::CodeGen::CodeGenFunction::EmitReturnStmt(clang::ReturnStmt const&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x41f777b)
#21 0x0000000004200a33 clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef<clang::Attr const*>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4200a33)
#22 0x0000000004207b0b clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&, bool, clang::CodeGen::AggValueSlot) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4207b0b)
#23 0x0000000004267ced clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4267ced)
#24 0x0000000004278963 clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4278963)
#25 0x00000000042d5e0b clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x42d5e0b)
#26 0x00000000042d1404 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x42d1404)
#27 0x00000000042d2030 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x42d2030)
#28 0x00000000042dca5f clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#29 0x000000000462a3b1 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#30 0x00000000046152e9 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x46152e9)
#31 0x0000000006294294 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6294294)
#32 0x0000000004628655 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4628655)
#33 0x000000000493c63a clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x493c63a)
#34 0x00000000048ba29b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x48ba29b)
#35 0x0000000004a328cb clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4a328cb)
#36 0x0000000000de5fcd cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xde5fcd)
#37 0x0000000000ddddfb ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#38 0x0000000000ddde9d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#39 0x00000000046a30b9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#40 0x0000000003c999a3 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c999a3)
#41 0x00000000046a32d9 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#42 0x00000000046663c2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x46663c2)
#43 0x00000000046672a1 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x46672a1)
#44 0x000000000466fd9c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x466fd9c)
#45 0x0000000000de28c9 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xde28c9)
#46 0x0000000000c97e54 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc97e54)
#47 0x0000768fb5829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#48 0x0000768fb5829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#49 0x0000000000ddd895 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xddd895)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: Vrukesh Panse (vrukesh)

<details>
**What code / commands /steps will reproduce the problem?**

Compile the sample code:

`
template&lt;typename T&gt;
struct bar { enum city : int; };

template&lt;typename T&gt;
enum bar&lt;T&gt;::city : T { london };

int main() { 
    bar&lt;int&gt;::city e = bar&lt;int&gt;::london;
    return e == london;
}
`

Compiler Explorer link: https://godbolt.org/z/4W4z8sv61

**What is the expected result?**
Compilation should fail, and there should be clear diagnostic about the member enum declaration/definition.


**What happens instead?**
**g++ (GCC) compilation fails.**

`
&lt;source&gt;:5:21: error: different underlying type in enum 'enum bar&lt;T&gt;::city' [-Wtemplate-body]
    5 | enum bar&lt;T&gt;::city : T { london };
      |                     ^
&lt;source&gt;:2:26: note: previous definition here
    2 | struct bar { enum city : int; };
      |                          ^~~
&lt;source&gt;: In function 'int main()':
&lt;source&gt;:9:17: error: 'london' was not declared in this scope
    9 |     return e == london;
      |                 ^~~~~~
Compiler returned: 1
`

**Clang frontend segfaults in CodeGen (`clang::CodeGenFunction::EmitLoadOfScalar`).**

`
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++17 &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	&lt;source&gt;:7:5: LLVM IR generation of declaration 'main'
3.	&lt;source&gt;:7:5: Generating code for declaration 'main'
4.	&lt;source&gt;:5:25: Mangling declaration 'bar::london'
 #<!-- -->0 0x0000000003d563b8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3d563b8)
 #<!-- -->1 0x0000000003d53dac llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3d53dac)
 #<!-- -->2 0x0000000003c99588 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x0000768fb5842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00000000077d5019 (anonymous namespace)::CXXNameMangler::mangle(clang::GlobalDecl) ItaniumMangle.cpp:0:0
 #<!-- -->5 0x00000000077d99c7 (anonymous namespace)::ItaniumMangleContextImpl::mangleCXXName(clang::GlobalDecl, llvm::raw_ostream&amp;) ItaniumMangle.cpp:0:0
 #<!-- -->6 0x000000000428fe1b getMangledNameImpl(clang::CodeGen::CodeGenModule&amp;, clang::GlobalDecl, clang::NamedDecl const*, bool) CodeGenModule.cpp:0:0
 #<!-- -->7 0x00000000042a07b3 clang::CodeGen::CodeGenModule::getMangledName(clang::GlobalDecl) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x42a07b3)
 #<!-- -->8 0x00000000042a1247 clang::CodeGen::CodeGenModule::GetAddrOfTemplateParamObject(clang::TemplateParamObjectDecl const*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x42a1247)
 #<!-- -->9 0x0000000004089dd5 clang::CodeGen::CodeGenFunction::EmitDeclRefLValue(clang::DeclRefExpr const*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4089dd5)
#<!-- -->10 0x000000000409384b clang::CodeGen::CodeGenFunction::EmitLValueHelper(clang::Expr const*, clang::CodeGen::KnownNonNull_t) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x409384b)
#<!-- -->11 0x0000000004094257 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::CodeGen::CodeGenFunction::EmitLValue(clang::Expr const*, clang::CodeGen::KnownNonNull_t)::'lambda'()&gt;(long) CGExpr.cpp:0:0
#<!-- -->12 0x0000000007c6ca11 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x7c6ca11)
#<!-- -->13 0x000000000408fbbb clang::CodeGen::CodeGenFunction::EmitCheckedLValue(clang::Expr const*, clang::CodeGen::CodeGenFunction::TypeCheckKind) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x408fbbb)
#<!-- -->14 0x00000000040f25a0 (anonymous namespace)::ScalarExprEmitter::EmitLoadOfLValue(clang::Expr const*) CGExprScalar.cpp:0:0
#<!-- -->15 0x00000000040ed7d1 clang::StmtVisitorBase&lt;std::add_pointer, (anonymous namespace)::ScalarExprEmitter, llvm::Value*&gt;::Visit(clang::Stmt*) CGExprScalar.cpp:0:0
#<!-- -->16 0x00000000040fbcd6 (anonymous namespace)::ScalarExprEmitter::EmitBinOps(clang::BinaryOperator const*, clang::QualType) CGExprScalar.cpp:0:0
#<!-- -->17 0x00000000040fc095 (anonymous namespace)::ScalarExprEmitter::EmitCompare(clang::BinaryOperator const*, llvm::CmpInst::Predicate, llvm::CmpInst::Predicate, llvm::CmpInst::Predicate, bool) CGExprScalar.cpp:0:0
#<!-- -->18 0x00000000040ecca5 clang::StmtVisitorBase&lt;std::add_pointer, (anonymous namespace)::ScalarExprEmitter, llvm::Value*&gt;::Visit(clang::Stmt*) CGExprScalar.cpp:0:0
#<!-- -->19 0x00000000040f0b42 clang::CodeGen::CodeGenFunction::EmitScalarExpr(clang::Expr const*, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x40f0b42)
#<!-- -->20 0x00000000041f777b clang::CodeGen::CodeGenFunction::EmitReturnStmt(clang::ReturnStmt const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x41f777b)
#<!-- -->21 0x0000000004200a33 clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef&lt;clang::Attr const*&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4200a33)
#<!-- -->22 0x0000000004207b0b clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&amp;, bool, clang::CodeGen::AggValueSlot) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4207b0b)
#<!-- -->23 0x0000000004267ced clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4267ced)
#<!-- -->24 0x0000000004278963 clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4278963)
#<!-- -->25 0x00000000042d5e0b clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x42d5e0b)
#<!-- -->26 0x00000000042d1404 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x42d1404)
#<!-- -->27 0x00000000042d2030 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x42d2030)
#<!-- -->28 0x00000000042dca5f clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#<!-- -->29 0x000000000462a3b1 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#<!-- -->30 0x00000000046152e9 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x46152e9)
#<!-- -->31 0x0000000006294294 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6294294)
#<!-- -->32 0x0000000004628655 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4628655)
#<!-- -->33 0x000000000493c63a clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x493c63a)
#<!-- -->34 0x00000000048ba29b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x48ba29b)
#<!-- -->35 0x0000000004a328cb clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4a328cb)
#<!-- -->36 0x0000000000de5fcd cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xde5fcd)
#<!-- -->37 0x0000000000ddddfb ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->38 0x0000000000ddde9d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->39 0x00000000046a30b9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->40 0x0000000003c999a3 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c999a3)
#<!-- -->41 0x00000000046a32d9 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->42 0x00000000046663c2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x46663c2)
#<!-- -->43 0x00000000046672a1 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x46672a1)
#<!-- -->44 0x000000000466fd9c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x466fd9c)
#<!-- -->45 0x0000000000de28c9 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xde28c9)
#<!-- -->46 0x0000000000c97e54 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc97e54)
#<!-- -->47 0x0000768fb5829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->48 0x0000768fb5829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->49 0x0000000000ddd895 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xddd895)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
`

</details>


---

