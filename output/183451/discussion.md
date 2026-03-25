# clang crashes with #pragma omp tile and -fopenmp-simd

**Author:** chillenb
**URL:** https://github.com/llvm/llvm-project/issues/183451
**Status:** Open
**Labels:** clang:codegen, crash, clang:openmp

## Body

I was messing around with the new openmp features and encountered a bug. Clang crashes trying to compile the following code with `-fopenmp-simd`.

What's the correct behavior here? Should the loop transformation be enabled because it doesn't need the runtime library, or should it be disabled because it's not a SIMD feature?
(GCC 15.2 enables it, icx 2025 disables it with both -qopenmp-simd and -fopenmp-simd)


```c
void dowork(double *a, int n) {
    #pragma omp tile sizes(2,2)
    for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++) {
            a[i*n+j] *= 2.0;
        }
}
```

Link: https://godbolt.org/z/hzqn3Pnq4

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-21.1.0/bin/clang -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-15.2.0 -fcolor-diagnostics -fno-crash-diagnostics -fopenmp-simd <source>
1.	<eof> parser at end of file
2.	<source>:1:6: LLVM IR generation of declaration 'dowork'
3.	<source>:1:6: Generating code for declaration 'dowork'
 #0 0x0000000003bfa6c8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x3bfa6c8)
 #1 0x0000000003bf809c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x3bf809c)
 #2 0x0000000003b483f8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007f9157042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00000000076bc570 clang::CapturedStmt::Capture::getCaptureKind() const (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x76bc570)
 #5 0x00000000040ea793 clang::CodeGen::CodeGenFunction::EmitSimpleOMPExecutableDirective(clang::OMPExecutableDirective const&) (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x40ea793)
 #6 0x00000000040ad402 clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef<clang::Attr const*>) (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x40ad402)
 #7 0x00000000040b48f9 clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&, bool, clang::CodeGen::AggValueSlot) (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x40b48f9)
 #8 0x00000000041166f5 clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x41166f5)
 #9 0x000000000412a183 clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&) (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x412a183)
#10 0x0000000004184b2b clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x4184b2b)
#11 0x00000000041805b4 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x41805b4)
#12 0x0000000004180eb3 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x4180eb3)
#13 0x000000000418aa2f clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#14 0x000000000454c6e0 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#15 0x00000000045363f9 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x45363f9)
#16 0x000000000618ef64 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x618ef64)
#17 0x000000000454985d clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x454985d)
#18 0x000000000485b83a clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x485b83a)
#19 0x00000000047d655b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x47d655b)
#20 0x000000000494e513 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x494e513)
#21 0x0000000000da5f25 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-21.1.0/bin/clang+0xda5f25)
#22 0x0000000000d9de2d ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#23 0x00000000045bb869 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#24 0x0000000003b48813 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x3b48813)
#25 0x00000000045bba89 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#26 0x000000000457eddd clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x457eddd)
#27 0x000000000457fd91 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x457fd91)
#28 0x000000000458d29c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x458d29c)
#29 0x0000000000da2751 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-21.1.0/bin/clang+0xda2751)
#30 0x0000000000c526f4 main (/opt/compiler-explorer/clang-21.1.0/bin/clang+0xc526f4)
#31 0x00007f9157029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#32 0x00007f9157029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#33 0x0000000000d9d8c5 _start (/opt/compiler-explorer/clang-21.1.0/bin/clang+0xd9d8c5)
clang: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: Christopher Hillenbrand (chillenb)

<details>
I was messing around with the new openmp features and encountered a bug. Clang crashes trying to compile the following code with `-fopenmp-simd`.

What's the correct behavior here? Should the loop transformation be enabled because it doesn't need the runtime library, or should it be disabled because it's not a SIMD feature?


```c
void dowork(double *a, int n) {
    #pragma omp tile sizes(2,2)
    for(int i = 0; i &lt; n; i++)
        for(int j = 0; j &lt; n; j++) {
            a[i*n+j] *= 2.0;
        }
}
```

