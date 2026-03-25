# [clang] Assertion `(S.getLangOpts().CPlusPlus11 || isCCEAllowedPreCXX11) && "converted constant expression outside C++11 or TTP matching"' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/154917

## Body

Reproducer:
https://godbolt.org/z/dYhcrndcE
```cpp
struct A { int a : 16; };

template<typename T> T &make();

template <typename T>
void foo() {
  A& a = make<A>();
  auto &[...an] = a;
  an...[0] = 42;
}

void bar = foo<int>();
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/Sema/SemaOverload.cpp:6280: clang::ExprResult BuildConvertedConstantExpression(clang::Sema&, clang::Expr*, clang::QualType, clang::CCEKind, clang::NamedDecl*, clang::APValue&): Assertion `(S.getLangOpts().CPlusPlus11 || isCCEAllowedPreCXX11) && "converted constant expression outside C++11 or TTP matching"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++03 <source>
1.	<source>:9:12: current parser token '='
2.	<source>:6:12: parsing function body 'foo'
3.	<source>:6:12: in compound statement ('{}')
 #0 0x000000000403b2a8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x403b2a8)
 #1 0x00000000040386d4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x40386d4)
 #2 0x0000000003f7cde8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007deb87042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007deb870969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007deb87042476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007deb870287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007deb8702871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007deb87039e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000006fc50d9 BuildConvertedConstantExpression(clang::Sema&, clang::Expr*, clang::QualType, clang::CCEKind, clang::NamedDecl*, clang::APValue&) SemaOverload.cpp:0:0
#10 0x0000000006fc544a clang::Sema::CheckConvertedConstantExpression(clang::Expr*, clang::QualType, llvm::APSInt&, clang::CCEKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6fc544a)
#11 0x00000000072d5411 clang::Sema::BuildPackIndexingExpr(clang::Expr*, clang::SourceLocation, clang::Expr*, clang::SourceLocation, llvm::ArrayRef<clang::Expr*>, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x72d5411)
#12 0x00000000072d6fa6 clang::Sema::ActOnPackIndexingExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, clang::SourceLocation, clang::Expr*, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x72d6fa6)
#13 0x0000000006757487 clang::Parser::ParseCXXPackIndexingExpression(clang::ActionResult<clang::Expr*, true>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6757487)
#14 0x0000000006764bdf clang::Parser::tryParseCXXIdExpression(clang::CXXScopeSpec&, bool, clang::Token&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6764bdf)
#15 0x0000000006748532 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6748532)
#16 0x000000000674a127 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x674a127)
#17 0x000000000674a1b9 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x674a1b9)
#18 0x000000000674e869 clang::Parser::ParseExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x674e869)
#19 0x00000000067e3ce9 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67e3ce9)
#20 0x00000000067db464 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67db464)
#21 0x00000000067dc2a9 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67dc2a9)
#22 0x00000000067e43fb clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67e43fb)
#23 0x00000000067e4c0a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67e4c0a)
#24 0x00000000066db11b clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66db11b)
#25 0x000000000671157d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x671157d)
#26 0x00000000067f0948 clang::Parser::ParseDeclarationAfterTemplate(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo&, clang::ParsingDeclRAIIObject&, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67f0948)
#27 0x00000000067fdb29 clang::Parser::ParseTemplateDeclarationOrSpecialization(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (.part.0) ParseTemplate.cpp:0:0
#28 0x00000000067fdf3a clang::Parser::ParseDeclarationStartingWithTemplate(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67fdf3a)
#29 0x0000000006719eb0 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6719eb0)
#30 0x00000000066d657c clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66d657c)
#31 0x00000000066d7465 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66d7465)
#32 0x00000000066c8fea clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66c8fea)
#33 0x00000000049aeef8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49aeef8)
#34 0x0000000004ca4185 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ca4185)
#35 0x0000000004c2032e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c2032e)
#36 0x0000000004d961b1 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d961b1)
#37 0x0000000000db3daf cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb3daf)
#38 0x0000000000daaa5a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#39 0x0000000004a16ed9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#40 0x0000000003f7d284 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f7d284)
#41 0x0000000004a174ef clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#42 0x00000000049d91ad clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49d91ad)
#43 0x00000000049da23e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49da23e)
#44 0x00000000049e1e75 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49e1e75)
#45 0x0000000000db0265 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb0265)
#46 0x0000000000c63c04 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc63c04)
#47 0x00007deb87029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#48 0x00007deb87029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#49 0x0000000000daa505 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdaa505)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/dYhcrndcE
```cpp
struct A { int a : 16; };

template&lt;typename T&gt; T &amp;make();

template &lt;typename T&gt;
void foo() {
  A&amp; a = make&lt;A&gt;();
  auto &amp;[...an] = a;
  an...[0] = 42;
}

void bar = foo&lt;int&gt;();
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/Sema/SemaOverload.cpp:6280: clang::ExprResult BuildConvertedConstantExpression(clang::Sema&amp;, clang::Expr*, clang::QualType, clang::CCEKind, clang::NamedDecl*, clang::APValue&amp;): Assertion `(S.getLangOpts().CPlusPlus11 || isCCEAllowedPreCXX11) &amp;&amp; "converted constant expression outside C++11 or TTP matching"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++03 &lt;source&gt;
1.	&lt;source&gt;:9:12: current parser token '='
2.	&lt;source&gt;:6:12: parsing function body 'foo'
3.	&lt;source&gt;:6:12: in compound statement ('{}')
 #<!-- -->0 0x000000000403b2a8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x403b2a8)
 #<!-- -->1 0x00000000040386d4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x40386d4)
 #<!-- -->2 0x0000000003f7cde8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007deb87042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007deb870969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007deb87042476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007deb870287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x00007deb8702871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x00007deb87039e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000006fc50d9 BuildConvertedConstantExpression(clang::Sema&amp;, clang::Expr*, clang::QualType, clang::CCEKind, clang::NamedDecl*, clang::APValue&amp;) SemaOverload.cpp:0:0
