# [fuzz] Assertion `VDecl->isLinkageValid()' failed.

**Author:** llvmbot
**URL:** https://github.com/llvm/llvm-project/issues/22203

## Body

|  |  |
| --- | --- |
| Bugzilla Link | [21829](https://llvm.org/bz21829) |
| Version | trunk |
| OS | Linux |
| Blocks | llvm/llvm-project#23431  |
| Attachments | [test case](https://user-images.githubusercontent.com/60944935/143750509-5b30533e-a396-4d79-b6ad-3012c8d876d3.gz) |
| Reporter | LLVM Bugzilla Contributor |
| CC | @majnemer,@rnk |

## Extended Description 


Fuzzing discovered that the attached test case, when given as input to

  clang -fno-crash-diagnostics -std=c++11 -xc++ -c -emit-llvm

causes this assertion failure:

clang-3.6: tools/clang/lib/Sema/SemaDecl.cpp:8656: void clang::Sema::AddInitializerToDecl(clang::Decl *, clang::Expr *, bool, bool): Assertion `VDecl->isLinkageValid()' failed.

## Comments

### Comment 1 - kcc

mentioned in issue llvm/llvm-project#23431

---

### Comment 2 - rnk

Still repros, cool test case. :)

---

### Comment 3 - 991901f3-cc14-4404-b340-165691b62a58

Less crazy test case:
auto g = [] { void g(); };

---

### Comment 4 - llvmbot

@llvm/issue-subscribers-c-11

---

### Comment 5 - AaronBallman

Both the original test case and the reduced test case still reproduce on Clang 16 with the following backtrace:
```
<source>:1:20: error: redefinition of 'g' as different kind of symbol
    1 | auto g = [] { void g(); };
      |                    ^
<source>:1:6: note: previous definition is here
    1 | auto g = [] { void g(); };
      |      ^
clang++: /root/llvm-project/clang/lib/Sema/SemaDecl.cpp:12755: bool clang::Sema::DeduceVariableDeclarationType(clang::VarDecl*, bool, clang::Expr*): Assertion `VDecl->isLinkageValid()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<source>:1:26: current parser token ';'
 #0 0x00005556bc01624f llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3c2824f)
 #1 0x00005556bc013fbc llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3c25fbc)
 #2 0x00005556bbf5d3b8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007f4fa0e5e420 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x14420)
 #4 0x00007f4fa092b00b raise (/lib/x86_64-linux-gnu/libc.so.6+0x4300b)
 #5 0x00007f4fa090a859 abort (/lib/x86_64-linux-gnu/libc.so.6+0x22859)
 #6 0x00007f4fa090a729 (/lib/x86_64-linux-gnu/libc.so.6+0x22729)
 #7 0x00007f4fa091bfd6 (/lib/x86_64-linux-gnu/libc.so.6+0x33fd6)
 #8 0x00005556beaae3a3 clang::Sema::DeduceVariableDeclarationType(clang::VarDecl*, bool, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66c03a3)
 #9 0x00005556bead0c5f clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66e2c5f)
#10 0x00005556be7934ae clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x63a54ae)
#11 0x00005556be7a0838 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x63b2838)
#12 0x00005556be76d721 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x637f721)
#13 0x00005556be76dfdf clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (.part.0) Parser.cpp:0:0
#14 0x00005556be774981 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6386981)
#15 0x00005556be7752f6 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x63872f6)
#16 0x00005556be775794 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6387794)
#17 0x00005556be76908a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x637b08a)
#18 0x00005556bd269598 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e7b598)
#19 0x00005556bcab9c79 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x46cbc79)
#20 0x00005556bca3e316 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4650316)
#21 0x00005556bcb9c0b6 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x47ae0b6)
#22 0x00005556b94bf16d cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x10d116d)
#23 0x00005556b94bae6a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#24 0x00005556bc8a6c1d void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#25 0x00005556bbf5d8c0 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3b6f8c0)
#26 0x00005556bc8a723f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#27 0x00005556bc86ea1c clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4480a1c)
#28 0x00005556bc86f4ad clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x44814ad)
#29 0x00005556bc87758d clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x448958d)
#30 0x00005556b94bd3ca clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x10cf3ca)
#31 0x00005556b93c42b5 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xfd62b5)
#32 0x00007f4fa090c083 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x24083)
#33 0x00005556b94b5c4e _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x10c7c4e)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

---

### Comment 6 - llvmbot

@llvm/issue-subscribers-clang-frontend

---

### Comment 7 - Endilll

Another reproducer from a Conor Hoekstra's CppNorth 2023 talk that is still crashing as of post-16 trunk. Reduced by C-Reduce:
```
namespace combinators {
auto _b = [] {
```
Original example:
```
#include <algorithm>
#include <ranges>
#include <vector>

#include <functional>

namespace combinators {

/////////////////
// combinators //
/////////////////

// B (The Bluebird)
auto _b = [](auto f, auto g) { return [=](auto x) { return f(g(x)); }; };

// BB (The Bluebird^2)
auto _bb = [](auto f, auto g, auto h) { return [=](auto x) { return f(g(h(x))); }; };

// BBB (The Bluebird^3)
auto _bbb = [](auto f, auto g, auto h, auto i) { return [=](auto x) { return f(g(h(i(x)))); }; };

// B1 (The Blackbird)
auto _b1 = [](auto f, auto g) { return [=](auto x, auto y) { return f(g(x, y)); }; };

// C (The Cardinal) aka `flip` in Haskell
auto _c = [](auto f) { return [=](auto x, auto y) { return f(y, x); }; };

// K (Kestrel)
auto _l_ = [](auto x, auto y) { return x; };

// KI
auto _r_ = [](auto x, auto y) { return y; };

// Phi (The Phoenix)
auto _phi = [](auto f, auto g, auto h) { return [=](auto x) { return g(f(x), h(x)); }; };

// Phi1 (The Pheasant)
auto _phi1_ = [](auto f, auto g, auto h) { return [=](auto x, auto y) { return g(f(x, y), h(x, y)); }; };

// Psi (The Psi Bird)
auto _psi = [](auto f, auto g) { return [=](auto x, auto y) { return f(g(x), g(y)); }; };

// W (The Warbler)
auto _w = [](auto f) { return [=](auto x) { return f(x, x); }; };

/////////////////////////////////////////////
// more convenient binary/unary operations //
/////////////////////////////////////////////

auto _eq    = [](auto x) { return [x](auto y) { return x == y; }; };
auto _eq_   = std::equal_to{};
auto _neq_  = std::not_equal_to{};
auto _lt    = [](auto x) { return [x](auto y) { return x > y; }; };
auto lt_    = [](auto x) { return [x](auto y) { return y < x; }; };
auto _lt_   = std::less{};
auto _gte   = [](auto x) { return [x](auto y) { return x >= y; }; };
auto _plus  = [](auto x) { return [x](auto y) { return x + y; }; };
auto _plus_ = std::plus{};
auto _mul   = [](auto x) { return [x](auto y) { return x * y; }; };
auto _mul_  = std::multiplies{};
auto _sub   = [](auto x) { return [x](auto y) { return x - y; }; };
auto sub_   = [](auto x) { return [x](auto y) { return y - x; }; };
auto _sub_  = std::minus{};
auto _mod   = [](auto x) { return [x](auto y) { return x % y; }; };
auto mod_   = [](auto x) { return [x](auto y) { return y % x; }; };
auto _or_   = std::logical_or{};
auto _and_  = std::logical_and{};
auto _not   = std::logical_not{};
auto _min_  = [](auto a, auto b) { return std::min(a, b); };
auto _max_  = [](auto a, auto b) { return std::max(a, b); };
auto _fst   = [](auto t) { return std::get<0>(t); };
auto _snd   = [](auto t) { return std::get<1>(t); };

auto _odd  = [](auto x) { return (x % 2) == 1; };
auto _even = [](auto x) { return (x % 2) == 0; };

}  // namespace combinators

using namespace std::views;
using namespace combinators;

// pure standard c++
auto sushi_for_two(std::vector<int> sushi) {
    return 2 * std::ranges::max(sushi
        | chunk_by(std::equal_to{})
        | transform(std::ranges::distance)
        | adjacent_transform<2>([](auto a, auto b) { return std::min(a, b); }));
}

// with blackbird
auto sushi_for_two2(std::vector<int> sushi) {
    return 2 * std::ranges::max(sushi
        | chunk_by(_eq_)
        | transform(std::ranges::distance)
        | adjacent_transform<2>(_min_));
}

auto main() -> int {

    fmt::print("{}\n", sushi_for_two ({2, 2, 2, 1, 1, 2, 2}));       // 4
    fmt::print("{}\n", sushi_for_two ({1, 2, 1, 2, 1, 2}));          // 2
    fmt::print("{}\n", sushi_for_two ({2, 2, 1, 1, 1, 2, 2, 2, 2})); // 6
    fmt::print("{}\n", sushi_for_two2({2, 2, 2, 1, 1, 2, 2}));       // 4
    fmt::print("{}\n", sushi_for_two2({1, 2, 1, 2, 1, 2}));          // 2
    fmt::print("{}\n", sushi_for_two2({2, 2, 1, 1, 1, 2, 2, 2, 2})); // 6

    return 0;
}
```


---

### Comment 8 - Endilll

Crash looks the same as Aaron's above: https://godbolt.org/z/T41Gac6r5
```
<source>:2:15: error: expected '}'
auto _b = [] {
              ^
<source>:2:14: note: to match this '{'
auto _b = [] {
             ^
clang++: /home/tonik/llvm-project/clang/lib/Sema/SemaDecl.cpp:12705: bool clang::Sema::DeduceVariableDeclarationType(clang::VarDecl *, bool, clang::Expr *): Assertion `VDecl->isLinkageValid()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-16.0.0-assert/bin/clang++ -gdwarf-4 -g -o /tmp/compiler-explorer-compiler2023622-723-ws3zg5.sv9b/output.s -mllvm --x86-asm-syntax=intel -S --gcc-toolchain=/opt/compiler-explorer/gcc-12.2.0 -fcolor-diagnostics -fno-crash-diagnostics -std=c++11 <source>
1.	<eof> parser at end of file
2.	<source>:1:1: parsing namespace 'combinators'
 #0 0x0000555afcfe1247 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-16.0.0-assert/bin/clang+++0x38b8247)
 #1 0x0000555afcfdf0ce llvm::sys::RunSignalHandlers() (/opt/compiler-explorer/clang-16.0.0-assert/bin/clang+++0x38b60ce)
 #2 0x0000555afcfe064d llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-16.0.0-assert/bin/clang+++0x38b764d)
 #3 0x0000555afcf5e558 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #4 0x00007ff28ee54df0 __restore_rt (/lib64/libc.so.6+0x54df0)
 #5 0x00007ff28eea154c __pthread_kill_implementation (/lib64/libc.so.6+0xa154c)
 #6 0x00007ff28ee54d46 gsignal (/lib64/libc.so.6+0x54d46)
 #7 0x00007ff28ee287f3 abort (/lib64/libc.so.6+0x287f3)
 #8 0x00007ff28ee2871b _nl_load_domain.cold (/lib64/libc.so.6+0x2871b)
 #9 0x00007ff28ee4dce6 (/lib64/libc.so.6+0x4dce6)
#10 0x0000555aff10c23f (/opt/compiler-explorer/clang-16.0.0-assert/bin/clang+++0x59e323f)
#11 0x0000555aff10c931 clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) (/opt/compiler-explorer/clang-16.0.0-assert/bin/clang+++0x59e3931)
#12 0x0000555afee89948 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-16.0.0-assert/bin/clang+++0x5760948)
#13 0x0000555afee86a5c clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-16.0.0-assert/bin/clang+++0x575da5c)
#14 0x0000555afee6a16b clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-16.0.0-assert/bin/clang+++0x574116b)
#15 0x0000555afee69a58 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-16.0.0-assert/bin/clang+++0x5740a58)
#16 0x0000555afee689f3 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-16.0.0-assert/bin/clang+++0x573f9f3)
#17 0x0000555afeea46a8 clang::Parser::ParseInnerNamespace(llvm::SmallVector<clang::Parser::InnerNamespaceInfo, 4u> const&, unsigned int, clang::SourceLocation&, clang::ParsedAttributes&, clang::BalancedDelimiterTracker&) (/opt/compiler-explorer/clang-16.0.0-assert/bin/clang+++0x577b6a8)
#18 0x0000555afeea3e58 clang::Parser::ParseNamespace(clang::DeclaratorContext, clang::SourceLocation&, clang::SourceLocation) (/opt/compiler-explorer/clang-16.0.0-assert/bin/clang+++0x577ae58)
#19 0x0000555afee855a6 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/opt/compiler-explorer/clang-16.0.0-assert/bin/clang+++0x575c5a6)
#20 0x0000555afee68540 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-16.0.0-assert/bin/clang+++0x573f540)
#21 0x0000555afee666ff clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-16.0.0-assert/bin/clang+++0x573d6ff)
#22 0x0000555afee65f4f clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-16.0.0-assert/bin/clang+++0x573cf4f)
#23 0x0000555afee60f70 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-16.0.0-assert/bin/clang+++0x5737f70)
#24 0x0000555afda7df60 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-16.0.0-assert/bin/clang+++0x4354f60)
#25 0x0000555afd9f189f clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-16.0.0-assert/bin/clang+++0x42c889f)
#26 0x0000555afdb4f793 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-16.0.0-assert/bin/clang+++0x4426793)
#27 0x0000555afabaa24b cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-16.0.0-assert/bin/clang+++0x148124b)
#28 0x0000555afaba6a6b ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#29 0x0000555afd8655c2 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::$_1>(long) Job.cpp:0:0
#30 0x0000555afcf5e29b llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-16.0.0-assert/bin/clang+++0x383529b)
#31 0x0000555afd864f20 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (/opt/compiler-explorer/clang-16.0.0-assert/bin/clang+++0x413bf20)
#32 0x0000555afd82899e clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-16.0.0-assert/bin/clang+++0x40ff99e)
#33 0x0000555afd828c4e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-16.0.0-assert/bin/clang+++0x40ffc4e)
#34 0x0000555afd847350 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-16.0.0-assert/bin/clang+++0x411e350)
#35 0x0000555afaba5b06 clang_main(int, char**) (/opt/compiler-explorer/clang-16.0.0-assert/bin/clang+++0x147cb06)
#36 0x00007ff28ee3feb0 __libc_start_call_main (/lib64/libc.so.6+0x3feb0)
#37 0x00007ff28ee3ff60 __libc_start_main@GLIBC_2.2.5 (/lib64/libc.so.6+0x3ff60)
#38 0x0000555afaba2b9e _start (/opt/compiler-explorer/clang-16.0.0-assert/bin/clang+++0x1479b9e)
clang-16: error: clang frontend command failed with exit code 134 (use -v to see invocation)
```

---

### Comment 9 - VoxSciurorum

I think the assertion is erroneous and the function should return false instead of assert failing.

[22203.diff.txt](https://github.com/llvm/llvm-project/files/12562414/22203.diff.txt)


---

### Comment 10 - shafik

This is crashing b/c of this section in `Parser::ParseCompoundStatementBody(...)`:

https://github.com/llvm/llvm-project/blob/6b6312b01d1f6966865dc0156c8837e0fc6d52eb/clang/lib/Parse/ParseStmt.cpp#L1269-L1272

the change was introduced via 6db850133f66a

I think it would make more sense to unconditionally return `StmtError()` but changing that breaks test introduced by the above change. I tested modifying `ParseCompoundStatementBody(...)` to take a second argument `IsLambda` and then only error if we are parsing a lambda body and that passing `check-clang`, so that is an alternative fix.

CC @akyrtzi @rjmccall who can hopefully help me understand the rationale for the change and maybe if we can rethink or I need to use the alternative fix.

CC @zygoloid who may have some opinions on the right approach

Note, there is a radar link but not really helpful since the larger rationale is not really laid out in the commit message nor the comment.

---

### Comment 11 - VoxSciurorum

The one-liner `auto g = [] { void g(); };` does not go through that else branch.  It is syntactically valid but semantically invalid.  The inner declaration `void g()` changes the linkage of the outer `auto g`.

> This is crashing b/c of this section in `Parser::ParseCompoundStatementBody(...)`:
> 
> https://github.com/llvm/llvm-project/blob/6b6312b01d1f6966865dc0156c8837e0fc6d52eb/clang/lib/Parse/ParseStmt.cpp#L1269-L1272
> 
> the change was introduced via [6db8501](https://github.com/llvm/llvm-project/commit/6db850133f66abbe89a2d6c0f161c063eba79840)



---

### Comment 12 - akyrtzi

The rationale is to preserve as much of the function body in the AST as mush as possible, even in the presence of erroneous code. This is particularly useful for libclang clients like an IDE.
The tests check that by checking that the "int x;" statement is preserved in the AST and can be found by libclang.

---

### Comment 13 - rjmccall

Making the type-checker just bail out and drop information earlier is more likely to avoid this kind of issue but can also impede effective recovery, as well as interfering with use cases like Argyrios's that would like to examine incomplete AST.  I think you need to investigate to figure out what exactly is going wrong downstream if you create a lambda with an invalid compound stmt like this.  Maybe there's something that sees an invalid ending source location and then skips some validation step?

---

### Comment 14 - shafik

> Making the type-checker just bail out and drop information earlier is more likely to avoid this kind of issue but can also impede effective recovery, as well as interfering with use cases like Argyrios's that would like to examine incomplete AST. I think you need to investigate to figure out what exactly is going wrong downstream if you create a lambda with an invalid compound stmt like this. Maybe there's something that sees an invalid ending source location and then skips some validation step?

This seems like we should be using Recovery Expressions then CC @hokein @sam-mccall for some advice to make sure it makes sense here.

---

### Comment 15 - hokein

> This seems like we should be using Recovery Expressions then CC @hokein @sam-mccall for some advice to make sure it makes sense here.

The `RecoveryExpr` only works on **Expr** level, not `Stmt`, so it doesn't fit on this case. The current recovery strategy seems reasonable to me -- in case of missing a closing `}` when parsing a compound statement, we still create one with an invalid source location of the `}`.

> I think you need to investigate to figure out what exactly is going wrong downstream if you create a lambda with an invalid compound stmt like this

+1, I think a fix is to figure out the related downstream place and teach it to handle this invalid case properly.

---

### Comment 16 - yurivict

Any progress on this?
We can't compile the [RDKit](https://www.rdkit.org/) port science/rdkit on FreeBSD 14 and 15 because of this bug.


---

### Comment 17 - alexfh

There's another one-liner test from https://github.com/llvm/llvm-project/issues/140776 (reduced from real code, and, AFAICT, valid C++): `namespace ns { auto _p = [] {}; }`

https://gcc.godbolt.org/z/qPveTez31

---

