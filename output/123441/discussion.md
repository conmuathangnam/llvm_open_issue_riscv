# [Clang][regression] Clang crashes when type of function parameter pack of lambda is `decltype(outer_function_parameter_pack)` and lambda has requires clause

**Author:** zwuis
**URL:** https://github.com/llvm/llvm-project/issues/123441
**Status:** Closed
**Labels:** clang:frontend, concepts, confirmed, crash, lambda, regression:16
**Closed Date:** 2025-01-21T06:16:18Z

## Body

This code makes Clang 16 and newer versions crash:

```cpp
void test() {
  auto L = [](auto... x) {
    return [](decltype(x)... y) requires true {};
  };
  L(0, 1)(1, 2);
}
```

Compiler Explorer: <https://godbolt.org/z/831x11eEr>

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Yanzuo Liu (zwuis)

<details>
This code makes Clang 16 and newer versions crash:

```cpp
void test() {
  auto L = [](auto... x) {
    return [](decltype(x)... y) requires true {};
  };
  L(0, 1)(1, 2);
}
```

Compiler Explorer: &lt;https://godbolt.org/z/831x11eEr&gt;
</details>


---

### Comment 2 - zyn0217

SetupConstraintScope is not handling nested lambdas very well. I think we might want to delegate the job to LambdaScopeForCallOperatorInstantiationRAII which handles both captures and parameters dedicatedly.

---

### Comment 3 - shafik

@zyn0217 please try and label regressions when you see them, crashes are bad and I try to track relatively recent-ish regressions to a commit.

---

### Comment 4 - zyn0217

@shafik I believe this is a regression introduced since the 'deferral concept evaluation' patch. So at least you don't have to bisect commits for this one :)

---

### Comment 5 - shafik

Assertion:

```console
clang++: /root/llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:4646:
llvm::PointerUnion<clang::Decl*, llvm::SmallVector<clang::VarDecl*, 4>*>* clang::LocalInstantiationScope::findInstantiationOf(const clang::Decl*):
Assertion `isa<LabelDecl>(D) && "declaration not instantiated in this scope"' failed.
```

Backtrace:

```console
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 <source>
1.	<source>:5:15: current parser token ')'
2.	<source>:1:13: parsing function body 'test'
3.	<source>:1:13: in compound statement ('{}')
 #0 0x0000000003cb1898 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3cb1898)
 #1 0x0000000003caf5a4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3caf5a4)
 #2 0x0000000003bfb728 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007f0ede042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007f0ede0969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007f0ede042476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007f0ede0287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007f0ede02871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007f0ede039e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000007166da5 clang::LocalInstantiationScope::findInstantiationOf(clang::Decl const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7166da5)
#10 0x000000000725c1b7 clang::Sema::getNumArgumentsInExpansionFromUnexpanded(llvm::ArrayRef<std::pair<llvm::PointerUnion<clang::TemplateTypeParmType const*, clang::NamedDecl*>, clang::SourceLocation>>, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x725c1b7)
#11 0x000000000725c484 clang::Sema::getNumArgumentsInExpansion(clang::QualType, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x725c484)
#12 0x00000000071f8e2c clang::Sema::addInstantiatedParametersToScope(clang::FunctionDecl*, clang::FunctionDecl const*, clang::LocalInstantiationScope&, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x71f8e2c)
#13 0x000000000694ca14 clang::Sema::SetupConstraintScope(clang::FunctionDecl*, std::optional<llvm::ArrayRef<clang::TemplateArgument>>, clang::MultiLevelTemplateArgumentList const&, clang::LocalInstantiationScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x694ca14)
#14 0x000000000694cce5 clang::Sema::SetupConstraintCheckingTemplateArgumentsAndScope(clang::FunctionDecl*, std::optional<llvm::ArrayRef<clang::TemplateArgument>>, clang::LocalInstantiationScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x694cce5)
#15 0x000000000695d457 clang::Sema::CheckFunctionConstraints(clang::FunctionDecl const*, clang::ConstraintSatisfaction&, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x695d457)
#16 0x0000000006f553d1 clang::Sema::AddMethodCandidate(clang::CXXMethodDecl*, clang::DeclAccessPair, clang::CXXRecordDecl*, clang::QualType, clang::Expr::Classification, llvm::ArrayRef<clang::Expr*>, clang::OverloadCandidateSet&, bool, bool, llvm::MutableArrayRef<clang::ImplicitConversionSequence>, clang::OverloadCandidateParamOrder) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f553d1)
#17 0x0000000006f62cac clang::Sema::AddMethodCandidate(clang::DeclAccessPair, clang::QualType, clang::Expr::Classification, llvm::ArrayRef<clang::Expr*>, clang::OverloadCandidateSet&, bool, clang::OverloadCandidateParamOrder) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f62cac)
#18 0x0000000006f73152 clang::Sema::BuildCallToObjectOfClassType(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f73152)
#19 0x0000000006ba81f2 clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ba81f2)
#20 0x0000000006ba8b0c clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ba8b0c)
#21 0x00000000066fa9fd clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult<clang::Expr*, true>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66fa9fd)
#22 0x00000000066f33c1 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66f33c1)
#23 0x00000000066f5527 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66f5527)
#24 0x00000000066f55b9 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66f55b9)
#25 0x00000000066f9ff9 clang::Parser::ParseExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66f9ff9)
#26 0x000000000677e7a9 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x677e7a9)
#27 0x0000000006774b1e clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6774b1e)
#28 0x0000000006775a70 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6775a70)
#29 0x0000000006776935 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6776935)
#30 0x000000000677813a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x677813a)
#31 0x0000000006689993 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6689993)
#32 0x00000000066be5dd clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66be5dd)
#33 0x000000000667d6fe clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x667d6fe)
#34 0x000000000667deb9 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x667deb9)
#35 0x0000000006685663 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6685663)
#36 0x000000000668653d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x668653d)
#37 0x00000000066869e0 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66869e0)
#38 0x0000000006678963 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6678963)
#39 0x000000000463ef38 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x463ef38)
#40 0x0000000004905675 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4905675)
#41 0x0000000004887fee clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4887fee)
#42 0x00000000049f2cce clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49f2cce)
#43 0x0000000000cf498f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xcf498f)
#44 0x0000000000cec45a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#45 0x00000000046858a9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#46 0x0000000003bfbbd4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3bfbbd4)
#47 0x0000000004685e9f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#48 0x0000000004648c6d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4648c6d)
#49 0x0000000004649cee clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4649cee)
#50 0x00000000046517f5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x46517f5)
#51 0x0000000000cf17c3 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xcf17c3)
#52 0x0000000000bbcb44 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xbbcb44)
#53 0x00007f0ede029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#54 0x00007f0ede029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#55 0x0000000000cebf05 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xcebf05)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

---

### Comment 6 - zwuis

I find another case without nested lambda making same versions of Clang crash.

```cpp
auto fn(auto... x) {
  return [](decltype(x)... y) requires true {};
}

void test() { fn(0, 1)(1, 2); }
```

Compiler Explorer: <https://godbolt.org/z/b59xveav5>

---

