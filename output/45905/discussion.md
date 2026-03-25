# Crash:SemaLookup.cpp:2146 Assertion `(!isa<TagDecl>(LookupCtx) || LookupCtx->isDependentContext() || cast<TagDecl>(LookupCtx)->isCompleteDefinition() || cast<TagDecl>(LookupCtx)->isBeingDefined()) && "Declaration context must already be complete!"' failed

**Author:** haoxintu
**URL:** https://github.com/llvm/llvm-project/issues/45905
**Status:** Closed
**Labels:** clang:frontend, bugzilla, confirmed, crash-on-valid, regression:17
**Closed Date:** 2026-03-14T21:10:54Z

## Body

|  |  |
| --- | --- |
| Bugzilla Link | [46560](https://llvm.org/bz46560) |
| Version | trunk |
| OS | Linux |
| CC | @zygoloid |

## Extended Description 
Hi, there.

This case, bug.cc, reduced by C-Reduce, makes clang-trunk crashed.
```console
$ cat bug.cc
```
```cpp
int a = static_cast < enum b : > (c).template d
```
```console
$ clang++ bug.cc
bug.cc:1:32: error: expected a type
int a = static_cast < enum b : > (c).template d
                               ^
clang-11: /home/haoxin/compilers/llvm-project/clang/lib/Sema/SemaLookup.cpp:2146: bool clang::Sema::LookupQualifiedName(clang::LookupResult &, clang::DeclContext *, bool): Assertion `(!isa<TagDecl>(LookupCtx) || LookupCtx->isDependentContext() || cast<TagDecl>(LookupCtx)->isCompleteDefinition() || cast<TagDecl>(LookupCtx)->isBeingDefined()) && "Declaration context must already be complete!"' failed.
PLEASE submit a bug report to https://bugs.llvm.org/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /home/haoxin/compilers/llvm-project/build/bin/clang-11 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -mrelax-all -disable-free -main-file-name bug.cc -mrelocation-model static -mframe-pointer=all -fmath-errno -fno-rounding-math -mconstructor-aliases -munwind-tables -target-cpu x86-64 -dwarf-column-info -fno-split-dwarf-inlining -debugger-tuning=gdb -resource-dir /home/haoxin/compilers/llvm-project/build/lib/clang/11.0.0 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/10/../../../../include/c++/10 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/10/../../../../include/x86_64-linux-gnu/c++/10 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/10/../../../../include/x86_64-linux-gnu/c++/10 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/10/../../../../include/c++/10/backward -internal-isystem /usr/local/include -internal-isystem /home/haoxin/compilers/llvm-project/build/lib/clang/11.0.0/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -fdeprecated-macro -fdebug-compilation-dir /home/haoxin/bitbucket/scg-backup-bitbucket/ice-test/100000-3/creduce -ferror-limit 19 -fgnuc-version=4.2.1 -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -o /tmp/bug-f13bf7.o -x c++ bug.cc 
1.      <eof> parser at end of file
 #0 0x00000000026f36c4 PrintStackTraceSignalHandler(void*) (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x26f36c4)
 #1 0x00000000026f12be llvm::sys::RunSignalHandlers() (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x26f12be)
 #2 0x00000000026f39e5 SignalHandler(int) (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x26f39e5)
 #3 0x00007fb71ae9c890 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x12890)
 #4 0x00007fb7198fae97 raise /build/glibc-OTsEL5/glibc-2.27/signal/../sysdeps/unix/sysv/linux/raise.c:51:0
 #5 0x00007fb7198fc801 abort /build/glibc-OTsEL5/glibc-2.27/stdlib/abort.c:81:0
 #6 0x00007fb7198ec39a __assert_fail_base /build/glibc-OTsEL5/glibc-2.27/assert/assert.c:89:0
 #7 0x00007fb7198ec412 (/lib/x86_64-linux-gnu/libc.so.6+0x30412)
 #8 0x000000000437ffe3 clang::Sema::LookupQualifiedName(clang::LookupResult&, clang::DeclContext*, bool) (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x437ffe3)
 #9 0x00000000044f9e45 clang::Sema::LookupTemplateName(clang::LookupResult&, clang::Scope*, clang::CXXScopeSpec&, clang::QualType, bool, bool&, clang::Sema::RequiredTemplateKind, clang::Sema::AssumedTemplateKind*, bool) (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x44f9e45)
#10 0x00000000044f9a73 clang::Sema::isTemplateName(clang::Scope*, clang::CXXScopeSpec&, bool, clang::UnqualifiedId const&, clang::OpaquePtr<clang::QualType>, bool, clang::OpaquePtr<clang::TemplateName>&, bool&, bool) (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x44f9a73)
#11 0x0000000004518b41 clang::Sema::ActOnTemplateName(clang::Scope*, clang::CXXScopeSpec&, clang::SourceLocation, clang::UnqualifiedId const&, clang::OpaquePtr<clang::QualType>, bool, clang::OpaquePtr<clang::TemplateName>&, bool) (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x4518b41)
#12 0x0000000003e4a522 clang::Parser::ParseUnqualifiedId(clang::CXXScopeSpec&, clang::OpaquePtr<clang::QualType>, bool, bool, bool, bool, bool, clang::SourceLocation*, clang::UnqualifiedId&) (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x3e4a522)
#13 0x0000000003e377c8 clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult<clang::Expr*, true>) (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x3e377c8)
#14 0x0000000003e39f72 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x3e39f72)
#15 0x0000000003e349b9 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x3e349b9)
#16 0x0000000003e0d1b5 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x3e0d1b5)
#17 0x0000000003e0b034 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x3e0b034)
#18 0x0000000003def8fc clang::Parser::ParseDeclOrFunctionDefInternal(clang::Parser::ParsedAttributesWithRange&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x3def8fc)
#19 0x0000000003def395 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::Parser::ParsedAttributesWithRange&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x3def395)
#20 0x0000000003dee1bf clang::Parser::ParseExternalDeclaration(clang::Parser::ParsedAttributesWithRange&, clang::ParsingDeclSpec*) (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x3dee1bf)
#21 0x0000000003dec28d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, bool) (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x3dec28d)
#22 0x0000000003debad7 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&) (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x3debad7)
#23 0x0000000003de76ee clang::ParseAST(clang::Sema&, bool, bool) (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x3de76ee)
#24 0x0000000002ebc3f3 clang::FrontendAction::Execute() (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x2ebc3f3)
#25 0x0000000002e539c3 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x2e539c3)
#26 0x0000000002f67532 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x2f67532)
#27 0x000000000095d060 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x95d060)
#28 0x000000000095b12c ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&) (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x95b12c)
#29 0x000000000095addf main (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x95addf)
#30 0x00007fb7198ddb97 __libc_start_main /build/glibc-OTsEL5/glibc-2.27/csu/../csu/libc-start.c:344:0
#31 0x0000000000957eba _start (/home/haoxin/compilers/llvm-project/build/bin/clang-11+0x957eba)
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
clang-11: note: diagnostic msg: /tmp/bug-006ebd.cpp
clang-11: note: diagnostic msg: /tmp/bug-006ebd.sh
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
```

## Comments

### Comment 1 - Endilll

Assertions build is still crashing: https://godbolt.org/z/baEj6hG4v
Diagnostics:
```
<source>:1:32: error: expected a type
    1 | int a = static_cast < enum b : > (c).template d
      |                                ^
```
Assertion:
```
clang++: /root/llvm-project/clang/lib/Sema/SemaLookup.cpp:2417:
bool clang::Sema::LookupQualifiedName(clang::LookupResult&, clang::DeclContext*, bool): 
Assertion `(!isa<TagDecl>(LookupCtx) || LookupCtx->isDependentContext() || cast<TagDecl>(LookupCtx)->isCompleteDefinition() || cast<TagDecl>(LookupCtx)->isBeingDefined()) && "Declaration context must already be complete!"' failed.
```
Stack:
```
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<eof> parser at end of file
 #0 0x000055ae1fb85dbf llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3c12dbf)
 #1 0x000055ae1fb83b2c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3c10b2c)
 #2 0x000055ae1facd1a8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007f9fc20d5420 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x14420)
 #4 0x00007f9fc1ba200b raise (/lib/x86_64-linux-gnu/libc.so.6+0x4300b)
 #5 0x00007f9fc1b81859 abort (/lib/x86_64-linux-gnu/libc.so.6+0x22859)
 #6 0x00007f9fc1b81729 (/lib/x86_64-linux-gnu/libc.so.6+0x22729)
 #7 0x00007f9fc1b92fd6 (/lib/x86_64-linux-gnu/libc.so.6+0x33fd6)
 #8 0x000055ae22a55b51 clang::Sema::LookupQualifiedName(clang::LookupResult&, clang::DeclContext*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ae2b51)
 #9 0x000055ae22c7b2d2 clang::Sema::LookupTemplateName(clang::LookupResult&, clang::Scope*, clang::CXXScopeSpec&, clang::QualType, bool, bool&, clang::Sema::RequiredTemplateKind, clang::Sema::AssumedTemplateKind*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d082d2)
#10 0x000055ae22c7c7f3 clang::Sema::isTemplateName(clang::Scope*, clang::CXXScopeSpec&, bool, clang::UnqualifiedId const&, clang::OpaquePtr<clang::QualType>, bool, clang::OpaquePtr<clang::TemplateName>&, bool&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d097f3)
#11 0x000055ae22c81b76 clang::Sema::ActOnTemplateName(clang::Scope*, clang::CXXScopeSpec&, clang::SourceLocation, clang::UnqualifiedId const&, clang::OpaquePtr<clang::QualType>, bool, clang::OpaquePtr<clang::TemplateName>&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d0eb76)
#12 0x000055ae223559d4 clang::Parser::ParseUnqualifiedId(clang::CXXScopeSpec&, clang::OpaquePtr<clang::QualType>, bool, bool, bool, bool, bool, clang::SourceLocation*, clang::UnqualifiedId&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x63e29d4)
#13 0x000055ae2233dc70 clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult<clang::Expr*, true>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x63cac70)
#14 0x000055ae22336277 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x63c3277)
#15 0x000055ae2233938b clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x63c638b)
#16 0x000055ae2233943d clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x63c643d)
#17 0x000055ae222f99cd clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x63869cd)
#18 0x000055ae22306d98 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6393d98)
#19 0x000055ae222d3cc1 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6360cc1)
#20 0x000055ae222d457f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (.part.0) Parser.cpp:0:0
#21 0x000055ae222daf21 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6367f21)
#22 0x000055ae222db896 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6368896)
#23 0x000055ae222dbd34 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6368d34)
#24 0x000055ae222cf62a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x635c62a)
#25 0x000055ae20dd37c8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e607c8)
#26 0x000055ae2062af09 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x46b7f09)
#27 0x000055ae205af3d6 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x463c3d6)
#28 0x000055ae2070d9e6 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x479a9e6)
#29 0x000055ae1d04146d cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x10ce46d)
#30 0x000055ae1d03d16a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#31 0x000055ae2041739d void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#32 0x000055ae1facd6b0 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3b5a6b0)
#33 0x000055ae204179bf clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#34 0x000055ae203df0ec clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x446c0ec)
#35 0x000055ae203dfb7d clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x446cb7d)
#36 0x000055ae203e7c5d clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4474c5d)
#37 0x000055ae1d03f6ca clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x10cc6ca)
#38 0x000055ae1cf46235 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xfd3235)
#39 0x00007f9fc1b83083 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x24083)
#40 0x000055ae1d037f4e _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x10c4f4e)
```

---

### Comment 2 - alexfh

Here an example reduced from production code: https://gcc.godbolt.org/z/c9aGGfGTo

Unfortunately, it's not as simple as the example here, but at least it can be compiled with non-assertions build of clang without errors.

```
struct b;
template <class a>
a d(int);
template <class a>
decltype(d<a>(0)) aa();
template <bool, class = void>
struct e;
template <class a>
struct e<true, a> {
  typedef a c;
};
template <bool g>
using h = e<g>;
template <int i>
struct j {
  static const int k = i;
  constexpr operator int() { return i; }
};
template <class a>
a ab();
template <class n, class... o>
decltype(aa<n>()(o()...)) p(n, o...);
template <decltype(sizeof(int)), class>
struct ac;
template <class...>
struct ad {};
template <int ae, class... a>
typename ac<ae, ad<a...>>::c af(ad<a...>);
template <decltype(sizeof(int))>
struct q {};
template <class>
struct ag {
  template <decltype(sizeof(int))>
  using w = q<1>;
};
template <decltype(sizeof(int)), decltype(sizeof(int)) r>
using s =
    typename __make_integer_seq<ag, decltype(sizeof(int)), r>::template w<r>;
template <class a, a>
struct u {};
template <class a, a ah>
using ai = __make_integer_seq<u, a, ah>;
template <int ah, decltype(sizeof(int)) r = 0>
struct aj {
  typedef s<ah, r> c;
};
template <class...>
struct ak;
template <class>
struct al;
template <class... a>
struct al<ad<a...>> : j<sizeof...(a)> {};
template <class a>
constexpr decltype(sizeof(int)) am = al<a>::k;
template <decltype(sizeof(int)) ae, class... an>
struct ac<ae, ak<an...>> {
  using c = __type_pack_element<ae, an...>;
};
template <decltype(sizeof(int)) ae, class... a>
struct ac<ae, ad<a...>> {
  using c = ac<ae, ak<a...>>::c;
};
template <class... a>
ad<a...> ao(a...);
template <class ap, class aq, decltype(sizeof(int))... ar>
void as(ap, aq, q<ar...>) noexcept(noexcept(p(ab<ap>(), af<ar>(aq())...)));
template <class ap, class aq>
void apply(ap,
           aq) noexcept(noexcept(as(ab<ap>(), aq(), typename aj<am<aq>>::c{})));
template <class, class = void>
struct au;
template <class, class = void>
struct ay;
template <class aw>
struct ay<aw, typename e<sizeof(az(aw()))>::c> : j<true> {};
template <class aw>
struct au<aw, typename e<ay<aw>::k>::c> {
  typedef decltype(az(aw())) c;
};
template <class>
struct ba;
template <class aw>
struct bb : ba<typename au<aw>::c>::template bb<aw> {};
template <class>
struct bc {
  typedef void c;
};
template <class, class = void>
struct bf {};
template <class aw>
struct bf<aw, typename bc<typename ba<typename au<aw>::c>::bf>::c>
    : ba<typename au<aw>::c>::bf {};
template <decltype(sizeof(int)) bd, class aw>
auto af(aw) -> decltype(ba<typename au<aw>::c>::template af<bd>(aw()));
template <int, class bg, int... bh>
void bi(bg bj, u<int, bh...>) {
  (bj.template operator()<bh>(), ...);
}
template <int bk, int bd, class bg>
void bl(bg bj) {
  bi<bk>(bj, ai<int, bd>());
}
template <int bd, class bg>
void bm(bg bj) {
  bl<0, bd>(bj);
}
template <class aw, class bg>
struct bn {
  template <int bd>
  void operator()() {
    decltype(af<bd>(aw())) bo = af<bd>(aw());
    f.template operator()<bd>(bo);
  }
  aw t;
  bg f;
};
template <class aw, class bg>
void bp(bg bj, aw) {
  bm<bb<aw>::k>(bn{aw(), bj});
}
template <class bq>
struct br : bq {};
template <>
struct br<void> {};
template <int bd>
struct bs {
  template <class bq>
  using c = br<typename bs<bd - 1>::template c<bq>>;
};
template <>
struct bs<1> {
  template <class bq>
  using c = bq;
};
template <int bt, int bu>
constexpr int bv() {
  return bt - bu;
}
template <int bd, class bq = void>
using bw = bs<bv<40, bd>()>::template c<bq>;
bw<0> bx;
enum by {};
template <class bz>
struct ca {
  ca(bz *);
  template <class aw, class = h<bf<aw>{}>>
  void cb(aw bj, bw<3>) {
    bp(cc{}, bj);
  }
  struct cc {
    template <int, class cf>
    void operator()(cf bj) {
      cg(ch, bj);
    }
    int ch;
    bz cg;
  };
  int cd;
  template <class aw>
  void cb(aw bj, bw<6>) {
    cd << bj;
  }
};
template <class ci = void>
struct cj {
  template <class aw>
  void operator()(int bj, aw p2) {
    ci cg;
    ca(&cg).cb(p2, bx);
  }
};
template <>
struct cj<> : cj<cj<>> {};
template <class aw>
struct cl {
  friend void operator<<(int bj, cl p2) { p2.ce(bj, p2.cm); }
  aw cm;
  cj<> ce;
};
cl<b> cn(b);
template <class co>
struct x {
  co cp;
};
template <int, class...>
struct cq {};
template <bool cr, class cs, class... ct>
struct ba<cq<cr, cs, ct...>> {
  using bf = j<1>;
  template <int bd>
  struct be : ac<bd, ::ad<ct...>>::c {};
  template <class>
  struct bb : j<sizeof...(ct)> {};
  template <decltype(sizeof(int)) bd, class aw>
  static auto af(aw) -> decltype(aw().*be<bd>::cp);
};
template <typename>
struct da;
struct z;
auto cx() { return static_cast<void (*)(da<z>)>(nullptr); }
template <typename...>
using cv = decltype(cx());
template <typename aw>
struct cw {
  static auto db(aw bj) { return bj.db(); }
};
template <typename, int, typename>
struct de;
template <typename aw, int df, typename... y>
struct de<aw, df, void (*)(y...)> : y... {
  auto db() {
    auto [dc] = static_cast<aw &>(*this);
    return ao(0, dc);
  }
};
template <int dg = 1>
struct dj {
  template <typename...>
  struct di : de<z, dg, cv<>> {};
};
template <class, class = void>
bool is_supported_ptr;
template <class aw>
bool is_supported_ptr<aw, decltype(aw().~dk())>;
template <typename aw>
struct da : cw<aw> {
  friend void operator<<(int bj, aw p2) {
    apply(
        [&](auto... args) {
          ([](auto v) { (void)is_supported_ptr<decltype(v)>; }(args), ...);
        },
        da::db(p2));
  }
};
enum ResourceType : int;
struct z : dj<>::di<> {
  ResourceType resource_type;
};
struct b {
  void op1() { 0 << cn(*this); }
  z chunk_parameters;
  friend cq<true, int, x<decltype(&b::chunk_parameters)>> az(b);
};
```

---

### Comment 3 - alexfh

@cor3ntin @mizvekov could you take a look? I wonder how deep of a problem this is. This is a long-standing issue, but it manifests when compiling some code with many dependencies in our codebase, as a result preventing a build of a large chunk of the codebase with assertions-enabled clang and potentially hiding newer problems in Clang.

---

### Comment 4 - Endilll

This is a lot of code to reason through looking at 1- and 2-letter identifiers. Can you reduce it further or re-reduce it without replacing original identifiers?

Also, looking at the original example, I'm not sure why this is labeled as `crash-on-valid`, because the code in the description is clearly invalid.

> Unfortunately, it's not as simple as the example here, but at least it can be compiled with non-assertions build of clang without errors.

I'm not sure if the output of Clang 20.1 can be trusted if it crashes with assertions enables, because internal invariants are not upheld. Even if its output was successfully deployed. I'm not sure I'd count this as a regression in 21. 

---

### Comment 5 - Endilll

Oh, now that I posted the comment, I see that you marked it as `crash-on-valid`. It's not easy to see from your reduction either way, but at least this part is clear now.

---

### Comment 6 - Endilll

Seems to be a regression in Clang 17: https://gcc.godbolt.org/z/8M71rjaKj

---

### Comment 7 - alexfh

I marked it crash-on-valid, since the original code seems to be valid (at least it compiles, but it's huge and non-shareable for other reasons). I can try to get a reduced example without renaming, but that's gonna take some time.

---

### Comment 8 - shafik

@yuanfang-chen git bisect points to 632dd6a4ca0036009febd55d95e4e1f93eeb412c that caused this regression

---

### Comment 9 - mizvekov

I think it's two unrelated issues, with the same assertion.

The original bug is a crash-on-invalid that is unrelated to the crash-on-valid clang-17 regression posted by Alex.

It is yet plausible there is a third bug with the same assertion:

Looking at the assert, it doesn't handle TagDecls which may have a out-of-date definition status, which is relevant for modules.

It's not immediately obvious if that is unneeded here; It's possible fixing this doesn't fix anything in practice because we may have always tried looking for a definition before getting here.

But otherwise, getting a reproducer for this might be quite difficult.

---

### Comment 10 - shafik

@alexfh I closed this b/c the original code does not crash anymore, your example is different. Can you open a new bug for that one assuming there are no duplicates since.

---

