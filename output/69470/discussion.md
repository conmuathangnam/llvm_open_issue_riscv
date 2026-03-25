# clang: Assertion 'idx < size()' failed. In function `clang::Sema::tryCaptureVariable`

**Author:** wierton
**URL:** https://github.com/llvm/llvm-project/issues/69470
**Status:** Closed
**Labels:** clang:frontend, crash
**Closed Date:** 2025-06-13T10:45:42Z

## Body

The link for quick verification: https://gcc.godbolt.org/z/Y8EWoxv13

The bug triggering program (with option `-fopenmp-simd`):
```
int (^a) (int i, int j) = ^(int i, int j)
{ return i / j; }/ j;
```

Stack dump is pasted below:
```
<source>:1:6: error: blocks support disabled - compile with -fblocks or pick a deployment target that supports them
    1 | int (^a) (int i, int j) = ^(int i, int j)
      |      ^
<source>:1:27: error: blocks support disabled - compile with -fblocks or pick a deployment target that supports them
    1 | int (^a) (int i, int j) = ^(int i, int j)
      |                           ^
<source>:2:20: error: use of undeclared identifier 'j'
    2 | { return i / j; }/ j;
      |                    ^
clang: /root/llvm-project/llvm/include/llvm/ADT/SmallVector.h:294: T& llvm::SmallVectorTemplateCommon<T, <template-parameter-1-2> >::operator[](llvm::SmallVectorTemplateCommon<T, <template-parameter-1-2> >::size_type) [with T = clang::sema::FunctionScopeInfo*; <template-parameter-1-2> = void; llvm::SmallVectorTemplateCommon<T, <template-parameter-1-2> >::reference = clang::sema::FunctionScopeInfo*&; llvm::SmallVectorTemplateCommon<T, <template-parameter-1-2> >::size_type = long unsigned int]: Assertion `idx < size()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang -gdwarf-4 -g -o /app/output.s -S --gcc-toolchain=/opt/compiler-explorer/gcc-9.2.0 -fcolor-diagnostics -fno-crash-diagnostics -fopenmp-simd <source>
1.	<source>:2:21: current parser token ';'
 #0 0x00000000037346b8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x37346b8)
 #1 0x000000000373237c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x373237c)
 #2 0x000000000367ac28 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007fa357695420 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x14420)
 #4 0x00007fa35715800b raise (/lib/x86_64-linux-gnu/libc.so.6+0x4300b)
 #5 0x00007fa357137859 abort (/lib/x86_64-linux-gnu/libc.so.6+0x22859)
 #6 0x00007fa357137729 (/lib/x86_64-linux-gnu/libc.so.6+0x22729)
 #7 0x00007fa357148fd6 (/lib/x86_64-linux-gnu/libc.so.6+0x33fd6)
 #8 0x00000000063f5033 clang::Sema::tryCaptureVariable(clang::ValueDecl*, clang::SourceLocation, clang::Sema::TryCaptureKind, clang::SourceLocation, bool, clang::QualType&, clang::QualType&, unsigned int const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x63f5033)
 #9 0x00000000063f5411 clang::Sema::getCapturedDeclRefType(clang::ValueDecl*, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x63f5411)
