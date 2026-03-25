# clang crash on x86_64-linux-gnu: Assertion `isLValue()' failed

**Author:** bi6c
**URL:** https://github.com/llvm/llvm-project/issues/118916
**Status:** Closed
**Labels:** duplicate, clang:frontend, confirmed, crash
**Closed Date:** 2024-12-06T06:41:42Z

## Body

Compiler Explorer: https://godbolt.org/z/TE8s6x9Wd
```console
<source>:4:12: error: unknown type name 'a_struct'; did you mean 'struct'?
    4 |   volatile a_struct *a;
      |            ^~~~~~~~
      |            struct
<source>:4:12: error: declaration of anonymous struct must be a definition
<source>:4:3: warning: declaration does not declare anything [-Wmissing-declarations]
    4 |   volatile a_struct *a;
      |   ^~~~~~~~
<source>:5:12: error: unknown type name 'a_struct'; did you mean 'struct'?
    5 |   volatile a_struct b;
      |            ^~~~~~~~
      |            struct
warning: 'volatile' ignored on this declaration [-Wmissing-declarations]
<source>:7:3: error: use of undeclared identifier 'a'
    7 |   a = &b;
      |   ^
<source>:7:8: error: use of undeclared identifier 'b'
    7 |   a = &b;
      |        ^
<source>:8:9: error: use of undeclared identifier 'a_struct'; did you mean 'struct'?
    8 |   *a = (a_struct){1,2,3,4};
      |         ^~~~~~~~
      |         struct
clang: /root/llvm-project/clang/lib/AST/ExprClassification.cpp:57: Cl clang::Expr::ClassifyImpl(clang::ASTContext&, clang::SourceLocation*) const: Assertion `isLValue()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<source>:8:18: current parser token '{'
2.	<source>:3:1: parsing function body 'main'
3.	<source>:3:1: in compound statement ('{}')
 #0 0x0000000003bffbb8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3bffbb8)
 #1 0x0000000003bfd8bc llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3bfd8bc)
 #2 0x0000000003b4adb8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007249c8042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007249c80969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007249c8042476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007249c80287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007249c802871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007249c8039e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x00000000076476f4 clang::Expr::ClassifyImpl(clang::ASTContext&, clang::SourceLocation*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x76476f4)
#10 0x00000000076478bd clang::Expr::isModifiableLvalue(clang::ASTContext&, clang::SourceLocation*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x76478bd)
#11 0x0000000006a16d1c CheckForModifiableLvalue(clang::Expr*, clang::SourceLocation, clang::Sema&) SemaExpr.cpp:0:0
#12 0x0000000006a4bb17 clang::Sema::CheckAssignmentOperands(clang::Expr*, clang::ActionResult<clang::Expr*, true>&, clang::SourceLocation, clang::QualType, clang::BinaryOperatorKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6a4bb17)
#13 0x0000000006a4d0f2 clang::Sema::CreateBuiltinBinOp(clang::SourceLocation, clang::BinaryOperatorKind, clang::Expr*, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6a4d0f2)
#14 0x0000000006a4eae5 clang::Sema::BuildBinOp(clang::Scope*, clang::SourceLocation, clang::BinaryOperatorKind, clang::Expr*, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6a4eae5)
#15 0x0000000006a4ece4 clang::Sema::ActOnBinOp(clang::Scope*, clang::SourceLocation, clang::tok::TokenKind, clang::Expr*, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6a4ece4)
#16 0x00000000065a964d clang::Parser::ParseRHSOfBinaryExpression(clang::ActionResult<clang::Expr*, true>, clang::prec::Level) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x65a964d)
#17 0x00000000065accd9 clang::Parser::ParseExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x65accd9)
#18 0x000000000662fed9 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x662fed9)
#19 0x000000000662626e clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x662626e)
#20 0x00000000066271c0 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x66271c0)
#21 0x0000000006628085 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6628085)
#22 0x000000000662988a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x662988a)
#23 0x000000000653cbb3 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x653cbb3)
#24 0x000000000657138d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x657138d)
#25 0x000000000653094e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x653094e)
#26 0x0000000006531109 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6531109)
#27 0x0000000006538893 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6538893)
#28 0x000000000653975d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x653975d)
#29 0x0000000006539c00 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6539c00)
#30 0x000000000652bc53 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x652bc53)
#31 0x0000000004581488 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4581488)
#32 0x000000000483c3e9 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x483c3e9)
#33 0x00000000047bc12e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x47bc12e)
#34 0x000000000492752e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x492752e)
#35 0x0000000000cafdff cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xcafdff)
#36 0x0000000000ca799a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#37 0x00000000045c46d9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#38 0x0000000003b4b264 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3b4b264)
#39 0x00000000045c4ccf clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#40 0x000000000458af6d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x458af6d)
#41 0x000000000458c05d clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x458c05d)
#42 0x0000000004593415 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4593415)
#43 0x0000000000cacc43 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xcacc43)
#44 0x0000000000b81294 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xb81294)
#45 0x00007249c8029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#46 0x00007249c8029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#47 0x0000000000ca744e _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xca744e)
clang: error: clang frontend command failed with exit code 134 (use -v to see invocation)
ASM generation compiler returned: 134
<source>:4:12: error: unknown type name 'a_struct'; did you mean 'struct'?
    4 |   volatile a_struct *a;
      |            ^~~~~~~~
      |            struct
