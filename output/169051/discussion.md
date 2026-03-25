# Opt crashes on simple input with command line arguments

**Author:** aleksisch
**URL:** https://github.com/llvm/llvm-project/issues/169051

## Body

Apparently these command line options incorrect, however I believe we should print something meaningful
```
$ /home/llvm_release/bin/opt -passes='stack-slot-coloring' -print-after=stack-slot-coloring opt.ll -S
opt: /home/llvm-project/llvm/include/llvm/IR/PassManager.h:412: typename PassT::Result& llvm::AnalysisManager<IRUnitT, ExtraArgTs>::getResult(IRUnitT&, ExtraArgTs ...) [with PassT = llvm::InnerAnalysisManagerProxy<llvm::AnalysisManager<llvm::MachineFunction>, llvm::Function>; IRUnitT = llvm::Function; ExtraArgTs = {}; typename PassT::Result = llvm::InnerAnalysisManagerProxy<llvm::AnalysisManager<llvm::MachineFunction>, llvm::Function>::Result]: Assertion `AnalysisPasses.count(PassT::ID()) && "This analysis pass was not registered prior to being queried"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /home/llvm_release/bin/opt -passes=stack-slot-coloring -print-after=stack-slot-coloring opt.ll -S
1.	Running pass "function(machine-function(stack-slot-coloring))" on module "opt.ll"
2.	Running pass "machine-function(stack-slot-coloring)" on function "main"
 #0 0x00005f1a7c9e4d88 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /home/llvm-project/llvm/lib/Support/Unix/Signals.inc:834:22
 #1 0x00005f1a7c9e51fb PrintStackTraceSignalHandler(void*) /home/llvm-project/llvm/lib/Support/Unix/Signals.inc:916:1
 #2 0x00005f1a7c9e2373 llvm::sys::RunSignalHandlers() /home/llvm-project/llvm/lib/Support/Signals.cpp:104:20
 #3 0x00005f1a7c9e45c5 SignalHandler(int, siginfo_t*, void*) /home/llvm-project/llvm/lib/Support/Unix/Signals.inc:426:14
 #4 0x000070b6c2a45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #5 0x000070b6c2a9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #6 0x000070b6c2a9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #7 0x000070b6c2a9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #8 0x000070b6c2a4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #9 0x000070b6c2a288ff abort ./stdlib/abort.c:81:7
#10 0x000070b6c2a2881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#11 0x000070b6c2a3b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#12 0x00005f1a7d5c18fd llvm::InnerAnalysisManagerProxy<llvm::AnalysisManager<llvm::MachineFunction>, llvm::Function>::Result& llvm::AnalysisManager<llvm::Function>::getResult<llvm::InnerAnalysisManagerProxy<llvm::AnalysisManager<llvm::MachineFunction>, llvm::Function>>(llvm::Function&) /home/llvm-project/llvm/include/llvm/IR/PassManager.h:415:22
#13 0x00005f1a7d5bf48d llvm::FunctionToMachineFunctionPassAdaptor::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) /home/llvm-project/llvm/lib/CodeGen/MachinePassManager.cpp:116:22
#14 0x00005f1a7ef22ddd llvm::detail::PassModel<llvm::Function, llvm::FunctionToMachineFunctionPassAdaptor, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) /home/llvm-project/llvm/include/llvm/IR/PassManagerInternal.h:92:3
#15 0x00005f1a7ce48439 llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) /home/llvm-project/llvm/include/llvm/IR/PassManagerImpl.h:80:18
#16 0x00005f1a7ef25357 llvm::detail::PassModel<llvm::Function, llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) /home/llvm-project/llvm/include/llvm/IR/PassManagerInternal.h:92:3
#17 0x00005f1a7ce46caa llvm::ModuleToFunctionPassAdaptor::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) /home/llvm-project/llvm/lib/IR/PassManager.cpp:132:23
#18 0x00005f1a7ef251a1 llvm::detail::PassModel<llvm::Module, llvm::ModuleToFunctionPassAdaptor, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) /home/llvm-project/llvm/include/llvm/IR/PassManagerInternal.h:92:3
#19 0x00005f1a7ce480a5 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) /home/llvm-project/llvm/include/llvm/IR/PassManagerImpl.h:80:18
#20 0x00005f1a826bfae0 llvm::runPassPipeline(llvm::StringRef, llvm::Module&, llvm::TargetMachine*, llvm::TargetLibraryInfoImpl*, llvm::RTLIB::RuntimeLibcallsInfo&, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::StringRef, llvm::ArrayRef<llvm::PassPlugin>, llvm::ArrayRef<std::function<void (llvm::PassBuilder&)>>, llvm::opt_tool::OutputKind, llvm::opt_tool::VerifierKind, bool, bool, bool, bool, bool, bool, bool, bool) /home/llvm-project/llvm/tools/opt/NewPMDriver.cpp:563:10
#21 0x00005f1a7c937697 optMain /home/llvm-project/llvm/tools/opt/optdriver.cpp:756:25
#22 0x00005f1a7c934a41 main /home/llvm-project/llvm/tools/opt/opt.cpp:27:64
#23 0x000070b6c2a2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#24 0x000070b6c2a2a28b call_init ./csu/../csu/libc-start.c:128:20
#25 0x000070b6c2a2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#26 0x00005f1a7c934925 _start (/home/llvm_release/bin/opt+0x8ac3925)
Aborted (core dumped)
$ cat opt.ll 
define dllexport <4 x float> @"main"() local_unnamed_addr {
  ret <4 x float> undef
}
```
https://alive2.llvm.org/ce/z/dMAePy

## Comments

### Comment 1 - Sriharika1506

I want to work on it, please assign me !!!

---

### Comment 2 - Sriharika1506

@EugeneZelenko Could you please assign this issue to me ?

---

### Comment 3 - Harikaraja

 @EugeneZelenko @aleksisch 

I am experimenting with adding MachineFunction analysis support in opt by taking inspiration from llc’s NewPMDriver. Specifically, I added:

```
MachineFunctionAnalysisManager MFAM;

