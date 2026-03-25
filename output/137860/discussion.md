# Clang 21 crash (stack dump) when using _Atomic anonymous struct field access with atomic_fetch_add

**Author:** mariete1223
**URL:** https://github.com/llvm/llvm-project/issues/137860
**Status:** Closed
**Labels:** clang:frontend, confirmed, crash
**Closed Date:** 2025-06-13T10:45:42Z

## Body

## Summary

Clang 21 crashes with a stack dump when trying to compile C code that uses _Atomic struct containing an anonymous struct, and accessing a subfield with atomic_fetch_add.
Instead of reporting a normal diagnostic, Clang crashes during semantic analysis (Sema::RefersToMemberWithReducedAlignment).

## Stack Trace

```
Stack dump:
0.      Program arguments: /usr/local/bin/clang-21 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name prueba.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/root -fcoverage-compilation-dir=/root -resource-dir /usr/local/lib/clang/21 -internal-isystem /usr/local/lib/clang/21/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/9/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -std=c2x -ferror-limit 19 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/prueba-d1672b.o -x c prueba.cpp
1.      prueba.cpp:17:1: current parser token '}'
2.      prueba.cpp:15:1: parsing function body 'add'
3.      prueba.cpp:15:1: in compound statement ('{}')
 #0 0x000055bf6ccfb18f llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/usr/local/bin/clang-21+0x3c4a18f)
 #1 0x000055bf6ccf8bd4 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x00007fcb90f86420 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x14420)
 #3 0x000055bf704f588b getInterestingTagDecl(clang::TagDecl*) Type.cpp:0:0
 #4 0x000055bf6f170b2b clang::Sema::RefersToMemberWithReducedAlignment(clang::Expr*, llvm::function_ref<void (clang::Expr*, clang::RecordDecl*, clang::FieldDecl*, clang::CharUnits)>) (/usr/local/bin/clang-21+0x60bfb2b)
 #5 0x000055bf6f171180 clang::Sema::CheckAddressOfPackedMember(clang::Expr*) (/usr/local/bin/clang-21+0x60c0180)
 #6 0x000055bf6f43e823 clang::Sema::CheckAddressOfOperand(clang::ActionResult<clang::Expr*, true>&, clang::SourceLocation) (/usr/local/bin/clang-21+0x638d823)
 #7 0x000055bf6f462710 clang::Sema::CreateBuiltinUnaryOp(clang::SourceLocation, clang::UnaryOperatorKind, clang::Expr*, bool) (/usr/local/bin/clang-21+0x63b1710)
 #8 0x000055bf6f462f85 clang::Sema::BuildUnaryOp(clang::Scope*, clang::SourceLocation, clang::UnaryOperatorKind, clang::Expr*, bool) (/usr/local/bin/clang-21+0x63b1f85)
 #9 0x000055bf6f541845 clang::TreeTransform<(anonymous namespace)::TransformTypos>::TransformInitializer(clang::Expr*, bool) SemaExprCXX.cpp:0:0
#10 0x000055bf6f5415c2 clang::TreeTransform<(anonymous namespace)::TransformTypos>::TransformExprs(clang::Expr* const*, unsigned int, bool, llvm::SmallVectorImpl<clang::Expr*>&, bool*) SemaExprCXX.cpp:0:0
#11 0x000055bf6f54851d clang::TreeTransform<(anonymous namespace)::TransformTypos>::TransformCallExpr(clang::CallExpr*) SemaExprCXX.cpp:0:0
#12 0x000055bf6f55fd4e (anonymous namespace)::TransformTypos::RecursiveTransformLoop(clang::Expr*, bool&) SemaExprCXX.cpp:0:0
#13 0x000055bf6f53d14a clang::Sema::CorrectDelayedTyposInExpr(clang::Expr*, clang::VarDecl*, bool, llvm::function_ref<clang::ActionResult<clang::Expr*, true> (clang::Expr*)>) (/usr/local/bin/clang-21+0x648c14a)
#14 0x000055bf6f53f45d clang::Sema::ActOnFinishFullExpr(clang::Expr*, clang::SourceLocation, bool, bool, bool) (/usr/local/bin/clang-21+0x648e45d)
#15 0x000055bf6f87b09e clang::Sema::ActOnExprStmt(clang::ActionResult<clang::Expr*, true>, bool) (/usr/local/bin/clang-21+0x67ca09e)
#16 0x000055bf6f02fdcc clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/usr/local/bin/clang-21+0x5f7edcc)
#17 0x000055bf6f0236b2 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/usr/local/bin/clang-21+0x5f726b2)
#18 0x000055bf6f024104 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/usr/local/bin/clang-21+0x5f73104)
#19 0x000055bf6f02546c clang::Parser::ParseCompoundStatementBody(bool) (/usr/local/bin/clang-21+0x5f7446c)
#20 0x000055bf6f0277e2 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/usr/local/bin/clang-21+0x5f767e2)
#21 0x000055bf6ef1ee83 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/usr/local/bin/clang-21+0x5e6de83)
#22 0x000055bf6ef68f05 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/usr/local/bin/clang-21+0x5eb7f05)
#23 0x000055bf6ef1762d clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/usr/local/bin/clang-21+0x5e6662d)
#24 0x000055bf6ef1816e clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/usr/local/bin/clang-21+0x5e6716e)
#25 0x000055bf6ef21d73 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/usr/local/bin/clang-21+0x5e70d73)
#26 0x000055bf6ef236a8 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/usr/local/bin/clang-21+0x5e726a8)
#27 0x000055bf6ef1162a clang::ParseAST(clang::Sema&, bool, bool) (/usr/local/bin/clang-21+0x5e6062a)
#28 0x000055bf6d67fe45 clang::CodeGenAction::ExecuteAction() (/usr/local/bin/clang-21+0x45cee45)
#29 0x000055bf6d9649b8 clang::FrontendAction::Execute() (/usr/local/bin/clang-21+0x48b39b8)
#30 0x000055bf6d8e2d3b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/usr/local/bin/clang-21+0x4831d3b)
#31 0x000055bf6da56c7b clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/local/bin/clang-21+0x49a5c7b)
#32 0x000055bf69f53f51 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/usr/local/bin/clang-21+0xea2f51)
#33 0x000055bf69f4c5fd ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#34 0x000055bf69f4f99f clang_main(int, char**, llvm::ToolContext const&) (/usr/local/bin/clang-21+0xe9e99f)
#35 0x000055bf69e2230b main (/usr/local/bin/clang-21+0xd7130b)
```

