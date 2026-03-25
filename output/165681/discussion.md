# Crash during shader compliation on AMD GPU.

**Author:** ShadowReaper420
**URL:** https://github.com/llvm/llvm-project/issues/165681
**Status:** Closed
**Labels:** llvm:codegen, crash
**Closed Date:** 2026-01-17T12:24:38Z

## Body

Hi to be 100% honest I have no idea if this is actually your bug or not I asked for support at the Azul project and they told me to come here.

[hs_err_pid230611.log](https://github.com/user-attachments/files/23229008/hs_err_pid230611.log)

If you ask me to give a more detailed explanation I not sure I'll be able to I barely know what these projects are but I will try my best. i am just trying to get a funny shaderpack working for a funny MC modpack.

## Comments

### Comment 1 - frederick-vs-ja

Related parts seem to be:

```
#
# A fatal error has been detected by the Java Runtime Environment:
#
#  SIGSEGV (0xb) at pc=0x00007f60d6f59f8d, pid=230611, tid=232724
#
# JRE version: OpenJDK Runtime Environment Zulu17.52+17-CA (17.0.12+7) (build 17.0.12+7-LTS)
# Java VM: OpenJDK 64-Bit Server VM Zulu17.52+17-CA (17.0.12+7-LTS, mixed mode, sharing, tiered, compressed oops, compressed class ptrs, g1 gc, linux-amd64)
# Problematic frame:
# C  [libLLVM.so.21.1+0x4559f8d]  llvm::SIInstrInfo::getInstSizeInBytes(llvm::MachineInstr const&) const+0x3d
#
# Core dump will be written. Default location: Core dumps may be processed with "/nix/store/gkpacfwizaq6z3dnshgaxcn6jhk77jjs-systemd/lib/systemd/systemd-coredump %P %u %g %s %t %c %h %d %F" (or dumping to /run/media/flugel/Modding/instances/GTNH-2025-10-26+209-mmcprism-new-java/.minecraft/core.230611)
#
# If you would like to submit a bug report, please visit:
#   http://www.azul.com/support/
#
```

```
Current thread is native thread

Stack: [0x00007f5f1054a000,0x00007f5f10d4a000],  sp=0x00007f5f10d40420,  free space=8153k
Native frames: (J=compiled Java code, j=interpreted, Vv=VM code, C=native code)
C  [libLLVM.so.21.1+0x4559f8d]  llvm::SIInstrInfo::getInstSizeInBytes(llvm::MachineInstr const&) const+0x3d
C  [libLLVM.so.21.1+0x1105f09]  (anonymous namespace)::BranchRelaxation::run(llvm::MachineFunction&)+0x2b9
C  [libLLVM.so.21.1+0x1107e1d]  (anonymous namespace)::BranchRelaxationLegacy::runOnMachineFunction(llvm::MachineFunction&)+0xcd
C  [libLLVM.so.21.1+0x1347ee7]  llvm::MachineFunctionPass::runOnFunction(llvm::Function&)+0x1e7
C  [libLLVM.so.21.1+0xf3aa23]  llvm::FPPassManager::runOnFunction(llvm::Function&)+0x713
C  [libLLVM.so.21.1+0x2efbf9b]  (anonymous namespace)::CGPassManager::runOnModule(llvm::Module&)+0x3cb
C  [libLLVM.so.21.1+0xf39cbb]  llvm::legacy::PassManagerImpl::run(llvm::Module&)+0x47b
C  [libgallium-25.2.5.so+0xbea3c4]  ac_compile_module_to_elf+0x24
C  [libgallium-25.2.5.so+0xb0cd20]  si_compile_llvm+0x180
C  [libgallium-25.2.5.so+0xb0e14b]  si_llvm_compile_shader+0x4cb
C  [libgallium-25.2.5.so+0xa93b97]  si_compile_shader+0x8d7
C  [libgallium-25.2.5.so+0xac8d2d]  si_init_shader_selector_async(void*, void*, int)+0x26d
C  [libgallium-25.2.5.so+0x609e61]  util_queue_thread_func+0x161
C  [libgallium-25.2.5.so+0x641b78]  impl_thrd_routine+0x18
C  [libc.so.6+0x9a97a]  start_thread+0x2aa
```

---

### Comment 2 - ShadowReaper420

Is there any chance of this getting fixed? I am still having the problem.

---

### Comment 3 - ShadowReaper420

I can no longer replicate this idk if something changed on amd's side or what but I'm guessing this is fixed now so I'm closing this.

---

