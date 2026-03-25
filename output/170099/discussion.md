# clang: crash in LazyCallGraph::visitReferences (using modules)

**Author:** adityasz
**URL:** https://github.com/llvm/llvm-project/issues/170099
**Status:** Closed
**Labels:** clang:modules, crash
**Closed Date:** 2026-03-16T09:05:36Z

## Body

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /usr/bin/clang-21 -cc1 -triple x86_64-redhat-linux-gnu -O3 -emit-llvm-bc -flto=full -flto-unit -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name Utils.ixx -mrelocation-model pic -pic-level 2 -fhalf-no-semantic-interposition -mframe-pointer=none -menable-no-infs -menable-no-nans -fapprox-func -funsafe-math-optimizations -fno-signed-zeros -mreassociate -freciprocal-math -ffp-contract=fast -fno-rounding-math -ffast-math -ffinite-math-only -mfpmath sse -complex-range=basic -mconstructor-aliases -funwind-tables=2 -target-cpu alderlake -target-feature +prfchw -target-feature -cldemote -target-feature +avx -target-feature +aes -target-feature +sahf -target-feature +pclmul -target-feature -xop -target-feature +crc32 -target-feature -amx-fp8 -target-feature +xsaves -target-feature -avx512fp16 -target-feature -usermsr -target-feature -sm4 -target-feature -egpr -target-feature +sse4.1 -target-feature -avx512ifma -target-feature +xsave -target-feature +sse4.2 -target-feature -tsxldtrk -target-feature -sm3 -target-feature +ptwrite -target-feature +widekl -target-feature -movrs -target-feature +invpcid -target-feature +64bit -target-feature +xsavec -target-feature -avx10.1-512 -target-feature -avx512vpopcntdq -target-feature +cmov -target-feature -avx512vp2intersect -target-feature -avx512cd -target-feature +movbe -target-feature -avxvnniint8 -target-feature -ccmp -target-feature -amx-int8 -target-feature +kl -target-feature -avx10.1-256 -target-feature -sha512 -target-feature +avxvnni -target-feature -rtm -target-feature +adx -target-feature +avx2 -target-feature +hreset -target-feature +movdiri -target-feature +serialize -target-feature +vpclmulqdq -target-feature -avx512vl -target-feature -uintr -target-feature -cf -target-feature +clflushopt -target-feature -raoint -target-feature -cmpccxadd -target-feature +bmi -target-feature -amx-tile -target-feature -avx10.2-256 -target-feature +gfni -target-feature -avxvnniint16 -target-feature -amx-fp16 -target-feature -zu -target-feature -ndd -target-feature +xsaveopt -target-feature +rdrnd -target-feature -avx512f -target-feature -amx-bf16 -target-feature -avx512bf16 -target-feature -avx512vnni -target-feature -push2pop2 -target-feature +cx8 -target-feature -avx512bw -target-feature +sse3 -target-feature +pku -target-feature -nf -target-feature -amx-tf32 -target-feature -amx-avx512 -target-feature +fsgsbase -target-feature -clzero -target-feature -mwaitx -target-feature -lwp -target-feature +lzcnt -target-feature +sha -target-feature +movdir64b -target-feature -ppx -target-feature -wbnoinvd -target-feature -enqcmd -target-feature -amx-transpose -target-feature -avx10.2-512 -target-feature -avxneconvert -target-feature -tbm -target-feature -pconfig -target-feature -amx-complex -target-feature +ssse3 -target-feature +cx16 -target-feature +bmi2 -target-feature +fma -target-feature +popcnt -target-feature -avxifma -target-feature +f16c -target-feature -avx512bitalg -target-feature -rdpru -target-feature +clwb -target-feature +mmx -target-feature +rdseed -target-feature -avx512vbmi2 -target-feature -prefetchi -target-feature -amx-movrs -target-feature +rdpid -target-feature -fma4 -target-feature -avx512vbmi -target-feature +shstk -target-feature +vaes -target-feature +waitpkg -target-feature -sgx -target-feature +fxsr -target-feature -avx512dq -target-feature -sse4a -target-feature +sse -target-feature +sse2 -debugger-tuning=gdb -fdebug-compilation-dir=/home/aditya/Projects/hyprland/wm/build/release -fcoverage-compilation-dir=/home/aditya/Projects/hyprland/wm/build/release -resource-dir /usr/bin/../lib/clang/21 -Wall -Wpedantic -Wextra -std=gnu++26 -fdeprecated-macro -ferror-limit 19 -pthread -fgnuc-version=4.2.1 -fno-implicit-modules -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -vectorize-loops -vectorize-slp -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o lib/Support/CMakeFiles/Support.dir/__/__/modules/Support/Utils.ixx.o -x pcm lib/Support/CMakeFiles/Support.dir/wm.Support.Utils.pcm
1.	<eof> parser at end of file
2.	Optimizer
3.	Running pass "rpo-function-attrs" on module "lib/Support/CMakeFiles/Support.dir/wm.Support.Utils.pcm"
 #0 0x00007f198facc869 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/lib64/libLLVM.so.21.1+0x50cc869)
 #1 0x00007f198facd2ad (/lib64/libLLVM.so.21.1+0x50cd2ad)
 #2 0x00007f198a4262c0 __restore_rt (/lib64/libc.so.6+0x1a2c0)
 #3 0x00007f198edb6012 llvm::LazyCallGraph::visitReferences(llvm::SmallVectorImpl<llvm::Constant*>&, llvm::SmallPtrSetImpl<llvm::Constant*>&, llvm::function_ref<void (llvm::Function&)>) (/lib64/libLLVM.so.21.1+0x43b6012)
 #4 0x00007f198edb52a4 llvm::LazyCallGraph::LazyCallGraph(llvm::Module&, llvm::function_ref<llvm::TargetLibraryInfo& (llvm::Function&)>) (/lib64/libLLVM.so.21.1+0x43b52a4)
 #5 0x00007f198f77e6a5 (/lib64/libLLVM.so.21.1+0x4d7e6a5)
 #6 0x00007f198f77cf01 llvm::AnalysisManager<llvm::Module>::getResultImpl(llvm::AnalysisKey*, llvm::Module&) (/lib64/libLLVM.so.21.1+0x4d7cf01)
 #7 0x00007f198f77d78a llvm::ReversePostOrderFunctionAttrsPass::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/lib64/libLLVM.so.21.1+0x4d7d78a)
 #8 0x00007f198f77d741 (/lib64/libLLVM.so.21.1+0x4d7d741)
 #9 0x00007f198eb808e7 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/lib64/libLLVM.so.21.1+0x41808e7)
