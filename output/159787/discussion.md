# [clang][bytecode] crash with -std=c++03 on C++11 extension

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/159787
**Status:** Closed
**Labels:** crash, regression:19, clang:bytecode
**Closed Date:** 2025-09-23T10:55:49Z

## Body

Reproducer:
https://godbolt.org/z/jjfc1Msq9
```cpp
typedef __attribute__((ext_vector_type(4))) int vi4b;
struct S {
  vi4b w;
};
int &&s = S().w[1];
```

Backtrace:
```console
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++03 -fexperimental-new-constant-interpreter <source>
1.	<source>:5:19: current parser token ';'
 #0 0x0000000003c5f198 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c5f198)
 #1 0x0000000003c5cb6c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c5cb6c)
 #2 0x0000000003bacbb8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007a79dfc42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x0000000007434f61 bool clang::interp::InitElem<(clang::interp::PrimType)4, clang::interp::Integral<32u, true>>(clang::interp::InterpState&, clang::interp::CodePtr, unsigned int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x7434f61)
 #5 0x00000000077ad7f1 clang::interp::Compiler<clang::interp::EvalEmitter>::visitZeroRecordInitializer(clang::interp::Record const*, clang::Expr const*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x77ad7f1)
 #6 0x00000000077d4242 clang::interp::Compiler<clang::interp::EvalEmitter>::VisitCXXConstructExpr(clang::CXXConstructExpr const*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x77d4242)
 #7 0x00000000077e2585 clang::interp::Compiler<clang::interp::EvalEmitter>::VisitMemberExpr(clang::MemberExpr const*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x77e2585)
 #8 0x00000000077d3a8e clang::interp::Compiler<clang::interp::EvalEmitter>::visitInitializer(clang::Expr const*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x77d3a8e)
 #9 0x00000000077de995 clang::interp::Compiler<clang::interp::EvalEmitter>::visit(clang::Expr const*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x77de995)
#10 0x00000000077aa4e0 clang::interp::Compiler<clang::interp::EvalEmitter>::VisitArraySubscriptExpr(clang::ArraySubscriptExpr const*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x77aa4e0)
#11 0x00000000077de81b clang::interp::Compiler<clang::interp::EvalEmitter>::visit(clang::Expr const*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x77de81b)
#12 0x00000000077d4a4f clang::interp::Compiler<clang::interp::EvalEmitter>::visitExpr(clang::Expr const*, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x77d4a4f)
#13 0x00000000073f67f3 clang::interp::EvalEmitter::interpretExpr(clang::Expr const*, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x73f67f3)
#14 0x00000000073e88e6 clang::interp::Context::evaluateAsRValue(clang::interp::State&, clang::Expr const*, clang::APValue&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x73e88e6)
#15 0x0000000007361c8a EvaluateAsRValue((anonymous namespace)::EvalInfo&, clang::Expr const*, clang::APValue&) ExprConstant.cpp:0:0
#16 0x00000000073661a8 clang::Expr::EvaluateAsRValue(clang::Expr::EvalResult&, clang::ASTContext const&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x73661a8)
#17 0x0000000007366329 clang::Expr::isEvaluatable(clang::ASTContext const&, clang::Expr::SideEffectsKind) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x7366329)
#18 0x00000000073268f5 clang::Expr::isConstantInitializer(clang::ASTContext&, bool, clang::Expr const**) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x73268f5)
#19 0x0000000007326889 clang::Expr::isConstantInitializer(clang::ASTContext&, bool, clang::Expr const**) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x7326889)
#20 0x00000000073267da clang::Expr::isConstantInitializer(clang::ASTContext&, bool, clang::Expr const**) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x73267da)
#21 0x0000000006559546 clang::Sema::CheckCompleteVariableDeclaration(clang::VarDecl*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6559546)
#22 0x00000000065649e5 clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x65649e5)
#23 0x00000000061d37f4 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61d37f4)
#24 0x00000000061e852c clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61e852c)
#25 0x0000000006194acf clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6194acf)
#26 0x00000000061958c7 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61958c7)
#27 0x000000000619e51f clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x619e51f)
#28 0x000000000619fd27 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x619fd27)
#29 0x00000000061787da clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61787da)
#30 0x0000000004529e8d clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4529e8d)
#31 0x0000000004825efa clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4825efa)
#32 0x00000000047a05eb clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x47a05eb)
#33 0x0000000004919a9b clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4919a9b)
#34 0x0000000000dc4415 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdc4415)
#35 0x0000000000dbc36b ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#36 0x0000000000dbc40d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#37 0x0000000004597ab9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#38 0x0000000003bacfd3 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3bacfd3)
#39 0x0000000004597cd9 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#40 0x000000000455ae02 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x455ae02)
#41 0x000000000455bce1 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x455bce1)
#42 0x000000000456483c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x456483c)
#43 0x0000000000dc0e61 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdc0e61)
#44 0x0000000000c6eb54 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc6eb54)
#45 0x00007a79dfc29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#46 0x00007a79dfc29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#47 0x0000000000dbbe05 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdbbe05)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```

## Comments

### Comment 1 - k-arrows

Goes back to clang-19.1.0:
https://godbolt.org/z/vfc9bns1s

---

