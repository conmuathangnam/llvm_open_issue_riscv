# Crash in TypoCorrectionConsumer::getNextCorrection

**Author:** olemayu
**URL:** https://github.com/llvm/llvm-project/issues/142457
**Status:** Closed
**Labels:** clang:frontend, regression, crash
**Closed Date:** 2025-06-13T10:45:41Z

## Body

This code crashes clangd 21.0.0 (20250601)

```c
static void func(void) {
    alloc_image(device,
                allocator,
                &texture->image, &texture->alloc, &texture->view,
                name, width, height, depth,
                mip_levels, sampling, usage, view_type,
                VK_IMAGE_ASPECT_COLOR_BIT, format);
}
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clangd

Author: None (olemayu)

<details>
This code crashes clangd 21.0.0 (20250601)

```c
static void func(void) {
    alloc_image(device,
                allocator,
                &amp;texture-&gt;image, &amp;texture-&gt;alloc, &amp;texture-&gt;view,
                name, width, height, depth,
                mip_levels, sampling, usage, view_type,
                VK_IMAGE_ASPECT_COLOR_BIT, format);
}
```
</details>


---

### Comment 2 - carsonRadtke

smaller repro:
```c++
void func() {
    foo(abc, texture.xyz); // crash goes away if "texture" is renamed.
}
```

clangd version:
```plaintext
clangd version 21.0.0git (https://github.com/carsonRadtke/llvm-project.git f3867f900fcb834bdc77c724766553d18aed0598)
Features: mac+debug+asan+xpc
Platform: arm64-apple-darwin24.5.0
```

crash:
```plaintext
Assertion failed: (Entry != DelayedTypos.end() && "Failed to get the state for a TypoExpr!"), function getTypoExprState, file SemaLookup.cpp, line 5810.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.	Program arguments: bin/clangd --check=/tmp/debug.cpp
1.	/private/tmp/debug.cpp:3:1: current parser token '}'
2.	/private/tmp/debug.cpp:1:13: parsing function body 'func'
3.	/private/tmp/debug.cpp:1:13: in compound statement ('{}')
 #0 0x0000000105fc44e4 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/Users/carson/repos/llvm-project/build/bin/clangd+0x1012984e4)
 #1 0x0000000105fc5a54 PrintStackTraceSignalHandler(void*) (/Users/carson/repos/llvm-project/build/bin/clangd+0x101299a54)
 #2 0x0000000105fbc8e0 llvm::sys::RunSignalHandlers() (/Users/carson/repos/llvm-project/build/bin/clangd+0x1012908e0)
 #3 0x0000000105fca6d4 SignalHandler(int, __siginfo*, void*) (/Users/carson/repos/llvm-project/build/bin/clangd+0x10129e6d4)
 #4 0x000000019cf38624 (/usr/lib/system/libsystem_platform.dylib+0x1804ac624)
 #5 0x000000019cefe88c (/usr/lib/system/libsystem_pthread.dylib+0x18047288c)
 #6 0x000000019ce07c60 (/usr/lib/system/libsystem_c.dylib+0x18037bc60)
 #7 0x000000019ce06eec (/usr/lib/system/libsystem_c.dylib+0x18037aeec)
 #8 0x000000010c92f308 clang::Sema::getTypoExprState(clang::TypoExpr*) const (/Users/carson/repos/llvm-project/build/bin/clangd+0x107c03308)
 #9 0x000000010ab2fa8c (anonymous namespace)::TransformTypos::TransformTypoExpr(clang::TypoExpr*) (/Users/carson/repos/llvm-project/build/bin/clangd+0x105e03a8c)
#10 0x000000010ab29bd4 clang::TreeTransform<(anonymous namespace)::TransformTypos>::TransformExpr(clang::Expr*) (/Users/carson/repos/llvm-project/build/bin/clangd+0x105dfdbd4)
#11 0x000000010ab37998 clang::TreeTransform<(anonymous namespace)::TransformTypos>::TransformRecoveryExpr(clang::RecoveryExpr*) (/Users/carson/repos/llvm-project/build/bin/clangd+0x105e0b998)
#12 0x000000010ab2a0fc clang::TreeTransform<(anonymous namespace)::TransformTypos>::TransformExpr(clang::Expr*) (/Users/carson/repos/llvm-project/build/bin/clangd+0x105dfe0fc)
#13 0x000000010ab1d41c (anonymous namespace)::TransformTypos::TryTransform(clang::Expr*) (/Users/carson/repos/llvm-project/build/bin/clangd+0x105df141c)
#14 0x000000010ab1a49c (anonymous namespace)::TransformTypos::RecursiveTransformLoop(clang::Expr*, bool&) (/Users/carson/repos/llvm-project/build/bin/clangd+0x105dee49c)
#15 0x000000010aadfaec (anonymous namespace)::TransformTypos::Transform(clang::Expr*) (/Users/carson/repos/llvm-project/build/bin/clangd+0x105db3aec)
#16 0x000000010aa83128 clang::Sema::CorrectDelayedTyposInExpr(clang::Expr*, clang::VarDecl*, bool, llvm::function_ref<clang::ActionResult<clang::Expr*, true> (clang::Expr*)>) (/Users/carson/repos/llvm-project/build/bin/clangd+0x105d57128)
#17 0x000000010aae4988 clang::Sema::ActOnFinishFullExpr(clang::Expr*, clang::SourceLocation, bool, bool, bool) (/Users/carson/repos/llvm-project/build/bin/clangd+0x105db8988)
#18 0x000000010d835ad4 clang::Sema::ActOnExprStmt(clang::ActionResult<clang::Expr*, true>, bool) (/Users/carson/repos/llvm-project/build/bin/clangd+0x108b09ad4)
#19 0x000000011c0514a0 clang::Parser::handleExprStmt(clang::ActionResult<clang::Expr*, true>, clang::Parser::ParsedStmtContext) (/Users/carson/repos/llvm-project/build/bin/clangd+0x1173254a0)
#20 0x000000011c0415e0 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/Users/carson/repos/llvm-project/build/bin/clangd+0x1173155e0)
#21 0x000000011c03cb50 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/Users/carson/repos/llvm-project/build/bin/clangd+0x117310b50)
#22 0x000000011c03ad94 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/Users/carson/repos/llvm-project/build/bin/clangd+0x11730ed94)
#23 0x000000011c055574 clang::Parser::ParseCompoundStatementBody(bool) (/Users/carson/repos/llvm-project/build/bin/clangd+0x117329574)
#24 0x000000011c0595c8 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/Users/carson/repos/llvm-project/build/bin/clangd+0x11732d5c8)
#25 0x000000011c2734b0 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/Users/carson/repos/llvm-project/build/bin/clangd+0x1175474b0)
#26 0x000000011c169edc clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/Users/carson/repos/llvm-project/build/bin/clangd+0x11743dedc)
#27 0x000000011c26f0ec clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/Users/carson/repos/llvm-project/build/bin/clangd+0x1175430ec)
#28 0x000000011c26c784 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/Users/carson/repos/llvm-project/build/bin/clangd+0x117540784)
#29 0x000000011c26a130 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/Users/carson/repos/llvm-project/build/bin/clangd+0x11753e130)
#30 0x000000011c263fd0 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/Users/carson/repos/llvm-project/build/bin/clangd+0x117537fd0)
#31 0x000000011c2623a0 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/Users/carson/repos/llvm-project/build/bin/clangd+0x1175363a0)
#32 0x000000011beb7a0c clang::ParseAST(clang::Sema&, bool, bool) (/Users/carson/repos/llvm-project/build/bin/clangd+0x11718ba0c)
#33 0x000000011a5f9858 clang::ASTFrontendAction::ExecuteAction() (/Users/carson/repos/llvm-project/build/bin/clangd+0x1158cd858)
#34 0x000000011a5f7ab8 clang::FrontendAction::Execute() (/Users/carson/repos/llvm-project/build/bin/clangd+0x1158cbab8)
#35 0x00000001102c39d8 clang::clangd::ParsedAST::build(llvm::StringRef, clang::clangd::ParseInputs const&, std::__1::unique_ptr<clang::CompilerInvocation, std::__1::default_delete<clang::CompilerInvocation>>, llvm::ArrayRef<clang::clangd::Diag>, std::__1::shared_ptr<clang::clangd::PreambleData const>) (/Users/carson/repos/llvm-project/build/bin/clangd+0x10b5979d8)
#36 0x000000010eb04598 clang::clangd::(anonymous namespace)::Checker::buildAST() (/Users/carson/repos/llvm-project/build/bin/clangd+0x109dd8598)
#37 0x000000010eb00d74 clang::clangd::check(llvm::StringRef, clang::clangd::ThreadsafeFS const&, clang::clangd::ClangdLSPServer::Options const&) (/Users/carson/repos/llvm-project/build/bin/clangd+0x109dd4d74)
#38 0x000000010e9a726c clang::clangd::clangdMain(int, char**) (/Users/carson/repos/llvm-project/build/bin/clangd+0x109c7b26c)
#39 0x0000000104d2decc main (/Users/carson/repos/llvm-project/build/bin/clangd+0x100001ecc)
#40 0x000000019cb5eb98 
```

---

### Comment 3 - HighCommander4

Thanks for the reports.

The crash is not clangd-specific -- it's in clang frontend code and I can trigger it with `clang -c <sourcefile>` as well. Adjusting issue label accordingly.

---

### Comment 4 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (olemayu)

<details>
This code crashes clangd 21.0.0 (20250601)

```c
static void func(void) {
    alloc_image(device,
                allocator,
                &amp;texture-&gt;image, &amp;texture-&gt;alloc, &amp;texture-&gt;view,
                name, width, height, depth,
                mip_levels, sampling, usage, view_type,
                VK_IMAGE_ASPECT_COLOR_BIT, format);
}
```
</details>


---

### Comment 5 - HighCommander4

Seems to be a trunk regression (clang 20 is unaffected).

---

### Comment 6 - HighCommander4

Bisected to https://github.com/llvm/llvm-project/commit/b144258b0c0cc63, cc @zyn0217 

(This regression window was brought to you by [manyclangs](https://github.com/elfshaker/manyclangs).)

---

### Comment 7 - zyn0217

The delayed typo correction feature is going to be removed, if there's no exception; see https://discourse.llvm.org/t/rfc-removing-support-for-delayed-typo-correction/86631

CC @AaronBallman for ideas that if we should still try to fix the regression on trunk at this point, or wait for it to be ripped out entirely before 21 release.

---

### Comment 8 - AaronBallman

While some folks have expressed interest in working on delayed typo correction, I'm skeptical of its value even if we fix bugs with it. I started a patch to tear it out and am seeing ~.3-.4% compile time improvements and very little in terms of degraded diagnostic behavior. So I think we may want to pull delayed typos for Clang 21 and focus more efforts on improving regular typo correction.

---

