# [RISCV] Incorrect disassembly of c.slli when umm > 31

**Author:** paulhuggett
**URL:** https://github.com/llvm/llvm-project/issues/133712
**Status:** Closed
**Labels:** backend:RISC-V
**Closed Date:** 2025-03-31T15:51:35Z

## Body


# context:

During disassembly, a binary can contain an invalid instruction and `objdump` says that this instruction is unknown.

For example, Bytecode 0x9005 represents instruction `c.srli x8, 0x21` and, for RV32C, `c.srli` is only valid when the immediate is in the range [1, 31].

If I try to disassemble a binary with Bytecode 0x9005, I get:

```
$ cat disasm.s
.insn 0x9005    # c.srli x8, 0x21
$ clang -c --target=riscv32 -march=rv32imafdc disasm.s -o disasm.o
$ llvm-objdump -M no-aliases -d disasm.o

disasm.o: file format elf32-littleriscv

Disassembly of section .text:

00000000 <.text>:
       0: 9005          <unknown>
```

# description of the problem:

According to RISC-V Instruction Set Manual, for RV32C, instruction `c.slli` is only valid when the immediate is in the range [1, 31]. For all base ISAs, the code points with `rd = x0` are HINTs, except those with `shamt[5]=1` in RV32C (where `shamt` is the shift amount immediate). For RV32C, the code points with `shamt[5]=1` are designated for custom extensions.

As a consequence, the following instruction is invalid (with no custom extension):

```
$ cat compile.s
c.slli x0, 0x21
```
LLVM verifies this before compilation:
```
$ clang -c --target=riscv32 -march=rv32imafdc compile.s -o compile.o
compile.s:1:12: error: immediate must be an integer in the range [1, 31]
c.slli x0, 0x21
           ^
```
But, if a binary contains such an invalid instruction, `objdump` says that it is valid:
```
$ cat problem.s
.insn 0x1006  # c.slli x0, 0x21
$ clang -c --target=riscv32 -march=rv32imafdc problem.s -o problem.o
$ llvm-objdump -M no-aliases -d problem.o

problem.o:  file format elf32-littleriscv

Disassembly of section .text:

00000000 <.text>:
       0: 1006          c.slli  zero, 0x21
```

The problem occurs with the 32 following Bytecodes:

- 0x1002 is disassembled to `c.slli  zero, 0x20`
- 0x1006 is disassembled to `c.slli  zero, 0x21`
- 0x100a is disassembled to `c.slli  zero, 0x22`
- 0x100e is disassembled to `c.slli  zero, 0x23`
- 0x1012 is disassembled to `c.slli  zero, 0x24`
- 0x1016 is disassembled to `c.slli  zero, 0x25`
- 0x101a is disassembled to `c.slli  zero, 0x26`
- 0x101e is disassembled to `c.slli  zero, 0x27`
- 0x1022 is disassembled to `c.slli  zero, 0x28`
- 0x1026 is disassembled to `c.slli  zero, 0x29`
- 0x102a is disassembled to `c.slli  zero, 0x2a`
- 0x102e is disassembled to `c.slli  zero, 0x2b`
- 0x1032 is disassembled to `c.slli  zero, 0x2c`
- 0x1036 is disassembled to `c.slli  zero, 0x2d`
- 0x103a is disassembled to `c.slli  zero, 0x2e`
- 0x103e is disassembled to `c.slli  zero, 0x2f`
- 0x1042 is disassembled to `c.slli  zero, 0x30`
- 0x1046 is disassembled to `c.slli  zero, 0x31`
- 0x104a is disassembled to `c.slli  zero, 0x32`
- 0x104e is disassembled to `c.slli  zero, 0x33`
- 0x1052 is disassembled to `c.slli  zero, 0x34`
- 0x1056 is disassembled to `c.slli  zero, 0x35`
- 0x105a is disassembled to `c.slli  zero, 0x36`
- 0x105e is disassembled to `c.slli  zero, 0x37`
- 0x1062 is disassembled to `c.slli  zero, 0x38`
- 0x1066 is disassembled to `c.slli  zero, 0x39`
- 0x106a is disassembled to `c.slli  zero, 0x3a`
- 0x106e is disassembled to `c.slli  zero, 0x3b`
- 0x1072 is disassembled to `c.slli  zero, 0x3c`
- 0x1076 is disassembled to `c.slli  zero, 0x3d`
- 0x107a is disassembled to `c.slli  zero, 0x3e`
- 0x107e is disassembled to `c.slli  zero, 0x3f`


# expected result:

Since these 32 Bytecodes represent invalid instructions, `objdump` should report that the resulting instructions are unknown.


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-risc-v

Author: Paul Bowen-Huggett (paulhuggett)

<details>

# context:

During disassembly, a binary can contain an invalid instruction and `objdump` says that this instruction is unknown.

For example, Bytecode 0x9005 represents instruction `c.srli x8, 0x21` and, for RV32C, `c.srli` is only valid when the immediate is in the range [1, 31].

If I try to disassemble a binary with Bytecode 0x9005, I get:

```
$ cat disasm.s
.insn 0x9005    # c.srli x8, 0x21
$ clang -c --target=riscv32 -march=rv32imafdc disasm.s -o disasm.o
$ llvm-objdump -M no-aliases -d disasm.o

disasm.o: file format elf32-littleriscv

Disassembly of section .text:

00000000 &lt;.text&gt;:
       0: 9005          &lt;unknown&gt;
```

# description of the problem:

According to RISC-V Instruction Set Manual, for RV32C, instruction `c.slli` is only valid when the immediate is in the range [1, 31]. For all base ISAs, the code points with `rd = x0` are HINTs, except those with `shamt[5]=1` in RV32C (where `shamt` is the shift amount immediate). For RV32C, the code points with `shamt[5]=1` are designated for custom extensions.

As a consequence, the following instruction is invalid (with no custom extension):

```
$ cat compile.s
c.slli x0, 0x21
```
LLVM verifies this before compilation:
```
$ clang -c --target=riscv32 -march=rv32imafdc compile.s -o compile.o
compile.s:1:12: error: immediate must be an integer in the range [1, 31]
c.slli x0, 0x21
           ^
```
But, if a binary contains such an invalid instruction, `objdump` says that it is valid:
```
$ cat problem.s
.insn 0x1006  # c.slli x0, 0x21
$ clang -c --target=riscv32 -march=rv32imafdc problem.s -o problem.o
$ llvm-objdump -M no-aliases -d problem.o

problem.o:  file format elf32-littleriscv

Disassembly of section .text:

00000000 &lt;.text&gt;:
       0: 1006          c.slli  zero, 0x21
```

The problem occurs with the 32 following Bytecodes:

- 0x1002 is disassembled to `c.slli  zero, 0x20`
- 0x1006 is disassembled to `c.slli  zero, 0x21`
- 0x100a is disassembled to `c.slli  zero, 0x22`
- 0x100e is disassembled to `c.slli  zero, 0x23`
- 0x1012 is disassembled to `c.slli  zero, 0x24`
- 0x1016 is disassembled to `c.slli  zero, 0x25`
- 0x101a is disassembled to `c.slli  zero, 0x26`
- 0x101e is disassembled to `c.slli  zero, 0x27`
- 0x1022 is disassembled to `c.slli  zero, 0x28`
- 0x1026 is disassembled to `c.slli  zero, 0x29`
- 0x102a is disassembled to `c.slli  zero, 0x2a`
- 0x102e is disassembled to `c.slli  zero, 0x2b`
- 0x1032 is disassembled to `c.slli  zero, 0x2c`
- 0x1036 is disassembled to `c.slli  zero, 0x2d`
- 0x103a is disassembled to `c.slli  zero, 0x2e`
- 0x103e is disassembled to `c.slli  zero, 0x2f`
- 0x1042 is disassembled to `c.slli  zero, 0x30`
- 0x1046 is disassembled to `c.slli  zero, 0x31`
- 0x104a is disassembled to `c.slli  zero, 0x32`
- 0x104e is disassembled to `c.slli  zero, 0x33`
- 0x1052 is disassembled to `c.slli  zero, 0x34`
- 0x1056 is disassembled to `c.slli  zero, 0x35`
- 0x105a is disassembled to `c.slli  zero, 0x36`
- 0x105e is disassembled to `c.slli  zero, 0x37`
- 0x1062 is disassembled to `c.slli  zero, 0x38`
- 0x1066 is disassembled to `c.slli  zero, 0x39`
- 0x106a is disassembled to `c.slli  zero, 0x3a`
- 0x106e is disassembled to `c.slli  zero, 0x3b`
- 0x1072 is disassembled to `c.slli  zero, 0x3c`
- 0x1076 is disassembled to `c.slli  zero, 0x3d`
- 0x107a is disassembled to `c.slli  zero, 0x3e`
- 0x107e is disassembled to `c.slli  zero, 0x3f`


# expected result:

Since these 32 Bytecodes represent invalid instructions, `objdump` should report that the resulting instructions are unknown.

</details>


---

