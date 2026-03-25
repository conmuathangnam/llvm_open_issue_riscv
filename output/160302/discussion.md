# [InstCombine] PtrReplacer creates select with invalid operands

**Author:** Maetveis
**URL:** https://github.com/llvm/llvm-project/issues/160302
**Status:** Closed
**Labels:** crash, llvm:instcombine
**Closed Date:** 2025-09-25T08:20:22Z

## Body

# Description

The following reduced testcase raises an assertion when run through instcombine:
(`opt -passes=instcombine input.ll`)
```llvm
target triple = "amdgcn-amd-amdhsa"

%"class.sycl::_V1::vec.380" = type { %"class.sycl::_V1::detail::vec_base.416" }
%"class.sycl::_V1::detail::vec_base.416" = type { [16 x float] }

@global = external addrspace(1) constant { [16 x float] }, align 64

define void @foo(ptr addrspace(5) %0) {
entry:
  %agg.tmp2.i4 = alloca %"class.sycl::_V1::vec.380", align 64, addrspace(5)
  call void @llvm.memcpy.p5.p1.i64(ptr addrspace(5) %agg.tmp2.i4, ptr addrspace(1) @global, i64 0, i1 false)
  %m_Data.i14.i = getelementptr %"class.sycl::_V1::detail::vec_base.416", ptr addrspace(5) %agg.tmp2.i4, i32 0, i32 0
  %1 = getelementptr [16 x float], ptr addrspace(5) %0, i32 0, i32 0
  %2 = select i1 false, ptr addrspace(5) %m_Data.i14.i, ptr addrspace(5) %1
  %cond.i = load float, ptr addrspace(5) %2, align 4
  store float %cond.i, ptr null, align 4
  ret void
}

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memcpy.p0.p1.i64(ptr noalias writeonly captures(none), ptr addrspace(1) noalias readonly captures(none), i64, i1 immarg) #0

attributes #0 = { nocallback nofree nounwind willreturn memory(argmem: readwrite) }
```

See this example on Compiler Explorer: https://godbolt.org/z/8T5TqT5Ws

# Analysis

This seems similar to https://github.com/llvm/llvm-project/pull/148829, only this time we're not checking if the argument of the GEP is available.


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-amdgpu

Author: Mészáros Gergely (Maetveis)

<details>
# Description

The following reduced testcase raises an assertion when run through instcombine:
(`opt -passes=instcombine input.ll`)
```llvm
target triple = "amdgcn-amd-amdhsa"

%"class.sycl::_V1::vec.380" = type { %"class.sycl::_V1::detail::vec_base.416" }
%"class.sycl::_V1::detail::vec_base.416" = type { [16 x float] }

@<!-- -->global = external addrspace(1) constant { [16 x float] }, align 64

define void @<!-- -->foo(ptr addrspace(5) %0) {
entry:
  %agg.tmp2.i4 = alloca %"class.sycl::_V1::vec.380", align 64, addrspace(5)
  call void @<!-- -->llvm.memcpy.p5.p1.i64(ptr addrspace(5) %agg.tmp2.i4, ptr addrspace(1) @<!-- -->global, i64 0, i1 false)
  %m_Data.i14.i = getelementptr %"class.sycl::_V1::detail::vec_base.416", ptr addrspace(5) %agg.tmp2.i4, i32 0, i32 0
  %1 = getelementptr [16 x float], ptr addrspace(5) %0, i32 0, i32 0
  %2 = select i1 false, ptr addrspace(5) %m_Data.i14.i, ptr addrspace(5) %1
  %cond.i = load float, ptr addrspace(5) %2, align 4
  store float %cond.i, ptr null, align 4
  ret void
}

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: readwrite)
declare void @<!-- -->llvm.memcpy.p0.p1.i64(ptr noalias writeonly captures(none), ptr addrspace(1) noalias readonly captures(none), i64, i1 immarg) #<!-- -->0

attributes #<!-- -->0 = { nocallback nofree nounwind willreturn memory(argmem: readwrite) }
```

See this example on Compiler Explorer: https://godbolt.org/z/8T5TqT5Ws

# Analysis

This seems similar to https://github.com/llvm/llvm-project/pull/148829, only this time we're not checking if the argument of the GEP is available.

</details>


---

