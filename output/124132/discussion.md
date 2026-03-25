# [RISCV] Frame offsets outside of the signed 32-bit range not supported

**Author:** amcn
**URL:** https://github.com/llvm/llvm-project/issues/124132

## Body

Compiling the following admittedly contrived code:

```c
int main()
{
  void *ptr = __builtin_alloca(1ull << 31);
  return 0;
}
```

with clang target set to `riscv32` or `riscv64` produces the following error:

```
fatal error: error in backend: Frame offsets outside of the signed 32-bit range not supported
clang: error: clang frontend command failed with exit code 70 (use -v to see invocation)
Ubuntu clang version 19.1.1 (1ubuntu1)
Target: riscv32
Thread model: posix
InstalledDir: /usr/lib/llvm-19/bin
clang: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang: note: diagnostic msg: /tmp/b-8d596a.c
clang: note: diagnostic msg: /tmp/b-8d596a.sh
clang: note: diagnostic msg: 

********************
```

When emitting IR via `-S -emit-llvm` I get: 

```
; ModuleID = 'b.c'
source_filename = "b.c"
target datalayout = "e-m:e-p:32:32-i64:64-n32-S128"
target triple = "riscv32"

; Function Attrs: noinline nounwind optnone
define dso_local i32 @main() #0 {
  %1 = alloca i32, align 4
  %2 = alloca ptr, align 4
  store i32 0, ptr %1, align 4
  %3 = alloca i8, i32 -2147483648, align 16
  store ptr %3, ptr %2, align 4
  ret i32 0
}

attributes #0 = { noinline nounwind optnone "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="generic-rv32" "target-features"="+32bit,+a,+c,+m,+relax,+zmmul,-b,-d,-e,-experimental-smmpm,-experimental-smnpm,-experimental-ssnpm,-experimental-sspm,-experimental-ssqosid,-experimental-supm,-experimental-zacas,-experimental-zalasr,-experimental-zicfilp,-experimental-zicfiss,-f,-h,-shcounterenw,-shgatpa,-shtvala,-shvsatpa,-shvstvala,-shvstvecd,-smaia,-smcdeleg,-smcsrind,-smepmp,-smstateen,-ssaia,-ssccfg,-ssccptr,-sscofpmf,-sscounterenw,-sscsrind,-ssstateen,-ssstrict,-sstc,-sstvala,-sstvecd,-ssu64xl,-svade,-svadu,-svbare,-svinval,-svnapot,-svpbmt,-v,-xcvalu,-xcvbi,-xcvbitmanip,-xcvelw,-xcvmac,-xcvmem,-xcvsimd,-xsfcease,-xsfvcp,-xsfvfnrclipxfqf,-xsfvfwmaccqqq,-xsfvqmaccdod,-xsfvqmaccqoq,-xsifivecdiscarddlone,-xsifivecflushdlone,-xtheadba,-xtheadbb,-xtheadbs,-xtheadcmo,-xtheadcondmov,-xtheadfmemidx,-xtheadmac,-xtheadmemidx,-xtheadmempair,-xtheadsync,-xtheadvdot,-xventanacondops,-xwchc,-za128rs,-za64rs,-zaamo,-zabha,-zalrsc,-zama16b,-zawrs,-zba,-zbb,-zbc,-zbkb,-zbkc,-zbkx,-zbs,-zca,-zcb,-zcd,-zce,-zcf,-zcmop,-zcmp,-zcmt,-zdinx,-zfa,-zfbfmin,-zfh,-zfhmin,-zfinx,-zhinx,-zhinxmin,-zic64b,-zicbom,-zicbop,-zicboz,-ziccamoa,-ziccif,-zicclsm,-ziccrse,-zicntr,-zicond,-zicsr,-zifencei,-zihintntl,-zihintpause,-zihpm,-zimop,-zk,-zkn,-zknd,-zkne,-zknh,-zkr,-zks,-zksed,-zksh,-zkt,-ztso,-zvbb,-zvbc,-zve32f,-zve32x,-zve64d,-zve64f,-zve64x,-zvfbfmin,-zvfbfwma,-zvfh,-zvfhmin,-zvkb,-zvkg,-zvkn,-zvknc,-zvkned,-zvkng,-zvknha,-zvknhb,-zvks,-zvksc,-zvksed,-zvksg,-zvksh,-zvkt,-zvl1024b,-zvl128b,-zvl16384b,-zvl2048b,-zvl256b,-zvl32768b,-zvl32b,-zvl4096b,-zvl512b,-zvl64b,-zvl65536b,-zvl8192b" }

!llvm.module.flags = !{!0, !1, !2, !4, !5}
!llvm.ident = !{!6}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 1, !"target-abi", !"ilp32"}
!2 = !{i32 6, !"riscv-isa", !3}
!3 = !{!"rv32i2p1_m2p0_a2p1_c2p0_zmmul1p0"}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{i32 8, !"SmallDataLimit", i32 8}
!6 = !{!"Ubuntu clang version 19.1.1 (1ubuntu1)"}

```

