# Clang crashes with "Fell off the end of a string-switch" assertion when using "sse4" in target_clones attribute

**Author:** AbyssStaror
**URL:** https://github.com/llvm/llvm-project/issues/172653

## Body

### Link for quick verification: https://godbolt.org/z/vY63h677j
```c
__attribute__((target_clones("sse4","default"))) void arch_dependent_func() {
    char v[16];
}
```
Go back to `clang 14.0.0 (assertions)` and crash both on C and C++.

## Stack dump
```
clang-21: /workspace/install/llvm/src/llvm-project/llvm/include/llvm/ADT/StringSwitch.h:184: llvm::StringSwitch<T, R>::operator R() [with T = llvm::X86::ProcessorFeatures; R = llvm::X86::ProcessorFeatures]: Assertion `Result && "Fell off the end of a string-switch"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /workspace/install/llvm/build_21.1.4/bin/clang-21 -cc1 -triple x86_64-unknown-linux-gnu -O3 -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -main-file-name 35998.fuzz -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=none -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/workspace/exps/featurefuzz/extracted_gcc_bug_cases -fcoverage-compilation-dir=/workspace/exps/featurefuzz/extracted_gcc_bug_cases -resource-dir /workspace/install/llvm/build_21.1.4/lib/clang/21 -internal-isystem /workspace/install/llvm/build_21.1.4/lib/clang/21/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -std=c2x -ferror-limit 19 -fmessage-length=131 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -vectorize-loops -vectorize-slp -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/35998-847250.o -x c /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_choose_2_5_exp/bugs/35998.fuzz
1.      /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_choose_2_5_exp/bugs/35998.fuzz:7:76: current parser token '{'
 #0 0x000055555ebe5c2a llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9691c2a)
 #1 0x000055555ebe6457 PrintStackTraceSignalHandler(void*) Signals.cpp:0:0
 #2 0x000055555ebdff57 llvm::sys::RunSignalHandlers() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x968bf57)
 #3 0x000055555ebe01c7 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #4 0x00007ffff7a3b330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #5 0x00007ffff7a94b2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #6 0x00007ffff7a94b2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #7 0x00007ffff7a94b2c pthread_kill ./nptl/pthread_kill.c:89:10
 #8 0x00007ffff7a3b27e raise ./signal/../sysdeps/posix/raise.c:27:6
 #9 0x00007ffff7a1e8ff abort ./stdlib/abort.c:81:7
