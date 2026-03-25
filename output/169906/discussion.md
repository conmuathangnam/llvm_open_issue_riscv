# [clang]: ICE in SemaTemplate.cpp on Invalid Using Declaration

**Author:** AbyssStaror
**URL:** https://github.com/llvm/llvm-project/issues/169906

## Body

### Link for quick verification: https://godbolt.org/z/sv76zbzYh

```c
template<int _Tp>
struct _List_node {
    virtual ~_List_node() {}
    _List_node<_Tp>* next;
};

struct _Base : public virtual _List_node<0> {};

struct list : public virtual _Base {
    using _Base::_List_node;

    void traverse(_List_node<_Tp>* start, _List_node<_Tp>* end) {
        while (start != end && start->next != nullptr) {
            start = start->next;
        }
    }
};
```
There are two issues in the code:

- The template parameter **_Tp** is not defined in the listclass.

- The **using** declaration **using _Base::_List_node;** is incorrect because **_Base** does not have a member named **_List_node**.  

However, instead of reporting errors, the compiler crashes with an assertion failure. I expect the compiler to report appropriate error messages rather than crashing.

## The stack dump: 
```
0.      Program arguments: /workspace/install/llvm/build_21.1.4/bin/clang-21 -cc1 -triple x86_64-unknown-linux-gnu -O3 -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -main-file-name 31501.fuzz -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=none -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/workspace/exps -fcoverage-compilation-dir=/workspace/exps -resource-dir /workspace/install/llvm/build_21.1.4/lib/clang/21 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/x86_64-linux-gnu/c++/13 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/backward -internal-isystem /workspace/install/llvm/build_21.1.4/lib/clang/21/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -std=c++20 -fdeprecated-macro -ferror-limit 19 -fmessage-length=153 -fgnuc-version=4.2.1 -fno-implicit-modules -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -vectorize-loops -vectorize-slp -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/31501-b08461.o -x c++ /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_choose_3_1_exp/cases/31501.fuzz
1.      /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_choose_3_1_exp/cases/31501.fuzz:18:29: current parser token '_List_node'
2.      /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_choose_3_1_exp/cases/31501.fuzz:14:1: parsing struct/union/class body 'list'
 #0 0x00006018b5ca1c2a llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9691c2a)
 #1 0x00006018b5ca2457 PrintStackTraceSignalHandler(void*) Signals.cpp:0:0
 #2 0x00006018b5c9bf57 llvm::sys::RunSignalHandlers() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x968bf57)
 #3 0x00006018b5c9c1c7 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #4 0x00007fe185fd3330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #5 0x00007fe18602cb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #6 0x00007fe18602cb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #7 0x00007fe18602cb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #8 0x00007fe185fd327e raise ./signal/../sysdeps/posix/raise.c:27:6
 #9 0x00007fe185fb68ff abort ./stdlib/abort.c:81:7
#10 0x00007fe185fb681b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#11 0x00007fe185fc9517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#12 0x00006018ba025507 clang::Sema::isTemplateName(clang::Scope*, clang::CXXScopeSpec&, bool, clang::UnqualifiedId const&, clang::OpaquePtr<clang::QualType>, bool, clang::OpaquePtr<clang::TemplateName>&, bool&, bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xda15507)
#13 0x00006018b924e73a clang::Parser::ParseOptionalCXXScopeSpecifier(clang::CXXScopeSpec&, clang::OpaquePtr<clang::QualType>, bool, bool, bool*, bool, clang::IdentifierInfo const**, bool, bool, bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc3e73a)
#14 0x00006018b918c432 clang::Parser::TryAnnotateCXXScopeToken(bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb7c432)
#15 0x00006018b91e249a clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcbd249a)
#16 0x00006018b91f7337 clang::Parser::ParseParameterDeclarationClause(clang::DeclaratorContext, clang::ParsedAttributes&, llvm::SmallVectorImpl<clang::DeclaratorChunk::ParamInfo>&, clang::SourceLocation&, bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcbe7337)
#17 0x00006018b91f9a65 clang::Parser::ParseFunctionDeclarator(clang::Declarator&, clang::ParsedAttributes&, clang::BalancedDelimiterTracker&, bool, bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcbe9a65)
#18 0x00006018b91fbb0f clang::Parser::ParseDirectDeclarator(clang::Declarator&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcbebb0f)
#19 0x00006018b91f36d3 clang::Parser::ParseDeclaratorInternal(clang::Declarator&, void (clang::Parser::*)(clang::Declarator&)) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcbe36d3)
#20 0x00006018b91f4c88 void llvm::function_ref<void ()>::callback_fn<clang::Parser::ParseDeclarator(clang::Declarator&)::'lambda'()>(long) ParseDecl.cpp:0:0
#21 0x00006018bb6b542b clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xf0a542b)
#22 0x00006018b93b09cc clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcda09cc)
#23 0x00006018b91c8b02 clang::Parser::ParseDeclarator(clang::Declarator&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcbb8b02)
#24 0x00006018b920a57d clang::Parser::ParseCXXMemberDeclaratorBeforeInitializer(clang::Declarator&, clang::VirtSpecifiers&, clang::ActionResult<clang::Expr*, true>&, clang::Parser::LateParsedAttrList&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcbfa57d)
#25 0x00006018b921bba0 clang::Parser::ParseCXXClassMemberDeclaration(clang::AccessSpecifier, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::ParsingDeclRAIIObject*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc0bba0)
#26 0x00006018b921e137 clang::Parser::ParseCXXClassMemberDeclarationWithPragmas(clang::AccessSpecifier&, clang::ParsedAttributes&, clang::TypeSpecifierType, clang::Decl*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc0e137)
#27 0x00006018b9220599 clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&, unsigned int, clang::Decl*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc10599)
#28 0x00006018b9224dd7 clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc14dd7)
#29 0x00006018b91db86d clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcbcb86d)
#30 0x00006018b918cfbf clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb7cfbf)
#31 0x00006018b918de9e clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb7de9e)
#32 0x00006018b9199f98 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb89f98)
#33 0x00006018b919b59d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb8b59d)
#34 0x00006018b91819d0 clang::ParseAST(clang::Sema&, bool, bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb719d0)
#35 0x00006018b6ec992b clang::ASTFrontendAction::ExecuteAction() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa8b992b)
#36 0x00006018b6ab7360 clang::CodeGenAction::ExecuteAction() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa4a7360)
#37 0x00006018b6ed19cf clang::FrontendAction::Execute() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa8c19cf)
#38 0x00006018b6e17f46 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa807f46)
#39 0x00006018b7059f7b clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xaa49f7b)
#40 0x00006018b0dc8523 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47b8523)
#41 0x00006018b0db8c58 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#42 0x00006018b0dbeb28 clang_main(int, char**, llvm::ToolContext const&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47aeb28)
#43 0x00006018b0c19b4a main (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x4609b4a)
#44 0x00007fe185fb81ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#45 0x00007fe185fb828b call_init ./csu/../csu/libc-start.c:128:20
#46 0x00007fe185fb828b __libc_start_main ./csu/../csu/libc-start.c:347:5
#47 0x00006018b0db6f55 _start (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47a6f55)
clang++: error: unable to execute command: Aborted (core dumped)
```

