# clang++ crash on constexpr simd (gnu vector extension) assignment

**Author:** ImpleLee
**URL:** https://github.com/llvm/llvm-project/issues/176983
**Status:** Closed
**Labels:** duplicate, clang:frontend, crash, constexpr
**Closed Date:** 2026-01-21T07:18:52Z

## Body

Godbolt link: https://godbolt.org/z/G7T4vo13z .

### code
```c++
#include <cstdint>

using u64x4 [[gnu::vector_size(4 * sizeof(std::uint64_t))]] = std::uint64_t;

constexpr u64x4 f() {
  u64x4 result;
  result[0] = 0;
  return result;
}

void g() { f(); }
```

### crash backtrace
```
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -g -o /app/output.s -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<source>:11:1: current parser token 'void'
2.	<source>:5:21: parsing function body 'f'
 #0 0x0000000003d58c08 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3d58c08)
 #1 0x0000000003d565fc llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3d565fc)
 #2 0x0000000003c9ba78 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000070f9c8442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x000070f9c88c6603 std::_Rb_tree_decrement(std::_Rb_tree_node_base const*) (/lib/x86_64-linux-gnu/libstdc++.so.6+0xc6603)
 #5 0x000000000750522e (anonymous namespace)::LValueExprEvaluator::VisitVarDecl(clang::Expr const*, clang::VarDecl const*) ExprConstant.cpp:0:0
 #6 0x000000000753e05a (anonymous namespace)::LValueExprEvaluator::VisitDeclRefExpr(clang::DeclRefExpr const*) ExprConstant.cpp:0:0
 #7 0x0000000007539736 clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::LValueExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
 #8 0x0000000007577794 (anonymous namespace)::VectorExprEvaluator::VisitCastExpr(clang::CastExpr const*) ExprConstant.cpp:0:0
 #9 0x0000000007578064 clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::VectorExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#10 0x0000000007523f6a Evaluate(clang::APValue&, (anonymous namespace)::EvalInfo&, clang::Expr const*) ExprConstant.cpp:0:0
#11 0x0000000007565882 EvaluateStmt((anonymous namespace)::StmtResult&, (anonymous namespace)::EvalInfo&, clang::Stmt const*, clang::SwitchCase const*) (.part.0) ExprConstant.cpp:0:0
#12 0x0000000007564be8 EvaluateStmt((anonymous namespace)::StmtResult&, (anonymous namespace)::EvalInfo&, clang::Stmt const*, clang::SwitchCase const*) (.part.0) ExprConstant.cpp:0:0
#13 0x000000000756ab2e HandleFunctionCall(clang::SourceLocation, clang::FunctionDecl const*, (anonymous namespace)::LValue const*, clang::Expr const*, llvm::ArrayRef<clang::Expr const*>, (anonymous namespace)::CallRef, clang::Stmt const*, (anonymous namespace)::EvalInfo&, clang::APValue&, (anonymous namespace)::LValue const*) (.isra.0) ExprConstant.cpp:0:0
#14 0x0000000007574b2e clang::Expr::isPotentialConstantExpr(clang::FunctionDecl const*, llvm::SmallVectorImpl<std::pair<clang::SourceLocation, clang::PartialDiagnostic>>&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x7574b2e)
#15 0x000000000671c326 clang::Sema::CheckConstexprFunctionDefinition(clang::FunctionDecl const*, clang::Sema::CheckConstexprKind) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x671c326)
#16 0x00000000066a883a clang::Sema::ActOnFinishFunctionBody(clang::Decl*, clang::Stmt*, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x66a883a)
#17 0x00000000063d04b4 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x63d04b4)
#18 0x00000000062bad9b clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62bad9b)
#19 0x000000000630775f clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x630775f)
#20 0x00000000062b4187 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62b4187)
#21 0x00000000062b4927 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62b4927)
#22 0x00000000062bd2ef clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62bd2ef)
#23 0x00000000062beaf7 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62beaf7)
#24 0x000000000629793a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x629793a)
#25 0x000000000462afd5 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x462afd5)
#26 0x000000000493f08a clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x493f08a)
#27 0x00000000048bcceb clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x48bcceb)
#28 0x0000000004a3530b clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4a3530b)
#29 0x0000000000de0d0d cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xde0d0d)
#30 0x0000000000dd8b3b ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#31 0x0000000000dd8bdd int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#32 0x00000000046a5bb9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#33 0x0000000003c9be93 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c9be93)
#34 0x00000000046a5dd9 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#35 0x0000000004668ec2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4668ec2)
#36 0x0000000004669da1 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4669da1)
#37 0x000000000467289c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x467289c)
#38 0x0000000000ddd609 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xddd609)
#39 0x0000000000c92984 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc92984)
#40 0x000070f9c8429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#41 0x000070f9c8429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#42 0x0000000000dd85d5 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdd85d5)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```

### comments

Crash starts from clang++-20, and the current trunk still crashes. The assignment in constexpr seems to be the key. Clang++ does not crash if the assignment or constexpr is removed.

## Comments

### Comment 1 - crackedhandle

Hi, I’d like to work on this issue.
I plan to first add a regression test that reproduces the crash, then debug the constexpr evaluation path (likely in ExprConstant.cpp) to find where vector types are mishandled and add the proper handling or guard to avoid the crash. I’ll post updates here as I make progress.


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Imple Lee (ImpleLee)

