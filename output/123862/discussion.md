# Generation of Object file gets stuck in infinite loop after enabling RISCVVLOptimizer

**Author:** MaheshRavishankar
**URL:** https://github.com/llvm/llvm-project/issues/123862
**Status:** Closed
**Labels:** backend:RISC-V, hang
**Closed Date:** 2025-01-22T18:49:56Z

## Body

I hit an issue in downstream project (IREE), where the PR to enable the RISCVVLOptimizer gets stuck in an infinite loop (i.e. after https://github.com/llvm/llvm-project/pull/119461). Reverting that seems to resolve the issue.

[module_three_fry_linked_embedded_elf_riscv_64.optimized.ll.txt](https://github.com/user-attachments/files/18498078/module_three_fry_linked_embedded_elf_riscv_64.optimized.ll.txt)

This is the output after what is effectively `opt` phase. The downstream project uses the c++ interface to emit object files, so I cant really reproduce the command line options for the hang, But there is not much secret sauce here on how it is generating the object file (https://github.com/iree-org/iree/blob/8c7eecaadda7931724aae3ac371fb60c1fb01873/compiler/plugins/target/LLVMCPU/LLVMIRPasses.cpp#L98) . If someone can give me tips on how I can try to get the equivalent command line for `llc` that mimics what is done there, I am happy to try.

@michaelmaitland  could you please provide guidance here. We have been reverting this patch downstream for a while, and it is getting hard for us to carry the revert.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-risc-v

Author: None (MaheshRavishankar)

<details>
I hit an issue in downstream project (IREE), where the PR to enable the RISCVVLOptimizer gets stuck in an infinite loop (i.e. after https://github.com/llvm/llvm-project/pull/119461). Reverting that seems to resolve the issue.

[module_three_fry_linked_embedded_elf_riscv_64.optimized.ll.txt](https://github.com/user-attachments/files/18498078/module_three_fry_linked_embedded_elf_riscv_64.optimized.ll.txt)

This is the output after what is effectively `opt` phase. The downstream project uses the c++ interface to emit object files, so I cant really reproduce the command line options for the hang, But there is not much secret sauce here on how it is generating the object file (https://github.com/iree-org/iree/blob/8c7eecaadda7931724aae3ac371fb60c1fb01873/compiler/plugins/target/LLVMCPU/LLVMIRPasses.cpp#L98) . If someone can give me tips on how I can try to get the equivalent command line for `llc` that mimics what is done there, I am happy to try.

@<!-- -->michaelmaitland  could you please provide guidance here. We have been reverting this patch downstream for a while, and it is getting hard for us to carry the revert.
</details>


---

### Comment 2 - michaelmaitland

#123878 should fix this.

---

