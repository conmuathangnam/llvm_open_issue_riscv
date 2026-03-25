# ICE / LLVM ERROR: Out of memory when compiling large std::array<shared_future<...>> with additional unexpected behaviours (C++23, Clang 19–21)

**Author:** mariete1223
**URL:** https://github.com/llvm/llvm-project/issues/138653
**Status:** Closed
**Labels:** clang:frontend, confirmed, crash, c++23
**Closed Date:** 2025-05-09T07:03:21Z

## Body

## Summary

LLVM ERROR: out of memory strange error, it occurs with std=c++23 and versions 19, 20 and current, it has been found that in current version if you remove ```#include <thread>``` the compiler does not crashes but in version 20 and 19 it does. 

## Assertion

No assertion given

## Stack dump

```
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x c++ -std=c++23 <source>
1.	<source>:9:81: current parser token ';'
2.	<source>:8:12: parsing function body 'main'
3.	<source>:8:12: in compound statement ('{}')
 #0 0x0000000003f70b58 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f70b58)
 #1 0x0000000003f6e7e4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f6e7e4)
 #2 0x0000000003eb36a8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007d9110042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007d91100969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007d9110042476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007d91100287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x0000000003ebeeab (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3ebeeab)
 #8 0x0000000003ebeec5 (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3ebeec5)
 #9 0x00007d91104ae9a3 operator new(unsigned long) (/lib/x86_64-linux-gnu/libstdc++.so.6+0xae9a3)
#10 0x00000000074b318c clang::APValue::MakeArray(unsigned int, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x74b318c)
#11 0x000000000788d422 (anonymous namespace)::ArrayExprEvaluator::VisitCXXParenListOrInitListExpr(clang::Expr const*, llvm::ArrayRef<clang::Expr*>, clang::Expr const*, clang::QualType) ExprConstant.cpp:0:0
#12 0x00000000078c4faf clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::ArrayExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#13 0x00000000078c65cd EvaluateArray(clang::Expr const*, (anonymous namespace)::LValue const&, clang::APValue&, (anonymous namespace)::EvalInfo&) ExprConstant.cpp:0:0
#14 0x000000000788bd11 EvaluateInPlace(clang::APValue&, (anonymous namespace)::EvalInfo&, (anonymous namespace)::LValue const&, clang::Expr const*, bool) ExprConstant.cpp:0:0
#15 0x000000000788ed06 (anonymous namespace)::RecordExprEvaluator::VisitCXXParenListOrInitListExpr(clang::Expr const*, llvm::ArrayRef<clang::Expr*>) ExprConstant.cpp:0:0
#16 0x00000000078c94bf clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::RecordExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#17 0x00000000078ca038 EvaluateRecord(clang::Expr const*, (anonymous namespace)::LValue const&, clang::APValue&, (anonymous namespace)::EvalInfo&) ExprConstant.cpp:0:0
#18 0x000000000788bd81 EvaluateInPlace(clang::APValue&, (anonymous namespace)::EvalInfo&, (anonymous namespace)::LValue const&, clang::Expr const*, bool) ExprConstant.cpp:0:0
#19 0x00000000078cf690 clang::Expr::EvaluateAsInitializer(clang::APValue&, clang::ASTContext const&, clang::VarDecl const*, llvm::SmallVectorImpl<std::pair<clang::SourceLocation, clang::PartialDiagnostic>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x78cf690)
#20 0x00000000076ecf42 clang::VarDecl::evaluateValueImpl(llvm::SmallVectorImpl<std::pair<clang::SourceLocation, clang::PartialDiagnostic>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x76ecf42)
#21 0x00000000076ed0ba clang::VarDecl::evaluateValue() const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x76ed0ba)
#22 0x0000000006a256d8 clang::Sema::FinalizeVarWithDestructor(clang::VarDecl*, clang::RecordType const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a256d8)
#23 0x000000000698dd5b clang::Sema::CheckCompleteVariableDeclaration(clang::VarDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x698dd5b)
#24 0x00000000069957b2 clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69957b2)
#25 0x00000000066189e5 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66189e5)
#26 0x00000000066286e9 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66286e9)
#27 0x00000000066313a7 clang::Parser::ParseSimpleDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, bool, clang::Parser::ForRangeInit*, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66313a7)
#28 0x0000000006631869 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6631869)
#29 0x00000000066e1332 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66e1332)
#30 0x00000000066e203d clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66e203d)
#31 0x00000000066e9d53 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66e9d53)
#32 0x00000000066ea51a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66ea51a)
#33 0x00000000065f2a43 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65f2a43)
#34 0x00000000066290ed clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66290ed)
#35 0x00000000065e657e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65e657e)
#36 0x00000000065e6d39 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65e6d39)
#37 0x00000000065ee65a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65ee65a)
#38 0x00000000065ef5fd clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65ef5fd)
#39 0x00000000065e19da clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65e19da)
#40 0x0000000004912908 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4912908)
#41 0x0000000004c05bb5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c05bb5)
#42 0x0000000004b851be clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b851be)
#43 0x0000000004cf9a09 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4cf9a09)
#44 0x0000000000da625f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda625f)
#45 0x0000000000d9c42a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#46 0x00000000049791b9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#47 0x0000000003eb3b44 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3eb3b44)
#48 0x00000000049797cf clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#49 0x000000000493bc1d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x493bc1d)
#50 0x000000000493ccae clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x493ccae)
#51 0x0000000004944b05 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4944b05)
#52 0x0000000000da21c8 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda21c8)
#53 0x0000000000c27864 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc27864)
#54 0x00007d9110029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#55 0x00007d9110029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#56 0x0000000000d9bed5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd9bed5)
```