## The clang version:
```
clang version 21.1.4 (https://github.com/llvm/llvm-project.git 222fc11f2b8f25f6a0f4976272ef1bb7bf49521d)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /workspace/install/llvm/build_21.1.4/bin
Build config: +assertions
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/13
Selected GCC installation: /usr/lib/gcc/x86_64-linux-gnu/13
Candidate multilib: .;@m64
Selected multilib: .;@m64
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (AbyssStaror)

<details>
### Link for quick verification: https://godbolt.org/z/sv76zbzYh

```c
template&lt;int _Tp&gt;
struct _List_node {
    virtual ~_List_node() {}
    _List_node&lt;_Tp&gt;* next;
};

struct _Base : public virtual _List_node&lt;0&gt; {};

struct list : public virtual _Base {
    using _Base::_List_node;

    void traverse(_List_node&lt;_Tp&gt;* start, _List_node&lt;_Tp&gt;* end) {
        while (start != end &amp;&amp; start-&gt;next != nullptr) {
            start = start-&gt;next;
        }
    }
};
```
There are two issues in the code:

- The template parameter **_Tp** is not defined in the listclass.

- The **using** declaration **using _Base::_List_node;** is incorrect because **_Base** does not have a member named **_List_node**.  

However, instead of reporting errors, the compiler crashes with an assertion failure. I expect the compiler to report appropriate error messages rather than crashing.

## The stack dump: 
```
0.      Program arguments: /workspace/install/llvm/build_21.1.4/bin/clang-21 -cc1 -triple x86_64-unknown-linux-gnu -O3 -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -main-file-name 31501.fuzz -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=none -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/workspace/exps -fcoverage-compilation-dir=/workspace/exps -resource-dir /workspace/install/llvm/build_21.1.4/lib/clang/21 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/x86_64-linux-gnu/c++/13 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/backward -internal-isystem /workspace/install/llvm/build_21.1.4/lib/clang/21/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -std=c++20 -fdeprecated-macro -ferror-limit 19 -fmessage-length=153 -fgnuc-version=4.2.1 -fno-implicit-modules -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -vectorize-loops -vectorize-slp -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/31501-b08461.o -x c++ /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_choose_3_1_exp/cases/31501.fuzz
1.      /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_choose_3_1_exp/cases/31501.fuzz:18:29: current parser token '_List_node'
2.      /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_choose_3_1_exp/cases/31501.fuzz:14:1: parsing struct/union/class body 'list'
 #<!-- -->0 0x00006018b5ca1c2a llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9691c2a)
 #<!-- -->1 0x00006018b5ca2457 PrintStackTraceSignalHandler(void*) Signals.cpp:0:0
 #<!-- -->2 0x00006018b5c9bf57 llvm::sys::RunSignalHandlers() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x968bf57)
 #<!-- -->3 0x00006018b5c9c1c7 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->4 0x00007fe185fd3330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->5 0x00007fe18602cb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->6 0x00007fe18602cb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->7 0x00007fe18602cb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->8 0x00007fe185fd327e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x00007fe185fb68ff abort ./stdlib/abort.c:81:7
