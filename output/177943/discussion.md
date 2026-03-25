# [clang] Assertion `ED->isThisDeclarationADemotedDefinition()' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/177943
**Status:** Closed
**Labels:** clang:frontend, confirmed, crash-on-valid, regression:22
**Closed Date:** 2026-01-29T12:26:15Z

## Body

Reproducer:
https://godbolt.org/z/7Ecq8s7qq
```cpp
enum E {};
enum E;

enum {
  a = (__underlying_type(E)){},
};
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/Sema/SemaType.cpp:9894: clang::QualType GetEnumUnderlyingType(clang::Sema&, clang::QualType, clang::SourceLocation): Assertion `ED->isThisDeclarationADemotedDefinition()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<source>:5:29: current parser token '{'
 #0 0x000000000430e638 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x430e638)
 #1 0x000000000430ba94 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x430ba94)
 #2 0x000000000424d858 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000076d486a42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x000076d486a969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x000076d486a42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x000076d486a287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x000076d486a2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x000076d486a39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x00000000076d275d GetEnumUnderlyingType(clang::Sema&, clang::QualType, clang::SourceLocation) SemaType.cpp:0:0
#10 0x00000000076dda08 clang::Sema::BuiltinEnumUnderlyingType(clang::QualType, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x76dda08)
#11 0x00000000076edd22 clang::Sema::BuildUnaryTransformType(clang::QualType, clang::UnaryTransformType::UTTKind, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x76edd22)
#12 0x00000000076f12fa GetDeclSpecTypeForDeclarator((anonymous namespace)::TypeProcessingState&, clang::TypeSourceInfo*&) (.constprop.0) SemaType.cpp:0:0
#13 0x0000000007701697 clang::Sema::GetTypeForDeclarator(clang::Declarator&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7701697)
#14 0x0000000007701a0e clang::Sema::ActOnTypeName(clang::Declarator&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7701a0e)
#15 0x0000000006b11a09 clang::Parser::ParseParenExpression(clang::ParenParseOption&, bool, clang::ParenExprKind, clang::TypoCorrectionTypeBehavior, clang::OpaquePtr<clang::QualType>&, clang::SourceLocation&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b11a09)
#16 0x0000000006b07678 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b07678)
#17 0x0000000006b1399e clang::Parser::ParseConstantExpressionInExprEvalContext(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b1399e)
#18 0x0000000006ace26d clang::Parser::ParseEnumBody(clang::SourceLocation, clang::Decl*, clang::SkipBodyInfo*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ace26d)
#19 0x0000000006ad6614 clang::Parser::ParseEnumSpecifier(clang::SourceLocation, clang::DeclSpec&, clang::Parser::ParsedTemplateInfo const&, clang::AccessSpecifier, clang::Parser::DeclSpecContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ad6614)
#20 0x0000000006ad26ea clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ad26ea)
#21 0x0000000006a8cb97 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a8cb97)
#22 0x0000000006a8d60f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a8d60f)
#23 0x0000000006a95611 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a95611)
#24 0x0000000006a96525 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a96525)
#25 0x0000000006a7723a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a7723a)
#26 0x0000000004c77cd8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c77cd8)
#27 0x0000000004f72e35 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f72e35)
#28 0x0000000004ef285e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ef285e)
#29 0x000000000506e01d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x506e01d)
#30 0x0000000000dde97e cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdde97e)
#31 0x0000000000dd534a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#32 0x0000000000dd54cd int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#33 0x0000000004ceb719 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#34 0x000000000424dcf4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x424dcf4)
#35 0x0000000004cebd2f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#36 0x0000000004cac702 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4cac702)
#37 0x0000000004cad6ae clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4cad6ae)
#38 0x0000000004cb4af5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4cb4af5)
#39 0x0000000000ddacf1 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xddacf1)
#40 0x0000000000c97d44 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc97d44)
#41 0x000076d486a29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#42 0x000076d486a29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#43 0x0000000000dd4de5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdd4de5)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/7Ecq8s7qq
```cpp
enum E {};
enum E;

enum {
  a = (__underlying_type(E)){},
};
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/Sema/SemaType.cpp:9894: clang::QualType GetEnumUnderlyingType(clang::Sema&amp;, clang::QualType, clang::SourceLocation): Assertion `ED-&gt;isThisDeclarationADemotedDefinition()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics &lt;source&gt;
1.	&lt;source&gt;:5:29: current parser token '{'
 #<!-- -->0 0x000000000430e638 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x430e638)
 #<!-- -->1 0x000000000430ba94 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x430ba94)
 #<!-- -->2 0x000000000424d858 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x000076d486a42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x000076d486a969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x000076d486a42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x000076d486a287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x000076d486a2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x000076d486a39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x00000000076d275d GetEnumUnderlyingType(clang::Sema&amp;, clang::QualType, clang::SourceLocation) SemaType.cpp:0:0
