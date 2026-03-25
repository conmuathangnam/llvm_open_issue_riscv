# Assertion `(!Node || !ResNo || ResNo < Node->getNumValues()) && "Invalid result number for the given node!"' failed.

**Author:** ye-luo
**URL:** https://github.com/llvm/llvm-project/issues/167746
**Status:** Closed
**Labels:** backend:X86, crash, llvm:SelectionDAG
**Closed Date:** 2025-11-14T19:21:10Z

## Body

reproducer attached.
[repro.zip](https://github.com/user-attachments/files/23508174/repro.zip)

```
clang-22: /scratch3/opt/llvm-clang/llvm-project-nightly/llvm/include/llvm/CodeGen/SelectionDAGNodes.h:1262: llvm::SDValue::SDValue(llvm::SDNode*, unsigned int): Assertion `(!Node || !ResNo || ResNo < Node->getNumValues()) && "Invalid result number for the given node!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /scratch3/packages/llvm/main-20251112/bin/clang-22 -cc1 -triple x86_64-unknown-linux-gnu -O3 -emit-obj -disable-free -clear-ast-before-backend -main-file-name DiracDeterminantBatched.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=none -menable-no-infs -menable-no-nans -fapprox-func -funsafe-math-optimizations -fno-signed-zeros -mreassociate -freciprocal-math -ffp-contract=fast -fno-rounding-math -ffast-math -ffinite-math-only -complex-range=basic -mconstructor-aliases -funwind-tables=2 -target-cpu skylake-avx512 -target-feature +prfchw -target-feature -cldemote -target-feature +avx -target-feature +aes -target-feature +sahf -target-feature +pclmul -target-feature -xop -target-feature +crc32 -target-feature -amx-fp8 -target-feature +xsaves -target-feature -avx512fp16 -target-feature -usermsr -target-feature -sm4 -target-feature -egpr -target-feature +sse4.1 -target-feature -avx10.1 -target-feature -avx512ifma -target-feature +xsave -target-feature +sse4.2 -target-feature -tsxldtrk -target-feature -sm3 -target-feature -ptwrite -target-feature -widekl -target-feature -movrs -target-feature +invpcid -target-feature +64bit -target-feature +xsavec -target-feature -avx512vpopcntdq -target-feature +cmov -target-feature -avx512vp2intersect -target-feature +avx512cd -target-feature +movbe -target-feature -avxvnniint8 -target-feature -ccmp -target-feature -amx-int8 -target-feature -kl -target-feature -sha512 -target-feature -avxvnni -target-feature +rtm -target-feature +adx -target-feature +avx2 -target-feature -hreset -target-feature -movdiri -target-feature -serialize -target-feature -vpclmulqdq -target-feature +avx512vl -target-feature -uintr -target-feature -cf -target-feature +clflushopt -target-feature -raoint -target-feature -cmpccxadd -target-feature +bmi -target-feature -amx-tile -target-feature +sse -target-feature -gfni -target-feature -avxvnniint16 -target-feature -amx-fp16 -target-feature -zu -target-feature -ndd -target-feature +xsaveopt -target-feature +rdrnd -target-feature +avx512f -target-feature -amx-bf16 -target-feature -avx512bf16 -target-feature -avx512vnni -target-feature -push2pop2 -target-feature +cx8 -target-feature +avx512bw -target-feature +sse3 -target-feature +pku -target-feature -nf -target-feature -amx-tf32 -target-feature -amx-avx512 -target-feature +fsgsbase -target-feature -clzero -target-feature -mwaitx -target-feature -lwp -target-feature +lzcnt -target-feature -sha -target-feature -movdir64b -target-feature -ppx -target-feature -wbnoinvd -target-feature -enqcmd -target-feature -avxneconvert -target-feature -tbm -target-feature -pconfig -target-feature -amx-complex -target-feature +ssse3 -target-feature +cx16 -target-feature -avx10.2 -target-feature +bmi2 -target-feature +fma -target-feature +popcnt -target-feature -avxifma -target-feature +f16c -target-feature -avx512bitalg -target-feature -rdpru -target-feature +clwb -target-feature +mmx -target-feature +sse2 -target-feature +rdseed -target-feature -avx512vbmi2 -target-feature -prefetchi -target-feature -amx-movrs -target-feature -rdpid -target-feature -fma4 -target-feature -avx512vbmi -target-feature -shstk -target-feature -vaes -target-feature -waitpkg -target-feature -sgx -target-feature +fxsr -target-feature +avx512dq -target-feature -sse4a -debugger-tuning=gdb -fdebug-compilation-dir=/scratch3/opt/qmcpack/build_llvmNightly_real_nompi/src/QMCWaveFunctions -fcoverage-compilation-dir=/scratch3/opt/qmcpack/build_llvmNightly_real_nompi/src/QMCWaveFunctions -sys-header-deps -D ADD_ -D H5_USE_110_API -D HAVE_CONFIG_H -D HAVE_MKL -D HAVE_MKL_VML -D OPENMP_NO_COMPLEX -D restrict=__restrict__ -D NDEBUG -Wvla -Wall -Wno-unused-variable -Wno-overloaded-virtual -Wno-unused-private-field -Wno-unused-local-typedef -Wsuggest-override -Wno-unknown-pragmas -Wmisleading-indentation -std=c++17 -fdeprecated-macro -ferror-limit 19 -fmessage-length=156 -fopenmp -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -vectorize-loops -vectorize-slp -faddrsig -fdwarf2-cfi-asm -x c++ DiracDeterminantBatched-3363c2.cpp
1.	<eof> parser at end of file
2.	Code generation
3.	Running pass 'Function Pass Manager' on module 'DiracDeterminantBatched-3363c2.cpp'.
4.	Running pass 'X86 DAG->DAG Instruction Selection' on function '@_ZN11qmcplusplus23DiracDeterminantBatchedILNS_12PlatformKindE1EddE10acceptMoveERNS_11ParticleSetEib'
 #0 0x0000560d6fdb35c0 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/scratch3/packages/llvm/main-20251112/bin/clang-22+0x2e165c0)
 #1 0x0000560d6fdb027f llvm::sys::RunSignalHandlers() (/scratch3/packages/llvm/main-20251112/bin/clang-22+0x2e1327f)
 #2 0x0000560d6fdb03d2 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x00007faa7b4c3520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007faa7b5179fc __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x00007faa7b5179fc __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x00007faa7b5179fc pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x00007faa7b4c3476 gsignal ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x00007faa7b4a97f3 abort ./stdlib/abort.c:81:7
 #9 0x00007faa7b4a971b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#10 0x00007faa7b4bae96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
#11 0x0000560d7122bf6b llvm::SelectionDAGBuilder::visitExtractValue(llvm::ExtractValueInst const&) (/scratch3/packages/llvm/main-20251112/bin/clang-22+0x428ef6b)
#12 0x0000560d7125015a llvm::SelectionDAGBuilder::visit(llvm::Instruction const&) (/scratch3/packages/llvm/main-20251112/bin/clang-22+0x42b315a)
#13 0x0000560d712e8ca2 llvm::SelectionDAGISel::SelectBasicBlock(llvm::ilist_iterator_w_bits<llvm::ilist_detail::node_options<llvm::Instruction, true, false, void, true, llvm::BasicBlock>, false, true>, llvm::ilist_iterator_w_bits<llvm::ilist_detail::node_options<llvm::Instruction, true, false, void, true, llvm::BasicBlock>, false, true>, bool&) (/scratch3/packages/llvm/main-20251112/bin/clang-22+0x434bca2)
#14 0x0000560d712e961d llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&) (/scratch3/packages/llvm/main-20251112/bin/clang-22+0x434c61d)
#15 0x0000560d712eaf5c llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&) (/scratch3/packages/llvm/main-20251112/bin/clang-22+0x434df5c)
#16 0x0000560d712d68d9 llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&) (/scratch3/packages/llvm/main-20251112/bin/clang-22+0x43398d9)
#17 0x0000560d6efe4d4b llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (.part.0) MachineFunctionPass.cpp:0:0
#18 0x0000560d6f681913 llvm::FPPassManager::runOnFunction(llvm::Function&) (/scratch3/packages/llvm/main-20251112/bin/clang-22+0x26e4913)
#19 0x0000560d6f681d69 llvm::FPPassManager::runOnModule(llvm::Module&) (/scratch3/packages/llvm/main-20251112/bin/clang-22+0x26e4d69)
#20 0x0000560d6f6826b7 llvm::legacy::PassManagerImpl::run(llvm::Module&) (/scratch3/packages/llvm/main-20251112/bin/clang-22+0x26e56b7)
#21 0x0000560d700635e3 clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/scratch3/packages/llvm/main-20251112/bin/clang-22+0x30c65e3)
#22 0x0000560d706efc82 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/scratch3/packages/llvm/main-20251112/bin/clang-22+0x3752c82)
#23 0x0000560d7220a60c clang::ParseAST(clang::Sema&, bool, bool) (/scratch3/packages/llvm/main-20251112/bin/clang-22+0x526d60c)
#24 0x0000560d70a1bc09 clang::FrontendAction::Execute() (/scratch3/packages/llvm/main-20251112/bin/clang-22+0x3a7ec09)
#25 0x0000560d7099b6f5 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/scratch3/packages/llvm/main-20251112/bin/clang-22+0x39fe6f5)
#26 0x0000560d70b11143 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/scratch3/packages/llvm/main-20251112/bin/clang-22+0x3b74143)
#27 0x0000560d6ddef287 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/scratch3/packages/llvm/main-20251112/bin/clang-22+0xe52287)
#28 0x0000560d6dde597a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#29 0x0000560d6dde9c78 clang_main(int, char**, llvm::ToolContext const&) (/scratch3/packages/llvm/main-20251112/bin/clang-22+0xe4cc78)
#30 0x0000560d6dcf4adb main (/scratch3/packages/llvm/main-20251112/bin/clang-22+0xd57adb)
#31 0x00007faa7b4aad90 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:58:16
#32 0x00007faa7b4aae40 call_init ./csu/../csu/libc-start.c:128:20
#33 0x00007faa7b4aae40 __libc_start_main ./csu/../csu/libc-start.c:379:5
#34 0x0000560d6dde5095 _start (/scratch3/packages/llvm/main-20251112/bin/clang-22+0xe48095)
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-x86

Author: Ye Luo (ye-luo)

<details>
reproducer attached.
[repro.zip](https://github.com/user-attachments/files/23508174/repro.zip)

```
clang-22: /scratch3/opt/llvm-clang/llvm-project-nightly/llvm/include/llvm/CodeGen/SelectionDAGNodes.h:1262: llvm::SDValue::SDValue(llvm::SDNode*, unsigned int): Assertion `(!Node || !ResNo || ResNo &lt; Node-&gt;getNumValues()) &amp;&amp; "Invalid result number for the given node!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /scratch3/packages/llvm/main-20251112/bin/clang-22 -cc1 -triple x86_64-unknown-linux-gnu -O3 -emit-obj -disable-free -clear-ast-before-backend -main-file-name DiracDeterminantBatched.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=none -menable-no-infs -menable-no-nans -fapprox-func -funsafe-math-optimizations -fno-signed-zeros -mreassociate -freciprocal-math -ffp-contract=fast -fno-rounding-math -ffast-math -ffinite-math-only -complex-range=basic -mconstructor-aliases -funwind-tables=2 -target-cpu skylake-avx512 -target-feature +prfchw -target-feature -cldemote -target-feature +avx -target-feature +aes -target-feature +sahf -target-feature +pclmul -target-feature -xop -target-feature +crc32 -target-feature -amx-fp8 -target-feature +xsaves -target-feature -avx512fp16 -target-feature -usermsr -target-feature -sm4 -target-feature -egpr -target-feature +sse4.1 -target-feature -avx10.1 -target-feature -avx512ifma -target-feature +xsave -target-feature +sse4.2 -target-feature -tsxldtrk -target-feature -sm3 -target-feature -ptwrite -target-feature -widekl -target-feature -movrs -target-feature +invpcid -target-feature +64bit -target-feature +xsavec -target-feature -avx512vpopcntdq -target-feature +cmov -target-feature -avx512vp2intersect -target-feature +avx512cd -target-feature +movbe -target-feature -avxvnniint8 -target-feature -ccmp -target-feature -amx-int8 -target-feature -kl -target-feature -sha512 -target-feature -avxvnni -target-feature +rtm -target-feature +adx -target-feature +avx2 -target-feature -hreset -target-feature -movdiri -target-feature -serialize -target-feature -vpclmulqdq -target-feature +avx512vl -target-feature -uintr -target-feature -cf -target-feature +clflushopt -target-feature -raoint -target-feature -cmpccxadd -target-feature +bmi -target-feature -amx-tile -target-feature +sse -target-feature -gfni -target-feature -avxvnniint16 -target-feature -amx-fp16 -target-feature -zu -target-feature -ndd -target-feature +xsaveopt -target-feature +rdrnd -target-feature +avx512f -target-feature -amx-bf16 -target-feature -avx512bf16 -target-feature -avx512vnni -target-feature -push2pop2 -target-feature +cx8 -target-feature +avx512bw -target-feature +sse3 -target-feature +pku -target-feature -nf -target-feature -amx-tf32 -target-feature -amx-avx512 -target-feature +fsgsbase -target-feature -clzero -target-feature -mwaitx -target-feature -lwp -target-feature +lzcnt -target-feature -sha -target-feature -movdir64b -target-feature -ppx -target-feature -wbnoinvd -target-feature -enqcmd -target-feature -avxneconvert -target-feature -tbm -target-feature -pconfig -target-feature -amx-complex -target-feature +ssse3 -target-feature +cx16 -target-feature -avx10.2 -target-feature +bmi2 -target-feature +fma -target-feature +popcnt -target-feature -avxifma -target-feature +f16c -target-feature -avx512bitalg -target-feature -rdpru -target-feature +clwb -target-feature +mmx -target-feature +sse2 -target-feature +rdseed -target-feature -avx512vbmi2 -target-feature -prefetchi -target-feature -amx-movrs -target-feature -rdpid -target-feature -fma4 -target-feature -avx512vbmi -target-feature -shstk -target-feature -vaes -target-feature -waitpkg -target-feature -sgx -target-feature +fxsr -target-feature +avx512dq -target-feature -sse4a -debugger-tuning=gdb -fdebug-compilation-dir=/scratch3/opt/qmcpack/build_llvmNightly_real_nompi/src/QMCWaveFunctions -fcoverage-compilation-dir=/scratch3/opt/qmcpack/build_llvmNightly_real_nompi/src/QMCWaveFunctions -sys-header-deps -D ADD_ -D H5_USE_110_API -D HAVE_CONFIG_H -D HAVE_MKL -D HAVE_MKL_VML -D OPENMP_NO_COMPLEX -D restrict=__restrict__ -D NDEBUG -Wvla -Wall -Wno-unused-variable -Wno-overloaded-virtual -Wno-unused-private-field -Wno-unused-local-typedef -Wsuggest-override -Wno-unknown-pragmas -Wmisleading-indentation -std=c++17 -fdeprecated-macro -ferror-limit 19 -fmessage-length=156 -fopenmp -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -vectorize-loops -vectorize-slp -faddrsig -fdwarf2-cfi-asm -x c++ DiracDeterminantBatched-3363c2.cpp
1.	&lt;eof&gt; parser at end of file
2.	Code generation
3.	Running pass 'Function Pass Manager' on module 'DiracDeterminantBatched-3363c2.cpp'.
4.	Running pass 'X86 DAG-&gt;DAG Instruction Selection' on function '@<!-- -->_ZN11qmcplusplus23DiracDeterminantBatchedILNS_12PlatformKindE1EddE10acceptMoveERNS_11ParticleSetEib'
 #<!-- -->0 0x0000560d6fdb35c0 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/scratch3/packages/llvm/main-20251112/bin/clang-22+0x2e165c0)
 #<!-- -->1 0x0000560d6fdb027f llvm::sys::RunSignalHandlers() (/scratch3/packages/llvm/main-20251112/bin/clang-22+0x2e1327f)
 #<!-- -->2 0x0000560d6fdb03d2 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x00007faa7b4c3520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007faa7b5179fc __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->5 0x00007faa7b5179fc __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->6 0x00007faa7b5179fc pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->7 0x00007faa7b4c3476 gsignal ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->8 0x00007faa7b4a97f3 abort ./stdlib/abort.c:81:7
 #<!-- -->9 0x00007faa7b4a971b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->10 0x00007faa7b4bae96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
#<!-- -->11 0x0000560d7122bf6b llvm::SelectionDAGBuilder::visitExtractValue(llvm::ExtractValueInst const&amp;) (/scratch3/packages/llvm/main-20251112/bin/clang-22+0x428ef6b)
#<!-- -->12 0x0000560d7125015a llvm::SelectionDAGBuilder::visit(llvm::Instruction const&amp;) (/scratch3/packages/llvm/main-20251112/bin/clang-22+0x42b315a)
#<!-- -->13 0x0000560d712e8ca2 llvm::SelectionDAGISel::SelectBasicBlock(llvm::ilist_iterator_w_bits&lt;llvm::ilist_detail::node_options&lt;llvm::Instruction, true, false, void, true, llvm::BasicBlock&gt;, false, true&gt;, llvm::ilist_iterator_w_bits&lt;llvm::ilist_detail::node_options&lt;llvm::Instruction, true, false, void, true, llvm::BasicBlock&gt;, false, true&gt;, bool&amp;) (/scratch3/packages/llvm/main-20251112/bin/clang-22+0x434bca2)
#<!-- -->14 0x0000560d712e961d llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&amp;) (/scratch3/packages/llvm/main-20251112/bin/clang-22+0x434c61d)
#<!-- -->15 0x0000560d712eaf5c llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&amp;) (/scratch3/packages/llvm/main-20251112/bin/clang-22+0x434df5c)
#<!-- -->16 0x0000560d712d68d9 llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&amp;) (/scratch3/packages/llvm/main-20251112/bin/clang-22+0x43398d9)
#<!-- -->17 0x0000560d6efe4d4b llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (.part.0) MachineFunctionPass.cpp:0:0
#<!-- -->18 0x0000560d6f681913 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (/scratch3/packages/llvm/main-20251112/bin/clang-22+0x26e4913)
#<!-- -->19 0x0000560d6f681d69 llvm::FPPassManager::runOnModule(llvm::Module&amp;) (/scratch3/packages/llvm/main-20251112/bin/clang-22+0x26e4d69)
#<!-- -->20 0x0000560d6f6826b7 llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/scratch3/packages/llvm/main-20251112/bin/clang-22+0x26e56b7)
#<!-- -->21 0x0000560d700635e3 clang::emitBackendOutput(clang::CompilerInstance&amp;, clang::CodeGenOptions&amp;, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;, std::unique_ptr&lt;llvm::raw_pwrite_stream, std::default_delete&lt;llvm::raw_pwrite_stream&gt;&gt;, clang::BackendConsumer*) (/scratch3/packages/llvm/main-20251112/bin/clang-22+0x30c65e3)
#<!-- -->22 0x0000560d706efc82 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&amp;) (/scratch3/packages/llvm/main-20251112/bin/clang-22+0x3752c82)
#<!-- -->23 0x0000560d7220a60c clang::ParseAST(clang::Sema&amp;, bool, bool) (/scratch3/packages/llvm/main-20251112/bin/clang-22+0x526d60c)
#<!-- -->24 0x0000560d70a1bc09 clang::FrontendAction::Execute() (/scratch3/packages/llvm/main-20251112/bin/clang-22+0x3a7ec09)
#<!-- -->25 0x0000560d7099b6f5 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/scratch3/packages/llvm/main-20251112/bin/clang-22+0x39fe6f5)
#<!-- -->26 0x0000560d70b11143 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/scratch3/packages/llvm/main-20251112/bin/clang-22+0x3b74143)
#<!-- -->27 0x0000560d6ddef287 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/scratch3/packages/llvm/main-20251112/bin/clang-22+0xe52287)
#<!-- -->28 0x0000560d6dde597a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->29 0x0000560d6dde9c78 clang_main(int, char**, llvm::ToolContext const&amp;) (/scratch3/packages/llvm/main-20251112/bin/clang-22+0xe4cc78)
#<!-- -->30 0x0000560d6dcf4adb main (/scratch3/packages/llvm/main-20251112/bin/clang-22+0xd57adb)
#<!-- -->31 0x00007faa7b4aad90 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:58:16
#<!-- -->32 0x00007faa7b4aae40 call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->33 0x00007faa7b4aae40 __libc_start_main ./csu/../csu/libc-start.c:379:5
#<!-- -->34 0x0000560d6dde5095 _start (/scratch3/packages/llvm/main-20251112/bin/clang-22+0xe48095)
```
</details>


---

### Comment 2 - ye-luo

The reproducer no longer fails since 24be0ba39b065c9b0703b1cf082d220f1522efe4

---

