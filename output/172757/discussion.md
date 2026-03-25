# Failed to compile when specifying `-O3 -mllvm -pre-RA-sched=fast`

**Author:** werifu
**URL:** https://github.com/llvm/llvm-project/issues/172757
**Status:** Closed
**Labels:** duplicate, backend:X86, crash
**Closed Date:** 2025-12-18T00:30:50Z

## Body

A quick reproduction:
https://godbolt.org/z/8zjr51svb

code:
```
#include <stdint.h>

void bug(uint64_t z[4], uint64_t x[2], uint64_t y) {
    uint64_t a, b, c, d, t, carry;
    __uint128_t tmp;
    // mul1: x[0] * y -> b:a
    tmp = ((__uint128_t)x[0]) * ((__uint128_t)y);
    b = (uint64_t)(tmp >> 64);
    a = (uint64_t)tmp;
    
    // mul2: x[1] * y -> c:t
    tmp = ((__uint128_t)x[1]) * ((__uint128_t)y);
    c = (uint64_t)(tmp >> 64);
    t = (uint64_t)tmp;
    
    // Add b + t with carry
    tmp = ((__uint128_t)b) + ((__uint128_t)t);
    b = (uint64_t)tmp;
    carry = (uint64_t)(tmp >> 64);
    
    // ADC: c + t + carry (reuse t)
    tmp = ((__uint128_t)c) + ((__uint128_t)t) + ((__uint128_t)carry);
    carry = (uint64_t)(tmp >> 64);
    
    // ADC: d + t + carry (uninitialized d, reuse t again)
    tmp = ((__uint128_t)d) + ((__uint128_t)t) + ((__uint128_t)carry);
    d = (uint64_t)tmp;
    
    z[0] = a;
    z[1] = b;
    z[2] = c;
    z[3] = d;
}

```
Run `clang -mllvm -pre-RA-sched=fast -O3 simple_repro.c -S` and get the following result

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -O3 -mllvm -pre-RA-sched=fast <source> -isystem/opt/compiler-explorer/libs/google-benchmark/v1.4.0/include
1.	<eof> parser at end of file
2.	Code generation
3.	Running pass 'Function Pass Manager' on module '<source>'.
4.	Running pass 'X86 EFLAGS copy lowering' on function '@_Z3bugPmS_m'
 #0 0x0000000003d10978 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3d10978)
 #1 0x0000000003d0e34c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3d0e34c)
 #2 0x0000000003c552d8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007f669f242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00000000027dbb37 (anonymous namespace)::X86FlagsCopyLoweringPass::runOnMachineFunction(llvm::MachineFunction&) (.part.0) X86FlagsCopyLowering.cpp:0:0
 #5 0x00000000027de6ee (anonymous namespace)::X86FlagsCopyLoweringPass::runOnMachineFunction(llvm::MachineFunction&) X86FlagsCopyLowering.cpp:0:0
 #6 0x000000000310bbcd llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x310bbcd)
 #7 0x000000000366dfc2 llvm::FPPassManager::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x366dfc2)
 #8 0x000000000366e251 llvm::FPPassManager::runOnModule(llvm::Module&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x366e251)
 #9 0x000000000366fac7 llvm::legacy::PassManagerImpl::run(llvm::Module&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x366fac7)
#10 0x0000000003fa69bb clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3fa69bb)
#11 0x00000000045d9b2b clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45d9b2b)
#12 0x00000000062235bc clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62235bc)
#13 0x00000000045da685 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45da685)
#14 0x00000000048ea59a clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x48ea59a)
#15 0x00000000048684eb clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x48684eb)
#16 0x00000000049e071b clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x49e071b)
#17 0x0000000000dccd85 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdccd85)
#18 0x0000000000dc4b7b ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#19 0x0000000000dc4c1d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#20 0x0000000004654e19 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#21 0x0000000003c556f3 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c556f3)
#22 0x0000000004655039 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#23 0x0000000004617ae2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4617ae2)
#24 0x00000000046189c1 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x46189c1)
#25 0x00000000046214ac clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x46214ac)
#26 0x0000000000dc9649 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdc9649)
#27 0x0000000000c7ac34 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc7ac34)
#28 0x00007f669f229d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#29 0x00007f669f229e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#30 0x0000000000dc4615 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdc4615)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```

