# InstCombine pass wrongly removes va_start/va_end

**Author:** hongjia
**URL:** https://github.com/llvm/llvm-project/issues/140215
**Status:** Closed
**Labels:** miscompilation, llvm:instcombine
**Closed Date:** 2025-05-17T08:04:04Z

## Body

Test program:

``` C
typedef __builtin_va_list va_list;
#define va_start(ap, param) __builtin_va_start(ap, param)
#define va_end(ap) __builtin_va_end(ap)
#define va_arg(ap, type) __builtin_va_arg(ap, type)
#define va_copy(d, s) __builtin_va_copy(d, s)

int foo(int n, ...)
{
  va_list ap, aq;
  int x;

  va_start(ap, n);
  va_copy(aq, ap);
  va_end(ap);

  x = va_arg(aq, int);
  va_end(aq);

  return x + n;
}
```

Compile it with -O1:

```
clang -S -emit-llvm -O1 ic-va.c -mllvm --debug-only=instcombine

...
IC: Visiting:   call void @llvm.va_copy.p0(ptr nonnull %aq, ptr nonnull %ap)
IC: Visiting:   call void @llvm.va_end.p0(ptr %ap)
IC: ERASE   call void @llvm.va_start.p0(ptr nonnull %ap)
ADD DEFERRED:   %ap = alloca ptr, align 2
IC: ERASE   call void @llvm.va_end.p0(ptr %ap)
...
```

The resulting IR:

```
...
; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn
define dso_local i32 @foo(i32 noundef %n, ...) local_unnamed_addr #0 {
entry:
  %ap = alloca ptr, align 2
  %aq = alloca ptr, align 2
  call void @llvm.lifetime.start.p0(i64 2, ptr nonnull %ap) #3
  call void @llvm.lifetime.start.p0(i64 2, ptr nonnull %aq) #3
  call void @llvm.va_copy.p0(ptr nonnull %aq, ptr nonnull %ap)
  %0 = va_arg ptr %aq, i32
  call void @llvm.va_end.p0(ptr %aq)
  %add = add nsw i32 %0, %n
  call void @llvm.lifetime.end.p0(i64 2, ptr nonnull %aq) #3
  call void @llvm.lifetime.end.p0(i64 2, ptr nonnull %ap) #3
  ret i32 %add
}
...
```

%ap is used in va_copy() without being initialized.

For now I use the following ugly patch to fix this:

```
diff --git a/llvm/lib/Transforms/InstCombine/InstCombineCalls.cpp b/llvm/lib/Transforms/InstCombine/InstCombineCalls.cpp
index c88710214e7e..d83228ae4b7e 100644
--- a/llvm/lib/Transforms/InstCombine/InstCombineCalls.cpp
+++ b/llvm/lib/Transforms/InstCombine/InstCombineCalls.cpp
@@ -808,7 +808,10 @@ removeTriviallyEmptyRange(IntrinsicInst &EndI, InstCombinerImpl &IC,
           IC.eraseInstFromFunction(*I);
           IC.eraseInstFromFunction(EndI);
           return true;
-        }
+        } else if (EndI.getIntrinsicID() == Intrinsic::vaend &&
+                   I->getIntrinsicID() == Intrinsic::vacopy &&
+                   I->getArgOperand(1) == EndI.getArgOperand(0))
+          break;
         // Skip start intrinsics that don't pair with this end intrinsic.
         continue;
       }
```

