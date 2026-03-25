# [clang][bytecode] Assertion `!ItemTypes.empty()' failed where expression is not an integral constant expression

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/177751
**Status:** Closed
**Labels:** crash, clang:bytecode
**Closed Date:** 2026-01-26T06:54:23Z

## Body

Reproducer:
https://godbolt.org/z/Mcz1Wh749
```cpp
enum { E = (__uint128_t)-1. };
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/AST/ByteCode/InterpStack.h:40: T clang::interp::InterpStack::pop() [with T = clang::interp::IntegralAP<false>]: Assertion `!ItemTypes.empty()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -fexperimental-new-constant-interpreter <source>
1.	<source>:1:29: current parser token '}'
 #0 0x0000000004309af8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4309af8)
 #1 0x0000000004306f54 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4306f54)
 #2 0x0000000004249938 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x0000706bfea42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x0000706bfea969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x0000706bfea42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x0000706bfea287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x0000706bfea2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x0000706bfea39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000007e8d393 clang::interp::IntegralAP<false> clang::interp::InterpStack::pop<clang::interp::IntegralAP<false>>() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7e8d393)
#10 0x0000000007e976af clang::interp::EvalEmitter::emitRet(clang::interp::PrimType, clang::interp::SourceInfo) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7e976af)
#11 0x00000000082b3883 clang::interp::Compiler<clang::interp::EvalEmitter>::visitExpr(clang::Expr const*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x82b3883)
#12 0x0000000007e2ad6d clang::interp::EvalEmitter::interpretExpr(clang::Expr const*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7e2ad6d)
#13 0x0000000007e1afd3 clang::interp::Context::evaluateAsRValue(clang::interp::State&, clang::Expr const*, clang::APValue&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7e1afd3)
#14 0x0000000007d883f3 EvaluateAsRValue((anonymous namespace)::EvalInfo&, clang::Expr const*, clang::APValue&) ExprConstant.cpp:0:0
#15 0x0000000007d908d7 clang::Expr::EvaluateAsRValue(clang::Expr::EvalResult&, clang::ASTContext const&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7d908d7)
#16 0x0000000006fdb761 clang::Sema::VerifyIntegerConstantExpression(clang::Expr*, llvm::APSInt*, clang::Sema::VerifyICEDiagnoser&, clang::AllowFoldKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6fdb761)
#17 0x0000000006fdc6af clang::Sema::VerifyIntegerConstantExpression(clang::Expr*, llvm::APSInt*, clang::AllowFoldKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6fdc6af)
#18 0x0000000006e13975 clang::Sema::CheckEnumConstant(clang::EnumDecl*, clang::EnumConstantDecl*, clang::SourceLocation, clang::IdentifierInfo*, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e13975)
#19 0x0000000006e3cdfe clang::Sema::ActOnEnumConstant(clang::Scope*, clang::Decl*, clang::Decl*, clang::SourceLocation, clang::IdentifierInfo*, clang::ParsedAttributesView const&, clang::SourceLocation, clang::Expr*, clang::SkipBodyInfo*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e3cdfe)
#20 0x0000000006ab9e2d clang::Parser::ParseEnumBody(clang::SourceLocation, clang::Decl*, clang::SkipBodyInfo*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ab9e2d)
#21 0x0000000006ac24d4 clang::Parser::ParseEnumSpecifier(clang::SourceLocation, clang::DeclSpec&, clang::Parser::ParsedTemplateInfo const&, clang::AccessSpecifier, clang::Parser::DeclSpecContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ac24d4)
#22 0x0000000006abe5aa clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6abe5aa)
#23 0x0000000006a78a57 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a78a57)
#24 0x0000000006a794cf clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a794cf)
#25 0x0000000006a814d1 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a814d1)
#26 0x0000000006a823e5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a823e5)
#27 0x0000000006a827d0 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a827d0)
#28 0x0000000006a630c3 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a630c3)
#29 0x0000000004c731e8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c731e8)
#30 0x0000000004f6e525 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f6e525)
#31 0x0000000004eedf4e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4eedf4e)
#32 0x00000000050696ed clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x50696ed)
#33 0x0000000000ddd95e cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xddd95e)
#34 0x0000000000dd432a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#35 0x0000000000dd44ad int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#36 0x0000000004ce6c29 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#37 0x0000000004249dd4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4249dd4)
#38 0x0000000004ce723f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#39 0x0000000004ca7c12 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ca7c12)
#40 0x0000000004ca8bbe clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ca8bbe)
#41 0x0000000004cb0005 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4cb0005)
#42 0x0000000000dd9cd1 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdd9cd1)
#43 0x0000000000c96d24 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc96d24)
#44 0x0000706bfea29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#45 0x0000706bfea29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#46 0x0000000000dd3dc5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdd3dc5)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

