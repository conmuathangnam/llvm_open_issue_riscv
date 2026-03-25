# Clang crashes on valid code when compiling with profiled data using PGO

**Author:** helloqirun
**URL:** https://github.com/llvm/llvm-project/issues/172195
**Status:** Closed
**Labels:** vectorizers, crash
**Closed Date:** 2026-01-01T10:26:38Z

## Body

The crash happens on the recent commit 86c5539aa89ac61058e3ba4fc0

```
$ clang-trunk -v
clang version 22.0.0git (https://github.com/llvm/llvm-project.git 86c5539aa89ac61058e3ba4fc0ae578c2879bf9e)
```

I have reduced the testcase. The profiling data is produced by

```
$ llvm-profdata merge prof.txt -o prof.profdata
```


It compiles at -O3

```
$ clang-trunk -O3  -fprofile-instr-use=prof.profdata aa-10084.c
```

It crashes at -Os
```
$ clang-trunk -Os  -fprofile-instr-use=prof.profdata aa-10084.c
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /data/installed/root-clang/bin/clang-22 -cc1 -triple x86_64-unknown-linux-gnu -Os -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -main-file-name aa-10084.c -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=none -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/home/absozero/projects/open-csmith/bugs/test/reduce -fprofile-instrument-use=clang -fprofile-instrument-use-path=prof.profdata -fcoverage-compilation-dir=/home/absozero/projects/open-csmith/bugs/test/reduce -resource-dir /data/installed/root-clang/lib/clang/22 -I/data/installed/root-bin/include/ -internal-isystem /data/installed/root-clang/lib/clang/22/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/14/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -ferror-limit 19 -fmessage-length=205 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -vectorize-loops -vectorize-slp -faddrsig -fdwarf2-cfi-asm -o /tmp/aa-10084-a28184.o -x c aa-10084.c
1.	<eof> parser at end of file
2.	Optimizer
3.	Running pass "function<eager-inv>(drop-unnecessary-assumes,float2int,lower-constant-intrinsics,loop(loop-rotate<header-duplication;no-prepare-for-lto>,loop-deletion),loop-distribute,inject-tli-mappings,loop-vectorize<no-interleave-forced-only;no-vectorize-forced-only;>,drop-unnecessary-assumes,infer-alignment,loop-load-elim,instcombine<max-iterations=1;no-verify-fixpoint>,simplifycfg<bonus-inst-threshold=1;forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;switch-to-lookup;no-keep-loops;hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,slp-vectorizer,vector-combine,instcombine<max-iterations=1;no-verify-fixpoint>,loop-unroll<O2>,transform-warning,sroa<preserve-cfg>,infer-alignment,instcombine<max-iterations=1;no-verify-fixpoint>,loop-mssa(licm<allowspeculation>),alignment-from-assumptions,loop-sink,instsimplify,div-rem-pairs,tailcallelim,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;speculate-unpredictables>)" on module "aa-10084.c"
4.	Running pass "loop-vectorize<no-interleave-forced-only;no-vectorize-forced-only;>" on function "func_21"
 #0 0x00005baa2d44f522 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/data/installed/root-clang/bin/clang-22+0x4453522)
 #1 0x00005baa2d44c21f llvm::sys::RunSignalHandlers() (/data/installed/root-clang/bin/clang-22+0x445021f)
 #2 0x00005baa2d44c36c SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x00007b1748445330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x00005baa2ef07c7f llvm::LoopVectorizationCostModel::expectedCost(llvm::ElementCount) (.constprop.0) LoopVectorize.cpp:0:0
 #5 0x00005baa2ef16f36 llvm::LoopVectorizationPlanner::computeBestVF() (/data/installed/root-clang/bin/clang-22+0x5f1af36)
 #6 0x00005baa2ef1a071 llvm::LoopVectorizePass::processLoop(llvm::Loop*) (/data/installed/root-clang/bin/clang-22+0x5f1e071)
 #7 0x00005baa2ef1ca61 llvm::LoopVectorizePass::runImpl(llvm::Function&) (/data/installed/root-clang/bin/clang-22+0x5f20a61)
 #8 0x00005baa2ef1d0d0 llvm::LoopVectorizePass::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/data/installed/root-clang/bin/clang-22+0x5f210d0)
 #9 0x00005baa2e50c215 llvm::detail::PassModel<llvm::Function, llvm::LoopVectorizePass, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/data/installed/root-clang/bin/clang-22+0x5510215)
#10 0x00005baa2cd5d8e5 llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/data/installed/root-clang/bin/clang-22+0x3d618e5)
#11 0x00005baa2a3c1565 llvm::detail::PassModel<llvm::Function, llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/data/installed/root-clang/bin/clang-22+0x13c5565)
#12 0x00005baa2cd5c031 llvm::ModuleToFunctionPassAdaptor::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/data/installed/root-clang/bin/clang-22+0x3d60031)
#13 0x00005baa2a3c1ca5 llvm::detail::PassModel<llvm::Module, llvm::ModuleToFunctionPassAdaptor, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/data/installed/root-clang/bin/clang-22+0x13c5ca5)
#14 0x00005baa2cd5c81d llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/data/installed/root-clang/bin/clang-22+0x3d6081d)
#15 0x00005baa2d70e2f2 (anonymous namespace)::EmitAssemblyHelper::RunOptimizationPipeline(clang::BackendAction, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>&, std::unique_ptr<llvm::ToolOutputFile, std::default_delete<llvm::ToolOutputFile>>&, clang::BackendConsumer*) BackendUtil.cpp:0:0
#16 0x00005baa2d7116d1 (anonymous namespace)::EmitAssemblyHelper::emitAssembly(clang::BackendAction, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) BackendUtil.cpp:0:0
#17 0x00005baa2d7125bc clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/data/installed/root-clang/bin/clang-22+0x47165bc)
#18 0x00005baa2dda0870 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/data/installed/root-clang/bin/clang-22+0x4da4870)
#19 0x00005baa2f8b945c clang::ParseAST(clang::Sema&, bool, bool) (/data/installed/root-clang/bin/clang-22+0x68bd45c)
#20 0x00005baa2e0d4947 clang::FrontendAction::Execute() (/data/installed/root-clang/bin/clang-22+0x50d8947)
#21 0x00005baa2e05981e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/data/installed/root-clang/bin/clang-22+0x505d81e)
#22 0x00005baa2e1d057e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/data/installed/root-clang/bin/clang-22+0x51d457e)
#23 0x00005baa29f21d9b cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/data/installed/root-clang/bin/clang-22+0xf25d9b)
#24 0x00005baa29f17227 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#25 0x00005baa29f1bf5e clang_main(int, char**, llvm::ToolContext const&) (/data/installed/root-clang/bin/clang-22+0xf1ff5e)
#26 0x00005baa29db2cda main (/data/installed/root-clang/bin/clang-22+0xdb6cda)
#27 0x00007b174842a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#28 0x00007b174842a28b call_init ./csu/../csu/libc-start.c:128:20
#29 0x00007b174842a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#30 0x00005baa29f16795 _start (/data/installed/root-clang/bin/clang-22+0xf1a795)
clang-trunk: error: unable to execute command: Floating point exception
clang-trunk: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 22.0.0git (https://github.com/llvm/llvm-project.git 86c5539aa89ac61058e3ba4fc0ae578c2879bf9e)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /data/installed/root-clang/bin
Build config: +assertions
clang-trunk: note: diagnostic msg:
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang-trunk: note: diagnostic msg: /tmp/aa-10084-fd1069.c
clang-trunk: note: diagnostic msg: /tmp/aa-10084-fd1069.sh
clang-trunk: note: diagnostic msg:

********************
```
---
The testcase as well as the profiling data (in txt):

