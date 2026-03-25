# opt crashes  in llvm/include/llvm/ADT/APInt.h:127

**Author:** Emilyaxe
**URL:** https://github.com/llvm/llvm-project/issues/172650
**Status:** Closed
**Labels:** llvm:asmparser, crash
**Closed Date:** 2025-12-23T12:37:18Z

## Body

When running opt on this following IR

llvm version 3186ca2

test.ll

```
; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare half @llvm.fabs.f16(half) #0

define half @test_inbounds_f16() {
  %1 = call half @llvm.fabs.f16(half 0xH5F2F00)
  ret half %1
}
attributes #0 = { nocallback nofree nosync nounwind speculatable willreturn memory(none) }
```


commads
./bin/opt test.ll  -S


stacktrace

```
opt: llvm-release/llvm-project/llvm/include/llvm/ADT/APInt.h:127: llvm::APInt::APInt(unsigned int, uint64_t, bool, bool): Assertion `llvm::isUIntN(BitWidth, val) && "Value is not an N-bit unsigned value"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: ./bin/opt test.ll
 #0 0x00005624827e359f llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (./bin/opt+0x560559f)
 #1 0x00005624827e020c llvm::sys::RunSignalHandlers() (./bin/opt+0x560220c)
 #2 0x00005624827e03a7 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x00007f26c6daf420 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x14420)
 #4 0x00007f26c63fc00b raise /build/glibc-LcI20x/glibc-2.31/signal/../sysdeps/unix/sysv/linux/raise.c:51:1
 #5 0x00007f26c63db859 abort /build/glibc-LcI20x/glibc-2.31/stdlib/abort.c:81:7
 #6 0x00007f26c63db729 get_sysdep_segment_value /build/glibc-LcI20x/glibc-2.31/intl/loadmsgcat.c:509:8
 #7 0x00007f26c63db729 _nl_load_domain /build/glibc-LcI20x/glibc-2.31/intl/loadmsgcat.c:970:34
 #8 0x00007f26c63ecfd6 (/lib/x86_64-linux-gnu/libc.so.6+0x33fd6)
 #9 0x0000562482165452 llvm::LLLexer::Lex0x() (./bin/opt+0x4f87452)
#10 0x000056248216bd80 llvm::LLLexer::LexDigitOrNegative() (./bin/opt+0x4f8dd80)
#11 0x0000562482189c1b llvm::LLParser::parseType(llvm::Type*&, llvm::Twine const&, bool) (./bin/opt+0x4fabc1b)
#12 0x00005624821ad276 llvm::LLParser::parseParameterList(llvm::SmallVectorImpl<llvm::LLParser::ParamInfo>&, llvm::LLParser::PerFunctionState&, bool, bool) (.part.0) LLParser.cpp:0:0
#13 0x00005624821b8615 llvm::LLParser::parseCall(llvm::Instruction*&, llvm::LLParser::PerFunctionState&, llvm::CallInst::TailCallKind) (./bin/opt+0x4fda615)
#14 0x00005624821b9250 llvm::LLParser::parseInstruction(llvm::Instruction*&, llvm::BasicBlock*, llvm::LLParser::PerFunctionState&) (./bin/opt+0x4fdb250)
#15 0x00005624821b9c45 llvm::LLParser::parseBasicBlock(llvm::LLParser::PerFunctionState&) (./bin/opt+0x4fdbc45)
#16 0x00005624821ba3d8 llvm::LLParser::parseFunctionBody(llvm::Function&, unsigned int, llvm::ArrayRef<unsigned int>) (./bin/opt+0x4fdc3d8)
#17 0x00005624821ba626 llvm::LLParser::parseDefine() (./bin/opt+0x4fdc626)
#18 0x00005624821bb0e8 llvm::LLParser::parseTopLevelEntities() (./bin/opt+0x4fdd0e8)
#19 0x00005624821bb191 llvm::LLParser::Run(bool, llvm::function_ref<std::optional<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>> (llvm::StringRef, llvm::StringRef)>) (./bin/opt+0x4fdd191)
#20 0x0000562482160839 parseAssemblyInto(llvm::MemoryBufferRef, llvm::Module*, llvm::ModuleSummaryIndex*, llvm::SMDiagnostic&, llvm::SlotMapping*, bool, llvm::function_ref<std::optional<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>> (llvm::StringRef, llvm::StringRef)>, llvm::AsmParserContext*) Parser.cpp:0:0
#21 0x0000562482160a05 llvm::parseAssembly(llvm::MemoryBufferRef, llvm::SMDiagnostic&, llvm::LLVMContext&, llvm::SlotMapping*, llvm::function_ref<std::optional<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>> (llvm::StringRef, llvm::StringRef)>, llvm::AsmParserContext*) (./bin/opt+0x4f82a05)
#22 0x000056248215b531 llvm::parseIR(llvm::MemoryBufferRef, llvm::SMDiagnostic&, llvm::LLVMContext&, llvm::ParserCallbacks, llvm::AsmParserContext*) (./bin/opt+0x4f7d531)
#23 0x000056248215d906 llvm::parseIRFile(llvm::StringRef, llvm::SMDiagnostic&, llvm::LLVMContext&, llvm::ParserCallbacks, llvm::AsmParserContext*) (./bin/opt+0x4f7f906)
#24 0x000056247da1f222 optMain (./bin/opt+0x841222)
#25 0x00007f26c63dd083 __libc_start_main /build/glibc-LcI20x/glibc-2.31/csu/../csu/libc-start.c:342:3
#26 0x000056247da17eae _start (./bin/opt+0x839eae)

```


## Comments

### Comment 1 - milos1397

I can see that issue here is that 0xH5F2F00 doesn't fit in 16bits. I guess we want more informative message? 
You can assign it to me.

---

