# Clang 21 crash when using __attribute__((alloc_size(2,1))) on variadic function declaration

**Author:** mariete1223
**URL:** https://github.com/llvm/llvm-project/issues/137870
**Status:** Closed
**Labels:** clang:frontend, crash
**Closed Date:** 2025-04-30T00:38:27Z

## Body

## Summary 

Clang 21 crashes with a segmentation fault when compiling a variadic function declaration annotated with __attribute__((alloc_size(2,1))).
The compiler fails to check whether the arguments to the attribute are valid for a variadic function before trying to process them.

## Stack Dump

```
Stack dump:
0.      Program arguments: /usr/local/bin/clang-21 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name prueba.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=none -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/root -fcoverage-compilation-dir=/root -resource-dir /usr/local/lib/clang/21 -internal-isystem /usr/local/lib/clang/21/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/9/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -O3 -std=c2x -ferror-limit 19 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -vectorize-loops -vectorize-slp -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/prueba-ee2b13.o -x c prueba.cpp
1.      prueba.cpp:7:64: current parser token '{'
 #0 0x0000564cefc1e18f llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/usr/local/bin/clang-21+0x3c4a18f)
 #1 0x0000564cefc1bbd4 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x00007fd7c765f420 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x14420)
 #3 0x0000564cf223ca85 bool checkParamIsIntegerType<clang::ParsedAttr>(clang::Sema&, clang::Decl const*, clang::ParsedAttr const&, unsigned int) SemaDeclAttr.cpp:0:0
 #4 0x0000564cf2243e56 handleAllocSizeAttr(clang::Sema&, clang::Decl*, clang::ParsedAttr const&) SemaDeclAttr.cpp:0:0
 #5 0x0000564cf224edb6 ProcessDeclAttribute(clang::Sema&, clang::Scope*, clang::Decl*, clang::ParsedAttr const&, clang::Sema::ProcessDeclAttributeOptions const&) (.isra.0) SemaDeclAttr.cpp:0:0
 #6 0x0000564cf22520f5 clang::Sema::ProcessDeclAttributes(clang::Scope*, clang::Decl*, clang::Declarator const&) (/usr/local/bin/clang-21+0x627e0f5)
 #7 0x0000564cf220787c clang::Sema::ActOnFunctionDeclarator(clang::Scope*, clang::Declarator&, clang::DeclContext*, clang::TypeSourceInfo*, clang::LookupResult&, llvm::MutableArrayRef<clang::TemplateParameterList*>, bool&) (/usr/local/bin/clang-21+0x623387c)
 #8 0x0000564cf220d114 clang::Sema::HandleDeclarator(clang::Scope*, clang::Declarator&, llvm::MutableArrayRef<clang::TemplateParameterList*>) (/usr/local/bin/clang-21+0x6239114)
 #9 0x0000564cf220d93c clang::Sema::ActOnStartOfFunctionDef(clang::Scope*, clang::Declarator&, llvm::MutableArrayRef<clang::TemplateParameterList*>, clang::SkipBodyInfo*, clang::Sema::FnBodyKind) (/usr/local/bin/clang-21+0x623993c)
#10 0x0000564cf1e41db1 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/usr/local/bin/clang-21+0x5e6ddb1)
#11 0x0000564cf1e8bf05 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/usr/local/bin/clang-21+0x5eb7f05)
#12 0x0000564cf1e3a62d clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/usr/local/bin/clang-21+0x5e6662d)
#13 0x0000564cf1e3b16e clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/usr/local/bin/clang-21+0x5e6716e)
#14 0x0000564cf1e44d73 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/usr/local/bin/clang-21+0x5e70d73)
#15 0x0000564cf1e466a8 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/usr/local/bin/clang-21+0x5e726a8)
#16 0x0000564cf1e3462a clang::ParseAST(clang::Sema&, bool, bool) (/usr/local/bin/clang-21+0x5e6062a)
#17 0x0000564cf05a2e45 clang::CodeGenAction::ExecuteAction() (/usr/local/bin/clang-21+0x45cee45)
#18 0x0000564cf08879b8 clang::FrontendAction::Execute() (/usr/local/bin/clang-21+0x48b39b8)
#19 0x0000564cf0805d3b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/usr/local/bin/clang-21+0x4831d3b)
#20 0x0000564cf0979c7b clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/local/bin/clang-21+0x49a5c7b)
#21 0x0000564cece76f51 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/usr/local/bin/clang-21+0xea2f51)
#22 0x0000564cece6f5fd ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#23 0x0000564cece7299f clang_main(int, char**, llvm::ToolContext const&) (/usr/local/bin/clang-21+0xe9e99f)
#24 0x0000564cecd4530b main (/usr/local/bin/clang-21+0xd7130b)
#25 0x00007fd7c710d083 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x24083)
#26 0x0000564cece6f07e _start (/usr/local/bin/clang-21+0xe9b07e)
```

## Program

```
#include <stdlib.h>
#include <stdint.h>
typedef struct {
    void *p;
    size_t n;
} ptn_p;
void *__attribute__((alloc_size(2, 1))) malloc_x(size_t n,...) {
}
```

## To quickly reproduce

https://gcc.godbolt.org/z/WEqaqcGMb

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (mariete1223)

<details>
## Summary 

Clang 21 crashes with a segmentation fault when compiling a variadic function declaration annotated with __attribute__((alloc_size(2,1))).
The compiler fails to check whether the arguments to the attribute are valid for a variadic function before trying to process them.

