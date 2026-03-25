# Clang 19 crash when building LLVM-libc complex f128 tests

**Author:** michaelrj-google
**URL:** https://github.com/llvm/llvm-project/issues/171881

## Body

Ran into a clang crash when building LLVM-libc with some experimental cmake changes. Here's the logs and such. I've also included the git diff  for the cmake (the base LLVM project commit was `2797688887217adefeb57fa589b803aceaea051d`). The commands I ran with that diff are:
`cmake ../runtimes -G Ninja -DLLVM_ENABLE_RUNTIMES="libc;compiler-rt" -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -DLLVM_LIBC_FULL_BUILD=ON -DLLVM_LIBC_INCLUDE_SCUDO=ON -DCOMPILER_RT_BUILD_SCUDO_STANDALONE_WITH_LLVM_LIBC=ON -DCOMPILER_RT_BUILD_GWP_ASAN=OFF -DCOMPILER_RT_SCUDO_STANDALONE_BUILD_SHARED=OFF -DCMAKE_INSTALL_PREFIX=/usr/local/google/home/michaelrj/install`
then `ninja check-libc`

There were several similar crashes, on `crealf128_test`, `cprojf128_test`, `cprojf128_test`, `crealf128_test`, `conjf128_test`, `conjf128_test`, and `cimagf128_test` (the one attached).

