# [clang] memory leak with late parsed attributes and -skip-function-bodies

**Author:** mizvekov
**URL:** https://github.com/llvm/llvm-project/issues/153551
**Status:** Closed
**Labels:** clang:frontend, code-quality
**Closed Date:** 2025-08-14T09:42:57Z

## Body

This can be observed using a clang built with Address sanitizer (LLVM_USE_SANITIZER=Address).

For this example:
```C++
void f() __attribute__((__diagnose_if__(false, "hello", "warning"))) {}
```

Running `clang -cc1 -skip-function-bodies test.cc` gives:
```
==194977==ERROR: LeakSanitizer: detected memory leaks

Direct leak of 184 byte(s) in 1 object(s) allocated from:
    #0 0xaaaad5046c60 in operator new(unsigned long) (build/dbg/bin/clang-20+0xd3a6c60) (BuildId: 6fd25c8c9a809e25ffe2ddb309ec5769179c642e)
    #1 0xaaaadf13e234 in clang::Parser::ParseSingleGNUAttribute(clang::ParsedAttributes&, clang::SourceLocation&, clang::Parser::LateParsedAttrList*, clang::Declarator*) clang/lib/Parse/ParseDecl.cpp:206:7
    #2 0xaaaadf13f9f0 in clang::Parser::ParseGNUAttributes(clang::ParsedAttributes&, clang::Parser::LateParsedAttrList*, clang::Declarator*) clang/lib/Parse/ParseDecl.cpp:307:11
    #3 0xaaaadf078368 in clang::Parser::MaybeParseGNUAttributes(clang::Declarator&, clang::Parser::LateParsedAttrList*) clang/include/clang/Parse/Parser.h:2937:7
    #4 0xaaaadf1595ec in clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) clang/lib/Parse/ParseDecl.cpp:2365:5
    #5 0xaaaadf068528 in clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) clang/lib/Parse/Parser.cpp:1244:10
    #6 0xaaaadf0674a0 in clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) clang/lib/Parse/Parser.cpp:1266:12
    #7 0xaaaadf064dcc in clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) clang/lib/Parse/Parser.cpp:1069:14
    #8 0xaaaadf0610a8 in clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) clang/lib/Parse/Parser.cpp:758:12
    #9 0xaaaadf060080 in clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) clang/lib/Parse/Parser.cpp:608:26
    #10 0xaaaadf054374 in clang::ParseAST(clang::Sema&, bool, bool) clang/lib/Parse/ParseAST.cpp:170:25
    #11 0xaaaadbe9dc98 in clang::FrontendAction::Execute() clang/lib/Frontend/FrontendAction.cpp:1077:8
    #12 0xaaaadbd774b0 in clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) clang/lib/Frontend/CompilerInstance.cpp:1062:33
    #13 0xaaaadc0c5e68 in clang::ExecuteCompilerInvocation(clang::CompilerInstance*) clang/lib/FrontendTool/ExecuteCompilerInvocation.cpp:296:25
    #14 0xaaaad50531d0 in cc1_main(llvm::ArrayRef<char const*>, char const*, void*) clang/tools/driver/cc1_main.cpp:286:15
    #15 0xaaaad504bb74 in ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) clang/tools/driver/driver.cpp:218:12
    #16 0xaaaad5049fb0 in clang_main(int, char**, llvm::ToolContext const&) clang/tools/driver/driver.cpp:259:12
    #17 0xaaaad506ad30 in main build/dbg/tools/clang/tools/driver/clang-driver.cpp:17:10
    #18 0xffff9b812298  (/lib/aarch64-linux-gnu/libc.so.6+0x22298) (BuildId: b32c35038e644c70c27979cb0b90182f43f6794b)
    #19 0xffff9b812378 in __libc_start_main (/lib/aarch64-linux-gnu/libc.so.6+0x22378) (BuildId: b32c35038e644c70c27979cb0b90182f43f6794b)
    #20 0xaaaad4f6012c in _start (build/dbg/bin/clang-20+0xd2c012c) (BuildId: 6fd25c8c9a809e25ffe2ddb309ec5769179c642e)

Indirect leak of 216 byte(s) in 1 object(s) allocated from:
    #0 0xaaaad5004220 in malloc (build/dbg/bin/clang-20+0xd364220) (BuildId: 6fd25c8c9a809e25ffe2ddb309ec5769179c642e)
    #1 0xaaaada4b27c0 in llvm::safe_malloc(unsigned long) llvm/include/llvm/Support/MemAlloc.h:26:18
    #2 0xaaaada4b27c0 in llvm::SmallVectorBase<unsigned int>::grow_pod(void*, unsigned long, unsigned long) llvm/lib/Support/SmallVector.cpp:151:15
    #3 0xaaaadf25794c in llvm::SmallVectorTemplateCommon<clang::Token, void>::grow_pod(unsigned long, unsigned long) llvm/include/llvm/ADT/SmallVector.h:138:11
    #4 0xaaaadf25794c in llvm::SmallVectorTemplateBase<clang::Token, true>::grow(unsigned long) llvm/include/llvm/ADT/SmallVector.h:525:41
    #5 0xaaaadf25794c in clang::Token const* llvm::SmallVectorTemplateCommon<clang::Token, void>::reserveForParamAndGetAddressImpl<llvm::SmallVectorTemplateBase<clang::Token, true>>(llvm::SmallVectorTemplateBase<clang::Token, true>*, clang::Token const&, unsigned long) llvm/include/llvm/ADT/SmallVector.h:243:11
    #6 0xaaaadf25794c in llvm::SmallVectorTemplateBase<clang::Token, true>::reserveForParamAndGetAddress(clang::Token const&, unsigned long) llvm/include/llvm/ADT/SmallVector.h:530:12
    #7 0xaaaadf25794c in llvm::SmallVectorTemplateBase<clang::Token, true>::push_back(clang::Token const&) llvm/include/llvm/ADT/SmallVector.h:562:23
    #8 0xaaaadf25794c in clang::Parser::ConsumeAndStoreUntil(clang::tok::TokenKind, clang::tok::TokenKind, llvm::SmallVector<clang::Token, 4u>&, bool, bool) clang/lib/Parse/ParseCXXInlineMethods.cpp:958:12
    #9 0xaaaadf13e66c in clang::Parser::ConsumeAndStoreUntil(clang::tok::TokenKind, llvm::SmallVector<clang::Token, 4u>&, bool, bool) clang/include/clang/Parse/Parser.h:1678:12
    #10 0xaaaadf13e66c in clang::Parser::ParseSingleGNUAttribute(clang::ParsedAttributes&, clang::SourceLocation&, clang::Parser::LateParsedAttrList*, clang::Declarator*) clang/lib/Parse/ParseDecl.cpp:219:3
    #11 0xaaaadf13f9f0 in clang::Parser::ParseGNUAttributes(clang::ParsedAttributes&, clang::Parser::LateParsedAttrList*, clang::Declarator*) clang/lib/Parse/ParseDecl.cpp:307:11
    #12 0xaaaadf078368 in clang::Parser::MaybeParseGNUAttributes(clang::Declarator&, clang::Parser::LateParsedAttrList*) clang/include/clang/Parse/Parser.h:2937:7
    #13 0xaaaadf1595ec in clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) clang/lib/Parse/ParseDecl.cpp:2365:5
    #14 0xaaaadf068528 in clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) clang/lib/Parse/Parser.cpp:1244:10
    #15 0xaaaadf0674a0 in clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) clang/lib/Parse/Parser.cpp:1266:12
    #16 0xaaaadf064dcc in clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) clang/lib/Parse/Parser.cpp:1069:14
    #17 0xaaaadf0610a8 in clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) clang/lib/Parse/Parser.cpp:758:12
    #18 0xaaaadf060080 in clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) clang/lib/Parse/Parser.cpp:608:26
    #19 0xaaaadf054374 in clang::ParseAST(clang::Sema&, bool, bool) clang/lib/Parse/ParseAST.cpp:170:25
    #20 0xaaaadbe9dc98 in clang::FrontendAction::Execute() clang/lib/Frontend/FrontendAction.cpp:1077:8
    #21 0xaaaadbd774b0 in clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) clang/lib/Frontend/CompilerInstance.cpp:1062:33
    #22 0xaaaadc0c5e68 in clang::ExecuteCompilerInvocation(clang::CompilerInstance*) clang/lib/FrontendTool/ExecuteCompilerInvocation.cpp:296:25
    #23 0xaaaad50531d0 in cc1_main(llvm::ArrayRef<char const*>, char const*, void*) clang/tools/driver/cc1_main.cpp:286:15
    #24 0xaaaad504bb74 in ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) clang/tools/driver/driver.cpp:218:12
    #25 0xaaaad5049fb0 in clang_main(int, char**, llvm::ToolContext const&) clang/tools/driver/driver.cpp:259:12
    #26 0xaaaad506ad30 in main build/dbg/tools/clang/tools/driver/clang-driver.cpp:17:10
    #27 0xffff9b812298  (/lib/aarch64-linux-gnu/libc.so.6+0x22298) (BuildId: b32c35038e644c70c27979cb0b90182f43f6794b)
    #28 0xffff9b812378 in __libc_start_main (/lib/aarch64-linux-gnu/libc.so.6+0x22378) (BuildId: b32c35038e644c70c27979cb0b90182f43f6794b)
    #29 0xaaaad4f6012c in _start (build/dbg/bin/clang-20+0xd2c012c) (BuildId: 6fd25c8c9a809e25ffe2ddb309ec5769179c642e)

SUMMARY: AddressSanitizer: 400 byte(s) leaked in 2 allocation(s).
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Matheus Izvekov (mizvekov)

<details>
This can be observed using a clang built with Address sanitizer (LLVM_USE_SANITIZER=Address).

For this example:
```C++
void f() __attribute__((__diagnose_if__(false, "hello", "warning"))) {}
```

Running `clang -cc1 -skip-function-bodies test.cc` gives:
```
==194977==ERROR: LeakSanitizer: detected memory leaks

