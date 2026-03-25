# [RISCV] Crash when ir-outliner is run on functions with interrupt attribute

**Author:** svs-quic
**URL:** https://github.com/llvm/llvm-project/issues/149969
**Status:** Closed
**Labels:** backend:RISC-V, ipo, crash
**Closed Date:** 2026-02-11T03:49:18Z

## Body

Consider the IR:

https://godbolt.org/z/czqe7ja33

```
define void @bar() "interrupt"="machine" {
  %1 = tail call i32 asm sideeffect "li $0, $1", "=r,i"(i32 10) nounwind
  %3 = and i32 %1, 31
  %4 = add nsw i32 %3, 6
  %5 = urem i32 %4, 24
  %6 = shl nuw nsw i32 1, %5
  tail call void @foo(i32 noundef %6) nounwind
  ret void
}

declare void @foo(i32 noundef) #2

define void @baz() "interrupt"="machine" {
  %1 = tail call i32 asm sideeffect "li $0, $1", "=r,i"(i32 10) nounwind
  %3 = and i32 %1, 31
  %4 = add nsw i32 %3, 6
  %5 = urem i32 %4, 24
  %6 = shl nuw nsw i32 1, %5
  tail call void @foo(i32 noundef %6) nounwind
  ret void
}
```

The IR Outliner when run on this IR produces:

```
define void @bar() #0 {
  %1 = tail call i32 asm sideeffect "li $0, $1", "=r,i"(i32 10) #2
  call void @outlined_ir_func_0(i32 %1)
  ret void
}

declare void @foo(i32 noundef)

define void @baz() #0 {
  %1 = tail call i32 asm sideeffect "li $0, $1", "=r,i"(i32 10) #2
  call void @outlined_ir_func_0(i32 %1)
  ret void
}

define internal void @outlined_ir_func_0(i32 %0) #1 {
newFuncRoot:
  br label %_to_outline

_to_outline:
  %1 = and i32 %0, 31
  %2 = add nsw i32 %1, 6
  %3 = urem i32 %2, 24
  %4 = shl nuw nsw i32 1, %3
  tail call void @foo(i32 noundef %4) #2
  br label %_after_outline.exitStub

_after_outline.exitStub:
  ret void
}

attributes #0 = { "interrupt"="machine" }
attributes #1 = { minsize optsize "interrupt"="machine" }
attributes #2 = { nounwind }
```

When we trying to run the following IR it crashes with the following message:

