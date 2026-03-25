# [SLPV] Pointer offsets truncated to 32-bits, resulting in wrong code

**Author:** rnk
**URL:** https://github.com/llvm/llvm-project/issues/139202
**Status:** Closed
**Labels:** miscompilation, llvm:SLPVectorizer, llvm:analysis
**Closed Date:** 2025-05-15T09:08:08Z

## Body

Consider:

```
$ cat t.ll
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-grtev4-linux-gnu"

define void @_ZN12_GLOBAL__N_111LargeObjectILm4294967296EEC2Em(ptr %this) #0 {
entry:
  store i64 1, ptr %this, align 8
  %b = getelementptr i8, ptr %this, i64 8
  store i64 2, ptr %b, align 8
  %c = getelementptr i8, ptr %this, i64 u0x100000010
  store i64 3, ptr %c, align 8
  %d = getelementptr i8, ptr %this, i64 u0x100000018
  store i64 4, ptr %d, align 8
  ret void
}

attributes #0 = { "target-features"="+aes,+avx,+cmov,+crc32,+cx16,+cx8,+fxsr,+mmx,+pclmul,+popcnt,+prfchw,+sse,+sse2,+sse3,+sse4.1,+sse4.2,+ssse3,+x87,+xsave" }

$ opt -passes=slp-vectorizer t.ll  -S -o -
; ModuleID = 't.ll'
source_filename = "t.ll"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-grtev4-linux-gnu"

define void @_ZN12_GLOBAL__N_111LargeObjectILm4294967296EEC2Em(ptr %this) #0 {
entry:
  store <4 x i64> <i64 1, i64 2, i64 3, i64 4>, ptr %this, align 8
  ret void
}

attributes #0 = { "target-features"="+aes,+avx,+cmov,+crc32,+cx16,+cx8,+fxsr,+mmx,+pclmul,+popcnt,+prfchw,+sse,+sse2,+sse3,+sse4.1,+sse4.2,+ssse3,+x87,+xsave" }
```

Clearly, somewhere inside SLPV, the GEP offsets here are truncated from 64-bits to 32. To quickly check SLPV with static analysis, I did this:

```
$ ninja opt # build everything once
$ touch ../llvm/lib/Transforms/Vectorize/SLPVectorizer.cpp
$ CCC_OVERRIDE_OPTIONS='+-Wshorten-64-to-32' ninja opt | tee slpv-truncations.txt
...
```

Unfortunately, there are 733 hits:
```
$ grep warning: slpv-truncations.txt  | wc -l
     733
```

This is not a recent regression, it goes back to LLVM 14 and earlier: https://godbolt.org/z/9baE551T7 LLVM 14 is just as far back as the IR is parseable out of the box.

I think the next move here would be to do a global search & replace of getZExtValue with some variant that asserts when the result is wider than 32-bits, or to focus only on the warnings that include getZExtValue in the warning text snippet.

## Comments

### Comment 1 - alexey-bataev

LoopAccessAnalysis.cpp, getPointersDiff() returns distance -16 for pointers %b and %d, there is an implicit cast from int64_t to int, which looses the info
https://github.com/llvm/llvm-project/blob/9eeae5a5de41b22d31d9037be2acc45dd7af4192/llvm/lib/Analysis/LoopAccessAnalysis.cpp#L1598
https://github.com/llvm/llvm-project/blob/9eeae5a5de41b22d31d9037be2acc45dd7af4192/llvm/lib/Analysis/LoopAccessAnalysis.cpp#L1589


---

