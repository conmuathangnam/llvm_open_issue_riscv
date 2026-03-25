# Default operator<=> with an __attribute__((vector_size)) field crashes frontend

**Author:** purplesyringa
**URL:** https://github.com/llvm/llvm-project/issues/137452
**Status:** Closed
**Labels:** clang:frontend, confirmed, crash
**Closed Date:** 2025-05-14T10:58:16Z

## Body

https://godbolt.org/z/1j3v6MW6v

```cpp
struct comparable_t {
    __attribute__((vector_size(32))) double numbers;
    auto operator<=>(const comparable_t& rhs) const = default;
};
```

```
clang++: /root/llvm-project/llvm/tools/clang/lib/Sema/SemaDeclCXX.cpp:8385: {anonymous}::DefaultedComparisonAnalyzer::Result {anonymous}::DefaultedComparisonAnalyzer::visitBinaryOperator(clang::OverloadedOperatorKind, llvm::ArrayRef<clang::Expr*>, {anonymous}::DefaultedComparisonAnalyzer::Subobject, clang::OverloadCandidateSet*): Assertion `Cat && "no category for builtin comparison?"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 <source>
1.	<source>:4:2: current parser token ';'
2.	<source>:1:1: parsing struct/union/class body 'comparable_t'
 #0 0x0000000003ef8fe8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3ef8fe8)
 #1 0x0000000003ef6c74 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3ef6c74)
 #2 0x0000000003e3b7a8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000076c993e42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x000076c993e969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x000076c993e42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x000076c993e287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x000076c993e2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x000076c993e39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x00000000069ac305 (anonymous namespace)::DefaultedComparisonAnalyzer::visitBinaryOperator(clang::OverloadedOperatorKind, llvm::ArrayRef<clang::Expr*>, (anonymous namespace)::DefaultedComparisonSubobject, clang::OverloadCandidateSet*) SemaDeclCXX.cpp:0:0
#10 0x00000000069acbda (anonymous namespace)::DefaultedComparisonVisitor<(anonymous namespace)::DefaultedComparisonAnalyzer, (anonymous namespace)::DefaultedComparisonInfo, (anonymous namespace)::DefaultedComparisonInfo, (anonymous namespace)::DefaultedComparisonSubobject>::visitSubobject(clang::QualType, (anonymous namespace)::DefaultedComparisonSubobject) SemaDeclCXX.cpp:0:0
#11 0x00000000069bf3cc (anonymous namespace)::DefaultedComparisonVisitor<(anonymous namespace)::DefaultedComparisonAnalyzer, (anonymous namespace)::DefaultedComparisonInfo, (anonymous namespace)::DefaultedComparisonInfo, (anonymous namespace)::DefaultedComparisonSubobject>::visitSubobjects((anonymous namespace)::DefaultedComparisonInfo&, clang::CXXRecordDecl*, clang::Qualifiers) SemaDeclCXX.cpp:0:0
#12 0x00000000069bf7a6 (anonymous namespace)::DefaultedComparisonAnalyzer::visit() SemaDeclCXX.cpp:0:0
#13 0x00000000069c65be clang::Sema::CheckExplicitlyDefaultedComparison(clang::Scope*, clang::FunctionDecl*, clang::Sema::DefaultedComparisonKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69c65be)
#14 0x00000000069df838 clang::Sema::CheckExplicitlyDefaultedFunction(clang::Scope*, clang::FunctionDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69df838)
#15 0x00000000069e4498 clang::Sema::CheckCompletedCXXClass(clang::Scope*, clang::CXXRecordDecl*)::'lambda1'(clang::CXXMethodDecl*)::operator()(clang::CXXMethodDecl*) const SemaDeclCXX.cpp:0:0
#16 0x00000000069e4bed clang::Sema::CheckCompletedCXXClass(clang::Scope*, clang::CXXRecordDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69e4bed)
#17 0x00000000069e6bf9 clang::Sema::ActOnFinishCXXMemberSpecification(clang::Scope*, clang::SourceLocation, clang::Decl*, clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributesView const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69e6bf9)
#18 0x00000000065cbcdb clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&, unsigned int, clang::Decl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65cbcdb)
#19 0x00000000065ce373 clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65ce373)
#20 0x00000000065a2708 clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65a2708)
#21 0x000000000655e2a4 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x655e2a4)
#22 0x000000000655ed09 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x655ed09)
#23 0x000000000656662a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x656662a)
#24 0x00000000065675cd clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65675cd)
#25 0x0000000006567a80 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6567a80)
#26 0x0000000006559973 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6559973)
#27 0x0000000004898328 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4898328)
#28 0x0000000004b8b815 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b8b815)
#29 0x0000000004b0a53e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b0a53e)
#30 0x0000000004c7ae4e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c7ae4e)
#31 0x0000000000da069f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda069f)
#32 0x0000000000d976aa ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#33 0x00000000048feba9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#34 0x0000000003e3bc44 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3e3bc44)
#35 0x00000000048ff1bf clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#36 0x00000000048c161d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48c161d)
#37 0x00000000048c26ae clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48c26ae)
#38 0x00000000048ca505 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48ca505)
#39 0x0000000000d9d3e8 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd9d3e8)
#40 0x0000000000c248e4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc248e4)
#41 0x000076c993e29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#42 0x000076c993e29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#43 0x0000000000d97155 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd97155)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Alisa Sireneva (purplesyringa)

