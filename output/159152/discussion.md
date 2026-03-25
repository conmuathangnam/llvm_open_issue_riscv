# [RISC-V] Miscompile at -O3 with -flto

**Author:** ewlu
**URL:** https://github.com/llvm/llvm-project/issues/159152
**Status:** Closed
**Labels:** backend:RISC-V, miscompilation, release:backport, LTO
**Closed Date:** 2025-09-19T16:19:58Z

## Body

Testcase:
```c
long long s;
void hash(long long *s, long t) { *s ^= t + 9 + *s; }
signed char o[25][25][25];
unsigned a[25];
void checksum() {
  for (int p = 0; p < 25; ++p)
    hash(&s, a[p]);
}
int main() {
  for (int p = 0; p < 25; ++p)
    for (int q = 0; q < 25; ++q)
      for (int u = 0; u < 25; ++u)
        o[p][q][u] = 90;
  for (short p = 0; p < 25; p++)
    for (int q = 0; q < 25; q++) {
      a[q] = -(o[p][p][p] ? o[p][4][q] : 0);
    }
  checksum();
  __builtin_printf("%llu\n", s);
}
```
Commands:
```
# O3
$ /scratch/ewlu/daily-upstream-build/build-gcv/bin/clang -march=rv64gcv -flto -O3 red.c -o user-config.out -fsigned-char -fno-strict-aliasing -fwrapv 
$ QEMU_CPU=rv64,vlen=128,rvv_ta_all_1s=true,rvv_ma_all_1s=true,v=true,vext_spec=v1.0,zve32f=true,zve64f=true timeout --verbose -k 0.1 4 /scratch/ewlu/daily-upstream-build/build-gcv/bin/qemu-riscv64 user-config.out 1
2194728288337
# O2
$ /scratch/ewlu/daily-upstream-build/build-gcv/bin/clang -march=rv64gcv -flto -O2 red.c -o user-config.out -fsigned-char -fno-strict-aliasing -fwrapv 
$ QEMU_CPU=rv64,vlen=128,rvv_ta_all_1s=true,rvv_ma_all_1s=true,v=true,vext_spec=v1.0,zve32f=true,zve64f=true timeout --verbose -k 0.1 4 /scratch/ewlu/daily-upstream-build/build-gcv/bin/qemu-riscv64 user-config.out 1
72057589742960721
```
Found via fuzzer.

Bisected to 571c8c2c88122d318ed84cd9e948613e3f1aac5f as the first bad commit

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-risc-v

Author: Edwin Lu (ewlu)

<details>
Testcase:
```c
long long s;
void hash(long long *s, long t) { *s ^= t + 9 + *s; }
signed char o[25][25][25];
unsigned a[25];
void checksum() {
  for (int p = 0; p &lt; 25; ++p)
    hash(&amp;s, a[p]);
}
int main() {
  for (int p = 0; p &lt; 25; ++p)
    for (int q = 0; q &lt; 25; ++q)
      for (int u = 0; u &lt; 25; ++u)
        o[p][q][u] = 90;
  for (short p = 0; p &lt; 25; p++)
    for (int q = 0; q &lt; 25; q++) {
      a[q] = -(o[p][p][p] ? o[p][4][q] : 0);
    }
  checksum();
  __builtin_printf("%llu\n", s);
}
```
Commands:
```
# O3
$ /scratch/ewlu/daily-upstream-build/build-gcv/bin/clang -march=rv64gcv -flto -O3 red.c -o user-config.out -fsigned-char -fno-strict-aliasing -fwrapv 
$ QEMU_CPU=rv64,vlen=128,rvv_ta_all_1s=true,rvv_ma_all_1s=true,v=true,vext_spec=v1.0,zve32f=true,zve64f=true timeout --verbose -k 0.1 4 /scratch/ewlu/daily-upstream-build/build-gcv/bin/qemu-riscv64 user-config.out 1
2194728288337
# O2
$ /scratch/ewlu/daily-upstream-build/build-gcv/bin/clang -march=rv64gcv -flto -O2 red.c -o user-config.out -fsigned-char -fno-strict-aliasing -fwrapv 
$ QEMU_CPU=rv64,vlen=128,rvv_ta_all_1s=true,rvv_ma_all_1s=true,v=true,vext_spec=v1.0,zve32f=true,zve64f=true timeout --verbose -k 0.1 4 /scratch/ewlu/daily-upstream-build/build-gcv/bin/qemu-riscv64 user-config.out 1
72057589742960721
```
Found via fuzzer.

Bisected to 571c8c2c88122d318ed84cd9e948613e3f1aac5f as the first bad commit
</details>


---

### Comment 2 - alexey-bataev

Is the bisecting correct? It points to a revert of a patch, so I doubt that's the cause

---

### Comment 3 - topperc

> Is the bisecting correct? It points to a revert of a patch, so I doubt that's the cause

opt-bisect-limit does point to SLP. It could still be a backend bug since opt-bisect-limit doesn't disable much of SelectionDAG.

---

### Comment 4 - topperc

This fixes it so I think it is a backend bug

```
diff --git a/llvm/lib/Target/RISCV/RISCVISelLowering.cpp b/llvm/lib/Target/RISCV/RISCVISelLowering.cpp
index 66ebda7aa586..9ffd10a346a8 100644
--- a/llvm/lib/Target/RISCV/RISCVISelLowering.cpp
+++ b/llvm/lib/Target/RISCV/RISCVISelLowering.cpp
@@ -17693,6 +17693,7 @@ static SDValue simplifyOp_VL(SDNode *N) {
 static SDValue combineOp_VLToVWOp_VL(SDNode *N,
                                      TargetLowering::DAGCombinerInfo &DCI,
                                      const RISCVSubtarget &Subtarget) {
+  return SDValue();
   SelectionDAG &DAG = DCI.DAG;
   if (DCI.isBeforeLegalize())
     return SDValue();
```

---

### Comment 5 - ewlu

> Is the bisecting correct? It points to a revert of a patch, so I doubt that's the cause

Something did feel off to me about this bisect. The fuzzer supposedly found this bug around 2 weeks ago but running at the `llvmorg-21-init` tag also resulted in a miscompile. I could have gone too far back when I was searching for my bisection range. 

---

### Comment 6 - alexey-bataev

Let me know, Craig, if you think it is still SLP bug, I will try to dig deeper

---

### Comment 7 - topperc

It is a RISC-V bug. I understand it. I'm trying to come up with the best fix.

---

### Comment 8 - topperc

/cherry-pick 6119d1f115625cd1b8a2b9d331609eb9e9f676ce

---

### Comment 9 - llvmbot

/pull-request llvm/llvm-project#159891

---