#10 0x00007ffff7a1e81b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#11 0x00007ffff7a31517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#12 0x00005555646ccc79 getFeature(llvm::StringRef) X86.cpp:0:0
#13 0x00005555646ccd43 clang::targets::X86TargetInfo::getFMVPriority(llvm::ArrayRef<llvm::StringRef>) const (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xf178d43)
#14 0x00005555626be91b clang::Sema::checkTargetClonesAttrString(clang::SourceLocation, llvm::StringRef, clang::StringLiteral const*, clang::Decl*, bool&, bool&, bool&, llvm::SmallVectorImpl<llvm::SmallString<64u>>&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xd16a91b)
#15 0x00005555626bf1e6 handleTargetClonesAttr(clang::Sema&, clang::Decl*, clang::ParsedAttr const&) SemaDeclAttr.cpp:0:0
#16 0x00005555626c0622 ProcessDeclAttribute(clang::Sema&, clang::Scope*, clang::Decl*, clang::ParsedAttr const&, clang::Sema::ProcessDeclAttributeOptions const&) SemaDeclAttr.cpp:0:0
#17 0x00005555626c3648 clang::Sema::ProcessDeclAttributeList(clang::Scope*, clang::Decl*, clang::ParsedAttributesView const&, clang::Sema::ProcessDeclAttributeOptions const&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xd16f648)
#18 0x00005555626c5f5c clang::Sema::ProcessDeclAttributes(clang::Scope*, clang::Decl*, clang::Declarator const&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xd171f5c)
#19 0x00005555626509ca clang::Sema::ActOnFunctionDeclarator(clang::Scope*, clang::Declarator&, clang::DeclContext*, clang::TypeSourceInfo*, clang::LookupResult&, llvm::MutableArrayRef<clang::TemplateParameterList*>, bool&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xd0fc9ca)
#20 0x00005555626586d6 clang::Sema::HandleDeclarator(clang::Scope*, clang::Declarator&, llvm::MutableArrayRef<clang::TemplateParameterList*>) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xd1046d6)
#21 0x0000555562659487 clang::Sema::ActOnStartOfFunctionDef(clang::Scope*, clang::Declarator&, llvm::MutableArrayRef<clang::TemplateParameterList*>, clang::SkipBodyInfo*, clang::Sema::FnBodyKind) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xd105487)
#22 0x00005555620dacb7 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb86cb7)
#23 0x0000555562134d18 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcbe0d18)
#24 0x00005555620d1307 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb7d307)
#25 0x00005555620d1e9e clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb7de9e)
#26 0x00005555620ddf98 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb89f98)
#27 0x00005555620df59d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb8b59d)
#28 0x00005555620c59d0 clang::ParseAST(clang::Sema&, bool, bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb719d0)
#29 0x000055555fe0d92b clang::ASTFrontendAction::ExecuteAction() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa8b992b)
#30 0x000055555f9fb360 clang::CodeGenAction::ExecuteAction() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa4a7360)
#31 0x000055555fe159cf clang::FrontendAction::Execute() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa8c19cf)
#32 0x000055555fd5bf46 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa807f46)
#33 0x000055555ff9df7b clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xaa49f7b)
#34 0x0000555559d0c523 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47b8523)
#35 0x0000555559cfcc58 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#36 0x0000555559d02b28 clang_main(int, char**, llvm::ToolContext const&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47aeb28)
#37 0x0000555559b5db4a main (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x4609b4a)
#38 0x00007ffff7a201ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#39 0x00007ffff7a2028b call_init ./csu/../csu/libc-start.c:128:20
#40 0x00007ffff7a2028b __libc_start_main ./csu/../csu/libc-start.c:347:5
#41 0x0000555559cfaf55 _start (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47a6f55)
clang++: error: unable to execute command: Aborted (core dumped)
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (AbyssStaror)

<details>
### Link for quick verification: https://godbolt.org/z/vY63h677j
```c
__attribute__((target_clones("sse4","default"))) void arch_dependent_func() {
    char v[16];
}
```
Go back to `clang 14.0.0 (assertions)` and crash both on C and C++.

## Stack dump
```
clang-21: /workspace/install/llvm/src/llvm-project/llvm/include/llvm/ADT/StringSwitch.h:184: llvm::StringSwitch&lt;T, R&gt;::operator R() [with T = llvm::X86::ProcessorFeatures; R = llvm::X86::ProcessorFeatures]: Assertion `Result &amp;&amp; "Fell off the end of a string-switch"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /workspace/install/llvm/build_21.1.4/bin/clang-21 -cc1 -triple x86_64-unknown-linux-gnu -O3 -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -main-file-name 35998.fuzz -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=none -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/workspace/exps/featurefuzz/extracted_gcc_bug_cases -fcoverage-compilation-dir=/workspace/exps/featurefuzz/extracted_gcc_bug_cases -resource-dir /workspace/install/llvm/build_21.1.4/lib/clang/21 -internal-isystem /workspace/install/llvm/build_21.1.4/lib/clang/21/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -std=c2x -ferror-limit 19 -fmessage-length=131 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -vectorize-loops -vectorize-slp -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/35998-847250.o -x c /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_choose_2_5_exp/bugs/35998.fuzz
1.      /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_choose_2_5_exp/bugs/35998.fuzz:7:76: current parser token '{'
 #<!-- -->0 0x000055555ebe5c2a llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9691c2a)
 #<!-- -->1 0x000055555ebe6457 PrintStackTraceSignalHandler(void*) Signals.cpp:0:0
 #<!-- -->2 0x000055555ebdff57 llvm::sys::RunSignalHandlers() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x968bf57)
 #<!-- -->3 0x000055555ebe01c7 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->4 0x00007ffff7a3b330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->5 0x00007ffff7a94b2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->6 0x00007ffff7a94b2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->7 0x00007ffff7a94b2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->8 0x00007ffff7a3b27e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x00007ffff7a1e8ff abort ./stdlib/abort.c:81:7
