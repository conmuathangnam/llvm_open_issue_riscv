# SimplifyCFG removes return statement

**Author:** ilovepi
**URL:** https://github.com/llvm/llvm-project/issues/118855
**Status:** Open
**Labels:** miscompilation, llvm:SCEV, llvm:transforms

## Body

We're seeing a miscompile from Rust, but the IR seems valid, and the bug only occurs while lowering LLVM IR.

I've somewhat reduced the case down, but I haven't run llvm-reduce over it yet.

The folowing diff shows that after simplifycfg we drop the return branch altogether.

```diff
❯ diff /tmp/lto.small.ll /tmp/lto.small.bad.strip.ll
1c1
< ; ModuleID = '/tmp/lto.strip.ll'
---
> ; ModuleID = '/tmp/lto.small.ll'
17c17
<   br label %4
---
>   br label %3
19,29c19,23
< 3:                                                ; preds = %4
<   ret void
<
< 4:                                                ; preds = %6, %0
<   %5 = phi i8 [ %7, %6 ], [ -3, %0 ]
<   br i1 false, label %3, label %6
<
< 6:                                                ; preds = %4
<   %7 = add nuw nsw i8 %5, 1
<   call fastcc void @_RNvMCs7vJBeK7brjY_8lto_hangNtB2_4Test3add(ptr noalias noundef nonnull align 8 dereferenceable(24) %1, i8 noundef %5) #4
<   br label %4
---
> 3:                                                ; preds = %3, %0
>   %4 = phi i8 [ %5, %3 ], [ -3, %0 ]
>   %5 = add nuw nsw i8 %4, 1
>   call fastcc void @_RNvMCs7vJBeK7brjY_8lto_hangNtB2_4Test3add(ptr noalias noundef nonnull align 8 dereferenceable(24) %1, i8 noundef %4) #4
>   br label %3
```

For completeness I'm including the IR before and after simplifycfg:

**Before simplifycfg**
```llvm
; ModuleID = '/tmp/lto.strip.ll'
source_filename = "lto_hang.577eccc79634c19c-cgu.0"
target datalayout = "e-m:e-i8:8:32-i16:16:32-i64:64-i128:128-n32:64-S128-Fn32"
target triple = "aarch64-unknown-fuchsia"

; Function Attrs: nounwind optsize shadowcallstack uwtable
declare hidden fastcc void @_RNvMCs7vJBeK7brjY_8lto_hangNtB2_4Test3add(ptr noalias nocapture noundef align 8 dereferenceable(24), i8 noundef) unnamed_addr #0

; Function Attrs: noinline nounwind optsize shadowcallstack uwtable
define hidden fastcc void @_RNvCs7vJBeK7brjY_8lto_hang11do_the_test() unnamed_addr #1 {
  %1 = alloca [24 x i8], align 8
  call void @llvm.lifetime.start.p0(i64 24, ptr nonnull %1)
  %2 = getelementptr inbounds i8, ptr %1, i64 16
  store i16 -32640, ptr %2, align 8, !alias.scope !6
  call void @llvm.memset.p0.i64(ptr noundef nonnull align 8 dereferenceable(16) %1, i8 0, i64 16, i1 false), !alias.scope !6
  call fastcc void @_RNvMCs7vJBeK7brjY_8lto_hangNtB2_4Test3add(ptr noalias noundef nonnull align 8 dereferenceable(24) %1, i8 noundef -4) #4
  br label %4

3:                                                ; preds = %4
  ret void

4:                                                ; preds = %6, %0
  %5 = phi i8 [ %7, %6 ], [ -3, %0 ]
  br i1 false, label %3, label %6

6:                                                ; preds = %4
  %7 = add nuw nsw i8 %5, 1
  call fastcc void @_RNvMCs7vJBeK7brjY_8lto_hangNtB2_4Test3add(ptr noalias noundef nonnull align 8 dereferenceable(24) %1, i8 noundef %5) #4
  br label %4
}

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: write)
declare void @llvm.memset.p0.i64(ptr nocapture writeonly, i8, i64, i1 immarg) #2

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare void @llvm.lifetime.start.p0(i64 immarg, ptr nocapture) #3

attributes #0 = { nounwind optsize shadowcallstack uwtable "frame-pointer"="all" "probe-stack"="inline-asm" "target-cpu"="generic" "target-features"="+v8a,+crc,+aes,+neon,+fp-armv8,+neon,+fp-armv8,+sha2" }
attributes #1 = { noinline nounwind optsize shadowcallstack uwtable "frame-pointer"="all" "probe-stack"="inline-asm" "target-cpu"="generic" "target-features"="+v8a,+crc,+aes,+neon,+fp-armv8,+neon,+fp-armv8,+sha2" }
attributes #2 = { nocallback nofree nounwind willreturn memory(argmem: write) }
attributes #3 = { nocallback nofree nosync nounwind willreturn memory(argmem: readwrite) }
attributes #4 = { nounwind }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 4, !"EnableSplitLTOUnit", i32 1}
!3 = !{i32 2, !"Dwarf Version", i32 4}
!4 = !{i32 2, !"Debug Info Version", i32 3}
!5 = !{!"rustc version 1.84.0-nightly (583b25d8d 2024-11-12)"}
!6 = !{!7}
!7 = distinct !{!7, !8, !"_RNvMCs7vJBeK7brjY_8lto_hangNtB2_4Test3new: argument 0"}
!8 = distinct !{!8, !"_RNvMCs7vJBeK7brjY_8lto_hangNtB2_4Test3new"}
````

**After simplifycfg:**
```llvm
; ModuleID = '/tmp/lto.small.ll'
source_filename = "lto_hang.577eccc79634c19c-cgu.0"
target datalayout = "e-m:e-i8:8:32-i16:16:32-i64:64-i128:128-n32:64-S128-Fn32"
target triple = "aarch64-unknown-fuchsia"

