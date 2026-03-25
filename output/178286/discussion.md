# wasm-ld crash when linking large binaries

**Author:** FatihBAKIR
**URL:** https://github.com/llvm/llvm-project/issues/178286
**Status:** Closed
**Labels:** lld:wasm, crash
**Closed Date:** 2026-02-10T20:04:06Z

## Body

We’ve been running into some linker crashes, and after some debugging I narrowed it down to a signed integer overflow in this member: https://github.com/llvm/llvm-project/blob/main/lld/wasm/InputChunks.h#L100

```cpp
  int32_t outSecOff = 0;
```

This stores the next offset to write to, but when linking large binaries over 2GB, this member overflows into a negative number and results in the linker crashing. Considering wasm section sizes are specified to be a `u32`, I expect linking a 2GB binary to work.

Is the `int32_t` supposed to be load bearing? I looked at all the assignments and as far as I can tell none of them can have a legitimate negative value, only through overflows. If not, changing it to a `uint32_t` (or a `int64_t` for catching errors) resolve the issue for us.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-lld-wasm

Author: Fatih BAKIR (FatihBAKIR)

<details>
We’ve been running into some linker crashes, and after some debugging I narrowed it down to a signed integer overflow in this member: https://github.com/llvm/llvm-project/blob/main/lld/wasm/InputChunks.h#L100

```cpp
  int32_t outSecOff = 0;
```

This stores the next offset to write to, but when linking large binaries over 2GB, this member overflows into a negative number and results in the linker crashing. Considering wasm section sizes are specified to be a `u32`, I expect linking a 2GB binary to work.

Is the `int32_t` supposed to be load bearing? I looked at all the assignments and as far as I can tell none of them can have a legitimate negative value, only through overflows. If not, changing it to a `uint32_t` (or a `int64_t` for catching errors) resolve the issue for us.
</details>


---

