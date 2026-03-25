# [RISCV] Crash on pass RISCVInsertVSETVLI

**Author:** sc-clulzze
**URL:** https://github.com/llvm/llvm-project/issues/146518
**Status:** Closed
**Labels:** backend:RISC-V, crash
**Closed Date:** 2025-07-03T15:15:41Z

## Body

`Assertion I->containsInterval(Start, End) && "Segment is not entirely in range!"' failed.`
Godbolt example - https://godbolt.org/z/GxTb5Grdd
Looking at backtace, problem is inside RISCVInsertVSETVLI pass, issue reproduced on ToT

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-risc-v

Author: None (sc-clulzze)

<details>
`Assertion I-&gt;containsInterval(Start, End) &amp;&amp; "Segment is not entirely in range!"' failed.`
Godbolt example - https://godbolt.org/z/GxTb5Grdd
Looking at backtace, problem is inside RISCVInsertVSETVLI pass, issue reproduced on ToT
</details>


---

### Comment 2 - topperc

cc: @lukel97 @BeMg 

---

