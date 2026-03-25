# Clang 21 crashes (stack dump) on invalid nested struct initialization

**Author:** mariete1223
**URL:** https://github.com/llvm/llvm-project/issues/137847

## Body

## Summary 

Clang 21 crashes during compilation when trying to compile an invalid C program with incorrect nested struct initialization.
Instead of reporting a normal error about the bad initializer, the compiler crashes internally during InitListChecker::CheckExplicitInitList.


## Stack Trace
```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /usr/local/bin/clang-21 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name prueba.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/root -fcoverage-compilation-dir=/root -resource-dir /usr/local/lib/clang/21 -internal-isystem /usr/local/lib/clang/21/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/9/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -std=c2x -ferror-limit 19 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/prueba-871b16.o -x c prueba.cpp
1.      prueba.cpp:11:20: current parser token ';'
2.      prueba.cpp:4:1: parsing function body 'main'
3.      prueba.cpp:4:1: in compound statement ('{}')
 #0 0x000055ecf098418f llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/usr/local/bin/clang-21+0x3c4a18f)
 #1 0x000055ecf0981bd4 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x00007fb911ec7420 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x14420)
 #3 0x000055ecf328ab44 (anonymous namespace)::InitListChecker::CheckExplicitInitList(clang::InitializedEntity const&, clang::InitListExpr*, clang::QualType&, clang::InitListExpr*, bool) (.constprop.0) SemaInit.cpp:0:0
 #4 0x000055ecf328b217 (anonymous namespace)::InitListChecker::InitListChecker(clang::Sema&, clang::InitializedEntity const&, clang::InitListExpr*, clang::QualType&, bool, bool, bool, llvm::SmallVectorImpl<clang::QualType>*) SemaInit.cpp:0:0
 #5 0x000055ecf328cf23 clang::InitializationSequence::Perform(clang::Sema&, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::MutableArrayRef<clang::Expr*>, clang::QualType*) (/usr/local/bin/clang-21+0x6552f23)
 #6 0x000055ecf329b18a (anonymous namespace)::InitListChecker::CheckSubElementType(clang::InitializedEntity const&, clang::InitListExpr*, clang::QualType, unsigned int&, clang::InitListExpr*, unsigned int&, bool) SemaInit.cpp:0:0
 #7 0x000055ecf329be4b (anonymous namespace)::InitListChecker::CheckStructUnionTypes(clang::InitializedEntity const&, clang::InitListExpr*, clang::QualType, llvm::iterator_range<clang::CXXBaseSpecifier const*>, clang::DeclContext::specific_decl_iterator<clang::FieldDecl>, bool, unsigned int&, clang::InitListExpr*, unsigned int&, bool) SemaInit.cpp:0:0
 #8 0x000055ecf3289f52 (anonymous namespace)::InitListChecker::CheckListElementTypes(clang::InitializedEntity const&, clang::InitListExpr*, clang::QualType&, bool, unsigned int&, clang::InitListExpr*, unsigned int&, bool) (.constprop.0) SemaInit.cpp:0:0
 #9 0x000055ecf328aa3d (anonymous namespace)::InitListChecker::CheckExplicitInitList(clang::InitializedEntity const&, clang::InitListExpr*, clang::QualType&, clang::InitListExpr*, bool) (.constprop.0) SemaInit.cpp:0:0
#10 0x000055ecf328b217 (anonymous namespace)::InitListChecker::InitListChecker(clang::Sema&, clang::InitializedEntity const&, clang::InitListExpr*, clang::QualType&, bool, bool, bool, llvm::SmallVectorImpl<clang::QualType>*) SemaInit.cpp:0:0
#11 0x000055ecf328cf23 clang::InitializationSequence::Perform(clang::Sema&, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::MutableArrayRef<clang::Expr*>, clang::QualType*) (/usr/local/bin/clang-21+0x6552f23)
#12 0x000055ecf2f5ce2c clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) (/usr/local/bin/clang-21+0x6222e2c)
#13 0x000055ecf2bdc15c clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/usr/local/bin/clang-21+0x5ea215c)
#14 0x000055ecf2bf1a31 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/usr/local/bin/clang-21+0x5eb7a31)
#15 0x000055ecf2bf45f0 clang::Parser::ParseSimpleDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, bool, clang::Parser::ForRangeInit*, clang::SourceLocation*) (/usr/local/bin/clang-21+0x5eba5f0)
#16 0x000055ecf2bf4b37 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/usr/local/bin/clang-21+0x5ebab37)
#17 0x000055ecf2cac74e clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/usr/local/bin/clang-21+0x5f7274e)
#18 0x000055ecf2cad104 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/usr/local/bin/clang-21+0x5f73104)
#19 0x000055ecf2cae46c clang::Parser::ParseCompoundStatementBody(bool) (/usr/local/bin/clang-21+0x5f7446c)
#20 0x000055ecf2cb07e2 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/usr/local/bin/clang-21+0x5f767e2)
#21 0x000055ecf2ba7e83 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/usr/local/bin/clang-21+0x5e6de83)
#22 0x000055ecf2bf1f05 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/usr/local/bin/clang-21+0x5eb7f05)
#23 0x000055ecf2ba062d clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/usr/local/bin/clang-21+0x5e6662d)
#24 0x000055ecf2ba116e clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/usr/local/bin/clang-21+0x5e6716e)
#25 0x000055ecf2baad73 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/usr/local/bin/clang-21+0x5e70d73)
#26 0x000055ecf2bac6a8 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/usr/local/bin/clang-21+0x5e726a8)
#27 0x000055ecf2b9a62a clang::ParseAST(clang::Sema&, bool, bool) (/usr/local/bin/clang-21+0x5e6062a)
#28 0x000055ecf1308e45 clang::CodeGenAction::ExecuteAction() (/usr/local/bin/clang-21+0x45cee45)
#29 0x000055ecf15ed9b8 clang::FrontendAction::Execute() (/usr/local/bin/clang-21+0x48b39b8)
#30 0x000055ecf156bd3b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/usr/local/bin/clang-21+0x4831d3b)
#31 0x000055ecf16dfc7b clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/local/bin/clang-21+0x49a5c7b)
#32 0x000055ecedbdcf51 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/usr/local/bin/clang-21+0xea2f51)
#33 0x000055ecedbd55fd ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#34 0x000055ecedbd899f clang_main(int, char**, llvm::ToolContext const&) (/usr/local/bin/clang-21+0xe9e99f)
#35 0x000055ecedaab30b main (/usr/local/bin/clang-21+0xd7130b)
#36 0x00007fb911975083 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x24083)
#37 0x000055ecedbd507e _start (/usr/local/bin/clang-21+0xe9b07e)
```


