# [HLSL][Matrix] "Matrix types not yet supported in HLSL" assertion when explicitly casting a matrix to a vector

**Author:** Icohedron
**URL:** https://github.com/llvm/llvm-project/issues/177487
**Status:** Closed
**Labels:** clang:codegen, crash, HLSL
**Closed Date:** 2026-02-17T21:43:14Z

## Body

When explicitly casting a matrix to a vector, an assertion is hit saying "Matrix types not yet supported in HLSL"

https://godbolt.org/z/681rYvbjb

```hlsl
export float4 fn(float2x2 m) {
    float4 v = (float4) m;
    return v;
}
```

```
clang: warning: argument unused during compilation: '-Qembed_debug' [-Wunused-command-line-argument]
clang: /root/llvm-project/llvm/tools/clang/lib/CodeGen/CGExpr.cpp:7119: void clang::CodeGen::CodeGenFunction::FlattenAccessAndTypeLValue(clang::CodeGen::LValue, llvm::SmallVectorImpl<clang::CodeGen::LValue>&): Assertion `!isa<MatrixType>(T) && "Matrix types not yet supported in HLSL"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang --driver-mode=dxc -Zi -Qembed_debug -Fc /app/output.s -fcolor-diagnostics -fno-crash-diagnostics -T lib_6_8 <source>
1.	<eof> parser at end of file
2.	<source>:1:1: LLVM IR generation of declaration
3.	<source>:1:15: Generating code for declaration 'fn'
 #0 0x0000000004338028 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4338028)
 #1 0x0000000004335484 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4335484)
 #2 0x0000000004278258 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007c6c82842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007c6c828969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007c6c82842476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007c6c828287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007c6c8282871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007c6c82839e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x00000000046af753 clang::CodeGen::CodeGenFunction::FlattenAccessAndTypeLValue(clang::CodeGen::LValue, llvm::SmallVectorImpl<clang::CodeGen::LValue>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x46af753)
#10 0x000000000472172d (anonymous namespace)::ScalarExprEmitter::VisitCastExpr(clang::CastExpr*) CGExprScalar.cpp:0:0
#11 0x0000000004717358 clang::StmtVisitorBase<std::add_pointer, (anonymous namespace)::ScalarExprEmitter, llvm::Value*>::Visit(clang::Stmt*) CGExprScalar.cpp:0:0
#12 0x000000000471c70c clang::CodeGen::CodeGenFunction::EmitScalarExpr(clang::Expr const*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x471c70c)
#13 0x0000000004bebb59 clang::CodeGen::CodeGenFunction::EmitScalarInit(clang::Expr const*, clang::ValueDecl const*, clang::CodeGen::LValue, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4bebb59)
#14 0x0000000004bed4c7 clang::CodeGen::CodeGenFunction::EmitExprAsInit(clang::Expr const*, clang::ValueDecl const*, clang::CodeGen::LValue, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4bed4c7)
#15 0x0000000004bfdc97 clang::CodeGen::CodeGenFunction::EmitAutoVarInit(clang::CodeGen::CodeGenFunction::AutoVarEmission const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4bfdc97)
#16 0x0000000004bfe336 clang::CodeGen::CodeGenFunction::EmitVarDecl(clang::VarDecl const&) (.part.0) CGDecl.cpp:0:0
#17 0x0000000004bfea07 clang::CodeGen::CodeGenFunction::EmitDecl(clang::Decl const&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4bfea07)
#18 0x000000000483654c clang::CodeGen::CodeGenFunction::EmitDeclStmt(clang::DeclStmt const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x483654c)
#19 0x000000000484c9c5 clang::CodeGen::CodeGenFunction::EmitSimpleStmt(clang::Stmt const*, llvm::ArrayRef<clang::Attr const*>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x484c9c5)
#20 0x0000000004843ec1 clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef<clang::Attr const*>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4843ec1)
#21 0x000000000484c53b clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&, bool, clang::CodeGen::AggValueSlot) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x484c53b)
#22 0x00000000048b4ec6 clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x48b4ec6)
#23 0x00000000048c69a4 clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x48c69a4)
#24 0x0000000004934670 clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4934670)
#25 0x000000000492f314 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x492f314)
#26 0x000000000492faf3 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x492faf3)
#27 0x000000000493b083 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#28 0x000000000493c05f clang::CodeGen::CodeGenModule::EmitDeclContext(clang::DeclContext const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x493c05f)
#29 0x000000000493b0f4 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#30 0x0000000004c9fcb9 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#31 0x0000000004c910a4 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4c910a4)
#32 0x0000000006a8c8c4 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6a8c8c4)
#33 0x0000000004c9da58 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4c9da58)
#34 0x0000000004fbba8d clang::HLSLFrontendAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4fbba8d)
#35 0x0000000004f984e5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4f984e5)
#36 0x0000000004f17ebe clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4f17ebe)
#37 0x00000000050936ad clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x50936ad)
#38 0x0000000000ddce2e cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xddce2e)
#39 0x0000000000dd37fa ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#40 0x0000000000dd397d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#41 0x0000000004d114b9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#42 0x00000000042786f4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x42786f4)
#43 0x0000000004d11acf clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#44 0x0000000004cd24a2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4cd24a2)
#45 0x0000000004cd344e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4cd344e)
#46 0x0000000004cda895 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4cda895)
#47 0x0000000000dd91a1 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xdd91a1)
#48 0x0000000000c95f24 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xc95f24)
#49 0x00007c6c82829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#50 0x00007c6c82829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#51 0x0000000000dd3295 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xdd3295)
clang: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Deric C. (Icohedron)