<source>:4:12: error: declaration of anonymous struct must be a definition
<source>:4:3: warning: declaration does not declare anything [-Wmissing-declarations]
    4 |   volatile a_struct *a;
      |   ^~~~~~~~
<source>:5:12: error: unknown type name 'a_struct'; did you mean 'struct'?
    5 |   volatile a_struct b;
      |            ^~~~~~~~
      |            struct
warning: 'volatile' ignored on this declaration [-Wmissing-declarations]
<source>:7:3: error: use of undeclared identifier 'a'
    7 |   a = &b;
      |   ^
<source>:7:8: error: use of undeclared identifier 'b'
    7 |   a = &b;
      |        ^
<source>:8:9: error: use of undeclared identifier 'a_struct'; did you mean 'struct'?
    8 |   *a = (a_struct){1,2,3,4};
      |         ^~~~~~~~
      |         struct
clang-20: /root/llvm-project/clang/lib/AST/ExprClassification.cpp:57: Cl clang::Expr::ClassifyImpl(clang::ASTContext&, clang::SourceLocation*) const: Assertion `isLValue()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk-20241205/bin/clang-20 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -dumpdir /app/output.s- -disable-free -clear-ast-before-backend -main-file-name example.c -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -fno-verbose-asm -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debug-info-kind=constructor -dwarf-version=4 -debugger-tuning=gdb -fdebug-compilation-dir=/app -fcoverage-compilation-dir=/app -resource-dir /opt/compiler-explorer/clang-assertions-trunk-20241205/lib/clang/20 -internal-isystem /opt/compiler-explorer/clang-assertions-trunk-20241205/lib/clang/20/include -internal-isystem /usr/local/include -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/15.0.0/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -ferror-limit 19 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/example-15d809.o -x c <source>
1.	<source>:8:18: current parser token '{'
2.	<source>:3:1: parsing function body 'main'
3.	<source>:3:1: in compound statement ('{}')
 #0 0x0000000003bffbb8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk-20241205/bin/clang-20+0x3bffbb8)
 #1 0x0000000003bfd5ac SignalHandler(int) Signals.cpp:0:0
 #2 0x0000760562442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x00007605624969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x0000760562442476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x00007605624287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x000076056242871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #7 0x0000760562439e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #8 0x00000000076476f4 clang::Expr::ClassifyImpl(clang::ASTContext&, clang::SourceLocation*) const (/opt/compiler-explorer/clang-assertions-trunk-20241205/bin/clang-20+0x76476f4)
 #9 0x00000000076478bd clang::Expr::isModifiableLvalue(clang::ASTContext&, clang::SourceLocation*) const (/opt/compiler-explorer/clang-assertions-trunk-20241205/bin/clang-20+0x76478bd)
#10 0x0000000006a16d1c CheckForModifiableLvalue(clang::Expr*, clang::SourceLocation, clang::Sema&) SemaExpr.cpp:0:0
#11 0x0000000006a4bb17 clang::Sema::CheckAssignmentOperands(clang::Expr*, clang::ActionResult<clang::Expr*, true>&, clang::SourceLocation, clang::QualType, clang::BinaryOperatorKind) (/opt/compiler-explorer/clang-assertions-trunk-20241205/bin/clang-20+0x6a4bb17)
#12 0x0000000006a4d0f2 clang::Sema::CreateBuiltinBinOp(clang::SourceLocation, clang::BinaryOperatorKind, clang::Expr*, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk-20241205/bin/clang-20+0x6a4d0f2)
#13 0x0000000006a4eae5 clang::Sema::BuildBinOp(clang::Scope*, clang::SourceLocation, clang::BinaryOperatorKind, clang::Expr*, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk-20241205/bin/clang-20+0x6a4eae5)
#14 0x0000000006a4ece4 clang::Sema::ActOnBinOp(clang::Scope*, clang::SourceLocation, clang::tok::TokenKind, clang::Expr*, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk-20241205/bin/clang-20+0x6a4ece4)
#15 0x00000000065a964d clang::Parser::ParseRHSOfBinaryExpression(clang::ActionResult<clang::Expr*, true>, clang::prec::Level) (/opt/compiler-explorer/clang-assertions-trunk-20241205/bin/clang-20+0x65a964d)
#16 0x00000000065accd9 clang::Parser::ParseExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-assertions-trunk-20241205/bin/clang-20+0x65accd9)
#17 0x000000000662fed9 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk-20241205/bin/clang-20+0x662fed9)
#18 0x000000000662626e clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk-20241205/bin/clang-20+0x662626e)
#19 0x00000000066271c0 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk-20241205/bin/clang-20+0x66271c0)
#20 0x0000000006628085 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk-20241205/bin/clang-20+0x6628085)
#21 0x000000000662988a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk-20241205/bin/clang-20+0x662988a)
#22 0x000000000653cbb3 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk-20241205/bin/clang-20+0x653cbb3)
#23 0x000000000657138d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk-20241205/bin/clang-20+0x657138d)
#24 0x000000000653094e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk-20241205/bin/clang-20+0x653094e)
#25 0x0000000006531109 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk-20241205/bin/clang-20+0x6531109)
#26 0x0000000006538893 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk-20241205/bin/clang-20+0x6538893)
#27 0x000000000653975d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk-20241205/bin/clang-20+0x653975d)
#28 0x0000000006539c00 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk-20241205/bin/clang-20+0x6539c00)
#29 0x000000000652bc53 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk-20241205/bin/clang-20+0x652bc53)
#30 0x0000000004581488 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk-20241205/bin/clang-20+0x4581488)
#31 0x000000000483c3e9 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk-20241205/bin/clang-20+0x483c3e9)
#32 0x00000000047bc12e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk-20241205/bin/clang-20+0x47bc12e)
#33 0x000000000492752e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk-20241205/bin/clang-20+0x492752e)
#34 0x0000000000cafdff cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk-20241205/bin/clang-20+0xcafdff)
#35 0x0000000000ca799a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#36 0x0000000000cab8de clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk-20241205/bin/clang-20+0xcab8de)
#37 0x0000000000b81294 main (/opt/compiler-explorer/clang-assertions-trunk-20241205/bin/clang-20+0xb81294)
#38 0x0000760562429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#39 0x0000760562429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#40 0x0000000000ca744e _start (/opt/compiler-explorer/clang-assertions-trunk-20241205/bin/clang-20+0xca744e)
clang: error: unable to execute command: Aborted (core dumped)
clang: error: clang frontend command failed due to signal (use -v to see invocation)
Execution build compiler returned: 254
```
Code:
```c
int
main(void)
{
  volatile a_struct *a;
  volatile a_struct b;

  a = &b;
  *a = (a_struct){1,2,3,4};
  a->e.b4 = 'c';

  if (a->a != 1 || a->b != 2 || a->c != 3 || a->d != 4 || a->e.b4 != 'c')
    abort ();

  exit (0);
}
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (bi6c)

<details>
Compiler Explorer: https://godbolt.org/z/TE8s6x9Wd
```console
&lt;source&gt;:4:12: error: unknown type name 'a_struct'; did you mean 'struct'?
    4 |   volatile a_struct *a;
      |            ^~~~~~~~
      |            struct
