# [AArch64][MachineSink] Clang crashes at -O1/2/3/s on aarch64: Segmentation fault in machine code sinking

**Author:** zxt5
**URL:** https://github.com/llvm/llvm-project/issues/187785
**Status:** Closed
**Labels:** llvm:codegen, crash
**Closed Date:** 2026-03-25T11:52:26Z

## Body

Compiler Explorer: https://godbolt.org/z/vo6bMhbPK

small.c
```c
void b(int c, long d) { *(short *)(d + c) = 0; }
void f(int c) { b(c - 2, c + 2); }
int main() {}
```

Crash:
```
clang-trunk -O1 small.c
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /data/x27zhou/compiler-nightly/install/llvm/bin/clang-23 -cc1 -triple aarch64-unknown-linux-gnu -O1 -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -main-file-name small.c -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=non-leaf-no-reserve -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -enable-tlsdesc -target-cpu generic -target-feature +v8a -target-feature +fp-armv8 -target-feature +neon -target-abi aapcs -debugger-tuning=gdb -fdebug-compilation-dir=/data/x27zhou/fuzz-pgo/PGOFuzz/pgofuzz2-output-2026-03-20-03-00-09/crash-clang-trunk-61385/reduction -fcoverage-compilation-dir=/data/x27zhou/fuzz-pgo/PGOFuzz/pgofuzz2-output-2026-03-20-03-00-09/crash-clang-trunk-61385/reduction -resource-dir /data/x27zhou/compiler-nightly/install/llvm/lib/clang/23 -internal-isystem /data/x27zhou/compiler-nightly/install/llvm/lib/clang/23/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/aarch64-linux-gnu/11/../../../../aarch64-linux-gnu/include -internal-externc-isystem /usr/include/aarch64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -ferror-limit 19 -fmessage-length=309 -fno-signed-char -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -target-feature +outline-atomics -target-feature -fmv -faddrsig -fdwarf2-cfi-asm -o /tmp/small-ac160b.o -x c small.c
1.      <eof> parser at end of file
2.      Code generation
3.      Running pass 'Function Pass Manager' on module 'small.c'.
4.      Running pass 'Machine code sinking' on function '@f'
 #0 0x0000aaaae511c128 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/Support/Unix/Signals.inc:884:3
 #1 0x0000aaaae51196cc llvm::sys::RunSignalHandlers() /data/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/Support/Signals.cpp:109:18
 #2 0x0000aaaae5119d90 SignalHandler(int, siginfo_t*, void*) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/Support/Unix/Signals.inc:448:14
 #3 0x0000aaaae5119e88 int const* std::__find_if<int const*, __gnu_cxx::__ops::_Iter_equals_val<int const>>(int const*, int const*, __gnu_cxx::__ops::_Iter_equals_val<int const>) /usr/include/c++/11/bits/stl_algobase.h:2114:23
 #4 0x0000aaaae5119e88 int const* std::find<int const*, int>(int const*, int const*, int const&) /usr/include/c++/11/bits/stl_algo.h:3884:28
 #5 0x0000aaaae5119e88 bool llvm::is_contained<int const (&) [10], int>(int const (&) [10], int const&) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/include/llvm/ADT/STLExtras.h:1953:21
 #6 0x0000aaaae5119e88 SignalHandlerTerminate(int, siginfo_t*, void*) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/Support/Unix/Signals.inc:464:25
 #7 0x0000ffffa694d7dc (linux-vdso.so.1+0x7dc)
 #8 0x0000aaaae453cad4 llvm::MachineInstr::getMF() const /data/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/CodeGen/MachineInstr.cpp:788:1
 #9 0x0000aaaae23c3fa8 llvm::AArch64InstrInfo::emitLdStWithAddr(llvm::MachineInstr&, llvm::ExtAddrMode const&) const /data/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/Target/AArch64/AArch64InstrInfo.cpp:4085:0
#10 0x0000aaaae45f1c84 PerformSinkAndFold /data/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/CodeGen/MachineSink.cpp:586:34
#11 0x0000aaaae45f1c84 (anonymous namespace)::MachineSinking::ProcessBlock(llvm::MachineBasicBlock&) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/CodeGen/MachineSink.cpp:985:48
#12 0x0000aaaae45f22b0 (anonymous namespace)::MachineSinking::run(llvm::MachineFunction&) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/CodeGen/MachineSink.cpp:850:33
#13 0x0000aaaae45f3fa8 (anonymous namespace)::MachineSinkingLegacy::runOnMachineFunction(llvm::MachineFunction&) (.part.0) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/CodeGen/MachineSink.cpp:827:18
#14 0x0000aaaae4533634 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (.part.0) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/CodeGen/MachineFunctionPass.cpp:112:30
#15 0x0000aaaae4aae3d0 llvm::FPPassManager::runOnFunction(llvm::Function&) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:1398:40
#16 0x0000aaaae4aae630 llvm::FPPassManager::runOnModule(llvm::Module&) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:1444:29
#17 0x0000aaaae4aaee04 runOnModule /data/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:1513:38
#18 0x0000aaaae4aaee04 llvm::legacy::PassManagerImpl::run(llvm::Module&) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:531:55
#19 0x0000aaaae54c80c0 RunCodegenPipeline /data/x27zhou/compiler-nightly/src/llvm-project/clang/lib/CodeGen/BackendUtil.cpp:1278:9
#20 0x0000aaaae54c80c0 emitAssembly /data/x27zhou/compiler-nightly/src/llvm-project/clang/lib/CodeGen/BackendUtil.cpp:1301:21
#21 0x0000aaaae54c80c0 clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) /data/x27zhou/compiler-nightly/src/llvm-project/clang/lib/CodeGen/BackendUtil.cpp:1475:25
#22 0x0000aaaae5aa7194 std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>::~unique_ptr() /usr/include/c++/11/bits/unique_ptr.h:360:12
#23 0x0000aaaae5aa7194 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) /data/x27zhou/compiler-nightly/src/llvm-project/clang/lib/CodeGen/CodeGenAction.cpp:312:20
#24 0x0000aaaae7097020 clang::ParseAST(clang::Sema&, bool, bool) /data/x27zhou/compiler-nightly/src/llvm-project/clang/lib/Parse/ParseAST.cpp:183:34
#25 0x0000aaaae5d2ec5c clang::FrontendAction::Execute() /data/x27zhou/compiler-nightly/src/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1330:38
#26 0x0000aaaae5ccfbf0 llvm::Error::getPtr() const /data/x27zhou/compiler-nightly/src/llvm-project/llvm/include/llvm/Support/Error.h:278:51
#27 0x0000aaaae5ccfbf0 llvm::Error::operator bool() /data/x27zhou/compiler-nightly/src/llvm-project/llvm/include/llvm/Support/Error.h:241:22
#28 0x0000aaaae5ccfbf0 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) /data/x27zhou/compiler-nightly/src/llvm-project/clang/lib/Frontend/CompilerInstance.cpp:989:42
#29 0x0000aaaae5e04b4c std::__shared_ptr<clang::FrontendOptions, (__gnu_cxx::_Lock_policy)2>::get() const /usr/include/c++/11/bits/shared_ptr_base.h:1296:16
#30 0x0000aaaae5e04b4c std::__shared_ptr_access<clang::FrontendOptions, (__gnu_cxx::_Lock_policy)2, false, false>::_M_get() const /usr/include/c++/11/bits/shared_ptr_base.h:993:69
#31 0x0000aaaae5e04b4c std::__shared_ptr_access<clang::FrontendOptions, (__gnu_cxx::_Lock_policy)2, false, false>::operator*() const /usr/include/c++/11/bits/shared_ptr_base.h:979:2
#32 0x0000aaaae5e04b4c clang::CompilerInvocation::getFrontendOpts() /data/x27zhou/compiler-nightly/src/llvm-project/clang/include/clang/Frontend/CompilerInvocation.h:277:48
#33 0x0000aaaae5e04b4c clang::CompilerInstance::getFrontendOpts() /data/x27zhou/compiler-nightly/src/llvm-project/clang/include/clang/Frontend/CompilerInstance.h:310:39
#34 0x0000aaaae5e04b4c clang::ExecuteCompilerInvocation(clang::CompilerInstance*) /data/x27zhou/compiler-nightly/src/llvm-project/clang/lib/FrontendTool/ExecuteCompilerInvocation.cpp:319:29
#35 0x0000aaaae22a51d0 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) /data/x27zhou/compiler-nightly/src/llvm-project/clang/tools/driver/cc1_main.cpp:302:40
#36 0x0000aaaae229d7c4 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) /data/x27zhou/compiler-nightly/src/llvm-project/clang/tools/driver/driver.cpp:229:20
#37 0x0000aaaae22a0110 llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>::release() /data/x27zhou/compiler-nightly/src/llvm-project/llvm/include/llvm/ADT/IntrusiveRefCntPtr.h:232:9
#38 0x0000aaaae22a0110 llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>::~IntrusiveRefCntPtr() /data/x27zhou/compiler-nightly/src/llvm-project/llvm/include/llvm/ADT/IntrusiveRefCntPtr.h:196:34
#39 0x0000aaaae22a0110 clang_main(int, char**, llvm::ToolContext const&) /data/x27zhou/compiler-nightly/src/llvm-project/clang/tools/driver/driver.cpp:277:26
#40 0x0000aaaae21bb674 main /data/x27zhou/compiler-nightly/build/llvm/tools/clang/tools/driver/clang-driver.cpp:17:20
#41 0x0000ffffa6457400 (/lib/aarch64-linux-gnu/libc.so.6+0x27400)
#42 0x0000ffffa64574d8 __libc_start_main (/lib/aarch64-linux-gnu/libc.so.6+0x274d8)
#43 0x0000aaaae229cb30 _start (/data/x27zhou/compiler-nightly/install/llvm/bin/clang-23+0xf2cb30)
clang-trunk: error: unable to execute command: Segmentation fault (core dumped)
clang-trunk: error: clang frontend command failed with exit code -2 (use -v to see invocation)
clang version 23.0.0git (https://github.com/llvm/llvm-project.git e6789f94b9cb385e8dd339aa35f659fe47d76f49)
Target: aarch64-unknown-linux-gnu
Thread model: posix
InstalledDir: /data/x27zhou/compiler-nightly/install/llvm/bin
Build config: +assertions
clang-trunk: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang-trunk: note: diagnostic msg: /tmp/small-a6cab8.c
clang-trunk: note: diagnostic msg: /tmp/small-a6cab8.sh
clang-trunk: note: diagnostic msg: 

********************
```

