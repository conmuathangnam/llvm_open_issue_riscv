# MIPSr6: fail to build: compare doubles and select floats

**Author:** wzssyqa
**URL:** https://github.com/llvm/llvm-project/issues/173592
**Status:** Open
**Labels:** backend:MIPS, crash, llvm:SelectionDAG

## Body

```
define float @ueq(double %lhs, double %rhs, float %true, float %false) {
entry:
  %0 = fcmp ueq double %lhs, %rhs
  %1 = select i1 %0, float %true, float %false
  ret float %1
}
```


```
llvm/lib/CodeGen/SelectionDAG/SelectionDAGBuilder.cpp:11957: void llvm::SelectionDAGISel::LowerArguments(const Function &): Assertion `EVT(Ins[i].VT) == InVals[i].getValueType() && "LowerFormalArguments emitted a value with the wrong type!"' failed.
```


It only happens when using `-debug` option.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-mips

Author: YunQiang Su (wzssyqa)

<details>
```
define float @<!-- -->ueq(double %lhs, double %rhs, float %true, float %false) {
entry:
  %0 = fcmp ueq double %lhs, %rhs
  %1 = select i1 %0, float %true, float %false
  ret float %1
}
```


```
llvm/lib/CodeGen/SelectionDAG/SelectionDAGBuilder.cpp:11957: void llvm::SelectionDAGISel::LowerArguments(const Function &amp;): Assertion `EVT(Ins[i].VT) == InVals[i].getValueType() &amp;&amp; "LowerFormalArguments emitted a value with the wrong type!"' failed.
```


It only happens when using `-debug` option.
</details>


---

### Comment 2 - DhanashreePetare

Greetings @wzssyqa , I am interested to work on the issue. Can you please assign it to me. Thankyou.

---

### Comment 3 - wzssyqa

> Greetings [@wzssyqa](https://github.com/wzssyqa) , I am interested to work on the issue. Can you please assign it to me. Thankyou.

You can just do it and submit a PR if the patch are ready.

---

