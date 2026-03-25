# [LLVM] Crash in upgradeAVX512MaskToSelect - "Unexpected intrinsic"

**Author:** sairam2661
**URL:** https://github.com/llvm/llvm-project/issues/176675
**Status:** Open
**Labels:** crash, llvm:ir

## Body

**Fuzzer Generated Test**
**Test Commit**
[0b2f3cfb72a76fa90f3ec2a234caabe0d0712590](https://github.com/llvm/llvm-project/commit/0b2f3cfb72a76fa90f3ec2a234caabe0d0712590)

**Reproducer**
https://godbolt.org/z/c4fKbfjhe

**Description**
The `AVX512` intrinsic upgrade logic crashes with "Unexpected intrinsic" when `llvm.x86.avx512.mask.packssdw.512` is called with invalid types.

**Steps to reproduce**
- Minimized test case, `input.ll`
```
define <4 x ptr> @test() {
  %v = call <4 x ptr> @llvm.x86.avx512.mask.packssdw.512(<4 x i32> zeroinitializer, <4 x i32> zeroinitializer, <4 x i32> zeroinitializer)
  ret <4 x ptr> %v
}
```

**Command**
```
opt -passes=verify -S input.ll
```

**Output**
```
Unexpected intrinsic
UNREACHABLE executed at /workdir/llvm-project/llvm/lib/IR/AutoUpgrade.cpp:2471!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: opt -passes=verify -S input.ll
 #0 0x00007fc724724728 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/workdir/llvm-project/build-libcxx/bin/../lib/libLLVMSupport.so.22.0git+0x1d3728)
 #1 0x00007fc7247219d5 llvm::sys::RunSignalHandlers() (/workdir/llvm-project/build-libcxx/bin/../lib/libLLVMSupport.so.22.0git+0x1d09d5)
 #2 0x00007fc7247253c6 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x00007fc72e84d420 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x14420)
 #4 0x00007fc72423800b raise /build/glibc-B3wQXB/glibc-2.31/signal/../sysdeps/unix/sysv/linux/raise.c:51:1
 #5 0x00007fc724217859 abort /build/glibc-B3wQXB/glibc-2.31/stdlib/abort.c:81:7
 #6 0x00007fc72464b0ff (/workdir/llvm-project/build-libcxx/bin/../lib/libLLVMSupport.so.22.0git+0xfa0ff)
 #7 0x00007fc7249dad42 upgradeAVX512MaskToSelect(llvm::StringRef, llvm::IRBuilder<llvm::ConstantFolder, llvm::IRBuilderDefaultInserter>&, llvm::CallBase&, llvm::Value*&) AutoUpgrade.cpp:0:0
 #8 0x00007fc7249c5231 upgradeX86IntrinsicCall(llvm::StringRef, llvm::CallBase*, llvm::Function*, llvm::IRBuilder<llvm::ConstantFolder, llvm::IRBuilderDefaultInserter>&) AutoUpgrade.cpp:0:0
 #9 0x00007fc7249b7b10 llvm::UpgradeIntrinsicCall(llvm::CallBase*, llvm::Function*) (/workdir/llvm-project/build-libcxx/bin/../lib/libLLVMCore.so.22.0git+0xe1b10)
#10 0x00007fc72504e03e llvm::LLParser::validateEndOfModule(bool) (/workdir/llvm-project/build-libcxx/bin/../lib/libLLVMAsmParser.so.22.0git+0x3503e)
#11 0x00007fc72504ce3a llvm::LLParser::Run(bool, llvm::function_ref<std::__1::optional<std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>>> (llvm::StringRef, llvm::StringRef)>) (/workdir/llvm-project/build-libcxx/bin/../lib/libLLVMAsmParser.so.22.0git+0x33e3a)
#12 0x00007fc72509e642 parseAssemblyInto(llvm::MemoryBufferRef, llvm::Module*, llvm::ModuleSummaryIndex*, llvm::SMDiagnostic&, llvm::SlotMapping*, bool, llvm::function_ref<std::__1::optional<std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>>> (llvm::StringRef, llvm::StringRef)>, llvm::AsmParserContext*) Parser.cpp:0:0
#13 0x00007fc72509e802 llvm::parseAssembly(llvm::MemoryBufferRef, llvm::SMDiagnostic&, llvm::LLVMContext&, llvm::SlotMapping*, llvm::function_ref<std::__1::optional<std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>>> (llvm::StringRef, llvm::StringRef)>, llvm::AsmParserContext*) (/workdir/llvm-project/build-libcxx/bin/../lib/libLLVMAsmParser.so.22.0git+0x85802)
#14 0x00007fc7250d63b8 llvm::parseIR(llvm::MemoryBufferRef, llvm::SMDiagnostic&, llvm::LLVMContext&, llvm::ParserCallbacks, llvm::AsmParserContext*) (/workdir/llvm-project/build-libcxx/bin/../lib/libLLVMIRReader.so.22.0git+0x33b8)
#15 0x00007fc7250d6cdf llvm::parseIRFile(llvm::StringRef, llvm::SMDiagnostic&, llvm::LLVMContext&, llvm::ParserCallbacks, llvm::AsmParserContext*) (/workdir/llvm-project/build-libcxx/bin/../lib/libLLVMIRReader.so.22.0git+0x3cdf)
#16 0x00007fc72e81236e optMain (/workdir/llvm-project/build-libcxx/bin/../lib/libLLVMOptDriver.so.22.0git+0x3636e)
#17 0x00007fc724219083 __libc_start_main /build/glibc-B3wQXB/glibc-2.31/csu/../csu/libc-start.c:342:3
#18 0x000055e2bbf8807e _start (/workdir/llvm-project/build-libcxx/bin/opt+0x207e)
```




## Comments

### Comment 1 - RKSimon

AutoUpgrade.cpp uses llvm_unreachable a lot when maybe we should more consistently use reportFatalUsageError for intrinsic mismatches

---

