# Parser crashes on AArch64 NEON intrinsics with mismatched element types

**Author:** sairam2661
**URL:** https://github.com/llvm/llvm-project/issues/176847
**Status:** Open
**Labels:** crash, llvm

## Body

**Fuzzer Generated Test**
**Reproducer**
1. https://godbolt.org/z/K16K7avc7
2. https://godbolt.org/z/b4n79TTbz

**Test Commit**
[0b2f3cfb72a76fa90f3ec2a234caabe0d0712590](https://github.com/llvm/llvm-project/commit/0b2f3cfb72a76fa90f3ec2a234caabe0d0712590)

**Description**
Parser crashes in intrinsic signature matching when AArch64 NEON intrinsics (`raddhn`, `sqdmull`) are called with floating-point vectors instead of required integer vector types.

**Steps to reproduce 1**
- Minimized test case, `input1.ll`
```
target triple = "aarch64-unknown-linux-gnu"

define <vscale x 4 x float> @test(<vscale x 4 x float> %arg0) {
start:
  %i = call <vscale x 4 x float> @llvm.aarch64.neon.raddhn.v4f32(<vscale x 4 x float> %arg0, i32 0)
  ret <vscale x 4 x float> %i
}
```

**Output**
```
opt: /root/llvm-project/llvm/include/llvm/IR/DerivedTypes.h:492: static llvm::VectorType* llvm::VectorType::getExtendedElementVectorType(llvm::VectorType*): Assertion `VTy->isIntOrIntVectorTy() && "VTy expected to be a vector of ints."' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/opt -o /app/output.s -S --passes=default<O2> <source>
 #0 0x0000000005a80c68 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5a80c68)
 #1 0x0000000005a7db44 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x000077c9a3442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x000077c9a34969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x000077c9a3442476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x000077c9a34287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x000077c9a342871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #7 0x000077c9a3439e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #8 0x000000000573a42c matchIntrinsicType(llvm::Type*, llvm::ArrayRef<llvm::Intrinsic::IITDescriptor>&, llvm::SmallVectorImpl<llvm::Type*>&, llvm::SmallVectorImpl<std::pair<llvm::Type*, llvm::ArrayRef<llvm::Intrinsic::IITDescriptor>>>&, bool) (.part.0) Intrinsics.cpp:0:0
 #9 0x000000000573a77f llvm::Intrinsic::getIntrinsicSignature(unsigned int, llvm::FunctionType*, llvm::SmallVectorImpl<llvm::Type*>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x573a77f)
#10 0x0000000005406f5c llvm::LLParser::validateEndOfModule(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5406f5c)
#11 0x000000000543f21b llvm::LLParser::Run(bool, llvm::function_ref<std::optional<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>> (llvm::StringRef, llvm::StringRef)>) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x543f21b)
#12 0x00000000053e7f37 parseAssemblyInto(llvm::MemoryBufferRef, llvm::Module*, llvm::ModuleSummaryIndex*, llvm::SMDiagnostic&, llvm::SlotMapping*, bool, llvm::function_ref<std::optional<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>> (llvm::StringRef, llvm::StringRef)>, llvm::AsmParserContext*) Parser.cpp:0:0
#13 0x00000000053e80e1 llvm::parseAssembly(llvm::MemoryBufferRef, llvm::SMDiagnostic&, llvm::LLVMContext&, llvm::SlotMapping*, llvm::function_ref<std::optional<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>> (llvm::StringRef, llvm::StringRef)>, llvm::AsmParserContext*) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x53e80e1)
#14 0x00000000053e2f71 llvm::parseIR(llvm::MemoryBufferRef, llvm::SMDiagnostic&, llvm::LLVMContext&, llvm::ParserCallbacks, llvm::AsmParserContext*) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x53e2f71)
#15 0x00000000053e5194 llvm::parseIRFile(llvm::StringRef, llvm::SMDiagnostic&, llvm::LLVMContext&, llvm::ParserCallbacks, llvm::AsmParserContext*) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x53e5194)
#16 0x0000000000960443 optMain (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x960443)
#17 0x000077c9a3429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#18 0x000077c9a3429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#19 0x00000000009595f5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x9595f5)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

**Steps to reproduce 2**
- Minimized test case, `input2.ll`
```
target triple = "aarch64-apple-macosx"

define <8 x half> @test(<8 x half> %arg0, <8 x half> %arg1) {
bb:
  %i5 = call <8 x half> @llvm.aarch64.neon.sqdmull.v8f16(<8 x half> %arg0, <8 x half> %arg1)
  ret <8 x half> %i5
}
```

**Output**
```
Cannot create narrower fp vector element type
UNREACHABLE executed at /root/llvm-project/llvm/include/llvm/IR/DerivedTypes.h:511!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/opt -o /app/output.s -S --passes=default<O2> <source>
 #0 0x0000000005a80c68 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5a80c68)
 #1 0x0000000005a7db44 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x000073f498a42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x000073f498a969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x000073f498a42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x000073f498a287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x00000000059b317a (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x59b317a)
 #7 0x000000000573513c llvm::VectorType::getTruncatedElementVectorType(llvm::VectorType*) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x573513c)
 #8 0x0000000005739c93 matchIntrinsicType(llvm::Type*, llvm::ArrayRef<llvm::Intrinsic::IITDescriptor>&, llvm::SmallVectorImpl<llvm::Type*>&, llvm::SmallVectorImpl<std::pair<llvm::Type*, llvm::ArrayRef<llvm::Intrinsic::IITDescriptor>>>&, bool) (.part.0) Intrinsics.cpp:0:0
 #9 0x000000000573a77f llvm::Intrinsic::getIntrinsicSignature(unsigned int, llvm::FunctionType*, llvm::SmallVectorImpl<llvm::Type*>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x573a77f)
#10 0x0000000005406f5c llvm::LLParser::validateEndOfModule(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5406f5c)
#11 0x000000000543f21b llvm::LLParser::Run(bool, llvm::function_ref<std::optional<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>> (llvm::StringRef, llvm::StringRef)>) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x543f21b)
#12 0x00000000053e7f37 parseAssemblyInto(llvm::MemoryBufferRef, llvm::Module*, llvm::ModuleSummaryIndex*, llvm::SMDiagnostic&, llvm::SlotMapping*, bool, llvm::function_ref<std::optional<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>> (llvm::StringRef, llvm::StringRef)>, llvm::AsmParserContext*) Parser.cpp:0:0
#13 0x00000000053e80e1 llvm::parseAssembly(llvm::MemoryBufferRef, llvm::SMDiagnostic&, llvm::LLVMContext&, llvm::SlotMapping*, llvm::function_ref<std::optional<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>> (llvm::StringRef, llvm::StringRef)>, llvm::AsmParserContext*) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x53e80e1)
#14 0x00000000053e2f71 llvm::parseIR(llvm::MemoryBufferRef, llvm::SMDiagnostic&, llvm::LLVMContext&, llvm::ParserCallbacks, llvm::AsmParserContext*) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x53e2f71)
#15 0x00000000053e5194 llvm::parseIRFile(llvm::StringRef, llvm::SMDiagnostic&, llvm::LLVMContext&, llvm::ParserCallbacks, llvm::AsmParserContext*) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x53e5194)
#16 0x0000000000960443 optMain (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x960443)
#17 0x000073f498a29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#18 0x000073f498a29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#19 0x00000000009595f5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x9595f5)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

