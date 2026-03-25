# [RISCV] Incorrect disassembly of c.lui when imm=0

**Author:** paulhuggett
**URL:** https://github.com/llvm/llvm-project/issues/133446
**Status:** Closed
**Labels:** backend:RISC-V
**Closed Date:** 2025-03-29T20:44:57Z

## Body


# context:

During disassembly, a binary can contain an invalid instruction and `objdump` tells that this instruction is unknown.

For example, Bytecode 0x6101 represents instruction `c.addi16sp x2, 0` (where `c.addi16sp` shares the opcode with `c.lui`, but has a destination field of `x2`) and `c.addi16sp` is only valid when the immediate is not equal to zero.

If I try to disassemble a binary with Bytecode 0x6101, I get:

```
$ cat disasm.s
.insn 0x6101  # c.addi16sp x2, 0
$ clang -c --target=riscv32 -march=rv32imafdc disasm.s -o disasm.o
$ llvm-objdump -M no-aliases -d disasm.o

disasm.o: file format elf32-littleriscv

Disassembly of section .text:

00000000 <.text>:
       0: 6101          <unknown>
```


# description of the problem:

According to RISC-V Instruction Set Manual, instruction `c.lui rd, imm` is only valid when `rd != {x0, x2}`, and when the immediate is not equal to zero.

As a consequence, the following instruction is invalid:

```
$ cat compile.s
c.lui x1, 0
```

`llvm` verifies this before compilation:

```
$ clang -c --target=riscv32 -march=rv32imafdc compile.s -o compile.o
compile.s:1:11: error: immediate must be in [0xfffe0, 0xfffff] or [1, 31]
c.lui x1, 0
          ^
```

But, if a binary contains such invalid instruction, `objdump` says that this is valid:

```
$ cat problem.s
.insn 0x6081  # c.lui x1, 0
$ clang -c --target=riscv32 -march=rv32imafdc problem.s -o problem.o
$ llvm-objdump -M no-aliases -d problem.o

problem.o:  file format elf32-littleriscv

Disassembly of section .text:

00000000 <.text>:
       0: 6081          c.lui ra, 0x0
```

The problem occurs with the 31 following Bytecodes:

- 0x6001 is disassembled to `c.lui  zero, 0x0`
- 0x6081 is disassembled to `c.lui  ra, 0x0`
- 0x6181 is disassembled to `c.lui  gp, 0x0`
- 0x6201 is disassembled to `c.lui  tp, 0x0`
- 0x6281 is disassembled to `c.lui  t0, 0x0`
- 0x6301 is disassembled to `c.lui  t1, 0x0`
- 0x6381 is disassembled to `c.lui  t2, 0x0`
- 0x6401 is disassembled to `c.lui  s0, 0x0`
- 0x6481 is disassembled to `c.lui  s1, 0x0`
- 0x6501 is disassembled to `c.lui  a0, 0x0`
- 0x6581 is disassembled to `c.lui  a1, 0x0`
- 0x6601 is disassembled to `c.lui  a2, 0x0`
- 0x6681 is disassembled to `c.lui  a3, 0x0`
- 0x6701 is disassembled to `c.lui  a4, 0x0`
- 0x6781 is disassembled to `c.lui  a5, 0x0`
- 0x6801 is disassembled to `c.lui  a6, 0x0`
- 0x6881 is disassembled to `c.lui  a7, 0x0`
- 0x6901 is disassembled to `c.lui  s2, 0x0`
- 0x6981 is disassembled to `c.lui  s3, 0x0`
- 0x6a01 is disassembled to `c.lui  s4, 0x0`
- 0x6a81 is disassembled to `c.lui  s5, 0x0`
- 0x6b01 is disassembled to `c.lui  s6, 0x0`
- 0x6b81 is disassembled to `c.lui  s7, 0x0`
- 0x6c01 is disassembled to `c.lui  s8, 0x0`
- 0x6c81 is disassembled to `c.lui  s9, 0x0`
- 0x6d01 is disassembled to `c.lui  s10, 0x0`
- 0x6d81 is disassembled to `c.lui  s11, 0x0`
- 0x6e01 is disassembled to `c.lui  t3, 0x0`
- 0x6e81 is disassembled to `c.lui  t4, 0x0`
- 0x6f01 is disassembled to `c.lui  t5, 0x0`
- 0x6f81 is disassembled to `c.lui  t6, 0x0`


# expected result:

Since these 31 Bytecodes represent invalid instructions, `objdump` should say that the resulting instructions are unknown.



## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-risc-v

Author: Paul Bowen-Huggett (paulhuggett)

<details>

# context:

During disassembly, a binary can contain an invalid instruction and `objdump` tells that this instruction is unknown.

For example, Bytecode 0x6101 represents instruction `c.addi16sp x2, 0` (where `c.addi16sp` shares the opcode with `c.lui`, but has a destination field of `x2`) and `c.addi16sp` is only valid when the immediate is not equal to zero.

If I try to disassemble a binary with Bytecode 0x6101, I get:

```
$ cat disasm.s
.insn 0x6101  # c.addi16sp x2, 0
$ clang -c --target=riscv32 -march=rv32imafdc disasm.s -o disasm.o
$ llvm-objdump -M no-aliases -d disasm.o

disasm.o: file format elf32-littleriscv

Disassembly of section .text:

00000000 &lt;.text&gt;:
       0: 6101          &lt;unknown&gt;
```


# description of the problem:

According to RISC-V Instruction Set Manual, instruction `c.lui rd, imm` is only valid when `rd != {x0, x2}`, and when the immediate is not equal to zero.

As a consequence, the following instruction is invalid:

```
$ cat compile.s
c.lui x1, 0
```

`llvm` verifies this before compilation:

```
$ clang -c --target=riscv32 -march=rv32imafdc compile.s -o compile.o
compile.s:1:11: error: immediate must be in [0xfffe0, 0xfffff] or [1, 31]
c.lui x1, 0
          ^
```

But, if a binary contains such invalid instruction, `objdump` says that this is valid:

```
$ cat problem.s
.insn 0x6081  # c.lui x1, 0
$ clang -c --target=riscv32 -march=rv32imafdc problem.s -o problem.o
$ llvm-objdump -M no-aliases -d problem.o

problem.o:  file format elf32-littleriscv

Disassembly of section .text:

00000000 &lt;.text&gt;:
       0: 6081          c.lui ra, 0x0
```

The problem occurs with the 31 following Bytecodes:

- 0x6001 is disassembled to `c.lui  zero, 0x0`
- 0x6081 is disassembled to `c.lui  ra, 0x0`
- 0x6181 is disassembled to `c.lui  gp, 0x0`
- 0x6201 is disassembled to `c.lui  tp, 0x0`
- 0x6281 is disassembled to `c.lui  t0, 0x0`
- 0x6301 is disassembled to `c.lui  t1, 0x0`
- 0x6381 is disassembled to `c.lui  t2, 0x0`
- 0x6401 is disassembled to `c.lui  s0, 0x0`
- 0x6481 is disassembled to `c.lui  s1, 0x0`
- 0x6501 is disassembled to `c.lui  a0, 0x0`
- 0x6581 is disassembled to `c.lui  a1, 0x0`
- 0x6601 is disassembled to `c.lui  a2, 0x0`
- 0x6681 is disassembled to `c.lui  a3, 0x0`
- 0x6701 is disassembled to `c.lui  a4, 0x0`
- 0x6781 is disassembled to `c.lui  a5, 0x0`
- 0x6801 is disassembled to `c.lui  a6, 0x0`
- 0x6881 is disassembled to `c.lui  a7, 0x0`
- 0x6901 is disassembled to `c.lui  s2, 0x0`
- 0x6981 is disassembled to `c.lui  s3, 0x0`
- 0x6a01 is disassembled to `c.lui  s4, 0x0`
- 0x6a81 is disassembled to `c.lui  s5, 0x0`
- 0x6b01 is disassembled to `c.lui  s6, 0x0`
- 0x6b81 is disassembled to `c.lui  s7, 0x0`
- 0x6c01 is disassembled to `c.lui  s8, 0x0`
- 0x6c81 is disassembled to `c.lui  s9, 0x0`
- 0x6d01 is disassembled to `c.lui  s10, 0x0`
- 0x6d81 is disassembled to `c.lui  s11, 0x0`
- 0x6e01 is disassembled to `c.lui  t3, 0x0`
- 0x6e81 is disassembled to `c.lui  t4, 0x0`
- 0x6f01 is disassembled to `c.lui  t5, 0x0`
- 0x6f81 is disassembled to `c.lui  t6, 0x0`


# expected result:

Since these 31 Bytecodes represent invalid instructions, `objdump` should say that the resulting instructions are unknown.


</details>


---

### Comment 2 - paulhuggett

Fixed by PR #133450

---

