# risc-v backend miscompile of cttz-related optimization

**Author:** regehr
**URL:** https://github.com/llvm/llvm-project/issues/155014
**Status:** Closed
**Labels:** backend:RISC-V, miscompilation
**Closed Date:** 2025-09-03T01:13:55Z

## Body

this function:
```
declare i7 @llvm.cttz.i7(i7, i1 immarg)

define i7 @f(i7 %0) {
  %2 = call i7 @llvm.cttz.i7(i7 %0, i1 false)
  %3 = icmp eq i7 %0, 0
  %4 = select i1 %3, i7 0, i7 %2
  ret i7 %4
}
```
is getting miscompiled by the RISCV64+B backend as:
```
f:
	ori	a0, a0, 128
	ctz	a0, a0
	andi	a0, a0, 6
	ret
```
the `6` in the output doesn't make sense, it is coming from an insufficiently-protected `Bitwidth - 1`. 

@MitchBriles, a student working with me, has the following candidate patch. it certainly fixes this case but we've not fully vetted it yet. actually, if nobody is in a rush to fix this, Mitch would like to be the one to submit the PR for it.

```
diff --git a/llvm/lib/Target/RISCV/RISCVISelLowering.cpp b/llvm/lib/Target/RISCV/RISCVISelLowering.cpp
index 4a1db80076..7a5ebf74c4 100644
--- a/llvm/lib/Target/RISCV/RISCVISelLowering.cpp
+++ b/llvm/lib/Target/RISCV/RISCVISelLowering.cpp
@@ -18740,6 +18740,10 @@ static SDValue foldSelectOfCTTZOrCTLZ(SDNode *N, SelectionDAG &DAG) {
   if (Cond->getOperand(0) != CountZeroesArgument)
     return SDValue();
 
+  unsigned BitWidth = CountZeroes.getValueSizeInBits();
+  if (!isPowerOf2_32(BitWidth))
+    return SDValue();
+
   if (CountZeroes.getOpcode() == ISD::CTTZ_ZERO_UNDEF) {
     CountZeroes = DAG.getNode(ISD::CTTZ, SDLoc(CountZeroes),
                               CountZeroes.getValueType(), CountZeroesArgument);
@@ -18748,7 +18752,6 @@ static SDValue foldSelectOfCTTZOrCTLZ(SDNode *N, SelectionDAG &DAG) {
                               CountZeroes.getValueType(), CountZeroesArgument);
   }
 
-  unsigned BitWidth = CountZeroes.getValueSizeInBits();
   SDValue BitWidthMinusOne =
       DAG.getConstant(BitWidth - 1, SDLoc(N), CountZeroes.getValueType());
 ```


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-bug

Author: John Regehr (regehr)

<details>
this function:
```
declare i7 @<!-- -->llvm.cttz.i7(i7, i1 immarg)

define i7 @<!-- -->f(i7 %0) {
  %2 = call i7 @<!-- -->llvm.cttz.i7(i7 %0, i1 false)
  %3 = icmp eq i7 %0, 0
  %4 = select i1 %3, i7 0, i7 %2
  ret i7 %4
}
```
is getting miscompiled by the RISCV64+B backend as:
```
f:
	ori	a0, a0, 128
	ctz	a0, a0
	andi	a0, a0, 6
	ret
```
the `6` in the output doesn't make sense, it is coming from an insufficiently-protected `Bitwidth - 1`. 

@<!-- -->MitchBriles, a student working with me, has the following candidate patch. it certainly fixes this case but we've not fully vetted it yet. actually, if nobody is in a rush to fix this, Mitch would like to be the one to submit the PR for it.

