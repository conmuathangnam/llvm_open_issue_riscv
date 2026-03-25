# [RISCV] Assertion `OldVNI == VNI && "Wrong value out of predecessor"' failed.

**Author:** XChy
**URL:** https://github.com/llvm/llvm-project/issues/156249

## Body

The reduced testcase below triggers an assertion in the trunk at O1/O2/O3:
```llvm
target datalayout = "e-m:e-p:64:64-i64:64-i128:128-n32:64-S128"
target triple = "riscv64-unknown-unknown"

define <8 x i64> @d() #0 {
entry:
  br label %ac

ac:                                               ; preds = %asm.fallthrough3, %asm.fallthrough, %ac, %entry
  %e.2 = phi <8 x i64> [ zeroinitializer, %entry ], [ %vecins, %ac ], [ %vecins4, %asm.fallthrough3 ], [ %vecins2, %asm.fallthrough ]
  %vecins = insertelement <8 x i64> %e.2, i64 0, i64 0
  callbr void asm sideeffect "", "!i"()
          to label %asm.fallthrough [label %ac]

asm.fallthrough:                                  ; preds = %ac
  %vecins2 = shufflevector <8 x i64> %vecins, <8 x i64> splat (i64 1), <8 x i32> <i32 0, i32 1, i32 2, i32 10, i32 4, i32 5, i32 6, i32 7>
  callbr void asm sideeffect "", "!i"()
          to label %asm.fallthrough3 [label %ac]

asm.fallthrough3:                                 ; preds = %asm.fallthrough
  %vecins4 = insertelement <8 x i64> %vecins2, i64 0, i64 0
  callbr void asm sideeffect "", "!i"()
          to label %asm.fallthrough5 [label %ac]

asm.fallthrough5:                                 ; preds = %asm.fallthrough3
  %or = or <8 x i64> %vecins2, %vecins4
  ret <8 x i64> %or
}

attributes #0 = { "target-features"="+v" }
```
Compiler dump:
```
clang-19: /home/shareduser/XChy/Projects/llvm-project/llvm/lib/CodeGen/LiveIntervals.cpp:466: void llvm::LiveIntervals::extendSegmentsToUses(llvm::LiveRange&, ShrinkToUsesWorkList&, llvm::Register, llvm::LaneBitmask): Assertion `OldVNI == VNI && "Wrong value out of predecessor"' failed.
....
```

https://godbolt.org/z/ExnhKzrMs

May be related to #134424.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-risc-v

Author: XChy (XChy)

<details>
The reduced testcase below triggers an assertion in the trunk at O1/O2/O3:
```llvm
target datalayout = "e-m:e-p:64:64-i64:64-i128:128-n32:64-S128"
target triple = "riscv64-unknown-unknown"

define &lt;8 x i64&gt; @<!-- -->d() #<!-- -->0 {
entry:
  br label %ac

ac:                                               ; preds = %asm.fallthrough3, %asm.fallthrough, %ac, %entry
  %e.2 = phi &lt;8 x i64&gt; [ zeroinitializer, %entry ], [ %vecins, %ac ], [ %vecins4, %asm.fallthrough3 ], [ %vecins2, %asm.fallthrough ]
  %vecins = insertelement &lt;8 x i64&gt; %e.2, i64 0, i64 0
  callbr void asm sideeffect "", "!i"()
          to label %asm.fallthrough [label %ac]

asm.fallthrough:                                  ; preds = %ac
  %vecins2 = shufflevector &lt;8 x i64&gt; %vecins, &lt;8 x i64&gt; splat (i64 1), &lt;8 x i32&gt; &lt;i32 0, i32 1, i32 2, i32 10, i32 4, i32 5, i32 6, i32 7&gt;
  callbr void asm sideeffect "", "!i"()
          to label %asm.fallthrough3 [label %ac]

asm.fallthrough3:                                 ; preds = %asm.fallthrough
  %vecins4 = insertelement &lt;8 x i64&gt; %vecins2, i64 0, i64 0
  callbr void asm sideeffect "", "!i"()
          to label %asm.fallthrough5 [label %ac]

asm.fallthrough5:                                 ; preds = %asm.fallthrough3
  %or = or &lt;8 x i64&gt; %vecins2, %vecins4
  ret &lt;8 x i64&gt; %or
}

attributes #<!-- -->0 = { "target-features"="+v" }
```
Compiler dump:
```
clang-19: /home/shareduser/XChy/Projects/llvm-project/llvm/lib/CodeGen/LiveIntervals.cpp:466: void llvm::LiveIntervals::extendSegmentsToUses(llvm::LiveRange&amp;, ShrinkToUsesWorkList&amp;, llvm::Register, llvm::LaneBitmask): Assertion `OldVNI == VNI &amp;&amp; "Wrong value out of predecessor"' failed.
....
```

https://godbolt.org/z/ExnhKzrMs

May be related to #<!-- -->134424.
</details>


---

### Comment 2 - XChy

It seems the value is marked as unused incorrectly:
https://github.com/llvm/llvm-project/blob/d6d0f4f156e27e5171475d229b8ea2616c2b3d0c/llvm/lib/CodeGen/RegisterCoalescer.cpp#L3410-L3411
When removing instruction at 496r in sub range `[352e,432B:2)[496r,624r:2)`, it marks 352e as unused, but it should be used later:
https://github.com/llvm/llvm-project/blob/d6d0f4f156e27e5171475d229b8ea2616c2b3d0c/llvm/lib/CodeGen/RegisterCoalescer.cpp#L2240
CC @MatzeB, @arsenm 

---

### Comment 3 - XChy

My fuzzer reports another testcase: https://godbolt.org/z/zWqbx3M54
This time it triggers different assertion:
 ``` Assertion `isValid() && "Attempt to compare reserved index." ```

---

### Comment 4 - arsenm

That one is usually from failing to filter out phi or dead values somewhere 

---

