# [clang] Crash at -O1: Assertion `materialized_use_empty() && "Uses remain when a value is destroyed!"' failed.

**Author:** cardigan1008
**URL:** https://github.com/llvm/llvm-project/issues/121915
**Status:** Closed
**Labels:** duplicate, llvm:codegen, crash-on-valid
**Closed Date:** 2025-01-07T12:53:26Z

## Body

This code crashes at `-O1`:

```c
int a, b, c;
unsigned char d;
short e;
int f(long g, int h) {
  switch (b) {
  case 2:
    return g;
  default:
    return *(int *)(h + g * sizeof(int));
  }
}
void i(long);
void j() {
  e = d >> a;
  i(e);
}
void i(long g) { c = f(g + 4, g); }
void main() {}
```

Compiler Explorer: https://godbolt.org/z/MT6jTvrhr

Crash:

```console
While deleting: i64 %conv2
Use still stuck around after Def is destroyed:  %sunkaddr7 = mul i64 %conv2, 4, !dbg !63
clang: /root/llvm-project/llvm/lib/IR/Value.cpp:103: llvm::Value::~Value(): Assertion `materialized_use_empty() && "Uses remain when a value is destroyed!"' failed.
```

Backtrace:

```console
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -O1 <source>
1.	<eof> parser at end of file
2.	Code generation
3.	Running pass 'Function Pass Manager' on module '<source>'.
4.	Running pass 'CodeGen Prepare' on function '@j'
 #0 0x0000000003c71738 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3c71738)
 #1 0x0000000003c6f444 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3c6f444)
 #2 0x0000000003bbc988 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x0000768367842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007683678969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x0000768367842476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007683678287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x000076836782871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x0000768367839e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000003648710 llvm::Value::~Value() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3648710)
#10 0x000000000364877a llvm::Value::deleteValue() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x364877a)
#11 0x0000000002e2e59f (anonymous namespace)::CodeGenPrepare::_run(llvm::Function&) CodeGenPrepare.cpp:0:0
#12 0x0000000002e30b7f (anonymous namespace)::CodeGenPrepareLegacyPass::runOnFunction(llvm::Function&) (.part.0) CodeGenPrepare.cpp:0:0
#13 0x00000000035c6cff llvm::FPPassManager::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x35c6cff)
#14 0x00000000035c70b1 llvm::FPPassManager::runOnModule(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x35c70b1)
#15 0x00000000035c7951 llvm::legacy::PassManagerImpl::run(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x35c7951)
#16 0x0000000003f285b2 clang::EmitBackendOutput(clang::DiagnosticsEngine&, clang::HeaderSearchOptions const&, clang::CodeGenOptions const&, clang::TargetOptions const&, clang::LangOptions const&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3f285b2)
#17 0x000000000460209e clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x460209e)
#18 0x000000000660f9cc clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x660f9cc)
#19 0x0000000004602478 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4602478)
#20 0x00000000048be799 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x48be799)
#21 0x0000000004840e2e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4840e2e)
#22 0x00000000049abb8e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x49abb8e)
#23 0x0000000000ce93ef cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xce93ef)
#24 0x0000000000ce0e8a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#25 0x0000000004647ae9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#26 0x0000000003bbce34 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3bbce34)
#27 0x00000000046480df clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#28 0x000000000460c1ed clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x460c1ed)
#29 0x000000000460d26e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x460d26e)
#30 0x0000000004614d95 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4614d95)
#31 0x0000000000ce6233 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xce6233)
#32 0x0000000000bb2524 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xbb2524)
#33 0x0000768367829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#34 0x0000768367829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#35 0x0000000000ce0935 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xce0935)
````

## Comments

### Comment 1 - nikic

Probably another duplicate of https://github.com/llvm/llvm-project/issues/70938? cc @dtcxzyw

---

