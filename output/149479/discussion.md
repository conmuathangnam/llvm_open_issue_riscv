# Clang 13-15 segfault during template instantiation with ill-formed nested type and RTTI operation

**Author:** Dong-hui-li
**URL:** https://github.com/llvm/llvm-project/issues/149479
**Status:** Closed
**Labels:** duplicate, c++20, clang:frontend, crash-on-invalid
**Closed Date:** 2025-07-18T17:21:06Z

## Body

### Description
Clang versions 13-15 crash with segmentation fault when compiling C++20 code that:
1. Declares a local class with the same name as an outer scoped enum
2. Attempts to initialize that class with an enum value
3. Uses `typeid` operator on the invalid object

### Steps to Reproduce
1. Save the following code as `crash.cpp`:
```cpp
#include <typeinfo>

struct scoped {
    enum class scoped2 { RED };
};

template<auto>
struct S {
    void f() {
        class scoped2 e = scoped::scoped2::RED;
        typeid(e);
    }
};

int main() {
    S<scoped::scoped2::RED>{}.f();
}
```

2. Compile with affected Clang versions:
```bash
clang++-13 -std=c++20 crash.cpp
clang++-14 -std=c++20 crash.cpp
clang++-15 -std=c++20 crash.cpp
```

### Expected Behavior
Compiler should report type errors without crashing.

### Actual Behavior
Compiler crashes with segmentation fault during template instantiation.

### Affected Versions
- Clang 13.0.1
- Clang 14.0.0
- Clang 15.0.7

### Environment
- OS: Ubuntu 22.04 LTS
- Architecture: x86_64

### Notes
Clang 11 behaves correctly (reports errors without crashing).

### Expected Behavior
Compiler should report type errors without crashing

### Actual Behavior
Compiler crashes with segmentation fault during template instantiation

### Affected Versions
- Clang 13.0.1
- Clang 14.0.0
- Clang 15.0.7

### Additional Info
- Clang 11 behaves correctly (reports errors without crashing)
- Crash occurs in `Sema::SubstStmt` and `Sema::InstantiateFunctionDefinition`
- Stack trace and preprocessed files generated automatically by compiler

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-c-20

Author: None (Dong-hui-li)

<details>
### Description
Clang versions 13-15 crash with segmentation fault when compiling C++20 code that:
1. Declares a local class with the same name as an outer scoped enum
2. Attempts to initialize that class with an enum value
3. Uses `typeid` operator on the invalid object

### Steps to Reproduce
1. Save the following code as `crash.cpp`:
```cpp
#include &lt;typeinfo&gt;

struct scoped {
    enum class scoped2 { RED };
};

template&lt;auto&gt;
struct S {
    void f() {
        class scoped2 e = scoped::scoped2::RED;
        typeid(e);
    }
};

int main() {
    S&lt;scoped::scoped2::RED&gt;{}.f();
}
```

2. Compile with affected Clang versions:
```bash
clang++-13 -std=c++20 crash.cpp
clang++-14 -std=c++20 crash.cpp
clang++-15 -std=c++20 crash.cpp
```

### Expected Behavior
Compiler should report type errors without crashing.

### Actual Behavior
Compiler crashes with segmentation fault during template instantiation.

### Affected Versions
- Clang 13.0.1
- Clang 14.0.0
- Clang 15.0.7

### Environment
- OS: Ubuntu 22.04 LTS
- Architecture: x86_64

### Notes
Clang 11 behaves correctly (reports errors without crashing).

### Expected Behavior
Compiler should report type errors without crashing

### Actual Behavior
Compiler crashes with segmentation fault during template instantiation

### Affected Versions
- Clang 13.0.1
- Clang 14.0.0
- Clang 15.0.7

### Additional Info
- Clang 11 behaves correctly (reports errors without crashing)
- Crash occurs in `Sema::SubstStmt` and `Sema::InstantiateFunctionDefinition`
- Stack trace and preprocessed files generated automatically by compiler
</details>


---

### Comment 2 - DimitryAndric

Still crashes with trunk: https://godbolt.org/z/nPaEfPf39 . Assertion:

```text
clang++: /root/llvm-project/llvm/tools/clang/include/clang/AST/DeclCXX.h:463: clang::CXXRecordDecl::DefinitionData& clang::CXXRecordDecl::data() const: Assertion `DD && "queried property of class with no definition"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 <source>
1.	<eof> parser at end of file
2.	<source>:9:10: instantiating function definition 'S<scoped::scoped2::RED>::f'
 #0 0x0000000003fc0ec8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3fc0ec8)
 #1 0x0000000003fbe2f4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3fbe2f4)
 #2 0x0000000003f02ac8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000076ec3b042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x000076ec3b0969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x000076ec3b042476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x000076ec3b0287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x000076ec3b02871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x000076ec3b039e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x00000000071b0d6e clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCXXTypeidExpr(clang::CXXTypeidExpr*) SemaTemplateInstantiate.cpp:0:0
