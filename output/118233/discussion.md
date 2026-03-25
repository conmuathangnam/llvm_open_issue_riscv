# Clang: __funcref induced ICE.

**Author:** moonheart08
**URL:** https://github.com/llvm/llvm-project/issues/118233
**Status:** Closed
**Labels:** clang:frontend, crash-on-invalid, confirmed
**Closed Date:** 2026-01-15T19:51:08Z

## Body

```c
int hsGetFuncRefForGlobal( __funcref function );
```

This is sufficient to cause the Clang frontend to crash for WebAssembly. Godbolt demonstration: https://godbolt.org/z/T1TWd3qon

Backtrace:
```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang -g -o /app/output.s -fno-verbose-asm -S -target wasm32 -fcolor-diagnostics -fno-crash-diagnostics -std=c23 <source>
1.	<source>:1:47: current parser token ')'
 #0 0x00000000039af2d8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-trunk/bin/clang+0x39af2d8)
 #1 0x00000000039ad424 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+0x39ad424)
 #2 0x00000000038fdd58 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007e7769442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x0000000006fdf3a3 clang::ASTContext::removeAddrSpaceQualType(clang::QualType) const (/opt/compiler-explorer/clang-trunk/bin/clang+0x6fdf3a3)
 #5 0x0000000006d9011d processTypeAttrs((anonymous namespace)::TypeProcessingState&, clang::QualType&, TypeAttrLocation, clang::ParsedAttributesView const&, clang::CUDAFunctionTarget) SemaType.cpp:0:0
 #6 0x0000000006d9369c GetDeclSpecTypeForDeclarator((anonymous namespace)::TypeProcessingState&, clang::TypeSourceInfo*&) (.constprop.0) SemaType.cpp:0:0
 #7 0x0000000006da2c4c clang::Sema::GetTypeForDeclarator(clang::Declarator&) (/opt/compiler-explorer/clang-trunk/bin/clang+0x6da2c4c)
 #8 0x0000000006529c39 clang::Sema::ActOnParamDeclarator(clang::Scope*, clang::Declarator&, clang::SourceLocation) (/opt/compiler-explorer/clang-trunk/bin/clang+0x6529c39)
 #9 0x00000000061fb8c0 clang::Parser::ParseParameterDeclarationClause(clang::DeclaratorContext, clang::ParsedAttributes&, llvm::SmallVectorImpl<clang::DeclaratorChunk::ParamInfo>&, clang::SourceLocation&, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+0x61fb8c0)
#10 0x00000000061fd79b clang::Parser::ParseFunctionDeclarator(clang::Declarator&, clang::ParsedAttributes&, clang::BalancedDelimiterTracker&, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+0x61fd79b)
#11 0x00000000061ff352 clang::Parser::ParseDirectDeclarator(clang::Declarator&) (/opt/compiler-explorer/clang-trunk/bin/clang+0x61ff352)
#12 0x00000000061f7adc clang::Parser::ParseDeclaratorInternal(clang::Declarator&, void (clang::Parser::*)(clang::Declarator&)) (/opt/compiler-explorer/clang-trunk/bin/clang+0x61f7adc)
#13 0x0000000007838951 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-trunk/bin/clang+0x7838951)
#14 0x00000000061f3e24 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x61f3e24)
#15 0x00000000061a2c87 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+0x61a2c87)
#16 0x00000000061a3aa3 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+0x61a3aa3)
#17 0x00000000061ad392 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x61ad392)
#18 0x00000000061aec28 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-trunk/bin/clang+0x61aec28)
#19 0x00000000061af170 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-trunk/bin/clang+0x61af170)
#20 0x000000000619d2c2 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+0x619d2c2)
#21 0x00000000042fe835 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+0x42fe835)
#22 0x00000000045c8761 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+0x45c8761)
#23 0x000000000454832b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-trunk/bin/clang+0x454832b)
#24 0x00000000046b10c3 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x46b10c3)
#25 0x0000000000ccb66c cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+0xccb66c)
#26 0x0000000000cc3fad ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#27 0x00000000043467c9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#28 0x00000000038fe183 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-trunk/bin/clang+0x38fe183)
#29 0x00000000043469e9 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#30 0x000000000430e21d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+0x430e21d)
#31 0x000000000430f21d clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+0x430f21d)
#32 0x00000000043187cc clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-trunk/bin/clang+0x43187cc)
#33 0x0000000000cc8301 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-trunk/bin/clang+0xcc8301)
#34 0x0000000000b96244 main (/opt/compiler-explorer/clang-trunk/bin/clang+0xb96244)
#35 0x00007e7769429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#36 0x00007e7769429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#37 0x0000000000cc3a4e _start (/opt/compiler-explorer/clang-trunk/bin/clang+0xcc3a4e)
clang: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Moony (moonheart08)

<details>
```c
int hsGetFuncRefForGlobal( __funcref function );
```

This is sufficient to cause the Clang frontend to crash for WebAssembly. Godbolt demonstration: https://godbolt.org/z/T1TWd3qon

Backtrace:
```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang -g -o /app/output.s -fno-verbose-asm -S -target wasm32 -fcolor-diagnostics -fno-crash-diagnostics -std=c23 &lt;source&gt;
1.	&lt;source&gt;:1:47: current parser token ')'
 #<!-- -->0 0x00000000039af2d8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-trunk/bin/clang+0x39af2d8)
 #<!-- -->1 0x00000000039ad424 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+0x39ad424)
 #<!-- -->2 0x00000000038fdd58 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007e7769442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x0000000006fdf3a3 clang::ASTContext::removeAddrSpaceQualType(clang::QualType) const (/opt/compiler-explorer/clang-trunk/bin/clang+0x6fdf3a3)
 #<!-- -->5 0x0000000006d9011d processTypeAttrs((anonymous namespace)::TypeProcessingState&amp;, clang::QualType&amp;, TypeAttrLocation, clang::ParsedAttributesView const&amp;, clang::CUDAFunctionTarget) SemaType.cpp:0:0
 #<!-- -->6 0x0000000006d9369c GetDeclSpecTypeForDeclarator((anonymous namespace)::TypeProcessingState&amp;, clang::TypeSourceInfo*&amp;) (.constprop.0) SemaType.cpp:0:0
 #<!-- -->7 0x0000000006da2c4c clang::Sema::GetTypeForDeclarator(clang::Declarator&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x6da2c4c)
 #<!-- -->8 0x0000000006529c39 clang::Sema::ActOnParamDeclarator(clang::Scope*, clang::Declarator&amp;, clang::SourceLocation) (/opt/compiler-explorer/clang-trunk/bin/clang+0x6529c39)
 #<!-- -->9 0x00000000061fb8c0 clang::Parser::ParseParameterDeclarationClause(clang::DeclaratorContext, clang::ParsedAttributes&amp;, llvm::SmallVectorImpl&lt;clang::DeclaratorChunk::ParamInfo&gt;&amp;, clang::SourceLocation&amp;, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+0x61fb8c0)
