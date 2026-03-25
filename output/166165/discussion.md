# if-converter crashes with Assertion `TBB && "insertBranch must not be told to insert a fallthrough"' failed.

**Author:** mikaelholmen
**URL:** https://github.com/llvm/llvm-project/issues/166165

## Body

llvm commit: 1667feb0fd

Reproduce with:
```llc bbi-112026_2.mir -mtriple=thumbv7-apple-ios -o - -run-pass=if-converter```

Result:
```
llc: ../lib/Target/ARM/ARMBaseInstrInfo.cpp:339: virtual unsigned int llvm::ARMBaseInstrInfo::insertBranch(MachineBasicBlock &, MachineBasicBlock *, MachineBasicBlock *, ArrayRef<MachineOperand>, const DebugLoc &, int *) const: Assertion `TBB && "insertBranch must not be told to insert a fallthrough"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: build-all/bin/llc bbi-112026_2.mir -mtriple=thumbv7-apple-ios -o - -run-pass=if-converter
1.	Running pass 'Function Pass Manager' on module 'bbi-112026_2.mir'.
2.	Running pass 'If Converter' on function '@main'
 #0 0x0000560388228cf6 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (build-all/bin/llc+0x7f63cf6)
 #1 0x0000560388226445 llvm::sys::RunSignalHandlers() (build-all/bin/llc+0x7f61445)
 #2 0x0000560388229b09 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x00007ff74814b990 __restore_rt (/lib64/libpthread.so.0+0x12990)
 #4 0x00007ff745aeb52f raise (/lib64/libc.so.6+0x4e52f)
 #5 0x00007ff745abee65 abort (/lib64/libc.so.6+0x21e65)
 #6 0x00007ff745abed39 _nl_load_domain.cold.0 (/lib64/libc.so.6+0x21d39)
 #7 0x00007ff745ae3e86 (/lib64/libc.so.6+0x46e86)
 #8 0x0000560385aaae94 llvm::ARMBaseInstrInfo::insertBranch(llvm::MachineBasicBlock&, llvm::MachineBasicBlock*, llvm::MachineBasicBlock*, llvm::ArrayRef<llvm::MachineOperand>, llvm::DebugLoc const&, int*) const ARMBaseInstrInfo.cpp:0:0
 #9 0x000056038710650a (anonymous namespace)::IfConverter::runOnMachineFunction(llvm::MachineFunction&) IfConversion.cpp:0:0
#10 0x00005603872086b7 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (build-all/bin/llc+0x6f436b7)
#11 0x000056038777ea5c llvm::FPPassManager::runOnFunction(llvm::Function&) (build-all/bin/llc+0x74b9a5c)
#12 0x0000560387786ca2 llvm::FPPassManager::runOnModule(llvm::Module&) (build-all/bin/llc+0x74c1ca2)
#13 0x000056038777f528 llvm::legacy::PassManagerImpl::run(llvm::Module&) (build-all/bin/llc+0x74ba528)
#14 0x00005603850ab247 compileModule(char**, llvm::LLVMContext&) llc.cpp:0:0
#15 0x00005603850a87a0 main (build-all/bin/llc+0x4de37a0)
#16 0x00007ff745ad77e5 __libc_start_main (/lib64/libc.so.6+0x3a7e5)
#17 0x00005603850a79ee _start (build-all/bin/llc+0x4de29ee)

Abort (core dumped)
```

[bbi-112026_2.mir.gz](https://github.com/user-attachments/files/23303203/bbi-112026_2.mir.gz)

Note: This was originally found on my out-of-tree target with a generated C program and then run through the compiler with a random opt pipeline.
This left some code after opt that was unreachable from entry, that also contained an "unreachable". Then llc crashed on this when I compiled for my target.
I extracted the mir-reproducer, reduced it and converted to ARM and ended up with the above reproducer.
So a lot of fuzzing has been involved here and I'm not sure if we can end up in this situation with a default pipeline or not.

https://llvm.godbolt.org/z/c6ezns5v8



## Comments

### Comment 1 - mikaelholmen

I think the problem is a mismatch between what IfConverter::ValidForkedDiamond and IfConverter::reverseBranchCondition/TargetInstrInfo::insertBranch accepts.

ValidForkedDiamond looks for
```
/// While not strictly a diamond, this pattern would form a diamond if
/// tail-merging had merged the shared tails.
///           EBB
///         _/   \_
///         |     |
///        TBB   FBB
///        /  \ /   \
///  FalseBB TrueBB FalseBB
/// Currently only handles analyzable branches.
/// Specifically excludes actual diamonds to avoid overlap.
```
and it accepts fallthrough for several of the edges:
```
  MachineBasicBlock *TT = TrueBBI.TrueBB;
  MachineBasicBlock *TF = TrueBBI.FalseBB;
  MachineBasicBlock *FT = FalseBBI.TrueBB;
  MachineBasicBlock *FF = FalseBBI.FalseBB;

  if (!TT)
    TT = getNextBlock(*TrueBBI.BB);
  if (!TF)
    TF = getNextBlock(*TrueBBI.BB);
  if (!FT)
    FT = getNextBlock(*FalseBBI.BB);
  if (!FF)
    FF = getNextBlock(*FalseBBI.BB);
```
So if e.g. FF (i.e. FalseBBI.FalseBB) is null we just set it to the fallthrough block of FalseBBI.BB instead:
```
    FF = getNextBlock(*FalseBBI.BB);
```
But then it does
```
  if (TF == FT && TT == FF) {
    // If the branches are opposing, but we can't reverse, don't do it.
    if (!FalseBBI.IsBrReversible)
      return false;
    FalseReversed = true;
    reverseBranchCondition(FalseBBI);
  }
```
and pass FalseBBI to reverseBranchCondition which does
```
bool IfConverter::reverseBranchCondition(BBInfo &BBI) const {
  DebugLoc dl;  // FIXME: this is nowhere
  if (!TII->reverseBranchCondition(BBI.BrCond)) {
    TII->removeBranch(*BBI.BB);
    TII->insertBranch(*BBI.BB, BBI.FalseBB, BBI.TrueBB, BBI.BrCond, dl);
    std::swap(BBI.TrueBB, BBI.FalseBB);
    return true;
  }
  return false;
}
```
So we pass BBI.FalseBB (which might be null) to insertBranch and from what I've seen the insertBranch implementations all contain an assert like
```
assert(TBB && "insertBranch must not be told to insert a fallthrough");
```
which then fails.

---

