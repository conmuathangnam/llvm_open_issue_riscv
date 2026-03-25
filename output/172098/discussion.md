# Assertion failure during ARM Block Placement pass for ARM 32 bit baremetal target

**Author:** Prabhuk
**URL:** https://github.com/llvm/llvm-project/issues/172098
**Status:** Open
**Labels:** backend:ARM, incomplete, crash

## Body

We ran into an assertion failure while handling the following instruction in ARM Block Placement Pass:

```
t2WhileLoopStart killed renamable $lr, %bb.12, implicit-def dead $cpsr, debug-location !2176;
```

The crash was due to the fact that `t2WhileLoopStart` instruction was not expected to be present by the time ARM Block Placement pass was run. I printed out the MachineInstructions for the function in question and noticed that `arm-mve-vpt-opts` pass did not remove the t2WhileLoopStart instruction and found this in the debug log:

```
Extra users of register found: t2WhileLoopStart %57:gprlr, %bb.18, implicit-def dead $cpsr, debug-location !2176;
``` 
This log appears from this location: https://github.com/llvm/llvm-project/blob/main/llvm/lib/Target/ARM/MVETPAndVPTOptimisationsPass.cpp#L371

Trying to figure out if this is a bug in the compilation pipeline. 

The assertion crash backtrace from lldb below. I am working on generating a reproducer that can be shared publicly.

```
clang: llvm/include/llvm/CodeGen/MachineOperand.h:572: MachineBasicBlock *llvm::MachineOperand::getMBB() const: Assertion `isMBB() && "Wrong MachineOperand accessor"' failed.
Process 2165633 stopped
* thread #1, name = 'clang', stop reason = signal SIGABRT
    frame #0: 0x00007ffff7d4395c libc.so.6`__pthread_kill_implementation(threadid=<unavailable>, signo=6, no_tid=<unavailable>) at pthread_kill.c:44:76
(lldb) bt
* thread #1, name = 'clang', stop reason = signal SIGABRT
  * frame #0: 0x00007ffff7d4395c libc.so.6`__pthread_kill_implementation(threadid=<unavailable>, signo=6, no_tid=<unavailable>) at pthread_kill.c:44:76
    frame #1: 0x00007ffff7ceecc2 libc.so.6`__GI_raise(sig=6) at raise.c:26:13
    frame #2: 0x00007ffff7cd74ac libc.so.6`__GI_abort at abort.c:73:3
    frame #3: 0x00007ffff7cd7420 libc.so.6`__assert_fail_base(fmt=<unavailable>, assertion=<unavailable>, file=<unavailable>, line=572, function=<unavailable>) at assert.c:118:3
    frame #4: 0x000055555b909341 clang`llvm::MachineOperand::getMBB(this=<unavailable>) const at MachineOperand.h:572:5 [inlined]
    frame #5: 0x000055555b909322 clang`llvm::getWhileLoopStartTargetBB(MI=<unavailable>) at MVETailPredUtils.h:90:28 [inlined]
    frame #6: 0x000055555b909303 clang`llvm::ARMBlockPlacement::fixBackwardsWLS(this=0x000055555ec24d80, ML=0x000055555ed5bd60) at ARMBlockPlacement.cpp:0
    frame #7: 0x000055555b9097c2 clang`llvm::ARMBlockPlacement::processPostOrderLoops(this=0x000055555ec24d80, ML=0x000055555ed5bd60) at ARMBlockPlacement.cpp:210:20
    frame #8: 0x000055555b9097ab clang`llvm::ARMBlockPlacement::processPostOrderLoops(this=0x000055555ec24d80, ML=0x000055555ed5be00) at ARMBlockPlacement.cpp:209:16
    frame #9: 0x000055555b9097ab clang`llvm::ARMBlockPlacement::processPostOrderLoops(this=0x000055555ec24d80, ML=0x000055555ed5bea0) at ARMBlockPlacement.cpp:209:16
    frame #10: 0x000055555b909b0b clang`llvm::ARMBlockPlacement::runOnMachineFunction(this=0x000055555ec24d80, MF=<unavailable>) at ARMBlockPlacement.cpp:231:16
    frame #11: 0x000055555cc2b9d3 clang`llvm::MachineFunctionPass::runOnFunction(this=0x000055555ec24d80, F=0x000055555ec71268) at MachineFunctionPass.cpp:108:10
    frame #12: 0x000055555df79b8d clang`llvm::FPPassManager::runOnFunction(this=0x000055555e947ac0, F=0x000055555ec71268) at LegacyPassManager.cpp:1398:27
    frame #13: 0x000055555df7f812 clang`llvm::FPPassManager::runOnModule(this=0x000055555e947ac0, M=<unavailable>) at LegacyPassManager.cpp:1444:16
    frame #14: 0x000055555df7a16c clang`(anonymous namespace)::MPPassManager::runOnModule(this=0x000055555ecab9f0, M=0x000055555e7d56f0) at LegacyPassManager.cpp:1513:27
    frame #15: 0x000055555df79f48 clang`llvm::legacy::PassManagerImpl::run(this=0x000055555eb7dcd0, M=0x000055555e7d56f0) at LegacyPassManager.cpp:531:44
    frame #16: 0x000055555df7fbb4 clang`llvm::legacy::PassManager::run(this=<unavailable>, M=<unavailable>) at LegacyPassManager.cpp:1640:14 [artificial]
    frame #17: 0x00005555594c3b7e clang`(anonymous namespace)::EmitAssemblyHelper::RunCodegenPipeline(this=<unavailable>, Action=Backend_EmitObj, OS=<unavailable>, DwoOS=<unavailable>) at BackendUtil.cpp:1319:19
    frame #18: 0x00005555594c3919 clang`(anonymous namespace)::EmitAssemblyHelper::emitAssembly(this=<unavailable>, Action=Backend_EmitObj, OS=<unavailable>, BC=<unavailable>) at BackendUtil.cpp:1343:3 [inlined]
    frame #19: 0x00005555594c331c clang`clang::emitBackendOutput(CI=0x000055555e80a2a0, CGOpts=0x000055555e7d9718, TDesc=(Data = "e-m:e-p:32:32-Fi8-i64:64-v128:64:128-a:0:32-n32-S64", Length = 51), M=0x000055555e7d56f0, Action=Backend_EmitObj, VFS=<unava
