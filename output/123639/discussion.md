# SLP Vectorizer miscompilation

**Author:** aleks-tmb
**URL:** https://github.com/llvm/llvm-project/issues/123639
**Status:** Closed
**Labels:** miscompilation, llvm:SLPVectorizer
**Closed Date:** 2025-01-20T22:25:54Z

## Body

During our local testing, we identified a miscompilation where the SLP Vectorizer appears to be the cause. Here is the reproducer we managed to prepare:
https://godbolt.org/z/9nePcq7Go

The issue can be demonstrated using the `lli` tool. Unfortunately, I cannot use alive2 due to unknown libcall errors, such as @llvm.vector.insert.v32i32.v16i32 not being recognized by alive2

```
$ bin/opt -passes=slp-vectorizer -mattr=+avx2 src.ll -S > tgt.ll
$ bin/lli src.ll
Result: -256
$ bin/lli tgt.ll
Result: -272
```

The pseudocode represented by the IR is
```
t = 0
for (i = 0; i < 16; i++) {
    s -= (t + i)
    t = i
}
```
```
s -= (1 + 0) + (2 + 1) + (3 + 2) + (4 + 3) + ... + (14 + 15) + (15 + 16)
s = -256
```
This indicates that the SLP Vectorizer pass produces incorrect IR.

The issue became reproducible after patch `[SLP] Match poison as instruction with the same opcode` made by @alexey-bataev 
https://github.com/llvm/llvm-project/commit/b8703369daf777706196ff914c0376c27adde3cf

I'll provide more details on how the patch affects the issue as soon as I receive them.

## Comments

### Comment 1 - alexey-bataev

No related directly to this patch, reveals the bug that can be triggered even without patch. I reduced the reproducer, will commit fix soon

---

