# Type pack indexing inside decltype crashes clang

**Author:** OgnianM
**URL:** https://github.com/llvm/llvm-project/issues/119072
**Status:** Closed
**Labels:** clang:frontend, confirmed, crash
**Closed Date:** 2024-12-09T01:58:38Z

## Body

https://godbolt.org/z/zE6aPh7Ms

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Ognyan Mirev (OgnianM)

<details>
https://godbolt.org/z/zE6aPh7Ms
</details>


---

### Comment 2 - zwuis

Failed assertion:
```txt
clang++: /root/llvm-project/clang/include/clang/Lex/Token.h:190: clang::IdentifierInfo* clang::Token::getIdentifierInfo() const: Assertion `!isAnnotation() && "getIdentifierInfo() on an annotation token!"' failed.
```
Stack dump:
```txt
0.  Program arguments: /opt/compiler-explorer/clang-assertions-19.1.0/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-14.2.0 -fcolor-diagnostics -fno-crash-diagnostics -std=c++26 <source>
1.  <source>:7:14: at annotation token
2.  <source>:6:12: parsing function body 'foo'
3.  <source>:6:12: in compound statement ('{}')
#0 0x0000000003ae9978 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x3ae9978)
#1 0x0000000003ae766c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x3ae766c)
#2 0x0000000003a308f8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
#3 0x0000714ebb642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
#4 0x0000714ebb6969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
#5 0x0000714ebb642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
#6 0x0000714ebb6287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
#7 0x0000714ebb62871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
#8 0x0000714ebb639e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
#9 0x00000000064cda81 clang::Token::getIdentifierInfo() const (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x64cda81)
#10 0x00000000065480b0 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x65480b0)
#11 0x0000000006548f7a clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x6548f7a)
#12 0x0000000006549119 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x6549119)
#13 0x000000000654d9a9 clang::Parser::ParseExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x654d9a9)
#14 0x0000000006529f1b clang::Parser::ParseDecltypeSpecifier(clang::DeclSpec&) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x6529f1b)
#15 0x0000000006560acf clang::Parser::ParseOptionalCXXScopeSpecifier(clang::CXXScopeSpec&, clang::OpaquePtr<clang::QualType>, bool, bool, bool*, bool, clang::IdentifierInfo const**, bool, bool) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x6560acf)
#16 0x00000000064d2aed clang::Parser::TryAnnotateTypeOrScopeToken(clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x64d2aed)
#17 0x00000000065e90fd clang::Parser::isCXXDeclarationSpecifier(clang::ImplicitTypenameContext, clang::Parser::TPResult, bool*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x65e90fd)
#18 0x00000000065ef2f7 clang::Parser::isCXXDeclarationStatement(bool) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x65ef2f7)
#19 0x00000000065c48b7 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x65c48b7)
#20 0x00000000065c5cc0 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x65c5cc0)
#21 0x00000000065c6ba9 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x65c6ba9)
#22 0x00000000065c837a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x65c837a)
#23 0x00000000064dd6c3 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x64dd6c3)
#24 0x0000000006512708 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x6512708)
#25 0x00000000065d9666 clang::Parser::ParseDeclarationAfterTemplate(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo&, clang::ParsingDeclRAIIObject&, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x65d9666)
#26 0x00000000065e6c54 clang::Parser::ParseTemplateDeclarationOrSpecialization(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (.part.0) ParseTemplate.cpp:0:0
#27 0x00000000065e6f6a clang::Parser::ParseDeclarationStartingWithTemplate(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x65e6f6a)
#28 0x000000000651b343 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x651b343)
#29 0x00000000064d80c1 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x64d80c1)
#30 0x00000000064d8f6f clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x64d8f6f)
#31 0x00000000064cc33a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x64cc33a)
#32 0x00000000043f9448 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x43f9448)
#33 0x0000000004684f59 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x4684f59)
#34 0x0000000004609f8e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x4609f8e)
#35 0x000000000476e2be clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x476e2be)
#36 0x0000000000c82b7f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0xc82b7f)
#37 0x0000000000c7bcda ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#38 0x000000000443cba9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#39 0x0000000003a30da4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x3a30da4)
#40 0x000000000443d19f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#41 0x0000000004402c05 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x4402c05)
#42 0x000000000440366d clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x440366d)
#43 0x000000000440b075 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x440b075)
#44 0x0000000000c7fe95 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0xc7fe95)
#45 0x0000000000b547a4 main (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0xb547a4)
#46 0x0000714ebb629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#47 0x0000714ebb629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#48 0x0000000000c7b78e _start (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0xc7b78e)
```

---

