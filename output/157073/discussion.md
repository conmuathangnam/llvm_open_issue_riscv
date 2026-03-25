# Rust's tests/ui/asm/x86_64/may_unwind.rs fails on Windows after b933f0c376c983614a0701f3bfd4054cf8fe4386

**Author:** zmodem
**URL:** https://github.com/llvm/llvm-project/issues/157073
**Status:** Closed
**Labels:** llvm:codegen, release:backport, platform:windows
**Closed Date:** 2026-01-19T08:09:06Z

## Body

As mentioned in https://github.com/llvm/llvm-project/pull/144745#issuecomment-3212031633, Rust's `tests/ui/asm/x86_64/may_unwind.rs` started failing on Windows after b933f0c376c983614a0701f3bfd4054cf8fe4386

On the PR, there seemed to be rough consensus around the proposed fix of

> Maybe we should make the compiler insert a nop after inline asm blocks marked "unwind"?



## Comments

### Comment 1 - zmodem

@sivadeilra @dpaoliello @efriedma-quic @rnk @aeubanks

---

### Comment 2 - sivadeilra

I'll take a look.

---

### Comment 3 - nico

@sivadeilra: Any news?

---

### Comment 4 - sivadeilra

Sorry, not yet -- trying to find time to fix this.  Will definitely get to it as soon as I can.

---

### Comment 5 - nico

Any chance to get it fixed this year? :)

---

### Comment 6 - nikic

/cherry-pick 792670a4003cba8636713916bed89a0b991fb32d

---

### Comment 7 - llvmbot

/pull-request llvm/llvm-project#176887

---

