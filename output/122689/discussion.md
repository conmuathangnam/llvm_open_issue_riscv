# Unnecessary call to `memset` when initializing an array of structs with non zero member initialization.

**Author:** gchatelet
**URL:** https://github.com/llvm/llvm-project/issues/122689

## Body

[godbolt link](https://godbolt.org/z/h39sa6fnv)

```
struct T {
  int a = 1;
  int b = 0;
};

template <int n>
struct S { T a[n]; };

S<75> F() { return {}; }
```

Compiled with `-O3 -std=c++20 -DNDEBUG -fno-exceptions -march=skylake` generates the following assembly
```
.LCPI0_0:
        .long   1
        .long   0
F():
        pushq   %rbx
        movq    %rdi, %rbx
        movl    $600, %edx
        xorl    %esi, %esi
        callq   memset@PLT
        vbroadcastsd    .LCPI0_0(%rip), %ymm0
        vmovups %ymm0, 32(%rbx)
        vmovups %ymm0, (%rbx)
        vmovups %ymm0, 96(%rbx)
        vmovups %ymm0, 64(%rbx)
        vmovups %ymm0, 160(%rbx)
        vmovups %ymm0, 128(%rbx)
        vmovups %ymm0, 224(%rbx)
        vmovups %ymm0, 192(%rbx)
        vmovups %ymm0, 288(%rbx)
        vmovups %ymm0, 256(%rbx)
        vmovups %ymm0, 352(%rbx)
        vmovups %ymm0, 320(%rbx)
        vmovups %ymm0, 416(%rbx)
        vmovups %ymm0, 384(%rbx)
        vmovups %ymm0, 480(%rbx)
        vmovups %ymm0, 448(%rbx)
        vmovups %ymm0, 544(%rbx)
        vmovups %ymm0, 512(%rbx)
        vmovups %xmm0, 576(%rbx)
        movq    $1, 592(%rbx)
        movq    %rbx, %rax
        popq    %rbx
        vzeroupper
        retq
```

The compiler first clears `S<75>` content with a call to `memset` and then sets its content through an unrolled loop of YMM stores.
If using `S<74>` instead of `S<75>` the call to `memset` goes away.

Apparently the clearing part  is created in the frontend (clang), here is the LLVM IR with `-O0`:
```
%struct.S = type { [75 x %struct.T] }
%struct.T = type { i32, i32 }

define dso_local void @F()(ptr dead_on_unwind noalias writable sret(%struct.S) align 4 %agg.result) {
entry:
  call void @llvm.memset.p0.i64(ptr align 4 %agg.result, i8 0, i64 592, i1 false)
  %a = getelementptr inbounds nuw %struct.S, ptr %agg.result, i32 0, i32 0
  %arrayinit.end = getelementptr inbounds %struct.T, ptr %a, i64 75
  br label %arrayinit.body

arrayinit.body:
  %arrayinit.cur = phi ptr [ %a, %entry ], [ %arrayinit.next, %arrayinit.body ]
  %a1 = getelementptr inbounds nuw %struct.T, ptr %arrayinit.cur, i32 0, i32 0
  store i32 1, ptr %a1, align 4
  %b = getelementptr inbounds nuw %struct.T, ptr %arrayinit.cur, i32 0, i32 1
  store i32 0, ptr %b, align 4
  %arrayinit.next = getelementptr inbounds %struct.T, ptr %arrayinit.cur, i64 1
  %arrayinit.done = icmp eq ptr %arrayinit.next, %arrayinit.end
  br i1 %arrayinit.done, label %arrayinit.end2, label %arrayinit.body

arrayinit.end2:
  ret void
}

declare void @llvm.memset.p0.i64(ptr nocapture writeonly, i8, i64, i1 immarg) #1
```

## Comments

### Comment 1 - gchatelet

CC @goldvitaly who discovered it.

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: Guillaume Chatelet (gchatelet)

<details>
[godbolt link](https://godbolt.org/z/h39sa6fnv)

```
struct T {
  int a = 1;
  int b = 0;
};

template &lt;int n&gt;
struct S { T a[n]; };

S&lt;75&gt; F() { return {}; }
```

Compiled with `-O3 -std=c++20 -DNDEBUG -fno-exceptions -march=skylake` generates the following assembly
```
.LCPI0_0:
        .long   1
        .long   0
F():
        pushq   %rbx
        movq    %rdi, %rbx
        movl    $600, %edx
        xorl    %esi, %esi
        callq   memset@<!-- -->PLT
        vbroadcastsd    .LCPI0_0(%rip), %ymm0
        vmovups %ymm0, 32(%rbx)
        vmovups %ymm0, (%rbx)
        vmovups %ymm0, 96(%rbx)
        vmovups %ymm0, 64(%rbx)
        vmovups %ymm0, 160(%rbx)
        vmovups %ymm0, 128(%rbx)
        vmovups %ymm0, 224(%rbx)
        vmovups %ymm0, 192(%rbx)
        vmovups %ymm0, 288(%rbx)
        vmovups %ymm0, 256(%rbx)
        vmovups %ymm0, 352(%rbx)
        vmovups %ymm0, 320(%rbx)
        vmovups %ymm0, 416(%rbx)
        vmovups %ymm0, 384(%rbx)
        vmovups %ymm0, 480(%rbx)
        vmovups %ymm0, 448(%rbx)
        vmovups %ymm0, 544(%rbx)
        vmovups %ymm0, 512(%rbx)
        vmovups %xmm0, 576(%rbx)
        movq    $1, 592(%rbx)
        movq    %rbx, %rax
        popq    %rbx
        vzeroupper
        retq
```

The compiler first clears `S&lt;75&gt;` content with a call to `memset` and then sets its content through an unrolled loop of YMM stores.
If using `S&lt;74&gt;` instead of `S&lt;75&gt;` the call to `memset` goes away.

Apparently the clearing part  is created in the frontend (clang), here is the LLVM IR with `-O0`:
```
%struct.S = type { [75 x %struct.T] }
%struct.T = type { i32, i32 }

define dso_local void @<!-- -->F()(ptr dead_on_unwind noalias writable sret(%struct.S) align 4 %agg.result) {
entry:
  call void @<!-- -->llvm.memset.p0.i64(ptr align 4 %agg.result, i8 0, i64 592, i1 false)
  %a = getelementptr inbounds nuw %struct.S, ptr %agg.result, i32 0, i32 0
  %arrayinit.end = getelementptr inbounds %struct.T, ptr %a, i64 75
  br label %arrayinit.body

arrayinit.body:
  %arrayinit.cur = phi ptr [ %a, %entry ], [ %arrayinit.next, %arrayinit.body ]
  %a1 = getelementptr inbounds nuw %struct.T, ptr %arrayinit.cur, i32 0, i32 0
  store i32 1, ptr %a1, align 4
  %b = getelementptr inbounds nuw %struct.T, ptr %arrayinit.cur, i32 0, i32 1
  store i32 0, ptr %b, align 4
  %arrayinit.next = getelementptr inbounds %struct.T, ptr %arrayinit.cur, i64 1
  %arrayinit.done = icmp eq ptr %arrayinit.next, %arrayinit.end
  br i1 %arrayinit.done, label %arrayinit.end2, label %arrayinit.body

arrayinit.end2:
  ret void
}

declare void @<!-- -->llvm.memset.p0.i64(ptr nocapture writeonly, i8, i64, i1 immarg) #<!-- -->1
```
</details>


---

