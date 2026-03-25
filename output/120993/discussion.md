# [llvm] Incorrect vectorized codegen for Cortex-M55

**Author:** PiJoules
**URL:** https://github.com/llvm/llvm-project/issues/120993
**Status:** Closed
**Labels:** backend:ARM, miscompilation
**Closed Date:** 2024-12-24T08:48:04Z

## Body

```
struct Functor {
  void assignCoeff(float &dst, const float &src) { dst += src; }
};

struct Kernel {
  float *dst;
  const float *src;
  Functor f;

  void assignCoeff(int i) {
    f.assignCoeff(dst[i], src[i * 4]);
  }
};

void func(Kernel &kernel, int size) {
  //#pragma clang loop vectorize(disable)
  for (int i = 0; i < size; ++i)
    kernel.assignCoeff(i);
}
```

Clang seems to produce incorrect codegen when compiling this snippet with `/usr/local/google/home/leonardchan/misc/clang-cipd-latest-2/bin/clang++ -mcpu=cortex-m55 -mthumb -mfloat-abi=hard -march=armv8.1-m.main+mve.fp+fp.dp --target=arm-none-eabi -O2  -c /tmp/test.cc -fno-unroll-loops`

```
_Z4funcR6Kerneli:
        .fnstart
@ %bb.0:
        .save   {r7, lr}
        push    {r7, lr}
        cmp     r1, #1
        it      lt
        poplt   {r7, pc}
.LBB0_1:
        ldrd    r0, r2, [r0]
        add.w   r3, r2, r1, lsl #4
        subs    r3, #12
        cmp     r0, r3
        bhs     .LBB0_5
@ %bb.2:
        add.w   r3, r0, r1, lsl #2
        cmp     r2, r3
        bhs     .LBB0_5
@ %bb.3:
        dls     lr, r1
        .p2align        2
.LBB0_4:                                @ =>This Inner Loop Header: Depth=1
        vldr    s0, [r2]
        vldr    s2, [r0]
        adds    r2, #16
        vadd.f32        s0, s0, s2
        vstmia  r0!, {s0}
        le      lr, .LBB0_4
        b       .LBB0_7
.LBB0_5:
        adr     r3, .LCPI0_0
        vldrw.u32       q0, [r3]
        vadd.i32        q0, q0, r2
        dlstp.32        lr, r1
        .p2align        2
.LBB0_6:                                @ =>This Inner Loop Header: Depth=1
        vldrw.u32       q1, [q0, #256]!
        vldrw.u32       q2, [r0]
        vadd.f32        q1, q1, q2
        vstrw.32        q1, [r0], #16
        letp    lr, .LBB0_6
.LBB0_7:
        pop     {r7, pc}
        .p2align        4
@ %bb.8:
.LCPI0_0:
        .long   4294967040                      @ 0xffffff00
        .long   4294967104                      @ 0xffffff40
        .long   4294967168                      @ 0xffffff80
        .long   4294967232                      @ 0xffffffc0
```

It looks like emits two separate loops that can be taken in `func` depending on if there's potential overlap between `src` and `dst`. This check is done under `.LBB0_1` which will continue to `.LBB0_5` if no overlap is detected. At a high level, the check is `if (dst >= src + size * 16 - 12 || src >= dst + size * 4) goto .LBB0_5`. This branch is where the vectorized instructions are emitted. Effectively, `.LBB0_5` *should* add four initial negative offsets to the first multiple of four elements of src (`src[i]`, `src[(i+1) * 4]`, `src[(i+2) * 4]`, `src[(i+3) * 4]`, then `.LBB0_6` iterates through each `dst` and multiple of `src` then add and store them back to `dst`, but I think the offset calculation is slightly off.

