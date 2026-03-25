# Scalable vectors should not be legalized by scalarizing

**Author:** hvdijk
**URL:** https://github.com/llvm/llvm-project/issues/156882

## Body

```llvm
define void @xor(ptr %p1, ptr %p2) {
entry:
  %v1 = load <vscale x 2 x i1>, ptr %p1
  %v2 = load <vscale x 2 x i1>, ptr %p2
  %w1 = bitcast <vscale x 2 x i1> %v1 to <vscale x 1 x i2>
  %w2 = bitcast <vscale x 2 x i1> %v2 to <vscale x 1 x i2>
  %x = xor <vscale x 1 x i2> %w1, %w2
  %y = bitcast <vscale x 1 x i2> %x to <vscale x 2 x i1>
  store <vscale x 2 x i1> %y, ptr %p1
  ret void
}
```
With `clang --target=riscv64-linux-gnu -march=rv64gcv -S -o - test.ll -O0`, this succeeds:
```asm
	.attribute	4, 16
	.attribute	5, "rv64i2p1_m2p0_a2p1_f2p2_d2p2_c2p0_v1p0_zicsr2p0_zifencei2p0_zmmul1p0_zaamo1p0_zalrsc1p0_zca1p0_zcd1p0_zve32f1p0_zve32x1p0_zve64d1p0_zve64f1p0_zve64x1p0_zvl128b1p0_zvl32b1p0_zvl64b1p0"
	.file	"test.ll"
	.text
	.globl	xor                             # -- Begin function xor
	.p2align	1
	.type	xor,@function
xor:                                    # @xor
	.cfi_startproc
# %bb.0:                                # %entry
                                        # implicit-def: $v8
	vsetvli	a2, zero, e8, mf4, ta, ma
	vlm.v	v8, (a0)
                                        # implicit-def: $v9
	vlm.v	v9, (a1)
	vmxor.mm	v8, v8, v9
	vsm.v	v8, (a0)
	ret
.Lfunc_end0:
	.size	xor, .Lfunc_end0-xor
	.cfi_endproc
                                        # -- End function
	.section	".note.GNU-stack","",@progbits
	.addrsig
```
With `clang --target=riscv64-linux-gnu -march=rv64gcv -S -o - test.ll -O1`, this fails:
```
warning: overriding the module target triple with riscv64-unknown-linux-gnu [-Woverride-module]
	.attribute	4, 16
	.attribute	5, "rv64i2p1_m2p0_a2p1_f2p2_d2p2_c2p0_v1p0_zicsr2p0_zifencei2p0_zmmul1p0_zaamo1p0_zalrsc1p0_zca1p0_zcd1p0_zve32f1p0_zve32x1p0_zve64d1p0_zve64f1p0_zve64x1p0_zvl128b1p0_zvl32b1p0_zvl64b1p0"
	.file	"test.ll"
fatal error: error in backend: Cannot scalarize scalable vector loads
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: clang --target=riscv64-linux-gnu -march=rv64gcv -S -o - test.ll -O1
1.	Code generation
2.	Running pass 'Function Pass Manager' on module 'test.ll'.
3.	Running pass 'RISC-V DAG->DAG Pattern Instruction Selection' on function '@xor'
 #0 0x0000ffff7a3d6020 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /home/harald/llvm-project/main/llvm/lib/Support/Unix/Signals.inc:834:22
 #1 0x0000ffff7a3d6448 PrintStackTraceSignalHandler(void*) /home/harald/llvm-project/main/llvm/lib/Support/Unix/Signals.inc:918:1
 #2 0x0000ffff7a3d3d4c llvm::sys::RunSignalHandlers() /home/harald/llvm-project/main/llvm/lib/Support/Signals.cpp:104:20
 #3 0x0000ffff7a3d583c llvm::sys::CleanupOnSignal(unsigned long) /home/harald/llvm-project/main/llvm/lib/Support/Unix/Signals.inc:373:31
 #4 0x0000ffff7a270c84 (anonymous namespace)::CrashRecoveryContextImpl::HandleCrash(int, unsigned long) /home/harald/llvm-project/main/llvm/lib/Support/CrashRecoveryContext.cpp:73:5
 #5 0x0000ffff7a271434 llvm::CrashRecoveryContext::HandleExit(int) /home/harald/llvm-project/main/llvm/lib/Support/CrashRecoveryContext.cpp:446:3
 #6 0x0000ffff7a3ce01c llvm::sys::Process::Exit(int, bool) /home/harald/llvm-project/main/llvm/lib/Support/Process.cpp:114:3
 #7 0x0000aaaae1e12410 (clang+0x22410)
 #8 0x0000ffff7a2985bc llvm::report_fatal_error(llvm::Twine const&, bool) /home/harald/llvm-project/main/llvm/lib/Support/ErrorHandling.cpp:117:36
 #9 0x0000ffff7a298504 llvm::report_fatal_error(llvm::StringRef, bool) /home/harald/llvm-project/main/llvm/lib/Support/ErrorHandling.cpp:99:68
#10 0x0000ffff79127034 llvm::TargetLowering::scalarizeVectorLoad(llvm::LoadSDNode*, llvm::SelectionDAG&) const /home/harald/llvm-project/main/llvm/lib/CodeGen/SelectionDAG/TargetLowering.cpp:10162:48
#11 0x0000ffff78f0a8d0 (anonymous namespace)::VectorLegalizer::ExpandLoad(llvm::SDNode*) /home/harald/llvm-project/main/llvm/lib/CodeGen/SelectionDAG/LegalizeVectorOps.cpp:912:41
#12 0x0000ffff78f0a990 (anonymous namespace)::VectorLegalizer::Expand(llvm::SDNode*, llvm::SmallVectorImpl<llvm::SDValue>&) /home/harald/llvm-project/main/llvm/lib/CodeGen/SelectionDAG/LegalizeVectorOps.cpp:925:22
#13 0x0000ffff78f08bcc (anonymous namespace)::VectorLegalizer::LegalizeOp(llvm::SDValue) /home/harald/llvm-project/main/llvm/lib/CodeGen/SelectionDAG/LegalizeVectorOps.cpp:587:5
#14 0x0000ffff78f01df4 (anonymous namespace)::VectorLegalizer::Run() /home/harald/llvm-project/main/llvm/lib/CodeGen/SelectionDAG/LegalizeVectorOps.cpp:235:64
#15 0x0000ffff78f11da0 llvm::SelectionDAG::LegalizeVectors() /home/harald/llvm-project/main/llvm/lib/CodeGen/SelectionDAG/LegalizeVectorOps.cpp:2410:36
#16 0x0000ffff790a14c8 llvm::SelectionDAGISel::CodeGenAndEmitDAG() /home/harald/llvm-project/main/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:1012:38
#17 0x0000ffff790a07b4 llvm::SelectionDAGISel::SelectBasicBlock(llvm::ilist_iterator_w_bits<llvm::ilist_detail::node_options<llvm::Instruction, true, false, void, true, llvm::BasicBlock>, false, true>, llvm::ilist_iterator_w_bits<llvm::ilist_detail::node_options<llvm::Instruction, true, false, void, true, llvm::BasicBlock>, false, true>, bool&) /home/harald/llvm-project/main/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:869:1
#18 0x0000ffff790a5774 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&) /home/harald/llvm-project/main/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:1893:33
#19 0x0000ffff7909f2cc llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&) /home/harald/llvm-project/main/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:607:7
#20 0x0000ffff877bb670 llvm::RISCVDAGToDAGISel::runOnMachineFunction(llvm::MachineFunction&) /home/harald/llvm-project/main/llvm/lib/Target/RISCV/RISCVISelDAGToDAG.h:36:0
#21 0x0000ffff7909e294 llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&) /home/harald/llvm-project/main/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:372:40
#22 0x0000ffff8148905c llvm::MachineFunctionPass::runOnFunction(llvm::Function&) /home/harald/llvm-project/main/llvm/lib/CodeGen/MachineFunctionPass.cpp:108:30
#23 0x0000ffff7aad4554 llvm::FPPassManager::runOnFunction(llvm::Function&) /home/harald/llvm-project/main/llvm/lib/IR/LegacyPassManager.cpp:1398:40
#24 0x0000ffff7aad4784 llvm::FPPassManager::runOnModule(llvm::Module&) /home/harald/llvm-project/main/llvm/lib/IR/LegacyPassManager.cpp:1444:29
#25 0x0000ffff7aad4b70 (anonymous namespace)::MPPassManager::runOnModule(llvm::Module&) /home/harald/llvm-project/main/llvm/lib/IR/LegacyPassManager.cpp:1513:38
#26 0x0000ffff7aad09f4 llvm::legacy::PassManagerImpl::run(llvm::Module&) /home/harald/llvm-project/main/llvm/lib/IR/LegacyPassManager.cpp:531:55
#27 0x0000ffff7aad5318 llvm::legacy::PassManager::run(llvm::Module&) /home/harald/llvm-project/main/llvm/lib/IR/LegacyPassManager.cpp:1640:17
#28 0x0000ffff82613154 (anonymous namespace)::EmitAssemblyHelper::RunCodegenPipeline(clang::BackendAction, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>&, std::unique_ptr<llvm::ToolOutputFile, std::default_delete<llvm::ToolOutputFile>>&) /home/harald/llvm-project/main/clang/lib/CodeGen/BackendUtil.cpp:1254:9
#29 0x0000ffff826132f8 (anonymous namespace)::EmitAssemblyHelper::emitAssembly(clang::BackendAction, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) /home/harald/llvm-project/main/clang/lib/CodeGen/BackendUtil.cpp:1279:7
#30 0x0000ffff82613fc8 clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) /home/harald/llvm-project/main/clang/lib/CodeGen/BackendUtil.cpp:1442:25
#31 0x0000ffff82d12de8 clang::CodeGenAction::ExecuteAction() /home/harald/llvm-project/main/clang/lib/CodeGen/CodeGenAction.cpp:1189:20
#32 0x0000ffff7f388844 clang::FrontendAction::Execute() /home/harald/llvm-project/main/clang/lib/Frontend/FrontendAction.cpp:1314:38
#33 0x0000ffff7f29bec0 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) /home/harald/llvm-project/main/clang/lib/Frontend/CompilerInstance.cpp:1066:42
#34 0x0000ffff8526446c clang::ExecuteCompilerInvocation(clang::CompilerInstance*) /home/harald/llvm-project/main/clang/lib/FrontendTool/ExecuteCompilerInvocation.cpp:302:38
#35 0x0000aaaae1e13228 (clang+0x23228)
#36 0x0000aaaae1e04f60 (clang+0x14f60)
#37 0x0000aaaae1e050f8 (clang+0x150f8)
#38 0x0000aaaae1e06500 (clang+0x16500)
#39 0x0000ffff7ec8fa34 llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::operator()(llvm::SmallVectorImpl<char const*>&) const /home/harald/llvm-project/main/llvm/include/llvm/ADT/STLFunctionalExtras.h:70:3
#40 0x0000ffff7ec8df40 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()::operator()() const /home/harald/llvm-project/main/clang/lib/Driver/Job.cpp:436:32
#41 0x0000ffff7ec8e2b8 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) /home/harald/llvm-project/main/llvm/include/llvm/ADT/STLFunctionalExtras.h:47:40
#42 0x0000ffff7a259aa4 llvm::function_ref<void ()>::operator()() const /home/harald/llvm-project/main/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:62
#43 0x0000ffff7a2713cc llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) /home/harald/llvm-project/main/llvm/lib/Support/CrashRecoveryContext.cpp:427:10
#44 0x0000ffff7ec8e094 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const /home/harald/llvm-project/main/clang/lib/Driver/Job.cpp:436:21
#45 0x0000ffff7ec0b318 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const /home/harald/llvm-project/main/clang/lib/Driver/Compilation.cpp:196:22
#46 0x0000ffff7ec0b64c clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const /home/harald/llvm-project/main/clang/lib/Driver/Compilation.cpp:246:62
#47 0x0000ffff7ec24618 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) /home/harald/llvm-project/main/clang/lib/Driver/Driver.cpp:2244:28
#48 0x0000aaaae1e06074 (clang+0x16074)
#49 0x0000aaaae1e38444 (clang+0x48444)
#50 0x0000ffff79d0229c (/lib/aarch64-linux-gnu/libc.so.6+0x2229c)
#51 0x0000ffff79d0237c __libc_start_main (/lib/aarch64-linux-gnu/libc.so.6+0x2237c)
#52 0x0000aaaae1e04570 (clang+0x14570)
clang: error: clang frontend command failed with exit code 70 (use -v to see invocation)
clang version 22.0.0git (https://github.com/llvm/llvm-project ce5a1158b8a2bca214ad37cd206f6c8eb659ea3c)
Target: riscv64-unknown-linux-gnu
Thread model: posix
InstalledDir: /home/harald/llvm-project/main/build/aarch64-linux/bin
Build config: +unoptimized, +assertions
clang: note: diagnostic msg: Error generating preprocessed source(s) - no preprocessable inputs.
```
What's happening here is that LLVM is picking up on the fact that a load in type A, immediately followed by a bitcast to type B, can be optimised to a load of type B, but the type legalization cannot handle type B. We're trying to legalize the load of `<vscale x 1 x i2>` by scalarizing, but that is never going to work and we should not be trying that.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-risc-v

