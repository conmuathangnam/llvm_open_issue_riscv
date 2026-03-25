# Clang Frontend ICE

**Author:** zcorniere
**URL:** https://github.com/llvm/llvm-project/issues/56360
**Status:** Closed
**Labels:** clang:frontend, crash
**Closed Date:** 2025-01-31T16:36:07Z

## Body

Hi ! 
I just found a crash in clang-13 front end.

Here is the reduced source code + build script + output of clang: [Archive.zip](https://github.com/llvm/llvm-project/files/9034904/Archive.zip)
The source code is ill formed, but clang shouldn't crash.

### System info:
```
clang version 13.0.1
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
```

## Comments

### Comment 1 - EugeneZelenko

Could you please try `main`? https://godbolt.org should be helpful.

---

### Comment 2 - zcorniere

It also happen with clang 14 on [godbolt](https://godbolt.org/z/cn3zosbd7), currently cloning `main` to test

---

### Comment 3 - llvmbot

@llvm/issue-subscribers-clang-frontend

---

### Comment 4 - zcorniere

Also happend with [d71a8bb157ea9160f090c79aaffab87080bd210b](https://github.com/llvm/llvm-project/files/9035217/stderr.d71a8bb157ea9160f090c79aaffab87080bd210b.txt)


---

### Comment 5 - zcorniere

No longer occurs in clang 19.

---

### Comment 6 - shafik

looks like this was fixed in clang-19

---

