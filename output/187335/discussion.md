# AMDGPU SILoadStoreOptimizer: Asymmetric TFE/LWE check silently drops TFE on merged MIMG

**Author:** yijan4845
**URL:** https://github.com/llvm/llvm-project/issues/187335
**Status:** Open
**Labels:** backend:AMDGPU, miscompilation

## Body

**Incorrect GPU code generation (silent miscompile).** TFE (Trap Fetch Enable) causes an MIMG instruction to write an additional status DWORD to a VGPR, used by shaders implementing Partially Resident Textures (PRT) and sparse/tiled-resource memory patterns to detect out-of-bounds or unmapped tile accesses.

PoC Run pass result: [https://godbolt.org/z/Yhdzx6Gzz](https://godbolt.org/z/Yhdzx6Gzz)

Compile result difference: [https://godbolt.org/z/ojW8bYjv1](https://godbolt.org/z/ojW8bYjv1)

## Vulnerable code location(s)

[https://github.com/llvm/llvm-project/blob/9f7af289728002487b032a8278bc2c540a02ff59/llvm/lib/Target/AMDGPU/SILoadStoreOptimizer.cpp#L1014-L1024](https://github.com/llvm/llvm-project/blob/9f7af289728002487b032a8278bc2c540a02ff59/llvm/lib/Target/AMDGPU/SILoadStoreOptimizer.cpp#L1014-L1024)

[https://github.com/llvm/llvm-project/blob/9f7af289728002487b032a8278bc2c540a02ff59/llvm/lib/Target/AMDGPU/SILoadStoreOptimizer.cpp#L1597-L1604](https://github.com/llvm/llvm-project/blob/9f7af289728002487b032a8278bc2c540a02ff59/llvm/lib/Target/AMDGPU/SILoadStoreOptimizer.cpp#L1597-L1604)

## Vulnerable code analysis

`dmasksCanBeCombined()` has two cooperating defects:

1. **Asymmetric TFE/LWE guard** (lines 1014–1019): `TFEOp`/`LWEOp` are fetched only from `*CI.I`. `Paired.I` is never inspected. When `CI.TFE=0` and `Paired.TFE=1`, the guard does not fire.

2. **`tfe`/`lwe` absent from `OperandsToMatch`** (lines 1022–1024): The equality check covers `cpol`, `d16`, `unorm`, `da`, `r128`, `a16`, but not `tfe` or `lwe`, providing no fallback rejection.

`getInstSubclass()` returns `Info->BaseOpcode` for MIMG instructions (line 621), which is identical for TFE=0 and TFE=1 variants (TFE is an explicit immediate operand, not encoded in the opcode), so the pre-filter at lines 1284–1286 does not block the pair.

`mergeImagePair()` (lines 1603–1607) copies all non-DMask operands from `CI.I` only. The merged instruction therefore inherits `CI.TFE=0`, silently discarding `Paired`'s `TFE=1`.

**Confirmed by PoC:** `llc -mtriple=amdgcn -mcpu=gfx900 -run-pass si-load-store-opt poc.mir` merges two instructions with disjoint dmasks (1 and 14) differing only in TFE into a single `IMAGE_LOAD_V4_V4` with dmask=15 and TFE=0. No diagnostic is emitted.





## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-amdgpu

Author: Yihan Yang (yijan4845)

<details>
**Incorrect GPU code generation (silent miscompile).** TFE (Trap Fetch Enable) causes an MIMG instruction to write an additional status DWORD to a VGPR, used by shaders implementing Partially Resident Textures (PRT) and sparse/tiled-resource memory patterns to detect out-of-bounds or unmapped tile accesses.

PoC Run pass result: [https://godbolt.org/z/Yhdzx6Gzz](https://godbolt.org/z/Yhdzx6Gzz)

Compile result difference: [https://godbolt.org/z/ojW8bYjv1](https://godbolt.org/z/ojW8bYjv1)

## Vulnerable code location(s)

[https://github.com/llvm/llvm-project/blob/9f7af289728002487b032a8278bc2c540a02ff59/llvm/lib/Target/AMDGPU/SILoadStoreOptimizer.cpp#L1014-L1024](https://github.com/llvm/llvm-project/blob/9f7af289728002487b032a8278bc2c540a02ff59/llvm/lib/Target/AMDGPU/SILoadStoreOptimizer.cpp#L1014-L1024)

[https://github.com/llvm/llvm-project/blob/9f7af289728002487b032a8278bc2c540a02ff59/llvm/lib/Target/AMDGPU/SILoadStoreOptimizer.cpp#L1597-L1604](https://github.com/llvm/llvm-project/blob/9f7af289728002487b032a8278bc2c540a02ff59/llvm/lib/Target/AMDGPU/SILoadStoreOptimizer.cpp#L1597-L1604)

## Vulnerable code analysis

`dmasksCanBeCombined()` has two cooperating defects:

1. **Asymmetric TFE/LWE guard** (lines 1014–1019): `TFEOp`/`LWEOp` are fetched only from `*CI.I`. `Paired.I` is never inspected. When `CI.TFE=0` and `Paired.TFE=1`, the guard does not fire.

2. **`tfe`/`lwe` absent from `OperandsToMatch`** (lines 1022–1024): The equality check covers `cpol`, `d16`, `unorm`, `da`, `r128`, `a16`, but not `tfe` or `lwe`, providing no fallback rejection.

`getInstSubclass()` returns `Info-&gt;BaseOpcode` for MIMG instructions (line 621), which is identical for TFE=0 and TFE=1 variants (TFE is an explicit immediate operand, not encoded in the opcode), so the pre-filter at lines 1284–1286 does not block the pair.

`mergeImagePair()` (lines 1603–1607) copies all non-DMask operands from `CI.I` only. The merged instruction therefore inherits `CI.TFE=0`, silently discarding `Paired`'s `TFE=1`.

**Confirmed by PoC:** `llc -mtriple=amdgcn -mcpu=gfx900 -run-pass si-load-store-opt poc.mir` merges two instructions with disjoint dmasks (1 and 14) differing only in TFE into a single `IMAGE_LOAD_V4_V4` with dmask=15 and TFE=0. No diagnostic is emitted.




</details>


---