ilable>, OS=nullptr, BC=<unavailable>) at BackendUtil.cpp:1513:13
    frame #20: 0x00005555594d5e2b clang`clang::BackendConsumer::HandleTranslationUnit(this=0x000055555e8413a0, C=0x000055555e800460) at CodeGenAction.cpp:312:3
    frame #21: 0x000055555a903359 clang`clang::ParseAST(S=0x000055555e8b2af0, PrintStats=false, SkipFunctionBodies=<unavailable>) at ParseAST.cpp:183:13
    frame #22: 0x00005555597f2b96 clang`clang::FrontendAction::Execute(this=<unavailable>) at FrontendAction.cpp:1312:3
    frame #23: 0x000055555977650d clang`clang::CompilerInstance::ExecuteAction(this=0x000055555e80a2a0, Act=0x000055555e7e6fb0) at CompilerInstance.cpp:1003:33
    frame #24: 0x00005555598cb043 clang`clang::ExecuteCompilerInvocation(Clang=0x000055555e80a2a0) at ExecuteCompilerInvocation.cpp:310:25
    frame #25: 0x000055555887a2ee clang`cc1_main(Argv=ArrayRef<const char *> @ 0x000010a86f313b70, Argv0="/usr/local/google/home/prabhukr/llvm-builds/build/install/bin/llvm", MainAddr=0x0000555558875b70) at cc1_main.cpp:300:15
    frame #26: 0x0000555558877bf3 clang`ExecuteCC1Tool(ArgV=<unavailable>, ToolContext=0x00007fffffff80d0, VFS=IntrusiveRefCntPtr<llvm::vfs::FileSystem> @ 0x00007fffffff38d8) at driver.cpp:225:12
    frame #27: 0x0000555558879b7d clang`clang_main(int, char**, llvm::ToolContext const&)::$_0::operator()(this=<unavailable>, ArgV=<unavailable>) const at driver.cpp:369:12 [inlined]
    frame #28: 0x0000555558879b58 clang`int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::$_0>(callable=<unavailable>, params=<unavailable>) at STLFunctionalExtras.h:46:12
    frame #29: 0x0000555559602cc9 clang`llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::operator()(this=<unavailable>, params=<unavailable>) const at STLFunctionalExtras.h:69:12 [inlined]
    frame #30: 0x0000555559602cbc clang`clang::driver::CC1Command::Execute(llvm::ArrayRef<std::__2::optional<llvm::StringRef>>, std::__2::basic_string<char, std::__2::char_traits<char>, std::__2::allocator<char>>*, bool*) const::$_0::operator()(this=0x00
