# [ARM64EC] Can't call a virtual function by function pointer

**Author:** tuxzz
**URL:** https://github.com/llvm/llvm-project/issues/139856
**Status:** Closed
**Labels:** backend:AArch64, miscompilation
**Closed Date:** 2025-05-16T18:30:53Z

## Body

Minimal code to reproduce:
````c++
struct ChildClass {
  virtual void fn() {}
};


int main()
{
  ChildClass obj;
  auto fnptr = &ChildClass::fn;
  (obj.*fnptr)();
  return 0;
}
````

The crash point always like this:

> [0x0]   ConsoleApplication1!ChildClass::`vcall'{0}'+0x54   0x7d75cffb80   0x7ff6e577106c   

![Image](https://github.com/user-attachments/assets/345d9da2-1148-4834-b52f-484f694b04fe)

Verison: clang 20.1.4
target: arm64ec-pc-windows-msvc

This is a arm64ec specific problem. This can't be reproduced on arm64-pc-windows-msvc
For a simple virtual function call like `obj->fn()`, there is no problem.

This bug also makes `std::invoke`, and googletest unusable.

## Comments

### Comment 1 - tuxzz

Full IR:

````llvm
; ModuleID = 'ConsoleApplication1.cpp'
source_filename = "ConsoleApplication1.cpp"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-p:64:64-i32:32-i64:64-i128:128-n32:64-S128-Fn32"
target triple = "arm64ec-pc-windows-msvc19.44.35207"

%rtti.CompleteObjectLocator = type { i32, i32, i32, i32, i32, i32 }
%rtti.TypeDescriptor16 = type { ptr, ptr, [17 x i8] }
%rtti.ClassHierarchyDescriptor = type { i32, i32, i32, i32 }
%rtti.BaseClassDescriptor = type { i32, i32, i32, i32, i32, i32, i32 }
%struct.ChildClass = type { ptr }

$"??0ChildClass@@QEAA@XZ" = comdat any

$"??_9ChildClass@@$BA@AA" = comdat any

$"?fn@ChildClass@@UEAAXXZ" = comdat any

$"??_7ChildClass@@6B@" = comdat largest

$"??_R4ChildClass@@6B@" = comdat any

$"??_R0?AUChildClass@@@8" = comdat any

$"??_R3ChildClass@@8" = comdat any

$"??_R2ChildClass@@8" = comdat any

$"??_R1A@?0A@EA@ChildClass@@8" = comdat any

@0 = private unnamed_addr constant { [2 x ptr] } { [2 x ptr] [ptr @"??_R4ChildClass@@6B@", ptr @"?fn@ChildClass@@UEAAXXZ"] }, comdat($"??_7ChildClass@@6B@")
@"??_R4ChildClass@@6B@" = linkonce_odr constant %rtti.CompleteObjectLocator { i32 1, i32 0, i32 0, i32 trunc (i64 sub nuw nsw (i64 ptrtoint (ptr @"??_R0?AUChildClass@@@8" to i64), i64 ptrtoint (ptr @__ImageBase to i64)) to i32), i32 trunc (i64 sub nuw nsw (i64 ptrtoint (ptr @"??_R3ChildClass@@8" to i64), i64 ptrtoint (ptr @__ImageBase to i64)) to i32), i32 trunc (i64 sub nuw nsw (i64 ptrtoint (ptr @"??_R4ChildClass@@6B@" to i64), i64 ptrtoint (ptr @__ImageBase to i64)) to i32) }, comdat
@"??_7type_info@@6B@" = external constant ptr
@"??_R0?AUChildClass@@@8" = linkonce_odr global %rtti.TypeDescriptor16 { ptr @"??_7type_info@@6B@", ptr null, [17 x i8] c".?AUChildClass@@\00" }, comdat
@__ImageBase = external dso_local constant i8
@"??_R3ChildClass@@8" = linkonce_odr constant %rtti.ClassHierarchyDescriptor { i32 0, i32 0, i32 1, i32 trunc (i64 sub nuw nsw (i64 ptrtoint (ptr @"??_R2ChildClass@@8" to i64), i64 ptrtoint (ptr @__ImageBase to i64)) to i32) }, comdat
@"??_R2ChildClass@@8" = linkonce_odr constant [2 x i32] [i32 trunc (i64 sub nuw nsw (i64 ptrtoint (ptr @"??_R1A@?0A@EA@ChildClass@@8" to i64), i64 ptrtoint (ptr @__ImageBase to i64)) to i32), i32 0], comdat
@"??_R1A@?0A@EA@ChildClass@@8" = linkonce_odr constant %rtti.BaseClassDescriptor { i32 trunc (i64 sub nuw nsw (i64 ptrtoint (ptr @"??_R0?AUChildClass@@@8" to i64), i64 ptrtoint (ptr @__ImageBase to i64)) to i32), i32 0, i32 0, i32 -1, i32 0, i32 64, i32 trunc (i64 sub nuw nsw (i64 ptrtoint (ptr @"??_R3ChildClass@@8" to i64), i64 ptrtoint (ptr @__ImageBase to i64)) to i32) }, comdat

@"??_7ChildClass@@6B@" = unnamed_addr alias ptr, getelementptr inbounds ({ [2 x ptr] }, ptr @0, i32 0, i32 0, i32 1)

; Function Attrs: mustprogress noinline norecurse optnone sspstrong uwtable
define dso_local noundef i32 @main() #0 !dbg !12 {
  %1 = alloca i32, align 4
  %2 = alloca %struct.ChildClass, align 8
  %3 = alloca ptr, align 8
  store i32 0, ptr %1, align 4
    #dbg_declare(ptr %2, !17, !DIExpression(), !27)
  %4 = call noundef ptr @"??0ChildClass@@QEAA@XZ"(ptr noundef nonnull align 8 dereferenceable(8) %2) #3, !dbg !27
    #dbg_declare(ptr %3, !28, !DIExpression(), !30)
  store ptr @"??_9ChildClass@@$BA@AA", ptr %3, align 8, !dbg !30
  %5 = load ptr, ptr %3, align 8, !dbg !31
  call void %5(ptr noundef nonnull align 8 dereferenceable(8) %2), !dbg !31
  ret i32 0, !dbg !32
}

; Function Attrs: mustprogress noinline nounwind optnone sspstrong uwtable
define linkonce_odr dso_local noundef ptr @"??0ChildClass@@QEAA@XZ"(ptr noundef nonnull returned align 8 dereferenceable(8) %0) unnamed_addr #1 comdat !dbg !33 {
  %2 = alloca ptr, align 8
  store ptr %0, ptr %2, align 8
    #dbg_declare(ptr %2, !35, !DIExpression(), !37)
  %3 = load ptr, ptr %2, align 8
  store ptr @"??_7ChildClass@@6B@", ptr %3, align 8, !dbg !38
  ret ptr %3, !dbg !38
}

; Function Attrs: noinline optnone sspstrong uwtable
define linkonce_odr void @"??_9ChildClass@@$BA@AA"(ptr noundef %0, ...) #2 comdat !dbg !39 {
  %2 = alloca ptr, align 8
  store ptr %0, ptr %2, align 8
  %3 = load ptr, ptr %2, align 8, !dbg !41
  %4 = load ptr, ptr %3, align 8, !dbg !41
  %5 = getelementptr inbounds ptr, ptr %4, i64 0, !dbg !41
  %6 = load ptr, ptr %5, align 8, !dbg !41
  musttail call void (ptr, ...) %6(ptr noundef %3, ...), !dbg !41
  ret void, !dbg !41

7:                                                ; No predecessors!
  ret void, !dbg !41
}

; Function Attrs: mustprogress noinline nounwind optnone sspstrong uwtable
define linkonce_odr dso_local void @"?fn@ChildClass@@UEAAXXZ"(ptr noundef nonnull align 8 dereferenceable(8) %0) unnamed_addr #1 comdat !dbg !42 {
  %2 = alloca ptr, align 8
  store ptr %0, ptr %2, align 8
    #dbg_declare(ptr %2, !43, !DIExpression(), !44)
  %3 = load ptr, ptr %2, align 8
  ret void, !dbg !45
}

attributes #0 = { mustprogress noinline norecurse optnone sspstrong uwtable "frame-pointer"="non-leaf" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="generic" "target-features"="+fp-armv8,+neon,+v8a,-fmv" }
attributes #1 = { mustprogress noinline nounwind optnone sspstrong uwtable "frame-pointer"="non-leaf" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="generic" "target-features"="+fp-armv8,+neon,+v8a,-fmv" }
attributes #2 = { noinline optnone sspstrong uwtable "frame-pointer"="non-leaf" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="generic" "target-features"="+fp-armv8,+neon,+v8a,-fmv" "thunk" }
attributes #3 = { nounwind }

!llvm.dbg.cu = !{!0}
!llvm.linker.options = !{!2, !3, !4}
!llvm.module.flags = !{!5, !6, !7, !8, !9, !10}
!llvm.ident = !{!11}

!0 = distinct !DICompileUnit(language: DW_LANG_C_plus_plus_14, file: !1, producer: "clang version 20.1.4", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false, nameTableKind: None)
!1 = !DIFile(filename: "ConsoleApplication1.cpp", directory: "C:\\Users\\doraz\\Desktop\\codebase\\ConsoleApplication1", checksumkind: CSK_MD5, checksum: "2b54e0c3c915e420ddf1a3e943e1de55")
!2 = !{!"/DEFAULTLIB:libcmtd.lib"}
!3 = !{!"/DEFAULTLIB:oldnames.lib"}
!4 = !{!"/DEFAULTLIB:softintrin.lib"}
!5 = !{i32 2, !"CodeView", i32 1}
!6 = !{i32 2, !"Debug Info Version", i32 3}
!7 = !{i32 1, !"wchar_size", i32 2}
!8 = !{i32 8, !"PIC Level", i32 2}
!9 = !{i32 7, !"uwtable", i32 2}
!10 = !{i32 7, !"frame-pointer", i32 1}
!11 = !{!"clang version 20.1.4"}
!12 = distinct !DISubprogram(name: "main", scope: !1, file: !1, line: 9, type: !13, scopeLine: 10, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !16)
!13 = !DISubroutineType(types: !14)
!14 = !{!15}
!15 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!16 = !{}
!17 = !DILocalVariable(name: "obj", scope: !12, file: !1, line: 11, type: !18)
!18 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "ChildClass", file: !1, line: 4, size: 64, flags: DIFlagTypePassByReference | DIFlagNonTrivial, elements: !19, vtableHolder: !18, identifier: ".?AUChildClass@@")
!19 = !{!20, !21, !23}
!20 = !DIDerivedType(tag: DW_TAG_pointer_type, name: "__vtbl_ptr_type", baseType: null, size: 64)
!21 = !DIDerivedType(tag: DW_TAG_member, name: "_vptr$ChildClass", scope: !1, file: !1, baseType: !22, size: 64, flags: DIFlagArtificial)
!22 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !20, size: 64)
!23 = !DISubprogram(name: "fn", linkageName: "?fn@ChildClass@@UEAAXXZ", scope: !18, file: !1, line: 5, type: !24, scopeLine: 5, containingType: !18, virtualIndex: 0, flags: DIFlagPrototyped | DIFlagIntroducedVirtual, spFlags: DISPFlagVirtual)
!24 = !DISubroutineType(types: !25)
!25 = !{null, !26}
!26 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !18, size: 64, flags: DIFlagArtificial | DIFlagObjectPointer)
!27 = !DILocation(line: 11, scope: !12)
!28 = !DILocalVariable(name: "fnptr", scope: !12, file: !1, line: 12, type: !29)
!29 = !DIDerivedType(tag: DW_TAG_ptr_to_member_type, baseType: !24, size: 64, flags: DIFlagSingleInheritance, extraData: !18)
!30 = !DILocation(line: 12, scope: !12)
!31 = !DILocation(line: 13, scope: !12)
!32 = !DILocation(line: 14, scope: !12)
!33 = distinct !DISubprogram(name: "ChildClass", linkageName: "??0ChildClass@@QEAA@XZ", scope: !18, file: !1, line: 4, type: !24, scopeLine: 4, flags: DIFlagArtificial | DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !0, declaration: !34, retainedNodes: !16)
!34 = !DISubprogram(name: "ChildClass", scope: !18, type: !24, flags: DIFlagArtificial | DIFlagPrototyped, spFlags: 0)
!35 = !DILocalVariable(name: "this", arg: 1, scope: !33, type: !36, flags: DIFlagArtificial | DIFlagObjectPointer)
!36 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !18, size: 64)
!37 = !DILocation(line: 0, scope: !33)
!38 = !DILocation(line: 4, scope: !33)
!39 = distinct !DISubprogram(linkageName: "??_9ChildClass@@$BA@AA", scope: !1, file: !1, line: 5, type: !40, flags: DIFlagArtificial | DIFlagThunk, spFlags: DISPFlagDefinition, unit: !0)
!40 = !DISubroutineType(types: !16)
!41 = !DILocation(line: 5, scope: !39)
!42 = distinct !DISubprogram(name: "fn", linkageName: "?fn@ChildClass@@UEAAXXZ", scope: !18, file: !1, line: 5, type: !24, scopeLine: 5, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !0, declaration: !23, retainedNodes: !16)
!43 = !DILocalVariable(name: "this", arg: 1, scope: !42, type: !36, flags: DIFlagArtificial | DIFlagObjectPointer)
!44 = !DILocation(line: 0, scope: !42)
!45 = !DILocation(line: 5, scope: !42)

````

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: tux (tuxzz)

<details>
Minimal code to reproduce:
````c++
struct ChildClass {
  virtual void fn() {}
};


int main()
{
  ChildClass obj;
  auto fnptr = &amp;ChildClass::fn;
  (obj.*fnptr)();
  return 0;
}
````

The crash point always like this:

&gt; [0x0]   ConsoleApplication1!ChildClass::`vcall'{0}'+0x54   0x7d75cffb80   0x7ff6e577106c   