Direct leak of 184 byte(s) in 1 object(s) allocated from:
    #<!-- -->0 0xaaaad5046c60 in operator new(unsigned long) (build/dbg/bin/clang-20+0xd3a6c60) (BuildId: 6fd25c8c9a809e25ffe2ddb309ec5769179c642e)
    #<!-- -->1 0xaaaadf13e234 in clang::Parser::ParseSingleGNUAttribute(clang::ParsedAttributes&amp;, clang::SourceLocation&amp;, clang::Parser::LateParsedAttrList*, clang::Declarator*) clang/lib/Parse/ParseDecl.cpp:206:7
    #<!-- -->2 0xaaaadf13f9f0 in clang::Parser::ParseGNUAttributes(clang::ParsedAttributes&amp;, clang::Parser::LateParsedAttrList*, clang::Declarator*) clang/lib/Parse/ParseDecl.cpp:307:11
    #<!-- -->3 0xaaaadf078368 in clang::Parser::MaybeParseGNUAttributes(clang::Declarator&amp;, clang::Parser::LateParsedAttrList*) clang/include/clang/Parse/Parser.h:2937:7
    #<!-- -->4 0xaaaadf1595ec in clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) clang/lib/Parse/ParseDecl.cpp:2365:5
    #<!-- -->5 0xaaaadf068528 in clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) clang/lib/Parse/Parser.cpp:1244:10
    #<!-- -->6 0xaaaadf0674a0 in clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) clang/lib/Parse/Parser.cpp:1266:12
    #<!-- -->7 0xaaaadf064dcc in clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) clang/lib/Parse/Parser.cpp:1069:14
    #<!-- -->8 0xaaaadf0610a8 in clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) clang/lib/Parse/Parser.cpp:758:12
    #<!-- -->9 0xaaaadf060080 in clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) clang/lib/Parse/Parser.cpp:608:26
    #<!-- -->10 0xaaaadf054374 in clang::ParseAST(clang::Sema&amp;, bool, bool) clang/lib/Parse/ParseAST.cpp:170:25
    #<!-- -->11 0xaaaadbe9dc98 in clang::FrontendAction::Execute() clang/lib/Frontend/FrontendAction.cpp:1077:8
    #<!-- -->12 0xaaaadbd774b0 in clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) clang/lib/Frontend/CompilerInstance.cpp:1062:33
    #<!-- -->13 0xaaaadc0c5e68 in clang::ExecuteCompilerInvocation(clang::CompilerInstance*) clang/lib/FrontendTool/ExecuteCompilerInvocation.cpp:296:25
    #<!-- -->14 0xaaaad50531d0 in cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) clang/tools/driver/cc1_main.cpp:286:15
    #<!-- -->15 0xaaaad504bb74 in ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) clang/tools/driver/driver.cpp:218:12
    #<!-- -->16 0xaaaad5049fb0 in clang_main(int, char**, llvm::ToolContext const&amp;) clang/tools/driver/driver.cpp:259:12
    #<!-- -->17 0xaaaad506ad30 in main build/dbg/tools/clang/tools/driver/clang-driver.cpp:17:10
    #<!-- -->18 0xffff9b812298  (/lib/aarch64-linux-gnu/libc.so.6+0x22298) (BuildId: b32c35038e644c70c27979cb0b90182f43f6794b)
    #<!-- -->19 0xffff9b812378 in __libc_start_main (/lib/aarch64-linux-gnu/libc.so.6+0x22378) (BuildId: b32c35038e644c70c27979cb0b90182f43f6794b)
    #<!-- -->20 0xaaaad4f6012c in _start (build/dbg/bin/clang-20+0xd2c012c) (BuildId: 6fd25c8c9a809e25ffe2ddb309ec5769179c642e)

