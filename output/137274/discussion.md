# AArch64 backend incorrectly lowers `mul` into `umull`

**Author:** ghost
**URL:** https://github.com/llvm/llvm-project/issues/137274
**Status:** Closed
**Labels:** backend:AArch64, miscompilation, llvm:SelectionDAG
**Closed Date:** 2025-05-01T07:59:00Z

## Body

The AArch64 backend incorrectly lowers `mul` into `umull` when a smaller than full-width pre/post index load precedes the instruction. Here is a minimum reproducible example that can be compiled with Clang:
```cpp
#include <cstdint>

uint64_t test(uint64_t *ptr) {
    uint64_t a = *ptr + 8;
    uint64_t b = *(uint32_t *)a;
    return a * b;
}
```

It outputs this assembly:
```asm
ldr     x8, [x0]
ldr     w9, [x8, #8]!
umull   x0, w8, w9
ret
```
This is incorrect as `umull` performs a 32-bit input to 64-bit output multiplication. The instruction should be `mul` for 64-bit inputs. [For reference, GCC and MSVC correctly compile it.](https://godbolt.org/z/jKnovWjr7)

The backend appears to think that the upper 32 bits of `x8` are zero, which triggers this rewrite:
```
def : Pat<(i64 (mul top32Zero:$Rn, top32Zero:$Rm)),
          (UMADDLrrr (EXTRACT_SUBREG $Rn, sub_32), (EXTRACT_SUBREG $Rm, sub_32), XZR)>;
```


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-aarch64

Author: None (vector542)

<details>
The AArch64 backend incorrectly lowers `mul` into `umull` when a smaller than full-width pre/post index load precedes the instruction. Here is a minimum reproducible example that can be compiled with Clang:
```cpp
#include &lt;cstdint&gt;

uint64_t test(uint64_t *ptr) {
    uint64_t a = *ptr + 8;
    uint64_t b = *(uint32_t *)a;
    return a * b;
}
```

It outputs this assembly:
```asm
ldr     x8, [x0]
ldr     w9, [x8, #<!-- -->8]!
umull   x0, w8, w9
ret
```
This is incorrect as `umull` performs a 32-bit input to 64-bit output multiplication. The instruction should be `mul` for 64-bit inputs. [For reference, GCC and MSVC correctly compile it.](https://godbolt.org/z/jKnovWjr7)

The backend appears to think that the upper 32 bits of `x8` are zero, which triggers this rewrite:
```
def : Pat&lt;(i64 (mul top32Zero:$Rn, top32Zero:$Rm)),
          (UMADDLrrr (EXTRACT_SUBREG $Rn, sub_32), (EXTRACT_SUBREG $Rm, sub_32), XZR)&gt;;
```

</details>


---

### Comment 2 - efriedma-quic

> The backend appears to think that the upper 32 bits of `x8` are zero, which triggers this rewrite:
> 
> ```
> def : Pat<(i64 (mul top32Zero:$Rn, top32Zero:$Rm)),
>           (UMADDLrrr (EXTRACT_SUBREG $Rn, sub_32), (EXTRACT_SUBREG $Rm, sub_32), XZR)>;
> ```

A post-increment load has two results, top32Zero checks if the first result has high bits zero, even if that's not the result which is actually getting used.  Not hard to rewrite the pattern, probably.

---

### Comment 3 - davemgreen

I was looking at these nodes the other day as they don't import for GISel at the moment. It would be good to have something that did work there too.

I was hoping that checking the number of output results from the SDNode was 1 would work, but that runs into issues with nodes that rightly have multiple results but the others are unimportant (like normal stores). Something like
```
 def top32Zero: PatLeaf<(i64 GPR64:$src), [{                                                         
-  return SDValue(N,0)->getValueType(0) == MVT::i64 &&                                               
+  return (N->getNumValues() == 1 || (N->getNumValues() == 2 && N->getValueType(1) == MVT::Other)) &&
+         N->getValueType(0) == MVT::i64 &&                                                          
          CurDAG->MaskedValueIsZero(SDValue(N,0), APInt::getHighBitsSet(64, 32));                    
   }]>;
```

@efriedma-quic what was your suggestion, something like `mul_with_top32Zero($Rn, $Rm)`? It looks like the list of affected PatFrags would be:
AArch64: top16Zero, top32Zero, topbitsallzero32, topbitsallzero64.
AMDGPU: uint5Bits
ARM: top16Zero, topbitsallzero32
RISCV: 33signbits_node
WebAssembly: bool_node

---

### Comment 4 - efriedma-quic

I was thinking of something like mul_with_top32Zero, yes.

I guess you could also do something like mul_using_first_result_of_operands, but that seems very ugly.

---

### Comment 5 - davemgreen

Because there are quite a few patterns with the same issue, I ended up plumbing through SDValue so that the correct operand is known. #137274.

---