```
LLVM ERROR: Functions with the interrupt attribute cannot have arguments!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/llc -o /app/output.s -x86-asm-syntax=intel -mtriple riscv32 <source>
1.	Running pass 'Function Pass Manager' on module '<source>'.
2.	Running pass 'RISC-V DAG->DAG Pattern Instruction Selection' on function '@outlined_ir_func_0'
 #0 0x0000000003bac828 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-trunk/bin/llc+0x3bac828)
 #1 0x0000000003ba99f4 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x00007aa330242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x00007aa3302969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x00007aa330242476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x00007aa3302287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x0000000000794a80 llvm::UniqueStringSaver::save(llvm::Twine const&) (.cold) StringSaver.cpp:0:0
 #7 0x0000000003b00648 (/opt/compiler-explorer/clang-trunk/bin/llc+0x3b00648)
 #8 0x0000000001b89fbc llvm::RISCVTargetLowering::LowerFormalArguments(llvm::SDValue, unsigned int, bool, llvm::SmallVectorImpl<llvm::ISD::InputArg> const&, llvm::SDLoc const&, llvm::SelectionDAG&, llvm::SmallVectorImpl<llvm::SDValue>&) const (/opt/compiler-explorer/clang-trunk/bin/llc+0x1b89fbc)
 #9 0x00000000038dc1ca llvm::SelectionDAGISel::LowerArguments(llvm::Function const&) (/opt/compiler-explorer/clang-trunk/bin/llc+0x38dc1ca)
#10 0x000000000395ff65 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&) (/opt/compiler-explorer/clang-trunk/bin/llc+0x395ff65)
#11 0x00000000039609b6 llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&) (/opt/compiler-explorer/clang-trunk/bin/llc+0x39609b6)
#12 0x000000000394d431 llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&) (/opt/compiler-explorer/clang-trunk/bin/llc+0x394d431)
#13 0x0000000002c0ca1d llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-trunk/bin/llc+0x2c0ca1d)
#14 0x000000000318fce2 llvm::FPPassManager::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-trunk/bin/llc+0x318fce2)
#15 0x000000000318ff71 llvm::FPPassManager::runOnModule(llvm::Module&) (/opt/compiler-explorer/clang-trunk/bin/llc+0x318ff71)
#16 0x00000000031918b9 llvm::legacy::PassManagerImpl::run(llvm::Module&) (/opt/compiler-explorer/clang-trunk/bin/llc+0x31918b9)
#17 0x00000000008d93a9 compileModule(char**, llvm::LLVMContext&) llc.cpp:0:0
#18 0x00000000007aab3e main (/opt/compiler-explorer/clang-trunk/bin/llc+0x7aab3e)
#19 0x00007aa330229d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#20 0x00007aa330229e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#21 0x00000000008d0c35 _start (/opt/compiler-explorer/clang-trunk/bin/llc+0x8d0c35)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

https://godbolt.org/z/drvWcKxYM

It looks like the IR Outliner is copying over the attributes from the original functions to the outlined one leading to this error. We could probably mark the original functions as `nooutline` but I don't think the outliner is doing anything incorrect here. Should we not be propagating the interrupt attribute to the outlined function?

There is no crash seen on ARM: https://godbolt.org/z/fv1GGc3oG

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-risc-v

Author: Sudharsan Veeravalli (svs-quic)

<details>
Consider the IR:

https://godbolt.org/z/czqe7ja33

```
define void @<!-- -->bar() "interrupt"="machine" {
  %1 = tail call i32 asm sideeffect "li $0, $1", "=r,i"(i32 10) nounwind
  %3 = and i32 %1, 31
  %4 = add nsw i32 %3, 6
  %5 = urem i32 %4, 24
  %6 = shl nuw nsw i32 1, %5
  tail call void @<!-- -->foo(i32 noundef %6) nounwind
  ret void
}

declare void @<!-- -->foo(i32 noundef) #<!-- -->2

define void @<!-- -->baz() "interrupt"="machine" {
  %1 = tail call i32 asm sideeffect "li $0, $1", "=r,i"(i32 10) nounwind
  %3 = and i32 %1, 31
  %4 = add nsw i32 %3, 6
  %5 = urem i32 %4, 24
  %6 = shl nuw nsw i32 1, %5
  tail call void @<!-- -->foo(i32 noundef %6) nounwind
  ret void
}
```

The IR Outliner when run on this IR produces:

```
define void @<!-- -->bar() #<!-- -->0 {
  %1 = tail call i32 asm sideeffect "li $0, $1", "=r,i"(i32 10) #<!-- -->2
  call void @<!-- -->outlined_ir_func_0(i32 %1)
  ret void
}

declare void @<!-- -->foo(i32 noundef)

define void @<!-- -->baz() #<!-- -->0 {
  %1 = tail call i32 asm sideeffect "li $0, $1", "=r,i"(i32 10) #<!-- -->2
  call void @<!-- -->outlined_ir_func_0(i32 %1)
  ret void
}

define internal void @<!-- -->outlined_ir_func_0(i32 %0) #<!-- -->1 {
newFuncRoot:
  br label %_to_outline

_to_outline:
  %1 = and i32 %0, 31
  %2 = add nsw i32 %1, 6
  %3 = urem i32 %2, 24
  %4 = shl nuw nsw i32 1, %3
  tail call void @<!-- -->foo(i32 noundef %4) #<!-- -->2
  br label %_after_outline.exitStub

_after_outline.exitStub:
  ret void
}