#<!-- -->10 0x00007fe185fb681b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->11 0x00007fe185fc9517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->12 0x00006018ba025507 clang::Sema::isTemplateName(clang::Scope*, clang::CXXScopeSpec&amp;, bool, clang::UnqualifiedId const&amp;, clang::OpaquePtr&lt;clang::QualType&gt;, bool, clang::OpaquePtr&lt;clang::TemplateName&gt;&amp;, bool&amp;, bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xda15507)
#<!-- -->13 0x00006018b924e73a clang::Parser::ParseOptionalCXXScopeSpecifier(clang::CXXScopeSpec&amp;, clang::OpaquePtr&lt;clang::QualType&gt;, bool, bool, bool*, bool, clang::IdentifierInfo const**, bool, bool, bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc3e73a)
#<!-- -->14 0x00006018b918c432 clang::Parser::TryAnnotateCXXScopeToken(bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb7c432)
#<!-- -->15 0x00006018b91e249a clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcbd249a)
#<!-- -->16 0x00006018b91f7337 clang::Parser::ParseParameterDeclarationClause(clang::DeclaratorContext, clang::ParsedAttributes&amp;, llvm::SmallVectorImpl&lt;clang::DeclaratorChunk::ParamInfo&gt;&amp;, clang::SourceLocation&amp;, bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcbe7337)
#<!-- -->17 0x00006018b91f9a65 clang::Parser::ParseFunctionDeclarator(clang::Declarator&amp;, clang::ParsedAttributes&amp;, clang::BalancedDelimiterTracker&amp;, bool, bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcbe9a65)
#<!-- -->18 0x00006018b91fbb0f clang::Parser::ParseDirectDeclarator(clang::Declarator&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcbebb0f)
#<!-- -->19 0x00006018b91f36d3 clang::Parser::ParseDeclaratorInternal(clang::Declarator&amp;, void (clang::Parser::*)(clang::Declarator&amp;)) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcbe36d3)
#<!-- -->20 0x00006018b91f4c88 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::Parser::ParseDeclarator(clang::Declarator&amp;)::'lambda'()&gt;(long) ParseDecl.cpp:0:0
#<!-- -->21 0x00006018bb6b542b clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xf0a542b)
#<!-- -->22 0x00006018b93b09cc clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcda09cc)
#<!-- -->23 0x00006018b91c8b02 clang::Parser::ParseDeclarator(clang::Declarator&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcbb8b02)
#<!-- -->24 0x00006018b920a57d clang::Parser::ParseCXXMemberDeclaratorBeforeInitializer(clang::Declarator&amp;, clang::VirtSpecifiers&amp;, clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, clang::Parser::LateParsedAttrList&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcbfa57d)
#<!-- -->25 0x00006018b921bba0 clang::Parser::ParseCXXClassMemberDeclaration(clang::AccessSpecifier, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::ParsingDeclRAIIObject*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc0bba0)
#<!-- -->26 0x00006018b921e137 clang::Parser::ParseCXXClassMemberDeclarationWithPragmas(clang::AccessSpecifier&amp;, clang::ParsedAttributes&amp;, clang::TypeSpecifierType, clang::Decl*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc0e137)
#<!-- -->27 0x00006018b9220599 clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&amp;, unsigned int, clang::Decl*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc10599)
#<!-- -->28 0x00006018b9224dd7 clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc14dd7)
#<!-- -->29 0x00006018b91db86d clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcbcb86d)
#<!-- -->30 0x00006018b918cfbf clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb7cfbf)
#<!-- -->31 0x00006018b918de9e clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb7de9e)
#<!-- -->32 0x00006018b9199f98 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb89f98)
#<!-- -->33 0x00006018b919b59d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb8b59d)
#<!-- -->34 0x00006018b91819d0 clang::ParseAST(clang::Sema&amp;, bool, bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb719d0)
#<!-- -->35 0x00006018b6ec992b clang::ASTFrontendAction::ExecuteAction() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa8b992b)
#<!-- -->36 0x00006018b6ab7360 clang::CodeGenAction::ExecuteAction() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa4a7360)
#<!-- -->37 0x00006018b6ed19cf clang::FrontendAction::Execute() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa8c19cf)
#<!-- -->38 0x00006018b6e17f46 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa807f46)
#<!-- -->39 0x00006018b7059f7b clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xaa49f7b)
#<!-- -->40 0x00006018b0dc8523 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47b8523)
#<!-- -->41 0x00006018b0db8c58 ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->42 0x00006018b0dbeb28 clang_main(int, char**, llvm::ToolContext const&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47aeb28)
#<!-- -->43 0x00006018b0c19b4a main (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x4609b4a)
#<!-- -->44 0x00007fe185fb81ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->45 0x00007fe185fb828b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->46 0x00007fe185fb828b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->47 0x00006018b0db6f55 _start (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47a6f55)
clang++: error: unable to execute command: Aborted (core dumped)
```

## The clang version:
```
clang version 21.1.4 (https://github.com/llvm/llvm-project.git 222fc11f2b8f25f6a0f4976272ef1bb7bf49521d)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /workspace/install/llvm/build_21.1.4/bin
Build config: +assertions
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/13
Selected GCC installation: /usr/lib/gcc/x86_64-linux-gnu/13
Candidate multilib: .;@<!-- -->m64
Selected multilib: .;@<!-- -->m64
```
</details>


---

