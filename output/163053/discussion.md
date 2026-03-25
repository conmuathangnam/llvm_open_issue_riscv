# [clang] Miscompiles OpenSSH 10.x's mlkem768 key-exchange protocol when avx512 is enabled.

**Author:** iucoen
**URL:** https://github.com/llvm/llvm-project/issues/163053
**Status:** Closed
**Labels:** backend:X86, miscompilation, release:backport
**Closed Date:** 2025-10-18T11:46:11Z

## Body

When OpenSSH 10.x is compiled with Clang (I've tested versions 19 and 20) and using `-march=x86-64-v4 -mtune=znver5` to enable AVX512, the resulting ssh client binary is unable to establish SSH connections and reports "incorrect signature" when the "mlkem768x25519-sha256" kex algorithm is used. I've narrowed down to the `libcrux_ml_kem_mlkem768_portable_decapsulate()` function that's misbehaving and returning the wrong shared secret. It also appears that the CFLAG `-fzero-call-used-regs=used` which is added by OpenSSH's build system is what's causing the problem. I've isolated just the mlkem768 algorithm and produced a small [test case](https://github.com/iucoen/clang-mlkem768-repro) to demonstrate the problem. 

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-x86

Author: None (iucoen)

<details>
When OpenSSH 10.x is compiled with Clang (I've tested versions 19 and 20) and using `-march=x86-64-v4 -mtune=znver5` to enable AVX512, the resulting ssh client binary is unable to establish SSH connections and reports "incorrect signature" when the "mlkem768x25519-sha256" kex algorithm is used. I've narrowed down to the `libcrux_ml_kem_mlkem768_portable_decapsulate()` function that's misbehaving and returning the wrong shared secret. It also appears that the CFLAG `-fzero-call-used-regs=used` which is added by OpenSSH's build system is what's causing the problem. I've isolated just the mlkem768 algorithm and produced a small [test case](https://github.com/iucoen/clang-mlkem768-repro) to demonstrate the problem. 
</details>


---

### Comment 2 - boomanaiden154

Can you add some instructions to the repo on what make commands to run and what the expected behavior is?

Also, have you run the test case under UBSan to help ensure this isn’t just exploited UB?

---

### Comment 3 - iucoen

> Can you add some instructions to the repo on what make commands to run and what the expected behavior is?
> 
Done. Added README

> Also, have you run the test case under UBSan to help ensure this isn’t just exploited UB?
>
When compiled with `-fsanitize=undefined` the test passes and the problem no longer reproduces.


---

### Comment 4 - antoniofrighetto

I'm hitting a different issue with Clang trunk and assertions enabled, possibly related:
```
$ ./bin/clang -O2 -g -Wall -Werror -fzero-call-used-regs=used -march=x86-64-v4 -mtune=znver5 -c -o libcrux_mlkem768_sha3.o libcrux_mlkem768_sha3.c

clang: llvm-project/llvm/lib/Target/X86/MCTargetDesc/X86MCCodeEmitter.cpp:174: void (anonymous namespace)::X86OpcodePrefixHelper::setR2(unsigned int): Assertion `(!R2 || (Kind <= REX2 || Kind == EVEX)) && "invalid setting"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: llvm-project/build/bin/clang -O2 -g -Wall -Werror -fzero-call-used-regs=used -march=x86-64-v4 -mtune=znver5 -c -o libcrux_mlkem768_sha3.o libcrux_mlkem768_sha3.c
1.	<eof> parser at end of file
2.	Code generation
3.	Running pass 'Function Pass Manager' on module 'libcrux_mlkem768_sha3.c'.
4.	Running pass 'X86 Assembly Printer' on function '@libcrux_ml_kem_mlkem768_portable_decapsulate'
```
Will reduce the test case further.

---

### Comment 5 - abhishek-kaushik22

I've managed to reduce to code to this snippet (https://godbolt.org/z/qYYzdn86z), which fails with `llc -filetype=obj`

```llvm
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define void @libcrux_ml_kem_mlkem768_portable_decapsulate(<6 x i1> %0, i1 %condloop.1316.not, <6 x i1> %1) #0 {
entry:
  br label %loop.1316

loop.1316:                                        ; preds = %loop.1316, %entry
  %2 = phi <4 x i8> [ splat (i8 1), %loop.1316 ], [ zeroinitializer, %entry ]
  %3 = phi <16 x i8> [ zeroinitializer, %loop.1316 ], [ splat (i8 1), %entry ]
  %4 = tail call <6 x i8> @llvm.masked.load.v6i8.p0(ptr null, i32 1, <6 x i1> %0, <6 x i8> zeroinitializer)
  %5 = shufflevector <4 x i8> %2, <4 x i8> zeroinitializer, <16 x i32> <i32 0, i32 1, i32 2, i32 3, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison>
  %6 = shufflevector <16 x i8> %3, <16 x i8> zeroinitializer, <16 x i32> <i32 7, i32 8, i32 11, i32 12, i32 13, i32 16, i32 17, i32 18, i32 7, i32 8, i32 11, i32 12, i32 13, i32 16, i32 17, i32 18>
  %7 = lshr <16 x i8> %6, splat (i8 1)
  %8 = shufflevector <16 x i8> %7, <16 x i8> zeroinitializer, <16 x i32> <i32 16, i32 17, i32 18, i32 19, i32 20, i32 21, i32 0, i32 1, i32 24, i32 2, i32 3, i32 4, i32 28, i32 5, i32 6, i32 7>
  %9 = zext <16 x i8> %8 to <16 x i32>
  %10 = shufflevector <16 x i8> splat (i8 1), <16 x i8> %5, <16 x i32> <i32 poison, i32 poison, i32 poison, i32 4, i32 poison, i32 poison, i32 poison, i32 9, i32 poison, i32 poison, i32 poison, i32 14, i32 poison, i32 poison, i32 poison, i32 19>
  %11 = and <6 x i8> %4, splat (i8 1)
  %12 = shufflevector <6 x i8> %11, <6 x i8> zeroinitializer, <16 x i32> <i32 0, i32 poison, i32 3, i32 4, i32 5, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison>
  %13 = shufflevector <16 x i8> %10, <16 x i8> %12, <16 x i32> <i32 poison, i32 poison, i32 poison, i32 3, i32 poison, i32 poison, i32 poison, i32 7, i32 poison, i32 poison, i32 16, i32 11, i32 18, i32 19, i32 20, i32 15>
  %14 = and <16 x i8> %3, splat (i8 1)
  %15 = shufflevector <16 x i8> %14, <16 x i8> %13, <16 x i32> <i32 0, i32 1, i32 2, i32 19, i32 3, i32 4, i32 5, i32 23, i32 6, i32 7, i32 26, i32 27, i32 28, i32 29, i32 30, i32 31>
  %16 = zext <16 x i8> %15 to <16 x i32>
  %17 = shl <16 x i32> %16, <i32 8, i32 6, i32 4, i32 2, i32 8, i32 6, i32 4, i32 2, i32 8, i32 6, i32 4, i32 2, i32 8, i32 6, i32 4, i32 2>
  %18 = or <16 x i32> %17, %9
  %19 = or <16 x i32> %18, splat (i32 1)
  %20 = trunc <16 x i32> %19 to <16 x i16>
  store <16 x i16> %20, ptr null, align 2
  br i1 %condloop.1316.not, label %afterloop.1316, label %loop.1316

afterloop.1316:                                   ; preds = %loop.1316
  ret void
}

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(argmem: read)
declare <6 x i8> @llvm.masked.load.v6i8.p0(ptr captures(none), i32 immarg, <6 x i1>, <6 x i8>) #1

attributes #0 = { "target-cpu"="x86-64-v4" "zero-call-used-regs"="used" }
attributes #1 = { nocallback nofree nosync nounwind willreturn memory(argmem: read) }
```

I would like to take up this issue as it also affects our downstream compiler.

---

### Comment 6 - iucoen

Ah I see what happened here. So when I compile with `-no-integrated-as` I actually get this error from gnu as:
```
Error: operand size mismatch for `vpxor'
```
Should the clang integrate assembler also fail instead of coding the wrong instruction?

---

### Comment 7 - boomanaiden154

There's an assertion in `AsmPrinter`, so it fails even before that (assuming you have assertions enabled). Not sure about all the error handling in LLVM's MC layer, but it will generally reject things (or at least `llvm-mc` does) that it does not think are correct.

---

### Comment 8 - RKSimon

/cherry-pick 228dae786b94bb85fb34bc157a43ca6c16932b6d

---

### Comment 9 - llvmbot

/pull-request llvm/llvm-project#164076

---

