# clang-20 crashed with preserve_most attribute and m16. Assertion `!KeyInfoT::isEqual(Val, EmptyKey) && !KeyInfoT::isEqual(Val, TombstoneKey) && "Empty/Tombstone value shouldn't be inserted into map!"` failed.

**Author:** iamanonymouscs
**URL:** https://github.com/llvm/llvm-project/issues/113401

## Body

Clang-20 crashed with ```preserve_most``` atrribute and ```-m16```.

Compiler explorer: https://godbolt.org/z/o8afxa8dP

```
$cat mutant.c
int __attribute__((preserve_most)) a(char c) {
  for (;;)
    ;
}

Also crashed on clang-19.

$clang-19 -m16 mutant.c
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /usr/lib/llvm-19/bin/clang -cc1 -triple i386-pc-linux-code16 -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name mutant.c -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu i686 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/home/code/InsertAttr -fcoverage-compilation-dir=/home/code/InsertAttr -resource-dir /usr/lib/llvm-19/lib/clang/19 -internal-isystem /usr/lib/llvm-19/lib/clang/19/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../x86_64-linux-gnu/include -internal-externc-isystem /include -internal-externc-isystem /usr/include -ferror-limit 19 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/mutant-c2a10a.o -x c mutant.c
1.      <eof> parser at end of file
2.      Code generation
3.      Running pass 'Function Pass Manager' on module 'mutant.c'.
4.      Running pass 'Check CFA info and insert CFI instructions if needed' on function '@a'
 #0 0x00007fbef0259246 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.0+0xea7246)
 #1 0x00007fbef0256e20 llvm::sys::RunSignalHandlers() (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.0+0xea4e20)
 #2 0x00007fbef025990b (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.0+0xea790b)
 #3 0x00007fbeeee91520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007fbef04cce7e (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.0+0x111ae7e)
 #5 0x00007fbef0639738 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.0+0x1287738)
 #6 0x00007fbef03a76f4 llvm::FPPassManager::runOnFunction(llvm::Function&) (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.0+0xff56f4)
 #7 0x00007fbef03acfe3 llvm::FPPassManager::runOnModule(llvm::Module&) (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.0+0xffafe3)
 #8 0x00007fbef03a7e4e llvm::legacy::PassManagerImpl::run(llvm::Module&) (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.0+0xff5e4e)
 #9 0x00007fbef8d64c7e clang::EmitBackendOutput(clang::DiagnosticsEngine&, clang::HeaderSearchOptions const&, clang::CodeGenOptions const&, clang::TargetOptions const&, clang::LangOptions const&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x1e20c7e)
#10 0x00007fbef910a132 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x21c6132)
#11 0x00007fbef7b73739 clang::ParseAST(clang::Sema&, bool, bool) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0xc2f739)
#12 0x00007fbef9c060b5 clang::FrontendAction::Execute() (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x2cc20b5)
#13 0x00007fbef9b762f4 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x2c322f4)
#14 0x00007fbef9c82dee clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.0+0x2d3edee)
#15 0x00005635aba7dbc5 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/usr/lib/llvm-19/bin/clang+0x11bc5)
#16 0x00005635aba7aad5 (/usr/lib/llvm-19/bin/clang+0xead5)
#17 0x00005635aba79c9b clang_main(int, char**, llvm::ToolContext const&) (/usr/lib/llvm-19/bin/clang+0xdc9b)
#18 0x00005635aba87f36 main (/usr/lib/llvm-19/bin/clang+0x1bf36)
#19 0x00007fbeeee78d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#20 0x00007fbeeee78e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#21 0x00005635aba78765 _start (/usr/lib/llvm-19/bin/clang+0xc765)
clang: error: unable to execute command: Segmentation fault (core dumped)
clang: error: clang frontend command failed due to signal (use -v to see invocation)
Ubuntu clang version 19.0.0 (++20240722031324+65825cd5431c-1~exp1~20240722151445.1819)
Target: i386-pc-linux-code16
Thread model: posix
InstalledDir: /usr/lib/llvm-19/bin
clang: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang: note: diagnostic msg: /tmp/mutant-a90822.c
clang: note: diagnostic msg: /tmp/mutant-a90822.sh
clang: note: diagnostic msg: 

********************
```

