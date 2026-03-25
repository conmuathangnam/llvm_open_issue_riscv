# Internal LLVM crash when using source-based profiling on a file.

**Author:** markww
**URL:** https://github.com/llvm/llvm-project/issues/169140
**Status:** Closed
**Labels:** clang:codegen, obsolete, crash
**Closed Date:** 2025-11-22T02:03:59Z

## Body

With Clang 15, I get an internal crash in LLVM when compiling this file for source-based coverage:

```C++
#include <vector>

bool unknown_bool();

template <typename RawValT>
class SomeClass {
 public:
    static std::vector<RawValT> get_test_values() {
        return unknown_bool() ? std::vector<RawValT>{RawValT{}} : std::vector<RawValT>{RawValT{}};
    }
};
void func() { SomeClass<unsigned short>::get_test_values(); }
```

(This is as minimal of a reproducer as I could muster).

Here is the stack dump:

```
Stack dump:
0.      Program arguments: clang-15 -std=c++17 -fprofile-instr-generate -fcoverage-mapping -c reproducer.cc -o reproducer.o
1.      <eof> parser at end of file
2.      Per-file LLVM IR generation
3.      reproducer.cc:9:33: Generating code for declaration 'SomeClass<unsigned short>::get_test_values'
 #0 0x00007f4ed3d043b1 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/usr/lib/llvm-15/bin/../lib/libLLVM-15.so.1+0xf043b1)
 #1 0x00007f4ed3d020fe llvm::sys::RunSignalHandlers() (/usr/lib/llvm-15/bin/../lib/libLLVM-15.so.1+0xf020fe)
 #2 0x00007f4ed3d03771 llvm::sys::CleanupOnSignal(unsigned long) (/usr/lib/llvm-15/bin/../lib/libLLVM-15.so.1+0xf03771)
 #3 0x00007f4ed3c2745a (/usr/lib/llvm-15/bin/../lib/libLLVM-15.so.1+0xe2745a)
 #4 0x00007f4ed3c273fb (/usr/lib/llvm-15/bin/../lib/libLLVM-15.so.1+0xe273fb)
 #5 0x00007f4ed3cfe5c7 llvm::sys::Process::Exit(int, bool) (/usr/lib/llvm-15/bin/../lib/libLLVM-15.so.1+0xefe5c7)
 #6 0x000055c1e1e12012 (/usr/lib/llvm-15/bin/clang+0x14012)
 #7 0x00007f4ed3c39339 llvm::report_fatal_error(llvm::Twine const&, bool) (/usr/lib/llvm-15/bin/../lib/libLLVM-15.so.1+0xe39339)
 #8 0x00007f4ed3c39226 (/usr/lib/llvm-15/bin/../lib/libLLVM-15.so.1+0xe39226)
 #9 0x00007f4edbb19d74 (/usr/lib/llvm-15/bin/../lib/libclang-cpp.so.15+0x1d19d74)
#10 0x00007f4edbb18220 (/usr/lib/llvm-15/bin/../lib/libclang-cpp.so.15+0x1d18220)
#11 0x00007f4edbb1ee1d (/usr/lib/llvm-15/bin/../lib/libclang-cpp.so.15+0x1d1ee1d)
#12 0x00007f4edbb1e0a0 (/usr/lib/llvm-15/bin/../lib/libclang-cpp.so.15+0x1d1e0a0)
#13 0x00007f4edbb1bce1 (/usr/lib/llvm-15/bin/../lib/libclang-cpp.so.15+0x1d1bce1)
#14 0x00007f4edbb1e0a0 (/usr/lib/llvm-15/bin/../lib/libclang-cpp.so.15+0x1d1e0a0)
#15 0x00007f4edbb1820a (/usr/lib/llvm-15/bin/../lib/libclang-cpp.so.15+0x1d1820a)
#16 0x00007f4edbb167fd clang::CodeGen::CoverageMappingGen::emitCounterMapping(clang::Decl const*, llvm::raw_ostream&) (/usr/lib/llvm-15/bin/../lib/libclang-cpp.so.15+0x1d167fd)
#17 0x00007f4edbae0693 clang::CodeGen::CodeGenPGO::emitCounterRegionMapping(clang::Decl const*) (/usr/lib/llvm-15/bin/../lib/libclang-cpp.so.15+0x1ce0693)
#18 0x00007f4edbae02c7 clang::CodeGen::CodeGenPGO::assignRegionCounters(clang::GlobalDecl, llvm::Function*) (/usr/lib/llvm-15/bin/../lib/libclang-cpp.so.15+0x1ce02c7)
#19 0x00007f4edba8c70e clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&) (/usr/lib/llvm-15/bin/../lib/libclang-cpp.so.15+0x1c8c70e)
#20 0x00007f4edbaaaca1 clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/usr/lib/llvm-15/bin/../lib/libclang-cpp.so.15+0x1caaca1)
#21 0x00007f4edbaa454b clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/usr/lib/llvm-15/bin/../lib/libclang-cpp.so.15+0x1ca454b)
#22 0x00007f4edba98b61 clang::CodeGen::CodeGenModule::EmitDeferred() (/usr/lib/llvm-15/bin/../lib/libclang-cpp.so.15+0x1c98b61)
#23 0x00007f4edba96a10 clang::CodeGen::CodeGenModule::Release() (/usr/lib/llvm-15/bin/../lib/libclang-cpp.so.15+0x1c96a10)
#24 0x00007f4edbb4b254 (/usr/lib/llvm-15/bin/../lib/libclang-cpp.so.15+0x1d4b254)
#25 0x00007f4edba82263 (/usr/lib/llvm-15/bin/../lib/libclang-cpp.so.15+0x1c82263)
#26 0x00007f4eda88ce9b clang::ParseAST(clang::Sema&, bool, bool) (/usr/lib/llvm-15/bin/../lib/libclang-cpp.so.15+0xa8ce9b)
#27 0x00007f4edba7e925 clang::CodeGenAction::ExecuteAction() (/usr/lib/llvm-15/bin/../lib/libclang-cpp.so.15+0x1c7e925)
#28 0x00007f4edc4b4ea7 clang::FrontendAction::Execute() (/usr/lib/llvm-15/bin/../lib/libclang-cpp.so.15+0x26b4ea7)
#29 0x00007f4edc427fb6 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/usr/lib/llvm-15/bin/../lib/libclang-cpp.so.15+0x2627fb6)
#30 0x00007f4edc532e4a clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/lib/llvm-15/bin/../lib/libclang-cpp.so.15+0x2732e4a)
#31 0x000055c1e1e119e0 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/usr/lib/llvm-15/bin/clang+0x139e0)
#32 0x000055c1e1e0fbab (/usr/lib/llvm-15/bin/clang+0x11bab)
#33 0x00007f4edc08bbd2 (/usr/lib/llvm-15/bin/../lib/libclang-cpp.so.15+0x228bbd2)
#34 0x00007f4ed3c273de llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/usr/lib/llvm-15/bin/../lib/libLLVM-15.so.1+0xe273de)
#35 0x00007f4edc08b698 clang::driver::CC1Command::Execute(llvm::ArrayRef<llvm::Optional<llvm::StringRef> >, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >*, bool*) const (/usr/lib/llvm-15/bin/../lib/libclang-cpp.so.15+0x228b698)
#36 0x00007f4edc04da8a clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/usr/lib/llvm-15/bin/../lib/libclang-cpp.so.15+0x224da8a)
#37 0x00007f4edc04dcde clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*> >&, bool) const (/usr/lib/llvm-15/bin/../lib/libclang-cpp.so.15+0x224dcde)
#38 0x00007f4edc06ab1f clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*> >&) (/usr/lib/llvm-15/bin/../lib/libclang-cpp.so.15+0x226ab1f)
#39 0x000055c1e1e0f311 clang_main(int, char**) (/usr/lib/llvm-15/bin/clang+0x11311)
#40 0x00007f4ed2629d90 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:58:16
#41 0x00007f4ed2629e40 call_init ./csu/../csu/libc-start.c:128:20
#42 0x00007f4ed2629e40 __libc_start_main ./csu/../csu/libc-start.c:379:5
#43 0x000055c1e1e0c3b5 _start (/usr/lib/llvm-15/bin/clang+0xe3b5)
clang: error: clang frontend command failed with exit code 70 (use -v to see invocation)
Ubuntu clang version 15.0.7
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
clang: note: diagnostic msg:
********************
```

