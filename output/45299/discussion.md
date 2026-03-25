# Incorrect instcombine fold of vector ult -> sgt

**Author:** nunoplopes
**URL:** https://github.com/llvm/llvm-project/issues/45299
**Status:** Closed
**Labels:** bugzilla, miscompilation, llvm:instcombine
**Closed Date:** 2025-09-15T07:55:59Z

## Body

|  |  |
| --- | --- |
| Bugzilla Link | [45954](https://llvm.org/bz45954) |
| Version | trunk |
| OS | All |
| CC | @aqjune,@LebedevRI,@RKSimon,@regehr,@rotateright |

## Extended Description 
Test: Transforms/InstCombine/canonicalize-clamp-like-pattern-between-zero-and-positive-threshold.ll
Summary: There's a off-by-one in the transformation below that makes the optimize code return the wrong value.
The original code can return either %x or %replacement_low, while the optimized code returns %replacement_high for %x[1] == 65536.
```ll
define <3 x i32> @t20_ult_slt_vec_undef1(<3 x i32> %x, <3 x i32> %replacement_low, <3 x i32> %replacement_high) {
  %t0 = icmp slt <3 x i32> %x, { 65536, 65537, 65536 }
  %t1 = select <3 x i1> %t0, <3 x i32> %replacement_low, <3 x i32> %replacement_high
  %t2 = icmp ult <3 x i32> %x, { 65536, undef, 65536 }
  %r = select <3 x i1> %t2, <3 x i32> %x, <3 x i32> %t1
  ret <3 x i32> %r
}
=>
define <3 x i32> @t20_ult_slt_vec_undef1(<3 x i32> %x, <3 x i32> %replacement_low, <3 x i32> %replacement_high) {
  %1 = icmp slt <3 x i32> %x, { 0, 0, 0 }
  %2 = icmp sgt <3 x i32> %x, { 65535, 65535, 65535 }
  %3 = select <3 x i1> %1, <3 x i32> %replacement_low, <3 x i32> %x
  %r = select <3 x i1> %2, <3 x i32> %replacement_high, <3 x i32> %3
  ret <3 x i32> %r
}
Transformation doesn't verify!
ERROR: Value mismatch

Example:
<3 x i32> %x = < #x0000ffff (65535), #x00010000 (65536), #x08000000 (134217728) >
<3 x i32> %replacement_low = < *, #x00000000 (0), * >
<3 x i32> %replacement_high = < *, #x00001000 (4096), #x0000ffff (65535) >

Source:
<3 x i1> %t0 = < #x1 (1), #x1 (1), #x0 (0) >
<3 x i32> %t1 = < *, #x00000000 (0), #x0000ffff (65535) >
<3 x i1> %t2 = < #x1 (1), undef, #x0 (0) >
<3 x i32> %r = < #x0000ffff (65535), #x00000000 (0)     [based on undef value], #x0000ffff (65535) >

Target:
<3 x i1> %1 = < #x0 (0), #x0 (0), #x0 (0) >
<3 x i1> %2 = < #x0 (0), #x1 (1), #x1 (1) >
<3 x i32> %3 = < #x0000ffff (65535), #x00010000 (65536), #x08000000 (134217728) >
<3 x i32> %r = < #x0000ffff (65535), #x00001000 (4096), #x0000ffff (65535) >
Source value: < #x0000ffff (65535), #x00000000 (0), #x0000ffff (65535) >
Target value: < #x0000ffff (65535), #x00001000 (4096), #x0000ffff (65535) >
```

https://web.ist.utl.pt/nuno.lopes/alive2/index.php?hash=72296a443c683892&test=Transforms%2FInstCombine%2Fcanonicalize-clamp-like-pattern-between-zero-and-positive-threshold.ll

## Comments

### Comment 1 - aqjune

Hi,

It is because the transformation is fine if we can choose a value for undef in source that explains it.
We can choose the two undefs to be 65536 and 65536, and then the transformation is correct, so it is okay.

Another example:

// you can assume that y is poison, because undef can be chosen to be NaN.
y = fadd nnan x, undef
use(y)
=>
use(poison)

On the contrary, if the target has undef, it becomes tough.

---

### Comment 2 - LebedevRI


> Hmm.
> But then why @&#8203;t19_ult_slt_vec_undef2 is fine?
@&#8203;t21_ult_slt_vec_undef2 that is of course.

> There undef's could be 65537 and 65536, and we'd have the same issue?

---

### Comment 3 - LebedevRI

Hmm.
But then why @&#8203;t19_ult_slt_vec_undef2 is fine?
There undef's could be 65537 and 65536, and we'd have the same issue?

---

### Comment 4 - nikic

As far as I can tell, this issue has been fixed. Possibly when we made PatternMatch only accept poison elements rather than undef.

---

