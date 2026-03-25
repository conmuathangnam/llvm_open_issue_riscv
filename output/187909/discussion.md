# [clang]Assertion failure when defining scoped enum member as a function template of a non-template specialization

**Author:** Attacker23
**URL:** https://github.com/llvm/llvm-project/issues/187909
**Status:** Open
**Labels:** clang:frontend, crash-on-invalid

## Body

The following ill-formed code triggers a **clang assertion failure since 12.0.0**:

```cpp
template<typename T>
struct A {
  enum E : T;
  E v = E{};
};

template<typename T>
enum A<int>::E : T { e1 };

A<int> a;
```
**https://godbolt.org/z/h6vKz1o79**


The line

```cpp
template<typename T> enum A<int>::E : T { e1 };
```

is invalid because it declares a _template_ definition (`template<typename T> ...`) for a **non‑template** nested type `A<int>::E`: `A<int>` is a fixed specialization that does not depend on `T`, yet the underlying enum type is written as `: T`, so it incorrectly mixes a non‑dependent entity (`A<int>::E`) with a dependent template parameter `T`.

output:

```
<source>:7:1: error: template parameter list matching the non-templated nested type 'A<int>' should be empty ('template<>')
    7 | template<typename T>
      | ^       ~~~~~~~~~~~~
    8 | enum A<int>::E : T { e1 };
      |      ~~~~~~
clang++: /root/build/tools/clang/include/clang/AST/TypeNodes.inc:82: clang::TypeInfo clang::ASTContext::getTypeInfoImpl(const clang::Type*) const: Assertion `!T->isDependentType() && "should not see dependent types here"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<source>:10:9: current parser token ';'
 #0 0x000000000444d8f8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x444d8f8)
 #1 0x000000000444a724 llvm::sys::RunSignalHandlers() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x444a724)
 #2 0x000000000444ad44 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x444ad44)
 #3 0x000000000438a0e8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #4 0x000075c982442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #5 0x000075c9824969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #6 0x000075c982442476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #7 0x000075c9824287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #8 0x000075c98242871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #9 0x000075c982439e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
#10 0x0000000007c7066a (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7c7066a)
#11 0x0000000007c6c8e7 clang::ASTContext::getTypeInfo(clang::Type const*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7c6c8e7)
#12 0x0000000007c6e083 clang::ASTContext::getIntWidth(clang::QualType) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7c6e083)
#13 0x0000000007fed3f5 (anonymous namespace)::IntExprEvaluator::Success(unsigned long, clang::Expr const*, clang::APValue&) (.isra.0) ExprConstant.cpp:0:0
#14 0x00000000080170e1 clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#15 0x0000000008096e53 (anonymous namespace)::IntExprEvaluator::VisitCastExpr(clang::CastExpr const*) ExprConstant.cpp:0:0
#16 0x0000000008015756 clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#17 0x0000000008015b40 clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#18 0x000000000803d506 Evaluate(clang::APValue&, (anonymous namespace)::EvalInfo&, clang::Expr const*) ExprConstant.cpp:0:0
#19 0x000000000802faa5 EvaluateInPlace(clang::APValue&, (anonymous namespace)::EvalInfo&, (anonymous namespace)::LValue const&, clang::Expr const*, bool) (.part.0) ExprConstant.cpp:0:0
#20 0x000000000805e415 HandleConstructorCall(clang::Expr const*, (anonymous namespace)::LValue const&, (anonymous namespace)::CallRef, clang::CXXConstructorDecl const*, (anonymous namespace)::EvalInfo&, clang::APValue&) ExprConstant.cpp:0:0
#21 0x000000000805f05f HandleConstructorCall(clang::Expr const*, (anonymous namespace)::LValue const&, llvm::ArrayRef<clang::Expr const*>, clang::CXXConstructorDecl const*, (anonymous namespace)::EvalInfo&, clang::APValue&) ExprConstant.cpp:0:0
#22 0x000000000805f295 (anonymous namespace)::RecordExprEvaluator::VisitCXXConstructExpr(clang::CXXConstructExpr const*, clang::QualType) ExprConstant.cpp:0:0
#23 0x000000000802d7b0 clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::RecordExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#24 0x000000000802f978 EvaluateRecord(clang::Expr const*, (anonymous namespace)::LValue const&, clang::APValue&, (anonymous namespace)::EvalInfo&) ExprConstant.cpp:0:0
#25 0x000000000802fac9 EvaluateInPlace(clang::APValue&, (anonymous namespace)::EvalInfo&, (anonymous namespace)::LValue const&, clang::Expr const*, bool) (.part.0) ExprConstant.cpp:0:0
#26 0x0000000008091a63 clang::Expr::EvaluateAsInitializer(clang::APValue&, clang::ASTContext const&, clang::VarDecl const*, llvm::SmallVectorImpl<std::pair<clang::SourceLocation, clang::PartialDiagnostic>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x8091a63)
#27 0x0000000007e7ab72 clang::VarDecl::evaluateValueImpl(llvm::SmallVectorImpl<std::pair<clang::SourceLocation, clang::PartialDiagnostic>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7e7ab72)
#28 0x0000000007e7af21 clang::VarDecl::checkForConstantInitialization(llvm::SmallVectorImpl<std::pair<clang::SourceLocation, clang::PartialDiagnostic>>&) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7e7af21)
#29 0x000000000709abcb clang::Sema::CheckCompleteVariableDeclaration(clang::VarDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x709abcb)
#30 0x000000000709c318 clang::Sema::ActOnUninitializedDecl(clang::Decl*) (.part.0) SemaDecl.cpp:0:0
#31 0x0000000006ce8d8b clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ce8d8b)
#32 0x0000000006cf6e3a clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6cf6e3a)
#33 0x0000000006cb4b8b clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6cb4b8b)
#34 0x0000000006cb538f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6cb538f)
#35 0x0000000006cbc9b1 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6cbc9b1)
#36 0x0000000006cbd8e5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6cbd8e5)
#37 0x0000000006c9efaa clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c9efaa)
#38 0x0000000004f38bb8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f38bb8)
#39 0x00000000052538b5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x52538b5)
#40 0x00000000051d13ae clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x51d13ae)
#41 0x000000000534f89d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x534f89d)
#42 0x0000000000de37e2 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde37e2)
#43 0x0000000000dda11a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#44 0x0000000000dda29d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#45 0x0000000004fa1d79 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#46 0x000000000438a524 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x438a524)
#47 0x0000000004fa23a6 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#48 0x0000000004f61482 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f61482)
#49 0x0000000004f6242e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f6242e)
#50 0x0000000004f69fe5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f69fe5)
#51 0x0000000000ddfb44 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xddfb44)
#52 0x0000000000c97b3a main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc97b3a)
#53 0x000075c982429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#54 0x000075c982429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#55 0x0000000000dd9bb5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdd9bb5)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (Attacker23)