Author: Harald van Dijk (hvdijk)

<details>
```llvm
define void @<!-- -->xor(ptr %p1, ptr %p2) {
entry:
  %v1 = load &lt;vscale x 2 x i1&gt;, ptr %p1
  %v2 = load &lt;vscale x 2 x i1&gt;, ptr %p2
  %w1 = bitcast &lt;vscale x 2 x i1&gt; %v1 to &lt;vscale x 1 x i2&gt;
  %w2 = bitcast &lt;vscale x 2 x i1&gt; %v2 to &lt;vscale x 1 x i2&gt;
  %x = xor &lt;vscale x 1 x i2&gt; %w1, %w2
  %y = bitcast &lt;vscale x 1 x i2&gt; %x to &lt;vscale x 2 x i1&gt;
  store &lt;vscale x 2 x i1&gt; %y, ptr %p1
  ret void
}
```
With `clang --target=riscv64-linux-gnu -march=rv64gcv -S -o - test.ll -O0`, this succeeds:
```asm
	.attribute	4, 16
	.attribute	5, "rv64i2p1_m2p0_a2p1_f2p2_d2p2_c2p0_v1p0_zicsr2p0_zifencei2p0_zmmul1p0_zaamo1p0_zalrsc1p0_zca1p0_zcd1p0_zve32f1p0_zve32x1p0_zve64d1p0_zve64f1p0_zve64x1p0_zvl128b1p0_zvl32b1p0_zvl64b1p0"
	.file	"test.ll"
	.text
	.globl	xor                             # -- Begin function xor
	.p2align	1
	.type	xor,@<!-- -->function
xor:                                    # @<!-- -->xor
	.cfi_startproc
# %bb.0:                                # %entry
                                        # implicit-def: $v8
	vsetvli	a2, zero, e8, mf4, ta, ma
	vlm.v	v8, (a0)
                                        # implicit-def: $v9
	vlm.v	v9, (a1)
	vmxor.mm	v8, v8, v9
	vsm.v	v8, (a0)
	ret
.Lfunc_end0:
	.size	xor, .Lfunc_end0-xor
	.cfi_endproc
                                        # -- End function
	.section	".note.GNU-stack","",@<!-- -->progbits
	.addrsig
```
With `clang --target=riscv64-linux-gnu -march=rv64gcv -S -o - test.ll -O1`, this fails:
```
warning: overriding the module target triple with riscv64-unknown-linux-gnu [-Woverride-module]
	.attribute	4, 16
	.attribute	5, "rv64i2p1_m2p0_a2p1_f2p2_d2p2_c2p0_v1p0_zicsr2p0_zifencei2p0_zmmul1p0_zaamo1p0_zalrsc1p0_zca1p0_zcd1p0_zve32f1p0_zve32x1p0_zve64d1p0_zve64f1p0_zve64x1p0_zvl128b1p0_zvl32b1p0_zvl64b1p0"
	.file	"test.ll"
fatal error: error in backend: Cannot scalarize scalable vector loads
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: clang --target=riscv64-linux-gnu -march=rv64gcv -S -o - test.ll -O1
1.	Code generation
2.	Running pass 'Function Pass Manager' on module 'test.ll'.
3.	Running pass 'RISC-V DAG-&gt;DAG Pattern Instruction Selection' on function '@<!-- -->xor'
 #<!-- -->0 0x0000ffff7a3d6020 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /home/harald/llvm-project/main/llvm/lib/Support/Unix/Signals.inc:834:22
 #<!-- -->1 0x0000ffff7a3d6448 PrintStackTraceSignalHandler(void*) /home/harald/llvm-project/main/llvm/lib/Support/Unix/Signals.inc:918:1
 #<!-- -->2 0x0000ffff7a3d3d4c llvm::sys::RunSignalHandlers() /home/harald/llvm-project/main/llvm/lib/Support/Signals.cpp:104:20
 #<!-- -->3 0x0000ffff7a3d583c llvm::sys::CleanupOnSignal(unsigned long) /home/harald/llvm-project/main/llvm/lib/Support/Unix/Signals.inc:373:31
 #<!-- -->4 0x0000ffff7a270c84 (anonymous namespace)::CrashRecoveryContextImpl::HandleCrash(int, unsigned long) /home/harald/llvm-project/main/llvm/lib/Support/CrashRecoveryContext.cpp:73:5
 #<!-- -->5 0x0000ffff7a271434 llvm::CrashRecoveryContext::HandleExit(int) /home/harald/llvm-project/main/llvm/lib/Support/CrashRecoveryContext.cpp:446:3
 #<!-- -->6 0x0000ffff7a3ce01c llvm::sys::Process::Exit(int, bool) /home/harald/llvm-project/main/llvm/lib/Support/Process.cpp:114:3
 #<!-- -->7 0x0000aaaae1e12410 (clang+0x22410)
 #<!-- -->8 0x0000ffff7a2985bc llvm::report_fatal_error(llvm::Twine const&amp;, bool) /home/harald/llvm-project/main/llvm/lib/Support/ErrorHandling.cpp:117:36
 #<!-- -->9 0x0000ffff7a298504 llvm::report_fatal_error(llvm::StringRef, bool) /home/harald/llvm-project/main/llvm/lib/Support/ErrorHandling.cpp:99:68
#<!-- -->10 0x0000ffff79127034 llvm::TargetLowering::scalarizeVectorLoad(llvm::LoadSDNode*, llvm::SelectionDAG&amp;) const /home/harald/llvm-project/main/llvm/lib/CodeGen/SelectionDAG/TargetLowering.cpp:10162:48
#<!-- -->11 0x0000ffff78f0a8d0 (anonymous namespace)::VectorLegalizer::ExpandLoad(llvm::SDNode*) /home/harald/llvm-project/main/llvm/lib/CodeGen/SelectionDAG/LegalizeVectorOps.cpp:912:41
#<!-- -->12 0x0000ffff78f0a990 (anonymous namespace)::VectorLegalizer::Expand(llvm::SDNode*, llvm::SmallVectorImpl&lt;llvm::SDValue&gt;&amp;) /home/harald/llvm-project/main/llvm/lib/CodeGen/SelectionDAG/LegalizeVectorOps.cpp:925:22
#<!-- -->13 0x0000ffff78f08bcc (anonymous namespace)::VectorLegalizer::LegalizeOp(llvm::SDValue) /home/harald/llvm-project/main/llvm/lib/CodeGen/SelectionDAG/LegalizeVectorOps.cpp:587:5
#<!-- -->14 0x0000ffff78f01df4 (anonymous namespace)::VectorLegalizer::Run() /home/harald/llvm-project/main/llvm/lib/CodeGen/SelectionDAG/LegalizeVectorOps.cpp:235:64
#<!-- -->15 0x0000ffff78f11da0 llvm::SelectionDAG::LegalizeVectors() /home/harald/llvm-project/main/llvm/lib/CodeGen/SelectionDAG/LegalizeVectorOps.cpp:2410:36
#<!-- -->16 0x0000ffff790a14c8 llvm::SelectionDAGISel::CodeGenAndEmitDAG() /home/harald/llvm-project/main/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:1012:38
#<!-- -->17 0x0000ffff790a07b4 llvm::SelectionDAGISel::SelectBasicBlock(llvm::ilist_iterator_w_bits&lt;llvm::ilist_detail::node_options&lt;llvm::Instruction, true, false, void, true, llvm::BasicBlock&gt;, false, true&gt;, llvm::ilist_iterator_w_bits&lt;llvm::ilist_detail::node_options&lt;llvm::Instruction, true, false, void, true, llvm::BasicBlock&gt;, false, true&gt;, bool&amp;) /home/harald/llvm-project/main/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:869:1
#<!-- -->18 0x0000ffff790a5774 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&amp;) /home/harald/llvm-project/main/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:1893:33
#<!-- -->19 0x0000ffff7909f2cc llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&amp;) /home/harald/llvm-project/main/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:607:7
#<!-- -->20 0x0000ffff877bb670 llvm::RISCVDAGToDAGISel::runOnMachineFunction(llvm::MachineFunction&amp;) /home/harald/llvm-project/main/llvm/lib/Target/RISCV/RISCVISelDAGToDAG.h:36:0
#<!-- -->21 0x0000ffff7909e294 llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&amp;) /home/harald/llvm-project/main/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:372:40
#<!-- -->22 0x0000ffff8148905c llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) /home/harald/llvm-project/main/llvm/lib/CodeGen/MachineFunctionPass.cpp:108:30
#<!-- -->23 0x0000ffff7aad4554 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) /home/harald/llvm-project/main/llvm/lib/IR/LegacyPassManager.cpp:1398:40
#<!-- -->24 0x0000ffff7aad4784 llvm::FPPassManager::runOnModule(llvm::Module&amp;) /home/harald/llvm-project/main/llvm/lib/IR/LegacyPassManager.cpp:1444:29
#<!-- -->25 0x0000ffff7aad4b70 (anonymous namespace)::MPPassManager::runOnModule(llvm::Module&amp;) /home/harald/llvm-project/main/llvm/lib/IR/LegacyPassManager.cpp:1513:38
#<!-- -->26 0x0000ffff7aad09f4 llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) /home/harald/llvm-project/main/llvm/lib/IR/LegacyPassManager.cpp:531:55
#<!-- -->27 0x0000ffff7aad5318 llvm::legacy::PassManager::run(llvm::Module&amp;) /home/harald/llvm-project/main/llvm/lib/IR/LegacyPassManager.cpp:1640:17
#<!-- -->28 0x0000ffff82613154 (anonymous namespace)::EmitAssemblyHelper::RunCodegenPipeline(clang::BackendAction, std::unique_ptr&lt;llvm::raw_pwrite_stream, std::default_delete&lt;llvm::raw_pwrite_stream&gt;&gt;&amp;, std::unique_ptr&lt;llvm::ToolOutputFile, std::default_delete&lt;llvm::ToolOutputFile&gt;&gt;&amp;) /home/harald/llvm-project/main/clang/lib/CodeGen/BackendUtil.cpp:1254:9
#<!-- -->29 0x0000ffff826132f8 (anonymous namespace)::EmitAssemblyHelper::emitAssembly(clang::BackendAction, std::unique_ptr&lt;llvm::raw_pwrite_stream, std::default_delete&lt;llvm::raw_pwrite_stream&gt;&gt;, clang::BackendConsumer*) /home/harald/llvm-project/main/clang/lib/CodeGen/BackendUtil.cpp:1279:7
#<!-- -->30 0x0000ffff82613fc8 clang::emitBackendOutput(clang::CompilerInstance&amp;, clang::CodeGenOptions&amp;, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;, std::unique_ptr&lt;llvm::raw_pwrite_stream, std::default_delete&lt;llvm::raw_pwrite_stream&gt;&gt;, clang::BackendConsumer*) /home/harald/llvm-project/main/clang/lib/CodeGen/BackendUtil.cpp:1442:25
#<!-- -->31 0x0000ffff82d12de8 clang::CodeGenAction::ExecuteAction() /home/harald/llvm-project/main/clang/lib/CodeGen/CodeGenAction.cpp:1189:20
#<!-- -->32 0x0000ffff7f388844 clang::FrontendAction::Execute() /home/harald/llvm-project/main/clang/lib/Frontend/FrontendAction.cpp:1314:38
#<!-- -->33 0x0000ffff7f29bec0 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) /home/harald/llvm-project/main/clang/lib/Frontend/CompilerInstance.cpp:1066:42
#<!-- -->34 0x0000ffff8526446c clang::ExecuteCompilerInvocation(clang::CompilerInstance*) /home/harald/llvm-project/main/clang/lib/FrontendTool/ExecuteCompilerInvocation.cpp:302:38
#<!-- -->35 0x0000aaaae1e13228 (clang+0x23228)
#<!-- -->36 0x0000aaaae1e04f60 (clang+0x14f60)
#<!-- -->37 0x0000aaaae1e050f8 (clang+0x150f8)
#<!-- -->38 0x0000aaaae1e06500 (clang+0x16500)
#<!-- -->39 0x0000ffff7ec8fa34 llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::operator()(llvm::SmallVectorImpl&lt;char const*&gt;&amp;) const /home/harald/llvm-project/main/llvm/include/llvm/ADT/STLFunctionalExtras.h:70:3
#<!-- -->40 0x0000ffff7ec8df40 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()::operator()() const /home/harald/llvm-project/main/clang/lib/Driver/Job.cpp:436:32
#<!-- -->41 0x0000ffff7ec8e2b8 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) /home/harald/llvm-project/main/llvm/include/llvm/ADT/STLFunctionalExtras.h:47:40
#<!-- -->42 0x0000ffff7a259aa4 llvm::function_ref&lt;void ()&gt;::operator()() const /home/harald/llvm-project/main/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:62
#<!-- -->43 0x0000ffff7a2713cc llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) /home/harald/llvm-project/main/llvm/lib/Support/CrashRecoveryContext.cpp:427:10
#<!-- -->44 0x0000ffff7ec8e094 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const /home/harald/llvm-project/main/clang/lib/Driver/Job.cpp:436:21
#<!-- -->45 0x0000ffff7ec0b318 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const /home/harald/llvm-project/main/clang/lib/Driver/Compilation.cpp:196:22
#<!-- -->46 0x0000ffff7ec0b64c clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const /home/harald/llvm-project/main/clang/lib/Driver/Compilation.cpp:246:62
#<!-- -->47 0x0000ffff7ec24618 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) /home/harald/llvm-project/main/clang/lib/Driver/Driver.cpp:2244:28
#<!-- -->48 0x0000aaaae1e06074 (clang+0x16074)
#<!-- -->49 0x0000aaaae1e38444 (clang+0x48444)
#<!-- -->50 0x0000ffff79d0229c (/lib/aarch64-linux-gnu/libc.so.6+0x2229c)
#<!-- -->51 0x0000ffff79d0237c __libc_start_main (/lib/aarch64-linux-gnu/libc.so.6+0x2237c)
#<!-- -->52 0x0000aaaae1e04570 (clang+0x14570)
clang: error: clang frontend command failed with exit code 70 (use -v to see invocation)
clang version 22.0.0git (https://github.com/llvm/llvm-project ce5a1158b8a2bca214ad37cd206f6c8eb659ea3c)
Target: riscv64-unknown-linux-gnu
Thread model: posix
InstalledDir: /home/harald/llvm-project/main/build/aarch64-linux/bin
Build config: +unoptimized, +assertions
clang: note: diagnostic msg: Error generating preprocessed source(s) - no preprocessable inputs.
```
What's happening here is that LLVM is picking up on the fact that a load in type A, immediately followed by a bitcast to type B, can be optimised to a load of type B, but the type legalization cannot handle type B. We're trying to legalize the load of `&lt;vscale x 1 x i2&gt;` by scalarizing, but that is never going to work and we should not be trying that.
</details>


---

### Comment 2 - topperc

I don't know where to begin to fix this. If I disable the load+bitcast folding we still end up creating a store+load of a stack temporary for the bitcast. That hits the same crash.

---

### Comment 3 - hvdijk

Yeah, I think the load+bitcast folding should just be considered valid, and we need to find a way of legalizing the load/store of `<vscale x 1 x i2>`. That may be possible by effectively re-inserting the bitcast and using what's already there for `<vscale x 2 x i1>`, but that would be a very special case, and I'm also trying to think of a more general solution.

---

