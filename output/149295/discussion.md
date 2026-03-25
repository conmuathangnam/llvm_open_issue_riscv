# [Mips] NaN-related regressions on hard float in LLVM 21

**Author:** alexrp
**URL:** https://github.com/llvm/llvm-project/issues/149295
**Status:** Closed
**Labels:** backend:MIPS, miscompilation, release:backport, regression:21
**Closed Date:** 2025-07-28T08:36:42Z

## Body

In Zig, we're seeing these test failures on all hard float MIPS targets with LLVM 21:

```
test-std
└─ run test std-mips-linux-gnueabihf-mips32r2-Debug-libc 2834/2926 passed, 5 failed, 87 skipped
error: 'fmt.test.float.special' failed: ====== expected this output: =========
f64: -nan␃

======== instead found this: =========
f64: nan␃

======================================
First difference occurs on line 1:
expected:
f64: -nan
     ^ ('\x2d')
found:
f64: nan
     ^ ('\x6e')
/home/alexrp/Source/ziglang/zig-llvm21/lib/std/testing.zig:672:9: 0xcbd977 in expectEqualStrings (test)
        return error.TestExpectedEqual;
        ^
/home/alexrp/Source/ziglang/zig-llvm21/lib/std/testing.zig:279:5: 0x15b609f in expectFmt__anon_206405 (test)
    return expectEqualStrings(expected, actual);
    ^
/home/alexrp/Source/ziglang/zig-llvm21/lib/std/fmt.zig:1001:9: 0x15b712f in test.float.special (test)
        try expectFmt("f64: -nan", "f64: {}", .{-math.nan(f64)});
        ^
error: 'fmt.test.float.hexadecimal.special' failed: ====== expected this output: =========
f64: -nan␃

======== instead found this: =========
f64: nan␃

======================================
First difference occurs on line 1:
expected:
f64: -nan
     ^ ('\x2d')
found:
f64: nan
     ^ ('\x6e')
/home/alexrp/Source/ziglang/zig-llvm21/lib/std/testing.zig:672:9: 0xcbd977 in expectEqualStrings (test)
        return error.TestExpectedEqual;
        ^
/home/alexrp/Source/ziglang/zig-llvm21/lib/std/testing.zig:279:5: 0x15b800f in expectFmt__anon_206508 (test)
    return expectEqualStrings(expected, actual);
    ^
/home/alexrp/Source/ziglang/zig-llvm21/lib/std/fmt.zig:1012:9: 0x15b9bff in test.float.hexadecimal.special (test)
        try expectFmt("f64: -nan", "f64: {x}", .{-math.nan(f64)});
        ^
error: 'math.isnan.decltest.isSignalNan' failed: /home/alexrp/Source/ziglang/zig-llvm21/lib/std/testing.zig:606:14: 0xcc1713 in expect (test)
    if (!ok) return error.TestUnexpectedResult;
             ^
/home/alexrp/Source/ziglang/zig-llvm21/lib/std/math/isnan.zig:43:9: 0x19f119f in decltest.isSignalNan (test)
        try expect(!isSignalNan(math.nan(T)));
        ^
error: 'math.copysign.decltest.copysign' failed: /home/alexrp/Source/ziglang/zig-llvm21/lib/std/testing.zig:606:14: 0xcc1713 in expect (test)
    if (!ok) return error.TestUnexpectedResult;
             ^
/home/alexrp/Source/ziglang/zig-llvm21/lib/std/math/copysign.zig:23:9: 0x1a0985f in decltest.copysign (test)
        try expect(copysign(@as(T, 6.0), -math.nan(T)) == -6.0);
        ^
error: 'math.signbit.decltest.signbit' failed: /home/alexrp/Source/ziglang/zig-llvm21/lib/std/testing.zig:606:14: 0xcc1713 in expect (test)
    if (!ok) return error.TestUnexpectedResult;
             ^
/home/alexrp/Source/ziglang/zig-llvm21/lib/std/math/signbit.zig:49:5: 0x1a34153 in testFloats__anon_341654 (test)
    try expect(signbit(-math.nan(Type)));
    ^
/home/alexrp/Source/ziglang/zig-llvm21/lib/std/math/signbit.zig:27:5: 0x1a364df in decltest.signbit (test)
    try testFloats(f32);
    ^
```