```
diff --git a/llvm/lib/Target/RISCV/RISCVISelLowering.cpp b/llvm/lib/Target/RISCV/RISCVISelLowering.cpp
index 4a1db80076..7a5ebf74c4 100644
--- a/llvm/lib/Target/RISCV/RISCVISelLowering.cpp
+++ b/llvm/lib/Target/RISCV/RISCVISelLowering.cpp
@@ -18740,6 +18740,10 @@ static SDValue foldSelectOfCTTZOrCTLZ(SDNode *N, SelectionDAG &amp;DAG) {
   if (Cond-&gt;getOperand(0) != CountZeroesArgument)
     return SDValue();
 
+  unsigned BitWidth = CountZeroes.getValueSizeInBits();
+  if (!isPowerOf2_32(BitWidth))
+    return SDValue();
+
   if (CountZeroes.getOpcode() == ISD::CTTZ_ZERO_UNDEF) {
     CountZeroes = DAG.getNode(ISD::CTTZ, SDLoc(CountZeroes),
                               CountZeroes.getValueType(), CountZeroesArgument);
@@ -18748,7 +18752,6 @@ static SDValue foldSelectOfCTTZOrCTLZ(SDNode *N, SelectionDAG &amp;DAG) {
                               CountZeroes.getValueType(), CountZeroesArgument);
   }
 
-  unsigned BitWidth = CountZeroes.getValueSizeInBits();
   SDValue BitWidthMinusOne =
       DAG.getConstant(BitWidth - 1, SDLoc(N), CountZeroes.getValueType());
 ```

</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-backend-risc-v

Author: John Regehr (regehr)

<details>
this function:
```
declare i7 @<!-- -->llvm.cttz.i7(i7, i1 immarg)

define i7 @<!-- -->f(i7 %0) {
  %2 = call i7 @<!-- -->llvm.cttz.i7(i7 %0, i1 false)
  %3 = icmp eq i7 %0, 0
  %4 = select i1 %3, i7 0, i7 %2
  ret i7 %4
}
```
is getting miscompiled by the RISCV64+B backend as:
```
f:
	ori	a0, a0, 128
	ctz	a0, a0
	andi	a0, a0, 6
	ret
```
the `6` in the output doesn't make sense, it is coming from an insufficiently-protected `Bitwidth - 1`. 

@<!-- -->MitchBriles, a student working with me, has the following candidate patch. it certainly fixes this case but we've not fully vetted it yet. actually, if nobody is in a rush to fix this, Mitch would like to be the one to submit the PR for it.

```
diff --git a/llvm/lib/Target/RISCV/RISCVISelLowering.cpp b/llvm/lib/Target/RISCV/RISCVISelLowering.cpp
index 4a1db80076..7a5ebf74c4 100644
--- a/llvm/lib/Target/RISCV/RISCVISelLowering.cpp
+++ b/llvm/lib/Target/RISCV/RISCVISelLowering.cpp
@@ -18740,6 +18740,10 @@ static SDValue foldSelectOfCTTZOrCTLZ(SDNode *N, SelectionDAG &amp;DAG) {
   if (Cond-&gt;getOperand(0) != CountZeroesArgument)
     return SDValue();
 
+  unsigned BitWidth = CountZeroes.getValueSizeInBits();
+  if (!isPowerOf2_32(BitWidth))
+    return SDValue();
+
   if (CountZeroes.getOpcode() == ISD::CTTZ_ZERO_UNDEF) {
     CountZeroes = DAG.getNode(ISD::CTTZ, SDLoc(CountZeroes),
                               CountZeroes.getValueType(), CountZeroesArgument);
@@ -18748,7 +18752,6 @@ static SDValue foldSelectOfCTTZOrCTLZ(SDNode *N, SelectionDAG &amp;DAG) {
                               CountZeroes.getValueType(), CountZeroesArgument);
   }
 
-  unsigned BitWidth = CountZeroes.getValueSizeInBits();
   SDValue BitWidthMinusOne =
       DAG.getConstant(BitWidth - 1, SDLoc(N), CountZeroes.getValueType());
 ```

</details>


---

### Comment 3 - lenary

Please do have Mitch go ahead and submit that patch. We're at a good point in the release cycle that this doesn't have to be fixed yesterday.

The preferred way the PR should be structured is similar to instcombine PRs: a commit that adds (in this case, `llc` tests), and then a second commit that fixes the issue and updates the test output. Use of `llvm/utils/update_llc_test_checks.py` is encouraged



---

### Comment 4 - lenary

(If mitch comments on this issue, I can assign it to him)

---

### Comment 5 - MitchBriles

Great, thanks. I'll start working on a PR.

---

