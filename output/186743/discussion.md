# [Flang][OpenMP] Crash with user-defined reduction on allocatable variables

**Author:** Ritanya-B-Bharadwaj
**URL:** https://github.com/llvm/llvm-project/issues/186743
**Status:** Open
**Labels:** crash, flang:openmp

## Body

Flang crashes when a user-defined reduction (declare reduction) is used with allocatable variables in a reduction clause. This includes allocatable scalars, 1D arrays, and 2D arrays.

The existing declare_reduction lowering creates the reduction op for the element type (e.g. i32, !fir.char<1>), but when the reduction variable is allocatable, the reduction needs to operate on the boxed type (!fir.box<!fir.heap<T>>). Flang doesn't handle this and produces invalid IR.

Testcase

```fortran
program test
  integer, allocatable :: a
  integer, allocatable :: b(:)
  integer, allocatable :: c(:,:)
  integer :: i

!$omp declare reduction (foo : integer : omp_out = omp_out + omp_in) &
!$omp& initializer (omp_priv = 0)

  allocate(a, b(6:9), c(3, 8:9))

  a = 0
  b = 0
  c = 0

!$omp parallel do reduction(foo : a, b, c)
  do i = 1, 10
     a = a + i
     b = b + 2 * i
     c = c + 3 * i
  end do

  if (a /= 55 .or. any(b /= 110) .or. any(c /= 165)) stop 1

end program
```

```
flang -fopenmp -fopenmp-version=45 test.f90
warning: OpenMP support for version 45 in flang is still incomplete
Intrinsic has incorrect argument type!
ptr @llvm.memcpy.p0.i32.i32
Call parameter type does not match function signature!
i32 0
 ptr  call void @_FortranAAssign(i32 0, ptr %9, ptr @_QQclX2093389c4a1268a17ade80165c6521fc, i32 18)
Intrinsic has incorrect argument type!
ptr @llvm.memcpy.p0.i32.i32
Call parameter type does not match function signature!
i32 0
 ptr  call void @_FortranAAssign(i32 0, ptr %7, ptr @_QQclX2093389c4a1268a17ade80165c6521fc, i32 19)
Intrinsic has incorrect argument type!
ptr @llvm.memcpy.p0.i32.i32
Call parameter type does not match function signature!
i32 0
 ptr  call void @_FortranAAssign(i32 0, ptr %5, ptr @_QQclX2093389c4a1268a17ade80165c6521fc, i32 20)
error: failed to create the LLVM module
```

Testcase

```fortran
program test_char
  implicit none

  character(len=1), allocatable :: k1

!$omp declare reduction (char_max : character(len=1) : &
!$omp& omp_out = max(omp_out, omp_in)) &
!$omp& initializer (omp_priv = 'a')

  allocate(k1)
  k1 = 'a'

!$omp parallel sections reduction(char_max : k1)
  k1 = max(k1, 'z')
!$omp end parallel sections

  if (k1 /= 'z') stop 1

end program
```

```
flang -fopenmp -fopenmp-version=45 test_char.f90 
warning: OpenMP support for version 45 in flang is still incomplete
error: loc("/test_char.f90":1:1): /workspace/rpmbuild/BUILD/llvm-project-23.0.0~git^568380.769b734c0204/flang/lib/Lower/Support/PrivateReductionUtils.cpp:676: not yet implemented: creating reduction/privatization init region for unsupported type
LLVM ERROR: aborting
```
gfortran handles both cases correctly.

