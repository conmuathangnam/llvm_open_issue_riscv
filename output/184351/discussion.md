# clangd crashes in DeclareAggregateDeductionGuideFromInitList

**Author:** yerseg
**URL:** https://github.com/llvm/llvm-project/issues/184351
**Status:** Closed
**Labels:** duplicate, clangd, crash
**Closed Date:** 2026-03-04T06:26:04Z

## Body

Have [this](https://github.com/llvm/llvm-project/issues/112953) problem again on clangd-21 :(

```
/usr/bin/clangd --version
Ubuntu clangd version 21.1.5 (++20251023083201+45afac62e373-1~exp1~20251023083316.53)
Features: linux+grpc
Platform: x86_64-pc-linux-gnu
```

```
"clangd.arguments": [
        "--compile-commands-dir=${workspaceFolder}/build",
        "-j=64",
        "--clang-tidy",
        "--background-index",
        "--completion-style=bundled",
        "--header-insertion=never",
        "--function-arg-placeholders=0",
        "--malloc-trim",
        "--pch-storage=memory",
    ],
```

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.	/home/sergei.kazmin/.conan2/p/range0301bf3d76d5d/p/include/range/v3/algorithm/nth_element.hpp:117:44: current parser token ';'
1.	/home/sergei.kazmin/.conan2/p/range0301bf3d76d5d/p/include/range/v3/algorithm/nth_element.hpp:47:1: parsing namespace 'ranges'
2.	/home/sergei.kazmin/.conan2/p/range0301bf3d76d5d/p/include/range/v3/algorithm/nth_element.hpp:106:5 <Spelling=/home/sergei.kazmin/.conan2/p/range0301bf3d76d5d/p/include/range/v3/detail/config.hpp:630:5>: parsing struct/union/class body 'ranges::nth_element_fn'
3.	/home/sergei.kazmin/.conan2/p/range0301bf3d76d5d/p/include/range/v3/algorithm/nth_element.hpp:113:9: parsing function body 'ranges::nth_element_fn::operator()'
4.	/home/sergei.kazmin/.conan2/p/range0301bf3d76d5d/p/include/range/v3/algorithm/nth_element.hpp:113:9: in compound statement ('{}')
 #0 0x00007e6979dbe316 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/usr/lib/llvm-21/bin/../lib/libLLVM.so.21.1+0xbbe316)
 #1 0x00007e6979dbbbc3 llvm::sys::RunSignalHandlers() (/usr/lib/llvm-21/bin/../lib/libLLVM.so.21.1+0xbbbbc3)
 #2 0x00007e6979dbf044 (/usr/lib/llvm-21/bin/../lib/libLLVM.so.21.1+0xbbf044)
 #3 0x00007e6978242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007e69833b4891 clang::Sema::DeclareAggregateDeductionGuideFromInitList(clang::TemplateDecl*, llvm::MutableArrayRef<clang::QualType>, clang::SourceLocation) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x1bb4891)
 #5 0x00007e698310bbbc (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x190bbbc)
 #6 0x00007e69830fda8b (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x18fda8b)
 #7 0x00007e69830fce54 clang::Sema::DeduceTemplateSpecializationFromInitializer(clang::TypeSourceInfo*, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::MutableArrayRef<clang::Expr*>) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x18fce54)
 #8 0x00007e6982ef719d clang::Sema::deduceVarTypeFromInitializer(clang::VarDecl*, clang::DeclarationName, clang::QualType, clang::TypeSourceInfo*, clang::SourceRange, bool, clang::Expr*) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x16f719d)
 #9 0x00007e6982ef788c clang::Sema::DeduceVariableDeclarationType(clang::VarDecl*, bool, clang::Expr*) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x16f788c)
#10 0x00007e6982ef83c2 clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x16f83c2)
#11 0x00007e6982397aed clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0xb97aed)
#12 0x00007e6982394dcb clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0xb94dcb)
#13 0x00007e69823942f0 clang::Parser::ParseSimpleDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, bool, clang::Parser::ForRangeInit*, clang::SourceLocation*) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0xb942f0)
#14 0x00007e6982393d22 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0xb93d22)
#15 0x00007e69824244bb clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0xc244bb)
#16 0x00007e6982423b9c clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0xc23b9c)
#17 0x00007e698242c181 clang::Parser::ParseCompoundStatementBody(bool) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0xc2c181)
#18 0x00007e698242cc65 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0xc2cc65)
#19 0x00007e6982386031 clang::Parser::ParseLexedMethodDef(clang::Parser::LexedMethod&) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0xb86031)
#20 0x00007e6982384dfa clang::Parser::ParseLexedMethodDefs(clang::Parser::ParsingClass&) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0xb84dfa)
#21 0x00007e69823b86a2 clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&, unsigned int, clang::Decl*) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0xbb86a2)
#22 0x00007e69823b6150 clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0xbb6150)
#23 0x00007e69823992bb clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0xb992bb)
#24 0x00007e6982443a01 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0xc43a01)
#25 0x00007e6982443755 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0xc43755)
#26 0x00007e6982442a71 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0xc42a71)
#27 0x00007e69823af1e9 clang::Parser::ParseInnerNamespace(llvm::SmallVector<clang::Parser::InnerNamespaceInfo, 4u> const&, unsigned int, clang::SourceLocation&, clang::ParsedAttributes&, clang::BalancedDelimiterTracker&) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0xbaf1e9)
#28 0x00007e69823aebd9 clang::Parser::ParseNamespace(clang::DeclaratorContext, clang::SourceLocation&, clang::SourceLocation) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0xbaebd9)
#29 0x00007e6982393e0c clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0xb93e0c)
#30 0x00007e69824422ff clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0xc422ff)
#31 0x00007e69824412b7 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0xc412b7)
#32 0x00007e6982380c4e clang::ParseAST(clang::Sema&, bool, bool) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0xb80c4e)
#33 0x00007e6983f3b6ab clang::FrontendAction::Execute() (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x273b6ab)
#34 0x00006099c8d350a8 (/usr/bin/clangd+0x59e0a8)
#35 0x00006099c8d380c5 (/usr/bin/clangd+0x5a10c5)
#36 0x00006099c8d3bf7b (/usr/bin/clangd+0x5a4f7b)
#37 0x00006099c8d3687e (/usr/bin/clangd+0x59f87e)
#38 0x00006099c8da2e3a (/usr/bin/clangd+0x60be3a)
#39 0x00007e6978294ac3 start_thread ./nptl/pthread_create.c:442:8
#40 0x00007e69783268c0 ./misc/../sysdeps/unix/sysv/linux/x86_64/clone3.S:83:0
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clangd

Author: Sergey Kazmin  (yerseg)

<details>
Have [this](https://github.com/llvm/llvm-project/issues/112953) problem again on clangd-21 :(

```
/usr/bin/clangd --version
Ubuntu clangd version 21.1.5 (++20251023083201+45afac62e373-1~exp1~20251023083316.53)
Features: linux+grpc
Platform: x86_64-pc-linux-gnu
```

```
"clangd.arguments": [
        "--compile-commands-dir=${workspaceFolder}/build",
        "-j=64",
        "--clang-tidy",
        "--background-index",
        "--completion-style=bundled",
        "--header-insertion=never",
        "--function-arg-placeholders=0",
        "--malloc-trim",
        "--pch-storage=memory",
    ],
```

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.	/home/sergei.kazmin/.conan2/p/range0301bf3d76d5d/p/include/range/v3/algorithm/nth_element.hpp:117:44: current parser token ';'
1.	/home/sergei.kazmin/.conan2/p/range0301bf3d76d5d/p/include/range/v3/algorithm/nth_element.hpp:47:1: parsing namespace 'ranges'
2.	/home/sergei.kazmin/.conan2/p/range0301bf3d76d5d/p/include/range/v3/algorithm/nth_element.hpp:106:5 &lt;Spelling=/home/sergei.kazmin/.conan2/p/range0301bf3d76d5d/p/include/range/v3/detail/config.hpp:630:5&gt;: parsing struct/union/class body 'ranges::nth_element_fn'
3.	/home/sergei.kazmin/.conan2/p/range0301bf3d76d5d/p/include/range/v3/algorithm/nth_element.hpp:113:9: parsing function body 'ranges::nth_element_fn::operator()'
4.	/home/sergei.kazmin/.conan2/p/range0301bf3d76d5d/p/include/range/v3/algorithm/nth_element.hpp:113:9: in compound statement ('{}')
 #<!-- -->0 0x00007e6979dbe316 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/usr/lib/llvm-21/bin/../lib/libLLVM.so.21.1+0xbbe316)
 #<!-- -->1 0x00007e6979dbbbc3 llvm::sys::RunSignalHandlers() (/usr/lib/llvm-21/bin/../lib/libLLVM.so.21.1+0xbbbbc3)
 #<!-- -->2 0x00007e6979dbf044 (/usr/lib/llvm-21/bin/../lib/libLLVM.so.21.1+0xbbf044)
 #<!-- -->3 0x00007e6978242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007e69833b4891 clang::Sema::DeclareAggregateDeductionGuideFromInitList(clang::TemplateDecl*, llvm::MutableArrayRef&lt;clang::QualType&gt;, clang::SourceLocation) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x1bb4891)
 #<!-- -->5 0x00007e698310bbbc (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x190bbbc)
 #<!-- -->6 0x00007e69830fda8b (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x18fda8b)
 #<!-- -->7 0x00007e69830fce54 clang::Sema::DeduceTemplateSpecializationFromInitializer(clang::TypeSourceInfo*, clang::InitializedEntity const&amp;, clang::InitializationKind const&amp;, llvm::MutableArrayRef&lt;clang::Expr*&gt;) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x18fce54)
 #<!-- -->8 0x00007e6982ef719d clang::Sema::deduceVarTypeFromInitializer(clang::VarDecl*, clang::DeclarationName, clang::QualType, clang::TypeSourceInfo*, clang::SourceRange, bool, clang::Expr*) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x16f719d)
 #<!-- -->9 0x00007e6982ef788c clang::Sema::DeduceVariableDeclarationType(clang::VarDecl*, bool, clang::Expr*) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x16f788c)
#<!-- -->10 0x00007e6982ef83c2 clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x16f83c2)
#<!-- -->11 0x00007e6982397aed clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0xb97aed)
#<!-- -->12 0x00007e6982394dcb clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0xb94dcb)
#<!-- -->13 0x00007e69823942f0 clang::Parser::ParseSimpleDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, bool, clang::Parser::ForRangeInit*, clang::SourceLocation*) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0xb942f0)
#<!-- -->14 0x00007e6982393d22 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0xb93d22)
#<!-- -->15 0x00007e69824244bb clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0xc244bb)
#<!-- -->16 0x00007e6982423b9c clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0xc23b9c)
#<!-- -->17 0x00007e698242c181 clang::Parser::ParseCompoundStatementBody(bool) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0xc2c181)
#<!-- -->18 0x00007e698242cc65 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0xc2cc65)
#<!-- -->19 0x00007e6982386031 clang::Parser::ParseLexedMethodDef(clang::Parser::LexedMethod&amp;) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0xb86031)
#<!-- -->20 0x00007e6982384dfa clang::Parser::ParseLexedMethodDefs(clang::Parser::ParsingClass&amp;) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0xb84dfa)
#<!-- -->21 0x00007e69823b86a2 clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&amp;, unsigned int, clang::Decl*) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0xbb86a2)
#<!-- -->22 0x00007e69823b6150 clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&amp;) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0xbb6150)
#<!-- -->23 0x00007e69823992bb clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0xb992bb)
#<!-- -->24 0x00007e6982443a01 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0xc43a01)
#<!-- -->25 0x00007e6982443755 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0xc43755)
#<!-- -->26 0x00007e6982442a71 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0xc42a71)
#<!-- -->27 0x00007e69823af1e9 clang::Parser::ParseInnerNamespace(llvm::SmallVector&lt;clang::Parser::InnerNamespaceInfo, 4u&gt; const&amp;, unsigned int, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::BalancedDelimiterTracker&amp;) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0xbaf1e9)
#<!-- -->28 0x00007e69823aebd9 clang::Parser::ParseNamespace(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::SourceLocation) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0xbaebd9)
#<!-- -->29 0x00007e6982393e0c clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0xb93e0c)
#<!-- -->30 0x00007e69824422ff clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0xc422ff)
#<!-- -->31 0x00007e69824412b7 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0xc412b7)
#<!-- -->32 0x00007e6982380c4e clang::ParseAST(clang::Sema&amp;, bool, bool) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0xb80c4e)
#<!-- -->33 0x00007e6983f3b6ab clang::FrontendAction::Execute() (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x273b6ab)
#<!-- -->34 0x00006099c8d350a8 (/usr/bin/clangd+0x59e0a8)
#<!-- -->35 0x00006099c8d380c5 (/usr/bin/clangd+0x5a10c5)
#<!-- -->36 0x00006099c8d3bf7b (/usr/bin/clangd+0x5a4f7b)
#<!-- -->37 0x00006099c8d3687e (/usr/bin/clangd+0x59f87e)
#<!-- -->38 0x00006099c8da2e3a (/usr/bin/clangd+0x60be3a)
#<!-- -->39 0x00007e6978294ac3 start_thread ./nptl/pthread_create.c:442:8
#<!-- -->40 0x00007e69783268c0 ./misc/../sysdeps/unix/sysv/linux/x86_64/clone3.S:83:0
```
</details>


---

### Comment 2 - EugeneZelenko

Could you please try 22 or `main` branch? Only most recent release is maintained.


---

### Comment 3 - HighCommander4

Based on the stack trace and the file on which clang crashes, `range/v3/algorithm/nth_element.hpp`, looks like https://github.com/llvm/llvm-project/issues/125167.

If you're able to provide a reproducer for the crash, please post it in that issue.

---

