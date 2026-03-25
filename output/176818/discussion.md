# [LoongArch] Zig's `log2` tests started failing with LLVM 22

**Author:** alexrp
**URL:** https://github.com/llvm/llvm-project/issues/176818
**Status:** Closed
**Labels:** miscompilation, backend:loongarch, regression:22
**Closed Date:** 2026-01-20T06:53:16Z

## Body

We only see these on LoongArch:

```
test-modules
└─ test-behavior
   └─ run test behavior-loongarch64-linux-none-la64v1_0-Debug 4 pass, 1 skip, 3 fail (8 total)
error: 'behavior.floatop.test.@log2 f16' failed:
       /home/alexrp/Sources/ziglang/zig-llvm22/lib/std/testing.zig:615:14: 0x11b5be3 in expect (test)
           if (!ok) return error.TestUnexpectedResult;
                    ^
       /home/alexrp/Sources/ziglang/zig-llvm22/test/behavior/floatop.zig:954:5: 0x11b5df7 in testLog2__anon_35916 (test)
           try expect(math.approxEqAbs(T, @log2(six), 2.5849625007212, eps));
           ^
       /home/alexrp/Sources/ziglang/zig-llvm22/test/behavior/floatop.zig:919:5: 0x11b60f3 in test.@log2 f16 (test)
           try testLog2(f16);
           ^
error: 'behavior.floatop.test.@log2 f32/f64' failed:
       /home/alexrp/Sources/ziglang/zig-llvm22/lib/std/testing.zig:615:14: 0x11b5be3 in expect (test)
           if (!ok) return error.TestUnexpectedResult;
                    ^
       /home/alexrp/Sources/ziglang/zig-llvm22/test/behavior/floatop.zig:954:5: 0x11b62d7 in testLog2__anon_35977 (test)
           try expect(math.approxEqAbs(T, @log2(six), 2.5849625007212, eps));
           ^
       /home/alexrp/Sources/ziglang/zig-llvm22/test/behavior/floatop.zig:929:5: 0x11b689f in test.@log2 f32/f64 (test)
           try testLog2(f32);
           ^
error: 'behavior.floatop.test.@log2 with vectors' failed:
       /home/alexrp/Sources/ziglang/zig-llvm22/lib/std/testing.zig:615:14: 0x11b5be3 in expect (test)
           if (!ok) return error.TestUnexpectedResult;
                    ^
       /home/alexrp/Sources/ziglang/zig-llvm22/test/behavior/floatop.zig:979:5: 0x11b7a8f in testLog2WithVectors (test)
           try expect(@log2(@as(f32, 1.1)) == result[0]);
           ^
       /home/alexrp/Sources/ziglang/zig-llvm22/test/behavior/floatop.zig:971:5: 0x11b7c73 in test.@log2 with vectors (test)
           try testLog2WithVectors();
           ^
failed command: qemu-loongarch64 ./.zig-cache/o/8fef89cfb464b38c352badafc91870c8/test --cache-dir=./.zig-cache --seed=0xa8633959 --listen=-

test-modules
└─ test-std
   └─ run test std-loongarch64-linux-none-la64v1_0-Debug 67 pass, 1 fail (68 total)
error: 'math.log2.decltest.log2' failed:
       /home/alexrp/Sources/ziglang/zig-llvm22/lib/std/testing.zig:615:14: 0x1231423 in expect (test)
           if (!ok) return error.TestUnexpectedResult;
                    ^
       /home/alexrp/Sources/ziglang/zig-llvm22/lib/std/math/log2.zig:47:5: 0x1234017 in decltest.log2 (test)
           try expect(log2(@as(f32, 0.2)) == @log2(0.2));
           ^
failed command: qemu-loongarch64 ./.zig-cache/o/c7cad71a90740cec4662186077312aeb/test --cache-dir=./.zig-cache --seed=0xa8633959 --listen=-
```

I suspect this may have been regressed by https://github.com/llvm/llvm-project/pull/162978.

Will provide a reduced repro soon.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-loongarch

Author: Alex Rønne Petersen (alexrp)

<details>
We only see these on LoongArch:

```
test-modules
└─ test-behavior
   └─ run test behavior-loongarch64-linux-none-la64v1_0-Debug 4 pass, 1 skip, 3 fail (8 total)
error: 'behavior.floatop.test.@<!-- -->log2 f16' failed:
       /home/alexrp/Sources/ziglang/zig-llvm22/lib/std/testing.zig:615:14: 0x11b5be3 in expect (test)
           if (!ok) return error.TestUnexpectedResult;
                    ^
       /home/alexrp/Sources/ziglang/zig-llvm22/test/behavior/floatop.zig:954:5: 0x11b5df7 in testLog2__anon_35916 (test)
           try expect(math.approxEqAbs(T, @<!-- -->log2(six), 2.5849625007212, eps));
           ^
       /home/alexrp/Sources/ziglang/zig-llvm22/test/behavior/floatop.zig:919:5: 0x11b60f3 in test.@<!-- -->log2 f16 (test)
           try testLog2(f16);
           ^
error: 'behavior.floatop.test.@<!-- -->log2 f32/f64' failed:
       /home/alexrp/Sources/ziglang/zig-llvm22/lib/std/testing.zig:615:14: 0x11b5be3 in expect (test)
           if (!ok) return error.TestUnexpectedResult;
                    ^
       /home/alexrp/Sources/ziglang/zig-llvm22/test/behavior/floatop.zig:954:5: 0x11b62d7 in testLog2__anon_35977 (test)
           try expect(math.approxEqAbs(T, @<!-- -->log2(six), 2.5849625007212, eps));
           ^
       /home/alexrp/Sources/ziglang/zig-llvm22/test/behavior/floatop.zig:929:5: 0x11b689f in test.@<!-- -->log2 f32/f64 (test)
           try testLog2(f32);
           ^
error: 'behavior.floatop.test.@<!-- -->log2 with vectors' failed:
       /home/alexrp/Sources/ziglang/zig-llvm22/lib/std/testing.zig:615:14: 0x11b5be3 in expect (test)
           if (!ok) return error.TestUnexpectedResult;
                    ^
       /home/alexrp/Sources/ziglang/zig-llvm22/test/behavior/floatop.zig:979:5: 0x11b7a8f in testLog2WithVectors (test)
           try expect(@<!-- -->log2(@<!-- -->as(f32, 1.1)) == result[0]);
           ^
       /home/alexrp/Sources/ziglang/zig-llvm22/test/behavior/floatop.zig:971:5: 0x11b7c73 in test.@<!-- -->log2 with vectors (test)
           try testLog2WithVectors();
           ^
failed command: qemu-loongarch64 ./.zig-cache/o/8fef89cfb464b38c352badafc91870c8/test --cache-dir=./.zig-cache --seed=0xa8633959 --listen=-

test-modules
└─ test-std
   └─ run test std-loongarch64-linux-none-la64v1_0-Debug 67 pass, 1 fail (68 total)
error: 'math.log2.decltest.log2' failed:
       /home/alexrp/Sources/ziglang/zig-llvm22/lib/std/testing.zig:615:14: 0x1231423 in expect (test)
           if (!ok) return error.TestUnexpectedResult;
                    ^
       /home/alexrp/Sources/ziglang/zig-llvm22/lib/std/math/log2.zig:47:5: 0x1234017 in decltest.log2 (test)
           try expect(log2(@<!-- -->as(f32, 0.2)) == @<!-- -->log2(0.2));
           ^
failed command: qemu-loongarch64 ./.zig-cache/o/c7cad71a90740cec4662186077312aeb/test --cache-dir=./.zig-cache --seed=0xa8633959 --listen=-
```

I suspect this may have been regressed by https://github.com/llvm/llvm-project/pull/162978.

Will provide a reduced repro soon.
</details>


---

### Comment 2 - alexrp

