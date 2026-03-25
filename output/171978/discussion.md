# [RISCV] Wrong code at -O2/O3

**Author:** XChy
**URL:** https://github.com/llvm/llvm-project/issues/171978

## Body

Reduced testcase:
```c
#include <stdint.h>
#include <stdio.h>
#define BS_VEC(type, num) type __attribute__((vector_size(num * sizeof(type))))
#define BITCAST(T, F, arg) (union { T dst }){ arg }.dst
uint64_t BS_CHECKSUM;

#include <riscv_bitmanip.h>
#define BARRIER_u32(x) (uint32_t)(__riscv_orc_b_32(1) + x - 0xff)

int16_t backsmith_snippet_246(uint64_t BS_ARG_0, BS_VEC(uint64_t, 32) BS_ARG_1)
{
    printf("%u\n", BARRIER_u32(0));
    BS_ARG_1[BARRIER_u32(0)] &= BS_ARG_0;
    return BS_ARG_1[0];
}
BS_VEC(int8_t, 32) backsmith_pure_2(int16_t)
{
    BS_VEC(uint32_t, 8) BS_VAR_1 = { 0, 7, 0, 0, 1, 800586355 };
    int16_t BS_TEMP_22 = backsmith_snippet_246(
        0,
        __builtin_convertvector(
            __builtin_shufflevector(BS_VAR_1, BS_VAR_1, 5, 5, 2, 9, 4, 4, 2, 0,
                                    2, 2, 4, 5, 3, 4, 0, 5, 7, 7, 3, 7, 0, 1, 8,
                                    4, 1, 2, 0, 4, 4, 2, 2, 2),
            BS_VEC(uint64_t, 32)));
    BS_VAR_1 = 0 == (BS_VEC(uint16_t, 16)){ BS_TEMP_22, BS_TEMP_22, BS_TEMP_22,
                                            BS_TEMP_22, BS_TEMP_22, BS_TEMP_22,
                                            BS_TEMP_22, BS_TEMP_22, BS_TEMP_22,
                                            BS_TEMP_22, BS_TEMP_22, BS_TEMP_22,
                                            BS_TEMP_22, BS_TEMP_22, BS_TEMP_22,
                                            BS_TEMP_22 };
    uint16_t BS_TEMP_81 = BS_VAR_1[3];
    BS_VAR_1 -= __builtin_convertvector(
        (BS_VEC(uint16_t, 8)){ BS_TEMP_81, BS_TEMP_81, BS_TEMP_81, BS_TEMP_81,
                               BS_TEMP_81 },
        BS_VEC(uint32_t, 8));
    return __builtin_convertvector(
        __builtin_convertvector(
            __builtin_shufflevector(BS_VAR_1, BS_VAR_1, 0, 1, 4, 1, 7, 6, 3, 5,
                                    1, 5, 4, 0, 7, 4, 9, 4, 5, 2, 1, 9, 2, 7, 0,
                                    2, 4, 3, 4, 9, 1, 5, 6, 6),
            BS_VEC(uint32_t, 32))
            * __builtin_convertvector(
                __builtin_shufflevector(
                    (BS_VEC(uint8_t, 8)){ 0, 9 }, (BS_VEC(uint8_t, 8)){}, 3, 5,
                    9, 1, 1, 0, 2, 0, 4, 4, 3, 5, 5, 8, 3, 1, 8, 0, 2, 3, 5, 1,
                    2, 3, 1, 0, 3, 7, 2, 4, 7, 9),
                BS_VEC(uint32_t, 32)),
        BS_VEC(int8_t, 32));
}

int zero;
int64_t func_19()
{
    BS_VEC(uint32_t, 4) BS_VAR_3[7];
    for (int i = 0; i <= 2; i += 1)
    {
        for (; zero;)
        {
            for (uint32_t BS_TEMP_166; BS_TEMP_166; BS_TEMP_166++)
                for (uint32_t BS_TEMP_167; BS_TEMP_167 < 4; BS_TEMP_167++)
                {
                    BS_CHECKSUM ^= BS_VAR_3[BS_TEMP_166][BS_TEMP_167];
                BS_LABEL_9:
                }
            int volatile BS_COND_54;
            switch (BS_COND_54)
            case 7: goto BS_LABEL_9;
        }
        BS_VAR_3[backsmith_pure_2(0)[15]] = (BS_VEC(uint32_t, 4)){};
    }
    return 0;
}
void main() { func_19(); }
```

