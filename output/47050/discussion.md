# clang-11 crashes on the deal.ii project

**Author:** yurivict
**URL:** https://github.com/llvm/llvm-project/issues/47050
**Status:** Closed
**Labels:** duplicate, c++, clang:frontend, bugzilla, confirmed, crash-on-valid
**Closed Date:** 2025-06-03T15:08:24Z

## Body

|  |  |
| --- | --- |
| Bugzilla Link | [47706](https://llvm.org/bz47706) |
| Version | 11.0 |
| OS | FreeBSD |
| CC | @DougGregor,@DimitryAndric,@riccibruno,@zygoloid |

## Extended Description 
Crash dump:
* https://people.freebsd.org/~yuri/grid_generator-5bb0cd.cpp
* https://people.freebsd.org/~yuri/grid_generator-5bb0cd.sh

Project:
dealii: https://github.com/dealii/dealii version 9.1.1

Encountered while bjuilding the FreeBSD port math/deal.ii r542423 on FreeBSD 13-CURRENT amd64.

## Comments

### Comment 1 - DimitryAndric

This still asserts as of 2021-08-17 with llvmorg-14-init-1622-g8f8f9260a95f:

Starting program: /home/dim/obj/llvm-llvmorg-14-init-1622-g8f8f9260a95f-freebsd14-amd64-ninja-clang-dbg-1/bin/clang -cc1 -triple x86_64-- -S -O1 -std=c++17 grid_generator-min.cpp
Didn't find this decl on its identifier's chain!
UNREACHABLE executed at /share/dim/src/llvm/llvm-project/clang/lib/Sema/IdentifierResolver.cpp:82!

Program received signal SIGABRT, Aborted.
thr_kill () at thr_kill.S:4
4       thr_kill.S: No such file or directory.
(gdb) bt
#&#8203;0  thr_kill () at thr_kill.S:4
#&#8203;1  0x000000080b8c2fc4 in __raise (s=s@entry=6) at /usr/src/lib/libc/gen/raise.c:52
#&#8203;2  0x000000080b9778e9 in abort () at /usr/src/lib/libc/stdlib/abort.c:67
#&#8203;3  0x0000000005380c30 in llvm::llvm_unreachable_internal (msg=0xdf2c28 "Didn't find this decl on its identifier's chain!", file=0xe6a192 "/share/dim/src/llvm/llvm-project/clang/lib/Sema/IdentifierResolver.cpp", line=82) at /share/dim/src/llvm/llvm-project/llvm/lib/Support/ErrorHandling.cpp:213
#&#8203;4  0x00000000090255b9 in clang::IdentifierResolver::IdDeclInfo::RemoveDecl (this=0x80c8057c8, D=0x80c802bd0) at /share/dim/src/llvm/llvm-project/clang/lib/Sema/IdentifierResolver.cpp:82
#&#8203;5  0x0000000009025d54 in clang::IdentifierResolver::RemoveDecl (this=0x80c27a4b8, D=0x80c802bd0) at /share/dim/src/llvm/llvm-project/clang/lib/Sema/IdentifierResolver.cpp:225
#&#8203;6  0x00000000092f334d in clang::Sema::ActOnPopScope (this=0x80c279800, Loc=..., S=0x80c22bfc0) at /share/dim/src/llvm/llvm-project/clang/lib/Sema/SemaDecl.cpp:2003
#&#8203;7  0x0000000008f157c7 in clang::Parser::ExitScope (this=0x80c333800) at /share/dim/src/llvm/llvm-project/clang/lib/Parse/Parser.cpp:420
#&#8203;8  0x0000000008f206aa in clang::Parser::ParseScope::Exit (this=0x7fffffff8e00) at /share/dim/src/llvm/llvm-project/clang/include/clang/Parse/Parser.h:1107
#&#8203;9  0x0000000008fc3b40 in clang::Parser::ParseFunctionStatementBody (this=0x80c333800, Decl=0x80c802f38, BodyScope=...) at /share/dim/src/llvm/llvm-project/clang/lib/Parse/ParseStmt.cpp:2284
#&#8203;10 0x0000000008f1ace8 in clang::Parser::ParseFunctionDefinition (this=0x80c333800, D=..., TemplateInfo=..., LateParsedAttrs=0x7fffffff9300) at /share/dim/src/llvm/llvm-project/clang/lib/Parse/Parser.cpp:1377
#&#8203;11 0x0000000008ff3161 in clang::Parser::ParseSingleDeclarationAfterTemplate (this=0x80c333800, Context=clang::DeclaratorContext::File, TemplateInfo=..., DiagsFromTParams=..., DeclEnd=..., AccessAttrs=..., AS=clang::AS_none) at /share/dim/src/llvm/llvm-project/clang/lib/Parse/ParseTemplate.cpp:345
#&#8203;12 0x0000000008ff1d94 in clang::Parser::ParseTemplateDeclarationOrSpecialization (this=0x80c333800, Context=clang::DeclaratorContext::File, DeclEnd=..., AccessAttrs=..., AS=clang::AS_none) at /share/dim/src/llvm/llvm-project/clang/lib/Parse/ParseTemplate.cpp:169
#&#8203;13 0x0000000008ff1784 in clang::Parser::ParseDeclarationStartingWithTemplate (this=0x80c333800, Context=clang::DeclaratorContext::File, DeclEnd=..., AccessAttrs=..., AS=clang::AS_none) at /share/dim/src/llvm/llvm-project/clang/lib/Parse/ParseTemplate.cpp:46
#&#8203;14 0x0000000008f58c8b in clang::Parser::ParseDeclaration (this=0x80c333800, Context=clang::DeclaratorContext::File, DeclEnd=..., attrs=..., DeclSpecStart=0x0) at /share/dim/src/llvm/llvm-project/clang/lib/Parse/ParseDecl.cpp:1729
#&#8203;15 0x0000000008f1880d in clang::Parser::ParseExternalDeclaration (this=0x80c333800, attrs=..., DS=0x0) at /share/dim/src/llvm/llvm-project/clang/lib/Parse/Parser.cpp:909
#&#8203;16 0x0000000008f2a143 in clang::Parser::ParseInnerNamespace (this=0x80c333800, InnerNSs=..., index=0, InlineLoc=..., attrs=..., Tracker=...) at /share/dim/src/llvm/llvm-project/clang/lib/Parse/ParseDeclCXX.cpp:246
#&#8203;17 0x0000000008f29b5a in clang::Parser::ParseNamespace (this=0x80c333800, Context=clang::DeclaratorContext::File, DeclEnd=..., InlineLoc=...) at /share/dim/src/llvm/llvm-project/clang/lib/Parse/ParseDeclCXX.cpp:224
#&#8203;18 0x0000000008f58e25 in clang::Parser::ParseDeclaration (this=0x80c333800, Context=clang::DeclaratorContext::File, DeclEnd=..., attrs=..., DeclSpecStart=0x0) at /share/dim/src/llvm/llvm-project/clang/lib/Parse/ParseDecl.cpp:1742
#&#8203;19 0x0000000008f1880d in clang::Parser::ParseExternalDeclaration (this=0x80c333800, attrs=..., DS=0x0) at /share/dim/src/llvm/llvm-project/clang/lib/Parse/Parser.cpp:909
#&#8203;20 0x0000000008f1707b in clang::Parser::ParseTopLevelDecl (this=0x80c333800, Result=..., IsFirstDecl=false) at /share/dim/src/llvm/llvm-project/clang/lib/Parse/Parser.cpp:720
#&#8203;21 0x0000000008f122f4 in clang::ParseAST (S=..., PrintStats=false, SkipFunctionBodies=false) at /share/dim/src/llvm/llvm-project/clang/lib/Parse/ParseAST.cpp:158
#&#8203;22 0x000000000663a79c in clang::ASTFrontendAction::ExecuteAction (this=0x80c221380) at /share/dim/src/llvm/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1058
#&#8203;23 0x0000000006801a60 in clang::CodeGenAction::ExecuteAction (this=0x80c221380) at /share/dim/src/llvm/llvm-project/clang/lib/CodeGen/CodeGenAction.cpp:1044
#&#8203;24 0x000000000663a19c in clang::FrontendAction::Execute (this=0x80c221380) at /share/dim/src/llvm/llvm-project/clang/lib/Frontend/FrontendAction.cpp:951
#&#8203;25 0x0000000006573266 in clang::CompilerInstance::ExecuteAction (this=0x80c31f000, Act=...) at /share/dim/src/llvm/llvm-project/clang/lib/Frontend/CompilerInstance.cpp:974
#&#8203;26 0x00000000067f33ae in clang::ExecuteCompilerInvocation (Clang=0x80c31f000) at /share/dim/src/llvm/llvm-project/clang/lib/FrontendTool/ExecuteCompilerInvocation.cpp:278
#&#8203;27 0x000000000363eaf3 in cc1_main (Argv=..., Argv0=0x7fffffffe768 "/home/dim/obj/llvm-llvmorg-14-init-1622-g8f8f9260a95f-freebsd14-amd64-ninja-clang-dbg-1/bin/clang", MainAddr=0x362fd90 <GetExecutablePath(char const*, bool)>) at /share/dim/src/llvm/llvm-project/clang/tools/driver/cc1_main.cpp:246
#&#8203;28 0x000000000363122b in ExecuteCC1Tool (ArgV=...) at /share/dim/src/llvm/llvm-project/clang/tools/driver/driver.cpp:338
#&#8203;29 0x0000000003630475 in main (Argc=8, Argv=0x7fffffffe3e0) at /share/dim/src/llvm/llvm-project/clang/tools/driver/driver.cpp:409
(gdb) frame 4
#&#8203;4  0x00000000090255b9 in clang::IdentifierResolver::IdDeclInfo::RemoveDecl (this=0x80c8057c8, D=0x80c802bd0) at /share/dim/src/llvm/llvm-project/clang/lib/Sema/IdentifierResolver.cpp:82
82        llvm_unreachable("Didn't find this decl on its identifier's chain!");
(gdb) print Decls
$1 = {<llvm::SmallVectorImpl<clang::NamedDecl*>> = {<llvm::SmallVectorTemplateBase<clang::NamedDecl*, true>> = {<llvm::SmallVectorTemplateCommon<clang::NamedDecl*, void>> = {<llvm::SmallVectorBase<unsigned int>> = {BeginX = 0x80c8057d8, Size = 0, Capacity = 2}, <No data fields>}, static TakesParamByValue = true}, <No data fields>}, <llvm::SmallVectorStorage<clang::NamedDecl*, 2>> = {InlineElts = " (\200\f\b\000\000\000P&\200\f\b\000\000"}, <No data fields>}

It seems as if Decls is just empty on entry of IdDeclInfo::RemoveDecl() ?

---

### Comment 2 - DimitryAndric

Minimized test case (this took quite a while to reduce!):

// clang -cc1 -triple x86_64-- -S -O1 -std=c++17 grid_generator-min.cpp
template <int> struct a {};
namespace b {
template <int c>
void e(a<c> &, const int &center, double, double, unsigned, bool = 0);
template <int c> void d(a<c> &, double, double, double, unsigned, bool);
} // namespace b
struct g {
  g(int center = 0);
  int center;
};
namespace b {
template <> void e(a<3> &, const int &f, double, double, unsigned, bool) {
  a<3> h;
  e(h, 0, 0, 0, 0);
}
template <> void d(a<0> &, double, double, double, unsigned, bool);
} // namespace b

Bisection shows this bug was introduced with https://reviews.llvm.org/rGa5569f089844:

commit a5569f089844209dbea2e3241460173d7b6b1420 (HEAD)
Author: Richard Smith <richard@metafoo.co.uk>
Date:   Thu Jul 9 14:57:30 2020 -0700

    Push parameters into the local instantiation scope before instantiating
    a default argument.
    
    Default arguments can (after recent language changes) refer to
    parameters of the same function. Make sure they're added to the local
    instantiation scope before transforming a default argument so that we
    can remap such references to them properly.

---

### Comment 3 - DimitryAndric

The actual error is:

% clang -cc1 -triple x86_64-unknown-freebsd13.0 -emit-obj -disable-free -main-file-name grid_generator.cc -mrelocation-model pic -pic-level 2 -mframe-pointer=all -fno-rounding-math -mconstructor-aliases -munwind-tables -target-cpu x86-64 -fno-split-dwarf-inlining -debugger-tuning=gdb -D BOOST_NO_AUTO_PTR -O2 -Wall -Wextra -Woverloaded-virtual -Wpointer-arith -Wsign-compare -Wsuggest-override -Wswitch -Wsynth -Wwrite-strings -Wno-deprecated-declarations -Wno-psabi -Wfloat-conversion -Wno-unsupported-friend -Wno-undefined-var-template -Wno-unused-local-typedefs -pedantic -std=c++17 -fdeprecated-macro -ferror-limit 19 -funroll-loops -stack-protector 2 -fgnuc-version=4.2.1 -fcxx-exceptions -fexceptions -vectorize-loops -vectorize-slp -faddrsig -x c++ grid_generator-5bb0cd.cpp
Didn't find this decl on its identifier's chain!
UNREACHABLE executed at /usr/src/contrib/llvm-project/clang/lib/Sema/IdentifierResolver.cpp:82!
PLEASE submit a bug report to https://bugs.freebsd.org/submit/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: clang -cc1 -triple x86_64-unknown-freebsd13.0 -emit-obj -disable-free -main-file-name grid_generator.cc -mrelocation-model pic -pic-level 2 -mframe-pointer=all -fno-rounding-math -mconstructor-aliases -munwind-tables -target-cpu x86-64 -fno-split-dwarf-inlining -debugger-tuning=gdb -D BOOST_NO_AUTO_PTR -O2 -Wall -Wextra -Woverloaded-virtual -Wpointer-arith -Wsign-compare -Wsuggest-override -Wswitch -Wsynth -Wwrite-strings -Wno-deprecated-declarations -Wno-psabi -Wfloat-conversion -Wno-unsupported-friend -Wno-undefined-var-template -Wno-unused-local-typedefs -pedantic -std=c++17 -fdeprecated-macro -ferror-limit 19 -funroll-loops -stack-protector 2 -fgnuc-version=4.2.1 -fcxx-exceptions -fexceptions -vectorize-loops -vectorize-slp -faddrsig -x c++ grid_generator-5bb0cd.cpp
1.      /usr/ports/math/deal.ii/work/dealii-9.1.1/source/grid/grid_generator.cc:4037:3: current parser token 'template'
2.      /usr/ports/math/deal.ii/work/dealii-9.1.1/source/grid/grid_generator.cc:31:1 <Spelling=/usr/ports/math/deal.ii/work/.build/include/deal.II/base/config.h:355:32>: parsing namespace 'dealii'
3.      /usr/ports/math/deal.ii/work/dealii-9.1.1/source/grid/grid_generator.cc:34:1: parsing namespace 'dealii::GridGenerator'
4.      /usr/ports/math/deal.ii/work/dealii-9.1.1/source/grid/grid_generator.cc:3896:3: parsing function body 'dealii::GridGenerator::hyper_shell<3>'
#&#8203;0 0x00000000041d47ce PrintStackTrace /usr/src/contrib/llvm-project/llvm/lib/Support/Unix/Signals.inc:564:13
#&#8203;1 0x00000000041d2a25 RunSignalHandlers /usr/src/contrib/llvm-project/llvm/lib/Support/Signals.cpp:69:18
#&#8203;2 0x00000000041d4ec0 SignalHandler /usr/src/contrib/llvm-project/llvm/lib/Support/Unix/Signals.inc:0:3
#&#8203;3 0x000000080568fb90 handle_signal /usr/src/lib/libthr/thread/thr_sig.c:0:3

---

### Comment 4 - Endilll

Still crashing as of post-17 trunk: https://godbolt.org/z/1a44v1Yx1
```
Didn't find this decl on its identifier's chain!
UNREACHABLE executed at /root/llvm-project/clang/lib/Sema/IdentifierResolver.cpp:85!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<source>:16:1: current parser token 'template'
2.	<source>:11:1: parsing namespace 'b'
3.	<source>:12:74: parsing function body 'b::e<3>'
 #0 0x00000000036af848 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x36af848)
 #1 0x00000000036ad50c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x36ad50c)
 #2 0x00000000035f6928 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007f2a5e0e4420 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x14420)
 #4 0x00007f2a5dba700b raise (/lib/x86_64-linux-gnu/libc.so.6+0x4300b)
 #5 0x00007f2a5db86859 abort (/lib/x86_64-linux-gnu/libc.so.6+0x22859)
 #6 0x0000000003601bda (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3601bda)
 #7 0x0000000005f21e37 clang::IdentifierResolver::IdDeclInfo::RemoveDecl(clang::NamedDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5f21e37)
 #8 0x0000000005f223ec clang::IdentifierResolver::RemoveDecl(clang::NamedDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5f223ec)
 #9 0x0000000006157b92 clang::Sema::ActOnPopScope(clang::SourceLocation, clang::Scope*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6157b92)
#10 0x0000000005e00095 clang::Parser::ExitScope() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5e00095)
#11 0x0000000005ee0671 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5ee0671)
#12 0x0000000005e0de21 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5e0de21)
#13 0x0000000005efa0a0 clang::Parser::ParseSingleDeclarationAfterTemplate(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo const&, clang::ParsingDeclRAIIObject&, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5efa0a0)
#14 0x0000000005eff598 clang::Parser::ParseTemplateDeclarationOrSpecialization(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (.part.0) ParseTemplate.cpp:0:0
#15 0x0000000005eff853 clang::Parser::ParseDeclarationStartingWithTemplate(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5eff853)
#16 0x0000000005e3d6a5 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5e3d6a5)
#17 0x0000000005e084a4 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5e084a4)
#18 0x0000000005e50ad4 clang::Parser::ParseInnerNamespace(llvm::SmallVector<clang::Parser::InnerNamespaceInfo, 4u> const&, unsigned int, clang::SourceLocation&, clang::ParsedAttributes&, clang::BalancedDelimiterTracker&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5e50ad4)
#19 0x0000000005e534bf clang::Parser::ParseNamespace(clang::DeclaratorContext, clang::SourceLocation&, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5e534bf)
#20 0x0000000005e3d6ee clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5e3d6ee)
#21 0x0000000005e084a4 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5e084a4)
#22 0x0000000005e0938d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5e0938d)
#23 0x0000000005dfd6ea clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5dfd6ea)
#24 0x00000000049074d8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49074d8)
#25 0x0000000004170419 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4170419)
#26 0x00000000040f104e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x40f104e)
#27 0x000000000424f82e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x424f82e)
#28 0x0000000000bd6c10 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xbd6c10)
#29 0x0000000000bcee3a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#30 0x0000000003f526d9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#31 0x00000000035f6dd4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x35f6dd4)
#32 0x0000000003f52ccf clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#33 0x0000000003f1b055 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f1b055)
#34 0x0000000003f1babd clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f1babd)
#35 0x0000000003f239e5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f239e5)
#36 0x0000000000bd4a1c clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xbd4a1c)
#37 0x0000000000ad1421 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xad1421)
#38 0x00007f2a5db88083 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x24083)
#39 0x0000000000bce91e _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xbce91e)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
```

---

### Comment 5 - llvmbot

@llvm/issue-subscribers-clang-frontend

---

### Comment 6 - shafik

Looks possibly related to: https://github.com/llvm/llvm-project/issues/54279

---

### Comment 7 - shafik

@zygoloid it looks like your commit was pointed to as well. I pinged you in the related bug I mentioned in my previous comment on the other bug.

---

### Comment 8 - mordante

> Looks possibly related to: [#54279](https://github.com/llvm/llvm-project/issues/54279)

It indeed is a duplicate of that issue.

---

