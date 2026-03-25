# [clang] assertion failure in Sema::FixOverloadedFunctionReference during template function address resolution with __extension__

**Author:** AbyssStaror
**URL:** https://github.com/llvm/llvm-project/issues/169802

## Body

### Link for quick verification: https://godbolt.org/z/514Gbehjc
When using the __extension__GCC extension, we encountered an assertion error. However, when we removed the __extension__, the assertion error disappeared.
```c
template <int n>
void foo() {
    __asm__("movl %0, %%eax" : /* no outputs */ : "r"(n) : "%eax");
}

void bar() {
    typedef void (*ptr)();
    ptr p = (ptr)(__extension__(&foo<3>));
}
```
## The stack dump:
```
0.      Program arguments: /workspace/install/llvm/build_21.1.4/bin/clang-21 -cc1 -triple x86_64-unknown-linux-gnu -O3 -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -main-file-name 16125.fuzz -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=none -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/workspace/exps -fcoverage-compilation-dir=/workspace/exps -resource-dir /workspace/install/llvm/build_21.1.4/lib/clang/21 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/x86_64-linux-gnu/c++/13 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/backward -internal-isystem /workspace/install/llvm/build_21.1.4/lib/clang/21/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -std=c++20 -fdeprecated-macro -ferror-limit 19 -fmessage-length=138 -fgnuc-version=4.2.1 -fno-implicit-modules -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -vectorize-loops -vectorize-slp -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/16125-4f8caa.o -x c++ /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_choose_3_1_exp/cases/16125.fuzz
1.      /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_choose_3_1_exp/cases/16125.fuzz:19:42: current parser token ';'
2.      /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_choose_3_1_exp/cases/16125.fuzz:17:12: parsing function body 'bar'
3.      /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_choose_3_1_exp/cases/16125.fuzz:17:12: in compound statement ('{}')
 #0 0x00005be23a9f5c2a llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9691c2a)
 #1 0x00005be23a9f6457 PrintStackTraceSignalHandler(void*) Signals.cpp:0:0
 #2 0x00005be23a9eff57 llvm::sys::RunSignalHandlers() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x968bf57)
 #3 0x00005be23a9f01c7 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #4 0x00007499a66f9330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #5 0x00007499a6752b2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #6 0x00007499a6752b2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #7 0x00007499a6752b2c pthread_kill ./nptl/pthread_kill.c:89:10
 #8 0x00007499a66f927e raise ./signal/../sysdeps/posix/raise.c:27:6
 #9 0x00007499a66dc8ff abort ./stdlib/abort.c:81:7
#10 0x00007499a66dc81b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#11 0x00007499a66ef517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#12 0x00005be23ebcd0c7 clang::Sema::FixOverloadedFunctionReference(clang::Expr*, clang::DeclAccessPair, clang::FunctionDecl*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xd8690c7)
#13 0x00005be23ebcc022 clang::Sema::FixOverloadedFunctionReference(clang::Expr*, clang::DeclAccessPair, clang::FunctionDecl*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xd868022)
#14 0x00005be23e7cdaf0 clang::Sema::PerformImplicitConversion(clang::Expr*, clang::QualType, clang::StandardConversionSequence const&, clang::AssignmentAction, clang::CheckedConversionKind) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xd469af0)
#15 0x00005be23e7cfae2 clang::Sema::PerformImplicitConversion(clang::Expr*, clang::QualType, clang::ImplicitConversionSequence const&, clang::AssignmentAction, clang::CheckedConversionKind) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xd46bae2)
#16 0x00005be23e8978ae clang::InitializationSequence::Perform(clang::Sema&, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::MutableArrayRef<clang::Expr*>, clang::QualType*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xd5338ae)
#17 0x00005be23e1bbedb TryStaticImplicitCast(clang::Sema&, clang::ActionResult<clang::Expr*, true>&, clang::QualType, clang::CheckedConversionKind, (anonymous namespace)::CastOperation::OpRangeType, unsigned int&, clang::CastKind&, bool) SemaCast.cpp:0:0
#18 0x00005be23e1c2cd8 TryStaticCast(clang::Sema&, clang::ActionResult<clang::Expr*, true>&, clang::QualType, clang::CheckedConversionKind, (anonymous namespace)::CastOperation::OpRangeType, unsigned int&, clang::CastKind&, llvm::SmallVector<clang::CXXBaseSpecifier*, 4u>&, bool) SemaCast.cpp:0:0
#19 0x00005be23e1d0da8 (anonymous namespace)::CastOperation::CheckCXXCStyleCast(bool, bool) SemaCast.cpp:0:0
#20 0x00005be23e1d13ce clang::Sema::BuildCStyleCastExpr(clang::SourceLocation, clang::TypeSourceInfo*, clang::SourceLocation, clang::Expr*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xce6d3ce)
#21 0x00005be23e6cdafb clang::Sema::ActOnCastExpr(clang::Scope*, clang::SourceLocation, clang::Declarator&, clang::OpaquePtr<clang::QualType>&, clang::SourceLocation, clang::Expr*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xd369afb)
#22 0x00005be23df91456 clang::Parser::ParseParenExpression(clang::ParenParseOption&, bool, clang::ParenExprKind, clang::TypoCorrectionTypeBehavior, clang::OpaquePtr<clang::QualType>&, clang::SourceLocation&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc2d456)
#23 0x00005be23df852da clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypoCorrectionTypeBehavior, bool, bool*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc212da)
#24 0x00005be23df880d1 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc240d1)
#25 0x00005be23df88194 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc24194)
#26 0x00005be23df229f6 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcbbe9f6)
#27 0x00005be23df43d1b clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcbdfd1b)
#28 0x00005be23df45a1f clang::Parser::ParseSimpleDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, bool, clang::Parser::ForRangeInit*, clang::SourceLocation*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcbe1a1f)
#29 0x00005be23df46069 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcbe2069)
#30 0x00005be23e04446c clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcce046c)
#31 0x00005be23e045099 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcce1099)
#32 0x00005be23e046e9e clang::Parser::ParseCompoundStatementBody(bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcce2e9e)
#33 0x00005be23e0478c2 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcce38c2)
#34 0x00005be23deeb337 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb87337)
#35 0x00005be23df44d18 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcbe0d18)
#36 0x00005be23dee1307 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb7d307)
#37 0x00005be23dee1e9e clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb7de9e)
#38 0x00005be23deedf98 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb89f98)
#39 0x00005be23deef59d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb8b59d)
#40 0x00005be23ded59d0 clang::ParseAST(clang::Sema&, bool, bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb719d0)
#41 0x00005be23bc1d92b clang::ASTFrontendAction::ExecuteAction() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa8b992b)
#42 0x00005be23b80b360 clang::CodeGenAction::ExecuteAction() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa4a7360)
#43 0x00005be23bc259cf clang::FrontendAction::Execute() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa8c19cf)
#44 0x00005be23bb6bf46 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa807f46)
#45 0x00005be23bdadf7b clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xaa49f7b)
#46 0x00005be235b1c523 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47b8523)
#47 0x00005be235b0cc58 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#48 0x00005be235b12b28 clang_main(int, char**, llvm::ToolContext const&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47aeb28)
#49 0x00005be23596db4a main (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x4609b4a)
#50 0x00007499a66de1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#51 0x00007499a66de28b call_init ./csu/../csu/libc-start.c:128:20
#52 0x00007499a66de28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#53 0x00005be235b0af55 _start (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47a6f55)
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
### Link for quick verification: https://godbolt.org/z/514Gbehjc
When using the __extension__GCC extension, we encountered an assertion error. However, when we removed the __extension__, the assertion error disappeared.
```c
template &lt;int n&gt;
void foo() {
    __asm__("movl %0, %%eax" : /* no outputs */ : "r"(n) : "%eax");
}