#<!-- -->10 0x00000000061fd79b clang::Parser::ParseFunctionDeclarator(clang::Declarator&amp;, clang::ParsedAttributes&amp;, clang::BalancedDelimiterTracker&amp;, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+0x61fd79b)
#<!-- -->11 0x00000000061ff352 clang::Parser::ParseDirectDeclarator(clang::Declarator&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x61ff352)
#<!-- -->12 0x00000000061f7adc clang::Parser::ParseDeclaratorInternal(clang::Declarator&amp;, void (clang::Parser::*)(clang::Declarator&amp;)) (/opt/compiler-explorer/clang-trunk/bin/clang+0x61f7adc)
#<!-- -->13 0x0000000007838951 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x7838951)
#<!-- -->14 0x00000000061f3e24 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x61f3e24)
#<!-- -->15 0x00000000061a2c87 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+0x61a2c87)
#<!-- -->16 0x00000000061a3aa3 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+0x61a3aa3)
#<!-- -->17 0x00000000061ad392 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x61ad392)
#<!-- -->18 0x00000000061aec28 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x61aec28)
#<!-- -->19 0x00000000061af170 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x61af170)
#<!-- -->20 0x000000000619d2c2 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+0x619d2c2)
#<!-- -->21 0x00000000042fe835 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+0x42fe835)
#<!-- -->22 0x00000000045c8761 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+0x45c8761)
#<!-- -->23 0x000000000454832b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x454832b)
#<!-- -->24 0x00000000046b10c3 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x46b10c3)
#<!-- -->25 0x0000000000ccb66c cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+0xccb66c)
#<!-- -->26 0x0000000000cc3fad ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->27 0x00000000043467c9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->28 0x00000000038fe183 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x38fe183)
#<!-- -->29 0x00000000043469e9 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->30 0x000000000430e21d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+0x430e21d)
#<!-- -->31 0x000000000430f21d clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+0x430f21d)
#<!-- -->32 0x00000000043187cc clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x43187cc)
#<!-- -->33 0x0000000000cc8301 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0xcc8301)
#<!-- -->34 0x0000000000b96244 main (/opt/compiler-explorer/clang-trunk/bin/clang+0xb96244)
#<!-- -->35 0x00007e7769429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->36 0x00007e7769429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->37 0x0000000000cc3a4e _start (/opt/compiler-explorer/clang-trunk/bin/clang+0xcc3a4e)
clang: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139```
</details>


---

### Comment 2 - shafik

CC @pmatos 



---

### Comment 3 - pmatos

> CC [@pmatos](https://github.com/pmatos)

I apologize but the customer who was supporting our work in this area - at Igalia - had a change in priorities and therefore we cannot resume work here.

---

### Comment 4 - AaronBallman

There is no type checking happening for the attribute in: https://github.com/llvm/llvm-project/blob/870ffbd9ed566079227f24606c88c5d9276378ea/clang/lib/Sema/SemaType.cpp#L7160

This means using the attribute on anything other than a pointer type will always crash. There needs to be a check that the attribute is only written on a function pointer type and diagnose otherwise. (I don't know whether member function pointers should be allowed or not, but since this appears to be unmaintained, I think it should be rejected until a wasm person comes along to maintain it.)

---

### Comment 5 - llvmbot


Hi!

This issue may be a good introductory issue for people new to working on LLVM. If you would like to work on this issue, your first steps are:

1. Check that no other contributor is working on this issue. If someone is assigned to the issue or claimed to be working on it, ping the person. After one week without a response, the assignee may be changed.
1. Leave a comment indicating that you are working on the issue, or just create a [pull request](https://github.com/llvm/llvm-project/pulls) after following the steps below. [Mention](https://docs.github.com/en/issues/tracking-your-work-with-issues/linking-a-pull-request-to-an-issue) this issue in the description of the pull request.
1. Fix the issue locally.
1. [Run the test suite](https://llvm.org/docs/TestingGuide.html#unit-and-regression-tests) locally. Remember that the subdirectories under `test/` create fine-grained testing targets, so you can e.g. use `make check-clang-ast` to only run Clang's AST tests.
1. Create a Git commit.
1. Run [`git clang-format HEAD~1`](https://clang.llvm.org/docs/ClangFormat.html#git-integration) to format your changes.
1. Open a [pull request](https://github.com/llvm/llvm-project/pulls) to the [upstream repository](https://github.com/llvm/llvm-project) on GitHub. Detailed instructions can be found [in GitHub's documentation](https://docs.github.com/en/pull-requests/collaborating-with-pull-requests/proposing-changes-to-your-work-with-pull-requests/creating-a-pull-request). [Mention](https://docs.github.com/en/issues/tracking-your-work-with-issues/linking-a-pull-request-to-an-issue) this issue in the description of the pull request.

If you have any further questions about this issue, don't hesitate to ask via a comment in the thread below.



---

### Comment 6 - llvmbot


@llvm/issue-subscribers-good-first-issue

Author: Moony (moonheart08)

<details>
```c
int hsGetFuncRefForGlobal( __funcref function );
```

This is sufficient to cause the Clang frontend to crash for WebAssembly. Godbolt demonstration: https://godbolt.org/z/T1TWd3qon

Backtrace:
```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang -g -o /app/output.s -fno-verbose-asm -S -target wasm32 -fcolor-diagnostics -fno-crash-diagnostics -std=c23 &lt;source&gt;
1.	&lt;source&gt;:1:47: current parser token ')'
 #<!-- -->0 0x00000000039af2d8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-trunk/bin/clang+0x39af2d8)
 #<!-- -->1 0x00000000039ad424 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+0x39ad424)
 #<!-- -->2 0x00000000038fdd58 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007e7769442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x0000000006fdf3a3 clang::ASTContext::removeAddrSpaceQualType(clang::QualType) const (/opt/compiler-explorer/clang-trunk/bin/clang+0x6fdf3a3)
 #<!-- -->5 0x0000000006d9011d processTypeAttrs((anonymous namespace)::TypeProcessingState&amp;, clang::QualType&amp;, TypeAttrLocation, clang::ParsedAttributesView const&amp;, clang::CUDAFunctionTarget) SemaType.cpp:0:0
 #<!-- -->6 0x0000000006d9369c GetDeclSpecTypeForDeclarator((anonymous namespace)::TypeProcessingState&amp;, clang::TypeSourceInfo*&amp;) (.constprop.0) SemaType.cpp:0:0
 #<!-- -->7 0x0000000006da2c4c clang::Sema::GetTypeForDeclarator(clang::Declarator&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x6da2c4c)
 #<!-- -->8 0x0000000006529c39 clang::Sema::ActOnParamDeclarator(clang::Scope*, clang::Declarator&amp;, clang::SourceLocation) (/opt/compiler-explorer/clang-trunk/bin/clang+0x6529c39)
 #<!-- -->9 0x00000000061fb8c0 clang::Parser::ParseParameterDeclarationClause(clang::DeclaratorContext, clang::ParsedAttributes&amp;, llvm::SmallVectorImpl&lt;clang::DeclaratorChunk::ParamInfo&gt;&amp;, clang::SourceLocation&amp;, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+0x61fb8c0)
