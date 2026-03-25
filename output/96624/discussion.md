# [clang][trunk] UNREACHABLE executed: Non-canonical and dependent types shouldn't get here

**Author:** wierton
**URL:** https://github.com/llvm/llvm-project/issues/96624

## Body

When compiling this program using clang-trunk, clang reports `UNREACHABLE executed`.

Compiler explorer: https://gcc.godbolt.org/z/ToorqvhGh

The program:
```
int *x;
void f(char (*t)[(*x | 6)++]);
void f(char (*t)[((const int *)(*x))++]);
```

The stack dump:
```
Non-canonical and dependent types shouldn't get here
UNREACHABLE executed at /root/llvm-project/clang/lib/AST/ASTContext.cpp:10832!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang -gdwarf-4 -g -o /app/output.s -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<source>:4:41: current parser token ';'
 #0 0x0000000003a1b328 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3a1b328)
 #1 0x0000000003a1900c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3a1900c)
 #2 0x000000000395eb28 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x0000726857c42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x0000726857c969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x0000726857c42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x0000726857c287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x000000000396a80a (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x396a80a)
 #8 0x00000000074928f3 clang::ASTContext::mergeTypes(clang::QualType, clang::QualType, bool, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x74928f3)
 #9 0x00000000074921b0 clang::ASTContext::mergeTypes(clang::QualType, clang::QualType, bool, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x74921b0)
#10 0x00000000074931f1 clang::ASTContext::mergeFunctionParameterTypes(clang::QualType, clang::QualType, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x74931f1)
#11 0x0000000007493cbf clang::ASTContext::mergeFunctionTypes(clang::QualType, clang::QualType, bool, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x7493cbf)
#12 0x0000000007492433 clang::ASTContext::mergeTypes(clang::QualType, clang::QualType, bool, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x7492433)
#13 0x0000000007494dd4 clang::ASTContext::typesAreCompatible(clang::QualType, clang::QualType, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x7494dd4)
#14 0x00000000067448a6 clang::Sema::MergeFunctionDecl(clang::FunctionDecl*, clang::NamedDecl*&, clang::Scope*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x67448a6)
#15 0x000000000674bee0 clang::Sema::CheckFunctionDeclaration(clang::Scope*, clang::FunctionDecl*, clang::LookupResult&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x674bee0)
#16 0x00000000067521a7 clang::Sema::ActOnFunctionDeclarator(clang::Scope*, clang::Declarator&, clang::DeclContext*, clang::TypeSourceInfo*, clang::LookupResult&, llvm::MutableArrayRef<clang::TemplateParameterList*>, bool&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x67521a7)
#17 0x0000000006757180 clang::Sema::HandleDeclarator(clang::Scope*, clang::Declarator&, llvm::MutableArrayRef<clang::TemplateParameterList*>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6757180)
#18 0x0000000006757cc0 clang::Sema::ActOnDeclarator(clang::Scope*, clang::Declarator&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6757cc0)
#19 0x00000000063c796f clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x63c796f)
#20 0x00000000063d696a clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x63d696a)
#21 0x000000000639519e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x639519e)
#22 0x00000000063959fe clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x63959fe)
#23 0x000000000639c107 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x639c107)
#24 0x000000000639d00f clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x639d00f)
#25 0x000000000639080a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x639080a)
#26 0x0000000004307c98 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4307c98)
#27 0x000000000458ea89 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x458ea89)
#28 0x0000000004514c7e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4514c7e)
#29 0x000000000467701e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x467701e)
#30 0x0000000000c6946c cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xc6946c)
#31 0x0000000000c627ca ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#32 0x000000000434bbe9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#33 0x000000000395efd4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x395efd4)
#34 0x000000000434c1df clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#35 0x0000000004311415 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4311415)
#36 0x0000000004311e7d clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4311e7d)
#37 0x0000000004319b75 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4319b75)
#38 0x0000000000c66975 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xc66975)
#39 0x0000000000b3fd24 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xb3fd24)
#40 0x0000726857c29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#41 0x0000726857c29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#42 0x0000000000c6227e _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xc6227e)
clang: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: wt.cc (wierton)

<details>
When compiling this program using clang-trunk, clang reports `UNREACHABLE executed`.

Compiler explorer: https://gcc.godbolt.org/z/ToorqvhGh

The program:
```
int *x;
void f(char (*t)[(*x | 6)++]);
void f(char (*t)[((const int *)(*x))++]);
```

The stack dump:
```
Non-canonical and dependent types shouldn't get here
UNREACHABLE executed at /root/llvm-project/clang/lib/AST/ASTContext.cpp:10832!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang -gdwarf-4 -g -o /app/output.s -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics &lt;source&gt;
1.	&lt;source&gt;:4:41: current parser token ';'
 #<!-- -->0 0x0000000003a1b328 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3a1b328)
 #<!-- -->1 0x0000000003a1900c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3a1900c)
 #<!-- -->2 0x000000000395eb28 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x0000726857c42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x0000726857c969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x0000726857c42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x0000726857c287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x000000000396a80a (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x396a80a)
 #<!-- -->8 0x00000000074928f3 clang::ASTContext::mergeTypes(clang::QualType, clang::QualType, bool, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x74928f3)
 #<!-- -->9 0x00000000074921b0 clang::ASTContext::mergeTypes(clang::QualType, clang::QualType, bool, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x74921b0)
