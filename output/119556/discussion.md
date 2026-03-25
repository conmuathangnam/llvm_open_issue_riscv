# Crash with -enable-ipra for trivial internal linkage functions for RISC-V/AArch64/Arm

**Author:** asb
**URL:** https://github.com/llvm/llvm-project/issues/119556

## Body

Reading #72129 it looks like there might have already been some related work looking at issues here - I'd really appreciate confirmation from @JanekvO / @arsenm on the status of this from an AMDGPU perspective and any other thoughts given it sounds like you've spent much more time looking into issues in this area. Many thanks in advance for any input.

Essentially, we see a crash for RISCV/AArch64/ARM for some internal linkage functions, notably even one that is just `ret void`. x86-64 and other targets don't fail. I've tracked down that disabling the machine outliner (i.e. `setMachineOutlin(false)` in RISCVTargetmachine) means there is no crash, and the function is completely elided, as it is for x86-64 and other targets.

Crash:
```
$ cat tc.ll 
define internal void @foo() {
  ret void
}
$ llc -mtriple=riscv64 -enable-ipra < tc.ll
llc: /home/asb/llvm-project/llvm/include/llvm/ADT/ilist_iterator.h:168: reference llvm::ilist_iterator<llvm::ilist_detail::node_options<llvm::MachineBasicBlock, true, false, void, false, void>, false, false>::operator*() const [OptionsT = llvm::ilist_detail::node_options<llvm::MachineBasicBlock, true, false, void, false, void>, IsReverse = false, IsConst = false]: Assertion `!NodePtr->isKnownSentinel()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.	Program arguments: ./build/default/bin/llc -mtriple=riscv64 -verify-machineinstrs -enable-ipra
1.	Running pass 'Function Pass Manager' on module '<stdin>'.
2.	Running pass 'RISC-V Assembly Printer' on function '@foo'
 #0 0x00007f66d4042e4d llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /home/asb/llvm-project/llvm/lib/Support/Unix/Signals.inc:723:11
 #1 0x00007f66d404343b PrintStackTraceSignalHandler(void*) /home/asb/llvm-project/llvm/lib/Support/Unix/Signals.inc:798:1
 #2 0x00007f66d4041003 llvm::sys::RunSignalHandlers() /home/asb/llvm-project/llvm/lib/Support/Signals.cpp:105:5
 #3 0x00007f66d4043cd1 SignalHandler(int) /home/asb/llvm-project/llvm/lib/Support/Unix/Signals.inc:413:1
 #4 0x00007f66d6a5a770 (/usr/lib/libc.so.6+0x3c770)
 #5 0x00007f66d6aab32c (/usr/lib/libc.so.6+0x8d32c)
 #6 0x00007f66d6a5a6c8 raise (/usr/lib/libc.so.6+0x3c6c8)
 #7 0x00007f66d6a424b8 abort (/usr/lib/libc.so.6+0x244b8)
 #8 0x00007f66d6a423dc (/usr/lib/libc.so.6+0x243dc)
 #9 0x00007f66d6a52d46 (/usr/lib/libc.so.6+0x34d46)
