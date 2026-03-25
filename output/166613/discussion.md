# [RISC-V] Crashed at -O2: Assertion `LVI->end >= Stop && "range ends mid block with no uses"' failed.

**Author:** cardigan1008
**URL:** https://github.com/llvm/llvm-project/issues/166613
**Status:** Closed
**Labels:** backend:RISC-V, llvm:regalloc, crash
**Closed Date:** 2025-11-06T14:18:54Z

## Body

This code crashed at `-O2`:

```c
#include "riscv_vector.h"
char b;
int i;
int a();
#define c() 0
void h() {
  {
    long d = 1, g;
    int e = a(), f = __builtin_rvv_vsetvli(e + d, 3, 0);
    __riscv_vle8ff_v_i8m1(&b, &g, f);
  }
  {
    a();
    int f = __builtin_rvv_vsetvli(1, 3, 0);
    long g;
    __riscv_vle8ff_v_i8m1(&b, &g, f);
  }
  i ? a() : c();
}
```

Crash:

```sh
clang-22: /data/yunboni/utils/compilers/repos/llvm-project/llvm/lib/CodeGen/SplitKit.cpp:229: void llvm::SplitAnalysis::c
alcLiveBlockInfo(): Assertion `LVI->end >= Stop && "range ends mid block with no uses"' failed.  
```

Backtrace:

```sh
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang -g -o /app/output.s -fno-verbose-asm -S -target riscv64-unknown-linux-gnu -march=rv64gc -mabi=lp64d --gcc-toolchain=/opt/compiler-explorer/riscv64/gcc-8.2.0/riscv64-unknown-linux-gnu --sysroot=/opt/compiler-explorer/riscv64/gcc-8.2.0/riscv64-unknown-linux-gnu/riscv64-unknown-linux-gnu/sysroot -fcolor-diagnostics -fno-crash-diagnostics --target=riscv64-unknown-linux-gnu -march=rv64gcv -mabi=lp64d -menable-experimental-extensions -O2 <source>
1.	<eof> parser at end of file
2.	Code generation
3.	Running pass 'Function Pass Manager' on module '<source>'.
4.	Running pass 'Greedy Register Allocator' on function '@h'
 #0 0x0000000003d11b38 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3d11b38)
 #1 0x0000000003d0f50c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3d0f50c)
 #2 0x0000000003c55298 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000074e44ba42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x0000000003330b20 llvm::SplitAnalysis::calcLiveBlockInfo() (/opt/compiler-explorer/clang-trunk/bin/clang+0x3330b20)
 #5 0x0000000003333031 llvm::SplitAnalysis::analyzeUses() (/opt/compiler-explorer/clang-trunk/bin/clang+0x3333031)
 #6 0x000000000329d9ae llvm::RAGreedy::trySplitAroundHintReg(unsigned short, llvm::LiveInterval const&, llvm::SmallVectorImpl<llvm::Register>&, llvm::AllocationOrder&) (/opt/compiler-explorer/clang-trunk/bin/clang+0x329d9ae)
 #7 0x00000000032a23c7 llvm::RAGreedy::tryAssign(llvm::LiveInterval const&, llvm::AllocationOrder&, llvm::SmallVectorImpl<llvm::Register>&, llvm::SmallSet<llvm::Register, 16u, std::less<llvm::Register>> const&) (/opt/compiler-explorer/clang-trunk/bin/clang+0x32a23c7)
 #8 0x00000000032a3fcb llvm::RAGreedy::selectOrSplitImpl(llvm::LiveInterval const&, llvm::SmallVectorImpl<llvm::Register>&, llvm::SmallSet<llvm::Register, 16u, std::less<llvm::Register>>&, llvm::SmallVector<std::pair<llvm::LiveInterval const*, llvm::MCRegister>, 8u>&, unsigned int) (/opt/compiler-explorer/clang-trunk/bin/clang+0x32a3fcb)
 #9 0x00000000032a4e37 llvm::RAGreedy::selectOrSplit(llvm::LiveInterval const&, llvm::SmallVectorImpl<llvm::Register>&) (/opt/compiler-explorer/clang-trunk/bin/clang+0x32a4e37)
