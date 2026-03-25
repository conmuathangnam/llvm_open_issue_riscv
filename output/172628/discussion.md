# [RISC-V] Clang crashes when running pass 'Post-RA pseudo instruction expansion pass' on function '@_Z1dv'

**Author:** ams-x9v2b7kq
**URL:** https://github.com/llvm/llvm-project/issues/172628
**Status:** Closed
**Labels:** backend:RISC-V, crash
**Closed Date:** 2025-12-18T06:45:48Z

## Body

## [RISC-V] Clang crashes when running pass 'Post-RA pseudo instruction expansion pass' on function '@_Z1dv'

### code link: https://godbolt.org/z/8rEznGKf4
```
#include <riscv_vector.h>
uint8_t a[1]; int32_t b[1]; int32_t c[1];
void d() {
  for (size_t e = 0;;) {
    size_t f(5);
    vuint8mf8_t g = __riscv_vle8_v_u8mf8(&a[e], f);
    vbool64_t i = __riscv_vmseq_vx_u8mf8_b64(g, 1, f);
    vint32mf2x5_t j = __riscv_vlseg5e32_v_i32mf2x5_mu(
        i,
        __riscv_vcreate_v_i32mf2x5(
            __riscv_vmv_v_x_i32mf2(0, __riscv_vsetvlmax_e16mf4()),
            __riscv_vmv_v_x_i32mf2(0, __riscv_vsetvlmax_e16mf4()),
            __riscv_vmv_v_x_i32mf2(0, __riscv_vsetvlmax_e16mf4()),
            __riscv_vmv_v_x_i32mf2(0, __riscv_vsetvlmax_e16mf4()),
            __riscv_vmv_v_x_i32mf2(0, __riscv_vsetvlmax_e16mf4())),
        &b[e], f);
    vint32mf2_t k = __riscv_vget_v_i32mf2x5_i32mf2(j, 0);
    vuint32mf2_t l = __riscv_vsll_vx_u32mf2(__riscv_vid_v_u32mf2(f), 2, f);
    __riscv_vsuxei32(&c[e], l, k, f);
  }
}
```

```
$ clang++ -O3 -march=rv64gcv_zvfh -mabi=lp64d test.cpp -o a.out
```

### crash backtrace
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
```
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -g -o /app/output.s -fno-verbose-asm -S -target riscv64-unknown-linux-gnu -march=rv64gc -mabi=lp64d --gcc-toolchain=/opt/compiler-explorer/riscv64/gcc-14.2.0/riscv64-unknown-linux-gnu --sysroot=/opt/compiler-explorer/riscv64/gcc-14.2.0/riscv64-unknown-linux-gnu/riscv64-unknown-linux-gnu/sysroot -fcolor-diagnostics -fno-crash-diagnostics -O3 -march=rv64gcv_zvfh <source>
1.	<eof> parser at end of file
2.	Code generation
3.	Running pass 'Function Pass Manager' on module '<source>'.
4.	Running pass 'Post-RA pseudo instruction expansion pass' on function '@_Z1dv'
 #0 0x0000000003d10978 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3d10978)
 #1 0x0000000003d0e34c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3d0e34c)
 #2 0x0000000003c552d8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007fda72042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00000000021fcfbd llvm::RISCVInstrInfo::copyPhysRegVector(llvm::MachineBasicBlock&, llvm::MachineInstrBundleIterator<llvm::MachineInstr, false>, llvm::DebugLoc const&, llvm::MCRegister, llvm::MCRegister, bool, llvm::TargetRegisterClass const*) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x21fcfbd)
 #5 0x00000000021fe86a llvm::RISCVInstrInfo::copyPhysReg(llvm::MachineBasicBlock&, llvm::MachineInstrBundleIterator<llvm::MachineInstr, false>, llvm::DebugLoc const&, llvm::Register, llvm::Register, bool, bool, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x21fe86a)
 #6 0x0000000003385e86 llvm::TargetInstrInfo::lowerCopy(llvm::MachineInstr*, llvm::TargetRegisterInfo const*) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3385e86)
 #7 0x0000000002ff1988 (anonymous namespace)::ExpandPostRA::run(llvm::MachineFunction&) (.constprop.0) ExpandPostRAPseudos.cpp:0:0
 #8 0x0000000002ff1ba4 (anonymous namespace)::ExpandPostRALegacy::runOnMachineFunction(llvm::MachineFunction&) ExpandPostRAPseudos.cpp:0:0
 #9 0x000000000310bbcd llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x310bbcd)
#10 0x000000000366dfc2 llvm::FPPassManager::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x366dfc2)
#11 0x000000000366e251 llvm::FPPassManager::runOnModule(llvm::Module&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x366e251)
#12 0x000000000366fac7 llvm::legacy::PassManagerImpl::run(llvm::Module&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x366fac7)
#13 0x0000000003fa69bb clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3fa69bb)
#14 0x00000000045d9b2b clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45d9b2b)
#15 0x00000000062235bc clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62235bc)
#16 0x00000000045da685 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45da685)
#17 0x00000000048ea59a clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x48ea59a)
#18 0x00000000048684eb clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x48684eb)
#19 0x00000000049e071b clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x49e071b)
#20 0x0000000000dccd85 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdccd85)
#21 0x0000000000dc4b7b ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#22 0x0000000000dc4c1d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#23 0x0000000004654e19 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#24 0x0000000003c556f3 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c556f3)
#25 0x0000000004655039 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#26 0x0000000004617ae2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4617ae2)
#27 0x00000000046189c1 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x46189c1)
#28 0x00000000046214ac clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x46214ac)
#29 0x0000000000dc9649 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdc9649)
#30 0x0000000000c7ac34 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc7ac34)
#31 0x00007fda72029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#32 0x00007fda72029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#33 0x0000000000dc4615 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdc4615)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```

