# Add bool getter\setter support for Single subscript matrix

**Author:** farzonl
**URL:** https://github.com/llvm/llvm-project/issues/172711
**Status:** Closed
**Labels:** clang:codegen, crash, HLSL
**Closed Date:** 2026-01-14T16:44:57Z

## Body

This currently crashes because  we are extracting an i32 bool and trying to insert into an i1.  An earlier bool change is representing matrix bools as i32 so we need to be consistent across accessors

```hlsl
// clang -cc1 -internal-isystem <build_dir>lib/clang/22/include -nostdsysteminc -triple dxil-pc-shadermodel6.7-library -disable-llvm-passes -emit-llvm -finclude-default-header -o - template.hlsl

bool3 getBoolVecFromTemplateMat(matrix<bool, 2, 3> M, int index) {
    return M[index];
}

bool3 getBoolVecFromTemplateMatRow0(matrix<bool, 2, 3> M) {
    return M[0];
}
```

```gdb
Assertion failed: (isValidOperands(Vec, Elt, Index) && "Invalid insertelement instruction operands!"), function InsertElementInst, file Instructions.cpp, line 1701.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang -cc1 -internal-isystem /Users/farzonlotfi/Projects/llvm_debug_offload_build/lib/clang/22/include -nostdsysteminc -triple dxil-pc-shadermodel6.7-library -disable-llvm-passes -emit-llvm -finclude-default-header -o - template.hlsl
1.      template.hlsl:5:1: current parser token 'bool3'
2.      template.hlsl:1:7: LLVM IR generation of declaration 'getBoolVecFromTemplateMat'
3.      template.hlsl:1:7: Generating code for declaration 'getBoolVecFromTemplateMat'
 #0 0x0000000107c77100 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x1034fb100)
 #1 0x0000000107c77710 PrintStackTraceSignalHandler(void*) (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x1034fb710)
 #2 0x0000000107c74f7c llvm::sys::RunSignalHandlers() (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x1034f8f7c)
 #3 0x0000000107c791a0 SignalHandler(int, __siginfo*, void*) (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x1034fd1a0)
 #4 0x000000019c3ad6a4 (/usr/lib/system/libsystem_platform.dylib+0x1804ad6a4)
 #5 0x000000019c373848 (/usr/lib/system/libsystem_pthread.dylib+0x180473848)
 #6 0x000000019c27c9e4 (/usr/lib/system/libsystem_c.dylib+0x18037c9e4)
 #7 0x000000019c27bc18 (/usr/lib/system/libsystem_c.dylib+0x18037bc18)
 #8 0x0000000106862604 llvm::InsertElementInst::InsertElementInst(llvm::Value*, llvm::Value*, llvm::Value*, llvm::Twine const&, llvm::InsertPosition) (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x1020e6604)
 #9 0x00000001068627fc llvm::InsertElementInst::InsertElementInst(llvm::Value*, llvm::Value*, llvm::Value*, llvm::Twine const&, llvm::InsertPosition) (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x1020e67fc)
#10 0x0000000104a27d24 llvm::InsertElementInst::Create(llvm::Value*, llvm::Value*, llvm::Value*, llvm::Twine const&, llvm::InsertPosition) (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x1002abd24)
#11 0x0000000104afe930 llvm::IRBuilderBase::CreateInsertElement(llvm::Value*, llvm::Value*, llvm::Value*, llvm::Twine const&) (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x100382930)
#12 0x0000000108b0d160 (anonymous namespace)::ScalarExprEmitter::VisitMatrixSingleSubscriptExpr(clang::MatrixSingleSubscriptExpr*) (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x104391160)
#13 0x0000000108b05580 clang::StmtVisitorBase<std::__1::add_pointer, (anonymous namespace)::ScalarExprEmitter, llvm::Value*>::Visit(clang::Stmt*) (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x104389580)
#14 0x0000000108afbc10 (anonymous namespace)::ScalarExprEmitter::Visit(clang::Expr*) (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x10437fc10)
#15 0x0000000108b19be0 (anonymous namespace)::ScalarExprEmitter::VisitCastExpr(clang::CastExpr*) (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x10439dbe0)
#16 0x0000000108b0f16c clang::StmtVisitorBase<std::__1::add_pointer, (anonymous namespace)::ScalarExprEmitter, llvm::Value*>::VisitImplicitCastExpr(clang::ImplicitCastExpr*) (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x10439316c)
#17 0x0000000108b05800 clang::StmtVisitorBase<std::__1::add_pointer, (anonymous namespace)::ScalarExprEmitter, llvm::Value*>::Visit(clang::Stmt*) (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x104389800)
#18 0x0000000108afbc10 (anonymous namespace)::ScalarExprEmitter::Visit(clang::Expr*) (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x10437fc10)
#19 0x0000000108afb9f4 clang::CodeGen::CodeGenFunction::EmitScalarExpr(clang::Expr const*, bool) (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x10437f9f4)
#20 0x0000000108da3578 clang::CodeGen::CodeGenFunction::EmitReturnStmt(clang::ReturnStmt const&) (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x104627578)
#21 0x0000000108d9fd94 clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef<clang::Attr const*>) (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x104623d94)
#22 0x0000000108dabd4c clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&, bool, clang::CodeGen::AggValueSlot) (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x10462fd4c)
#23 0x0000000108e86398 clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x10470a398)
#24 0x0000000108e87410 clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&) (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x10470b410)
#25 0x0000000108ebc8c4 clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x1047408c4)
#26 0x0000000108eb1ee8 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x104735ee8)
#27 0x0000000108eb75a0 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x10473b5a0)
#28 0x0000000108eb04f4 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x1047344f4)
#29 0x00000001090e00f0 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x1049640f0)
#30 0x0000000108e6582c clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x1046e982c)
#31 0x000000010b57627c clang::ParseAST(clang::Sema&, bool, bool) (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x106dfa27c)
#32 0x0000000109848998 clang::ASTFrontendAction::ExecuteAction() (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x1050cc998)
#33 0x0000000108e6c05c clang::CodeGenAction::ExecuteAction() (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x1046f005c)
#34 0x0000000109848c6c clang::WrapperFrontendAction::ExecuteAction() (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x1050ccc6c)
#35 0x000000010986c494 clang::HLSLFrontendAction::ExecuteAction() (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x1050f0494)
#36 0x00000001098481a8 clang::FrontendAction::Execute() (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x1050cc1a8)
#37 0x000000010975b3f8 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x104fdf3f8)
#38 0x000000010999cbf4 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x105220bf4)
#39 0x000000010478f2d8 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x1000132d8)
#40 0x0000000104781778 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x100005778)
#41 0x0000000104780468 clang_main(int, char**, llvm::ToolContext const&) (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x100004468)
#42 0x00000001047bb9f4 main (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x10003f9f4)
#43 0x000000019bfd2b98
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: Farzon Lotfi (farzonl)

<details>
This currently crashes because  we are extracting an i32 bool and trying to insert into an i1.  An earlier bool change is representing matrix bools as i32 so we need to be consistent across accessors

```hlsl
// clang -cc1 -internal-isystem &lt;build_dir&gt;lib/clang/22/include -nostdsysteminc -triple dxil-pc-shadermodel6.7-library -disable-llvm-passes -emit-llvm -finclude-default-header -o - template.hlsl