007fffffff3938) const at Job.cpp:437:34 [inlined]
    frame #31: 0x0000555559602cb4 clang`void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::__2::optional<llvm::StringRef>>, std::__2::basic_string<char, std::__2::char_traits<char>, std::__2::allocator<ch
ar>>*, bool*) const::$_0>(callable=140737488304440) at STLFunctionalExtras.h:46:12
    frame #32: 0x000055555e1b24ae clang`llvm::function_ref<void ()>::operator()(this=<unavailable>) const at STLFunctionalExtras.h:69:12 [inlined]
    frame #33: 0x000055555e1b24a9 clang`llvm::CrashRecoveryContext::RunSafely(this=0x00007fffffff3950, Fn=function_ref<void ()> @ 0x000010a97c168190) at CrashRecoveryContext.cpp:426:3
    frame #34: 0x00005555596023f3 clang`clang::driver::CC1Command::Execute(this=0x000055555e7b8e10, Redirects=<unavailable>, ErrMsg=<unavailable>, ExecutionFailed=<unavailable>) const at Job.cpp:437:12
    frame #35: 0x00005555595c73b3 clang`clang::driver::Compilation::ExecuteCommand(this=0x000055555e7cd050, C=0x000055555e7b8e10, FailingCommand=0x00007fffffff3e70, LogOnly=<unavailable>) const at Compilation.cpp:196:15
    frame #36: 0x00005555595c75ee clang`clang::driver::Compilation::ExecuteJobs(this=0x000055555e7cd050, Jobs=0x000055555e7cd0d0, FailingCommands=0x00007fffffff3f90, LogOnly=<unavailable>) const at Compilation.cpp:246:19
    frame #37: 0x00005555595df0bf clang`clang::driver::Driver::ExecuteCompilation(this=0x00007fffffff4120, C=0x000055555e7cd050, FailingCommands=0x00007fffffff3f90) at Driver.cpp:2293:5
    frame #38: 0x000055555887738c clang`clang_main(Argc=278, Argv=0x00007fffffff8318, ToolContext=0x00007fffffff80d0) at driver.cpp:407:21
    frame #39: 0x0000555558b9de4e clang`findTool(Argc=278, Argv=0x00007fffffff8318, Argv0=<unavailable>) at LLVMDriverTools.def:0:1
    frame #40: 0x0000555558b9d51f clang`main(Argc=278, Argv=0x00007fffffff8318) at llvm-driver.cpp:85:10
    frame #41: 0x00007ffff7cd8ca8 libc.so.6`__libc_start_call_main(main=(clang`main at llvm-driver.cpp:83), argc=278, argv=0x00007fffffff8318) at libc_start_call_main.h:58:16
    frame #42: 0x00007ffff7cd8d65 libc.so.6`__libc_start_main_impl(main=(clang`main at llvm-driver.cpp:83), argc=278, argv=0x00007fffffff8318, init=<unavailable>, fini=<unavailable>, rtld_fini=<unavailable>, stack_end=0x00007fffffff8308) at libc-start.c:
360:3
    frame #43: 0x0000555558869569 clang`_start + 41
