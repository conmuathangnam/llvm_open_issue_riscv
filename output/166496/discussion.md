# miscompile when IndVarSimplify turns a float induction variable into an int?

**Author:** regehr
**URL:** https://github.com/llvm/llvm-project/issues/166496
**Status:** Closed
**Labels:** miscompilation, llvm:transforms
**Closed Date:** 2025-11-11T09:31:56Z

## Body

IndVarSimplify can turn a float induction variable into an integer, but this isn't behavior-preserving when we get out of the range that can be represented exactly. it seems like an extra safety check on this transformation is needed.

```
~$ cat foo.c
void bar(void);

void foo(void) {
    for (float f = 25.0; f <= 100000000.0; f += 17.0)
        bar();
}
~$ cat bar.c
#include <stdio.h>

long cnt;

void bar(void) {
  ++cnt;
}

void foo(void);

int main(void) {
  foo();
  printf("%ld\n", cnt);
}
~$ clang -O0 foo.c bar.c && ./a.out
6188318
~$ clang -O1 foo.c bar.c && ./a.out
5882352
~$
```

## Comments

### Comment 1 - nelhage

Single-file reproducer via `__attribute__((noinline))` ([godbolt link](https://godbolt.org/z/965G167Y8)):

```c
#include <stdio.h>

unsigned long count;
void __attribute__((noinline)) bar() {
    count += 1;
}

void foo(void) {
    for (float f = 25.0; f <= 100000000.0; f += 17.0)
        bar();
}

int main() {
    foo();
    printf("%ld\n", count);
}
```

---

