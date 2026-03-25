# [Clang] Crash when mangling lambda with requires clause

**Author:** MagentaTreehouse
**URL:** https://github.com/llvm/llvm-project/issues/123854

## Body

The following C++20 code causes a crash since Clang 18:
```c++
template <class T>
constexpr auto f() {
    return [] () requires requires (T x) { x; } {};
}

int main() {
    f<int>()();
}
```

See https://compiler-explorer.com/z/z475c49eW.

Assertion:
```console
clang++: /root/llvm-project/clang/lib/AST/ItaniumMangle.cpp:5919:
void {anonymous}::CXXNameMangler::mangleFunctionParam(const clang::ParmVarDecl*):
Assertion `parmDepth < FunctionTypeDepth.getDepth()' failed.
```

<details>

<summary>Stack dump</summary>

```console
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 <source>
1.	<eof> parser at end of file
2.	<source>:6:5: LLVM IR generation of declaration 'main'
3.	<source>:6:5: Generating code for declaration 'main'
4.	<source>:3:12: Mangling declaration 'f()::(anonymous class)::operator()'
 #0 0x0000000003cb5ea8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3cb5ea8)
 #1 0x0000000003cb3bb4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3cb3bb4)
 #2 0x0000000003c00088 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x0000757fbf442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x0000757fbf4969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x0000757fbf442476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x0000757fbf4287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x0000757fbf42871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x0000757fbf439e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000007a9ca8d (anonymous namespace)::CXXNameMangler::mangleFunctionParam(clang::ParmVarDecl const*) ItaniumMangle.cpp:0:0
#10 0x0000000007a9fd15 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool)::'lambda0'(clang::NamedDecl const*)::operator()(clang::NamedDecl const*) const (.isra.0) ItaniumMangle.cpp:0:0
#11 0x0000000007a9869f (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#12 0x0000000007aa8eba (anonymous namespace)::CXXNameMangler::mangleRequirement(clang::SourceLocation, clang::concepts::Requirement const*) ItaniumMangle.cpp:0:0
#13 0x0000000007a99405 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#14 0x0000000007a9cfdb (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) ItaniumMangle.cpp:0:0
#15 0x0000000007a9e4e0 (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#16 0x0000000007a9f5c7 (anonymous namespace)::CXXNameMangler::mangle(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#17 0x0000000007aa1e04 (anonymous namespace)::ItaniumMangleContextImpl::mangleCXXName(clang::GlobalDecl, llvm::raw_ostream&) ItaniumMangle.cpp:0:0
#18 0x0000000007ac663b clang::MangleContext::mangleName(clang::GlobalDecl, llvm::raw_ostream&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7ac663b)
#19 0x00000000041f1c24 getMangledNameImpl(clang::CodeGen::CodeGenModule&, clang::GlobalDecl, clang::NamedDecl const*, bool) CodeGenModule.cpp:0:0
#20 0x00000000042045dd clang::CodeGen::CodeGenModule::getMangledName(clang::GlobalDecl) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42045dd)
#21 0x000000000423e701 clang::CodeGen::CodeGenModule::GetAddrOfFunction(clang::GlobalDecl, llvm::Type*, bool, bool, clang::CodeGen::ForDefinition_t) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x423e701)
#22 0x000000000403b703 clang::CodeGen::CodeGenFunction::EmitCXXMemberOrOperatorMemberCallExpr(clang::CallExpr const*, clang::CXXMethodDecl const*, clang::CodeGen::ReturnValueSlot, bool, clang::NestedNameSpecifier*, bool, clang::Expr const*, llvm::CallBase**) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x403b703)
#23 0x000000000403bd13 clang::CodeGen::CodeGenFunction::EmitCXXOperatorMemberCallExpr(clang::CXXOperatorCallExpr const*, clang::CXXMethodDecl const*, clang::CodeGen::ReturnValueSlot, llvm::CallBase**) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x403bd13)
#24 0x0000000004019246 clang::CodeGen::CodeGenFunction::EmitCallExpr(clang::CallExpr const*, clang::CodeGen::ReturnValueSlot, llvm::CallBase**) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4019246)
#25 0x0000000004087814 (anonymous namespace)::ScalarExprEmitter::VisitCallExpr(clang::CallExpr const*) CGExprScalar.cpp:0:0
#26 0x000000000407b6d0 clang::StmtVisitorBase<std::add_pointer, (anonymous namespace)::ScalarExprEmitter, llvm::Value*>::Visit(clang::Stmt*) CGExprScalar.cpp:0:0
#27 0x0000000004088aca (anonymous namespace)::ScalarExprEmitter::VisitExprWithCleanups(clang::ExprWithCleanups*) CGExprScalar.cpp:0:0
#28 0x000000000407c2b7 clang::StmtVisitorBase<std::add_pointer, (anonymous namespace)::ScalarExprEmitter, llvm::Value*>::Visit(clang::Stmt*) CGExprScalar.cpp:0:0
#29 0x000000000408148c clang::CodeGen::CodeGenFunction::EmitScalarExpr(clang::Expr const*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x408148c)
#30 0x0000000003ff2ffe clang::CodeGen::CodeGenFunction::EmitAnyExpr(clang::Expr const*, clang::CodeGen::AggValueSlot, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3ff2ffe)
#31 0x00000000040175fd clang::CodeGen::CodeGenFunction::EmitIgnoredExpr(clang::Expr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x40175fd)
#32 0x0000000004155e2e clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef<clang::Attr const*>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4155e2e)
#33 0x000000000415dbdc clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&, bool, clang::CodeGen::AggValueSlot) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x415dbdc)
#34 0x00000000041c2f7c clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41c2f7c)
#35 0x00000000041d5ffe clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41d5ffe)
#36 0x000000000423ebda clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x423ebda)
#37 0x0000000004239e65 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4239e65)
#38 0x000000000423a24b clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x423a24b)
#39 0x0000000004244573 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#40 0x0000000004646386 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#41 0x0000000004636ea4 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4636ea4)
#42 0x000000000667de74 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x667de74)
#43 0x0000000004643fd8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4643fd8)
#44 0x000000000490a755 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x490a755)
#45 0x000000000488d0ce clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x488d0ce)
#46 0x00000000049f7dae clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49f7dae)
#47 0x0000000000cf4ccf cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xcf4ccf)
#48 0x0000000000cec79a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#49 0x000000000468a949 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#50 0x0000000003c00534 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3c00534)
#51 0x000000000468af3f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#52 0x000000000464dd0d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x464dd0d)
#53 0x000000000464ed8e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x464ed8e)
#54 0x0000000004656895 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4656895)
#55 0x0000000000cf1b03 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xcf1b03)
#56 0x0000000000bbcc44 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xbbcc44)
#57 0x0000757fbf429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#58 0x0000757fbf429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#59 0x0000000000cec245 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xcec245)
```

</details>

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (MagentaTreehouse)

<details>
The following C++20 code causes a crash since Clang 18:
```c++
template &lt;class T&gt;
constexpr auto f() {
    return [] () requires requires (T x) { x; } {};
}