```



## Comments

### Comment 1 - Prabhuk

CC: @davemgreen 

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-backend-arm

Author: Prabhu Rajasekaran (Prabhuk)

<details>
We ran into an assertion failure while handling the following instruction in ARM Block Placement Pass:

```
t2WhileLoopStart killed renamable $lr, %bb.12, implicit-def dead $cpsr, debug-location !2176;
```

The crash was due to the fact that `t2WhileLoopStart` instruction was not expected to be present by the time ARM Block Placement pass was run. I printed out the MachineInstructions for the function in question and noticed that `arm-mve-vpt-opts` pass did not remove the t2WhileLoopStart instruction and found this in the debug log:

```
Extra users of register found: t2WhileLoopStart %57:gprlr, %bb.18, implicit-def dead $cpsr, debug-location !2176;
``` 
This log appears from this location: https://github.com/llvm/llvm-project/blob/main/llvm/lib/Target/ARM/MVETPAndVPTOptimisationsPass.cpp#L371

Trying to figure out if this is a bug in the compilation pipeline. 

The assertion crash backtrace from lldb below. I am working on generating a reproducer that can be shared publicly.

```
clang: llvm/include/llvm/CodeGen/MachineOperand.h:572: MachineBasicBlock *llvm::MachineOperand::getMBB() const: Assertion `isMBB() &amp;&amp; "Wrong MachineOperand accessor"' failed.
Process 2165633 stopped
* thread #<!-- -->1, name = 'clang', stop reason = signal SIGABRT
    frame #<!-- -->0: 0x00007ffff7d4395c libc.so.6`__pthread_kill_implementation(threadid=&lt;unavailable&gt;, signo=6, no_tid=&lt;unavailable&gt;) at pthread_kill.c:44:76
(lldb) bt
* thread #<!-- -->1, name = 'clang', stop reason = signal SIGABRT
  * frame #<!-- -->0: 0x00007ffff7d4395c libc.so.6`__pthread_kill_implementation(threadid=&lt;unavailable&gt;, signo=6, no_tid=&lt;unavailable&gt;) at pthread_kill.c:44:76
    frame #<!-- -->1: 0x00007ffff7ceecc2 libc.so.6`__GI_raise(sig=6) at raise.c:26:13
    frame #<!-- -->2: 0x00007ffff7cd74ac libc.so.6`__GI_abort at abort.c:73:3
    frame #<!-- -->3: 0x00007ffff7cd7420 libc.so.6`__assert_fail_base(fmt=&lt;unavailable&gt;, assertion=&lt;unavailable&gt;, file=&lt;unavailable&gt;, line=572, function=&lt;unavailable&gt;) at assert.c:118:3
    frame #<!-- -->4: 0x000055555b909341 clang`llvm::MachineOperand::getMBB(this=&lt;unavailable&gt;) const at MachineOperand.h:572:5 [inlined]
    frame #<!-- -->5: 0x000055555b909322 clang`llvm::getWhileLoopStartTargetBB(MI=&lt;unavailable&gt;) at MVETailPredUtils.h:90:28 [inlined]
    frame #<!-- -->6: 0x000055555b909303 clang`llvm::ARMBlockPlacement::fixBackwardsWLS(this=0x000055555ec24d80, ML=0x000055555ed5bd60) at ARMBlockPlacement.cpp:0
    frame #<!-- -->7: 0x000055555b9097c2 clang`llvm::ARMBlockPlacement::processPostOrderLoops(this=0x000055555ec24d80, ML=0x000055555ed5bd60) at ARMBlockPlacement.cpp:210:20
    frame #<!-- -->8: 0x000055555b9097ab clang`llvm::ARMBlockPlacement::processPostOrderLoops(this=0x000055555ec24d80, ML=0x000055555ed5be00) at ARMBlockPlacement.cpp:209:16
    frame #<!-- -->9: 0x000055555b9097ab clang`llvm::ARMBlockPlacement::processPostOrderLoops(this=0x000055555ec24d80, ML=0x000055555ed5bea0) at ARMBlockPlacement.cpp:209:16
    frame #<!-- -->10: 0x000055555b909b0b clang`llvm::ARMBlockPlacement::runOnMachineFunction(this=0x000055555ec24d80, MF=&lt;unavailable&gt;) at ARMBlockPlacement.cpp:231:16
    frame #<!-- -->11: 0x000055555cc2b9d3 clang`llvm::MachineFunctionPass::runOnFunction(this=0x000055555ec24d80, F=0x000055555ec71268) at MachineFunctionPass.cpp:108:10
    frame #<!-- -->12: 0x000055555df79b8d clang`llvm::FPPassManager::runOnFunction(this=0x000055555e947ac0, F=0x000055555ec71268) at LegacyPassManager.cpp:1398:27
    frame #<!-- -->13: 0x000055555df7f812 clang`llvm::FPPassManager::runOnModule(this=0x000055555e947ac0, M=&lt;unavailable&gt;) at LegacyPassManager.cpp:1444:16
    frame #<!-- -->14: 0x000055555df7a16c clang`(anonymous namespace)::MPPassManager::runOnModule(this=0x000055555ecab9f0, M=0x000055555e7d56f0) at LegacyPassManager.cpp:1513:27
    frame #<!-- -->15: 0x000055555df79f48 clang`llvm::legacy::PassManagerImpl::run(this=0x000055555eb7dcd0, M=0x000055555e7d56f0) at LegacyPassManager.cpp:531:44
    frame #<!-- -->16: 0x000055555df7fbb4 clang`llvm::legacy::PassManager::run(this=&lt;unavailable&gt;, M=&lt;unavailable&gt;) at LegacyPassManager.cpp:1640:14 [artificial]
    frame #<!-- -->17: 0x00005555594c3b7e clang`(anonymous namespace)::EmitAssemblyHelper::RunCodegenPipeline(this=&lt;unavailable&gt;, Action=Backend_EmitObj, OS=&lt;unavailable&gt;, DwoOS=&lt;unavailable&gt;) at BackendUtil.cpp:1319:19
    frame #<!-- -->18: 0x00005555594c3919 clang`(anonymous namespace)::EmitAssemblyHelper::emitAssembly(this=&lt;unavailable&gt;, Action=Backend_EmitObj, OS=&lt;unavailable&gt;, BC=&lt;unavailable&gt;) at BackendUtil.cpp:1343:3 [inlined]
    frame #<!-- -->19: 0x00005555594c331c clang`clang::emitBackendOutput(CI=0x000055555e80a2a0, CGOpts=0x000055555e7d9718, TDesc=(Data = "e-m:e-p:32:32-Fi8-i64:64-v128:64:128-a:0:32-n32-S64", Length = 51), M=0x000055555e7d56f0, Action=Backend_EmitObj, VFS=&lt;unava