Added some comments to the relevant bits:
```
.LBB0_5: 
        adr     r3, .LCPI0_0
        vldrw.u32       q0, [r3]         // load the 4 32-bit vals into q0
        vadd.i32        q0, q0, r2       // add each val against src
                                         //   q0.0 = src - 256
                                         //   q0.1 = src - 192
                                         //   q0.2 = src - 128
                                         //   q0.3 = src - 64
        dlstp.32        lr, r1           // lr = size
.LBB0_6: 
        vldrw.u32       q1, [q0, #256]!  // load the 4 32-bit vals from q0 into q1 + 256 and q1 += 256
                                         //   q1.0 = *(src)
                                         //   q1.1 = *(src + 64)
                                         //   q1.2 = *(src + 128)
                                         //   q1.3 = *(src + 192)
        vldrw.u32       q2, [r0]         // load 4 32-bit values from dst
        vadd.f32        q1, q1, q2       // add each 32-bit val from src and dst
                                         //   q1.0 = *(src) + dst[0]
                                         //   q1.1 = *(src + 64) + dst[1]
                                         //   q1.2 = *(src + 128) + dst[2]
                                         //   q1.3 = *(src + 192) + dst[3]
        vstrw.32        q1, [r0], #16    // store the 4 32-bit vals into dst then dst += 16
...
.LCPI0_0:
        .long   4294967040                      @ 0xffffff00 // -256
        .long   4294967104                      @ 0xffffff40 // -192
        .long   4294967168                      @ 0xffffff80 // -128
        .long   4294967232                      @ 0xffffffc0 // -64
```

Each of the offsets for the element accesses are 64 bytes apart when I think they should instead be 16 bytes since the access is `src[i * 4]` and `src` points to floats which should be 4 bytes. This can cause the `vldrw.u32 q1, [q0, #256]!` to access bad/uninitialized memory. I can verify with

```
__attribute__((optnone, noinline))
void testfunc() {
  constexpr int kSize = 2;
  float dst[kSize] = {1.0f, 2.0f};
  float src[] = {3.0f, -1.0f, -1.0f, -1.0f,
                          4.0f, -1.0f, -1.0f, -1.0f,
                          4.0f, -1.0f, -1.0f, -1.0f,
                          4.0f, -1.0f, -1.0f, -1.0f,
                          12.0f, -1.0f, -1.0f, -1.0f,
  };
  Kernel k;
  k.dst = dst;
  k.src = src;
  func(k, kSize);
}
```

that the vectorized code does indeed access the 16th element (`12.0f`) of `src` rather than the 1st element `4.0f`. Nothing seems to stand out from IR corresponding to the vectorized bits:

```
19:                                               ; preds = %19, %16
  %20 = phi i32 [ 0, %16 ], [ %29, %19 ]
  %21 = phi <4 x i32> [ <i32 0, i32 1, i32 2, i32 3>, %16 ], [ %30, %19 ]
  %22 = tail call <4 x i1> @llvm.get.active.lane.mask.v4i1.i32(i32 %20, i32 %1)
  %23 = getelementptr inbounds nuw float, ptr %5, i32 %20
  %24 = shl nsw <4 x i32> %21, splat (i32 4)
  %25 = getelementptr inbounds nuw i8, ptr %7, <4 x i32> %24
  %26 = tail call <4 x float> @llvm.masked.gather.v4f32.v4p0(<4 x ptr> %25, i32 4, <4 x i1> %22, <4 x float> poison), !tbaa !11, !alias.scope !13
  %27 = tail call <4 x float> @llvm.masked.load.v4f32.p0(ptr %23, i32 4, <4 x i1> %22, <4 x float> poison), !tbaa !11, !alias.scope !16, !noalias !13
  %28 = fadd <4 x float> %26, %27
  tail call void @llvm.masked.store.v4f32.p0(<4 x float> %28, ptr %23, i32 4, <4 x i1> %22), !tbaa !11, !alias.scope !16, !noalias !13
  %29 = add nuw i32 %20, 4
  %30 = add <4 x i32> %21, splat (i32 4)
  %31 = icmp eq i32 %29, %18
  br i1 %31, label %32, label %19, !llvm.loop !18

32:                                               ; preds = %19, %33, %2
  ret void
```

so I would suspect it's a backend issue when lowering, but I'm also unfamiliar with any of these intrinsics so maybe the IR might be incorrect.

Note that uncommenting the pragma which disables the loop vectorizer makes clang only emit the loop under `.LBB0_4` which does the accesses correctly.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-arm

Author: None (PiJoules)

