# [SLP] Binary combines with MUL and AND may use the wrong identity value

**Author:** bababuck
**URL:** https://github.com/llvm/llvm-project/issues/180456
**Status:** Closed
**Labels:** miscompilation, llvm:SLPVectorizer
**Closed Date:** 2026-02-12T17:34:46Z

## Body

The following lowering currently occurs: 
`opt -S --passes=slp-vectorizer -mtriple=aarch64-unknown-linux-gnu -slp-threshold=-50`
```llvm
define void @sub_mul(ptr %p, ptr %s) {
entry:
  %p1 = getelementptr i16, ptr %p, i64 1
  %p2 = getelementptr i16, ptr %p, i64 2
  %p3 = getelementptr i16, ptr %p, i64 3

  %l0 = load i16, ptr %p
  %l1 = load i16, ptr %p1
  %l2 = load i16, ptr %p2
  %l3 = load i16, ptr %p3

  %mul0 = sub i16 %l0, 0
  %mul1 = mul i16 %l1, 5
  %mul2 = mul i16 %l2, 2
  %mul3 = mul i16 %l3, 3

  %s1 = getelementptr i16, ptr %s, i64 1
  %s2 = getelementptr i16, ptr %s, i64 2
  %s3 = getelementptr i16, ptr %s, i64 3

  store i16 %mul0, ptr %s
  store i16 %mul1, ptr %s1
  store i16 %mul2, ptr %s2
  store i16 %mul3, ptr %s3
  ret void
}
```
to
```llvm
define void @sub_mul(ptr %p, ptr %s) {
entry:
%tmp0 = load <4 x i16>, ptr %p, align 2
%tmp1 = mul <4 x i16> %tmp0, <i16 0, i16 5, i16 2, i16 3>
store <4 x i16> %tmp1, ptr %s, align 2
ret void
}
```

Which is an incorrect transformation: https://alive2.llvm.org/ce/z/BgqGWV

