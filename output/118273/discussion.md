# [Power Hardware] Codegen bug

**Author:** IshwaraK
**URL:** https://github.com/llvm/llvm-project/issues/118273
**Status:** Closed
**Labels:** backend:PowerPC, miscompilation
**Closed Date:** 2025-02-13T15:28:39Z

## Body

The Clang compiler generates wrong assembly for the below C code in Power target. The function arguments are `unsigned char` and doesn't appear to be honored in assembly code.

```c
#include <stdio.h>
#include <stdlib.h>

typedef unsigned char ub1;
void  oraSetRegInfo(ub1);

void __attribute__((noinline)) oraBar(ub1 var1, ub1 var2, ub1 var3, ub1 var4,
                    ub1 var5, ub1 var6, ub1 var7,
                    ub1 var8, ub1 var9)
{
    oraSetRegInfo(var9);
}

void __attribute__((noinline)) oraFoo() 
{
  ub1 var = 255;
  oraBar(10, 20, 30, 40, 50, 60, 70, 80, var);
}
```


Please see the assembly output here, https://godbolt.org/z/MPE5qoKsv 

In `oraBar()` function, the register `r3` is loaded with 32-bits wide value using `lwz` and then `oraSetRegInfo()` is called. 

But in `oraFoo()`, the `r3` is loaded with `li 11, 255` (`addi reg, 0, signed_immediate`) instruction, this means that the stored value in stack memory location doesn't guarantee to be unsigned 8-bits value. Thus, datatype of function args are not honored in this function call-chain and results into runtime wrong output.

The same C code is exercised in GCC and produces right assembly using `lbz` instruction in `oraBar()` function. Please see the results here, https://godbolt.org/z/a6PYcxjPY

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-powerpc

Author: Ishwara Sharma (IshwaraK)

<details>
The clang compiler generates wrong assembly for the below C code in Power target. The function arguments are 'unsigned char' and doesn't appear to be honored in assembly code.

```
#include &lt;stdio.h&gt;
#include &lt;stdlib.h&gt;

typedef unsigned char ub1;
void  oraSetRegInfo(ub1);

void __attribute__((noinline)) oraBar(ub1 var1, ub1 var2, ub1 var3, ub1 var4,
                    ub1 var5, ub1 var6, ub1 var7,
                    ub1 var8, ub1 var9)
{
    oraSetRegInfo(var9);
}

void __attribute__((noinline)) oraFoo() 
{
  ub1 var = 255;
  oraBar(10, 20, 30, 40, 50, 60, 70, 80, var);
}
```


Please see the assembly output here, https://godbolt.org/z/MPE5qoKsv 

In oraBar() function, the register r3 is loaded with 32 bits wide value using 'lwz' and then oraSetRegInfo() is called. 

But in oraFoo(), the r3 is loaded with 'li 11, 255' (addi reg, 0, signed_immediate) instruction, this means that the stored value in stack memory location doesn't guarantee to be unsigned 8 bits value. Thus, datatype of function args are not honored in this function call-chain and results into runtime wrong output.

The same C code is exercised in gcc and produces right assembly using 'lbz' instruction in oraBar() function. Please see the results here, https://godbolt.org/z/a6PYcxjPY




</details>


---

### Comment 2 - amy-kwan

This should be fixed by: https://github.com/llvm/llvm-project/commit/67eb05b2928ea707761bb040e6eb824f4ca9ef3a

---

