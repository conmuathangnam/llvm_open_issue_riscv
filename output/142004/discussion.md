# [RISC-V] Miscompile on -O[1-3]

**Author:** ewlu
**URL:** https://github.com/llvm/llvm-project/issues/142004
**Status:** Closed
**Labels:** miscompilation, llvm:SelectionDAG
**Closed Date:** 2025-05-30T23:00:44Z

## Body

Testcase:
```c
unsigned short b;
long long n;
long d;
long f;
int i;
unsigned short a = 293;
int main() {
  a = a < ((unsigned short)((unsigned char)~f | (d ?: 0)));
  b = i;
  n ^= a;
  n ^= b;
  __builtin_printf("%llu\n", n);
}
```

Commands:
```
# O3
$ /scratch/ewlu/daily-upstream-build/build-gcv/bin/clang -O3 red.c -o user-config.out -fsigned-char -fno-strict-aliasing -fwrapv 
$ QEMU_CPU=rv64,vlen=128,rvv_ta_all_1s=true,rvv_ma_all_1s=true,v=true,vext_spec=v1.0,zve32f=true,zve64f=true timeout --verbose -k 0.1 4 /scratch/ewlu/daily-upstream-build/build-gcv/bin/qemu-riscv64 user-config.out 1
1

# O2
$ /scratch/ewlu/daily-upstream-build/build-gcv/bin/clang -O2 red.c -o user-config.out -fsigned-char -fno-strict-aliasing -fwrapv 
$ QEMU_CPU=rv64,vlen=128,rvv_ta_all_1s=true,rvv_ma_all_1s=true,v=true,vext_spec=v1.0,zve32f=true,zve64f=true timeout --verbose -k 0.1 4 /scratch/ewlu/daily-upstream-build/build-gcv/bin/qemu-riscv64 user-config.out 1
1

# O1 
$ /scratch/ewlu/daily-upstream-build/build-gcv/bin/clang -O1 red.c -o user-config.out -fsigned-char -fno-strict-aliasing -fwrapv 
$ QEMU_CPU=rv64,vlen=128,rvv_ta_all_1s=true,rvv_ma_all_1s=true,v=true,vext_spec=v1.0,zve32f=true,zve64f=true timeout --verbose -k 0.1 4 /scratch/ewlu/daily-upstream-build/build-gcv/bin/qemu-riscv64 user-config.out 1
1

# O0 
$ /scratch/ewlu/daily-upstream-build/build-gcv/bin/clang -O0 red.c -o user-config.out -fsigned-char -fno-strict-aliasing -fwrapv 
$ QEMU_CPU=rv64,vlen=128,rvv_ta_all_1s=true,rvv_ma_all_1s=true,v=true,vext_spec=v1.0,zve32f=true,zve64f=true timeout --verbose -k 0.1 4 /scratch/ewlu/daily-upstream-build/build-gcv/bin/qemu-riscv64 user-config.out 1
0
```

First detected 5/23/2025. I haven't been able to bisect the commit which caused the error. Went back several release versions with my bisect script and error kept appearing. I'm hoping it's not an environment thing?

Found via fuzzer.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-risc-v

Author: Edwin Lu (ewlu)

<details>
Testcase:
```c
unsigned short b;
long long n;
long d;
long f;
int i;
unsigned short a = 293;
int main() {
  a = a &lt; ((unsigned short)((unsigned char)~f | (d ?: 0)));
  b = i;
  n ^= a;
  n ^= b;
  __builtin_printf("%llu\n", n);
}
```

Commands:
```
# O3
$ /scratch/ewlu/daily-upstream-build/build-gcv/bin/clang -O3 red.c -o user-config.out -fsigned-char -fno-strict-aliasing -fwrapv 
$ QEMU_CPU=rv64,vlen=128,rvv_ta_all_1s=true,rvv_ma_all_1s=true,v=true,vext_spec=v1.0,zve32f=true,zve64f=true timeout --verbose -k 0.1 4 /scratch/ewlu/daily-upstream-build/build-gcv/bin/qemu-riscv64 user-config.out 1
1

# O2
$ /scratch/ewlu/daily-upstream-build/build-gcv/bin/clang -O2 red.c -o user-config.out -fsigned-char -fno-strict-aliasing -fwrapv 
$ QEMU_CPU=rv64,vlen=128,rvv_ta_all_1s=true,rvv_ma_all_1s=true,v=true,vext_spec=v1.0,zve32f=true,zve64f=true timeout --verbose -k 0.1 4 /scratch/ewlu/daily-upstream-build/build-gcv/bin/qemu-riscv64 user-config.out 1
1

# O1 
$ /scratch/ewlu/daily-upstream-build/build-gcv/bin/clang -O1 red.c -o user-config.out -fsigned-char -fno-strict-aliasing -fwrapv 
$ QEMU_CPU=rv64,vlen=128,rvv_ta_all_1s=true,rvv_ma_all_1s=true,v=true,vext_spec=v1.0,zve32f=true,zve64f=true timeout --verbose -k 0.1 4 /scratch/ewlu/daily-upstream-build/build-gcv/bin/qemu-riscv64 user-config.out 1
1

# O0 
$ /scratch/ewlu/daily-upstream-build/build-gcv/bin/clang -O0 red.c -o user-config.out -fsigned-char -fno-strict-aliasing -fwrapv 
$ QEMU_CPU=rv64,vlen=128,rvv_ta_all_1s=true,rvv_ma_all_1s=true,v=true,vext_spec=v1.0,zve32f=true,zve64f=true timeout --verbose -k 0.1 4 /scratch/ewlu/daily-upstream-build/build-gcv/bin/qemu-riscv64 user-config.out 1
0
```

First detected 5/23/2025. I haven't been able to bisect the commit which caused the error. Went back several release versions with my bisect script and error kept appearing. I'm hoping it's not an environment thing?

Found via fuzzer.
</details>


---

### Comment 2 - topperc

-opt-bisect-limit points at the ConstantHoisting pass. Which is really odd

---