## Program

```c++
#include <thread>
#include <future>
#include <memory>
using namespace std;
constexpr size_t kMemoryChunk = 1024 * 8;
constexpr size_t kNumberOfIterations = 2000000;
constexpr size_t kThreadsNumber = 2 * kMemoryChunk;
int main() {
    array<shared_future<short*>, kThreadsNumber * kNumberOfIterations> futures{};
}
```

## Gcc Bolt

Here it can be seen stranger behaviour when include is removed.

https://gcc.godbolt.org/z/af8eKT3ax

Here in current version with include 

https://gcc.godbolt.org/z/zv6b6vWs1

## Additional question

I am also wondering if LLVM ERROR: out of memory programs are worth submiting an issue, thanks in advance




## Comments

### Comment 1 - tbaederr

We should probably do

```diff
diff --git i/clang/lib/AST/ExprConstant.cpp w/clang/lib/AST/ExprConstant.cpp
index e5950f461e4b..2e6074d125f4 100644
--- i/clang/lib/AST/ExprConstant.cpp
+++ w/clang/lib/AST/ExprConstant.cpp
@@ -11788,6 +11788,12 @@ bool ArrayExprEvaluator::VisitCXXParenListOrInitListExpr(
   LLVM_DEBUG(llvm::dbgs() << "The number of elements to initialize: "
                           << NumEltsToInit << ".\n");

+  if (!Info.CheckArraySize(
+      ExprToVisit->getExprLoc(),
+      CAT->getNumAddressingBits(Info.Ctx), NumEltsToInit,
+      /*Diag=*/true))
+    return false;
+
   Result = APValue(APValue::UninitArray(), NumEltsToInit, NumElts);

   // If the array was previously zero-initialized, preserve the
```
to check for the array size.

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (mariete1223)

<details>
## Summary

LLVM ERROR: out of memory strange error, it occurs with std=c++23 and versions 19, 20 and current, it has been found that in current version if you remove ```#include &lt;thread&gt;``` the compiler does not crashes but in version 20 and 19 it does. 

## Assertion

No assertion given

