# [Clang] Crash on constraint for lambda in lambda with init-capture

**Author:** MagentaTreehouse
**URL:** https://github.com/llvm/llvm-project/issues/128175
**Status:** Closed
**Labels:** clang:frontend, regression, concepts, crash, lambda
**Closed Date:** 2025-02-25T06:35:08Z

## Body

The following C++20 code causes a crash in Clang trunk:

```c++
template <class>
void f() {
    [i{0}] {
        [] () requires true {}();
    }();
}

int main() {
    f<int>();
}
```

See https://compiler-explorer.com/z/6W4To8dco.

Assertion:
```console
clang++: /root/llvm-project/clang/include/clang/AST/DeclCXX.h:1127:
const clang::LambdaCapture* clang::CXXRecordDecl::getCapture(unsigned int) const:
Assertion `isLambda() && I < capture_size() && "invalid index for capture"' failed.
```

<details>

<summary>Stack dump</summary>

```console
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 <source>
1.	<eof> parser at end of file
2.	<source>:2:6: instantiating function definition 'f<int>'
 #0 0x0000000003e98398 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3e98398)
 #1 0x0000000003e96054 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3e96054)
 #2 0x0000000003de2458 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007d78da842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007d78da8969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007d78da842476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007d78da8287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007d78da82871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007d78da839e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x00000000067822cf clang::Sema::addInstantiatedCapturesToScope(clang::FunctionDecl*, clang::FunctionDecl const*, clang::LocalInstantiationScope&, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67822cf)
#10 0x0000000006bab5df clang::Sema::LambdaScopeForCallOperatorInstantiationRAII::LambdaScopeForCallOperatorInstantiationRAII(clang::Sema&, clang::FunctionDecl*, clang::MultiLevelTemplateArgumentList, clang::LocalInstantiationScope&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bab5df)
#11 0x0000000006790c82 clang::Sema::CheckFunctionConstraints(clang::FunctionDecl const*, clang::ConstraintSatisfaction&, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6790c82)
#12 0x0000000006db3b65 clang::Sema::AddMethodCandidate(clang::CXXMethodDecl*, clang::DeclAccessPair, clang::CXXRecordDecl*, clang::QualType, clang::Expr::Classification, llvm::ArrayRef<clang::Expr*>, clang::OverloadCandidateSet&, bool, bool, llvm::MutableArrayRef<clang::ImplicitConversionSequence>, clang::OverloadCandidateParamOrder, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6db3b65)
#13 0x0000000006dc1442 clang::Sema::AddMethodCandidate(clang::DeclAccessPair, clang::QualType, clang::Expr::Classification, llvm::ArrayRef<clang::Expr*>, clang::OverloadCandidateSet&, bool, clang::OverloadCandidateParamOrder) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6dc1442)
#14 0x0000000006dd18f2 clang::Sema::BuildCallToObjectOfClassType(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6dd18f2)
#15 0x00000000069e7663 clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69e7663)
#16 0x00000000069e7f4c clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69e7f4c)
#17 0x0000000007045bd8 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCallExpr(clang::CallExpr*) SemaTemplateInstantiate.cpp:0:0
#18 0x0000000007033d62 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#19 0x0000000007061657 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformStmt(clang::Stmt*, clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::StmtDiscardKind) SemaTemplateInstantiate.cpp:0:0
#20 0x000000000706226e clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaTemplateInstantiate.cpp:0:0
#21 0x0000000007033154 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformLambdaExpr(clang::LambdaExpr*) SemaTemplateInstantiate.cpp:0:0
#22 0x00000000070344ae clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#23 0x0000000007045b06 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCallExpr(clang::CallExpr*) SemaTemplateInstantiate.cpp:0:0
#24 0x0000000007033d62 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#25 0x0000000007061657 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformStmt(clang::Stmt*, clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::StmtDiscardKind) SemaTemplateInstantiate.cpp:0:0
#26 0x000000000706226e clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaTemplateInstantiate.cpp:0:0
#27 0x000000000706b274 clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x706b274)
#28 0x00000000070e30c2 clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x70e30c2)
#29 0x00000000070e0fee clang::Sema::PerformPendingInstantiations(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x70e0fee)
#30 0x000000000664cd6d clang::Sema::ActOnEndOfTranslationUnitFragment(clang::Sema::TUFragmentKind) (.part.0) Sema.cpp:0:0
#31 0x000000000664d562 clang::Sema::ActOnEndOfTranslationUnit() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x664d562)
#32 0x00000000064b7dfb clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64b7dfb)
#33 0x00000000064aa1ba clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64aa1ba)
#34 0x000000000485a108 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x485a108)
#35 0x0000000004b211e5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b211e5)
#36 0x0000000004aa498e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4aa498e)
#37 0x0000000004c0fc3e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c0fc3e)
#38 0x0000000000d5c7ef cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd5c7ef)
#39 0x0000000000d5426a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#40 0x00000000048a0719 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#41 0x0000000003de2904 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3de2904)
#42 0x00000000048a0d0f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#43 0x0000000004863e8d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4863e8d)
#44 0x0000000004864f0e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4864f0e)
#45 0x000000000486cec5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x486cec5)
#46 0x0000000000d595e3 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd595e3)
#47 0x0000000000c2b854 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc2b854)
#48 0x00007d78da829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#49 0x00007d78da829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#50 0x0000000000d53d15 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd53d15)
```

</details>



## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (MagentaTreehouse)

<details>
The following C++20 code causes a crash in Clang trunk:

```c++
template &lt;class&gt;
void f() {
    [i{0}] {
        [] () requires true {}();
    }();
}

