# [CodeGenPrepare] Miscompilation with -O3

**Author:** dtcxzyw
**URL:** https://github.com/llvm/llvm-project/issues/118172
**Status:** Closed
**Labels:** llvm:codegen, miscompilation
**Closed Date:** 2024-12-01T03:25:32Z

## Body

Reproducer: https://godbolt.org/z/h4T6Gh5f4
```
#include <stdint.h>
#include <stdio.h>
uint8_t b;
int32_t i;
static int32_t e = -6L;
uint16_t g;
int8_t j;
void r() {
  for (e = 9;;)
    ;
}
void u() { e = 0; }
static void o(int8_t t) {
  g = t;
  if ((b = t) | j)
    i = 0;
}
int main() {
  o(e);
  printf("%d\n", (int)g);
  return 0;
}
```


## Comments

### Comment 1 - dtcxzyw

Reduced reproducer:
```
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@e = global i32 -6, align 4
@.str = constant [4 x i8] c"%d\0A\00", align 1

define i32 @main() {
entry:
  %0 = load i32, ptr @e, align 4
  %conv = trunc nsw i32 %0 to i8
  %conv.i = sext i8 %conv to i16
  %conv1.i = and i32 %0, 255
  %conv1 = zext i16 %conv.i to i32
  %call = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str, i32 noundef %conv1)
  ret i32 0
}

declare i32 @printf(ptr, ...)
```
```
> bin/lli test.ll
250
> bin/lli --force-interpreter test.ll
65530
```

---

### Comment 2 - dtcxzyw

Bisected to CGP: https://godbolt.org/z/ns4YzMbsj
```
; bin/opt -codegenprepare test.ll -S
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @main(ptr %p) {
entry:
  %x = load i32, ptr %p, align 4
  %conv = trunc nsw i32 %x to i8
  %conv.i = sext i8 %conv to i16
  %conv1.i = and i32 %x, 255
  %conv1 = zext i16 %conv.i to i32
  ret i32 %conv1
}
```
```
source_filename = "/app/example.ll"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @main(ptr %p) {
  %x = load i32, ptr %p, align 4
  %0 = and i32 %x, 255
  %conv = trunc nsw i32 %0 to i8
  %conv.i = sext i8 %conv to i16
  %conv1 = zext i16 %conv.i to i32
  ret i32 %conv1
}
```
Alive2: https://alive2.llvm.org/ce/z/SRPsG5



---

