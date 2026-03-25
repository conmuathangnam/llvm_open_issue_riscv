# [AArch64] copy incorrectly erased and discard zero-extend operation

**Author:** llvmbot
**URL:** https://github.com/llvm/llvm-project/issues/47410
**Status:** Closed
**Labels:** backend:AArch64, bugzilla, miscompilation
**Closed Date:** 2025-05-07T10:33:17Z

## Body

|  |  |
| --- | --- |
| Bugzilla Link | [48066](https://llvm.org/bz48066) |
| Version | trunk |
| OS | Linux |
| Reporter | LLVM Bugzilla Contributor |
| CC | @Arnaud-de-Grandmaison-ARM,@DMG862,@smithp35 |

## Extended Description 
$ clang -O3 test-case.ll -w ;./a.out
c = -8917
$ clang -O0 test-case.ll -w ;./a.out
c = 4294958379


@&#8203;b = dso_local local_unnamed_addr global { [3 x i8], i8, i8, i8, i8, i8 } { [3 x i8] undef, i8 -64, i8 74, i8 -9, i8 -1, i8 7 }, align 8
@&#8203;c = common dso_local local_unnamed_addr global i64 0, align 8
@.str = private unnamed_addr constant [9 x i8] c"c = %ld\0A\00", align 1


define dso_local i8 @&#8203;d(i64 %h) local_unnamed_addr {
entry:
  ret i8 undef
}

declare dso_local i32 @&#8203;printf(i8* nocapture readonly, ...) local_unnamed_addr

define dso_local i32 @&#8203;main() local_unnamed_addr {
entry:
  %bf.load = load i64, i64* bitcast ({ [3 x i8], i8, i8, i8, i8, i8 }* @&#8203;b to i64*), align 8
  %0 = lshr i64 %bf.load, 30
  %sext3 = shl nuw nsw i64 %0, 16
  %sext = trunc i64 %sext3 to i32
  %conv.i = ashr exact i32 %sext, 16
  %conv.i.i = zext i32 %conv.i to i64
  store i64 %conv.i.i, i64* @&#8203;c, align 8
  %call.i.i = tail call i32 (i8*, ...) @&#8203;printf(i8* nonnull dereferenceable(1) getelementptr inbounds ([9 x i8], [9 x i8]* @.str, i64 0, i64 0), i64 %conv.i.i)
  %call4.i = tail call i8 bitcast (i8 (i64)* @&#8203;d to i8 (i32, i32)*)(i32 0, i32 %conv.i)
  ret i32 0
}

## Comments

### Comment 1 - c-rhodes

now fixed: https://godbolt.org/z/xW579rzTY

I managed to reproduce the issue on LLVM 11: https://godbolt.org/z/6saK44nf9

It's been fixed since LLVM 12.

---

