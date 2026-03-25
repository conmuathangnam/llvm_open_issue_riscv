# MIPS build of Linux kernel fails with `Unsupported instruction : <MCInst 0 <MCOperand Reg:346> <MCOperand Reg:252>>`

**Author:** bvanassche
**URL:** https://github.com/llvm/llvm-project/issues/181442
**Status:** Open
**Labels:** backend:MIPS, crash, needs-reduction

## Body

If I build commit f606cc48f9e482e65c1365ce695c87d309f3affd of https://github.com/bvanassche/linux with Clang commit d406ce8e20600171b6030c9dcf728a23975c9eaf and the attached kernel config, the following output appears:

[kernel-config.txt](https://github.com/user-attachments/files/25309254/kernel-config.txt)
[main-ad6244.c](https://github.com/user-attachments/files/25309255/main-ad6244.c)
[main-ad6244.sh](https://github.com/user-attachments/files/25309253/main-ad6244.sh)

```
cd ~/software/linux-kernel/ && make ARCH=mips LLVM=1
  SYNC    include/config/auto.conf
  UPD     include/generated/compile.h
  CC      scripts/mod/empty.o
  MKELF   scripts/mod/elfconfig.h
  HOSTCC  scripts/mod/modpost.o
  CC      scripts/mod/devicetable-offsets.s
  HOSTCC  scripts/mod/file2alias.o
  HOSTCC  scripts/mod/sumversion.o
  HOSTCC  scripts/mod/symsearch.o
  HOSTLD  scripts/mod/modpost
  CC      kernel/bounds.s
  CC      arch/mips/kernel/asm-offsets.s
  CC      kernel/sched/rq-offsets.s
  CALL    scripts/checksyscalls.sh
  CC      init/main.o
fatal error: error in backend: Unsupported instruction : <MCInst 0 <MCOperand Reg:346> <MCOperand Reg:252>>
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: clang -Wp,-MMD,init/.main.o.d -nostdinc -I./arch/mips/include -I./arch/mips/include/generated -I./include -I./include -I./arch/mips/include/uapi -I./arch/mips/include/generated/uapi -I./include/uapi -I./inclu\
de/generated/uapi -include ./include/linux/compiler-version.h -include ./include/linux/kconfig.h -include ./include/linux/compiler_types.h -D__KERNEL__ --target=mips[…]
1.      <eof> parser at end of file
2.      Code generation
3.      Running pass 'Function Pass Manager' on module 'init/main.c'.
4.      Running pass 'Mips Assembly Printer' on function '@start_kernel'
 #0 0x0000560d8f16b1d8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/usr/local/google/home/bvanassche/software/llvm-project/build/bin/clang-23+0x418e1d8)
 #1 0x0000560d8f168e54 llvm::sys::CleanupOnSignal(unsigned long) (/usr/local/google/home/bvanassche/software/llvm-project/build/bin/clang-23+0x418be54)
 #2 0x0000560d8f09c3e2 llvm::CrashRecoveryContext::HandleExit(int) (/usr/local/google/home/bvanassche/software/llvm-project/build/bin/clang-23+0x40bf3e2)
 #3 0x0000560d8f16056e llvm::sys::Process::Exit(int, bool) (/usr/local/google/home/bvanassche/software/llvm-project/build/bin/clang-23+0x418356e)
 #4 0x0000560d8bf13e67 LLVMErrorHandler(void*, char const*, bool) cc1_main.cpp:0:0
 #5 0x0000560d8f0a5c99 llvm::report_fatal_error(llvm::Twine const&, bool) (/usr/local/google/home/bvanassche/software/llvm-project/build/bin/clang-23+0x40c8c99)
 #6 0x0000560d8f0a5e19 (/usr/local/google/home/bvanassche/software/llvm-project/build/bin/clang-23+0x40c8e19)
 #7 0x0000560d8edefae6 (/usr/local/google/home/bvanassche/software/llvm-project/build/bin/clang-23+0x3e12ae6)
 #8 0x0000560d8d1cc0d1 llvm::MipsMCCodeEmitter::getBinaryCodeForInstr(llvm::MCInst const&, llvm::SmallVectorImpl<llvm::MCFixup>&, llvm::MCSubtargetInfo const&) const (/usr/local/google/home/bvanassche/software/llvm-project/build/bin/cl\
ang-23+0x21ef0d1)
 #9 0x0000560d8d1d2cc2 llvm::MipsMCCodeEmitter::encodeInstruction(llvm::MCInst const&, llvm::SmallVectorImpl<char>&, llvm::SmallVectorImpl<llvm::MCFixup>&, llvm::MCSubtargetInfo const&) const (/usr/local/google/home/bvanassche/software\
/llvm-project/build/bin/clang-23+0x21f5cc2)
#10 0x0000560d8ee26774 llvm::MCObjectStreamer::emitInstToData(llvm::MCInst const&, llvm::MCSubtargetInfo const&) (/usr/local/google/home/bvanassche/software/llvm-project/build/bin/clang-23+0x3e49774)
#11 0x0000560d8d1c78c5 llvm::MipsELFStreamer::emitInstruction(llvm::MCInst const&, llvm::MCSubtargetInfo const&) (/usr/local/google/home/bvanassche/software/llvm-project/build/bin/clang-23+0x21ea8c5)
#12 0x0000560d8d093fc4 llvm::MipsAsmPrinter::emitInstruction(llvm::MachineInstr const*) (/usr/local/google/home/bvanassche/software/llvm-project/build/bin/clang-23+0x20b6fc4)
#13 0x0000560d90094224 llvm::AsmPrinter::emitFunctionBody() (/usr/local/google/home/bvanassche/software/llvm-project/build/bin/clang-23+0x50b7224)
#14 0x0000560d8d08fc6b llvm::MipsAsmPrinter::runOnMachineFunction(llvm::MachineFunction&) (/usr/local/google/home/bvanassche/software/llvm-project/build/bin/clang-23+0x20b2c6b)
#15 0x0000560d8e4471fb llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (/usr/local/google/home/bvanassche/software/llvm-project/build/bin/clang-23+0x346a1fb)
#16 0x0000560d8ea2e8c1 llvm::FPPassManager::runOnFunction(llvm::Function&) (/usr/local/google/home/bvanassche/software/llvm-project/build/bin/clang-23+0x3a518c1)
#17 0x0000560d8ea2eb63 llvm::FPPassManager::runOnModule(llvm::Module&) (/usr/local/google/home/bvanassche/software/llvm-project/build/bin/clang-23+0x3a51b63)
#18 0x0000560d8ea2ddb1 llvm::legacy::PassManagerImpl::run(llvm::Module&) (/usr/local/google/home/bvanassche/software/llvm-project/build/bin/clang-23+0x3a50db1)
#19 0x0000560d8f433569 (anonymous namespace)::EmitAssemblyHelper::RunCodegenPipeline(clang::BackendAction, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>&, std::unique_ptr<llvm::ToolOutputFile, s\
td::default_delete<llvm::ToolOutputFile>>&) BackendUtil.cpp:0:0
#20 0x0000560d8f43414f clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_s\
tream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/usr/local/google/home/bvanassche/software/llvm-project/build/bin/clang-23+0x445714f)
#21 0x0000560d8fa7deb0 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/usr/local/google/home/bvanassche/software/llvm-project/build/bin/clang-23+0x4aa0eb0)
#22 0x0000560d9156927c clang::ParseAST(clang::Sema&, bool, bool) (/usr/local/google/home/bvanassche/software/llvm-project/build/bin/clang-23+0x658c27c)
#23 0x0000560d8fdd34e6 clang::FrontendAction::Execute() (/usr/local/google/home/bvanassche/software/llvm-project/build/bin/clang-23+0x4df64e6)
#24 0x0000560d8fd4f2ba clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/usr/local/google/home/bvanassche/software/llvm-project/build/bin/clang-23+0x4d722ba)
#25 0x0000560d8febd082 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/local/google/home/bvanassche/software/llvm-project/build/bin/clang-23+0x4ee0082)
#26 0x0000560d8bf16dcd cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/usr/local/google/home/bvanassche/software/llvm-project/build/bin/clang-23+0xf39dcd)
#27 0x0000560d8bf0de24 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#28 0x0000560d8bf0de6a int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char co\
nst*>&) driver.cpp:0:0
#29 0x0000560d8faff159 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bo\
ol*) const::'lambda'()>(long) Job.cpp:0:0
#30 0x0000560d8f09c2f4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/usr/local/google/home/bvanassche/software/llvm-project/build/bin/clang-23+0x40bf2f4)
#31 0x0000560d8faff810 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#32 0x0000560d8fabf2c8 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/usr/local/google/home/bvanassche/software/llvm-project/build/bin/clang-23+0x4ae22c8)
#33 0x0000560d8fac0202 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/usr/local/google/home/bvanassche/software/llvm-project/b\
uild/bin/clang-23+0x4ae3202)
#34 0x0000560d8fad522c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/usr/local/google/home/bvanassche/software/llvm-project/build/bin/cla\
ng-23+0x4af822c)
#35 0x0000560d8bf12ddd clang_main(int, char**, llvm::ToolContext const&) (/usr/local/google/home/bvanassche/software/llvm-project/build/bin/clang-23+0xf35ddd)
#36 0x0000560d8bda856f main (/usr/local/google/home/bvanassche/software/llvm-project/build/bin/clang-23+0xdcb56f)
#37 0x00007f4d30c33ca8 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#38 0x00007f4d30c33d65 call_init ./csu/../csu/libc-start.c:128:20
#39 0x00007f4d30c33d65 __libc_start_main ./csu/../csu/libc-start.c:347:5
#40 0x0000560d8bf0d861 _start (/usr/local/google/home/bvanassche/software/llvm-project/build/bin/clang-23+0xf30861)
clang: error: clang frontend command failed with exit code 70 (use -v to see invocation)
clang version 23.0.0git (git@github.com:llvm/llvm-project.git d406ce8e20600171b6030c9dcf728a23975c9eaf)
Target: mips-unknown-linux-gnu
Thread model: posix
InstalledDir: /usr/local/google/home/bvanassche/software/llvm-project/build/bin
clang: note: diagnostic msg:
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang: note: diagnostic msg: /tmp/main-ad6244.c
clang: note: diagnostic msg: /tmp/main-ad6244.sh
clang: note: diagnostic msg:
```

## Comments

### Comment 1 - bvanassche

Cc-ing @melver although I don't expect that this is related to thread-safety analysis.

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-backend-mips

Author: Bart Van Assche (bvanassche)

<details>
If I build commit f606cc48f9e482e65c1365ce695c87d309f3affd of https://github.com/bvanassche/linux with Clang commit d406ce8e20600171b6030c9dcf728a23975c9eaf and the attached kernel config, the following output appears:

[kernel-config.txt](https://github.com/user-attachments/files/25309254/kernel-config.txt)
[main-ad6244.c](https://github.com/user-attachments/files/25309255/main-ad6244.c)
[main-ad6244.sh](https://github.com/user-attachments/files/25309253/main-ad6244.sh)

```
cd ~/software/linux-kernel/ &amp;&amp; make ARCH=mips LLVM=1
  SYNC    include/config/auto.conf
  UPD     include/generated/compile.h
  CC      scripts/mod/empty.o
  MKELF   scripts/mod/elfconfig.h
  HOSTCC  scripts/mod/modpost.o
  CC      scripts/mod/devicetable-offsets.s
  HOSTCC  scripts/mod/file2alias.o
  HOSTCC  scripts/mod/sumversion.o
  HOSTCC  scripts/mod/symsearch.o
  HOSTLD  scripts/mod/modpost
  CC      kernel/bounds.s
  CC      arch/mips/kernel/asm-offsets.s
  CC      kernel/sched/rq-offsets.s
  CALL    scripts/checksyscalls.sh
  CC      init/main.o
fatal error: error in backend: Unsupported instruction : &lt;MCInst 0 &lt;MCOperand Reg:346&gt; &lt;MCOperand Reg:252&gt;&gt;
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: clang -Wp,-MMD,init/.main.o.d -nostdinc -I./arch/mips/include -I./arch/mips/include/generated -I./include -I./include -I./arch/mips/include/uapi -I./arch/mips/include/generated/uapi -I./include/uapi -I./inclu\
de/generated/uapi -include ./include/linux/compiler-version.h -include ./include/linux/kconfig.h -include ./include/linux/compiler_types.h -D__KERNEL__ --target=mips[…]
1.      &lt;eof&gt; parser at end of file
2.      Code generation
3.      Running pass 'Function Pass Manager' on module 'init/main.c'.
4.      Running pass 'Mips Assembly Printer' on function '@<!-- -->start_kernel'
 #<!-- -->0 0x0000560d8f16b1d8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/usr/local/google/home/bvanassche/software/llvm-project/build/bin/clang-23+0x418e1d8)
 #<!-- -->1 0x0000560d8f168e54 llvm::sys::CleanupOnSignal(unsigned long) (/usr/local/google/home/bvanassche/software/llvm-project/build/bin/clang-23+0x418be54)
 #<!-- -->2 0x0000560d8f09c3e2 llvm::CrashRecoveryContext::HandleExit(int) (/usr/local/google/home/bvanassche/software/llvm-project/build/bin/clang-23+0x40bf3e2)
 #<!-- -->3 0x0000560d8f16056e llvm::sys::Process::Exit(int, bool) (/usr/local/google/home/bvanassche/software/llvm-project/build/bin/clang-23+0x418356e)
 #<!-- -->4 0x0000560d8bf13e67 LLVMErrorHandler(void*, char const*, bool) cc1_main.cpp:0:0
 #<!-- -->5 0x0000560d8f0a5c99 llvm::report_fatal_error(llvm::Twine const&amp;, bool) (/usr/local/google/home/bvanassche/software/llvm-project/build/bin/clang-23+0x40c8c99)
 #<!-- -->6 0x0000560d8f0a5e19 (/usr/local/google/home/bvanassche/software/llvm-project/build/bin/clang-23+0x40c8e19)
 #<!-- -->7 0x0000560d8edefae6 (/usr/local/google/home/bvanassche/software/llvm-project/build/bin/clang-23+0x3e12ae6)
 #<!-- -->8 0x0000560d8d1cc0d1 llvm::MipsMCCodeEmitter::getBinaryCodeForInstr(llvm::MCInst const&amp;, llvm::SmallVectorImpl&lt;llvm::MCFixup&gt;&amp;, llvm::MCSubtargetInfo const&amp;) const (/usr/local/google/home/bvanassche/software/llvm-project/build/bin/cl\
ang-23+0x21ef0d1)
 #<!-- -->9 0x0000560d8d1d2cc2 llvm::MipsMCCodeEmitter::encodeInstruction(llvm::MCInst const&amp;, llvm::SmallVectorImpl&lt;char&gt;&amp;, llvm::SmallVectorImpl&lt;llvm::MCFixup&gt;&amp;, llvm::MCSubtargetInfo const&amp;) const (/usr/local/google/home/bvanassche/software\
/llvm-project/build/bin/clang-23+0x21f5cc2)
#<!-- -->10 0x0000560d8ee26774 llvm::MCObjectStreamer::emitInstToData(llvm::MCInst const&amp;, llvm::MCSubtargetInfo const&amp;) (/usr/local/google/home/bvanassche/software/llvm-project/build/bin/clang-23+0x3e49774)
#<!-- -->11 0x0000560d8d1c78c5 llvm::MipsELFStreamer::emitInstruction(llvm::MCInst const&amp;, llvm::MCSubtargetInfo const&amp;) (/usr/local/google/home/bvanassche/software/llvm-project/build/bin/clang-23+0x21ea8c5)
#<!-- -->12 0x0000560d8d093fc4 llvm::MipsAsmPrinter::emitInstruction(llvm::MachineInstr const*) (/usr/local/google/home/bvanassche/software/llvm-project/build/bin/clang-23+0x20b6fc4)
#<!-- -->13 0x0000560d90094224 llvm::AsmPrinter::emitFunctionBody() (/usr/local/google/home/bvanassche/software/llvm-project/build/bin/clang-23+0x50b7224)
#<!-- -->14 0x0000560d8d08fc6b llvm::MipsAsmPrinter::runOnMachineFunction(llvm::MachineFunction&amp;) (/usr/local/google/home/bvanassche/software/llvm-project/build/bin/clang-23+0x20b2c6b)
#<!-- -->15 0x0000560d8e4471fb llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (/usr/local/google/home/bvanassche/software/llvm-project/build/bin/clang-23+0x346a1fb)
#<!-- -->16 0x0000560d8ea2e8c1 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (/usr/local/google/home/bvanassche/software/llvm-project/build/bin/clang-23+0x3a518c1)
#<!-- -->17 0x0000560d8ea2eb63 llvm::FPPassManager::runOnModule(llvm::Module&amp;) (/usr/local/google/home/bvanassche/software/llvm-project/build/bin/clang-23+0x3a51b63)
#<!-- -->18 0x0000560d8ea2ddb1 llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/usr/local/google/home/bvanassche/software/llvm-project/build/bin/clang-23+0x3a50db1)
#<!-- -->19 0x0000560d8f433569 (anonymous namespace)::EmitAssemblyHelper::RunCodegenPipeline(clang::BackendAction, std::unique_ptr&lt;llvm::raw_pwrite_stream, std::default_delete&lt;llvm::raw_pwrite_stream&gt;&gt;&amp;, std::unique_ptr&lt;llvm::ToolOutputFile, s\
td::default_delete&lt;llvm::ToolOutputFile&gt;&gt;&amp;) BackendUtil.cpp:0:0
#<!-- -->20 0x0000560d8f43414f clang::emitBackendOutput(clang::CompilerInstance&amp;, clang::CodeGenOptions&amp;, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;, std::unique_ptr&lt;llvm::raw_pwrite_s\
tream, std::default_delete&lt;llvm::raw_pwrite_stream&gt;&gt;, clang::BackendConsumer*) (/usr/local/google/home/bvanassche/software/llvm-project/build/bin/clang-23+0x445714f)
#<!-- -->21 0x0000560d8fa7deb0 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&amp;) (/usr/local/google/home/bvanassche/software/llvm-project/build/bin/clang-23+0x4aa0eb0)
#<!-- -->22 0x0000560d9156927c clang::ParseAST(clang::Sema&amp;, bool, bool) (/usr/local/google/home/bvanassche/software/llvm-project/build/bin/clang-23+0x658c27c)
#<!-- -->23 0x0000560d8fdd34e6 clang::FrontendAction::Execute() (/usr/local/google/home/bvanassche/software/llvm-project/build/bin/clang-23+0x4df64e6)
#<!-- -->24 0x0000560d8fd4f2ba clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/usr/local/google/home/bvanassche/software/llvm-project/build/bin/clang-23+0x4d722ba)
#<!-- -->25 0x0000560d8febd082 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/local/google/home/bvanassche/software/llvm-project/build/bin/clang-23+0x4ee0082)
#<!-- -->26 0x0000560d8bf16dcd cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/usr/local/google/home/bvanassche/software/llvm-project/build/bin/clang-23+0xf39dcd)
#<!-- -->27 0x0000560d8bf0de24 ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->28 0x0000560d8bf0de6a int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char co\
nst*&gt;&amp;) driver.cpp:0:0
#<!-- -->29 0x0000560d8faff159 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bo\
ol*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->30 0x0000560d8f09c2f4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/usr/local/google/home/bvanassche/software/llvm-project/build/bin/clang-23+0x40bf2f4)
#<!-- -->31 0x0000560d8faff810 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->32 0x0000560d8fabf2c8 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/usr/local/google/home/bvanassche/software/llvm-project/build/bin/clang-23+0x4ae22c8)
#<!-- -->33 0x0000560d8fac0202 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/usr/local/google/home/bvanassche/software/llvm-project/b\
uild/bin/clang-23+0x4ae3202)
#<!-- -->34 0x0000560d8fad522c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/usr/local/google/home/bvanassche/software/llvm-project/build/bin/cla\
ng-23+0x4af822c)
#<!-- -->35 0x0000560d8bf12ddd clang_main(int, char**, llvm::ToolContext const&amp;) (/usr/local/google/home/bvanassche/software/llvm-project/build/bin/clang-23+0xf35ddd)
#<!-- -->36 0x0000560d8bda856f main (/usr/local/google/home/bvanassche/software/llvm-project/build/bin/clang-23+0xdcb56f)
#<!-- -->37 0x00007f4d30c33ca8 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->38 0x00007f4d30c33d65 call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->39 0x00007f4d30c33d65 __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->40 0x0000560d8bf0d861 _start (/usr/local/google/home/bvanassche/software/llvm-project/build/bin/clang-23+0xf30861)
clang: error: clang frontend command failed with exit code 70 (use -v to see invocation)
clang version 23.0.0git (git@<!-- -->github.com:llvm/llvm-project.git d406ce8e20600171b6030c9dcf728a23975c9eaf)
Target: mips-unknown-linux-gnu
Thread model: posix
InstalledDir: /usr/local/google/home/bvanassche/software/llvm-project/build/bin
clang: note: diagnostic msg:
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang: note: diagnostic msg: /tmp/main-ad6244.c
clang: note: diagnostic msg: /tmp/main-ad6244.sh
clang: note: diagnostic msg:
```
</details>


---

### Comment 3 - brad0

cc @yingopq 

---

### Comment 4 - djtodoro

@bvanassche can you please try to reduce the test case for this issue? Thanks!

---

### Comment 5 - bvanassche

A reproducer is already available. With the attachments I provided I can reproduce the unsupported instruction error as follows:
```
$ mkdir -p scripts/basic
$ touch ./scripts/basic/randstruct.seed
$ chmod a+x main-ad6244.sh
$ ./main-ad6244.sh 
fatal error: error in backend: Unsupported instruction : <MCInst 0 <MCOperand Reg:346> <MCOperand Reg:252>>
```

---

### Comment 6 - djtodoro

I see that, but is there a change to reduce it? e.g. to run `creduce` on it?

---