## Stack dump

```
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x c++ -std=c++23 &lt;source&gt;
1.	&lt;source&gt;:9:81: current parser token ';'
2.	&lt;source&gt;:8:12: parsing function body 'main'
3.	&lt;source&gt;:8:12: in compound statement ('{}')
 #<!-- -->0 0x0000000003f70b58 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f70b58)
 #<!-- -->1 0x0000000003f6e7e4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f6e7e4)
 #<!-- -->2 0x0000000003eb36a8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007d9110042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007d91100969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007d9110042476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007d91100287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x0000000003ebeeab (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3ebeeab)
 #<!-- -->8 0x0000000003ebeec5 (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3ebeec5)
 #<!-- -->9 0x00007d91104ae9a3 operator new(unsigned long) (/lib/x86_64-linux-gnu/libstdc++.so.6+0xae9a3)
#<!-- -->10 0x00000000074b318c clang::APValue::MakeArray(unsigned int, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x74b318c)
#<!-- -->11 0x000000000788d422 (anonymous namespace)::ArrayExprEvaluator::VisitCXXParenListOrInitListExpr(clang::Expr const*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::Expr const*, clang::QualType) ExprConstant.cpp:0:0
#<!-- -->12 0x00000000078c4faf clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::ArrayExprEvaluator, bool&gt;::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#<!-- -->13 0x00000000078c65cd EvaluateArray(clang::Expr const*, (anonymous namespace)::LValue const&amp;, clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;) ExprConstant.cpp:0:0
#<!-- -->14 0x000000000788bd11 EvaluateInPlace(clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;, (anonymous namespace)::LValue const&amp;, clang::Expr const*, bool) ExprConstant.cpp:0:0
#<!-- -->15 0x000000000788ed06 (anonymous namespace)::RecordExprEvaluator::VisitCXXParenListOrInitListExpr(clang::Expr const*, llvm::ArrayRef&lt;clang::Expr*&gt;) ExprConstant.cpp:0:0
#<!-- -->16 0x00000000078c94bf clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::RecordExprEvaluator, bool&gt;::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#<!-- -->17 0x00000000078ca038 EvaluateRecord(clang::Expr const*, (anonymous namespace)::LValue const&amp;, clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;) ExprConstant.cpp:0:0
#<!-- -->18 0x000000000788bd81 EvaluateInPlace(clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;, (anonymous namespace)::LValue const&amp;, clang::Expr const*, bool) ExprConstant.cpp:0:0
#<!-- -->19 0x00000000078cf690 clang::Expr::EvaluateAsInitializer(clang::APValue&amp;, clang::ASTContext const&amp;, clang::VarDecl const*, llvm::SmallVectorImpl&lt;std::pair&lt;clang::SourceLocation, clang::PartialDiagnostic&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x78cf690)
#<!-- -->20 0x00000000076ecf42 clang::VarDecl::evaluateValueImpl(llvm::SmallVectorImpl&lt;std::pair&lt;clang::SourceLocation, clang::PartialDiagnostic&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x76ecf42)
#<!-- -->21 0x00000000076ed0ba clang::VarDecl::evaluateValue() const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x76ed0ba)
#<!-- -->22 0x0000000006a256d8 clang::Sema::FinalizeVarWithDestructor(clang::VarDecl*, clang::RecordType const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a256d8)
#<!-- -->23 0x000000000698dd5b clang::Sema::CheckCompleteVariableDeclaration(clang::VarDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x698dd5b)
#<!-- -->24 0x00000000069957b2 clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69957b2)
#<!-- -->25 0x00000000066189e5 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66189e5)
#<!-- -->26 0x00000000066286e9 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66286e9)
#<!-- -->27 0x00000000066313a7 clang::Parser::ParseSimpleDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, bool, clang::Parser::ForRangeInit*, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66313a7)
#<!-- -->28 0x0000000006631869 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6631869)
#<!-- -->29 0x00000000066e1332 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66e1332)
#<!-- -->30 0x00000000066e203d clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66e203d)
#<!-- -->31 0x00000000066e9d53 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66e9d53)
#<!-- -->32 0x00000000066ea51a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66ea51a)
#<!-- -->33 0x00000000065f2a43 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65f2a43)
#<!-- -->34 0x00000000066290ed clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66290ed)
#<!-- -->35 0x00000000065e657e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65e657e)
#<!-- -->36 0x00000000065e6d39 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65e6d39)
#<!-- -->37 0x00000000065ee65a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65ee65a)
#<!-- -->38 0x00000000065ef5fd clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65ef5fd)
#<!-- -->39 0x00000000065e19da clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65e19da)
#<!-- -->40 0x0000000004912908 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4912908)
#<!-- -->41 0x0000000004c05bb5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c05bb5)
#<!-- -->42 0x0000000004b851be clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b851be)
#<!-- -->43 0x0000000004cf9a09 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4cf9a09)
#<!-- -->44 0x0000000000da625f cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda625f)
#<!-- -->45 0x0000000000d9c42a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->46 0x00000000049791b9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->47 0x0000000003eb3b44 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3eb3b44)
#<!-- -->48 0x00000000049797cf clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->49 0x000000000493bc1d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x493bc1d)
#<!-- -->50 0x000000000493ccae clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x493ccae)
#<!-- -->51 0x0000000004944b05 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4944b05)
#<!-- -->52 0x0000000000da21c8 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda21c8)
#<!-- -->53 0x0000000000c27864 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc27864)
#<!-- -->54 0x00007d9110029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->55 0x00007d9110029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->56 0x0000000000d9bed5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd9bed5)
```

