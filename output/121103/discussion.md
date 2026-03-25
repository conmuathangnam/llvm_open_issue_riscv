# [EntryExitInstrumenter][AArch64][RISCV][Loongarch] return address should be passed into `_mcount`

**Author:** dtcxzyw
**URL:** https://github.com/llvm/llvm-project/issues/121103
**Status:** Closed
**Labels:** miscompilation, llvm:transforms
**Closed Date:** 2025-01-01T07:02:09Z

## Body

Reproducer:
```
; bin/clang -pg test.c && ./a.out && gprof -b a.out gmon.out
int fib(int n) {return n <=1 ? 1 : fib(n-1) + fib(n-2); }
int main() {fib(10); return 0;}
```
The call graph is missing if it is compiled with clang:
```
Flat profile:

Each sample counts as 0.01 seconds.
 no time accumulated

  %   cumulative   self              self     total           
 time   seconds   seconds    calls  Ts/call  Ts/call  name
```
With gcc, it works as expected:
```
Flat profile:

Each sample counts as 0.01 seconds.
 no time accumulated

  %   cumulative   self              self     total           
 time   seconds   seconds    calls  Ts/call  Ts/call  name    
  0.00      0.00     0.00        1     0.00     0.00  fib


                        Call graph


granularity: each sample hit covers 2 byte(s) no time propagated

index % time    self  children    called     name
                                 176             fib [1]
                0.00    0.00       1/1           main [6]
[1]      0.0    0.00    0.00       1+176     fib [1]
                                 176             fib [1]
-----------------------------------------------


Index by function name

   [1] fib
```
This issue can be reproduced on AArch64/RISCV/Loongarch platforms.

After investing this issue, I found that we should pass the return address into `_mcount`:
https://github.com/gcc-mirror/gcc/blob/c6b7d0343f2c0eb3102341c9326fe314a047bee6/gcc/config/riscv/riscv.h#L466-L473
https://github.com/bminor/glibc/blob/757ac24f8f2344e5f8afd2465c808d0f6adcc948/sysdeps/riscv/machine-gmon.h

This issue was originally reported by @wzssyqa.


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-risc-v

Author: Yingwei Zheng (dtcxzyw)

<details>
Reproducer:
```
; bin/clang -pg test.c &amp;&amp; ./a.out &amp;&amp; gprof -b a.out gmon.out
int fib(int n) {return n &lt;=1 ? 1 : fib(n-1) + fib(n-2); }
int main() {fib(10); return 0;}
```
The call graph is missing if it is compiled with clang:
```
Flat profile:

Each sample counts as 0.01 seconds.
 no time accumulated

  %   cumulative   self              self     total           
 time   seconds   seconds    calls  Ts/call  Ts/call  name
```
With gcc, it works as expected:
```
Flat profile:

Each sample counts as 0.01 seconds.
 no time accumulated

  %   cumulative   self              self     total           
 time   seconds   seconds    calls  Ts/call  Ts/call  name    
  0.00      0.00     0.00        1     0.00     0.00  fib


                        Call graph


granularity: each sample hit covers 2 byte(s) no time propagated

index % time    self  children    called     name
                                 176             fib [1]
                0.00    0.00       1/1           main [6]
[1]      0.0    0.00    0.00       1+176     fib [1]
                                 176             fib [1]
-----------------------------------------------


Index by function name

   [1] fib
```
This issue can be reproduced on AArch64/RISCV/Loongarch platforms.

After investing this issue, I found that we should pass the return address into `_mcount`:
https://github.com/gcc-mirror/gcc/blob/c6b7d0343f2c0eb3102341c9326fe314a047bee6/gcc/config/riscv/riscv.h#L466-L473
https://github.com/bminor/glibc/blob/757ac24f8f2344e5f8afd2465c808d0f6adcc948/sysdeps/riscv/machine-gmon.h

This issue was originally reported by @<!-- -->wzssyqa.

</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-backend-aarch64

Author: Yingwei Zheng (dtcxzyw)

<details>
Reproducer:
```
; bin/clang -pg test.c &amp;&amp; ./a.out &amp;&amp; gprof -b a.out gmon.out
int fib(int n) {return n &lt;=1 ? 1 : fib(n-1) + fib(n-2); }
int main() {fib(10); return 0;}
```
The call graph is missing if it is compiled with clang:
```
Flat profile:

Each sample counts as 0.01 seconds.
 no time accumulated

  %   cumulative   self              self     total           
 time   seconds   seconds    calls  Ts/call  Ts/call  name
```
With gcc, it works as expected:
```
Flat profile:

Each sample counts as 0.01 seconds.
 no time accumulated

  %   cumulative   self              self     total           
 time   seconds   seconds    calls  Ts/call  Ts/call  name    
  0.00      0.00     0.00        1     0.00     0.00  fib


                        Call graph


granularity: each sample hit covers 2 byte(s) no time propagated

index % time    self  children    called     name
                                 176             fib [1]
                0.00    0.00       1/1           main [6]
[1]      0.0    0.00    0.00       1+176     fib [1]
                                 176             fib [1]
-----------------------------------------------


Index by function name

   [1] fib
```
This issue can be reproduced on AArch64/RISCV/Loongarch platforms.

After investing this issue, I found that we should pass the return address into `_mcount`:
https://github.com/gcc-mirror/gcc/blob/c6b7d0343f2c0eb3102341c9326fe314a047bee6/gcc/config/riscv/riscv.h#L466-L473
https://github.com/bminor/glibc/blob/757ac24f8f2344e5f8afd2465c808d0f6adcc948/sysdeps/riscv/machine-gmon.h

This issue was originally reported by @<!-- -->wzssyqa.

</details>


---

### Comment 3 - dtcxzyw

@wzssyqa Please file a separate issue for MIPS target.


---

### Comment 4 - SixWeining

Are the 2 arguments (`frompc` and `selfpc`) to `mcount_internal` the same? Both are `__builtin_return_address(0)`?

---

### Comment 5 - dtcxzyw

> Are the 2 arguments (`frompc` and `selfpc`) to `mcount_internal` the same? Both are `__builtin_return_address(0)`?

No. Please refer to the generic implementation: https://github.com/bminor/glibc/blob/757ac24f8f2344e5f8afd2465c808d0f6adcc948/sysdeps/generic/machine-gmon.h.
`frompc` should be `__builtin_return_address(1)`. Since `frompc` is passed from the caller of `_mcount` on these platforms, we should use `__builtin_return_address(0)` instead.



---

### Comment 6 - SixWeining

> > Are the 2 arguments (`frompc` and `selfpc`) to `mcount_internal` the same? Both are `__builtin_return_address(0)`?
> 
> No. Please refer to the generic implementation: https://github.com/bminor/glibc/blob/757ac24f8f2344e5f8afd2465c808d0f6adcc948/sysdeps/generic/machine-gmon.h. `frompc` should be `__builtin_return_address(1)`. Since `frompc` is passed from the caller of `_mcount` on these platforms, we should use `__builtin_return_address(0)` instead.

Oh I see. Thanks.

---

