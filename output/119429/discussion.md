# [clang] Crash at -O1: Assertion `materialized_use_empty() && "Uses remain when a value is destroyed!"' failed.

**Author:** cardigan1008
**URL:** https://github.com/llvm/llvm-project/issues/119429
**Status:** Closed
**Labels:** duplicate, llvm:codegen, crash
**Closed Date:** 2024-12-11T01:07:36Z

## Body

This code crash at `-O1`:
```c
int a;
long b;
int c(long d, long e) {
  long f = e - 2;
  if (f >= a)
    return 1;
  long *g = (long *)(d + f * sizeof(long));
  *g = b;
}
int h();
void i() {
  int k = h();
  {
    unsigned short j = k;
    c(j, j);
  }
}
```

Compiler Explorer: https://godbolt.org/z/63Yrh1f8o

Crash:
```console
While deleting: i64 %conv1
Use still stuck around after Def is destroyed:  %sunkaddr5 = mul i64 %conv1, 8, !dbg !53
clang: /root/llvm-project/llvm/lib/IR/Value.cpp:103: llvm::Value::~Value(): Assertion `materialized_use_empty() && "Uses remain when a value is destroyed!"' failed.
```

Backtrace:
```console
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -O1 -Wall -Wextra <source>
1.	<eof> parser at end of file
2.	Code generation
3.	Running pass 'Function Pass Manager' on module '<source>'.
4.	Running pass 'CodeGen Prepare' on function '@i'
 #0 0x0000000003c11508 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3c11508)
 #1 0x0000000003c0f214 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3c0f214)
 #2 0x0000000003b5c778 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000075473d242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x000075473d2969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x000075473d242476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x000075473d2287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x000075473d22871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x000075473d239e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x00000000035edfb0 llvm::Value::~Value() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x35edfb0)
#10 0x00000000035ee01a llvm::Value::deleteValue() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x35ee01a)
#11 0x0000000002ddad6f (anonymous namespace)::CodeGenPrepare::_run(llvm::Function&) CodeGenPrepare.cpp:0:0
#12 0x0000000002ddd34f (anonymous namespace)::CodeGenPrepareLegacyPass::runOnFunction(llvm::Function&) (.part.0) CodeGenPrepare.cpp:0:0
#13 0x000000000356c40f llvm::FPPassManager::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x356c40f)
#14 0x000000000356c7c1 llvm::FPPassManager::runOnModule(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x356c7c1)
#15 0x000000000356d061 llvm::legacy::PassManagerImpl::run(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x356d061)
#16 0x0000000003ec6167 clang::EmitBackendOutput(clang::DiagnosticsEngine&, clang::HeaderSearchOptions const&, clang::CodeGenOptions const&, clang::TargetOptions const&, clang::LangOptions const&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3ec6167)
#17 0x0000000004593bfe clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4593bfe)
#18 0x000000000654ea8c clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x654ea8c)
#19 0x0000000004593fd8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4593fd8)
#20 0x000000000484f3f9 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x484f3f9)
#21 0x00000000047cf08e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x47cf08e)
#22 0x000000000493a4be clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x493a4be)
#23 0x0000000000cb812f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xcb812f)
#24 0x0000000000cafbca ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#25 0x00000000045d7a39 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#26 0x0000000003b5cc24 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3b5cc24)
#27 0x00000000045d802f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#28 0x000000000459dabd clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x459dabd)
#29 0x000000000459ebad clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x459ebad)
#30 0x00000000045a5f65 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x45a5f65)
#31 0x0000000000cb4f73 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xcb4f73)
#32 0x0000000000b83334 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xb83334)
#33 0x000075473d229d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#34 0x000075473d229e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#35 0x0000000000caf675 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xcaf675)
```

## Comments

### Comment 1 - shafik

Looks like a clang-18 regression: https://godbolt.org/z/Wh6ETbvM6

---

### Comment 2 - dtcxzyw

Duplicate of https://github.com/llvm/llvm-project/issues/70938. I have confirmed that it can be fixed by https://github.com/llvm/llvm-project/pull/71058.


---

