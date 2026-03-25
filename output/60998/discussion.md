# clang 15.0.7: Out of memory compiling large array of atomics with -std=c++20

**Author:** sliedes
**URL:** https://github.com/llvm/llvm-project/issues/60998
**Status:** Closed
**Labels:** duplicate, clang:frontend, crash
**Closed Date:** 2025-07-02T21:00:33Z

## Body

When compiling a large array with atomics, Clang 15.0.7, installed from the Arch package archive, crashes with out of memory error. Making the array significantly smaller makes the crash go away. The crash only happens with `-std=c++20` or `-std=gnu++20`, not with `c++17` or `c++14`.

Test case:

```c++
#include <array>
#include <atomic>

using Array = std::array<std::atomic<int>, 1000000000>;

Array *arr;

int main() { arr = new Array(); }
```

Invocation:

```console
$ /usr/bin/clang -c -std=c++20 -emit-llvm -Xclang -disable-llvm-passes crash.cpp
LLVM ERROR: out of memory
Allocation failed
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /usr/bin/clang -c -std=c++20 -emit-llvm -Xclang -disable-llvm-passes crash.cpp
1.	crash.cpp:8:33: current parser token '}'
2.	crash.cpp:8:12: parsing function body 'main'
3.	crash.cpp:8:12: in compound statement ('{}')
 #0 0x00007f5ac9d7c48a (/usr/lib/libLLVM-15.so+0xd7c48a)
 #1 0x00007f5ac9d79b1f llvm::sys::RunSignalHandlers() (/usr/lib/libLLVM-15.so+0xd79b1f)
 #2 0x00007f5ac9c5c5e9 (/usr/lib/libLLVM-15.so+0xc5c5e9)
 #3 0x00007f5ac8a51f50 (/usr/lib/libc.so.6+0x38f50)
 #4 0x00007f5ac8aa08ec (/usr/lib/libc.so.6+0x878ec)
 #5 0x00007f5ac8a51ea8 raise (/usr/lib/libc.so.6+0x38ea8)
 #6 0x00007f5ac8a3b53d abort (/usr/lib/libc.so.6+0x2253d)
 #7 0x00007f5ac9c75b46 (/usr/lib/libLLVM-15.so+0xc75b46)
 #8 0x00007f5ac9c75b6b (/usr/lib/libLLVM-15.so+0xc75b6b)
 #9 0x00007f5ac8ca74a5 operator new(unsigned long) /usr/src/debug/gcc/gcc/libstdc++-v3/libsupc++/new_op.cc:55:15
#10 0x00007f5ad156f1b3 (/usr/lib/libclang-cpp.so.15+0xb6f1b3)
#11 0x00007f5ad156cecc (/usr/lib/libclang-cpp.so.15+0xb6cecc)
#12 0x00007f5ad156d52d (/usr/lib/libclang-cpp.so.15+0xb6d52d)
#13 0x00007f5ad156eb37 (/usr/lib/libclang-cpp.so.15+0xb6eb37)
#14 0x00007f5ad156ede4 (/usr/lib/libclang-cpp.so.15+0xb6ede4)
#15 0x00007f5ad30b1a74 (/usr/lib/libclang-cpp.so.15+0x26b1a74)
#16 0x00007f5ad1570085 (/usr/lib/libclang-cpp.so.15+0xb70085)
#17 0x00007f5ad157caec (/usr/lib/libclang-cpp.so.15+0xb7caec)
#18 0x00007f5ad158129c (/usr/lib/libclang-cpp.so.15+0xb8129c)
#19 0x00007f5ad157ca07 (/usr/lib/libclang-cpp.so.15+0xb7ca07)
#20 0x00007f5ad1587ac9 (/usr/lib/libclang-cpp.so.15+0xb87ac9)
#21 0x00007f5ad1589c80 clang::Expr::EvaluateForOverflow(clang::ASTContext const&) const (/usr/lib/libclang-cpp.so.15+0xb89c80)
#22 0x00007f5ad19db9fc clang::Sema::CheckForIntOverflow(clang::Expr*) (/usr/lib/libclang-cpp.so.15+0xfdb9fc)
#23 0x00007f5ad19ecb9b clang::Sema::CheckCompletedExpr(clang::Expr*, clang::SourceLocation, bool) (/usr/lib/libclang-cpp.so.15+0xfecb9b)
#24 0x00007f5ad1c48a36 clang::Sema::ActOnFinishFullExpr(clang::Expr*, clang::SourceLocation, bool, bool) (/usr/lib/libclang-cpp.so.15+0x1248a36)
#25 0x00007f5ad1ddbd4c clang::Sema::ActOnExprStmt(clang::ActionResult<clang::Expr*, true>, bool) (/usr/lib/libclang-cpp.so.15+0x13dbd4c)
#26 0x00007f5ad12f0529 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/usr/lib/libclang-cpp.so.15+0x8f0529)
#27 0x00007f5ad12f101a clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/usr/lib/libclang-cpp.so.15+0x8f101a)
#28 0x00007f5ad12f5f8d clang::Parser::ParseCompoundStatementBody(bool) (/usr/lib/libclang-cpp.so.15+0x8f5f8d)
#29 0x00007f5ad12f7c3a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/usr/lib/libclang-cpp.so.15+0x8f7c3a)
#30 0x00007f5ad12f8af4 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/usr/lib/libclang-cpp.so.15+0x8f8af4)
#31 0x00007f5ad12767c3 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/usr/lib/libclang-cpp.so.15+0x8767c3)
#32 0x00007f5ad12e7292 (/usr/lib/libclang-cpp.so.15+0x8e7292)
#33 0x00007f5ad12fd01c clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/usr/lib/libclang-cpp.so.15+0x8fd01c)
#34 0x00007f5ad12fd400 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/usr/lib/libclang-cpp.so.15+0x8fd400)
#35 0x00007f5ad1249169 clang::ParseAST(clang::Sema&, bool, bool) (/usr/lib/libclang-cpp.so.15+0x849169)
#36 0x00007f5ad28e7479 clang::FrontendAction::Execute() (/usr/lib/libclang-cpp.so.15+0x1ee7479)
#37 0x00007f5ad288d14f clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/usr/lib/libclang-cpp.so.15+0x1e8d14f)
#38 0x00007f5ad295209a clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/lib/libclang-cpp.so.15+0x1f5209a)
#39 0x00005590ddc774d0 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/usr/bin/clang+0x104d0)
#40 0x00005590ddc7d197 (/usr/bin/clang+0x16197)
#41 0x00007f5ad25d5959 (/usr/lib/libclang-cpp.so.15+0x1bd5959)
#42 0x00007f5ac9c5c6b7 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/usr/lib/libLLVM-15.so+0xc5c6b7)
#43 0x00007f5ad25db48f (/usr/lib/libclang-cpp.so.15+0x1bdb48f)
#44 0x00007f5ad259ebbe clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/usr/lib/libclang-cpp.so.15+0x1b9ebbe)
#45 0x00007f5ad259ef7d clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/usr/lib/libclang-cpp.so.15+0x1b9ef7d)
#46 0x00007f5ad25f815c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/usr/lib/libclang-cpp.so.15+0x1bf815c)
#47 0x00005590ddc7f7f9 clang_main(int, char**) (/usr/bin/clang+0x187f9)
#48 0x00007f5ac8a3c790 (/usr/lib/libc.so.6+0x23790)
#49 0x00007f5ac8a3c84a __libc_start_main (/usr/lib/libc.so.6+0x2384a)
#50 0x00005590ddc73d25 _start (/usr/bin/clang+0xcd25)
clang-15: error: clang frontend command failed with exit code 134 (use -v to see invocation)
clang version 15.0.7
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
clang-15: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang-15: note: diagnostic msg: /tmp/crash-de1c94.cpp
clang-15: note: diagnostic msg: /tmp/crash-de1c94.sh
clang-15: note: diagnostic msg: 

********************
```

