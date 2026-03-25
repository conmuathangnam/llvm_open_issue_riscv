# [clang] Miscompilation at -O2/3

**Author:** cardigan1008
**URL:** https://github.com/llvm/llvm-project/issues/128528
**Status:** Closed
**Labels:** miscompilation, llvm:SelectionDAG
**Closed Date:** 2025-02-25T14:03:19Z

## Body

This code prints 3333 at -O0/1 and -2147483648 at -O2/3:

```c
int printf(const char *, ...);
int a, b, c;
long double d;
int f(int g, int i) {
  int e = g + i;
  e++;
  return e;
}
int j() {
  int h = -1;
  if (b)
    return h;
  return 0;
}
void k(int g) { a = -66 + g - 38; }
int main() {
  int l, m = j();
  l = f(m + 34, m + 80) + m - 112 + m;
  k(l + 38 - 37 + 38);
  d = 4 + a - -66;
  a = c = d * 4.16666666666666666019e02;
  printf("%d\n", c);
}
```

Compiler Explorer: https://godbolt.org/z/TnK1bWcGd

Bisected to https://github.com/llvm/llvm-project/commit/15a7de697ae5ad88fd96ef7dc39ac479cc6e2eaf, which was committed by @goldsteinn 

## Comments

### Comment 1 - goldsteinn

I don't have time to look at this now, I have no objections to a revert.

Likely the issue is 1) incorrect `nneg` applied in the middle end or 2) this is exposing an incorrect transform.

@RKSimon 

---

### Comment 2 - dtcxzyw

Reduced Reproducer:
```
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@b = global i32 0
@.str = constant [4 x i8] c"%d\0A\00"

define i32 @main() {
entry:
  %0 = load i32, ptr @b, align 4
  %tobool.not.i.not = icmp eq i32 %0, 0
  %sub9 = select i1 %tobool.not.i.not, i32 8, i32 1
  %conv = uitofp i32 %sub9 to x86_fp80
  %mul = fmul x86_fp80 %conv, 0xK4007D055555555555800
  %conv10 = fptosi x86_fp80 %mul to i32
  %call11 = tail call i32 (ptr, ...) @printf(ptr @.str, i32 %conv10)
  ret i32 0
}

declare i32 @printf(ptr, ...)
```

---

### Comment 3 - llvmbot


@llvm/issue-subscribers-backend-x86

Author: Yunbo Ni (cardigan1008)

<details>
This code prints 3333 at -O0/1 and -2147483648 at -O2/3:

```c
int printf(const char *, ...);
int a, b, c;
long double d;
int f(int g, int i) {
  int e = g + i;
  e++;
  return e;
}
int j() {
  int h = -1;
  if (b)
    return h;
  return 0;
}
void k(int g) { a = -66 + g - 38; }
int main() {
  int l, m = j();
  l = f(m + 34, m + 80) + m - 112 + m;
  k(l + 38 - 37 + 38);
  d = 4 + a - -66;
  a = c = d * 4.16666666666666666019e02;
  printf("%d\n", c);
}
```

Compiler Explorer: https://godbolt.org/z/TnK1bWcGd

Bisected to https://github.com/llvm/llvm-project/commit/15a7de697ae5ad88fd96ef7dc39ac479cc6e2eaf, which was committed by @<!-- -->goldsteinn 
</details>


---

