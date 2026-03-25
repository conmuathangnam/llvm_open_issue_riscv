# [CodeGen] Assertion failure in MachineInstr::setDebugValueUndef during BranchFolder optimization

**Author:** krystophny
**URL:** https://github.com/llvm/llvm-project/issues/151047
**Status:** Open
**Labels:** llvm:codegen, crash

## Body

## Summary
Clang crashes with an assertion failure when building LLVM's Polly component in a self-hosted build configuration. The crash occurs during the Control Flow Optimizer pass in the BranchFolder's `HoistCommonCodeInSuccs` function.

## Bug Classification
Compiler crash bug - Backend code generator crash

## Assertion Details
```
Assertion failed: (isDebugValue() && "Must be a debug value instruction."), function setDebugValueUndef, file MachineInstr.h, line 2004.
```

## Stack Trace
```
Stack dump:
0.	Program arguments: clang [compiler flags]
1.	<eof> parser at end of file
2.	Code generation
3.	Running pass 'Function Pass Manager' on module 'polly/lib/External/isl/isl_transitive_closure.c'.
4.	Running pass 'Control Flow Optimizer' on function '@has_spurious_elements'
 #8 (anonymous namespace)::BranchRelaxation::run(llvm::MachineFunction&) (.cold.9)
 #9 llvm::MachineInstr::setDebugValueUndef()
#10 llvm::BranchFolder::HoistCommonCodeInSuccs(llvm::MachineBasicBlock*)::::operator()
#11 llvm::BranchFolder::HoistCommonCodeInSuccs(llvm::MachineBasicBlock*)
#12 llvm::BranchFolder::OptimizeFunction(...)
```

## Reproduction Steps
1. Configure LLVM build with self-hosted clang
2. Run `ninja` in build directory
3. Crash occurs when compiling `polly/lib/External/isl/isl_transitive_closure.c`
4. Also reproduces with `polly/lib/External/isl/isl_scheduler.c`

## Build Configuration
- **Host System:** macOS (Darwin 24.5.0, arm64)
- **Compiler:** clang version 22.0.0git 
- **Target:** arm64-apple-darwin24.5.0
- **Build Type:** Self-hosted build (clang building LLVM)
- **Optimization:** `-O2 -g -DNDEBUG`
- **Assertions:** Enabled (`+assertions`)

## Analysis
The assertion failure occurs when `setDebugValueUndef()` is called on a `MachineInstr` that fails the `isDebugValue()` check. This suggests that:

1. The BranchFolder optimization is incorrectly identifying a non-debug instruction as a debug instruction
2. There may be corruption in the debug metadata during the optimization pass
3. The debug instruction handling in `HoistCommonCodeInSuccs` has a logic error

This appears related to recent changes in debug instruction handling during branch folding, as there were recent reverts of patches modifying this code (commits 654c0d8 and 8d4fde5).

## Affected Files
- Files that trigger the crash: `polly/lib/External/isl/isl_transitive_closure.c`, `polly/lib/External/isl/isl_scheduler.c`
- Code location: `MachineInstr.h:2004` (`setDebugValueUndef` function)
- Optimization pass: `BranchFolder::HoistCommonCodeInSuccs`

## Workarounds
- Building with `-O1` or `-O0` may avoid the crash
- Disabling debug info (`-g`) may prevent the issue
- Excluding Polly from the build avoids the crash

## Additional Context
This crash prevents successful self-hosted LLVM builds and appears to be a regression in the debug instruction handling during backend optimization passes.

## Comments

### Comment 1 - krystophny

## Additional Crash Instance - Same Root Cause

This assertion failure also occurs when compiling **clangd's CompletionModel.cpp**, confirming this is a **systematic bug** in the compiler, not specific to Polly/ISL files.

