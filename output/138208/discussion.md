# Codegenprepare creates invalid IR

**Author:** aeubanks
**URL:** https://github.com/llvm/llvm-project/issues/138208
**Status:** Closed
**Labels:** llvm:codegen, crash-on-valid
**Closed Date:** 2025-05-15T16:27:26Z

## Body

```
$ cat a.ll
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-grtev4-linux-gnu"

declare void @g(ptr)

define void @f(ptr %arg) {
bb:
  %getelementptr = getelementptr i8, ptr %arg, i64 -64
  %getelementptr1 = getelementptr i8, ptr %arg, i64 -56
  call void @g(ptr %getelementptr)
  br label %bb3

bb3:
  %load = load ptr, ptr %getelementptr, align 8
  %load4 = load i32, ptr %getelementptr1, align 8
  %load5 = load ptr, ptr %getelementptr, align 8
  %add = add i32 1, 0
  %icmp = icmp eq i32 %add, 0
  br i1 %icmp, label %bb7, label %bb7

bb7:
  ret void
}

$ opt -p 'require<profile-summary>,codegenprepare' a.ll -cgpp-huge-func=0 -disable-output  
Instruction does not dominate all uses!
  %sunkaddr1 = getelementptr i8, ptr %arg, i64 -64
  %load = load ptr, ptr %sunkaddr1, align 8
LLVM ERROR: Broken module found, compilation aborted!
```

seems like the GEP we create [here](https://github.com/llvm/llvm-project/blob/a76936f1c01c7cadbce8ea6553af758d0f614b6a/llvm/lib/CodeGen/CodeGenPrepare.cpp#L6059) is getting inserted right before `%load5` to use in that load, but then we end up reusing that one [here](https://github.com/llvm/llvm-project/blob/a76936f1c01c7cadbce8ea6553af758d0f614b6a/llvm/lib/CodeGen/CodeGenPrepare.cpp#L5909) for `%load` in a later iteration

