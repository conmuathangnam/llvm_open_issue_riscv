# Clang ICE when dereferencing volatile void pointer in statement expression

**Author:** AbyssStaror
**URL:** https://github.com/llvm/llvm-project/issues/170451

## Body

### Link for quick verification: https://godbolt.org/z/7bf48cnbv
```c
int main() {
    __int128 z = __extension__ ({ 
        union { 
            unsigned long u[2]; 
            __int128 i;
            } tmp;
        *(volatile void*)(&tmp.i); // Problematic line
      });
    
   return 0;
}
```
Clang triggers an internal compiler error (ICE) with `UNREACHABLE executed` when attempting to compile code that dereferences a `volatile void*pointer` within a statement expression. The crash occurs during the implicit cast from glvalue to prvalue.

## Stack dump:
```
/workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_1_exp/cases/7467.fuzz:13:7: warning: ISO C does not
      allow indirection on operand of type 'volatile void *' [-Wvoid-ptr-dereference]
   13 |       *(volatile void *)&tmp; });
      |       ^~~~~~~~~~~~~~~~~~~~~~
can't implicitly cast glvalue to prvalue with this cast kind: NoOp
UNREACHABLE executed at /workspace/install/llvm/src/llvm-project/clang/lib/Sema/Sema.cpp:765!
Stack dump:
0.      Program arguments: /workspace/install/llvm/build_21.1.4/bin/clang-21 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -main-file-name 7467.fuzz -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/workspace/exps -fcoverage-compilation-dir=/workspace/exps -resource-dir /workspace/install/llvm/build_21.1.4/lib/clang/21 -internal-isystem /workspace/install/llvm/build_21.1.4/lib/clang/21/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -ferror-limit 19 -fmessage-length=137 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/7467-c69d83.o -x c /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_1_exp/cases/7467.fuzz
1.      /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_1_exp/cases/7467.fuzz:13:31: current parser token '}'
2.      /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_1_exp/cases/7467.fuzz:7:12: parsing function body 'main'
3.      /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_1_exp/cases/7467.fuzz:7:12: in compound statement ('{}')
4.      /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_1_exp/cases/7467.fuzz:9:33: in compound statement ('{}')
 #0 0x0000653223aaac2a llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9691c2a)
 #1 0x0000653223aab457 PrintStackTraceSignalHandler(void*) Signals.cpp:0:0
 #2 0x0000653223aa4f57 llvm::sys::RunSignalHandlers() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x968bf57)
 #3 0x0000653223aa51c7 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #4 0x00007bb4b8da0330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #5 0x00007bb4b8df9b2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #6 0x00007bb4b8df9b2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #7 0x00007bb4b8df9b2c pthread_kill ./nptl/pthread_kill.c:89:10
 #8 0x00007bb4b8da027e raise ./signal/../sysdeps/posix/raise.c:27:6
 #9 0x00007bb4b8d838ff abort ./stdlib/abort.c:81:7
#10 0x00006532239a26de llvm::llvm_unreachable_internal(char const*, char const*, unsigned int) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x95896de)
#11 0x00006532271df5f4 clang::Sema::ImpCastExprToType(clang::Expr*, clang::QualType, clang::CastKind, clang::ExprValueKind, llvm::SmallVector<clang::CXXBaseSpecifier*, 4u> const*, clang::CheckedConversionKind) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcdc65f4)
#12 0x0000653227768ac8 clang::Sema::CheckSingleAssignmentConstraints(clang::QualType, clang::ActionResult<clang::Expr*, true>&, bool, bool, bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xd34fac8)
#13 0x000065322794e51b clang::InitializationSequence::Perform(clang::Sema&, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::MutableArrayRef<clang::Expr*>, clang::QualType*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xd53551b)
#14 0x0000653227951b54 clang::Sema::PerformCopyInitialization(clang::InitializedEntity const&, clang::SourceLocation, clang::ActionResult<clang::Expr*, true>, bool, bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xd538b54)
#15 0x00006532277492ba clang::Sema::ActOnStmtExprResult(clang::ActionResult<clang::Expr*, true>) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xd3302ba)
#16 0x00006532270f104f clang::Parser::handleExprStmt(clang::ActionResult<clang::Expr*, true>, clang::Parser::ParsedStmtContext) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xccd804f)
#17 0x00006532270f7353 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xccde353)
#18 0x00006532270f9487 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcce0487)
#19 0x00006532270fa099 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcce1099)
#20 0x00006532270fbe9e clang::Parser::ParseCompoundStatementBody(bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcce2e9e)
#21 0x00006532270fc7d0 void llvm::function_ref<void ()>::callback_fn<clang::Parser::ParseCompoundStatement(bool, unsigned int)::'lambda'()>(long) ParseStmt.cpp:0:0
#22 0x00006532294be42b clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xf0a542b)
#23 0x00006532270efe0e clang::Parser::ParseCompoundStatement(bool, unsigned int) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xccd6e0e)
#24 0x00006532270efe9e clang::Parser::ParseCompoundStatement(bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xccd6e9e)
#25 0x0000653227045e31 clang::Parser::ParseParenExpression(clang::ParenParseOption&, bool, clang::ParenExprKind, clang::TypoCorrectionTypeBehavior, clang::OpaquePtr<clang::QualType>&, clang::SourceLocation&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc2ce31)
#26 0x000065322703a2da clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypoCorrectionTypeBehavior, bool, bool*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc212da)
#27 0x000065322703d0d1 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc240d1)
#28 0x000065322703aa77 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypoCorrectionTypeBehavior, bool, bool*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc21a77)
#29 0x000065322703d0d1 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc240d1)
#30 0x000065322703d194 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc24194)
#31 0x0000653226fd79f6 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcbbe9f6)
#32 0x0000653226ff8d1b clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcbdfd1b)
#33 0x0000653226ffaa1f clang::Parser::ParseSimpleDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, bool, clang::Parser::ForRangeInit*, clang::SourceLocation*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcbe1a1f)
#34 0x0000653226ffb069 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcbe2069)
#35 0x00006532270f946c clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcce046c)
#36 0x00006532270fa099 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcce1099)
#37 0x00006532270fbe9e clang::Parser::ParseCompoundStatementBody(bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcce2e9e)
#38 0x00006532270fc8c2 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcce38c2)
#39 0x0000653226fa0337 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb87337)
#40 0x0000653226ff9d18 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcbe0d18)
#41 0x0000653226f96307 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb7d307)
#42 0x0000653226f96e9e clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb7de9e)
#43 0x0000653226fa2f98 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb89f98)
#44 0x0000653226fa459d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb8b59d)
#45 0x0000653226f8a9d0 clang::ParseAST(clang::Sema&, bool, bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb719d0)
#46 0x0000653224cd292b clang::ASTFrontendAction::ExecuteAction() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa8b992b)
#47 0x00006532248c0360 clang::CodeGenAction::ExecuteAction() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa4a7360)
#48 0x0000653224cda9cf clang::FrontendAction::Execute() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa8c19cf)
#49 0x0000653224c20f46 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa807f46)
#50 0x0000653224e62f7b clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xaa49f7b)
#51 0x000065321ebd1523 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47b8523)
#52 0x000065321ebc1c58 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#53 0x000065321ebc7b28 clang_main(int, char**, llvm::ToolContext const&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47aeb28)
#54 0x000065321ea22b4a main (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x4609b4a)
#55 0x00007bb4b8d851ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#56 0x00007bb4b8d8528b call_init ./csu/../csu/libc-start.c:128:20
#57 0x00007bb4b8d8528b __libc_start_main ./csu/../csu/libc-start.c:347:5
#58 0x000065321ebbff55 _start (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47a6f55)
clang: error: unable to execute command: Aborted (core dumped)
```
## Clang version:
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

