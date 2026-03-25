# Assert in `DeduceTemplateSpecializationFromInitializer`

**Author:** vogelsgesang
**URL:** https://github.com/llvm/llvm-project/issues/176013

## Body

The source code

```
namespace x {
template <int> struct myType {
  static constexpr x::myType variable {};
};
}
```

with `-std=c++20` crashes clang on trunk (as of commit b8d5ef9d2e197234da197986b606b6753e363f24)
Started with clang 19 (https://godbolt.org/z/E63qT8vYn), clang 18 seems to still be fine (https://godbolt.org/z/vYeKhanj6)

Based on the `DeduceTemplateSpecializationFromInitializer` in the stack trace, I guess this is related to CTAD


crash trace (as of commit b8d5ef9d2e197234da197986b606b6753e363f24):

```
clang-22: /home/avogelsgesang/Documents/llvm-project/llvm/include/llvm/ADT/SmallVector.h:295: T& llvm::SmallVectorTemplateCommon<T, <template-parameter-1-2> >::operator[](size_type) [with T = clang::QualType; <template-parameter-1-2> = void; reference = clang::QualType&; size_type = long unsigned int]: Assertion `idx < size()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /home/avogelsgesang/Documents/llvm-project/build/bin/clang-22 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -main-file-name repro.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/home/avogelsgesang/Documents/hyper/avogelsgesang-string-alloc -fcoverage-compilation-dir=/home/avogelsgesang/Documents/hyper/avogelsgesang-string-alloc -resource-dir /home/avogelsgesang/Documents/llvm-project/build/lib/clang/22 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/14/../../../../include/c++/14 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/14/../../../../include/x86_64-linux-gnu/c++/14 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/14/../../../../include/c++/14/backward -internal-isystem /home/avogelsgesang/Documents/llvm-project/build/lib/clang/22/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/14/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -std=c++20 -fdeprecated-macro -ferror-limit 19 -fmessage-length=115 -fgnuc-version=4.2.1 -fno-implicit-modules -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -fdwarf2-cfi-asm -o /tmp/repro-1a04e2.o -x c++ repro.cpp
1.      repro.cpp:3:41: current parser token ';'
2.      repro.cpp:1:1: parsing namespace 'x'
3.      repro.cpp:2:16: parsing struct/union/class body 'x::myType'
 #0 0x000063f8e0b4af62 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /home/avogelsgesang/Documents/llvm-project/llvm/lib/Support/Unix/Signals.inc:846:3
 #1 0x000063f8e0b47ebf llvm::sys::RunSignalHandlers() /home/avogelsgesang/Documents/llvm-project/llvm/lib/Support/Signals.cpp:108:20
 #2 0x000063f8e0b486d1 SignalHandler(int, siginfo_t*, void*) /home/avogelsgesang/Documents/llvm-project/llvm/lib/Support/Unix/Signals.inc:429:14
 #3 0x00007c3522245330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x00007c352229eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x00007c352229eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x00007c352229eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x00007c352224527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x00007c35222288ff abort ./stdlib/abort.c:81:7
 #9 0x00007c352222881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#10 0x00007c352223b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#11 0x000063f8e34a5688 clang::Sema::DeduceTemplateSpecializationFromInitializer(clang::TypeSourceInfo*, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::MutableArrayRef<clang::Expr*>)::'lambda'(bool)::operator()(bool) const::'lambda'(clang::InitListExpr*)::operator()(clang::InitListExpr*) const /home/avogelsgesang/Documents/llvm-project/llvm/include/llvm/Support/Casting.h:109:5
#12 0x000063f8e34a5988 clang::Sema::DeduceTemplateSpecializationFromInitializer(clang::TypeSourceInfo*, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::MutableArrayRef<clang::Expr*>)::'lambda'(bool)::operator()(bool) const /home/avogelsgesang/Documents/llvm-project/clang/lib/Sema/SemaInit.cpp:10321:28
#13 0x000063f8e34a5e30 clang::Sema::DeduceTemplateSpecializationFromInitializer(clang::TypeSourceInfo*, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::MutableArrayRef<clang::Expr*>) /home/avogelsgesang/Documents/llvm-project/clang/lib/Sema/SemaInit.cpp:10365:69
#14 0x000063f8e31c72c6 clang::Sema::deduceVarTypeFromInitializer(clang::VarDecl*, clang::DeclarationName, clang::QualType, clang::TypeSourceInfo*, clang::SourceRange, bool, clang::Expr*) /home/avogelsgesang/Documents/llvm-project/llvm/include/llvm/ADT/SmallVector.h:145:39
#15 0x000063f8e31dba90 clang::Sema::DeduceVariableDeclarationType(clang::VarDecl*, bool, clang::Expr*) /home/avogelsgesang/Documents/llvm-project/clang/lib/Sema/SemaDecl.cpp:13346:3
#16 0x000063f8e31f36f6 clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) /home/avogelsgesang/Documents/llvm-project/clang/lib/Sema/SemaDecl.cpp:13843:5
#17 0x000063f8e2ed3648 clang::Parser::ParseCXXClassMemberDeclaration(clang::AccessSpecifier, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::ParsingDeclRAIIObject*) /home/avogelsgesang/Documents/llvm-project/clang/lib/Parse/ParseDeclCXX.cpp:3205:5
#18 0x000063f8e2ed4669 clang::Parser::ParseCXXClassMemberDeclarationWithPragmas(clang::AccessSpecifier&, clang::ParsedAttributes&, clang::TypeSpecifierType, clang::Decl*) /home/avogelsgesang/Documents/llvm-project/clang/lib/Parse/ParseDeclCXX.cpp:3499:42
#19 0x000063f8e2ed4d3c clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&, unsigned int, clang::Decl*) /home/avogelsgesang/Documents/llvm-project/clang/include/clang/Parse/Parser.h:7821:5
#20 0x000063f8e2ed73d0 clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&) /home/avogelsgesang/Documents/llvm-project/clang/lib/Parse/ParseDeclCXX.cpp:2170:34
#21 0x000063f8e2ea231e clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) /home/avogelsgesang/Documents/llvm-project/clang/lib/Parse/ParseDecl.cpp:4471:7
#22 0x000063f8e2f6c458 clang::Parser::ParseDeclarationAfterTemplate(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo&, clang::ParsingDeclRAIIObject&, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) /home/avogelsgesang/Documents/llvm-project/clang/include/clang/Lex/Token.h:102:44
#23 0x000063f8e2f77496 clang::Parser::ParseTemplateDeclarationOrSpecialization(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (.part.0) /home/avogelsgesang/Documents/llvm-project/clang/lib/Parse/ParseTemplate.cpp:147:39
#24 0x000063f8e2f778c5 clang::Parser::ParseDeclarationStartingWithTemplate(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&) /home/avogelsgesang/Documents/llvm-project/clang/lib/Parse/ParseTemplate.cpp:38:32
#25 0x000063f8e2eb3213 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) /home/avogelsgesang/Documents/llvm-project/clang/lib/Parse/ParseDecl.cpp:1890:48
#26 0x000063f8e2e777b5 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) /home/avogelsgesang/Documents/llvm-project/clang/lib/Parse/Parser.cpp:965:30
#27 0x000063f8e2ecc772 clang::Parser::ParseInnerNamespace(llvm::SmallVector<clang::Parser::InnerNamespaceInfo, 4u> const&, unsigned int, clang::SourceLocation&, clang::ParsedAttributes&, clang::BalancedDelimiterTracker&) /home/avogelsgesang/Documents/llvm-project/clang/include/clang/Sema/ParsedAttr.h:709:41
#28 0x000063f8e2ed006f clang::Parser::ParseNamespace(clang::DeclaratorContext, clang::SourceLocation&, clang::SourceLocation) /home/avogelsgesang/Documents/llvm-project/clang/include/clang/Parse/Parser.h:422:24
#29 0x000063f8e2eb32c9 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) /home/avogelsgesang/Documents/llvm-project/clang/lib/Parse/ParseDecl.cpp:1909:26
#30 0x000063f8e2e777b5 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) /home/avogelsgesang/Documents/llvm-project/clang/lib/Parse/Parser.cpp:965:30
#31 0x000063f8e2e79227 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) /home/avogelsgesang/Documents/llvm-project/clang/lib/Parse/Parser.cpp:744:36
#32 0x000063f8e2e79703 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) /home/avogelsgesang/Documents/llvm-project/clang/lib/Parse/Parser.cpp:608:32
#33 0x000063f8e2e5c8ba clang::ParseAST(clang::Sema&, bool, bool) /home/avogelsgesang/Documents/llvm-project/clang/lib/Parse/ParseAST.cpp:169:69
#34 0x000063f8e16d2487 clang::FrontendAction::Execute() /home/avogelsgesang/Documents/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1316:38
#35 0x000063f8e1665ece clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) /home/avogelsgesang/Documents/llvm-project/llvm/include/llvm/Support/Error.h:278:51
#36 0x000063f8e17b4535 std::__shared_ptr<clang::FrontendOptions, (__gnu_cxx::_Lock_policy)2>::get() const /usr/include/c++/13/bits/shared_ptr_base.h:1666:16
#37 0x000063f8e17b4535 std::__shared_ptr_access<clang::FrontendOptions, (__gnu_cxx::_Lock_policy)2, false, false>::_M_get() const /usr/include/c++/13/bits/shared_ptr_base.h:1363:69
#38 0x000063f8e17b4535 std::__shared_ptr_access<clang::FrontendOptions, (__gnu_cxx::_Lock_policy)2, false, false>::operator*() const /usr/include/c++/13/bits/shared_ptr_base.h:1349:2
#39 0x000063f8e17b4535 clang::CompilerInvocation::getFrontendOpts() /home/avogelsgesang/Documents/llvm-project/clang/include/clang/Frontend/CompilerInvocation.h:277:48
#40 0x000063f8e17b4535 clang::CompilerInstance::getFrontendOpts() /home/avogelsgesang/Documents/llvm-project/clang/include/clang/Frontend/CompilerInstance.h:301:39
#41 0x000063f8e17b4535 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) /home/avogelsgesang/Documents/llvm-project/clang/lib/FrontendTool/ExecuteCompilerInvocation.cpp:311:29
#42 0x000063f8df715580 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) /home/avogelsgesang/Documents/llvm-project/clang/tools/driver/cc1_main.cpp:304:40
#43 0x000063f8df70ca29 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) /home/avogelsgesang/Documents/llvm-project/clang/tools/driver/driver.cpp:225:20
#44 0x000063f8df70f969 clang_main(int, char**, llvm::ToolContext const&) /home/avogelsgesang/Documents/llvm-project/clang/tools/driver/driver.cpp:268:26
#45 0x000063f8df60d17a main /home/avogelsgesang/Documents/llvm-project/build/tools/clang/tools/driver/clang-driver.cpp:18:1
#46 0x00007c352222a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#47 0x00007c352222a28b call_init ./csu/../csu/libc-start.c:128:20
#48 0x00007c352222a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#49 0x000063f8df70bd85 _start (/home/avogelsgesang/Documents/llvm-project/build/bin/clang-22+0xcb3d85)
clang: error: unable to execute command: Aborted (core dumped)
clang: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 22.0.0git
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /home/avogelsgesang/Documents/llvm-project/build/bin
Build config: +assertions
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-c-20

Author: Adrian Vogelsgesang (vogelsgesang)

<details>
The source code

```
namespace x {
template &lt;int&gt; struct myType {
  static constexpr x::myType variable {};
};
}
```

with `-std=c++20` crashes clang.
Started with clang 19 (https://godbolt.org/z/E63qT8vYn), clang 18 seems to still be fine (https://godbolt.org/z/vYeKhanj6)

Based on the `DeduceTemplateSpecializationFromInitializer` in the stack trace, I guess this is related to CTAD


crash trace (clang commit b8d5ef9d2e197234da197986b606b6753e363f24):

```
clang-22: /home/avogelsgesang/Documents/llvm-project/llvm/include/llvm/ADT/SmallVector.h:295: T&amp; llvm::SmallVectorTemplateCommon&lt;T, &lt;template-parameter-1-2&gt; &gt;::operator[](size_type) [with T = clang::QualType; &lt;template-parameter-1-2&gt; = void; reference = clang::QualType&amp;; size_type = long unsigned int]: Assertion `idx &lt; size()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /home/avogelsgesang/Documents/llvm-project/build/bin/clang-22 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -main-file-name repro.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/home/avogelsgesang/Documents/hyper/avogelsgesang-string-alloc -fcoverage-compilation-dir=/home/avogelsgesang/Documents/hyper/avogelsgesang-string-alloc -resource-dir /home/avogelsgesang/Documents/llvm-project/build/lib/clang/22 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/14/../../../../include/c++/14 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/14/../../../../include/x86_64-linux-gnu/c++/14 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/14/../../../../include/c++/14/backward -internal-isystem /home/avogelsgesang/Documents/llvm-project/build/lib/clang/22/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/14/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -std=c++20 -fdeprecated-macro -ferror-limit 19 -fmessage-length=115 -fgnuc-version=4.2.1 -fno-implicit-modules -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -fdwarf2-cfi-asm -o /tmp/repro-1a04e2.o -x c++ repro.cpp
1.      repro.cpp:3:41: current parser token ';'
2.      repro.cpp:1:1: parsing namespace 'x'
3.      repro.cpp:2:16: parsing struct/union/class body 'x::myType'
 #<!-- -->0 0x000063f8e0b4af62 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /home/avogelsgesang/Documents/llvm-project/llvm/lib/Support/Unix/Signals.inc:846:3
 #<!-- -->1 0x000063f8e0b47ebf llvm::sys::RunSignalHandlers() /home/avogelsgesang/Documents/llvm-project/llvm/lib/Support/Signals.cpp:108:20
 #<!-- -->2 0x000063f8e0b486d1 SignalHandler(int, siginfo_t*, void*) /home/avogelsgesang/Documents/llvm-project/llvm/lib/Support/Unix/Signals.inc:429:14
 #<!-- -->3 0x00007c3522245330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->4 0x00007c352229eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->5 0x00007c352229eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->6 0x00007c352229eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->7 0x00007c352224527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->8 0x00007c35222288ff abort ./stdlib/abort.c:81:7
 #<!-- -->9 0x00007c352222881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->10 0x00007c352223b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->11 0x000063f8e34a5688 clang::Sema::DeduceTemplateSpecializationFromInitializer(clang::TypeSourceInfo*, clang::InitializedEntity const&amp;, clang::InitializationKind const&amp;, llvm::MutableArrayRef&lt;clang::Expr*&gt;)::'lambda'(bool)::operator()(bool) const::'lambda'(clang::InitListExpr*)::operator()(clang::InitListExpr*) const /home/avogelsgesang/Documents/llvm-project/llvm/include/llvm/Support/Casting.h:109:5
#<!-- -->12 0x000063f8e34a5988 clang::Sema::DeduceTemplateSpecializationFromInitializer(clang::TypeSourceInfo*, clang::InitializedEntity const&amp;, clang::InitializationKind const&amp;, llvm::MutableArrayRef&lt;clang::Expr*&gt;)::'lambda'(bool)::operator()(bool) const /home/avogelsgesang/Documents/llvm-project/clang/lib/Sema/SemaInit.cpp:10321:28
#<!-- -->13 0x000063f8e34a5e30 clang::Sema::DeduceTemplateSpecializationFromInitializer(clang::TypeSourceInfo*, clang::InitializedEntity const&amp;, clang::InitializationKind const&amp;, llvm::MutableArrayRef&lt;clang::Expr*&gt;) /home/avogelsgesang/Documents/llvm-project/clang/lib/Sema/SemaInit.cpp:10365:69
#<!-- -->14 0x000063f8e31c72c6 clang::Sema::deduceVarTypeFromInitializer(clang::VarDecl*, clang::DeclarationName, clang::QualType, clang::TypeSourceInfo*, clang::SourceRange, bool, clang::Expr*) /home/avogelsgesang/Documents/llvm-project/llvm/include/llvm/ADT/SmallVector.h:145:39
#<!-- -->15 0x000063f8e31dba90 clang::Sema::DeduceVariableDeclarationType(clang::VarDecl*, bool, clang::Expr*) /home/avogelsgesang/Documents/llvm-project/clang/lib/Sema/SemaDecl.cpp:13346:3
#<!-- -->16 0x000063f8e31f36f6 clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) /home/avogelsgesang/Documents/llvm-project/clang/lib/Sema/SemaDecl.cpp:13843:5
#<!-- -->17 0x000063f8e2ed3648 clang::Parser::ParseCXXClassMemberDeclaration(clang::AccessSpecifier, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::ParsingDeclRAIIObject*) /home/avogelsgesang/Documents/llvm-project/clang/lib/Parse/ParseDeclCXX.cpp:3205:5
#<!-- -->18 0x000063f8e2ed4669 clang::Parser::ParseCXXClassMemberDeclarationWithPragmas(clang::AccessSpecifier&amp;, clang::ParsedAttributes&amp;, clang::TypeSpecifierType, clang::Decl*) /home/avogelsgesang/Documents/llvm-project/clang/lib/Parse/ParseDeclCXX.cpp:3499:42
#<!-- -->19 0x000063f8e2ed4d3c clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&amp;, unsigned int, clang::Decl*) /home/avogelsgesang/Documents/llvm-project/clang/include/clang/Parse/Parser.h:7821:5
#<!-- -->20 0x000063f8e2ed73d0 clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&amp;) /home/avogelsgesang/Documents/llvm-project/clang/lib/Parse/ParseDeclCXX.cpp:2170:34
#<!-- -->21 0x000063f8e2ea231e clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) /home/avogelsgesang/Documents/llvm-project/clang/lib/Parse/ParseDecl.cpp:4471:7
#<!-- -->22 0x000063f8e2f6c458 clang::Parser::ParseDeclarationAfterTemplate(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo&amp;, clang::ParsingDeclRAIIObject&amp;, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::AccessSpecifier) /home/avogelsgesang/Documents/llvm-project/clang/include/clang/Lex/Token.h:102:44
#<!-- -->23 0x000063f8e2f77496 clang::Parser::ParseTemplateDeclarationOrSpecialization(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::AccessSpecifier) (.part.0) /home/avogelsgesang/Documents/llvm-project/clang/lib/Parse/ParseTemplate.cpp:147:39
#<!-- -->24 0x000063f8e2f778c5 clang::Parser::ParseDeclarationStartingWithTemplate(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;) /home/avogelsgesang/Documents/llvm-project/clang/lib/Parse/ParseTemplate.cpp:38:32
#<!-- -->25 0x000063f8e2eb3213 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) /home/avogelsgesang/Documents/llvm-project/clang/lib/Parse/ParseDecl.cpp:1890:48
#<!-- -->26 0x000063f8e2e777b5 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) /home/avogelsgesang/Documents/llvm-project/clang/lib/Parse/Parser.cpp:965:30
#<!-- -->27 0x000063f8e2ecc772 clang::Parser::ParseInnerNamespace(llvm::SmallVector&lt;clang::Parser::InnerNamespaceInfo, 4u&gt; const&amp;, unsigned int, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::BalancedDelimiterTracker&amp;) /home/avogelsgesang/Documents/llvm-project/clang/include/clang/Sema/ParsedAttr.h:709:41
#<!-- -->28 0x000063f8e2ed006f clang::Parser::ParseNamespace(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::SourceLocation) /home/avogelsgesang/Documents/llvm-project/clang/include/clang/Parse/Parser.h:422:24
#<!-- -->29 0x000063f8e2eb32c9 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) /home/avogelsgesang/Documents/llvm-project/clang/lib/Parse/ParseDecl.cpp:1909:26
#<!-- -->30 0x000063f8e2e777b5 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) /home/avogelsgesang/Documents/llvm-project/clang/lib/Parse/Parser.cpp:965:30
#<!-- -->31 0x000063f8e2e79227 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) /home/avogelsgesang/Documents/llvm-project/clang/lib/Parse/Parser.cpp:744:36
#<!-- -->32 0x000063f8e2e79703 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) /home/avogelsgesang/Documents/llvm-project/clang/lib/Parse/Parser.cpp:608:32
#<!-- -->33 0x000063f8e2e5c8ba clang::ParseAST(clang::Sema&amp;, bool, bool) /home/avogelsgesang/Documents/llvm-project/clang/lib/Parse/ParseAST.cpp:169:69
#<!-- -->34 0x000063f8e16d2487 clang::FrontendAction::Execute() /home/avogelsgesang/Documents/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1316:38
#<!-- -->35 0x000063f8e1665ece clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) /home/avogelsgesang/Documents/llvm-project/llvm/include/llvm/Support/Error.h:278:51
#<!-- -->36 0x000063f8e17b4535 std::__shared_ptr&lt;clang::FrontendOptions, (__gnu_cxx::_Lock_policy)2&gt;::get() const /usr/include/c++/13/bits/shared_ptr_base.h:1666:16
#<!-- -->37 0x000063f8e17b4535 std::__shared_ptr_access&lt;clang::FrontendOptions, (__gnu_cxx::_Lock_policy)2, false, false&gt;::_M_get() const /usr/include/c++/13/bits/shared_ptr_base.h:1363:69
#<!-- -->38 0x000063f8e17b4535 std::__shared_ptr_access&lt;clang::FrontendOptions, (__gnu_cxx::_Lock_policy)2, false, false&gt;::operator*() const /usr/include/c++/13/bits/shared_ptr_base.h:1349:2
#<!-- -->39 0x000063f8e17b4535 clang::CompilerInvocation::getFrontendOpts() /home/avogelsgesang/Documents/llvm-project/clang/include/clang/Frontend/CompilerInvocation.h:277:48
#<!-- -->40 0x000063f8e17b4535 clang::CompilerInstance::getFrontendOpts() /home/avogelsgesang/Documents/llvm-project/clang/include/clang/Frontend/CompilerInstance.h:301:39
#<!-- -->41 0x000063f8e17b4535 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) /home/avogelsgesang/Documents/llvm-project/clang/lib/FrontendTool/ExecuteCompilerInvocation.cpp:311:29
#<!-- -->42 0x000063f8df715580 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) /home/avogelsgesang/Documents/llvm-project/clang/tools/driver/cc1_main.cpp:304:40
#<!-- -->43 0x000063f8df70ca29 ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) /home/avogelsgesang/Documents/llvm-project/clang/tools/driver/driver.cpp:225:20
#<!-- -->44 0x000063f8df70f969 clang_main(int, char**, llvm::ToolContext const&amp;) /home/avogelsgesang/Documents/llvm-project/clang/tools/driver/driver.cpp:268:26
#<!-- -->45 0x000063f8df60d17a main /home/avogelsgesang/Documents/llvm-project/build/tools/clang/tools/driver/clang-driver.cpp:18:1
#<!-- -->46 0x00007c352222a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->47 0x00007c352222a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->48 0x00007c352222a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->49 0x000063f8df70bd85 _start (/home/avogelsgesang/Documents/llvm-project/build/bin/clang-22+0xcb3d85)
clang: error: unable to execute command: Aborted (core dumped)
clang: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 22.0.0git
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /home/avogelsgesang/Documents/llvm-project/build/bin
Build config: +assertions
```
</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Adrian Vogelsgesang (vogelsgesang)

<details>
The source code

```
namespace x {
template &lt;int&gt; struct myType {
  static constexpr x::myType variable {};
};
}
```

with `-std=c++20` crashes clang on trunk (as of commit b8d5ef9d2e197234da197986b606b6753e363f24)
Started with clang 19 (https://godbolt.org/z/E63qT8vYn), clang 18 seems to still be fine (https://godbolt.org/z/vYeKhanj6)

Based on the `DeduceTemplateSpecializationFromInitializer` in the stack trace, I guess this is related to CTAD


crash trace (as of commit b8d5ef9d2e197234da197986b606b6753e363f24):

```
clang-22: /home/avogelsgesang/Documents/llvm-project/llvm/include/llvm/ADT/SmallVector.h:295: T&amp; llvm::SmallVectorTemplateCommon&lt;T, &lt;template-parameter-1-2&gt; &gt;::operator[](size_type) [with T = clang::QualType; &lt;template-parameter-1-2&gt; = void; reference = clang::QualType&amp;; size_type = long unsigned int]: Assertion `idx &lt; size()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /home/avogelsgesang/Documents/llvm-project/build/bin/clang-22 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -main-file-name repro.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/home/avogelsgesang/Documents/hyper/avogelsgesang-string-alloc -fcoverage-compilation-dir=/home/avogelsgesang/Documents/hyper/avogelsgesang-string-alloc -resource-dir /home/avogelsgesang/Documents/llvm-project/build/lib/clang/22 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/14/../../../../include/c++/14 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/14/../../../../include/x86_64-linux-gnu/c++/14 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/14/../../../../include/c++/14/backward -internal-isystem /home/avogelsgesang/Documents/llvm-project/build/lib/clang/22/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/14/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -std=c++20 -fdeprecated-macro -ferror-limit 19 -fmessage-length=115 -fgnuc-version=4.2.1 -fno-implicit-modules -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -fdwarf2-cfi-asm -o /tmp/repro-1a04e2.o -x c++ repro.cpp
1.      repro.cpp:3:41: current parser token ';'
2.      repro.cpp:1:1: parsing namespace 'x'
3.      repro.cpp:2:16: parsing struct/union/class body 'x::myType'
 #<!-- -->0 0x000063f8e0b4af62 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /home/avogelsgesang/Documents/llvm-project/llvm/lib/Support/Unix/Signals.inc:846:3
 #<!-- -->1 0x000063f8e0b47ebf llvm::sys::RunSignalHandlers() /home/avogelsgesang/Documents/llvm-project/llvm/lib/Support/Signals.cpp:108:20
 #<!-- -->2 0x000063f8e0b486d1 SignalHandler(int, siginfo_t*, void*) /home/avogelsgesang/Documents/llvm-project/llvm/lib/Support/Unix/Signals.inc:429:14
 #<!-- -->3 0x00007c3522245330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->4 0x00007c352229eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->5 0x00007c352229eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->6 0x00007c352229eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->7 0x00007c352224527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->8 0x00007c35222288ff abort ./stdlib/abort.c:81:7
 #<!-- -->9 0x00007c352222881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->10 0x00007c352223b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->11 0x000063f8e34a5688 clang::Sema::DeduceTemplateSpecializationFromInitializer(clang::TypeSourceInfo*, clang::InitializedEntity const&amp;, clang::InitializationKind const&amp;, llvm::MutableArrayRef&lt;clang::Expr*&gt;)::'lambda'(bool)::operator()(bool) const::'lambda'(clang::InitListExpr*)::operator()(clang::InitListExpr*) const /home/avogelsgesang/Documents/llvm-project/llvm/include/llvm/Support/Casting.h:109:5
#<!-- -->12 0x000063f8e34a5988 clang::Sema::DeduceTemplateSpecializationFromInitializer(clang::TypeSourceInfo*, clang::InitializedEntity const&amp;, clang::InitializationKind const&amp;, llvm::MutableArrayRef&lt;clang::Expr*&gt;)::'lambda'(bool)::operator()(bool) const /home/avogelsgesang/Documents/llvm-project/clang/lib/Sema/SemaInit.cpp:10321:28
#<!-- -->13 0x000063f8e34a5e30 clang::Sema::DeduceTemplateSpecializationFromInitializer(clang::TypeSourceInfo*, clang::InitializedEntity const&amp;, clang::InitializationKind const&amp;, llvm::MutableArrayRef&lt;clang::Expr*&gt;) /home/avogelsgesang/Documents/llvm-project/clang/lib/Sema/SemaInit.cpp:10365:69
#<!-- -->14 0x000063f8e31c72c6 clang::Sema::deduceVarTypeFromInitializer(clang::VarDecl*, clang::DeclarationName, clang::QualType, clang::TypeSourceInfo*, clang::SourceRange, bool, clang::Expr*) /home/avogelsgesang/Documents/llvm-project/llvm/include/llvm/ADT/SmallVector.h:145:39
#<!-- -->15 0x000063f8e31dba90 clang::Sema::DeduceVariableDeclarationType(clang::VarDecl*, bool, clang::Expr*) /home/avogelsgesang/Documents/llvm-project/clang/lib/Sema/SemaDecl.cpp:13346:3
#<!-- -->16 0x000063f8e31f36f6 clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) /home/avogelsgesang/Documents/llvm-project/clang/lib/Sema/SemaDecl.cpp:13843:5
#<!-- -->17 0x000063f8e2ed3648 clang::Parser::ParseCXXClassMemberDeclaration(clang::AccessSpecifier, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::ParsingDeclRAIIObject*) /home/avogelsgesang/Documents/llvm-project/clang/lib/Parse/ParseDeclCXX.cpp:3205:5
#<!-- -->18 0x000063f8e2ed4669 clang::Parser::ParseCXXClassMemberDeclarationWithPragmas(clang::AccessSpecifier&amp;, clang::ParsedAttributes&amp;, clang::TypeSpecifierType, clang::Decl*) /home/avogelsgesang/Documents/llvm-project/clang/lib/Parse/ParseDeclCXX.cpp:3499:42
#<!-- -->19 0x000063f8e2ed4d3c clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&amp;, unsigned int, clang::Decl*) /home/avogelsgesang/Documents/llvm-project/clang/include/clang/Parse/Parser.h:7821:5
#<!-- -->20 0x000063f8e2ed73d0 clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&amp;) /home/avogelsgesang/Documents/llvm-project/clang/lib/Parse/ParseDeclCXX.cpp:2170:34
#<!-- -->21 0x000063f8e2ea231e clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) /home/avogelsgesang/Documents/llvm-project/clang/lib/Parse/ParseDecl.cpp:4471:7
#<!-- -->22 0x000063f8e2f6c458 clang::Parser::ParseDeclarationAfterTemplate(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo&amp;, clang::ParsingDeclRAIIObject&amp;, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::AccessSpecifier) /home/avogelsgesang/Documents/llvm-project/clang/include/clang/Lex/Token.h:102:44
#<!-- -->23 0x000063f8e2f77496 clang::Parser::ParseTemplateDeclarationOrSpecialization(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::AccessSpecifier) (.part.0) /home/avogelsgesang/Documents/llvm-project/clang/lib/Parse/ParseTemplate.cpp:147:39
#<!-- -->24 0x000063f8e2f778c5 clang::Parser::ParseDeclarationStartingWithTemplate(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;) /home/avogelsgesang/Documents/llvm-project/clang/lib/Parse/ParseTemplate.cpp:38:32
#<!-- -->25 0x000063f8e2eb3213 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) /home/avogelsgesang/Documents/llvm-project/clang/lib/Parse/ParseDecl.cpp:1890:48
#<!-- -->26 0x000063f8e2e777b5 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) /home/avogelsgesang/Documents/llvm-project/clang/lib/Parse/Parser.cpp:965:30
#<!-- -->27 0x000063f8e2ecc772 clang::Parser::ParseInnerNamespace(llvm::SmallVector&lt;clang::Parser::InnerNamespaceInfo, 4u&gt; const&amp;, unsigned int, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::BalancedDelimiterTracker&amp;) /home/avogelsgesang/Documents/llvm-project/clang/include/clang/Sema/ParsedAttr.h:709:41
#<!-- -->28 0x000063f8e2ed006f clang::Parser::ParseNamespace(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::SourceLocation) /home/avogelsgesang/Documents/llvm-project/clang/include/clang/Parse/Parser.h:422:24
#<!-- -->29 0x000063f8e2eb32c9 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) /home/avogelsgesang/Documents/llvm-project/clang/lib/Parse/ParseDecl.cpp:1909:26
#<!-- -->30 0x000063f8e2e777b5 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) /home/avogelsgesang/Documents/llvm-project/clang/lib/Parse/Parser.cpp:965:30
#<!-- -->31 0x000063f8e2e79227 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) /home/avogelsgesang/Documents/llvm-project/clang/lib/Parse/Parser.cpp:744:36
#<!-- -->32 0x000063f8e2e79703 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) /home/avogelsgesang/Documents/llvm-project/clang/lib/Parse/Parser.cpp:608:32
#<!-- -->33 0x000063f8e2e5c8ba clang::ParseAST(clang::Sema&amp;, bool, bool) /home/avogelsgesang/Documents/llvm-project/clang/lib/Parse/ParseAST.cpp:169:69
#<!-- -->34 0x000063f8e16d2487 clang::FrontendAction::Execute() /home/avogelsgesang/Documents/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1316:38
#<!-- -->35 0x000063f8e1665ece clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) /home/avogelsgesang/Documents/llvm-project/llvm/include/llvm/Support/Error.h:278:51
#<!-- -->36 0x000063f8e17b4535 std::__shared_ptr&lt;clang::FrontendOptions, (__gnu_cxx::_Lock_policy)2&gt;::get() const /usr/include/c++/13/bits/shared_ptr_base.h:1666:16
#<!-- -->37 0x000063f8e17b4535 std::__shared_ptr_access&lt;clang::FrontendOptions, (__gnu_cxx::_Lock_policy)2, false, false&gt;::_M_get() const /usr/include/c++/13/bits/shared_ptr_base.h:1363:69
#<!-- -->38 0x000063f8e17b4535 std::__shared_ptr_access&lt;clang::FrontendOptions, (__gnu_cxx::_Lock_policy)2, false, false&gt;::operator*() const /usr/include/c++/13/bits/shared_ptr_base.h:1349:2
#<!-- -->39 0x000063f8e17b4535 clang::CompilerInvocation::getFrontendOpts() /home/avogelsgesang/Documents/llvm-project/clang/include/clang/Frontend/CompilerInvocation.h:277:48
#<!-- -->40 0x000063f8e17b4535 clang::CompilerInstance::getFrontendOpts() /home/avogelsgesang/Documents/llvm-project/clang/include/clang/Frontend/CompilerInstance.h:301:39
#<!-- -->41 0x000063f8e17b4535 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) /home/avogelsgesang/Documents/llvm-project/clang/lib/FrontendTool/ExecuteCompilerInvocation.cpp:311:29
#<!-- -->42 0x000063f8df715580 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) /home/avogelsgesang/Documents/llvm-project/clang/tools/driver/cc1_main.cpp:304:40
#<!-- -->43 0x000063f8df70ca29 ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) /home/avogelsgesang/Documents/llvm-project/clang/tools/driver/driver.cpp:225:20
#<!-- -->44 0x000063f8df70f969 clang_main(int, char**, llvm::ToolContext const&amp;) /home/avogelsgesang/Documents/llvm-project/clang/tools/driver/driver.cpp:268:26
#<!-- -->45 0x000063f8df60d17a main /home/avogelsgesang/Documents/llvm-project/build/tools/clang/tools/driver/clang-driver.cpp:18:1
#<!-- -->46 0x00007c352222a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->47 0x00007c352222a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->48 0x00007c352222a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->49 0x000063f8df70bd85 _start (/home/avogelsgesang/Documents/llvm-project/build/bin/clang-22+0xcb3d85)
clang: error: unable to execute command: Aborted (core dumped)
clang: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 22.0.0git
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /home/avogelsgesang/Documents/llvm-project/build/bin
Build config: +assertions
```
</details>


---

### Comment 3 - shafik

@sdkrystian this bisects to acf5ad2a4ed9bf94b03d18ccddce7710e721dc6c

---