#10 0x00007f199525b1fd (/lib64/libclang-cpp.so.21.1+0x1e5b1fd)
#11 0x00007f19952565fa clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/lib64/libclang-cpp.so.21.1+0x1e565fa)
#12 0x00007f19952545a6 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/lib64/libclang-cpp.so.21.1+0x1e545a6)
#13 0x00007f199490c712 clang::ParseAST(clang::Sema&, bool, bool) (/lib64/libclang-cpp.so.21.1+0x150c712)
#14 0x00007f19951c8fff clang::FrontendAction::Execute() (/lib64/libclang-cpp.so.21.1+0x1dc8fff)
#15 0x00007f19951c8b10 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/lib64/libclang-cpp.so.21.1+0x1dc8b10)
#16 0x00007f19951c84b5 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/lib64/libclang-cpp.so.21.1+0x1dc84b5)
#17 0x000055f02f7268d6 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/usr/bin/clang-21+0x198d6)
#18 0x000055f02f71da4e (/usr/bin/clang-21+0x10a4e)
#19 0x000055f02f71f534 clang_main(int, char**, llvm::ToolContext const&) (/usr/bin/clang-21+0x12534)
#20 0x000055f02f71e782 main (/usr/bin/clang-21+0x11782)
#21 0x00007f198a40f5b5 __libc_start_call_main (/lib64/libc.so.6+0x35b5)
#22 0x00007f198a40f668 __libc_start_main@GLIBC_2.2.5 (/lib64/libc.so.6+0x3668)
#23 0x000055f02f728f85 _start (/usr/bin/clang-21+0x1bf85)
clang++: error: unable to execute command: Segmentation fault (core dumped)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 21.1.6 (Fedora 21.1.6-1.fc43)
Target: x86_64-redhat-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
Configuration file: /etc/clang/x86_64-redhat-linux-gnu-clang++.cfg
clang++: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: /tmp/Utils-61a62a.cppm
clang++: note: diagnostic msg: /tmp/Utils-61a62a.sh
clang++: note: diagnostic msg: 