attributes #<!-- -->0 = { "interrupt"="machine" }
attributes #<!-- -->1 = { minsize optsize "interrupt"="machine" }
attributes #<!-- -->2 = { nounwind }
```

When we trying to run the following IR it crashes with the following message:

```
LLVM ERROR: Functions with the interrupt attribute cannot have arguments!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/llc -o /app/output.s -x86-asm-syntax=intel -mtriple riscv32 &lt;source&gt;
1.	Running pass 'Function Pass Manager' on module '&lt;source&gt;'.
2.	Running pass 'RISC-V DAG-&gt;DAG Pattern Instruction Selection' on function '@<!-- -->outlined_ir_func_0'
 #<!-- -->0 0x0000000003bac828 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-trunk/bin/llc+0x3bac828)
 #<!-- -->1 0x0000000003ba99f4 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->2 0x00007aa330242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->3 0x00007aa3302969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->4 0x00007aa330242476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->5 0x00007aa3302287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->6 0x0000000000794a80 llvm::UniqueStringSaver::save(llvm::Twine const&amp;) (.cold) StringSaver.cpp:0:0
 #<!-- -->7 0x0000000003b00648 (/opt/compiler-explorer/clang-trunk/bin/llc+0x3b00648)
 #<!-- -->8 0x0000000001b89fbc llvm::RISCVTargetLowering::LowerFormalArguments(llvm::SDValue, unsigned int, bool, llvm::SmallVectorImpl&lt;llvm::ISD::InputArg&gt; const&amp;, llvm::SDLoc const&amp;, llvm::SelectionDAG&amp;, llvm::SmallVectorImpl&lt;llvm::SDValue&gt;&amp;) const (/opt/compiler-explorer/clang-trunk/bin/llc+0x1b89fbc)
 #<!-- -->9 0x00000000038dc1ca llvm::SelectionDAGISel::LowerArguments(llvm::Function const&amp;) (/opt/compiler-explorer/clang-trunk/bin/llc+0x38dc1ca)
#<!-- -->10 0x000000000395ff65 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&amp;) (/opt/compiler-explorer/clang-trunk/bin/llc+0x395ff65)
#<!-- -->11 0x00000000039609b6 llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&amp;) (/opt/compiler-explorer/clang-trunk/bin/llc+0x39609b6)
#<!-- -->12 0x000000000394d431 llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&amp;) (/opt/compiler-explorer/clang-trunk/bin/llc+0x394d431)
#<!-- -->13 0x0000000002c0ca1d llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (/opt/compiler-explorer/clang-trunk/bin/llc+0x2c0ca1d)
#<!-- -->14 0x000000000318fce2 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (/opt/compiler-explorer/clang-trunk/bin/llc+0x318fce2)
#<!-- -->15 0x000000000318ff71 llvm::FPPassManager::runOnModule(llvm::Module&amp;) (/opt/compiler-explorer/clang-trunk/bin/llc+0x318ff71)
#<!-- -->16 0x00000000031918b9 llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/opt/compiler-explorer/clang-trunk/bin/llc+0x31918b9)
#<!-- -->17 0x00000000008d93a9 compileModule(char**, llvm::LLVMContext&amp;) llc.cpp:0:0
#<!-- -->18 0x00000000007aab3e main (/opt/compiler-explorer/clang-trunk/bin/llc+0x7aab3e)
#<!-- -->19 0x00007aa330229d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->20 0x00007aa330229e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->21 0x00000000008d0c35 _start (/opt/compiler-explorer/clang-trunk/bin/llc+0x8d0c35)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

https://godbolt.org/z/drvWcKxYM

It looks like the IR Outliner is copying over the attributes from the original functions to the outlined one leading to this error. We could probably mark the original functions as `nooutline` but I don't think the outliner is doing anything incorrect here. Should we not be propagating the interrupt attribute to the outlined function?

Ther is no crash seen on ARM: https://godbolt.org/z/fv1GGc3oG
</details>


---

### Comment 2 - lenary

This is not a selectiondag issue, it's a problem with the IR outliner (not even the machine outliner)

The `"interrupt"` attribute in risc-v means the function is an interrupt service routine, and has a different prolog/epilog (the value of the attribute informs which prolog/epilog), and cannot have any arguments/return values. These functions can call regular functions, and the functions they call do not need the interrupt attribute.

I don't know which other targets use these IR attributes, and which do the same enforcement that RISC-V does that there are no arguments/returns.

The problem, as far as I see it, is for the IR outliner to not blindly copy attributes from the origin function into the outlined function.

---

### Comment 3 - ukalappa-mips

@lenary and @svs-quic    ,the outliner shouldn't the outline these interrupt attributed functions at first place and we are looking at it .

---

### Comment 4 - lenary

@ukalappa-mips I don't think there's a problem outlining from interrupt functions (on risc-v at least) - if there was we'd have just told our users to add the `nooutline` attribute.

As I said, it's fine for `__attribute__((interrupt))` functions to make any calls they want to, including to outlined functions - the problem is just that the outliner is propagating the attributes from the original function to the outlined function incorrectly.

---

