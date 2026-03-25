# [cuclang 20.1.0] Using __reduce_max_sync in a cuda kernel fails with an illegal instruction was encountered

**Author:** AustinSchuh
**URL:** https://github.com/llvm/llvm-project/issues/131415
**Status:** Closed
**Labels:** cuda, clang:headers, miscompilation, release:backport
**Closed Date:** 2025-03-26T20:54:59Z

## Body

Using clang 20.1.0.  I've got a CUDA kernel which boils down to the following:

```
__global__ void testcode(const float* data, unsigned *max_value) {
    unsigned r = static_cast<unsigned>(data[threadIdx.x]);

    const unsigned mask = __ballot_sync(0xFFFFFFFF, true);

    unsigned mx = __reduce_max_sync(mask, r);
    atomicMax(max_value, mx);
}
```

When I run this with nvcc, it works, and with clang, I get an `an illegal instruction was encountered` message.

Compiler explorer confirms that they generate slightly different ptx.

nvcc
```
        ld.param.u64    %rd1, [testcode(float const*, unsigned int*)_param_0];
        ld.param.u64    %rd2, [testcode(float const*, unsigned int*)_param_1];
        cvta.to.global.u64      %rd3, %rd2;
        cvta.to.global.u64      %rd4, %rd1;
        mov.u32         %r1, %tid.x;
        mul.wide.u32    %rd5, %r1, 4;
        add.s64         %rd6, %rd4, %rd5;
        ld.global.f32   %f1, [%rd6];
        cvt.rzi.u32.f32         %r2, %f1;
        mov.pred        %p1, -1;
        mov.u32         %r3, -1;
        vote.sync.ballot.b32    %r4, %p1, %r3;
        redux.sync.max.u32 %r5, %r2, %r4;
        atom.global.max.u32     %r6, [%rd3], %r5;
        ret;
```

clang
```
        ld.param.u64    %rd1, [testcode(float const*, unsigned int*)_param_0];
        ld.param.u64    %rd2, [testcode(float const*, unsigned int*)_param_1];
        cvta.to.global.u64      %rd3, %rd2;
        cvta.to.global.u64      %rd4, %rd1;
        mov.u32         %r1, %tid.x;
        mul.wide.u32    %rd5, %r1, 4;
        add.s64         %rd6, %rd4, %rd5;
        ld.global.f32   %f1, [%rd6];
        cvt.rzi.u32.f32         %r2, %f1;
        mov.pred        %p1, -1;
        vote.sync.ballot.b32    %r3, %p1, -1;
        redux.sync.max.u32 %r4, %r3, %r2;
        atom.global.max.u32     %r5, [%rd3], %r4;
        ret;
```

I hacked around and got it to work with:
```
__global__ void testcode2(const float* data, unsigned int* max_value) {
    unsigned int r = static_cast<unsigned int>(data[threadIdx.x]);
    unsigned int mask = __ballot_sync(0xFFFFFFFF, true);
    unsigned int mx;                                           
    asm volatile(                               
        "        redux.sync.max.u32 %0, %2, %1;"
        : "=r"(mx) // Output operand (mask)     
        : "r"(mask), "r"(r)// No input operands          
        : "cc" // Clobbered registers and condition codes
    );                                                   
      
    atomicMax(max_value, mx);         
}      
```

The fix being to swap the mask and input on the redux.sync.

## Comments

### Comment 1 - AustinSchuh

Notes from debugging:

https://docs.nvidia.com/cuda/parallel-thread-execution/#parallel-synchronization-and-communication-instructions-redux-sync
https://docs.nvidia.com/cuda/cuda-c-programming-guide/#warp-reduce-synopsis

---

### Comment 2 - AustinSchuh

Looks like the following might fix it, unsure if it has other consequences:

```
diff --git a/llvm/lib/Target/NVPTX/NVPTXIntrinsics.td b/llvm/lib/Target/NVPTX/NVPTXIntrinsics.td
index 56d8b734bf01..2c1485893346 100644
--- a/llvm/lib/Target/NVPTX/NVPTXIntrinsics.td
+++ b/llvm/lib/Target/NVPTX/NVPTXIntrinsics.td
@@ -315,7 +315,7 @@ defm MATCH_ALLP_SYNC_64 : MATCH_ALLP_SYNC<Int64Regs, "b64", int_nvvm_match_all_s
 multiclass REDUX_SYNC<string BinOp, string PTXType, Intrinsic Intrin> {
   def : NVPTXInst<(outs Int32Regs:$dst), (ins Int32Regs:$src, Int32Regs:$mask),
           "redux.sync." # BinOp # "." # PTXType # " $dst, $src, $mask;",
-          [(set i32:$dst, (Intrin i32:$src, Int32Regs:$mask))]>,
+          [(set i32:$dst, (Intrin i32:$mask, Int32Regs:$src))]>,
         Requires<[hasPTX<70>, hasSM<80>]>;
 }
 
```

---

### Comment 3 - AustinSchuh

I'd like to get this backported to the clang 20 stable branch if that is reasonable, since clang 20 has this issue.  https://llvm.org/docs/GitHub.html#backporting-fixes-to-the-release-branches says I edit the milestone on the right first.  Looks like I don't have permission?

---

### Comment 4 - Artem-B

/cherry-pick 2d1517d

---

### Comment 5 - llvmbot

/pull-request llvm/llvm-project#134295

---

