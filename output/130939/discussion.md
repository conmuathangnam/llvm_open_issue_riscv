# [clangd] Crashes when opening a file with invalid escape sequence

**Author:** henryhchchc
**URL:** https://github.com/llvm/llvm-project/issues/130939
**Status:** Closed
**Labels:** duplicate, clang:frontend, crash-on-invalid
**Closed Date:** 2025-03-23T01:46:29Z

## Body

```c
void foo() {
    printf("\o{}");
}
```

The code snippet above causes a crash in `clangd` when generating inlay hints. The crash occurs only when the malformed `printf` is inside a function.

The crash log is as follows

```log

I[18:28:11.499] clangd version 19.1.7
I[18:28:11.499] Features: linux
I[18:28:11.499] PID: 1610337
I[18:28:11.499] Working directory: /path/to/workspace
I[18:28:11.499] argv[0]: /nix/store/xkb7pp8hidrq6yam5i23csp4y1azk29d-clang-tools-19.1.7/bin/clangd
I[18:28:11.499] Starting LSP over stdin/stdout
I[18:28:11.499] <-- initialize(1)

I[18:28:11.507] --> reply:initialize(1) 7 ms

I[18:28:11.670] <-- initialized
I[18:28:11.670] <-- textDocument/didOpen

I[18:28:11.671] Failed to find compilation database for /path/to/main.c
I[18:28:11.671] ASTWorker building file /path/to/main.c version 0 with command clangd fallback
[/path/to/workspace]
/usr/bin/clang -resource-dir=/nix/store/hwp5wkjs8kf8nvvkq3byv3zid7nclrvp-clang-19.1.7/lib/clang/19 -- /path/to/main.c
I[18:28:11.671] <-- textDocument/inlayHint(2)

I[18:28:11.672] <-- textDocument/inlayHint(3)

I[18:28:11.678] Built preamble of size 232664 for file /path/to/main.c version 0 in 0.01 seconds
I[18:28:11.678] --> workspace/semanticTokens/refresh(0)
I[18:28:11.679] Indexing c17 standard library in the context of /path/to/main.c

PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.	/path/to/main.c:2:16: current parser token ')'
1.	/path/to/main.c:1:12: parsing function body 'foo'
2.	/path/to/main.c:1:12: in compound statement ('{}')

I[18:28:11.694] <-- textDocument/inlayHint(4)

I[18:28:11.695] <-- textDocument/semanticTokens/full(5)

I[18:28:11.695] <-- $/cancelRequest
I[18:28:11.695] <-- textDocument/semanticTokens/full(6)

I[18:28:11.695] <-- reply(0)

I[18:28:11.757] Indexed c17 standard library: 3493 symbols, 42 filtered

 #0 0x00007fffeafee11c llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/nix/store/sj58s6n3rhvp21b4zmh7igbwzgvh8kwz-llvm-19.1.7-lib/lib/libLLVM.so.19.1+0xfee11c)
 #1 0x00007fffeafeb55b SignalHandler(int) (/nix/store/sj58s6n3rhvp21b4zmh7igbwzgvh8kwz-llvm-19.1.7-lib/lib/libLLVM.so.19.1+0xfeb55b)
 #2 0x00007fffe9840f30 __restore_rt (/nix/store/6q2mknq81cyscjmkv72fpcsvan56qhmg-glibc-2.40-66/lib/libc.so.6+0x40f30)
 #3 0x00007ffff422cfb7 Diag(clang::DiagnosticsEngine*, clang::LangOptions const&, clang::FullSourceLoc, char const*, char const*, char const*, unsigned int) (.isra.0) (/nix/store/j6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0xc2cfb7)
 #4 0x00007ffff422fe55 ProcessCharEscape(char const*, char const*&, char const*, bool&, clang::FullSourceLoc, unsigned int, clang::DiagnosticsEngine*, clang::LangOptions const&, clang::StringLiteralEvalMethod) (.isra.0) (/nix/store/j6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0xc2fe55)
 #5 0x00007ffff423bcb0 clang::StringLiteralParser::init(llvm::ArrayRef<clang::Token>) (/nix/store/j6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0xc3bcb0)
 #6 0x00007ffff466c1cf clang::StringLiteral::getLocationOfByte(unsigned int, clang::SourceManager const&, clang::LangOptions const&, clang::TargetInfo const&, unsigned int*, unsigned int*) const (/nix/store/j6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0x106c1cf)
 #7 0x00007ffff4e6bc35 (anonymous namespace)::CheckFormatHandler::HandleNullChar(char const*) (/nix/store/j6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0x186bc35)
 #8 0x00007ffff49cf329 clang::analyze_format_string::ParsePrintfString(clang::analyze_format_string::FormatStringHandler&, char const*, char const*, clang::LangOptions const&, clang::TargetInfo const&, bool) (/nix/store/j6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0x13cf329)
 #9 0x00007ffff4e6d366 checkFormatStringExpr(clang::Sema&, clang::Expr const*, llvm::ArrayRef<clang::Expr const*>, clang::Sema::FormatArgumentPassingKind, unsigned int, unsigned int, clang::Sema::FormatStringType, clang::Sema::VariadicCallType, bool, llvm::SmallBitVector&, (anonymous namespace)::UncoveredArgHandler&, llvm::APSInt, bool) (/nix/store/j6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0x186d366)
#10 0x00007ffff4e6d9c0 clang::Sema::CheckFormatArguments(llvm::ArrayRef<clang::Expr const*>, clang::Sema::FormatArgumentPassingKind, unsigned int, unsigned int, clang::Sema::FormatStringType, clang::Sema::VariadicCallType, clang::SourceLocation, clang::SourceRange, llvm::SmallBitVector&) (/nix/store/j6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0x186d9c0)
#11 0x00007ffff4e6dea2 clang::Sema::CheckFormatArguments(clang::FormatAttr const*, llvm::ArrayRef<clang::Expr const*>, bool, clang::Sema::VariadicCallType, clang::SourceLocation, clang::SourceRange, llvm::SmallBitVector&) (/nix/store/j6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0x186dea2)
#12 0x00007ffff4e6e0d7 clang::Sema::checkCall(clang::NamedDecl*, clang::FunctionProtoType const*, clang::Expr const*, llvm::ArrayRef<clang::Expr const*>, bool, clang::SourceLocation, clang::SourceRange, clang::Sema::VariadicCallType) (/nix/store/j6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0x186e0d7)
#13 0x00007ffff4e7b559 clang::Sema::CheckFunctionCall(clang::FunctionDecl*, clang::CallExpr*, clang::FunctionProtoType const*) (/nix/store/j6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0x187b559)
#14 0x00007ffff5191104 clang::Sema::BuildResolvedCallExpr(clang::Expr*, clang::NamedDecl*, clang::SourceLocation, llvm::ArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, clang::CallExpr::ADLCallKind) (/nix/store/j6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0x1b91104)
#15 0x00007ffff51948b1 clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) (/nix/store/j6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0x1b948b1)
#16 0x00007ffff433116b clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult<clang::Expr*, true>) (/nix/store/j6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0xd3116b)
#17 0x00007ffff432a227 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) (/nix/store/j6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0xd2a227)
#18 0x00007ffff432c6e7 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) (/nix/store/j6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0xd2c6e7)
#19 0x00007ffff432c8b8 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/nix/store/j6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0xd2c8b8)
#20 0x00007ffff4330e49 clang::Parser::ParseExpression(clang::Parser::TypeCastState) (/nix/store/j6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0xd30e49)
#21 0x00007ffff43abd2b clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/nix/store/j6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0xdabd2b)
#22 0x00007ffff43ad551 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/nix/store/j6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0xdad551)
#23 0x00007ffff43ae07a clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/nix/store/j6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0xdae07a)
#24 0x00007ffff43af369 clang::Parser::ParseCompoundStatementBody(bool) (/nix/store/j6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0xdaf369)
#25 0x00007ffff43afdea clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/nix/store/j6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0xdafdea)
#26 0x00007ffff43e76a2 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/nix/store/j6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0xde76a2)
#27 0x00007ffff42f72d4 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/nix/store/j6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0xcf72d4)
#28 0x00007ffff43dfd51 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/nix/store/j6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0xddfd51)
#29 0x00007ffff43e0b4d clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/nix/store/j6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0xde0b4d)
#30 0x00007ffff43e9ced clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/nix/store/j6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0xde9ced)
#31 0x00007ffff43eae05 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/nix/store/j6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0xdeae05)
#32 0x00007ffff43eb32d clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/nix/store/j
6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0xdeb32d)
#33 0x00007ffff42c26e0 clang::ParseAST(clang::Sema&, bool, bool) (/nix/store/j6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0xcc26e0)
#34 0x00007ffff67dda69 clang::FrontendAction::Execute() (/nix/store/j6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0x31dda69)
#35 0x000000000076ffdf (/nix/store/xkb7pp8hidrq6yam5i23csp4y1azk29d-clang-tools-19.1.7/bin/clangd+0x76ffdf)
#36 0x00000000007fdbb9 (/nix/store/xkb7pp8hidrq6yam5i23csp4y1azk29d-clang-tools-19.1.7/bin/clangd+0x7fdbb9)
#37 0x00000000007fe4cf (/nix/store/xkb7pp8hidrq6yam5i23csp4y1azk29d-clang-tools-19.1.7/bin/clangd+0x7fe4cf)
#38 0x00000000007f7b33 (/nix/store/xkb7pp8hidrq6yam5i23csp4y1azk29d-clang-tools-19.1.7/bin/clangd+0x7f7b33)
#39 0x00000000007f7e9a (/nix/store/xkb7pp8hidrq6yam5i23csp4y1azk29d-clang-tools-19.1.7/bin/clangd+0x7f7e9a)
#40 0x0000000000a386e9 (/nix/store/xkb7pp8hidrq6yam5i23csp4y1azk29d-clang-tools-19.1.7/bin/clangd+0xa386e9)
#41 0x00007fffe98972e3 start_thread (/nix/store/6q2mknq81cyscjmkv72fpcsvan56qhmg-glibc-2.40-66/lib/libc.so.6+0x972e3)
#42 0x00007fffe991b2fc __GI___clone3 (/nix/store/6q2mknq81cyscjmkv72fpcsvan56qhmg-glibc-2.40-66/lib/libc.so.6+0x11b2fc)
Signalled during AST worker action: Build AST
  Filename: main.c
  Directory: /path/to
  Command Line: /usr/bin/clang -resource-dir=/nix/store/hwp5wkjs8kf8nvvkq3byv3zid7nclrvp-clang-19.1.7/lib/clang/19 -- /path/to/main.c
  Version: 0
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clangd

Author: Henry Chu (henryhchchc)

<details>
```c
void foo() {
    printf("\o{}");
}
```

The code snippet above causes a crash in `clangd` when generating inlay hints. The crash occurs only when the malformed `printf` is inside a function.

The crash log is as follows

```log

