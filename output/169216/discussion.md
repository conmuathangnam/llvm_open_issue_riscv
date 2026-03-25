# [RISCV] llc cannot lower `llvm.canonicalize.f64`

**Author:** valadaptive
**URL:** https://github.com/llvm/llvm-project/issues/169216
**Status:** Closed
**Labels:** crash, llvm:SelectionDAG
**Closed Date:** 2025-12-18T00:38:19Z

## Body

Previously reported for the x86 backend in https://github.com/llvm/llvm-project/issues/32650. @tgross35 mentioned in that issue that some backends still fail to lower `llvm.canonicalize.f64`, and linked to an armv7 example. The armv7 example now compiles in LLVM 21, but RISC-V still cannot compile `llvm.canonicalize.f64` (see https://llvm.godbolt.org/z/5jE3zeEc1):

```
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"

; Function Attrs: nounwind readnone uwtable
define double @max(double, double) unnamed_addr #0 {
start:
  %2 = fcmp olt double %0, %1
  %3 = fcmp uno double %0, 0.000000e+00
  %or.cond.i.i = or i1 %3, %2
  %4 = select i1 %or.cond.i.i, double %1, double %0
  %5 = tail call double @llvm.canonicalize.f64(double %4) #2
  ret double %5
}

; Function Attrs: nounwind readnone
declare double @llvm.canonicalize.f64(double) #1

attributes #0 = { nounwind readnone uwtable }
attributes #1 = { nounwind readnone }
attributes #2 = { nounwind }

---

LLVM ERROR: Do not know how to soften the result of this operator!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/llc -o /app/output.s -x86-asm-syntax=intel -mtriple=riscv64-unknown-linux-gnu -O3 <source>
1.	Running pass 'Function Pass Manager' on module '<source>'.
2.	Running pass 'RISC-V DAG->DAG Pattern Instruction Selection' on function '@max'
 #0 0x0000000003bbc648 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-trunk/bin/llc+0x3bbc648)
 #1 0x0000000003bb9814 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x0000739b09642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x0000739b096969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x0000739b09642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x0000739b096287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x00000000007ba4b0 llvm::json::operator==(llvm::json::Value const&, llvm::json::Value const&) (.cold) JSON.cpp:0:0
 #7 0x0000000003b04418 (/opt/compiler-explorer/clang-trunk/bin/llc+0x3b04418)
 #8 0x0000000003a77851 llvm::DAGTypeLegalizer::SoftenFloatResult(llvm::SDNode*, unsigned int) (/opt/compiler-explorer/clang-trunk/bin/llc+0x3a77851)
 #9 0x00000000039fba26 llvm::DAGTypeLegalizer::run() (/opt/compiler-explorer/clang-trunk/bin/llc+0x39fba26)
#10 0x00000000039fc0a1 llvm::SelectionDAG::LegalizeTypes() (/opt/compiler-explorer/clang-trunk/bin/llc+0x39fc0a1)
#11 0x0000000003966b92 llvm::SelectionDAGISel::CodeGenAndEmitDAG() (/opt/compiler-explorer/clang-trunk/bin/llc+0x3966b92)
#12 0x0000000003969651 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&) (/opt/compiler-explorer/clang-trunk/bin/llc+0x3969651)
#13 0x000000000396b605 llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&) (/opt/compiler-explorer/clang-trunk/bin/llc+0x396b605)
#14 0x000000000395a5a1 llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&) (/opt/compiler-explorer/clang-trunk/bin/llc+0x395a5a1)
#15 0x0000000002c5c9fd llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-trunk/bin/llc+0x2c5c9fd)
#16 0x00000000031b3112 llvm::FPPassManager::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-trunk/bin/llc+0x31b3112)
#17 0x00000000031b33a1 llvm::FPPassManager::runOnModule(llvm::Module&) (/opt/compiler-explorer/clang-trunk/bin/llc+0x31b33a1)
#18 0x00000000031b4c17 llvm::legacy::PassManagerImpl::run(llvm::Module&) (/opt/compiler-explorer/clang-trunk/bin/llc+0x31b4c17)
#19 0x00000000007cecd2 main (/opt/compiler-explorer/clang-trunk/bin/llc+0x7cecd2)
#20 0x0000739b09629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#21 0x0000739b09629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#22 0x00000000008fbf05 _start (/opt/compiler-explorer/clang-trunk/bin/llc+0x8fbf05)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-risc-v

Author: None (valadaptive)

<details>
Previously reported for the x86 backend in https://github.com/llvm/llvm-project/issues/32650. @<!-- -->tgross35 mentioned in that issue that some backends still fail to lower `llvm.canonicalize.f64`, and linked to an armv7 example. The armv7 example now compiles in LLVM 21, but RISC-V still cannot compile `llvm.canonicalize.f64` (see https://llvm.godbolt.org/z/5jE3zeEc1):

```
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"