&lt;source&gt;:4:12: error: declaration of anonymous struct must be a definition
&lt;source&gt;:4:3: warning: declaration does not declare anything [-Wmissing-declarations]
    4 |   volatile a_struct *a;
      |   ^~~~~~~~
&lt;source&gt;:5:12: error: unknown type name 'a_struct'; did you mean 'struct'?
    5 |   volatile a_struct b;
      |            ^~~~~~~~
      |            struct
warning: 'volatile' ignored on this declaration [-Wmissing-declarations]
&lt;source&gt;:7:3: error: use of undeclared identifier 'a'
    7 |   a = &amp;b;
      |   ^
&lt;source&gt;:7:8: error: use of undeclared identifier 'b'
    7 |   a = &amp;b;
      |        ^
&lt;source&gt;:8:9: error: use of undeclared identifier 'a_struct'; did you mean 'struct'?
    8 |   *a = (a_struct){1,2,3,4};
      |         ^~~~~~~~
      |         struct
clang: /root/llvm-project/clang/lib/AST/ExprClassification.cpp:57: Cl clang::Expr::ClassifyImpl(clang::ASTContext&amp;, clang::SourceLocation*) const: Assertion `isLValue()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics &lt;source&gt;
1.	&lt;source&gt;:8:18: current parser token '{'
2.	&lt;source&gt;:3:1: parsing function body 'main'
3.	&lt;source&gt;:3:1: in compound statement ('{}')
 #<!-- -->0 0x0000000003bffbb8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3bffbb8)
 #<!-- -->1 0x0000000003bfd8bc llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3bfd8bc)
 #<!-- -->2 0x0000000003b4adb8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007249c8042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007249c80969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007249c8042476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007249c80287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x00007249c802871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x00007249c8039e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x00000000076476f4 clang::Expr::ClassifyImpl(clang::ASTContext&amp;, clang::SourceLocation*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x76476f4)
#<!-- -->10 0x00000000076478bd clang::Expr::isModifiableLvalue(clang::ASTContext&amp;, clang::SourceLocation*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x76478bd)
#<!-- -->11 0x0000000006a16d1c CheckForModifiableLvalue(clang::Expr*, clang::SourceLocation, clang::Sema&amp;) SemaExpr.cpp:0:0
#<!-- -->12 0x0000000006a4bb17 clang::Sema::CheckAssignmentOperands(clang::Expr*, clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, clang::SourceLocation, clang::QualType, clang::BinaryOperatorKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6a4bb17)
#<!-- -->13 0x0000000006a4d0f2 clang::Sema::CreateBuiltinBinOp(clang::SourceLocation, clang::BinaryOperatorKind, clang::Expr*, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6a4d0f2)
#<!-- -->14 0x0000000006a4eae5 clang::Sema::BuildBinOp(clang::Scope*, clang::SourceLocation, clang::BinaryOperatorKind, clang::Expr*, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6a4eae5)
#<!-- -->15 0x0000000006a4ece4 clang::Sema::ActOnBinOp(clang::Scope*, clang::SourceLocation, clang::tok::TokenKind, clang::Expr*, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6a4ece4)
#<!-- -->16 0x00000000065a964d clang::Parser::ParseRHSOfBinaryExpression(clang::ActionResult&lt;clang::Expr*, true&gt;, clang::prec::Level) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x65a964d)
#<!-- -->17 0x00000000065accd9 clang::Parser::ParseExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x65accd9)
#<!-- -->18 0x000000000662fed9 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x662fed9)
#<!-- -->19 0x000000000662626e clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x662626e)
#<!-- -->20 0x00000000066271c0 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x66271c0)
#<!-- -->21 0x0000000006628085 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6628085)
#<!-- -->22 0x000000000662988a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x662988a)
#<!-- -->23 0x000000000653cbb3 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x653cbb3)
#<!-- -->24 0x000000000657138d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x657138d)
#<!-- -->25 0x000000000653094e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x653094e)
#<!-- -->26 0x0000000006531109 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6531109)
#<!-- -->27 0x0000000006538893 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6538893)
#<!-- -->28 0x000000000653975d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x653975d)
#<!-- -->29 0x0000000006539c00 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6539c00)
#<!-- -->30 0x000000000652bc53 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x652bc53)
#<!-- -->31 0x0000000004581488 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4581488)
#<!-- -->32 0x000000000483c3e9 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x483c3e9)
#<!-- -->33 0x00000000047bc12e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x47bc12e)
#<!-- -->34 0x000000000492752e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x492752e)
#<!-- -->35 0x0000000000cafdff cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xcafdff)
#<!-- -->36 0x0000000000ca799a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->37 0x00000000045c46d9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->38 0x0000000003b4b264 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3b4b264)
#<!-- -->39 0x00000000045c4ccf clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->40 0x000000000458af6d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x458af6d)
#<!-- -->41 0x000000000458c05d clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x458c05d)
#<!-- -->42 0x0000000004593415 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4593415)
#<!-- -->43 0x0000000000cacc43 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xcacc43)
#<!-- -->44 0x0000000000b81294 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xb81294)
#<!-- -->45 0x00007249c8029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->46 0x00007249c8029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->47 0x0000000000ca744e _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xca744e)
clang: error: clang frontend command failed with exit code 134 (use -v to see invocation)
ASM generation compiler returned: 134
&lt;source&gt;:4:12: error: unknown type name 'a_struct'; did you mean 'struct'?
    4 |   volatile a_struct *a;
      |            ^~~~~~~~
      |            struct
&lt;source&gt;:4:12: error: declaration of anonymous struct must be a definition
&lt;source&gt;:4:3: warning: declaration does not declare anything [-Wmissing-declarations]
    4 |   volatile a_struct *a;
      |   ^~~~~~~~
&lt;source&gt;:5:12: error: unknown type name 'a_struct'; did you mean 'struct'?
    5 |   volatile a_struct b;
      |            ^~~~~~~~
      |            struct
warning: 'volatile' ignored on this declaration [-Wmissing-declarations]
&lt;source&gt;:7:3: error: use of undeclared identifier 'a'
    7 |   a = &amp;b;
      |   ^
&lt;source&gt;:7:8: error: use of undeclared identifier 'b'
    7 |   a = &amp;b;
      |        ^
&lt;source&gt;:8:9: error: use of undeclared identifier 'a_struct'; did you mean 'struct'?
    8 |   *a = (a_struct){1,2,3,4};
      |         ^~~~~~~~
      |         struct
clang-20: /root/llvm-project/clang/lib/AST/ExprClassification.cpp:57: Cl clang::Expr::ClassifyImpl(clang::ASTContext&amp;, clang::SourceLocation*) const: Assertion `isLValue()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk-20241205/bin/clang-20 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -dumpdir /app/output.s- -disable-free -clear-ast-before-backend -main-file-name example.c -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -fno-verbose-asm -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debug-info-kind=constructor -dwarf-version=4 -debugger-tuning=gdb -fdebug-compilation-dir=/app -fcoverage-compilation-dir=/app -resource-dir /opt/compiler-explorer/clang-assertions-trunk-20241205/lib/clang/20 -internal-isystem /opt/compiler-explorer/clang-assertions-trunk-20241205/lib/clang/20/include -internal-isystem /usr/local/include -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/15.0.0/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -ferror-limit 19 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/example-15d809.o -x c &lt;source&gt;
1.	&lt;source&gt;:8:18: current parser token '{'
2.	&lt;source&gt;:3:1: parsing function body 'main'
3.	&lt;source&gt;:3:1: in compound statement ('{}')
 #<!-- -->0 0x0000000003bffbb8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk-20241205/bin/clang-20+0x3bffbb8)
 #<!-- -->1 0x0000000003bfd5ac SignalHandler(int) Signals.cpp:0:0
 #<!-- -->2 0x0000760562442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->3 0x00007605624969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->4 0x0000760562442476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->5 0x00007605624287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->6 0x000076056242871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->7 0x0000760562439e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->8 0x00000000076476f4 clang::Expr::ClassifyImpl(clang::ASTContext&amp;, clang::SourceLocation*) const (/opt/compiler-explorer/clang-assertions-trunk-20241205/bin/clang-20+0x76476f4)
 #<!-- -->9 0x00000000076478bd clang::Expr::isModifiableLvalue(clang::ASTContext&amp;, clang::SourceLocation*) const (/opt/compiler-explorer/clang-assertions-trunk-20241205/bin/clang-20+0x76478bd)