I[18:28:11.499] clangd version 19.1.7
I[18:28:11.499] Features: linux
I[18:28:11.499] PID: 1610337
I[18:28:11.499] Working directory: /path/to/workspace
I[18:28:11.499] argv[0]: /nix/store/xkb7pp8hidrq6yam5i23csp4y1azk29d-clang-tools-19.1.7/bin/clangd
I[18:28:11.499] Starting LSP over stdin/stdout
I[18:28:11.499] &lt;-- initialize(1)

I[18:28:11.507] --&gt; reply:initialize(1) 7 ms

I[18:28:11.670] &lt;-- initialized
I[18:28:11.670] &lt;-- textDocument/didOpen

I[18:28:11.671] Failed to find compilation database for /path/to/main.c
I[18:28:11.671] ASTWorker building file /path/to/main.c version 0 with command clangd fallback
[/path/to/workspace]
/usr/bin/clang -resource-dir=/nix/store/hwp5wkjs8kf8nvvkq3byv3zid7nclrvp-clang-19.1.7/lib/clang/19 -- /path/to/main.c
I[18:28:11.671] &lt;-- textDocument/inlayHint(2)

I[18:28:11.672] &lt;-- textDocument/inlayHint(3)

I[18:28:11.678] Built preamble of size 232664 for file /path/to/main.c version 0 in 0.01 seconds
I[18:28:11.678] --&gt; workspace/semanticTokens/refresh(0)
I[18:28:11.679] Indexing c17 standard library in the context of /path/to/main.c

PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.	/path/to/main.c:2:16: current parser token ')'
1.	/path/to/main.c:1:12: parsing function body 'foo'
2.	/path/to/main.c:1:12: in compound statement ('{}')

I[18:28:11.694] &lt;-- textDocument/inlayHint(4)

I[18:28:11.695] &lt;-- textDocument/semanticTokens/full(5)

I[18:28:11.695] &lt;-- $/cancelRequest
I[18:28:11.695] &lt;-- textDocument/semanticTokens/full(6)

I[18:28:11.695] &lt;-- reply(0)

I[18:28:11.757] Indexed c17 standard library: 3493 symbols, 42 filtered

 #<!-- -->0 0x00007fffeafee11c llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/nix/store/sj58s6n3rhvp21b4zmh7igbwzgvh8kwz-llvm-19.1.7-lib/lib/libLLVM.so.19.1+0xfee11c)
 #<!-- -->1 0x00007fffeafeb55b SignalHandler(int) (/nix/store/sj58s6n3rhvp21b4zmh7igbwzgvh8kwz-llvm-19.1.7-lib/lib/libLLVM.so.19.1+0xfeb55b)
 #<!-- -->2 0x00007fffe9840f30 __restore_rt (/nix/store/6q2mknq81cyscjmkv72fpcsvan56qhmg-glibc-2.40-66/lib/libc.so.6+0x40f30)
 #<!-- -->3 0x00007ffff422cfb7 Diag(clang::DiagnosticsEngine*, clang::LangOptions const&amp;, clang::FullSourceLoc, char const*, char const*, char const*, unsigned int) (.isra.0) (/nix/store/j6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0xc2cfb7)
 #<!-- -->4 0x00007ffff422fe55 ProcessCharEscape(char const*, char const*&amp;, char const*, bool&amp;, clang::FullSourceLoc, unsigned int, clang::DiagnosticsEngine*, clang::LangOptions const&amp;, clang::StringLiteralEvalMethod) (.isra.0) (/nix/store/j6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0xc2fe55)
 #<!-- -->5 0x00007ffff423bcb0 clang::StringLiteralParser::init(llvm::ArrayRef&lt;clang::Token&gt;) (/nix/store/j6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0xc3bcb0)
 #<!-- -->6 0x00007ffff466c1cf clang::StringLiteral::getLocationOfByte(unsigned int, clang::SourceManager const&amp;, clang::LangOptions const&amp;, clang::TargetInfo const&amp;, unsigned int*, unsigned int*) const (/nix/store/j6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0x106c1cf)
 #<!-- -->7 0x00007ffff4e6bc35 (anonymous namespace)::CheckFormatHandler::HandleNullChar(char const*) (/nix/store/j6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0x186bc35)
 #<!-- -->8 0x00007ffff49cf329 clang::analyze_format_string::ParsePrintfString(clang::analyze_format_string::FormatStringHandler&amp;, char const*, char const*, clang::LangOptions const&amp;, clang::TargetInfo const&amp;, bool) (/nix/store/j6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0x13cf329)
 #<!-- -->9 0x00007ffff4e6d366 checkFormatStringExpr(clang::Sema&amp;, clang::Expr const*, llvm::ArrayRef&lt;clang::Expr const*&gt;, clang::Sema::FormatArgumentPassingKind, unsigned int, unsigned int, clang::Sema::FormatStringType, clang::Sema::VariadicCallType, bool, llvm::SmallBitVector&amp;, (anonymous namespace)::UncoveredArgHandler&amp;, llvm::APSInt, bool) (/nix/store/j6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0x186d366)
