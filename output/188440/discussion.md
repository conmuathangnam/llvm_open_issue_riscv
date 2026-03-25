# [AArch64][ISel] Miscompilation fptosi when cast float to char when fsigned-char

**Author:** hstk30-hw
**URL:** https://github.com/llvm/llvm-project/issues/188440
**Status:** Open
**Labels:** backend:AArch64, miscompilation, llvm:SelectionDAG

## Body

Code like:

```c
#include<stdint.h>
#include<stdlib.h>
#include <string.h>
#include <stdio.h>

typedef  char SMS_CHAR;
typedef  unsigned char SMS_UCHAR;
#define SMS_NULL_BYPE 0xff
typedef struct {
    float Lo;
    float Up;
} SA;

extern unsigned int Init(char*);

unsigned int test(SA *attr)
{
    SMS_CHAR aucData[2]= {0};
    SMS_UCHAR aucData1[8] = {0};
    unsigned int ulRet;
    aucData[0] = (SMS_CHAR)attr->Up;
    if (SMS_NULL_BYPE == (SMS_UCHAR)(aucData[0])) {
        aucData[0] = 65;
    }
    ulRet = Init(aucData);
    return ulRet;
}
```

https://godbolt.org/z/9ebf4T844

Generate assemble below:

```asm
test:
        sub     sp, sp, #32
        stp     x29, x30, [sp, #16]
        add     x29, sp, #16
        ldr     s0, [x0, #4]
        mov     w8, #65
        sub     x0, x29, #4
        sturh   wzr, [x29, #-4]
        fcvtzs  w9, s0                     // w9 not sign extension
        cmn     w9, #1
        csel    w8, w8, w9, eq
        sturb   w8, [x29, #-4]
        bl      Init
        ldp     x29, x30, [sp, #16]
        add     sp, sp, #32
        ret
```

## Comments

### Comment 1 - hstk30-hw

CC @davemgreen 

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-backend-aarch64

Author: hstk30-hw

<details>
Code like:

```
#include&lt;stdint.h&gt;
#include&lt;stdlib.h&gt;
#include &lt;string.h&gt;
#include &lt;stdio.h&gt;

typedef  char SMS_CHAR;
typedef  unsigned char SMS_UCHAR;
#define SMS_NULL_BYPE 0xff
typedef struct {
    float Lo;
    float Up;
} SA;

extern unsigned int Init(char*);

unsigned int test(SA *attr)
{
    SMS_CHAR aucData[2]= {0};
    SMS_UCHAR aucData1[8] = {0};
    unsigned int ulRet;
    aucData[0] = (SMS_CHAR)attr-&gt;Up;
    if (SMS_NULL_BYPE == (SMS_UCHAR)(aucData[0])) {
        aucData[0] = 65;
    }
    ulRet = Init(aucData);
    return ulRet;
}
```

https://godbolt.org/z/9ebf4T844

Generate assemble below:

```
test:
        sub     sp, sp, #<!-- -->32
        stp     x29, x30, [sp, #<!-- -->16]
        add     x29, sp, #<!-- -->16
        ldr     s0, [x0, #<!-- -->4]
        mov     w8, #<!-- -->65
        sub     x0, x29, #<!-- -->4
        sturh   wzr, [x29, #-4]
        fcvtzs  w9, s0                     // w9 not sign extension
        cmn     w9, #<!-- -->1
        csel    w8, w8, w9, eq
        sturb   w8, [x29, #-4]
        bl      Init
        ldp     x29, x30, [sp, #<!-- -->16]
        add     sp, sp, #<!-- -->32
        ret
```
</details>


---

### Comment 3 - davemgreen

Hi - It is UB in C to convert a float to an integer type that is outside of the range of the value. Is that what you are seeing?

---

### Comment 4 - zhangtianhao6

> Hi - It is UB in C to convert a float to an integer type that is outside of the range of the value. Is that what you are seeing?

Hi，Is there a way to check for such UB behavior before or during compilation?

---

### Comment 5 - dzaima

`-fsanitize=undefined` (or more specifically `-fsanitize=float-cast-overflow`) will warn you when it's encountered at runtime.

---