bool3 getBoolVecFromTemplateMat(matrix&lt;bool, 2, 3&gt; M, int index) {
    return M[index];
}

bool3 getBoolVecFromTemplateMatRow0(matrix&lt;bool, 2, 3&gt; M) {
    return M[0];
}
```

```gdb
Assertion failed: (isValidOperands(Vec, Elt, Index) &amp;&amp; "Invalid insertelement instruction operands!"), function InsertElementInst, file Instructions.cpp, line 1701.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang -cc1 -internal-isystem /Users/farzonlotfi/Projects/llvm_debug_offload_build/lib/clang/22/include -nostdsysteminc -triple dxil-pc-shadermodel6.7-library -disable-llvm-passes -emit-llvm -finclude-default-header -o - template.hlsl
1.      template.hlsl:5:1: current parser token 'bool3'
2.      template.hlsl:1:7: LLVM IR generation of declaration 'getBoolVecFromTemplateMat'
3.      template.hlsl:1:7: Generating code for declaration 'getBoolVecFromTemplateMat'
 #<!-- -->0 0x0000000107c77100 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x1034fb100)
 #<!-- -->1 0x0000000107c77710 PrintStackTraceSignalHandler(void*) (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x1034fb710)
 #<!-- -->2 0x0000000107c74f7c llvm::sys::RunSignalHandlers() (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x1034f8f7c)
 #<!-- -->3 0x0000000107c791a0 SignalHandler(int, __siginfo*, void*) (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x1034fd1a0)
 #<!-- -->4 0x000000019c3ad6a4 (/usr/lib/system/libsystem_platform.dylib+0x1804ad6a4)
 #<!-- -->5 0x000000019c373848 (/usr/lib/system/libsystem_pthread.dylib+0x180473848)
 #<!-- -->6 0x000000019c27c9e4 (/usr/lib/system/libsystem_c.dylib+0x18037c9e4)
 #<!-- -->7 0x000000019c27bc18 (/usr/lib/system/libsystem_c.dylib+0x18037bc18)
 #<!-- -->8 0x0000000106862604 llvm::InsertElementInst::InsertElementInst(llvm::Value*, llvm::Value*, llvm::Value*, llvm::Twine const&amp;, llvm::InsertPosition) (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x1020e6604)
 #<!-- -->9 0x00000001068627fc llvm::InsertElementInst::InsertElementInst(llvm::Value*, llvm::Value*, llvm::Value*, llvm::Twine const&amp;, llvm::InsertPosition) (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x1020e67fc)
