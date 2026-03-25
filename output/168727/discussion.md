# Frontend dumps core consistently compiling rocblas in Debian/Ubuntu on `ppc64el`

**Author:** Hyask
**URL:** https://github.com/llvm/llvm-project/issues/168727
**Status:** Open
**Labels:** backend:PowerPC, crash, llvm:SelectionDAG

## Body

The same sources work fine on other architectures like `amd64` or `arm64`.

Please find the Debian unstable build log here: https://buildd.debian.org/status/logs.php?pkg=rocblas&ver=6.4.4-1&arch=ppc64el
Please find the Ubuntu resolute build log here: https://launchpad.net/ubuntu/+source/rocblas/6.4.4-1/+build/31463786

Here is the backtrace:
```
Stack dump:
0.      Program arguments: /usr/lib/llvm-21/bin/clang -cc1 -triple powerpc64le-unknown-linux-gnu -aux-triple amdgcn-amd-amdhsa -O3 -emit-obj -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard
-value-names -main-file-name common_gemm_ex3.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=none -fmath-errno -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu ppc64le -m
abi=ieeelongdouble -mfloat-abi hard -target-abi elfv2 -debug-info-kind=constructor -dwarf-version=5 -debugger-tuning=gdb --compress-debug-sections=zlib -fdebug-compilation-dir=/home/ubuntu/workspace/rocblas/rocbl
as-6.4.4/obj-powerpc64le-linux-gnu/clients/common -fdebug-prefix-map=/home/ubuntu/workspace/rocblas/rocblas-6.4.4=. -fdebug-prefix-map=/home/ubuntu/workspace/rocblas/rocblas-6.4.4=/usr/src/rocblas-6.4.4-1 -fcover
age-compilation-dir=/home/ubuntu/workspace/rocblas/rocblas-6.4.4/obj-powerpc64le-linux-gnu/clients/common -resource-dir /usr/lib/llvm-21/lib/clang/21 -dependency-file CMakeFiles/rocblas_clients_common.dir/blas_ex
/common_gemm_ex3.cpp.o.d -MT clients/common/CMakeFiles/rocblas_clients_common.dir/blas_ex/common_gemm_ex3.cpp.o -sys-header-deps -internal-isystem /usr/lib/llvm-21/lib/clang/21/include/cuda_wrappers -idirafter /u
sr/include -include __clang_hip_runtime_wrapper.h -D BUILD_WITH_TENSILE -D GOOGLE_TEST -D ROCBLAS_INTERNAL_API -D ROCBLAS_NO_DEPRECATED_WARNINGS -D ROCBLAS_TENSILE_LAZY_LOAD=1 -D ROCBLAS_TENSILE_SEPARATE_ARCH=1 -
D ROCM_USE_FLOAT16 -D USE_PROF_API=1 -D __HIP_PLATFORM_AMD__=1 -I /home/ubuntu/workspace/rocblas/rocblas-6.4.4/obj-powerpc64le-linux-gnu/include/rocblas -I /home/ubuntu/workspace/rocblas/rocblas-6.4.4/obj-powerpc
64le-linux-gnu/include/rocblas/internal -I /home/ubuntu/workspace/rocblas/rocblas-6.4.4/clients/common/../include -I /home/ubuntu/workspace/rocblas/rocblas-6.4.4/clients/common/../include/blas1 -I /home/ubuntu/wo
rkspace/rocblas/rocblas-6.4.4/clients/common/../include/blas2 -I /home/ubuntu/workspace/rocblas/rocblas-6.4.4/clients/common/../include/blas3 -I /home/ubuntu/workspace/rocblas/rocblas-6.4.4/clients/common/../incl
ude/blas_ex -I /home/ubuntu/workspace/rocblas/rocblas-6.4.4/clients/common/blas_ex -I /home/ubuntu/workspace/rocblas/rocblas-6.4.4/clients/common/../../library/include -I /home/ubuntu/workspace/rocblas/rocblas-6.
4.4/clients/common/../../library/src/include -I /home/ubuntu/workspace/rocblas/rocblas-6.4.4/clients/common/../../library/src -I /home/ubuntu/workspace/rocblas/rocblas-6.4.4/clients/common -I /home/ubuntu/workspa
ce/rocblas/rocblas-6.4.4/library/src -I /home/ubuntu/workspace/rocblas/rocblas-6.4.4/library/include -I /home/ubuntu/workspace/rocblas/rocblas-6.4.4/library/include/internal -I /home/ubuntu/workspace/rocblas/rocb
las-6.4.4/library/src/include -I /home/ubuntu/workspace/rocblas/rocblas-6.4.4/obj-powerpc64le-linux-gnu/include -I /home/ubuntu/workspace/rocblas/rocblas-6.4.4/library/src/blas3/Tensile -D _FORTIFY_SOURCE=3 -D __
HIP_HCC_COMPAT_MODE__=1 -D NDEBUG -D GTEST_HAS_PTHREAD=1 -internal-isystem /usr/lib/gcc/powerpc64le-linux-gnu/15/../../../../include/c++/15 -internal-isystem /usr/lib/gcc/powerpc64le-linux-gnu/15/../../../../incl
ude/powerpc64le-linux-gnu/c++/15 -internal-isystem /usr/lib/gcc/powerpc64le-linux-gnu/15/../../../../include/c++/15/backward -internal-isystem /usr/lib/gcc/powerpc64le-linux-gnu/15/../../../../include/c++/15 -int
ernal-isystem /usr/lib/gcc/powerpc64le-linux-gnu/15/../../../../include/powerpc64le-linux-gnu/c++/15 -internal-isystem /usr/lib/gcc/powerpc64le-linux-gnu/15/../../../../include/c++/15/backward -internal-isystem /
usr/lib/llvm-21/lib/clang/21/include/ppc_wrappers -internal-isystem /usr/lib/llvm-21/lib/clang/21/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/powerpc64le-linux-gnu/15/../../../../p
owerpc64le-linux-gnu/include -internal-externc-isystem /usr/include/powerpc64le-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -internal-isystem /usr/lib/llvm-21/lib/clang/21/
include/ppc_wrappers -internal-isystem /usr/lib/llvm-21/lib/clang/21/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/powerpc64le-linux-gnu/15/../../../../powerpc64le-linux-gnu/include
-internal-externc-isystem /usr/include/powerpc64le-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -fmacro-prefix-map=/home/ubuntu/workspace/rocblas/rocblas-6.4.4=. -fcoverage-
prefix-map=/home/ubuntu/workspace/rocblas/rocblas-6.4.4=. -Wformat -Werror=format-security -Wdate-time -Wno-unused-result -std=c++17 -fdeprecated-macro -ferror-limit 19 -fmessage-length=143 -stack-protector 2 -fh
ip-new-launch-api -fno-signed-char -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -vectorize-loops -vectorize-slp -fcuda-include-gpubinary /tmp/common_gemm_ex3-b25c
1c.hipfb -cuid=c90d31e2da28f391 -fcuda-allow-variadic-functions -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o CMakeFiles/rocblas_clients_common.dir/blas_ex/common_gemm_ex3.cpp.o -x hip /home/ubuntu/workspace/rocblas
/rocblas-6.4.4/clients/common/blas_ex/common_gemm_ex3.cpp
1.      <eof> parser at end of file
2.      Code generation
3.      Running pass 'Function Pass Manager' on module '/home/ubuntu/workspace/rocblas/rocblas-6.4.4/clients/common/blas_ex/common_gemm_ex3.cpp'.
4.      Running pass 'PowerPC DAG->DAG Pattern Instruction Selection' on function '@_Z19rocblas_init_matrixIDF16_Lb0EEvR11host_matrixIT_ERK9Arguments23rocblas_check_nan_init_26rocblas_check_matrix_type_bb'
 #0 0x0000646c77b19178 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/usr/lib/powerpc64le-linux-gnu/libLLVM.so.21.1+0x4079178)
 #1 0x0000646c77b19a04 (/usr/lib/powerpc64le-linux-gnu/libLLVM.so.21.1+0x4079a04)
 #2 0x0000646c77b15f14 llvm::sys::RunSignalHandlers() (/usr/lib/powerpc64le-linux-gnu/libLLVM.so.21.1+0x4075f14)
 #3 0x0000646c77b1a724 (/usr/lib/powerpc64le-linux-gnu/libLLVM.so.21.1+0x407a724)
 #4 0x0000646c80cb04e4 (linux-vdso64.so.1+0x4e4)
 #5 0x0000646c78609540 llvm::SelectionDAG::isConstantIntBuildVectorOrConstantInt(llvm::SDValue, bool) const (/usr/lib/powerpc64le-linux-gnu/libLLVM.so.21.1+0x4b69540)
 #6 0x0000646c78609304 llvm::SelectionDAG::canonicalizeCommutativeBinop(unsigned int, llvm::SDValue&, llvm::SDValue&) const (/usr/lib/powerpc64le-linux-gnu/libLLVM.so.21.1+0x4b69304)
 #7 0x0000646c78607b24 llvm::SelectionDAG::getNode(unsigned int, llvm::SDLoc const&, llvm::EVT, llvm::SDValue, llvm::SDValue, llvm::SDNodeFlags) (/usr/lib/powerpc64le-linux-gnu/libLLVM.so.21.1+0x4b67b24)
 #8 0x0000646c784d7960 (/usr/lib/powerpc64le-linux-gnu/libLLVM.so.21.1+0x4a37960)
 #9 0x0000646c784d644c (/usr/lib/powerpc64le-linux-gnu/libLLVM.so.21.1+0x4a3644c)
#10 0x0000646c7850f414 (/usr/lib/powerpc64le-linux-gnu/libLLVM.so.21.1+0x4a6f414)
#11 0x0000646c785140d0 llvm::SelectionDAG::LegalizeTypes() (/usr/lib/powerpc64le-linux-gnu/libLLVM.so.21.1+0x4a740d0)
#12 0x0000646c786400a0 llvm::SelectionDAGISel::CodeGenAndEmitDAG() (/usr/lib/powerpc64le-linux-gnu/libLLVM.so.21.1+0x4ba00a0)
#13 0x0000646c7863ff2c llvm::SelectionDAGISel::SelectBasicBlock(llvm::ilist_iterator_w_bits<llvm::ilist_detail::node_options<llvm::Instruction, false, false, void, true, llvm::BasicBlock>, false, true>, llvm::ili
st_iterator_w_bits<llvm::ilist_detail::node_options<llvm::Instruction, false, false, void, true, llvm::BasicBlock>, false, true>, bool&) (/usr/lib/powerpc64le-linux-gnu/libLLVM.so.21.1+0x4b9ff2c)
#14 0x0000646c7863f970 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&) (/usr/lib/powerpc64le-linux-gnu/libLLVM.so.21.1+0x4b9f970)
#15 0x0000646c7863da00 llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&) (/usr/lib/powerpc64le-linux-gnu/libLLVM.so.21.1+0x4b9da00)
#16 0x0000646c7b307448 (/usr/lib/powerpc64le-linux-gnu/libLLVM.so.21.1+0x7867448)
#17 0x0000646c7863bdb4 llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&) (/usr/lib/powerpc64le-linux-gnu/libLLVM.so.21.1+0x4b9bdb4)
#18 0x0000646c78062ec8 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (/usr/lib/powerpc64le-linux-gnu/libLLVM.so.21.1+0x45c2ec8)
#19 0x0000646c77cfd908 llvm::FPPassManager::runOnFunction(llvm::Function&) (/usr/lib/powerpc64le-linux-gnu/libLLVM.so.21.1+0x425d908)
#20 0x0000646c77d0512c llvm::FPPassManager::runOnModule(llvm::Module&) (/usr/lib/powerpc64le-linux-gnu/libLLVM.so.21.1+0x426512c)
#21 0x0000646c77cfe330 llvm::legacy::PassManagerImpl::run(llvm::Module&) (/usr/lib/powerpc64le-linux-gnu/libLLVM.so.21.1+0x425e330)
#22 0x0000646c77d0567c llvm::legacy::PassManager::run(llvm::Module&) (/usr/lib/powerpc64le-linux-gnu/libLLVM.so.21.1+0x426567c)
#23 0x0000646c7f358f34 clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique
_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x2938f34)
#24 0x0000646c7f6d5c34 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x2cb5c34)
#25 0x0000646c7dd36490 clang::ParseAST(clang::Sema&, bool, bool) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x1316490)
#26 0x0000646c800212bc clang::ASTFrontendAction::ExecuteAction() (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x36012bc)
#27 0x0000646c7f6da6fc clang::CodeGenAction::ExecuteAction() (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x2cba6fc)
#28 0x0000646c80020a80 clang::FrontendAction::Execute() (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x3600a80)
#29 0x0000646c7ff8a088 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x356a088)
#30 0x0000646c800c8f04 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x36a8f04)
#31 0x0000095b61a62d10 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/usr/lib/llvm-21/bin/clang+0x22d10)
#32 0x0000095b61a5ef10 (/usr/lib/llvm-21/bin/clang+0x1ef10)
#33 0x0000095b61a5e058 clang_main(int, char**, llvm::ToolContext const&) (/usr/lib/llvm-21/bin/clang+0x1e058)
#34 0x0000095b61a700cc main (/usr/lib/llvm-21/bin/clang+0x300cc)
#35 0x0000646c73356ec4 (/usr/lib/powerpc64le-linux-gnu/libc.so.6+0x26ec4)
#36 0x0000646c7335711c __libc_start_main (/usr/lib/powerpc64le-linux-gnu/libc.so.6+0x2711c)
clang++-21: error: unable to execute command: Segmentation fault (core dumped)
clang++-21: error: clang frontend command failed due to signal (use -v to see invocation)
Ubuntu clang version 21.1.2 (2ubuntu6)
Target: powerpc64le-unknown-linux-gnu
Thread model: posix
InstalledDir: /usr/lib/llvm-21/bin
clang++-21: note: diagnostic msg: Error generating preprocessed source(s).
```

