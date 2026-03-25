# Clang crash in clang/lib/Sema/SemaLookup.cpp:5407: const Sema::TypoExprState &clang::Sema::getTypoExprState(clang::TypoExpr *) const: Assertion `Entry != DelayedTypos.end() && "Failed to get the state for a TypoExpr!"' failed

**Author:** haoxintu
**URL:** https://github.com/llvm/llvm-project/issues/45891
**Status:** Closed
**Labels:** clang:frontend, bugzilla, crash-on-invalid, confirmed
**Closed Date:** 2025-06-13T10:45:43Z

## Body

|  |  |
| --- | --- |
| Bugzilla Link | [46546](https://llvm.org/bz46546) |
| Version | trunk |
| OS | Linux |
| CC | @zygoloid |

## Extended Description 
Hi, there.

This case, bug.cc, reduced by C-Reduce, makes clang-trunk crashed.

$cat bug.cc
a = b.c < enum , > :: template ~d < > [ e


$clang++ bug.cc
bug.cc:1:1: error: C++ requires a type specifier for all declarations
a = b.c < enum , > :: template ~d < > [ e
^
bug.cc:1:16: error: expected identifier or '{'
a = b.c < enum , > :: template ~d < > [ e
               ^
bug.cc:1:18: error: expected a type
a = b.c < enum , > :: template ~d < > [ e
                 ^
bug.cc:1:23: error: 'template' keyword not permitted here
a = b.c < enum , > :: template ~d < > [ e
                      ^~~~~~~~~
bug.cc:1:33: error: destructor name 'd' does not refer to a template
a = b.c < enum , > :: template ~d < > [ e
      ~~~~~~~~~~~~~~~           ^
bug.cc:1:5: error: use of undeclared identifier 'b'
a = b.c < enum , > :: template ~d < > [ e
    ^
clang-11: /home/haoxin/compilers/llvm-project/clang/lib/Sema/SemaLookup.cpp:5407: const Sema::TypoExprState &clang::Sema::getTypoExprState(clang::TypoExpr *) const: Assertion `Entry != DelayedTypos.end() && "Failed to get the state for a TypoExpr!"' failed.
PLEASE submit a bug report to https://bugs.llvm.org/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /home/haoxin/compilers/llvm-project/build/bin/clang-11 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -mrelax-all -disable-free -main-file-name bug.cc -mrelocation-model static -mframe-pointer=all -fmath-errno -fno-rounding-math -mconstructor-aliases -munwind-tables -target-cpu x86-64 -dwarf-column-info -fno-split-dwarf-inlining -debugger-tuning=gdb -resource-dir /home/haoxin/compilers/llvm-project/build/lib/clang/11.0.0 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/10/../../../../include/c++/10 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/10/../../../../include/x86_64-linux-gnu/c++/10 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/10/../../../../include/x86_64-linux-gnu/c++/10 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/10/../../../../include/c++/10/backward -internal-isystem /usr/local/include -internal-isystem /home/haoxin/compilers/llvm-project/build/lib/clang/11.0.0/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -fdeprecated-macro -fdebug-compilation-dir /home/haoxin/bitbucket/scg-backup-bitbucket/ice-test/4/creduce -ferror-limit 19 -fgnuc-version=4.2.1 -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -o /tmp/bug-276f11.o -x c++ bug.cc 
1.      <eof> parser at end of file
 #&#8203;0 0x00000000026f36c4 PrintStackTraceSignalHandler(void*) (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x26f36c4)
 #&#8203;1 0x00000000026f12be llvm::sys::RunSignalHandlers() (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x26f12be)
 #&#8203;2 0x00000000026f39e5 SignalHandler(int) (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x26f39e5)
 #&#8203;3 0x00007f6c74f0a890 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x12890)
 #&#8203;4 0x00007f6c73968e97 raise /build/glibc-OTsEL5/glibc-2.27/signal/../sysdeps/unix/sysv/linux/raise.c:51:0
 #&#8203;5 0x00007f6c7396a801 abort /build/glibc-OTsEL5/glibc-2.27/stdlib/abort.c:81:0
 #&#8203;6 0x00007f6c7395a39a __assert_fail_base /build/glibc-OTsEL5/glibc-2.27/assert/assert.c:89:0
 #&#8203;7 0x00007f6c7395a412 (/lib/x86_64-linux-gnu/libc.so.6+0x30412)
 #&#8203;8 0x0000000004392750 llvm::MapVector<clang::TypoExpr*, clang::Sema::TypoExprState, llvm::DenseMap<clang::TypoExpr*, unsigned int, llvm::DenseMapInfo<clang::TypoExpr*>, llvm::detail::DenseMapPair<clang::TypoExpr*, unsigned int> >, std::vector<std::pair<clang::TypoExpr*, clang::Sema::TypoExprState>, std::allocator<std::pair<clang::TypoExpr*, clang::Sema::TypoExprState> > > >::find(clang::TypoExpr* const&) const (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x4392750)
 #&#8203;9 0x00000000042a1d27 clang::TreeTransform<(anonymous namespace)::TransformTypos>::TransformExpr(clang::Expr*) (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x42a1d27)
#&#8203;10 0x00000000042ae5e1 clang::TreeTransform<(anonymous namespace)::TransformTypos>::TransformRecoveryExpr(clang::RecoveryExpr*) (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x42ae5e1)
#&#8203;11 0x000000000429e659 (anonymous namespace)::TransformTypos::RecursiveTransformLoop(clang::Expr*, bool&) (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x429e659)
#&#8203;12 0x0000000004279a62 clang::Sema::CorrectDelayedTyposInExpr(clang::Expr*, clang::VarDecl*, bool, llvm::function_ref<clang::ActionResult<clang::Expr*, true> (clang::Expr*)>) (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x4279a62)
#&#8203;13 0x0000000003e36cf7 clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult<clang::Expr*, true>) (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x3e36cf7)
#&#8203;14 0x0000000003e39f72 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x3e39f72)
#&#8203;15 0x0000000003e349b9 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x3e349b9)
#&#8203;16 0x0000000003e0d1b5 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x3e0d1b5)
#&#8203;17 0x0000000003e0b034 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x3e0b034)
#&#8203;18 0x0000000003def8fc clang::Parser::ParseDeclOrFunctionDefInternal(clang::Parser::ParsedAttributesWithRange&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x3def8fc)
#&#8203;19 0x0000000003def395 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::Parser::ParsedAttributesWithRange&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x3def395)
#&#8203;20 0x0000000003dee1bf clang::Parser::ParseExternalDeclaration(clang::Parser::ParsedAttributesWithRange&, clang::ParsingDeclSpec*) (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x3dee1bf)
#&#8203;21 0x0000000003dec28d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, bool) (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x3dec28d)
#&#8203;22 0x0000000003debad7 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&) (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x3debad7)
#&#8203;23 0x0000000003de76ee clang::ParseAST(clang::Sema&, bool, bool) (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x3de76ee)
#&#8203;24 0x0000000002ebc3f3 clang::FrontendAction::Execute() (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x2ebc3f3)
#&#8203;25 0x0000000002e539c3 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x2e539c3)
#&#8203;26 0x0000000002f67532 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x2f67532)
#&#8203;27 0x000000000095d060 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x95d060)
#&#8203;28 0x000000000095b12c ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&) (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x95b12c)
#&#8203;29 0x000000000095addf main (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x95addf)
#&#8203;30 0x00007f6c7394bb97 __libc_start_main /build/glibc-OTsEL5/glibc-2.27/csu/../csu/libc-start.c:344:0
#&#8203;31 0x0000000000957eba _start (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x957eba)
clang-11: error: unable to execute command: Aborted (core dumped)
clang-11: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 11.0.0 (https://github.com/llvm/llvm-project b9306fd042ce1c11d84f05d2124dfdc65b8331fe)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /home/haoxin/compilers/llvm-project/build/bin
clang-11: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang-11: note: diagnostic msg: /tmp/bug-c02c11.cpp
clang-11: note: diagnostic msg: /tmp/bug-c02c11.sh
clang-11: note: diagnostic msg: 

********************

$clang++ -v
clang version 11.0.0 (https://github.com/llvm/llvm-project b9306fd042ce1c11d84f05d2124dfdc65b8331fe)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /home/haoxin/compilers/llvm-project/build/bin
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/10
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/5
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/5.5.0
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/6
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/6.5.0
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/7
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/7.5.0
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/8
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/9
Selected GCC installation: /usr/lib/gcc/x86_64-linux-gnu/10
Candidate multilib: .;@m64
Selected multilib: .;@m64

## Comments

### Comment 1 - Endilll

Still crashing as of Clang 16: https://godbolt.org/z/xszc3v8vx

---

