# Crash in ASTDeclContextNameLookupTrait::getLookupVisibility 

**Author:** marpe2020
**URL:** https://github.com/llvm/llvm-project/issues/161215
**Status:** Open
**Labels:** clang:modules, crash

## Body

```console
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /opt/llvm/21.1.2/bin/clang++ -std=c++20 -O2 -Wall -Wextra -Wno-experimental-header-units -fprebuilt-module-path=. --precompile -fmodule-file=cinttypes.pch -fmodule-file=functional.pch -fmodule-file=memory.pch -fmodule-file=stdexcept.pch -fmodule-file=unordered_map.pch -fmodule-file=utility.pch counters.cppm -o counters.pcm
1.      <eof> parser at end of file
 #0 0x000055fca81709b7 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/llvm/21.1.2/bin/clang+++0x7f709b7)
 #1 0x000055fca8170780 llvm::sys::CleanupOnSignal(unsigned long) (/opt/llvm/21.1.2/bin/clang+++0x7f70780)
 #2 0x000055fca811ed9e (anonymous namespace)::CrashRecoveryContextImpl::HandleCrash(int, unsigned long) CrashRecoveryContext.cpp:0:0
 #3 0x000055fca811ef6e CrashRecoverySignalHandler(int) (.llvm.16389050741671196674) CrashRecoveryContext.cpp:0:0
 #4 0x00007f8be65d6df0 (/lib/x86_64-linux-gnu/libc.so.6+0x3fdf0)
 #5 0x000055fca8968090 (anonymous namespace)::ASTDeclContextNameLookupTrait::getLookupVisibility(clang::NamedDecl*) const ASTWriter.cpp:0:0
 #6 0x000055fca893bf6d clang::ASTWriter::GenerateNameLookupTable(clang::ASTContext&, clang::DeclContext const*, llvm::SmallVectorImpl<char>&, llvm::SmallVectorImpl<char>&, llvm::SmallVectorImpl<char>&) (/opt/llvm/21.1.2/bin/clang+++0x873bf6d)
 #7 0x000055fca893ed9d clang::ASTWriter::WriteDeclContextVisibleUpdate(clang::ASTContext&, clang::DeclContext const*) (/opt/llvm/21.1.2/bin/clang+++0x873ed9d)
 #8 0x000055fca894cdac clang::ASTWriter::WriteDeclAndTypes(clang::ASTContext&) (/opt/llvm/21.1.2/bin/clang+++0x874cdac)
 #9 0x000055fca894943d clang::ASTWriter::WriteASTCore(clang::Sema*, llvm::StringRef, clang::Module*) (/opt/llvm/21.1.2/bin/clang+++0x874943d)
#10 0x000055fca8948693 clang::ASTWriter::WriteAST(llvm::PointerUnion<clang::Sema*, clang::Preprocessor*>, llvm::StringRef, clang::Module*, llvm::StringRef, bool) (/opt/llvm/21.1.2/bin/clang+++0x8748693)
#11 0x000055fca898dbef clang::PCHGenerator::HandleTranslationUnit(clang::ASTContext&) (/opt/llvm/21.1.2/bin/clang+++0x878dbef)
#12 0x000055fca898dd76 clang::CXX20ModulesGenerator::HandleTranslationUnit(clang::ASTContext&) (/opt/llvm/21.1.2/bin/clang+++0x878dd76)
#13 0x000055fca884dff1 clang::MultiplexConsumer::HandleTranslationUnit(clang::ASTContext&) (/opt/llvm/21.1.2/bin/clang+++0x864dff1)
#14 0x000055fcaaeb27de clang::ParseAST(clang::Sema&, bool, bool) (/opt/llvm/21.1.2/bin/clang+++0xacb27de)
#15 0x000055fcab7327ad clang::FrontendAction::Execute() (/opt/llvm/21.1.2/bin/clang+++0xb5327ad)
#16 0x000055fcab7392ef clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/llvm/21.1.2/bin/clang+++0xb5392ef)
#17 0x000055fcab739192 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/llvm/21.1.2/bin/clang+++0xb539192)
#18 0x000055fcab738915 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/llvm/21.1.2/bin/clang+++0xb538915)
#19 0x000055fcab5fd603 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#20 0x000055fcab5fd4b7 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::__1::optional<llvm::StringRef>>, std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>>*, bool*) const::$_0>(long) Job.cpp:0:0
#21 0x000055fcab5fd48e llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/llvm/21.1.2/bin/clang+++0xb3fd48e)
#22 0x000055fcab5fd37f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::__1::optional<llvm::StringRef>>, std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>>*, bool*) const (/opt/llvm/21.1.2/bin/clang+++0xb3fd37f)
#23 0x000055fcab5fd0d1 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/llvm/21.1.2/bin/clang+++0xb3fd0d1)
#24 0x000055fcab5fcf6d clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::__1::pair<int, clang::driver::Command const*>>&, bool) const (/opt/llvm/21.1.2/bin/clang+++0xb3fcf6d)
#25 0x000055fcab5fcbb6 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::__1::pair<int, clang::driver::Command const*>>&) (/opt/llvm/21.1.2/bin/clang+++0xb3fcbb6)
#26 0x000055fcab601b33 clang_main(int, char**, llvm::ToolContext const&) (/opt/llvm/21.1.2/bin/clang+++0xb401b33)
#27 0x000055fcab601012 main (/opt/llvm/21.1.2/bin/clang+++0xb401012)
#28 0x00007f8be65c0ca8 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#29 0x00007f8be65c0d65 call_init ./csu/../csu/libc-start.c:128:20
#30 0x00007f8be65c0d65 __libc_start_main ./csu/../csu/libc-start.c:347:5
#31 0x000055fcab4ef765 _start (/opt/llvm/21.1.2/bin/clang+++0xb2ef765)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
clang version 21.1.2 (https://github.com/llvm/llvm-project b708aea0bc7127adf4ec643660699c8bcdde1273)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /opt/llvm/21.1.2/bin
clang++: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: /tmp/counters-9e2494.cppm
clang++: note: diagnostic msg: /tmp/counters-9e2494.sh
clang++: note: diagnostic msg: 
```
[counters-9e2494.cppm](https://github.com/user-attachments/files/22601027/counters-9e2494.cpp)

[counters-9e2494.sh](https://github.com/user-attachments/files/22601050/counters-9e2494.sh)

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-modules

Author: None (marpe2020)

<details>
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /opt/llvm/21.1.2/bin/clang++ -std=c++20 -O2 -Wall -Wextra -Wno-experimental-header-units -fprebuilt-module-path=. --precompile -fmodule-file=cinttypes.pch -fmodule-file=functional.pch -fmodule-file=memory.pch -fmodule-file=stdexcept.pch -fmodule-file=unordered_map.pch -fmodule-file=utility.pch counters.cppm -o counters.pcm
1.      &lt;eof&gt; parser at end of file
 #<!-- -->0 0x000055fca81709b7 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/llvm/21.1.2/bin/clang+++0x7f709b7)
 #<!-- -->1 0x000055fca8170780 llvm::sys::CleanupOnSignal(unsigned long) (/opt/llvm/21.1.2/bin/clang+++0x7f70780)
 #<!-- -->2 0x000055fca811ed9e (anonymous namespace)::CrashRecoveryContextImpl::HandleCrash(int, unsigned long) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x000055fca811ef6e CrashRecoverySignalHandler(int) (.llvm.16389050741671196674) CrashRecoveryContext.cpp:0:0
 #<!-- -->4 0x00007f8be65d6df0 (/lib/x86_64-linux-gnu/libc.so.6+0x3fdf0)
 #<!-- -->5 0x000055fca8968090 (anonymous namespace)::ASTDeclContextNameLookupTrait::getLookupVisibility(clang::NamedDecl*) const ASTWriter.cpp:0:0
 #<!-- -->6 0x000055fca893bf6d clang::ASTWriter::GenerateNameLookupTable(clang::ASTContext&amp;, clang::DeclContext const*, llvm::SmallVectorImpl&lt;char&gt;&amp;, llvm::SmallVectorImpl&lt;char&gt;&amp;, llvm::SmallVectorImpl&lt;char&gt;&amp;) (/opt/llvm/21.1.2/bin/clang+++0x873bf6d)
 #<!-- -->7 0x000055fca893ed9d clang::ASTWriter::WriteDeclContextVisibleUpdate(clang::ASTContext&amp;, clang::DeclContext const*) (/opt/llvm/21.1.2/bin/clang+++0x873ed9d)
 #<!-- -->8 0x000055fca894cdac clang::ASTWriter::WriteDeclAndTypes(clang::ASTContext&amp;) (/opt/llvm/21.1.2/bin/clang+++0x874cdac)
 #<!-- -->9 0x000055fca894943d clang::ASTWriter::WriteASTCore(clang::Sema*, llvm::StringRef, clang::Module*) (/opt/llvm/21.1.2/bin/clang+++0x874943d)
#<!-- -->10 0x000055fca8948693 clang::ASTWriter::WriteAST(llvm::PointerUnion&lt;clang::Sema*, clang::Preprocessor*&gt;, llvm::StringRef, clang::Module*, llvm::StringRef, bool) (/opt/llvm/21.1.2/bin/clang+++0x8748693)
#<!-- -->11 0x000055fca898dbef clang::PCHGenerator::HandleTranslationUnit(clang::ASTContext&amp;) (/opt/llvm/21.1.2/bin/clang+++0x878dbef)
#<!-- -->12 0x000055fca898dd76 clang::CXX20ModulesGenerator::HandleTranslationUnit(clang::ASTContext&amp;) (/opt/llvm/21.1.2/bin/clang+++0x878dd76)
#<!-- -->13 0x000055fca884dff1 clang::MultiplexConsumer::HandleTranslationUnit(clang::ASTContext&amp;) (/opt/llvm/21.1.2/bin/clang+++0x864dff1)
#<!-- -->14 0x000055fcaaeb27de clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/llvm/21.1.2/bin/clang+++0xacb27de)
#<!-- -->15 0x000055fcab7327ad clang::FrontendAction::Execute() (/opt/llvm/21.1.2/bin/clang+++0xb5327ad)
#<!-- -->16 0x000055fcab7392ef clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/llvm/21.1.2/bin/clang+++0xb5392ef)
#<!-- -->17 0x000055fcab739192 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/llvm/21.1.2/bin/clang+++0xb539192)
#<!-- -->18 0x000055fcab738915 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/llvm/21.1.2/bin/clang+++0xb538915)
#<!-- -->19 0x000055fcab5fd603 ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->20 0x000055fcab5fd4b7 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::__1::optional&lt;llvm::StringRef&gt;&gt;, std::__1::basic_string&lt;char, std::__1::char_traits&lt;char&gt;, std::__1::allocator&lt;char&gt;&gt;*, bool*) const::$_0&gt;(long) Job.cpp:0:0
#<!-- -->21 0x000055fcab5fd48e llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/llvm/21.1.2/bin/clang+++0xb3fd48e)
#<!-- -->22 0x000055fcab5fd37f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::__1::optional&lt;llvm::StringRef&gt;&gt;, std::__1::basic_string&lt;char, std::__1::char_traits&lt;char&gt;, std::__1::allocator&lt;char&gt;&gt;*, bool*) const (/opt/llvm/21.1.2/bin/clang+++0xb3fd37f)
#<!-- -->23 0x000055fcab5fd0d1 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/llvm/21.1.2/bin/clang+++0xb3fd0d1)
#<!-- -->24 0x000055fcab5fcf6d clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::__1::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/llvm/21.1.2/bin/clang+++0xb3fcf6d)
#<!-- -->25 0x000055fcab5fcbb6 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::__1::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/llvm/21.1.2/bin/clang+++0xb3fcbb6)
#<!-- -->26 0x000055fcab601b33 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/llvm/21.1.2/bin/clang+++0xb401b33)
#<!-- -->27 0x000055fcab601012 main (/opt/llvm/21.1.2/bin/clang+++0xb401012)
#<!-- -->28 0x00007f8be65c0ca8 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->29 0x00007f8be65c0d65 call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->30 0x00007f8be65c0d65 __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->31 0x000055fcab4ef765 _start (/opt/llvm/21.1.2/bin/clang+++0xb2ef765)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
clang version 21.1.2 (https://github.com/llvm/llvm-project b708aea0bc7127adf4ec643660699c8bcdde1273)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /opt/llvm/21.1.2/bin
clang++: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: /tmp/counters-9e2494.cppm
clang++: note: diagnostic msg: /tmp/counters-9e2494.sh
clang++: note: diagnostic msg: 

[counters-9e2494.cppm](https://github.com/user-attachments/files/22601027/counters-9e2494.cpp)

[counters-9e2494.sh](https://github.com/user-attachments/files/22601050/counters-9e2494.sh)
</details>


---

### Comment 2 - shafik

CC @ChuanqiXu9 @mpark 

---

