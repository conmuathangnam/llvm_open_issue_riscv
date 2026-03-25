# [clang] UNREACHABLE executed at /root/llvm-project/llvm/tools/clang/lib/Sema/SemaExpr.cpp:14597!

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/151531
**Status:** Closed
**Labels:** c++20, clang:frontend, confirmed, crash
**Closed Date:** 2025-09-17T12:38:10Z

## Body

Reproducer:
https://godbolt.org/z/eM7n4GT1h
```cpp
struct w { int n; };

template<const w* X>
void f() {
  static_assert(X->n == 42);
}

template<w X>
void g() {
  f<&X>();
}

int main() {
  constexpr w X = {42};
  g<X>();
}
```

Backtrace:
```console
Unknown/unexpected decl type
UNREACHABLE executed at /root/llvm-project/llvm/tools/clang/lib/Sema/SemaExpr.cpp:14597!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics --std=c++20 <source>
1.	<eof> parser at end of file
2.	<source>:9:6: instantiating function definition 'g<w{42}>'
3.	<source>:4:6: instantiating function definition 'f<&<template param w{42}>>'
 #0 0x0000000003fdb0d8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3fdb0d8)
 #1 0x0000000003fd8504 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3fd8504)
 #2 0x0000000003f1ccf8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00006ffc0b842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00006ffc0b8969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00006ffc0b842476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00006ffc0b8287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x0000000003f284ba (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f284ba)
 #8 0x0000000006bb755b clang::Sema::CheckAddressOfOperand(clang::ActionResult<clang::Expr*, true>&, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bb755b)
 #9 0x0000000006bd3a53 clang::Sema::CreateBuiltinUnaryOp(clang::SourceLocation, clang::UnaryOperatorKind, clang::Expr*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bd3a53)
#10 0x000000000703ba11 clang::Sema::BuildExpressionFromDeclTemplateArgument(clang::TemplateArgument const&, clang::QualType, clang::SourceLocation, clang::NamedDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x703ba11)
#11 0x00000000071c6a66 (anonymous namespace)::TemplateInstantiator::transformNonTypeTemplateParmRef(clang::Decl*, clang::NonTypeTemplateParmDecl const*, clang::SourceLocation, clang::TemplateArgument, clang::UnsignedOrNone, bool) SemaTemplateInstantiate.cpp:0:0
#12 0x00000000071ea895 (anonymous namespace)::TemplateInstantiator::TransformDeclRefExpr(clang::DeclRefExpr*) SemaTemplateInstantiate.cpp:0:0
#13 0x00000000071b6719 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#14 0x00000000071e8e80 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformMemberExpr(clang::MemberExpr*) SemaTemplateInstantiate.cpp:0:0
#15 0x00000000071b6893 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#16 0x00000000071b66c6 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#17 0x00000000071dfac8 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformBinaryOperator(clang::BinaryOperator*) SemaTemplateInstantiate.cpp:0:0
#18 0x00000000071b63da clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#19 0x00000000071f4a42 clang::Sema::SubstExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x71f4a42)
#20 0x000000000720cf7e clang::TemplateDeclInstantiator::VisitStaticAssertDecl(clang::StaticAssertDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x720cf7e)
#21 0x0000000007270ca4 void llvm::function_ref<void ()>::callback_fn<clang::Sema::SubstDecl(clang::Decl*, clang::DeclContext*, clang::MultiLevelTemplateArgumentList const&)::'lambda'()>(long) SemaTemplateInstantiateDecl.cpp:0:0
#22 0x0000000007f4fe61 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7f4fe61)
#23 0x000000000720e7da clang::Sema::SubstDecl(clang::Decl*, clang::DeclContext*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x720e7da)
#24 0x00000000071afcdd clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformDeclStmt(clang::DeclStmt*) SemaTemplateInstantiate.cpp:0:0
#25 0x00000000071fa52e clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaTemplateInstantiate.cpp:0:0
#26 0x0000000007203be4 clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7203be4)
#27 0x0000000007263eb0 clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7263eb0)
#28 0x0000000007261b76 clang::Sema::PerformPendingInstantiations(bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7261b76)
#29 0x00000000072641b3 clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x72641b3)
#30 0x0000000007261b76 clang::Sema::PerformPendingInstantiations(bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7261b76)
#31 0x00000000068372f2 clang::Sema::ActOnEndOfTranslationUnitFragment(clang::TUFragmentKind) (.part.0) Sema.cpp:0:0
#32 0x0000000006837672 clang::Sema::ActOnEndOfTranslationUnit() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6837672)
#33 0x0000000006694340 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6694340)
#34 0x0000000006685fda clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6685fda)
#35 0x0000000004987388 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4987388)
#36 0x0000000004c7aa05 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c7aa05)
#37 0x0000000004bf780e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bf780e)
#38 0x0000000004d6e731 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d6e731)
#39 0x0000000000daa41f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdaa41f)
#40 0x0000000000da106a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#41 0x00000000049eeaa9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#42 0x0000000003f1d194 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f1d194)
#43 0x00000000049ef0bf clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#44 0x00000000049b16ed clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49b16ed)
#45 0x00000000049b277e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49b277e)
#46 0x00000000049ba7b5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49ba7b5)
#47 0x0000000000da68cf clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda68cf)
#48 0x0000000000c5b7d4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc5b7d4)
#49 0x00006ffc0b829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#50 0x00006ffc0b829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#51 0x0000000000da0b15 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda0b15)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/eM7n4GT1h
```cpp
struct w { int n; };