Preprocessed source: 
https://pastebin.com/7P1Sgsuc
Associated run script:
```
# Crash reproducer for Ubuntu clang version 18.1.3 (1ubuntu1)
# Driver args: "-mllvm" "-pre-RA-sched=fast" "-O3" "-mbmi2" "simple_repro.c" "-S"
# Original command:  "/usr/lib/llvm-18/bin/clang" "-cc1" "-triple" "x86_64-pc-linux-gnu" "-S" "-disable-free" "-clear-ast-before-backend" "-disable-llvm-verifier" "-discard-value-names" "-main-file-name" "simple_repro.c" "-mrelocation-model" "pic" "-pic-level" "2" "-pic-is-pie" "-mframe-pointer=none" "-fmath-errno" "-ffp-contract=on" "-fno-rounding-math" "-mconstructor-aliases" "-funwind-tables=2" "-target-cpu" "x86-64" "-target-feature" "+bmi2" "-tune-cpu" "generic" "-debugger-tuning=gdb" "-fdebug-compilation-dir=/home/chen/mask4crypto/mul-p25519-msk" "-fcoverage-compilation-dir=/home/chen/mask4crypto/mul-p25519-msk" "-resource-dir" "/usr/lib/llvm-18/lib/clang/18" "-internal-isystem" "/usr/lib/llvm-18/lib/clang/18/include" "-internal-isystem" "/usr/local/include" "-internal-isystem" "/usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../x86_64-linux-gnu/include" "-internal-externc-isystem" "/usr/include/x86_64-linux-gnu" "-internal-externc-isystem" "/include" "-internal-externc-isystem" "/usr/include" "-O3" "-ferror-limit" "19" "-fgnuc-version=4.2.1" "-fskip-odr-check-in-gmf" "-fcolor-diagnostics" "-vectorize-loops" "-vectorize-slp" "-mllvm" "-pre-RA-sched=fast" "-faddrsig" "-D__GCC_HAVE_DWARF2_CFI_ASM=1" "-o" "simple_repro.s" "-x" "c" "simple_repro.c"
 "/usr/lib/llvm-18/bin/clang" "-cc1" "-triple" "x86_64-pc-linux-gnu" "-S" "-disable-free" "-clear-ast-before-backend" "-disable-llvm-verifier" "-discard-value-names" "-main-file-name" "simple_repro.c" "-mrelocation-model" "pic" "-pic-level" "2" "-pic-is-pie" "-mframe-pointer=none" "-fmath-errno" "-ffp-contract=on" "-fno-rounding-math" "-mconstructor-aliases" "-funwind-tables=2" "-target-cpu" "x86-64" "-target-feature" "+bmi2" "-tune-cpu" "generic" "-debugger-tuning=gdb" "-fdebug-compilation-dir=/home/chen/mask4crypto/mul-p25519-msk" "-fcoverage-compilation-dir=/home/chen/mask4crypto/mul-p25519-msk" "-O3" "-ferror-limit" "19" "-fgnuc-version=4.2.1" "-fskip-odr-check-in-gmf" "-fcolor-diagnostics" "-vectorize-loops" "-vectorize-slp" "-mllvm" "-pre-RA-sched=fast" "-faddrsig" "-D__GCC_HAVE_DWARF2_CFI_ASM=1" "-x" "c" "simple_repro-7d00da.c"
```

System: WSL2 ubuntu on x64 Windows11
```
Ubuntu clang version 18.1.3 (1ubuntu1)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
```


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-x86

Author: Chen (werifu)

<details>
A quick reproduction:
https://godbolt.org/z/8zjr51svb

