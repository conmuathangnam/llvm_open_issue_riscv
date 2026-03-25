# [clang] Assertion `(S.getLangOpts().CPlusPlus11 || CCE == CCEKind::TempArgStrict) && "converted constant expression outside C++11 or TTP matching"' failed

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/152729
**Status:** Closed
**Labels:** clang:frontend, confirmed, crash
**Closed Date:** 2025-08-13T14:03:11Z

## Body

Reproducer:
https://godbolt.org/z/7rTfGo6ze
```cpp
struct S {
  explicit(true) S(int);
};
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/Sema/SemaOverload.cpp:6271: clang::ExprResult BuildConvertedConstantExpression(clang::Sema&, clang::Expr*, clang::QualType, clang::CCEKind, clang::NamedDecl*, clang::APValue&): Assertion `(S.getLangOpts().CPlusPlus11 || CCE == CCEKind::TempArgStrict) && "converted constant expression outside C++11 or TTP matching"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics --analyze --std=c++03 <source>
1.	<source>:2:18: current parser token 'S'
2.	<source>:1:1: parsing struct/union/class body 'S'
 #0 0x0000000003ff2ff8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3ff2ff8)
 #1 0x0000000003ff0424 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3ff0424)
 #2 0x0000000003f34d18 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007ff955842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007ff9558969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007ff955842476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007ff9558287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007ff95582871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007ff955839e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000006f934af BuildConvertedConstantExpression(clang::Sema&, clang::Expr*, clang::QualType, clang::CCEKind, clang::NamedDecl*, clang::APValue&) SemaOverload.cpp:0:0
#10 0x0000000006f9369a clang::Sema::CheckConvertedConstantExpression(clang::Expr*, clang::QualType, llvm::APSInt&, clang::CCEKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f9369a)
#11 0x0000000006adddb8 clang::Sema::tryResolveExplicitSpecifier(clang::ExplicitSpecifier&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6adddb8)
#12 0x0000000006addeca clang::Sema::ActOnExplicitBoolSpecifier(clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6addeca)
#13 0x00000000066f0c09 clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66f0c09)
#14 0x0000000006713ff6 clang::Parser::ParseCXXClassMemberDeclaration(clang::AccessSpecifier, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::ParsingDeclRAIIObject*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6713ff6)
#15 0x00000000067164bf clang::Parser::ParseCXXClassMemberDeclarationWithPragmas(clang::AccessSpecifier&, clang::ParsedAttributes&, clang::TypeSpecifierType, clang::Decl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67164bf)
#16 0x00000000067182d7 clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&, unsigned int, clang::Decl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67182d7)
#17 0x000000000671b200 clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x671b200)
#18 0x00000000066ee9ce clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66ee9ce)
#19 0x00000000066a9ad8 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66a9ad8)
#20 0x00000000066aa4ff clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66aa4ff)
#21 0x00000000066b220a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66b220a)
#22 0x00000000066b3195 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66b3195)
#23 0x00000000066b3650 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66b3650)
#24 0x00000000066a4eb3 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66a4eb3)
#25 0x0000000004c8ebd5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c8ebd5)
#26 0x0000000004c0aeae clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c0aeae)
#27 0x0000000004d83d01 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d83d01)
#28 0x0000000000dae39f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdae39f)
#29 0x0000000000da500a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#30 0x0000000004a01d39 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#31 0x0000000003f351b4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f351b4)
#32 0x0000000004a0234f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#33 0x00000000049c458d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49c458d)
#34 0x00000000049c561e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49c561e)
#35 0x00000000049cd065 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49cd065)
#36 0x0000000000daa835 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdaa835)
#37 0x0000000000c5ece4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc5ece4)
#38 0x00007ff955829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#39 0x00007ff955829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#40 0x0000000000da4ab5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda4ab5)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-static-analyzer

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/7rTfGo6ze
```cpp
struct S {
  explicit(true) S(int);
};
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/Sema/SemaOverload.cpp:6271: clang::ExprResult BuildConvertedConstantExpression(clang::Sema&amp;, clang::Expr*, clang::QualType, clang::CCEKind, clang::NamedDecl*, clang::APValue&amp;): Assertion `(S.getLangOpts().CPlusPlus11 || CCE == CCEKind::TempArgStrict) &amp;&amp; "converted constant expression outside C++11 or TTP matching"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics --analyze --std=c++03 &lt;source&gt;
1.	&lt;source&gt;:2:18: current parser token 'S'
2.	&lt;source&gt;:1:1: parsing struct/union/class body 'S'
 #<!-- -->0 0x0000000003ff2ff8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3ff2ff8)
 #<!-- -->1 0x0000000003ff0424 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3ff0424)
 #<!-- -->2 0x0000000003f34d18 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007ff955842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007ff9558969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007ff955842476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007ff9558287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x00007ff95582871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x00007ff955839e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000006f934af BuildConvertedConstantExpression(clang::Sema&amp;, clang::Expr*, clang::QualType, clang::CCEKind, clang::NamedDecl*, clang::APValue&amp;) SemaOverload.cpp:0:0
