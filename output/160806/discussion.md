# Cannot select: v64i1 = truncate

**Author:** androm3da
**URL:** https://github.com/llvm/llvm-project/issues/160806
**Status:** Closed
**Labels:** backend:Hexagon, crash
**Closed Date:** 2025-10-31T05:11:07Z

## Body

This program reduced from Rust's `core_simd` fails as shown below.

```
$ cat ./test_v64i1_truncate_error.ll
; Minimal LLVM IR reproducer for "Cannot select: v64i1 = truncate" error  
; Based on error from: crates/core_simd/src/select.rs:71:13
; Function: Mask::select implementations in portable-simd

target datalayout = "e-m:e-p:32:32:32-a:0-n16:32-i64:64:64-i32:32:32-i16:16:16-i1:8:8-f32:32:32-f64:64:64-v32:32:32-v64:64:64-v512:512:512-v1024:1024:1024-v2048:2048:2048"
target triple = "hexagon-unknown-linux-musl"

define void @test_v64i1_truncate_error(<64 x i32>* %mask_storage, <64 x i32>* %true_vals, <64 x i32>* %false_vals, <64 x i32>* %result) {
entry:
  ; Load a wide integer vector that represents mask data
  %mask_wide = load <64 x i32>, <64 x i32>* %mask_storage
  
  ; This truncate from <64 x i32> to <64 x i1> triggers: "Cannot select: v64i1 = truncate"
  ; This operation mimics what happens in Mask::select when converting mask representations
  %mask = trunc <64 x i32> %mask_wide to <64 x i1>
  
  ; Load the input vectors
  %true_vec = load <64 x i32>, <64 x i32>* %true_vals
  %false_vec = load <64 x i32>, <64 x i32>* %false_vals
  
  ; Use the v64i1 mask in a select operation - this forces LLVM to handle the truncate
  %selected = select <64 x i1> %mask, <64 x i32> %true_vec, <64 x i32> %false_vec
  
  ; Store result to prevent optimization elimination  
  store <64 x i32> %selected, <64 x i32>* %result
  ret void
}
```

```
$ ~/src/toolchain_for_hexagon/obj_lld/bin/llc -mattr=-small-data,+hvx-length128b ./test_v64i1_truncate_error.ll 
LLVM ERROR: Cannot select: t12: v64i1 = truncate t63
  t63: v64i32 = concat_vectors t61, t62
    t61: v32i32,ch = load<(load (s1024) from %ir.mask_storage, align 256)> t0, t2, undef:i32
      t2: i32,ch = CopyFromReg t0, Register:i32 %0
    t62: v32i32,ch = load<(load (s1024) from %ir.mask_storage + 128, basealign 256)> t0, t60, undef:i32
      t60: i32 = add t2, Constant:i32<128>
        t2: i32,ch = CopyFromReg t0, Register:i32 %0
In function: test_v64i1_truncate_error
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.      Program arguments: /home/brian/src/toolchain_for_hexagon/obj_lld/bin/llc -mattr=-small-data,+hvx-length128b ./test_v64i1_truncate_error.ll
1.      Running pass 'Function Pass Manager' on module './test_v64i1_truncate_error.ll'.
2.      Running pass 'Hexagon DAG->DAG Pattern Instruction Selection' on function '@test_v64i1_truncate_error'
...
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-hexagon

Author: Brian Cain (androm3da)

<details>
This program reduced from Rust's `core_simd` fails as shown below.

```
$ cat ./test_v64i1_truncate_error.ll
; Minimal LLVM IR reproducer for "Cannot select: v64i1 = truncate" error  
; Based on error from: crates/core_simd/src/select.rs:71:13
; Function: Mask::select implementations in portable-simd

target datalayout = "e-m:e-p:32:32:32-a:0-n16:32-i64:64:64-i32:32:32-i16:16:16-i1:8:8-f32:32:32-f64:64:64-v32:32:32-v64:64:64-v512:512:512-v1024:1024:1024-v2048:2048:2048"
target triple = "hexagon-unknown-linux-musl"

