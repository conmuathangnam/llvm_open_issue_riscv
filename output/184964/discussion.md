# [MC][ARM] Clang crashes when assembling ARM Thumb2 LDRD/STRD post-indexed with non-4-byte-aligned offset

**Author:** venkyqz
**URL:** https://github.com/llvm/llvm-project/issues/184964
**Status:** Open
**Labels:** backend:ARM, llvm:mc, crash

## Body

## Summary

A cross-layer semantic gap exists in the ARM Thumb-2 assembly pipeline for post-indexed `ldrd` and `strd` instructions. The frontend `AsmParser` accepts unaligned offsets due to a missing `ParserMatchClass` in TableGen. This perfectly bypasses frontend validation, resulting in:
- **Debug Build**: Assertion failure (`Exit 134`)
- **Release Build**: Silent miscompilation (`Exit 0`, offset truncated to nearest 4-byte boundary)

This bug is triggerable entirely via plain text assembly.

---

## Affected Instructions

| Instruction | Opcode | Description |
|-------------|--------|-------------|
| `ldrd Rt, Rt2, [Rn], #imm` | `t2LDRD_POST` | Post-indexed load double |
| `strd Rt, Rt2, [Rn], #imm` | `t2STRD_POST` | Post-indexed store double |

---

## Reproduction
**Godbolt Link**
+ https://godbolt.org/z/oefoPMeYK

**Test File (`poc.s`):**

```asm
.syntax unified
.thumb
.text
ldrd r0, r1, [r2], #1
```

Any offset not strictly divisible by 4 (e.g., 1, 2, 3, 5, -1) will trigger the divergence.

**Commands:**

```bash
# Debug Build - Crashes with assertion
llvm-mc --arch=arm --triple=armv7-linux-gnueabihf --mcpu=cortex-a9 --filetype=asm --show-encoding poc.s
# Exit 134, Assertion failed

# Release Build - Silent miscompilation
llvm-mc --arch=arm --triple=armv7-linux-gnueabihf --mcpu=cortex-a9 --filetype=asm --show-encoding poc.s
# Exit 0, emits corrupted output: encoding: [0xf2,0xe8, 0x00,0x01] (#1 treated as #0)
```

## Debug Build Output

```
llvm-mc: /root/llvm-project/llvm/lib/Target/ARM/MCTargetDesc/ARMInstPrinter.cpp:1348: void llvm::ARMInstPrinter::printT2AddrModeImm8s4OffsetOperand(const llvm::MCInst*, unsigned int, const llvm::MCSubtargetInfo&, llvm::raw_ostream&): Assertion `((OffImm & 0x3) == 0) && "Not a valid immediate!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/llvm-mc -filetype=obj -o example.o --arch=arm --triple=armv7-linux-gnueabihf --mcpu=cortex-a9 --filetype=asm --show-encoding <source>
 #0 0x0000000000d41d58 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/llvm-mc+0xd41d58)
 #1 0x0000000000d3ee24 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x000071f7b8642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x000071f7b86969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x000071f7b8642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x000071f7b86287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x000071f7b862871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #7 0x000071f7b8639e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #8 0x00000000007d6b58 llvm::ARMInstPrinter::printT2AddrModeImm8s4OffsetOperand(llvm::MCInst const*, unsigned int, llvm::MCSubtargetInfo const&, llvm::raw_ostream&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llvm-mc+0x7d6b58)
 #9 0x00000000007e1b44 llvm::ARMInstPrinter::printInst(llvm::MCInst const*, unsigned long, llvm::StringRef, llvm::MCSubtargetInfo const&, llvm::raw_ostream&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llvm-mc+0x7e1b44)
