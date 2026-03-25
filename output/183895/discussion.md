# [LifetimeSafety] Assertion `Src && "Dst is non-null but Src is null. List must have the same length"' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/183895
**Status:** Open
**Labels:** crash, clang:temporal-safety

## Body

Reproducer:
https://godbolt.org/z/TWzrqW1rh
```cpp
int val = 42;

int foo() {
  return val ? throw val : val;
}
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/Analysis/LifetimeSafety/FactsGenerator.cpp:54: void clang::lifetimes::internal::FactsGenerator::flow(clang::lifetimes::internal::OriginList*, clang::lifetimes::internal::OriginList*, bool): Assertion `Src && "Dst is non-null but Src is null. List must have the same length"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -Werror=lifetime-safety <source>
1.	<eof> parser at end of file
2.	<source>:3:11: parsing function body 'foo'
 #0 0x000000000440ac38 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x440ac38)
 #1 0x0000000004408084 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4408084)
 #2 0x0000000004347e88 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x0000787235c42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x0000787235c969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x0000787235c42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x0000787235c287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x0000787235c2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x0000787235c39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x00000000079cbedd clang::lifetimes::internal::FactsGenerator::flow(clang::lifetimes::internal::OriginList*, clang::lifetimes::internal::OriginList*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x79cbedd)
#10 0x00000000079cc129 clang::lifetimes::internal::FactsGenerator::VisitConditionalOperator(clang::ConditionalOperator const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x79cc129)
#11 0x00000000079d04bf clang::lifetimes::internal::FactsGenerator::run() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x79d04bf)
#12 0x000000000797a496 clang::lifetimes::internal::LifetimeSafetyAnalysis::run() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x797a496)
#13 0x000000000797b405 clang::lifetimes::runLifetimeSafetyAnalysis(clang::AnalysisDeclContext&, clang::lifetimes::LifetimeSafetySemaHelper*, clang::lifetimes::LifetimeSafetyStats&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x797b405)
#14 0x00000000078b4a4b clang::sema::AnalysisBasedWarnings::IssueWarnings(clang::sema::AnalysisBasedWarnings::Policy, clang::sema::FunctionScopeInfo*, clang::Decl const*, clang::QualType) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x78b4a4b)
#15 0x0000000006d71692 clang::Sema::PopFunctionScopeInfo(clang::sema::AnalysisBasedWarnings::Policy const*, clang::Decl*, clang::QualType) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d71692)
#16 0x0000000006fd2384 clang::Sema::ActOnFinishFunctionBody(clang::Decl*, clang::Stmt*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6fd2384)
#17 0x0000000006cebb94 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6cebb94)
#18 0x0000000006bf11c3 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bf11c3)
#19 0x0000000006c2785d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c2785d)
#20 0x0000000006be4bbb clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6be4bbb)
#21 0x0000000006be53bf clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6be53bf)
#22 0x0000000006bec9c1 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bec9c1)
#23 0x0000000006bed8f5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bed8f5)
#24 0x0000000006bcefda clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bcefda)
#25 0x0000000004ee4ef8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ee4ef8)
#26 0x00000000051eb615 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x51eb615)
#27 0x00000000051698fe clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x51698fe)
#28 0x00000000052e72dd clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x52e72dd)
#29 0x0000000000df063e cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdf063e)
#30 0x0000000000de6faa ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#31 0x0000000000de712d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#32 0x0000000004f59659 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#33 0x00000000043482c4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x43482c4)
#34 0x0000000004f59c6f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#35 0x0000000004f19952 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f19952)
#36 0x0000000004f1a8fe clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f1a8fe)
#37 0x0000000004f22645 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f22645)
#38 0x0000000000dec9ae clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdec9ae)
#39 0x0000000000ca4d0a main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xca4d0a)
#40 0x0000787235c29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#41 0x0000787235c29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#42 0x0000000000de6a45 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde6a45)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-temporal-safety

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/TWzrqW1rh
```cpp
int val = 42;

int foo() {
  return val ? throw val : val;
}
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/Analysis/LifetimeSafety/FactsGenerator.cpp:54: void clang::lifetimes::internal::FactsGenerator::flow(clang::lifetimes::internal::OriginList*, clang::lifetimes::internal::OriginList*, bool): Assertion `Src &amp;&amp; "Dst is non-null but Src is null. List must have the same length"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -Werror=lifetime-safety &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	&lt;source&gt;:3:11: parsing function body 'foo'
 #<!-- -->0 0x000000000440ac38 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x440ac38)
 #<!-- -->1 0x0000000004408084 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4408084)
 #<!-- -->2 0x0000000004347e88 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x0000787235c42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x0000787235c969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x0000787235c42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x0000787235c287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x0000787235c2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x0000787235c39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x00000000079cbedd clang::lifetimes::internal::FactsGenerator::flow(clang::lifetimes::internal::OriginList*, clang::lifetimes::internal::OriginList*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x79cbedd)
