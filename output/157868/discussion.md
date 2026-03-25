# [clang][OpenMP] crash on omp allocate

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/157868

## Body

Reproducer:
https://godbolt.org/z/jW1x9Yv4E
```cpp
//#include<omp.h>
typedef enum omp_allocator_handle_t
{
  omp_default_mem_alloc = 1,
  __omp_allocator_handle_t_max__ = __UINTPTR_MAX__
} omp_allocator_handle_t;

void foo()
{
  omp_allocator_handle_t my_handle;
  int A[2];
  #pragma omp allocate(A) allocator(my_handle)
  #pragma omp allocate(A) allocator(my_handle)
}
```

Backtrace:
```console
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -fopenmp <source>
1.	<source>:13:47: at annotation token
2.	<source>:9:1: parsing function body 'foo'
3.	<source>:9:1: in compound statement ('{}')
 #0 0x0000000003c40e18 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3c40e18)
 #1 0x0000000003c3e7ec llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3c3e7ec)
 #2 0x0000000003b8e6d8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000078884dc42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00000000072fd974 clang::Expr::IgnoreImpCasts() (/opt/compiler-explorer/clang-trunk/bin/clang+0x72fd974)
 #5 0x000000000690f6c9 getAllocatorKind(clang::Sema&, (anonymous namespace)::DSAStackTy*, clang::Expr*) (.part.0) SemaOpenMP.cpp:0:0
 #6 0x0000000006940a08 clang::SemaOpenMP::ActOnOpenMPAllocateDirective(clang::SourceLocation, llvm::ArrayRef<clang::Expr*>, llvm::ArrayRef<clang::OMPClause*>, clang::DeclContext*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x6940a08)
 #7 0x0000000006267ca6 clang::Parser::ParseOpenMPDeclarativeOrExecutableDirective(clang::Parser::ParsedStmtContext, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+0x6267ca6)
 #8 0x000000000628a945 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::LabelDecl*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x628a945)
 #9 0x000000000628c2d9 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x628c2d9)
#10 0x0000000006295d90 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-trunk/bin/clang+0x6295d90)
#11 0x00000000062967ca clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-trunk/bin/clang+0x62967ca)
#12 0x00000000061801c3 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x61801c3)
#13 0x00000000061ccb9f clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x61ccb9f)
#14 0x0000000006178caf clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+0x6178caf)
#15 0x0000000006179aa7 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+0x6179aa7)
#16 0x00000000061826ff clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x61826ff)
#17 0x0000000006183f07 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-trunk/bin/clang+0x6183f07)
#18 0x000000000615c9ba clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+0x615c9ba)
#19 0x000000000450920d clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+0x450920d)
#20 0x000000000480f91a clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+0x480f91a)
#21 0x0000000004789e2b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-trunk/bin/clang+0x4789e2b)
#22 0x0000000004903fb3 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x4903fb3)
#23 0x0000000000dc2f15 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+0xdc2f15)
#24 0x0000000000dbaebd ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#25 0x00000000045764a9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#26 0x0000000003b8eaf3 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3b8eaf3)
#27 0x00000000045766c9 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#28 0x000000000453982d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+0x453982d)
#29 0x000000000453a7e1 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+0x453a7e1)
#30 0x000000000454413c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-trunk/bin/clang+0x454413c)
#31 0x0000000000dbf721 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-trunk/bin/clang+0xdbf721)
#32 0x0000000000c6df24 main (/opt/compiler-explorer/clang-trunk/bin/clang+0xc6df24)
#33 0x000078884dc29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#34 0x000078884dc29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#35 0x0000000000dba955 _start (/opt/compiler-explorer/clang-trunk/bin/clang+0xdba955)
clang: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/jW1x9Yv4E
```cpp
//#include&lt;omp.h&gt;
typedef enum omp_allocator_handle_t
{
  omp_default_mem_alloc = 1,
  __omp_allocator_handle_t_max__ = __UINTPTR_MAX__
} omp_allocator_handle_t;

