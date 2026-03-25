# Clang assertions failed when promise_type::get_return_object is a function template in C++20 coroutine

**Author:** AbyssStaror
**URL:** https://github.com/llvm/llvm-project/issues/173664

## Body

### Link for quick verification: https://godbolt.org/z/3vb38fTPv
```c
#include <coroutine>
#include <iostream>

template<typename... Args>
struct coroutine_test {
    struct promise_type {
        // question is here
        template<class T>
        auto get_return_object(T&&) {
            return coroutine_test{};
        }
        
        std::suspend_never initial_suspend() { return {}; }
        std::suspend_never final_suspend() noexcept { return {}; }
        void unhandled_exception() {}
        void return_value(int) {}
    };

    static coroutine_test foo() {
       co_return 42; 
   }
};

int main() {
   auto cobj_foo = coroutine_test<>::foo();
}
```
Go back to `clang 14.1.0 (assertions)`. And this issue is specifically reproducible when using -std=c++20 or later.
Clang crashes with an assertion failure when compiling a C++20 coroutine whose `promise_type::get_return_object` is a function template.
The crash occurs during coroutine semantic analysis in `SemaCoroutine.cpp`, hitting the assertion.
## Stack dump:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/Sema/SemaCoroutine.cpp:1826: bool clang::CoroutineStmtBuilder::makeGroDeclAndReturnStmt(): Assertion `!GroType->isDependentType() && "get_return_object type must no longer be dependent"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 <source>
1.	<eof> parser at end of file
2.	<source>:19:27: instantiating function definition 'coroutine_test<>::foo'
 #0 0x00000000042ecab8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42ecab8)
 #1 0x00000000042e9ee4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42e9ee4)
 #2 0x000000000422ec88 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x0000778975c42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x0000778975c969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x0000778975c42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x0000778975c287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x0000778975c2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x0000778975c39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000006d8365b clang::CoroutineStmtBuilder::makeGroDeclAndReturnStmt() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d8365b)
#10 0x0000000006d8afa4 clang::CoroutineStmtBuilder::buildDependentStatements() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d8afa4)
#11 0x00000000075a18e9 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCoroutineBodyStmt(clang::CoroutineBodyStmt*) SemaTemplateInstantiate.cpp:0:0
#12 0x00000000075a2d0d clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x75a2d0d)
#13 0x000000000763f070 clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x763f070)
#14 0x000000000763cc36 clang::Sema::PerformPendingInstantiations(bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x763cc36)
#15 0x0000000006bbd072 clang::Sema::ActOnEndOfTranslationUnitFragment(clang::TUFragmentKind) (.part.0) Sema.cpp:0:0
#16 0x0000000006bbd3f2 clang::Sema::ActOnEndOfTranslationUnit() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bbd3f2)
#17 0x0000000006a27e15 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a27e15)
#18 0x0000000006a08dca clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a08dca)
#19 0x0000000004c4dde8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c4dde8)
#20 0x0000000004f446e5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f446e5)
#21 0x0000000004ec497e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ec497e)
#22 0x000000000503ea6d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x503ea6d)
#23 0x0000000000de5b4c cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde5b4c)
#24 0x0000000000ddc59a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#25 0x0000000000ddc71d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#26 0x0000000004cc0739 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#27 0x000000000422f124 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x422f124)
#28 0x0000000004cc0d4f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#29 0x0000000004c81832 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c81832)
#30 0x0000000004c827de clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c827de)
#31 0x0000000004c89c35 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c89c35)
#32 0x0000000000de1f41 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde1f41)
#33 0x0000000000c8cf94 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc8cf94)
#34 0x0000778975c29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#35 0x0000778975c29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#36 0x0000000000ddc035 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xddc035)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (AbyssStaror)

<details>
### Link for quick verification: https://godbolt.org/z/3vb38fTPv
```c
#include &lt;coroutine&gt;
#include &lt;iostream&gt;

template&lt;typename... Args&gt;
struct coroutine_test {
    struct promise_type {
        template&lt;class T&gt;
        auto get_return_object(T&amp;&amp;) {
            return coroutine_test{};
        }
        
        std::suspend_never initial_suspend() { return {}; }
        std::suspend_never final_suspend() noexcept { return {}; }
        void unhandled_exception() {}
        void return_value(int) {}
    };

    static coroutine_test foo() {
       co_return 42; 
   }
};

int main() {
   auto cobj_foo = coroutine_test&lt;&gt;::foo();
}
```
Go back to `clang 14.1.0 (assertions)`. And this issue is specifically reproducible when using -std=c++20 or later.
Clang crashes with an assertion failure when compiling a C++20 coroutine whose `promise_type::get_return_object` is a function template.
The crash occurs during coroutine semantic analysis in `SemaCoroutine.cpp`, hitting the assertion.
## Stack dump:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/Sema/SemaCoroutine.cpp:1826: bool clang::CoroutineStmtBuilder::makeGroDeclAndReturnStmt(): Assertion `!GroType-&gt;isDependentType() &amp;&amp; "get_return_object type must no longer be dependent"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	&lt;source&gt;:19:27: instantiating function definition 'coroutine_test&lt;&gt;::foo'
 #<!-- -->0 0x00000000042ecab8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42ecab8)
 #<!-- -->1 0x00000000042e9ee4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42e9ee4)
 #<!-- -->2 0x000000000422ec88 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x0000778975c42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x0000778975c969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x0000778975c42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x0000778975c287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x0000778975c2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x0000778975c39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000006d8365b clang::CoroutineStmtBuilder::makeGroDeclAndReturnStmt() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d8365b)