#<!-- -->10 0x00000000074931f1 clang::ASTContext::mergeFunctionParameterTypes(clang::QualType, clang::QualType, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x74931f1)
#<!-- -->11 0x0000000007493cbf clang::ASTContext::mergeFunctionTypes(clang::QualType, clang::QualType, bool, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x7493cbf)
#<!-- -->12 0x0000000007492433 clang::ASTContext::mergeTypes(clang::QualType, clang::QualType, bool, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x7492433)
#<!-- -->13 0x0000000007494dd4 clang::ASTContext::typesAreCompatible(clang::QualType, clang::QualType, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x7494dd4)
#<!-- -->14 0x00000000067448a6 clang::Sema::MergeFunctionDecl(clang::FunctionDecl*, clang::NamedDecl*&amp;, clang::Scope*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x67448a6)
#<!-- -->15 0x000000000674bee0 clang::Sema::CheckFunctionDeclaration(clang::Scope*, clang::FunctionDecl*, clang::LookupResult&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x674bee0)
#<!-- -->16 0x00000000067521a7 clang::Sema::ActOnFunctionDeclarator(clang::Scope*, clang::Declarator&amp;, clang::DeclContext*, clang::TypeSourceInfo*, clang::LookupResult&amp;, llvm::MutableArrayRef&lt;clang::TemplateParameterList*&gt;, bool&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x67521a7)
#<!-- -->17 0x0000000006757180 clang::Sema::HandleDeclarator(clang::Scope*, clang::Declarator&amp;, llvm::MutableArrayRef&lt;clang::TemplateParameterList*&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6757180)
#<!-- -->18 0x0000000006757cc0 clang::Sema::ActOnDeclarator(clang::Scope*, clang::Declarator&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6757cc0)
#<!-- -->19 0x00000000063c796f clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x63c796f)
#<!-- -->20 0x00000000063d696a clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x63d696a)
#<!-- -->21 0x000000000639519e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x639519e)
#<!-- -->22 0x00000000063959fe clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x63959fe)
#<!-- -->23 0x000000000639c107 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x639c107)
#<!-- -->24 0x000000000639d00f clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x639d00f)
#<!-- -->25 0x000000000639080a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x639080a)
#<!-- -->26 0x0000000004307c98 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4307c98)
#<!-- -->27 0x000000000458ea89 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x458ea89)
#<!-- -->28 0x0000000004514c7e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4514c7e)
#<!-- -->29 0x000000000467701e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x467701e)
#<!-- -->30 0x0000000000c6946c cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xc6946c)
#<!-- -->31 0x0000000000c627ca ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->32 0x000000000434bbe9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->33 0x000000000395efd4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x395efd4)
#<!-- -->34 0x000000000434c1df clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->35 0x0000000004311415 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4311415)
#<!-- -->36 0x0000000004311e7d clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4311e7d)
#<!-- -->37 0x0000000004319b75 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4319b75)
#<!-- -->38 0x0000000000c66975 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xc66975)
#<!-- -->39 0x0000000000b3fd24 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xb3fd24)
#<!-- -->40 0x0000726857c29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->41 0x0000726857c29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->42 0x0000000000c6227e _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xc6227e)
clang: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

### Comment 2 - shafik

Possibly related: https://github.com/llvm/llvm-project/issues/69058

---

