# SafeStack crashes on dynamic alloca with scalable vector type due to TypeSize implicit conversion

**Author:** yijan4845
**URL:** https://github.com/llvm/llvm-project/issues/182759
**Status:** Open
**Labels:** llvm:codegen, crash, regression:13

## Body

Compiler Explorer: [https://godbolt.org/z/Y1rTr61r7](https://godbolt.org/z/Y1rTr61r7)

This seems to be a regression in llc-13.

## Vulnerable code location(s)

- [llvm/lib/CodeGen/SafeStack.cpp:676](https://github.com/llvm/llvm-project/blob/9f7af289728002487b032a8278bc2c540a02ff59/llvm/lib/CodeGen/SafeStack.cpp#L676): primary crash site (`getTypeAllocSize`: implicit `uint64_t` conversion)
- [llvm/lib/CodeGen/SafeStack.cpp:411](https://github.com/llvm/llvm-project/blob/9f7af289728002487b032a8278bc2c540a02ff59/llvm/lib/CodeGen/SafeStack.cpp#L411): `getTypeStoreSize` on byval arg
- [llvm/lib/CodeGen/SafeStack.cpp:520](https://github.com/llvm/llvm-project/blob/9f7af289728002487b032a8278bc2c540a02ff59/llvm/lib/CodeGen/SafeStack.cpp#L520): `getTypeStoreSize` on byval arg
- [llvm/lib/CodeGen/SafeStack.cpp:578](https://github.com/llvm/llvm-project/blob/9f7af289728002487b032a8278bc2c540a02ff59/llvm/lib/CodeGen/SafeStack.cpp#L578): `getTypeStoreSize` on byval arg

## Vulnerable code analysis

At line 676, `DL.getTypeAllocSize(Ty)` returns a `TypeSize` object that is implicitly converted to `uint64_t`:

```cpp
uint64_t TySize = DL.getTypeAllocSize(Ty);
```

When `Ty` is a scalable vector type (e.g., `<vscale x 4 x i32>`), `TypeSize::operator ScalarTy()` calls `reportFatalInternalError()` and aborts because scalable sizes cannot be represented as a fixed `uint64_t`.

This path is reached when:

1. A function has the `safestack` attribute.
2. It contains a dynamic alloca (non-constant array size) with a scalable vector allocated type.
3. The alloca is classified as unsafe, routing it to `moveDynamicAllocasToUnsafeStack`.

Note that `getStaticAllocaAllocationSize` (line 198) also calls `getTypeAllocSize` with an implicit conversion to `uint64_t` (line 199), but static allocas with scalable types are filtered earlier. The dynamic alloca path has no such guard.

## Simplified PoC

```
define void @test(i32 %n) #0 {
  %p = alloca <vscale x 4 x i32>, i32 %n
  ret void
}
```

Stack dump:

```
LLVM ERROR: Invalid size request on a scalable vector.
PLEASE submit a bug report to https://bugs.llvm.org/ and include the crash backtrace.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-13.0.0/bin/llc -o /app/output.s -x86-asm-syntax=intel <source>
1.	Running pass 'Function Pass Manager' on module '<source>'.
2.	Running pass 'X86 DAG->DAG Instruction Selection' on function '@test'
 #0 0x00006297a34435b4 PrintStackTraceSignalHandler(void*) Signals.cpp:0:0
 #1 0x00006297a3440dce SignalHandler(int) Signals.cpp:0:0
 #2 0x00007d9c4e842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x00007d9c4e8969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x00007d9c4e842476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x00007d9c4e8287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x00006297a33999ca llvm::report_fatal_error(llvm::Twine const&, bool) (/opt/compiler-explorer/clang-13.0.0/bin/llc+0x2bf49ca)
 #7 0x00006297a3399b6e (/opt/compiler-explorer/clang-13.0.0/bin/llc+0x2bf4b6e)
 #8 0x00006297a33e66ad (/opt/compiler-explorer/clang-13.0.0/bin/llc+0x2c416ad)
 #9 0x00006297a33e66d4 llvm::TypeSize::operator unsigned long() const (/opt/compiler-explorer/clang-13.0.0/bin/llc+0x2c416d4)
#10 0x00006297a31cddee llvm::SelectionDAGBuilder::visitAlloca(llvm::AllocaInst const&) (/opt/compiler-explorer/clang-13.0.0/bin/llc+0x2a28dee)
#11 0x00006297a31efa67 llvm::SelectionDAGBuilder::visit(llvm::Instruction const&) (/opt/compiler-explorer/clang-13.0.0/bin/llc+0x2a4aa67)
#12 0x00006297a325250e llvm::SelectionDAGISel::SelectBasicBlock(llvm::ilist_iterator<llvm::ilist_detail::node_options<llvm::Instruction, false, false, void>, false, true>, llvm::ilist_iterator<llvm::ilist_detail::node_options<llvm::Instruction, false, false, void>, false, true>, bool&) (/opt/compiler-explorer/clang-13.0.0/bin/llc+0x2aad50e)
#13 0x00006297a3253a60 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&) (/opt/compiler-explorer/clang-13.0.0/bin/llc+0x2aaea60)
#14 0x00006297a32564d7 llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&) (.part.0) SelectionDAGISel.cpp:0:0
#15 0x00006297a1eddebd (anonymous namespace)::X86DAGToDAGISel::runOnMachineFunction(llvm::MachineFunction&) X86ISelDAGToDAG.cpp:0:0
#16 0x00006297a27f1f0c llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-13.0.0/bin/llc+0x204cf0c)
#17 0x00006297a2c3d2e0 llvm::FPPassManager::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-13.0.0/bin/llc+0x24982e0)
#18 0x00006297a2c3d459 llvm::FPPassManager::runOnModule(llvm::Module&) (/opt/compiler-explorer/clang-13.0.0/bin/llc+0x2498459)
#19 0x00006297a2c3f790 llvm::legacy::PassManagerImpl::run(llvm::Module&) (/opt/compiler-explorer/clang-13.0.0/bin/llc+0x249a790)
#20 0x00006297a0f75c2f main (/opt/compiler-explorer/clang-13.0.0/bin/llc+0x7d0c2f)
#21 0x00007d9c4e829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#22 0x00007d9c4e829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#23 0x00006297a103be8e _start (/opt/compiler-explorer/clang-13.0.0/bin/llc+0x896e8e)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```






