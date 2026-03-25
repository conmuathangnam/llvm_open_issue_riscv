# [Polly] Assertion '!Ast.is_null()' failed in IslAst.cpp with `-O3 -mllvm -polly` (Structurally similar to #180000)

**Author:** thapgua
**URL:** https://github.com/llvm/llvm-project/issues/180958
**Status:** Closed
**Labels:** polly, crash
**Closed Date:** 2026-03-01T22:07:00Z

## Body

Clang crashes with an assertion failure `!Ast.is_null()` when compiling with `-O3 -mllvm -polly`. 
The code structure resembles the test case in issue #180000.

Version: 
```
clang version 23.0.0git (https://github.com/llvm/llvm-project.git 6af11dba3cb158d46e598363ae1035f7935aa700)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /data/lenovo/compiler-build/llvm/llvm-project/build/bin
Build config: +assertions
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/13
Selected GCC installation: /usr/lib/gcc/x86_64-linux-gnu/13
Candidate multilib: .;@m64
Selected multilib: .;@m64

$ cat $LLVM_DIR/llvm-project/polly/lib/External/isl/GIT_HEAD_ID
isl-0.27-78-gfc484e00
```

Attachments:

[bug-5ef845.c](https://github.com/user-attachments/files/25238090/bug-5ef845.c)

[bug-5ef845.sh](https://github.com/user-attachments/files/25238092/bug-5ef845.sh)

Backtrace: 
```
bug.c:1:7: warning: tentative array definition assumed to have one element
      [-Wtentative-definition-array]
    1 | short a[][3][3];
      |       ^
clang-23: /data/lenovo/compiler-build/llvm/llvm-project/polly/lib/CodeGen/IslAst.cpp:463: void walkAstForStatistics(const isl::ast_node&): Assertion `!Ast.is_null()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23 -cc1 -triple x86_64-unknown-linux-gnu -O3 -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -main-file-name bug.c -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=none -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/home/lenovo/bug-report/26-2-11 -fcoverage-compilation-dir=/home/lenovo/bug-report/26-2-11 -resource-dir /data/lenovo/compiler-build/llvm/llvm-project/build/lib/clang/23 -internal-isystem /data/lenovo/compiler-build/llvm/llvm-project/build/lib/clang/23/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -ferror-limit 19 -fmessage-length=95 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -vectorize-loops -vectorize-slp -mllvm -polly -faddrsig -fdwarf2-cfi-asm -o /tmp/bug-599e9f.o -x c bug.c
1.      <eof> parser at end of file
2.      Optimizer
3.      Running pass "function<eager-inv>(drop-unnecessary-assumes,float2int,lower-constant-intrinsics,chr,polly,sroa<modify-cfg>,early-cse<memssa>,speculative-execution<only-if-divergent-target>,jump-threading,correlated-propagation,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,instcombine<max-iterations=1;no-verify-fixpoint>,aggressive-instcombine,libcalls-shrinkwrap,tailcallelim,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,reassociate,constraint-elimination,loop-mssa(loop-instsimplify,loop-simplifycfg,licm<no-allowspeculation>,loop-rotate<header-duplication;no-prepare-for-lto>,licm<allowspeculation>,simple-loop-unswitch<nontrivial;trivial>),simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,instcombine<max-iterations=1;no-verify-fixpoint>,loop(loop-idiom,indvars,extra-simple-loop-unswitch-passes,loop-deletion,loop-unroll-full),sroa<modify-cfg>,vector-combine,mldst-motion<no-split-footer-bb>,gvn<>,sccp,bdce,instcombine<max-iterations=1;no-verify-fixpoint>,jump-threading,correlated-propagation,adce,memcpyopt,dse,move-auto-init,loop-mssa(licm<allowspeculation>),coro-elide,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;no-switch-to-lookup;keep-loops;hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,instcombine<max-iterations=1;no-verify-fixpoint>,loop(loop-rotate<header-duplication;no-prepare-for-lto>,loop-deletion),loop-distribute,inject-tli-mappings,loop-vectorize<no-interleave-forced-only;no-vectorize-forced-only;>,drop-unnecessary-assumes,infer-alignment,loop-load-elim,instcombine<max-iterations=1;no-verify-fixpoint>,simplifycfg<bonus-inst-threshold=1;forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;switch-to-lookup;no-keep-loops;hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,slp-vectorizer,vector-combine,instcombine<max-iterations=1;no-verify-fixpoint>,loop-unroll<O3>,transform-warning,sroa<preserve-cfg>,infer-alignment,instcombine<max-iterations=1;no-verify-fixpoint>,loop-mssa(licm<allowspeculation>),alignment-from-assumptions,loop-sink,instsimplify,div-rem-pairs,tailcallelim,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;speculate-unpredictables>)" on module "bug.c"
4.      Running pass "polly" on function "b"
 #0 0x00005dd57e509492 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x4643492)
 #1 0x00005dd57e5061bf llvm::sys::RunSignalHandlers() (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x46401bf)
 #2 0x00005dd57e50630c SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x00007295ecc45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x00007295ecc9eb2c pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x9eb2c)
 #5 0x00007295ecc4527e raise (/lib/x86_64-linux-gnu/libc.so.6+0x4527e)
 #6 0x00007295ecc288ff abort (/lib/x86_64-linux-gnu/libc.so.6+0x288ff)
 #7 0x00007295ecc2881b (/lib/x86_64-linux-gnu/libc.so.6+0x2881b)
 #8 0x00007295ecc3b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
 #9 0x00005dd57f6f869d polly::IslAst::init(polly::Dependences const&) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x583269d)
#10 0x00005dd57f6f89b7 polly::runIslAstGen(polly::Scop&, polly::DependenceAnalysis::Result&) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x58329b7)
#11 0x00005dd57f66ed53 polly::runPollyPass(llvm::Function&, llvm::AnalysisManager<llvm::Function>&, polly::PollyPassOptions) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x57a8d53)
#12 0x00005dd57f66fcbf polly::PollyFunctionPass::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x57a9cbf)
#13 0x00005dd57f5e3c45 llvm::detail::PassModel<llvm::Function, polly::PollyFunctionPass, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x571dc45)
#14 0x00005dd57ddff055 llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x3f39055)
#15 0x00005dd57b36c385 llvm::detail::PassModel<llvm::Function, llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x14a6385)
#16 0x00005dd57ddfd7a1 llvm::ModuleToFunctionPassAdaptor::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x3f377a1)
#17 0x00005dd57b36ca25 llvm::detail::PassModel<llvm::Module, llvm::ModuleToFunctionPassAdaptor, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x14a6a25)
#18 0x00005dd57ddfdf8d llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x3f37f8d)
#19 0x00005dd57e7d10ce (anonymous namespace)::EmitAssemblyHelper::RunOptimizationPipeline(clang::BackendAction, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>&, std::unique_ptr<llvm::ToolOutputFile, std::default_delete<llvm::ToolOutputFile>>&, clang::BackendConsumer*) BackendUtil.cpp:0:0
#20 0x00005dd57e7d4e37 clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x490ee37)
#21 0x00005dd57ee74d38 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x4faed38)
#22 0x00005dd580d7017c clang::ParseAST(clang::Sema&, bool, bool) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x6eaa17c)
#23 0x00005dd57f1af487 clang::FrontendAction::Execute() (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x52e9487)
#24 0x00005dd57f13320d clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x526d20d)
#25 0x00005dd57f2ac51e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x53e651e)
#26 0x00005dd57aebbdcf cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0xff5dcf)
#27 0x00005dd57aeb1317 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#28 0x00005dd57aeb604e clang_main(int, char**, llvm::ToolContext const&) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0xff004e)
#29 0x00005dd57ad3b35a main (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0xe7535a)
#30 0x00007295ecc2a1ca (/lib/x86_64-linux-gnu/libc.so.6+0x2a1ca)
#31 0x00007295ecc2a28b __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x2a28b)
#32 0x00005dd57aeb0885 _start (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0xfea885)
clang: error: unable to execute command: Aborted (core dumped)
clang: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 23.0.0git (https://github.com/llvm/llvm-project.git 6af11dba3cb158d46e598363ae1035f7935aa700)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /data/lenovo/compiler-build/llvm/llvm-project/build/bin
Build config: +assertions
clang: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang: note: diagnostic msg: /tmp/bug-5ef845.c
clang: note: diagnostic msg: /tmp/bug-5ef845.sh
clang: note: diagnostic msg: 

********************
```

Code: 
```c
short a[][3][3];
void b(char c, int d, int f, int g, int h, int i[], int j, _Bool k[][3][3],
       short l[][3][3]) {
  for (int m;;)
    for (char n; j ?: k[n][n][n];) {
      for (unsigned o; o < c; o++)
        for (int p = 0; p < 23; p += 4)
          a[o][n][o] = o % 3 == h ? l[o][m][o] >> ({__typeof__(n) e = i[n];e;}) % p : (d ? f : 0) ? : l[o][m][o] ;
      __builtin_assume((_Bool)g + (_Bool)f - 1 == 1);
      for (_Bool q = 0; q < g; q = f)
        ;
    }
}
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-polly

Author: None (thapgua)

<details>
Clang crashes with an assertion failure `!Ast.is_null()` when compiling with `-O3 -mllvm -polly`. 
The code structure resembles the test case in issue #<!-- -->180000.

Version: 
```
clang version 23.0.0git (https://github.com/llvm/llvm-project.git 6af11dba3cb158d46e598363ae1035f7935aa700)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /data/lenovo/compiler-build/llvm/llvm-project/build/bin
Build config: +assertions
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/13
Selected GCC installation: /usr/lib/gcc/x86_64-linux-gnu/13
Candidate multilib: .;@<!-- -->m64
Selected multilib: .;@<!-- -->m64

$ cat $LLVM_DIR/llvm-project/polly/lib/External/isl/GIT_HEAD_ID
isl-0.27-78-gfc484e00
```

Attachments:

[bug-5ef845.c](https://github.com/user-attachments/files/25238090/bug-5ef845.c)

[bug-5ef845.sh](https://github.com/user-attachments/files/25238092/bug-5ef845.sh)

Backtrace: 
```
bug.c:1:7: warning: tentative array definition assumed to have one element
      [-Wtentative-definition-array]
    1 | short a[][3][3];
      |       ^
clang-23: /data/lenovo/compiler-build/llvm/llvm-project/polly/lib/CodeGen/IslAst.cpp:463: void walkAstForStatistics(const isl::ast_node&amp;): Assertion `!Ast.is_null()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23 -cc1 -triple x86_64-unknown-linux-gnu -O3 -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -main-file-name bug.c -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=none -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/home/lenovo/bug-report/26-2-11 -fcoverage-compilation-dir=/home/lenovo/bug-report/26-2-11 -resource-dir /data/lenovo/compiler-build/llvm/llvm-project/build/lib/clang/23 -internal-isystem /data/lenovo/compiler-build/llvm/llvm-project/build/lib/clang/23/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -ferror-limit 19 -fmessage-length=95 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -vectorize-loops -vectorize-slp -mllvm -polly -faddrsig -fdwarf2-cfi-asm -o /tmp/bug-599e9f.o -x c bug.c
1.      &lt;eof&gt; parser at end of file
2.      Optimizer
3.      Running pass "function&lt;eager-inv&gt;(drop-unnecessary-assumes,float2int,lower-constant-intrinsics,chr,polly,sroa&lt;modify-cfg&gt;,early-cse&lt;memssa&gt;,speculative-execution&lt;only-if-divergent-target&gt;,jump-threading,correlated-propagation,simplifycfg&lt;bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables&gt;,instcombine&lt;max-iterations=1;no-verify-fixpoint&gt;,aggressive-instcombine,libcalls-shrinkwrap,tailcallelim,simplifycfg&lt;bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables&gt;,reassociate,constraint-elimination,loop-mssa(loop-instsimplify,loop-simplifycfg,licm&lt;no-allowspeculation&gt;,loop-rotate&lt;header-duplication;no-prepare-for-lto&gt;,licm&lt;allowspeculation&gt;,simple-loop-unswitch&lt;nontrivial;trivial&gt;),simplifycfg&lt;bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables&gt;,instcombine&lt;max-iterations=1;no-verify-fixpoint&gt;,loop(loop-idiom,indvars,extra-simple-loop-unswitch-passes,loop-deletion,loop-unroll-full),sroa&lt;modify-cfg&gt;,vector-combine,mldst-motion&lt;no-split-footer-bb&gt;,gvn&lt;&gt;,sccp,bdce,instcombine&lt;max-iterations=1;no-verify-fixpoint&gt;,jump-threading,correlated-propagation,adce,memcpyopt,dse,move-auto-init,loop-mssa(licm&lt;allowspeculation&gt;),coro-elide,simplifycfg&lt;bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;no-switch-to-lookup;keep-loops;hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables&gt;,instcombine&lt;max-iterations=1;no-verify-fixpoint&gt;,loop(loop-rotate&lt;header-duplication;no-prepare-for-lto&gt;,loop-deletion),loop-distribute,inject-tli-mappings,loop-vectorize&lt;no-interleave-forced-only;no-vectorize-forced-only;&gt;,drop-unnecessary-assumes,infer-alignment,loop-load-elim,instcombine&lt;max-iterations=1;no-verify-fixpoint&gt;,simplifycfg&lt;bonus-inst-threshold=1;forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;switch-to-lookup;no-keep-loops;hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables&gt;,slp-vectorizer,vector-combine,instcombine&lt;max-iterations=1;no-verify-fixpoint&gt;,loop-unroll&lt;O3&gt;,transform-warning,sroa&lt;preserve-cfg&gt;,infer-alignment,instcombine&lt;max-iterations=1;no-verify-fixpoint&gt;,loop-mssa(licm&lt;allowspeculation&gt;),alignment-from-assumptions,loop-sink,instsimplify,div-rem-pairs,tailcallelim,simplifycfg&lt;bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;speculate-unpredictables&gt;)" on module "bug.c"
4.      Running pass "polly" on function "b"
 #<!-- -->0 0x00005dd57e509492 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x4643492)
 #<!-- -->1 0x00005dd57e5061bf llvm::sys::RunSignalHandlers() (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x46401bf)
 #<!-- -->2 0x00005dd57e50630c SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x00007295ecc45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->4 0x00007295ecc9eb2c pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x9eb2c)
 #<!-- -->5 0x00007295ecc4527e raise (/lib/x86_64-linux-gnu/libc.so.6+0x4527e)
 #<!-- -->6 0x00007295ecc288ff abort (/lib/x86_64-linux-gnu/libc.so.6+0x288ff)
 #<!-- -->7 0x00007295ecc2881b (/lib/x86_64-linux-gnu/libc.so.6+0x2881b)
 #<!-- -->8 0x00007295ecc3b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
 #<!-- -->9 0x00005dd57f6f869d polly::IslAst::init(polly::Dependences const&amp;) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x583269d)
#<!-- -->10 0x00005dd57f6f89b7 polly::runIslAstGen(polly::Scop&amp;, polly::DependenceAnalysis::Result&amp;) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x58329b7)
#<!-- -->11 0x00005dd57f66ed53 polly::runPollyPass(llvm::Function&amp;, llvm::AnalysisManager&lt;llvm::Function&gt;&amp;, polly::PollyPassOptions) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x57a8d53)
#<!-- -->12 0x00005dd57f66fcbf polly::PollyFunctionPass::run(llvm::Function&amp;, llvm::AnalysisManager&lt;llvm::Function&gt;&amp;) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x57a9cbf)
#<!-- -->13 0x00005dd57f5e3c45 llvm::detail::PassModel&lt;llvm::Function, polly::PollyFunctionPass, llvm::AnalysisManager&lt;llvm::Function&gt;&gt;::run(llvm::Function&amp;, llvm::AnalysisManager&lt;llvm::Function&gt;&amp;) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x571dc45)
#<!-- -->14 0x00005dd57ddff055 llvm::PassManager&lt;llvm::Function, llvm::AnalysisManager&lt;llvm::Function&gt;&gt;::run(llvm::Function&amp;, llvm::AnalysisManager&lt;llvm::Function&gt;&amp;) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x3f39055)
#<!-- -->15 0x00005dd57b36c385 llvm::detail::PassModel&lt;llvm::Function, llvm::PassManager&lt;llvm::Function, llvm::AnalysisManager&lt;llvm::Function&gt;&gt;, llvm::AnalysisManager&lt;llvm::Function&gt;&gt;::run(llvm::Function&amp;, llvm::AnalysisManager&lt;llvm::Function&gt;&amp;) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x14a6385)
#<!-- -->16 0x00005dd57ddfd7a1 llvm::ModuleToFunctionPassAdaptor::run(llvm::Module&amp;, llvm::AnalysisManager&lt;llvm::Module&gt;&amp;) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x3f377a1)
#<!-- -->17 0x00005dd57b36ca25 llvm::detail::PassModel&lt;llvm::Module, llvm::ModuleToFunctionPassAdaptor, llvm::AnalysisManager&lt;llvm::Module&gt;&gt;::run(llvm::Module&amp;, llvm::AnalysisManager&lt;llvm::Module&gt;&amp;) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x14a6a25)
#<!-- -->18 0x00005dd57ddfdf8d llvm::PassManager&lt;llvm::Module, llvm::AnalysisManager&lt;llvm::Module&gt;&gt;::run(llvm::Module&amp;, llvm::AnalysisManager&lt;llvm::Module&gt;&amp;) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x3f37f8d)
#<!-- -->19 0x00005dd57e7d10ce (anonymous namespace)::EmitAssemblyHelper::RunOptimizationPipeline(clang::BackendAction, std::unique_ptr&lt;llvm::raw_pwrite_stream, std::default_delete&lt;llvm::raw_pwrite_stream&gt;&gt;&amp;, std::unique_ptr&lt;llvm::ToolOutputFile, std::default_delete&lt;llvm::ToolOutputFile&gt;&gt;&amp;, clang::BackendConsumer*) BackendUtil.cpp:0:0
#<!-- -->20 0x00005dd57e7d4e37 clang::emitBackendOutput(clang::CompilerInstance&amp;, clang::CodeGenOptions&amp;, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;, std::unique_ptr&lt;llvm::raw_pwrite_stream, std::default_delete&lt;llvm::raw_pwrite_stream&gt;&gt;, clang::BackendConsumer*) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x490ee37)
#<!-- -->21 0x00005dd57ee74d38 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&amp;) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x4faed38)
#<!-- -->22 0x00005dd580d7017c clang::ParseAST(clang::Sema&amp;, bool, bool) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x6eaa17c)
#<!-- -->23 0x00005dd57f1af487 clang::FrontendAction::Execute() (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x52e9487)
#<!-- -->24 0x00005dd57f13320d clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x526d20d)
#<!-- -->25 0x00005dd57f2ac51e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x53e651e)
#<!-- -->26 0x00005dd57aebbdcf cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0xff5dcf)
#<!-- -->27 0x00005dd57aeb1317 ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->28 0x00005dd57aeb604e clang_main(int, char**, llvm::ToolContext const&amp;) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0xff004e)
#<!-- -->29 0x00005dd57ad3b35a main (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0xe7535a)
#<!-- -->30 0x00007295ecc2a1ca (/lib/x86_64-linux-gnu/libc.so.6+0x2a1ca)
#<!-- -->31 0x00007295ecc2a28b __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x2a28b)
#<!-- -->32 0x00005dd57aeb0885 _start (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0xfea885)
clang: error: unable to execute command: Aborted (core dumped)
clang: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 23.0.0git (https://github.com/llvm/llvm-project.git 6af11dba3cb158d46e598363ae1035f7935aa700)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /data/lenovo/compiler-build/llvm/llvm-project/build/bin
Build config: +assertions
clang: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang: note: diagnostic msg: /tmp/bug-5ef845.c
clang: note: diagnostic msg: /tmp/bug-5ef845.sh
clang: note: diagnostic msg: 

********************
```

Code: 
```c
short a[][3][3];
void b(char c, int d, int f, int g, int h, int i[], int j, _Bool k[][3][3],
       short l[][3][3]) {
  for (int m;;)
    for (char n; j ?: k[n][n][n];) {
      for (unsigned o; o &lt; c; o++)
        for (int p = 0; p &lt; 23; p += 4)
          a[o][n][o] = o % 3 == h ? l[o][m][o] &gt;&gt; ({__typeof__(n) e = i[n];e;}) % p : (d ? f : 0) ? : l[o][m][o] ;
      __builtin_assume((_Bool)g + (_Bool)f - 1 == 1);
      for (_Bool q = 0; q &lt; g; q = f)
        ;
    }
}
```
</details>


---

### Comment 2 - Meinersbur

reduced: [pr180958.ll.txt](https://github.com/user-attachments/files/25299950/pr180958.ll.txt)

---

### Comment 3 - Meinersbur

Upstream bug report: https://groups.google.com/g/isl-development/c/aaW8arSBW-E

---

