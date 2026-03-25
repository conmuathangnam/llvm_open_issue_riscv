# [clang] Assertion `Ancestor->getEntity() == CurContext && "ancestor context mismatch"' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/174130

## Body

Reproducer:
https://godbolt.org/z/rc7M1vf3T
```cpp
a = [](::
```

The above reproducer is available at:
https://github.com/k-arrows/clang-assert-tests/blob/main/cases/00012.cpp

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/Sema/SemaDecl.cpp:1424: void clang::Sema::EnterDeclaratorContext(clang::Scope*, clang::DeclContext*): Assertion `Ancestor->getEntity() == CurContext && "ancestor context mismatch"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<eof> parser at end of file
2.	<source>:1:5: lambda expression parsing
 #0 0x0000000004302c88 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4302c88)
 #1 0x00000000043000b4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x43000b4)
 #2 0x0000000004244ab8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007826c5642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007826c56969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007826c5642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007826c56287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007826c562871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007826c5639e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000006dbd94a clang::Sema::EnterDeclaratorContext(clang::Scope*, clang::DeclContext*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6dbd94a)
#10 0x0000000006c4a320 clang::Sema::ActOnCXXEnterDeclaratorScope(clang::Scope*, clang::CXXScopeSpec&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c4a320)
#11 0x0000000006a6572a clang::Parser::DeclaratorScopeObj::EnterDeclaratorScope() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a6572a)
#12 0x0000000006a8ddb0 clang::Parser::ParseDirectDeclarator(clang::Declarator&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a8ddb0)
#13 0x0000000006a7a70e clang::Parser::ParseDeclaratorInternal(clang::Declarator&, void (clang::Parser::*)(clang::Declarator&)) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a7a70e)
#14 0x0000000008514a41 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x8514a41)
#15 0x0000000006a87354 clang::Parser::ParseParameterDeclarationClause(clang::DeclaratorContext, clang::ParsedAttributes&, llvm::SmallVectorImpl<clang::DeclaratorChunk::ParamInfo>&, clang::SourceLocation&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a87354)
#16 0x0000000006ad5a76 clang::Parser::ParseLambdaExpressionAfterIntroducer(clang::LambdaIntroducer&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ad5a76)
#17 0x0000000006ad621b clang::Parser::ParseLambdaExpression() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ad621b)
#18 0x0000000006ab67f5 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ab67f5)
#19 0x0000000006ab7217 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ab7217)
#20 0x0000000006ab72a9 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ab72a9)
#21 0x0000000006adf790 clang::Parser::ParseInitializer(clang::Decl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6adf790)
#22 0x0000000006a707af clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a707af)
#23 0x0000000006a7db7a clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a7db7a)
#24 0x0000000006a3b22b clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a3b22b)
#25 0x0000000006a3ba1f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a3ba1f)
#26 0x0000000006a4384a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a4384a)
#27 0x0000000006a447e5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a447e5)
#28 0x0000000006a44c90 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a44c90)
#29 0x0000000006a25833 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a25833)
#30 0x0000000004c63ea8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c63ea8)
#31 0x0000000004f5a675 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f5a675)
#32 0x0000000004eda82e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4eda82e)
#33 0x00000000050549fd clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x50549fd)
#34 0x0000000000de66ac cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde66ac)
#35 0x0000000000ddd0fa ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#36 0x0000000000ddd27d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#37 0x0000000004cd6579 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#38 0x0000000004244f54 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4244f54)
#39 0x0000000004cd6b8f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#40 0x0000000004c97672 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c97672)
#41 0x0000000004c9861e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c9861e)
#42 0x0000000004c9fa75 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c9fa75)
#43 0x0000000000de2aa1 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde2aa1)
#44 0x0000000000c8d3d4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc8d3d4)
#45 0x00007826c5629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#46 0x00007826c5629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#47 0x0000000000ddcb95 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xddcb95)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

Goes back to clang-17:
https://godbolt.org/z/Yxqefe37c


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/rc7M1vf3T
```cpp
a = [](::
```

The above reproducer is available at:
https://github.com/k-arrows/clang-assert-tests/blob/main/cases/00012.cpp

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/Sema/SemaDecl.cpp:1424: void clang::Sema::EnterDeclaratorContext(clang::Scope*, clang::DeclContext*): Assertion `Ancestor-&gt;getEntity() == CurContext &amp;&amp; "ancestor context mismatch"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	&lt;source&gt;:1:5: lambda expression parsing
 #<!-- -->0 0x0000000004302c88 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4302c88)
 #<!-- -->1 0x00000000043000b4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x43000b4)
 #<!-- -->2 0x0000000004244ab8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007826c5642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007826c56969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007826c5642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007826c56287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x00007826c562871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x00007826c5639e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000006dbd94a clang::Sema::EnterDeclaratorContext(clang::Scope*, clang::DeclContext*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6dbd94a)
#<!-- -->10 0x0000000006c4a320 clang::Sema::ActOnCXXEnterDeclaratorScope(clang::Scope*, clang::CXXScopeSpec&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c4a320)
#<!-- -->11 0x0000000006a6572a clang::Parser::DeclaratorScopeObj::EnterDeclaratorScope() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a6572a)
#<!-- -->12 0x0000000006a8ddb0 clang::Parser::ParseDirectDeclarator(clang::Declarator&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a8ddb0)
#<!-- -->13 0x0000000006a7a70e clang::Parser::ParseDeclaratorInternal(clang::Declarator&amp;, void (clang::Parser::*)(clang::Declarator&amp;)) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a7a70e)
#<!-- -->14 0x0000000008514a41 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x8514a41)
#<!-- -->15 0x0000000006a87354 clang::Parser::ParseParameterDeclarationClause(clang::DeclaratorContext, clang::ParsedAttributes&amp;, llvm::SmallVectorImpl&lt;clang::DeclaratorChunk::ParamInfo&gt;&amp;, clang::SourceLocation&amp;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a87354)
#<!-- -->16 0x0000000006ad5a76 clang::Parser::ParseLambdaExpressionAfterIntroducer(clang::LambdaIntroducer&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ad5a76)
#<!-- -->17 0x0000000006ad621b clang::Parser::ParseLambdaExpression() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ad621b)
#<!-- -->18 0x0000000006ab67f5 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&amp;, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ab67f5)
#<!-- -->19 0x0000000006ab7217 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ab7217)
#<!-- -->20 0x0000000006ab72a9 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ab72a9)
#<!-- -->21 0x0000000006adf790 clang::Parser::ParseInitializer(clang::Decl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6adf790)
#<!-- -->22 0x0000000006a707af clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a707af)
#<!-- -->23 0x0000000006a7db7a clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a7db7a)
#<!-- -->24 0x0000000006a3b22b clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a3b22b)
#<!-- -->25 0x0000000006a3ba1f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a3ba1f)
#<!-- -->26 0x0000000006a4384a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a4384a)
#<!-- -->27 0x0000000006a447e5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a447e5)
#<!-- -->28 0x0000000006a44c90 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a44c90)
#<!-- -->29 0x0000000006a25833 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a25833)
#<!-- -->30 0x0000000004c63ea8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c63ea8)
#<!-- -->31 0x0000000004f5a675 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f5a675)
#<!-- -->32 0x0000000004eda82e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4eda82e)
#<!-- -->33 0x00000000050549fd clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x50549fd)
#<!-- -->34 0x0000000000de66ac cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde66ac)
#<!-- -->35 0x0000000000ddd0fa ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->36 0x0000000000ddd27d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->37 0x0000000004cd6579 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->38 0x0000000004244f54 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4244f54)
#<!-- -->39 0x0000000004cd6b8f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->40 0x0000000004c97672 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c97672)
#<!-- -->41 0x0000000004c9861e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c9861e)
#<!-- -->42 0x0000000004c9fa75 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c9fa75)
#<!-- -->43 0x0000000000de2aa1 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde2aa1)
#<!-- -->44 0x0000000000c8d3d4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc8d3d4)
#<!-- -->45 0x00007826c5629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->46 0x00007826c5629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->47 0x0000000000ddcb95 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xddcb95)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

Goes back to clang-17:
https://godbolt.org/z/Yxqefe37c

</details>


---

### Comment 2 - shafik

@k-arrows can you also add regressions "Clang Regressions" project too?

---