## Program 

```
#include <stdlib.h>
#include <stddef.h>
int main(void)
{
    struct A {
        int a;
        struct {
            int b;
        } c;
        int d;
    } a = {{{}, 1}};
    return 0;
}
```

## To reproduce quickly

https://gcc.godbolt.org/z/cT9v5hreK


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (mariete1223)

<details>
## Summary 

Clang 21 crashes during compilation when trying to compile an invalid C program with incorrect nested struct initialization.
Instead of reporting a normal error about the bad initializer, the compiler crashes internally during InitListChecker::CheckExplicitInitList.


## Stack Trace
```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /usr/local/bin/clang-21 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name prueba.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/root -fcoverage-compilation-dir=/root -resource-dir /usr/local/lib/clang/21 -internal-isystem /usr/local/lib/clang/21/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/9/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -std=c2x -ferror-limit 19 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/prueba-871b16.o -x c prueba.cpp
1.      prueba.cpp:11:20: current parser token ';'
2.      prueba.cpp:4:1: parsing function body 'main'
3.      prueba.cpp:4:1: in compound statement ('{}')
 #<!-- -->0 0x000055ecf098418f llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/usr/local/bin/clang-21+0x3c4a18f)
 #<!-- -->1 0x000055ecf0981bd4 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->2 0x00007fb911ec7420 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x14420)
 #<!-- -->3 0x000055ecf328ab44 (anonymous namespace)::InitListChecker::CheckExplicitInitList(clang::InitializedEntity const&amp;, clang::InitListExpr*, clang::QualType&amp;, clang::InitListExpr*, bool) (.constprop.0) SemaInit.cpp:0:0
 #<!-- -->4 0x000055ecf328b217 (anonymous namespace)::InitListChecker::InitListChecker(clang::Sema&amp;, clang::InitializedEntity const&amp;, clang::InitListExpr*, clang::QualType&amp;, bool, bool, bool, llvm::SmallVectorImpl&lt;clang::QualType&gt;*) SemaInit.cpp:0:0
 #<!-- -->5 0x000055ecf328cf23 clang::InitializationSequence::Perform(clang::Sema&amp;, clang::InitializedEntity const&amp;, clang::InitializationKind const&amp;, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::QualType*) (/usr/local/bin/clang-21+0x6552f23)
 #<!-- -->6 0x000055ecf329b18a (anonymous namespace)::InitListChecker::CheckSubElementType(clang::InitializedEntity const&amp;, clang::InitListExpr*, clang::QualType, unsigned int&amp;, clang::InitListExpr*, unsigned int&amp;, bool) SemaInit.cpp:0:0
 #<!-- -->7 0x000055ecf329be4b (anonymous namespace)::InitListChecker::CheckStructUnionTypes(clang::InitializedEntity const&amp;, clang::InitListExpr*, clang::QualType, llvm::iterator_range&lt;clang::CXXBaseSpecifier const*&gt;, clang::DeclContext::specific_decl_iterator&lt;clang::FieldDecl&gt;, bool, unsigned int&amp;, clang::InitListExpr*, unsigned int&amp;, bool) SemaInit.cpp:0:0
 #<!-- -->8 0x000055ecf3289f52 (anonymous namespace)::InitListChecker::CheckListElementTypes(clang::InitializedEntity const&amp;, clang::InitListExpr*, clang::QualType&amp;, bool, unsigned int&amp;, clang::InitListExpr*, unsigned int&amp;, bool) (.constprop.0) SemaInit.cpp:0:0
 #<!-- -->9 0x000055ecf328aa3d (anonymous namespace)::InitListChecker::CheckExplicitInitList(clang::InitializedEntity const&amp;, clang::InitListExpr*, clang::QualType&amp;, clang::InitListExpr*, bool) (.constprop.0) SemaInit.cpp:0:0
#<!-- -->10 0x000055ecf328b217 (anonymous namespace)::InitListChecker::InitListChecker(clang::Sema&amp;, clang::InitializedEntity const&amp;, clang::InitListExpr*, clang::QualType&amp;, bool, bool, bool, llvm::SmallVectorImpl&lt;clang::QualType&gt;*) SemaInit.cpp:0:0
#<!-- -->11 0x000055ecf328cf23 clang::InitializationSequence::Perform(clang::Sema&amp;, clang::InitializedEntity const&amp;, clang::InitializationKind const&amp;, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::QualType*) (/usr/local/bin/clang-21+0x6552f23)
#<!-- -->12 0x000055ecf2f5ce2c clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) (/usr/local/bin/clang-21+0x6222e2c)
#<!-- -->13 0x000055ecf2bdc15c clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) (/usr/local/bin/clang-21+0x5ea215c)
#<!-- -->14 0x000055ecf2bf1a31 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/usr/local/bin/clang-21+0x5eb7a31)
#<!-- -->15 0x000055ecf2bf45f0 clang::Parser::ParseSimpleDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, bool, clang::Parser::ForRangeInit*, clang::SourceLocation*) (/usr/local/bin/clang-21+0x5eba5f0)
#<!-- -->16 0x000055ecf2bf4b37 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/usr/local/bin/clang-21+0x5ebab37)
#<!-- -->17 0x000055ecf2cac74e clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/usr/local/bin/clang-21+0x5f7274e)
#<!-- -->18 0x000055ecf2cad104 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/usr/local/bin/clang-21+0x5f73104)
#<!-- -->19 0x000055ecf2cae46c clang::Parser::ParseCompoundStatementBody(bool) (/usr/local/bin/clang-21+0x5f7446c)
#<!-- -->20 0x000055ecf2cb07e2 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/usr/local/bin/clang-21+0x5f767e2)
#<!-- -->21 0x000055ecf2ba7e83 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/usr/local/bin/clang-21+0x5e6de83)
#<!-- -->22 0x000055ecf2bf1f05 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/usr/local/bin/clang-21+0x5eb7f05)
#<!-- -->23 0x000055ecf2ba062d clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/usr/local/bin/clang-21+0x5e6662d)
#<!-- -->24 0x000055ecf2ba116e clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/usr/local/bin/clang-21+0x5e6716e)
#<!-- -->25 0x000055ecf2baad73 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/usr/local/bin/clang-21+0x5e70d73)
#<!-- -->26 0x000055ecf2bac6a8 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/usr/local/bin/clang-21+0x5e726a8)
#<!-- -->27 0x000055ecf2b9a62a clang::ParseAST(clang::Sema&amp;, bool, bool) (/usr/local/bin/clang-21+0x5e6062a)
#<!-- -->28 0x000055ecf1308e45 clang::CodeGenAction::ExecuteAction() (/usr/local/bin/clang-21+0x45cee45)
#<!-- -->29 0x000055ecf15ed9b8 clang::FrontendAction::Execute() (/usr/local/bin/clang-21+0x48b39b8)
#<!-- -->30 0x000055ecf156bd3b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/usr/local/bin/clang-21+0x4831d3b)
#<!-- -->31 0x000055ecf16dfc7b clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/local/bin/clang-21+0x49a5c7b)
#<!-- -->32 0x000055ecedbdcf51 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/usr/local/bin/clang-21+0xea2f51)
#<!-- -->33 0x000055ecedbd55fd ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->34 0x000055ecedbd899f clang_main(int, char**, llvm::ToolContext const&amp;) (/usr/local/bin/clang-21+0xe9e99f)
#<!-- -->35 0x000055ecedaab30b main (/usr/local/bin/clang-21+0xd7130b)
#<!-- -->36 0x00007fb911975083 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x24083)
#<!-- -->37 0x000055ecedbd507e _start (/usr/local/bin/clang-21+0xe9b07e)
```


## Program 

```
#include &lt;stdlib.h&gt;
#include &lt;stddef.h&gt;
int main(void)
{
    struct A {
        int a;
        struct {
            int b;
        } c;
        int d;
    } a = {{{}, 1}};
    return 0;
}
```

## To reproduce quickly

https://gcc.godbolt.org/z/cT9v5hreK

</details>


---

### Comment 2 - AaronBallman

The crash started happening in Clang 17: https://gcc.godbolt.org/z/h9q3E53T7

---

