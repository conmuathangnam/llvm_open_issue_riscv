# AArch64 backend crashing on trivial but non-canonical code with Assertion failed: (isVirtual() && "Not a virtual register")

**Author:** regehr
**URL:** https://github.com/llvm/llvm-project/issues/151888
**Status:** Closed
**Labels:** llvm:codegen, crash-on-valid
**Closed Date:** 2025-08-04T12:37:20Z

## Body

seeing this on `clang -O0` output

https://godbolt.org/z/zc5b5bhWE
```
~/alive2-regehr/build$ cat r1.ll 
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i8:8:32-i16:16:32-i64:64-i128:128-n32:64-S128-Fn32"
target triple = "aarch64-unknown-linux-gnu"

; Function Attrs: noinline optnone
define i64 @tf_0_foo() #0 {
entry:
  %conv203 = zext i32 0 to i64
  ret i64 %conv203
}

attributes #0 = { noinline optnone }
~/alive2-regehr/build$ ~/llvm-project/for-alive/bin/llc r1.ll
Assertion failed: (isVirtual() && "Not a virtual register"), function virtRegIndex, file Register.h, line 83.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.	Program arguments: /Users/regehr/llvm-project/for-alive/bin/llc r1.ll
1.	Running pass 'Function Pass Manager' on module 'r1.ll'.
2.	Running pass 'Register Coalescer' on function '@tf_0_foo'
 #0 0x000000010bac68f4 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/Users/regehr/llvm-project/for-alive/lib/libLLVMSupport.dylib+0x1568f4)
 #1 0x000000010bac3f44 llvm::sys::RunSignalHandlers() (/Users/regehr/llvm-project/for-alive/lib/libLLVMSupport.dylib+0x153f44)
 #2 0x000000010bac73ec SignalHandler(int, __siginfo*, void*) (/Users/regehr/llvm-project/for-alive/lib/libLLVMSupport.dylib+0x1573ec)
 #3 0x000000019ad78624 (/usr/lib/system/libsystem_platform.dylib+0x1804ac624)
 #4 0x000000019ad3e88c (/usr/lib/system/libsystem_pthread.dylib+0x18047288c)
 #5 0x000000019ac47c60 (/usr/lib/system/libsystem_c.dylib+0x18037bc60)
 #6 0x000000019ac46eec (/usr/lib/system/libsystem_c.dylib+0x18037aeec)
 #7 0x000000010c4a5808 (anonymous namespace)::RegisterCoalescer::joinCopy(llvm::MachineInstr*, bool&, llvm::SmallPtrSetImpl<llvm::MachineInstr*>&) (/Users/regehr/llvm-project/for-alive/lib/libLLVMCodeGen.dylib+0x46d808)
 #8 0x000000010c4a1494 (anonymous namespace)::RegisterCoalescer::copyCoalesceWorkList(llvm::MutableArrayRef<llvm::MachineInstr*>) (/Users/regehr/llvm-project/for-alive/lib/libLLVMCodeGen.dylib+0x469494)
 #9 0x000000010c49d1c8 (anonymous namespace)::RegisterCoalescer::run(llvm::MachineFunction&) (/Users/regehr/llvm-project/for-alive/lib/libLLVMCodeGen.dylib+0x4651c8)
#10 0x000000010c49ebe4 (anonymous namespace)::RegisterCoalescerLegacy::runOnMachineFunction(llvm::MachineFunction&) (/Users/regehr/llvm-project/for-alive/lib/libLLVMCodeGen.dylib+0x466be4)
#11 0x000000010c295cf0 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (/Users/regehr/llvm-project/for-alive/lib/libLLVMCodeGen.dylib+0x25dcf0)
#12 0x000000010ce386b8 llvm::FPPassManager::runOnFunction(llvm::Function&) (/Users/regehr/llvm-project/for-alive/lib/libLLVMCore.dylib+0x1bc6b8)
#13 0x000000010ce401f0 llvm::FPPassManager::runOnModule(llvm::Module&) (/Users/regehr/llvm-project/for-alive/lib/libLLVMCore.dylib+0x1c41f0)
#14 0x000000010ce38fec llvm::legacy::PassManagerImpl::run(llvm::Module&) (/Users/regehr/llvm-project/for-alive/lib/libLLVMCore.dylib+0x1bcfec)
#15 0x000000010229cbbc compileModule(char**, llvm::LLVMContext&) (/Users/regehr/llvm-project/for-alive/bin/llc+0x10001cbbc)
#16 0x000000010229ac08 main (/Users/regehr/llvm-project/for-alive/bin/llc+0x10001ac08)
#17 0x000000019a99eb98
zsh: abort      ~/llvm-project/for-alive/bin/llc r1.ll
~/alive2-regehr/build$ 
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-aarch64

Author: John Regehr (regehr)

<details>
seeing this on `clang -O0` output

```
~/alive2-regehr/build$ cat r1.ll 
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i8:8:32-i16:16:32-i64:64-i128:128-n32:64-S128-Fn32"
target triple = "aarch64-unknown-linux-gnu"

; Function Attrs: noinline optnone
define i64 @<!-- -->tf_0_foo() #<!-- -->0 {
entry:
  %conv203 = zext i32 0 to i64
  ret i64 %conv203
}

