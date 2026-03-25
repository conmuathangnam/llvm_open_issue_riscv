# [clang] Assertion `Constructor->getInheritedConstructor() && !Constructor->doesThisDeclarationHaveABody() && !Constructor->isDeleted()` failed

**Author:** Rush10233
**URL:** https://github.com/llvm/llvm-project/issues/132613

## Body

Goes back to clang-17: https://godbolt.org/z/PTzPbaM4j

Input test:

```c++
struct base {
  base (int, int) {}
};

template<class>
struct derived : base {
    using base::base;
    base derived_base_0;
	derived() : derived_base_0(42, 42) {}
};

int main ()
{
  derived<int> (13, 42);
}
```

Stack Dump:

```c++
clang++: /root/llvm-project/clang/lib/Sema/SemaDeclCXX.cpp:14237: void clang::Sema::DefineInheritingConstructor(clang::SourceLocation, clang::CXXConstructorDecl*): Assertion `Constructor->getInheritedConstructor() && !Constructor->doesThisDeclarationHaveABody() && !Constructor->isDeleted()` failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<source>:14:24: current parser token ';'
2.	<source>:13:1: parsing function body 'main'
3.	<source>:13:1: in compound statement ('{}')
 #0 0x0000000003e95b68 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3e95b68)
 #1 0x0000000003e937f4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3e937f4)
 #2 0x0000000003dd8688 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007bedbac42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007bedbac969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007bedbac42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007bedbac287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007bedbac2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007bedbac39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000006940087 clang::Sema::DefineInheritingConstructor(clang::SourceLocation, clang::CXXConstructorDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6940087)
#10 0x00000000069dc743 void llvm::function_ref<void ()>::callback_fn<clang::Sema::MarkFunctionReferenced(clang::SourceLocation, clang::FunctionDecl*, bool)::'lambda'()>(long) SemaExpr.cpp:0:0
#11 0x0000000007d8f151 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7d8f151)
#12 0x00000000069dac2f clang::Sema::MarkFunctionReferenced(clang::SourceLocation, clang::FunctionDecl*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69dac2f)
#13 0x0000000006ba463f PerformConstructorInitialization(clang::Sema&, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::MutableArrayRef<clang::Expr*>, clang::InitializationSequence::Step const&, bool&, bool, bool, clang::SourceLocation, clang::SourceLocation) SemaInit.cpp:0:0
#14 0x0000000006bab31a clang::InitializationSequence::Perform(clang::Sema&, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::MutableArrayRef<clang::Expr*>, clang::QualType*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bab31a)
#15 0x0000000006ab05e7 clang::Sema::BuildCXXTypeConstructExpr(clang::TypeSourceInfo*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ab05e7)
#16 0x0000000006b0fb4d clang::Sema::ActOnCXXTypeConstructExpr(clang::OpaquePtr<clang::QualType>, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b0fb4d)
#17 0x0000000006552855 clang::Parser::ParseCXXTypeConstructExpression(clang::DeclSpec const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6552855)
#18 0x0000000006535f4a clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6535f4a)
#19 0x0000000006536a87 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6536a87)
#20 0x0000000006536b19 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6536b19)
#21 0x000000000653b679 clang::Parser::ParseExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x653b679)
#22 0x00000000065becc9 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65becc9)
#23 0x00000000065b69e2 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65b69e2)
#24 0x00000000065b790d clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65b790d)
#25 0x00000000065bf3c3 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65bf3c3)
#26 0x00000000065bfb8a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65bfb8a)
#27 0x00000000064c9973 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64c9973)
#28 0x00000000064fef0d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64fef0d)
#29 0x00000000064bd58e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64bd58e)
#30 0x00000000064bdd49 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64bdd49)
#31 0x00000000064c5613 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64c5613)
#32 0x00000000064c652d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64c652d)
#33 0x00000000064b8a7a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64b8a7a)
#34 0x0000000004827f98 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4827f98)
#35 0x0000000004b158e5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b158e5)
#36 0x0000000004a9759e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4a9759e)
#37 0x0000000004c05b4e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c05b4e)
#38 0x0000000000d51dcf cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd51dcf)
#39 0x0000000000d4958a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#40 0x000000000488d679 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#41 0x0000000003dd8b24 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3dd8b24)
#42 0x000000000488dc8f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#43 0x00000000048506fd clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48506fd)
#44 0x000000000485178e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x485178e)
#45 0x0000000004858e65 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4858e65)
#46 0x0000000000d4ebc8 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd4ebc8)
#47 0x0000000000c15884 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc15884)
#48 0x00007bedbac29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#49 0x00007bedbac29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#50 0x0000000000d49035 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd49035)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (Rush10233)

<details>
Goes back to clang-17: https://godbolt.org/z/PTzPbaM4j

Input test:

```c++
struct base {
  base (int, int) {}
};

template&lt;class&gt;
struct derived : base {
    using base::base;
    base derived_base_0;
	derived() : derived_base_0(42, 42) {}
};

