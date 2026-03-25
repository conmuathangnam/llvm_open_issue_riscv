# [SLP] Internal compiler error in SLP module when placing vectorized instructions.

**Author:** offsake
**URL:** https://github.com/llvm/llvm-project/issues/168576
**Status:** Closed
**Labels:** llvm:SLPVectorizer, crash
**Closed Date:** 2025-11-20T01:16:10Z

## Body

Here is the link to input code and trunk opt (doesn't matter w/ or w/o assertions) to reproduce the issue:
https://llvm.godbolt.org/z/c67a4dsrM

The same problem reproduces on 21.1.0 released versions of opt. Compiles fine with 20.1.0. So, may be a stability regression due to new capabilities to vectorize.

Also duplicating the input code and command line here.
`opt -disable-output reduced.ll -passes=slp-vectorizer`

The content of reduced.ll:
```
target triple = "x86_64-unknown-linux-gnu"

define float @_foo() {
label:
  %tmp.0.4.vec.extract = extractelement <4 x float> zeroinitializer, i64 0
  %tmp.0.0.vec.extract = extractelement <4 x float> zeroinitializer, i64 0
  %tmp.12.36.vec.extract = extractelement <4 x float> zeroinitializer, i64 0
  %tmp.7.28.vec.extract = extractelement <4 x float> zeroinitializer, i64 0
  %tmp.0.12.vec.extract = extractelement <4 x float> zeroinitializer, i64 0
  %mul3.i128.i = fmul float 0.000000e+00, 0.000000e+00
  %mul3.i138.i = fmul float 0.000000e+00, 0.000000e+00
  %sub.i102.i = fsub float 0.000000e+00, 0.000000e+00
  %mul.i136.i = fmul float %sub.i102.i, 0.000000e+00
  %v.0.4.vec.extract = extractelement <8 x float> zeroinitializer, i64 0
  %v.0.24.vec.extract = extractelement <8 x float> zeroinitializer, i64 0
  %v.0.28.vec.extract = extractelement <8 x float> zeroinitializer, i64 0
  %v.11.48.vec.extract = extractelement <8 x float> zeroinitializer, i64 0
  %v.20.72.vec.extract = extractelement <8 x float> zeroinitializer, i64 0
  %v.20.76.vec.extract = extractelement <8 x float> zeroinitializer, i64 0
  %0 = fmul float %v.0.4.vec.extract, %tmp.0.0.vec.extract
  %1 = fadd float %0, 0.000000e+00
  %2 = fmul float %v.0.28.vec.extract, 0.000000e+00
  %3 = fmul float %v.0.28.vec.extract, %tmp.0.0.vec.extract
  %4 = fadd float %3, 0.000000e+00
  %5 = fmul float %tmp.0.4.vec.extract, %v.11.48.vec.extract
  %6 = fadd float 0.000000e+00, %5
  %7 = fmul float %v.20.76.vec.extract, %tmp.0.0.vec.extract
  %8 = fadd float %7, 0.000000e+00
  %9 = fmul float 0.000000e+00, %tmp.0.12.vec.extract
  %10 = fadd float %2, %9
  %11 = fadd float %10, 0.000000e+00
  %12 = fsub float 0.000000e+00, %11
  %13 = fadd float 0.000000e+00, %1
  %14 = fadd float 0.000000e+00, %4
  %15 = fadd float 0.000000e+00, %6
  %16 = fadd float 0.000000e+00, %8
  %17 = fmul float 0.000000e+00, 0.000000e+00
  %18 = fmul float 0.000000e+00, %tmp.7.28.vec.extract
  %19 = fmul float 0.000000e+00, 0.000000e+00
  %20 = fmul float 0.000000e+00, 0.000000e+00
  %21 = fmul float 0.000000e+00, %tmp.7.28.vec.extract
  %22 = fmul float 0.000000e+00, 0.000000e+00
  %23 = fmul float 0.000000e+00, %tmp.12.36.vec.extract
  %24 = fadd float %18, %23
  %25 = fmul float 0.000000e+00, %tmp.12.36.vec.extract
  %26 = fadd float %21, %25
  %27 = fsub float 0.000000e+00, poison
  %28 = fadd float %24, 0.000000e+00
  %29 = fsub float 0.000000e+00, %28
  %30 = fadd float %26, 0.000000e+00
  %31 = fsub float 0.000000e+00, %30
  %32 = fadd float 0.000000e+00, %17
  %33 = fadd float 0.000000e+00, %19
  %34 = fadd float 0.000000e+00, %20
  %35 = fadd float 0.000000e+00, %22
  %36 = fmul float 0.000000e+00, %mul3.i138.i
  %37 = fmul float %v.0.4.vec.extract, %mul.i136.i
  %38 = fadd float %37, 0.000000e+00
  %39 = fmul float 0.000000e+00, %mul3.i138.i
  %40 = fmul float %mul3.i138.i, %v.0.24.vec.extract
  %41 = fadd float 0.000000e+00, %40
  %42 = fmul float 0.000000e+00, %mul3.i138.i
  %43 = fmul float 0.000000e+00, %mul3.i138.i
  %44 = fmul float %mul3.i138.i, %v.20.72.vec.extract
  %45 = fadd float 0.000000e+00, %44
  %46 = fmul float 0.000000e+00, 1.000000e+00
  %47 = fmul float 0.000000e+00, %mul3.i128.i
  %48 = fadd float %36, %47
  %49 = fmul float 0.000000e+00, %mul3.i128.i
  %50 = fadd float %39, %49
  %51 = fmul float 0.000000e+00, %mul3.i128.i
  %52 = fadd float %42, %51
  %53 = fmul float 0.000000e+00, %mul3.i128.i
  %54 = fadd float %43, %53
  %55 = fadd float %46, 0.000000e+00
  %56 = fadd float %48, 0.000000e+00
  %57 = fsub float %55, %56
  %58 = fadd float %50, 0.000000e+00
  %59 = fsub float 0.000000e+00, %58
  %60 = fadd float %52, 0.000000e+00
  %61 = fsub float 0.000000e+00, %60
  %62 = fadd float %mul.i136.i, 0.000000e+00
  %63 = fadd float %54, 0.000000e+00
  %64 = fsub float %62, %63
  %65 = fadd float 0.000000e+00, %38
  %66 = fadd float 0.000000e+00, %41
  %67 = fadd float 0.000000e+00, %45
  br label %region.30

region.30:                                        ; preds = %label
  %68 = phi float [ %29, %label ]
  %69 = phi float [ %66, %label ]
  %70 = phi float [ %59, %label ]
  %71 = phi float [ %15, %label ]
  %72 = phi float [ %34, %label ]
  %73 = phi float [ poison, %label ]
  %74 = phi float [ %61, %label ]
  %75 = phi float [ %16, %label ]
  %76 = phi float [ %35, %label ]
  %77 = phi float [ %31, %label ]
  %78 = phi float [ %67, %label ]
  %79 = phi float [ %64, %label ]
  %80 = phi float [ %33, %label ]
  %81 = phi float [ %12, %label ]
  %82 = phi float [ %14, %label ]
  %83 = phi float [ %57, %label ]
  %84 = phi float [ %65, %label ]
  %85 = phi float [ %27, %label ]
  %86 = phi float [ %32, %label ]
  %87 = phi float [ %13, %label ]
  ret float %87
}
```

The expected output:
```
Instruction does not dominate all uses!
  %5 = fmul <4 x float> %4, <float 0.000000e+00, float 0.000000e+00, float 0.000000e+00, float poison>
  %0 = insertelement <4 x float> %5, float 0.000000e+00, i32 1
```

## Comments

### Comment 1 - alexey-bataev

Fixed in 2c3aa92089695713a1fd4264e80941fd9679150b

---

