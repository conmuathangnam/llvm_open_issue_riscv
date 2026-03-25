# Clang crash in clang/lib/Sema/SemaTemplate.cpp:392 Assertion `SS.isEmpty() && "ObjectType and scope specifier cannot coexist"' failed

**Author:** haoxintu
**URL:** https://github.com/llvm/llvm-project/issues/45915
**Status:** Closed
**Labels:** clang:frontend, bugzilla, crash-on-invalid, confirmed
**Closed Date:** 2025-06-13T10:45:43Z

## Body

|  |  |
| --- | --- |
| Bugzilla Link | [46570](https://llvm.org/bz46570) |
| Version | trunk |
| OS | Linux |
| CC | @zygoloid |

## Extended Description 
Hi, there.

This case, bug.cc, reduced by C-Reduce, makes clang-trunk crashed.

Noted that clang can not emit any error messages about this case.

$cat bug.cc
```cpp
short g_volatile_ushort;
namespace a {
   int b = l_volatile_uwchar.a ::c ::~d<>
}
```

$clang++ bug.cc
clang-11: /home/haoxin/compilers/llvm-project/clang/lib/Sema/SemaTemplate.cpp:392: bool clang::Sema::LookupTemplateName(clang::LookupResult &, clang::Scope *, clang::CXXScopeSpec &, clang::QualType, bool, bool &, clang::Sema::RequiredTemplateKind, clang::Sema::AssumedTemplateKind *, bool): Assertion `SS.isEmpty() && "ObjectType and scope specifier cannot coexist"' failed.
PLEASE submit a bug report to https://bugs.llvm.org/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /home/haoxin/compilers/llvm-project/build/bin/clang-11 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -mrelax-all -disable-free -main-file-name bug.cc -mrelocation-model static -mframe-pointer=all -fmath-errno -fno-rounding-math -mconstructor-aliases -munwind-tables -target-cpu x86-64 -dwarf-column-info -fno-split-dwarf-inlining -debugger-tuning=gdb -resource-dir /home/haoxin/compilers/llvm-project/build/lib/clang/11.0.0 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/10/../../../../include/c++/10 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/10/../../../../include/x86_64-linux-gnu/c++/10 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/10/../../../../include/x86_64-linux-gnu/c++/10 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/10/../../../../include/c++/10/backward -internal-isystem /usr/local/include -internal-isystem /home/haoxin/compilers/llvm-project/build/lib/clang/11.0.0/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -fdeprecated-macro -fdebug-compilation-dir /home/haoxin/bitbucket/scg-backup-bitbucket/ice-test/0703-test/4/test_0/clang-creduce -ferror-limit 19 -fgnuc-version=4.2.1 -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -o /tmp/bug-39a191.o -x c++ bug.cc 
1.      bug.cc:4:1: current parser token '}'
2.      bug.cc:2:1: parsing namespace 'a'
 #&#8203;0 0x00000000026f36c4 PrintStackTraceSignalHandler(void*) (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x26f36c4)
 #&#8203;1 0x00000000026f12be llvm::sys::RunSignalHandlers() (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x26f12be)
 #&#8203;2 0x00000000026f39e5 SignalHandler(int) (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x26f39e5)
 #&#8203;3 0x00007ff73fb2c890 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x12890)
 #&#8203;4 0x00007ff73e58ae97 raise /build/glibc-OTsEL5/glibc-2.27/signal/../sysdeps/unix/sysv/linux/raise.c:51:0
 #&#8203;5 0x00007ff73e58c801 abort /build/glibc-OTsEL5/glibc-2.27/stdlib/abort.c:81:0
 #&#8203;6 0x00007ff73e57c39a __assert_fail_base /build/glibc-OTsEL5/glibc-2.27/assert/assert.c:89:0
 #&#8203;7 0x00007ff73e57c412 (/lib/x86_64-linux-gnu/libc.so.6+0x30412)
 #&#8203;8 0x00000000044fb7e1 clang::Sema::LookupTemplateName(clang::LookupResult&, clang::Scope*, clang::CXXScopeSpec&, clang::QualType, bool, bool&, clang::Sema::RequiredTemplateKind, clang::Sema::AssumedTemplateKind*, bool) (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x44fb7e1)
 #&#8203;9 0x00000000044f9a73 clang::Sema::isTemplateName(clang::Scope*, clang::CXXScopeSpec&, bool, clang::UnqualifiedId const&, clang::OpaquePtr<clang::QualType>, bool, clang::OpaquePtr<clang::TemplateName>&, bool&, bool) (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x44f9a73)
#&#8203;10 0x0000000004518b41 clang::Sema::ActOnTemplateName(clang::Scope*, clang::CXXScopeSpec&, clang::SourceLocation, clang::UnqualifiedId const&, clang::OpaquePtr<clang::QualType>, bool, clang::OpaquePtr<clang::TemplateName>&, bool) (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x4518b41)
#&#8203;11 0x00000000042b9bbe clang::TreeTransform<(anonymous namespace)::TransformTypos>::TransformTSIInObjectScope(clang::TypeLoc, clang::QualType, clang::NamedDecl*, clang::CXXScopeSpec&) (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x42b9bbe)
#&#8203;12 0x00000000042a5c5f clang::TreeTransform<(anonymous namespace)::TransformTypos>::TransformCXXPseudoDestructorExpr(clang::CXXPseudoDestructorExpr*) (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x42a5c5f)
#&#8203;13 0x000000000429e659 (anonymous namespace)::TransformTypos::RecursiveTransformLoop(clang::Expr*, bool&) (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x429e659)
#&#8203;14 0x0000000004279a62 clang::Sema::CorrectDelayedTyposInExpr(clang::Expr*, clang::VarDecl*, bool, llvm::function_ref<clang::ActionResult<clang::Expr*, true> (clang::Expr*)>) (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x4279a62)
#&#8203;15 0x000000000403a40e clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x403a40e)
#&#8203;16 0x0000000003e0d614 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x3e0d614)
#&#8203;17 0x0000000003e0b034 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x3e0b034)
#&#8203;18 0x0000000003def8fc clang::Parser::ParseDeclOrFunctionDefInternal(clang::Parser::ParsedAttributesWithRange&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x3def8fc)
#&#8203;19 0x0000000003def395 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::Parser::ParsedAttributesWithRange&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x3def395)
#&#8203;20 0x0000000003dee1bf clang::Parser::ParseExternalDeclaration(clang::Parser::ParsedAttributesWithRange&, clang::ParsingDeclSpec*) (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x3dee1bf)
#&#8203;21 0x0000000003e1f92b clang::Parser::ParseInnerNamespace(llvm::SmallVector<clang::Parser::InnerNamespaceInfo, 4u> const&, unsigned int, clang::SourceLocation&, clang::ParsedAttributes&, clang::BalancedDelimiterTracker&) (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x3e1f92b)
#&#8203;22 0x0000000003e1f258 clang::Parser::ParseNamespace(clang::DeclaratorContext, clang::SourceLocation&, clang::SourceLocation) (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x3e1f258)
#&#8203;23 0x0000000003e05d67 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::Parser::ParsedAttributesWithRange&, clang::SourceLocation*) (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x3e05d67)
#&#8203;24 0x0000000003ded86a clang::Parser::ParseExternalDeclaration(clang::Parser::ParsedAttributesWithRange&, clang::ParsingDeclSpec*) (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x3ded86a)
#&#8203;25 0x0000000003dec28d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, bool) (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x3dec28d)
#&#8203;26 0x0000000003de770d clang::ParseAST(clang::Sema&, bool, bool) (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x3de770d)
#&#8203;27 0x0000000002ebc3f3 clang::FrontendAction::Execute() (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x2ebc3f3)
#&#8203;28 0x0000000002e539c3 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x2e539c3)
#&#8203;29 0x0000000002f67532 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x2f67532)
#&#8203;30 0x000000000095d060 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x95d060)
#&#8203;31 0x000000000095b12c ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&) (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x95b12c)
#&#8203;32 0x000000000095addf main (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x95addf)
#&#8203;33 0x00007ff73e56db97 __libc_start_main /build/glibc-OTsEL5/glibc-2.27/csu/../csu/libc-start.c:344:0
#&#8203;34 0x0000000000957eba _start (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x957eba)
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
clang-11: note: diagnostic msg: /tmp/bug-7c4a71.cpp
clang-11: note: diagnostic msg: /tmp/bug-7c4a71.sh
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

Assertions build is still crashing: https://godbolt.org/z/ff4zaz476
Assertion:
```
clang++: /root/llvm-project/clang/lib/Sema/SemaTemplate.cpp:398:
bool clang::Sema::LookupTemplateName(clang::LookupResult&, clang::Scope*, clang::CXXScopeSpec&, clang::QualType, bool, bool&, clang::Sema::RequiredTemplateKind, clang::Sema::AssumedTemplateKind*, bool):
Assertion `SS.isEmpty() && "ObjectType and scope specifier cannot coexist"' failed.
```
Stack:
```
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<source>:4:1: current parser token '}'
2.	<source>:2:1: parsing namespace 'a'
 #0 0x000055a769ab3dbf llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3c12dbf)
 #1 0x000055a769ab1b2c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3c10b2c)
 #2 0x000055a7699fb1a8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007f5d89812420 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x14420)
 #4 0x00007f5d892df00b raise (/lib/x86_64-linux-gnu/libc.so.6+0x4300b)
 #5 0x00007f5d892be859 abort (/lib/x86_64-linux-gnu/libc.so.6+0x22859)
 #6 0x00007f5d892be729 (/lib/x86_64-linux-gnu/libc.so.6+0x22729)
 #7 0x00007f5d892cffd6 (/lib/x86_64-linux-gnu/libc.so.6+0x33fd6)
 #8 0x000055a76cba9200 clang::Sema::LookupTemplateName(clang::LookupResult&, clang::Scope*, clang::CXXScopeSpec&, clang::QualType, bool, bool&, clang::Sema::RequiredTemplateKind, clang::Sema::AssumedTemplateKind*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d08200)
 #9 0x000055a76cbaa7f3 clang::Sema::isTemplateName(clang::Scope*, clang::CXXScopeSpec&, bool, clang::UnqualifiedId const&, clang::OpaquePtr<clang::QualType>, bool, clang::OpaquePtr<clang::TemplateName>&, bool&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d097f3)
#10 0x000055a76cbafb76 clang::Sema::ActOnTemplateName(clang::Scope*, clang::CXXScopeSpec&, clang::SourceLocation, clang::UnqualifiedId const&, clang::OpaquePtr<clang::QualType>, bool, clang::OpaquePtr<clang::TemplateName>&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d0eb76)
#11 0x000055a76c8a446c clang::TreeTransform<(anonymous namespace)::TransformTypos>::TransformTSIInObjectScope(clang::TypeLoc, clang::QualType, clang::NamedDecl*, clang::CXXScopeSpec&) SemaExprCXX.cpp:0:0
#12 0x000055a76c8b1bcf clang::TreeTransform<(anonymous namespace)::TransformTypos>::TransformCXXPseudoDestructorExpr(clang::CXXPseudoDestructorExpr*) SemaExprCXX.cpp:0:0
#13 0x000055a76c89a2b2 (anonymous namespace)::TransformTypos::RecursiveTransformLoop(clang::Expr*, bool&) SemaExprCXX.cpp:0:0
#14 0x000055a76c89c422 clang::Sema::CorrectDelayedTyposInExpr(clang::Expr*, clang::VarDecl*, bool, llvm::function_ref<clang::ActionResult<clang::Expr*, true> (clang::Expr*)>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69fb422)
#15 0x000055a76c564fe5 clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66c3fe5)
#16 0x000055a76c227a0e clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6386a0e)
#17 0x000055a76c234d98 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6393d98)
#18 0x000055a76c201cc1 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6360cc1)
#19 0x000055a76c20257f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (.part.0) Parser.cpp:0:0
#20 0x000055a76c208f21 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6367f21)
#21 0x000055a76c25122d clang::Parser::ParseInnerNamespace(llvm::SmallVector<clang::Parser::InnerNamespaceInfo, 4u> const&, unsigned int, clang::SourceLocation&, clang::ParsedAttributes&, clang::BalancedDelimiterTracker&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x63b022d)
#22 0x000055a76c253d79 clang::Parser::ParseNamespace(clang::DeclaratorContext, clang::SourceLocation&, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x63b2d79)
#23 0x000055a76c23d750 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x639c750)
#24 0x000055a76c208954 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6367954)
#25 0x000055a76c209896 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6368896)
#26 0x000055a76c1fd65a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x635c65a)
#27 0x000055a76ad017c8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e607c8)
#28 0x000055a76a558f09 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x46b7f09)
#29 0x000055a76a4dd3d6 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x463c3d6)
#30 0x000055a76a63b9e6 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x479a9e6)
#31 0x000055a766f6f46d cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x10ce46d)
#32 0x000055a766f6b16a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#33 0x000055a76a34539d void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#34 0x000055a7699fb6b0 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3b5a6b0)
#35 0x000055a76a3459bf clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#36 0x000055a76a30d0ec clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x446c0ec)
#37 0x000055a76a30db7d clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x446cb7d)
#38 0x000055a76a315c5d clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4474c5d)
#39 0x000055a766f6d6ca clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x10cc6ca)
#40 0x000055a766e74235 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xfd3235)
#41 0x00007f5d892c0083 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x24083)
#42 0x000055a766f65f4e _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x10c4f4e)
```

---

