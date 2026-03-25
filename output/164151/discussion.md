# [clang][bytecode] Assertion `B != this' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/164151
**Status:** Closed
**Labels:** crash, clang:bytecode
**Closed Date:** 2025-10-20T10:55:07Z

## Body

Reproducer:
https://godbolt.org/z/z7f3x1Mjf
```cpp
extern const double NaN;
extern const double NaN = (__builtin_nanf(""));
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/AST/ByteCode/InterpBlock.cpp:104: void clang::interp::Block::movePointersTo(clang::interp::Block*): Assertion `B != this' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -fexperimental-new-constant-interpreter <source>
1.	<source>:2:47: current parser token ';'
 #0 0x00000000042021d8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42021d8)
 #1 0x00000000041ff604 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41ff604)
 #2 0x0000000004143778 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000071ad39442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x000071ad394969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x000071ad39442476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x000071ad394287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x000071ad3942871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x000071ad39439e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x00000000080c021e (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x80c021e)
#10 0x0000000007e3ae2d clang::interp::Program::createGlobal(clang::ValueDecl const*, clang::Expr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7e3ae2d)
#11 0x00000000080ad166 clang::interp::Compiler<clang::interp::EvalEmitter>::visitVarDecl(clang::VarDecl const*, clang::Expr const*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x80ad166)
#12 0x00000000080ad614 clang::interp::Compiler<clang::interp::EvalEmitter>::visitDeclAndReturn(clang::VarDecl const*, clang::Expr const*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x80ad614)
#13 0x0000000007c619bd clang::interp::EvalEmitter::interpretDecl(clang::VarDecl const*, clang::Expr const*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7c619bd)
#14 0x0000000007c3e7b6 clang::interp::Context::evaluateAsInitializer(clang::interp::State&, clang::VarDecl const*, clang::Expr const*, clang::APValue&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7c3e7b6)
#15 0x0000000007c1635e clang::Expr::EvaluateAsInitializer(clang::APValue&, clang::ASTContext const&, clang::VarDecl const*, llvm::SmallVectorImpl<std::pair<clang::SourceLocation, clang::PartialDiagnostic>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7c1635e)
#16 0x0000000007a1bf22 clang::VarDecl::evaluateValueImpl(llvm::SmallVectorImpl<std::pair<clang::SourceLocation, clang::PartialDiagnostic>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7a1bf22)
#17 0x0000000007a1c2d1 clang::VarDecl::checkForConstantInitialization(llvm::SmallVectorImpl<std::pair<clang::SourceLocation, clang::PartialDiagnostic>>&) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7a1c2d1)
#18 0x0000000006ccf06b clang::Sema::CheckCompleteVariableDeclaration(clang::VarDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ccf06b)
#19 0x0000000006ce7017 clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ce7017)
#20 0x0000000006931aea clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6931aea)
#21 0x000000000693ee9a clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x693ee9a)
#22 0x00000000068fcbde clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68fcbde)
#23 0x00000000068fd37f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68fd37f)
#24 0x0000000006904c33 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6904c33)
#25 0x0000000006905b45 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6905b45)
#26 0x00000000068e74fa clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68e74fa)
#27 0x0000000004b4abe8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b4abe8)
#28 0x0000000004e38265 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e38265)
#29 0x0000000004db92fe clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4db92fe)
#30 0x0000000004f2fcfd clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f2fcfd)
#31 0x0000000000dc2b50 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdc2b50)
#32 0x0000000000db96ba ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#33 0x0000000000db983d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#34 0x0000000004bb3d99 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#35 0x0000000004143c14 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4143c14)
#36 0x0000000004bb43af clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#37 0x0000000004b756c2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b756c2)
#38 0x0000000004b7666e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b7666e)
#39 0x0000000004b7dda5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b7dda5)
#40 0x0000000000dbf079 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdbf079)
#41 0x0000000000c6ee04 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc6ee04)
#42 0x000071ad39429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#43 0x000071ad39429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#44 0x0000000000db9155 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb9155)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - k-arrows

This is trunk only:
https://godbolt.org/z/v8MjMcecf

---

