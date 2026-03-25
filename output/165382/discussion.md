# [HashRecognize] Miscompile due to data evolution with an outside user: poison generated

**Author:** iamanonymouscs
**URL:** https://github.com/llvm/llvm-project/issues/165382
**Status:** Closed
**Labels:** miscompilation, llvm:analysis
**Closed Date:** 2025-10-29T21:50:14Z

## Body

This program returns 0 at -O0/s and 139 at -O1/2/3

```
int a, b;
short c, e;
long d[56];
int main() {
  a = 0;
  for (; a < 6; a++) {
    b = 0;
    for (; b < 8; b++) {
      if ((e ^ c) & 32768)
        e = e << 1 ^ 3;
      else
        e <<= 1;
      c <<= 1;
    }
    d[b + c - a] = 0;
  }
}
```

Compiler Explorer: https://godbolt.org/z/fxjj1o4Yf

## Comments

### Comment 1 - antoniofrighetto

Partially reduced IR until it matches the output of clang 21.1.0:
```llvm
@a = global i32 0, align 4
@b = global i32 0, align 4
@e = global i16 0, align 2
@c = global i16 0, align 2
@d = global [56 x i64] zeroinitializer, align 16

define i32 @main() {
entry:
  %e.promoted29 = load i16, ptr @e, align 2
  %c.promoted31 = load i16, ptr @c, align 2
  br label %for.cond1.preheader

for.cond1.preheader:                              ; preds = %entry, %for.end
  %storemerge35 = phi i32 [ 0, %entry ], [ %inc16, %for.end ]
  %storemerge2023.lcssa3034 = phi i16 [ %e.promoted29, %entry ], [ %storemerge20, %for.end ]
  %shl1224.lcssa3233 = phi i16 [ %c.promoted31, %entry ], [ %shl12, %for.end ]
  br label %for.body3

for.body3:                                        ; preds = %for.cond1.preheader, %for.body3
  %storemerge1827 = phi i32 [ 0, %for.cond1.preheader ], [ %inc, %for.body3 ]
  %storemerge202326 = phi i16 [ %storemerge2023.lcssa3034, %for.cond1.preheader ], [ %storemerge20, %for.body3 ]
  %shl122425 = phi i16 [ %shl1224.lcssa3233, %for.cond1.preheader ], [ %shl12, %for.body3 ]
  %xor19 = xor i16 %shl122425, %storemerge202326
  %shl9 = shl i16 %storemerge202326, 1
  %xor6 = xor i16 %shl9, 3
  %tobool.not21 = icmp slt i16 %xor19, 0
  %storemerge20 = select i1 %tobool.not21, i16 %xor6, i16 %shl9
  %shl12 = shl i16 %shl122425, 1
  %inc = add nuw nsw i32 %storemerge1827, 1
  %cmp2 = icmp samesign ult i32 %storemerge1827, 7
  br i1 %cmp2, label %for.body3, label %for.end

for.end:                                          ; preds = %for.body3
  %conv14 = sext i16 %shl12 to i32
  %add = sub nsw i32 %inc, %storemerge35
  %sub = add i32 %add, %conv14
  %idxprom = sext i32 %sub to i64
  %arrayidx = getelementptr inbounds i64, ptr @d, i64 %idxprom
  store i64 0, ptr %arrayidx, align 8
  %inc16 = add nuw nsw i32 %storemerge35, 1
  %cmp = icmp samesign ult i32 %storemerge35, 5
  br i1 %cmp, label %for.cond1.preheader, label %for.end17

for.end17:                                        ; preds = %for.end
  store i32 %inc16, ptr @a, align 4
  store i32 %inc, ptr @b, align 4
  store i16 %storemerge20, ptr @e, align 2
  store i16 %shl12, ptr @c, align 2
  ret i32 0
}
```
Running `./bin/opt -passes="loop(loop-idiom,indvars)" test.ll -o test.opt.ll && ./bin/clang test.opt.ll && ./a.out` leads to the segfault.