attributes #<!-- -->0 = { noinline optnone }
~/alive2-regehr/build$ ~/llvm-project/for-alive/bin/llc r1.ll
Assertion failed: (isVirtual() &amp;&amp; "Not a virtual register"), function virtRegIndex, file Register.h, line 83.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.	Program arguments: /Users/regehr/llvm-project/for-alive/bin/llc r1.ll
1.	Running pass 'Function Pass Manager' on module 'r1.ll'.
2.	Running pass 'Register Coalescer' on function '@<!-- -->tf_0_foo'
 #<!-- -->0 0x000000010bac68f4 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/Users/regehr/llvm-project/for-alive/lib/libLLVMSupport.dylib+0x1568f4)
 #<!-- -->1 0x000000010bac3f44 llvm::sys::RunSignalHandlers() (/Users/regehr/llvm-project/for-alive/lib/libLLVMSupport.dylib+0x153f44)
 #<!-- -->2 0x000000010bac73ec SignalHandler(int, __siginfo*, void*) (/Users/regehr/llvm-project/for-alive/lib/libLLVMSupport.dylib+0x1573ec)
 #<!-- -->3 0x000000019ad78624 (/usr/lib/system/libsystem_platform.dylib+0x1804ac624)
 #<!-- -->4 0x000000019ad3e88c (/usr/lib/system/libsystem_pthread.dylib+0x18047288c)
 #<!-- -->5 0x000000019ac47c60 (/usr/lib/system/libsystem_c.dylib+0x18037bc60)
 #<!-- -->6 0x000000019ac46eec (/usr/lib/system/libsystem_c.dylib+0x18037aeec)
 #<!-- -->7 0x000000010c4a5808 (anonymous namespace)::RegisterCoalescer::joinCopy(llvm::MachineInstr*, bool&amp;, llvm::SmallPtrSetImpl&lt;llvm::MachineInstr*&gt;&amp;) (/Users/regehr/llvm-project/for-alive/lib/libLLVMCodeGen.dylib+0x46d808)
 #<!-- -->8 0x000000010c4a1494 (anonymous namespace)::RegisterCoalescer::copyCoalesceWorkList(llvm::MutableArrayRef&lt;llvm::MachineInstr*&gt;) (/Users/regehr/llvm-project/for-alive/lib/libLLVMCodeGen.dylib+0x469494)
 #<!-- -->9 0x000000010c49d1c8 (anonymous namespace)::RegisterCoalescer::run(llvm::MachineFunction&amp;) (/Users/regehr/llvm-project/for-alive/lib/libLLVMCodeGen.dylib+0x4651c8)
#<!-- -->10 0x000000010c49ebe4 (anonymous namespace)::RegisterCoalescerLegacy::runOnMachineFunction(llvm::MachineFunction&amp;) (/Users/regehr/llvm-project/for-alive/lib/libLLVMCodeGen.dylib+0x466be4)
#<!-- -->11 0x000000010c295cf0 llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (/Users/regehr/llvm-project/for-alive/lib/libLLVMCodeGen.dylib+0x25dcf0)
#<!-- -->12 0x000000010ce386b8 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (/Users/regehr/llvm-project/for-alive/lib/libLLVMCore.dylib+0x1bc6b8)
#<!-- -->13 0x000000010ce401f0 llvm::FPPassManager::runOnModule(llvm::Module&amp;) (/Users/regehr/llvm-project/for-alive/lib/libLLVMCore.dylib+0x1c41f0)
#<!-- -->14 0x000000010ce38fec llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/Users/regehr/llvm-project/for-alive/lib/libLLVMCore.dylib+0x1bcfec)
#<!-- -->15 0x000000010229cbbc compileModule(char**, llvm::LLVMContext&amp;) (/Users/regehr/llvm-project/for-alive/bin/llc+0x10001cbbc)
#<!-- -->16 0x000000010229ac08 main (/Users/regehr/llvm-project/for-alive/bin/llc+0x10001ac08)
#<!-- -->17 0x000000019a99eb98
zsh: abort      ~/llvm-project/for-alive/bin/llc r1.ll
~/alive2-regehr/build$ 
```
</details>


---

### Comment 2 - davemgreen

FastISel is getting involved because it is a optnone function ran with -O2 in llc. The same thing seems to happen with clang under -O2, which is something we should look into addressing now that gisel is usually used at -O0.

The IR fast-isel produces looks like
```
bb.0.entry:                                           
  %1:gpr32 = COPY killed $wzr                         
  %2:gpr64 = SUBREG_TO_REG 0, %1:gpr32, %subreg.sub_32
  %3:gpr64 = UBFMXri %2:gpr64, 0, 31                  
  $x0 = COPY %3:gpr64                                 
  RET_ReallyLR implicit $x0                           
```

The assert started with #134408. FYI @sdesmalen-arm. The above code doesn't look incorrect to me, maybe it just needs an extra !CP.isPhys() check and it's hopefully a relatively simple one to fix.

---

### Comment 3 - sdesmalen-arm

This case is simple to fix, but I'm reverting #134408 for the time being while I investigate some of the other failures. I'll add this one as a test-case to the patch!

---

### Comment 4 - davemgreen

Thank you.

---