![Image](https://github.com/user-attachments/assets/345d9da2-1148-4834-b52f-484f694b04fe)

Verison: clang 20.1.4
target: arm64ec-pc-windows-msvc

This is a arm64ec specific problem. This can't be reproduced on arm64-pc-windows-msvc
For a simple virtual function call like `obj-&gt;fn()`, there is no problem.

This bug also makes `std::invoke`, and googletest unusable.
</details>


---

### Comment 3 - llvmbot


@llvm/issue-subscribers-backend-aarch64

Author: tux (tuxzz)

<details>
Minimal code to reproduce:
````c++
struct ChildClass {
  virtual void fn() {}
};


int main()
{
  ChildClass obj;
  auto fnptr = &amp;ChildClass::fn;
  (obj.*fnptr)();
  return 0;
}
````

The crash point always like this:

&gt; [0x0]   ConsoleApplication1!ChildClass::`vcall'{0}'+0x54   0x7d75cffb80   0x7ff6e577106c   

![Image](https://github.com/user-attachments/assets/345d9da2-1148-4834-b52f-484f694b04fe)

Verison: clang 20.1.4
target: arm64ec-pc-windows-msvc

This is a arm64ec specific problem. This can't be reproduced on arm64-pc-windows-msvc
For a simple virtual function call like `obj-&gt;fn()`, there is no problem.

This bug also makes `std::invoke`, and googletest unusable.
</details>


---

