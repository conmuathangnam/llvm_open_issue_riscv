# [clang]Clang assertion failure after diagnosing missing `promise_type::final_suspend` in coroutine template instantiation

**Author:** Attacker23
**URL:** https://github.com/llvm/llvm-project/issues/175720

## Body

Clang **14.0.0 and later** (assertions enabled) crashes when compiling this code in **C++20 mode or later** (`-std=c++20` or newer).

```cpp
#include <coroutine>

struct Tag {};

struct Y {
  bool await_ready() const;
  void await_suspend(...) const;
  void await_resume() const;
};

struct Promise {
  Tag get_return_object();
  std::suspend_always initial_suspend();
  //std::suspend_always final_suspend() noexcept;
  void unhandled_exception();
  Y yield_value(int);
  void return_void();
};

template <class... Args>
struct std::coroutine_traits<Tag, Args...> {
  using promise_type = Promise;
};

template <class T>
struct S {
  template <class U>
  static Tag f() { co_yield 0; }
};

Tag g() { return S<int>::f<void>(); }
```
**https://godbolt.org/z/qvjK84f6h**

It first emits the expected diagnostic (`no member named 'final_suspend' in 'Promise'`) for an ill-formed coroutine, but then hits an assertion failure during template instantiation while transforming the coroutine body (`TransformCoyieldExpr`), ending in `LocalInstantiationScope::findInstantiationOf` in `SemaTemplateInstantiate.cpp`. The compiler should diagnose the missing `final_suspend` and recover.

# Output:

```
<source>:28:14: error: no member named 'final_suspend' in 'Promise'
   28 |   static Tag f() { co_yield 0; }
      |              ^
clang++: /root/llvm-project/llvm/tools/clang/lib/Sema/SemaTemplateInstantiate.cpp:4632: llvm::PointerUnion<clang::Decl*, llvm::SmallVector<clang::ValueDecl*, 4>*>* clang::LocalInstantiationScope::findInstantiationOf(const clang::Decl*): Assertion `isa<LabelDecl>(D) && "declaration not instantiated in this scope"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 <source>
1.	<eof> parser at end of file
2.	<source>:28:14: instantiating function definition 'S<int>::f<void>'
 #0 0x00000000042fea18 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42fea18)
 #1 0x00000000042fbe54 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42fbe54)
 #2 0x000000000423e748 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007fe4ae642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007fe4ae6969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007fe4ae642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007fe4ae6287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007fe4ae62871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007fe4ae639e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x000000000758598d clang::LocalInstantiationScope::findInstantiationOf(clang::Decl const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x758598d)
#10 0x000000000766d017 clang::Sema::FindInstantiatedDecl(clang::SourceLocation, clang::NamedDecl*, clang::MultiLevelTemplateArgumentList const&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x766d017)
#11 0x00000000075ac9f3 (anonymous namespace)::TemplateInstantiator::TransformDecl(clang::SourceLocation, clang::Decl*) SemaTemplateInstantiate.cpp:0:0
#12 0x00000000075c2190 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformDeclRefExpr(clang::DeclRefExpr*) SemaTemplateInstantiate.cpp:0:0
#13 0x00000000075c269b (anonymous namespace)::TemplateInstantiator::TransformDeclRefExpr(clang::DeclRefExpr*) SemaTemplateInstantiate.cpp:0:0
#14 0x0000000007592283 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#15 0x00000000075c14e0 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformMemberExpr(clang::MemberExpr*) SemaTemplateInstantiate.cpp:0:0
#16 0x00000000075923c7 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#17 0x00000000075b53d6 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCallExpr(clang::CallExpr*) SemaTemplateInstantiate.cpp:0:0
#18 0x000000000759217c clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#19 0x00000000075b1f15 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformInitializer(clang::Expr*, bool) (.part.0) SemaTemplateInstantiate.cpp:0:0
#20 0x00000000075b257c clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCoyieldExpr(clang::CoyieldExpr*) SemaTemplateInstantiate.cpp:0:0
#21 0x000000000759226b clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#22 0x00000000075922f0 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#23 0x00000000075cf3b7 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformStmt(clang::Stmt*, clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::StmtDiscardKind) SemaTemplateInstantiate.cpp:0:0
#24 0x00000000075cfa65 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaTemplateInstantiate.cpp:0:0
#25 0x00000000075d8e9d clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x75d8e9d)
#26 0x00000000076751d0 clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x76751d0)
#27 0x0000000007672d96 clang::Sema::PerformPendingInstantiations(bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7672d96)
#28 0x0000000006bee482 clang::Sema::ActOnEndOfTranslationUnitFragment(clang::TUFragmentKind) (.part.0) Sema.cpp:0:0
#29 0x0000000006bee802 clang::Sema::ActOnEndOfTranslationUnit() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bee802)
#30 0x0000000006a597d5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a597d5)
#31 0x0000000006a3a78a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a3a78a)
#32 0x0000000004c66488 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c66488)
#33 0x0000000004f60135 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f60135)
#34 0x0000000004edff0e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4edff0e)
#35 0x000000000505a7cd clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x505a7cd)
#36 0x0000000000dce92e cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdce92e)
#37 0x0000000000dc52fa ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#38 0x0000000000dc547d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#39 0x0000000004cd96e9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#40 0x000000000423ebe4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x423ebe4)
#41 0x0000000004cd9cff clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#42 0x0000000004c9a692 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c9a692)
#43 0x0000000004c9b63e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c9b63e)
#44 0x0000000004ca2a95 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ca2a95)
#45 0x0000000000dcaca1 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdcaca1)
#46 0x0000000000c88434 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc88434)
#47 0x00007fe4ae629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#48 0x00007fe4ae629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#49 0x0000000000dc4d95 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdc4d95)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (Attacker23)