```
$ clang-22 --version
clang version 22.0.0git (https://github.com/llvm/llvm-project.git d502ff094937ff9e532a675df11d55bb0b134066)
Target: riscv64-unknown-unknown-elf
Thread model: posix
InstalledDir: /home/2025-compiler/gcc-16-20251214/bin
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-risc-v

Author: None (ams-x9v2b7kq)

<details>
## [RISC-V] Clang crashes when running pass 'Post-RA pseudo instruction expansion pass' on function '@<!-- -->_Z1dv'

### code link: https://godbolt.org/z/8rEznGKf4
```
#include &lt;riscv_vector.h&gt;
uint8_t a[1]; int32_t b[1]; int32_t c[1];
void d() {
  for (size_t e = 0;;) {
    size_t f(5);
    vuint8mf8_t g = __riscv_vle8_v_u8mf8(&amp;a[e], f);
    vbool64_t i = __riscv_vmseq_vx_u8mf8_b64(g, 1, f);
    vint32mf2x5_t j = __riscv_vlseg5e32_v_i32mf2x5_mu(
        i,
        __riscv_vcreate_v_i32mf2x5(
            __riscv_vmv_v_x_i32mf2(0, __riscv_vsetvlmax_e16mf4()),
            __riscv_vmv_v_x_i32mf2(0, __riscv_vsetvlmax_e16mf4()),
            __riscv_vmv_v_x_i32mf2(0, __riscv_vsetvlmax_e16mf4()),
            __riscv_vmv_v_x_i32mf2(0, __riscv_vsetvlmax_e16mf4()),
            __riscv_vmv_v_x_i32mf2(0, __riscv_vsetvlmax_e16mf4())),
        &amp;b[e], f);
    vint32mf2_t k = __riscv_vget_v_i32mf2x5_i32mf2(j, 0);
    vuint32mf2_t l = __riscv_vsll_vx_u32mf2(__riscv_vid_v_u32mf2(f), 2, f);
    __riscv_vsuxei32(&amp;c[e], l, k, f);
  }
}
```

```
$ clang++ -O3 -march=rv64gcv_zvfh -mabi=lp64d test.cpp -o a.out
```

### crash backtrace
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
```
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -g -o /app/output.s -fno-verbose-asm -S -target riscv64-unknown-linux-gnu -march=rv64gc -mabi=lp64d --gcc-toolchain=/opt/compiler-explorer/riscv64/gcc-14.2.0/riscv64-unknown-linux-gnu --sysroot=/opt/compiler-explorer/riscv64/gcc-14.2.0/riscv64-unknown-linux-gnu/riscv64-unknown-linux-gnu/sysroot -fcolor-diagnostics -fno-crash-diagnostics -O3 -march=rv64gcv_zvfh &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	Code generation
3.	Running pass 'Function Pass Manager' on module '&lt;source&gt;'.
4.	Running pass 'Post-RA pseudo instruction expansion pass' on function '@<!-- -->_Z1dv'
 #<!-- -->0 0x0000000003d10978 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3d10978)
 #<!-- -->1 0x0000000003d0e34c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3d0e34c)
 #<!-- -->2 0x0000000003c552d8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007fda72042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00000000021fcfbd llvm::RISCVInstrInfo::copyPhysRegVector(llvm::MachineBasicBlock&amp;, llvm::MachineInstrBundleIterator&lt;llvm::MachineInstr, false&gt;, llvm::DebugLoc const&amp;, llvm::MCRegister, llvm::MCRegister, bool, llvm::TargetRegisterClass const*) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x21fcfbd)
 #<!-- -->5 0x00000000021fe86a llvm::RISCVInstrInfo::copyPhysReg(llvm::MachineBasicBlock&amp;, llvm::MachineInstrBundleIterator&lt;llvm::MachineInstr, false&gt;, llvm::DebugLoc const&amp;, llvm::Register, llvm::Register, bool, bool, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x21fe86a)
 #<!-- -->6 0x0000000003385e86 llvm::TargetInstrInfo::lowerCopy(llvm::MachineInstr*, llvm::TargetRegisterInfo const*) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3385e86)
 #<!-- -->7 0x0000000002ff1988 (anonymous namespace)::ExpandPostRA::run(llvm::MachineFunction&amp;) (.constprop.0) ExpandPostRAPseudos.cpp:0:0
 #<!-- -->8 0x0000000002ff1ba4 (anonymous namespace)::ExpandPostRALegacy::runOnMachineFunction(llvm::MachineFunction&amp;) ExpandPostRAPseudos.cpp:0:0
 #<!-- -->9 0x000000000310bbcd llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x310bbcd)
