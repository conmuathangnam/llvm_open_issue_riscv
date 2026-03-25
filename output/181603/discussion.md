# ICE-on-invalid when defining a friend function inside a template struct

**Author:** Alcaro
**URL:** https://github.com/llvm/llvm-project/issues/181603

## Body

```c++
template<int f>
void h();
template<int a>
struct b {
    friend void h<1>() {}
};
template struct b<5>;
void d()
{
    h<1>();
}
```
No flags needed. https://godbolt.org/z/aj365h8cK

Output:
<details>
<summary>&lt;source>:5:17: error: friend function specialization cannot be defined</summary>

```
<source>:5:17: error: friend function specialization cannot be defined
    5 |     friend void h<1>() {}
      |                 ^
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-21.1.0/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-15.2.0 -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<eof> parser at end of file
2.	<source>:5:17: instantiating function definition 'h<1>'
 #0 0x0000000003bfa6c8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x3bfa6c8)
 #1 0x0000000003bf809c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x3bf809c)
 #2 0x0000000003b483f8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007e5477a42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x0000000000c47269 clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (.cold) SemaTemplateInstantiateDecl.cpp:0:0
 #5 0x0000000006d892d8 clang::Sema::PerformPendingInstantiations(bool, bool) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x6d892d8)
 #6 0x0000000006350ec9 clang::Sema::ActOnEndOfTranslationUnitFragment(clang::TUFragmentKind) (.part.0) Sema.cpp:0:0
 #7 0x00000000063511a4 clang::Sema::ActOnEndOfTranslationUnit() (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x63511a4)
 #8 0x00000000061a0888 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x61a0888)
 #9 0x000000000618ef7a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x618ef7a)
#10 0x000000000454985d clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x454985d)
#11 0x000000000485b83a clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x485b83a)
#12 0x00000000047d655b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x47d655b)
#13 0x000000000494e513 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x494e513)
#14 0x0000000000da5f25 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0xda5f25)
#15 0x0000000000d9de2d ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#16 0x00000000045bb869 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#17 0x0000000003b48813 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x3b48813)
#18 0x00000000045bba89 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#19 0x000000000457eddd clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x457eddd)
#20 0x000000000457fd91 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x457fd91)
#21 0x000000000458d29c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x458d29c)
#22 0x0000000000da2751 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0xda2751)
#23 0x0000000000c526f4 main (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0xc526f4)
#24 0x00007e5477a29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#25 0x00007e5477a29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#26 0x0000000000d9d8c5 _start (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0xd9d8c5)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```
</details>
Expected: Just the first three lines

## Comments

### Comment 1 - keinflue

Regression since Clang 20: https://godbolt.org/z/cvdEEqWMj

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (Alcaro)

<details>
```c++
template&lt;int f&gt;
void h();
template&lt;int a&gt;
struct b {
    friend void h&lt;1&gt;() {}
};
template struct b&lt;5&gt;;
void d()
{
    h&lt;1&gt;();
}
```
No flags needed. https://godbolt.org/z/aj365h8cK

Output:
&lt;details&gt;
&lt;summary&gt;&amp;lt;source&gt;:5:17: error: friend function specialization cannot be defined&lt;/summary&gt;

```
&lt;source&gt;:5:17: error: friend function specialization cannot be defined
    5 |     friend void h&lt;1&gt;() {}
      |                 ^
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-21.1.0/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-15.2.0 -fcolor-diagnostics -fno-crash-diagnostics &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	&lt;source&gt;:5:17: instantiating function definition 'h&lt;1&gt;'
 #<!-- -->0 0x0000000003bfa6c8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x3bfa6c8)
 #<!-- -->1 0x0000000003bf809c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x3bf809c)
 #<!-- -->2 0x0000000003b483f8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007e5477a42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x0000000000c47269 clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (.cold) SemaTemplateInstantiateDecl.cpp:0:0
 #<!-- -->5 0x0000000006d892d8 clang::Sema::PerformPendingInstantiations(bool, bool) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x6d892d8)
 #<!-- -->6 0x0000000006350ec9 clang::Sema::ActOnEndOfTranslationUnitFragment(clang::TUFragmentKind) (.part.0) Sema.cpp:0:0
 #<!-- -->7 0x00000000063511a4 clang::Sema::ActOnEndOfTranslationUnit() (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x63511a4)
 #<!-- -->8 0x00000000061a0888 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x61a0888)
 #<!-- -->9 0x000000000618ef7a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x618ef7a)
#<!-- -->10 0x000000000454985d clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x454985d)
#<!-- -->11 0x000000000485b83a clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x485b83a)
#<!-- -->12 0x00000000047d655b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x47d655b)
#<!-- -->13 0x000000000494e513 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x494e513)
#<!-- -->14 0x0000000000da5f25 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0xda5f25)
#<!-- -->15 0x0000000000d9de2d ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->16 0x00000000045bb869 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->17 0x0000000003b48813 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x3b48813)
#<!-- -->18 0x00000000045bba89 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->19 0x000000000457eddd clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x457eddd)
#<!-- -->20 0x000000000457fd91 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x457fd91)
#<!-- -->21 0x000000000458d29c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x458d29c)
#<!-- -->22 0x0000000000da2751 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0xda2751)
#<!-- -->23 0x0000000000c526f4 main (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0xc526f4)
#<!-- -->24 0x00007e5477a29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->25 0x00007e5477a29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->26 0x0000000000d9d8c5 _start (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0xd9d8c5)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```
&lt;/details&gt;
Expected: Just the first three lines
</details>


---

### Comment 3 - Iasonaskrpr

I would like to work on this

---

### Comment 4 - shafik

> I would like to work on this

Note, we don't assign issues until after a PR has been created and it is verified that it is moving in the right direction.

---

### Comment 5 - Iasonaskrpr

I have opened a PR that fixes this issue I would appreciate a review!

---

### Comment 6 - shafik

@mizvekov this bisect to 346077aaa6bef5652a72a2f3d9fc134ea8fc6a5b

CC @Iasonaskrpr 

---

### Comment 7 - Iasonaskrpr

I found another ICE for the same code that was not a regression from this commit, while I have fixed  this specific issue from this commit the same code crashes, this happens on clang-19 also, with a different error https://godbolt.org/z/fs7ooTafj. Should I try to fix this as well or open a new issue since this error is not linked specifically to this one?

---

### Comment 8 - mizvekov

I'd at least look at the new issue first, maybe the problem is more general and that can give you more information.

---

### Comment 9 - Iasonaskrpr

Okey I will look at it because this may be a more general issue that was exposed by your changes 

---