PB.registerMachineFunctionAnalyses(MFAM);

MAM.registerPass([&] { return MachineModuleAnalysis(*MMI); })
```

After this change, opt successfully compiles, and the earlier compile-time errors are gone.

However, opt now crashes at runtime when I try to run any MachineFunction pass, for example:

`./bin/opt -passes=stack-slot-coloring sample.ll`

`Stack dump:
 Program arguments: ./bin/opt -passes=stack-slot-coloring sample.ll
Running pass "function(machine-function(stack-slot-coloring))" on module "sample.ll"
Running pass "machine-function(stack-slot-coloring)" on function "_Z5sampleRSt6vectorIiSaIiEEiii"
 #0 0x000055edab7cbd7f llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (.localalias) (./bin/opt+0x353dd7f)
 #1 0x000055edab7c8a34 SignalHandler(int, siginfo_t*, void*) (./bin/opt+0x353aa34)
 #2 0x00007f116ec9d420 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x14420)
 #3 0x000055eda8744d74 llvm::MachineFunctionAnalysis::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (.cold) (./bin/opt+0x4b6d74)
 #4 0x000055eda8da740a llvm::detail::AnalysisPassModel<llvm::Function, llvm::MachineFunctionAnalysis, llvm::AnalysisManager<llvm::Function>::Invalidator>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (./bin/opt+0xb1940a)
 #5 0x000055edab5beb41 llvm::AnalysisManager<llvm::Function>::getResultImpl(llvm::AnalysisKey*, llvm::Function&) (./bin/opt+0x3330b41)
 #6 0x000055eda9f1735a llvm::FunctionToMachineFunctionPassAdaptor::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (./bin/opt+0x1c8935a)
 #7 0x000055eda88c9996 llvm::detail::PassModel<llvm::Function, llvm::FunctionToMachineFunctionPassAdaptor, llvm::AnalysisManager<llvm::Function> >::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (./bin/opt+0x63b996)
 #8 0x000055edab5bf611 llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function> >::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (./bin/opt+0x3331611)
 #9 0x000055eda88c8f36 llvm::detail::PassModel<llvm::Function, llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function> >, llvm::AnalysisManager<llvm::Function> >::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (./bin/opt+0x63af36)
#10 0x000055edab5bdbfe llvm::ModuleToFunctionPassAdaptor::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (./bin/opt+0x332fbfe)
#11 0x000055eda881f2a6 llvm::detail::PassModel<llvm::Module, llvm::ModuleToFunctionPassAdaptor, llvm::AnalysisManager<llvm::Module> >::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (./bin/opt+0x5912a6)
#12 0x000055edab5bd591 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module> >::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (./bin/opt+0x332f591)
#13 0x000055eda882a202 llvm::runPassPipeline(llvm::StringRef, llvm::Module&, llvm::TargetMachine*, llvm::TargetLibraryInfoImpl*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::StringRef, llvm::ArrayRef<llvm::PassPlugin>, 
llvm::ArrayRef<std::function<void (llvm::PassBuilder&)> >, llvm::opt_tool::OutputKind, llvm::opt_tool::VerifierKind, bool, bool, bool, bool, bool, bool, bool, bool) (./bin/opt+0x59c202)
#14 0x000055eda881d19e optMain (./bin/opt+0x58f19e)
#15 0x00007f116e612083 __libc_start_main /build/glibc-B3wQXB/glibc-2.31/csu/../csu/libc-start.c:342:3
#16 0x000055eda8813ede _start (./bin/opt+0x585ede)
Segmentation fault (core `dumped)`

**Does opt officially support running MachineFunction passes under the new pass manager?**

---

