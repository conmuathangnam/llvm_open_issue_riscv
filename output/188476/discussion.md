# [clang] Crash with enum in function declaration defined somewhere else in C

**Author:** hueychen27
**URL:** https://github.com/llvm/llvm-project/issues/188476
**Status:** Open
**Labels:** clang:codegen, crash, needs-reduction

## Body

While compiling my source code for a file, `parser.c`, `clang` crashes.

[parser-838e49.c](https://github.com/user-attachments/files/26243119/parser-838e49.c)
[parser-838e49.sh](https://github.com/user-attachments/files/26243125/parser-838e49.sh)

This is returned while compiling:
```none
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: clang -Wall -Wextra -Wpedantic -Wformat=2 -Werror=implicit-function-declaration -std=gnu23 -Isrc/include -Isrc -Isrc/frontend -Isrc/runtime -Isrc/utils -funsigned-char -fstack-protector-strong -fstack-clash-protection -MMD -MP -Og -finstrument-functions -fsanitize=address,undefined -g3 -c src/frontend/parser.c -o build/frontend/parser.o
1.      src/frontend/parser.c:204:1: current parser token 'Expression'
2.      src/frontend/parser.c:195:13: LLVM IR generation of declaration 'parseMemberExpr'
3.      src/frontend/parser.c:195:13: Generating code for declaration 'parseMemberExpr'
 #0 0x00007f5ee514431a llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/usr/lib/llvm-23/bin/../lib/libLLVM.so.23.0+0x4d4431a)
 #1 0x00007f5ee5141b47 llvm::sys::RunSignalHandlers() (/usr/lib/llvm-23/bin/../lib/libLLVM.so.23.0+0x4d41b47)
 #2 0x00007f5ee506fd34 (/usr/lib/llvm-23/bin/../lib/libLLVM.so.23.0+0x4c6fd34)
 #3 0x00007f5edfe4b830 (/usr/lib/x86_64-linux-gnu/libc.so.6+0x40830)
 #4 0x00007f5eec17092d (/usr/lib/llvm-23/bin/../lib/libclang-cpp.so.23.0+0x2d7092d)
 #5 0x00007f5eec16f54c (/usr/lib/llvm-23/bin/../lib/libclang-cpp.so.23.0+0x2d6f54c)
 #6 0x00007f5eec16d5cd (/usr/lib/llvm-23/bin/../lib/libclang-cpp.so.23.0+0x2d6d5cd)
 #7 0x00007f5eebcda790 clang::CodeGen::CodeGenTypes::arrangeLLVMFunctionInfo(clang::CanQual<clang::Type>, clang::CodeGen::FnInfoOpts, llvm::ArrayRef<clang::CanQual<clang::Type>>, clang::FunctionType::ExtInfo, llvm::ArrayRef<clang::FunctionType::ExtParameterInfo>, clang::CodeGen::RequiredArgs) (/usr/lib/llvm-23/bin/../lib/libclang-cpp.so.23.0+0x28da790)
 #8 0x00007f5eebcdabc0 (/usr/lib/llvm-23/bin/../lib/libclang-cpp.so.23.0+0x28dabc0)
 #9 0x00007f5eebcdc188 clang::CodeGen::CodeGenTypes::arrangeFunctionDeclaration(clang::GlobalDecl) (/usr/lib/llvm-23/bin/../lib/libclang-cpp.so.23.0+0x28dc188)
#10 0x00007f5eebfdc94c clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/usr/lib/llvm-23/bin/../lib/libclang-cpp.so.23.0+0x2bdc94c)
#11 0x00007f5eebfd61af clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/usr/lib/llvm-23/bin/../lib/libclang-cpp.so.23.0+0x2bd61af)
#12 0x00007f5eebfda04a clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/usr/lib/llvm-23/bin/../lib/libclang-cpp.so.23.0+0x2bda04a)
#13 0x00007f5eebfd4ee5 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (/usr/lib/llvm-23/bin/../lib/libclang-cpp.so.23.0+0x2bd4ee5)
#14 0x00007f5eec0820bc (/usr/lib/llvm-23/bin/../lib/libclang-cpp.so.23.0+0x2c820bc)
#15 0x00007f5eebfa760d clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/usr/lib/llvm-23/bin/../lib/libclang-cpp.so.23.0+0x2ba760d)
#16 0x00007f5eeaa5a0aa clang::ParseAST(clang::Sema&, bool, bool) (/usr/lib/llvm-23/bin/../lib/libclang-cpp.so.23.0+0x165a0aa)
#17 0x00007f5eec7b131f clang::FrontendAction::Execute() (/usr/lib/llvm-23/bin/../lib/libclang-cpp.so.23.0+0x33b131f)
#18 0x00007f5eec724bb4 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/usr/lib/llvm-23/bin/../lib/libclang-cpp.so.23.0+0x3324bb4)
#19 0x00007f5eec83f8ba clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/lib/llvm-23/bin/../lib/libclang-cpp.so.23.0+0x343f8ba)
#20 0x0000562d0386eced cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/usr/lib/llvm-23/bin/clang+0x13ced)
#21 0x0000562d0386b55b (/usr/lib/llvm-23/bin/clang+0x1055b)
#22 0x0000562d0386cfec (/usr/lib/llvm-23/bin/clang+0x11fec)
#23 0x00007f5eec3d801d (/usr/lib/llvm-23/bin/../lib/libclang-cpp.so.23.0+0x2fd801d)
#24 0x00007f5ee506faa0 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/usr/lib/llvm-23/bin/../lib/libLLVM.so.23.0+0x4c6faa0)
#25 0x00007f5eec3d7ad1 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (/usr/lib/llvm-23/bin/../lib/libclang-cpp.so.23.0+0x2fd7ad1)
#26 0x00007f5eec396cf2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/usr/lib/llvm-23/bin/../lib/libclang-cpp.so.23.0+0x2f96cf2)
#27 0x00007f5eec396ebe clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/usr/lib/llvm-23/bin/../lib/libclang-cpp.so.23.0+0x2f96ebe)
#28 0x00007f5eec3b87cf clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/usr/lib/llvm-23/bin/../lib/libclang-cpp.so.23.0+0x2fb87cf)
#29 0x0000562d0386b04d clang_main(int, char**, llvm::ToolContext const&) (/usr/lib/llvm-23/bin/clang+0x1004d)
#30 0x0000562d0387997a main (/usr/lib/llvm-23/bin/clang+0x1e97a)
#31 0x00007f5edfe34fb8 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:83:3
#32 0x00007f5edfe350d7 call_init ./csu/../csu/libc-start.c:128:20
#33 0x00007f5edfe350d7 __libc_start_main ./csu/../csu/libc-start.c:347:5
#34 0x0000562d038691f1 _start (/usr/lib/llvm-23/bin/clang+0xe1f1)
clang: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Debian clang version 23.0.0 (++20260204101751+dd02b404b020-1~exp1)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/lib/llvm-23/bin
clang: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang: note: diagnostic msg: /tmp/user/1000/parser-838e49.c
clang: note: diagnostic msg: /tmp/user/1000/parser-838e49.sh
clang: note: diagnostic msg: 

********************
```

## Comments

### Comment 1 - hueychen27

Crash fixed when `enum BindingPower` is switched to `int` for `parser.c` and `parser.h`.

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: hueychen27

<details>
While compiling my source code for a file, `parser.c`, `clang` crashes.

[parser-838e49.c](https://github.com/user-attachments/files/26243119/parser-838e49.c)
[parser-838e49.sh](https://github.com/user-attachments/files/26243125/parser-838e49.sh)

This is returned while compiling:
```none
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: clang -Wall -Wextra -Wpedantic -Wformat=2 -Werror=implicit-function-declaration -std=gnu23 -Isrc/include -Isrc -Isrc/frontend -Isrc/runtime -Isrc/utils -funsigned-char -fstack-protector-strong -fstack-clash-protection -MMD -MP -Og -finstrument-functions -fsanitize=address,undefined -g3 -c src/frontend/parser.c -o build/frontend/parser.o
1.      src/frontend/parser.c:204:1: current parser token 'Expression'
2.      src/frontend/parser.c:195:13: LLVM IR generation of declaration 'parseMemberExpr'
3.      src/frontend/parser.c:195:13: Generating code for declaration 'parseMemberExpr'
 #<!-- -->0 0x00007f5ee514431a llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/usr/lib/llvm-23/bin/../lib/libLLVM.so.23.0+0x4d4431a)
 #<!-- -->1 0x00007f5ee5141b47 llvm::sys::RunSignalHandlers() (/usr/lib/llvm-23/bin/../lib/libLLVM.so.23.0+0x4d41b47)
 #<!-- -->2 0x00007f5ee506fd34 (/usr/lib/llvm-23/bin/../lib/libLLVM.so.23.0+0x4c6fd34)
 #<!-- -->3 0x00007f5edfe4b830 (/usr/lib/x86_64-linux-gnu/libc.so.6+0x40830)
 #<!-- -->4 0x00007f5eec17092d (/usr/lib/llvm-23/bin/../lib/libclang-cpp.so.23.0+0x2d7092d)
 #<!-- -->5 0x00007f5eec16f54c (/usr/lib/llvm-23/bin/../lib/libclang-cpp.so.23.0+0x2d6f54c)
 #<!-- -->6 0x00007f5eec16d5cd (/usr/lib/llvm-23/bin/../lib/libclang-cpp.so.23.0+0x2d6d5cd)
 #<!-- -->7 0x00007f5eebcda790 clang::CodeGen::CodeGenTypes::arrangeLLVMFunctionInfo(clang::CanQual&lt;clang::Type&gt;, clang::CodeGen::FnInfoOpts, llvm::ArrayRef&lt;clang::CanQual&lt;clang::Type&gt;&gt;, clang::FunctionType::ExtInfo, llvm::ArrayRef&lt;clang::FunctionType::ExtParameterInfo&gt;, clang::CodeGen::RequiredArgs) (/usr/lib/llvm-23/bin/../lib/libclang-cpp.so.23.0+0x28da790)
 #<!-- -->8 0x00007f5eebcdabc0 (/usr/lib/llvm-23/bin/../lib/libclang-cpp.so.23.0+0x28dabc0)
 #<!-- -->9 0x00007f5eebcdc188 clang::CodeGen::CodeGenTypes::arrangeFunctionDeclaration(clang::GlobalDecl) (/usr/lib/llvm-23/bin/../lib/libclang-cpp.so.23.0+0x28dc188)
#<!-- -->10 0x00007f5eebfdc94c clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/usr/lib/llvm-23/bin/../lib/libclang-cpp.so.23.0+0x2bdc94c)
#<!-- -->11 0x00007f5eebfd61af clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/usr/lib/llvm-23/bin/../lib/libclang-cpp.so.23.0+0x2bd61af)
#<!-- -->12 0x00007f5eebfda04a clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/usr/lib/llvm-23/bin/../lib/libclang-cpp.so.23.0+0x2bda04a)
#<!-- -->13 0x00007f5eebfd4ee5 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (/usr/lib/llvm-23/bin/../lib/libclang-cpp.so.23.0+0x2bd4ee5)
#<!-- -->14 0x00007f5eec0820bc (/usr/lib/llvm-23/bin/../lib/libclang-cpp.so.23.0+0x2c820bc)
#<!-- -->15 0x00007f5eebfa760d clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/usr/lib/llvm-23/bin/../lib/libclang-cpp.so.23.0+0x2ba760d)
#<!-- -->16 0x00007f5eeaa5a0aa clang::ParseAST(clang::Sema&amp;, bool, bool) (/usr/lib/llvm-23/bin/../lib/libclang-cpp.so.23.0+0x165a0aa)
#<!-- -->17 0x00007f5eec7b131f clang::FrontendAction::Execute() (/usr/lib/llvm-23/bin/../lib/libclang-cpp.so.23.0+0x33b131f)
#<!-- -->18 0x00007f5eec724bb4 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/usr/lib/llvm-23/bin/../lib/libclang-cpp.so.23.0+0x3324bb4)
#<!-- -->19 0x00007f5eec83f8ba clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/lib/llvm-23/bin/../lib/libclang-cpp.so.23.0+0x343f8ba)
#<!-- -->20 0x0000562d0386eced cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/usr/lib/llvm-23/bin/clang+0x13ced)
#<!-- -->21 0x0000562d0386b55b (/usr/lib/llvm-23/bin/clang+0x1055b)
#<!-- -->22 0x0000562d0386cfec (/usr/lib/llvm-23/bin/clang+0x11fec)
#<!-- -->23 0x00007f5eec3d801d (/usr/lib/llvm-23/bin/../lib/libclang-cpp.so.23.0+0x2fd801d)
#<!-- -->24 0x00007f5ee506faa0 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/usr/lib/llvm-23/bin/../lib/libLLVM.so.23.0+0x4c6faa0)
#<!-- -->25 0x00007f5eec3d7ad1 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (/usr/lib/llvm-23/bin/../lib/libclang-cpp.so.23.0+0x2fd7ad1)
#<!-- -->26 0x00007f5eec396cf2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/usr/lib/llvm-23/bin/../lib/libclang-cpp.so.23.0+0x2f96cf2)
#<!-- -->27 0x00007f5eec396ebe clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/usr/lib/llvm-23/bin/../lib/libclang-cpp.so.23.0+0x2f96ebe)
#<!-- -->28 0x00007f5eec3b87cf clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/usr/lib/llvm-23/bin/../lib/libclang-cpp.so.23.0+0x2fb87cf)
#<!-- -->29 0x0000562d0386b04d clang_main(int, char**, llvm::ToolContext const&amp;) (/usr/lib/llvm-23/bin/clang+0x1004d)
#<!-- -->30 0x0000562d0387997a main (/usr/lib/llvm-23/bin/clang+0x1e97a)
#<!-- -->31 0x00007f5edfe34fb8 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:83:3
#<!-- -->32 0x00007f5edfe350d7 call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->33 0x00007f5edfe350d7 __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->34 0x0000562d038691f1 _start (/usr/lib/llvm-23/bin/clang+0xe1f1)
clang: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Debian clang version 23.0.0 (++20260204101751+dd02b404b020-1~exp1)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/lib/llvm-23/bin
clang: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang: note: diagnostic msg: /tmp/user/1000/parser-838e49.c
clang: note: diagnostic msg: /tmp/user/1000/parser-838e49.sh
clang: note: diagnostic msg: 

********************
```
</details>


---

