# [Clang] Assertion failure in Preprocessor::AnnotatePreviousCachedTokens on invalid use of ::template operator

**Author:** YuchangSu
**URL:** https://github.com/llvm/llvm-project/issues/186582

## Body

Code:
```CPP
a(   ::template operator
```

Assertion failure:
```text
clang++: /root/llvm-project/llvm/tools/clang/lib/Lex/PPCaching.cpp:151: void clang::Preprocessor::AnnotatePreviousCachedTokens(const clang::Token&): Assertion `CachedTokens[CachedLexPos-1].getLastLoc() == Tok.getAnnotationEndLoc() && "The annotation should be until the most recent cached token"' failed.
```

Backtrace:
```text
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<source>:1:5: at annotation token
 #0 0x0000000004445bb8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4445bb8)
 #1 0x00000000044429e4 llvm::sys::RunSignalHandlers() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x44429e4)
 #2 0x0000000004443004 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4443004)
 #3 0x0000000004382328 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #4 0x00007f1ec2642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #5 0x00007f1ec26969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #6 0x00007f1ec2642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #7 0x00007f1ec26287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #8 0x00007f1ec262871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #9 0x00007f1ec2639e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
#10 0x00000000086a389d clang::Preprocessor::AnnotatePreviousCachedTokens(clang::Token const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x86a389d)
#11 0x0000000006c266b6 clang::Parser::TryAnnotateCXXScopeToken(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c266b6)
#12 0x0000000006d48acd clang::Parser::TryParsePtrOperatorSeq() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d48acd)
#13 0x0000000006d4dcd9 clang::Parser::TryParseDeclarator(bool, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d4dcd9)
#14 0x0000000006d4e27c clang::Parser::TryParseDeclarator(bool, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d4e27c)
#15 0x0000000006c70ef6 clang::Parser::ParseImplicitInt(clang::DeclSpec&, clang::CXXScopeSpec*, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c70ef6)
#16 0x0000000006c6e03b clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c6e03b)
#17 0x0000000006c26c77 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c26c77)
#18 0x0000000006c276ff clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c276ff)
#19 0x0000000006c2ed21 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c2ed21)
#20 0x0000000006c2fc55 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c2fc55)
#21 0x0000000006c30040 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c30040)
#22 0x0000000006c112e3 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c112e3)
#23 0x0000000004f21698 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f21698)
#24 0x000000000522a7a5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x522a7a5)
#25 0x00000000051a8eee clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x51a8eee)
#26 0x000000000532711d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x532711d)
#27 0x0000000000de5fe2 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde5fe2)
#28 0x0000000000ddc91a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#29 0x0000000000ddca9d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#30 0x0000000004f96da9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#31 0x0000000004382764 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4382764)
#32 0x0000000004f973d6 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#33 0x0000000004f57032 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f57032)
#34 0x0000000004f57fde clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f57fde)
#35 0x0000000004f5fd25 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f5fd25)
#36 0x0000000000de2344 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde2344)
#37 0x0000000000c997ea main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc997ea)
#38 0x00007f1ec2629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#39 0x00007f1ec2629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#40 0x0000000000ddc3b5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xddc3b5)
```

Goes back to Clang-2.7 [https://godbolt.org/z/1aoGfoYvq](https://godbolt.org/z/1aoGfoYvq)

See Compiler Explorer: [https://godbolt.org/z/dcjTex6f6](https://godbolt.org/z/dcjTex6f6)

The test case was generated by a fuzzer.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (YuchangSu)

<details>
Code:
```CPP
a(   ::template operator
```

Assertion failure:
```text
clang++: /root/llvm-project/llvm/tools/clang/lib/Lex/PPCaching.cpp:151: void clang::Preprocessor::AnnotatePreviousCachedTokens(const clang::Token&amp;): Assertion `CachedTokens[CachedLexPos-1].getLastLoc() == Tok.getAnnotationEndLoc() &amp;&amp; "The annotation should be until the most recent cached token"' failed.
```

Backtrace:
```text
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics &lt;source&gt;
1.	&lt;source&gt;:1:5: at annotation token
 #<!-- -->0 0x0000000004445bb8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4445bb8)
 #<!-- -->1 0x00000000044429e4 llvm::sys::RunSignalHandlers() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x44429e4)
 #<!-- -->2 0x0000000004443004 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4443004)
 #<!-- -->3 0x0000000004382328 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->4 0x00007f1ec2642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->5 0x00007f1ec26969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->6 0x00007f1ec2642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->7 0x00007f1ec26287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->8 0x00007f1ec262871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->9 0x00007f1ec2639e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
#<!-- -->10 0x00000000086a389d clang::Preprocessor::AnnotatePreviousCachedTokens(clang::Token const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x86a389d)
#<!-- -->11 0x0000000006c266b6 clang::Parser::TryAnnotateCXXScopeToken(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c266b6)
#<!-- -->12 0x0000000006d48acd clang::Parser::TryParsePtrOperatorSeq() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d48acd)
#<!-- -->13 0x0000000006d4dcd9 clang::Parser::TryParseDeclarator(bool, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d4dcd9)
#<!-- -->14 0x0000000006d4e27c clang::Parser::TryParseDeclarator(bool, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d4e27c)
#<!-- -->15 0x0000000006c70ef6 clang::Parser::ParseImplicitInt(clang::DeclSpec&amp;, clang::CXXScopeSpec*, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c70ef6)
#<!-- -->16 0x0000000006c6e03b clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c6e03b)
#<!-- -->17 0x0000000006c26c77 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c26c77)
#<!-- -->18 0x0000000006c276ff clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c276ff)
#<!-- -->19 0x0000000006c2ed21 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c2ed21)
#<!-- -->20 0x0000000006c2fc55 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c2fc55)
#<!-- -->21 0x0000000006c30040 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c30040)
#<!-- -->22 0x0000000006c112e3 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c112e3)
#<!-- -->23 0x0000000004f21698 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f21698)
#<!-- -->24 0x000000000522a7a5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x522a7a5)
#<!-- -->25 0x00000000051a8eee clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x51a8eee)
#<!-- -->26 0x000000000532711d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x532711d)
#<!-- -->27 0x0000000000de5fe2 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde5fe2)
#<!-- -->28 0x0000000000ddc91a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->29 0x0000000000ddca9d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->30 0x0000000004f96da9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->31 0x0000000004382764 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4382764)
#<!-- -->32 0x0000000004f973d6 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->33 0x0000000004f57032 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f57032)
#<!-- -->34 0x0000000004f57fde clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f57fde)
#<!-- -->35 0x0000000004f5fd25 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f5fd25)
#<!-- -->36 0x0000000000de2344 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde2344)
#<!-- -->37 0x0000000000c997ea main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc997ea)
#<!-- -->38 0x00007f1ec2629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->39 0x00007f1ec2629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->40 0x0000000000ddc3b5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xddc3b5)
```

Goes back to Clang-2.7 [https://godbolt.org/z/1aoGfoYvq](https://godbolt.org/z/1aoGfoYvq)

See Compiler Explorer: [https://godbolt.org/z/dcjTex6f6](https://godbolt.org/z/dcjTex6f6)

The test case was generated by a fuzzer.
</details>


---