#<!-- -->10 0x00000000061fd79b clang::Parser::ParseFunctionDeclarator(clang::Declarator&amp;, clang::ParsedAttributes&amp;, clang::BalancedDelimiterTracker&amp;, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+0x61fd79b)
#<!-- -->11 0x00000000061ff352 clang::Parser::ParseDirectDeclarator(clang::Declarator&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x61ff352)
#<!-- -->12 0x00000000061f7adc clang::Parser::ParseDeclaratorInternal(clang::Declarator&amp;, void (clang::Parser::*)(clang::Declarator&amp;)) (/opt/compiler-explorer/clang-trunk/bin/clang+0x61f7adc)
#<!-- -->13 0x0000000007838951 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x7838951)
#<!-- -->14 0x00000000061f3e24 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x61f3e24)
#<!-- -->15 0x00000000061a2c87 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+0x61a2c87)
#<!-- -->16 0x00000000061a3aa3 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+0x61a3aa3)
#<!-- -->17 0x00000000061ad392 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x61ad392)
#<!-- -->18 0x00000000061aec28 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x61aec28)
#<!-- -->19 0x00000000061af170 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x61af170)
#<!-- -->20 0x000000000619d2c2 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+0x619d2c2)
#<!-- -->21 0x00000000042fe835 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+0x42fe835)
#<!-- -->22 0x00000000045c8761 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+0x45c8761)
#<!-- -->23 0x000000000454832b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x454832b)
#<!-- -->24 0x00000000046b10c3 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x46b10c3)
#<!-- -->25 0x0000000000ccb66c cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+0xccb66c)
#<!-- -->26 0x0000000000cc3fad ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->27 0x00000000043467c9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->28 0x00000000038fe183 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x38fe183)
#<!-- -->29 0x00000000043469e9 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->30 0x000000000430e21d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+0x430e21d)
#<!-- -->31 0x000000000430f21d clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+0x430f21d)
#<!-- -->32 0x00000000043187cc clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x43187cc)
#<!-- -->33 0x0000000000cc8301 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0xcc8301)
#<!-- -->34 0x0000000000b96244 main (/opt/compiler-explorer/clang-trunk/bin/clang+0xb96244)
#<!-- -->35 0x00007e7769429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->36 0x00007e7769429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->37 0x0000000000cc3a4e _start (/opt/compiler-explorer/clang-trunk/bin/clang+0xcc3a4e)
clang: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139```
</details>


---

### Comment 7 - moonheart08

If this is a good first issue as stated I think I'd like to fix it myself, can I be assigned?
General plan is the given one, introduce appropriate diagnostic for invalid types, opting against allowing member function pointers for now.
 

---

### Comment 8 - AaronBallman

Ohhhh, I take it back about this being a good first issue (sorry @moonheart08!), it might not be. This thing seems to have even more problems: https://godbolt.org/z/3hn7x75Mf (using it at all in C++ crashes always, only seems to work in C and only if you hold it just right). I think we need to find an active wasm maintainer to step in for this, but I'm not certain who to reach out to. CC @nikic who may know someone

---

### Comment 9 - nikic

cc @dschuff @aheejin 

---

### Comment 10 - pmatos

A quick look again after a while

> Ohhhh, I take it back about this being a good first issue (sorry [@moonheart08](https://github.com/moonheart08)!), it might not be. This thing seems to have even more problems: https://godbolt.org/z/3hn7x75Mf (using it at all in C++ crashes always, only seems to work in C and only if you hold it just right). I think we need to find an active wasm maintainer to step in for this, but I'm not certain who to reach out to. CC [@nikic](https://github.com/nikic) who may know someone

If `function` variable is not defined, the type defaults to int and then it crashes. From a look at this, it seems we need a check to ensure that the type of the funcref variable is a function pointer.

---

### Comment 11 - AaronBallman

> A quick look again after a while
> 
> > Ohhhh, I take it back about this being a good first issue (sorry [@moonheart08](https://github.com/moonheart08)!), it might not be. This thing seems to have even more problems: https://godbolt.org/z/3hn7x75Mf (using it at all in C++ crashes always, only seems to work in C and only if you hold it just right). I think we need to find an active wasm maintainer to step in for this, but I'm not certain who to reach out to. CC [@nikic](https://github.com/nikic) who may know someone
> 
> If `function` variable is not defined, the type defaults to int and then it crashes. From a look at this, it seems we need a check to ensure that the type of the funcref variable is a function pointer.

Thanks! The fix you posted is pretty close to good, but there is a second (separate) issue that remains. If you enable codegen in C++ mode, any use of `__funcref` crashes even after your patch:
```
clang++: warning: argument unused during compilation: '--gcc-toolchain=/opt/compiler-explorer/gcc-snapshot' [-Wunused-command-line-argument]
Not a language specific address space
UNREACHABLE executed at /root/llvm-project/llvm/tools/clang/lib/AST/ItaniumMangle.cpp:2748!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -target wasm32 <source>
1.	<eof> parser at end of file
2.	<source>:1:18: LLVM IR generation of declaration 'ptr'
3.	<source>:1:18: Generating code for declaration 'ptr'
 #0 0x00000000042fea18 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42fea18)
 #1 0x00000000042fbe54 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42fbe54)
 #2 0x000000000423e748 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x0000753577e42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x0000753577e969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x0000753577e42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x0000753577e287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00000000042491da (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42491da)
 #8 0x00000000080226e7 (anonymous namespace)::CXXNameMangler::mangleQualifiers(clang::Qualifiers, clang::DependentAddressSpaceType const*) ItaniumMangle.cpp:0:0
 #9 0x00000000080154d5 (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) ItaniumMangle.cpp:0:0