Indirect leak of 216 byte(s) in 1 object(s) allocated from:
    #<!-- -->0 0xaaaad5004220 in malloc (build/dbg/bin/clang-20+0xd364220) (BuildId: 6fd25c8c9a809e25ffe2ddb309ec5769179c642e)
    #<!-- -->1 0xaaaada4b27c0 in llvm::safe_malloc(unsigned long) llvm/include/llvm/Support/MemAlloc.h:26:18
    #<!-- -->2 0xaaaada4b27c0 in llvm::SmallVectorBase&lt;unsigned int&gt;::grow_pod(void*, unsigned long, unsigned long) llvm/lib/Support/SmallVector.cpp:151:15
    #<!-- -->3 0xaaaadf25794c in llvm::SmallVectorTemplateCommon&lt;clang::Token, void&gt;::grow_pod(unsigned long, unsigned long) llvm/include/llvm/ADT/SmallVector.h:138:11
    #<!-- -->4 0xaaaadf25794c in llvm::SmallVectorTemplateBase&lt;clang::Token, true&gt;::grow(unsigned long) llvm/include/llvm/ADT/SmallVector.h:525:41
    #<!-- -->5 0xaaaadf25794c in clang::Token const* llvm::SmallVectorTemplateCommon&lt;clang::Token, void&gt;::reserveForParamAndGetAddressImpl&lt;llvm::SmallVectorTemplateBase&lt;clang::Token, true&gt;&gt;(llvm::SmallVectorTemplateBase&lt;clang::Token, true&gt;*, clang::Token const&amp;, unsigned long) llvm/include/llvm/ADT/SmallVector.h:243:11
    #<!-- -->6 0xaaaadf25794c in llvm::SmallVectorTemplateBase&lt;clang::Token, true&gt;::reserveForParamAndGetAddress(clang::Token const&amp;, unsigned long) llvm/include/llvm/ADT/SmallVector.h:530:12
    #<!-- -->7 0xaaaadf25794c in llvm::SmallVectorTemplateBase&lt;clang::Token, true&gt;::push_back(clang::Token const&amp;) llvm/include/llvm/ADT/SmallVector.h:562:23
    #<!-- -->8 0xaaaadf25794c in clang::Parser::ConsumeAndStoreUntil(clang::tok::TokenKind, clang::tok::TokenKind, llvm::SmallVector&lt;clang::Token, 4u&gt;&amp;, bool, bool) clang/lib/Parse/ParseCXXInlineMethods.cpp:958:12
    #<!-- -->9 0xaaaadf13e66c in clang::Parser::ConsumeAndStoreUntil(clang::tok::TokenKind, llvm::SmallVector&lt;clang::Token, 4u&gt;&amp;, bool, bool) clang/include/clang/Parse/Parser.h:1678:12
    #<!-- -->10 0xaaaadf13e66c in clang::Parser::ParseSingleGNUAttribute(clang::ParsedAttributes&amp;, clang::SourceLocation&amp;, clang::Parser::LateParsedAttrList*, clang::Declarator*) clang/lib/Parse/ParseDecl.cpp:219:3
    #<!-- -->11 0xaaaadf13f9f0 in clang::Parser::ParseGNUAttributes(clang::ParsedAttributes&amp;, clang::Parser::LateParsedAttrList*, clang::Declarator*) clang/lib/Parse/ParseDecl.cpp:307:11
    #<!-- -->12 0xaaaadf078368 in clang::Parser::MaybeParseGNUAttributes(clang::Declarator&amp;, clang::Parser::LateParsedAttrList*) clang/include/clang/Parse/Parser.h:2937:7
    #<!-- -->13 0xaaaadf1595ec in clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) clang/lib/Parse/ParseDecl.cpp:2365:5
    #<!-- -->14 0xaaaadf068528 in clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) clang/lib/Parse/Parser.cpp:1244:10
    #<!-- -->15 0xaaaadf0674a0 in clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) clang/lib/Parse/Parser.cpp:1266:12
    #<!-- -->16 0xaaaadf064dcc in clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) clang/lib/Parse/Parser.cpp:1069:14
    #<!-- -->17 0xaaaadf0610a8 in clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) clang/lib/Parse/Parser.cpp:758:12
    #<!-- -->18 0xaaaadf060080 in clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) clang/lib/Parse/Parser.cpp:608:26
    #<!-- -->19 0xaaaadf054374 in clang::ParseAST(clang::Sema&amp;, bool, bool) clang/lib/Parse/ParseAST.cpp:170:25
    #<!-- -->20 0xaaaadbe9dc98 in clang::FrontendAction::Execute() clang/lib/Frontend/FrontendAction.cpp:1077:8
    #<!-- -->21 0xaaaadbd774b0 in clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) clang/lib/Frontend/CompilerInstance.cpp:1062:33
    #<!-- -->22 0xaaaadc0c5e68 in clang::ExecuteCompilerInvocation(clang::CompilerInstance*) clang/lib/FrontendTool/ExecuteCompilerInvocation.cpp:296:25
    #<!-- -->23 0xaaaad50531d0 in cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) clang/tools/driver/cc1_main.cpp:286:15
    #<!-- -->24 0xaaaad504bb74 in ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) clang/tools/driver/driver.cpp:218:12
    #<!-- -->25 0xaaaad5049fb0 in clang_main(int, char**, llvm::ToolContext const&amp;) clang/tools/driver/driver.cpp:259:12
    #<!-- -->26 0xaaaad506ad30 in main build/dbg/tools/clang/tools/driver/clang-driver.cpp:17:10
    #<!-- -->27 0xffff9b812298  (/lib/aarch64-linux-gnu/libc.so.6+0x22298) (BuildId: b32c35038e644c70c27979cb0b90182f43f6794b)
    #<!-- -->28 0xffff9b812378 in __libc_start_main (/lib/aarch64-linux-gnu/libc.so.6+0x22378) (BuildId: b32c35038e644c70c27979cb0b90182f43f6794b)
    #<!-- -->29 0xaaaad4f6012c in _start (build/dbg/bin/clang-20+0xd2c012c) (BuildId: 6fd25c8c9a809e25ffe2ddb309ec5769179c642e)

SUMMARY: AddressSanitizer: 400 byte(s) leaked in 2 allocation(s).
```
</details>


---