#10 0x0000000000c0bc32 llvm::MCTargetStreamer::prettyPrintAsm(llvm::MCInstPrinter&, unsigned long, llvm::MCInst const&, llvm::MCSubtargetInfo const&, llvm::raw_ostream&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llvm-mc+0xc0bc32)
#11 0x0000000000bbd43b (anonymous namespace)::MCAsmStreamer::emitInstruction(llvm::MCInst const&, llvm::MCSubtargetInfo const&) MCAsmStreamer.cpp:0:0
#12 0x0000000000570401 (anonymous namespace)::ARMAsmParser::matchAndEmitInstruction(llvm::SMLoc, unsigned int&, llvm::SmallVectorImpl<std::unique_ptr<llvm::MCParsedAsmOperand, std::default_delete<llvm::MCParsedAsmOperand>>>&, llvm::MCStreamer&, unsigned long&, bool) ARMAsmParser.cpp:0:0
#13 0x0000000000c85857 (anonymous namespace)::AsmParser::parseAndMatchAndEmitTargetInstruction((anonymous namespace)::ParseStatementInfo&, llvm::StringRef, llvm::AsmToken, llvm::SMLoc) AsmParser.cpp:0:0
#14 0x0000000000c99d73 (anonymous namespace)::AsmParser::parseStatement((anonymous namespace)::ParseStatementInfo&, llvm::MCAsmParserSemaCallback*) (.part.0) AsmParser.cpp:0:0
#15 0x0000000000c9dfc3 (anonymous namespace)::AsmParser::Run(bool, bool) AsmParser.cpp:0:0
#16 0x000000000040c6ba main (/opt/compiler-explorer/clang-assertions-trunk/bin/llvm-mc+0x40c6ba)
#17 0x000071f7b8629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#18 0x000071f7b8629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#19 0x000000000047bbe5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/llvm-mc+0x47bbe5)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

---

## Debug vs Release Behavior

| Offset | Debug Exit | Release Exit | Release Encoding        | Expected           |
| ------ | ---------- | ------------ | ----------------------- | ------------------ |
| `#0`   | 0          | 0            | `[0xf2,0xe8,0x00,0x01]` | Accept             |
| `#1`   | **134**    | **0**        | `[0xf2,0xe8,0x00,0x01]` | **Reject** |
| `#2`   | **134**    | **0**        | `[0xf2,0xe8,0x00,0x01]` | **Reject** |
| `#3`   | **134**    | **0**        | `[0xf2,0xe8,0x00,0x01]` | **Reject** |
| `#4`   | 0          | 0            | `[0xf2,0xe8,0x01,0x01]` | Accept             |
| `#5`   | **134**    | **0**        | `[0xf2,0xe8,0x01,0x01]` | **Reject** |
| `#-1`  | **134**    | **0**        | `[0x72,0xe8,0x00,0x01]` | **Reject** |
| `#-4`  | 0          | 0            | `[0x72,0xe8,0x01,0x01]` | Accept             |

---

## Root Cause

The vulnerability stems from a declarative omission in TableGen. The operand `t2am_imm8s4_offset` (used for the post-indexed offset of `t2LDRD_POST` and `t2STRD_POST`) lacks a `ParserMatchClass`.

Unlike the pre-indexed variant (`t2addrmode_imm8s4`), which correctly uses `MemImm8s4OffsetAsmOperand`, the post-indexed variant falls back to accepting any immediate without 4-byte alignment validation.

**Buggy Code (`llvm/lib/Target/ARM/ARMInstrThumb2.td`):**

```
def t2am_imm8s4_offset_asmoperand : AsmOperandClass { let Name = "Imm8s4"; }

def t2am_imm8s4_offset : MemOperand {  // VULNERABILITY: Missing ParserMatchClass!
  let PrintMethod = "printT2AddrModeImm8s4OffsetOperand";
  let EncoderMethod = "getT2ScaledImmOpValue<8,2>";
  let DecoderMethod = "DecodeT2Imm8S4";
}
```

Because the `t2am_imm8s4_offset_asmoperand` class is defined but never assigned, the `AsmMatcher` accepts unaligned immediate.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-arm

Author: Qi Zhang (venkyqz)

<details>
## Summary

