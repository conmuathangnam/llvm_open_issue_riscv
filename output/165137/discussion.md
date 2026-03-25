# opt (assertions trunk) crashes on llvm/test/Bitcode/constexpr-scalable-vector-gep.ll

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/165137
**Status:** Closed
**Labels:** llvm:asmparser, crash
**Closed Date:** 2025-10-29T13:08:41Z

## Body

Reproducer:
https://godbolt.org/z/b1Ksooaan

Backtrace:
```console
opt: /root/llvm-project/llvm/lib/IR/Constants.cpp:2522: static llvm::Constant* llvm::ConstantExpr::getGetElementPtr(llvm::Type*, llvm::Constant*, llvm::ArrayRef<llvm::Value*>, llvm::GEPNoWrapFlags, std::optional<llvm::ConstantRange>, llvm::Type*): Assertion `isSupportedGetElementPtr(Ty) && "Element type is unsupported!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/opt -o /app/output.s -S <source>
 #0 0x0000000005974be8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5974be8)
 #1 0x0000000005971a94 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x000072ce03a42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x000072ce03a969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x000072ce03a42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x000072ce03a287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x000072ce03a2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #7 0x000072ce03a39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #8 0x00000000055ce23f llvm::ConstantExpr::getGetElementPtr(llvm::Type*, llvm::Constant*, llvm::ArrayRef<llvm::Value*>, llvm::GEPNoWrapFlags, std::optional<llvm::ConstantRange>, llvm::Type*) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x55ce23f)
 #9 0x00000000053501b1 llvm::LLParser::parseValID(llvm::ValID&, llvm::LLParser::PerFunctionState*, llvm::Type*) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x53501b1)
#10 0x00000000053529f2 llvm::LLParser::parseValue(llvm::Type*, llvm::Value*&, llvm::LLParser::PerFunctionState*) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x53529f2)
#11 0x0000000005357ea5 llvm::LLParser::parseRet(llvm::Instruction*&, llvm::BasicBlock*, llvm::LLParser::PerFunctionState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5357ea5)
#12 0x0000000005373b91 llvm::LLParser::parseInstruction(llvm::Instruction*&, llvm::BasicBlock*, llvm::LLParser::PerFunctionState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5373b91)
#13 0x0000000005374f35 llvm::LLParser::parseBasicBlock(llvm::LLParser::PerFunctionState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5374f35)
#14 0x00000000053756a8 llvm::LLParser::parseFunctionBody(llvm::Function&, unsigned int, llvm::ArrayRef<unsigned int>) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x53756a8)
#15 0x00000000053758d6 llvm::LLParser::parseDefine() (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x53758d6)
#16 0x00000000053762d8 llvm::LLParser::parseTopLevelEntities() (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x53762d8)
#17 0x000000000537634d llvm::LLParser::Run(bool, llvm::function_ref<std::optional<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>> (llvm::StringRef, llvm::StringRef)>) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x537634d)
#18 0x000000000531f547 parseAssemblyInto(llvm::MemoryBufferRef, llvm::Module*, llvm::ModuleSummaryIndex*, llvm::SMDiagnostic&, llvm::SlotMapping*, bool, llvm::function_ref<std::optional<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>> (llvm::StringRef, llvm::StringRef)>, llvm::AsmParserContext*) Parser.cpp:0:0
#19 0x000000000531f6f1 llvm::parseAssembly(llvm::MemoryBufferRef, llvm::SMDiagnostic&, llvm::LLVMContext&, llvm::SlotMapping*, llvm::function_ref<std::optional<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>> (llvm::StringRef, llvm::StringRef)>, llvm::AsmParserContext*) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x531f6f1)
#20 0x0000000005319ef1 llvm::parseIR(llvm::MemoryBufferRef, llvm::SMDiagnostic&, llvm::LLVMContext&, llvm::ParserCallbacks, llvm::AsmParserContext*) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5319ef1)
#21 0x000000000531c114 llvm::parseIRFile(llvm::StringRef, llvm::SMDiagnostic&, llvm::LLVMContext&, llvm::ParserCallbacks, llvm::AsmParserContext*) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x531c114)
#22 0x0000000000977f29 optMain (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x977f29)
#23 0x000072ce03a29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#24 0x000072ce03a29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#25 0x00000000009712c5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x9712c5)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

## Comments

### Comment 1 - Rajveer100

Looks like it only happens with `vscale`, fixed size works.

---

### Comment 2 - Rajveer100

@paulwalker-arm 
Is this intended?

```c++
  /// Whether creating a constant expression for this getelementptr type is
  /// supported.
  static bool isSupportedGetElementPtr(const Type *SrcElemTy) {
    return !SrcElemTy->isScalableTy();
  }
```

Meaning, should scalable types fail, this did not fail till llvm `21.0.0`

---

### Comment 3 - nikic

The IR should be rejected, but with a parser error rather than an assertion failure.

---

### Comment 4 - paulwalker-arm

Yes, this is intentional.  It was done in response to a comment on https://reviews.llvm.org/D134648 because vscale is not strictly speaking a constant.

---

### Comment 5 - paulwalker-arm

> The IR should be rejected, but with a parser error rather than an assertion failure.

I'll create a PR to fix the parsing.

---

