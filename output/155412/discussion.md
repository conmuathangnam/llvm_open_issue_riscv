# clang crashed with vector_size attribute. Assertion `Chunk.Kind == DeclaratorChunk::MemberPointer' failed.

**Author:** iamanonymouscs
**URL:** https://github.com/llvm/llvm-project/issues/155412

## Body

clang++-20 crashed with ```vector_size``` attribute, also crashed on trunk.

Compiler explorer (assertion trunk): https://godbolt.org/z/Mnqrnj4Ts


$ cat mutant.cpp
```cpp
template<typename T> struct a {int (T::*b) __attribute__((vector_size(4))); };
```

```
$ clang++-20 mutant.cpp
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /usr/lib/llvm-20/bin/clang -cc1 -triple x86_64-pc-linux-gnu -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name mutant.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/home/code/reduce -fcoverage-compilation-dir=/home/code/reduce -resource-dir /usr/lib/llvm-20/lib/clang/20 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/x86_64-linux-gnu/c++/11 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/backward -internal-isystem /usr/lib/llvm-20/lib/clang/20/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -fdeprecated-macro -ferror-limit 19 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/mutant-84f7b2.o -x c++ mutant.cpp
1.      mutant.cpp:1:75: current parser token ';'
2.      mutant.cpp:1:22: parsing struct/union/class body 'a'
 #0 0x00007fd745441816 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/usr/lib/llvm-20/bin/../lib/libLLVM.so.20.1+0x101f816)
 #1 0x00007fd74543f410 llvm::sys::RunSignalHandlers() (/usr/lib/llvm-20/bin/../lib/libLLVM.so.20.1+0x101d410)
 #2 0x00007fd745441ecb (/usr/lib/llvm-20/bin/../lib/libLLVM.so.20.1+0x101fecb)
 #3 0x00007fd743eff520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007fd74406bdcd (/lib/x86_64-linux-gnu/libc.so.6+0x1aedcd)
 #5 0x00007fd74e173d86 clang::NestedNameSpecifierLocBuilder::getWithLocInContext(clang::ASTContext&) const (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x1389d86)
 #6 0x00007fd74e48dde1 clang::CXXScopeSpec::getWithLocInContext(clang::ASTContext&) const (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x16a3de1)
 #7 0x00007fd74ec08bbc (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x1e1ebbc)
 #8 0x00007fd74ebffd55 clang::Sema::GetTypeForDeclarator(clang::Declarator&) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x1e15d55)
 #9 0x00007fd74e640861 clang::Sema::HandleField(clang::Scope*, clang::RecordDecl*, clang::SourceLocation, clang::Declarator&, clang::Expr*, clang::InClassInitStyle, clang::AccessSpecifier) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x1856861)
#10 0x00007fd74e6912b6 clang::Sema::ActOnCXXMemberDeclarator(clang::Scope*, clang::AccessSpecifier, clang::Declarator&, llvm::MutableArrayRef<clang::TemplateParameterList*>, clang::Expr*, clang::VirtSpecifiers const&, clang::InClassInitStyle) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x18a72b6)
#11 0x00007fd74db96095 clang::Parser::ParseCXXClassMemberDeclaration(clang::AccessSpecifier, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::ParsingDeclRAIIObject*) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xdac095)
#12 0x00007fd74db97e43 clang::Parser::ParseCXXClassMemberDeclarationWithPragmas(clang::AccessSpecifier&, clang::ParsedAttributes&, clang::TypeSpecifierType, clang::Decl*) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xdade43)
#13 0x00007fd74db92e53 clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&, unsigned int, clang::Decl*) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xda8e53)
#14 0x00007fd74db90e6d clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xda6e6d)
#15 0x00007fd74db74563 clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xd8a563)
#16 0x00007fd74dc066d6 clang::Parser::ParseDeclarationAfterTemplate(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo&, clang::ParsingDeclRAIIObject&, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xe1c6d6)
#17 0x00007fd74dc05cd5 clang::Parser::ParseTemplateDeclarationOrSpecialization(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xe1bcd5)
#18 0x00007fd74dc05601 clang::Parser::ParseDeclarationStartingWithTemplate(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xe1b601)
#19 0x00007fd74db6eeb9 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xd84eb9)
#20 0x00007fd74dc1479f clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xe2a79f)
#21 0x00007fd74dc13717 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xe29717)
#22 0x00007fd74dc1318f clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xe2918f)
#23 0x00007fd74db5bf1a clang::ParseAST(clang::Sema&, bool, bool) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xd71f1a)
#24 0x00007fd74fa7a2cb clang::FrontendAction::Execute() (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x2c902cb)
#25 0x00007fd74f9eeea4 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x2c04ea4)
#26 0x00007fd74fafda0e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x2d13a0e)
#27 0x000055cf43cee0f3 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/usr/lib/llvm-20/bin/clang+0x120f3)
#28 0x000055cf43ceaed5 (/usr/lib/llvm-20/bin/clang+0xeed5)
#29 0x000055cf43cea2de clang_main(int, char**, llvm::ToolContext const&) (/usr/lib/llvm-20/bin/clang+0xe2de)
#30 0x000055cf43cf7716 main (/usr/lib/llvm-20/bin/clang+0x1b716)
#31 0x00007fd743ee6d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#32 0x00007fd743ee6e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#33 0x000055cf43ce8d75 _start (/usr/lib/llvm-20/bin/clang+0xcd75)
clang++-20: error: unable to execute command: Segmentation fault (core dumped)
clang++-20: error: clang frontend command failed due to signal (use -v to see invocation)
Ubuntu clang version 20.1.8 (++20250708123929+0de59a293f7a-1~exp1~20250708003944.130)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/lib/llvm-20/bin
clang++-20: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++-20: note: diagnostic msg: /tmp/mutant-2bc6a4.cpp
clang++-20: note: diagnostic msg: /tmp/mutant-2bc6a4.sh
clang++-20: note: diagnostic msg: 

********************
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Anonymous (iamanonymouscs)

<details>
clang++-20 crashed with ```vector_size``` attribute, also crashed on trunk.

Compiler explorer (assertion trunk): https://godbolt.org/z/Mnqrnj4Ts

```
$ cat mutant.cpp
template&lt;typename T&gt; struct a {int (T::*b) __attribute__((vector_size(4))); };
```

```
$ clang++-20 mutant.cpp
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /usr/lib/llvm-20/bin/clang -cc1 -triple x86_64-pc-linux-gnu -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name mutant.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/home/code/reduce -fcoverage-compilation-dir=/home/code/reduce -resource-dir /usr/lib/llvm-20/lib/clang/20 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/x86_64-linux-gnu/c++/11 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/backward -internal-isystem /usr/lib/llvm-20/lib/clang/20/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -fdeprecated-macro -ferror-limit 19 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/mutant-84f7b2.o -x c++ mutant.cpp
1.      mutant.cpp:1:75: current parser token ';'
2.      mutant.cpp:1:22: parsing struct/union/class body 'a'
 #<!-- -->0 0x00007fd745441816 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/usr/lib/llvm-20/bin/../lib/libLLVM.so.20.1+0x101f816)
 #<!-- -->1 0x00007fd74543f410 llvm::sys::RunSignalHandlers() (/usr/lib/llvm-20/bin/../lib/libLLVM.so.20.1+0x101d410)
 #<!-- -->2 0x00007fd745441ecb (/usr/lib/llvm-20/bin/../lib/libLLVM.so.20.1+0x101fecb)
 #<!-- -->3 0x00007fd743eff520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007fd74406bdcd (/lib/x86_64-linux-gnu/libc.so.6+0x1aedcd)
 #<!-- -->5 0x00007fd74e173d86 clang::NestedNameSpecifierLocBuilder::getWithLocInContext(clang::ASTContext&amp;) const (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x1389d86)
 #<!-- -->6 0x00007fd74e48dde1 clang::CXXScopeSpec::getWithLocInContext(clang::ASTContext&amp;) const (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x16a3de1)
 #<!-- -->7 0x00007fd74ec08bbc (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x1e1ebbc)
 #<!-- -->8 0x00007fd74ebffd55 clang::Sema::GetTypeForDeclarator(clang::Declarator&amp;) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x1e15d55)
 #<!-- -->9 0x00007fd74e640861 clang::Sema::HandleField(clang::Scope*, clang::RecordDecl*, clang::SourceLocation, clang::Declarator&amp;, clang::Expr*, clang::InClassInitStyle, clang::AccessSpecifier) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x1856861)
