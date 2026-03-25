# [ObjC] Assertion failure when parsing constructor with invalid attribute syntax

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/153731

## Body

Reproducer:
https://godbolt.org/z/K41cYoGvY
```cpp
struct S1 {
  int v{0};
};

struct S2 : S1 {
  S2(int a)
  [[a < 0]] : S2{a, a} {}

  int vc{0};
};
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/include/llvm/Support/Casting.h:578: decltype(auto) llvm::cast(From*) [with To = clang::CXXConstructExpr; From = clang::Expr]: Assertion `isa<To>(Val) && "cast<Ty>() argument of incompatible type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x objective-c++ <source>
1.	<source>:7:24: current parser token '{'
2.	<source>:5:1: parsing struct/union/class body 'S2'
 #0 0x0000000004022128 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4022128)
 #1 0x000000000401f554 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x401f554)
 #2 0x0000000003f63e08 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007ec3a3c42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007ec3a3c969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007ec3a3c42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007ec3a3c287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007ec3a3c2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007ec3a3c39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000006b1aacf clang::Sema::BuildDelegatingInitializer(clang::TypeSourceInfo*, clang::Expr*, clang::CXXRecordDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b1aacf)
#10 0x0000000006b25801 clang::Sema::BuildBaseInitializer(clang::QualType, clang::TypeSourceInfo*, clang::Expr*, clang::CXXRecordDecl*, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b25801)
#11 0x0000000006b3852d clang::Sema::BuildMemInitializer(clang::Decl*, clang::Scope*, clang::CXXScopeSpec&, clang::IdentifierInfo*, clang::OpaquePtr<clang::QualType>, clang::DeclSpec const&, clang::SourceLocation, clang::Expr*, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b3852d)
#12 0x00000000067314d7 clang::Parser::ParseMemInitializer(clang::Decl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67314d7)
#13 0x0000000006733505 clang::Parser::ParseConstructorInitializer(clang::Decl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6733505)
#14 0x00000000066f1208 clang::Parser::ParseLexedMethodDef(clang::Parser::LexedMethod&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66f1208)
#15 0x00000000066f0d6d clang::Parser::ParseLexedMethodDefs(clang::Parser::ParsingClass&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66f0d6d)
#16 0x0000000006744761 clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&, unsigned int, clang::Decl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6744761)
#17 0x0000000006747530 clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6747530)
#18 0x000000000671a6ae clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x671a6ae)
#19 0x00000000066d5068 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66d5068)
#20 0x00000000066d5a9f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66d5a9f)
#21 0x00000000066dd8fa clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66dd8fa)
#22 0x00000000066de895 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66de895)
#23 0x00000000066d03fa clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66d03fa)
#24 0x00000000049c4218 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49c4218)
#25 0x0000000004cb8795 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4cb8795)
#26 0x0000000004c34aae clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c34aae)
#27 0x0000000004daa3b1 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4daa3b1)
#28 0x0000000000daf54f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdaf54f)
#29 0x0000000000da61fa ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#30 0x0000000004a2bc09 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#31 0x0000000003f642a4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f642a4)
#32 0x0000000004a2c21f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#33 0x00000000049ee4bd clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49ee4bd)
#34 0x00000000049ef54e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49ef54e)
#35 0x00000000049f6f95 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49f6f95)
#36 0x0000000000daba05 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdaba05)
#37 0x0000000000c5fa04 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc5fa04)
#38 0x00007ec3a3c29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#39 0x00007ec3a3c29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#40 0x0000000000da5ca5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda5ca5)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - k-arrows

Goes back to clang-16.0.0:
https://godbolt.org/z/5hcfvqxGc

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/K41cYoGvY
```cpp
struct S1 {
  int v{0};
};

struct S2 : S1 {
  S2(int a)
  [[a &lt; 0]] : S2{a, a} {}

  int vc{0};
};
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/include/llvm/Support/Casting.h:578: decltype(auto) llvm::cast(From*) [with To = clang::CXXConstructExpr; From = clang::Expr]: Assertion `isa&lt;To&gt;(Val) &amp;&amp; "cast&lt;Ty&gt;() argument of incompatible type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x objective-c++ &lt;source&gt;
1.	&lt;source&gt;:7:24: current parser token '{'
2.	&lt;source&gt;:5:1: parsing struct/union/class body 'S2'
 #<!-- -->0 0x0000000004022128 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4022128)
 #<!-- -->1 0x000000000401f554 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x401f554)
 #<!-- -->2 0x0000000003f63e08 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007ec3a3c42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007ec3a3c969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007ec3a3c42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007ec3a3c287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x00007ec3a3c2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x00007ec3a3c39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000006b1aacf clang::Sema::BuildDelegatingInitializer(clang::TypeSourceInfo*, clang::Expr*, clang::CXXRecordDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b1aacf)
