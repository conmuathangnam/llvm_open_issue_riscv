# [clangd] crash when parsing the code

**Author:** noirgif
**URL:** https://github.com/llvm/llvm-project/issues/150195

## Body

❯ clangd --version
clangd version 20.1.8 (Fedora 20.1.8-1.fc42)
Features: linux
Platform: aarch64-unknown-linux-gnu; target=aarch64-redhat-linux-gnu 

Constantly crashes at this location.

```
  diff = diff.unaryExpr(
     [this](matrix::Value value) -> matrix::Value {
        if (abs(value) > delta_) {
          if (value > 0) {
              return delta_;
            } else {
              return -delta_;
            }
        }
        return value;
    });
```

```
Stack dump:
0.	xxxxxx:26:22: current parser token ')'  --> the right bracket after `abs(value`
1.	xxxxxx:6:1: parsing namespace 'xxxxxx'
2.	xxxxxx: parsing function body 'xxxxxx'
3.	xxxxxx: in compound statement ('{}')
4.	xxxxxx: lambda expression parsing
5.	xxxxxx: in compound statement ('{}')


#0 0x0000ffffaf905e0c llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/lib64/libLLVM.so.20.1+0x695e0c)
 #1 0x0000ffffaf9036f0 llvm::sys::RunSignalHandlers() (/lib64/libLLVM.so.20.1+0x6936f0)
 #2 0x0000ffffaf90662c (/lib64/libLLVM.so.20.1+0x69662c)
 #3 0x0000ffffbb240840 (linux-vdso.so.1+0x840)
 #4 0x0000ffffb83b9038 (/lib64/libclang-cpp.so.20.1+0x12c9038)
 #5 0x0000ffffb83b9730 (/lib64/libclang-cpp.so.20.1+0x12c9730)
 #6 0x0000ffffb83af460 clang::Sema::CheckAbsoluteValueFunction(clang::CallExpr const*, clang::FunctionDecl const*) (/lib64/libclang-cpp.so.20.1+0x12bf460)
 #7 0x0000ffffb83ae85c clang::Sema::CheckFunctionCall(clang::FunctionDecl*, clang::CallExpr*, clang::FunctionProtoType const*) (/lib64/libclang-cpp.so.20.1+0x12be85c)
 #8 0x0000ffffb85b49f8 clang::Sema::BuildResolvedCallExpr(clang::Expr*, clang::NamedDecl*, clang::SourceLocation, llvm::ArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, clang::CallExpr::ADLCallKind) (/lib64/libclang-cpp.so.20.1+0x14c49f8)
 #9 0x0000ffffb88e7658 (/lib64/libclang-cpp.so.20.1+0x17f7658)
#10 0x0000ffffb88e73c8 clang::Sema::BuildOverloadedCallExpr(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/lib64/libclang-cpp.so.20.1+0x17f73c8)
#11 0x0000ffffb859b638 clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/lib64/libclang-cpp.so.20.1+0x14ab638)
#12 0x0000ffffb85b2670 clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) (/lib64/libclang-cpp.so.20.1+0x14c2670)
#13 0x0000ffffb7773690 clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult<clang::Expr*, true>) (/lib64/libclang-cpp.so.20.1+0x683690)
#14 0x0000ffffb7775c04 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) (/lib64/libclang-cpp.so.20.1+0x685c04)
#15 0x0000ffffb77712a0 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/lib64/libclang-cpp.so.20.1+0x6812a0)
#16 0x0000ffffb778d6f4 clang::Parser::ParseCXXCondition(clang::ActionResult<clang::Stmt*, true>*, clang::SourceLocation, clang::Sema::ConditionKind, bool, clang::Parser::ForRangeInfo*, bool) (/lib64/libclang-cpp.so.20.1+0x69d6f4)
#17 0x0000ffffb77e2644 clang::Parser::ParseParenExprOrCondition(clang::ActionResult<clang::Stmt*, true>*, clang::Sema::ConditionResult&, clang::SourceLocation, clang::Sema::ConditionKind, clang::SourceLocation&, clang::SourceLocation&) (/lib64/libclang-cpp.so.20.1+0x6f2644)
#18 0x0000ffffb77dbef4 clang::Parser::ParseIfStatement(clang::SourceLocation*) (/lib64/libclang-cpp.so.20.1+0x6ebef4)
#19 0x0000ffffb77d9370 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/lib64/libclang-cpp.so.20.1+0x6e9370)
#20 0x0000ffffb77d7de8 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/lib64/libclang-cpp.so.20.1+0x6e7de8)
#21 0x0000ffffb77e1bb4 clang::Parser::ParseCompoundStatementBody(bool) (/lib64/libclang-cpp.so.20.1+0x6f1bb4)
#22 0x0000ffffb778a380 clang::Parser::ParseLambdaExpressionAfterIntroducer(clang::LambdaIntroducer&) (/lib64/libclang-cpp.so.20.1+0x69a380)
#23 0x0000ffffb7787d08 clang::Parser::ParseLambdaExpression() (/lib64/libclang-cpp.so.20.1+0x697d08)
#24 0x0000ffffb7776fa4 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) (/lib64/libclang-cpp.so.20.1+0x686fa4)
#25 0x0000ffffb77712a0 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/lib64/libclang-cpp.so.20.1+0x6812a0)
#26 0x0000ffffb77800a0 clang::Parser::ParseExpressionList(llvm::SmallVectorImpl<clang::Expr*>&, llvm::function_ref<void ()>, bool, bool, bool*) (/lib64/libclang-cpp.so.20.1+0x6900a0)
#27 0x0000ffffb77735f8 clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult<clang::Expr*, true>) (/lib64/libclang-cpp.so.20.1+0x6835f8)
#28 0x0000ffffb7775c04 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) (/lib64/libclang-cpp.so.20.1+0x685c04)
#29 0x0000ffffb77712a0 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/lib64/libclang-cpp.so.20.1+0x6812a0)
#30 0x0000ffffb7771cac clang::Parser::ParseRHSOfBinaryExpression(clang::ActionResult<clang::Expr*, true>, clang::prec::Level) (/lib64/libclang-cpp.so.20.1+0x681cac)
#31 0x0000ffffb77712fc clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/lib64/libclang-cpp.so.20.1+0x6812fc)
#32 0x0000ffffb77da8b8 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/lib64/libclang-cpp.so.20.1+0x6ea8b8)
#33 0x0000ffffb77d8684 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/lib64/libclang-cpp.so.20.1+0x6e8684)
#34 0x0000ffffb77d7de8 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/lib64/libclang-cpp.so.20.1+0x6e7de8)
#35 0x0000ffffb77e1bb4 clang::Parser::ParseCompoundStatementBody(bool) (/lib64/libclang-cpp.so.20.1+0x6f1bb4)
#36 0x0000ffffb77e2fc8 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/lib64/libclang-cpp.so.20.1+0x6f2fc8)
#37 0x0000ffffb77fdd74 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/lib64/libclang-cpp.so.20.1+0x70dd74)
#38 0x0000ffffb773e014 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/lib64/libclang-cpp.so.20.1+0x64e014)
#39 0x0000ffffb77fcb18 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/lib64/libclang-cpp.so.20.1+0x70cb18)
#40 0x0000ffffb77fc3e4 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/lib64/libclang-cpp.so.20.1+0x70c3e4)
#41 0x0000ffffb77fb5a4 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/lib64/libclang-cpp.so.20.1+0x70b5a4)
#42 0x0000ffffb775a84c clang::Parser::ParseInnerNamespace(llvm::SmallVector<clang::Parser::InnerNamespaceInfo, 4u> const&, unsigned int, clang::SourceLocation&, clang::ParsedAttributes&, clang::BalancedDelimiterTracker&) (/lib64/libclang-cpp.so.20.1+0x66a84c)
#43 0x0000ffffb7759d7c clang::Parser::ParseNamespace(clang::DeclaratorContext, clang::SourceLocation&, clang::SourceLocation) (/lib64/libclang-cpp.so.20.1+0x669d7c)
#44 0x0000ffffb773c438 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/lib64/libclang-cpp.so.20.1+0x64c438)
#45 0x0000ffffb77facbc clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/lib64/libclang-cpp.so.20.1+0x70acbc)
#46 0x0000ffffb77f9a90 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/lib64/libclang-cpp.so.20.1+0x709a90)
#47 0x0000ffffb77253bc clang::ParseAST(clang::Sema&, bool, bool) (/lib64/libclang-cpp.so.20.1+0x6353bc)
#48 0x0000ffffb9b9a324 clang::FrontendAction::Execute() (/lib64/libclang-cpp.so.20.1+0x2aaa324)
#49 0x0000aaab3545f240 (/usr/bin/clangd+0x50f240)
#50 0x0000aaab3546400c (/usr/bin/clangd+0x51400c)
#51 0x0000aaab3545c738 (/usr/bin/clangd+0x50c738)
#52 0x0000aaab354c5e98 (/usr/bin/clangd+0x575e98)
#53 0x0000ffffaedad284 start_thread (/lib64/libc.so.6+0x8d284)
#54 0x0000ffffaee1804c thread_start (/lib64/libc.so.6+0xf804c)
[Error - 5:10:03 PM] Server process exited with signal SIGTRAP.
[Error - 5:10:03 PM] The Clang Language Server server crashed 5 times in the last 3 minutes. The server will not be restarted. See the output for more information.
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Ruohui Wang (noirgif)

<details>
❯ clangd --version
clangd version 20.1.8 (Fedora 20.1.8-1.fc42)
Features: linux
Platform: aarch64-unknown-linux-gnu; target=aarch64-redhat-linux-gnu 

Constantly crashes at this location.

```
  diff = diff.unaryExpr(
     [this](matrix::Value value) -&gt; matrix::Value {
        if (abs(value) &gt; delta_) {
          if (value &gt; 0) {
              return delta_;
            } else {
              return -delta_;
            }
        }
        return value;
    });
```

```
Stack dump:
0.	xxxxxx:26:22: current parser token ')'  --&gt; the right bracket after `abs(value`
1.	xxxxxx:6:1: parsing namespace 'xxxxxx'
2.	xxxxxx: parsing function body 'xxxxxx'
3.	xxxxxx: in compound statement ('{}')
4.	xxxxxx: lambda expression parsing
5.	xxxxxx: in compound statement ('{}')


#<!-- -->0 0x0000ffffaf905e0c llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/lib64/libLLVM.so.20.1+0x695e0c)
 #<!-- -->1 0x0000ffffaf9036f0 llvm::sys::RunSignalHandlers() (/lib64/libLLVM.so.20.1+0x6936f0)
 #<!-- -->2 0x0000ffffaf90662c (/lib64/libLLVM.so.20.1+0x69662c)
 #<!-- -->3 0x0000ffffbb240840 (linux-vdso.so.1+0x840)
 #<!-- -->4 0x0000ffffb83b9038 (/lib64/libclang-cpp.so.20.1+0x12c9038)
 #<!-- -->5 0x0000ffffb83b9730 (/lib64/libclang-cpp.so.20.1+0x12c9730)
 #<!-- -->6 0x0000ffffb83af460 clang::Sema::CheckAbsoluteValueFunction(clang::CallExpr const*, clang::FunctionDecl const*) (/lib64/libclang-cpp.so.20.1+0x12bf460)
 #<!-- -->7 0x0000ffffb83ae85c clang::Sema::CheckFunctionCall(clang::FunctionDecl*, clang::CallExpr*, clang::FunctionProtoType const*) (/lib64/libclang-cpp.so.20.1+0x12be85c)
 #<!-- -->8 0x0000ffffb85b49f8 clang::Sema::BuildResolvedCallExpr(clang::Expr*, clang::NamedDecl*, clang::SourceLocation, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, clang::CallExpr::ADLCallKind) (/lib64/libclang-cpp.so.20.1+0x14c49f8)
 #<!-- -->9 0x0000ffffb88e7658 (/lib64/libclang-cpp.so.20.1+0x17f7658)
#<!-- -->10 0x0000ffffb88e73c8 clang::Sema::BuildOverloadedCallExpr(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, bool) (/lib64/libclang-cpp.so.20.1+0x17f73c8)
#<!-- -->11 0x0000ffffb859b638 clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, bool) (/lib64/libclang-cpp.so.20.1+0x14ab638)
#<!-- -->12 0x0000ffffb85b2670 clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*) (/lib64/libclang-cpp.so.20.1+0x14c2670)
#<!-- -->13 0x0000ffffb7773690 clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult&lt;clang::Expr*, true&gt;) (/lib64/libclang-cpp.so.20.1+0x683690)
#<!-- -->14 0x0000ffffb7775c04 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) (/lib64/libclang-cpp.so.20.1+0x685c04)
#<!-- -->15 0x0000ffffb77712a0 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/lib64/libclang-cpp.so.20.1+0x6812a0)
#<!-- -->16 0x0000ffffb778d6f4 clang::Parser::ParseCXXCondition(clang::ActionResult&lt;clang::Stmt*, true&gt;*, clang::SourceLocation, clang::Sema::ConditionKind, bool, clang::Parser::ForRangeInfo*, bool) (/lib64/libclang-cpp.so.20.1+0x69d6f4)
#<!-- -->17 0x0000ffffb77e2644 clang::Parser::ParseParenExprOrCondition(clang::ActionResult&lt;clang::Stmt*, true&gt;*, clang::Sema::ConditionResult&amp;, clang::SourceLocation, clang::Sema::ConditionKind, clang::SourceLocation&amp;, clang::SourceLocation&amp;) (/lib64/libclang-cpp.so.20.1+0x6f2644)
#<!-- -->18 0x0000ffffb77dbef4 clang::Parser::ParseIfStatement(clang::SourceLocation*) (/lib64/libclang-cpp.so.20.1+0x6ebef4)
#<!-- -->19 0x0000ffffb77d9370 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/lib64/libclang-cpp.so.20.1+0x6e9370)
#<!-- -->20 0x0000ffffb77d7de8 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/lib64/libclang-cpp.so.20.1+0x6e7de8)
#<!-- -->21 0x0000ffffb77e1bb4 clang::Parser::ParseCompoundStatementBody(bool) (/lib64/libclang-cpp.so.20.1+0x6f1bb4)
#<!-- -->22 0x0000ffffb778a380 clang::Parser::ParseLambdaExpressionAfterIntroducer(clang::LambdaIntroducer&amp;) (/lib64/libclang-cpp.so.20.1+0x69a380)
#<!-- -->23 0x0000ffffb7787d08 clang::Parser::ParseLambdaExpression() (/lib64/libclang-cpp.so.20.1+0x697d08)
#<!-- -->24 0x0000ffffb7776fa4 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) (/lib64/libclang-cpp.so.20.1+0x686fa4)
#<!-- -->25 0x0000ffffb77712a0 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/lib64/libclang-cpp.so.20.1+0x6812a0)
#<!-- -->26 0x0000ffffb77800a0 clang::Parser::ParseExpressionList(llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, llvm::function_ref&lt;void ()&gt;, bool, bool, bool*) (/lib64/libclang-cpp.so.20.1+0x6900a0)
#<!-- -->27 0x0000ffffb77735f8 clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult&lt;clang::Expr*, true&gt;) (/lib64/libclang-cpp.so.20.1+0x6835f8)
#<!-- -->28 0x0000ffffb7775c04 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) (/lib64/libclang-cpp.so.20.1+0x685c04)
#<!-- -->29 0x0000ffffb77712a0 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/lib64/libclang-cpp.so.20.1+0x6812a0)
#<!-- -->30 0x0000ffffb7771cac clang::Parser::ParseRHSOfBinaryExpression(clang::ActionResult&lt;clang::Expr*, true&gt;, clang::prec::Level) (/lib64/libclang-cpp.so.20.1+0x681cac)
#<!-- -->31 0x0000ffffb77712fc clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/lib64/libclang-cpp.so.20.1+0x6812fc)
#<!-- -->32 0x0000ffffb77da8b8 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/lib64/libclang-cpp.so.20.1+0x6ea8b8)
#<!-- -->33 0x0000ffffb77d8684 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/lib64/libclang-cpp.so.20.1+0x6e8684)
#<!-- -->34 0x0000ffffb77d7de8 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/lib64/libclang-cpp.so.20.1+0x6e7de8)
#<!-- -->35 0x0000ffffb77e1bb4 clang::Parser::ParseCompoundStatementBody(bool) (/lib64/libclang-cpp.so.20.1+0x6f1bb4)
#<!-- -->36 0x0000ffffb77e2fc8 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/lib64/libclang-cpp.so.20.1+0x6f2fc8)
#<!-- -->37 0x0000ffffb77fdd74 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/lib64/libclang-cpp.so.20.1+0x70dd74)
#<!-- -->38 0x0000ffffb773e014 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/lib64/libclang-cpp.so.20.1+0x64e014)
#<!-- -->39 0x0000ffffb77fcb18 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/lib64/libclang-cpp.so.20.1+0x70cb18)
#<!-- -->40 0x0000ffffb77fc3e4 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/lib64/libclang-cpp.so.20.1+0x70c3e4)
#<!-- -->41 0x0000ffffb77fb5a4 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/lib64/libclang-cpp.so.20.1+0x70b5a4)
#<!-- -->42 0x0000ffffb775a84c clang::Parser::ParseInnerNamespace(llvm::SmallVector&lt;clang::Parser::InnerNamespaceInfo, 4u&gt; const&amp;, unsigned int, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::BalancedDelimiterTracker&amp;) (/lib64/libclang-cpp.so.20.1+0x66a84c)
#<!-- -->43 0x0000ffffb7759d7c clang::Parser::ParseNamespace(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::SourceLocation) (/lib64/libclang-cpp.so.20.1+0x669d7c)
#<!-- -->44 0x0000ffffb773c438 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/lib64/libclang-cpp.so.20.1+0x64c438)
#<!-- -->45 0x0000ffffb77facbc clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/lib64/libclang-cpp.so.20.1+0x70acbc)
#<!-- -->46 0x0000ffffb77f9a90 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/lib64/libclang-cpp.so.20.1+0x709a90)
#<!-- -->47 0x0000ffffb77253bc clang::ParseAST(clang::Sema&amp;, bool, bool) (/lib64/libclang-cpp.so.20.1+0x6353bc)
#<!-- -->48 0x0000ffffb9b9a324 clang::FrontendAction::Execute() (/lib64/libclang-cpp.so.20.1+0x2aaa324)
#<!-- -->49 0x0000aaab3545f240 (/usr/bin/clangd+0x50f240)
#<!-- -->50 0x0000aaab3546400c (/usr/bin/clangd+0x51400c)
#<!-- -->51 0x0000aaab3545c738 (/usr/bin/clangd+0x50c738)
#<!-- -->52 0x0000aaab354c5e98 (/usr/bin/clangd+0x575e98)
#<!-- -->53 0x0000ffffaedad284 start_thread (/lib64/libc.so.6+0x8d284)
#<!-- -->54 0x0000ffffaee1804c thread_start (/lib64/libc.so.6+0xf804c)
[Error - 5:10:03 PM] Server process exited with signal SIGTRAP.
[Error - 5:10:03 PM] The Clang Language Server server crashed 5 times in the last 3 minutes. The server will not be restarted. See the output for more information.
```
</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clangd

Author: Ruohui Wang (noirgif)

<details>
❯ clangd --version
clangd version 20.1.8 (Fedora 20.1.8-1.fc42)
Features: linux
Platform: aarch64-unknown-linux-gnu; target=aarch64-redhat-linux-gnu 

Constantly crashes at this location.

```
  diff = diff.unaryExpr(
     [this](matrix::Value value) -&gt; matrix::Value {
        if (abs(value) &gt; delta_) {
          if (value &gt; 0) {
              return delta_;
            } else {
              return -delta_;
            }
        }
        return value;
    });
```

```
Stack dump:
0.	xxxxxx:26:22: current parser token ')'  --&gt; the right bracket after `abs(value`
1.	xxxxxx:6:1: parsing namespace 'xxxxxx'
2.	xxxxxx: parsing function body 'xxxxxx'
3.	xxxxxx: in compound statement ('{}')
4.	xxxxxx: lambda expression parsing
5.	xxxxxx: in compound statement ('{}')


#<!-- -->0 0x0000ffffaf905e0c llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/lib64/libLLVM.so.20.1+0x695e0c)
 #<!-- -->1 0x0000ffffaf9036f0 llvm::sys::RunSignalHandlers() (/lib64/libLLVM.so.20.1+0x6936f0)
 #<!-- -->2 0x0000ffffaf90662c (/lib64/libLLVM.so.20.1+0x69662c)
 #<!-- -->3 0x0000ffffbb240840 (linux-vdso.so.1+0x840)
 #<!-- -->4 0x0000ffffb83b9038 (/lib64/libclang-cpp.so.20.1+0x12c9038)
 #<!-- -->5 0x0000ffffb83b9730 (/lib64/libclang-cpp.so.20.1+0x12c9730)
 #<!-- -->6 0x0000ffffb83af460 clang::Sema::CheckAbsoluteValueFunction(clang::CallExpr const*, clang::FunctionDecl const*) (/lib64/libclang-cpp.so.20.1+0x12bf460)
 #<!-- -->7 0x0000ffffb83ae85c clang::Sema::CheckFunctionCall(clang::FunctionDecl*, clang::CallExpr*, clang::FunctionProtoType const*) (/lib64/libclang-cpp.so.20.1+0x12be85c)
 #<!-- -->8 0x0000ffffb85b49f8 clang::Sema::BuildResolvedCallExpr(clang::Expr*, clang::NamedDecl*, clang::SourceLocation, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, clang::CallExpr::ADLCallKind) (/lib64/libclang-cpp.so.20.1+0x14c49f8)
 #<!-- -->9 0x0000ffffb88e7658 (/lib64/libclang-cpp.so.20.1+0x17f7658)
#<!-- -->10 0x0000ffffb88e73c8 clang::Sema::BuildOverloadedCallExpr(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, bool) (/lib64/libclang-cpp.so.20.1+0x17f73c8)
#<!-- -->11 0x0000ffffb859b638 clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, bool) (/lib64/libclang-cpp.so.20.1+0x14ab638)
#<!-- -->12 0x0000ffffb85b2670 clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*) (/lib64/libclang-cpp.so.20.1+0x14c2670)
#<!-- -->13 0x0000ffffb7773690 clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult&lt;clang::Expr*, true&gt;) (/lib64/libclang-cpp.so.20.1+0x683690)
#<!-- -->14 0x0000ffffb7775c04 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) (/lib64/libclang-cpp.so.20.1+0x685c04)
#<!-- -->15 0x0000ffffb77712a0 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/lib64/libclang-cpp.so.20.1+0x6812a0)
#<!-- -->16 0x0000ffffb778d6f4 clang::Parser::ParseCXXCondition(clang::ActionResult&lt;clang::Stmt*, true&gt;*, clang::SourceLocation, clang::Sema::ConditionKind, bool, clang::Parser::ForRangeInfo*, bool) (/lib64/libclang-cpp.so.20.1+0x69d6f4)
#<!-- -->17 0x0000ffffb77e2644 clang::Parser::ParseParenExprOrCondition(clang::ActionResult&lt;clang::Stmt*, true&gt;*, clang::Sema::ConditionResult&amp;, clang::SourceLocation, clang::Sema::ConditionKind, clang::SourceLocation&amp;, clang::SourceLocation&amp;) (/lib64/libclang-cpp.so.20.1+0x6f2644)
#<!-- -->18 0x0000ffffb77dbef4 clang::Parser::ParseIfStatement(clang::SourceLocation*) (/lib64/libclang-cpp.so.20.1+0x6ebef4)
#<!-- -->19 0x0000ffffb77d9370 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/lib64/libclang-cpp.so.20.1+0x6e9370)
#<!-- -->20 0x0000ffffb77d7de8 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/lib64/libclang-cpp.so.20.1+0x6e7de8)
#<!-- -->21 0x0000ffffb77e1bb4 clang::Parser::ParseCompoundStatementBody(bool) (/lib64/libclang-cpp.so.20.1+0x6f1bb4)
#<!-- -->22 0x0000ffffb778a380 clang::Parser::ParseLambdaExpressionAfterIntroducer(clang::LambdaIntroducer&amp;) (/lib64/libclang-cpp.so.20.1+0x69a380)
#<!-- -->23 0x0000ffffb7787d08 clang::Parser::ParseLambdaExpression() (/lib64/libclang-cpp.so.20.1+0x697d08)
#<!-- -->24 0x0000ffffb7776fa4 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) (/lib64/libclang-cpp.so.20.1+0x686fa4)
#<!-- -->25 0x0000ffffb77712a0 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/lib64/libclang-cpp.so.20.1+0x6812a0)
#<!-- -->26 0x0000ffffb77800a0 clang::Parser::ParseExpressionList(llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, llvm::function_ref&lt;void ()&gt;, bool, bool, bool*) (/lib64/libclang-cpp.so.20.1+0x6900a0)
#<!-- -->27 0x0000ffffb77735f8 clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult&lt;clang::Expr*, true&gt;) (/lib64/libclang-cpp.so.20.1+0x6835f8)
#<!-- -->28 0x0000ffffb7775c04 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) (/lib64/libclang-cpp.so.20.1+0x685c04)
#<!-- -->29 0x0000ffffb77712a0 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/lib64/libclang-cpp.so.20.1+0x6812a0)
#<!-- -->30 0x0000ffffb7771cac clang::Parser::ParseRHSOfBinaryExpression(clang::ActionResult&lt;clang::Expr*, true&gt;, clang::prec::Level) (/lib64/libclang-cpp.so.20.1+0x681cac)
#<!-- -->31 0x0000ffffb77712fc clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/lib64/libclang-cpp.so.20.1+0x6812fc)
#<!-- -->32 0x0000ffffb77da8b8 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/lib64/libclang-cpp.so.20.1+0x6ea8b8)
#<!-- -->33 0x0000ffffb77d8684 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/lib64/libclang-cpp.so.20.1+0x6e8684)
#<!-- -->34 0x0000ffffb77d7de8 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/lib64/libclang-cpp.so.20.1+0x6e7de8)
#<!-- -->35 0x0000ffffb77e1bb4 clang::Parser::ParseCompoundStatementBody(bool) (/lib64/libclang-cpp.so.20.1+0x6f1bb4)
#<!-- -->36 0x0000ffffb77e2fc8 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/lib64/libclang-cpp.so.20.1+0x6f2fc8)
#<!-- -->37 0x0000ffffb77fdd74 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/lib64/libclang-cpp.so.20.1+0x70dd74)
#<!-- -->38 0x0000ffffb773e014 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/lib64/libclang-cpp.so.20.1+0x64e014)
#<!-- -->39 0x0000ffffb77fcb18 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/lib64/libclang-cpp.so.20.1+0x70cb18)
#<!-- -->40 0x0000ffffb77fc3e4 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/lib64/libclang-cpp.so.20.1+0x70c3e4)
#<!-- -->41 0x0000ffffb77fb5a4 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/lib64/libclang-cpp.so.20.1+0x70b5a4)
#<!-- -->42 0x0000ffffb775a84c clang::Parser::ParseInnerNamespace(llvm::SmallVector&lt;clang::Parser::InnerNamespaceInfo, 4u&gt; const&amp;, unsigned int, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::BalancedDelimiterTracker&amp;) (/lib64/libclang-cpp.so.20.1+0x66a84c)
#<!-- -->43 0x0000ffffb7759d7c clang::Parser::ParseNamespace(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::SourceLocation) (/lib64/libclang-cpp.so.20.1+0x669d7c)
#<!-- -->44 0x0000ffffb773c438 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/lib64/libclang-cpp.so.20.1+0x64c438)
#<!-- -->45 0x0000ffffb77facbc clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/lib64/libclang-cpp.so.20.1+0x70acbc)
#<!-- -->46 0x0000ffffb77f9a90 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/lib64/libclang-cpp.so.20.1+0x709a90)
#<!-- -->47 0x0000ffffb77253bc clang::ParseAST(clang::Sema&amp;, bool, bool) (/lib64/libclang-cpp.so.20.1+0x6353bc)
#<!-- -->48 0x0000ffffb9b9a324 clang::FrontendAction::Execute() (/lib64/libclang-cpp.so.20.1+0x2aaa324)
#<!-- -->49 0x0000aaab3545f240 (/usr/bin/clangd+0x50f240)
#<!-- -->50 0x0000aaab3546400c (/usr/bin/clangd+0x51400c)
#<!-- -->51 0x0000aaab3545c738 (/usr/bin/clangd+0x50c738)
#<!-- -->52 0x0000aaab354c5e98 (/usr/bin/clangd+0x575e98)
#<!-- -->53 0x0000ffffaedad284 start_thread (/lib64/libc.so.6+0x8d284)
#<!-- -->54 0x0000ffffaee1804c thread_start (/lib64/libc.so.6+0xf804c)
[Error - 5:10:03 PM] Server process exited with signal SIGTRAP.
[Error - 5:10:03 PM] The Clang Language Server server crashed 5 times in the last 3 minutes. The server will not be restarted. See the output for more information.
```
</details>


---

### Comment 3 - HighCommander4

It's hard to be sure without a complete code example (entire file + any included header files), but I suspect this is a duplicate of #111304 or #96627, or at least a related issue.

---

