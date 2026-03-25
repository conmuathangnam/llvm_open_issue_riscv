# [RISCV] `clang` encounters a fatal error in `getCopyFromParts()` when referencing an array element in inline assembly for `Zilsd` and `Zclsd` instructions

**Author:** thebigclub
**URL:** https://github.com/llvm/llvm-project/issues/137726
**Status:** Closed
**Labels:** backend:RISC-V, crash
**Closed Date:** 2025-04-29T15:12:34Z

## Body

This happens even when you have an invalid combination (`Zilsd` paired with `c.ld`). I'll document a single example:

```bash
$ cat << EOF > test.c
#include <stdint.h>

int fn(void *arg1, int arg2, int arg3, int arg4, int arg5) {
        uint32_t val[2];

        asm volatile ("c.ld %0, 0(%1)"
                      : "=R" (val[0])
                      : "r" (arg1)
        );

        return val[1] + arg2 + arg3 + arg4 + arg5;
}
EOF
$ clang -march=rv32i_zclsd -c test.c
fatal error: error in backend: Unknown mismatch in getCopyFromParts!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: clang -march=rv32i_zclsd -c test.c
1.	<eof> parser at end of file
2.	Code generation
3.	Running pass 'Function Pass Manager' on module 'test.c'.
4.	Running pass 'RISC-V DAG->DAG Pattern Instruction Selection' on function '@fn'
 #0 0x00000000016f8c1b llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/home/thebigclub/riscv/llvm/rv32ec/bin/clang-21+0x16f8c1b)
 #1 0x00000000016f6a44 llvm::sys::CleanupOnSignal(unsigned long) (/home/thebigclub/riscv/llvm/rv32ec/bin/clang-21+0x16f6a44)
 #2 0x00000000016441b2 llvm::CrashRecoveryContext::HandleExit(int) (/home/thebigclub/riscv/llvm/rv32ec/bin/clang-21+0x16441b2)
 #3 0x00000000016edf0e llvm::sys::Process::Exit(int, bool) (/home/thebigclub/riscv/llvm/rv32ec/bin/clang-21+0x16edf0e)
 #4 0x000000000056f897 LLVMErrorHandler(void*, char const*, bool) cc1_main.cpp:0:0
 #5 0x000000000164c71c llvm::report_fatal_error(llvm::Twine const&, bool) (/home/thebigclub/riscv/llvm/rv32ec/bin/clang-21+0x164c71c)
 #6 0x000000000164c878 (/home/thebigclub/riscv/llvm/rv32ec/bin/clang-21+0x164c878)
 #7 0x0000000002919ec1 getCopyFromParts(llvm::SelectionDAG&, llvm::SDLoc const&, llvm::SDValue const*, unsigned int, llvm::MVT, llvm::EVT, llvm::Value const*, llvm::SDValue, std::optional<unsigned int>, std::optional<llvm::ISD::NodeType>) (.isra.0) SelectionDAGBuilder.cpp:0:0
 #8 0x000000000291de37 llvm::RegsForValue::getCopyFromRegs(llvm::SelectionDAG&, llvm::FunctionLoweringInfo&, llvm::SDLoc const&, llvm::SDValue&, llvm::SDValue*, llvm::Value const*) const (/home/thebigclub/riscv/llvm/rv32ec/bin/clang-21+0x291de37)
 #9 0x0000000002947f32 llvm::SelectionDAGBuilder::visitInlineAsm(llvm::CallBase const&, llvm::BasicBlock const*) (/home/thebigclub/riscv/llvm/rv32ec/bin/clang-21+0x2947f32)
#10 0x000000000297eb80 llvm::SelectionDAGBuilder::visit(llvm::Instruction const&) (/home/thebigclub/riscv/llvm/rv32ec/bin/clang-21+0x297eb80)
#11 0x0000000002a0270c llvm::SelectionDAGISel::SelectBasicBlock(llvm::ilist_iterator_w_bits<llvm::ilist_detail::node_options<llvm::Instruction, false, false, void, true, llvm::BasicBlock>, false, true>, llvm::ilist_iterator_w_bits<llvm::ilist_detail::node_options<llvm::Instruction, false, false, void, true, llvm::BasicBlock>, false, true>, bool&) (/home/thebigclub/riscv/llvm/rv32ec/bin/clang-21+0x2a0270c)
#12 0x0000000002a03bc7 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&) (/home/thebigclub/riscv/llvm/rv32ec/bin/clang-21+0x2a03bc7)
#13 0x0000000002a05b08 llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&) (/home/thebigclub/riscv/llvm/rv32ec/bin/clang-21+0x2a05b08)
#14 0x00000000029f4319 llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&) (/home/thebigclub/riscv/llvm/rv32ec/bin/clang-21+0x29f4319)
#15 0x0000000000b40813 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (.part.0) MachineFunctionPass.cpp:0:0
#16 0x000000000115b689 llvm::FPPassManager::runOnFunction(llvm::Function&) (/home/thebigclub/riscv/llvm/rv32ec/bin/clang-21+0x115b689)
#17 0x000000000115b82c llvm::FPPassManager::runOnModule(llvm::Module&) (/home/thebigclub/riscv/llvm/rv32ec/bin/clang-21+0x115b82c)
#18 0x000000000115aa54 llvm::legacy::PassManagerImpl::run(llvm::Module&) (/home/thebigclub/riscv/llvm/rv32ec/bin/clang-21+0x115aa54)
#19 0x00000000019a47be clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/home/thebigclub/riscv/llvm/rv32ec/bin/clang-21+0x19a47be)
#20 0x000000000202b6f4 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/home/thebigclub/riscv/llvm/rv32ec/bin/clang-21+0x202b6f4)
#21 0x0000000003aa6edc clang::ParseAST(clang::Sema&, bool, bool) (/home/thebigclub/riscv/llvm/rv32ec/bin/clang-21+0x3aa6edc)
#22 0x00000000023647fa clang::FrontendAction::Execute() (/home/thebigclub/riscv/llvm/rv32ec/bin/clang-21+0x23647fa)
#23 0x00000000022e21be clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/home/thebigclub/riscv/llvm/rv32ec/bin/clang-21+0x22e21be)
#24 0x000000000243a247 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/home/thebigclub/riscv/llvm/rv32ec/bin/clang-21+0x243a247)
#25 0x00000000005714f6 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/home/thebigclub/riscv/llvm/rv32ec/bin/clang-21+0x5714f6)
#26 0x0000000000569f13 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#27 0x00000000020a0989 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#28 0x00000000016440c3 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/home/thebigclub/riscv/llvm/rv32ec/bin/clang-21+0x16440c3)
#29 0x00000000020a10d4 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#30 0x000000000206253b clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/home/thebigclub/riscv/llvm/rv32ec/bin/clang-21+0x206253b)
#31 0x0000000002063502 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/home/thebigclub/riscv/llvm/rv32ec/bin/clang-21+0x2063502)
#32 0x0000000002077e3c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/home/thebigclub/riscv/llvm/rv32ec/bin/clang-21+0x2077e3c)
#33 0x000000000056e47e clang_main(int, char**, llvm::ToolContext const&) (/home/thebigclub/riscv/llvm/rv32ec/bin/clang-21+0x56e47e)
#34 0x000000000043fb73 main (/home/thebigclub/riscv/llvm/rv32ec/bin/clang-21+0x43fb73)
#35 0x00007fb6b26115f5 __libc_start_call_main (/lib64/libc.so.6+0x35f5)
#36 0x00007fb6b26116a8 __libc_start_main@GLIBC_2.2.5 (/lib64/libc.so.6+0x36a8)
#37 0x0000000000569965 _start (/home/thebigclub/riscv/llvm/rv32ec/bin/clang-21+0x569965)
clang: error: clang frontend command failed with exit code 70 (use -v to see invocation)
clang version 21.0.0git (https://github.com/llvm/llvm-project.git 913dcf1aa36f3ea2d67a0d2b05b9d1375987e553)
Target: riscv32-unknown-unknown-elf
Thread model: posix
InstalledDir: /home/thebigclub/riscv/llvm/rv32ec/bin
clang: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang: note: diagnostic msg: /tmp/test-f5e115.c
clang: note: diagnostic msg: /tmp/test-f5e115.sh
clang: note: diagnostic msg: 

********************
$ clang -v
clang version 21.0.0git (https://github.com/llvm/llvm-project.git 913dcf1aa36f3ea2d67a0d2b05b9d1375987e553)
Target: riscv32-unknown-unknown-elf
```

