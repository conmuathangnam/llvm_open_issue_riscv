# [flang][OpenMP] Custom mapper not found and crashing compilation when used in different module than which it was defined in

**Author:** jfuchs-kmt
**URL:** https://github.com/llvm/llvm-project/issues/163385
**Status:** Closed
**Labels:** flang:frontend, flang:ir, crash, flang:openmp
**Closed Date:** 2025-11-13T15:07:47Z

## Body

When using a custom mapper outside of the module it was defined, it is not found and crashes the compilation. Below is a short reproducer for the issue:
```fortran
MODULE field_mod
    IMPLICIT NONE

    TYPE :: field_t
        REAL, ALLOCATABLE :: buf(:)
    END TYPE field_t
    !$omp declare mapper(custom: field_t :: t) map(t%buf)
END MODULE field_mod

PROGRAM reproducer
    USE field_mod

    IMPLICIT NONE

    REAL :: validation(5)
    TYPE(field_t) :: f_t
    ALLOCATE(f_t%buf(5))
    f_t%buf = 42.0

    !$omp target map(mapper(custom), tofrom: f_t) map(from: validation)
        validation(:) = f_t%buf(:)
    !$omp end target

    PRINT*, "Expected: 42. 42. 42. 42. 42."
    PRINT*, "Actual:", validation

    DEALLOCATE(f_t%buf)
END PROGRAM reproducer
```

Compiling this code (with cuda in my case) crashes...
```
flang -fopenmp -fopenmp-targets=nvptx64-nvidia-cuda -fopenmp-version=52 main.F90 -O3
```
...with the error message:
```
error: loc("f_t"("/home/jfuchs/reproducer/main.F90":18:22)): invalid mapper id
error: verification of lowering to FIR failed
```

We can cross check the correct behavior by moving the `!$omp declare mapper...` line below into the PROGRAM directly. Then compilation succeeds and the values are as expected after the target region.

I am using `flang version 22.0.0git (git@github.com:llvm/llvm-project.git a8057ff12956b7dd6dec44f9610c446c828c4af8)`.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-flang-ir

Author: None (jfuchs-kmt)

<details>
When using a custom mapper outside of the module it was defined, it is not found and crashes the compilation. Below is a short reproducer for the issue:
```fortran
MODULE field_mod
    IMPLICIT NONE

    TYPE :: field_t
        REAL, ALLOCATABLE :: buf(:)
    END TYPE field_t
    !$omp declare mapper(custom: field_t :: t) map(t%buf)
END MODULE field_mod

PROGRAM reproducer
    USE field_mod

    IMPLICIT NONE

    REAL :: validation(5)
    TYPE(field_t) :: f_t
    ALLOCATE(f_t%buf(5))
    f_t%buf = 42.0

    !$omp target map(mapper(custom), tofrom: f_t) map(from: validation)
        validation(:) = f_t%buf(:)
    !$omp end target

    PRINT*, "Expected: 42. 42. 42. 42. 42."
    PRINT*, "Actual:", validation

    DEALLOCATE(f_t%buf)
END PROGRAM reproducer
```

Compiling this code (with cuda in my case) crashes...
```
flang -fopenmp -fopenmp-targets=nvptx64-nvidia-cuda -fopenmp-version=52 main.F90 -O3
```
...with the error message:
```
error: loc("f_t"("/home/jfuchs/reproducer/main.F90":18:22)): invalid mapper id
error: verification of lowering to FIR failed
```

We can cross check the correct behavior by moving the `!$omp declare mapper...` line below into the PROGRAM directly. Then compilation succeeds and the values are as expected after the target region.

I am using `flang version 22.0.0git (git@<!-- -->github.com:llvm/llvm-project.git a8057ff12956b7dd6dec44f9610c446c828c4af8)`.
</details>


---

### Comment 2 - kiranchandramohan

FYI @TIFitis @agozillon. Have we added `declare mapper` to the module file? CC: @kparzysz 

---

### Comment 3 - TIFitis

I'll look into this and report back.

---

### Comment 4 - kparzysz

I think we only put reductions in the module files at the moment.

---

### Comment 5 - kiranchandramohan

> I think we only put reductions in the module files at the moment.

We do threadprivate. We probably need to create a list of all declarative directives in a meta ticket and get it all done from a module file perspective.

---

### Comment 6 - agozillon

> > I think we only put reductions in the module files at the moment.
> 
> We do threadprivate. We probably need to create a list of all declarative directives in a meta ticket and get it all done from a module file perspective.

Would be quite useful, I imagine something like declare target also might need to be exported to the module files to act coherently in all cases. 


---

### Comment 7 - llvmbot


@llvm/issue-subscribers-flang-frontend

Author: None (jfuchs-kmt)

<details>
When using a custom mapper outside of the module it was defined, it is not found and crashes the compilation. Below is a short reproducer for the issue:
```fortran
MODULE field_mod
    IMPLICIT NONE

    TYPE :: field_t
        REAL, ALLOCATABLE :: buf(:)
    END TYPE field_t
    !$omp declare mapper(custom: field_t :: t) map(t%buf)
END MODULE field_mod

PROGRAM reproducer
    USE field_mod

    IMPLICIT NONE

    REAL :: validation(5)
    TYPE(field_t) :: f_t
    ALLOCATE(f_t%buf(5))
    f_t%buf = 42.0

    !$omp target map(mapper(custom), tofrom: f_t) map(from: validation)
        validation(:) = f_t%buf(:)
    !$omp end target

    PRINT*, "Expected: 42. 42. 42. 42. 42."
    PRINT*, "Actual:", validation

    DEALLOCATE(f_t%buf)
END PROGRAM reproducer
```

Compiling this code (with cuda in my case) crashes...
```
flang -fopenmp -fopenmp-targets=nvptx64-nvidia-cuda -fopenmp-version=52 main.F90 -O3
```
...with the error message:
```
error: loc("f_t"("/home/jfuchs/reproducer/main.F90":18:22)): invalid mapper id
error: verification of lowering to FIR failed
```

We can cross check the correct behavior by moving the `!$omp declare mapper...` line below into the PROGRAM directly. Then compilation succeeds and the values are as expected after the target region.

I am using `flang version 22.0.0git (git@<!-- -->github.com:llvm/llvm-project.git a8057ff12956b7dd6dec44f9610c446c828c4af8)`.
</details>


---

### Comment 8 - jfuchs-kmt

@TIFitis Hi, thank you for your effort to work on the mapper issues! I suspect there might be an issue with the fix from  https://github.com/llvm/llvm-project/pull/167903. While the reproducer we sent now works perfectly, we have found nasty crashes when trying to compile a more sophisticated example. 
If possible I would kindly like to ask you to have a look at this testcase of just a few files we have setup: https://github.com/kmturbulenz/mglet-openmp-compiler-tests/tree/master/mglet-mockup. You can try to compile this with the latest LLVM from main using `FFLAGS=-fopenmp-targets=nvptx64-nvidia-cuda make llvm` (if you are using NVIDIA, replace the target with the AMD target if you use AMD) and no matter how you try to arrange the mappers, you will run into into some nasty internal parsing error with a larger stacktrace.
I checked the commit in LLVM just before merging the fix for this issue and there is no crash before and both checks in the testcase pass.

---

### Comment 9 - jfuchs-kmt

@TIFitis I managed to narrow down the crash into a more manageable reproducer. I opened a new issue for this purpose https://github.com/llvm/llvm-project/issues/168336.

---

