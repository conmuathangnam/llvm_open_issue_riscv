# [InstCombine] `noundef` attribute should be dropped

**Author:** dtcxzyw
**URL:** https://github.com/llvm/llvm-project/issues/121428
**Status:** Closed
**Labels:** miscompilation, llvm:instcombine
**Closed Date:** 2025-01-05T16:04:29Z

## Body

Alive2: https://alive2.llvm.org/ce/z/wKsUWK
```
define i32 @src(i32 noundef %x) {
  %ct = call noundef i32 @llvm.cttz.i32(i32 %x, i1 false)
  %cmp = icmp ne i32 %x, 0
  %res = select i1 %cmp, i32 %ct, i32 123
  ret i32 %res
}
define i32 @tgt(i32 noundef %x) {
  %ct = call noundef range(i32 0, 33) i32 @llvm.cttz.i32(i32 %x, i1 true)
  %cmp.not = icmp eq i32 %x, 0
  %res = select i1 %cmp.not, i32 123, i32 %ct
  ret i32 %res
}
```