## Comments

### Comment 1 - markww

Here are the additional files I was supposed to attach:

[reproducer-6a8fb6.cpp](https://github.com/user-attachments/files/23686250/reproducer-6a8fb6.cpp)

[reproducer-6a8fb6.sh](https://github.com/user-attachments/files/23686253/reproducer-6a8fb6.sh)

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: Mark Winterrowd (markww)

<details>
With Clang 15, I get an internal crash in LLVM when compiling this file for source-based coverage:

```C++
#include &lt;vector&gt;

bool unknown_bool();

template &lt;typename RawValT&gt;
class SomeClass {
 public:
    static std::vector&lt;RawValT&gt; get_test_values() {
        return unknown_bool() ? std::vector&lt;RawValT&gt;{RawValT{}} : std::vector&lt;RawValT&gt;{RawValT{}};
    }
};
void func() { SomeClass&lt;unsigned short&gt;::get_test_values(); }
```

(This is as minimal of a reproducer as I could muster).

Here is the stack dump:

```
Stack dump:
0.      Program arguments: clang-15 -std=c++17 -fprofile-instr-generate -fcoverage-mapping -c reproducer.cc -o reproducer.o
1.      &lt;eof&gt; parser at end of file
2.      Per-file LLVM IR generation
3.      reproducer.cc:9:33: Generating code for declaration 'SomeClass&lt;unsigned short&gt;::get_test_values'
 #<!-- -->0 0x00007f4ed3d043b1 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/usr/lib/llvm-15/bin/../lib/libLLVM-15.so.1+0xf043b1)
 #<!-- -->1 0x00007f4ed3d020fe llvm::sys::RunSignalHandlers() (/usr/lib/llvm-15/bin/../lib/libLLVM-15.so.1+0xf020fe)
 #<!-- -->2 0x00007f4ed3d03771 llvm::sys::CleanupOnSignal(unsigned long) (/usr/lib/llvm-15/bin/../lib/libLLVM-15.so.1+0xf03771)
 #<!-- -->3 0x00007f4ed3c2745a (/usr/lib/llvm-15/bin/../lib/libLLVM-15.so.1+0xe2745a)
 #<!-- -->4 0x00007f4ed3c273fb (/usr/lib/llvm-15/bin/../lib/libLLVM-15.so.1+0xe273fb)
 #<!-- -->5 0x00007f4ed3cfe5c7 llvm::sys::Process::Exit(int, bool) (/usr/lib/llvm-15/bin/../lib/libLLVM-15.so.1+0xefe5c7)
 #<!-- -->6 0x000055c1e1e12012 (/usr/lib/llvm-15/bin/clang+0x14012)
 #<!-- -->7 0x00007f4ed3c39339 llvm::report_fatal_error(llvm::Twine const&amp;, bool) (/usr/lib/llvm-15/bin/../lib/libLLVM-15.so.1+0xe39339)
 #<!-- -->8 0x00007f4ed3c39226 (/usr/lib/llvm-15/bin/../lib/libLLVM-15.so.1+0xe39226)
 #<!-- -->9 0x00007f4edbb19d74 (/usr/lib/llvm-15/bin/../lib/libclang-cpp.so.15+0x1d19d74)
#<!-- -->10 0x00007f4edbb18220 (/usr/lib/llvm-15/bin/../lib/libclang-cpp.so.15+0x1d18220)
#<!-- -->11 0x00007f4edbb1ee1d (/usr/lib/llvm-15/bin/../lib/libclang-cpp.so.15+0x1d1ee1d)
#<!-- -->12 0x00007f4edbb1e0a0 (/usr/lib/llvm-15/bin/../lib/libclang-cpp.so.15+0x1d1e0a0)
#<!-- -->13 0x00007f4edbb1bce1 (/usr/lib/llvm-15/bin/../lib/libclang-cpp.so.15+0x1d1bce1)
#<!-- -->14 0x00007f4edbb1e0a0 (/usr/lib/llvm-15/bin/../lib/libclang-cpp.so.15+0x1d1e0a0)
#<!-- -->15 0x00007f4edbb1820a (/usr/lib/llvm-15/bin/../lib/libclang-cpp.so.15+0x1d1820a)
#<!-- -->16 0x00007f4edbb167fd clang::CodeGen::CoverageMappingGen::emitCounterMapping(clang::Decl const*, llvm::raw_ostream&amp;) (/usr/lib/llvm-15/bin/../lib/libclang-cpp.so.15+0x1d167fd)
#<!-- -->17 0x00007f4edbae0693 clang::CodeGen::CodeGenPGO::emitCounterRegionMapping(clang::Decl const*) (/usr/lib/llvm-15/bin/../lib/libclang-cpp.so.15+0x1ce0693)
#<!-- -->18 0x00007f4edbae02c7 clang::CodeGen::CodeGenPGO::assignRegionCounters(clang::GlobalDecl, llvm::Function*) (/usr/lib/llvm-15/bin/../lib/libclang-cpp.so.15+0x1ce02c7)
#<!-- -->19 0x00007f4edba8c70e clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&amp;) (/usr/lib/llvm-15/bin/../lib/libclang-cpp.so.15+0x1c8c70e)
#<!-- -->20 0x00007f4edbaaaca1 clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/usr/lib/llvm-15/bin/../lib/libclang-cpp.so.15+0x1caaca1)
#<!-- -->21 0x00007f4edbaa454b clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/usr/lib/llvm-15/bin/../lib/libclang-cpp.so.15+0x1ca454b)
#<!-- -->22 0x00007f4edba98b61 clang::CodeGen::CodeGenModule::EmitDeferred() (/usr/lib/llvm-15/bin/../lib/libclang-cpp.so.15+0x1c98b61)
#<!-- -->23 0x00007f4edba96a10 clang::CodeGen::CodeGenModule::Release() (/usr/lib/llvm-15/bin/../lib/libclang-cpp.so.15+0x1c96a10)
#<!-- -->24 0x00007f4edbb4b254 (/usr/lib/llvm-15/bin/../lib/libclang-cpp.so.15+0x1d4b254)
#<!-- -->25 0x00007f4edba82263 (/usr/lib/llvm-15/bin/../lib/libclang-cpp.so.15+0x1c82263)
#<!-- -->26 0x00007f4eda88ce9b clang::ParseAST(clang::Sema&amp;, bool, bool) (/usr/lib/llvm-15/bin/../lib/libclang-cpp.so.15+0xa8ce9b)
#<!-- -->27 0x00007f4edba7e925 clang::CodeGenAction::ExecuteAction() (/usr/lib/llvm-15/bin/../lib/libclang-cpp.so.15+0x1c7e925)
#<!-- -->28 0x00007f4edc4b4ea7 clang::FrontendAction::Execute() (/usr/lib/llvm-15/bin/../lib/libclang-cpp.so.15+0x26b4ea7)
#<!-- -->29 0x00007f4edc427fb6 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/usr/lib/llvm-15/bin/../lib/libclang-cpp.so.15+0x2627fb6)
#<!-- -->30 0x00007f4edc532e4a clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/lib/llvm-15/bin/../lib/libclang-cpp.so.15+0x2732e4a)
#<!-- -->31 0x000055c1e1e119e0 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/usr/lib/llvm-15/bin/clang+0x139e0)
#<!-- -->32 0x000055c1e1e0fbab (/usr/lib/llvm-15/bin/clang+0x11bab)
#<!-- -->33 0x00007f4edc08bbd2 (/usr/lib/llvm-15/bin/../lib/libclang-cpp.so.15+0x228bbd2)
#<!-- -->34 0x00007f4ed3c273de llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/usr/lib/llvm-15/bin/../lib/libLLVM-15.so.1+0xe273de)
#<!-- -->35 0x00007f4edc08b698 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;llvm::Optional&lt;llvm::StringRef&gt; &gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt; &gt;*, bool*) const (/usr/lib/llvm-15/bin/../lib/libclang-cpp.so.15+0x228b698)
#<!-- -->36 0x00007f4edc04da8a clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/usr/lib/llvm-15/bin/../lib/libclang-cpp.so.15+0x224da8a)
#<!-- -->37 0x00007f4edc04dcde clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt; &gt;&amp;, bool) const (/usr/lib/llvm-15/bin/../lib/libclang-cpp.so.15+0x224dcde)
#<!-- -->38 0x00007f4edc06ab1f clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt; &gt;&amp;) (/usr/lib/llvm-15/bin/../lib/libclang-cpp.so.15+0x226ab1f)
#<!-- -->39 0x000055c1e1e0f311 clang_main(int, char**) (/usr/lib/llvm-15/bin/clang+0x11311)
#<!-- -->40 0x00007f4ed2629d90 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:58:16
#<!-- -->41 0x00007f4ed2629e40 call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->42 0x00007f4ed2629e40 __libc_start_main ./csu/../csu/libc-start.c:379:5
#<!-- -->43 0x000055c1e1e0c3b5 _start (/usr/lib/llvm-15/bin/clang+0xe3b5)
clang: error: clang frontend command failed with exit code 70 (use -v to see invocation)
Ubuntu clang version 15.0.7
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
clang: note: diagnostic msg:
********************
```
</details>


---

### Comment 3 - EugeneZelenko

Could you please try 21 or `main` branch? https://godbolt.org should be helpful. Only most recent release is maintained.

---

### Comment 4 - markww

According to Godbolt, this was fixed sometime before 17.0.1 . I was last able to get it to fail [using 16](https://godbolt.org/z/5748hhq67)

---

