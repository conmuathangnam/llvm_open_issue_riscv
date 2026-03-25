# opt crashes when running a backend-only pass (block-placement) in llvm/include/llvm/IR/PassManager.h:412

**Author:** Emilyaxe
**URL:** https://github.com/llvm/llvm-project/issues/172644
**Status:** Open
**Labels:** crash, llvm

## Body

When running opt with the backend-only pass block-placement, opt crashes with an assertion failure instead of reporting an error (e.g., “unknown pass name”). Initially, I was not aware that block-placement is a Machine IR (backend) pass. However,  I think that opt should reject backend-only passes, rather than crashing. I am not sure whether this is a valid test case, and I would appreciate any clarification.

llvm version: 3186ca25b

test.ll

```
@G = external global i32
@G1 = internal constant [2 x ptr] [ptr @G, ptr @G]

define void @foo(ptr %A) {
entry:
  store i8 7, ptr %A, align 1
  br label %L1

L1:                                               ; preds = %entry
  store i8 8, ptr %A, align 1
  br label %L4

L4:                                               ; preds = %L1
  %t = load i32, ptr @G, align 4
  %c = icmp ne i32 %t, 0
  br i1 %c, label %L5, label %InvalidExit

L5:                                               ; preds = %L4
  ret void

InvalidExit:                                      ; preds = %L4
  br label %L5
}
```



commads
./bin/opt -passes=block-placement test.ll  -S


stacktrace

```
opt: path-to-llvm-project/llvm/include/llvm/IR/PassManager.h:412: typename PassT::Result& llvm::AnalysisManager<IRUnitT, ExtraArgTs>::getResult(IRUnitT&, ExtraArgTs ...) [with PassT = llvm::InnerAnalysisManagerProxy<llvm::AnalysisManager<llvm::MachineFunction>, llvm::Function>; IRUnitT = llvm::Function; ExtraArgTs = {}; typename PassT::Result = llvm::InnerAnalysisManagerProxy<llvm::AnalysisManager<llvm::MachineFunction>, llvm::Function>::Result]: Assertion `AnalysisPasses.count(PassT::ID()) && "This analysis pass was not registered prior to being queried"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: ./bin/opt test.ll -passes=block-placement -S
1.	Running pass "function(machine-function(block-placement))" on module "reported_bugs/1.ll"
2.	Running pass "machine-function(block-placement)" on function "foo"
 #0 0x000055909269559f llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (./bin/opt+0x560559f)
 #1 0x000055909269220c llvm::sys::RunSignalHandlers() (./bin/opt+0x560220c)
 #2 0x00005590926923a7 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x00007f86abdeb420 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x14420)
 #4 0x00007f86ab43800b raise /build/glibc-LcI20x/glibc-2.31/signal/../sysdeps/unix/sysv/linux/raise.c:51:1
 #5 0x00007f86ab417859 abort /build/glibc-LcI20x/glibc-2.31/stdlib/abort.c:81:7
 #6 0x00007f86ab417729 get_sysdep_segment_value /build/glibc-LcI20x/glibc-2.31/intl/loadmsgcat.c:509:8
 #7 0x00007f86ab417729 _nl_load_domain /build/glibc-LcI20x/glibc-2.31/intl/loadmsgcat.c:970:34
 #8 0x00007f86ab428fd6 (/lib/x86_64-linux-gnu/libc.so.6+0x33fd6)
 #9 0x0000559090d823e2 llvm::FunctionToMachineFunctionPassAdaptor::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (./bin/opt+0x3cf23e2)
#10 0x000055908de7bec6 llvm::detail::PassModel<llvm::Function, llvm::FunctionToMachineFunctionPassAdaptor, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (./bin/opt+0xdebec6)
#11 0x0000559092461161 llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (./bin/opt+0x53d1161)
#12 0x000055908de7dd26 llvm::detail::PassModel<llvm::Function, llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (./bin/opt+0xdedd26)
#13 0x000055909245f79e llvm::ModuleToFunctionPassAdaptor::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (./bin/opt+0x53cf79e)
#14 0x000055908d8d5b56 llvm::detail::PassModel<llvm::Module, llvm::ModuleToFunctionPassAdaptor, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (./bin/opt+0x845b56)
#15 0x000055909245f131 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (./bin/opt+0x53cf131)
#16 0x000055908d8e057a llvm::runPassPipeline(llvm::StringRef, llvm::Module&, llvm::TargetMachine*, llvm::TargetLibraryInfoImpl*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::StringRef, llvm::ArrayRef<llvm::PassPlugin>, llvm::ArrayRef<std::function<void (llvm::PassBuilder&)>>, llvm::opt_tool::OutputKind, llvm::opt_tool::VerifierKind, bool, bool, bool, bool, bool, bool, bool, bool) (./bin/opt+0x85057a)
#17 0x000055908d8d394e optMain (./bin/opt+0x84394e)
#18 0x00007f86ab419083 __libc_start_main /build/glibc-LcI20x/glibc-2.31/csu/../csu/libc-start.c:342:3
#19 0x000055908d8c9eae _start (./bin/opt+0x839eae)

```


