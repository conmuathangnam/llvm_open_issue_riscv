# [Crash] clang: error: unable to execute command: Segmentation fault (core dumped); clang: error: clang frontend command failed with exit code 139

**Author:** 8ss-boop
**URL:** https://github.com/llvm/llvm-project/issues/152826
**Status:** Closed
**Labels:** clang:frontend, crash
**Closed Date:** 2025-08-25T19:06:50Z

## Body

Compiler Explorer: https://godbolt.org/z/sEbTqb5d7

The crash triggering program (compile with `clang -O0 -std=c2x`):
```
void foo (char (*a)[*][5], int (*x)[_Countof (*a)]);
```

Crash output:
```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /root/software/llvm-releases/llvm-0804/bin/clang-22 -cc1 -triple x86_64-unknown-linux-gnu -O0 -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name 4.c -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/root/LLMFUZZ/LLVM-CRASH -fcoverage-compilation-dir=/root/LLMFUZZ/LLVM-CRASH -resource-dir /root/software/llvm-releases/llvm-0804/lib/clang/22 -internal-isystem /root/software/llvm-releases/llvm-0804/lib/clang/22/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/12/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -std=c2x -ferror-limit 19 -fmessage-length=180 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/4-725f76.o -x c 4.c
1.      4.c:1:51: current parser token ')'
 #0 0x00007f2d4d6c2d01 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/root/software/llvm-releases/llvm-0804/bin/../lib/libLLVMSupport.so.22.0git+0x1f3d01)
 #1 0x00007f2d4d6bfd27 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x00007f2d4cf6a520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x00007f2d4b83c434 CheckICE(clang::Expr const*, clang::ASTContext const&) ExprConstant.cpp:0:0
 #4 0x00007f2d4b83d233 clang::Expr::isIntegerConstantExpr(clang::ASTContext const&) const (/root/software/llvm-releases/llvm-0804/bin/../lib/../lib/libclangAST.so.22.0git+0x6c1233)
 #5 0x00007f2d49fe020f clang::Sema::VerifyIntegerConstantExpression(clang::Expr*, llvm::APSInt*, clang::Sema::VerifyICEDiagnoser&, clang::AllowFoldKind) (/root/software/llvm-releases/llvm-0804/bin/../lib/../lib/libclangSema.so.22.0git+0x71420f)
 #6 0x00007f2d4a720992 clang::Sema::BuildArrayType(clang::QualType, clang::ArraySizeModifier, clang::Expr*, unsigned int, clang::SourceRange, clang::DeclarationName) (/root/software/llvm-releases/llvm-0804/bin/../lib/../lib/libclangSema.so.22.0git+0xe54992)
 #7 0x00007f2d4a73ce9b GetFullTypeForDeclarator((anonymous namespace)::TypeProcessingState&, clang::QualType, clang::TypeSourceInfo*) SemaType.cpp:0:0
 #8 0x00007f2d4a740685 clang::Sema::GetTypeForDeclarator(clang::Declarator&) (/root/software/llvm-releases/llvm-0804/bin/../lib/../lib/libclangSema.so.22.0git+0xe74685)
 #9 0x00007f2d49e0d573 clang::Sema::ActOnParamDeclarator(clang::Scope*, clang::Declarator&, clang::SourceLocation) (/root/software/llvm-releases/llvm-0804/bin/../lib/../lib/libclangSema.so.22.0git+0x541573)
#10 0x00007f2d4a9b8e8c clang::Parser::ParseParameterDeclarationClause(clang::DeclaratorContext, clang::ParsedAttributes&, llvm::SmallVectorImpl<clang::DeclaratorChunk::ParamInfo>&, clang::SourceLocation&, bool) (/root/software/llvm-releases/llvm-0804/bin/../lib/../lib/libclangParse.so.22.0git+0x7be8c)
#11 0x00007f2d4a9c0311 clang::Parser::ParseFunctionDeclarator(clang::Declarator&, clang::ParsedAttributes&, clang::BalancedDelimiterTracker&, bool, bool) (/root/software/llvm-releases/llvm-0804/bin/../lib/../lib/libclangParse.so.22.0git+0x83311)
#12 0x00007f2d4a9c1f89 clang::Parser::ParseDirectDeclarator(clang::Declarator&) (/root/software/llvm-releases/llvm-0804/bin/../lib/../lib/libclangParse.so.22.0git+0x84f89)
#13 0x00007f2d4a9bbd15 clang::Parser::ParseDeclaratorInternal(clang::Declarator&, void (clang::Parser::*)(clang::Declarator&)) (/root/software/llvm-releases/llvm-0804/bin/../lib/../lib/libclangParse.so.22.0git+0x7ed15)
#14 0x00007f2d4ef2cf72 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/root/software/llvm-releases/llvm-0804/bin/../lib/libclangBasic.so.22.0git+0x19cf72)
#15 0x00007f2d4a9b56e6 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/root/software/llvm-releases/llvm-0804/bin/../lib/../lib/libclangParse.so.22.0git+0x786e6)
#16 0x00007f2d4aac4f86 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/root/software/llvm-releases/llvm-0804/bin/../lib/../lib/libclangParse.so.22.0git+0x187f86)
#17 0x00007f2d4aac5d2b clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/root/software/llvm-releases/llvm-0804/bin/../lib/../lib/libclangParse.so.22.0git+0x188d2b)
#18 0x00007f2d4aacf473 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/root/software/llvm-releases/llvm-0804/bin/../lib/../lib/libclangParse.so.22.0git+0x192473)
#19 0x00007f2d4aad06d5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/root/software/llvm-releases/llvm-0804/bin/../lib/../lib/libclangParse.so.22.0git+0x1936d5)
#20 0x00007f2d4aad0c41 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/root/software/llvm-releases/llvm-0804/bin/../lib/../lib/libclangParse.so.22.0git+0x193c41)
#21 0x00007f2d4a97cff6 clang::ParseAST(clang::Sema&, bool, bool) (/root/software/llvm-releases/llvm-0804/bin/../lib/../lib/libclangParse.so.22.0git+0x3fff6)
#22 0x00007f2d4f9d1236 clang::FrontendAction::Execute() (/root/software/llvm-releases/llvm-0804/bin/../lib/libclangFrontend.so.22.0git+0x124236)
#23 0x00007f2d4f947a9a clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/root/software/llvm-releases/llvm-0804/bin/../lib/libclangFrontend.so.22.0git+0x9aa9a)
#24 0x00007f2d516c77a2 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/root/software/llvm-releases/llvm-0804/bin/../lib/libclangFrontendTool.so.22.0git+0x57a2)
#25 0x0000000000414d9b cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/root/software/llvm-releases/llvm-0804/bin/clang-22+0x414d9b)
#26 0x000000000040c3c4 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#27 0x00000000004101e1 clang_main(int, char**, llvm::ToolContext const&) (/root/software/llvm-releases/llvm-0804/bin/clang-22+0x4101e1)
#28 0x000000000040bdbe main (/root/software/llvm-releases/llvm-0804/bin/clang-22+0x40bdbe)
#29 0x00007f2d4cf51d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#30 0x00007f2d4cf51e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#31 0x000000000040be05 _start (/root/software/llvm-releases/llvm-0804/bin/clang-22+0x40be05)
clang: error: unable to execute command: Segmentation fault (core dumped)
clang: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 22.0.0git (https://github.com/llvm/llvm-project.git 28ed57eda80dc5e3b7dd01d15100c126fa4e50f6)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /root/software/llvm-releases/llvm-0804/bin
clang: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang: note: diagnostic msg: /tmp/4-2695e1.c
clang: note: diagnostic msg: /tmp/4-2695e1.sh
clang: note: diagnostic msg: 
```