#10 0x00007f66da81960f llvm::ilist_iterator<llvm::ilist_detail::node_options<llvm::MachineBasicBlock, true, false, void, false, void>, false, false>::operator*() const /home/asb/llvm-project/llvm/include/llvm/ADT/ilist_iterator.h:0:5
#11 0x00007f66da82bcef llvm::simple_ilist<llvm::MachineBasicBlock>::front() /home/asb/llvm-project/llvm/include/llvm/ADT/simple_ilist.h:144:30
#12 0x00007f66da81695c llvm::MachineFunction::front() /home/asb/llvm-project/llvm/include/llvm/CodeGen/MachineFunction.h:940:44
#13 0x00007f66da7fadda llvm::AsmPrinter::emitFunctionHeader() /home/asb/llvm-project/llvm/lib/CodeGen/AsmPrinter/AsmPrinter.cpp:963:11
#14 0x00007f66da7fea36 llvm::AsmPrinter::emitFunctionBody() /home/asb/llvm-project/llvm/lib/CodeGen/AsmPrinter/AsmPrinter.cpp:1783:3
#15 0x00007f66de471ea9 (anonymous namespace)::RISCVAsmPrinter::runOnMachineFunction(llvm::MachineFunction&) /home/asb/llvm-project/llvm/lib/Target/RISCV/RISCVAsmPrinter.cpp:476:3
#16 0x00007f66d8428a5c llvm::MachineFunctionPass::runOnFunction(llvm::Function&) /home/asb/llvm-project/llvm/lib/CodeGen/MachineFunctionPass.cpp:94:8
#17 0x00007f66d4af5ae5 llvm::FPPassManager::runOnFunction(llvm::Function&) /home/asb/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:1406:23
#18 0x00007f66d4afad4f llvm::FPPassManager::runOnModule(llvm::Module&) /home/asb/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:1452:16
#19 0x00007f66d4af64cb (anonymous namespace)::MPPassManager::runOnModule(llvm::Module&) /home/asb/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:1521:23
#20 0x00007f66d4af5fb4 llvm::legacy::PassManagerImpl::run(llvm::Module&) /home/asb/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:539:16
#21 0x00007f66d4afb051 llvm::legacy::PassManager::run(llvm::Module&) /home/asb/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:1648:3
#22 0x000055612c1f6ceb compileModule(char**, llvm::LLVMContext&) /home/asb/llvm-project/llvm/tools/llc/llc.cpp:753:9
#23 0x000055612c1f4d26 main /home/asb/llvm-project/llvm/tools/llc/llc.cpp:411:13
#24 0x00007f66d6a43cd0 (/usr/lib/libc.so.6+0x25cd0)
#25 0x00007f66d6a43d8a __libc_start_main (/usr/lib/libc.so.6+0x25d8a)
#26 0x000055612c1f4415 _start (./build/default/bin/llc+0x49415)
```

Successful compilation for x86-64:
```
$ llc -mtriple=x86_64 -verify-machineinstrs < tc.ll -enable-ipra
	.text
	.file	"<stdin>"
	.section	".note.GNU-stack","",@progbits
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-risc-v

Author: Alex Bradbury (asb)

<details>
Reading #<!-- -->72129 it looks like there might have already been some related work looking at issues here - I'd really appreciate confirmation from @<!-- -->JanekvO / @<!-- -->arsenm on the status of this from an AMDGPU perspective and any other thoughts given it sounds like you've spent much more time looking into issues in this area. Many thanks in advance for any input.

Essentially, we see a crash for RISCV/AArch64/ARM for some internal linkage functions, notably even one that is just `ret void`. x86-64 and other targets don't fail. I've tracked down that disabling the machine outliner (i.e. `setMachineOutlin(false)` in RISCVTargetmachine) means there is no crash, and the function is completely elided, as it is for x86-64 and other targets.

