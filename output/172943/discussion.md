# opt crashes in llvm/lib/IR/Metadata.cpp:509

**Author:** Emilyaxe
**URL:** https://github.com/llvm/llvm-project/issues/172943
**Status:** Open
**Labels:** crash, llvm

## Body

When running opt on the following IR, opt crashes. 
The metadata `#dbg_declare` may be invalid.  If using opt in this way is not appropriate, I am open to closing this issue.
godbolt: https://godbolt.org/z/nd3rPa545

llvm version 50ae726bb3498

test.ll

```
define i1 @test1(i1 %C, i1 %A, i1 %B) {
entry:
  br i1 %C, label %yes, label %no3
yes:                                              
  ret i1 false
no3: 
  #dbg_declare(label %dest3,!7,!DIExpression(),!11)
  br i1 %A, label %yes, label %no3

}

```


commads
./bin/opt test.ll  -S


stacktrace

```
opt: /root/llvm-project/llvm/lib/IR/Metadata.cpp:509: static llvm::ValueAsMetadata* llvm::ValueAsMetadata::get(llvm::Value*): Assertion `(isa<Constant>(V) || isa<Argument>(V) || isa<Instruction>(V)) && "Expected constant or function-local value"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/opt -o /app/output.s -S -S <source>
 #0 0x00000000059aeec8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x59aeec8)
 #1 0x00000000059abd74 llvm::sys::RunSignalHandlers() (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x59abd74)
 #2 0x00000000059abeea SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x000070d259842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x000070d2598969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x000070d259842476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x000070d2598287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x000070d25982871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x000070d259839e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000005752cc8 (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5752cc8)
#10 0x0000000005398d2b llvm::LLParser::parseMetadata(llvm::Metadata*&, llvm::LLParser::PerFunctionState*) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5398d2b)
#11 0x00000000053a869a llvm::LLParser::parseDebugRecord(llvm::DbgRecord*&, llvm::LLParser::PerFunctionState&) (.part.0) LLParser.cpp:0:0
#12 0x00000000053b56d1 llvm::LLParser::parseBasicBlock(llvm::LLParser::PerFunctionState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x53b56d1)
#13 0x00000000053b5bf8 llvm::LLParser::parseFunctionBody(llvm::Function&, unsigned int, llvm::ArrayRef<unsigned int>) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x53b5bf8)
#14 0x00000000053b5e26 llvm::LLParser::parseDefine() (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x53b5e26)
#15 0x00000000053b6828 llvm::LLParser::parseTopLevelEntities() (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x53b6828)
#16 0x00000000053b689d llvm::LLParser::Run(bool, llvm::function_ref<std::optional<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>> (llvm::StringRef, llvm::StringRef)>) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x53b689d)
#17 0x000000000535f727 parseAssemblyInto(llvm::MemoryBufferRef, llvm::Module*, llvm::ModuleSummaryIndex*, llvm::SMDiagnostic&, llvm::SlotMapping*, bool, llvm::function_ref<std::optional<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>> (llvm::StringRef, llvm::StringRef)>, llvm::AsmParserContext*) Parser.cpp:0:0
#18 0x000000000535f8d1 llvm::parseAssembly(llvm::MemoryBufferRef, llvm::SMDiagnostic&, llvm::LLVMContext&, llvm::SlotMapping*, llvm::function_ref<std::optional<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>> (llvm::StringRef, llvm::StringRef)>, llvm::AsmParserContext*) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x535f8d1)
#19 0x000000000535a761 llvm::parseIR(llvm::MemoryBufferRef, llvm::SMDiagnostic&, llvm::LLVMContext&, llvm::ParserCallbacks, llvm::AsmParserContext*) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x535a761)
#20 0x000000000535c984 llvm::parseIRFile(llvm::StringRef, llvm::SMDiagnostic&, llvm::LLVMContext&, llvm::ParserCallbacks, llvm::AsmParserContext*) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x535c984)
#21 0x000000000096eb03 optMain (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x96eb03)
#22 0x000070d259829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#23 0x000070d259829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#24 0x0000000000967b25 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x967b25)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```


