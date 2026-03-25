# [X86] llc SIGSEGV in llvm::X86TargetLowering::LowerVAARG

**Author:** aHeraud
**URL:** https://github.com/llvm/llvm-project/issues/162900
**Status:** Open
**Labels:** backend:X86, crash, llvm:SelectionDAG

## Body

I added var args support to my toy C compiler, and have discovered a segfault inside of llc when attempting to compile my generated llvm module. 

Here's a minimal example that can be used to re-produce the crash:

```
; ModuleID = 'module'
source_filename = "module"

define void @test(i32 %0, ...) {
block_0:
  %1 = alloca <{ i32, i32, ptr, ptr }>, align 8
  %2 = alloca <{ i32 }>, align 8
  call void @llvm.va_start(ptr %1)
  %3 = va_arg ptr %1, <{ i32 }>
  store <{ i32 }> %3, ptr %2, align 1
  call void @llvm.va_end(ptr %1)
  ret void
}

; Function Attrs: nocallback nofree nosync nounwind willreturn
declare void @llvm.va_start(ptr) #0

; Function Attrs: nocallback nofree nosync nounwind willreturn
declare void @llvm.va_end(ptr) #0

attributes #0 = { nocallback nofree nosync nounwind willreturn }
```

The crash backtrace from llc:
```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.      Program arguments: ../llvm-21/LLVM-21.1.0-Linux-X64/bin/llc main.ll
1.      Running pass 'Function Pass Manager' on module 'main.ll'.
2.      Running pass 'X86 DAG->DAG Instruction Selection' on function '@test'
 #0 0x0000598099ad0ce7 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (../llvm-21/LLVM-21.1.0-Linux-X64/bin/llc+0x62d6ce7)
 #1 0x0000598099ad14db SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x000075e1b9565050 (/lib/x86_64-linux-gnu/libc.so.6+0x3c050)
 #3 0x0000598096f8bd1e llvm::DataLayout::getTypeSizeInBits(llvm::Type*) const AArch64ISelLowering.cpp:0:0
 #4 0x0000598098fbf52f llvm::X86TargetLowering::LowerVAARG(llvm::SDValue, llvm::SelectionDAG&) const X86ISelLowering.cpp:0:0
 #5 0x00005980973a597f (anonymous namespace)::SelectionDAGLegalize::LegalizeOp(llvm::SDNode*) (.llvm.6546466950859915230) LegalizeDAG.cpp:0:0
 #6 0x00005980973a4cd3 llvm::SelectionDAG::Legalize() (../llvm-21/LLVM-21.1.0-Linux-X64/bin/llc+0x3baacd3)
 #7 0x00005980973a094e llvm::SelectionDAGISel::CodeGenAndEmitDAG() (../llvm-21/LLVM-21.1.0-Linux-X64/bin/llc+0x3ba694e)
 #8 0x0000598097165e71 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&) (../llvm-21/LLVM-21.1.0-Linux-X64/bin/llc+0x396be71)
 #9 0x0000598097b2aa8e llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&) (../llvm-21/LLVM-21.1.0-Linux-X64/bin/llc+0x4330a8e)
#10 0x0000598097a26df1 llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&) (../llvm-21/LLVM-21.1.0-Linux-X64/bin/llc+0x422cdf1)
#11 0x0000598097b01687 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (../llvm-21/LLVM-21.1.0-Linux-X64/bin/llc+0x4307687)
#12 0x000059809784be2a llvm::FPPassManager::runOnFunction(llvm::Function&) (../llvm-21/LLVM-21.1.0-Linux-X64/bin/llc+0x4051e2a)
#13 0x000059809784b768 llvm::FPPassManager::runOnModule(llvm::Module&) (../llvm-21/LLVM-21.1.0-Linux-X64/bin/llc+0x4051768)
#14 0x0000598097848023 llvm::legacy::PassManagerImpl::run(llvm::Module&) (../llvm-21/LLVM-21.1.0-Linux-X64/bin/llc+0x404e023)
#15 0x0000598098252626 main (../llvm-21/LLVM-21.1.0-Linux-X64/bin/llc+0x4a58626)
#16 0x000075e1b955024a __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#17 0x000075e1b9550305 call_init ./csu/../csu/libc-start.c:128:20
#18 0x000075e1b9550305 __libc_start_main ./csu/../csu/libc-start.c:347:5
#19 0x00005980980fbba5 _start (../llvm-21/LLVM-21.1.0-Linux-X64/bin/llc+0x4901ba5)
Segmentation fault (core dumped)
```

And the original C code for reference:
```c
struct S {
    int a;
};

void test(int n, ...) {
    __builtin_va_list args;
    __builtin_va_start(args, n);
    struct S s = __builtin_va_arg(args, struct S);
    __builtin_va_end(args);
}
```

I've tested this with llvm-19 and llvm-21. I don't have llvm checked out locally, but I also tested it against the trunk version of llc in godbolt and the crash still occurrs so the problem appears to still be present in the latest development version: https://godbolt.org/z/7o8j9v4cs.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-x86

Author: Achille Heraud (aHeraud)

<details>
I added var args support to my toy C compiler, and have discovered a segfault inside of llc when attempting to compile my generated llvm module. 