#<!-- -->10 0x0000000006a16d1c CheckForModifiableLvalue(clang::Expr*, clang::SourceLocation, clang::Sema&amp;) SemaExpr.cpp:0:0
#<!-- -->11 0x0000000006a4bb17 clang::Sema::CheckAssignmentOperands(clang::Expr*, clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, clang::SourceLocation, clang::QualType, clang::BinaryOperatorKind) (/opt/compiler-explorer/clang-assertions-trunk-20241205/bin/clang-20+0x6a4bb17)
#<!-- -->12 0x0000000006a4d0f2 clang::Sema::CreateBuiltinBinOp(clang::SourceLocation, clang::BinaryOperatorKind, clang::Expr*, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk-20241205/bin/clang-20+0x6a4d0f2)
#<!-- -->13 0x0000000006a4eae5 clang::Sema::BuildBinOp(clang::Scope*, clang::SourceLocation, clang::BinaryOperatorKind, clang::Expr*, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk-20241205/bin/clang-20+0x6a4eae5)
#<!-- -->14 0x0000000006a4ece4 clang::Sema::ActOnBinOp(clang::Scope*, clang::SourceLocation, clang::tok::TokenKind, clang::Expr*, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk-20241205/bin/clang-20+0x6a4ece4)
#<!-- -->15 0x00000000065a964d clang::Parser::ParseRHSOfBinaryExpression(clang::ActionResult&lt;clang::Expr*, true&gt;, clang::prec::Level) (/opt/compiler-explorer/clang-assertions-trunk-20241205/bin/clang-20+0x65a964d)
#<!-- -->16 0x00000000065accd9 clang::Parser::ParseExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-assertions-trunk-20241205/bin/clang-20+0x65accd9)
#<!-- -->17 0x000000000662fed9 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk-20241205/bin/clang-20+0x662fed9)
#<!-- -->18 0x000000000662626e clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk-20241205/bin/clang-20+0x662626e)
#<!-- -->19 0x00000000066271c0 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk-20241205/bin/clang-20+0x66271c0)
#<!-- -->20 0x0000000006628085 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk-20241205/bin/clang-20+0x6628085)
#<!-- -->21 0x000000000662988a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-assertions-trunk-20241205/bin/clang-20+0x662988a)
#<!-- -->22 0x000000000653cbb3 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk-20241205/bin/clang-20+0x653cbb3)
#<!-- -->23 0x000000000657138d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk-20241205/bin/clang-20+0x657138d)
#<!-- -->24 0x000000000653094e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk-20241205/bin/clang-20+0x653094e)
#<!-- -->25 0x0000000006531109 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk-20241205/bin/clang-20+0x6531109)
#<!-- -->26 0x0000000006538893 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk-20241205/bin/clang-20+0x6538893)
#<!-- -->27 0x000000000653975d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk-20241205/bin/clang-20+0x653975d)
#<!-- -->28 0x0000000006539c00 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk-20241205/bin/clang-20+0x6539c00)
#<!-- -->29 0x000000000652bc53 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk-20241205/bin/clang-20+0x652bc53)
#<!-- -->30 0x0000000004581488 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk-20241205/bin/clang-20+0x4581488)
#<!-- -->31 0x000000000483c3e9 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk-20241205/bin/clang-20+0x483c3e9)
#<!-- -->32 0x00000000047bc12e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk-20241205/bin/clang-20+0x47bc12e)
#<!-- -->33 0x000000000492752e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk-20241205/bin/clang-20+0x492752e)
#<!-- -->34 0x0000000000cafdff cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk-20241205/bin/clang-20+0xcafdff)
#<!-- -->35 0x0000000000ca799a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->36 0x0000000000cab8de clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk-20241205/bin/clang-20+0xcab8de)
#<!-- -->37 0x0000000000b81294 main (/opt/compiler-explorer/clang-assertions-trunk-20241205/bin/clang-20+0xb81294)
#<!-- -->38 0x0000760562429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->39 0x0000760562429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->40 0x0000000000ca744e _start (/opt/compiler-explorer/clang-assertions-trunk-20241205/bin/clang-20+0xca744e)
clang: error: unable to execute command: Aborted (core dumped)
clang: error: clang frontend command failed due to signal (use -v to see invocation)
Execution build compiler returned: 254
```
Code:
```c
int
main(void)
{
  volatile a_struct *a;
  volatile a_struct b;

  a = &amp;b;
  *a = (a_struct){1,2,3,4};
  a-&gt;e.b4 = 'c';

  if (a-&gt;a != 1 || a-&gt;b != 2 || a-&gt;c != 3 || a-&gt;d != 4 || a-&gt;e.b4 != 'c')
    abort ();

  exit (0);
}
```
</details>


---

### Comment 2 - shafik

Was this fuzzer generated?

---

### Comment 3 - shafik

Looks like this goes back to clang-12: https://godbolt.org/z/MznM68e8W

---

### Comment 4 - shafik

duplicate: https://github.com/llvm/llvm-project/issues/69874

---

