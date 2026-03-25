# [MIPS64][GISel] GlobalISel crashed on a simple empty function and reported "Target needs to handle register class ID 0x35"

**Author:** XChy
**URL:** https://github.com/llvm/llvm-project/issues/166002
**Status:** Closed
**Labels:** backend:MIPS, llvm:globalisel, crash
**Closed Date:** 2025-12-26T10:49:56Z

## Body

Reproducer: https://godbolt.org/z/45c6de4nW
Testcase:
```llvm
target datalayout = "e-m:e-i8:8:32-i16:16:32-i64:64-i128:128-n32:64-S128"
target triple = "mips64el-unknown-unknown-elf"

define i64 @func_3() local_unnamed_addr #0 {
entry:
  ret i64 undef
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(none) "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="mips64r2" "target-features"="+mips64r2,-noabicalls" }
```

llc output:
```
Target needs to handle register class ID 0x35
UNREACHABLE executed at /root/build/lib/Target/Mips/MipsGenRegisterBank.inc:193!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/llc -o /app/output.s -x86-asm-syntax=intel -O3 -global-isel -global-isel-abort=2 <source>
1.	Running pass 'Function Pass Manager' on module '<source>'.
2.	Running pass 'RegBankSelect' on function '@func_3'
 #0 0x000000000419b988 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x419b988)
 #1 0x0000000004198834 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x000070a60e042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x000070a60e0969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x000070a60e042476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x000070a60e0287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x00000000040da8fa (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x40da8fa)
 #7 0x0000000001a3a145 llvm::MipsGenRegisterBankInfo::getRegBankFromRegClass(llvm::TargetRegisterClass const&, llvm::LLT) const (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x1a3a145)
 #8 0x0000000001a3dc0c llvm::MipsRegisterBankInfo::TypeInfoForMF::setTypesAccordingToPhysicalRegister(llvm::MachineInstr const*, llvm::MachineInstr const*, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x1a3dc0c)
 #9 0x0000000001a43713 llvm::MipsRegisterBankInfo::TypeInfoForMF::visitAdjacentInstrs(llvm::MachineInstr const*, llvm::SmallVectorImpl<llvm::MachineInstr*>&, bool, llvm::MipsRegisterBankInfo::InstType&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x1a43713)
#10 0x0000000001a4256d llvm::MipsRegisterBankInfo::TypeInfoForMF::visit(llvm::MachineInstr const*, llvm::MachineInstr const*, llvm::MipsRegisterBankInfo::InstType&) (.part.0) MipsRegisterBankInfo.cpp:0:0
#11 0x0000000001a43942 llvm::MipsRegisterBankInfo::TypeInfoForMF::determineInstType(llvm::MachineInstr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x1a43942)
#12 0x0000000001a449d5 llvm::MipsRegisterBankInfo::getInstrMapping(llvm::MachineInstr const&) const (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x1a449d5)
#13 0x000000000488c9d7 llvm::RegBankSelect::assignInstr(llvm::MachineInstr&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x488c9d7)
#14 0x000000000488cf75 llvm::RegBankSelect::assignRegisterBanks(llvm::MachineFunction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x488cf75)
#15 0x000000000488d1b6 llvm::RegBankSelect::runOnMachineFunction(llvm::MachineFunction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x488d1b6)
#16 0x000000000307eca9 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (.part.0) MachineFunctionPass.cpp:0:0
#17 0x00000000036ce866 llvm::FPPassManager::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x36ce866)
#18 0x00000000036cec11 llvm::FPPassManager::runOnModule(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x36cec11)
#19 0x00000000036cf47f llvm::legacy::PassManagerImpl::run(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x36cf47f)
#20 0x00000000008fa6c3 compileModule(char**, llvm::LLVMContext&) llc.cpp:0:0
#21 0x00000000007c7846 main (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x7c7846)
#22 0x000070a60e029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#23 0x000070a60e029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#24 0x00000000008efb05 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x8efb05)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-mips

Author: Hongyu Chen (XChy)

<details>
Reproducer: https://godbolt.org/z/45c6de4nW
Testcase:
```llvm
target datalayout = "e-m:e-i8:8:32-i16:16:32-i64:64-i128:128-n32:64-S128"
target triple = "mips64el-unknown-unknown-elf"

