# [Polly] Crash in isl_ast_build_expr: "cannot handle void expression" with tentative array definition

**Author:** thapgua
**URL:** https://github.com/llvm/llvm-project/issues/180000
**Status:** Closed
**Labels:** polly, crash
**Closed Date:** 2026-02-07T23:30:35Z

## Body

Clang crashes with an assertion failure in isl when compiling C code containing a tentative array definition of _Bool type with `-O3 -mllvm -polly`. 

Version: 
```
clang version 23.0.0git (https://github.com/llvm/llvm-project.git d737229efdb3f3949f9c483d256eea2b946808a9)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /data/lenovo/compiler-build/llvm/llvm-project/build/bin
Build config: +assertions
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/13
Selected GCC installation: /usr/lib/gcc/x86_64-linux-gnu/13
Candidate multilib: .;@m64
Selected multilib: .;@m64
```

Attached files: 

[bug-6530a8.sh](https://github.com/user-attachments/files/25108986/bug-6530a8.sh)

[bug-6530a8.c](https://github.com/user-attachments/files/25108989/bug-6530a8.c)

Backtrace: 
```
bug.c:2:7: warning: tentative array definition assumed to have one element
      [-Wtentative-definition-array]
    2 | _Bool b[][3];
      |       ^
/data/lenovo/compiler-build/llvm/llvm-project/polly/lib/External/isl/isl_ast_build_expr.c:2211: cannot handle void expression
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23 -cc1 -triple x86_64-unknown-linux-gnu -O3 -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -main-file-name bug.c -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=none -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/home/lenovo/bug-report/llvm-crash-2 -fcoverage-compilation-dir=/home/lenovo/bug-report/llvm-crash-2 -resource-dir /data/lenovo/compiler-build/llvm/llvm-project/build/lib/clang/23 -internal-isystem /data/lenovo/compiler-build/llvm/llvm-project/build/lib/clang/23/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -ferror-limit 19 -fmessage-length=95 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -vectorize-loops -vectorize-slp -mllvm -polly -faddrsig -fdwarf2-cfi-asm -o /tmp/bug-e858aa.o -x c bug.c
1.      <eof> parser at end of file
2.      Optimizer
3.      Running pass "function<eager-inv>(drop-unnecessary-assumes,float2int,lower-constant-intrinsics,chr,polly,sroa<modify-cfg>,early-cse<memssa>,speculative-execution<only-if-divergent-target>,jump-threading,correlated-propagation,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,instcombine<max-iterations=1;no-verify-fixpoint>,aggressive-instcombine,libcalls-shrinkwrap,tailcallelim,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,reassociate,constraint-elimination,loop-mssa(loop-instsimplify,loop-simplifycfg,licm<no-allowspeculation>,loop-rotate<header-duplication;no-prepare-for-lto>,licm<allowspeculation>,simple-loop-unswitch<nontrivial;trivial>),simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,instcombine<max-iterations=1;no-verify-fixpoint>,loop(loop-idiom,indvars,extra-simple-loop-unswitch-passes,loop-deletion,loop-unroll-full),sroa<modify-cfg>,vector-combine,mldst-motion<no-split-footer-bb>,gvn<>,sccp,bdce,instcombine<max-iterations=1;no-verify-fixpoint>,jump-threading,correlated-propagation,adce,memcpyopt,dse,move-auto-init,loop-mssa(licm<allowspeculation>),coro-elide,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;no-switch-to-lookup;keep-loops;hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,instcombine<max-iterations=1;no-verify-fixpoint>,loop(loop-rotate<header-duplication;no-prepare-for-lto>,loop-deletion),loop-distribute,inject-tli-mappings,loop-vectorize<no-interleave-forced-only;no-vectorize-forced-only;>,drop-unnecessary-assumes,infer-alignment,loop-load-elim,instcombine<max-iterations=1;no-verify-fixpoint>,simplifycfg<bonus-inst-threshold=1;forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;switch-to-lookup;no-keep-loops;hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,slp-vectorizer,vector-combine,instcombine<max-iterations=1;no-verify-fixpoint>,loop-unroll<O3>,transform-warning,sroa<preserve-cfg>,infer-alignment,instcombine<max-iterations=1;no-verify-fixpoint>,loop-mssa(licm<allowspeculation>),alignment-from-assumptions,loop-sink,instsimplify,div-rem-pairs,tailcallelim,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;speculate-unpredictables>)" on module "bug.c"
4.      Running pass "polly" on function "c"
 #0 0x00005ec6b216d7d2 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x45d87d2)
 #1 0x00005ec6b216a4ff llvm::sys::RunSignalHandlers() (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x45d54ff)
 #2 0x00005ec6b216a64c SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x00007dcfb1245330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x00007dcfb129eb2c pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x9eb2c)
 #5 0x00007dcfb124527e raise (/lib/x86_64-linux-gnu/libc.so.6+0x4527e)
 #6 0x00007dcfb12288ff abort (/lib/x86_64-linux-gnu/libc.so.6+0x288ff)
 #7 0x00005ec6b41acba1 (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x6617ba1)
 #8 0x00005ec6b41ad4bc (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x66184bc)
 #9 0x00005ec6b417803d isl_ast_build_expr_from_pw_aff_internal (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x65e303d)
#10 0x00005ec6b4178278 isl_ast_build_with_arguments isl_ast_build_expr.c:0:0
#11 0x00005ec6b417868e isl_ast_build_call_from_executed (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x65e368e)
#12 0x00005ec6b4190a43 isl_ast_graft_alloc_domain (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x65fba43)
#13 0x00005ec6b4186e22 add_domain isl_ast_codegen.c:0:0
#14 0x00005ec6b41b849c isl_hash_table_foreach (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x662349c)
#15 0x00005ec6b42decd1 isl_union_map_foreach_map (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x6749cd1)
#16 0x00005ec6b418abe9 generate_inner_level isl_ast_codegen.c:0:0
#17 0x00005ec6b418a23b build_ast_from_schedule_node isl_ast_codegen.c:0:0
#18 0x00005ec6b418a01d build_ast_from_schedule_node isl_ast_codegen.c:0:0
#19 0x00005ec6b418ac4e generate_inner_level isl_ast_codegen.c:0:0
#20 0x00005ec6b418adc4 create_node_scaled isl_ast_codegen.c:0:0
#21 0x00005ec6b418c138 add_node isl_ast_codegen.c:0:0
#22 0x00005ec6b418c92e add_nodes isl_ast_codegen.c:0:0
#23 0x00005ec6b429b3e3 isl_basic_set_list_foreach_scc (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x67063e3)
#24 0x00005ec6b418c38a generate_sorted_domains isl_ast_codegen.c:0:0
#25 0x00005ec6b418c52a generate_sorted_domains_wrap isl_ast_codegen.c:0:0
#26 0x00005ec6b429b51c isl_basic_set_list_foreach_scc (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x670651c)
#27 0x00005ec6b418c486 generate_parallel_domains isl_ast_codegen.c:0:0
#28 0x00005ec6b41880ed generate_shifted_component_tree_base isl_ast_codegen.c:0:0
#29 0x00005ec6b418975d generate_next_level isl_ast_codegen.c:0:0
#30 0x00005ec6b418a3bf build_ast_from_schedule_node isl_ast_codegen.c:0:0
#31 0x00005ec6b418a01d build_ast_from_schedule_node isl_ast_codegen.c:0:0
#32 0x00005ec6b418cb5c isl_ast_build_node_from_schedule (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x65f7b5c)
#33 0x00005ec6b3350092 polly::IslAst::init(polly::Dependences const&) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x57bb092)
#34 0x00005ec6b33505a7 polly::runIslAstGen(polly::Scop&, polly::DependenceAnalysis::Result&) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x57bb5a7)
#35 0x00005ec6b32c6943 polly::runPollyPass(llvm::Function&, llvm::AnalysisManager<llvm::Function>&, polly::PollyPassOptions) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x5731943)
#36 0x00005ec6b32c78af polly::PollyFunctionPass::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x57328af)
#37 0x00005ec6b323b835 llvm::detail::PassModel<llvm::Function, polly::PollyFunctionPass, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x56a6835)
#38 0x00005ec6b1a61d35 llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x3eccd35)
#39 0x00005ec6af032045 llvm::detail::PassModel<llvm::Function, llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x149d045)
#40 0x00005ec6b1a60481 llvm::ModuleToFunctionPassAdaptor::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x3ecb481)
#41 0x00005ec6af0326e5 llvm::detail::PassModel<llvm::Module, llvm::ModuleToFunctionPassAdaptor, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x149d6e5)
#42 0x00005ec6b1a60c6d llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x3ecbc6d)
#43 0x00005ec6b242f02e (anonymous namespace)::EmitAssemblyHelper::RunOptimizationPipeline(clang::BackendAction, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>&, std::unique_ptr<llvm::ToolOutputFile, std::default_delete<llvm::ToolOutputFile>>&, clang::BackendConsumer*) BackendUtil.cpp:0:0
#44 0x00005ec6b2432d97 clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x489dd97)
#45 0x00005ec6b2acec48 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x4f39c48)
#46 0x00005ec6b49b87cc clang::ParseAST(clang::Sema&, bool, bool) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x6e237cc)
#47 0x00005ec6b2e073d7 clang::FrontendAction::Execute() (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x52723d7)
#48 0x00005ec6b2d8afbd clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x51f5fbd)
#49 0x00005ec6b2f0455e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x536f55e)
#50 0x00005ec6aeb82aef cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0xfedaef)
#51 0x00005ec6aeb78037 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#52 0x00005ec6aeb7cd6e clang_main(int, char**, llvm::ToolContext const&) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0xfe7d6e)
#53 0x00005ec6aea0332a main (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0xe6e32a)
#54 0x00007dcfb122a1ca (/lib/x86_64-linux-gnu/libc.so.6+0x2a1ca)
#55 0x00007dcfb122a28b __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x2a28b)
#56 0x00005ec6aeb775a5 _start (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0xfe25a5)
clang: error: unable to execute command: Aborted (core dumped)
clang: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 23.0.0git (https://github.com/llvm/llvm-project.git d737229efdb3f3949f9c483d256eea2b946808a9)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /data/lenovo/compiler-build/llvm/llvm-project/build/bin
Build config: +assertions
clang: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang: note: diagnostic msg: /tmp/bug-6530a8.c
clang: note: diagnostic msg: /tmp/bug-6530a8.sh
clang: note: diagnostic msg: 

********************
```

Failed code: 
```c
int a;
_Bool b[][3];
void c(_Bool d, char e, short g, int h, int i[][4][4], _Bool j,
       _Bool k[][0][20]) {
  for (short l = 0; l < 3; l += d) {
    for (short m = 0; m < g - 7; m += 3) {
      a = h ? ({
        __typeof__(-k[2][m][m]) f = -k[2][m][m];
        f;
      })
            : 0;
      b[l][m] = (e ? g : 0) ? j ? 2 : d : i[l][m][m];
      for (int n; n < 4; n += 050)
        if (!e)
          __builtin_unreachable();
    }
    for (short o; o; o += 3)
      ;
  }
}
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-polly

Author: None (thapgua)

<details>
Clang crashes with an assertion failure in isl when compiling C code containing a tentative array definition of _Bool type with `-O3 -mllvm -polly`. 

Version: 
```
clang version 23.0.0git (https://github.com/llvm/llvm-project.git d737229efdb3f3949f9c483d256eea2b946808a9)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /data/lenovo/compiler-build/llvm/llvm-project/build/bin
Build config: +assertions
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/13
Selected GCC installation: /usr/lib/gcc/x86_64-linux-gnu/13
Candidate multilib: .;@<!-- -->m64
Selected multilib: .;@<!-- -->m64
```

Attached files: 

[bug-6530a8.sh](https://github.com/user-attachments/files/25108986/bug-6530a8.sh)

[bug-6530a8.c](https://github.com/user-attachments/files/25108989/bug-6530a8.c)

Backtrace: 
```
bug.c:2:7: warning: tentative array definition assumed to have one element
      [-Wtentative-definition-array]
    2 | _Bool b[][3];
      |       ^
/data/lenovo/compiler-build/llvm/llvm-project/polly/lib/External/isl/isl_ast_build_expr.c:2211: cannot handle void expression
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23 -cc1 -triple x86_64-unknown-linux-gnu -O3 -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -main-file-name bug.c -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=none -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/home/lenovo/bug-report/llvm-crash-2 -fcoverage-compilation-dir=/home/lenovo/bug-report/llvm-crash-2 -resource-dir /data/lenovo/compiler-build/llvm/llvm-project/build/lib/clang/23 -internal-isystem /data/lenovo/compiler-build/llvm/llvm-project/build/lib/clang/23/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -ferror-limit 19 -fmessage-length=95 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -vectorize-loops -vectorize-slp -mllvm -polly -faddrsig -fdwarf2-cfi-asm -o /tmp/bug-e858aa.o -x c bug.c
1.      &lt;eof&gt; parser at end of file
2.      Optimizer
3.      Running pass "function&lt;eager-inv&gt;(drop-unnecessary-assumes,float2int,lower-constant-intrinsics,chr,polly,sroa&lt;modify-cfg&gt;,early-cse&lt;memssa&gt;,speculative-execution&lt;only-if-divergent-target&gt;,jump-threading,correlated-propagation,simplifycfg&lt;bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables&gt;,instcombine&lt;max-iterations=1;no-verify-fixpoint&gt;,aggressive-instcombine,libcalls-shrinkwrap,tailcallelim,simplifycfg&lt;bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables&gt;,reassociate,constraint-elimination,loop-mssa(loop-instsimplify,loop-simplifycfg,licm&lt;no-allowspeculation&gt;,loop-rotate&lt;header-duplication;no-prepare-for-lto&gt;,licm&lt;allowspeculation&gt;,simple-loop-unswitch&lt;nontrivial;trivial&gt;),simplifycfg&lt;bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables&gt;,instcombine&lt;max-iterations=1;no-verify-fixpoint&gt;,loop(loop-idiom,indvars,extra-simple-loop-unswitch-passes,loop-deletion,loop-unroll-full),sroa&lt;modify-cfg&gt;,vector-combine,mldst-motion&lt;no-split-footer-bb&gt;,gvn&lt;&gt;,sccp,bdce,instcombine&lt;max-iterations=1;no-verify-fixpoint&gt;,jump-threading,correlated-propagation,adce,memcpyopt,dse,move-auto-init,loop-mssa(licm&lt;allowspeculation&gt;),coro-elide,simplifycfg&lt;bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;no-switch-to-lookup;keep-loops;hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables&gt;,instcombine&lt;max-iterations=1;no-verify-fixpoint&gt;,loop(loop-rotate&lt;header-duplication;no-prepare-for-lto&gt;,loop-deletion),loop-distribute,inject-tli-mappings,loop-vectorize&lt;no-interleave-forced-only;no-vectorize-forced-only;&gt;,drop-unnecessary-assumes,infer-alignment,loop-load-elim,instcombine&lt;max-iterations=1;no-verify-fixpoint&gt;,simplifycfg&lt;bonus-inst-threshold=1;forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;switch-to-lookup;no-keep-loops;hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables&gt;,slp-vectorizer,vector-combine,instcombine&lt;max-iterations=1;no-verify-fixpoint&gt;,loop-unroll&lt;O3&gt;,transform-warning,sroa&lt;preserve-cfg&gt;,infer-alignment,instcombine&lt;max-iterations=1;no-verify-fixpoint&gt;,loop-mssa(licm&lt;allowspeculation&gt;),alignment-from-assumptions,loop-sink,instsimplify,div-rem-pairs,tailcallelim,simplifycfg&lt;bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;speculate-unpredictables&gt;)" on module "bug.c"
4.      Running pass "polly" on function "c"
 #<!-- -->0 0x00005ec6b216d7d2 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x45d87d2)
 #<!-- -->1 0x00005ec6b216a4ff llvm::sys::RunSignalHandlers() (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x45d54ff)
 #<!-- -->2 0x00005ec6b216a64c SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x00007dcfb1245330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->4 0x00007dcfb129eb2c pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x9eb2c)
 #<!-- -->5 0x00007dcfb124527e raise (/lib/x86_64-linux-gnu/libc.so.6+0x4527e)
 #<!-- -->6 0x00007dcfb12288ff abort (/lib/x86_64-linux-gnu/libc.so.6+0x288ff)
 #<!-- -->7 0x00005ec6b41acba1 (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x6617ba1)
 #<!-- -->8 0x00005ec6b41ad4bc (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x66184bc)
 #<!-- -->9 0x00005ec6b417803d isl_ast_build_expr_from_pw_aff_internal (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x65e303d)
#<!-- -->10 0x00005ec6b4178278 isl_ast_build_with_arguments isl_ast_build_expr.c:0:0
#<!-- -->11 0x00005ec6b417868e isl_ast_build_call_from_executed (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x65e368e)
#<!-- -->12 0x00005ec6b4190a43 isl_ast_graft_alloc_domain (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x65fba43)
#<!-- -->13 0x00005ec6b4186e22 add_domain isl_ast_codegen.c:0:0
#<!-- -->14 0x00005ec6b41b849c isl_hash_table_foreach (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x662349c)
#<!-- -->15 0x00005ec6b42decd1 isl_union_map_foreach_map (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x6749cd1)
#<!-- -->16 0x00005ec6b418abe9 generate_inner_level isl_ast_codegen.c:0:0
#<!-- -->17 0x00005ec6b418a23b build_ast_from_schedule_node isl_ast_codegen.c:0:0
#<!-- -->18 0x00005ec6b418a01d build_ast_from_schedule_node isl_ast_codegen.c:0:0
#<!-- -->19 0x00005ec6b418ac4e generate_inner_level isl_ast_codegen.c:0:0
#<!-- -->20 0x00005ec6b418adc4 create_node_scaled isl_ast_codegen.c:0:0
#<!-- -->21 0x00005ec6b418c138 add_node isl_ast_codegen.c:0:0
#<!-- -->22 0x00005ec6b418c92e add_nodes isl_ast_codegen.c:0:0
#<!-- -->23 0x00005ec6b429b3e3 isl_basic_set_list_foreach_scc (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x67063e3)
#<!-- -->24 0x00005ec6b418c38a generate_sorted_domains isl_ast_codegen.c:0:0
#<!-- -->25 0x00005ec6b418c52a generate_sorted_domains_wrap isl_ast_codegen.c:0:0
#<!-- -->26 0x00005ec6b429b51c isl_basic_set_list_foreach_scc (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x670651c)
#<!-- -->27 0x00005ec6b418c486 generate_parallel_domains isl_ast_codegen.c:0:0
#<!-- -->28 0x00005ec6b41880ed generate_shifted_component_tree_base isl_ast_codegen.c:0:0
#<!-- -->29 0x00005ec6b418975d generate_next_level isl_ast_codegen.c:0:0
#<!-- -->30 0x00005ec6b418a3bf build_ast_from_schedule_node isl_ast_codegen.c:0:0
#<!-- -->31 0x00005ec6b418a01d build_ast_from_schedule_node isl_ast_codegen.c:0:0
#<!-- -->32 0x00005ec6b418cb5c isl_ast_build_node_from_schedule (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x65f7b5c)
#<!-- -->33 0x00005ec6b3350092 polly::IslAst::init(polly::Dependences const&amp;) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x57bb092)
#<!-- -->34 0x00005ec6b33505a7 polly::runIslAstGen(polly::Scop&amp;, polly::DependenceAnalysis::Result&amp;) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x57bb5a7)
#<!-- -->35 0x00005ec6b32c6943 polly::runPollyPass(llvm::Function&amp;, llvm::AnalysisManager&lt;llvm::Function&gt;&amp;, polly::PollyPassOptions) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x5731943)
#<!-- -->36 0x00005ec6b32c78af polly::PollyFunctionPass::run(llvm::Function&amp;, llvm::AnalysisManager&lt;llvm::Function&gt;&amp;) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x57328af)
#<!-- -->37 0x00005ec6b323b835 llvm::detail::PassModel&lt;llvm::Function, polly::PollyFunctionPass, llvm::AnalysisManager&lt;llvm::Function&gt;&gt;::run(llvm::Function&amp;, llvm::AnalysisManager&lt;llvm::Function&gt;&amp;) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x56a6835)
#<!-- -->38 0x00005ec6b1a61d35 llvm::PassManager&lt;llvm::Function, llvm::AnalysisManager&lt;llvm::Function&gt;&gt;::run(llvm::Function&amp;, llvm::AnalysisManager&lt;llvm::Function&gt;&amp;) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x3eccd35)
#<!-- -->39 0x00005ec6af032045 llvm::detail::PassModel&lt;llvm::Function, llvm::PassManager&lt;llvm::Function, llvm::AnalysisManager&lt;llvm::Function&gt;&gt;, llvm::AnalysisManager&lt;llvm::Function&gt;&gt;::run(llvm::Function&amp;, llvm::AnalysisManager&lt;llvm::Function&gt;&amp;) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x149d045)
#<!-- -->40 0x00005ec6b1a60481 llvm::ModuleToFunctionPassAdaptor::run(llvm::Module&amp;, llvm::AnalysisManager&lt;llvm::Module&gt;&amp;) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x3ecb481)
#<!-- -->41 0x00005ec6af0326e5 llvm::detail::PassModel&lt;llvm::Module, llvm::ModuleToFunctionPassAdaptor, llvm::AnalysisManager&lt;llvm::Module&gt;&gt;::run(llvm::Module&amp;, llvm::AnalysisManager&lt;llvm::Module&gt;&amp;) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x149d6e5)
#<!-- -->42 0x00005ec6b1a60c6d llvm::PassManager&lt;llvm::Module, llvm::AnalysisManager&lt;llvm::Module&gt;&gt;::run(llvm::Module&amp;, llvm::AnalysisManager&lt;llvm::Module&gt;&amp;) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x3ecbc6d)
#<!-- -->43 0x00005ec6b242f02e (anonymous namespace)::EmitAssemblyHelper::RunOptimizationPipeline(clang::BackendAction, std::unique_ptr&lt;llvm::raw_pwrite_stream, std::default_delete&lt;llvm::raw_pwrite_stream&gt;&gt;&amp;, std::unique_ptr&lt;llvm::ToolOutputFile, std::default_delete&lt;llvm::ToolOutputFile&gt;&gt;&amp;, clang::BackendConsumer*) BackendUtil.cpp:0:0
#<!-- -->44 0x00005ec6b2432d97 clang::emitBackendOutput(clang::CompilerInstance&amp;, clang::CodeGenOptions&amp;, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;, std::unique_ptr&lt;llvm::raw_pwrite_stream, std::default_delete&lt;llvm::raw_pwrite_stream&gt;&gt;, clang::BackendConsumer*) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x489dd97)
#<!-- -->45 0x00005ec6b2acec48 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&amp;) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x4f39c48)
#<!-- -->46 0x00005ec6b49b87cc clang::ParseAST(clang::Sema&amp;, bool, bool) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x6e237cc)
#<!-- -->47 0x00005ec6b2e073d7 clang::FrontendAction::Execute() (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x52723d7)
#<!-- -->48 0x00005ec6b2d8afbd clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x51f5fbd)
#<!-- -->49 0x00005ec6b2f0455e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x536f55e)
#<!-- -->50 0x00005ec6aeb82aef cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0xfedaef)
#<!-- -->51 0x00005ec6aeb78037 ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->52 0x00005ec6aeb7cd6e clang_main(int, char**, llvm::ToolContext const&amp;) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0xfe7d6e)
#<!-- -->53 0x00005ec6aea0332a main (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0xe6e32a)
#<!-- -->54 0x00007dcfb122a1ca (/lib/x86_64-linux-gnu/libc.so.6+0x2a1ca)
#<!-- -->55 0x00007dcfb122a28b __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x2a28b)
#<!-- -->56 0x00005ec6aeb775a5 _start (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0xfe25a5)
clang: error: unable to execute command: Aborted (core dumped)
clang: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 23.0.0git (https://github.com/llvm/llvm-project.git d737229efdb3f3949f9c483d256eea2b946808a9)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /data/lenovo/compiler-build/llvm/llvm-project/build/bin
Build config: +assertions
clang: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang: note: diagnostic msg: /tmp/bug-6530a8.c
clang: note: diagnostic msg: /tmp/bug-6530a8.sh
clang: note: diagnostic msg: 

********************
```

Failed code: 
```c
int a;
_Bool b[][3];
void c(_Bool d, char e, short g, int h, int i[][4][4], _Bool j,
       _Bool k[][0][20]) {
  for (short l = 0; l &lt; 3; l += d) {
    for (short m = 0; m &lt; g - 7; m += 3) {
      a = h ? ({
        __typeof__(-k[2][m][m]) f = -k[2][m][m];
        f;
      })
            : 0;
      b[l][m] = (e ? g : 0) ? j ? 2 : d : i[l][m][m];
      for (int n; n &lt; 4; n += 050)
        if (!e)
          __builtin_unreachable();
    }
    for (short o; o; o += 3)
      ;
  }
}
```
</details>


---

### Comment 2 - Meinersbur

Thanks for the report

Reduced: [pr180000.ll.txt](https://github.com/user-attachments/files/25135608/pr180000.ll.txt)

Upstream ISL report: https://groups.google.com/g/isl-development/c/mB2GYBoZjLg


---

