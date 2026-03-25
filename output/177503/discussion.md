# [Flang] Default initialization for pointers causes an internal compilation error

**Author:** boo-aboutme
**URL:** https://github.com/llvm/llvm-project/issues/177503
**Status:** Open
**Labels:** flang:ir, crash

## Body

``` text
Version of flang : 22.0.0git (https://github.com/llvm/llvm-project.git 2f7e218017db69454ea84e1849fcb9a11b1f7fef)
```

Flang causes an internal error while compiling the sample program shown below.

test.f90
``` fortran
call sub01()
print *,'pass'
end
subroutine sub01()
type ty1
 integer :: arr(10)
end type
type(ty1),target,save :: ss1(10,20,30)
type ty2
  type(ty1) :: ss1(10,20,30)
end type
type(ty2) ,target,save :: ss2(10,20,30)

integer ,pointer :: ps2a(:,:,:) =>ss2(1:10:2,5:10,16:8)%ss1(10,10,10)%arr(1)
end
```

No compiler options are needed.

``` text
$ flang test.f90
loc("/work/home/boo/SecondaryResearch/5780/test.f90":14:21): error: ops with side effects not allowed in global initializers
error: Lowering to LLVM IR failed
loc("/work/home/boo/SecondaryResearch/5780/test.f90":14:21): error: LLVM Translation failed for operation: builtin.unrealized_conversion_cast
loc("/work/home/boo/SecondaryResearch/5780/test.f90":14:21): error: fail to convert global initializer
error: failed to create the LLVM module
```

Other compilers can compile the program with no issues:

``` text
$ gfortran test.f90
$ ./a.out
 pass
$
```

Line 14 is an initialization using default initialization of a pointer variable defined in Fortran 2023 as follows:

**R805** *initialization* **is** = *constant-expr*  
**or** => *null-init*  
**or** => *initial-data-target*  

**C812** (R803) If *initial-data-target* appears, *object-name* shall be data-pointer-initialization compatible with it (7.5.4.6).

7.5.4.6 Default initialization for components  
Default initialization provides a means of automatically initializing
pointer components to be disassociated or associated with specific
targets, and nonpointer nonallocatable components to have a particular
value. Allocatable components are always initialized to unallocated.  

A pointer variable or component is data-pointer-initialization
compatible with a target if the pointer is type compatible with the
target, they have the same rank, all nondeferred type parameters of
the pointer have the same values as the corresponding type parameters
of the target, and the target is contiguous if the pointer has the
CONTIGUOUS attribute.

**R743** *component-initialization* **is** = *constant-expr*  
**or** => *null-init*  
**or** => *initial-data-target*  

**R744** *initial-data-target* **is** *designator*

...

**C770** A designator that is an initial-data-target shall designate a
nonallocatable, noncoindexed variable that has the TARGET and SAVE
attributes and does not have a vector subscript. Every subscript,
section subscript, substring starting point, and substring ending
point in designator shall be a constant expression.


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-flang-ir

Author: Manabu Matsuyama (boo-aboutme)

<details>
``` text
Version of flang : 22.0.0git (https://github.com/llvm/llvm-project.git 2f7e218017db69454ea84e1849fcb9a11b1f7fef)
```

Flang causes an internal error while compiling the sample program shown below.

test.f90
``` fortran
call sub01()
print *,'pass'
end
subroutine sub01()
type ty1
 integer :: arr(10)
end type
type(ty1),target,save :: ss1(10,20,30)
type ty2
  type(ty1) :: ss1(10,20,30)
end type
type(ty2) ,target,save :: ss2(10,20,30)

integer ,pointer :: ps2a(:,:,:) =&gt;ss2(1:10:2,5:10,16:8)%ss1(10,10,10)%arr(1)
end
```

No compiler options are needed.

``` text
$ flang test.f90
loc("/work/home/boo/SecondaryResearch/5780/test.f90":14:21): error: ops with side effects not allowed in global initializers
error: Lowering to LLVM IR failed
loc("/work/home/boo/SecondaryResearch/5780/test.f90":14:21): error: LLVM Translation failed for operation: builtin.unrealized_conversion_cast
loc("/work/home/boo/SecondaryResearch/5780/test.f90":14:21): error: fail to convert global initializer
error: failed to create the LLVM module
```

Other compilers can compile the program with no issues:

``` text
$ gfortran test.f90
$ ./a.out
 pass
$
```

Line 14 is an initialization using default initialization of a pointer variable defined in Fortran 2023 as follows:

**R805** *initialization* **is** = *constant-expr*  
**or** =&gt; *null-init*  
**or** =&gt; *initial-data-target*  

**C812** (R803) If *initial-data-target* appears, *object-name* shall be data-pointer-initialization compatible with it (7.5.4.6).

7.5.4.6 Default initialization for components  
Default initialization provides a means of automatically initializing
pointer components to be disassociated or associated with specific
targets, and nonpointer nonallocatable components to have a particular
value. Allocatable components are always initialized to unallocated.  

A pointer variable or component is data-pointer-initialization
compatible with a target if the pointer is type compatible with the
target, they have the same rank, all nondeferred type parameters of
the pointer have the same values as the corresponding type parameters
of the target, and the target is contiguous if the pointer has the
CONTIGUOUS attribute.

**R743** *component-initialization* **is** = *constant-expr*  
**or** =&gt; *null-init*  
**or** =&gt; *initial-data-target*  

**R744** *initial-data-target* **is** *designator*

...

**C770** A designator that is an initial-data-target shall designate a
nonallocatable, noncoindexed variable that has the TARGET and SAVE
attributes and does not have a vector subscript. Every subscript,
section subscript, substring starting point, and substring ending
point in designator shall be a constant expression.

</details>


---

### Comment 2 - eugeneepshteyn

Just a note that in the original reproducer `ps2a` is supposed to point to an empty array section due to `16:8`.  On the other hand, even if I change `16:8` to `6:8` to make the array section non-empty, the error is the same.

---

