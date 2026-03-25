# Assertion `(PDiffI->getUnitInc() >= 0) == (PNew >= POld) && "PSet overflow/underflow"' failed.

**Author:** XChy
**URL:** https://github.com/llvm/llvm-project/issues/155807

## Body

The reduced testcase below triggers an assertion in the trunk:
```llvm
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i64 @e(i64 %vecext) {
entry:
  callbr void asm sideeffect "", "!i,~{dirflag},~{fpsr},~{flags}"()
          to label %g.preheader [label %g.preheader]

g.preheader:                                      ; preds = %entry, %entry
  store i64 0, ptr null, align 8
  tail call void asm sideeffect "", "~{rax},~{eax},~{dirflag},~{fpsr},~{flags}"()
  %rem = srem i64 705425148255374, %vecext
  ret i64 %rem
}
```

Compiler dump:
```
llc: /home/xchy/Projects/llvm-project/llvm/lib/CodeGen/RegisterPressure.cpp:1170: void llvm::RegPressureTracker::getUpwardPressureDelta(const llvm::MachineInstr*, llvm::PressureDiff&, llvm::RegPressureDelta&, llvm::ArrayR
ef<llvm::PressureChange>, llvm::ArrayRef<unsigned int>) const: Assertion `(PDiffI->getUnitInc() >= 0) == (PNew >= POld) && "PSet overflow/underflow"' failed.
```

https://godbolt.org/z/jf61qz184

## Comments

### Comment 1 - XChy

A minimal testcase:
```llvm
define i64 @e(i64 %vecext) {
entry:
  tail call void asm sideeffect "", "~{rax},~{eax},~{dirflag},~{fpsr},~{flags}"()
  %rem = srem i64 705425148255374, %vecext
  ret i64 %rem
}
```

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-backend-x86

Author: XChy (XChy)

<details>
The reduced testcase below triggers an assertion in the trunk:
```llvm
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i64 @<!-- -->e(i64 %vecext) {
entry:
  callbr void asm sideeffect "", "!i,~{dirflag},~{fpsr},~{flags}"()
          to label %g.preheader [label %g.preheader]

g.preheader:                                      ; preds = %entry, %entry
  store i64 0, ptr null, align 8
  tail call void asm sideeffect "", "~{rax},~{eax},~{dirflag},~{fpsr},~{flags}"()
  %rem = srem i64 705425148255374, %vecext
  ret i64 %rem
}
```

Compiler dump:
```
llc: /home/xchy/Projects/llvm-project/llvm/lib/CodeGen/RegisterPressure.cpp:1170: void llvm::RegPressureTracker::getUpwardPressureDelta(const llvm::MachineInstr*, llvm::PressureDiff&amp;, llvm::RegPressureDelta&amp;, llvm::ArrayR
ef&lt;llvm::PressureChange&gt;, llvm::ArrayRef&lt;unsigned int&gt;) const: Assertion `(PDiffI-&gt;getUnitInc() &gt;= 0) == (PNew &gt;= POld) &amp;&amp; "PSet overflow/underflow"' failed.
```

https://godbolt.org/z/jf61qz184
</details>


---

