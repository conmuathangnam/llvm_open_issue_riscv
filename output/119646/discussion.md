# [clang] Miscompilation at -Os

**Author:** cardigan1008
**URL:** https://github.com/llvm/llvm-project/issues/119646
**Status:** Closed
**Labels:** miscompilation, llvm:transforms
**Closed Date:** 2025-01-23T08:23:07Z

## Body

This code prints 1 at -Os and 0 at -O0/1/2/3:
```c
int printf(const char *, ...);
long a, b = 208;
short c;
long(d)(long e) { return (a && e && 2036854775807 / e) * a; }
void f(long *e) {
  if (d(c | (*e = b || 0)))
    for (;;)
      ;
}
int main() {
  long *g = &b;
  *g = 0;
  f(&b);
  printf("%d\n", (int)b);
}
```

Compiler Explorer: https://godbolt.org/z/vPvna9s5Y 

Bisected to https://github.com/llvm/llvm-project/commit/ebe741fad07e3fda388d0fa44f256a07429cce6a which was committed by @haopliu 

## Comments

### Comment 1 - pinskia

Reduced down to just:
```
int b = 208;
[[gnu::noinline]]
void f(int *e, int a) {
  *e = !!b;
  if (a)
    __builtin_trap();
}
int main(void) {
  b = 0;
  f(&b, 0);
  if (b != 0)
    __builtin_trap();
}
```

---

### Comment 2 - nikic

I think the problem here is basically the same as the one in https://github.com/llvm/llvm-project/pull/119116#issuecomment-2535369091.

---

### Comment 3 - haopliu

Fixed the problem in https://github.com/llvm/llvm-project/pull/120044.

Will reenable the DSE improvement in https://github.com/llvm/llvm-project/pull/124058

---

