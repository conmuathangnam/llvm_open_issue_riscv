# [X86] LLVM ERROR: Cannot select: t10: v8i32 = fp_to_sint_sat t3, ValueType:ch:i24

**Author:** Andarwinux
**URL:** https://github.com/llvm/llvm-project/issues/186115
**Status:** Closed
**Labels:** backend:X86, crash, llvm:SelectionDAG
**Closed Date:** 2026-03-20T13:16:27Z

## Body

https://llvm.godbolt.org/z/rcnYParcY

reduced.ll
```llvm
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define fastcc <8 x i24> @store_s24() {
  %1 = tail call <8 x i24> @llvm.fptosi.sat.v8i24.v8f32(<8 x float> zeroinitializer)
  ret <8 x i24> %1
}

; Function Attrs: nocallback nocreateundeforpoison nofree nosync nounwind speculatable willreturn memory(none)
declare <8 x i24> @llvm.fptosi.sat.v8i24.v8f32(<8 x float>) #0

attributes #0 = { nocallback nocreateundeforpoison nofree nosync nounwind speculatable willreturn memory(none) }
```

llc -O3 -mcpu=novalake
```
LLVM ERROR: Cannot select: t10: v8i32 = fp_to_sint_sat t3, ValueType:ch:i24
  t3: v8f32 = BUILD_VECTOR ConstantFP:f32<0.000000e+00>, ConstantFP:f32<0.000000e+00>, ConstantFP:f32<0.000000e+00>, ConstantFP:f32<0.000000e+00>, ConstantFP:f32<0.000000e+00>, ConstantFP:f32<0.000000e+00>, ConstantFP:f32<0.000000e+00>, ConstantFP:f32<0.000000e+00>
In function: store_s24
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/llc -o /app/output.s -x86-asm-syntax=intel -O3 -mcpu=novalake <source>
1.	Running pass 'Function Pass Manager' on module '<source>'.
2.	Running pass 'X86 DAG->DAG Instruction Selection' on function '@store_s24'
 #0 0x00000000044be588 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x44be588)
 #1 0x00000000044bb3b4 llvm::sys::RunSignalHandlers() (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x44bb3b4)
 #2 0x00000000044bb52a SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x000073f04a842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x000073f04a8969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x000073f04a842476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x000073f04a8287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00000000007b6a27 llvm::json::operator==(llvm::json::Value const&, llvm::json::Value const&) (.cold) JSON.cpp:0:0
 #8 0x000000000423b685 llvm::SelectionDAGISel::CannotYetSelect(llvm::SDNode*) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x423b685)
 #9 0x00000000042433ba llvm::SelectionDAGISel::SelectCodeCommon(llvm::SDNode*, unsigned char const*, unsigned int, unsigned char const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x42433ba)
#10 0x000000000246323a (anonymous namespace)::X86DAGToDAGISel::Select(llvm::SDNode*) X86ISelDAGToDAG.cpp:0:0
#11 0x000000000423800b llvm::SelectionDAGISel::DoInstructionSelection() (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x423800b)
#12 0x0000000004248218 llvm::SelectionDAGISel::CodeGenAndEmitDAG() (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x4248218)
#13 0x000000000424b4f5 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x424b4f5)
#14 0x000000000424ce8c llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x424ce8c)
#15 0x00000000042377ff llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x42377ff)
#16 0x0000000003221629 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (.part.0) MachineFunctionPass.cpp:0:0
#17 0x0000000003881006 llvm::FPPassManager::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3881006)
#18 0x00000000038813b1 llvm::FPPassManager::runOnModule(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x38813b1)
#19 0x0000000003881c1f llvm::legacy::PassManagerImpl::run(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3881c1f)
#20 0x00000000008e6922 compileModule(char**, llvm::SmallVectorImpl<llvm::PassPlugin>&, llvm::LLVMContext&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>&) llc.cpp:0:0
#21 0x00000000007bcb6c main (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x7bcb6c)
#22 0x000073f04a829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#23 0x000073f04a829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#24 0x00000000008db9c5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x8db9c5)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

## Comments

### Comment 1 - phoebewang

We cannot select `v8i32 = fp_to_sint_sat t3, ValueType:ch:i24` directly, because we only have native instruction for i32. We can refer to AArch64's implement in `LowerVectorFP_TO_INT_SAT`, which widden it to i32 and then inserts min/max/truncate nodes to make it fix to i24.

---

### Comment 2 - Andarwinux

But without AVX10.2, it seems to work: https://llvm.godbolt.org/z/fjoahjTxz

---

### Comment 3 - llvmbot


@llvm/issue-subscribers-backend-x86

Author: None (Andarwinux)

<details>
https://llvm.godbolt.org/z/rcnYParcY

reduced.ll
```llvm
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define fastcc &lt;8 x i24&gt; @<!-- -->store_s24() {
  %1 = tail call &lt;8 x i24&gt; @<!-- -->llvm.fptosi.sat.v8i24.v8f32(&lt;8 x float&gt; zeroinitializer)
  ret &lt;8 x i24&gt; %1
}