ilable&gt;, OS=nullptr, BC=&lt;unavailable&gt;) at BackendUtil.cpp:1513:13
    frame #<!-- -->20: 0x00005555594d5e2b clang`clang::BackendConsumer::HandleTranslationUnit(this=0x000055555e8413a0, C=0x000055555e800460) at CodeGenAction.cpp:312:3
    frame #<!-- -->21: 0x000055555a903359 clang`clang::ParseAST(S=0x000055555e8b2af0, PrintStats=false, SkipFunctionBodies=&lt;unavailable&gt;) at ParseAST.cpp:183:13
    frame #<!-- -->22: 0x00005555597f2b96 clang`clang::FrontendAction::Execute(this=&lt;unavailable&gt;) at FrontendAction.cpp:1312:3
    frame #<!-- -->23: 0x000055555977650d clang`clang::CompilerInstance::ExecuteAction(this=0x000055555e80a2a0, Act=0x000055555e7e6fb0) at CompilerInstance.cpp:1003:33
    frame #<!-- -->24: 0x00005555598cb043 clang`clang::ExecuteCompilerInvocation(Clang=0x000055555e80a2a0) at ExecuteCompilerInvocation.cpp:310:25
    frame #<!-- -->25: 0x000055555887a2ee clang`cc1_main(Argv=ArrayRef&lt;const char *&gt; @ 0x000010a86f313b70, Argv0="/usr/local/google/home/prabhukr/llvm-builds/build/install/bin/llvm", MainAddr=0x0000555558875b70) at cc1_main.cpp:300:15
    frame #<!-- -->26: 0x0000555558877bf3 clang`ExecuteCC1Tool(ArgV=&lt;unavailable&gt;, ToolContext=0x00007fffffff80d0, VFS=IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt; @ 0x00007fffffff38d8) at driver.cpp:225:12
    frame #<!-- -->27: 0x0000555558879b7d clang`clang_main(int, char**, llvm::ToolContext const&amp;)::$_0::operator()(this=&lt;unavailable&gt;, ArgV=&lt;unavailable&gt;) const at driver.cpp:369:12 [inlined]
    frame #<!-- -->28: 0x0000555558879b58 clang`int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::$_0&gt;(callable=&lt;unavailable&gt;, params=&lt;unavailable&gt;) at STLFunctionalExtras.h:46:12
    frame #<!-- -->29: 0x0000555559602cc9 clang`llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::operator()(this=&lt;unavailable&gt;, params=&lt;unavailable&gt;) const at STLFunctionalExtras.h:69:12 [inlined]
    frame #<!-- -->30: 0x0000555559602cbc clang`clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::__2::optional&lt;llvm::StringRef&gt;&gt;, std::__2::basic_string&lt;char, std::__2::char_traits&lt;char&gt;, std::__2::allocator&lt;char&gt;&gt;*, bool*) const::$_0::operator()(this=0x00
