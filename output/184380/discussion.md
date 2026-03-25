# [HLSL][Matrix] Undersized vector swizzle applied to a constant matrix single subscript hits an assertion error

**Author:** Icohedron
**URL:** https://github.com/llvm/llvm-project/issues/184380
**Status:** Closed
**Labels:** clang:codegen, crash, HLSL
**Closed Date:** 2026-03-10T16:27:31Z

## Body

Applying vector swizzling to the result of a matrix single subscript with a constant index can crash Clang due to hitting an assertion error.
This happens when the vector swizzle results in a vector smaller than the vector returned by the matrix single subscript.

https://hlsl.godbolt.org/z/eo1dfoEje

```hlsl
// args: -T lib_6_8
export float2 fn(float4x4 M, int I) {
    return M[0].xy;
}
```

Stack trace:
```
clang: /root/llvm-project/llvm/include/llvm/ADT/SmallVector.h:297: T& llvm::SmallVectorTemplateCommon<T, <template-parameter-1-2> >::operator[](llvm::SmallVectorTemplateCommon<T, <template-parameter-1-2> >::size_type) [with T = unsigned int; <template-parameter-1-2> = void; llvm::SmallVectorTemplateCommon<T, <template-parameter-1-2> >::reference = unsigned int&; llvm::SmallVectorTemplateCommon<T, <template-parameter-1-2> >::size_type = long unsigned int]: Assertion `idx < size()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang --driver-mode=dxc -Zi -Qembed_debug -Fc /app/output.s -fcolor-diagnostics -fno-crash-diagnostics -T lib_6_8 <source>
1.	<eof> parser at end of file
2.	<source>:1:1: LLVM IR generation of declaration
3.	<source>:1:15: Generating code for declaration 'fn'
 #0 0x000000000440c2b8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x440c2b8)
 #1 0x0000000004409704 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4409704)
 #2 0x0000000004349508 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007d0aca242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007d0aca2969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007d0aca242476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007d0aca2287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007d0aca22871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007d0aca239e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x00000000048d6314 clang::CodeGen::CodeGenFunction::EmitExtVectorElementExpr(clang::ExtVectorElementExpr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x48d6314)
