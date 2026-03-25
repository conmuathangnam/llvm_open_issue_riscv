# [Clang] `if constexpr` inside variable template of generic lambda leads to assertion error or invalid instantiations

**Author:** MitalAshok
**URL:** https://github.com/llvm/llvm-project/issues/97881

## Body

This has an assertion error: https://godbolt.org/z/bbd89P7jc

```c++
template<int N>
auto g = [](int i) {
  if constexpr (N < 4) {
    g<N+1>(i);
  }
};

int main() {
    g<0>(0);
}
```

But it compiles correctly without assertions enabled.

This has an assertion error too: https://godbolt.org/z/j1Kr8vK67

```c++
template<int N>
auto g = [](auto i) {
  if (N < sizeof(i)) {
    g<N+1>(i);
  }
};

int main() {
    g<0>(0);
}
```

```
clang++: /llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:4417: void clang::LocalInstantiationScope::InstantiatedLocal(const clang::Decl *, clang::Decl *): Assertion `!Current->LocalDecls.contains(D) && "Instantiated local in inner and outer scopes"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /cmake-build-debug/bin/clang++ -cc1 -x c++ -fsyntax-only -std=c++20 test.cpp
1.      test.cpp:9:9: current parser token '('
2.      test.cpp:8:12: parsing function body 'main'
3.      test.cpp:8:12: in compound statement ('{}')
 #0 0x000056316d15163d llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /llvm-project/llvm/lib/Support/Unix/Signals.inc:723:11
 #1 0x000056316d151b2b PrintStackTraceSignalHandler(void*) /llvm-project/llvm/lib/Support/Unix/Signals.inc:798:1
 #2 0x000056316d14fb96 llvm::sys::RunSignalHandlers() /llvm-project/llvm/lib/Support/Signals.cpp:105:5
 #3 0x000056316d1522e5 SignalHandler(int) /llvm-project/llvm/lib/Support/Unix/Signals.inc:413:1
 #4 0x00007fa1db517520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #5 0x00007fa1db56b9fc __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #6 0x00007fa1db56b9fc __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #7 0x00007fa1db56b9fc pthread_kill ./nptl/pthread_kill.c:89:10
 #8 0x00007fa1db517476 gsignal ./signal/../sysdeps/posix/raise.c:27:6
 #9 0x00007fa1db4fd7f3 abort ./stdlib/abort.c:81:7
