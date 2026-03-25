# Assertion `(FrameSDOps.empty() || MF.getFrameInfo().adjustsStack()) && "AdjustsStack n ot set in presence of a frame pseudo instruction."' failed.

**Author:** enoskova-sc
**URL:** https://github.com/llvm/llvm-project/issues/160104
**Status:** Closed
**Labels:** backend:RISC-V, crash
**Closed Date:** 2025-11-01T11:18:41Z

## Body

Running 
`$llc -mtriple riscv32 test.ll`
results in

```
Assertion `(FrameSDOps.empty() || MF.getFrameInfo().adjustsStack()) && "AdjustsStack not set in presence of a frame pseudo instruction."' failed.
```
Test itself:

```
$cat test.ll
define i32 @test(i1 %arg_1, i32 %arg_2) {
entry:
  %sel_1 = select i1 %arg_1, i32 %arg_2, i32 1
  %div = udiv i32 %arg_2, 7
  %cond_1 = icmp ugt i32 %div, %sel_1
  %sel_2 = select i1 %arg_1, i32 %div, i32 3
  %sel = select i1 %arg_1, i32 %sel_1, i32 %sel_2
  br label %body

body:
  %res = phi i32 [ %sel, %entry ], [ %add_loop, %body ]
  %add_loop = add i32 4, %res
  %cond_2 = icmp ugt i32 %add_loop, 3
  br i1 %cond_2, label %body, label %exit

exit:
  ret i32 %add_loop
}
```

Compiler revision: e9db38c17673a5183a78c3c2e1765eb018629137


## Comments

### Comment 1 - enoskova-sc

PR: https://github.com/llvm/llvm-project/pull/160105

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-backend-risc-v

Author: Elizaveta Noskova (enoskova-sc)

<details>
Running 
`$llc -mtriple riscv32 test.ll`
results in

```
Assertion `(FrameSDOps.empty() || MF.getFrameInfo().adjustsStack()) &amp;&amp; "AdjustsStack not set in presence of a frame pseudo instruction."' failed.
```
Test itself:

```
$cat test.ll
define i32 @<!-- -->test(i1 %arg_1, i32 %arg_2) {
entry:
  %sel_1 = select i1 %arg_1, i32 %arg_2, i32 1
  %div = udiv i32 %arg_2, 7
  %cond_1 = icmp ugt i32 %div, %sel_1
  %sel_2 = select i1 %arg_1, i32 %div, i32 3
  %sel = select i1 %arg_1, i32 %sel_1, i32 %sel_2
  br label %body

body:
  %res = phi i32 [ %sel, %entry ], [ %add_loop, %body ]
  %add_loop = add i32 4, %res
  %cond_2 = icmp ugt i32 %add_loop, 3
  br i1 %cond_2, label %body, label %exit

exit:
  ret i32 %add_loop
}
```

Compiler revision: e9db38c17673a5183a78c3c2e1765eb018629137

</details>


---

### Comment 3 - EugeneZelenko

@XChy: Could you please elaborate?

---

### Comment 4 - XChy

This issue has been fixed by #160105.

---

