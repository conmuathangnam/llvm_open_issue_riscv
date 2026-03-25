# [SPIR-V] Hitting llvm_unreachable in SPIRVLegalizePointerCast when compiling local resource arrays

**Author:** hekota
**URL:** https://github.com/llvm/llvm-project/issues/154669

## Body

Hitting `llvm_unreachable ` in `SPIRVLegalizePointerCast` when compiling these two offload-tests cases with SPIR-V:

Feature/ResourceArrays/array-local.test:
```
RWBuffer<float> BufA : register(u0);
RWBuffer<float> BufB : register(u1);
RWBuffer<float> Out  : register(u2);

float foo(RWBuffer<float> ArgArray[2], uint Index) {
    return ArgArray[0][Index] + ArgArray[1][Index] * 10;
}

float bar(uint Index) {
  RWBuffer<float> LocalArray[2];
  LocalArray[0] = BufA;
  LocalArray[1] = BufB;
  return foo(LocalArray, Index);
}

[numthreads(4,2,1)]
void main(uint GI : SV_GroupIndex) {
  Out[GI] = bar(GI);
}
```
Feature/ResourceArrays/array-local2.test
```
RWBuffer<int> X : register(u0);
RWBuffer<int> Y : register(u1);

void SomeFn(RWBuffer<int> B[2], uint Idx, int Val0) {
  B[0] = Y;
  B[0][Idx] = Val0;
}

[numthreads(4,1,1)]
void main(uint GI : SV_GroupIndex) {
  RWBuffer<int> A[2] = {X, Y};
  SomeFn(A, GI, 1);
  A[0][GI] = 2;
}
```

I am going to mark these two tests as XFAIL for Clang-Vulkan. Please re-enable when fixed.




## Comments

### Comment 1 - s-perron

The optimizer is leaving a lot of code around that has not been legalized. The llvm-ir going into the backend still has the definitions function `foo` and `bar` are still present, even if they have been inlined. I think this is causing the loads and stored of the global static variables to not be optimized.

I'll need to look into the optimizations closer to understand exactly why that is happening.

---

### Comment 2 - s-perron

The problem is that the function `foo` and `bar` are not "internal", so the optimizer cannot remove them. Because these function could be called from somewhere else and reference the global variable, we cannot optimize the global variables. See the definition of foo at https://godbolt.org/z/xbxn7av97.

```
; Function Attrs: alwaysinline convergent mustprogress norecurse nounwind
define hidden spir_func noundef nofpclass(nan inf) float @foo(...) #2 !dbg !87
```

I'm guessing this works for DXIL because that backend will ignore functions that are not reachable from an entry point, and there is a specific pass to remove loads and stores of resources.

Note that even if I make the functions static the variables (local and global) are not optimized aways: https://godbolt.org/z/7n5Ev7zG1.


---

### Comment 3 - s-perron

Why are the variables not optimized even when the functions are static?

Short answer is type punning. Consider https://godbolt.org/z/qEK1sf1qx.

In the function `bar`, `BufA` and `BufB` are copied to the local array via memcpy's. 

```
  %12 = getelementptr inbounds [2 x %"class.hlsl::RWBuffer"], ptr %3, i32 0, i32 0, !dbg !116
  call void @llvm.memcpy.p0.p0.i32(ptr align 4 %12, ptr align 4 @BufA, i32 4, i1 false), !dbg !117
```

Because the memcpy's are for element of a small size, InstCombine will convert that into a load and store

```
  %arrayidx1 = getelementptr inbounds nuw i8, ptr %LocalArray, i64 8 
  %3 = load i64, ptr @_ZL4BufB, align 8, !tbaa !9
  store i64 %3, ptr %arrayidx1, align 8, !tbaa !9
```

This causes a type mismatch. The stores to the arrays are i64, but they are loaded as the handle type. If the memory references were removed, there would be type mismatch, so SROA bails.


---

### Comment 4 - s-perron

The solution to the mempy problem is to add a copy constructor and operator= to the buffer types. I have that done, but need to add tests. I'll try to get a PR open soon. 

I don't know the solution to the visibility problem on the functions yet. 

---

### Comment 5 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Helena Kotas (hekota)

<details>
Hitting `llvm_unreachable ` in `SPIRVLegalizePointerCast` when compiling these two offload-tests cases with SPIR-V:

Feature/ResourceArrays/array-local.test:
```
RWBuffer&lt;float&gt; BufA : register(u0);
RWBuffer&lt;float&gt; BufB : register(u1);
RWBuffer&lt;float&gt; Out  : register(u2);

float foo(RWBuffer&lt;float&gt; ArgArray[2], uint Index) {
    return ArgArray[0][Index] + ArgArray[1][Index] * 10;
}

float bar(uint Index) {
  RWBuffer&lt;float&gt; LocalArray[2];
  LocalArray[0] = BufA;
  LocalArray[1] = BufB;
  return foo(LocalArray, Index);
}

[numthreads(4,2,1)]
void main(uint GI : SV_GroupIndex) {
  Out[GI] = bar(GI);
}
```
Feature/ResourceArrays/array-local2.test
```
RWBuffer&lt;int&gt; X : register(u0);
RWBuffer&lt;int&gt; Y : register(u1);

void SomeFn(RWBuffer&lt;int&gt; B[2], uint Idx, int Val0) {
  B[0] = Y;
  B[0][Idx] = Val0;
}

[numthreads(4,1,1)]
void main(uint GI : SV_GroupIndex) {
  RWBuffer&lt;int&gt; A[2] = {X, Y};
  SomeFn(A, GI, 1);
  A[0][GI] = 2;
}
```

I am going to mark these two tests as XFAIL for Clang-Vulkan. Please re-enable when fixed.



</details>


---

