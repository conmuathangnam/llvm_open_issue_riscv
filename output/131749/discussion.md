# [NVPTX] `sqrtf` always approximated with `llvm.nvvm.sqrt.approx.f`

**Author:** Lai-YT
**URL:** https://github.com/llvm/llvm-project/issues/131749
**Status:** Open
**Labels:** clang:codegen, miscompilation, backend:NVPTX

## Body

I've encountered an issue with the square-root (`sqrtf`) floating-point operation on NVPTX. It always compiles to the `llvm.nvvm.sqrt.approx.f` intrinsic, even when `-fno-approx-func` is specified. I have a hypothesis about the cause and would like to explore a potential fix. Any guidance would be appreciated! 😃 

---

## What's the Problem?

Here's the relevant code:

```cuda
// cat main.cu
__device__ float f(float x) {
  return sqrtf(x);
}
```

The function `sqrtf` always compiles to the `llvm.nvvm.sqrt.approx.f` intrinsic, regardless of `-fno-approx-func`:

```llvm
; clang -Wno-unknown-cuda-version -fno-approx-func main.cu -S -emit-llvm --cuda-device-only -o -
; ModuleID = 'main.cu'
source_filename = "main.cu"
target datalayout = "e-p6:32:32-i64:64-i128:128-v16:16-v32:32-n16:32:64"
target triple = "nvptx64-nvidia-cuda"

@.str = private unnamed_addr constant [11 x i8] c"__CUDA_FTZ\00", align 1
@.str.2 = private unnamed_addr constant [17 x i8] c"__CUDA_PREC_SQRT\00", align 1

; Function Attrs: convergent noinline nounwind optnone
define dso_local noundef float @_Z1ff(float noundef %x) #0 {
entry:
  %__a.addr.i = alloca float, align 4
  %x.addr = alloca float, align 4
  store float %x, ptr %x.addr, align 4
  %0 = load float, ptr %x.addr, align 4
  store float %0, ptr %__a.addr.i, align 4
  %1 = load float, ptr %__a.addr.i, align 4
  %2 = call float @llvm.nvvm.sqrt.approx.f(float %1) #3
  ret float %2
}

; Function Attrs: convergent nounwind
declare i32 @__nvvm_reflect(ptr) #1

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(none)
declare float @llvm.nvvm.sqrt.rn.ftz.f(float) #2

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(none)
declare float @llvm.nvvm.sqrt.approx.ftz.f(float) #2

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(none)
declare float @llvm.nvvm.sqrt.rn.f(float) #2

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(none)
declare float @llvm.nvvm.sqrt.approx.f(float) #2

attributes #0 = { convergent noinline nounwind optnone "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="sm_52" "target-features"="+ptx85,+sm_52" }
attributes #1 = { convergent nounwind "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="sm_52" "target-features"="+ptx85,+sm_52" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nocallback nofree nosync nounwind willreturn memory(none) }
attributes #3 = { nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4, !5}
!nvvmir.version = !{!6}

!0 = !{i32 2, !"SDK Version", [2 x i32] [i32 12, i32 6]}
!1 = !{i32 1, !"wchar_size", i32 4}
!2 = !{i32 4, !"nvvm-reflect-ftz", i32 0}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"clang version 21.0.0git (git@github.com:Lai-YT/llvm-project.git e57cd100ca297cf81854e35cccbf703edddd4aad)"}
!5 = !{!"clang version 3.8.0 (tags/RELEASE_380/final)"}
!6 = !{i32 2, i32 0}
```

## What's the Possible Cause?

As defined in `__clang_cuda_math.h`, `sqrtf` expands to the `__nv_sqrtf` intrinsic:

https://github.com/llvm/llvm-project/blob/2f808dd0702ba3c364eb4373714a1fb09078909d/clang/lib/Headers/__clang_cuda_math.h#L318

I noticed that `__nv_sqrtf` is defined by NVIDIA in _libdevice.bc_, so I grabbed the libdevice on my machine (which is `libdevice.10.bc`) and disassembled the bitcode with _llvm-dis_:

```llvm
@.str = private unnamed_addr constant [11 x i8] c"__CUDA_FTZ\00", align 1
@.str.2 = private unnamed_addr constant [17 x i8] c"__CUDA_PREC_SQRT\00", align 1

...

define float @__nv_sqrtf(float %x) #0 {
  %1 = call i32 @__nvvm_reflect(ptr @.str) #6
  %2 = icmp ne i32 %1, 0
  br i1 %2, label %3, label %10

3:                                                ; preds = %0
  %4 = call i32 @__nvvm_reflect(ptr @.str.2) #6
  %5 = icmp ne i32 %4, 0
  br i1 %5, label %6, label %8

6:                                                ; preds = %3
  %7 = call float @llvm.nvvm.sqrt.rn.ftz.f(float %x) #6
  br label %__nvvm_sqrt_f.exit

8:                                                ; preds = %3
  %9 = call float @llvm.nvvm.sqrt.approx.ftz.f(float %x) #6
  br label %__nvvm_sqrt_f.exit

10:                                               ; preds = %0
  %11 = call i32 @__nvvm_reflect(ptr @.str.2) #6
  %12 = icmp ne i32 %11, 0
  br i1 %12, label %13, label %15

13:                                               ; preds = %10
  %14 = call float @llvm.nvvm.sqrt.rn.f(float %x) #6
  br label %__nvvm_sqrt_f.exit

15:                                               ; preds = %10
  %16 = call float @llvm.nvvm.sqrt.approx.f(float %x) #6
  br label %__nvvm_sqrt_f.exit

__nvvm_sqrt_f.exit:                               ; preds = %6, %8, %13, %15
  %.0 = phi float [ %7, %6 ], [ %9, %8 ], [ %14, %13 ], [ %16, %15 ]
  ret float %.0
}
```

The selection of intrinsic respects the value of `__nvvm_reflect("__CUDA_FTZ")` and `__nvvm_reflect("__CUDA_PREC_SQRT")`. The [_NVVMReflectPass_](llvm/lib/Target/NVPTX/NVVMReflect.cpp) is the pass that picks them up and replaces the `__nvvm_reflect(...)` expressions with appropriate integer values.
However, it only recognizes `"__CUDA_FTZ"` and `"__CUDA_ARCH"`, leaving others with the default value `0`. This is why `__nvvm_reflect("__CUDA_PREC_SQRT")` always evaluates to `0`, leading to the selection of `llvm.nvvm.sqrt.approx.f`:

https://github.com/llvm/llvm-project/blob/2f808dd0702ba3c364eb4373714a1fb09078909d/llvm/lib/Target/NVPTX/NVVMReflect.cpp#L168-L178

I guess that the old libdevice doesn't rely on `"__CUDA_PREC_SQRT"`, thus _NVVMReflectPass_ doesn't try to handle it. (The last commit on this is 7 years ago. 😏)

## Any Possible Solution?

