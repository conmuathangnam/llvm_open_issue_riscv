# Crash in llvm::cast<ConstantArrayType> when using compound literal with designated initializer. 

**Author:** mariete1223
**URL:** https://github.com/llvm/llvm-project/issues/144866

## Body

It is curious because in clang 17 it fails, it seems that it was solved for versions 18-20 and reappeared in version 21(trunk).

## Assertion

```
clang: /root/llvm-project/llvm/include/llvm/Support/Casting.h:578: decltype(auto) llvm::cast(From*) [with To = clang::ConstantArrayType; From = const clang::ArrayType]: Assertion `isa<To>(Val) && "cast<Ty>() argument of incompatible type!"' failed.
```

## Stack dump

```
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -O0 -std=c2x <source>
1.	<source>:8:14: current parser token ')'
2.	<source>:6:12: parsing function body 'main'
3.	<source>:6:12: in compound statement ('{}')
 #0 0x0000000003f6df68 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3f6df68)
 #1 0x0000000003f6bbf4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3f6bbf4)
 #2 0x0000000003eb0708 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x0000753cc5842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x0000753cc58969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x0000753cc5842476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x0000753cc58287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x0000753cc582871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x0000753cc5839e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000006c8bf6f decltype(auto) llvm::cast<clang::ConstantArrayType, clang::ArrayType const>(clang::ArrayType const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6c8bf6f)
#10 0x0000000006cb3abb (anonymous namespace)::InitListChecker::CheckDesignatedInitializer(clang::InitializedEntity const&, clang::InitListExpr*, clang::DesignatedInitExpr*, unsigned int, clang::QualType&, clang::DeclContext::specific_decl_iterator<clang::FieldDecl>*, llvm::APSInt*, unsigned int&, clang::InitListExpr*, unsigned int&, bool, bool) SemaInit.cpp:0:0
#11 0x0000000006cb1eaa (anonymous namespace)::InitListChecker::CheckArrayType(clang::InitializedEntity const&, clang::InitListExpr*, clang::QualType&, llvm::APSInt, bool, unsigned int&, clang::InitListExpr*, unsigned int&) SemaInit.cpp:0:0
#12 0x0000000006caf909 (anonymous namespace)::InitListChecker::CheckListElementTypes(clang::InitializedEntity const&, clang::InitListExpr*, clang::QualType&, bool, unsigned int&, clang::InitListExpr*, unsigned int&, bool) (.constprop.0) SemaInit.cpp:0:0
#13 0x0000000006c9e95a (anonymous namespace)::InitListChecker::CheckExplicitInitList(clang::InitializedEntity const&, clang::InitListExpr*, clang::QualType&, clang::InitListExpr*, bool) (.constprop.0) SemaInit.cpp:0:0
#14 0x0000000006c9f367 (anonymous namespace)::InitListChecker::InitListChecker(clang::Sema&, clang::InitializedEntity const&, clang::InitListExpr*, clang::QualType&, bool, bool, bool, llvm::SmallVectorImpl<clang::QualType>*) SemaInit.cpp:0:0
#15 0x0000000006ca8dfb clang::InitializationSequence::Perform(clang::Sema&, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::MutableArrayRef<clang::Expr*>, clang::QualType*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6ca8dfb)
#16 0x0000000006b115a2 clang::Sema::BuildCompoundLiteralExpr(clang::SourceLocation, clang::TypeSourceInfo*, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6b115a2)
#17 0x0000000006b11ee8 clang::Sema::ActOnCompoundLiteral(clang::SourceLocation, clang::OpaquePtr<clang::QualType>, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6b11ee8)
#18 0x0000000006691ca2 clang::Parser::ParseCompoundLiteralExpression(clang::OpaquePtr<clang::QualType>, clang::SourceLocation, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6691ca2)
#19 0x0000000006699e0d clang::Parser::ParseParenExpression(clang::ParenParseOption&, bool, bool, clang::OpaquePtr<clang::QualType>&, clang::SourceLocation&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6699e0d)
#20 0x000000000668fa6c clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x668fa6c)
#21 0x0000000006690d47 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6690d47)
#22 0x0000000006690dd9 clang::Parser::ParseAssignmentExpression(clang::TypeCastState) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6690dd9)
#23 0x000000000669113e clang::Parser::ParseExpressionList(llvm::SmallVectorImpl<clang::Expr*>&, llvm::function_ref<void ()>, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x669113e)
#24 0x0000000006695f4d clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult<clang::Expr*, true>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6695f4d)
#25 0x000000000668ebaa clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x668ebaa)
#26 0x0000000006690d47 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6690d47)
#27 0x0000000006690dd9 clang::Parser::ParseAssignmentExpression(clang::TypeCastState) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6690dd9)
#28 0x0000000006695499 clang::Parser::ParseExpression(clang::TypeCastState) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6695499)
#29 0x00000000067294f9 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x67294f9)
#30 0x0000000006720ea8 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6720ea8)
#31 0x0000000006721ccd clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6721ccd)
#32 0x0000000006729bf3 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6729bf3)
#33 0x000000000672a3ba clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x672a3ba)
#34 0x0000000006622853 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6622853)
#35 0x00000000066584bd clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x66584bd)
#36 0x000000000661635e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x661635e)
#37 0x0000000006616b09 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6616b09)
#38 0x000000000661e42a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x661e42a)
#39 0x000000000661f3fd clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x661f3fd)
#40 0x000000000661f8b0 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x661f8b0)
#41 0x00000000066117b3 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x66117b3)
#42 0x0000000004922d68 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4922d68)
#43 0x0000000004c1ce55 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4c1ce55)
#44 0x0000000004b9a5de clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4b9a5de)
#45 0x0000000004d0f6e1 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4d0f6e1)
#46 0x0000000000d8cf2f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xd8cf2f)
#47 0x0000000000d83b5a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#48 0x0000000004989c89 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#49 0x0000000003eb0ba4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3eb0ba4)
#50 0x000000000498a29f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#51 0x000000000494be5d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x494be5d)
#52 0x000000000494ceee clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x494ceee)
#53 0x0000000004954fb5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4954fb5)
#54 0x0000000000d8943f clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xd8943f)
#55 0x0000000000c3fc84 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xc3fc84)
#56 0x0000753cc5829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#57 0x0000753cc5829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#58 0x0000000000d83605 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xd83605)
```

## Program

```
#define TIME (const char[]){ \
    [0] = __FILE__, \
    [1] = \
        "\nCompiled ", \
  }
