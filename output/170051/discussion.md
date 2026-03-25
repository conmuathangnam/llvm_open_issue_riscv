# [NVPTX] Assertion `!B && "UpdateTerminators requires analyzable predecessors!"' failed.

**Author:** XChy
**URL:** https://github.com/llvm/llvm-project/issues/170051
**Status:** Closed
**Labels:** llvm:codegen, crash-on-valid
**Closed Date:** 2025-12-03T15:56:01Z

## Body

Reproducer: https://godbolt.org/z/qMr1xn7eb
Reduced testcase:
```llvm
target datalayout = "e-p6:32:32-i64:64-i128:128-i256:256-v16:16-v32:32-n16:32:64"
target triple = "nvptx64-unknown-nvidiacl"

define void @func_26(i32 %BS_COND_16.0.BS_COND_16.0.BS_COND_16.0.BS_COND_16.0.) {
entry:
  br label %for.cond

for.cond:                                         ; preds = %for.cond4.for.cond_crit_edge, %BS_LABEL_1, %BS_LABEL_1, %entry
  %p_2218_0.1 = phi i32 [ 0, %entry ], [ %p_2218_0.3, %BS_LABEL_1 ], [ %p_2218_0.3, %BS_LABEL_1 ], [ poison, %for.cond4.for.cond_crit_edge ]
  br label %BS_LABEL_1

BS_LABEL_2:                                       ; preds = %BS_LABEL_1
  %sub = or i32 %p_2218_0.3, 1
  br label %for.cond4

for.cond4:                                        ; preds = %BS_LABEL_1, %BS_LABEL_2
  %p_2218_0.2 = phi i32 [ 0, %BS_LABEL_1 ], [ %sub, %BS_LABEL_2 ]
  br i1 false, label %for.cond4.for.cond_crit_edge, label %BS_LABEL_1

for.cond4.for.cond_crit_edge:                     ; preds = %for.cond4
  br label %for.cond

BS_LABEL_1:                                       ; preds = %for.cond4, %for.cond
  %p_2218_0.3 = phi i32 [ %p_2218_0.2, %for.cond4 ], [ %p_2218_0.1, %for.cond ]
  switch i32 %BS_COND_16.0.BS_COND_16.0.BS_COND_16.0.BS_COND_16.0., label %unreachable [
    i32 0, label %for.cond4
    i32 4, label %BS_LABEL_2
    i32 1, label %for.cond
    i32 6, label %for.cond
  ]

unreachable:                                      ; preds = %BS_LABEL_1
  unreachable

; uselistorder directives
  uselistorder label %for.cond, { 1, 2, 0, 3 }
  uselistorder i32 %p_2218_0.3, { 2, 1, 0 }
}
```

Dumps:
```
llc: /root/llvm-project/llvm/lib/CodeGen/MachineBasicBlock.cpp:726: void llvm::MachineBasicBlock::updateTerminator(llvm::MachineBasicBlock*): Assertion `!B && "UpdateTerminators requires analyzable predecessors!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/llc -o /app/output.s -x86-asm-syntax=intel <source>
1.	Running pass 'Function Pass Manager' on module '<source>'.
2.	Running pass 'Eliminate PHI nodes for register allocation' on function '@func_26'
 #0 0x00000000041df148 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x41df148)
 #1 0x00000000041dbff4 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x00007fb2c4c42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x00007fb2c4c969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x00007fb2c4c42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x00007fb2c4c287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x00007fb2c4c2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #7 0x00007fb2c4c39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #8 0x00000000030421a2 llvm::MachineBasicBlock::updateTerminator(llvm::MachineBasicBlock*) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x30421a2)
 #9 0x000000000304338e llvm::MachineBasicBlock::SplitCriticalEdge(llvm::MachineBasicBlock*, llvm::MachineBasicBlock::SplitCriticalEdgeAnalyses const&, std::vector<llvm::SparseBitVector<128u>, std::allocator<llvm::SparseBitVector<128u>>>*, llvm::MachineDomTreeUpdater*) (.part.0) MachineBasicBlock.cpp:0:0
#10 0x000000000304603d llvm::MachineBasicBlock::SplitCriticalEdge(llvm::MachineBasicBlock*, llvm::Pass*, llvm::AnalysisManager<llvm::MachineFunction>*, std::vector<llvm::SparseBitVector<128u>, std::allocator<llvm::SparseBitVector<128u>>>*, llvm::MachineDomTreeUpdater*) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x304603d)
#11 0x000000000327db98 (anonymous namespace)::PHIEliminationImpl::run(llvm::MachineFunction&) PHIElimination.cpp:0:0
#12 0x0000000003280764 (anonymous namespace)::PHIElimination::runOnMachineFunction(llvm::MachineFunction&) PHIElimination.cpp:0:0
#13 0x00000000030cf2b9 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (.part.0) MachineFunctionPass.cpp:0:0
#14 0x000000000371a9e6 llvm::FPPassManager::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x371a9e6)
#15 0x000000000371ad91 llvm::FPPassManager::runOnModule(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x371ad91)
#16 0x000000000371b5ff llvm::legacy::PassManagerImpl::run(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x371b5ff)
#17 0x00000000008f909f compileModule(char**, llvm::LLVMContext&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>&) llc.cpp:0:0
#18 0x00000000007c6299 main (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x7c6299)
#19 0x00007fb2c4c29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#20 0x00007fb2c4c29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#21 0x00000000008ee345 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x8ee345)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

