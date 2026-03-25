# Clang segfault in clang::Sema::PushCompoundScope

**Author:** melver
**URL:** https://github.com/llvm/llvm-project/issues/129873

## Body

Clang crashes in clang::Sema::PushCompoundScope:

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /opt/llvm/bin/clang -Wp,-MMD,kernel/sched/.core.o.d -nostdinc -I../arch/x86/include -I./arch/x86/include/generated -I../include -I./include -I../arch/x86/include/uapi -I./arch/x86/include/generated/uapi -I../include/uapi -I./include/generated/uapi -include ../include/linux/compiler-version.h -include ../include/linux/kconfig.h -include ../include/linux/compiler_types.h -D__KERNEL__ --target=x86_64-linux-gnu -fintegrated-as -Werror=unknown-warning-option -Werror=ignored-optimization-argument -Werror=option-ignored -Werror=unused-command-line-argument -fmacro-prefix-map=../= -std=gnu11 -fshort-wchar -funsigned-char -fno-common -fno-PIE -fno-strict-aliasing -mno-sse -mno-mmx -mno-sse2 -mno-3dnow -mno-avx -fcf-protection=branch -fno-jump-tables -m64 -falign-loops=1 -mno-80387 -mno-fp-ret-in-387 -mstack-alignment=8 -mskip-rax-setup -march=core2 -mno-red-zone -mcmodel=kernel -Wno-sign-compare -fno-asynchronous-unwind-tables -mretpoline-external-thunk -mindirect-branch-cs-prefix -mfunction-return=thunk-extern -fpatchable-function-entry=11,11 -fno-delete-null-pointer-checks -O2 -fstack-protector-strong -fno-stack-clash-protection -pg -mfentry -DCC_USING_NOP_MCOUNT -DCC_USING_FENTRY -fsanitize=kcfi -falign-functions=16 -fstrict-flex-arrays=3 -fno-strict-overflow -fno-stack-check -Wall -Wundef -Werror=implicit-function-declaration -Werror=implicit-int -Werror=return-type -Werror=strict-prototypes -Wno-format-security -Wno-trigraphs -Wno-frame-address -Wno-address-of-packed-member -Wmissing-declarations -Wmissing-prototypes -Wframe-larger-than=2048 -Wno-gnu -Wno-format-overflow-non-kprintf -Wno-format-truncation-non-kprintf -Wvla -Wno-pointer-sign -Wcast-function-type -Wimplicit-fallthrough -Werror=date-time -Werror=incompatible-pointer-types -Wenum-conversion -Wextra -Wunused -Wno-unused-but-set-variable -Wno-unused-const-variable -Wno-format-overflow -Wno-override-init -Wno-pointer-to-enum-cast -Wno-tautological-constant-out-of-range-compare -Wno-unaligned-access -Wno-enum-compare-conditional -Wno-missing-field-initializers -Wno-type-limits -Wno-shift-negative-value -Wno-enum-enum-conversion -Wno-sign-compare -Wno-unused-parameter -g -Wno-unused-but-set-variable -DWARN_CAPABILITY_ANALYSIS -fexperimental-late-parse-attributes -Wthread-safety -I../kernel/sched -Ikernel/sched -DKBUILD_MODFILE=\"kernel/sched/core\" -DKBUILD_BASENAME=\"core\" -DKBUILD_MODNAME=\"core\" -D__KBUILD_MODNAME=kmod_core -c -o kernel/sched/core.o ../kernel/sched/core.c
1.      ../kernel/sched/sched.h:1816:2 <Spelling=../include/linux/percpu-defs.h:217:37>: current parser token '\
do'
2.      ../kernel/sched/sched.h:1816:2 <Spelling=../include/linux/percpu-defs.h:236:2>: in compound statement ('{}')
 #0 0x000055e362aee5e8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/llvm/bin/clang+0x43435e8)
 #1 0x000055e362aec2ee llvm::sys::RunSignalHandlers() (/opt/llvm/bin/clang+0x43412ee)
 #2 0x000055e362a7ca49 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007f6e08049d20 (/lib/x86_64-linux-gnu/libc.so.6+0x3fd20)
 #4 0x000055e36467d629 clang::Sema::PushCompoundScope(bool) (/opt/llvm/bin/clang+0x5ed2629)
 #5 0x000055e3645f9fee clang::Parser::ParseCompoundStatementBody(bool) (/opt/llvm/bin/clang+0x5e4efee)
 #6 0x000055e3645f5a0d clang::Parser::ParseCompoundStatement(bool) (/opt/llvm/bin/clang+0x5e4aa0d)
 #7 0x000055e36458a673 clang::Parser::ParseParenExpression(clang::Parser::ParenParseOption&, bool, bool, clang::OpaquePtr<clang::QualType>&, clang::SourceLocation&) (/opt/llvm/bin/clang+0x5ddf673)
 #8 0x000055e364587c6e clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) (/opt/llvm/bin/clang+0x5ddcc6e)
 #9 0x000055e364586c6c clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) (/opt/llvm/bin/clang+0x5ddbc6c)