```
$ cat aa-10084.c
long b;
short c, f, g, h;
int i, j, k, l;
short(a)(short m, short o) { return m + o; }
int(d)(int m, int o) { return o == 0 ? 0 : m % o; }
char(e)(char m, char o) { return m + o; }
static long func_21();
void n() { func_21(); }
long func_21() {
  int p = 0;
  for (; p < 6; p = e(p, 4))
    if ((0 && p) | (((9 && 1) > 0 && 0 && h) == 0) >= 0) {
      j = 0;
      for (; j != 12; j = a(j, 4)) {
        for (; 9 == 0;) {
          (5 ==
           (0 >= ((7 != !!((p || 0 <= (p >= p) && 3) <= 0) > p == p) <= 3 &&
                  p)) >= f) >= p;
          for (; 0 == 2;)
            ;
          if (4073709551607)
            continue;
        }
        for (; 0 >= 6;) {
          1 <= 0;
          return 0;
        }
        if ((5 || 0) <=
            (b && (c = d(((0 != (0 || p)) < 5 || p) >= b == 0 != i, p)) < p))
          (p ==
           ((0 == (!(((!((0 == 5 && 4 && 1) <= p) > 5 != l) <= 5) > 0 || 3) &&
                   p)) >= 0 &&
            0)) <= 0;
        else
          p > (4 && j);
      }
      for (; 0 >= 4;) {
        1 <= ((p || (c >= k) >= (b == p) || c) > 9);
        return p;
      }
    } else {
      !0x81EC1E3B16488FA2LL;
      for (; 0 <= 9;)
        ;
      !!((1 > ((g >= ((4 || 4073709551609) != 1) == p) < 2) || 1) < 6);
    }
  p >= (((4 && (p || (((p && p > (4073709551613 != (0 && j) < p)) == 5 && 8) >
                      p) > 4) == 2 != 0) < 1) >= h !=
        b);
  return j;
}
void main() {}
```

```
$ cat prof.txt
func_16
9015145131845
48
1
0
4
4
6
4
4
4
9
1
7
4
0
8
5
8
4
7
9
7
3
3
0
1
0
7
1
1
7
6
3
8
4
5
6
7
8
8
0
3
7
6
3
9
2
2
2
2
aa-10084.c:func_21
774005734872250401
62
1
9
8
9
4
6
0
2
3
8
2
7
8
7
1
6
5
3
4
9
30
9560
7
0
9
4
8
8
5
4
2
6
4
9
2
9
9
3
4
2
1
2
3
8
2
2
4
0
3
0
0
6
3
2
5
1
8
5
7
1
3
0
```


## Comments

### Comment 1 - helloqirun

Tried it on the latest version: f2be60c40ef485f13

It seems to be fixed.

---

### Comment 2 - fhahn

Thanks @helloqirun, closing for now. Please re-open if it still reproduces

---

