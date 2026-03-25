# [Clang 22][ICE] "cannot compile this l-value expression yet" when using std::array inside consteval lambda

**Author:** prevter
**URL:** https://github.com/llvm/llvm-project/issues/165039
**Status:** Open
**Labels:** clang, regression, confirmed, crash, needs-reduction

## Body

Code below compiles successfully on Clang 21 and GCC, but crashes on the current trunk build available on Compiler Explorer.

Godbolt link: https://godbolt.org/z/svb3faqoP

```cpp
#include <array>
#include <cstddef>
#include <tuple>
#include <utility>

template <int V>
struct Tag {};

template <class... Ts>
struct Wrapper {
    constexpr Wrapper(std::tuple<Ts...>) {}
};

template <size_t N, std::array<int, N> A>
consteval auto make_tuple() {
    return []<size_t... I>(std::index_sequence<I...>) {
        return std::tuple<Tag<A[I]>...>{};
    }(std::make_index_sequence<N>{});
}

template <size_t N, std::array<int, N> A>
using Alias = decltype(Wrapper{ make_tuple<N, A>() });

template <class>
constexpr int bar() { return 1; }

template <std::array A>
constexpr int foo() {
    // "error: cannot compile this l-value expression yet":
    return bar<Alias<A.size(), A>>();
}

int main() {
    constexpr std::array data = {1};
    return foo<data>();
}
```

Compiler output:
```
<source>:30:12: error: cannot compile this l-value expression yet
   30 |     return bar<Alias<A.size(), A>>();
      |            ^~~~~~~~~~~~~~~~~~~~~~~
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -O3 -std=c++20 <source>
1.	<eof> parser at end of file
2.	Per-file LLVM IR generation
3.	<source>:28:15: Generating code for declaration 'foo'
#0 0x0000000003cecc38 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3cecc38)
#1 0x0000000003cea60c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3cea60c)
#2 0x0000000003c30708 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
#3 0x0000762349042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
#4 0x0000000034f4c520
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```

## Comments

### Comment 1 - shafik

Everyone accepts this: https://godbolt.org/z/oveh1zjaY

In general use the least command line options possible and assertions build, we will get more information that way.

unreachable:

```console
<source>:30:12: error: cannot compile this l-value expression yet
   30 |     return bar<Alias<A.size(), A>>();
      |            ^~~~~~~~~~~~~~~~~~~~~~~
Unexpected placeholder builtin type!
UNREACHABLE executed at /root/llvm-project/llvm/tools/clang/lib/CodeGen/CodeGenTypes.cpp:594!
```

Backtrace:

```console
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++26 <source> -isystem/opt/compiler-explorer/libs/fmt/trunk/include
1.	<eof> parser at end of file
2.	Per-file LLVM IR generation
3.	<source>:28:15: Generating code for declaration 'foo'
 #0 0x0000000004214d18 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4214d18)
 #1 0x0000000004212144 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4212144)
 #2 0x0000000004156348 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007a2f92242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007a2f922969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007a2f92242476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007a2f922287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x0000000004161f2a (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4161f2a)
 #8 0x0000000004844ae0 clang::CodeGen::CodeGenTypes::ConvertType(clang::QualType) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4844ae0)
 #9 0x0000000004556030 clang::CodeGen::CodeGenFunction::EmitUnsupportedLValue(clang::Expr const*, char const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4556030)
#10 0x000000000458cdd5 clang::CodeGen::CodeGenFunction::EmitLValueHelper(clang::Expr const*, clang::CodeGen::KnownNonNull_t) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x458cdd5)
#11 0x000000000458d967 void llvm::function_ref<void ()>::callback_fn<clang::CodeGen::CodeGenFunction::EmitLValue(clang::Expr const*, clang::CodeGen::KnownNonNull_t)::'lambda'()>(long) CGExpr.cpp:0:0
#12 0x00000000082579b1 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x82579b1)
#13 0x0000000004556373 clang::CodeGen::CodeGenFunction::EmitLValue(clang::Expr const*, clang::CodeGen::KnownNonNull_t) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4556373)
#14 0x000000000458609b clang::CodeGen::CodeGenFunction::EmitCallee(clang::Expr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x458609b)
#15 0x000000000458a6cf clang::CodeGen::CodeGenFunction::EmitCallExpr(clang::CallExpr const*, clang::CodeGen::ReturnValueSlot, llvm::CallBase**) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x458a6cf)
#16 0x00000000045f8184 (anonymous namespace)::ScalarExprEmitter::VisitCallExpr(clang::CallExpr const*) CGExprScalar.cpp:0:0
#17 0x00000000045ec7c0 clang::StmtVisitorBase<std::add_pointer, (anonymous namespace)::ScalarExprEmitter, llvm::Value*>::Visit(clang::Stmt*) CGExprScalar.cpp:0:0
#18 0x00000000045f0f7c clang::CodeGen::CodeGenFunction::EmitScalarExpr(clang::Expr const*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x45f0f7c)
#19 0x00000000046fd1cc clang::CodeGen::CodeGenFunction::EmitReturnStmt(clang::ReturnStmt const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x46fd1cc)
#20 0x000000000470666e clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef<clang::Attr const*>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x470666e)
#21 0x000000000470e65c clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&, bool, clang::CodeGen::AggValueSlot) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x470e65c)
#22 0x0000000004776c2e clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4776c2e)
#23 0x0000000004788c84 clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4788c84)
#24 0x00000000047f5d80 clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x47f5d80)
#25 0x00000000047f0c44 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x47f0c44)
#26 0x00000000047fdafc clang::CodeGen::CodeGenModule::EmitDeferred() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x47fdafc)
#27 0x00000000048001a3 clang::CodeGen::CodeGenModule::Release() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48001a3)
#28 0x0000000004b60cfe (anonymous namespace)::CodeGeneratorImpl::HandleTranslationUnit(clang::ASTContext&) ModuleBuilder.cpp:0:0
#29 0x0000000004b5cd05 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b5cd05)
#30 0x00000000069013fc clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69013fc)
#31 0x0000000004b5d4e8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b5d4e8)
#32 0x0000000004e4a685 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e4a685)
#33 0x0000000004dcb73e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4dcb73e)
#34 0x0000000004f41c2d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f41c2d)
#35 0x0000000000dce380 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdce380)
#36 0x0000000000dc4eea ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#37 0x0000000000dc506d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#38 0x0000000004bc6a79 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#39 0x00000000041567e4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41567e4)
#40 0x0000000004bc708f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#41 0x0000000004b87fd2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b87fd2)
#42 0x0000000004b88f7e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b88f7e)
#43 0x0000000004b90695 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b90695)
#44 0x0000000000dca8a9 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdca8a9)
#45 0x0000000000c79c64 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc79c64)
#46 0x00007a2f92229d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#47 0x00007a2f92229e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#48 0x0000000000dc4985 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdc4985)
```

---

### Comment 2 - prevter

Played a bit and managed to reduce needed code, if this helps:
https://godbolt.org/z/Wa5GGxPMa
```cpp
#include <array>
#include <cstddef>
#include <tuple>

template <size_t N, std::array<int, N> A>
consteval auto make_tuple() {
    return []<size_t... I>(std::index_sequence<I...>) {
        return std::tuple{A[I]...};
    }(std::make_index_sequence<N>{});
}

template <class>
constexpr int foo() { return 1; }

int main() {
    constexpr std::array data = {1};
    using T = decltype(make_tuple<data.size(), data>());
    return foo<T>();
}
```
few notes:
- it requires explicitly `template <size_t N, std::array<int, N> A>`, doing `template <auto A>` and replacing `N` with `A.size()` does not crash
- moving the `std::make_index_sequence` outside the function and not using the lambda also fixes the problem

Also not sure whether this is related or a separate bug, but I also managed to get a different assert:
https://godbolt.org/z/xvnf6YTsa
```cpp
#include <array>
#include <cstddef>

template <size_t N, std::array<int, N> A>
consteval int foo() {
    return []<size_t... I>(std::index_sequence<I...>) {
        return (A[I] + ... + 0); 
    }(std::make_index_sequence<N>{});
}

int main() {
    constexpr std::array data = {1};
    return foo<data.size(), data>();
}
```