<details>
When explicitly casting a matrix to a vector, an assertion is hit saying "Matrix types not yet supported in HLSL"

https://godbolt.org/z/681rYvbjb

```hlsl
export float4 fn(float2x2 m) {
    float4 v = (float4) m;
    return v;
}
```

```
clang: warning: argument unused during compilation: '-Qembed_debug' [-Wunused-command-line-argument]
clang: /root/llvm-project/llvm/tools/clang/lib/CodeGen/CGExpr.cpp:7119: void clang::CodeGen::CodeGenFunction::FlattenAccessAndTypeLValue(clang::CodeGen::LValue, llvm::SmallVectorImpl&lt;clang::CodeGen::LValue&gt;&amp;): Assertion `!isa&lt;MatrixType&gt;(T) &amp;&amp; "Matrix types not yet supported in HLSL"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang --driver-mode=dxc -Zi -Qembed_debug -Fc /app/output.s -fcolor-diagnostics -fno-crash-diagnostics -T lib_6_8 &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	&lt;source&gt;:1:1: LLVM IR generation of declaration
3.	&lt;source&gt;:1:15: Generating code for declaration 'fn'
 #<!-- -->0 0x0000000004338028 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4338028)
 #<!-- -->1 0x0000000004335484 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4335484)
 #<!-- -->2 0x0000000004278258 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007c6c82842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007c6c828969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007c6c82842476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007c6c828287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x00007c6c8282871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x00007c6c82839e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x00000000046af753 clang::CodeGen::CodeGenFunction::FlattenAccessAndTypeLValue(clang::CodeGen::LValue, llvm::SmallVectorImpl&lt;clang::CodeGen::LValue&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x46af753)