#<!-- -->10 0x0000000006fc544a clang::Sema::CheckConvertedConstantExpression(clang::Expr*, clang::QualType, llvm::APSInt&amp;, clang::CCEKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6fc544a)
#<!-- -->11 0x00000000072d5411 clang::Sema::BuildPackIndexingExpr(clang::Expr*, clang::SourceLocation, clang::Expr*, clang::SourceLocation, llvm::ArrayRef&lt;clang::Expr*&gt;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x72d5411)
#<!-- -->12 0x00000000072d6fa6 clang::Sema::ActOnPackIndexingExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, clang::SourceLocation, clang::Expr*, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x72d6fa6)
#<!-- -->13 0x0000000006757487 clang::Parser::ParseCXXPackIndexingExpression(clang::ActionResult&lt;clang::Expr*, true&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6757487)
#<!-- -->14 0x0000000006764bdf clang::Parser::tryParseCXXIdExpression(clang::CXXScopeSpec&amp;, bool, clang::Token&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6764bdf)
#<!-- -->15 0x0000000006748532 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&amp;, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6748532)
#<!-- -->16 0x000000000674a127 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x674a127)
#<!-- -->17 0x000000000674a1b9 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x674a1b9)
#<!-- -->18 0x000000000674e869 clang::Parser::ParseExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x674e869)
#<!-- -->19 0x00000000067e3ce9 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67e3ce9)
#<!-- -->20 0x00000000067db464 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67db464)
#<!-- -->21 0x00000000067dc2a9 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67dc2a9)
#<!-- -->22 0x00000000067e43fb clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67e43fb)
#<!-- -->23 0x00000000067e4c0a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67e4c0a)
#<!-- -->24 0x00000000066db11b clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66db11b)
#<!-- -->25 0x000000000671157d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x671157d)
#<!-- -->26 0x00000000067f0948 clang::Parser::ParseDeclarationAfterTemplate(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo&amp;, clang::ParsingDeclRAIIObject&amp;, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67f0948)
#<!-- -->27 0x00000000067fdb29 clang::Parser::ParseTemplateDeclarationOrSpecialization(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::AccessSpecifier) (.part.0) ParseTemplate.cpp:0:0
#<!-- -->28 0x00000000067fdf3a clang::Parser::ParseDeclarationStartingWithTemplate(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67fdf3a)
#<!-- -->29 0x0000000006719eb0 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6719eb0)
#<!-- -->30 0x00000000066d657c clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66d657c)
#<!-- -->31 0x00000000066d7465 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66d7465)
#<!-- -->32 0x00000000066c8fea clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66c8fea)
#<!-- -->33 0x00000000049aeef8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49aeef8)
#<!-- -->34 0x0000000004ca4185 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ca4185)
#<!-- -->35 0x0000000004c2032e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c2032e)
#<!-- -->36 0x0000000004d961b1 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d961b1)
#<!-- -->37 0x0000000000db3daf cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb3daf)
#<!-- -->38 0x0000000000daaa5a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->39 0x0000000004a16ed9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->40 0x0000000003f7d284 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f7d284)
#<!-- -->41 0x0000000004a174ef clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->42 0x00000000049d91ad clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49d91ad)
#<!-- -->43 0x00000000049da23e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49da23e)
#<!-- -->44 0x00000000049e1e75 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49e1e75)
#<!-- -->45 0x0000000000db0265 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb0265)
#<!-- -->46 0x0000000000c63c04 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc63c04)
#<!-- -->47 0x00007deb87029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->48 0x00007deb87029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->49 0x0000000000daa505 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdaa505)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

### Comment 2 - k-arrows

This is trunk only:
https://godbolt.org/z/GxfvYMsKT

---

### Comment 3 - k-arrows

may be related to https://github.com/llvm/llvm-project/pull/152985

---

### Comment 4 - shafik

Note uses `-std=c++03`

---

### Comment 5 - shafik

CC @ricejasonf make sense this is linked to abc8812df02599fc413d9ed77b992f8236ed2af9

---

### Comment 6 - ricejasonf

This is just building the `0` in the pack indexing expression:
```
template <int ...an>
void foo() {
  an...[0] = 42; 
}
```
That hits the same assertion. CompilerExplorer is down...

(with `-std=c++03`)


---

