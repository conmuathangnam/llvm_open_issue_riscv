# Assertion `isScalableVector() == VT.isScalableVector() && "Comparison between scalable and fixed types"' failed.

**Author:** gawbl
**URL:** https://github.com/llvm/llvm-project/issues/180057
**Status:** Closed
**Labels:** duplicate, crash, llvm:SelectionDAG
**Closed Date:** 2026-02-06T09:24:10Z

## Body

Assertion while building PyTorch on an ARM/linux machine.

[BinaryOpsKernel-755e76.cpp](https://github.com/user-attachments/files/25113856/BinaryOpsKernel-755e76.cpp)
[BinaryOpsKernel-755e76.sh](https://github.com/user-attachments/files/25113855/BinaryOpsKernel-755e76.sh)

[backtrace.txt](https://github.com/user-attachments/files/25113882/backtrace.txt)

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-aarch64

Author: None (gawbl)

<details>
Assertion while building PyTorch on an ARM/linux machine.

[BinaryOpsKernel-755e76.cpp](https://github.com/user-attachments/files/25113856/BinaryOpsKernel-755e76.cpp)
[BinaryOpsKernel-755e76.sh](https://github.com/user-attachments/files/25113855/BinaryOpsKernel-755e76.sh)

[backtrace.txt](https://github.com/user-attachments/files/25113882/backtrace.txt)
</details>


---

### Comment 2 - abhishek-kaushik22

Not reproducible anymore, as this was fixed in https://github.com/llvm/llvm-project/commit/2fa1ba62ac92fe95613d980e38ac2db36141fee8

---

