# RISC-V backend does not support PreserveMost calling convention

**Author:** CaiWeiran
**URL:** https://github.com/llvm/llvm-project/issues/148147
**Status:** Closed
**Labels:** backend:RISC-V, crash
**Closed Date:** 2025-07-30T08:30:49Z

## Body

The RISC-V backend currently does not support the `PreserveMost` calling convention. Attempting to compile code that uses it results in a fatal error:

> fatal error: error in backend: Unsupported calling convention

A minimal example can be seen on Compiler Explorer:  
https://godbolt.org/z/h7e5WT7eM

This issue raises a few questions:

* Should the RISC-V backend implement support for `PreserveMost`, similar to x86, ARM, and AArch64?
* Alternatively, should the frontend (or mid-end) automatically lower unsupported calling conventions like `PreserveMost` to a more compatible one such as `C`?

It would be helpful to clarify whether support is planned or if users should explicitly avoid using this calling convention on RISC-V targets.

Thanks!

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-risc-v

Author: None (CaiWeiran)

<details>
The RISC-V backend currently does not support the `PreserveMost` calling convention. Attempting to compile code that uses it results in a fatal error:

&gt; fatal error: error in backend: Unsupported calling convention

A minimal example can be seen on Compiler Explorer:  
https://godbolt.org/z/h7e5WT7eM

This issue raises a few questions:

* Should the RISC-V backend implement support for `PreserveMost`, similar to x86, ARM, and AArch64?
* Alternatively, should the frontend (or mid-end) automatically lower unsupported calling conventions like `PreserveMost` to a more compatible one such as `C`?

It would be helpful to clarify whether support is planned or if users should explicitly avoid using this calling convention on RISC-V targets.

Thanks!
</details>


---

### Comment 2 - folkertdev

I'll add that many more targets support (or at least do not error on) `preserve_mostcc` (wasm, s390x, powerpc) making riscv kind of the odd one out.

This came up in https://github.com/rust-lang/rust/issues/143319.

---

### Comment 3 - wangpc-pp

Adding the support should be easy, but I am not sure which registers we should preserve, maybe x5-x31?

---

