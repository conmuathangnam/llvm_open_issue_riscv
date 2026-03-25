# clang-20 crashed with vectorcall attribute.

**Author:** iamanonymouscs
**URL:** https://github.com/llvm/llvm-project/issues/121865

## Body

clang-20 crashed with ```vectorcall``` attribute.

Compiler explorer: https://godbolt.org/z/6fd1eYa4W

```
$cat mutant.C
template <class> class a {
    typedef int a::*b();        
    b c __attribute__((vectorcall)) d
} a<int>::
```

It was reduced by Creduce.
Also crashed at Clang-19.

```
$clang-19 mutant.C
mutant.C:3:36: error: expected ';' at end of declaration list
    3 |     b c __attribute__((vectorcall)) d
      |                                    ^
      |                                    ;
mutant.C:4:2: error: expected ';' after class
    4 | } a<int>::
      |  ^
      |  ;
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /usr/lib/llvm-19/bin/clang -cc1 -triple x86_64-pc-linux-gnu -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name mutant.C -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/home/code/InsertAttr -fcoverage-compilation-dir=/home/code/InsertAttr -resource-dir /usr/lib/llvm-19/lib/clang/19 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/x86_64-linux-gnu/c++/11 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/backward -internal-isystem /usr/lib/llvm-19/lib/clang/19/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -fdeprecated-macro -ferror-limit 19 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/mutant-64345b.o -x c++ mutant.C
1.      <eof> parser at end of file
2.      mutant.C:1:24: instantiating class definition 'a<int>'
 #0 0x00007f1ec31c7246 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.0+0xea7246)
 #1 0x00007f1ec31c4e20 llvm::sys::RunSignalHandlers() (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.0+0xea4e20)
 #2 0x00007f1ec31c790b (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.0+0xea790b)
 #3 0x00007f1ec1dff520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007f1ecb16c0e2 clang::TypeLoc::getBeginLoc() const (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x12ba0e2)
 #5 0x00007f1ecbc0966a (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x1d5766a)
 #6 0x00007f1ecbc2e8ad (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x1d7c8ad)
 #7 0x00007f1ecbc31b86 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x1d7fb86)
 #8 0x00007f1ecbc31733 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x1d7f733)
 #9 0x00007f1ecbc31624 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x1d7f624)
#10 0x00007f1ecbc0a346 clang::Sema::SubstFunctionDeclType(clang::TypeSourceInfo*, clang::MultiLevelTemplateArgumentList const&, clang::SourceLocation, clang::DeclarationName, clang::CXXRecordDecl*, clang::Qualifiers, bool) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x1d58346)
#11 0x00007f1ecbc4ece4 clang::TemplateDeclInstantiator::SubstFunctionType(clang::FunctionDecl*, llvm::SmallVectorImpl<clang::ParmVarDecl*>&) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x1d9cce4)
#12 0x00007f1ecbc4bbe1 clang::TemplateDeclInstantiator::VisitCXXMethodDecl(clang::CXXMethodDecl*, clang::TemplateParameterList*, clang::TemplateDeclInstantiator::RewriteKind) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x1d99be1)
#13 0x00007f1ecbc0d402 clang::Sema::InstantiateClass(clang::SourceLocation, clang::CXXRecordDecl*, clang::CXXRecordDecl*, clang::MultiLevelTemplateArgumentList const&, clang::TemplateSpecializationKind, bool) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x1d5b402)
#14 0x00007f1ecbc0f306 clang::Sema::InstantiateClassTemplateSpecialization(clang::SourceLocation, clang::ClassTemplateSpecializationDecl*, clang::TemplateSpecializationKind, bool) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x1d5d306)
#15 0x00007f1ecbcc811e (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x1e1611e)
#16 0x00007f1ecb414c3d clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x1562c3d)
#17 0x00007f1ecbcb7fbf clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x1e05fbf)
#18 0x00007f1ecbcb786f clang::Sema::RequireCompleteType(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser&) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x1e0586f)
#19 0x00007f1ecb4a6593 clang::Sema::RequireCompleteDeclContext(clang::CXXScopeSpec&, clang::DeclContext*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x15f4593)
#20 0x00007f1ecb4a95b8 clang::Sema::ActOnCXXEnterDeclaratorScope(clang::Scope*, clang::CXXScopeSpec&) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x15f75b8)
#21 0x00007f1ecab09ce0 clang::Parser::ParseDirectDeclarator(clang::Declarator&) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0xc57ce0)
#22 0x00007f1ecab08b4a clang::Parser::ParseDeclaratorInternal(clang::Declarator&, void (clang::Parser::*)(clang::Declarator&)) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0xc56b4a)
#23 0x00007f1ecb414c3d clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x1562c3d)
#24 0x00007f1ecaaf7e14 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0xc45e14)
#25 0x00007f1ecabb6987 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0xd04987)
#26 0x00007f1ecabb633b clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0xd0433b)
#27 0x00007f1ecabb5547 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0xd03547)
#28 0x00007f1ecabb3a27 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0xd01a27)
#29 0x00007f1ecaae17de clang::ParseAST(clang::Sema&, bool, bool) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0xc2f7de)
#30 0x00007f1eccb740b5 clang::FrontendAction::Execute() (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x2cc20b5)
#31 0x00007f1eccae42f4 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x2c322f4)
#32 0x00007f1eccbf0dee clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x2d3edee)
#33 0x00005635e849abc5 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/usr/lib/llvm-19/bin/clang+0x11bc5)
#34 0x00005635e8497ad5 (/usr/lib/llvm-19/bin/clang+0xead5)
#35 0x00005635e8496c9b clang_main(int, char**, llvm::ToolContext const&) (/usr/lib/llvm-19/bin/clang+0xdc9b)
#36 0x00005635e84a4f36 main (/usr/lib/llvm-19/bin/clang+0x1bf36)
#37 0x00007f1ec1de6d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#38 0x00007f1ec1de6e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#39 0x00005635e8495765 _start (/usr/lib/llvm-19/bin/clang+0xc765)
clang-19: error: unable to execute command: Segmentation fault (core dumped)
clang-19: error: clang frontend command failed due to signal (use -v to see invocation)
Ubuntu clang version 19.0.0 (++20240722031324+65825cd5431c-1~exp1~20240722151445.1819)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/lib/llvm-19/bin
clang-19: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang-19: note: diagnostic msg: /tmp/mutant-6508f1.cpp
clang-19: note: diagnostic msg: /tmp/mutant-6508f1.sh
clang-19: note: diagnostic msg: 

********************
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Anonymous (iamanonymouscs)

<details>
clang-20 crashed with ```vectorcall``` attribute.

Compiler explorer: https://godbolt.org/z/6fd1eYa4W

```
$cat mutant.C
template &lt;class&gt; class a {
    typedef int a::*b();        
    b c __attribute__((vectorcall)) d
} a&lt;int&gt;::
```

It was reduced by Creduce.
Also crashed at Clang-19.

```
$clang-19 mutant.C
mutant.C:3:36: error: expected ';' at end of declaration list
    3 |     b c __attribute__((vectorcall)) d
      |                                    ^
      |                                    ;
