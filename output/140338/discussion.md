# [OpenMP] Assertion `(isUnevaluated() || getCharByteWidth() == 1) && "This function is used in places that assume strings use char"' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/140338

## Body

Reproducer:
https://godbolt.org/z/6cKcbE459
```c
#pragma omp error message(L"")
```

The original reproducer of this issue is the following:
https://github.com/gcc-mirror/gcc/blob/master/gcc/testsuite/c-c%2B%2B-common/gomp/error-3.c

Backtrace:
```console
clang: /root/llvm-project/llvm/tools/clang/include/clang/AST/Expr.h:1870: llvm::StringRef clang::StringLiteral::getString() const: Assertion `(isUnevaluated() || getCharByteWidth() == 1) && "This function is used in places that assume strings use char"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -fopenmp <source>
1.	<source>:1:31: at annotation token
 #0 0x0000000003f99d48 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3f99d48)
 #1 0x0000000003f979d4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3f979d4)
 #2 0x0000000003edc608 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007de9c9642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007de9c96969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007de9c9642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007de9c96287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007de9c962871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007de9c9639e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000006e59732 clang::SemaOpenMP::ActOnOpenMPErrorDirective(llvm::ArrayRef<clang::OMPClause*>, clang::SourceLocation, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6e59732)
#10 0x000000000671aabe clang::Parser::ParseOpenMPDeclarativeDirectiveWithExtDecl(clang::AccessSpecifier&, clang::ParsedAttributes&, bool, clang::TypeSpecifierType, clang::Decl*) (.part.0) ParseOpenMP.cpp:0:0
#11 0x0000000006639eb9 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6639eb9)
#12 0x000000000663ad5d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x663ad5d)
#13 0x000000000663b210 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x663b210)
#14 0x000000000662d0f3 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x662d0f3)
#15 0x0000000004944fd8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4944fd8)
#16 0x0000000004c3cea5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4c3cea5)
#17 0x0000000004bbbb3e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4bbbb3e)
#18 0x0000000004d30a49 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4d30a49)
#19 0x0000000000dac29f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xdac29f)
#20 0x0000000000da246a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#21 0x00000000049ac2c9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#22 0x0000000003edcaa4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3edcaa4)
#23 0x00000000049ac8df clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#24 0x000000000496e6ad clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x496e6ad)
#25 0x000000000496f73e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x496f73e)
#26 0x0000000004977a25 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4977a25)
#27 0x0000000000da8208 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xda8208)
#28 0x0000000000c2df44 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xc2df44)
#29 0x00007de9c9629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#30 0x00007de9c9629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#31 0x0000000000da1f15 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xda1f15)
clang: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/6cKcbE459
```c
#pragma omp error message(L"")
```
</details>


---