#<!-- -->10 0x0000000006d8afa4 clang::CoroutineStmtBuilder::buildDependentStatements() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d8afa4)
#<!-- -->11 0x00000000075a18e9 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCoroutineBodyStmt(clang::CoroutineBodyStmt*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->12 0x00000000075a2d0d clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x75a2d0d)
#<!-- -->13 0x000000000763f070 clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x763f070)
#<!-- -->14 0x000000000763cc36 clang::Sema::PerformPendingInstantiations(bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x763cc36)
#<!-- -->15 0x0000000006bbd072 clang::Sema::ActOnEndOfTranslationUnitFragment(clang::TUFragmentKind) (.part.0) Sema.cpp:0:0
#<!-- -->16 0x0000000006bbd3f2 clang::Sema::ActOnEndOfTranslationUnit() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bbd3f2)
#<!-- -->17 0x0000000006a27e15 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a27e15)
#<!-- -->18 0x0000000006a08dca clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a08dca)
#<!-- -->19 0x0000000004c4dde8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c4dde8)
#<!-- -->20 0x0000000004f446e5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f446e5)
#<!-- -->21 0x0000000004ec497e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ec497e)
#<!-- -->22 0x000000000503ea6d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x503ea6d)
#<!-- -->23 0x0000000000de5b4c cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde5b4c)
#<!-- -->24 0x0000000000ddc59a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->25 0x0000000000ddc71d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->26 0x0000000004cc0739 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->27 0x000000000422f124 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x422f124)
#<!-- -->28 0x0000000004cc0d4f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->29 0x0000000004c81832 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c81832)
#<!-- -->30 0x0000000004c827de clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c827de)
#<!-- -->31 0x0000000004c89c35 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c89c35)
#<!-- -->32 0x0000000000de1f41 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde1f41)
#<!-- -->33 0x0000000000c8cf94 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc8cf94)
#<!-- -->34 0x0000778975c29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->35 0x0000778975c29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->36 0x0000000000ddc035 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xddc035)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-coroutines

Author: None (AbyssStaror)

<details>
### Link for quick verification: https://godbolt.org/z/3vb38fTPv
```c
#include &lt;coroutine&gt;
#include &lt;iostream&gt;

template&lt;typename... Args&gt;
struct coroutine_test {
    struct promise_type {
        template&lt;class T&gt;
        auto get_return_object(T&amp;&amp;) {
            return coroutine_test{};
        }
        
        std::suspend_never initial_suspend() { return {}; }
        std::suspend_never final_suspend() noexcept { return {}; }
        void unhandled_exception() {}
        void return_value(int) {}
    };

    static coroutine_test foo() {
       co_return 42; 
   }
};

int main() {
   auto cobj_foo = coroutine_test&lt;&gt;::foo();
}
```
Go back to `clang 14.1.0 (assertions)`. And this issue is specifically reproducible when using -std=c++20 or later.
Clang crashes with an assertion failure when compiling a C++20 coroutine whose `promise_type::get_return_object` is a function template.
The crash occurs during coroutine semantic analysis in `SemaCoroutine.cpp`, hitting the assertion.
## Stack dump:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/Sema/SemaCoroutine.cpp:1826: bool clang::CoroutineStmtBuilder::makeGroDeclAndReturnStmt(): Assertion `!GroType-&gt;isDependentType() &amp;&amp; "get_return_object type must no longer be dependent"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	&lt;source&gt;:19:27: instantiating function definition 'coroutine_test&lt;&gt;::foo'
 #<!-- -->0 0x00000000042ecab8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42ecab8)
 #<!-- -->1 0x00000000042e9ee4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42e9ee4)
 #<!-- -->2 0x000000000422ec88 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x0000778975c42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x0000778975c969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x0000778975c42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x0000778975c287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x0000778975c2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x0000778975c39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000006d8365b clang::CoroutineStmtBuilder::makeGroDeclAndReturnStmt() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d8365b)
#<!-- -->10 0x0000000006d8afa4 clang::CoroutineStmtBuilder::buildDependentStatements() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d8afa4)
#<!-- -->11 0x00000000075a18e9 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCoroutineBodyStmt(clang::CoroutineBodyStmt*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->12 0x00000000075a2d0d clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x75a2d0d)
#<!-- -->13 0x000000000763f070 clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x763f070)
#<!-- -->14 0x000000000763cc36 clang::Sema::PerformPendingInstantiations(bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x763cc36)
#<!-- -->15 0x0000000006bbd072 clang::Sema::ActOnEndOfTranslationUnitFragment(clang::TUFragmentKind) (.part.0) Sema.cpp:0:0
#<!-- -->16 0x0000000006bbd3f2 clang::Sema::ActOnEndOfTranslationUnit() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bbd3f2)
#<!-- -->17 0x0000000006a27e15 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a27e15)
#<!-- -->18 0x0000000006a08dca clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a08dca)
#<!-- -->19 0x0000000004c4dde8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c4dde8)
#<!-- -->20 0x0000000004f446e5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f446e5)
#<!-- -->21 0x0000000004ec497e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ec497e)
#<!-- -->22 0x000000000503ea6d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x503ea6d)
#<!-- -->23 0x0000000000de5b4c cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde5b4c)
#<!-- -->24 0x0000000000ddc59a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->25 0x0000000000ddc71d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->26 0x0000000004cc0739 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->27 0x000000000422f124 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x422f124)
#<!-- -->28 0x0000000004cc0d4f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->29 0x0000000004c81832 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c81832)
#<!-- -->30 0x0000000004c827de clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c827de)
#<!-- -->31 0x0000000004c89c35 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c89c35)
#<!-- -->32 0x0000000000de1f41 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde1f41)
#<!-- -->33 0x0000000000c8cf94 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc8cf94)
#<!-- -->34 0x0000778975c29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->35 0x0000778975c29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->36 0x0000000000ddc035 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xddc035)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