void bar() {
    typedef void (*ptr)();
    ptr p = (ptr)(__extension__(&amp;foo&lt;3&gt;));
}
```
## The stack dump:
```
0.      Program arguments: /workspace/install/llvm/build_21.1.4/bin/clang-21 -cc1 -triple x86_64-unknown-linux-gnu -O3 -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -main-file-name 16125.fuzz -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=none -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/workspace/exps -fcoverage-compilation-dir=/workspace/exps -resource-dir /workspace/install/llvm/build_21.1.4/lib/clang/21 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/x86_64-linux-gnu/c++/13 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/backward -internal-isystem /workspace/install/llvm/build_21.1.4/lib/clang/21/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -std=c++20 -fdeprecated-macro -ferror-limit 19 -fmessage-length=138 -fgnuc-version=4.2.1 -fno-implicit-modules -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -vectorize-loops -vectorize-slp -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/16125-4f8caa.o -x c++ /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_choose_3_1_exp/cases/16125.fuzz
1.      /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_choose_3_1_exp/cases/16125.fuzz:19:42: current parser token ';'
2.      /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_choose_3_1_exp/cases/16125.fuzz:17:12: parsing function body 'bar'
3.      /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_choose_3_1_exp/cases/16125.fuzz:17:12: in compound statement ('{}')
 #<!-- -->0 0x00005be23a9f5c2a llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9691c2a)
 #<!-- -->1 0x00005be23a9f6457 PrintStackTraceSignalHandler(void*) Signals.cpp:0:0
 #<!-- -->2 0x00005be23a9eff57 llvm::sys::RunSignalHandlers() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x968bf57)
 #<!-- -->3 0x00005be23a9f01c7 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->4 0x00007499a66f9330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->5 0x00007499a6752b2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->6 0x00007499a6752b2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->7 0x00007499a6752b2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->8 0x00007499a66f927e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x00007499a66dc8ff abort ./stdlib/abort.c:81:7
