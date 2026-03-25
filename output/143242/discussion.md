# Clang crashes when passing std::string_view to asm() outside of functions, instead of reporting an error

**Author:** PyXiion
**URL:** https://github.com/llvm/llvm-project/issues/143242
**Status:** Closed
**Labels:** clang:frontend, confirmed, crash
**Closed Date:** 2025-06-08T07:16:58Z

## Body

When I pass a std::string_view to the asm() statement outside of any function, Clang crashes instead of producing a compilation error.

Example (crashes Clang):
```
constexpr std::string_view code2 = R"(.intel_syntax noprefix
hello:
push rbp
mov rbp, rsp
mov eax, 5
mov dword ptr [ebp-4], eax
mov rsp, rbp
pop rbp
ret
.att_syntax)";

asm((code2));

int main() {
  ...
}
```

Example (works fine with a literal):
```
asm(R"(
// same thing here
)")

int main() {
  ...
}
```

This appears to be a frontend crash in Clang. It should accept the code.

Files: https://filebin.net/hwb2cwq6kyyqz4sx

```
Stack dump:
0.	Program arguments: /home/pyxiion/llvm21/llvm-project/build/bin/clang++ -g -std=c++26 -fcolor-diagnostics -fconstexpr-depth=20000 -MD -MT CMakeFiles/pxccc.dir/src/main.cpp.o -MF CMakeFiles/pxccc.dir/src/main.cpp.o.d @CMakeFiles/pxccc.dir/src/main.cpp.o.modmap -o CMakeFiles/pxccc.dir/src/main.cpp.o -c /home/pyxiion/CLionProjects/pxccc/src/main.cpp
1.	/home/pyxiion/CLionProjects/pxccc/src/main.cpp:26:1: current parser token 'int'
2.	/home/pyxiion/CLionProjects/pxccc/src/main.cpp:24:1: LLVM IR generation of declaration
 #0 0x00005e125339a8f2 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/home/pyxiion/llvm21/llvm-project/build/bin/clang+++0x3cb18f2)
 #1 0x00005e125339865c llvm::sys::CleanupOnSignal(unsigned long) (/home/pyxiion/llvm21/llvm-project/build/bin/clang+++0x3caf65c)
 #2 0x00005e12532dca28 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007e6fbc645330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x00007e6fbc788f90 __memcpy_avx_unaligned_erms ./string/../sysdeps/x86_64/multiarch/memmove-vec-unaligned-erms.S:833:0
 #5 0x00007e6fbcb68755 std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>::_M_mutate(unsigned long, unsigned long, char const*, unsigned long) (/lib/x86_64-linux-gnu/libstdc++.so.6+0x168755)
 #6 0x00007e6fbcb6a050 std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>::_M_append(char const*, unsigned long) (/lib/x86_64-linux-gnu/libstdc++.so.6+0x16a050)
 #7 0x00005e12538d9c10 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
 #8 0x00005e1253c7bb88 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
 #9 0x00005e1253c680d7 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/home/pyxiion/llvm21/llvm-project/build/bin/clang+++0x457f0d7)
#10 0x00005e1255501ac4 clang::ParseAST(clang::Sema&, bool, bool) (/home/pyxiion/llvm21/llvm-project/build/bin/clang+++0x5e18ac4)
#11 0x00005e1253fa0f6e clang::FrontendAction::Execute() (/home/pyxiion/llvm21/llvm-project/build/bin/clang+++0x48b7f6e)
#12 0x00005e1253f20983 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/home/pyxiion/llvm21/llvm-project/build/bin/clang+++0x4837983)
#13 0x00005e125407d06b clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/home/pyxiion/llvm21/llvm-project/build/bin/clang+++0x499406b)
#14 0x00005e1250580e2e cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/home/pyxiion/llvm21/llvm-project/build/bin/clang+++0xe97e2e)
#15 0x00005e125057789a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#16 0x00005e1253ceab9d void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#17 0x00005e12532dce57 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/home/pyxiion/llvm21/llvm-project/build/bin/clang+++0x3bf3e57)
#18 0x00005e1253ceb291 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#19 0x00005e1253caeb0a clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/home/pyxiion/llvm21/llvm-project/build/bin/clang+++0x45c5b0a)
#20 0x00005e1253cafb89 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/home/pyxiion/llvm21/llvm-project/build/bin/clang+++0x45c6b89)
#21 0x00005e1253cc46ac clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/home/pyxiion/llvm21/llvm-project/build/bin/clang+++0x45db6ac)
#22 0x00005e125057c6e8 clang_main(int, char**, llvm::ToolContext const&) (/home/pyxiion/llvm21/llvm-project/build/bin/clang+++0xe936e8)
#23 0x00005e12503f1d3a main (/home/pyxiion/llvm21/llvm-project/build/bin/clang+++0xd08d3a)
#24 0x00007e6fbc62a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#25 0x00007e6fbc62a28b call_init ./csu/../csu/libc-start.c:128:20
#26 0x00007e6fbc62a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#27 0x00005e12505772d5 _start (/home/pyxiion/llvm21/llvm-project/build/bin/clang+++0xe8e2d5)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
clang version 21.0.0git (https://github.com/llvm/llvm-project.git 0c495ce9c4237f0f090b672efd94839e52cb5f18)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /home/pyxiion/llvm21/llvm-project/build/bin
clang++: note: diagnostic msg: 
```

## Comments

### Comment 1 - cor3ntin

