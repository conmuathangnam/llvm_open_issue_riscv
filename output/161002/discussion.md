# [Clang] Crash on pack expansion with lambda capture

**Author:** MagentaTreehouse
**URL:** https://github.com/llvm/llvm-project/issues/161002

## Body

```c++
template <class... Ts>
auto f{[] (auto... a0) {
    return [=] (Ts... a1) {
        (..., [a0Cap{a0}, a1] { return a0Cap + a1; });
    };
}};

auto g{f<int>};
```

See https://compiler-explorer.com/z/7Wd17nWz7.

Assertion:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:6856:
clang::NamedDecl* clang::Sema::FindInstantiatedDecl(clang::SourceLocation, clang::NamedDecl*, const clang::MultiLevelTemplateArgumentList&, bool):
Assertion `ArgPackSubstIndex && "found declaration pack but not pack expanding"' failed.
```

<details>

<summary>Stack dump</summary>

```console
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++17 <source>
1.	<source>:8:14: current parser token '}'
 #0 0x00000000041b0348 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41b0348)
 #1 0x00000000041ad774 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41ad774)
 #2 0x00000000040f1aa8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000072c226642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x000072c2266969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x000072c226642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x000072c2266287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x000072c22662871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x000072c226639e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000007425f5d clang::Sema::FindInstantiatedDecl(clang::SourceLocation, clang::NamedDecl*, clang::MultiLevelTemplateArgumentList const&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7425f5d)
#10 0x0000000007398df1 (anonymous namespace)::TemplateInstantiator::TransformDecl(clang::SourceLocation, clang::Decl*) SemaTemplateInstantiate.cpp:0:0
#11 0x00000000073c65d3 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformLambdaExpr(clang::LambdaExpr*) SemaTemplateInstantiate.cpp:0:0
#12 0x000000000737e8dd clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#13 0x00000000073a3814 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCXXFoldExpr(clang::CXXFoldExpr*) SemaTemplateInstantiate.cpp:0:0
#14 0x000000000737e349 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#15 0x00000000073bb5e7 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformStmt(clang::Stmt*, clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::StmtDiscardKind) SemaTemplateInstantiate.cpp:0:0
#16 0x00000000073bbc5e clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaTemplateInstantiate.cpp:0:0
#17 0x00000000073c6fcb clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformLambdaExpr(clang::LambdaExpr*) SemaTemplateInstantiate.cpp:0:0
#18 0x000000000737e8dd clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#19 0x000000000739ea95 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformInitializer(clang::Expr*, bool) (.part.0) SemaTemplateInstantiate.cpp:0:0
#20 0x000000000739ee1c clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformReturnStmt(clang::ReturnStmt*) SemaTemplateInstantiate.cpp:0:0
#21 0x00000000073bbc5e clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaTemplateInstantiate.cpp:0:0
#22 0x00000000073c6fcb clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformLambdaExpr(clang::LambdaExpr*) SemaTemplateInstantiate.cpp:0:0
#23 0x000000000737e8dd clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#24 0x000000000739e64b clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExprs(clang::Expr* const*, unsigned int, bool, llvm::SmallVectorImpl<clang::Expr*>&, bool*) SemaTemplateInstantiate.cpp:0:0
#25 0x00000000073a098c clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformInitListExpr(clang::InitListExpr*) SemaTemplateInstantiate.cpp:0:0
#26 0x000000000737e592 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#27 0x000000000739ea95 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformInitializer(clang::Expr*, bool) (.part.0) SemaTemplateInstantiate.cpp:0:0
#28 0x000000000739f57b clang::Sema::SubstInitializer(clang::Expr*, clang::MultiLevelTemplateArgumentList const&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x739f57b)
#29 0x00000000073d884d clang::Sema::InstantiateVariableInitializer(clang::VarDecl*, clang::VarDecl*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x73d884d)
#30 0x0000000007429250 clang::Sema::BuildVariableInstantiation(clang::VarDecl*, clang::VarDecl*, clang::MultiLevelTemplateArgumentList const&, llvm::SmallVector<clang::Sema::LateInstantiatedAttribute, 1u>*, clang::DeclContext*, clang::LocalInstantiationScope*, bool, clang::VarTemplateSpecializationDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7429250)
#31 0x0000000007429ba7 clang::TemplateDeclInstantiator::VisitVarTemplateSpecializationDecl(clang::VarTemplateDecl*, clang::VarDecl*, llvm::ArrayRef<clang::TemplateArgument>, clang::VarTemplateSpecializationDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7429ba7)
#32 0x0000000007434d7a clang::Sema::BuildVarTemplateInstantiation(clang::VarTemplateDecl*, clang::VarDecl*, clang::TemplateArgumentList const*, llvm::SmallVectorImpl<clang::TemplateArgument>&, clang::SourceLocation, llvm::SmallVector<clang::Sema::LateInstantiatedAttribute, 1u>*, clang::LocalInstantiationScope*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7434d7a)
#33 0x00000000072611fe clang::Sema::CheckVarTemplateId(clang::VarTemplateDecl*, clang::SourceLocation, clang::SourceLocation, clang::TemplateArgumentListInfo const&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x72611fe)
#34 0x0000000007261a94 clang::Sema::CheckVarTemplateId(clang::CXXScopeSpec const&, clang::DeclarationNameInfo const&, clang::VarTemplateDecl*, clang::NamedDecl*, clang::SourceLocation, clang::TemplateArgumentListInfo const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7261a94)
#35 0x0000000007261cf7 clang::Sema::BuildTemplateIdExpr(clang::CXXScopeSpec const&, clang::SourceLocation, clang::LookupResult&, bool, clang::TemplateArgumentListInfo const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7261cf7)
#36 0x0000000006dafd98 clang::Sema::ActOnIdExpression(clang::Scope*, clang::CXXScopeSpec&, clang::SourceLocation, clang::UnqualifiedId&, bool, bool, clang::CorrectionCandidateCallback*, bool, clang::Token*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6dafd98)
#37 0x0000000006925106 clang::Parser::tryParseCXXIdExpression(clang::CXXScopeSpec&, bool, clang::Token&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6925106)
#38 0x00000000069253d0 clang::Parser::ParseCXXIdExpression(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69253d0)
#39 0x00000000069090ba clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69090ba)
#40 0x000000000690884e clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x690884e)
#41 0x000000000690a837 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x690a837)
#42 0x000000000690a8c9 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x690a8c9)
#43 0x00000000069327d4 clang::Parser::ParseBraceInitializer() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69327d4)
#44 0x00000000068c31d0 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68c31d0)
#45 0x00000000068d0ba9 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68d0ba9)
#46 0x000000000688e4ee clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x688e4ee)
#47 0x000000000688ec8f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x688ec8f)
#48 0x0000000006896543 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6896543)
#49 0x0000000006897455 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6897455)
#50 0x000000000687840a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x687840a)
#51 0x0000000004aecd58 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4aecd58)
#52 0x0000000004dd8b65 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4dd8b65)
#53 0x0000000004d549fe clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d549fe)
#54 0x0000000004ecf73d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ecf73d)
#55 0x0000000000db8cf0 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb8cf0)
#56 0x0000000000daf85a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#57 0x0000000000daf9dd int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#58 0x0000000004b55e99 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#59 0x00000000040f1f44 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x40f1f44)
#60 0x0000000004b564af clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#61 0x0000000004b17812 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b17812)
#62 0x0000000004b187be clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b187be)
#63 0x0000000004b1fef5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b1fef5)
#64 0x0000000000db5219 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb5219)
#65 0x0000000000c66b84 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc66b84)
#66 0x000072c226629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#67 0x000072c226629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#68 0x0000000000daf2f5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdaf2f5)
```

