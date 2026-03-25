# [clang][bytecode] crashes with --std=c++20 when typeid is used on consteval-evaluated object

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/160071
**Status:** Closed
**Labels:** crash, clang:bytecode
**Closed Date:** 2025-09-23T09:22:28Z

## Body

Reproducer:
https://godbolt.org/z/YE9fY3xKv
```cpp
namespace std {
struct type_info;
};

struct C {};

consteval C fC() { return {}; }

C c = fC();
consteval const std::type_info *ftype_info() { return &typeid(c); }

const std::type_info *T1 = ftype_info();
```

Backtrace:
```console
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics --std=c++20 -fexperimental-new-constant-interpreter <source>
1.	<source>:12:40: current parser token ';'
 #0 0x0000000003c70008 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c70008)
 #1 0x0000000003c6d9dc llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c6d9dc)
 #2 0x0000000003bbd9f8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x0000772e19842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x0000000000c6a86a clang::interp::EvaluationResult::checkFullyInitialized(clang::interp::InterpState&, clang::interp::Pointer const&) const (.cold) EvaluationResult.cpp:0:0
 #5 0x000000000742789e bool clang::interp::EvalEmitter::emitRet<(clang::interp::PrimType)13>(clang::interp::SourceInfo) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x742789e)
 #6 0x0000000007487088 clang::interp::EvalEmitter::emitRet(clang::interp::PrimType, clang::interp::SourceInfo) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x7487088)
 #7 0x00000000077d67d7 clang::interp::Compiler<clang::interp::EvalEmitter>::visitExpr(clang::Expr const*, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x77d67d7)
 #8 0x000000000740a8c3 clang::interp::EvalEmitter::interpretExpr(clang::Expr const*, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x740a8c3)
 #9 0x00000000073fcd81 clang::interp::Context::evaluate(clang::interp::State&, clang::Expr const*, clang::APValue&, clang::Expr::ConstantExprKind) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x73fcd81)
#10 0x00000000073e76dd clang::Expr::EvaluateAsConstantExpr(clang::Expr::EvalResult&, clang::ASTContext const&, clang::Expr::ConstantExprKind) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x73e76dd)
#11 0x00000000066a1d57 EvaluateAndDiagnoseImmediateInvocation(clang::Sema&, llvm::PointerIntPair<clang::ConstantExpr*, 1u, unsigned int, llvm::PointerLikeTypeTraits<clang::ConstantExpr*>, llvm::PointerIntPairInfo<clang::ConstantExpr*, 1u, llvm::PointerLikeTypeTraits<clang::ConstantExpr*>>>) SemaExpr.cpp:0:0
#12 0x00000000066f8d4e clang::Sema::PopExpressionEvaluationContext() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x66f8d4e)
#13 0x00000000061e762e clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61e762e)
#14 0x00000000061fc35c clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61fc35c)
#15 0x00000000061a88ff clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61a88ff)
#16 0x00000000061a96f7 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61a96f7)
#17 0x00000000061b234f clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61b234f)
#18 0x00000000061b3b57 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61b3b57)
#19 0x000000000618c60a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x618c60a)
#20 0x000000000453b45d clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x453b45d)
#21 0x000000000483752a clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x483752a)
#22 0x00000000047b1c1b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x47b1c1b)
#23 0x000000000492b0cb clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x492b0cb)
#24 0x0000000000dc8055 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdc8055)
#25 0x0000000000dbffab ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#26 0x0000000000dc004d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#27 0x00000000045a9089 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#28 0x0000000003bbde13 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3bbde13)
#29 0x00000000045a92a9 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#30 0x000000000456c3d2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x456c3d2)
#31 0x000000000456d2b1 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x456d2b1)
#32 0x0000000004575e0c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4575e0c)
#33 0x0000000000dc4aa1 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdc4aa1)
#34 0x0000000000c728b4 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc728b4)
#35 0x0000772e19829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#36 0x0000772e19829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#37 0x0000000000dbfa45 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdbfa45)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```

## Comments

### Comment 1 - k-arrows

Goes back to clang-20.1.0:
https://godbolt.org/z/rdEzY15Wr

---