_Clang_ provides several options that affect the behavior of floating-point operations, and three of them are said to take effect in CUDA code. (Stated in [Compiling CUDA with clang, Flags that control numerical code](https://llvm.org/docs/CompileCudaWithLLVM.html#id5). Some of the flags are renamed after then.):

- `ffp-contract`
- `fgpu-flush-denormals-to-zero`
- `fgpu-approx-transcendentals`

These flags don't seem to handle `sqrt`. Maybe we can add a flag, e.g., `-fgpu-approx-sqrt`, or respect the general `-fapprox-func` flag? And set a metadata just like `nvvm-reflect-ftz` for _NVVMReflectPass_ to pick up. Or, look at the `"approx-func-fp-math"="true"` metadata, which is already set when `-fapprox-func` is given. 

To add metadata, the following part is probably where we should place the logic:

https://github.com/llvm/llvm-project/blob/1fbfef9b8ac684483619811d9e90cf8e1f84c6ee/clang/lib/CodeGen/CodeGenModule.cpp#L1289-L1296

--- 

To sum up the issue, I believe the non-approximated square-root (`llvm.nvvm.sqrt.rn.f`) should be used by default, or unless there should be an option for the user to control with. Just like _NVCC_ provides the `-prec-sqrt` flag, _Clang_ could provide this as well.

If this is a missing puzzle, I would like to work on this; if it's already supported or not desired to be touched, please let me know!

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: Lai-YT (Lai-YT)

<details>
I've encountered an issue with the square-root (`sqrtf`) floating-point operation on NVPTX. It always compiles to the `llvm.nvvm.sqrt.approx.f` intrinsic, even when `-fno-approx-func` is specified. I have a hypothesis about the cause and would like to explore a potential fix. Any guidance would be appreciated! 😃 

---

## What's the Problem?

Here's the relevant code:

```cuda
// cat main.cu
__device__ float f(float x) {
  return sqrtf(x);
}
```

The function `sqrtf` always compiles to the `llvm.nvvm.sqrt.approx.f` intrinsic, regardless of `-fno-approx-func`:

```llvm
; clang -Wno-unknown-cuda-version -fno-approx-func main.cu -S -emit-llvm --cuda-device-only -o -
; ModuleID = 'main.cu'
source_filename = "main.cu"
target datalayout = "e-p6:32:32-i64:64-i128:128-v16:16-v32:32-n16:32:64"
target triple = "nvptx64-nvidia-cuda"

@.str = private unnamed_addr constant [11 x i8] c"__CUDA_FTZ\00", align 1
@.str.2 = private unnamed_addr constant [17 x i8] c"__CUDA_PREC_SQRT\00", align 1

; Function Attrs: convergent noinline nounwind optnone
define dso_local noundef float @<!-- -->_Z1ff(float noundef %x) #<!-- -->0 {
entry:
  %__a.addr.i = alloca float, align 4
  %x.addr = alloca float, align 4
  store float %x, ptr %x.addr, align 4
  %0 = load float, ptr %x.addr, align 4
  store float %0, ptr %__a.addr.i, align 4
  %1 = load float, ptr %__a.addr.i, align 4
  %2 = call float @<!-- -->llvm.nvvm.sqrt.approx.f(float %1) #<!-- -->3
  ret float %2
}

; Function Attrs: convergent nounwind
declare i32 @<!-- -->__nvvm_reflect(ptr) #<!-- -->1

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(none)
declare float @<!-- -->llvm.nvvm.sqrt.rn.ftz.f(float) #<!-- -->2

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(none)
declare float @<!-- -->llvm.nvvm.sqrt.approx.ftz.f(float) #<!-- -->2

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(none)
declare float @<!-- -->llvm.nvvm.sqrt.rn.f(float) #<!-- -->2

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(none)
declare float @<!-- -->llvm.nvvm.sqrt.approx.f(float) #<!-- -->2

attributes #<!-- -->0 = { convergent noinline nounwind optnone "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="sm_52" "target-features"="+ptx85,+sm_52" }
attributes #<!-- -->1 = { convergent nounwind "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="sm_52" "target-features"="+ptx85,+sm_52" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #<!-- -->2 = { nocallback nofree nosync nounwind willreturn memory(none) }
attributes #<!-- -->3 = { nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4, !5}
!nvvmir.version = !{!6}

!0 = !{i32 2, !"SDK Version", [2 x i32] [i32 12, i32 6]}
!1 = !{i32 1, !"wchar_size", i32 4}
!2 = !{i32 4, !"nvvm-reflect-ftz", i32 0}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"clang version 21.0.0git (git@<!-- -->github.com:Lai-YT/llvm-project.git e57cd100ca297cf81854e35cccbf703edddd4aad)"}
!5 = !{!"clang version 3.8.0 (tags/RELEASE_380/final)"}
!6 = !{i32 2, i32 0}
```

## What's the Possible Cause?

As defined in `__clang_cuda_math.h`, `sqrtf` expands to the `__nv_sqrtf` intrinsic:

https://github.com/llvm/llvm-project/blob/2f808dd0702ba3c364eb4373714a1fb09078909d/clang/lib/Headers/__clang_cuda_math.h#L318

I noticed that `__nv_sqrtf` is defined by NVIDIA in _libdevice.bc_, so I grabbed the libdevice on my machine (which is `libdevice.10.bc`) and disassembled the bitcode with _llvm-dis_:

```llvm
@.str = private unnamed_addr constant [11 x i8] c"__CUDA_FTZ\00", align 1
@.str.2 = private unnamed_addr constant [17 x i8] c"__CUDA_PREC_SQRT\00", align 1

...

define float @<!-- -->__nv_sqrtf(float %x) #<!-- -->0 {
  %1 = call i32 @<!-- -->__nvvm_reflect(ptr @.str) #<!-- -->6
  %2 = icmp ne i32 %1, 0
  br i1 %2, label %3, label %10

3:                                                ; preds = %0
  %4 = call i32 @<!-- -->__nvvm_reflect(ptr @.str.2) #<!-- -->6
  %5 = icmp ne i32 %4, 0
  br i1 %5, label %6, label %8

6:                                                ; preds = %3
  %7 = call float @<!-- -->llvm.nvvm.sqrt.rn.ftz.f(float %x) #<!-- -->6
  br label %__nvvm_sqrt_f.exit

8:                                                ; preds = %3
  %9 = call float @<!-- -->llvm.nvvm.sqrt.approx.ftz.f(float %x) #<!-- -->6
  br label %__nvvm_sqrt_f.exit

10:                                               ; preds = %0
  %11 = call i32 @<!-- -->__nvvm_reflect(ptr @.str.2) #<!-- -->6
  %12 = icmp ne i32 %11, 0
  br i1 %12, label %13, label %15

13:                                               ; preds = %10
  %14 = call float @<!-- -->llvm.nvvm.sqrt.rn.f(float %x) #<!-- -->6
  br label %__nvvm_sqrt_f.exit

15:                                               ; preds = %10
  %16 = call float @<!-- -->llvm.nvvm.sqrt.approx.f(float %x) #<!-- -->6
  br label %__nvvm_sqrt_f.exit

__nvvm_sqrt_f.exit:                               ; preds = %6, %8, %13, %15
  %.0 = phi float [ %7, %6 ], [ %9, %8 ], [ %14, %13 ], [ %16, %15 ]
  ret float %.0
}
```

The selection of intrinsic respects the value of `__nvvm_reflect("__CUDA_FTZ")` and `__nvvm_reflect("__CUDA_PREC_SQRT")`. The [_NVVMReflectPass_](llvm/lib/Target/NVPTX/NVVMReflect.cpp) is the pass that picks them up and replaces the `__nvvm_reflect(...)` expressions with appropriate integer values.
However, it only recognizes `"__CUDA_FTZ"` and `"__CUDA_ARCH"`, leaving others with the default value `0`. This is why `__nvvm_reflect("__CUDA_PREC_SQRT")` always evaluates to `0`, leading to the selection of `llvm.nvvm.sqrt.approx.f`:

https://github.com/llvm/llvm-project/blob/2f808dd0702ba3c364eb4373714a1fb09078909d/llvm/lib/Target/NVPTX/NVVMReflect.cpp#L168-L178

I guess that the old libdevice doesn't rely on `"__CUDA_PREC_SQRT"`, thus _NVVMReflectPass_ doesn't try to handle it. (The last commit on this is 7 years ago. 😏)

## Any Possible Solution?

_Clang_ provides several options that affect the behavior of floating-point operations, and three of them are said to take effect in CUDA code. (Stated in [Compiling CUDA with clang, Flags that control numerical code](https://llvm.org/docs/CompileCudaWithLLVM.html#id5). Some of the flags are renamed after then.):

- `ffp-contract`
- `fgpu-flush-denormals-to-zero`
- `fgpu-approx-transcendentals`

These flags don't seem to handle `sqrt`. Maybe we can add a flag, e.g., `-fgpu-approx-sqrt`, or respect the general `-fapprox-func` flag? And set a metadata just like `nvvm-reflect-ftz` for _NVVMReflectPass_ to pick up. Or, look at the `"approx-func-fp-math"="true"` metadata, which is already set when `-fapprox-func` is given. 

To add metadata, the following part is probably where we should place the logic:

https://github.com/llvm/llvm-project/blob/1fbfef9b8ac684483619811d9e90cf8e1f84c6ee/clang/lib/CodeGen/CodeGenModule.cpp#L1289-L1296

--- 

To sum up the issue, I believe the non-approximated square-root (`llvm.nvvm.sqrt.rn.f`) should be used by default, or unless there should be an option for the user to control with. Just like _NVCC_ provides the `-prec-sqrt` flag, _Clang_ could provide this as well.

If this is a missing puzzle, I would like to work on this; if it's already supported or not desired to be touched, please let me know!
</details>


---

### Comment 2 - Lai-YT

Oh, maybe it's better to retain the default behavior—the approximated one—to avoid impacting existing users.
@Artem-B, may I ask for your insights on this?

---

### Comment 3 - Artem-B

Good catch. The fact that we do not plumb through `__CUDA_PREC_SQRT` is an oversight.

Your point that fixing the issue will shift the current behavior is valid, and the change in the results will affect numeric results for the users. Adding `--gpu-prec-div` option to override the selection of sqrt variant would work as an escape hatch when specific behavior is desired. When the override is not used, we should mimic behavior of how precise/approximate variants of math functions are handled on other  targets. I'm not very familiar with the specific expectations or requirements. 



---

### Comment 4 - Artem-B

@jhuber6 -- did sqrt result discrepancies show up on any libc/libm tests?

---

### Comment 5 - jhuber6

The `libm` uses its own implementation as we generally assume the builtin to have lower accuracy. That being said, we don't yet have exhausting testing that will give us a ULP bound yet, hopefully soon.

---

### Comment 6 - jhuber6

Nevermind, we actually set that to the builtin sqrtf which likely lowers to that. Probably would help to have accuracy testing working.

---