</details>

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (MagentaTreehouse)

<details>
```c++
template &lt;class... Ts&gt;
auto f{[] (auto... a0) {
    return [=] (Ts... a1) {
        (..., [a0Cap{a0}, a1] { return a0Cap + a1; });
    };
}};

auto g{f&lt;int&gt;};
```

See https://compiler-explorer.com/z/7Wd17nWz7.

Assertion:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:6856:
clang::NamedDecl* clang::Sema::FindInstantiatedDecl(clang::SourceLocation, clang::NamedDecl*, const clang::MultiLevelTemplateArgumentList&amp;, bool):
Assertion `ArgPackSubstIndex &amp;&amp; "found declaration pack but not pack expanding"' failed.
```

&lt;details&gt;

&lt;summary&gt;Stack dump&lt;/summary&gt;

```console
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++17 &lt;source&gt;
1.	&lt;source&gt;:8:14: current parser token '}'
 #<!-- -->0 0x00000000041b0348 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41b0348)
 #<!-- -->1 0x00000000041ad774 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41ad774)
 #<!-- -->2 0x00000000040f1aa8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x000072c226642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x000072c2266969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x000072c226642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x000072c2266287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x000072c22662871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x000072c226639e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000007425f5d clang::Sema::FindInstantiatedDecl(clang::SourceLocation, clang::NamedDecl*, clang::MultiLevelTemplateArgumentList const&amp;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7425f5d)
#<!-- -->10 0x0000000007398df1 (anonymous namespace)::TemplateInstantiator::TransformDecl(clang::SourceLocation, clang::Decl*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->11 0x00000000073c65d3 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformLambdaExpr(clang::LambdaExpr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->12 0x000000000737e8dd clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->13 0x00000000073a3814 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCXXFoldExpr(clang::CXXFoldExpr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->14 0x000000000737e349 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->15 0x00000000073bb5e7 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformStmt(clang::Stmt*, clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::StmtDiscardKind) SemaTemplateInstantiate.cpp:0:0
#<!-- -->16 0x00000000073bbc5e clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaTemplateInstantiate.cpp:0:0
#<!-- -->17 0x00000000073c6fcb clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformLambdaExpr(clang::LambdaExpr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->18 0x000000000737e8dd clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->19 0x000000000739ea95 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformInitializer(clang::Expr*, bool) (.part.0) SemaTemplateInstantiate.cpp:0:0
#<!-- -->20 0x000000000739ee1c clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformReturnStmt(clang::ReturnStmt*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->21 0x00000000073bbc5e clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaTemplateInstantiate.cpp:0:0
#<!-- -->22 0x00000000073c6fcb clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformLambdaExpr(clang::LambdaExpr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->23 0x000000000737e8dd clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->24 0x000000000739e64b clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExprs(clang::Expr* const*, unsigned int, bool, llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, bool*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->25 0x00000000073a098c clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformInitListExpr(clang::InitListExpr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->26 0x000000000737e592 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->27 0x000000000739ea95 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformInitializer(clang::Expr*, bool) (.part.0) SemaTemplateInstantiate.cpp:0:0
#<!-- -->28 0x000000000739f57b clang::Sema::SubstInitializer(clang::Expr*, clang::MultiLevelTemplateArgumentList const&amp;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x739f57b)
#<!-- -->29 0x00000000073d884d clang::Sema::InstantiateVariableInitializer(clang::VarDecl*, clang::VarDecl*, clang::MultiLevelTemplateArgumentList const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x73d884d)
#<!-- -->30 0x0000000007429250 clang::Sema::BuildVariableInstantiation(clang::VarDecl*, clang::VarDecl*, clang::MultiLevelTemplateArgumentList const&amp;, llvm::SmallVector&lt;clang::Sema::LateInstantiatedAttribute, 1u&gt;*, clang::DeclContext*, clang::LocalInstantiationScope*, bool, clang::VarTemplateSpecializationDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7429250)
#<!-- -->31 0x0000000007429ba7 clang::TemplateDeclInstantiator::VisitVarTemplateSpecializationDecl(clang::VarTemplateDecl*, clang::VarDecl*, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::VarTemplateSpecializationDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7429ba7)
#<!-- -->32 0x0000000007434d7a clang::Sema::BuildVarTemplateInstantiation(clang::VarTemplateDecl*, clang::VarDecl*, clang::TemplateArgumentList const*, llvm::SmallVectorImpl&lt;clang::TemplateArgument&gt;&amp;, clang::SourceLocation, llvm::SmallVector&lt;clang::Sema::LateInstantiatedAttribute, 1u&gt;*, clang::LocalInstantiationScope*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7434d7a)
#<!-- -->33 0x00000000072611fe clang::Sema::CheckVarTemplateId(clang::VarTemplateDecl*, clang::SourceLocation, clang::SourceLocation, clang::TemplateArgumentListInfo const&amp;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x72611fe)
#<!-- -->34 0x0000000007261a94 clang::Sema::CheckVarTemplateId(clang::CXXScopeSpec const&amp;, clang::DeclarationNameInfo const&amp;, clang::VarTemplateDecl*, clang::NamedDecl*, clang::SourceLocation, clang::TemplateArgumentListInfo const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7261a94)
#<!-- -->35 0x0000000007261cf7 clang::Sema::BuildTemplateIdExpr(clang::CXXScopeSpec const&amp;, clang::SourceLocation, clang::LookupResult&amp;, bool, clang::TemplateArgumentListInfo const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7261cf7)
#<!-- -->36 0x0000000006dafd98 clang::Sema::ActOnIdExpression(clang::Scope*, clang::CXXScopeSpec&amp;, clang::SourceLocation, clang::UnqualifiedId&amp;, bool, bool, clang::CorrectionCandidateCallback*, bool, clang::Token*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6dafd98)
#<!-- -->37 0x0000000006925106 clang::Parser::tryParseCXXIdExpression(clang::CXXScopeSpec&amp;, bool, clang::Token&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6925106)
#<!-- -->38 0x00000000069253d0 clang::Parser::ParseCXXIdExpression(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69253d0)
#<!-- -->39 0x00000000069090ba clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&amp;, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69090ba)
#<!-- -->40 0x000000000690884e clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&amp;, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x690884e)
#<!-- -->41 0x000000000690a837 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x690a837)
#<!-- -->42 0x000000000690a8c9 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x690a8c9)
#<!-- -->43 0x00000000069327d4 clang::Parser::ParseBraceInitializer() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69327d4)
#<!-- -->44 0x00000000068c31d0 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68c31d0)
#<!-- -->45 0x00000000068d0ba9 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68d0ba9)
#<!-- -->46 0x000000000688e4ee clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x688e4ee)
#<!-- -->47 0x000000000688ec8f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x688ec8f)
#<!-- -->48 0x0000000006896543 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6896543)
#<!-- -->49 0x0000000006897455 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6897455)
#<!-- -->50 0x000000000687840a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x687840a)
#<!-- -->51 0x0000000004aecd58 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4aecd58)
#<!-- -->52 0x0000000004dd8b65 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4dd8b65)
#<!-- -->53 0x0000000004d549fe clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d549fe)
#<!-- -->54 0x0000000004ecf73d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ecf73d)
#<!-- -->55 0x0000000000db8cf0 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb8cf0)
#<!-- -->56 0x0000000000daf85a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->57 0x0000000000daf9dd int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->58 0x0000000004b55e99 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->59 0x00000000040f1f44 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x40f1f44)
#<!-- -->60 0x0000000004b564af clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->61 0x0000000004b17812 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b17812)
#<!-- -->62 0x0000000004b187be clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b187be)
#<!-- -->63 0x0000000004b1fef5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b1fef5)
#<!-- -->64 0x0000000000db5219 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb5219)
#<!-- -->65 0x0000000000c66b84 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc66b84)
#<!-- -->66 0x000072c226629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->67 0x000072c226629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->68 0x0000000000daf2f5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdaf2f5)
```

&lt;/details&gt;
</details>


---

### Comment 2 - cor3ntin

Slightly simplified

```
template <class... Ts>
auto f{[] (auto... a0) {
    return [=] (Ts... a1) {
        (..., [a1, a0] { return a1; });
    };
}};

auto g{f<int>};
```

---

### Comment 3 - shafik

Goes back to clang-3.8: https://compiler-explorer.com/z/br461ffEx

---

### Comment 4 - cor3ntin

@zyn0217 

---

### Comment 5 - zyn0217

Related: #18873

---

