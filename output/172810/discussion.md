# opt crashes in llvm-project/llvm/lib/IR/Instructions.cpp:762

**Author:** Emilyaxe
**URL:** https://github.com/llvm/llvm-project/issues/172810
**Status:** Open
**Labels:** crash, llvm

## Body

When running opt on this following IR, opt crashes. 
This may be an invalid test case; if using opt in this way is not appropriate, I am open to closing this issue.

llvm version 3186ca2

test.ll

```
@g_obj = external global [20 x i8], align 1
declare i32 @llvm.objectsize.i32(ptr, i1) nounwind readonly

define i32 @f() {
entry:
  %0 = load i32, ptr @g_obj, align 1
  %conv = zext nneg i32 1 to i64
  %eq = icmp ne i32 poison, %0
  %lcssa = select i1 %eq, i64 %conv, i64 %conv
  %conv.1.i = trunc i64 %lcssa to i32
  tail call void @llvm.objectsize.i32(i32 %conv.1.i, i32 0, i64 1)
  ret i32 %0
}

```


commads
./bin/opt test.ll  -S


stacktrace

```
opt: llvm-project/llvm/lib/IR/Instructions.cpp:762: void llvm::CallInst::init(llvm::FunctionType*, llvm::Value*, llvm::ArrayRef<llvm::Value*>, llvm::ArrayRef<llvm::OperandBundleDefT<llvm::Value*> >, const llvm::Twine&): Assertion `(i >= FTy->getNumParams() || FTy->getParamType(i) == Args[i]->getType()) && "Calling a function with a bad signature!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: ../llvm-dir/llvm-release/llvm-project/build/bin/opt test.ll -S
 #0 0x000055eff9d5a59f llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (../llvm-dir/llvm-release/llvm-project/build/bin/opt+0x560559f)
 #1 0x000055eff9d5720c llvm::sys::RunSignalHandlers() (../llvm-dir/llvm-release/llvm-project/build/bin/opt+0x560220c)
 #2 0x000055eff9d573a7 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x00007f0fc7805420 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x14420)
 #4 0x00007f0fc6e5200b raise /build/glibc-LcI20x/glibc-2.31/signal/../sysdeps/unix/sysv/linux/raise.c:51:1
 #5 0x00007f0fc6e31859 abort /build/glibc-LcI20x/glibc-2.31/stdlib/abort.c:81:7
 #6 0x00007f0fc6e31729 get_sysdep_segment_value /build/glibc-LcI20x/glibc-2.31/intl/loadmsgcat.c:509:8
 #7 0x00007f0fc6e31729 _nl_load_domain /build/glibc-LcI20x/glibc-2.31/intl/loadmsgcat.c:970:34
 #8 0x00007f0fc6e42fd6 (/lib/x86_64-linux-gnu/libc.so.6+0x33fd6)
 #9 0x000055eff9a8895f llvm::CallInst::init(llvm::FunctionType*, llvm::Value*, llvm::ArrayRef<llvm::Value*>, llvm::ArrayRef<llvm::OperandBundleDefT<llvm::Value*>>, llvm::Twine const&) (../llvm-dir/llvm-release/llvm-project/build/bin/opt+0x533395f)
#10 0x000055eff521f1d8 llvm::IRBuilderBase::CreateCall(llvm::FunctionType*, llvm::Value*, llvm::ArrayRef<llvm::Value*>, llvm::Twine const&, llvm::MDNode*) (../llvm-dir/llvm-release/llvm-project/build/bin/opt+0xaca1d8)
#11 0x000055eff992fc33 llvm::UpgradeIntrinsicCall(llvm::CallBase*, llvm::Function*) (../llvm-dir/llvm-release/llvm-project/build/bin/opt+0x51dac33)
#12 0x000055eff9947e3a llvm::UpgradeCallsToIntrinsic(llvm::Function*) (../llvm-dir/llvm-release/llvm-project/build/bin/opt+0x51f2e3a)
#13 0x000055eff96f7776 llvm::LLParser::validateEndOfModule(bool) (../llvm-dir/llvm-release/llvm-project/build/bin/opt+0x4fa2776)
#14 0x000055eff973220b llvm::LLParser::Run(bool, llvm::function_ref<std::optional<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>> (llvm::StringRef, llvm::StringRef)>) (../llvm-dir/llvm-release/llvm-project/build/bin/opt+0x4fdd20b)
#15 0x000055eff96d7839 parseAssemblyInto(llvm::MemoryBufferRef, llvm::Module*, llvm::ModuleSummaryIndex*, llvm::SMDiagnostic&, llvm::SlotMapping*, bool, llvm::function_ref<std::optional<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>> (llvm::StringRef, llvm::StringRef)>, llvm::AsmParserContext*) Parser.cpp:0:0
#16 0x000055eff96d7a05 llvm::parseAssembly(llvm::MemoryBufferRef, llvm::SMDiagnostic&, llvm::LLVMContext&, llvm::SlotMapping*, llvm::function_ref<std::optional<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>> (llvm::StringRef, llvm::StringRef)>, llvm::AsmParserContext*) (../llvm-dir/llvm-release/llvm-project/build/bin/opt+0x4f82a05)
#17 0x000055eff96d2531 llvm::parseIR(llvm::MemoryBufferRef, llvm::SMDiagnostic&, llvm::LLVMContext&, llvm::ParserCallbacks, llvm::AsmParserContext*) (../llvm-dir/llvm-release/llvm-project/build/bin/opt+0x4f7d531)
#18 0x000055eff96d4906 llvm::parseIRFile(llvm::StringRef, llvm::SMDiagnostic&, llvm::LLVMContext&, llvm::ParserCallbacks, llvm::AsmParserContext*) (../llvm-dir/llvm-release/llvm-project/build/bin/opt+0x4f7f906)
#19 0x000055eff4f96222 optMain (../llvm-dir/llvm-release/llvm-project/build/bin/opt+0x841222)
#20 0x00007f0fc6e33083 __libc_start_main /build/glibc-LcI20x/glibc-2.31/csu/../csu/libc-start.c:342:3
#21 0x000055eff4f8eeae _start (../llvm-dir/llvm-release/llvm-project/build/bin/opt+0x839eae)
```


## Comments

### Comment 1 - kper

hmmm
it seems like it is crashing because the first operand of `@llvm.objectsize.i32` isn't a `ptr`

---

