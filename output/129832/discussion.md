# Assertion `!isa<NamedDecl>(static_cast<decl_type*>(this)) || cast<NamedDecl>(static_cast<decl_type*>(this))->isLinkageValid()' failed.

**Author:** bi6c
**URL:** https://github.com/llvm/llvm-project/issues/129832
**Status:** Closed
**Labels:** duplicate, clang:frontend, crash
**Closed Date:** 2025-03-05T06:10:52Z

## Body

Compiler Explorer: https://godbolt.org/z/Tb1YaaPs4

```console
<source>:7:12: warning: #pragma redefine_extname is applicable to external C declarations only; not applied to function 'foo' [-Wpragmas]
    7 | static int foo(void);
      |            ^
clang: /root/llvm-project/clang/include/clang/AST/Decl.h:5157: void clang::Redeclarable<decl_type>::setPreviousDecl(decl_type*) [with decl_type = clang::FunctionDecl]: Assertion `!isa<NamedDecl>(static_cast<decl_type*>(this)) || cast<NamedDecl>(static_cast<decl_type*>(this))->isLinkageValid()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<source>:8:21: current parser token ';'
 #0 0x0000000003e53898 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3e53898)
 #1 0x0000000003e51554 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3e51554)
 #2 0x0000000003d9de88 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007d7f66c42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007d7f66c969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007d7f66c42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007d7f66c287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007d7f66c2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007d7f66c39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x00000000073739ac clang::Redeclarable<clang::FunctionDecl>::setPreviousDecl(clang::FunctionDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x73739ac)
#10 0x00000000074ee3d5 clang::FunctionDecl::setPreviousDeclaration(clang::FunctionDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x74ee3d5)
#11 0x00000000067b7420 clang::Sema::CheckFunctionDeclaration(clang::Scope*, clang::FunctionDecl*, clang::LookupResult&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x67b7420)
#12 0x00000000067bc5f0 clang::Sema::ActOnFunctionDeclarator(clang::Scope*, clang::Declarator&, clang::DeclContext*, clang::TypeSourceInfo*, clang::LookupResult&, llvm::MutableArrayRef<clang::TemplateParameterList*>, bool&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x67bc5f0)
#13 0x00000000067c1530 clang::Sema::HandleDeclarator(clang::Scope*, clang::Declarator&, llvm::MutableArrayRef<clang::TemplateParameterList*>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x67c1530)
#14 0x00000000067c2070 clang::Sema::ActOnDeclarator(clang::Scope*, clang::Declarator&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x67c2070)
#15 0x000000000642fd1e clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x642fd1e)
#16 0x000000000643f8c9 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x643f8c9)
#17 0x00000000063ff73e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x63ff73e)
#18 0x00000000063ffef9 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x63ffef9)
#19 0x00000000064076d3 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x64076d3)
#20 0x00000000064085ad clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x64085ad)
#21 0x00000000063faa3a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x63faa3a)
#22 0x0000000004812598 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4812598)
#23 0x0000000004ada245 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4ada245)
#24 0x0000000004a5d92e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4a5d92e)
#25 0x0000000004bc809e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4bc809e)
#26 0x0000000000d4ab2f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xd4ab2f)
#27 0x0000000000d4244a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#28 0x000000000485a4c9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#29 0x0000000003d9e334 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3d9e334)
#30 0x000000000485aabf clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#31 0x000000000481d6dd clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x481d6dd)
#32 0x000000000481e75e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x481e75e)
#33 0x00000000048266f5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x48266f5)
#34 0x0000000000d47923 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xd47923)
#35 0x0000000000c11464 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xc11464)
#36 0x00007d7f66c29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#37 0x00007d7f66c29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#38 0x0000000000d41ef5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xd41ef5)
clang: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
Code:
```c
#ifndef __PRAGMA_REDEFINE_EXTNAME
#error
#endif

#pragma redefine_extname foo bar
static int foo(void);
extern int foo(void);
int (*p)(void) = foo;
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (bi6c)

<details>
Compiler Explorer: https://godbolt.org/z/Tb1YaaPs4

```console
&lt;source&gt;:7:12: warning: #pragma redefine_extname is applicable to external C declarations only; not applied to function 'foo' [-Wpragmas]
    7 | static int foo(void);
      |            ^