```llvm
; ModuleID = 'floatop'
source_filename = "floatop"
target datalayout = "e-m:e-p:64:64-i64:64-i128:128-n32:64-S128"
target triple = "loongarch64-unknown-linux5.19.0-musl"

%SemanticVersion = type { i64, i64, i64, { ptr, i64 }, { ptr, i64 } }
%SemanticVersion.Range = type { %SemanticVersion, %SemanticVersion }
%Target.Os.LinuxVersionRange = type { %SemanticVersion.Range, %SemanticVersion, i32, [4 x i8] }
%Target.Os.VersionRange = type { %Target.Os.LinuxVersionRange, i3, [7 x i8] }
%Target.Os = type { %Target.Os.VersionRange, i6, [7 x i8] }
%Target.Cpu.Feature.Set = type { [6 x i64] }
%Target.Cpu = type { ptr, %Target.Cpu.Feature.Set, i6, [7 x i8] }
%Target.Cpu.Model = type { { ptr, i64 }, { ptr, i64 }, %Target.Cpu.Feature.Set }
%Target.DynamicLinker = type { [255 x i8], i8 }

@builtin.output_mode = internal unnamed_addr constant i2 0, align 1
@builtin.link_libc = internal unnamed_addr constant i1 false, align 1
@builtin.os = internal unnamed_addr constant %Target.Os { %Target.Os.VersionRange { %Target.Os.LinuxVersionRange { %SemanticVersion.Range { %SemanticVersion { i64 5, i64 19, i64 0, { ptr, i64 } zeroinitializer, { ptr, i64 } zeroinitializer }, %SemanticVersion { i64 6, i64 17, i64 0, { ptr, i64 } zeroinitializer, { ptr, i64 } zeroinitializer } }, %SemanticVersion { i64 2, i64 36, i64 0, { ptr, i64 } zeroinitializer, { ptr, i64 } zeroinitializer }, i32 29, [4 x i8] undef }, i3 3, [7 x i8] undef }, i6 9, [7 x i8] undef }, align 8
@start.native_os = internal unnamed_addr constant i6 9, align 1
@Target.Cpu.Feature.Set.empty = internal unnamed_addr constant %Target.Cpu.Feature.Set zeroinitializer, align 8
@builtin.cpu = internal unnamed_addr constant %Target.Cpu { ptr getelementptr inbounds (i8, ptr @Target.loongarch.cpu.la64v1_0, i64 0), %Target.Cpu.Feature.Set { [6 x i64] [i64 1081390, i64 0, i64 0, i64 0, i64 0, i64 0] }, i6 19, [7 x i8] undef }, align 8
@Target.loongarch.cpu.la64v1_0 = internal unnamed_addr constant %Target.Cpu.Model { { ptr, i64 } { ptr getelementptr inbounds (i8, ptr @__anon_1670, i64 0), i64 8 }, { ptr, i64 } zeroinitializer, %Target.Cpu.Feature.Set { [6 x i64] [i64 1081348, i64 0, i64 0, i64 0, i64 0, i64 0] } }, align 8
@start.native_arch = internal unnamed_addr constant i6 19, align 1
@start.is_wasm = internal unnamed_addr constant i1 false, align 1
@start.start_sym_name = internal unnamed_addr constant ptr getelementptr inbounds (i8, ptr @__anon_877, i64 0), align 8
@__anon_877 = internal unnamed_addr constant [7 x i8] c"_start\00", align 1
@Target.DynamicLinker.none = internal unnamed_addr constant %Target.DynamicLinker { [255 x i8] undef, i8 0 }, align 1
@os.linux.native_arch = internal unnamed_addr constant i6 19, align 1
@__anon_1670 = internal unnamed_addr constant [9 x i8] c"la64v1_0\00", align 1

; Function Attrs: noredzone nounwind
define dso_local void @_start() #0 {
0:
  %1 = alloca float, align 4
  store float 0x4018000000000000, ptr %1, align 4
  %2 = load float, ptr %1, align 4
  %3 = call float @llvm.log2.f32(float %2)
  %4 = call fastcc i1 @math.approxEqAbs__anon_922(float %3, float 0x4004AE00E0000000, float 0x3EB0C6F7A0000000)
  %5 = xor i1 %4, true
  %6 = zext i1 %5 to i8
  %7 = zext i8 %6 to i32
  call fastcc void @os.linux.exit_group(i32 %7)
  unreachable
}

; Function Attrs: nounwind speculatable willreturn nofree nosync nocallback memory(none)
declare float @llvm.log2.f32(float %0) #1

; Function Attrs: noredzone nounwind
define internal fastcc i1 @math.approxEqAbs__anon_922(float %0, float %1, float %2) unnamed_addr #0 {
3:
  call fastcc void @debug.assert(i1 true)
  %4 = fcmp oge float %2, 0x0000000000000000
  call fastcc void @debug.assert(i1 %4)
  %5 = fcmp oeq float %0, %1
  br i1 %5, label %8, label %9

6:
  %7 = call fastcc i1 @math.isnan.isNan__anon_944(float %0)
  br i1 %7, label %16, label %17

8:
  ret i1 true

9:
  br label %6

10:
  %11 = fsub float %0, %1
  %12 = call float @llvm.fabs.f32(float %11)
  %13 = fcmp ole float %12, %2
  ret i1 %13

14:
  %15 = phi i1 [ true, %16 ], [ %18, %17 ]
  br i1 %15, label %19, label %20

16:
  br label %14

17:
  %18 = call fastcc i1 @math.isnan.isNan__anon_944(float %1)
  br label %14

19:
  ret i1 false

20:
  br label %10
}

; Function Attrs: noredzone noreturn nounwind cold
define internal fastcc void @os.linux.exit_group(i32 %0) unnamed_addr #2 {
1:
  %2 = sext i32 %0 to i64
  %3 = call fastcc i64 @os.linux.loongarch64.syscall1(i64 94, i64 %2)
  call fastcc void @debug.no_panic.reachedUnreachable()
  unreachable
}

; Function Attrs: noredzone nounwind
define internal fastcc void @debug.assert(i1 %0) unnamed_addr #0 {
1:
  %2 = xor i1 %0, true
  br i1 %2, label %4, label %5

3:
  ret void

4:
  call void @llvm.assume(i1 true) [ "cold"() ]
  call fastcc void @debug.no_panic.reachedUnreachable()
  unreachable

5:
  br label %3
}

; Function Attrs: noredzone nounwind
define internal fastcc i1 @math.isnan.isNan__anon_944(float %0) unnamed_addr #0 {
1:
  %2 = fcmp une float %0, %0
  ret i1 %2
}

; Function Attrs: nounwind speculatable willreturn nofree nosync nocallback memory(none)
declare float @llvm.fabs.f32(float %0) #1

; Function Attrs: noredzone nounwind
define internal fastcc i64 @os.linux.loongarch64.syscall1(i64 %0, i64 %1) unnamed_addr #0 {
2:
  %3 = call i64 asm sideeffect " syscall 0", "={$r4},{$r11},{$r4},~{memory},~{r12},~{r13},~{r14},~{r15},~{r16},~{r17},~{r18},~{r19},~{r20}"(i64 %0, i64 %1)
  ret i64 %3
}

; Function Attrs: noredzone noreturn nounwind cold
define internal fastcc void @debug.no_panic.reachedUnreachable() unnamed_addr #2 {
0:
  call void @llvm.trap()
  unreachable
}

; Function Attrs: nounwind willreturn nofree nosync nocallback memory(inaccessiblemem: write)
declare void @llvm.assume(i1 noundef %0) #3

; Function Attrs: noreturn nounwind cold memory(inaccessiblemem: write)
declare void @llvm.trap() #4

attributes #0 = { noredzone nounwind "frame-pointer"="all" "target-features"="+32s,+64bit,+d,+f,+lsx,+ual,-32bit,-div32,-frecipe,-la-global-with-abs,-la-global-with-pcrel,-la-local-with-abs,-lam-bh,-lamcas,-lasx,-lbt,-ld-seq-sa,-lvz,-prefer-w-inst,-relax,-scq" }
attributes #1 = { nounwind speculatable willreturn nofree nosync nocallback memory(none) }
attributes #2 = { noredzone noreturn nounwind cold "frame-pointer"="all" "target-features"="+32s,+64bit,+d,+f,+lsx,+ual,-32bit,-div32,-frecipe,-la-global-with-abs,-la-global-with-pcrel,-la-local-with-abs,-lam-bh,-lamcas,-lasx,-lbt,-ld-seq-sa,-lvz,-prefer-w-inst,-relax,-scq" }
attributes #3 = { nounwind willreturn nofree nosync nocallback memory(inaccessiblemem: write) }
attributes #4 = { noreturn nounwind cold memory(inaccessiblemem: write) }

!llvm.module.flags = !{!0}

!0 = !{i32 1, !"target-abi", !"lp64d"}
```

```
❯ llc --version | head -n2
LLVM (http://llvm.org/):
  LLVM version 22.1.0-rc1
❯ llc floatop.ll -filetype obj
❯ ld.lld floatop.o
❯ qemu-loongarch64 ./a.out
❯ echo $status
1
```

(Should exit with code zero if compiled correctly.)

With LLVM 21, the `@llvm.log2.f32` intrinsic turns into a `log2f` libcall. With 22, it turns into an `flogb.s` instruction. So I feel pretty confident that https://github.com/llvm/llvm-project/pull/162978 is related.

---

### Comment 3 - wangleiat

Thanks.
The semantics of `flog2(x)` and `logb(x)`(`flogb*`) are different, so `flogb*` instructions cannot be used directly.
I will revert #162978 and cherry-pick it to the llvm-22.x branch.


---