## Program

```
#include <stdint.h>
#include <stdatomic.h>
struct S {

    struct {
        uint8_t h;
        uint8_t f;
    };

};
_Atomic struct S s = {
                     .h = UINT8_MIN,};
void add(void)
{
    atomic_fetch_add(&s.f, UINT8_MIN);
}
```

## To reproduce quickly 

https://gcc.godbolt.org/z/ojzzEevv7


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (mariete1223)

<details>
## Summary

Clang 21 crashes with a stack dump when trying to compile C code that uses _Atomic struct containing an anonymous struct, and accessing a subfield with atomic_fetch_add.
Instead of reporting a normal diagnostic, Clang crashes during semantic analysis (Sema::RefersToMemberWithReducedAlignment).

## Stack Trace

```
Stack dump:
0.      Program arguments: /usr/local/bin/clang-21 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name prueba.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/root -fcoverage-compilation-dir=/root -resource-dir /usr/local/lib/clang/21 -internal-isystem /usr/local/lib/clang/21/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/9/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -std=c2x -ferror-limit 19 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/prueba-d1672b.o -x c prueba.cpp
1.      prueba.cpp:17:1: current parser token '}'
2.      prueba.cpp:15:1: parsing function body 'add'
3.      prueba.cpp:15:1: in compound statement ('{}')
 #<!-- -->0 0x000055bf6ccfb18f llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/usr/local/bin/clang-21+0x3c4a18f)
 #<!-- -->1 0x000055bf6ccf8bd4 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->2 0x00007fcb90f86420 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x14420)
 #<!-- -->3 0x000055bf704f588b getInterestingTagDecl(clang::TagDecl*) Type.cpp:0:0
 #<!-- -->4 0x000055bf6f170b2b clang::Sema::RefersToMemberWithReducedAlignment(clang::Expr*, llvm::function_ref&lt;void (clang::Expr*, clang::RecordDecl*, clang::FieldDecl*, clang::CharUnits)&gt;) (/usr/local/bin/clang-21+0x60bfb2b)
 #<!-- -->5 0x000055bf6f171180 clang::Sema::CheckAddressOfPackedMember(clang::Expr*) (/usr/local/bin/clang-21+0x60c0180)
 #<!-- -->6 0x000055bf6f43e823 clang::Sema::CheckAddressOfOperand(clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, clang::SourceLocation) (/usr/local/bin/clang-21+0x638d823)
 #<!-- -->7 0x000055bf6f462710 clang::Sema::CreateBuiltinUnaryOp(clang::SourceLocation, clang::UnaryOperatorKind, clang::Expr*, bool) (/usr/local/bin/clang-21+0x63b1710)
 #<!-- -->8 0x000055bf6f462f85 clang::Sema::BuildUnaryOp(clang::Scope*, clang::SourceLocation, clang::UnaryOperatorKind, clang::Expr*, bool) (/usr/local/bin/clang-21+0x63b1f85)
 #<!-- -->9 0x000055bf6f541845 clang::TreeTransform&lt;(anonymous namespace)::TransformTypos&gt;::TransformInitializer(clang::Expr*, bool) SemaExprCXX.cpp:0:0
#<!-- -->10 0x000055bf6f5415c2 clang::TreeTransform&lt;(anonymous namespace)::TransformTypos&gt;::TransformExprs(clang::Expr* const*, unsigned int, bool, llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, bool*) SemaExprCXX.cpp:0:0
#<!-- -->11 0x000055bf6f54851d clang::TreeTransform&lt;(anonymous namespace)::TransformTypos&gt;::TransformCallExpr(clang::CallExpr*) SemaExprCXX.cpp:0:0
#<!-- -->12 0x000055bf6f55fd4e (anonymous namespace)::TransformTypos::RecursiveTransformLoop(clang::Expr*, bool&amp;) SemaExprCXX.cpp:0:0
#<!-- -->13 0x000055bf6f53d14a clang::Sema::CorrectDelayedTyposInExpr(clang::Expr*, clang::VarDecl*, bool, llvm::function_ref&lt;clang::ActionResult&lt;clang::Expr*, true&gt; (clang::Expr*)&gt;) (/usr/local/bin/clang-21+0x648c14a)
#<!-- -->14 0x000055bf6f53f45d clang::Sema::ActOnFinishFullExpr(clang::Expr*, clang::SourceLocation, bool, bool, bool) (/usr/local/bin/clang-21+0x648e45d)
#<!-- -->15 0x000055bf6f87b09e clang::Sema::ActOnExprStmt(clang::ActionResult&lt;clang::Expr*, true&gt;, bool) (/usr/local/bin/clang-21+0x67ca09e)
#<!-- -->16 0x000055bf6f02fdcc clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/usr/local/bin/clang-21+0x5f7edcc)
#<!-- -->17 0x000055bf6f0236b2 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/usr/local/bin/clang-21+0x5f726b2)
#<!-- -->18 0x000055bf6f024104 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/usr/local/bin/clang-21+0x5f73104)
#<!-- -->19 0x000055bf6f02546c clang::Parser::ParseCompoundStatementBody(bool) (/usr/local/bin/clang-21+0x5f7446c)
#<!-- -->20 0x000055bf6f0277e2 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/usr/local/bin/clang-21+0x5f767e2)
#<!-- -->21 0x000055bf6ef1ee83 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/usr/local/bin/clang-21+0x5e6de83)
#<!-- -->22 0x000055bf6ef68f05 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/usr/local/bin/clang-21+0x5eb7f05)
#<!-- -->23 0x000055bf6ef1762d clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/usr/local/bin/clang-21+0x5e6662d)
#<!-- -->24 0x000055bf6ef1816e clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/usr/local/bin/clang-21+0x5e6716e)
#<!-- -->25 0x000055bf6ef21d73 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/usr/local/bin/clang-21+0x5e70d73)
#<!-- -->26 0x000055bf6ef236a8 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/usr/local/bin/clang-21+0x5e726a8)
#<!-- -->27 0x000055bf6ef1162a clang::ParseAST(clang::Sema&amp;, bool, bool) (/usr/local/bin/clang-21+0x5e6062a)
#<!-- -->28 0x000055bf6d67fe45 clang::CodeGenAction::ExecuteAction() (/usr/local/bin/clang-21+0x45cee45)
#<!-- -->29 0x000055bf6d9649b8 clang::FrontendAction::Execute() (/usr/local/bin/clang-21+0x48b39b8)
#<!-- -->30 0x000055bf6d8e2d3b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/usr/local/bin/clang-21+0x4831d3b)
#<!-- -->31 0x000055bf6da56c7b clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/local/bin/clang-21+0x49a5c7b)
#<!-- -->32 0x000055bf69f53f51 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/usr/local/bin/clang-21+0xea2f51)
#<!-- -->33 0x000055bf69f4c5fd ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->34 0x000055bf69f4f99f clang_main(int, char**, llvm::ToolContext const&amp;) (/usr/local/bin/clang-21+0xe9e99f)
#<!-- -->35 0x000055bf69e2230b main (/usr/local/bin/clang-21+0xd7130b)
```

