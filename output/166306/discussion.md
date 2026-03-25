# [opt] [pseudo-probe-update] Casting Error on Probe Update on Code Sinking

**Author:** fraglantia
**URL:** https://github.com/llvm/llvm-project/issues/166306

## Body

`gvn-sink` sinks the probes inserted by `pseudo-probe`:
```
# for example it sinks from
# ...
call void @pseudoprobe(…, i64 2, …)
# ...
call void @pseudoprobe(…, i64 3, …)

# to be
%.sink = phi i64 [ 3, %7 ], [ 2, %6 ]
call void @pseudoprobe(…, i64 %.sink, …)
```

`pseudo-probe-update` expects (casts) the argument to be a `ConstantInt`, while after sinking it's a phi node instead which causes the casting error.

Reproducer:
https://godbolt.org/z/W3EcreK8z

Backtrace:
```
opt: /root/llvm-project/llvm/include/llvm/Support/Casting.h:572: decltype(auto) llvm::cast(From*) [with To = llvm::ConstantInt; From = llvm::Value]: Assertion `isa<To>(Val) && "cast<Ty>() argument of incompatible type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/opt -o /app/output.s -S --passes=pseudo-probe,cgscc(function(gvn-sink)),pseudo-probe-update <source>
1.	Running pass "pseudo-probe-update" on module "<source>"
 #0 0x00000000059945f8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x59945f8)
 #1 0x00000000059914a4 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x00007af2e6242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x00007af2e62969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x00007af2e6242476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x00007af2e62287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x00007af2e622871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #7 0x00007af2e6239e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #8 0x00000000057844de llvm::extractProbe(llvm::Instruction const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x57844de)
 #9 0x00000000033b2173 llvm::PseudoProbeUpdatePass::runOnFunction(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x33b2173)
#10 0x00000000033b26da llvm::PseudoProbeUpdatePass::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x33b26da)
#11 0x0000000002fb413e llvm::detail::PassModel<llvm::Module, llvm::PseudoProbeUpdatePass, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x2fb413e)
#12 0x000000000576acc1 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x576acc1)
#13 0x0000000000986d8a llvm::runPassPipeline(llvm::StringRef, llvm::Module&, llvm::TargetMachine*, llvm::TargetLibraryInfoImpl*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::StringRef, llvm::ArrayRef<llvm::PassPlugin>, llvm::ArrayRef<std::function<void (llvm::PassBuilder&)>>, llvm::opt_tool::OutputKind, llvm::opt_tool::VerifierKind, bool, bool, bool, bool, bool, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x986d8a)
#14 0x000000000097b001 optMain (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x97b001)
#15 0x00007af2e6229d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#16 0x00007af2e6229e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#17 0x0000000000972415 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x972415)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