<details>
The following ill-formed code triggers a **clang assertion failure since 12.0.0**:

```cpp
template&lt;typename T&gt;
struct A {
  enum E : T;
  E v = E{};
};

template&lt;typename T&gt;
enum A&lt;int&gt;::E : T { e1 };

A&lt;int&gt; a;
```
**https://godbolt.org/z/h6vKz1o79**


The line

```cpp
template&lt;typename T&gt; enum A&lt;int&gt;::E : T { e1 };
```

is invalid because it declares a _template_ definition (`template&lt;typename T&gt; ...`) for a **non‑template** nested type `A&lt;int&gt;::E`: `A&lt;int&gt;` is a fixed specialization that does not depend on `T`, yet the underlying enum type is written as `: T`, so it incorrectly mixes a non‑dependent entity (`A&lt;int&gt;::E`) with a dependent template parameter `T`.

output:

```
&lt;source&gt;:7:1: error: template parameter list matching the non-templated nested type 'A&lt;int&gt;' should be empty ('template&lt;&gt;')
    7 | template&lt;typename T&gt;
      | ^       ~~~~~~~~~~~~
    8 | enum A&lt;int&gt;::E : T { e1 };
      |      ~~~~~~
clang++: /root/build/tools/clang/include/clang/AST/TypeNodes.inc:82: clang::TypeInfo clang::ASTContext::getTypeInfoImpl(const clang::Type*) const: Assertion `!T-&gt;isDependentType() &amp;&amp; "should not see dependent types here"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics &lt;source&gt;
1.	&lt;source&gt;:10:9: current parser token ';'
 #<!-- -->0 0x000000000444d8f8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x444d8f8)
 #<!-- -->1 0x000000000444a724 llvm::sys::RunSignalHandlers() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x444a724)
 #<!-- -->2 0x000000000444ad44 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x444ad44)
 #<!-- -->3 0x000000000438a0e8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->4 0x000075c982442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->5 0x000075c9824969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->6 0x000075c982442476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->7 0x000075c9824287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->8 0x000075c98242871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->9 0x000075c982439e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
