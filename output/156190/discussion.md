# Wrong Answer after LoopVectorize on Arm

**Author:** ashermancinelli
**URL:** https://github.com/llvm/llvm-project/issues/156190
**Status:** Closed
**Labels:** miscompilation, release:backport, vectorizers
**Closed Date:** 2025-09-30T09:55:49Z

## Body

[Found in this Fortran test case](https://github.com/fujitsu/compiler-test-suite/blob/main/Fortran/0631/0631_0058.f). We find incorrect answers after loop-vectorize and only see this on aarch64. This is the LLVM IR before and after loop vectorize, with the dependency on the Fortran runtime removed:

* [Before loop vectorize](https://godbolt.org/z/boEbPWxYK) (good output)
* [After loop vectorize](https://godbolt.org/z/5hYEeshed) (bad output)
* [Opt LV debug output](https://godbolt.org/z/aPKbqv6qG)

This is the expected output of the program alongside the output after LV:
```
Good:
v[90][90] = 40.000000 + 0.000000i
y[90][90] = 21.000000 + 0.000000i

Bad:
v[90][90] = 40.000000 + 40.000000i ;; << real component sum is written to both
y[90][90] = 21.000000 + 0.000000i
```

You can see the real component is written to both the real and imaginary component of this array. If we replace the two instructions responsible for computing the imaginary component, the test yields the expected output again:

```diff
139,141c139,140
<   %broadcast.splatinsert13 = insertelement <2 x double> poison, double %47, i64 0, !dbg !8
<   %broadcast.splat14 = shufflevector <2 x double> %broadcast.splatinsert13, <2 x double> poison, <2 x i32> zeroinitializer, !dbg !8
<   %48 = fadd contract <2 x double> %broadcast.splat12, %broadcast.splat14, !dbg !8
---
>   %wide.load.x = load <2 x double>, ptr %46, align 8, !dbg !8
>   %48 = fadd contract <2 x double> %wide.load, %wide.load.x, !dbg !8
```

[This godbolt link has the corrected IR](https://godbolt.org/z/78ofh1Yeh):

```llvm
  ; this looks wrong
  %47 = load double, ptr %46, align 8, !dbg !8
  %broadcast.splatinsert13 = insertelement <2 x double> poison, double %47, i64 0, !dbg !8
  %broadcast.splat14 = shufflevector <2 x double> %broadcast.splatinsert13, <2 x double> poison, <2 x i32> zeroinitializer, !dbg !8
  %48 = fadd contract <2 x double> %broadcast.splat12, %broadcast.splat14, !dbg !8

  ; Hand-edits which give expected results
  ;%wide.load.x = load <2 x double>, ptr %46, align 8, !dbg !8
  ;%48 = fadd contract <2 x double> %wide.load, %wide.load.x, !dbg !8
```

Note that if we compile the exact same IR pre-LV for a generic x86 architecture we do not see the same behavior:
```shell
> opt /home/amancinelli/tmp.iJQ3cjnSSc-before-pass-316.ll -passes=loop-vectorize -S -o /tmp/t.ll && clang /tmp/t.ll && ./a.out
opt: WARNING: failed to create target machine for 'aarch64-unknown-linux-gnu': unable to get target for 'aarch64-unknown-linux-gnu', see --version and --triple.
warning: overriding the module target triple with x86_64-unknown-linux-gnu
      [-Woverride-module]
1 warning generated.
v[90][90] = 40.000000 + 0.000000i
y[90][90] = 21.000000 + 0.000000i
```

## Comments

### Comment 1 - fhahn

Should be fixed by 055e4ff35ab0789494ff9a3d41e3182a3aca8d0a

---

### Comment 2 - nikic

Does this need backporting, or is it a new issue?

---

### Comment 3 - fhahn

Yes we should back-port this, https://godbolt.org/z/q7hn6vGdY

---

### Comment 4 - fhahn

/cherry-pick https://github.com/llvm/llvm-project/commit/7b828738c692ba64a48b4f4f6763064d58ef0618 https://github.com/llvm/llvm-project/commit/055e4ff35ab0789494ff9a3d41e3182a3aca8d0a

---

### Comment 5 - fhahn

Cherry-pick PR: https://github.com/llvm/llvm-project/pull/158013

---

### Comment 6 - fhahn

Fix for this landed on the release branch as well: https://github.com/llvm/llvm-project/commit/661c387fc2f16c5d2a6ef2843b0e5874b259a3e8

---

