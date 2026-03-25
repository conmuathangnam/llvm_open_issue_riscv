# aarch64 specific misscompilation with TBAA

**Author:** tblah
**URL:** https://github.com/llvm/llvm-project/issues/141928
**Status:** Closed
**Labels:** wontfix, backend:AArch64, miscompilation, TBAA
**Closed Date:** 2025-06-10T14:09:44Z

## Body

This issue relates to https://github.com/llvm/llvm-project/pull/139682, which had to be reverted due to a misscompilation which was only reproducible on aarch64. That patch causes flang to emit more specific TBAA information about local variables. The pull request passed a large number of other tests on aarch64.

The misscompilation is for https://github.com/fujitsu/compiler-test-suite/blob/main/Fortran/0614/0614_0005.f. Without the patch, that program outputs
```
*** ok ***
```

With the patch (and `-O2` or above) the program emits
```
*** ng ***
3 5 7 9 11 13 15 17 19 10 13 14 15 16 17 18 19 20 11 11
```

It can be built using `flang 0614_0005.f -O3`. Alternately, here is the generated LLVMIR:
[llvm-ir.txt](https://github.com/user-attachments/files/20503379/llvm-ir.txt) (in .txt because github doesn't allow .ll).

`flang bug.ll -O3 -o /tmp/out` (flang has to be used so that the correct runtime libraries are linked).

This is reproducible with [the original patch](https://github.com/llvm/llvm-project/commit/cf16c97bfa1416672d8990862369e86f360aa11e) and with that patch cherry-picked onto current main (6602bfa721889).

I don't see any change to the LLVM IR produced by `opt -O3` with and without this patch (other than the changes to the tbaa tags) and this does not reproduce on X86, and so I think this could be an aarch64-specific misscompilation rather than a flang bug.

CC: @DominikAdamski 

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-aarch64

Author: Tom Eccles (tblah)

<details>
This issue relates to https://github.com/llvm/llvm-project/pull/139682, which had to be reverted due to a misscompilation which was only reproducible on aarch64. That patch causes flang to emit more specific TBAA information about local variables. The pull request passed a large number of other tests on aarch64.

The misscompilation is for https://github.com/fujitsu/compiler-test-suite/blob/main/Fortran/0614/0614_0005.f. Without the patch, that program outputs
```
*** ok ***
```

With the patch (and `-O2` or above) the program emits
```
*** ng ***
3 5 7 9 11 13 15 17 19 10 13 14 15 16 17 18 19 20 11 11
```

It can be built using `flang 0614_0005.f -O3`. Alternately, here is the generated LLVMIR:
[llvm-ir.txt](https://github.com/user-attachments/files/20503379/llvm-ir.txt) (in .txt because github doesn't allow .ll).

`flang bug.ll -O3 -o /tmp/out` (flang has to be used so that the correct runtime libraries are linked).

This is reproducible with [the original patch](https://github.com/llvm/llvm-project/commit/cf16c97bfa1416672d8990862369e86f360aa11e) and with that patch cherry-picked onto current main (6602bfa721889).

I don't see any change to the LLVM IR produced by `opt -O3` with and without this patch (other than the changes to the tbaa tags) and this does not reproduce on X86, and so I think this could be an aarch64-specific misscompilation rather than a flang bug.

CC: @<!-- -->DominikAdamski 
</details>


---

### Comment 2 - efriedma-quic

You can use -print-after-all to see exactly what pass starts behaving differently.

It's possible you're running into an aarch64 backend bug, but more likely there's something wrong with your TBAA info, which allows your code to get rescheduled in an illegal way.

---

### Comment 3 - davemgreen

In sub2_ there are 2 inputs, %0 and %1
These are the important bits of sub2_:
```
%2 = alloca { ptr, i64, i32, i8, i8, i8, i8 }, align 8
%3 = ptrtoint ptr %0 to i64
%50 = add i64 %3, 32           
%51 = inttoptr i64 %50 to ptr
call void @_FortranAPointerAssociateScalar(ptr nonnull %2, ptr %51)
%.sroa.024.0.copyload.8 = load ptr, ptr %2, align 8, !tbaa !50     
store i32 %55, ptr %.sroa.024.0.copyload.8, align 4, !tbaa !54     

%47 = getelementptr i8, ptr %0, i64 32
%64 = load <2 x i32>, ptr %47, align 4, !tbaa !57
```
So `%51 = %0+32`, and if the load from `%2` after `_FortranAPointerAssociateScalar` just reloads `%51` (?) then `%.sroa.024.0.copyload.8 == %0+32 == %47`
So the store of `%55` to `%.sroa.024.0.copyload.8` needs to happen before the load of `%47`.

But TBAA says they don't alias
`  NoAlias:   %64 = load <2 x i32>, ptr %47, align 4, !tbaa !11 <->   store i32 %55, ptr %.sroa.024.0.copyload.8, align 4, !tbaa !8`
from https://godbolt.org/z/o741xMbqM (the tbaa attributes get renumbered as I removed a lot of the code).

(AArch64 is different to X86 due to use-aa, which helps a number of things, especially allowing better inorder scheduling. https://godbolt.org/z/bqMK658ad)

The original code has overlapping pointers, if I am reading the fortran correctly, and `%.sroa.024.0.copyload.8` is probably `p1` so the store of `pa` is moving past the load of `a(1:10)`? I'm not sure exactly how the fortran pointer stuff works.
```
      subroutine sub2(a,b)
      integer a(10),b(10),pa,pb
      pointer (p1 ,pa)
      pointer (p2 ,pb)

      p1 = loc(a(1))
      p2 = loc(b(1))

      do i = 1,9
         pa = pa + a(i+1)
         p1 = p1 + 4
      end do

      b(1:10) = a(1:10) + b(1:10)

      return
      end
```

---

### Comment 4 - tblah

Thanks for looking into this Dave. You're right, flang's alias analysis isn't handling cray pointers correctly.

---

### Comment 5 - vzakhari

I think this is a test error, and other compilers may misoptimize it as well.

GCC, https://gcc.gnu.org/onlinedocs/gfortran/Cray-pointers.html:
```
As long as ipt remains unchanged, iarr is now an alias for target. The optimizer, however, does not detect this aliasing, so it is unsafe to use iarr and target simultaneously. Using a pointee in any way that violates the Fortran aliasing rules or assumptions is invalid. It is the user’s responsibility to avoid doing this; the compiler works under the assumption that no such aliasing occurs.

Cray pointers work correctly when there is no aliasing (i.e., when they are used to access a dynamically allocated block of memory), and also in any routine where a pointee is used, but any variable with which it shares storage is not used. Code that violates these rules may not run as the user intends. This is not a bug in the optimizer; any code that violates the aliasing rules is invalid. (Note that this is not unique to GNU Fortran; any Fortran compiler that supports Cray pointers “incorrectly” optimizes code with invalid aliasing.)
```

HPE, https://support.hpe.com/hpesc/public/docDisplay?docId=a00113911en_us&docLocale=en_US&page=Types.html:
```
For purposes of optimization, the compiler assumes that the storage of a pointee is never overlaid on the storage of another variable; that is, it assumes that a pointee is not associated with another variable or array. This kind of association occurs when a Cray pointer has two pointees, or when two Cray pointers are given the same value. Although these practices are sometimes used deliberately (such as for equivalencing arrays), results can differ depending on whether optimization is turned on or off. The code developer is responsible for preventing such association.
```

Flang, https://flang.llvm.org/docs/Aliasing.html#cray-pointers:
```
Optimizations assume that Cray pointers do not alias any other variables. In the above example, it is assumed that handle and target do not alias, and optimizations will treat them as separate entities.

In order to disable optimizations that assume that there is no aliasing between Cray pointer targets and entities they alias with, add the TARGET attribute to variables aliasing with a Cray pointer (the target variable in this example).
```

In this example, `pa` and `a` effectively alias, whereas Flang is allowed to assume they are not aliasing.  I suggest adding TARGET attribute to `a` - does it resolve the issue?

---

### Comment 6 - DominikAdamski

> In this example, pa and a effectively alias, whereas Flang is allowed to assume they are not aliasing. I suggest adding TARGET attribute to a - does it resolve the issue?

Yes, adding TARGET attribute resolves the issue.

---

### Comment 7 - tblah

Closing because we don't plan to change alias analysis to accommodate this test.

---

