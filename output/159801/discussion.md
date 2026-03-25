# [AArch64][SVE] LLVM ERROR: Cannot select in llvm.vector.deinterleave3.nxv12i32

**Author:** stevesuzuki-arm
**URL:** https://github.com/llvm/llvm-project/issues/159801
**Status:** Closed
**Labels:** backend:AArch64, SVE, crash
**Closed Date:** 2025-10-10T12:26:37Z

## Body

https://godbolt.org/z/e3o9KM8Wa
```
target triple = "aarch64-linux-gnu"

define void @load_factor3(i32* %ptr, <vscale x 4 x i32>* %s1, <vscale x 4 x i32>* %s2, <vscale x 4 x i32>* %s3) #0 {

  %wide.vec = load <vscale x 12 x i32>, ptr %ptr, align 8
  %ldN = tail call { <vscale x 4 x i32>, <vscale x 4 x i32>, <vscale x 4 x i32> } @llvm.vector.deinterleave3.nxv12i32(<vscale x 12 x i32> %wide.vec)

  %3 = extractvalue { <vscale x 4 x i32>, <vscale x 4 x i32>, <vscale x 4 x i32> } %ldN, 0
  %4 = extractvalue { <vscale x 4 x i32>, <vscale x 4 x i32>, <vscale x 4 x i32> } %ldN, 1
  %5 = extractvalue { <vscale x 4 x i32>, <vscale x 4 x i32>, <vscale x 4 x i32> } %ldN, 2

  store <vscale x 4 x i32> %3, <vscale x 4 x i32>* %s1
  store <vscale x 4 x i32> %4, <vscale x 4 x i32>* %s2
  store <vscale x 4 x i32> %5, <vscale x 4 x i32>* %s3
  ret void
}

declare { <vscale x 4 x i32>, <vscale x 4 x i32>, <vscale x 4 x i32> } @llvm.vector.deinterleave3.nxv12i32(<vscale x 12 x i32>)

```
`llc -mtriple=aarch64-none-elf -mattr=+sve2 -O2`

Factor of 2 and 4 works, but 3 fails for all the data types. I expect `ld3` is generated in this case.
```
LLVM ERROR: Cannot select: 0x2a224360: nxv4i32,nxv4i32,nxv4i32 = vector_deinterleave 0x2a224590, 0x2a224750, 0x2a224830
  0x2a224590: nxv4i32,ch = load<(load (<vscale x 1 x s128>) from %ir.ptr, align 8)> 0x2a1aa7e0, 0x2a223cd0, undef:i64
    0x2a223cd0: i64,ch = CopyFromReg 0x2a1aa7e0, Register:i64 %0
  0x2a224750: nxv4i32,ch = load<(load (<vscale x 1 x s128>), align 8)> 0x2a1aa7e0, 0x2a2246e0, undef:i64
    0x2a2246e0: i64 = add nuw 0x2a223cd0, 0x2a224670
      0x2a223cd0: i64,ch = CopyFromReg 0x2a1aa7e0, Register:i64 %0
      0x2a224670: i64 = vscale Constant:i64<16>
  0x2a224830: nxv4i32,ch = load<(load (<vscale x 1 x s128>), align 8)> 0x2a1aa7e0, 0x2a224130, undef:i64
    0x2a224130: i64 = add 0x2a223cd0, 0x2a223fe0
      0x2a223cd0: i64,ch = CopyFromReg 0x2a1aa7e0, Register:i64 %0
      0x2a223fe0: i64 = vscale Constant:i64<32>
In function: load_factor3
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/llc -o /app/output.s -mtriple=aarch64-none-elf -mattr=+sve2 -O2 <source>
1.	Running pass 'Function Pass Manager' on module '<source>'.
2.	Running pass 'AArch64 Instruction Selection' on function '@load_factor3'
 #0 0x0000000003b7eed8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-trunk/bin/llc+0x3b7eed8)
 #1 0x0000000003b7c0a4 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x0000797ea0642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x0000797ea06969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x0000797ea0642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x0000797ea06287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x00000000007b5b1a llvm::UniqueStringSaver::save(llvm::Twine const&) (.cold) StringSaver.cpp:0:0
 #7 0x0000000003924cba llvm::SelectionDAGISel::CannotYetSelect(llvm::SDNode*) (/opt/compiler-explorer/clang-trunk/bin/llc+0x3924cba)
 #8 0x0000000003928ee9 llvm::SelectionDAGISel::SelectCodeCommon(llvm::SDNode*, unsigned char const*, unsigned int) (/opt/compiler-explorer/clang-trunk/bin/llc+0x3928ee9)
 #9 0x0000000000c3fad7 (anonymous namespace)::AArch64DAGToDAGISel::Select(llvm::SDNode*) AArch64ISelDAGToDAG.cpp:0:0
#10 0x0000000003921e24 llvm::SelectionDAGISel::DoInstructionSelection() (/opt/compiler-explorer/clang-trunk/bin/llc+0x3921e24)
#11 0x000000000392e1e2 llvm::SelectionDAGISel::CodeGenAndEmitDAG() (/opt/compiler-explorer/clang-trunk/bin/llc+0x392e1e2)
#12 0x0000000003930b6a llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&) (/opt/compiler-explorer/clang-trunk/bin/llc+0x3930b6a)
#13 0x0000000003932abe llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&) (/opt/compiler-explorer/clang-trunk/bin/llc+0x3932abe)
#14 0x00000000039219b1 llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&) (/opt/compiler-explorer/clang-trunk/bin/llc+0x39219b1)
#15 0x0000000002c126ad llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-trunk/bin/llc+0x2c126ad)
#16 0x00000000031747c2 llvm::FPPassManager::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-trunk/bin/llc+0x31747c2)
#17 0x0000000003174a51 llvm::FPPassManager::runOnModule(llvm::Module&) (/opt/compiler-explorer/clang-trunk/bin/llc+0x3174a51)
#18 0x0000000003176327 llvm::legacy::PassManagerImpl::run(llvm::Module&) (/opt/compiler-explorer/clang-trunk/bin/llc+0x3176327)
#19 0x00000000008fd479 compileModule(char**, llvm::LLVMContext&) llc.cpp:0:0
#20 0x00000000007ca916 main (/opt/compiler-explorer/clang-trunk/bin/llc+0x7ca916)
#21 0x0000797ea0629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#22 0x0000797ea0629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#23 0x00000000008f4ce5 _start (/opt/compiler-explorer/clang-trunk/bin/llc+0x8f4ce5)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-aarch64

Author: Steve Suzuki (stevesuzuki-arm)

<details>
https://godbolt.org/z/e3o9KM8Wa
```
target triple = "aarch64-linux-gnu"

