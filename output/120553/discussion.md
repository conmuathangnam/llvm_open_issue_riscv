# [RISC-V] epilogue_begin is set incorrectly

**Author:** RamNalamothu
**URL:** https://github.com/llvm/llvm-project/issues/120553
**Status:** Closed
**Labels:** backend:RISC-V, debuginfo
**Closed Date:** 2025-01-22T08:06:40Z

## Body

For the following test,
```
int main(int argc, char **argv)
{
  int foo = 1;

  return 0;
}
```
GCC (riscv32) generates (https://godbolt.org/z/Wqze6n8Gs):
```
main:
.LFB0:
        .file 1 "/app/example.c"
        .loc 1 2 1
        .cfi_startproc
        addi    sp,sp,-48
        .cfi_def_cfa_offset 48
        sw      ra,44(sp)
        sw      s0,40(sp)
        .cfi_offset 1, -4
        .cfi_offset 8, -8
        addi    s0,sp,48
        .cfi_def_cfa 8, 0
        sw      a0,-36(s0)
        sw      a1,-40(s0)
        .loc 1 3 7
        li      a5,1
        sw      a5,-20(s0)
        .loc 1 5 10
        li      a5,0
        .loc 1 6 1
        mv      a0,a5
        lw      ra,44(sp)
        .cfi_restore 1
        lw      s0,40(sp)
        .cfi_restore 8
        .cfi_def_cfa 2, 48
        addi    sp,sp,48
        .cfi_def_cfa_offset 0
        jr      ra
        .cfi_endproc
.LFE0:
        .size   main, .-main
```
and Clang riscv32 generates (https://godbolt.org/z/TMGPsr44s):
```
main:
.Lfunc_begin0:
        .file   0 "/app" "/app/example.c" md5 0x8aa04db627be45b3215ab92eac2e23c5
        .file   1 "example.c" md5 0x8aa04db627be45b3215ab92eac2e23c5
        .loc    1 2 0
        .cfi_sections .debug_frame
        .cfi_startproc
        addi    sp, sp, -32
        .cfi_def_cfa_offset 32
        sw      ra, 28(sp)
        sw      s0, 24(sp)
        .cfi_offset ra, -4
        .cfi_offset s0, -8
        addi    s0, sp, 32
        .cfi_def_cfa s0, 0
        mv      a2, a0
        li      a0, 0
        sw      a0, -12(s0)
        sw      a2, -16(s0)
        sw      a1, -20(s0)
        li      a1, 1
.Ltmp0:
        .loc    1 3 7 prologue_end
        sw      a1, -24(s0)
        .cfi_def_cfa sp, 32
        lw      ra, 28(sp)                                        <<<<<<<<<<
        lw      s0, 24(sp)
        .cfi_restore ra
        .cfi_restore s0
        .loc    1 5 3 epilogue_begin                               <<<<<<<<<<
        addi    sp, sp, 32
        .cfi_def_cfa_offset 0
        ret
.Ltmp1:
.Lfunc_end0:
        .size   main, .Lfunc_end0-main
```
As can be seen from the Clang's output, the _epilogue_begin_ is set after the epilogue has actually begun. This creates a problem if we single step from line 3, or set a breakpoint on line 5, the FP has been restored to the parent frame's FP, and accessing variables goes to the wrong place.

$ gdb main.gcc
```
(gdb) b main
Breakpoint 1 at 0x10444: file main.c, line 3.
(gdb) c
Continuing.

Breakpoint 1, main (argc=1, argv=0x40800004) at main.c:3
3                 int foo = 1;
(gdb) si
0x00010446      3                 int foo = 1;
(gdb) p foo
$1 = 0
(gdb) si
5                   return 0;
(gdb) p foo
$2 = 1
(gdb) s
6       }
(gdb) p foo
$3 = 1
(gdb)

```
$ gdb main.llvm
```
(gdb) b main
Breakpoint 1 at 0x10496: file main.c, line 3.
(gdb) c
Continuing.

Breakpoint 1, main (argc=1, argv=0x407ffff4) at main.c:3
3                 int foo = 1;
(gdb) si
0x0001049a      3                 int foo = 1;
(gdb) p foo
$1 = 1
(gdb) si
0x0001049c      3                 int foo = 1;
(gdb) p foo
$2 = 1
(gdb) s
5                   return 0;
(gdb) p foo
$3 = -1242739775
(gdb)
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-bug

Author: Venkata Ramanaiah Nalamothu (RamNalamothu)

<details>
For the following test,
```
int main(int argc, char **argv)
{
  int foo = 1;

  return 0;
}
```
GCC (riscv32) generates (https://godbolt.org/z/Wqze6n8Gs):
```
main:
.LFB0:
        .file 1 "/app/example.c"
        .loc 1 2 1
        .cfi_startproc
        addi    sp,sp,-48
        .cfi_def_cfa_offset 48
        sw      ra,44(sp)
        sw      s0,40(sp)
        .cfi_offset 1, -4
        .cfi_offset 8, -8
        addi    s0,sp,48
        .cfi_def_cfa 8, 0
        sw      a0,-36(s0)
        sw      a1,-40(s0)
        .loc 1 3 7
        li      a5,1
        sw      a5,-20(s0)
        .loc 1 5 10
        li      a5,0
        .loc 1 6 1
        mv      a0,a5
        lw      ra,44(sp)
        .cfi_restore 1
        lw      s0,40(sp)
        .cfi_restore 8
        .cfi_def_cfa 2, 48
        addi    sp,sp,48
        .cfi_def_cfa_offset 0
        jr      ra
        .cfi_endproc
.LFE0:
        .size   main, .-main
```
and Clang riscv32 generates (https://godbolt.org/z/TMGPsr44s):
```
main:
.Lfunc_begin0:
        .file   0 "/app" "/app/example.c" md5 0x8aa04db627be45b3215ab92eac2e23c5
        .file   1 "example.c" md5 0x8aa04db627be45b3215ab92eac2e23c5
        .loc    1 2 0
        .cfi_sections .debug_frame
        .cfi_startproc
        addi    sp, sp, -32
        .cfi_def_cfa_offset 32
        sw      ra, 28(sp)
        sw      s0, 24(sp)
        .cfi_offset ra, -4
        .cfi_offset s0, -8
        addi    s0, sp, 32
        .cfi_def_cfa s0, 0
        mv      a2, a0
        li      a0, 0
        sw      a0, -12(s0)
        sw      a2, -16(s0)
        sw      a1, -20(s0)
        li      a1, 1
.Ltmp0:
        .loc    1 3 7 prologue_end
        sw      a1, -24(s0)
        .cfi_def_cfa sp, 32
        lw      ra, 28(sp)                                        &lt;&lt;&lt;&lt;&lt;&lt;&lt;&lt;&lt;&lt;
        lw      s0, 24(sp)
        .cfi_restore ra
        .cfi_restore s0
        .loc    1 5 3 epilogue_begin                               &lt;&lt;&lt;&lt;&lt;&lt;&lt;&lt;&lt;&lt;
        addi    sp, sp, 32
        .cfi_def_cfa_offset 0
        ret
.Ltmp1:
.Lfunc_end0:
        .size   main, .Lfunc_end0-main
```
As can be seen from the Clang's output, the _epilogue_begin_ is set after the epilogue has actually begun. This creates a problem if we single step from line 3, or set a breakpoint on line 5, the FP has been restored to the parent frame's FP, and accessing variables goes to the wrong place.

$ gdb main.gcc
```
(gdb) b main
Breakpoint 1 at 0x10444: file main.c, line 3.
(gdb) c
Continuing.

Breakpoint 1, main (argc=1, argv=0x40800004) at main.c:3
3                 int foo = 1;
(gdb) si
0x00010446      3                 int foo = 1;
(gdb) p foo
$1 = 0
(gdb) si
5                   return 0;
(gdb) p foo
$2 = 1
(gdb) s
6       }
(gdb) p foo
$3 = 1
(gdb)

```
$ gdb main.llvm
```
(gdb) b main
Breakpoint 1 at 0x10496: file main.c, line 3.
(gdb) c
Continuing.

Breakpoint 1, main (argc=1, argv=0x407ffff4) at main.c:3
3                 int foo = 1;
(gdb) si
0x0001049a      3                 int foo = 1;
(gdb) p foo
$1 = 1
(gdb) si
0x0001049c      3                 int foo = 1;
(gdb) p foo
$2 = 1
(gdb) s
5                   return 0;
(gdb) p foo
$3 = -1242739775
(gdb)
```
</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-debuginfo

Author: Venkata Ramanaiah Nalamothu (RamNalamothu)

<details>
For the following test,
```
int main(int argc, char **argv)
{
  int foo = 1;

  return 0;
}
```
GCC (riscv32) generates (https://godbolt.org/z/Wqze6n8Gs):
```
main:
.LFB0:
        .file 1 "/app/example.c"
        .loc 1 2 1
        .cfi_startproc
        addi    sp,sp,-48
        .cfi_def_cfa_offset 48
        sw      ra,44(sp)
        sw      s0,40(sp)
        .cfi_offset 1, -4
        .cfi_offset 8, -8
        addi    s0,sp,48
        .cfi_def_cfa 8, 0
        sw      a0,-36(s0)
        sw      a1,-40(s0)
        .loc 1 3 7
        li      a5,1
        sw      a5,-20(s0)
        .loc 1 5 10
        li      a5,0
        .loc 1 6 1
        mv      a0,a5
        lw      ra,44(sp)
        .cfi_restore 1
        lw      s0,40(sp)
        .cfi_restore 8
        .cfi_def_cfa 2, 48
        addi    sp,sp,48
        .cfi_def_cfa_offset 0
        jr      ra
        .cfi_endproc
.LFE0:
        .size   main, .-main
```
and Clang riscv32 generates (https://godbolt.org/z/TMGPsr44s):
```
main:
.Lfunc_begin0:
        .file   0 "/app" "/app/example.c" md5 0x8aa04db627be45b3215ab92eac2e23c5
        .file   1 "example.c" md5 0x8aa04db627be45b3215ab92eac2e23c5
        .loc    1 2 0
        .cfi_sections .debug_frame
        .cfi_startproc
        addi    sp, sp, -32
        .cfi_def_cfa_offset 32
        sw      ra, 28(sp)
        sw      s0, 24(sp)
        .cfi_offset ra, -4
        .cfi_offset s0, -8
        addi    s0, sp, 32
        .cfi_def_cfa s0, 0
        mv      a2, a0
        li      a0, 0
        sw      a0, -12(s0)
        sw      a2, -16(s0)
        sw      a1, -20(s0)
        li      a1, 1
.Ltmp0:
        .loc    1 3 7 prologue_end
        sw      a1, -24(s0)
        .cfi_def_cfa sp, 32
        lw      ra, 28(sp)                                        &lt;&lt;&lt;&lt;&lt;&lt;&lt;&lt;&lt;&lt;
        lw      s0, 24(sp)
        .cfi_restore ra
        .cfi_restore s0
        .loc    1 5 3 epilogue_begin                               &lt;&lt;&lt;&lt;&lt;&lt;&lt;&lt;&lt;&lt;
        addi    sp, sp, 32
        .cfi_def_cfa_offset 0
        ret
.Ltmp1:
.Lfunc_end0:
        .size   main, .Lfunc_end0-main
```
As can be seen from the Clang's output, the _epilogue_begin_ is set after the epilogue has actually begun. This creates a problem if we single step from line 3, or set a breakpoint on line 5, the FP has been restored to the parent frame's FP, and accessing variables goes to the wrong place.

$ gdb main.gcc
```
(gdb) b main
Breakpoint 1 at 0x10444: file main.c, line 3.
(gdb) c
Continuing.

Breakpoint 1, main (argc=1, argv=0x40800004) at main.c:3
3                 int foo = 1;
(gdb) si
0x00010446      3                 int foo = 1;
(gdb) p foo
$1 = 0
(gdb) si
5                   return 0;
(gdb) p foo
$2 = 1
(gdb) s
6       }
(gdb) p foo
$3 = 1
(gdb)