int main() {
    f&lt;int&gt;();
}
```

See https://compiler-explorer.com/z/6W4To8dco.

Assertion:
```console
clang++: /root/llvm-project/clang/include/clang/AST/DeclCXX.h:1127:
const clang::LambdaCapture* clang::CXXRecordDecl::getCapture(unsigned int) const:
Assertion `isLambda() &amp;&amp; I &lt; capture_size() &amp;&amp; "invalid index for capture"' failed.
```

&lt;details&gt;

&lt;summary&gt;Stack dump&lt;/summary&gt;

```console
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	&lt;source&gt;:2:6: instantiating function definition 'f&lt;int&gt;'
 #<!-- -->0 0x0000000003e98398 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3e98398)
 #<!-- -->1 0x0000000003e96054 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3e96054)
 #<!-- -->2 0x0000000003de2458 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007d78da842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007d78da8969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007d78da842476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007d78da8287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x00007d78da82871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x00007d78da839e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x00000000067822cf clang::Sema::addInstantiatedCapturesToScope(clang::FunctionDecl*, clang::FunctionDecl const*, clang::LocalInstantiationScope&amp;, clang::MultiLevelTemplateArgumentList const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67822cf)
#<!-- -->10 0x0000000006bab5df clang::Sema::LambdaScopeForCallOperatorInstantiationRAII::LambdaScopeForCallOperatorInstantiationRAII(clang::Sema&amp;, clang::FunctionDecl*, clang::MultiLevelTemplateArgumentList, clang::LocalInstantiationScope&amp;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bab5df)
#<!-- -->11 0x0000000006790c82 clang::Sema::CheckFunctionConstraints(clang::FunctionDecl const*, clang::ConstraintSatisfaction&amp;, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6790c82)
#<!-- -->12 0x0000000006db3b65 clang::Sema::AddMethodCandidate(clang::CXXMethodDecl*, clang::DeclAccessPair, clang::CXXRecordDecl*, clang::QualType, clang::Expr::Classification, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::OverloadCandidateSet&amp;, bool, bool, llvm::MutableArrayRef&lt;clang::ImplicitConversionSequence&gt;, clang::OverloadCandidateParamOrder, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6db3b65)
#<!-- -->13 0x0000000006dc1442 clang::Sema::AddMethodCandidate(clang::DeclAccessPair, clang::QualType, clang::Expr::Classification, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::OverloadCandidateSet&amp;, bool, clang::OverloadCandidateParamOrder) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6dc1442)
#<!-- -->14 0x0000000006dd18f2 clang::Sema::BuildCallToObjectOfClassType(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6dd18f2)
#<!-- -->15 0x00000000069e7663 clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69e7663)
#<!-- -->16 0x00000000069e7f4c clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69e7f4c)
#<!-- -->17 0x0000000007045bd8 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCallExpr(clang::CallExpr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->18 0x0000000007033d62 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->19 0x0000000007061657 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformStmt(clang::Stmt*, clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::StmtDiscardKind) SemaTemplateInstantiate.cpp:0:0
#<!-- -->20 0x000000000706226e clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaTemplateInstantiate.cpp:0:0
#<!-- -->21 0x0000000007033154 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformLambdaExpr(clang::LambdaExpr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->22 0x00000000070344ae clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->23 0x0000000007045b06 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCallExpr(clang::CallExpr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->24 0x0000000007033d62 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->25 0x0000000007061657 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformStmt(clang::Stmt*, clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::StmtDiscardKind) SemaTemplateInstantiate.cpp:0:0
#<!-- -->26 0x000000000706226e clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaTemplateInstantiate.cpp:0:0
#<!-- -->27 0x000000000706b274 clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x706b274)
#<!-- -->28 0x00000000070e30c2 clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x70e30c2)
#<!-- -->29 0x00000000070e0fee clang::Sema::PerformPendingInstantiations(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x70e0fee)
#<!-- -->30 0x000000000664cd6d clang::Sema::ActOnEndOfTranslationUnitFragment(clang::Sema::TUFragmentKind) (.part.0) Sema.cpp:0:0
#<!-- -->31 0x000000000664d562 clang::Sema::ActOnEndOfTranslationUnit() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x664d562)
#<!-- -->32 0x00000000064b7dfb clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64b7dfb)
#<!-- -->33 0x00000000064aa1ba clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64aa1ba)
#<!-- -->34 0x000000000485a108 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x485a108)
#<!-- -->35 0x0000000004b211e5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b211e5)
#<!-- -->36 0x0000000004aa498e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4aa498e)
#<!-- -->37 0x0000000004c0fc3e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c0fc3e)
#<!-- -->38 0x0000000000d5c7ef cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd5c7ef)
#<!-- -->39 0x0000000000d5426a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->40 0x00000000048a0719 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->41 0x0000000003de2904 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3de2904)
#<!-- -->42 0x00000000048a0d0f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->43 0x0000000004863e8d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4863e8d)
#<!-- -->44 0x0000000004864f0e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4864f0e)
#<!-- -->45 0x000000000486cec5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x486cec5)
#<!-- -->46 0x0000000000d595e3 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd595e3)
#<!-- -->47 0x0000000000c2b854 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc2b854)
#<!-- -->48 0x00007d78da829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->49 0x00007d78da829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->50 0x0000000000d53d15 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd53d15)
```

&lt;/details&gt;


</details>


---

### Comment 2 - zyn0217

This seems to be a 20 regression

---

### Comment 3 - zyn0217

Unfortunately, I think fixing this requires deferring the instantiation of the lambda body. We crashed when trying to look for instantiated captures from the parent lambda, which aren't available until after the transformed parent LambdaExpr is built. However, that's not the case right now because we're in the middle of transforming the parent lambda's body!

@MagentaTreehouse is this derived from a real-world use case, or does it come from a fuzzer such that there's less risk of regressing it in 20?

---

### Comment 4 - MagentaTreehouse

> [@MagentaTreehouse](https://github.com/MagentaTreehouse) is this derived from a real-world use case, or does it come from a fuzzer such that there's less risk of regressing it in 20?

It is derived from an internal project I am working on, no fuzzer involved.

---

### Comment 5 - shafik

CC @cor3ntin 

---