#<!-- -->10 0x00007ffff4e6d9c0 clang::Sema::CheckFormatArguments(llvm::ArrayRef&lt;clang::Expr const*&gt;, clang::Sema::FormatArgumentPassingKind, unsigned int, unsigned int, clang::Sema::FormatStringType, clang::Sema::VariadicCallType, clang::SourceLocation, clang::SourceRange, llvm::SmallBitVector&amp;) (/nix/store/j6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0x186d9c0)
#<!-- -->11 0x00007ffff4e6dea2 clang::Sema::CheckFormatArguments(clang::FormatAttr const*, llvm::ArrayRef&lt;clang::Expr const*&gt;, bool, clang::Sema::VariadicCallType, clang::SourceLocation, clang::SourceRange, llvm::SmallBitVector&amp;) (/nix/store/j6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0x186dea2)
#<!-- -->12 0x00007ffff4e6e0d7 clang::Sema::checkCall(clang::NamedDecl*, clang::FunctionProtoType const*, clang::Expr const*, llvm::ArrayRef&lt;clang::Expr const*&gt;, bool, clang::SourceLocation, clang::SourceRange, clang::Sema::VariadicCallType) (/nix/store/j6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0x186e0d7)
#<!-- -->13 0x00007ffff4e7b559 clang::Sema::CheckFunctionCall(clang::FunctionDecl*, clang::CallExpr*, clang::FunctionProtoType const*) (/nix/store/j6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0x187b559)
#<!-- -->14 0x00007ffff5191104 clang::Sema::BuildResolvedCallExpr(clang::Expr*, clang::NamedDecl*, clang::SourceLocation, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, clang::CallExpr::ADLCallKind) (/nix/store/j6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0x1b91104)
#<!-- -->15 0x00007ffff51948b1 clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*) (/nix/store/j6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0x1b948b1)
#<!-- -->16 0x00007ffff433116b clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult&lt;clang::Expr*, true&gt;) (/nix/store/j6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0xd3116b)
#<!-- -->17 0x00007ffff432a227 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) (/nix/store/j6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0xd2a227)
#<!-- -->18 0x00007ffff432c6e7 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) (/nix/store/j6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0xd2c6e7)
#<!-- -->19 0x00007ffff432c8b8 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/nix/store/j6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0xd2c8b8)
#<!-- -->20 0x00007ffff4330e49 clang::Parser::ParseExpression(clang::Parser::TypeCastState) (/nix/store/j6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0xd30e49)
#<!-- -->21 0x00007ffff43abd2b clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/nix/store/j6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0xdabd2b)
#<!-- -->22 0x00007ffff43ad551 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/nix/store/j6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0xdad551)
#<!-- -->23 0x00007ffff43ae07a clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/nix/store/j6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0xdae07a)
#<!-- -->24 0x00007ffff43af369 clang::Parser::ParseCompoundStatementBody(bool) (/nix/store/j6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0xdaf369)
#<!-- -->25 0x00007ffff43afdea clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/nix/store/j6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0xdafdea)
#<!-- -->26 0x00007ffff43e76a2 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/nix/store/j6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0xde76a2)
#<!-- -->27 0x00007ffff42f72d4 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/nix/store/j6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0xcf72d4)
#<!-- -->28 0x00007ffff43dfd51 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/nix/store/j6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0xddfd51)
#<!-- -->29 0x00007ffff43e0b4d clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/nix/store/j6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0xde0b4d)
#<!-- -->30 0x00007ffff43e9ced clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/nix/store/j6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0xde9ced)
#<!-- -->31 0x00007ffff43eae05 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/nix/store/j6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0xdeae05)
#<!-- -->32 0x00007ffff43eb32d clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/nix/store/j
6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0xdeb32d)
#<!-- -->33 0x00007ffff42c26e0 clang::ParseAST(clang::Sema&amp;, bool, bool) (/nix/store/j6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0xcc26e0)
#<!-- -->34 0x00007ffff67dda69 clang::FrontendAction::Execute() (/nix/store/j6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0x31dda69)
#<!-- -->35 0x000000000076ffdf (/nix/store/xkb7pp8hidrq6yam5i23csp4y1azk29d-clang-tools-19.1.7/bin/clangd+0x76ffdf)
#<!-- -->36 0x00000000007fdbb9 (/nix/store/xkb7pp8hidrq6yam5i23csp4y1azk29d-clang-tools-19.1.7/bin/clangd+0x7fdbb9)
#<!-- -->37 0x00000000007fe4cf (/nix/store/xkb7pp8hidrq6yam5i23csp4y1azk29d-clang-tools-19.1.7/bin/clangd+0x7fe4cf)
#<!-- -->38 0x00000000007f7b33 (/nix/store/xkb7pp8hidrq6yam5i23csp4y1azk29d-clang-tools-19.1.7/bin/clangd+0x7f7b33)
#<!-- -->39 0x00000000007f7e9a (/nix/store/xkb7pp8hidrq6yam5i23csp4y1azk29d-clang-tools-19.1.7/bin/clangd+0x7f7e9a)
#<!-- -->40 0x0000000000a386e9 (/nix/store/xkb7pp8hidrq6yam5i23csp4y1azk29d-clang-tools-19.1.7/bin/clangd+0xa386e9)
#<!-- -->41 0x00007fffe98972e3 start_thread (/nix/store/6q2mknq81cyscjmkv72fpcsvan56qhmg-glibc-2.40-66/lib/libc.so.6+0x972e3)
#<!-- -->42 0x00007fffe991b2fc __GI___clone3 (/nix/store/6q2mknq81cyscjmkv72fpcsvan56qhmg-glibc-2.40-66/lib/libc.so.6+0x11b2fc)
Signalled during AST worker action: Build AST
  Filename: main.c
  Directory: /path/to
  Command Line: /usr/bin/clang -resource-dir=/nix/store/hwp5wkjs8kf8nvvkq3byv3zid7nclrvp-clang-19.1.7/lib/clang/19 -- /path/to/main.c
  Version: 0
