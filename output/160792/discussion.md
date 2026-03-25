# Assertion "Request for a fixed element count on a scalable object" failed, vectorizer crashing with -Rpass=loop-vectorize

**Author:** namikukr-qc
**URL:** https://github.com/llvm/llvm-project/issues/160792
**Status:** Closed
**Labels:** vectorizers, crash
**Closed Date:** 2025-10-11T18:29:02Z

## Body

Ran into a crash recently with the vectorizer while trying to build something. Note that this crash only happens if -Rpass=loop-vectorize is enabled.

Reproducer: https://godbolt.org/z/Ms47v6Eea
Note: using the x86 trunk for the assertions build since there doesn't seem to one for armv8-a.

Can't seem to reproduce it while giving the resulting IR into llc to produce assembly.

Seems to be associated with commit 1858532c48cf5d93aa82966110fe1cada6ab6ba8  (tag: @fhahn ).

Source C code used to produce the issue:

```
void foo(int *a, int *b, int *c) {
  int i;
  int temp;
  int *ptr = a;

  long qval;

  for (i = 0; i < 64; i++) {
    qval = b[i];
    temp = c[i];
    if (temp < 0) {
      temp += qval >> 1;
      temp /= qval;
    }
    ptr[i] = temp;
  }
}

int main(int argc, char **argv) {
  int a[] = {5, 3, 4, 5, 6, 7, 6};
  int b[] = {0, 1, 2, 3, 4, 5, 6};
  int c[] = {1, 2, 3, 4, 5, 6, 7};
  foo(a, b, c);
  return 0;
}
```
`clang -march=armv8-a+sve -O3 -Rpass=loop-vectorize test.c -o test.s`