#10 0x00000000048eeda6 clang::CodeGen::CodeGenFunction::EmitLValueHelper(clang::Expr const*, clang::CodeGen::KnownNonNull_t) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x48eeda6)
#11 0x00000000048ef687 void llvm::function_ref<void ()>::callback_fn<clang::CodeGen::CodeGenFunction::EmitLValue(clang::Expr const*, clang::CodeGen::KnownNonNull_t)::'lambda'()>(long) CGExpr.cpp:0:0
#12 0x0000000008713061 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x8713061)
#13 0x00000000048b4adc clang::CodeGen::CodeGenFunction::EmitLValue(clang::Expr const*, clang::CodeGen::KnownNonNull_t) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x48b4adc)
#14 0x00000000048db25e clang::CodeGen::CodeGenFunction::EmitCheckedLValue(clang::Expr const*, clang::CodeGen::CodeGenFunction::TypeCheckKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x48db25e)
#15 0x0000000004954220 (anonymous namespace)::ScalarExprEmitter::EmitLoadOfLValue(clang::Expr const*) CGExprScalar.cpp:0:0
#16 0x000000000494e1c5 clang::StmtVisitorBase<std::add_pointer, (anonymous namespace)::ScalarExprEmitter, llvm::Value*>::Visit(clang::Stmt*) CGExprScalar.cpp:0:0
#17 0x000000000495043b (anonymous namespace)::ScalarExprEmitter::Visit(clang::Expr*) CGExprScalar.cpp:0:0
#18 0x00000000049546eb (anonymous namespace)::ScalarExprEmitter::VisitCastExpr(clang::CastExpr*) CGExprScalar.cpp:0:0
#19 0x000000000494e226 clang::StmtVisitorBase<std::add_pointer, (anonymous namespace)::ScalarExprEmitter, llvm::Value*>::Visit(clang::Stmt*) CGExprScalar.cpp:0:0
#20 0x0000000004953c1c clang::CodeGen::CodeGenFunction::EmitScalarExpr(clang::Expr const*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4953c1c)
#21 0x0000000004a734fc clang::CodeGen::CodeGenFunction::EmitReturnStmt(clang::ReturnStmt const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4a734fc)
#22 0x0000000004a7c79d clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef<clang::Attr const*>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4a7c79d)
#23 0x0000000004a84e9b clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&, bool, clang::CodeGen::AggValueSlot) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4a84e9b)
#24 0x0000000004aedf96 clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4aedf96)
#25 0x0000000004affdd4 clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4affdd4)
#26 0x0000000004b6eb00 clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4b6eb00)
#27 0x0000000004b697d4 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4b697d4)
#28 0x0000000004b69fb3 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4b69fb3)
#29 0x0000000004b756a3 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#30 0x0000000004b7665f clang::CodeGen::CodeGenModule::EmitDeclContext(clang::DeclContext const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4b7665f)
#31 0x0000000004b75891 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#32 0x0000000004ee7a59 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#33 0x0000000004ed8e64 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4ed8e64)
#34 0x0000000006bd0fb4 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6bd0fb4)
#35 0x0000000004ee57f8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4ee57f8)
#36 0x000000000520f6ed clang::HLSLFrontendAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x520f6ed)
#37 0x00000000051ebf75 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x51ebf75)
#38 0x000000000516a17e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x516a17e)
#39 0x00000000052e788d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x52e788d)
#40 0x0000000000df08a4 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xdf08a4)
#41 0x0000000000de721a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#42 0x0000000000de739d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#43 0x0000000004f59f59 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#44 0x0000000004349944 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4349944)
#45 0x0000000004f5a56f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#46 0x0000000004f1a252 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4f1a252)
#47 0x0000000004f1b1fe clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4f1b1fe)
#48 0x0000000004f22f45 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4f22f45)
#49 0x0000000000decc2a clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xdecc2a)
#50 0x0000000000ca4dba main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xca4dba)
#51 0x00007d0aca229d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#52 0x00007d0aca229e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#53 0x0000000000de6cb5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xde6cb5)
clang: error: clang frontend command failed due to signal (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: Deric C. (Icohedron)

<details>
Applying vector swizzling to the result of a matrix single subscript with a constant index can crash Clang due to hitting an assertion error.
This happens when the vector swizzle results in a vector smaller than the vector returned by the matrix single subscript.

https://hlsl.godbolt.org/z/eo1dfoEje

```hlsl
// args: -T lib_6_8
export float2 fn(float4x4 M, int I) {
    return M[0].xy;
}
```

Stack trace:
```
clang: /root/llvm-project/llvm/include/llvm/ADT/SmallVector.h:297: T&amp; llvm::SmallVectorTemplateCommon&lt;T, &lt;template-parameter-1-2&gt; &gt;::operator[](llvm::SmallVectorTemplateCommon&lt;T, &lt;template-parameter-1-2&gt; &gt;::size_type) [with T = unsigned int; &lt;template-parameter-1-2&gt; = void; llvm::SmallVectorTemplateCommon&lt;T, &lt;template-parameter-1-2&gt; &gt;::reference = unsigned int&amp;; llvm::SmallVectorTemplateCommon&lt;T, &lt;template-parameter-1-2&gt; &gt;::size_type = long unsigned int]: Assertion `idx &lt; size()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang --driver-mode=dxc -Zi -Qembed_debug -Fc /app/output.s -fcolor-diagnostics -fno-crash-diagnostics -T lib_6_8 &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	&lt;source&gt;:1:1: LLVM IR generation of declaration
3.	&lt;source&gt;:1:15: Generating code for declaration 'fn'
 #<!-- -->0 0x000000000440c2b8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x440c2b8)
 #<!-- -->1 0x0000000004409704 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4409704)
 #<!-- -->2 0x0000000004349508 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007d0aca242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007d0aca2969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007d0aca242476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007d0aca2287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x00007d0aca22871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x00007d0aca239e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x00000000048d6314 clang::CodeGen::CodeGenFunction::EmitExtVectorElementExpr(clang::ExtVectorElementExpr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x48d6314)
#<!-- -->10 0x00000000048eeda6 clang::CodeGen::CodeGenFunction::EmitLValueHelper(clang::Expr const*, clang::CodeGen::KnownNonNull_t) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x48eeda6)
#<!-- -->11 0x00000000048ef687 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::CodeGen::CodeGenFunction::EmitLValue(clang::Expr const*, clang::CodeGen::KnownNonNull_t)::'lambda'()&gt;(long) CGExpr.cpp:0:0
#<!-- -->12 0x0000000008713061 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x8713061)
#<!-- -->13 0x00000000048b4adc clang::CodeGen::CodeGenFunction::EmitLValue(clang::Expr const*, clang::CodeGen::KnownNonNull_t) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x48b4adc)
#<!-- -->14 0x00000000048db25e clang::CodeGen::CodeGenFunction::EmitCheckedLValue(clang::Expr const*, clang::CodeGen::CodeGenFunction::TypeCheckKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x48db25e)
#<!-- -->15 0x0000000004954220 (anonymous namespace)::ScalarExprEmitter::EmitLoadOfLValue(clang::Expr const*) CGExprScalar.cpp:0:0
#<!-- -->16 0x000000000494e1c5 clang::StmtVisitorBase&lt;std::add_pointer, (anonymous namespace)::ScalarExprEmitter, llvm::Value*&gt;::Visit(clang::Stmt*) CGExprScalar.cpp:0:0
#<!-- -->17 0x000000000495043b (anonymous namespace)::ScalarExprEmitter::Visit(clang::Expr*) CGExprScalar.cpp:0:0
#<!-- -->18 0x00000000049546eb (anonymous namespace)::ScalarExprEmitter::VisitCastExpr(clang::CastExpr*) CGExprScalar.cpp:0:0
#<!-- -->19 0x000000000494e226 clang::StmtVisitorBase&lt;std::add_pointer, (anonymous namespace)::ScalarExprEmitter, llvm::Value*&gt;::Visit(clang::Stmt*) CGExprScalar.cpp:0:0
#<!-- -->20 0x0000000004953c1c clang::CodeGen::CodeGenFunction::EmitScalarExpr(clang::Expr const*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4953c1c)
#<!-- -->21 0x0000000004a734fc clang::CodeGen::CodeGenFunction::EmitReturnStmt(clang::ReturnStmt const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4a734fc)
#<!-- -->22 0x0000000004a7c79d clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef&lt;clang::Attr const*&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4a7c79d)
#<!-- -->23 0x0000000004a84e9b clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&amp;, bool, clang::CodeGen::AggValueSlot) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4a84e9b)
#<!-- -->24 0x0000000004aedf96 clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4aedf96)
#<!-- -->25 0x0000000004affdd4 clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4affdd4)
#<!-- -->26 0x0000000004b6eb00 clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4b6eb00)
#<!-- -->27 0x0000000004b697d4 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4b697d4)
#<!-- -->28 0x0000000004b69fb3 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4b69fb3)
#<!-- -->29 0x0000000004b756a3 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#<!-- -->30 0x0000000004b7665f clang::CodeGen::CodeGenModule::EmitDeclContext(clang::DeclContext const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4b7665f)
#<!-- -->31 0x0000000004b75891 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#<!-- -->32 0x0000000004ee7a59 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#<!-- -->33 0x0000000004ed8e64 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4ed8e64)
#<!-- -->34 0x0000000006bd0fb4 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6bd0fb4)
#<!-- -->35 0x0000000004ee57f8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4ee57f8)
#<!-- -->36 0x000000000520f6ed clang::HLSLFrontendAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x520f6ed)
#<!-- -->37 0x00000000051ebf75 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x51ebf75)
#<!-- -->38 0x000000000516a17e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x516a17e)
#<!-- -->39 0x00000000052e788d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x52e788d)
#<!-- -->40 0x0000000000df08a4 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xdf08a4)
#<!-- -->41 0x0000000000de721a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->42 0x0000000000de739d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->43 0x0000000004f59f59 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->44 0x0000000004349944 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4349944)
#<!-- -->45 0x0000000004f5a56f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->46 0x0000000004f1a252 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4f1a252)
#<!-- -->47 0x0000000004f1b1fe clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4f1b1fe)
#<!-- -->48 0x0000000004f22f45 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4f22f45)
#<!-- -->49 0x0000000000decc2a clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xdecc2a)
#<!-- -->50 0x0000000000ca4dba main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xca4dba)
#<!-- -->51 0x00007d0aca229d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->52 0x00007d0aca229e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->53 0x0000000000de6cb5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xde6cb5)
clang: error: clang frontend command failed due to signal (use -v to see invocation)
Compiler returned: 134
```
</details>


---