```
</details>


---

### Comment 2 - HighCommander4

I can reproduce this with clangd 19.

The good news is, it looks to be fixed in clangd 20.

---

### Comment 3 - HighCommander4

Note also that this issue is not specific to clangd: compiling this file (`clang -c main.c`) crashes as well (with clang 19, but not clang 20).

---

### Comment 4 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Henry Chu (henryhchchc)

<details>
```c
void foo() {
    printf("\o{}");
}
```

The code snippet above causes a crash in `clangd` when generating inlay hints. The crash occurs only when the malformed `printf` is inside a function.

The crash log is as follows

```log

I[18:28:11.499] clangd version 19.1.7
I[18:28:11.499] Features: linux
I[18:28:11.499] PID: 1610337
I[18:28:11.499] Working directory: /path/to/workspace
I[18:28:11.499] argv[0]: /nix/store/xkb7pp8hidrq6yam5i23csp4y1azk29d-clang-tools-19.1.7/bin/clangd
I[18:28:11.499] Starting LSP over stdin/stdout
I[18:28:11.499] &lt;-- initialize(1)

I[18:28:11.507] --&gt; reply:initialize(1) 7 ms

I[18:28:11.670] &lt;-- initialized
I[18:28:11.670] &lt;-- textDocument/didOpen

