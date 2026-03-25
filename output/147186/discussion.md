# [clang] ICE on invalid inheritance since version 6

**Author:** renehsz
**URL:** https://github.com/llvm/llvm-project/issues/147186
**Status:** Closed
**Labels:** clang:frontend, crash-on-invalid, confirmed
**Closed Date:** 2025-07-10T11:31:42Z

## Body

The code below causes ICE on Clang since version 6. (Diagnostic compilation errors are expected, but not a compiler crash.)
```c++
class a : ;
class b : ;
struct c : virtual a;
template <typename d>
class e : c, d 
using b::b;
```
Stack trace:
```
0.	Program arguments: /usr/bin/clang++ -cc1 -triple x86_64-pc-linux-gnu -emit-obj -dumpdir code.o- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name code.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/home/r/projects/clang_bug -fcoverage-compilation-dir=/home/r/projects/clang_bug -resource-dir /usr/lib/clang/20 -internal-isystem /usr/bin/../lib64/gcc/x86_64-pc-linux-gnu/15.1.1/../../../../include/c++/15.1.1 -internal-isystem /usr/bin/../lib64/gcc/x86_64-pc-linux-gnu/15.1.1/../../../../include/c++/15.1.1/x86_64-pc-linux-gnu -internal-isystem /usr/bin/../lib64/gcc/x86_64-pc-linux-gnu/15.1.1/../../../../include/c++/15.1.1/backward -internal-isystem /usr/lib/clang/20/include -internal-isystem /usr/local/include -internal-isystem /usr/bin/../lib64/gcc/x86_64-pc-linux-gnu/15.1.1/../../../../x86_64-pc-linux-gnu/include -internal-externc-isystem /include -internal-externc-isystem /usr/include -fdeprecated-macro -ferror-limit 19 -stack-protector 2 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/code-126fe3.o -x c++ code.cpp
1.	code.cpp:6:11: current parser token ';'
2.	code.cpp:5:1: parsing struct/union/class body 'e'
 #0 0x00007fa71d2e09ce llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/usr/lib/libLLVM.so.20.1+0x8e09ce)
 #1 0x00007fa71d2de047 (/usr/lib/libLLVM.so.20.1+0x8de047)
 #2 0x00007fa71c44f600 (/usr/lib/libc.so.6+0x3d600)
 #3 0x00007fa72764fbe5 clang::Sema::BuildUsingShadowDecl(clang::Scope*, clang::BaseUsingDecl*, clang::NamedDecl*, clang::UsingShadowDecl*) (/usr/lib/libclang-cpp.so.20.1+0x144fbe5)
 #4 0x00007fa727654254 clang::Sema::BuildUsingDeclaration(clang::Scope*, clang::AccessSpecifier, clang::SourceLocation, bool, clang::SourceLocation, clang::CXXScopeSpec&, clang::DeclarationNameInfo, clang::SourceLocation, clang::ParsedAttributesView const&, bool, bool) (/usr/lib/libclang-cpp.so.20.1+0x1454254)
 #5 0x00007fa727654a5c clang::Sema::ActOnUsingDeclaration(clang::Scope*, clang::AccessSpecifier, clang::SourceLocation, clang::SourceLocation, clang::CXXScopeSpec&, clang::UnqualifiedId&, clang::SourceLocation, clang::ParsedAttributesView const&) (/usr/lib/libclang-cpp.so.20.1+0x1454a5c)
 #6 0x00007fa72695159e clang::Parser::ParseUsingDeclaration(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo const&, clang::SourceLocation, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (/usr/lib/libclang-cpp.so.20.1+0x75159e)
 #7 0x00007fa72692a9d1 clang::Parser::ParseCXXClassMemberDeclaration(clang::AccessSpecifier, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::ParsingDeclRAIIObject*) (/usr/lib/libclang-cpp.so.20.1+0x72a9d1)
 #8 0x00007fa72692d920 clang::Parser::ParseCXXClassMemberDeclarationWithPragmas(clang::AccessSpecifier&, clang::ParsedAttributes&, clang::TypeSpecifierType, clang::Decl*) (/usr/lib/libclang-cpp.so.20.1+0x72d920)
 #9 0x00007fa72692e081 clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&, unsigned int, clang::Decl*) (/usr/lib/libclang-cpp.so.20.1+0x72e081)
#10 0x00007fa7269305fd clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&) (/usr/lib/libclang-cpp.so.20.1+0x7305fd)
#11 0x00007fa7269321c5 clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/usr/lib/libclang-cpp.so.20.1+0x7321c5)
#12 0x00007fa7269de5a6 clang::Parser::ParseDeclarationAfterTemplate(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo&, clang::ParsingDeclRAIIObject&, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (/usr/lib/libclang-cpp.so.20.1+0x7de5a6)
#13 0x00007fa7269ecf81 clang::Parser::ParseTemplateDeclarationOrSpecialization(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (/usr/lib/libclang-cpp.so.20.1+0x7ecf81)
#14 0x00007fa7269ed23b clang::Parser::ParseDeclarationStartingWithTemplate(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&) (/usr/lib/libclang-cpp.so.20.1+0x7ed23b)
#15 0x00007fa72695b207 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/usr/lib/libclang-cpp.so.20.1+0x75b207)
#16 0x00007fa7269f4ea2 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/usr/lib/libclang-cpp.so.20.1+0x7f4ea2)
#17 0x00007fa7269f58eb clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/usr/lib/libclang-cpp.so.20.1+0x7f58eb)
#18 0x00007fa7268fe36b clang::ParseAST(clang::Sema&, bool, bool) (/usr/lib/libclang-cpp.so.20.1+0x6fe36b)
#19 0x00007fa728a0f8b8 clang::FrontendAction::Execute() (/usr/lib/libclang-cpp.so.20.1+0x280f8b8)
#20 0x00007fa728998897 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/usr/lib/libclang-cpp.so.20.1+0x2798897)
#21 0x00007fa728a760d0 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/lib/libclang-cpp.so.20.1+0x28760d0)
#22 0x0000561cb535d625 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/usr/bin/clang+++0x14625)
#23 0x0000561cb5361568 (/usr/bin/clang+++0x18568)
#24 0x0000561cb5362fa7 clang_main(int, char**, llvm::ToolContext const&) (/usr/bin/clang+++0x19fa7)
#25 0x0000561cb53541c6 main (/usr/bin/clang+++0xb1c6)
#26 0x00007fa71c43952e __libc_start_call_main /usr/src/debug/glibc/glibc/csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#27 0x00007fa71c4395ea call_init /usr/src/debug/glibc/glibc/csu/../csu/libc-start.c:128:20
#28 0x00007fa71c4395ea __libc_start_main /usr/src/debug/glibc/glibc/csu/../csu/libc-start.c:347:5
#29 0x0000561cb5354225 _start (/usr/bin/clang+++0xb225)
clang++: error: unable to execute command: Segmentation fault
```