## Program

```
#include &lt;stdint.h&gt;
#include &lt;stdatomic.h&gt;
struct S {

    struct {
        uint8_t h;
        uint8_t f;
    };

};
_Atomic struct S s = {
                     .h = UINT8_MIN,};
void add(void)
{
    atomic_fetch_add(&amp;s.f, UINT8_MIN);
}
```

## To reproduce quickly 

https://gcc.godbolt.org/z/ojzzEevv7

</details>


---

### Comment 2 - shafik

Goes back to clang-15

---

### Comment 3 - sweiglbosker

A couple things:
Using an undefined identifier seems root cause of the failed assertion. Note that when you `#define UINT8_MIN 1` the example above will behave as expected.
The "correct" tree we are looking for after we reverse the chain of member exprs is something like this, where errors are propagated correctly:
```
MemberExpr 0x55556ccca610 'uint8_t':'unsigned char' contains-errors lvalue .f 0x55556ccc9ab0
`-MemberExpr 0x55556ccca5d8 'struct S::(anonymous at atom3.c:8:5)' contains-errors lvalue .struct S::(anonymous at atom3.c:8:5) 0x55556ccc9b58
  `-ImplicitCastExpr 0x55556ccca5c0 'struct S' contains-errors lvalue <AtomicToNonAtomic>
    `-DeclRefExpr 0x55556ccca5a0 '_Atomic(struct S)' contains-errors lvalue Var 0x55556ccc9d80 's' '_Atomic(struct S)'
```
what we end up with is this:
```
MemberExpr 0x55556ccca3e0 'uint8_t':'unsigned char' lvalue .f 0x55556ccc9d00
`-MemberExpr 0x55556ccca3a8 'struct S::(anonymous at atom.c:5:5)' lvalue .struct S::(anonymous at atom.c:5:5) 0x55556ccc9da8
  `-ImplicitCastExpr 0x55556ccca390 'struct S' lvalue <AtomicToNonAtomic>
    `-DeclRefExpr 0x55556ccca370 '_Atomic(struct S)' lvalue Var 0x55556ccc9fd0 's' '_Atomic(struct S)'
```
tracing this back a bit, it starts with this function call: https://github.com/llvm/llvm-project/blob/4efcc52ed839c4348c69a01538c7ecd399e4b113/clang/lib/Parse/ParseExpr.cpp#L1366

---

