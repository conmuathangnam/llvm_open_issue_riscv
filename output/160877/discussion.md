# [clang][bytecode] crash on invalid pointer-to-member cast inside a static_assert

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/160877
**Status:** Open
**Labels:** crash, clang:bytecode

## Body

Reproducer:
https://godbolt.org/z/zqan4Px7s
```cpp
struct S1 {};
struct S2 : S1 {
  int i;
  static_assert((int S1::*)&S2::i, "");
};
```

Backtrace:
```console
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -fexperimental-new-constant-interpreter <source>
1.	<source>:5:1: current parser token '}'
2.	<source>:2:1: parsing struct/union/class body 'S2'
 #0 0x0000000003c877b8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c877b8)
 #1 0x0000000003c8518c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c8518c)
 #2 0x0000000003bd5038 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007f7401042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x000000000742a8b0 clang::interp::Context::collectBaseOffset(clang::RecordDecl const*, clang::RecordDecl const*) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x742a8b0)
 #5 0x00000000078245fd clang::interp::Compiler<clang::interp::EvalEmitter>::VisitCastExpr(clang::CastExpr const*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x78245fd)
 #6 0x000000000782321b clang::interp::Compiler<clang::interp::EvalEmitter>::visit(clang::Expr const*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x782321b)
 #7 0x0000000007823a58 clang::interp::Compiler<clang::interp::EvalEmitter>::VisitCastExpr(clang::CastExpr const*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x7823a58)
 #8 0x000000000782321b clang::interp::Compiler<clang::interp::EvalEmitter>::visit(clang::Expr const*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x782321b)
 #9 0x000000000781925f clang::interp::Compiler<clang::interp::EvalEmitter>::visitExpr(clang::Expr const*, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x781925f)
#10 0x00000000074384c3 clang::interp::EvalEmitter::interpretExpr(clang::Expr const*, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x74384c3)
#11 0x0000000007429656 clang::interp::Context::evaluateAsRValue(clang::interp::State&, clang::Expr const*, clang::APValue&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x7429656)
#12 0x00000000073a0c1a EvaluateAsRValue((anonymous namespace)::EvalInfo&, clang::Expr const*, clang::APValue&) ExprConstant.cpp:0:0
#13 0x00000000073a59d8 clang::Expr::EvaluateAsRValue(clang::Expr::EvalResult&, clang::ASTContext const&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x73a59d8)
#14 0x000000000672b548 clang::Sema::VerifyIntegerConstantExpression(clang::Expr*, llvm::APSInt*, clang::Sema::VerifyICEDiagnoser&, clang::AllowFoldKind) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x672b548)
#15 0x000000000672c092 clang::Sema::VerifyIntegerConstantExpression(clang::Expr*, llvm::APSInt*, unsigned int, clang::AllowFoldKind) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x672c092)
#16 0x0000000006644d7b clang::Sema::BuildStaticAssertDeclaration(clang::SourceLocation, clang::Expr*, clang::Expr*, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6644d7b)
#17 0x0000000006238348 clang::Parser::ParseStaticAssertDeclaration(clang::SourceLocation&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6238348)
#18 0x000000000624630f clang::Parser::ParseCXXClassMemberDeclaration(clang::AccessSpecifier, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::ParsingDeclRAIIObject*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x624630f)
#19 0x0000000006248f80 clang::Parser::ParseCXXClassMemberDeclarationWithPragmas(clang::AccessSpecifier&, clang::ParsedAttributes&, clang::TypeSpecifierType, clang::Decl*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6248f80)
#20 0x0000000006249604 clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&, unsigned int, clang::Decl*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6249604)
#21 0x000000000624b803 clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x624b803)
#22 0x0000000006213edf clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6213edf)
#23 0x00000000061cee9f clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61cee9f)
#24 0x00000000061d0047 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61d0047)
#25 0x00000000061d8c9f clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61d8c9f)
#26 0x00000000061da4a7 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61da4a7)
#27 0x00000000061b2f5a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61b2f5a)
#28 0x0000000004554515 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4554515)
#29 0x000000000484f69a clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x484f69a)
#30 0x00000000047c9d9b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x47c9d9b)
#31 0x000000000494513b clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x494513b)
#32 0x0000000000dc6965 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdc6965)
#33 0x0000000000dbe8bb ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#34 0x0000000000dbe95d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#35 0x00000000045c1e39 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#36 0x0000000003bd5453 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3bd5453)
#37 0x00000000045c2059 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#38 0x00000000045851a2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45851a2)
#39 0x0000000004586081 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4586081)
#40 0x000000000458ebcc clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x458ebcc)
#41 0x0000000000dc33b9 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdc33b9)
#42 0x0000000000c70da4 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc70da4)
#43 0x00007f7401029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#44 0x00007f7401029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#45 0x0000000000dbe355 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdbe355)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```

## Comments

### Comment 1 - k-arrows

Goes back to clang-19.1.0:
https://godbolt.org/z/na1vE6fo5

---

### Comment 2 - Rajveer100

cc @llvm/pr-subscribers-clang 

---