#<!-- -->10 0x00007499a66dc81b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->11 0x00007499a66ef517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->12 0x00005be23ebcd0c7 clang::Sema::FixOverloadedFunctionReference(clang::Expr*, clang::DeclAccessPair, clang::FunctionDecl*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xd8690c7)
#<!-- -->13 0x00005be23ebcc022 clang::Sema::FixOverloadedFunctionReference(clang::Expr*, clang::DeclAccessPair, clang::FunctionDecl*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xd868022)
#<!-- -->14 0x00005be23e7cdaf0 clang::Sema::PerformImplicitConversion(clang::Expr*, clang::QualType, clang::StandardConversionSequence const&amp;, clang::AssignmentAction, clang::CheckedConversionKind) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xd469af0)
#<!-- -->15 0x00005be23e7cfae2 clang::Sema::PerformImplicitConversion(clang::Expr*, clang::QualType, clang::ImplicitConversionSequence const&amp;, clang::AssignmentAction, clang::CheckedConversionKind) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xd46bae2)
#<!-- -->16 0x00005be23e8978ae clang::InitializationSequence::Perform(clang::Sema&amp;, clang::InitializedEntity const&amp;, clang::InitializationKind const&amp;, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::QualType*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xd5338ae)
#<!-- -->17 0x00005be23e1bbedb TryStaticImplicitCast(clang::Sema&amp;, clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, clang::QualType, clang::CheckedConversionKind, (anonymous namespace)::CastOperation::OpRangeType, unsigned int&amp;, clang::CastKind&amp;, bool) SemaCast.cpp:0:0
#<!-- -->18 0x00005be23e1c2cd8 TryStaticCast(clang::Sema&amp;, clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, clang::QualType, clang::CheckedConversionKind, (anonymous namespace)::CastOperation::OpRangeType, unsigned int&amp;, clang::CastKind&amp;, llvm::SmallVector&lt;clang::CXXBaseSpecifier*, 4u&gt;&amp;, bool) SemaCast.cpp:0:0
#<!-- -->19 0x00005be23e1d0da8 (anonymous namespace)::CastOperation::CheckCXXCStyleCast(bool, bool) SemaCast.cpp:0:0
#<!-- -->20 0x00005be23e1d13ce clang::Sema::BuildCStyleCastExpr(clang::SourceLocation, clang::TypeSourceInfo*, clang::SourceLocation, clang::Expr*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xce6d3ce)
#<!-- -->21 0x00005be23e6cdafb clang::Sema::ActOnCastExpr(clang::Scope*, clang::SourceLocation, clang::Declarator&amp;, clang::OpaquePtr&lt;clang::QualType&gt;&amp;, clang::SourceLocation, clang::Expr*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xd369afb)
#<!-- -->22 0x00005be23df91456 clang::Parser::ParseParenExpression(clang::ParenParseOption&amp;, bool, clang::ParenExprKind, clang::TypoCorrectionTypeBehavior, clang::OpaquePtr&lt;clang::QualType&gt;&amp;, clang::SourceLocation&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc2d456)
#<!-- -->23 0x00005be23df852da clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&amp;, clang::TypoCorrectionTypeBehavior, bool, bool*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc212da)
#<!-- -->24 0x00005be23df880d1 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc240d1)
#<!-- -->25 0x00005be23df88194 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc24194)
#<!-- -->26 0x00005be23df229f6 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcbbe9f6)
#<!-- -->27 0x00005be23df43d1b clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcbdfd1b)
#<!-- -->28 0x00005be23df45a1f clang::Parser::ParseSimpleDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, bool, clang::Parser::ForRangeInit*, clang::SourceLocation*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcbe1a1f)
#<!-- -->29 0x00005be23df46069 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcbe2069)
#<!-- -->30 0x00005be23e04446c clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcce046c)
#<!-- -->31 0x00005be23e045099 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcce1099)
#<!-- -->32 0x00005be23e046e9e clang::Parser::ParseCompoundStatementBody(bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcce2e9e)
#<!-- -->33 0x00005be23e0478c2 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcce38c2)
#<!-- -->34 0x00005be23deeb337 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb87337)
#<!-- -->35 0x00005be23df44d18 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcbe0d18)
#<!-- -->36 0x00005be23dee1307 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb7d307)
#<!-- -->37 0x00005be23dee1e9e clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb7de9e)
#<!-- -->38 0x00005be23deedf98 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb89f98)
#<!-- -->39 0x00005be23deef59d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb8b59d)
#<!-- -->40 0x00005be23ded59d0 clang::ParseAST(clang::Sema&amp;, bool, bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb719d0)
#<!-- -->41 0x00005be23bc1d92b clang::ASTFrontendAction::ExecuteAction() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa8b992b)
#<!-- -->42 0x00005be23b80b360 clang::CodeGenAction::ExecuteAction() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa4a7360)
#<!-- -->43 0x00005be23bc259cf clang::FrontendAction::Execute() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa8c19cf)
#<!-- -->44 0x00005be23bb6bf46 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa807f46)
#<!-- -->45 0x00005be23bdadf7b clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xaa49f7b)
#<!-- -->46 0x00005be235b1c523 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47b8523)
#<!-- -->47 0x00005be235b0cc58 ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->48 0x00005be235b12b28 clang_main(int, char**, llvm::ToolContext const&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47aeb28)
#<!-- -->49 0x00005be23596db4a main (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x4609b4a)
#<!-- -->50 0x00007499a66de1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->51 0x00007499a66de28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->52 0x00007499a66de28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->53 0x00005be235b0af55 _start (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47a6f55)
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

### Comment 2 - shafik

goes back to clang-2.7: https://godbolt.org/z/49WdEE3Yd

---

