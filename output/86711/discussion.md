# [AArch64] Wrong code for i64 to f32 vector conversion

**Author:** hvdijk
**URL:** https://github.com/llvm/llvm-project/issues/86711
**Status:** Closed
**Labels:** backend:AArch64, miscompilation
**Closed Date:** 2025-08-12T11:52:52Z

## Body

Please consider:
```c++
#include <stdio.h>

typedef unsigned long ul;
typedef float f;

typedef unsigned long ul1 __attribute__((ext_vector_type(1)));
typedef float f1 __attribute__((ext_vector_type(1)));

__attribute__((noinline))
f converts(ul v) { return (f)v; }

__attribute__((noinline))
f1 convertv(ul1 v) { return __builtin_convertvector(v, f1); }

int main(void) {
  ul s = 0xffffff7fffffffff;
  ul1 v = 0xffffff7fffffffff;
  printf("%A %A\n", converts(s), convertv(v)[0]);
}
```
Expected output, and actual output with `clang -O0`:
```
0X1.FFFFFEP+63 0X1.FFFFFEP+63
```
Actual output with `clang -O3`:
```
0X1.FFFFFEP+63 0X1P+64
```
This happens because the vector conversion from i64 to f32 is performed as a vector conversion from i64 to f64, and then a conversion from f64 to f32. But both of these conversions round, and the first conversion does not preserve enough information for the second conversion to produce the correct result.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-aarch64

Author: Harald van Dijk (hvdijk)

<details>
Please consider:
```c++
#include &lt;stdio.h&gt;

typedef unsigned long ul;
typedef float f;

typedef unsigned long ul1 __attribute__((ext_vector_type(1)));
typedef float f1 __attribute__((ext_vector_type(1)));

__attribute__((noinline))
f converts(ul v) { return (f)v; }

__attribute__((noinline))
f1 convertv(ul1 v) { return __builtin_convertvector(v, f1); }

int main(void) {
  ul s = 0xffffff7fffffffff;
  ul1 v = 0xffffff7fffffffff;
  printf("%A %A\n", converts(s), convertv(v)[0]);
}
```
Expected output, and actual output with `clang -O0`:
```
0X1.FFFFFEP+63 0X1.FFFFFEP+63
```
Actual output with `clang -O3`:
```
0X1.FFFFFEP+63 0X1P+64
```
This happens because the vector conversion from i64 to f32 is performed as a vector conversion from i64 to f64, and then a conversion from f64 to f32. But both of these conversions round, and the first conversion does not preserve enough information for the second conversion to produce the correct result.
</details>


---

### Comment 2 - pinskia

I am 99% sure both answers are correct here.

---

### Comment 3 - hvdijk

> I am 99% sure both answers are correct here.

While that may be a correct answer from a C standard's perspective, it is not a correct answer in the context of LLVM. Excess precision is handled at the Clang level, it chooses to not use excess precision here, and even if it did there are options to turn that off. At the LLVM IR level, which is where the miscompilation happens, excess precision is only permitted in special cases that do not apply here.

More context: I saw this in the context of OpenCL, where the OpenCL CTS absolutely requires the result of 0X1.FFFFFEP+63, but an LLVM-based OpenCL implementation running on AArch64 produced the result of 0x1P+64.

---

### Comment 4 - efriedma-quic

Goes back to ef0d760cd9, looks like?  CC @TNorthover in case there's any additional context.

---

### Comment 5 - c-rhodes

fixed on trunk by #130665

---