A cross-layer semantic gap exists in the ARM Thumb-2 assembly pipeline for post-indexed `ldrd` and `strd` instructions. The frontend `AsmParser` accepts unaligned offsets due to a missing `ParserMatchClass` in TableGen. This perfectly bypasses frontend validation, resulting in:
- **Debug Build**: Assertion failure (`Exit 134`)
- **Release Build**: Silent miscompilation (`Exit 0`, offset truncated to nearest 4-byte boundary)

This bug is triggerable entirely via plain text assembly.

---

## Affected Instructions

| Instruction | Opcode | Description |
|-------------|--------|-------------|
| `ldrd Rt, Rt2, [Rn], #imm` | `t2LDRD_POST` | Post-indexed load double |
| `strd Rt, Rt2, [Rn], #imm` | `t2STRD_POST` | Post-indexed store double |

---

## Reproduction
**Godbolt Link**
+ https://godbolt.org/z/oefoPMeYK

**Test File (`poc.s`):**

```asm
.syntax unified
.thumb
.text
ldrd r0, r1, [r2], #<!-- -->1
```

Any offset not strictly divisible by 4 (e.g., 1, 2, 3, 5, -1) will trigger the divergence.

**Commands:**

```bash
# Debug Build - Crashes with assertion
llvm-mc --arch=arm --triple=armv7-linux-gnueabihf --mcpu=cortex-a9 --filetype=asm --show-encoding poc.s
# Exit 134, Assertion failed

# Release Build - Silent miscompilation
llvm-mc --arch=arm --triple=armv7-linux-gnueabihf --mcpu=cortex-a9 --filetype=asm --show-encoding poc.s
# Exit 0, emits corrupted output: encoding: [0xf2,0xe8, 0x00,0x01] (#<!-- -->1 treated as #<!-- -->0)
```

## Debug Build Output

```
llvm-mc: /root/llvm-project/llvm/lib/Target/ARM/MCTargetDesc/ARMInstPrinter.cpp:1348: void llvm::ARMInstPrinter::printT2AddrModeImm8s4OffsetOperand(const llvm::MCInst*, unsigned int, const llvm::MCSubtargetInfo&amp;, llvm::raw_ostream&amp;): Assertion `((OffImm &amp; 0x3) == 0) &amp;&amp; "Not a valid immediate!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/llvm-mc -filetype=obj -o example.o --arch=arm --triple=armv7-linux-gnueabihf --mcpu=cortex-a9 --filetype=asm --show-encoding &lt;source&gt;
 #<!-- -->0 0x0000000000d41d58 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/llvm-mc+0xd41d58)
 #<!-- -->1 0x0000000000d3ee24 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->2 0x000071f7b8642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->3 0x000071f7b86969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->4 0x000071f7b8642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->5 0x000071f7b86287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->6 0x000071f7b862871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->7 0x000071f7b8639e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->8 0x00000000007d6b58 llvm::ARMInstPrinter::printT2AddrModeImm8s4OffsetOperand(llvm::MCInst const*, unsigned int, llvm::MCSubtargetInfo const&amp;, llvm::raw_ostream&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llvm-mc+0x7d6b58)
 #<!-- -->9 0x00000000007e1b44 llvm::ARMInstPrinter::printInst(llvm::MCInst const*, unsigned long, llvm::StringRef, llvm::MCSubtargetInfo const&amp;, llvm::raw_ostream&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llvm-mc+0x7e1b44)