#10 0x000055e364583278 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/opt/llvm/bin/clang+0x5dd8278)
#11 0x000055e36458ad15 clang::Parser::ParseParenExpression(clang::Parser::ParenParseOption&, bool, bool, clang::OpaquePtr<clang::QualType>&, clang::SourceLocation&) (/opt/llvm/bin/clang+0x5ddfd15)
#12 0x000055e364587c6e clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) (/opt/llvm/bin/clang+0x5ddcc6e)
#13 0x000055e364584581 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) (/opt/llvm/bin/clang+0x5dd9581)
#14 0x000055e364587b9f clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) (/opt/llvm/bin/clang+0x5ddcb9f)
#15 0x000055e364583278 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/opt/llvm/bin/clang+0x5dd8278)
#16 0x000055e36458ad15 clang::Parser::ParseParenExpression(clang::Parser::ParenParseOption&, bool, bool, clang::OpaquePtr<clang::QualType>&, clang::SourceLocation&) (/opt/llvm/bin/clang+0x5ddfd15)
#17 0x000055e364587c6e clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) (/opt/llvm/bin/clang+0x5ddcc6e)
#18 0x000055e364583278 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/opt/llvm/bin/clang+0x5dd8278)
#19 0x000055e3645b8197 clang::Parser::ParseAttributeArgumentList(clang::IdentifierInfo const&, llvm::SmallVectorImpl<clang::Expr*>&, clang::ParsedAttributeArgumentsProperties) (/opt/llvm/bin/clang+0x5e0d197)
#20 0x000055e3645ba694 clang::Parser::ParseAttributeArgsCommon(clang::IdentifierInfo*, clang::SourceLocation, clang::ParsedAttributes&, clang::SourceLocation*, clang::IdentifierInfo*, clang::SourceLocation, clang::AttributeCommonInfo::Form) (/opt/llvm/bin/clang+0x5e0f694)
#21 0x000055e3645b776b clang::Parser::ParseGNUAttributeArgs(clang::IdentifierInfo*, clang::SourceLocation, clang::ParsedAttributes&, clang::SourceLocation*, clang::IdentifierInfo*, clang::SourceLocation, clang::AttributeCommonInfo::Form, clang::Declarator*) (/opt/llvm/bin/clang+0x5e0c76b)
#22 0x000055e3646189ff clang::Parser::ParseLexedAttribute(clang::Parser::LateParsedAttribute&, bool, bool) (/opt/llvm/bin/clang+0x5e6d9ff)
#23 0x000055e364618eef clang::Parser::ParseLexedAttributeList(clang::Parser::LateParsedAttrList&, clang::Decl*, bool, bool) (/opt/llvm/bin/clang+0x5e6deef)
#24 0x000055e3645c1fc7 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/llvm/bin/clang+0x5e16fc7)
#25 0x000055e36456ebad clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/llvm/bin/clang+0x5dc3bad)
#26 0x000055e36456e599 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/llvm/bin/clang+0x5dc3599)
#27 0x000055e36456d938 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/llvm/bin/clang+0x5dc2938)
#28 0x000055e36456c233 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/llvm/bin/clang+0x5dc1233)
#29 0x000055e3645689fe clang::ParseAST(clang::Sema&, bool, bool) (/opt/llvm/bin/clang+0x5dbd9fe)
#30 0x000055e3635cba6d clang::FrontendAction::Execute() (/opt/llvm/bin/clang+0x4e20a6d)
#31 0x000055e363548484 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/llvm/bin/clang+0x4d9d484)
#32 0x000055e363680212 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/llvm/bin/clang+0x4ed5212)
#33 0x000055e3611430b2 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/llvm/bin/clang+0x29980b2)
#34 0x000055e36113fdc1 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#35 0x000055e3633d7399 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::$_0>(long) Job.cpp:0:0
#36 0x000055e362a7c868 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/llvm/bin/clang+0x42d1868)
#37 0x000055e3633d6de5 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (/opt/llvm/bin/clang+0x4c2bde5)
#38 0x000055e36339df7f clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/llvm/bin/clang+0x4bf2f7f)
#39 0x000055e36339e11e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/llvm/bin/clang+0x4bf311e)
#40 0x000055e3633b736d clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/llvm/bin/clang+0x4c0c36d)
#41 0x000055e36113f73a clang_main(int, char**, llvm::ToolContext const&) (/opt/llvm/bin/clang+0x299473a)
#42 0x000055e36114cfa7 main (/opt/llvm/bin/clang+0x29a1fa7)
#43 0x00007f6e08033d68 (/lib/x86_64-linux-gnu/libc.so.6+0x29d68)
#44 0x00007f6e08033e25 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e25)
#45 0x000055e36113dae1 _start (/opt/llvm/bin/clang+0x2992ae1)
clang: error: clang frontend command failed with exit code 139 (use -v to see invocation)
ClangBuiltLinux clang version 21.0.0git (https://github.com/llvm/llvm-project d0f472c246911e35656bf24fd282f80d7482211a)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /opt/llvm/bin
```

To reproduce:
[core-d0a8fd.c.txt](https://github.com/user-attachments/files/19088234/core-d0a8fd.c.txt)
[core-d0a8fd.sh.txt](https://github.com/user-attachments/files/19088235/core-d0a8fd.sh.txt)

Report courtesy of Peter Zijlstra.

I was able to confirm the crash on my own Clang instance as well:
```
clang version 21.0.0git (https://github.com/llvm/llvm-project.git 829e2a55261890e15102d978f714001a2d1acf85)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /usr/local/google/home/elver/third_party/llvm/build/bin
Build config: +assertions
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-c

Author: Marco Elver (melver)

<details>
Clang crashes in clang::Sema::PushCompoundScope:

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /opt/llvm/bin/clang -Wp,-MMD,kernel/sched/.core.o.d -nostdinc -I../arch/x86/include -I./arch/x86/include/generated -I../include -I./include -I../arch/x86/include/uapi -I./arch/x86/include/generated/uapi -I../include/uapi -I./include/generated/uapi -include ../include/linux/compiler-version.h -include ../include/linux/kconfig.h -include ../include/linux/compiler_types.h -D__KERNEL__ --target=x86_64-linux-gnu -fintegrated-as -Werror=unknown-warning-option -Werror=ignored-optimization-argument -Werror=option-ignored -Werror=unused-command-line-argument -fmacro-prefix-map=../= -std=gnu11 -fshort-wchar -funsigned-char -fno-common -fno-PIE -fno-strict-aliasing -mno-sse -mno-mmx -mno-sse2 -mno-3dnow -mno-avx -fcf-protection=branch -fno-jump-tables -m64 -falign-loops=1 -mno-80387 -mno-fp-ret-in-387 -mstack-alignment=8 -mskip-rax-setup -march=core2 -mno-red-zone -mcmodel=kernel -Wno-sign-compare -fno-asynchronous-unwind-tables -mretpoline-external-thunk -mindirect-branch-cs-prefix -mfunction-return=thunk-extern -fpatchable-function-entry=11,11 -fno-delete-null-pointer-checks -O2 -fstack-protector-strong -fno-stack-clash-protection -pg -mfentry -DCC_USING_NOP_MCOUNT -DCC_USING_FENTRY -fsanitize=kcfi -falign-functions=16 -fstrict-flex-arrays=3 -fno-strict-overflow -fno-stack-check -Wall -Wundef -Werror=implicit-function-declaration -Werror=implicit-int -Werror=return-type -Werror=strict-prototypes -Wno-format-security -Wno-trigraphs -Wno-frame-address -Wno-address-of-packed-member -Wmissing-declarations -Wmissing-prototypes -Wframe-larger-than=2048 -Wno-gnu -Wno-format-overflow-non-kprintf -Wno-format-truncation-non-kprintf -Wvla -Wno-pointer-sign -Wcast-function-type -Wimplicit-fallthrough -Werror=date-time -Werror=incompatible-pointer-types -Wenum-conversion -Wextra -Wunused -Wno-unused-but-set-variable -Wno-unused-const-variable -Wno-format-overflow -Wno-override-init -Wno-pointer-to-enum-cast -Wno-tautological-constant-out-of-range-compare -Wno-unaligned-access -Wno-enum-compare-conditional -Wno-missing-field-initializers -Wno-type-limits -Wno-shift-negative-value -Wno-enum-enum-conversion -Wno-sign-compare -Wno-unused-parameter -g -Wno-unused-but-set-variable -DWARN_CAPABILITY_ANALYSIS -fexperimental-late-parse-attributes -Wthread-safety -I../kernel/sched -Ikernel/sched -DKBUILD_MODFILE=\"kernel/sched/core\" -DKBUILD_BASENAME=\"core\" -DKBUILD_MODNAME=\"core\" -D__KBUILD_MODNAME=kmod_core -c -o kernel/sched/core.o ../kernel/sched/core.c
1.      ../kernel/sched/sched.h:1816:2 &lt;Spelling=../include/linux/percpu-defs.h:217:37&gt;: current parser token '\
do'
2.      ../kernel/sched/sched.h:1816:2 &lt;Spelling=../include/linux/percpu-defs.h:236:2&gt;: in compound statement ('{}')
 #<!-- -->0 0x000055e362aee5e8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/llvm/bin/clang+0x43435e8)
 #<!-- -->1 0x000055e362aec2ee llvm::sys::RunSignalHandlers() (/opt/llvm/bin/clang+0x43412ee)
 #<!-- -->2 0x000055e362a7ca49 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007f6e08049d20 (/lib/x86_64-linux-gnu/libc.so.6+0x3fd20)
 #<!-- -->4 0x000055e36467d629 clang::Sema::PushCompoundScope(bool) (/opt/llvm/bin/clang+0x5ed2629)
 #<!-- -->5 0x000055e3645f9fee clang::Parser::ParseCompoundStatementBody(bool) (/opt/llvm/bin/clang+0x5e4efee)
 #<!-- -->6 0x000055e3645f5a0d clang::Parser::ParseCompoundStatement(bool) (/opt/llvm/bin/clang+0x5e4aa0d)
 #<!-- -->7 0x000055e36458a673 clang::Parser::ParseParenExpression(clang::Parser::ParenParseOption&amp;, bool, bool, clang::OpaquePtr&lt;clang::QualType&gt;&amp;, clang::SourceLocation&amp;) (/opt/llvm/bin/clang+0x5ddf673)
 #<!-- -->8 0x000055e364587c6e clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) (/opt/llvm/bin/clang+0x5ddcc6e)
 #<!-- -->9 0x000055e364586c6c clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) (/opt/llvm/bin/clang+0x5ddbc6c)
#<!-- -->10 0x000055e364583278 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/opt/llvm/bin/clang+0x5dd8278)
#<!-- -->11 0x000055e36458ad15 clang::Parser::ParseParenExpression(clang::Parser::ParenParseOption&amp;, bool, bool, clang::OpaquePtr&lt;clang::QualType&gt;&amp;, clang::SourceLocation&amp;) (/opt/llvm/bin/clang+0x5ddfd15)
#<!-- -->12 0x000055e364587c6e clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) (/opt/llvm/bin/clang+0x5ddcc6e)
#<!-- -->13 0x000055e364584581 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) (/opt/llvm/bin/clang+0x5dd9581)
#<!-- -->14 0x000055e364587b9f clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) (/opt/llvm/bin/clang+0x5ddcb9f)
#<!-- -->15 0x000055e364583278 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/opt/llvm/bin/clang+0x5dd8278)
#<!-- -->16 0x000055e36458ad15 clang::Parser::ParseParenExpression(clang::Parser::ParenParseOption&amp;, bool, bool, clang::OpaquePtr&lt;clang::QualType&gt;&amp;, clang::SourceLocation&amp;) (/opt/llvm/bin/clang+0x5ddfd15)
#<!-- -->17 0x000055e364587c6e clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) (/opt/llvm/bin/clang+0x5ddcc6e)
#<!-- -->18 0x000055e364583278 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/opt/llvm/bin/clang+0x5dd8278)
#<!-- -->19 0x000055e3645b8197 clang::Parser::ParseAttributeArgumentList(clang::IdentifierInfo const&amp;, llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, clang::ParsedAttributeArgumentsProperties) (/opt/llvm/bin/clang+0x5e0d197)
#<!-- -->20 0x000055e3645ba694 clang::Parser::ParseAttributeArgsCommon(clang::IdentifierInfo*, clang::SourceLocation, clang::ParsedAttributes&amp;, clang::SourceLocation*, clang::IdentifierInfo*, clang::SourceLocation, clang::AttributeCommonInfo::Form) (/opt/llvm/bin/clang+0x5e0f694)
#<!-- -->21 0x000055e3645b776b clang::Parser::ParseGNUAttributeArgs(clang::IdentifierInfo*, clang::SourceLocation, clang::ParsedAttributes&amp;, clang::SourceLocation*, clang::IdentifierInfo*, clang::SourceLocation, clang::AttributeCommonInfo::Form, clang::Declarator*) (/opt/llvm/bin/clang+0x5e0c76b)
#<!-- -->22 0x000055e3646189ff clang::Parser::ParseLexedAttribute(clang::Parser::LateParsedAttribute&amp;, bool, bool) (/opt/llvm/bin/clang+0x5e6d9ff)
#<!-- -->23 0x000055e364618eef clang::Parser::ParseLexedAttributeList(clang::Parser::LateParsedAttrList&amp;, clang::Decl*, bool, bool) (/opt/llvm/bin/clang+0x5e6deef)
#<!-- -->24 0x000055e3645c1fc7 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/llvm/bin/clang+0x5e16fc7)
#<!-- -->25 0x000055e36456ebad clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/llvm/bin/clang+0x5dc3bad)
#<!-- -->26 0x000055e36456e599 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/llvm/bin/clang+0x5dc3599)
#<!-- -->27 0x000055e36456d938 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/llvm/bin/clang+0x5dc2938)
#<!-- -->28 0x000055e36456c233 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/llvm/bin/clang+0x5dc1233)
#<!-- -->29 0x000055e3645689fe clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/llvm/bin/clang+0x5dbd9fe)
#<!-- -->30 0x000055e3635cba6d clang::FrontendAction::Execute() (/opt/llvm/bin/clang+0x4e20a6d)
#<!-- -->31 0x000055e363548484 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/llvm/bin/clang+0x4d9d484)
#<!-- -->32 0x000055e363680212 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/llvm/bin/clang+0x4ed5212)
#<!-- -->33 0x000055e3611430b2 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/llvm/bin/clang+0x29980b2)
#<!-- -->34 0x000055e36113fdc1 ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->35 0x000055e3633d7399 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::$_0&gt;(long) Job.cpp:0:0
#<!-- -->36 0x000055e362a7c868 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/llvm/bin/clang+0x42d1868)
#<!-- -->37 0x000055e3633d6de5 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (/opt/llvm/bin/clang+0x4c2bde5)
#<!-- -->38 0x000055e36339df7f clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/llvm/bin/clang+0x4bf2f7f)
#<!-- -->39 0x000055e36339e11e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/llvm/bin/clang+0x4bf311e)
#<!-- -->40 0x000055e3633b736d clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/llvm/bin/clang+0x4c0c36d)
#<!-- -->41 0x000055e36113f73a clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/llvm/bin/clang+0x299473a)
#<!-- -->42 0x000055e36114cfa7 main (/opt/llvm/bin/clang+0x29a1fa7)
#<!-- -->43 0x00007f6e08033d68 (/lib/x86_64-linux-gnu/libc.so.6+0x29d68)
#<!-- -->44 0x00007f6e08033e25 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e25)
#<!-- -->45 0x000055e36113dae1 _start (/opt/llvm/bin/clang+0x2992ae1)
clang: error: clang frontend command failed with exit code 139 (use -v to see invocation)
ClangBuiltLinux clang version 21.0.0git (https://github.com/llvm/llvm-project d0f472c246911e35656bf24fd282f80d7482211a)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /opt/llvm/bin
```

To reproduce:
[core-d0a8fd.c.txt](https://github.com/user-attachments/files/19088234/core-d0a8fd.c.txt)
[core-d0a8fd.sh.txt](https://github.com/user-attachments/files/19088235/core-d0a8fd.sh.txt)

Report courtesy of Peter Zijlstra.

I was able to confirm the crash on my own Clang instance as well:
```
clang version 21.0.0git (https://github.com/llvm/llvm-project.git 829e2a55261890e15102d978f714001a2d1acf85)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /usr/local/google/home/elver/third_party/llvm/build/bin
Build config: +assertions
```
</details>


---

### Comment 2 - melver

With ASan:
```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /usr/local/google/home/elver/third_party/llvm/build2/bin/clang -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name core.c -function-alignment 4 -falign-loops=1 -mrelocation-model static -fno-delete-null-pointer-checks -fwarn-stack-size=2048 -fno-jump-tables -mframe-pointer=none -relaxed-aliasing -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -mcmodel=kernel -target-cpu core2 -target-feature +retpoline-indirect-calls -target-feature +retpoline-indirect-branches -target-feature -sse -target-feature -mmx -target-feature -sse2 -target-feature -avx -target-feature -x87 -target-feature +retpoline-external-thunk -disable-red-zone -mskip-rax-setup -debug-info-kind=constructor -dwarf-version=5 -debugger-tuning=gdb -fdebug-compilation-dir=/usr/src/linux-2.6/adl3-build -fcoverage-compilation-dir=/usr/src/linux-2.6/adl3-build -nostdsysteminc -nobuiltininc -D __KERNEL__ -D CC_USING_NOP_MCOUNT -D CC_USING_FENTRY -D WARN_CAPABILITY_ANALYSIS -D KBUILD_MODFILE=\"kernel/sched/core\" -D KBUILD_BASENAME=\"core\" -D KBUILD_MODNAME=\"core\" -D __KBUILD_MODNAME=kmod_core -fmacro-prefix-map=../= -O2 -Werror=unknown-warning-option -Werror=ignored-optimization-argument -Werror=option-ignored -Werror=unused-command-line-argument -Wno-sign-compare -Wall -Wundef -Werror=implicit-function-declaration -Werror=implicit-int -Werror=return-type -Werror=strict-prototypes -Wno-format-security -Wno-trigraphs -Wno-frame-address -Wno-address-of-packed-member -Wmissing-declarations -Wmissing-prototypes -Wframe-larger-than=2048 -Wno-gnu -Wno-format-overflow-non-kprintf -Wno-format-truncation-non-kprintf -Wvla -Wno-pointer-sign -Wcast-function-type -Wimplicit-fallthrough -Werror=date-time -Werror=incompatible-pointer-types -Wenum-conversion -Wextra -Wunused -Wno-unused-but-set-variable -Wno-unused-const-variable -Wno-format-overflow -Wno-override-init -Wno-pointer-to-enum-cast -Wno-tautological-constant-out-of-range-compare -Wno-unaligned-access -Wno-enum-compare-conditional -Wno-missing-field-initializers -Wno-type-limits -Wno-shift-negative-value -Wno-enum-enum-conversion -Wno-sign-compare -Wno-unused-parameter -Wno-unused-but-set-variable -std=gnu11 -ferror-limit 19 -fpatchable-function-entry=11 -fpatchable-function-entry-offset=11 -pg -mfentry -fwrapv -fwrapv-pointer -fstrict-flex-arrays=3 -stack-protector 2 -mstack-alignment=8 -fcf-protection=branch -mfunction-return=thunk-extern -mindirect-branch-cs-prefix -fno-signed-char -fwchar-type=short -fno-signed-wchar -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -vectorize-loops -vectorize-slp -fexperimental-late-parse-attributes -fsanitize=kcfi -fsanitize-recover=kcfi -fno-sanitize-memory-param-retval -fno-sanitize-address-use-odr-indicator -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -x c core-d0a8fd.c
1.      ../kernel/sched/sched.h:1816:2 <Spelling=../include/linux/percpu-defs.h:217:37>: current parser token '\
do'
2.      ../kernel/sched/sched.h:1816:2 <Spelling=../include/linux/percpu-defs.h:236:2>: in compound statement ('{}')
 #0 0x00005629f0d5526b backtrace (/usr/local/google/home/elver/third_party/llvm/build2/bin/clang+0x3a5a26b)
 #1 0x00005629f975db51 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /usr/local/google/home/elver/third_party/llvm/llvm-project2/llvm/lib/Support/Unix/Signals.inc:804:13
 #2 0x00005629f9755c2b llvm::sys::RunSignalHandlers() /usr/local/google/home/elver/third_party/llvm/llvm-project2/llvm/lib/Support/Signals.cpp:0:5
 #3 0x00005629f975edd7 SignalHandler(int, siginfo_t*, void*) /usr/local/google/home/elver/third_party/llvm/llvm-project2/llvm/lib/Support/Unix/Signals.inc:0:3
 #4 0x00007f7b6aa08e20 (/lib/x86_64-linux-gnu/libc.so.6+0x3fe20)
 #5 0x00005629ffe3c140 size /usr/local/google/home/elver/third_party/llvm/llvm-project2/llvm/include/llvm/ADT/SmallVector.h:78:32
 #6 0x00005629ffe3c140 reserveForParamAndGetAddressImpl<llvm::SmallVectorTemplateBase<clang::sema::CompoundScopeInfo, true> > /usr/local/google/home/elver/third_party/llvm/llvm-project2/llvm/include/llvm/ADT/SmallVector.h:231:28
 #7 0x00005629ffe3c140 reserveForParamAndGetAddress /usr/local/google/home/elver/third_party/llvm/llvm-project2/llvm/include/llvm/ADT/SmallVector.h:537:9
 #8 0x00005629ffe3c140 push_back /usr/local/google/home/elver/third_party/llvm/llvm-project2/llvm/include/llvm/ADT/SmallVector.h:562:23
 #9 0x00005629ffe3c140 clang::Sema::PushCompoundScope(bool) /usr/local/google/home/elver/third_party/llvm/llvm-project2/clang/lib/Sema/Sema.cpp:2336:36
#10 0x00005629ffc6dd90 clang::Parser::ParseCompoundStatementBody(bool) /usr/local/google/home/elver/third_party/llvm/llvm-project2/clang/lib/Parse/ParseStmt.cpp:1215:3
#11 0x00005629ffc59b32 ParseCompoundStatement /usr/local/google/home/elver/third_party/llvm/llvm-project2/clang/lib/Parse/ParseStmt.cpp:1060:10
#12 0x00005629ffc59b32 clang::Parser::ParseCompoundStatement(bool) /usr/local/google/home/elver/third_party/llvm/llvm-project2/clang/lib/Parse/ParseStmt.cpp:1025:10
#13 0x00005629ffaeb11d clang::Parser::ParseParenExpression(clang::Parser::ParenParseOption&, bool, bool, clang::OpaquePtr<clang::QualType>&, clang::SourceLocation&) /usr/local/google/home/elver/third_party/llvm/llvm-project2/clang/lib/Parse/ParseExpr.cpp:0:23
#14 0x00005629ffae4089 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) /usr/local/google/home/elver/third_party/llvm/llvm-project2/clang/lib/Parse/ParseExpr.cpp:1108:11
#15 0x00005629ffadf7b1 ParseCastExpression /usr/local/google/home/elver/third_party/llvm/llvm-project2/clang/lib/Parse/ParseExpr.cpp:729:20
#16 0x00005629ffadf7b1 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) /usr/local/google/home/elver/third_party/llvm/llvm-project2/clang/lib/Parse/ParseExpr.cpp:1500:11
#17 0x00005629ffacf8f0 ParseCastExpression /usr/local/google/home/elver/third_party/llvm/llvm-project2/clang/lib/Parse/ParseExpr.cpp:729:20
#18 0x00005629ffacf8f0 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) /usr/local/google/home/elver/third_party/llvm/llvm-project2/clang/lib/Parse/ParseExpr.cpp:184:20
#19 0x00005629ffaec55d ParseExpression /usr/local/google/home/elver/third_party/llvm/llvm-project2/clang/lib/Parse/ParseExpr.cpp:136:10
#20 0x00005629ffaec55d clang::Parser::ParseParenExpression(clang::Parser::ParenParseOption&, bool, bool, clang::OpaquePtr<clang::QualType>&, clang::SourceLocation&) /usr/local/google/home/elver/third_party/llvm/llvm-project2/clang/lib/Parse/ParseExpr.cpp:3374:14
#21 0x00005629ffae4089 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) /usr/local/google/home/elver/third_party/llvm/llvm-project2/clang/lib/Parse/ParseExpr.cpp:1108:11
#22 0x00005629ffad4bee clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) /usr/local/google/home/elver/third_party/llvm/llvm-project2/clang/lib/Parse/ParseExpr.cpp:729:20
#23 0x00005629ffae2537 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) /usr/local/google/home/elver/third_party/llvm/llvm-project2/clang/lib/Parse/ParseExpr.cpp:1478:11
#24 0x00005629ffacf8f0 ParseCastExpression /usr/local/google/home/elver/third_party/llvm/llvm-project2/clang/lib/Parse/ParseExpr.cpp:729:20
#25 0x00005629ffacf8f0 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) /usr/local/google/home/elver/third_party/llvm/llvm-project2/clang/lib/Parse/ParseExpr.cpp:184:20
#26 0x00005629ffaec55d ParseExpression /usr/local/google/home/elver/third_party/llvm/llvm-project2/clang/lib/Parse/ParseExpr.cpp:136:10
#27 0x00005629ffaec55d clang::Parser::ParseParenExpression(clang::Parser::ParenParseOption&, bool, bool, clang::OpaquePtr<clang::QualType>&, clang::SourceLocation&) /usr/local/google/home/elver/third_party/llvm/llvm-project2/clang/lib/Parse/ParseExpr.cpp:3374:14
#28 0x00005629ffae4089 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) /usr/local/google/home/elver/third_party/llvm/llvm-project2/clang/lib/Parse/ParseExpr.cpp:1108:11
#29 0x00005629ffacf8f0 ParseCastExpression /usr/local/google/home/elver/third_party/llvm/llvm-project2/clang/lib/Parse/ParseExpr.cpp:729:20
#30 0x00005629ffacf8f0 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) /usr/local/google/home/elver/third_party/llvm/llvm-project2/clang/lib/Parse/ParseExpr.cpp:184:20
#31 0x00005629ff9de72b clang::Parser::ParseAttributeArgumentList(clang::IdentifierInfo const&, llvm::SmallVectorImpl<clang::Expr*>&, clang::ParsedAttributeArgumentsProperties) /usr/local/google/home/elver/third_party/llvm/llvm-project2/clang/lib/Parse/ParseDecl.cpp:502:12
#32 0x00005629ff9e24e2 clang::Parser::ParseAttributeArgsCommon(clang::IdentifierInfo*, clang::SourceLocation, clang::ParsedAttributes&, clang::SourceLocation*, clang::IdentifierInfo*, clang::SourceLocation, clang::AttributeCommonInfo::Form) /usr/local/google/home/elver/third_party/llvm/llvm-project2/clang/lib/Parse/ParseDecl.cpp:658:11
#33 0x00005629ff9db2b7 clang::Parser::ParseGNUAttributeArgs(clang::IdentifierInfo*, clang::SourceLocation, clang::ParsedAttributes&, clang::SourceLocation*, clang::IdentifierInfo*, clang::SourceLocation, clang::AttributeCommonInfo::Form, clang::Declarator*) /usr/local/google/home/elver/third_party/llvm/llvm-project2/clang/lib/Parse/ParseDecl.cpp:763:3
#34 0x00005629ff9d2daf clang::Parser::ParseLexedAttribute(clang::Parser::LateParsedAttribute&, bool, bool) /usr/local/google/home/elver/third_party/llvm/llvm-project2/clang/lib/Parse/ParseCXXInlineMethods.cpp:815:7
#35 0x00005629ff9d4595 clang::Parser::ParseLexedAttributeList(clang::Parser::LateParsedAttrList&, clang::Decl*, bool, bool) /usr/local/google/home/elver/third_party/llvm/llvm-project2/clang/lib/Parse/ParseCXXInlineMethods.cpp:0:5
#36 0x00005629ffa05397 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) /usr/local/google/home/elver/third_party/llvm/llvm-project2/clang/lib/Parse/ParseDecl.cpp:0:5
#37 0x00005629ff989ddb clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) /usr/local/google/home/elver/third_party/llvm/llvm-project2/clang/lib/Parse/Parser.cpp:1244:10
#38 0x00005629ff988714 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) /usr/local/google/home/elver/third_party/llvm/llvm-project2/clang/lib/Parse/Parser.cpp:1266:12
#39 0x00005629ff984465 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) /usr/local/google/home/elver/third_party/llvm/llvm-project2/clang/lib/Parse/Parser.cpp:0:14
#40 0x00005629ff97e119 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) /usr/local/google/home/elver/third_party/llvm/llvm-project2/clang/lib/Parse/Parser.cpp:0:12
#41 0x00005629ff96dddf clang::ParseAST(clang::Sema&, bool, bool) /usr/local/google/home/elver/third_party/llvm/llvm-project2/clang/lib/Parse/ParseAST.cpp:170:5
#42 0x00005629fb0795ca clang::CodeGenAction::ExecuteAction() /usr/local/google/home/elver/third_party/llvm/llvm-project2/clang/lib/CodeGen/CodeGenAction.cpp:1183:1
#43 0x00005629fb98b555 clang::FrontendAction::Execute() /usr/local/google/home/elver/third_party/llvm/llvm-project2/clang/lib/Frontend/FrontendAction.cpp:1076:10
#44 0x00005629fb76750b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) /usr/local/google/home/elver/third_party/llvm/llvm-project2/clang/lib/Frontend/CompilerInstance.cpp:0:33
#45 0x00005629fbc8df06 get /usr/bin/../lib/gcc/x86_64-linux-gnu/14/../../../../include/c++/14/bits/shared_ptr_base.h:1667:16
#46 0x00005629fbc8df06 _M_get /usr/bin/../lib/gcc/x86_64-linux-gnu/14/../../../../include/c++/14/bits/shared_ptr_base.h:1364:66
#47 0x00005629fbc8df06 operator-> /usr/bin/../lib/gcc/x86_64-linux-gnu/14/../../../../include/c++/14/bits/shared_ptr_base.h:1358:9
#48 0x00005629fbc8df06 getFrontendOpts /usr/local/google/home/elver/third_party/llvm/llvm-project2/clang/include/clang/Frontend/CompilerInstance.h:313:12
#49 0x00005629fbc8df06 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) /usr/local/google/home/elver/third_party/llvm/llvm-project2/clang/lib/FrontendTool/ExecuteCompilerInvocation.cpp:281:14
#50 0x00005629f0df990c cc1_main(llvm::ArrayRef<char const*>, char const*, void*) /usr/local/google/home/elver/third_party/llvm/llvm-project2/clang/tools/driver/cc1_main.cpp:0:15
#51 0x00005629f0deedaa ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) /usr/local/google/home/elver/third_party/llvm/llvm-project2/clang/tools/driver/driver.cpp:218:12
#52 0x00005629f0deba93 clang_main(int, char**, llvm::ToolContext const&) /usr/local/google/home/elver/third_party/llvm/llvm-project2/clang/tools/driver/driver.cpp:0:12
#53 0x00005629f0e20903 main /usr/local/google/home/elver/third_party/llvm/build2/tools/clang/tools/driver/clang-driver.cpp:17:10
#54 0x00007f7b6a9f2d68 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#55 0x00007f7b6a9f2e25 call_init ./csu/../csu/libc-start.c:128:20
#56 0x00007f7b6a9f2e25 __libc_start_main ./csu/../csu/libc-start.c:347:5
#57 0x00005629f0d0fb41 _start (/usr/local/google/home/elver/third_party/llvm/build2/bin/clang+0x3a14b41)
AddressSanitizer:DEADLYSIGNAL
=================================================================
==3564069==ERROR: AddressSanitizer: SEGV on unknown address 0x4f60b00366225 (pc 0x7f7b6aa5ce5c bp 0x7f7b6b042670 sp 0x7f7b6b042570 T0)
==3564069==The signal is caused by a READ memory access.
    #0 0x7f7b6aa5ce5c in __pthread_kill_implementation nptl/pthread_kill.c:44:76
    #1 0x7f7b6aa08d81 in raise signal/../sysdeps/posix/raise.c:26:13
    #2 0x5629f975ee05 in SignalHandler(int, siginfo_t*, void*) /usr/local/google/home/elver/third_party/llvm/llvm-project2/llvm/lib/Support/Unix/Signals.inc
    #3 0x7f7b6aa08e1f  (/lib/x86_64-linux-gnu/libc.so.6+0x3fe1f) (BuildId: ea119b374e0f8f858c82ad03a9542414f9ea1c8c)
    #4 0x5629ffe3c13f in size /usr/local/google/home/elver/third_party/llvm/llvm-project2/llvm/include/llvm/ADT/SmallVector.h:78:32
    #5 0x5629ffe3c13f in reserveForParamAndGetAddressImpl<llvm::SmallVectorTemplateBase<clang::sema::CompoundScopeInfo, true> > /usr/local/google/home/elver/third_party/llvm/llvm-project2/llvm/include/llvm/ADT/SmallVector.h:231:28
    #6 0x5629ffe3c13f in reserveForParamAndGetAddress /usr/local/google/home/elver/third_party/llvm/llvm-project2/llvm/include/llvm/ADT/SmallVector.h:537:9
    #7 0x5629ffe3c13f in push_back /usr/local/google/home/elver/third_party/llvm/llvm-project2/llvm/include/llvm/ADT/SmallVector.h:562:23
    #8 0x5629ffe3c13f in clang::Sema::PushCompoundScope(bool) /usr/local/google/home/elver/third_party/llvm/llvm-project2/clang/lib/Sema/Sema.cpp:2336:36
    #9 0x5629ffc6dd8f in CompoundScopeRAII /usr/local/google/home/elver/third_party/llvm/llvm-project2/clang/include/clang/Sema/Sema.h:920:9
    #10 0x5629ffc6dd8f in clang::Parser::ParseCompoundStatementBody(bool) /usr/local/google/home/elver/third_party/llvm/llvm-project2/clang/lib/Parse/ParseStmt.cpp:1212:27
    #11 0x5629ffc59b31 in ParseCompoundStatement /usr/local/google/home/elver/third_party/llvm/llvm-project2/clang/lib/Parse/ParseStmt.cpp:1060:10
    #12 0x5629ffc59b31 in clang::Parser::ParseCompoundStatement(bool) /usr/local/google/home/elver/third_party/llvm/llvm-project2/clang/lib/Parse/ParseStmt.cpp:1025:10
    #13 0x5629ffaeb11c in clang::Parser::ParseParenExpression(clang::Parser::ParenParseOption&, bool, bool, clang::OpaquePtr<clang::QualType>&, clang::SourceLocation&) /usr/local/google/home/elver/third_party/llvm/llvm-project2/clang/lib/Parse/ParseExpr.cpp:3129:23
    #14 0x5629ffae4088 in clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) /usr/local/google/home/elver/third_party/llvm/llvm-project2/clang/lib/Parse/ParseExpr.cpp:1108:11
    #15 0x5629ffadf7b0 in ParseCastExpression /usr/local/google/home/elver/third_party/llvm/llvm-project2/clang/lib/Parse/ParseExpr.cpp:729:20
    #16 0x5629ffadf7b0 in clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) /usr/local/google/home/elver/third_party/llvm/llvm-project2/clang/lib/Parse/ParseExpr.cpp:1500:11
    #17 0x5629ffacf8ef in ParseCastExpression /usr/local/google/home/elver/third_party/llvm/llvm-project2/clang/lib/Parse/ParseExpr.cpp:729:20
    #18 0x5629ffacf8ef in clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) /usr/local/google/home/elver/third_party/llvm/llvm-project2/clang/lib/Parse/ParseExpr.cpp:184:20
    #19 0x5629ffaec55c in ParseExpression /usr/local/google/home/elver/third_party/llvm/llvm-project2/clang/lib/Parse/ParseExpr.cpp:135:18
    #20 0x5629ffaec55c in clang::Parser::ParseParenExpression(clang::Parser::ParenParseOption&, bool, bool, clang::OpaquePtr<clang::QualType>&, clang::SourceLocation&) /usr/local/google/home/elver/third_party/llvm/llvm-project2/clang/lib/Parse/ParseExpr.cpp:3374:14
    #21 0x5629ffae4088 in clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) /usr/local/google/home/elver/third_party/llvm/llvm-project2/clang/lib/Parse/ParseExpr.cpp:1108:11
    #22 0x5629ffad4bed in clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) /usr/local/google/home/elver/third_party/llvm/llvm-project2/clang/lib/Parse/ParseExpr.cpp:729:20
    #23 0x5629ffae2536 in clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) /usr/local/google/home/elver/third_party/llvm/llvm-project2/clang/lib/Parse/ParseExpr.cpp:1478:11
    #24 0x5629ffacf8ef in ParseCastExpression /usr/local/google/home/elver/third_party/llvm/llvm-project2/clang/lib/Parse/ParseExpr.cpp:729:20
    #25 0x5629ffacf8ef in clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) /usr/local/google/home/elver/third_party/llvm/llvm-project2/clang/lib/Parse/ParseExpr.cpp:184:20
    #26 0x5629ffaec55c in ParseExpression /usr/local/google/home/elver/third_party/llvm/llvm-project2/clang/lib/Parse/ParseExpr.cpp:135:18
    #27 0x5629ffaec55c in clang::Parser::ParseParenExpression(clang::Parser::ParenParseOption&, bool, bool, clang::OpaquePtr<clang::QualType>&, clang::SourceLocation&) /usr/local/google/home/elver/third_party/llvm/llvm-project2/clang/lib/Parse/ParseExpr.cpp:3374:14
    #28 0x5629ffae4088 in clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) /usr/local/google/home/elver/third_party/llvm/llvm-project2/clang/lib/Parse/ParseExpr.cpp:1108:11
    #29 0x5629ffacf8ef in ParseCastExpression /usr/local/google/home/elver/third_party/llvm/llvm-project2/clang/lib/Parse/ParseExpr.cpp:729:20
    #30 0x5629ffacf8ef in clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) /usr/local/google/home/elver/third_party/llvm/llvm-project2/clang/lib/Parse/ParseExpr.cpp:184:20
    #31 0x5629ff9de72a in clang::Parser::ParseAttributeArgumentList(clang::IdentifierInfo const&, llvm::SmallVectorImpl<clang::Expr*>&, clang::ParsedAttributeArgumentsProperties) /usr/local/google/home/elver/third_party/llvm/llvm-project2/clang/lib/Parse/ParseDecl.cpp:500:14
    #32 0x5629ff9e24e1 in clang::Parser::ParseAttributeArgsCommon(clang::IdentifierInfo*, clang::SourceLocation, clang::ParsedAttributes&, clang::SourceLocation*, clang::IdentifierInfo*, clang::SourceLocation, clang::AttributeCommonInfo::Form) /usr/local/google/home/elver/third_party/llvm/llvm-project2/clang/lib/Parse/ParseDecl.cpp:658:11
    #33 0x5629ff9db2b6 in clang::Parser::ParseGNUAttributeArgs(clang::IdentifierInfo*, clang::SourceLocation, clang::ParsedAttributes&, clang::SourceLocation*, clang::IdentifierInfo*, clang::SourceLocation, clang::AttributeCommonInfo::Form, clang::Declarator*) /usr/local/google/home/elver/third_party/llvm/llvm-project2/clang/lib/Parse/ParseDecl.cpp:763:3
    #34 0x5629ff9d2dae in clang::Parser::ParseLexedAttribute(clang::Parser::LateParsedAttribute&, bool, bool) /usr/local/google/home/elver/third_party/llvm/llvm-project2/clang/lib/Parse/ParseCXXInlineMethods.cpp:815:7
    #35 0x5629ff9d4594 in clang::Parser::ParseLexedAttributeList(clang::Parser::LateParsedAttrList&, clang::Decl*, bool, bool) /usr/local/google/home/elver/third_party/llvm/llvm-project2/clang/lib/Parse/ParseCXXInlineMethods.cpp:763:5
    #36 0x5629ffa05396 in clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) /usr/local/google/home/elver/third_party/llvm/llvm-project2/clang/lib/Parse/ParseDecl.cpp:2553:5
    #37 0x5629ff989dda in clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) /usr/local/google/home/elver/third_party/llvm/llvm-project2/clang/lib/Parse/Parser.cpp:1244:10
    #38 0x5629ff988713 in clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) /usr/local/google/home/elver/third_party/llvm/llvm-project2/clang/lib/Parse/Parser.cpp:1266:12
    #39 0x5629ff984464 in clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) /usr/local/google/home/elver/third_party/llvm/llvm-project2/clang/lib/Parse/Parser.cpp:1069:14
    #40 0x5629ff97e118 in clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) /usr/local/google/home/elver/third_party/llvm/llvm-project2/clang/lib/Parse/Parser.cpp:758:12
    #41 0x5629ff96ddde in clang::ParseAST(clang::Sema&, bool, bool) /usr/local/google/home/elver/third_party/llvm/llvm-project2/clang/lib/Parse/ParseAST.cpp:171:20
    #42 0x5629fb0795c9 in clang::CodeGenAction::ExecuteAction() /usr/local/google/home/elver/third_party/llvm/llvm-project2/clang/lib/CodeGen/CodeGenAction.cpp:1101:30
    #43 0x5629fb98b554 in clang::FrontendAction::Execute() /usr/local/google/home/elver/third_party/llvm/llvm-project2/clang/lib/Frontend/FrontendAction.cpp:1072:3
    #44 0x5629fb76750a in clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) /usr/local/google/home/elver/third_party/llvm/llvm-project2/clang/lib/Frontend/CompilerInstance.cpp:1056:33
    #45 0x5629fbc8df05 in clang::ExecuteCompilerInvocation(clang::CompilerInstance*) /usr/local/google/home/elver/third_party/llvm/llvm-project2/clang/lib/FrontendTool/ExecuteCompilerInvocation.cpp:280:25
    #46 0x5629f0df990b in cc1_main(llvm::ArrayRef<char const*>, char const*, void*) /usr/local/google/home/elver/third_party/llvm/llvm-project2/clang/tools/driver/cc1_main.cpp:290:15
    #47 0x5629f0deeda9 in ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) /usr/local/google/home/elver/third_party/llvm/llvm-project2/clang/tools/driver/driver.cpp:218:12
    #48 0x5629f0deba92 in clang_main(int, char**, llvm::ToolContext const&) /usr/local/google/home/elver/third_party/llvm/llvm-project2/clang/tools/driver/driver.cpp:259:12
    #49 0x5629f0e20902 in main /usr/local/google/home/elver/third_party/llvm/build2/tools/clang/tools/driver/clang-driver.cpp:17:10
    #50 0x7f7b6a9f2d67 in __libc_start_call_main csu/../sysdeps/nptl/libc_start_call_main.h:58:16
    #51 0x7f7b6a9f2e24 in __libc_start_main csu/../csu/libc-start.c:360:3
    #52 0x5629f0d0fb40 in _start (/usr/local/google/home/elver/third_party/llvm/build2/bin/clang-21+0x3a14b40) (BuildId: 8db1232284daf23c3295856a00ebcf506d32ad8e)

AddressSanitizer can not provide additional info.
SUMMARY: AddressSanitizer: SEGV nptl/pthread_kill.c:44:76 in __pthread_kill_implementation
==3564069==ABORTING

```

---

### Comment 3 - melver

```
/usr/local/google/home/elver/third_party/llvm/llvm-project2/clang/lib/Sema/Sema.cpp:2336:21: runtime error: member access within null pointer of type 'sema::FunctionScopeInfo'
SUMMARY: UndefinedBehaviorSanitizer: undefined-behavior /usr/local/google/home/elver/third_party/llvm/llvm-project2/clang/lib/Sema/Sema.cpp:2336:21 in
```

---

### Comment 4 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Marco Elver (melver)

<details>
Clang crashes in clang::Sema::PushCompoundScope:

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /opt/llvm/bin/clang -Wp,-MMD,kernel/sched/.core.o.d -nostdinc -I../arch/x86/include -I./arch/x86/include/generated -I../include -I./include -I../arch/x86/include/uapi -I./arch/x86/include/generated/uapi -I../include/uapi -I./include/generated/uapi -include ../include/linux/compiler-version.h -include ../include/linux/kconfig.h -include ../include/linux/compiler_types.h -D__KERNEL__ --target=x86_64-linux-gnu -fintegrated-as -Werror=unknown-warning-option -Werror=ignored-optimization-argument -Werror=option-ignored -Werror=unused-command-line-argument -fmacro-prefix-map=../= -std=gnu11 -fshort-wchar -funsigned-char -fno-common -fno-PIE -fno-strict-aliasing -mno-sse -mno-mmx -mno-sse2 -mno-3dnow -mno-avx -fcf-protection=branch -fno-jump-tables -m64 -falign-loops=1 -mno-80387 -mno-fp-ret-in-387 -mstack-alignment=8 -mskip-rax-setup -march=core2 -mno-red-zone -mcmodel=kernel -Wno-sign-compare -fno-asynchronous-unwind-tables -mretpoline-external-thunk -mindirect-branch-cs-prefix -mfunction-return=thunk-extern -fpatchable-function-entry=11,11 -fno-delete-null-pointer-checks -O2 -fstack-protector-strong -fno-stack-clash-protection -pg -mfentry -DCC_USING_NOP_MCOUNT -DCC_USING_FENTRY -fsanitize=kcfi -falign-functions=16 -fstrict-flex-arrays=3 -fno-strict-overflow -fno-stack-check -Wall -Wundef -Werror=implicit-function-declaration -Werror=implicit-int -Werror=return-type -Werror=strict-prototypes -Wno-format-security -Wno-trigraphs -Wno-frame-address -Wno-address-of-packed-member -Wmissing-declarations -Wmissing-prototypes -Wframe-larger-than=2048 -Wno-gnu -Wno-format-overflow-non-kprintf -Wno-format-truncation-non-kprintf -Wvla -Wno-pointer-sign -Wcast-function-type -Wimplicit-fallthrough -Werror=date-time -Werror=incompatible-pointer-types -Wenum-conversion -Wextra -Wunused -Wno-unused-but-set-variable -Wno-unused-const-variable -Wno-format-overflow -Wno-override-init -Wno-pointer-to-enum-cast -Wno-tautological-constant-out-of-range-compare -Wno-unaligned-access -Wno-enum-compare-conditional -Wno-missing-field-initializers -Wno-type-limits -Wno-shift-negative-value -Wno-enum-enum-conversion -Wno-sign-compare -Wno-unused-parameter -g -Wno-unused-but-set-variable -DWARN_CAPABILITY_ANALYSIS -fexperimental-late-parse-attributes -Wthread-safety -I../kernel/sched -Ikernel/sched -DKBUILD_MODFILE=\"kernel/sched/core\" -DKBUILD_BASENAME=\"core\" -DKBUILD_MODNAME=\"core\" -D__KBUILD_MODNAME=kmod_core -c -o kernel/sched/core.o ../kernel/sched/core.c
1.      ../kernel/sched/sched.h:1816:2 &lt;Spelling=../include/linux/percpu-defs.h:217:37&gt;: current parser token '\
do'
2.      ../kernel/sched/sched.h:1816:2 &lt;Spelling=../include/linux/percpu-defs.h:236:2&gt;: in compound statement ('{}')
 #<!-- -->0 0x000055e362aee5e8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/llvm/bin/clang+0x43435e8)
 #<!-- -->1 0x000055e362aec2ee llvm::sys::RunSignalHandlers() (/opt/llvm/bin/clang+0x43412ee)
 #<!-- -->2 0x000055e362a7ca49 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007f6e08049d20 (/lib/x86_64-linux-gnu/libc.so.6+0x3fd20)
 #<!-- -->4 0x000055e36467d629 clang::Sema::PushCompoundScope(bool) (/opt/llvm/bin/clang+0x5ed2629)
 #<!-- -->5 0x000055e3645f9fee clang::Parser::ParseCompoundStatementBody(bool) (/opt/llvm/bin/clang+0x5e4efee)
 #<!-- -->6 0x000055e3645f5a0d clang::Parser::ParseCompoundStatement(bool) (/opt/llvm/bin/clang+0x5e4aa0d)
 #<!-- -->7 0x000055e36458a673 clang::Parser::ParseParenExpression(clang::Parser::ParenParseOption&amp;, bool, bool, clang::OpaquePtr&lt;clang::QualType&gt;&amp;, clang::SourceLocation&amp;) (/opt/llvm/bin/clang+0x5ddf673)
 #<!-- -->8 0x000055e364587c6e clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) (/opt/llvm/bin/clang+0x5ddcc6e)
 #<!-- -->9 0x000055e364586c6c clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) (/opt/llvm/bin/clang+0x5ddbc6c)
#<!-- -->10 0x000055e364583278 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/opt/llvm/bin/clang+0x5dd8278)
#<!-- -->11 0x000055e36458ad15 clang::Parser::ParseParenExpression(clang::Parser::ParenParseOption&amp;, bool, bool, clang::OpaquePtr&lt;clang::QualType&gt;&amp;, clang::SourceLocation&amp;) (/opt/llvm/bin/clang+0x5ddfd15)
#<!-- -->12 0x000055e364587c6e clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) (/opt/llvm/bin/clang+0x5ddcc6e)
#<!-- -->13 0x000055e364584581 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) (/opt/llvm/bin/clang+0x5dd9581)
#<!-- -->14 0x000055e364587b9f clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) (/opt/llvm/bin/clang+0x5ddcb9f)
#<!-- -->15 0x000055e364583278 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/opt/llvm/bin/clang+0x5dd8278)
#<!-- -->16 0x000055e36458ad15 clang::Parser::ParseParenExpression(clang::Parser::ParenParseOption&amp;, bool, bool, clang::OpaquePtr&lt;clang::QualType&gt;&amp;, clang::SourceLocation&amp;) (/opt/llvm/bin/clang+0x5ddfd15)
#<!-- -->17 0x000055e364587c6e clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) (/opt/llvm/bin/clang+0x5ddcc6e)
#<!-- -->18 0x000055e364583278 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/opt/llvm/bin/clang+0x5dd8278)
#<!-- -->19 0x000055e3645b8197 clang::Parser::ParseAttributeArgumentList(clang::IdentifierInfo const&amp;, llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, clang::ParsedAttributeArgumentsProperties) (/opt/llvm/bin/clang+0x5e0d197)
#<!-- -->20 0x000055e3645ba694 clang::Parser::ParseAttributeArgsCommon(clang::IdentifierInfo*, clang::SourceLocation, clang::ParsedAttributes&amp;, clang::SourceLocation*, clang::IdentifierInfo*, clang::SourceLocation, clang::AttributeCommonInfo::Form) (/opt/llvm/bin/clang+0x5e0f694)
#<!-- -->21 0x000055e3645b776b clang::Parser::ParseGNUAttributeArgs(clang::IdentifierInfo*, clang::SourceLocation, clang::ParsedAttributes&amp;, clang::SourceLocation*, clang::IdentifierInfo*, clang::SourceLocation, clang::AttributeCommonInfo::Form, clang::Declarator*) (/opt/llvm/bin/clang+0x5e0c76b)
#<!-- -->22 0x000055e3646189ff clang::Parser::ParseLexedAttribute(clang::Parser::LateParsedAttribute&amp;, bool, bool) (/opt/llvm/bin/clang+0x5e6d9ff)
#<!-- -->23 0x000055e364618eef clang::Parser::ParseLexedAttributeList(clang::Parser::LateParsedAttrList&amp;, clang::Decl*, bool, bool) (/opt/llvm/bin/clang+0x5e6deef)
#<!-- -->24 0x000055e3645c1fc7 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/llvm/bin/clang+0x5e16fc7)
#<!-- -->25 0x000055e36456ebad clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/llvm/bin/clang+0x5dc3bad)
#<!-- -->26 0x000055e36456e599 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/llvm/bin/clang+0x5dc3599)
#<!-- -->27 0x000055e36456d938 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/llvm/bin/clang+0x5dc2938)
#<!-- -->28 0x000055e36456c233 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/llvm/bin/clang+0x5dc1233)
#<!-- -->29 0x000055e3645689fe clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/llvm/bin/clang+0x5dbd9fe)
#<!-- -->30 0x000055e3635cba6d clang::FrontendAction::Execute() (/opt/llvm/bin/clang+0x4e20a6d)
#<!-- -->31 0x000055e363548484 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/llvm/bin/clang+0x4d9d484)
#<!-- -->32 0x000055e363680212 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/llvm/bin/clang+0x4ed5212)
#<!-- -->33 0x000055e3611430b2 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/llvm/bin/clang+0x29980b2)
#<!-- -->34 0x000055e36113fdc1 ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->35 0x000055e3633d7399 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::$_0&gt;(long) Job.cpp:0:0
#<!-- -->36 0x000055e362a7c868 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/llvm/bin/clang+0x42d1868)
#<!-- -->37 0x000055e3633d6de5 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (/opt/llvm/bin/clang+0x4c2bde5)
#<!-- -->38 0x000055e36339df7f clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/llvm/bin/clang+0x4bf2f7f)
#<!-- -->39 0x000055e36339e11e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/llvm/bin/clang+0x4bf311e)
#<!-- -->40 0x000055e3633b736d clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/llvm/bin/clang+0x4c0c36d)
#<!-- -->41 0x000055e36113f73a clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/llvm/bin/clang+0x299473a)
#<!-- -->42 0x000055e36114cfa7 main (/opt/llvm/bin/clang+0x29a1fa7)
#<!-- -->43 0x00007f6e08033d68 (/lib/x86_64-linux-gnu/libc.so.6+0x29d68)
#<!-- -->44 0x00007f6e08033e25 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e25)
#<!-- -->45 0x000055e36113dae1 _start (/opt/llvm/bin/clang+0x2992ae1)
clang: error: clang frontend command failed with exit code 139 (use -v to see invocation)
ClangBuiltLinux clang version 21.0.0git (https://github.com/llvm/llvm-project d0f472c246911e35656bf24fd282f80d7482211a)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /opt/llvm/bin
```

To reproduce:
[core-d0a8fd.c.txt](https://github.com/user-attachments/files/19088234/core-d0a8fd.c.txt)
[core-d0a8fd.sh.txt](https://github.com/user-attachments/files/19088235/core-d0a8fd.sh.txt)

Report courtesy of Peter Zijlstra.

I was able to confirm the crash on my own Clang instance as well:
```
clang version 21.0.0git (https://github.com/llvm/llvm-project.git 829e2a55261890e15102d978f714001a2d1acf85)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /usr/local/google/home/elver/third_party/llvm/build/bin
Build config: +assertions
```
</details>


---

### Comment 5 - ilya-biryukov

Minimized example:
```cpp
extern struct rq *__task_rq_lock(struct task_struct *p, struct rq_flags *rf)
    __attribute__((acquire_capability(
        (&(*({
          do {
          } while (0);
          ({
          });
        }))) -> __lock)));
extern struct rq *__task_rq_lock(struct task_struct *p, struct rq_flags *rf)
    __attribute__((acquire_capability(
        (&(*({
          do {
          } while (0);
          ({
          });
        }))) -> __lock)));

```

See https://gcc.godbolt.org/z/Evjf8P3nM, Clang crashes, GCC fails with an error
```
<source>:3:13: error: statement-expressions are not allowed outside functions nor in template-argument lists
```

I think it would be reasonable to produce the same error in Clang as a response, the code that crashes does seem to rely on having a function for a compound statement and given that GCC does not support this either, it's a pretty reasonable assumption.


---

### Comment 6 - melver

> Minimized example:
[..]
> ```
> <source>:3:13: error: statement-expressions are not allowed outside functions nor in template-argument lists
> ```
> 
> I think it would be reasonable to produce the same error in Clang as a response, the code that crashes does seem to rely on having a function for a compound statement and given that GCC does not support this either, it's a pretty reasonable assumption.

I think it's reasonable to match what GCC says here. I don't think we should have statement-expressions within this attribute, and if we tried, we probably ought to look for a different solution.

---

### Comment 7 - shafik

Looks like this goes back to clang-7: https://gcc.godbolt.org/z/n7j58qq38

---

### Comment 8 - aaronpuchert

> Clang crashes, GCC fails with an error

GCC doesn't support these attributes anyway, and I'm not aware that GCC supports any attributes with the same kind of symbolic expressions. For example, if we have just `p->__lock` in the attribute argument, GCC also complains with "error: expected `)` before `->` token". GCC doesn't support expressions at all in this context (to my knowledge).

However, without having checked I would assume that statement expressions are currently not supported by the analysis anyway, so disallowing them should be totally fine for now. If you need support for this in Linux, we can look into this separately once you've got the parser to work.

---

