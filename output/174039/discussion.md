# [clang] Assertion `LV.getLinkage() == OldLinkage' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/174039

## Body

Reproducer:
https://godbolt.org/z/WzMsvx4K7
```cpp
struct S {};

const S foo;

extern const S foo = {1};

extern S foo;
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/AST/Decl.cpp:1617: clang::LinkageInfo clang::LinkageComputer::getLVForDecl(const clang::NamedDecl*, clang::LVComputationKind): Assertion `LV.getLinkage() == OldLinkage' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<source>:7:13: current parser token ';'
 #0 0x0000000004302f28 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4302f28)
 #1 0x0000000004300354 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4300354)
 #2 0x0000000004244d58 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007f0437e42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007f0437e969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007f0437e42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007f0437e287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007f0437e2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007f0437e39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000007b9d07f clang::LinkageComputer::getLVForDecl(clang::NamedDecl const*, clang::LVComputationKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7b9d07f)
#10 0x0000000007b9d24f clang::NamedDecl::getLinkageInternal() const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7b9d24f)
#11 0x0000000006e267d1 clang::Sema::FinalizeDeclaration(clang::Decl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e267d1)
#12 0x0000000006a6ff36 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a6ff36)
#13 0x0000000006a7dc7a clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a7dc7a)
#14 0x0000000006a3b32b clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a3b32b)
#15 0x0000000006a3bb1f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a3bb1f)
#16 0x0000000006a4394a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a4394a)
#17 0x0000000006a448e5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a448e5)
#18 0x0000000006a2596a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a2596a)
#19 0x0000000004c64298 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c64298)
#20 0x0000000004f5a915 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f5a915)
#21 0x0000000004edabae clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4edabae)
#22 0x0000000005054c9d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5054c9d)
#23 0x0000000000de672c cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde672c)
#24 0x0000000000ddd17a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#25 0x0000000000ddd2fd int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#26 0x0000000004cd6969 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#27 0x00000000042451f4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42451f4)
#28 0x0000000004cd6f7f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#29 0x0000000004c97a62 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c97a62)
#30 0x0000000004c98a0e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c98a0e)
#31 0x0000000004c9fe65 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c9fe65)
#32 0x0000000000de2b21 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde2b21)
#33 0x0000000000c8d454 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc8d454)
#34 0x00007f0437e29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#35 0x00007f0437e29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#36 0x0000000000ddcc15 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xddcc15)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/WzMsvx4K7
```cpp
struct S {};

const S foo;

extern const S foo = {1};

extern S foo;
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/AST/Decl.cpp:1617: clang::LinkageInfo clang::LinkageComputer::getLVForDecl(const clang::NamedDecl*, clang::LVComputationKind): Assertion `LV.getLinkage() == OldLinkage' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics &lt;source&gt;
1.	&lt;source&gt;:7:13: current parser token ';'
 #<!-- -->0 0x0000000004302f28 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4302f28)
 #<!-- -->1 0x0000000004300354 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4300354)
 #<!-- -->2 0x0000000004244d58 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007f0437e42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007f0437e969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007f0437e42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007f0437e287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x00007f0437e2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x00007f0437e39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000007b9d07f clang::LinkageComputer::getLVForDecl(clang::NamedDecl const*, clang::LVComputationKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7b9d07f)
#<!-- -->10 0x0000000007b9d24f clang::NamedDecl::getLinkageInternal() const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7b9d24f)
#<!-- -->11 0x0000000006e267d1 clang::Sema::FinalizeDeclaration(clang::Decl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e267d1)
#<!-- -->12 0x0000000006a6ff36 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a6ff36)
#<!-- -->13 0x0000000006a7dc7a clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a7dc7a)
#<!-- -->14 0x0000000006a3b32b clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a3b32b)
#<!-- -->15 0x0000000006a3bb1f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a3bb1f)
#<!-- -->16 0x0000000006a4394a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a4394a)
#<!-- -->17 0x0000000006a448e5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a448e5)
#<!-- -->18 0x0000000006a2596a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a2596a)
#<!-- -->19 0x0000000004c64298 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c64298)
#<!-- -->20 0x0000000004f5a915 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f5a915)
#<!-- -->21 0x0000000004edabae clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4edabae)
#<!-- -->22 0x0000000005054c9d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5054c9d)
#<!-- -->23 0x0000000000de672c cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde672c)
#<!-- -->24 0x0000000000ddd17a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->25 0x0000000000ddd2fd int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->26 0x0000000004cd6969 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->27 0x00000000042451f4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42451f4)
#<!-- -->28 0x0000000004cd6f7f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->29 0x0000000004c97a62 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c97a62)
#<!-- -->30 0x0000000004c98a0e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c98a0e)
#<!-- -->31 0x0000000004c9fe65 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c9fe65)
#<!-- -->32 0x0000000000de2b21 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde2b21)
#<!-- -->33 0x0000000000c8d454 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc8d454)
#<!-- -->34 0x00007f0437e29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->35 0x00007f0437e29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->36 0x0000000000ddcc15 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xddcc15)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

### Comment 2 - k-arrows

Crash goes back to clang-3.7:
https://godbolt.org/z/x35z1TjnT

---

### Comment 3 - shafik

Very similar crashes but goes back further: https://github.com/llvm/llvm-project/issues/45893 and https://github.com/llvm/llvm-project/issues/22228 

---

### Comment 4 - k-arrows

> Very similar crashes but goes back further: [#45893](https://github.com/llvm/llvm-project/issues/45893) and [#22228](https://github.com/llvm/llvm-project/issues/22228)

Confirmed. I do try to search for existing issues using the assertion message to avoid duplicates, but it seems I sometimes miss them. There’s no problem with closing this issue as duplicate.

---

### Comment 5 - shafik

> > Very similar crashes but goes back further: [#45893](https://github.com/llvm/llvm-project/issues/45893) and [#22228](https://github.com/llvm/llvm-project/issues/22228)
> 
> Confirmed. I do try to search for existing issues using the assertion message to avoid duplicates, but it seems I sometimes miss them. There’s no problem with closing this issue as duplicate.

It is hard to tell if it is really a dup or not, if they all went back to the same version I likely would have closed it but I have doubts.

---

