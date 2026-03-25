# [clang][bytecode] Assertion `IsInitialized' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/173950
**Status:** Closed
**Labels:** regression, crash, clang:bytecode
**Closed Date:** 2026-01-02T15:49:45Z

## Body

Reproducer:
https://godbolt.org/z/9qEqrexnx
```cpp
namespace std {

class type_info {};

} // namespace std

namespace Test1 {

struct A {
  virtual void f();
};

static A &a = *new A;

extern A &a;

const std::type_info &a_ti = typeid(a);

} // namespace Test1
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/AST/ByteCode/InterpBlock.h:147: void clang::interp::Block::invokeDtor(): Assertion `IsInitialized' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 -fexperimental-new-constant-interpreter <source>
1.	<source>:17:39: current parser token ';'
2.	<source>:7:1: parsing namespace 'Test1'
 #0 0x0000000004302f28 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4302f28)
 #1 0x0000000004300354 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4300354)
 #2 0x0000000004244d58 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x0000794fc1e42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x0000794fc1e969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x0000794fc1e42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x0000794fc1e287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x0000794fc1e2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x0000794fc1e39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x000000000825816d clang::interp::Compiler<clang::interp::EvalEmitter>::visitDeclAndReturn(clang::VarDecl const*, clang::Expr const*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x825816d)
#10 0x0000000007de64bd clang::interp::EvalEmitter::interpretDecl(clang::VarDecl const*, clang::Expr const*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7de64bd)
#11 0x0000000007dc53a6 clang::interp::Context::evaluateAsInitializer(clang::interp::State&, clang::VarDecl const*, clang::Expr const*, clang::APValue&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7dc53a6)
#12 0x0000000007dae296 clang::Expr::EvaluateAsInitializer(clang::APValue&, clang::ASTContext const&, clang::VarDecl const*, llvm::SmallVectorImpl<std::pair<clang::SourceLocation, clang::PartialDiagnostic>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7dae296)
#13 0x0000000008258dc2 clang::interp::Compiler<clang::interp::EvalEmitter>::visitDeclRef(clang::ValueDecl const*, clang::Expr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x8258dc2)
#14 0x0000000008262e2a clang::interp::Compiler<clang::interp::EvalEmitter>::visit(clang::Expr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x8262e2a)
#15 0x0000000008227cbb clang::interp::Compiler<clang::interp::EvalEmitter>::VisitCXXTypeidExpr(clang::CXXTypeidExpr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x8227cbb)
#16 0x0000000008262e2a clang::interp::Compiler<clang::interp::EvalEmitter>::visit(clang::Expr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x8262e2a)
#17 0x0000000008257841 clang::interp::Compiler<clang::interp::EvalEmitter>::visitVarDecl(clang::VarDecl const*, clang::Expr const*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x8257841)
#18 0x0000000008257bbd clang::interp::Compiler<clang::interp::EvalEmitter>::visitDeclAndReturn(clang::VarDecl const*, clang::Expr const*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x8257bbd)
#19 0x0000000007de64bd clang::interp::EvalEmitter::interpretDecl(clang::VarDecl const*, clang::Expr const*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7de64bd)
#20 0x0000000007dc53a6 clang::interp::Context::evaluateAsInitializer(clang::interp::State&, clang::VarDecl const*, clang::Expr const*, clang::APValue&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7dc53a6)
#21 0x0000000007dae296 clang::Expr::EvaluateAsInitializer(clang::APValue&, clang::ASTContext const&, clang::VarDecl const*, llvm::SmallVectorImpl<std::pair<clang::SourceLocation, clang::PartialDiagnostic>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7dae296)
#22 0x0000000007b92d62 clang::VarDecl::evaluateValueImpl(llvm::SmallVectorImpl<std::pair<clang::SourceLocation, clang::PartialDiagnostic>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7b92d62)
#23 0x0000000007b93111 clang::VarDecl::checkForConstantInitialization(llvm::SmallVectorImpl<std::pair<clang::SourceLocation, clang::PartialDiagnostic>>&) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7b93111)
#24 0x0000000006e1a8eb clang::Sema::CheckCompleteVariableDeclaration(clang::VarDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e1a8eb)
#25 0x0000000006e22676 clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e22676)
#26 0x0000000006a70a21 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a70a21)
#27 0x0000000006a7dc7a clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a7dc7a)
#28 0x0000000006a3b32b clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a3b32b)
#29 0x0000000006a3bb1f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a3bb1f)
#30 0x0000000006a4394a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a4394a)
#31 0x0000000006a9fef5 clang::Parser::ParseInnerNamespace(llvm::SmallVector<clang::Parser::InnerNamespaceInfo, 4u> const&, unsigned int, clang::SourceLocation&, clang::ParsedAttributes&, clang::BalancedDelimiterTracker&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a9fef5)
#32 0x0000000006aa2dc6 clang::Parser::ParseNamespace(clang::DeclaratorContext, clang::SourceLocation&, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6aa2dc6)
#33 0x0000000006a87040 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a87040)
#34 0x0000000006a439fc clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a439fc)
#35 0x0000000006a448e5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a448e5)
#36 0x0000000006a2596a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a2596a)
#37 0x0000000004c64298 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c64298)
#38 0x0000000004f5a915 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f5a915)
#39 0x0000000004edabae clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4edabae)
#40 0x0000000005054c9d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5054c9d)
#41 0x0000000000de672c cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde672c)
#42 0x0000000000ddd17a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#43 0x0000000000ddd2fd int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#44 0x0000000004cd6969 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#45 0x00000000042451f4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42451f4)
#46 0x0000000004cd6f7f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#47 0x0000000004c97a62 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c97a62)
#48 0x0000000004c98a0e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c98a0e)
#49 0x0000000004c9fe65 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c9fe65)
#50 0x0000000000de2b21 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde2b21)
#51 0x0000000000c8d454 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc8d454)
#52 0x0000794fc1e29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#53 0x0000794fc1e29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#54 0x0000000000ddcc15 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xddcc15)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

This is trunk only:
https://godbolt.org/z/deGj76b8z

Note: using `-std=c++20`

## Comments

### Comment 1 - shafik

CC @tbaederr 

---

### Comment 2 - tbaederr

There's really no need to CC me on bytecode issues, I see all of them anyway.

---

