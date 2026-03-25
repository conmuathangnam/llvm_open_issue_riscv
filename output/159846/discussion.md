# [DA] Incorrect DA results due to overflow in Strong SIV test

**Author:** amehsan
**URL:** https://github.com/llvm/llvm-project/issues/159846
**Status:** Closed
**Labels:** miscompilation, loopoptim
**Closed Date:** 2025-11-27T17:09:57Z

## Body

The issue was initially reported by @kasuga-fj during a discussion on [PR 128782](https://github.com/llvm/llvm-project/pull/128782). For the loop in Example 1  (IR at the end)  DA reports incorrect results (most notably it reports no dependency between two store instructions). It is easy to show that different variations of this loop have the same problem. See Example 2 for one of the cases.

**Example 1:**
```
; for (i = 0; i < 9223372036854775806; i++) {
;   if (i < 2147483640)
;     for (j = 0; j < 2147483640; j++)
;       a[i + j * 4294967296] = 0;
;
;   for (j = 0; j < 2147483640; j++)
;     a[j * 2] = 0;
; }
;
```

**Example 2:**
```
void foo2 (int *a ) {

  unsigned long long i;
  unsigned long long j;

  for (i = 0; i < 9223372036854775806; i++) {
    for (j = 0; j < 2147483640; j++) {
      a[i + j * 4294967296] = 0;
      a[j * 2] = 0;
    }
  }
}
```










**IR for Example 1**
```
define void @f(ptr %a) {
entry:
  br label %loop.i.header

loop.i.header:
  %i = phi i64 [ 0 , %entry ], [ %i.next, %loop.i.latch ]
  br label %loop.j.0.pr

loop.j.0.pr:
  %guard.j.0 = icmp slt i64 %i, 2147483640
  br i1 %guard.j.0, label %loop.j.0, label %loop.j.1

loop.j.0:
  %j.0 = phi i64 [ 0, %loop.j.0.pr ], [ %j.0.next, %loop.j.0 ]
  %val.0 = phi i64 [ %i, %loop.j.0.pr ], [ %val.0.next, %loop.j.0 ]
  %j.0.next = add nsw i64 %j.0, 1
  %idx.0 = getelementptr inbounds i8, ptr %a, i64 %val.0
  store i8 0, ptr %idx.0
  %val.0.next = add nsw i64 %val.0, 4294967296
  %exitcond.j.0 = icmp eq i64 %j.0.next, 2147483640
  br i1 %exitcond.j.0, label %loop.j.1, label %loop.j.0

loop.j.1:
  %j.1 = phi i64 [ 0, %loop.j.0 ], [ 0, %loop.j.0.pr ], [ %j.1.next, %loop.j.1 ]
  %val.1 = phi i64 [ 0, %loop.j.0 ], [ 0, %loop.j.0.pr ], [ %val.1.next, %loop.j.1 ]
  %idx.1 = getelementptr inbounds i8, ptr %a, i64 %val.1
  store i8 0, ptr %idx.1
  %j.1.next = add nsw i64 %j.1, 1
  %val.1.next = add nsw i64 %val.1, 2
  %exitcond.j.1 = icmp eq i64 %j.1.next, 2147483640
  br i1 %exitcond.j.1, label %loop.i.latch, label %loop.j.1

loop.i.latch:
  %i.next = add nsw i64 %i, 1
  %exitcond.i = icmp eq i64 %i.next, 9223372036854775806
  br i1 %exitcond.i, label %exit, label %loop.i.header

exit:
  ret void
}

```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-bug

Author: None (amehsan)

<details>
The issue was initially reported by @<!-- -->kasuga-fj during a discussion on [PR 128782](https://github.com/llvm/llvm-project/pull/128782). For the loop in Example 1  (IR at the end)  DA reports incorrect results (most notably it reports no dependency between two store instructions). It is easy to show that different variations of this loop have the same problem. See Example 2 for one of the cases.

**Example 1:**
```
; for (i = 0; i &lt; 9223372036854775806; i++) {
;   if (i &lt; 2147483640)
;     for (j = 0; j &lt; 2147483640; j++)
;       a[i + j * 4294967296] = 0;
;
;   for (j = 0; j &lt; 2147483640; j++)
;     a[j * 2] = 0;
; }
;
```

**Example 2:**
```
void foo2 (int *a ) {

  unsigned long long i;
  unsigned long long j;

  for (i = 0; i &lt; 9223372036854775806; i++) {
    for (j = 0; j &lt; 2147483640; j++) {
      a[i + j * 4294967296] = 0;
      a[j * 2] = 0;
    }
  }
}
```










**IR for Example 1**
```
define void @<!-- -->f(ptr %a) {
entry:
  br label %loop.i.header

loop.i.header:
  %i = phi i64 [ 0 , %entry ], [ %i.next, %loop.i.latch ]
  br label %loop.j.0.pr

loop.j.0.pr:
  %guard.j.0 = icmp slt i64 %i, 2147483640
  br i1 %guard.j.0, label %loop.j.0, label %loop.j.1

loop.j.0:
  %j.0 = phi i64 [ 0, %loop.j.0.pr ], [ %j.0.next, %loop.j.0 ]
  %val.0 = phi i64 [ %i, %loop.j.0.pr ], [ %val.0.next, %loop.j.0 ]
  %j.0.next = add nsw i64 %j.0, 1
  %idx.0 = getelementptr inbounds i8, ptr %a, i64 %val.0
  store i8 0, ptr %idx.0
  %val.0.next = add nsw i64 %val.0, 4294967296
  %exitcond.j.0 = icmp eq i64 %j.0.next, 2147483640
  br i1 %exitcond.j.0, label %loop.j.1, label %loop.j.0

loop.j.1:
  %j.1 = phi i64 [ 0, %loop.j.0 ], [ 0, %loop.j.0.pr ], [ %j.1.next, %loop.j.1 ]
  %val.1 = phi i64 [ 0, %loop.j.0 ], [ 0, %loop.j.0.pr ], [ %val.1.next, %loop.j.1 ]
  %idx.1 = getelementptr inbounds i8, ptr %a, i64 %val.1
  store i8 0, ptr %idx.1
  %j.1.next = add nsw i64 %j.1, 1
  %val.1.next = add nsw i64 %val.1, 2
  %exitcond.j.1 = icmp eq i64 %j.1.next, 2147483640
  br i1 %exitcond.j.1, label %loop.i.latch, label %loop.j.1

loop.i.latch:
  %i.next = add nsw i64 %i, 1
  %exitcond.i = icmp eq i64 %i.next, 9223372036854775806
  br i1 %exitcond.i, label %exit, label %loop.i.header

exit:
  ret void
}

```
</details>


---

### Comment 2 - amehsan

@kasuga-fj 

In example 1 above, you can change the condition of the loop guard to be something different and DA will still produce incorrect results. That helps to see why loop guard is not relevant to this bug in DA.


@1997alireza has looked at these cases. There is a problem with singned/unsigned comparison in different places in the code. We will add the information here soon and we are currently investigating the issue.



---

### Comment 3 - 1997alireza

The bug shows up in DA tests SIV, RDIV, and MIV.

In Example 1, DA reports no dependency between `a[i + j * 4294967296]` and itself, and none between `a[i + j * 4294967296]` and `a[j * 2]`, both incorrect. The root cause is a [computation](https://github.com/llvm/llvm-project/blob/dfad9837d6440a3274d7002a73d7b64a2b3eaedd/llvm/lib/Analysis/DependenceAnalysis.cpp#L2948) in `findBoundsALL` (used by `banerjeeMIVtest`) that multiplies the iteration count by another term and overflows.


**Example 3:**
```
for (i = 0; i < 9223372036854775806; i++) {
  for (j = 0; j < 2147483640; j++) {
    a[i] = 0;
    a[j] = 0;
  }
}
```

Here, DA again reports no dependency for `a[i]` vs itself, and none for `a[i]` vs `a[j]`.

- For `a[i]` vs itself: DA uses `Strong SIV`, where an overflow happens in a [multiplication](https://github.com/llvm/llvm-project/blob/dfad9837d6440a3274d7002a73d7b64a2b3eaedd/llvm/lib/Analysis/DependenceAnalysis.cpp#L1352) on the loop iteration count. Switching the following comparison in the next line to an unsigned predicate fixes this specific case.

- For `a[i]` vs `a[j]`: in `symbolicRDIVtest`, computing [`A1N1`](https://github.com/llvm/llvm-project/blob/dfad9837d6440a3274d7002a73d7b64a2b3eaedd/llvm/lib/Analysis/DependenceAnalysis.cpp#L2258) also overflows, similar to the previous cases.

---

### Comment 4 - amehsan


> 
> * For `a[i]` vs itself: DA uses `Strong SIV`, where an overflow happens in a [multiplication](https://github.com/llvm/llvm-project/blob/dfad9837d6440a3274d7002a73d7b64a2b3eaedd/llvm/lib/Analysis/DependenceAnalysis.cpp#L1352) on the loop iteration count. Switching the following comparison in the next line to an unsigned predicate fixes this specific case.

Could you explain why switching to unsigned comparison is appropriate fix for an overflow problem? (or maybe I misunderstood your comment?)


---

### Comment 5 - 1997alireza

> > * For `a[i]` vs itself: DA uses `Strong SIV`, where an overflow happens in a [multiplication](https://github.com/llvm/llvm-project/blob/dfad9837d6440a3274d7002a73d7b64a2b3eaedd/llvm/lib/Analysis/DependenceAnalysis.cpp#L1352) on the loop iteration count. Switching the following comparison in the next line to an unsigned predicate fixes this specific case.
> 
> Could you explain why switching to unsigned comparison is appropriate fix for an overflow problem? (or maybe I misunderstood your comment?)

I am not suggesting the unsigned comparison as a solution. In general, we are not sure if the values are positive. But it makes it more clear about the reason of the bug by showing that if we have kept the values for this specific case in unsigned SCEVs, we could not have the overflow problem.

---

### Comment 6 - kasuga-fj

As far as I can tell, similar bugs (overflow during analysis) are occurring in many parts of DA. Recently I fixed [one of them](https://github.com/llvm/llvm-project/commit/b6231f5197820026adc452707e920847364f2e6f). Probably we should apply the similar fix in other places. The problem is a bit more complicated in these cases because we use `SCEV` instead of `APInt`.

> * For `a[i]` vs itself: DA uses `Strong SIV`, where an overflow happens in a [multiplication](https://github.com/llvm/llvm-project/blob/dfad9837d6440a3274d7002a73d7b64a2b3eaedd/llvm/lib/Analysis/DependenceAnalysis.cpp#L1352) on the loop iteration count. Switching the following comparison in the next line to an unsigned predicate fixes this specific case.

Hm, considering the fact that we are using the absolute value of `Delta` and `Coef`[^abs], maybe it's okay to use unsigned here...

> * For `a[i]` vs `a[j]`: in `symbolicRDIVtest`, computing [`A1N1`](https://github.com/llvm/llvm-project/blob/dfad9837d6440a3274d7002a73d7b64a2b3eaedd/llvm/lib/Analysis/DependenceAnalysis.cpp#L2258) also overflows, similar to the previous cases.

A while ago, I tried to insert an overflow check using `ScalarEvolution::willNotOverflow` in `symbolicRDIVtest`, which resulted in almost all tests in SymbolicRDIV.ll failing. Hence, I stopped working on it and have left it as is. Of course, we could handle only the case of `SCEVConstant`, but doing just that wouldn't make much sense...

[^abs]: That aside, [the absolute value calculation is probably incorrect](https://github.com/llvm/llvm-project/pull/157738#pullrequestreview-3205085986), though...

---

### Comment 7 - amehsan

> Hm, considering the fact that we are using the absolute value of `Delta` and `Coef`[1](#user-content-fn-abs-58a2923928322b8a16872da7bcb6873c), maybe it's okay to use unsigned here...

I believe we should do this and it may fix some other bugs (I need to verify this) but it won't address the root cause of this bug. The overflow problem still needs to be addressed. 

> Of course, we could handle only the case of SCEVConstant, but doing just that wouldn't make much sense...

We are currently checking whether the bug exists when the expressions invovled are not constant. The situation is somewhat different in this case. 



---

### Comment 8 - kasuga-fj

> > Hm, considering the fact that we are using the absolute value of `Delta` and `Coef`[1](#user-content-fn-abs-58a2923928322b8a16872da7bcb6873c), maybe it's okay to use unsigned here...
> 
> I believe we should do this and it may fix some other bugs (I need to verify this) but it won't address the root cause of this bug. The overflow problem still needs to be addressed.

Yes, definitely.

> > Of course, we could handle only the case of SCEVConstant, but doing just that wouldn't make much sense...
> 
> We are currently checking whether the bug exists when the expressions invovled are not constant. The situation is somewhat different in this case.


For example, in `@symbolicrdiv0` in SymbolicRDIV.ll, the subtraction `12 * n1 - 4 * n1` is performed to compute `C2_C1`. I believe an overflow check is necessary here; otherwise, the analysis might yield incorrect results.

---

### Comment 9 - amehsan

> For example, in `@symbolicrdiv0` in SymbolicRDIV.ll, the subtraction `12 * n1 - 4 * n1` is performed to compute `C2_C1`. I believe an overflow check is necessary here; otherwise, the analysis might yield incorrect results.

If I am not mistaken this expression is used only within DA and we do not use it for the purposes of generating code in the IR. This is what we are working on right now: We check the data type of these values (`n1` and other values involved). Then based on the expression we can think of a wider data type that has X bits such that the following is true: If we extend all the values to X bit and do the calculation using X bit values there won't be any overflow (e.g. for the expression above we need 4 or 5 additional bits at most). all the calculation for DA purposes can be done this way.

(EDIT: There might be some challenges in deciding whether to zero extend or sign extend the value...we need to look into it more)


---

### Comment 10 - amehsan

> `12 * n1 - 4 * n1`

There will be more challenges when (for example) the loop upper bound is not a simple expression `n1`, but a more complex SCEV expression which itself invovles computations that may or may not overflow. We haven't yet looked into it. We are currently trying to check if the idea works in the simplest cases or not.

---

### Comment 11 - kasuga-fj

Using a sufficiently wide data type for intermediate calculations seems reasonable to me. If doing so, we probably need to check that the final result fits into the original data type. I'm also concerned about the compile-time overhead, so we might need to measure it if we decide to go this way.

> (EDIT: There might be some challenges in deciding whether to zero extend or sign extend the value...we need to look into it more)

I believe DA basically assumes signed integers so it should be sign-extend. That said, we should probably clarify the policy on which interpretation to use first. Then maybe whether it is sign-extend or zero-extend should be determined naturally?

FWIW, in the first place, DA already mishandles `sext`/`zext` in some cases. For example, see https://godbolt.org/z/Ydvosr4qT. While it ultimately crashes, the debug output shows that something quite strange is happening: The original offset contains `(zext ...)`, but just before crashing, DA somehow attempts to analyze `(sext ...)`. Probably the following is happening:

- The original offset value contains `(zext i1 {false,+,true}<%loop> to i64)` as its subexpression
- Delinearization extracts this part
- `zext` is purged by `removeMatchExtensions` and it's transformed into `{false,+,true}<%loop>`, which I think is incorrect
- At some point, `unifySubscriptType` is called and results in `(sext i1 {false,+,true}<%loop> to i64)`, which is obviously wrong value with the original zero-extended one


> There will be more challenges when (for example) the loop upper bound is not a simple expression `n1`, but a more complex SCEV expression which itself invovles computations that may or may not overflow.

It seems that in the worst case the required bitwidth increases linearly with the number of multiplications. For example, for the product `x0 * x1 * x2 * x3` where all operands are 64-bit integers, we might need 64 * 4 = 256 bits?

---

### Comment 12 - kasuga-fj

By the way, going back a bit, I still think some kind of check on loop guards is necessary, because:

- DA needs to ensure that an AddRec doesn't wrap, which is not checked now.
    - Rationale: the underlying algorithm assumes the property. In principle, DA cannot handle (potentially) wrapping AddRecs, e.g., `{false,+,true}<%loop>` with its backedge-taken count is greater than 1. That is, it doesn't expect non-linear expressions.
- If an AddRec consists of only constants and backedge-taken count is also a constant, then probably we can check it easily. However, if some non-constant values are involved, the only option may be to reason about it from `nuw`/`nsw` attached to the AddRec.
- Looking at Example 1, it appears that those flags are justified under the condition to enter the loop (`0 <= i < 2147483640`), not the entire iteration space (`0 <= i < 9223372036854775806`).

I now think if there is a loop-guard that involves an induction variable, the analysis should bail out (not sure about a case where the condition of the loop-guard is loop-invariant). However, the implementation does not look trivial.

---

### Comment 13 - amehsan

> By the way, going back a bit, I still think some kind of check on loop guards is necessary, because:

Please provide an example that supports the claim. The examples 1, 2, and 3 here are not related to loop guard.

---

### Comment 14 - amehsan

> Using a sufficiently wide data type for intermediate calculations seems reasonable to me. If doing so, we probably need to check that the final result fits into the original data type. I'm also concerned about the compile-time overhead, so we might need to measure it if we decide to go this way.

Yes, we definitely need to consider compile time.  As for the final result fitting in to the original data type, I am not sure. That needs to be decided case by case. For now our plan is to create the patch for one case. That will help the discussion. 

---

### Comment 15 - amehsan

> I believe DA basically assumes signed integers so it should be sign-extend. That said, we should probably clarify the policy on which interpretation to use first. Then maybe whether it is sign-extend or zero-extend should be determined naturally?

This may be right. We will go case by case and post everything for review in small increments. I think it is better to judge every case separately (At least in the beginning) and later on if we can infer a general rule that is provably always correct we can rely on that.

---

### Comment 16 - kasuga-fj

> > By the way, going back a bit, I still think some kind of check on loop guards is necessary, because:
> 
> Please provide an example that supports the claim. The examples 1, 2, and 3 here are not related to loop guard.

Not yet. It's something we'll need in the future to prove that an AddRec doesn't wrap in the entire iteration space, but we have not even checked the case where there's no loop guard. Any concrete example should come after that.

So, what does it mean for an AddRec to wrap? Consider the following case:

```llvm
; for (i = 0; i < (1ULL << 60); i++) {
;   A[i] = 1;
;
;   unsigned long long offset = i * 32 + (1ULL << 62);
;   // offset is positive when interpreted as a signed value.
;   // To prevent violating the size limitation for an allocated object.
;   if (offset < (1ULL << 63))
;     A[offset] = 2;
; }
define void @f(ptr %A) {
entry:
  %tc = shl i64 1, 60
  %const.1 = shl i64 1, 62
  br label %loop.header

loop.header:
  %i = phi i64 [ 0, %entry ], [ %i.next, %loop.latch ]
  %idx.0 = getelementptr inbounds i8, ptr %A, i64 %i
  store i8 1, ptr %idx.0
  %offset.tmp = mul i64 %i, 32
  %offset = add i64 %offset.tmp, %const.1
  %if.cond = icmp sge i64 %offset, 0
  br i1 %if.cond, label %if.then, label %loop.latch

if.then:
  %idx.1 = getelementptr inbounds i8, ptr %A, i64 %offset
  store i8 2, ptr %idx.1
  br label %loop.latch

loop.latch:
  %i.next = add nuw nsw i64 %i, 1
  %exit.cond = icmp eq i64 %i.next, %tc
  br i1 %exit.cond, label %exit, label %loop.header

exit:
  ret void
}
```

There are positive and negative dependencies between the two stores, e.g.,

| Location | `A[i]` | `A[i * 32 + (1ULL << 62)]` | direction |
|-|-|-|-|
| `A[0]` | `i = 0` | `i = 2^59 - 2^57` | positive |
| `A[2^60 - 32]` | `i = 2^60 - 32` | `i = 2^59 - 2^57 + 2^55 - 1` | negative |

I checked the current behavior of `symbolicRDIVtest` locally by disabling other tests (like `exactRDIVtest`). Here's the debug output:

```
Src:  store i8 1, ptr %idx.0, align 1 --> Dst:  store i8 2, ptr %idx.1, align 1
  da analyze -     SrcSCEV = {%A,+,1}<nuw><%loop.header>
    DstSCEV = {(4611686018427387904 + %A),+,32}<%loop.header>
    common nesting levels = 1
    maximum nesting levels = 1
    SameSD nesting levels = 0
    subscript 0
        src = {0,+,1}<nuw><nsw><%loop.header>
        dst = {4611686018427387904,+,32}<%loop.header>
        class = 1
        loops = {1}
    Separable = {0}
    Coupled = {}
testing subscript 0, SIV
    src = {0,+,1}<nuw><nsw><%loop.header>
    dst = {4611686018427387904,+,32}<%loop.header>
        try symbolic RDIV test
            A1 = 1, type = i64
            A2 = 32
            C1 = 0
            C2 = 4611686018427387904
            N1 = 1152921504606846975
            N2 = 1152921504606846975
            C2 - C1 = 4611686018427387904
            C1 - C2 = -4611686018427387904
            A1*N1 = 1152921504606846975
none!
```

Apparently, the dependency is disproved [here](https://github.com/llvm/llvm-project/blob/5e07093917fa84b3ee9d09189a34a0c750f68cb7/llvm/lib/Analysis/DependenceAnalysis.cpp#L2259-L2263). Notably, intermediate computations (`C2 - C1`, `A1*N1`, etc.) don't overflow. The root cause would be that `symbolicRDIVtest` assumes the subscripts are monotonic, but in this case `i * 32 + (1ULL << 62)` is not. This applies all other functions. Thus we need to check the monotonicity first, which I'm working on https://github.com/llvm/llvm-project/pull/154527. I think Example 2 should be bail out at an earlier stage since `i + j * 4294967296` is not monotonic.
Now, the following question comes to my mind: is the offset `i + j * 4294967296` in Example 1 monotonic? I think it is monotonic under the condition `i < 2147483640`, but not the entire iteration space (`i < 9223372036854775806`). This is why I'm considering loop guards should be examined.  If the offset and/or backedge-taken count are not constant values, we may have no choice but to rely on `nuw`/`nsw` of an AddRec. But Example 1 implies that these flags hold under the loop guard, not in the entire iteration space.

---

### Comment 17 - amehsan

I know what it means for an AddRec to wrap. I haven't looked closely at your montonicity work, but at a high level it sounds reasonable to me (I will try to look into it more closely when I have time).  As for the loop guards, if I understand correctly, now you say it may be useful to consider loopguards for more accurate analysis (as opposed to using loop guards for correctness of DA). In principle this correct, but:

> I think it is monotonic under the condition i < 2147483640, but not the entire iteration space (i < 9223372036854775806)

You have an example where AddRec is not monotonic in the entire iteration space but is monotonic if the loop guard is considered. What do we lose if we conservatively return a correct result saying that src and dst are dependent on each other with unknown distance/direction? We do not try to write a perfect DA that gives the most accurate results for any arbitrary loop. There are tradeoffs to make, and so if the issue is precision/performance then: (1) there should be a reason to believe the problem you want to solve has performance implications for real world programs. (2) we should investigate alternative solutions to solve the performance problem and choose the best one. 

---

### Comment 18 - kasuga-fj

I believe both Example 1 and Example 2 should be treated as non-monotonic, and the DA should conservatively return an unknown dependency in these cases. I'm not suggesting that we inspect the *contents* of loop guards. What I mean is that we should ensure the *absence* of loop guards -- otherwise, we can't properly handle cases where an AddRec or backedge-taken count involves non-constant loop-invariant values.

Consider the following two examples:

```c
#define F(i, j) ... // some affine expression of i and j

void loop1(int *A, int Ni, int Nj, int M) {
  for (int i = 0; i < Ni; i++)
    if (i < M)
      for (int j = 0; j < Nj; j++) {
        A[F(i, j)] = ...;
        ...
      }
}

void loop2(int *A, int Ni, int Nj, int M) {
  for (int i = 0; i < Ni; i++)
    for (int j = 0; j < Nj; j++) {
      A[F(i, j)] = ...;
      ...
    }
}
```

As for loop1, I believe DA should conservatively return an unknown dependency, same as Example 1. However, I think `F(i, j)` in loop2 is monotonic over the entire iteration space, and we probably want to be able to handle such cases. So, how to prove the monotonicity? I believe the following two conditions must be checked:

- The corresponding AddRec has `nsw` flags for both the i-loop and j-loop.
- There is no loop guard (like `i < M` in loop1) in the loop nest.
    - If there is a loop guard, the `nsw` holds only under the condition imposed by the loop guard, not across the entire iteration space, as Example 1 suggests.

I believe the absence of loop guards can be checked by using DomTree. When I previously said that the implementation isn't trivial, I meant that maybe some loop guards can be ignored (e.g. when the condition is loop-invariant, like `Ni < 10`), and handling them properly seems rather complicated. But I don't plan to pursue such a complicated implementation.

An alternative approach is to pessimistically assume the presence of loop guards and return unknown dependency whenever non-constant values are involved in the offsets and/or backedge-taken counts. I think this is too pessimistic...

---

### Comment 19 - amehsan

Let me first make sure I understand your statement. Then we can discuss it. For now please let me know if the following is correct or not:

Your concern is about the case that 
(1) loop has a guard
(2) SCEV takes loop guard into account when computing nsw/nuw flags for AddRec expression of the subscripts.
(3) At least one induction variable that is involved in the subscript is also invovled in the loop guard.

Is my statement above is correct or not.

I also tried to create an example. Let me know what you think about this example:


```
void foo (char *A, char *B, unsigned int n, unsigned m) {

   for (unsigned int i = 0; i < n; i++) {

     if (i < 3000) {
       for (unsigned int j = 0; j <  i ; j++) {
         A[i+j] = 0;
         B[j] = A[j+2*i];
       }
     }
 }

```

In this example (Compiled with `-O3 -fno-vectorize`) if I remove the condition `if (i < 3000)` then the `SrcSCEV` and `DsrSCEV` expressions in DA will be different  For example  I see the following in the debug output (when the loop guards is removed)

```
Src:  store i8 0, ptr %arrayidx, align 1, !tbaa !8 --> Dst:  %0 = load i8, ptr %arrayidx7, align 1, !tbaa !8
  da analyze -     SrcSCEV = ((zext i32 {{0,+,1}<nuw><%for.cond1.preheader>,+,1}<%for.body4> to i64) + %A)<nuw>
    DstSCEV = ((zext i32 {{0,+,2}<%for.cond1.preheader>,+,1}<%for.body4> to i64) + %A)<nuw>
```

But with the loop guard:

```
Src:  store i8 0, ptr %gep, align 1, !tbaa !6 --> Dst:  %2 = load i8, ptr %gep39, align 1, !tbaa !6
da analyze -     SrcSCEV = {{%A,+,1}<nw><%for.body>,+,1}<nuw><%for.body5>
    DstSCEV = {{%A,+,2}<nw><%for.body>,+,1}<nuw><%for.body5>
```

Clearly in these two cases, SCEV expressions are different and very clearly wrapping flags are used differently too.


So please answer two short questions:

(a) Does the three condition (1) (2) and (3) above characterize a case of your concern or not. (If your concern covers wider cases that is fine. For now we can focus on this part)
(b) Is the example I provided an example that demonstrates your concern or not


Let's first make sure we agree on the basics of the discussion. Then we can move ahead and discuss further.

---

### Comment 20 - kasuga-fj

> (a) Does the three condition (1) (2) and (3) above characterize a case of your concern or not. (If your concern covers wider cases that is fine. For now we can focus on this part)

As for (1) and (2), Yes. For (3), yes, but I think there are other cases to be considered (and I believe this is what you meant by "If your concern covers wider cases that is fine").

> (b) Is the example I provided an example that demonstrates your concern or not

I'm not entirely sure what happens in your example, but the answer might be No. It doesn’t seem to be an example that *accurately* reproduces (1), (2) and (3). But I think the intention comes across.

Maybe it is a bit difficult to make an example from C/C++ that exactly demonstrates the above points. I think it would be better to create one in LLVM IR to rule out other factors. What I have in mind is something like the following:

```llvm
define void @with_loop_guard(...) {
...
loop.i.header:
  ...
  br label %loop.j.preheader

loop.j.preheader:
  %cond = icmp slt i64 %i, 3000
  br i1 %cond, label %loop.j, label %loop.i.latch

loop.j:
  ...
  %subscript.0 = add nuw nsw i64 %i, %j           ; not poison, since %i + %j < %i + %i < 3000 + 3000
  %subscript.1 = add nuw nsw i64 %subscript.0, %i ; not poison, similarly
  %gep0 = getelementptr inbounds nuw i8, ptr %A, i64 %subscript.0
  %gep1 = getelementptr inbounds nuw i8, ptr %A, i64 %subscript.1 
  ... ; store to %gep0, load from %gep1, etc.
}
```

I believe that the SCEVs for `%subscript.0` and `%subscript.1` will have both `nuw` and `nsw` flags, and they should only hold due to the presence of the loop-guard. Does it make sense to you?

---

### Comment 21 - amehsan


> 
> loop.j:
>   ...
>   %subscript.0 = add nuw nsw i64 %i, %j           ; not poison, since %i + %j < %i + %i < 3000 + 3000
>   %subscript.1 = add nuw nsw i64 %subscript.0, %i ; not poison, similarly
>   %gep0 = getelementptr inbounds nuw i8, ptr %A, i64 %subscript.0
>   %gep1 = getelementptr inbounds nuw i8, ptr %A, i64 %subscript.1 
>   ... ; store to %gep0, load from %gep1, etc.
> }
> I believe that the SCEVs for `%subscript.0` and `%subscript.1` will have both `nuw` and `nsw` flags, and they should only hold due to the presence of the loop-guard. Does it make sense to you?


Yes, my example is not exactly what you are talking about. You REALLY need an example. If you need to write manual IR that is fine. But an incomplete example that you **_believe_** will work as you think is not enough.

Does any of the examples in https://github.com/llvm/llvm-project/pull/162281/ demonstrate the issue?

---

### Comment 22 - amehsan

>  my example is not exactly what you are talking about

I spent some time trying to create one. But I was not able to. It seems to me that you are talking about a hypothetical situation for which you cannot provide an example. Either in C/C++ or in the LLVM IR. 

---

### Comment 23 - amehsan

> I believe both Example 1 and Example 2 should be treated as non-monotonic, and the DA should conservatively return an unknown dependency in these cases.

That is fine. If the indices are not monotonic we can conservativey return unknown dependence for both cases. We don't need to know anything about the loop guards. 

---

### Comment 24 - kasuga-fj

I thought my previous rough example was enough, but it seems it wasn't...
I’ve now prepared a complete example, would this one meet your expectations?

```llvm
; for (int i = 0; i < n; i++)
;   if (i < 3000)
;     for (int j = 0; j < 100; j++)
;       A[i + j] = 0;
define void @with_loop_guard(ptr %A, i64 %n) {
entry:
  br label %loop.i.header

loop.i.header:
  %i = phi i64 [ 0, %entry ], [ %i.inc, %loop.i.latch ]
  br label %loop.j.preheader

loop.j.preheader:
  %cond = icmp slt i64 %i, 3000
  br i1 %cond, label %loop.j, label %loop.i.latch

loop.j:
  %j = phi i64 [ 0, %loop.j.preheader ], [ %j.inc, %loop.j ]
  %subscript = add nuw nsw i64 %i, %j  ; 0 <= %i + %j < 3000 + 100
  %gep = getelementptr inbounds i8, ptr %A, i64 %subscript
  store i8 0, ptr %gep
  %j.inc = add nuw nsw i64 %j, 1
  %ec.j = icmp eq i64 %j.inc, 100
  br i1 %ec.j, label %loop.i.latch, label %loop.j

loop.i.latch:
  %i.inc = add nsw nuw i64 %i, 1
  %ec.i = icmp eq i64 %i.inc, %n
  br i1 %ec.i, label %exit, label %loop.i.header

exit:
  ret void
}
```

godbolt: https://godbolt.org/z/o1ez9sETr

Here's a portion of debug output
 
```
    subscript 0
	src = {{0,+,1}<nuw><nsw><%loop.i.header>,+,1}<nuw><nsw><%loop.j>
	dst = {{0,+,1}<nuw><nsw><%loop.i.header>,+,1}<nuw><nsw><%loop.j>
```

I believe this IR is valid, and the following holds:

- As you can see from the debug output, the AddRec has `nsw`, which is necessary to prove monotonicity in some cases.
- Passing `INT64_MAX` to `%n` is allowed, and it doesn't trigger UB.
- When actually `INT64_MAX` is passed, `%subscript` is monotonic only under the condition `%i < 3000`, but not for all `%i`.

I believe this case should also be considered non-monotonic, but how can we detect it? I don’t have any idea other than checking for the presence of a loop guard.

---

### Comment 25 - kasuga-fj

I think the following example, unlike the previous one (`@with_loop_guard`), should be considered monotonic, and I think the only way to infer that is by checking the `nsw` flag.

```llvm
; for (int i = 0; i < n; i++)
;   for (int j = 0; j < 100; j++)
;     A[i + j] = 0;
define void @no_loop_guard(ptr %A, i64 %n) {
entry:
  br label %loop.i.header

loop.i.header:
  %i = phi i64 [ 0, %entry ], [ %i.inc, %loop.i.latch ]
  br label %loop.j

loop.j:
  %j = phi i64 [ 0, %loop.i.header ], [ %j.inc, %loop.j ]
  %subscript = add nuw nsw i64 %i, %j  ; 0 <= %i + %j < 3000 + 100
  %gep = getelementptr inbounds i8, ptr %A, i64 %subscript
  store i8 0, ptr %gep
  %j.inc = add nuw nsw i64 %j, 1
  %ec.j = icmp eq i64 %j.inc, 100
  br i1 %ec.j, label %loop.i.latch, label %loop.j

loop.i.latch:
  %i.inc = add nsw nuw i64 %i, 1
  %ec.i = icmp eq i64 %i.inc, %n
  br i1 %ec.i, label %exit, label %loop.i.header

exit:
  ret void
}
```

That is,

- In some cases (including this one), the only way to prove monotonicity is by checking the `nsw` flag.
- On the other hand, it doesn’t necessarily hold over the entire iteration space, as you can see from `@with_loop_guard`. To infer monotonicity from `nsw`, we need to ensure that it holds throughout the entire iteration space.

---

### Comment 26 - amehsan

`@no_loop_guard` will trigger UB when `%n` is large enough. Results of DA is not required to be correct when the program triggers UB. 

`@with_loop_guard` gives you AddRec with all nsw or nuw that you may want. By analyzing the AddRec you can prove monotonicity.

That is why we had difficulty creating an example from C/C++. The compiler won't generate invalid IR.

---

### Comment 27 - amehsan

> `@with_loop_guard` gives you AddRec with all nsw or nuw that you may want. By analyzing the AddRec you can prove monotonicity.

I mean without looking at the loop guard.

---

### Comment 28 - kasuga-fj

> `@no_loop_guard` will trigger UB when `%n` is large enough. Results of DA is not required to be correct when the program triggers UB.

I'm not sure what you mean. I think the program is still well-defined for a sufficiently small value of `%n`, and I believe this is a fairly typical case. I presented this example as a case where monotonicity can be inferred from no-wrap flags. (and I believe this is the only viable way to reason about it).

> `@with_loop_guard` gives you AddRec with all nsw or nuw that you may want. By analyzing the AddRec you can prove monotonicity.

I'm saying that this is NOT monotonic, even though the AddRec has no-wrap flags. These flags (at least `nsw`) are valid only under the assumption that `i < 3000`. However, monotonicity should hold across the entire iteration space. What I want to emphasize is that, inferring monotonicity from no-wrap flags is valid only when there is no loop-guard restricting the iteration space.

> That is why we had difficulty creating an example from C/C++. The compiler won't generate invalid IR.

I'm not sure what you mean by "invalid IR", but I think the difficulty arises from the influence of other passes. In the context of correctness issues, I believe it's better to write the IR directly to eliminate such external factors.

---

### Comment 29 - nikic

> I'm saying that this is NOT monotonic, even though the AddRec has no-wrap flags. These flags (at least nsw) are valid only under the assumption that i < 3000. However, monotonicity should hold across the entire iteration space. What I want to emphasize is that, inferring monotonicity from no-wrap flags is valid only when there is no loop-guard restricting the iteration space.

I haven't followed all the discussions around this, so this is probably a stupid question: Why does monotonicity have to hold for the entire iteration space if there is a loop guard that constrains the iteration space?

I guess I'm a bit unclear on what the difference between `for (i = 0; i < 3000; i++) { foo; }` and `for (i = 0; i < n; i++) if (i < 3000) { foo; }` is.

---

### Comment 30 - kasuga-fj

> > I'm saying that this is NOT monotonic, even though the AddRec has no-wrap flags. These flags (at least nsw) are valid only under the assumption that i < 3000. However, monotonicity should hold across the entire iteration space. What I want to emphasize is that, inferring monotonicity from no-wrap flags is valid only when there is no loop-guard restricting the iteration space.
> 
> I haven't followed all the discussions around this, so this is probably a stupid question: Why does monotonicity have to hold for the entire iteration space if there is a loop guard that constrains the iteration space?
> 
> I guess I'm a bit unclear on what the difference between `for (i = 0; i < 3000; i++) { foo; }` and `for (i = 0; i < n; i++) if (i < 3000) { foo; }` is.

Good question. The short answer is: Because DA uses the BTC in various places, and some of its uses are effectively to evaluate an AddRec at the last iteration.

Theoretically, I also believe we don't need to prove monotonicity over the entire iteration space if a loop guard constrains it. For example, in a loop like `for (i = 0; i < n; i++) if (i < 3000) { foo; }`, we might treat the "effective" BTC as 3000 instead of `n`. However, computing such an "effective" BTC is not trivial, especially when the loop guard involves more complex expressions, e.g., `i * i + 2 * i < 3000`.

In any case, my argument is that some form of loop guard analysis is necessary to prove monotonicity. There are various ways to handle loop guards, but I think the simplest option is to bail out when one is present.

Does this make sense?

---

Since we're here, may I ask one question about SCEV? In the following code

```llvm
; for (int i = 0; i < n; i++)
;   if (i < 3000)
;     for (int j = 0; j < 100; j++)
;       A[i + j] = 0;
define void @with_loop_guard(ptr %A, i64 %n) {
entry:
  br label %loop.i.header

loop.i.header:
  %i = phi i64 [ 0, %entry ], [ %i.inc, %loop.i.latch ]
  br label %loop.j.preheader

loop.j.preheader:
  %cond = icmp slt i64 %i, 3000
  br i1 %cond, label %loop.j, label %loop.i.latch

loop.j:
  %j = phi i64 [ 0, %loop.j.preheader ], [ %j.inc, %loop.j ]
  %subscript = add nuw nsw i64 %i, %j  ; 0 <= %i + %j < 3000 + 100
  %gep = getelementptr inbounds i8, ptr %A, i64 %subscript
  store i8 0, ptr %gep
  %j.inc = add nuw nsw i64 %j, 1
  %ec.j = icmp eq i64 %j.inc, 100
  br i1 %ec.j, label %loop.i.latch, label %loop.j

loop.i.latch:
  %i.inc = add nsw nuw i64 %i, 1
  %ec.i = icmp eq i64 %i.inc, %n
  br i1 %ec.i, label %exit, label %loop.i.header

exit:
  ret void
}
```

The SCEV for `%subscript` is `{{0,+,1}<nuw><nsw><%loop.i.header>,+,1}<nuw><nsw><%loop.j>` ([godbolt](https://godbolt.org/z/YW1oEcMns)). In my understanding, the no-wrap flags for `<%loop.j>` are valid under the loop guard condition `i < 3000`, but doesn't necessarily hold for `3000 <= i < n`. Is this correct?

---