int main ()
{
  derived&lt;int&gt; (13, 42);
}
```

Stack Dump:

```c++
clang++: /root/llvm-project/clang/lib/Sema/SemaDeclCXX.cpp:14237: void clang::Sema::DefineInheritingConstructor(clang::SourceLocation, clang::CXXConstructorDecl*): Assertion `Constructor-&gt;getInheritedConstructor() &amp;&amp; !Constructor-&gt;doesThisDeclarationHaveABody() &amp;&amp; !Constructor-&gt;isDeleted()` failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics &lt;source&gt;
1.	&lt;source&gt;:14:24: current parser token ';'
2.	&lt;source&gt;:13:1: parsing function body 'main'
3.	&lt;source&gt;:13:1: in compound statement ('{}')
 #<!-- -->0 0x0000000003e95b68 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3e95b68)
 #<!-- -->1 0x0000000003e937f4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3e937f4)
 #<!-- -->2 0x0000000003dd8688 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007bedbac42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007bedbac969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007bedbac42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007bedbac287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x00007bedbac2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x00007bedbac39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000006940087 clang::Sema::DefineInheritingConstructor(clang::SourceLocation, clang::CXXConstructorDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6940087)
#<!-- -->10 0x00000000069dc743 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::Sema::MarkFunctionReferenced(clang::SourceLocation, clang::FunctionDecl*, bool)::'lambda'()&gt;(long) SemaExpr.cpp:0:0
#<!-- -->11 0x0000000007d8f151 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7d8f151)
#<!-- -->12 0x00000000069dac2f clang::Sema::MarkFunctionReferenced(clang::SourceLocation, clang::FunctionDecl*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69dac2f)
#<!-- -->13 0x0000000006ba463f PerformConstructorInitialization(clang::Sema&amp;, clang::InitializedEntity const&amp;, clang::InitializationKind const&amp;, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::InitializationSequence::Step const&amp;, bool&amp;, bool, bool, clang::SourceLocation, clang::SourceLocation) SemaInit.cpp:0:0
#<!-- -->14 0x0000000006bab31a clang::InitializationSequence::Perform(clang::Sema&amp;, clang::InitializedEntity const&amp;, clang::InitializationKind const&amp;, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::QualType*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bab31a)
#<!-- -->15 0x0000000006ab05e7 clang::Sema::BuildCXXTypeConstructExpr(clang::TypeSourceInfo*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ab05e7)
#<!-- -->16 0x0000000006b0fb4d clang::Sema::ActOnCXXTypeConstructExpr(clang::OpaquePtr&lt;clang::QualType&gt;, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b0fb4d)
#<!-- -->17 0x0000000006552855 clang::Parser::ParseCXXTypeConstructExpression(clang::DeclSpec const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6552855)
#<!-- -->18 0x0000000006535f4a clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6535f4a)
#<!-- -->19 0x0000000006536a87 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6536a87)
#<!-- -->20 0x0000000006536b19 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6536b19)
#<!-- -->21 0x000000000653b679 clang::Parser::ParseExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x653b679)
#<!-- -->22 0x00000000065becc9 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65becc9)
#<!-- -->23 0x00000000065b69e2 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65b69e2)
#<!-- -->24 0x00000000065b790d clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65b790d)
#<!-- -->25 0x00000000065bf3c3 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65bf3c3)
#<!-- -->26 0x00000000065bfb8a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65bfb8a)
#<!-- -->27 0x00000000064c9973 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64c9973)
#<!-- -->28 0x00000000064fef0d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64fef0d)
#<!-- -->29 0x00000000064bd58e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64bd58e)
#<!-- -->30 0x00000000064bdd49 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64bdd49)
#<!-- -->31 0x00000000064c5613 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64c5613)
#<!-- -->32 0x00000000064c652d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64c652d)
#<!-- -->33 0x00000000064b8a7a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64b8a7a)
#<!-- -->34 0x0000000004827f98 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4827f98)
#<!-- -->35 0x0000000004b158e5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b158e5)
#<!-- -->36 0x0000000004a9759e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4a9759e)
#<!-- -->37 0x0000000004c05b4e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c05b4e)
#<!-- -->38 0x0000000000d51dcf cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd51dcf)
#<!-- -->39 0x0000000000d4958a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->40 0x000000000488d679 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->41 0x0000000003dd8b24 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3dd8b24)
#<!-- -->42 0x000000000488dc8f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->43 0x00000000048506fd clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48506fd)
#<!-- -->44 0x000000000485178e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x485178e)
#<!-- -->45 0x0000000004858e65 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4858e65)
#<!-- -->46 0x0000000000d4ebc8 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd4ebc8)
#<!-- -->47 0x0000000000c15884 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc15884)
#<!-- -->48 0x00007bedbac29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->49 0x00007bedbac29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->50 0x0000000000d49035 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd49035)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
```
</details>


---

