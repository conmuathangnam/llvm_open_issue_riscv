# [Flang][OpenMP] verification of lowering to FIR failed on omp atomic capture with derived type array element

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/161934
**Status:** Open
**Labels:** flang:ir, crash, flang:openmp

## Body

Reproducer:
https://godbolt.org/z/sWPvEbWqW
```f90
type t1
  integer :: i(1)
end type

type(t1) :: t2
t2%i=0
j=1

!$omp atomic capture
t2%i(j*1) = t2%i(1) + 1
t2%i(1) = t2%i(j*1)
!$omp end atomic
end
```

The expected behavior is that the code either compiles successfully or produces an appropriate and informative diagnostic.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-flang-ir

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/sWPvEbWqW
```f90
type t1
  integer :: i(1)
end type

type(t1) :: t2
t2%i=0
j=1

!$omp atomic capture
t2%i(j*1) = t2%i(1) + 1
t2%i(1) = t2%i(j*1)
!$omp end atomic
end
```

The expected behavior is that the code either compiles successfully or produces an appropriate and informative diagnostic.
</details>


---

### Comment 2 - KrxGu

Hi, I’d like to investigate and send a fix here.
Plan:
• Reproduce and inspect the FIR/OMP IR for the reproducer.
• In Flang’s OpenMP lowering, adjust `atomic capture` for derived-type array elements so the capture region uses a legal op pair (`update `then `read`) and compute the element address once (component + subscript).
• Add a regression test under flang/test/Lower/OpenMP that checks we emit `omp.atomic.capture` with `omp.atomic.update` followed by `omp.atomic.read` for the example.
If anyone has work in progress on this already, happy to coordinate. Otherwise I’ll proceed and open a PR with the test + fix. 

Thanks!

---

### Comment 3 - KrxGu

## Investigation Update

I spent time investigating this issue and attempted to implement a semantic check to catch this case early (before lowering), but encountered significant complexity with Flang's expression representation system.

### Approaches Attempted

**Approach 1: Structural Designator Comparison**
- Implemented recursive helpers (`sameBaseAndComponents`, `sameSubscriptsStructurally`)
- Successfully compiled but hit a wall: `evaluate::ExtractDataRef()` only works on pure designators, not operations containing designators
- Cannot extract `t2%i(1)` from `t2%i(1) + 1` without full expression tree traversal

**Approach 2: Expression Tree Traversal with `evaluate::Traverse`**
- Attempted visitor pattern to walk expression tree and find all DataRef nodes
- Failed due to incomplete handling of the 50+ variant types in `Expr<SomeType>`
- Requires deep knowledge of Flang's template metaprogramming and type hierarchy

**Approach 3: Manual Binary Operation Handling**
- Tried to directly access operation operands assuming `.left()` and `.right()` methods
- Failed - operations don't expose member functions as expected
- Additional type system misunderstandings (e.g., `evaluate::Integer<4>` vs `Type<TypeCategory::Integer, 4>`)

**Approach 4: String-Based Heuristic**
- Converted expressions to strings for textual comparison
- Compiled but had logic bugs causing false positives/negatives
- Did not trigger on any test cases

### Current State

The issue is still caught by MLIR verification at runtime, so it doesn't cause silent failures. However, a proper semantic check would provide better error messages earlier in the compilation pipeline.

A complete solution requires:
- Deep expertise in Flang's `evaluate` namespace
- Understanding of the deeply nested variant type system
- Proper expression tree walker handling all expression types
- Estimated 150-200 LOC

I'm deferring this to someone with more Flang internals expertise. The infrastructure (placeholder lambda, includes, integration points) is in place in the semantic checker for future implementation.

### Related PR
- PR #162884 (updated to remove ineffective code and document attempts)

---

### Comment 4 - kparzysz

Here's a smaller reproducer
```
subroutine f00(x, y, z)
  integer :: x, y, z
  !$omp atomic capture
    x = y
    z = x
  !$omp end atomic
end
```

This fails MLIR verification directly, since it doesn't accept write followed by a read.  I'd have to double check if we allow this in the OpenMP spec.

---

