# [PowerPC] Branch target out of range

**Author:** nikic
**URL:** https://github.com/llvm/llvm-project/issues/175190
**Status:** Closed
**Labels:** backend:PowerPC, miscompilation, release:backport
**Closed Date:** 2026-01-23T16:41:48Z

## Body

Reproducer: https://gist.githubusercontent.com/nikic/4e7863f539a198b51d5f10bb40667eaf/raw/290bf899fb353d97984a2e3ddcebe90289725339/test.ll

`llc -filetype=obj < test.ll` results in:

> `<unknown>:0: error: branch target out of range (32768 not between -32768 and 32764)`

Possibly a bug in PPCBranchSelector.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-powerpc

Author: Nikita Popov (nikic)

<details>
Reproducer: https://gist.githubusercontent.com/nikic/4e7863f539a198b51d5f10bb40667eaf/raw/290bf899fb353d97984a2e3ddcebe90289725339/test.ll

`llc -filetype=obj &lt; test.ll` results in:

&gt; `&lt;unknown&gt;:0: error: branch target out of range (32768 not between -32768 and 32764)`

Possibly a bug in PPCBranchSelector.
</details>


---

### Comment 2 - nikic

I believe the problem are instructions expanded by PPCAsmPrinter. Relevant for this test case is the expansion of PPC32GOT emits two instructions, while still being reported as having a size of 4. 

---

### Comment 3 - nikic

/cherry-pick 30fc5c1cdf9b177fabba55f9fc9df0025d54cf18 45abb3027d183dd009c8f64d50d0ff8de24d3f1e

---

### Comment 4 - llvmbot

/pull-request llvm/llvm-project#177619

---

