# [MC][MIPS] Clang crashes when assembling MIPS64r6 LDPC with non-8-byte-aligned offset

**Author:** venkyqz
**URL:** https://github.com/llvm/llvm-project/issues/184959
**Status:** Open
**Labels:** backend:MIPS, llvm:mc, crash

## Body

## Summary

There exists a semantic gap in the MIPS64r6 assembly pipeline. The frontend `AsmParser` accepts unaligned PC-relative offsets for the `ldpc` instruction due to a missing specification in `TableGen`. This perfectly bypasses frontend validation, resulting in:

- **Debug Build**: Assertion failure (`Exit 134`)
- **Release Build**: Silent miscompilation (`Exit 0`, emitting incorrect bytes)

This bug is triggerable entirely via assembling (Clang or llvm-mc)

---

## Reproduction
**Godbolt Link**
+ https://godbolt.org/z/bbK6af1hc

**Test File (`poc.s`):**

```asm
.set mips64r6
.text
ldpc $at, 3
```

Any offset not strictly divisible by 8 (e.g., 3, 7, -1) will trigger the divergence.

**Commands:**

```bash
# Debug Build - Crashes with assertion
llvm-mc -filetype=obj -triple=mips64 -mattr=+mips64r6 poc.s
# Exit 134, Assertion failed

# Release Build - Silent miscompilation
llvm-mc -filetype=obj -triple=mips64 -mattr=+mips64r6 poc.s
# Exit 0, emits corrupted object code
```

**Debug Build Output:**

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/llvm-mc -filetype=obj -o example.o --triple=mips64el-linux-gnu <source>
 #0 0x0000000000d41d58 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/llvm-mc+0xd41d58)
 #1 0x0000000000d3ee24 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x000074492b642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x000074492b6969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x000074492b642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x000074492b6287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x000074492b62871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #7 0x000074492b639e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #8 0x00000000008a2f4c (/opt/compiler-explorer/clang-assertions-trunk/bin/llvm-mc+0x8a2f4c)
 #9 0x00000000008a9584 llvm::MipsMCCodeEmitter::getBinaryCodeForInstr(llvm::MCInst const&, llvm::SmallVectorImpl<llvm::MCFixup>&, llvm::MCSubtargetInfo const&) const (/opt/compiler-explorer/clang-assertions-trunk/bin/llvm-mc+0x8a9584)
#10 0x00000000008ab628 llvm::MipsMCCodeEmitter::encodeInstruction(llvm::MCInst const&, llvm::SmallVectorImpl<char>&, llvm::SmallVectorImpl<llvm::MCFixup>&, llvm::MCSubtargetInfo const&) const (/opt/compiler-explorer/clang-assertions-trunk/bin/llvm-mc+0x8ab628)
#11 0x0000000000c00b6e llvm::MCObjectStreamer::emitInstToData(llvm::MCInst const&, llvm::MCSubtargetInfo const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llvm-mc+0xc00b6e)
#12 0x0000000000c034de llvm::MCObjectStreamer::emitInstruction(llvm::MCInst const&, llvm::MCSubtargetInfo const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llvm-mc+0xc034de)
#13 0x000000000089a835 llvm::MipsELFStreamer::emitInstruction(llvm::MCInst const&, llvm::MCSubtargetInfo const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llvm-mc+0x89a835)
#14 0x00000000005cbd25 (anonymous namespace)::MipsAsmParser::processInstruction(llvm::MCInst&, llvm::SMLoc, llvm::MCStreamer&, llvm::MCSubtargetInfo const*) MipsAsmParser.cpp:0:0
#15 0x00000000005cdd84 (anonymous namespace)::MipsAsmParser::matchAndEmitInstruction(llvm::SMLoc, unsigned int&, llvm::SmallVectorImpl<std::unique_ptr<llvm::MCParsedAsmOperand, std::default_delete<llvm::MCParsedAsmOperand>>>&, llvm::MCStreamer&, unsigned long&, bool) MipsAsmParser.cpp:0:0
#16 0x0000000000c85857 (anonymous namespace)::AsmParser::parseAndMatchAndEmitTargetInstruction((anonymous namespace)::ParseStatementInfo&, llvm::StringRef, llvm::AsmToken, llvm::SMLoc) AsmParser.cpp:0:0
#17 0x0000000000c99d73 (anonymous namespace)::AsmParser::parseStatement((anonymous namespace)::ParseStatementInfo&, llvm::MCAsmParserSemaCallback*) (.part.0) AsmParser.cpp:0:0
#18 0x0000000000c9dfc3 (anonymous namespace)::AsmParser::Run(bool, bool) AsmParser.cpp:0:0
#19 0x000000000040c6ba main (/opt/compiler-explorer/clang-assertions-trunk/bin/llvm-mc+0x40c6ba)
#20 0x000074492b629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#21 0x000074492b629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#22 0x000000000047bbe5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/llvm-mc+0x47bbe5)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```
---