which when fed to `llc` causes the following stack to be dumped:

```
LLVM ERROR: Frame offsets outside of the signed 32-bit range not supported
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.	Program arguments: llc b.ll
1.	Running pass 'Function Pass Manager' on module 'b.ll'.
2.	Running pass 'Prologue/Epilogue Insertion & Frame Finalization' on function '@main'
 #0 0x00007ea00bead12f llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) build-llvm/tools/clang/stage2-bins/llvm/lib/Support/Unix/Signals.inc:723:13
 #1 0x00007ea00beaae39 llvm::sys::RunSignalHandlers() build-llvm/tools/clang/stage2-bins/llvm/lib/Support/Signals.cpp:106:18
 #2 0x00007ea00bead880 SignalHandler build-llvm/tools/clang/stage2-bins/llvm/lib/Support/Unix/Signals.inc:413:1
 #3 0x00007ea00a845250 (/lib/x86_64-linux-gnu/libc.so.6+0x45250)
 #4 0x00007ea00a8a3f1c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x00007ea00a8a3f1c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x00007ea00a8a3f1c pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x00007ea00a84519e raise ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x00007ea00a828902 abort ./stdlib/abort.c:81:7
 #9 0x00007ea00be02fd4 llvm::report_fatal_error(llvm::Twine const&, bool) build-llvm/tools/clang/stage2-bins/llvm/lib/Support/ErrorHandling.cpp:125:5
#10 0x00007ea00be02e0d (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.1+0xe02e0d)
#11 0x00007ea00eb7101c eliminateFrameIndex build-llvm/tools/clang/stage2-bins/llvm/lib/Target/RISCV/RISCVRegisterInfo.cpp:0:0
#12 0x00007ea00c3cddf6 replaceFrameIndicesBackward build-llvm/tools/clang/stage2-bins/llvm/lib/CodeGen/PrologEpilogInserter.cpp:0:0
#13 0x00007ea00c3cddf6 replaceFrameIndicesBackward build-llvm/tools/clang/stage2-bins/llvm/lib/CodeGen/PrologEpilogInserter.cpp:1357:5
#14 0x00007ea00c3cddf6 runOnMachineFunction build-llvm/tools/clang/stage2-bins/llvm/lib/CodeGen/PrologEpilogInserter.cpp:280:7
#15 0x00007ea00c2a7dc9 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) build-llvm/tools/clang/stage2-bins/llvm/lib/CodeGen/MachineFunctionPass.cpp:94:13
#16 0x00007ea00c005e12 llvm::FPPassManager::runOnFunction(llvm::Function&) build-llvm/tools/clang/stage2-bins/llvm/lib/IR/LegacyPassManager.cpp:1440:27
#17 0x00007ea00c00ba84 llvm::FPPassManager::runOnModule(llvm::Module&) build-llvm/tools/clang/stage2-bins/llvm/lib/IR/LegacyPassManager.cpp:1486:13
#18 0x00007ea00c0065bf runOnModule build-llvm/tools/clang/stage2-bins/llvm/lib/IR/LegacyPassManager.cpp:0:27
#19 0x00007ea00c0065bf llvm::legacy::PassManagerImpl::run(llvm::Module&) build-llvm/tools/clang/stage2-bins/llvm/lib/IR/LegacyPassManager.cpp:541:44
#20 0x000055fa4a88cb82 compileModule build-llvm/tools/clang/stage2-bins/llvm/tools/llc/llc.cpp:0:8
#21 0x000055fa4a88a67f main build-llvm/tools/clang/stage2-bins/llvm/tools/llc/llc.cpp:409:13
#22 0x00007ea00a82a3b8 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#23 0x00007ea00a82a47b call_init ./csu/../csu/libc-start.c:128:20
#24 0x00007ea00a82a47b __libc_start_main ./csu/../csu/libc-start.c:347:5
#25 0x000055fa4a886e45 _start (/usr/lib/llvm-19/bin/llc+0x11e45)
Aborted (core dumped)
```

