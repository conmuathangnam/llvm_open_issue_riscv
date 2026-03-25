# [IndVarSimplify][SCEV] IndVarSimplify incorrectly hoists loop exit condition due to signed wrap

**Author:** KirillVNaumov
**URL:** https://github.com/llvm/llvm-project/issues/180870
**Status:** Closed
**Labels:** miscompilation, llvm:SCEV
**Closed Date:** 2026-02-13T11:23:32Z

## Body

### Bug
Here is a link to the test, demonstrating the issue: https://godbolt.org/z/8rqoE6s9E
This is one of the tests that demonstrates the issue:
```llvm
define void @test_indvar_limits_samesign(i32 %unknown_limit, ptr addrspace(1) %length_ptr) {
entry:
  br label %header

header:                                           ; preds = %latch, %entry
  %iv = phi i32 [ 1, %entry ], [ %iv.next, %latch ]
  %iv.next = add i32 %iv, 1
  %range_check1 = icmp samesign ult i32 %iv.next, %unknown_limit
  %is_positive = icmp sgt i32 %unknown_limit, 0
  %or.cond = and i1 %range_check1, %is_positive
  br i1 %or.cond, label %latch, label %ret

latch:                                            ; preds = %header
  %iv.prev = add i32 %iv, -1
  %length = load i32, ptr addrspace(1) %length_ptr, align 4, !range !0
  %range_check2 = icmp ult i32 %iv.prev, %length
  br i1 %range_check2, label %header, label %ret

ret:                                              ; preds = %latch, %header
  ret void
}

!0 = !{i32 0, i32 2147483647}
```
Running `indvars` pass for this test results in hoisting of `%range_check1 = icmp samesign ult i32 %iv.next, %unknown_limit` instruction to the preheader with the replacement of the inductive variable by its value during the first iteration: `%range_check1.first_iter = icmp ult i32 2, %unknown_limit`. As is obvious from this test, this is a misoptimization since the information in the test is not sufficient to make this substitution.

### Analysis
The cause of the bug is likely located not in IndVarSimplify analysis, but in SCEV, since it's used to prove the replacement invariants. Another observation is related to signed wrap, since the range metadata plays a crucial role in the test to identify the maximum number of iterations and the value of the inductive variable on the last iteration - reducing the upper bound on the range metadata or removing it altogether masks the bug. `samesign` attribute also affects the bug's reproducibility, so I added two tests - with and without this attribute.

## Comments

### Comment 1 - nikic

> As is obvious from this test, this is a misoptimization since the information in the test is not sufficient to make this substitution.

Please be more explicit. For which value of unknown_limit is the transform incorrect?

---

### Comment 2 - KirillVNaumov

> For which value of unknown_limit is the transform incorrect?

Consider the following scenario: `%unknown_limit = 10` and `%length` that is loaded from `%length_ptr` is `100`. When the initial IR is executed, the loop will exit with the induction variable `%iv = 9` due to `%range_check1 = icmp samesign ult i32 %iv.next, %unknown_limit`. However, after running `indvars` pass on the test we get:

```llvm
define void @test_indvar_limits_samesign(i32 %unknown_limit, ptr addrspace(1) %length_ptr) {
entry:
  %range_check1.first_iter = icmp ult i32 2, %unknown_limit
  br label %header

header:
  %iv = phi i32 [ 1, %entry ], [ %iv.next, %latch ]
  %iv.next = add nuw i32 %iv, 1
  %is_positive = icmp sgt i32 %unknown_limit, 0
  %or.cond = and i1 %range_check1.first_iter, %is_positive
  br i1 %or.cond, label %latch, label %ret

latch:
  %iv.prev = add nsw i32 %iv, -1
  %length = load i32, ptr addrspace(1) %length_ptr, align 4
  %range_check2 = icmp samesign ult i32 %iv.prev, %length
  br i1 %range_check2, label %header, label %ret

ret:
  ret void
}
```
Here, the first range check is hoisted out of the loop (`%range_check1.first_iter = icmp ult i32 2, %unknown_limit`) and is always true; therefore, the loop can only exit via the second range check (`%range_check2 = icmp samesign ult i32 %iv.prev, %length`) when the induction variable `%iv` is equal to `101`.

---

### Comment 3 - nikic

Thanks, that does sound right.

Here's an alive2 failure if we add a use of an IV into the loop, otherwise the number of iterations is not observed: https://alive2.llvm.org/ce/z/em3ZHe