```
[4027/13019] Building CXX object libc/test/src/complex/CMak...agf128_test.__hermetic__.__build__.dir/cimagf128_test.cpp.
FAILED: libc/test/src/complex/CMakeFiles/libc.test.src.complex.cimagf128_test.__hermetic__.__build__.dir/cimagf128_test.cpp.o 
/usr/bin/clang++ -DLIBC_NAMESPACE=__llvm_libc_22_0_0_git -I/usr/local/google/home/michaelrj/llvm-project/libc -isystem /usr/local/google/home/michaelrj/llvm-project/build-whatever/libc/include -fvisibility-inlines-hidden -Werror=date-time -Werror=unguarded-availability-new -Wall -Wextra -Wno-unused-parameter -Wwrite-strings -Wcast-qual -Wmissing-field-initializers -Wimplicit-fallthrough -Wcovered-switch-default -Wno-noexcept-type -Wnon-virtual-dtor -Wdelete-non-virtual-dtor -Wsuggest-override -Wstring-conversion -Wno-pass-failed -Wmisleading-indentation -Wctad-maybe-unsupported -fdiagnostics-color -ffunction-sections -fdata-sections -O3 -DNDEBUG -std=gnu++17 -DLIBC_QSORT_IMPL=LIBC_QSORT_QUICK_SORT -DLIBC_COPT_STRING_LENGTH_IMPL=clang_vector -DLIBC_COPT_FIND_FIRST_CHARACTER_IMPL=word -DLIBC_ERRNO_MODE=LIBC_ERRNO_MODE_DEFAULT -DLIBC_THREAD_MODE=LIBC_THREAD_MODE_PLATFORM -fpie -DLIBC_FULL_BUILD -ffreestanding -fno-exceptions -fno-unwind-tables -fno-asynchronous-unwind-tables -fno-rtti -ffixed-point -Wall -Wextra -Wconversion -Wno-sign-conversion -Wimplicit-fallthrough -Wwrite-strings -Wno-c99-extensions -Wno-gnu-imaginary-constant -Wno-pedantic -Wstrict-prototypes -Wextra-semi -Wnewline-eof -Wnonportable-system-include-path -Wthread-safety -DLIBC_TEST=HERMETIC -DLIBC_COPT_RAW_MUTEX_DEFAULT_SPIN_COUNT=100 -DLIBC_COPT_TIMEOUT_ENSURE_MONOTONICITY=1 -MD -MT libc/test/src/complex/CMakeFiles/libc.test.src.complex.cimagf128_test.__hermetic__.__build__.dir/cimagf128_test.cpp.o -MF libc/test/src/complex/CMakeFiles/libc.test.src.complex.cimagf128_test.__hermetic__.__build__.dir/cimagf128_test.cpp.o.d -o libc/test/src/complex/CMakeFiles/libc.test.src.complex.cimagf128_test.__hermetic__.__build__.dir/cimagf128_test.cpp.o -c /usr/local/google/home/michaelrj/llvm-project/libc/test/src/complex/cimagf128_test.cpp
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /usr/bin/clang++ -DLIBC_NAMESPACE=__llvm_libc_22_0_0_git -I/usr/local/google/home/michaelrj/llvm-project/libc -isystem /usr/local/google/home/michaelrj/llvm-project/build-whatever/libc/include -fvisibility-inlines-hidden -Werror=date-time -Werror=unguarded-availability-new -Wall -Wextra -Wno-unused-parameter -Wwrite-strings -Wcast-qual -Wmissing-field-initializers -Wimplicit-fallthrough -Wcovered-switch-default -Wno-noexcept-type -Wnon-virtual-dtor -Wdelete-non-virtual-dtor -Wsuggest-override -Wstring-conversion -Wno-pass-failed -Wmisleading-indentation -Wctad-maybe-unsupported -fdiagnostics-color -ffunction-sections -fdata-sections -O3 -DNDEBUG -std=gnu++17 -DLIBC_QSORT_IMPL=LIBC_QSORT_QUICK_SORT -DLIBC_COPT_STRING_LENGTH_IMPL=clang_vector -DLIBC_COPT_FIND_FIRST_CHARACTER_IMPL=word -DLIBC_ERRNO_MODE=LIBC_ERRNO_MODE_DEFAULT -DLIBC_THREAD_MODE=LIBC_THREAD_MODE_PLATFORM -fpie -DLIBC_FULL_BUILD -ffreestanding -fno-exceptions -fno-unwind-tables -fno-asynchronous-unwind-tables -fno-rtti -ffixed-point -Wall -Wextra -Wconversion -Wno-sign-conversion -Wimplicit-fallthrough -Wwrite-strings -Wno-c99-extensions -Wno-gnu-imaginary-constant -Wno-pedantic -Wstrict-prototypes -Wextra-semi -Wnewline-eof -Wnonportable-system-include-path -Wthread-safety -DLIBC_TEST=HERMETIC -DLIBC_COPT_RAW_MUTEX_DEFAULT_SPIN_COUNT=100 -DLIBC_COPT_TIMEOUT_ENSURE_MONOTONICITY=1 -MD -MT libc/test/src/complex/CMakeFiles/libc.test.src.complex.cimagf128_test.__hermetic__.__build__.dir/cimagf128_test.cpp.o -MF libc/test/src/complex/CMakeFiles/libc.test.src.complex.cimagf128_test.__hermetic__.__build__.dir/cimagf128_test.cpp.o.d -o libc/test/src/complex/CMakeFiles/libc.test.src.complex.cimagf128_test.__hermetic__.__build__.dir/cimagf128_test.cpp.o -c /usr/local/google/home/michaelrj/llvm-project/libc/test/src/complex/cimagf128_test.cpp
1.	<eof> parser at end of file
2.	Optimizer
3.	Running pass "require<globals-aa>,function(invalidate<aa>),require<profile-summary>,cgscc(devirt<4>(inline,function-attrs<skip-non-recursive-function-attrs>,argpromotion,openmp-opt-cgscc,function<eager-inv;no-rerun>(sroa<modify-cfg>,early-cse<memssa>,speculative-execution<only-if-divergent-target>,jump-threading,correlated-propagation,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,instcombine<max-iterations=1;no-use-loop-info;no-verify-fixpoint>,aggressive-instcombine,libcalls-shrinkwrap,tailcallelim,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,reassociate,constraint-elimination,loop-mssa(loop-instsimplify,loop-simplifycfg,licm<no-allowspeculation>,loop-rotate<header-duplication;no-prepare-for-lto>,licm<allowspeculation>,simple-loop-unswitch<nontrivial;trivial>),simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,instcombine<max-iterations=1;no-use-loop-info;no-verify-fixpoint>,loop(loop-idiom,indvars,simple-loop-unswitch<nontrivial;trivial>,loop-deletion,loop-unroll-full),sroa<modify-cfg>,vector-combine,mldst-motion<no-split-footer-bb>,gvn<>,sccp,bdce,instcombine<max-iterations=1;no-use-loop-info;no-verify-fixpoint>,jump-threading,correlated-propagation,adce,memcpyopt,dse,move-auto-init,loop-mssa(licm<allowspeculation>),coro-elide,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-lookup;keep-loops;hoist-common-insts;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,instcombine<max-iterations=1;no-use-loop-info;no-verify-fixpoint>),function-attrs,function(require<should-not-run-function-passes>),coro-split)),function(invalidate<should-not-run-function-passes>),cgscc(devirt<4>())" on module "/usr/local/google/home/michaelrj/llvm-project/libc/test/src/complex/cimagf128_test.cpp"
4.	Running pass "cgscc(devirt<4>(inline,function-attrs<skip-non-recursive-function-attrs>,argpromotion,openmp-opt-cgscc,function<eager-inv;no-rerun>(sroa<modify-cfg>,early-cse<memssa>,speculative-execution<only-if-divergent-target>,jump-threading,correlated-propagation,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,instcombine<max-iterations=1;no-use-loop-info;no-verify-fixpoint>,aggressive-instcombine,libcalls-shrinkwrap,tailcallelim,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,reassociate,constraint-elimination,loop-mssa(loop-instsimplify,loop-simplifycfg,licm<no-allowspeculation>,loop-rotate<header-duplication;no-prepare-for-lto>,licm<allowspeculation>,simple-loop-unswitch<nontrivial;trivial>),simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,instcombine<max-iterations=1;no-use-loop-info;no-verify-fixpoint>,loop(loop-idiom,indvars,simple-loop-unswitch<nontrivial;trivial>,loop-deletion,loop-unroll-full),sroa<modify-cfg>,vector-combine,mldst-motion<no-split-footer-bb>,gvn<>,sccp,bdce,instcombine<max-iterations=1;no-use-loop-info;no-verify-fixpoint>,jump-threading,correlated-propagation,adce,memcpyopt,dse,move-auto-init,loop-mssa(licm<allowspeculation>),coro-elide,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-lookup;keep-loops;hoist-common-insts;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,instcombine<max-iterations=1;no-use-loop-info;no-verify-fixpoint>),function-attrs,function(require<should-not-run-function-passes>),coro-split))" on module "/usr/local/google/home/michaelrj/llvm-project/libc/test/src/complex/cimagf128_test.cpp"
Stack dump without symbol names (ensure you have llvm-symbolizer in your PATH or set the environment var `LLVM_SYMBOLIZER_PATH` to point to it):
0  libLLVM.so.19.1      0x00007f35e56b067a llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) + 58
1  libLLVM.so.19.1      0x00007f35e56ae224 llvm::sys::RunSignalHandlers() + 84
2  libLLVM.so.19.1      0x00007f35e55f4144
3  libc.so.6            0x00007f35e4249df0
4  libLLVM.so.19.1      0x00007f35e6bb290a
5  libLLVM.so.19.1      0x00007f35e6baf6ac llvm::ConstantFoldCall(llvm::CallBase const*, llvm::Function*, llvm::ArrayRef<llvm::Constant*>, llvm::TargetLibraryInfo const*, bool) + 316
6  libLLVM.so.19.1      0x00007f35e6c1e1d3
7  libLLVM.so.19.1      0x00007f35e6c199a1
8  libLLVM.so.19.1      0x00007f35e6c1576b
9  libLLVM.so.19.1      0x00007f35e6c149ef llvm::getInlineCost(llvm::CallBase&, llvm::Function*, llvm::InlineParams const&, llvm::TargetTransformInfo&, llvm::function_ref<llvm::AssumptionCache& (llvm::Function&)>, llvm::function_ref<llvm::TargetLibraryInfo const& (llvm::Function&)>, llvm::function_ref<llvm::BlockFrequencyInfo& (llvm::Function&)>, llvm::ProfileSummaryInfo*, llvm::OptimizationRemarkEmitter*) + 319
10 libLLVM.so.19.1      0x00007f35e6c1489f llvm::getInlineCost(llvm::CallBase&, llvm::InlineParams const&, llvm::TargetTransformInfo&, llvm::function_ref<llvm::AssumptionCache& (llvm::Function&)>, llvm::function_ref<llvm::TargetLibraryInfo const& (llvm::Function&)>, llvm::function_ref<llvm::BlockFrequencyInfo& (llvm::Function&)>, llvm::ProfileSummaryInfo*, llvm::OptimizationRemarkEmitter*) + 159
11 libLLVM.so.19.1      0x00007f35e6c24b5c
12 libLLVM.so.19.1      0x00007f35e6c21c7b llvm::shouldInline(llvm::CallBase&, llvm::function_ref<llvm::InlineCost (llvm::CallBase&)>, llvm::OptimizationRemarkEmitter&, bool) + 75
13 libLLVM.so.19.1      0x00007f35e6c2122b
14 libLLVM.so.19.1      0x00007f35e6c20fae llvm::DefaultInlineAdvisor::getAdviceImpl(llvm::CallBase&) + 62
15 libLLVM.so.19.1      0x00007f35e6c24361 llvm::InlineAdvisor::getAdvice(llvm::CallBase&, bool) + 225
16 libLLVM.so.19.1      0x00007f35e68c44d2 llvm::InlinerPass::run(llvm::LazyCallGraph::SCC&, llvm::AnalysisManager<llvm::LazyCallGraph::SCC, llvm::LazyCallGraph&>&, llvm::LazyCallGraph&, llvm::CGSCCUpdateResult&) + 3122
17 libLLVM.so.19.1      0x00007f35e68c8ba1
18 libLLVM.so.19.1      0x00007f35e6b91a07 llvm::PassManager<llvm::LazyCallGraph::SCC, llvm::AnalysisManager<llvm::LazyCallGraph::SCC, llvm::LazyCallGraph&>, llvm::LazyCallGraph&, llvm::CGSCCUpdateResult&>::run(llvm::LazyCallGraph::SCC&, llvm::AnalysisManager<llvm::LazyCallGraph::SCC, llvm::LazyCallGraph&>&, llvm::LazyCallGraph&, llvm::CGSCCUpdateResult&) + 519
19 libLLVM.so.19.1      0x00007f35e68c9011
20 libLLVM.so.19.1      0x00007f35e6b93440 llvm::DevirtSCCRepeatedPass::run(llvm::LazyCallGraph::SCC&, llvm::AnalysisManager<llvm::LazyCallGraph::SCC, llvm::LazyCallGraph&>&, llvm::LazyCallGraph&, llvm::CGSCCUpdateResult&) + 320
21 libLLVM.so.19.1      0x00007f35e68c95d1
22 libLLVM.so.19.1      0x00007f35e6b92b6f llvm::ModuleToPostOrderCGSCCPassAdaptor::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) + 2383
23 libLLVM.so.19.1      0x00007f35e68c93d1
24 libLLVM.so.19.1      0x00007f35e583c729 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) + 489
25 libLLVM.so.19.1      0x00007f35e68c64ec llvm::ModuleInlinerWrapperPass::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) + 604
26 libLLVM.so.19.1      0x00007f35e89fab11
27 libLLVM.so.19.1      0x00007f35e583c729 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) + 489
28 libclang-cpp.so.19.1 0x00007f35ee4856f3
29 libclang-cpp.so.19.1 0x00007f35ee47cd9a clang::EmitBackendOutput(clang::DiagnosticsEngine&, clang::HeaderSearchOptions const&, clang::CodeGenOptions const&, clang::TargetOptions const&, clang::LangOptions const&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) + 6954
30 libclang-cpp.so.19.1 0x00007f35ee82a551 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) + 1953
31 libclang-cpp.so.19.1 0x00007f35ed2344e9 clang::ParseAST(clang::Sema&, bool, bool) + 665
32 libclang-cpp.so.19.1 0x00007f35ef347789 clang::FrontendAction::Execute() + 89
33 libclang-cpp.so.19.1 0x00007f35ef2b6084 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) + 1012
34 libclang-cpp.so.19.1 0x00007f35ef3c5c0e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) + 702
35 clang++              0x0000560953b03c61 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) + 5905
36 clang++              0x0000560953b00b35
37 libclang-cpp.so.19.1 0x00007f35eef354cd
38 libLLVM.so.19.1      0x00007f35e55f3ee0 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) + 144
39 libclang-cpp.so.19.1 0x00007f35eef34f61 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const + 369
40 libclang-cpp.so.19.1 0x00007f35eeefa198 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const + 920
41 libclang-cpp.so.19.1 0x00007f35eeefa40e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const + 142
42 libclang-cpp.so.19.1 0x00007f35eef17bcc clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) + 348
43 clang++              0x0000560953b00574 clang_main(int, char**, llvm::ToolContext const&) + 6820
44 clang++              0x0000560953b0e4ba main + 106
45 libc.so.6            0x00007f35e4233ca8
46 libc.so.6            0x00007f35e4233d65 __libc_start_main + 133
47 clang++              0x0000560953afe731 _start + 33
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Debian clang version 19.1.7 (3+build5)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/lib/llvm-19/bin
clang++: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: /tmp/cimagf128_test-fa9fb3.cpp
clang++: note: diagnostic msg: /tmp/cimagf128_test-fa9fb3.sh
clang++: note: diagnostic msg: 

********************
```

