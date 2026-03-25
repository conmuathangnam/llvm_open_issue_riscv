# Clang ICE: constexpr evaluation of ext_vector_type assignment triggers APValue::getVectorElt assertion

**Author:** AbyssStaror
**URL:** https://github.com/llvm/llvm-project/issues/173491

## Body

### Link for quick verification: https://godbolt.org/z/1PxjvMed1
```c
#include <vector>
typedef __attribute__((ext_vector_type(64))) __int128 vtype ;

constexpr vtype fill() {
    vtype result;
    for (size_t i = 0; i < 64; ++i) {
        result[i] = i;
    }
    return result;
}
```
Go back to `clang 20.1.0`.
The code uses a `constexpr` function that initializes a Clang vector extension type `(__attribute__((ext_vector_type(64))))` of `__int128` via indexed assignment. This may be an inconsistency between vector element lvalue handling and APValue representation during constexpr evaluation.

## Stack dump:
```console
clang++: /root/llvm-project/llvm/tools/clang/include/clang/AST/APValue.h:564: clang::APValue& clang::APValue::getVectorElt(unsigned int): Assertion `isVector() && "Invalid accessor"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<eof> parser at end of file
2.	<source>:5:24: parsing function body 'fill'
 #0 0x00000000042ebc28 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42ebc28)
 #1 0x00000000042e9054 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42e9054)
 #2 0x000000000422de38 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x0000727ea3a42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x0000727ea3a969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x0000727ea3a42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x0000727ea3a287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x0000727ea3a2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x0000727ea3a39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000007cdcbe7 clang::APValue::getVectorElt(unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7cdcbe7)
#10 0x0000000007cfea73 handleAssignment((anonymous namespace)::EvalInfo&, clang::Expr const*, (anonymous namespace)::LValue const&, clang::QualType, clang::APValue&) ExprConstant.cpp:0:0
#11 0x0000000007d4070e clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::LValueExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#12 0x0000000007d40fdd EvaluateLValue(clang::Expr const*, (anonymous namespace)::LValue&, (anonymous namespace)::EvalInfo&, bool) ExprConstant.cpp:0:0
#13 0x0000000007d0a8b6 Evaluate(clang::APValue&, (anonymous namespace)::EvalInfo&, clang::Expr const*) ExprConstant.cpp:0:0
#14 0x0000000007d134b4 EvaluateIgnoredValue((anonymous namespace)::EvalInfo&, clang::Expr const*) ExprConstant.cpp:0:0
#15 0x0000000007d24e97 EvaluateStmt((anonymous namespace)::StmtResult&, (anonymous namespace)::EvalInfo&, clang::Stmt const*, clang::SwitchCase const*) (.part.0) ExprConstant.cpp:0:0
#16 0x0000000007d25648 EvaluateStmt((anonymous namespace)::StmtResult&, (anonymous namespace)::EvalInfo&, clang::Stmt const*, clang::SwitchCase const*) (.part.0) ExprConstant.cpp:0:0
#17 0x0000000007d8eb2a EvaluateLoopBody((anonymous namespace)::StmtResult&, (anonymous namespace)::EvalInfo&, clang::Stmt const*, clang::SwitchCase const*) ExprConstant.cpp:0:0
#18 0x0000000007d253be EvaluateStmt((anonymous namespace)::StmtResult&, (anonymous namespace)::EvalInfo&, clang::Stmt const*, clang::SwitchCase const*) (.part.0) ExprConstant.cpp:0:0
#19 0x0000000007d25648 EvaluateStmt((anonymous namespace)::StmtResult&, (anonymous namespace)::EvalInfo&, clang::Stmt const*, clang::SwitchCase const*) (.part.0) ExprConstant.cpp:0:0
#20 0x0000000007d2b6fb HandleFunctionCall(clang::SourceLocation, clang::FunctionDecl const*, (anonymous namespace)::LValue const*, clang::Expr const*, llvm::ArrayRef<clang::Expr const*>, (anonymous namespace)::CallRef, clang::Stmt const*, (anonymous namespace)::EvalInfo&, clang::APValue&, (anonymous namespace)::LValue const*) ExprConstant.cpp:0:0
#21 0x0000000007d444c4 clang::Expr::isPotentialConstantExpr(clang::FunctionDecl const*, llvm::SmallVectorImpl<std::pair<clang::SourceLocation, clang::PartialDiagnostic>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7d444c4)
#22 0x0000000006e7ff7e clang::Sema::CheckConstexprFunctionDefinition(clang::FunctionDecl const*, clang::Sema::CheckConstexprKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e7ff7e)
#23 0x0000000006e0578b clang::Sema::ActOnFinishFunctionBody(clang::Decl*, clang::Stmt*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e0578b)
#24 0x0000000006b238c4 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b238c4)
#25 0x0000000006a297eb clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a297eb)
#26 0x0000000006a5fcad clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a5fcad)
#27 0x0000000006a1c94b clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a1c94b)
#28 0x0000000006a1d13f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a1d13f)
#29 0x0000000006a24f6a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a24f6a)
#30 0x0000000006a25f05 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a25f05)
#31 0x0000000006a06f8a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a06f8a)
#32 0x0000000004c4cfb8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c4cfb8)
#33 0x0000000004f438b5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f438b5)
#34 0x0000000004ec3b4e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ec3b4e)
#35 0x000000000503dc3d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x503dc3d)
#36 0x0000000000de5a6c cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde5a6c)
#37 0x0000000000ddc4ba ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#38 0x0000000000ddc63d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#39 0x0000000004cbf909 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#40 0x000000000422e2d4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x422e2d4)
#41 0x0000000004cbff1f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#42 0x0000000004c80a02 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c80a02)
#43 0x0000000004c819ae clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c819ae)
#44 0x0000000004c88e05 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c88e05)
#45 0x0000000000de1e61 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde1e61)
#46 0x0000000000c8ceb4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc8ceb4)
#47 0x0000727ea3a29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#48 0x0000727ea3a29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#49 0x0000000000ddbf55 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xddbf55)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (AbyssStaror)

