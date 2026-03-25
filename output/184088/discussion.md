# [DSE] Pointer provenance to be taken into account in `dominatingConditionImpliesValue`

**Author:** antoniofrighetto
**URL:** https://github.com/llvm/llvm-project/issues/184088
**Status:** Closed
**Labels:** miscompilation, llvm:transforms
**Closed Date:** 2026-03-04T09:36:45Z

## Body

Alive2: https://alive2.llvm.org/ce/z/e6pCvy.
```llvm
; RUN: ./bin/opt -S -passes=dse < %s
define void @src(ptr %x, ptr %y) {
entry:
  %val = load ptr, ptr %x, align 8
  %cmp = icmp eq ptr %val, %y
  br i1 %cmp, label %then, label %end

then:
  store ptr %y, ptr %x, align 8
  br label %end

end:
  ret void
}

define void @tgt(ptr %x, ptr %y) {
entry:
  %val = load ptr, ptr %x, align 8
  %cmp = icmp eq ptr %val, %y
  br i1 %cmp, label %then, label %end

then:
  br label %end

end:
  ret void
}
```

Stores whose value being written may be implied by a dominating condition of kind `icmp eq ptr` may not be eliminated straightforwardly,  as `icmp eq` compare their integral values only, not pointers' provenance. (From: https://github.com/llvm/llvm-project/pull/181709#discussion_r2868739673, similarly already tracked in SCCP, LVI).

