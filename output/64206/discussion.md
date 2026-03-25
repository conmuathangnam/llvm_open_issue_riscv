# [RISC-V] Do not know how to soften llvm.{min,max}imum.{f32,f64} result

**Author:** Urgau
**URL:** https://github.com/llvm/llvm-project/issues/64206
**Status:** Closed
**Labels:** backend:RISC-V, crash, llvm:SelectionDAG
**Closed Date:** 2025-05-08T09:22:35Z

## Body

It seems that none of these intrinsics are currently working on RISC-V:

- llvm.minimum.f32
- llvm.minimum.f64
- llvm.maximum.f32
- llvm.maximum.f64

Error output [\[godbolt\]](https://godbolt.org/z/rs54vf7YY):
```
LLVM ERROR: Do not know how to soften the result of this operator!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/llc -o /app/output.s -x86-asm-syntax=intel <source>
1.	Running pass 'Function Pass Manager' on module '<source>'.
2.	Running pass 'RISC-V DAG->DAG Pattern Instruction Selection' on function '@_ZN1a4test17hb60f9f6975710eeaE'
 #0 0x0000000003066278 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-trunk/bin/llc+0x3066278)
 #1 0x0000000003063e3c SignalHandler(int) Signals.cpp:0:0
 #2 0x00007efea069d420 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x14420)
 #3 0x00007efea016000b raise (/lib/x86_64-linux-gnu/libc.so.6+0x4300b)
 #4 0x00007efea013f859 abort (/lib/x86_64-linux-gnu/libc.so.6+0x22859)
 #5 0x000000000069cfa2 llvm::RISCVISAInfo::updateImplication() (.cold) RISCVISAInfo.cpp:0:0
 #6 0x0000000002fc1688 (/opt/compiler-explorer/clang-trunk/bin/llc+0x2fc1688)
 #7 0x0000000002f3df91 llvm::DAGTypeLegalizer::SoftenFloatResult(llvm::SDNode*, unsigned int) (/opt/compiler-explorer/clang-trunk/bin/llc+0x2f3df91)
 #8 0x0000000002ed7612 llvm::DAGTypeLegalizer::run() (/opt/compiler-explorer/clang-trunk/bin/llc+0x2ed7612)
 #9 0x0000000002ed7c69 llvm::SelectionDAG::LegalizeTypes() (/opt/compiler-explorer/clang-trunk/bin/llc+0x2ed7c69)
#10 0x0000000002e58dd1 llvm::SelectionDAGISel::CodeGenAndEmitDAG() (/opt/compiler-explorer/clang-trunk/bin/llc+0x2e58dd1)
#11 0x0000000002e5b9b9 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&) (/opt/compiler-explorer/clang-trunk/bin/llc+0x2e5b9b9)
#12 0x0000000002e5e086 llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&) (.part.0) SelectionDAGISel.cpp:0:0
#13 0x0000000002443820 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (.part.0) MachineFunctionPass.cpp:0:0
#14 0x00000000029263f2 llvm::FPPassManager::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-trunk/bin/llc+0x29263f2)
#15 0x0000000002926571 llvm::FPPassManager::runOnModule(llvm::Module&) (/opt/compiler-explorer/clang-trunk/bin/llc+0x2926571)
#16 0x00000000029280e0 llvm::legacy::PassManagerImpl::run(llvm::Module&) (/opt/compiler-explorer/clang-trunk/bin/llc+0x29280e0)
#17 0x0000000000766170 compileModule(char**, llvm::LLVMContext&) llc.cpp:0:0
#18 0x00000000006a7ce2 main (/opt/compiler-explorer/clang-trunk/bin/llc+0x6a7ce2)
#19 0x00007efea0141083 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x24083)
#20 0x000000000075e0be _start (/opt/compiler-explorer/clang-trunk/bin/llc+0x75e0be)
```

Similar to https://github.com/llvm/llvm-project/issues/53353 which was for x86_64

## Comments

### Comment 1 - llvmbot

@llvm/issue-subscribers-backend-risc-v

---

### Comment 2 - dtcxzyw

It works with `-mattr=+f,+d` after 49429783b0f4fb9c6f2a25746fe90072e19eb38c. 
godbolt: https://godbolt.org/z/YM754x6o7
I have no idea how to add a fallback to libcalls (not C `fmin`) without F/D ext.


---

### Comment 3 - wangpc-pp

As what has been documented in https://reviews.llvm.org/D152718, this is a known issue because there is no corresponding libcalls for `minimum`/`maximum`.
> // FIXME: We do not have libcalls for FMAXIMUM and FMINIMUM. So, we cannot use
   // libcall legalization for these nodes, but there is no default expasion for
   // these nodes either (see PR63267 for example).

---

### Comment 4 - el-ev

Fixed since 20.1.0 on godbolt

---