#<!-- -->10 0x0000000000c0bc32 llvm::MCTargetStreamer::prettyPrintAsm(llvm::MCInstPrinter&amp;, unsigned long, llvm::MCInst const&amp;, llvm::MCSubtargetInfo const&amp;, llvm::raw_ostream&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llvm-mc+0xc0bc32)
#<!-- -->11 0x0000000000bbd43b (anonymous namespace)::MCAsmStreamer::emitInstruction(llvm::MCInst const&amp;, llvm::MCSubtargetInfo const&amp;) MCAsmStreamer.cpp:0:0
#<!-- -->12 0x0000000000570401 (anonymous namespace)::ARMAsmParser::matchAndEmitInstruction(llvm::SMLoc, unsigned int&amp;, llvm::SmallVectorImpl&lt;std::unique_ptr&lt;llvm::MCParsedAsmOperand, std::default_delete&lt;llvm::MCParsedAsmOperand&gt;&gt;&gt;&amp;, llvm::MCStreamer&amp;, unsigned long&amp;, bool) ARMAsmParser.cpp:0:0
#<!-- -->13 0x0000000000c85857 (anonymous namespace)::AsmParser::parseAndMatchAndEmitTargetInstruction((anonymous namespace)::ParseStatementInfo&amp;, llvm::StringRef, llvm::AsmToken, llvm::SMLoc) AsmParser.cpp:0:0
#<!-- -->14 0x0000000000c99d73 (anonymous namespace)::AsmParser::parseStatement((anonymous namespace)::ParseStatementInfo&amp;, llvm::MCAsmParserSemaCallback*) (.part.0) AsmParser.cpp:0:0
#<!-- -->15 0x0000000000c9dfc3 (anonymous namespace)::AsmParser::Run(bool, bool) AsmParser.cpp:0:0
#<!-- -->16 0x000000000040c6ba main (/opt/compiler-explorer/clang-assertions-trunk/bin/llvm-mc+0x40c6ba)
#<!-- -->17 0x000071f7b8629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->18 0x000071f7b8629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->19 0x000000000047bbe5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/llvm-mc+0x47bbe5)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

---

## Debug vs Release Behavior

| Offset | Debug Exit | Release Exit | Release Encoding        | Expected           |
| ------ | ---------- | ------------ | ----------------------- | ------------------ |
| `#<!-- -->0`   | 0          | 0            | `[0xf2,0xe8,0x00,0x01]` | Accept             |
| `#<!-- -->1`   | **134**    | **0**        | `[0xf2,0xe8,0x00,0x01]` | **Reject** |
| `#<!-- -->2`   | **134**    | **0**        | `[0xf2,0xe8,0x00,0x01]` | **Reject** |
| `#<!-- -->3`   | **134**    | **0**        | `[0xf2,0xe8,0x00,0x01]` | **Reject** |
| `#<!-- -->4`   | 0          | 0            | `[0xf2,0xe8,0x01,0x01]` | Accept             |
| `#<!-- -->5`   | **134**    | **0**        | `[0xf2,0xe8,0x01,0x01]` | **Reject** |
| `#-1`  | **134**    | **0**        | `[0x72,0xe8,0x00,0x01]` | **Reject** |
| `#-4`  | 0          | 0            | `[0x72,0xe8,0x01,0x01]` | Accept             |

---

## Root Cause

The vulnerability stems from a declarative omission in TableGen. The operand `t2am_imm8s4_offset` (used for the post-indexed offset of `t2LDRD_POST` and `t2STRD_POST`) lacks a `ParserMatchClass`.

Unlike the pre-indexed variant (`t2addrmode_imm8s4`), which correctly uses `MemImm8s4OffsetAsmOperand`, the post-indexed variant falls back to accepting any immediate without 4-byte alignment validation.

**Buggy Code (`llvm/lib/Target/ARM/ARMInstrThumb2.td`):**

```
def t2am_imm8s4_offset_asmoperand : AsmOperandClass { let Name = "Imm8s4"; }

def t2am_imm8s4_offset : MemOperand {  // VULNERABILITY: Missing ParserMatchClass!
  let PrintMethod = "printT2AddrModeImm8s4OffsetOperand";
  let EncoderMethod = "getT2ScaledImmOpValue&lt;8,2&gt;";
  let DecoderMethod = "DecodeT2Imm8S4";
}
```

Because the `t2am_imm8s4_offset_asmoperand` class is defined but never assigned, the `AsmMatcher` accepts unaligned immediate.
</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-llvm-mc

Author: Qi Zhang (venkyqz)

<details>
## Summary