code:
```
#include &lt;stdint.h&gt;

void bug(uint64_t z[4], uint64_t x[2], uint64_t y) {
    uint64_t a, b, c, d, t, carry;
    __uint128_t tmp;
    // mul1: x[0] * y -&gt; b:a
    tmp = ((__uint128_t)x[0]) * ((__uint128_t)y);
    b = (uint64_t)(tmp &gt;&gt; 64);
    a = (uint64_t)tmp;
    
    // mul2: x[1] * y -&gt; c:t
    tmp = ((__uint128_t)x[1]) * ((__uint128_t)y);
    c = (uint64_t)(tmp &gt;&gt; 64);
    t = (uint64_t)tmp;
    
    // Add b + t with carry
    tmp = ((__uint128_t)b) + ((__uint128_t)t);
    b = (uint64_t)tmp;
    carry = (uint64_t)(tmp &gt;&gt; 64);
    
    // ADC: c + t + carry (reuse t)
    tmp = ((__uint128_t)c) + ((__uint128_t)t) + ((__uint128_t)carry);
    carry = (uint64_t)(tmp &gt;&gt; 64);
    
    // ADC: d + t + carry (uninitialized d, reuse t again)
    tmp = ((__uint128_t)d) + ((__uint128_t)t) + ((__uint128_t)carry);
    d = (uint64_t)tmp;
    
    z[0] = a;
    z[1] = b;
    z[2] = c;
    z[3] = d;
}

```
Run `clang -mllvm -pre-RA-sched=fast -O3 simple_repro.c -S` and get the following result

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -O3 -mllvm -pre-RA-sched=fast &lt;source&gt; -isystem/opt/compiler-explorer/libs/google-benchmark/v1.4.0/include
1.	&lt;eof&gt; parser at end of file
2.	Code generation
3.	Running pass 'Function Pass Manager' on module '&lt;source&gt;'.
4.	Running pass 'X86 EFLAGS copy lowering' on function '@<!-- -->_Z3bugPmS_m'
 #<!-- -->0 0x0000000003d10978 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3d10978)
 #<!-- -->1 0x0000000003d0e34c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3d0e34c)
 #<!-- -->2 0x0000000003c552d8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007f669f242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00000000027dbb37 (anonymous namespace)::X86FlagsCopyLoweringPass::runOnMachineFunction(llvm::MachineFunction&amp;) (.part.0) X86FlagsCopyLowering.cpp:0:0
 #<!-- -->5 0x00000000027de6ee (anonymous namespace)::X86FlagsCopyLoweringPass::runOnMachineFunction(llvm::MachineFunction&amp;) X86FlagsCopyLowering.cpp:0:0
 #<!-- -->6 0x000000000310bbcd llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x310bbcd)
 #<!-- -->7 0x000000000366dfc2 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x366dfc2)
 #<!-- -->8 0x000000000366e251 llvm::FPPassManager::runOnModule(llvm::Module&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x366e251)
 #<!-- -->9 0x000000000366fac7 llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x366fac7)
#<!-- -->10 0x0000000003fa69bb clang::emitBackendOutput(clang::CompilerInstance&amp;, clang::CodeGenOptions&amp;, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;, std::unique_ptr&lt;llvm::raw_pwrite_stream, std::default_delete&lt;llvm::raw_pwrite_stream&gt;&gt;, clang::BackendConsumer*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3fa69bb)
#<!-- -->11 0x00000000045d9b2b clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45d9b2b)
#<!-- -->12 0x00000000062235bc clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62235bc)
#<!-- -->13 0x00000000045da685 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45da685)
#<!-- -->14 0x00000000048ea59a clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x48ea59a)
#<!-- -->15 0x00000000048684eb clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x48684eb)
#<!-- -->16 0x00000000049e071b clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x49e071b)
#<!-- -->17 0x0000000000dccd85 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdccd85)
#<!-- -->18 0x0000000000dc4b7b ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->19 0x0000000000dc4c1d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->20 0x0000000004654e19 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->21 0x0000000003c556f3 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c556f3)
#<!-- -->22 0x0000000004655039 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->23 0x0000000004617ae2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4617ae2)
#<!-- -->24 0x00000000046189c1 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x46189c1)
#<!-- -->25 0x00000000046214ac clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x46214ac)
#<!-- -->26 0x0000000000dc9649 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdc9649)
#<!-- -->27 0x0000000000c7ac34 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc7ac34)
#<!-- -->28 0x00007f669f229d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->29 0x00007f669f229e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->30 0x0000000000dc4615 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdc4615)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```