template&lt;const w* X&gt;
void f() {
  static_assert(X-&gt;n == 42);
}

template&lt;w X&gt;
void g() {
  f&lt;&amp;X&gt;();
}

int main() {
  constexpr w X = {42};
  g&lt;X&gt;();
}
```

Backtrace:
```console
Unknown/unexpected decl type
UNREACHABLE executed at /root/llvm-project/llvm/tools/clang/lib/Sema/SemaExpr.cpp:14597!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics --std=c++20 &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	&lt;source&gt;:9:6: instantiating function definition 'g&lt;w{42}&gt;'
3.	&lt;source&gt;:4:6: instantiating function definition 'f&lt;&amp;&lt;template param w{42}&gt;&gt;'
 #<!-- -->0 0x0000000003fdb0d8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3fdb0d8)
 #<!-- -->1 0x0000000003fd8504 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3fd8504)
 #<!-- -->2 0x0000000003f1ccf8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00006ffc0b842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00006ffc0b8969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00006ffc0b842476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00006ffc0b8287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x0000000003f284ba (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f284ba)
 #<!-- -->8 0x0000000006bb755b clang::Sema::CheckAddressOfOperand(clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bb755b)
 #<!-- -->9 0x0000000006bd3a53 clang::Sema::CreateBuiltinUnaryOp(clang::SourceLocation, clang::UnaryOperatorKind, clang::Expr*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bd3a53)
