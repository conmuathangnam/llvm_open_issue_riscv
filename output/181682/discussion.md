# [SLPVectorizer] Crash when offset equals DenseMap reserved keys for int64_t (regressed by 3572e62991f4)

**Author:** mateuszchudyk
**URL:** https://github.com/llvm/llvm-project/issues/181682
**Status:** Closed
**Labels:** llvm:SLPVectorizer, crash
**Closed Date:** 2026-02-16T19:07:30Z

## Body

A change introduced in [3572e62](https://github.com/llvm/llvm-project/commit/3572e62991f4) made SLPVectorizer use a DenseMap<int64_t, ...> keyed by an offset. When the offset equals to INT64_MAX or INT64_MAX-1 the pass crashes, because those two values are used by this DenseMap as EmptyKey and TombstoneKey.

**Error message**

> bool llvm::DenseMapBase<DerivedT, KeyT, ValueT, KeyInfoT, BucketT>::LookupBucketFor(const LookupKeyT&, BucketT*&) [with LookupKeyT = long int; DerivedT = llvm::SmallDenseMap<long int, std::pair<llvm::SmallVector<llvm::Value*>, llvm::SmallVector<unsigned int> > >; KeyT = long int; ValueT = std::pair<llvm::SmallVector<llvm::Value*>, llvm::SmallVector<unsigned int> >; KeyInfoT = llvm::DenseMapInfo<long int>; BucketT = llvm::detail::DenseMapPair<long int, std::pair<llvm::SmallVector<llvm::Value*>, llvm::SmallVector<unsigned int> > >]: Assertion `!KeyInfoT::isEqual(Val, EmptyKey) && !KeyInfoT::isEqual(Val, TombstoneKey) && "Empty/Tombstone value shouldn't be inserted into map!"' failed.

**Minimal reproducer**
```llvm
define void @crash() {
entry:
  %0 = getelementptr i8, ptr addrspace(1) null, i64 9223372036854775807
  %a = load float, ptr addrspace(1) %0, align 4
  %b = load float, ptr addrspace(1) null, align 4
  %c = fadd float %a, %b
  store float %c, ptr addrspace(1) null, align 4
  ret void
}
```
```
opt -passes=slp-vectorizer -S crash.ll
```
https://godbolt.org/z/Wdz1jMda6

**Proposed Fix**
Disable this code-path for offsets equal to EmptyKey or TomestoneKey. In the long run, consider changing the data structure or changing the key type (wider type or wrapper structure) to prevent such collisions.



