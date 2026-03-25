# [LLC] CodeGenPrepare::runOnFunction hanging because of Bitcast and Phi

**Author:** emu1200tra
**URL:** https://github.com/llvm/llvm-project/issues/176688
**Status:** Closed
**Labels:** llvm:codegen, hang
**Closed Date:** 2026-01-22T08:29:10Z

## Body

As described in [this question](https://discourse.llvm.org/t/codegenprepare-runonfunction-hanging-because-of-bitcast-and-phi/89478), the following IR would make llc hang in CodeGenPrepare::runOnFunction:
```
// test.ll
define linkonce void @foo(i64 %r8) {
	%r11 = alloca i8, i32 180, align 1
	br label %BB0

BB0:
	%r101 = sext i32 0 to i64
	%r100 = add i64 %r8, %r101
	%r102 = inttoptr i64 %r100 to ptr
	%r24 = load i32, ptr %r102, align 4
	%r143 = getelementptr i8, ptr %r11, i32 128
	%r144 = load i8, ptr %r143, align 1
	%r53 = zext i8 %r144 to i32
	%r145 = icmp ne i32 %r53, 0
	br i1 %r145, label %BB1, label %BB2
BB1:
	%r64 = bitcast i32 %r24 to i32
	;%r64 = add i32 %r24, 0
	br label %BB6
BB2:
	%r58 = add i32 0, 0
	%r146 = icmp ne i32 %r58, 0
	br i1 %r146, label %BB3, label %BB4
BB3:
	%r147 = getelementptr i8, ptr %r11, i32 148
	%r148 = bitcast ptr %r147 to ptr
	%r62 = load i32, ptr %r148, align 4
	br label %BB5
BB4:
	%r61 = bitcast i32 %r24 to i32
	br label %BB5
BB5:
	%r59 = phi i32 [%r61, %BB4], [%r62, %BB3]
	%r60 = bitcast i32 %r59 to i32
	br label %BB6
BB6:
	%r46 = phi i32 [ %r60, %BB5], [%r64, %BB1]
	%r48 = bitcast i32 %r46 to i32
	%r149 = getelementptr i8, ptr %r11, i32 140
	%r150 = bitcast ptr %r149 to ptr
	store i32 %r48, ptr %r150, align 4
	ret void
}
```
The hanging loop is doing the following:

    1. Remove %r24.bc which is a bitcast after %r24 in BB0 since it is a NOOP copy in OptimizeNoopCopyExpression.
    2. Insert a bitcast %0 = bitcast i32 %r24 to i32 in BB4 because of SinkCast in NOOP optimization (originally %r61).
    3. When doing optimizePhiType for %r59, remove bitcast %0 (originally %r61).
    4. Insert bitcast after %r24 as %r24.bc = bitcast i32 %r24 to i32
    5. go to 1) again.

Can also obersve %r59 becoming %r59.tc.tc.tc.tc…… because of this infinite loop.

LLCs with version 10, 17, and 20 can reproduce this infinite loop.

Reproduce command: 
llc -O2 -march=x86-64 -relocation-model=dynamic-no-pic -filetype=obj --enable-patchpoint-liveness=false -o test.o test.ll