I've reproduced this against main HEAD and Ubuntu 24.10's clang package.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-risc-v

Author: Andrew McNulty (amcn)

<details>
Compiling the following admittedly contrived code:

```c
int main()
{
  void *ptr = __builtin_alloca(1ull &lt;&lt; 31);
  return 0;
}
```

with clang target set to `riscv32` or `riscv64` produces the following error:

```
fatal error: error in backend: Frame offsets outside of the signed 32-bit range not supported
clang: error: clang frontend command failed with exit code 70 (use -v to see invocation)
Ubuntu clang version 19.1.1 (1ubuntu1)
Target: riscv32
Thread model: posix
InstalledDir: /usr/lib/llvm-19/bin
clang: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang: note: diagnostic msg: /tmp/b-8d596a.c
clang: note: diagnostic msg: /tmp/b-8d596a.sh
clang: note: diagnostic msg: 

********************
```

When emitting IR via `-S -emit-llvm` I get: 

```
; ModuleID = 'b.c'
source_filename = "b.c"
target datalayout = "e-m:e-p:32:32-i64:64-n32-S128"
target triple = "riscv32"

; Function Attrs: noinline nounwind optnone
define dso_local i32 @<!-- -->main() #<!-- -->0 {
  %1 = alloca i32, align 4
  %2 = alloca ptr, align 4
  store i32 0, ptr %1, align 4
  %3 = alloca i8, i32 -2147483648, align 16
  store ptr %3, ptr %2, align 4
  ret i32 0
}

attributes #<!-- -->0 = { noinline nounwind optnone "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="generic-rv32" "target-features"="+32bit,+a,+c,+m,+relax,+zmmul,-b,-d,-e,-experimental-smmpm,-experimental-smnpm,-experimental-ssnpm,-experimental-sspm,-experimental-ssqosid,-experimental-supm,-experimental-zacas,-experimental-zalasr,-experimental-zicfilp,-experimental-zicfiss,-f,-h,-shcounterenw,-shgatpa,-shtvala,-shvsatpa,-shvstvala,-shvstvecd,-smaia,-smcdeleg,-smcsrind,-smepmp,-smstateen,-ssaia,-ssccfg,-ssccptr,-sscofpmf,-sscounterenw,-sscsrind,-ssstateen,-ssstrict,-sstc,-sstvala,-sstvecd,-ssu64xl,-svade,-svadu,-svbare,-svinval,-svnapot,-svpbmt,-v,-xcvalu,-xcvbi,-xcvbitmanip,-xcvelw,-xcvmac,-xcvmem,-xcvsimd,-xsfcease,-xsfvcp,-xsfvfnrclipxfqf,-xsfvfwmaccqqq,-xsfvqmaccdod,-xsfvqmaccqoq,-xsifivecdiscarddlone,-xsifivecflushdlone,-xtheadba,-xtheadbb,-xtheadbs,-xtheadcmo,-xtheadcondmov,-xtheadfmemidx,-xtheadmac,-xtheadmemidx,-xtheadmempair,-xtheadsync,-xtheadvdot,-xventanacondops,-xwchc,-za128rs,-za64rs,-zaamo,-zabha,-zalrsc,-zama16b,-zawrs,-zba,-zbb,-zbc,-zbkb,-zbkc,-zbkx,-zbs,-zca,-zcb,-zcd,-zce,-zcf,-zcmop,-zcmp,-zcmt,-zdinx,-zfa,-zfbfmin,-zfh,-zfhmin,-zfinx,-zhinx,-zhinxmin,-zic64b,-zicbom,-zicbop,-zicboz,-ziccamoa,-ziccif,-zicclsm,-ziccrse,-zicntr,-zicond,-zicsr,-zifencei,-zihintntl,-zihintpause,-zihpm,-zimop,-zk,-zkn,-zknd,-zkne,-zknh,-zkr,-zks,-zksed,-zksh,-zkt,-ztso,-zvbb,-zvbc,-zve32f,-zve32x,-zve64d,-zve64f,-zve64x,-zvfbfmin,-zvfbfwma,-zvfh,-zvfhmin,-zvkb,-zvkg,-zvkn,-zvknc,-zvkned,-zvkng,-zvknha,-zvknhb,-zvks,-zvksc,-zvksed,-zvksg,-zvksh,-zvkt,-zvl1024b,-zvl128b,-zvl16384b,-zvl2048b,-zvl256b,-zvl32768b,-zvl32b,-zvl4096b,-zvl512b,-zvl64b,-zvl65536b,-zvl8192b" }

!llvm.module.flags = !{!0, !1, !2, !4, !5}
!llvm.ident = !{!6}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 1, !"target-abi", !"ilp32"}
!2 = !{i32 6, !"riscv-isa", !3}
!3 = !{!"rv32i2p1_m2p0_a2p1_c2p0_zmmul1p0"}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{i32 8, !"SmallDataLimit", i32 8}
!6 = !{!"Ubuntu clang version 19.1.1 (1ubuntu1)"}

```