Crash:
```
$ cat tc.ll 
define internal void @<!-- -->foo() {
  ret void
}
$ llc -mtriple=riscv64 -enable-ipra &lt; tc.ll
llc: /home/asb/llvm-project/llvm/include/llvm/ADT/ilist_iterator.h:168: reference llvm::ilist_iterator&lt;llvm::ilist_detail::node_options&lt;llvm::MachineBasicBlock, true, false, void, false, void&gt;, false, false&gt;::operator*() const [OptionsT = llvm::ilist_detail::node_options&lt;llvm::MachineBasicBlock, true, false, void, false, void&gt;, IsReverse = false, IsConst = false]: Assertion `!NodePtr-&gt;isKnownSentinel()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.	Program arguments: ./build/default/bin/llc -mtriple=riscv64 -verify-machineinstrs -enable-ipra
1.	Running pass 'Function Pass Manager' on module '&lt;stdin&gt;'.
2.	Running pass 'RISC-V Assembly Printer' on function '@<!-- -->foo'
 #<!-- -->0 0x00007f66d4042e4d llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /home/asb/llvm-project/llvm/lib/Support/Unix/Signals.inc:723:11
 #<!-- -->1 0x00007f66d404343b PrintStackTraceSignalHandler(void*) /home/asb/llvm-project/llvm/lib/Support/Unix/Signals.inc:798:1
 #<!-- -->2 0x00007f66d4041003 llvm::sys::RunSignalHandlers() /home/asb/llvm-project/llvm/lib/Support/Signals.cpp:105:5
 #<!-- -->3 0x00007f66d4043cd1 SignalHandler(int) /home/asb/llvm-project/llvm/lib/Support/Unix/Signals.inc:413:1
 #<!-- -->4 0x00007f66d6a5a770 (/usr/lib/libc.so.6+0x3c770)
 #<!-- -->5 0x00007f66d6aab32c (/usr/lib/libc.so.6+0x8d32c)
 #<!-- -->6 0x00007f66d6a5a6c8 raise (/usr/lib/libc.so.6+0x3c6c8)
 #<!-- -->7 0x00007f66d6a424b8 abort (/usr/lib/libc.so.6+0x244b8)
 #<!-- -->8 0x00007f66d6a423dc (/usr/lib/libc.so.6+0x243dc)
 #<!-- -->9 0x00007f66d6a52d46 (/usr/lib/libc.so.6+0x34d46)
#<!-- -->10 0x00007f66da81960f llvm::ilist_iterator&lt;llvm::ilist_detail::node_options&lt;llvm::MachineBasicBlock, true, false, void, false, void&gt;, false, false&gt;::operator*() const /home/asb/llvm-project/llvm/include/llvm/ADT/ilist_iterator.h:0:5
#<!-- -->11 0x00007f66da82bcef llvm::simple_ilist&lt;llvm::MachineBasicBlock&gt;::front() /home/asb/llvm-project/llvm/include/llvm/ADT/simple_ilist.h:144:30
#<!-- -->12 0x00007f66da81695c llvm::MachineFunction::front() /home/asb/llvm-project/llvm/include/llvm/CodeGen/MachineFunction.h:940:44
#<!-- -->13 0x00007f66da7fadda llvm::AsmPrinter::emitFunctionHeader() /home/asb/llvm-project/llvm/lib/CodeGen/AsmPrinter/AsmPrinter.cpp:963:11
#<!-- -->14 0x00007f66da7fea36 llvm::AsmPrinter::emitFunctionBody() /home/asb/llvm-project/llvm/lib/CodeGen/AsmPrinter/AsmPrinter.cpp:1783:3
#<!-- -->15 0x00007f66de471ea9 (anonymous namespace)::RISCVAsmPrinter::runOnMachineFunction(llvm::MachineFunction&amp;) /home/asb/llvm-project/llvm/lib/Target/RISCV/RISCVAsmPrinter.cpp:476:3
#<!-- -->16 0x00007f66d8428a5c llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) /home/asb/llvm-project/llvm/lib/CodeGen/MachineFunctionPass.cpp:94:8
#<!-- -->17 0x00007f66d4af5ae5 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) /home/asb/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:1406:23
#<!-- -->18 0x00007f66d4afad4f llvm::FPPassManager::runOnModule(llvm::Module&amp;) /home/asb/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:1452:16
#<!-- -->19 0x00007f66d4af64cb (anonymous namespace)::MPPassManager::runOnModule(llvm::Module&amp;) /home/asb/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:1521:23
#<!-- -->20 0x00007f66d4af5fb4 llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) /home/asb/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:539:16
#<!-- -->21 0x00007f66d4afb051 llvm::legacy::PassManager::run(llvm::Module&amp;) /home/asb/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:1648:3
#<!-- -->22 0x000055612c1f6ceb compileModule(char**, llvm::LLVMContext&amp;) /home/asb/llvm-project/llvm/tools/llc/llc.cpp:753:9
#<!-- -->23 0x000055612c1f4d26 main /home/asb/llvm-project/llvm/tools/llc/llc.cpp:411:13
#<!-- -->24 0x00007f66d6a43cd0 (/usr/lib/libc.so.6+0x25cd0)
#<!-- -->25 0x00007f66d6a43d8a __libc_start_main (/usr/lib/libc.so.6+0x25d8a)
#<!-- -->26 0x000055612c1f4415 _start (./build/default/bin/llc+0x49415)
```

Successful compilation for x86-64:
```
$ llc -mtriple=x86_64 -verify-machineinstrs &lt; tc.ll -enable-ipra
	.text
	.file	"&lt;stdin&gt;"
	.section	".note.GNU-stack","",@<!-- -->progbits