[crash-de1c94.cpp.gz](https://github.com/llvm/llvm-project/files/10831915/crash-de1c94.cpp.gz)
[crash-de1c94.sh.gz](https://github.com/llvm/llvm-project/files/10831925/crash-de1c94.sh.gz)


## Comments

### Comment 1 - llvmbot

@llvm/issue-subscribers-clang-frontend

---

### Comment 2 - shafik

Confirmed: https://godbolt.org/z/9orq9sd5f

Backtrace:

```console
LLVM ERROR: out of memory
Allocation failed
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 <source>
1.	<source>:8:33: current parser token '}'
2.	<source>:8:12: parsing function body 'main'
3.	<source>:8:12: in compound statement ('{}')
 #0 0x00005626cb8f38df llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x40728df)
 #1 0x00005626cb8f161c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x407061c)
 #2 0x00005626cb83efc8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007f0709b45420 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x14420)
 #4 0x00007f070961200b raise (/lib/x86_64-linux-gnu/libc.so.6+0x4300b)
 #5 0x00007f07095f1859 abort (/lib/x86_64-linux-gnu/libc.so.6+0x22859)
 #6 0x00005626cb84a36f (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3fc936f)
 #7 0x00005626cb84a38b (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3fc938b)
 #8 0x00007f07099d7b50 operator new(unsigned long) (/lib/x86_64-linux-gnu/libstdc++.so.6+0xaab50)
 #9 0x00005626cedca3aa clang::APValue::MakeArray(unsigned int, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x75493aa)
#10 0x00005626cf0cffb1 (anonymous namespace)::ArrayExprEvaluator::VisitCXXConstructExpr(clang::CXXConstructExpr const*, (anonymous namespace)::LValue const&, clang::APValue*, clang::QualType) ExprConstant.cpp:0:0
#11 0x00005626cf0d17dd clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::ArrayExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#12 0x00005626cf0d24ad EvaluateArray(clang::Expr const*, (anonymous namespace)::LValue const&, clang::APValue&, (anonymous namespace)::EvalInfo&) ExprConstant.cpp:0:0
#13 0x00005626cf0a0084 EvaluateInPlace(clang::APValue&, (anonymous namespace)::EvalInfo&, (anonymous namespace)::LValue const&, clang::Expr const*, bool) ExprConstant.cpp:0:0
#14 0x00005626cf0ce96f HandleConstructorCall(clang::Expr const*, (anonymous namespace)::LValue const&, (anonymous namespace)::CallRef, clang::CXXConstructorDecl const*, (anonymous namespace)::EvalInfo&, clang::APValue&) ExprConstant.cpp:0:0
#15 0x00005626cf0cf8e7 HandleConstructorCall(clang::Expr const*, (anonymous namespace)::LValue const&, llvm::ArrayRef<clang::Expr const*>, clang::CXXConstructorDecl const*, (anonymous namespace)::EvalInfo&, clang::APValue&) ExprConstant.cpp:0:0
#16 0x00005626cf0cfb23 (anonymous namespace)::RecordExprEvaluator::VisitCXXConstructExpr(clang::CXXConstructExpr const*, clang::QualType) ExprConstant.cpp:0:0
#17 0x00005626cf0d3fd0 clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::RecordExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#18 0x00005626cf0d5cd4 EvaluateRecord(clang::Expr const*, (anonymous namespace)::LValue const&, clang::APValue&, (anonymous namespace)::EvalInfo&) ExprConstant.cpp:0:0
#19 0x00005626cf0a00c5 EvaluateInPlace(clang::APValue&, (anonymous namespace)::EvalInfo&, (anonymous namespace)::LValue const&, clang::Expr const*, bool) ExprConstant.cpp:0:0
#20 0x00005626cf0d8e22 (anonymous namespace)::PointerExprEvaluator::VisitCXXNewExpr(clang::CXXNewExpr const*) ExprConstant.cpp:0:0
#21 0x00005626cf0998fe clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::PointerExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#22 0x00005626cf09ae46 EvaluatePointer(clang::Expr const*, (anonymous namespace)::LValue&, (anonymous namespace)::EvalInfo&, bool) ExprConstant.cpp:0:0
#23 0x00005626cf080de8 Evaluate(clang::APValue&, (anonymous namespace)::EvalInfo&, clang::Expr const*) ExprConstant.cpp:0:0
#24 0x00005626cf0ab9cf clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::LValueExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#25 0x00005626cf0ac94d EvaluateLValue(clang::Expr const*, (anonymous namespace)::LValue&, (anonymous namespace)::EvalInfo&, bool) ExprConstant.cpp:0:0
#26 0x00005626cf080a44 Evaluate(clang::APValue&, (anonymous namespace)::EvalInfo&, clang::Expr const*) ExprConstant.cpp:0:0
#27 0x00005626cf08ad59 EvaluateAsRValue((anonymous namespace)::EvalInfo&, clang::Expr const*, clang::APValue&) ExprConstant.cpp:0:0
#28 0x00005626cf08bd41 clang::Expr::EvaluateForOverflow(clang::ASTContext const&) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x780ad41)
#29 0x00005626ce167995 clang::Sema::CheckForIntOverflow(clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68e6995)
#30 0x00005626ce1bab75 clang::Sema::CheckCompletedExpr(clang::Expr*, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6939b75)
#31 0x00005626ce620e2a clang::Sema::ActOnFinishFullExpr(clang::Expr*, clang::SourceLocation, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d9fe2a)
#32 0x00005626ce8a093e clang::Sema::ActOnExprStmt(clang::ActionResult<clang::Expr*, true>, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x701f93e)
#33 0x00005626ce08bbbc clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x680abbc)
#34 0x00005626ce081e41 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6800e41)
#35 0x00005626ce082cb5 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6801cb5)
#36 0x00005626ce083c8a clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6802c8a)
#37 0x00005626ce08563a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x680463a)
#38 0x00005626cdfb3391 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6732391)
#39 0x00005626cdfde550 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x675d550)
#40 0x00005626cdfa6a32 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6725a32)
#41 0x00005626cdfa72df clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (.part.0) Parser.cpp:0:0
#42 0x00005626cdfaddb9 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x672cdb9)
#43 0x00005626cdfae70d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x672d70d)
#44 0x00005626cdfa25aa clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67215aa)
#45 0x00005626ccc547f8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x53d37f8)
#46 0x00005626cc4bde59 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c3ce59)
#47 0x00005626cc441de6 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bc0de6)
#48 0x00005626cc5a1797 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d20797)
#49 0x00005626c8de4666 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x1563666)
#50 0x00005626c8de017a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#51 0x00005626cc2a910d void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#52 0x00005626cb83f4b0 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3fbe4b0)
#53 0x00005626cc2a99cf clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#54 0x00005626cc2716dc clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49f06dc)
#55 0x00005626cc27217d clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49f117d)
#56 0x00005626cc279e4d clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49f8e4d)
#57 0x00005626c8de2a1c clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x1561a1c)
#58 0x00005626c8cef4a5 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x146e4a5)
#59 0x00007f07095f3083 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x24083)
#60 0x00005626c8ddadae _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x1559dae)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

Looks similar to: https://github.com/llvm/llvm-project/issues/52644 and https://github.com/llvm/llvm-project/issues/54825

---