[test-f5e115.c.txt](https://github.com/user-attachments/files/19949915/test-f5e115.c.txt)
[test-f5e115.sh.txt](https://github.com/user-attachments/files/19949914/test-f5e115.sh.txt)

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-risc-v

Author: None (thebigclub)

<details>
This happens even when you have an invalid combination (`Zilsd` paired with `c.ld`). I'll document a single example:

```bash
$ cat &lt;&lt; EOF &gt; test.c
#include &lt;stdint.h&gt;

int fn(void *arg1, int arg2, int arg3, int arg4, int arg5) {
        uint32_t val[2];

        asm volatile ("c.ld %0, 0(%1)"
                      : "=R" (val[0])
                      : "r" (arg1)
        );

        return val[1] + arg2 + arg3 + arg4 + arg5;
}
EOF
$ clang -march=rv32i_zclsd -c test.c
fatal error: error in backend: Unknown mismatch in getCopyFromParts!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: clang -march=rv32i_zclsd -c test.c
1.	&lt;eof&gt; parser at end of file
2.	Code generation
3.	Running pass 'Function Pass Manager' on module 'test.c'.
4.	Running pass 'RISC-V DAG-&gt;DAG Pattern Instruction Selection' on function '@<!-- -->fn'
 #<!-- -->0 0x00000000016f8c1b llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/home/thebigclub/riscv/llvm/rv32ec/bin/clang-21+0x16f8c1b)
 #<!-- -->1 0x00000000016f6a44 llvm::sys::CleanupOnSignal(unsigned long) (/home/thebigclub/riscv/llvm/rv32ec/bin/clang-21+0x16f6a44)
 #<!-- -->2 0x00000000016441b2 llvm::CrashRecoveryContext::HandleExit(int) (/home/thebigclub/riscv/llvm/rv32ec/bin/clang-21+0x16441b2)
 #<!-- -->3 0x00000000016edf0e llvm::sys::Process::Exit(int, bool) (/home/thebigclub/riscv/llvm/rv32ec/bin/clang-21+0x16edf0e)
 #<!-- -->4 0x000000000056f897 LLVMErrorHandler(void*, char const*, bool) cc1_main.cpp:0:0
 #<!-- -->5 0x000000000164c71c llvm::report_fatal_error(llvm::Twine const&amp;, bool) (/home/thebigclub/riscv/llvm/rv32ec/bin/clang-21+0x164c71c)
 #<!-- -->6 0x000000000164c878 (/home/thebigclub/riscv/llvm/rv32ec/bin/clang-21+0x164c878)
 #<!-- -->7 0x0000000002919ec1 getCopyFromParts(llvm::SelectionDAG&amp;, llvm::SDLoc const&amp;, llvm::SDValue const*, unsigned int, llvm::MVT, llvm::EVT, llvm::Value const*, llvm::SDValue, std::optional&lt;unsigned int&gt;, std::optional&lt;llvm::ISD::NodeType&gt;) (.isra.0) SelectionDAGBuilder.cpp:0:0
 #<!-- -->8 0x000000000291de37 llvm::RegsForValue::getCopyFromRegs(llvm::SelectionDAG&amp;, llvm::FunctionLoweringInfo&amp;, llvm::SDLoc const&amp;, llvm::SDValue&amp;, llvm::SDValue*, llvm::Value const*) const (/home/thebigclub/riscv/llvm/rv32ec/bin/clang-21+0x291de37)
 #<!-- -->9 0x0000000002947f32 llvm::SelectionDAGBuilder::visitInlineAsm(llvm::CallBase const&amp;, llvm::BasicBlock const*) (/home/thebigclub/riscv/llvm/rv32ec/bin/clang-21+0x2947f32)
#<!-- -->10 0x000000000297eb80 llvm::SelectionDAGBuilder::visit(llvm::Instruction const&amp;) (/home/thebigclub/riscv/llvm/rv32ec/bin/clang-21+0x297eb80)
#<!-- -->11 0x0000000002a0270c llvm::SelectionDAGISel::SelectBasicBlock(llvm::ilist_iterator_w_bits&lt;llvm::ilist_detail::node_options&lt;llvm::Instruction, false, false, void, true, llvm::BasicBlock&gt;, false, true&gt;, llvm::ilist_iterator_w_bits&lt;llvm::ilist_detail::node_options&lt;llvm::Instruction, false, false, void, true, llvm::BasicBlock&gt;, false, true&gt;, bool&amp;) (/home/thebigclub/riscv/llvm/rv32ec/bin/clang-21+0x2a0270c)
#<!-- -->12 0x0000000002a03bc7 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&amp;) (/home/thebigclub/riscv/llvm/rv32ec/bin/clang-21+0x2a03bc7)
#<!-- -->13 0x0000000002a05b08 llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&amp;) (/home/thebigclub/riscv/llvm/rv32ec/bin/clang-21+0x2a05b08)
#<!-- -->14 0x00000000029f4319 llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&amp;) (/home/thebigclub/riscv/llvm/rv32ec/bin/clang-21+0x29f4319)
#<!-- -->15 0x0000000000b40813 llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (.part.0) MachineFunctionPass.cpp:0:0
#<!-- -->16 0x000000000115b689 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (/home/thebigclub/riscv/llvm/rv32ec/bin/clang-21+0x115b689)
#<!-- -->17 0x000000000115b82c llvm::FPPassManager::runOnModule(llvm::Module&amp;) (/home/thebigclub/riscv/llvm/rv32ec/bin/clang-21+0x115b82c)
#<!-- -->18 0x000000000115aa54 llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/home/thebigclub/riscv/llvm/rv32ec/bin/clang-21+0x115aa54)
#<!-- -->19 0x00000000019a47be clang::emitBackendOutput(clang::CompilerInstance&amp;, clang::CodeGenOptions&amp;, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;, std::unique_ptr&lt;llvm::raw_pwrite_stream, std::default_delete&lt;llvm::raw_pwrite_stream&gt;&gt;, clang::BackendConsumer*) (/home/thebigclub/riscv/llvm/rv32ec/bin/clang-21+0x19a47be)
#<!-- -->20 0x000000000202b6f4 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&amp;) (/home/thebigclub/riscv/llvm/rv32ec/bin/clang-21+0x202b6f4)
#<!-- -->21 0x0000000003aa6edc clang::ParseAST(clang::Sema&amp;, bool, bool) (/home/thebigclub/riscv/llvm/rv32ec/bin/clang-21+0x3aa6edc)
#<!-- -->22 0x00000000023647fa clang::FrontendAction::Execute() (/home/thebigclub/riscv/llvm/rv32ec/bin/clang-21+0x23647fa)
#<!-- -->23 0x00000000022e21be clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/home/thebigclub/riscv/llvm/rv32ec/bin/clang-21+0x22e21be)
#<!-- -->24 0x000000000243a247 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/home/thebigclub/riscv/llvm/rv32ec/bin/clang-21+0x243a247)
#<!-- -->25 0x00000000005714f6 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/home/thebigclub/riscv/llvm/rv32ec/bin/clang-21+0x5714f6)
#<!-- -->26 0x0000000000569f13 ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->27 0x00000000020a0989 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->28 0x00000000016440c3 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/home/thebigclub/riscv/llvm/rv32ec/bin/clang-21+0x16440c3)
#<!-- -->29 0x00000000020a10d4 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->30 0x000000000206253b clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/home/thebigclub/riscv/llvm/rv32ec/bin/clang-21+0x206253b)
#<!-- -->31 0x0000000002063502 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/home/thebigclub/riscv/llvm/rv32ec/bin/clang-21+0x2063502)
#<!-- -->32 0x0000000002077e3c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/home/thebigclub/riscv/llvm/rv32ec/bin/clang-21+0x2077e3c)
#<!-- -->33 0x000000000056e47e clang_main(int, char**, llvm::ToolContext const&amp;) (/home/thebigclub/riscv/llvm/rv32ec/bin/clang-21+0x56e47e)
#<!-- -->34 0x000000000043fb73 main (/home/thebigclub/riscv/llvm/rv32ec/bin/clang-21+0x43fb73)
#<!-- -->35 0x00007fb6b26115f5 __libc_start_call_main (/lib64/libc.so.6+0x35f5)
#<!-- -->36 0x00007fb6b26116a8 __libc_start_main@<!-- -->GLIBC_2.2.5 (/lib64/libc.so.6+0x36a8)
#<!-- -->37 0x0000000000569965 _start (/home/thebigclub/riscv/llvm/rv32ec/bin/clang-21+0x569965)
clang: error: clang frontend command failed with exit code 70 (use -v to see invocation)
clang version 21.0.0git (https://github.com/llvm/llvm-project.git 913dcf1aa36f3ea2d67a0d2b05b9d1375987e553)
Target: riscv32-unknown-unknown-elf
Thread model: posix
InstalledDir: /home/thebigclub/riscv/llvm/rv32ec/bin
clang: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang: note: diagnostic msg: /tmp/test-f5e115.c
clang: note: diagnostic msg: /tmp/test-f5e115.sh
clang: note: diagnostic msg: 

********************
$ clang -v
clang version 21.0.0git (https://github.com/llvm/llvm-project.git 913dcf1aa36f3ea2d67a0d2b05b9d1375987e553)
Target: riscv32-unknown-unknown-elf
```

[test-f5e115.c.txt](https://github.com/user-attachments/files/19949915/test-f5e115.c.txt)
[test-f5e115.sh.txt](https://github.com/user-attachments/files/19949914/test-f5e115.sh.txt)
</details>


---

### Comment 2 - topperc

Why are you using a uint32_t array? The correct type for R constraint should be uint64_t.

We shouldn't crash though.

---

### Comment 3 - thebigclub

I totally agree. I just happened to notice this when I was trying to get it to work (I wasn't sure what type to use). I also found another bug related to this that I'll file.

---

### Comment 4 - lenary

Thanks for fixing this, Craig!

---

