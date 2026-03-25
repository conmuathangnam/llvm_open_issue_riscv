# clang crashed with transparent_union attribute on union at -O1 and above. Assertion `!T->isDependentType() && "should not see dependent types here"' failed.

**Author:** iamanonymouscs
**URL:** https://github.com/llvm/llvm-project/issues/155417

## Body

clang++-20 crashed with ```transparent_union``` attribute on ```union``` at ```-O1``` and above, also crashed on trunk.

Compiler explorer (assertion trunk): https://godbolt.org/z/MW6sd7Wz9

```
$ cat mutant.cpp
template <typename T> struct a { 
    union __attribute__((transparent_union)) { 
        T b; 
    }; 
}
```

```
$ clang++-20 -O1 mutant.cpp
mutant.cpp:2:26: warning: 'transparent_union' attribute ignored [-Wignored-attributes]
    2 |     union __attribute__((transparent_union)) { 
      |                          ^
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /usr/lib/llvm-20/bin/clang -cc1 -triple x86_64-pc-linux-gnu -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name mutant.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=none -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/home/code/reduce -fcoverage-compilation-dir=/home/code/reduce -resource-dir /usr/lib/llvm-20/lib/clang/20 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/x86_64-linux-gnu/c++/11 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/backward -internal-isystem /usr/lib/llvm-20/lib/clang/20/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -O1 -fdeprecated-macro -ferror-limit 19 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/mutant-107cbb.o -x c++ mutant.cpp
1.      mutant.cpp:4:6: current parser token ';'
2.      mutant.cpp:1:23: parsing struct/union/class body 'a'
 #0 0x00007f30597af816 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/usr/lib/llvm-20/bin/../lib/libLLVM.so.20.1+0x101f816)
 #1 0x00007f30597ad410 llvm::sys::RunSignalHandlers() (/usr/lib/llvm-20/bin/../lib/libLLVM.so.20.1+0x101d410)
 #2 0x00007f30597afecb (/usr/lib/llvm-20/bin/../lib/libLLVM.so.20.1+0x101fecb)
 #3 0x00007f305826d520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007f3061fa6670 clang::ASTContext::getTypeInfoImpl(clang::Type const*) const (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xe4e670)
 #5 0x00007f3061fa81bc clang::ASTContext::getTypeInfo(clang::Type const*) const (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xe501bc)
 #6 0x00007f30629d7d00 (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x187fd00)
 #7 0x00007f30629d7a3b clang::Sema::ProcessDeclAttributeDelayed(clang::Decl*, clang::ParsedAttributesView const&) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x187fa3b)
 #8 0x00007f3061efeebf clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xda6ebf)
 #9 0x00007f3061ee2563 clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xd8a563)
#10 0x00007f3061f03493 clang::Parser::ParseCXXClassMemberDeclaration(clang::AccessSpecifier, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::ParsingDeclRAIIObject*) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xdab493)
#11 0x00007f3061f05e43 clang::Parser::ParseCXXClassMemberDeclarationWithPragmas(clang::AccessSpecifier&, clang::ParsedAttributes&, clang::TypeSpecifierType, clang::Decl*) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xdade43)
#12 0x00007f3061f00e53 clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&, unsigned int, clang::Decl*) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xda8e53)
#13 0x00007f3061efee6d clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xda6e6d)
#14 0x00007f3061ee2563 clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xd8a563)
#15 0x00007f3061f746d6 clang::Parser::ParseDeclarationAfterTemplate(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo&, clang::ParsingDeclRAIIObject&, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xe1c6d6)
#16 0x00007f3061f73cd5 clang::Parser::ParseTemplateDeclarationOrSpecialization(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xe1bcd5)
#17 0x00007f3061f73601 clang::Parser::ParseDeclarationStartingWithTemplate(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xe1b601)
#18 0x00007f3061edceb9 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xd84eb9)
#19 0x00007f3061f8279f clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xe2a79f)
#20 0x00007f3061f81717 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xe29717)
#21 0x00007f3061f8118f clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xe2918f)
#22 0x00007f3061ec9f1a clang::ParseAST(clang::Sema&, bool, bool) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xd71f1a)
#23 0x00007f3063de82cb clang::FrontendAction::Execute() (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x2c902cb)
#24 0x00007f3063d5cea4 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x2c04ea4)
#25 0x00007f3063e6ba0e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x2d13a0e)
#26 0x0000561ca75030f3 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/usr/lib/llvm-20/bin/clang+0x120f3)
#27 0x0000561ca74ffed5 (/usr/lib/llvm-20/bin/clang+0xeed5)
#28 0x0000561ca74ff2de clang_main(int, char**, llvm::ToolContext const&) (/usr/lib/llvm-20/bin/clang+0xe2de)
#29 0x0000561ca750c716 main (/usr/lib/llvm-20/bin/clang+0x1b716)
#30 0x00007f3058254d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#31 0x00007f3058254e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#32 0x0000561ca74fdd75 _start (/usr/lib/llvm-20/bin/clang+0xcd75)
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
clang++-20: note: diagnostic msg: /tmp/mutant-c9dddd.cpp
clang++-20: note: diagnostic msg: /tmp/mutant-c9dddd.sh
clang++-20: note: diagnostic msg: 

********************
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Anonymous (iamanonymouscs)

<details>
clang++-20 crashed with ```transparent_union``` attribute on ```union``` at ```-O1``` and above, also crashed on trunk.

Compiler explorer (assertion trunk): https://godbolt.org/z/MW6sd7Wz9

```
$ cat mutant.cpp
template &lt;typename T&gt; struct a { 
    union __attribute__((transparent_union)) { 
        T b; 
    }; 
}
```

```
$ clang++-20 -O1 mutant.cpp
mutant.cpp:2:26: warning: 'transparent_union' attribute ignored [-Wignored-attributes]
    2 |     union __attribute__((transparent_union)) { 
      |                          ^
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /usr/lib/llvm-20/bin/clang -cc1 -triple x86_64-pc-linux-gnu -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name mutant.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=none -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/home/code/reduce -fcoverage-compilation-dir=/home/code/reduce -resource-dir /usr/lib/llvm-20/lib/clang/20 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/x86_64-linux-gnu/c++/11 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/backward -internal-isystem /usr/lib/llvm-20/lib/clang/20/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -O1 -fdeprecated-macro -ferror-limit 19 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/mutant-107cbb.o -x c++ mutant.cpp
1.      mutant.cpp:4:6: current parser token ';'
2.      mutant.cpp:1:23: parsing struct/union/class body 'a'
 #<!-- -->0 0x00007f30597af816 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/usr/lib/llvm-20/bin/../lib/libLLVM.so.20.1+0x101f816)
 #<!-- -->1 0x00007f30597ad410 llvm::sys::RunSignalHandlers() (/usr/lib/llvm-20/bin/../lib/libLLVM.so.20.1+0x101d410)
 #<!-- -->2 0x00007f30597afecb (/usr/lib/llvm-20/bin/../lib/libLLVM.so.20.1+0x101fecb)
 #<!-- -->3 0x00007f305826d520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007f3061fa6670 clang::ASTContext::getTypeInfoImpl(clang::Type const*) const (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xe4e670)
 #<!-- -->5 0x00007f3061fa81bc clang::ASTContext::getTypeInfo(clang::Type const*) const (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xe501bc)
 #<!-- -->6 0x00007f30629d7d00 (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x187fd00)
 #<!-- -->7 0x00007f30629d7a3b clang::Sema::ProcessDeclAttributeDelayed(clang::Decl*, clang::ParsedAttributesView const&amp;) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x187fa3b)
 #<!-- -->8 0x00007f3061efeebf clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&amp;) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xda6ebf)
 #<!-- -->9 0x00007f3061ee2563 clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xd8a563)
#<!-- -->10 0x00007f3061f03493 clang::Parser::ParseCXXClassMemberDeclaration(clang::AccessSpecifier, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::ParsingDeclRAIIObject*) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xdab493)
#<!-- -->11 0x00007f3061f05e43 clang::Parser::ParseCXXClassMemberDeclarationWithPragmas(clang::AccessSpecifier&amp;, clang::ParsedAttributes&amp;, clang::TypeSpecifierType, clang::Decl*) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xdade43)
#<!-- -->12 0x00007f3061f00e53 clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&amp;, unsigned int, clang::Decl*) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xda8e53)
#<!-- -->13 0x00007f3061efee6d clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&amp;) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xda6e6d)
#<!-- -->14 0x00007f3061ee2563 clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xd8a563)
#<!-- -->15 0x00007f3061f746d6 clang::Parser::ParseDeclarationAfterTemplate(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo&amp;, clang::ParsingDeclRAIIObject&amp;, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::AccessSpecifier) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xe1c6d6)
#<!-- -->16 0x00007f3061f73cd5 clang::Parser::ParseTemplateDeclarationOrSpecialization(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::AccessSpecifier) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xe1bcd5)
#<!-- -->17 0x00007f3061f73601 clang::Parser::ParseDeclarationStartingWithTemplate(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xe1b601)
#<!-- -->18 0x00007f3061edceb9 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xd84eb9)
#<!-- -->19 0x00007f3061f8279f clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xe2a79f)
#<!-- -->20 0x00007f3061f81717 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xe29717)
#<!-- -->21 0x00007f3061f8118f clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xe2918f)
#<!-- -->22 0x00007f3061ec9f1a clang::ParseAST(clang::Sema&amp;, bool, bool) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xd71f1a)
#<!-- -->23 0x00007f3063de82cb clang::FrontendAction::Execute() (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x2c902cb)
#<!-- -->24 0x00007f3063d5cea4 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x2c04ea4)
#<!-- -->25 0x00007f3063e6ba0e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x2d13a0e)
#<!-- -->26 0x0000561ca75030f3 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/usr/lib/llvm-20/bin/clang+0x120f3)
#<!-- -->27 0x0000561ca74ffed5 (/usr/lib/llvm-20/bin/clang+0xeed5)
#<!-- -->28 0x0000561ca74ff2de clang_main(int, char**, llvm::ToolContext const&amp;) (/usr/lib/llvm-20/bin/clang+0xe2de)
#<!-- -->29 0x0000561ca750c716 main (/usr/lib/llvm-20/bin/clang+0x1b716)
#<!-- -->30 0x00007f3058254d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->31 0x00007f3058254e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->32 0x0000561ca74fdd75 _start (/usr/lib/llvm-20/bin/clang+0xcd75)
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
clang++-20: note: diagnostic msg: /tmp/mutant-c9dddd.cpp
clang++-20: note: diagnostic msg: /tmp/mutant-c9dddd.sh
clang++-20: note: diagnostic msg: 

********************
```
</details>


---

