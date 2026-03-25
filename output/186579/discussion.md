# [Clang] Assertion failure in Sema::LookupTemplateName on invalid expression with trailing return type and template keyword

**Author:** YuchangSu
**URL:** https://github.com/llvm/llvm-project/issues/186579

## Body

Code:
```CPP
a(     (enum b )b {       }
         -> template c
```

Assertion failure:
```text
clang++: /root/llvm-project/llvm/tools/clang/lib/Sema/SemaTemplate.cpp:409: bool clang::Sema::LookupTemplateName(clang::LookupResult&, clang::Scope*, clang::CXXScopeSpec&, clang::QualType, bool, clang::Sema::RequiredTemplateKind, clang::Sema::AssumedTemplateKind*, bool): Assertion `(IsDependent || !ObjectType->isIncompleteType() || !ObjectType->getAs<TagType>() || ObjectType->castAs<TagType>()->getDecl()->isEntityBeingDefined()) && "Caller should have completed object type"' failed.
```

Backtrace:
```text
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<eof> parser at end of file
 #0 0x0000000004445bb8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4445bb8)
 #1 0x00000000044429e4 llvm::sys::RunSignalHandlers() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x44429e4)
 #2 0x0000000004443004 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4443004)
 #3 0x0000000004382328 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #4 0x0000731ad6e42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #5 0x0000731ad6e969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #6 0x0000731ad6e42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #7 0x0000731ad6e287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #8 0x0000731ad6e2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #9 0x0000731ad6e39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