Reduced to
```C
int main(void) {
  ({ *(volatile void*)0; });
}
```

[Godbolt link](https://godbolt.org/z/q5abWeGTn)

`const void*` and `const volatile void*` also trigger this bug. It seems that in C++ mode more checks are performed before. Regressed since Clang 3.1.

This seems completely invalid to me, but Clang accepts `*(void*)p` here (for some reason unknown to me). So https://github.com/llvm/llvm-project/labels/crash-on-valid.

----
Did you generate these examples by fuzzer?

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (AbyssStaror)

<details>
### Link for quick verification: https://godbolt.org/z/7bf48cnbv
```c
int main() {
    __int128 z = __extension__ ({ 
        union { 
            unsigned long u[2]; 
            __int128 i;
            } tmp;
        *(volatile void*)(&amp;tmp.i); // Problematic line
      });
    
   return 0;
}
```
Clang triggers an internal compiler error (ICE) with `UNREACHABLE executed` when attempting to compile code that dereferences a `volatile void*pointer` within a statement expression. The crash occurs during the implicit cast from glvalue to prvalue.

## Stack dump:
```
/workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_1_exp/cases/7467.fuzz:13:7: warning: ISO C does not
      allow indirection on operand of type 'volatile void *' [-Wvoid-ptr-dereference]
   13 |       *(volatile void *)&amp;tmp; });
      |       ^~~~~~~~~~~~~~~~~~~~~~
can't implicitly cast glvalue to prvalue with this cast kind: NoOp
UNREACHABLE executed at /workspace/install/llvm/src/llvm-project/clang/lib/Sema/Sema.cpp:765!
Stack dump:
0.      Program arguments: /workspace/install/llvm/build_21.1.4/bin/clang-21 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -main-file-name 7467.fuzz -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/workspace/exps -fcoverage-compilation-dir=/workspace/exps -resource-dir /workspace/install/llvm/build_21.1.4/lib/clang/21 -internal-isystem /workspace/install/llvm/build_21.1.4/lib/clang/21/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -ferror-limit 19 -fmessage-length=137 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/7467-c69d83.o -x c /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_1_exp/cases/7467.fuzz
1.      /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_1_exp/cases/7467.fuzz:13:31: current parser token '}'
2.      /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_1_exp/cases/7467.fuzz:7:12: parsing function body 'main'
3.      /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_1_exp/cases/7467.fuzz:7:12: in compound statement ('{}')
4.      /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_1_exp/cases/7467.fuzz:9:33: in compound statement ('{}')
 #<!-- -->0 0x0000653223aaac2a llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9691c2a)
 #<!-- -->1 0x0000653223aab457 PrintStackTraceSignalHandler(void*) Signals.cpp:0:0
 #<!-- -->2 0x0000653223aa4f57 llvm::sys::RunSignalHandlers() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x968bf57)
 #<!-- -->3 0x0000653223aa51c7 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->4 0x00007bb4b8da0330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->5 0x00007bb4b8df9b2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->6 0x00007bb4b8df9b2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->7 0x00007bb4b8df9b2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->8 0x00007bb4b8da027e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x00007bb4b8d838ff abort ./stdlib/abort.c:81:7
#<!-- -->10 0x00006532239a26de llvm::llvm_unreachable_internal(char const*, char const*, unsigned int) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x95896de)
#<!-- -->11 0x00006532271df5f4 clang::Sema::ImpCastExprToType(clang::Expr*, clang::QualType, clang::CastKind, clang::ExprValueKind, llvm::SmallVector&lt;clang::CXXBaseSpecifier*, 4u&gt; const*, clang::CheckedConversionKind) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcdc65f4)
#<!-- -->12 0x0000653227768ac8 clang::Sema::CheckSingleAssignmentConstraints(clang::QualType, clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, bool, bool, bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xd34fac8)
#<!-- -->13 0x000065322794e51b clang::InitializationSequence::Perform(clang::Sema&amp;, clang::InitializedEntity const&amp;, clang::InitializationKind const&amp;, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::QualType*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xd53551b)
#<!-- -->14 0x0000653227951b54 clang::Sema::PerformCopyInitialization(clang::InitializedEntity const&amp;, clang::SourceLocation, clang::ActionResult&lt;clang::Expr*, true&gt;, bool, bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xd538b54)
#<!-- -->15 0x00006532277492ba clang::Sema::ActOnStmtExprResult(clang::ActionResult&lt;clang::Expr*, true&gt;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xd3302ba)
#<!-- -->16 0x00006532270f104f clang::Parser::handleExprStmt(clang::ActionResult&lt;clang::Expr*, true&gt;, clang::Parser::ParsedStmtContext) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xccd804f)
#<!-- -->17 0x00006532270f7353 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xccde353)
#<!-- -->18 0x00006532270f9487 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcce0487)
#<!-- -->19 0x00006532270fa099 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcce1099)
#<!-- -->20 0x00006532270fbe9e clang::Parser::ParseCompoundStatementBody(bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcce2e9e)
#<!-- -->21 0x00006532270fc7d0 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::Parser::ParseCompoundStatement(bool, unsigned int)::'lambda'()&gt;(long) ParseStmt.cpp:0:0
#<!-- -->22 0x00006532294be42b clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xf0a542b)
#<!-- -->23 0x00006532270efe0e clang::Parser::ParseCompoundStatement(bool, unsigned int) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xccd6e0e)
#<!-- -->24 0x00006532270efe9e clang::Parser::ParseCompoundStatement(bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xccd6e9e)
#<!-- -->25 0x0000653227045e31 clang::Parser::ParseParenExpression(clang::ParenParseOption&amp;, bool, clang::ParenExprKind, clang::TypoCorrectionTypeBehavior, clang::OpaquePtr&lt;clang::QualType&gt;&amp;, clang::SourceLocation&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc2ce31)
#<!-- -->26 0x000065322703a2da clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&amp;, clang::TypoCorrectionTypeBehavior, bool, bool*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc212da)
#<!-- -->27 0x000065322703d0d1 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc240d1)
#<!-- -->28 0x000065322703aa77 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&amp;, clang::TypoCorrectionTypeBehavior, bool, bool*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc21a77)
#<!-- -->29 0x000065322703d0d1 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc240d1)
#<!-- -->30 0x000065322703d194 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcc24194)
#<!-- -->31 0x0000653226fd79f6 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcbbe9f6)
#<!-- -->32 0x0000653226ff8d1b clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcbdfd1b)
#<!-- -->33 0x0000653226ffaa1f clang::Parser::ParseSimpleDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, bool, clang::Parser::ForRangeInit*, clang::SourceLocation*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcbe1a1f)
#<!-- -->34 0x0000653226ffb069 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcbe2069)
#<!-- -->35 0x00006532270f946c clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcce046c)
#<!-- -->36 0x00006532270fa099 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcce1099)
#<!-- -->37 0x00006532270fbe9e clang::Parser::ParseCompoundStatementBody(bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcce2e9e)
#<!-- -->38 0x00006532270fc8c2 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcce38c2)
#<!-- -->39 0x0000653226fa0337 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb87337)
#<!-- -->40 0x0000653226ff9d18 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcbe0d18)
#<!-- -->41 0x0000653226f96307 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb7d307)
#<!-- -->42 0x0000653226f96e9e clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb7de9e)
#<!-- -->43 0x0000653226fa2f98 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb89f98)
#<!-- -->44 0x0000653226fa459d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb8b59d)
#<!-- -->45 0x0000653226f8a9d0 clang::ParseAST(clang::Sema&amp;, bool, bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb719d0)
#<!-- -->46 0x0000653224cd292b clang::ASTFrontendAction::ExecuteAction() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa8b992b)
#<!-- -->47 0x00006532248c0360 clang::CodeGenAction::ExecuteAction() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa4a7360)
#<!-- -->48 0x0000653224cda9cf clang::FrontendAction::Execute() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa8c19cf)
#<!-- -->49 0x0000653224c20f46 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa807f46)
#<!-- -->50 0x0000653224e62f7b clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xaa49f7b)
#<!-- -->51 0x000065321ebd1523 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47b8523)
#<!-- -->52 0x000065321ebc1c58 ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->53 0x000065321ebc7b28 clang_main(int, char**, llvm::ToolContext const&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47aeb28)
#<!-- -->54 0x000065321ea22b4a main (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x4609b4a)
#<!-- -->55 0x00007bb4b8d851ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->56 0x00007bb4b8d8528b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->57 0x00007bb4b8d8528b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->58 0x000065321ebbff55 _start (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47a6f55)
clang: error: unable to execute command: Aborted (core dumped)
```
## Clang version:
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

### Comment 3 - AbyssStaror


> Did you generate these examples by fuzzer?

These code was generated by our automated code generation pipeline,  a tool which leverages the advanced Large Language Model.


---

### Comment 4 - shafik

> > Did you generate these examples by fuzzer?
> 
> These code was generated by our automated code generation pipeline, a tool which leverages the advanced Large Language Model.

This sounds awfully similar to fuzzing but using a different method. Can you please clarify with more details, so we can better understand. We have limited capacity to deal w/ mass generated bugs. If you are going to generate many of these you need to make sure you are looking for duplicates an if so then please don't create a new bug but add your example to the original bug if it is really novel.

CC @AaronBallman 

---

### Comment 5 - AbyssStaror

> This sounds awfully similar to fuzzing but using a different method. Can you please clarify with more details, so we can better understand.

 We create a bunch of features and select these feature to do the most promising combinations. And then we employ LLMs to generate syntactically valid but semantically complex test cases that specifically target these selected features.




---

### Comment 6 - AbyssStaror

> If you are going to generate many of these you need to make sure you are looking for duplicates an if so then please don't create a new bug but add your example to the original bug if it is really novel.

Thanks for pointing this out. We're working on improving our deduplication. Currently, we check for similar error messages and stack traces. Could you point us to what we should be looking for more closely to accurately identify duplicates? We want to get this right.

---

### Comment 7 - AaronBallman

It's a bit of an art form in some ways; checking for similar (but not identical) assert messages and stack traces is a great first step. You can also sometimes play around with the examples to see if they behave in somewhat similar ways. e.g., "if we make <foo> `const` in both cases, the issue goes away".

---

### Comment 8 - AbyssStaror

> It's a bit of an art form in some ways; checking for similar (but not identical) assert messages and stack traces is a great first step. You can also sometimes play around with the examples to see if they behave in somewhat similar ways. e.g., "if we make `const` in both cases, the issue goes away".

That’s helpful guidance—thank you. I’ll work on incorporating these steps into our process.

---

