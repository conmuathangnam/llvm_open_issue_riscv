# riscv64 and aarch64 backends propagating range metadata too aggressively

**Author:** regehr
**URL:** https://github.com/llvm/llvm-project/issues/145363
**Status:** Closed
**Labels:** miscompilation, llvm:SelectionDAG
**Closed Date:** 2025-06-27T15:58:07Z

## Body

let's take this function:
```llvm
define i32 @f(ptr %0) {
  %2 = load i32, ptr %0, align 4, !range !0
  %3 = load i32, ptr %0, align 4
  %4 = add i32 1206154280, %3
  ret i32 %4
}

!0 = !{i32 1, i32 2, i32 3, i32 4}
```
both the risc64 and aarch64 backends lower it to something that does an `or` instead of an `add`:
```
f:
        lw      a0, 0(a0)
        lui     a1, 294471
        addi    a1, a1, 1064
        or      a0, a0, a1
        ret
```
this would work fine if it was `%2` that was getting added, but it's `%3`. if the range invariant isn't respected by the data in memory `%2` harmlessly gets poison, but this doesn't allow us to infer anything about `%3`

cc @nunoplopes 


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-aarch64

Author: John Regehr (regehr)

<details>
let's take this function:
```llvm
define i32 @<!-- -->f(ptr %0) {
  %2 = load i32, ptr %0, align 4, !range !0
  %3 = load i32, ptr %0, align 4
  %4 = add i32 1206154280, %3
  ret i32 %4
}

!0 = !{i32 1, i32 2, i32 3, i32 4}
```
both the risc64 and aarch64 backends lower it to something that does an `or` instead of an `add`:
```
f:
        lw      a0, 0(a0)
        lui     a1, 294471
        addi    a1, a1, 1064
        or      a0, a0, a1
        ret
```
this would work fine if it was `%2` that was getting added, but it's `%3`. if the range invariant isn't respected by the data in memory `%2` harmlessly gets poison, but this doesn't allow us to infer anything about `%3`

cc @<!-- -->nunoplopes 

</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-backend-risc-v

Author: John Regehr (regehr)

<details>
let's take this function:
```llvm
define i32 @<!-- -->f(ptr %0) {
  %2 = load i32, ptr %0, align 4, !range !0
  %3 = load i32, ptr %0, align 4
  %4 = add i32 1206154280, %3
  ret i32 %4
}

!0 = !{i32 1, i32 2, i32 3, i32 4}
```
both the risc64 and aarch64 backends lower it to something that does an `or` instead of an `add`:
```
f:
        lw      a0, 0(a0)
        lui     a1, 294471
        addi    a1, a1, 1064
        or      a0, a0, a1
        ret
```
this would work fine if it was `%2` that was getting added, but it's `%3`. if the range invariant isn't respected by the data in memory `%2` harmlessly gets poison, but this doesn't allow us to infer anything about `%3`

cc @<!-- -->nunoplopes 

</details>


---

### Comment 3 - jroelofs

Why isn't this a valid GVN-ing of `%3` => `%2`? https://alive2.llvm.org/ce/z/xbMspv

---

### Comment 4 - regehr

> Why isn't this a valid GVN-ing of `%2` => `%3`? https://alive2.llvm.org/ce/z/xbMspv

the transformation makes the code valid in fewer situations than the original code was-- it returns poison more often, so it can't be done. when doing any sort of CSE, we'll need to take the intersection of the non-poison-generating ranges, not the union.

---

### Comment 5 - topperc

This fixes this case, but I'm not sure if there are other CSE paths we need to fix.

```
diff --git a/llvm/lib/CodeGen/SelectionDAG/SelectionDAG.cpp b/llvm/lib/CodeGen/SelectionDAG/SelectionDAG.cpp
index 30ee6a99b9df..64fd8848f534 100644
--- a/llvm/lib/CodeGen/SelectionDAG/SelectionDAG.cpp
+++ b/llvm/lib/CodeGen/SelectionDAG/SelectionDAG.cpp
@@ -9403,6 +9403,8 @@ SDValue SelectionDAG::getLoad(ISD::MemIndexedMode AM, ISD::LoadExtType ExtType,
   void *IP = nullptr;
   if (SDNode *E = FindNodeOrInsertPos(ID, dl, IP)) {
     cast<LoadSDNode>(E)->refineAlignment(MMO);
+    if (cast<LoadSDNode>(E)->getMemOperand()->getRanges() && !MMO->getRanges())
+      cast<LoadSDNode>(E)->getMemOperand()->clearRanges();
     return SDValue(E, 0);
   }
   auto *N = newSDNode<LoadSDNode>(dl.getIROrder(), dl.getDebugLoc(), VTs, AM,
```

---

### Comment 6 - regehr

Craig do you want to intersect the ranges instead of clearing?

---

### Comment 7 - topperc

> Craig do you want to intersect the ranges instead of clearing?

I agree that would be more correct, but we're holding a reference to an IR metadata node. I wasn't sure about creating new IR metadata nodes that aren't used by IR itself.

---

### Comment 8 - topperc

> > Craig do you want to intersect the ranges instead of clearing?
> 
> I agree that would be more correct, but we're holding a reference to an IR metadata node. I wasn't sure about creating new IR metadata nodes that aren't used by IR itself.

Looks like we do create a new node in at least one place in DAGCombiner so I guess it should be ok.

---

