# Crash during code completion on code with C-style cast

**Author:** HighCommander4
**URL:** https://github.com/llvm/llvm-project/issues/180125
**Status:** Closed
**Labels:** clang:frontend, confirmed, crash, objective-c
**Closed Date:** 2026-02-15T23:30:05Z

## Body

Given `test.h` with the following contents:

```c
void crash_test() {
	int* a = (int*)(0x200);
};
```

Run:

```console
clang -cc1 -fsyntax-only -xobjective-c++-header -code-completion-at=test.h:2:24 test.h
```

The command crashes with the following output:

```console
clang: clang/include/clang/AST/TypeBase.h:953: const ExtQualsTypeCommonBase *clang::QualType::getCommonPtr() const: Assertion `!isNull() && "Cannot retrieve a NULL type pointer"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: clang -cc1 -fsyntax-only -xobjective-c++-header -code-completion-at=test.h:2:26 test.h
1.      <eof> parser at end of file
2.      test.h:1:19: parsing function body 'crash_test'
3.      test.h:1:19: in compound statement ('{}')
 #0 0x00007f884e0c7bb9 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) llvm/lib/Support/Unix/Signals.inc:842:11
 #1 0x00007f884e0c80db PrintStackTraceSignalHandler(void*) llvm/lib/Support/Unix/Signals.inc:924:1
 #2 0x00007f884e0c5fd4 llvm::sys::RunSignalHandlers() llvm/lib/Support/Signals.cpp:108:5
 #3 0x00007f884e0c8799 SignalHandler(int, siginfo_t*, void*) llvm/lib/Support/Unix/Signals.inc:429:38
 #4 0x00007f884d65a050 (/lib/x86_64-linux-gnu/libc.so.6+0x3c050)
 #5 0x00007f884d6a8eec __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #6 0x00007f884d659fb2 raise ./signal/../sysdeps/posix/raise.c:27:6
 #7 0x00007f884d644472 abort ./stdlib/abort.c:81:7
 #8 0x00007f884d644395 _nl_load_domain ./intl/loadmsgcat.c:1177:9
 #9 0x00007f884d652ec2 (/lib/x86_64-linux-gnu/libc.so.6+0x34ec2)
