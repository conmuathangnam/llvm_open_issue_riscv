# RegScavenger Crash in replaceFrameIndicesBackward Due to Invalid MBBI After Frame Instruction Elimination

**Author:** Sightem
**URL:** https://github.com/llvm/llvm-project/issues/174251
**Status:** Open
**Labels:** llvm:codegen, crash

## Body

There is a crash in `PrologEpilogInserter::replaceFrameIndicesBackward` when a basic block's last instruction is a call frame pseudo instruction. The `RegScavenger::MBBI` iterator becomes invalid (dangling) after `eliminateCallFramePseudoInstr` erases the instruction, causing a segmentation fault on the next `backward()` call.

## Environment

- **LLVM Version**: 17.0.6 ([CE-Programming/llvm-project](https://github.com/CE-Programming/llvm-project) fork with Z80 experimental target)
- **Target**: Z80/eZ80
- **OS**: Linux x86_64
- **Bug Location**: Target-independent code in `PrologEpilogInserter.cpp`


Bug appears to be present on current trunk. See comment

## Minimal reproducer
```c
void callee(const char *);

__attribute__((noreturn)) void crash_repro(void) {
    callee("message");
    __builtin_unreachable();
}
```

Compile with:
```bash
clang --target=ez80-none-elf -Oz -S repro.c
```


Crash log is attached at the end of the issue.


## Our Attempt at Root Cause Analysis:

In `replaceFrameIndicesBackward` (in `PrologEpilogInserter.cpp`) , the original code flow was:

```cpp
for (MachineInstr &MI : make_early_inc_range(reverse(*BB))) {
  if (TII.isFrameInstr(MI)) {
    TFI.eliminateCallFramePseudoInstr(MF, *BB, &MI);  // erases MI
    continue;
  }

  // Step backwards to get the liveness state at (immediately after) MI.
  if (LocalRS)
    LocalRS->backward(MI);  // called AFTER frame instruction check
  
  // ... rest of frame index elimination ...
}
```

The issue appears to be that `enterBasicBlockEnd` initializes MBBI to the last instruction in the BB:
```cpp
void RegScavenger::enterBasicBlockEnd(MachineBasicBlock &MBB) {
  // ...
  if (!MBB.empty()) {
    MBBI = std::prev(MBB.end());  // points to last instruction
    Tracking = true;
  }
}
```


### Apparent Crash Scenario

From what we can tell , the following needs to happen for the crash to reproduce:

1. `enterBasicBlockEnd` sets `MBBI` to the last instruction in the BB
2. the loop's first iteration processes this last instruction
3. if it's a frame pseudo instruction (e.g., `ADJCALLSTACKDOWN`/`ADJCALLSTACKUP`), it gets erased by `eliminateCallFramePseudoInstr`
4. `LocalRS->backward(MI)` is NOT called for frame instructions (it was after the check)
5. `MBBI` now points to erased memory (Parent=NULL, Prev=NULL)
6. on the next iteration, `backward(MI)` is called, which internally does:
   ```cpp
   void backward(MachineBasicBlock::iterator I) {
     while (MBBI != I)
       backward();  // dereferences invalid MBBI
   }
   ```


The following conditions are required for the crash:
1. Target uses backward scavenging (`supportsBackwardScavenger() == true`)
2. Target generates call frame pseudos: `hasReservedCallFrame() == false`
3. A call frame pseudo is the last instruction in a BB


Z80 always generates `ADJCALLSTACKDOWN`/`ADJCALLSTACKUP` pseudo instructions around calls.

Mips on the other hand *usually* reserves the call frame (returns `true`) when the call frame fits in 16 bit offset and there are no variable sized objects, so it typically doesnt generate these pseudo instructions.

This bug appears to be latent in mainline LLVM and could affect Mips when compiling functions with variable-sized objects, or other backward scavenging targets if they do not reserve call frames, though we were unable to reproduce this.

## Fix 

Our fix involves two changes:

1. Move `backward(MI)` Before Frame Instruction Check
This makes it so backward is called for all instructions, including frame instructions, before any modifications. This ensures the scavenger's iterator is valid and not pointing to an instruction that might be erased.

2. Step Past Frame Instructions Before Erasing
`backward(MI)` only walks MBBI until `MBBI == MI`, it doesnt step past MI. For frame instructions that will be erased, we must step past MI first so that MBBI does not point to the instruction being erased.


```diff
--- a/llvm/lib/CodeGen/PrologEpilogInserter.cpp
+++ b/llvm/lib/CodeGen/PrologEpilogInserter.cpp
@@ -1470,14 +1470,17 @@ void PEI::replaceFrameIndicesBackward(MachineBasicBlock *BB,
   for (MachineInstr &MI : make_early_inc_range(reverse(*BB))) {
-    if (TII.isFrameInstr(MI)) {
-      TFI.eliminateCallFramePseudoInstr(MF, *BB, &MI);
-      continue;
-    }
-
     if (LocalRS)
       LocalRS->backward(MI);

+    if (TII.isFrameInstr(MI)) {
+      if (LocalRS)
+        LocalRS->backward();
+      TFI.eliminateCallFramePseudoInstr(MF, *BB, &MI);
+      continue;
+    }
+
     for (unsigned i = 0; i != MI.getNumOperands(); ++i) {
```


After this fix, the minimal reproducer compiles, as well as real world (e)z80 projects. 


## Questions

1. Is this fix correct? we are calling backward before erasing frame instructions. Is this the right approach, or should `eliminateCallFramePseudoInstr` handle scavenger state?

2. Should backward step past I also? the semantics of `backward(I)` is to walk MBBI until it equals I, but not step past I. Should there be a variant that also steps past the target instruction?

3. Currently, if MBBI points to an erased instruction (Parent=NULL), the crash happens deep in the ilist iteration. Should `backward()` check for invalid MBBI state?

4. Is this issue present in trunk/21? We have only been able to test on 17.0.6 as we are somewhat behind upstream. 


Crash log can be found here: https://pastebin.com/gbz8Y7kZ

## Comments

### Comment 1 - Sightem

Upon a closer inspection of the current main branch, the bug appears to still be present. Looking at

```cpp
for (MachineBasicBlock::iterator I = BB->end(); I != BB->begin();) {
    MachineInstr &MI = *std::prev(I);
    if (TII.isFrameInstr(MI)) {
      SPAdj -= TII.getSPAdjust(MI);
      TFI.eliminateCallFramePseudoInstr(MF, *BB, &MI);  // erases MI
      continue;
    }
    // Step backwards to get the liveness state at (immedately after) MI.
    if (LocalRS)
      LocalRS->backward(I);
    // ...
}
```

The same issue exists: `enterBasicBlockEnd` sets MBBI to the last instruction, and when a frame instruction is erased, `backward()` is never called to step the scavenger past it. On the next iteration, `backward(I)` will dereference the dangling MBBI.

---

### Comment 2 - Sightem

Follow up on this, after applying the fix we discovered another possible issue that causes incorrect codegen in certain scenarios. a workaround has been implemented in our backend, but wanted to document this. 

The fix described here (moving backward(MI) before the frame instruction check) changes the scavenger's liveness state at the point `eliminateFrameIndex` is called. This exposes a potential oversight where `RegScavenger::skipTo()` does not update `LiveUnits` after frame index elimination inserts new instructions.

When multiple frame indices are rewritten in a single basic block, the scavenger's liveness tracking becomes desynchronized. Instructions inserted during the first frame index rewrite are not reflected in `LiveUnits`, causing subsequent rewrites to incorrectly determine that certain registers are not live.

As an example:


For frame accesses with offsets > 127 bytes (requiring `ADD` instead of `LEA`), the Z80 backend inserts `push af / add iy, de / pop af` to preserve the flags register when flags are live. After this fix:

```asm
call    __llcmpu           ; sets carry flag
ld      de, -142
lea     iy, ix
add     iy, de             ; NO push/pop AF, flags were live but not detected!
...
push    af                 ; only the LAST frame access is protected
add     iy, de
pop     af
jp      nc, label          ; Uses (clobbered) carry flag
```

Only 3 of 6 required flag preserving sequences were emitted. The first frame accesses processed (last in program order) correctly detect live flags, but subsequent ones see stale `LiveUnits`.

We assume the root cause is that in `replaceFrameIndicesBackward`:

1. `eliminateFrameIndex` inserts new instructions
2. PEI calls `LocalRS->skipTo(std::prev(Save))` to restore scavenger position
3. `skipTo()` only moves the iterator, it does NOT update `LiveUnits`
4. Subsequent calls to `RS->isRegUsed()` return stale results

our workaround is to implement a forward scannuing liveness check in our `rewriteFrameIndex()`

```cpp
auto isFlagLiveForward = [&]() -> bool {
  if (RS && RS->isRegUsed(Z80::F)) return true;
  for (auto I = std::next(II); I != MBB.end(); ++I) {
    if (I->definesRegister(Z80::F, &TRI)) return false;
    if (I->readsRegister(Z80::F, &TRI)) return true;
  }
  for (auto *Succ : MBB.successors())
    if (Succ->isLiveIn(Z80::F)) return true;
  return false;
};
```

A couple questions:
1. Is `skipTo()` intentionally designed to not update liveness?
2. Should `eliminateFrameIndex` return information about inserted instructions so PEI can update the scavenger?



---