<details>
https://godbolt.org/z/1j3v6MW6v

```cpp
struct comparable_t {
    __attribute__((vector_size(32))) double numbers;
    auto operator&lt;=&gt;(const comparable_t&amp; rhs) const = default;
};
```

```
clang++: /root/llvm-project/llvm/tools/clang/lib/Sema/SemaDeclCXX.cpp:8385: {anonymous}::DefaultedComparisonAnalyzer::Result {anonymous}::DefaultedComparisonAnalyzer::visitBinaryOperator(clang::OverloadedOperatorKind, llvm::ArrayRef&lt;clang::Expr*&gt;, {anonymous}::DefaultedComparisonAnalyzer::Subobject, clang::OverloadCandidateSet*): Assertion `Cat &amp;&amp; "no category for builtin comparison?"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 &lt;source&gt;
1.	&lt;source&gt;:4:2: current parser token ';'
2.	&lt;source&gt;:1:1: parsing struct/union/class body 'comparable_t'
 #<!-- -->0 0x0000000003ef8fe8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3ef8fe8)
 #<!-- -->1 0x0000000003ef6c74 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3ef6c74)
 #<!-- -->2 0x0000000003e3b7a8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x000076c993e42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x000076c993e969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x000076c993e42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x000076c993e287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x000076c993e2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x000076c993e39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x00000000069ac305 (anonymous namespace)::DefaultedComparisonAnalyzer::visitBinaryOperator(clang::OverloadedOperatorKind, llvm::ArrayRef&lt;clang::Expr*&gt;, (anonymous namespace)::DefaultedComparisonSubobject, clang::OverloadCandidateSet*) SemaDeclCXX.cpp:0:0