#10 0x0000000006428471 clang::Sema::BuildDeclarationNameExpr(clang::CXXScopeSpec const&, clang::DeclarationNameInfo const&, clang::NamedDecl*, clang::NamedDecl*, clang::TemplateArgumentListInfo const*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6428471)
#11 0x0000000006560260 clang::TreeTransform<clang::Sema::CorrectDelayedTyposInExpr(clang::Expr*, clang::VarDecl*, bool, llvm::function_ref<clang::ActionResult<clang::Expr*, true> (clang::Expr*)>)::TyposReplace>::TransformDeclRefExpr(clang::DeclRefExpr*) SemaExprCXX.cpp:0:0
#12 0x0000000006562b08 clang::TreeTransform<clang::Sema::CorrectDelayedTyposInExpr(clang::Expr*, clang::VarDecl*, bool, llvm::function_ref<clang::ActionResult<clang::Expr*, true> (clang::Expr*)>)::TyposReplace>::TransformBinaryOperator(clang::BinaryOperator*) SemaExprCXX.cpp:0:0
#13 0x0000000006560ef8 clang::TreeTransform<clang::Sema::CorrectDelayedTyposInExpr(clang::Expr*, clang::VarDecl*, bool, llvm::function_ref<clang::ActionResult<clang::Expr*, true> (clang::Expr*)>)::TyposReplace>::TransformInitializer(clang::Expr*, bool) (.part.0) SemaExprCXX.cpp:0:0
#14 0x000000000656127c clang::TreeTransform<clang::Sema::CorrectDelayedTyposInExpr(clang::Expr*, clang::VarDecl*, bool, llvm::function_ref<clang::ActionResult<clang::Expr*, true> (clang::Expr*)>)::TyposReplace>::TransformReturnStmt(clang::ReturnStmt*) SemaExprCXX.cpp:0:0
#15 0x0000000006576cb4 clang::TreeTransform<clang::Sema::CorrectDelayedTyposInExpr(clang::Expr*, clang::VarDecl*, bool, llvm::function_ref<clang::ActionResult<clang::Expr*, true> (clang::Expr*)>)::TyposReplace>::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaExprCXX.cpp:0:0
#16 0x000000000657973c clang::TreeTransform<clang::Sema::CorrectDelayedTyposInExpr(clang::Expr*, clang::VarDecl*, bool, llvm::function_ref<clang::ActionResult<clang::Expr*, true> (clang::Expr*)>)::TyposReplace>::TransformBlockExpr(clang::BlockExpr*) SemaExprCXX.cpp:0:0
#17 0x0000000006562b08 clang::TreeTransform<clang::Sema::CorrectDelayedTyposInExpr(clang::Expr*, clang::VarDecl*, bool, llvm::function_ref<clang::ActionResult<clang::Expr*, true> (clang::Expr*)>)::TyposReplace>::TransformBinaryOperator(clang::BinaryOperator*) SemaExprCXX.cpp:0:0
#18 0x000000000654f13d clang::Sema::CorrectDelayedTyposInExpr(clang::Expr*, clang::VarDecl*, bool, llvm::function_ref<clang::ActionResult<clang::Expr*, true> (clang::Expr*)>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x654f13d)
#19 0x000000000620d29c clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x620d29c)
#20 0x0000000005ed15e3 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5ed15e3)
#21 0x0000000005edecb0 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5edecb0)
#22 0x0000000005eac30b clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5eac30b)
#23 0x0000000005eaca3f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (.part.0) Parser.cpp:0:0
#24 0x0000000005eb43e4 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5eb43e4)
#25 0x0000000005eb4c0d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5eb4c0d)
#26 0x0000000005eb5050 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5eb5050)
#27 0x0000000005ea7c82 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5ea7c82)
#28 0x00000000049a0c78 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x49a0c78)
#29 0x00000000042046a9 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x42046a9)
#30 0x0000000004185b4e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4185b4e)
#31 0x00000000042e382e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x42e382e)
#32 0x0000000000bf0b06 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xbf0b06)
#33 0x0000000000be83ca ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#34 0x0000000003fe1d39 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#35 0x000000000367b0d4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x367b0d4)
#36 0x0000000003fe232f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#37 0x0000000003faa4f5 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3faa4f5)
#38 0x0000000003faaf5d clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3faaf5d)
#39 0x0000000003fb2e85 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3fb2e85)
#40 0x0000000000bedfac clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xbedfac)
#41 0x0000000000ae8141 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xae8141)
#42 0x00007fa357139083 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x24083)
#43 0x0000000000be7eae _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xbe7eae)
clang: error: clang frontend command failed with exit code 134 (use -v to see invocation)
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: wt.cc (wierton)

<details>
The link for quick verification: https://gcc.godbolt.org/z/Y8EWoxv13

The bug triggering program (with option `-fopenmp-simd`):
```
int (^a) (int i, int j) = ^(int i, int j)
{ return i / j; }/ j;
```

