# [LV]Miscompile in Loop Vectorizer

**Author:** alexey-bataev
**URL:** https://github.com/llvm/llvm-project/issues/187323
**Status:** Closed
**Labels:** miscompilation, vectorizers
**Closed Date:** 2026-03-23T22:09:42Z

## Body

https://godbolt.org/z/5r3ejc7c6
Probably related to https://github.com/llvm/llvm-project/pull/185969
Note
```
%0 = trunc i64 %indvars.iv.next682 to i2
```
@fhahn

## Comments

### Comment 1 - fhahn

Is this he right reproducer? there's no `%0 = trunc i64 %indvars.iv.next682 to i2` and also no epilogue


---

### Comment 2 - alexey-bataev

Yeah, looks like no epilogue, but there is %0 = trunc i64 %indvars.iv.next682 to i2 in the vectorizied code

---

### Comment 3 - fhahn

Not seeing the issue currently, `  %0 = trunc i64 %indvars.iv.next682 to i2` is the same as `  %0 = and i64 %indvars.iv.next682, 3` in the original code?

---

### Comment 4 - alexey-bataev

Let me double check, probably overreduced the reproducer

---

### Comment 5 - alexey-bataev

Ok, hope have the right one.
https://godbolt.org/z/53zjhEsdG
The original scalar version results in 2
The loop vectorized version 
https://godbolt.org/z/z5v1TeTG4
Results in 4

---