; Function Attrs: nounwind optsize shadowcallstack uwtable
declare hidden fastcc void @_RNvMCs7vJBeK7brjY_8lto_hangNtB2_4Test3add(ptr noalias nocapture noundef align 8 dereferenceable(24), i8 noundef) unnamed_addr #0

; Function Attrs: noinline nounwind optsize shadowcallstack uwtable
define hidden fastcc void @_RNvCs7vJBeK7brjY_8lto_hang11do_the_test() unnamed_addr #1 {
  %1 = alloca [24 x i8], align 8
  call void @llvm.lifetime.start.p0(i64 24, ptr nonnull %1)
  %2 = getelementptr inbounds i8, ptr %1, i64 16
  store i16 -32640, ptr %2, align 8, !alias.scope !6
  call void @llvm.memset.p0.i64(ptr noundef nonnull align 8 dereferenceable(16) %1, i8 0, i64 16, i1 false), !alias.scope !6
  call fastcc void @_RNvMCs7vJBeK7brjY_8lto_hangNtB2_4Test3add(ptr noalias noundef nonnull align 8 dereferenceable(24) %1, i8 noundef -4) #4
  br label %3

3:                                                ; preds = %3, %0
  %4 = phi i8 [ %5, %3 ], [ -3, %0 ]
  %5 = add nuw nsw i8 %4, 1
  call fastcc void @_RNvMCs7vJBeK7brjY_8lto_hangNtB2_4Test3add(ptr noalias noundef nonnull align 8 dereferenceable(24) %1, i8 noundef %4) #4
  br label %3
}

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: write)
declare void @llvm.memset.p0.i64(ptr nocapture writeonly, i8, i64, i1 immarg) #2

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare void @llvm.lifetime.start.p0(i64 immarg, ptr nocapture) #3

