# Assertion failed: Result.isAbsent() && "local created multiple times"

**Author:** mmatic05
**URL:** https://github.com/llvm/llvm-project/issues/55163
**Status:** Closed
**Labels:** invalid, clang:frontend, incomplete, crash
**Closed Date:** 2025-02-07T14:43:20Z

## Body

Attached are images showing bug. The bug has existed since the change created by the commit ID (SHA) f7f2e4261a98b.

![open-source-llvm-bug (1)](https://user-images.githubusercontent.com/84574066/165737854-d4e086b1-15cc-43d3-9195-49075495fb05.png)


![Screenshot from 2022-04-28 12-40-32](https://user-images.githubusercontent.com/84574066/165737937-adca184d-dd39-444b-b754-630540b5ce59.png)

![Screenshot from 2022-04-28 12-40-36](https://user-images.githubusercontent.com/84574066/165737953-af191b72-9c1f-4cc0-9760-7b93cb2d94d7.png)



**Command line:** 
clang++ -DLLVM -DELF -Wa,--defsym,ELF=1 -DDP_TO_SP -o ./t_class_types_in_non-type_templ_param-9.s -I. -Ia -O4 -std=c++20 -DNEWFILEIO -no-integrated-as -S t_class_types_in_non-type_templ_param-9.i

**Used test:**
[test.zip](https://github.com/llvm/llvm-project/files/8582056/test.zip)


## Comments

### Comment 1 - llvmbot

@llvm/issue-subscribers-clang-frontend

---

### Comment 2 - cor3ntin

neither the image or the archive can be used to reproduce this bug.
Feel free to reopen if you have more information

---