********************
```

`Utils-61a62a.cppm` was renamed because GitHub was not uploading it with the `.cppm` extension.

[Utils-61a62a.cpp](https://github.com/user-attachments/files/23851314/Utils-61a62a.cpp)
[Utils-61a62a.sh](https://github.com/user-attachments/files/23851303/Utils-61a62a.sh)

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-modules

Author: Aditya Singh (adityasz)

<details>
```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /usr/bin/clang-21 -cc1 -triple x86_64-redhat-linux-gnu -O3 -emit-llvm-bc -flto=full -flto-unit -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name Utils.ixx -mrelocation-model pic -pic-level 2 -fhalf-no-semantic-interposition -mframe-pointer=none -menable-no-infs -menable-no-nans -fapprox-func -funsafe-math-optimizations -fno-signed-zeros -mreassociate -freciprocal-math -ffp-contract=fast -fno-rounding-math -ffast-math -ffinite-math-only -mfpmath sse -complex-range=basic -mconstructor-aliases -funwind-tables=2 -target-cpu alderlake -target-feature +prfchw -target-feature -cldemote -target-feature +avx -target-feature +aes -target-feature +sahf -target-feature +pclmul -target-feature -xop -target-feature +crc32 -target-feature -amx-fp8 -target-feature +xsaves -target-feature -avx512fp16 -target-feature -usermsr -target-feature -sm4 -target-feature -egpr -target-feature +sse4.1 -target-feature -avx512ifma -target-feature +xsave -target-feature +sse4.2 -target-feature -tsxldtrk -target-feature -sm3 -target-feature +ptwrite -target-feature +widekl -target-feature -movrs -target-feature +invpcid -target-feature +64bit -target-feature +xsavec -target-feature -avx10.1-512 -target-feature -avx512vpopcntdq -target-feature +cmov -target-feature -avx512vp2intersect -target-feature -avx512cd -target-feature +movbe -target-feature -avxvnniint8 -target-feature -ccmp -target-feature -amx-int8 -target-feature +kl -target-feature -avx10.1-256 -target-feature -sha512 -target-feature +avxvnni -target-feature -rtm -target-feature +adx -target-feature +avx2 -target-feature +hreset -target-feature +movdiri -target-feature +serialize -target-feature +vpclmulqdq -target-feature -avx512vl -target-feature -uintr -target-feature -cf -target-feature +clflushopt -target-feature -raoint -target-feature -cmpccxadd -target-feature +bmi -target-feature -amx-tile -target-feature -avx10.2-256 -target-feature +gfni -target-feature -avxvnniint16 -target-feature -amx-fp16 -target-feature -zu -target-feature -ndd -target-feature +xsaveopt -target-feature +rdrnd -target-feature -avx512f -target-feature -amx-bf16 -target-feature -avx512bf16 -target-feature -avx512vnni -target-feature -push2pop2 -target-feature +cx8 -target-feature -avx512bw -target-feature +sse3 -target-feature +pku -target-feature -nf -target-feature -amx-tf32 -target-feature -amx-avx512 -target-feature +fsgsbase -target-feature -clzero -target-feature -mwaitx -target-feature -lwp -target-feature +lzcnt -target-feature +sha -target-feature +movdir64b -target-feature -ppx -target-feature -wbnoinvd -target-feature -enqcmd -target-feature -amx-transpose -target-feature -avx10.2-512 -target-feature -avxneconvert -target-feature -tbm -target-feature -pconfig -target-feature -amx-complex -target-feature +ssse3 -target-feature +cx16 -target-feature +bmi2 -target-feature +fma -target-feature +popcnt -target-feature -avxifma -target-feature +f16c -target-feature -avx512bitalg -target-feature -rdpru -target-feature +clwb -target-feature +mmx -target-feature +rdseed -target-feature -avx512vbmi2 -target-feature -prefetchi -target-feature -amx-movrs -target-feature +rdpid -target-feature -fma4 -target-feature -avx512vbmi -target-feature +shstk -target-feature +vaes -target-feature +waitpkg -target-feature -sgx -target-feature +fxsr -target-feature -avx512dq -target-feature -sse4a -target-feature +sse -target-feature +sse2 -debugger-tuning=gdb -fdebug-compilation-dir=/home/aditya/Projects/hyprland/wm/build/release -fcoverage-compilation-dir=/home/aditya/Projects/hyprland/wm/build/release -resource-dir /usr/bin/../lib/clang/21 -Wall -Wpedantic -Wextra -std=gnu++26 -fdeprecated-macro -ferror-limit 19 -pthread -fgnuc-version=4.2.1 -fno-implicit-modules -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -vectorize-loops -vectorize-slp -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o lib/Support/CMakeFiles/Support.dir/__/__/modules/Support/Utils.ixx.o -x pcm lib/Support/CMakeFiles/Support.dir/wm.Support.Utils.pcm
1.	&lt;eof&gt; parser at end of file
2.	Optimizer
3.	Running pass "rpo-function-attrs" on module "lib/Support/CMakeFiles/Support.dir/wm.Support.Utils.pcm"
 #<!-- -->0 0x00007f198facc869 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/lib64/libLLVM.so.21.1+0x50cc869)
 #<!-- -->1 0x00007f198facd2ad (/lib64/libLLVM.so.21.1+0x50cd2ad)
 #<!-- -->2 0x00007f198a4262c0 __restore_rt (/lib64/libc.so.6+0x1a2c0)
 #<!-- -->3 0x00007f198edb6012 llvm::LazyCallGraph::visitReferences(llvm::SmallVectorImpl&lt;llvm::Constant*&gt;&amp;, llvm::SmallPtrSetImpl&lt;llvm::Constant*&gt;&amp;, llvm::function_ref&lt;void (llvm::Function&amp;)&gt;) (/lib64/libLLVM.so.21.1+0x43b6012)
 #<!-- -->4 0x00007f198edb52a4 llvm::LazyCallGraph::LazyCallGraph(llvm::Module&amp;, llvm::function_ref&lt;llvm::TargetLibraryInfo&amp; (llvm::Function&amp;)&gt;) (/lib64/libLLVM.so.21.1+0x43b52a4)
 #<!-- -->5 0x00007f198f77e6a5 (/lib64/libLLVM.so.21.1+0x4d7e6a5)
 #<!-- -->6 0x00007f198f77cf01 llvm::AnalysisManager&lt;llvm::Module&gt;::getResultImpl(llvm::AnalysisKey*, llvm::Module&amp;) (/lib64/libLLVM.so.21.1+0x4d7cf01)
 #<!-- -->7 0x00007f198f77d78a llvm::ReversePostOrderFunctionAttrsPass::run(llvm::Module&amp;, llvm::AnalysisManager&lt;llvm::Module&gt;&amp;) (/lib64/libLLVM.so.21.1+0x4d7d78a)
 #<!-- -->8 0x00007f198f77d741 (/lib64/libLLVM.so.21.1+0x4d7d741)
 #<!-- -->9 0x00007f198eb808e7 llvm::PassManager&lt;llvm::Module, llvm::AnalysisManager&lt;llvm::Module&gt;&gt;::run(llvm::Module&amp;, llvm::AnalysisManager&lt;llvm::Module&gt;&amp;) (/lib64/libLLVM.so.21.1+0x41808e7)