Stack dump is pasted below:
```
&lt;source&gt;:1:6: error: blocks support disabled - compile with -fblocks or pick a deployment target that supports them
    1 | int (^a) (int i, int j) = ^(int i, int j)
      |      ^
&lt;source&gt;:1:27: error: blocks support disabled - compile with -fblocks or pick a deployment target that supports them
    1 | int (^a) (int i, int j) = ^(int i, int j)
      |                           ^
&lt;source&gt;:2:20: error: use of undeclared identifier 'j'
    2 | { return i / j; }/ j;
      |                    ^
clang: /root/llvm-project/llvm/include/llvm/ADT/SmallVector.h:294: T&amp; llvm::SmallVectorTemplateCommon&lt;T, &lt;template-parameter-1-2&gt; &gt;::operator[](llvm::SmallVectorTemplateCommon&lt;T, &lt;template-parameter-1-2&gt; &gt;::size_type) [with T = clang::sema::FunctionScopeInfo*; &lt;template-parameter-1-2&gt; = void; llvm::SmallVectorTemplateCommon&lt;T, &lt;template-parameter-1-2&gt; &gt;::reference = clang::sema::FunctionScopeInfo*&amp;; llvm::SmallVectorTemplateCommon&lt;T, &lt;template-parameter-1-2&gt; &gt;::size_type = long unsigned int]: Assertion `idx &lt; size()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang -gdwarf-4 -g -o /app/output.s -S --gcc-toolchain=/opt/compiler-explorer/gcc-9.2.0 -fcolor-diagnostics -fno-crash-diagnostics -fopenmp-simd &lt;source&gt;
1.	&lt;source&gt;:2:21: current parser token ';'
 #<!-- -->0 0x00000000037346b8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x37346b8)
 #<!-- -->1 0x000000000373237c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x373237c)
 #<!-- -->2 0x000000000367ac28 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007fa357695420 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x14420)
 #<!-- -->4 0x00007fa35715800b raise (/lib/x86_64-linux-gnu/libc.so.6+0x4300b)
 #<!-- -->5 0x00007fa357137859 abort (/lib/x86_64-linux-gnu/libc.so.6+0x22859)
 #<!-- -->6 0x00007fa357137729 (/lib/x86_64-linux-gnu/libc.so.6+0x22729)
 #<!-- -->7 0x00007fa357148fd6 (/lib/x86_64-linux-gnu/libc.so.6+0x33fd6)
 #<!-- -->8 0x00000000063f5033 clang::Sema::tryCaptureVariable(clang::ValueDecl*, clang::SourceLocation, clang::Sema::TryCaptureKind, clang::SourceLocation, bool, clang::QualType&amp;, clang::QualType&amp;, unsigned int const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x63f5033)
 #<!-- -->9 0x00000000063f5411 clang::Sema::getCapturedDeclRefType(clang::ValueDecl*, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x63f5411)
