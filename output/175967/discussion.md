# [SLPVectorizer] Crash after 1a8f5fa823a55bce9be368097d2ac7e137fe5d17

**Author:** boomanaiden154
**URL:** https://github.com/llvm/llvm-project/issues/175967
**Status:** Closed
**Labels:** release:backport, llvm:SLPVectorizer, crash
**Closed Date:** 2026-01-15T11:51:10Z

## Body

Looking at the following IR:
```
; ModuleID = './reduced.ll'
source_filename = "./reduced.ll"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-grtev4-linux-gnu"

define fastcc void @wombat(i32 %arg) #0 {
bb:
  %zext = zext i32 %arg to i64
  %sext = sext i32 0 to i64
  %select = select i1 false, i32 0, i32 0
  %zext1 = zext i32 %select to i64
  %mul = mul i64 0, %zext1
  %or = or i64 0, %mul
  %or2 = or i64 %or, 0
  %zext3 = zext i32 0 to i64
  %mul4 = mul i64 0, %zext3
  %or5 = or i64 %or2, %mul4
  %lshr = lshr i64 %or5, 0
  %trunc = trunc i64 %lshr to i32
  %call = tail call i32 @llvm.smax.i32(i32 %trunc, i32 0)
  %call6 = tail call i32 @llvm.smin.i32(i32 %call, i32 0)
  %trunc7 = trunc i32 %call6 to i16
  %getelementptr = getelementptr i8, ptr poison, i64 16
  store i16 %trunc7, ptr %getelementptr, align 2
  %mul8 = mul i64 0, %zext
  %or9 = or i64 0, %mul8
  %or10 = or i64 %or9, 0
  %sext11 = sext i32 0 to i64
  %mul12 = mul i64 0, %sext11
  %or13 = or i64 %or10, %mul12
  %lshr14 = lshr i64 %or13, 0
  %trunc15 = trunc i64 %lshr14 to i32
  %call16 = tail call i32 @llvm.smax.i32(i32 %trunc15, i32 0)
  %call17 = tail call i32 @llvm.smin.i32(i32 %call16, i32 0)
  %trunc18 = trunc i32 %call17 to i16
  %getelementptr19 = getelementptr i8, ptr poison, i64 18
  store i16 %trunc18, ptr %getelementptr19, align 2
  %sext20 = sext i32 0 to i64
  %mul21 = mul i64 0, %sext20
  %or22 = or i64 %zext3, %mul21
  %or23 = or i64 %or22, 0
  %mul24 = mul i64 0, %zext1
  %or25 = or i64 %or23, %mul24
  %lshr26 = lshr i64 %or25, 0
  %trunc27 = trunc i64 %lshr26 to i32
  %call28 = tail call i32 @llvm.smax.i32(i32 %trunc27, i32 0)
  %call29 = tail call i32 @llvm.smin.i32(i32 %call28, i32 0)
  %trunc30 = trunc i32 %call29 to i16
  %getelementptr31 = getelementptr i8, ptr poison, i64 20
  store i16 %trunc30, ptr %getelementptr31, align 2
  %sext32 = sext i32 0 to i64
  %mul33 = mul i64 0, %sext32
  %or34 = or i64 0, %mul33
  %or35 = or i64 %or34, 0
  %mul36 = mul i64 0, %sext
  %or37 = or i64 %or35, %mul36
  %lshr38 = lshr i64 %or37, 0
  %trunc39 = trunc i64 %lshr38 to i32
  %call40 = tail call i32 @llvm.smax.i32(i32 %trunc39, i32 0)
  %call41 = tail call i32 @llvm.smin.i32(i32 %call40, i32 0)
  %trunc42 = trunc i32 %call41 to i16
  %getelementptr43 = getelementptr i8, ptr poison, i64 22
  store i16 %trunc42, ptr %getelementptr43, align 2
  %mul44 = mul i64 0, %zext1
  %or45 = or i64 0, %mul44
  %or46 = or i64 %or45, 0
  %mul47 = mul i64 0, %zext3
  %or48 = or i64 %or46, %mul47
  %lshr49 = lshr i64 %or48, 0
  %trunc50 = trunc i64 %lshr49 to i32
  %call51 = tail call i32 @llvm.smax.i32(i32 %trunc50, i32 0)
  %call52 = tail call i32 @llvm.smin.i32(i32 %call51, i32 0)
  %trunc53 = trunc i32 %call52 to i16
  %getelementptr54 = getelementptr i8, ptr poison, i64 24
  store i16 %trunc53, ptr %getelementptr54, align 2
  %mul55 = mul i64 0, %zext
  %or56 = or i64 0, %mul55
  %or57 = or i64 %or56, 0
  %mul58 = mul i64 0, %sext11
  %or59 = or i64 %or57, %mul58
  %lshr60 = lshr i64 %or59, 0
  %trunc61 = trunc i64 %lshr60 to i32
  %call62 = tail call i32 @llvm.smax.i32(i32 %trunc61, i32 0)
  %call63 = tail call i32 @llvm.smin.i32(i32 %call62, i32 0)
  %trunc64 = trunc i32 %call63 to i16
  %getelementptr65 = getelementptr i8, ptr poison, i64 26
  store i16 %trunc64, ptr %getelementptr65, align 2
  %mul66 = mul i64 0, %sext20
  %or67 = or i64 0, %mul66
  %or68 = or i64 %or67, 0
  %mul69 = mul i64 0, %zext1
  %or70 = or i64 %or68, %mul69
  %lshr71 = lshr i64 %or70, 0
  %trunc72 = trunc i64 %lshr71 to i32
  %call73 = tail call i32 @llvm.smax.i32(i32 %trunc72, i32 0)
  %call74 = tail call i32 @llvm.smin.i32(i32 %call73, i32 0)
  %trunc75 = trunc i32 %call74 to i16
  %getelementptr76 = getelementptr i8, ptr poison, i64 28
  store i16 %trunc75, ptr %getelementptr76, align 2
  %mul77 = mul i64 0, %sext32
  %or78 = or i64 0, %mul77
  %or79 = or i64 %or78, 0
  %mul80 = mul i64 0, %sext
  %or81 = or i64 %or79, %mul80
  %lshr82 = lshr i64 %or81, 0
  %trunc83 = trunc i64 %lshr82 to i32
  %call84 = tail call i32 @llvm.smax.i32(i32 %trunc83, i32 0)
  %call85 = tail call i32 @llvm.smin.i32(i32 %call84, i32 0)
  %trunc86 = trunc i32 %call85 to i16
  %getelementptr87 = getelementptr i8, ptr poison, i64 30
  store i16 %trunc86, ptr %getelementptr87, align 2
  ret void
}

; Function Attrs: nocallback nocreateundeforpoison nofree nosync nounwind speculatable willreturn memory(none)
declare i32 @llvm.smin.i32(i32, i32) #1

; Function Attrs: nocallback nocreateundeforpoison nofree nosync nounwind speculatable willreturn memory(none)
declare i32 @llvm.smax.i32(i32, i32) #1

attributes #0 = { "target-cpu"="haswell" }
attributes #1 = { nocallback nocreateundeforpoison nofree nosync nounwind speculatable willreturn memory(none) }
```

and running `opt`:
```
./bin/opt -passes="slp-vectorizer,verify" ./reduced.ll -disable-output
```

we're seeing the following error after the commit in the title:
```
Instruction does not dominate all uses!
  %6 = zext <2 x i32> %5 to <2 x i64>
  %0 = shufflevector <2 x i64> %6, <2 x i64> poison, <4 x i32> <i32 1, i32 poison, i32 poison, i32 poison>
LLVM ERROR: Broken function found, compilation aborted!
```

## Comments

### Comment 1 - nikic

/cherry-pick c322a0c462b1b277e5862aeae2e95a40f7b130d4

---

### Comment 2 - llvmbot

/pull-request llvm/llvm-project#176326

---

