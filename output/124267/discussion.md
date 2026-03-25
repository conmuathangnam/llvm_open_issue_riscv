# [ValueTracking] miscompile in samesign compare

**Author:** dianqk
**URL:** https://github.com/llvm/llvm-project/issues/124267
**Status:** Closed
**Labels:** miscompilation, llvm:analysis
**Closed Date:** 2025-01-24T15:02:51Z

## Body

#122590 and #122700 miscompile the following code:

```llvm
define i1 @src(i8 %L0, i8 %L1, i1 %V) {
  %LHS = icmp samesign ugt i8 %L0, %L1
  %R0 = sub nsw i8 %L0, %L1
  %RHS = icmp ult i8 %R0, -1
  br i1 %LHS, label %LHS_true, label %LHS_false

LHS_true:
  br label %EXIT

LHS_false:
  br label %EXIT

EXIT:
  %RET = phi i1 [ %RHS, %LHS_true ], [ %V, %LHS_false ]
  ret i1 %RET
}
```

`%RHS` should be 1: https://alive2.llvm.org/ce/z/RscAzy.

