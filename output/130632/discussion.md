# [llvm][opt][regression] `memcpyopt` pass inserts incorrect `memmove` call

**Author:** HertzDevil
**URL:** https://github.com/llvm/llvm-project/issues/130632
**Status:** Closed
**Labels:** miscompilation, release:backport, llvm:transforms
**Closed Date:** 2025-03-12T13:53:59Z

## Body

The following LLVM IR, which is already partially optimized from earlier passes, allocates two `{ i64, i32, i64 }`s and swaps their contents using a non-default allocator function: (for less minimal reproductions see https://github.com/crystal-lang/crystal/issues/15543)

```llvm
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"

define void @main() {
alloca:
  %0 = call ptr @GC_malloc(i64 8)
  %1 = call ptr @GC_malloc(i64 48)
  store ptr %1, ptr %0, align 8
  %2 = getelementptr inbounds nuw i8, ptr %1, i64 24
  call void @llvm.memset.p0.i64(ptr noundef nonnull align 1 dereferenceable(24) %1, i8 18, i64 24, i1 false)
  call void @llvm.memset.p0.i64(ptr noundef nonnull align 1 dereferenceable(24) %2, i8 52, i64 24, i1 false)
  %3 = load ptr, ptr %0, align 8
  %4 = load i64, ptr %3, align 8
  %5 = getelementptr inbounds nuw i8, ptr %3, i64 8
  %6 = load { i32, i64 }, ptr %5, align 8
  %7 = getelementptr inbounds nuw i8, ptr %1, i64 32
  %8 = load { i32, i64 }, ptr %7, align 8
  store i64 3761688987579986996, ptr %1, align 8
  %9 = getelementptr inbounds nuw i8, ptr %1, i64 8
  store { i32, i64 } %8, ptr %9, align 8
  store i64 %4, ptr %2, align 8
  store { i32, i64 } %6, ptr %7, align 8
  ret void
}

declare ptr @GC_malloc(i64)

declare void @llvm.memset.p0.i64(ptr, i8, i64, i1)
```

Running just the `memcpyopt` pass used to produce the following:

```llvm
  %0 = call ptr @GC_malloc(i64 8)
  %1 = call ptr @GC_malloc(i64 48)
  store ptr %1, ptr %0, align 8
  %2 = getelementptr inbounds nuw i8, ptr %1, i64 24
  call void @llvm.memset.p0.i64(ptr noundef nonnull align 1 dereferenceable(24) %1, i8 18, i64 24, i1 false)
  call void @llvm.memset.p0.i64(ptr noundef nonnull align 1 dereferenceable(24) %2, i8 52, i64 24, i1 false)
  %3 = load ptr, ptr %0, align 8
  %4 = load i64, ptr %3, align 8
  %5 = getelementptr inbounds nuw i8, ptr %3, i64 8
  %6 = load { i32, i64 }, ptr %5, align 8
  %7 = getelementptr inbounds nuw i8, ptr %1, i64 32
  store i64 3761688987579986996, ptr %1, align 8
  %8 = getelementptr inbounds nuw i8, ptr %1, i64 8
  call void @llvm.memcpy.p0.p0.i64(ptr align 8 %8, ptr align 8 %7, i64 16, i1 false)
  store i64 %4, ptr %2, align 8
  store { i32, i64 } %6, ptr %7, align 8
  ret void
```

Since LLVM 20 this becomes:

```llvm
  %0 = call ptr @GC_malloc(i64 8)
  %1 = call ptr @GC_malloc(i64 48)
  store ptr %1, ptr %0, align 8
  %2 = getelementptr inbounds nuw i8, ptr %1, i64 24
  call void @llvm.memset.p0.i64(ptr noundef nonnull align 1 dereferenceable(24) %1, i8 18, i64 24, i1 false)
  call void @llvm.memset.p0.i64(ptr noundef nonnull align 1 dereferenceable(24) %2, i8 52, i64 24, i1 false)
  %3 = load ptr, ptr %0, align 8
  %4 = load i64, ptr %3, align 8
  %5 = getelementptr inbounds nuw i8, ptr %3, i64 8
  %6 = getelementptr inbounds nuw i8, ptr %1, i64 32
  store i64 3761688987579986996, ptr %1, align 8
  %7 = getelementptr inbounds nuw i8, ptr %1, i64 8
  call void @llvm.memcpy.p0.p0.i64(ptr align 8 %7, ptr align 8 %6, i64 16, i1 false)
  call void @llvm.memmove.p0.p0.i64(ptr align 8 %6, ptr align 8 %5, i64 16, i1 false)
  store i64 %4, ptr %2, align 8
  ret void
```

The new call to `llvm.memmove.p0.p0.i64` seems to assume that `%5` and `%7` can never alias, when they should be identical here. Adding `noalias` to `GC_malloc` does not appear to help, and I doubt this attribute is necessary.

The first PR that broke this was #108535.

[Compiler Explorer link](https://godbolt.org/z/Gfeja4M6c)

## Comments

### Comment 1 - nikic

Slightly reduced and using named values:
```llvm
define void @main(ptr %ptr, ptr %ptr.copy) {
  %ptr.8 = getelementptr inbounds nuw i8, ptr %ptr, i64 8
  %ptr.24 = getelementptr inbounds nuw i8, ptr %ptr, i64 24
  %ptr.32 = getelementptr inbounds nuw i8, ptr %ptr, i64 32
  %ptr.copy.8 = getelementptr inbounds nuw i8, ptr %ptr.copy, i64 8
  %struct1 = load { i32, i64 }, ptr %ptr.copy.8, align 8
  %struct2 = load { i32, i64 }, ptr %ptr.32, align 8
  store { i32, i64 } %struct2, ptr %ptr.8, align 8
  store i64 1, ptr %ptr.24, align 8
  store { i32, i64 } %struct1, ptr %ptr.32, align 8
  ret void
}
```

---

### Comment 2 - nikic

/cherry-pick 5da9044c40840187330526ca888290a95927a629

---

### Comment 3 - llvmbot

/pull-request llvm/llvm-project#130964

---