## Root Cause

The vulnerability stems from a declarative omission in TableGen. The `ldpc` instruction uses `simm18_lsl3` operand (requires 8-byte alignment), but is mapped to an overly permissive parser class.

**Buggy Code (`MipsInstrInfo.td`):**

```cpp
def simm18_lsl3 : Operand<i64> {
  let ParserMatchClass = MipsJumpTargetAsmOperand; // VULNERABILITY: Only calls isImm()
}
```

**Comparison**: Sibling instructions like `addiupc` and `lwpc` (using `simm19_lsl2`) are mapped to `Simm19Lsl2AsmOperand`, which enforces 4-byte alignment via `isS19ImmX4()` predicate. The `simm18_lsl3` operand is missing an analogous `Simm18Lsl3AsmOperand` class with `isS18ImmX8()` validation.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-mips

Author: Qi Zhang (venkyqz)

<details>
## Summary

There exists a semantic gap in the MIPS64r6 assembly pipeline. The frontend `AsmParser` accepts unaligned PC-relative offsets for the `ldpc` instruction due to a missing specification in `TableGen`. This perfectly bypasses frontend validation, resulting in:

- **Debug Build**: Assertion failure (`Exit 134`)
- **Release Build**: Silent miscompilation (`Exit 0`, emitting incorrect bytes)

This bug is triggerable entirely via assembling (Clang or llvm-mc)

---

## Reproduction
**Godbolt Link**
+ https://godbolt.org/z/bbK6af1hc

**Test File (`poc.s`):**

```asm
.set mips64r6
.text
ldpc $at, 3
```

Any offset not strictly divisible by 8 (e.g., 3, 7, -1) will trigger the divergence.

**Commands:**

```bash
# Debug Build - Crashes with assertion
llvm-mc -filetype=obj -triple=mips64 -mattr=+mips64r6 poc.s
# Exit 134, Assertion failed

# Release Build - Silent miscompilation
llvm-mc -filetype=obj -triple=mips64 -mattr=+mips64r6 poc.s
# Exit 0, emits corrupted object code
```

**Debug Build Output:**

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/llvm-mc -filetype=obj -o example.o --triple=mips64el-linux-gnu &lt;source&gt;
 #<!-- -->0 0x0000000000d41d58 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/llvm-mc+0xd41d58)
 #<!-- -->1 0x0000000000d3ee24 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->2 0x000074492b642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->3 0x000074492b6969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->4 0x000074492b642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->5 0x000074492b6287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->6 0x000074492b62871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->7 0x000074492b639e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->8 0x00000000008a2f4c (/opt/compiler-explorer/clang-assertions-trunk/bin/llvm-mc+0x8a2f4c)
 #<!-- -->9 0x00000000008a9584 llvm::MipsMCCodeEmitter::getBinaryCodeForInstr(llvm::MCInst const&amp;, llvm::SmallVectorImpl&lt;llvm::MCFixup&gt;&amp;, llvm::MCSubtargetInfo const&amp;) const (/opt/compiler-explorer/clang-assertions-trunk/bin/llvm-mc+0x8a9584)