## Program

```c++
#include &lt;thread&gt;
#include &lt;future&gt;
#include &lt;memory&gt;
using namespace std;
constexpr size_t kMemoryChunk = 1024 * 8;
constexpr size_t kNumberOfIterations = 2000000;
constexpr size_t kThreadsNumber = 2 * kMemoryChunk;
int main() {
    array&lt;shared_future&lt;short*&gt;, kThreadsNumber * kNumberOfIterations&gt; futures{};
}
```

## Gcc Bolt

Here it can be seen stranger behaviour when include is removed.

https://gcc.godbolt.org/z/af8eKT3ax

Here in current version with include 

https://gcc.godbolt.org/z/zv6b6vWs1

## Additional question

I am also wondering if LLVM ERROR: out of memory programs are worth submiting an issue, thanks in advance



</details>


---

### Comment 3 - shafik

Reduced a bit: https://gcc.godbolt.org/z/e3McKqPez

```cpp
#include <array>

constexpr unsigned kMemoryChunk = 1024 * 8;
constexpr unsigned kNumberOfIterations = 2000000;
constexpr unsigned kThreadsNumber = 2 * kMemoryChunk;

template <typename T>
class A{};

int main() {
    std::array<A<short*>, kThreadsNumber * kNumberOfIterations> futures{};
}
```

---

### Comment 4 - llvmbot


Hi!

This issue may be a good introductory issue for people new to working on LLVM. If you would like to work on this issue, your first steps are:

1. Check that no other contributor has already been assigned to this issue. If you believe that no one is actually working on it despite an assignment, ping the person. After one week without a response, the assignee may be changed.
1. In the comments of this issue, request for it to be assigned to you, or just create a [pull request](https://github.com/llvm/llvm-project/pulls) after following the steps below. [Mention](https://docs.github.com/en/issues/tracking-your-work-with-issues/linking-a-pull-request-to-an-issue) this issue in the description of the pull request.
1. Fix the issue locally.
1. [Run the test suite](https://llvm.org/docs/TestingGuide.html#unit-and-regression-tests) locally. Remember that the subdirectories under `test/` create fine-grained testing targets, so you can e.g. use `make check-clang-ast` to only run Clang's AST tests.
1. Create a Git commit.
1. Run [`git clang-format HEAD~1`](https://clang.llvm.org/docs/ClangFormat.html#git-integration) to format your changes.
1. Open a [pull request](https://github.com/llvm/llvm-project/pulls) to the [upstream repository](https://github.com/llvm/llvm-project) on GitHub. Detailed instructions can be found [in GitHub's documentation](https://docs.github.com/en/pull-requests/collaborating-with-pull-requests/proposing-changes-to-your-work-with-pull-requests/creating-a-pull-request). [Mention](https://docs.github.com/en/issues/tracking-your-work-with-issues/linking-a-pull-request-to-an-issue) this issue in the description of the pull request.

If you have any further questions about this issue, don't hesitate to ask via a comment in the thread below.



---

### Comment 5 - llvmbot


@llvm/issue-subscribers-good-first-issue

Author: None (mariete1223)

<details>
## Summary

LLVM ERROR: out of memory strange error, it occurs with std=c++23 and versions 19, 20 and current, it has been found that in current version if you remove ```#include &lt;thread&gt;``` the compiler does not crashes but in version 20 and 19 it does. 

## Assertion

No assertion given

## Stack dump

```
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x c++ -std=c++23 &lt;source&gt;
1.	&lt;source&gt;:9:81: current parser token ';'
2.	&lt;source&gt;:8:12: parsing function body 'main'
3.	&lt;source&gt;:8:12: in compound statement ('{}')
 #<!-- -->0 0x0000000003f70b58 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f70b58)
 #<!-- -->1 0x0000000003f6e7e4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f6e7e4)
 #<!-- -->2 0x0000000003eb36a8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007d9110042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007d91100969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007d9110042476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007d91100287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x0000000003ebeeab (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3ebeeab)
 #<!-- -->8 0x0000000003ebeec5 (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3ebeec5)
 #<!-- -->9 0x00007d91104ae9a3 operator new(unsigned long) (/lib/x86_64-linux-gnu/libstdc++.so.6+0xae9a3)
#<!-- -->10 0x00000000074b318c clang::APValue::MakeArray(unsigned int, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x74b318c)
#<!-- -->11 0x000000000788d422 (anonymous namespace)::ArrayExprEvaluator::VisitCXXParenListOrInitListExpr(clang::Expr const*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::Expr const*, clang::QualType) ExprConstant.cpp:0:0
#<!-- -->12 0x00000000078c4faf clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::ArrayExprEvaluator, bool&gt;::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#<!-- -->13 0x00000000078c65cd EvaluateArray(clang::Expr const*, (anonymous namespace)::LValue const&amp;, clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;) ExprConstant.cpp:0:0
#<!-- -->14 0x000000000788bd11 EvaluateInPlace(clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;, (anonymous namespace)::LValue const&amp;, clang::Expr const*, bool) ExprConstant.cpp:0:0
#<!-- -->15 0x000000000788ed06 (anonymous namespace)::RecordExprEvaluator::VisitCXXParenListOrInitListExpr(clang::Expr const*, llvm::ArrayRef&lt;clang::Expr*&gt;) ExprConstant.cpp:0:0
#<!-- -->16 0x00000000078c94bf clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::RecordExprEvaluator, bool&gt;::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#<!-- -->17 0x00000000078ca038 EvaluateRecord(clang::Expr const*, (anonymous namespace)::LValue const&amp;, clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;) ExprConstant.cpp:0:0
#<!-- -->18 0x000000000788bd81 EvaluateInPlace(clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;, (anonymous namespace)::LValue const&amp;, clang::Expr const*, bool) ExprConstant.cpp:0:0
#<!-- -->19 0x00000000078cf690 clang::Expr::EvaluateAsInitializer(clang::APValue&amp;, clang::ASTContext const&amp;, clang::VarDecl const*, llvm::SmallVectorImpl&lt;std::pair&lt;clang::SourceLocation, clang::PartialDiagnostic&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x78cf690)
#<!-- -->20 0x00000000076ecf42 clang::VarDecl::evaluateValueImpl(llvm::SmallVectorImpl&lt;std::pair&lt;clang::SourceLocation, clang::PartialDiagnostic&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x76ecf42)
#<!-- -->21 0x00000000076ed0ba clang::VarDecl::evaluateValue() const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x76ed0ba)
#<!-- -->22 0x0000000006a256d8 clang::Sema::FinalizeVarWithDestructor(clang::VarDecl*, clang::RecordType const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a256d8)
#<!-- -->23 0x000000000698dd5b clang::Sema::CheckCompleteVariableDeclaration(clang::VarDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x698dd5b)
#<!-- -->24 0x00000000069957b2 clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69957b2)
#<!-- -->25 0x00000000066189e5 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66189e5)
#<!-- -->26 0x00000000066286e9 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66286e9)
#<!-- -->27 0x00000000066313a7 clang::Parser::ParseSimpleDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, bool, clang::Parser::ForRangeInit*, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66313a7)
#<!-- -->28 0x0000000006631869 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6631869)
#<!-- -->29 0x00000000066e1332 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66e1332)
#<!-- -->30 0x00000000066e203d clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66e203d)
#<!-- -->31 0x00000000066e9d53 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66e9d53)
#<!-- -->32 0x00000000066ea51a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66ea51a)
#<!-- -->33 0x00000000065f2a43 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65f2a43)
#<!-- -->34 0x00000000066290ed clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66290ed)
#<!-- -->35 0x00000000065e657e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65e657e)
#<!-- -->36 0x00000000065e6d39 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65e6d39)
#<!-- -->37 0x00000000065ee65a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65ee65a)
#<!-- -->38 0x00000000065ef5fd clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65ef5fd)
#<!-- -->39 0x00000000065e19da clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65e19da)
#<!-- -->40 0x0000000004912908 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4912908)
#<!-- -->41 0x0000000004c05bb5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c05bb5)
#<!-- -->42 0x0000000004b851be clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b851be)
#<!-- -->43 0x0000000004cf9a09 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4cf9a09)
#<!-- -->44 0x0000000000da625f cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda625f)
#<!-- -->45 0x0000000000d9c42a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->46 0x00000000049791b9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->47 0x0000000003eb3b44 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3eb3b44)
#<!-- -->48 0x00000000049797cf clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->49 0x000000000493bc1d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x493bc1d)
#<!-- -->50 0x000000000493ccae clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x493ccae)
#<!-- -->51 0x0000000004944b05 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4944b05)
#<!-- -->52 0x0000000000da21c8 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda21c8)
#<!-- -->53 0x0000000000c27864 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc27864)
#<!-- -->54 0x00007d9110029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->55 0x00007d9110029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->56 0x0000000000d9bed5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd9bed5)
```

## Program

```c++
#include &lt;thread&gt;
#include &lt;future&gt;
#include &lt;memory&gt;
using namespace std;
constexpr size_t kMemoryChunk = 1024 * 8;
constexpr size_t kNumberOfIterations = 2000000;
constexpr size_t kThreadsNumber = 2 * kMemoryChunk;
int main() {
    array&lt;shared_future&lt;short*&gt;, kThreadsNumber * kNumberOfIterations&gt; futures{};
}
```

## Gcc Bolt

Here it can be seen stranger behaviour when include is removed.

https://gcc.godbolt.org/z/af8eKT3ax

Here in current version with include 

https://gcc.godbolt.org/z/zv6b6vWs1

## Additional question

I am also wondering if LLVM ERROR: out of memory programs are worth submiting an issue, thanks in advance



</details>


---

### Comment 6 - tbaederr

Removing the beginner label since there's a PR already.

---