007fffffff3938) const at Job.cpp:437:34 [inlined]
    frame #<!-- -->31: 0x0000555559602cb4 clang`void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::__2::optional&lt;llvm::StringRef&gt;&gt;, std::__2::basic_string&lt;char, std::__2::char_traits&lt;char&gt;, std::__2::allocator&lt;ch
ar&gt;&gt;*, bool*) const::$_0&gt;(callable=140737488304440) at STLFunctionalExtras.h:46:12
    frame #<!-- -->32: 0x000055555e1b24ae clang`llvm::function_ref&lt;void ()&gt;::operator()(this=&lt;unavailable&gt;) const at STLFunctionalExtras.h:69:12 [inlined]
    frame #<!-- -->33: 0x000055555e1b24a9 clang`llvm::CrashRecoveryContext::RunSafely(this=0x00007fffffff3950, Fn=function_ref&lt;void ()&gt; @ 0x000010a97c168190) at CrashRecoveryContext.cpp:426:3
    frame #<!-- -->34: 0x00005555596023f3 clang`clang::driver::CC1Command::Execute(this=0x000055555e7b8e10, Redirects=&lt;unavailable&gt;, ErrMsg=&lt;unavailable&gt;, ExecutionFailed=&lt;unavailable&gt;) const at Job.cpp:437:12
    frame #<!-- -->35: 0x00005555595c73b3 clang`clang::driver::Compilation::ExecuteCommand(this=0x000055555e7cd050, C=0x000055555e7b8e10, FailingCommand=0x00007fffffff3e70, LogOnly=&lt;unavailable&gt;) const at Compilation.cpp:196:15
    frame #<!-- -->36: 0x00005555595c75ee clang`clang::driver::Compilation::ExecuteJobs(this=0x000055555e7cd050, Jobs=0x000055555e7cd0d0, FailingCommands=0x00007fffffff3f90, LogOnly=&lt;unavailable&gt;) const at Compilation.cpp:246:19
    frame #<!-- -->37: 0x00005555595df0bf clang`clang::driver::Driver::ExecuteCompilation(this=0x00007fffffff4120, C=0x000055555e7cd050, FailingCommands=0x00007fffffff3f90) at Driver.cpp:2293:5
    frame #<!-- -->38: 0x000055555887738c clang`clang_main(Argc=278, Argv=0x00007fffffff8318, ToolContext=0x00007fffffff80d0) at driver.cpp:407:21
    frame #<!-- -->39: 0x0000555558b9de4e clang`findTool(Argc=278, Argv=0x00007fffffff8318, Argv0=&lt;unavailable&gt;) at LLVMDriverTools.def:0:1
    frame #<!-- -->40: 0x0000555558b9d51f clang`main(Argc=278, Argv=0x00007fffffff8318) at llvm-driver.cpp:85:10
    frame #<!-- -->41: 0x00007ffff7cd8ca8 libc.so.6`__libc_start_call_main(main=(clang`main at llvm-driver.cpp:83), argc=278, argv=0x00007fffffff8318) at libc_start_call_main.h:58:16
    frame #<!-- -->42: 0x00007ffff7cd8d65 libc.so.6`__libc_start_main_impl(main=(clang`main at llvm-driver.cpp:83), argc=278, argv=0x00007fffffff8318, init=&lt;unavailable&gt;, fini=&lt;unavailable&gt;, rtld_fini=&lt;unavailable&gt;, stack_end=0x00007fffffff8308) at libc-start.c:
360:3
    frame #<!-- -->43: 0x0000555558869569 clang`_start + 41
```


</details>


---

### Comment 3 - davemgreen

Do you have a reproducer?

---

