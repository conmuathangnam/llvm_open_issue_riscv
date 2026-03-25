# [clang] Assertion failure in getASTRecordLayout when declaring global register variable with incomplete struct type

**Author:** AbyssStaror
**URL:** https://github.com/llvm/llvm-project/issues/169792
**Status:** Closed
**Labels:** duplicate, clang:frontend, crash-on-invalid, inline-asm
**Closed Date:** 2025-12-09T03:28:42Z

## Body

### Link for quick verification: https://godbolt.org/z/31cxdbbbM
When I used the Global register variable and struct forward declaration, the compiler went crash.
```c
register struct _ts a __asm__("r13");

struct _ts {
    int tss_dict;
};
```
## The stack dump:
```
0.      Program arguments: /workspace/install/llvm/build_21.1.4/bin/clang-21 -cc1 -triple x86_64-unknown-linux-gnu -O3 -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -main-file-name 11504.fuzz -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=none -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/workspace/exps -fcoverage-compilation-dir=/workspace/exps -resource-dir /workspace/install/llvm/build_21.1.4/lib/clang/21 -internal-isystem /workspace/install/llvm/build_21.1.4/lib/clang/21/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -ferror-limit 19 -fmessage-length=138 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -vectorize-loops -vectorize-slp -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/11504-c502ba.o -x c /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_choose_3_1_exp/cases/11504.fuzz
1.      /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_choose_3_1_exp/cases/11504.fuzz:1:50: current parser token ';'
 #0 0x00005e8661cc4c2a llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9691c2a)
 #1 0x00005e8661cc5457 PrintStackTraceSignalHandler(void*) Signals.cpp:0:0
 #2 0x00005e8661cbef57 llvm::sys::RunSignalHandlers() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x968bf57)
 #3 0x00005e8661cbf1c7 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #4 0x000070751a80d330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #5 0x000070751a866b2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #6 0x000070751a866b2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #7 0x000070751a866b2c pthread_kill ./nptl/pthread_kill.c:89:10
 #8 0x000070751a80d27e raise ./signal/../sysdeps/posix/raise.c:27:6
 #9 0x000070751a7f08ff abort ./stdlib/abort.c:81:7
#10 0x000070751a7f081b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#11 0x000070751a803517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#12 0x00005e86672777a7 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xec447a7)
#13 0x00005e86668401de clang::ASTContext::getTypeInfoImpl(clang::Type const*) const (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xe20d1de)
#14 0x00005e86668022e4 clang::ASTContext::getTypeInfo(clang::Type const*) const (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xe1cf2e4)
#15 0x00005e866683f043 clang::ASTContext::getTypeInfoImpl(clang::Type const*) const (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xe20c043)
#16 0x00005e86668022e4 clang::ASTContext::getTypeInfo(clang::Type const*) const (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xe1cf2e4)
#17 0x00005e8662102839 clang::ASTContext::getTypeSize(clang::QualType) const (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9acf839)
#18 0x00005e8665716d25 clang::Sema::ActOnVariableDeclarator(clang::Scope*, clang::Declarator&, clang::DeclContext*, clang::TypeSourceInfo*, clang::LookupResult&, llvm::MutableArrayRef<clang::TemplateParameterList*>, bool&, llvm::ArrayRef<clang::BindingDecl*>) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xd0e3d25)
#19 0x00005e86657378ae clang::Sema::HandleDeclarator(clang::Scope*, clang::Declarator&, llvm::MutableArrayRef<clang::TemplateParameterList*>) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xd1048ae)
#20 0x00005e8665738743 clang::Sema::ActOnDeclarator(clang::Scope*, clang::Declarator&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xd105743)
#21 0x00005e86651f0edf clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcbbdedf)
#22 0x00005e8665212d1b clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcbdfd1b)
#23 0x00005e86651b0307 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb7d307)
#24 0x00005e86651b0e9e clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb7de9e)
#25 0x00005e86651bcf98 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb89f98)
#26 0x00005e86651be59d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb8b59d)
#27 0x00005e86651beb63 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb8bb63)
#28 0x00005e86651a498c clang::ParseAST(clang::Sema&, bool, bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb7198c)
#29 0x00005e8662eec92b clang::ASTFrontendAction::ExecuteAction() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa8b992b)
#30 0x00005e8662ada360 clang::CodeGenAction::ExecuteAction() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa4a7360)
#31 0x00005e8662ef49cf clang::FrontendAction::Execute() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa8c19cf)
#32 0x00005e8662e3af46 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa807f46)
#33 0x00005e866307cf7b clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xaa49f7b)
#34 0x00005e865cdeb523 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47b8523)
#35 0x00005e865cddbc58 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#36 0x00005e865cde1b28 clang_main(int, char**, llvm::ToolContext const&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47aeb28)
#37 0x00005e865cc3cb4a main (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x4609b4a)
#38 0x000070751a7f21ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#39 0x000070751a7f228b call_init ./csu/../csu/libc-start.c:128:20
#40 0x000070751a7f228b __libc_start_main ./csu/../csu/libc-start.c:347:5
#41 0x00005e865cdd9f55 _start (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47a6f55)
clang: error: unable to execute command: Aborted (core dumped)
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

### Comment 1 - frederick-vs-ja

Regression since 3.8.

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (AbyssStaror)

<details>
### Link for quick verification: https://godbolt.org/z/31cxdbbbM
When I used the Global register variable and struct forward declaration, the compiler went crash.
```c
register struct _ts a __asm__("r13");

