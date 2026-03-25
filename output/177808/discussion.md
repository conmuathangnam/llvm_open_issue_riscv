# Polly crash: polly/lib/External/isl/isl_map_simplify.c:5992: total dimensionality changed unexpectedly

**Author:** Meinersbur
**URL:** https://github.com/llvm/llvm-project/issues/177808
**Status:** Closed
**Labels:** polly, crash
**Closed Date:** 2026-02-02T23:02:05Z

## Body

This bug was reported at https://github.com/llvm/llvm-project/pull/177776#issuecomment-3795306837

Polly crashes trying the optimized the following file (LLVM 5dec140ab2bd)

[reduced.ll.txt](https://github.com/user-attachments/files/24840978/reduced.ll.txt)

```sh
$ opt reduced.ll -polly-vectorizer=stripmine -passes="polly"
WARNING: You're attempting to print out a bitcode file.
This is inadvisable as it may cause display problems. If
you REALLY want to taste LLVM bitcode first-hand, you
can force output with the `-f' option.

/home/meinersbur/src/llvm/main/_src/polly/lib/External/isl/isl_map_simplify.c:6373: total dimensionality changed unexpectedly
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: ../release/bin/opt reduced.ll -polly-vectorizer=stripmine -passes=polly
1.      Running pass "function(polly)" on module "reduced.ll"
 #0 0x00007f6a32750512 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/home/meinersbur/src/llvm/main/release/bin/../lib/libLLVMSupport.so.23.0git+0x2dd512)
 #1 0x00007f6a3274d23f llvm::sys::RunSignalHandlers() (/home/meinersbur/src/llvm/main/release/bin/../lib/libLLVMSupport.so.23.0git+0x2da23f)
 #2 0x00007f6a3274d38c SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x00007f6a322a5330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x00007f6a322feb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x00007f6a322feb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x00007f6a322feb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x00007f6a322a527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x00007f6a322888ff abort ./stdlib/abort.c:81:7
 #9 0x00007f6a2aac77d1 (/home/meinersbur/src/llvm/main/release/bin/../lib/../lib/../lib/libPollyISL.so+0x1647d1)
#10 0x00007f6a2aac80ec (/home/meinersbur/src/llvm/main/release/bin/../lib/../lib/../lib/libPollyISL.so+0x1650ec)
#11 0x00007f6a2ab46b4e reduce_coefficients isl_map_simplify.c:0:0
#12 0x00007f6a2ab4c570 isl_basic_map_reduce_coefficients (/home/meinersbur/src/llvm/main/release/bin/../lib/../lib/../lib/libPollyISL.so+0x1e9570)
#13 0x00007f6a2aab6d24 isl_map_coalesce (/home/meinersbur/src/llvm/main/release/bin/../lib/../lib/../lib/libPollyISL.so+0x153d24)
#14 0x00007f6a2aa9df80 generate_domain isl_ast_codegen.c:0:0
#15 0x00007f6a2aae662c isl_hash_table_foreach (/home/meinersbur/src/llvm/main/release/bin/../lib/../lib/../lib/libPollyISL.so+0x18362c)
#16 0x00007f6a2ac144a1 isl_union_map_foreach_map (/home/meinersbur/src/llvm/main/release/bin/../lib/../lib/../lib/libPollyISL.so+0x2b14a1)
#17 0x00007f6a2aaa1c19 generate_inner_level isl_ast_codegen.c:0:0
#18 0x00007f6a2aaa126b build_ast_from_schedule_node isl_ast_codegen.c:0:0
#19 0x00007f6a2aaa1c7e generate_inner_level isl_ast_codegen.c:0:0
#20 0x00007f6a2aaa1df4 create_node_scaled isl_ast_codegen.c:0:0
#21 0x00007f6a2aaa3168 add_node isl_ast_codegen.c:0:0
#22 0x00007f6a2aaa3428 generate_sorted_domains isl_ast_codegen.c:0:0
#23 0x00007f6a2aaa355a generate_sorted_domains_wrap isl_ast_codegen.c:0:0
#24 0x00007f6a2abd0bb3 isl_basic_set_list_foreach_scc (/home/meinersbur/src/llvm/main/release/bin/../lib/../lib/../lib/libPollyISL.so+0x26dbb3)
#25 0x00007f6a2aaa34b6 generate_parallel_domains isl_ast_codegen.c:0:0
#26 0x00007f6a2aa9f11d generate_shifted_component_tree_base isl_ast_codegen.c:0:0
#27 0x00007f6a2aaa13ef build_ast_from_schedule_node isl_ast_codegen.c:0:0
#28 0x00007f6a2aaa3b8c isl_ast_build_node_from_schedule (/home/meinersbur/src/llvm/main/release/bin/../lib/../lib/../lib/libPollyISL.so+0x140b8c)
#29 0x00007f6a2f4332d9 polly::IslAst::init(polly::Dependences const&) (/home/meinersbur/src/llvm/main/release/bin/../lib/../lib/libPolly.so.23.0git+0x15c2d9)
#30 0x00007f6a2f433787 polly::runIslAstGen(polly::Scop&, polly::DependenceAnalysis::Result&) (/home/meinersbur/src/llvm/main/release/bin/../lib/../lib/libPolly.so.23.0git+0x15c787)
#31 0x00007f6a2f488863 polly::runPollyPass(llvm::Function&, llvm::AnalysisManager<llvm::Function>&, polly::PollyPassOptions) (/home/meinersbur/src/llvm/main/release/bin/../lib/../lib/libPolly.so.23.0git+0x1b1863)
#32 0x00007f6a2f4897cf polly::PollyFunctionPass::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/home/meinersbur/src/llvm/main/release/bin/../lib/../lib/libPolly.so.23.0git+0x1b27cf)
#33 0x00007f6a2f4a0a25 llvm::detail::PassModel<llvm::Function, polly::PollyFunctionPass, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) RegisterPasses.cpp:0:0
#34 0x00007f6a2c4f8c51 llvm::ModuleToFunctionPassAdaptor::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/home/meinersbur/src/llvm/main/release/bin/../lib/../lib/libLLVMCore.so.23.0git+0x532c51)
#35 0x00007f6a32785365 llvm::detail::PassModel<llvm::Module, llvm::ModuleToFunctionPassAdaptor, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) NewPMDriver.cpp:0:0
#36 0x00007f6a2c4f943d llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/home/meinersbur/src/llvm/main/release/bin/../lib/../lib/libLLVMCore.so.23.0git+0x53343d)
#37 0x00007f6a32793f8c llvm::runPassPipeline(llvm::StringRef, llvm::Module&, llvm::TargetMachine*, llvm::TargetLibraryInfoImpl*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::StringRef, llvm::ArrayRef<llvm::PassPlugin>, llvm::ArrayRef<std::function<void (llvm::PassBuilder&)>>, llvm::opt_tool::OutputKind, llvm::opt_tool::VerifierKind, bool, bool, bool, bool, bool, bool, bool, bool) (/home/meinersbur/src/llvm/main/release/bin/../lib/libLLVMOptDriver.so.23.0git+0x2bf8c)
#38 0x00007f6a327a2f77 optMain (/home/meinersbur/src/llvm/main/release/bin/../lib/libLLVMOptDriver.so.23.0git+0x3af77)
#39 0x00007f6a3228a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#40 0x00007f6a3228a28b call_init ./csu/../csu/libc-start.c:128:20
#41 0x00007f6a3228a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#42 0x00007f6a327fc835 _start (../release/bin/opt+0x1835)
Aborted (core dumped)                                            
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-polly

Author: Michael Kruse (Meinersbur)

<details>
This is bug reported at https://github.com/llvm/llvm-project/pull/177776#issuecomment-3795306837

Polly crashes trying the optimized the following file

[reduced.ll.txt](https://github.com/user-attachments/files/24840978/reduced.ll.txt)

```sh
$ opt reduced.ll   -polly-vectorizer=stripmine          -passes="polly"
WARNING: You're attempting to print out a bitcode file.
This is inadvisable as it may cause display problems. If
you REALLY want to taste LLVM bitcode first-hand, you
can force output with the `-f' option.

/home/meinersbur/src/llvm/main/_src/polly/lib/External/isl/isl_map_simplify.c:6373: total dimensionality changed unexpectedly
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: ../release/bin/opt reduced.ll -polly-vectorizer=stripmine -passes=polly
1.      Running pass "function(polly)" on module "reduced.ll"
 #<!-- -->0 0x00007f6a32750512 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/home/meinersbur/src/llvm/main/release/bin/../lib/libLLVMSupport.so.23.0git+0x2dd512)
 #<!-- -->1 0x00007f6a3274d23f llvm::sys::RunSignalHandlers() (/home/meinersbur/src/llvm/main/release/bin/../lib/libLLVMSupport.so.23.0git+0x2da23f)
 #<!-- -->2 0x00007f6a3274d38c SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x00007f6a322a5330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->4 0x00007f6a322feb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->5 0x00007f6a322feb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->6 0x00007f6a322feb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->7 0x00007f6a322a527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->8 0x00007f6a322888ff abort ./stdlib/abort.c:81:7
 #<!-- -->9 0x00007f6a2aac77d1 (/home/meinersbur/src/llvm/main/release/bin/../lib/../lib/../lib/libPollyISL.so+0x1647d1)
#<!-- -->10 0x00007f6a2aac80ec (/home/meinersbur/src/llvm/main/release/bin/../lib/../lib/../lib/libPollyISL.so+0x1650ec)
#<!-- -->11 0x00007f6a2ab46b4e reduce_coefficients isl_map_simplify.c:0:0
#<!-- -->12 0x00007f6a2ab4c570 isl_basic_map_reduce_coefficients (/home/meinersbur/src/llvm/main/release/bin/../lib/../lib/../lib/libPollyISL.so+0x1e9570)
#<!-- -->13 0x00007f6a2aab6d24 isl_map_coalesce (/home/meinersbur/src/llvm/main/release/bin/../lib/../lib/../lib/libPollyISL.so+0x153d24)
#<!-- -->14 0x00007f6a2aa9df80 generate_domain isl_ast_codegen.c:0:0
#<!-- -->15 0x00007f6a2aae662c isl_hash_table_foreach (/home/meinersbur/src/llvm/main/release/bin/../lib/../lib/../lib/libPollyISL.so+0x18362c)
#<!-- -->16 0x00007f6a2ac144a1 isl_union_map_foreach_map (/home/meinersbur/src/llvm/main/release/bin/../lib/../lib/../lib/libPollyISL.so+0x2b14a1)
#<!-- -->17 0x00007f6a2aaa1c19 generate_inner_level isl_ast_codegen.c:0:0
#<!-- -->18 0x00007f6a2aaa126b build_ast_from_schedule_node isl_ast_codegen.c:0:0
#<!-- -->19 0x00007f6a2aaa1c7e generate_inner_level isl_ast_codegen.c:0:0
#<!-- -->20 0x00007f6a2aaa1df4 create_node_scaled isl_ast_codegen.c:0:0
#<!-- -->21 0x00007f6a2aaa3168 add_node isl_ast_codegen.c:0:0
#<!-- -->22 0x00007f6a2aaa3428 generate_sorted_domains isl_ast_codegen.c:0:0
#<!-- -->23 0x00007f6a2aaa355a generate_sorted_domains_wrap isl_ast_codegen.c:0:0
#<!-- -->24 0x00007f6a2abd0bb3 isl_basic_set_list_foreach_scc (/home/meinersbur/src/llvm/main/release/bin/../lib/../lib/../lib/libPollyISL.so+0x26dbb3)
#<!-- -->25 0x00007f6a2aaa34b6 generate_parallel_domains isl_ast_codegen.c:0:0
#<!-- -->26 0x00007f6a2aa9f11d generate_shifted_component_tree_base isl_ast_codegen.c:0:0
#<!-- -->27 0x00007f6a2aaa13ef build_ast_from_schedule_node isl_ast_codegen.c:0:0
#<!-- -->28 0x00007f6a2aaa3b8c isl_ast_build_node_from_schedule (/home/meinersbur/src/llvm/main/release/bin/../lib/../lib/../lib/libPollyISL.so+0x140b8c)
#<!-- -->29 0x00007f6a2f4332d9 polly::IslAst::init(polly::Dependences const&amp;) (/home/meinersbur/src/llvm/main/release/bin/../lib/../lib/libPolly.so.23.0git+0x15c2d9)
#<!-- -->30 0x00007f6a2f433787 polly::runIslAstGen(polly::Scop&amp;, polly::DependenceAnalysis::Result&amp;) (/home/meinersbur/src/llvm/main/release/bin/../lib/../lib/libPolly.so.23.0git+0x15c787)
#<!-- -->31 0x00007f6a2f488863 polly::runPollyPass(llvm::Function&amp;, llvm::AnalysisManager&lt;llvm::Function&gt;&amp;, polly::PollyPassOptions) (/home/meinersbur/src/llvm/main/release/bin/../lib/../lib/libPolly.so.23.0git+0x1b1863)
#<!-- -->32 0x00007f6a2f4897cf polly::PollyFunctionPass::run(llvm::Function&amp;, llvm::AnalysisManager&lt;llvm::Function&gt;&amp;) (/home/meinersbur/src/llvm/main/release/bin/../lib/../lib/libPolly.so.23.0git+0x1b27cf)
#<!-- -->33 0x00007f6a2f4a0a25 llvm::detail::PassModel&lt;llvm::Function, polly::PollyFunctionPass, llvm::AnalysisManager&lt;llvm::Function&gt;&gt;::run(llvm::Function&amp;, llvm::AnalysisManager&lt;llvm::Function&gt;&amp;) RegisterPasses.cpp:0:0
#<!-- -->34 0x00007f6a2c4f8c51 llvm::ModuleToFunctionPassAdaptor::run(llvm::Module&amp;, llvm::AnalysisManager&lt;llvm::Module&gt;&amp;) (/home/meinersbur/src/llvm/main/release/bin/../lib/../lib/libLLVMCore.so.23.0git+0x532c51)
#<!-- -->35 0x00007f6a32785365 llvm::detail::PassModel&lt;llvm::Module, llvm::ModuleToFunctionPassAdaptor, llvm::AnalysisManager&lt;llvm::Module&gt;&gt;::run(llvm::Module&amp;, llvm::AnalysisManager&lt;llvm::Module&gt;&amp;) NewPMDriver.cpp:0:0
#<!-- -->36 0x00007f6a2c4f943d llvm::PassManager&lt;llvm::Module, llvm::AnalysisManager&lt;llvm::Module&gt;&gt;::run(llvm::Module&amp;, llvm::AnalysisManager&lt;llvm::Module&gt;&amp;) (/home/meinersbur/src/llvm/main/release/bin/../lib/../lib/libLLVMCore.so.23.0git+0x53343d)
#<!-- -->37 0x00007f6a32793f8c llvm::runPassPipeline(llvm::StringRef, llvm::Module&amp;, llvm::TargetMachine*, llvm::TargetLibraryInfoImpl*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::StringRef, llvm::ArrayRef&lt;llvm::PassPlugin&gt;, llvm::ArrayRef&lt;std::function&lt;void (llvm::PassBuilder&amp;)&gt;&gt;, llvm::opt_tool::OutputKind, llvm::opt_tool::VerifierKind, bool, bool, bool, bool, bool, bool, bool, bool) (/home/meinersbur/src/llvm/main/release/bin/../lib/libLLVMOptDriver.so.23.0git+0x2bf8c)
#<!-- -->38 0x00007f6a327a2f77 optMain (/home/meinersbur/src/llvm/main/release/bin/../lib/libLLVMOptDriver.so.23.0git+0x3af77)
#<!-- -->39 0x00007f6a3228a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->40 0x00007f6a3228a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->41 0x00007f6a3228a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->42 0x00007f6a327fc835 _start (../release/bin/opt+0x1835)
Aborted (core dumped)                                            
```
</details>


---

### Comment 2 - Meinersbur

Upstream bug report: https://groups.google.com/g/isl-development/c/flxZEBW1o-E/m/gk1sm611AQAJ

---

