# opt crashes in llvm/lib/IR/Value.cpp:519

**Author:** Emilyaxe
**URL:** https://github.com/llvm/llvm-project/issues/172806
**Status:** Open
**Labels:** crash, llvm

## Body

When running opt on this following IR, opt crashes. 
I am not sure whether this is a valid test case, and I am appreciate for any clarification.

llvm version 3186ca2

test.ll

```
@g_obj = external global [20 x i8], align 1
declare i32 @llvm.objectsize.i32(ptr, i1) nounwind readonly
define i32 @f() {
entry:
  %0 = load i32, ptr @g_obj, align 1
  tail call void @llvm.objectsize.i32(ptr @g_obj, i1 true)
  ret i32 %0
}

```


commads
./bin/opt test.ll  -S


stacktrace

```
opt: llvm-release/llvm-project/llvm/lib/IR/Value.cpp:519: void llvm::Value::doRAUW(llvm::Value*, llvm::Value::ReplaceMetadataUses): Assertion `New->getType() == getType() && "replaceAllUses of value with new value of different type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: ../llvm-dir/llvm-release/llvm-project/build/bin/opt test.ll -S
 #0 0x000055616601059f llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (../llvm-dir/llvm-release/llvm-project/build/bin/opt+0x560559f)
 #1 0x000055616600d20c llvm::sys::RunSignalHandlers() (../llvm-dir/llvm-release/llvm-project/build/bin/opt+0x560220c)
 #2 0x000055616600d3a7 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x00007f2b39c5d420 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x14420)
 #4 0x00007f2b392aa00b raise /build/glibc-LcI20x/glibc-2.31/signal/../sysdeps/unix/sysv/linux/raise.c:51:1
 #5 0x00007f2b39289859 abort /build/glibc-LcI20x/glibc-2.31/stdlib/abort.c:81:7
 #6 0x00007f2b39289729 get_sysdep_segment_value /build/glibc-LcI20x/glibc-2.31/intl/loadmsgcat.c:509:8
 #7 0x00007f2b39289729 _nl_load_domain /build/glibc-LcI20x/glibc-2.31/intl/loadmsgcat.c:970:34
 #8 0x00007f2b3929afd6 (/lib/x86_64-linux-gnu/libc.so.6+0x33fd6)
 #9 0x0000556165e148f2 (../llvm-dir/llvm-release/llvm-project/build/bin/opt+0x54098f2)
#10 0x0000556165be5c61 llvm::UpgradeIntrinsicCall(llvm::CallBase*, llvm::Function*) (../llvm-dir/llvm-release/llvm-project/build/bin/opt+0x51dac61)
#11 0x0000556165bfde3a llvm::UpgradeCallsToIntrinsic(llvm::Function*) (../llvm-dir/llvm-release/llvm-project/build/bin/opt+0x51f2e3a)
#12 0x00005561659ad776 llvm::LLParser::validateEndOfModule(bool) (../llvm-dir/llvm-release/llvm-project/build/bin/opt+0x4fa2776)
#13 0x00005561659e820b llvm::LLParser::Run(bool, llvm::function_ref<std::optional<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>> (llvm::StringRef, llvm::StringRef)>) (../llvm-dir/llvm-release/llvm-project/build/bin/opt+0x4fdd20b)
#14 0x000055616598d839 parseAssemblyInto(llvm::MemoryBufferRef, llvm::Module*, llvm::ModuleSummaryIndex*, llvm::SMDiagnostic&, llvm::SlotMapping*, bool, llvm::function_ref<std::optional<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>> (llvm::StringRef, llvm::StringRef)>, llvm::AsmParserContext*) Parser.cpp:0:0
#15 0x000055616598da05 llvm::parseAssembly(llvm::MemoryBufferRef, llvm::SMDiagnostic&, llvm::LLVMContext&, llvm::SlotMapping*, llvm::function_ref<std::optional<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>> (llvm::StringRef, llvm::StringRef)>, llvm::AsmParserContext*) (../llvm-dir/llvm-release/llvm-project/build/bin/opt+0x4f82a05)
#16 0x0000556165988531 llvm::parseIR(llvm::MemoryBufferRef, llvm::SMDiagnostic&, llvm::LLVMContext&, llvm::ParserCallbacks, llvm::AsmParserContext*) (../llvm-dir/llvm-release/llvm-project/build/bin/opt+0x4f7d531)
#17 0x000055616598a906 llvm::parseIRFile(llvm::StringRef, llvm::SMDiagnostic&, llvm::LLVMContext&, llvm::ParserCallbacks, llvm::AsmParserContext*) (../llvm-dir/llvm-release/llvm-project/build/bin/opt+0x4f7f906)
#18 0x000055616124c222 optMain (../llvm-dir/llvm-release/llvm-project/build/bin/opt+0x841222)
#19 0x00007f2b3928b083 __libc_start_main /build/glibc-LcI20x/glibc-2.31/csu/../csu/libc-start.c:342:3
#20 0x0000556161244eae _start (../llvm-dir/llvm-release/llvm-project/build/bin/opt+0x839eae)
```


## Comments

### Comment 1 - fhahn

>  I am not sure whether this is a valid test case, and I am appreciate for any clarification.

I think so, `opt` should no crash in invalid IR, it should produce an error message

---

### Comment 2 - kper

The problem is the return type of `tail call void @llvm.objectsize.i32(ptr @g_obj, i1 true)`
If I change it to `tail call i32 @llvm.objectsize.i32(ptr @g_obj, i1 true)`, it works.

@fhahn could you tell me where such a check should be implemented? Or do you suggest to replace the assertion by a more user-friendly check?

---

### Comment 3 - nikic

Generally auto-upgrade isn't prepared to deal with cases where the original IR was already malformed. This specific case would be somewhat simple to address with a generic check prior to RAUW, but for the more general case (https://github.com/llvm/llvm-project/issues/172810) you'd probably want to change all the Builder.CreateCall() calls with a helper that does more validation and uses reportFatalUsageError() on violations. I'm not sure it's worth spending time on this.

---

