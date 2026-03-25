# [clang]Clang crash when forming `decltype` of static alias template member without template arguments

**Author:** Attacker23
**URL:** https://github.com/llvm/llvm-project/issues/173349

## Body

The following code has been causing Clang to crash since version 16.0.0 and still does in the current trunk (x86‑64).

```cpp
template <class T>
struct A {
  template <class U>
  using E = U;
  static E u;
};

decltype(A<int>::u) a;
```

Please check: https://godbolt.org/z/ovvr6KE1n

Clang crashes while emitting IR for the `a` declared as `decltype(A<int>::u)`, where `u` is declared using an alias template `E` without providing template arguments (ill-formed).

**Output:**  
```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -ferror-limit=0 <source>
1.	<eof> parser at end of file
2.	<source>:8:21: LLVM IR generation of declaration 'a'
3.	<source>:8:21: Generating code for declaration 'a'
 #0 0x0000000003d38488 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3d38488)
 #1 0x0000000003d35e5c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3d35e5c)
 #2 0x0000000003c7cde8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000079f62ba42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00000000036f8e32 llvm::Type::isIntegerTy(unsigned int) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x36f8e32)
 #5 0x000000000430a7dd clang::CodeGen::CodeGenTypes::ConvertTypeForMem(clang::QualType) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x430a7dd)
 #6 0x00000000040aeec7 clang::CodeGen::CodeGenModule::EmitNullConstant(clang::QualType) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x40aeec7)
 #7 0x000000000428ec84 clang::CodeGen::CodeGenModule::EmitGlobalVarDefinition(clang::VarDecl const*, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x428ec84)
 #8 0x00000000042b0c35 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x42b0c35)
 #9 0x00000000042b18e0 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x42b18e0)
#10 0x00000000042bc1ef clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#11 0x00000000046078b1 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#12 0x00000000045f27b9 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45f27b9)
#13 0x00000000062545b4 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62545b4)
#14 0x0000000004605b55 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4605b55)
#15 0x000000000491659a clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x491659a)
#16 0x00000000048944ab clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x48944ab)
#17 0x0000000004a0c9bb clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4a0c9bb)
#18 0x0000000000de1f5d cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xde1f5d)
#19 0x0000000000dd9ddb ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#20 0x0000000000dd9e7d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#21 0x000000000467fba9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#22 0x0000000003c7d203 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c7d203)
#23 0x000000000467fdc9 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#24 0x0000000004642fb2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4642fb2)
#25 0x0000000004643e91 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4643e91)
#26 0x000000000464c8dc clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x464c8dc)
#27 0x0000000000dde8a9 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdde8a9)
#28 0x0000000000c8f7c4 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc8f7c4)
#29 0x000079f62ba29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#30 0x000079f62ba29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#31 0x0000000000dd9875 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdd9875)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```

The following code is correct:

```cpp
template <class T>
struct A {
  template <class U>
  using E = U;
  static E<int> u;  //correct code
};

decltype(A<int>::u) a;
```

## Comments

### Comment 1 - frederick-vs-ja

Only crashes in C++17 and later modes. Probably related to some bug around class template argument deduction, and not a regression.

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: None (Attacker23)

<details>
The following code has been causing Clang to crash since version 16.0.0 and still does in the current trunk (x86‑64).

```cpp
template &lt;class T&gt;
struct A {
  template &lt;class U&gt;
  using E = U;
  static E u;
};

decltype(A&lt;int&gt;::u) a;
```

Please check: https://godbolt.org/z/ovvr6KE1n

Clang crashes while emitting IR for the `a` declared as `decltype(A&lt;int&gt;::u)`, where `u` is declared using an alias template `E` without providing template arguments (ill-formed).

**Output:**  
```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -ferror-limit=0 &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	&lt;source&gt;:8:21: LLVM IR generation of declaration 'a'
3.	&lt;source&gt;:8:21: Generating code for declaration 'a'
 #<!-- -->0 0x0000000003d38488 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3d38488)
 #<!-- -->1 0x0000000003d35e5c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3d35e5c)
 #<!-- -->2 0x0000000003c7cde8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x000079f62ba42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00000000036f8e32 llvm::Type::isIntegerTy(unsigned int) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x36f8e32)
 #<!-- -->5 0x000000000430a7dd clang::CodeGen::CodeGenTypes::ConvertTypeForMem(clang::QualType) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x430a7dd)
 #<!-- -->6 0x00000000040aeec7 clang::CodeGen::CodeGenModule::EmitNullConstant(clang::QualType) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x40aeec7)
 #<!-- -->7 0x000000000428ec84 clang::CodeGen::CodeGenModule::EmitGlobalVarDefinition(clang::VarDecl const*, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x428ec84)
 #<!-- -->8 0x00000000042b0c35 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x42b0c35)
 #<!-- -->9 0x00000000042b18e0 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x42b18e0)
#<!-- -->10 0x00000000042bc1ef clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#<!-- -->11 0x00000000046078b1 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#<!-- -->12 0x00000000045f27b9 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45f27b9)
#<!-- -->13 0x00000000062545b4 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62545b4)
#<!-- -->14 0x0000000004605b55 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4605b55)
#<!-- -->15 0x000000000491659a clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x491659a)
#<!-- -->16 0x00000000048944ab clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x48944ab)
#<!-- -->17 0x0000000004a0c9bb clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4a0c9bb)
#<!-- -->18 0x0000000000de1f5d cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xde1f5d)
#<!-- -->19 0x0000000000dd9ddb ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->20 0x0000000000dd9e7d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->21 0x000000000467fba9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->22 0x0000000003c7d203 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c7d203)
#<!-- -->23 0x000000000467fdc9 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->24 0x0000000004642fb2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4642fb2)
#<!-- -->25 0x0000000004643e91 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4643e91)
#<!-- -->26 0x000000000464c8dc clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x464c8dc)
#<!-- -->27 0x0000000000dde8a9 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdde8a9)
#<!-- -->28 0x0000000000c8f7c4 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc8f7c4)
#<!-- -->29 0x000079f62ba29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->30 0x000079f62ba29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->31 0x0000000000dd9875 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdd9875)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```

