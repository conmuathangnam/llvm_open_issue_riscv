# [AArch64]  Unable to widen vector load

**Author:** sjoerdmeijer
**URL:** https://github.com/llvm/llvm-project/issues/159995
**Status:** Closed
**Labels:** backend:AArch64, crash, llvm:SelectionDAG
**Closed Date:** 2025-10-02T11:24:25Z

## Body

The IR in this compiler explorer link

https://godbolt.org/z/ajc4Wf7z8

results in this error:

```
fatal error: error in backend: Unable to widen vector load
```

this is the backtrace:

```
 #0 0x0000aaaaae064dc0 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) ( /llvm-project/build/bin/clang+++0x35c4dc0)
 #1 0x0000aaaaae062b6c llvm::sys::RunSignalHandlers() ( /llvm-project/build/bin/clang+++0x35c2b6c)
 #2 0x0000aaaaadfe12e4 (anonymous namespace)::CrashRecoveryContextImpl::HandleCrash(int, unsigned long) CrashRecoveryContext.cpp:0:0
 #3 0x0000aaaaadfe1294 (anonymous namespace)::CrashRecoveryContextImpl::HandleCrash(int, unsigned long) CrashRecoveryContext.cpp:0:0
 #4 0x0000aaaaae05f930 llvm::sys::Process::Exit(int, bool) ( /llvm-project/build/bin/clang+++0x35bf930)
 #5 0x0000aaaaaceb5940 llvm::raw_ostream::operator<<(char const*) cc1_main.cpp:0:0
 #6 0x0000aaaaadfe6648 llvm::report_fatal_error(llvm::Twine const&, bool) ( /llvm-project/build/bin/clang+++0x3546648)
 #7 0x0000aaaaadfe651c llvm::report_fatal_error(llvm::Twine const&, bool) ( /llvm-project/build/bin/clang+++0x354651c)
 #8 0x0000aaaaaf795908 llvm::DAGTypeLegalizer::WidenVecRes_LOAD(llvm::SDNode*) LegalizeVectorTypes.cpp:0:0
 #9 0x0000aaaaaf792e3c llvm::DAGTypeLegalizer::WidenVectorResult(llvm::SDNode*, unsigned int) LegalizeVectorTypes.cpp:0:0
#10 0x0000aaaaaf74cb20 llvm::DAGTypeLegalizer::run() LegalizeTypes.cpp:0:0
#11 0x0000aaaaaf7507bc llvm::SelectionDAG::LegalizeTypes() ( /llvm-project/build/bin/clang+++0x4cb07bc)
```

I haven't reduced the IR input yet, but for completeness, this is the reproducer:

```
@a = local_unnamed_addr global i32 0, align 4

define void @b(ptr noundef readonly captures(none) %0, ptr noundef readonly captures(none) %1) local_unnamed_addr #0 {
  %3 = getelementptr inbounds nuw i8, ptr %1, i64 64
  %4 = getelementptr inbounds nuw i8, ptr %1, i64 128
  %5 = tail call i64 @llvm.vscale.i64()
  %6 = icmp samesign ugt i64 %5, 4
  br i1 %6, label %21, label %7

7:
  %8 = tail call i64 @llvm.vscale.i64()
  %9 = shl nuw nsw i64 %8, 3
  %10 = sub nsw i64 0, %9
  %11 = and i64 %10, 32
  br label %12

12:
  %13 = phi i64 [ 0, %7 ], [ %14, %12 ]
  %14 = add nuw i64 %13, %9
  %15 = icmp eq i64 %14, %11
  br i1 %15, label %16, label %12

16:
  %17 = or i64 %10, -35
  %18 = sub nuw nsw i64 35, %11
  %19 = tail call i64 @llvm.vscale.i64()
  %20 = icmp samesign ult i64 %18, %19
  br i1 %20, label %45, label %21

21:
  %22 = phi i64 [ %11, %16 ], [ 0, %2 ]
  %23 = tail call i64 @llvm.vscale.i64()
  %24 = sub nsw i64 0, %23
  %25 = and i64 %24, 35
  %26 = add nsw i64 %25, -35
  %27 = insertelement <vscale x 1 x ptr> poison, ptr %3, i64 0
  %28 = shufflevector <vscale x 1 x ptr> %27, <vscale x 1 x ptr> poison, <vscale x 1 x i32> zeroinitializer
  %29 = insertelement <vscale x 1 x ptr> poison, ptr %4, i64 0
  %30 = shufflevector <vscale x 1 x ptr> %29, <vscale x 1 x ptr> poison, <vscale x 1 x i32> zeroinitializer
  br label %31

31:
  %32 = phi i64 [ %22, %21 ], [ %33, %31 ]
  %33 = add nuw i64 %32, %23
  %34 = icmp eq i64 %33, %25
  br i1 %34, label %35, label %31

35:
  %36 = getelementptr i64, ptr %1, i64 %32
  %37 = getelementptr i8, ptr %36, i64 -280
  %38 = load <vscale x 1 x i64>, ptr %37, align 8
  %39 = icmp eq <vscale x 1 x i64> %38, zeroinitializer
  %40 = select <vscale x 1 x i1> %39, <vscale x 1 x ptr> %30, <vscale x 1 x ptr> %28
  %41 = tail call i32 @llvm.vscale.i32()
  %42 = add nsw i32 %41, -1
  %43 = extractelement <vscale x 1 x ptr> %40, i32 %42
  %44 = icmp eq i64 %25, 35
  br i1 %44, label %56, label %45

45:
  %46 = phi i64 [ %17, %16 ], [ %26, %35 ]
  br label %47

47:
  %48 = phi i64 [ %49, %47 ], [ %46, %45 ]
  %49 = add nsw i64 %48, 1
  %50 = icmp eq i64 %49, 0
  br i1 %50, label %51, label %47

51:
  %52 = getelementptr inbounds i64, ptr %1, i64 %48
  %53 = load i64, ptr %52, align 8
  %54 = icmp eq i64 %53, 0
  %55 = select i1 %54, ptr %4, ptr %3
  br label %56

56:
  %57 = phi ptr [ %43, %35 ], [ %55, %51 ]
  %58 = load i64, ptr %57, align 8
  %59 = tail call i64 @llvm.smax.i64(i64 %58, i64 0)
  %60 = trunc i64 %59 to i32
  store i32 %60, ptr @a, align 4
  ret void
}

declare i64 @llvm.smax.i64(i64, i64) #1

declare i64 @llvm.vscale.i64() #2

declare i32 @llvm.vscale.i32() #2

attributes #0 = { mustprogress nofree norecurse nosync nounwind memory(write, argmem: read, inaccessiblemem: none) uwtable vscale_range(1,16) "frame-pointer"="non-leaf" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="grace" "target-features"="+aes,+bf16,+bti,+ccidx,+complxnum,+crc,+dit,+dotprod,+ete,+flagm,+fp-armv8,+fp16fml,+fpac,+fullfp16,+i8mm,+jsconv,+lse,+mte,+neon,+outline-atomics,+pauth,+perfmon,+predres,+rand,+ras,+rcpc,+rdm,+sb,+sha2,+sha3,+sm4,+spe,+ssbs,+sve,+sve-aes,+sve-bitperm,+sve-sha3,+sve-sm4,+sve2,+trbe,+v8.1a,+v8.2a,+v8.3a,+v8.4a,+v8.5a,+v8a,+v9a,-fmv" }
attributes #1 = { nocallback nofree nosync nounwind speculatable willreturn memory(none) }
attributes #2 = { nocallback nofree nosync nounwind willreturn memory(none) }
```

## Comments

### Comment 1 - sjoerdmeijer

CC: @davemgreen, @david-arm, @paulwalker-arm, @sdesmalen-arm 

---

### Comment 2 - davemgreen

Hello. My understanding is that <vscale x 1 x ..> vectorization is not currently expected to work. It gets given an invalid cost so that the vectorizer should not be generating it. Do you know where this example comes from? It is only certain operations that get given an invalid cost, but load should be included.

---

### Comment 3 - llvmbot


@llvm/issue-subscribers-backend-aarch64

Author: Sjoerd Meijer (sjoerdmeijer)

<details>
The IR in this compiler explorer link

https://godbolt.org/z/ajc4Wf7z8

results in this error:

```
fatal error: error in backend: Unable to widen vector load
```

this is the backtrace:

```
 #<!-- -->0 0x0000aaaaae064dc0 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) ( /llvm-project/build/bin/clang+++0x35c4dc0)
 #<!-- -->1 0x0000aaaaae062b6c llvm::sys::RunSignalHandlers() ( /llvm-project/build/bin/clang+++0x35c2b6c)
 #<!-- -->2 0x0000aaaaadfe12e4 (anonymous namespace)::CrashRecoveryContextImpl::HandleCrash(int, unsigned long) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x0000aaaaadfe1294 (anonymous namespace)::CrashRecoveryContextImpl::HandleCrash(int, unsigned long) CrashRecoveryContext.cpp:0:0
 #<!-- -->4 0x0000aaaaae05f930 llvm::sys::Process::Exit(int, bool) ( /llvm-project/build/bin/clang+++0x35bf930)
 #<!-- -->5 0x0000aaaaaceb5940 llvm::raw_ostream::operator&lt;&lt;(char const*) cc1_main.cpp:0:0
 #<!-- -->6 0x0000aaaaadfe6648 llvm::report_fatal_error(llvm::Twine const&amp;, bool) ( /llvm-project/build/bin/clang+++0x3546648)
 #<!-- -->7 0x0000aaaaadfe651c llvm::report_fatal_error(llvm::Twine const&amp;, bool) ( /llvm-project/build/bin/clang+++0x354651c)
 #<!-- -->8 0x0000aaaaaf795908 llvm::DAGTypeLegalizer::WidenVecRes_LOAD(llvm::SDNode*) LegalizeVectorTypes.cpp:0:0
 #<!-- -->9 0x0000aaaaaf792e3c llvm::DAGTypeLegalizer::WidenVectorResult(llvm::SDNode*, unsigned int) LegalizeVectorTypes.cpp:0:0
#<!-- -->10 0x0000aaaaaf74cb20 llvm::DAGTypeLegalizer::run() LegalizeTypes.cpp:0:0
#<!-- -->11 0x0000aaaaaf7507bc llvm::SelectionDAG::LegalizeTypes() ( /llvm-project/build/bin/clang+++0x4cb07bc)
```

I haven't reduced the IR input yet, but for completeness, this is the reproducer:

