# AMDGPU mishandles canonicalize with ieee mode disabled

**Author:** arsenm
**URL:** https://github.com/llvm/llvm-project/issues/139622
**Status:** Open
**Labels:** backend:AMDGPU, miscompilation

## Body

With the IEEE mode bit disabled (i.e. shaders or any function with the `"amdgpu-ieee"="false"` attribute, the canonicalize intrinsic is not implemented correctly. No floating point instruction will quiet a signaling nan in this mode. We need to do something else to ensure a signaling nan is quieted.

```
; llc -mtriple=amdgcn-amd-amdhsa -mcpu=gfx803 < %s | FileCheck -check-prefix=GFX8 %s
; llc -mtriple=amdgcn-amd-amdhsa -mcpu=gfx803 < %s | FileCheck -check-prefix=GFX9 %s

; These do not quiet with ieee=0
; GFX8 v_mul_f32_e32 v0, 1.0, v0
; GFX9: v_max_f32_e32 v0, v0, v0
define float @canonicalize(float %x) "amdgpu-ieee"="false" {
  %result = call float @llvm.canonicalize.f32(float %x)
  ret float %result
}

; For gfx9 (and gfx8 without daz/ftz), maybe can do this:
; s_mov_b32 s4, 0x7fc00000
; v_max_f32_e64 v0, v0, s4

; The documentation pseudocode suggests this is biased to return src1
; if src0 is a nan.

; For gfx8, this will still not flush if daz/ftz is enabled. Probably
; need to do both:
; s_mov_b32 s4, 0x7fc00000
; v_max_f32_e64 v0, v0, s4
; v_mul_f32_e32 v0, 1.0, v0

```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-amdgpu

Author: Matt Arsenault (arsenm)

<details>
With the IEEE mode bit disabled (i.e. shaders or any function with the `"amdgpu-ieee"="false"` attribute, the canonicalize intrinsic is not implemented correctly. No floating point instruction will quiet a signaling nan in this mode. We need to do something else to ensure a signaling nan is quieted.

```
; llc -mtriple=amdgcn-amd-amdhsa -mcpu=gfx803 &lt; %s | FileCheck -check-prefix=GFX8 %s
; llc -mtriple=amdgcn-amd-amdhsa -mcpu=gfx803 &lt; %s | FileCheck -check-prefix=GFX9 %s

; These do not quiet with ieee=0
; GFX8 v_mul_f32_e32 v0, 1.0, v0
; GFX9: v_max_f32_e32 v0, v0, v0
define float @<!-- -->canonicalize(float %x) "amdgpu-ieee"="false" {
  %result = call float @<!-- -->llvm.canonicalize.f32(float %x)
  ret float %result
}

; For gfx9 (and gfx8 without daz/ftz), maybe can do this:
; s_mov_b32 s4, 0x7fc00000
; v_max_f32_e64 v0, v0, s4

; The documentation pseudocode suggests this is biased to return src1
; if src0 is a nan.

; For gfx8, this will still not flush if daz/ftz is enabled. Probably
; need to do both:
; s_mov_b32 s4, 0x7fc00000
; v_max_f32_e64 v0, v0, s4
; v_mul_f32_e32 v0, 1.0, v0

```
</details>


---

### Comment 2 - jwanggit86

@arsenm Is this still a problem? It seems for the given example, with or without "amdgpu-ieee=false" the assembly files are identical, eg. for gfx903:
```
canonicalize:                           ; @canonicalize
; %bb.0:
        s_waitcnt vmcnt(0) expcnt(0) lgkmcnt(0)
        v_mul_f32_e32 v0, 1.0, v0
        s_setpc_b64 s[30:31]
.Lfunc_end0:
```

---

### Comment 3 - arsenm

Yes, that is the problem. With amdgpu-ieee false, that will not quiet a signaling nan. It will pass through, you can't implement this with floating point instructions 

---

### Comment 4 - jwanggit86

Based on my experiments on a GFX1030 GPU, when ieee-mode is off, the instruction sequence suggested above does not quiet a signaling NaN, ie
```
s_mov_b32 s4, 0x7fc00000
v_max_f32_e64 v0, v0, s4
```
When one of the operands is qNaN (eg 7fc00000) the v_max_f32 returns the other operand (This behavior is consistent with the pseudo-code for the instruction.). So, if the other operand is sNaN it will be returned. In other words, it won't be able to quiet the sNaN. 

I have also tested v_min_f32, v_add_f32, v_mul_f32. When ieee-mode is off, none of these instructions would quiet a signaling NaN. 

Looking at the LLVM Language Reference, the section on the `llvm.canonicalize.*` seems to suggest that the concept of "canonical encoding" itself is from IEEE-754. So, the question is, is "canonical encoding" still meaningful when ieee-mode is off? Your thoughts?


---

### Comment 5 - jwanggit86

Actually, should the `llvm.canonicalize` intrinsics be lowered to nop when ieee-mode is off?

---

### Comment 6 - arsenm

> Based on my experiments on a GFX1030 GPU, when ieee-mode is off, the instruction sequence suggested above does not quiet a signaling NaN, ie
> 
> ```
> s_mov_b32 s4, 0x7fc00000
> v_max_f32_e64 v0, v0, s4
> ```

Yes, that's the whole problem. This cannot be implemented with floating point instructions.

> 
> When one of the operands is qNaN (eg 7fc00000) the v_max_f32 returns the other operand (This behavior is consistent with the pseudo-code for the instruction.). So, if the other operand is sNaN it will be returned. In other words, it won't be able to quiet the sNaN.
> 
> I have also tested v_min_f32, v_add_f32, v_mul_f32. When ieee-mode is off, none of these instructions would quiet a signaling NaN.

Yes, no quieting happens with ieee=0

> 
> Looking at the LLVM Language Reference, the section on the `llvm.canonicalize.*` seems to suggest that the concept of "canonical encoding" itself is from IEEE-754. So, the question is, is "canonical encoding" still meaningful when ieee-mode is off? Your thoughts?

No, the standard IEEE formats have no non-canonical encodings. Non-canonical encodings are a target concept. For AMDGPU this only applies for denormal flushing, which the instructions will do regardless of IEEE mode. IEEE mode only breaks signaling nan quieting, which canonicalize is still required to do

> Actually, should the llvm.canonicalize intrinsics be lowered to nop when ieee-mode is off?

No. It needs to be a software expansion to quiet a signaling nan by forcing the quiet bit on. e.g.

```
  v_or_b32 v1, 0x400000, v0
  v_cmp_u_f32 vcc, 0, v0
  v_cndmask_b32 v0, v0, v1
```

---