#<!-- -->10 0x00000000008ab628 llvm::MipsMCCodeEmitter::encodeInstruction(llvm::MCInst const&amp;, llvm::SmallVectorImpl&lt;char&gt;&amp;, llvm::SmallVectorImpl&lt;llvm::MCFixup&gt;&amp;, llvm::MCSubtargetInfo const&amp;) const (/opt/compiler-explorer/clang-assertions-trunk/bin/llvm-mc+0x8ab628)
#<!-- -->11 0x0000000000c00b6e llvm::MCObjectStreamer::emitInstToData(llvm::MCInst const&amp;, llvm::MCSubtargetInfo const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llvm-mc+0xc00b6e)
#<!-- -->12 0x0000000000c034de llvm::MCObjectStreamer::emitInstruction(llvm::MCInst const&amp;, llvm::MCSubtargetInfo const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llvm-mc+0xc034de)
#<!-- -->13 0x000000000089a835 llvm::MipsELFStreamer::emitInstruction(llvm::MCInst const&amp;, llvm::MCSubtargetInfo const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llvm-mc+0x89a835)
#<!-- -->14 0x00000000005cbd25 (anonymous namespace)::MipsAsmParser::processInstruction(llvm::MCInst&amp;, llvm::SMLoc, llvm::MCStreamer&amp;, llvm::MCSubtargetInfo const*) MipsAsmParser.cpp:0:0
#<!-- -->15 0x00000000005cdd84 (anonymous namespace)::MipsAsmParser::matchAndEmitInstruction(llvm::SMLoc, unsigned int&amp;, llvm::SmallVectorImpl&lt;std::unique_ptr&lt;llvm::MCParsedAsmOperand, std::default_delete&lt;llvm::MCParsedAsmOperand&gt;&gt;&gt;&amp;, llvm::MCStreamer&amp;, unsigned long&amp;, bool) MipsAsmParser.cpp:0:0
#<!-- -->16 0x0000000000c85857 (anonymous namespace)::AsmParser::parseAndMatchAndEmitTargetInstruction((anonymous namespace)::ParseStatementInfo&amp;, llvm::StringRef, llvm::AsmToken, llvm::SMLoc) AsmParser.cpp:0:0
#<!-- -->17 0x0000000000c99d73 (anonymous namespace)::AsmParser::parseStatement((anonymous namespace)::ParseStatementInfo&amp;, llvm::MCAsmParserSemaCallback*) (.part.0) AsmParser.cpp:0:0
#<!-- -->18 0x0000000000c9dfc3 (anonymous namespace)::AsmParser::Run(bool, bool) AsmParser.cpp:0:0
#<!-- -->19 0x000000000040c6ba main (/opt/compiler-explorer/clang-assertions-trunk/bin/llvm-mc+0x40c6ba)
#<!-- -->20 0x000074492b629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->21 0x000074492b629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->22 0x000000000047bbe5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/llvm-mc+0x47bbe5)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```
---

## Root Cause

The vulnerability stems from a declarative omission in TableGen. The `ldpc` instruction uses `simm18_lsl3` operand (requires 8-byte alignment), but is mapped to an overly permissive parser class.

**Buggy Code (`MipsInstrInfo.td`):**

```cpp
def simm18_lsl3 : Operand&lt;i64&gt; {
  let ParserMatchClass = MipsJumpTargetAsmOperand; // VULNERABILITY: Only calls isImm()
}
```

**Comparison**: Sibling instructions like `addiupc` and `lwpc` (using `simm19_lsl2`) are mapped to `Simm19Lsl2AsmOperand`, which enforces 4-byte alignment via `isS19ImmX4()` predicate. The `simm18_lsl3` operand is missing an analogous `Simm18Lsl3AsmOperand` class with `isS18ImmX8()` validation.
</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-llvm-mc

Author: Qi Zhang (venkyqz)

<details>
## Summary

There exists a semantic gap in the MIPS64r6 assembly pipeline. The frontend `AsmParser` accepts unaligned PC-relative offsets for the `ldpc` instruction due to a missing specification in `TableGen`. This perfectly bypasses frontend validation, resulting in:

- **Debug Build**: Assertion failure (`Exit 134`)
- **Release Build**: Silent miscompilation (`Exit 0`, emitting incorrect bytes)

This bug is triggerable entirely via assembling (Clang or llvm-mc)

---

## Reproduction
**Godbolt Link**
+ https://godbolt.org/z/bbK6af1hc

**Test File (`poc.s`):**

```asm
.set mips64r6
.text
ldpc $at, 3
```

Any offset not strictly divisible by 8 (e.g., 3, 7, -1) will trigger the divergence.

**Commands:**

```bash
# Debug Build - Crashes with assertion
llvm-mc -filetype=obj -triple=mips64 -mattr=+mips64r6 poc.s
# Exit 134, Assertion failed

# Release Build - Silent miscompilation
llvm-mc -filetype=obj -triple=mips64 -mattr=+mips64r6 poc.s
# Exit 0, emits corrupted object code
```

