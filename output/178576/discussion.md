# Missing stack unpoisoning for alloca

**Author:** PiJoules
**URL:** https://github.com/llvm/llvm-project/issues/178576
**Status:** Open
**Labels:** compiler-rt:asan, miscompilation

## Body

```c++
#include <stdint.h>

[[clang::noinline]]
void func(uintptr_t** ptr) {
  if (*ptr)
    __builtin_trap();
}

void func2() {
  uintptr_t* ptr = nullptr;
  func(&ptr);
}
```

The above code compiled with `clang++ -fsanitize=address -S -o - -emit-llvm /tmp/test.cc -O1` will produce

```llvm
define dso_local void @_Z4funcPPm(ptr noundef readonly captures(none) %ptr) local_unnamed_addr #0 {
entry:                                                                                                                    
  %0 = ptrtoint ptr %ptr to i64                                                                                           
  %1 = lshr i64 %0, 3      
  %2 = add i64 %1, 2147450880
  %3 = inttoptr i64 %2 to ptr
  %4 = load i8, ptr %3, align 1           
  %5 = icmp ne i8 %4, 0
  br i1 %5, label %6, label %7

6:                                                ; preds = %entry
  call void @__asan_report_load8(i64 %0) #5
  unreachable                     
                                                             
7:                                                ; preds = %entry
  %8 = load ptr, ptr %ptr, align 8, !tbaa !10                                                                                                                                                                                                       
  %tobool.not = icmp eq ptr %8, null
  br i1 %tobool.not, label %if.end, label %if.then
                                                             
if.then:                                          ; preds = %7
  call void @__asan_handle_no_return()
  tail call void @llvm.trap()  
  unreachable                     
                                                             
if.end:                                           ; preds = %7
  ret void                                                  
}

; Function Attrs: cold nobuiltin noreturn nounwind
declare void @llvm.trap() #1

; Function Attrs: mustprogress nobuiltin nounwind sanitize_address uwtable
define dso_local void @_Z5func2v() local_unnamed_addr #2 {
entry:
  %ptr = alloca ptr, align 8, !annotation !13     ;; NO UNPOISONING HERE
  call void @llvm.lifetime.start.p0(ptr nonnull %ptr) #6
  store ptr null, ptr %ptr, align 8, !tbaa !10
  call void @_Z4funcPPm(ptr noundef nonnull %ptr)
  call void @llvm.lifetime.end.p0(ptr nonnull %ptr) #6
  ret void
}
```

IR is not emitted for `func2` to either unpoison shadow corresponding to the `%ptr` alloca, nor is the IR for the stack malloc and poisoning of redzones emitted. When `func` is called, the instrumentation will check shadow for `%ptr` but since it wasn't unpoisoned, its shadow could be any value, potentially leading to a false positive.

I suspect this alloca isn't instrumented because it's not seen as an "interesting" alloca because it's "safe" according to `StackSafetyGlobalInfo::isSafe`:

```
  bool IsInteresting =
...
       // safe allocas are not interesting
       !(SSGI && SSGI->isSafe(AI)));
```

If I disable `StackSafetyGlobalInfo` via `-mllvm -asan-use-stack-safety=0` then I see the expected instrumentation for that alloca.

I don't see this at `-O0` so it could also be there's a bad optimization elsewhere.

## Comments

### Comment 1 - vitalybuka

> %ptr = alloca ptr, align 8, !annotation !13     ;; NO UNPOISONING HERE

Asan is build with assumption that everything on stack is unpoisoned by default

> since it wasn't unpoisoned, its shadow could be any value, potentially leading to a false positive.

The issue is the non-zero value there when we enter the function func2.

---

### Comment 2 - PiJoules

> > %ptr = alloca ptr, align 8, !annotation !13     ;; NO UNPOISONING HERE
> 
> Asan is build with assumption that everything on stack is unpoisoned by default
> 
> > since it wasn't unpoisoned, its shadow could be any value, potentially leading to a false positive.
> 
> The issue is the non-zero value there when we enter the function func2.

Hmm. Does asan explicitly set shadow to zero for the stack after exiting a function or scope? I would suspect that if any function prologue would contain stack poisoning, then that stack shadow would need to be zero'd somewhere if another function were to reuse that stack space.

---

### Comment 3 - vitalybuka

> > > %ptr = alloca ptr, align 8, !annotation !13     ;; NO UNPOISONING HERE
> > 
> > 
> > Asan is build with assumption that everything on stack is unpoisoned by default
> > > since it wasn't unpoisoned, its shadow could be any value, potentially leading to a false positive.
> > 
> > 
> > The issue is the non-zero value there when we enter the function func2.
> 
> Hmm. Does asan explicitly set shadow to zero for the stack after exiting a function or scope? I would suspect that if any function prologue would contain stack poisoning, then that stack shadow would need to be zero'd somewhere if another function were to reuse that stack space.

Yes.
Asan has 2 kind of stack frames:
1. Normal - there must be cleanup code before every return. To optimize it's incremental, so it's possible to have bug there.
2. FakeStack - it's actually poisoned on return, for UAR detection, and unpoisoned on re-allocation.

Also code with fancy stack management can break stuff.

User code with custom container poisoning, which can incompletely unpoison the stack.

---