#<!-- -->10 0x0000000006b25801 clang::Sema::BuildBaseInitializer(clang::QualType, clang::TypeSourceInfo*, clang::Expr*, clang::CXXRecordDecl*, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b25801)
#<!-- -->11 0x0000000006b3852d clang::Sema::BuildMemInitializer(clang::Decl*, clang::Scope*, clang::CXXScopeSpec&amp;, clang::IdentifierInfo*, clang::OpaquePtr&lt;clang::QualType&gt;, clang::DeclSpec const&amp;, clang::SourceLocation, clang::Expr*, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b3852d)
#<!-- -->12 0x00000000067314d7 clang::Parser::ParseMemInitializer(clang::Decl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67314d7)
#<!-- -->13 0x0000000006733505 clang::Parser::ParseConstructorInitializer(clang::Decl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6733505)
#<!-- -->14 0x00000000066f1208 clang::Parser::ParseLexedMethodDef(clang::Parser::LexedMethod&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66f1208)
#<!-- -->15 0x00000000066f0d6d clang::Parser::ParseLexedMethodDefs(clang::Parser::ParsingClass&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66f0d6d)
#<!-- -->16 0x0000000006744761 clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&amp;, unsigned int, clang::Decl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6744761)
#<!-- -->17 0x0000000006747530 clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6747530)
#<!-- -->18 0x000000000671a6ae clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x671a6ae)
#<!-- -->19 0x00000000066d5068 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66d5068)
#<!-- -->20 0x00000000066d5a9f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66d5a9f)
#<!-- -->21 0x00000000066dd8fa clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66dd8fa)
#<!-- -->22 0x00000000066de895 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66de895)
#<!-- -->23 0x00000000066d03fa clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66d03fa)
#<!-- -->24 0x00000000049c4218 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49c4218)
#<!-- -->25 0x0000000004cb8795 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4cb8795)
#<!-- -->26 0x0000000004c34aae clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c34aae)
#<!-- -->27 0x0000000004daa3b1 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4daa3b1)
#<!-- -->28 0x0000000000daf54f cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdaf54f)
#<!-- -->29 0x0000000000da61fa ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->30 0x0000000004a2bc09 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->31 0x0000000003f642a4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f642a4)
#<!-- -->32 0x0000000004a2c21f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->33 0x00000000049ee4bd clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49ee4bd)
#<!-- -->34 0x00000000049ef54e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49ef54e)
#<!-- -->35 0x00000000049f6f95 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49f6f95)
#<!-- -->36 0x0000000000daba05 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdaba05)
#<!-- -->37 0x0000000000c5fa04 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc5fa04)
#<!-- -->38 0x00007ec3a3c29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->39 0x00007ec3a3c29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->40 0x0000000000da5ca5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda5ca5)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

### Comment 3 - erichkeane

Something wacky is happening here :)  The thing that we are trying to cast is:
```
(gdb) p DelegationInit.get()->dump()
InitListExpr 0x55556c72a8f0 'struct S2'
|-InitListExpr 0x55556c72a940 'S1'
| `-ImplicitCastExpr 0x55556c72a988 'int' <LValueToRValue>
|   `-DeclRefExpr 0x55556c7061d8 'int' lvalue ParmVar 0x55556c705df8 'a' 'int'
`-ImplicitCastExpr 0x55556c72aad0 'int' <LValueToRValue>
  `-DeclRefExpr 0x55556c7061f8 'int' lvalue ParmVar 0x55556c705df8 'a' 'int'
$3 = void
```

So for some reason that constructor is being created as a InitList of two references to 'a', and not the constructor we expect.  it isn't clear to me what is going on, but it appears the parser is doing something strange here for ObjC++.

---