define void @<!-- -->load_factor3(i32* %ptr, &lt;vscale x 4 x i32&gt;* %s1, &lt;vscale x 4 x i32&gt;* %s2, &lt;vscale x 4 x i32&gt;* %s3) #<!-- -->0 {

  %wide.vec = load &lt;vscale x 12 x i32&gt;, ptr %ptr, align 8
  %ldN = tail call { &lt;vscale x 4 x i32&gt;, &lt;vscale x 4 x i32&gt;, &lt;vscale x 4 x i32&gt; } @<!-- -->llvm.vector.deinterleave3.nxv12i32(&lt;vscale x 12 x i32&gt; %wide.vec)

  %3 = extractvalue { &lt;vscale x 4 x i32&gt;, &lt;vscale x 4 x i32&gt;, &lt;vscale x 4 x i32&gt; } %ldN, 0
  %4 = extractvalue { &lt;vscale x 4 x i32&gt;, &lt;vscale x 4 x i32&gt;, &lt;vscale x 4 x i32&gt; } %ldN, 1
  %5 = extractvalue { &lt;vscale x 4 x i32&gt;, &lt;vscale x 4 x i32&gt;, &lt;vscale x 4 x i32&gt; } %ldN, 2

  store &lt;vscale x 4 x i32&gt; %3, &lt;vscale x 4 x i32&gt;* %s1
  store &lt;vscale x 4 x i32&gt; %4, &lt;vscale x 4 x i32&gt;* %s2
  store &lt;vscale x 4 x i32&gt; %5, &lt;vscale x 4 x i32&gt;* %s3
  ret void
}

declare { &lt;vscale x 4 x i32&gt;, &lt;vscale x 4 x i32&gt;, &lt;vscale x 4 x i32&gt; } @<!-- -->llvm.vector.deinterleave3.nxv12i32(&lt;vscale x 12 x i32&gt;)

```
`llc -mtriple=aarch64-none-elf -mattr=+sve2 -O2`

Factor of 2 and 4 works, but 3 fails for all the data types. I expect `ld3` is generated in this case.
```
LLVM ERROR: Cannot select: 0x2a224360: nxv4i32,nxv4i32,nxv4i32 = vector_deinterleave 0x2a224590, 0x2a224750, 0x2a224830
  0x2a224590: nxv4i32,ch = load&lt;(load (&lt;vscale x 1 x s128&gt;) from %ir.ptr, align 8)&gt; 0x2a1aa7e0, 0x2a223cd0, undef:i64
    0x2a223cd0: i64,ch = CopyFromReg 0x2a1aa7e0, Register:i64 %0
  0x2a224750: nxv4i32,ch = load&lt;(load (&lt;vscale x 1 x s128&gt;), align 8)&gt; 0x2a1aa7e0, 0x2a2246e0, undef:i64
    0x2a2246e0: i64 = add nuw 0x2a223cd0, 0x2a224670
      0x2a223cd0: i64,ch = CopyFromReg 0x2a1aa7e0, Register:i64 %0
      0x2a224670: i64 = vscale Constant:i64&lt;16&gt;
  0x2a224830: nxv4i32,ch = load&lt;(load (&lt;vscale x 1 x s128&gt;), align 8)&gt; 0x2a1aa7e0, 0x2a224130, undef:i64
    0x2a224130: i64 = add 0x2a223cd0, 0x2a223fe0
      0x2a223cd0: i64,ch = CopyFromReg 0x2a1aa7e0, Register:i64 %0
      0x2a223fe0: i64 = vscale Constant:i64&lt;32&gt;
