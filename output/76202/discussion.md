# Miscompile after f5687636415969e6d945659a0b78734abdfb0f06

**Author:** vitalybuka
**URL:** https://github.com/llvm/llvm-project/issues/76202
**Status:** Closed
**Labels:** backend:AArch64, miscompilation
**Closed Date:** 2025-06-18T10:04:51Z

## Body

https://lab.llvm.org/buildbot/#/builders/238/builds/7108



## Comments

### Comment 1 - vitalybuka

Local bisect points to f5687636415969e6d945659a0b78734abdfb0f06.

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-backend-aarch64

Author: Vitaly Buka (vitalybuka)

<details>
https://lab.llvm.org/buildbot/#/builders/238/builds/7108
</details>


---

### Comment 3 - vfdff

hi @vitalybuka would your please give me some advice on how to reproduce the issue ?
I can't see any valid info from above link, does any extra right required?

---

### Comment 4 - vfdff

ping @vitalybuka ?

---

### Comment 5 - vitalybuka

You probably can follow https://github.com/google/sanitizers/wiki/SanitizerBotReproduceBuild on GCE ARM machine

This is UBSAN, so maybe even QEMU is enough.

---

### Comment 6 - vfdff

hi @vitalybuka, As I can't reproduce your issue, and now with extra `commit 46ea6571` on  [#79951](https://github.com/llvm/llvm-project/pull/79951), I've been testing our downstream branch for some time and have found no new issues. so would you help me check this also fix the runtime issue you reported ?

---

### Comment 7 - vfdff

sorry for trouble!

---

### Comment 8 - vitalybuka

Unfortunately link to bot is expired, I am not sure which one was broken.
I added msan recently, but it was incorrect. I tried to reproduce f5687636415969e6d945659a0b78734abdfb0f06 to be sure, but bots are newer linux, and it fails with unrelated mmap error. To pass that error to reach miscompile I need to downgrade bots, which it to much effor. I will try to run #79951 on head and see if it's still broken.

---

### Comment 9 - vitalybuka

Reverted with 43ffe2e
details are here https://github.com/llvm/llvm-project/pull/79951


---

### Comment 10 - c-rhodes

I can't see a reproducer, so not much we can do about this, but looks like it was potentially fixed by #106065

---

