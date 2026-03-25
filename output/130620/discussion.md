# [Clang][Regression:19] Crash on pack expansion of requires expression with lambda

**Author:** MagentaTreehouse
**URL:** https://github.com/llvm/llvm-project/issues/130620
**Status:** Closed
**Labels:** clang:frontend, concepts, crash, regression:19
**Closed Date:** 2026-01-20T15:51:38Z

## Body

The following C++20 code causes a crash since Clang 19:
```c++
template <class T>
constexpr auto f{[] (auto... x) {
    (... && requires {
        x;
        [] (auto) {}(T{x});
    });
}};

int main() {
    f<int>(0);
}
```

See https://compiler-explorer.com/z/bvTvPeM77.

Assertion:
```console
clang++: /root/llvm-project/clang/include/clang/AST/ExprConcepts.h:416:
clang::Expr* clang::concepts::ExprRequirement::getExpr() const:
Assertion `!isExprSubstitutionFailure() && "ExprRequirement has no expression because there has been a " "substitution failure."' failed.
```

<details>

<summary>Stack dump</summary>

```console
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 <source>
1.	<source>:10:11: current parser token '('
2.	<source>:9:12: parsing function body 'main'
3.	<source>:9:12: in compound statement ('{}')
 #0 0x0000000003e6c598 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3e6c598)
 #1 0x0000000003e6a254 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3e6a254)
 #2 0x0000000003db6b68 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007ab49ec42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007ab49ec969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007ab49ec42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007ab49ec287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007ab49ec2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007ab49ec39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000007670545 clang::RequiresExpr::RequiresExpr(clang::ASTContext&, clang::SourceLocation, clang::RequiresExprBodyDecl*, clang::SourceLocation, llvm::ArrayRef<clang::ParmVarDecl*>, clang::SourceLocation, llvm::ArrayRef<clang::concepts::Requirement*>, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7670545)