## Stack Dump

```
Stack dump:
0.      Program arguments: /usr/local/bin/clang-21 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name prueba.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=none -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/root -fcoverage-compilation-dir=/root -resource-dir /usr/local/lib/clang/21 -internal-isystem /usr/local/lib/clang/21/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/9/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -O3 -std=c2x -ferror-limit 19 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -vectorize-loops -vectorize-slp -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/prueba-ee2b13.o -x c prueba.cpp
1.      prueba.cpp:7:64: current parser token '{'
 #<!-- -->0 0x0000564cefc1e18f llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/usr/local/bin/clang-21+0x3c4a18f)
 #<!-- -->1 0x0000564cefc1bbd4 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->2 0x00007fd7c765f420 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x14420)
 #<!-- -->3 0x0000564cf223ca85 bool checkParamIsIntegerType&lt;clang::ParsedAttr&gt;(clang::Sema&amp;, clang::Decl const*, clang::ParsedAttr const&amp;, unsigned int) SemaDeclAttr.cpp:0:0
 #<!-- -->4 0x0000564cf2243e56 handleAllocSizeAttr(clang::Sema&amp;, clang::Decl*, clang::ParsedAttr const&amp;) SemaDeclAttr.cpp:0:0
 #<!-- -->5 0x0000564cf224edb6 ProcessDeclAttribute(clang::Sema&amp;, clang::Scope*, clang::Decl*, clang::ParsedAttr const&amp;, clang::Sema::ProcessDeclAttributeOptions const&amp;) (.isra.0) SemaDeclAttr.cpp:0:0
 #<!-- -->6 0x0000564cf22520f5 clang::Sema::ProcessDeclAttributes(clang::Scope*, clang::Decl*, clang::Declarator const&amp;) (/usr/local/bin/clang-21+0x627e0f5)
 #<!-- -->7 0x0000564cf220787c clang::Sema::ActOnFunctionDeclarator(clang::Scope*, clang::Declarator&amp;, clang::DeclContext*, clang::TypeSourceInfo*, clang::LookupResult&amp;, llvm::MutableArrayRef&lt;clang::TemplateParameterList*&gt;, bool&amp;) (/usr/local/bin/clang-21+0x623387c)
 #<!-- -->8 0x0000564cf220d114 clang::Sema::HandleDeclarator(clang::Scope*, clang::Declarator&amp;, llvm::MutableArrayRef&lt;clang::TemplateParameterList*&gt;) (/usr/local/bin/clang-21+0x6239114)
 #<!-- -->9 0x0000564cf220d93c clang::Sema::ActOnStartOfFunctionDef(clang::Scope*, clang::Declarator&amp;, llvm::MutableArrayRef&lt;clang::TemplateParameterList*&gt;, clang::SkipBodyInfo*, clang::Sema::FnBodyKind) (/usr/local/bin/clang-21+0x623993c)
#<!-- -->10 0x0000564cf1e41db1 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/usr/local/bin/clang-21+0x5e6ddb1)
#<!-- -->11 0x0000564cf1e8bf05 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/usr/local/bin/clang-21+0x5eb7f05)
#<!-- -->12 0x0000564cf1e3a62d clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/usr/local/bin/clang-21+0x5e6662d)
#<!-- -->13 0x0000564cf1e3b16e clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/usr/local/bin/clang-21+0x5e6716e)
#<!-- -->14 0x0000564cf1e44d73 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/usr/local/bin/clang-21+0x5e70d73)
#<!-- -->15 0x0000564cf1e466a8 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/usr/local/bin/clang-21+0x5e726a8)
#<!-- -->16 0x0000564cf1e3462a clang::ParseAST(clang::Sema&amp;, bool, bool) (/usr/local/bin/clang-21+0x5e6062a)
#<!-- -->17 0x0000564cf05a2e45 clang::CodeGenAction::ExecuteAction() (/usr/local/bin/clang-21+0x45cee45)
#<!-- -->18 0x0000564cf08879b8 clang::FrontendAction::Execute() (/usr/local/bin/clang-21+0x48b39b8)
#<!-- -->19 0x0000564cf0805d3b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/usr/local/bin/clang-21+0x4831d3b)
#<!-- -->20 0x0000564cf0979c7b clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/local/bin/clang-21+0x49a5c7b)
#<!-- -->21 0x0000564cece76f51 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/usr/local/bin/clang-21+0xea2f51)
#<!-- -->22 0x0000564cece6f5fd ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->23 0x0000564cece7299f clang_main(int, char**, llvm::ToolContext const&amp;) (/usr/local/bin/clang-21+0xe9e99f)
#<!-- -->24 0x0000564cecd4530b main (/usr/local/bin/clang-21+0xd7130b)
#<!-- -->25 0x00007fd7c710d083 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x24083)
#<!-- -->26 0x0000564cece6f07e _start (/usr/local/bin/clang-21+0xe9b07e)
```

## Program

```
#include &lt;stdlib.h&gt;
#include &lt;stdint.h&gt;
typedef struct {
    void *p;
    size_t n;
} ptn_p;
void *__attribute__((alloc_size(2, 1))) malloc_x(size_t n,...) {
}
```

## To quickly reproduce

https://gcc.godbolt.org/z/WEqaqcGMb
</details>


---

### Comment 2 - shafik

Looks fixed in trunk: https://gcc.godbolt.org/z/8Mx8GoT1z

---

