# [ControlHeightReductionPass] Creates invalid IR in ThinLTO Backend on AArch64 when building clang with ThinLTO+IRPGO

**Author:** boomanaiden154
**URL:** https://github.com/llvm/llvm-project/issues/159621
**Status:** Closed
**Labels:** crash, LTO, llvm:transforms
**Closed Date:** 2025-09-22T20:58:01Z

## Body

https://gist.github.com/boomanaiden154/7f1400804c879e25a012d7907f3922c9 is the IR.

Running `opt -mtriple=aarch64-unknown-linux-gnu -passes="default<O3>" <file path> -disable-output` will reproduce the issue. The file comes from compiling clang on Aarch64 when using the BOLT caches. More details on the failure are in https://github.com/llvm/llvm-project/pull/159367.

## Comments

### Comment 1 - boomanaiden154

Working on minimizing this currently.

---

### Comment 2 - boomanaiden154

https://gist.github.com/boomanaiden154/3ae4fd0efc2cefa4d006fe1a4caec509 is the crash log.

---

### Comment 3 - boomanaiden154

Reduced IR:
```llvm
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i8:8:32-i16:16:32-i64:64-i128:128-n32:64-S128-Fn32"
target triple = "aarch64-unknown-linux-gnu"

%"class.llvm::SmallVector.136" = type { %"class.llvm::SmallVectorImpl.76", %"struct.llvm::SmallVectorStorage.137" }
%"class.llvm::SmallVectorImpl.76" = type { %"class.llvm::SmallVectorTemplateBase.77" }
%"class.llvm::SmallVectorTemplateBase.77" = type { %"class.llvm::SmallVectorTemplateCommon.78" }
%"class.llvm::SmallVectorTemplateCommon.78" = type { %"class.llvm::SmallVectorBase" }
%"class.llvm::SmallVectorBase" = type { ptr, i32, i32 }
%"struct.llvm::SmallVectorStorage.137" = type { [256 x i8] }

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare void @llvm.lifetime.start.p0(ptr captures(none)) #0

define i1 @_ZN4llvm23MemoryDependenceResults27getNonLocalPointerDepFromBBEPNS_11InstructionERKNS_12PHITransAddrERKNS_14MemoryLocationEbPNS_10BasicBlockERNS_15SmallVectorImplINS_17NonLocalDepResultEEERNS_13SmallDenseMapISA_PNS_5ValueELj16ENS_12DenseMapInfoISA_vEENS_6detail12DenseMapPairISA_SH_EEEEbb(ptr %this, ptr %QueryInst, i1 %isLoad, i1 %cmp.i.i.not.i.i) !prof !29 {
entry:
  %Worklist1 = alloca %"class.llvm::SmallVector.136", align 8
  %shl.i.i.i = select i1 %isLoad, i64 4, i64 0, !prof !30
  br i1 %cmp.i.i.not.i.i, label %_ZNSt6vectorIN4llvm16NonLocalDepEntryESaIS1_EEC2ERKS3_.exit, label %if.end.i.i, !prof !31

if.end.i.i:                                       ; preds = %entry
  call void @llvm.assume(i1 %isLoad)
  br label %_ZNSt6vectorIN4llvm16NonLocalDepEntryESaIS1_EEC2ERKS3_.exit

_ZNSt6vectorIN4llvm16NonLocalDepEntryESaIS1_EEC2ERKS3_.exit: ; preds = %if.end.i.i, %entry
  store i64 %shl.i.i.i, ptr %QueryInst, align 8
  call void @llvm.lifetime.start.p0(ptr %Worklist1)
  store ptr %Worklist1, ptr %this, align 8
  br label %for.body.i.i.i.i1401

for.cond.preheader.i.i.i:                         ; preds = %for.body.i.i.i.i1401
  ret i1 false

for.body.i.i.i.i1401:                             ; preds = %for.body.i.i.i.i1401, %_ZNSt6vectorIN4llvm16NonLocalDepEntryESaIS1_EEC2ERKS3_.exit
  %__begin0.0.i34.i.i.i = phi ptr [ %incdec.ptr.i.i.i.i1402, %for.body.i.i.i.i1401 ], [ null, %_ZNSt6vectorIN4llvm16NonLocalDepEntryESaIS1_EEC2ERKS3_.exit ]
  %incdec.ptr.i.i.i.i1402 = getelementptr i8, ptr %__begin0.0.i34.i.i.i, i64 24
  %cmp.not.i.i.i.i1403 = icmp eq ptr %__begin0.0.i34.i.i.i, %this
  br i1 %cmp.not.i.i.i.i1403, label %for.cond.preheader.i.i.i, label %for.body.i.i.i.i1401
}

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(inaccessiblemem: write)
declare void @llvm.assume(i1 noundef) #1

attributes #0 = { nocallback nofree nosync nounwind willreturn memory(argmem: readwrite) }
attributes #1 = { nocallback nofree nosync nounwind willreturn memory(inaccessiblemem: write) }

!llvm.module.flags = !{!0}

!0 = !{i32 1, !"ProfileSummary", !1}
!1 = !{!2, !3, !4, !5, !6, !7, !8, !9, !10, !11}
!2 = !{!"ProfileFormat", !"InstrProf"}
!3 = !{!"TotalCount", i64 51888332104}
!4 = !{!"MaxCount", i64 367187222}
!5 = !{!"MaxInternalCount", i64 367187222}
!6 = !{!"MaxFunctionCount", i64 281208867}
!7 = !{!"NumCounts", i64 1698950}
!8 = !{!"NumFunctions", i64 164155}
!9 = !{!"IsPartialProfile", i64 0}
!10 = !{!"PartialProfileRatio", double 0.000000e+00}
!11 = !{!"DetailedSummary", !12}
!12 = !{!13, !14, !15, !16, !17, !18, !19, !20, !21, !22, !23, !24, !25, !26, !27, !28}
!13 = !{i32 10000, i64 281208867, i32 2}
!14 = !{i32 100000, i64 88528372, i32 35}
!15 = !{i32 200000, i64 38263822, i32 128}
!16 = !{i32 300000, i64 20879095, i32 314}
!17 = !{i32 400000, i64 12121080, i32 643}
!18 = !{i32 500000, i64 6627144, i32 1239}
!19 = !{i32 600000, i64 3697961, i32 2312}
!20 = !{i32 700000, i64 2031812, i32 4214}
!21 = !{i32 800000, i64 988368, i32 7927}
!22 = !{i32 900000, i64 365146, i32 16522}
!23 = !{i32 950000, i64 149448, i32 27512}
!24 = !{i32 990000, i64 25291, i32 58500}
!25 = !{i32 999000, i64 2484, i32 108076}
!26 = !{i32 999900, i64 238, i32 156548}
!27 = !{i32 999990, i64 44, i32 191314}
!28 = !{i32 999999, i64 6, i32 216289}
!29 = !{!"function_entry_count", i64 193405}
!30 = !{!"branch_weights", i32 193405, i32 0}
!31 = !{!"branch_weights", i32 0, i32 193405}
```