```
$ gdb main.llvm
```
(gdb) b main
Breakpoint 1 at 0x10496: file main.c, line 3.
(gdb) c
Continuing.

Breakpoint 1, main (argc=1, argv=0x407ffff4) at main.c:3
3                 int foo = 1;
(gdb) si
0x0001049a      3                 int foo = 1;
(gdb) p foo
$1 = 1
(gdb) si
0x0001049c      3                 int foo = 1;
(gdb) p foo
$2 = 1
(gdb) s
5                   return 0;
(gdb) p foo
$3 = -1242739775
(gdb)
```
</details>


---

### Comment 3 - llvmbot


@llvm/issue-subscribers-backend-risc-v

Author: Venkata Ramanaiah Nalamothu (RamNalamothu)

<details>
For the following test,
```
int main(int argc, char **argv)
{
  int foo = 1;

  return 0;
}
```
GCC (riscv32) generates (https://godbolt.org/z/Wqze6n8Gs):
```
main:
.LFB0:
        .file 1 "/app/example.c"
        .loc 1 2 1
        .cfi_startproc
        addi    sp,sp,-48
        .cfi_def_cfa_offset 48
        sw      ra,44(sp)
        sw      s0,40(sp)
        .cfi_offset 1, -4
        .cfi_offset 8, -8
        addi    s0,sp,48
        .cfi_def_cfa 8, 0
        sw      a0,-36(s0)
        sw      a1,-40(s0)
        .loc 1 3 7
        li      a5,1
        sw      a5,-20(s0)
        .loc 1 5 10
        li      a5,0
        .loc 1 6 1
        mv      a0,a5
        lw      ra,44(sp)
        .cfi_restore 1
        lw      s0,40(sp)
        .cfi_restore 8
        .cfi_def_cfa 2, 48
        addi    sp,sp,48
        .cfi_def_cfa_offset 0
        jr      ra
        .cfi_endproc