#<!-- -->10 0x000000000703ba11 clang::Sema::BuildExpressionFromDeclTemplateArgument(clang::TemplateArgument const&amp;, clang::QualType, clang::SourceLocation, clang::NamedDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x703ba11)
#<!-- -->11 0x00000000071c6a66 (anonymous namespace)::TemplateInstantiator::transformNonTypeTemplateParmRef(clang::Decl*, clang::NonTypeTemplateParmDecl const*, clang::SourceLocation, clang::TemplateArgument, clang::UnsignedOrNone, bool) SemaTemplateInstantiate.cpp:0:0
#<!-- -->12 0x00000000071ea895 (anonymous namespace)::TemplateInstantiator::TransformDeclRefExpr(clang::DeclRefExpr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->13 0x00000000071b6719 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->14 0x00000000071e8e80 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformMemberExpr(clang::MemberExpr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->15 0x00000000071b6893 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->16 0x00000000071b66c6 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->17 0x00000000071dfac8 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformBinaryOperator(clang::BinaryOperator*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->18 0x00000000071b63da clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->19 0x00000000071f4a42 clang::Sema::SubstExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x71f4a42)
#<!-- -->20 0x000000000720cf7e clang::TemplateDeclInstantiator::VisitStaticAssertDecl(clang::StaticAssertDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x720cf7e)
#<!-- -->21 0x0000000007270ca4 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::Sema::SubstDecl(clang::Decl*, clang::DeclContext*, clang::MultiLevelTemplateArgumentList const&amp;)::'lambda'()&gt;(long) SemaTemplateInstantiateDecl.cpp:0:0
#<!-- -->22 0x0000000007f4fe61 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7f4fe61)
#<!-- -->23 0x000000000720e7da clang::Sema::SubstDecl(clang::Decl*, clang::DeclContext*, clang::MultiLevelTemplateArgumentList const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x720e7da)
#<!-- -->24 0x00000000071afcdd clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformDeclStmt(clang::DeclStmt*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->25 0x00000000071fa52e clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaTemplateInstantiate.cpp:0:0
#<!-- -->26 0x0000000007203be4 clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7203be4)
#<!-- -->27 0x0000000007263eb0 clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7263eb0)
#<!-- -->28 0x0000000007261b76 clang::Sema::PerformPendingInstantiations(bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7261b76)
#<!-- -->29 0x00000000072641b3 clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x72641b3)
#<!-- -->30 0x0000000007261b76 clang::Sema::PerformPendingInstantiations(bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7261b76)
#<!-- -->31 0x00000000068372f2 clang::Sema::ActOnEndOfTranslationUnitFragment(clang::TUFragmentKind) (.part.0) Sema.cpp:0:0
#<!-- -->32 0x0000000006837672 clang::Sema::ActOnEndOfTranslationUnit() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6837672)
#<!-- -->33 0x0000000006694340 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6694340)
#<!-- -->34 0x0000000006685fda clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6685fda)
#<!-- -->35 0x0000000004987388 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4987388)
#<!-- -->36 0x0000000004c7aa05 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c7aa05)
#<!-- -->37 0x0000000004bf780e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bf780e)
#<!-- -->38 0x0000000004d6e731 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d6e731)
#<!-- -->39 0x0000000000daa41f cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdaa41f)
#<!-- -->40 0x0000000000da106a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->41 0x00000000049eeaa9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->42 0x0000000003f1d194 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f1d194)
#<!-- -->43 0x00000000049ef0bf clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->44 0x00000000049b16ed clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49b16ed)
#<!-- -->45 0x00000000049b277e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49b277e)
#<!-- -->46 0x00000000049ba7b5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49ba7b5)
#<!-- -->47 0x0000000000da68cf clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda68cf)
#<!-- -->48 0x0000000000c5b7d4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc5b7d4)
#<!-- -->49 0x00006ffc0b829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->50 0x00006ffc0b829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->51 0x0000000000da0b15 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda0b15)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-c-20

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/eM7n4GT1h
```cpp
struct w { int n; };

template&lt;const w* X&gt;
void f() {
  static_assert(X-&gt;n == 42);
}

template&lt;w X&gt;
void g() {
  f&lt;&amp;X&gt;();
}

int main() {
  constexpr w X = {42};
  g&lt;X&gt;();
}
```

