# Miscompilation: Missing prefixes for emulatedTLS variables on macOS/ARM64 with -O0

**Author:** sschaller
**URL:** https://github.com/llvm/llvm-project/issues/126200
**Status:** Closed
**Labels:** backend:AArch64, miscompilation, platform:macos
**Closed Date:** 2025-03-01T15:47:27Z

## Body

Reproducer:
https://alive2.llvm.org/ce/z/6Xr9_c

```cpp
struct T {
    T() {} // required
    int a = 0;
};

thread_local T aa;

int main() {
    return 0;
}
```
Trying to compile this code on macOS / ARM64 with `clang -stdlib=libc++ -std=c++17 -femulated-tls main.cpp`
leads to a linker error for undefined symbols `___tls_guard` and `_aa` (4-5, 60-61 in the reproducer).

Inspecting the generated assembly code shows that both symbols are used as operands instead of the prefixed variables `___emutls_v.__tls_guard` and `___emutls_v.aa`.

Compiling with `-O1` correctly prefixes the operands and fixes the issue.

I'm not quite sure what is missing and would appreciate any pointers.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-aarch64

Author: Sebastian Schaller (sschaller)

<details>
Reproducer:
https://alive2.llvm.org/ce/z/6Xr9_c

```cpp
struct T {
    T() {} // required
    int a = 0;
};

thread_local T aa;

int main() {
    return 0;
}
```
Trying to compile this code on macOS / ARM64 with `clang -stdlib=libc++ -std=c++17 -femulated-tls main.cpp`
leads to a linker error for undefined symbols `___tls_guard` and `_aa` (4-5, 60-61 in the reproducer).

Inspecting the generated assembly code shows that both symbols are used as operands instead of the prefixed variables `___emutls_v.__tls_guard` and `___emutls_v.aa`.

Compiling with `-O1` correctly prefixes the operands and fixes the issue.

I'm not quite sure what is missing and would appreciate any pointers.
</details>


---

### Comment 2 - davemgreen

It sounds like it is not implemented for GlobalISel (GISel is used at -O0, SDAG for higher optimization levels). See 01eb869114bd307e879cdbf7607e7fb546c7d31a for where that was implemented, vs AArch64TargetLowering::LowerGlobalTLSAddress that has the option to use TargetLowering::LowerToTLSEmulatedModel.

---

### Comment 3 - sschaller

Hi @davemgreen I just wanted to say thank you for pinpointing the issue. I probably wouldn't have found it by myself.
I created a PR with a fix that will fall back to SDAG in this case. I shortly tried to implement it, but quickly found myself out of depth.

---

