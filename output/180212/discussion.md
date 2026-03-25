# [X86] error in backend: Do not know how to split this operator's operand!

**Author:** Andarwinux
**URL:** https://github.com/llvm/llvm-project/issues/180212
**Status:** Closed
**Labels:** crash, llvm:SelectionDAG
**Closed Date:** 2026-02-10T09:01:15Z

## Body

https://rust.godbolt.org/z/hx74ejbzW

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-x86

Author: None (Andarwinux)

<details>
https://rust.godbolt.org/z/hx74ejbzW
</details>


---

### Comment 2 - RKSimon

```ll
  %971 = call i1 @llvm.experimental.vector.extract.last.active.v128i1(<128 x i1> %967, <128 x i1> %966, i1 %914)
```


---

### Comment 3 - Andarwinux

https://github.com/llvm/llvm-project/pull/180290

---

