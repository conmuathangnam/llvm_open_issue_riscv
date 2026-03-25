# [Clang][Windows] Accepts invalid `[] int x;`

**Author:** MagentaTreehouse
**URL:** https://github.com/llvm/llvm-project/issues/173156

## Body

```c++
[] int x;
```
This code is invalid but accepted by Clang on Windows, reproducible with the main branch.
See https://compiler-explorer.com/z/s65rv9Pcf. (clang-cl 18.1 only)

## Comments

### Comment 1 - EugeneZelenko

Could you please try 21 or `main` branch? Only most recent release is maintained.


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (MagentaTreehouse)

<details>
```c++
[] int x;
```
This code is invalid but accepted by Clang on Windows, reproducible with the main branch.
See https://compiler-explorer.com/z/s65rv9Pcf. (clang-cl 18.1 only)
</details>


---

### Comment 3 - MagentaTreehouse

> Could you please try 21 or `main` branch? Only most recent release is maintained.

@EugeneZelenko Yes, this is reproducible with the main branch as described in the OP. It is Compiler Explorer that provides 18.1 only for clang-cl.

---

### Comment 4 - keinflue

Also reproducible with `-fms-extensions` and usual clang driver on main: https://compiler-explorer.com/z/xcs6axovW

---

### Comment 5 - shafik

Goes all the way back to after clang-2.8: https://compiler-explorer.com/z/6sceKEzTM

---

### Comment 6 - shafik

CC @AaronBallman 

---