mutant.C:4:2: error: expected ';' after class
    4 | } a&lt;int&gt;::
      |  ^
      |  ;
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /usr/lib/llvm-19/bin/clang -cc1 -triple x86_64-pc-linux-gnu -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name mutant.C -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/home/code/InsertAttr -fcoverage-compilation-dir=/home/code/InsertAttr -resource-dir /usr/lib/llvm-19/lib/clang/19 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/x86_64-linux-gnu/c++/11 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/backward -internal-isystem /usr/lib/llvm-19/lib/clang/19/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -fdeprecated-macro -ferror-limit 19 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/mutant-64345b.o -x c++ mutant.C
1.      &lt;eof&gt; parser at end of file
2.      mutant.C:1:24: instantiating class definition 'a&lt;int&gt;'
 #<!-- -->0 0x00007f1ec31c7246 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.0+0xea7246)
 #<!-- -->1 0x00007f1ec31c4e20 llvm::sys::RunSignalHandlers() (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.0+0xea4e20)
 #<!-- -->2 0x00007f1ec31c790b (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.0+0xea790b)
 #<!-- -->3 0x00007f1ec1dff520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007f1ecb16c0e2 clang::TypeLoc::getBeginLoc() const (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x12ba0e2)
 #<!-- -->5 0x00007f1ecbc0966a (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x1d5766a)
 #<!-- -->6 0x00007f1ecbc2e8ad (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x1d7c8ad)
 #<!-- -->7 0x00007f1ecbc31b86 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x1d7fb86)
 #<!-- -->8 0x00007f1ecbc31733 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x1d7f733)
 #<!-- -->9 0x00007f1ecbc31624 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x1d7f624)
