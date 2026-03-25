# [RISCV] Incorrect cost for fptosi_sat.v8i16.v8f64 with Zve32f

**Author:** topperc
**URL:** https://github.com/llvm/llvm-project/issues/142973
**Status:** Closed
**Labels:** llvm:codegen
**Closed Date:** 2025-07-10T06:44:29Z

## Body

v8i16 is legal with Zve32f, but v8f64 needs to be scalarized. The cost model only checks the destination type not the input type so does not report the scalarization cost.

CC @arsenm @lukel97 @wangpc-pp @preames 

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-risc-v

Author: Craig Topper (topperc)

<details>
v8i16 is legal with Zve32f, but v8f64 needs to be scalarized. The cost model only checks the destination type not the input type so does not report the scalarization cost.

CC @<!-- -->arsenm @<!-- -->lukel97 @<!-- -->wangpc-pp @<!-- -->preames 
</details>


---