Link: https://godbolt.org/z/hzqn3Pnq4

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-21.1.0/bin/clang -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-15.2.0 -fcolor-diagnostics -fno-crash-diagnostics -fopenmp-simd &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	&lt;source&gt;:1:6: LLVM IR generation of declaration 'dowork'
3.	&lt;source&gt;:1:6: Generating code for declaration 'dowork'
 #<!-- -->0 0x0000000003bfa6c8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x3bfa6c8)
 #<!-- -->1 0x0000000003bf809c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x3bf809c)
 #<!-- -->2 0x0000000003b483f8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007f9157042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00000000076bc570 clang::CapturedStmt::Capture::getCaptureKind() const (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x76bc570)
 #<!-- -->5 0x00000000040ea793 clang::CodeGen::CodeGenFunction::EmitSimpleOMPExecutableDirective(clang::OMPExecutableDirective const&amp;) (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x40ea793)
 #<!-- -->6 0x00000000040ad402 clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef&lt;clang::Attr const*&gt;) (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x40ad402)
 #<!-- -->7 0x00000000040b48f9 clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&amp;, bool, clang::CodeGen::AggValueSlot) (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x40b48f9)
 #<!-- -->8 0x00000000041166f5 clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x41166f5)
 #<!-- -->9 0x000000000412a183 clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&amp;) (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x412a183)
#<!-- -->10 0x0000000004184b2b clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x4184b2b)
#<!-- -->11 0x00000000041805b4 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x41805b4)
#<!-- -->12 0x0000000004180eb3 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x4180eb3)
#<!-- -->13 0x000000000418aa2f clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#<!-- -->14 0x000000000454c6e0 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#<!-- -->15 0x00000000045363f9 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x45363f9)
#<!-- -->16 0x000000000618ef64 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x618ef64)
#<!-- -->17 0x000000000454985d clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x454985d)
#<!-- -->18 0x000000000485b83a clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x485b83a)
#<!-- -->19 0x00000000047d655b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x47d655b)
#<!-- -->20 0x000000000494e513 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x494e513)
#<!-- -->21 0x0000000000da5f25 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-21.1.0/bin/clang+0xda5f25)
#<!-- -->22 0x0000000000d9de2d ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->23 0x00000000045bb869 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->24 0x0000000003b48813 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x3b48813)
#<!-- -->25 0x00000000045bba89 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->26 0x000000000457eddd clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x457eddd)
#<!-- -->27 0x000000000457fd91 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x457fd91)
#<!-- -->28 0x000000000458d29c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x458d29c)
#<!-- -->29 0x0000000000da2751 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-21.1.0/bin/clang+0xda2751)
#<!-- -->30 0x0000000000c526f4 main (/opt/compiler-explorer/clang-21.1.0/bin/clang+0xc526f4)
#<!-- -->31 0x00007f9157029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->32 0x00007f9157029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->33 0x0000000000d9d8c5 _start (/opt/compiler-explorer/clang-21.1.0/bin/clang+0xd9d8c5)
clang: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```
</details>


---

### Comment 2 - shafik

CC @zahiraam 

---

### Comment 3 - chillenb

Similar behavior happens with reverse, interchange, and stripe

---

### Comment 4 - zahiraam

Looking at the optimization report for this particular case, the `Intel` doesn’t not tile the loop when `-f/qopenmp-simd` is used but it doesn’t vectorize it either. But then again it doesn’t vectorize it even without the `#pragma`.

In general, `#pragma omp tile` with `-fopenmp-simd` can be problematic, because `tile` is more aligned with thread-level parallelism, while `SIMD` requires vectorized execution of individual loop iterations. There is no clear, standard behavior here because the `tile` pragma and `SIMD` vectorization are not typically used together.

Not sure what clang should be doing, but it should certainly not crash. 
I will assign the issue to myself and make some investigation to understand what the `Intel` compiler is actually doing in the general case.


---

