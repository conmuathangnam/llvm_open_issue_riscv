# [HLSL] Attribute vectors have different cast behavior

**Author:** V-FEXrt
**URL:** https://github.com/llvm/llvm-project/issues/142430
**Status:** Closed
**Labels:** clang:frontend, HLSL
**Closed Date:** 2025-06-17T20:32:12Z

## Body

https://godbolt.org/z/j1WPxaGKG

`uint32_t4` can be implicitly casted to `uint64_t4` but `__attribute__((__vector_size__(4 * sizeof(unsigned int)))) unsigned int` raises an error instead.

This causes issues with intrinsics such as `firstbitlow` which use the attribute type

DXC is perfectly fine with the implict cast for firstbitlow, while clang raises a compiler error


## Comments

### Comment 1 - V-FEXrt

I need to poke at this further and pin down more details

---

### Comment 2 - V-FEXrt

Alright so the core issue here is that the firstbit* intrinsics were using Vector instead of ExtVector. This kind of error is generally expected in that case. The reason it wasn't detected during lowering or testing is that both vectors lower to the same IR.

Thanks @llvm-beanz for the context!

Fix will be up in a second


---

### Comment 3 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Ashley Coleman (V-FEXrt)

<details>
https://godbolt.org/z/j1WPxaGKG

`uint32_t4` can be implicitly casted to `uint64_t4` but `__attribute__((__vector_size__(4 * sizeof(unsigned int)))) unsigned int` raises an error instead.

This causes issues with intrinsics such as `firstbitlow` which use the attribute type

DXC is perfectly fine with the implict cast for firstbitlow, while clang raises a compiler error

</details>


---

