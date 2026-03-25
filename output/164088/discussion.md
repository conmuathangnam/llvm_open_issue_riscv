# clang 21 crashes in DiagnoseTypeAwareAllocators with coroutines

**Author:** kelbon
**URL:** https://github.com/llvm/llvm-project/issues/164088
**Status:** Closed
**Labels:** clang:frontend, crash
**Closed Date:** 2026-02-24T17:25:54Z

## Body

https://godbolt.org/z/K9x6P7xGf

```cpp
using wnr = dd::with_resource<dd::chunk_from<dd::new_delete_resource>>;

dd::generator<int> gen(int x, wnr) {
  co_yield x;
}


void foo() {
    for (int x : gen(5, wnr{})) {
        ;
    }
}
```


<details> <summary> crash output </summary>
```cpp

PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-21.1.0/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-15.2.0 -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 -O3 -DNDEBUG <source>
1.	<source>:1639:1: current parser token 'void'
2.	<source>:1634:36: parsing function body 'gen'
 #0 0x0000000003bfa6c8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x3bfa6c8)
 #1 0x0000000003bf809c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x3bf809c)
 #2 0x0000000003b483f8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x0000771af2a42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00000000072395d0 clang::Decl::getASTContext() const (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x72395d0)
 #5 0x000000000721bb09 clang::FunctionDecl::isTypeAwareOperatorNewOrDelete() const (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x721bb09)
 #6 0x00000000064ee9dd DiagnoseTypeAwareAllocators(clang::Sema&, clang::SourceLocation, unsigned int, clang::DeclarationName, clang::QualType) SemaCoroutine.cpp:0:0
 #7 0x00000000064f2d81 clang::CoroutineStmtBuilder::makeNewAndDeleteExpr() (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x64f2d81)
 #8 0x00000000064faa17 clang::Sema::CheckCompletedCoroutineBody(clang::FunctionDecl*, clang::Stmt*&) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x64faa17)
 #9 0x0000000006557ac2 clang::Sema::ActOnFinishFunctionBody(clang::Decl*, clang::Stmt*, bool) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x6557ac2)
#10 0x00000000062c4e2f clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x62c4e2f)
#11 0x000000000619c808 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x619c808)
#12 0x00000000061e8cdf clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x61e8cdf)
#13 0x00000000061955ff clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x61955ff)
#14 0x00000000061963de clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x61963de)
#15 0x000000000619ef7f clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x619ef7f)
#16 0x00000000061a07b0 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x61a07b0)
#17 0x000000000618ef7a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x618ef7a)
#18 0x000000000454985d clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x454985d)
#19 0x000000000485b83a clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x485b83a)
#20 0x00000000047d655b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x47d655b)
#21 0x000000000494e513 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x494e513)
#22 0x0000000000da5f25 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0xda5f25)
#23 0x0000000000d9de2d ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#24 0x00000000045bb869 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#25 0x0000000003b48813 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x3b48813)
#26 0x00000000045bba89 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#27 0x000000000457eddd clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x457eddd)
#28 0x000000000457fd91 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x457fd91)
#29 0x000000000458d29c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x458d29c)
#30 0x0000000000da2751 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0xda2751)
#31 0x0000000000c526f4 main (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0xc526f4)
#32 0x0000771af2a29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#33 0x0000771af2a29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#34 0x0000000000d9d8c5 _start (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0xd9d8c5)

```
</details>

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (kelbon)

<details>
https://godbolt.org/z/K9x6P7xGf

```cpp
using wnr = dd::with_resource&lt;dd::chunk_from&lt;dd::new_delete_resource&gt;&gt;;

dd::generator&lt;int&gt; gen(int x, wnr) {
  co_yield x;
}


void foo() {
    for (int x : gen(5, wnr{})) {
        ;
    }
}
```


&lt;details&gt; &lt;summary&gt; crash output &lt;/summary&gt;
```cpp

PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-21.1.0/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-15.2.0 -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 -O3 -DNDEBUG &lt;source&gt;
1.	&lt;source&gt;:1639:1: current parser token 'void'
2.	&lt;source&gt;:1634:36: parsing function body 'gen'
 #<!-- -->0 0x0000000003bfa6c8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x3bfa6c8)
 #<!-- -->1 0x0000000003bf809c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x3bf809c)
 #<!-- -->2 0x0000000003b483f8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x0000771af2a42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00000000072395d0 clang::Decl::getASTContext() const (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x72395d0)
 #<!-- -->5 0x000000000721bb09 clang::FunctionDecl::isTypeAwareOperatorNewOrDelete() const (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x721bb09)
 #<!-- -->6 0x00000000064ee9dd DiagnoseTypeAwareAllocators(clang::Sema&amp;, clang::SourceLocation, unsigned int, clang::DeclarationName, clang::QualType) SemaCoroutine.cpp:0:0
 #<!-- -->7 0x00000000064f2d81 clang::CoroutineStmtBuilder::makeNewAndDeleteExpr() (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x64f2d81)
 #<!-- -->8 0x00000000064faa17 clang::Sema::CheckCompletedCoroutineBody(clang::FunctionDecl*, clang::Stmt*&amp;) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x64faa17)
 #<!-- -->9 0x0000000006557ac2 clang::Sema::ActOnFinishFunctionBody(clang::Decl*, clang::Stmt*, bool) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x6557ac2)
#<!-- -->10 0x00000000062c4e2f clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x62c4e2f)
#<!-- -->11 0x000000000619c808 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x619c808)
#<!-- -->12 0x00000000061e8cdf clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x61e8cdf)
#<!-- -->13 0x00000000061955ff clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x61955ff)
#<!-- -->14 0x00000000061963de clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x61963de)
#<!-- -->15 0x000000000619ef7f clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x619ef7f)
#<!-- -->16 0x00000000061a07b0 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x61a07b0)
#<!-- -->17 0x000000000618ef7a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x618ef7a)
#<!-- -->18 0x000000000454985d clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x454985d)
#<!-- -->19 0x000000000485b83a clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x485b83a)
#<!-- -->20 0x00000000047d655b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x47d655b)
#<!-- -->21 0x000000000494e513 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x494e513)
#<!-- -->22 0x0000000000da5f25 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0xda5f25)
#<!-- -->23 0x0000000000d9de2d ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->24 0x00000000045bb869 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->25 0x0000000003b48813 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x3b48813)
#<!-- -->26 0x00000000045bba89 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->27 0x000000000457eddd clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x457eddd)
#<!-- -->28 0x000000000457fd91 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x457fd91)
#<!-- -->29 0x000000000458d29c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x458d29c)
#<!-- -->30 0x0000000000da2751 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0xda2751)
#<!-- -->31 0x0000000000c526f4 main (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0xc526f4)
#<!-- -->32 0x0000771af2a29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->33 0x0000771af2a29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->34 0x0000000000d9d8c5 _start (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0xd9d8c5)

```
&lt;/details&gt;
</details>


---

