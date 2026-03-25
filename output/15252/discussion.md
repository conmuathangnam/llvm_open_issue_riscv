# --bitfield code is computed incorrectly

**Author:** llvmbot
**URL:** https://github.com/llvm/llvm-project/issues/15252
**Status:** Closed
**Labels:** bugzilla, miscompilation, llvm:optimizations
**Closed Date:** 2025-07-14T04:03:15Z

## Body

|  |  |
| --- | --- |
| Bugzilla Link | [14880](https://llvm.org/bz14880) |
| Version | trunk |
| OS | MacOS X |
| Reporter | LLVM Bugzilla Contributor |
| CC | @zygoloid |

## Extended Description 
Code:

===
#include <stdio.h>
#include <stdlib.h>

struct foo {
    int x:31;
    int y:1;
};

static void baz(struct foo* f) {
    --f->x;
    if (f->x < 0)
        exit(1);
}

int main() {
    struct foo f;

    f.x = 1;
    f.y = 0;

    printf("%d %d\n", f.x, f.y);
    baz(&f);
    printf("%d %d\n", f.x, f.y);

    return 0;
}
===

When compiled with clang 3.2 with -O2 on OSX or on Linux, outputs:

===
1 0
0 -1
===

Should output:

===
1 0
0 0
===

## Comments

### Comment 1 - llvmbot

Richard, works for you in trunk or in 3.2?

When "baz" is static, clang 3.2 `clang -c -emit-llvm -O2 ./tmp.c` emits bitcode:

===
; ModuleID = './tmp.o'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-apple-macosx10.8.0"

@.str = private unnamed_addr constant [7 x i8] c"%d %d\0A\00", align 1

define i32 @&#8203;main() nounwind uwtable ssp {
baz.exit:
  %0 = tail call i32 (i8*, ...)* @&#8203;printf(i8* getelementptr inbounds ([7 x i8]* @.str, i64 0, i64 0), i32 1, i32 0) nounwind
  %1 = tail call i32 (i8*, ...)* @&#8203;printf(i8* getelementptr inbounds ([7 x i8]* @.str, i64 0, i64 0), i32 0, i32 -1) nounwind
  ret i32 0
}

declare i32 @&#8203;printf(i8* nocapture, ...) nounwind
===

(similar on Linux x86_64)

---

### Comment 2 - ec04fc15-fa35-46f2-80e1-5d271f2ef708

This works for me, on x86_64 linux. However, I can see a bug in the optimization of 'baz'. After removing the 'if', here's the IR:

%struct.foo = type { [4 x i8] }

define void @&#8203;baz(%struct.foo* %f) nounwind uwtable {
entry:
  %f.addr = alloca %struct.foo*, align 8
  store %struct.foo* %f, %struct.foo** %f.addr, align 8
  %0 = load %struct.foo** %f.addr, align 8
  %1 = bitcast %struct.foo* %0 to i32*
  %bf.load = load i32* %1, align 4
  %bf.shl = shl i32 %bf.load, 1
  %bf.ashr = ashr i32 %bf.shl, 1
  %dec = add nsw i32 %bf.ashr, -1
  %bf.load1 = load i32* %1, align 4
  %bf.value = and i32 %dec, 2147483647
  %bf.clear = and i32 %bf.load1, -2147483648
  %bf.set = or i32 %bf.clear, %bf.value
  store i32 %bf.set, i32* %1, align 4
  %bf.result.shl = shl i32 %bf.value, 1
  %bf.result.ashr = ashr i32 %bf.result.shl, 1
  ret void
}

This looks correct, but we optimize it to:

define void @&#8203;baz(%struct.foo* nocapture %f) nounwind uwtable {
entry:
  %0 = bitcast %struct.foo* %f to i32*
  %bf.load = load i32* %0, align 4
  %dec = add nsw i32 %bf.load, 2147483647
  %bf.value = and i32 %dec, 2147483647
  %bf.clear = and i32 %bf.load, -2147483648
  %bf.set = or i32 %bf.value, %bf.clear
  store i32 %bf.set, i32* %0, align 4
  ret void
}

Note that the 'nsw' on the 'add' here is wrong. If this were 'add nsw i32 %bf.load, -1', it would be correct.

IIRC, this is a bug in SimplifyDemandedBits: when simplifying the operands of an 'add nsw' where the sign bit is not demanded, it should either demand the sign bit of the inputs or drop the 'nsw' (it's not OK to replace it with 0!).

---

### Comment 3 - Sunday111

This issue is reproducible via online compiler on version 3.2 (https://godbolt.org/z/P8xfh7fsY) and disappears if you switch to 3.3. So I believe it would be safe to close it

---