#<!-- -->10 0x00000000069acbda (anonymous namespace)::DefaultedComparisonVisitor&lt;(anonymous namespace)::DefaultedComparisonAnalyzer, (anonymous namespace)::DefaultedComparisonInfo, (anonymous namespace)::DefaultedComparisonInfo, (anonymous namespace)::DefaultedComparisonSubobject&gt;::visitSubobject(clang::QualType, (anonymous namespace)::DefaultedComparisonSubobject) SemaDeclCXX.cpp:0:0
#<!-- -->11 0x00000000069bf3cc (anonymous namespace)::DefaultedComparisonVisitor&lt;(anonymous namespace)::DefaultedComparisonAnalyzer, (anonymous namespace)::DefaultedComparisonInfo, (anonymous namespace)::DefaultedComparisonInfo, (anonymous namespace)::DefaultedComparisonSubobject&gt;::visitSubobjects((anonymous namespace)::DefaultedComparisonInfo&amp;, clang::CXXRecordDecl*, clang::Qualifiers) SemaDeclCXX.cpp:0:0
#<!-- -->12 0x00000000069bf7a6 (anonymous namespace)::DefaultedComparisonAnalyzer::visit() SemaDeclCXX.cpp:0:0
#<!-- -->13 0x00000000069c65be clang::Sema::CheckExplicitlyDefaultedComparison(clang::Scope*, clang::FunctionDecl*, clang::Sema::DefaultedComparisonKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69c65be)
#<!-- -->14 0x00000000069df838 clang::Sema::CheckExplicitlyDefaultedFunction(clang::Scope*, clang::FunctionDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69df838)
#<!-- -->15 0x00000000069e4498 clang::Sema::CheckCompletedCXXClass(clang::Scope*, clang::CXXRecordDecl*)::'lambda1'(clang::CXXMethodDecl*)::operator()(clang::CXXMethodDecl*) const SemaDeclCXX.cpp:0:0
#<!-- -->16 0x00000000069e4bed clang::Sema::CheckCompletedCXXClass(clang::Scope*, clang::CXXRecordDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69e4bed)
#<!-- -->17 0x00000000069e6bf9 clang::Sema::ActOnFinishCXXMemberSpecification(clang::Scope*, clang::SourceLocation, clang::Decl*, clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributesView const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69e6bf9)
#<!-- -->18 0x00000000065cbcdb clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&amp;, unsigned int, clang::Decl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65cbcdb)
#<!-- -->19 0x00000000065ce373 clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65ce373)
#<!-- -->20 0x00000000065a2708 clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65a2708)
#<!-- -->21 0x000000000655e2a4 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x655e2a4)
#<!-- -->22 0x000000000655ed09 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x655ed09)
#<!-- -->23 0x000000000656662a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x656662a)
#<!-- -->24 0x00000000065675cd clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65675cd)
#<!-- -->25 0x0000000006567a80 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6567a80)
#<!-- -->26 0x0000000006559973 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6559973)
#<!-- -->27 0x0000000004898328 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4898328)
#<!-- -->28 0x0000000004b8b815 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b8b815)
#<!-- -->29 0x0000000004b0a53e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b0a53e)
#<!-- -->30 0x0000000004c7ae4e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c7ae4e)
#<!-- -->31 0x0000000000da069f cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda069f)
#<!-- -->32 0x0000000000d976aa ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->33 0x00000000048feba9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->34 0x0000000003e3bc44 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3e3bc44)
#<!-- -->35 0x00000000048ff1bf clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->36 0x00000000048c161d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48c161d)
#<!-- -->37 0x00000000048c26ae clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48c26ae)
#<!-- -->38 0x00000000048ca505 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48ca505)
#<!-- -->39 0x0000000000d9d3e8 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd9d3e8)
#<!-- -->40 0x0000000000c248e4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc248e4)
#<!-- -->41 0x000076c993e29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->42 0x000076c993e29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->43 0x0000000000d97155 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd97155)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

### Comment 2 - shafik

This goes all the way back to clang-10.

---

### Comment 3 - pinskia

Note GCC prints out a sorry message when used:
```
<source>:7:42: error: use of deleted function 'constexpr auto comparable_t::operator<=>(const comparable_t&) const'
    7 | bool f = comparable_t{} <=> comparable_t{};
      |                                          ^
<source>:4:10: note: 'constexpr auto comparable_t::operator<=>(const comparable_t&) const' is implicitly deleted because the default definition would be ill-formed:
    4 |     auto operator<=>(const comparable_t& rhs) const = default;
      |          ^~~~~~~~
<source>:3:45: sorry, unimplemented: three-way comparison of vectors
    3 |     __attribute__((vector_size(32))) double numbers;
      |                                             ^~~~~~~
```

---

### Comment 4 - purplesyringa

Clang also prints a sorry message when trying to invoke `<=>` on a vector:

```cpp
__attribute__((vector_size(32))) double numbers;
int main() {
    numbers <=> numbers;
}
```

```
<source>:3:13: error: three-way comparison between vectors is not supported
    3 |     numbers <=> numbers;
      |             ^
1 error generated.
Compiler returned: 1
```

So that's specifically an issue with the default implementation.

---

