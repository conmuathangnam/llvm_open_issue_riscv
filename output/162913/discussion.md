# [HLSL] A boolean vector as a result of logical NOT on an integer vector is sign-extended when converted back to an integer vector

**Author:** Icohedron
**URL:** https://github.com/llvm/llvm-project/issues/162913
**Status:** Closed
**Labels:** clang:frontend, HLSL
**Closed Date:** 2025-10-17T13:42:01Z

## Body

A boolean vector as a result of logical NOT (`!`) on an integer vector is sign-extended when converted back to an integer vector.

## Reproduction

https://godbolt.org/z/hzarWGcoo

Given the following HLSL:
```hlsl
export uint32_t2 foo(uint32_t2 b) {
    return !b;
}

export uint32_t2 bar(uint32_t2 b) {
    return !bool2(b);
}
```

You would expect `foo` and `bar` to be equivalent. However, Clang produces the following IR:
```llvm
; RUN: clang-dxc -T lib_6_7 -Xclang -emit-llvm %s

define noundef range(i32 -1, 1) <2 x i32> @foo(unsigned int vector[2])(<2 x i32> noundef %0) local_unnamed_addr #0 {
  %2 = icmp eq <2 x i32> %0, zeroinitializer
  %3 = sext <2 x i1> %2 to <2 x i32>
  ret <2 x i32> %3
}

define noundef range(i32 0, 2) <2 x i32> @bar(unsigned int vector[2])(<2 x i32> noundef %0) local_unnamed_addr #0 {
  %2 = icmp eq <2 x i32> %0, zeroinitializer
  %3 = zext <2 x i1> %2 to <2 x i32>
  ret <2 x i32> %3
}

attributes #0 = { alwaysinline mustprogress nofree norecurse nosync nounwind willreturn memory(none) "frame-pointer"="all" "no-infs-fp-math"="true" "no-nans-fp-math"="true" "no-signed-zeros-fp-math"="true" "no-trapping-math"="true" "stack-protector-buffer-size"="8" }
```

## Reproduction 2

This same issue can manifest in unusual ways, such as in multiplication between an integer-vector and a boolean-vector.

When multiplying an integer vector by a boolean vector that is the result of a logical NOT (`!`) on another integer vector, the result is negated.

https://godbolt.org/z/n8zYee3W1

Given the following HLSL:
```hlsl
export uint32_t2 foo(uint32_t2 a, uint32_t2 b) {
    return a * !b;
}

export uint32_t2 bar(uint32_t2 a, uint32_t2 b) {
    return a * !bool2(b);
}
```

You would expect `foo` and `bar` to be equivalent. However, Clang produces the following IR:
```llvm
; RUN: clang-dxc -T lib_6_7 -Xclang -emit-llvm %s

define noundef <2 x i32> @foo(unsigned int vector[2], unsigned int vector[2])(<2 x i32> noundef %0, <2 x i32> noundef %1) local_unnamed_addr #0 {
  %3 = icmp eq <2 x i32> %1, zeroinitializer
  %4 = sub <2 x i32> zeroinitializer, %0
  %5 = select <2 x i1> %3, <2 x i32> %4, <2 x i32> zeroinitializer
  ret <2 x i32> %5
}

define noundef <2 x i32> @bar(unsigned int vector[2], unsigned int vector[2])(<2 x i32> noundef %0, <2 x i32> noundef %1) local_unnamed_addr #0 {
  %3 = icmp eq <2 x i32> %1, zeroinitializer
  %4 = select <2 x i1> %3, <2 x i32> %0, <2 x i32> zeroinitializer
  ret <2 x i32> %4
}

attributes #0 = { alwaysinline mustprogress nofree norecurse nosync nounwind willreturn memory(none) "frame-pointer"="all" "no-infs-fp-math"="true" "no-nans-fp-math"="true" "no-signed-zeros-fp-math"="true" "no-trapping-math"="true" "stack-protector-buffer-size"="8" }
```

