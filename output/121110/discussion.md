# [clang] Miscompilation at -Os

**Author:** cardigan1008
**URL:** https://github.com/llvm/llvm-project/issues/121110
**Status:** Closed
**Labels:** miscompilation, vectorizers
**Closed Date:** 2024-12-28T09:54:36Z

## Body

This code prints 9 at `-Os` and 0 at `-O0/1/2/3`:

```c
int printf(const char *, ...);
char a = 9, c, d;
int b, e;
unsigned short f;
char *g = &a;
void h(int i) {
  for (; b; b++)
    c &= 0 <= i;
}
static short j(unsigned long i) {
  int k;
  for (; e + d + 4 > 0;) {
    k = i + 49;
    h(k + i - 52 + i);
    *g = 0;
    return 0;
  }
  return 0;
}
int main() {
  j(6 < (unsigned short)(f - 7) + f);
  printf("%d\n", a);
}
```

Compiler Explorer: https://godbolt.org/z/Mo5TEKh8r

Bisected to https://github.com/llvm/llvm-project/commit/5287299f8809ae927a0acafb179c4b37ce9ff21d, which was committed by @RKSimon 

## Comments

### Comment 1 - RKSimon

I'm not convinced the patch is responsible, it's more likely exposed something else.  I'll take a look after the Christmas break.

---

### Comment 2 - dtcxzyw

Reproducer: https://alive2.llvm.org/ce/z/jdkwFh
```
; bin/opt -passes=vector-combine test.ll -S
target triple = "x86_64-unknown-linux-gnu"

define <2 x i1> @src() {
entry:
  %0 = icmp samesign ugt <2 x i32> zeroinitializer, zeroinitializer
  %1 = icmp sgt <2 x i32> zeroinitializer, <i32 6, i32 -4>
  %2 = shufflevector <2 x i1> %0, <2 x i1> %1, <2 x i32> <i32 0, i32 3>
  ret <2 x i1> %2
}
```
```
define <2 x i1> @tgt() {
entry:
  ret <2 x i1> zeroinitializer
}
```
```

----------------------------------------
define <2 x i1> @src() {
entry:
  %#0 = icmp samesign ugt <2 x i32> { 0, 0 }, { 0, 0 }
  %#1 = icmp sgt <2 x i32> { 0, 0 }, { 6, 4294967292 }
  %#2 = shufflevector <2 x i1> %#0, <2 x i1> %#1, 0, 3
  ret <2 x i1> %#2
}
=>
define <2 x i1> @tgt() {
entry:
  ret <2 x i1> { 0, 0 }
}
Transformation doesn't verify!

ERROR: Value mismatch

Example:

Source:
<2 x i1> %#0 = < #x0 (0), #x0 (0) >
<2 x i1> %#1 = < #x0 (0), #x1 (1) >
<2 x i1> %#2 = < #x0 (0), #x1 (1) >

Target:
Source value: < #x0 (0), #x1 (1) >
Target value: < #x0 (0), #x0 (0) >
```


---

