# [RISCV] PseudoVCPOP_M_B8_MASK transforms into incorrect machine code after RISCVVectorPeephole

**Author:** BeMg
**URL:** https://github.com/llvm/llvm-project/issues/122245
**Status:** Closed
**Labels:** backend:RISC-V, miscompilation
**Closed Date:** 2025-01-09T11:54:39Z

## Body

https://godbolt.org/z/4sxvzGoPG 

```
#include <iostream>
#include <riscv_vector.h>
using std::cerr;
using std::endl;
template <class T> bool __attribute__((noinline)) check(T, T) {}
unsigned long main_var_59;
int main() {
  unsigned long var_61;
  vbool8_t var_8 = __riscv_vmset_m_b8(7);
  unsigned long var_5 = __riscv_vcpop_m_b8_m(var_8, var_8, 6);
  vbool8_t var_19;
  if (check(var_5, var_61))
    cerr << endl;
  unsigned long var_4 = __riscv_vcpop_m_b8_m(var_8, var_19, 8);
  check(var_4, main_var_59);
}
```

```
# After RISC-V Vector Peephole Optimization
# Machine code for function main: IsSSA, TracksLiveness

...
*** Bad machine code: Too few operands ***
- function:    main
- basic block: %bb.7  (0x411b1a70)
- instruction: %20:gpr = PseudoVCPOP_M_B8 8, 0, debug-location !1042; example.cpp:14:25
4 operands expected, but 3 given.

*** Bad machine code: Expected a register operand. ***
- function:    main
- basic block: %bb.7  (0x411b1a70)
- instruction: %20:gpr = PseudoVCPOP_M_B8 8, 0, debug-location !1042; example.cpp:14:25
- operand 1:   8

```


Maybe this line https://github.com/llvm/llvm-project/blob/82d5dd28b4de7245088f7ed40da37f8cf80461e4/llvm/lib/Target/RISCV/RISCVVectorPeephole.cpp#L486 which comes from https://github.com/llvm/llvm-project/commit/129440728ce3e688969ed253a6709a5d50c5e850 is reason.



## Comments

### Comment 1 - BeMg

cc @lukel97 @preames

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-backend-risc-v

Author: Piyou Chen (BeMg)

<details>
https://godbolt.org/z/4sxvzGoPG 

```
#include &lt;iostream&gt;
#include &lt;riscv_vector.h&gt;
using std::cerr;
using std::endl;
template &lt;class T&gt; bool __attribute__((noinline)) check(T, T) {}
unsigned long main_var_59;
int main() {
  unsigned long var_61;
  vbool8_t var_8 = __riscv_vmset_m_b8(7);
  unsigned long var_5 = __riscv_vcpop_m_b8_m(var_8, var_8, 6);
  vbool8_t var_19;
  if (check(var_5, var_61))
    cerr &lt;&lt; endl;
  unsigned long var_4 = __riscv_vcpop_m_b8_m(var_8, var_19, 8);
  check(var_4, main_var_59);
}
```

```
# After RISC-V Vector Peephole Optimization
# Machine code for function main: IsSSA, TracksLiveness

...
*** Bad machine code: Too few operands ***
- function:    main
- basic block: %bb.7  (0x411b1a70)
- instruction: %20:gpr = PseudoVCPOP_M_B8 8, 0, debug-location !1042; example.cpp:14:25
4 operands expected, but 3 given.

*** Bad machine code: Expected a register operand. ***
- function:    main
- basic block: %bb.7  (0x411b1a70)
- instruction: %20:gpr = PseudoVCPOP_M_B8 8, 0, debug-location !1042; example.cpp:14:25
- operand 1:   8

```


Maybe this line https://github.com/llvm/llvm-project/blob/82d5dd28b4de7245088f7ed40da37f8cf80461e4/llvm/lib/Target/RISCV/RISCVVectorPeephole.cpp#L486 which comes from https://github.com/llvm/llvm-project/commit/129440728ce3e688969ed253a6709a5d50c5e850 is reason.


</details>


---