#<!-- -->10 0x000000000472172d (anonymous namespace)::ScalarExprEmitter::VisitCastExpr(clang::CastExpr*) CGExprScalar.cpp:0:0
#<!-- -->11 0x0000000004717358 clang::StmtVisitorBase&lt;std::add_pointer, (anonymous namespace)::ScalarExprEmitter, llvm::Value*&gt;::Visit(clang::Stmt*) CGExprScalar.cpp:0:0
#<!-- -->12 0x000000000471c70c clang::CodeGen::CodeGenFunction::EmitScalarExpr(clang::Expr const*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x471c70c)
#<!-- -->13 0x0000000004bebb59 clang::CodeGen::CodeGenFunction::EmitScalarInit(clang::Expr const*, clang::ValueDecl const*, clang::CodeGen::LValue, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4bebb59)
#<!-- -->14 0x0000000004bed4c7 clang::CodeGen::CodeGenFunction::EmitExprAsInit(clang::Expr const*, clang::ValueDecl const*, clang::CodeGen::LValue, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4bed4c7)
#<!-- -->15 0x0000000004bfdc97 clang::CodeGen::CodeGenFunction::EmitAutoVarInit(clang::CodeGen::CodeGenFunction::AutoVarEmission const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4bfdc97)
#<!-- -->16 0x0000000004bfe336 clang::CodeGen::CodeGenFunction::EmitVarDecl(clang::VarDecl const&amp;) (.part.0) CGDecl.cpp:0:0
#<!-- -->17 0x0000000004bfea07 clang::CodeGen::CodeGenFunction::EmitDecl(clang::Decl const&amp;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4bfea07)
#<!-- -->18 0x000000000483654c clang::CodeGen::CodeGenFunction::EmitDeclStmt(clang::DeclStmt const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x483654c)
#<!-- -->19 0x000000000484c9c5 clang::CodeGen::CodeGenFunction::EmitSimpleStmt(clang::Stmt const*, llvm::ArrayRef&lt;clang::Attr const*&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x484c9c5)
#<!-- -->20 0x0000000004843ec1 clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef&lt;clang::Attr const*&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4843ec1)
#<!-- -->21 0x000000000484c53b clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&amp;, bool, clang::CodeGen::AggValueSlot) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x484c53b)
#<!-- -->22 0x00000000048b4ec6 clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x48b4ec6)
#<!-- -->23 0x00000000048c69a4 clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x48c69a4)
#<!-- -->24 0x0000000004934670 clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4934670)
#<!-- -->25 0x000000000492f314 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x492f314)
#<!-- -->26 0x000000000492faf3 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x492faf3)
#<!-- -->27 0x000000000493b083 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#<!-- -->28 0x000000000493c05f clang::CodeGen::CodeGenModule::EmitDeclContext(clang::DeclContext const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x493c05f)
#<!-- -->29 0x000000000493b0f4 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#<!-- -->30 0x0000000004c9fcb9 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#<!-- -->31 0x0000000004c910a4 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4c910a4)
#<!-- -->32 0x0000000006a8c8c4 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6a8c8c4)
#<!-- -->33 0x0000000004c9da58 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4c9da58)
#<!-- -->34 0x0000000004fbba8d clang::HLSLFrontendAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4fbba8d)
#<!-- -->35 0x0000000004f984e5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4f984e5)
#<!-- -->36 0x0000000004f17ebe clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4f17ebe)
#<!-- -->37 0x00000000050936ad clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x50936ad)
#<!-- -->38 0x0000000000ddce2e cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xddce2e)
#<!-- -->39 0x0000000000dd37fa ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->40 0x0000000000dd397d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->41 0x0000000004d114b9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->42 0x00000000042786f4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x42786f4)
#<!-- -->43 0x0000000004d11acf clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->44 0x0000000004cd24a2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4cd24a2)
#<!-- -->45 0x0000000004cd344e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4cd344e)
#<!-- -->46 0x0000000004cda895 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4cda895)
#<!-- -->47 0x0000000000dd91a1 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xdd91a1)
#<!-- -->48 0x0000000000c95f24 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xc95f24)
#<!-- -->49 0x00007c6c82829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->50 0x00007c6c82829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->51 0x0000000000dd3295 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xdd3295)
clang: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: Deric C. (Icohedron)

<details>
When explicitly casting a matrix to a vector, an assertion is hit saying "Matrix types not yet supported in HLSL"

https://godbolt.org/z/681rYvbjb

```hlsl
export float4 fn(float2x2 m) {
    float4 v = (float4) m;
    return v;
}
```

```
clang: warning: argument unused during compilation: '-Qembed_debug' [-Wunused-command-line-argument]
clang: /root/llvm-project/llvm/tools/clang/lib/CodeGen/CGExpr.cpp:7119: void clang::CodeGen::CodeGenFunction::FlattenAccessAndTypeLValue(clang::CodeGen::LValue, llvm::SmallVectorImpl&lt;clang::CodeGen::LValue&gt;&amp;): Assertion `!isa&lt;MatrixType&gt;(T) &amp;&amp; "Matrix types not yet supported in HLSL"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang --driver-mode=dxc -Zi -Qembed_debug -Fc /app/output.s -fcolor-diagnostics -fno-crash-diagnostics -T lib_6_8 &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	&lt;source&gt;:1:1: LLVM IR generation of declaration
3.	&lt;source&gt;:1:15: Generating code for declaration 'fn'
 #<!-- -->0 0x0000000004338028 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4338028)
 #<!-- -->1 0x0000000004335484 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4335484)
 #<!-- -->2 0x0000000004278258 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007c6c82842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007c6c828969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007c6c82842476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007c6c828287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x00007c6c8282871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x00007c6c82839e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x00000000046af753 clang::CodeGen::CodeGenFunction::FlattenAccessAndTypeLValue(clang::CodeGen::LValue, llvm::SmallVectorImpl&lt;clang::CodeGen::LValue&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x46af753)