<details>
Clang **14.0.0 and later** (assertions enabled) crashes when compiling this code in **C++20 mode or later** (`-std=c++20` or newer).

```cpp
#include &lt;coroutine&gt;

struct Tag {};

struct Y {
  bool await_ready() const;
  void await_suspend(...) const;
  void await_resume() const;
};

struct Promise {
  Tag get_return_object();
  std::suspend_always initial_suspend();
  //std::suspend_always final_suspend() noexcept;
  void unhandled_exception();
  Y yield_value(int);
  void return_void();
};

template &lt;class... Args&gt;
struct std::coroutine_traits&lt;Tag, Args...&gt; {
  using promise_type = Promise;
};

template &lt;class T&gt;
struct S {
  template &lt;class U&gt;
  static Tag f() { co_yield 0; }
};

Tag g() { return S&lt;int&gt;::f&lt;void&gt;(); }
```
**https://godbolt.org/z/qvjK84f6h**

It first emits the expected diagnostic (`no member named 'final_suspend' in 'Promise'`) for an ill-formed coroutine, but then hits an assertion failure during template instantiation while transforming the coroutine body (`TransformCoyieldExpr`), ending in `LocalInstantiationScope::findInstantiationOf` in `SemaTemplateInstantiate.cpp`. The compiler should diagnose the missing `final_suspend` and recover.

# Output:

```
&lt;source&gt;:28:14: error: no member named 'final_suspend' in 'Promise'
   28 |   static Tag f() { co_yield 0; }
      |              ^
clang++: /root/llvm-project/llvm/tools/clang/lib/Sema/SemaTemplateInstantiate.cpp:4632: llvm::PointerUnion&lt;clang::Decl*, llvm::SmallVector&lt;clang::ValueDecl*, 4&gt;*&gt;* clang::LocalInstantiationScope::findInstantiationOf(const clang::Decl*): Assertion `isa&lt;LabelDecl&gt;(D) &amp;&amp; "declaration not instantiated in this scope"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	&lt;source&gt;:28:14: instantiating function definition 'S&lt;int&gt;::f&lt;void&gt;'
 #<!-- -->0 0x00000000042fea18 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42fea18)
 #<!-- -->1 0x00000000042fbe54 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42fbe54)
 #<!-- -->2 0x000000000423e748 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007fe4ae642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007fe4ae6969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007fe4ae642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007fe4ae6287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x00007fe4ae62871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x00007fe4ae639e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x000000000758598d clang::LocalInstantiationScope::findInstantiationOf(clang::Decl const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x758598d)
