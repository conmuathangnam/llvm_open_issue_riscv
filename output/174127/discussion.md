# clang assertion failure with implicit immediates for AMX tile dimensions

**Author:** thurstond
**URL:** https://github.com/llvm/llvm-project/issues/174127
**Status:** Closed
**Labels:** llvm:codegen, crash
**Closed Date:** 2026-01-03T20:56:38Z

## Body

*This issue is for documentation purposes, to avoid dumping it in the commit message.*

## Reproducer 

`clang -O1` fails at trunk with an assertion failure:

https://godbolt.org/z/xd77jjeb6

```
void *a;
short b;
int c, d, e, f;
struct g {
  g(int, long, int *);
  virtual void h();
};
template <typename i> struct j : g {
  j(int k, int, int *l, i) : g(k, 0, l) {}
  __attribute((target("amx-tile"))) void h() {
    __builtin_ia32_tileloadd64_internal(b, 0, a, c);
    __builtin_ia32_tileloadd64_internal(b, 0, a, c);
  }
};
void m() { j(d, f, &e, 0); }
```

```
error: _ZN1jIiE1hEv: Failed to config tile register, please define the shape earlier
clang++: /root/llvm-project/llvm/include/llvm/CodeGen/MachineOperand.h:561: int64_t llvm::MachineOperand::getImm() const: Assertion `isImm() && "Wrong MachineOperand accessor"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -fno-verbose-asm -c --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -O1 <source>
1.	<eof> parser at end of file
2.	Code generation
3.	Running pass 'Function Pass Manager' on module '<source>'.
4.	Running pass 'Greedy Register Allocator' on function '@_ZN1jIiE1hEv'
 #0 0x0000000004302c88 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4302c88)
 #1 0x00000000043000b4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x43000b4)
 #2 0x0000000004244ab8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007e7f76242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007e7f762969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007e7f76242476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007e7f762287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007e7f7622871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007e7f76239e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000002ab8d0b getTileShape(llvm::Register, llvm::VirtRegMap*, llvm::MachineRegisterInfo const*) X86RegisterInfo.cpp:0:0
...
```

(ignore the "error: _ZN1jIiE1hEv: Failed to config tile register, please define the shape earlier"; that is unrelated)

## Cause

This is because `__builtin_ia32_tileloadd64_internal(b, /* columns */ 0, a, c);` has the columns field converted to `MOV32r0`, which does not allow `getImm()`.

```
# Machine code for function _ZN1jIiE1hEv: NoPHIs, TracksLiveness, TiedOpsRewritten, TracksDebugUserValues

0B      bb.0.entry:
16B       %1:gr16 = MOV16rm $rip, 1, $noreg, @b, $noreg :: (dereferenceable load (s16) from @b, !tbaa !11)
32B       %2:gr64 = MOV64rm $rip, 1, $noreg, @a, $noreg :: (dereferenceable load (s64) from @a, !tbaa !13)
48B       %3:gr64_nosp = MOVSX64rm32 $rip, 1, $noreg, @c, $noreg :: (dereferenceable load (s32) from @c, !tbaa !5)
64B       %4:gr32 = MOV32r0 implicit-def dead $eflags
96B       dead %6:tile = PTILELOADDV %1:gr16, %4.sub_16bit:gr32, %2:gr64, 1, %3:gr64_nosp, 0, $noreg
...
```

This is a regression by https://github.com/llvm/llvm-project/pull/165556, which removed support for implicit operands:
```
// llvm/include/llvm/CodeGen/TileShapeInfo.h
        if (MI->isMoveImmediate()) {
-          if (MI->getOperand(1).isImm()) {
-            Imm = MI->getOperand(1).getImm();
-          } else {
-            assert(MI->getOperand(1).isImplicit() &&
-                   "Operand 1 is assumed to be implicit.");
-            Imm = 0;
-          }

+          Imm = MI->getOperand(1).getImm();
```

(though the implicit operand is not guaranteed to be zero - e.g., MOV32r1, MOV32r_1).

Although the implicit operands of 0/1/-1 are not valid values for the row/column sizes of tiles, it would be preferable to not have an assertion failure, especially since reduction algorithms will often replace variables with zero.