int main() {
    f&lt;int&gt;()();
}
```

See https://compiler-explorer.com/z/z475c49eW.

Assertion:
```console
clang++: /root/llvm-project/clang/lib/AST/ItaniumMangle.cpp:5919:
void {anonymous}::CXXNameMangler::mangleFunctionParam(const clang::ParmVarDecl*):
Assertion `parmDepth &lt; FunctionTypeDepth.getDepth()' failed.
```

&lt;details&gt;

&lt;summary&gt;Stack dump&lt;/summary&gt;

```console
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	&lt;source&gt;:6:5: LLVM IR generation of declaration 'main'
3.	&lt;source&gt;:6:5: Generating code for declaration 'main'
4.	&lt;source&gt;:3:12: Mangling declaration 'f()::(anonymous class)::operator()'
 #<!-- -->0 0x0000000003cb5ea8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3cb5ea8)
 #<!-- -->1 0x0000000003cb3bb4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3cb3bb4)
 #<!-- -->2 0x0000000003c00088 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x0000757fbf442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x0000757fbf4969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x0000757fbf442476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x0000757fbf4287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x0000757fbf42871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x0000757fbf439e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000007a9ca8d (anonymous namespace)::CXXNameMangler::mangleFunctionParam(clang::ParmVarDecl const*) ItaniumMangle.cpp:0:0
#<!-- -->10 0x0000000007a9fd15 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool)::'lambda0'(clang::NamedDecl const*)::operator()(clang::NamedDecl const*) const (.isra.0) ItaniumMangle.cpp:0:0
#<!-- -->11 0x0000000007a9869f (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#<!-- -->12 0x0000000007aa8eba (anonymous namespace)::CXXNameMangler::mangleRequirement(clang::SourceLocation, clang::concepts::Requirement const*) ItaniumMangle.cpp:0:0
#<!-- -->13 0x0000000007a99405 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#<!-- -->14 0x0000000007a9cfdb (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) ItaniumMangle.cpp:0:0
#<!-- -->15 0x0000000007a9e4e0 (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#<!-- -->16 0x0000000007a9f5c7 (anonymous namespace)::CXXNameMangler::mangle(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#<!-- -->17 0x0000000007aa1e04 (anonymous namespace)::ItaniumMangleContextImpl::mangleCXXName(clang::GlobalDecl, llvm::raw_ostream&amp;) ItaniumMangle.cpp:0:0
#<!-- -->18 0x0000000007ac663b clang::MangleContext::mangleName(clang::GlobalDecl, llvm::raw_ostream&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7ac663b)
#<!-- -->19 0x00000000041f1c24 getMangledNameImpl(clang::CodeGen::CodeGenModule&amp;, clang::GlobalDecl, clang::NamedDecl const*, bool) CodeGenModule.cpp:0:0
#<!-- -->20 0x00000000042045dd clang::CodeGen::CodeGenModule::getMangledName(clang::GlobalDecl) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42045dd)
#<!-- -->21 0x000000000423e701 clang::CodeGen::CodeGenModule::GetAddrOfFunction(clang::GlobalDecl, llvm::Type*, bool, bool, clang::CodeGen::ForDefinition_t) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x423e701)
#<!-- -->22 0x000000000403b703 clang::CodeGen::CodeGenFunction::EmitCXXMemberOrOperatorMemberCallExpr(clang::CallExpr const*, clang::CXXMethodDecl const*, clang::CodeGen::ReturnValueSlot, bool, clang::NestedNameSpecifier*, bool, clang::Expr const*, llvm::CallBase**) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x403b703)
#<!-- -->23 0x000000000403bd13 clang::CodeGen::CodeGenFunction::EmitCXXOperatorMemberCallExpr(clang::CXXOperatorCallExpr const*, clang::CXXMethodDecl const*, clang::CodeGen::ReturnValueSlot, llvm::CallBase**) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x403bd13)
#<!-- -->24 0x0000000004019246 clang::CodeGen::CodeGenFunction::EmitCallExpr(clang::CallExpr const*, clang::CodeGen::ReturnValueSlot, llvm::CallBase**) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4019246)
#<!-- -->25 0x0000000004087814 (anonymous namespace)::ScalarExprEmitter::VisitCallExpr(clang::CallExpr const*) CGExprScalar.cpp:0:0
#<!-- -->26 0x000000000407b6d0 clang::StmtVisitorBase&lt;std::add_pointer, (anonymous namespace)::ScalarExprEmitter, llvm::Value*&gt;::Visit(clang::Stmt*) CGExprScalar.cpp:0:0
#<!-- -->27 0x0000000004088aca (anonymous namespace)::ScalarExprEmitter::VisitExprWithCleanups(clang::ExprWithCleanups*) CGExprScalar.cpp:0:0
#<!-- -->28 0x000000000407c2b7 clang::StmtVisitorBase&lt;std::add_pointer, (anonymous namespace)::ScalarExprEmitter, llvm::Value*&gt;::Visit(clang::Stmt*) CGExprScalar.cpp:0:0
#<!-- -->29 0x000000000408148c clang::CodeGen::CodeGenFunction::EmitScalarExpr(clang::Expr const*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x408148c)
#<!-- -->30 0x0000000003ff2ffe clang::CodeGen::CodeGenFunction::EmitAnyExpr(clang::Expr const*, clang::CodeGen::AggValueSlot, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3ff2ffe)
#<!-- -->31 0x00000000040175fd clang::CodeGen::CodeGenFunction::EmitIgnoredExpr(clang::Expr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x40175fd)
#<!-- -->32 0x0000000004155e2e clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef&lt;clang::Attr const*&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4155e2e)
#<!-- -->33 0x000000000415dbdc clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&amp;, bool, clang::CodeGen::AggValueSlot) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x415dbdc)
#<!-- -->34 0x00000000041c2f7c clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41c2f7c)
#<!-- -->35 0x00000000041d5ffe clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41d5ffe)
#<!-- -->36 0x000000000423ebda clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x423ebda)
#<!-- -->37 0x0000000004239e65 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4239e65)
#<!-- -->38 0x000000000423a24b clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x423a24b)
#<!-- -->39 0x0000000004244573 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#<!-- -->40 0x0000000004646386 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#<!-- -->41 0x0000000004636ea4 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4636ea4)
#<!-- -->42 0x000000000667de74 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x667de74)
#<!-- -->43 0x0000000004643fd8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4643fd8)
#<!-- -->44 0x000000000490a755 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x490a755)
#<!-- -->45 0x000000000488d0ce clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x488d0ce)
#<!-- -->46 0x00000000049f7dae clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49f7dae)
#<!-- -->47 0x0000000000cf4ccf cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xcf4ccf)
#<!-- -->48 0x0000000000cec79a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->49 0x000000000468a949 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->50 0x0000000003c00534 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3c00534)
#<!-- -->51 0x000000000468af3f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->52 0x000000000464dd0d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x464dd0d)
#<!-- -->53 0x000000000464ed8e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x464ed8e)
#<!-- -->54 0x0000000004656895 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4656895)
#<!-- -->55 0x0000000000cf1b03 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xcf1b03)
#<!-- -->56 0x0000000000bbcc44 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xbbcc44)
#<!-- -->57 0x0000757fbf429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->58 0x0000757fbf429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->59 0x0000000000cec245 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xcec245)
```

&lt;/details&gt;
</details>


---

### Comment 2 - frederick-vs-ja

Similar to #70064. (But not an exact duplicate, IIUC.)

---

### Comment 3 - zyn0217

CC @mizvekov for his lambda patch

---