#10 0x00007fa1db4fd71b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#11 0x00007fa1db50ee96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
#12 0x000056317341ec19 clang::LocalInstantiationScope::InstantiatedLocal(clang::Decl const*, clang::Decl*) /llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:4414:5
#13 0x0000563173447d83 (anonymous namespace)::TemplateInstantiator::transformedLocalDecl(clang::Decl*, llvm::ArrayRef<clang::Decl*>) /llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:1480:44
#14 0x0000563173448cf3 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformLambdaExpr(clang::LambdaExpr*) /llvm-project/clang/lib/Sema/TreeTransform.h:14370:16
#15 0x0000563173435a65 (anonymous namespace)::TemplateInstantiator::TransformLambdaExpr(clang::LambdaExpr*) /llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:1659:38
#16 0x000056317342611b clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) /cmake-build-debug/tools/clang/include/clang/AST/StmtNodes.inc:340:1
#17 0x00005631734274ce clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformInitializer(clang::Expr*, bool) /llvm-project/clang/lib/Sema/TreeTransform.h:4188:25
#18 0x0000563173421819 clang::Sema::SubstInitializer(clang::Expr*, clang::MultiLevelTemplateArgumentList const&, bool) /llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:4288:23
#19 0x00005631734ba2b2 clang::Sema::InstantiateVariableInitializer(clang::VarDecl*, clang::VarDecl*, clang::MultiLevelTemplateArgumentList const&) /llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:5423:14
#20 0x00005631734abefc clang::Sema::BuildVariableInstantiation(clang::VarDecl*, clang::VarDecl*, clang::MultiLevelTemplateArgumentList const&, llvm::SmallVector<clang::Sema::LateInstantiatedAttribute, 16u>*, clang::DeclContext*, clang::LocalInstantiationScope*, bool, clang::VarTemplateSpecializationDecl*) /llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:5381:3
#21 0x00005631734bc041 clang::TemplateDeclInstantiator::VisitVarTemplateSpecializationDecl(clang::VarTemplateDecl*, clang::VarDecl*, clang::TemplateArgumentListInfo const&, llvm::ArrayRef<clang::TemplateArgument>, clang::VarTemplateSpecializationDecl*) /llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:4043:10
#22 0x00005631734bfa60 clang::Sema::BuildVarTemplateInstantiation(clang::VarTemplateDecl*, clang::VarDecl*, clang::TemplateArgumentList const*, clang::TemplateArgumentListInfo const&, llvm::SmallVectorImpl<clang::TemplateArgument>&, clang::SourceLocation, llvm::SmallVector<clang::Sema::LateInstantiatedAttribute, 16u>*, clang::LocalInstantiationScope*) /llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:5237:20
#23 0x000056317308b51b clang::Sema::CheckVarTemplateId(clang::VarTemplateDecl*, clang::SourceLocation, clang::SourceLocation, clang::TemplateArgumentListInfo const&) /llvm-project/clang/lib/Sema/SemaTemplate.cpp:5483:34
#24 0x000056317308bb30 clang::Sema::CheckVarTemplateId(clang::CXXScopeSpec const&, clang::DeclarationNameInfo const&, clang::VarTemplateDecl*, clang::NamedDecl*, clang::SourceLocation, clang::TemplateArgumentListInfo const*) /llvm-project/clang/lib/Sema/SemaTemplate.cpp:5518:21
#25 0x000056317308c5b9 clang::Sema::BuildTemplateIdExpr(clang::CXXScopeSpec const&, clang::SourceLocation, clang::LookupResult&, bool, clang::TemplateArgumentListInfo const*) /llvm-project/clang/lib/Sema/SemaTemplate.cpp:5631:22
#26 0x0000563173440cd2 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::RebuildTemplateIdExpr(clang::CXXScopeSpec const&, clang::SourceLocation, clang::LookupResult&, bool, clang::TemplateArgumentListInfo const*) /llvm-project/clang/lib/Sema/TreeTransform.h:3498:22
#27 0x0000563173440476 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformUnresolvedLookupExpr(clang::UnresolvedLookupExpr*, bool) /llvm-project/clang/lib/Sema/TreeTransform.h:13689:23
#28 0x000056317343228f clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformUnresolvedLookupExpr(clang::UnresolvedLookupExpr*) /llvm-project/clang/lib/Sema/TreeTransform.h:13623:10
#29 0x0000563173425c21 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) /cmake-build-debug/tools/clang/include/clang/AST/StmtNodes.inc:180:1
#30 0x0000563173438622 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCallExpr(clang::CallExpr*) /llvm-project/clang/lib/Sema/TreeTransform.h:12254:36
#31 0x0000563173426830 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) /cmake-build-debug/tools/clang/include/clang/AST/StmtNodes.inc:602:1
#32 0x00005631734248e8 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformStmt(clang::Stmt*, clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::StmtDiscardKind) /llvm-project/clang/lib/Sema/TreeTransform.h:4107:35
#33 0x0000563173441b36 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCompoundStmt(clang::CompoundStmt*, bool) /llvm-project/clang/lib/Sema/TreeTransform.h:7819:38
#34 0x0000563173473207 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCompoundStmt(clang::CompoundStmt*) /llvm-project/clang/lib/Sema/TreeTransform.h:7801:23
#35 0x0000563173424760 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformStmt(clang::Stmt*, clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::StmtDiscardKind) /cmake-build-debug/tools/clang/include/clang/AST/StmtNodes.inc:1538:1
#36 0x00005631734716b6 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformIfStmt(clang::IfStmt*) /llvm-project/clang/lib/Sema/TreeTransform.h:8025:25
#37 0x00005631734245d8 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformStmt(clang::Stmt*, clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::StmtDiscardKind) /cmake-build-debug/tools/clang/include/clang/AST/StmtNodes.inc:1490:1
#38 0x0000563173441b36 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCompoundStmt(clang::CompoundStmt*, bool) /llvm-project/clang/lib/Sema/TreeTransform.h:7819:38
#39 0x0000563173473207 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCompoundStmt(clang::CompoundStmt*) /llvm-project/clang/lib/Sema/TreeTransform.h:7801:23
#40 0x0000563173424760 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformStmt(clang::Stmt*, clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::StmtDiscardKind) /cmake-build-debug/tools/clang/include/clang/AST/StmtNodes.inc:1538:1
#41 0x00005631734548b3 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformLambdaBody(clang::LambdaExpr*, clang::Stmt*) /llvm-project/clang/lib/Sema/TreeTransform.h:14682:10
#42 0x000056317344ac05 (anonymous namespace)::TemplateInstantiator::TransformLambdaBody(clang::LambdaExpr*, clang::Stmt*) /llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:1698:36
#43 0x000056317344a0b3 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformLambdaExpr(clang::LambdaExpr*) /llvm-project/clang/lib/Sema/TreeTransform.h:14613:44
#44 0x0000563173435a65 (anonymous namespace)::TemplateInstantiator::TransformLambdaExpr(clang::LambdaExpr*) /llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:1659:38
#45 0x000056317342611b clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) /cmake-build-debug/tools/clang/include/clang/AST/StmtNodes.inc:340:1
#46 0x00005631734274ce clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformInitializer(clang::Expr*, bool) /llvm-project/clang/lib/Sema/TreeTransform.h:4188:25
#47 0x0000563173421819 clang::Sema::SubstInitializer(clang::Expr*, clang::MultiLevelTemplateArgumentList const&, bool) /llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:4288:23
#48 0x00005631734ba2b2 clang::Sema::InstantiateVariableInitializer(clang::VarDecl*, clang::VarDecl*, clang::MultiLevelTemplateArgumentList const&) /llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:5423:14
#49 0x00005631734abefc clang::Sema::BuildVariableInstantiation(clang::VarDecl*, clang::VarDecl*, clang::MultiLevelTemplateArgumentList const&, llvm::SmallVector<clang::Sema::LateInstantiatedAttribute, 16u>*, clang::DeclContext*, clang::LocalInstantiationScope*, bool, clang::VarTemplateSpecializationDecl*) /llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:5381:3
#50 0x00005631734bc041 clang::TemplateDeclInstantiator::VisitVarTemplateSpecializationDecl(clang::VarTemplateDecl*, clang::VarDecl*, clang::TemplateArgumentListInfo const&, llvm::ArrayRef<clang::TemplateArgument>, clang::VarTemplateSpecializationDecl*) /llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:4043:10
#51 0x00005631734bfa60 clang::Sema::BuildVarTemplateInstantiation(clang::VarTemplateDecl*, clang::VarDecl*, clang::TemplateArgumentList const*, clang::TemplateArgumentListInfo const&, llvm::SmallVectorImpl<clang::TemplateArgument>&, clang::SourceLocation, llvm::SmallVector<clang::Sema::LateInstantiatedAttribute, 16u>*, clang::LocalInstantiationScope*) /llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:5237:20
#52 0x000056317308b51b clang::Sema::CheckVarTemplateId(clang::VarTemplateDecl*, clang::SourceLocation, clang::SourceLocation, clang::TemplateArgumentListInfo const&) /llvm-project/clang/lib/Sema/SemaTemplate.cpp:5483:34
#53 0x000056317308bb30 clang::Sema::CheckVarTemplateId(clang::CXXScopeSpec const&, clang::DeclarationNameInfo const&, clang::VarTemplateDecl*, clang::NamedDecl*, clang::SourceLocation, clang::TemplateArgumentListInfo const*) /llvm-project/clang/lib/Sema/SemaTemplate.cpp:5518:21
#54 0x000056317308c5b9 clang::Sema::BuildTemplateIdExpr(clang::CXXScopeSpec const&, clang::SourceLocation, clang::LookupResult&, bool, clang::TemplateArgumentListInfo const*) /llvm-project/clang/lib/Sema/SemaTemplate.cpp:5631:22
#55 0x00005631726cc15d clang::Sema::ActOnIdExpression(clang::Scope*, clang::CXXScopeSpec&, clang::SourceLocation, clang::UnqualifiedId&, bool, bool, clang::CorrectionCandidateCallback*, bool, clang::Token*) /llvm-project/clang/lib/Sema/SemaExpr.cpp:2857:12
#56 0x0000563171c2f05b clang::Parser::tryParseCXXIdExpression(clang::CXXScopeSpec&, bool, clang::Token&) /llvm-project/clang/lib/Parse/ParseExprCXX.cpp:658:17
#57 0x0000563171c30a5a clang::Parser::ParseCXXIdExpression(bool) /llvm-project/clang/lib/Parse/ParseExprCXX.cpp:751:7
#58 0x0000563171c1a234 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) /llvm-project/clang/lib/Parse/ParseExpr.cpp:1720:11
#59 0x0000563171c11b19 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) /llvm-project/clang/lib/Parse/ParseExpr.cpp:712:20
#60 0x0000563171c10011 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) /llvm-project/clang/lib/Parse/ParseExpr.cpp:182:20
#61 0x0000563171c0febf clang::Parser::ParseExpression(clang::Parser::TypeCastState) /llvm-project/clang/lib/Parse/ParseExpr.cpp:133:18
#62 0x0000563171cabfa8 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) /llvm-project/clang/lib/Parse/ParseStmt.cpp:551:19
#63 0x0000563171caa35a clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) /llvm-project/clang/lib/Parse/ParseStmt.cpp:289:14
#64 0x0000563171ca99cb clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) /llvm-project/clang/lib/Parse/ParseStmt.cpp:121:20
#65 0x0000563171cb2c42 clang::Parser::ParseCompoundStatementBody(bool) /llvm-project/clang/lib/Parse/ParseStmt.cpp:1245:11
#66 0x0000563171cb4357 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) /llvm-project/clang/lib/Parse/ParseStmt.cpp:2523:21
#67 0x0000563171bd8580 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) /llvm-project/clang/lib/Parse/Parser.cpp:1525:3
#68 0x0000563171c814f9 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) /llvm-project/clang/lib/Parse/ParseDecl.cpp:2418:21
#69 0x0000563171bd733c clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) /llvm-project/clang/lib/Parse/Parser.cpp:1249:10
#70 0x0000563171bd681e clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) /llvm-project/clang/lib/Parse/Parser.cpp:1271:12
#71 0x0000563171bd60a9 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) /llvm-project/clang/lib/Parse/Parser.cpp:1074:14
#72 0x0000563171bd3ecc clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) /llvm-project/clang/lib/Parse/Parser.cpp:763:12
#73 0x0000563171bcebed clang::ParseAST(clang::Sema&, bool, bool) /llvm-project/clang/lib/Parse/ParseAST.cpp:163:16
#74 0x000056316e6960f1 clang::ASTFrontendAction::ExecuteAction() /llvm-project/clang/lib/Frontend/FrontendAction.cpp:1194:1
#75 0x000056316e695b0c clang::FrontendAction::Execute() /llvm-project/clang/lib/Frontend/FrontendAction.cpp:1082:7
#76 0x000056316e5b1e19 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) /llvm-project/clang/lib/Frontend/CompilerInstance.cpp:1061:23
#77 0x000056316e87215e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) /llvm-project/clang/lib/FrontendTool/ExecuteCompilerInvocation.cpp:280:8
#78 0x000056316b274a81 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) /llvm-project/clang/tools/driver/cc1_main.cpp:276:13
#79 0x000056316b2673f2 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) /llvm-project/clang/tools/driver/driver.cpp:215:5
#80 0x000056316b26615b clang_main(int, char**, llvm::ToolContext const&) /llvm-project/clang/tools/driver/driver.cpp:256:5
#81 0x000056316b29ba05 main /cmake-build-debug/tools/clang/tools/driver/clang-driver.cpp:17:3
#82 0x00007fa1db4fed90 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:58:16
#83 0x00007fa1db4fee40 call_init ./csu/../csu/libc-start.c:128:20
#84 0x00007fa1db4fee40 __libc_start_main ./csu/../csu/libc-start.c:379:5
#85 0x000056316b265be5 _start (/cmake-build-debug/bin/clang+++0x5b6abe5)
```

But when run without assertions it seems to ignore the `if constexpr` and instantiate `g<N+1>` forever: https://godbolt.org/z/hhhhb8fhj

```
<source>:4:5: warning: stack nearly exhausted; compilation time may suffer, and crashes due to stack overflow are likely [-Wstack-exhausted]
    4 |     g<N+1>(i);
      |     ^
