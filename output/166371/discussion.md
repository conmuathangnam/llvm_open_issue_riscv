# [clang][bytecode] crash when casting address of struct field to enclosing struct type

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/166371
**Status:** Closed
**Labels:** crash, clang:bytecode
**Closed Date:** 2025-11-17T07:24:46Z

## Body

Reproducer:
https://godbolt.org/z/35e8azrda
```cpp
struct s { int a; int b[1]; };
struct s x;
int *y = ((struct s *)&x.a)->b;
```

Backtrace:
```console
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -fexperimental-new-constant-interpreter <source>
1.	<source>:3:31: current parser token ';'
 #0 0x0000000004238d78 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4238d78)
 #1 0x00000000042361a4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42361a4)
 #2 0x000000000417a418 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007edf7e042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x0000000007c96578 clang::interp::EvalEmitter::emitArrayDecay(clang::interp::SourceInfo) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7c96578)
 #5 0x000000000810f258 clang::interp::Compiler<clang::interp::EvalEmitter>::VisitCastExpr(clang::CastExpr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x810f258)
 #6 0x000000000810e45a clang::interp::Compiler<clang::interp::EvalEmitter>::visit(clang::Expr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x810e45a)
 #7 0x000000000810367c clang::interp::Compiler<clang::interp::EvalEmitter>::visitVarDecl(clang::VarDecl const*, clang::Expr const*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x810367c)
 #8 0x0000000008103c24 clang::interp::Compiler<clang::interp::EvalEmitter>::visitDeclAndReturn(clang::VarDecl const*, clang::Expr const*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x8103c24)
 #9 0x0000000007c9dffd clang::interp::EvalEmitter::interpretDecl(clang::VarDecl const*, clang::Expr const*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7c9dffd)
#10 0x0000000007c7b676 clang::interp::Context::evaluateAsInitializer(clang::interp::State&, clang::VarDecl const*, clang::Expr const*, clang::APValue&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7c7b676)
#11 0x0000000007c52866 clang::Expr::EvaluateAsInitializer(clang::APValue&, clang::ASTContext const&, clang::VarDecl const*, llvm::SmallVectorImpl<std::pair<clang::SourceLocation, clang::PartialDiagnostic>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7c52866)
#12 0x0000000007a55542 clang::VarDecl::evaluateValueImpl(llvm::SmallVectorImpl<std::pair<clang::SourceLocation, clang::PartialDiagnostic>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7a55542)
#13 0x0000000007a558f1 clang::VarDecl::checkForConstantInitialization(llvm::SmallVectorImpl<std::pair<clang::SourceLocation, clang::PartialDiagnostic>>&) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7a558f1)
#14 0x0000000006d0e61b clang::Sema::CheckCompleteVariableDeclaration(clang::VarDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d0e61b)
#15 0x0000000006d25e97 clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d25e97)
#16 0x000000000697145a clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x697145a)
#17 0x000000000697e77a clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x697e77a)
#18 0x000000000693c56e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x693c56e)
#19 0x000000000693cd0f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x693cd0f)
#20 0x00000000069445c3 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69445c3)
#21 0x00000000069454d5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69454d5)
#22 0x0000000006926b5a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6926b5a)
#23 0x0000000004b84408 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b84408)
#24 0x0000000004e72255 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e72255)
#25 0x0000000004df319e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4df319e)
#26 0x0000000004f6a9cd clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f6a9cd)
#27 0x0000000000dcd1b0 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdcd1b0)
#28 0x0000000000dc3d2a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#29 0x0000000000dc3ead int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#30 0x0000000004bedcb9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#31 0x000000000417a8b4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x417a8b4)
#32 0x0000000004bee2cf clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#33 0x0000000004baf022 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4baf022)
#34 0x0000000004baffce clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4baffce)
#35 0x0000000004bb7725 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bb7725)
#36 0x0000000000dc96d1 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdc96d1)
#37 0x0000000000c79094 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc79094)
#38 0x00007edf7e029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#39 0x00007edf7e029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#40 0x0000000000dc37c5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdc37c5)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```

