# RISC-V EVL tail folding failure on SPEC CPU 2017 502.gcc_r

**Author:** lukel97
**URL:** https://github.com/llvm/llvm-project/issues/122682
**Status:** Closed
**Labels:** miscompilation, vectorizers
**Closed Date:** 2025-01-14T14:17:53Z

## Body

Split out from the discussion here: https://github.com/llvm/llvm-project/pull/122458#issuecomment-2585713670 

On RISC-V with `-march=rva22u64_v -O3 -flto -mllvm -force-tail-folding-style=data-with-evl -mllvm -prefer-predicate-over-epilogue=predicate-else-scalar-epilogue`, the SPEC CPU 2017 502.gcc_r benchmark fails in the train dataset, likely due to a miscompile.

It's been failing since at least 6ad0dcf67f5dccdf8506ce5f51d793062a1c6879, detected from this LNT run: https://lnt.lukelau.me/db_default/v4/nts/89

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-risc-v

Author: Luke Lau (lukel97)

<details>
Split out from the discussion here: https://github.com/llvm/llvm-project/pull/122458#issuecomment-2585713670 

On RISC-V with `-march=rva22u64_v -O3 -flto -mllvm -force-tail-folding-style=data-with-evl -mllvm -prefer-predicate-over-epilogue=predicate-else-scalar-epilogue`, the SPEC CPU 2017 502.gcc_r benchmark fails in the train dataset, likely due to a miscompile.

It's been failing since at least 6ad0dcf67f5dccdf8506ce5f51d793062a1c6879, detected from this LNT run: https://lnt.lukelau.me/db_default/v4/nts/89
</details>


---

### Comment 2 - fhahn

cc @Mel-Chen @LiqinWeng @alexey-bataev @ElvisWang123 

---

