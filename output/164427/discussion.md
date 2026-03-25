# [crash, reproducer] X86 DAG->DAG Instruction Selection

**Author:** attila-lendvai
**URL:** https://github.com/llvm/llvm-project/issues/164427
**Status:** Open
**Labels:** backend:X86, crash, llvm:SelectionDAG

## Body

# context

i'm working on https://github.com/attila-lendvai/maru and trying to access `argv` and `argc` on a freestanding build directly on top of the linux kernel.

with current trunk, i.e. beyond version 21.1.0

# reproducer

https://godbolt.org/z/n78dsvnPo

```llvm
%word = type i64
%oop = type ptr
declare ptr @llvm.sponentry()

define %oop @m__start.code() {
	%r3 = call ptr @llvm.sponentry()
	ret %oop %r3
}
```

output from the actual context:

```
clang -O3  -nostdlib -nostartfiles -ffreestanding -Wl,-Bstatic,-Ttext=0x08048000,-no-pie -Qunused-arguments --target=x86_64-linux-gnu -o build/llvm-linux/x86_64-linux-gnu/eval1  build/llvm-linux/x86_64-linux-gnu/eval1.ll
warning: overriding the module target triple with x86_64-unknown-linux-gnu [-Woverride-module]
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /gnu/store/0nmbzahh7blsn8xp8kmasa6zy147k5cd-clang-20.1.8/bin/clang-20 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -dumpdir build/llvm-linux/x86_64-linux-gnu/eval1- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name eval1.ll -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=none -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -ffreestanding -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/home/alendvai/common-lisp/maru -fcoverage-compilation-dir=/home/alendvai/common-lisp/maru -resource-dir /gnu/store/0nmbzahh7blsn8xp8kmasa6zy147k5cd-clang-20.1.8/lib/clang/20 -O3 -ferror-limit 19 -fmessage-length=159 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -vectorize-loops -vectorize-slp -faddrsig -o /tmp/eval1-cfad08.o -x ir build/llvm-linux/x86_64-linux-gnu/eval1.ll
1.	Code generation
2.	Running pass 'Function Pass Manager' on module 'build/llvm-linux/x86_64-linux-gnu/eval1.ll'.
3.	Running pass 'X86 DAG->DAG Instruction Selection' on function '@m__start.code'
 #0 0x00007f12adc293fb llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/gnu/store/rrrgwn9xnx5v4k4bmlkf499vi8d6p8cd-llvm-20.1.8/lib/libLLVM.so.20.1+0xc293fb)
 #1 0x00007f12adc26996 (/gnu/store/rrrgwn9xnx5v4k4bmlkf499vi8d6p8cd-llvm-20.1.8/lib/libLLVM.so.20.1+0xc26996)
 #2 0x00007f12aca51b00 __restore_rt (/gnu/store/yj053cys0724p7vs9kir808x7fivz17m-glibc-2.41/lib/libc.so.6+0x3fb00)
 #3 0x00007f12b24c9fc8 (/gnu/store/rrrgwn9xnx5v4k4bmlkf499vi8d6p8cd-llvm-20.1.8/lib/libLLVM.so.20.1+0x54c9fc8)
 #4 0x00007f12ae6e7d1a (/gnu/store/rrrgwn9xnx5v4k4bmlkf499vi8d6p8cd-llvm-20.1.8/lib/libLLVM.so.20.1+0x16e7d1a)
 #5 0x00007f12ae6eb235 llvm::SelectionDAG::Legalize() (/gnu/store/rrrgwn9xnx5v4k4bmlkf499vi8d6p8cd-llvm-20.1.8/lib/libLLVM.so.20.1+0x16eb235)
 #6 0x00007f12ae8d0066 llvm::SelectionDAGISel::CodeGenAndEmitDAG() (/gnu/store/rrrgwn9xnx5v4k4bmlkf499vi8d6p8cd-llvm-20.1.8/lib/libLLVM.so.20.1+0x18d0066)
 #7 0x00007f12ae8d2be4 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&) (/gnu/store/rrrgwn9xnx5v4k4bmlkf499vi8d6p8cd-llvm-20.1.8/lib/libLLVM.so.20.1+0x18d2be4)
 #8 0x00007f12ae8d4ac8 llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&) (/gnu/store/rrrgwn9xnx5v4k4bmlkf499vi8d6p8cd-llvm-20.1.8/lib/libLLVM.so.20.1+0x18d4ac8)
 #9 0x00007f12ae8c2f29 llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&) (/gnu/store/rrrgwn9xnx5v4k4bmlkf499vi8d6p8cd-llvm-20.1.8/lib/libLLVM.so.20.1+0x18c2f29)
#10 0x00007f12ae1d7193 (/gnu/store/rrrgwn9xnx5v4k4bmlkf499vi8d6p8cd-llvm-20.1.8/lib/libLLVM.so.20.1+0x11d7193)
#11 0x00007f12addfcd89 llvm::FPPassManager::runOnFunction(llvm::Function&) (/gnu/store/rrrgwn9xnx5v4k4bmlkf499vi8d6p8cd-llvm-20.1.8/lib/libLLVM.so.20.1+0xdfcd89)
#12 0x00007f12addfcf2c llvm::FPPassManager::runOnModule(llvm::Module&) (/gnu/store/rrrgwn9xnx5v4k4bmlkf499vi8d6p8cd-llvm-20.1.8/lib/libLLVM.so.20.1+0xdfcf2c)
#13 0x00007f12addfc154 llvm::legacy::PassManagerImpl::run(llvm::Module&) (/gnu/store/rrrgwn9xnx5v4k4bmlkf499vi8d6p8cd-llvm-20.1.8/lib/libLLVM.so.20.1+0xdfc154)
#14 0x00007f12b8cd69db clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/gnu/store/0nmbzahh7blsn8xp8kmasa6zy147k5cd-clang-20.1.8/lib/libclang-cpp.so.20.1+0x24d69db)
#15 0x00007f12b922c6eb clang::CodeGenAction::ExecuteAction() (/gnu/store/0nmbzahh7blsn8xp8kmasa6zy147k5cd-clang-20.1.8/lib/libclang-cpp.so.20.1+0x2a2c6eb)
#16 0x00007f12b9cc9ad1 clang::FrontendAction::Execute() (/gnu/store/0nmbzahh7blsn8xp8kmasa6zy147k5cd-clang-20.1.8/lib/libclang-cpp.so.20.1+0x34c9ad1)
#17 0x00007f12b9c42dee clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/gnu/store/0nmbzahh7blsn8xp8kmasa6zy147k5cd-clang-20.1.8/lib/libclang-cpp.so.20.1+0x3442dee)
#18 0x00007f12b9d68676 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/gnu/store/0nmbzahh7blsn8xp8kmasa6zy147k5cd-clang-20.1.8/lib/libclang-cpp.so.20.1+0x3568676)
#19 0x00000000004161bf cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/gnu/store/0nmbzahh7blsn8xp8kmasa6zy147k5cd-clang-20.1.8/bin/clang-20+0x4161bf)
#20 0x000000000040e7f3 (/gnu/store/0nmbzahh7blsn8xp8kmasa6zy147k5cd-clang-20.1.8/bin/clang-20+0x40e7f3)
#21 0x0000000000411f7d clang_main(int, char**, llvm::ToolContext const&) (/gnu/store/0nmbzahh7blsn8xp8kmasa6zy147k5cd-clang-20.1.8/bin/clang-20+0x411f7d)
#22 0x000000000040e203 main (/gnu/store/0nmbzahh7blsn8xp8kmasa6zy147k5cd-clang-20.1.8/bin/clang-20+0x40e203)
#23 0x00007f12aca3cbd7 __libc_start_call_main (/gnu/store/yj053cys0724p7vs9kir808x7fivz17m-glibc-2.41/lib/libc.so.6+0x2abd7)
#24 0x00007f12aca3cc95 __libc_start_main@GLIBC_2.2.5 (/gnu/store/yj053cys0724p7vs9kir808x7fivz17m-glibc-2.41/lib/libc.so.6+0x2ac95)
#25 0x000000000040e241 _start (/gnu/store/0nmbzahh7blsn8xp8kmasa6zy147k5cd-clang-20.1.8/bin/clang-20+0x40e241)
clang: error: unable to execute command: Segmentation fault
clang: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 20.1.8
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /gnu/store/0nmbzahh7blsn8xp8kmasa6zy147k5cd-clang-20.1.8/bin
clang: note: diagnostic msg: Error generating preprocessed source(s) - no preprocessable inputs.
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-x86

Author: Attila Lendvai (attila-lendvai)

<details>
# context

i'm working on https://github.com/attila-lendvai/maru and trying to access `argv` and `argc` on a freestanding build directly on top of the linux kernel.

with current trunk, i.e. beyond version 21.1.0

# reproducer

https://godbolt.org/z/n78dsvnPo

```
%word = type i64
%oop = type ptr
declare ptr @<!-- -->llvm.sponentry()

