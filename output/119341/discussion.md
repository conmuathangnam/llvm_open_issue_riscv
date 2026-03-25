# Clang: Both operands of conditional operator are evaluated

**Author:** wzssyqa
**URL:** https://github.com/llvm/llvm-project/issues/119341

## Body

In some cases both the second and third operands are evaluated, which is not allowed by the C standard.

I find this problem when I try to build glibc with Clang.
With the test of `powl` function, both `OVERFLOW` and `UNDERFLOW` exceptions are raised, while only one is expected.
I haven't figure out a short case.

How to reproduce
1. Download glibc 2.40 etc, and unpack it, and `cd` into the source tree
2.
   ```shell
   mkdir -p build-clang/math
   ```
3.
   ```shell
   cd build-clang
   ```
4. put the attached `abi-versions.h`, `config.h`, `first-versions.h`, `libc-modules.h` to `build-clang`
5.
   ```shell
   <PATH_OF_LLVM>/bin/clang --target=riscv64-linux-gnu ../sysdeps/ieee754/ldbl-128/e_powl.c -S -std=gnu11 -fgnu89-inline  -g -O2 -Wall -Wwrite-strings -Wundef -Werror -fmerge-all-constants -frounding-math -ftrapping-math -fno-stack-protector -fno-common -Wp,-U_FORTIFY_SOURCE -Wstrict-prototypes -Wold-style-definition -fno-math-errno    -fPIE   -fno-builtin-powf64x -fno-builtin-powf128       -I../include -Imath   -I../sysdeps/unix/sysv/linux/riscv/rv64  -I../sysdeps/unix/sysv/linux/riscv/include -I../sysdeps/unix/sysv/linux/riscv  -I../sysdeps/riscv/nptl  -I../sysdeps/unix/sysv/linux/wordsize-64  -I../sysdeps/unix/sysv/linux/riscv/multiarch  -I../sysdeps/unix/sysv/linux/include -I../sysdeps/unix/sysv/linux  -I../sysdeps/nptl  -I../sysdeps/pthread  -I../sysdeps/gnu  -I../sysdeps/unix/inet  -I../sysdeps/unix/sysv  -I../sysdeps/unix  -I../sysdeps/posix  -I../sysdeps/riscv/rv64/rvd  -I../sysdeps/riscv/rv64/rvf  -I../sysdeps/riscv/rvf  -I../sysdeps/riscv/rvd  -I../sysdeps/riscv/rv64  -I../sysdeps/riscv/multiarch  -I../sysdeps/riscv  -I../sysdeps/ieee754/ldbl-128  -I../sysdeps/ieee754/dbl-64  -I../sysdeps/ieee754/flt-32  -I../sysdeps/wordsize-64  -I../sysdeps/ieee754  -I../sysdeps/generic  -I.. -I../libio -I.  -D_LIBC_REENTRANT -include libc-modules.h -DMODULE_NAME=libm -include ../include/libc-symbols.h  -DPIC     -DTOP_NAMESPACE=glibc -o math/e_powl.o -MD -MP -MF math/e_powl.o.dt -MT math/e_powl.o
   ```

The C code is like
```c
  /* |y| is huge.
     2^-16495 = 1/2 of smallest representable value.
     If (1 - 1/131072)^y underflows, y > 1.4986e9 */
  if (iy > 0x401d654b) 
    {
      /* if (1 - 2^-113)^y underflows, y > 1.1873e38 */
      if (iy > 0x407d654b)
        {
          if (ix <= 0x3ffeffff)
            return (hy < 0) ? huge * huge : tiny * tiny;
          if (ix >= 0x3fff0000)
            return (hy > 0) ? huge * huge : tiny * tiny;
        }
      /* over/underflow if x is not close to one */
      if (ix < 0x3ffeffff)
        return (hy < 0) ? sgn * huge * huge : sgn * tiny * tiny;
      if (ix > 0x3fff0000)
        return (hy > 0) ? sgn * huge * huge : sgn * tiny * tiny;
    }
```


So we get a file `math/e_powl.o`,  which is an `asm text` file.
In it we can find there are code like:

```asm
.Ltmp87:
# %bb.51:                               # %if.then222
        #DEBUG_VALUE: __ieee754_powl:x <- [DW_OP_LLVM_fragment 0 64] $x26
        #DEBUG_VALUE: __ieee754_powl:x <- [DW_OP_LLVM_fragment 64 64] $x9
        #DEBUG_VALUE: __ieee754_powl:y <- [DW_OP_LLVM_fragment 0 64] $x19
        #DEBUG_VALUE: __ieee754_powl:y <- [DW_OP_LLVM_fragment 64 64] $x24
        #DEBUG_VALUE: __ieee754_powl:iy <- $x18
        .loc    0 0 14 is_stmt 0                # ../sysdeps/ieee754/ldbl-128/e_powl.c:0:14
        mv      s0, a3                          
.Lpcrel_hi1:
        auipc   a0, %pcrel_hi(.LCPI0_30)
        ld      a0, %pcrel_lo(.Lpcrel_hi1)(a0)
.Lpcrel_hi2:    
        auipc   a1, %pcrel_hi(.LCPI0_31)
        ld      a1, %pcrel_lo(.Lpcrel_hi2)(a1)
.Ltmp88:
        mv      a2, a0
        mv      a3, a1
        call    __multf3
.Ltmp89:
        mv      s10, a0
.Ltmp90:
        mv      s5, a1
.Lpcrel_hi3:
        auipc   a0, %pcrel_hi(.LCPI0_28)        
        ld      a0, %pcrel_lo(.Lpcrel_hi3)(a0)
.Lpcrel_hi4:
        auipc   a1, %pcrel_hi(.LCPI0_29)        
        ld      a1, %pcrel_lo(.Lpcrel_hi4)(a1)
        mv      a2, a0
        mv      a3, a1
        call    __multf3
.Ltmp91:
        lui     a2, 262128
        addiw   a2, a2, -1
.Ltmp92:
        .loc    0 281 11 is_stmt 1              # ../sysdeps/ieee754/ldbl-128/e_powl.c:281:11
        bltu    a2, s6, .LBB0_72        
```

Note that `__multf3` are called twice one by one without condition test.

## Comments

### Comment 1 - wzssyqa

[attached.tar.gz](https://github.com/user-attachments/files/18075403/attached.tar.gz)


---

### Comment 2 - wzssyqa

Ref: https://sourceware.org/pipermail/libc-alpha/2024-December/162295.html

---