```
</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-backend-aarch64

Author: Alex Bradbury (asb)

<details>
Reading #<!-- -->72129 it looks like there might have already been some related work looking at issues here - I'd really appreciate confirmation from @<!-- -->JanekvO / @<!-- -->arsenm on the status of this from an AMDGPU perspective and any other thoughts given it sounds like you've spent much more time looking into issues in this area. Many thanks in advance for any input.

Essentially, we see a crash for RISCV/AArch64/ARM for some internal linkage functions, notably even one that is just `ret void`. x86-64 and other targets don't fail. I've tracked down that disabling the machine outliner (i.e. `setMachineOutlin(false)` in RISCVTargetmachine) means there is no crash, and the function is completely elided, as it is for x86-64 and other targets.

Crash:
```
$ cat tc.ll 
define internal void @<!-- -->foo() {
  ret void
}
$ llc -mtriple=riscv64 -enable-ipra &lt; tc.ll
llc: /home/asb/llvm-project/llvm/include/llvm/ADT/ilist_iterator.h:168: reference llvm::ilist_iterator&lt;llvm::ilist_detail::node_options&lt;llvm::MachineBasicBlock, true, false, void, false, void&gt;, false, false&gt;::operator*() const [OptionsT = llvm::ilist_detail::node_options&lt;llvm::MachineBasicBlock, true, false, void, false, void&gt;, IsReverse = false, IsConst = false]: Assertion `!NodePtr-&gt;isKnownSentinel()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.	Program arguments: ./build/default/bin/llc -mtriple=riscv64 -verify-machineinstrs -enable-ipra
1.	Running pass 'Function Pass Manager' on module '&lt;stdin&gt;'.
2.	Running pass 'RISC-V Assembly Printer' on function '@<!-- -->foo'
 #<!-- -->0 0x00007f66d4042e4d llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /home/asb/llvm-project/llvm/lib/Support/Unix/Signals.inc:723:11
 #<!-- -->1 0x00007f66d404343b PrintStackTraceSignalHandler(void*) /home/asb/llvm-project/llvm/lib/Support/Unix/Signals.inc:798:1
 #<!-- -->2 0x00007f66d4041003 llvm::sys::RunSignalHandlers() /home/asb/llvm-project/llvm/lib/Support/Signals.cpp:105:5
 #<!-- -->3 0x00007f66d4043cd1 SignalHandler(int) /home/asb/llvm-project/llvm/lib/Support/Unix/Signals.inc:413:1
 #<!-- -->4 0x00007f66d6a5a770 (/usr/lib/libc.so.6+0x3c770)
 #<!-- -->5 0x00007f66d6aab32c (/usr/lib/libc.so.6+0x8d32c)
 #<!-- -->6 0x00007f66d6a5a6c8 raise (/usr/lib/libc.so.6+0x3c6c8)
 #<!-- -->7 0x00007f66d6a424b8 abort (/usr/lib/libc.so.6+0x244b8)
 #<!-- -->8 0x00007f66d6a423dc (/usr/lib/libc.so.6+0x243dc)
 #<!-- -->9 0x00007f66d6a52d46 (/usr/lib/libc.so.6+0x34d46)
#<!-- -->10 0x00007f66da81960f llvm::ilist_iterator&lt;llvm::ilist_detail::node_options&lt;llvm::MachineBasicBlock, true, false, void, false, void&gt;, false, false&gt;::operator*() const /home/asb/llvm-project/llvm/include/llvm/ADT/ilist_iterator.h:0:5
#<!-- -->11 0x00007f66da82bcef llvm::simple_ilist&lt;llvm::MachineBasicBlock&gt;::front() /home/asb/llvm-project/llvm/include/llvm/ADT/simple_ilist.h:144:30
#<!-- -->12 0x00007f66da81695c llvm::MachineFunction::front() /home/asb/llvm-project/llvm/include/llvm/CodeGen/MachineFunction.h:940:44
#<!-- -->13 0x00007f66da7fadda llvm::AsmPrinter::emitFunctionHeader() /home/asb/llvm-project/llvm/lib/CodeGen/AsmPrinter/AsmPrinter.cpp:963:11
#<!-- -->14 0x00007f66da7fea36 llvm::AsmPrinter::emitFunctionBody() /home/asb/llvm-project/llvm/lib/CodeGen/AsmPrinter/AsmPrinter.cpp:1783:3
#<!-- -->15 0x00007f66de471ea9 (anonymous namespace)::RISCVAsmPrinter::runOnMachineFunction(llvm::MachineFunction&amp;) /home/asb/llvm-project/llvm/lib/Target/RISCV/RISCVAsmPrinter.cpp:476:3
#<!-- -->16 0x00007f66d8428a5c llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) /home/asb/llvm-project/llvm/lib/CodeGen/MachineFunctionPass.cpp:94:8
#<!-- -->17 0x00007f66d4af5ae5 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) /home/asb/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:1406:23
#<!-- -->18 0x00007f66d4afad4f llvm::FPPassManager::runOnModule(llvm::Module&amp;) /home/asb/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:1452:16
#<!-- -->19 0x00007f66d4af64cb (anonymous namespace)::MPPassManager::runOnModule(llvm::Module&amp;) /home/asb/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:1521:23
#<!-- -->20 0x00007f66d4af5fb4 llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) /home/asb/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:539:16
#<!-- -->21 0x00007f66d4afb051 llvm::legacy::PassManager::run(llvm::Module&amp;) /home/asb/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:1648:3
#<!-- -->22 0x000055612c1f6ceb compileModule(char**, llvm::LLVMContext&amp;) /home/asb/llvm-project/llvm/tools/llc/llc.cpp:753:9
#<!-- -->23 0x000055612c1f4d26 main /home/asb/llvm-project/llvm/tools/llc/llc.cpp:411:13
#<!-- -->24 0x00007f66d6a43cd0 (/usr/lib/libc.so.6+0x25cd0)
#<!-- -->25 0x00007f66d6a43d8a __libc_start_main (/usr/lib/libc.so.6+0x25d8a)
#<!-- -->26 0x000055612c1f4415 _start (./build/default/bin/llc+0x49415)
```