[4.zip](https://github.com/user-attachments/files/21695960/4.zip)

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (8ss-boop)

<details>
Compiler Explorer: https://godbolt.org/z/sEbTqb5d7

The crash triggering program (compile with `clang -O0 -std=c2x`):
```
void foo (char (*a)[*][5], int (*x)[_Countof (*a)]);
```

Crash output:
```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /root/software/llvm-releases/llvm-0804/bin/clang-22 -cc1 -triple x86_64-unknown-linux-gnu -O0 -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name 4.c -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/root/LLMFUZZ/LLVM-CRASH -fcoverage-compilation-dir=/root/LLMFUZZ/LLVM-CRASH -resource-dir /root/software/llvm-releases/llvm-0804/lib/clang/22 -internal-isystem /root/software/llvm-releases/llvm-0804/lib/clang/22/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/12/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -std=c2x -ferror-limit 19 -fmessage-length=180 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/4-725f76.o -x c 4.c
1.      4.c:1:51: current parser token ')'
 #<!-- -->0 0x00007f2d4d6c2d01 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/root/software/llvm-releases/llvm-0804/bin/../lib/libLLVMSupport.so.22.0git+0x1f3d01)
 #<!-- -->1 0x00007f2d4d6bfd27 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->2 0x00007f2d4cf6a520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->3 0x00007f2d4b83c434 CheckICE(clang::Expr const*, clang::ASTContext const&amp;) ExprConstant.cpp:0:0
 #<!-- -->4 0x00007f2d4b83d233 clang::Expr::isIntegerConstantExpr(clang::ASTContext const&amp;) const (/root/software/llvm-releases/llvm-0804/bin/../lib/../lib/libclangAST.so.22.0git+0x6c1233)
 #<!-- -->5 0x00007f2d49fe020f clang::Sema::VerifyIntegerConstantExpression(clang::Expr*, llvm::APSInt*, clang::Sema::VerifyICEDiagnoser&amp;, clang::AllowFoldKind) (/root/software/llvm-releases/llvm-0804/bin/../lib/../lib/libclangSema.so.22.0git+0x71420f)
 #<!-- -->6 0x00007f2d4a720992 clang::Sema::BuildArrayType(clang::QualType, clang::ArraySizeModifier, clang::Expr*, unsigned int, clang::SourceRange, clang::DeclarationName) (/root/software/llvm-releases/llvm-0804/bin/../lib/../lib/libclangSema.so.22.0git+0xe54992)
 #<!-- -->7 0x00007f2d4a73ce9b GetFullTypeForDeclarator((anonymous namespace)::TypeProcessingState&amp;, clang::QualType, clang::TypeSourceInfo*) SemaType.cpp:0:0
 #<!-- -->8 0x00007f2d4a740685 clang::Sema::GetTypeForDeclarator(clang::Declarator&amp;) (/root/software/llvm-releases/llvm-0804/bin/../lib/../lib/libclangSema.so.22.0git+0xe74685)
 #<!-- -->9 0x00007f2d49e0d573 clang::Sema::ActOnParamDeclarator(clang::Scope*, clang::Declarator&amp;, clang::SourceLocation) (/root/software/llvm-releases/llvm-0804/bin/../lib/../lib/libclangSema.so.22.0git+0x541573)
#<!-- -->10 0x00007f2d4a9b8e8c clang::Parser::ParseParameterDeclarationClause(clang::DeclaratorContext, clang::ParsedAttributes&amp;, llvm::SmallVectorImpl&lt;clang::DeclaratorChunk::ParamInfo&gt;&amp;, clang::SourceLocation&amp;, bool) (/root/software/llvm-releases/llvm-0804/bin/../lib/../lib/libclangParse.so.22.0git+0x7be8c)
#<!-- -->11 0x00007f2d4a9c0311 clang::Parser::ParseFunctionDeclarator(clang::Declarator&amp;, clang::ParsedAttributes&amp;, clang::BalancedDelimiterTracker&amp;, bool, bool) (/root/software/llvm-releases/llvm-0804/bin/../lib/../lib/libclangParse.so.22.0git+0x83311)
#<!-- -->12 0x00007f2d4a9c1f89 clang::Parser::ParseDirectDeclarator(clang::Declarator&amp;) (/root/software/llvm-releases/llvm-0804/bin/../lib/../lib/libclangParse.so.22.0git+0x84f89)
#<!-- -->13 0x00007f2d4a9bbd15 clang::Parser::ParseDeclaratorInternal(clang::Declarator&amp;, void (clang::Parser::*)(clang::Declarator&amp;)) (/root/software/llvm-releases/llvm-0804/bin/../lib/../lib/libclangParse.so.22.0git+0x7ed15)
#<!-- -->14 0x00007f2d4ef2cf72 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/root/software/llvm-releases/llvm-0804/bin/../lib/libclangBasic.so.22.0git+0x19cf72)
#<!-- -->15 0x00007f2d4a9b56e6 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/root/software/llvm-releases/llvm-0804/bin/../lib/../lib/libclangParse.so.22.0git+0x786e6)
#<!-- -->16 0x00007f2d4aac4f86 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/root/software/llvm-releases/llvm-0804/bin/../lib/../lib/libclangParse.so.22.0git+0x187f86)
#<!-- -->17 0x00007f2d4aac5d2b clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/root/software/llvm-releases/llvm-0804/bin/../lib/../lib/libclangParse.so.22.0git+0x188d2b)
#<!-- -->18 0x00007f2d4aacf473 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/root/software/llvm-releases/llvm-0804/bin/../lib/../lib/libclangParse.so.22.0git+0x192473)
#<!-- -->19 0x00007f2d4aad06d5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/root/software/llvm-releases/llvm-0804/bin/../lib/../lib/libclangParse.so.22.0git+0x1936d5)
#<!-- -->20 0x00007f2d4aad0c41 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/root/software/llvm-releases/llvm-0804/bin/../lib/../lib/libclangParse.so.22.0git+0x193c41)
#<!-- -->21 0x00007f2d4a97cff6 clang::ParseAST(clang::Sema&amp;, bool, bool) (/root/software/llvm-releases/llvm-0804/bin/../lib/../lib/libclangParse.so.22.0git+0x3fff6)
#<!-- -->22 0x00007f2d4f9d1236 clang::FrontendAction::Execute() (/root/software/llvm-releases/llvm-0804/bin/../lib/libclangFrontend.so.22.0git+0x124236)
#<!-- -->23 0x00007f2d4f947a9a clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/root/software/llvm-releases/llvm-0804/bin/../lib/libclangFrontend.so.22.0git+0x9aa9a)
#<!-- -->24 0x00007f2d516c77a2 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/root/software/llvm-releases/llvm-0804/bin/../lib/libclangFrontendTool.so.22.0git+0x57a2)
#<!-- -->25 0x0000000000414d9b cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/root/software/llvm-releases/llvm-0804/bin/clang-22+0x414d9b)
#<!-- -->26 0x000000000040c3c4 ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->27 0x00000000004101e1 clang_main(int, char**, llvm::ToolContext const&amp;) (/root/software/llvm-releases/llvm-0804/bin/clang-22+0x4101e1)
#<!-- -->28 0x000000000040bdbe main (/root/software/llvm-releases/llvm-0804/bin/clang-22+0x40bdbe)
#<!-- -->29 0x00007f2d4cf51d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->30 0x00007f2d4cf51e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->31 0x000000000040be05 _start (/root/software/llvm-releases/llvm-0804/bin/clang-22+0x40be05)
clang: error: unable to execute command: Segmentation fault (core dumped)
clang: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 22.0.0git (https://github.com/llvm/llvm-project.git 28ed57eda80dc5e3b7dd01d15100c126fa4e50f6)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /root/software/llvm-releases/llvm-0804/bin
clang: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang: note: diagnostic msg: /tmp/4-2695e1.c
clang: note: diagnostic msg: /tmp/4-2695e1.sh
clang: note: diagnostic msg: 
```

[4.zip](https://github.com/user-attachments/files/21695960/4.zip)
</details>


---