define %oop @<!-- -->m__start.code() {
	%r3 = call ptr @<!-- -->llvm.sponentry()
	ret %oop %r3
}
```

output from the actual context:

```
clang -O3  -nostdlib -nostartfiles -ffreestanding -Wl,-Bstatic,-Ttext=0x08048000,-no-pie -Qunused-arguments --target=x86_64-linux-gnu -o build/llvm-linux/x86_64-linux-gnu/eval1  build/llvm-linux/x86_64-linux-gnu/eval1.ll
warning: overriding the module target triple with x86_64-unknown-linux-gnu [-Woverride-module]
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /gnu/store/0nmbzahh7blsn8xp8kmasa6zy147k5cd-clang-20.1.8/bin/clang-20 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -dumpdir build/llvm-linux/x86_64-linux-gnu/eval1- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name eval1.ll -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=none -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -ffreestanding -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/home/alendvai/common-lisp/maru -fcoverage-compilation-dir=/home/alendvai/common-lisp/maru -resource-dir /gnu/store/0nmbzahh7blsn8xp8kmasa6zy147k5cd-clang-20.1.8/lib/clang/20 -O3 -ferror-limit 19 -fmessage-length=159 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -vectorize-loops -vectorize-slp -faddrsig -o /tmp/eval1-cfad08.o -x ir build/llvm-linux/x86_64-linux-gnu/eval1.ll
1.	Code generation
2.	Running pass 'Function Pass Manager' on module 'build/llvm-linux/x86_64-linux-gnu/eval1.ll'.
3.	Running pass 'X86 DAG-&gt;DAG Instruction Selection' on function '@<!-- -->m__start.code'
 #<!-- -->0 0x00007f12adc293fb llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/gnu/store/rrrgwn9xnx5v4k4bmlkf499vi8d6p8cd-llvm-20.1.8/lib/libLLVM.so.20.1+0xc293fb)
 #<!-- -->1 0x00007f12adc26996 (/gnu/store/rrrgwn9xnx5v4k4bmlkf499vi8d6p8cd-llvm-20.1.8/lib/libLLVM.so.20.1+0xc26996)
 #<!-- -->2 0x00007f12aca51b00 __restore_rt (/gnu/store/yj053cys0724p7vs9kir808x7fivz17m-glibc-2.41/lib/libc.so.6+0x3fb00)
 #<!-- -->3 0x00007f12b24c9fc8 (/gnu/store/rrrgwn9xnx5v4k4bmlkf499vi8d6p8cd-llvm-20.1.8/lib/libLLVM.so.20.1+0x54c9fc8)
 #<!-- -->4 0x00007f12ae6e7d1a (/gnu/store/rrrgwn9xnx5v4k4bmlkf499vi8d6p8cd-llvm-20.1.8/lib/libLLVM.so.20.1+0x16e7d1a)
 #<!-- -->5 0x00007f12ae6eb235 llvm::SelectionDAG::Legalize() (/gnu/store/rrrgwn9xnx5v4k4bmlkf499vi8d6p8cd-llvm-20.1.8/lib/libLLVM.so.20.1+0x16eb235)
 #<!-- -->6 0x00007f12ae8d0066 llvm::SelectionDAGISel::CodeGenAndEmitDAG() (/gnu/store/rrrgwn9xnx5v4k4bmlkf499vi8d6p8cd-llvm-20.1.8/lib/libLLVM.so.20.1+0x18d0066)
 #<!-- -->7 0x00007f12ae8d2be4 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&amp;) (/gnu/store/rrrgwn9xnx5v4k4bmlkf499vi8d6p8cd-llvm-20.1.8/lib/libLLVM.so.20.1+0x18d2be4)
 #<!-- -->8 0x00007f12ae8d4ac8 llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&amp;) (/gnu/store/rrrgwn9xnx5v4k4bmlkf499vi8d6p8cd-llvm-20.1.8/lib/libLLVM.so.20.1+0x18d4ac8)
 #<!-- -->9 0x00007f12ae8c2f29 llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&amp;) (/gnu/store/rrrgwn9xnx5v4k4bmlkf499vi8d6p8cd-llvm-20.1.8/lib/libLLVM.so.20.1+0x18c2f29)
