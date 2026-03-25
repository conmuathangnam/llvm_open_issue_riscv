# [HLSL] PHI Node operand in @llvm.dx.resource.casthandle causing assertion error in DXIL Op Lowering

**Author:** Icohedron
**URL:** https://github.com/llvm/llvm-project/issues/161754
**Status:** Closed
**Labels:** crash, llvm:GVN
**Closed Date:** 2025-10-10T19:38:25Z

## Body

DML shaders of the name `ArgOpMultiDimsArg*` (16 total) are failing to compile due to an assertion error being triggered by [line 181 of DXILOpLowering.cpp](https://github.com/llvm/llvm-project/blob/ab2c4a0ee1b4710c0f610292b6e9dcb45839c25f/llvm/lib/Target/DirectX/DXILOpLowering.cpp#L181).

It appears to be caused by a PHI Node appearing as an operand of `@llvm.dx.resource.casthandle.s_dx.types.Handles.tdx.RawBuffer_f16_1_0t`

```llvm
  %35 = phi target("dx.RawBuffer", half, 1, 0) [ %1, %entry.if.end.i_crit_edge ], [ %1, %if.then.i ]
  ...
  %46 = call %dx.types.Handle @llvm.dx.resource.casthandle.s_dx.types.Handles.tdx.RawBuffer_f16_1_0t(target("dx.RawBuffer", half, 1, 0) %35)
```

This shader used to compile successfully in the past. Running git bisect reveals the first "bad" commit:
```
87e1da3f2e95fef052e2f7b4167ede0838be24d0 is the first bad commit
commit 87e1da3f2e95fef052e2f7b4167ede0838be24d0
Author: Finn Plummer <mail@inbelic.dev>
Date:   Mon Sep 22 12:18:28 2025 -0700

    [HLSL] Mark `GroupMemoryBarrierWithGroupSync` as `convergent` (#160175)

    `GroupMemoryBarrierWithGroupSync` is required to be marked as convergent
    so that it can't generate duplicate calls or be moved to identical
    control flow.

    Without it, we generate undefined behaviour during optimization. For
    instance: https://godbolt.org/z/9j3vsq1h3.

    Testing that the convergent attribute is added is sufficient. There
    already exists testing,
    [here](https://github.com/inbelic/llvm-project/blob/main/llvm/test/Transforms/SimplifyCFG/attr-convergent.ll),
    that it will not be moved as described in the above link.

clang/lib/Headers/hlsl/hlsl_alias_intrinsics.h                       | 2 +-
clang/test/CodeGenHLSL/builtins/GroupMemoryBarrierWithGroupSync.hlsl | 2 +-
llvm/include/llvm/IR/IntrinsicsDirectX.td                            | 3 ++-
llvm/include/llvm/IR/IntrinsicsSPIRV.td                              | 3 ++-
4 files changed, 6 insertions(+), 4 deletions(-)
bisect found first bad commit
```

## Sample Reproduction

```
❯ /workspace/llvm-project/build/bin/clang-dxc /mnt/c/workspace/DirectML/Product/Shaders/Generated/ArgOpMultiDimsArgMax_float16_native_accum32_uint32_native_8.hlsl -E CSMain -T cs_6_2  -enable-16bit-types  -O3 -D DXC_COMPILER=1 -D __SHADER_TARGET_MAJOR=6 -D __SHADER_TARGET_MINOR=2 -Vd -I /mnt/c/workspace/DirectML/Product/Shaders/                                            clang-dxc: warning: argument unused during compilation: '-Vd' [-Wunused-command-line-argument]
In file included from /mnt/c/workspace/DirectML/Product/Shaders/Generated/ArgOpMultiDimsArgMax_float16_native_accum32_uint32_native_8.hlsl:51:
In file included from /mnt/c/workspace/DirectML/Product/Shaders/ArgOpMultiDims.hlsl:13:
/mnt/c/workspace/DirectML/Product/Shaders/ShaderIncludes.hlsl:1721:12: warning: 'max<int, 4U>' is deprecated: In 202x mismatched vector/scalar lowering for max is
      deprecated. Explicitly cast parameters. [-Wdeprecated-declarations]
 1721 |     return max(a, s);
      |            ^
/workspace/llvm-project/build/lib/clang/22/include/hlsl/hlsl_compat_overloads.h:404:1: note: 'max<int, 4U>' has been explicitly marked deprecated here
  404 | _DXC_DEPRECATED_VEC_SCALAR_FN(max)
      | ^
/workspace/llvm-project/build/lib/clang/22/include/hlsl/hlsl_compat_overloads.h:28:5: note: expanded from macro '_DXC_DEPRECATED_VEC_SCALAR_FN'
   28 |   [[deprecated("In 202x mismatched vector/scalar lowering for " #fn " is "     \
      |     ^
In file included from /mnt/c/workspace/DirectML/Product/Shaders/Generated/ArgOpMultiDimsArgMax_float16_native_accum32_uint32_native_8.hlsl:51:
In file included from /mnt/c/workspace/DirectML/Product/Shaders/ArgOpMultiDims.hlsl:13:
/mnt/c/workspace/DirectML/Product/Shaders/ShaderIncludes.hlsl:1727:17: warning: 'max<int, 4U>' is deprecated: In 202x mismatched vector/scalar lowering for max is
      deprecated. Explicitly cast parameters. [-Wdeprecated-declarations]
 1727 |     output[0] = max(a[0], s);
      |                 ^
/workspace/llvm-project/build/lib/clang/22/include/hlsl/hlsl_compat_overloads.h:404:1: note: 'max<int, 4U>' has been explicitly marked deprecated here
  404 | _DXC_DEPRECATED_VEC_SCALAR_FN(max)
      | ^
/workspace/llvm-project/build/lib/clang/22/include/hlsl/hlsl_compat_overloads.h:28:5: note: expanded from macro '_DXC_DEPRECATED_VEC_SCALAR_FN'
   28 |   [[deprecated("In 202x mismatched vector/scalar lowering for " #fn " is "     \
      |     ^
In file included from /mnt/c/workspace/DirectML/Product/Shaders/Generated/ArgOpMultiDimsArgMax_float16_native_accum32_uint32_native_8.hlsl:51:
In file included from /mnt/c/workspace/DirectML/Product/Shaders/ArgOpMultiDims.hlsl:13:
/mnt/c/workspace/DirectML/Product/Shaders/ShaderIncludes.hlsl:1728:17: warning: 'max<int, 4U>' is deprecated: In 202x mismatched vector/scalar lowering for max is
      deprecated. Explicitly cast parameters. [-Wdeprecated-declarations]
 1728 |     output[1] = max(a[1], s);
      |                 ^
/workspace/llvm-project/build/lib/clang/22/include/hlsl/hlsl_compat_overloads.h:404:1: note: 'max<int, 4U>' has been explicitly marked deprecated here
  404 | _DXC_DEPRECATED_VEC_SCALAR_FN(max)
      | ^
/workspace/llvm-project/build/lib/clang/22/include/hlsl/hlsl_compat_overloads.h:28:5: note: expanded from macro '_DXC_DEPRECATED_VEC_SCALAR_FN'
   28 |   [[deprecated("In 202x mismatched vector/scalar lowering for " #fn " is "     \
      |     ^
clang-dxc: /workspace/llvm-project/llvm/include/llvm/Support/Casting.h:573: decltype(auto) llvm::cast(From *) [To = llvm::CallInst, From = llvm::Value]: Assertion `isa<To>(Val) && "cast<Ty>() argument of incompatible type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /workspace/llvm-project/build/bin/clang-dxc /mnt/c/workspace/DirectML/Product/Shaders/Generated/ArgOpMultiDimsArgMax_float16_native_accum32_uint32_native_8.hlsl -E CSMain -T cs_6_2 -enable-16bit-types -O3 -D DXC_COMPILER=1 -D __SHADER_TARGET_MAJOR=6 -D __SHADER_TARGET_MINOR=2 -Vd -I /mnt/c/workspace/DirectML/Product/Shaders/
1.      <eof> parser at end of file
2.      Code generation
3.      Running pass 'DXIL Op Lowering' on module '/mnt/c/workspace/DirectML/Product/Shaders/Generated/ArgOpMultiDimsArgMax_float16_native_accum32_uint32_native_8.hlsl'.
 #0 0x00005a1af7610f2b llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /workspace/llvm-project/llvm/lib/Support/Unix/Signals.inc:834:13
 #1 0x00005a1af760e8c3 llvm::sys::RunSignalHandlers() /workspace/llvm-project/llvm/lib/Support/Signals.cpp:105:18
 #2 0x00005a1af757a9e0 (anonymous namespace)::CrashRecoveryContextImpl::HandleCrash(int, unsigned long) /workspace/llvm-project/llvm/lib/Support/CrashRecoveryContext.cpp:73:5
 #3 0x00005a1af757a9e0 CrashRecoverySignalHandler(int) /workspace/llvm-project/llvm/lib/Support/CrashRecoveryContext.cpp:390:51
 #4 0x00007fbe4e4419c0 __restore_rt (/nix/store/lmn7lwydprqibdkghw7wgcn21yhllz13-glibc-2.40-66/lib/libc.so.6+0x419c0)
 #5 0x00007fbe4e49cf3c __pthread_kill_implementation (/nix/store/lmn7lwydprqibdkghw7wgcn21yhllz13-glibc-2.40-66/lib/libc.so.6+0x9cf3c)
 #6 0x00007fbe4e44190e gsignal (/nix/store/lmn7lwydprqibdkghw7wgcn21yhllz13-glibc-2.40-66/lib/libc.so.6+0x4190e)
 #7 0x00007fbe4e428942 abort (/nix/store/lmn7lwydprqibdkghw7wgcn21yhllz13-glibc-2.40-66/lib/libc.so.6+0x28942)
 #8 0x00007fbe4e42885e _nl_load_domain.cold (/nix/store/lmn7lwydprqibdkghw7wgcn21yhllz13-glibc-2.40-66/lib/libc.so.6+0x2885e)
 #9 0x00007fbe4e4396f7 (/nix/store/lmn7lwydprqibdkghw7wgcn21yhllz13-glibc-2.40-66/lib/libc.so.6+0x396f7)
#10 0x00005a1af62926ee (anonymous namespace)::OpLowerer::lowerIntrinsics() /workspace/llvm-project/llvm/lib/Target/DirectX/DXILOpLowering.cpp:0:0
#11 0x00005a1af62978fd (anonymous namespace)::DXILOpLoweringLegacy::runOnModule(llvm::Module&) /workspace/llvm-project/llvm/lib/Target/DirectX/DXILOpLowering.cpp:994:42
#12 0x00005a1af70b0674 (anonymous namespace)::MPPassManager::runOnModule(llvm::Module&) /workspace/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:0:27
#13 0x00005a1af70b0674 llvm::legacy::PassManagerImpl::run(llvm::Module&) /workspace/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:531:44
#14 0x00005a1af7d5fd50 (anonymous namespace)::EmitAssemblyHelper::RunCodegenPipeline(clang::BackendAction, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>&, std::unique_ptr<llvm::ToolOutputFile, std::default_delete<llvm::ToolOutputFile>>&) /workspace/llvm-project/clang/lib/CodeGen/BackendUtil.cpp:1259:9
#15 0x00005a1af7d5fd50 (anonymous namespace)::EmitAssemblyHelper::emitAssembly(clang::BackendAction, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) /workspace/llvm-project/clang/lib/CodeGen/BackendUtil.cpp:1282:3
#16 0x00005a1af7d5fd50 clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) /workspace/llvm-project/clang/lib/CodeGen/BackendUtil.cpp:1452:13
#17 0x00005a1af7d73a18 std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>::~unique_ptr() /nix/store/eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee-gcc-14.3.0/include/c++/14.3.0/bits/unique_ptr.h:398:6
#18 0x00005a1af7d73a18 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) /workspace/llvm-project/clang/lib/CodeGen/CodeGenAction.cpp:312:3
#19 0x00005a1af989e3e9 __gnu_cxx::__normal_iterator<std::unique_ptr<clang::TemplateInstantiationCallback, std::default_delete<clang::TemplateInstantiationCallback>>*, std::vector<std::unique_ptr<clang::TemplateInstantiationCallback, std::default_delete<clang::TemplateInstantiationCallback>>, std::allocator<std::unique_ptr<clang::TemplateInstantiationCallback, std::default_delete<clang::TemplateInstantiationCallback>>>>>::__normal_iterator(std::unique_ptr<clang::TemplateInstantiationCallback, std::default_delete<clang::TemplateInstantiationCallback>>* const&) /nix/store/eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee-gcc-14.3.0/include/c++/14.3.0/bits/stl_iterator.h:1068:20
#20 0x00005a1af989e3e9 std::vector<std::unique_ptr<clang::TemplateInstantiationCallback, std::default_delete<clang::TemplateInstantiationCallback>>, std::allocator<std::unique_ptr<clang::TemplateInstantiationCallback, std::default_delete<clang::TemplateInstantiationCallback>>>>::begin() /nix/store/eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee-gcc-14.3.0/include/c++/14.3.0/bits/stl_vector.h:887:16
#21 0x00005a1af989e3e9 void clang::finalize<std::vector<std::unique_ptr<clang::TemplateInstantiationCallback, std::default_delete<clang::TemplateInstantiationCallback>>, std::allocator<std::unique_ptr<clang::TemplateInstantiationCallback, std::default_delete<clang::TemplateInstantiationCallback>>>>>(std::vector<std::unique_ptr<clang::TemplateInstantiationCallback, std::default_delete<clang::TemplateInstantiationCallback>>, std::allocator<std::unique_ptr<clang::TemplateInstantiationCallback, std::default_delete<clang::TemplateInstantiationCallback>>>>&, clang::Sema const&) /workspace/llvm-project/clang/include/clang/Sema/TemplateInstCallback.h:54:16
#22 0x00005a1af989e3e9 clang::ParseAST(clang::Sema&, bool, bool) /workspace/llvm-project/clang/lib/Parse/ParseAST.cpp:190:3
#23 0x00005a1af82b86df clang::HLSLFrontendAction::ExecuteAction() /workspace/llvm-project/clang/lib/Frontend/FrontendActions.cpp:0:35
#24 0x00005a1af827ea57 clang::FrontendAction::Execute() /workspace/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1318:10
#25 0x00005a1af81ea55d llvm::Error::getPtr() const /workspace/llvm-project/llvm/include/llvm/Support/Error.h:278:42
#26 0x00005a1af81ea55d llvm::Error::operator bool() /workspace/llvm-project/llvm/include/llvm/Support/Error.h:241:16
#27 0x00005a1af81ea55d clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) /workspace/llvm-project/clang/lib/Frontend/CompilerInstance.cpp:1007:23
#28 0x00005a1af8373f91 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) /workspace/llvm-project/clang/lib/FrontendTool/ExecuteCompilerInvocation.cpp:310:25
#29 0x00005a1af613921a cc1_main(llvm::ArrayRef<char const*>, char const*, void*) /workspace/llvm-project/clang/tools/driver/cc1_main.cpp:300:15
#30 0x00005a1af6135622 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) /workspace/llvm-project/clang/tools/driver/driver.cpp:225:12
#31 0x00005a1af61379ec clang_main(int, char**, llvm::ToolContext const&)::$_0::operator()(llvm::SmallVectorImpl<char const*>&) const /workspace/llvm-project/clang/tools/driver/driver.cpp:0:12
#32 0x00005a1af61379ec int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::$_0>(long, llvm::SmallVectorImpl<char const*>&) /workspace/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#33 0x00005a1af805b70e clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*,
bool*) const::$_0::operator()() const /workspace/llvm-project/clang/lib/Driver/Job.cpp:436:30
#34 0x00005a1af805b70e void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::$_0>(long) /workspace/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#35 0x00005a1af757a5d9 llvm::function_ref<void ()>::operator()() const /workspace/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:0:12
#36 0x00005a1af757a5d9 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) /workspace/llvm-project/llvm/lib/Support/CrashRecoveryContext.cpp:426:3
#37 0x00005a1af805ace9 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*,bool*) const /workspace/llvm-project/clang/lib/Driver/Job.cpp:436:7
#38 0x00005a1af801be36 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const /workspace/llvm-project/clang/lib/Driver/Compilation.cpp:196:15
#39 0x00005a1af801c0a8 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const /workspace/llvm-project/clang/lib/Driver/Compilation.cpp:246:13
#40 0x00005a1af8035269 llvm::SmallVectorBase<unsigned int>::empty() const /workspace/llvm-project/llvm/include/llvm/ADT/SmallVector.h:82:46
#41 0x00005a1af8035269 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) /workspace/llvm-project/clang/lib/Driver/Driver.cpp:2244:23
#42 0x00005a1af6134fa3 clang_main(int, char**, llvm::ToolContext const&) /workspace/llvm-project/clang/tools/driver/driver.cpp:407:21
#43 0x00005a1af614512f main /workspace/llvm-project/build/tools/clang/tools/driver/clang-driver.cpp:17:10
#44 0x00007fbe4e42a4d8 __libc_start_call_main (/nix/store/lmn7lwydprqibdkghw7wgcn21yhllz13-glibc-2.40-66/lib/libc.so.6+0x2a4d8)
#45 0x00007fbe4e42a59b __libc_start_main@GLIBC_2.2.5 (/nix/store/lmn7lwydprqibdkghw7wgcn21yhllz13-glibc-2.40-66/lib/libc.so.6+0x2a59b)
#46 0x00005a1af6132f25 _start (/workspace/llvm-project/build/bin/clang-dxc+0x2dd2f25)
clang-dxc: error: clang frontend command failed with exit code 134 (use -v to see invocation)
clang version 22.0.0git (git@github.com:llvm/llvm-project 5843ffb14940920f72516dfe18ed657bee23d1c5)
Target: dxilv1.2-unknown-shadermodel6.2-compute
Thread model: posix
InstalledDir: /workspace/llvm-project/build/bin
Build config: +assertions
```

I tried using a test case reducer to get a minimal reproduction of the problematic shader code, but even the minimal version is rather large and potentially reveals too much about the original shader source code to publicly share.

## Comments

### Comment 1 - damyanp

@farzonl to do initial investigation.  

---

### Comment 2 - farzonl

So it looks like what is going on here is SSAUpdater  used by the GVN pass doesn't have a way of knowing that it should ignore creating a phi for `isTokenLikeTy` types.

```gdb
(lldb) expr PHI->dump()
  <badref> = phi target("dx.RawBuffer", half, 1, 0) 
