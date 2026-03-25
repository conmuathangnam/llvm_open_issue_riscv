# [RISCV] Assertion `LVI->end >= Stop && "range ends mid block with no uses"' failed.

**Author:** XChy
**URL:** https://github.com/llvm/llvm-project/issues/177784

## Body

Reproducer: https://godbolt.org/z/jdxW8hnY8
Testcase:
```llvm
target datalayout = "e-m:e-p:64:64-i64:64-i128:128-n32:64-S128"
target triple = "riscv64-unknown-linux-musl"

define i32 @func_1(i16 %conv, i1 %tobool.not) #0 {
entry:
  br label %BS_LABEL_9

BS_LABEL_9:                                       ; preds = %BS_LABEL_1, %entry
  %0 = insertelement <16 x i16> zeroinitializer, i16 %conv, i64 0
  br label %for.body

for.body:                                         ; preds = %for.body, %BS_LABEL_9
  %l_4157.093 = phi i32 [ 1, %BS_LABEL_9 ], [ 0, %for.body ]
  %call = tail call i64 null()
  br i1 %tobool.not, label %for.end21.loopexit, label %for.body

for.end21.loopexit:                               ; preds = %for.body
  %conv1.le = trunc i32 %l_4157.093 to i16
  %vecinit16.le = insertelement <16 x i16> %0, i16 %conv1.le, i64 1
  br label %BS_LABEL_12.outer

BS_LABEL_12.outer:                                ; preds = %BS_LABEL_1, %for.end21.loopexit
  %BS_VAR_4.4.ph135 = phi <16 x i16> [ %vecinit16.le, %for.end21.loopexit ], [ %BS_VAR_4.4, %BS_LABEL_1 ]
  br label %BS_LABEL_12

BS_LABEL_12:                                      ; preds = %cleanup63, %BS_LABEL_12.outer
  %BS_VAR_4.4 = phi <16 x i16> [ %BS_VAR_4.4.ph135, %BS_LABEL_12.outer ], [ %BS_VAR_4.5, %cleanup63 ]
  br label %for.cond42

for.cond42:                                       ; preds = %cleanup63, %BS_LABEL_12
  %BS_VAR_4.5 = phi <16 x i16> [ %BS_VAR_4.4.ph135, %BS_LABEL_12 ], [ zeroinitializer, %cleanup63 ]
  br label %BS_LABEL_1

cleanup63:                                        ; preds = %BS_LABEL_1
  switch i32 0, label %cleanup63.unreachabledefault116 [
    i32 0, label %for.cond42
    i32 13, label %BS_LABEL_12
    i32 23, label %BS_LABEL_1
  ]

BS_LABEL_1:                                       ; preds = %BS_LABEL_1, %cleanup63, %for.cond42
  callbr void asm sideeffect "", "!i,!i,!i,!i"()
          to label %cleanup68 [label %BS_LABEL_12.outer, label %BS_LABEL_1, label %cleanup63, label %BS_LABEL_9]

cleanup63.unreachabledefault116:                  ; preds = %cleanup63
  unreachable

cleanup68:                                        ; preds = %BS_LABEL_1
  ret i32 0
}

attributes #0 = { "target-features"="+v" }
```

Dump:
```
llc: /root/llvm-project/llvm/lib/CodeGen/SplitKit.cpp:229: void llvm::SplitAnalysis::calcLiveBlockInfo(): Assertion `LVI->end >= Stop && "range ends mid block with no uses"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/llc -o /app/output.s -x86-asm-syntax=intel <source>
1.	Running pass 'Function Pass Manager' on module '<source>'.
2.	Running pass 'Greedy Register Allocator' on function '@func_1'
 #0 0x00000000042533c8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x42533c8)
 #1 0x00000000042502a4 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x000072fd2d042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x000072fd2d0969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x000072fd2d042476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x000072fd2d0287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x000072fd2d02871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #7 0x000072fd2d039e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #8 0x000000000340b349 llvm::SplitAnalysis::calcLiveBlockInfo() (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x340b349)
 #9 0x000000000340b678 llvm::SplitAnalysis::analyzeUses() (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x340b678)