Please find a tarball [1] containing a subset of the sources (generated by `strace`ing the failing process), that leads to a fast and consistent reproducer using the following procedure.
Please make sure to run that in an isolated environment. 1) you can never trust a tarball from the Internet and 2) this will make a mess of the machine, writing to many system-owned paths.
```
# Start a VM/container (any VM/container would likely do)
lxc launch ubuntu-daily:resolute resolute
lxc file push sources.tar.zst resolute/
lxc exec resolute bash
# Extract the sources at the root of the machine
cd /
tar xvPf sources.tar.zst
# Install the build dependencies
sed -i 's# deb$# deb deb-src#' /etc/apt/sources.list.d/ubuntu.sources
apt update
apt build-dep rocblas
# Finally make the build
cd /home/ubuntu/workspace/rocblas/rocblas-6.4.4/obj-powerpc64le-linux-gnu/clients/common
mkdir -p CMakeFiles/rocblas_clients_common.dir/blas_ex
./run.sh
```
[1]: https://people.canonical.com/~skia/sources.tar.zst

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-powerpc

Author: Skia (Hyask)

<details>
The same sources work fine on other architectures like `amd64` or `arm64`.

Please find the Debian unstable build log here: https://buildd.debian.org/status/logs.php?pkg=rocblas&amp;ver=6.4.4-1&amp;arch=ppc64el
Please find the Ubuntu resolute build log here: https://launchpad.net/ubuntu/+source/rocblas/6.4.4-1/+build/31463786