#10 0x00000000080156b5 (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) ItaniumMangle.cpp:0:0
#11 0x00000000080135a0 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#12 0x000000000802569f (anonymous namespace)::CXXNameMangler::mangle(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#13 0x0000000008027266 (anonymous namespace)::CXXNameMangler::shouldHaveAbiTags((anonymous namespace)::ItaniumMangleContextImpl&, clang::VarDecl const*) ItaniumMangle.cpp:0:0
#14 0x00000000080276ab (anonymous namespace)::ItaniumMangleContextImpl::shouldMangleCXXName(clang::NamedDecl const*) ItaniumMangle.cpp:0:0
#15 0x00000000048a4fb4 getMangledNameImpl(clang::CodeGen::CodeGenModule&, clang::GlobalDecl, clang::NamedDecl const*, bool) CodeGenModule.cpp:0:0
#16 0x00000000048bc7cd clang::CodeGen::CodeGenModule::getMangledName(clang::GlobalDecl) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48bc7cd)
#17 0x00000000048d0205 clang::CodeGen::CodeGenModule::GetAddrOfGlobalVar(clang::VarDecl const*, llvm::Type*, clang::CodeGen::ForDefinition_t) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48d0205)
#18 0x00000000048cbaee clang::CodeGen::CodeGenModule::EmitGlobalVarDefinition(clang::VarDecl const*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48cbaee)
#19 0x00000000048f4721 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48f4721)
#20 0x00000000048f4f63 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48f4f63)
#21 0x000000000490037b clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#22 0x0000000004c686e9 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#23 0x0000000004c59ad4 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c59ad4)
#24 0x0000000006a3a774 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a3a774)
#25 0x0000000004c66488 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c66488)
#26 0x0000000004f60135 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f60135)
#27 0x0000000004edff0e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4edff0e)
#28 0x000000000505a7cd clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x505a7cd)
#29 0x0000000000dce92e cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdce92e)
#30 0x0000000000dc52fa ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#31 0x0000000000dc547d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#32 0x0000000004cd96e9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#33 0x000000000423ebe4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x423ebe4)
#34 0x0000000004cd9cff clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#35 0x0000000004c9a692 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c9a692)
#36 0x0000000004c9b63e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c9b63e)
#37 0x0000000004ca2a95 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ca2a95)
#38 0x0000000000dcaca1 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdcaca1)
#39 0x0000000000c88434 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc88434)
#40 0x0000753577e29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#41 0x0000753577e29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#42 0x0000000000dc4d95 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdc4d95)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
https://godbolt.org/z/3hn7x75Mf

