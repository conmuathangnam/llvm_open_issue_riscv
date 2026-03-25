# wrong folding of `select`s in slp-vectorizer

**Author:** bongjunj
**URL:** https://github.com/llvm/llvm-project/issues/114905
**Status:** Closed
**Labels:** miscompilation, llvm:SLPVectorizer
**Closed Date:** 2024-12-26T13:22:55Z

## Body

Alive2 report: https://alive2.llvm.org/ce/z/jEopx9

```llvm
----------------------------------------
define i1 @logical_and_icmp_diff_preds.2(<4 x i32> %x) {
#0:
  %x0 = extractelement <4 x i32> %x, i32 0
  %x1 = extractelement <4 x i32> %x, i32 4294967295
  %x3 = extractelement <4 x i32> %x, i32 3
  %#1 = icmp ugt i32 %x0, 0
  %c1 = icmp slt i32 %x1, 0
  %c3 = icmp slt i32 %x3, 0
  %s1 = select i1 %#1, i1 %c1, i1 0
  %s2 = select i1 %s1, i1 %c3, i1 0
  %s3 = select i1 %s2, i1 %c3, i1 0
  ret i1 %s3
}
=>
define i1 @logical_and_icmp_diff_preds.2(<4 x i32> %x) {
#0:
  %x0 = extractelement <4 x i32> %x, i32 0
  %x1 = extractelement <4 x i32> %x, i32 4294967295
  %x3 = extractelement <4 x i32> %x, i32 3
  %#1 = icmp ugt i32 %x0, 0
  %c1 = icmp slt i32 %x1, 0
  %c3 = icmp slt i32 %x3, 0
  %op.rdx = select i1 %c3, i1 %c1, i1 0
  %op.rdx1 = select i1 %op.rdx, i1 %#1, i1 0
  ret i1 %op.rdx1
}
Transformation doesn't verify!

ERROR: Target is more poisonous than source

Example:
<4 x i32> %x = < #x00000000 (0), #x00000000 (0), #x00000000 (0), #xffffffff (4294967295, -1) >

Source:
i32 %x0 = #x00000000 (0)
i32 %x1 = poison
i32 %x3 = #xffffffff (4294967295, -1)
i1 %#1 = #x0 (0)
i1 %c1 = poison
i1 %c3 = #x1 (1)
i1 %s1 = #x0 (0)
i1 %s2 = #x0 (0)
i1 %s3 = #x0 (0)

Target:
i32 %x0 = #x00000000 (0)
i32 %x1 = poison
i32 %x3 = #xffffffff (4294967295, -1)
i1 %#1 = #x0 (0)
i1 %c1 = poison
i1 %c3 = #x1 (1)
i1 %op.rdx = poison
i1 %op.rdx1 = poison
Source value: #x0 (0)
Target value: poison

Summary:
  0 correct transformations
  1 incorrect transformations
  0 failed-to-prove transformations
  0 Alive2 errors
```

## Comments

### Comment 1 - nunoplopes

Alive2 is still not happy with the fix:
```llvm
define i1 @test(<4 x i32> %x) {
  %x0 = extractelement <4 x i32> %x, i32 0
  %x1 = extractelement <4 x i32> %x, i32 4294967295
  %x3 = extractelement <4 x i32> %x, i32 3
  %#1 = icmp ugt i32 %x0, 0
  %c1 = icmp slt i32 %x1, 0
  %c3 = icmp slt i32 %x3, 0
  %s1 = select i1 %#1, i1 %c1, i1 0
  %s2 = select i1 %s1, i1 %c3, i1 0
  %s3 = select i1 %s2, i1 %c3, i1 0
  ret i1 %s3
}
=>
define i1 @test(<4 x i32> %x) {
  %x0 = extractelement <4 x i32> %x, i32 0
  %x1 = extractelement <4 x i32> %x, i32 4294967295
  %x3 = extractelement <4 x i32> %x, i32 3
  %#1 = icmp ugt i32 %x0, 0
  %c1 = icmp slt i32 %x1, 0
  %c3 = icmp slt i32 %x3, 0
  %#2 = freeze i1 %c3
  %op.rdx = select i1 %#2, i1 %c1, i1 0
  %op.rdx1 = select i1 %op.rdx, i1 %#1, i1 0
  ret i1 %op.rdx1
}
Transformation doesn't verify!

ERROR: Target is more poisonous than source

Example:
<4 x i32> %x = < #x00000000 (0), poison, poison, #xffffffff (4294967295, -1) >

Source:
i32 %x0 = #x00000000 (0)
i32 %x1 = poison
i32 %x3 = #xffffffff (4294967295, -1)
i1 %#1 = #x0 (0)
i1 %c1 = poison
i1 %c3 = #x1 (1)
i1 %s1 = #x0 (0)
i1 %s2 = #x0 (0)
i1 %s3 = #x0 (0)

Target:
i32 %x0 = #x00000000 (0)
i32 %x1 = poison
i32 %x3 = #xffffffff (4294967295, -1)
i1 %#1 = #x0 (0)
i1 %c1 = poison
i1 %c3 = #x1 (1)
i1 %#2 = #x1 (1)
i1 %op.rdx = poison
i1 %op.rdx1 = poison
Source value: #x0 (0)
Target value: poison
```

---

