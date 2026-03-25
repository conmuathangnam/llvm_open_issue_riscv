# opt crashes in CodeGenPrepare.cpp

**Author:** Emilyaxe
**URL:** https://github.com/llvm/llvm-project/issues/173360
**Status:** Open
**Labels:** llvm:codegen, crash

## Body

When running opt on the following IR, opt crashes. 
I am not sure whether this is a valid case. If using opt in this way is not appropriate, I am open to closing this issue.
godbolt: https://godbolt.org/z/e7xc4h93Y

llvm version 50ae726bb3498

test.ll

```
target triple = "x86_64-unknown-linux-gnu"
define void @f(ptr %A, i32 %n) {
entry:
  %iv.next.reg2mem = alloca i32, align 4
  ret void
}

```


commads
./bin/opt test.ll -passes=codegenprepare -S

stacktrace

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/opt -o /app/output.s -S -passes=codegenprepare -S <source>
1.	Running pass "function(codegenprepare)" on module "<source>"
2.	Running pass "codegenprepare" on function "f"
 #0 0x0000000005a44d98 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5a44d98)
 #1 0x0000000005a41c44 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x00007cf32d042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x0000000003fa0425 (anonymous namespace)::CodeGenPrepare::_run(llvm::Function&) CodeGenPrepare.cpp:0:0
 #4 0x0000000003fa3b70 llvm::CodeGenPreparePass::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x3fa3b70)
 #5 0x0000000000f183ee llvm::detail::PassModel<llvm::Function, llvm::CodeGenPreparePass, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0xf183ee)
 #6 0x0000000005821031 llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5821031)
 #7 0x0000000000f1952e llvm::detail::PassModel<llvm::Function, llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0xf1952e)
 #8 0x000000000581f6fa llvm::ModuleToFunctionPassAdaptor::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x581f6fa)
 #9 0x000000000097d43e llvm::detail::PassModel<llvm::Module, llvm::ModuleToFunctionPassAdaptor, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x97d43e)
#10 0x000000000581f0b1 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x581f0b1)
#11 0x000000000098776a llvm::runPassPipeline(llvm::StringRef, llvm::Module&, llvm::TargetMachine*, llvm::TargetLibraryInfoImpl*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::StringRef, llvm::ArrayRef<llvm::PassPlugin>, llvm::ArrayRef<std::function<void (llvm::PassBuilder&)>>, llvm::opt_tool::OutputKind, llvm::opt_tool::VerifierKind, bool, bool, bool, bool, bool, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x98776a)
#12 0x000000000097b5e8 optMain (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x97b5e8)
#13 0x00007cf32d029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#14 0x00007cf32d029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#15 0x0000000000972085 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x972085)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```