Preprocessed source: 
https://pastebin.com/7P1Sgsuc
Associated run script:
```
# Crash reproducer for Ubuntu clang version 18.1.3 (1ubuntu1)
# Driver args: "-mllvm" "-pre-RA-sched=fast" "-O3" "-mbmi2" "simple_repro.c" "-S"
# Original command:  "/usr/lib/llvm-18/bin/clang" "-cc1" "-triple" "x86_64-pc-linux-gnu" "-S" "-disable-free" "-clear-ast-before-backend" "-disable-llvm-verifier" "-discard-value-names" "-main-file-name" "simple_repro.c" "-mrelocation-model" "pic" "-pic-level" "2" "-pic-is-pie" "-mframe-pointer=none" "-fmath-errno" "-ffp-contract=on" "-fno-rounding-math" "-mconstructor-aliases" "-funwind-tables=2" "-target-cpu" "x86-64" "-target-feature" "+bmi2" "-tune-cpu" "generic" "-debugger-tuning=gdb" "-fdebug-compilation-dir=/home/chen/mask4crypto/mul-p25519-msk" "-fcoverage-compilation-dir=/home/chen/mask4crypto/mul-p25519-msk" "-resource-dir" "/usr/lib/llvm-18/lib/clang/18" "-internal-isystem" "/usr/lib/llvm-18/lib/clang/18/include" "-internal-isystem" "/usr/local/include" "-internal-isystem" "/usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../x86_64-linux-gnu/include" "-internal-externc-isystem" "/usr/include/x86_64-linux-gnu" "-internal-externc-isystem" "/include" "-internal-externc-isystem" "/usr/include" "-O3" "-ferror-limit" "19" "-fgnuc-version=4.2.1" "-fskip-odr-check-in-gmf" "-fcolor-diagnostics" "-vectorize-loops" "-vectorize-slp" "-mllvm" "-pre-RA-sched=fast" "-faddrsig" "-D__GCC_HAVE_DWARF2_CFI_ASM=1" "-o" "simple_repro.s" "-x" "c" "simple_repro.c"
 "/usr/lib/llvm-18/bin/clang" "-cc1" "-triple" "x86_64-pc-linux-gnu" "-S" "-disable-free" "-clear-ast-before-backend" "-disable-llvm-verifier" "-discard-value-names" "-main-file-name" "simple_repro.c" "-mrelocation-model" "pic" "-pic-level" "2" "-pic-is-pie" "-mframe-pointer=none" "-fmath-errno" "-ffp-contract=on" "-fno-rounding-math" "-mconstructor-aliases" "-funwind-tables=2" "-target-cpu" "x86-64" "-target-feature" "+bmi2" "-tune-cpu" "generic" "-debugger-tuning=gdb" "-fdebug-compilation-dir=/home/chen/mask4crypto/mul-p25519-msk" "-fcoverage-compilation-dir=/home/chen/mask4crypto/mul-p25519-msk" "-O3" "-ferror-limit" "19" "-fgnuc-version=4.2.1" "-fskip-odr-check-in-gmf" "-fcolor-diagnostics" "-vectorize-loops" "-vectorize-slp" "-mllvm" "-pre-RA-sched=fast" "-faddrsig" "-D__GCC_HAVE_DWARF2_CFI_ASM=1" "-x" "c" "simple_repro-7d00da.c"
```

System: WSL2 ubuntu on x64 Windows11
```
Ubuntu clang version 18.1.3 (1ubuntu1)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
```

</details>


---

