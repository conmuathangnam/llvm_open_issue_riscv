# [RISCV] Assertion failure from std::optional in VLOptimizer

**Author:** topperc
**URL:** https://github.com/llvm/llvm-project/issues/139288
**Status:** Closed
**Labels:** backend:RISC-V, crash
**Closed Date:** 2025-05-14T10:03:36Z

## Body

This test case causes VL optimizer to access the value of an unset std::optional. https://godbolt.org/z/cPccraWM6

```
  source_filename = "reduced_2.ll"                                               
  target datalayout = "e-m:e-p:64:64-i64:64-i128:128-n32:64-S128"                
  target triple = "riscv64-unknown-linux-gnu"                                    
                                                                                 
  define i32 @pps_is_equal(<vscale x 16 x i1> %0, <vscale x 16 x i1> %1) #0 {    
  entry:                                                                         
    %2 = tail call <vscale x 16 x i1> @llvm.vp.or.nxv16i1(<vscale x 16 x i1> zeroinitializer, <vscale x 16 x i1> zeroinitializer, <vscale x 16 x i1> %0, i32 1)
    %3 = tail call i32 @llvm.vp.reduce.smax.nxv16i32(i32 0, <vscale x 16 x i32> zeroinitializer, <vscale x 16 x i1> %2, i32 1)
    ret i32 %3                                                                   
  }                                                                              
                                                                                 
  declare <vscale x 16 x i1> @llvm.vp.or.nxv16i1(<vscale x 16 x i1>, <vscale x 16 x i1>, <vscale x 16 x i1>, i32) #1
                                                                                 
  declare i32 @llvm.vp.reduce.smax.nxv16i32(i32, <vscale x 16 x i32>, <vscale x 16 x i1>, i32) #1
                                                                                 
  attributes #0 = { "target-cpu"="sifive-x280" }                                 
  attributes #1 = { nocallback nofree nosync nounwind willreturn memory(none) }
```

## Comments

### Comment 1 - topperc

CC: @BeMg @lukel97 @preames 

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-backend-risc-v

Author: Craig Topper (topperc)

<details>
This test case causes VL optimizer to access the value of an unset std::optional. https://godbolt.org/z/cPccraWM6

```
  source_filename = "reduced_2.ll"                                               
  target datalayout = "e-m:e-p:64:64-i64:64-i128:128-n32:64-S128"                
  target triple = "riscv64-unknown-linux-gnu"                                    
                                                                                 
  define i32 @<!-- -->pps_is_equal(&lt;vscale x 16 x i1&gt; %0, &lt;vscale x 16 x i1&gt; %1) #<!-- -->0 {    
  entry:                                                                         
    %2 = tail call &lt;vscale x 16 x i1&gt; @<!-- -->llvm.vp.or.nxv16i1(&lt;vscale x 16 x i1&gt; zeroinitializer, &lt;vscale x 16 x i1&gt; zeroinitializer, &lt;vscale x 16 x i1&gt; %0, i32 1)
    %3 = tail call i32 @<!-- -->llvm.vp.reduce.smax.nxv16i32(i32 0, &lt;vscale x 16 x i32&gt; zeroinitializer, &lt;vscale x 16 x i1&gt; %2, i32 1)
    ret i32 %3                                                                   
  }                                                                              
                                                                                 
  declare &lt;vscale x 16 x i1&gt; @<!-- -->llvm.vp.or.nxv16i1(&lt;vscale x 16 x i1&gt;, &lt;vscale x 16 x i1&gt;, &lt;vscale x 16 x i1&gt;, i32) #<!-- -->1
                                                                                 
  declare i32 @<!-- -->llvm.vp.reduce.smax.nxv16i32(i32, &lt;vscale x 16 x i32&gt;, &lt;vscale x 16 x i1&gt;, i32) #<!-- -->1
                                                                                 
  attributes #<!-- -->0 = { "target-cpu"="sifive-x280" }                                 
  attributes #<!-- -->1 = { nocallback nofree nosync nounwind willreturn memory(none) }
```
</details>


---