The subtraction from zero to negate the integer vector seems to appear unexpectedly, but it makes sense when adding `-O0` to the compiler flags:
```llvm
; RUN: clang-dxc -T lib_6_7 -Xclang -emit-llvm -O0 %s

define noundef <2 x i32> @foo(unsigned int vector[2], unsigned int vector[2])(<2 x i32> noundef %0, <2 x i32> noundef %1) #0 {
  %3 = alloca <2 x i32>, align 8
  %4 = alloca <2 x i32>, align 8
  store <2 x i32> %0, ptr %3, align 8
  store <2 x i32> %1, ptr %4, align 8
  %5 = load <2 x i32>, ptr %3, align 8
  %6 = load <2 x i32>, ptr %4, align 8
  %7 = icmp eq <2 x i32> %6, zeroinitializer
  %8 = sext <2 x i1> %7 to <2 x i32> ; <-- a 'true' gets sign-extended to -1
  %9 = mul <2 x i32> %5, %8
  ret <2 x i32> %9
}

define noundef <2 x i32> @bar(unsigned int vector[2], unsigned int vector[2])(<2 x i32> noundef %0, <2 x i32> noundef %1) #0 {
  %3 = alloca <2 x i32>, align 8
  %4 = alloca <2 x i32>, align 8
  store <2 x i32> %0, ptr %3, align 8
  store <2 x i32> %1, ptr %4, align 8
  %5 = load <2 x i32>, ptr %3, align 8
  %6 = load <2 x i32>, ptr %4, align 8
  %7 = icmp ne <2 x i32> %6, zeroinitializer
  %8 = icmp eq <2 x i1> %7, zeroinitializer
  %9 = zext <2 x i1> %8 to <2 x i32>
  %10 = mul <2 x i32> %5, %9
  ret <2 x i32> %10
}

attributes #0 = { alwaysinline convergent mustprogress norecurse nounwind "frame-pointer"="all" "no-infs-fp-math"="true" "no-nans-fp-math"="true" "no-signed-zeros-fp-math"="true" "no-trapping-math"="true" "stack-protector-buffer-size"="8" }
```




## Comments

### Comment 1 - llvm-beanz

This is a really subtle language difference between HLSL and C++ which we haven't put in our spec or Clang implementation yet. The DXC implementation for logical not has this:

https://github.com/microsoft/DirectXShaderCompiler/blob/main/tools/clang/lib/Sema/SemaHLSL.cpp#L10557

The result is to convert numeric values to boolean values before applying logical not.

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Deric C. (Icohedron)

<details>
A boolean vector as a result of logical NOT (`!`) on an integer vector is sign-extended when converted back to an integer vector.

## Reproduction

https://godbolt.org/z/hzarWGcoo

Given the following HLSL:
```hlsl
export uint32_t2 foo(uint32_t2 b) {
    return !b;
}

export uint32_t2 bar(uint32_t2 b) {
    return !bool2(b);
}
```

You would expect `foo` and `bar` to be equivalent. However, Clang produces the following IR:
```llvm
; RUN: clang-dxc -T lib_6_7 -Xclang -emit-llvm %s

define noundef range(i32 -1, 1) &lt;2 x i32&gt; @<!-- -->foo(unsigned int vector[2])(&lt;2 x i32&gt; noundef %0) local_unnamed_addr #<!-- -->0 {
  %2 = icmp eq &lt;2 x i32&gt; %0, zeroinitializer
  %3 = sext &lt;2 x i1&gt; %2 to &lt;2 x i32&gt;
  ret &lt;2 x i32&gt; %3
}

define noundef range(i32 0, 2) &lt;2 x i32&gt; @<!-- -->bar(unsigned int vector[2])(&lt;2 x i32&gt; noundef %0) local_unnamed_addr #<!-- -->0 {
  %2 = icmp eq &lt;2 x i32&gt; %0, zeroinitializer
  %3 = zext &lt;2 x i1&gt; %2 to &lt;2 x i32&gt;
  ret &lt;2 x i32&gt; %3
}

attributes #<!-- -->0 = { alwaysinline mustprogress nofree norecurse nosync nounwind willreturn memory(none) "frame-pointer"="all" "no-infs-fp-math"="true" "no-nans-fp-math"="true" "no-signed-zeros-fp-math"="true" "no-trapping-math"="true" "stack-protector-buffer-size"="8" }
```

## Reproduction 2

This same issue can manifest in unusual ways, such as in multiplication between an integer-vector and a boolean-vector.

When multiplying an integer vector by a boolean vector that is the result of a logical NOT (`!`) on another integer vector, the result is negated.

https://godbolt.org/z/n8zYee3W1

Given the following HLSL:
```hlsl
export uint32_t2 foo(uint32_t2 a, uint32_t2 b) {
    return a * !b;
}

export uint32_t2 bar(uint32_t2 a, uint32_t2 b) {
    return a * !bool2(b);
}
```

