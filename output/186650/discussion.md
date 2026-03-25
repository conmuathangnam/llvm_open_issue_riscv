# Assertion failure `isa<To>(Val) && "cast<Ty>() argument of incompatible type!"' while parsing invalid ref-qualified constructor with delegating initializer

**Author:** YuchangSu
**URL:** https://github.com/llvm/llvm-project/issues/186650

## Body

Code:
```CPP
struct a {
  a() && : a{} {
```

Assertion:
```text
clang++: /root/llvm-project/llvm/include/llvm/Support/Casting.h:572: decltype(auto) llvm::cast(From*) [with To = clang::CXXConstructExpr; From = clang::Expr]: Assertion `isa<To>(Val) && "cast<Ty>() argument of incompatible type!"' failed.
```

Backtrace:
```text
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<source>:2:16: current parser token '{'
2.	<source>:1:1: parsing struct/union/class body 'a'
 #0 0x0000000004443e38 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4443e38)
 #1 0x0000000004440c64 llvm::sys::RunSignalHandlers() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4440c64)
 #2 0x0000000004441284 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4441284)
 #3 0x00000000043805a8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #4 0x00007622b4242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #5 0x00007622b42969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #6 0x00007622b4242476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #7 0x00007622b42287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #8 0x00007622b422871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #9 0x00007622b4239e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