#<!-- -->10 0x00007f199525b1fd (/lib64/libclang-cpp.so.21.1+0x1e5b1fd)
#<!-- -->11 0x00007f19952565fa clang::emitBackendOutput(clang::CompilerInstance&amp;, clang::CodeGenOptions&amp;, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;, std::unique_ptr&lt;llvm::raw_pwrite_stream, std::default_delete&lt;llvm::raw_pwrite_stream&gt;&gt;, clang::BackendConsumer*) (/lib64/libclang-cpp.so.21.1+0x1e565fa)
#<!-- -->12 0x00007f19952545a6 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&amp;) (/lib64/libclang-cpp.so.21.1+0x1e545a6)
#<!-- -->13 0x00007f199490c712 clang::ParseAST(clang::Sema&amp;, bool, bool) (/lib64/libclang-cpp.so.21.1+0x150c712)
#<!-- -->14 0x00007f19951c8fff clang::FrontendAction::Execute() (/lib64/libclang-cpp.so.21.1+0x1dc8fff)
#<!-- -->15 0x00007f19951c8b10 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/lib64/libclang-cpp.so.21.1+0x1dc8b10)
#<!-- -->16 0x00007f19951c84b5 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/lib64/libclang-cpp.so.21.1+0x1dc84b5)
#<!-- -->17 0x000055f02f7268d6 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/usr/bin/clang-21+0x198d6)
#<!-- -->18 0x000055f02f71da4e (/usr/bin/clang-21+0x10a4e)
#<!-- -->19 0x000055f02f71f534 clang_main(int, char**, llvm::ToolContext const&amp;) (/usr/bin/clang-21+0x12534)
#<!-- -->20 0x000055f02f71e782 main (/usr/bin/clang-21+0x11782)
#<!-- -->21 0x00007f198a40f5b5 __libc_start_call_main (/lib64/libc.so.6+0x35b5)
#<!-- -->22 0x00007f198a40f668 __libc_start_main@<!-- -->GLIBC_2.2.5 (/lib64/libc.so.6+0x3668)
#<!-- -->23 0x000055f02f728f85 _start (/usr/bin/clang-21+0x1bf85)
clang++: error: unable to execute command: Segmentation fault (core dumped)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 21.1.6 (Fedora 21.1.6-1.fc43)
Target: x86_64-redhat-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
Configuration file: /etc/clang/x86_64-redhat-linux-gnu-clang++.cfg
clang++: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: /tmp/Utils-61a62a.cppm
clang++: note: diagnostic msg: /tmp/Utils-61a62a.sh
clang++: note: diagnostic msg: 

