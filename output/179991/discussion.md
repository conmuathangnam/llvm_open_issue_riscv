# [AVR] Failed to select `ISD::FRAMEADDR` and `ISD::RETURNADDR`

**Author:** sourcery-zone
**URL:** https://github.com/llvm/llvm-project/issues/179991
**Status:** Open
**Labels:** backend:AVR, crash

## Body

## Description

The AVR backend crashes with a segmentation fault when compiling a function in `addrspace(1)` calls `@llvm.returnaddress(i32 0)`. I'm not knowledgeable about LLVM, but given the situation I experienced from my source program in Zig (details reported on [Zig issue tracker](https://codeberg.org/ziglang/zig/issues/31127)), I guess data type of other numeric values suffer the same issue. 

## Minimal Reproduction

`reduced.ll`:
```llvm
; ModuleID = 'reduced.bc'
target datalayout = "e-P1-p:16:8-i8:8-i16:8-i32:8-i64:8-f32:8-f64:8-n8-a:8"
target triple = "avr-unknown-unknown-unknown"

define fastcc ptr @"debug.FullPanic((function 'defaultPanic')).integerOutOfBounds"() addrspace(1) {
Entry:
  %0 = call addrspace(0) ptr @llvm.returnaddress(i32 0)
  ret ptr %0
}

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(none)
declare ptr @llvm.returnaddress(i32 immarg) addrspace(0) #0

attributes #0 = { nocallback nofree nosync nounwind willreturn memory(none) }
```

Command:
```sh
llc reduced.ll -mtriple=avr -mcpu=atmega2560 -o test.s
```

## Actual Behavior
```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.	Program arguments: llc reduced.ll -mtriple=avr -mcpu=atmega2560 -o test.s
1.	Running pass 'Function Pass Manager' on module 'reduced.ll'.
2.	Running pass 'AVR Assembly Printer' on function '@"debug.FullPanic((function 'defaultPanic')).integerOutOfBounds"'
 #0 0x00007fd1e2341b2b llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/nix/store/swch85kls1srlrji1i0g28wpa4y607hk-llvm-21.1.2-lib/lib/libLLVM.so.21.1+0xd41b2b)
 #1 0x00007fd1e233e9ba SignalHandler(int, siginfo_t*, void*) (/nix/store/swch85kls1srlrji1i0g28wpa4y607hk-llvm-21.1.2-lib/lib/libLLVM.so.21.1+0xd3e9ba)
 #2 0x00007fd1e0e419c0 __restore_rt (/nix/store/xx7cm72qy2c0643cm1ipngd87aqwkcdp-glibc-2.40-66/lib/libc.so.6+0x419c0)
 #3 0x00007fd1e3167edb llvm::AsmPrinter::GetBlockAddressSymbol(llvm::BlockAddress const*) const (/nix/store/swch85kls1srlrji1i0g28wpa4y607hk-llvm-21.1.2-lib/lib/libLLVM.so.21.1+0x1b67edb)
 #4 0x00007fd1e61b8b84 llvm::AVRMCInstLower::lowerInstruction(llvm::MachineInstr const&, llvm::MCInst&) const (/nix/store/swch85kls1srlrji1i0g28wpa4y607hk-llvm-21.1.2-lib/lib/libLLVM.so.21.1+0x4bb8b84)
 #5 0x00007fd1e6190e5c (anonymous namespace)::AVRAsmPrinter::emitInstruction(llvm::MachineInstr const*) (/nix/store/swch85kls1srlrji1i0g28wpa4y607hk-llvm-21.1.2-lib/lib/libLLVM.so.21.1+0x4b90e5c)
...
```

## Expected Behavior

Successful compilation to AVR assembly. 

- LLVM version: 21.1.2 (also reproduces on LLVM 20.1)
- OS: Linux

## Extra Context