which when fed to `llc` causes the following stack to be dumped:

```
LLVM ERROR: Frame offsets outside of the signed 32-bit range not supported
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.	Program arguments: llc b.ll
1.	Running pass 'Function Pass Manager' on module 'b.ll'.
2.	Running pass 'Prologue/Epilogue Insertion &amp; Frame Finalization' on function '@<!-- -->main'
 #<!-- -->0 0x00007ea00bead12f llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) build-llvm/tools/clang/stage2-bins/llvm/lib/Support/Unix/Signals.inc:723:13
 #<!-- -->1 0x00007ea00beaae39 llvm::sys::RunSignalHandlers() build-llvm/tools/clang/stage2-bins/llvm/lib/Support/Signals.cpp:106:18
 #<!-- -->2 0x00007ea00bead880 SignalHandler build-llvm/tools/clang/stage2-bins/llvm/lib/Support/Unix/Signals.inc:413:1
 #<!-- -->3 0x00007ea00a845250 (/lib/x86_64-linux-gnu/libc.so.6+0x45250)
 #<!-- -->4 0x00007ea00a8a3f1c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->5 0x00007ea00a8a3f1c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->6 0x00007ea00a8a3f1c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->7 0x00007ea00a84519e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->8 0x00007ea00a828902 abort ./stdlib/abort.c:81:7
 #<!-- -->9 0x00007ea00be02fd4 llvm::report_fatal_error(llvm::Twine const&amp;, bool) build-llvm/tools/clang/stage2-bins/llvm/lib/Support/ErrorHandling.cpp:125:5
#<!-- -->10 0x00007ea00be02e0d (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.1+0xe02e0d)
#<!-- -->11 0x00007ea00eb7101c eliminateFrameIndex build-llvm/tools/clang/stage2-bins/llvm/lib/Target/RISCV/RISCVRegisterInfo.cpp:0:0
#<!-- -->12 0x00007ea00c3cddf6 replaceFrameIndicesBackward build-llvm/tools/clang/stage2-bins/llvm/lib/CodeGen/PrologEpilogInserter.cpp:0:0
#<!-- -->13 0x00007ea00c3cddf6 replaceFrameIndicesBackward build-llvm/tools/clang/stage2-bins/llvm/lib/CodeGen/PrologEpilogInserter.cpp:1357:5
#<!-- -->14 0x00007ea00c3cddf6 runOnMachineFunction build-llvm/tools/clang/stage2-bins/llvm/lib/CodeGen/PrologEpilogInserter.cpp:280:7
#<!-- -->15 0x00007ea00c2a7dc9 llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) build-llvm/tools/clang/stage2-bins/llvm/lib/CodeGen/MachineFunctionPass.cpp:94:13
#<!-- -->16 0x00007ea00c005e12 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) build-llvm/tools/clang/stage2-bins/llvm/lib/IR/LegacyPassManager.cpp:1440:27
#<!-- -->17 0x00007ea00c00ba84 llvm::FPPassManager::runOnModule(llvm::Module&amp;) build-llvm/tools/clang/stage2-bins/llvm/lib/IR/LegacyPassManager.cpp:1486:13
#<!-- -->18 0x00007ea00c0065bf runOnModule build-llvm/tools/clang/stage2-bins/llvm/lib/IR/LegacyPassManager.cpp:0:27
#<!-- -->19 0x00007ea00c0065bf llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) build-llvm/tools/clang/stage2-bins/llvm/lib/IR/LegacyPassManager.cpp:541:44
#<!-- -->20 0x000055fa4a88cb82 compileModule build-llvm/tools/clang/stage2-bins/llvm/tools/llc/llc.cpp:0:8
#<!-- -->21 0x000055fa4a88a67f main build-llvm/tools/clang/stage2-bins/llvm/tools/llc/llc.cpp:409:13
#<!-- -->22 0x00007ea00a82a3b8 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->23 0x00007ea00a82a47b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->24 0x00007ea00a82a47b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->25 0x000055fa4a886e45 _start (/usr/lib/llvm-19/bin/llc+0x11e45)
Aborted (core dumped)
```

