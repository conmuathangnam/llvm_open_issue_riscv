# [RISC-V] SLEEF vectorization fails due to incompatible scalar attribute mapping

**Author:** Ruhung
**URL:** https://github.com/llvm/llvm-project/issues/175129
**Status:** Closed
**Labels:** miscompilation, llvm:transforms
**Closed Date:** 2026-02-12T07:19:17Z

## Body

When using -fveclib=SLEEF to enable math function vectorization on RISC-V, I ran into the following issue.

The problem occurs when a function declares a signext argument, E.g.:

`define double @ldexp_f64_signext(double %x, i32 signext %exp)
`

When InjectTLIMappings maps this scalar function to a vector variant, the signext attribute is copied to the vector type. This is invalid and causes the LLVM verifier pass to fail.

I have a PR that attempts to fix this issue: https://github.com/llvm/llvm-project/pull/173206, I would appreciate feedback and discussion on whether this is the correct way to fix it.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-risc-v

Author: Rux124 (Ruhung)

<details>
When using -fveclib=SLEEF to enable math function vectorization on RISC-V, I ran into the following issue.

The problem occurs when a function declares a signext argument, E.g.:

`define double @<!-- -->ldexp_f64_signext(double %x, i32 signext %exp)
`

When InjectTLIMappings maps this scalar function to a vector variant, the signext attribute is copied to the vector type. This is invalid and causes the LLVM verifier pass to fail.

I have a draft PR that attempts to fix this issue: https://github.com/llvm/llvm-project/pull/173206, I would appreciate feedback and discussion on whether this is the correct way to fix it.
</details>


---

### Comment 2 - tclin914

https://github.com/llvm/llvm-project/pull/173206 has been merged.

---