Run script: [code-22a624.sh](https://github.com/user-attachments/files/21088705/code-22a624.sh.txt)

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: René Hickersberger (renehsz)

<details>
The code below causes ICE on Clang since version 6. (Diagnostic compilation errors are expected, but not a compiler crash.)
```c++
class a : ;
class b : ;
struct c : virtual a;
template &lt;typename d&gt;
class e : c, d 
using b::b;
```
Stack trace:
```
0.	Program arguments: /usr/bin/clang++ -cc1 -triple x86_64-pc-linux-gnu -emit-obj -dumpdir code.o- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name code.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/home/r/projects/clang_bug -fcoverage-compilation-dir=/home/r/projects/clang_bug -resource-dir /usr/lib/clang/20 -internal-isystem /usr/bin/../lib64/gcc/x86_64-pc-linux-gnu/15.1.1/../../../../include/c++/15.1.1 -internal-isystem /usr/bin/../lib64/gcc/x86_64-pc-linux-gnu/15.1.1/../../../../include/c++/15.1.1/x86_64-pc-linux-gnu -internal-isystem /usr/bin/../lib64/gcc/x86_64-pc-linux-gnu/15.1.1/../../../../include/c++/15.1.1/backward -internal-isystem /usr/lib/clang/20/include -internal-isystem /usr/local/include -internal-isystem /usr/bin/../lib64/gcc/x86_64-pc-linux-gnu/15.1.1/../../../../x86_64-pc-linux-gnu/include -internal-externc-isystem /include -internal-externc-isystem /usr/include -fdeprecated-macro -ferror-limit 19 -stack-protector 2 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/code-126fe3.o -x c++ code.cpp
1.	code.cpp:6:11: current parser token ';'
2.	code.cpp:5:1: parsing struct/union/class body 'e'
 #<!-- -->0 0x00007fa71d2e09ce llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/usr/lib/libLLVM.so.20.1+0x8e09ce)
 #<!-- -->1 0x00007fa71d2de047 (/usr/lib/libLLVM.so.20.1+0x8de047)
 #<!-- -->2 0x00007fa71c44f600 (/usr/lib/libc.so.6+0x3d600)
 #<!-- -->3 0x00007fa72764fbe5 clang::Sema::BuildUsingShadowDecl(clang::Scope*, clang::BaseUsingDecl*, clang::NamedDecl*, clang::UsingShadowDecl*) (/usr/lib/libclang-cpp.so.20.1+0x144fbe5)
 #<!-- -->4 0x00007fa727654254 clang::Sema::BuildUsingDeclaration(clang::Scope*, clang::AccessSpecifier, clang::SourceLocation, bool, clang::SourceLocation, clang::CXXScopeSpec&amp;, clang::DeclarationNameInfo, clang::SourceLocation, clang::ParsedAttributesView const&amp;, bool, bool) (/usr/lib/libclang-cpp.so.20.1+0x1454254)
 #<!-- -->5 0x00007fa727654a5c clang::Sema::ActOnUsingDeclaration(clang::Scope*, clang::AccessSpecifier, clang::SourceLocation, clang::SourceLocation, clang::CXXScopeSpec&amp;, clang::UnqualifiedId&amp;, clang::SourceLocation, clang::ParsedAttributesView const&amp;) (/usr/lib/libclang-cpp.so.20.1+0x1454a5c)
 #<!-- -->6 0x00007fa72695159e clang::Parser::ParseUsingDeclaration(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo const&amp;, clang::SourceLocation, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::AccessSpecifier) (/usr/lib/libclang-cpp.so.20.1+0x75159e)
 #<!-- -->7 0x00007fa72692a9d1 clang::Parser::ParseCXXClassMemberDeclaration(clang::AccessSpecifier, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::ParsingDeclRAIIObject*) (/usr/lib/libclang-cpp.so.20.1+0x72a9d1)
 #<!-- -->8 0x00007fa72692d920 clang::Parser::ParseCXXClassMemberDeclarationWithPragmas(clang::AccessSpecifier&amp;, clang::ParsedAttributes&amp;, clang::TypeSpecifierType, clang::Decl*) (/usr/lib/libclang-cpp.so.20.1+0x72d920)
 #<!-- -->9 0x00007fa72692e081 clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&amp;, unsigned int, clang::Decl*) (/usr/lib/libclang-cpp.so.20.1+0x72e081)
#<!-- -->10 0x00007fa7269305fd clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&amp;) (/usr/lib/libclang-cpp.so.20.1+0x7305fd)
#<!-- -->11 0x00007fa7269321c5 clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/usr/lib/libclang-cpp.so.20.1+0x7321c5)
#<!-- -->12 0x00007fa7269de5a6 clang::Parser::ParseDeclarationAfterTemplate(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo&amp;, clang::ParsingDeclRAIIObject&amp;, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::AccessSpecifier) (/usr/lib/libclang-cpp.so.20.1+0x7de5a6)
#<!-- -->13 0x00007fa7269ecf81 clang::Parser::ParseTemplateDeclarationOrSpecialization(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::AccessSpecifier) (/usr/lib/libclang-cpp.so.20.1+0x7ecf81)
#<!-- -->14 0x00007fa7269ed23b clang::Parser::ParseDeclarationStartingWithTemplate(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;) (/usr/lib/libclang-cpp.so.20.1+0x7ed23b)
#<!-- -->15 0x00007fa72695b207 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/usr/lib/libclang-cpp.so.20.1+0x75b207)
#<!-- -->16 0x00007fa7269f4ea2 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/usr/lib/libclang-cpp.so.20.1+0x7f4ea2)
#<!-- -->17 0x00007fa7269f58eb clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/usr/lib/libclang-cpp.so.20.1+0x7f58eb)
#<!-- -->18 0x00007fa7268fe36b clang::ParseAST(clang::Sema&amp;, bool, bool) (/usr/lib/libclang-cpp.so.20.1+0x6fe36b)
#<!-- -->19 0x00007fa728a0f8b8 clang::FrontendAction::Execute() (/usr/lib/libclang-cpp.so.20.1+0x280f8b8)
#<!-- -->20 0x00007fa728998897 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/usr/lib/libclang-cpp.so.20.1+0x2798897)
#<!-- -->21 0x00007fa728a760d0 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/lib/libclang-cpp.so.20.1+0x28760d0)
#<!-- -->22 0x0000561cb535d625 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/usr/bin/clang+++0x14625)
#<!-- -->23 0x0000561cb5361568 (/usr/bin/clang+++0x18568)
#<!-- -->24 0x0000561cb5362fa7 clang_main(int, char**, llvm::ToolContext const&amp;) (/usr/bin/clang+++0x19fa7)
#<!-- -->25 0x0000561cb53541c6 main (/usr/bin/clang+++0xb1c6)
#<!-- -->26 0x00007fa71c43952e __libc_start_call_main /usr/src/debug/glibc/glibc/csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->27 0x00007fa71c4395ea call_init /usr/src/debug/glibc/glibc/csu/../csu/libc-start.c:128:20
#<!-- -->28 0x00007fa71c4395ea __libc_start_main /usr/src/debug/glibc/glibc/csu/../csu/libc-start.c:347:5
#<!-- -->29 0x0000561cb5354225 _start (/usr/bin/clang+++0xb225)
clang++: error: unable to execute command: Segmentation fault
```

Run script: [code-22a624.sh](https://github.com/user-attachments/files/21088705/code-22a624.sh.txt)
</details>


---

### Comment 2 - EugeneZelenko

Crashed in `main`: https://godbolt.org/z/6d6j7jshq

---

### Comment 3 - shashforge

Hello, I’d like to take this on. Could you please assign it to me?

---