The relevant fold here is optimizeLoopExitWithUnknownExitCount().

---

### Comment 4 - nikic

Adding some debug output:
```
  dbgs() << "MaxIter: " << *MaxIter << "\n";
  dbgs() << "Orig: " << *LHSS << " " << Pred << " " << *RHSS << "\n";
  dbgs() << "LIP: " << *LIP->LHS << " " << LIP->Pred << " " << *LIP->RHS << "\n";
```
I get:
```
MaxIter: (2147483646 umin (-2 + (2 umax %unknown_limit)))
Orig: {2,+,1}<nuw><%header> ult %unknown_limit
LIP: 2 ult %unknown_limit
```

---

### Comment 5 - nikic

In
https://github.com/llvm/llvm-project/blob/208b4dbf122465db894bc0d62f73e802834b170e/llvm/lib/Analysis/ScalarEvolution.cpp#L11519-L11520
we end up checking a condition like `-2147483648 samesign ult %unknown_limit`.

We probably need to discard samesign in this code.

---

### Comment 6 - KirillVNaumov

Just in case - in https://godbolt.org/z/8rqoE6s9E, there is another very similar test that (in the initial IR) doesn't use `samesign` attribute. Initial IR:

```llvm
define void @test_indvar_limits(i32 %unknown_limit, i32 range(i32 0, 2147483647) %length1, i1 %cond, ptr addrspace(1) %length_ptr) {
entry:
  %precheck = icmp ugt i32 %length1, 1
  br i1 %precheck, label %header, label %ret

header:                                           ; preds = %latch, %entry
  %iv = phi i32 [ %iv.next, %latch ], [ 1, %entry ]
  %iv.next = add i32 %iv, 1
  %range_check1 = icmp ult i32 %iv.next, %length1
  %guard_cond = and i1 %range_check1, %cond
  %limit_not_reached = icmp slt i32 %iv, %unknown_limit
  %or.cond = and i1 %guard_cond, %limit_not_reached
  br i1 %or.cond, label %latch, label %ret

latch:                                            ; preds = %header
  %iv.prev = add i32 %iv, -1
  %length2 = load i32, ptr addrspace(1) %length_ptr, align 4, !range !0
  %range_check2 = icmp ult i32 %iv.prev, %length2
  br i1 %range_check2, label %header, label %ret

ret:                                              ; preds = %latch, %header, %entry
  ret void
}

!0 = !{i32 0, i32 2147483647}
```

After `-passes=indvars`:

```llvm
define void @test_indvar_limits(i32 %unknown_limit, i32 range(i32 0, 2147483647) %length1, i1 %cond, ptr addrspace(1) %length_ptr) {
entry:
  %precheck = icmp ugt i32 %length1, 1
  br i1 %precheck, label %header.preheader, label %ret

header.preheader:
  %range_check1.first_iter = icmp ult i32 2, %length1
  br label %header

header:
  %iv = phi i32 [ %iv.next, %latch ], [ 1, %header.preheader ]
  %iv.next = add nuw nsw i32 %iv, 1
  %guard_cond = and i1 %range_check1.first_iter, %cond
  %limit_not_reached = icmp slt i32 %iv, %unknown_limit
  %or.cond = and i1 %guard_cond, %limit_not_reached
  br i1 %or.cond, label %latch, label %ret.loopexit

latch:
  %iv.prev = add nsw i32 %iv, -1
  %length2 = load i32, ptr addrspace(1) %length_ptr, align 4
  %range_check2 = icmp samesign ult i32 %iv.prev, %length2
  br i1 %range_check2, label %header, label %ret.loopexit

ret.loopexit:
  br label %ret

ret:
  ret void
}
```

This test shows an exactly the same problem as in the first testcase: consider a scenario with values `%length1 = 10`, `%unknown_limit = 100`, and `%length2 = 100`. In the intial IR, we expect to exit on the iteration with the induction variable `%iv = 9`, however after the first range check is hoisted out of the loop, we would exit due to the `%limit_not_reached = icmp slt i32 %iv, %unknown_limit` with induction variable `%iv = 100`.

---

### Comment 7 - nikic

Candidate fix: https://github.com/llvm/llvm-project/pull/181171

@KirillVNaumov The root cause for that example is the same, as indvars ends up inferring the samesign flag on the icmp.

---