A cross-layer semantic gap exists in the ARM Thumb-2 assembly pipeline for post-indexed `ldrd` and `strd` instructions. The frontend `AsmParser` accepts unaligned offsets due to a missing `ParserMatchClass` in TableGen. This perfectly bypasses frontend validation, resulting in:
- **Debug Build**: Assertion failure (`Exit 134`)
- **Release Build**: Silent miscompilation (`Exit 0`, offset truncated to nearest 4-byte boundary)

This bug is triggerable entirely via plain text assembly.

---

## Affected Instructions

| Instruction | Opcode | Description |
|-------------|--------|-------------|
| `ldrd Rt, Rt2, [Rn], #imm` | `t2LDRD_POST` | Post-indexed load double |
| `strd Rt, Rt2, [Rn], #imm` | `t2STRD_POST` | Post-indexed store double |

---

## Reproduction
**Godbolt Link**
+ https://godbolt.org/z/oefoPMeYK

**Test File (`poc.s`):**

```asm
.syntax unified
.thumb
.text
ldrd r0, r1, [r2], #<!-- -->1
```

Any offset not strictly divisible by 4 (e.g., 1, 2, 3, 5, -1) will trigger the divergence.

**Commands:**

```bash
# Debug Build - Crashes with assertion
llvm-mc --arch=arm --triple=armv7-linux-gnueabihf --mcpu=cortex-a9 --filetype=asm --show-encoding poc.s
# Exit 134, Assertion failed

# Release Build - Silent miscompilation
llvm-mc --arch=arm --triple=armv7-linux-gnueabihf --mcpu=cortex-a9 --filetype=asm --show-encoding poc.s
# Exit 0, emits corrupted output: encoding: [0xf2,0xe8, 0x00,0x01] (#<!-- -->1 treated as #<!-- -->0)
```

## Debug Build Output

```
llvm-mc: /root/llvm-project/llvm/lib/Target/ARM/MCTargetDesc/ARMInstPrinter.cpp:1348: void llvm::ARMInstPrinter::printT2AddrModeImm8s4OffsetOperand(const llvm::MCInst*, unsigned int, const llvm::MCSubtargetInfo&amp;, llvm::raw_ostream&amp;): Assertion `((OffImm &amp; 0x3) == 0) &amp;&amp; "Not a valid immediate!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/llvm-mc -filetype=obj -o example.o --arch=arm --triple=armv7-linux-gnueabihf --mcpu=cortex-a9 --filetype=asm --show-encoding &lt;source&gt;
 #<!-- -->0 0x0000000000d41d58 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/llvm-mc+0xd41d58)
 #<!-- -->1 0x0000000000d3ee24 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->2 0x000071f7b8642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->3 0x000071f7b86969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->4 0x000071f7b8642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->5 0x000071f7b86287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->6 0x000071f7b862871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->7 0x000071f7b8639e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->8 0x00000000007d6b58 llvm::ARMInstPrinter::printT2AddrModeImm8s4OffsetOperand(llvm::MCInst const*, unsigned int, llvm::MCSubtargetInfo const&amp;, llvm::raw_ostream&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llvm-mc+0x7d6b58)
 #<!-- -->9 0x00000000007e1b44 llvm::ARMInstPrinter::printInst(llvm::MCInst const*, unsigned long, llvm::StringRef, llvm::MCSubtargetInfo const&amp;, llvm::raw_ostream&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llvm-mc+0x7e1b44)