### Second Crash Details
- **File**: `tools/clang/tools/extra/clangd/CompletionModel.cpp`
- **Function being optimized**: `@_ZN5clang6clangd12_GLOBAL__N_113EvaluateTree0ERKNS0_7ExampleE`
- **Same assertion**: `Assertion failed: (isDebugValue() && "Must be a debug value instruction."), function setDebugValueUndef, file MachineInstr.h, line 2004.`
- **Same stack trace pattern**:
  ```
  #9  llvm::MachineInstr::setDebugValueUndef()
  #10 llvm::BranchFolder::HoistCommonCodeInSuccs(llvm::MachineBasicBlock*)::::operator()
  #11 llvm::BranchFolder::HoistCommonCodeInSuccs(llvm::MachineBasicBlock*)
  #12 llvm::BranchFolder::OptimizeFunction(...)
  ```

### Impact Assessment
This demonstrates the bug is **systematic** and affects:
- ✅ Polly ISL files (`isl_transitive_closure.c`, `isl_scheduler.c`)  
- ✅ Clangd C++ files (`CompletionModel.cpp`)
- ❌ **Self-hosted builds completely broken**

The bug triggers during the Control Flow Optimizer pass when:
1. Compiling with `-O2 -g` (optimization + debug info)
2. BranchFolder attempts to hoist common code
3. `setDebugValueUndef()` is called on non-debug instructions

### Workaround Confirmation
Excluding Polly from the build only delays the crash to the next affected file. The real solutions are:
- Use a stable clang version instead of the development `/opt/llvm-git/bin/clang++`
- Build with `-O1` or `-O0` 
- Build without debug info (`-g`)

This is a **high-priority regression** preventing self-hosted LLVM builds.

---

### Comment 2 - thejh

I think this might be the same issue, with a reduced reproducer:
```
int drm_gem_get_pages_pages;
_Bool IS_ERR();
void instrument_atomic_read();
void instrument_atomic_write();
void mapping_clear_unevictable() { instrument_atomic_write(0, sizeof(long)); }
int *drm_gem_get_pages() {
  if (IS_ERR())
    goto fail;
  instrument_atomic_read(0, sizeof(long));
  return &drm_gem_get_pages_pages;
fail:
  mapping_clear_unevictable();
}
```
Invocation:
```
[...]/bin/clang -cc1 -O2 -emit-obj drm_gem-92d199.reduced.c  -debug-info-kind=constructor
```

This also crashes with assertion:
```
$ /usr/local/google/home/jannh/git/foreign/llvmp-build-debug/bin/clang -cc1 -O2 -emit-obj drm_gem-92d199.reduced.c  -debug-info-kind=constructor
drm_gem-92d199.reduced.c:5:59: warning: passing arguments to 'instrument_atomic_write' without a prototype is deprecated in all versions of C and is not supported in C23 [-Wdeprecated-non-prototype]
    5 | void mapping_clear_unevictable() { instrument_atomic_write(0, sizeof(long)); }
      |                                                           ^
drm_gem-92d199.reduced.c:9:25: warning: passing arguments to 'instrument_atomic_read' without a prototype is deprecated in all versions of C and is not supported in C23 [-Wdeprecated-non-prototype]
    9 |   instrument_atomic_read(0, sizeof(long));
      |                         ^
drm_gem-92d199.reduced.c:13:1: warning: non-void function does not return a value in all control paths [-Wreturn-type]
   13 | }
      | ^
clang: /usr/local/google/home/jannh/git/foreign/llvm-project/llvm/include/llvm/CodeGen/MachineInstr.h:2004: void llvm::MachineInstr::setDebugValueUndef(): Assertion `isDebugValue() && "Must be a debug value instruction."' failed.
PLEASE submit a bug report to https://bugs.llvm.org/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /usr/local/google/home/jannh/git/foreign/llvmp-build-debug/bin/clang -cc1 -O2 -emit-obj drm_gem-92d199.reduced.c -debug-info-kind=constructor
1.      <eof> parser at end of file
2.      Code generation
3.      Running pass 'Function Pass Manager' on module 'drm_gem-92d199.reduced.c'.
4.      Running pass 'Control Flow Optimizer' on function '@drm_gem_get_pages'
 #0 0x0000563542434b7d llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /usr/local/google/home/jannh/git/foreign/llvm-project/llvm/lib/Support/Unix/Signals.inc:834:11
 #1 0x00005635424350bb PrintStackTraceSignalHandler(void*) /usr/local/google/home/jannh/git/foreign/llvm-project/llvm/lib/Support/Unix/Signals.inc:918:1
 #2 0x0000563542432fbf llvm::sys::RunSignalHandlers() /usr/local/google/home/jannh/git/foreign/llvm-project/llvm/lib/Support/Signals.cpp:104:5
 #3 0x0000563542435759 SignalHandler(int, siginfo_t*, void*) /usr/local/google/home/jannh/git/foreign/llvm-project/llvm/lib/Support/Unix/Signals.inc:426:38
 #4 0x00007ff4bfa49df0 (/lib/x86_64-linux-gnu/libc.so.6+0x3fdf0)
 #5 0x00007ff4bfa9e95c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #6 0x00007ff4bfa49cc2 raise ./signal/../sysdeps/posix/raise.c:27:6
 #7 0x00007ff4bfa324ac abort ./stdlib/abort.c:81:3
 #8 0x00007ff4bfa32420 __assert_perror_fail ./assert/assert-perr.c:31:1
 #9 0x00005635410ba398 llvm::MachineInstr::setDebugValueUndef() /usr/local/google/home/jannh/git/foreign/llvm-project/llvm/include/llvm/CodeGen/MachineInstr.h:0:5