#<!-- -->10 0x00007f1ecbc0a346 clang::Sema::SubstFunctionDeclType(clang::TypeSourceInfo*, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceLocation, clang::DeclarationName, clang::CXXRecordDecl*, clang::Qualifiers, bool) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x1d58346)
#<!-- -->11 0x00007f1ecbc4ece4 clang::TemplateDeclInstantiator::SubstFunctionType(clang::FunctionDecl*, llvm::SmallVectorImpl&lt;clang::ParmVarDecl*&gt;&amp;) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x1d9cce4)
#<!-- -->12 0x00007f1ecbc4bbe1 clang::TemplateDeclInstantiator::VisitCXXMethodDecl(clang::CXXMethodDecl*, clang::TemplateParameterList*, clang::TemplateDeclInstantiator::RewriteKind) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x1d99be1)
#<!-- -->13 0x00007f1ecbc0d402 clang::Sema::InstantiateClass(clang::SourceLocation, clang::CXXRecordDecl*, clang::CXXRecordDecl*, clang::MultiLevelTemplateArgumentList const&amp;, clang::TemplateSpecializationKind, bool) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x1d5b402)
#<!-- -->14 0x00007f1ecbc0f306 clang::Sema::InstantiateClassTemplateSpecialization(clang::SourceLocation, clang::ClassTemplateSpecializationDecl*, clang::TemplateSpecializationKind, bool) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x1d5d306)
#<!-- -->15 0x00007f1ecbcc811e (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x1e1611e)
#<!-- -->16 0x00007f1ecb414c3d clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x1562c3d)
#<!-- -->17 0x00007f1ecbcb7fbf clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x1e05fbf)
#<!-- -->18 0x00007f1ecbcb786f clang::Sema::RequireCompleteType(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser&amp;) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x1e0586f)
#<!-- -->19 0x00007f1ecb4a6593 clang::Sema::RequireCompleteDeclContext(clang::CXXScopeSpec&amp;, clang::DeclContext*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x15f4593)
#<!-- -->20 0x00007f1ecb4a95b8 clang::Sema::ActOnCXXEnterDeclaratorScope(clang::Scope*, clang::CXXScopeSpec&amp;) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x15f75b8)
#<!-- -->21 0x00007f1ecab09ce0 clang::Parser::ParseDirectDeclarator(clang::Declarator&amp;) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0xc57ce0)
#<!-- -->22 0x00007f1ecab08b4a clang::Parser::ParseDeclaratorInternal(clang::Declarator&amp;, void (clang::Parser::*)(clang::Declarator&amp;)) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0xc56b4a)
#<!-- -->23 0x00007f1ecb414c3d clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x1562c3d)
#<!-- -->24 0x00007f1ecaaf7e14 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0xc45e14)
#<!-- -->25 0x00007f1ecabb6987 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0xd04987)
#<!-- -->26 0x00007f1ecabb633b clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0xd0433b)
#<!-- -->27 0x00007f1ecabb5547 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0xd03547)
#<!-- -->28 0x00007f1ecabb3a27 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0xd01a27)
#<!-- -->29 0x00007f1ecaae17de clang::ParseAST(clang::Sema&amp;, bool, bool) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0xc2f7de)
#<!-- -->30 0x00007f1eccb740b5 clang::FrontendAction::Execute() (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x2cc20b5)
#<!-- -->31 0x00007f1eccae42f4 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x2c322f4)
#<!-- -->32 0x00007f1eccbf0dee clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x2d3edee)
#<!-- -->33 0x00005635e849abc5 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/usr/lib/llvm-19/bin/clang+0x11bc5)
#<!-- -->34 0x00005635e8497ad5 (/usr/lib/llvm-19/bin/clang+0xead5)
#<!-- -->35 0x00005635e8496c9b clang_main(int, char**, llvm::ToolContext const&amp;) (/usr/lib/llvm-19/bin/clang+0xdc9b)
#<!-- -->36 0x00005635e84a4f36 main (/usr/lib/llvm-19/bin/clang+0x1bf36)
#<!-- -->37 0x00007f1ec1de6d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->38 0x00007f1ec1de6e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->39 0x00005635e8495765 _start (/usr/lib/llvm-19/bin/clang+0xc765)
clang-19: error: unable to execute command: Segmentation fault (core dumped)
clang-19: error: clang frontend command failed due to signal (use -v to see invocation)
Ubuntu clang version 19.0.0 (++20240722031324+65825cd5431c-1~exp1~20240722151445.1819)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/lib/llvm-19/bin
clang-19: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang-19: note: diagnostic msg: /tmp/mutant-6508f1.cpp
clang-19: note: diagnostic msg: /tmp/mutant-6508f1.sh
clang-19: note: diagnostic msg: 

********************
```
</details>


---