[cimagf128_test-fa9fb3.cpp](https://github.com/user-attachments/files/24109766/cimagf128_test-fa9fb3.cpp)
[cimagf128_test-fa9fb3.sh](https://github.com/user-attachments/files/24109764/cimagf128_test-fa9fb3.sh)
[libc_cmake_diff.txt](https://github.com/user-attachments/files/24109765/libc_cmake_diff.txt)

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-libc

Author: Michael Jones (michaelrj-google)

<details>
Ran into a clang crash when building LLVM-libc with some experimental cmake changes. Here's the logs and such. I've also included the git diff  for the cmake (the base LLVM project commit was `2797688887217adefeb57fa589b803aceaea051d`). The commands I ran with that diff are:
`cmake ../runtimes -G Ninja -DLLVM_ENABLE_RUNTIMES="libc;compiler-rt" -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -DLLVM_LIBC_FULL_BUILD=ON -DLLVM_LIBC_INCLUDE_SCUDO=ON -DCOMPILER_RT_BUILD_SCUDO_STANDALONE_WITH_LLVM_LIBC=ON -DCOMPILER_RT_BUILD_GWP_ASAN=OFF -DCOMPILER_RT_SCUDO_STANDALONE_BUILD_SHARED=OFF -DCMAKE_INSTALL_PREFIX=/usr/local/google/home/michaelrj/install`
then `ninja check-libc`

There were several similar crashes, on `crealf128_test`, `cprojf128_test`, `cprojf128_test`, `crealf128_test`, `conjf128_test`, `conjf128_test`, and `cimagf128_test` (the one attached).

```
[4027/13019] Building CXX object libc/test/src/complex/CMak...agf128_test.__hermetic__.__build__.dir/cimagf128_test.cpp.
FAILED: libc/test/src/complex/CMakeFiles/libc.test.src.complex.cimagf128_test.__hermetic__.__build__.dir/cimagf128_test.cpp.o 
/usr/bin/clang++ -DLIBC_NAMESPACE=__llvm_libc_22_0_0_git -I/usr/local/google/home/michaelrj/llvm-project/libc -isystem /usr/local/google/home/michaelrj/llvm-project/build-whatever/libc/include -fvisibility-inlines-hidden -Werror=date-time -Werror=unguarded-availability-new -Wall -Wextra -Wno-unused-parameter -Wwrite-strings -Wcast-qual -Wmissing-field-initializers -Wimplicit-fallthrough -Wcovered-switch-default -Wno-noexcept-type -Wnon-virtual-dtor -Wdelete-non-virtual-dtor -Wsuggest-override -Wstring-conversion -Wno-pass-failed -Wmisleading-indentation -Wctad-maybe-unsupported -fdiagnostics-color -ffunction-sections -fdata-sections -O3 -DNDEBUG -std=gnu++17 -DLIBC_QSORT_IMPL=LIBC_QSORT_QUICK_SORT -DLIBC_COPT_STRING_LENGTH_IMPL=clang_vector -DLIBC_COPT_FIND_FIRST_CHARACTER_IMPL=word -DLIBC_ERRNO_MODE=LIBC_ERRNO_MODE_DEFAULT -DLIBC_THREAD_MODE=LIBC_THREAD_MODE_PLATFORM -fpie -DLIBC_FULL_BUILD -ffreestanding -fno-exceptions -fno-unwind-tables -fno-asynchronous-unwind-tables -fno-rtti -ffixed-point -Wall -Wextra -Wconversion -Wno-sign-conversion -Wimplicit-fallthrough -Wwrite-strings -Wno-c99-extensions -Wno-gnu-imaginary-constant -Wno-pedantic -Wstrict-prototypes -Wextra-semi -Wnewline-eof -Wnonportable-system-include-path -Wthread-safety -DLIBC_TEST=HERMETIC -DLIBC_COPT_RAW_MUTEX_DEFAULT_SPIN_COUNT=100 -DLIBC_COPT_TIMEOUT_ENSURE_MONOTONICITY=1 -MD -MT libc/test/src/complex/CMakeFiles/libc.test.src.complex.cimagf128_test.__hermetic__.__build__.dir/cimagf128_test.cpp.o -MF libc/test/src/complex/CMakeFiles/libc.test.src.complex.cimagf128_test.__hermetic__.__build__.dir/cimagf128_test.cpp.o.d -o libc/test/src/complex/CMakeFiles/libc.test.src.complex.cimagf128_test.__hermetic__.__build__.dir/cimagf128_test.cpp.o -c /usr/local/google/home/michaelrj/llvm-project/libc/test/src/complex/cimagf128_test.cpp
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /usr/bin/clang++ -DLIBC_NAMESPACE=__llvm_libc_22_0_0_git -I/usr/local/google/home/michaelrj/llvm-project/libc -isystem /usr/local/google/home/michaelrj/llvm-project/build-whatever/libc/include -fvisibility-inlines-hidden -Werror=date-time -Werror=unguarded-availability-new -Wall -Wextra -Wno-unused-parameter -Wwrite-strings -Wcast-qual -Wmissing-field-initializers -Wimplicit-fallthrough -Wcovered-switch-default -Wno-noexcept-type -Wnon-virtual-dtor -Wdelete-non-virtual-dtor -Wsuggest-override -Wstring-conversion -Wno-pass-failed -Wmisleading-indentation -Wctad-maybe-unsupported -fdiagnostics-color -ffunction-sections -fdata-sections -O3 -DNDEBUG -std=gnu++17 -DLIBC_QSORT_IMPL=LIBC_QSORT_QUICK_SORT -DLIBC_COPT_STRING_LENGTH_IMPL=clang_vector -DLIBC_COPT_FIND_FIRST_CHARACTER_IMPL=word -DLIBC_ERRNO_MODE=LIBC_ERRNO_MODE_DEFAULT -DLIBC_THREAD_MODE=LIBC_THREAD_MODE_PLATFORM -fpie -DLIBC_FULL_BUILD -ffreestanding -fno-exceptions -fno-unwind-tables -fno-asynchronous-unwind-tables -fno-rtti -ffixed-point -Wall -Wextra -Wconversion -Wno-sign-conversion -Wimplicit-fallthrough -Wwrite-strings -Wno-c99-extensions -Wno-gnu-imaginary-constant -Wno-pedantic -Wstrict-prototypes -Wextra-semi -Wnewline-eof -Wnonportable-system-include-path -Wthread-safety -DLIBC_TEST=HERMETIC -DLIBC_COPT_RAW_MUTEX_DEFAULT_SPIN_COUNT=100 -DLIBC_COPT_TIMEOUT_ENSURE_MONOTONICITY=1 -MD -MT libc/test/src/complex/CMakeFiles/libc.test.src.complex.cimagf128_test.__hermetic__.__build__.dir/cimagf128_test.cpp.o -MF libc/test/src/complex/CMakeFiles/libc.test.src.complex.cimagf128_test.__hermetic__.__build__.dir/cimagf128_test.cpp.o.d -o libc/test/src/complex/CMakeFiles/libc.test.src.complex.cimagf128_test.__hermetic__.__build__.dir/cimagf128_test.cpp.o -c /usr/local/google/home/michaelrj/llvm-project/libc/test/src/complex/cimagf128_test.cpp
1.	&lt;eof&gt; parser at end of file
2.	Optimizer
3.	Running pass "require&lt;globals-aa&gt;,function(invalidate&lt;aa&gt;),require&lt;profile-summary&gt;,cgscc(devirt&lt;4&gt;(inline,function-attrs&lt;skip-non-recursive-function-attrs&gt;,argpromotion,openmp-opt-cgscc,function&lt;eager-inv;no-rerun&gt;(sroa&lt;modify-cfg&gt;,early-cse&lt;memssa&gt;,speculative-execution&lt;only-if-divergent-target&gt;,jump-threading,correlated-propagation,simplifycfg&lt;bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables&gt;,instcombine&lt;max-iterations=1;no-use-loop-info;no-verify-fixpoint&gt;,aggressive-instcombine,libcalls-shrinkwrap,tailcallelim,simplifycfg&lt;bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables&gt;,reassociate,constraint-elimination,loop-mssa(loop-instsimplify,loop-simplifycfg,licm&lt;no-allowspeculation&gt;,loop-rotate&lt;header-duplication;no-prepare-for-lto&gt;,licm&lt;allowspeculation&gt;,simple-loop-unswitch&lt;nontrivial;trivial&gt;),simplifycfg&lt;bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables&gt;,instcombine&lt;max-iterations=1;no-use-loop-info;no-verify-fixpoint&gt;,loop(loop-idiom,indvars,simple-loop-unswitch&lt;nontrivial;trivial&gt;,loop-deletion,loop-unroll-full),sroa&lt;modify-cfg&gt;,vector-combine,mldst-motion&lt;no-split-footer-bb&gt;,gvn&lt;&gt;,sccp,bdce,instcombine&lt;max-iterations=1;no-use-loop-info;no-verify-fixpoint&gt;,jump-threading,correlated-propagation,adce,memcpyopt,dse,move-auto-init,loop-mssa(licm&lt;allowspeculation&gt;),coro-elide,simplifycfg&lt;bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-lookup;keep-loops;hoist-common-insts;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables&gt;,instcombine&lt;max-iterations=1;no-use-loop-info;no-verify-fixpoint&gt;),function-attrs,function(require&lt;should-not-run-function-passes&gt;),coro-split)),function(invalidate&lt;should-not-run-function-passes&gt;),cgscc(devirt&lt;4&gt;())" on module "/usr/local/google/home/michaelrj/llvm-project/libc/test/src/complex/cimagf128_test.cpp"
4.	Running pass "cgscc(devirt&lt;4&gt;(inline,function-attrs&lt;skip-non-recursive-function-attrs&gt;,argpromotion,openmp-opt-cgscc,function&lt;eager-inv;no-rerun&gt;(sroa&lt;modify-cfg&gt;,early-cse&lt;memssa&gt;,speculative-execution&lt;only-if-divergent-target&gt;,jump-threading,correlated-propagation,simplifycfg&lt;bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables&gt;,instcombine&lt;max-iterations=1;no-use-loop-info;no-verify-fixpoint&gt;,aggressive-instcombine,libcalls-shrinkwrap,tailcallelim,simplifycfg&lt;bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables&gt;,reassociate,constraint-elimination,loop-mssa(loop-instsimplify,loop-simplifycfg,licm&lt;no-allowspeculation&gt;,loop-rotate&lt;header-duplication;no-prepare-for-lto&gt;,licm&lt;allowspeculation&gt;,simple-loop-unswitch&lt;nontrivial;trivial&gt;),simplifycfg&lt;bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables&gt;,instcombine&lt;max-iterations=1;no-use-loop-info;no-verify-fixpoint&gt;,loop(loop-idiom,indvars,simple-loop-unswitch&lt;nontrivial;trivial&gt;,loop-deletion,loop-unroll-full),sroa&lt;modify-cfg&gt;,vector-combine,mldst-motion&lt;no-split-footer-bb&gt;,gvn&lt;&gt;,sccp,bdce,instcombine&lt;max-iterations=1;no-use-loop-info;no-verify-fixpoint&gt;,jump-threading,correlated-propagation,adce,memcpyopt,dse,move-auto-init,loop-mssa(licm&lt;allowspeculation&gt;),coro-elide,simplifycfg&lt;bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-lookup;keep-loops;hoist-common-insts;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables&gt;,instcombine&lt;max-iterations=1;no-use-loop-info;no-verify-fixpoint&gt;),function-attrs,function(require&lt;should-not-run-function-passes&gt;),coro-split))" on module "/usr/local/google/home/michaelrj/llvm-project/libc/test/src/complex/cimagf128_test.cpp"
Stack dump without symbol names (ensure you have llvm-symbolizer in your PATH or set the environment var `LLVM_SYMBOLIZER_PATH` to point to it):
0  libLLVM.so.19.1      0x00007f35e56b067a llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) + 58
1  libLLVM.so.19.1      0x00007f35e56ae224 llvm::sys::RunSignalHandlers() + 84
2  libLLVM.so.19.1      0x00007f35e55f4144
3  libc.so.6            0x00007f35e4249df0
4  libLLVM.so.19.1      0x00007f35e6bb290a
5  libLLVM.so.19.1      0x00007f35e6baf6ac llvm::ConstantFoldCall(llvm::CallBase const*, llvm::Function*, llvm::ArrayRef&lt;llvm::Constant*&gt;, llvm::TargetLibraryInfo const*, bool) + 316
6  libLLVM.so.19.1      0x00007f35e6c1e1d3
7  libLLVM.so.19.1      0x00007f35e6c199a1
8  libLLVM.so.19.1      0x00007f35e6c1576b
9  libLLVM.so.19.1      0x00007f35e6c149ef llvm::getInlineCost(llvm::CallBase&amp;, llvm::Function*, llvm::InlineParams const&amp;, llvm::TargetTransformInfo&amp;, llvm::function_ref&lt;llvm::AssumptionCache&amp; (llvm::Function&amp;)&gt;, llvm::function_ref&lt;llvm::TargetLibraryInfo const&amp; (llvm::Function&amp;)&gt;, llvm::function_ref&lt;llvm::BlockFrequencyInfo&amp; (llvm::Function&amp;)&gt;, llvm::ProfileSummaryInfo*, llvm::OptimizationRemarkEmitter*) + 319
10 libLLVM.so.19.1      0x00007f35e6c1489f llvm::getInlineCost(llvm::CallBase&amp;, llvm::InlineParams const&amp;, llvm::TargetTransformInfo&amp;, llvm::function_ref&lt;llvm::AssumptionCache&amp; (llvm::Function&amp;)&gt;, llvm::function_ref&lt;llvm::TargetLibraryInfo const&amp; (llvm::Function&amp;)&gt;, llvm::function_ref&lt;llvm::BlockFrequencyInfo&amp; (llvm::Function&amp;)&gt;, llvm::ProfileSummaryInfo*, llvm::OptimizationRemarkEmitter*) + 159
11 libLLVM.so.19.1      0x00007f35e6c24b5c
12 libLLVM.so.19.1      0x00007f35e6c21c7b llvm::shouldInline(llvm::CallBase&amp;, llvm::function_ref&lt;llvm::InlineCost (llvm::CallBase&amp;)&gt;, llvm::OptimizationRemarkEmitter&amp;, bool) + 75
13 libLLVM.so.19.1      0x00007f35e6c2122b
14 libLLVM.so.19.1      0x00007f35e6c20fae llvm::DefaultInlineAdvisor::getAdviceImpl(llvm::CallBase&amp;) + 62
15 libLLVM.so.19.1      0x00007f35e6c24361 llvm::InlineAdvisor::getAdvice(llvm::CallBase&amp;, bool) + 225
16 libLLVM.so.19.1      0x00007f35e68c44d2 llvm::InlinerPass::run(llvm::LazyCallGraph::SCC&amp;, llvm::AnalysisManager&lt;llvm::LazyCallGraph::SCC, llvm::LazyCallGraph&amp;&gt;&amp;, llvm::LazyCallGraph&amp;, llvm::CGSCCUpdateResult&amp;) + 3122
17 libLLVM.so.19.1      0x00007f35e68c8ba1
18 libLLVM.so.19.1      0x00007f35e6b91a07 llvm::PassManager&lt;llvm::LazyCallGraph::SCC, llvm::AnalysisManager&lt;llvm::LazyCallGraph::SCC, llvm::LazyCallGraph&amp;&gt;, llvm::LazyCallGraph&amp;, llvm::CGSCCUpdateResult&amp;&gt;::run(llvm::LazyCallGraph::SCC&amp;, llvm::AnalysisManager&lt;llvm::LazyCallGraph::SCC, llvm::LazyCallGraph&amp;&gt;&amp;, llvm::LazyCallGraph&amp;, llvm::CGSCCUpdateResult&amp;) + 519
19 libLLVM.so.19.1      0x00007f35e68c9011
20 libLLVM.so.19.1      0x00007f35e6b93440 llvm::DevirtSCCRepeatedPass::run(llvm::LazyCallGraph::SCC&amp;, llvm::AnalysisManager&lt;llvm::LazyCallGraph::SCC, llvm::LazyCallGraph&amp;&gt;&amp;, llvm::LazyCallGraph&amp;, llvm::CGSCCUpdateResult&amp;) + 320
21 libLLVM.so.19.1      0x00007f35e68c95d1
22 libLLVM.so.19.1      0x00007f35e6b92b6f llvm::ModuleToPostOrderCGSCCPassAdaptor::run(llvm::Module&amp;, llvm::AnalysisManager&lt;llvm::Module&gt;&amp;) + 2383
23 libLLVM.so.19.1      0x00007f35e68c93d1
24 libLLVM.so.19.1      0x00007f35e583c729 llvm::PassManager&lt;llvm::Module, llvm::AnalysisManager&lt;llvm::Module&gt;&gt;::run(llvm::Module&amp;, llvm::AnalysisManager&lt;llvm::Module&gt;&amp;) + 489
25 libLLVM.so.19.1      0x00007f35e68c64ec llvm::ModuleInlinerWrapperPass::run(llvm::Module&amp;, llvm::AnalysisManager&lt;llvm::Module&gt;&amp;) + 604
26 libLLVM.so.19.1      0x00007f35e89fab11
27 libLLVM.so.19.1      0x00007f35e583c729 llvm::PassManager&lt;llvm::Module, llvm::AnalysisManager&lt;llvm::Module&gt;&gt;::run(llvm::Module&amp;, llvm::AnalysisManager&lt;llvm::Module&gt;&amp;) + 489
28 libclang-cpp.so.19.1 0x00007f35ee4856f3
29 libclang-cpp.so.19.1 0x00007f35ee47cd9a clang::EmitBackendOutput(clang::DiagnosticsEngine&amp;, clang::HeaderSearchOptions const&amp;, clang::CodeGenOptions const&amp;, clang::TargetOptions const&amp;, clang::LangOptions const&amp;, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;, std::unique_ptr&lt;llvm::raw_pwrite_stream, std::default_delete&lt;llvm::raw_pwrite_stream&gt;&gt;, clang::BackendConsumer*) + 6954
30 libclang-cpp.so.19.1 0x00007f35ee82a551 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&amp;) + 1953
31 libclang-cpp.so.19.1 0x00007f35ed2344e9 clang::ParseAST(clang::Sema&amp;, bool, bool) + 665
32 libclang-cpp.so.19.1 0x00007f35ef347789 clang::FrontendAction::Execute() + 89
33 libclang-cpp.so.19.1 0x00007f35ef2b6084 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) + 1012
34 libclang-cpp.so.19.1 0x00007f35ef3c5c0e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) + 702
35 clang++              0x0000560953b03c61 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) + 5905
36 clang++              0x0000560953b00b35
37 libclang-cpp.so.19.1 0x00007f35eef354cd
38 libLLVM.so.19.1      0x00007f35e55f3ee0 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) + 144
39 libclang-cpp.so.19.1 0x00007f35eef34f61 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const + 369
40 libclang-cpp.so.19.1 0x00007f35eeefa198 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const + 920
41 libclang-cpp.so.19.1 0x00007f35eeefa40e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const + 142
42 libclang-cpp.so.19.1 0x00007f35eef17bcc clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) + 348
43 clang++              0x0000560953b00574 clang_main(int, char**, llvm::ToolContext const&amp;) + 6820
44 clang++              0x0000560953b0e4ba main + 106
45 libc.so.6            0x00007f35e4233ca8
46 libc.so.6            0x00007f35e4233d65 __libc_start_main + 133
47 clang++              0x0000560953afe731 _start + 33
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Debian clang version 19.1.7 (3+build5)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/lib/llvm-19/bin
clang++: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: /tmp/cimagf128_test-fa9fb3.cpp
clang++: note: diagnostic msg: /tmp/cimagf128_test-fa9fb3.sh
clang++: note: diagnostic msg: 

********************
```

[cimagf128_test-fa9fb3.cpp](https://github.com/user-attachments/files/24109766/cimagf128_test-fa9fb3.cpp)
[cimagf128_test-fa9fb3.sh](https://github.com/user-attachments/files/24109764/cimagf128_test-fa9fb3.sh)
[libc_cmake_diff.txt](https://github.com/user-attachments/files/24109765/libc_cmake_diff.txt)
</details>


---