#10 0x00005635410b5242 llvm::BranchFolder::HoistCommonCodeInSuccs(llvm::MachineBasicBlock*)::$_0::operator()(llvm::MachineInstrBundleIterator<llvm::MachineInstr, false>) const /usr/local/google/home/jannh/git/foreign/llvm-project/llvm/lib/CodeGen/BranchFolding.cpp:2108:7
#11 0x00005635410b4626 llvm::BranchFolder::HoistCommonCodeInSuccs(llvm::MachineBasicBlock*) /usr/local/google/home/jannh/git/foreign/llvm-project/llvm/lib/CodeGen/BranchFolding.cpp:2126:9
#12 0x00005635410ade33 llvm::BranchFolder::HoistCommonCode(llvm::MachineFunction&) /usr/local/google/home/jannh/git/foreign/llvm-project/llvm/lib/CodeGen/BranchFolding.cpp:1813:19
#13 0x00005635410ac961 llvm::BranchFolder::OptimizeFunction(llvm::MachineFunction&, llvm::TargetInstrInfo const*, llvm::TargetRegisterInfo const*, llvm::MachineLoopInfo*, bool) /usr/local/google/home/jannh/git/foreign/llvm-project/llvm/lib/CodeGen/BranchFolding.cpp:243:34
#14 0x00005635410b550b (anonymous namespace)::BranchFolderLegacy::runOnMachineFunction(llvm::MachineFunction&) /usr/local/google/home/jannh/git/foreign/llvm-project/llvm/lib/CodeGen/BranchFolding.cpp:162:3
#15 0x000056354134d5d2 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) /usr/local/google/home/jannh/git/foreign/llvm-project/llvm/lib/CodeGen/MachineFunctionPass.cpp:108:8
#16 0x0000563541c2feff llvm::FPPassManager::runOnFunction(llvm::Function&) /usr/local/google/home/jannh/git/foreign/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:1398:23
#17 0x0000563541c3456b llvm::FPPassManager::runOnModule(llvm::Module&) /usr/local/google/home/jannh/git/foreign/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:1444:16
#18 0x0000563541c30729 (anonymous namespace)::MPPassManager::runOnModule(llvm::Module&) /usr/local/google/home/jannh/git/foreign/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:1513:23
#19 0x0000563541c302ef llvm::legacy::PassManagerImpl::run(llvm::Module&) /usr/local/google/home/jannh/git/foreign/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:531:16
#20 0x0000563541c34811 llvm::legacy::PassManager::run(llvm::Module&) /usr/local/google/home/jannh/git/foreign/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:1640:3
#21 0x00005635431e5140 (anonymous namespace)::EmitAssemblyHelper::RunCodegenPipeline(clang::BackendAction, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>&, std::unique_ptr<llvm::ToolOutputFile, std::default_delete<llvm::ToolOutputFile>>&) /usr/local/google/home/jannh/git/foreign/llvm-project/clang/lib/CodeGen/BackendUtil.cpp:1254:19
#22 0x00005635431ded0f (anonymous namespace)::EmitAssemblyHelper::emitAssembly(clang::BackendAction, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) /usr/local/google/home/jannh/git/foreign/llvm-project/clang/lib/CodeGen/BackendUtil.cpp:1280:7
#23 0x00005635431de264 clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) /usr/local/google/home/jannh/git/foreign/llvm-project/clang/lib/CodeGen/BackendUtil.cpp:1443:3
#24 0x00005635432051ea clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) /usr/local/google/home/jannh/git/foreign/llvm-project/clang/lib/CodeGen/CodeGenAction.cpp:316:3
#25 0x0000563545ef38eb clang::ParseAST(clang::Sema&, bool, bool) /usr/local/google/home/jannh/git/foreign/llvm-project/clang/lib/Parse/ParseAST.cpp:190:12
#26 0x0000563543adbab9 clang::ASTFrontendAction::ExecuteAction() /usr/local/google/home/jannh/git/foreign/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1344:1
#27 0x0000563543208e6d clang::CodeGenAction::ExecuteAction() /usr/local/google/home/jannh/git/foreign/llvm-project/clang/lib/CodeGen/CodeGenAction.cpp:1112:5
#28 0x0000563543adb506 clang::FrontendAction::Execute() /usr/local/google/home/jannh/git/foreign/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1226:7
#29 0x00005635439fb333 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) /usr/local/google/home/jannh/git/foreign/llvm-project/clang/lib/Frontend/CompilerInstance.cpp:1056:23
#30 0x0000563543cb313a clang::ExecuteCompilerInvocation(clang::CompilerInstance*) /usr/local/google/home/jannh/git/foreign/llvm-project/clang/lib/FrontendTool/ExecuteCompilerInvocation.cpp:299:8
#31 0x00005635404cb8b7 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) /usr/local/google/home/jannh/git/foreign/llvm-project/clang/tools/driver/cc1_main.cpp:297:13
#32 0x00005635404bf08e ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) /usr/local/google/home/jannh/git/foreign/llvm-project/clang/tools/driver/driver.cpp:223:5
#33 0x00005635404bde67 clang_main(int, char**, llvm::ToolContext const&) /usr/local/google/home/jannh/git/foreign/llvm-project/clang/tools/driver/driver.cpp:264:5
#34 0x00005635404f18d5 main /usr/local/google/home/jannh/git/foreign/llvmp-build-debug/tools/clang/tools/driver/clang-driver.cpp:17:3
#35 0x00007ff4bfa33ca8 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#36 0x00007ff4bfa33d65 call_init ./csu/../csu/libc-start.c:128:20
#37 0x00007ff4bfa33d65 __libc_start_main ./csu/../csu/libc-start.c:347:5
#38 0x00005635404bd921 _start (/usr/local/google/home/jannh/git/foreign/llvmp-build-debug/bin/clang+0x5a7c921)
Aborted (core dumped)
```
It looks like the crash happens on a `DBG_LABEL` instruction, so a confusion between `DBG_LABEL` and `DBG_VALUE`:
```
(gdb) frame 5
#5  0x000055555bbcd398 in llvm::MachineInstr::setDebugValueUndef (this=0x55556381a678)
    at /usr/local/google/home/jannh/git/foreign/llvm-project/llvm/include/llvm/CodeGen/MachineInstr.h:2004
2004        assert(isDebugValue() && "Must be a debug value instruction.");
(gdb) print this->dump()
  DBG_LABEL "fail", debug-location !25; drm_gem-92d199.reduced.c:11:1
```

---

### Comment 3 - thejh

Ah, looks like the issue I hit is fixed in latest `main`.

---

### Comment 4 - Sharjeel-Khan

I believe https://github.com/llvm/llvm-project/commit/fbf6271c7da20356d7b34583b3711b4126ca1dbb and https://github.com/llvm/llvm-project/commit/5dab1fa1fa174085a9f265ff25763a31af97c9e3 together fixed it based on https://github.com/llvm/llvm-project/pull/149999

---

