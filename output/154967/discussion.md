# wrong code at -O{s,2} with "-mllvm -force-vector-width=1 -mllvm -force-vector-interleave=2" on x86_64-linux-gnu

**Author:** zhendongsu
**URL:** https://github.com/llvm/llvm-project/issues/154967
**Status:** Closed
**Labels:** miscompilation, vectorizers
**Closed Date:** 2025-08-25T20:45:35Z

## Body

Compiler Explorer: https://godbolt.org/z/eEqc4oeET

Note:
- works: 20.1.0 and earlier
- fails: trunk, only at -O{s,2}
- perhaps related: https://github.com/llvm/llvm-project/issues/153946 

```
[516] % clangtk -v
clang version 22.0.0git (https://github.com/llvm/llvm-project.git fc6299065794d05bcab8f6bb9628441b3d071131)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /local/home/suz/suz-local/software/local/clang-trunk/bin
Build config: +assertions
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/11
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/12
Selected GCC installation: /usr/lib/gcc/x86_64-linux-gnu/12
Candidate multilib: .;@m64
Selected multilib: .;@m64
Found CUDA installation: /usr/local/cuda, version 12.1
[517] % 
[517] % clangtk -O2 small.c; ./a.out
[518] % 
[518] % clangtk -O2 -mllvm -force-vector-width=1 -mllvm -force-vector-interleave=2 small.c
[519] % ./a.out
Aborted
[520] % cat small.c
int a, b;
int f(int g) {
  a = 0;
  for (; a < 32; a++)
    if (g >> a & 1)
      return a;
  return 0;
}
int main() {
  if (f(b-2) != 1)
    __builtin_abort();
  return 0;
}
```

## Comments

### Comment 1 - MacDue

This seems to be an issue with early-exit vectorization not handling a return value depending on the induction variable correctly. It looks like it just passes through the vectorized induction `vp<%4>`:

```
<x1> vector loop: {
  vector.body:
    EMIT vp<%3> = CANONICAL-INDUCTION ir<0>, vp<%index.next>
    vp<%4> = SCALAR-STEPS vp<%3>, ir<1>, vp<%0>
    CLONE ir<%0> = shl nuw ir<1>, vp<%4>
    CLONE ir<%1> = and ir<%0>, ir<%g>
    CLONE ir<%tobool.not> = icmp eq ir<%1>, ir<0>
    EMIT vp<%index.next> = add nuw vp<%3>, vp<%1>
    EMIT vp<%5> = not ir<%tobool.not>
    EMIT vp<%6> = any-of vp<%5>
    EMIT vp<%7> = icmp eq vp<%index.next>, vp<%2>
    EMIT vp<%8> = or vp<%6>, vp<%7>
    EMIT branch-on-cond vp<%8>
  No successors
}
Successor(s): middle.split

middle.split:
  EMIT branch-on-cond vp<%6>
Successor(s): vector.early.exit, middle.block

middle.block:
  EMIT vp<%cmp.n> = icmp eq ir<32>, vp<%2>
  EMIT branch-on-cond vp<%cmp.n>
Successor(s): ir-bb<return>, scalar.ph

vector.early.exit:
Successor(s): ir-bb<return>

ir-bb<return>:
  IR   %storemerge.lcssa = phi i32 [ 32, %for.inc ], [ %storemerge2, %for.body ] (extra operands: ir<32> from middle.block, vp<%4> from vector.early.exit)
  IR   %retval.0 = phi i32 [ 0, %for.inc ], [ %storemerge2, %for.body ] (extra operands: ir<0> from middle.block, vp<%4> from vector.early.exit)
  IR   store i32 %storemerge.lcssa, ptr @a, align 4, !tbaa !7
No successors
```


---

### Comment 2 - fhahn

Don't think this is related to https://github.com/llvm/llvm-project/issues/153946, it is specific to VF = 1. Will look into a fix

---