In function: load_factor3
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/llc -o /app/output.s -mtriple=aarch64-none-elf -mattr=+sve2 -O2 &lt;source&gt;
1.	Running pass 'Function Pass Manager' on module '&lt;source&gt;'.
2.	Running pass 'AArch64 Instruction Selection' on function '@<!-- -->load_factor3'
 #<!-- -->0 0x0000000003b7eed8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-trunk/bin/llc+0x3b7eed8)
 #<!-- -->1 0x0000000003b7c0a4 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->2 0x0000797ea0642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->3 0x0000797ea06969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->4 0x0000797ea0642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->5 0x0000797ea06287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->6 0x00000000007b5b1a llvm::UniqueStringSaver::save(llvm::Twine const&amp;) (.cold) StringSaver.cpp:0:0
 #<!-- -->7 0x0000000003924cba llvm::SelectionDAGISel::CannotYetSelect(llvm::SDNode*) (/opt/compiler-explorer/clang-trunk/bin/llc+0x3924cba)
 #<!-- -->8 0x0000000003928ee9 llvm::SelectionDAGISel::SelectCodeCommon(llvm::SDNode*, unsigned char const*, unsigned int) (/opt/compiler-explorer/clang-trunk/bin/llc+0x3928ee9)
 #<!-- -->9 0x0000000000c3fad7 (anonymous namespace)::AArch64DAGToDAGISel::Select(llvm::SDNode*) AArch64ISelDAGToDAG.cpp:0:0
#<!-- -->10 0x0000000003921e24 llvm::SelectionDAGISel::DoInstructionSelection() (/opt/compiler-explorer/clang-trunk/bin/llc+0x3921e24)
#<!-- -->11 0x000000000392e1e2 llvm::SelectionDAGISel::CodeGenAndEmitDAG() (/opt/compiler-explorer/clang-trunk/bin/llc+0x392e1e2)
#<!-- -->12 0x0000000003930b6a llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&amp;) (/opt/compiler-explorer/clang-trunk/bin/llc+0x3930b6a)
#<!-- -->13 0x0000000003932abe llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&amp;) (/opt/compiler-explorer/clang-trunk/bin/llc+0x3932abe)
#<!-- -->14 0x00000000039219b1 llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&amp;) (/opt/compiler-explorer/clang-trunk/bin/llc+0x39219b1)
#<!-- -->15 0x0000000002c126ad llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (/opt/compiler-explorer/clang-trunk/bin/llc+0x2c126ad)
#<!-- -->16 0x00000000031747c2 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (/opt/compiler-explorer/clang-trunk/bin/llc+0x31747c2)
#<!-- -->17 0x0000000003174a51 llvm::FPPassManager::runOnModule(llvm::Module&amp;) (/opt/compiler-explorer/clang-trunk/bin/llc+0x3174a51)
#<!-- -->18 0x0000000003176327 llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/opt/compiler-explorer/clang-trunk/bin/llc+0x3176327)
#<!-- -->19 0x00000000008fd479 compileModule(char**, llvm::LLVMContext&amp;) llc.cpp:0:0
#<!-- -->20 0x00000000007ca916 main (/opt/compiler-explorer/clang-trunk/bin/llc+0x7ca916)
#<!-- -->21 0x0000797ea0629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->22 0x0000797ea0629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->23 0x00000000008f4ce5 _start (/opt/compiler-explorer/clang-trunk/bin/llc+0x8f4ce5)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```
</details>


---

### Comment 2 - Rajveer100

cc @davemgreen 

It looks like something changed after 20.1.0?

---

### Comment 3 - davemgreen

I don't think anyone has ever added support for these for SVE. (Before clang-19 they were not treated as intrinsics, so turned into normal calls). The load+deinterleave3 should become a ld3 instruction, but the general case is more difficult to efficiently codegen.

---

### Comment 4 - Rajveer100

> I don't think anyone has ever added support for these for SVE. (Before clang-19 they were not treated as intrinsics, so turned into normal calls). The load+deinterleave3 should become a ld3 instruction, but the general case is more difficult to efficiently codegen.

Any possibility of potentially approaching this :)

Also, could you explain what's the challenging part here and relevant pieces to look into?

---

### Comment 5 - davemgreen

The load+deinterleave3 could be handled by the InterleavedAccessPass. Probably via lowerDeinterleaveIntrinsicToLoad?

Lowering the intrinsic more generically needs a way to shuffle the values into the correct lanes, which for a scalable vector can be difficult. I haven't spent too much time thinking about it, maybe there is a better way than storing it to the stack and reloading with an ld3.

---

### Comment 6 - paulwalker-arm

Fixed by https://github.com/llvm/llvm-project/issues/162502. When https://github.com/llvm/llvm-project/pull/162475 lands you'll see an improvement to the generated code.

---