#<!-- -->10 0x0000000104a27d24 llvm::InsertElementInst::Create(llvm::Value*, llvm::Value*, llvm::Value*, llvm::Twine const&amp;, llvm::InsertPosition) (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x1002abd24)
#<!-- -->11 0x0000000104afe930 llvm::IRBuilderBase::CreateInsertElement(llvm::Value*, llvm::Value*, llvm::Value*, llvm::Twine const&amp;) (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x100382930)
#<!-- -->12 0x0000000108b0d160 (anonymous namespace)::ScalarExprEmitter::VisitMatrixSingleSubscriptExpr(clang::MatrixSingleSubscriptExpr*) (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x104391160)
#<!-- -->13 0x0000000108b05580 clang::StmtVisitorBase&lt;std::__1::add_pointer, (anonymous namespace)::ScalarExprEmitter, llvm::Value*&gt;::Visit(clang::Stmt*) (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x104389580)
#<!-- -->14 0x0000000108afbc10 (anonymous namespace)::ScalarExprEmitter::Visit(clang::Expr*) (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x10437fc10)
#<!-- -->15 0x0000000108b19be0 (anonymous namespace)::ScalarExprEmitter::VisitCastExpr(clang::CastExpr*) (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x10439dbe0)
#<!-- -->16 0x0000000108b0f16c clang::StmtVisitorBase&lt;std::__1::add_pointer, (anonymous namespace)::ScalarExprEmitter, llvm::Value*&gt;::VisitImplicitCastExpr(clang::ImplicitCastExpr*) (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x10439316c)
#<!-- -->17 0x0000000108b05800 clang::StmtVisitorBase&lt;std::__1::add_pointer, (anonymous namespace)::ScalarExprEmitter, llvm::Value*&gt;::Visit(clang::Stmt*) (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x104389800)
#<!-- -->18 0x0000000108afbc10 (anonymous namespace)::ScalarExprEmitter::Visit(clang::Expr*) (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x10437fc10)
#<!-- -->19 0x0000000108afb9f4 clang::CodeGen::CodeGenFunction::EmitScalarExpr(clang::Expr const*, bool) (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x10437f9f4)
#<!-- -->20 0x0000000108da3578 clang::CodeGen::CodeGenFunction::EmitReturnStmt(clang::ReturnStmt const&amp;) (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x104627578)
#<!-- -->21 0x0000000108d9fd94 clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef&lt;clang::Attr const*&gt;) (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x104623d94)
#<!-- -->22 0x0000000108dabd4c clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&amp;, bool, clang::CodeGen::AggValueSlot) (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x10462fd4c)
#<!-- -->23 0x0000000108e86398 clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x10470a398)
#<!-- -->24 0x0000000108e87410 clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&amp;) (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x10470b410)
#<!-- -->25 0x0000000108ebc8c4 clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x1047408c4)
#<!-- -->26 0x0000000108eb1ee8 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x104735ee8)
#<!-- -->27 0x0000000108eb75a0 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x10473b5a0)
#<!-- -->28 0x0000000108eb04f4 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x1047344f4)
#<!-- -->29 0x00000001090e00f0 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x1049640f0)
#<!-- -->30 0x0000000108e6582c clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x1046e982c)
#<!-- -->31 0x000000010b57627c clang::ParseAST(clang::Sema&amp;, bool, bool) (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x106dfa27c)
#<!-- -->32 0x0000000109848998 clang::ASTFrontendAction::ExecuteAction() (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x1050cc998)
#<!-- -->33 0x0000000108e6c05c clang::CodeGenAction::ExecuteAction() (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x1046f005c)
#<!-- -->34 0x0000000109848c6c clang::WrapperFrontendAction::ExecuteAction() (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x1050ccc6c)
#<!-- -->35 0x000000010986c494 clang::HLSLFrontendAction::ExecuteAction() (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x1050f0494)
#<!-- -->36 0x00000001098481a8 clang::FrontendAction::Execute() (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x1050cc1a8)
#<!-- -->37 0x000000010975b3f8 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x104fdf3f8)
#<!-- -->38 0x000000010999cbf4 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x105220bf4)
#<!-- -->39 0x000000010478f2d8 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x1000132d8)
#<!-- -->40 0x0000000104781778 ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x100005778)
#<!-- -->41 0x0000000104780468 clang_main(int, char**, llvm::ToolContext const&amp;) (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x100004468)
#<!-- -->42 0x00000001047bb9f4 main (/Users/farzonlotfi/Projects/llvm_debug_offload_build/bin/clang-21+0x10003f9f4)
#<!-- -->43 0x000000019bfd2b98
```
</details>


---