#<!-- -->10 0x000000000766d017 clang::Sema::FindInstantiatedDecl(clang::SourceLocation, clang::NamedDecl*, clang::MultiLevelTemplateArgumentList const&amp;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x766d017)
#<!-- -->11 0x00000000075ac9f3 (anonymous namespace)::TemplateInstantiator::TransformDecl(clang::SourceLocation, clang::Decl*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->12 0x00000000075c2190 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformDeclRefExpr(clang::DeclRefExpr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->13 0x00000000075c269b (anonymous namespace)::TemplateInstantiator::TransformDeclRefExpr(clang::DeclRefExpr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->14 0x0000000007592283 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->15 0x00000000075c14e0 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformMemberExpr(clang::MemberExpr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->16 0x00000000075923c7 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->17 0x00000000075b53d6 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCallExpr(clang::CallExpr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->18 0x000000000759217c clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->19 0x00000000075b1f15 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformInitializer(clang::Expr*, bool) (.part.0) SemaTemplateInstantiate.cpp:0:0
#<!-- -->20 0x00000000075b257c clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCoyieldExpr(clang::CoyieldExpr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->21 0x000000000759226b clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->22 0x00000000075922f0 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->23 0x00000000075cf3b7 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformStmt(clang::Stmt*, clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::StmtDiscardKind) SemaTemplateInstantiate.cpp:0:0
#<!-- -->24 0x00000000075cfa65 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaTemplateInstantiate.cpp:0:0
#<!-- -->25 0x00000000075d8e9d clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x75d8e9d)
#<!-- -->26 0x00000000076751d0 clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x76751d0)
#<!-- -->27 0x0000000007672d96 clang::Sema::PerformPendingInstantiations(bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7672d96)
#<!-- -->28 0x0000000006bee482 clang::Sema::ActOnEndOfTranslationUnitFragment(clang::TUFragmentKind) (.part.0) Sema.cpp:0:0
#<!-- -->29 0x0000000006bee802 clang::Sema::ActOnEndOfTranslationUnit() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bee802)
#<!-- -->30 0x0000000006a597d5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a597d5)
#<!-- -->31 0x0000000006a3a78a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a3a78a)
#<!-- -->32 0x0000000004c66488 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c66488)
#<!-- -->33 0x0000000004f60135 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f60135)
#<!-- -->34 0x0000000004edff0e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4edff0e)
#<!-- -->35 0x000000000505a7cd clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x505a7cd)
#<!-- -->36 0x0000000000dce92e cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdce92e)
#<!-- -->37 0x0000000000dc52fa ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->38 0x0000000000dc547d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->39 0x0000000004cd96e9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->40 0x000000000423ebe4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x423ebe4)
#<!-- -->41 0x0000000004cd9cff clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->42 0x0000000004c9a692 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c9a692)
#<!-- -->43 0x0000000004c9b63e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c9b63e)
#<!-- -->44 0x0000000004ca2a95 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ca2a95)
#<!-- -->45 0x0000000000dcaca1 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdcaca1)
#<!-- -->46 0x0000000000c88434 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc88434)
#<!-- -->47 0x00007fe4ae629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->48 0x00007fe4ae629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->49 0x0000000000dc4d95 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdc4d95)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-c-20

Author: None (Attacker23)

<details>
Clang **14.0.0 and later** (assertions enabled) crashes when compiling this code in **C++20 mode or later** (`-std=c++20` or newer).

```cpp
#include &lt;coroutine&gt;

struct Tag {};

struct Y {
  bool await_ready() const;
  void await_suspend(...) const;
  void await_resume() const;
};

struct Promise {
  Tag get_return_object();
  std::suspend_always initial_suspend();
  //std::suspend_always final_suspend() noexcept;
  void unhandled_exception();
  Y yield_value(int);
  void return_void();
};

template &lt;class... Args&gt;
struct std::coroutine_traits&lt;Tag, Args...&gt; {
  using promise_type = Promise;
};

template &lt;class T&gt;
struct S {
  template &lt;class U&gt;
  static Tag f() { co_yield 0; }
};

Tag g() { return S&lt;int&gt;::f&lt;void&gt;(); }
```
**https://godbolt.org/z/qvjK84f6h**

It first emits the expected diagnostic (`no member named 'final_suspend' in 'Promise'`) for an ill-formed coroutine, but then hits an assertion failure during template instantiation while transforming the coroutine body (`TransformCoyieldExpr`), ending in `LocalInstantiationScope::findInstantiationOf` in `SemaTemplateInstantiate.cpp`. The compiler should diagnose the missing `final_suspend` and recover.

# Output:

```
&lt;source&gt;:28:14: error: no member named 'final_suspend' in 'Promise'
   28 |   static Tag f() { co_yield 0; }
      |              ^
clang++: /root/llvm-project/llvm/tools/clang/lib/Sema/SemaTemplateInstantiate.cpp:4632: llvm::PointerUnion&lt;clang::Decl*, llvm::SmallVector&lt;clang::ValueDecl*, 4&gt;*&gt;* clang::LocalInstantiationScope::findInstantiationOf(const clang::Decl*): Assertion `isa&lt;LabelDecl&gt;(D) &amp;&amp; "declaration not instantiated in this scope"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	&lt;source&gt;:28:14: instantiating function definition 'S&lt;int&gt;::f&lt;void&gt;'
 #<!-- -->0 0x00000000042fea18 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42fea18)
 #<!-- -->1 0x00000000042fbe54 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42fbe54)
 #<!-- -->2 0x000000000423e748 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007fe4ae642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007fe4ae6969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007fe4ae642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007fe4ae6287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x00007fe4ae62871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x00007fe4ae639e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x000000000758598d clang::LocalInstantiationScope::findInstantiationOf(clang::Decl const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x758598d)
#<!-- -->10 0x000000000766d017 clang::Sema::FindInstantiatedDecl(clang::SourceLocation, clang::NamedDecl*, clang::MultiLevelTemplateArgumentList const&amp;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x766d017)
#<!-- -->11 0x00000000075ac9f3 (anonymous namespace)::TemplateInstantiator::TransformDecl(clang::SourceLocation, clang::Decl*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->12 0x00000000075c2190 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformDeclRefExpr(clang::DeclRefExpr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->13 0x00000000075c269b (anonymous namespace)::TemplateInstantiator::TransformDeclRefExpr(clang::DeclRefExpr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->14 0x0000000007592283 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->15 0x00000000075c14e0 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformMemberExpr(clang::MemberExpr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->16 0x00000000075923c7 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->17 0x00000000075b53d6 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCallExpr(clang::CallExpr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->18 0x000000000759217c clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->19 0x00000000075b1f15 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformInitializer(clang::Expr*, bool) (.part.0) SemaTemplateInstantiate.cpp:0:0
#<!-- -->20 0x00000000075b257c clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCoyieldExpr(clang::CoyieldExpr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->21 0x000000000759226b clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->22 0x00000000075922f0 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->23 0x00000000075cf3b7 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformStmt(clang::Stmt*, clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::StmtDiscardKind) SemaTemplateInstantiate.cpp:0:0
#<!-- -->24 0x00000000075cfa65 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaTemplateInstantiate.cpp:0:0
#<!-- -->25 0x00000000075d8e9d clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x75d8e9d)
#<!-- -->26 0x00000000076751d0 clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x76751d0)
#<!-- -->27 0x0000000007672d96 clang::Sema::PerformPendingInstantiations(bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7672d96)
#<!-- -->28 0x0000000006bee482 clang::Sema::ActOnEndOfTranslationUnitFragment(clang::TUFragmentKind) (.part.0) Sema.cpp:0:0
#<!-- -->29 0x0000000006bee802 clang::Sema::ActOnEndOfTranslationUnit() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bee802)
#<!-- -->30 0x0000000006a597d5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a597d5)
#<!-- -->31 0x0000000006a3a78a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a3a78a)
#<!-- -->32 0x0000000004c66488 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c66488)
#<!-- -->33 0x0000000004f60135 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f60135)
#<!-- -->34 0x0000000004edff0e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4edff0e)
#<!-- -->35 0x000000000505a7cd clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x505a7cd)
#<!-- -->36 0x0000000000dce92e cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdce92e)
#<!-- -->37 0x0000000000dc52fa ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->38 0x0000000000dc547d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->39 0x0000000004cd96e9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->40 0x000000000423ebe4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x423ebe4)
#<!-- -->41 0x0000000004cd9cff clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->42 0x0000000004c9a692 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c9a692)
#<!-- -->43 0x0000000004c9b63e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c9b63e)
#<!-- -->44 0x0000000004ca2a95 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ca2a95)
#<!-- -->45 0x0000000000dcaca1 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdcaca1)
#<!-- -->46 0x0000000000c88434 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc88434)
#<!-- -->47 0x00007fe4ae629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->48 0x00007fe4ae629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->49 0x0000000000dc4d95 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdc4d95)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