Backtrace:
```console
Unknown/unexpected decl type
UNREACHABLE executed at /root/llvm-project/llvm/tools/clang/lib/Sema/SemaExpr.cpp:14597!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics --std=c++20 &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	&lt;source&gt;:9:6: instantiating function definition 'g&lt;w{42}&gt;'
3.	&lt;source&gt;:4:6: instantiating function definition 'f&lt;&amp;&lt;template param w{42}&gt;&gt;'
 #<!-- -->0 0x0000000003fdb0d8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3fdb0d8)
 #<!-- -->1 0x0000000003fd8504 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3fd8504)
 #<!-- -->2 0x0000000003f1ccf8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00006ffc0b842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00006ffc0b8969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00006ffc0b842476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00006ffc0b8287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x0000000003f284ba (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f284ba)
 #<!-- -->8 0x0000000006bb755b clang::Sema::CheckAddressOfOperand(clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bb755b)
 #<!-- -->9 0x0000000006bd3a53 clang::Sema::CreateBuiltinUnaryOp(clang::SourceLocation, clang::UnaryOperatorKind, clang::Expr*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bd3a53)
#<!-- -->10 0x000000000703ba11 clang::Sema::BuildExpressionFromDeclTemplateArgument(clang::TemplateArgument const&amp;, clang::QualType, clang::SourceLocation, clang::NamedDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x703ba11)
#<!-- -->11 0x00000000071c6a66 (anonymous namespace)::TemplateInstantiator::transformNonTypeTemplateParmRef(clang::Decl*, clang::NonTypeTemplateParmDecl const*, clang::SourceLocation, clang::TemplateArgument, clang::UnsignedOrNone, bool) SemaTemplateInstantiate.cpp:0:0
#<!-- -->12 0x00000000071ea895 (anonymous namespace)::TemplateInstantiator::TransformDeclRefExpr(clang::DeclRefExpr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->13 0x00000000071b6719 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->14 0x00000000071e8e80 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformMemberExpr(clang::MemberExpr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->15 0x00000000071b6893 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->16 0x00000000071b66c6 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->17 0x00000000071dfac8 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformBinaryOperator(clang::BinaryOperator*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->18 0x00000000071b63da clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->19 0x00000000071f4a42 clang::Sema::SubstExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x71f4a42)
#<!-- -->20 0x000000000720cf7e clang::TemplateDeclInstantiator::VisitStaticAssertDecl(clang::StaticAssertDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x720cf7e)
#<!-- -->21 0x0000000007270ca4 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::Sema::SubstDecl(clang::Decl*, clang::DeclContext*, clang::MultiLevelTemplateArgumentList const&amp;)::'lambda'()&gt;(long) SemaTemplateInstantiateDecl.cpp:0:0
#<!-- -->22 0x0000000007f4fe61 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7f4fe61)
#<!-- -->23 0x000000000720e7da clang::Sema::SubstDecl(clang::Decl*, clang::DeclContext*, clang::MultiLevelTemplateArgumentList const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x720e7da)
#<!-- -->24 0x00000000071afcdd clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformDeclStmt(clang::DeclStmt*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->25 0x00000000071fa52e clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaTemplateInstantiate.cpp:0:0
#<!-- -->26 0x0000000007203be4 clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7203be4)
#<!-- -->27 0x0000000007263eb0 clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7263eb0)
#<!-- -->28 0x0000000007261b76 clang::Sema::PerformPendingInstantiations(bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7261b76)
#<!-- -->29 0x00000000072641b3 clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x72641b3)
#<!-- -->30 0x0000000007261b76 clang::Sema::PerformPendingInstantiations(bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7261b76)
#<!-- -->31 0x00000000068372f2 clang::Sema::ActOnEndOfTranslationUnitFragment(clang::TUFragmentKind) (.part.0) Sema.cpp:0:0
#<!-- -->32 0x0000000006837672 clang::Sema::ActOnEndOfTranslationUnit() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6837672)
#<!-- -->33 0x0000000006694340 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6694340)
#<!-- -->34 0x0000000006685fda clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6685fda)
#<!-- -->35 0x0000000004987388 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4987388)
#<!-- -->36 0x0000000004c7aa05 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c7aa05)
#<!-- -->37 0x0000000004bf780e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bf780e)
#<!-- -->38 0x0000000004d6e731 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d6e731)
#<!-- -->39 0x0000000000daa41f cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdaa41f)
#<!-- -->40 0x0000000000da106a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->41 0x00000000049eeaa9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->42 0x0000000003f1d194 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f1d194)
#<!-- -->43 0x00000000049ef0bf clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->44 0x00000000049b16ed clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49b16ed)
#<!-- -->45 0x00000000049b277e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49b277e)
#<!-- -->46 0x00000000049ba7b5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49ba7b5)
#<!-- -->47 0x0000000000da68cf clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda68cf)
#<!-- -->48 0x0000000000c5b7d4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc5b7d4)
#<!-- -->49 0x00006ffc0b829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->50 0x00006ffc0b829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->51 0x0000000000da0b15 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda0b15)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

### Comment 3 - shafik

Goes back to clang-12: https://godbolt.org/z/YhY9ssc6b

---

### Comment 4 - Mr-Anyone

Seems to be an assertion failure only. 

---

