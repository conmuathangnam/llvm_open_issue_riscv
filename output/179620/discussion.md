# [clang][StaticAnalyzer] Crash when analyzing code importing C++20 modules built with coverage flags

**Author:** mrwsl
**URL:** https://github.com/llvm/llvm-project/issues/179620
**Status:** Closed
**Labels:** clang:modules, clang:static analyzer, coverage, crash
**Closed Date:** 2026-02-04T12:49:10Z

## Body

The Clang static analyzer crashes when analyzing code that imports C++20 modules **built with coverage instrumentation flags** (`-fprofile-instr-generate -fcoverage-mapping`).

Environment:

- **Clang version**: 21.1.6 (could also reproduce with Ubuntu clang version 21.1.8)
- **OS**: Linux
- **Standard**: C++26 (also reproducible with C++23)

There is a minimal reproducer attached. 

[minimal-standalone.zip](https://github.com/user-attachments/files/25064649/minimal-standalone.zip)

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-modules

Author: None (mrwsl)

<details>
The Clang static analyzer crashes when analyzing code that imports C++20 modules **built with coverage instrumentation flags** (`-fprofile-instr-generate -fcoverage-mapping`).

**No external dependencies required** - crashes with just standard library templates.

Environment:

- **Clang version**: 21.1.6 (could also reproduce with Ubuntu clang version 21.1.8)
- **OS**: Linux
- **Standard**: C++26 (also reproducible with C++23)

There is a minimal reproducer attached. 

[minimal-standalone.zip](https://github.com/user-attachments/files/25064649/minimal-standalone.zip)
</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-static-analyzer

Author: None (mrwsl)

<details>
The Clang static analyzer crashes when analyzing code that imports C++20 modules **built with coverage instrumentation flags** (`-fprofile-instr-generate -fcoverage-mapping`).

**No external dependencies required** - crashes with just standard library templates.

Environment:

- **Clang version**: 21.1.6 (could also reproduce with Ubuntu clang version 21.1.8)
- **OS**: Linux
- **Standard**: C++26 (also reproducible with C++23)

There is a minimal reproducer attached. 

[minimal-standalone.zip](https://github.com/user-attachments/files/25064649/minimal-standalone.zip)
</details>


---

### Comment 3 - steakhal

Could you try clang `main` if that reproduces the crash?
I build it just now, and it doesn't seem to crash.

---

### Comment 4 - mrwsl

You are correct. I could not reproduce the crash on `main`. Thanks! Do you know by any chance what commit fixed it? This one: 6c2caa6?

---

### Comment 5 - steakhal

> You are correct. I could not reproduce the crash on `main`. Thanks! Do you know by any chance what commit fixed it? This one: [6c2caa6](https://github.com/llvm/llvm-project/commit/6c2caa63d7d2929765199a66a61660f5372f01c7)?

I don't know, and unfortunately I don't have the time to bisect this.
If it wasn't fixed on main, but would have worked in some earlier clang version; then I would have bisected this to find the cause. But I guess, this way it's less interesting for me.

---