void foo()
{
  omp_allocator_handle_t my_handle;
  int A[2];
  #pragma omp allocate(A) allocator(my_handle)
  #pragma omp allocate(A) allocator(my_handle)
}
```

Backtrace:
```console
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -fopenmp &lt;source&gt;
1.	&lt;source&gt;:13:47: at annotation token
2.	&lt;source&gt;:9:1: parsing function body 'foo'
3.	&lt;source&gt;:9:1: in compound statement ('{}')
 #<!-- -->0 0x0000000003c40e18 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3c40e18)
 #<!-- -->1 0x0000000003c3e7ec llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3c3e7ec)
 #<!-- -->2 0x0000000003b8e6d8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x000078884dc42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00000000072fd974 clang::Expr::IgnoreImpCasts() (/opt/compiler-explorer/clang-trunk/bin/clang+0x72fd974)
 #<!-- -->5 0x000000000690f6c9 getAllocatorKind(clang::Sema&amp;, (anonymous namespace)::DSAStackTy*, clang::Expr*) (.part.0) SemaOpenMP.cpp:0:0
 #<!-- -->6 0x0000000006940a08 clang::SemaOpenMP::ActOnOpenMPAllocateDirective(clang::SourceLocation, llvm::ArrayRef&lt;clang::Expr*&gt;, llvm::ArrayRef&lt;clang::OMPClause*&gt;, clang::DeclContext*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x6940a08)
 #<!-- -->7 0x0000000006267ca6 clang::Parser::ParseOpenMPDeclarativeOrExecutableDirective(clang::Parser::ParsedStmtContext, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+0x6267ca6)
 #<!-- -->8 0x000000000628a945 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::LabelDecl*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x628a945)
 #<!-- -->9 0x000000000628c2d9 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x628c2d9)
#<!-- -->10 0x0000000006295d90 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-trunk/bin/clang+0x6295d90)
#<!-- -->11 0x00000000062967ca clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x62967ca)
#<!-- -->12 0x00000000061801c3 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x61801c3)
#<!-- -->13 0x00000000061ccb9f clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x61ccb9f)
#<!-- -->14 0x0000000006178caf clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+0x6178caf)
#<!-- -->15 0x0000000006179aa7 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+0x6179aa7)
#<!-- -->16 0x00000000061826ff clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x61826ff)
#<!-- -->17 0x0000000006183f07 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x6183f07)
#<!-- -->18 0x000000000615c9ba clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+0x615c9ba)
#<!-- -->19 0x000000000450920d clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+0x450920d)
#<!-- -->20 0x000000000480f91a clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+0x480f91a)
#<!-- -->21 0x0000000004789e2b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x4789e2b)
#<!-- -->22 0x0000000004903fb3 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x4903fb3)
#<!-- -->23 0x0000000000dc2f15 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+0xdc2f15)
#<!-- -->24 0x0000000000dbaebd ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->25 0x00000000045764a9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->26 0x0000000003b8eaf3 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3b8eaf3)
#<!-- -->27 0x00000000045766c9 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->28 0x000000000453982d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+0x453982d)
#<!-- -->29 0x000000000453a7e1 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+0x453a7e1)
#<!-- -->30 0x000000000454413c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x454413c)
#<!-- -->31 0x0000000000dbf721 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0xdbf721)
#<!-- -->32 0x0000000000c6df24 main (/opt/compiler-explorer/clang-trunk/bin/clang+0xc6df24)
#<!-- -->33 0x000078884dc29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->34 0x000078884dc29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->35 0x0000000000dba955 _start (/opt/compiler-explorer/clang-trunk/bin/clang+0xdba955)
clang: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```
</details>


---

### Comment 2 - k-arrows

Goes back to clang-16.0.0:
https://godbolt.org/z/148drb79o

---

### Comment 3 - KrxGu

@k-arrows @shafik #158146 should fix this issue, open to make any require changes to the PR

---

