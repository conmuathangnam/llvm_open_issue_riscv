# [Polly] Assertion "pivot >= 0" failed in isl_mat.c:1147 during ScopBuilder::buildDomains

**Author:** thapgua
**URL:** https://github.com/llvm/llvm-project/issues/179993
**Status:** Closed
**Labels:** polly, crash
**Closed Date:** 2026-02-23T00:09:37Z

## Body

A crash occurred when compiling with `-O3 -mllvm -polly`. 
The crash is an assertion failure in the underlying `isl` library used by Polly.

Version: 
```
clang version 23.0.0git (https://github.com/llvm/llvm-project.git d737229efdb3f3949f9c483d256eea2b946808a9)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /data/zhangguangwei/compiler-build/llvm/llvm-project/build/bin
Build config: +assertions
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/13
Selected GCC installation: /usr/lib/gcc/x86_64-linux-gnu/13
Candidate multilib: .;@m64
Selected multilib: .;@m64
```

[bug-eb3f22.c](https://github.com/user-attachments/files/25107965/bug-eb3f22.c)

[bug-eb3f22.sh](https://github.com/user-attachments/files/25107968/bug-eb3f22.sh)

Backtrace: 
```
/data/lenovo/compiler-build/llvm/llvm-project/polly/lib/External/isl/isl_mat.c:1147: Assertion "pivot >= 0" failed
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang bug.c -c -O3 -mllvm -polly
1.      <eof> parser at end of file
2.      Optimizer
3.      Running pass "function<eager-inv>(drop-unnecessary-assumes,float2int,lower-constant-intrinsics,chr,polly,sroa<modify-cfg>,early-cse<memssa>,speculative-execution<only-if-divergent-target>,jump-threading,correlated-propagation,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,instcombine<max-iterations=1;no-verify-fixpoint>,aggressive-instcombine,libcalls-shrinkwrap,tailcallelim,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,reassociate,constraint-elimination,loop-mssa(loop-instsimplify,loop-simplifycfg,licm<no-allowspeculation>,loop-rotate<header-duplication;no-prepare-for-lto>,licm<allowspeculation>,simple-loop-unswitch<nontrivial;trivial>),simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,instcombine<max-iterations=1;no-verify-fixpoint>,loop(loop-idiom,indvars,extra-simple-loop-unswitch-passes,loop-deletion,loop-unroll-full),sroa<modify-cfg>,vector-combine,mldst-motion<no-split-footer-bb>,gvn<>,sccp,bdce,instcombine<max-iterations=1;no-verify-fixpoint>,jump-threading,correlated-propagation,adce,memcpyopt,dse,move-auto-init,loop-mssa(licm<allowspeculation>),coro-elide,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;no-switch-to-lookup;keep-loops;hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,instcombine<max-iterations=1;no-verify-fixpoint>,loop(loop-rotate<header-duplication;no-prepare-for-lto>,loop-deletion),loop-distribute,inject-tli-mappings,loop-vectorize<no-interleave-forced-only;no-vectorize-forced-only;>,drop-unnecessary-assumes,infer-alignment,loop-load-elim,instcombine<max-iterations=1;no-verify-fixpoint>,simplifycfg<bonus-inst-threshold=1;forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;switch-to-lookup;no-keep-loops;hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,slp-vectorizer,vector-combine,instcombine<max-iterations=1;no-verify-fixpoint>,loop-unroll<O3>,transform-warning,sroa<preserve-cfg>,infer-alignment,instcombine<max-iterations=1;no-verify-fixpoint>,loop-mssa(licm<allowspeculation>),alignment-from-assumptions,loop-sink,instsimplify,div-rem-pairs,tailcallelim,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;speculate-unpredictables>)" on module "bug.c"
4.      Running pass "polly" on function "c"
 #0 0x000060730a93d7d2 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang+0x45d87d2)
 #1 0x000060730a93a4ff llvm::sys::RunSignalHandlers() (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang+0x45d54ff)
 #2 0x000060730a868ce8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007bcfacc45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x00007bcfacc9eb2c pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x9eb2c)
 #5 0x00007bcfacc4527e raise (/lib/x86_64-linux-gnu/libc.so.6+0x4527e)
 #6 0x00007bcfacc288ff abort (/lib/x86_64-linux-gnu/libc.so.6+0x288ff)
 #7 0x000060730c97cba1 (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang+0x6617ba1)
 #8 0x000060730c97d4bc (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang+0x66184bc)
 #9 0x000060730c9fed30 isl_mat_right_inverse (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang+0x6699d30)
#10 0x000060730c9774a0 isl_set_wrap_facet (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang+0x66124a0)
#11 0x000060730c963d73 add_wrap isl_coalesce.c:0:0
#12 0x000060730c964178 add_selected_wraps isl_coalesce.c:0:0
#13 0x000060730c964ec2 can_wrap_in_facet isl_coalesce.c:0:0
#14 0x000060730c967fc6 coalesce_local_pair_reuse isl_coalesce.c:0:0
#15 0x000060730c96a4ec coalesce_after_aligning_divs isl_coalesce.c:0:0
#16 0x000060730c96ba84 coalesce_range isl_coalesce.c:0:0
#17 0x000060730c96c27a isl_map_coalesce (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang+0x660727a)
#18 0x000060730ba757d7 polly::ScopBuilder::propagateDomainConstraints(llvm::Region*, llvm::DenseMap<llvm::BasicBlock*, isl::set, llvm::DenseMapInfo<llvm::BasicBlock*, void>, llvm::detail::DenseMapPair<llvm::BasicBlock*, isl::set>>&) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang+0x57107d7)
#19 0x000060730ba75c96 polly::ScopBuilder::buildDomains(llvm::Region*, llvm::DenseMap<llvm::BasicBlock*, isl::set, llvm::DenseMapInfo<llvm::BasicBlock*, void>, llvm::detail::DenseMapPair<llvm::BasicBlock*, isl::set>>&) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang+0x5710c96)
#20 0x000060730ba83454 polly::ScopBuilder::buildScop(llvm::Region&, llvm::AssumptionCache&) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang+0x571e454)
#21 0x000060730ba84738 polly::ScopBuilder::ScopBuilder(llvm::Region*, llvm::AssumptionCache&, llvm::AAResults&, llvm::DataLayout const&, llvm::DominatorTree&, llvm::LoopInfo&, polly::ScopDetection&, llvm::ScalarEvolution&, llvm::OptimizationRemarkEmitter&) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang+0x571f738)
#22 0x000060730ba5f432 polly::ScopInfo::recompute() (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang+0x56fa432)
#23 0x000060730ba959b9 polly::runPollyPass(llvm::Function&, llvm::AnalysisManager<llvm::Function>&, polly::PollyPassOptions) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang+0x57309b9)
#24 0x000060730ba978af polly::PollyFunctionPass::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang+0x57328af)
#25 0x000060730ba0b835 llvm::detail::PassModel<llvm::Function, polly::PollyFunctionPass, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang+0x56a6835)
#26 0x000060730a231d35 llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang+0x3eccd35)
#27 0x0000607307802045 llvm::detail::PassModel<llvm::Function, llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang+0x149d045)
#28 0x000060730a230481 llvm::ModuleToFunctionPassAdaptor::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang+0x3ecb481)
#29 0x00006073078026e5 llvm::detail::PassModel<llvm::Module, llvm::ModuleToFunctionPassAdaptor, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang+0x149d6e5)
#30 0x000060730a230c6d llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang+0x3ecbc6d)
#31 0x000060730abff02e (anonymous namespace)::EmitAssemblyHelper::RunOptimizationPipeline(clang::BackendAction, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>&, std::unique_ptr<llvm::ToolOutputFile, std::default_delete<llvm::ToolOutputFile>>&, clang::BackendConsumer*) BackendUtil.cpp:0:0
#32 0x000060730ac02d97 clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang+0x489dd97)
#33 0x000060730b29ec48 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang+0x4f39c48)
#34 0x000060730d1887cc clang::ParseAST(clang::Sema&, bool, bool) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang+0x6e237cc)
#35 0x000060730b5d73d7 clang::FrontendAction::Execute() (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang+0x52723d7)
#36 0x000060730b55afbd clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang+0x51f5fbd)
#37 0x000060730b6d455e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang+0x536f55e)
#38 0x0000607307352aef cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang+0xfedaef)
#39 0x0000607307348037 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#40 0x0000607307348215 int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#41 0x000060730b317e1d void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#42 0x000060730a8691ae llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang+0x45041ae)
#43 0x000060730b318f28 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#44 0x000060730b2d5f07 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang+0x4f70f07)
#45 0x000060730b2d7002 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang+0x4f72002)
#46 0x000060730b2e2a34 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang+0x4f7da34)
#47 0x000060730734d98c clang_main(int, char**, llvm::ToolContext const&) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang+0xfe898c)
#48 0x00006073071d332a main (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang+0xe6e32a)
#49 0x00007bcfacc2a1ca (/lib/x86_64-linux-gnu/libc.so.6+0x2a1ca)
#50 0x00007bcfacc2a28b __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x2a28b)
#51 0x00006073073475a5 _start (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang+0xfe25a5)
clang: error: clang frontend command failed with exit code 134 (use -v to see invocation)
clang version 23.0.0git (https://github.com/llvm/llvm-project.git d737229efdb3f3949f9c483d256eea2b946808a9)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /data/lenovo/compiler-build/llvm/llvm-project/build/bin
Build config: +assertions
clang: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang: note: diagnostic msg: /tmp/bug-eb3f22.c
clang: note: diagnostic msg: /tmp/bug-eb3f22.sh
clang: note: diagnostic msg: 

********************
```

Failed Code: 
```c
char a;
int b;
void c(int d, int e, int f, int g, int h[][3], char i, int j[][2][12],
       int k[][2][2], int l[][2][12], char m[][8], int n[], int o[][2]) {
  for (;;)
    for (int p = g; p < 0;)
      for (int q; q < 1; q += 807879) {
        for (char r; r; r += 3)
          ;
        for (int s = (i ?: h[p][q]) - 2; s < 0; s++) {
          a = p % 2 == 0 ? (char)(o ? k[0][0][p] : f)
                         : (char)(o[1][2]
                                      ?: m[p][q] ? j[2][s][2]
                                                 : 0);
          b = l[6][q][q] << e | d ? n[1] : 0;
        }
        for (int t; t; t++)
          ;
        for (int u; u; u++)
          ;
      }
}
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-polly

Author: None (thapgua)

<details>
A crash occurred when compiling with `-O3 -mllvm -polly`. 
The crash is an assertion failure in the underlying `isl` library used by Polly.

Version: 
```
clang version 23.0.0git (https://github.com/llvm/llvm-project.git d737229efdb3f3949f9c483d256eea2b946808a9)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /data/zhangguangwei/compiler-build/llvm/llvm-project/build/bin
Build config: +assertions
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/13
Selected GCC installation: /usr/lib/gcc/x86_64-linux-gnu/13
Candidate multilib: .;@<!-- -->m64
Selected multilib: .;@<!-- -->m64
```

[bug-eb3f22.c](https://github.com/user-attachments/files/25107965/bug-eb3f22.c)

[bug-eb3f22.sh](https://github.com/user-attachments/files/25107968/bug-eb3f22.sh)

Backtrace: 
```
/data/lenovo/compiler-build/llvm/llvm-project/polly/lib/External/isl/isl_mat.c:1147: Assertion "pivot &gt;= 0" failed
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang bug.c -c -O3 -mllvm -polly
1.      &lt;eof&gt; parser at end of file
2.      Optimizer
3.      Running pass "function&lt;eager-inv&gt;(drop-unnecessary-assumes,float2int,lower-constant-intrinsics,chr,polly,sroa&lt;modify-cfg&gt;,early-cse&lt;memssa&gt;,speculative-execution&lt;only-if-divergent-target&gt;,jump-threading,correlated-propagation,simplifycfg&lt;bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables&gt;,instcombine&lt;max-iterations=1;no-verify-fixpoint&gt;,aggressive-instcombine,libcalls-shrinkwrap,tailcallelim,simplifycfg&lt;bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables&gt;,reassociate,constraint-elimination,loop-mssa(loop-instsimplify,loop-simplifycfg,licm&lt;no-allowspeculation&gt;,loop-rotate&lt;header-duplication;no-prepare-for-lto&gt;,licm&lt;allowspeculation&gt;,simple-loop-unswitch&lt;nontrivial;trivial&gt;),simplifycfg&lt;bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables&gt;,instcombine&lt;max-iterations=1;no-verify-fixpoint&gt;,loop(loop-idiom,indvars,extra-simple-loop-unswitch-passes,loop-deletion,loop-unroll-full),sroa&lt;modify-cfg&gt;,vector-combine,mldst-motion&lt;no-split-footer-bb&gt;,gvn&lt;&gt;,sccp,bdce,instcombine&lt;max-iterations=1;no-verify-fixpoint&gt;,jump-threading,correlated-propagation,adce,memcpyopt,dse,move-auto-init,loop-mssa(licm&lt;allowspeculation&gt;),coro-elide,simplifycfg&lt;bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;no-switch-to-lookup;keep-loops;hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables&gt;,instcombine&lt;max-iterations=1;no-verify-fixpoint&gt;,loop(loop-rotate&lt;header-duplication;no-prepare-for-lto&gt;,loop-deletion),loop-distribute,inject-tli-mappings,loop-vectorize&lt;no-interleave-forced-only;no-vectorize-forced-only;&gt;,drop-unnecessary-assumes,infer-alignment,loop-load-elim,instcombine&lt;max-iterations=1;no-verify-fixpoint&gt;,simplifycfg&lt;bonus-inst-threshold=1;forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;switch-to-lookup;no-keep-loops;hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables&gt;,slp-vectorizer,vector-combine,instcombine&lt;max-iterations=1;no-verify-fixpoint&gt;,loop-unroll&lt;O3&gt;,transform-warning,sroa&lt;preserve-cfg&gt;,infer-alignment,instcombine&lt;max-iterations=1;no-verify-fixpoint&gt;,loop-mssa(licm&lt;allowspeculation&gt;),alignment-from-assumptions,loop-sink,instsimplify,div-rem-pairs,tailcallelim,simplifycfg&lt;bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;speculate-unpredictables&gt;)" on module "bug.c"
4.      Running pass "polly" on function "c"
 #<!-- -->0 0x000060730a93d7d2 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang+0x45d87d2)
 #<!-- -->1 0x000060730a93a4ff llvm::sys::RunSignalHandlers() (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang+0x45d54ff)
 #<!-- -->2 0x000060730a868ce8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007bcfacc45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->4 0x00007bcfacc9eb2c pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x9eb2c)
 #<!-- -->5 0x00007bcfacc4527e raise (/lib/x86_64-linux-gnu/libc.so.6+0x4527e)
 #<!-- -->6 0x00007bcfacc288ff abort (/lib/x86_64-linux-gnu/libc.so.6+0x288ff)
 #<!-- -->7 0x000060730c97cba1 (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang+0x6617ba1)
 #<!-- -->8 0x000060730c97d4bc (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang+0x66184bc)
 #<!-- -->9 0x000060730c9fed30 isl_mat_right_inverse (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang+0x6699d30)
#<!-- -->10 0x000060730c9774a0 isl_set_wrap_facet (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang+0x66124a0)
#<!-- -->11 0x000060730c963d73 add_wrap isl_coalesce.c:0:0
#<!-- -->12 0x000060730c964178 add_selected_wraps isl_coalesce.c:0:0
#<!-- -->13 0x000060730c964ec2 can_wrap_in_facet isl_coalesce.c:0:0
#<!-- -->14 0x000060730c967fc6 coalesce_local_pair_reuse isl_coalesce.c:0:0
#<!-- -->15 0x000060730c96a4ec coalesce_after_aligning_divs isl_coalesce.c:0:0
#<!-- -->16 0x000060730c96ba84 coalesce_range isl_coalesce.c:0:0
#<!-- -->17 0x000060730c96c27a isl_map_coalesce (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang+0x660727a)
#<!-- -->18 0x000060730ba757d7 polly::ScopBuilder::propagateDomainConstraints(llvm::Region*, llvm::DenseMap&lt;llvm::BasicBlock*, isl::set, llvm::DenseMapInfo&lt;llvm::BasicBlock*, void&gt;, llvm::detail::DenseMapPair&lt;llvm::BasicBlock*, isl::set&gt;&gt;&amp;) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang+0x57107d7)
#<!-- -->19 0x000060730ba75c96 polly::ScopBuilder::buildDomains(llvm::Region*, llvm::DenseMap&lt;llvm::BasicBlock*, isl::set, llvm::DenseMapInfo&lt;llvm::BasicBlock*, void&gt;, llvm::detail::DenseMapPair&lt;llvm::BasicBlock*, isl::set&gt;&gt;&amp;) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang+0x5710c96)
#<!-- -->20 0x000060730ba83454 polly::ScopBuilder::buildScop(llvm::Region&amp;, llvm::AssumptionCache&amp;) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang+0x571e454)
#<!-- -->21 0x000060730ba84738 polly::ScopBuilder::ScopBuilder(llvm::Region*, llvm::AssumptionCache&amp;, llvm::AAResults&amp;, llvm::DataLayout const&amp;, llvm::DominatorTree&amp;, llvm::LoopInfo&amp;, polly::ScopDetection&amp;, llvm::ScalarEvolution&amp;, llvm::OptimizationRemarkEmitter&amp;) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang+0x571f738)
#<!-- -->22 0x000060730ba5f432 polly::ScopInfo::recompute() (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang+0x56fa432)
#<!-- -->23 0x000060730ba959b9 polly::runPollyPass(llvm::Function&amp;, llvm::AnalysisManager&lt;llvm::Function&gt;&amp;, polly::PollyPassOptions) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang+0x57309b9)
#<!-- -->24 0x000060730ba978af polly::PollyFunctionPass::run(llvm::Function&amp;, llvm::AnalysisManager&lt;llvm::Function&gt;&amp;) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang+0x57328af)
#<!-- -->25 0x000060730ba0b835 llvm::detail::PassModel&lt;llvm::Function, polly::PollyFunctionPass, llvm::AnalysisManager&lt;llvm::Function&gt;&gt;::run(llvm::Function&amp;, llvm::AnalysisManager&lt;llvm::Function&gt;&amp;) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang+0x56a6835)
#<!-- -->26 0x000060730a231d35 llvm::PassManager&lt;llvm::Function, llvm::AnalysisManager&lt;llvm::Function&gt;&gt;::run(llvm::Function&amp;, llvm::AnalysisManager&lt;llvm::Function&gt;&amp;) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang+0x3eccd35)
#<!-- -->27 0x0000607307802045 llvm::detail::PassModel&lt;llvm::Function, llvm::PassManager&lt;llvm::Function, llvm::AnalysisManager&lt;llvm::Function&gt;&gt;, llvm::AnalysisManager&lt;llvm::Function&gt;&gt;::run(llvm::Function&amp;, llvm::AnalysisManager&lt;llvm::Function&gt;&amp;) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang+0x149d045)
#<!-- -->28 0x000060730a230481 llvm::ModuleToFunctionPassAdaptor::run(llvm::Module&amp;, llvm::AnalysisManager&lt;llvm::Module&gt;&amp;) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang+0x3ecb481)
#<!-- -->29 0x00006073078026e5 llvm::detail::PassModel&lt;llvm::Module, llvm::ModuleToFunctionPassAdaptor, llvm::AnalysisManager&lt;llvm::Module&gt;&gt;::run(llvm::Module&amp;, llvm::AnalysisManager&lt;llvm::Module&gt;&amp;) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang+0x149d6e5)
#<!-- -->30 0x000060730a230c6d llvm::PassManager&lt;llvm::Module, llvm::AnalysisManager&lt;llvm::Module&gt;&gt;::run(llvm::Module&amp;, llvm::AnalysisManager&lt;llvm::Module&gt;&amp;) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang+0x3ecbc6d)
#<!-- -->31 0x000060730abff02e (anonymous namespace)::EmitAssemblyHelper::RunOptimizationPipeline(clang::BackendAction, std::unique_ptr&lt;llvm::raw_pwrite_stream, std::default_delete&lt;llvm::raw_pwrite_stream&gt;&gt;&amp;, std::unique_ptr&lt;llvm::ToolOutputFile, std::default_delete&lt;llvm::ToolOutputFile&gt;&gt;&amp;, clang::BackendConsumer*) BackendUtil.cpp:0:0
#<!-- -->32 0x000060730ac02d97 clang::emitBackendOutput(clang::CompilerInstance&amp;, clang::CodeGenOptions&amp;, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;, std::unique_ptr&lt;llvm::raw_pwrite_stream, std::default_delete&lt;llvm::raw_pwrite_stream&gt;&gt;, clang::BackendConsumer*) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang+0x489dd97)
#<!-- -->33 0x000060730b29ec48 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&amp;) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang+0x4f39c48)
#<!-- -->34 0x000060730d1887cc clang::ParseAST(clang::Sema&amp;, bool, bool) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang+0x6e237cc)
#<!-- -->35 0x000060730b5d73d7 clang::FrontendAction::Execute() (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang+0x52723d7)
#<!-- -->36 0x000060730b55afbd clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang+0x51f5fbd)
#<!-- -->37 0x000060730b6d455e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang+0x536f55e)
#<!-- -->38 0x0000607307352aef cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang+0xfedaef)
#<!-- -->39 0x0000607307348037 ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->40 0x0000607307348215 int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->41 0x000060730b317e1d void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->42 0x000060730a8691ae llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang+0x45041ae)
#<!-- -->43 0x000060730b318f28 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->44 0x000060730b2d5f07 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang+0x4f70f07)
#<!-- -->45 0x000060730b2d7002 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang+0x4f72002)
#<!-- -->46 0x000060730b2e2a34 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang+0x4f7da34)
#<!-- -->47 0x000060730734d98c clang_main(int, char**, llvm::ToolContext const&amp;) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang+0xfe898c)
#<!-- -->48 0x00006073071d332a main (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang+0xe6e32a)
#<!-- -->49 0x00007bcfacc2a1ca (/lib/x86_64-linux-gnu/libc.so.6+0x2a1ca)
#<!-- -->50 0x00007bcfacc2a28b __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x2a28b)
#<!-- -->51 0x00006073073475a5 _start (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang+0xfe25a5)
clang: error: clang frontend command failed with exit code 134 (use -v to see invocation)
clang version 23.0.0git (https://github.com/llvm/llvm-project.git d737229efdb3f3949f9c483d256eea2b946808a9)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /data/lenovo/compiler-build/llvm/llvm-project/build/bin
Build config: +assertions
clang: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang: note: diagnostic msg: /tmp/bug-eb3f22.c
clang: note: diagnostic msg: /tmp/bug-eb3f22.sh
clang: note: diagnostic msg: 

********************
```

Failed Code: 
```c
char a;
int b;
void c(int d, int e, int f, int g, int h[][3], char i, int j[][2][12],
       int k[][2][2], int l[][2][12], char m[][8], int n[], int o[][2]) {
  for (;;)
    for (int p = g; p &lt; 0;)
      for (int q; q &lt; 1; q += 807879) {
        for (char r; r; r += 3)
          ;
        for (int s = (i ?: h[p][q]) - 2; s &lt; 0; s++) {
          a = p % 2 == 0 ? (char)(o ? k[0][0][p] : f)
                         : (char)(o[1][2]
                                      ?: m[p][q] ? j[2][s][2]
                                                 : 0);
          b = l[6][q][q] &lt;&lt; e | d ? n[1] : 0;
        }
        for (int t; t; t++)
          ;
        for (int u; u; u++)
          ;
      }
}
```
</details>


---

### Comment 2 - Meinersbur

Reduced test case: [pr179993.ll.txt](https://github.com/user-attachments/files/25156649/pr179993.ll.txt)

Upstream bug report: https://groups.google.com/g/isl-development/c/o2ZxXc5ssrg

---