.LFE0:
        .size   main, .-main
```
and Clang riscv32 generates (https://godbolt.org/z/TMGPsr44s):
```
main:
.Lfunc_begin0:
        .file   0 "/app" "/app/example.c" md5 0x8aa04db627be45b3215ab92eac2e23c5
        .file   1 "example.c" md5 0x8aa04db627be45b3215ab92eac2e23c5
        .loc    1 2 0
        .cfi_sections .debug_frame
        .cfi_startproc
        addi    sp, sp, -32
        .cfi_def_cfa_offset 32
        sw      ra, 28(sp)
        sw      s0, 24(sp)
        .cfi_offset ra, -4
        .cfi_offset s0, -8
        addi    s0, sp, 32
        .cfi_def_cfa s0, 0
        mv      a2, a0
        li      a0, 0
        sw      a0, -12(s0)
        sw      a2, -16(s0)
        sw      a1, -20(s0)
        li      a1, 1
.Ltmp0:
        .loc    1 3 7 prologue_end
        sw      a1, -24(s0)
        .cfi_def_cfa sp, 32
        lw      ra, 28(sp)                                        &lt;&lt;&lt;&lt;&lt;&lt;&lt;&lt;&lt;&lt;
        lw      s0, 24(sp)
        .cfi_restore ra
        .cfi_restore s0
        .loc    1 5 3 epilogue_begin                               &lt;&lt;&lt;&lt;&lt;&lt;&lt;&lt;&lt;&lt;
        addi    sp, sp, 32
        .cfi_def_cfa_offset 0
        ret