<details>
```
struct Functor {
  void assignCoeff(float &amp;dst, const float &amp;src) { dst += src; }
};

struct Kernel {
  float *dst;
  const float *src;
  Functor f;

  void assignCoeff(int i) {
    f.assignCoeff(dst[i], src[i * 4]);
  }
};

void func(Kernel &amp;kernel, int size) {
  //#pragma clang loop vectorize(disable)
  for (int i = 0; i &lt; size; ++i)
    kernel.assignCoeff(i);
}
```

Clang seems to produce incorrect codegen when compiling this snippet with `/usr/local/google/home/leonardchan/misc/clang-cipd-latest-2/bin/clang++ -mcpu=cortex-m55 -mthumb -mfloat-abi=hard -march=armv8.1-m.main+mve.fp+fp.dp --target=arm-none-eabi -O2  -c /tmp/test.cc -fno-unroll-loops`

```
_Z4funcR6Kerneli:
        .fnstart
@ %bb.0:
        .save   {r7, lr}
        push    {r7, lr}
        cmp     r1, #<!-- -->1
        it      lt
        poplt   {r7, pc}
.LBB0_1:
        ldrd    r0, r2, [r0]
        add.w   r3, r2, r1, lsl #<!-- -->4
        subs    r3, #<!-- -->12
        cmp     r0, r3
        bhs     .LBB0_5
@ %bb.2:
        add.w   r3, r0, r1, lsl #<!-- -->2
        cmp     r2, r3
        bhs     .LBB0_5
@ %bb.3:
        dls     lr, r1
        .p2align        2
.LBB0_4:                                @ =&gt;This Inner Loop Header: Depth=1
        vldr    s0, [r2]
        vldr    s2, [r0]
        adds    r2, #<!-- -->16
        vadd.f32        s0, s0, s2
        vstmia  r0!, {s0}
        le      lr, .LBB0_4
        b       .LBB0_7
.LBB0_5:
        adr     r3, .LCPI0_0
        vldrw.u32       q0, [r3]
        vadd.i32        q0, q0, r2
        dlstp.32        lr, r1
        .p2align        2
.LBB0_6:                                @ =&gt;This Inner Loop Header: Depth=1
        vldrw.u32       q1, [q0, #<!-- -->256]!
        vldrw.u32       q2, [r0]
        vadd.f32        q1, q1, q2
        vstrw.32        q1, [r0], #<!-- -->16
        letp    lr, .LBB0_6
.LBB0_7:
        pop     {r7, pc}
        .p2align        4
@ %bb.8:
.LCPI0_0:
        .long   4294967040                      @ 0xffffff00
        .long   4294967104                      @ 0xffffff40
        .long   4294967168                      @ 0xffffff80
        .long   4294967232                      @ 0xffffffc0
```

It looks like emits two separate loops that can be taken in `func` depending on if there's potential overlap between `src` and `dst`. This check is done under `.LBB0_1` which will continue to `.LBB0_5` if no overlap is detected. At a high level, the check is `if (dst &gt;= src + size * 16 - 12 || src &gt;= dst + size * 4) goto .LBB0_5`. This branch is where the vectorized instructions are emitted. Effectively, `.LBB0_5` *should* add four initial negative offsets to the first multiple of four elements of src (`src[i]`, `src[(i+1) * 4]`, `src[(i+2) * 4]`, `src[(i+3) * 4]`, then `.LBB0_6` iterates through each `dst` and multiple of `src` then add and store them back to `dst`, but I think the offset calculation is slightly off.