#<!-- -->10 0x00000000076dda08 clang::Sema::BuiltinEnumUnderlyingType(clang::QualType, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x76dda08)
#<!-- -->11 0x00000000076edd22 clang::Sema::BuildUnaryTransformType(clang::QualType, clang::UnaryTransformType::UTTKind, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x76edd22)
#<!-- -->12 0x00000000076f12fa GetDeclSpecTypeForDeclarator((anonymous namespace)::TypeProcessingState&amp;, clang::TypeSourceInfo*&amp;) (.constprop.0) SemaType.cpp:0:0
#<!-- -->13 0x0000000007701697 clang::Sema::GetTypeForDeclarator(clang::Declarator&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7701697)
#<!-- -->14 0x0000000007701a0e clang::Sema::ActOnTypeName(clang::Declarator&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7701a0e)
#<!-- -->15 0x0000000006b11a09 clang::Parser::ParseParenExpression(clang::ParenParseOption&amp;, bool, clang::ParenExprKind, clang::TypoCorrectionTypeBehavior, clang::OpaquePtr&lt;clang::QualType&gt;&amp;, clang::SourceLocation&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b11a09)
#<!-- -->16 0x0000000006b07678 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&amp;, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b07678)
#<!-- -->17 0x0000000006b1399e clang::Parser::ParseConstantExpressionInExprEvalContext(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b1399e)
#<!-- -->18 0x0000000006ace26d clang::Parser::ParseEnumBody(clang::SourceLocation, clang::Decl*, clang::SkipBodyInfo*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ace26d)
#<!-- -->19 0x0000000006ad6614 clang::Parser::ParseEnumSpecifier(clang::SourceLocation, clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::AccessSpecifier, clang::Parser::DeclSpecContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ad6614)
#<!-- -->20 0x0000000006ad26ea clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ad26ea)
#<!-- -->21 0x0000000006a8cb97 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a8cb97)
#<!-- -->22 0x0000000006a8d60f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a8d60f)
#<!-- -->23 0x0000000006a95611 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a95611)
#<!-- -->24 0x0000000006a96525 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a96525)
#<!-- -->25 0x0000000006a7723a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a7723a)
#<!-- -->26 0x0000000004c77cd8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c77cd8)
#<!-- -->27 0x0000000004f72e35 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f72e35)
#<!-- -->28 0x0000000004ef285e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ef285e)
#<!-- -->29 0x000000000506e01d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x506e01d)
#<!-- -->30 0x0000000000dde97e cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdde97e)
#<!-- -->31 0x0000000000dd534a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->32 0x0000000000dd54cd int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->33 0x0000000004ceb719 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->34 0x000000000424dcf4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x424dcf4)
#<!-- -->35 0x0000000004cebd2f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->36 0x0000000004cac702 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4cac702)
#<!-- -->37 0x0000000004cad6ae clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4cad6ae)
#<!-- -->38 0x0000000004cb4af5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4cb4af5)
#<!-- -->39 0x0000000000ddacf1 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xddacf1)
#<!-- -->40 0x0000000000c97d44 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc97d44)
#<!-- -->41 0x000076d486a29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->42 0x000076d486a29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->43 0x0000000000dd4de5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdd4de5)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

### Comment 2 - k-arrows

fine with clang-21:
https://godbolt.org/z/Ybh11erd9

---

### Comment 3 - k-arrows

The above reproducer is available at:
https://github.com/k-arrows/clang-assert-tests/blob/main/cases/00116.cpp

---

