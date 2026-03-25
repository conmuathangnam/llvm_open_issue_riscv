# [indvars] Miscompile when loop body has an operation with Undefined Behaviour

**Author:** Nirhar
**URL:** https://github.com/llvm/llvm-project/issues/117133
**Status:** Closed
**Labels:** miscompilation, llvm:SCEV
**Closed Date:** 2024-12-01T12:11:11Z

## Body

Here is the problematic IR:
```
define i32 @widget() {
bb:
  br label %bb1

bb1:                                              ; preds = %bb5, %bb
  %phi = phi i32 [ 0, %bb ], [ %udiv6, %bb5 ]
  %phi2 = phi i32 [ 1, %bb ], [ %add, %bb5 ]
  %icmp = icmp eq i32 %phi, 0
  br i1 %icmp, label %bb3, label %bb8

bb3:                                              ; preds = %bb1
  %udiv = udiv i32 10, %phi2
  %urem = urem i32 %udiv, 10
  %icmp4 = icmp eq i32 %urem, 0
  br i1 %icmp4, label %bb7, label %bb5

bb5:                                              ; preds = %bb3
  %udiv6 = udiv i32 %phi2, 0
  %add = add i32 %phi2, 1
  br label %bb1

bb7:                                              ; preds = %bb3
  ret i32 5

bb8:                                              ; preds = %bb1
  ret i32 7
}
```
produces incorrect IR when `indvars` pass is run. I suspect this is because of the `%udiv6 = udiv i32 %phi2, 0` divide by zero operation. 
Look at the indvars transformation here: https://godbolt.org/z/cz1r5178h
The original IR must return 5, while the transformed IR returns 7

Proof of wrong transformation: https://alive2.llvm.org/ce/z/vPFhzg

## Comments

### Comment 1 - nikic

Is this a fuzzer-generated test case?

---

### Comment 2 - Nirhar

> Is this a fuzzer-generated test case?

Hi @nikic , yes this test was fuzzer-generated.

---