I've reproduced this against main HEAD and Ubuntu 24.10's clang package.
</details>


---

### Comment 2 - amcn

Oops, I forgot about this. I'm going through some old issues and I thought I would add some context to explain how I ended up hitting this:

The N25F IP from Andes has hardware support for stack bounds tracking and I am in the process of validating this IP in the context of a larger SOC. I needed some code to provoke a large modification to the stack pointer register which would, when hardware stack bounds tracking was activated, provoke the Andes IP to trap and raise the correct error code. The first tool I reached for was `__builtin_alloca`, and the first value I gave it, something large enough to provoke the desired stack pointer modification, was `1ull << 31`. 

As I mentioned initially, it was a very contrived set of conditions. It was also not a blocker at all: the chosen solution was to use inline asm to put precisely the right instructions in place, which was sufficient for validating that the Andes feature works as expected. The only reason I thought it worth raising here is that under gcc this code compiles to the correct asm, so I figured it must relate to LLVM's current implementation.

---

### Comment 3 - fahaichen

<img width="1144" height="549" alt="Image" src="https://github.com/user-attachments/assets/7dceb9db-88a0-4ace-a7b9-01fd83d362e5" />

<img width="1895" height="293" alt="Image" src="https://github.com/user-attachments/assets/b29bae84-6d9c-4851-a0d1-8b860499b5ac" />

I found that this issue was produced firstly in llvm 13.0.0，but in llvm 10/11/12 version is okay. as shown in screenshots.

---

### Comment 4 - lenary

> `LLVM ERROR: Frame offsets outside of the signed 32-bit range not supported`

`%3 = alloca i8, i32 -2147483648, align 16`

This alloca is out of signed 32-bit range.

I'm going to open a PR so you don't get a stack trace (instead, you will just get the error message), but the backend is not going to support a stack allocation so large without quite a bit of work (stack handling code as so many edge cases). Especially on rv32, this work does not seem fruitful to me, but others may disagree.

I will not close this issue, as I realise my change will not end up supporting the requested usecase. I plan to rename this issue to make the specific crash/error clearer.

---

### Comment 5 - amcn

Closing this issue is fine by me.

When I first hit this issue back in January I did take a look at what it would take to fix this and upon discovering the complexity of the frame lowering code I realised a fix in the backend would be best left to the experts.

After that I considered that detecting this case and raising an error in the frontend would be a good idea  but I never got around to implementing it. I believe I opened this issue right after that idea came to me and then I moved onto other topics. I could prototype that if you like.

---