attributes #0 = { nounwind optsize shadowcallstack uwtable "frame-pointer"="all" "probe-stack"="inline-asm" "target-cpu"="generic" "target-features"="+v8a,+crc,+aes,+neon,+fp-armv8,+neon,+fp-armv8,+sha2" }
attributes #1 = { noinline nounwind optsize shadowcallstack uwtable "frame-pointer"="all" "probe-stack"="inline-asm" "target-cpu"="generic" "target-features"="+v8a,+crc,+aes,+neon,+fp-armv8,+neon,+fp-armv8,+sha2" }
attributes #2 = { nocallback nofree nounwind willreturn memory(argmem: write) }
attributes #3 = { nocallback nofree nosync nounwind willreturn memory(argmem: readwrite) }
attributes #4 = { nounwind }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 4, !"EnableSplitLTOUnit", i32 1}
!3 = !{i32 2, !"Dwarf Version", i32 4}
!4 = !{i32 2, !"Debug Info Version", i32 3}
!5 = !{!"rustc version 1.84.0-nightly (583b25d8d 2024-11-12)"}
!6 = !{!7}
!7 = distinct !{!7, !8, !"_RNvMCs7vJBeK7brjY_8lto_hangNtB2_4Test3new: argument 0"}
!8 = distinct !{!8, !"_RNvMCs7vJBeK7brjY_8lto_hangNtB2_4Test3new"}
```

The after module was generated w/ `opt -passes=simplifycfg < before.ll -S`

Unfortunately, this issue originates from Rust (Fuchsia's toolchain, which uses a different cadence for release process than upstream), and it isn't clear when this behavior was introduced in the LLVM used by `rustc`, so we'll probably need to bisect quite a ways back. I did reproduce this with ToT LLVM, though, so its still an issue on the LLVM side.

If no one has a thought on the underlying issue, I'll pick up the investigation again tomorrow, reduce the test further, and try to kick of a bisect to run this down a bit more.

CC: @nikic @tmandry

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-bug

Author: Paul Kirth (ilovepi)

<details>
We're seeing a miscompile from Rust, but the IR seems valid, and the bug only occurs while lowering LLVM IR.

I've somewhat reduced the case down, but I haven't run llvm-reduce over it yet.

The folowing diff shows that after simplifycfg we drop the return branch altogether.

```diff
❯ diff /tmp/lto.small.ll /tmp/lto.small.bad.strip.ll
1c1
&lt; ; ModuleID = '/tmp/lto.strip.ll'
---
&gt; ; ModuleID = '/tmp/lto.small.ll'
17c17
&lt;   br label %4
---
&gt;   br label %3
19,29c19,23
&lt; 3:                                                ; preds = %4
&lt;   ret void
&lt;
&lt; 4:                                                ; preds = %6, %0
&lt;   %5 = phi i8 [ %7, %6 ], [ -3, %0 ]
&lt;   br i1 false, label %3, label %6
&lt;
&lt; 6:                                                ; preds = %4
&lt;   %7 = add nuw nsw i8 %5, 1
&lt;   call fastcc void @<!-- -->_RNvMCs7vJBeK7brjY_8lto_hangNtB2_4Test3add(ptr noalias noundef nonnull align 8 dereferenceable(24) %1, i8 noundef %5) #<!-- -->4
&lt;   br label %4
---
&gt; 3:                                                ; preds = %3, %0
&gt;   %4 = phi i8 [ %5, %3 ], [ -3, %0 ]
&gt;   %5 = add nuw nsw i8 %4, 1
&gt;   call fastcc void @<!-- -->_RNvMCs7vJBeK7brjY_8lto_hangNtB2_4Test3add(ptr noalias noundef nonnull align 8 dereferenceable(24) %1, i8 noundef %4) #<!-- -->4
&gt;   br label %3
```

For completeness I'm including the IR before and after simplifycfg:

**Before simplifycfg**
```
; ModuleID = '/tmp/lto.strip.ll'
source_filename = "lto_hang.577eccc79634c19c-cgu.0"
target datalayout = "e-m:e-i8:8:32-i16:16:32-i64:64-i128:128-n32:64-S128-Fn32"
target triple = "aarch64-unknown-fuchsia"