(lldb) bt
* thread #1, queue = 'com.apple.main-thread', stop reason = breakpoint 3.1
  * frame #0: 0x000000010368b9ac clang-dxc`llvm::SSAUpdaterTraits<llvm::SSAUpdater>::CreateEmptyPHI(BB=0x000000011dbd8f20, NumPreds=2, Updater=0x000000016fdedf68) at SSAUpdater.cpp:303:5
    frame #1: 0x0000000103689e60 clang-dxc`llvm::SSAUpdaterImpl<llvm::SSAUpdater>::FindAvailableVals(this=0x000000016fdeda20, BlockList=0x000000016fded678) at SSAUpdaterImpl.h:373:18
    frame #2: 0x0000000103686468 clang-dxc`llvm::SSAUpdaterImpl<llvm::SSAUpdater>::GetValue(this=0x000000016fdeda20, BB=0x000000011dbd8f20) at SSAUpdaterImpl.h:107:5
    frame #3: 0x0000000103685724 clang-dxc`llvm::SSAUpdater::GetValueAtEndOfBlockInternal(this=0x000000016fdedf68, BB=0x000000011dbd8f20) at SSAUpdater.cpp:346:15
    frame #4: 0x0000000103685674 clang-dxc`llvm::SSAUpdater::GetValueAtEndOfBlock(this=0x000000016fdedf68, BB=0x000000011dbd8f20) at SSAUpdater.cpp:93:16
    frame #5: 0x00000001036857c4 clang-dxc`llvm::SSAUpdater::GetValueInMiddleOfBlock(this=0x000000016fdedf68, BB=0x000000011dbd8f20) at SSAUpdater.cpp:101:12
    frame #6: 0x0000000102ea2d54 clang-dxc`ConstructSSAForLoadSet(Load=0x000000011dbe4460, ValuesPerBlock=0x000000016fdeeab8, GVN=0x000000011db51bc8) at GVN.cpp:1145:20
    frame #7: 0x0000000102ea266c clang-dxc`llvm::GVNPass::eliminatePartiallyRedundantLoad(this=0x000000011db51bc8, Load=0x000000011dbe4460, ValuesPerBlock=0x000000016fdeeab8, AvailableLoads=0x000000016fdee5c0, CriticalEdgePredAndLoad=0x000000016fdee530) at GVN.cpp:1635:14
    frame #8: 0x0000000102ea3e54 clang-dxc`llvm::GVNPass::PerformLoadPRE(this=0x000000011db51bc8, Load=0x000000011dbe4460, ValuesPerBlock=0x000000016fdeeab8, UnavailableBlocks=0x000000016fdee8a8) at GVN.cpp:1899:3
    frame #9: 0x0000000102ea50e0 clang-dxc`llvm::GVNPass::processNonLocalLoad(this=0x000000011db51bc8, Load=0x000000011dbe4460) at GVN.cpp:2080:7
    frame #10: 0x0000000102ea7014 clang-dxc`llvm::GVNPass::processLoad(this=0x000000011db51bc8, L=0x000000011dbe4460) at GVN.cpp:2257:12
    frame #11: 0x0000000102ea91a4 clang-dxc`llvm::GVNPass::processInstruction(this=0x000000011db51bc8, I=0x000000011dbe4460) at GVN.cpp:2758:9
    frame #12: 0x0000000102eaa2b4 clang-dxc`llvm::GVNPass::processBlock(this=0x000000011db51bc8, BB=0x000000011dbd8f20) at GVN.cpp:2960:24
    frame #13: 0x0000000102ea9d6c clang-dxc`llvm::GVNPass::iterateOnFunction(this=0x000000011db51bc8, F=0x000000011db62fe8) at GVN.cpp:3252:16
    frame #14: 0x0000000102e9f95c clang-dxc`llvm::GVNPass::runImpl(this=0x000000011db51bc8, F=0x000000011db62fe8, RunAC=0x000000011db57728, RunDT=0x000000011db57698, RunTLI=0x000000011db66b68, RunAA=0x000000011db51348, RunMD=0x000000011dbd5ba8, LI=0x000000011dbc14f8, RunORE=0x000000011db2f8a8, MSSA=0x0000000000000000) at GVN.cpp:2908:22
    frame #15: 0x0000000102e9f50c clang-dxc`llvm::GVNPass::run(this=0x000000011db51bc8, F=0x000000011db62fe8, AM=0x000000016fdf29b0) at GVN.cpp:900:18
    frame #16: 0x0000000103e22650 clang-dxc`llvm::detail::PassModel<llvm::Function, llvm::GVNPass, llvm::AnalysisManager<llvm::Function>>::run(this=0x000000011db51bc0, IR=0x000000011
