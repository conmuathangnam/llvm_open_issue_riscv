# [LifetimeSafety] Assertion failure with `static operator()`

**Author:** davidstone
**URL:** https://github.com/llvm/llvm-project/issues/187426
**Status:** Closed
**Labels:** crash, clang:temporal-safety
**Closed Date:** 2026-03-23T16:53:36Z

## Body

The following valid translation unit:

```c++
struct s {
	static void operator()(int, int &&);
};

void f() {
	s()(1, 1);
}
```

when compiled with `-std=c++23` and any of the following warnings:

* `-Wlifetime-safety-noescape`
* `-Wlifetime-safety-invalidation`
* `-Wlifetime-safety-return-stack-addr-moved`
* `-Wlifetime-safety-use-after-scope-moved`
* `-Wlifetime-safety-use-after-scope`
* `-Wlifetime-safety-return-stack-addr`

causes clang main to crash with

```console
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++23 -Wlifetime-safety-return-stack-addr <source> -isystem/opt/compiler-explorer/libs/llvm/trunk/include
1.	<eof> parser at end of file
2.	<source>:5:10: parsing function body 'f'
 #0 0x000000000444ea18 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x444ea18)
 #1 0x000000000444b844 llvm::sys::RunSignalHandlers() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x444b844)
 #2 0x000000000444be64 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x444be64)
 #3 0x000000000438b208 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #4 0x000075e136e42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #5 0x0000000007a5c04b clang::lifetimes::internal::FactsGenerator::handleMovedArgsInCall(clang::FunctionDecl const*, llvm::ArrayRef<clang::Expr const*>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7a5c04b)
 #6 0x0000000007a5c82a clang::lifetimes::internal::FactsGenerator::handleFunctionCall(clang::Expr const*, clang::FunctionDecl const*, llvm::ArrayRef<clang::Expr const*>, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7a5c82a)
 #7 0x0000000007a5fa3f clang::lifetimes::internal::FactsGenerator::run() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7a5fa3f)
 #8 0x0000000007a09826 clang::lifetimes::internal::LifetimeSafetyAnalysis::run() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7a09826)
 #9 0x0000000007a0a795 clang::lifetimes::runLifetimeSafetyAnalysis(clang::AnalysisDeclContext&, clang::lifetimes::LifetimeSafetySemaHelper*, clang::lifetimes::LifetimeSafetyStats&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7a0a795)
#10 0x00000000079401e3 clang::sema::AnalysisBasedWarnings::IssueWarnings(clang::sema::AnalysisBasedWarnings::Policy, clang::sema::FunctionScopeInfo*, clang::Decl const*, clang::QualType) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x79401e3)
#11 0x0000000006dee0b2 clang::Sema::PopFunctionScopeInfo(clang::sema::AnalysisBasedWarnings::Policy const*, clang::Decl*, clang::QualType) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6dee0b2)
#12 0x00000000070501dc clang::Sema::ActOnFinishFunctionBody(clang::Decl*, clang::Stmt*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x70501dc)
#13 0x0000000006d66d24 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d66d24)
#14 0x0000000006c6c3f3 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c6c3f3)
#15 0x0000000006ca2a8d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ca2a8d)
#16 0x0000000006c5fdcb clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c5fdcb)
#17 0x0000000006c605cf clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c605cf)
#18 0x0000000006c67bf1 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c67bf1)
#19 0x0000000006c68b25 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c68b25)
#20 0x0000000006c4a1ea clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c4a1ea)
#21 0x0000000004f2a6b8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f2a6b8)
#22 0x0000000005236385 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5236385)
#23 0x00000000051b41ee clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x51b41ee)
#24 0x0000000005332d4d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5332d4d)
#25 0x0000000000decf52 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdecf52)
#26 0x0000000000de388a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#27 0x0000000000de3a0d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#28 0x0000000004fa09f9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#29 0x000000000438b644 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x438b644)
#30 0x0000000004fa1026 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#31 0x0000000004f60102 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f60102)
#32 0x0000000004f610ae clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f610ae)
#33 0x0000000004f68c65 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f68c65)
#34 0x0000000000de92b4 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde92b4)
#35 0x0000000000ca091a main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xca091a)
#36 0x000075e136e29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#37 0x000075e136e29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#38 0x0000000000de3325 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde3325)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
Compiler returned: 139
```

See it live: https://godbolt.org/z/sef15G95h

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-temporal-safety

Author: David Stone (davidstone)

<details>
The following valid translation unit:

```c++
struct s {
	static void operator()(int, int &amp;&amp;);
};

void f() {
	s()(1, 1);
}
```

when compiled with `-std=c++23 -Wlifetime-safety-return-stack-addr` causes clang main to crash with