I'll attempt to reduce these failures soon; just filing the issue in advance.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-mips

Author: Alex Rønne Petersen (alexrp)

<details>
In Zig, we're seeing these test failures on all hard float MIPS targets with LLVM 21:

```
test-std
└─ run test std-mips-linux-gnueabihf-mips32r2-Debug-libc 2834/2926 passed, 5 failed, 87 skipped
error: 'fmt.test.float.special' failed: ====== expected this output: =========
f64: -nan␃

======== instead found this: =========
f64: nan␃

======================================
First difference occurs on line 1:
expected:
f64: -nan
     ^ ('\x2d')
found:
f64: nan
     ^ ('\x6e')
/home/alexrp/Source/ziglang/zig-llvm21/lib/std/testing.zig:672:9: 0xcbd977 in expectEqualStrings (test)
        return error.TestExpectedEqual;
        ^
/home/alexrp/Source/ziglang/zig-llvm21/lib/std/testing.zig:279:5: 0x15b609f in expectFmt__anon_206405 (test)
    return expectEqualStrings(expected, actual);
    ^
/home/alexrp/Source/ziglang/zig-llvm21/lib/std/fmt.zig:1001:9: 0x15b712f in test.float.special (test)
        try expectFmt("f64: -nan", "f64: {}", .{-math.nan(f64)});
        ^
error: 'fmt.test.float.hexadecimal.special' failed: ====== expected this output: =========
f64: -nan␃

======== instead found this: =========
f64: nan␃

======================================
First difference occurs on line 1:
expected:
f64: -nan
     ^ ('\x2d')
found:
f64: nan
     ^ ('\x6e')
/home/alexrp/Source/ziglang/zig-llvm21/lib/std/testing.zig:672:9: 0xcbd977 in expectEqualStrings (test)
        return error.TestExpectedEqual;
        ^
/home/alexrp/Source/ziglang/zig-llvm21/lib/std/testing.zig:279:5: 0x15b800f in expectFmt__anon_206508 (test)
    return expectEqualStrings(expected, actual);
    ^
/home/alexrp/Source/ziglang/zig-llvm21/lib/std/fmt.zig:1012:9: 0x15b9bff in test.float.hexadecimal.special (test)
        try expectFmt("f64: -nan", "f64: {x}", .{-math.nan(f64)});
        ^
error: 'math.isnan.decltest.isSignalNan' failed: /home/alexrp/Source/ziglang/zig-llvm21/lib/std/testing.zig:606:14: 0xcc1713 in expect (test)
    if (!ok) return error.TestUnexpectedResult;
             ^
/home/alexrp/Source/ziglang/zig-llvm21/lib/std/math/isnan.zig:43:9: 0x19f119f in decltest.isSignalNan (test)
        try expect(!isSignalNan(math.nan(T)));
        ^
error: 'math.copysign.decltest.copysign' failed: /home/alexrp/Source/ziglang/zig-llvm21/lib/std/testing.zig:606:14: 0xcc1713 in expect (test)
    if (!ok) return error.TestUnexpectedResult;
             ^
/home/alexrp/Source/ziglang/zig-llvm21/lib/std/math/copysign.zig:23:9: 0x1a0985f in decltest.copysign (test)
        try expect(copysign(@<!-- -->as(T, 6.0), -math.nan(T)) == -6.0);
        ^
error: 'math.signbit.decltest.signbit' failed: /home/alexrp/Source/ziglang/zig-llvm21/lib/std/testing.zig:606:14: 0xcc1713 in expect (test)
    if (!ok) return error.TestUnexpectedResult;
             ^
/home/alexrp/Source/ziglang/zig-llvm21/lib/std/math/signbit.zig:49:5: 0x1a34153 in testFloats__anon_341654 (test)
    try expect(signbit(-math.nan(Type)));
    ^
/home/alexrp/Source/ziglang/zig-llvm21/lib/std/math/signbit.zig:27:5: 0x1a364df in decltest.signbit (test)
    try testFloats(f32);
    ^
```

I'll attempt to reduce these failures soon; just filing the issue in advance.
</details>


---

### Comment 2 - alexrp