Command:
```sh
> clang --target=riscv64-unknown-linux-musl --sysroot=/opt/riscv64/riscv64-unknown-linux-musl/sysroot --gcc-toolchain=/opt/riscv64 -march=rv64gcvb_zicond -static -O2 test.c
> qemu-riscv64 a.out
Illegal instruction (core dumped)

> clang --target=riscv64-unknown-linux-musl --sysroot=/opt/riscv64/riscv64-unknown-linux-musl/sysroot --gcc-toolchain=/opt/riscv64 -march=rv64gcvb_zicond -static -O1 test.c
> qemu-riscv64 a.out
0
0
0
```

clang version 4427e34a6476721b53364e7dad97340dc38f5cb6
qemu-riscv64 version 10.1.2

If replace `#define BARRIER_u32(x) (uint32_t)(__riscv_orc_b_32(1) + x - 0xff)` with `#define BARRIER_u32(x) (uint32_t)(x)`, the program passes X86 UBSan and ASan: https://godbolt.org/z/zz9Wa88M1

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-risc-v

Author: Hongyu Chen (XChy)

<details>
Reduced testcase:
```c
#include &lt;stdint.h&gt;
#include &lt;stdio.h&gt;
#define BS_VEC(type, num) type __attribute__((vector_size(num * sizeof(type))))
#define BITCAST(T, F, arg) (union { T dst }){ arg }.dst
uint64_t BS_CHECKSUM;

#include &lt;riscv_bitmanip.h&gt;
#define BARRIER_u32(x) (uint32_t)(__riscv_orc_b_32(1) + x - 0xff)

int16_t backsmith_snippet_246(uint64_t BS_ARG_0, BS_VEC(uint64_t, 32) BS_ARG_1)
{
    printf("%u\n", BARRIER_u32(0));
    BS_ARG_1[BARRIER_u32(0)] &amp;= BS_ARG_0;
    return BS_ARG_1[0];
}
BS_VEC(int8_t, 32) backsmith_pure_2(int16_t)
{
    BS_VEC(uint32_t, 8) BS_VAR_1 = { 0, 7, 0, 0, 1, 800586355 };
    int16_t BS_TEMP_22 = backsmith_snippet_246(
        0,
        __builtin_convertvector(
            __builtin_shufflevector(BS_VAR_1, BS_VAR_1, 5, 5, 2, 9, 4, 4, 2, 0,
                                    2, 2, 4, 5, 3, 4, 0, 5, 7, 7, 3, 7, 0, 1, 8,
                                    4, 1, 2, 0, 4, 4, 2, 2, 2),
            BS_VEC(uint64_t, 32)));
    BS_VAR_1 = 0 == (BS_VEC(uint16_t, 16)){ BS_TEMP_22, BS_TEMP_22, BS_TEMP_22,
                                            BS_TEMP_22, BS_TEMP_22, BS_TEMP_22,
                                            BS_TEMP_22, BS_TEMP_22, BS_TEMP_22,
                                            BS_TEMP_22, BS_TEMP_22, BS_TEMP_22,
                                            BS_TEMP_22, BS_TEMP_22, BS_TEMP_22,
                                            BS_TEMP_22 };
    uint16_t BS_TEMP_81 = BS_VAR_1[3];
    BS_VAR_1 -= __builtin_convertvector(
        (BS_VEC(uint16_t, 8)){ BS_TEMP_81, BS_TEMP_81, BS_TEMP_81, BS_TEMP_81,
                               BS_TEMP_81 },
        BS_VEC(uint32_t, 8));
    return __builtin_convertvector(
        __builtin_convertvector(
            __builtin_shufflevector(BS_VAR_1, BS_VAR_1, 0, 1, 4, 1, 7, 6, 3, 5,
                                    1, 5, 4, 0, 7, 4, 9, 4, 5, 2, 1, 9, 2, 7, 0,
                                    2, 4, 3, 4, 9, 1, 5, 6, 6),
            BS_VEC(uint32_t, 32))
            * __builtin_convertvector(
                __builtin_shufflevector(
                    (BS_VEC(uint8_t, 8)){ 0, 9 }, (BS_VEC(uint8_t, 8)){}, 3, 5,
                    9, 1, 1, 0, 2, 0, 4, 4, 3, 5, 5, 8, 3, 1, 8, 0, 2, 3, 5, 1,
                    2, 3, 1, 0, 3, 7, 2, 4, 7, 9),
                BS_VEC(uint32_t, 32)),
        BS_VEC(int8_t, 32));
}

int zero;
int64_t func_19()
{
    BS_VEC(uint32_t, 4) BS_VAR_3[7];
    for (int i = 0; i &lt;= 2; i += 1)
    {
        for (; zero;)
        {
            for (uint32_t BS_TEMP_166; BS_TEMP_166; BS_TEMP_166++)
                for (uint32_t BS_TEMP_167; BS_TEMP_167 &lt; 4; BS_TEMP_167++)
                {
                    BS_CHECKSUM ^= BS_VAR_3[BS_TEMP_166][BS_TEMP_167];
                BS_LABEL_9:
                }
            int volatile BS_COND_54;
            switch (BS_COND_54)
            case 7: goto BS_LABEL_9;
        }
        BS_VAR_3[backsmith_pure_2(0)[15]] = (BS_VEC(uint32_t, 4)){};
    }
    return 0;
}
void main() { func_19(); }
```

