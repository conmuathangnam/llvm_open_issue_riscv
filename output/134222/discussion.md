# Incorrect float conversion via __bf16 at -O0

**Author:** PenXLa
**URL:** https://github.com/llvm/llvm-project/issues/134222
**Status:** Closed
**Labels:** miscompilation, confirmed, llvm:SelectionDAG
**Closed Date:** 2025-10-22T10:55:36Z

## Body

# Description

When compiling with clang++ at -O0, a float -> __bf16 -> float conversion using a templated static_cast chain produces an incorrect result (0.0f instead of the expected value). This issue disappears when optimization is enabled (-O1 or higher).

# Reproduction:

Compile the following code with: `clang++ filename.cpp`

```cpp
#include <iostream>

template<typename D, typename S> D fp_cast(S x) {
    return static_cast<D>(x);
}

float do_cast(float x) {
    return fp_cast<float>(fp_cast<__bf16>(x));
}

int main() {
    float t0 = 3.f;
    float t2 = do_cast(t0);
    std::cout << t2 << std::endl;
    return 0;
}
```

*Expected behavior*: `do_cast(3.f)` should return `3.0f`.

*Actual behavior*: It returns `0.0f`.

# Environment

```txt
Ubuntu clang version 20.1.0 (++20250221063304+dc1bd6a8fa6a-1~exp1~20250221183420.63)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/lib/llvm-20/bin
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-x86

Author: None (PenXLa)

<details>
# Description

When compiling with clang++ at -O0, a float -&gt; __bf16 -&gt; float conversion using a templated static_cast chain produces an incorrect result (0.0f instead of the expected value). This issue disappears when optimization is enabled (-O1 or higher).

# Reproduction:

Compile the following code with: `clang++ filename.cpp`

```cpp
#include &lt;iostream&gt;

template&lt;typename D, typename S&gt; D fp_cast(S x) {
    return static_cast&lt;D&gt;(x);
}

float do_cast(float x) {
    return fp_cast&lt;float&gt;(fp_cast&lt;__bf16&gt;(x));
}

int main() {
    float t0 = 3.f;
    float t2 = do_cast(t0);
    std::cout &lt;&lt; t2 &lt;&lt; std::endl;
    return 0;
}
```

*Expected behavior*: `do_cast(3.f)` should return `3.0f`.

*Actual behavior*: It returns `0.0f`.

# Environment

```txt
Ubuntu clang version 20.1.0 (++20250221063304+dc1bd6a8fa6a-1~exp1~20250221183420.63)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/lib/llvm-20/bin
```
</details>


---

### Comment 2 - phoebewang

Seems a bug in fastisel: https://godbolt.org/z/ozPxzao5z

---

### Comment 3 - antoniofrighetto

IR:
```llvm
define float @do_cast(float %x) {
entry:
  %x.addr = alloca float, align 4
  store float %x, ptr %x.addr, align 4
  %0 = load float, ptr %x.addr, align 4
  %call = call bfloat @foo(float %0)
  %call1 = call float @bar(bfloat %call)
  ret float %call1
}

declare bfloat @foo(float)
declare float @bar(bfloat)
```

I'm finding this supposedly fast-path for FastISel super confusing. After examining the call-site `call float @bar(bfloat %call)` via SDAG (fallen back from FastISel), the partial DAG created is the following (which goes through combiners later):
```
t0: ch,glue = EntryToken
  t1: i64 = GlobalAddress<ptr @bar> 0
    t8: ch,glue = callseq_start t0, TargetConstant:i64<0>, TargetConstant:i64<0>
        t3: f32,ch = CopyFromReg t0, Register:f32 %3
      t5: bf16 = fp_round t3, TargetConstant:i64<1>
    t6: f16 = bitcast t5
  t10: ch,glue = CopyToReg t8, Register:f16 $xmm0, t6
  t13: ch,glue = X86ISD::CALL t10, TargetGlobalAddress:i64<ptr @bar> 0 [TF=7], Register:f16 $xmm0, RegisterMask:Untyped, t10:1
  t14: ch,glue = callseq_end t13, TargetConstant:i64<0>, TargetConstant:i64<0>, t13:1
  t16: f32,ch,glue = CopyFromReg t14, Register:f32 $xmm0, t14:1
```
We haven't examined the call-site of `foo` yet, but we have bothered creating a bunch of nodes for the argument of `bar`, i.e.:
```
  t3: f32,ch = CopyFromReg t0, Register:f32 %3
t5: bf16 = fp_round t3, TargetConstant:i64<1>
```
SDValue::getValue for the argument won't use getCopyFromRegs since not visited yet, thus we happen to end up in:

https://github.com/llvm/llvm-project/blob/8fddef8483dc9eb569580ffd13695b8f54d3c058/llvm/lib/CodeGen/SelectionDAG/SelectionDAGBuilder.cpp#L287-L302

---

### Comment 4 - johnplatts

I reported a similar issue involving the same codegen bug at issue #151692.

---

### Comment 5 - LuoYuanke

I tested the case with my local clang++ which is patched with https://github.com/llvm/llvm-project/pull/164565 and got below output `3`.

---

### Comment 6 - antoniofrighetto

> I tested the case with my local clang++ which is patched with [#164565](https://github.com/llvm/llvm-project/pull/164565) and got below output `3`.

It looks indeed fixed, thank you for looking into it!

---

