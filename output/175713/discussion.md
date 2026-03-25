# [clang]Clang assertions crash in ItaniumMangle when lambda parameter type contains decltype of invalid expression

**Author:** Attacker23
**URL:** https://github.com/llvm/llvm-project/issues/175713

## Body

Clang (assertions build) crashes with `UNREACHABLE executed at ItaniumMangle.cpp` when a lambda parameter type contains `decltype(...)` of a semantically invalid expression. 

## Reproducer 1 (crashes since Clang 21.1.0) https://godbolt.org/z/aK4Td8zzM

```cpp
struct A {
  void f(this A&) {
    auto l = [](auto g, decltype(g(this))){};
  }
};
```

Error: `invalid use of 'this' in a function with an explicit object parameter`, suggesting a possible regression.

## Reproducer 2 (crashes since Clang 11.0.0) https://godbolt.org/z/1d6oGseac

```cpp
struct A {
  void f() {
    auto l = [](int g, decltype(g(this))){};
  }
};
```

Error: `called object type 'int' is not a function or function pointer`


## Stack trace (from Reproducer 1)

```
<source>:3:36: error: invalid use of 'this' in a function with an explicit object parameter
    3 |     auto l = [](auto g, decltype(g(this))){};
      |                                    ^
unexpected statement kind
UNREACHABLE executed at /root/llvm-project/llvm/tools/clang/lib/AST/ItaniumMangle.cpp:4948!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++23 <source>
1.	<source>:3:43: current parser token '{'
2.	<source>:1:1: parsing struct/union/class body 'A'
3.	<source>:2:19: parsing function body 'A::f'
4.	<source>:2:19: in compound statement ('{}')
5.	<source>:3:14: lambda expression parsing
 #0 0x000000000430abe8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x430abe8)
 #1 0x0000000004308014 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4308014)
 #2 0x000000000424aa08 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000075d81b642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x000075d81b6969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x000075d81b642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x000075d81b6287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x000000000425557a (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x425557a)
 #8 0x0000000008022b8c (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
 #9 0x000000000801a35e (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) ItaniumMangle.cpp:0:0
#10 0x00000000080277fe (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) ItaniumMangle.cpp:0:0
#11 0x000000000803340e (anonymous namespace)::ItaniumMangleContextImpl::mangleLambdaSig(clang::CXXRecordDecl const*, llvm::raw_ostream&) ItaniumMangle.cpp:0:0
#12 0x000000000800ba49 (anonymous namespace)::ItaniumNumberingContext::getManglingNumber(clang::CXXMethodDecl const*) ItaniumCXXABI.cpp:0:0
#13 0x000000000712e545 clang::Sema::handleLambdaNumbering(clang::CXXRecordDecl*, clang::CXXMethodDecl*, std::optional<clang::CXXRecordDecl::LambdaNumbering>) (.part.0) SemaLambda.cpp:0:0
#14 0x00000000071340df clang::Sema::ActOnStartOfLambdaDefinition(clang::LambdaIntroducer&, clang::Declarator&, clang::DeclSpec const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x71340df)
#15 0x0000000006aeec48 clang::Parser::ParseLambdaExpressionAfterIntroducer(clang::LambdaIntroducer&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6aeec48)
#16 0x0000000006af004b clang::Parser::ParseLambdaExpression() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6af004b)
#17 0x0000000006ad0625 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ad0625)
#18 0x0000000006ad1047 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ad1047)
#19 0x0000000006ad10d9 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ad10d9)
#20 0x0000000006af95c0 clang::Parser::ParseInitializer(clang::Decl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6af95c0)
#21 0x0000000006a8aabf clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a8aabf)
#22 0x0000000006a97e5a clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a97e5a)
#23 0x0000000006aa0b6f clang::Parser::ParseSimpleDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, bool, clang::Parser::ForRangeInit*, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6aa0b6f)
#24 0x0000000006aa103c clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6aa103c)
#25 0x0000000006b52584 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b52584)
#26 0x0000000006b52e16 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b52e16)
#27 0x0000000006b5b75e clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b5b75e)
#28 0x0000000006b5bfaa clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b5bfaa)
#29 0x0000000006a70eb0 clang::Parser::ParseLexedMethodDef(clang::Parser::LexedMethod&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a70eb0)
#30 0x0000000006a70b9d clang::Parser::ParseLexedMethodDefs(clang::Parser::ParsingClass&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a70b9d)
#31 0x0000000006ac4b36 clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&, unsigned int, clang::Decl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ac4b36)
#32 0x0000000006ac71b0 clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ac71b0)
#33 0x0000000006a9a48e clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a9a48e)
#34 0x0000000006a552d7 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a552d7)
#35 0x0000000006a55d4f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a55d4f)
#36 0x0000000006a5db7a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a5db7a)
#37 0x0000000006a5eb15 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a5eb15)
#38 0x0000000006a5efc0 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a5efc0)
#39 0x0000000006a3fb63 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a3fb63)
#40 0x0000000004c6daf8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c6daf8)
#41 0x0000000004f66ee5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f66ee5)
#42 0x0000000004ee6d1e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ee6d1e)
#43 0x000000000506158d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x506158d)
#44 0x0000000000de8a9e cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde8a9e)
#45 0x0000000000ddf46a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#46 0x0000000000ddf5ed int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#47 0x0000000004ce0b39 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#48 0x000000000424aea4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x424aea4)
#49 0x0000000004ce114f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#50 0x0000000004ca1ae2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ca1ae2)
#51 0x0000000004ca2a8e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ca2a8e)
#52 0x0000000004ca9ee5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ca9ee5)
#53 0x0000000000de4e11 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde4e11)
#54 0x0000000000c8f104 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc8f104)
#55 0x000075d81b629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#56 0x000075d81b629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#57 0x0000000000ddef05 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xddef05)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (Attacker23)

<details>
Clang (assertions build) crashes with `UNREACHABLE executed at ItaniumMangle.cpp` when a lambda parameter type contains `decltype(...)` of a semantically invalid expression. 

## Reproducer 1 (crashes since Clang 21.1.0) https://godbolt.org/z/aK4Td8zzM

```cpp
struct A {
  void f(this A&amp;) {
    auto l = [](auto g, decltype(g(this))){};
  }
};
```

Error: `invalid use of 'this' in a function with an explicit object parameter`, suggesting a possible regression.

## Reproducer 2 (crashes since Clang 11.0.0) https://godbolt.org/z/1d6oGseac

```cpp
struct A {
  void f() {
    auto l = [](int g, decltype(g(this))){};
  }
};
```

Error: `called object type 'int' is not a function or function pointer`


## Stack trace (from Reproducer 1)

```
&lt;source&gt;:3:36: error: invalid use of 'this' in a function with an explicit object parameter
    3 |     auto l = [](auto g, decltype(g(this))){};
      |                                    ^