struct _ts {
    int tss_dict;
};
```
## The stack dump:
```
0.      Program arguments: /workspace/install/llvm/build_21.1.4/bin/clang-21 -cc1 -triple x86_64-unknown-linux-gnu -O3 -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -main-file-name 11504.fuzz -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=none -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/workspace/exps -fcoverage-compilation-dir=/workspace/exps -resource-dir /workspace/install/llvm/build_21.1.4/lib/clang/21 -internal-isystem /workspace/install/llvm/build_21.1.4/lib/clang/21/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -ferror-limit 19 -fmessage-length=138 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -vectorize-loops -vectorize-slp -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/11504-c502ba.o -x c /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_choose_3_1_exp/cases/11504.fuzz
1.      /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_choose_3_1_exp/cases/11504.fuzz:1:50: current parser token ';'
 #<!-- -->0 0x00005e8661cc4c2a llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9691c2a)
 #<!-- -->1 0x00005e8661cc5457 PrintStackTraceSignalHandler(void*) Signals.cpp:0:0
 #<!-- -->2 0x00005e8661cbef57 llvm::sys::RunSignalHandlers() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x968bf57)
 #<!-- -->3 0x00005e8661cbf1c7 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->4 0x000070751a80d330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->5 0x000070751a866b2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->6 0x000070751a866b2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->7 0x000070751a866b2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->8 0x000070751a80d27e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x000070751a7f08ff abort ./stdlib/abort.c:81:7
#<!-- -->10 0x000070751a7f081b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->11 0x000070751a803517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->12 0x00005e86672777a7 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xec447a7)
#<!-- -->13 0x00005e86668401de clang::ASTContext::getTypeInfoImpl(clang::Type const*) const (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xe20d1de)
#<!-- -->14 0x00005e86668022e4 clang::ASTContext::getTypeInfo(clang::Type const*) const (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xe1cf2e4)
#<!-- -->15 0x00005e866683f043 clang::ASTContext::getTypeInfoImpl(clang::Type const*) const (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xe20c043)
#<!-- -->16 0x00005e86668022e4 clang::ASTContext::getTypeInfo(clang::Type const*) const (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xe1cf2e4)
#<!-- -->17 0x00005e8662102839 clang::ASTContext::getTypeSize(clang::QualType) const (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9acf839)
#<!-- -->18 0x00005e8665716d25 clang::Sema::ActOnVariableDeclarator(clang::Scope*, clang::Declarator&amp;, clang::DeclContext*, clang::TypeSourceInfo*, clang::LookupResult&amp;, llvm::MutableArrayRef&lt;clang::TemplateParameterList*&gt;, bool&amp;, llvm::ArrayRef&lt;clang::BindingDecl*&gt;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xd0e3d25)
#<!-- -->19 0x00005e86657378ae clang::Sema::HandleDeclarator(clang::Scope*, clang::Declarator&amp;, llvm::MutableArrayRef&lt;clang::TemplateParameterList*&gt;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xd1048ae)
#<!-- -->20 0x00005e8665738743 clang::Sema::ActOnDeclarator(clang::Scope*, clang::Declarator&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xd105743)
#<!-- -->21 0x00005e86651f0edf clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcbbdedf)
#<!-- -->22 0x00005e8665212d1b clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcbdfd1b)
#<!-- -->23 0x00005e86651b0307 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb7d307)
#<!-- -->24 0x00005e86651b0e9e clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb7de9e)
#<!-- -->25 0x00005e86651bcf98 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb89f98)
#<!-- -->26 0x00005e86651be59d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb8b59d)
#<!-- -->27 0x00005e86651beb63 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb8bb63)
#<!-- -->28 0x00005e86651a498c clang::ParseAST(clang::Sema&amp;, bool, bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb7198c)
#<!-- -->29 0x00005e8662eec92b clang::ASTFrontendAction::ExecuteAction() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa8b992b)
#<!-- -->30 0x00005e8662ada360 clang::CodeGenAction::ExecuteAction() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa4a7360)
#<!-- -->31 0x00005e8662ef49cf clang::FrontendAction::Execute() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa8c19cf)
#<!-- -->32 0x00005e8662e3af46 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa807f46)
#<!-- -->33 0x00005e866307cf7b clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xaa49f7b)
#<!-- -->34 0x00005e865cdeb523 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47b8523)
#<!-- -->35 0x00005e865cddbc58 ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->36 0x00005e865cde1b28 clang_main(int, char**, llvm::ToolContext const&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47aeb28)
#<!-- -->37 0x00005e865cc3cb4a main (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x4609b4a)
#<!-- -->38 0x000070751a7f21ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->39 0x000070751a7f228b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->40 0x000070751a7f228b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->41 0x00005e865cdd9f55 _start (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47a6f55)
clang: error: unable to execute command: Aborted (core dumped)
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

### Comment 3 - shafik

You need to check for duplicates, especially w/ fuzzer generated bugs.

---

### Comment 4 - AbyssStaror

> You need to check for duplicates, especially w/ fuzzer generated bugs.

I'm sorry for the duplicate and I'll make carefully verification before submitting any future bug reports.

---

