# [X86] error in backend: Cannot emit physreg copy instruction

**Author:** Andarwinux
**URL:** https://github.com/llvm/llvm-project/issues/181130
**Status:** Closed
**Labels:** crash, llvm:SelectionDAG
**Closed Date:** 2026-02-14T13:05:32Z

## Body

[test.ll.txt](https://github.com/user-attachments/files/25261699/test.ll.txt)
```
fatal error: error in backend: Cannot emit physreg copy instruction
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /build/install/llvmbin/clang --target=x86_64-pc-windows-gnu -O3 -march=tigerlake test.ll -c -o /dev/null
1.      Code generation
2.      Running pass 'Function Pass Manager' on module 'test.ll'.
3.      Running pass 'Post-RA pseudo instruction expansion pass' on function '@svt_aom_sig_deriv_me_tf'
#0 0x000059b4137ea323 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) ld-temp.o:0:0
clang: error: clang frontend command failed with exit code 70 (use -v to see invocation)
```

## Comments

### Comment 1 - Andarwinux

https://github.com/llvm/llvm-project/pull/180795

---

### Comment 2 - Michael-Chen-NJU

Hi @Andarwinux,

Thanks again for the testcase! I've successfully reduced it using llvm-reduce. The crash is indeed an assertion failure in LegalizeDAG.cpp triggered by an unexpected illegal type during the operation legalization phase.

```llvm
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-windows-gnu"

define <32 x i1> @svt_aom_sig_deriv_me_tf(<4 x i8> %0) {
  %2 = bitcast <4 x i8> %0 to <32 x i1>
  ret <32 x i1> %2
}
```

---

### Comment 3 - Michael-Chen-NJU

Hi @Andarwinux,

I have successfully identified the root cause. The crash happened because we created an illegal intermediate type (v128i1) during the transformation, which the backend cannot handle.

I've added a check to ensure the InterVT is legal before performing the transformation:

```
--- a/llvm/lib/CodeGen/SelectionDAG/DAGCombiner.cpp
+++ b/llvm/lib/CodeGen/SelectionDAG/DAGCombiner.cpp
@@ -20828,7 +20828,7 @@ SDValue DAGCombiner::ForwardStoreValueToDirectLoad(LoadSDNode *LD) {
 
          EVT InterVT = EVT::getVectorVT(*DAG.getContext(), EltVT,
                                         StMemSize.divideCoefficientBy(EltSize));
-        if (!TLI.isOperationLegalOrCustom(ISD::EXTRACT_SUBVECTOR, LDMemType))
+        if (!TLI.isOperationLegalOrCustom(ISD::EXTRACT_SUBVECTOR, LDMemType) || !TLI.isTypeLegal(InterVT))
            break;
```
This fix successfully prevents the crash on my end using the reduced testcase. Could you please check if this patch resolves the issue in your full environment?

---

### Comment 4 - Andarwinux

Thanks for the quick fix! I can confirm this fixed the problem.

---

### Comment 5 - Michael-Chen-NJU

> Thanks for the quick fix! I can confirm this fixed the problem.

Great! Thanks for the confirmation. I'll update the patch and submit the PR shortly, including a regression test based on the reduced case.

---

