# SIGSEGV in EvaluateComparisonBinaryOperator -> clang::Decl::getKind

**Author:** stbergmann
**URL:** https://github.com/llvm/llvm-project/issues/150015
**Status:** Closed
**Labels:** clang:frontend, regression, release:backport, confirmed, crash-on-valid
**Closed Date:** 2025-07-28T16:06:37Z

## Body

Bisected this to <https://github.com/llvm/llvm-project/commit/20c8e3c2a4744524396cc473a370cfb7855850b6> "[clang] Fix pointer comparisons between pointers to constexpr-unknown (https://github.com/llvm/llvm-project/pull/147663)" (and didn't shrink the reproducer beyond including `<algorithm>`, seen it reproducible with at least `libstdc++-devel-15.1.1-2.fc42.x86_64` and recent GCC trunk `libstdc++`):
```
$ cat test.cc
#include <algorithm>
int * f(int (& c)[8])  { return std::find(c, c + 8, 0); }
```
```
$ /home/sberg/llvm/debug/inst/bin/clang++ -D_GLIBCXX_DEBUG -std=c++23 -c test.cc
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /home/sberg/llvm/debug/inst/bin/clang++ -D_GLIBCXX_DEBUG -std=c++23 -c test.cc
1.	<eof> parser at end of file
2.	test.cc:2:7: LLVM IR generation of declaration 'f'
3.	test.cc:2:7: Generating code for declaration 'f'
  #0 0x00007fe0c9458b5d llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /home/sberg/llvm/debug/llvm-project/llvm/lib/Support/Unix/Signals.inc:834:11
  #1 0x00007fe0c945909b PrintStackTraceSignalHandler(void*) /home/sberg/llvm/debug/llvm-project/llvm/lib/Support/Unix/Signals.inc:918:1
  #2 0x00007fe0c9456f9f llvm::sys::RunSignalHandlers() /home/sberg/llvm/debug/llvm-project/llvm/lib/Support/Signals.cpp:104:5
  #3 0x00007fe0c9458469 llvm::sys::CleanupOnSignal(unsigned long) /home/sberg/llvm/debug/llvm-project/llvm/lib/Support/Unix/Signals.inc:374:1
  #4 0x00007fe0c92ed192 (anonymous namespace)::CrashRecoveryContextImpl::HandleCrash(int, unsigned long) /home/sberg/llvm/debug/llvm-project/llvm/lib/Support/CrashRecoveryContext.cpp:0:7
  #5 0x00007fe0c92ed546 CrashRecoverySignalHandler(int) /home/sberg/llvm/debug/llvm-project/llvm/lib/Support/CrashRecoveryContext.cpp:391:1
  #6 0x00007fe0cc227c30 __restore_rt (/lib64/libc.so.6+0x19c30)
  #7 0x00007fe0c4e310ec clang::Decl::getKind() const /home/sberg/llvm/debug/llvm-project/clang/include/clang/AST/DeclBase.h:445:51
  #8 0x00007fe0c4e3adb5 clang::FieldDecl::classof(clang::Decl const*) /home/sberg/llvm/debug/llvm-project/clang/include/clang/AST/Decl.h:3379:62
  #9 0x00007fe0c4e3ad95 llvm::isa_impl<clang::FieldDecl, clang::Decl, void>::doit(clang::Decl const&) /home/sberg/llvm/debug/llvm-project/llvm/include/llvm/Support/Casting.h:64:46
 #10 0x00007fe0c4e3ad72 llvm::isa_impl_cl<clang::FieldDecl, clang::Decl const*>::doit(clang::Decl const*) /home/sberg/llvm/debug/llvm-project/llvm/include/llvm/Support/Casting.h:110:5
 #11 0x00007fe0c4e3ad18 llvm::isa_impl_wrap<clang::FieldDecl, clang::Decl const*, clang::Decl const*>::doit(clang::Decl const* const&) /home/sberg/llvm/debug/llvm-project/llvm/include/llvm/Support/Casting.h:137:5
 #12 0x00007fe0c4e3acf2 llvm::isa_impl_wrap<clang::FieldDecl, clang::Decl const* const, clang::Decl const*>::doit(clang::Decl const* const&) /home/sberg/llvm/debug/llvm-project/llvm/include/llvm/Support/Casting.h:127:5
 #13 0x00007fe0c4e3acc5 llvm::CastIsPossible<clang::FieldDecl, clang::Decl const*, void>::isPossible(clang::Decl const* const&) /home/sberg/llvm/debug/llvm-project/llvm/include/llvm/Support/Casting.h:255:5
 #14 0x00007fe0c4ee5115 llvm::CastInfo<clang::FieldDecl, clang::Decl const*, void>::doCastIfPossible(clang::Decl const* const&) /home/sberg/llvm/debug/llvm-project/llvm/include/llvm/Support/Casting.h:493:9
 #15 0x00007fe0c5317e75 auto llvm::dyn_cast_if_present<clang::FieldDecl, clang::Decl const>(clang::Decl const*) /home/sberg/llvm/debug/llvm-project/llvm/include/llvm/Support/Casting.h:753:3
 #16 0x00007fe0c563e735 auto llvm::dyn_cast_or_null<clang::FieldDecl, clang::Decl const>(clang::Decl const*) /home/sberg/llvm/debug/llvm-project/llvm/include/llvm/Support/Casting.h:768:3
 #17 0x00007fe0c558c38a (anonymous namespace)::getAsField(clang::APValue::LValuePathEntry) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:96:5
 #18 0x00007fe0c55f59ce bool EvaluateComparisonBinaryOperator<(anonymous namespace)::IntExprEvaluator::VisitBinaryOperator(clang::BinaryOperator const*)::$_0&, (anonymous namespace)::IntExprEvaluator::VisitBinaryOperator(clang::BinaryOperator const*)::$_1>((anonymous namespace)::EvalInfo&, clang::BinaryOperator const*, (anonymous namespace)::IntExprEvaluator::VisitBinaryOperator(clang::BinaryOperator const*)::$_0&, (anonymous namespace)::IntExprEvaluator::VisitBinaryOperator(clang::BinaryOperator const*)::$_1&&) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:14576:26
 #19 0x00007fe0c55a56b4 (anonymous namespace)::IntExprEvaluator::VisitBinaryOperator(clang::BinaryOperator const*) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:14780:5
 #20 0x00007fe0c55a197d clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool>::VisitBinLE(clang::BinaryOperator const*) /home/sberg/llvm/debug/llvm-project/clang/include/clang/AST/StmtVisitor.h:133:46
 #21 0x00007fe0c559fc76 clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool>::Visit(clang::Stmt const*) /home/sberg/llvm/debug/llvm-project/clang/include/clang/AST/StmtVisitor.h:63:26
 #22 0x00007fe0c557c681 Evaluate(clang::APValue&, (anonymous namespace)::EvalInfo&, clang::Expr const*) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:16638:9
 #23 0x00007fe0c55f7c6c (anonymous namespace)::DataRecursiveIntBinOpEvaluator::EvaluateExpr(clang::Expr const*, (anonymous namespace)::DataRecursiveIntBinOpEvaluator::EvalResult&) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:14117:22
 #24 0x00007fe0c55f70ed (anonymous namespace)::DataRecursiveIntBinOpEvaluator::process((anonymous namespace)::DataRecursiveIntBinOpEvaluator::EvalResult&) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:14299:7
 #25 0x00007fe0c55f4818 (anonymous namespace)::DataRecursiveIntBinOpEvaluator::Traverse(clang::BinaryOperator const*) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:14082:5
 #26 0x00007fe0c55a55b1 (anonymous namespace)::IntExprEvaluator::VisitBinaryOperator(clang::BinaryOperator const*) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:14748:5
 #27 0x00007fe0c55a1afd clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool>::VisitBinLAnd(clang::BinaryOperator const*) /home/sberg/llvm/debug/llvm-project/clang/include/clang/AST/StmtVisitor.h:138:3
 #28 0x00007fe0c559fd2e clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool>::Visit(clang::Stmt const*) /home/sberg/llvm/debug/llvm-project/clang/include/clang/AST/StmtVisitor.h:72:26
 #29 0x00007fe0c557c681 Evaluate(clang::APValue&, (anonymous namespace)::EvalInfo&, clang::Expr const*) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:16638:9
 #30 0x00007fe0c5599eda EvaluateStmt((anonymous namespace)::StmtResult&, (anonymous namespace)::EvalInfo&, clang::Stmt const*, clang::SwitchCase const*) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:5618:17
 #31 0x00007fe0c5599fd4 EvaluateStmt((anonymous namespace)::StmtResult&, (anonymous namespace)::EvalInfo&, clang::Stmt const*, clang::SwitchCase const*) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:5631:22
 #32 0x00007fe0c557d76d HandleFunctionCall(clang::SourceLocation, clang::FunctionDecl const*, (anonymous namespace)::LValue const*, clang::Expr const*, llvm::ArrayRef<clang::Expr const*>, (anonymous namespace)::CallRef, clang::Stmt const*, (anonymous namespace)::EvalInfo&, clang::APValue&, (anonymous namespace)::LValue const*) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:6663:18
 #33 0x00007fe0c55f2153 (anonymous namespace)::ExprEvaluatorBase<(anonymous namespace)::IntExprEvaluator>::handleCallExpr(clang::CallExpr const*, clang::APValue&, (anonymous namespace)::LValue const*) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:8468:66
 #34 0x00007fe0c55eb9a6 (anonymous namespace)::ExprEvaluatorBase<(anonymous namespace)::IntExprEvaluator>::VisitCallExpr(clang::CallExpr const*) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:8263:9
 #35 0x00007fe0c55a4e92 (anonymous namespace)::IntExprEvaluator::VisitCallExpr(clang::CallExpr const*) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:12933:5
 #36 0x00007fe0c55a082b clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool>::Visit(clang::Stmt const*) /home/sberg/llvm/debug/build/tools/clang/include/clang/AST/StmtNodes.inc:608:1
 #37 0x00007fe0c557c681 Evaluate(clang::APValue&, (anonymous namespace)::EvalInfo&, clang::Expr const*) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:16638:9
 #38 0x00007fe0c5599eda EvaluateStmt((anonymous namespace)::StmtResult&, (anonymous namespace)::EvalInfo&, clang::Stmt const*, clang::SwitchCase const*) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:5618:17
 #39 0x00007fe0c5599fd4 EvaluateStmt((anonymous namespace)::StmtResult&, (anonymous namespace)::EvalInfo&, clang::Stmt const*, clang::SwitchCase const*) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:5631:22
 #40 0x00007fe0c557d76d HandleFunctionCall(clang::SourceLocation, clang::FunctionDecl const*, (anonymous namespace)::LValue const*, clang::Expr const*, llvm::ArrayRef<clang::Expr const*>, (anonymous namespace)::CallRef, clang::Stmt const*, (anonymous namespace)::EvalInfo&, clang::APValue&, (anonymous namespace)::LValue const*) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:6663:18
 #41 0x00007fe0c55f2153 (anonymous namespace)::ExprEvaluatorBase<(anonymous namespace)::IntExprEvaluator>::handleCallExpr(clang::CallExpr const*, clang::APValue&, (anonymous namespace)::LValue const*) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:8468:66
 #42 0x00007fe0c55eb9a6 (anonymous namespace)::ExprEvaluatorBase<(anonymous namespace)::IntExprEvaluator>::VisitCallExpr(clang::CallExpr const*) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:8263:9
 #43 0x00007fe0c55a4e92 (anonymous namespace)::IntExprEvaluator::VisitCallExpr(clang::CallExpr const*) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:12933:5
 #44 0x00007fe0c55a082b clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool>::Visit(clang::Stmt const*) /home/sberg/llvm/debug/build/tools/clang/include/clang/AST/StmtNodes.inc:608:1
 #45 0x00007fe0c557c681 Evaluate(clang::APValue&, (anonymous namespace)::EvalInfo&, clang::Expr const*) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:16638:9
 #46 0x00007fe0c5599eda EvaluateStmt((anonymous namespace)::StmtResult&, (anonymous namespace)::EvalInfo&, clang::Stmt const*, clang::SwitchCase const*) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:5618:17
 #47 0x00007fe0c5599fd4 EvaluateStmt((anonymous namespace)::StmtResult&, (anonymous namespace)::EvalInfo&, clang::Stmt const*, clang::SwitchCase const*) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:5631:22
 #48 0x00007fe0c557d76d HandleFunctionCall(clang::SourceLocation, clang::FunctionDecl const*, (anonymous namespace)::LValue const*, clang::Expr const*, llvm::ArrayRef<clang::Expr const*>, (anonymous namespace)::CallRef, clang::Stmt const*, (anonymous namespace)::EvalInfo&, clang::APValue&, (anonymous namespace)::LValue const*) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:6663:18
 #49 0x00007fe0c55f2153 (anonymous namespace)::ExprEvaluatorBase<(anonymous namespace)::IntExprEvaluator>::handleCallExpr(clang::CallExpr const*, clang::APValue&, (anonymous namespace)::LValue const*) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:8468:66
 #50 0x00007fe0c55eb9a6 (anonymous namespace)::ExprEvaluatorBase<(anonymous namespace)::IntExprEvaluator>::VisitCallExpr(clang::CallExpr const*) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:8263:9
 #51 0x00007fe0c55a4e92 (anonymous namespace)::IntExprEvaluator::VisitCallExpr(clang::CallExpr const*) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:12933:5
 #52 0x00007fe0c55a082b clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool>::Visit(clang::Stmt const*) /home/sberg/llvm/debug/build/tools/clang/include/clang/AST/StmtNodes.inc:608:1
 #53 0x00007fe0c55eb5ed (anonymous namespace)::ExprEvaluatorBase<(anonymous namespace)::IntExprEvaluator>::VisitCastExpr(clang::CastExpr const*) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:8573:7
 #54 0x00007fe0c55ea8d9 (anonymous namespace)::IntExprEvaluator::VisitCastExpr(clang::CastExpr const*) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:15185:5
 #55 0x00007fe0c55eb8cd clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool>::VisitExplicitCastExpr(clang::ExplicitCastExpr const*) /home/sberg/llvm/debug/build/tools/clang/include/clang/AST/StmtNodes.inc:534:1
 #56 0x00007fe0c55a4ded clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool>::VisitCXXFunctionalCastExpr(clang::CXXFunctionalCastExpr const*) /home/sberg/llvm/debug/build/tools/clang/include/clang/AST/StmtNodes.inc:582:1
 #57 0x00007fe0c55a07e6 clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool>::Visit(clang::Stmt const*) /home/sberg/llvm/debug/build/tools/clang/include/clang/AST/StmtNodes.inc:582:1
 #58 0x00007fe0c557c681 Evaluate(clang::APValue&, (anonymous namespace)::EvalInfo&, clang::Expr const*) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:16638:9
 #59 0x00007fe0c559ec78 EvaluateAsBooleanCondition(clang::Expr const*, bool&, (anonymous namespace)::EvalInfo&) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:2769:7
 #60 0x00007fe0c55a26cc (anonymous namespace)::IntExprEvaluator::VisitUnaryOperator(clang::UnaryOperator const*) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:15111:9
 #61 0x00007fe0c55a1f5d clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool>::VisitUnaryLNot(clang::UnaryOperator const*) /home/sberg/llvm/debug/llvm-project/clang/include/clang/AST/StmtVisitor.h:167:31
 #62 0x00007fe0c559ff88 clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool>::Visit(clang::Stmt const*) /home/sberg/llvm/debug/llvm-project/clang/include/clang/AST/StmtVisitor.h:98:26
 #63 0x00007fe0c55eab06 (anonymous namespace)::IntExprEvaluator::VisitCastExpr(clang::CastExpr const*) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:15225:9
 #64 0x00007fe0c55a4c1d clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool>::VisitImplicitCastExpr(clang::ImplicitCastExpr const*) /home/sberg/llvm/debug/build/tools/clang/include/clang/AST/StmtNodes.inc:528:1
 #65 0x00007fe0c55a0745 clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool>::Visit(clang::Stmt const*) /home/sberg/llvm/debug/build/tools/clang/include/clang/AST/StmtNodes.inc:528:1
 #66 0x00007fe0c55ed562 (anonymous namespace)::IntExprEvaluator::VisitBuiltinCallExpr(clang::CallExpr const*, unsigned int) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:13217:5
 #67 0x00007fe0c55a4eb9 (anonymous namespace)::IntExprEvaluator::VisitCallExpr(clang::CallExpr const*) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:12934:3
 #68 0x00007fe0c55a082b clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool>::Visit(clang::Stmt const*) /home/sberg/llvm/debug/build/tools/clang/include/clang/AST/StmtNodes.inc:608:1
 #69 0x00007fe0c557c681 Evaluate(clang::APValue&, (anonymous namespace)::EvalInfo&, clang::Expr const*) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:16638:9
 #70 0x00007fe0c559ec78 EvaluateAsBooleanCondition(clang::Expr const*, bool&, (anonymous namespace)::EvalInfo&) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:2769:7
 #71 0x00007fe0c55ea8fa (anonymous namespace)::IntExprEvaluator::VisitCastExpr(clang::CastExpr const*) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:15195:9
 #72 0x00007fe0c55a4c1d clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool>::VisitImplicitCastExpr(clang::ImplicitCastExpr const*) /home/sberg/llvm/debug/build/tools/clang/include/clang/AST/StmtNodes.inc:528:1
 #73 0x00007fe0c55a0745 clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool>::Visit(clang::Stmt const*) /home/sberg/llvm/debug/build/tools/clang/include/clang/AST/StmtNodes.inc:528:1
 #74 0x00007fe0c557c681 Evaluate(clang::APValue&, (anonymous namespace)::EvalInfo&, clang::Expr const*) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:16638:9
 #75 0x00007fe0c559ec78 EvaluateAsBooleanCondition(clang::Expr const*, bool&, (anonymous namespace)::EvalInfo&) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:2769:7
 #76 0x00007fe0c559eb65 EvaluateCond((anonymous namespace)::EvalInfo&, clang::VarDecl const*, clang::Expr const*, bool&) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:5286:7
 #77 0x00007fe0c559a1d8 EvaluateStmt((anonymous namespace)::StmtResult&, (anonymous namespace)::EvalInfo&, clang::Stmt const*, clang::SwitchCase const*) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:5665:16
 #78 0x00007fe0c5599fd4 EvaluateStmt((anonymous namespace)::StmtResult&, (anonymous namespace)::EvalInfo&, clang::Stmt const*, clang::SwitchCase const*) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:5631:22
 #79 0x00007fe0c559e58a EvaluateLoopBody((anonymous namespace)::StmtResult&, (anonymous namespace)::EvalInfo&, clang::Stmt const*, clang::SwitchCase const*) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:5327:18
 #80 0x00007fe0c559a4b3 EvaluateStmt((anonymous namespace)::StmtResult&, (anonymous namespace)::EvalInfo&, clang::Stmt const*, clang::SwitchCase const*) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:5707:22
 #81 0x00007fe0c5599fd4 EvaluateStmt((anonymous namespace)::StmtResult&, (anonymous namespace)::EvalInfo&, clang::Stmt const*, clang::SwitchCase const*) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:5631:22
 #82 0x00007fe0c557d76d HandleFunctionCall(clang::SourceLocation, clang::FunctionDecl const*, (anonymous namespace)::LValue const*, clang::Expr const*, llvm::ArrayRef<clang::Expr const*>, (anonymous namespace)::CallRef, clang::Stmt const*, (anonymous namespace)::EvalInfo&, clang::APValue&, (anonymous namespace)::LValue const*) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:6663:18
 #83 0x00007fe0c55fef83 (anonymous namespace)::ExprEvaluatorBase<(anonymous namespace)::PointerExprEvaluator>::handleCallExpr(clang::CallExpr const*, clang::APValue&, (anonymous namespace)::LValue const*) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:8468:66
 #84 0x00007fe0c55fde76 (anonymous namespace)::ExprEvaluatorBase<(anonymous namespace)::PointerExprEvaluator>::VisitCallExpr(clang::CallExpr const*) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:8263:9
 #85 0x00007fe0c55fbe81 (anonymous namespace)::PointerExprEvaluator::visitNonBuiltinCallExpr(clang::CallExpr const*) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:9912:28
 #86 0x00007fe0c5595c62 (anonymous namespace)::PointerExprEvaluator::VisitCallExpr(clang::CallExpr const*) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:9926:5
 #87 0x00007fe0c55924fb clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::PointerExprEvaluator, bool>::Visit(clang::Stmt const*) /home/sberg/llvm/debug/build/tools/clang/include/clang/AST/StmtNodes.inc:608:1
 #88 0x00007fe0c5591785 EvaluatePointer(clang::Expr const*, (anonymous namespace)::LValue&, (anonymous namespace)::EvalInfo&, bool) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:9618:3
 #89 0x00007fe0c557c6d0 Evaluate(clang::APValue&, (anonymous namespace)::EvalInfo&, clang::Expr const*) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:16642:9
 #90 0x00007fe0c5579eed EvaluateAsRValue((anonymous namespace)::EvalInfo&, clang::Expr const*, clang::APValue&) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:16760:7
 #91 0x00007fe0c5576244 EvaluateAsRValue(clang::Expr const*, clang::Expr::EvalResult&, clang::ASTContext const&, (anonymous namespace)::EvalInfo&) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:16843:3
 #92 0x00007fe0c5575f4d clang::Expr::EvaluateAsRValue(clang::Expr::EvalResult&, clang::ASTContext const&, bool) const /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:16892:10
 #93 0x00007fe0c557998f clang::Expr::isEvaluatable(clang::ASTContext const&, clang::Expr::SideEffectsKind) const /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:17162:10
 #94 0x00007fe0d22f071b clang::CodeGen::CodeGenFunction::EmitReturnStmt(clang::ReturnStmt const&) /home/sberg/llvm/debug/llvm-project/clang/lib/CodeGen/CGStmt.cpp:1714:11
 #95 0x00007fe0d22ec83f clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef<clang::Attr const*>) /home/sberg/llvm/debug/llvm-project/clang/lib/CodeGen/CGStmt.cpp:163:75
 #96 0x00007fe0d22f69b3 clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&, bool, clang::CodeGen::AggValueSlot) /home/sberg/llvm/debug/llvm-project/clang/lib/CodeGen/CGStmt.cpp:588:3
 #97 0x00007fe0d23c6bdf clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) /home/sberg/llvm/debug/llvm-project/clang/lib/CodeGen/CodeGenFunction.cpp:1377:5
 #98 0x00007fe0d23c7d96 clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&) /home/sberg/llvm/debug/llvm-project/clang/lib/CodeGen/CodeGenFunction.cpp:1630:3
 #99 0x00007fe0d23fcf9f clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) /home/sberg/llvm/debug/llvm-project/clang/lib/CodeGen/CodeGenModule.cpp:6307:3
#100 0x00007fe0d23f398c clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) /home/sberg/llvm/debug/llvm-project/clang/lib/CodeGen/CodeGenModule.cpp:4411:12
#101 0x00007fe0d23f8d25 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) /home/sberg/llvm/debug/llvm-project/clang/lib/CodeGen/CodeGenModule.cpp:4125:5
#102 0x00007fe0d23f2128 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) /home/sberg/llvm/debug/llvm-project/clang/lib/CodeGen/CodeGenModule.cpp:7210:5
#103 0x00007fe0d25e0fdf (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) /home/sberg/llvm/debug/llvm-project/clang/lib/CodeGen/ModuleBuilder.cpp:189:20
#104 0x00007fe0d23a6ef1 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) /home/sberg/llvm/debug/llvm-project/clang/lib/CodeGen/CodeGenAction.cpp:170:8
#105 0x00007fe0c42089a3 clang::ParseAST(clang::Sema&, bool, bool) /home/sberg/llvm/debug/llvm-project/clang/lib/Parse/ParseAST.cpp:174:17
#106 0x00007fe0ce6f0cd9 clang::ASTFrontendAction::ExecuteAction() /home/sberg/llvm/debug/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1343:1
#107 0x00007fe0d23ab9ad clang::CodeGenAction::ExecuteAction() /home/sberg/llvm/debug/llvm-project/clang/lib/CodeGen/CodeGenAction.cpp:1112:5
#108 0x00007fe0ce6f0726 clang::FrontendAction::Execute() /home/sberg/llvm/debug/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1225:7
#109 0x00007fe0ce5e4163 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) /home/sberg/llvm/debug/llvm-project/clang/lib/Frontend/CompilerInstance.cpp:1055:23
#110 0x00007fe0d62914da clang::ExecuteCompilerInvocation(clang::CompilerInstance*) /home/sberg/llvm/debug/llvm-project/clang/lib/FrontendTool/ExecuteCompilerInvocation.cpp:299:8
#111 0x0000000000410127 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) /home/sberg/llvm/debug/llvm-project/clang/tools/driver/cc1_main.cpp:297:13
#112 0x00000000004033ae ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) /home/sberg/llvm/debug/llvm-project/clang/tools/driver/driver.cpp:223:5
#113 0x0000000000403e70 clang_main(int, char**, llvm::ToolContext const&)::$_0::operator()(llvm::SmallVectorImpl<char const*>&) const /home/sberg/llvm/debug/llvm-project/clang/tools/driver/driver.cpp:368:9
#114 0x0000000000403e3d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::$_0>(long, llvm::SmallVectorImpl<char const*>&) /home/sberg/llvm/debug/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#115 0x00007fe0cde9b181 llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::operator()(llvm::SmallVectorImpl<char const*>&) const /home/sberg/llvm/debug/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#116 0x00007fe0cde951b8 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::$_0::operator()() const /home/sberg/llvm/debug/llvm-project/clang/lib/Driver/Job.cpp:436:34
#117 0x00007fe0cde95185 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::$_0>(long) /home/sberg/llvm/debug/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#118 0x00007fe0c92d8a49 llvm::function_ref<void ()>::operator()() const /home/sberg/llvm/debug/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#119 0x00007fe0c92ecfaf llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) /home/sberg/llvm/debug/llvm-project/llvm/lib/Support/CrashRecoveryContext.cpp:427:3
#120 0x00007fe0cde94d23 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const /home/sberg/llvm/debug/llvm-project/clang/lib/Driver/Job.cpp:436:7
#121 0x00007fe0cde10822 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const /home/sberg/llvm/debug/llvm-project/clang/lib/Driver/Compilation.cpp:196:15
#122 0x00007fe0cde109fc clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const /home/sberg/llvm/debug/llvm-project/clang/lib/Driver/Compilation.cpp:251:13
#123 0x00007fe0cde33171 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) /home/sberg/llvm/debug/llvm-project/clang/lib/Driver/Driver.cpp:2246:7
#124 0x0000000000402ecc clang_main(int, char**, llvm::ToolContext const&) /home/sberg/llvm/debug/llvm-project/clang/tools/driver/driver.cpp:406:9
#125 0x0000000000435fb5 main /home/sberg/llvm/debug/build/tools/clang/tools/driver/clang-driver.cpp:17:3
#126 0x00007fe0cc2115f5 __libc_start_call_main (/lib64/libc.so.6+0x35f5)
#127 0x00007fe0cc2116a8 __libc_start_main@GLIBC_2.2.5 (/lib64/libc.so.6+0x36a8)
#128 0x0000000000401c45 _start (/home/sberg/llvm/debug/inst/bin/clang+++0x401c45)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
clang version 22.0.0git (git@github.com:llvm/llvm-project.git 20c8e3c2a4744524396cc473a370cfb7855850b6)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /home/sberg/llvm/debug/inst/bin
Build config: +unoptimized, +assertions
clang++: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: /tmp/test-bc64f1.cpp
clang++: note: diagnostic msg: /tmp/test-bc64f1.sh
clang++: note: diagnostic msg: 

********************
```