Successful compilation for x86-64:
```
$ llc -mtriple=x86_64 -verify-machineinstrs &lt; tc.ll -enable-ipra
	.text
	.file	"&lt;stdin&gt;"
	.section	".note.GNU-stack","",@<!-- -->progbits
```
</details>


---

### Comment 3 - llvmbot


@llvm/issue-subscribers-backend-arm

Author: Alex Bradbury (asb)

<details>
Reading #<!-- -->72129 it looks like there might have already been some related work looking at issues here - I'd really appreciate confirmation from @<!-- -->JanekvO / @<!-- -->arsenm on the status of this from an AMDGPU perspective and any other thoughts given it sounds like you've spent much more time looking into issues in this area. Many thanks in advance for any input.

Essentially, we see a crash for RISCV/AArch64/ARM for some internal linkage functions, notably even one that is just `ret void`. x86-64 and other targets don't fail. I've tracked down that disabling the machine outliner (i.e. `setMachineOutlin(false)` in RISCVTargetmachine) means there is no crash, and the function is completely elided, as it is for x86-64 and other targets.

Crash:
```
$ cat tc.ll 
define internal void @<!-- -->foo() {
  ret void
}
$ llc -mtriple=riscv64 -enable-ipra &lt; tc.ll
llc: /home/asb/llvm-project/llvm/include/llvm/ADT/ilist_iterator.h:168: reference llvm::ilist_iterator&lt;llvm::ilist_detail::node_options&lt;llvm::MachineBasicBlock, true, false, void, false, void&gt;, false, false&gt;::operator*() const [OptionsT = llvm::ilist_detail::node_options&lt;llvm::MachineBasicBlock, true, false, void, false, void&gt;, IsReverse = false, IsConst = false]: Assertion `!NodePtr-&gt;isKnownSentinel()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.	Program arguments: ./build/default/bin/llc -mtriple=riscv64 -verify-machineinstrs -enable-ipra
1.	Running pass 'Function Pass Manager' on module '&lt;stdin&gt;'.
2.	Running pass 'RISC-V Assembly Printer' on function '@<!-- -->foo'
 #<!-- -->0 0x00007f66d4042e4d llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /home/asb/llvm-project/llvm/lib/Support/Unix/Signals.inc:723:11
 #<!-- -->1 0x00007f66d404343b PrintStackTraceSignalHandler(void*) /home/asb/llvm-project/llvm/lib/Support/Unix/Signals.inc:798:1
 #<!-- -->2 0x00007f66d4041003 llvm::sys::RunSignalHandlers() /home/asb/llvm-project/llvm/lib/Support/Signals.cpp:105:5
 #<!-- -->3 0x00007f66d4043cd1 SignalHandler(int) /home/asb/llvm-project/llvm/lib/Support/Unix/Signals.inc:413:1
 #<!-- -->4 0x00007f66d6a5a770 (/usr/lib/libc.so.6+0x3c770)
 #<!-- -->5 0x00007f66d6aab32c (/usr/lib/libc.so.6+0x8d32c)
 #<!-- -->6 0x00007f66d6a5a6c8 raise (/usr/lib/libc.so.6+0x3c6c8)
 #<!-- -->7 0x00007f66d6a424b8 abort (/usr/lib/libc.so.6+0x244b8)
 #<!-- -->8 0x00007f66d6a423dc (/usr/lib/libc.so.6+0x243dc)
 #<!-- -->9 0x00007f66d6a52d46 (/usr/lib/libc.so.6+0x34d46)