This I faced with when compiling my Zig code targeting AVR in Debug mode. Full information from Zig point of view is listed [here](https://codeberg.org/ziglang/zig/issues/31127). The short term workaround to avoid this issue, was to use optimization for release, or use wrapped arithmetic operations. 

## Comments

### Comment 1 - benshi001

The root cause is `ISD::FRAMEADDR` and `ISD::RETURNADDR` are required to be `Custom` than `Expnad` or `Legal`. While AVR backend has not implemented `AVRTargetLowering::LowerRETURNADDR` and `AVRTargetLowering::LowerFRAMEADDR` as other backends have done.

I will implement them later this month.

---

### Comment 2 - benshi001

The full crash log is 

```
Don't know how to custom lower this!
UNREACHABLE executed at /data/home/bennshi/llvm-project/llvm/lib/Target/AVR/AVRISelLowering.cpp:931!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: ./bin/llc a.ll
1.	Running pass 'Function Pass Manager' on module 'a.ll'.
2.	Running pass 'AVR DAG->DAG Instruction Selection' on function '@"debug.FullPanic((function 'defaultPanic')).integerOutOfBounds"'
 #0 0x0000000004de1d32 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /data/home/bennshi/llvm-project/llvm/lib/Support/Unix/Signals.inc:880:22
 #1 0x0000000004de21f2 PrintStackTraceSignalHandler(void*) /data/home/bennshi/llvm-project/llvm/lib/Support/Unix/Signals.inc:962:1
 #2 0x0000000004ddf5b4 llvm::sys::RunSignalHandlers() /data/home/bennshi/llvm-project/llvm/lib/Support/Signals.cpp:108:20
 #3 0x0000000004de1600 SignalHandler(int, siginfo_t*, void*) /data/home/bennshi/llvm-project/llvm/lib/Support/Unix/Signals.inc:448:14
 #4 0x00007f25b35ec990 __restore_rt (/lib64/libpthread.so.0+0x12990)
 #5 0x00007f25b207a5af raise (/lib64/libc.so.6+0x4e5af)
 #6 0x00007f25b204dee5 abort (/lib64/libc.so.6+0x21ee5)
 #7 0x0000000004cfbcf5 bindingsErrorHandler(void*, char const*, bool) /data/home/bennshi/llvm-project/llvm/lib/Support/ErrorHandling.cpp:253:55
 #8 0x0000000002d3cbb5 llvm::AVRTargetLowering::LowerOperation(llvm::SDValue, llvm::SelectionDAG&) const /data/home/bennshi/llvm-project/llvm/lib/Target/AVR/AVRISelLowering.cpp:937:31
 #9 0x00000000049bafe0 (anonymous namespace)::SelectionDAGLegalize::LegalizeOp(llvm::SDNode*) /data/home/bennshi/llvm-project/llvm/lib/CodeGen/SelectionDAG/LegalizeDAG.cpp:1350:65
#10 0x00000000049e3ce8 llvm::SelectionDAG::Legalize() /data/home/bennshi/llvm-project/llvm/lib/CodeGen/SelectionDAG/LegalizeDAG.cpp:6546:25
#11 0x0000000004b20b25 llvm::SelectionDAGISel::CodeGenAndEmitDAG() /data/home/bennshi/llvm-project/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:1120:61
#12 0x0000000004b1f53e llvm::SelectionDAGISel::SelectBasicBlock(llvm::ilist_iterator_w_bits<llvm::ilist_detail::node_options<llvm::Instruction, true, false, void, true, llvm::BasicBlock>, false, true>, llvm::ilist_iterator_w_bits<llvm::ilist_detail::node_options<llvm::Instruction, true, false, void, true, llvm::BasicBlock>, false, true>, bool&) /data/home/bennshi/llvm-project/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:921:1
#13 0x0000000004b25023 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&) /data/home/bennshi/llvm-project/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:1962:33
#14 0x0000000004b1dd8a llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&) /data/home/bennshi/llvm-project/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:659:7
#15 0x0000000002d319e5 (anonymous namespace)::AVRDAGToDAGISel::runOnMachineFunction(llvm::MachineFunction&) /data/home/bennshi/llvm-project/llvm/lib/Target/AVR/AVRISelDAGToDAG.cpp:78:1
#16 0x0000000004b1c8d9 llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&) /data/home/bennshi/llvm-project/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:400:40
#17 0x00000000036bbe3d llvm::MachineFunctionPass::runOnFunction(llvm::Function&) /data/home/bennshi/llvm-project/llvm/lib/CodeGen/MachineFunctionPass.cpp:112:30
#18 0x0000000003f6aaa7 llvm::FPPassManager::runOnFunction(llvm::Function&) /data/home/bennshi/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:1398:20
#19 0x0000000003f6ada0 llvm::FPPassManager::runOnModule(llvm::Module&) /data/home/bennshi/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:1444:13
#20 0x0000000003f6b177 (anonymous namespace)::MPPassManager::runOnModule(llvm::Module&) /data/home/bennshi/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:1513:20
#21 0x0000000003f66a9f llvm::legacy::PassManagerImpl::run(llvm::Module&) /data/home/bennshi/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:531:13
#22 0x0000000003f6b98b llvm::legacy::PassManager::run(llvm::Module&) /data/home/bennshi/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:1641:1
#23 0x000000000263922a compileModule(char**, llvm::SmallVectorImpl<llvm::PassPlugin>&, llvm::LLVMContext&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>&) /data/home/bennshi/llvm-project/llvm/tools/llc/llc.cpp:871:34
#24 0x0000000002636728 main /data/home/bennshi/llvm-project/llvm/tools/llc/llc.cpp:459:35
#25 0x00007f25b2066865 __libc_start_main (/lib64/libc.so.6+0x3a865)
#26 0x00000000026351ee _start (./bin/llc+0x26351ee)
```

---