```
clang++: /root/llvm-project/llvm/tools/clang/lib/AST/ExprConstant.cpp:5309: bool EvaluateDependentExpr(const clang::Expr*, {anonymous}::EvalInfo&): Assertion `E->containsErrors() && "valid value-dependent expression should never " "reach invalid code path."' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++26 <source> -isystem/opt/compiler-explorer/libs/fmt/trunk/include
1.	<eof> parser at end of file
2.	<source>:12:12: parsing function body 'main'
 #0 0x0000000004214d18 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4214d18)
 #1 0x0000000004212144 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4212144)
 #2 0x0000000004156348 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x0000776285242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007762852969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x0000776285242476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007762852287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x000077628522871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x0000776285239e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000007b90464 EvaluateDependentExpr(clang::Expr const*, (anonymous namespace)::EvalInfo&) ExprConstant.cpp:0:0
#10 0x0000000007be6540 EvaluateStmt((anonymous namespace)::StmtResult&, (anonymous namespace)::EvalInfo&, clang::Stmt const*, clang::SwitchCase const*) (.part.0) ExprConstant.cpp:0:0
#11 0x0000000007be5158 EvaluateStmt((anonymous namespace)::StmtResult&, (anonymous namespace)::EvalInfo&, clang::Stmt const*, clang::SwitchCase const*) (.part.0) ExprConstant.cpp:0:0
#12 0x0000000007bec89b HandleFunctionCall(clang::SourceLocation, clang::FunctionDecl const*, (anonymous namespace)::LValue const*, clang::Expr const*, llvm::ArrayRef<clang::Expr const*>, (anonymous namespace)::CallRef, clang::Stmt const*, (anonymous namespace)::EvalInfo&, clang::APValue&, (anonymous namespace)::LValue const*) ExprConstant.cpp:0:0
#13 0x0000000007c3ef75 (anonymous namespace)::IntExprEvaluator::VisitCallExpr(clang::CallExpr const*) ExprConstant.cpp:0:0
#14 0x0000000007bd2a15 clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#15 0x0000000007bd41f0 clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#16 0x0000000007bc20de Evaluate(clang::APValue&, (anonymous namespace)::EvalInfo&, clang::Expr const*) ExprConstant.cpp:0:0
#17 0x0000000007bdf765 EvaluateInPlace(clang::APValue&, (anonymous namespace)::EvalInfo&, (anonymous namespace)::LValue const&, clang::Expr const*, bool) (.part.0) ExprConstant.cpp:0:0
#18 0x0000000007c2c312 clang::Expr::EvaluateAsConstantExpr(clang::Expr::EvalResult&, clang::ASTContext const&, clang::Expr::ConstantExprKind) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7c2c312)
#19 0x0000000006e67b1d clang::Sema::PopExpressionEvaluationContext() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e67b1d)
#20 0x0000000006d018ea clang::Sema::ActOnFinishFunctionBody(clang::Decl*, clang::Stmt*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d018ea)
#21 0x0000000006a1c9a4 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a1c9a4)
#22 0x000000000692343b clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x692343b)
#23 0x00000000069598ad clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69598ad)
#24 0x0000000006916bee clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6916bee)
#25 0x000000000691738f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x691738f)
#26 0x000000000691ec43 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x691ec43)
#27 0x000000000691fb55 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x691fb55)
#28 0x00000000069011da clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69011da)
#29 0x0000000004b5d4e8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b5d4e8)
#30 0x0000000004e4a685 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e4a685)
#31 0x0000000004dcb73e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4dcb73e)
#32 0x0000000004f41c2d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f41c2d)
#33 0x0000000000dce380 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdce380)
#34 0x0000000000dc4eea ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#35 0x0000000000dc506d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#36 0x0000000004bc6a79 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#37 0x00000000041567e4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41567e4)
#38 0x0000000004bc708f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#39 0x0000000004b87fd2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b87fd2)
#40 0x0000000004b88f7e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b88f7e)
#41 0x0000000004b90695 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b90695)
#42 0x0000000000dca8a9 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdca8a9)
#43 0x0000000000c79c64 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc79c64)
#44 0x0000776285229d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#45 0x0000776285229e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#46 0x0000000000dc4985 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdc4985)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

---

### Comment 3 - shafik

@mizvekov git bisect points to 078e99ef017cac3899e5dbc2ed917f173c9eedad

---

