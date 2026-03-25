# Crash when using __hip_atomic_fetch_min/max with volatile float*

**Author:** misos1
**URL:** https://github.com/llvm/llvm-project/issues/155178

## Body

Full `a.cpp` content:

``` c++
__global__ void kernel(volatile float *f)
{
	__hip_atomic_fetch_min(f, 1, __ATOMIC_RELAXED, __HIP_MEMORY_SCOPE_AGENT);
	__hip_atomic_fetch_max(f, 1, __ATOMIC_RELAXED, __HIP_MEMORY_SCOPE_AGENT);
}
```

Run with:

```
clang++ -x hip -O3 --offload-arch=gfx1100 a.cpp
```

<details>
<summary>Stack dump</summary>

```
Stack dump:
0.	Program arguments: /usr/lib/llvm-22/bin/clang -cc1 -triple amdgcn-amd-amdhsa -aux-triple x86_64-pc-linux-gnu -O3 -Werror=atomic-alignment -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name a.cpp -mrelocation-model pic -pic-level 2 -fhalf-no-semantic-interposition -mframe-pointer=none -fno-rounding-math -mconstructor-aliases -aux-target-cpu x86-64 -fcuda-is-device -fno-threadsafe-statics -mllvm -amdgpu-internalize-symbols -fcuda-allow-variadic-functions -fvisibility=hidden -fapply-global-visibility-to-externs -mlink-builtin-bitcode /opt/rocm/amdgcn/bitcode/ocml.bc -mlink-builtin-bitcode /opt/rocm/amdgcn/bitcode/ockl.bc -mlink-builtin-bitcode /opt/rocm/amdgcn/bitcode/oclc_daz_opt_off.bc -mlink-builtin-bitcode /opt/rocm/amdgcn/bitcode/oclc_unsafe_math_off.bc -mlink-builtin-bitcode /opt/rocm/amdgcn/bitcode/oclc_finite_only_off.bc -mlink-builtin-bitcode /opt/rocm/amdgcn/bitcode/oclc_correctly_rounded_sqrt_on.bc -mlink-builtin-bitcode /opt/rocm/amdgcn/bitcode/oclc_wavefrontsize64_off.bc -mlink-builtin-bitcode /opt/rocm/amdgcn/bitcode/oclc_isa_version_1100.bc -mlink-builtin-bitcode /opt/rocm/amdgcn/bitcode/oclc_abi_version_600.bc -target-cpu gfx1100 -debugger-tuning=gdb -fdebug-compilation-dir=/home/a -fcoverage-compilation-dir=/home/a -resource-dir /usr/lib/llvm-22/lib/clang/22 -internal-isystem /usr/lib/llvm-22/lib/clang/22/include/cuda_wrappers -idirafter /opt/rocm/include -include __clang_hip_runtime_wrapper.h -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/x86_64-linux-gnu/c++/13 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/backward -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/x86_64-linux-gnu/c++/13 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/backward -internal-isystem /usr/lib/llvm-22/lib/clang/22/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -internal-isystem /usr/lib/llvm-22/lib/clang/22/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -fdeprecated-macro -fno-autolink -ferror-limit 19 -fmessage-length=150 -fhip-new-launch-api -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -vectorize-loops -vectorize-slp -cuid=b418a93f1fff3131 -fcuda-allow-variadic-functions -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/a-gfx1100-cf271d.o -x hip a.cpp
1.	<eof> parser at end of file
2.	Code generation
3.	Running pass 'CallGraph Pass Manager' on module 'a.cpp'.
4.	Running pass 'Branch relaxation pass' on function '@_Z6kernelPVf'
 #0 0x00007dd4fc9d2d3f llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/usr/lib/llvm-22/bin/../lib/libLLVM.so.22.0+0xbd2d3f)
 #1 0x00007dd4fc9d05f7 llvm::sys::RunSignalHandlers() (/usr/lib/llvm-22/bin/../lib/libLLVM.so.22.0+0xbd05f7)
 #2 0x00007dd4fc9d3afa (/usr/lib/llvm-22/bin/../lib/libLLVM.so.22.0+0xbd3afa)
 #3 0x00007dd4fb645330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x00007dd4ff03c02d (/usr/lib/llvm-22/bin/../lib/libLLVM.so.22.0+0x323c02d)
 #5 0x00007dd4ff0540d2 (/usr/lib/llvm-22/bin/../lib/libLLVM.so.22.0+0x32540d2)
 #6 0x00007dd4fcc769d6 (/usr/lib/llvm-22/bin/../lib/libLLVM.so.22.0+0xe769d6)
 #7 0x00007dd4fcc78950 (/usr/lib/llvm-22/bin/../lib/libLLVM.so.22.0+0xe78950)
 #8 0x00007dd4fce03379 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (/usr/lib/llvm-22/bin/../lib/libLLVM.so.22.0+0x1003379)
 #9 0x00007dd4fcb3b0cf llvm::FPPassManager::runOnFunction(llvm::Function&) (/usr/lib/llvm-22/bin/../lib/libLLVM.so.22.0+0xd3b0cf)
#10 0x00007dd4fe0d5797 (/usr/lib/llvm-22/bin/../lib/libLLVM.so.22.0+0x22d5797)
#11 0x00007dd4fcb3b82d llvm::legacy::PassManagerImpl::run(llvm::Module&) (/usr/lib/llvm-22/bin/../lib/libLLVM.so.22.0+0xd3b82d)
#12 0x00007dd5060a1d94 clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1ca1d94)
#13 0x00007dd50639aee6 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1f9aee6)
#14 0x00007dd504f883a9 clang::ParseAST(clang::Sema&, bool, bool) (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0xb883a9)
#15 0x00007dd506b8d79a clang::FrontendAction::Execute() (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x278d79a)
#16 0x00007dd506b067d4 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x27067d4)
#17 0x00007dd506c15a7c clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x2815a7c)
#18 0x000056e42cd35f5c cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/usr/lib/llvm-22/bin/clang+0x13f5c)
#19 0x000056e42cd32c84 (/usr/lib/llvm-22/bin/clang+0x10c84)
#20 0x000056e42cd3206b clang_main(int, char**, llvm::ToolContext const&) (/usr/lib/llvm-22/bin/clang+0x1006b)
#21 0x000056e42cd40083 main (/usr/lib/llvm-22/bin/clang+0x1e083)
#22 0x00007dd4fb62a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#23 0x00007dd4fb62a28b call_init ./csu/../csu/libc-start.c:128:20
#24 0x00007dd4fb62a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#25 0x000056e42cd309c5 _start (/usr/lib/llvm-22/bin/clang+0xe9c5)
clang++: error: unable to execute command: Segmentation fault (core dumped)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
Ubuntu clang version 22.0.0 (++20250812082130+296e057d0bf2-1~exp1~20250812082146.1091)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/lib/llvm-22/bin
clang++: note: diagnostic msg: Error generating preprocessed source(s).
```

</details>

`rocm/noble,now 6.4.3.60403-128~24.04 amd64`


