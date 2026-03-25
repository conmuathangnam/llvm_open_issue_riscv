# Clang crashes when building ScyllaDB for s390x (exit code 139)

**Author:** shahidhs-ibm
**URL:** https://github.com/llvm/llvm-project/issues/59749
**Status:** Closed
**Labels:** llvm:optimizations, crash
**Closed Date:** 2024-11-29T06:41:17Z

## Body

We're trying to build ScyllaDB (ver 5.1.1) from source on s390x architecture.

We're observing following error:
```console
[26/807] CXX build/release/replica/table.o
FAILED: build/release/replica/table.o
clang++ -MD -MT build/release/replica/table.o -MF build/release/replica/table.o.d -I/scylla/seastar/include -I/scylla/build/release/seastar/gen/include -U_FORTIFY_SOURCE -DSEASTAR_SSTRING -Werror=unused-result -DSEASTAR_API_LEVEL=6 -DSEASTAR_SCHEDULING_GROUPS_COUNT=16 -DFMT_LOCALE -DFMT_SHARED -I/usr/include/p11-kit-1 -Wl,--gc-sections -ffunction-sections -fdata-sections  -O3 -mllvm -inline-threshold=2500 -fno-slp-vectorize -g -gz -iquote. -iquote build/release/gen --std=gnu++20  -ffile-prefix-map=/scylla=.  -march= -DBOOST_TEST_DYN_LINK   -Iabseil -fvisibility=hidden  -Wall -Werror -Wno-mismatched-tags -Wno-tautological-compare -Wno-parentheses-equality -Wno-c++11-narrowing -Wno-sometimes-uninitialized -Wno-return-stack-address -Wno-missing-braces -Wno-unused-lambda-capture -Wno-overflow -Wno-noexcept-type -Wno-error=cpp -Wno-ignored-attributes -Wno-overloaded-virtual -Wno-unused-command-line-argument -Wno-defaulted-function-deleted -Wno-redeclared-class-member -Wno-unsupported-friend -Wno-unused-variable -Wno-delete-non-abstract-non-virtual-dtor -Wno-braced-scalar-init -Wno-implicit-int-float-conversion -Wno-delete-abstract-non-virtual-dtor -Wno-uninitialized-const-reference -Wno-psabi -Wno-narrowing -Wno-array-bounds -Wno-nonnull -Wno-uninitialized -Wno-unused-private-field -Wno-ambiguous-reversed-operator -Wno-deprecated-declarations -Wno-unused-value -Wno-error=deprecated-declarations -DXXH_PRIVATE_API -DSEASTAR_TESTING_MAIN -DHAVE_LZ4_COMPRESS_DEFAULT  -c -o build/release/replica/table.o replica/table.cc
In file included from replica/table.cc:18:
In file included from ./replica/database.hh:11:
In file included from ./locator/abstract_replication_strategy.hh:15:
In file included from ./locator/snitch_base.hh:20:
In file included from ./gms/versioned_value.hh:18:
In file included from ./dht/i_partitioner.hh:14:
In file included from ./types.hh:32:
./utils/exceptions.hh:21:6: warning: "Fast implementation of some of the exception handling routines is not available for this platform. Expect poor exception handling performance." [-W#warnings]
    #warning "Fast implementation of some of the exception handling routines is not available for this platform. Expect poor exception handling performance."
     ^
PLEASE submit a bug report to https://bugs.llvm.org/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: clang++ -MD -MT build/release/replica/table.o -MF build/release/replica/table.o.d -I/scylla/seastar/include -I/scylla/build/release/seastar/gen/include -U_FORTIFY_SOURCE -DSEASTAR_SSTRING -Werror=unused-result -DSEASTAR_API_LEVEL=6 -DSEASTAR_SCHEDULING_GROUPS_COUNT=16 -DFMT_LOCALE -DFMT_SHARED -I/usr/include/p11-kit-1 -Wl,--gc-sections -ffunction-sections -fdata-sections -O3 -mllvm -inline-threshold=2500 -fno-slp-vectorize -g -gz -iquote. -iquote build/release/gen --std=gnu++20 -ffile-prefix-map=/scylla=. -march= -DBOOST_TEST_DYN_LINK -Iabseil -fvisibility=hidden -Wall -Werror -Wno-mismatched-tags -Wno-tautological-compare -Wno-parentheses-equality -Wno-c++11-narrowing -Wno-sometimes-uninitialized -Wno-return-stack-address -Wno-missing-braces -Wno-unused-lambda-capture -Wno-overflow -Wno-noexcept-type -Wno-error=cpp -Wno-ignored-attributes -Wno-overloaded-virtual -Wno-unused-command-line-argument -Wno-defaulted-function-deleted -Wno-redeclared-class-member -Wno-unsupported-friend -Wno-unused-variable -Wno-delete-non-abstract-non-virtual-dtor -Wno-braced-scalar-init -Wno-implicit-int-float-conversion -Wno-delete-abstract-non-virtual-dtor -Wno-uninitialized-const-reference -Wno-psabi -Wno-narrowing -Wno-array-bounds -Wno-nonnull -Wno-uninitialized -Wno-unused-private-field -Wno-ambiguous-reversed-operator -Wno-deprecated-declarations -Wno-unused-value -Wno-error=deprecated-declarations -DXXH_PRIVATE_API -DSEASTAR_TESTING_MAIN -DHAVE_LZ4_COMPRESS_DEFAULT -c -o build/release/replica/table.o replica/table.cc
1.      <eof> parser at end of file
2.      Per-module optimization passes
3.      Running pass 'CallGraph Pass Manager' on module 'replica/table.cc'.
4.      While splitting coroutine @_ZN7replica5table5queryEN7seastar13lw_shared_ptrIK6schemaEE13reader_permitRKN5query12read_commandENS7_14result_optionsERKSt6vectorI20nonwrapping_intervalIN3dht13ring_positionEESaISG_EEN7tracing15trace_state_ptrERNS7_21result_memory_limiterENSt6chrono10time_pointINS1_12lowres_clockENSP_8durationIlSt5ratioILl1ELl1000000000EEEEEEPSt8optionalINS7_7querierEE
Stack dump without symbol names (ensure you have llvm-symbolizer in your PATH or set the environment var `LLVM_SYMBOLIZER_PATH` to point to it):
/lib64/libLLVM-12.so(_ZN4llvm3sys15PrintStackTraceERNS_11raw_ostreamEi+0x38)[0x3ff78efb428]
/lib64/libLLVM-12.so(_ZN4llvm3sys17RunSignalHandlersEv+0x36)[0x3ff78ef9416]
/lib64/libLLVM-12.so(+0xb32c8e)[0x3ff78e32c8e]
linux-vdso64.so.1(__kernel_sigreturn+0x0)[0x3ff8167e488]
/lib64/libLLVM-12.so(_ZN4llvm23removeUnreachableBlocksERNS_8FunctionEPNS_14DomTreeUpdaterEPNS_16MemorySSAUpdaterE+0x19c)[0x3ff79a3845c]
/lib64/libLLVM-12.so(+0x1e73782)[0x3ff7a173782]
/lib64/libLLVM-12.so(+0x1e7445e)[0x3ff7a17445e]
/lib64/libLLVM-12.so(+0x1e75b26)[0x3ff7a175b26]
/lib64/libLLVM-12.so(+0x1e77f60)[0x3ff7a177f60]
/lib64/libLLVM-12.so(+0x1f030c4)[0x3ff7a2030c4]
/lib64/libLLVM-12.so(_ZN4llvm6legacy15PassManagerImpl3runERNS_6ModuleE+0x3ec)[0x3ff790411bc]
/lib64/libclang-cpp.so.12(+0x189a0ce)[0x3ff7fb9a0ce]
/lib64/libclang-cpp.so.12(_ZN5clang17EmitBackendOutputERNS_17DiagnosticsEngineERKNS_19HeaderSearchOptionsERKNS_14CodeGenOptionsERKNS_13TargetOptionsERKNS_11LangOptionsERKN4llvm10DataLayoutEPNSE_6ModuleENS_13BackendActionESt10unique_ptrINSE_17raw_pwrite_streamESt14default_deleteISM_EE+0x2e8)[0x3ff7fb9bb98]
/lib64/libclang-cpp.so.12(+0x1bbc828)[0x3ff7febc828]
/lib64/libclang-cpp.so.12(_ZN5clang8ParseASTERNS_4SemaEbb+0x4c8)[0x3ff7ed0e588]
/lib64/libclang-cpp.so.12(_ZN5clang14FrontendAction7ExecuteEv+0xe6)[0x3ff805c4116]
/lib64/libclang-cpp.so.12(_ZN5clang16CompilerInstance13ExecuteActionERNS_14FrontendActionE+0x24a)[0x3ff8056a7da]
/lib64/libclang-cpp.so.12(_ZN5clang25ExecuteCompilerInvocationEPNS_16CompilerInstanceE+0x606)[0x3ff80635586]
clang++(_Z8cc1_mainN4llvm8ArrayRefIPKcEES2_Pv+0xe0a)[0x2aa2419645a]
clang++(+0x13454)[0x2aa24193454]
/lib64/libclang-cpp.so.12(+0x1f70836)[0x3ff80270836]
/lib64/libLLVM-12.so(_ZN4llvm20CrashRecoveryContext9RunSafelyENS_12function_refIFvvEEE+0x40)[0x3ff78e32db0]
/lib64/libclang-cpp.so.12(+0x1f71c42)[0x3ff80271c42]
/lib64/libclang-cpp.so.12(_ZNK5clang6driver11Compilation14ExecuteCommandERKNS0_7CommandERPS3_+0xd4)[0x3ff80246eb4]
/lib64/libclang-cpp.so.12(_ZNK5clang6driver11Compilation11ExecuteJobsERKNS0_7JobListERN4llvm15SmallVectorImplISt4pairIiPKNS0_7CommandEEEE+0xac)[0x3ff8024777c]
/lib64/libclang-cpp.so.12(_ZN5clang6driver6Driver18ExecuteCompilationERNS0_11CompilationERN4llvm15SmallVectorImplISt4pairIiPKNS0_7CommandEEEE+0xd2)[0x3ff80252aa2]
clang++(main+0x1bac)[0x2aa24190d5c]
/lib64/libc.so.6(__libc_start_main+0xe4)[0x3ff77eabe04]
clang++(+0x12654)[0x2aa24192654]
clang-12: error: clang frontend command failed with exit code 139 (use -v to see invocation)
clang version 12.0.1 (Fedora 12.0.1-1.fc34)
Target: s390x-ibm-linux
Thread model: posix
InstalledDir: /usr/bin
clang-12: note: diagnostic msg:
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang-12: note: diagnostic msg: /tmp/table-880922.cpp
clang-12: note: diagnostic msg: /tmp/table-880922.sh
clang-12: note: diagnostic msg:

********************
```

Steps to reproduce (on s390x, Fedora 34):
```console
$ git clone https://github.com/scylladb/scylla
$ cd scylla
$ git checkout scylla-5.1.1
$ git submodule update --init --force --recursive
$ ./install-dependencies.sh
$ git apply scylla.patch
$ cd seastar
$ git apply seastar.patch
$ cd ..
$ ./configure.py --mode=release
$ ninja build -j $(nproc)
```

Please let us know if any more information is required.

[seastar.patch](https://github.com/llvm/llvm-project/files/10330128/seastar.patch)
[scylla.patch](https://github.com/llvm/llvm-project/files/10330130/scylla.patch)


## Comments

### Comment 1 - tbaederr

clang 12 is quite old, any chance you could try a newer version (preferably 15)? 

---

### Comment 2 - shahidhs-ibm

@tbaederr We tried with clang 15 as well but facing same error.

---

### Comment 3 - shahidhs-ibm

This issue is no longer reproducible.

---

