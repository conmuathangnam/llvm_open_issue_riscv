# Assertion failure in CanShareConstantPoolEntry via implicit TypeSize-to-uint64_t conversion

**Author:** yijan4845
**URL:** https://github.com/llvm/llvm-project/issues/183335
**Status:** Open
**Labels:** llvm:codegen, crash-on-invalid

## Body

Compiler Explorer: [https://godbolt.org/z/bdf9P7MWc](https://godbolt.org/z/bdf9P7MWc)

## Vulnerable code location(s)

- [llvm/lib/CodeGen/MachineFunction.cpp:1528](https://github.com/llvm/llvm-project/blob/9f7af289728002487b032a8278bc2c540a02ff59/llvm/lib/CodeGen/MachineFunction.cpp#L1528): `uint64_t StoreSize = DL.getTypeStoreSize(A->getType());`
- [llvm/lib/CodeGen/MachineFunction.cpp:1529](https://github.com/llvm/llvm-project/blob/9f7af289728002487b032a8278bc2c540a02ff59/llvm/lib/CodeGen/MachineFunction.cpp#L1529): `DL.getTypeStoreSize(B->getType())` implicitly converted to `uint64_t` in comparison.
- [llvm/lib/CodeGen/MachineFunction.cpp:1534](https://github.com/llvm/llvm-project/blob/9f7af289728002487b032a8278bc2c540a02ff59/llvm/lib/CodeGen/MachineFunction.cpp#L1534): `IntegerType::get(A->getContext(), StoreSize*8)` would produce an incorrect fixed-width type for scalable vectors.

## Vulnerable code analysis

`CanShareConstantPoolEntry` guards against `StructType` and `ArrayType` (lines 1522–1525) but not `ScalableVectorType`. When `MachineConstantPool::getConstantPoolIndex` (line 1574) receives a scalable vector constant, it calls `CanShareConstantPoolEntry` against existing pool entries. `DL.getTypeStoreSize()` returns a `TypeSize` with `isScalable() == true`, and the implicit conversion to `uint64_t` fires the assertion `"Invalid size request on a scalable vector."`.

## PoC

```
--- |
  target datalayout = "e-m:e-i8:8:32-i16:16:32-i64:64-i128:128-n32:64-S128"
  target triple = "x86_64-unknown-linux-gnu"

  define void @test() {
    ret void
  }
...
---
name: test
constants:
  - id:          0
    value:        'i32 1'
    alignment:    4
  - id:          1
    value:        '<vscale x 4 x i32> splat (i32 1)'
    alignment:    16
body: |
  bb.0:
    RETQ
...
```

Stack dump:
```
LLVM ERROR: Cannot implicitly convert a scalable size to a fixed-width size in `TypeSize::operator ScalarTy()`
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/llc -o /app/output.s -x86-asm-syntax=intel <source>
 #0 0x0000000003e16fb8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-trunk/bin/llc+0x3e16fb8)
 #1 0x0000000003e140f4 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x00007b73a0e42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x00007b73a0e969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x00007b73a0e42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x00007b73a0e287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x00000000007bc869 llvm::json::operator==(llvm::json::Value const&, llvm::json::Value const&) (.cold) JSON.cpp:0:0
 #7 0x0000000003d58ea9 (/opt/compiler-explorer/clang-trunk/bin/llc+0x3d58ea9)
 #8 0x0000000002d782a9 llvm::MachineConstantPool::getConstantPoolIndex(llvm::Constant const*, llvm::Align) (/opt/compiler-explorer/clang-trunk/bin/llc+0x2d782a9)
 #9 0x000000000358f82d llvm::MIRParserImpl::initializeConstantPool(llvm::PerFunctionMIParsingState&, llvm::MachineConstantPool&, llvm::yaml::MachineFunction const&) (/opt/compiler-explorer/clang-trunk/bin/llc+0x358f82d)
#10 0x0000000003598915 llvm::MIRParserImpl::initializeMachineFunction(llvm::yaml::MachineFunction const&, llvm::MachineFunction&) (/opt/compiler-explorer/clang-trunk/bin/llc+0x3598915)
#11 0x000000000359b187 llvm::MIRParserImpl::parseMachineFunction(llvm::Module&, llvm::MachineModuleInfo&, llvm::AnalysisManager<llvm::Module>*) (/opt/compiler-explorer/clang-trunk/bin/llc+0x359b187)
#12 0x000000000359b35c llvm::MIRParser::parseMachineFunctions(llvm::Module&, llvm::MachineModuleInfo&) (/opt/compiler-explorer/clang-trunk/bin/llc+0x359b35c)
#13 0x00000000008fce7c compileModule(char**, llvm::SmallVectorImpl<llvm::PassPlugin>&, llvm::LLVMContext&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>&) llc.cpp:0:0
#14 0x00000000007cbb9c main (/opt/compiler-explorer/clang-trunk/bin/llc+0x7cbb9c)
#15 0x00007b73a0e29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#16 0x00007b73a0e29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#17 0x00000000008f3435 _start (/opt/compiler-explorer/clang-trunk/bin/llc+0x8f3435)
Program terminated with signal: SIGSEGV
Compiler returned: 139

```


