# [flang][OpenMP] fails to compile OpenMP parallel do simd with linear clause on INTEGER(8) variable

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/173332
**Status:** Closed
**Labels:** crash, mlir:openmp
**Closed Date:** 2026-01-02T11:52:34Z

## Body

Reproducer:
https://godbolt.org/z/Th44q4cK9
```f90
integer(8) :: i, j

!$omp parallel do simd linear(j)
do i = 1,100,1
end do
!$omp end parallel do simd

end
```

With assertion-enabled flang, compiling the above reproducer hits the following assertion failure:
```txt
llvm-project/llvm/lib/IR/Instructions.cpp:2706: static llvm::BinaryOperator *llvm::BinaryOperator::Create(llvm::Instruction::BinaryOps, llvm::Value *, llvm::Value *, const llvm::Twine &, llvm::InsertPosition): Assertion `S1->getType() == S2->getType() && "Cannot create binary operator with two operands of differing type!"' failed.
```

## Comments

### Comment 1 - KrxGu

I can take this issue. I reproduced the failure with the Godbolt reproducer and it appears to be a type mismatch in the `linear` step handling for `INTEGER(8)` (default step literal ends up 32-bit and later gets combined with 64-bit values during OpenMP lowering/IR building).

I’ll investigate `flang/lib/Lower/OpenMP` clause handling first, and if needed the MLIR OpenMP-to-LLVM translation / `llvm/lib/Frontend/OpenMP/OMPIRBuilder.cpp,` then post a PR with a regression test.

Could someone please assign #173332 to me?

---

