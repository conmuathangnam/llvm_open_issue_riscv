# [PowerPC] Assertion failure in DAGCombineExtBoolTrunc

**Author:** nikic
**URL:** https://github.com/llvm/llvm-project/issues/160040
**Status:** Closed
**Labels:** backend:PowerPC, crash
**Closed Date:** 2025-09-22T19:37:15Z

## Body

```llvm
target triple = "powerpc64le-unknown-linux-gnu"

define i32 @test(i32 %arg) {
  %icmp = icmp sgt i32 %arg, -1
  %select = select i1 %icmp, i16 1, i16 0
  %select1 = select i1 %icmp, i16 16384, i16 0
  %lshr = lshr i16 %select1, 1
  %zext = zext i16 %lshr to i32
  %lshr2 = lshr i32 %zext, 1
  %shl = shl i16 %select, 1
  %zext3 = zext i16 %shl to i32
  %or = or i32 %lshr2, %zext3
  ret i32 %or
}
```

```
llc: /home/npopov/repos/llvm-project/llvm/include/llvm/CodeGen/SelectionDAGNodes.h:1035: const SDValue &llvm::SDNode::getOperand(unsigned int) const: Assertion `Num < NumOperands && "Invalid child # of SDNode!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: build/bin/llc
1.	Running pass 'Function Pass Manager' on module '<stdin>'.
2.	Running pass 'PowerPC DAG->DAG Pattern Instruction Selection' on function '@test'
 #0 0x000000000794da28 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (build/bin/llc+0x794da28)
 #1 0x000000000794b115 llvm::sys::RunSignalHandlers() (build/bin/llc+0x794b115)
 #2 0x000000000794e7d1 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x00007f8258828bb0 __restore_rt (/lib64/libc.so.6+0x19bb0)
 #4 0x00007f825888209c __pthread_kill_implementation (/lib64/libc.so.6+0x7309c)
 #5 0x00007f8258828a7e gsignal (/lib64/libc.so.6+0x19a7e)
 #6 0x00007f82588106d0 abort (/lib64/libc.so.6+0x16d0)
 #7 0x00007f8258810639 __assert_perror_fail (/lib64/libc.so.6+0x1639)
 #8 0x000000000595cd14 llvm::PPCTargetLowering::DAGCombineExtBoolTrunc(llvm::SDNode*, llvm::TargetLowering::DAGCombinerInfo&) const PPCISelLowering.cpp:0:0
 #9 0x0000000005960e43 llvm::PPCTargetLowering::PerformDAGCombine(llvm::SDNode*, llvm::TargetLowering::DAGCombinerInfo&) const PPCISelLowering.cpp:0:0
#10 0x0000000007527da2 (anonymous namespace)::DAGCombiner::combine(llvm::SDNode*) DAGCombiner.cpp:0:0
#11 0x0000000007526ae6 llvm::SelectionDAG::Combine(llvm::CombineLevel, llvm::BatchAAResults*, llvm::CodeGenOptLevel) (build/bin/llc+0x7526ae6)
#12 0x000000000772cb76 llvm::SelectionDAGISel::CodeGenAndEmitDAG() (build/bin/llc+0x772cb76)
#13 0x000000000772b8c1 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&) (build/bin/llc+0x772b8c1)
#14 0x0000000007728cf0 llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&) (build/bin/llc+0x7728cf0)
#15 0x0000000005971b4e (anonymous namespace)::PPCDAGToDAGISel::runOnMachineFunction(llvm::MachineFunction&) PPCISelDAGToDAG.cpp:0:0
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-powerpc

Author: Nikita Popov (nikic)