## Comments

### Comment 1 - haberman

I got the same crash with `-m32` with a slightly different reproducer:

```c
int f(void);
__attribute__((preserve_most)) int g(void) { return f(); }
```

```
$ clang --version
Debian clang version 19.1.7 (3+build5)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/lib/llvm-19/bin
$ clang -c repro.c -m32
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: clang -c repro.c -m32
1.	<eof> parser at end of file
2.	Code generation
3.	Running pass 'Function Pass Manager' on module 'repro.c'.
4.	Running pass 'Check CFA info and insert CFI instructions if needed' on function '@g'
 #0 0x00007f5227eb067a llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.1+0xeb067a)
 #1 0x00007f5227eae224 llvm::sys::RunSignalHandlers() (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.1+0xeae224)
 #2 0x00007f5227df4144 (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.1+0xdf4144)
 #3 0x00007f5226a49df0 (/lib/x86_64-linux-gnu/libc.so.6+0x3fdf0)
 #4 0x00007f522812c4f2 (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.1+0x112c4f2)
 #5 0x00007f522829de28 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.1+0x129de28)
 #6 0x00007f5228004424 llvm::FPPassManager::runOnFunction(llvm::Function&) (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.1+0x1004424)
 #7 0x00007f5228009f33 llvm::FPPassManager::runOnModule(llvm::Module&) (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.1+0x1009f33)
 #8 0x00007f5228004b7e llvm::legacy::PassManagerImpl::run(llvm::Module&) (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.1+0x1004b7e)
 #9 0x00007f5230c7d0e8 clang::EmitBackendOutput(clang::DiagnosticsEngine&, clang::HeaderSearchOptions const&, clang::CodeGenOptions const&, clang::TargetOptions const&, clang::LangOptions const&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1e7d0e8)
#10 0x00007f523102a551 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x222a551)
#11 0x00007f522fa344e9 clang::ParseAST(clang::Sema&, bool, bool) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0xc344e9)
#12 0x00007f5231b47789 clang::FrontendAction::Execute() (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x2d47789)
#13 0x00007f5231ab6084 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x2cb6084)
#14 0x00007f5231bc5c0e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x2dc5c0e)
#15 0x0000559b85a4dc61 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/usr/lib/llvm-19/bin/clang+0x11c61)
#16 0x0000559b85a4ab35 (/usr/lib/llvm-19/bin/clang+0xeb35)
#17 0x00007f52317354cd (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x29354cd)
#18 0x00007f5227df3ee0 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.1+0xdf3ee0)
#19 0x00007f5231734f61 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x2934f61)
#20 0x00007f52316fa198 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x28fa198)
#21 0x00007f52316fa40e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x28fa40e)
#22 0x00007f5231717bcc clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x2917bcc)
#23 0x0000559b85a4a574 clang_main(int, char**, llvm::ToolContext const&) (/usr/lib/llvm-19/bin/clang+0xe574)
#24 0x0000559b85a584ba main (/usr/lib/llvm-19/bin/clang+0x1c4ba)
#25 0x00007f5226a33ca8 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#26 0x00007f5226a33d65 call_init ./csu/../csu/libc-start.c:128:20
#27 0x00007f5226a33d65 __libc_start_main ./csu/../csu/libc-start.c:347:5
#28 0x0000559b85a48731 _start (/usr/lib/llvm-19/bin/clang+0xc731)
clang: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Debian clang version 19.1.7 (3+build5)
Target: i386-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/lib/llvm-19/bin
clang: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang: note: diagnostic msg: /tmp/repro-3027eb.c
clang: note: diagnostic msg: /tmp/repro-3027eb.sh
clang: note: diagnostic msg: 

********************
```

---

### Comment 2 - nagisa

a similar crash with preserve_nonecc reduced to

```llvm
target datalayout = "e-m:e-p:32:32-p270:32:32-p271:32:32-p272:64:64-i128:128-f64:32:64-f80:32-n8:16:32-S128"
target triple = "i586-unknown-linux-gnu"

declare void @whatever()

define internal preserve_nonecc void @mini() {
start:
  tail call void @whatever()
  unreachable
}
```

for me. Replace `preserve_nonecc` with `preserve_mostcc` or `preserve_allcc` and it still crashes.

---

