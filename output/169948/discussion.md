# [LLVM][RISCV][ICE] LoopVectorize Assertion Failure in computeBestVF()

**Author:** whileone-sk
**URL:** https://github.com/llvm/llvm-project/issues/169948
**Status:** Closed
**Labels:** vectorizers, crash
**Closed Date:** 2025-11-29T22:05:19Z

## Body

llvm::LoopVectorizationPlanner::computeBestVF(): Assertion `(BestFactor.Width == LegacyVF.Width || BestPlan.hasEarlyExit() || !Legal->getLAI()->getSymbolicStrides().empty() || planContainsAdditionalSimplifications(getPlanFor(BestFactor.Width), CostCtx, OrigLoop, BestFactor.Width) || planContainsAdditionalSimplifications( getPlanFor(LegacyVF.Width), CostCtx, OrigLoop, LegacyVF.Width)) && " VPlan cost model and legacy cost model disagreed"' failed.

 
RUN script red-e0c444.sh to reproduce the issue.
[red-e0c444.sh](https://github.com/user-attachments/files/23827986/red-e0c444.sh)
[red-e0c444.c](https://github.com/user-attachments/files/23827993/red-e0c444.c)

Attached backtrace.
[verbose-log.txt](https://github.com/user-attachments/files/23828005/verbose-log.txt)

Reduced testcase.
[red.c](https://github.com/user-attachments/files/23828018/red.c)


Found via Fuzzer.