Here's a minimal example that can be used to re-produce the crash:

```
; ModuleID = 'module'
source_filename = "module"

define void @<!-- -->test(i32 %0, ...) {
block_0:
  %1 = alloca &lt;{ i32, i32, ptr, ptr }&gt;, align 8
  %2 = alloca &lt;{ i32 }&gt;, align 8
  call void @<!-- -->llvm.va_start(ptr %1)
  %3 = va_arg ptr %1, &lt;{ i32 }&gt;
  store &lt;{ i32 }&gt; %3, ptr %2, align 1
  call void @<!-- -->llvm.va_end(ptr %1)
  ret void
}

; Function Attrs: nocallback nofree nosync nounwind willreturn
declare void @<!-- -->llvm.va_start(ptr) #<!-- -->0

; Function Attrs: nocallback nofree nosync nounwind willreturn
declare void @<!-- -->llvm.va_end(ptr) #<!-- -->0

attributes #<!-- -->0 = { nocallback nofree nosync nounwind willreturn }
```

The crash backtrace from llc:
```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.      Program arguments: ../llvm-21/LLVM-21.1.0-Linux-X64/bin/llc main.ll
1.      Running pass 'Function Pass Manager' on module 'main.ll'.
2.      Running pass 'X86 DAG-&gt;DAG Instruction Selection' on function '@<!-- -->test'
 #<!-- -->0 0x0000598099ad0ce7 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (../llvm-21/LLVM-21.1.0-Linux-X64/bin/llc+0x62d6ce7)
 #<!-- -->1 0x0000598099ad14db SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->2 0x000075e1b9565050 (/lib/x86_64-linux-gnu/libc.so.6+0x3c050)
 #<!-- -->3 0x0000598096f8bd1e llvm::DataLayout::getTypeSizeInBits(llvm::Type*) const AArch64ISelLowering.cpp:0:0
 #<!-- -->4 0x0000598098fbf52f llvm::X86TargetLowering::LowerVAARG(llvm::SDValue, llvm::SelectionDAG&amp;) const X86ISelLowering.cpp:0:0
 #<!-- -->5 0x00005980973a597f (anonymous namespace)::SelectionDAGLegalize::LegalizeOp(llvm::SDNode*) (.llvm.6546466950859915230) LegalizeDAG.cpp:0:0
 #<!-- -->6 0x00005980973a4cd3 llvm::SelectionDAG::Legalize() (../llvm-21/LLVM-21.1.0-Linux-X64/bin/llc+0x3baacd3)
 #<!-- -->7 0x00005980973a094e llvm::SelectionDAGISel::CodeGenAndEmitDAG() (../llvm-21/LLVM-21.1.0-Linux-X64/bin/llc+0x3ba694e)
 #<!-- -->8 0x0000598097165e71 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&amp;) (../llvm-21/LLVM-21.1.0-Linux-X64/bin/llc+0x396be71)
 #<!-- -->9 0x0000598097b2aa8e llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&amp;) (../llvm-21/LLVM-21.1.0-Linux-X64/bin/llc+0x4330a8e)
#<!-- -->10 0x0000598097a26df1 llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&amp;) (../llvm-21/LLVM-21.1.0-Linux-X64/bin/llc+0x422cdf1)
#<!-- -->11 0x0000598097b01687 llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (../llvm-21/LLVM-21.1.0-Linux-X64/bin/llc+0x4307687)
#<!-- -->12 0x000059809784be2a llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (../llvm-21/LLVM-21.1.0-Linux-X64/bin/llc+0x4051e2a)
#<!-- -->13 0x000059809784b768 llvm::FPPassManager::runOnModule(llvm::Module&amp;) (../llvm-21/LLVM-21.1.0-Linux-X64/bin/llc+0x4051768)
#<!-- -->14 0x0000598097848023 llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (../llvm-21/LLVM-21.1.0-Linux-X64/bin/llc+0x404e023)
#<!-- -->15 0x0000598098252626 main (../llvm-21/LLVM-21.1.0-Linux-X64/bin/llc+0x4a58626)
#<!-- -->16 0x000075e1b955024a __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->17 0x000075e1b9550305 call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->18 0x000075e1b9550305 __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->19 0x00005980980fbba5 _start (../llvm-21/LLVM-21.1.0-Linux-X64/bin/llc+0x4901ba5)
Segmentation fault (core dumped)
```

And the original C code for reference:
```c
struct S {
    int a;
};

void test(int n, ...) {
    __builtin_va_list args;
    __builtin_va_start(args, n);
    struct S s = __builtin_va_arg(args, struct S);
    __builtin_va_end(args);
}
```

I've tested this with llvm-19 and llvm-21. I don't have llvm checked out locally, but I also tested it against the trunk version of llc in godbolt and the crash still occurrs so the problem appears to still be present in the latest development version: https://godbolt.org/z/7o8j9v4cs.
</details>


---

### Comment 2 - aHeraud

The [documentation](https://llvm.org/docs/LangRef.html#id345) mentions this:

> Note that the code generator does not yet fully support va_arg on many targets. Also, it does not currently support va_arg with aggregate types on any target.

So I think all that's missing here may be some validation that causes llc to error out when you pass an aggregate type to `va_arg`.

---