[test-bc64f1.cpp.txt](https://github.com/user-attachments/files/21367457/test-bc64f1.cpp.txt)
[test-bc64f1.sh.txt](https://github.com/user-attachments/files/21367458/test-bc64f1.sh.txt)

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Stephan Bergmann (stbergmann)

<details>
Bisected this to &lt;https://github.com/llvm/llvm-project/commit/20c8e3c2a4744524396cc473a370cfb7855850b6&gt; "[clang] Fix pointer comparisons between pointers to constexpr-unknown (https://github.com/llvm/llvm-project/pull/147663)" (and didn't shrink the reproducer beyond including `&lt;algorithm&gt;`, seen it reproducible with at least `libstdc++-devel-15.1.1-2.fc42.x86_64` and recent GCC trunk `libstdc++`):
```
$ cat test.cc
#include &lt;algorithm&gt;
int * f(int (&amp; c)[8])  { return std::find(c, c + 8, 0); }
```
```
$ /home/sberg/llvm/debug/inst/bin/clang++ -D_GLIBCXX_DEBUG -std=c++23 -c test.cc
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /home/sberg/llvm/debug/inst/bin/clang++ -D_GLIBCXX_DEBUG -std=c++23 -c test.cc
1.	&lt;eof&gt; parser at end of file
2.	test.cc:2:7: LLVM IR generation of declaration 'f'
3.	test.cc:2:7: Generating code for declaration 'f'
  #<!-- -->0 0x00007fe0c9458b5d llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /home/sberg/llvm/debug/llvm-project/llvm/lib/Support/Unix/Signals.inc:834:11
  #<!-- -->1 0x00007fe0c945909b PrintStackTraceSignalHandler(void*) /home/sberg/llvm/debug/llvm-project/llvm/lib/Support/Unix/Signals.inc:918:1
  #<!-- -->2 0x00007fe0c9456f9f llvm::sys::RunSignalHandlers() /home/sberg/llvm/debug/llvm-project/llvm/lib/Support/Signals.cpp:104:5
  #<!-- -->3 0x00007fe0c9458469 llvm::sys::CleanupOnSignal(unsigned long) /home/sberg/llvm/debug/llvm-project/llvm/lib/Support/Unix/Signals.inc:374:1
  #<!-- -->4 0x00007fe0c92ed192 (anonymous namespace)::CrashRecoveryContextImpl::HandleCrash(int, unsigned long) /home/sberg/llvm/debug/llvm-project/llvm/lib/Support/CrashRecoveryContext.cpp:0:7
  #<!-- -->5 0x00007fe0c92ed546 CrashRecoverySignalHandler(int) /home/sberg/llvm/debug/llvm-project/llvm/lib/Support/CrashRecoveryContext.cpp:391:1
  #<!-- -->6 0x00007fe0cc227c30 __restore_rt (/lib64/libc.so.6+0x19c30)
  #<!-- -->7 0x00007fe0c4e310ec clang::Decl::getKind() const /home/sberg/llvm/debug/llvm-project/clang/include/clang/AST/DeclBase.h:445:51
  #<!-- -->8 0x00007fe0c4e3adb5 clang::FieldDecl::classof(clang::Decl const*) /home/sberg/llvm/debug/llvm-project/clang/include/clang/AST/Decl.h:3379:62
  #<!-- -->9 0x00007fe0c4e3ad95 llvm::isa_impl&lt;clang::FieldDecl, clang::Decl, void&gt;::doit(clang::Decl const&amp;) /home/sberg/llvm/debug/llvm-project/llvm/include/llvm/Support/Casting.h:64:46
 #<!-- -->10 0x00007fe0c4e3ad72 llvm::isa_impl_cl&lt;clang::FieldDecl, clang::Decl const*&gt;::doit(clang::Decl const*) /home/sberg/llvm/debug/llvm-project/llvm/include/llvm/Support/Casting.h:110:5
 #<!-- -->11 0x00007fe0c4e3ad18 llvm::isa_impl_wrap&lt;clang::FieldDecl, clang::Decl const*, clang::Decl const*&gt;::doit(clang::Decl const* const&amp;) /home/sberg/llvm/debug/llvm-project/llvm/include/llvm/Support/Casting.h:137:5
 #<!-- -->12 0x00007fe0c4e3acf2 llvm::isa_impl_wrap&lt;clang::FieldDecl, clang::Decl const* const, clang::Decl const*&gt;::doit(clang::Decl const* const&amp;) /home/sberg/llvm/debug/llvm-project/llvm/include/llvm/Support/Casting.h:127:5
 #<!-- -->13 0x00007fe0c4e3acc5 llvm::CastIsPossible&lt;clang::FieldDecl, clang::Decl const*, void&gt;::isPossible(clang::Decl const* const&amp;) /home/sberg/llvm/debug/llvm-project/llvm/include/llvm/Support/Casting.h:255:5
 #<!-- -->14 0x00007fe0c4ee5115 llvm::CastInfo&lt;clang::FieldDecl, clang::Decl const*, void&gt;::doCastIfPossible(clang::Decl const* const&amp;) /home/sberg/llvm/debug/llvm-project/llvm/include/llvm/Support/Casting.h:493:9
 #<!-- -->15 0x00007fe0c5317e75 auto llvm::dyn_cast_if_present&lt;clang::FieldDecl, clang::Decl const&gt;(clang::Decl const*) /home/sberg/llvm/debug/llvm-project/llvm/include/llvm/Support/Casting.h:753:3
 #<!-- -->16 0x00007fe0c563e735 auto llvm::dyn_cast_or_null&lt;clang::FieldDecl, clang::Decl const&gt;(clang::Decl const*) /home/sberg/llvm/debug/llvm-project/llvm/include/llvm/Support/Casting.h:768:3
 #<!-- -->17 0x00007fe0c558c38a (anonymous namespace)::getAsField(clang::APValue::LValuePathEntry) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:96:5
 #<!-- -->18 0x00007fe0c55f59ce bool EvaluateComparisonBinaryOperator&lt;(anonymous namespace)::IntExprEvaluator::VisitBinaryOperator(clang::BinaryOperator const*)::$_0&amp;, (anonymous namespace)::IntExprEvaluator::VisitBinaryOperator(clang::BinaryOperator const*)::$_1&gt;((anonymous namespace)::EvalInfo&amp;, clang::BinaryOperator const*, (anonymous namespace)::IntExprEvaluator::VisitBinaryOperator(clang::BinaryOperator const*)::$_0&amp;, (anonymous namespace)::IntExprEvaluator::VisitBinaryOperator(clang::BinaryOperator const*)::$_1&amp;&amp;) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:14576:26
 #<!-- -->19 0x00007fe0c55a56b4 (anonymous namespace)::IntExprEvaluator::VisitBinaryOperator(clang::BinaryOperator const*) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:14780:5
 #<!-- -->20 0x00007fe0c55a197d clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool&gt;::VisitBinLE(clang::BinaryOperator const*) /home/sberg/llvm/debug/llvm-project/clang/include/clang/AST/StmtVisitor.h:133:46
 #<!-- -->21 0x00007fe0c559fc76 clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool&gt;::Visit(clang::Stmt const*) /home/sberg/llvm/debug/llvm-project/clang/include/clang/AST/StmtVisitor.h:63:26
 #<!-- -->22 0x00007fe0c557c681 Evaluate(clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Expr const*) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:16638:9
 #<!-- -->23 0x00007fe0c55f7c6c (anonymous namespace)::DataRecursiveIntBinOpEvaluator::EvaluateExpr(clang::Expr const*, (anonymous namespace)::DataRecursiveIntBinOpEvaluator::EvalResult&amp;) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:14117:22
 #<!-- -->24 0x00007fe0c55f70ed (anonymous namespace)::DataRecursiveIntBinOpEvaluator::process((anonymous namespace)::DataRecursiveIntBinOpEvaluator::EvalResult&amp;) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:14299:7
 #<!-- -->25 0x00007fe0c55f4818 (anonymous namespace)::DataRecursiveIntBinOpEvaluator::Traverse(clang::BinaryOperator const*) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:14082:5
 #<!-- -->26 0x00007fe0c55a55b1 (anonymous namespace)::IntExprEvaluator::VisitBinaryOperator(clang::BinaryOperator const*) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:14748:5
 #<!-- -->27 0x00007fe0c55a1afd clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool&gt;::VisitBinLAnd(clang::BinaryOperator const*) /home/sberg/llvm/debug/llvm-project/clang/include/clang/AST/StmtVisitor.h:138:3
 #<!-- -->28 0x00007fe0c559fd2e clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool&gt;::Visit(clang::Stmt const*) /home/sberg/llvm/debug/llvm-project/clang/include/clang/AST/StmtVisitor.h:72:26
 #<!-- -->29 0x00007fe0c557c681 Evaluate(clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Expr const*) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:16638:9
 #<!-- -->30 0x00007fe0c5599eda EvaluateStmt((anonymous namespace)::StmtResult&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Stmt const*, clang::SwitchCase const*) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:5618:17
 #<!-- -->31 0x00007fe0c5599fd4 EvaluateStmt((anonymous namespace)::StmtResult&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Stmt const*, clang::SwitchCase const*) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:5631:22
 #<!-- -->32 0x00007fe0c557d76d HandleFunctionCall(clang::SourceLocation, clang::FunctionDecl const*, (anonymous namespace)::LValue const*, clang::Expr const*, llvm::ArrayRef&lt;clang::Expr const*&gt;, (anonymous namespace)::CallRef, clang::Stmt const*, (anonymous namespace)::EvalInfo&amp;, clang::APValue&amp;, (anonymous namespace)::LValue const*) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:6663:18
 #<!-- -->33 0x00007fe0c55f2153 (anonymous namespace)::ExprEvaluatorBase&lt;(anonymous namespace)::IntExprEvaluator&gt;::handleCallExpr(clang::CallExpr const*, clang::APValue&amp;, (anonymous namespace)::LValue const*) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:8468:66
 #<!-- -->34 0x00007fe0c55eb9a6 (anonymous namespace)::ExprEvaluatorBase&lt;(anonymous namespace)::IntExprEvaluator&gt;::VisitCallExpr(clang::CallExpr const*) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:8263:9
 #<!-- -->35 0x00007fe0c55a4e92 (anonymous namespace)::IntExprEvaluator::VisitCallExpr(clang::CallExpr const*) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:12933:5
 #<!-- -->36 0x00007fe0c55a082b clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool&gt;::Visit(clang::Stmt const*) /home/sberg/llvm/debug/build/tools/clang/include/clang/AST/StmtNodes.inc:608:1
 #<!-- -->37 0x00007fe0c557c681 Evaluate(clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Expr const*) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:16638:9
 #<!-- -->38 0x00007fe0c5599eda EvaluateStmt((anonymous namespace)::StmtResult&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Stmt const*, clang::SwitchCase const*) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:5618:17
 #<!-- -->39 0x00007fe0c5599fd4 EvaluateStmt((anonymous namespace)::StmtResult&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Stmt const*, clang::SwitchCase const*) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:5631:22
 #<!-- -->40 0x00007fe0c557d76d HandleFunctionCall(clang::SourceLocation, clang::FunctionDecl const*, (anonymous namespace)::LValue const*, clang::Expr const*, llvm::ArrayRef&lt;clang::Expr const*&gt;, (anonymous namespace)::CallRef, clang::Stmt const*, (anonymous namespace)::EvalInfo&amp;, clang::APValue&amp;, (anonymous namespace)::LValue const*) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:6663:18
 #<!-- -->41 0x00007fe0c55f2153 (anonymous namespace)::ExprEvaluatorBase&lt;(anonymous namespace)::IntExprEvaluator&gt;::handleCallExpr(clang::CallExpr const*, clang::APValue&amp;, (anonymous namespace)::LValue const*) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:8468:66
 #<!-- -->42 0x00007fe0c55eb9a6 (anonymous namespace)::ExprEvaluatorBase&lt;(anonymous namespace)::IntExprEvaluator&gt;::VisitCallExpr(clang::CallExpr const*) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:8263:9
 #<!-- -->43 0x00007fe0c55a4e92 (anonymous namespace)::IntExprEvaluator::VisitCallExpr(clang::CallExpr const*) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:12933:5
 #<!-- -->44 0x00007fe0c55a082b clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool&gt;::Visit(clang::Stmt const*) /home/sberg/llvm/debug/build/tools/clang/include/clang/AST/StmtNodes.inc:608:1
 #<!-- -->45 0x00007fe0c557c681 Evaluate(clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Expr const*) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:16638:9
 #<!-- -->46 0x00007fe0c5599eda EvaluateStmt((anonymous namespace)::StmtResult&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Stmt const*, clang::SwitchCase const*) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:5618:17
 #<!-- -->47 0x00007fe0c5599fd4 EvaluateStmt((anonymous namespace)::StmtResult&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Stmt const*, clang::SwitchCase const*) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:5631:22
 #<!-- -->48 0x00007fe0c557d76d HandleFunctionCall(clang::SourceLocation, clang::FunctionDecl const*, (anonymous namespace)::LValue const*, clang::Expr const*, llvm::ArrayRef&lt;clang::Expr const*&gt;, (anonymous namespace)::CallRef, clang::Stmt const*, (anonymous namespace)::EvalInfo&amp;, clang::APValue&amp;, (anonymous namespace)::LValue const*) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:6663:18
 #<!-- -->49 0x00007fe0c55f2153 (anonymous namespace)::ExprEvaluatorBase&lt;(anonymous namespace)::IntExprEvaluator&gt;::handleCallExpr(clang::CallExpr const*, clang::APValue&amp;, (anonymous namespace)::LValue const*) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:8468:66
 #<!-- -->50 0x00007fe0c55eb9a6 (anonymous namespace)::ExprEvaluatorBase&lt;(anonymous namespace)::IntExprEvaluator&gt;::VisitCallExpr(clang::CallExpr const*) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:8263:9
 #<!-- -->51 0x00007fe0c55a4e92 (anonymous namespace)::IntExprEvaluator::VisitCallExpr(clang::CallExpr const*) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:12933:5
 #<!-- -->52 0x00007fe0c55a082b clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool&gt;::Visit(clang::Stmt const*) /home/sberg/llvm/debug/build/tools/clang/include/clang/AST/StmtNodes.inc:608:1
 #<!-- -->53 0x00007fe0c55eb5ed (anonymous namespace)::ExprEvaluatorBase&lt;(anonymous namespace)::IntExprEvaluator&gt;::VisitCastExpr(clang::CastExpr const*) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:8573:7
 #<!-- -->54 0x00007fe0c55ea8d9 (anonymous namespace)::IntExprEvaluator::VisitCastExpr(clang::CastExpr const*) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:15185:5
 #<!-- -->55 0x00007fe0c55eb8cd clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool&gt;::VisitExplicitCastExpr(clang::ExplicitCastExpr const*) /home/sberg/llvm/debug/build/tools/clang/include/clang/AST/StmtNodes.inc:534:1
 #<!-- -->56 0x00007fe0c55a4ded clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool&gt;::VisitCXXFunctionalCastExpr(clang::CXXFunctionalCastExpr const*) /home/sberg/llvm/debug/build/tools/clang/include/clang/AST/StmtNodes.inc:582:1
 #<!-- -->57 0x00007fe0c55a07e6 clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool&gt;::Visit(clang::Stmt const*) /home/sberg/llvm/debug/build/tools/clang/include/clang/AST/StmtNodes.inc:582:1
 #<!-- -->58 0x00007fe0c557c681 Evaluate(clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Expr const*) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:16638:9
 #<!-- -->59 0x00007fe0c559ec78 EvaluateAsBooleanCondition(clang::Expr const*, bool&amp;, (anonymous namespace)::EvalInfo&amp;) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:2769:7
 #<!-- -->60 0x00007fe0c55a26cc (anonymous namespace)::IntExprEvaluator::VisitUnaryOperator(clang::UnaryOperator const*) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:15111:9
 #<!-- -->61 0x00007fe0c55a1f5d clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool&gt;::VisitUnaryLNot(clang::UnaryOperator const*) /home/sberg/llvm/debug/llvm-project/clang/include/clang/AST/StmtVisitor.h:167:31
 #<!-- -->62 0x00007fe0c559ff88 clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool&gt;::Visit(clang::Stmt const*) /home/sberg/llvm/debug/llvm-project/clang/include/clang/AST/StmtVisitor.h:98:26
 #<!-- -->63 0x00007fe0c55eab06 (anonymous namespace)::IntExprEvaluator::VisitCastExpr(clang::CastExpr const*) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:15225:9
 #<!-- -->64 0x00007fe0c55a4c1d clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool&gt;::VisitImplicitCastExpr(clang::ImplicitCastExpr const*) /home/sberg/llvm/debug/build/tools/clang/include/clang/AST/StmtNodes.inc:528:1
 #<!-- -->65 0x00007fe0c55a0745 clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool&gt;::Visit(clang::Stmt const*) /home/sberg/llvm/debug/build/tools/clang/include/clang/AST/StmtNodes.inc:528:1
 #<!-- -->66 0x00007fe0c55ed562 (anonymous namespace)::IntExprEvaluator::VisitBuiltinCallExpr(clang::CallExpr const*, unsigned int) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:13217:5
 #<!-- -->67 0x00007fe0c55a4eb9 (anonymous namespace)::IntExprEvaluator::VisitCallExpr(clang::CallExpr const*) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:12934:3
 #<!-- -->68 0x00007fe0c55a082b clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool&gt;::Visit(clang::Stmt const*) /home/sberg/llvm/debug/build/tools/clang/include/clang/AST/StmtNodes.inc:608:1
 #<!-- -->69 0x00007fe0c557c681 Evaluate(clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Expr const*) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:16638:9
 #<!-- -->70 0x00007fe0c559ec78 EvaluateAsBooleanCondition(clang::Expr const*, bool&amp;, (anonymous namespace)::EvalInfo&amp;) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:2769:7
 #<!-- -->71 0x00007fe0c55ea8fa (anonymous namespace)::IntExprEvaluator::VisitCastExpr(clang::CastExpr const*) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:15195:9
 #<!-- -->72 0x00007fe0c55a4c1d clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool&gt;::VisitImplicitCastExpr(clang::ImplicitCastExpr const*) /home/sberg/llvm/debug/build/tools/clang/include/clang/AST/StmtNodes.inc:528:1
 #<!-- -->73 0x00007fe0c55a0745 clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool&gt;::Visit(clang::Stmt const*) /home/sberg/llvm/debug/build/tools/clang/include/clang/AST/StmtNodes.inc:528:1
 #<!-- -->74 0x00007fe0c557c681 Evaluate(clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Expr const*) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:16638:9
 #<!-- -->75 0x00007fe0c559ec78 EvaluateAsBooleanCondition(clang::Expr const*, bool&amp;, (anonymous namespace)::EvalInfo&amp;) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:2769:7
 #<!-- -->76 0x00007fe0c559eb65 EvaluateCond((anonymous namespace)::EvalInfo&amp;, clang::VarDecl const*, clang::Expr const*, bool&amp;) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:5286:7
 #<!-- -->77 0x00007fe0c559a1d8 EvaluateStmt((anonymous namespace)::StmtResult&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Stmt const*, clang::SwitchCase const*) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:5665:16
 #<!-- -->78 0x00007fe0c5599fd4 EvaluateStmt((anonymous namespace)::StmtResult&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Stmt const*, clang::SwitchCase const*) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:5631:22
 #<!-- -->79 0x00007fe0c559e58a EvaluateLoopBody((anonymous namespace)::StmtResult&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Stmt const*, clang::SwitchCase const*) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:5327:18
 #<!-- -->80 0x00007fe0c559a4b3 EvaluateStmt((anonymous namespace)::StmtResult&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Stmt const*, clang::SwitchCase const*) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:5707:22
 #<!-- -->81 0x00007fe0c5599fd4 EvaluateStmt((anonymous namespace)::StmtResult&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Stmt const*, clang::SwitchCase const*) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:5631:22
 #<!-- -->82 0x00007fe0c557d76d HandleFunctionCall(clang::SourceLocation, clang::FunctionDecl const*, (anonymous namespace)::LValue const*, clang::Expr const*, llvm::ArrayRef&lt;clang::Expr const*&gt;, (anonymous namespace)::CallRef, clang::Stmt const*, (anonymous namespace)::EvalInfo&amp;, clang::APValue&amp;, (anonymous namespace)::LValue const*) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:6663:18
 #<!-- -->83 0x00007fe0c55fef83 (anonymous namespace)::ExprEvaluatorBase&lt;(anonymous namespace)::PointerExprEvaluator&gt;::handleCallExpr(clang::CallExpr const*, clang::APValue&amp;, (anonymous namespace)::LValue const*) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:8468:66
 #<!-- -->84 0x00007fe0c55fde76 (anonymous namespace)::ExprEvaluatorBase&lt;(anonymous namespace)::PointerExprEvaluator&gt;::VisitCallExpr(clang::CallExpr const*) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:8263:9
 #<!-- -->85 0x00007fe0c55fbe81 (anonymous namespace)::PointerExprEvaluator::visitNonBuiltinCallExpr(clang::CallExpr const*) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:9912:28
 #<!-- -->86 0x00007fe0c5595c62 (anonymous namespace)::PointerExprEvaluator::VisitCallExpr(clang::CallExpr const*) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:9926:5
 #<!-- -->87 0x00007fe0c55924fb clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::PointerExprEvaluator, bool&gt;::Visit(clang::Stmt const*) /home/sberg/llvm/debug/build/tools/clang/include/clang/AST/StmtNodes.inc:608:1
 #<!-- -->88 0x00007fe0c5591785 EvaluatePointer(clang::Expr const*, (anonymous namespace)::LValue&amp;, (anonymous namespace)::EvalInfo&amp;, bool) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:9618:3
 #<!-- -->89 0x00007fe0c557c6d0 Evaluate(clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Expr const*) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:16642:9
 #<!-- -->90 0x00007fe0c5579eed EvaluateAsRValue((anonymous namespace)::EvalInfo&amp;, clang::Expr const*, clang::APValue&amp;) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:16760:7
 #<!-- -->91 0x00007fe0c5576244 EvaluateAsRValue(clang::Expr const*, clang::Expr::EvalResult&amp;, clang::ASTContext const&amp;, (anonymous namespace)::EvalInfo&amp;) /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:16843:3
 #<!-- -->92 0x00007fe0c5575f4d clang::Expr::EvaluateAsRValue(clang::Expr::EvalResult&amp;, clang::ASTContext const&amp;, bool) const /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:16892:10
 #<!-- -->93 0x00007fe0c557998f clang::Expr::isEvaluatable(clang::ASTContext const&amp;, clang::Expr::SideEffectsKind) const /home/sberg/llvm/debug/llvm-project/clang/lib/AST/ExprConstant.cpp:17162:10
 #<!-- -->94 0x00007fe0d22f071b clang::CodeGen::CodeGenFunction::EmitReturnStmt(clang::ReturnStmt const&amp;) /home/sberg/llvm/debug/llvm-project/clang/lib/CodeGen/CGStmt.cpp:1714:11
 #<!-- -->95 0x00007fe0d22ec83f clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef&lt;clang::Attr const*&gt;) /home/sberg/llvm/debug/llvm-project/clang/lib/CodeGen/CGStmt.cpp:163:75
 #<!-- -->96 0x00007fe0d22f69b3 clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&amp;, bool, clang::CodeGen::AggValueSlot) /home/sberg/llvm/debug/llvm-project/clang/lib/CodeGen/CGStmt.cpp:588:3
 #<!-- -->97 0x00007fe0d23c6bdf clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) /home/sberg/llvm/debug/llvm-project/clang/lib/CodeGen/CodeGenFunction.cpp:1377:5
 #<!-- -->98 0x00007fe0d23c7d96 clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&amp;) /home/sberg/llvm/debug/llvm-project/clang/lib/CodeGen/CodeGenFunction.cpp:1630:3
 #<!-- -->99 0x00007fe0d23fcf9f clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) /home/sberg/llvm/debug/llvm-project/clang/lib/CodeGen/CodeGenModule.cpp:6307:3
#<!-- -->100 0x00007fe0d23f398c clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) /home/sberg/llvm/debug/llvm-project/clang/lib/CodeGen/CodeGenModule.cpp:4411:12
#<!-- -->101 0x00007fe0d23f8d25 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) /home/sberg/llvm/debug/llvm-project/clang/lib/CodeGen/CodeGenModule.cpp:4125:5
#<!-- -->102 0x00007fe0d23f2128 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) /home/sberg/llvm/debug/llvm-project/clang/lib/CodeGen/CodeGenModule.cpp:7210:5
#<!-- -->103 0x00007fe0d25e0fdf (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) /home/sberg/llvm/debug/llvm-project/clang/lib/CodeGen/ModuleBuilder.cpp:189:20
#<!-- -->104 0x00007fe0d23a6ef1 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) /home/sberg/llvm/debug/llvm-project/clang/lib/CodeGen/CodeGenAction.cpp:170:8
#<!-- -->105 0x00007fe0c42089a3 clang::ParseAST(clang::Sema&amp;, bool, bool) /home/sberg/llvm/debug/llvm-project/clang/lib/Parse/ParseAST.cpp:174:17
#<!-- -->106 0x00007fe0ce6f0cd9 clang::ASTFrontendAction::ExecuteAction() /home/sberg/llvm/debug/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1343:1
#<!-- -->107 0x00007fe0d23ab9ad clang::CodeGenAction::ExecuteAction() /home/sberg/llvm/debug/llvm-project/clang/lib/CodeGen/CodeGenAction.cpp:1112:5
#<!-- -->108 0x00007fe0ce6f0726 clang::FrontendAction::Execute() /home/sberg/llvm/debug/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1225:7
#<!-- -->109 0x00007fe0ce5e4163 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) /home/sberg/llvm/debug/llvm-project/clang/lib/Frontend/CompilerInstance.cpp:1055:23
#<!-- -->110 0x00007fe0d62914da clang::ExecuteCompilerInvocation(clang::CompilerInstance*) /home/sberg/llvm/debug/llvm-project/clang/lib/FrontendTool/ExecuteCompilerInvocation.cpp:299:8
#<!-- -->111 0x0000000000410127 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) /home/sberg/llvm/debug/llvm-project/clang/tools/driver/cc1_main.cpp:297:13
#<!-- -->112 0x00000000004033ae ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) /home/sberg/llvm/debug/llvm-project/clang/tools/driver/driver.cpp:223:5
#<!-- -->113 0x0000000000403e70 clang_main(int, char**, llvm::ToolContext const&amp;)::$_0::operator()(llvm::SmallVectorImpl&lt;char const*&gt;&amp;) const /home/sberg/llvm/debug/llvm-project/clang/tools/driver/driver.cpp:368:9
#<!-- -->114 0x0000000000403e3d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::$_0&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) /home/sberg/llvm/debug/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->115 0x00007fe0cde9b181 llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::operator()(llvm::SmallVectorImpl&lt;char const*&gt;&amp;) const /home/sberg/llvm/debug/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->116 0x00007fe0cde951b8 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::$_0::operator()() const /home/sberg/llvm/debug/llvm-project/clang/lib/Driver/Job.cpp:436:34
#<!-- -->117 0x00007fe0cde95185 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::$_0&gt;(long) /home/sberg/llvm/debug/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->118 0x00007fe0c92d8a49 llvm::function_ref&lt;void ()&gt;::operator()() const /home/sberg/llvm/debug/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->119 0x00007fe0c92ecfaf llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) /home/sberg/llvm/debug/llvm-project/llvm/lib/Support/CrashRecoveryContext.cpp:427:3
#<!-- -->120 0x00007fe0cde94d23 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const /home/sberg/llvm/debug/llvm-project/clang/lib/Driver/Job.cpp:436:7
#<!-- -->121 0x00007fe0cde10822 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const /home/sberg/llvm/debug/llvm-project/clang/lib/Driver/Compilation.cpp:196:15
#<!-- -->122 0x00007fe0cde109fc clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const /home/sberg/llvm/debug/llvm-project/clang/lib/Driver/Compilation.cpp:251:13
#<!-- -->123 0x00007fe0cde33171 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) /home/sberg/llvm/debug/llvm-project/clang/lib/Driver/Driver.cpp:2246:7
#<!-- -->124 0x0000000000402ecc clang_main(int, char**, llvm::ToolContext const&amp;) /home/sberg/llvm/debug/llvm-project/clang/tools/driver/driver.cpp:406:9
#<!-- -->125 0x0000000000435fb5 main /home/sberg/llvm/debug/build/tools/clang/tools/driver/clang-driver.cpp:17:3
#<!-- -->126 0x00007fe0cc2115f5 __libc_start_call_main (/lib64/libc.so.6+0x35f5)
#<!-- -->127 0x00007fe0cc2116a8 __libc_start_main@<!-- -->GLIBC_2.2.5 (/lib64/libc.so.6+0x36a8)
#<!-- -->128 0x0000000000401c45 _start (/home/sberg/llvm/debug/inst/bin/clang+++0x401c45)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
clang version 22.0.0git (git@<!-- -->github.com:llvm/llvm-project.git 20c8e3c2a4744524396cc473a370cfb7855850b6)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /home/sberg/llvm/debug/inst/bin
Build config: +unoptimized, +assertions
clang++: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: /tmp/test-bc64f1.cpp
clang++: note: diagnostic msg: /tmp/test-bc64f1.sh
clang++: note: diagnostic msg: 

********************
```

[test-bc64f1.cpp.txt](https://github.com/user-attachments/files/21367457/test-bc64f1.cpp.txt)
[test-bc64f1.sh.txt](https://github.com/user-attachments/files/21367458/test-bc64f1.sh.txt)
</details>


---

### Comment 2 - zwuis

CC @efriedma-quic 

---

### Comment 3 - efriedma-quic

Reduced:

```
extern int (& c)[8];
constexpr int x = c <= c+8;
```

---

### Comment 4 - efriedma-quic

#150088 should fix this.

---

### Comment 5 - efriedma-quic

/cherry-pick bba846773c7dfce0f95b8846672d8dd5fa8912be

---

### Comment 6 - llvmbot

<!--IGNORE-->
Failed to cherry-pick: bba846773c7dfce0f95b8846672d8dd5fa8912be

https://github.com/llvm/llvm-project/actions/runs/16507332100

Please manually backport the fix and push it to your github fork.  Once this is done, please create a [pull request](https://github.com/llvm/llvm-project/compare)

---

### Comment 7 - efriedma-quic

Cherry-pick depends on #149402 .  I guess I'll wait for that to get merged.

---

### Comment 8 - efriedma-quic

/cherry-pick bba846773c7dfce0f95b8846672d8dd5fa8912be

---

### Comment 9 - llvmbot

/pull-request llvm/llvm-project#150981

---