; Function Attrs: nocallback nocreateundeforpoison nofree nosync nounwind speculatable willreturn memory(none)
declare &lt;8 x i24&gt; @<!-- -->llvm.fptosi.sat.v8i24.v8f32(&lt;8 x float&gt;) #<!-- -->0

attributes #<!-- -->0 = { nocallback nocreateundeforpoison nofree nosync nounwind speculatable willreturn memory(none) }
```

llc -O3 -mcpu=novalake
```
LLVM ERROR: Cannot select: t10: v8i32 = fp_to_sint_sat t3, ValueType:ch:i24
  t3: v8f32 = BUILD_VECTOR ConstantFP:f32&lt;0.000000e+00&gt;, ConstantFP:f32&lt;0.000000e+00&gt;, ConstantFP:f32&lt;0.000000e+00&gt;, ConstantFP:f32&lt;0.000000e+00&gt;, ConstantFP:f32&lt;0.000000e+00&gt;, ConstantFP:f32&lt;0.000000e+00&gt;, ConstantFP:f32&lt;0.000000e+00&gt;, ConstantFP:f32&lt;0.000000e+00&gt;
In function: store_s24
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/llc -o /app/output.s -x86-asm-syntax=intel -O3 -mcpu=novalake &lt;source&gt;
1.	Running pass 'Function Pass Manager' on module '&lt;source&gt;'.
2.	Running pass 'X86 DAG-&gt;DAG Instruction Selection' on function '@<!-- -->store_s24'
 #<!-- -->0 0x00000000044be588 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x44be588)
 #<!-- -->1 0x00000000044bb3b4 llvm::sys::RunSignalHandlers() (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x44bb3b4)
 #<!-- -->2 0x00000000044bb52a SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x000073f04a842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x000073f04a8969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x000073f04a842476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x000073f04a8287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x00000000007b6a27 llvm::json::operator==(llvm::json::Value const&amp;, llvm::json::Value const&amp;) (.cold) JSON.cpp:0:0
 #<!-- -->8 0x000000000423b685 llvm::SelectionDAGISel::CannotYetSelect(llvm::SDNode*) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x423b685)
 #<!-- -->9 0x00000000042433ba llvm::SelectionDAGISel::SelectCodeCommon(llvm::SDNode*, unsigned char const*, unsigned int, unsigned char const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x42433ba)
#<!-- -->10 0x000000000246323a (anonymous namespace)::X86DAGToDAGISel::Select(llvm::SDNode*) X86ISelDAGToDAG.cpp:0:0
#<!-- -->11 0x000000000423800b llvm::SelectionDAGISel::DoInstructionSelection() (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x423800b)
#<!-- -->12 0x0000000004248218 llvm::SelectionDAGISel::CodeGenAndEmitDAG() (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x4248218)
#<!-- -->13 0x000000000424b4f5 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x424b4f5)
#<!-- -->14 0x000000000424ce8c llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x424ce8c)
#<!-- -->15 0x00000000042377ff llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x42377ff)
#<!-- -->16 0x0000000003221629 llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (.part.0) MachineFunctionPass.cpp:0:0
#<!-- -->17 0x0000000003881006 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3881006)
#<!-- -->18 0x00000000038813b1 llvm::FPPassManager::runOnModule(llvm::Module&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x38813b1)
#<!-- -->19 0x0000000003881c1f llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3881c1f)
#<!-- -->20 0x00000000008e6922 compileModule(char**, llvm::SmallVectorImpl&lt;llvm::PassPlugin&gt;&amp;, llvm::LLVMContext&amp;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;&amp;) llc.cpp:0:0
#<!-- -->21 0x00000000007bcb6c main (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x7bcb6c)
#<!-- -->22 0x000073f04a829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->23 0x000073f04a829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->24 0x00000000008db9c5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x8db9c5)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```
</details>


