# Pack expansion with a lambda as a pattern leads to ICE

**Author:** ddvamp
**URL:** https://github.com/llvm/llvm-project/issues/161780
**Status:** Closed
**Labels:** duplicate, clang:frontend, crash, lambda
**Closed Date:** 2025-10-03T06:08:11Z

## Body

On godbolt, x86-64 clang trunk (22.0)

```cpp
template <typename ...Ts>
void foo(Ts ...ts) {
  [ts...]<typename ...Us>(Us ...us) {
    ([t = ts, u = us]{}, ...); // OK
    ([t = ts, us]{}, ...); // OK
    ([ts, u = us]{}, ...); // ICE
    ([ts, us]{}, ...); // ICE
  };
}

int main() {
  foo(0);
  return 0;
}
```
**Output:**
```text
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -g -o /app/output.s -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics --std=c++20 <source>
1.	<eof> parser at end of file
2.	<source>:2:6: instantiating function definition 'foo<int>'
 #0 0x0000000003c9a4f8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c9a4f8)
 #1 0x0000000003c97ecc llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c97ecc)
 #2 0x0000000003be7ca8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x0000775bcd242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x0000000006d964d4 clang::Sema::FindInstantiatedDecl(clang::SourceLocation, clang::NamedDecl*, clang::MultiLevelTemplateArgumentList const&, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6d964d4)
 #5 0x0000000006d16d50 (anonymous namespace)::TemplateInstantiator::TransformDecl(clang::SourceLocation, clang::Decl*) SemaTemplateInstantiate.cpp:0:0
 #6 0x0000000006d426cb clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformLambdaExpr(clang::LambdaExpr*) SemaTemplateInstantiate.cpp:0:0
 #7 0x0000000006d00690 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
 #8 0x0000000006d221b2 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCXXFoldExpr(clang::CXXFoldExpr*) SemaTemplateInstantiate.cpp:0:0
 #9 0x0000000006cffd69 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#10 0x0000000006d3a849 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformStmt(clang::Stmt*, clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::StmtDiscardKind) SemaTemplateInstantiate.cpp:0:0
#11 0x0000000006d3ae95 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaTemplateInstantiate.cpp:0:0
#12 0x0000000006d437ea clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformLambdaExpr(clang::LambdaExpr*) SemaTemplateInstantiate.cpp:0:0
#13 0x0000000006d00690 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#14 0x0000000006d3a849 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformStmt(clang::Stmt*, clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::StmtDiscardKind) SemaTemplateInstantiate.cpp:0:0
#15 0x0000000006d3ae95 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaTemplateInstantiate.cpp:0:0
#16 0x0000000006d43c7b clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6d43c7b)
#17 0x0000000006d9e2fa clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6d9e2fa)
#18 0x0000000006d9c5a8 clang::Sema::PerformPendingInstantiations(bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6d9c5a8)
#19 0x0000000006392c69 clang::Sema::ActOnEndOfTranslationUnitFragment(clang::TUFragmentKind) (.part.0) Sema.cpp:0:0
#20 0x0000000006392f44 clang::Sema::ActOnEndOfTranslationUnit() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6392f44)
#21 0x00000000061f188f clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61f188f)
#22 0x00000000061c9aea clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61c9aea)
#23 0x0000000004567685 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4567685)
#24 0x00000000048636da clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x48636da)
#25 0x00000000047e326b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x47e326b)
#26 0x00000000049597cb clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x49597cb)
#27 0x0000000000dc91c5 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdc91c5)
#28 0x0000000000dc111b ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#29 0x0000000000dc11bd int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#30 0x00000000045d4f29 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#31 0x0000000003be80c3 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3be80c3)
#32 0x00000000045d5149 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#33 0x0000000004598392 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4598392)
#34 0x0000000004599271 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4599271)
#35 0x00000000045a1dbc clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45a1dbc)
#36 0x0000000000dc5c19 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdc5c19)
#37 0x0000000000c72ea4 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc72ea4)
#38 0x0000775bcd229d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#39 0x0000775bcd229e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#40 0x0000000000dc0bb5 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdc0bb5)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Artyom Kolpakov (ddvamp)

<details>
On godbolt, x86-64 clang trunk (22.0)

```cpp
template &lt;typename ...Ts&gt;
void foo(Ts ...ts) {
  [ts...]&lt;typename ...Us&gt;(Us ...us) {
    ([t = ts, u = us]{}, ...); // OK
    ([t = ts, us]{}, ...); // OK
    ([ts, u = us]{}, ...); // ICE
    ([ts, us]{}, ...); // ICE
  };
}

