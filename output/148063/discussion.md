# [HLSL] `inout` on arrays is not correctly handled

**Author:** Icohedron
**URL:** https://github.com/llvm/llvm-project/issues/148063
**Status:** Closed
**Labels:** clang:codegen, miscompilation, HLSL
**Closed Date:** 2025-07-24T20:54:01Z

## Body

After fixing #147114 there are now 14 DML shaders failing to pass validation due to error `Instructions should not read uninitialized value`. 
While this validation error was encountered and fixed before (#139011), this appears to another unique case.

Reproduction: https://godbolt.org/z/chcadYss3

The following shader defines and uses a function `FillZero` with an inout parameter to fill an array with zeros.
```hlsl
// compile args: -T lib_6_7 -Xclang -emit-llvm
export void FillZero(inout int v[2]) {
  v = {0,0};
}
```

Clang emits FillZero as a no-op function.
```llvm
; Function Attrs: alwaysinline mustprogress nofree norecurse nosync nounwind willreturn memory(none)
define void @FillZero(int [2])(ptr noalias noundef readnone byval([2 x i32]) align 4 captures(none) %0) local_unnamed_addr #0 !dbg !8 {
    #dbg_declare(ptr %0, !17, !DIExpression(), !18)
  ret void, !dbg !19
}
```

One DML shader with this issue is `Cast_float16_native_accum32_uint8_packed32_256_Strided8D.dat`

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: Deric C. (Icohedron)

<details>
After fixing #<!-- -->147114 there are now 14 DML shaders failing to pass validation due to error `Instructions should not read uninitialized value`. 
While this validation error was encountered and fixed before (#<!-- -->139011), this appears to another unique case.

Reproduction: https://godbolt.org/z/chcadYss3

The following shader defines and uses a function `FillZero` with an inout parameter to fill an array with zeros.
```hlsl
// compile args: -T lib_6_7 -Xclang -emit-llvm
export void FillZero(inout int v[2]) {
  v = {0,0};
}
```

Clang emits FillZero as a no-op function.
```llvm
; Function Attrs: alwaysinline mustprogress nofree norecurse nosync nounwind willreturn memory(none)
define void @<!-- -->FillZero(int [2])(ptr noalias noundef readnone byval([2 x i32]) align 4 captures(none) %0) local_unnamed_addr #<!-- -->0 !dbg !8 {
    #dbg_declare(ptr %0, !17, !DIExpression(), !18)
  ret void, !dbg !19
}
```

One DML shader with this issue is `Cast_float16_native_accum32_uint8_packed32_256_Strided8D.dat`
</details>


---

