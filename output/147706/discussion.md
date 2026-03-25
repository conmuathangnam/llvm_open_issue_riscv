# Attributor assigns `!invariant.load` attribute incorrectly

**Author:** igogo-x86
**URL:** https://github.com/llvm/llvm-project/issues/147706
**Status:** Open
**Labels:** miscompilation, llvm::Attributor

## Body

Reproducer:

```
; ModuleID = 'test-invariant.c'
source_filename = "test-invariant.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i8:8:32-i16:16:32-i64:64-i128:128-n32:64-S128-Fn32"
target triple = "aarch64-unknown-linux-gnu"

define void @init(ptr %call) #0 {
	store i32 100, ptr %call, align 4
	ret void
}

define i32 @read(ptr %call) {
  %arrayidx = getelementptr inbounds float, ptr %call, i64 0
  %result = load i32, ptr %arrayidx
	ret i32 %result
}

define i32 @x()  {
  %call = alloca i32, align 4
  call void @init(ptr %call)
  %r = call i32 @read(ptr %call)
  ret i32 %r
}

attributes #0 = { noinline }
```

After running `opt -S -passes=attributor,inline,gvn < test-invariant.ll` function `x` returns under and `load i32, ptr %call` gets incorrect `!invaraint.load` attribute:


```
; Function Attrs: mustprogress nofree noinline norecurse nosync nounwind willreturn memory(argmem: write)
define void @init(ptr nofree noundef nonnull writeonly align 4 captures(none) dereferenceable(4) %call) #0 {
  store i32 100, ptr %call, align 4
  ret void
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: read)
define i32 @read(ptr nofree nonnull readonly align 4 captures(none) dereferenceable(4) %call) #1 {
  %result = load i32, ptr %call, align 4, !invariant.load !0
  ret i32 %result
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none)
define i32 @x() #2 {
  %call = alloca i32, align 4
  call void @init(ptr noalias nofree noundef nonnull writeonly align 4 captures(none) dereferenceable(4) %call) #4
  call void @llvm.experimental.noalias.scope.decl(metadata !1)
  ret i32 undef
}

```

## Comments

### Comment 1 - antoniofrighetto

Looks like this occurs only when passing through the zero-indexed gep.

---

### Comment 2 - antoniofrighetto

@nikic The memory is written only once in `init` and never modified elsewhere. Regardless of the type-punning to float (happens with integer type too), am I missing something or Attributor may correctly infer that the load is invariant here (thus adding !invariant.load)? Or should the region be delimited by llvm.invariant.start/end?

---

### Comment 3 - nikic

> The optional !invariant.load metadata must reference a single metadata name <empty_node> corresponding to a metadata node with no entries. If a load instruction tagged with the !invariant.load metadata is executed, the memory location referenced by the load has to contain the same value at all points in the program where the memory location is dereferenceable; otherwise, the behavior is undefined.

`!invariant.load` is an absolute statement, it's not valid to use it for "it's invariant starting from ..." cases.

---