Here is the backtrace:
```
Stack dump:
0.      Program arguments: /usr/lib/llvm-21/bin/clang -cc1 -triple powerpc64le-unknown-linux-gnu -aux-triple amdgcn-amd-amdhsa -O3 -emit-obj -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard
-value-names -main-file-name common_gemm_ex3.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=none -fmath-errno -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu ppc64le -m
abi=ieeelongdouble -mfloat-abi hard -target-abi elfv2 -debug-info-kind=constructor -dwarf-version=5 -debugger-tuning=gdb --compress-debug-sections=zlib -fdebug-compilation-dir=/home/ubuntu/workspace/rocblas/rocbl
as-6.4.4/obj-powerpc64le-linux-gnu/clients/common -fdebug-prefix-map=/home/ubuntu/workspace/rocblas/rocblas-6.4.4=. -fdebug-prefix-map=/home/ubuntu/workspace/rocblas/rocblas-6.4.4=/usr/src/rocblas-6.4.4-1 -fcover
age-compilation-dir=/home/ubuntu/workspace/rocblas/rocblas-6.4.4/obj-powerpc64le-linux-gnu/clients/common -resource-dir /usr/lib/llvm-21/lib/clang/21 -dependency-file CMakeFiles/rocblas_clients_common.dir/blas_ex
/common_gemm_ex3.cpp.o.d -MT clients/common/CMakeFiles/rocblas_clients_common.dir/blas_ex/common_gemm_ex3.cpp.o -sys-header-deps -internal-isystem /usr/lib/llvm-21/lib/clang/21/include/cuda_wrappers -idirafter /u
sr/include -include __clang_hip_runtime_wrapper.h -D BUILD_WITH_TENSILE -D GOOGLE_TEST -D ROCBLAS_INTERNAL_API -D ROCBLAS_NO_DEPRECATED_WARNINGS -D ROCBLAS_TENSILE_LAZY_LOAD=1 -D ROCBLAS_TENSILE_SEPARATE_ARCH=1 -
D ROCM_USE_FLOAT16 -D USE_PROF_API=1 -D __HIP_PLATFORM_AMD__=1 -I /home/ubuntu/workspace/rocblas/rocblas-6.4.4/obj-powerpc64le-linux-gnu/include/rocblas -I /home/ubuntu/workspace/rocblas/rocblas-6.4.4/obj-powerpc
64le-linux-gnu/include/rocblas/internal -I /home/ubuntu/workspace/rocblas/rocblas-6.4.4/clients/common/../include -I /home/ubuntu/workspace/rocblas/rocblas-6.4.4/clients/common/../include/blas1 -I /home/ubuntu/wo
rkspace/rocblas/rocblas-6.4.4/clients/common/../include/blas2 -I /home/ubuntu/workspace/rocblas/rocblas-6.4.4/clients/common/../include/blas3 -I /home/ubuntu/workspace/rocblas/rocblas-6.4.4/clients/common/../incl
ude/blas_ex -I /home/ubuntu/workspace/rocblas/rocblas-6.4.4/clients/common/blas_ex -I /home/ubuntu/workspace/rocblas/rocblas-6.4.4/clients/common/../../library/include -I /home/ubuntu/workspace/rocblas/rocblas-6.
4.4/clients/common/../../library/src/include -I /home/ubuntu/workspace/rocblas/rocblas-6.4.4/clients/common/../../library/src -I /home/ubuntu/workspace/rocblas/rocblas-6.4.4/clients/common -I /home/ubuntu/workspa
ce/rocblas/rocblas-6.4.4/library/src -I /home/ubuntu/workspace/rocblas/rocblas-6.4.4/library/include -I /home/ubuntu/workspace/rocblas/rocblas-6.4.4/library/include/internal -I /home/ubuntu/workspace/rocblas/rocb
las-6.4.4/library/src/include -I /home/ubuntu/workspace/rocblas/rocblas-6.4.4/obj-powerpc64le-linux-gnu/include -I /home/ubuntu/workspace/rocblas/rocblas-6.4.4/library/src/blas3/Tensile -D _FORTIFY_SOURCE=3 -D __
HIP_HCC_COMPAT_MODE__=1 -D NDEBUG -D GTEST_HAS_PTHREAD=1 -internal-isystem /usr/lib/gcc/powerpc64le-linux-gnu/15/../../../../include/c++/15 -internal-isystem /usr/lib/gcc/powerpc64le-linux-gnu/15/../../../../incl
ude/powerpc64le-linux-gnu/c++/15 -internal-isystem /usr/lib/gcc/powerpc64le-linux-gnu/15/../../../../include/c++/15/backward -internal-isystem /usr/lib/gcc/powerpc64le-linux-gnu/15/../../../../include/c++/15 -int
ernal-isystem /usr/lib/gcc/powerpc64le-linux-gnu/15/../../../../include/powerpc64le-linux-gnu/c++/15 -internal-isystem /usr/lib/gcc/powerpc64le-linux-gnu/15/../../../../include/c++/15/backward -internal-isystem /
usr/lib/llvm-21/lib/clang/21/include/ppc_wrappers -internal-isystem /usr/lib/llvm-21/lib/clang/21/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/powerpc64le-linux-gnu/15/../../../../p
owerpc64le-linux-gnu/include -internal-externc-isystem /usr/include/powerpc64le-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -internal-isystem /usr/lib/llvm-21/lib/clang/21/
include/ppc_wrappers -internal-isystem /usr/lib/llvm-21/lib/clang/21/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/powerpc64le-linux-gnu/15/../../../../powerpc64le-linux-gnu/include
-internal-externc-isystem /usr/include/powerpc64le-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -fmacro-prefix-map=/home/ubuntu/workspace/rocblas/rocblas-6.4.4=. -fcoverage-
prefix-map=/home/ubuntu/workspace/rocblas/rocblas-6.4.4=. -Wformat -Werror=format-security -Wdate-time -Wno-unused-result -std=c++17 -fdeprecated-macro -ferror-limit 19 -fmessage-length=143 -stack-protector 2 -fh
ip-new-launch-api -fno-signed-char -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -vectorize-loops -vectorize-slp -fcuda-include-gpubinary /tmp/common_gemm_ex3-b25c
1c.hipfb -cuid=c90d31e2da28f391 -fcuda-allow-variadic-functions -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o CMakeFiles/rocblas_clients_common.dir/blas_ex/common_gemm_ex3.cpp.o -x hip /home/ubuntu/workspace/rocblas
/rocblas-6.4.4/clients/common/blas_ex/common_gemm_ex3.cpp
1.      &lt;eof&gt; parser at end of file
2.      Code generation
3.      Running pass 'Function Pass Manager' on module '/home/ubuntu/workspace/rocblas/rocblas-6.4.4/clients/common/blas_ex/common_gemm_ex3.cpp'.
4.      Running pass 'PowerPC DAG-&gt;DAG Pattern Instruction Selection' on function '@<!-- -->_Z19rocblas_init_matrixIDF16_Lb0EEvR11host_matrixIT_ERK9Arguments23rocblas_check_nan_init_26rocblas_check_matrix_type_bb'
 #<!-- -->0 0x0000646c77b19178 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/usr/lib/powerpc64le-linux-gnu/libLLVM.so.21.1+0x4079178)
 #<!-- -->1 0x0000646c77b19a04 (/usr/lib/powerpc64le-linux-gnu/libLLVM.so.21.1+0x4079a04)
 #<!-- -->2 0x0000646c77b15f14 llvm::sys::RunSignalHandlers() (/usr/lib/powerpc64le-linux-gnu/libLLVM.so.21.1+0x4075f14)
 #<!-- -->3 0x0000646c77b1a724 (/usr/lib/powerpc64le-linux-gnu/libLLVM.so.21.1+0x407a724)
 #<!-- -->4 0x0000646c80cb04e4 (linux-vdso64.so.1+0x4e4)
 #<!-- -->5 0x0000646c78609540 llvm::SelectionDAG::isConstantIntBuildVectorOrConstantInt(llvm::SDValue, bool) const (/usr/lib/powerpc64le-linux-gnu/libLLVM.so.21.1+0x4b69540)
 #<!-- -->6 0x0000646c78609304 llvm::SelectionDAG::canonicalizeCommutativeBinop(unsigned int, llvm::SDValue&amp;, llvm::SDValue&amp;) const (/usr/lib/powerpc64le-linux-gnu/libLLVM.so.21.1+0x4b69304)
 #<!-- -->7 0x0000646c78607b24 llvm::SelectionDAG::getNode(unsigned int, llvm::SDLoc const&amp;, llvm::EVT, llvm::SDValue, llvm::SDValue, llvm::SDNodeFlags) (/usr/lib/powerpc64le-linux-gnu/libLLVM.so.21.1+0x4b67b24)
 #<!-- -->8 0x0000646c784d7960 (/usr/lib/powerpc64le-linux-gnu/libLLVM.so.21.1+0x4a37960)
 #<!-- -->9 0x0000646c784d644c (/usr/lib/powerpc64le-linux-gnu/libLLVM.so.21.1+0x4a3644c)
#<!-- -->10 0x0000646c7850f414 (/usr/lib/powerpc64le-linux-gnu/libLLVM.so.21.1+0x4a6f414)
#<!-- -->11 0x0000646c785140d0 llvm::SelectionDAG::LegalizeTypes() (/usr/lib/powerpc64le-linux-gnu/libLLVM.so.21.1+0x4a740d0)
#<!-- -->12 0x0000646c786400a0 llvm::SelectionDAGISel::CodeGenAndEmitDAG() (/usr/lib/powerpc64le-linux-gnu/libLLVM.so.21.1+0x4ba00a0)
#<!-- -->13 0x0000646c7863ff2c llvm::SelectionDAGISel::SelectBasicBlock(llvm::ilist_iterator_w_bits&lt;llvm::ilist_detail::node_options&lt;llvm::Instruction, false, false, void, true, llvm::BasicBlock&gt;, false, true&gt;, llvm::ili
st_iterator_w_bits&lt;llvm::ilist_detail::node_options&lt;llvm::Instruction, false, false, void, true, llvm::BasicBlock&gt;, false, true&gt;, bool&amp;) (/usr/lib/powerpc64le-linux-gnu/libLLVM.so.21.1+0x4b9ff2c)
#<!-- -->14 0x0000646c7863f970 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&amp;) (/usr/lib/powerpc64le-linux-gnu/libLLVM.so.21.1+0x4b9f970)
#<!-- -->15 0x0000646c7863da00 llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&amp;) (/usr/lib/powerpc64le-linux-gnu/libLLVM.so.21.1+0x4b9da00)
#<!-- -->16 0x0000646c7b307448 (/usr/lib/powerpc64le-linux-gnu/libLLVM.so.21.1+0x7867448)
#<!-- -->17 0x0000646c7863bdb4 llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&amp;) (/usr/lib/powerpc64le-linux-gnu/libLLVM.so.21.1+0x4b9bdb4)
#<!-- -->18 0x0000646c78062ec8 llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (/usr/lib/powerpc64le-linux-gnu/libLLVM.so.21.1+0x45c2ec8)
#<!-- -->19 0x0000646c77cfd908 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (/usr/lib/powerpc64le-linux-gnu/libLLVM.so.21.1+0x425d908)
#<!-- -->20 0x0000646c77d0512c llvm::FPPassManager::runOnModule(llvm::Module&amp;) (/usr/lib/powerpc64le-linux-gnu/libLLVM.so.21.1+0x426512c)
#<!-- -->21 0x0000646c77cfe330 llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/usr/lib/powerpc64le-linux-gnu/libLLVM.so.21.1+0x425e330)
#<!-- -->22 0x0000646c77d0567c llvm::legacy::PassManager::run(llvm::Module&amp;) (/usr/lib/powerpc64le-linux-gnu/libLLVM.so.21.1+0x426567c)
#<!-- -->23 0x0000646c7f358f34 clang::emitBackendOutput(clang::CompilerInstance&amp;, clang::CodeGenOptions&amp;, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;, std::unique
_ptr&lt;llvm::raw_pwrite_stream, std::default_delete&lt;llvm::raw_pwrite_stream&gt;&gt;, clang::BackendConsumer*) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x2938f34)
#<!-- -->24 0x0000646c7f6d5c34 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&amp;) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x2cb5c34)
#<!-- -->25 0x0000646c7dd36490 clang::ParseAST(clang::Sema&amp;, bool, bool) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x1316490)
#<!-- -->26 0x0000646c800212bc clang::ASTFrontendAction::ExecuteAction() (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x36012bc)
#<!-- -->27 0x0000646c7f6da6fc clang::CodeGenAction::ExecuteAction() (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x2cba6fc)
#<!-- -->28 0x0000646c80020a80 clang::FrontendAction::Execute() (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x3600a80)
#<!-- -->29 0x0000646c7ff8a088 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x356a088)
#<!-- -->30 0x0000646c800c8f04 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x36a8f04)
#<!-- -->31 0x0000095b61a62d10 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/usr/lib/llvm-21/bin/clang+0x22d10)
#<!-- -->32 0x0000095b61a5ef10 (/usr/lib/llvm-21/bin/clang+0x1ef10)
#<!-- -->33 0x0000095b61a5e058 clang_main(int, char**, llvm::ToolContext const&amp;) (/usr/lib/llvm-21/bin/clang+0x1e058)
#<!-- -->34 0x0000095b61a700cc main (/usr/lib/llvm-21/bin/clang+0x300cc)
#<!-- -->35 0x0000646c73356ec4 (/usr/lib/powerpc64le-linux-gnu/libc.so.6+0x26ec4)
#<!-- -->36 0x0000646c7335711c __libc_start_main (/usr/lib/powerpc64le-linux-gnu/libc.so.6+0x2711c)
clang++-21: error: unable to execute command: Segmentation fault (core dumped)
clang++-21: error: clang frontend command failed due to signal (use -v to see invocation)
Ubuntu clang version 21.1.2 (2ubuntu6)
Target: powerpc64le-unknown-linux-gnu
Thread model: posix
InstalledDir: /usr/lib/llvm-21/bin
clang++-21: note: diagnostic msg: Error generating preprocessed source(s).
```

Please find a tarball [1] containing a subset of the sources (generated by `strace`ing the failing process), that leads to a fast and consistent reproducer using the following procedure.
Please make sure to run that in an isolated environment. 1) you can never trust a tarball from the Internet and 2) this will make a mess of the machine, writing to many system-owned paths.
```
# Start a VM/container (any VM/container would likely do)
lxc launch ubuntu-daily:resolute resolute
lxc file push sources.tar.zst resolute/
lxc exec resolute bash
# Extract the sources at the root of the machine
cd /
tar xvPf sources.tar.zst
# Install the build dependencies
sed -i 's# deb$# deb deb-src#' /etc/apt/sources.list.d/ubuntu.sources
apt update
apt build-dep rocblas
# Finally make the build
cd /home/ubuntu/workspace/rocblas/rocblas-6.4.4/obj-powerpc64le-linux-gnu/clients/common
mkdir -p CMakeFiles/rocblas_clients_common.dir/blas_ex
./run.sh
```
[1]: https://people.canonical.com/~skia/sources.tar.zst
</details>


---

