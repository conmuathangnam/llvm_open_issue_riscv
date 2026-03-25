# [hexagon] Assertion failure `I != Instrs.begin() - 1 && "Couldn't find producer"`

**Author:** androm3da
**URL:** https://github.com/llvm/llvm-project/issues/180614
**Status:** Closed
**Labels:** backend:Hexagon, release:backport, crash
**Closed Date:** 2026-02-24T20:34:42Z

## Body

This reproduces an assertion in llc when compiling for Hexagon with `--disable-packetizer`.  This was discovered while bootstrapping a native toolchain for hexagon.  The LLVM sources triggered another issue which we tried to workaround with `--disable-packetizer` but then that failed also.

```
llc -mtriple=hexagon-unknown-linux-musl -mcpu=hexagonv68 -O1 --disable-packetizer --filetype=obj -o /dev/null reduced.ll
```

reduced.ll:
```llvm
; ModuleID = '/home/brian/src/toolchain_for_hexagon/dis_packetizer_bug/reduced.bc'
target datalayout = "e-m:e-p:32:32:32-a:0-n16:32-i64:64:64-i32:32:32-i16:16:16-i1:8:8-f32:32:32-f64:64:64-v32:32:32-v64:64:64-v512:512:512-v1024:1024:1024-v2048:2048:2048"
target triple = "hexagon-unknown-linux-musl"

define i32 @_ZN4llvm2cl16ExpansionContextC2ERNS_20BumpPtrAllocatorImplINS_15MallocAllocatorELj4096ELj4096ELj128EEEPFvNS_9StringRefERNS_11StringSaverERNS_15SmallVectorImplIPKcEEbEPNS_3vfs10FileSystemE() {
entry:
  %0 = load ptr, ptr null, align 4
  %tobool.not.i.i = icmp eq ptr %0, null
  br i1 %tobool.not.i.i, label %cleanup.done9, label %if.then.i.i

common.ret:                                       ; preds = %cleanup.done9, %if.then.i.i
  %common.ret.op = phi i32 [ 0, %if.then.i.i ], [ 0, %cleanup.done9 ]
  ret i32 %common.ret.op

if.then.i.i:                                      ; preds = %entry
  %1 = atomicrmw sub ptr null, i32 0 monotonic, align 4
  br label %common.ret

cleanup.done9:                                    ; preds = %entry
  call void @llvm.memset.p0.i64(ptr null, i8 0, i64 19, i1 false)
  br label %common.ret
}

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: write)
declare void @llvm.memset.p0.i64(ptr writeonly captures(none), i8, i64, i1 immarg) #0

attributes #0 = { nocallback nofree nounwind willreturn memory(argmem: write) }

```




## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-hexagon

Author: Brian Cain (androm3da)

<details>
This reproduces an assertion in llc when compiling for Hexagon with `--disable-packetizer`.  This was discovered while bootstrapping a native toolchain for hexagon.  The LLVM sources triggered another issue which we tried to workaround with `--disable-packetizer` but then that failed also.

```
llc -mtriple=hexagon-unknown-linux-musl -mcpu=hexagonv68 -O1 --disable-packetizer --filetype=obj -o /dev/null reduced.ll
```

reduced.ll:
```
; ModuleID = '/home/brian/src/toolchain_for_hexagon/dis_packetizer_bug/reduced.bc'
target datalayout = "e-m:e-p:32:32:32-a:0-n16:32-i64:64:64-i32:32:32-i16:16:16-i1:8:8-f32:32:32-f64:64:64-v32:32:32-v64:64:64-v512:512:512-v1024:1024:1024-v2048:2048:2048"
target triple = "hexagon-unknown-linux-musl"

define i32 @<!-- -->_ZN4llvm2cl16ExpansionContextC2ERNS_20BumpPtrAllocatorImplINS_15MallocAllocatorELj4096ELj4096ELj128EEEPFvNS_9StringRefERNS_11StringSaverERNS_15SmallVectorImplIPKcEEbEPNS_3vfs10FileSystemE() {
entry:
  %0 = load ptr, ptr null, align 4
  %tobool.not.i.i = icmp eq ptr %0, null
  br i1 %tobool.not.i.i, label %cleanup.done9, label %if.then.i.i

common.ret:                                       ; preds = %cleanup.done9, %if.then.i.i
  %common.ret.op = phi i32 [ 0, %if.then.i.i ], [ 0, %cleanup.done9 ]
  ret i32 %common.ret.op

if.then.i.i:                                      ; preds = %entry
  %1 = atomicrmw sub ptr null, i32 0 monotonic, align 4
  br label %common.ret

cleanup.done9:                                    ; preds = %entry
  call void @<!-- -->llvm.memset.p0.i64(ptr null, i8 0, i64 19, i1 false)
  br label %common.ret
}

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: write)
declare void @<!-- -->llvm.memset.p0.i64(ptr writeonly captures(none), i8, i64, i1 immarg) #<!-- -->0

attributes #<!-- -->0 = { nocallback nofree nounwind willreturn memory(argmem: write) }

```



</details>


---

### Comment 2 - aankit-ca

/cherry-pick [e1be4df](https://github.com/llvm/llvm-project/commit/e1be4dfe1ef889c51d7e1704782a31dc65bb745c)

---

### Comment 3 - llvmbot

<!--IGNORE-->
Failed to cherry-pick: [e1be4df](https://github.com/llvm/llvm-project/commit/e1be4dfe1ef889c51d7e1704782a31dc65bb745c)

https://github.com/llvm/llvm-project/actions/runs/22367672742

Please manually backport the fix and push it to your github fork.  Once this is done, please create a [pull request](https://github.com/llvm/llvm-project/compare)

---

### Comment 4 - androm3da

/cherry-pick e1be4dfe1ef889c51d7e1704782a31dc65bb745c

---

### Comment 5 - llvmbot

/pull-request llvm/llvm-project#183162

---