<details>
### Link for quick verification: https://godbolt.org/z/1PxjvMed1
```c
#include &lt;vector&gt;
typedef __attribute__((ext_vector_type(64))) __int128 vtype ;

constexpr vtype fill() {
    vtype result;
    for (size_t i = 0; i &lt; 64; ++i) {
        result[i] = i;
    }
    return result;
}
```
Go back to `clang 20.1.0`.
The code uses a `constexpr` function that initializes a Clang vector extension type `(__attribute__((ext_vector_type(64))))` of `__int128` via indexed assignment. This may be an inconsistency between vector element lvalue handling and APValue representation during constexpr evaluation.

## Stack dump:
```console
clang++: /root/llvm-project/llvm/tools/clang/include/clang/AST/APValue.h:564: clang::APValue&amp; clang::APValue::getVectorElt(unsigned int): Assertion `isVector() &amp;&amp; "Invalid accessor"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	&lt;source&gt;:5:24: parsing function body 'fill'
 #<!-- -->0 0x00000000042ebc28 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42ebc28)
 #<!-- -->1 0x00000000042e9054 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42e9054)
 #<!-- -->2 0x000000000422de38 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x0000727ea3a42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x0000727ea3a969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x0000727ea3a42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x0000727ea3a287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x0000727ea3a2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x0000727ea3a39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000007cdcbe7 clang::APValue::getVectorElt(unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7cdcbe7)