```console
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++23 -Wlifetime-safety-return-stack-addr &lt;source&gt; -isystem/opt/compiler-explorer/libs/llvm/trunk/include
1.	&lt;eof&gt; parser at end of file
2.	&lt;source&gt;:5:10: parsing function body 'f'
 #<!-- -->0 0x000000000444ea18 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x444ea18)
 #<!-- -->1 0x000000000444b844 llvm::sys::RunSignalHandlers() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x444b844)
 #<!-- -->2 0x000000000444be64 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x444be64)
 #<!-- -->3 0x000000000438b208 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->4 0x000075e136e42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->5 0x0000000007a5c04b clang::lifetimes::internal::FactsGenerator::handleMovedArgsInCall(clang::FunctionDecl const*, llvm::ArrayRef&lt;clang::Expr const*&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7a5c04b)
 #<!-- -->6 0x0000000007a5c82a clang::lifetimes::internal::FactsGenerator::handleFunctionCall(clang::Expr const*, clang::FunctionDecl const*, llvm::ArrayRef&lt;clang::Expr const*&gt;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7a5c82a)
 #<!-- -->7 0x0000000007a5fa3f clang::lifetimes::internal::FactsGenerator::run() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7a5fa3f)
 #<!-- -->8 0x0000000007a09826 clang::lifetimes::internal::LifetimeSafetyAnalysis::run() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7a09826)
 #<!-- -->9 0x0000000007a0a795 clang::lifetimes::runLifetimeSafetyAnalysis(clang::AnalysisDeclContext&amp;, clang::lifetimes::LifetimeSafetySemaHelper*, clang::lifetimes::LifetimeSafetyStats&amp;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7a0a795)
#<!-- -->10 0x00000000079401e3 clang::sema::AnalysisBasedWarnings::IssueWarnings(clang::sema::AnalysisBasedWarnings::Policy, clang::sema::FunctionScopeInfo*, clang::Decl const*, clang::QualType) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x79401e3)
#<!-- -->11 0x0000000006dee0b2 clang::Sema::PopFunctionScopeInfo(clang::sema::AnalysisBasedWarnings::Policy const*, clang::Decl*, clang::QualType) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6dee0b2)
#<!-- -->12 0x00000000070501dc clang::Sema::ActOnFinishFunctionBody(clang::Decl*, clang::Stmt*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x70501dc)
#<!-- -->13 0x0000000006d66d24 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d66d24)
#<!-- -->14 0x0000000006c6c3f3 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c6c3f3)
#<!-- -->15 0x0000000006ca2a8d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ca2a8d)
#<!-- -->16 0x0000000006c5fdcb clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c5fdcb)
#<!-- -->17 0x0000000006c605cf clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c605cf)
#<!-- -->18 0x0000000006c67bf1 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c67bf1)
#<!-- -->19 0x0000000006c68b25 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c68b25)
#<!-- -->20 0x0000000006c4a1ea clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c4a1ea)
#<!-- -->21 0x0000000004f2a6b8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f2a6b8)
#<!-- -->22 0x0000000005236385 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5236385)
#<!-- -->23 0x00000000051b41ee clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x51b41ee)
#<!-- -->24 0x0000000005332d4d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5332d4d)
#<!-- -->25 0x0000000000decf52 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdecf52)
#<!-- -->26 0x0000000000de388a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->27 0x0000000000de3a0d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->28 0x0000000004fa09f9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->29 0x000000000438b644 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x438b644)
#<!-- -->30 0x0000000004fa1026 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->31 0x0000000004f60102 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f60102)
#<!-- -->32 0x0000000004f610ae clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f610ae)
#<!-- -->33 0x0000000004f68c65 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f68c65)
#<!-- -->34 0x0000000000de92b4 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde92b4)
#<!-- -->35 0x0000000000ca091a main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xca091a)
#<!-- -->36 0x000075e136e29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->37 0x000075e136e29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->38 0x0000000000de3325 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde3325)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
Compiler returned: 139
```

See it live: https://godbolt.org/z/sef15G95h
</details>


---

### Comment 2 - NeKon69

I would like to work on this!

---

### Comment 3 - usx95

Related:
```cpp
#include <string>
struct s {
    static int& operator()(std::string&&,
                           const int& a [[clang::lifetimebound]],
                           const int& b [[clang::lifetimebound]]);
};

void f() {
    int& x = s()("1", 2, 3);
    (void)x;
}
```
```
<source>:9:23: warning: object whose reference is captured does not live long enough [-Wlifetime-safety-use-after-scope]
    9 |     int& x = s()("1", 2, 3);
      |                       ^
<source>:9:23: note: destroyed here
<source>:10:11: note: later used here
   10 |     (void)x;
      |           ^
<source>:9:18: warning: object whose reference is captured does not live long enough [-Wlifetime-safety-use-after-scope]
    9 |     int& x = s()("1", 2, 3);
      |                  ^~~
<source>:9:18: note: destroyed here
<source>:10:11: note: later used here
   10 |     (void)x;
      |           ^
```
Wrongly identifies 1 and 2 are dangling. Correct: 2 and 3 are dangling!
https://godbolt.org/z/Mv5Edc7zc

---

