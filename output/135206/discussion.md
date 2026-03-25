# [RISC-V] Infinite loop when compiling w/ -fstack-clash-protection on RISC-V

**Author:** ilovepi
**URL:** https://github.com/llvm/llvm-project/issues/135206
**Status:** Closed
**Labels:** backend:RISC-V, release:backport, needs-reduction
**Closed Date:** 2025-04-18T16:12:53Z

## Body

When compiling some Fuchsia code for RISC-V, we found that enabling -fstack-clash-protection started causing our builders to time out. When investigating locally, I found that the build would not complete, even after more than a full day. Typically these files compile on the order of seconds, so there seems to be something wrong in the RISC-V implementation of stack clash protection. Note: we don't see these issues on x86_64 or Aarch64.

I'm uploading a reproducer, but I haven't had a chance to reduce it yet.

[clang-crashreports.zip](https://github.com/user-attachments/files/19688491/clang-crashreports.zip)

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-risc-v

Author: Paul Kirth (ilovepi)

<details>
When compiling some Fuchsia code for RISC-V, we found that enabling -fstack-clash-protection started causing our builders to time out. When investigating locally, I found that the build would not complete, even after more than a full day. Typically these files compile on the order of seconds, so there seems to be something wrong in the RISC-V implementation of stack clash protection. Note: we don't see these issues on x86_64 or Aarch64.

I'm uploading a reproducer, but this I haven't had a chance to reduce it yet.

[clang-crashreports.zip](https://github.com/user-attachments/files/19688491/clang-crashreports.zip)
</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-bug

Author: Paul Kirth (ilovepi)

<details>
When compiling some Fuchsia code for RISC-V, we found that enabling -fstack-clash-protection started causing our builders to time out. When investigating locally, I found that the build would not complete, even after more than a full day. Typically these files compile on the order of seconds, so there seems to be something wrong in the RISC-V implementation of stack clash protection. Note: we don't see these issues on x86_64 or Aarch64.

I'm uploading a reproducer, but this I haven't had a chance to reduce it yet.

[clang-crashreports.zip](https://github.com/user-attachments/files/19688491/clang-crashreports.zip)
</details>


---

### Comment 3 - topperc

cc: @rzinsly 

---

### Comment 4 - topperc

Assertions build gives this error

```
clang++: llvm/include/llvm/CodeGen/MachineBasicBlock.h:1045: iterator llvm::MachineBasicBlock::insert(iterator, MachineInstr *): Assertion `(I == end() || I->getParent() == this) && "iterator points outside of basic block"' failed.
```

with this stack trace

```
-lto-objects -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -x c++ sysmem_tests-1a6d89.cpp -mllvm -opt-bisect-limit=-1
1.      <eof> parser at end of file
2.      Code generation
3.      Running pass 'Function Pass Manager' on module 'sysmem_tests-1a6d89.cpp'.
4.      Running pass 'Prologue/Epilogue Insertion & Frame Finalization' on function '@_ZN12_GLOBAL__N_121AttachTokenSucceedsV1EbbN3fit13function_implILm16ELb0EFvRN14fuchsia_sysmem4wire27BufferCollectionConstraintsEENSt3__29allocatorISt4byteEEEESB_SB_NS1_ILm16ELb0EFvRNS3_21BufferCollectionInfo2EESA_EEj'
 #0 0x000055c5984332f8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (./bin/clang+++0x3c842f8)
 #1 0x000055c598430e4e llvm::sys::RunSignalHandlers() (./bin/clang+++0x3c81e4e)
 #2 0x000055c5984339a6 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x00007f83913e6cf0 __restore_rt (/lib64/libpthread.so.0+0x12cf0)
 #4 0x00007f838fe66acf raise (/lib64/libc.so.6+0x4eacf)
 #5 0x00007f838fe39ea5 abort (/lib64/libc.so.6+0x21ea5)
 #6 0x00007f838fe39d79 _nl_load_domain.cold.0 (/lib64/libc.so.6+0x21d79)
 #7 0x00007f838fe5f426 (/lib64/libc.so.6+0x47426)
 #8 0x000055c595ab8e62 (./bin/clang+++0x1309e62)
 #9 0x000055c596be5c4b llvm::RISCVInstrInfo::movImm(llvm::MachineBasicBlock&, llvm::MachineInstrBundleIterator<llvm::MachineInstr, false>, llvm::DebugLoc const&, llvm::Register, unsigned long, llvm::MachineInstr::MIFlag, bool, bool) const (./bin/clang+++0x2436c4b)
#10 0x000055c596ca3f5d llvm::RISCVFrameLowering::inlineStackProbe(llvm::MachineFunction&, llvm::MachineBasicBlock&) const (./bin/clang+++0x24f4f5d)
#11 0x000055c597a38b84 (anonymous namespace)::PEI::runOnMachineFunction(llvm::MachineFunction&) PrologEpilogInserter.cpp:0:0
#12 0x000055c5978dc88b llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (./bin/clang+++0x312d88b)
#13 0x000055c597e31e15 llvm::FPPassManager::runOnFunction(llvm::Function&) (./bin/clang+++0x3682e15)
#14 0x000055c597e39b52 llvm::FPPassManager::runOnModule(llvm::Module&) (./bin/clang+++0x368ab52)
#15 0x000055c597e32661 llvm::legacy::PassManagerImpl::run(llvm::Module&) (./bin/clang+++0x3683661)
#16 0x000055c59865b7c0 clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (./bin/clang+++0x3eac7c0)
#17 0x000055c598c61160 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (./bin/clang+++0x44b2160)
#18 0x000055c59a314b79 clang::ParseAST(clang::Sema&, bool, bool) (./bin/clang+++0x5b65b79)
#19 0x000055c598f0a126 clang::FrontendAction::Execute() (./bin/clang+++0x475b126)
#20 0x000055c598e7e39d clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (./bin/clang+++0x46cf39d)
#21 0x000055c598ff1d0f clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (./bin/clang+++0x4842d0f)
#22 0x000055c595a78a74 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (./bin/clang+++0x12c9a74)
#23 0x000055c595a752f5 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#24 0x000055c595a73f65 clang_main(int, char**, llvm::ToolContext const&) (./bin/clang+++0x12c4f65)
#25 0x000055c595a84197 main (./bin/clang+++0x12d5197)
#26 0x00007f838fe52d85 __libc_start_main (/lib64/libc.so.6+0x3ad85)
#27 0x000055c595a72a8e _start (./bin/clang+++0x12c3a8e)
```


---

### Comment 5 - topperc

Proposed fix. We can't assume MBBI is still pointing at MBB if we've already expanded a probe. We need to re-query the MBB from MBBI.

```
diff --git a/llvm/lib/Target/RISCV/RISCVFrameLowering.cpp b/llvm/lib/Target/RISCV/RISCVFrameLowering.cpp
index c7b2b781422d..a83119957d95 100644
--- a/llvm/lib/Target/RISCV/RISCVFrameLowering.cpp
+++ b/llvm/lib/Target/RISCV/RISCVFrameLowering.cpp
@@ -2270,11 +2270,13 @@ TargetStackID::Value RISCVFrameLowering::getStackIDForScalableVectors() const {
 }
 
 // Synthesize the probe loop.
-static void emitStackProbeInline(MachineFunction &MF, MachineBasicBlock &MBB,
-                                 MachineBasicBlock::iterator MBBI, DebugLoc DL,
+static void emitStackProbeInline(MachineBasicBlock::iterator MBBI, DebugLoc DL,
                                  Register TargetReg, bool IsRVV) {
   assert(TargetReg != RISCV::X2 && "New top of stack cannot already be in SP");
 
+  MachineBasicBlock &MBB = *MBBI->getParent();
+  MachineFunction &MF = *MBB.getParent();
+
   auto &Subtarget = MF.getSubtarget<RISCVSubtarget>();
   const RISCVInstrInfo *TII = Subtarget.getInstrInfo();
   bool IsRV64 = Subtarget.is64Bit();
@@ -2363,7 +2365,7 @@ void RISCVFrameLowering::inlineStackProbe(MachineFunction &MF,
       MachineBasicBlock::iterator MBBI = MI->getIterator();
       DebugLoc DL = MBB.findDebugLoc(MBBI);
       Register TargetReg = MI->getOperand(1).getReg();
-      emitStackProbeInline(MF, MBB, MBBI, DL, TargetReg,
+      emitStackProbeInline(MBBI, DL, TargetReg,
                            (MI->getOpcode() == RISCV::PROBED_STACKALLOC_RVV));
       MBBI->eraseFromParent();
     }
```

---

### Comment 6 - ilovepi

@topperc That does fix the assertion, and I'm not seeing an infinite loop when compiling some other files too. Thanks.

---

### Comment 7 - topperc

Looks like this needs to be backported to llvm-20.

---

### Comment 8 - topperc

/cherry-pick b3d2dc321c5c78b7204696afe07fe6ef3375acfd

---

### Comment 9 - topperc

/cherry-pick https://github.com/llvm/llvm-project/commit/b3d2dc321c5c78b7204696afe07fe6ef3375acfd

---

### Comment 10 - tstellar

/cherry-pick b3d2dc321c5c78b7204696afe07fe6ef3375acfd

---

### Comment 11 - llvmbot

/pull-request llvm/llvm-project#139388

---