#10 0x000000000708c60f clang::Sema::BuildDelegatingInitializer(clang::TypeSourceInfo*, clang::Expr*, clang::CXXRecordDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x708c60f)
#11 0x0000000007096ca1 clang::Sema::BuildBaseInitializer(clang::QualType, clang::TypeSourceInfo*, clang::Expr*, clang::CXXRecordDecl*, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7096ca1)
#12 0x00000000070a48cd clang::Sema::BuildMemInitializer(clang::Decl*, clang::Scope*, clang::CXXScopeSpec&, clang::IdentifierInfo*, clang::OpaquePtr<clang::QualType>, clang::DeclSpec const&, clang::SourceLocation, clang::Expr*, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x70a48cd)
#13 0x0000000006c7fd07 clang::Parser::ParseMemInitializer(clang::Decl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c7fd07)
#14 0x0000000006c81c85 clang::Parser::ParseConstructorInitializer(clang::Decl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c81c85)
#15 0x0000000006c3ee68 clang::Parser::ParseLexedMethodDef(clang::Parser::LexedMethod&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c3ee68)
#16 0x0000000006c3e9cd clang::Parser::ParseLexedMethodDefs(clang::Parser::ParsingClass&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c3e9cd)
#17 0x0000000006c92c76 clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&, unsigned int, clang::Decl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c92c76)
#18 0x0000000006c953d3 clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c953d3)
#19 0x0000000006c684e6 clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c684e6)
#20 0x0000000006c23977 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c23977)
#21 0x0000000006c243ff clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c243ff)
#22 0x0000000006c2ba21 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c2ba21)
#23 0x0000000006c2c955 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c2c955)
#24 0x0000000006c2cd40 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c2cd40)
#25 0x0000000006c0dfe3 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c0dfe3)
#26 0x0000000004f1e888 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f1e888)
#27 0x0000000005228445 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5228445)
#28 0x00000000051a6b8e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x51a6b8e)
#29 0x0000000005324dbd clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5324dbd)
#30 0x0000000000de5fc2 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde5fc2)
#31 0x0000000000ddc8fa ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#32 0x0000000000ddca7d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#33 0x0000000004f94a49 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#34 0x00000000043809e4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x43809e4)
#35 0x0000000004f95076 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#36 0x0000000004f54222 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f54222)
#37 0x0000000004f551ce clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f551ce)
#38 0x0000000004f5cf15 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f5cf15)
#39 0x0000000000de2324 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde2324)
#40 0x0000000000c996ba main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc996ba)
#41 0x00007622b4229d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#42 0x00007622b4229e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#43 0x0000000000ddc395 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xddc395)
```

Goes back to Clang-6.0

See Compiler Explorer: [https://godbolt.org/z/EPP8vdhGe](https://godbolt.org/z/EPP8vdhGe)

The test case was generated by a fuzzer.


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (YuchangSu)

<details>
Code:
```CPP
struct a {
  a() &amp;&amp; : a{} {
```

Assertion:
```text
clang++: /root/llvm-project/llvm/include/llvm/Support/Casting.h:572: decltype(auto) llvm::cast(From*) [with To = clang::CXXConstructExpr; From = clang::Expr]: Assertion `isa&lt;To&gt;(Val) &amp;&amp; "cast&lt;Ty&gt;() argument of incompatible type!"' failed.
```

Backtrace:
```text
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics &lt;source&gt;
1.	&lt;source&gt;:2:16: current parser token '{'
2.	&lt;source&gt;:1:1: parsing struct/union/class body 'a'
 #<!-- -->0 0x0000000004443e38 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4443e38)
 #<!-- -->1 0x0000000004440c64 llvm::sys::RunSignalHandlers() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4440c64)
 #<!-- -->2 0x0000000004441284 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4441284)
 #<!-- -->3 0x00000000043805a8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->4 0x00007622b4242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->5 0x00007622b42969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->6 0x00007622b4242476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->7 0x00007622b42287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->8 0x00007622b422871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->9 0x00007622b4239e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
#<!-- -->10 0x000000000708c60f clang::Sema::BuildDelegatingInitializer(clang::TypeSourceInfo*, clang::Expr*, clang::CXXRecordDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x708c60f)
#<!-- -->11 0x0000000007096ca1 clang::Sema::BuildBaseInitializer(clang::QualType, clang::TypeSourceInfo*, clang::Expr*, clang::CXXRecordDecl*, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7096ca1)
#<!-- -->12 0x00000000070a48cd clang::Sema::BuildMemInitializer(clang::Decl*, clang::Scope*, clang::CXXScopeSpec&amp;, clang::IdentifierInfo*, clang::OpaquePtr&lt;clang::QualType&gt;, clang::DeclSpec const&amp;, clang::SourceLocation, clang::Expr*, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x70a48cd)
#<!-- -->13 0x0000000006c7fd07 clang::Parser::ParseMemInitializer(clang::Decl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c7fd07)
#<!-- -->14 0x0000000006c81c85 clang::Parser::ParseConstructorInitializer(clang::Decl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c81c85)
#<!-- -->15 0x0000000006c3ee68 clang::Parser::ParseLexedMethodDef(clang::Parser::LexedMethod&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c3ee68)
#<!-- -->16 0x0000000006c3e9cd clang::Parser::ParseLexedMethodDefs(clang::Parser::ParsingClass&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c3e9cd)
#<!-- -->17 0x0000000006c92c76 clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&amp;, unsigned int, clang::Decl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c92c76)
#<!-- -->18 0x0000000006c953d3 clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c953d3)
#<!-- -->19 0x0000000006c684e6 clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c684e6)
#<!-- -->20 0x0000000006c23977 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c23977)
#<!-- -->21 0x0000000006c243ff clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c243ff)
#<!-- -->22 0x0000000006c2ba21 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c2ba21)
#<!-- -->23 0x0000000006c2c955 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c2c955)
#<!-- -->24 0x0000000006c2cd40 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c2cd40)
#<!-- -->25 0x0000000006c0dfe3 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c0dfe3)
#<!-- -->26 0x0000000004f1e888 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f1e888)
#<!-- -->27 0x0000000005228445 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5228445)
#<!-- -->28 0x00000000051a6b8e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x51a6b8e)
#<!-- -->29 0x0000000005324dbd clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5324dbd)
#<!-- -->30 0x0000000000de5fc2 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde5fc2)
#<!-- -->31 0x0000000000ddc8fa ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->32 0x0000000000ddca7d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->33 0x0000000004f94a49 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->34 0x00000000043809e4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x43809e4)
#<!-- -->35 0x0000000004f95076 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->36 0x0000000004f54222 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f54222)
#<!-- -->37 0x0000000004f551ce clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f551ce)
#<!-- -->38 0x0000000004f5cf15 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f5cf15)
#<!-- -->39 0x0000000000de2324 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde2324)
#<!-- -->40 0x0000000000c996ba main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc996ba)
#<!-- -->41 0x00007622b4229d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->42 0x00007622b4229e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->43 0x0000000000ddc395 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xddc395)
```

Goes back to Clang-6.0

See Compiler Explorer: [https://godbolt.org/z/EPP8vdhGe](https://godbolt.org/z/EPP8vdhGe)

The test case was generated by a fuzzer.

</details>


---