.Ltmp1:
.Lfunc_end0:
        .size   main, .Lfunc_end0-main
```
As can be seen from the Clang's output, the _epilogue_begin_ is set after the epilogue has actually begun. This creates a problem if we single step from line 3, or set a breakpoint on line 5, the FP has been restored to the parent frame's FP, and accessing variables goes to the wrong place.

$ gdb main.gcc
```
(gdb) b main
Breakpoint 1 at 0x10444: file main.c, line 3.
(gdb) c
Continuing.

Breakpoint 1, main (argc=1, argv=0x40800004) at main.c:3
3                 int foo = 1;
(gdb) si
0x00010446      3                 int foo = 1;
(gdb) p foo
$1 = 0
(gdb) si
5                   return 0;
(gdb) p foo
$2 = 1
(gdb) s
6       }
(gdb) p foo
$3 = 1
(gdb)

```
$ gdb main.llvm
```
(gdb) b main
Breakpoint 1 at 0x10496: file main.c, line 3.
(gdb) c
Continuing.

Breakpoint 1, main (argc=1, argv=0x407ffff4) at main.c:3
3                 int foo = 1;
(gdb) si
0x0001049a      3                 int foo = 1;
(gdb) p foo
$1 = 1
(gdb) si
0x0001049c      3                 int foo = 1;
(gdb) p foo
$2 = 1
(gdb) s
5                   return 0;
(gdb) p foo
$3 = -1242739775
(gdb)
```
</details>


---

### Comment 4 - RamNalamothu

Also, in Clang generated code, the _prologue_end_ is set after the user code i.e. after _li      a1, 1_, which is happening due to https://reviews.llvm.org/D48468. But GCC generated code doesn't have this issue.

I have a work in progress patch to fix the _epilogue_begin_ issue.

---

### Comment 5 - wangpc-pp

Please check if this is a duplicated work: https://github.com/llvm/llvm-project/pull/74702

---

### Comment 6 - RamNalamothu

> Please check if this is a duplicated work: [#74702](https://github.com/llvm/llvm-project/pull/74702)

Thanks for pointing it out but I am taking a different approach.
Hopefully, I should be able to publish the patches in the next 1-2 days.

---

