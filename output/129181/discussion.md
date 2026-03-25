# [clang] Miscompile at -O2/3

**Author:** cardigan1008
**URL:** https://github.com/llvm/llvm-project/issues/129181
**Status:** Closed
**Labels:** miscompilation, llvm:SelectionDAG
**Closed Date:** 2025-03-01T12:21:29Z

## Body

This code prints random value at `-O2/3` and 92 at `-O0/1`:

```c
int printf(const char *, ...);
int a, c, d, e;
int b[0];
int f() {
  d = 7;
  for (; 4 + d; d--)
    e += 92 & 1 << d;
  return e;
}
int main() {
  int g = f();
  printf("%d\n", g);
  c = b[g];
}
```

Compiler Explorer: https://godbolt.org/z/9oa95vEjY

It starts from clang-13. 

## Comments

### Comment 1 - dtcxzyw

```
> gcc -fsanitize=undefined test.c && ./a.out
test.c:7:17: runtime error: shift exponent -1 is negative
92
test.c:13:8: runtime error: load of address 0x5e373bbbc2ac with insufficient space for an object of type 'int'
0x5e373bbbc2ac: note: pointer points here
  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00
              ^
```


---

### Comment 2 - cardigan1008

Sorry for the above invalid case. I forgot to turn on the sanitizer check during the reduction.

I reduced this case with the check again and I got this case:

```c
int printf(const char *, ...);
static int a = -92, d, e, f;
int b = -91;
int c;
int g(int h, int i) {
  if (c)
    d = 1;
  else
    d = 0;
  e = 7;
  for (; d + h - 28 + e >= 0; e--) {
    f += i + a - 92 & 1 << (e + b - -91) ? 1 : 0;
    b = -91;
  }
  return f;
}
int main() {
  int j = g(14 + 13, 65);
  printf("%d\n", j);
}
```

It prints 2 at `-O2/3` but 1 at `-O0/1`

But it prints 2 at `-O2` when I turn on the `-fsanitize=undefined`. I'm not so sure if it is valid. 

Compiler Explorer: https://godbolt.org/z/jG6GPMxYP

---

### Comment 3 - dtcxzyw

Reproducer:
```
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@.str = constant [4 x i8] c"%d\0A\00"

define i32 @main() {
entry:
  %broadcast.splatinsert2 = insertelement <4 x i32> zeroinitializer, i32 0, i64 0
  %.not = icmp ult <4 x i32> %broadcast.splatinsert2, splat (i32 1)
  %.v = select <4 x i1> %.not, <4 x i32> zeroinitializer, <4 x i32> <i32 0, i32 0, i32 1, i32 poison>
  %0 = tail call i32 @llvm.vector.reduce.add.v4i32(<4 x i32> %.v)
  %call1 = tail call i32 (ptr, ...) @printf(ptr @.str, i32 %0)
  ret i32 0
}

declare i32 @printf(ptr, ...)

```

---

### Comment 4 - llvmbot


@llvm/issue-subscribers-backend-x86

Author: Yunbo Ni (cardigan1008)

<details>
This code prints random value at `-O2/3` and 92 at `-O0/1`:

```c
int printf(const char *, ...);
int a, c, d, e;
int b[0];
int f() {
  d = 7;
  for (; 4 + d; d--)
    e += 92 &amp; 1 &lt;&lt; d;
  return e;
}
int main() {
  int g = f();
  printf("%d\n", g);
  c = b[g];
}
```

Compiler Explorer: https://godbolt.org/z/9oa95vEjY

It starts from clang-13. 
</details>


---

