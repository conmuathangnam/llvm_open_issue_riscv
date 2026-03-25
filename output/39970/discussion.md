# -mlong-calls generates non-PIC code on ARM

**Author:** glandium
**URL:** https://github.com/llvm/llvm-project/issues/39970
**Status:** Open
**Labels:** backend:ARM, bugzilla, miscompilation

## Body

|  |  |
| --- | --- |
| Bugzilla Link | [40624](https://llvm.org/bz40624) |
| Version | unspecified |
| OS | Linux |
| CC | @froydnj |

## Extended Description 
With the following source:
```
extern __attribute__((visibility("hidden"))) int foo();

int bar() { return foo(); };
```

clang generates the following with -fPIC -mlong-calls -O3 --target=armv7a-linux-gnueabi:
```
bar:
        ldr     r0, .LCPI0_0
        bx      r0
.LCPI0_0:
        .long   foo
```

That is not PIC, as that .LCPI0_0 label is in .text.

GCC generates the following:
```
bar:
        ldr     r3, .L3
.LPIC0:
        add     r3, pc, r3
        bx      r3
.L3:
        .word   foo-(.LPIC0+8)
```

(All courtesy of godbolt)

https://godbolt.org/z/gKwbiO

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-arm

Author: Mike Hommey (glandium)

<details>
|  |  |
| --- | --- |
| Bugzilla Link | [40624](https://llvm.org/bz40624) |
| Version | unspecified |
| OS | Linux |
| CC | @<!-- -->froydnj |

## Extended Description 
With the following source:
```
extern __attribute__((visibility("hidden"))) int foo();

int bar() { return foo(); };
```

clang generates the following with -fPIC -mlong-calls -O3 --target=armv7a-linux-gnueabi:
```
bar:
        ldr     r0, .LCPI0_0
        bx      r0
.LCPI0_0:
        .long   foo
```

That is not PIC, as that .LCPI0_0 label is in .text.

GCC generates the following:
```
bar:
        ldr     r3, .L3
.LPIC0:
        add     r3, pc, r3
        bx      r3
.L3:
        .word   foo-(.LPIC0+8)
```

(All courtesy of godbolt)

https://godbolt.org/z/gKwbiO
</details>


---

### Comment 2 - hstk30-hw

So, can I say -mlong-calls isn't currently compatible with position independent code in LLVM?

GCC seem ok, GCC will generate R_ARM_GOT_BREL or R_ARM_REL32 for some call.
I'm not sure which behavior is reasonable.

---

### Comment 3 - jiangxuezhi

Hi, I try to slove this problem using GOT, https://github.com/llvm/llvm-project/pull/147313
Maybe helpful for you~

e.g.
the command is clang -O2 -mlong-calls -fPIC

the code is:
#include <stdio.h>

int main()
{
    printf("hello");
    return 0;
}

the Relocation section is:
 Offset     Info    Type                Sym. Value  Symbol's Name
00000020  00000503 R_ARM_REL32            00000000   .L.str
00000024  00000760 R_ARM_GOT_PREL         00000000   printf

Relocation section '.rel.ARM.exidx' at offset 0x1a8 contains 1 entry:
 Offset     Info    Type                Sym. Value  Symbol's Name
00000000  0000022a R_ARM_PREL31           00000000   .text


---