#10 0x000000000719e25e clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#11 0x00000000071e1c47 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformStmt(clang::Stmt*, clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::StmtDiscardKind) SemaTemplateInstantiate.cpp:0:0
#12 0x00000000071e22ce clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaTemplateInstantiate.cpp:0:0
#13 0x00000000071eb974 clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x71eb974)
#14 0x000000000724bb80 clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x724bb80)
#15 0x0000000007249846 clang::Sema::PerformPendingInstantiations(bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7249846)
#16 0x0000000006818822 clang::Sema::ActOnEndOfTranslationUnitFragment(clang::TUFragmentKind) (.part.0) Sema.cpp:0:0
#17 0x0000000006818ba2 clang::Sema::ActOnEndOfTranslationUnit() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6818ba2)
#18 0x00000000066758a0 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66758a0)
#19 0x000000000666759a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x666759a)
#20 0x0000000004971d48 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4971d48)
#21 0x0000000004c65855 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c65855)
#22 0x0000000004be269e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4be269e)
#23 0x0000000004d59261 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d59261)
#24 0x0000000000d9e37f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd9e37f)
#25 0x0000000000d94f5a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#26 0x00000000049d92c9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#27 0x0000000003f02f64 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f02f64)
#28 0x00000000049d98df clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#29 0x000000000499b92d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x499b92d)
#30 0x000000000499c9be clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x499c9be)
#31 0x00000000049a5505 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49a5505)
#32 0x0000000000d9a82f clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd9a82f)
#33 0x0000000000c4ed64 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc4ed64)
#34 0x000076ec3b029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#35 0x000076ec3b029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#36 0x0000000000d94a05 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd94a05)
```

gcc produces a regular compilation error:

```text
<source>: In instantiation of 'void S<<anonymous> >::f() [with auto <anonymous> = scoped::scoped2::RED]':
<source>:16:32:   required from here
   16 |     S<scoped::scoped2::RED>{}.f();
      |     ~~~~~~~~~~~~~~~~~~~~~~~~~~~^~
<source>:10:23: error: 'S<scoped::scoped2::RED>::f()::scoped2 e' has incomplete type
   10 |         class scoped2 e = scoped::scoped2::RED;
      |                       ^
```


---

### Comment 3 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (Dong-hui-li)

<details>
### Description
Clang versions 13-15 crash with segmentation fault when compiling C++20 code that:
1. Declares a local class with the same name as an outer scoped enum
2. Attempts to initialize that class with an enum value
3. Uses `typeid` operator on the invalid object

### Steps to Reproduce
1. Save the following code as `crash.cpp`:
```cpp
#include &lt;typeinfo&gt;

struct scoped {
    enum class scoped2 { RED };
};

template&lt;auto&gt;
struct S {
    void f() {
        class scoped2 e = scoped::scoped2::RED;
        typeid(e);
    }
};

int main() {
    S&lt;scoped::scoped2::RED&gt;{}.f();
}
```

2. Compile with affected Clang versions:
```bash
clang++-13 -std=c++20 crash.cpp
clang++-14 -std=c++20 crash.cpp
clang++-15 -std=c++20 crash.cpp
```

### Expected Behavior
Compiler should report type errors without crashing.

### Actual Behavior
Compiler crashes with segmentation fault during template instantiation.

### Affected Versions
- Clang 13.0.1
- Clang 14.0.0
- Clang 15.0.7

### Environment
- OS: Ubuntu 22.04 LTS
- Architecture: x86_64

### Notes
Clang 11 behaves correctly (reports errors without crashing).

### Expected Behavior
Compiler should report type errors without crashing

### Actual Behavior
Compiler crashes with segmentation fault during template instantiation

### Affected Versions
- Clang 13.0.1
- Clang 14.0.0
- Clang 15.0.7

### Additional Info
- Clang 11 behaves correctly (reports errors without crashing)
- Crash occurs in `Sema::SubstStmt` and `Sema::InstantiateFunctionDefinition`
- Stack trace and preprocessed files generated automatically by compiler
</details>


---

### Comment 4 - shafik

This is just pared down version of this previous bug report? How are you generating this code? Is this via a fuzzer, if so what is your corpus?

---

### Comment 5 - Dong-hui-li

Hello, I’m sorry I didn’t reply to you in time due to time differences.
​It’s really nice to get your reply, thank you.​
1. No, of course not. As with the two bug reports I wrote before, this comes from the project I’m currently working on.
​2. The code comes from my new project, which I’ll make public once there are results.​
3. This seed generation is from one of the strategies in my project, through llm generation and differential testing.​



---Original---
From: "Shafik ***@***.***&gt;
Date: Sat, Jul 19, 2025 01:21 AM
To: ***@***.***&gt;;
Cc: ***@***.******@***.***&gt;;
Subject: Re: [llvm/llvm-project] Clang 13-15 segfault during templateinstantiation with ill-formed nested type and RTTI operation (Issue #149479)


shafik left a comment (llvm/llvm-project#149479)
 
This is just pared down version of this previous bug report? How are you generating this code? Is this via a fuzzer, if so what is your corpus?
 
—
Reply to this email directly, view it on GitHub, or unsubscribe.
You are receiving this because you authored the thread.Message ID: ***@***.***&gt;

---

### Comment 6 - EugeneZelenko

@Dong-hui-li: Please note that only most recent release (21 as of now) is maintained.

---

