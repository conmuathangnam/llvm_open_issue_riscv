# riscv64 backend failed to perform tail call elimination on a call site marked musttail

**Author:** lqcd
**URL:** https://github.com/llvm/llvm-project/issues/56908

## Body

`__attribute__((musttail))` crashes the RISCV backend in version 14.0.6 when running

```console
$ clang -O3 --target=riscv64 -fomit-frame-pointer -Dregcall='' -S -c reg-args.c 
```

with `reg-args.c` in the attached tarball.

There is no `/Users/avp/Library/Logs/DiagnosticReports/clang-14*` files, `report.crash` is stderr
[clang-14-report.tar.gz](https://github.com/llvm/llvm-project/files/9255302/clang-14-report.tar.gz)

## Comments

### Comment 1 - llvmbot

@llvm/issue-subscribers-backend-risc-v

---

### Comment 2 - luxufan

For llvm's RISCV target, tail call is disabled if the stack is used for passing parameters. But it seems that if the stack offset of the parameter that passed by stack is consistent with the formal argument's stack offset, tail call optimization call be enabled. And ARM target does that. I am referencing arm's implementation to achieve this.

---