I[18:28:11.671] Failed to find compilation database for /path/to/main.c
I[18:28:11.671] ASTWorker building file /path/to/main.c version 0 with command clangd fallback
[/path/to/workspace]
/usr/bin/clang -resource-dir=/nix/store/hwp5wkjs8kf8nvvkq3byv3zid7nclrvp-clang-19.1.7/lib/clang/19 -- /path/to/main.c
I[18:28:11.671] &lt;-- textDocument/inlayHint(2)

I[18:28:11.672] &lt;-- textDocument/inlayHint(3)

I[18:28:11.678] Built preamble of size 232664 for file /path/to/main.c version 0 in 0.01 seconds
I[18:28:11.678] --&gt; workspace/semanticTokens/refresh(0)
I[18:28:11.679] Indexing c17 standard library in the context of /path/to/main.c

PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.	/path/to/main.c:2:16: current parser token ')'
1.	/path/to/main.c:1:12: parsing function body 'foo'
2.	/path/to/main.c:1:12: in compound statement ('{}')

I[18:28:11.694] &lt;-- textDocument/inlayHint(4)

I[18:28:11.695] &lt;-- textDocument/semanticTokens/full(5)

I[18:28:11.695] &lt;-- $/cancelRequest
I[18:28:11.695] &lt;-- textDocument/semanticTokens/full(6)

I[18:28:11.695] &lt;-- reply(0)

