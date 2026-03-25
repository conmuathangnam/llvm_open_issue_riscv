# [clang] Miscompilation at -O3

**Author:** cardigan1008
**URL:** https://github.com/llvm/llvm-project/issues/119173
**Status:** Closed
**Labels:** miscompilation, release:backport, vectorizers
**Closed Date:** 2025-02-04T09:37:45Z

## Body

This code prints 15 at -O3, but prints 5 at -O0/1/2:
```c
int printf(const char *, ...);
static int a[] = {4294967295, 5};
int b, c;
int main() {
  a[1] = b = 5;
  unsigned d = -13;
  for (; d >= 8; d = a[0] + d + 6) {
    int *e = &b;
    *e = a[0] - -1 + b;
  }
  a[c];
  printf("%d\n", b);
}
```

Compiler Explorer: https://godbolt.org/z/4xdPqcjPb

Bisected to https://github.com/llvm/llvm-project/commit/6d6eea92e36c301e34a7ec11b2a40e3080f79f53, which was committed by @fhahn 

## Comments

### Comment 1 - hstk30-hw

Don't ignore the warning.

```
<source>:2:19: warning: implicit conversion from 'long' to 'int' changes value from 4294967294 to -2 [-Wconstant-conversion]
    2 | static int a[] = {4294967294, 5};
      |                  ~^~~~~~~~~~
```

---

### Comment 2 - antoniofrighetto

Please leave the issue opened, as it's implementation-defined behaviour, not undefined behaviour.

---

### Comment 3 - antoniofrighetto

Reduced to:
```llvm
target triple = "x86_64-unknown-linux-gnu"

define noundef i32 @src() {
entry:
  br label %for.body

for.body:                                         ; preds = %entry, %for.body
  %d.06 = phi i32 [ -13, %entry ], [ %add2.reass, %for.body ]
  %add45 = phi i32 [ 5, %entry ], [ %add, %for.body ]
  %add = add i32 0, %add45
  %add2.reass = add i32 %d.06, 5
  %cmp = icmp ugt i32 %add2.reass, 7
  br i1 %cmp, label %for.body, label %for.end

for.end:                                          ; preds = %for.body
  ret i32 %add
}
```

@fhahn I think the issue fundamentally here lies in the fact that we are incorrectly widening the constant 5 (retrieved from SCEV) to `<5, 5, 5, 5>`, when in fact it should be `<5, 0, 0, 0>` (VPlan value: `WIDEN-REDUCTION-PHI ir<%add45> = phi ir<5>`). The VPBasicBlock for the miscompiled basic block is the following one:
```
vector.body:
  EMIT vp<%4> = CANONICAL-INDUCTION ir<0>, vp<%7>
  WIDEN-INDUCTION %d.06 = phi -13, %add2.reass, ir<5>, vp<%0>
  WIDEN-REDUCTION-PHI ir<%add45> = phi ir<5>
  EMIT vp<%5> = WIDEN-CANONICAL-INDUCTION vp<%4>
  EMIT vp<%6> = icmp ule vp<%5>, vp<%3>
```
I'm not that familiar with VPlan, though I feel like that either we are not taking into account the active lane, or we should have not emitted a widen instruction in the first place.

(Godbolt: https://llvm.godbolt.org/z/4Wz46ceh1)

---

### Comment 4 - antoniofrighetto

@fhahn Would you be willing to have a look at it? Happy to help with any additional triage, if needed.

---

### Comment 5 - fhahn

Yep let me take a look!

---

### Comment 6 - fhahn

/cherry-pick e83e0c300d6f2cc41c9c74ed4fce90272acef16a 30f3752e54fa7cd595a434a985efbe9a7abe9b65

---

### Comment 7 - llvmbot

<!--IGNORE-->
Failed to cherry-pick: e83e0c300d6f2cc41c9c74ed4fce90272acef16a

https://github.com/llvm/llvm-project/actions/runs/13119736028

Please manually backport the fix and push it to your github fork.  Once this is done, please create a [pull request](https://github.com/llvm/llvm-project/compare)

---

### Comment 8 - fhahn

/cherry-pick 30f3752e54fa7cd595a434a985efbe9a7abe9b65

---

### Comment 9 - llvmbot

/pull-request llvm/llvm-project#125659

---