#<!-- -->10 0x0000000000c0bc32 llvm::MCTargetStreamer::prettyPrintAsm(llvm::MCInstPrinter&amp;, unsigned long, llvm::MCInst const&amp;, llvm::MCSubtargetInfo const&amp;, llvm::raw_ostream&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llvm-mc+0xc0bc32)
#<!-- -->11 0x0000000000bbd43b (anonymous namespace)::MCAsmStreamer::emitInstruction(llvm::MCInst const&amp;, llvm::MCSubtargetInfo const&amp;) MCAsmStreamer.cpp:0:0
#<!-- -->12 0x0000000000570401 (anonymous namespace)::ARMAsmParser::matchAndEmitInstruction(llvm::SMLoc, unsigned int&amp;, llvm::SmallVectorImpl&lt;std::unique_ptr&lt;llvm::MCParsedAsmOperand, std::default_delete&lt;llvm::MCParsedAsmOperand&gt;&gt;&gt;&amp;, llvm::MCStreamer&amp;, unsigned long&amp;, bool) ARMAsmParser.cpp:0:0
#<!-- -->13 0x0000000000c85857 (anonymous namespace)::AsmParser::parseAndMatchAndEmitTargetInstruction((anonymous namespace)::ParseStatementInfo&amp;, llvm::StringRef, llvm::AsmToken, llvm::SMLoc) AsmParser.cpp:0:0
#<!-- -->14 0x0000000000c99d73 (anonymous namespace)::AsmParser::parseStatement((anonymous namespace)::ParseStatementInfo&amp;, llvm::MCAsmParserSemaCallback*) (.part.0) AsmParser.cpp:0:0
#<!-- -->15 0x0000000000c9dfc3 (anonymous namespace)::AsmParser::Run(bool, bool) AsmParser.cpp:0:0
#<!-- -->16 0x000000000040c6ba main (/opt/compiler-explorer/clang-assertions-trunk/bin/llvm-mc+0x40c6ba)
#<!-- -->17 0x000071f7b8629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->18 0x000071f7b8629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->19 0x000000000047bbe5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/llvm-mc+0x47bbe5)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

---

## Debug vs Release Behavior

| Offset | Debug Exit | Release Exit | Release Encoding        | Expected           |
| ------ | ---------- | ------------ | ----------------------- | ------------------ |
| `#<!-- -->0`   | 0          | 0            | `[0xf2,0xe8,0x00,0x01]` | Accept             |
| `#<!-- -->1`   | **134**    | **0**        | `[0xf2,0xe8,0x00,0x01]` | **Reject** |
| `#<!-- -->2`   | **134**    | **0**        | `[0xf2,0xe8,0x00,0x01]` | **Reject** |
| `#<!-- -->3`   | **134**    | **0**        | `[0xf2,0xe8,0x00,0x01]` | **Reject** |
| `#<!-- -->4`   | 0          | 0            | `[0xf2,0xe8,0x01,0x01]` | Accept             |
| `#<!-- -->5`   | **134**    | **0**        | `[0xf2,0xe8,0x01,0x01]` | **Reject** |
| `#-1`  | **134**    | **0**        | `[0x72,0xe8,0x00,0x01]` | **Reject** |
| `#-4`  | 0          | 0            | `[0x72,0xe8,0x01,0x01]` | Accept             |

---

## Root Cause

The vulnerability stems from a declarative omission in TableGen. The operand `t2am_imm8s4_offset` (used for the post-indexed offset of `t2LDRD_POST` and `t2STRD_POST`) lacks a `ParserMatchClass`.

Unlike the pre-indexed variant (`t2addrmode_imm8s4`), which correctly uses `MemImm8s4OffsetAsmOperand`, the post-indexed variant falls back to accepting any immediate without 4-byte alignment validation.

**Buggy Code (`llvm/lib/Target/ARM/ARMInstrThumb2.td`):**

```
def t2am_imm8s4_offset_asmoperand : AsmOperandClass { let Name = "Imm8s4"; }

def t2am_imm8s4_offset : MemOperand {  // VULNERABILITY: Missing ParserMatchClass!
  let PrintMethod = "printT2AddrModeImm8s4OffsetOperand";
  let EncoderMethod = "getT2ScaledImmOpValue&lt;8,2&gt;";
  let DecoderMethod = "DecodeT2Imm8S4";
}
```

Because the `t2am_imm8s4_offset_asmoperand` class is defined but never assigned, the `AsmMatcher` accepts unaligned immediate.
</details>


---