#<!-- -->10 0x0000000007c7066a (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7c7066a)
#<!-- -->11 0x0000000007c6c8e7 clang::ASTContext::getTypeInfo(clang::Type const*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7c6c8e7)
#<!-- -->12 0x0000000007c6e083 clang::ASTContext::getIntWidth(clang::QualType) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7c6e083)
#<!-- -->13 0x0000000007fed3f5 (anonymous namespace)::IntExprEvaluator::Success(unsigned long, clang::Expr const*, clang::APValue&amp;) (.isra.0) ExprConstant.cpp:0:0
#<!-- -->14 0x00000000080170e1 clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool&gt;::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#<!-- -->15 0x0000000008096e53 (anonymous namespace)::IntExprEvaluator::VisitCastExpr(clang::CastExpr const*) ExprConstant.cpp:0:0
#<!-- -->16 0x0000000008015756 clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool&gt;::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#<!-- -->17 0x0000000008015b40 clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool&gt;::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#<!-- -->18 0x000000000803d506 Evaluate(clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Expr const*) ExprConstant.cpp:0:0
#<!-- -->19 0x000000000802faa5 EvaluateInPlace(clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;, (anonymous namespace)::LValue const&amp;, clang::Expr const*, bool) (.part.0) ExprConstant.cpp:0:0
#<!-- -->20 0x000000000805e415 HandleConstructorCall(clang::Expr const*, (anonymous namespace)::LValue const&amp;, (anonymous namespace)::CallRef, clang::CXXConstructorDecl const*, (anonymous namespace)::EvalInfo&amp;, clang::APValue&amp;) ExprConstant.cpp:0:0
#<!-- -->21 0x000000000805f05f HandleConstructorCall(clang::Expr const*, (anonymous namespace)::LValue const&amp;, llvm::ArrayRef&lt;clang::Expr const*&gt;, clang::CXXConstructorDecl const*, (anonymous namespace)::EvalInfo&amp;, clang::APValue&amp;) ExprConstant.cpp:0:0
#<!-- -->22 0x000000000805f295 (anonymous namespace)::RecordExprEvaluator::VisitCXXConstructExpr(clang::CXXConstructExpr const*, clang::QualType) ExprConstant.cpp:0:0
#<!-- -->23 0x000000000802d7b0 clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::RecordExprEvaluator, bool&gt;::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#<!-- -->24 0x000000000802f978 EvaluateRecord(clang::Expr const*, (anonymous namespace)::LValue const&amp;, clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;) ExprConstant.cpp:0:0
#<!-- -->25 0x000000000802fac9 EvaluateInPlace(clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;, (anonymous namespace)::LValue const&amp;, clang::Expr const*, bool) (.part.0) ExprConstant.cpp:0:0
#<!-- -->26 0x0000000008091a63 clang::Expr::EvaluateAsInitializer(clang::APValue&amp;, clang::ASTContext const&amp;, clang::VarDecl const*, llvm::SmallVectorImpl&lt;std::pair&lt;clang::SourceLocation, clang::PartialDiagnostic&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x8091a63)
#<!-- -->27 0x0000000007e7ab72 clang::VarDecl::evaluateValueImpl(llvm::SmallVectorImpl&lt;std::pair&lt;clang::SourceLocation, clang::PartialDiagnostic&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7e7ab72)
#<!-- -->28 0x0000000007e7af21 clang::VarDecl::checkForConstantInitialization(llvm::SmallVectorImpl&lt;std::pair&lt;clang::SourceLocation, clang::PartialDiagnostic&gt;&gt;&amp;) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7e7af21)
#<!-- -->29 0x000000000709abcb clang::Sema::CheckCompleteVariableDeclaration(clang::VarDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x709abcb)
#<!-- -->30 0x000000000709c318 clang::Sema::ActOnUninitializedDecl(clang::Decl*) (.part.0) SemaDecl.cpp:0:0
#<!-- -->31 0x0000000006ce8d8b clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ce8d8b)
#<!-- -->32 0x0000000006cf6e3a clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6cf6e3a)
#<!-- -->33 0x0000000006cb4b8b clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6cb4b8b)
#<!-- -->34 0x0000000006cb538f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6cb538f)
#<!-- -->35 0x0000000006cbc9b1 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6cbc9b1)
#<!-- -->36 0x0000000006cbd8e5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6cbd8e5)
#<!-- -->37 0x0000000006c9efaa clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c9efaa)
#<!-- -->38 0x0000000004f38bb8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f38bb8)
#<!-- -->39 0x00000000052538b5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x52538b5)
#<!-- -->40 0x00000000051d13ae clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x51d13ae)
#<!-- -->41 0x000000000534f89d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x534f89d)
#<!-- -->42 0x0000000000de37e2 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde37e2)
#<!-- -->43 0x0000000000dda11a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->44 0x0000000000dda29d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->45 0x0000000004fa1d79 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->46 0x000000000438a524 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x438a524)
#<!-- -->47 0x0000000004fa23a6 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->48 0x0000000004f61482 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f61482)
#<!-- -->49 0x0000000004f6242e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f6242e)
#<!-- -->50 0x0000000004f69fe5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f69fe5)
#<!-- -->51 0x0000000000ddfb44 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xddfb44)
#<!-- -->52 0x0000000000c97b3a main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc97b3a)
#<!-- -->53 0x000075c982429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->54 0x000075c982429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->55 0x0000000000dd9bb5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdd9bb5)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
Compiler returned: 134
```
</details>


---

