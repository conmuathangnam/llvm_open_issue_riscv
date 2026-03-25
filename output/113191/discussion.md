# [flang][OpenMP] runtime segfault when an allocatable variable is used with copyin

**Author:** tblah
**URL:** https://github.com/llvm/llvm-project/issues/113191
**Status:** Closed
**Labels:** miscompilation, flang:ir, flang:openmp
**Closed Date:** 2025-01-23T05:44:02Z

## Body

```
program foo
  real, allocatable :: ra
  !$omp threadprivate(ra)
  allocate(ra)
  ra = 10
  !$omp parallel num_threads(2) copyin(ra)
  print *,ra
  !$omp end parallel
end program
```

Compiling with `flang -fopenmp` version `8e6abf52e7` gives a segfault in the compiled aplication after the print.

CC @DavidTruby 

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-flang-ir

Author: Tom Eccles (tblah)

<details>
```
program foo
  real, allocatable :: ra
  !$omp threadprivate(ra)
  allocate(ra)
  ra = 10
  !$omp parallel num_threads(2) copyin(ra)
  print *,ra
  !$omp end parallel
end program
```

Compiling with `flang -fopenmp` version `8e6abf52e7` gives a segfault in the compiled aplication after the print.

CC @<!-- -->DavidTruby 
</details>


---

