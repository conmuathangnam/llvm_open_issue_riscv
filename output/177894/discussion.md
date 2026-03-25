# [clang] Assertion `Dtor && "class has literal fields and bases but no dtor?"' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/177894

## Body

Reproducer:
https://godbolt.org/z/Ta964cMjW
```cpp
struct S {
  ~S() = (0);
};

constexpr void foo() { S s; }
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/Sema/SemaType.cpp:9682: bool clang::Sema::RequireLiteralType(clang::SourceLocation, clang::QualType, clang::Sema::TypeDiagnoser&): Assertion `Dtor && "class has literal fields and bases but no dtor?"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<eof> parser at end of file
2.	<source>:5:22: parsing function body 'foo'
 #0 0x000000000430ae68 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x430ae68)
 #1 0x00000000043082c4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x43082c4)
 #2 0x000000000424a088 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000075d824a42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x000075d824a969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x000075d824a42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x000075d824a287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x000075d824a2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x000075d824a39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x00000000076d0804 (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x76d0804)
#10 0x0000000006ecc2cd bool CheckLiteralType<bool>(clang::Sema&, clang::Sema::CheckConstexprKind, clang::SourceLocation, clang::QualType, unsigned int, bool&&) SemaDeclCXX.cpp:0:0
#11 0x0000000006ee5daf CheckConstexprFunctionStmt(clang::Sema&, clang::FunctionDecl const*, clang::Stmt*, llvm::SmallVectorImpl<clang::SourceLocation>&, clang::SourceLocation&, clang::SourceLocation&, clang::SourceLocation&, clang::Sema::CheckConstexprKind) SemaDeclCXX.cpp:0:0
#12 0x0000000006ee642a clang::Sema::CheckConstexprFunctionDefinition(clang::FunctionDecl const*, clang::Sema::CheckConstexprKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ee642a)
#13 0x0000000006e6b58b clang::Sema::ActOnFinishFunctionBody(clang::Decl*, clang::Stmt*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e6b58b)
#14 0x0000000006b87a24 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b87a24)
#15 0x0000000006a8d22b clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a8d22b)
#16 0x0000000006ac369d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ac369d)
#17 0x0000000006a802fb clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a802fb)
#18 0x0000000006a80aef clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a80aef)
#19 0x0000000006a88af1 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a88af1)
#20 0x0000000006a89a05 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a89a05)
#21 0x0000000006a6a71a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a6a71a)
#22 0x0000000004c74558 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c74558)
#23 0x0000000004f6f6b5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f6f6b5)
#24 0x0000000004eef0de clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4eef0de)
#25 0x000000000506a87d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x506a87d)
#26 0x0000000000dde12e cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdde12e)
#27 0x0000000000dd4afa ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#28 0x0000000000dd4c7d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#29 0x0000000004ce7f99 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#30 0x000000000424a524 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x424a524)
#31 0x0000000004ce85af clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#32 0x0000000004ca8f82 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ca8f82)
#33 0x0000000004ca9f2e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ca9f2e)
#34 0x0000000004cb1375 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4cb1375)
#35 0x0000000000dda4a1 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdda4a1)
#36 0x0000000000c974f4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc974f4)
#37 0x000075d824a29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#38 0x000075d824a29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#39 0x0000000000dd4595 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdd4595)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/Ta964cMjW
```cpp
struct S {
  ~S() = (0);
};

constexpr void foo() { S s; }
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/Sema/SemaType.cpp:9682: bool clang::Sema::RequireLiteralType(clang::SourceLocation, clang::QualType, clang::Sema::TypeDiagnoser&amp;): Assertion `Dtor &amp;&amp; "class has literal fields and bases but no dtor?"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	&lt;source&gt;:5:22: parsing function body 'foo'
 #<!-- -->0 0x000000000430ae68 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x430ae68)
 #<!-- -->1 0x00000000043082c4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x43082c4)
 #<!-- -->2 0x000000000424a088 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x000075d824a42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x000075d824a969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x000075d824a42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x000075d824a287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x000075d824a2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x000075d824a39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x00000000076d0804 (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x76d0804)
#<!-- -->10 0x0000000006ecc2cd bool CheckLiteralType&lt;bool&gt;(clang::Sema&amp;, clang::Sema::CheckConstexprKind, clang::SourceLocation, clang::QualType, unsigned int, bool&amp;&amp;) SemaDeclCXX.cpp:0:0
#<!-- -->11 0x0000000006ee5daf CheckConstexprFunctionStmt(clang::Sema&amp;, clang::FunctionDecl const*, clang::Stmt*, llvm::SmallVectorImpl&lt;clang::SourceLocation&gt;&amp;, clang::SourceLocation&amp;, clang::SourceLocation&amp;, clang::SourceLocation&amp;, clang::Sema::CheckConstexprKind) SemaDeclCXX.cpp:0:0
#<!-- -->12 0x0000000006ee642a clang::Sema::CheckConstexprFunctionDefinition(clang::FunctionDecl const*, clang::Sema::CheckConstexprKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ee642a)
#<!-- -->13 0x0000000006e6b58b clang::Sema::ActOnFinishFunctionBody(clang::Decl*, clang::Stmt*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e6b58b)
#<!-- -->14 0x0000000006b87a24 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b87a24)
#<!-- -->15 0x0000000006a8d22b clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a8d22b)
#<!-- -->16 0x0000000006ac369d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ac369d)
#<!-- -->17 0x0000000006a802fb clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a802fb)
#<!-- -->18 0x0000000006a80aef clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a80aef)
#<!-- -->19 0x0000000006a88af1 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a88af1)
#<!-- -->20 0x0000000006a89a05 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a89a05)
#<!-- -->21 0x0000000006a6a71a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a6a71a)
#<!-- -->22 0x0000000004c74558 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c74558)
#<!-- -->23 0x0000000004f6f6b5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f6f6b5)
#<!-- -->24 0x0000000004eef0de clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4eef0de)
#<!-- -->25 0x000000000506a87d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x506a87d)
#<!-- -->26 0x0000000000dde12e cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdde12e)
#<!-- -->27 0x0000000000dd4afa ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->28 0x0000000000dd4c7d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->29 0x0000000004ce7f99 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->30 0x000000000424a524 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x424a524)
#<!-- -->31 0x0000000004ce85af clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->32 0x0000000004ca8f82 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ca8f82)
#<!-- -->33 0x0000000004ca9f2e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ca9f2e)
#<!-- -->34 0x0000000004cb1375 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4cb1375)
#<!-- -->35 0x0000000000dda4a1 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdda4a1)
#<!-- -->36 0x0000000000c974f4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc974f4)
#<!-- -->37 0x000075d824a29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->38 0x000075d824a29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->39 0x0000000000dd4595 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdd4595)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

### Comment 2 - k-arrows

Goes back to clang-15:
https://godbolt.org/z/7aM5zs8dc

---

### Comment 3 - k-arrows

The above reproducer is available at:
https://github.com/k-arrows/clang-assert-tests/blob/main/cases/00113.cpp

---