#<!-- -->10 0x00007ffff7a1e81b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->11 0x00007ffff7a31517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->12 0x00005555646ccc79 getFeature(llvm::StringRef) X86.cpp:0:0
#<!-- -->13 0x00005555646ccd43 clang::targets::X86TargetInfo::getFMVPriority(llvm::ArrayRef&lt;llvm::StringRef&gt;) const (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xf178d43)
#<!-- -->14 0x00005555626be91b clang::Sema::checkTargetClonesAttrString(clang::SourceLocation, llvm::StringRef, clang::StringLiteral const*, clang::Decl*, bool&amp;, bool&amp;, bool&amp;, llvm::SmallVectorImpl&lt;llvm::SmallString&lt;64u&gt;&gt;&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xd16a91b)
#<!-- -->15 0x00005555626bf1e6 handleTargetClonesAttr(clang::Sema&amp;, clang::Decl*, clang::ParsedAttr const&amp;) SemaDeclAttr.cpp:0:0
#<!-- -->16 0x00005555626c0622 ProcessDeclAttribute(clang::Sema&amp;, clang::Scope*, clang::Decl*, clang::ParsedAttr const&amp;, clang::Sema::ProcessDeclAttributeOptions const&amp;) SemaDeclAttr.cpp:0:0
#<!-- -->17 0x00005555626c3648 clang::Sema::ProcessDeclAttributeList(clang::Scope*, clang::Decl*, clang::ParsedAttributesView const&amp;, clang::Sema::ProcessDeclAttributeOptions const&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xd16f648)
#<!-- -->18 0x00005555626c5f5c clang::Sema::ProcessDeclAttributes(clang::Scope*, clang::Decl*, clang::Declarator const&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xd171f5c)
#<!-- -->19 0x00005555626509ca clang::Sema::ActOnFunctionDeclarator(clang::Scope*, clang::Declarator&amp;, clang::DeclContext*, clang::TypeSourceInfo*, clang::LookupResult&amp;, llvm::MutableArrayRef&lt;clang::TemplateParameterList*&gt;, bool&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xd0fc9ca)
#<!-- -->20 0x00005555626586d6 clang::Sema::HandleDeclarator(clang::Scope*, clang::Declarator&amp;, llvm::MutableArrayRef&lt;clang::TemplateParameterList*&gt;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xd1046d6)
#<!-- -->21 0x0000555562659487 clang::Sema::ActOnStartOfFunctionDef(clang::Scope*, clang::Declarator&amp;, llvm::MutableArrayRef&lt;clang::TemplateParameterList*&gt;, clang::SkipBodyInfo*, clang::Sema::FnBodyKind) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xd105487)
#<!-- -->22 0x00005555620dacb7 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb86cb7)
#<!-- -->23 0x0000555562134d18 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcbe0d18)
#<!-- -->24 0x00005555620d1307 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb7d307)
#<!-- -->25 0x00005555620d1e9e clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb7de9e)
#<!-- -->26 0x00005555620ddf98 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb89f98)
#<!-- -->27 0x00005555620df59d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb8b59d)
#<!-- -->28 0x00005555620c59d0 clang::ParseAST(clang::Sema&amp;, bool, bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb719d0)
#<!-- -->29 0x000055555fe0d92b clang::ASTFrontendAction::ExecuteAction() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa8b992b)
#<!-- -->30 0x000055555f9fb360 clang::CodeGenAction::ExecuteAction() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa4a7360)
#<!-- -->31 0x000055555fe159cf clang::FrontendAction::Execute() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa8c19cf)
#<!-- -->32 0x000055555fd5bf46 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa807f46)
#<!-- -->33 0x000055555ff9df7b clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xaa49f7b)
#<!-- -->34 0x0000555559d0c523 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47b8523)
#<!-- -->35 0x0000555559cfcc58 ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->36 0x0000555559d02b28 clang_main(int, char**, llvm::ToolContext const&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47aeb28)
#<!-- -->37 0x0000555559b5db4a main (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x4609b4a)
#<!-- -->38 0x00007ffff7a201ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->39 0x00007ffff7a2028b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->40 0x00007ffff7a2028b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->41 0x0000555559cfaf55 _start (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47a6f55)
clang++: error: unable to execute command: Aborted (core dumped)
```
</details>


---

