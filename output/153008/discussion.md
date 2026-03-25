# [RISCV] Assert fail on loop-vectorize pass

**Author:** amitch1999
**URL:** https://github.com/llvm/llvm-project/issues/153008
**Status:** Closed
**Labels:** backend:RISC-V, crash
**Closed Date:** 2025-08-11T16:29:41Z

## Body

Hi, 

Compiling a test taken from gcc (`/gcc/gcc/testsuite/gcc.c-torture/compile/pr39943.c`) :
```
void gl_fog_index_pixels(float f, unsigned int n, unsigned int index[])
{ 
  unsigned int i;
  for (i=0; i<n; i++) 
    index[i] = (unsigned int) ((float) index[i] + (1.0F-f));
}
```

Compilation command:
`clang ./pr39943.c -march=rv64imfdc_zve64x -mabi=lp64d -c -O2`

Crashes on an assert:
```
/llvm-project/llvm/lib/Target/RISCV/RISCVTargetTransformInfo.cpp:1323: 
llvm::InstructionCost llvm::RISCVTTIImpl::getCastInstrCost(unsigned int, 
                                                           llvm::Type*,
                                                           llvm::Type*, 
                                                           llvm::TargetTransformInfo::CastContextHint,
                                                           llvm::TargetTransformInfo::TargetCostKind,
                                                           const llvm::Instruction*): 
Assertion `(SrcLT.first == 1) && (DstLT.first == 1) && "Illegal type"' failed.
```

I tried this on the current HEAD (`ccd35e5b2f1438646d11c27eaa475f34681f9cbe`)
And on 20.1.8 (`87f0227cb60147a26a1eeb4fb06e3b505e9c7261`)
The same happens on both.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-risc-v

Author: None (amitch1999)

<details>
Hi, 

Compiling a test taken from gcc (`/gcc/gcc/testsuite/gcc.c-torture/compile/pr39943.c`) :
```
void gl_fog_index_pixels(float f, unsigned int n, unsigned int index[])
{ 
  unsigned int i;
  for (i=0; i&lt;n; i++) 
    index[i] = (unsigned int) ((float) index[i] + (1.0F-f));
}
```

Compilation command:
`clang ./pr39943.c -march=rv64imfdc_zve64x -mabi=lp64d -c -O2`

Crashes on an assert:
```
/llvm-project/llvm/lib/Target/RISCV/RISCVTargetTransformInfo.cpp:1323: 
llvm::InstructionCost llvm::RISCVTTIImpl::getCastInstrCost(unsigned int, 
                                                           llvm::Type*,
                                                           llvm::Type*, 
                                                           llvm::TargetTransformInfo::CastContextHint,
                                                           llvm::TargetTransformInfo::TargetCostKind,
                                                           const llvm::Instruction*): 
Assertion `(SrcLT.first == 1) &amp;&amp; (DstLT.first == 1) &amp;&amp; "Illegal type"' failed.
```

I tried this on the current HEAD (`ccd35e5b2f1438646d11c27eaa475f34681f9cbe`)
And on 20.1.8 (`87f0227cb60147a26a1eeb4fb06e3b505e9c7261`)
The same happens on both.
</details>


---

### Comment 2 - amitch1999

Thanks @lukel97 !

---

### Comment 3 - lukel97

> Thanks @lukel97 !

Thanks for the easy reproducer :)

---