#<!-- -->10 0x0000000006f9369a clang::Sema::CheckConvertedConstantExpression(clang::Expr*, clang::QualType, llvm::APSInt&amp;, clang::CCEKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f9369a)
#<!-- -->11 0x0000000006adddb8 clang::Sema::tryResolveExplicitSpecifier(clang::ExplicitSpecifier&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6adddb8)
#<!-- -->12 0x0000000006addeca clang::Sema::ActOnExplicitBoolSpecifier(clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6addeca)
#<!-- -->13 0x00000000066f0c09 clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66f0c09)
#<!-- -->14 0x0000000006713ff6 clang::Parser::ParseCXXClassMemberDeclaration(clang::AccessSpecifier, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::ParsingDeclRAIIObject*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6713ff6)
#<!-- -->15 0x00000000067164bf clang::Parser::ParseCXXClassMemberDeclarationWithPragmas(clang::AccessSpecifier&amp;, clang::ParsedAttributes&amp;, clang::TypeSpecifierType, clang::Decl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67164bf)
#<!-- -->16 0x00000000067182d7 clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&amp;, unsigned int, clang::Decl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67182d7)
#<!-- -->17 0x000000000671b200 clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x671b200)
#<!-- -->18 0x00000000066ee9ce clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66ee9ce)
#<!-- -->19 0x00000000066a9ad8 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66a9ad8)
#<!-- -->20 0x00000000066aa4ff clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66aa4ff)
#<!-- -->21 0x00000000066b220a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66b220a)
#<!-- -->22 0x00000000066b3195 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66b3195)
#<!-- -->23 0x00000000066b3650 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66b3650)
#<!-- -->24 0x00000000066a4eb3 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66a4eb3)
#<!-- -->25 0x0000000004c8ebd5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c8ebd5)
#<!-- -->26 0x0000000004c0aeae clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c0aeae)
#<!-- -->27 0x0000000004d83d01 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d83d01)
#<!-- -->28 0x0000000000dae39f cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdae39f)
#<!-- -->29 0x0000000000da500a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->30 0x0000000004a01d39 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->31 0x0000000003f351b4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f351b4)
#<!-- -->32 0x0000000004a0234f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->33 0x00000000049c458d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49c458d)
#<!-- -->34 0x00000000049c561e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49c561e)
#<!-- -->35 0x00000000049cd065 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49cd065)
#<!-- -->36 0x0000000000daa835 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdaa835)
#<!-- -->37 0x0000000000c5ece4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc5ece4)
#<!-- -->38 0x00007ff955829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->39 0x00007ff955829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->40 0x0000000000da4ab5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda4ab5)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

### Comment 2 - k-arrows

Goes back to clang-10.0.0:
https://godbolt.org/z/aacd788db

---

### Comment 3 - steakhal

This appears to be a regular clang parser crash. Dropped the static analyzer label.
Hopefully, someone will eventually pick it up. Thanks.

---

### Comment 4 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/7rTfGo6ze
```cpp
struct S {
  explicit(true) S(int);
};
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/Sema/SemaOverload.cpp:6271: clang::ExprResult BuildConvertedConstantExpression(clang::Sema&amp;, clang::Expr*, clang::QualType, clang::CCEKind, clang::NamedDecl*, clang::APValue&amp;): Assertion `(S.getLangOpts().CPlusPlus11 || CCE == CCEKind::TempArgStrict) &amp;&amp; "converted constant expression outside C++11 or TTP matching"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics --analyze --std=c++03 &lt;source&gt;
1.	&lt;source&gt;:2:18: current parser token 'S'
2.	&lt;source&gt;:1:1: parsing struct/union/class body 'S'
 #<!-- -->0 0x0000000003ff2ff8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3ff2ff8)
 #<!-- -->1 0x0000000003ff0424 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3ff0424)
 #<!-- -->2 0x0000000003f34d18 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007ff955842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007ff9558969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007ff955842476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007ff9558287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x00007ff95582871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x00007ff955839e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000006f934af BuildConvertedConstantExpression(clang::Sema&amp;, clang::Expr*, clang::QualType, clang::CCEKind, clang::NamedDecl*, clang::APValue&amp;) SemaOverload.cpp:0:0