#10 0x0000000007670670 clang::RequiresExpr::Create(clang::ASTContext&, clang::SourceLocation, clang::RequiresExprBodyDecl*, clang::SourceLocation, llvm::ArrayRef<clang::ParmVarDecl*>, clang::SourceLocation, llvm::ArrayRef<clang::concepts::Requirement*>, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7670670)
#11 0x0000000006fd0ad7 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformRequiresExpr(clang::RequiresExpr*) SemaTemplateInstantiate.cpp:0:0
#12 0x0000000006fc3650 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#13 0x0000000006fd5d9c clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCXXFoldExpr(clang::CXXFoldExpr*) SemaTemplateInstantiate.cpp:0:0
#14 0x0000000006fc30f9 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#15 0x0000000006ff1177 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformStmt(clang::Stmt*, clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::StmtDiscardKind) SemaTemplateInstantiate.cpp:0:0
#16 0x0000000006ff1d8e clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaTemplateInstantiate.cpp:0:0
#17 0x0000000006fc25e4 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformLambdaExpr(clang::LambdaExpr*) SemaTemplateInstantiate.cpp:0:0
#18 0x0000000006fc3936 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#19 0x0000000006fc6073 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExprs(clang::Expr* const*, unsigned int, bool, llvm::SmallVectorImpl<clang::Expr*>&, bool*) SemaTemplateInstantiate.cpp:0:0
#20 0x0000000006fc7efc clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformInitListExpr(clang::InitListExpr*) SemaTemplateInstantiate.cpp:0:0
#21 0x0000000006fc33b6 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#22 0x0000000006fc6465 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformInitializer(clang::Expr*, bool) (.part.0) SemaTemplateInstantiate.cpp:0:0
#23 0x0000000006fc6c9b clang::Sema::SubstInitializer(clang::Expr*, clang::MultiLevelTemplateArgumentList const&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6fc6c9b)
#24 0x00000000070227f6 clang::Sema::InstantiateVariableInitializer(clang::VarDecl*, clang::VarDecl*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x70227f6)
#25 0x0000000007071500 clang::Sema::BuildVariableInstantiation(clang::VarDecl*, clang::VarDecl*, clang::MultiLevelTemplateArgumentList const&, llvm::SmallVector<clang::Sema::LateInstantiatedAttribute, 16u>*, clang::DeclContext*, clang::LocalInstantiationScope*, bool, clang::VarTemplateSpecializationDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7071500)
#26 0x0000000007071e06 clang::TemplateDeclInstantiator::VisitVarTemplateSpecializationDecl(clang::VarTemplateDecl*, clang::VarDecl*, clang::TemplateArgumentListInfo const&, llvm::ArrayRef<clang::TemplateArgument>, clang::VarTemplateSpecializationDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7071e06)
#27 0x000000000707bc57 clang::Sema::BuildVarTemplateInstantiation(clang::VarTemplateDecl*, clang::VarDecl*, clang::TemplateArgumentList const*, clang::TemplateArgumentListInfo const&, llvm::SmallVectorImpl<clang::TemplateArgument>&, clang::SourceLocation, llvm::SmallVector<clang::Sema::LateInstantiatedAttribute, 16u>*, clang::LocalInstantiationScope*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x707bc57)
#28 0x0000000006e42ed6 clang::Sema::CheckVarTemplateId(clang::VarTemplateDecl*, clang::SourceLocation, clang::SourceLocation, clang::TemplateArgumentListInfo const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e42ed6)
#29 0x0000000006e43541 clang::Sema::CheckVarTemplateId(clang::CXXScopeSpec const&, clang::DeclarationNameInfo const&, clang::VarTemplateDecl*, clang::NamedDecl*, clang::SourceLocation, clang::TemplateArgumentListInfo const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e43541)
#30 0x0000000006e46c4b clang::Sema::BuildTemplateIdExpr(clang::CXXScopeSpec const&, clang::SourceLocation, clang::LookupResult&, bool, clang::TemplateArgumentListInfo const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e46c4b)
#31 0x000000000693f7e8 clang::Sema::ActOnIdExpression(clang::Scope*, clang::CXXScopeSpec&, clang::SourceLocation, clang::UnqualifiedId&, bool, bool, clang::CorrectionCandidateCallback*, bool, clang::Token*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x693f7e8)
#32 0x00000000064c0c06 clang::Parser::tryParseCXXIdExpression(clang::CXXScopeSpec&, bool, clang::Token&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64c0c06)
#33 0x00000000064c0f2a clang::Parser::ParseCXXIdExpression(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64c0f2a)
#34 0x00000000064a43da clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64a43da)
#35 0x00000000064a5a97 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64a5a97)
#36 0x00000000064a5b29 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64a5b29)
#37 0x00000000064aa569 clang::Parser::ParseExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64aa569)
#38 0x000000000652e2b9 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x652e2b9)
#39 0x000000000652461e clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x652461e)
#40 0x0000000006525570 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6525570)
#41 0x0000000006526461 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6526461)
#42 0x0000000006527c4a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6527c4a)
#43 0x0000000006439343 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6439343)
#44 0x000000000646e0fd clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x646e0fd)
#45 0x000000000642d07e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x642d07e)
#46 0x000000000642d839 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x642d839)
#47 0x0000000006435013 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6435013)
#48 0x0000000006435eed clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6435eed)
#49 0x000000000642837a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x642837a)
#50 0x00000000048300a8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48300a8)
#51 0x0000000004af89e5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4af89e5)
#52 0x0000000004a7c1be clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4a7c1be)
#53 0x0000000004be76ae clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4be76ae)
#54 0x0000000000d52de7 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd52de7)
#55 0x0000000000d49baa ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#56 0x00000000048784a9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#57 0x0000000003db7014 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3db7014)
#58 0x0000000004878a9f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#59 0x000000000483b27d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x483b27d)
#60 0x000000000483c2fe clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x483c2fe)
#61 0x0000000004844215 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4844215)
#62 0x0000000000d4f083 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd4f083)
#63 0x0000000000c16c64 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc16c64)
#64 0x00007ab49ec29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#65 0x00007ab49ec29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#66 0x0000000000d49655 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd49655)
```

</details>


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (MagentaTreehouse)

<details>
The following C++20 code causes a crash since Clang 19:
```c++
template &lt;class T&gt;
constexpr auto f{[] (auto... x) {
    (... &amp;&amp; requires {
        x;
        [] (auto) {}(T{x});
    });
}};