#10 0x000000000334616f llvm::RAGreedy::tryAssignCSRFirstTime(llvm::LiveInterval const&, llvm::AllocationOrder&, llvm::MCRegister, unsigned char&, llvm::SmallVectorImpl<llvm::Register>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x334616f)
#11 0x000000000334d9cb llvm::RAGreedy::selectOrSplitImpl(llvm::LiveInterval const&, llvm::SmallVectorImpl<llvm::Register>&, llvm::SmallSet<llvm::Register, 16u, std::less<llvm::Register>>&, llvm::SmallVector<std::pair<llvm::LiveInterval const*, llvm::MCRegister>, 8u>&, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x334d9cb)
#12 0x000000000334e707 llvm::RAGreedy::selectOrSplit(llvm::LiveInterval const&, llvm::SmallVectorImpl<llvm::Register>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x334e707)
#13 0x00000000035b60f0 llvm::RegAllocBase::allocatePhysRegs() (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x35b60f0)
#14 0x000000000334a2fb llvm::RAGreedy::run(llvm::MachineFunction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x334a2fb)
#15 0x000000000334af4a (anonymous namespace)::RAGreedyLegacy::runOnMachineFunction(llvm::MachineFunction&) RegAllocGreedy.cpp:0:0
#16 0x000000000310c569 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (.part.0) MachineFunctionPass.cpp:0:0
#17 0x00000000037693a6 llvm::FPPassManager::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x37693a6)
#18 0x0000000003769751 llvm::FPPassManager::runOnModule(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3769751)
#19 0x0000000003769fbf llvm::legacy::PassManagerImpl::run(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3769fbf)
#20 0x00000000008ea452 compileModule(char**, llvm::SmallVectorImpl<llvm::PassPlugin>&, llvm::LLVMContext&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>&) llc.cpp:0:0
#21 0x00000000007c5c2c main (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x7c5c2c)
#22 0x000072fd2d029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#23 0x000072fd2d029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#24 0x00000000008df4f5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x8df4f5)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-risc-v

Author: Hongyu Chen (XChy)

<details>
Reproducer: https://godbolt.org/z/jdxW8hnY8
Testcase:
```llvm
target datalayout = "e-m:e-p:64:64-i64:64-i128:128-n32:64-S128"
target triple = "riscv64-unknown-linux-musl"

define i32 @<!-- -->func_1(i16 %conv, i1 %tobool.not) #<!-- -->0 {
entry:
  br label %BS_LABEL_9

BS_LABEL_9:                                       ; preds = %BS_LABEL_1, %entry
  %0 = insertelement &lt;16 x i16&gt; zeroinitializer, i16 %conv, i64 0
  br label %for.body

for.body:                                         ; preds = %for.body, %BS_LABEL_9
  %l_4157.093 = phi i32 [ 1, %BS_LABEL_9 ], [ 0, %for.body ]
  %call = tail call i64 null()
  br i1 %tobool.not, label %for.end21.loopexit, label %for.body

for.end21.loopexit:                               ; preds = %for.body
  %conv1.le = trunc i32 %l_4157.093 to i16
  %vecinit16.le = insertelement &lt;16 x i16&gt; %0, i16 %conv1.le, i64 1
  br label %BS_LABEL_12.outer

BS_LABEL_12.outer:                                ; preds = %BS_LABEL_1, %for.end21.loopexit
  %BS_VAR_4.4.ph135 = phi &lt;16 x i16&gt; [ %vecinit16.le, %for.end21.loopexit ], [ %BS_VAR_4.4, %BS_LABEL_1 ]
  br label %BS_LABEL_12

BS_LABEL_12:                                      ; preds = %cleanup63, %BS_LABEL_12.outer
  %BS_VAR_4.4 = phi &lt;16 x i16&gt; [ %BS_VAR_4.4.ph135, %BS_LABEL_12.outer ], [ %BS_VAR_4.5, %cleanup63 ]
  br label %for.cond42

for.cond42:                                       ; preds = %cleanup63, %BS_LABEL_12
  %BS_VAR_4.5 = phi &lt;16 x i16&gt; [ %BS_VAR_4.4.ph135, %BS_LABEL_12 ], [ zeroinitializer, %cleanup63 ]
  br label %BS_LABEL_1

cleanup63:                                        ; preds = %BS_LABEL_1
  switch i32 0, label %cleanup63.unreachabledefault116 [
    i32 0, label %for.cond42
    i32 13, label %BS_LABEL_12
    i32 23, label %BS_LABEL_1
  ]

BS_LABEL_1:                                       ; preds = %BS_LABEL_1, %cleanup63, %for.cond42
  callbr void asm sideeffect "", "!i,!i,!i,!i"()
          to label %cleanup68 [label %BS_LABEL_12.outer, label %BS_LABEL_1, label %cleanup63, label %BS_LABEL_9]

cleanup63.unreachabledefault116:                  ; preds = %cleanup63
  unreachable

cleanup68:                                        ; preds = %BS_LABEL_1
  ret i32 0
}

attributes #<!-- -->0 = { "target-features"="+v" }
```