#10 0x00007f88470f9a16 clang::QualType::getCommonPtr() const clang/include/clang/AST/TypeBase.h:0:5
#11 0x00007f88470f78c5 clang::QualType::getTypePtr() const clang/include/clang/AST/TypeBase.h:8293:26
#12 0x00007f88470f75a5 clang::QualType::operator->() const clang/include/clang/AST/TypeBase.h:997:5
#13 0x00007f88473c32f2 clang::Expr::hasPlaceholderType() const clang/include/clang/AST/Expr.h:524:12
#14 0x00007f884781676b clang::Sema::DefaultFunctionArrayConversion(clang::Expr*, bool) clang/lib/Sema/SemaExpr.cpp:517:10
#15 0x00007f8847818eb0 clang::Sema::DefaultFunctionArrayLvalueConversion(clang::Expr*, bool) clang/lib/Sema/SemaExpr.cpp:755:20
#16 0x00007f88473ef7fe clang::SemaCodeCompletion::CodeCompleteObjCInstanceMessage(clang::Scope*, clang::Expr*, llvm::ArrayRef<clang::IdentifierInfo const*>, bool, clang::ObjCInterfaceDecl*) clang/lib/Sema/SemaCodeComplete.cpp:8445:31
#17 0x00007f88473ef789 clang::SemaCodeCompletion::CodeCompletePostfixExpression(clang::Scope*, clang::ActionResult<clang::Expr*, true>, clang::QualType) clang/lib/Sema/SemaCodeComplete.cpp:0:5
#18 0x00007f8848586cd1 clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult<clang::Expr*, true>) clang/lib/Parse/ParseExpr.cpp:1655:14
#19 0x00007f884858cc62 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypoCorrectionTypeBehavior, bool, bool*) clang/lib/Parse/ParseExpr.cpp:1622:9
#20 0x00007f88485869cc clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) clang/lib/Parse/ParseExpr.cpp:567:20
#21 0x00007f8848590369 clang::Parser::ParseParenExpression(clang::ParenParseOption&, bool, clang::ParenExprKind, clang::TypoCorrectionTypeBehavior, clang::OpaquePtr<clang::QualType>&, clang::SourceLocation&) clang/lib/Parse/ParseExpr.cpp:2853:18
#22 0x00007f884858992a clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypoCorrectionTypeBehavior, bool, bool*) clang/lib/Parse/ParseExpr.cpp:763:11
#23 0x00007f88485869cc clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) clang/lib/Parse/ParseExpr.cpp:567:20
#24 0x00007f88485853f6 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) clang/lib/Parse/ParseExpr.cpp:90:7
#25 0x00007f88485c96bd clang::Parser::ParseInitializer(clang::Decl*) clang/lib/Parse/ParseInit.cpp:600:12
#26 0x00007f8848531ca1 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) clang/lib/Parse/ParseDecl.cpp:2615:25
#27 0x00007f8848530494 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) clang/lib/Parse/ParseDecl.cpp:2354:9
#28 0x00007f884852f08b clang::Parser::ParseSimpleDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, bool, clang::Parser::ForRangeInit*, clang::SourceLocation*) clang/lib/Parse/ParseDecl.cpp:1977:10
#29 0x00007f884852ec95 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) clang/lib/Parse/ParseDecl.cpp:1921:12
#30 0x00007f884863b20c clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::LabelDecl*) clang/lib/Parse/ParseStmt.cpp:223:16
#31 0x00007f884863ab47 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) clang/lib/Parse/ParseStmt.cpp:78:20
#32 0x00007f8848644256 clang::Parser::ParseCompoundStatementBody(bool) clang/lib/Parse/ParseStmt.cpp:1165:11
#33 0x00007f8848645792 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) clang/lib/Parse/ParseStmt.cpp:2455:21
#34 0x00007f884866e0f1 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) clang/lib/Parse/Parser.cpp:1441:3
#35 0x00007f884852ff7d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) clang/lib/Parse/ParseDecl.cpp:2264:21
#36 0x00007f884866d07d clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) clang/lib/Parse/Parser.cpp:1180:10
#37 0x00007f884866c54b clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) clang/lib/Parse/Parser.cpp:1202:12
#38 0x00007f884866be44 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) clang/lib/Parse/Parser.cpp:1030:14
#39 0x00007f8848669cc4 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) clang/lib/Parse/Parser.cpp:743:12
#40 0x00007f8848669460 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) clang/lib/Parse/Parser.cpp:599:8
#41 0x00007f8848508bd9 clang::ParseAST(clang::Sema&, bool, bool) clang/lib/Parse/ParseAST.cpp:169:15
#42 0x00007f88531b2589 clang::ASTFrontendAction::ExecuteAction() clang/lib/Frontend/FrontendAction.cpp:1435:1
#43 0x00007f88531b1f66 clang::FrontendAction::Execute() clang/lib/Frontend/FrontendAction.cpp:1317:7
#44 0x00007f88530a75b3 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) clang/lib/Frontend/CompilerInstance.cpp:1006:23
#45 0x00007f88599b7f25 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) clang/lib/FrontendTool/ExecuteCompilerInvocation.cpp:310:8
#46 0x00005650c1a18c26 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) clang/tools/driver/cc1_main.cpp:304:13
#47 0x00005650c1a0c3c8 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) clang/tools/driver/driver.cpp:226:5
#48 0x00005650c1a0b1c6 clang_main(int, char**, llvm::ToolContext const&) clang/tools/driver/driver.cpp:274:5
```

## Comments

### Comment 1 - HighCommander4

Tested with recent trunk.

Not a recent regression, issue goes back to at least clang 14.

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Nathan Ridge (HighCommander4)

<details>
Given `test.h` with the following contents:

```c
void crash_test() {
	int* a = (int*)(0x200);
};
```

Run:

```console
clang -cc1 -fsyntax-only -xobjective-c++-header -code-completion-at=test.h:2:24 test.h
```

The command crashes with the following output:

```console
clang: clang/include/clang/AST/TypeBase.h:953: const ExtQualsTypeCommonBase *clang::QualType::getCommonPtr() const: Assertion `!isNull() &amp;&amp; "Cannot retrieve a NULL type pointer"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: clang -cc1 -fsyntax-only -xobjective-c++-header -code-completion-at=test.h:2:26 test.h
1.      &lt;eof&gt; parser at end of file
2.      test.h:1:19: parsing function body 'crash_test'
3.      test.h:1:19: in compound statement ('{}')
 #<!-- -->0 0x00007f884e0c7bb9 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) llvm/lib/Support/Unix/Signals.inc:842:11
 #<!-- -->1 0x00007f884e0c80db PrintStackTraceSignalHandler(void*) llvm/lib/Support/Unix/Signals.inc:924:1
 #<!-- -->2 0x00007f884e0c5fd4 llvm::sys::RunSignalHandlers() llvm/lib/Support/Signals.cpp:108:5
 #<!-- -->3 0x00007f884e0c8799 SignalHandler(int, siginfo_t*, void*) llvm/lib/Support/Unix/Signals.inc:429:38
 #<!-- -->4 0x00007f884d65a050 (/lib/x86_64-linux-gnu/libc.so.6+0x3c050)
 #<!-- -->5 0x00007f884d6a8eec __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->6 0x00007f884d659fb2 raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->7 0x00007f884d644472 abort ./stdlib/abort.c:81:7
 #<!-- -->8 0x00007f884d644395 _nl_load_domain ./intl/loadmsgcat.c:1177:9
 #<!-- -->9 0x00007f884d652ec2 (/lib/x86_64-linux-gnu/libc.so.6+0x34ec2)
#<!-- -->10 0x00007f88470f9a16 clang::QualType::getCommonPtr() const clang/include/clang/AST/TypeBase.h:0:5
#<!-- -->11 0x00007f88470f78c5 clang::QualType::getTypePtr() const clang/include/clang/AST/TypeBase.h:8293:26
#<!-- -->12 0x00007f88470f75a5 clang::QualType::operator-&gt;() const clang/include/clang/AST/TypeBase.h:997:5
#<!-- -->13 0x00007f88473c32f2 clang::Expr::hasPlaceholderType() const clang/include/clang/AST/Expr.h:524:12
#<!-- -->14 0x00007f884781676b clang::Sema::DefaultFunctionArrayConversion(clang::Expr*, bool) clang/lib/Sema/SemaExpr.cpp:517:10
#<!-- -->15 0x00007f8847818eb0 clang::Sema::DefaultFunctionArrayLvalueConversion(clang::Expr*, bool) clang/lib/Sema/SemaExpr.cpp:755:20
#<!-- -->16 0x00007f88473ef7fe clang::SemaCodeCompletion::CodeCompleteObjCInstanceMessage(clang::Scope*, clang::Expr*, llvm::ArrayRef&lt;clang::IdentifierInfo const*&gt;, bool, clang::ObjCInterfaceDecl*) clang/lib/Sema/SemaCodeComplete.cpp:8445:31
#<!-- -->17 0x00007f88473ef789 clang::SemaCodeCompletion::CodeCompletePostfixExpression(clang::Scope*, clang::ActionResult&lt;clang::Expr*, true&gt;, clang::QualType) clang/lib/Sema/SemaCodeComplete.cpp:0:5
#<!-- -->18 0x00007f8848586cd1 clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult&lt;clang::Expr*, true&gt;) clang/lib/Parse/ParseExpr.cpp:1655:14
#<!-- -->19 0x00007f884858cc62 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&amp;, clang::TypoCorrectionTypeBehavior, bool, bool*) clang/lib/Parse/ParseExpr.cpp:1622:9
#<!-- -->20 0x00007f88485869cc clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) clang/lib/Parse/ParseExpr.cpp:567:20
#<!-- -->21 0x00007f8848590369 clang::Parser::ParseParenExpression(clang::ParenParseOption&amp;, bool, clang::ParenExprKind, clang::TypoCorrectionTypeBehavior, clang::OpaquePtr&lt;clang::QualType&gt;&amp;, clang::SourceLocation&amp;) clang/lib/Parse/ParseExpr.cpp:2853:18
#<!-- -->22 0x00007f884858992a clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&amp;, clang::TypoCorrectionTypeBehavior, bool, bool*) clang/lib/Parse/ParseExpr.cpp:763:11
#<!-- -->23 0x00007f88485869cc clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) clang/lib/Parse/ParseExpr.cpp:567:20
#<!-- -->24 0x00007f88485853f6 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) clang/lib/Parse/ParseExpr.cpp:90:7
#<!-- -->25 0x00007f88485c96bd clang::Parser::ParseInitializer(clang::Decl*) clang/lib/Parse/ParseInit.cpp:600:12
#<!-- -->26 0x00007f8848531ca1 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) clang/lib/Parse/ParseDecl.cpp:2615:25
#<!-- -->27 0x00007f8848530494 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) clang/lib/Parse/ParseDecl.cpp:2354:9
#<!-- -->28 0x00007f884852f08b clang::Parser::ParseSimpleDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, bool, clang::Parser::ForRangeInit*, clang::SourceLocation*) clang/lib/Parse/ParseDecl.cpp:1977:10
#<!-- -->29 0x00007f884852ec95 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) clang/lib/Parse/ParseDecl.cpp:1921:12
#<!-- -->30 0x00007f884863b20c clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::LabelDecl*) clang/lib/Parse/ParseStmt.cpp:223:16
#<!-- -->31 0x00007f884863ab47 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) clang/lib/Parse/ParseStmt.cpp:78:20
#<!-- -->32 0x00007f8848644256 clang::Parser::ParseCompoundStatementBody(bool) clang/lib/Parse/ParseStmt.cpp:1165:11
#<!-- -->33 0x00007f8848645792 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) clang/lib/Parse/ParseStmt.cpp:2455:21
#<!-- -->34 0x00007f884866e0f1 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) clang/lib/Parse/Parser.cpp:1441:3
#<!-- -->35 0x00007f884852ff7d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) clang/lib/Parse/ParseDecl.cpp:2264:21
#<!-- -->36 0x00007f884866d07d clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) clang/lib/Parse/Parser.cpp:1180:10
#<!-- -->37 0x00007f884866c54b clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) clang/lib/Parse/Parser.cpp:1202:12
#<!-- -->38 0x00007f884866be44 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) clang/lib/Parse/Parser.cpp:1030:14
#<!-- -->39 0x00007f8848669cc4 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) clang/lib/Parse/Parser.cpp:743:12
#<!-- -->40 0x00007f8848669460 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) clang/lib/Parse/Parser.cpp:599:8
#<!-- -->41 0x00007f8848508bd9 clang::ParseAST(clang::Sema&amp;, bool, bool) clang/lib/Parse/ParseAST.cpp:169:15
#<!-- -->42 0x00007f88531b2589 clang::ASTFrontendAction::ExecuteAction() clang/lib/Frontend/FrontendAction.cpp:1435:1
#<!-- -->43 0x00007f88531b1f66 clang::FrontendAction::Execute() clang/lib/Frontend/FrontendAction.cpp:1317:7
#<!-- -->44 0x00007f88530a75b3 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) clang/lib/Frontend/CompilerInstance.cpp:1006:23
#<!-- -->45 0x00007f88599b7f25 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) clang/lib/FrontendTool/ExecuteCompilerInvocation.cpp:310:8
#<!-- -->46 0x00005650c1a18c26 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) clang/tools/driver/cc1_main.cpp:304:13
#<!-- -->47 0x00005650c1a0c3c8 ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) clang/tools/driver/driver.cpp:226:5
#<!-- -->48 0x00005650c1a0b1c6 clang_main(int, char**, llvm::ToolContext const&amp;) clang/tools/driver/driver.cpp:274:5
```
</details>