int main() {

  printf(TIME);
}
```

To quickly reproduce:

https://godbolt.org/z/e468G4WG5

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (mariete1223)

<details>
It is curious because in clang 17 it fails, it seems that it was solved for versions 18-20 and reappeared in version 21(trunk).

## Assertion

```
clang: /root/llvm-project/llvm/include/llvm/Support/Casting.h:578: decltype(auto) llvm::cast(From*) [with To = clang::ConstantArrayType; From = const clang::ArrayType]: Assertion `isa&lt;To&gt;(Val) &amp;&amp; "cast&lt;Ty&gt;() argument of incompatible type!"' failed.
```

## Stack dump

```
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -O0 -std=c2x &lt;source&gt;
1.	&lt;source&gt;:8:14: current parser token ')'
2.	&lt;source&gt;:6:12: parsing function body 'main'
3.	&lt;source&gt;:6:12: in compound statement ('{}')
 #<!-- -->0 0x0000000003f6df68 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3f6df68)
 #<!-- -->1 0x0000000003f6bbf4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3f6bbf4)
 #<!-- -->2 0x0000000003eb0708 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x0000753cc5842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x0000753cc58969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x0000753cc5842476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x0000753cc58287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x0000753cc582871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x0000753cc5839e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000006c8bf6f decltype(auto) llvm::cast&lt;clang::ConstantArrayType, clang::ArrayType const&gt;(clang::ArrayType const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6c8bf6f)
#<!-- -->10 0x0000000006cb3abb (anonymous namespace)::InitListChecker::CheckDesignatedInitializer(clang::InitializedEntity const&amp;, clang::InitListExpr*, clang::DesignatedInitExpr*, unsigned int, clang::QualType&amp;, clang::DeclContext::specific_decl_iterator&lt;clang::FieldDecl&gt;*, llvm::APSInt*, unsigned int&amp;, clang::InitListExpr*, unsigned int&amp;, bool, bool) SemaInit.cpp:0:0
#<!-- -->11 0x0000000006cb1eaa (anonymous namespace)::InitListChecker::CheckArrayType(clang::InitializedEntity const&amp;, clang::InitListExpr*, clang::QualType&amp;, llvm::APSInt, bool, unsigned int&amp;, clang::InitListExpr*, unsigned int&amp;) SemaInit.cpp:0:0
#<!-- -->12 0x0000000006caf909 (anonymous namespace)::InitListChecker::CheckListElementTypes(clang::InitializedEntity const&amp;, clang::InitListExpr*, clang::QualType&amp;, bool, unsigned int&amp;, clang::InitListExpr*, unsigned int&amp;, bool) (.constprop.0) SemaInit.cpp:0:0
#<!-- -->13 0x0000000006c9e95a (anonymous namespace)::InitListChecker::CheckExplicitInitList(clang::InitializedEntity const&amp;, clang::InitListExpr*, clang::QualType&amp;, clang::InitListExpr*, bool) (.constprop.0) SemaInit.cpp:0:0
#<!-- -->14 0x0000000006c9f367 (anonymous namespace)::InitListChecker::InitListChecker(clang::Sema&amp;, clang::InitializedEntity const&amp;, clang::InitListExpr*, clang::QualType&amp;, bool, bool, bool, llvm::SmallVectorImpl&lt;clang::QualType&gt;*) SemaInit.cpp:0:0
#<!-- -->15 0x0000000006ca8dfb clang::InitializationSequence::Perform(clang::Sema&amp;, clang::InitializedEntity const&amp;, clang::InitializationKind const&amp;, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::QualType*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6ca8dfb)
#<!-- -->16 0x0000000006b115a2 clang::Sema::BuildCompoundLiteralExpr(clang::SourceLocation, clang::TypeSourceInfo*, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6b115a2)
#<!-- -->17 0x0000000006b11ee8 clang::Sema::ActOnCompoundLiteral(clang::SourceLocation, clang::OpaquePtr&lt;clang::QualType&gt;, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6b11ee8)
#<!-- -->18 0x0000000006691ca2 clang::Parser::ParseCompoundLiteralExpression(clang::OpaquePtr&lt;clang::QualType&gt;, clang::SourceLocation, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6691ca2)
#<!-- -->19 0x0000000006699e0d clang::Parser::ParseParenExpression(clang::ParenParseOption&amp;, bool, bool, clang::OpaquePtr&lt;clang::QualType&gt;&amp;, clang::SourceLocation&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6699e0d)
#<!-- -->20 0x000000000668fa6c clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&amp;, clang::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x668fa6c)
#<!-- -->21 0x0000000006690d47 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6690d47)
#<!-- -->22 0x0000000006690dd9 clang::Parser::ParseAssignmentExpression(clang::TypeCastState) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6690dd9)
#<!-- -->23 0x000000000669113e clang::Parser::ParseExpressionList(llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, llvm::function_ref&lt;void ()&gt;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x669113e)
#<!-- -->24 0x0000000006695f4d clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult&lt;clang::Expr*, true&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6695f4d)
#<!-- -->25 0x000000000668ebaa clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&amp;, clang::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x668ebaa)
#<!-- -->26 0x0000000006690d47 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6690d47)
#<!-- -->27 0x0000000006690dd9 clang::Parser::ParseAssignmentExpression(clang::TypeCastState) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6690dd9)
#<!-- -->28 0x0000000006695499 clang::Parser::ParseExpression(clang::TypeCastState) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6695499)
#<!-- -->29 0x00000000067294f9 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x67294f9)
#<!-- -->30 0x0000000006720ea8 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6720ea8)
#<!-- -->31 0x0000000006721ccd clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6721ccd)
#<!-- -->32 0x0000000006729bf3 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6729bf3)
#<!-- -->33 0x000000000672a3ba clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x672a3ba)
#<!-- -->34 0x0000000006622853 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6622853)
#<!-- -->35 0x00000000066584bd clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x66584bd)
#<!-- -->36 0x000000000661635e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x661635e)
#<!-- -->37 0x0000000006616b09 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6616b09)
#<!-- -->38 0x000000000661e42a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x661e42a)
#<!-- -->39 0x000000000661f3fd clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x661f3fd)
#<!-- -->40 0x000000000661f8b0 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x661f8b0)
#<!-- -->41 0x00000000066117b3 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x66117b3)
#<!-- -->42 0x0000000004922d68 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4922d68)
#<!-- -->43 0x0000000004c1ce55 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4c1ce55)
#<!-- -->44 0x0000000004b9a5de clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4b9a5de)
#<!-- -->45 0x0000000004d0f6e1 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4d0f6e1)
#<!-- -->46 0x0000000000d8cf2f cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xd8cf2f)
#<!-- -->47 0x0000000000d83b5a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->48 0x0000000004989c89 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->49 0x0000000003eb0ba4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3eb0ba4)
#<!-- -->50 0x000000000498a29f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->51 0x000000000494be5d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x494be5d)
#<!-- -->52 0x000000000494ceee clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x494ceee)
#<!-- -->53 0x0000000004954fb5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4954fb5)
#<!-- -->54 0x0000000000d8943f clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xd8943f)
#<!-- -->55 0x0000000000c3fc84 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xc3fc84)
#<!-- -->56 0x0000753cc5829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->57 0x0000753cc5829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->58 0x0000000000d83605 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xd83605)
```

