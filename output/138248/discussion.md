# Assertion failure in TreeTransform::TransformInitializer with templated constructor and invalid type usage in Clang

**Author:** mariete1223
**URL:** https://github.com/llvm/llvm-project/issues/138248

## Body

## Summary 

Clang crashes with an assertion failure during template instantiation when attempting to use a templated constructor that involves incorrect usage of typeid(T1*) and object initialization.

## Assertion

```
clang++: /root/llvm-project/llvm/tools/clang/lib/Sema/TreeTransform.h:4424: clang::ExprResult clang::TreeTransform<Derived>::TransformInitializer(clang::Expr*, bool) [with Derived = {anonymous}::TemplateInstantiator; clang::ExprResult = clang::ActionResult<clang::Expr*>]: Assertion `NewArgs.empty() && "no parens or braces but have direct init with arguments?"' failed.
```

## Stack dump

```
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x c++ <source>
1.	<eof> parser at end of file
2.	<source>:7:5: instantiating function definition 'print<int *>::print<int>'
 #0 0x0000000003f6f428 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f6f428)
 #1 0x0000000003f6d0b4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f6d0b4)
 #2 0x0000000003eb2098 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007caff5442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007caff54969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007caff5442476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007caff54287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007caff542871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007caff5439e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000007186943 (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7186943)
#10 0x0000000007186bdb clang::Sema::SubstInitializer(clang::Expr*, clang::MultiLevelTemplateArgumentList const&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7186bdb)
#11 0x0000000007227a8c clang::Sema::InstantiateMemInitializers(clang::CXXConstructorDecl*, clang::CXXConstructorDecl const*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7227a8c)
#12 0x000000000722dfa5 clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x722dfa5)
#13 0x000000000722bdf6 clang::Sema::PerformPendingInstantiations(bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x722bdf6)
#14 0x00000000067800d2 clang::Sema::ActOnEndOfTranslationUnitFragment(clang::TUFragmentKind) (.part.0) Sema.cpp:0:0
#15 0x0000000006780972 clang::Sema::ActOnEndOfTranslationUnit() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6780972)
#16 0x00000000065ef88b clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65ef88b)
#17 0x00000000065e1b8a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65e1b8a)
#18 0x0000000004910eb8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4910eb8)
#19 0x0000000004c03675 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c03675)
#20 0x0000000004b82cfe clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b82cfe)
#21 0x0000000004cf74f9 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4cf74f9)
#22 0x0000000000da554f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda554f)
#23 0x0000000000d9b71a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#24 0x0000000004977769 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#25 0x0000000003eb2534 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3eb2534)
#26 0x0000000004977d7f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#27 0x000000000493a1cd clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x493a1cd)
#28 0x000000000493b25e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x493b25e)
#29 0x00000000049430b5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49430b5)
#30 0x0000000000da14b8 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda14b8)
#31 0x0000000000c26ce4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc26ce4)
#32 0x00007caff5429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#33 0x00007caff5429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#34 0x0000000000d9b1c5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd9b1c5)
```

## Program

```
#include <iostream>
template<typename T>
struct print<T*>
{
    std::string const& name;
    template<typename T1>
    print(T1 t) : name(typeid(T1*).name()) {
       typeid(&t);
    }
};
int main()
{
    print<int*>(42);
}
```

## To quickly reproduce

It happens since clang 6 until now

https://gcc.godbolt.org/z/5Mh1zaEK7

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (mariete1223)

<details>
## Summary 

Clang crashes with an assertion failure during template instantiation when attempting to use a templated constructor that involves incorrect usage of typeid(T1*) and object initialization.

## Assertion

```
clang++: /root/llvm-project/llvm/tools/clang/lib/Sema/TreeTransform.h:4424: clang::ExprResult clang::TreeTransform&lt;Derived&gt;::TransformInitializer(clang::Expr*, bool) [with Derived = {anonymous}::TemplateInstantiator; clang::ExprResult = clang::ActionResult&lt;clang::Expr*&gt;]: Assertion `NewArgs.empty() &amp;&amp; "no parens or braces but have direct init with arguments?"' failed.
```

## Stack dump

```
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x c++ &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	&lt;source&gt;:7:5: instantiating function definition 'print&lt;int *&gt;::print&lt;int&gt;'
 #<!-- -->0 0x0000000003f6f428 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f6f428)
 #<!-- -->1 0x0000000003f6d0b4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f6d0b4)
 #<!-- -->2 0x0000000003eb2098 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007caff5442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007caff54969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007caff5442476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007caff54287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x00007caff542871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x00007caff5439e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000007186943 (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7186943)
#<!-- -->10 0x0000000007186bdb clang::Sema::SubstInitializer(clang::Expr*, clang::MultiLevelTemplateArgumentList const&amp;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7186bdb)
#<!-- -->11 0x0000000007227a8c clang::Sema::InstantiateMemInitializers(clang::CXXConstructorDecl*, clang::CXXConstructorDecl const*, clang::MultiLevelTemplateArgumentList const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7227a8c)
#<!-- -->12 0x000000000722dfa5 clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x722dfa5)
#<!-- -->13 0x000000000722bdf6 clang::Sema::PerformPendingInstantiations(bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x722bdf6)
#<!-- -->14 0x00000000067800d2 clang::Sema::ActOnEndOfTranslationUnitFragment(clang::TUFragmentKind) (.part.0) Sema.cpp:0:0
#<!-- -->15 0x0000000006780972 clang::Sema::ActOnEndOfTranslationUnit() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6780972)
#<!-- -->16 0x00000000065ef88b clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65ef88b)
#<!-- -->17 0x00000000065e1b8a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65e1b8a)
#<!-- -->18 0x0000000004910eb8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4910eb8)
#<!-- -->19 0x0000000004c03675 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c03675)
#<!-- -->20 0x0000000004b82cfe clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b82cfe)
#<!-- -->21 0x0000000004cf74f9 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4cf74f9)
#<!-- -->22 0x0000000000da554f cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda554f)
#<!-- -->23 0x0000000000d9b71a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->24 0x0000000004977769 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->25 0x0000000003eb2534 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3eb2534)
#<!-- -->26 0x0000000004977d7f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->27 0x000000000493a1cd clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x493a1cd)
#<!-- -->28 0x000000000493b25e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x493b25e)
#<!-- -->29 0x00000000049430b5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49430b5)
#<!-- -->30 0x0000000000da14b8 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda14b8)
#<!-- -->31 0x0000000000c26ce4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc26ce4)
#<!-- -->32 0x00007caff5429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->33 0x00007caff5429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->34 0x0000000000d9b1c5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd9b1c5)
```

## Program

```
#include &lt;iostream&gt;
template&lt;typename T&gt;
struct print&lt;T*&gt;
{
    std::string const&amp; name;
    template&lt;typename T1&gt;
    print(T1 t) : name(typeid(T1*).name()) {
       typeid(&amp;t);
    }
};
int main()
{
    print&lt;int*&gt;(42);
}
```

## To quickly reproduce

It happens since clang 6 until now

https://gcc.godbolt.org/z/5Mh1zaEK7
</details>


---

