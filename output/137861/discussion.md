# Clang 21 crash (stack dump) when parsing #pragma clang attribute push inside function body

**Author:** mariete1223
**URL:** https://github.com/llvm/llvm-project/issues/137861
**Status:** Closed
**Labels:** clang:frontend, confirmed, crash
**Closed Date:** 2025-04-30T14:31:43Z

## Body

## Summary:

Clang 21 crashes when parsing a #pragma clang attribute push directive inside a function body, wrapping a compound statement {}.
Instead of rejecting the invalid pragma usage or handling it gracefully, Clang crashes during parsing and statement attribute processing (ProcessStmtAttribute).

##  Stack trace

```
Stack dump:
0.      Program arguments: /usr/local/bin/clang-21 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name prueba.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/root -fcoverage-compilation-dir=/root -resource-dir /usr/local/lib/clang/21 -internal-isystem /usr/local/lib/clang/21/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/9/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -std=c2x -ferror-limit 19 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/prueba-0fe160.o -x c prueba.cpp
1.      prueba.cpp:11:1: current parser token '{'
2.      prueba.cpp:5:11: parsing function body 'main'
3.      prueba.cpp:5:11: in compound statement ('{}')
 #0 0x000055c08fa8118f llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/usr/local/bin/clang-21+0x3c4a18f)
 #1 0x000055c08fa7ebd4 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x00007f2863c40420 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x14420)
 #3 0x000055c0931f2523 clang::Stmt::getBeginLoc() const (/usr/local/bin/clang-21+0x73bb523)
 #4 0x000055c092644dcb ProcessStmtAttribute(clang::Sema&, clang::Stmt*, clang::ParsedAttr const&, clang::SourceRange) SemaStmtAttr.cpp:0:0
 #5 0x000055c092648382 clang::Sema::ProcessStmtAttributes(clang::Stmt*, clang::ParsedAttributes const&, llvm::SmallVectorImpl<clang::Attr const*>&) (/usr/local/bin/clang-21+0x6811382)
 #6 0x000055c09260db87 clang::Sema::ActOnAttributedStmt(clang::ParsedAttributes const&, clang::Stmt*) (/usr/local/bin/clang-21+0x67d6b87)
 #7 0x000055c091daa315 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/usr/local/bin/clang-21+0x5f73315)
 #8 0x000055c091dab46c clang::Parser::ParseCompoundStatementBody(bool) (/usr/local/bin/clang-21+0x5f7446c)
 #9 0x000055c091dad7e2 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/usr/local/bin/clang-21+0x5f767e2)
#10 0x000055c091ca4e83 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/usr/local/bin/clang-21+0x5e6de83)
#11 0x000055c091ceef05 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/usr/local/bin/clang-21+0x5eb7f05)
#12 0x000055c091c9d62d clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/usr/local/bin/clang-21+0x5e6662d)
#13 0x000055c091c9e16e clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/usr/local/bin/clang-21+0x5e6716e)
#14 0x000055c091ca7d73 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/usr/local/bin/clang-21+0x5e70d73)
#15 0x000055c091ca96a8 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/usr/local/bin/clang-21+0x5e726a8)
#16 0x000055c091c9762a clang::ParseAST(clang::Sema&, bool, bool) (/usr/local/bin/clang-21+0x5e6062a)
#17 0x000055c090405e45 clang::CodeGenAction::ExecuteAction() (/usr/local/bin/clang-21+0x45cee45)
#18 0x000055c0906ea9b8 clang::FrontendAction::Execute() (/usr/local/bin/clang-21+0x48b39b8)
#19 0x000055c090668d3b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/usr/local/bin/clang-21+0x4831d3b)
#20 0x000055c0907dcc7b clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/local/bin/clang-21+0x49a5c7b)
#21 0x000055c08ccd9f51 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/usr/local/bin/clang-21+0xea2f51)
#22 0x000055c08ccd25fd ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#23 0x000055c08ccd599f clang_main(int, char**, llvm::ToolContext const&) (/usr/local/bin/clang-21+0xe9e99f)
#24 0x000055c08cba830b main (/usr/local/bin/clang-21+0xd7130b)
#25 0x00007f28636ee083 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x24083)
#26 0x000055c08ccd207e _start (/usr/local/bin/clang-21+0xe9b07e)
```

## Program

```
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
int main(){
__attribute__ ((deprecated))
int my_val = 5;
const char* my_string = "Hello, World!\n";
__attribute__((aligned(64)))
#pragma clang attribute push(__attribute__((cleanup(cleanup1))))
{
char* p = malloc(100);
free(p);
 }
#pragma clang attribute pop
}
```

## To quickly reproduce

https://gcc.godbolt.org/z/3xc5qaM4T

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (mariete1223)

<details>
## Summary:

Clang 21 crashes when parsing a #pragma clang attribute push directive inside a function body, wrapping a compound statement {}.
Instead of rejecting the invalid pragma usage or handling it gracefully, Clang crashes during parsing and statement attribute processing (ProcessStmtAttribute).