#10 0x00000000034d9137 llvm::RegAllocBase::allocatePhysRegs() (/opt/compiler-explorer/clang-trunk/bin/clang+0x34d9137)
#11 0x00000000032a04f3 llvm::RAGreedy::run(llvm::MachineFunction&) (/opt/compiler-explorer/clang-trunk/bin/clang+0x32a04f3)
#12 0x00000000032a1b9a (anonymous namespace)::RAGreedyLegacy::runOnMachineFunction(llvm::MachineFunction&) RegAllocGreedy.cpp:0:0
#13 0x00000000030ef0dd llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-trunk/bin/clang+0x30ef0dd)
#14 0x0000000003663302 llvm::FPPassManager::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3663302)
#15 0x0000000003663591 llvm::FPPassManager::runOnModule(llvm::Module&) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3663591)
#16 0x0000000003664e73 llvm::legacy::PassManagerImpl::run(llvm::Module&) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3664e73)
#17 0x0000000003fae042 clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3fae042)
#18 0x00000000045e5ccb clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/opt/compiler-explorer/clang-trunk/bin/clang+0x45e5ccb)
#19 0x000000000625ba5c clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+0x625ba5c)
#20 0x00000000045e6825 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+0x45e6825)
#21 0x00000000048e4c2a clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+0x48e4c2a)
#22 0x000000000486496b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-trunk/bin/clang+0x486496b)
#23 0x00000000049dcdfb clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x49dcdfb)
#24 0x0000000000ddc575 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+0xddc575)
#25 0x0000000000dd44db ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#26 0x0000000000dd457d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#27 0x0000000004654899 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#28 0x0000000003c556b3 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3c556b3)
#29 0x0000000004654ab9 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#30 0x0000000004617682 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+0x4617682)
#31 0x0000000004618561 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+0x4618561)
#32 0x000000000462140c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-trunk/bin/clang+0x462140c)
#33 0x0000000000dd8fa9 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-trunk/bin/clang+0xdd8fa9)
#34 0x0000000000c83ec4 main (/opt/compiler-explorer/clang-trunk/bin/clang+0xc83ec4)
#35 0x000074e44ba29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#36 0x000074e44ba29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#37 0x0000000000dd3f75 _start (/opt/compiler-explorer/clang-trunk/bin/clang+0xdd3f75)
```

Compiler explorer: https://godbolt.org/z/oGWs13hf1

Clang version:

```sh
clang version 22.0.0git (https://github.com/llvm/llvm-project.git 4c2a9c4ba3796799032c12673510279d51c65370)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /data/yunboni/utils/compilers/llvm_latest_rvv/bin
Build config: +assertions
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-risc-v

Author: Yunbo Ni (cardigan1008)

<details>
This code crashed at `-O2`:

```c
#include "riscv_vector.h"
char b;
int i;
int a();
#define c() 0
void h() {
  {
    long d = 1, g;
    int e = a(), f = __builtin_rvv_vsetvli(e + d, 3, 0);
    __riscv_vle8ff_v_i8m1(&amp;b, &amp;g, f);
  }
  {
    a();
    int f = __builtin_rvv_vsetvli(1, 3, 0);
    long g;
    __riscv_vle8ff_v_i8m1(&amp;b, &amp;g, f);
  }
  i ? a() : c();
}
```

Crash:

```sh
clang-22: /data/yunboni/utils/compilers/repos/llvm-project/llvm/lib/CodeGen/SplitKit.cpp:229: void llvm::SplitAnalysis::c
alcLiveBlockInfo(): Assertion `LVI-&gt;end &gt;= Stop &amp;&amp; "range ends mid block with no uses"' failed.  
```

Backtrace:

```sh
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang -g -o /app/output.s -fno-verbose-asm -S -target riscv64-unknown-linux-gnu -march=rv64gc -mabi=lp64d --gcc-toolchain=/opt/compiler-explorer/riscv64/gcc-8.2.0/riscv64-unknown-linux-gnu --sysroot=/opt/compiler-explorer/riscv64/gcc-8.2.0/riscv64-unknown-linux-gnu/riscv64-unknown-linux-gnu/sysroot -fcolor-diagnostics -fno-crash-diagnostics --target=riscv64-unknown-linux-gnu -march=rv64gcv -mabi=lp64d -menable-experimental-extensions -O2 &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	Code generation
3.	Running pass 'Function Pass Manager' on module '&lt;source&gt;'.
4.	Running pass 'Greedy Register Allocator' on function '@<!-- -->h'
 #<!-- -->0 0x0000000003d11b38 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3d11b38)
 #<!-- -->1 0x0000000003d0f50c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3d0f50c)
 #<!-- -->2 0x0000000003c55298 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x000074e44ba42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x0000000003330b20 llvm::SplitAnalysis::calcLiveBlockInfo() (/opt/compiler-explorer/clang-trunk/bin/clang+0x3330b20)
 #<!-- -->5 0x0000000003333031 llvm::SplitAnalysis::analyzeUses() (/opt/compiler-explorer/clang-trunk/bin/clang+0x3333031)
 #<!-- -->6 0x000000000329d9ae llvm::RAGreedy::trySplitAroundHintReg(unsigned short, llvm::LiveInterval const&amp;, llvm::SmallVectorImpl&lt;llvm::Register&gt;&amp;, llvm::AllocationOrder&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x329d9ae)
 #<!-- -->7 0x00000000032a23c7 llvm::RAGreedy::tryAssign(llvm::LiveInterval const&amp;, llvm::AllocationOrder&amp;, llvm::SmallVectorImpl&lt;llvm::Register&gt;&amp;, llvm::SmallSet&lt;llvm::Register, 16u, std::less&lt;llvm::Register&gt;&gt; const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x32a23c7)
 #<!-- -->8 0x00000000032a3fcb llvm::RAGreedy::selectOrSplitImpl(llvm::LiveInterval const&amp;, llvm::SmallVectorImpl&lt;llvm::Register&gt;&amp;, llvm::SmallSet&lt;llvm::Register, 16u, std::less&lt;llvm::Register&gt;&gt;&amp;, llvm::SmallVector&lt;std::pair&lt;llvm::LiveInterval const*, llvm::MCRegister&gt;, 8u&gt;&amp;, unsigned int) (/opt/compiler-explorer/clang-trunk/bin/clang+0x32a3fcb)
 #<!-- -->9 0x00000000032a4e37 llvm::RAGreedy::selectOrSplit(llvm::LiveInterval const&amp;, llvm::SmallVectorImpl&lt;llvm::Register&gt;&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x32a4e37)
#<!-- -->10 0x00000000034d9137 llvm::RegAllocBase::allocatePhysRegs() (/opt/compiler-explorer/clang-trunk/bin/clang+0x34d9137)
#<!-- -->11 0x00000000032a04f3 llvm::RAGreedy::run(llvm::MachineFunction&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x32a04f3)
#<!-- -->12 0x00000000032a1b9a (anonymous namespace)::RAGreedyLegacy::runOnMachineFunction(llvm::MachineFunction&amp;) RegAllocGreedy.cpp:0:0
#<!-- -->13 0x00000000030ef0dd llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x30ef0dd)
#<!-- -->14 0x0000000003663302 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3663302)
#<!-- -->15 0x0000000003663591 llvm::FPPassManager::runOnModule(llvm::Module&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3663591)
#<!-- -->16 0x0000000003664e73 llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3664e73)
#<!-- -->17 0x0000000003fae042 clang::emitBackendOutput(clang::CompilerInstance&amp;, clang::CodeGenOptions&amp;, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;, std::unique_ptr&lt;llvm::raw_pwrite_stream, std::default_delete&lt;llvm::raw_pwrite_stream&gt;&gt;, clang::BackendConsumer*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3fae042)
#<!-- -->18 0x00000000045e5ccb clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x45e5ccb)
#<!-- -->19 0x000000000625ba5c clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+0x625ba5c)
#<!-- -->20 0x00000000045e6825 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+0x45e6825)
#<!-- -->21 0x00000000048e4c2a clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+0x48e4c2a)
#<!-- -->22 0x000000000486496b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x486496b)
#<!-- -->23 0x00000000049dcdfb clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x49dcdfb)
#<!-- -->24 0x0000000000ddc575 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+0xddc575)
#<!-- -->25 0x0000000000dd44db ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->26 0x0000000000dd457d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->27 0x0000000004654899 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->28 0x0000000003c556b3 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3c556b3)
#<!-- -->29 0x0000000004654ab9 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->30 0x0000000004617682 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+0x4617682)
#<!-- -->31 0x0000000004618561 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+0x4618561)
#<!-- -->32 0x000000000462140c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x462140c)
#<!-- -->33 0x0000000000dd8fa9 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0xdd8fa9)
#<!-- -->34 0x0000000000c83ec4 main (/opt/compiler-explorer/clang-trunk/bin/clang+0xc83ec4)
#<!-- -->35 0x000074e44ba29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->36 0x000074e44ba29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->37 0x0000000000dd3f75 _start (/opt/compiler-explorer/clang-trunk/bin/clang+0xdd3f75)
```

Compiler explorer: https://godbolt.org/z/oGWs13hf1

Clang version:

```sh
clang version 22.0.0git (https://github.com/llvm/llvm-project.git 4c2a9c4ba3796799032c12673510279d51c65370)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /data/yunboni/utils/compilers/llvm_latest_rvv/bin
Build config: +assertions
```
</details>


---

### Comment 2 - topperc

It looks like we delete an ADDI that isn't an LI and don't cleanup the live range of the source we disconnected.

This fixes it by only removing LI.

```
diff --git a/llvm/lib/Target/RISCV/RISCVInsertVSETVLI.cpp b/llvm/lib/Target/RISCV/RISCVInsertVSETVLI.cpp
index 636e31c47ddb..ec39467fa790 100644
--- a/llvm/lib/Target/RISCV/RISCVInsertVSETVLI.cpp
+++ b/llvm/lib/Target/RISCV/RISCVInsertVSETVLI.cpp
@@ -1580,7 +1580,9 @@ void RISCVInsertVSETVLI::emitVSETVLIs(MachineBasicBlock &MBB) {
             // vsetvli, or a vsetvli might not have been emitted, so it may be
             // dead now.
             for (MachineInstr *DeadMI : DeadMIs) {
-              if (!TII->isAddImmediate(*DeadMI, Reg))
+              if (!TII->isAddImmediate(*DeadMI, Reg) ||
+                  !DeadMI->getOperand(1).isReg() ||
+                  DeadMI->getOperand(1).getReg() != RISCV::X0)
                 continue;
               LIS->RemoveMachineInstrFromMaps(*DeadMI);
               DeadMI->eraseFromParent();
@@ -1780,6 +1782,8 @@ void RISCVInsertVSETVLI::coalesceVSETVLIs(MachineBasicBlock &MBB) const {
 
     MachineInstr *VLOpDef = MRI->getUniqueVRegDef(OldVLReg);
     if (VLOpDef && TII->isAddImmediate(*VLOpDef, OldVLReg) &&
+        VLOpDef->getOperand(1).isReg() &&
+        VLOpDef->getOperand(1).getReg() == RISCV::X0 &&
         MRI->use_nodbg_empty(OldVLReg))
       ToDelete.push_back(VLOpDef);
   };
```

---

### Comment 3 - topperc

CC @preames @lukel97 

---