Command:
```sh
&gt; clang --target=riscv64-unknown-linux-musl --sysroot=/opt/riscv64/riscv64-unknown-linux-musl/sysroot --gcc-toolchain=/opt/riscv64 -march=rv64gcvb_zicond -static -O2 test.c
&gt; qemu-riscv64 a.out
Illegal instruction (core dumped)

&gt; clang --target=riscv64-unknown-linux-musl --sysroot=/opt/riscv64/riscv64-unknown-linux-musl/sysroot --gcc-toolchain=/opt/riscv64 -march=rv64gcvb_zicond -static -O1 test.c
&gt; qemu-riscv64 a.out
0
0
0
```

clang version 4427e34a6476721b53364e7dad97340dc38f5cb6
qemu-riscv64 version 10.1.2

If replace `#define BARRIER_u32(x) (uint32_t)(__riscv_orc_b_32(1) + x - 0xff)` with `#define BARRIER_u32(x) (uint32_t)(x)`, the program passes X86 UBSan and ASan: https://godbolt.org/z/zz9Wa88M1
</details>


---

### Comment 2 - rishabhmadan19

I want to fix it.
Please assign to me.

---

### Comment 3 - wangpc-pp

The illegal instruction is `vwsubu.wv v10,v8,v8` because `v8` is of two `EEW`s:
```
(gdb) target remote localhost:2333
Remote debugging using localhost:2333
0x0000000000010510 in _start ()
(gdb) c
Continuing.

Program received signal SIGILL, Illegal instruction.
0x000000000001085e in func_19 () at reduced.c:63
63                          BS_CHECKSUM ^= BS_VAR_3[BS_TEMP_166][BS_TEMP_167];
(gdb) disassemble $pc-16,$pc+16
Dump of assembler code from 0x1084e to 0x1086e:
   0x000000000001084e <func_19+168>:    vsetvli zero,zero,e16,m2,ta,ma
   0x0000000000010852 <func_19+172>:    vmv.v.i v8,0
   0x0000000000010856 <func_19+176>:    vmerge.vim      v8,v8,-1,v0
   0x000000000001085a <func_19+180>:    vsetivli        zero,8,e16,m1,ta,ma
=> 0x000000000001085e <func_19+184>:    vwsubu.wv       v10,v8,v8
   0x0000000000010862 <func_19+188>:    vsetivli        zero,1,e8,m2,ta,ma
   0x0000000000010866 <func_19+192>:    vslidedown.vi   v8,v10,16
   0x000000000001086a <func_19+196>:    vmv.x.s a0,v8
End of assembler dump.
```
This is related to https://github.com/llvm/llvm-project/issues/80099 as well.

---

### Comment 4 - wangpc-pp

Reduced LLVM IR: https://godbolt.org/z/vr3z78r94

---

### Comment 5 - wangpc-pp

I don't know if we should fix this, because the source code intentionally interprets a vector source as two kinds of vectors. This is exactly what we should avoid. I think this issue just falls within the range of https://github.com/llvm/llvm-project/issues/80099.

---

### Comment 6 - XChy

@wangpc-pp Thanks for your information!
> I don't know if we should fix this, because the source code intentionally interprets a vector source as two kinds of vectors. This is exactly what we should avoid.

Is there a specification about this kind of vector operations at the language level, or is it just not allowed in the RISC-V backend? The information may help me filter out these cases. (Though I think even if we don't involve any vector in code, the middle-end may generate bitcast on vectors at IR level.

---

### Comment 7 - wangpc-pp

> [@wangpc-pp](https://github.com/wangpc-pp) Thanks for your information!
> 
> > I don't know if we should fix this, because the source code intentionally interprets a vector source as two kinds of vectors. This is exactly what we should avoid.
> 
> Is there a specification about this kind of vector operations at the language level, or is it just not allowed in the RISC-V backend? The information may help me filter out these cases. (Though I think even if we don't involve any vector in code, the middle-end may generate bitcast on vectors at IR level.

There is no spec for this behavior/restriction. The compiler usually won't generate such `bitcast`s unless you construct such code at the source level (just like this issue) and such cases usually don't make sense from the view of semantics. IIUC, this is not an emergency problem so we just burry our heads in the sand. We can fix this issue via approaches proposed in #80099, but I don't know if this worths the effort.

---

