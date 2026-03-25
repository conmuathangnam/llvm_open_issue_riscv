# [clang][bytecode] plain _Complex causes Assertion `SrcDesc->isPrimitiveArray()' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/163563
**Status:** Closed
**Labels:** confirmed, crash, clang:bytecode
**Closed Date:** 2025-11-11T10:29:00Z

## Body

Reproducer:
https://godbolt.org/z/f8oxM6E3v
```c
void foo() {
  _Complex cd;
  cd = *(_Complex *)&(struct { double r, i; }){0.0, 0.0};
}
```

Backtrace:
```console
clang: /root/llvm-project/llvm/tools/clang/lib/AST/ByteCode/InterpBuiltin.cpp:4140: bool clang::interp::copyComposite(clang::interp::InterpState&, clang::interp::CodePtr, const clang::interp::Pointer&, clang::interp::Pointer&, bool): Assertion `SrcDesc->isPrimitiveArray()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -fexperimental-new-constant-interpreter <source>
1.	<source>:4:1: current parser token '}'
2.	<source>:1:12: parsing function body 'foo'
3.	<source>:1:12: in compound statement ('{}')
 #0 0x00000000041f8598 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x41f8598)
 #1 0x00000000041f59c4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x41f59c4)
 #2 0x0000000004139df8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007d0076242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007d00762969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007d0076242476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007d00762287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007d007622871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007d0076239e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000007d252ff clang::interp::copyComposite(clang::interp::InterpState&, clang::interp::CodePtr, clang::interp::Pointer const&, clang::interp::Pointer&, bool) InterpBuiltin.cpp:0:0
#10 0x0000000007c12e19 clang::interp::EvalEmitter::emitMemcpy(clang::interp::SourceInfo) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x7c12e19)
#11 0x000000000805b8bd clang::interp::Compiler<clang::interp::EvalEmitter>::VisitCastExpr(clang::CastExpr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x805b8bd)
#12 0x000000000804d162 clang::interp::Compiler<clang::interp::EvalEmitter>::visitInitializer(clang::Expr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x804d162)
#13 0x0000000008059e40 clang::interp::Compiler<clang::interp::EvalEmitter>::visit(clang::Expr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x8059e40)
#14 0x000000000804814d clang::interp::Compiler<clang::interp::EvalEmitter>::VisitComplexBinOp(clang::BinaryOperator const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x804814d)
#15 0x000000000805c5d3 clang::interp::Compiler<clang::interp::EvalEmitter>::VisitBinaryOperator(clang::BinaryOperator const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x805c5d3)
#16 0x000000000804d162 clang::interp::Compiler<clang::interp::EvalEmitter>::visitInitializer(clang::Expr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x804d162)
#17 0x000000000804eb15 clang::interp::Compiler<clang::interp::EvalEmitter>::visitExpr(clang::Expr const*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x804eb15)
#18 0x0000000007c08ddd clang::interp::EvalEmitter::interpretExpr(clang::Expr const*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x7c08ddd)
#19 0x0000000007bf9713 clang::interp::Context::evaluateAsRValue(clang::interp::State&, clang::Expr const*, clang::APValue&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x7bf9713)
#20 0x0000000007b6c123 EvaluateAsRValue((anonymous namespace)::EvalInfo&, clang::Expr const*, clang::APValue&) ExprConstant.cpp:0:0
#21 0x0000000007b6f7ae clang::Expr::EvaluateForOverflow(clang::ASTContext const&) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x7b6f7ae)
#22 0x0000000006b2e296 clang::Sema::CheckForIntOverflow(clang::Expr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6b2e296)
#23 0x0000000006b713dc clang::Sema::CheckCompletedExpr(clang::Expr*, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6b713dc)
#24 0x0000000006f141d7 clang::Sema::ActOnFinishFullExpr(clang::Expr*, clang::SourceLocation, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6f141d7)
#25 0x000000000722bece clang::Sema::ActOnExprStmt(clang::ActionResult<clang::Expr*, true>, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x722bece)
#26 0x00000000069f5964 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x69f5964)
#27 0x00000000069ecef2 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x69ecef2)
#28 0x00000000069ed8b9 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x69ed8b9)
#29 0x00000000069f6046 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x69f6046)
#30 0x00000000069f685a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x69f685a)
#31 0x00000000068fd26b clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x68fd26b)
#32 0x00000000069336dd clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x69336dd)
#33 0x00000000068f040e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x68f040e)
#34 0x00000000068f0baf clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x68f0baf)
#35 0x00000000068f89ea clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x68f89ea)
#36 0x00000000068f9985 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x68f9985)
#37 0x00000000068f9e40 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x68f9e40)
#38 0x00000000068dadc3 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x68dadc3)
#39 0x0000000004b3f4f8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4b3f4f8)
#40 0x0000000004e2c555 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4e2c555)
#41 0x0000000004dad6ee clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4dad6ee)
#42 0x0000000004f23d2d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4f23d2d)
#43 0x0000000000dc20f0 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xdc20f0)
#44 0x0000000000db8c5a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#45 0x0000000000db8ddd int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#46 0x0000000004ba8689 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#47 0x000000000413a294 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x413a294)
#48 0x0000000004ba8c9f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#49 0x0000000004b69fb2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4b69fb2)
#50 0x0000000004b6af5e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4b6af5e)
#51 0x0000000004b72695 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4b72695)
#52 0x0000000000dbe619 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xdbe619)
#53 0x0000000000c6e1b4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xc6e1b4)
#54 0x00007d0076229d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#55 0x00007d0076229e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#56 0x0000000000db86f5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xdb86f5)
clang: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

Related issue: https://github.com/llvm/llvm-project/issues/155144

