# RegisterCoalescer: Assertion `isReg() && "This is not a register operand!"' failed.

**Author:** cbeuw
**URL:** https://github.com/llvm/llvm-project/issues/63033
**Status:** Closed
**Labels:** llvm:codegen, crash
**Closed Date:** 2025-03-23T15:42:22Z

## Body

With `llc -O2`:

```llvm
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define void @_ZN5repro4fn1617h3fe1cacc58d45f54E(ptr %_29.61573, i1 %0, i128 %1) {
start:
  br label %bb12

bb12:                                             ; preds = %bb41, %bb32, %bb17, %start
  %_15.2.ph = phi ptr [ null, %start ], [ null, %bb17 ], [ %_20.13, %bb41 ], [ null, %bb32 ]
  br i1 %0, label %bb23, label %bb17

bb17:                                             ; preds = %bb12
  store i8 0, ptr %_15.2.ph, align 1
  br label %bb12

bb23:                                             ; preds = %bb30, %bb12
  %_20.71455 = phi ptr [ %_29.61573, %bb12 ], [ null, %bb30 ]
  br label %bb34

bb34:                                       ; preds = %bb32, %bb23
  %_20.10 = phi ptr [ %_20.71455, %bb23 ], [ %_20.14, %bb32 ]
  br label %bb28

bb28:                                 ; preds = %bb35, %bb30, %bb34
  %_40.2 = phi ptr [ %_20.71455, %bb34 ], [ %_20.71455, %bb35 ], [ %_40.3, %bb30 ]
  %_20.12 = phi ptr [ %_20.10, %bb34 ], [ %_20.10, %bb35 ], [ %_20.14, %bb30 ]
  store ptr null, ptr %_40.2, align 8
  br label %bb29

bb29:                              ; preds = %bb38, %bb28
  %_40.3 = phi ptr [ %_40.2, %bb28 ], [ %_20.71455, %bb38 ]
  %_20.13 = phi ptr [ %_20.12, %bb28 ], [ %_20.10, %bb38 ]
  br label %bb30

bb30:                           ; preds = %bb41, %bb29
  %_20.14 = phi ptr [ %_20.13, %bb29 ], [ %_20.10, %bb41 ]
  switch i128 %1, label %bb23 [
    i128 1, label %bb28
    i128 0, label %bb32
  ]

bb32:                           ; preds = %bb30
  switch i1 false, label %bb12 [
    i1 true, label %bb34
    i1 false, label %bb35
  ]

bb35:                           ; preds = %bb32
  br i1 %0, label %bb38, label %bb28

bb38:                        ; preds = %bb35
  br i1 %0, label %bb41, label %bb29

bb41:                        ; preds = %bb38
  switch i8 0, label %bb12 [
    i8 0, label %bb30
  ]

; uselistorder directives
  uselistorder ptr %_20.14, { 1, 0 }
}
```

Backtrace:
```console
llc: /root/llvm-project/llvm/include/llvm/CodeGen/MachineOperand.h:370: llvm::Register llvm::MachineOperand::getReg() const: Assertion `isReg() && "This is not a register operand!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/llc -o /app/output.s -x86-asm-syntax=intel -O2 <source>
1.	Running pass 'Function Pass Manager' on module '<source>'.
2.	Running pass 'Simple Register Coalescing' on function '@_ZN5repro4fn1617h3fe1cacc58d45f54E'
 #0 0x000056486c672e9f llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3501e9f)
 #1 0x000056486c6705f4 SignalHandler(int) Signals.cpp:0:0
 #2 0x00007f0a33a42420 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x14420)
 #3 0x00007f0a3350f00b raise (/lib/x86_64-linux-gnu/libc.so.6+0x4300b)
 #4 0x00007f0a334ee859 abort (/lib/x86_64-linux-gnu/libc.so.6+0x22859)
 #5 0x00007f0a334ee729 (/lib/x86_64-linux-gnu/libc.so.6+0x22729)
 #6 0x00007f0a334fffd6 (/lib/x86_64-linux-gnu/libc.so.6+0x33fd6)
 #7 0x000056486bace540 isMoveInstr(llvm::TargetRegisterInfo const&, llvm::MachineInstr const*, llvm::Register&, llvm::Register&, unsigned int&, unsigned int&) (.constprop.0) RegisterCoalescer.cpp:0:0
 #8 0x000056486bad8b6a llvm::CoalescerPair::setRegisters(llvm::MachineInstr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x2967b6a)
 #9 0x000056486badde0e (anonymous namespace)::RegisterCoalescer::joinCopy(llvm::MachineInstr*, bool&) (.constprop.0) RegisterCoalescer.cpp:0:0
#10 0x000056486bae0d9f (anonymous namespace)::RegisterCoalescer::copyCoalesceWorkList(llvm::MutableArrayRef<llvm::MachineInstr*>) RegisterCoalescer.cpp:0:0
#11 0x000056486bae4c95 (anonymous namespace)::RegisterCoalescer::runOnMachineFunction(llvm::MachineFunction&) RegisterCoalescer.cpp:0:0
#12 0x000056486b8f0cee llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (.part.0) MachineFunctionPass.cpp:0:0
#13 0x000056486be7e1e1 llvm::FPPassManager::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x2d0d1e1)
#14 0x000056486be7e429 llvm::FPPassManager::runOnModule(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x2d0d429)
#15 0x000056486be7eca2 llvm::legacy::PassManagerImpl::run(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x2d0dca2)
#16 0x00005648699924f4 compileModule(char**, llvm::LLVMContext&) llc.cpp:0:0
#17 0x00005648698dac46 main (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x769c46)
#18 0x00007f0a334f0083 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x24083)
#19 0x000056486998881e _start (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x81781e)
```

On 17.0.0 trunk

https://godbolt.org/z/5f5o9qnWT

## Comments

### Comment 1 - cbeuw

There are empty phi nodes in the reproduction, but it still crashes even with https://github.com/llvm/llvm-project/commit/1379127481c3b26a6000d8d8f98e21fe7946e019, so it's likely a separate issue

---

### Comment 2 - Pierre-vh

I think some COPY is being added twice to the worklist.

e.g. removing this fixes it
```
  // CopyMI has been erased by joinIntervals at this point. Remove it from
  // ErasedInstrs since copyCoalesceWorkList() won't add a successful join back
  // to the work list. This keeps ErasedInstrs from growing needlessly.
  ErasedInstrs.erase(CopyMI);
```
So the assumption that an instruction cannot be re-added to the worklist is broken


---

### Comment 3 - qarmin

Looks that this no longer crashes with llvm 18.1.0

---