#<!-- -->10 0x000000000472172d (anonymous namespace)::ScalarExprEmitter::VisitCastExpr(clang::CastExpr*) CGExprScalar.cpp:0:0
#<!-- -->11 0x0000000004717358 clang::StmtVisitorBase&lt;std::add_pointer, (anonymous namespace)::ScalarExprEmitter, llvm::Value*&gt;::Visit(clang::Stmt*) CGExprScalar.cpp:0:0
#<!-- -->12 0x000000000471c70c clang::CodeGen::CodeGenFunction::EmitScalarExpr(clang::Expr const*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x471c70c)
#<!-- -->13 0x0000000004bebb59 clang::CodeGen::CodeGenFunction::EmitScalarInit(clang::Expr const*, clang::ValueDecl const*, clang::CodeGen::LValue, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4bebb59)
#<!-- -->14 0x0000000004bed4c7 clang::CodeGen::CodeGenFunction::EmitExprAsInit(clang::Expr const*, clang::ValueDecl const*, clang::CodeGen::LValue, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4bed4c7)
#<!-- -->15 0x0000000004bfdc97 clang::CodeGen::CodeGenFunction::EmitAutoVarInit(clang::CodeGen::CodeGenFunction::AutoVarEmission const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4bfdc97)
#<!-- -->16 0x0000000004bfe336 clang::CodeGen::CodeGenFunction::EmitVarDecl(clang::VarDecl const&amp;) (.part.0) CGDecl.cpp:0:0
#<!-- -->17 0x0000000004bfea07 clang::CodeGen::CodeGenFunction::EmitDecl(clang::Decl const&amp;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4bfea07)
#<!-- -->18 0x000000000483654c clang::CodeGen::CodeGenFunction::EmitDeclStmt(clang::DeclStmt const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x483654c)
#<!-- -->19 0x000000000484c9c5 clang::CodeGen::CodeGenFunction::EmitSimpleStmt(clang::Stmt const*, llvm::ArrayRef&lt;clang::Attr const*&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x484c9c5)
#<!-- -->20 0x0000000004843ec1 clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef&lt;clang::Attr const*&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4843ec1)
#<!-- -->21 0x000000000484c53b clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&amp;, bool, clang::CodeGen::AggValueSlot) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x484c53b)
#<!-- -->22 0x00000000048b4ec6 clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x48b4ec6)
#<!-- -->23 0x00000000048c69a4 clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x48c69a4)
#<!-- -->24 0x0000000004934670 clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4934670)
#<!-- -->25 0x000000000492f314 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x492f314)
#<!-- -->26 0x000000000492faf3 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x492faf3)
#<!-- -->27 0x000000000493b083 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#<!-- -->28 0x000000000493c05f clang::CodeGen::CodeGenModule::EmitDeclContext(clang::DeclContext const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x493c05f)
#<!-- -->29 0x000000000493b0f4 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#<!-- -->30 0x0000000004c9fcb9 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#<!-- -->31 0x0000000004c910a4 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4c910a4)
#<!-- -->32 0x0000000006a8c8c4 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6a8c8c4)
#<!-- -->33 0x0000000004c9da58 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4c9da58)
#<!-- -->34 0x0000000004fbba8d clang::HLSLFrontendAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4fbba8d)
#<!-- -->35 0x0000000004f984e5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4f984e5)
#<!-- -->36 0x0000000004f17ebe clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4f17ebe)
#<!-- -->37 0x00000000050936ad clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x50936ad)
#<!-- -->38 0x0000000000ddce2e cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xddce2e)
#<!-- -->39 0x0000000000dd37fa ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->40 0x0000000000dd397d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->41 0x0000000004d114b9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->42 0x00000000042786f4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x42786f4)
#<!-- -->43 0x0000000004d11acf clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->44 0x0000000004cd24a2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4cd24a2)
#<!-- -->45 0x0000000004cd344e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4cd344e)
#<!-- -->46 0x0000000004cda895 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4cda895)
#<!-- -->47 0x0000000000dd91a1 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xdd91a1)
#<!-- -->48 0x0000000000c95f24 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xc95f24)
#<!-- -->49 0x00007c6c82829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->50 0x00007c6c82829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->51 0x0000000000dd3295 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xdd3295)
clang: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

