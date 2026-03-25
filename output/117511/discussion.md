# clang segfaults with 1,000,000 if statements

**Author:** lenanya
**URL:** https://github.com/llvm/llvm-project/issues/117511

## Body

While doing a 1,000,000 if statement function as a joke, i discovered that clang segfaults during compilation.

Preprocessed Source Code and Build file as the clang said to provide:
[clang_bug_report.zip](https://github.com/user-attachments/files/17895911/clang_bug_report.zip)


## Comments

### Comment 1 - EugeneZelenko

@lenanya: Could you please provide call stack?

---

### Comment 2 - lenanya

This is what clang gave
```
Stack dump:
0.	Program arguments: /usr/bin/clang-18 -cc1 -triple x86_64-pc-linux-gnu -emit-obj -mrelax-all -dumpdir example- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name example.c -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/home/lena/coding/py/iseven -fcoverage-compilation-dir=/home/lena/coding/py/iseven -resource-dir /usr/lib/clang/18 -internal-isystem /usr/lib/clang/18/include -internal-isystem /usr/local/include -internal-isystem /usr/bin/../lib64/gcc/x86_64-pc-linux-gnu/14.2.1/../../../../x86_64-pc-linux-gnu/include -internal-externc-isystem /include -internal-externc-isystem /usr/include -ferror-limit 19 -stack-protector 2 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/example-c438ab.o -x c example.c
1.	./iseven.h:10643:20: current parser token ';'
2.	./iseven.h:5:28: parsing function body 'isEven'
3.	./iseven.h:5:28: in compound statement ('{}')
4.	./iseven.h:10642:32: in compound statement ('{}')
  #0 0x000073878cdc4d90 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /usr/src/debug/llvm/llvm-18.1.8.src/lib/Support/Unix/Signals.inc:723:22
  #1 0x000073878cdc1d4d llvm::sys::RunSignalHandlers() /usr/src/debug/llvm/llvm-18.1.8.src/lib/Support/Signals.cpp:105:20
  #2 0x000073878cdc1d4d SignalHandler /usr/src/debug/llvm/llvm-18.1.8.src/lib/Support/Unix/Signals.inc:403:31
  #3 0x000073878c04c1d0 (/usr/lib/libc.so.6+0x3d1d0)
  #4 0x0000738795b8e19e clang::Sema::DiagnoseAssignmentEnum(clang::QualType, clang::QualType, clang::Expr*) /usr/src/debug/clang/clang-18.1.8.src/lib/Sema/SemaStmt.cpp:1641:0
  #5 0x00007387958e1733 clang::Sema::DiagnoseAssignmentResult(clang::Sema::AssignConvertType, clang::SourceLocation, clang::QualType, clang::QualType, clang::Expr*, clang::Sema::AssignmentAction, bool*) /usr/src/debug/clang/clang-18.1.8.src/lib/Sema/SemaExpr.cpp:17673:0
  #6 0x0000738795a3e8a7 clang::InitializationSequence::Perform(clang::Sema&, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::MutableArrayRef<clang::Expr*>, clang::QualType*) /usr/src/debug/clang/clang-18.1.8.src/lib/Sema/SemaInit.cpp:9192:0
  #7 0x0000738795a4527b clang::Sema::PerformCopyInitialization(clang::InitializedEntity const&, clang::SourceLocation, clang::ActionResult<clang::Expr*, true>, bool, bool) /usr/src/debug/clang/clang-18.1.8.src/lib/Sema/SemaInit.cpp:10568:0
  #8 0x0000738795b934f3 clang::Sema::PerformMoveOrCopyInitialization(clang::InitializedEntity const&, clang::Sema::NamedReturnInfo const&, clang::Expr*, bool) /usr/src/debug/clang/clang-18.1.8.src/lib/Sema/SemaStmt.cpp:3568:0
  #9 0x0000738795ba92c3 clang::Sema::BuildReturnStmt(clang::SourceLocation, clang::Expr*, bool) /usr/src/debug/clang/clang-18.1.8.src/lib/Sema/SemaStmt.cpp:4181:0
 #10 0x0000738795ba984e clang::Sema::ActOnReturnStmt(clang::SourceLocation, clang::Expr*, clang::Scope*) /usr/src/debug/clang/clang-18.1.8.src/lib/Sema/SemaStmt.cpp:3917:0
 #11 0x0000738794e5df2f clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:327:15
 #12 0x0000738794e5e258 llvm::SmallVectorBase<unsigned int>::empty() const /usr/include/llvm/ADT/SmallVector.h:94:46
 #13 0x0000738794e5e258 clang::Parser::MaybeDestroyTemplateIds() /usr/src/debug/clang/clang-18.1.8.src/include/clang/Parse/Parser.h:315:27
 #14 0x0000738794e5e258 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:120:26
 #15 0x0000738794e63b0a clang::Parser::ParseCompoundStatementBody(bool) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:1236:38
 #16 0x0000738794e64184 clang::Parser::ParseScope::Exit() /usr/src/debug/clang/clang-18.1.8.src/include/clang/Parse/Parser.h:1161:24
 #17 0x0000738794e64184 clang::Parser::ParseScope::~ParseScope() /usr/src/debug/clang/clang-18.1.8.src/include/clang/Parse/Parser.h:1167:11
 #18 0x0000738794e64184 clang::Parser::ParseCompoundStatement(bool, unsigned int) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:1031:1
 #19 0x0000738794e5df7f clang::Parser::ParseCompoundStatement(bool) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:996:76
 #20 0x0000738794e5df7f clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:293:34
 #21 0x0000738794e5e258 llvm::SmallVectorBase<unsigned int>::empty() const /usr/include/llvm/ADT/SmallVector.h:94:46
 #22 0x0000738794e5e258 clang::Parser::MaybeDestroyTemplateIds() /usr/src/debug/clang/clang-18.1.8.src/include/clang/Parse/Parser.h:315:27
 #23 0x0000738794e5e258 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:120:26
 #24 0x0000738794e5e770 clang::Parser::ParseStatement(clang::SourceLocation*, clang::Parser::ParsedStmtContext) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:46:29
 #25 0x0000738794e6093a clang::Parser::ParseIfStatement(clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:1596:30
 #26 0x0000738794e5df22 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:300:44
 #27 0x0000738794e5e258 llvm::SmallVectorBase<unsigned int>::empty() const /usr/include/llvm/ADT/SmallVector.h:94:46
 #28 0x0000738794e5e258 clang::Parser::MaybeDestroyTemplateIds() /usr/src/debug/clang/clang-18.1.8.src/include/clang/Parse/Parser.h:315:27
 #29 0x0000738794e5e258 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:120:26
 #30 0x0000738794e5e770 clang::Parser::ParseStatement(clang::SourceLocation*, clang::Parser::ParsedStmtContext) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:46:29
 #31 0x0000738794e60ae3 clang::Parser::ParseIfStatement(clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:1641:30
 #32 0x0000738794e5df22 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:300:44
 #33 0x0000738794e5e258 llvm::SmallVectorBase<unsigned int>::empty() const /usr/include/llvm/ADT/SmallVector.h:94:46
 #34 0x0000738794e5e258 clang::Parser::MaybeDestroyTemplateIds() /usr/src/debug/clang/clang-18.1.8.src/include/clang/Parse/Parser.h:315:27
 #35 0x0000738794e5e258 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:120:26
 #36 0x0000738794e5e770 clang::Parser::ParseStatement(clang::SourceLocation*, clang::Parser::ParsedStmtContext) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:46:29
 #37 0x0000738794e60ae3 clang::Parser::ParseIfStatement(clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:1641:30
 #38 0x0000738794e5df22 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:300:44
 #39 0x0000738794e5e258 llvm::SmallVectorBase<unsigned int>::empty() const /usr/include/llvm/ADT/SmallVector.h:94:46
 #40 0x0000738794e5e258 clang::Parser::MaybeDestroyTemplateIds() /usr/src/debug/clang/clang-18.1.8.src/include/clang/Parse/Parser.h:315:27
 #41 0x0000738794e5e258 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:120:26
 #42 0x0000738794e5e770 clang::Parser::ParseStatement(clang::SourceLocation*, clang::Parser::ParsedStmtContext) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:46:29
 #43 0x0000738794e60ae3 clang::Parser::ParseIfStatement(clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:1641:30
 #44 0x0000738794e5df22 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:300:44
 #45 0x0000738794e5e258 llvm::SmallVectorBase<unsigned int>::empty() const /usr/include/llvm/ADT/SmallVector.h:94:46
 #46 0x0000738794e5e258 clang::Parser::MaybeDestroyTemplateIds() /usr/src/debug/clang/clang-18.1.8.src/include/clang/Parse/Parser.h:315:27
 #47 0x0000738794e5e258 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:120:26
 #48 0x0000738794e5e770 clang::Parser::ParseStatement(clang::SourceLocation*, clang::Parser::ParsedStmtContext) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:46:29
 #49 0x0000738794e60ae3 clang::Parser::ParseIfStatement(clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:1641:30
 #50 0x0000738794e5df22 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:300:44
 #51 0x0000738794e5e258 llvm::SmallVectorBase<unsigned int>::empty() const /usr/include/llvm/ADT/SmallVector.h:94:46
 #52 0x0000738794e5e258 clang::Parser::MaybeDestroyTemplateIds() /usr/src/debug/clang/clang-18.1.8.src/include/clang/Parse/Parser.h:315:27
 #53 0x0000738794e5e258 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:120:26
 #54 0x0000738794e5e770 clang::Parser::ParseStatement(clang::SourceLocation*, clang::Parser::ParsedStmtContext) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:46:29
 #55 0x0000738794e60ae3 clang::Parser::ParseIfStatement(clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:1641:30
 #56 0x0000738794e5df22 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:300:44
 #57 0x0000738794e5e258 llvm::SmallVectorBase<unsigned int>::empty() const /usr/include/llvm/ADT/SmallVector.h:94:46
 #58 0x0000738794e5e258 clang::Parser::MaybeDestroyTemplateIds() /usr/src/debug/clang/clang-18.1.8.src/include/clang/Parse/Parser.h:315:27
 #59 0x0000738794e5e258 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:120:26
 #60 0x0000738794e5e770 clang::Parser::ParseStatement(clang::SourceLocation*, clang::Parser::ParsedStmtContext) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:46:29
 #61 0x0000738794e60ae3 clang::Parser::ParseIfStatement(clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:1641:30
 #62 0x0000738794e5df22 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:300:44
 #63 0x0000738794e5e258 llvm::SmallVectorBase<unsigned int>::empty() const /usr/include/llvm/ADT/SmallVector.h:94:46
 #64 0x0000738794e5e258 clang::Parser::MaybeDestroyTemplateIds() /usr/src/debug/clang/clang-18.1.8.src/include/clang/Parse/Parser.h:315:27
 #65 0x0000738794e5e258 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:120:26
 #66 0x0000738794e5e770 clang::Parser::ParseStatement(clang::SourceLocation*, clang::Parser::ParsedStmtContext) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:46:29
 #67 0x0000738794e60ae3 clang::Parser::ParseIfStatement(clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:1641:30
 #68 0x0000738794e5df22 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:300:44
 #69 0x0000738794e5e258 llvm::SmallVectorBase<unsigned int>::empty() const /usr/include/llvm/ADT/SmallVector.h:94:46
 #70 0x0000738794e5e258 clang::Parser::MaybeDestroyTemplateIds() /usr/src/debug/clang/clang-18.1.8.src/include/clang/Parse/Parser.h:315:27
 #71 0x0000738794e5e258 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:120:26
 #72 0x0000738794e5e770 clang::Parser::ParseStatement(clang::SourceLocation*, clang::Parser::ParsedStmtContext) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:46:29
 #73 0x0000738794e60ae3 clang::Parser::ParseIfStatement(clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:1641:30
 #74 0x0000738794e5df22 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:300:44
 #75 0x0000738794e5e258 llvm::SmallVectorBase<unsigned int>::empty() const /usr/include/llvm/ADT/SmallVector.h:94:46
 #76 0x0000738794e5e258 clang::Parser::MaybeDestroyTemplateIds() /usr/src/debug/clang/clang-18.1.8.src/include/clang/Parse/Parser.h:315:27
 #77 0x0000738794e5e258 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:120:26
 #78 0x0000738794e5e770 clang::Parser::ParseStatement(clang::SourceLocation*, clang::Parser::ParsedStmtContext) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:46:29
 #79 0x0000738794e60ae3 clang::Parser::ParseIfStatement(clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:1641:30
 #80 0x0000738794e5df22 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:300:44
 #81 0x0000738794e5e258 llvm::SmallVectorBase<unsigned int>::empty() const /usr/include/llvm/ADT/SmallVector.h:94:46
 #82 0x0000738794e5e258 clang::Parser::MaybeDestroyTemplateIds() /usr/src/debug/clang/clang-18.1.8.src/include/clang/Parse/Parser.h:315:27
 #83 0x0000738794e5e258 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:120:26
 #84 0x0000738794e5e770 clang::Parser::ParseStatement(clang::SourceLocation*, clang::Parser::ParsedStmtContext) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:46:29
 #85 0x0000738794e60ae3 clang::Parser::ParseIfStatement(clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:1641:30
 #86 0x0000738794e5df22 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:300:44
 #87 0x0000738794e5e258 llvm::SmallVectorBase<unsigned int>::empty() const /usr/include/llvm/ADT/SmallVector.h:94:46
 #88 0x0000738794e5e258 clang::Parser::MaybeDestroyTemplateIds() /usr/src/debug/clang/clang-18.1.8.src/include/clang/Parse/Parser.h:315:27
 #89 0x0000738794e5e258 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:120:26
 #90 0x0000738794e5e770 clang::Parser::ParseStatement(clang::SourceLocation*, clang::Parser::ParsedStmtContext) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:46:29
 #91 0x0000738794e60ae3 clang::Parser::ParseIfStatement(clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:1641:30
 #92 0x0000738794e5df22 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:300:44
 #93 0x0000738794e5e258 llvm::SmallVectorBase<unsigned int>::empty() const /usr/include/llvm/ADT/SmallVector.h:94:46
 #94 0x0000738794e5e258 clang::Parser::MaybeDestroyTemplateIds() /usr/src/debug/clang/clang-18.1.8.src/include/clang/Parse/Parser.h:315:27
 #95 0x0000738794e5e258 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:120:26
 #96 0x0000738794e5e770 clang::Parser::ParseStatement(clang::SourceLocation*, clang::Parser::ParsedStmtContext) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:46:29
 #97 0x0000738794e60ae3 clang::Parser::ParseIfStatement(clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:1641:30
 #98 0x0000738794e5df22 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:300:44
 #99 0x0000738794e5e258 llvm::SmallVectorBase<unsigned int>::empty() const /usr/include/llvm/ADT/SmallVector.h:94:46
#100 0x0000738794e5e258 clang::Parser::MaybeDestroyTemplateIds() /usr/src/debug/clang/clang-18.1.8.src/include/clang/Parse/Parser.h:315:27
#101 0x0000738794e5e258 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:120:26
#102 0x0000738794e5e770 clang::Parser::ParseStatement(clang::SourceLocation*, clang::Parser::ParsedStmtContext) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:46:29
#103 0x0000738794e60ae3 clang::Parser::ParseIfStatement(clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:1641:30
#104 0x0000738794e5df22 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:300:44
#105 0x0000738794e5e258 llvm::SmallVectorBase<unsigned int>::empty() const /usr/include/llvm/ADT/SmallVector.h:94:46
#106 0x0000738794e5e258 clang::Parser::MaybeDestroyTemplateIds() /usr/src/debug/clang/clang-18.1.8.src/include/clang/Parse/Parser.h:315:27
#107 0x0000738794e5e258 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:120:26
#108 0x0000738794e5e770 clang::Parser::ParseStatement(clang::SourceLocation*, clang::Parser::ParsedStmtContext) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:46:29
#109 0x0000738794e60ae3 clang::Parser::ParseIfStatement(clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:1641:30
#110 0x0000738794e5df22 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:300:44
#111 0x0000738794e5e258 llvm::SmallVectorBase<unsigned int>::empty() const /usr/include/llvm/ADT/SmallVector.h:94:46
#112 0x0000738794e5e258 clang::Parser::MaybeDestroyTemplateIds() /usr/src/debug/clang/clang-18.1.8.src/include/clang/Parse/Parser.h:315:27
#113 0x0000738794e5e258 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:120:26
#114 0x0000738794e5e770 clang::Parser::ParseStatement(clang::SourceLocation*, clang::Parser::ParsedStmtContext) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:46:29
#115 0x0000738794e60ae3 clang::Parser::ParseIfStatement(clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:1641:30
#116 0x0000738794e5df22 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:300:44
#117 0x0000738794e5e258 llvm::SmallVectorBase<unsigned int>::empty() const /usr/include/llvm/ADT/SmallVector.h:94:46
#118 0x0000738794e5e258 clang::Parser::MaybeDestroyTemplateIds() /usr/src/debug/clang/clang-18.1.8.src/include/clang/Parse/Parser.h:315:27
#119 0x0000738794e5e258 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:120:26
#120 0x0000738794e5e770 clang::Parser::ParseStatement(clang::SourceLocation*, clang::Parser::ParsedStmtContext) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:46:29
#121 0x0000738794e60ae3 clang::Parser::ParseIfStatement(clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:1641:30
#122 0x0000738794e5df22 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:300:44
#123 0x0000738794e5e258 llvm::SmallVectorBase<unsigned int>::empty() const /usr/include/llvm/ADT/SmallVector.h:94:46
#124 0x0000738794e5e258 clang::Parser::MaybeDestroyTemplateIds() /usr/src/debug/clang/clang-18.1.8.src/include/clang/Parse/Parser.h:315:27
#125 0x0000738794e5e258 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:120:26
#126 0x0000738794e5e770 clang::Parser::ParseStatement(clang::SourceLocation*, clang::Parser::ParsedStmtContext) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:46:29
#127 0x0000738794e60ae3 clang::Parser::ParseIfStatement(clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:1641:30
#128 0x0000738794e5df22 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:300:44
#129 0x0000738794e5e258 llvm::SmallVectorBase<unsigned int>::empty() const /usr/include/llvm/ADT/SmallVector.h:94:46
#130 0x0000738794e5e258 clang::Parser::MaybeDestroyTemplateIds() /usr/src/debug/clang/clang-18.1.8.src/include/clang/Parse/Parser.h:315:27
#131 0x0000738794e5e258 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:120:26
#132 0x0000738794e5e770 clang::Parser::ParseStatement(clang::SourceLocation*, clang::Parser::ParsedStmtContext) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:46:29
#133 0x0000738794e60ae3 clang::Parser::ParseIfStatement(clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:1641:30
#134 0x0000738794e5df22 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:300:44
#135 0x0000738794e5e258 llvm::SmallVectorBase<unsigned int>::empty() const /usr/include/llvm/ADT/SmallVector.h:94:46
#136 0x0000738794e5e258 clang::Parser::MaybeDestroyTemplateIds() /usr/src/debug/clang/clang-18.1.8.src/include/clang/Parse/Parser.h:315:27
#137 0x0000738794e5e258 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:120:26
#138 0x0000738794e5e770 clang::Parser::ParseStatement(clang::SourceLocation*, clang::Parser::ParsedStmtContext) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:46:29
#139 0x0000738794e60ae3 clang::Parser::ParseIfStatement(clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:1641:30
#140 0x0000738794e5df22 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:300:44
#141 0x0000738794e5e258 llvm::SmallVectorBase<unsigned int>::empty() const /usr/include/llvm/ADT/SmallVector.h:94:46
#142 0x0000738794e5e258 clang::Parser::MaybeDestroyTemplateIds() /usr/src/debug/clang/clang-18.1.8.src/include/clang/Parse/Parser.h:315:27
#143 0x0000738794e5e258 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:120:26
#144 0x0000738794e5e770 clang::Parser::ParseStatement(clang::SourceLocation*, clang::Parser::ParsedStmtContext) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:46:29
#145 0x0000738794e60ae3 clang::Parser::ParseIfStatement(clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:1641:30
#146 0x0000738794e5df22 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:300:44
#147 0x0000738794e5e258 llvm::SmallVectorBase<unsigned int>::empty() const /usr/include/llvm/ADT/SmallVector.h:94:46
#148 0x0000738794e5e258 clang::Parser::MaybeDestroyTemplateIds() /usr/src/debug/clang/clang-18.1.8.src/include/clang/Parse/Parser.h:315:27
#149 0x0000738794e5e258 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:120:26
#150 0x0000738794e5e770 clang::Parser::ParseStatement(clang::SourceLocation*, clang::Parser::ParsedStmtContext) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:46:29
#151 0x0000738794e60ae3 clang::Parser::ParseIfStatement(clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:1641:30
#152 0x0000738794e5df22 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:300:44
#153 0x0000738794e5e258 llvm::SmallVectorBase<unsigned int>::empty() const /usr/include/llvm/ADT/SmallVector.h:94:46
#154 0x0000738794e5e258 clang::Parser::MaybeDestroyTemplateIds() /usr/src/debug/clang/clang-18.1.8.src/include/clang/Parse/Parser.h:315:27
#155 0x0000738794e5e258 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:120:26
#156 0x0000738794e5e770 clang::Parser::ParseStatement(clang::SourceLocation*, clang::Parser::ParsedStmtContext) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:46:29
#157 0x0000738794e60ae3 clang::Parser::ParseIfStatement(clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:1641:30
#158 0x0000738794e5df22 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:300:44
#159 0x0000738794e5e258 llvm::SmallVectorBase<unsigned int>::empty() const /usr/include/llvm/ADT/SmallVector.h:94:46
#160 0x0000738794e5e258 clang::Parser::MaybeDestroyTemplateIds() /usr/src/debug/clang/clang-18.1.8.src/include/clang/Parse/Parser.h:315:27
#161 0x0000738794e5e258 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:120:26
#162 0x0000738794e5e770 clang::Parser::ParseStatement(clang::SourceLocation*, clang::Parser::ParsedStmtContext) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:46:29
#163 0x0000738794e60ae3 clang::Parser::ParseIfStatement(clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:1641:30
#164 0x0000738794e5df22 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:300:44
#165 0x0000738794e5e258 llvm::SmallVectorBase<unsigned int>::empty() const /usr/include/llvm/ADT/SmallVector.h:94:46
#166 0x0000738794e5e258 clang::Parser::MaybeDestroyTemplateIds() /usr/src/debug/clang/clang-18.1.8.src/include/clang/Parse/Parser.h:315:27
#167 0x0000738794e5e258 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:120:26
#168 0x0000738794e5e770 clang::Parser::ParseStatement(clang::SourceLocation*, clang::Parser::ParsedStmtContext) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:46:29
#169 0x0000738794e60ae3 clang::Parser::ParseIfStatement(clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:1641:30
#170 0x0000738794e5df22 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:300:44
#171 0x0000738794e5e258 llvm::SmallVectorBase<unsigned int>::empty() const /usr/include/llvm/ADT/SmallVector.h:94:46
#172 0x0000738794e5e258 clang::Parser::MaybeDestroyTemplateIds() /usr/src/debug/clang/clang-18.1.8.src/include/clang/Parse/Parser.h:315:27
#173 0x0000738794e5e258 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:120:26
#174 0x0000738794e5e770 clang::Parser::ParseStatement(clang::SourceLocation*, clang::Parser::ParsedStmtContext) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:46:29
#175 0x0000738794e60ae3 clang::Parser::ParseIfStatement(clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:1641:30
#176 0x0000738794e5df22 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:300:44
#177 0x0000738794e5e258 llvm::SmallVectorBase<unsigned int>::empty() const /usr/include/llvm/ADT/SmallVector.h:94:46
#178 0x0000738794e5e258 clang::Parser::MaybeDestroyTemplateIds() /usr/src/debug/clang/clang-18.1.8.src/include/clang/Parse/Parser.h:315:27
#179 0x0000738794e5e258 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:120:26
#180 0x0000738794e5e770 clang::Parser::ParseStatement(clang::SourceLocation*, clang::Parser::ParsedStmtContext) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:46:29
#181 0x0000738794e60ae3 clang::Parser::ParseIfStatement(clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:1641:30
#182 0x0000738794e5df22 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:300:44
#183 0x0000738794e5e258 llvm::SmallVectorBase<unsigned int>::empty() const /usr/include/llvm/ADT/SmallVector.h:94:46
#184 0x0000738794e5e258 clang::Parser::MaybeDestroyTemplateIds() /usr/src/debug/clang/clang-18.1.8.src/include/clang/Parse/Parser.h:315:27
#185 0x0000738794e5e258 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:120:26
#186 0x0000738794e5e770 clang::Parser::ParseStatement(clang::SourceLocation*, clang::Parser::ParsedStmtContext) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:46:29
#187 0x0000738794e60ae3 clang::Parser::ParseIfStatement(clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:1641:30
#188 0x0000738794e5df22 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:300:44
#189 0x0000738794e5e258 llvm::SmallVectorBase<unsigned int>::empty() const /usr/include/llvm/ADT/SmallVector.h:94:46
#190 0x0000738794e5e258 clang::Parser::MaybeDestroyTemplateIds() /usr/src/debug/clang/clang-18.1.8.src/include/clang/Parse/Parser.h:315:27
#191 0x0000738794e5e258 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:120:26
#192 0x0000738794e5e770 clang::Parser::ParseStatement(clang::SourceLocation*, clang::Parser::ParsedStmtContext) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:46:29
#193 0x0000738794e60ae3 clang::Parser::ParseIfStatement(clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:1641:30
#194 0x0000738794e5df22 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:300:44
#195 0x0000738794e5e258 llvm::SmallVectorBase<unsigned int>::empty() const /usr/include/llvm/ADT/SmallVector.h:94:46
#196 0x0000738794e5e258 clang::Parser::MaybeDestroyTemplateIds() /usr/src/debug/clang/clang-18.1.8.src/include/clang/Parse/Parser.h:315:27
#197 0x0000738794e5e258 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:120:26
#198 0x0000738794e5e770 clang::Parser::ParseStatement(clang::SourceLocation*, clang::Parser::ParsedStmtContext) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:46:29
#199 0x0000738794e60ae3 clang::Parser::ParseIfStatement(clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:1641:30
#200 0x0000738794e5df22 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:300:44
#201 0x0000738794e5e258 llvm::SmallVectorBase<unsigned int>::empty() const /usr/include/llvm/ADT/SmallVector.h:94:46
#202 0x0000738794e5e258 clang::Parser::MaybeDestroyTemplateIds() /usr/src/debug/clang/clang-18.1.8.src/include/clang/Parse/Parser.h:315:27
#203 0x0000738794e5e258 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:120:26
#204 0x0000738794e5e770 clang::Parser::ParseStatement(clang::SourceLocation*, clang::Parser::ParsedStmtContext) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:46:29
#205 0x0000738794e60ae3 clang::Parser::ParseIfStatement(clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:1641:30
#206 0x0000738794e5df22 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:300:44
#207 0x0000738794e5e258 llvm::SmallVectorBase<unsigned int>::empty() const /usr/include/llvm/ADT/SmallVector.h:94:46
#208 0x0000738794e5e258 clang::Parser::MaybeDestroyTemplateIds() /usr/src/debug/clang/clang-18.1.8.src/include/clang/Parse/Parser.h:315:27
#209 0x0000738794e5e258 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:120:26
#210 0x0000738794e5e770 clang::Parser::ParseStatement(clang::SourceLocation*, clang::Parser::ParsedStmtContext) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:46:29
#211 0x0000738794e60ae3 clang::Parser::ParseIfStatement(clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:1641:30
#212 0x0000738794e5df22 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:300:44
#213 0x0000738794e5e258 llvm::SmallVectorBase<unsigned int>::empty() const /usr/include/llvm/ADT/SmallVector.h:94:46
#214 0x0000738794e5e258 clang::Parser::MaybeDestroyTemplateIds() /usr/src/debug/clang/clang-18.1.8.src/include/clang/Parse/Parser.h:315:27
#215 0x0000738794e5e258 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:120:26
#216 0x0000738794e5e770 clang::Parser::ParseStatement(clang::SourceLocation*, clang::Parser::ParsedStmtContext) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:46:29
#217 0x0000738794e60ae3 clang::Parser::ParseIfStatement(clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:1641:30
#218 0x0000738794e5df22 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:300:44
#219 0x0000738794e5e258 llvm::SmallVectorBase<unsigned int>::empty() const /usr/include/llvm/ADT/SmallVector.h:94:46
#220 0x0000738794e5e258 clang::Parser::MaybeDestroyTemplateIds() /usr/src/debug/clang/clang-18.1.8.src/include/clang/Parse/Parser.h:315:27
#221 0x0000738794e5e258 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:120:26
#222 0x0000738794e5e770 clang::Parser::ParseStatement(clang::SourceLocation*, clang::Parser::ParsedStmtContext) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:46:29
#223 0x0000738794e60ae3 clang::Parser::ParseIfStatement(clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:1641:30
#224 0x0000738794e5df22 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:300:44
#225 0x0000738794e5e258 llvm::SmallVectorBase<unsigned int>::empty() const /usr/include/llvm/ADT/SmallVector.h:94:46
#226 0x0000738794e5e258 clang::Parser::MaybeDestroyTemplateIds() /usr/src/debug/clang/clang-18.1.8.src/include/clang/Parse/Parser.h:315:27
#227 0x0000738794e5e258 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:120:26
#228 0x0000738794e5e770 clang::Parser::ParseStatement(clang::SourceLocation*, clang::Parser::ParsedStmtContext) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:46:29
#229 0x0000738794e60ae3 clang::Parser::ParseIfStatement(clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:1641:30
#230 0x0000738794e5df22 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:300:44
#231 0x0000738794e5e258 llvm::SmallVectorBase<unsigned int>::empty() const /usr/include/llvm/ADT/SmallVector.h:94:46
#232 0x0000738794e5e258 clang::Parser::MaybeDestroyTemplateIds() /usr/src/debug/clang/clang-18.1.8.src/include/clang/Parse/Parser.h:315:27
#233 0x0000738794e5e258 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:120:26
#234 0x0000738794e5e770 clang::Parser::ParseStatement(clang::SourceLocation*, clang::Parser::ParsedStmtContext) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:46:29
#235 0x0000738794e60ae3 clang::Parser::ParseIfStatement(clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:1641:30
#236 0x0000738794e5df22 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:300:44
#237 0x0000738794e5e258 llvm::SmallVectorBase<unsigned int>::empty() const /usr/include/llvm/ADT/SmallVector.h:94:46
#238 0x0000738794e5e258 clang::Parser::MaybeDestroyTemplateIds() /usr/src/debug/clang/clang-18.1.8.src/include/clang/Parse/Parser.h:315:27
#239 0x0000738794e5e258 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:120:26
#240 0x0000738794e5e770 clang::Parser::ParseStatement(clang::SourceLocation*, clang::Parser::ParsedStmtContext) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:46:29
#241 0x0000738794e60ae3 clang::Parser::ParseIfStatement(clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:1641:30
#242 0x0000738794e5df22 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:300:44
#243 0x0000738794e5e258 llvm::SmallVectorBase<unsigned int>::empty() const /usr/include/llvm/ADT/SmallVector.h:94:46
#244 0x0000738794e5e258 clang::Parser::MaybeDestroyTemplateIds() /usr/src/debug/clang/clang-18.1.8.src/include/clang/Parse/Parser.h:315:27
#245 0x0000738794e5e258 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:120:26
#246 0x0000738794e5e770 clang::Parser::ParseStatement(clang::SourceLocation*, clang::Parser::ParsedStmtContext) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:46:29
#247 0x0000738794e60ae3 clang::Parser::ParseIfStatement(clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:1641:30
#248 0x0000738794e5df22 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:300:44
#249 0x0000738794e5e258 llvm::SmallVectorBase<unsigned int>::empty() const /usr/include/llvm/ADT/SmallVector.h:94:46
#250 0x0000738794e5e258 clang::Parser::MaybeDestroyTemplateIds() /usr/src/debug/clang/clang-18.1.8.src/include/clang/Parse/Parser.h:315:27
#251 0x0000738794e5e258 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:120:26
#252 0x0000738794e5e770 clang::Parser::ParseStatement(clang::SourceLocation*, clang::Parser::ParsedStmtContext) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:46:29
#253 0x0000738794e60ae3 clang::Parser::ParseIfStatement(clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:1641:30
#254 0x0000738794e5df22 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:300:44
#255 0x0000738794e5e258 llvm::SmallVectorBase<unsigned int>::empty() const /usr/include/llvm/ADT/SmallVector.h:94:46
#256 0x0000738794e5e258 clang::Parser::MaybeDestroyTemplateIds() /usr/src/debug/clang/clang-18.1.8.src/include/clang/Parse/Parser.h:315:27
#257 0x0000738794e5e258 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:120:26
#258 0x0000738794e5e770 clang::Parser::ParseStatement(clang::SourceLocation*, clang::Parser::ParsedStmtContext) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:46:29
#259 0x0000738794e60ae3 clang::Parser::ParseIfStatement(clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:1641:30
#260 0x0000738794e5df22 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:300:44
#261 0x0000738794e5e258 llvm::SmallVectorBase<unsigned int>::empty() const /usr/include/llvm/ADT/SmallVector.h:94:46
#262 0x0000738794e5e258 clang::Parser::MaybeDestroyTemplateIds() /usr/src/debug/clang/clang-18.1.8.src/include/clang/Parse/Parser.h:315:27
#263 0x0000738794e5e258 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:120:26
#264 0x0000738794e5e770 clang::Parser::ParseStatement(clang::SourceLocation*, clang::Parser::ParsedStmtContext) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:46:29
#265 0x0000738794e60ae3 clang::Parser::ParseIfStatement(clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:1641:30
#266 0x0000738794e5df22 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:300:44
#267 0x0000738794e5e258 llvm::SmallVectorBase<unsigned int>::empty() const /usr/include/llvm/ADT/SmallVector.h:94:46
#268 0x0000738794e5e258 clang::Parser::MaybeDestroyTemplateIds() /usr/src/debug/clang/clang-18.1.8.src/include/clang/Parse/Parser.h:315:27
#269 0x0000738794e5e258 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:120:26
#270 0x0000738794e5e770 clang::Parser::ParseStatement(clang::SourceLocation*, clang::Parser::ParsedStmtContext) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:46:29
#271 0x0000738794e60ae3 clang::Parser::ParseIfStatement(clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:1641:30
#272 0x0000738794e5df22 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:300:44
#273 0x0000738794e5e258 llvm::SmallVectorBase<unsigned int>::empty() const /usr/include/llvm/ADT/SmallVector.h:94:46
#274 0x0000738794e5e258 clang::Parser::MaybeDestroyTemplateIds() /usr/src/debug/clang/clang-18.1.8.src/include/clang/Parse/Parser.h:315:27
#275 0x0000738794e5e258 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:120:26
#276 0x0000738794e5e770 clang::Parser::ParseStatement(clang::SourceLocation*, clang::Parser::ParsedStmtContext) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:46:29
#277 0x0000738794e60ae3 clang::Parser::ParseIfStatement(clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:1641:30
#278 0x0000738794e5df22 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:300:44
#279 0x0000738794e5e258 llvm::SmallVectorBase<unsigned int>::empty() const /usr/include/llvm/ADT/SmallVector.h:94:46
#280 0x0000738794e5e258 clang::Parser::MaybeDestroyTemplateIds() /usr/src/debug/clang/clang-18.1.8.src/include/clang/Parse/Parser.h:315:27
#281 0x0000738794e5e258 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:120:26
#282 0x0000738794e5e770 clang::Parser::ParseStatement(clang::SourceLocation*, clang::Parser::ParsedStmtContext) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:46:29
#283 0x0000738794e60ae3 clang::Parser::ParseIfStatement(clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:1641:30
#284 0x0000738794e5df22 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:300:44
#285 0x0000738794e5e258 llvm::SmallVectorBase<unsigned int>::empty() const /usr/include/llvm/ADT/SmallVector.h:94:46
#286 0x0000738794e5e258 clang::Parser::MaybeDestroyTemplateIds() /usr/src/debug/clang/clang-18.1.8.src/include/clang/Parse/Parser.h:315:27
#287 0x0000738794e5e258 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:120:26
#288 0x0000738794e5e770 clang::Parser::ParseStatement(clang::SourceLocation*, clang::Parser::ParsedStmtContext) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:46:29
#289 0x0000738794e60ae3 clang::Parser::ParseIfStatement(clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:1641:30
#290 0x0000738794e5df22 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:300:44
#291 0x0000738794e5e258 llvm::SmallVectorBase<unsigned int>::empty() const /usr/include/llvm/ADT/SmallVector.h:94:46
#292 0x0000738794e5e258 clang::Parser::MaybeDestroyTemplateIds() /usr/src/debug/clang/clang-18.1.8.src/include/clang/Parse/Parser.h:315:27
#293 0x0000738794e5e258 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:120:26
#294 0x0000738794e5e770 clang::Parser::ParseStatement(clang::SourceLocation*, clang::Parser::ParsedStmtContext) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:46:29
#295 0x0000738794e60ae3 clang::Parser::ParseIfStatement(clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:1641:30
#296 0x0000738794e5df22 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:300:44
#297 0x0000738794e5e258 llvm::SmallVectorBase<unsigned int>::empty() const /usr/include/llvm/ADT/SmallVector.h:94:46
#298 0x0000738794e5e258 clang::Parser::MaybeDestroyTemplateIds() /usr/src/debug/clang/clang-18.1.8.src/include/clang/Parse/Parser.h:315:27
#299 0x0000738794e5e258 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:120:26
#300 0x0000738794e5e770 clang::Parser::ParseStatement(clang::SourceLocation*, clang::Parser::ParsedStmtContext) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:46:29
#301 0x0000738794e60ae3 clang::Parser::ParseIfStatement(clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:1641:30
#302 0x0000738794e5df22 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:300:44
#303 0x0000738794e5e258 llvm::SmallVectorBase<unsigned int>::empty() const /usr/include/llvm/ADT/SmallVector.h:94:46
#304 0x0000738794e5e258 clang::Parser::MaybeDestroyTemplateIds() /usr/src/debug/clang/clang-18.1.8.src/include/clang/Parse/Parser.h:315:27
#305 0x0000738794e5e258 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:120:26
#306 0x0000738794e5e770 clang::Parser::ParseStatement(clang::SourceLocation*, clang::Parser::ParsedStmtContext) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:46:29
#307 0x0000738794e60ae3 clang::Parser::ParseIfStatement(clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:1641:30
#308 0x0000738794e5df22 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:300:44
#309 0x0000738794e5e258 llvm::SmallVectorBase<unsigned int>::empty() const /usr/include/llvm/ADT/SmallVector.h:94:46
#310 0x0000738794e5e258 clang::Parser::MaybeDestroyTemplateIds() /usr/src/debug/clang/clang-18.1.8.src/include/clang/Parse/Parser.h:315:27
#311 0x0000738794e5e258 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:120:26
#312 0x0000738794e5e770 clang::Parser::ParseStatement(clang::SourceLocation*, clang::Parser::ParsedStmtContext) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:46:29
#313 0x0000738794e60ae3 clang::Parser::ParseIfStatement(clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:1641:30
#314 0x0000738794e5df22 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:300:44
#315 0x0000738794e5e258 llvm::SmallVectorBase<unsigned int>::empty() const /usr/include/llvm/ADT/SmallVector.h:94:46
#316 0x0000738794e5e258 clang::Parser::MaybeDestroyTemplateIds() /usr/src/debug/clang/clang-18.1.8.src/include/clang/Parse/Parser.h:315:27
#317 0x0000738794e5e258 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:120:26
#318 0x0000738794e5e770 clang::Parser::ParseStatement(clang::SourceLocation*, clang::Parser::ParsedStmtContext) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:46:29
#319 0x0000738794e60ae3 clang::Parser::ParseIfStatement(clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:1641:30
#320 0x0000738794e5df22 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:300:44
#321 0x0000738794e5e258 llvm::SmallVectorBase<unsigned int>::empty() const /usr/include/llvm/ADT/SmallVector.h:94:46
#322 0x0000738794e5e258 clang::Parser::MaybeDestroyTemplateIds() /usr/src/debug/clang/clang-18.1.8.src/include/clang/Parse/Parser.h:315:27
#323 0x0000738794e5e258 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:120:26
#324 0x0000738794e5e770 clang::Parser::ParseStatement(clang::SourceLocation*, clang::Parser::ParsedStmtContext) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:46:29
#325 0x0000738794e60ae3 clang::Parser::ParseIfStatement(clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:1641:30
#326 0x0000738794e5df22 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:300:44
#327 0x0000738794e5e258 llvm::SmallVectorBase<unsigned int>::empty() const /usr/include/llvm/ADT/SmallVector.h:94:46
#328 0x0000738794e5e258 clang::Parser::MaybeDestroyTemplateIds() /usr/src/debug/clang/clang-18.1.8.src/include/clang/Parse/Parser.h:315:27
#329 0x0000738794e5e258 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:120:26
#330 0x0000738794e5e770 clang::Parser::ParseStatement(clang::SourceLocation*, clang::Parser::ParsedStmtContext) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:46:29
#331 0x0000738794e60ae3 clang::Parser::ParseIfStatement(clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:1641:30
#332 0x0000738794e5df22 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:300:44
#333 0x0000738794e5e258 llvm::SmallVectorBase<unsigned int>::empty() const /usr/include/llvm/ADT/SmallVector.h:94:46
#334 0x0000738794e5e258 clang::Parser::MaybeDestroyTemplateIds() /usr/src/debug/clang/clang-18.1.8.src/include/clang/Parse/Parser.h:315:27
#335 0x0000738794e5e258 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:120:26
#336 0x0000738794e5e770 clang::Parser::ParseStatement(clang::SourceLocation*, clang::Parser::ParsedStmtContext) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:46:29
#337 0x0000738794e60ae3 clang::Parser::ParseIfStatement(clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:1641:30
#338 0x0000738794e5df22 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:300:44
#339 0x0000738794e5e258 llvm::SmallVectorBase<unsigned int>::empty() const /usr/include/llvm/ADT/SmallVector.h:94:46
#340 0x0000738794e5e258 clang::Parser::MaybeDestroyTemplateIds() /usr/src/debug/clang/clang-18.1.8.src/include/clang/Parse/Parser.h:315:27
#341 0x0000738794e5e258 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:120:26
#342 0x0000738794e5e770 clang::Parser::ParseStatement(clang::SourceLocation*, clang::Parser::ParsedStmtContext) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:46:29
#343 0x0000738794e60ae3 clang::Parser::ParseIfStatement(clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:1641:30
#344 0x0000738794e5df22 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:300:44
#345 0x0000738794e5e258 llvm::SmallVectorBase<unsigned int>::empty() const /usr/include/llvm/ADT/SmallVector.h:94:46
#346 0x0000738794e5e258 clang::Parser::MaybeDestroyTemplateIds() /usr/src/debug/clang/clang-18.1.8.src/include/clang/Parse/Parser.h:315:27
#347 0x0000738794e5e258 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:120:26
#348 0x0000738794e5e770 clang::Parser::ParseStatement(clang::SourceLocation*, clang::Parser::ParsedStmtContext) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:46:29
#349 0x0000738794e60ae3 clang::Parser::ParseIfStatement(clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:1641:30
#350 0x0000738794e5df22 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:300:44
#351 0x0000738794e5e258 llvm::SmallVectorBase<unsigned int>::empty() const /usr/include/llvm/ADT/SmallVector.h:94:46
#352 0x0000738794e5e258 clang::Parser::MaybeDestroyTemplateIds() /usr/src/debug/clang/clang-18.1.8.src/include/clang/Parse/Parser.h:315:27
#353 0x0000738794e5e258 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:120:26
#354 0x0000738794e5e770 clang::Parser::ParseStatement(clang::SourceLocation*, clang::Parser::ParsedStmtContext) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:46:29
#355 0x0000738794e60ae3 clang::Parser::ParseIfStatement(clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:1641:30
#356 0x0000738794e5df22 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:300:44
#357 0x0000738794e5e258 llvm::SmallVectorBase<unsigned int>::empty() const /usr/include/llvm/ADT/SmallVector.h:94:46
#358 0x0000738794e5e258 clang::Parser::MaybeDestroyTemplateIds() /usr/src/debug/clang/clang-18.1.8.src/include/clang/Parse/Parser.h:315:27
#359 0x0000738794e5e258 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:120:26
#360 0x0000738794e5e770 clang::Parser::ParseStatement(clang::SourceLocation*, clang::Parser::ParsedStmtContext) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:46:29
#361 0x0000738794e60ae3 clang::Parser::ParseIfStatement(clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:1641:30
#362 0x0000738794e5df22 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:300:44
#363 0x0000738794e5e258 llvm::SmallVectorBase<unsigned int>::empty() const /usr/include/llvm/ADT/SmallVector.h:94:46
#364 0x0000738794e5e258 clang::Parser::MaybeDestroyTemplateIds() /usr/src/debug/clang/clang-18.1.8.src/include/clang/Parse/Parser.h:315:27
#365 0x0000738794e5e258 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:120:26
#366 0x0000738794e5e770 clang::Parser::ParseStatement(clang::SourceLocation*, clang::Parser::ParsedStmtContext) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:46:29
#367 0x0000738794e60ae3 clang::Parser::ParseIfStatement(clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:1641:30
#368 0x0000738794e5df22 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:300:44
#369 0x0000738794e5e258 llvm::SmallVectorBase<unsigned int>::empty() const /usr/include/llvm/ADT/SmallVector.h:94:46
#370 0x0000738794e5e258 clang::Parser::MaybeDestroyTemplateIds() /usr/src/debug/clang/clang-18.1.8.src/include/clang/Parse/Parser.h:315:27
#371 0x0000738794e5e258 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:120:26
#372 0x0000738794e5e770 clang::Parser::ParseStatement(clang::SourceLocation*, clang::Parser::ParsedStmtContext) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:46:29
#373 0x0000738794e60ae3 clang::Parser::ParseIfStatement(clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:1641:30
#374 0x0000738794e5df22 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:300:44
#375 0x0000738794e5e258 llvm::SmallVectorBase<unsigned int>::empty() const /usr/include/llvm/ADT/SmallVector.h:94:46
#376 0x0000738794e5e258 clang::Parser::MaybeDestroyTemplateIds() /usr/src/debug/clang/clang-18.1.8.src/include/clang/Parse/Parser.h:315:27
#377 0x0000738794e5e258 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:120:26
#378 0x0000738794e5e770 clang::Parser::ParseStatement(clang::SourceLocation*, clang::Parser::ParsedStmtContext) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:46:29
#379 0x0000738794e60ae3 clang::Parser::ParseIfStatement(clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:1641:30
#380 0x0000738794e5df22 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:300:44
#381 0x0000738794e5e258 llvm::SmallVectorBase<unsigned int>::empty() const /usr/include/llvm/ADT/SmallVector.h:94:46
#382 0x0000738794e5e258 clang::Parser::MaybeDestroyTemplateIds() /usr/src/debug/clang/clang-18.1.8.src/include/clang/Parse/Parser.h:315:27
#383 0x0000738794e5e258 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) /usr/src/debug/clang/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:120:26
```

---

### Comment 3 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: lenanya (lenanya)

<details>
While doing a 1,000,000 if statement function as a joke, i discovered that clang segfaults during compilation.

Preprocessed Source Code and Build file as the clang said to provide:
[clang_bug_report.zip](https://github.com/user-attachments/files/17895911/clang_bug_report.zip)

</details>


---

### Comment 4 - asl

Looks like stack overflow

---

### Comment 5 - jandevaan

The code looks like this:
 
```
bool isEven(size_t number) {
    if (number == 0) {
        return true;
    } else if (number == 1) {
        return false;
    } else if (number == 2) {
        return true;
    } else if (number == 3) {
        return false;
    } else if (number == 4) {
        return true;
    } else if (number == 5) {
        return false;
    } else if (number == 6) {
        return true;
```

So the if's are nested in the else clauses. I googled if the standards impose limits, according to [microsoft](https://learn.microsoft.com/en-us/cpp/cpp/compiler-limits?view=msvc-170), C++11 limits nesting of control structures to 256 levels. 
However, this does not apply here as this is a C program. From what I gather C does not have such limits. 

A possible fix would be to produce an error message when the parse depth is too deep (I'd expect something around 10k levels would not segfault on most platforms, and would allow realistic code to compile. 

~~I am interested to contribute to LLVM (not speciffically this issue), is this worthwhile to pursue?~~

---