Dump:
```
llc: /root/llvm-project/llvm/lib/CodeGen/SplitKit.cpp:229: void llvm::SplitAnalysis::calcLiveBlockInfo(): Assertion `LVI-&gt;end &gt;= Stop &amp;&amp; "range ends mid block with no uses"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/llc -o /app/output.s -x86-asm-syntax=intel &lt;source&gt;
1.	Running pass 'Function Pass Manager' on module '&lt;source&gt;'.
2.	Running pass 'Greedy Register Allocator' on function '@<!-- -->func_1'
 #<!-- -->0 0x00000000042533c8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x42533c8)
 #<!-- -->1 0x00000000042502a4 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->2 0x000072fd2d042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->3 0x000072fd2d0969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->4 0x000072fd2d042476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->5 0x000072fd2d0287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->6 0x000072fd2d02871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->7 0x000072fd2d039e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->8 0x000000000340b349 llvm::SplitAnalysis::calcLiveBlockInfo() (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x340b349)
 #<!-- -->9 0x000000000340b678 llvm::SplitAnalysis::analyzeUses() (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x340b678)
#<!-- -->10 0x000000000334616f llvm::RAGreedy::tryAssignCSRFirstTime(llvm::LiveInterval const&amp;, llvm::AllocationOrder&amp;, llvm::MCRegister, unsigned char&amp;, llvm::SmallVectorImpl&lt;llvm::Register&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x334616f)
#<!-- -->11 0x000000000334d9cb llvm::RAGreedy::selectOrSplitImpl(llvm::LiveInterval const&amp;, llvm::SmallVectorImpl&lt;llvm::Register&gt;&amp;, llvm::SmallSet&lt;llvm::Register, 16u, std::less&lt;llvm::Register&gt;&gt;&amp;, llvm::SmallVector&lt;std::pair&lt;llvm::LiveInterval const*, llvm::MCRegister&gt;, 8u&gt;&amp;, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x334d9cb)
#<!-- -->12 0x000000000334e707 llvm::RAGreedy::selectOrSplit(llvm::LiveInterval const&amp;, llvm::SmallVectorImpl&lt;llvm::Register&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x334e707)
#<!-- -->13 0x00000000035b60f0 llvm::RegAllocBase::allocatePhysRegs() (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x35b60f0)
#<!-- -->14 0x000000000334a2fb llvm::RAGreedy::run(llvm::MachineFunction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x334a2fb)
#<!-- -->15 0x000000000334af4a (anonymous namespace)::RAGreedyLegacy::runOnMachineFunction(llvm::MachineFunction&amp;) RegAllocGreedy.cpp:0:0
#<!-- -->16 0x000000000310c569 llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (.part.0) MachineFunctionPass.cpp:0:0
#<!-- -->17 0x00000000037693a6 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x37693a6)
#<!-- -->18 0x0000000003769751 llvm::FPPassManager::runOnModule(llvm::Module&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3769751)
#<!-- -->19 0x0000000003769fbf llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3769fbf)
#<!-- -->20 0x00000000008ea452 compileModule(char**, llvm::SmallVectorImpl&lt;llvm::PassPlugin&gt;&amp;, llvm::LLVMContext&amp;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;&amp;) llc.cpp:0:0
#<!-- -->21 0x00000000007c5c2c main (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x7c5c2c)
#<!-- -->22 0x000072fd2d029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->23 0x000072fd2d029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->24 0x00000000008df4f5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x8df4f5)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```
</details>


---

### Comment 2 - topperc

@BeMg do you want to take a look?

---

### Comment 3 - sweiglbosker

This should fix?

```diff
diff --git a/llvm/lib/CodeGen/LiveRangeEdit.cpp b/llvm/lib/CodeGen/LiveRangeEdit.cpp
index 6fe11704a913..d34f8d672ac9 100644
--- a/llvm/lib/CodeGen/LiveRangeEdit.cpp
+++ b/llvm/lib/CodeGen/LiveRangeEdit.cpp
@@ -253,6 +253,9 @@ void LiveRangeEdit::eliminateDeadDef(MachineInstr *MI, ToShrinkSet &ToShrink) {
     }
     LiveInterval &LI = LIS.getInterval(Reg);
 
+    if (MO.readsReg())
+      HasLiveVRegUses = true;
+
     // Shrink read registers, unless it is likely to be expensive and
     // unlikely to change anything. We typically don't want to shrink the
     // PIC base register that has lots of uses everywhere.
@@ -261,8 +264,6 @@ void LiveRangeEdit::eliminateDeadDef(MachineInstr *MI, ToShrinkSet &ToShrink) {
          (MO.isDef() || TII.isCopyInstr(*MI))) ||
         (MO.readsReg() && (MRI.hasOneNonDBGUse(Reg) || useIsKill(LI, MO))))
       ToShrink.insert(&LI);
-    else if (MO.readsReg())
-      HasLiveVRegUses = true;
 
     // Remove defined valu
```

---

