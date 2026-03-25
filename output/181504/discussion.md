# [AVR] Assertion "integer overflow in comparison transform" gets triggered

**Author:** Patryk27
**URL:** https://github.com/llvm/llvm-project/issues/181504
**Status:** Closed
**Labels:** backend:AVR, crash
**Closed Date:** 2026-02-23T18:30:06Z

## Body

This:

```ll
define fastcc fp128 @fn(i128 %0) {
start:
  br label %bb106.i

bb129.i:                                          
  ret fp128 0xL00000000000000000000000000000000

bb106.i:                                         
  %_0.i28.i = icmp ult i128 %0, 41538374868278621028243970633760768
  br i1 %_0.i28.i, label %bb129.thread.i, label %bb129.i

bb129.thread.i:                                 
  ret fp128 0xL00000000000000000000000000000000
}
```

... crashes when compiled for AVR:

```
$ llc -mtriple=avr reduced.ll

llc: /home/pwy/x/llvm-project/llvm/lib/Target/AVR/AVRISelLowering.cpp:722: SDValue llvm::AVRTargetLowering::getAVRCmp(SDValue, SDValue, ISD::CondCode, SDValue &, SelectionDAG &, SDLoc) const: Assertion `(!C->isAllOnes()) && "integer overflow in comparison transform"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: /home/pwy/x/llvm-project/build/bin/llc -mtriple=avr reduced.ll
1.      Running pass 'Function Pass Manager' on module 'reduced.ll'.
2.      Running pass 'AVR DAG->DAG Instruction Selection' on function '@fn'
 #0 0x0000556133c6a3bb llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/home/pwy/x/llvm-project/build/bin/llc+0x203c3bb)
 #1 0x0000556133c67bdb llvm::sys::RunSignalHandlers() (/home/pwy/x/llvm-project/build/bin/llc+0x2039bdb)
 #2 0x0000556133c6b177 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x00007f2c93f00790 __restore_rt (/nix/store/wb6rhpznjfczwlwx23zmdrrw74bayxw4-glibc-2.42-47/lib/libc.so.6+0x42790)
 #4 0x00007f2c93f5ddcc __pthread_kill_implementation (/nix/store/wb6rhpznjfczwlwx23zmdrrw74bayxw4-glibc-2.42-47/lib/libc.so.6+0x9fdcc)
 #5 0x00007f2c93f0065e gsignal (/nix/store/wb6rhpznjfczwlwx23zmdrrw74bayxw4-glibc-2.42-47/lib/libc.so.6+0x4265e)
 #6 0x00007f2c93ee7350 abort (/nix/store/wb6rhpznjfczwlwx23zmdrrw74bayxw4-glibc-2.42-47/lib/libc.so.6+0x29350)
 #7 0x00007f2c93ee72b9 __assert_perror_fail (/nix/store/wb6rhpznjfczwlwx23zmdrrw74bayxw4-glibc-2.42-47/lib/libc.so.6+0x292b9)
 #8 0x0000556132217626 llvm::AVRTargetLowering::getAVRCmp(llvm::SDValue, llvm::SDValue, llvm::ISD::CondCode, llvm::SDValue&, llvm::SelectionDAG&, llvm::SDLoc) const (/home/pwy/x/llvm-project/build/bin/llc+0x5e9626)
 #9 0x0000556132217c79 llvm::AVRTargetLowering::LowerSETCC(llvm::SDValue, llvm::SelectionDAG&) const (/home/pwy/x/llvm-project/build/bin/llc+0x5e9c79)
#10 0x00005561339bfdef llvm::TargetLowering::LowerOperationWrapper(llvm::SDNode*, llvm::SmallVectorImpl<llvm::SDValue>&, llvm::SelectionDAG&) const (/home/pwy/x/llvm-project/build/bin/llc+0x1d91def)
#11 0x0000556133acab71 llvm::DAGTypeLegalizer::CustomLowerNode(llvm::SDNode*, llvm::EVT, bool) (/home/pwy/x/llvm-project/build/bin/llc+0x1e9cb71)
#12 0x0000556133b70feb llvm::DAGTypeLegalizer::ExpandIntegerOperand(llvm::SDNode*, unsigned int) (/home/pwy/x/llvm-project/build/bin/llc+0x1f42feb)
#13 0x0000556133ac6dba llvm::DAGTypeLegalizer::run() (/home/pwy/x/llvm-project/build/bin/llc+0x1e98dba)
#14 0x0000556133acbf40 llvm::SelectionDAG::LegalizeTypes() (/home/pwy/x/llvm-project/build/bin/llc+0x1e9df40)
#15 0x0000556133a42d41 llvm::SelectionDAGISel::CodeGenAndEmitDAG() (/home/pwy/x/llvm-project/build/bin/llc+0x1e14d41)
#16 0x0000556133a41b46 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&) (/home/pwy/x/llvm-project/build/bin/llc+0x1e13b46)
#17 0x0000556133a3eb49 llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&) (/home/pwy/x/llvm-project/build/bin/llc+0x1e10b49)
#18 0x0000556133a3c245 llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&) (/home/pwy/x/llvm-project/build/bin/llc+0x1e0e245)
#19 0x0000556132d0fe63 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (/home/pwy/x/llvm-project/build/bin/llc+0x10e1e63)
#20 0x00005561332b8aa4 llvm::FPPassManager::runOnFunction(llvm::Function&) (/home/pwy/x/llvm-project/build/bin/llc+0x168aaa4)
#21 0x00005561332c0934 llvm::FPPassManager::runOnModule(llvm::Module&) (/home/pwy/x/llvm-project/build/bin/llc+0x1692934)
#22 0x00005561332b9500 llvm::legacy::PassManagerImpl::run(llvm::Module&) (/home/pwy/x/llvm-project/build/bin/llc+0x168b500)
#23 0x0000556132197644 compileModule(char**, llvm::SmallVectorImpl<llvm::PassPlugin>&, llvm::LLVMContext&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>&) llc.cpp:0:0
#24 0x0000556132194e42 main (/home/pwy/x/llvm-project/build/bin/llc+0x566e42)
#25 0x00007f2c93ee9285 __libc_start_call_main (/nix/store/wb6rhpznjfczwlwx23zmdrrw74bayxw4-glibc-2.42-47/lib/libc.so.6+0x2b285)
#26 0x00007f2c93ee9338 __libc_start_main@GLIBC_2.2.5 (/nix/store/wb6rhpznjfczwlwx23zmdrrw74bayxw4-glibc-2.42-47/lib/libc.so.6+0x2b338)
#27 0x0000556132190865 _start (/home/pwy/x/llvm-project/build/bin/llc+0x562865)
aborted (core dumped)
```

Found accidentally when working on https://github.com/llvm/llvm-project/issues/169190. I'm on my way preparing a fix for this, because it's low-key annoying to work on that other bug otherwise.

## Comments

### Comment 1 - Patryk27

Minimized further:
```ll
define i1 @fn(i128 %0) {
bb0:                                         
  %1 = icmp ugt i128 %0, 18446744073709551615 ; 2^64-1

  ret i1 %1
}
```

---