Particularly, it seems like LoopIdiomRecognize, as part of the CRC transform achieved via recent HashRecognize analysis, moved the loop `for.body3` into a sequence of instructions, where the table lookup looks as follows:
```llvm
  %tbl.ptradd = getelementptr inbounds i16, ptr @.crctable, i64 %indexer.ext
  %tbl.ld = load i16, ptr %tbl.ptradd, align 2
  %crc.be.shift = shl i16 %crc, 8
  %crc.next = xor i16 %crc.be.shift, %tbl.ld
  %shl12 = shl i16 poison, 1
```
poison in `%shl12` is subsequently propagated and finally used to index `@d` global variable. cc/ @artagnon, @fhahn

---

### Comment 2 - artagnon

Thanks, will look into this along with @pfusik in the morning.

---

### Comment 3 - artagnon

Further reduced:

```llvm
define i16 @crc.data.next.outside.user(i16 %crc.init, i16 %data.init) {
entry:
  br label %loop

loop:
  %iv = phi i32 [ 0, %entry ], [ %iv.next, %loop ]
  %crc = phi i16 [ %crc.init, %entry ], [ %crc.next, %loop ]
  %data = phi i16 [ %data.init, %entry ], [ %data.next, %loop ]
  %xor.crc.data = xor i16 %data, %crc
  %crc.shl = shl i16 %crc, 1
  %crc.xor = xor i16 %crc.shl, 3
  %check.sb = icmp slt i16 %xor.crc.data, 0
  %crc.next = select i1 %check.sb, i16 %crc.xor, i16 %crc.shl
  %data.next = shl i16 %data, 1
  %iv.next = add nuw nsw i32 %iv, 1
  %exit.cond = icmp samesign ult i32 %iv, 7
  br i1 %exit.cond, label %loop, label %exit

exit:
  %ret = xor i16 %data.next, %crc.next
  ret i16 %ret
}
```

The diff between `opt -passes=loop(loop-idiom)` with and without `-disable-loop-idiom-hashrecognize` is:

```diff
diff --git a/main.li.ll b/reduced.li.ll
index b342d46..d84cbd2 100644
--- a/main.li.ll
+++ b/reduced.li.ll
@@ -1,26 +1,33 @@
 ; ModuleID = 'reduced.ll'
 source_filename = "reduced.ll"

+@.crctable = private constant [256 x i16] [i16 0, i16 3, i16 6, i16 5, i16 12, i16 15, i16 10, i16 9, i16 24, i16 27, i16 30, i16 29, i16 20, i16 23, i16 18, i16 17, i16 48, i16 51, i16 54, i16 53, i16 60, i16 63, i16 58, i16 57, i16 40, i16 43, i16 46, i16 45, i16 36, i16 39, i16 34, i16 33, i16 96, i16 99, i16 102, i16 101, i16 108, i16 111, i16 106, i16 105, i16 120, i16 123, i16 126, i16 125, i16 116, i16 119, i16 114, i16 113, i16 80, i16 83, i16 86, i16 85, i16 92, i16 95, i16 90, i16 89, i16 72, i16 75, i16 78, i16 77, i16 68, i16 71, i16 66, i16 65, i16 192, i16 195, i16 198, i16 197, i16 204, i16 207, i16 202, i16 201, i16 216, i16 219, i16 222, i16 221, i16 212, i16 215, i16 210, i16 209, i16 240, i16 243, i16 246, i16 245, i16 252, i16 255, i16 250, i16 249, i16 232, i16 235, i16 238, i16 237, i16 228, i16 231, i16 226, i16 225, i16 160, i16 163, i16 166, i16 165, i16 172, i16 175, i16 170, i16 169, i16 184, i16 187, i16 190, i16 189, i16 180, i16 183, i16 178, i16 177, i16 144, i16 147, i16 150, i16 149, i16 156, i16 159, i16 154, i16 153, i16 136, i16 139, i16 142, i16 141, i16 132, i16 135, i16 130, i16 129, i16 384, i16 387, i16 390, i16 389, i16 396, i16 399, i16 394, i16 393, i16 408, i16 411, i16 414, i16 413, i16 404, i16 407, i16 402, i16 401, i16 432, i16 435, i16 438, i16 437, i16 444, i16 447, i16 442, i16 441, i16 424, i16 427, i16 430, i16 429, i16 420, i16 423, i16 418, i16 417, i16 480, i16 483, i16 486, i16 485, i16 492, i16 495, i16 490, i16 489, i16 504, i16 507, i16 510, i16 509, i16 500, i16 503, i16 498, i16 497, i16 464, i16 467, i16 470, i16 469, i16 476, i16 479, i16 474, i16 473, i16 456, i16 459, i16 462, i16 461, i16 452, i16 455, i16 450, i16 449, i16 320, i16 323, i16 326, i16 325, i16 332, i16 335, i16 330, i16 329, i16 344, i16 347, i16 350, i16 349, i16 340, i16 343, i16 338, i16 337, i16 368, i16 371, i16 374, i16 373, i16 380, i16 383, i16 378, i16 377, i16 360, i16 363, i16 366, i16 365, i16 356, i16 359, i16 354, i16 353, i16 288, i16 291, i16 294, i16 293, i16 300, i16 303, i16 298, i16 297, i16 312, i16 315, i16 318, i16 317, i16 308, i16 311, i16 306, i16 305, i16 272, i16 275, i16 278, i16 277, i16 284, i16 287, i16 282, i16 281, i16 264, i16 267, i16 270, i16 269, i16 260, i16 263, i16 258, i16 257]
+
 define i16 @crc.data.next.outside.user(i16 %crc.init, i16 %data.init) {
 entry:
   br label %loop

 loop:                                             ; preds = %loop, %entry
   %iv = phi i32 [ 0, %entry ], [ %iv.next, %loop ]
-  %crc = phi i16 [ %crc.init, %entry ], [ %crc.next, %loop ]
-  %data = phi i16 [ %data.init, %entry ], [ %data.next, %loop ]
-  %xor.crc.data = xor i16 %data, %crc
-  %crc.shl = shl i16 %crc, 1
-  %crc.xor = xor i16 %crc.shl, 3
-  %check.sb = icmp slt i16 %xor.crc.data, 0
-  %crc.next = select i1 %check.sb, i16 %crc.xor, i16 %crc.shl
-  %data.next = shl i16 %data, 1
+  %crc2 = phi i16 [ %crc.init, %entry ], [ %crc.next3, %loop ]
+  %iv.bits = shl i32 %iv, 3
+  %iv.indexer = trunc i32 %iv.bits to i16
+  %data.indexer = shl i16 %data.init, %iv.indexer
+  %crc.data.indexer = xor i16 %data.indexer, %crc2
+  %indexer.hi = lshr i16 %crc.data.indexer, 8
+  %indexer.hi.lo.byte = trunc i16 %indexer.hi to i8
+  %indexer.ext = zext i8 %indexer.hi.lo.byte to i64
+  %tbl.ptradd = getelementptr inbounds i16, ptr @.crctable, i64 %indexer.ext
+  %tbl.ld = load i16, ptr %tbl.ptradd, align 2
+  %crc.be.shift = shl i16 %crc2, 8
+  %crc.next3 = xor i16 %crc.be.shift, %tbl.ld
+  %data.next = shl i16 poison, 1
   %iv.next = add nuw nsw i32 %iv, 1
-  %exit.cond = icmp samesign ult i32 %iv, 7
-  br i1 %exit.cond, label %loop, label %exit
+  %exit.cond1 = icmp ne i32 %iv, 0
+  br i1 %exit.cond1, label %loop, label %exit

 exit:                                             ; preds = %loop
-  %crc.next.lcssa = phi i16 [ %crc.next, %loop ]
+  %crc.next.lcssa = phi i16 [ %crc.next3, %loop ]
   %data.next.lcssa = phi i16 [ %data.next, %loop ]
   %ret = xor i16 %data.next.lcssa, %crc.next.lcssa
   ret i16 %ret
```

The issue is that data.next has a user outside the loop, and hence CRC optimization is not possible for this example, as the optimization would strip the data PHI entirely. Will post a fix soon.

---