#<!-- -->10 0x00000000079cc129 clang::lifetimes::internal::FactsGenerator::VisitConditionalOperator(clang::ConditionalOperator const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x79cc129)
#<!-- -->11 0x00000000079d04bf clang::lifetimes::internal::FactsGenerator::run() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x79d04bf)
#<!-- -->12 0x000000000797a496 clang::lifetimes::internal::LifetimeSafetyAnalysis::run() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x797a496)
#<!-- -->13 0x000000000797b405 clang::lifetimes::runLifetimeSafetyAnalysis(clang::AnalysisDeclContext&amp;, clang::lifetimes::LifetimeSafetySemaHelper*, clang::lifetimes::LifetimeSafetyStats&amp;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x797b405)
#<!-- -->14 0x00000000078b4a4b clang::sema::AnalysisBasedWarnings::IssueWarnings(clang::sema::AnalysisBasedWarnings::Policy, clang::sema::FunctionScopeInfo*, clang::Decl const*, clang::QualType) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x78b4a4b)
#<!-- -->15 0x0000000006d71692 clang::Sema::PopFunctionScopeInfo(clang::sema::AnalysisBasedWarnings::Policy const*, clang::Decl*, clang::QualType) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d71692)
#<!-- -->16 0x0000000006fd2384 clang::Sema::ActOnFinishFunctionBody(clang::Decl*, clang::Stmt*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6fd2384)
#<!-- -->17 0x0000000006cebb94 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6cebb94)
#<!-- -->18 0x0000000006bf11c3 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bf11c3)
#<!-- -->19 0x0000000006c2785d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c2785d)
#<!-- -->20 0x0000000006be4bbb clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6be4bbb)
#<!-- -->21 0x0000000006be53bf clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6be53bf)
#<!-- -->22 0x0000000006bec9c1 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bec9c1)
#<!-- -->23 0x0000000006bed8f5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bed8f5)
#<!-- -->24 0x0000000006bcefda clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bcefda)
#<!-- -->25 0x0000000004ee4ef8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ee4ef8)
#<!-- -->26 0x00000000051eb615 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x51eb615)
#<!-- -->27 0x00000000051698fe clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x51698fe)
#<!-- -->28 0x00000000052e72dd clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x52e72dd)
#<!-- -->29 0x0000000000df063e cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdf063e)
#<!-- -->30 0x0000000000de6faa ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->31 0x0000000000de712d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->32 0x0000000004f59659 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->33 0x00000000043482c4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x43482c4)
#<!-- -->34 0x0000000004f59c6f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->35 0x0000000004f19952 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f19952)
#<!-- -->36 0x0000000004f1a8fe clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f1a8fe)
#<!-- -->37 0x0000000004f22645 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f22645)
#<!-- -->38 0x0000000000dec9ae clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdec9ae)
#<!-- -->39 0x0000000000ca4d0a main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xca4d0a)
#<!-- -->40 0x0000787235c29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->41 0x0000787235c29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->42 0x0000000000de6a45 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde6a45)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
Compiler returned: 134
```
</details>


---