You would expect `foo` and `bar` to be equivalent. However, Clang produces the following IR:
```llvm
; RUN: clang-dxc -T lib_6_7 -Xclang -emit-llvm %s

define noundef &lt;2 x i32&gt; @<!-- -->foo(unsigned int vector[2], unsigned int vector[2])(&lt;2 x i32&gt; noundef %0, &lt;2 x i32&gt; noundef %1) local_unnamed_addr #<!-- -->0 {
  %3 = icmp eq &lt;2 x i32&gt; %1, zeroinitializer
  %4 = sub &lt;2 x i32&gt; zeroinitializer, %0
  %5 = select &lt;2 x i1&gt; %3, &lt;2 x i32&gt; %4, &lt;2 x i32&gt; zeroinitializer
  ret &lt;2 x i32&gt; %5
}

define noundef &lt;2 x i32&gt; @<!-- -->bar(unsigned int vector[2], unsigned int vector[2])(&lt;2 x i32&gt; noundef %0, &lt;2 x i32&gt; noundef %1) local_unnamed_addr #<!-- -->0 {
  %3 = icmp eq &lt;2 x i32&gt; %1, zeroinitializer
  %4 = select &lt;2 x i1&gt; %3, &lt;2 x i32&gt; %0, &lt;2 x i32&gt; zeroinitializer
  ret &lt;2 x i32&gt; %4
}

attributes #<!-- -->0 = { alwaysinline mustprogress nofree norecurse nosync nounwind willreturn memory(none) "frame-pointer"="all" "no-infs-fp-math"="true" "no-nans-fp-math"="true" "no-signed-zeros-fp-math"="true" "no-trapping-math"="true" "stack-protector-buffer-size"="8" }
```

The subtraction from zero to negate the integer vector seems to appear unexpectedly, but it makes sense when adding `-O0` to the compiler flags:
```llvm
; RUN: clang-dxc -T lib_6_7 -Xclang -emit-llvm -O0 %s

define noundef &lt;2 x i32&gt; @<!-- -->foo(unsigned int vector[2], unsigned int vector[2])(&lt;2 x i32&gt; noundef %0, &lt;2 x i32&gt; noundef %1) #<!-- -->0 {
  %3 = alloca &lt;2 x i32&gt;, align 8
  %4 = alloca &lt;2 x i32&gt;, align 8
  store &lt;2 x i32&gt; %0, ptr %3, align 8
  store &lt;2 x i32&gt; %1, ptr %4, align 8
  %5 = load &lt;2 x i32&gt;, ptr %3, align 8
  %6 = load &lt;2 x i32&gt;, ptr %4, align 8
  %7 = icmp eq &lt;2 x i32&gt; %6, zeroinitializer
  %8 = sext &lt;2 x i1&gt; %7 to &lt;2 x i32&gt; ; &lt;-- a 'true' gets sign-extended to -1
  %9 = mul &lt;2 x i32&gt; %5, %8
  ret &lt;2 x i32&gt; %9
}

define noundef &lt;2 x i32&gt; @<!-- -->bar(unsigned int vector[2], unsigned int vector[2])(&lt;2 x i32&gt; noundef %0, &lt;2 x i32&gt; noundef %1) #<!-- -->0 {
  %3 = alloca &lt;2 x i32&gt;, align 8
  %4 = alloca &lt;2 x i32&gt;, align 8
  store &lt;2 x i32&gt; %0, ptr %3, align 8
  store &lt;2 x i32&gt; %1, ptr %4, align 8
  %5 = load &lt;2 x i32&gt;, ptr %3, align 8
  %6 = load &lt;2 x i32&gt;, ptr %4, align 8
  %7 = icmp ne &lt;2 x i32&gt; %6, zeroinitializer
  %8 = icmp eq &lt;2 x i1&gt; %7, zeroinitializer
  %9 = zext &lt;2 x i1&gt; %8 to &lt;2 x i32&gt;
  %10 = mul &lt;2 x i32&gt; %5, %9
  ret &lt;2 x i32&gt; %10
}

attributes #<!-- -->0 = { alwaysinline convergent mustprogress norecurse nounwind "frame-pointer"="all" "no-infs-fp-math"="true" "no-nans-fp-math"="true" "no-signed-zeros-fp-math"="true" "no-trapping-math"="true" "stack-protector-buffer-size"="8" }
```



</details>


---