#<!-- -->10 0x00007fd74e6912b6 clang::Sema::ActOnCXXMemberDeclarator(clang::Scope*, clang::AccessSpecifier, clang::Declarator&amp;, llvm::MutableArrayRef&lt;clang::TemplateParameterList*&gt;, clang::Expr*, clang::VirtSpecifiers const&amp;, clang::InClassInitStyle) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x18a72b6)
#<!-- -->11 0x00007fd74db96095 clang::Parser::ParseCXXClassMemberDeclaration(clang::AccessSpecifier, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::ParsingDeclRAIIObject*) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xdac095)
#<!-- -->12 0x00007fd74db97e43 clang::Parser::ParseCXXClassMemberDeclarationWithPragmas(clang::AccessSpecifier&amp;, clang::ParsedAttributes&amp;, clang::TypeSpecifierType, clang::Decl*) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xdade43)
#<!-- -->13 0x00007fd74db92e53 clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&amp;, unsigned int, clang::Decl*) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xda8e53)
#<!-- -->14 0x00007fd74db90e6d clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&amp;) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xda6e6d)
#<!-- -->15 0x00007fd74db74563 clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xd8a563)
#<!-- -->16 0x00007fd74dc066d6 clang::Parser::ParseDeclarationAfterTemplate(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo&amp;, clang::ParsingDeclRAIIObject&amp;, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::AccessSpecifier) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xe1c6d6)
#<!-- -->17 0x00007fd74dc05cd5 clang::Parser::ParseTemplateDeclarationOrSpecialization(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::AccessSpecifier) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xe1bcd5)
#<!-- -->18 0x00007fd74dc05601 clang::Parser::ParseDeclarationStartingWithTemplate(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xe1b601)
#<!-- -->19 0x00007fd74db6eeb9 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xd84eb9)
#<!-- -->20 0x00007fd74dc1479f clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xe2a79f)
#<!-- -->21 0x00007fd74dc13717 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xe29717)
#<!-- -->22 0x00007fd74dc1318f clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xe2918f)
#<!-- -->23 0x00007fd74db5bf1a clang::ParseAST(clang::Sema&amp;, bool, bool) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xd71f1a)
#<!-- -->24 0x00007fd74fa7a2cb clang::FrontendAction::Execute() (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x2c902cb)
#<!-- -->25 0x00007fd74f9eeea4 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x2c04ea4)
#<!-- -->26 0x00007fd74fafda0e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x2d13a0e)
#<!-- -->27 0x000055cf43cee0f3 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/usr/lib/llvm-20/bin/clang+0x120f3)
#<!-- -->28 0x000055cf43ceaed5 (/usr/lib/llvm-20/bin/clang+0xeed5)
#<!-- -->29 0x000055cf43cea2de clang_main(int, char**, llvm::ToolContext const&amp;) (/usr/lib/llvm-20/bin/clang+0xe2de)
#<!-- -->30 0x000055cf43cf7716 main (/usr/lib/llvm-20/bin/clang+0x1b716)
#<!-- -->31 0x00007fd743ee6d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->32 0x00007fd743ee6e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->33 0x000055cf43ce8d75 _start (/usr/lib/llvm-20/bin/clang+0xcd75)
clang++-20: error: unable to execute command: Segmentation fault (core dumped)
clang++-20: error: clang frontend command failed due to signal (use -v to see invocation)
Ubuntu clang version 20.1.8 (++20250708123929+0de59a293f7a-1~exp1~20250708003944.130)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/lib/llvm-20/bin
clang++-20: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++-20: note: diagnostic msg: /tmp/mutant-2bc6a4.cpp
clang++-20: note: diagnostic msg: /tmp/mutant-2bc6a4.sh
clang++-20: note: diagnostic msg: 

********************
```
</details>


---

### Comment 2 - shafik

Assuming by the file name `mutant.cpp` this is a fuzzing result. Please make sure before submitting fuzzer based reports that you look for duplicates, report how far back this problems happens in clang, please report assertions or unreachable messages as well. 

Also code should be in it's own text box w/o any command line stuff to make copy paste more useful.

---

