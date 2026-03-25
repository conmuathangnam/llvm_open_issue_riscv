# Miscompile with [[clang::musttail]]

**Author:** gerben-stavenga
**URL:** https://github.com/llvm/llvm-project/issues/72390
**Status:** Closed
**Labels:** backend:X86, miscompilation, release:backport
**Closed Date:** 2026-01-14T22:58:20Z

## Body

When adding [[clang::musttail]] to a tail function call the program crashes. Inspection of the output asm clearly shows the stack return address is written over by a skewed memcpy, it copies the struct that is passed on the stack over itself but with a shift of 16 globbering the return address. Without [[clang::musttail] the function runs correctly and -fsanitize=undefined,asan does not indicate any problems

https://godbolt.org/z/Y7fq1xK54


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: None (gerben-stavenga)

<details>
When adding [[clang::musttail]] to a tail function call the program crashes. Inspection of the output asm clearly shows the stack return address is written over by a skewed memcpy, it copies the struct that is passed on the stack over itself but with a shift of 16 globbering the return address. Without [[clang::musttail] the function runs correctly and -fsanitize=undefined,asan does not indicate any problems

https://godbolt.org/z/Y7fq1xK54

</details>


---

### Comment 2 - gerben-stavenga

A simpler repro
https://godbolt.org/z/Ys5G6xnaz

at line 5 you see a copy over [rsp] which of course contains the return address

---

### Comment 3 - llvmbot


@llvm/issue-subscribers-backend-x86

Author: None (gerben-stavenga)

<details>
When adding [[clang::musttail]] to a tail function call the program crashes. Inspection of the output asm clearly shows the stack return address is written over by a skewed memcpy, it copies the struct that is passed on the stack over itself but with a shift of 16 globbering the return address. Without [[clang::musttail] the function runs correctly and -fsanitize=undefined,asan does not indicate any problems

https://godbolt.org/z/Y7fq1xK54

</details>


---

### Comment 4 - rnk

/cherry-pick 782bf6aff6ba6e9617bd3c4e27b3b9220ed5c850

---

### Comment 5 - llvmbot

<!--IGNORE-->
>/cherry-pick 782bf6aff6ba6e9617bd3c4e27b3b9220ed5c850

Error: Command failed due to missing milestone.

---

### Comment 6 - rnk

/cherry-pick https://github.com/llvm/llvm-project/commit/782bf6aff6ba6e9617bd3c4e27b3b9220ed5c850

---

### Comment 7 - llvmbot

/pull-request llvm/llvm-project#176470

---

