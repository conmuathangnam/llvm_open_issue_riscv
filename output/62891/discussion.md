# Crash report from trying to compile garbage input

**Author:** nakst
**URL:** https://github.com/llvm/llvm-project/issues/62891
**Status:** Closed
**Labels:** duplicate, clang:frontend, crash-on-invalid
**Closed Date:** 2025-02-10T01:47:10Z

## Body

I was working on parallelizing my asset processor, and accidentally had multiple threads overwriting the same source file and trying to compile it. This caused Clang to crash.

I am running on Arch Linux 64-bit Intel.

```console
> clang --version
clang version 15.0.7
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
```

```console
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: clang --target=wasm32 -c -o bin/9_wasm.obj bin/embed_source.c
1.	<unknown> parser at unknown location
 #0 0x00007fe92d97c48a (/usr/lib/libLLVM-15.so+0xd7c48a)
 #1 0x00007fe92d979b1f llvm::sys::RunSignalHandlers() (/usr/lib/libLLVM-15.so+0xd79b1f)
 #2 0x00007fe92d85c5e9 (/usr/lib/libLLVM-15.so+0xc5c5e9)
 #3 0x00007fe92c651f50 (/usr/lib/libc.so.6+0x38f50)
 #4 0x00007fe934ded019 clang::Lexer::LexTokenInternal(clang::Token&, bool) (/usr/lib/libclang-cpp.so.15+0x7ed019)
 #5 0x00007fe934e2c7d7 clang::Preprocessor::Lex(clang::Token&) (/usr/lib/libclang-cpp.so.15+0x82c7d7)
 #6 0x00007fe934ea73de clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) (/usr/lib/libclang-cpp.so.15+0x8a73de)
 #7 0x00007fe936c323cd (/usr/lib/libclang-cpp.so.15+0x26323cd)
 #8 0x00007fe936c32546 (/usr/lib/libclang-cpp.so.15+0x2632546)
 #9 0x00007fe934e9e8a4 clang::Parser::ParseBraceInitializer() (/usr/lib/libclang-cpp.so.15+0x89e8a4)
#10 0x00007fe934e57985 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/usr/lib/libclang-cpp.so.15+0x857985)
#11 0x00007fe934e765f3 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/usr/lib/libclang-cpp.so.15+0x8765f3)
#12 0x00007fe934ee7292 (/usr/lib/libclang-cpp.so.15+0x8e7292)
#13 0x00007fe934efd01c clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/usr/lib/libclang-cpp.so.15+0x8fd01c)
#14 0x00007fe934efd400 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/usr/lib/libclang-cpp.so.15+0x8fd400)
#15 0x00007fe934efd748 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/usr/lib/libclang-cpp.so.15+0x8fd748)
#16 0x00007fe934e49134 clang::ParseAST(clang::Sema&, bool, bool) (/usr/lib/libclang-cpp.so.15+0x849134)
#17 0x00007fe9364e7479 clang::FrontendAction::Execute() (/usr/lib/libclang-cpp.so.15+0x1ee7479)
#18 0x00007fe93648d14f clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/usr/lib/libclang-cpp.so.15+0x1e8d14f)
#19 0x00007fe93655209a clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/lib/libclang-cpp.so.15+0x1f5209a)
#20 0x00005557d65884d0 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/usr/bin/clang-15+0x104d0)
#21 0x00005557d658e197 (/usr/bin/clang-15+0x16197)
#22 0x00007fe9361d5959 (/usr/lib/libclang-cpp.so.15+0x1bd5959)
#23 0x00007fe92d85c6b7 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/usr/lib/libLLVM-15.so+0xc5c6b7)
#24 0x00007fe9361db48f (/usr/lib/libclang-cpp.so.15+0x1bdb48f)
#25 0x00007fe93619ebbe clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/usr/lib/libclang-cpp.so.15+0x1b9ebbe)
#26 0x00007fe93619ef7d clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/usr/lib/libclang-cpp.so.15+0x1b9ef7d)
#27 0x00007fe9361f815c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/usr/lib/libclang-cpp.so.15+0x1bf815c)
#28 0x00005557d65907f9 clang_main(int, char**) (/usr/bin/clang-15+0x187f9)
#29 0x00007fe92c63c790 (/usr/lib/libc.so.6+0x23790)
#30 0x00007fe92c63c84a __libc_start_main (/usr/lib/libc.so.6+0x2384a)
#31 0x00005557d6584d25 _start (/usr/bin/clang-15+0xcd25)
clang-15: error: clang frontend command failed with exit code 135 (use -v to see invocation)
clang version 15.0.7
Target: wasm32
Thread model: posix
InstalledDir: /usr/bin
```

Attached files: 

[embed_source-4f2c07-script.txt](https://github.com/llvm/llvm-project/files/11546377/embed_source-4f2c07-script.txt)
[embed_source-4f2c07-source.txt](https://github.com/llvm/llvm-project/files/11546378/embed_source-4f2c07-source.txt)

Let me know if you need more information.

## Comments

### Comment 1 - llvmbot

@llvm/issue-subscribers-clang-frontend

---

### Comment 2 - shafik

I am not able to reproduce on godbolt: https://godbolt.org/z/x9z53svdb 

---