<details>
Godbolt link: https://godbolt.org/z/G7T4vo13z .

### code
```c++
#include &lt;cstdint&gt;

using u64x4 [[gnu::vector_size(4 * sizeof(std::uint64_t))]] = std::uint64_t;

constexpr u64x4 f() {
  u64x4 result;
  result[0] = 0;
  return result;
}

void g() { f(); }
```

### crash backtrace
```
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -g -o /app/output.s -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics &lt;source&gt;
1.	&lt;source&gt;:11:1: current parser token 'void'
2.	&lt;source&gt;:5:21: parsing function body 'f'
 #<!-- -->0 0x0000000003d58c08 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3d58c08)
 #<!-- -->1 0x0000000003d565fc llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3d565fc)
 #<!-- -->2 0x0000000003c9ba78 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x000070f9c8442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x000070f9c88c6603 std::_Rb_tree_decrement(std::_Rb_tree_node_base const*) (/lib/x86_64-linux-gnu/libstdc++.so.6+0xc6603)
 #<!-- -->5 0x000000000750522e (anonymous namespace)::LValueExprEvaluator::VisitVarDecl(clang::Expr const*, clang::VarDecl const*) ExprConstant.cpp:0:0
 #<!-- -->6 0x000000000753e05a (anonymous namespace)::LValueExprEvaluator::VisitDeclRefExpr(clang::DeclRefExpr const*) ExprConstant.cpp:0:0
 #<!-- -->7 0x0000000007539736 clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::LValueExprEvaluator, bool&gt;::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
 #<!-- -->8 0x0000000007577794 (anonymous namespace)::VectorExprEvaluator::VisitCastExpr(clang::CastExpr const*) ExprConstant.cpp:0:0
 #<!-- -->9 0x0000000007578064 clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::VectorExprEvaluator, bool&gt;::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#<!-- -->10 0x0000000007523f6a Evaluate(clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Expr const*) ExprConstant.cpp:0:0
#<!-- -->11 0x0000000007565882 EvaluateStmt((anonymous namespace)::StmtResult&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Stmt const*, clang::SwitchCase const*) (.part.0) ExprConstant.cpp:0:0
#<!-- -->12 0x0000000007564be8 EvaluateStmt((anonymous namespace)::StmtResult&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Stmt const*, clang::SwitchCase const*) (.part.0) ExprConstant.cpp:0:0
#<!-- -->13 0x000000000756ab2e HandleFunctionCall(clang::SourceLocation, clang::FunctionDecl const*, (anonymous namespace)::LValue const*, clang::Expr const*, llvm::ArrayRef&lt;clang::Expr const*&gt;, (anonymous namespace)::CallRef, clang::Stmt const*, (anonymous namespace)::EvalInfo&amp;, clang::APValue&amp;, (anonymous namespace)::LValue const*) (.isra.0) ExprConstant.cpp:0:0
#<!-- -->14 0x0000000007574b2e clang::Expr::isPotentialConstantExpr(clang::FunctionDecl const*, llvm::SmallVectorImpl&lt;std::pair&lt;clang::SourceLocation, clang::PartialDiagnostic&gt;&gt;&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x7574b2e)
#<!-- -->15 0x000000000671c326 clang::Sema::CheckConstexprFunctionDefinition(clang::FunctionDecl const*, clang::Sema::CheckConstexprKind) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x671c326)
#<!-- -->16 0x00000000066a883a clang::Sema::ActOnFinishFunctionBody(clang::Decl*, clang::Stmt*, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x66a883a)
#<!-- -->17 0x00000000063d04b4 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x63d04b4)
#<!-- -->18 0x00000000062bad9b clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62bad9b)
#<!-- -->19 0x000000000630775f clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x630775f)
#<!-- -->20 0x00000000062b4187 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62b4187)
#<!-- -->21 0x00000000062b4927 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62b4927)
#<!-- -->22 0x00000000062bd2ef clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62bd2ef)
#<!-- -->23 0x00000000062beaf7 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62beaf7)
#<!-- -->24 0x000000000629793a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x629793a)
#<!-- -->25 0x000000000462afd5 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x462afd5)
#<!-- -->26 0x000000000493f08a clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x493f08a)
#<!-- -->27 0x00000000048bcceb clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x48bcceb)
#<!-- -->28 0x0000000004a3530b clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4a3530b)
#<!-- -->29 0x0000000000de0d0d cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xde0d0d)
#<!-- -->30 0x0000000000dd8b3b ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->31 0x0000000000dd8bdd int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->32 0x00000000046a5bb9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->33 0x0000000003c9be93 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c9be93)
#<!-- -->34 0x00000000046a5dd9 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->35 0x0000000004668ec2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4668ec2)
#<!-- -->36 0x0000000004669da1 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4669da1)
#<!-- -->37 0x000000000467289c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x467289c)
#<!-- -->38 0x0000000000ddd609 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xddd609)
#<!-- -->39 0x0000000000c92984 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc92984)
#<!-- -->40 0x000070f9c8429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->41 0x000070f9c8429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->42 0x0000000000dd85d5 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdd85d5)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```

### comments

Crash starts from clang++-20, and the current trunk still crashes. The assignment in constexpr seems to be the key. Clang++ does not crash if the assignment or constexpr is removed.
</details>


---