define i64 @<!-- -->func_3() local_unnamed_addr #<!-- -->0 {
entry:
  ret i64 undef
}

attributes #<!-- -->0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(none) "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="mips64r2" "target-features"="+mips64r2,-noabicalls" }
```

llc output:
```
Target needs to handle register class ID 0x35
UNREACHABLE executed at /root/build/lib/Target/Mips/MipsGenRegisterBank.inc:193!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/llc -o /app/output.s -x86-asm-syntax=intel -O3 -global-isel -global-isel-abort=2 &lt;source&gt;
1.	Running pass 'Function Pass Manager' on module '&lt;source&gt;'.
2.	Running pass 'RegBankSelect' on function '@<!-- -->func_3'
 #<!-- -->0 0x000000000419b988 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x419b988)
 #<!-- -->1 0x0000000004198834 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->2 0x000070a60e042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->3 0x000070a60e0969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->4 0x000070a60e042476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->5 0x000070a60e0287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->6 0x00000000040da8fa (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x40da8fa)
 #<!-- -->7 0x0000000001a3a145 llvm::MipsGenRegisterBankInfo::getRegBankFromRegClass(llvm::TargetRegisterClass const&amp;, llvm::LLT) const (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x1a3a145)
 #<!-- -->8 0x0000000001a3dc0c llvm::MipsRegisterBankInfo::TypeInfoForMF::setTypesAccordingToPhysicalRegister(llvm::MachineInstr const*, llvm::MachineInstr const*, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x1a3dc0c)
 #<!-- -->9 0x0000000001a43713 llvm::MipsRegisterBankInfo::TypeInfoForMF::visitAdjacentInstrs(llvm::MachineInstr const*, llvm::SmallVectorImpl&lt;llvm::MachineInstr*&gt;&amp;, bool, llvm::MipsRegisterBankInfo::InstType&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x1a43713)
#<!-- -->10 0x0000000001a4256d llvm::MipsRegisterBankInfo::TypeInfoForMF::visit(llvm::MachineInstr const*, llvm::MachineInstr const*, llvm::MipsRegisterBankInfo::InstType&amp;) (.part.0) MipsRegisterBankInfo.cpp:0:0
#<!-- -->11 0x0000000001a43942 llvm::MipsRegisterBankInfo::TypeInfoForMF::determineInstType(llvm::MachineInstr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x1a43942)
#<!-- -->12 0x0000000001a449d5 llvm::MipsRegisterBankInfo::getInstrMapping(llvm::MachineInstr const&amp;) const (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x1a449d5)
#<!-- -->13 0x000000000488c9d7 llvm::RegBankSelect::assignInstr(llvm::MachineInstr&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x488c9d7)
#<!-- -->14 0x000000000488cf75 llvm::RegBankSelect::assignRegisterBanks(llvm::MachineFunction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x488cf75)
#<!-- -->15 0x000000000488d1b6 llvm::RegBankSelect::runOnMachineFunction(llvm::MachineFunction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x488d1b6)
#<!-- -->16 0x000000000307eca9 llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (.part.0) MachineFunctionPass.cpp:0:0
#<!-- -->17 0x00000000036ce866 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x36ce866)
#<!-- -->18 0x00000000036cec11 llvm::FPPassManager::runOnModule(llvm::Module&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x36cec11)
#<!-- -->19 0x00000000036cf47f llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x36cf47f)
#<!-- -->20 0x00000000008fa6c3 compileModule(char**, llvm::LLVMContext&amp;) llc.cpp:0:0
#<!-- -->21 0x00000000007c7846 main (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x7c7846)
#<!-- -->22 0x000070a60e029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->23 0x000070a60e029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->24 0x00000000008efb05 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x8efb05)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```
</details>


---

### Comment 2 - XChy

C version:
```c
#include "stdint.h"
int64_t func_3() {}
```

---

### Comment 3 - brad0

cc @yingopq 

---