It seems like this might be an issue with an earlier pass. It's just a verify call in LoopVectorizer that's tripping, and at the crash site we have the following IR:
```llvm
_ZNSt6vectorIN4llvm16NonLocalDepEntryESaIS1_EEC2ERKS3_.exit: ; preds = %if.end.i.i.nonchr, %entry.split.nonchr, %if.end.i.i, %entry.split
  %5 = phi i64 [ %shl.i.i.i, %if.end.i.i ], [ %shl.i.i.i, %entry.split ], [ %shl.i.i.i.nonchr, %if.end.i.i.nonchr ], [ %shl.i.i.i.nonchr, %entry.split.nonchr ]
  %6 = phi ptr [ %Worklist1, %if.end.i.i ], [ %Worklist1, %entry.split ], [ %Worklist1, %if.end.i.i.nonchr ], [ %Worklist1, %entry.split.nonchr ]
  store i64 %5, ptr %QueryInst, align 8
  call void @llvm.lifetime.start.p0(ptr nonnull %6)
  store ptr %6, ptr %this, align 8
  br label %for.body.i.i.i.i1401
```
The lifetime start annotation pointing at a phi node is not legal.

---

### Comment 4 - fhahn

The input to LV is already invalid, `-verify-each` points to ControlHeightReductionPass or earlier

