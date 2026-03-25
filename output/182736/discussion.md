# [Polly] Segmentation fault in isl_basic_map_remove_duplicate_constraints

**Author:** thapgua
**URL:** https://github.com/llvm/llvm-project/issues/182736
**Status:** Closed
**Labels:** polly, crash
**Closed Date:** 2026-02-23T00:11:53Z

## Body

Clang++ crashes with `-mllvm -polly -O3` while compiling the given source code. The crash results in a Segmentation fault within `isl_basic_map_remove_duplicate_constraints` during the "polly" pass.

Compiler version: 
```
clang version 23.0.0git (https://github.com/llvm/llvm-project.git 6e9a308a884d18315fa3cb31693718148dfa63dd)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /data/lenovo/compiler-build/llvm/llvm-project/build/bin
Build config: +assertions
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/13
Selected GCC installation: /usr/lib/gcc/x86_64-linux-gnu/13
Candidate multilib: .;@m64
Selected multilib: .;@m64
```

Attached files: (The preprocessed .cpp file is relatively large due to the inclusion of the header file `<algorithm>`.)

[bug-5f6bd7.cpp](https://github.com/user-attachments/files/25467081/bug-5f6bd7.cpp)

[bug-5f6bd7.sh](https://github.com/user-attachments/files/25467082/bug-5f6bd7.sh)

Backtrace: 
```
bug.cpp:10:16: warning: integer literal is too large to be represented in a signed integer type, interpreting as unsigned
      [-Wimplicitly-unsigned-literal]
   10 |   [[assume(h - 14211339058593952328 == 19)]];
      |                ^
bug.cpp:12:23: warning: integer literal is too large to be represented in a signed integer type, interpreting as unsigned
      [-Wimplicitly-unsigned-literal]
   12 |   for (int p; p < h - 14211339058593952328; p += l - 4413ULL) {
      |                       ^
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23 -cc1 -triple x86_64-unknown-linux-gnu -O3 -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -main-file-name bug.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=none -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/home/lenovo/bug-report/26-2-22 -fcoverage-compilation-dir=/home/lenovo/bug-report/26-2-22 -resource-dir /data/lenovo/compiler-build/llvm/llvm-project/build/lib/clang/23 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/x86_64-linux-gnu/c++/13 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/backward -internal-isystem /data/lenovo/compiler-build/llvm/llvm-project/build/lib/clang/23/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -fdeprecated-macro -ferror-limit 19 -fmessage-length=135 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -vectorize-loops -vectorize-slp -mllvm -polly -faddrsig -fdwarf2-cfi-asm -o /tmp/bug-4b456e.o -x c++ bug.cpp
1.      <eof> parser at end of file
2.      Optimizer
3.      Running pass "function<eager-inv>(drop-unnecessary-assumes,float2int,lower-constant-intrinsics,chr,polly,sroa<modify-cfg>,early-cse<memssa>,speculative-execution<only-if-divergent-target>,jump-threading,correlated-propagation,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,instcombine<max-iterations=1;no-verify-fixpoint>,aggressive-instcombine,libcalls-shrinkwrap,tailcallelim,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,reassociate,constraint-elimination,loop-mssa(loop-instsimplify,loop-simplifycfg,licm<no-allowspeculation>,loop-rotate<header-duplication;no-prepare-for-lto>,licm<allowspeculation>,simple-loop-unswitch<nontrivial;trivial>),simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,instcombine<max-iterations=1;no-verify-fixpoint>,loop(loop-idiom,indvars,extra-simple-loop-unswitch-passes,loop-deletion,loop-unroll-full),sroa<modify-cfg>,vector-combine,mldst-motion<no-split-footer-bb>,gvn<>,sccp,bdce,instcombine<max-iterations=1;no-verify-fixpoint>,jump-threading,correlated-propagation,adce,memcpyopt,dse,move-auto-init,loop-mssa(licm<allowspeculation>),coro-elide,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;no-switch-to-lookup;keep-loops;hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,instcombine<max-iterations=1;no-verify-fixpoint>,loop(loop-rotate<header-duplication;no-prepare-for-lto>,loop-deletion),loop-distribute,inject-tli-mappings,loop-vectorize<no-interleave-forced-only;no-vectorize-forced-only;>,drop-unnecessary-assumes,infer-alignment,loop-load-elim,instcombine<max-iterations=1;no-verify-fixpoint>,simplifycfg<bonus-inst-threshold=1;forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;switch-to-lookup;no-keep-loops;hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,slp-vectorizer,vector-combine,instcombine<max-iterations=1;no-verify-fixpoint>,loop-unroll<O3>,transform-warning,sroa<preserve-cfg>,infer-alignment,instcombine<max-iterations=1;no-verify-fixpoint>,loop-mssa(licm<allowspeculation>),alignment-from-assumptions,loop-sink,instsimplify,div-rem-pairs,tailcallelim,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;speculate-unpredictables>)" on module "bug.cpp"
4.      Running pass "polly" on function "_Z1glsiicsPbPA9_sS1_"
 #0 0x000058ab542af952 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x467a952)
 #1 0x000058ab542ac5af llvm::sys::RunSignalHandlers() (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x46775af)
 #2 0x000058ab542ac6fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x000058ab542ac7bc SignalHandlerTerminate(int, siginfo_t*, void*) Signals.cpp:0:0
 #4 0x00007120a2645330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #5 0x000058ab563932e9 isl_basic_map_remove_duplicate_constraints (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x675e2e9)
 #6 0x000058ab563989b3 isl_basic_map_simplify (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x67639b3)
 #7 0x000058ab563a181b basic_map_collect_diff.part.0 isl_map_subtract.c:0:0
 #8 0x000058ab563a2b39 isl_map_subtract (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x676db39)
 #9 0x000058ab5645a7f7 gen_bin_entry isl_union_map.c:0:0
#10 0x000058ab563347ac isl_hash_table_foreach (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x66ff7ac)
#11 0x000058ab56458458 gen_bin_op isl_union_map.c:0:0
#12 0x000058ab5645bfb0 isl_union_set_subtract (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x6826fb0)
#13 0x000058ab555e6b1b polly::ZoneAlgorithm::computeKnownFromLoad() const (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x59b1b1b)
#14 0x000058ab555e73db polly::ZoneAlgorithm::computeKnown(bool, bool) const (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x59b23db)
#15 0x000058ab555d6491 (anonymous namespace)::runForwardOpTreeImpl(polly::Scop&, llvm::LoopInfo&) ForwardOpTree.cpp:0:0
#16 0x000058ab555d6f09 polly::runForwardOpTree(polly::Scop&) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x59a1f09)
#17 0x000058ab55586779 polly::runPollyPass(llvm::Function&, llvm::AnalysisManager<llvm::Function>&, polly::PollyPassOptions) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x5951779)
#18 0x000058ab55586dff polly::PollyFunctionPass::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x5951dff)
#19 0x000058ab554fbb15 llvm::detail::PassModel<llvm::Function, polly::PollyFunctionPass, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x58c6b15)
#20 0x000058ab53ba2f55 llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x3f6df55)
#21 0x000058ab510e8985 llvm::detail::PassModel<llvm::Function, llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x14b3985)
#22 0x000058ab53ba16a1 llvm::ModuleToFunctionPassAdaptor::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x3f6c6a1)
#23 0x000058ab510e9025 llvm::detail::PassModel<llvm::Module, llvm::ModuleToFunctionPassAdaptor, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x14b4025)
#24 0x000058ab53ba1e8d llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x3f6ce8d)
#25 0x000058ab546de84e (anonymous namespace)::EmitAssemblyHelper::RunOptimizationPipeline(clang::BackendAction, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>&, std::unique_ptr<llvm::ToolOutputFile, std::default_delete<llvm::ToolOutputFile>>&, clang::BackendConsumer*) BackendUtil.cpp:0:0
#26 0x000058ab546e2607 clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x4aad607)
#27 0x000058ab54d87890 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x5152890)
#28 0x000058ab56b2f53c clang::ParseAST(clang::Sema&, bool, bool) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x6efa53c)
#29 0x000058ab550c52f7 clang::FrontendAction::Execute() (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x54902f7)
#30 0x000058ab5504841d clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x541341d)
#31 0x000058ab551c29de clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x558d9de)
#32 0x000058ab50c3768f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x100268f)
#33 0x000058ab50c2cb47 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#34 0x000058ab50c3189f clang_main(int, char**, llvm::ToolContext const&) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0xffc89f)
#35 0x000058ab50ab4390 main (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0xe7f390)
#36 0x00007120a262a1ca (/lib/x86_64-linux-gnu/libc.so.6+0x2a1ca)
#37 0x00007120a262a28b __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x2a28b)
#38 0x000058ab50c2c0b5 _start (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0xff70b5)
clang++: error: unable to execute command: Segmentation fault (core dumped)
clang++: error: clang frontend command failed with exit code -2 (use -v to see invocation)
clang version 23.0.0git (https://github.com/llvm/llvm-project.git 6e9a308a884d18315fa3cb31693718148dfa63dd)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /data/lenovo/compiler-build/llvm/llvm-project/build/bin
Build config: +assertions
clang++: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: /tmp/bug-5f6bd7.cpp
clang++: note: diagnostic msg: /tmp/bug-5f6bd7.sh
clang++: note: diagnostic msg: 

********************
```

Source code: 
```cpp
#include <algorithm>
char a;
short b;
short c;
short d;
extern short e[][9][9];
extern bool f[];
void g(long h, short i, int u, int j, char k, short l, bool m[], short n[][9],
       short o[][9]) {
  [[assume(h - 14211339058593952328 == 19)]];
  [[assume(l - 4413 == 1)]];
  for (int p; p < h - 14211339058593952328; p += l - 4413ULL) {
    [[assume(m[p])]];
    for (bool q = 0; q < m[p]; q = 1) {
      a = n[p][p];
      [[assume(u + 1948106743 == 19)]];
      for (int r = 0; r < u + 1948106743; r += 2)
        for (char s = 0; s < 4; s += 2) {
          [[assume(bool((bool)i - 1) == 0)]];
          [[assume(k == 1)]];
          for (bool t((bool)i - 1); t < k; t = std::max((short)7, (short)2))
            e[p][p][r] = 0;
          b = h;
          c = 4;
        }
      f[p] = 0;
      d = 402025379 ? j ?: o[p][q] : 0;
    }
  }
}
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-polly

Author: None (thapgua)

<details>
Clang++ crashes with `-mllvm -polly -O3` while compiling the given source code. The crash results in a Segmentation fault within `isl_basic_map_remove_duplicate_constraints` during the "polly" pass.

Compiler version: 
```
clang version 23.0.0git (https://github.com/llvm/llvm-project.git 6e9a308a884d18315fa3cb31693718148dfa63dd)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /data/lenovo/compiler-build/llvm/llvm-project/build/bin
Build config: +assertions
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/13
Selected GCC installation: /usr/lib/gcc/x86_64-linux-gnu/13
Candidate multilib: .;@<!-- -->m64
Selected multilib: .;@<!-- -->m64
```

Attached files: (The preprocessed .cpp file is relatively large due to the inclusion of the header file `&lt;algorithm&gt;`.)

[bug-5f6bd7.cpp](https://github.com/user-attachments/files/25467081/bug-5f6bd7.cpp)

[bug-5f6bd7.sh](https://github.com/user-attachments/files/25467082/bug-5f6bd7.sh)

Backtrace: 
```
bug.cpp:10:16: warning: integer literal is too large to be represented in a signed integer type, interpreting as unsigned
      [-Wimplicitly-unsigned-literal]
   10 |   [[assume(h - 14211339058593952328 == 19)]];
      |                ^
bug.cpp:12:23: warning: integer literal is too large to be represented in a signed integer type, interpreting as unsigned
      [-Wimplicitly-unsigned-literal]
   12 |   for (int p; p &lt; h - 14211339058593952328; p += l - 4413ULL) {
      |                       ^
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23 -cc1 -triple x86_64-unknown-linux-gnu -O3 -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -main-file-name bug.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=none -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/home/lenovo/bug-report/26-2-22 -fcoverage-compilation-dir=/home/lenovo/bug-report/26-2-22 -resource-dir /data/lenovo/compiler-build/llvm/llvm-project/build/lib/clang/23 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/x86_64-linux-gnu/c++/13 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/backward -internal-isystem /data/lenovo/compiler-build/llvm/llvm-project/build/lib/clang/23/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -fdeprecated-macro -ferror-limit 19 -fmessage-length=135 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -vectorize-loops -vectorize-slp -mllvm -polly -faddrsig -fdwarf2-cfi-asm -o /tmp/bug-4b456e.o -x c++ bug.cpp
1.      &lt;eof&gt; parser at end of file
2.      Optimizer
3.      Running pass "function&lt;eager-inv&gt;(drop-unnecessary-assumes,float2int,lower-constant-intrinsics,chr,polly,sroa&lt;modify-cfg&gt;,early-cse&lt;memssa&gt;,speculative-execution&lt;only-if-divergent-target&gt;,jump-threading,correlated-propagation,simplifycfg&lt;bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables&gt;,instcombine&lt;max-iterations=1;no-verify-fixpoint&gt;,aggressive-instcombine,libcalls-shrinkwrap,tailcallelim,simplifycfg&lt;bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables&gt;,reassociate,constraint-elimination,loop-mssa(loop-instsimplify,loop-simplifycfg,licm&lt;no-allowspeculation&gt;,loop-rotate&lt;header-duplication;no-prepare-for-lto&gt;,licm&lt;allowspeculation&gt;,simple-loop-unswitch&lt;nontrivial;trivial&gt;),simplifycfg&lt;bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables&gt;,instcombine&lt;max-iterations=1;no-verify-fixpoint&gt;,loop(loop-idiom,indvars,extra-simple-loop-unswitch-passes,loop-deletion,loop-unroll-full),sroa&lt;modify-cfg&gt;,vector-combine,mldst-motion&lt;no-split-footer-bb&gt;,gvn&lt;&gt;,sccp,bdce,instcombine&lt;max-iterations=1;no-verify-fixpoint&gt;,jump-threading,correlated-propagation,adce,memcpyopt,dse,move-auto-init,loop-mssa(licm&lt;allowspeculation&gt;),coro-elide,simplifycfg&lt;bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;no-switch-to-lookup;keep-loops;hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables&gt;,instcombine&lt;max-iterations=1;no-verify-fixpoint&gt;,loop(loop-rotate&lt;header-duplication;no-prepare-for-lto&gt;,loop-deletion),loop-distribute,inject-tli-mappings,loop-vectorize&lt;no-interleave-forced-only;no-vectorize-forced-only;&gt;,drop-unnecessary-assumes,infer-alignment,loop-load-elim,instcombine&lt;max-iterations=1;no-verify-fixpoint&gt;,simplifycfg&lt;bonus-inst-threshold=1;forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;switch-to-lookup;no-keep-loops;hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables&gt;,slp-vectorizer,vector-combine,instcombine&lt;max-iterations=1;no-verify-fixpoint&gt;,loop-unroll&lt;O3&gt;,transform-warning,sroa&lt;preserve-cfg&gt;,infer-alignment,instcombine&lt;max-iterations=1;no-verify-fixpoint&gt;,loop-mssa(licm&lt;allowspeculation&gt;),alignment-from-assumptions,loop-sink,instsimplify,div-rem-pairs,tailcallelim,simplifycfg&lt;bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;speculate-unpredictables&gt;)" on module "bug.cpp"
4.      Running pass "polly" on function "_Z1glsiicsPbPA9_sS1_"
 #<!-- -->0 0x000058ab542af952 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x467a952)
 #<!-- -->1 0x000058ab542ac5af llvm::sys::RunSignalHandlers() (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x46775af)
 #<!-- -->2 0x000058ab542ac6fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x000058ab542ac7bc SignalHandlerTerminate(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->4 0x00007120a2645330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->5 0x000058ab563932e9 isl_basic_map_remove_duplicate_constraints (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x675e2e9)
 #<!-- -->6 0x000058ab563989b3 isl_basic_map_simplify (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x67639b3)
 #<!-- -->7 0x000058ab563a181b basic_map_collect_diff.part.0 isl_map_subtract.c:0:0
 #<!-- -->8 0x000058ab563a2b39 isl_map_subtract (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x676db39)
 #<!-- -->9 0x000058ab5645a7f7 gen_bin_entry isl_union_map.c:0:0
#<!-- -->10 0x000058ab563347ac isl_hash_table_foreach (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x66ff7ac)
#<!-- -->11 0x000058ab56458458 gen_bin_op isl_union_map.c:0:0
#<!-- -->12 0x000058ab5645bfb0 isl_union_set_subtract (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x6826fb0)
#<!-- -->13 0x000058ab555e6b1b polly::ZoneAlgorithm::computeKnownFromLoad() const (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x59b1b1b)
#<!-- -->14 0x000058ab555e73db polly::ZoneAlgorithm::computeKnown(bool, bool) const (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x59b23db)
#<!-- -->15 0x000058ab555d6491 (anonymous namespace)::runForwardOpTreeImpl(polly::Scop&amp;, llvm::LoopInfo&amp;) ForwardOpTree.cpp:0:0
#<!-- -->16 0x000058ab555d6f09 polly::runForwardOpTree(polly::Scop&amp;) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x59a1f09)
#<!-- -->17 0x000058ab55586779 polly::runPollyPass(llvm::Function&amp;, llvm::AnalysisManager&lt;llvm::Function&gt;&amp;, polly::PollyPassOptions) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x5951779)
#<!-- -->18 0x000058ab55586dff polly::PollyFunctionPass::run(llvm::Function&amp;, llvm::AnalysisManager&lt;llvm::Function&gt;&amp;) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x5951dff)
#<!-- -->19 0x000058ab554fbb15 llvm::detail::PassModel&lt;llvm::Function, polly::PollyFunctionPass, llvm::AnalysisManager&lt;llvm::Function&gt;&gt;::run(llvm::Function&amp;, llvm::AnalysisManager&lt;llvm::Function&gt;&amp;) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x58c6b15)
#<!-- -->20 0x000058ab53ba2f55 llvm::PassManager&lt;llvm::Function, llvm::AnalysisManager&lt;llvm::Function&gt;&gt;::run(llvm::Function&amp;, llvm::AnalysisManager&lt;llvm::Function&gt;&amp;) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x3f6df55)
#<!-- -->21 0x000058ab510e8985 llvm::detail::PassModel&lt;llvm::Function, llvm::PassManager&lt;llvm::Function, llvm::AnalysisManager&lt;llvm::Function&gt;&gt;, llvm::AnalysisManager&lt;llvm::Function&gt;&gt;::run(llvm::Function&amp;, llvm::AnalysisManager&lt;llvm::Function&gt;&amp;) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x14b3985)
#<!-- -->22 0x000058ab53ba16a1 llvm::ModuleToFunctionPassAdaptor::run(llvm::Module&amp;, llvm::AnalysisManager&lt;llvm::Module&gt;&amp;) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x3f6c6a1)
#<!-- -->23 0x000058ab510e9025 llvm::detail::PassModel&lt;llvm::Module, llvm::ModuleToFunctionPassAdaptor, llvm::AnalysisManager&lt;llvm::Module&gt;&gt;::run(llvm::Module&amp;, llvm::AnalysisManager&lt;llvm::Module&gt;&amp;) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x14b4025)
#<!-- -->24 0x000058ab53ba1e8d llvm::PassManager&lt;llvm::Module, llvm::AnalysisManager&lt;llvm::Module&gt;&gt;::run(llvm::Module&amp;, llvm::AnalysisManager&lt;llvm::Module&gt;&amp;) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x3f6ce8d)
#<!-- -->25 0x000058ab546de84e (anonymous namespace)::EmitAssemblyHelper::RunOptimizationPipeline(clang::BackendAction, std::unique_ptr&lt;llvm::raw_pwrite_stream, std::default_delete&lt;llvm::raw_pwrite_stream&gt;&gt;&amp;, std::unique_ptr&lt;llvm::ToolOutputFile, std::default_delete&lt;llvm::ToolOutputFile&gt;&gt;&amp;, clang::BackendConsumer*) BackendUtil.cpp:0:0
#<!-- -->26 0x000058ab546e2607 clang::emitBackendOutput(clang::CompilerInstance&amp;, clang::CodeGenOptions&amp;, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;, std::unique_ptr&lt;llvm::raw_pwrite_stream, std::default_delete&lt;llvm::raw_pwrite_stream&gt;&gt;, clang::BackendConsumer*) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x4aad607)
#<!-- -->27 0x000058ab54d87890 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&amp;) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x5152890)
#<!-- -->28 0x000058ab56b2f53c clang::ParseAST(clang::Sema&amp;, bool, bool) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x6efa53c)
#<!-- -->29 0x000058ab550c52f7 clang::FrontendAction::Execute() (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x54902f7)
#<!-- -->30 0x000058ab5504841d clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x541341d)
#<!-- -->31 0x000058ab551c29de clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x558d9de)
#<!-- -->32 0x000058ab50c3768f cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x100268f)
#<!-- -->33 0x000058ab50c2cb47 ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->34 0x000058ab50c3189f clang_main(int, char**, llvm::ToolContext const&amp;) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0xffc89f)
#<!-- -->35 0x000058ab50ab4390 main (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0xe7f390)
#<!-- -->36 0x00007120a262a1ca (/lib/x86_64-linux-gnu/libc.so.6+0x2a1ca)
#<!-- -->37 0x00007120a262a28b __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x2a28b)
#<!-- -->38 0x000058ab50c2c0b5 _start (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0xff70b5)
clang++: error: unable to execute command: Segmentation fault (core dumped)
clang++: error: clang frontend command failed with exit code -2 (use -v to see invocation)
clang version 23.0.0git (https://github.com/llvm/llvm-project.git 6e9a308a884d18315fa3cb31693718148dfa63dd)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /data/lenovo/compiler-build/llvm/llvm-project/build/bin
Build config: +assertions
clang++: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: /tmp/bug-5f6bd7.cpp
clang++: note: diagnostic msg: /tmp/bug-5f6bd7.sh
clang++: note: diagnostic msg: 

********************
```

Source code: 
```cpp
#include &lt;algorithm&gt;
char a;
short b;
short c;
short d;
extern short e[][9][9];
extern bool f[];
void g(long h, short i, int u, int j, char k, short l, bool m[], short n[][9],
       short o[][9]) {
  [[assume(h - 14211339058593952328 == 19)]];
  [[assume(l - 4413 == 1)]];
  for (int p; p &lt; h - 14211339058593952328; p += l - 4413ULL) {
    [[assume(m[p])]];
    for (bool q = 0; q &lt; m[p]; q = 1) {
      a = n[p][p];
      [[assume(u + 1948106743 == 19)]];
      for (int r = 0; r &lt; u + 1948106743; r += 2)
        for (char s = 0; s &lt; 4; s += 2) {
          [[assume(bool((bool)i - 1) == 0)]];
          [[assume(k == 1)]];
          for (bool t((bool)i - 1); t &lt; k; t = std::max((short)7, (short)2))
            e[p][p][r] = 0;
          b = h;
          c = 4;
        }
      f[p] = 0;
      d = 402025379 ? j ?: o[p][q] : 0;
    }
  }
}
```
</details>


---

### Comment 2 - Meinersbur

Reduced regression test: [pr182736.ll.txt](https://github.com/user-attachments/files/25472659/pr182736.ll.txt)

---

### Comment 3 - Meinersbur

Thanks for the report. The segfault disappears with #182786. I am not sure whether this is because it also fixes this bug, or just makes this particular reproducer not cause it anymore. If you find a new reproducer, feel free to re-open this #.

---