********************
```

`Utils-61a62a.cppm` was renamed because GitHub was not uploading it with the `.cppm` extension.

[Utils-61a62a.cpp](https://github.com/user-attachments/files/23851314/Utils-61a62a.cpp)
[Utils-61a62a.sh](https://github.com/user-attachments/files/23851303/Utils-61a62a.sh)
</details>


---

### Comment 2 - adityasz

I added a (not minimal) example [here](https://github.com/adityasz/clang-170099/). (There are multiple [dependencies](https://github.com/adityasz/clang-170099/blob/7035886d552a165d17820a085e97862a17e4a64e/CMakeLists.txt#L36) and [`#include`](https://github.com/adityasz/clang-170099/blob/7035886d552a165d17820a085e97862a17e4a64e/include/Hyprland.h#L3)s. The problem is that the segfaults get rarer as I remove stuff, so it will take a long time to find a trivial example without sufficient knowledge of clang/llvm internals.)

---

### Comment 3 - ChuanqiXu9

Would you like to provide a reduced example?

And it would be helpful to test it with trunk and debugged build.

---

### Comment 4 - adityasz

[Reduced](https://github.com/adityasz/clang-170099) it to this:

```cpp
module;

struct A {};

struct B {
	int x;
	A   a;
	constexpr B(char *) { x = int(); }
	~B();
};

struct C {
	B b = "";
} inline c{};

export module foo;
```

<details>
  <summary>Stack trace from x86-64 clang (trunk) on <a href="https://godbolt.org/#g:!((g:!((g:!((h:codeEditor,i:(filename:'1',fontScale:14,fontUsePx:'0',j:1,lang:c%2B%2B,selection:(endColumn:1,endLineNumber:17,positionColumn:1,positionLineNumber:17,selectionStartColumn:1,selectionStartLineNumber:17,startColumn:1,startLineNumber:17),source:'module%3B%0A%0Astruct+A+%7B%7D%3B%0A%0Astruct+B+%7B%0A%09int+x%3B%0A%09A+++a%3B%0A%09constexpr+B(char+*)+%7B+x+%3D+int()%3B+%7D%0A%09~B()%3B%0A%7D%3B%0A%0Astruct+C+%7B%0A%09B+b+%3D+%22%22%3B%0A%7D+inline+c%7B%7D%3B%0A%0Aexport+module+foo%3B%0A'),l:'5',n:'0',o:'C%2B%2B+source+%231',t:'0')),k:50,l:'4',n:'0',o:'',s:0,t:'0'),(g:!((g:!((h:compiler,i:(compiler:clang_trunk,filters:(b:'0',binary:'1',binaryObject:'1',commentOnly:'0',debugCalls:'1',demangle:'0',directives:'0',execute:'1',intel:'0',libraryCode:'0',trim:'1',verboseDemangling:'0'),flagsViewOpen:'1',fontScale:14,fontUsePx:'0',j:1,lang:c%2B%2B,libs:!(),options:'-std%3Dc%2B%2B26+-O3',overrides:!(),selection:(endColumn:1,endLineNumber:1,positionColumn:1,positionLineNumber:1,selectionStartColumn:1,selectionStartLineNumber:1,startColumn:1,startLineNumber:1),source:1),l:'5',n:'0',o:'+x86-64+clang+(trunk)+(Editor+%231)',t:'0')),k:50,l:'4',m:50,n:'0',o:'',s:0,t:'0'),(g:!((h:output,i:(compilerName:'x86-64+gcc+15.2',editorid:1,fontScale:14,fontUsePx:'0',j:1,wrap:'1'),l:'5',n:'0',o:'Output+of+x86-64+clang+(trunk)+(Compiler+%231)',t:'0')),header:(),l:'4',m:50,n:'0',o:'',s:0,t:'0')),k:50,l:'3',n:'0',o:'',t:'0')),l:'2',n:'0',o:'',t:'0')),version:4">Compiler Explorer</a>
  </summary>

  ```
  PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
  Stack dump:
  0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++26 -O3 <source>
  1.	<eof> parser at end of file
  2.	Code generation
   #0 0x0000000003e398d8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3e398d8)
   #1 0x0000000003e372bc llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3e372bc)
   #2 0x0000000003d79ce8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
   #3 0x000070ca68642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
   #4 0x00000000037f6655 llvm::Value::stripPointerCasts() const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x37f6655)
   #5 0x0000000004e24131 llvm::AsmPrinter::preprocessXXStructorList(llvm::DataLayout const&, llvm::Constant const*, llvm::SmallVector<llvm::AsmPrinter::Structor, 8u>&) (.part.0) AsmPrinter.cpp:0:0
   #6 0x0000000004e257df llvm::AsmPrinter::emitXXStructorList(llvm::DataLayout const&, llvm::Constant const*, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4e257df)
   #7 0x0000000004e0cd21 llvm::AsmPrinter::emitSpecialLLVMGlobal(llvm::GlobalVariable const*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4e0cd21)
   #8 0x0000000004e19b1b llvm::AsmPrinter::emitGlobalVariable(llvm::GlobalVariable const*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4e19b1b)
   #9 0x0000000004e17e11 llvm::AsmPrinter::doFinalization(llvm::Module&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4e17e11)
  #10 0x0000000003776b1d llvm::FPPassManager::doFinalization(llvm::Module&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3776b1d)
  #11 0x0000000003781b10 llvm::legacy::PassManagerImpl::run(llvm::Module&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3781b10)
  #12 0x00000000041e83ae clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x41e83ae)
  #13 0x000000000481f229 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x481f229)
  #14 0x00000000063d29dc clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x63d29dc)
  #15 0x000000000481fd85 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x481fd85)
  #16 0x0000000004b4233a clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4b4233a)
  #17 0x0000000004abf32b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4abf32b)
  #18 0x0000000004c39adb clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4c39adb)
  #19 0x0000000000df72c5 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdf72c5)
  #20 0x0000000000deef8b ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
  #21 0x0000000000def02d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
  #22 0x000000000489ba19 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
  #23 0x0000000003d7a0a3 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3d7a0a3)
  #24 0x000000000489bc39 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
  #25 0x000000000485e412 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x485e412)
  #26 0x000000000485f2f1 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x485f2f1)
  #27 0x000000000486817c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x486817c)
  #28 0x0000000000df3aa1 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdf3aa1)
  #29 0x0000000000ca3c2a main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xca3c2a)
  #30 0x000070ca68629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
  #31 0x000070ca68629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
  #32 0x0000000000deea25 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdeea25)
  clang++: error: clang frontend command failed due to signal (use -v to see invocation)
  Compiler returned: 139
  ```
</details>
  
<details>
  <summary>Stack trace from clang 21.1.8 <code>-std=c++26 -O3</code></summary>
  
  ```
  PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
  Stack dump:
  0.      Program arguments: /usr/bin/clang-21 -cc1 -triple x86_64-redhat-linux-gnu -O3 -emit-obj -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name Foo.cppm -mrelocation-model static -mframe-pointer=none -fmath-errno -ffp-contract=on -fno-rounding-math
   -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/home/aditya/Playground/cpp/compiler-crash -fcoverage-compilation-dir=/home/aditya/Playground/cpp/compiler-crash -resource-dir /usr/bin/../lib/clang/21 -std=c++26 -fdeprecated-m
  acro -ferror-limit 19 -fmessage-length=115 -fgnuc-version=4.2.1 -fno-implicit-modules -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -vectorize-loops -vectorize-slp -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /dev/null -x pcm /tmp/Foo-7a0997.pcm
  1.      <eof> parser at end of file
  2.      Optimizer
  3.      Running pass "require<globals-aa>,function(invalidate<aa>),require<profile-summary>,cgscc(devirt<4>(inline,function-attrs<skip-non-recursive-function-attrs>,argpromotion,openmp-opt-cgscc,function<eager-inv;no-rerun>(sroa<modify-cfg>,early-cse<memssa>,speculative-execution<only-if-divergent-t
  arget>,jump-threading,correlated-propagation,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredicta
  bles>,instcombine<max-iterations=1;no-verify-fixpoint>,aggressive-instcombine,libcalls-shrinkwrap,tailcallelim,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-i
  nsts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,reassociate,constraint-elimination,loop-mssa(loop-instsimplify,loop-simplifycfg,licm<no-allowspeculation>,loop-rotate<header-duplication;no-prepare-for-lto>,licm<allowspeculation>,simple-loop-unswitch<nontrivial;trivial>),simpli
  fycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,instcombine<max-iterations=1;no-verify-fixpoi
  nt>,loop(loop-idiom,indvars,extra-simple-loop-unswitch-passes,loop-deletion,loop-unroll-full),sroa<modify-cfg>,vector-combine,mldst-motion<no-split-footer-bb>,gvn<>,sccp,bdce,instcombine<max-iterations=1;no-verify-fixpoint>,jump-threading,correlated-propagation,adce,memcpyopt,dse,move-auto-init,loop
  -mssa(licm<allowspeculation>),coro-elide,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-lookup;keep-loops;hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,inst
  combine<max-iterations=1;no-verify-fixpoint>),function-attrs,function(require<should-not-run-function-passes>),coro-split,coro-annotation-elide)),function(invalidate<should-not-run-function-passes>),cgscc(devirt<4>())" on module "/tmp/Foo-7a0997.pcm"
  4.      Running pass "cgscc(devirt<4>(inline,function-attrs<skip-non-recursive-function-attrs>,argpromotion,openmp-opt-cgscc,function<eager-inv;no-rerun>(sroa<modify-cfg>,early-cse<memssa>,speculative-execution<only-if-divergent-target>,jump-threading,correlated-propagation,simplifycfg<bonus-inst-th
  reshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,instcombine<max-iterations=1;no-verify-fixpoint>,aggressive-inst
  combine,libcalls-shrinkwrap,tailcallelim,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables
  >,reassociate,constraint-elimination,loop-mssa(loop-instsimplify,loop-simplifycfg,licm<no-allowspeculation>,loop-rotate<header-duplication;no-prepare-for-lto>,licm<allowspeculation>,simple-loop-unswitch<nontrivial;trivial>),simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-ic
  mp;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,instcombine<max-iterations=1;no-verify-fixpoint>,loop(loop-idiom,indvars,extra-simple-loop-unswitch-passes,loop-del
  etion,loop-unroll-full),sroa<modify-cfg>,vector-combine,mldst-motion<no-split-footer-bb>,gvn<>,sccp,bdce,instcombine<max-iterations=1;no-verify-fixpoint>,jump-threading,correlated-propagation,adce,memcpyopt,dse,move-auto-init,loop-mssa(licm<allowspeculation>),coro-elide,simplifycfg<bonus-inst-thresh
  old=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-lookup;keep-loops;hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,instcombine<max-iterations=1;no-verify-fixpoint>),function-attrs,function(
  require<should-not-run-function-passes>),coro-split,coro-annotation-elide))" on module "/tmp/Foo-7a0997.pcm"
   #0 0x00007f781facc569 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/lib64/libLLVM.so.21.1+0x50cc569)
   #1 0x00007f781faccfad (/lib64/libLLVM.so.21.1+0x50ccfad)
   #2 0x00007f781a427290 __restore_rt (/lib64/libc.so.6+0x1a290)
   #3 0x00007f781ee0e672 llvm::LazyCallGraph::visitReferences(llvm::SmallVectorImpl<llvm::Constant*>&, llvm::SmallPtrSetImpl<llvm::Constant*>&, llvm::function_ref<void (llvm::Function&)>) (/lib64/libLLVM.so.21.1+0x440e672)
   #4 0x00007f781ee0d904 llvm::LazyCallGraph::LazyCallGraph(llvm::Module&, llvm::function_ref<llvm::TargetLibraryInfo& (llvm::Function&)>) (/lib64/libLLVM.so.21.1+0x440d904)
   #5 0x00007f781f7afaf5 (/lib64/libLLVM.so.21.1+0x4dafaf5)
   #6 0x00007f781f7ae31e llvm::AnalysisManager<llvm::Module>::getResultImpl(llvm::AnalysisKey*, llvm::Module&) (/lib64/libLLVM.so.21.1+0x4dae31e)
   #7 0x00007f781f7ae05d (/lib64/libLLVM.so.21.1+0x4dae05d)
   #8 0x00007f781f7ae31e llvm::AnalysisManager<llvm::Module>::getResultImpl(llvm::AnalysisKey*, llvm::Module&) (/lib64/libLLVM.so.21.1+0x4dae31e)
   #9 0x00007f781ed17322 llvm::ModuleToPostOrderCGSCCPassAdaptor::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/lib64/libLLVM.so.21.1+0x4317322)
  #10 0x00007f781ed16801 (/lib64/libLLVM.so.21.1+0x4316801)
  #11 0x00007f781ebaa757 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/lib64/libLLVM.so.21.1+0x41aa757)
  #12 0x00007f781f7ad54e llvm::ModuleInlinerWrapperPass::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/lib64/libLLVM.so.21.1+0x4dad54e)
  #13 0x00007f781f7ad291 (/lib64/libLLVM.so.21.1+0x4dad291)
  #14 0x00007f781ebaa757 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/lib64/libLLVM.so.21.1+0x41aa757)
  #15 0x00007f782525aadd (/lib64/libclang-cpp.so.21.1+0x1e5aadd)
  #16 0x00007f7825255eda clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::Back
  endConsumer*) (/lib64/libclang-cpp.so.21.1+0x1e55eda)
  #17 0x00007f7825253e76 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/lib64/libclang-cpp.so.21.1+0x1e53e76)
  #18 0x00007f782490cbb2 clang::ParseAST(clang::Sema&, bool, bool) (/lib64/libclang-cpp.so.21.1+0x150cbb2)
  #19 0x00007f78251c85bf clang::FrontendAction::Execute() (/lib64/libclang-cpp.so.21.1+0x1dc85bf)
  #20 0x00007f78251c80d0 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/lib64/libclang-cpp.so.21.1+0x1dc80d0)
  #21 0x00007f78251c7a75 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/lib64/libclang-cpp.so.21.1+0x1dc7a75)
  #22 0x000055f0fd4fe8d6 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/usr/bin/clang-21+0x198d6)
  #23 0x000055f0fd4f5a4e (/usr/bin/clang-21+0x10a4e)
  #24 0x000055f0fd4f7534 clang_main(int, char**, llvm::ToolContext const&) (/usr/bin/clang-21+0x12534)
  #25 0x000055f0fd4f6782 main (/usr/bin/clang-21+0x11782)
  #26 0x00007f781a4105b5 __libc_start_call_main (/lib64/libc.so.6+0x35b5)
  #27 0x00007f781a410668 __libc_start_main@GLIBC_2.2.5 (/lib64/libc.so.6+0x3668)
  #28 0x000055f0fd500f85 _start (/usr/bin/clang-21+0x1bf85)
  clang: error: unable to execute command: Segmentation fault (core dumped)
  clang: error: clang frontend command failed due to signal (use -v to see invocation)
  clang version 21.1.8 (Fedora 21.1.8-4.fc43)
  ```
</details>

- clang 21.1.8 does not crash if I remove `-O3` and add `-w`: `clang -w -std=c++26 -o /dev/null -c Foo.cppm` exits with status 0.


---

### Comment 5 - ChuanqiXu9

Interesting, this relates to std version. It doesn't happen with C++20 but with C++26

---

### Comment 6 - adityasz

@EugeneZelenko Could you please remove the needs-reduction label now that the reproducer has been reduced?

---

