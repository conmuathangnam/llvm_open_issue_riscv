# Invalid IR crashing clang-19.1.7 O3

**Author:** connorl309
**URL:** https://github.com/llvm/llvm-project/issues/125123

## Body

Hi, I have received the following crash and error information from running clang on invalid IR. Is clang supposed to crash on invalid IR? Is this considered to be a bug?

opt-verify yields the following error on a generated .ll IR file:

```
PHI node entries do not match predecessors!
  %0 = phi i32 [ %1, %execute_if ], [ %2, %div ]
label %div
label %cont
opt-19: local2ifelse.ll: error: input module is broken!
```
I recognize that the IR is invalid, but if it is run through clang-19 with the following command:
`clang-19 -O3 local2ifelse.ll`

I receive the following crashing backtrace:
```
warning: overriding the module target triple with x86_64-pc-linux-gnu [-Woverride-module]
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /usr/lib/llvm-19/bin/clang -cc1 -triple x86_64-pc-linux-gnu -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name local2ifelse.ll -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=none -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/root -fcoverage-compilation-dir=/root -resource-dir /usr/lib/llvm-19/lib/clang/19 -O3 -ferror-limit 19 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -vectorize-loops -vectorize-slp -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/local2ifelse-a8a9ae.o -x ir local2ifelse.ll
1.      Optimizer
2.      Running pass "function<eager-inv>(ee-instrument<>,lower-expect,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;no-switch-range-to-icmp;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,sroa<modify-cfg>,early-cse<>,callsite-splitting)" on module "local2ifelse.ll"
3.      Running pass "simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;no-switch-range-to-icmp;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>" on function "Main_main"
 #0 0x00007efdaf705876 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.1+0xea9876)
 #1 0x00007efdaf703450 llvm::sys::RunSignalHandlers() (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.1+0xea7450)
 #2 0x00007efdaf705f3b (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.1+0xea9f3b)
 #3 0x00007efdae334520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007efdb02d9c10 llvm::GetIfCondition(llvm::BasicBlock*, llvm::BasicBlock*&, llvm::BasicBlock*&) (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.1+0x1a7dc10)
 #5 0x00007efdb03e98e9 (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.1+0x1b8d8e9)
 #6 0x00007efdb03e5933 (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.1+0x1b89933)
 #7 0x00007efdb03e4630 llvm::simplifyCFG(llvm::BasicBlock*, llvm::TargetTransformInfo const&, llvm::DomTreeUpdater*, llvm::SimplifyCFGOptions const&, llvm::ArrayRef<llvm::WeakVH>) (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.1+0x1b88630)
 #8 0x00007efdb07e6fef (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.1+0x1f8afef)
 #9 0x00007efdb07e6925 (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.1+0x1f8a925)
#10 0x00007efdb07e5862 llvm::SimplifyCFGPass::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.1+0x1f89862)
#11 0x00007efdb0b46bad (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.1+0x22eabad)
#12 0x00007efdaf88c5f9 llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.1+0x10305f9)
#13 0x00007efdb18ebc4d (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.1+0x308fc4d)
#14 0x00007efdaf890234 llvm::ModuleToFunctionPassAdaptor::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.1+0x1034234)
#15 0x00007efdb18eba6d (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.1+0x308fa6d)
#16 0x00007efdaf88b3f9 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.1+0x102f3f9)
#17 0x00007efdb825b711 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1e49711)
#18 0x00007efdb8253f7b clang::EmitBackendOutput(clang::DiagnosticsEngine&, clang::HeaderSearchOptions const&, clang::CodeGenOptions const&, clang::TargetOptions const&, clang::LangOptions const&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1e41f7b)
#19 0x00007efdb85ff417 clang::CodeGenAction::ExecuteAction() (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x21ed417)
#20 0x00007efdb90f8a95 clang::FrontendAction::Execute() (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x2ce6a95)
#21 0x00007efdb9068a34 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x2c56a34)
#22 0x00007efdb9175bee clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x2d63bee)
#23 0x00005606fedfcc25 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/usr/lib/llvm-19/bin/clang+0x11c25)
#24 0x00005606fedf9b35 (/usr/lib/llvm-19/bin/clang+0xeb35)
#25 0x00005606fedf8cfb clang_main(int, char**, llvm::ToolContext const&) (/usr/lib/llvm-19/bin/clang+0xdcfb)
#26 0x00005606fee06f96 main (/usr/lib/llvm-19/bin/clang+0x1bf96)
#27 0x00007efdae31bd90 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:58:16
#28 0x00007efdae31be40 call_init ./csu/../csu/libc-start.c:128:20
#29 0x00007efdae31be40 __libc_start_main ./csu/../csu/libc-start.c:379:5
#30 0x00005606fedf77c5 _start (/usr/lib/llvm-19/bin/clang+0xc7c5)
clang-19: error: unable to execute command: Segmentation fault (core dumped)
clang-19: error: clang frontend command failed due to signal (use -v to see invocation)
Ubuntu clang version 19.1.7 (++20250114103320+cd708029e0b2-1~exp1~20250114103432.75)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/lib/llvm-19/bin
clang-19: note: diagnostic msg: Error generating preprocessed source(s) - no preprocessable inputs.
```
Thanks.

## Comments

### Comment 1 - connorl309

This is the full generated IR:
```
; ModuleID = 'module'
source_filename = "module"

@outstr = private unnamed_addr constant [25 x i8] c"Main.main() returned %d\0A\00", align 1

declare i32 @strcmp(ptr, ptr)

declare i32 @printf(ptr, ...)

declare void @abort()

declare ptr @malloc(i32)

define i32 @Main_main() {
entry_block:
  %m = alloca i32, align 4
  %n = alloca i32, align 4
  store i32 0, ptr %n, align 4
  store i32 14, ptr %n, align 4
  store i32 0, ptr %m, align 4
  %n_deref_obj = load i32, ptr %n, align 4
  %"lhs < rhs" = icmp slt i32 %n_deref_obj, 8
  br i1 %"lhs < rhs", label %execute_if, label %execute_else

finished_conditional:                             ; preds = %cont, %execute_if
  %0 = phi i32 [ %1, %execute_if ], [ %2, %div ]
  %m_deref_obj = load i32, ptr %m, align 4
  br i1 false, label %err4, label %div3

execute_else:                                     ; preds = %entry_block
  %n_deref_obj2 = load i32, ptr %n, align 4
  br i1 false, label %err, label %div

execute_if:                                       ; preds = %entry_block
  %n_deref_obj1 = load i32, ptr %n, align 4
  %1 = mul i32 %n_deref_obj1, 2
  store i32 %1, ptr %m, align 4
  br label %finished_conditional

div:                                              ; preds = %execute_else
  %2 = sdiv exact i32 %n_deref_obj2, 2
  br label %cont

err:                                              ; preds = %execute_else
  call void @abort()
  unreachable

cont:                                             ; preds = %div
  store i32 %2, ptr %m, align 4
  br label %finished_conditional

div3:                                             ; preds = %finished_conditional
  %3 = sdiv exact i32 %m_deref_obj, 2
  br label %cont5

err4:                                             ; preds = %finished_conditional
  call void @abort()
  unreachable

cont5:                                            ; preds = %div3
  ret i32 %3
}

define i32 @main() {
entry_block:
  %0 = call i32 @Main_main()
  %1 = call i32 (ptr, ...) @printf(ptr @outstr, i32 %0)
  ret i32 0
}
```

---

### Comment 2 - abhishek-kaushik22

What's the intended behavior here? I think clang is correct to report a fatal error and abort compilation.

```
fatal error: error in backend: Broken module found, compilation aborted!
```

---