#<!-- -->10 0x0000000007cfea73 handleAssignment((anonymous namespace)::EvalInfo&amp;, clang::Expr const*, (anonymous namespace)::LValue const&amp;, clang::QualType, clang::APValue&amp;) ExprConstant.cpp:0:0
#<!-- -->11 0x0000000007d4070e clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::LValueExprEvaluator, bool&gt;::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#<!-- -->12 0x0000000007d40fdd EvaluateLValue(clang::Expr const*, (anonymous namespace)::LValue&amp;, (anonymous namespace)::EvalInfo&amp;, bool) ExprConstant.cpp:0:0
#<!-- -->13 0x0000000007d0a8b6 Evaluate(clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Expr const*) ExprConstant.cpp:0:0
#<!-- -->14 0x0000000007d134b4 EvaluateIgnoredValue((anonymous namespace)::EvalInfo&amp;, clang::Expr const*) ExprConstant.cpp:0:0
#<!-- -->15 0x0000000007d24e97 EvaluateStmt((anonymous namespace)::StmtResult&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Stmt const*, clang::SwitchCase const*) (.part.0) ExprConstant.cpp:0:0
#<!-- -->16 0x0000000007d25648 EvaluateStmt((anonymous namespace)::StmtResult&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Stmt const*, clang::SwitchCase const*) (.part.0) ExprConstant.cpp:0:0
#<!-- -->17 0x0000000007d8eb2a EvaluateLoopBody((anonymous namespace)::StmtResult&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Stmt const*, clang::SwitchCase const*) ExprConstant.cpp:0:0
#<!-- -->18 0x0000000007d253be EvaluateStmt((anonymous namespace)::StmtResult&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Stmt const*, clang::SwitchCase const*) (.part.0) ExprConstant.cpp:0:0
#<!-- -->19 0x0000000007d25648 EvaluateStmt((anonymous namespace)::StmtResult&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Stmt const*, clang::SwitchCase const*) (.part.0) ExprConstant.cpp:0:0
#<!-- -->20 0x0000000007d2b6fb HandleFunctionCall(clang::SourceLocation, clang::FunctionDecl const*, (anonymous namespace)::LValue const*, clang::Expr const*, llvm::ArrayRef&lt;clang::Expr const*&gt;, (anonymous namespace)::CallRef, clang::Stmt const*, (anonymous namespace)::EvalInfo&amp;, clang::APValue&amp;, (anonymous namespace)::LValue const*) ExprConstant.cpp:0:0
#<!-- -->21 0x0000000007d444c4 clang::Expr::isPotentialConstantExpr(clang::FunctionDecl const*, llvm::SmallVectorImpl&lt;std::pair&lt;clang::SourceLocation, clang::PartialDiagnostic&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7d444c4)
#<!-- -->22 0x0000000006e7ff7e clang::Sema::CheckConstexprFunctionDefinition(clang::FunctionDecl const*, clang::Sema::CheckConstexprKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e7ff7e)
#<!-- -->23 0x0000000006e0578b clang::Sema::ActOnFinishFunctionBody(clang::Decl*, clang::Stmt*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e0578b)
#<!-- -->24 0x0000000006b238c4 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b238c4)
#<!-- -->25 0x0000000006a297eb clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a297eb)
#<!-- -->26 0x0000000006a5fcad clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a5fcad)
#<!-- -->27 0x0000000006a1c94b clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a1c94b)
#<!-- -->28 0x0000000006a1d13f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a1d13f)
#<!-- -->29 0x0000000006a24f6a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a24f6a)
#<!-- -->30 0x0000000006a25f05 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a25f05)
#<!-- -->31 0x0000000006a06f8a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a06f8a)
#<!-- -->32 0x0000000004c4cfb8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c4cfb8)
#<!-- -->33 0x0000000004f438b5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f438b5)
#<!-- -->34 0x0000000004ec3b4e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ec3b4e)
#<!-- -->35 0x000000000503dc3d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x503dc3d)
#<!-- -->36 0x0000000000de5a6c cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde5a6c)
#<!-- -->37 0x0000000000ddc4ba ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->38 0x0000000000ddc63d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->39 0x0000000004cbf909 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->40 0x000000000422e2d4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x422e2d4)
#<!-- -->41 0x0000000004cbff1f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->42 0x0000000004c80a02 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c80a02)
#<!-- -->43 0x0000000004c819ae clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c819ae)
#<!-- -->44 0x0000000004c88e05 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c88e05)
#<!-- -->45 0x0000000000de1e61 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde1e61)
#<!-- -->46 0x0000000000c8ceb4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc8ceb4)
#<!-- -->47 0x0000727ea3a29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->48 0x0000727ea3a29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->49 0x0000000000ddbf55 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xddbf55)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

### Comment 2 - tbaederr

Minimized:
```c++
typedef __attribute__((ext_vector_type(64))) __int128 vtype ;

constexpr vtype fill() {
    vtype result;
    result[0] = 0;
    return result;
}
```

---

### Comment 3 - shafik

@vikramRH this bisects to 77534291fcbd2c784c54e39a60895e4f60f19742

---

### Comment 4 - ImpleLee

Seems like this issue is now fixed? https://godbolt.org/z/9fEavnc45
Maybe we can find which exact commit fixed this.

---