; Function Attrs: nounwind optsize shadowcallstack uwtable
declare hidden fastcc void @<!-- -->_RNvMCs7vJBeK7brjY_8lto_hangNtB2_4Test3add(ptr noalias nocapture noundef align 8 dereferenceable(24), i8 noundef) unnamed_addr #<!-- -->0

; Function Attrs: noinline nounwind optsize shadowcallstack uwtable
define hidden fastcc void @<!-- -->_RNvCs7vJBeK7brjY_8lto_hang11do_the_test() unnamed_addr #<!-- -->1 {
  %1 = alloca [24 x i8], align 8
  call void @<!-- -->llvm.lifetime.start.p0(i64 24, ptr nonnull %1)
  %2 = getelementptr inbounds i8, ptr %1, i64 16
  store i16 -32640, ptr %2, align 8, !alias.scope !6
  call void @<!-- -->llvm.memset.p0.i64(ptr noundef nonnull align 8 dereferenceable(16) %1, i8 0, i64 16, i1 false), !alias.scope !6
  call fastcc void @<!-- -->_RNvMCs7vJBeK7brjY_8lto_hangNtB2_4Test3add(ptr noalias noundef nonnull align 8 dereferenceable(24) %1, i8 noundef -4) #<!-- -->4
  br label %4

3:                                                ; preds = %4
  ret void

4:                                                ; preds = %6, %0
  %5 = phi i8 [ %7, %6 ], [ -3, %0 ]
  br i1 false, label %3, label %6

6:                                                ; preds = %4
  %7 = add nuw nsw i8 %5, 1
  call fastcc void @<!-- -->_RNvMCs7vJBeK7brjY_8lto_hangNtB2_4Test3add(ptr noalias noundef nonnull align 8 dereferenceable(24) %1, i8 noundef %5) #<!-- -->4
  br label %4
}

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: write)
declare void @<!-- -->llvm.memset.p0.i64(ptr nocapture writeonly, i8, i64, i1 immarg) #<!-- -->2

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare void @<!-- -->llvm.lifetime.start.p0(i64 immarg, ptr nocapture) #<!-- -->3

attributes #<!-- -->0 = { nounwind optsize shadowcallstack uwtable "frame-pointer"="all" "probe-stack"="inline-asm" "target-cpu"="generic" "target-features"="+v8a,+crc,+aes,+neon,+fp-armv8,+neon,+fp-armv8,+sha2" }
attributes #<!-- -->1 = { noinline nounwind optsize shadowcallstack uwtable "frame-pointer"="all" "probe-stack"="inline-asm" "target-cpu"="generic" "target-features"="+v8a,+crc,+aes,+neon,+fp-armv8,+neon,+fp-armv8,+sha2" }
attributes #<!-- -->2 = { nocallback nofree nounwind willreturn memory(argmem: write) }
attributes #<!-- -->3 = { nocallback nofree nosync nounwind willreturn memory(argmem: readwrite) }
attributes #<!-- -->4 = { nounwind }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 4, !"EnableSplitLTOUnit", i32 1}
!3 = !{i32 2, !"Dwarf Version", i32 4}
!4 = !{i32 2, !"Debug Info Version", i32 3}
!5 = !{!"rustc version 1.84.0-nightly (583b25d8d 2024-11-12)"}
!6 = !{!7}
!7 = distinct !{!7, !8, !"_RNvMCs7vJBeK7brjY_8lto_hangNtB2_4Test3new: argument 0"}
!8 = distinct !{!8, !"_RNvMCs7vJBeK7brjY_8lto_hangNtB2_4Test3new"}
````

