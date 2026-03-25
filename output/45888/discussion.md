# Clang crash in clang/lib/Sema/SemaTemplate.cpp:397 Assertion `(IsDependent || !ObjectType->isIncompleteType() || ObjectType->castAs<TagType>()->isBeingDefined()) && "Caller should have completed object type"' failed

**Author:** haoxintu
**URL:** https://github.com/llvm/llvm-project/issues/45888
**Status:** Closed
**Labels:** clang:frontend, bugzilla, crash-on-invalid, confirmed
**Closed Date:** 2025-12-09T02:43:59Z

## Body

|  |  |
| --- | --- |
| Bugzilla Link | [46543](https://llvm.org/bz46543) |
| Version | trunk |
| OS | Linux |
| CC | @zygoloid |

## Extended Description 
Hi, there.

This case, bug.cc, reduced by C-Reduce, makes clang-trunk crashed.

$cat bug.cc
```cpp
a = const_cast <enum b > (c).d <
```


$clang++ bug.cc
```
bug.cc:1:1: error: C++ requires a type specifier for all declarations
a = const_cast <enum b > (c).d <
^
bug.cc:1:22: error: ISO C++ forbids forward references to 'enum' types
a = const_cast <enum b > (c).d <
                     ^
clang-11: /home/haoxin/compilers/llvm-project/clang/lib/Sema/SemaTemplate.cpp:397: bool clang::Sema::LookupTemplateName(clang::LookupResult &, clang::Scope *, clang::CXXScopeSpec &, clang::QualType, bool, bool &, clang::Sema::RequiredTemplateKind, clang::Sema::AssumedTemplateKind *, bool): Assertion `(IsDependent || !ObjectType->isIncompleteType() || ObjectType->castAs<TagType>()->isBeingDefined()) && "Caller should have completed object type"' failed.
PLEASE submit a bug report to https://bugs.llvm.org/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /home/haoxin/compilers/llvm-project/build/bin/clang-11 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -mrelax-all -disable-free -main-file-name bug.cc -mrelocation-model static -mframe-pointer=all -fmath-errno -fno-rounding-math -mconstructor-aliases -munwind-tables -target-cpu x86-64 -dwarf-column-info -fno-split-dwarf-inlining -debugger-tuning=gdb -resource-dir /home/haoxin/compilers/llvm-project/build/lib/clang/11.0.0 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/10/../../../../include/c++/10 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/10/../../../../include/x86_64-linux-gnu/c++/10 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/10/../../../../include/x86_64-linux-gnu/c++/10 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/10/../../../../include/c++/10/backward -internal-isystem /usr/local/include -internal-isystem /home/haoxin/compilers/llvm-project/build/lib/clang/11.0.0/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -fdeprecated-macro -fdebug-compilation-dir /home/haoxin/bitbucket/scg-backup-bitbucket/ice-test/clang-creduce -ferror-limit 19 -fgnuc-version=4.2.1 -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -o /tmp/bug-561a7f.o -x c++ bug.cc 
1.      bug.cc:1:30: current parser token 'd'
 #&#8203;0 0x00000000026f36c4 PrintStackTraceSignalHandler(void*) (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x26f36c4)
 #&#8203;1 0x00000000026f12be llvm::sys::RunSignalHandlers() (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x26f12be)
 #&#8203;2 0x00000000026f39e5 SignalHandler(int) (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x26f39e5)
 #&#8203;3 0x00007fe3b895f890 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x12890)
 #&#8203;4 0x00007fe3b73bde97 raise /build/glibc-OTsEL5/glibc-2.27/signal/../sysdeps/unix/sysv/linux/raise.c:51:0
 #&#8203;5 0x00007fe3b73bf801 abort /build/glibc-OTsEL5/glibc-2.27/stdlib/abort.c:81:0
 #&#8203;6 0x00007fe3b73af39a __assert_fail_base /build/glibc-OTsEL5/glibc-2.27/assert/assert.c:89:0
 #&#8203;7 0x00007fe3b73af412 (/lib/x86_64-linux-gnu/libc.so.6+0x30412)
 #&#8203;8 0x00000000044fb83e clang::Sema::LookupTemplateName(clang::LookupResult&, clang::Scope*, clang::CXXScopeSpec&, clang::QualType, bool, bool&, clang::Sema::RequiredTemplateKind, clang::Sema::AssumedTemplateKind*, bool) (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x44fb83e)
 #&#8203;9 0x00000000044f9a73 clang::Sema::isTemplateName(clang::Scope*, clang::CXXScopeSpec&, bool, clang::UnqualifiedId const&, clang::OpaquePtr<clang::QualType>, bool, clang::OpaquePtr<clang::TemplateName>&, bool&, bool) (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x44f9a73)
#&#8203;10 0x0000000003e4714a clang::Parser::ParseOptionalCXXScopeSpecifier(clang::CXXScopeSpec&, clang::OpaquePtr<clang::QualType>, bool, bool, bool*, bool, clang::IdentifierInfo**, bool, bool) (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x3e4714a)
#&#8203;11 0x0000000003e373c8 clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult<clang::Expr*, true>) (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x3e373c8)
#&#8203;12 0x0000000003e39f72 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x3e39f72)
#&#8203;13 0x0000000003e349b9 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x3e349b9)
#&#8203;14 0x0000000003e0d1b5 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x3e0d1b5)
#&#8203;15 0x0000000003e0b034 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x3e0b034)
#&#8203;16 0x0000000003def8fc clang::Parser::ParseDeclOrFunctionDefInternal(clang::Parser::ParsedAttributesWithRange&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x3def8fc)
#&#8203;17 0x0000000003def395 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::Parser::ParsedAttributesWithRange&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x3def395)
#&#8203;18 0x0000000003dee1bf clang::Parser::ParseExternalDeclaration(clang::Parser::ParsedAttributesWithRange&, clang::ParsingDeclSpec*) (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x3dee1bf)
#&#8203;19 0x0000000003dec28d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, bool) (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x3dec28d)
#&#8203;20 0x0000000003debad7 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&) (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x3debad7)
#&#8203;21 0x0000000003de76ee clang::ParseAST(clang::Sema&, bool, bool) (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x3de76ee)
#&#8203;22 0x0000000002ebc3f3 clang::FrontendAction::Execute() (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x2ebc3f3)
#&#8203;23 0x0000000002e539c3 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x2e539c3)
#&#8203;24 0x0000000002f67532 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x2f67532)
#&#8203;25 0x000000000095d060 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x95d060)
#&#8203;26 0x000000000095b12c ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&) (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x95b12c)
#&#8203;27 0x000000000095addf main (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x95addf)
#&#8203;28 0x00007fe3b73a0b97 __libc_start_main /build/glibc-OTsEL5/glibc-2.27/csu/../csu/libc-start.c:344:0
#&#8203;29 0x0000000000957eba _start (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x957eba)
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
clang-11: note: diagnostic msg: /tmp/bug-efa152.cpp
clang-11: note: diagnostic msg: /tmp/bug-efa152.sh
clang-11: note: diagnostic msg: 

********************
```

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

### Comment 1 - chfast

Confirmed in trunk: https://godbolt.org/z/53GeoKo53

---

### Comment 2 - Endilll

```
clang++: /root/llvm-project/clang/lib/Sema/SemaTemplate.cpp:404:
bool clang::Sema::LookupTemplateName(clang::LookupResult&, clang::Scope*, clang::CXXScopeSpec&, clang::QualType, bool, bool&, clang::Sema::RequiredTemplateKind, clang::Sema::AssumedTemplateKind*, bool):
Assertion `(IsDependent || !ObjectType->isIncompleteType() || !ObjectType->getAs<TagType>() || ObjectType->castAs<TagType>()->isBeingDefined()) && "Caller should have completed object type"' failed.

PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<source>:1:30: current parser token 'd'
 #0 0x000056477e29ac1a llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3a9ac1a)
 #1 0x000056477e29884c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3a9884c)
 #2 0x000056477e1e7020 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007fcc84887420 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x14420)
 #4 0x00007fcc8435400b raise (/lib/x86_64-linux-gnu/libc.so.6+0x4300b)
 #5 0x00007fcc84333859 abort (/lib/x86_64-linux-gnu/libc.so.6+0x22859)
 #6 0x00007fcc84333729 (/lib/x86_64-linux-gnu/libc.so.6+0x22729)
 #7 0x00007fcc84344fd6 (/lib/x86_64-linux-gnu/libc.so.6+0x33fd6)
 #8 0x0000564780e6dbee clang::Sema::LookupTemplateName(clang::LookupResult&, clang::Scope*, clang::CXXScopeSpec&, clang::QualType, bool, bool&, clang::Sema::RequiredTemplateKind, clang::Sema::AssumedTemplateKind*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x666dbee)
 #9 0x0000564780e6eae5 clang::Sema::isTemplateName(clang::Scope*, clang::CXXScopeSpec&, bool, clang::UnqualifiedId const&, clang::OpaquePtr<clang::QualType>, bool, clang::OpaquePtr<clang::TemplateName>&, bool&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x666eae5)
#10 0x00005647806a5f2b clang::Parser::ParseOptionalCXXScopeSpecifier(clang::CXXScopeSpec&, clang::OpaquePtr<clang::QualType>, bool, bool, bool*, bool, clang::IdentifierInfo**, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5ea5f2b)
#11 0x0000564780694cf8 clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult<clang::Expr*, true>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5e94cf8)
#12 0x000056478068ecb7 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5e8ecb7)
#13 0x0000564780691ca6 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5e91ca6)
#14 0x0000564780691d48 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5e91d48)
#15 0x000056478065bd6c clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5e5bd6c)
#16 0x00005647806722b7 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5e722b7)
#17 0x000056478063e728 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5e3e728)
#18 0x000056478063f0a6 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (.part.321) Parser.cpp:0:0
#19 0x000056478064394b clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5e4394b)
#20 0x0000564780645176 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5e45176)
#21 0x000056478064549f clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5e4549f)
#22 0x000056478063a1f3 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5e3a1f3)
#23 0x000056477f367560 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b67560)
#24 0x000056477ec892e9 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x44892e9)
#25 0x000056477ec1dc42 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x441dc42)
#26 0x000056477ed60c9f clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4560c9f)
#27 0x000056477ba22cc3 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x1222cc3)
#28 0x000056477ba1ade8 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#29 0x000056477ea912f9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#30 0x000056477e1e74a5 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x39e74a5)
#31 0x000056477ea93eee clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4293eee)
#32 0x000056477ea6060b clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x426060b)
#33 0x000056477ea611e3 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42611e3)
#34 0x000056477ea69fd6 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4269fd6)
#35 0x000056477ba20a8e clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x1220a8e)
#36 0x000056477b951cb1 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x1151cb1)
#37 0x00007fcc84335083 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x24083)
#38 0x000056477ba1a4da _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x121a4da)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

---

### Comment 3 - shafik

Fixed in clang-21: https://godbolt.org/z/x5xvM5EeE

---