#<!-- -->10 0x0000000006428471 clang::Sema::BuildDeclarationNameExpr(clang::CXXScopeSpec const&amp;, clang::DeclarationNameInfo const&amp;, clang::NamedDecl*, clang::NamedDecl*, clang::TemplateArgumentListInfo const*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6428471)
#<!-- -->11 0x0000000006560260 clang::TreeTransform&lt;clang::Sema::CorrectDelayedTyposInExpr(clang::Expr*, clang::VarDecl*, bool, llvm::function_ref&lt;clang::ActionResult&lt;clang::Expr*, true&gt; (clang::Expr*)&gt;)::TyposReplace&gt;::TransformDeclRefExpr(clang::DeclRefExpr*) SemaExprCXX.cpp:0:0
#<!-- -->12 0x0000000006562b08 clang::TreeTransform&lt;clang::Sema::CorrectDelayedTyposInExpr(clang::Expr*, clang::VarDecl*, bool, llvm::function_ref&lt;clang::ActionResult&lt;clang::Expr*, true&gt; (clang::Expr*)&gt;)::TyposReplace&gt;::TransformBinaryOperator(clang::BinaryOperator*) SemaExprCXX.cpp:0:0
#<!-- -->13 0x0000000006560ef8 clang::TreeTransform&lt;clang::Sema::CorrectDelayedTyposInExpr(clang::Expr*, clang::VarDecl*, bool, llvm::function_ref&lt;clang::ActionResult&lt;clang::Expr*, true&gt; (clang::Expr*)&gt;)::TyposReplace&gt;::TransformInitializer(clang::Expr*, bool) (.part.0) SemaExprCXX.cpp:0:0
#<!-- -->14 0x000000000656127c clang::TreeTransform&lt;clang::Sema::CorrectDelayedTyposInExpr(clang::Expr*, clang::VarDecl*, bool, llvm::function_ref&lt;clang::ActionResult&lt;clang::Expr*, true&gt; (clang::Expr*)&gt;)::TyposReplace&gt;::TransformReturnStmt(clang::ReturnStmt*) SemaExprCXX.cpp:0:0
#<!-- -->15 0x0000000006576cb4 clang::TreeTransform&lt;clang::Sema::CorrectDelayedTyposInExpr(clang::Expr*, clang::VarDecl*, bool, llvm::function_ref&lt;clang::ActionResult&lt;clang::Expr*, true&gt; (clang::Expr*)&gt;)::TyposReplace&gt;::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaExprCXX.cpp:0:0
#<!-- -->16 0x000000000657973c clang::TreeTransform&lt;clang::Sema::CorrectDelayedTyposInExpr(clang::Expr*, clang::VarDecl*, bool, llvm::function_ref&lt;clang::ActionResult&lt;clang::Expr*, true&gt; (clang::Expr*)&gt;)::TyposReplace&gt;::TransformBlockExpr(clang::BlockExpr*) SemaExprCXX.cpp:0:0
#<!-- -->17 0x0000000006562b08 clang::TreeTransform&lt;clang::Sema::CorrectDelayedTyposInExpr(clang::Expr*, clang::VarDecl*, bool, llvm::function_ref&lt;clang::ActionResult&lt;clang::Expr*, true&gt; (clang::Expr*)&gt;)::TyposReplace&gt;::TransformBinaryOperator(clang::BinaryOperator*) SemaExprCXX.cpp:0:0
#<!-- -->18 0x000000000654f13d clang::Sema::CorrectDelayedTyposInExpr(clang::Expr*, clang::VarDecl*, bool, llvm::function_ref&lt;clang::ActionResult&lt;clang::Expr*, true&gt; (clang::Expr*)&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x654f13d)
#<!-- -->19 0x000000000620d29c clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x620d29c)
#<!-- -->20 0x0000000005ed15e3 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5ed15e3)
#<!-- -->21 0x0000000005edecb0 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5edecb0)
#<!-- -->22 0x0000000005eac30b clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5eac30b)
#<!-- -->23 0x0000000005eaca3f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (.part.0) Parser.cpp:0:0
#<!-- -->24 0x0000000005eb43e4 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5eb43e4)
#<!-- -->25 0x0000000005eb4c0d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5eb4c0d)
#<!-- -->26 0x0000000005eb5050 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5eb5050)
#<!-- -->27 0x0000000005ea7c82 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5ea7c82)
#<!-- -->28 0x00000000049a0c78 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x49a0c78)
#<!-- -->29 0x00000000042046a9 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x42046a9)
#<!-- -->30 0x0000000004185b4e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4185b4e)
#<!-- -->31 0x00000000042e382e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x42e382e)
#<!-- -->32 0x0000000000bf0b06 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xbf0b06)
#<!-- -->33 0x0000000000be83ca ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->34 0x0000000003fe1d39 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->35 0x000000000367b0d4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x367b0d4)
#<!-- -->36 0x0000000003fe232f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->37 0x0000000003faa4f5 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3faa4f5)
#<!-- -->38 0x0000000003faaf5d clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3faaf5d)
#<!-- -->39 0x0000000003fb2e85 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3fb2e85)
#<!-- -->40 0x0000000000bedfac clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xbedfac)
#<!-- -->41 0x0000000000ae8141 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xae8141)
#<!-- -->42 0x00007fa357139083 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x24083)
#<!-- -->43 0x0000000000be7eae _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xbe7eae)
clang: error: clang frontend command failed with exit code 134 (use -v to see invocation)
```
</details>


---