https://godbolt.org/z/P6qj888Mz

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (PyXiion)

<details>
When I pass a std::string_view to the asm() statement outside of any function, Clang crashes instead of producing a compilation error.

Example (crashes Clang):
```
constexpr std::string_view code2 = R"(.intel_syntax noprefix
hello:
push rbp
mov rbp, rsp
mov eax, 5
mov dword ptr [ebp-4], eax
mov rsp, rbp
pop rbp
ret
.att_syntax)";

asm((code2));

int main() {
  ...
}
```

Example (works fine with a literal):
```
asm(R"(
// same thing here
)")

int main() {
  ...
}
```

This appears to be a frontend crash in Clang. It should accept the code.

Files: https://filebin.net/hwb2cwq6kyyqz4sx

```
Stack dump:
0.	Program arguments: /home/pyxiion/llvm21/llvm-project/build/bin/clang++ -g -std=c++26 -fcolor-diagnostics -fconstexpr-depth=20000 -MD -MT CMakeFiles/pxccc.dir/src/main.cpp.o -MF CMakeFiles/pxccc.dir/src/main.cpp.o.d @<!-- -->CMakeFiles/pxccc.dir/src/main.cpp.o.modmap -o CMakeFiles/pxccc.dir/src/main.cpp.o -c /home/pyxiion/CLionProjects/pxccc/src/main.cpp
1.	/home/pyxiion/CLionProjects/pxccc/src/main.cpp:26:1: current parser token 'int'
2.	/home/pyxiion/CLionProjects/pxccc/src/main.cpp:24:1: LLVM IR generation of declaration
 #<!-- -->0 0x00005e125339a8f2 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/home/pyxiion/llvm21/llvm-project/build/bin/clang+++0x3cb18f2)
 #<!-- -->1 0x00005e125339865c llvm::sys::CleanupOnSignal(unsigned long) (/home/pyxiion/llvm21/llvm-project/build/bin/clang+++0x3caf65c)
 #<!-- -->2 0x00005e12532dca28 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007e6fbc645330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->4 0x00007e6fbc788f90 __memcpy_avx_unaligned_erms ./string/../sysdeps/x86_64/multiarch/memmove-vec-unaligned-erms.S:833:0
 #<!-- -->5 0x00007e6fbcb68755 std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;::_M_mutate(unsigned long, unsigned long, char const*, unsigned long) (/lib/x86_64-linux-gnu/libstdc++.so.6+0x168755)
 #<!-- -->6 0x00007e6fbcb6a050 std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;::_M_append(char const*, unsigned long) (/lib/x86_64-linux-gnu/libstdc++.so.6+0x16a050)
 #<!-- -->7 0x00005e12538d9c10 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
 #<!-- -->8 0x00005e1253c7bb88 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
 #<!-- -->9 0x00005e1253c680d7 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/home/pyxiion/llvm21/llvm-project/build/bin/clang+++0x457f0d7)
#<!-- -->10 0x00005e1255501ac4 clang::ParseAST(clang::Sema&amp;, bool, bool) (/home/pyxiion/llvm21/llvm-project/build/bin/clang+++0x5e18ac4)
#<!-- -->11 0x00005e1253fa0f6e clang::FrontendAction::Execute() (/home/pyxiion/llvm21/llvm-project/build/bin/clang+++0x48b7f6e)
#<!-- -->12 0x00005e1253f20983 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/home/pyxiion/llvm21/llvm-project/build/bin/clang+++0x4837983)
#<!-- -->13 0x00005e125407d06b clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/home/pyxiion/llvm21/llvm-project/build/bin/clang+++0x499406b)
#<!-- -->14 0x00005e1250580e2e cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/home/pyxiion/llvm21/llvm-project/build/bin/clang+++0xe97e2e)
#<!-- -->15 0x00005e125057789a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->16 0x00005e1253ceab9d void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->17 0x00005e12532dce57 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/home/pyxiion/llvm21/llvm-project/build/bin/clang+++0x3bf3e57)
#<!-- -->18 0x00005e1253ceb291 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->19 0x00005e1253caeb0a clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/home/pyxiion/llvm21/llvm-project/build/bin/clang+++0x45c5b0a)
#<!-- -->20 0x00005e1253cafb89 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/home/pyxiion/llvm21/llvm-project/build/bin/clang+++0x45c6b89)
#<!-- -->21 0x00005e1253cc46ac clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/home/pyxiion/llvm21/llvm-project/build/bin/clang+++0x45db6ac)
#<!-- -->22 0x00005e125057c6e8 clang_main(int, char**, llvm::ToolContext const&amp;) (/home/pyxiion/llvm21/llvm-project/build/bin/clang+++0xe936e8)
#<!-- -->23 0x00005e12503f1d3a main (/home/pyxiion/llvm21/llvm-project/build/bin/clang+++0xd08d3a)
#<!-- -->24 0x00007e6fbc62a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->25 0x00007e6fbc62a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->26 0x00007e6fbc62a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->27 0x00005e12505772d5 _start (/home/pyxiion/llvm21/llvm-project/build/bin/clang+++0xe8e2d5)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
clang version 21.0.0git (https://github.com/llvm/llvm-project.git 0c495ce9c4237f0f090b672efd94839e52cb5f18)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /home/pyxiion/llvm21/llvm-project/build/bin
clang++: note: diagnostic msg: 
```
</details>


---

### Comment 3 - zwuis

Maybe related: #137459

---