I[18:28:11.757] Indexed c17 standard library: 3493 symbols, 42 filtered

 #<!-- -->0 0x00007fffeafee11c llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/nix/store/sj58s6n3rhvp21b4zmh7igbwzgvh8kwz-llvm-19.1.7-lib/lib/libLLVM.so.19.1+0xfee11c)
 #<!-- -->1 0x00007fffeafeb55b SignalHandler(int) (/nix/store/sj58s6n3rhvp21b4zmh7igbwzgvh8kwz-llvm-19.1.7-lib/lib/libLLVM.so.19.1+0xfeb55b)
 #<!-- -->2 0x00007fffe9840f30 __restore_rt (/nix/store/6q2mknq81cyscjmkv72fpcsvan56qhmg-glibc-2.40-66/lib/libc.so.6+0x40f30)
 #<!-- -->3 0x00007ffff422cfb7 Diag(clang::DiagnosticsEngine*, clang::LangOptions const&amp;, clang::FullSourceLoc, char const*, char const*, char const*, unsigned int) (.isra.0) (/nix/store/j6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0xc2cfb7)
 #<!-- -->4 0x00007ffff422fe55 ProcessCharEscape(char const*, char const*&amp;, char const*, bool&amp;, clang::FullSourceLoc, unsigned int, clang::DiagnosticsEngine*, clang::LangOptions const&amp;, clang::StringLiteralEvalMethod) (.isra.0) (/nix/store/j6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0xc2fe55)
 #<!-- -->5 0x00007ffff423bcb0 clang::StringLiteralParser::init(llvm::ArrayRef&lt;clang::Token&gt;) (/nix/store/j6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0xc3bcb0)
 #<!-- -->6 0x00007ffff466c1cf clang::StringLiteral::getLocationOfByte(unsigned int, clang::SourceManager const&amp;, clang::LangOptions const&amp;, clang::TargetInfo const&amp;, unsigned int*, unsigned int*) const (/nix/store/j6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0x106c1cf)
 #<!-- -->7 0x00007ffff4e6bc35 (anonymous namespace)::CheckFormatHandler::HandleNullChar(char const*) (/nix/store/j6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0x186bc35)
 #<!-- -->8 0x00007ffff49cf329 clang::analyze_format_string::ParsePrintfString(clang::analyze_format_string::FormatStringHandler&amp;, char const*, char const*, clang::LangOptions const&amp;, clang::TargetInfo const&amp;, bool) (/nix/store/j6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0x13cf329)
 #<!-- -->9 0x00007ffff4e6d366 checkFormatStringExpr(clang::Sema&amp;, clang::Expr const*, llvm::ArrayRef&lt;clang::Expr const*&gt;, clang::Sema::FormatArgumentPassingKind, unsigned int, unsigned int, clang::Sema::FormatStringType, clang::Sema::VariadicCallType, bool, llvm::SmallBitVector&amp;, (anonymous namespace)::UncoveredArgHandler&amp;, llvm::APSInt, bool) (/nix/store/j6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0x186d366)