; Function Attrs: nounwind readnone uwtable
define double @<!-- -->max(double, double) unnamed_addr #<!-- -->0 {
start:
  %2 = fcmp olt double %0, %1
  %3 = fcmp uno double %0, 0.000000e+00
  %or.cond.i.i = or i1 %3, %2
  %4 = select i1 %or.cond.i.i, double %1, double %0
  %5 = tail call double @<!-- -->llvm.canonicalize.f64(double %4) #<!-- -->2
  ret double %5
}

; Function Attrs: nounwind readnone
declare double @<!-- -->llvm.canonicalize.f64(double) #<!-- -->1

attributes #<!-- -->0 = { nounwind readnone uwtable }
attributes #<!-- -->1 = { nounwind readnone }
attributes #<!-- -->2 = { nounwind }

---

LLVM ERROR: Do not know how to soften the result of this operator!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/llc -o /app/output.s -x86-asm-syntax=intel -mtriple=riscv64-unknown-linux-gnu -O3 &lt;source&gt;
1.	Running pass 'Function Pass Manager' on module '&lt;source&gt;'.
2.	Running pass 'RISC-V DAG-&gt;DAG Pattern Instruction Selection' on function '@<!-- -->max'
 #<!-- -->0 0x0000000003bbc648 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-trunk/bin/llc+0x3bbc648)
 #<!-- -->1 0x0000000003bb9814 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->2 0x0000739b09642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->3 0x0000739b096969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->4 0x0000739b09642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->5 0x0000739b096287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->6 0x00000000007ba4b0 llvm::json::operator==(llvm::json::Value const&amp;, llvm::json::Value const&amp;) (.cold) JSON.cpp:0:0
 #<!-- -->7 0x0000000003b04418 (/opt/compiler-explorer/clang-trunk/bin/llc+0x3b04418)
 #<!-- -->8 0x0000000003a77851 llvm::DAGTypeLegalizer::SoftenFloatResult(llvm::SDNode*, unsigned int) (/opt/compiler-explorer/clang-trunk/bin/llc+0x3a77851)
 #<!-- -->9 0x00000000039fba26 llvm::DAGTypeLegalizer::run() (/opt/compiler-explorer/clang-trunk/bin/llc+0x39fba26)
#<!-- -->10 0x00000000039fc0a1 llvm::SelectionDAG::LegalizeTypes() (/opt/compiler-explorer/clang-trunk/bin/llc+0x39fc0a1)
#<!-- -->11 0x0000000003966b92 llvm::SelectionDAGISel::CodeGenAndEmitDAG() (/opt/compiler-explorer/clang-trunk/bin/llc+0x3966b92)
#<!-- -->12 0x0000000003969651 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&amp;) (/opt/compiler-explorer/clang-trunk/bin/llc+0x3969651)
#<!-- -->13 0x000000000396b605 llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&amp;) (/opt/compiler-explorer/clang-trunk/bin/llc+0x396b605)
#<!-- -->14 0x000000000395a5a1 llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&amp;) (/opt/compiler-explorer/clang-trunk/bin/llc+0x395a5a1)
#<!-- -->15 0x0000000002c5c9fd llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (/opt/compiler-explorer/clang-trunk/bin/llc+0x2c5c9fd)
#<!-- -->16 0x00000000031b3112 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (/opt/compiler-explorer/clang-trunk/bin/llc+0x31b3112)
#<!-- -->17 0x00000000031b33a1 llvm::FPPassManager::runOnModule(llvm::Module&amp;) (/opt/compiler-explorer/clang-trunk/bin/llc+0x31b33a1)
#<!-- -->18 0x00000000031b4c17 llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/opt/compiler-explorer/clang-trunk/bin/llc+0x31b4c17)
#<!-- -->19 0x00000000007cecd2 main (/opt/compiler-explorer/clang-trunk/bin/llc+0x7cecd2)
#<!-- -->20 0x0000739b09629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->21 0x0000739b09629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->22 0x00000000008fbf05 _start (/opt/compiler-explorer/clang-trunk/bin/llc+0x8fbf05)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```
</details>


---

### Comment 2 - topperc

Looks like this only occurs when hardware floating point support is not enabled. Adding -mattr=+d works.

---

### Comment 3 - lenary

I should hope so, the stacktrace is pointing at SoftenFloatResult

---

