# ICE: clang::Expr::ClassifyImpl(clang::ASTContext&, clang::SourceLocation*) const: Assertion `isPRValue()' failed.

**Author:** chengniansun
**URL:** https://github.com/llvm/llvm-project/issues/50821
**Status:** Closed
**Labels:** c, clang:frontend, bugzilla, confirmed, crash
**Closed Date:** 2025-04-03T18:56:04Z

## Body

|  |  |
| --- | --- |
| Bugzilla Link | [51479](https://llvm.org/bz51479) |
| Version | trunk |
| OS | All |
| CC | @DougGregor,@zygoloid |

## Extended Description 
$ clang-trunk -v
clang version 14.0.0 (https://github.com/llvm/llvm-project.git 16e8134e7c770849a6a1b9c79fdd548a0ef99190)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /home/cnsun/usr/bin
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/10
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/8
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/9
Selected GCC installation: /usr/lib/gcc/x86_64-linux-gnu/10
Candidate multilib: .;@m64
Candidate multilib: 32;@m32
Candidate multilib: x32;@mx32
Selected multilib: .;@m64

$ cat mutant.c
void foo(int m) __attribute__((overloadable, enable_if(m, "")));
void foo(int);
bar() { foo = bar; }

$ clang-trunk  mutant.c
mutant.c:3:1: warning: type specifier missing, defaults to 'int' [-Wimplicit-int]
bar() { foo = bar; }
^
clang-14: /tmp/tmp.m5wTsRPoA0-clang-builder/llvm-project/clang/lib/AST/ExprClassification.cpp:72: Cl clang::Expr::ClassifyImpl(clang::ASTContext&, clang::SourceLocation*) const: Assertion `isPRValue()' failed.
PLEASE submit a bug report to https://bugs.llvm.org/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /scratch/software/clang-trunk/bin/clang-14 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -mrelax-all --mrelax-relocations -disable-free -main-file-name mutant.c -mrelocation-model static -mframe-pointer=all -fmath-errno -fno-rounding-math -mconstructor-aliases -munwind-tables -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fcoverage-compilation-dir=/scratch/cnsun/workspace/perses-fuzzer/temp_testing_campaigns/default_finding_folder_c/crash_20210813_112802_0132/delta/perses_result -resource-dir /scratch/software/clang-trunk/lib/clang/14.0.0 -c-isystem . -c-isystem /usr/local/include/cmsith -internal-isystem /scratch/software/clang-trunk/lib/clang/14.0.0/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/10/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -fdebug-compilation-dir=/scratch/cnsun/workspace/perses-fuzzer/temp_testing_campaigns/default_finding_folder_c/crash_20210813_112802_0132/delta/perses_result -ferror-limit 19 -fgnuc-version=4.2.1 -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/mutant-81c2d2.o -x c mutant.c
1.	mutant.c:3:18: current parser token ';'
2.	mutant.c:3:7: parsing function body 'bar'
3.	mutant.c:3:7: in compound statement ('{}')
 #&#8203;0 0x00005640ab77b864 PrintStackTraceSignalHandler(void*) Signals.cpp:0:0
 #&#8203;1 0x00005640ab778f7e SignalHandler(int) Signals.cpp:0:0
 #&#8203;2 0x00007f8a877043c0 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x153c0)
 #&#8203;3 0x00007f8a871a318b raise /build/glibc-ZN95T4/glibc-2.31/signal/../sysdeps/unix/sysv/linux/raise.c:51:1
 #&#8203;4 0x00007f8a87182859 abort /build/glibc-ZN95T4/glibc-2.31/stdlib/abort.c:81:7
 #&#8203;5 0x00007f8a87182729 get_sysdep_segment_value /build/glibc-ZN95T4/glibc-2.31/intl/loadmsgcat.c:509:8
 #&#8203;6 0x00007f8a87182729 _nl_load_domain /build/glibc-ZN95T4/glibc-2.31/intl/loadmsgcat.c:970:34
 #&#8203;7 0x00007f8a87193f36 (/lib/x86_64-linux-gnu/libc.so.6+0x36f36)
 #&#8203;8 0x00005640ae8a8f18 clang::Expr::ClassifyImpl(clang::ASTContext&, clang::SourceLocation*) const (/scratch/software/clang-trunk/bin/clang-14+0x6ca9f18)
 #&#8203;9 0x00005640ae8a9111 clang::Expr::isModifiableLvalue(clang::ASTContext&, clang::SourceLocation*) const (/scratch/software/clang-trunk/bin/clang-14+0x6caa111)
#&#8203;10 0x00005640ade5dbf6 CheckForModifiableLvalue(clang::Expr*, clang::SourceLocation, clang::Sema&) SemaExpr.cpp:0:0
#&#8203;11 0x00005640ade917b5 clang::Sema::CheckAssignmentOperands(clang::Expr*, clang::ActionResult<clang::Expr*, true>&, clang::SourceLocation, clang::QualType) (/scratch/software/clang-trunk/bin/clang-14+0x62927b5)
#&#8203;12 0x00005640ade92a7b clang::Sema::CreateBuiltinBinOp(clang::SourceLocation, clang::BinaryOperatorKind, clang::Expr*, clang::Expr*) (/scratch/software/clang-trunk/bin/clang-14+0x6293a7b)
#&#8203;13 0x00005640ade939c4 clang::Sema::BuildBinOp(clang::Scope*, clang::SourceLocation, clang::BinaryOperatorKind, clang::Expr*, clang::Expr*) (/scratch/software/clang-trunk/bin/clang-14+0x62949c4)
#&#8203;14 0x00005640ade93e61 clang::Sema::ActOnBinOp(clang::Scope*, clang::SourceLocation, clang::tok::TokenKind, clang::Expr*, clang::Expr*) (/scratch/software/clang-trunk/bin/clang-14+0x6294e61)
#&#8203;15 0x00005640ada4951f clang::Parser::ParseRHSOfBinaryExpression(clang::ActionResult<clang::Expr*, true>, clang::prec::Level) (/scratch/software/clang-trunk/bin/clang-14+0x5e4a51f)
#&#8203;16 0x00005640ada48d9d clang::Parser::ParseExpression(clang::Parser::TypeCastState) (/scratch/software/clang-trunk/bin/clang-14+0x5e49d9d)
#&#8203;17 0x00005640adab34ee clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/scratch/software/clang-trunk/bin/clang-14+0x5eb44ee)
#&#8203;18 0x00005640adaaaad2 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributesWithRange&) (/scratch/software/clang-trunk/bin/clang-14+0x5eabad2)
#&#8203;19 0x00005640adaab4ba clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/scratch/software/clang-trunk/bin/clang-14+0x5eac4ba)
#&#8203;20 0x00005640adaac439 clang::Parser::ParseCompoundStatementBody(bool) (/scratch/software/clang-trunk/bin/clang-14+0x5ead439)
#&#8203;21 0x00005640adaae9fa clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/scratch/software/clang-trunk/bin/clang-14+0x5eaf9fa)
#&#8203;22 0x00005640ad9f3942 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/scratch/software/clang-trunk/bin/clang-14+0x5df4942)
#&#8203;23 0x00005640ada20530 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/scratch/software/clang-trunk/bin/clang-14+0x5e21530)
#&#8203;24 0x00005640ad9ee126 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributesWithRange&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/scratch/software/clang-trunk/bin/clang-14+0x5def126)
#&#8203;25 0x00005640ad9ee711 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributesWithRange&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (.part.0) Parser.cpp:0:0
#&#8203;26 0x00005640ad9f4f37 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributesWithRange&, clang::ParsingDeclSpec*) (/scratch/software/clang-trunk/bin/clang-14+0x5df5f37)
#&#8203;27 0x00005640ad9f64ce clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, bool) (/scratch/software/clang-trunk/bin/clang-14+0x5df74ce)
#&#8203;28 0x00005640ad9e8ca9 clang::ParseAST(clang::Sema&, bool, bool) (/scratch/software/clang-trunk/bin/clang-14+0x5de9ca9)
#&#8203;29 0x00005640ac9204c8 clang::CodeGenAction::ExecuteAction() (/scratch/software/clang-trunk/bin/clang-14+0x4d214c8)
#&#8203;30 0x00005640ac1b6799 clang::FrontendAction::Execute() (/scratch/software/clang-trunk/bin/clang-14+0x45b7799)
#&#8203;31 0x00005640ac14bf0e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/scratch/software/clang-trunk/bin/clang-14+0x454cf0e)
#&#8203;32 0x00005640ac295b90 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/scratch/software/clang-trunk/bin/clang-14+0x4696b90)
#&#8203;33 0x00005640a8faa1cd cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/scratch/software/clang-trunk/bin/clang-14+0x13ab1cd)
#&#8203;34 0x00005640a8fa6518 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#&#8203;35 0x00005640a8ed1059 main (/scratch/software/clang-trunk/bin/clang-14+0x12d2059)
#&#8203;36 0x00007f8a871840b3 __libc_start_main /build/glibc-ZN95T4/glibc-2.31/csu/../csu/libc-start.c:342:3
#&#8203;37 0x00005640a8fa608e _start (/scratch/software/clang-trunk/bin/clang-14+0x13a708e)
clang-14: error: unable to execute command: Aborted (core dumped)
clang-14: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 14.0.0 (https://github.com/llvm/llvm-project.git 16e8134e7c770849a6a1b9c79fdd548a0ef99190)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /home/cnsun/usr/bin
clang-14: note: diagnostic msg:
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang-14: note: diagnostic msg: /tmp/mutant-575972.c
clang-14: note: diagnostic msg: /tmp/mutant-575972.sh
clang-14: note: diagnostic msg:

********************

## Comments

### Comment 1 - AaronBallman

Note, this requires an assertions build to trigger the ICE.

---

### Comment 2 - llvmbot

@llvm/issue-subscribers-clang-frontend

---

### Comment 3 - AaronBallman

Closing as this issue was fixed in Clang 15: https://godbolt.org/z/P751frbhW

---

