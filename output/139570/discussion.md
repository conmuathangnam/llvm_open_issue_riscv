# [HIP][AMDGPU] Assertion failure when AMD attribute value depends on template argument

**Author:** jayfoad
**URL:** https://github.com/llvm/llvm-project/issues/139570
**Status:** Closed
**Labels:** clang:frontend, confirmed, crash
**Closed Date:** 2025-05-28T12:05:05Z

## Body

With this [test case](https://github.com/user-attachments/files/20164855/f.txt) I get:
```
$ clang++ -nogpuinc -nogpulib -c -x hip f.txt
clang-21: lib/AST/ExprConstant.cpp:17150: APSInt clang::Expr::EvaluateKnownConstInt(const ASTContext &, SmallVectorImpl<PartialDiagnosticAt> *) const: Assertion `!isValueDependent() && "Expression evaluator can't be called on a dependent expression."' failed.
```

## Comments

### Comment 1 - jayfoad

This doesn't just affect `amdgpu_max_num_work_groups`. There are other attributes that `AMDGPUTargetCodeGenInfo` handles by calling `EvaluateKnownConstInt`, which probably have the same problem.

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Jay Foad (jayfoad)

<details>
With this [test case](https://github.com/user-attachments/files/20164855/f.txt) I get:
```
$ clang++ -nogpuinc -nogpulib -c -x hip f.txt
clang-21: lib/AST/ExprConstant.cpp:17150: APSInt clang::Expr::EvaluateKnownConstInt(const ASTContext &amp;, SmallVectorImpl&lt;PartialDiagnosticAt&gt; *) const: Assertion `!isValueDependent() &amp;&amp; "Expression evaluator can't be called on a dependent expression."' failed.
```
</details>


---

### Comment 3 - shiltian

Yup, we need to skip dependent expression.

---