I'll file this as a separate issue so we can track it more easily.

---

### Comment 12 - pmatos

> > A quick look again after a while
> > > Ohhhh, I take it back about this being a good first issue (sorry [@moonheart08](https://github.com/moonheart08)!), it might not be. This thing seems to have even more problems: https://godbolt.org/z/3hn7x75Mf (using it at all in C++ crashes always, only seems to work in C and only if you hold it just right). I think we need to find an active wasm maintainer to step in for this, but I'm not certain who to reach out to. CC [@nikic](https://github.com/nikic) who may know someone
> > 
> > 
> > If `function` variable is not defined, the type defaults to int and then it crashes. From a look at this, it seems we need a check to ensure that the type of the funcref variable is a function pointer.
> 
> Thanks! The fix you posted is pretty close to good, but there is a second (separate) issue that remains. If you enable codegen in C++ mode, any use of `__funcref` crashes even after your patch:
> 
> ```
> clang++: warning: argument unused during compilation: '--gcc-toolchain=/opt/compiler-explorer/gcc-snapshot' [-Wunused-command-line-argument]
> Not a language specific address space
> UNREACHABLE executed at /root/llvm-project/llvm/tools/clang/lib/AST/ItaniumMangle.cpp:2748!
> PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
> Stack dump:
> 0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -target wasm32 <source>
> 1.	<eof> parser at end of file
> 2.	<source>:1:18: LLVM IR generation of declaration 'ptr'
> 3.	<source>:1:18: Generating code for declaration 'ptr'
>  #0 0x00000000042fea18 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42fea18)
>  #1 0x00000000042fbe54 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42fbe54)
>  #2 0x000000000423e748 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
>  #3 0x0000753577e42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
>  #4 0x0000753577e969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
>  #5 0x0000753577e42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
>  #6 0x0000753577e287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
>  #7 0x00000000042491da (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42491da)
>  #8 0x00000000080226e7 (anonymous namespace)::CXXNameMangler::mangleQualifiers(clang::Qualifiers, clang::DependentAddressSpaceType const*) ItaniumMangle.cpp:0:0
>  #9 0x00000000080154d5 (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) ItaniumMangle.cpp:0:0
> #10 0x00000000080156b5 (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) ItaniumMangle.cpp:0:0
> #11 0x00000000080135a0 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) ItaniumMangle.cpp:0:0
> #12 0x000000000802569f (anonymous namespace)::CXXNameMangler::mangle(clang::GlobalDecl) ItaniumMangle.cpp:0:0
> #13 0x0000000008027266 (anonymous namespace)::CXXNameMangler::shouldHaveAbiTags((anonymous namespace)::ItaniumMangleContextImpl&, clang::VarDecl const*) ItaniumMangle.cpp:0:0
> #14 0x00000000080276ab (anonymous namespace)::ItaniumMangleContextImpl::shouldMangleCXXName(clang::NamedDecl const*) ItaniumMangle.cpp:0:0
> #15 0x00000000048a4fb4 getMangledNameImpl(clang::CodeGen::CodeGenModule&, clang::GlobalDecl, clang::NamedDecl const*, bool) CodeGenModule.cpp:0:0
> #16 0x00000000048bc7cd clang::CodeGen::CodeGenModule::getMangledName(clang::GlobalDecl) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48bc7cd)
> #17 0x00000000048d0205 clang::CodeGen::CodeGenModule::GetAddrOfGlobalVar(clang::VarDecl const*, llvm::Type*, clang::CodeGen::ForDefinition_t) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48d0205)
> #18 0x00000000048cbaee clang::CodeGen::CodeGenModule::EmitGlobalVarDefinition(clang::VarDecl const*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48cbaee)
> #19 0x00000000048f4721 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48f4721)
> #20 0x00000000048f4f63 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48f4f63)
> #21 0x000000000490037b clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
> #22 0x0000000004c686e9 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
> #23 0x0000000004c59ad4 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c59ad4)
> #24 0x0000000006a3a774 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a3a774)
> #25 0x0000000004c66488 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c66488)
> #26 0x0000000004f60135 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f60135)
> #27 0x0000000004edff0e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4edff0e)
> #28 0x000000000505a7cd clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x505a7cd)
> #29 0x0000000000dce92e cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdce92e)
> #30 0x0000000000dc52fa ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
> #31 0x0000000000dc547d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
> #32 0x0000000004cd96e9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
> #33 0x000000000423ebe4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x423ebe4)
> #34 0x0000000004cd9cff clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
> #35 0x0000000004c9a692 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c9a692)
> #36 0x0000000004c9b63e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c9b63e)
> #37 0x0000000004ca2a95 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ca2a95)
> #38 0x0000000000dcaca1 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdcaca1)
> #39 0x0000000000c88434 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc88434)
> #40 0x0000753577e29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
> #41 0x0000753577e29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
> #42 0x0000000000dc4d95 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdc4d95)
> clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
> Compiler returned: 134
> ```
> 
> https://godbolt.org/z/3hn7x75Mf
> 
> I'll file this as a separate issue so we can track it more easily.

Interesting - I was pretty confident my tests in clang/test/SemaCXX/wasm-funcref-crash.cpp would check that. Thanks for opening a new issue.

---

### Comment 13 - pmatos


> Interesting - I was pretty confident my tests in clang/test/SemaCXX/wasm-funcref-crash.cpp would check that. Thanks for opening a new issue.

OK, I fixed the Sema part. But this new one is a code gen issue.


---

### Comment 14 - AaronBallman

> > Interesting - I was pretty confident my tests in clang/test/SemaCXX/wasm-funcref-crash.cpp would check that. Thanks for opening a new issue.
> 
> OK, I fixed the Sema part. But this new one is a code gen issue.

Yeah, because there's an `-fsyntax-only` involved, codegen never runs. You'd only see it if you do `-emit-llvm`

---

### Comment 15 - pmatos

> > > Interesting - I was pretty confident my tests in clang/test/SemaCXX/wasm-funcref-crash.cpp would check that. Thanks for opening a new issue.
> > 
> > 
> > OK, I fixed the Sema part. But this new one is a code gen issue.
> 
> Yeah, because there's an `-fsyntax-only` involved, codegen never runs. You'd only see it if you do `-emit-llvm`

Indeed, I will fix it in a separate PR.

---