#<!-- -->10 0x000000000366dfc2 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x366dfc2)
#<!-- -->11 0x000000000366e251 llvm::FPPassManager::runOnModule(llvm::Module&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x366e251)
#<!-- -->12 0x000000000366fac7 llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x366fac7)
#<!-- -->13 0x0000000003fa69bb clang::emitBackendOutput(clang::CompilerInstance&amp;, clang::CodeGenOptions&amp;, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;, std::unique_ptr&lt;llvm::raw_pwrite_stream, std::default_delete&lt;llvm::raw_pwrite_stream&gt;&gt;, clang::BackendConsumer*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3fa69bb)
#<!-- -->14 0x00000000045d9b2b clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45d9b2b)
#<!-- -->15 0x00000000062235bc clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62235bc)
#<!-- -->16 0x00000000045da685 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45da685)
#<!-- -->17 0x00000000048ea59a clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x48ea59a)
#<!-- -->18 0x00000000048684eb clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x48684eb)
#<!-- -->19 0x00000000049e071b clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x49e071b)
#<!-- -->20 0x0000000000dccd85 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdccd85)
#<!-- -->21 0x0000000000dc4b7b ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->22 0x0000000000dc4c1d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->23 0x0000000004654e19 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->24 0x0000000003c556f3 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c556f3)
#<!-- -->25 0x0000000004655039 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->26 0x0000000004617ae2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4617ae2)
#<!-- -->27 0x00000000046189c1 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x46189c1)
#<!-- -->28 0x00000000046214ac clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x46214ac)
#<!-- -->29 0x0000000000dc9649 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdc9649)
#<!-- -->30 0x0000000000c7ac34 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc7ac34)
#<!-- -->31 0x00007fda72029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->32 0x00007fda72029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->33 0x0000000000dc4615 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdc4615)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```

```
$ clang-22 --version
clang version 22.0.0git (https://github.com/llvm/llvm-project.git d502ff094937ff9e532a675df11d55bb0b134066)
Target: riscv64-unknown-unknown-elf
Thread model: posix
InstalledDir: /home/2025-compiler/gcc-16-20251214/bin
```
</details>


---