#10 0x000000000763e98f clang::Sema::LookupTemplateName(clang::LookupResult&, clang::Scope*, clang::CXXScopeSpec&, clang::QualType, bool, clang::Sema::RequiredTemplateKind, clang::Sema::AssumedTemplateKind*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x763e98f)
#11 0x000000000763feac clang::Sema::isTemplateName(clang::Scope*, clang::CXXScopeSpec&, bool, clang::UnqualifiedId const&, clang::OpaquePtr<clang::QualType>, bool, clang::OpaquePtr<clang::TemplateName>&, bool&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x763feac)
#12 0x00000000076404f9 clang::Sema::ActOnTemplateName(clang::Scope*, clang::CXXScopeSpec&, clang::SourceLocation, clang::UnqualifiedId const&, clang::OpaquePtr<clang::QualType>, bool, clang::OpaquePtr<clang::TemplateName>&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x76404f9)
#13 0x0000000006cbb0b1 clang::Parser::ParseUnqualifiedId(clang::CXXScopeSpec&, clang::OpaquePtr<clang::QualType>, bool, bool, bool, bool, bool, clang::SourceLocation*, clang::UnqualifiedId&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6cbb0b1)
#14 0x0000000006ca7034 clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult<clang::Expr*, true>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ca7034)
#15 0x0000000006c9fd2a clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c9fd2a)
#16 0x0000000006c9ff9e clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c9ff9e)
#17 0x0000000006ca1fc7 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ca1fc7)
#18 0x0000000006caae18 clang::Parser::ParseParenExpression(clang::ParenParseOption&, bool, clang::ParenExprKind, clang::TypoCorrectionTypeBehavior, clang::OpaquePtr<clang::QualType>&, clang::SourceLocation&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6caae18)
#19 0x0000000006ca0de0 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ca0de0)
#20 0x0000000006ca1fc7 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ca1fc7)
#21 0x0000000006ca2059 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ca2059)
#22 0x0000000006ca23be clang::Parser::ParseExpressionList(llvm::SmallVectorImpl<clang::Expr*>&, llvm::function_ref<void ()>, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ca23be)
#23 0x0000000006c5b6da clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c5b6da)
#24 0x0000000006c691aa clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c691aa)
#25 0x0000000006c26efb clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c26efb)
#26 0x0000000006c276ff clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c276ff)
#27 0x0000000006c2ed21 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c2ed21)
#28 0x0000000006c2fc55 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c2fc55)
#29 0x0000000006c30040 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c30040)
#30 0x0000000006c112e3 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c112e3)
#31 0x0000000004f21698 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f21698)
#32 0x000000000522a7a5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x522a7a5)
#33 0x00000000051a8eee clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x51a8eee)
#34 0x000000000532711d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x532711d)
#35 0x0000000000de5fe2 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde5fe2)
#36 0x0000000000ddc91a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#37 0x0000000000ddca9d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#38 0x0000000004f96da9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#39 0x0000000004382764 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4382764)
#40 0x0000000004f973d6 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#41 0x0000000004f57032 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f57032)
#42 0x0000000004f57fde clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f57fde)
#43 0x0000000004f5fd25 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f5fd25)
#44 0x0000000000de2344 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde2344)
#45 0x0000000000c997ea main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc997ea)
#46 0x0000731ad6e29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#47 0x0000731ad6e29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#48 0x0000000000ddc3b5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xddc3b5)
```

Goes back to Clang-12

See Compiler Explorer: [https://godbolt.org/z/9boocqjY8](https://godbolt.org/z/9boocqjY8)

The test case was generated by a fuzzer.


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (YuchangSu)

<details>
Code:
```CPP
a(     (enum b )b {       }
         -&gt; template c
```

Assertion failure:
```text
clang++: /root/llvm-project/llvm/tools/clang/lib/Sema/SemaTemplate.cpp:409: bool clang::Sema::LookupTemplateName(clang::LookupResult&amp;, clang::Scope*, clang::CXXScopeSpec&amp;, clang::QualType, bool, clang::Sema::RequiredTemplateKind, clang::Sema::AssumedTemplateKind*, bool): Assertion `(IsDependent || !ObjectType-&gt;isIncompleteType() || !ObjectType-&gt;getAs&lt;TagType&gt;() || ObjectType-&gt;castAs&lt;TagType&gt;()-&gt;getDecl()-&gt;isEntityBeingDefined()) &amp;&amp; "Caller should have completed object type"' failed.
```

Backtrace:
```text
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
 #<!-- -->0 0x0000000004445bb8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4445bb8)
 #<!-- -->1 0x00000000044429e4 llvm::sys::RunSignalHandlers() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x44429e4)
 #<!-- -->2 0x0000000004443004 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4443004)
 #<!-- -->3 0x0000000004382328 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->4 0x0000731ad6e42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->5 0x0000731ad6e969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->6 0x0000731ad6e42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->7 0x0000731ad6e287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->8 0x0000731ad6e2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->9 0x0000731ad6e39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
#<!-- -->10 0x000000000763e98f clang::Sema::LookupTemplateName(clang::LookupResult&amp;, clang::Scope*, clang::CXXScopeSpec&amp;, clang::QualType, bool, clang::Sema::RequiredTemplateKind, clang::Sema::AssumedTemplateKind*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x763e98f)
#<!-- -->11 0x000000000763feac clang::Sema::isTemplateName(clang::Scope*, clang::CXXScopeSpec&amp;, bool, clang::UnqualifiedId const&amp;, clang::OpaquePtr&lt;clang::QualType&gt;, bool, clang::OpaquePtr&lt;clang::TemplateName&gt;&amp;, bool&amp;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x763feac)
#<!-- -->12 0x00000000076404f9 clang::Sema::ActOnTemplateName(clang::Scope*, clang::CXXScopeSpec&amp;, clang::SourceLocation, clang::UnqualifiedId const&amp;, clang::OpaquePtr&lt;clang::QualType&gt;, bool, clang::OpaquePtr&lt;clang::TemplateName&gt;&amp;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x76404f9)
#<!-- -->13 0x0000000006cbb0b1 clang::Parser::ParseUnqualifiedId(clang::CXXScopeSpec&amp;, clang::OpaquePtr&lt;clang::QualType&gt;, bool, bool, bool, bool, bool, clang::SourceLocation*, clang::UnqualifiedId&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6cbb0b1)
#<!-- -->14 0x0000000006ca7034 clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult&lt;clang::Expr*, true&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ca7034)
#<!-- -->15 0x0000000006c9fd2a clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&amp;, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c9fd2a)
#<!-- -->16 0x0000000006c9ff9e clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&amp;, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c9ff9e)
#<!-- -->17 0x0000000006ca1fc7 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ca1fc7)
#<!-- -->18 0x0000000006caae18 clang::Parser::ParseParenExpression(clang::ParenParseOption&amp;, bool, clang::ParenExprKind, clang::TypoCorrectionTypeBehavior, clang::OpaquePtr&lt;clang::QualType&gt;&amp;, clang::SourceLocation&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6caae18)
#<!-- -->19 0x0000000006ca0de0 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&amp;, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ca0de0)
#<!-- -->20 0x0000000006ca1fc7 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ca1fc7)
#<!-- -->21 0x0000000006ca2059 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ca2059)
#<!-- -->22 0x0000000006ca23be clang::Parser::ParseExpressionList(llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, llvm::function_ref&lt;void ()&gt;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ca23be)
#<!-- -->23 0x0000000006c5b6da clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c5b6da)
#<!-- -->24 0x0000000006c691aa clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c691aa)
#<!-- -->25 0x0000000006c26efb clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c26efb)
#<!-- -->26 0x0000000006c276ff clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c276ff)
#<!-- -->27 0x0000000006c2ed21 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c2ed21)
#<!-- -->28 0x0000000006c2fc55 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c2fc55)
#<!-- -->29 0x0000000006c30040 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c30040)
#<!-- -->30 0x0000000006c112e3 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c112e3)
#<!-- -->31 0x0000000004f21698 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f21698)
#<!-- -->32 0x000000000522a7a5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x522a7a5)
#<!-- -->33 0x00000000051a8eee clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x51a8eee)
#<!-- -->34 0x000000000532711d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x532711d)
#<!-- -->35 0x0000000000de5fe2 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde5fe2)
#<!-- -->36 0x0000000000ddc91a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->37 0x0000000000ddca9d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->38 0x0000000004f96da9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->39 0x0000000004382764 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4382764)
#<!-- -->40 0x0000000004f973d6 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->41 0x0000000004f57032 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f57032)
#<!-- -->42 0x0000000004f57fde clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f57fde)
#<!-- -->43 0x0000000004f5fd25 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f5fd25)
#<!-- -->44 0x0000000000de2344 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde2344)
#<!-- -->45 0x0000000000c997ea main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc997ea)
#<!-- -->46 0x0000731ad6e29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->47 0x0000731ad6e29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->48 0x0000000000ddc3b5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xddc3b5)
```

Goes back to Clang-12

See Compiler Explorer: [https://godbolt.org/z/9boocqjY8](https://godbolt.org/z/9boocqjY8)

The test case was generated by a fuzzer.

</details>


---

### Comment 2 - shafik

Please focus on issues in the last five releases.

---