#<!-- -->10 0x0000000006f9369a clang::Sema::CheckConvertedConstantExpression(clang::Expr*, clang::QualType, llvm::APSInt&amp;, clang::CCEKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f9369a)
#<!-- -->11 0x0000000006adddb8 clang::Sema::tryResolveExplicitSpecifier(clang::ExplicitSpecifier&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6adddb8)
#<!-- -->12 0x0000000006addeca clang::Sema::ActOnExplicitBoolSpecifier(clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6addeca)
#<!-- -->13 0x00000000066f0c09 clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66f0c09)
#<!-- -->14 0x0000000006713ff6 clang::Parser::ParseCXXClassMemberDeclaration(clang::AccessSpecifier, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::ParsingDeclRAIIObject*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6713ff6)
#<!-- -->15 0x00000000067164bf clang::Parser::ParseCXXClassMemberDeclarationWithPragmas(clang::AccessSpecifier&amp;, clang::ParsedAttributes&amp;, clang::TypeSpecifierType, clang::Decl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67164bf)
#<!-- -->16 0x00000000067182d7 clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&amp;, unsigned int, clang::Decl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67182d7)
#<!-- -->17 0x000000000671b200 clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x671b200)
#<!-- -->18 0x00000000066ee9ce clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66ee9ce)
#<!-- -->19 0x00000000066a9ad8 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66a9ad8)
#<!-- -->20 0x00000000066aa4ff clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66aa4ff)
#<!-- -->21 0x00000000066b220a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66b220a)
#<!-- -->22 0x00000000066b3195 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66b3195)
#<!-- -->23 0x00000000066b3650 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66b3650)
#<!-- -->24 0x00000000066a4eb3 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66a4eb3)
#<!-- -->25 0x0000000004c8ebd5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c8ebd5)
#<!-- -->26 0x0000000004c0aeae clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c0aeae)
#<!-- -->27 0x0000000004d83d01 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d83d01)
#<!-- -->28 0x0000000000dae39f cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdae39f)
#<!-- -->29 0x0000000000da500a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->30 0x0000000004a01d39 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->31 0x0000000003f351b4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f351b4)
#<!-- -->32 0x0000000004a0234f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->33 0x00000000049c458d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49c458d)
#<!-- -->34 0x00000000049c561e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49c561e)
#<!-- -->35 0x00000000049cd065 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49cd065)
#<!-- -->36 0x0000000000daa835 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdaa835)
#<!-- -->37 0x0000000000c5ece4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc5ece4)
#<!-- -->38 0x00007ff955829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->39 0x00007ff955829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->40 0x0000000000da4ab5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda4ab5)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

### Comment 5 - AaronBallman

Curiously, it requires c++03 or c++98 mode explicitly, any later mode and the crash goes away.

---

### Comment 6 - Sirraide

The assertion in question here is
https://github.com/llvm/llvm-project/blob/2344f82b58c773abd33debc077647f2adc8bfbd5/clang/lib/Sema/SemaOverload.cpp#L6274-L6280

This also appears to be a recent regression since it works fine in Clang 20.

---

### Comment 7 - jongmyeong-choi

Since it worked in Clang 20, I should probably close the above change. Thanks for pointing it out. Should we remove the assertion,  or add the CCE condition? like 
```cpp
  assert((S.getLangOpts().CPlusPlus11 || CCE == CCEKind::TempArgStrict ||
          CCE == CCEKind::ExplicitBool) &&
         "converted constant expression outside C++11 or TTP matching");
```

---

### Comment 8 - Sirraide

> Should we remove the assertion, or add the CCE condition? like

Not sure off the top of my head. Iirc @mizvekov worked on this a while ago so he might have an idea.

---

### Comment 9 - mizvekov

Yeah, if we are to provide explicit as an extension in C++98, then the assert is wrong and this needs to be accepted.

The assert is not really protecting anything from the implementation side, it's just trying to make sure we don't use a machinery that was only introduced to implement new C++11 wording.

We already make an exception to this so we can backport various partial ordering bug fixes into C++98, that's what the `TempArgStrict` exception is about.

---

### Comment 10 - injae-kim

FYI) Fix PR: https://github.com/llvm/llvm-project/pull/152985 👍

---

### Comment 11 - shafik

>
> This also appears to be a recent regression since it works fine in Clang 20.

Why do you say this? AFAICT this crashes in 20: https://godbolt.org/z/7naEjrhvn

---

### Comment 12 - Sirraide

> > This also appears to be a recent regression since it works fine in Clang 20.
> 
> Why do you say this? AFAICT this crashes in 20: https://godbolt.org/z/7naEjrhvn

I’m candidly not sure; I distinctly recall it not crashing when I tested it, but I may have simply been stupid and forgotten to add `-std=c++03`...

---