**After simplifycfg:**
```
; ModuleID = '/tmp/lto.small.ll'
source_filename = "lto_hang.577eccc79634c19c-cgu.0"
target datalayout = "e-m:e-i8:8:32-i16:16:32-i64:64-i128:128-n32:64-S128-Fn32"
target triple = "aarch64-unknown-fuchsia"

; Function Attrs: nounwind optsize shadowcallstack uwtable
declare hidden fastcc void @<!-- -->_RNvMCs7vJBeK7brjY_8lto_hangNtB2_4Test3add(ptr noalias nocapture noundef align 8 dereferenceable(24), i8 noundef) unnamed_addr #<!-- -->0

; Function Attrs: noinline nounwind optsize shadowcallstack uwtable
define hidden fastcc void @<!-- -->_RNvCs7vJBeK7brjY_8lto_hang11do_the_test() unnamed_addr #<!-- -->1 {
  %1 = alloca [24 x i8], align 8
  call void @<!-- -->llvm.lifetime.start.p0(i64 24, ptr nonnull %1)
  %2 = getelementptr inbounds i8, ptr %1, i64 16
  store i16 -32640, ptr %2, align 8, !alias.scope !6
  call void @<!-- -->llvm.memset.p0.i64(ptr noundef nonnull align 8 dereferenceable(16) %1, i8 0, i64 16, i1 false), !alias.scope !6
  call fastcc void @<!-- -->_RNvMCs7vJBeK7brjY_8lto_hangNtB2_4Test3add(ptr noalias noundef nonnull align 8 dereferenceable(24) %1, i8 noundef -4) #<!-- -->4
  br label %3

3:                                                ; preds = %3, %0
  %4 = phi i8 [ %5, %3 ], [ -3, %0 ]
  %5 = add nuw nsw i8 %4, 1
  call fastcc void @<!-- -->_RNvMCs7vJBeK7brjY_8lto_hangNtB2_4Test3add(ptr noalias noundef nonnull align 8 dereferenceable(24) %1, i8 noundef %4) #<!-- -->4
  br label %3
}

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: write)
declare void @<!-- -->llvm.memset.p0.i64(ptr nocapture writeonly, i8, i64, i1 immarg) #<!-- -->2

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare void @<!-- -->llvm.lifetime.start.p0(i64 immarg, ptr nocapture) #<!-- -->3

attributes #<!-- -->0 = { nounwind optsize shadowcallstack uwtable "frame-pointer"="all" "probe-stack"="inline-asm" "target-cpu"="generic" "target-features"="+v8a,+crc,+aes,+neon,+fp-armv8,+neon,+fp-armv8,+sha2" }
attributes #<!-- -->1 = { noinline nounwind optsize shadowcallstack uwtable "frame-pointer"="all" "probe-stack"="inline-asm" "target-cpu"="generic" "target-features"="+v8a,+crc,+aes,+neon,+fp-armv8,+neon,+fp-armv8,+sha2" }
attributes #<!-- -->2 = { nocallback nofree nounwind willreturn memory(argmem: write) }
attributes #<!-- -->3 = { nocallback nofree nosync nounwind willreturn memory(argmem: readwrite) }
attributes #<!-- -->4 = { nounwind }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 4, !"EnableSplitLTOUnit", i32 1}
!3 = !{i32 2, !"Dwarf Version", i32 4}
!4 = !{i32 2, !"Debug Info Version", i32 3}
!5 = !{!"rustc version 1.84.0-nightly (583b25d8d 2024-11-12)"}
!6 = !{!7}
!7 = distinct !{!7, !8, !"_RNvMCs7vJBeK7brjY_8lto_hangNtB2_4Test3new: argument 0"}
!8 = distinct !{!8, !"_RNvMCs7vJBeK7brjY_8lto_hangNtB2_4Test3new"}
```

The after module was generated w/ `opt -passes=simplifycfg &lt; before.ll -S`

