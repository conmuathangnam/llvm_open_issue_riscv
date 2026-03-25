# [clang] Assertion `BaseTy->castAs<RecordType>()->getDecl()->getCanonicalDecl() == FD->getParent()->getCanonicalDecl() && "record / field mismatch"' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/140370

## Body

Reproducer:
https://godbolt.org/z/a3jnKEz54
```cpp
template <class T> void f(T);

template <class U> class C {
  template <class T> friend void f(T) {
    C<U> c;
    c.i = 0;
  }

  int i;
};

int main() {
  f(0);
  C<float> c;
}
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/AST/ExprConstant.cpp:8734: bool {anonymous}::LValueExprEvaluatorBase<Derived>::VisitMemberExpr(const clang::MemberExpr*) [with Derived = {anonymous}::LValueExprEvaluator]: Assertion `BaseTy->castAs<RecordType>()->getDecl()->getCanonicalDecl() == FD->getParent()->getCanonicalDecl() && "record / field mismatch"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<eof> parser at end of file
2.	<source>:4:34: instantiating function definition 'f<int>'
 #0 0x0000000003f9bf28 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f9bf28)
 #1 0x0000000003f99bb4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f99bb4)
 #2 0x0000000003ede7e8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007cc446442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007cc4464969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007cc446442476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007cc4464287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007cc44642871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007cc446439e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x00000000079238dd (anonymous namespace)::LValueExprEvaluator::VisitMemberExpr(clang::MemberExpr const*) ExprConstant.cpp:0:0
#10 0x00000000078fdb2c clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::LValueExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#11 0x00000000078fe65c clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::LValueExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#12 0x00000000078ff06d EvaluateLValue(clang::Expr const*, (anonymous namespace)::LValue&, (anonymous namespace)::EvalInfo&, bool) ExprConstant.cpp:0:0
#13 0x00000000078d2a46 Evaluate(clang::APValue&, (anonymous namespace)::EvalInfo&, clang::Expr const*) ExprConstant.cpp:0:0
#14 0x00000000078da90b EvaluateAsRValue((anonymous namespace)::EvalInfo&, clang::Expr const*, clang::APValue&) ExprConstant.cpp:0:0
#15 0x00000000078dcc5c clang::Expr::EvaluateForOverflow(clang::ASTContext const&) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x78dcc5c)
#16 0x0000000006882696 clang::Sema::CheckForIntOverflow(clang::Expr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6882696)
#17 0x00000000068c1e15 clang::Sema::CheckCompletedExpr(clang::Expr*, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68c1e15)
#18 0x0000000006c72aec clang::Sema::ActOnFinishFullExpr(clang::Expr*, clang::SourceLocation, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c72aec)
#19 0x0000000006fd023e clang::Sema::ActOnExprStmt(clang::ActionResult<clang::Expr*, true>, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6fd023e)
#20 0x000000000722f10e clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaTemplateInstantiate.cpp:0:0
#21 0x0000000007238434 clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7238434)
#22 0x0000000007297bc7 clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7297bc7)
#23 0x0000000007295a16 clang::Sema::PerformPendingInstantiations(bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7295a16)
#24 0x00000000067e0642 clang::Sema::ActOnEndOfTranslationUnitFragment(clang::TUFragmentKind) (.part.0) Sema.cpp:0:0
#25 0x00000000067e0ee2 clang::Sema::ActOnEndOfTranslationUnit() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67e0ee2)
#26 0x000000000664130b clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x664130b)
#27 0x00000000066335fa clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66335fa)
#28 0x00000000049478f8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49478f8)
#29 0x0000000004c3f825 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c3f825)
#30 0x0000000004bbe4ee clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bbe4ee)
#31 0x0000000004d32309 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d32309)
#32 0x0000000000daceaf cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdaceaf)
#33 0x0000000000da307a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#34 0x00000000049aebe9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#35 0x0000000003edec84 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3edec84)
#36 0x00000000049af1ff clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#37 0x0000000004970fcd clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4970fcd)
#38 0x000000000497205e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x497205e)
#39 0x000000000497a345 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x497a345)
#40 0x0000000000da8e18 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda8e18)
#41 0x0000000000c2e464 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc2e464)
#42 0x00007cc446429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#43 0x00007cc446429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#44 0x0000000000da2b25 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda2b25)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/a3jnKEz54
```cpp
template &lt;class T&gt; void f(T);

template &lt;class U&gt; class C {
  template &lt;class T&gt; friend void f(T) {
    C&lt;U&gt; c;
    c.i = 0;
  }

  int i;
};

int main() {
  f(0);
  C&lt;float&gt; c;
}
```
</details>


---

### Comment 2 - EugeneZelenko

@k-arrows: You could set labels and type during issues creation, so default labels will not be added. This will save a little bit of time for you and reduce number of notifications.

---

### Comment 3 - shafik

Goes back to clang-19: https://godbolt.org/z/W4o1hqnd3

---