int main() {
    f&lt;int&gt;(0);
}
```

See https://compiler-explorer.com/z/bvTvPeM77.

Assertion:
```console
clang++: /root/llvm-project/clang/include/clang/AST/ExprConcepts.h:416:
clang::Expr* clang::concepts::ExprRequirement::getExpr() const:
Assertion `!isExprSubstitutionFailure() &amp;&amp; "ExprRequirement has no expression because there has been a " "substitution failure."' failed.
```

&lt;details&gt;

&lt;summary&gt;Stack dump&lt;/summary&gt;

```console
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 &lt;source&gt;
1.	&lt;source&gt;:10:11: current parser token '('
2.	&lt;source&gt;:9:12: parsing function body 'main'
3.	&lt;source&gt;:9:12: in compound statement ('{}')
 #<!-- -->0 0x0000000003e6c598 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3e6c598)
 #<!-- -->1 0x0000000003e6a254 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3e6a254)
 #<!-- -->2 0x0000000003db6b68 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007ab49ec42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007ab49ec969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007ab49ec42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007ab49ec287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x00007ab49ec2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x00007ab49ec39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000007670545 clang::RequiresExpr::RequiresExpr(clang::ASTContext&amp;, clang::SourceLocation, clang::RequiresExprBodyDecl*, clang::SourceLocation, llvm::ArrayRef&lt;clang::ParmVarDecl*&gt;, clang::SourceLocation, llvm::ArrayRef&lt;clang::concepts::Requirement*&gt;, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7670545)
#<!-- -->10 0x0000000007670670 clang::RequiresExpr::Create(clang::ASTContext&amp;, clang::SourceLocation, clang::RequiresExprBodyDecl*, clang::SourceLocation, llvm::ArrayRef&lt;clang::ParmVarDecl*&gt;, clang::SourceLocation, llvm::ArrayRef&lt;clang::concepts::Requirement*&gt;, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7670670)
#<!-- -->11 0x0000000006fd0ad7 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformRequiresExpr(clang::RequiresExpr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->12 0x0000000006fc3650 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->13 0x0000000006fd5d9c clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCXXFoldExpr(clang::CXXFoldExpr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->14 0x0000000006fc30f9 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->15 0x0000000006ff1177 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformStmt(clang::Stmt*, clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::StmtDiscardKind) SemaTemplateInstantiate.cpp:0:0
#<!-- -->16 0x0000000006ff1d8e clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaTemplateInstantiate.cpp:0:0
#<!-- -->17 0x0000000006fc25e4 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformLambdaExpr(clang::LambdaExpr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->18 0x0000000006fc3936 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->19 0x0000000006fc6073 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExprs(clang::Expr* const*, unsigned int, bool, llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, bool*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->20 0x0000000006fc7efc clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformInitListExpr(clang::InitListExpr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->21 0x0000000006fc33b6 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->22 0x0000000006fc6465 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformInitializer(clang::Expr*, bool) (.part.0) SemaTemplateInstantiate.cpp:0:0
#<!-- -->23 0x0000000006fc6c9b clang::Sema::SubstInitializer(clang::Expr*, clang::MultiLevelTemplateArgumentList const&amp;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6fc6c9b)
#<!-- -->24 0x00000000070227f6 clang::Sema::InstantiateVariableInitializer(clang::VarDecl*, clang::VarDecl*, clang::MultiLevelTemplateArgumentList const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x70227f6)
#<!-- -->25 0x0000000007071500 clang::Sema::BuildVariableInstantiation(clang::VarDecl*, clang::VarDecl*, clang::MultiLevelTemplateArgumentList const&amp;, llvm::SmallVector&lt;clang::Sema::LateInstantiatedAttribute, 16u&gt;*, clang::DeclContext*, clang::LocalInstantiationScope*, bool, clang::VarTemplateSpecializationDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7071500)
#<!-- -->26 0x0000000007071e06 clang::TemplateDeclInstantiator::VisitVarTemplateSpecializationDecl(clang::VarTemplateDecl*, clang::VarDecl*, clang::TemplateArgumentListInfo const&amp;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::VarTemplateSpecializationDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7071e06)
#<!-- -->27 0x000000000707bc57 clang::Sema::BuildVarTemplateInstantiation(clang::VarTemplateDecl*, clang::VarDecl*, clang::TemplateArgumentList const*, clang::TemplateArgumentListInfo const&amp;, llvm::SmallVectorImpl&lt;clang::TemplateArgument&gt;&amp;, clang::SourceLocation, llvm::SmallVector&lt;clang::Sema::LateInstantiatedAttribute, 16u&gt;*, clang::LocalInstantiationScope*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x707bc57)
#<!-- -->28 0x0000000006e42ed6 clang::Sema::CheckVarTemplateId(clang::VarTemplateDecl*, clang::SourceLocation, clang::SourceLocation, clang::TemplateArgumentListInfo const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e42ed6)
#<!-- -->29 0x0000000006e43541 clang::Sema::CheckVarTemplateId(clang::CXXScopeSpec const&amp;, clang::DeclarationNameInfo const&amp;, clang::VarTemplateDecl*, clang::NamedDecl*, clang::SourceLocation, clang::TemplateArgumentListInfo const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e43541)
#<!-- -->30 0x0000000006e46c4b clang::Sema::BuildTemplateIdExpr(clang::CXXScopeSpec const&amp;, clang::SourceLocation, clang::LookupResult&amp;, bool, clang::TemplateArgumentListInfo const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e46c4b)
#<!-- -->31 0x000000000693f7e8 clang::Sema::ActOnIdExpression(clang::Scope*, clang::CXXScopeSpec&amp;, clang::SourceLocation, clang::UnqualifiedId&amp;, bool, bool, clang::CorrectionCandidateCallback*, bool, clang::Token*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x693f7e8)
#<!-- -->32 0x00000000064c0c06 clang::Parser::tryParseCXXIdExpression(clang::CXXScopeSpec&amp;, bool, clang::Token&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64c0c06)
#<!-- -->33 0x00000000064c0f2a clang::Parser::ParseCXXIdExpression(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64c0f2a)
#<!-- -->34 0x00000000064a43da clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64a43da)
#<!-- -->35 0x00000000064a5a97 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64a5a97)
#<!-- -->36 0x00000000064a5b29 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64a5b29)
#<!-- -->37 0x00000000064aa569 clang::Parser::ParseExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64aa569)
#<!-- -->38 0x000000000652e2b9 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x652e2b9)
#<!-- -->39 0x000000000652461e clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x652461e)
#<!-- -->40 0x0000000006525570 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6525570)
#<!-- -->41 0x0000000006526461 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6526461)
#<!-- -->42 0x0000000006527c4a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6527c4a)
#<!-- -->43 0x0000000006439343 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6439343)
#<!-- -->44 0x000000000646e0fd clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x646e0fd)
#<!-- -->45 0x000000000642d07e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x642d07e)
#<!-- -->46 0x000000000642d839 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x642d839)
#<!-- -->47 0x0000000006435013 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6435013)
#<!-- -->48 0x0000000006435eed clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6435eed)
#<!-- -->49 0x000000000642837a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x642837a)
#<!-- -->50 0x00000000048300a8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48300a8)
#<!-- -->51 0x0000000004af89e5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4af89e5)
#<!-- -->52 0x0000000004a7c1be clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4a7c1be)
#<!-- -->53 0x0000000004be76ae clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4be76ae)
#<!-- -->54 0x0000000000d52de7 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd52de7)
#<!-- -->55 0x0000000000d49baa ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->56 0x00000000048784a9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->57 0x0000000003db7014 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3db7014)
#<!-- -->58 0x0000000004878a9f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->59 0x000000000483b27d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x483b27d)
#<!-- -->60 0x000000000483c2fe clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x483c2fe)
#<!-- -->61 0x0000000004844215 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4844215)
#<!-- -->62 0x0000000000d4f083 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd4f083)
#<!-- -->63 0x0000000000c16c64 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc16c64)
#<!-- -->64 0x00007ab49ec29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->65 0x00007ab49ec29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->66 0x0000000000d49655 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd49655)
```

&lt;/details&gt;

</details>


---

### Comment 2 - shafik

CC @cor3ntin @erichkeane 

---

### Comment 3 - shafik

@zyn0217 git bisect points to 9fe5aa31eccff49632e40bcdad6e64fc00190889 as the cause of this regression

---

### Comment 4 - shafik

This no longer crashes in trunk: https://compiler-explorer.com/z/7beP1odvx

CC @zyn0217 

---