**Debug Build Output:**

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/llvm-mc -filetype=obj -o example.o --triple=mips64el-linux-gnu &lt;source&gt;
 #<!-- -->0 0x0000000000d41d58 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/llvm-mc+0xd41d58)
 #<!-- -->1 0x0000000000d3ee24 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->2 0x000074492b642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->3 0x000074492b6969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->4 0x000074492b642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->5 0x000074492b6287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->6 0x000074492b62871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->7 0x000074492b639e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->8 0x00000000008a2f4c (/opt/compiler-explorer/clang-assertions-trunk/bin/llvm-mc+0x8a2f4c)
 #<!-- -->9 0x00000000008a9584 llvm::MipsMCCodeEmitter::getBinaryCodeForInstr(llvm::MCInst const&amp;, llvm::SmallVectorImpl&lt;llvm::MCFixup&gt;&amp;, llvm::MCSubtargetInfo const&amp;) const (/opt/compiler-explorer/clang-assertions-trunk/bin/llvm-mc+0x8a9584)
#<!-- -->10 0x00000000008ab628 llvm::MipsMCCodeEmitter::encodeInstruction(llvm::MCInst const&amp;, llvm::SmallVectorImpl&lt;char&gt;&amp;, llvm::SmallVectorImpl&lt;llvm::MCFixup&gt;&amp;, llvm::MCSubtargetInfo const&amp;) const (/opt/compiler-explorer/clang-assertions-trunk/bin/llvm-mc+0x8ab628)
#<!-- -->11 0x0000000000c00b6e llvm::MCObjectStreamer::emitInstToData(llvm::MCInst const&amp;, llvm::MCSubtargetInfo const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llvm-mc+0xc00b6e)
#<!-- -->12 0x0000000000c034de llvm::MCObjectStreamer::emitInstruction(llvm::MCInst const&amp;, llvm::MCSubtargetInfo const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llvm-mc+0xc034de)
#<!-- -->13 0x000000000089a835 llvm::MipsELFStreamer::emitInstruction(llvm::MCInst const&amp;, llvm::MCSubtargetInfo const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llvm-mc+0x89a835)
#<!-- -->14 0x00000000005cbd25 (anonymous namespace)::MipsAsmParser::processInstruction(llvm::MCInst&amp;, llvm::SMLoc, llvm::MCStreamer&amp;, llvm::MCSubtargetInfo const*) MipsAsmParser.cpp:0:0
#<!-- -->15 0x00000000005cdd84 (anonymous namespace)::MipsAsmParser::matchAndEmitInstruction(llvm::SMLoc, unsigned int&amp;, llvm::SmallVectorImpl&lt;std::unique_ptr&lt;llvm::MCParsedAsmOperand, std::default_delete&lt;llvm::MCParsedAsmOperand&gt;&gt;&gt;&amp;, llvm::MCStreamer&amp;, unsigned long&amp;, bool) MipsAsmParser.cpp:0:0
#<!-- -->16 0x0000000000c85857 (anonymous namespace)::AsmParser::parseAndMatchAndEmitTargetInstruction((anonymous namespace)::ParseStatementInfo&amp;, llvm::StringRef, llvm::AsmToken, llvm::SMLoc) AsmParser.cpp:0:0
#<!-- -->17 0x0000000000c99d73 (anonymous namespace)::AsmParser::parseStatement((anonymous namespace)::ParseStatementInfo&amp;, llvm::MCAsmParserSemaCallback*) (.part.0) AsmParser.cpp:0:0
#<!-- -->18 0x0000000000c9dfc3 (anonymous namespace)::AsmParser::Run(bool, bool) AsmParser.cpp:0:0
#<!-- -->19 0x000000000040c6ba main (/opt/compiler-explorer/clang-assertions-trunk/bin/llvm-mc+0x40c6ba)
#<!-- -->20 0x000074492b629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->21 0x000074492b629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->22 0x000000000047bbe5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/llvm-mc+0x47bbe5)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```
---

## Root Cause

The vulnerability stems from a declarative omission in TableGen. The `ldpc` instruction uses `simm18_lsl3` operand (requires 8-byte alignment), but is mapped to an overly permissive parser class.

**Buggy Code (`MipsInstrInfo.td`):**

```cpp
def simm18_lsl3 : Operand&lt;i64&gt; {
  let ParserMatchClass = MipsJumpTargetAsmOperand; // VULNERABILITY: Only calls isImm()
}
```

**Comparison**: Sibling instructions like `addiupc` and `lwpc` (using `simm19_lsl2`) are mapped to `Simm19Lsl2AsmOperand`, which enforces 4-byte alignment via `isS19ImmX4()` predicate. The `simm18_lsl3` operand is missing an analogous `Simm18Lsl3AsmOperand` class with `isS18ImmX8()` validation.
</details>


---

