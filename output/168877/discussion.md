# [X86][GISel] Wrong code at -O0

**Author:** XChy
**URL:** https://github.com/llvm/llvm-project/issues/168877
**Status:** Open
**Labels:** backend:X86, miscompilation, llvm:globalisel, confirmed

## Body

Testcase:
```c
#include "stdio.h"
#include "stdint.h"
#include "limits.h"

int32_t g_21[5][2];
int32_t g_2121[];

int32_t func_14(int32_t, int64_t, int32_t, int32_t *, int8_t);
int32_t func_1() {
  int32_t *l_20 = &g_21[4][1];
  uint64_t l_2506 = 4;
  *l_20 = func_14(0, 0, 2, &g_2121[0], l_2506) - 1;
  return 0;
}
int32_t func_14(int32_t, int64_t, int32_t, int32_t *p_18, int8_t) {
  return *p_18;
}
int main() {
  func_1();
  printf("checksum = %08X\n", g_21[4][1]);
}
```

Reproducer command: `clang -fglobal-isel -O0 testcase.c`
output: `checksum = 0041995F` (the output is indeterministic)

When compiling with GCC or Clang with other optimization flags, the output is `checksum = FFFFFFFF`

Somehow, I cannot reproduce it on Godbolt. However, I can reproduce at trunk commit 76f1949cfa0d8fed73c153af83a7054073686506 and clang-17, and on both processors `Intel(R) Xeon(R) Gold 5118 CPU @ 2.30GHz` and `12th Gen Intel(R) Core(TM) i7-12700H`.


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-x86

Author: Hongyu Chen (XChy)

<details>
Testcase
```c
#include "stdio.h"
#include "stdint.h"
#include "limits.h"

int32_t g_21[5][2];
int32_t g_2121[];

int32_t func_14(int32_t, int64_t, int32_t, int32_t *, int8_t);
int32_t func_1() {
  int32_t *l_20 = &amp;g_21[4][1];
  uint64_t l_2506 = 4;
  *l_20 = func_14(0, 0, 2, &amp;g_2121[0], l_2506) - 1;
  return 0;
}
int32_t func_14(int32_t, int64_t, int32_t, int32_t *p_18, int8_t) {
  return *p_18;
}
int main() {
  func_1();
  printf("checksum = %08X\n", g_21[4][1]);
}
```

Reproducer command: `clang -fglobal-isel -O0 testcase.c`
output: `checksum = 0041995F` (the output is indeterministic)

When compiling with GCC and clang with other optimization flags, the output is `checksum = FFFFFFFF`

Somehow, I cannot reproduce it on Godbolt. But I can reproduce on trunk commit 76f1949cfa0d8fed73c153af83a7054073686506 and clang-17.
And I can reproduce on both processors `Intel(R) Xeon(R) Gold 5118 CPU @ 2.30GHz` and `12th Gen Intel(R) Core(TM) i7-12700H`.

</details>


---

### Comment 2 - s-barannikov

It reads past the end of g_2121 array, this is an undefined behavior.


---

### Comment 3 - XChy

@s-barannikov Do you mean `g_2121[0]` is an out-of-bound access? The Clang warning told me:
```
<source>:6:9: warning: tentative array definition assumed to have one element [-Wtentative-definition-array]
    6 | int32_t g_2121[];
      |         ^
```
And this code snippet passes the UBSan, MemorySanitizer of Clang, and also passes the UBSan, ASan of GCC.

---

### Comment 4 - XChy

IR under clang -O0: https://godbolt.org/z/38f3s99q5

---

### Comment 5 - s-barannikov

> [@s-barannikov](https://github.com/s-barannikov) Do you mean `g_2121[0]` is an out-of-bound access? The Clang warning told me:

Yeah, I did. I didn't know it will assume there is one element.


---

### Comment 6 - s-barannikov

https://godbolt.org/z/44vcTrMvh

```
        lea     rcx, [rip + g_2121]
        mov     qword ptr [rbp - 8], r8
        mov     qword ptr [rbp - 16], rax
        mov     rax, qword ptr [rbp - 16]
        mov     r8d, eax
        mov     cl, 24
        shl     r8d, 24
        sar     r8d, cl
        call    func_14
```

Here, `rcx` is `&g_2121[0]` passed to `func_14`, but it gets clobbered by `mov cl, 24` before the call.


---

### Comment 7 - e-kud

Looks like a duplicate of https://github.com/llvm/llvm-project/issues/37654 since it was closed as obsolete, let's keep this one as actual one.

---

### Comment 8 - s-barannikov

Here is a small reproducer:
```LLVM
declare void @foo(i32, i32, i32, i32, i8 signext)
define void @test(i8 %x) {
  call void @foo(i32 1, i32 2, i32 3, i32 4, i8 signext %x)
  ret void
}
```
`llc test1.l -o - -global-isel -O0 -x86-asm-syntax=intel`

```
	push	rax
	.cfi_def_cfa_offset 16
	mov	r8d, edi
	mov	edi, 1
	mov	esi, 2
	mov	edx, 3
	mov	ecx, 4
	mov	cl, 24
	shl	r8d, 24
	sar	r8d, cl
	call	foo
	pop	rax
	.cfi_def_cfa_offset 8
	ret
```


---

### Comment 9 - e-kud

@s-barannikov yeah, it is exactly the code from the #37654 :)

---

### Comment 10 - XChy

Oops, that sounds a pity. Is x86 GlobalISel planned to be maintained in the future? If not, we will stop testing it...

---

### Comment 11 - RKSimon

Its a long term project - but its not expected to work for anything but the simplest cases at the moment. Patches welcome :) 

---

### Comment 12 - topperc

We should be using constant shifts here. The problem seems to be the extra copy in this MIR after the legalizer.

```
  %11:_(s8) = G_CONSTANT i8 25
  %8:_(s32) = G_SHL %1:_, %11:_(s8)
  %10:_(s8) = COPY %11:_(s8)
  %7:_(s32) = G_ASHR %8:_, %10:_(s8)
  $r8d = COPY %7:_(s32)
```

Both shifts started out with an i32 shift amount. Each got legalized by inserting a G_TRUNC for the shift. This wasn't CSEd because of -O0. Then we legalize the other truncate and CSE the constant created by the previous G_TRUNC by using a COPY.

InstructionSelection is unable to peek through this COPY to find the shift amount.

For this specific case, we should be using MOVSX which would side step the issue, but it would still exist for arguments that aren't s8, s16, s32 and need to be extended.

---

### Comment 13 - topperc

I tried to make G_SEXT_INREG legal for 8, 16, and 32 extension size, but not all of sext_inreg patterns from SDAG are importing.

---

