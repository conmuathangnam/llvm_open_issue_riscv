# Compiler crash when capturing parameter name in unevaluated context in a lambda capture clause

**Author:** dgodfrey206
**URL:** https://github.com/llvm/llvm-project/issues/176534

## Body

https://godbolt.org/z/YnWr41a56

```cpp
struct Bob {
    void method( int p, int = sizeof([p]{ return p; }) );
};
```

Crash dump:

```
> 0.	Program arguments: /opt/compiler-explorer/clang-21.1.0/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-15.2.0 -fcolor-diagnostics -fno-crash-diagnostics --std=c++23 <source>
> 1.	<source>:2:40: current parser token '{'
> 2.	<source>:1:1: parsing struct/union/class body 'Bob'
> 3.	<source>:2:37: lambda expression parsing
>  #0 0x0000000003bfa6c8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x3bfa6c8)
>  #1 0x0000000003bf809c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x3bf809c)
>  #2 0x0000000003b483f8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
>  #3 0x0000777d2e442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
>  #4 0x00000000066eac10 clang::Sema::tryCaptureVariable(clang::ValueDecl*, clang::SourceLocation, clang::TryCaptureKind, clang::SourceLocation, bool, clang::QualType&, clang::QualType&, unsigned int const*) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x66eac10)
>  #5 0x00000000066ed64f clang::Sema::tryCaptureVariable(clang::ValueDecl*, clang::SourceLocation, clang::TryCaptureKind, clang::SourceLocation) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x66ed64f)
>  #6 0x0000000006863a15 clang::Sema::ActOnLambdaExpressionAfterIntroducer(clang::LambdaIntroducer&, clang::Scope*) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x6863a15)
>  #7 0x00000000062399b7 clang::Parser::ParseLambdaExpressionAfterIntroducer(clang::LambdaIntroducer&) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x62399b7)
>  #8 0x000000000623c42b clang::Parser::ParseLambdaExpression() (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x623c42b)
>  #9 0x000000000621aced clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x621aced)
> #10 0x000000000621b5ba clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x621b5ba)
> #11 0x000000000621b7a9 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x621b7a9)
> #12 0x0000000006220759 clang::Parser::ParseExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x6220759)
> #13 0x0000000006224bf5 clang::Parser::ParseParenExpression(clang::ParenParseOption&, bool, clang::ParenExprKind, clang::TypoCorrectionTypeBehavior, clang::OpaquePtr<clang::QualType>&, clang::SourceLocation&) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x6224bf5)
> #14 0x0000000006225dce clang::Parser::ParseExprAfterUnaryExprOrTypeTrait(clang::Token const&, bool&, clang::OpaquePtr<clang::QualType>&, clang::SourceRange&) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x6225dce)
> #15 0x0000000006226553 clang::Parser::ParseUnaryExprOrTypeTraitExpression() (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x6226553)
> #16 0x000000000621983a clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x621983a)
> #17 0x000000000621b5ba clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x621b5ba)
> #18 0x000000000621b7a9 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x621b7a9)
> #19 0x00000000061c04e0 clang::Parser::ParseLexedMethodDeclaration(clang::Parser::LateParsedMethodDeclaration&) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x61c04e0)
> #20 0x00000000061b644d clang::Parser::ParseLexedMethodDeclarations(clang::Parser::ParsingClass&) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x61b644d)
> #21 0x000000000620ea1d clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&, unsigned int, clang::Decl*) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x620ea1d)
> #22 0x0000000006211113 clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x6211113)
> #23 0x00000000061d9549 clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x61d9549)
> #24 0x0000000006195256 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x6195256)
> #25 0x00000000061963de clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x61963de)
> #26 0x000000000619ef7f clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x619ef7f)
> #27 0x00000000061a07b0 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x61a07b0)
> #28 0x00000000061a0d00 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x61a0d00)
> #29 0x000000000618ef43 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x618ef43)
> #30 0x000000000454985d clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x454985d)
> #31 0x000000000485b83a clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x485b83a)
> #32 0x00000000047d655b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x47d655b)
> #33 0x000000000494e513 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x494e513)
> #34 0x0000000000da5f25 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0xda5f25)
> #35 0x0000000000d9de2d ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
> #36 0x00000000045bb869 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
> #37 0x0000000003b48813 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x3b48813)
> #38 0x00000000045bba89 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
> #39 0x000000000457eddd clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x457eddd)
> #40 0x000000000457fd91 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x457fd91)
> #41 0x000000000458d29c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x458d29c)
> #42 0x0000000000da2751 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0xda2751)
> #43 0x0000000000c526f4 main (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0xc526f4)
> #44 0x0000777d2e429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
> #45 0x0000777d2e429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
> #46 0x0000000000d9d8c5 _start (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0xd9d8c5)
> clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
> Compiler returned: 139
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: David Godfrey (dgodfrey206)

<details>
https://godbolt.org/z/38h8E4b5c

```cpp
struct Bob {
    void method(int p, int = sizeof([p]{
        return p;
    }()));
};
```

Crash dump:

&gt; 0.	Program arguments: /opt/compiler-explorer/clang-21.1.0/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-15.2.0 -fcolor-diagnostics -fno-crash-diagnostics --std=c++23 &lt;source&gt;
&gt; 1.	&lt;source&gt;:2:40: current parser token '{'
&gt; 2.	&lt;source&gt;:1:1: parsing struct/union/class body 'Bob'
&gt; 3.	&lt;source&gt;:2:37: lambda expression parsing
&gt;  #<!-- -->0 0x0000000003bfa6c8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x3bfa6c8)
&gt;  #<!-- -->1 0x0000000003bf809c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x3bf809c)
&gt;  #<!-- -->2 0x0000000003b483f8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
&gt;  #<!-- -->3 0x0000777d2e442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
&gt;  #<!-- -->4 0x00000000066eac10 clang::Sema::tryCaptureVariable(clang::ValueDecl*, clang::SourceLocation, clang::TryCaptureKind, clang::SourceLocation, bool, clang::QualType&amp;, clang::QualType&amp;, unsigned int const*) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x66eac10)
&gt;  #<!-- -->5 0x00000000066ed64f clang::Sema::tryCaptureVariable(clang::ValueDecl*, clang::SourceLocation, clang::TryCaptureKind, clang::SourceLocation) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x66ed64f)
&gt;  #<!-- -->6 0x0000000006863a15 clang::Sema::ActOnLambdaExpressionAfterIntroducer(clang::LambdaIntroducer&amp;, clang::Scope*) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x6863a15)
&gt;  #<!-- -->7 0x00000000062399b7 clang::Parser::ParseLambdaExpressionAfterIntroducer(clang::LambdaIntroducer&amp;) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x62399b7)
&gt;  #<!-- -->8 0x000000000623c42b clang::Parser::ParseLambdaExpression() (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x623c42b)
&gt;  #<!-- -->9 0x000000000621aced clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&amp;, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x621aced)
&gt; #<!-- -->10 0x000000000621b5ba clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x621b5ba)
&gt; #<!-- -->11 0x000000000621b7a9 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x621b7a9)
&gt; #<!-- -->12 0x0000000006220759 clang::Parser::ParseExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x6220759)
&gt; #<!-- -->13 0x0000000006224bf5 clang::Parser::ParseParenExpression(clang::ParenParseOption&amp;, bool, clang::ParenExprKind, clang::TypoCorrectionTypeBehavior, clang::OpaquePtr&lt;clang::QualType&gt;&amp;, clang::SourceLocation&amp;) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x6224bf5)
&gt; #<!-- -->14 0x0000000006225dce clang::Parser::ParseExprAfterUnaryExprOrTypeTrait(clang::Token const&amp;, bool&amp;, clang::OpaquePtr&lt;clang::QualType&gt;&amp;, clang::SourceRange&amp;) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x6225dce)
&gt; #<!-- -->15 0x0000000006226553 clang::Parser::ParseUnaryExprOrTypeTraitExpression() (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x6226553)
&gt; #<!-- -->16 0x000000000621983a clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&amp;, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x621983a)
&gt; #<!-- -->17 0x000000000621b5ba clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x621b5ba)
&gt; #<!-- -->18 0x000000000621b7a9 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x621b7a9)
&gt; #<!-- -->19 0x00000000061c04e0 clang::Parser::ParseLexedMethodDeclaration(clang::Parser::LateParsedMethodDeclaration&amp;) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x61c04e0)
&gt; #<!-- -->20 0x00000000061b644d clang::Parser::ParseLexedMethodDeclarations(clang::Parser::ParsingClass&amp;) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x61b644d)
&gt; #<!-- -->21 0x000000000620ea1d clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&amp;, unsigned int, clang::Decl*) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x620ea1d)
&gt; #<!-- -->22 0x0000000006211113 clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x6211113)
&gt; #<!-- -->23 0x00000000061d9549 clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x61d9549)
&gt; #<!-- -->24 0x0000000006195256 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x6195256)
&gt; #<!-- -->25 0x00000000061963de clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x61963de)
&gt; #<!-- -->26 0x000000000619ef7f clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x619ef7f)
&gt; #<!-- -->27 0x00000000061a07b0 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x61a07b0)
&gt; #<!-- -->28 0x00000000061a0d00 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x61a0d00)
&gt; #<!-- -->29 0x000000000618ef43 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x618ef43)
&gt; #<!-- -->30 0x000000000454985d clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x454985d)
&gt; #<!-- -->31 0x000000000485b83a clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x485b83a)
&gt; #<!-- -->32 0x00000000047d655b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x47d655b)
&gt; #<!-- -->33 0x000000000494e513 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x494e513)
&gt; #<!-- -->34 0x0000000000da5f25 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0xda5f25)
&gt; #<!-- -->35 0x0000000000d9de2d ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
&gt; #<!-- -->36 0x00000000045bb869 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
&gt; #<!-- -->37 0x0000000003b48813 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x3b48813)
&gt; #<!-- -->38 0x00000000045bba89 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
&gt; #<!-- -->39 0x000000000457eddd clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x457eddd)
&gt; #<!-- -->40 0x000000000457fd91 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x457fd91)
&gt; #<!-- -->41 0x000000000458d29c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0x458d29c)
&gt; #<!-- -->42 0x0000000000da2751 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0xda2751)
&gt; #<!-- -->43 0x0000000000c526f4 main (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0xc526f4)
&gt; #<!-- -->44 0x0000777d2e429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
&gt; #<!-- -->45 0x0000777d2e429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
&gt; #<!-- -->46 0x0000000000d9d8c5 _start (/opt/compiler-explorer/clang-21.1.0/bin/clang+++0xd9d8c5)
&gt; clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
&gt; Compiler returned: 139
</details>


---

### Comment 2 - EugeneZelenko

Crashes in `main` too: https://godbolt.org/z/Ezd8GrWqh

---