The following code is correct:

```cpp
template &lt;class T&gt;
struct A {
  template &lt;class U&gt;
  using E = U;
  static E&lt;int&gt; u;  //correct code
};

decltype(A&lt;int&gt;::u) a;
```
</details>


---

### Comment 3 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (Attacker23)

<details>
The following code has been causing Clang to crash since version 16.0.0 and still does in the current trunk (x86‑64).

```cpp
template &lt;class T&gt;
struct A {
  template &lt;class U&gt;
  using E = U;
  static E u;
};

decltype(A&lt;int&gt;::u) a;
```

Please check: https://godbolt.org/z/ovvr6KE1n

Clang crashes while emitting IR for the `a` declared as `decltype(A&lt;int&gt;::u)`, where `u` is declared using an alias template `E` without providing template arguments (ill-formed).

**Output:**  
```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -ferror-limit=0 &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	&lt;source&gt;:8:21: LLVM IR generation of declaration 'a'
3.	&lt;source&gt;:8:21: Generating code for declaration 'a'
 #<!-- -->0 0x0000000003d38488 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3d38488)
 #<!-- -->1 0x0000000003d35e5c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3d35e5c)
 #<!-- -->2 0x0000000003c7cde8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x000079f62ba42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00000000036f8e32 llvm::Type::isIntegerTy(unsigned int) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x36f8e32)
 #<!-- -->5 0x000000000430a7dd clang::CodeGen::CodeGenTypes::ConvertTypeForMem(clang::QualType) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x430a7dd)
 #<!-- -->6 0x00000000040aeec7 clang::CodeGen::CodeGenModule::EmitNullConstant(clang::QualType) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x40aeec7)
 #<!-- -->7 0x000000000428ec84 clang::CodeGen::CodeGenModule::EmitGlobalVarDefinition(clang::VarDecl const*, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x428ec84)
 #<!-- -->8 0x00000000042b0c35 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x42b0c35)
 #<!-- -->9 0x00000000042b18e0 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x42b18e0)
#<!-- -->10 0x00000000042bc1ef clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#<!-- -->11 0x00000000046078b1 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#<!-- -->12 0x00000000045f27b9 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45f27b9)
#<!-- -->13 0x00000000062545b4 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62545b4)
#<!-- -->14 0x0000000004605b55 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4605b55)
#<!-- -->15 0x000000000491659a clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x491659a)
#<!-- -->16 0x00000000048944ab clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x48944ab)
#<!-- -->17 0x0000000004a0c9bb clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4a0c9bb)
#<!-- -->18 0x0000000000de1f5d cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xde1f5d)
#<!-- -->19 0x0000000000dd9ddb ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->20 0x0000000000dd9e7d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->21 0x000000000467fba9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->22 0x0000000003c7d203 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c7d203)
#<!-- -->23 0x000000000467fdc9 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->24 0x0000000004642fb2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4642fb2)
#<!-- -->25 0x0000000004643e91 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4643e91)
#<!-- -->26 0x000000000464c8dc clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x464c8dc)
#<!-- -->27 0x0000000000dde8a9 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdde8a9)
#<!-- -->28 0x0000000000c8f7c4 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc8f7c4)
#<!-- -->29 0x000079f62ba29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->30 0x000079f62ba29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->31 0x0000000000dd9875 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdd9875)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```

The following code is correct:

```cpp
template &lt;class T&gt;
struct A {
  template &lt;class U&gt;
  using E = U;
  static E&lt;int&gt; u;  //correct code
};

decltype(A&lt;int&gt;::u) a;
```
</details>


---

### Comment 4 - HamzaHassanain

Can I investigate this and work on a fix?

---

### Comment 5 - HamzaHassanain

@frederick-vs-ja 

It crashes with c++ 23, and 20 also

---

### Comment 6 - shafik

> Can I investigate this and work on a fix?

As long as no one else stated they wanted to work on it, put up a PR and then if it looks reasonable we can assign the issue to you.

---

### Comment 7 - HamzaHassanain

> > Can I investigate this and work on a fix?
> 
> As long as no one else stated they wanted to work on it, put up a PR and then if it looks reasonable we can assign the issue to you.

So like a project proposal kind of thing?

---

### Comment 8 - shafik

> > > Can I investigate this and work on a fix?
> > 
> > 
> > As long as no one else stated they wanted to work on it, put up a PR and then if it looks reasonable we can assign the issue to you.
> 
> So like a project proposal kind of thing?

I don't understand the question but there is a "How to contribute page": https://llvm.org/docs/Contributing.html#how-to-submit-a-patch

---