```

---

### Comment 3 - farzonl

I have a fix just trying to work out what the ir tests will look like the fix was pretty straight forward but is making me think there could be other passes that use SSAUpdater that could be a problem for us:
```diff
diff --git a/llvm/lib/Transforms/Scalar/GVN.cpp b/llvm/lib/Transforms/Scalar/GVN.cpp
index b9b5b5823d78..45da481fab80 100644
--- a/llvm/lib/Transforms/Scalar/GVN.cpp
+++ b/llvm/lib/Transforms/Scalar/GVN.cpp
@@ -1659,6 +1659,9 @@ bool GVNPass::PerformLoadPRE(LoadInst *Load, AvailValInBlkVect &ValuesPerBlock,
   // that we only have to insert *one* load (which means we're basically moving
   // the load, not inserting a new one).
 
+  if(Load->getType()->isTokenLikeTy())
+    return false;
+
   SmallPtrSet<BasicBlock *, 4> Blockers(llvm::from_range, UnavailableBlocks);
 
   // Let's find the first basic block with more than one predecessor.  Walk
```

Will talk with @bogner to see how to proceed.

---

### Comment 4 - farzonl

ok have a repro https://godbolt.org/z/xM69fY8zM
proceeding with a bug fix.

---

### Comment 5 - farzonl

This bug really isn't a backend bug. I think it just shows up there because we don't have the verifier\asserts on as my godbolt link above demostrates. as such i'm going to change the  labels.

---