Seems like an 18 regression.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-aarch64

Author: Xintong Zhou (zxt5)

<details>
Compiler Explorer: https://godbolt.org/z/vo6bMhbPK

small.c
```c
void b(int c, long d) { *(short *)(d + c) = 0; }
void f(int c) { b(c - 2, c + 2); }
int main() {}
```

Crash:
```
clang-trunk -O1 small.c
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /data/x27zhou/compiler-nightly/install/llvm/bin/clang-23 -cc1 -triple aarch64-unknown-linux-gnu -O1 -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -main-file-name small.c -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=non-leaf-no-reserve -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -enable-tlsdesc -target-cpu generic -target-feature +v8a -target-feature +fp-armv8 -target-feature +neon -target-abi aapcs -debugger-tuning=gdb -fdebug-compilation-dir=/data/x27zhou/fuzz-pgo/PGOFuzz/pgofuzz2-output-2026-03-20-03-00-09/crash-clang-trunk-61385/reduction -fcoverage-compilation-dir=/data/x27zhou/fuzz-pgo/PGOFuzz/pgofuzz2-output-2026-03-20-03-00-09/crash-clang-trunk-61385/reduction -resource-dir /data/x27zhou/compiler-nightly/install/llvm/lib/clang/23 -internal-isystem /data/x27zhou/compiler-nightly/install/llvm/lib/clang/23/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/aarch64-linux-gnu/11/../../../../aarch64-linux-gnu/include -internal-externc-isystem /usr/include/aarch64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -ferror-limit 19 -fmessage-length=309 -fno-signed-char -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -target-feature +outline-atomics -target-feature -fmv -faddrsig -fdwarf2-cfi-asm -o /tmp/small-ac160b.o -x c small.c
1.      &lt;eof&gt; parser at end of file
2.      Code generation
3.      Running pass 'Function Pass Manager' on module 'small.c'.
4.      Running pass 'Machine code sinking' on function '@<!-- -->f'
 #<!-- -->0 0x0000aaaae511c128 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/Support/Unix/Signals.inc:884:3
 #<!-- -->1 0x0000aaaae51196cc llvm::sys::RunSignalHandlers() /data/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/Support/Signals.cpp:109:18
 #<!-- -->2 0x0000aaaae5119d90 SignalHandler(int, siginfo_t*, void*) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/Support/Unix/Signals.inc:448:14
 #<!-- -->3 0x0000aaaae5119e88 int const* std::__find_if&lt;int const*, __gnu_cxx::__ops::_Iter_equals_val&lt;int const&gt;&gt;(int const*, int const*, __gnu_cxx::__ops::_Iter_equals_val&lt;int const&gt;) /usr/include/c++/11/bits/stl_algobase.h:2114:23
 #<!-- -->4 0x0000aaaae5119e88 int const* std::find&lt;int const*, int&gt;(int const*, int const*, int const&amp;) /usr/include/c++/11/bits/stl_algo.h:3884:28
 #<!-- -->5 0x0000aaaae5119e88 bool llvm::is_contained&lt;int const (&amp;) [10], int&gt;(int const (&amp;) [10], int const&amp;) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/include/llvm/ADT/STLExtras.h:1953:21
 #<!-- -->6 0x0000aaaae5119e88 SignalHandlerTerminate(int, siginfo_t*, void*) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/Support/Unix/Signals.inc:464:25
 #<!-- -->7 0x0000ffffa694d7dc (linux-vdso.so.1+0x7dc)
 #<!-- -->8 0x0000aaaae453cad4 llvm::MachineInstr::getMF() const /data/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/CodeGen/MachineInstr.cpp:788:1
 #<!-- -->9 0x0000aaaae23c3fa8 llvm::AArch64InstrInfo::emitLdStWithAddr(llvm::MachineInstr&amp;, llvm::ExtAddrMode const&amp;) const /data/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/Target/AArch64/AArch64InstrInfo.cpp:4085:0
#<!-- -->10 0x0000aaaae45f1c84 PerformSinkAndFold /data/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/CodeGen/MachineSink.cpp:586:34
#<!-- -->11 0x0000aaaae45f1c84 (anonymous namespace)::MachineSinking::ProcessBlock(llvm::MachineBasicBlock&amp;) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/CodeGen/MachineSink.cpp:985:48
#<!-- -->12 0x0000aaaae45f22b0 (anonymous namespace)::MachineSinking::run(llvm::MachineFunction&amp;) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/CodeGen/MachineSink.cpp:850:33
#<!-- -->13 0x0000aaaae45f3fa8 (anonymous namespace)::MachineSinkingLegacy::runOnMachineFunction(llvm::MachineFunction&amp;) (.part.0) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/CodeGen/MachineSink.cpp:827:18
#<!-- -->14 0x0000aaaae4533634 llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (.part.0) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/CodeGen/MachineFunctionPass.cpp:112:30
#<!-- -->15 0x0000aaaae4aae3d0 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:1398:40
#<!-- -->16 0x0000aaaae4aae630 llvm::FPPassManager::runOnModule(llvm::Module&amp;) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:1444:29
#<!-- -->17 0x0000aaaae4aaee04 runOnModule /data/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:1513:38
#<!-- -->18 0x0000aaaae4aaee04 llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:531:55
#<!-- -->19 0x0000aaaae54c80c0 RunCodegenPipeline /data/x27zhou/compiler-nightly/src/llvm-project/clang/lib/CodeGen/BackendUtil.cpp:1278:9
#<!-- -->20 0x0000aaaae54c80c0 emitAssembly /data/x27zhou/compiler-nightly/src/llvm-project/clang/lib/CodeGen/BackendUtil.cpp:1301:21
#<!-- -->21 0x0000aaaae54c80c0 clang::emitBackendOutput(clang::CompilerInstance&amp;, clang::CodeGenOptions&amp;, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;, std::unique_ptr&lt;llvm::raw_pwrite_stream, std::default_delete&lt;llvm::raw_pwrite_stream&gt;&gt;, clang::BackendConsumer*) /data/x27zhou/compiler-nightly/src/llvm-project/clang/lib/CodeGen/BackendUtil.cpp:1475:25
#<!-- -->22 0x0000aaaae5aa7194 std::unique_ptr&lt;llvm::raw_pwrite_stream, std::default_delete&lt;llvm::raw_pwrite_stream&gt;&gt;::~unique_ptr() /usr/include/c++/11/bits/unique_ptr.h:360:12
#<!-- -->23 0x0000aaaae5aa7194 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&amp;) /data/x27zhou/compiler-nightly/src/llvm-project/clang/lib/CodeGen/CodeGenAction.cpp:312:20
#<!-- -->24 0x0000aaaae7097020 clang::ParseAST(clang::Sema&amp;, bool, bool) /data/x27zhou/compiler-nightly/src/llvm-project/clang/lib/Parse/ParseAST.cpp:183:34
#<!-- -->25 0x0000aaaae5d2ec5c clang::FrontendAction::Execute() /data/x27zhou/compiler-nightly/src/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1330:38
#<!-- -->26 0x0000aaaae5ccfbf0 llvm::Error::getPtr() const /data/x27zhou/compiler-nightly/src/llvm-project/llvm/include/llvm/Support/Error.h:278:51
#<!-- -->27 0x0000aaaae5ccfbf0 llvm::Error::operator bool() /data/x27zhou/compiler-nightly/src/llvm-project/llvm/include/llvm/Support/Error.h:241:22
#<!-- -->28 0x0000aaaae5ccfbf0 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) /data/x27zhou/compiler-nightly/src/llvm-project/clang/lib/Frontend/CompilerInstance.cpp:989:42
#<!-- -->29 0x0000aaaae5e04b4c std::__shared_ptr&lt;clang::FrontendOptions, (__gnu_cxx::_Lock_policy)2&gt;::get() const /usr/include/c++/11/bits/shared_ptr_base.h:1296:16
#<!-- -->30 0x0000aaaae5e04b4c std::__shared_ptr_access&lt;clang::FrontendOptions, (__gnu_cxx::_Lock_policy)2, false, false&gt;::_M_get() const /usr/include/c++/11/bits/shared_ptr_base.h:993:69
#<!-- -->31 0x0000aaaae5e04b4c std::__shared_ptr_access&lt;clang::FrontendOptions, (__gnu_cxx::_Lock_policy)2, false, false&gt;::operator*() const /usr/include/c++/11/bits/shared_ptr_base.h:979:2
#<!-- -->32 0x0000aaaae5e04b4c clang::CompilerInvocation::getFrontendOpts() /data/x27zhou/compiler-nightly/src/llvm-project/clang/include/clang/Frontend/CompilerInvocation.h:277:48
#<!-- -->33 0x0000aaaae5e04b4c clang::CompilerInstance::getFrontendOpts() /data/x27zhou/compiler-nightly/src/llvm-project/clang/include/clang/Frontend/CompilerInstance.h:310:39
#<!-- -->34 0x0000aaaae5e04b4c clang::ExecuteCompilerInvocation(clang::CompilerInstance*) /data/x27zhou/compiler-nightly/src/llvm-project/clang/lib/FrontendTool/ExecuteCompilerInvocation.cpp:319:29
#<!-- -->35 0x0000aaaae22a51d0 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) /data/x27zhou/compiler-nightly/src/llvm-project/clang/tools/driver/cc1_main.cpp:302:40
#<!-- -->36 0x0000aaaae229d7c4 ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) /data/x27zhou/compiler-nightly/src/llvm-project/clang/tools/driver/driver.cpp:229:20
#<!-- -->37 0x0000aaaae22a0110 llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;::release() /data/x27zhou/compiler-nightly/src/llvm-project/llvm/include/llvm/ADT/IntrusiveRefCntPtr.h:232:9
#<!-- -->38 0x0000aaaae22a0110 llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;::~IntrusiveRefCntPtr() /data/x27zhou/compiler-nightly/src/llvm-project/llvm/include/llvm/ADT/IntrusiveRefCntPtr.h:196:34
#<!-- -->39 0x0000aaaae22a0110 clang_main(int, char**, llvm::ToolContext const&amp;) /data/x27zhou/compiler-nightly/src/llvm-project/clang/tools/driver/driver.cpp:277:26
#<!-- -->40 0x0000aaaae21bb674 main /data/x27zhou/compiler-nightly/build/llvm/tools/clang/tools/driver/clang-driver.cpp:17:20
#<!-- -->41 0x0000ffffa6457400 (/lib/aarch64-linux-gnu/libc.so.6+0x27400)
#<!-- -->42 0x0000ffffa64574d8 __libc_start_main (/lib/aarch64-linux-gnu/libc.so.6+0x274d8)
#<!-- -->43 0x0000aaaae229cb30 _start (/data/x27zhou/compiler-nightly/install/llvm/bin/clang-23+0xf2cb30)
clang-trunk: error: unable to execute command: Segmentation fault (core dumped)
clang-trunk: error: clang frontend command failed with exit code -2 (use -v to see invocation)
clang version 23.0.0git (https://github.com/llvm/llvm-project.git e6789f94b9cb385e8dd339aa35f659fe47d76f49)
Target: aarch64-unknown-linux-gnu
Thread model: posix
InstalledDir: /data/x27zhou/compiler-nightly/install/llvm/bin
Build config: +assertions
clang-trunk: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang-trunk: note: diagnostic msg: /tmp/small-a6cab8.c
clang-trunk: note: diagnostic msg: /tmp/small-a6cab8.sh
clang-trunk: note: diagnostic msg: 

********************
```

Seems like an 18 regression.
</details>


---

### Comment 2 - davemgreen

IR example: https://godbolt.org/z/G1eobbs4T
@momchil-velikov any ideas?

---

### Comment 3 - momchil-velikov

I'll have a look. It seems we're trying to fold two things into the same instruction and it's been deleted the second time around.

---