```llvm
; ModuleID = 'BitcodeBuffer'
source_filename = "repro"
target datalayout = "E-m:m-p:32:32-i8:8:32-i16:16:32-i64:64-n32-S64"
target triple = "mips-unknown-linux5.10.0-eabihf"

@builtin.zig_backend = internal unnamed_addr constant i64 2, align 8
@start.simplified_logic = internal unnamed_addr constant i1 false, align 1
@builtin.output_mode = internal unnamed_addr constant i2 -2, align 1

; Function Attrs: noredzone nounwind uwtable
define dso_local zeroext i1 @repro() #0 {
  %1 = call fastcc i1 @math.isnan.isSignalNan__anon_561(double 0x7FF8000000000000)
  ret i1 %1
}

; Function Attrs: noredzone nounwind uwtable
define internal fastcc i1 @math.isnan.isSignalNan__anon_561(double %0) unnamed_addr #0 {
  %2 = call fastcc i1 @math.isnan.isNan__anon_575(double %0)
  br i1 %2, label %5, label %9

3:                                                ; preds = %9, %5
  %4 = phi i1 [ %8, %5 ], [ false, %9 ]
  ret i1 %4

5:                                                ; preds = %1
  %6 = bitcast double %0 to i64
  %7 = and i64 %6, 2251799813685248
  %8 = icmp eq i64 %7, 0
  br label %3

9:                                                ; preds = %1
  br label %3
}

; Function Attrs: noredzone nounwind uwtable
define internal fastcc i1 @math.isnan.isNan__anon_575(double %0) unnamed_addr #0 {
  %2 = fcmp une double %0, %0
  ret i1 %2
}

attributes #0 = { noredzone nounwind uwtable "frame-pointer"="all" "target-cpu"="mips32r2" "target-features"="+mips1,+mips2,+mips32,+mips32r2,+mips3_32,+mips3_32r2,+mips4_32,+mips4_32r2,+mips5_32r2,-abs2008,-cnmips,-cnmipsp,-crc,-dsp,-dspr2,-dspr3,-eva,-fp64,-fpxx,-ginv,-gp64,-i6400,-i6500,-long-calls,-micromips,-mips16,-mips3,-mips32r3,-mips32r5,-mips32r6,-mips3d,-mips4,-mips5,-mips64,-mips64r2,-mips64r3,-mips64r5,-mips64r6,-msa,-mt,-nan2008,-noabicalls,-nomadd4,-nooddspreg,-p5600,-ptr64,-single-float,-soft-float,-strict-align,-sym32,-use-indirect-jump-hazard,-use-tcc-in-div,-vfpu,-virt,-xgot" }

!llvm.module.flags = !{!0}

!0 = !{i32 1, !"target-abi", [3 x i8] c"o32"}
```

Diff of LLVM 20 vs 21 assembly (irrelevant parts omitted):

```diff
--- repro.20.s  2025-07-22 17:32:15.627864410 +0200
+++ repro.21.s  2025-07-22 17:32:22.272150432 +0200
@@ -7,7 +7,7 @@
        .section        .rodata.cst8,"aM",@progbits,8
        .p2align        3, 0x0
 $CPI0_0:
-       .8byte  0x7ff8000000000000
+       .8byte  0x7ff4000000000000
        .text
        .globl  repro
        .p2align        2
```

#139829 seems potentially relevant here. cc @anbbna @wzssyqa

---

### Comment 3 - alexrp

I can confirm that reverting #139829 makes all of Zig's module tests pass on all MIPS targets. Notably, in addition to the failures reported here, the revert also fixes some related-looking `fp128` failures that only occur on 64-bit.

---

### Comment 4 - brad0

cc @yingopq 

---

### Comment 5 - nikic

/cherry-pick 525090e83ca392753d371602b5e64f06e7debd9a

---

### Comment 6 - llvmbot

/pull-request llvm/llvm-project#150902

---

### Comment 7 - yingopq

OK, I am analyzing this issue and 100495, and would find another solution instead of pr https://github.com/llvm/llvm-project/pull/139829.

---

### Comment 8 - nikic

@yingopq The correct way to handle this is to generate the correct NaN in the first place. But I'm not sure how much interest there would be in really supporting legacy NaNs in LLVM...

---

### Comment 9 - yingopq

> [@yingopq](https://github.com/yingopq) The correct way to handle this is to generate the correct NaN in the first place. But I'm not sure how much interest there would be in really supporting legacy NaNs in LLVM...

OK, my current idea is to fix the display problem first. https://github.com/llvm/llvm-project/pull/153777

---