#<!-- -->10 0x00007f66da81960f llvm::ilist_iterator&lt;llvm::ilist_detail::node_options&lt;llvm::MachineBasicBlock, true, false, void, false, void&gt;, false, false&gt;::operator*() const /home/asb/llvm-project/llvm/include/llvm/ADT/ilist_iterator.h:0:5
#<!-- -->11 0x00007f66da82bcef llvm::simple_ilist&lt;llvm::MachineBasicBlock&gt;::front() /home/asb/llvm-project/llvm/include/llvm/ADT/simple_ilist.h:144:30
#<!-- -->12 0x00007f66da81695c llvm::MachineFunction::front() /home/asb/llvm-project/llvm/include/llvm/CodeGen/MachineFunction.h:940:44
#<!-- -->13 0x00007f66da7fadda llvm::AsmPrinter::emitFunctionHeader() /home/asb/llvm-project/llvm/lib/CodeGen/AsmPrinter/AsmPrinter.cpp:963:11
#<!-- -->14 0x00007f66da7fea36 llvm::AsmPrinter::emitFunctionBody() /home/asb/llvm-project/llvm/lib/CodeGen/AsmPrinter/AsmPrinter.cpp:1783:3
#<!-- -->15 0x00007f66de471ea9 (anonymous namespace)::RISCVAsmPrinter::runOnMachineFunction(llvm::MachineFunction&amp;) /home/asb/llvm-project/llvm/lib/Target/RISCV/RISCVAsmPrinter.cpp:476:3
#<!-- -->16 0x00007f66d8428a5c llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) /home/asb/llvm-project/llvm/lib/CodeGen/MachineFunctionPass.cpp:94:8
#<!-- -->17 0x00007f66d4af5ae5 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) /home/asb/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:1406:23
#<!-- -->18 0x00007f66d4afad4f llvm::FPPassManager::runOnModule(llvm::Module&amp;) /home/asb/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:1452:16
#<!-- -->19 0x00007f66d4af64cb (anonymous namespace)::MPPassManager::runOnModule(llvm::Module&amp;) /home/asb/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:1521:23
#<!-- -->20 0x00007f66d4af5fb4 llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) /home/asb/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:539:16
#<!-- -->21 0x00007f66d4afb051 llvm::legacy::PassManager::run(llvm::Module&amp;) /home/asb/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:1648:3
#<!-- -->22 0x000055612c1f6ceb compileModule(char**, llvm::LLVMContext&amp;) /home/asb/llvm-project/llvm/tools/llc/llc.cpp:753:9
#<!-- -->23 0x000055612c1f4d26 main /home/asb/llvm-project/llvm/tools/llc/llc.cpp:411:13
#<!-- -->24 0x00007f66d6a43cd0 (/usr/lib/libc.so.6+0x25cd0)
#<!-- -->25 0x00007f66d6a43d8a __libc_start_main (/usr/lib/libc.so.6+0x25d8a)
#<!-- -->26 0x000055612c1f4415 _start (./build/default/bin/llc+0x49415)
```

Successful compilation for x86-64:
```
$ llc -mtriple=x86_64 -verify-machineinstrs &lt; tc.ll -enable-ipra
	.text
	.file	"&lt;stdin&gt;"
	.section	".note.GNU-stack","",@<!-- -->progbits