#<!-- -->10 0x00007ffff4e6d9c0 clang::Sema::CheckFormatArguments(llvm::ArrayRef&lt;clang::Expr const*&gt;, clang::Sema::FormatArgumentPassingKind, unsigned int, unsigned int, clang::Sema::FormatStringType, clang::Sema::VariadicCallType, clang::SourceLocation, clang::SourceRange, llvm::SmallBitVector&amp;) (/nix/store/j6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0x186d9c0)
#<!-- -->11 0x00007ffff4e6dea2 clang::Sema::CheckFormatArguments(clang::FormatAttr const*, llvm::ArrayRef&lt;clang::Expr const*&gt;, bool, clang::Sema::VariadicCallType, clang::SourceLocation, clang::SourceRange, llvm::SmallBitVector&amp;) (/nix/store/j6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0x186dea2)
#<!-- -->12 0x00007ffff4e6e0d7 clang::Sema::checkCall(clang::NamedDecl*, clang::FunctionProtoType const*, clang::Expr const*, llvm::ArrayRef&lt;clang::Expr const*&gt;, bool, clang::SourceLocation, clang::SourceRange, clang::Sema::VariadicCallType) (/nix/store/j6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0x186e0d7)
#<!-- -->13 0x00007ffff4e7b559 clang::Sema::CheckFunctionCall(clang::FunctionDecl*, clang::CallExpr*, clang::FunctionProtoType const*) (/nix/store/j6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0x187b559)
#<!-- -->14 0x00007ffff5191104 clang::Sema::BuildResolvedCallExpr(clang::Expr*, clang::NamedDecl*, clang::SourceLocation, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, clang::CallExpr::ADLCallKind) (/nix/store/j6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0x1b91104)
#<!-- -->15 0x00007ffff51948b1 clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*) (/nix/store/j6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0x1b948b1)
#<!-- -->16 0x00007ffff433116b clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult&lt;clang::Expr*, true&gt;) (/nix/store/j6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0xd3116b)
#<!-- -->17 0x00007ffff432a227 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) (/nix/store/j6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0xd2a227)
#<!-- -->18 0x00007ffff432c6e7 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) (/nix/store/j6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0xd2c6e7)
#<!-- -->19 0x00007ffff432c8b8 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/nix/store/j6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0xd2c8b8)
#<!-- -->20 0x00007ffff4330e49 clang::Parser::ParseExpression(clang::Parser::TypeCastState) (/nix/store/j6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0xd30e49)
#<!-- -->21 0x00007ffff43abd2b clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/nix/store/j6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0xdabd2b)
#<!-- -->22 0x00007ffff43ad551 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/nix/store/j6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0xdad551)
#<!-- -->23 0x00007ffff43ae07a clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/nix/store/j6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0xdae07a)
#<!-- -->24 0x00007ffff43af369 clang::Parser::ParseCompoundStatementBody(bool) (/nix/store/j6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0xdaf369)
#<!-- -->25 0x00007ffff43afdea clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/nix/store/j6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0xdafdea)
#<!-- -->26 0x00007ffff43e76a2 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/nix/store/j6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0xde76a2)
#<!-- -->27 0x00007ffff42f72d4 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/nix/store/j6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0xcf72d4)
#<!-- -->28 0x00007ffff43dfd51 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/nix/store/j6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0xddfd51)
#<!-- -->29 0x00007ffff43e0b4d clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/nix/store/j6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0xde0b4d)
#<!-- -->30 0x00007ffff43e9ced clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/nix/store/j6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0xde9ced)
#<!-- -->31 0x00007ffff43eae05 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/nix/store/j6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0xdeae05)
#<!-- -->32 0x00007ffff43eb32d clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/nix/store/j
6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0xdeb32d)
#<!-- -->33 0x00007ffff42c26e0 clang::ParseAST(clang::Sema&amp;, bool, bool) (/nix/store/j6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0xcc26e0)
#<!-- -->34 0x00007ffff67dda69 clang::FrontendAction::Execute() (/nix/store/j6hkmkijipl535dpimvjyh8bv5qjin4h-clang-19.1.7-lib/lib/libclang-cpp.so.19.1+0x31dda69)
#<!-- -->35 0x000000000076ffdf (/nix/store/xkb7pp8hidrq6yam5i23csp4y1azk29d-clang-tools-19.1.7/bin/clangd+0x76ffdf)
#<!-- -->36 0x00000000007fdbb9 (/nix/store/xkb7pp8hidrq6yam5i23csp4y1azk29d-clang-tools-19.1.7/bin/clangd+0x7fdbb9)
#<!-- -->37 0x00000000007fe4cf (/nix/store/xkb7pp8hidrq6yam5i23csp4y1azk29d-clang-tools-19.1.7/bin/clangd+0x7fe4cf)
#<!-- -->38 0x00000000007f7b33 (/nix/store/xkb7pp8hidrq6yam5i23csp4y1azk29d-clang-tools-19.1.7/bin/clangd+0x7f7b33)
#<!-- -->39 0x00000000007f7e9a (/nix/store/xkb7pp8hidrq6yam5i23csp4y1azk29d-clang-tools-19.1.7/bin/clangd+0x7f7e9a)
#<!-- -->40 0x0000000000a386e9 (/nix/store/xkb7pp8hidrq6yam5i23csp4y1azk29d-clang-tools-19.1.7/bin/clangd+0xa386e9)
#<!-- -->41 0x00007fffe98972e3 start_thread (/nix/store/6q2mknq81cyscjmkv72fpcsvan56qhmg-glibc-2.40-66/lib/libc.so.6+0x972e3)
#<!-- -->42 0x00007fffe991b2fc __GI___clone3 (/nix/store/6q2mknq81cyscjmkv72fpcsvan56qhmg-glibc-2.40-66/lib/libc.so.6+0x11b2fc)
Signalled during AST worker action: Build AST
  Filename: main.c
  Directory: /path/to
  Command Line: /usr/bin/clang -resource-dir=/nix/store/hwp5wkjs8kf8nvvkq3byv3zid7nclrvp-clang-19.1.7/lib/clang/19 -- /path/to/main.c
  Version: 0
```
</details>


---

### Comment 5 - henryhchchc

Yes I tried clang/clangd 20.1.1 and it no longer crashes.

---

### Comment 6 - HighCommander4

This was fixed by https://github.com/llvm/llvm-project/commit/8f0c865d1024a9f.

---

### Comment 7 - HighCommander4

Duplicate of https://github.com/llvm/llvm-project/issues/102218

---