Added some comments to the relevant bits:
```
.LBB0_5: 
        adr     r3, .LCPI0_0
        vldrw.u32       q0, [r3]         // load the 4 32-bit vals into q0
        vadd.i32        q0, q0, r2       // add each val against src
                                         //   q0.0 = src - 256
                                         //   q0.1 = src - 192
                                         //   q0.2 = src - 128
                                         //   q0.3 = src - 64
        dlstp.32        lr, r1           // lr = size
.LBB0_6: 
        vldrw.u32       q1, [q0, #<!-- -->256]!  // load the 4 32-bit vals from q0 into q1 + 256 and q1 += 256
                                         //   q1.0 = *(src)
                                         //   q1.1 = *(src + 64)
                                         //   q1.2 = *(src + 128)
                                         //   q1.3 = *(src + 192)
        vldrw.u32       q2, [r0]         // load 4 32-bit values from dst
        vadd.f32        q1, q1, q2       // add each 32-bit val from src and dst
                                         //   q1.0 = *(src) + dst[0]
                                         //   q1.1 = *(src + 64) + dst[1]
                                         //   q1.2 = *(src + 128) + dst[2]
                                         //   q1.3 = *(src + 192) + dst[3]
        vstrw.32        q1, [r0], #<!-- -->16    // store the 4 32-bit vals into dst then dst += 16
...
.LCPI0_0:
        .long   4294967040                      @ 0xffffff00 // -256
        .long   4294967104                      @ 0xffffff40 // -192
        .long   4294967168                      @ 0xffffff80 // -128
        .long   4294967232                      @ 0xffffffc0 // -64
```

Each of the offsets for the element accesses are 64 bytes apart when I think they should instead be 16 bytes since the access is `src[i * 4]` and `src` points to floats which should be 4 bytes. This can cause the `vldrw.u32 q1, [q0, #<!-- -->256]!` to access bad/uninitialized memory. I can verify with

```
__attribute__((optnone, noinline))
void testfunc() {
  constexpr int kSize = 2;
  float dst[kSize] = {1.0f, 2.0f};
  float src[] = {3.0f, -1.0f, -1.0f, -1.0f,
                          4.0f, -1.0f, -1.0f, -1.0f,
                          4.0f, -1.0f, -1.0f, -1.0f,
                          4.0f, -1.0f, -1.0f, -1.0f,
                          12.0f, -1.0f, -1.0f, -1.0f,
  };
  Kernel k;
  k.dst = dst;
  k.src = src;
  func(k, kSize);
}
```

that the vectorized code does indeed access the 16th element (`12.0f`) of `src` rather than the 1st element `4.0f`. Nothing seems to stand out from IR corresponding to the vectorized bits:

```
19:                                               ; preds = %19, %16
  %20 = phi i32 [ 0, %16 ], [ %29, %19 ]
  %21 = phi &lt;4 x i32&gt; [ &lt;i32 0, i32 1, i32 2, i32 3&gt;, %16 ], [ %30, %19 ]
  %22 = tail call &lt;4 x i1&gt; @<!-- -->llvm.get.active.lane.mask.v4i1.i32(i32 %20, i32 %1)
  %23 = getelementptr inbounds nuw float, ptr %5, i32 %20
  %24 = shl nsw &lt;4 x i32&gt; %21, splat (i32 4)
  %25 = getelementptr inbounds nuw i8, ptr %7, &lt;4 x i32&gt; %24
  %26 = tail call &lt;4 x float&gt; @<!-- -->llvm.masked.gather.v4f32.v4p0(&lt;4 x ptr&gt; %25, i32 4, &lt;4 x i1&gt; %22, &lt;4 x float&gt; poison), !tbaa !11, !alias.scope !13
  %27 = tail call &lt;4 x float&gt; @<!-- -->llvm.masked.load.v4f32.p0(ptr %23, i32 4, &lt;4 x i1&gt; %22, &lt;4 x float&gt; poison), !tbaa !11, !alias.scope !16, !noalias !13
  %28 = fadd &lt;4 x float&gt; %26, %27
  tail call void @<!-- -->llvm.masked.store.v4f32.p0(&lt;4 x float&gt; %28, ptr %23, i32 4, &lt;4 x i1&gt; %22), !tbaa !11, !alias.scope !16, !noalias !13
  %29 = add nuw i32 %20, 4
  %30 = add &lt;4 x i32&gt; %21, splat (i32 4)
  %31 = icmp eq i32 %29, %18
  br i1 %31, label %32, label %19, !llvm.loop !18

32:                                               ; preds = %19, %33, %2
  ret void
```

so I would suspect it's a backend issue when lowering.
</details>


---

### Comment 2 - fhahn

cc @davemgreen for MVE codegen

---

### Comment 3 - davemgreen

Thanks for the report - it looks like the gep index type is wrong. I'll put a patch together.

Let us know if you find any other issues.

---