define void @<!-- -->test_v64i1_truncate_error(&lt;64 x i32&gt;* %mask_storage, &lt;64 x i32&gt;* %true_vals, &lt;64 x i32&gt;* %false_vals, &lt;64 x i32&gt;* %result) {
entry:
  ; Load a wide integer vector that represents mask data
  %mask_wide = load &lt;64 x i32&gt;, &lt;64 x i32&gt;* %mask_storage
  
  ; This truncate from &lt;64 x i32&gt; to &lt;64 x i1&gt; triggers: "Cannot select: v64i1 = truncate"
  ; This operation mimics what happens in Mask::select when converting mask representations
  %mask = trunc &lt;64 x i32&gt; %mask_wide to &lt;64 x i1&gt;
  
  ; Load the input vectors
  %true_vec = load &lt;64 x i32&gt;, &lt;64 x i32&gt;* %true_vals
  %false_vec = load &lt;64 x i32&gt;, &lt;64 x i32&gt;* %false_vals
  
  ; Use the v64i1 mask in a select operation - this forces LLVM to handle the truncate
  %selected = select &lt;64 x i1&gt; %mask, &lt;64 x i32&gt; %true_vec, &lt;64 x i32&gt; %false_vec
  
  ; Store result to prevent optimization elimination  
  store &lt;64 x i32&gt; %selected, &lt;64 x i32&gt;* %result
  ret void
}
```

```
$ ~/src/toolchain_for_hexagon/obj_lld/bin/llc -mattr=-small-data,+hvx-length128b ./test_v64i1_truncate_error.ll 
LLVM ERROR: Cannot select: t12: v64i1 = truncate t63
  t63: v64i32 = concat_vectors t61, t62
    t61: v32i32,ch = load&lt;(load (s1024) from %ir.mask_storage, align 256)&gt; t0, t2, undef:i32
      t2: i32,ch = CopyFromReg t0, Register:i32 %0
    t62: v32i32,ch = load&lt;(load (s1024) from %ir.mask_storage + 128, basealign 256)&gt; t0, t60, undef:i32
      t60: i32 = add t2, Constant:i32&lt;128&gt;
        t2: i32,ch = CopyFromReg t0, Register:i32 %0
In function: test_v64i1_truncate_error
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.      Program arguments: /home/brian/src/toolchain_for_hexagon/obj_lld/bin/llc -mattr=-small-data,+hvx-length128b ./test_v64i1_truncate_error.ll
1.      Running pass 'Function Pass Manager' on module './test_v64i1_truncate_error.ll'.
2.      Running pass 'Hexagon DAG-&gt;DAG Pattern Instruction Selection' on function '@<!-- -->test_v64i1_truncate_error'
...
```
</details>


---

### Comment 2 - androm3da

This case seems like it could have a common cause with the previous one.

```
; Minimal LLVM IR reproducer for "Cannot select: v64i1 = bitcast" error
; Based on error from: crates/core_simd/src/select.rs:107:17
; Function: u64::select_impl operations in portable-simd

target datalayout = "e-m:e-p:32:32:32-a:0-n16:32-i64:64:64-i32:32:32-i16:16:16-i1:8:8-f32:32:32-f64:64:64-v32:32:32-v64:64:64-v512:512:512-v1024:1024:1024-v2048:2048:2048"
target triple = "hexagon-unknown-linux-musl"

define void @test_v64i1_bitcast_error(i64 %mask_bits, <64 x i32>* %true_vals, <64 x i32>* %false_vals, <64 x i32>* %result) {
entry:
  ; This bitcast from i64 to <64 x i1> triggers: "Cannot select: v64i1 = bitcast"
  %mask = bitcast i64 %mask_bits to <64 x i1>
  
  ; Load the input vectors  
  %true_vec = load <64 x i32>, <64 x i32>* %true_vals
  %false_vec = load <64 x i32>, <64 x i32>* %false_vals
  
  ; Use the v64i1 mask in a select operation - this forces LLVM to handle the bitcast
  %selected = select <64 x i1> %mask, <64 x i32> %true_vec, <64 x i32> %false_vec
  
  ; Store result to prevent optimization elimination
  store <64 x i32> %selected, <64 x i32>* %result
  ret void
}
```

```
 ~/src/toolchain_for_hexagon/obj_lld/bin/llc -mattr=-small-data,+hvx-length128b ./test_v64i1_bitcast_error.ll 
LLVM ERROR: Cannot select: t9: v64i1 = bitcast t2
  t2: i64,ch = CopyFromReg t0, Register:i64 %0
In function: test_v64i1_bitcast_error
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.      Program arguments: /home/brian/src/toolchain_for_hexagon/obj_lld/bin/llc -mattr=-small-data,+hvx-length128b ./test_v64i1_bitcast_error.ll
1.      Running pass 'Function Pass Manager' on module './test_v64i1_bitcast_error.ll'.
2.      Running pass 'Hexagon DAG->DAG Pattern Instruction Selection' on function '@test_v64i1_bitcast_error'
 #0 0x00005d8c8598f9e8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/home/brian/src/toolchain_for_hexagon/obj_lld/bin/llc+0x80939e8)
 #1 0x00005d8c8598d0e5 llvm::sys::RunSignalHandlers() (/home/brian/src/toolchain_for_hexagon/obj_lld/bin/llc+0x80910e5)

```

---

### Comment 3 - aankit-ca

@pkarveti You worked on a similar ISEL issue. Can you please take care of this one too?

---

### Comment 4 - androm3da

@iajbar you closed this issue but the pull req says:

> Partially Fixes #160806

Maybe we should reopen it?

cc @pkarveti 

---

### Comment 5 - iajbar

I didn’t manually close it. It was closed automatically when the patch was merged.

---