```
@<!-- -->a = local_unnamed_addr global i32 0, align 4

define void @<!-- -->b(ptr noundef readonly captures(none) %0, ptr noundef readonly captures(none) %1) local_unnamed_addr #<!-- -->0 {
  %3 = getelementptr inbounds nuw i8, ptr %1, i64 64
  %4 = getelementptr inbounds nuw i8, ptr %1, i64 128
  %5 = tail call i64 @<!-- -->llvm.vscale.i64()
  %6 = icmp samesign ugt i64 %5, 4
  br i1 %6, label %21, label %7

7:
  %8 = tail call i64 @<!-- -->llvm.vscale.i64()
  %9 = shl nuw nsw i64 %8, 3
  %10 = sub nsw i64 0, %9
  %11 = and i64 %10, 32
  br label %12

12:
  %13 = phi i64 [ 0, %7 ], [ %14, %12 ]
  %14 = add nuw i64 %13, %9
  %15 = icmp eq i64 %14, %11
  br i1 %15, label %16, label %12

16:
  %17 = or i64 %10, -35
  %18 = sub nuw nsw i64 35, %11
  %19 = tail call i64 @<!-- -->llvm.vscale.i64()
  %20 = icmp samesign ult i64 %18, %19
  br i1 %20, label %45, label %21

21:
  %22 = phi i64 [ %11, %16 ], [ 0, %2 ]
  %23 = tail call i64 @<!-- -->llvm.vscale.i64()
  %24 = sub nsw i64 0, %23
  %25 = and i64 %24, 35
  %26 = add nsw i64 %25, -35
  %27 = insertelement &lt;vscale x 1 x ptr&gt; poison, ptr %3, i64 0
  %28 = shufflevector &lt;vscale x 1 x ptr&gt; %27, &lt;vscale x 1 x ptr&gt; poison, &lt;vscale x 1 x i32&gt; zeroinitializer
  %29 = insertelement &lt;vscale x 1 x ptr&gt; poison, ptr %4, i64 0
  %30 = shufflevector &lt;vscale x 1 x ptr&gt; %29, &lt;vscale x 1 x ptr&gt; poison, &lt;vscale x 1 x i32&gt; zeroinitializer
  br label %31

31:
  %32 = phi i64 [ %22, %21 ], [ %33, %31 ]
  %33 = add nuw i64 %32, %23
  %34 = icmp eq i64 %33, %25
  br i1 %34, label %35, label %31

35:
  %36 = getelementptr i64, ptr %1, i64 %32
  %37 = getelementptr i8, ptr %36, i64 -280
  %38 = load &lt;vscale x 1 x i64&gt;, ptr %37, align 8
  %39 = icmp eq &lt;vscale x 1 x i64&gt; %38, zeroinitializer
  %40 = select &lt;vscale x 1 x i1&gt; %39, &lt;vscale x 1 x ptr&gt; %30, &lt;vscale x 1 x ptr&gt; %28
  %41 = tail call i32 @<!-- -->llvm.vscale.i32()
  %42 = add nsw i32 %41, -1
  %43 = extractelement &lt;vscale x 1 x ptr&gt; %40, i32 %42
  %44 = icmp eq i64 %25, 35
  br i1 %44, label %56, label %45

45:
  %46 = phi i64 [ %17, %16 ], [ %26, %35 ]
  br label %47

47:
  %48 = phi i64 [ %49, %47 ], [ %46, %45 ]
  %49 = add nsw i64 %48, 1
  %50 = icmp eq i64 %49, 0
  br i1 %50, label %51, label %47

51:
  %52 = getelementptr inbounds i64, ptr %1, i64 %48
  %53 = load i64, ptr %52, align 8
  %54 = icmp eq i64 %53, 0
  %55 = select i1 %54, ptr %4, ptr %3
  br label %56

56:
  %57 = phi ptr [ %43, %35 ], [ %55, %51 ]
  %58 = load i64, ptr %57, align 8
  %59 = tail call i64 @<!-- -->llvm.smax.i64(i64 %58, i64 0)
  %60 = trunc i64 %59 to i32
  store i32 %60, ptr @<!-- -->a, align 4
  ret void
}

declare i64 @<!-- -->llvm.smax.i64(i64, i64) #<!-- -->1

declare i64 @<!-- -->llvm.vscale.i64() #<!-- -->2

declare i32 @<!-- -->llvm.vscale.i32() #<!-- -->2

attributes #<!-- -->0 = { mustprogress nofree norecurse nosync nounwind memory(write, argmem: read, inaccessiblemem: none) uwtable vscale_range(1,16) "frame-pointer"="non-leaf" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="grace" "target-features"="+aes,+bf16,+bti,+ccidx,+complxnum,+crc,+dit,+dotprod,+ete,+flagm,+fp-armv8,+fp16fml,+fpac,+fullfp16,+i8mm,+jsconv,+lse,+mte,+neon,+outline-atomics,+pauth,+perfmon,+predres,+rand,+ras,+rcpc,+rdm,+sb,+sha2,+sha3,+sm4,+spe,+ssbs,+sve,+sve-aes,+sve-bitperm,+sve-sha3,+sve-sm4,+sve2,+trbe,+v8.1a,+v8.2a,+v8.3a,+v8.4a,+v8.5a,+v8a,+v9a,-fmv" }
attributes #<!-- -->1 = { nocallback nofree nosync nounwind speculatable willreturn memory(none) }
attributes #<!-- -->2 = { nocallback nofree nosync nounwind willreturn memory(none) }
```
</details>


---

### Comment 4 - sjoerdmeijer

> Hello. My understanding is that <vscale x 1 x ..> vectorization is not currently expected to work. It gets given an invalid cost so that the vectorizer should not be generating it. Do you know where this example comes from? It is only certain operations that get given an invalid cost, but load should be included.

Thanks for the analysis. The IR was generated from source code, so it's that this cannot be triggered from C++. 
Usually I clean up my test directories after upstreaming a bug, but I will see if I can find it, or if I have another example because this bug was triggered a couple of times if I remember that well.

---

### Comment 5 - sjoerdmeijer

I deleted the the C++ test case. But I can start testing again, I will probably hit this again if we need a C++ test case. 

---

### Comment 6 - paulwalker-arm

This has been on my TODO list to fix for a while but was dependent on other codegen work. This landed a month or so back so I'll take another run at it.

---

### Comment 7 - sjoerdmeijer

Great, thanks!

---

