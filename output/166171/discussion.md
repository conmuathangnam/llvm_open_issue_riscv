# [clang][bytecode] crashes when initializing struct with default member initializer for rvalue reference

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/166171
**Status:** Closed
**Labels:** crash, clang:bytecode
**Closed Date:** 2025-11-11T09:36:01Z

## Body

Reproducer:
https://godbolt.org/z/Wb9E88Mr4
```cpp
struct S {
  int &&a = 2;
  int b[1]{a};
};

S c[2][2]{{{5}}};

void foo() {
  if (++c[0][0].b[0] != 6)
    __builtin_abort();
}
```

Backtrace:
```console
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -fexperimental-new-constant-interpreter <source>
1.	<source>:6:17: current parser token ';'
 #0 0x0000000004239d18 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4239d18)
 #1 0x0000000004237144 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4237144)
 #2 0x000000000417b398 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007fc919442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x0000000007e2f4b7 clang::interp::CheckRange(clang::interp::InterpState&, clang::interp::CodePtr, clang::interp::Pointer const&, clang::AccessKinds) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7e2f4b7)
 #5 0x0000000007e53087 clang::interp::CheckLoad(clang::interp::InterpState&, clang::interp::CodePtr, clang::interp::Pointer const&, clang::AccessKinds) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7e53087)
 #6 0x0000000007cad91a bool clang::interp::GetFieldPop<(clang::interp::PrimType)13, clang::interp::Pointer>(clang::interp::InterpState&, clang::interp::CodePtr, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7cad91a)
 #7 0x00000000080fcd75 clang::interp::Compiler<clang::interp::EvalEmitter>::VisitMemberExpr(clang::MemberExpr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x80fcd75)
 #8 0x00000000081027ba clang::interp::Compiler<clang::interp::EvalEmitter>::visit(clang::Expr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x81027ba)
 #9 0x0000000008103f9a clang::interp::Compiler<clang::interp::EvalEmitter>::VisitCastExpr(clang::CastExpr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x8103f9a)
#10 0x00000000081027ba clang::interp::Compiler<clang::interp::EvalEmitter>::visit(clang::Expr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x81027ba)
#11 0x00000000080facb5 clang::interp::Compiler<clang::interp::EvalEmitter>::visitArrayElemInit(unsigned int, clang::Expr const*, clang::interp::OptPrimType) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x80facb5)
#12 0x00000000081097ad clang::interp::Compiler<clang::interp::EvalEmitter>::visitInitList(llvm::ArrayRef<clang::Expr const*>, clang::Expr const*, clang::Expr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x81097ad)
#13 0x0000000008105ca7 clang::interp::Compiler<clang::interp::EvalEmitter>::VisitCXXDefaultInitExpr(clang::CXXDefaultInitExpr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x8105ca7)
#14 0x00000000080f5c22 clang::interp::Compiler<clang::interp::EvalEmitter>::visitInitializer(clang::Expr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x80f5c22)
#15 0x00000000080fabd6 clang::interp::Compiler<clang::interp::EvalEmitter>::visitInitList(llvm::ArrayRef<clang::Expr const*>, clang::Expr const*, clang::Expr const*)::'lambda0'(clang::interp::Record::Field const*, clang::Expr const*, bool)::operator()(clang::interp::Record::Field const*, clang::Expr const*, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x80fabd6)
#16 0x0000000008109a3e clang::interp::Compiler<clang::interp::EvalEmitter>::visitInitList(llvm::ArrayRef<clang::Expr const*>, clang::Expr const*, clang::Expr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x8109a3e)
#17 0x00000000080f5c22 clang::interp::Compiler<clang::interp::EvalEmitter>::visitInitializer(clang::Expr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x80f5c22)
#18 0x00000000080fadca clang::interp::Compiler<clang::interp::EvalEmitter>::visitArrayElemInit(unsigned int, clang::Expr const*, clang::interp::OptPrimType) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x80fadca)
#19 0x00000000081097ad clang::interp::Compiler<clang::interp::EvalEmitter>::visitInitList(llvm::ArrayRef<clang::Expr const*>, clang::Expr const*, clang::Expr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x81097ad)
#20 0x00000000080f5c22 clang::interp::Compiler<clang::interp::EvalEmitter>::visitInitializer(clang::Expr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x80f5c22)
#21 0x00000000080fadca clang::interp::Compiler<clang::interp::EvalEmitter>::visitArrayElemInit(unsigned int, clang::Expr const*, clang::interp::OptPrimType) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x80fadca)
#22 0x00000000081097ad clang::interp::Compiler<clang::interp::EvalEmitter>::visitInitList(llvm::ArrayRef<clang::Expr const*>, clang::Expr const*, clang::Expr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x81097ad)
#23 0x0000000008107d75 clang::interp::Compiler<clang::interp::EvalEmitter>::VisitExprWithCleanups(clang::ExprWithCleanups const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x8107d75)
#24 0x00000000080f5c22 clang::interp::Compiler<clang::interp::EvalEmitter>::visitInitializer(clang::Expr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x80f5c22)
#25 0x00000000080f7c23 clang::interp::Compiler<clang::interp::EvalEmitter>::visitVarDecl(clang::VarDecl const*, clang::Expr const*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x80f7c23)
#26 0x00000000080f7f84 clang::interp::Compiler<clang::interp::EvalEmitter>::visitDeclAndReturn(clang::VarDecl const*, clang::Expr const*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x80f7f84)
#27 0x0000000007ca801d clang::interp::EvalEmitter::interpretDecl(clang::VarDecl const*, clang::Expr const*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7ca801d)
#28 0x0000000007c856a6 clang::interp::Context::evaluateAsInitializer(clang::interp::State&, clang::VarDecl const*, clang::Expr const*, clang::APValue&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7c856a6)
#29 0x0000000007c5c896 clang::Expr::EvaluateAsInitializer(clang::APValue&, clang::ASTContext const&, clang::VarDecl const*, llvm::SmallVectorImpl<std::pair<clang::SourceLocation, clang::PartialDiagnostic>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7c5c896)
#30 0x0000000007a5f4f2 clang::VarDecl::evaluateValueImpl(llvm::SmallVectorImpl<std::pair<clang::SourceLocation, clang::PartialDiagnostic>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7a5f4f2)
#31 0x0000000007a5f8a1 clang::VarDecl::checkForConstantInitialization(llvm::SmallVectorImpl<std::pair<clang::SourceLocation, clang::PartialDiagnostic>>&) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7a5f8a1)
#32 0x0000000006d1867b clang::Sema::CheckCompleteVariableDeclaration(clang::VarDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d1867b)
#33 0x0000000006d2fef7 clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d2fef7)
#34 0x000000000697aab4 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x697aab4)
#35 0x00000000069884ca clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69884ca)
#36 0x00000000069461ae clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69461ae)
#37 0x000000000694694f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x694694f)
#38 0x000000000694e203 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x694e203)
#39 0x000000000694f115 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x694f115)
#40 0x000000000693079a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x693079a)
#41 0x0000000004b84e58 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b84e58)
#42 0x0000000004e72a45 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e72a45)
#43 0x0000000004df384e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4df384e)
#44 0x0000000004f6b21d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f6b21d)
#45 0x0000000000dd0670 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdd0670)
#46 0x0000000000dc71da ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#47 0x0000000000dc735d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#48 0x0000000004bee6f9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#49 0x000000000417b834 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x417b834)
#50 0x0000000004beed0f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#51 0x0000000004bafaa2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bafaa2)
#52 0x0000000004bb0a4e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bb0a4e)
#53 0x0000000004bb8185 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bb8185)
#54 0x0000000000dccb99 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdccb99)
#55 0x0000000000c7bb64 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc7bb64)
#56 0x00007fc919429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#57 0x00007fc919429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#58 0x0000000000dc6c75 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdc6c75)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```