<details>
```llvm
target triple = "powerpc64le-unknown-linux-gnu"

define i32 @<!-- -->test(i32 %arg) {
  %icmp = icmp sgt i32 %arg, -1
  %select = select i1 %icmp, i16 1, i16 0
  %select1 = select i1 %icmp, i16 16384, i16 0
  %lshr = lshr i16 %select1, 1
  %zext = zext i16 %lshr to i32
  %lshr2 = lshr i32 %zext, 1
  %shl = shl i16 %select, 1
  %zext3 = zext i16 %shl to i32
  %or = or i32 %lshr2, %zext3
  ret i32 %or
}
```

```
llc: /home/npopov/repos/llvm-project/llvm/include/llvm/CodeGen/SelectionDAGNodes.h:1035: const SDValue &amp;llvm::SDNode::getOperand(unsigned int) const: Assertion `Num &lt; NumOperands &amp;&amp; "Invalid child # of SDNode!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: build/bin/llc
1.	Running pass 'Function Pass Manager' on module '&lt;stdin&gt;'.
2.	Running pass 'PowerPC DAG-&gt;DAG Pattern Instruction Selection' on function '@<!-- -->test'
 #<!-- -->0 0x000000000794da28 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (build/bin/llc+0x794da28)
 #<!-- -->1 0x000000000794b115 llvm::sys::RunSignalHandlers() (build/bin/llc+0x794b115)
 #<!-- -->2 0x000000000794e7d1 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x00007f8258828bb0 __restore_rt (/lib64/libc.so.6+0x19bb0)
 #<!-- -->4 0x00007f825888209c __pthread_kill_implementation (/lib64/libc.so.6+0x7309c)
 #<!-- -->5 0x00007f8258828a7e gsignal (/lib64/libc.so.6+0x19a7e)
 #<!-- -->6 0x00007f82588106d0 abort (/lib64/libc.so.6+0x16d0)
 #<!-- -->7 0x00007f8258810639 __assert_perror_fail (/lib64/libc.so.6+0x1639)
 #<!-- -->8 0x000000000595cd14 llvm::PPCTargetLowering::DAGCombineExtBoolTrunc(llvm::SDNode*, llvm::TargetLowering::DAGCombinerInfo&amp;) const PPCISelLowering.cpp:0:0
 #<!-- -->9 0x0000000005960e43 llvm::PPCTargetLowering::PerformDAGCombine(llvm::SDNode*, llvm::TargetLowering::DAGCombinerInfo&amp;) const PPCISelLowering.cpp:0:0
#<!-- -->10 0x0000000007527da2 (anonymous namespace)::DAGCombiner::combine(llvm::SDNode*) DAGCombiner.cpp:0:0
#<!-- -->11 0x0000000007526ae6 llvm::SelectionDAG::Combine(llvm::CombineLevel, llvm::BatchAAResults*, llvm::CodeGenOptLevel) (build/bin/llc+0x7526ae6)
#<!-- -->12 0x000000000772cb76 llvm::SelectionDAGISel::CodeGenAndEmitDAG() (build/bin/llc+0x772cb76)
#<!-- -->13 0x000000000772b8c1 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&amp;) (build/bin/llc+0x772b8c1)
#<!-- -->14 0x0000000007728cf0 llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&amp;) (build/bin/llc+0x7728cf0)
#<!-- -->15 0x0000000005971b4e (anonymous namespace)::PPCDAGToDAGISel::runOnMachineFunction(llvm::MachineFunction&amp;) PPCISelDAGToDAG.cpp:0:0
```
</details>


---

### Comment 2 - nikic

Crashing invocation is on:
```
t48: i32 = zero_extend t46
  t46: i1 = xor t31, Constant:i1<-1>
    t31: i1 = truncate t30
      t30: i32 = srl t3, Constant:i32<31>
        t3: i32 = truncate t2
          t2: i64,ch = CopyFromReg t0, Register:i64 %0
            t0: ch,glue = EntryToken
```

In https://github.com/llvm/llvm-project/blob/8323ff0c59739e4735fd896fad4be05e54432815/llvm/lib/Target/PowerPC/PPCISelLowering.cpp#L15201 we end up operating on a deleted node.

---