<source>:4:5: note: in instantiation of variable template specialization 'g' requested here
<source>:2:21: note: while substituting into a lambda expression here
    2 | auto g = [](auto i) {
      |                     ^
<source>:4:5: note: in instantiation of variable template specialization 'g' requested here
    4 |     g<N+1>(i);
      |     ^
<source>:2:21: note: while substituting into a lambda expression here
    2 | auto g = [](auto i) {
      |                     ^
<source>:4:5: note: in instantiation of variable template specialization 'g' requested here
    4 |     g<N+1>(i);
      |     ^
<source>:2:21: note: (skipping 1741 contexts in backtrace; use -ftemplate-backtrace-limit=0 to see all)
    2 | auto g = [](auto i) {
      |                     ^
<source>:4:5: note: in instantiation of variable template specialization 'g' requested here
    4 |     g<N+1>(i);
      |     ^
<source>:2:21: note: while substituting into a lambda expression here
    2 | auto g = [](auto i) {
      |                     ^
<source>:4:5: note: in instantiation of variable template specialization 'g' requested here
    4 |     g<N+1>(i);
      |     ^
<source>:2:21: note: while substituting into a lambda expression here
    2 | auto g = [](auto i) {
      |                     ^
<source>:9:5: note: in instantiation of variable template specialization 'g' requested here
    9 |     g<0>(0);
      |     ^
```

---

This one has no assertion error but still compiles wrong: https://godbolt.org/z/n8P4reaob

```c++
template<int X>
void f() {
  static_assert(X != sizeof(int));
}

template<>
void f<sizeof(int)>() = delete;

template<int N>
auto g = [](auto i) {
  if constexpr (N != sizeof(i)) {
    f<N>();
  }
};

int main() {
  g<0>(0);
  g<sizeof(int)>(0);
}
```

```
<source>:12:5: error: call to deleted function 'f'
   12 |     f<N>();
      |     ^~~~
<source>:10:21: note: while substituting into a lambda expression here
   10 | auto g = [](auto i) {
      |                     ^
<source>:18:3: note: in instantiation of variable template specialization 'g' requested here
   18 |   g<sizeof(int)>(0);
      |   ^
<source>:7:6: note: candidate function [with X = 4] has been implicitly deleted
    7 | void f<sizeof(int)>() = delete;
      |      ^
```

So seems like it's a problem when the `if constexpr` is dependent both on the variable template and the generic lambda

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Mital Ashok (MitalAshok)

<details>
This has an assertion error: https://godbolt.org/z/bbd89P7jc

```c++
template&lt;int N&gt;
auto g = [](int i) {
  if constexpr (N &lt; 4) {
    g&lt;N+1&gt;(i);
  }
};

int main() {
    g&lt;0&gt;(0);
}
```

But it compiles correctly without assertions enabled.

This has an assertion error too: https://godbolt.org/z/j1Kr8vK67

```c++
template&lt;int N&gt;
auto g = [](auto i) {
  if (N &lt; sizeof(i)) {
    g&lt;N+1&gt;(i);
  }
};

int main() {
    g&lt;0&gt;(0);
}
```

```
clang++: /llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:4417: void clang::LocalInstantiationScope::InstantiatedLocal(const clang::Decl *, clang::Decl *): Assertion `!Current-&gt;LocalDecls.contains(D) &amp;&amp; "Instantiated local in inner and outer scopes"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /cmake-build-debug/bin/clang++ -cc1 -x c++ -fsyntax-only -std=c++20 test.cpp
1.      test.cpp:9:9: current parser token '('
2.      test.cpp:8:12: parsing function body 'main'
3.      test.cpp:8:12: in compound statement ('{}')
 #<!-- -->0 0x000056316d15163d llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /llvm-project/llvm/lib/Support/Unix/Signals.inc:723:11
 #<!-- -->1 0x000056316d151b2b PrintStackTraceSignalHandler(void*) /llvm-project/llvm/lib/Support/Unix/Signals.inc:798:1
 #<!-- -->2 0x000056316d14fb96 llvm::sys::RunSignalHandlers() /llvm-project/llvm/lib/Support/Signals.cpp:105:5
 #<!-- -->3 0x000056316d1522e5 SignalHandler(int) /llvm-project/llvm/lib/Support/Unix/Signals.inc:413:1
 #<!-- -->4 0x00007fa1db517520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->5 0x00007fa1db56b9fc __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->6 0x00007fa1db56b9fc __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->7 0x00007fa1db56b9fc pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->8 0x00007fa1db517476 gsignal ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x00007fa1db4fd7f3 abort ./stdlib/abort.c:81:7
#<!-- -->10 0x00007fa1db4fd71b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->11 0x00007fa1db50ee96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
#<!-- -->12 0x000056317341ec19 clang::LocalInstantiationScope::InstantiatedLocal(clang::Decl const*, clang::Decl*) /llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:4414:5
#<!-- -->13 0x0000563173447d83 (anonymous namespace)::TemplateInstantiator::transformedLocalDecl(clang::Decl*, llvm::ArrayRef&lt;clang::Decl*&gt;) /llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:1480:44
#<!-- -->14 0x0000563173448cf3 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformLambdaExpr(clang::LambdaExpr*) /llvm-project/clang/lib/Sema/TreeTransform.h:14370:16
#<!-- -->15 0x0000563173435a65 (anonymous namespace)::TemplateInstantiator::TransformLambdaExpr(clang::LambdaExpr*) /llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:1659:38
#<!-- -->16 0x000056317342611b clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) /cmake-build-debug/tools/clang/include/clang/AST/StmtNodes.inc:340:1
#<!-- -->17 0x00005631734274ce clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformInitializer(clang::Expr*, bool) /llvm-project/clang/lib/Sema/TreeTransform.h:4188:25
#<!-- -->18 0x0000563173421819 clang::Sema::SubstInitializer(clang::Expr*, clang::MultiLevelTemplateArgumentList const&amp;, bool) /llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:4288:23
#<!-- -->19 0x00005631734ba2b2 clang::Sema::InstantiateVariableInitializer(clang::VarDecl*, clang::VarDecl*, clang::MultiLevelTemplateArgumentList const&amp;) /llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:5423:14
#<!-- -->20 0x00005631734abefc clang::Sema::BuildVariableInstantiation(clang::VarDecl*, clang::VarDecl*, clang::MultiLevelTemplateArgumentList const&amp;, llvm::SmallVector&lt;clang::Sema::LateInstantiatedAttribute, 16u&gt;*, clang::DeclContext*, clang::LocalInstantiationScope*, bool, clang::VarTemplateSpecializationDecl*) /llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:5381:3
#<!-- -->21 0x00005631734bc041 clang::TemplateDeclInstantiator::VisitVarTemplateSpecializationDecl(clang::VarTemplateDecl*, clang::VarDecl*, clang::TemplateArgumentListInfo const&amp;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::VarTemplateSpecializationDecl*) /llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:4043:10
#<!-- -->22 0x00005631734bfa60 clang::Sema::BuildVarTemplateInstantiation(clang::VarTemplateDecl*, clang::VarDecl*, clang::TemplateArgumentList const*, clang::TemplateArgumentListInfo const&amp;, llvm::SmallVectorImpl&lt;clang::TemplateArgument&gt;&amp;, clang::SourceLocation, llvm::SmallVector&lt;clang::Sema::LateInstantiatedAttribute, 16u&gt;*, clang::LocalInstantiationScope*) /llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:5237:20
#<!-- -->23 0x000056317308b51b clang::Sema::CheckVarTemplateId(clang::VarTemplateDecl*, clang::SourceLocation, clang::SourceLocation, clang::TemplateArgumentListInfo const&amp;) /llvm-project/clang/lib/Sema/SemaTemplate.cpp:5483:34
#<!-- -->24 0x000056317308bb30 clang::Sema::CheckVarTemplateId(clang::CXXScopeSpec const&amp;, clang::DeclarationNameInfo const&amp;, clang::VarTemplateDecl*, clang::NamedDecl*, clang::SourceLocation, clang::TemplateArgumentListInfo const*) /llvm-project/clang/lib/Sema/SemaTemplate.cpp:5518:21
#<!-- -->25 0x000056317308c5b9 clang::Sema::BuildTemplateIdExpr(clang::CXXScopeSpec const&amp;, clang::SourceLocation, clang::LookupResult&amp;, bool, clang::TemplateArgumentListInfo const*) /llvm-project/clang/lib/Sema/SemaTemplate.cpp:5631:22
#<!-- -->26 0x0000563173440cd2 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::RebuildTemplateIdExpr(clang::CXXScopeSpec const&amp;, clang::SourceLocation, clang::LookupResult&amp;, bool, clang::TemplateArgumentListInfo const*) /llvm-project/clang/lib/Sema/TreeTransform.h:3498:22
#<!-- -->27 0x0000563173440476 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformUnresolvedLookupExpr(clang::UnresolvedLookupExpr*, bool) /llvm-project/clang/lib/Sema/TreeTransform.h:13689:23
#<!-- -->28 0x000056317343228f clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformUnresolvedLookupExpr(clang::UnresolvedLookupExpr*) /llvm-project/clang/lib/Sema/TreeTransform.h:13623:10
#<!-- -->29 0x0000563173425c21 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) /cmake-build-debug/tools/clang/include/clang/AST/StmtNodes.inc:180:1
#<!-- -->30 0x0000563173438622 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCallExpr(clang::CallExpr*) /llvm-project/clang/lib/Sema/TreeTransform.h:12254:36
#<!-- -->31 0x0000563173426830 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) /cmake-build-debug/tools/clang/include/clang/AST/StmtNodes.inc:602:1
#<!-- -->32 0x00005631734248e8 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformStmt(clang::Stmt*, clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::StmtDiscardKind) /llvm-project/clang/lib/Sema/TreeTransform.h:4107:35
#<!-- -->33 0x0000563173441b36 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCompoundStmt(clang::CompoundStmt*, bool) /llvm-project/clang/lib/Sema/TreeTransform.h:7819:38
#<!-- -->34 0x0000563173473207 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCompoundStmt(clang::CompoundStmt*) /llvm-project/clang/lib/Sema/TreeTransform.h:7801:23
#<!-- -->35 0x0000563173424760 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformStmt(clang::Stmt*, clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::StmtDiscardKind) /cmake-build-debug/tools/clang/include/clang/AST/StmtNodes.inc:1538:1
#<!-- -->36 0x00005631734716b6 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformIfStmt(clang::IfStmt*) /llvm-project/clang/lib/Sema/TreeTransform.h:8025:25
#<!-- -->37 0x00005631734245d8 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformStmt(clang::Stmt*, clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::StmtDiscardKind) /cmake-build-debug/tools/clang/include/clang/AST/StmtNodes.inc:1490:1
#<!-- -->38 0x0000563173441b36 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCompoundStmt(clang::CompoundStmt*, bool) /llvm-project/clang/lib/Sema/TreeTransform.h:7819:38
#<!-- -->39 0x0000563173473207 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCompoundStmt(clang::CompoundStmt*) /llvm-project/clang/lib/Sema/TreeTransform.h:7801:23
#<!-- -->40 0x0000563173424760 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformStmt(clang::Stmt*, clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::StmtDiscardKind) /cmake-build-debug/tools/clang/include/clang/AST/StmtNodes.inc:1538:1
#<!-- -->41 0x00005631734548b3 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformLambdaBody(clang::LambdaExpr*, clang::Stmt*) /llvm-project/clang/lib/Sema/TreeTransform.h:14682:10
#<!-- -->42 0x000056317344ac05 (anonymous namespace)::TemplateInstantiator::TransformLambdaBody(clang::LambdaExpr*, clang::Stmt*) /llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:1698:36
#<!-- -->43 0x000056317344a0b3 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformLambdaExpr(clang::LambdaExpr*) /llvm-project/clang/lib/Sema/TreeTransform.h:14613:44
#<!-- -->44 0x0000563173435a65 (anonymous namespace)::TemplateInstantiator::TransformLambdaExpr(clang::LambdaExpr*) /llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:1659:38
#<!-- -->45 0x000056317342611b clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) /cmake-build-debug/tools/clang/include/clang/AST/StmtNodes.inc:340:1
#<!-- -->46 0x00005631734274ce clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformInitializer(clang::Expr*, bool) /llvm-project/clang/lib/Sema/TreeTransform.h:4188:25
#<!-- -->47 0x0000563173421819 clang::Sema::SubstInitializer(clang::Expr*, clang::MultiLevelTemplateArgumentList const&amp;, bool) /llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:4288:23
#<!-- -->48 0x00005631734ba2b2 clang::Sema::InstantiateVariableInitializer(clang::VarDecl*, clang::VarDecl*, clang::MultiLevelTemplateArgumentList const&amp;) /llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:5423:14
#<!-- -->49 0x00005631734abefc clang::Sema::BuildVariableInstantiation(clang::VarDecl*, clang::VarDecl*, clang::MultiLevelTemplateArgumentList const&amp;, llvm::SmallVector&lt;clang::Sema::LateInstantiatedAttribute, 16u&gt;*, clang::DeclContext*, clang::LocalInstantiationScope*, bool, clang::VarTemplateSpecializationDecl*) /llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:5381:3
#<!-- -->50 0x00005631734bc041 clang::TemplateDeclInstantiator::VisitVarTemplateSpecializationDecl(clang::VarTemplateDecl*, clang::VarDecl*, clang::TemplateArgumentListInfo const&amp;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::VarTemplateSpecializationDecl*) /llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:4043:10
#<!-- -->51 0x00005631734bfa60 clang::Sema::BuildVarTemplateInstantiation(clang::VarTemplateDecl*, clang::VarDecl*, clang::TemplateArgumentList const*, clang::TemplateArgumentListInfo const&amp;, llvm::SmallVectorImpl&lt;clang::TemplateArgument&gt;&amp;, clang::SourceLocation, llvm::SmallVector&lt;clang::Sema::LateInstantiatedAttribute, 16u&gt;*, clang::LocalInstantiationScope*) /llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:5237:20
#<!-- -->52 0x000056317308b51b clang::Sema::CheckVarTemplateId(clang::VarTemplateDecl*, clang::SourceLocation, clang::SourceLocation, clang::TemplateArgumentListInfo const&amp;) /llvm-project/clang/lib/Sema/SemaTemplate.cpp:5483:34
#<!-- -->53 0x000056317308bb30 clang::Sema::CheckVarTemplateId(clang::CXXScopeSpec const&amp;, clang::DeclarationNameInfo const&amp;, clang::VarTemplateDecl*, clang::NamedDecl*, clang::SourceLocation, clang::TemplateArgumentListInfo const*) /llvm-project/clang/lib/Sema/SemaTemplate.cpp:5518:21
#<!-- -->54 0x000056317308c5b9 clang::Sema::BuildTemplateIdExpr(clang::CXXScopeSpec const&amp;, clang::SourceLocation, clang::LookupResult&amp;, bool, clang::TemplateArgumentListInfo const*) /llvm-project/clang/lib/Sema/SemaTemplate.cpp:5631:22
#<!-- -->55 0x00005631726cc15d clang::Sema::ActOnIdExpression(clang::Scope*, clang::CXXScopeSpec&amp;, clang::SourceLocation, clang::UnqualifiedId&amp;, bool, bool, clang::CorrectionCandidateCallback*, bool, clang::Token*) /llvm-project/clang/lib/Sema/SemaExpr.cpp:2857:12
#<!-- -->56 0x0000563171c2f05b clang::Parser::tryParseCXXIdExpression(clang::CXXScopeSpec&amp;, bool, clang::Token&amp;) /llvm-project/clang/lib/Parse/ParseExprCXX.cpp:658:17
#<!-- -->57 0x0000563171c30a5a clang::Parser::ParseCXXIdExpression(bool) /llvm-project/clang/lib/Parse/ParseExprCXX.cpp:751:7
#<!-- -->58 0x0000563171c1a234 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) /llvm-project/clang/lib/Parse/ParseExpr.cpp:1720:11
#<!-- -->59 0x0000563171c11b19 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) /llvm-project/clang/lib/Parse/ParseExpr.cpp:712:20
#<!-- -->60 0x0000563171c10011 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) /llvm-project/clang/lib/Parse/ParseExpr.cpp:182:20
#<!-- -->61 0x0000563171c0febf clang::Parser::ParseExpression(clang::Parser::TypeCastState) /llvm-project/clang/lib/Parse/ParseExpr.cpp:133:18
#<!-- -->62 0x0000563171cabfa8 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) /llvm-project/clang/lib/Parse/ParseStmt.cpp:551:19
#<!-- -->63 0x0000563171caa35a clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) /llvm-project/clang/lib/Parse/ParseStmt.cpp:289:14
#<!-- -->64 0x0000563171ca99cb clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) /llvm-project/clang/lib/Parse/ParseStmt.cpp:121:20
#<!-- -->65 0x0000563171cb2c42 clang::Parser::ParseCompoundStatementBody(bool) /llvm-project/clang/lib/Parse/ParseStmt.cpp:1245:11
#<!-- -->66 0x0000563171cb4357 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) /llvm-project/clang/lib/Parse/ParseStmt.cpp:2523:21
#<!-- -->67 0x0000563171bd8580 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) /llvm-project/clang/lib/Parse/Parser.cpp:1525:3
#<!-- -->68 0x0000563171c814f9 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) /llvm-project/clang/lib/Parse/ParseDecl.cpp:2418:21
#<!-- -->69 0x0000563171bd733c clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) /llvm-project/clang/lib/Parse/Parser.cpp:1249:10
#<!-- -->70 0x0000563171bd681e clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) /llvm-project/clang/lib/Parse/Parser.cpp:1271:12
#<!-- -->71 0x0000563171bd60a9 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) /llvm-project/clang/lib/Parse/Parser.cpp:1074:14
#<!-- -->72 0x0000563171bd3ecc clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) /llvm-project/clang/lib/Parse/Parser.cpp:763:12
#<!-- -->73 0x0000563171bcebed clang::ParseAST(clang::Sema&amp;, bool, bool) /llvm-project/clang/lib/Parse/ParseAST.cpp:163:16
#<!-- -->74 0x000056316e6960f1 clang::ASTFrontendAction::ExecuteAction() /llvm-project/clang/lib/Frontend/FrontendAction.cpp:1194:1
#<!-- -->75 0x000056316e695b0c clang::FrontendAction::Execute() /llvm-project/clang/lib/Frontend/FrontendAction.cpp:1082:7
#<!-- -->76 0x000056316e5b1e19 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) /llvm-project/clang/lib/Frontend/CompilerInstance.cpp:1061:23
#<!-- -->77 0x000056316e87215e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) /llvm-project/clang/lib/FrontendTool/ExecuteCompilerInvocation.cpp:280:8
#<!-- -->78 0x000056316b274a81 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) /llvm-project/clang/tools/driver/cc1_main.cpp:276:13
#<!-- -->79 0x000056316b2673f2 ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) /llvm-project/clang/tools/driver/driver.cpp:215:5
#<!-- -->80 0x000056316b26615b clang_main(int, char**, llvm::ToolContext const&amp;) /llvm-project/clang/tools/driver/driver.cpp:256:5
#<!-- -->81 0x000056316b29ba05 main /cmake-build-debug/tools/clang/tools/driver/clang-driver.cpp:17:3
#<!-- -->82 0x00007fa1db4fed90 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:58:16
#<!-- -->83 0x00007fa1db4fee40 call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->84 0x00007fa1db4fee40 __libc_start_main ./csu/../csu/libc-start.c:379:5
#<!-- -->85 0x000056316b265be5 _start (/cmake-build-debug/bin/clang+++0x5b6abe5)
```

But when run without assertions it seems to ignore the `if constexpr` and instantiate `g&lt;N+1&gt;` forever: https://godbolt.org/z/hhhhb8fhj

```
&lt;source&gt;:4:5: warning: stack nearly exhausted; compilation time may suffer, and crashes due to stack overflow are likely [-Wstack-exhausted]
    4 |     g&lt;N+1&gt;(i);
      |     ^
&lt;source&gt;:4:5: note: in instantiation of variable template specialization 'g' requested here
&lt;source&gt;:2:21: note: while substituting into a lambda expression here
    2 | auto g = [](auto i) {
      |                     ^
&lt;source&gt;:4:5: note: in instantiation of variable template specialization 'g' requested here
    4 |     g&lt;N+1&gt;(i);
      |     ^
&lt;source&gt;:2:21: note: while substituting into a lambda expression here
    2 | auto g = [](auto i) {
      |                     ^
&lt;source&gt;:4:5: note: in instantiation of variable template specialization 'g' requested here
    4 |     g&lt;N+1&gt;(i);
      |     ^
&lt;source&gt;:2:21: note: (skipping 1741 contexts in backtrace; use -ftemplate-backtrace-limit=0 to see all)
    2 | auto g = [](auto i) {
      |                     ^
&lt;source&gt;:4:5: note: in instantiation of variable template specialization 'g' requested here
    4 |     g&lt;N+1&gt;(i);
      |     ^
&lt;source&gt;:2:21: note: while substituting into a lambda expression here
    2 | auto g = [](auto i) {
      |                     ^
&lt;source&gt;:4:5: note: in instantiation of variable template specialization 'g' requested here
    4 |     g&lt;N+1&gt;(i);
      |     ^
&lt;source&gt;:2:21: note: while substituting into a lambda expression here
    2 | auto g = [](auto i) {
      |                     ^
&lt;source&gt;:9:5: note: in instantiation of variable template specialization 'g' requested here
    9 |     g&lt;0&gt;(0);
      |     ^
```

---

This one has no assertion error but still compiles wrong: https://godbolt.org/z/n8P4reaob

```c++
template&lt;int X&gt;
void f() {
  static_assert(X != sizeof(int));
}

template&lt;&gt;
void f&lt;sizeof(int)&gt;() = delete;

template&lt;int N&gt;
auto g = [](auto i) {
  if constexpr (N != sizeof(i)) {
    f&lt;N&gt;();
  }
};

int main() {
  g&lt;0&gt;(0);
  g&lt;sizeof(int)&gt;(0);
}
```

```
&lt;source&gt;:12:5: error: call to deleted function 'f'
   12 |     f&lt;N&gt;();
      |     ^~~~
&lt;source&gt;:10:21: note: while substituting into a lambda expression here
   10 | auto g = [](auto i) {
      |                     ^
&lt;source&gt;:18:3: note: in instantiation of variable template specialization 'g' requested here
   18 |   g&lt;sizeof(int)&gt;(0);
      |   ^
&lt;source&gt;:7:6: note: candidate function [with X = 4] has been implicitly deleted
    7 | void f&lt;sizeof(int)&gt;() = delete;
      |      ^
```

So seems like it's a problem when the `if constexpr` is dependent both on the variable template and the generic lambda
</details>


---

### Comment 2 - LYP951018

This bug is likely related to P0588R1 (tracking via #61426, delayed instantiation of `if constexpr`s within generic lambda) which I’m implementing.

But the first case is not a generic lambda

---

### Comment 3 - MitalAshok

The crash and the `if constexpr` bug might be unrelated, I think the crash can be reduced to this: https://godbolt.org/z/xhx6s94vd

```c++
template<bool B>
auto g = sizeof(g<!B>);

int main() {
  // Invalid: instantiating g<false> instantiates g<true> which uses g<false> before the auto type is deduced
  (void)sizeof(g<false>);
}
```

So to do with placeholder types and variable templates

---

### Comment 4 - shafik

Like related to: https://github.com/llvm/llvm-project/issues/48183

---

### Comment 5 - zwuis

> The crash and the `if constexpr` bug might be unrelated, I think the crash can be reduced to this: https://godbolt.org/z/xhx6s94vd
> 
> ```c++
> template<bool B>
> auto g = sizeof(g<!B>);
> 
> int main() {
>   // Invalid: instantiating g<false> instantiates g<true> which uses g<false> before the auto type is deduced
>   (void)sizeof(g<false>);
> }
> ```
> 
> So to do with placeholder types and variable templates

This was fixed by #134522.

---