#<!-- -->10 0x00007f12ae1d7193 (/gnu/store/rrrgwn9xnx5v4k4bmlkf499vi8d6p8cd-llvm-20.1.8/lib/libLLVM.so.20.1+0x11d7193)
#<!-- -->11 0x00007f12addfcd89 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (/gnu/store/rrrgwn9xnx5v4k4bmlkf499vi8d6p8cd-llvm-20.1.8/lib/libLLVM.so.20.1+0xdfcd89)
#<!-- -->12 0x00007f12addfcf2c llvm::FPPassManager::runOnModule(llvm::Module&amp;) (/gnu/store/rrrgwn9xnx5v4k4bmlkf499vi8d6p8cd-llvm-20.1.8/lib/libLLVM.so.20.1+0xdfcf2c)
#<!-- -->13 0x00007f12addfc154 llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/gnu/store/rrrgwn9xnx5v4k4bmlkf499vi8d6p8cd-llvm-20.1.8/lib/libLLVM.so.20.1+0xdfc154)
#<!-- -->14 0x00007f12b8cd69db clang::emitBackendOutput(clang::CompilerInstance&amp;, clang::CodeGenOptions&amp;, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;, std::unique_ptr&lt;llvm::raw_pwrite_stream, std::default_delete&lt;llvm::raw_pwrite_stream&gt;&gt;, clang::BackendConsumer*) (/gnu/store/0nmbzahh7blsn8xp8kmasa6zy147k5cd-clang-20.1.8/lib/libclang-cpp.so.20.1+0x24d69db)
#<!-- -->15 0x00007f12b922c6eb clang::CodeGenAction::ExecuteAction() (/gnu/store/0nmbzahh7blsn8xp8kmasa6zy147k5cd-clang-20.1.8/lib/libclang-cpp.so.20.1+0x2a2c6eb)
#<!-- -->16 0x00007f12b9cc9ad1 clang::FrontendAction::Execute() (/gnu/store/0nmbzahh7blsn8xp8kmasa6zy147k5cd-clang-20.1.8/lib/libclang-cpp.so.20.1+0x34c9ad1)
#<!-- -->17 0x00007f12b9c42dee clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/gnu/store/0nmbzahh7blsn8xp8kmasa6zy147k5cd-clang-20.1.8/lib/libclang-cpp.so.20.1+0x3442dee)
#<!-- -->18 0x00007f12b9d68676 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/gnu/store/0nmbzahh7blsn8xp8kmasa6zy147k5cd-clang-20.1.8/lib/libclang-cpp.so.20.1+0x3568676)
#<!-- -->19 0x00000000004161bf cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/gnu/store/0nmbzahh7blsn8xp8kmasa6zy147k5cd-clang-20.1.8/bin/clang-20+0x4161bf)
#<!-- -->20 0x000000000040e7f3 (/gnu/store/0nmbzahh7blsn8xp8kmasa6zy147k5cd-clang-20.1.8/bin/clang-20+0x40e7f3)
#<!-- -->21 0x0000000000411f7d clang_main(int, char**, llvm::ToolContext const&amp;) (/gnu/store/0nmbzahh7blsn8xp8kmasa6zy147k5cd-clang-20.1.8/bin/clang-20+0x411f7d)
#<!-- -->22 0x000000000040e203 main (/gnu/store/0nmbzahh7blsn8xp8kmasa6zy147k5cd-clang-20.1.8/bin/clang-20+0x40e203)
#<!-- -->23 0x00007f12aca3cbd7 __libc_start_call_main (/gnu/store/yj053cys0724p7vs9kir808x7fivz17m-glibc-2.41/lib/libc.so.6+0x2abd7)
#<!-- -->24 0x00007f12aca3cc95 __libc_start_main@<!-- -->GLIBC_2.2.5 (/gnu/store/yj053cys0724p7vs9kir808x7fivz17m-glibc-2.41/lib/libc.so.6+0x2ac95)
#<!-- -->25 0x000000000040e241 _start (/gnu/store/0nmbzahh7blsn8xp8kmasa6zy147k5cd-clang-20.1.8/bin/clang-20+0x40e241)
clang: error: unable to execute command: Segmentation fault
clang: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 20.1.8
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /gnu/store/0nmbzahh7blsn8xp8kmasa6zy147k5cd-clang-20.1.8/bin
clang: note: diagnostic msg: Error generating preprocessed source(s) - no preprocessable inputs.
```
</details>


---

### Comment 2 - topperc

sponentry appears to only be supported for ARM and AArch64. https://llvm.org/docs/LangRef.html#id419

---

### Comment 3 - attila-lendvai

@topperc oh! thanks!

any insight on what else to use?

my current solution/workaround is the following:

```llvm
	%r15 = call ptr @llvm.frameaddress(i32 0)
	%r14 = bitcast %oop %r15 to [0 x %oop]*
	%r13 = getelementptr [0 x %oop], [0 x %oop]* %r14, %word 0, %word 1
	%r12 = bitcast %oop* %r13 to %oop
```


---

### Comment 4 - attila-lendvai

meanwhile i ported my code to aarch64, and there it's `llvm.frameaddress` that doesn't work. so, i need to dispatch on the target arch, and use `llvm.frameaddress` for x86_64, and `llvm.sponentry` for aarch64.

---

