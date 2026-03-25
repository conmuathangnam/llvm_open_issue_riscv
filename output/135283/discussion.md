# [Mips] Presence of inline asm instructions affects ELF ABI info

**Author:** alexrp
**URL:** https://github.com/llvm/llvm-project/issues/135283
**Status:** Closed
**Labels:** backend:MIPS, miscompilation
**Closed Date:** 2025-07-28T01:07:52Z

## Body

```console
❯ cat test.ll
target datalayout = "E-m:m-p:32:32-i8:8:32-i16:16:32-i64:64-n32-S64"
target triple = "mips-linux-musleabi"

define dso_local void @entry() "use-soft-float"="true" {
  call void asm sideeffect "nop", ""()
  ret void
}
❯ llc --version | head -n2
LLVM (http://llvm.org/):
  LLVM version 20.1.2
❯ llc -filetype obj test.ll -O0
❯ readelf -A test.o

MIPS ABI Flags Version: 0

ISA: MIPS32
GPR size: 32
CPR1 size: 0
CPR2 size: 0
FP ABI: Soft float
ISA Extension: None
ASEs:
        None
FLAGS 1: 00000001
FLAGS 2: 00000000
❯ sed -i s/nop// test.ll
❯ llc -filetype obj test.ll -O0
❯ readelf -A test.o

MIPS ABI Flags Version: 0

ISA: MIPS32
GPR size: 32
CPR1 size: 32
CPR2 size: 0
FP ABI: Hard float (double precision)
ISA Extension: None
ASEs:
        None
FLAGS 1: 00000001
FLAGS 2: 00000000
```

Note that the resulting code is soft float in both cases. It's just the ELF ABI info that's wrong.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-mips

Author: Alex Rønne Petersen (alexrp)

<details>
```console
❯ cat test.ll
target datalayout = "E-m:m-p:32:32-i8:8:32-i16:16:32-i64:64-n32-S64"
target triple = "mips-linux-musleabi"

define dso_local void @<!-- -->entry() "use-soft-float"="true" {
  call void asm sideeffect "nop", ""()
  ret void
}
❯ llc --version | head -n2
LLVM (http://llvm.org/):
  LLVM version 20.1.2
❯ llc -filetype obj test.ll -O0
❯ readelf -A test.o

MIPS ABI Flags Version: 0

ISA: MIPS32
GPR size: 32
CPR1 size: 0
CPR2 size: 0
FP ABI: Soft float
ISA Extension: None
ASEs:
        None
FLAGS 1: 00000001
FLAGS 2: 00000000
❯ sed -i s/nop// test.ll
❯ llc -filetype obj test.ll -O0
❯ readelf -A test.o

MIPS ABI Flags Version: 0

ISA: MIPS32
GPR size: 32
CPR1 size: 32
CPR2 size: 0
FP ABI: Hard float (double precision)
ISA Extension: None
ASEs:
        None
FLAGS 1: 00000001
FLAGS 2: 00000000
```

Note that the resulting code is soft float in both cases. It's just the ELF ABI info that's wrong.
</details>


---

### Comment 2 - brad0

cc @yingopq @MaskRay 

---

