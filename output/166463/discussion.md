# [flang][frontend] Internal compiler error caused by program fragment

**Author:** akuhlens
**URL:** https://github.com/llvm/llvm-project/issues/166463
**Status:** Closed
**Labels:** flang:frontend, crash
**Closed Date:** 2025-12-11T17:16:10Z

## Body

The following code causes the following assertion violation.
```fortran
program main
type t
  sequence
  integer :: i = 1       
end type
type (t) :: A
integer :: x
equivalence (A, x)
end

subroutine s()
  type g
    integer::d/1/
  end type
end subroutine
```

```
fatal internal error: CHECK(offset_ + elements * stride <= image_.data_.size() || padWithZero_) failed at /proj/build/llvm/Linux_x86_64/flang/lib/Evaluate/initial-image.cpp(104)
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-flang-frontend

Author: Andre Kuhlenschmidt (akuhlens)

<details>
The following code causes the following assertion violation.
```fortran
program main
type t
  sequence
  integer :: i = 1       
end type
type (t) :: A
integer :: x
equivalence (A, x)
end

subroutine s()
  type g
    integer::d/1/
  end type
end subroutine
```

```
fatal internal error: CHECK(offset_ + elements * stride &lt;= image_.data_.size() || padWithZero_) failed at /proj/build/llvm/Linux_x86_64/flang/lib/Evaluate/initial-image.cpp(104)
```
</details>


---