---

### Comment 3 - shafik

Can you reproduce on godbolt?

---

### Comment 4 - HighCommander4

> Can you reproduce on godbolt?

The crash only occurs when doing code completion. I'm not sure how to trigger code completion on godbolt.

---

### Comment 5 - zwuis

https://godbolt.org/z/YrP85ana3

---

### Comment 6 - HighCommander4

> https://godbolt.org/z/YrP85ana3

Thanks!

(I was trying to pass `-cc1` which godbolt rejected. I forgot that `-Xclang -code-completion-at` is another way to do it.)

---

### Comment 7 - edisongz

I’d like to work on this issue. Could you please assign it to me?

---

### Comment 8 - HighCommander4

> I’d like to work on this issue.

Sure, please go ahead.

> Could you please assign it to me?

[Current guidance](https://discourse.llvm.org/t/some-questions-about-the-issue-assignment/89334/6) is to assign issues only after a PR is posted.

---

### Comment 9 - shafik

So this only happens w/ `-xobjective-c++-header` so I think the objective-c labels is appropriate, right?

---

### Comment 10 - HighCommander4

> So this only happens w/ `-xobjective-c++-header` so I think the objective-c labels is appropriate, right?

Yep, added. (`-x-objective-c-header` triggers it as well.)

---

### Comment 11 - HighCommander4

(And just as some added background: this issue was hit by a clangd user. Clangd uses `-xobjective-c++-header` to process `.h` files for which there is no `compile_commands.json` file to get actual flags from, because objective-c++ is sort of a "superset" language for the various languages for which an `.h` file could contain code.)

---

### Comment 12 - HighCommander4

Thank you @edisongz for fixing!

---

