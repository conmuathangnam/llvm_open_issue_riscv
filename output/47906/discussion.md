# clang-11 crashes on the thrill project

**Author:** yurivict
**URL:** https://github.com/llvm/llvm-project/issues/47906
**Status:** Closed
**Labels:** duplicate, c++11, clang:frontend, bugzilla, crash-on-invalid, confirmed
**Closed Date:** 2025-06-03T15:08:36Z

## Body

|  |  |
| --- | --- |
| Bugzilla Link | [48562](https://llvm.org/bz48562) |
| Version | trunk |
| OS | FreeBSD |
| CC | @DougGregor,@zygoloid |

## Extended Description 
Please see the attached crash log.

The crash occurred on the Thrill project https://github.com/thrill/thrill rev.12c5b59bca66df93b66628b3829027bd0f110dd9

## Comments

### Comment 1 - yurivict

https://people.freebsd.org/~yuri/llvm-bug-misc-thrill.tar.bz2

---

### Comment 2 - Endilll

Still crashing as of post-17 trunk: https://godbolt.org/z/5YbG4rqe1
Reduced by C-Reduce:
```cpp
template <int dim> class Triangulation : public Subscriptor {};

namespace GridGenerator {
template <int dim>
void hyper_shell(Triangulation<dim> &tria, const int &center,
                 const double inner_radius, const double outer_radius,
                 const unsigned int n_cells = 0, bool colorize = false);
}

template class PolarManifold : public ChartManifold {
public:
  PolarManifold(const int center = Point0);
  const int center;
};

namespace GridGenerator {
template <>
void hyper_shell(Triangulation<3> &tria, const int &p,
                  const double inner_radius, const double outer_radius,
                  const unsigned int n_cells, const bool colorize) {
  Triangulation<3> tmp;
  hyper_shell(tmp, p, inner_radius, outer_radius, 2);
}
}
```
Crash:
```
<source>:1:49: error: expected class name
    1 | template <int dim> class Triangulation : public Subscriptor {};
      |                                                 ^
<source>:10:30: error: class cannot be defined in an explicit instantiation; if this declaration is meant to be a class definition, remove the 'template' keyword
   10 | template class PolarManifold : public ChartManifold {
      | ~~~~~~~~                     ^
<source>:10:39: error: unknown class name 'ChartManifold'; did you mean 'PolarManifold'?
   10 | template class PolarManifold : public ChartManifold {
      |                                       ^~~~~~~~~~~~~
      |                                       PolarManifold
<source>:10:16: note: 'PolarManifold' declared here
   10 | template class PolarManifold : public ChartManifold {
      |                ^
<source>:10:39: error: base class has incomplete type
   10 | template class PolarManifold : public ChartManifold {
      |                                ~~~~~~~^~~~~~~~~~~~~
<source>:10:16: note: definition of 'PolarManifold' is not complete until the closing '}'
   10 | template class PolarManifold : public ChartManifold {
      |                ^
<source>:12:36: error: use of undeclared identifier 'Point0'
   12 |   PolarManifold(const int center = Point0);
      |                                    ^

Didn't find this decl on its identifier's chain!
UNREACHABLE executed at /root/llvm-project/clang/lib/Sema/IdentifierResolver.cpp:85!

PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<source>:24:1: current parser token '}'
2.	<source>:16:1: parsing namespace 'GridGenerator'
3.	<source>:20:68: parsing function body 'GridGenerator::hyper_shell<3>'
 #0 0x00000000036af848 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x36af848)
 #1 0x00000000036ad50c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x36ad50c)
 #2 0x00000000035f6928 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007fe4c72cf420 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x14420)
 #4 0x00007fe4c6d9200b raise (/lib/x86_64-linux-gnu/libc.so.6+0x4300b)
 #5 0x00007fe4c6d71859 abort (/lib/x86_64-linux-gnu/libc.so.6+0x22859)
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
#38 0x00007fe4c6d73083 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x24083)
#39 0x0000000000bce91e _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xbce91e)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
```

---

### Comment 3 - Endilll

While reducing original crash, I found another one that is reproducible on post-17 trunk as well: https://godbolt.org/z/zc4Tn79xr
Reduced by C-Reduce:
```cpp
template <int dim>
struct Triangulation {};

namespace GridGenerator {
template < int dim >
void hyper_shell(Triangulation< dim > &, const int &center, double, double, unsigned = 0, bool = 0);
}

PolarManifold(const int center = Point0);
const int center;

namespace GridGenerator {

template <>
void hyper_shell(Triangulation<3> &, const int &p, double, double, unsigned, bool) {
  Triangulation<3> tmp;
  hyper_shell(tmpp, 0, 0, 0);
}
```
Crash:
```
<source>:9:34: error: use of undeclared identifier 'Point0'
    9 | PolarManifold(const int center = Point0);
      |                                  ^
<source>:9:1: error: a type specifier is required for all declarations
    9 | PolarManifold(const int center = Point0);
      | ^
<source>:10:11: error: default initialization of an object of const type 'const int'
   10 | const int center;
      |           ^     
      |                  = 0
<source>:17:15: error: use of undeclared identifier 'tmpp'; did you mean 'tmp'?
   17 |   hyper_shell(tmpp, 0, 0, 0);
      |               ^~~~
      |               tmp
<source>:16:20: note: 'tmp' declared here
   16 |   Triangulation<3> tmp;
      |                    ^

clang++: /root/llvm-project/clang/lib/Sema/IdentifierResolver.cpp:229:
void clang::IdentifierResolver::RemoveDecl(clang::NamedDecl*):
Assertion `D == Ptr && "Didn't find this decl on its identifier's chain!"' failed.

PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<eof> parser at end of file
2.	<source>:12:1: parsing namespace 'GridGenerator'
3.	<source>:15:84: parsing function body 'GridGenerator::hyper_shell<3>'
 #0 0x00000000036af848 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x36af848)
 #1 0x00000000036ad50c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x36ad50c)
 #2 0x00000000035f6928 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007f16ea7a5420 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x14420)
 #4 0x00007f16ea26800b raise (/lib/x86_64-linux-gnu/libc.so.6+0x4300b)
 #5 0x00007f16ea247859 abort (/lib/x86_64-linux-gnu/libc.so.6+0x22859)
 #6 0x00007f16ea247729 (/lib/x86_64-linux-gnu/libc.so.6+0x22729)
 #7 0x00007f16ea258fd6 (/lib/x86_64-linux-gnu/libc.so.6+0x33fd6)
 #8 0x0000000005f22480 clang::IdentifierResolver::RemoveDecl(clang::NamedDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5f22480)
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
#38 0x00007f16ea249083 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x24083)
#39 0x0000000000bce91e _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xbce91e)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
```

---

### Comment 4 - llvmbot

@llvm/issue-subscribers-clang-frontend

---

### Comment 5 - shafik

Looks like a duplicate of https://github.com/llvm/llvm-project/issues/54279 which has a much smaller reproducer.

---

### Comment 6 - mordante

> Looks like a duplicate of [#54279](https://github.com/llvm/llvm-project/issues/54279) which has a much smaller reproducer.

It's indeed a duplicate of that bug.

---

