# [Flang][OpenMP] verification of lowering to FIR failed on omp atomic write with pointer assignment to derived type

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/161932
**Status:** Closed
**Labels:** flang:frontend, crash, flang:openmp
**Closed Date:** 2025-10-15T18:42:34Z

## Body

Reproducer:
https://godbolt.org/z/odGoe5Evq
```f90
type t
end type

type(t), pointer :: a1, a2

!$omp atomic write
a1 = a2
end
```

The expected behavior is that the code either compiles successfully or produces an appropriate and informative diagnostic.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-flang-ir

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/odGoe5Evq
```f90
type t
end type

type(t), pointer :: a1, a2

!$omp atomic write
a1 = a2
end
```

The expected behavior is that the code either compiles successfully or produces an appropriate and informative diagnostic.
</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-flang-frontend

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/odGoe5Evq
```f90
type t
end type

type(t), pointer :: a1, a2

!$omp atomic write
a1 = a2
end
```

The expected behavior is that the code either compiles successfully or produces an appropriate and informative diagnostic.
</details>


---

