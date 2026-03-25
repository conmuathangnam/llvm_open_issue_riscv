# [X86] Backend crash: Cannot select any_extend from PINSRB result during udivrem vectorization

**Author:** alexey-milovidov
**URL:** https://github.com/llvm/llvm-project/issues/178777
**Status:** Open
**Labels:** vectorizers, crash

## Body

Clang crashes during instruction selection when compiling ClickHouse's `AggregateFunctionSparkbar.cpp` with -O3.

**Clang version:** 23.0.0git (commit 3c45148e9266bbd9830c2b977c497795d9307c72)

**Error:**
fatal error: error in backend: Cannot select: 0x...: i32 = any_extend ...
  ... v16i8 = X86ISD::PINSRB ...
    ... i8,i8 = udivrem ...

**Reproducer:**
[AggregateFunctionSparkbar-3d6231.sh](https://github.com/user-attachments/files/24950252/AggregateFunctionSparkbar-3d6231.sh)
[AggregateFunctionSparkbar-3d6231.cpp](https://github.com/user-attachments/files/24950256/AggregateFunctionSparkbar-3d6231.cpp)

**Analysis:**
The crash occurs in `X86DAGToDAGISel::Select` when the vectorizer creates a sequence involving:
1. 8-bit unsigned division (`udivrem`)
2. Results inserted into a v16i8 vector via `PINSRB`
3. An `any_extend` to i32 from the vector result

The X86 backend cannot select an instruction pattern for this combination.

**Workaround:**
Adding `#pragma clang optimize off` for this function.

See https://github.com/ClickHouse/ClickHouse/pull/95578

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-x86

Author: Alexey Milovidov (alexey-milovidov)

<details>
Clang crashes during instruction selection when compiling ClickHouse's `AggregateFunctionSparkbar.cpp` with -O3.

**Clang version:** 23.0.0git (commit 3c45148e9266bbd9830c2b977c497795d9307c72)

**Error:**
fatal error: error in backend: Cannot select: 0x...: i32 = any_extend ...
  ... v16i8 = X86ISD::PINSRB ...
    ... i8,i8 = udivrem ...

**Reproducer:**
[AggregateFunctionSparkbar-3d6231.sh](https://github.com/user-attachments/files/24950252/AggregateFunctionSparkbar-3d6231.sh)
[AggregateFunctionSparkbar-3d6231.cpp](https://github.com/user-attachments/files/24950256/AggregateFunctionSparkbar-3d6231.cpp)

**Analysis:**
The crash occurs in `X86DAGToDAGISel::Select` when the vectorizer creates a sequence involving:
1. 8-bit unsigned division (`udivrem`)
2. Results inserted into a v16i8 vector via `PINSRB`
3. An `any_extend` to i32 from the vector result

The X86 backend cannot select an instruction pattern for this combination.

**Workaround:**
Adding `#pragma clang optimize off` for this function.

See https://github.com/ClickHouse/ClickHouse/pull/95578
</details>


---

### Comment 2 - phoebewang

@alexey-milovidov Are you using a release only build? I saw an assertion raised in VPlan.cpp:

> VPlan.cpp:267: llvm::Value *llvm::VPTransformState::get(const llvm::VPValue *, const llvm::VPLane &): Assertion `Lane.isFirstLane() && "cannot get lane > 0 for scalar"' failed.

Not sure if something already wrong in the IR. Tag vectorizers to take a look first.

---

### Comment 3 - alexey-milovidov

Yes, release only. I've built as follows:
```
git clone git@github.com:llvm/llvm-project.git
mkdir llvm-build
cd llvm-build
cmake -GNinja -DCMAKE_BUILD_TYPE:STRING=Release -DLLVM_ENABLE_PROJECTS=all -DLLVM_TARGETS_TO_BUILD=all ../llvm-project/llvm
time ninja
sudo ninja install
```

I can enable assertions if needed.

---

### Comment 4 - alexey-milovidov

Minimized it:

```
$ cat /tmp/minimal_reproducer.cpp 
```

```
// Minimal reproducer for LLVM X86 backend crash with vectorized 8-bit udivrem
// Bug: https://github.com/llvm/llvm-project/issues/178777
//
// Compile with: clang++ -O2 minimal_reproducer.cpp
// Crashes with: fatal error: error in backend: Cannot select: i32 = any_extend ...

using u8 = unsigned char;

void test(u8* __restrict arr, unsigned n, u8 y_max) {
    for (unsigned i = 0; i < n; ++i) {
        u8 y = arr[i];
        if (y <= 0)
            continue;

        u8 scaled;
        bool has_overflow = __builtin_mul_overflow(y, (u8)7, &scaled);

        if (has_overflow)
            arr[i] = y / (y_max / 7) + 1;
        else
            arr[i] = scaled / y_max + 1;
    }
}
```

```
$ clang++ --version
clang version 23.0.0git (git@github.com:llvm/llvm-project.git 3c45148e9266bbd9830c2b977c497795d9307c72)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /usr/local/bin

$ clang++ -O2 /tmp/minimal_reproducer.cpp -c
fatal error: error in backend: Cannot select: 0x55bd78514cb0: i32 = any_extend 0x55bd7851f140                                                                                                                                                                                                             
  0x55bd7851f140: v16i8 = bitcast 0x55bd78522a30
    0x55bd78522a30: v2i64 = X86ISD::UNPCKL 0x55bd78519240, 0x55bd78514150
      0x55bd78519240: v2i64 = bitcast 0x55bd7851f290
        0x55bd7851f290: v4i32 = X86ISD::UNPCKL 0x55bd7851f3e0, 0x55bd78524da0
          0x55bd7851f3e0: v4i32 = bitcast 0x55bd78518750
            0x55bd78518750: v8i16 = X86ISD::UNPCKL 0x55bd78522800, 0x55bd7851f760
              0x55bd78522800: v8i16 = bitcast 0x55bd78522480
                0x55bd78522480: v16i8 = X86ISD::UNPCKL 0x55bd78524c50, 0x55bd78524b70
                  0x55bd78524c50: v16i8 = bitcast 0x55bd78524be0

                  0x55bd78524b70: v16i8 = bitcast 0x55bd78524b00

              0x55bd7851f760: v8i16 = bitcast 0x55bd785224f0
                0x55bd785224f0: v16i8 = X86ISD::UNPCKL 0x55bd78524a90, 0x55bd785249b0
                  0x55bd78524a90: v16i8 = bitcast 0x55bd78524a20

                  0x55bd785249b0: v16i8 = bitcast 0x55bd78524940

          0x55bd78524da0: v4i32 = bitcast 0x55bd7851f7d0
            0x55bd7851f7d0: v8i16 = X86ISD::UNPCKL 0x55bd78522870, 0x55bd7851f610
              0x55bd78522870: v8i16 = bitcast 0x55bd78522560
                0x55bd78522560: v16i8 = X86ISD::UNPCKL 0x55bd785248d0, 0x55bd785247f0
                  0x55bd785248d0: v16i8 = bitcast 0x55bd78524860

                  0x55bd785247f0: v16i8 = bitcast 0x55bd78524780

              0x55bd7851f610: v8i16 = bitcast 0x55bd785225d0
                0x55bd785225d0: v16i8 = X86ISD::UNPCKL 0x55bd78524710, 0x55bd78524630
                  0x55bd78524710: v16i8 = bitcast 0x55bd785246a0

                  0x55bd78524630: v16i8 = bitcast 0x55bd78520090

      0x55bd78514150: v2i64 = bitcast 0x55bd78524fd0
        0x55bd78524fd0: v4i32 = X86ISD::UNPCKL 0x55bd785229c0, 0x55bd7851f220
          0x55bd785229c0: v4i32 = bitcast 0x55bd7851f680
            0x55bd7851f680: v8i16 = X86ISD::UNPCKL 0x55bd785228e0, 0x55bd7851f4c0
              0x55bd785228e0: v8i16 = bitcast 0x55bd78522640
                0x55bd78522640: v16i8 = X86ISD::UNPCKL 0x55bd78520020, 0x55bd7851ff40
                  0x55bd78520020: v16i8 = bitcast 0x55bd7851ffb0

                  0x55bd7851ff40: v16i8 = bitcast 0x55bd7851fed0

              0x55bd7851f4c0: v8i16 = bitcast 0x55bd785226b0
                0x55bd785226b0: v16i8 = X86ISD::UNPCKL 0x55bd7851fe60, 0x55bd7851fd80
                  0x55bd7851fe60: v16i8 = bitcast 0x55bd7851fdf0

                  0x55bd7851fd80: v16i8 = bitcast 0x55bd7851fd10

          0x55bd7851f220: v4i32 = bitcast 0x55bd7851f530
            0x55bd7851f530: v8i16 = X86ISD::UNPCKL 0x55bd78522950, 0x55bd7851f370
              0x55bd78522950: v8i16 = bitcast 0x55bd78522720
                0x55bd78522720: v16i8 = X86ISD::UNPCKL 0x55bd7851fca0, 0x55bd7851fbc0
                  0x55bd7851fca0: v16i8 = bitcast 0x55bd7851fc30

                  0x55bd7851fbc0: v16i8 = bitcast 0x55bd7851fb50

              0x55bd7851f370: v8i16 = bitcast 0x55bd7851f920
                0x55bd7851f920: v16i8 = X86ISD::UNPCKL 0x55bd7851fae0, 0x55bd7851fa00
                  0x55bd7851fae0: v16i8 = bitcast 0x55bd7851fa70

                  0x55bd7851fa00: v16i8 = bitcast 0x55bd7851f990

In function: _Z4testPhjh
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: clang++ -O2 /tmp/minimal_reproducer.cpp -c
1.      <eof> parser at end of file
2.      Code generation
3.      Running pass 'Function Pass Manager' on module '/tmp/minimal_reproducer.cpp'.
4.      Running pass 'X86 DAG->DAG Instruction Selection' on function '@_Z4testPhjh'
 #0 0x000055bd6a8b1ef8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/usr/local/bin/clang-23+0x332eef8)
 #1 0x000055bd6a8af985 llvm::sys::RunSignalHandlers() (/usr/local/bin/clang-23+0x332c985)
 #2 0x000055bd6a82308e (anonymous namespace)::CrashRecoveryContextImpl::HandleCrash(int, unsigned long) CrashRecoveryContext.cpp:0:0
 #3 0x000055bd6a82304b (/usr/local/bin/clang-23+0x32a004b)
 #4 0x000055bd6a8ac4f7 llvm::sys::Process::Exit(int, bool) (/usr/local/bin/clang-23+0x33294f7)
 #5 0x000055bd684b1196 (/usr/local/bin/clang-23+0xf2e196)
 #6 0x000055bd6a828df6 llvm::report_fatal_error(llvm::Twine const&, bool) (/usr/local/bin/clang-23+0x32a5df6)
 #7 0x000055bd6b934ff5 (/usr/local/bin/clang-23+0x43b1ff5)
 #8 0x000055bd6b934615 (/usr/local/bin/clang-23+0x43b1615)
 #9 0x000055bd698befdd (anonymous namespace)::X86DAGToDAGISel::Select(llvm::SDNode*) X86ISelDAGToDAG.cpp:0:0
#10 0x000055bd6b92b57c llvm::SelectionDAGISel::DoInstructionSelection() (/usr/local/bin/clang-23+0x43a857c)
#11 0x000055bd6b92ad38 llvm::SelectionDAGISel::CodeGenAndEmitDAG() (/usr/local/bin/clang-23+0x43a7d38)
#12 0x000055bd6b92a538 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&) (/usr/local/bin/clang-23+0x43a7538)
#13 0x000055bd6b92839a llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&) (/usr/local/bin/clang-23+0x43a539a)
#14 0x000055bd6b92673d llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&) (/usr/local/bin/clang-23+0x43a373d)
#15 0x000055bd69f1b238 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (/usr/local/bin/clang-23+0x2998238)
#16 0x000055bd6a36ceca llvm::FPPassManager::runOnFunction(llvm::Function&) (/usr/local/bin/clang-23+0x2de9eca)
#17 0x000055bd6a3736d3 llvm::FPPassManager::runOnModule(llvm::Module&) (/usr/local/bin/clang-23+0x2df06d3)
#18 0x000055bd6a36dbd6 llvm::legacy::PassManagerImpl::run(llvm::Module&) (/usr/local/bin/clang-23+0x2deabd6)
#19 0x000055bd6aa8e324 clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/usr/local/bin/clang-23+0x350b324)
#20 0x000055bd6af228ae clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/usr/local/bin/clang-23+0x399f8ae)
#21 0x000055bd6c57f5c9 clang::ParseAST(clang::Sema&, bool, bool) (/usr/local/bin/clang-23+0x4ffc5c9)
#22 0x000055bd6b1852ed clang::FrontendAction::Execute() (/usr/local/bin/clang-23+0x3c022ed)
#23 0x000055bd6b0f8b94 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/usr/local/bin/clang-23+0x3b75b94)
#24 0x000055bd6b2519a9 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/local/bin/clang-23+0x3cce9a9)
#25 0x000055bd684b09a7 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/usr/local/bin/clang-23+0xf2d9a7)
#26 0x000055bd684ad3c7 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#27 0x000055bd684aec6d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::$_0>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#28 0x000055bd6af7dcf9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::$_0>(long) Job.cpp:0:0
#29 0x000055bd6a823028 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/usr/local/bin/clang-23+0x32a0028)
#30 0x000055bd6af7d6a5 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (/usr/local/bin/clang-23+0x39fa6a5)
#31 0x000055bd6af44b6f clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/usr/local/bin/clang-23+0x39c1b6f)
#32 0x000055bd6af44d0e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/usr/local/bin/clang-23+0x39c1d0e)
#33 0x000055bd6af5f0cd clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/usr/local/bin/clang-23+0x39dc0cd)
#34 0x000055bd684acd99 clang_main(int, char**, llvm::ToolContext const&) (/usr/local/bin/clang-23+0xf29d99)
#35 0x000055bd684bacb7 main (/usr/local/bin/clang-23+0xf37cb7)
#36 0x00007f5c0e0bcd90 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:58:16
#37 0x00007f5c0e0bce40 call_init ./csu/../csu/libc-start.c:128:20
#38 0x00007f5c0e0bce40 __libc_start_main ./csu/../csu/libc-start.c:379:5
#39 0x000055bd684aafa5 _start (/usr/local/bin/clang-23+0xf27fa5)
clang++: error: clang frontend command failed with exit code 70 (use -v to see invocation)
clang version 23.0.0git (git@github.com:llvm/llvm-project.git 3c45148e9266bbd9830c2b977c497795d9307c72)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /usr/local/bin
clang++: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: /tmp/minimal_reproducer-023c7b.cpp
clang++: note: diagnostic msg: /tmp/minimal_reproducer-023c7b.sh
clang++: note: diagnostic msg: 

********************
```

Btw, this is the Claude Code (Opus 4.5) prompt for minimizing the reproducer:
`Using reproducers from this issue: https://github.com/llvm/llvm-project/issues/178777, try to minimize the reproducer. Btw, the corresponding clang version is already installed in the system, so you can easily compile the reproducers.`

---

### Comment 5 - RKSimon

https://clang.godbolt.org/z/jWEfcMojz

LoopVectorizePass is creating:
```ll
pred.udiv.if:                                     ; preds = %vector.body
  %8 = extractvalue { <16 x i8>, <16 x i1> } %3, 0
  %9 = udiv i8 %8, %y_max
  %10 = insertelement <16 x i8> poison, i8 %9, i32 0
  br label %pred.udiv.continue
```
%8 is a <16 x i8> but %9 treats it as a %8

@fhahn could LV be getting confused over aggregates of vectors?

---