## Program

```
#define TIME (const char[]){ \
    [0] = __FILE__, \
    [1] = \
        "\nCompiled ", \
  }
int main() {

  printf(TIME);
}
```

To quickly reproduce:

https://godbolt.org/z/e468G4WG5
</details>


---

### Comment 2 - shafik

This consistently occurs from clang-14 in assertions mode: https://godbolt.org/z/YxYa8bdG9

With crashes for C it usually makes sense to use C++ on godbolt w/ `-x c` since we have assertions builds for all versions for C++. 

---

### Comment 3 - yronglin

I have a more simple reproducer [CompilerExplorer](https://godbolt.org/z/EjnWGb1bP). This problem should not be related to macros:
```cpp
const char *x = (const char[]){
  [0] = __FILE__,
  [1] = "\nCompiled ",
};

```

Below changes can fix the assertion crash, but I'm not sure it's a correct approach.
```diff
diff --git a/clang/lib/Sema/SemaInit.cpp b/clang/lib/Sema/SemaInit.cpp
index da56225b2f92..7bb31c0ebe0d 100644
--- a/clang/lib/Sema/SemaInit.cpp
+++ b/clang/lib/Sema/SemaInit.cpp
@@ -3287,8 +3287,8 @@ InitListChecker::CheckDesignatedInitializer(const InitializedEntity &Entity,
     if (StringLiteral *SL = dyn_cast<StringLiteral>(SubExpr)) {
       // Get the length of the string.
       uint64_t StrLen = SL->getLength();
-      if (cast<ConstantArrayType>(AT)->getSize().ult(StrLen))
-        StrLen = cast<ConstantArrayType>(AT)->getZExtSize();
+      if (const auto *CAT = dyn_cast<ConstantArrayType>(AT); CAT && CAT->getSize().ult(StrLen))
+        StrLen = CAT->getZExtSize();
       StructuredList->resizeInits(Context, StrLen);
 
       // Build a literal for each character in the string, and put them into
```

---

### Comment 4 - AaronBallman

> I have a more simple reproducer [CompilerExplorer](https://godbolt.org/z/EjnWGb1bP). This problem should not be related to macros:
> 
> const char *x = (const char[]){
>   [0] = __FILE__,
>   [1] = "\nCompiled ",
> };
> 
> Below changes can fix the assertion crash, but I'm not sure it's a correct approach.
> 
> diff --git a/clang/lib/Sema/SemaInit.cpp b/clang/lib/Sema/SemaInit.cpp
> index da56225b2f92..7bb31c0ebe0d 100644
> --- a/clang/lib/Sema/SemaInit.cpp
> +++ b/clang/lib/Sema/SemaInit.cpp
> @@ -3287,8 +3287,8 @@ InitListChecker::CheckDesignatedInitializer(const InitializedEntity &Entity,
>      if (StringLiteral *SL = dyn_cast<StringLiteral>(SubExpr)) {
>        // Get the length of the string.
>        uint64_t StrLen = SL->getLength();
> -      if (cast<ConstantArrayType>(AT)->getSize().ult(StrLen))
> -        StrLen = cast<ConstantArrayType>(AT)->getZExtSize();
> +      if (const auto *CAT = dyn_cast<ConstantArrayType>(AT); CAT && CAT->getSize().ult(StrLen))
> +        StrLen = CAT->getZExtSize();
>        StructuredList->resizeInits(Context, StrLen);
>  
>        // Build a literal for each character in the string, and put them into

This is for error recovery purposes; we can either try to recover so we can issue more diagnostics about the initializer or we can try to bail early to suppress follow-on diagnostics. I kind of think bailing early might make more sense, otherwise you're going to get the same diagnostic repeatedly when the issue has more to do with the type in the compound literal than the members of the initializer list. But this doesn't seem entirely unreasonable, either.

---

