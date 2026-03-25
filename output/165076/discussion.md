# [clang][bytecode] crash on __builtin_sadd_overflow: Assertion `isBlockPointer()' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/165076
**Status:** Closed
**Labels:** crash, clang:bytecode
**Closed Date:** 2025-10-27T10:55:53Z

## Body

Reproducer:
https://godbolt.org/z/b3Y8njocW
```cpp
int a;

void foo(void) { a *= __builtin_sadd_overflow(1, 2, 0); }
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/AST/ByteCode/Pointer.h:665: T& clang::interp::Pointer::deref() const [with T = clang::interp::Integral<32, true>]: Assertion `isBlockPointer()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x c -fexperimental-new-constant-interpreter <source>
1.	<source>:3:57: current parser token '}'
2.	<source>:3:16: parsing function body 'foo'
3.	<source>:3:16: in compound statement ('{}')
 #0 0x0000000004214d18 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4214d18)
 #1 0x0000000004212144 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4212144)
 #2 0x0000000004156348 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x0000707a34842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x0000707a348969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x0000707a34842476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x0000707a348287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x0000707a3482871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x0000707a34839e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000007c733a9 clang::interp::Integral<32u, true>& clang::interp::Pointer::deref<clang::interp::Integral<32u, true>>() const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7c733a9)
#10 0x0000000007da44a8 clang::interp::assignInteger(clang::interp::InterpState&, clang::interp::Pointer const&, clang::interp::PrimType, llvm::APSInt const&) InterpBuiltin.cpp:0:0
#11 0x0000000007da4cd7 clang::interp::interp__builtin_overflowop(clang::interp::InterpState&, clang::interp::CodePtr, clang::CallExpr const*, unsigned int) (.isra.0) InterpBuiltin.cpp:0:0
#12 0x0000000007dad656 clang::interp::InterpretBuiltin(clang::interp::InterpState&, clang::interp::CodePtr, clang::CallExpr const*, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7dad656)
#13 0x00000000080c929b clang::interp::Compiler<clang::interp::EvalEmitter>::VisitBuiltinCallExpr(clang::CallExpr const*, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x80c929b)
#14 0x00000000080c962e clang::interp::Compiler<clang::interp::EvalEmitter>::VisitCallExpr(clang::CallExpr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x80c962e)
#15 0x00000000080d04ca clang::interp::Compiler<clang::interp::EvalEmitter>::visit(clang::Expr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x80d04ca)
#16 0x00000000080c50c6 clang::interp::Compiler<clang::interp::EvalEmitter>::visitExpr(clang::Expr const*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x80c50c6)
#17 0x0000000007c657bd clang::interp::EvalEmitter::interpretExpr(clang::Expr const*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7c657bd)
#18 0x0000000007c54c33 clang::interp::Context::evaluateAsRValue(clang::interp::State&, clang::Expr const*, clang::APValue&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7c54c33)
#19 0x0000000007bc2ae3 EvaluateAsRValue((anonymous namespace)::EvalInfo&, clang::Expr const*, clang::APValue&) ExprConstant.cpp:0:0
#20 0x0000000007bc7190 clang::Expr::EvaluateAsRValue(clang::Expr::EvalResult&, clang::ASTContext const&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7bc7190)
#21 0x0000000006b54fdb TryGetExprRange(clang::ASTContext&, clang::Expr const*, unsigned int, bool, bool) SemaChecking.cpp:0:0
#22 0x0000000006b9130e clang::Sema::CheckImplicitConversion(clang::Expr*, clang::QualType, clang::SourceLocation, bool*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b9130e)
#23 0x0000000006b95069 AnalyzeImplicitConversions(clang::Sema&, clang::Expr*, clang::SourceLocation, bool) (.constprop.0) SemaChecking.cpp:0:0
#24 0x0000000006b951de AnalyzeImplicitConversions(clang::Sema&, clang::Expr*, clang::SourceLocation, bool) (.constprop.0) SemaChecking.cpp:0:0
#25 0x0000000006b96e25 clang::Sema::CheckCompletedExpr(clang::Expr*, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b96e25)
#26 0x0000000006f396e7 clang::Sema::ActOnFinishFullExpr(clang::Expr*, clang::SourceLocation, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f396e7)
#27 0x000000000724f57e clang::Sema::ActOnExprStmt(clang::ActionResult<clang::Expr*, true>, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x724f57e)
#28 0x0000000006a1ba74 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a1ba74)
#29 0x0000000006a13002 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a13002)
#30 0x0000000006a139c9 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a139c9)
#31 0x0000000006a1c156 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a1c156)
#32 0x0000000006a1c96a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a1c96a)
#33 0x000000000692343b clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x692343b)
#34 0x00000000069598ad clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69598ad)
#35 0x0000000006916bee clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6916bee)
#36 0x000000000691738f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x691738f)
#37 0x000000000691ec43 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x691ec43)
#38 0x000000000691fb55 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x691fb55)
#39 0x00000000069011da clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69011da)
#40 0x0000000004b5d4e8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b5d4e8)
#41 0x0000000004e4a685 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e4a685)
#42 0x0000000004dcb73e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4dcb73e)
#43 0x0000000004f41c2d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f41c2d)
#44 0x0000000000dce380 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdce380)
#45 0x0000000000dc4eea ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#46 0x0000000000dc506d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#47 0x0000000004bc6a79 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#48 0x00000000041567e4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41567e4)
#49 0x0000000004bc708f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#50 0x0000000004b87fd2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b87fd2)
#51 0x0000000004b88f7e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b88f7e)
#52 0x0000000004b90695 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b90695)
#53 0x0000000000dca8a9 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdca8a9)
#54 0x0000000000c79c64 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc79c64)
#55 0x0000707a34829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#56 0x0000707a34829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#57 0x0000000000dc4985 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdc4985)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - k-arrows

Goes back to clang-19.1.0:
https://godbolt.org/z/qTdPxrhbM

---

