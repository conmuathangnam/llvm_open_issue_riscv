# clang frontend crash due to an exponential complexity of typo corrector

**Author:** llvmbot
**URL:** https://github.com/llvm/llvm-project/issues/43925
**Status:** Closed
**Labels:** c++, clang:frontend, bugzilla, confirmed, hang
**Closed Date:** 2025-06-17T16:28:34Z

## Body

|  |  |
| --- | --- |
| Bugzilla Link | [44580](https://llvm.org/bz44580) |
| Version | unspecified |
| OS | All |
| Attachments | [typos.cpp from description](https://user-images.githubusercontent.com/60944935/143760271-8021ec6a-d52b-474d-9574-ebad55c2d0e4.gz) |
| Reporter | LLVM Bugzilla Contributor |
| CC | @dwblaikie,@DougGregor,@zygoloid,@Weverything |

## Extended Description 


Compiling typos.cpp ends with an "LLVM ERROR: out of memory"


typos.cpp is a very simple file with n=10 int definitions and a function call with n arguments. 
When clang analyzes this file it tries to correct typos in it. In order to do this it calls method clangSema::SemaExprCXX.cpp::CheckAndAdvanceTypoExprCorrectionStreams

The problem is that amount of calls is around 8^(n-1), which leads to a crash or infinitely long waiting time.

## Comments

### Comment 1 - Endilll

Still hanging as of post-17 trunk: https://godbolt.org/z/4PfT54sjG

---

### Comment 2 - llvmbot

@llvm/issue-subscribers-clang-frontend

---

### Comment 3 - AaronBallman

This issue is fixed now that we've removed delayed typo correction in 9eef4d1c5fa6b1bcbbe675c14ca8301d5d346f7b

---

