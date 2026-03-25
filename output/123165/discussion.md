# Hexagon backend assertion "Decremented past the beginning of region to repair" in 7zIn.cpp

**Author:** androm3da
**URL:** https://github.com/llvm/llvm-project/issues/123165
**Status:** Closed
**Labels:** backend:Hexagon, llvm:codegen, crash
**Closed Date:** 2025-01-31T05:23:34Z

## Body

clang++ asserts when compiling [7zIn.cpp's `CInArchive::GetNextFolderItem(CFolder &folder)`](https://github.com/llvm/llvm-test-suite/blob/1f917f95918479727e727c710348d9bf674588fc/MultiSource/Benchmarks/7zip/CPP/7zip/Archive/7z/7zIn.cpp#L385) from `llvm-test-suite`:

```
hexagon-unknown-linux-musl-clang++: /local/mnt/workspace/upstream/toolchain_for_hexagon/llvm-project/llvm/lib/CodeGen/SlotIndexes.cpp:218: void llvm::SlotIndexes::repairIndexesInRange(MachineBasicBlock *, MachineBasicBlock::iterator, MachineBasicBlock::iterator): Assertion `ListI->getIndex() >= startIdx.getIndex() && (includeStart || !pastStart) && "Decremented past the beginning of region to repair."' failed.
```

I have reduced this failure to the test case below and bisected the failure to b6bf4024a031a5e7b58aff1425d94841a88002d6.

`./bin/llc --mtriple=hexagon -O2 ~/src/7z_20_0_0.ll --window-sched=off` does not assert.

Reduced failure context:
```
$ ./bin/llc --mtriple=hexagon -O2 ~/src/7z_20_0_0.ll
llc: /local/mnt/workspace/upstream/llvm-project/llvm/lib/CodeGen/SlotIndexes.cpp:218: void llvm::SlotIndexes::repairIndexesInRange(MachineBasicBlock *, MachineBasicBlock::iterator, MachineBasicBlock::iterator): Assertion `ListI->getIndex() >= startIdx.getIndex() && (includeStart || !pastStart) && "Decremented past the beginning of region to repair."' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.	Program arguments: ./bin/llc --mtriple=hexagon -O2 /usr2/bcain/src/7z_20_0_0.ll

...
```

`7z_20_0_0.ll`:
```
define void @_ZN10CInArchive17GetNextFolderItemEv(ptr %this) {
for.body.lr.ph.i:
  br label %for.body.i.epil

for.body.i.epil:                                  ; preds = %for.body.i.epil, %for.body.lr.ph.i
  %epil.iter = phi i32 [ %epil.iter.next, %for.body.i.epil ], [ 0, %for.body.lr.ph.i ]
  %0 = load ptr, ptr %this, align 8
  %1 = load i8, ptr %0, align 1
  %2 = load ptr, ptr null, align 8
  store i8 %1, ptr %2, align 1
  %epil.iter.next = add i32 %epil.iter, 1
  %epil.iter.cmp.not = icmp eq i32 %epil.iter, 0
  br i1 %epil.iter.cmp.not, label %for.body, label %for.body.i.epil

for.body.epil:                                    ; preds = %for.body
  %sub.epil = xor i32 %j.012, 0
  store i32 %or911, ptr %this, align 4
  ret void

for.body:                                         ; preds = %for.body, %for.body.i.epil
  %j.012 = phi i32 [ %inc.7, %for.body ], [ 0, %for.body.i.epil ]
  %or911 = phi i32 [ %or.7, %for.body ], [ 0, %for.body.i.epil ]
  %niter19 = phi i32 [ %niter19.next.7, %for.body ], [ 0, %for.body.i.epil ]
  %or.7 = or i32 1, %j.012
  %inc.7 = add i32 %j.012, 1
  %niter19.next.7 = add i32 %niter19, 1
  %niter19.ncmp.7 = icmp eq i32 %niter19, 0
  br i1 %niter19.ncmp.7, label %for.body.epil, label %for.body
}
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-hexagon

Author: Brian Cain (androm3da)

<details>
clang++ asserts when compiling [7zIn.cpp's `CInArchive::GetNextFolderItem(CFolder &amp;folder)`](https://github.com/llvm/llvm-test-suite/blob/1f917f95918479727e727c710348d9bf674588fc/MultiSource/Benchmarks/7zip/CPP/7zip/Archive/7z/7zIn.cpp#L385) from `llvm-test-suite`:

```
hexagon-unknown-linux-musl-clang++: /local/mnt/workspace/upstream/toolchain_for_hexagon/llvm-project/llvm/lib/CodeGen/SlotIndexes.cpp:218: void llvm::SlotIndexes::repairIndexesInRange(MachineBasicBlock *, MachineBasicBlock::iterator, MachineBasicBlock::iterator): Assertion `ListI-&gt;getIndex() &gt;= startIdx.getIndex() &amp;&amp; (includeStart || !pastStart) &amp;&amp; "Decremented past the beginning of region to repair."' failed.
```

I have reduced this failure to the test case below and bisected the failure to b6bf4024a031a5e7b58aff1425d94841a88002d6.

`./bin/llc --mtriple=hexagon -O2 ~/src/7z_20_0_0.ll --window-sched=off` does not assert.

Reduced failure context:
```
$ ./bin/llc --mtriple=hexagon -O2 ~/src/7z_20_0_0.ll
llc: /local/mnt/workspace/upstream/llvm-project/llvm/lib/CodeGen/SlotIndexes.cpp:218: void llvm::SlotIndexes::repairIndexesInRange(MachineBasicBlock *, MachineBasicBlock::iterator, MachineBasicBlock::iterator): Assertion `ListI-&gt;getIndex() &gt;= startIdx.getIndex() &amp;&amp; (includeStart || !pastStart) &amp;&amp; "Decremented past the beginning of region to repair."' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.	Program arguments: ./bin/llc --mtriple=hexagon -O2 /usr2/bcain/src/7z_20_0_0.ll

...
```

`7z_20_0_0.ll`:
```
define void @<!-- -->_ZN10CInArchive17GetNextFolderItemEv(ptr %this) {
for.body.lr.ph.i:
  br label %for.body.i.epil

for.body.i.epil:                                  ; preds = %for.body.i.epil, %for.body.lr.ph.i
  %epil.iter = phi i32 [ %epil.iter.next, %for.body.i.epil ], [ 0, %for.body.lr.ph.i ]
  %0 = load ptr, ptr %this, align 8
  %1 = load i8, ptr %0, align 1
  %2 = load ptr, ptr null, align 8
  store i8 %1, ptr %2, align 1
  %epil.iter.next = add i32 %epil.iter, 1
  %epil.iter.cmp.not = icmp eq i32 %epil.iter, 0
  br i1 %epil.iter.cmp.not, label %for.body, label %for.body.i.epil

for.body.epil:                                    ; preds = %for.body
  %sub.epil = xor i32 %j.012, 0
  store i32 %or911, ptr %this, align 4
  ret void

for.body:                                         ; preds = %for.body, %for.body.i.epil
  %j.012 = phi i32 [ %inc.7, %for.body ], [ 0, %for.body.i.epil ]
  %or911 = phi i32 [ %or.7, %for.body ], [ 0, %for.body.i.epil ]
  %niter19 = phi i32 [ %niter19.next.7, %for.body ], [ 0, %for.body.i.epil ]
  %or.7 = or i32 1, %j.012
  %inc.7 = add i32 %j.012, 1
  %niter19.next.7 = add i32 %niter19, 1
  %niter19.ncmp.7 = icmp eq i32 %niter19, 0
  br i1 %niter19.ncmp.7, label %for.body.epil, label %for.body
}
```
</details>


---

### Comment 2 - androm3da

Here's the [unabridged test log](https://github.com/user-attachments/files/18434406/test.log.gz) in case it's helpful to see the failure in context.

---

### Comment 3 - androm3da

cc @huaatian - primary author of the window scheduler and has fixed other regressions, in case they have any advice.

---

### Comment 4 - huaatian

Okay, I will analyze this issue and resolve it as soon as possible.

---

### Comment 5 - iajbar

Thanks @huaatian for looking into this.

---

### Comment 6 - androm3da

This was fixed by #123184

Thanks @huaatian !

---