int main() {
  foo(0);
  return 0;
}
```
**Output:**
```text
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -g -o /app/output.s -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics --std=c++20 &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	&lt;source&gt;:2:6: instantiating function definition 'foo&lt;int&gt;'
 #<!-- -->0 0x0000000003c9a4f8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c9a4f8)
 #<!-- -->1 0x0000000003c97ecc llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c97ecc)
 #<!-- -->2 0x0000000003be7ca8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x0000775bcd242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x0000000006d964d4 clang::Sema::FindInstantiatedDecl(clang::SourceLocation, clang::NamedDecl*, clang::MultiLevelTemplateArgumentList const&amp;, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6d964d4)
 #<!-- -->5 0x0000000006d16d50 (anonymous namespace)::TemplateInstantiator::TransformDecl(clang::SourceLocation, clang::Decl*) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->6 0x0000000006d426cb clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformLambdaExpr(clang::LambdaExpr*) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->7 0x0000000006d00690 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->8 0x0000000006d221b2 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCXXFoldExpr(clang::CXXFoldExpr*) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->9 0x0000000006cffd69 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->10 0x0000000006d3a849 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformStmt(clang::Stmt*, clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::StmtDiscardKind) SemaTemplateInstantiate.cpp:0:0
#<!-- -->11 0x0000000006d3ae95 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaTemplateInstantiate.cpp:0:0
#<!-- -->12 0x0000000006d437ea clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformLambdaExpr(clang::LambdaExpr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->13 0x0000000006d00690 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->14 0x0000000006d3a849 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformStmt(clang::Stmt*, clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::StmtDiscardKind) SemaTemplateInstantiate.cpp:0:0
#<!-- -->15 0x0000000006d3ae95 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaTemplateInstantiate.cpp:0:0
#<!-- -->16 0x0000000006d43c7b clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6d43c7b)
#<!-- -->17 0x0000000006d9e2fa clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6d9e2fa)
#<!-- -->18 0x0000000006d9c5a8 clang::Sema::PerformPendingInstantiations(bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6d9c5a8)
#<!-- -->19 0x0000000006392c69 clang::Sema::ActOnEndOfTranslationUnitFragment(clang::TUFragmentKind) (.part.0) Sema.cpp:0:0
#<!-- -->20 0x0000000006392f44 clang::Sema::ActOnEndOfTranslationUnit() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6392f44)
#<!-- -->21 0x00000000061f188f clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61f188f)
#<!-- -->22 0x00000000061c9aea clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61c9aea)
#<!-- -->23 0x0000000004567685 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4567685)
#<!-- -->24 0x00000000048636da clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x48636da)
#<!-- -->25 0x00000000047e326b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x47e326b)
#<!-- -->26 0x00000000049597cb clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x49597cb)
#<!-- -->27 0x0000000000dc91c5 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdc91c5)
#<!-- -->28 0x0000000000dc111b ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->29 0x0000000000dc11bd int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->30 0x00000000045d4f29 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->31 0x0000000003be80c3 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3be80c3)
#<!-- -->32 0x00000000045d5149 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->33 0x0000000004598392 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4598392)
#<!-- -->34 0x0000000004599271 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4599271)
#<!-- -->35 0x00000000045a1dbc clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45a1dbc)
#<!-- -->36 0x0000000000dc5c19 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdc5c19)
#<!-- -->37 0x0000000000c72ea4 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc72ea4)
#<!-- -->38 0x0000775bcd229d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->39 0x0000775bcd229e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->40 0x0000000000dc0bb5 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdc0bb5)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
```
</details>


---

### Comment 2 - cor3ntin

This is #161002

---