---

### Comment 4 - ParkHanbum

maybe related this : https://github.com/llvm/llvm-project/commit/36dd421355791c9c9f6552f8730b36b8953ce5db#diff-127804a86c7aa0a15bf9f5092e9d649364fdf7aed495534ee1ad9de424f3ab8b

---

### Comment 5 - ParkHanbum

`m@MN:~/main$ cat t.s`
```asm
        .att_syntax
        .file   "t.ll"
        .section        .rodata.cst4,"aM",@progbits,4
        .p2align        2, 0x0                          # -- Begin function store_s24
.LCPI0_0:
        .long   4286578688                      # 0xff800000
.LCPI0_1:
        .long   8388607                         # 0x7fffff
        .text
        .globl  store_s24
        .p2align        4
        .type   store_s24,@function
store_s24:                              # @store_s24
        .cfi_startproc
# %bb.0:
        vxorps  %xmm0, %xmm0, %xmm0
        vcvttps2dqs     %ymm0, %ymm0
        vpmaxsd .LCPI0_0(%rip){1to8}, %ymm0, %ymm0
        vpminsd .LCPI0_1(%rip){1to8}, %ymm0, %ymm0
        retq
.Lfunc_end0:
        .size   store_s24, .Lfunc_end0-store_s24
        .cfi_endproc
                                        # -- End function
        .section        ".note.GNU-stack","",@progbits
```

is this result what we expect?

---

### Comment 6 - Andarwinux

It seems so.

---

### Comment 7 - phoebewang

> is this result what we expect?

I think it doesn't show any gain compared with https://llvm.godbolt.org/z/fjoahjTxz, at least we don't know for now without uarch information. So a better solution would still make them Custom under AVX10.2, but return `Op` when `SatVT` matches `SrcVT` and `hasAVX10_2()` in LowerFP_TO_INT_SAT.

---

### Comment 8 - ParkHanbum

@phoebewang you correct. are you have plan to submit patch of this?

---

### Comment 9 - phoebewang

> [@phoebewang](https://github.com/phoebewang) you correct. are you have plan to submit patch of this?

I'm late for me. I'll do it tomorrow if no volunteers :)

---

### Comment 10 - ParkHanbum

@phoebewang I'll volunteer! 
Should I add i24 type to all previously created test cases for test cases?

---

### Comment 11 - ParkHanbum

I think we need more to work. see this : https://llvm.godbolt.org/z/onqxWGTfY 

"I noticed that if we simply return SDValue() for unsupported saturation widths (like i24 or i48), the legalizer falls back to the generic expansion path.

This generic path (which is the standard behavior in AVX10.1) relies on scalarizing the vector—extracting each element to perform normalization and NaN checking—which generates severely bloated code. The issue is that once we fall back to this generic expansion, we suffer this massive scalarization penalty even with AVX10.2 enabled.

To truly leverage AVX10.2 and avoid this unavoidable generic penalty, wouldn't it be better to custom lower these unsupported types? For instance, we could natively saturate to a supported width (e.g., i32 or i64 using vcvttps2dqs) to handle NaNs natively, and then simply clamp the results using vector vpmaxsd/vpminsd. This would prevent the fallback to the AVX10.1-era scalarization completely."

---

### Comment 12 - phoebewang

Thanks @ParkHanbum for working on this! You are correct. The `zeroinitializer` simplifies some check conditions. I was thinking `LowerFP_TO_INT_SAT` handle it well, but it only handles scalar case. That's why we saw the scalarization. I think we may enhance it. But for the AVX10.2 case, `vcvttps2dqs+vpmaxsd/vpminsd` is the best solution.

---

### Comment 13 - Andarwinux

fixed.

---

