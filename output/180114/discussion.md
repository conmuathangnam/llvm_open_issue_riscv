# Segmentation fault      (core dumped) clang-tidy SparkFun_ProMicro.ino -- -x c++ -std=gnu++17 -DARDUINO=10607 -DARDUINO_ARCH_ESP32 -I$CORE_INC $VARIANT_INC -I$LIB_NTP -I$LIB_JSON $EXTRA_INCLUDES

**Author:** 6FXX001
**URL:** https://github.com/llvm/llvm-project/issues/180114
**Status:** Open
**Labels:** clang-tidy, obsolete, crash, needs-reduction

## Body

Per instructions, reporting the crash backtrace. 

[backtrace.txt](https://github.com/user-attachments/files/25118307/backtrace.txt)

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-tidy

Author: Grant (6FXX001)

<details>
Per instructions, reporting the crash backtrace. 

[backtrace.txt](https://github.com/user-attachments/files/25118307/backtrace.txt)
</details>


---

### Comment 2 - EugeneZelenko

What is Clang-Tidy version?

---

### Comment 3 - zeyi2

> What is Clang-Tidy version?

By looking at the backtrace, seems like 18.1

---

### Comment 4 - vbvictor

Looking at function offsets I suspect this is duplicate of https://github.com/llvm/llvm-project/issues/79505.

Please try clang-tidy-19 or later.

---