```
</details>


---

### Comment 4 - JanekvO

Wrt #72129, we took the alternative approach of moving the resource usage propagation logic from the AMDGPUResourceUsageInfo pass to the MC layer which meant that AMDGPUResourceUsageInfo wasn't required to be a Module pass anymore.

> Essentially, we see a crash for RISCV/AArch64/ARM for some internal linkage functions, notably even one that is just ret void

Yeah, this does seem like the same as I've witnessed:
The core of the problem is that the pass manager stack pops off the CGSCC pass manager when a module pass is added as part of the codegen passes. This is also apparent with `--debug-pass=Structure` https://godbolt.org/z/M635a6M9n where the indent for FunctionPasses before and after adding the `MachineOutliner` pass are different (i.e., CGSCC pass manager is not re-added to the pass manager stack).

The behaviour of CGSCC pass manager is that all FunctionPasses added under the CGSCC pass manager are run as CGSCC passes. Uncalled internal functions are not run for the CGSCC pass manager passes but **are** on the regular FunctionPass pass manager passes.

Perhaps there's an easy legacy pass manager infrastructure fix but I don't recall finding any low hanging fruit like that (albeit, that might be more my lack of knowledge than anything else). The hacky fix that is used occasionally in AMDGPU, for other module passes added during codegen is by adding a `DummyCGSCCPass` after it to push a new CGSCC pass manager onto the pass manager stack but this is not very scalable + requires all the aforementioned pass manager knowledge from the developer who is adding a module pass in the codegen phase.

>  x86-64 and other targets don't fail

I recall looking at x86-64 and why it wasn't crashing on their end with `--enable-ipra` and what I found at the time was that they don't add any module passes as part of their codegen passes.

---

### Comment 5 - arsenm

I'd guess nobody has ever tried to turn out IPRA and the machine outliner at the same time before. The assert is just hitting an empty MachineFunction, presumably this is the first time it was queried from MachineModuleInfo.

The current IPRA logic assumes all the functions will be present in the original IR, though not sure in a way that is problematic for machine outlining. Mechanically the pass manager has no way of ordering the codegen of outlined functions, short of using a module barrier pass.


---

