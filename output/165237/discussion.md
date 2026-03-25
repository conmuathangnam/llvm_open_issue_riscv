# [SPIRV][OPENCL]"Assertion `Args.size() >= Call->Builtin->MinNumArgs && "Too few arguments to generate the builtin"' failed."

**Author:** FinalEvilution
**URL:** https://github.com/llvm/llvm-project/issues/165237
**Status:** Closed
**Labels:** OpenCL, crash, backend:SPIR-V
**Closed Date:** 2026-03-04T09:41:18Z

## Body

llc crashes with "Assertion `Args.size() >= Call->Builtin->MinNumArgs && "Too few arguments to generate the builtin"' failed." when building a spirv ir file with a declare and call of "void @barrier()" using llvm llc versions 19.1.7 to current main, but not 18.1.8.
Some  additional history on how this bug was discovered is at  [https://github.com/ldc-developers/ldc/issues/4998#issuecomment-3449545048](url)

<details>
<summary> Bugpoint output </summary>

```
Read input file      : 'kernels_ocl220_64.spv'
*** All input ok
Initializing execution environment: Found llc: /mnt/RAID1/Home/GitHub/llvm-project/llvm-build/bin/llc
Running the code generator to test for a crash: <llc>
Error running tool:
  /mnt/RAID1/Home/GitHub/llvm-project/llvm-build/bin/llc -o bugpoint-test-program-264785d.bc-49899e9.llc.s bugpoint-test-program-264785d.bc
llc: /mnt/RAID1/Home/GitHub/llvm-project/llvm/lib/Target/SPIRV/SPIRVBuiltins.cpp:2975: std::optional<bool> llvm::SPIRV::lowerBuiltin(llvm::StringRef, InstructionSet::InstructionSet, llvm::MachineIRBuilder&, llvm::Register, const llvm::Type*, const llvm::SmallVectorImpl<llvm::Register>&, llvm::SPIRVGlobalRegistry*, const llvm::CallBase&): Assertion `Args.size() >= Call->Builtin->MinNumArgs && "Too few arguments to generate the builtin"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /mnt/RAID1/Home/GitHub/llvm-project/llvm-build/bin/llc -o bugpoint-test-program-264785d.bc-49899e9.llc.s bugpoint-test-program-264785d.bc
1.	Running pass 'Function Pass Manager' on module 'bugpoint-test-program-264785d.bc'.
2.	Running pass 'IRTranslator' on function '@_D4testQfFZv'
 #0 0x00000000032aba56 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /mnt/RAID1/Home/GitHub/llvm-project/llvm/lib/Support/Unix/Signals.inc:834:22
 #1 0x00000000032abf26 PrintStackTraceSignalHandler(void*) /mnt/RAID1/Home/GitHub/llvm-project/llvm/lib/Support/Unix/Signals.inc:916:1
 #2 0x00000000032a9419 llvm::sys::RunSignalHandlers() /mnt/RAID1/Home/GitHub/llvm-project/llvm/lib/Support/Signals.cpp:104:20
 #3 0x00000000032ab38a SignalHandler(int, siginfo_t*, void*) /mnt/RAID1/Home/GitHub/llvm-project/llvm/lib/Support/Unix/Signals.inc:426:14
 #4 0x00007f2963247c00 __restore_rt libc_sigaction.c:0:0
 #5 0x00007f29632ac061 __pthread_kill_implementation /usr/src/debug/sys-libs/glibc-2.42-r1/glibc-2.42/nptl/pthread_kill.c:44:76
 #6 0x00007f29632ac061 __pthread_kill_internal /usr/src/debug/sys-libs/glibc-2.42-r1/glibc-2.42/nptl/pthread_kill.c:89:10
 #7 0x00007f29632ac061 pthread_kill@@GLIBC_2.34 /usr/src/debug/sys-libs/glibc-2.42-r1/glibc-2.42/nptl/pthread_kill.c:100:10
 #8 0x00007f2963247b06 gsignal /usr/src/debug/sys-libs/glibc-2.42-r1/glibc-2.42/signal/../sysdeps/posix/raise.c:27:6
 #9 0x00007f29632292ef internal_signal_block_all /usr/src/debug/sys-libs/glibc-2.42-r1/glibc-2.42/stdlib/../sysdeps/unix/sysv/linux/internal-signals.h:81:3
#10 0x00007f29632292ef __abort_lock_wrlock /usr/src/debug/sys-libs/glibc-2.42-r1/glibc-2.42/stdlib/abort.c:62:3
#11 0x00007f29632292ef abort /usr/src/debug/sys-libs/glibc-2.42-r1/glibc-2.42/stdlib/abort.c:85:3
#12 0x00007f296322927e __assert_perror_fail /usr/src/debug/sys-libs/glibc-2.42-r1/glibc-2.42/assert/assert-perr.c:31:1
#13 0x0000000001259721 llvm::SPIRV::lowerBuiltin(llvm::StringRef, llvm::SPIRV::InstructionSet::InstructionSet, llvm::MachineIRBuilder&, llvm::Register, llvm::Type const*, llvm::SmallVectorImpl<llvm::Register> const&, llvm::SPIRVGlobalRegistry*, llvm::CallBase const&) /mnt/RAID1/Home/GitHub/llvm-project/llvm/lib/Target/SPIRV/SPIRVBuiltins.cpp:2977:11
#14 0x0000000001127322 llvm::SPIRVCallLowering::lowerCall(llvm::MachineIRBuilder&, llvm::CallLowering::CallLoweringInfo&) const /mnt/RAID1/Home/GitHub/llvm-project/llvm/lib/Target/SPIRV/SPIRVCallLowering.cpp:634:39
#15 0x00000000039d7c99 llvm::CallLowering::lowerCall(llvm::MachineIRBuilder&, llvm::CallBase const&, llvm::ArrayRef<llvm::Register>, llvm::ArrayRef<llvm::ArrayRef<llvm::Register>>, llvm::Register, std::optional<llvm::CallLowering::PtrAuthInfo>, llvm::Register, std::function<llvm::Register ()>) const /mnt/RAID1/Home/GitHub/llvm-project/llvm/lib/CodeGen/GlobalISel/CallLowering.cpp:208:7
#16 0x0000000003a6e588 llvm::IRTranslator::translateCallBase(llvm::CallBase const&, llvm::MachineIRBuilder&) /mnt/RAID1/Home/GitHub/llvm-project/llvm/lib/CodeGen/GlobalISel/IRTranslator.cpp:2767:32
#17 0x0000000003a6e897 llvm::IRTranslator::translateCall(llvm::User const&, llvm::MachineIRBuilder&) /mnt/RAID1/Home/GitHub/llvm-project/llvm/lib/CodeGen/GlobalISel/IRTranslator.cpp:2808:5
#18 0x0000000003a74b29 llvm::IRTranslator::translate(llvm::Instruction const&) /mnt/RAID1/Home/GitHub/llvm-project/llvm/include/llvm/IR/Instruction.def:210:1
#19 0x0000000003a790dc llvm::IRTranslator::runOnMachineFunction(llvm::MachineFunction&) /mnt/RAID1/Home/GitHub/llvm-project/llvm/lib/CodeGen/GlobalISel/IRTranslator.cpp:4221:9
#20 0x0000000001baf125 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) /mnt/RAID1/Home/GitHub/llvm-project/llvm/lib/CodeGen/MachineFunctionPass.cpp:108:30
#21 0x000000000248f0d9 llvm::FPPassManager::runOnFunction(llvm::Function&) /mnt/RAID1/Home/GitHub/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:1398:20
#22 0x000000000248f33f llvm::FPPassManager::runOnModule(llvm::Module&) /mnt/RAID1/Home/GitHub/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:1444:13
#23 0x000000000248f756 (anonymous namespace)::MPPassManager::runOnModule(llvm::Module&) /mnt/RAID1/Home/GitHub/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:1513:20
#24 0x000000000248ae50 llvm::legacy::PassManagerImpl::run(llvm::Module&) /mnt/RAID1/Home/GitHub/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:531:13
#25 0x000000000248ff97 llvm::legacy::PassManager::run(llvm::Module&) /mnt/RAID1/Home/GitHub/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:1641:1
#26 0x0000000000a25277 compileModule(char**, llvm::LLVMContext&) /mnt/RAID1/Home/GitHub/llvm-project/llvm/tools/llc/llc.cpp:833:34
#27 0x0000000000a22948 main /mnt/RAID1/Home/GitHub/llvm-project/llvm/tools/llc/llc.cpp:446:35
#28 0x00007f296322b43a __libc_start_call_main /usr/src/debug/sys-libs/glibc-2.42-r1/glibc-2.42/csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#29 0x00007f296322b4ea call_init /usr/src/debug/sys-libs/glibc-2.42-r1/glibc-2.42/csu/../csu/libc-start.c:128:20
#30 0x00007f296322b4ea __libc_start_main@GLIBC_2.2.5 /usr/src/debug/sys-libs/glibc-2.42-r1/glibc-2.42/csu/../csu/libc-start.c:347:5
#31 0x0000000000a212e5 _start /usr/src/debug/sys-libs/glibc-2.42-r1/glibc-2.42/csu/../sysdeps/x86_64/start.S:117:0
*** Debugging code generator crash!

*** Attempting to reduce the number of function attributes in the testcase
Checking for crash with changing conditionals to always jump to true: : <llc><crash>
Emitted bitcode to 'bugpoint-reduced-conditionals.bc'
Checking for crash with only these blocks: : <llc><crash>
Emitted bitcode to 'bugpoint-reduced-blocks.bc'
Checking for crash with CFG simplifying: : <llc><crash>
Emitted bitcode to 'bugpoint-reduced-simplifycfg.bc'
Checking for crash with only 1 instruction: <llc><crash>

*** Attempting to reduce testcase by deleting instructions: Simplification Level #1
Checking instruction:   call spir_func void @barrier()<llc>

*** Attempting to reduce testcase by deleting instructions: Simplification Level #0
Checking instruction:   call spir_func void @barrier()<llc>
Checking for crash with metadata retained from 2 instructions: <llc><crash>
Checking for crash with metadata retained from 1 instruction: <llc><crash>
Emitted bitcode to 'bugpoint-reduced-instructions.bc'

*** Attempting to strip the debug info: <llc><crash>

*** Attempting to strip the debug type info: <llc><crash>

*** Attempting to remove named metadata: Checking for crash with only these named metadata nodes: opencl.spir.version opencl.ocl.version: <llc><crash>
Checking for crash with only these named metadata nodes: opencl.ocl.version: <llc><crash>
Checking for crash with only 1 named metadata operand: <llc><crash>
Emitted bitcode to 'bugpoint-reduced-named-md.bc'

*** Attempting to perform final cleanups: <llc><crash>
Emitted bitcode to 'bugpoint-reduced-simplified.bc'
```

</details>

<details>
<summary> bugpoint-reduced-simplified.ll </summary>

```
; ModuleID = 'bugpoint-reduced-simplified.bc'
source_filename = "dcomputeTargetOCL"
target datalayout = "e-i64:64-v16:16-v24:32-v32:32-v48:64-v96:128-v192:256-v256:256-v512:512-v1024:1024-G1"
target triple = "spirv64-unknown-unknown"

define spir_func void @_D4testQfFZv() {
  call spir_func void @barrier()
  ret void
}

declare spir_func void @barrier()

!opencl.ocl.version = !{!0}

!0 = !{i32 2, i32 2}
```

</details>


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-opencl

Author: None (FinalEvilution)

<details>
llc crashes with "Assertion `Args.size() &gt;= Call-&gt;Builtin-&gt;MinNumArgs &amp;&amp; "Too few arguments to generate the builtin"' failed." when building a spirv ir file with a declare and call of "void @<!-- -->barrier()" using llvm llc versions 19.1.7 to current main, but not 18.1.8.
This issue was first discovered when debugging a crash in the ldc dlang compiler building opencl spirv, a reduced test case can be found at [https://github.com/ldc-developers/ldc/issues/4998#issuecomment-3449545048](url)

&lt;details&gt;
&lt;summary&gt; Bugpoint output &lt;/summary&gt;

```
Read input file      : 'kernels_ocl220_64.spv'
*** All input ok
Initializing execution environment: Found llc: /mnt/RAID1/Home/GitHub/llvm-project/llvm-build/bin/llc
Running the code generator to test for a crash: &lt;llc&gt;
Error running tool:
  /mnt/RAID1/Home/GitHub/llvm-project/llvm-build/bin/llc -o bugpoint-test-program-264785d.bc-49899e9.llc.s bugpoint-test-program-264785d.bc
llc: /mnt/RAID1/Home/GitHub/llvm-project/llvm/lib/Target/SPIRV/SPIRVBuiltins.cpp:2975: std::optional&lt;bool&gt; llvm::SPIRV::lowerBuiltin(llvm::StringRef, InstructionSet::InstructionSet, llvm::MachineIRBuilder&amp;, llvm::Register, const llvm::Type*, const llvm::SmallVectorImpl&lt;llvm::Register&gt;&amp;, llvm::SPIRVGlobalRegistry*, const llvm::CallBase&amp;): Assertion `Args.size() &gt;= Call-&gt;Builtin-&gt;MinNumArgs &amp;&amp; "Too few arguments to generate the builtin"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /mnt/RAID1/Home/GitHub/llvm-project/llvm-build/bin/llc -o bugpoint-test-program-264785d.bc-49899e9.llc.s bugpoint-test-program-264785d.bc
1.	Running pass 'Function Pass Manager' on module 'bugpoint-test-program-264785d.bc'.
2.	Running pass 'IRTranslator' on function '@<!-- -->_D4testQfFZv'
 #<!-- -->0 0x00000000032aba56 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /mnt/RAID1/Home/GitHub/llvm-project/llvm/lib/Support/Unix/Signals.inc:834:22
 #<!-- -->1 0x00000000032abf26 PrintStackTraceSignalHandler(void*) /mnt/RAID1/Home/GitHub/llvm-project/llvm/lib/Support/Unix/Signals.inc:916:1
 #<!-- -->2 0x00000000032a9419 llvm::sys::RunSignalHandlers() /mnt/RAID1/Home/GitHub/llvm-project/llvm/lib/Support/Signals.cpp:104:20
 #<!-- -->3 0x00000000032ab38a SignalHandler(int, siginfo_t*, void*) /mnt/RAID1/Home/GitHub/llvm-project/llvm/lib/Support/Unix/Signals.inc:426:14
 #<!-- -->4 0x00007f2963247c00 __restore_rt libc_sigaction.c:0:0
 #<!-- -->5 0x00007f29632ac061 __pthread_kill_implementation /usr/src/debug/sys-libs/glibc-2.42-r1/glibc-2.42/nptl/pthread_kill.c:44:76
 #<!-- -->6 0x00007f29632ac061 __pthread_kill_internal /usr/src/debug/sys-libs/glibc-2.42-r1/glibc-2.42/nptl/pthread_kill.c:89:10
 #<!-- -->7 0x00007f29632ac061 pthread_kill@@<!-- -->GLIBC_2.34 /usr/src/debug/sys-libs/glibc-2.42-r1/glibc-2.42/nptl/pthread_kill.c:100:10
 #<!-- -->8 0x00007f2963247b06 gsignal /usr/src/debug/sys-libs/glibc-2.42-r1/glibc-2.42/signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x00007f29632292ef internal_signal_block_all /usr/src/debug/sys-libs/glibc-2.42-r1/glibc-2.42/stdlib/../sysdeps/unix/sysv/linux/internal-signals.h:81:3
#<!-- -->10 0x00007f29632292ef __abort_lock_wrlock /usr/src/debug/sys-libs/glibc-2.42-r1/glibc-2.42/stdlib/abort.c:62:3
#<!-- -->11 0x00007f29632292ef abort /usr/src/debug/sys-libs/glibc-2.42-r1/glibc-2.42/stdlib/abort.c:85:3
#<!-- -->12 0x00007f296322927e __assert_perror_fail /usr/src/debug/sys-libs/glibc-2.42-r1/glibc-2.42/assert/assert-perr.c:31:1
#<!-- -->13 0x0000000001259721 llvm::SPIRV::lowerBuiltin(llvm::StringRef, llvm::SPIRV::InstructionSet::InstructionSet, llvm::MachineIRBuilder&amp;, llvm::Register, llvm::Type const*, llvm::SmallVectorImpl&lt;llvm::Register&gt; const&amp;, llvm::SPIRVGlobalRegistry*, llvm::CallBase const&amp;) /mnt/RAID1/Home/GitHub/llvm-project/llvm/lib/Target/SPIRV/SPIRVBuiltins.cpp:2977:11
#<!-- -->14 0x0000000001127322 llvm::SPIRVCallLowering::lowerCall(llvm::MachineIRBuilder&amp;, llvm::CallLowering::CallLoweringInfo&amp;) const /mnt/RAID1/Home/GitHub/llvm-project/llvm/lib/Target/SPIRV/SPIRVCallLowering.cpp:634:39
#<!-- -->15 0x00000000039d7c99 llvm::CallLowering::lowerCall(llvm::MachineIRBuilder&amp;, llvm::CallBase const&amp;, llvm::ArrayRef&lt;llvm::Register&gt;, llvm::ArrayRef&lt;llvm::ArrayRef&lt;llvm::Register&gt;&gt;, llvm::Register, std::optional&lt;llvm::CallLowering::PtrAuthInfo&gt;, llvm::Register, std::function&lt;llvm::Register ()&gt;) const /mnt/RAID1/Home/GitHub/llvm-project/llvm/lib/CodeGen/GlobalISel/CallLowering.cpp:208:7
#<!-- -->16 0x0000000003a6e588 llvm::IRTranslator::translateCallBase(llvm::CallBase const&amp;, llvm::MachineIRBuilder&amp;) /mnt/RAID1/Home/GitHub/llvm-project/llvm/lib/CodeGen/GlobalISel/IRTranslator.cpp:2767:32
#<!-- -->17 0x0000000003a6e897 llvm::IRTranslator::translateCall(llvm::User const&amp;, llvm::MachineIRBuilder&amp;) /mnt/RAID1/Home/GitHub/llvm-project/llvm/lib/CodeGen/GlobalISel/IRTranslator.cpp:2808:5
#<!-- -->18 0x0000000003a74b29 llvm::IRTranslator::translate(llvm::Instruction const&amp;) /mnt/RAID1/Home/GitHub/llvm-project/llvm/include/llvm/IR/Instruction.def:210:1
#<!-- -->19 0x0000000003a790dc llvm::IRTranslator::runOnMachineFunction(llvm::MachineFunction&amp;) /mnt/RAID1/Home/GitHub/llvm-project/llvm/lib/CodeGen/GlobalISel/IRTranslator.cpp:4221:9
#<!-- -->20 0x0000000001baf125 llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) /mnt/RAID1/Home/GitHub/llvm-project/llvm/lib/CodeGen/MachineFunctionPass.cpp:108:30
#<!-- -->21 0x000000000248f0d9 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) /mnt/RAID1/Home/GitHub/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:1398:20
#<!-- -->22 0x000000000248f33f llvm::FPPassManager::runOnModule(llvm::Module&amp;) /mnt/RAID1/Home/GitHub/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:1444:13
#<!-- -->23 0x000000000248f756 (anonymous namespace)::MPPassManager::runOnModule(llvm::Module&amp;) /mnt/RAID1/Home/GitHub/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:1513:20
#<!-- -->24 0x000000000248ae50 llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) /mnt/RAID1/Home/GitHub/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:531:13
#<!-- -->25 0x000000000248ff97 llvm::legacy::PassManager::run(llvm::Module&amp;) /mnt/RAID1/Home/GitHub/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:1641:1
#<!-- -->26 0x0000000000a25277 compileModule(char**, llvm::LLVMContext&amp;) /mnt/RAID1/Home/GitHub/llvm-project/llvm/tools/llc/llc.cpp:833:34
#<!-- -->27 0x0000000000a22948 main /mnt/RAID1/Home/GitHub/llvm-project/llvm/tools/llc/llc.cpp:446:35
#<!-- -->28 0x00007f296322b43a __libc_start_call_main /usr/src/debug/sys-libs/glibc-2.42-r1/glibc-2.42/csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->29 0x00007f296322b4ea call_init /usr/src/debug/sys-libs/glibc-2.42-r1/glibc-2.42/csu/../csu/libc-start.c:128:20
#<!-- -->30 0x00007f296322b4ea __libc_start_main@<!-- -->GLIBC_2.2.5 /usr/src/debug/sys-libs/glibc-2.42-r1/glibc-2.42/csu/../csu/libc-start.c:347:5
#<!-- -->31 0x0000000000a212e5 _start /usr/src/debug/sys-libs/glibc-2.42-r1/glibc-2.42/csu/../sysdeps/x86_64/start.S:117:0
*** Debugging code generator crash!

*** Attempting to reduce the number of function attributes in the testcase
Checking for crash with changing conditionals to always jump to true: : &lt;llc&gt;&lt;crash&gt;
Emitted bitcode to 'bugpoint-reduced-conditionals.bc'
Checking for crash with only these blocks: : &lt;llc&gt;&lt;crash&gt;
Emitted bitcode to 'bugpoint-reduced-blocks.bc'
Checking for crash with CFG simplifying: : &lt;llc&gt;&lt;crash&gt;
Emitted bitcode to 'bugpoint-reduced-simplifycfg.bc'
Checking for crash with only 1 instruction: &lt;llc&gt;&lt;crash&gt;

*** Attempting to reduce testcase by deleting instructions: Simplification Level #<!-- -->1
Checking instruction:   call spir_func void @<!-- -->barrier()&lt;llc&gt;

*** Attempting to reduce testcase by deleting instructions: Simplification Level #<!-- -->0
Checking instruction:   call spir_func void @<!-- -->barrier()&lt;llc&gt;
Checking for crash with metadata retained from 2 instructions: &lt;llc&gt;&lt;crash&gt;
Checking for crash with metadata retained from 1 instruction: &lt;llc&gt;&lt;crash&gt;
Emitted bitcode to 'bugpoint-reduced-instructions.bc'

*** Attempting to strip the debug info: &lt;llc&gt;&lt;crash&gt;

*** Attempting to strip the debug type info: &lt;llc&gt;&lt;crash&gt;

*** Attempting to remove named metadata: Checking for crash with only these named metadata nodes: opencl.spir.version opencl.ocl.version: &lt;llc&gt;&lt;crash&gt;
Checking for crash with only these named metadata nodes: opencl.ocl.version: &lt;llc&gt;&lt;crash&gt;
Checking for crash with only 1 named metadata operand: &lt;llc&gt;&lt;crash&gt;
Emitted bitcode to 'bugpoint-reduced-named-md.bc'

*** Attempting to perform final cleanups: &lt;llc&gt;&lt;crash&gt;
Emitted bitcode to 'bugpoint-reduced-simplified.bc'
```

&lt;/details&gt;

&lt;details&gt;
&lt;summary&gt; bugpoint-reduced-simplified.ll &lt;/summary&gt;

```
; ModuleID = 'bugpoint-reduced-simplified.bc'
source_filename = "dcomputeTargetOCL"
target datalayout = "e-i64:64-v16:16-v24:32-v32:32-v48:64-v96:128-v192:256-v256:256-v512:512-v1024:1024-G1"
target triple = "spirv64-unknown-unknown"

define spir_func void @<!-- -->_D4testQfFZv() {
  call spir_func void @<!-- -->barrier()
  ret void
}

declare spir_func void @<!-- -->barrier()

!opencl.ocl.version = !{!0}

!0 = !{i32 2, i32 2}
```

&lt;/details&gt;

</details>


---

### Comment 2 - thewilsonator

This is an issue with LDC not with LLVM

---

### Comment 3 - FinalEvilution

@thewilsonator This issue is not about LDC. It's for a crash in the LLVM system compiler (llc) when compiling the sample LLVM IR. (bugpoint-reduced-simplified.ll) I included the link to the LDC issue for historical context on how this bug was discovered not because it's dependent on LDC, although i admit my phrasing was poorly thought out, I'll edit my post.
This bug actually can be reproduced using clang. See the example below.

clang -cc1 -triple spirv64 -emit-llvm-bc clang_kernel.cl
/usr/lib/llvm/19/bin/llc clang_kernel.bc #Crashes llc with llvm >= 19 but not 18.
```
void barrier();

kernel void Test()
{
    barrier();
}
```

---

### Comment 4 - thewilsonator

Does 
```c
void barrier(int);

kernel void Test()
{
    barrier(0);
}
```
work?

---

### Comment 5 - FinalEvilution

It compiles with both 18 and 19 but only inserts an OpControlBarrier for llvm 19.
Interestingly enough i get the same results for long, short, unsigned int, bool and char. while float and double crashes llc 19 but compiles fine with 18.
I actually can't find a combination that will insert a barrier for llvm 18, but if i commit out the declaration and compile with "clang --target=spirv64 clang_kernel.cl" it produces a spirv file with a barrier.

---

### Comment 6 - FinalEvilution

Using an IA64 mangling of "void _Z7barrierj();" i can reproduce both OpControlBarrier generation and the crash with llvm 18.
It seems that support for lowering non-mangled calls to barrier was not added till llvm 19 and this explains the discrepancy between llvm 18 and 19.

It turns out that other builtins can be made to crash by changing the number of parameters, some examples below.
```
void _Z15get_global_sizej();
void _Z4acosf();
void _Z8half_cosf();
void _Z3absf();
void _Z5mad24iii();
void _Z5clampfff();
```
You can even strip the types off the mangling like so "void _Z5clamp();" and it will still lower it.

My theory is that llvm searches for function calls matching a builtin and if found, lowers it, without checking if the number and/or types of parameters are valid first.


---

