# [clang][bytecode] crash on plus_minus_equal_month.pass.cpp in libcxx test

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/170981
**Status:** Closed
**Labels:** crash, clang:bytecode
**Closed Date:** 2025-12-09T07:03:29Z

## Body

Here is the reduced reproducer:
https://godbolt.org/z/ja81zo8Mr
```cpp
#include <chrono>

using month = std::chrono::month;
using months = std::chrono::months;
using year = std::chrono::year;
using year_month = std::chrono::year_month;

constexpr bool test() {
  for (unsigned i = 0; i <= 10; ++i) {
    year y{1234};
    year_month ym(y, month{i});
  }

  year_month ym{year{2020}, month{4}};
  ym += months{12};

  return true;
}

int main(int, char **) {
  static_assert(test());

  return 0;
}
```

Backtrace:
```console
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 -fexperimental-new-constant-interpreter <source>
1.	<source>:23:3: current parser token 'return'
2.	<source>:20:24: parsing function body 'main'
3.	<source>:20:24: in compound statement ('{}')
 #0 0x0000000003cff998 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3cff998)
 #1 0x0000000003cfd36c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3cfd36c)
 #2 0x0000000003c443e8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x0000719cf5442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x0000000007515918 clang::interp::Integral<16u, true>& clang::interp::Pointer::deref<clang::interp::Integral<16u, true>>() const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x7515918)
 #5 0x0000000007536f1f bool clang::interp::InitThisField<(clang::interp::PrimType)2, clang::interp::Integral<16u, true>>(clang::interp::InterpState&, clang::interp::CodePtr, unsigned int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x7536f1f)
 #6 0x00000000076b1f39 clang::interp::Interpret(clang::interp::InterpState&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x76b1f39)
 #7 0x00000000076bc249 clang::interp::Call(clang::interp::InterpState&, clang::interp::CodePtr, clang::interp::Function const*, unsigned int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x76bc249)
 #8 0x00000000076ba407 clang::interp::Interpret(clang::interp::InterpState&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x76ba407)
 #9 0x00000000076bc249 clang::interp::Call(clang::interp::InterpState&, clang::interp::CodePtr, clang::interp::Function const*, unsigned int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x76bc249)
#10 0x00000000076ba407 clang::interp::Interpret(clang::interp::InterpState&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x76ba407)
#11 0x00000000076bc249 clang::interp::Call(clang::interp::InterpState&, clang::interp::CodePtr, clang::interp::Function const*, unsigned int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x76bc249)
#12 0x00000000076ba407 clang::interp::Interpret(clang::interp::InterpState&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x76ba407)
#13 0x00000000076bc249 clang::interp::Call(clang::interp::InterpState&, clang::interp::CodePtr, clang::interp::Function const*, unsigned int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x76bc249)
#14 0x00000000076ba407 clang::interp::Interpret(clang::interp::InterpState&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x76ba407)
#15 0x00000000076bc249 clang::interp::Call(clang::interp::InterpState&, clang::interp::CodePtr, clang::interp::Function const*, unsigned int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x76bc249)
#16 0x00000000078fd16e clang::interp::Compiler<clang::interp::EvalEmitter>::VisitCallExpr(clang::CallExpr const*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x78fd16e)
#17 0x000000000790376b clang::interp::Compiler<clang::interp::EvalEmitter>::visit(clang::Expr const*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x790376b)
#18 0x00000000078f8fbf clang::interp::Compiler<clang::interp::EvalEmitter>::visitExpr(clang::Expr const*, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x78f8fbf)
#19 0x00000000074f1303 clang::interp::EvalEmitter::interpretExpr(clang::Expr const*, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x74f1303)
#20 0x00000000074e2816 clang::interp::Context::evaluateAsRValue(clang::interp::State&, clang::Expr const*, clang::APValue&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x74e2816)
#21 0x000000000744918a EvaluateAsRValue((anonymous namespace)::EvalInfo&, clang::Expr const*, clang::APValue&) ExprConstant.cpp:0:0
#22 0x000000000744e8f0 clang::Expr::EvaluateAsRValue(clang::Expr::EvalResult&, clang::ASTContext const&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x744e8f0)
#23 0x0000000006789478 clang::Sema::VerifyIntegerConstantExpression(clang::Expr*, llvm::APSInt*, clang::Sema::VerifyICEDiagnoser&, clang::AllowFoldKind) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6789478)
#24 0x0000000006789fb2 clang::Sema::VerifyIntegerConstantExpression(clang::Expr*, llvm::APSInt*, unsigned int, clang::AllowFoldKind) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6789fb2)
#25 0x00000000066a92ec clang::Sema::BuildStaticAssertDeclaration(clang::SourceLocation, clang::Expr*, clang::Expr*, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x66a92ec)
#26 0x00000000062793b8 clang::Parser::ParseStaticAssertDeclaration(clang::SourceLocation&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62793b8)
#27 0x0000000006266f2b clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6266f2b)
#28 0x00000000063237ef clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::LabelDecl*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x63237ef)
#29 0x0000000006324136 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6324136)
#30 0x000000000632d699 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x632d699)
#31 0x000000000632e13a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x632e13a)
#32 0x0000000006217b5b clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6217b5b)
#33 0x000000000626442f clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x626442f)
#34 0x0000000006210f37 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6210f37)
#35 0x00000000062116d7 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62116d7)
#36 0x000000000621a0af clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x621a0af)
#37 0x000000000621b8b7 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x621b8b7)
#38 0x00000000061f46da clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61f46da)
#39 0x00000000045b54c5 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45b54c5)
#40 0x00000000048c3e4a clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x48c3e4a)
#41 0x0000000004842cbb clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4842cbb)
#42 0x00000000049b89ab clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x49b89ab)
#43 0x0000000000dc9255 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdc9255)
#44 0x0000000000dc110b ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#45 0x0000000000dc11ad int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#46 0x000000000462fa69 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#47 0x0000000003c44803 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c44803)
#48 0x000000000462fc89 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#49 0x00000000045f2612 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45f2612)
#50 0x00000000045f34f1 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45f34f1)
#51 0x00000000045fc0dc clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45fc0dc)
#52 0x0000000000dc5bd9 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdc5bd9)
#53 0x0000000000c77d44 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc77d44)
#54 0x0000719cf5429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#55 0x0000719cf5429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#56 0x0000000000dc0ba5 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdc0ba5)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```

## Comments

### Comment 1 - k-arrows

This crash is trunk only:
https://godbolt.org/z/rovbdTY6n

---

