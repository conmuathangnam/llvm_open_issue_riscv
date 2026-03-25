# 16% compile-time slowdown after clang #161671

**Author:** zmodem
**URL:** https://github.com/llvm/llvm-project/issues/172266

## Body

Filing as a proper bug so we don't lose track of this.

Chromium reported a 16% build time slowdown after https://github.com/llvm/llvm-project/pull/161671 That includes linking and other build actions, so the average compiler slowdown is probably a little higher.

When bootstrapping Clang with `-DCMAKE_CXX_STANDARD=20`, we only measured an 1.5% regression (https://github.com/llvm/llvm-project/pull/161671#issuecomment-3442833336), so projects are probably differently affected depending on their use of c++ features.

Code that's especially affected by this is *badly* affected however. For example, https://github.com/llvm/llvm-project/pull/161671#issuecomment-3533372315 shows a 140% slowdown, and there is lots more like it.

It would be great if we could fix or at least mitigate this before Clang 22 -- or if that's not possible, give some guidance on source mitigations to avoid the slowdown.

@zyn0217 @cor3ntin @AaronBallman 

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Hans Wennborg (zmodem)

<details>
Filing as a proper bug so we don't lose track of this.

Chromium reported a 16% build time slowdown after https://github.com/llvm/llvm-project/pull/161671 That includes linking and other build actions, so the average compiler slowdown is probably a little higher.

When bootstrapping Clang with `-DCMAKE_CXX_STANDARD=20`, we only measured an 1.5% regression (https://github.com/llvm/llvm-project/pull/161671#issuecomment-3442833336), so projects are probably differently affected depending on their use of c++ features.

Code that's especially affected by this is *badly* affected however. For example, https://github.com/llvm/llvm-project/pull/161671#issuecomment-3533372315 shows a 140% slowdown, and there is lots more like it.

It would be great if we could fix or at least mitigate this before Clang 22 -- or if that's not possible, give some guidance on source mitigations to avoid the slowdown.

@<!-- -->zyn0217 @<!-- -->cor3ntin @<!-- -->AaronBallman 
</details>


---

### Comment 2 - mikeynap

+1 here, our codebase (heavy template usage, giant AST) has a > 30%  regression in memory usage  and compile time with this commit (we bisected to this one specifically). 

Reverting the commit from 22 was very hard, ended up just reinstating the old pathway in SemaConcept.cpp as a way to compare before and after. 

---

### Comment 3 - c-rhodes

@cor3ntin I don't know the context but this seems pretty bad, please can you take a look.

---

### Comment 4 - AaronBallman

I believe this is already on his radar, but his availability may be delayed right now (I know he's in the middle of a move). CC @zyn0217 who also helped out on that PR for more eyeballs. This is something that would be good to get solved for Clang 22 given the size of the performance regression.

---

### Comment 5 - cor3ntin

Yes, we are aware. Note that since the issue was reported, we deployed _some_ mitigations - but not quite enough.
This has to do with repeated evaluation of nested requirements in some cases - and seem to be triggered by libc++ patterns in c++20 modes (the regression is not nearly as bad on the shared repro in c++23 for example, due do fold expresssion subsumption)

The actual issue is proving tricky to pin down and yes, I am in the middle of a move, things are going to be hectic for yet a few more days.

Given that both Younan and myself have spent quite a significant amount of time on this already, I'm sure we would appreciate more eyeballs.

---

### Comment 6 - philnik777

Is this something we can mitigate in libc++? Would avoiding certain patterns improve compile times further potentially?

---