##  Stack trace

```
Stack dump:
0.      Program arguments: /usr/local/bin/clang-21 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name prueba.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/root -fcoverage-compilation-dir=/root -resource-dir /usr/local/lib/clang/21 -internal-isystem /usr/local/lib/clang/21/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/9/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -std=c2x -ferror-limit 19 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/prueba-0fe160.o -x c prueba.cpp
1.      prueba.cpp:11:1: current parser token '{'
2.      prueba.cpp:5:11: parsing function body 'main'
3.      prueba.cpp:5:11: in compound statement ('{}')
 #<!-- -->0 0x000055c08fa8118f llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/usr/local/bin/clang-21+0x3c4a18f)
 #<!-- -->1 0x000055c08fa7ebd4 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->2 0x00007f2863c40420 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x14420)
 #<!-- -->3 0x000055c0931f2523 clang::Stmt::getBeginLoc() const (/usr/local/bin/clang-21+0x73bb523)
 #<!-- -->4 0x000055c092644dcb ProcessStmtAttribute(clang::Sema&amp;, clang::Stmt*, clang::ParsedAttr const&amp;, clang::SourceRange) SemaStmtAttr.cpp:0:0
 #<!-- -->5 0x000055c092648382 clang::Sema::ProcessStmtAttributes(clang::Stmt*, clang::ParsedAttributes const&amp;, llvm::SmallVectorImpl&lt;clang::Attr const*&gt;&amp;) (/usr/local/bin/clang-21+0x6811382)
 #<!-- -->6 0x000055c09260db87 clang::Sema::ActOnAttributedStmt(clang::ParsedAttributes const&amp;, clang::Stmt*) (/usr/local/bin/clang-21+0x67d6b87)
 #<!-- -->7 0x000055c091daa315 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/usr/local/bin/clang-21+0x5f73315)
 #<!-- -->8 0x000055c091dab46c clang::Parser::ParseCompoundStatementBody(bool) (/usr/local/bin/clang-21+0x5f7446c)
 #<!-- -->9 0x000055c091dad7e2 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/usr/local/bin/clang-21+0x5f767e2)
#<!-- -->10 0x000055c091ca4e83 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/usr/local/bin/clang-21+0x5e6de83)
#<!-- -->11 0x000055c091ceef05 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/usr/local/bin/clang-21+0x5eb7f05)
#<!-- -->12 0x000055c091c9d62d clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/usr/local/bin/clang-21+0x5e6662d)
#<!-- -->13 0x000055c091c9e16e clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/usr/local/bin/clang-21+0x5e6716e)
#<!-- -->14 0x000055c091ca7d73 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/usr/local/bin/clang-21+0x5e70d73)
#<!-- -->15 0x000055c091ca96a8 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/usr/local/bin/clang-21+0x5e726a8)
#<!-- -->16 0x000055c091c9762a clang::ParseAST(clang::Sema&amp;, bool, bool) (/usr/local/bin/clang-21+0x5e6062a)
#<!-- -->17 0x000055c090405e45 clang::CodeGenAction::ExecuteAction() (/usr/local/bin/clang-21+0x45cee45)
#<!-- -->18 0x000055c0906ea9b8 clang::FrontendAction::Execute() (/usr/local/bin/clang-21+0x48b39b8)
#<!-- -->19 0x000055c090668d3b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/usr/local/bin/clang-21+0x4831d3b)
#<!-- -->20 0x000055c0907dcc7b clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/local/bin/clang-21+0x49a5c7b)
#<!-- -->21 0x000055c08ccd9f51 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/usr/local/bin/clang-21+0xea2f51)
#<!-- -->22 0x000055c08ccd25fd ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->23 0x000055c08ccd599f clang_main(int, char**, llvm::ToolContext const&amp;) (/usr/local/bin/clang-21+0xe9e99f)
#<!-- -->24 0x000055c08cba830b main (/usr/local/bin/clang-21+0xd7130b)
#<!-- -->25 0x00007f28636ee083 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x24083)
#<!-- -->26 0x000055c08ccd207e _start (/usr/local/bin/clang-21+0xe9b07e)
```

## Program

```
#include &lt;stdio.h&gt;
#include &lt;math.h&gt;
#include &lt;stdlib.h&gt;
#include &lt;stdio.h&gt;
int main(){
__attribute__ ((deprecated))
int my_val = 5;
const char* my_string = "Hello, World!\n";
__attribute__((aligned(64)))
#pragma clang attribute push(__attribute__((cleanup(cleanup1))))
{
char* p = malloc(100);
free(p);
 }
#pragma clang attribute pop
}
```

## To quickly reproduce

https://gcc.godbolt.org/z/3xc5qaM4T
</details>


---

### Comment 2 - shafik

Goes back to clang-10, looks like an attributes issue.

CC @erichkeane 

---