Unfortunately, this issue originates from Rust (Fuchsia's toolchain, which uses a different cadence for release process than upstream), and it isn't clear when this behavior was introduced in the LLVM used by `rustc`, so we'll probably need to bisect quite a ways back. I did reproduce this with ToT LLVM, though, so its still an issue on the LLVM side.

If no one has a thought on the underlying issue, I'll pick up the investigation again tomorrow, reduce the test further, and try to kick of a bisect to run this down a bit more.

CC: @<!-- -->nikic @<!-- -->tmandry
</details>


---

### Comment 2 - efriedma-quic

The ret is already dead in the "before" IR: the only predecessor is `br i1 false, label %3, label %6`, and that obvious can't branch to `%3`.

---

### Comment 3 - ilovepi

Ah, you're right 🤦 . The underlying problem seems to have happened earlier. I'll try to walk back through the transforms to see when that condition changed. 

---

### Comment 4 - ilovepi

This is probably more helpful that the previous attempt. I've included a zip file w/ a number of intermediate artifacts. From what I can tell so far, it seems that the branch is made dead by IndvarSimplify. That seems wrong to me, since the loop should be well defined in the source. The input Rust code is very simple https://fuchsia-review.git.corp.google.com/c/fuchsia/+/1164954, and I don't think has any UB. Nothing in the initial IR looks wrong to me offhand. That said, as pointed out above,  its easy to miss a detail, so maybe the IR coming out of the frontend is subtly wrong. If it is, it isn't obvious to me.

The reproducer zip has a few items in it. 

- The initial module before any passes: `initial.ll`
- A script (`repro.sh`) that highlights the problematic transforms from IndvarSimplify -> Instcombine -> SimplifyCFG
- The intermediate IR at each step.

[repro.zip](https://github.com/user-attachments/files/18030965/repro.zip)

Note that I used `llvm-extract` to isolate the single function after scanning through the output of `--print-after-all` with the full module context, which is how I got the IR in `before.indvars.0.ll`. 

I'm not sure how to get `rustc` to emit the equivalent of `--print-pipeline-passes`, but I'm fairly certain there is something different w/ the pipeline used, since I can't get it to trigger w/ the `default<O{s,2,3}>` or `lto<O{s,2,3}>` pipelines in opt.




---

### Comment 5 - efriedma-quic

Take the following testcase:

```
define void @g() {
  ret void
}

define i32 @main() {
  call void @f()
  ret i32 0
}

define void @f() {
  br label %2

1:                                                ; preds = %2
  ret void

2:                                                ; preds = %6, %0
  %3 = phi i8 [ %7, %6 ], [ -3, %0 ]
  %4 = phi i8 [ %3, %6 ], [ -4, %0 ]
  %5 = icmp eq i8 %4, -1
  br i1 %5, label %1, label %6

6:                                                ; preds = %2
  %7 = add nuw nsw i8 %3, 1
  call void @g()
  br label %2
}
```

`opt "-passes=print<scalar-evolution>"` gives:

```
Classifying expressions for: @f
  %3 = phi i8 [ %7, %6 ], [ -3, %0 ]
  -->  {-3,+,1}<nuw><nsw><%2> U: [-3,0) S: [-3,0)               Exits: 0                LoopDispositions: { %2: Computable }
  %4 = phi i8 [ %3, %6 ], [ -4, %0 ]
  -->  {-4,+,1}<nsw><%2> U: [-4,0) S: [-4,0)            Exits: -1               LoopDispositions: { %2: Computable }
  %7 = add nuw nsw i8 %3, 1
  -->  {-2,+,1}<nsw><%2> U: [-2,2) S: [-2,2)            Exits: 1                LoopDispositions: { %2: Computable }
```

I think we're incorrectly inferring nuw on the AddRec.  (%3 is poison on the last iteration, but I don't think that's enough to infer nuw.)  Then I think indvars rewrites the comparison based on that nuw marking.

---

### Comment 6 - nikic

I think this is the issue I tried to fix in https://reviews.llvm.org/D148931. SCEV currently unconditionally transfers nowrap flags to pre-inc addrecs, which is not correct in general (if the nowrap violation is not guaranteed to cause UB).

---

### Comment 7 - nikic

I confirmed that https://github.com/llvm/llvm-project/pull/118959 does fix the issue, but it also causes a lot of regressions, mainly in IV widening. I've been triaging the diffs on llvm-opt-benchmark, and some of them are recoverable e.g. by improving programUndefinedIfPoison to handle control flow triangles/diamonds, but many aren't.

I think we need https://github.com/llvm/llvm-project/pull/91961 to fix this without significant regressions.

---

