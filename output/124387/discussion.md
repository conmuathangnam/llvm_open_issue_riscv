# [clang] Miscompile with -O2/3/s

**Author:** cardigan1008
**URL:** https://github.com/llvm/llvm-project/issues/124387
**Status:** Closed
**Labels:** miscompilation, llvm:instcombine, regression:19
**Closed Date:** 2025-01-25T21:35:13Z

## Body

This code prints random value at `-O2/3/s` and -1 at `-O0/1`:

```c
int printf(const char *, ...);
int a, b;
void c(char d) { a = d; }
int e(int d) {
  if (d < 0)
    return 1;
  return 0;
}
int f() {
  if (b)
    return 0;
  return 1;
}
int g(int d) {
  int h = 0;
  if (3 + d)
    h = f() - 1 - 1;
  return e(h) + h + h;
}
int main() {
  int i = g(0);
  c(i);
  printf("%d\n", a);
}
```

Compiler Explorer: https://godbolt.org/z/KfWsKxcvE

It starts from x86_64 clang 19.1.0. 

## Comments

### Comment 1 - MaskRay

InstCombine miscompiles 

```llvm
define i32 @src(i32 %x) local_unnamed_addr {
entry:
  %cmp = icmp eq i32 %x, 0
  %zext = zext i1 %cmp to i32
  %or = or disjoint i32 %zext, -2
  %fshl = call range(i32 -4, 2) i32 @llvm.fshl.i32(i32 %or, i32 %or, i32 1)
  %tr = trunc nsw i32 %fshl to i8
  %res = sext i8 %tr to i32
  ret i32 %res
}
```

edit: `g` has a Range return attribute, which is propagated to the call site in `main` after inlining.
InstCombine `InstCombinerImpl::SimplifyDemandedUseBits` simplifies the operand, but keeps the no-longer-valid Range return attribute.

Then, CorrelatedValuePropagationPass added nsw, and the value eventually becomes poison.

---

### Comment 2 - nikic

Slightly reduced: https://alive2.llvm.org/ce/z/3Kf7-2

I assume the problem is that we don't clear the return range attribute in SimplifyDemanded.

---