clang: /root/llvm-project/clang/include/clang/AST/Decl.h:5157: void clang::Redeclarable&lt;decl_type&gt;::setPreviousDecl(decl_type*) [with decl_type = clang::FunctionDecl]: Assertion `!isa&lt;NamedDecl&gt;(static_cast&lt;decl_type*&gt;(this)) || cast&lt;NamedDecl&gt;(static_cast&lt;decl_type*&gt;(this))-&gt;isLinkageValid()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics &lt;source&gt;
1.	&lt;source&gt;:8:21: current parser token ';'
 #<!-- -->0 0x0000000003e53898 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3e53898)
 #<!-- -->1 0x0000000003e51554 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3e51554)
 #<!-- -->2 0x0000000003d9de88 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007d7f66c42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007d7f66c969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007d7f66c42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007d7f66c287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x00007d7f66c2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x00007d7f66c39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x00000000073739ac clang::Redeclarable&lt;clang::FunctionDecl&gt;::setPreviousDecl(clang::FunctionDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x73739ac)
#<!-- -->10 0x00000000074ee3d5 clang::FunctionDecl::setPreviousDeclaration(clang::FunctionDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x74ee3d5)
#<!-- -->11 0x00000000067b7420 clang::Sema::CheckFunctionDeclaration(clang::Scope*, clang::FunctionDecl*, clang::LookupResult&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x67b7420)
#<!-- -->12 0x00000000067bc5f0 clang::Sema::ActOnFunctionDeclarator(clang::Scope*, clang::Declarator&amp;, clang::DeclContext*, clang::TypeSourceInfo*, clang::LookupResult&amp;, llvm::MutableArrayRef&lt;clang::TemplateParameterList*&gt;, bool&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x67bc5f0)
#<!-- -->13 0x00000000067c1530 clang::Sema::HandleDeclarator(clang::Scope*, clang::Declarator&amp;, llvm::MutableArrayRef&lt;clang::TemplateParameterList*&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x67c1530)
#<!-- -->14 0x00000000067c2070 clang::Sema::ActOnDeclarator(clang::Scope*, clang::Declarator&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x67c2070)
#<!-- -->15 0x000000000642fd1e clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x642fd1e)
#<!-- -->16 0x000000000643f8c9 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x643f8c9)
#<!-- -->17 0x00000000063ff73e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x63ff73e)
#<!-- -->18 0x00000000063ffef9 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x63ffef9)
#<!-- -->19 0x00000000064076d3 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x64076d3)
#<!-- -->20 0x00000000064085ad clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x64085ad)
#<!-- -->21 0x00000000063faa3a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x63faa3a)
#<!-- -->22 0x0000000004812598 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4812598)
#<!-- -->23 0x0000000004ada245 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4ada245)
#<!-- -->24 0x0000000004a5d92e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4a5d92e)
#<!-- -->25 0x0000000004bc809e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4bc809e)
#<!-- -->26 0x0000000000d4ab2f cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xd4ab2f)
#<!-- -->27 0x0000000000d4244a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->28 0x000000000485a4c9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->29 0x0000000003d9e334 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3d9e334)
#<!-- -->30 0x000000000485aabf clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->31 0x000000000481d6dd clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x481d6dd)
#<!-- -->32 0x000000000481e75e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x481e75e)
#<!-- -->33 0x00000000048266f5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x48266f5)
#<!-- -->34 0x0000000000d47923 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xd47923)
#<!-- -->35 0x0000000000c11464 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xc11464)
#<!-- -->36 0x00007d7f66c29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->37 0x00007d7f66c29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->38 0x0000000000d41ef5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xd41ef5)
clang: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
Code:
```c
#ifndef __PRAGMA_REDEFINE_EXTNAME
#error
#endif

#pragma redefine_extname foo bar
static int foo(void);
extern int foo(void);
int (*p)(void) = foo;
```
</details>


---