unexpected statement kind
UNREACHABLE executed at /root/llvm-project/llvm/tools/clang/lib/AST/ItaniumMangle.cpp:4948!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++23 &lt;source&gt;
1.	&lt;source&gt;:3:43: current parser token '{'
2.	&lt;source&gt;:1:1: parsing struct/union/class body 'A'
3.	&lt;source&gt;:2:19: parsing function body 'A::f'
4.	&lt;source&gt;:2:19: in compound statement ('{}')
5.	&lt;source&gt;:3:14: lambda expression parsing
 #<!-- -->0 0x000000000430abe8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x430abe8)
 #<!-- -->1 0x0000000004308014 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4308014)
 #<!-- -->2 0x000000000424aa08 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x000075d81b642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x000075d81b6969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x000075d81b642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x000075d81b6287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x000000000425557a (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x425557a)
 #<!-- -->8 0x0000000008022b8c (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
 #<!-- -->9 0x000000000801a35e (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) ItaniumMangle.cpp:0:0
#<!-- -->10 0x00000000080277fe (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) ItaniumMangle.cpp:0:0
#<!-- -->11 0x000000000803340e (anonymous namespace)::ItaniumMangleContextImpl::mangleLambdaSig(clang::CXXRecordDecl const*, llvm::raw_ostream&amp;) ItaniumMangle.cpp:0:0
#<!-- -->12 0x000000000800ba49 (anonymous namespace)::ItaniumNumberingContext::getManglingNumber(clang::CXXMethodDecl const*) ItaniumCXXABI.cpp:0:0
#<!-- -->13 0x000000000712e545 clang::Sema::handleLambdaNumbering(clang::CXXRecordDecl*, clang::CXXMethodDecl*, std::optional&lt;clang::CXXRecordDecl::LambdaNumbering&gt;) (.part.0) SemaLambda.cpp:0:0
#<!-- -->14 0x00000000071340df clang::Sema::ActOnStartOfLambdaDefinition(clang::LambdaIntroducer&amp;, clang::Declarator&amp;, clang::DeclSpec const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x71340df)
#<!-- -->15 0x0000000006aeec48 clang::Parser::ParseLambdaExpressionAfterIntroducer(clang::LambdaIntroducer&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6aeec48)
#<!-- -->16 0x0000000006af004b clang::Parser::ParseLambdaExpression() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6af004b)
#<!-- -->17 0x0000000006ad0625 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&amp;, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ad0625)
#<!-- -->18 0x0000000006ad1047 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ad1047)
#<!-- -->19 0x0000000006ad10d9 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ad10d9)
#<!-- -->20 0x0000000006af95c0 clang::Parser::ParseInitializer(clang::Decl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6af95c0)
#<!-- -->21 0x0000000006a8aabf clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a8aabf)
#<!-- -->22 0x0000000006a97e5a clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a97e5a)
#<!-- -->23 0x0000000006aa0b6f clang::Parser::ParseSimpleDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, bool, clang::Parser::ForRangeInit*, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6aa0b6f)
#<!-- -->24 0x0000000006aa103c clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6aa103c)
#<!-- -->25 0x0000000006b52584 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b52584)
#<!-- -->26 0x0000000006b52e16 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b52e16)
#<!-- -->27 0x0000000006b5b75e clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b5b75e)
#<!-- -->28 0x0000000006b5bfaa clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b5bfaa)
#<!-- -->29 0x0000000006a70eb0 clang::Parser::ParseLexedMethodDef(clang::Parser::LexedMethod&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a70eb0)
#<!-- -->30 0x0000000006a70b9d clang::Parser::ParseLexedMethodDefs(clang::Parser::ParsingClass&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a70b9d)
#<!-- -->31 0x0000000006ac4b36 clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&amp;, unsigned int, clang::Decl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ac4b36)
#<!-- -->32 0x0000000006ac71b0 clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ac71b0)
#<!-- -->33 0x0000000006a9a48e clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a9a48e)
#<!-- -->34 0x0000000006a552d7 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a552d7)
#<!-- -->35 0x0000000006a55d4f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a55d4f)
#<!-- -->36 0x0000000006a5db7a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a5db7a)
#<!-- -->37 0x0000000006a5eb15 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a5eb15)
#<!-- -->38 0x0000000006a5efc0 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a5efc0)
#<!-- -->39 0x0000000006a3fb63 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a3fb63)
#<!-- -->40 0x0000000004c6daf8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c6daf8)
#<!-- -->41 0x0000000004f66ee5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f66ee5)
#<!-- -->42 0x0000000004ee6d1e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ee6d1e)
#<!-- -->43 0x000000000506158d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x506158d)
#<!-- -->44 0x0000000000de8a9e cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde8a9e)
#<!-- -->45 0x0000000000ddf46a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->46 0x0000000000ddf5ed int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->47 0x0000000004ce0b39 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->48 0x000000000424aea4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x424aea4)
#<!-- -->49 0x0000000004ce114f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->50 0x0000000004ca1ae2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ca1ae2)
#<!-- -->51 0x0000000004ca2a8e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ca2a8e)
#<!-- -->52 0x0000000004ca9ee5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ca9ee5)
#<!-- -->53 0x0000000000de4e11 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde4e11)
#<!-- -->54 0x0000000000c8f104 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc8f104)
#<!-- -->55 0x000075d81b629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->56 0x000075d81b629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->57 0x0000000000ddef05 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xddef05)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

</details>


---

### Comment 2 - shafik

I think these are varieties of this: https://github.com/llvm/llvm-project/issues/150733 but there may be multiple ways of getting there.

CC @cor3ntin 

---