https://llvm.godbolt.org/z/bdMMq6q5E

---

### Comment 5 - fhahn

Just extracting the IR before CHR doesn't seem to reproduce it, so possibly it gets broken earlier

---

### Comment 6 - boomanaiden154

Yeah, just saw that `ControlHeightReduction` is what introduces the invalid construct (from `-print-after-all`). Sorry for the noise about the loop vectorizer.

> Just extracting the IR before CHR doesn't seem to reproduce it, so possibly it gets broken earlier

Just ran into that too.

---

### Comment 7 - boomanaiden154

Running just `chr` in opt doesn't work, because `chr` expects a cached profile summary analysis to be available. Taking the following IR from directly before CHR in the pipeline:
```llvm
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i8:8:32-i16:16:32-i64:64-i128:128-n32:64-S128-Fn32"
target triple = "aarch64-unknown-linux-gnu"

%"class.llvm::SmallVector.136" = type { %"class.llvm::SmallVectorImpl.76", %"struct.llvm::SmallVectorStorage.137" }
%"class.llvm::SmallVectorImpl.76" = type { %"class.llvm::SmallVectorTemplateBase.77" }
%"class.llvm::SmallVectorTemplateBase.77" = type { %"class.llvm::SmallVectorTemplateCommon.78" }
%"class.llvm::SmallVectorTemplateCommon.78" = type { %"class.llvm::SmallVectorBase" }
%"class.llvm::SmallVectorBase" = type { ptr, i32, i32 }
%"struct.llvm::SmallVectorStorage.137" = type { [256 x i8] }

; Function Attrs: mustprogress nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare void @llvm.lifetime.start.p0(ptr captures(none)) #0

; Function Attrs: nofree norecurse nosync nounwind memory(argmem: write, inaccessiblemem: write)
define noundef i1 @_ZN4llvm23MemoryDependenceResults27getNonLocalPointerDepFromBBEPNS_11InstructionERKNS_12PHITransAddrERKNS_14MemoryLocationEbPNS_10BasicBlockERNS_15SmallVectorImplINS_17NonLocalDepResultEEERNS_13SmallDenseMapISA_PNS_5ValueELj16ENS_12DenseMapInfoISA_vEENS_6detail12DenseMapPairISA_SH_EEEEbb(ptr writeonly captures(address) initializes((0, 8)) %this, ptr writeonly captures(none) initializes((0, 8)) %QueryInst, i1 %isLoad, i1 %cmp.i.i.not.i.i) local_unnamed_addr #1 !prof !29 {
entry:
  %Worklist1 = alloca %"class.llvm::SmallVector.136", align 8
  %shl.i.i.i = select i1 %isLoad, i64 4, i64 0, !prof !30
  br i1 %cmp.i.i.not.i.i, label %_ZNSt6vectorIN4llvm16NonLocalDepEntryESaIS1_EEC2ERKS3_.exit, label %if.end.i.i, !prof !31

if.end.i.i:                                       ; preds = %entry
  tail call void @llvm.assume(i1 %isLoad)
  br label %_ZNSt6vectorIN4llvm16NonLocalDepEntryESaIS1_EEC2ERKS3_.exit

_ZNSt6vectorIN4llvm16NonLocalDepEntryESaIS1_EEC2ERKS3_.exit: ; preds = %if.end.i.i, %entry
  store i64 %shl.i.i.i, ptr %QueryInst, align 8
  call void @llvm.lifetime.start.p0(ptr nonnull %Worklist1)
  store ptr %Worklist1, ptr %this, align 8
  br label %for.body.i.i.i.i1401

for.cond.preheader.i.i.i:                         ; preds = %for.body.i.i.i.i1401
  ret i1 false

for.body.i.i.i.i1401:                             ; preds = %for.body.i.i.i.i1401, %_ZNSt6vectorIN4llvm16NonLocalDepEntryESaIS1_EEC2ERKS3_.exit
  %__begin0.0.i34.i.i.i = phi ptr [ %incdec.ptr.i.i.i.i1402, %for.body.i.i.i.i1401 ], [ null, %_ZNSt6vectorIN4llvm16NonLocalDepEntryESaIS1_EEC2ERKS3_.exit ]
  %incdec.ptr.i.i.i.i1402 = getelementptr i8, ptr %__begin0.0.i34.i.i.i, i64 24
  %cmp.not.i.i.i.i1403 = icmp eq ptr %__begin0.0.i34.i.i.i, %this
  br i1 %cmp.not.i.i.i.i1403, label %for.cond.preheader.i.i.i, label %for.body.i.i.i.i1401
}

; Function Attrs: mustprogress nocallback nofree nosync nounwind willreturn memory(inaccessiblemem: write)
declare void @llvm.assume(i1 noundef) #2

attributes #0 = { mustprogress nocallback nofree nosync nounwind willreturn memory(argmem: readwrite) }
attributes #1 = { nofree norecurse nosync nounwind memory(argmem: write, inaccessiblemem: write) }
attributes #2 = { mustprogress nocallback nofree nosync nounwind willreturn memory(inaccessiblemem: write) }

!llvm.module.flags = !{!0}

!0 = !{i32 1, !"ProfileSummary", !1}
!1 = !{!2, !3, !4, !5, !6, !7, !8, !9, !10, !11}
!2 = !{!"ProfileFormat", !"InstrProf"}
!3 = !{!"TotalCount", i64 51888332104}
!4 = !{!"MaxCount", i64 367187222}
!5 = !{!"MaxInternalCount", i64 367187222}
!6 = !{!"MaxFunctionCount", i64 281208867}
!7 = !{!"NumCounts", i64 1698950}
!8 = !{!"NumFunctions", i64 164155}
!9 = !{!"IsPartialProfile", i64 0}
!10 = !{!"PartialProfileRatio", double 0.000000e+00}
!11 = !{!"DetailedSummary", !12}
!12 = !{!13, !14, !15, !16, !17, !18, !19, !20, !21, !22, !23, !24, !25, !26, !27, !28}
!13 = !{i32 10000, i64 281208867, i32 2}
!14 = !{i32 100000, i64 88528372, i32 35}
!15 = !{i32 200000, i64 38263822, i32 128}
!16 = !{i32 300000, i64 20879095, i32 314}
!17 = !{i32 400000, i64 12121080, i32 643}
!18 = !{i32 500000, i64 6627144, i32 1239}
!19 = !{i32 600000, i64 3697961, i32 2312}
!20 = !{i32 700000, i64 2031812, i32 4214}
!21 = !{i32 800000, i64 988368, i32 7927}
!22 = !{i32 900000, i64 365146, i32 16522}
!23 = !{i32 950000, i64 149448, i32 27512}
!24 = !{i32 990000, i64 25291, i32 58500}
!25 = !{i32 999000, i64 2484, i32 108076}
!26 = !{i32 999900, i64 238, i32 156548}
!27 = !{i32 999990, i64 44, i32 191314}
!28 = !{i32 999999, i64 6, i32 216289}
!29 = !{!"function_entry_count", i64 193405}
!30 = !{!"branch_weights", i32 193405, i32 0}
!31 = !{!"branch_weights", i32 0, i32 193405}
```

And then running the following opt invocation:
```
opt - -passes="require<profile-summary>,chr" -disable-output
```
successfully reproduces the crash. Running that on the original IR rather than the IR right before CHR also reproduces the crash, so I don't think it's something earlier producing invalid IR.

It looks like this was caught by 92c55a315eab455d5fed2625fe0f61f88cb25499.



---

