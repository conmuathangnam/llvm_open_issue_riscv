# z3 refutation crash - fatal error: error in backend: Z3 error: operator is applied to arguments of the wrong sort

**Author:** vabridgers
**URL:** https://github.com/llvm/llvm-project/issues/165779
**Status:** Open
**Labels:** clang:static analyzer, crash

## Body


The simple reproducer


```
void z3_crash(long a) {
  if (~-(5 && a)) {
    long *c;
    *c; // expected-warning{{Dereference of undefined pointer value (loaded from variable 'c')}}
  }
}
```


To run: 

```clang --analyze -Xclang -analyzer-config -Xclang crosscheck-with-z3=true z3.c ```

The crashstack
```
fatal error: error in backend: Z3 error: operator is applied to arguments of the wrong sort
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: clang --analyze -Xclang -analyzer-config -Xclang crosscheck-with-z3=true z3.c
1.      <eof> parser at end of file
 #0 0x0000000004163fa8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (clang+0x4163fa8)
 #1 0x00000000041613cc llvm::sys::CleanupOnSignal(unsigned long) (clang+0x41613cc)
 #2 0x00000000040a5cc6 llvm::CrashRecoveryContext::HandleExit(int) (clang+0x40a5cc6)
 #3 0x000000000415889e llvm::sys::Process::Exit(int, bool) (clang+0x415889e)
 #4 0x0000000000dc1e10 LLVMErrorHandler(void*, char const*, bool) cc1_main.cpp:0:0
 #5 0x00000000040b0fc3 llvm::report_fatal_error(llvm::Twine const&, bool) (clang+0x40b0fc3)
 #6 0x000000000879cb93 (anonymous namespace)::Z3ErrorHandler(_Z3_context*, Z3_error_code) Z3Solver.cpp:0:0
 #7 0x00007f89c4d8e0bd Z3_mk_bvnot (libz3.so+0x12a0bd)
 #8 0x000000000879ac22 (anonymous namespace)::Z3Solver::mkBVNot(llvm::SMTExpr const* const&) Z3Solver.cpp:0:0
 #9 0x0000000006481892 clang::ento::SMTConv::getSymExpr(std::shared_ptr<llvm::SMTSolver>&, clang::ASTContext&, clang::ento::SymExpr const*, clang::QualType*, bool*) (clang+0x6481892)
#10 0x00000000064a5efd clang::ento::SMTConv::getRangeExpr(std::shared_ptr<llvm::SMTSolver>&, clang::ASTContext&, clang::ento::SymExpr const*, llvm::APSInt const&, llvm::APSInt const&, bool) (.constprop.0) Z3CrosscheckVisitor.cpp:0:0
#11 0x00000000064ab72f clang::ento::Z3CrosscheckVisitor::finalizeVisitor(clang::ento::BugReporterContext&, clang::ento::ExplodedNode const*, clang::ento::PathSensitiveBugReport&) (clang+0x64ab72f)
#12 0x000000000631cd54 generateVisitorsDiagnostics(clang::ento::PathSensitiveBugReport*, clang::ento::ExplodedNode const*, clang::ento::BugReporterContext&) BugReporter.cpp:0:0
#13 0x000000000631f7c8 (anonymous namespace)::PathDiagnosticBuilder::findValidReport(llvm::ArrayRef<clang::ento::PathSensitiveBugReport*>&, clang::ento::PathSensitiveBugReporter&) BugReporter.cpp:0:0
#14 0x0000000006327720 clang::ento::PathSensitiveBugReporter::generatePathDiagnostics(llvm::ArrayRef<std::unique_ptr<clang::ento::PathDiagnosticConsumer, std::default_delete<clang::ento::PathDiagnosticConsumer>>>, llvm::ArrayRef<clang::ento::PathSensitiveBugReport*>&) (clang+0x6327720)
#15 0x0000000006328221 clang::ento::PathSensitiveBugReporter::generateDiagnosticForConsumerMap(clang::ento::BugReport*, llvm::ArrayRef<std::unique_ptr<clang::ento::PathDiagnosticConsumer, std::default_delete<clang::ento::PathDiagnosticConsumer>>>, llvm::ArrayRef<clang::ento::BugReport*>) (clang+0x6328221)
#16 0x00000000063246f7 clang::ento::BugReporter::FlushReport(clang::ento::BugReportEquivClass&) (clang+0x63246f7)
#17 0x000000000632570f clang::ento::BugReporter::FlushReports() (clang+0x632570f)
#18 0x0000000005f1e3c0 (anonymous namespace)::AnalysisConsumer::HandleCode(clang::Decl*, unsigned int, clang::ento::ExprEngine::InliningModes, llvm::DenseSet<clang::Decl const*, llvm::DenseMapInfo<clang::Decl const*, void>>*) AnalysisConsumer.cpp:0:0
#19 0x0000000005f1fc2f (anonymous namespace)::AnalysisConsumer::HandleTranslationUnit(clang::ASTContext&) AnalysisConsumer.cpp:0:0
#20 0x00000000064ee7dc clang::ParseAST(clang::Sema&, bool, bool) (clang+0x64ee7dc)
#21 0x0000000004daeaf5 clang::FrontendAction::Execute() (clang+0x4daeaf5)
#22 0x0000000004d2fc7e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (clang+0x4d2fc7e)
#23 0x0000000004ea788d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (clang+0x4ea788d)
#24 0x0000000000dc4540 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (clang+0xdc4540)
#25 0x0000000000dbb0ea ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#26 0x0000000000dbb26d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#27 0x0000000004b2b169 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#28 0x00000000040a5c04 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (clang+0x40a5c04)
#29 0x0000000004b2b77f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#30 0x0000000004aec5c2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (clang+0x4aec5c2)
#31 0x0000000004aed56e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (clang+0x4aed56e)
#32 0x0000000004af4cc5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (clang+0x4af4cc5)
#33 0x0000000000dc0a31 clang_main(int, char**, llvm::ToolContext const&) (clang+0xdc0a31)
#34 0x0000000000c7eea4 main (clang+0xc7eea4)
#35 0x00007f89c3a937e5 __libc_start_main (/lib64/libc.so.6+0x3a7e5)
#36 0x0000000000dbab8e _start (clang+0xdbab8e)
clang: error: clang frontend command failed with exit code 70 (use -v to see invocation)
clang version 22.0.0git (https://github.com/llvm/llvm-project.git 87616939190b1c0d322f0f3c1d69ba3626d18582)
Target: x86_64-unknown-linux-gnu
Thread model: posix

```



## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-static-analyzer

Author: None (vabridgers)

<details>

The simple reproducer


```
void z3_crash(long a) {
  if (~-(5 &amp;&amp; a)) {
    long *c;
    *c; // expected-warning{{Dereference of undefined pointer value (loaded from variable 'c')}}
  }
}
```


To run: 

```clang --analyze -Xclang -analyzer-config -Xclang crosscheck-with-z3=true z3.c ```

The crashstack
```
fatal error: error in backend: Z3 error: operator is applied to arguments of the wrong sort
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: clang --analyze -Xclang -analyzer-config -Xclang crosscheck-with-z3=true z3.c
1.      &lt;eof&gt; parser at end of file
 #<!-- -->0 0x0000000004163fa8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (clang+0x4163fa8)
 #<!-- -->1 0x00000000041613cc llvm::sys::CleanupOnSignal(unsigned long) (clang+0x41613cc)
 #<!-- -->2 0x00000000040a5cc6 llvm::CrashRecoveryContext::HandleExit(int) (clang+0x40a5cc6)
 #<!-- -->3 0x000000000415889e llvm::sys::Process::Exit(int, bool) (clang+0x415889e)
 #<!-- -->4 0x0000000000dc1e10 LLVMErrorHandler(void*, char const*, bool) cc1_main.cpp:0:0
 #<!-- -->5 0x00000000040b0fc3 llvm::report_fatal_error(llvm::Twine const&amp;, bool) (clang+0x40b0fc3)
 #<!-- -->6 0x000000000879cb93 (anonymous namespace)::Z3ErrorHandler(_Z3_context*, Z3_error_code) Z3Solver.cpp:0:0
 #<!-- -->7 0x00007f89c4d8e0bd Z3_mk_bvnot (libz3.so+0x12a0bd)
 #<!-- -->8 0x000000000879ac22 (anonymous namespace)::Z3Solver::mkBVNot(llvm::SMTExpr const* const&amp;) Z3Solver.cpp:0:0
 #<!-- -->9 0x0000000006481892 clang::ento::SMTConv::getSymExpr(std::shared_ptr&lt;llvm::SMTSolver&gt;&amp;, clang::ASTContext&amp;, clang::ento::SymExpr const*, clang::QualType*, bool*) (clang+0x6481892)
#<!-- -->10 0x00000000064a5efd clang::ento::SMTConv::getRangeExpr(std::shared_ptr&lt;llvm::SMTSolver&gt;&amp;, clang::ASTContext&amp;, clang::ento::SymExpr const*, llvm::APSInt const&amp;, llvm::APSInt const&amp;, bool) (.constprop.0) Z3CrosscheckVisitor.cpp:0:0
#<!-- -->11 0x00000000064ab72f clang::ento::Z3CrosscheckVisitor::finalizeVisitor(clang::ento::BugReporterContext&amp;, clang::ento::ExplodedNode const*, clang::ento::PathSensitiveBugReport&amp;) (clang+0x64ab72f)
#<!-- -->12 0x000000000631cd54 generateVisitorsDiagnostics(clang::ento::PathSensitiveBugReport*, clang::ento::ExplodedNode const*, clang::ento::BugReporterContext&amp;) BugReporter.cpp:0:0
#<!-- -->13 0x000000000631f7c8 (anonymous namespace)::PathDiagnosticBuilder::findValidReport(llvm::ArrayRef&lt;clang::ento::PathSensitiveBugReport*&gt;&amp;, clang::ento::PathSensitiveBugReporter&amp;) BugReporter.cpp:0:0
#<!-- -->14 0x0000000006327720 clang::ento::PathSensitiveBugReporter::generatePathDiagnostics(llvm::ArrayRef&lt;std::unique_ptr&lt;clang::ento::PathDiagnosticConsumer, std::default_delete&lt;clang::ento::PathDiagnosticConsumer&gt;&gt;&gt;, llvm::ArrayRef&lt;clang::ento::PathSensitiveBugReport*&gt;&amp;) (clang+0x6327720)
#<!-- -->15 0x0000000006328221 clang::ento::PathSensitiveBugReporter::generateDiagnosticForConsumerMap(clang::ento::BugReport*, llvm::ArrayRef&lt;std::unique_ptr&lt;clang::ento::PathDiagnosticConsumer, std::default_delete&lt;clang::ento::PathDiagnosticConsumer&gt;&gt;&gt;, llvm::ArrayRef&lt;clang::ento::BugReport*&gt;) (clang+0x6328221)
#<!-- -->16 0x00000000063246f7 clang::ento::BugReporter::FlushReport(clang::ento::BugReportEquivClass&amp;) (clang+0x63246f7)
#<!-- -->17 0x000000000632570f clang::ento::BugReporter::FlushReports() (clang+0x632570f)
#<!-- -->18 0x0000000005f1e3c0 (anonymous namespace)::AnalysisConsumer::HandleCode(clang::Decl*, unsigned int, clang::ento::ExprEngine::InliningModes, llvm::DenseSet&lt;clang::Decl const*, llvm::DenseMapInfo&lt;clang::Decl const*, void&gt;&gt;*) AnalysisConsumer.cpp:0:0
#<!-- -->19 0x0000000005f1fc2f (anonymous namespace)::AnalysisConsumer::HandleTranslationUnit(clang::ASTContext&amp;) AnalysisConsumer.cpp:0:0
#<!-- -->20 0x00000000064ee7dc clang::ParseAST(clang::Sema&amp;, bool, bool) (clang+0x64ee7dc)
#<!-- -->21 0x0000000004daeaf5 clang::FrontendAction::Execute() (clang+0x4daeaf5)
#<!-- -->22 0x0000000004d2fc7e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (clang+0x4d2fc7e)
#<!-- -->23 0x0000000004ea788d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (clang+0x4ea788d)
#<!-- -->24 0x0000000000dc4540 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (clang+0xdc4540)
#<!-- -->25 0x0000000000dbb0ea ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->26 0x0000000000dbb26d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->27 0x0000000004b2b169 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->28 0x00000000040a5c04 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (clang+0x40a5c04)
#<!-- -->29 0x0000000004b2b77f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->30 0x0000000004aec5c2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (clang+0x4aec5c2)
#<!-- -->31 0x0000000004aed56e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (clang+0x4aed56e)
#<!-- -->32 0x0000000004af4cc5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (clang+0x4af4cc5)
#<!-- -->33 0x0000000000dc0a31 clang_main(int, char**, llvm::ToolContext const&amp;) (clang+0xdc0a31)
#<!-- -->34 0x0000000000c7eea4 main (clang+0xc7eea4)
#<!-- -->35 0x00007f89c3a937e5 __libc_start_main (/lib64/libc.so.6+0x3a7e5)
#<!-- -->36 0x0000000000dbab8e _start (clang+0xdbab8e)
clang: error: clang frontend command failed with exit code 70 (use -v to see invocation)
clang version 22.0.0git (https://github.com/llvm/llvm-project.git 87616939190b1c0d322f0f3c1d69ba3626d18582)
Target: x86_64-unknown-linux-gnu
Thread model: posix

```


</details>


---

### Comment 2 - Snape3058

https://github.com/llvm/llvm-project/blob/f8b5f86cd9c1a6b10c7cf161075eae5b32e18126/clang/include/clang/StaticAnalyzer/Core/PathSensitive/SMTConv.h#L461-L469
When handling `-((reg_$1<long a>) != 0)`, a boolean expression `(not (= reg_$1 #x0000000000000000))` is hence returned without handling the negation, where the BVNot operator on a boolean value is not allowed.

I think the solution should be resolving the TODO here by implementing the negation.

---

### Comment 3 - Snape3058

I also tried replacing the condition `~-(5 &&a)` with `(5 && a) + 1` (SI or IS) and `(a > 5) > (a < 5)` (SS), and both also crash.
Hence, I am considering adding a cast from boolean to int for both unary and binary operators:

* doTypeConversion should consider the case when either hand is boolean but the operator needs an integer.
* getSymExpr (unary handler branch) should consider the case when the operand is boolean but the operator needs an integer.

---

### Comment 4 - NagyDonat

@Snape3058 As a reviewer of the commit that added the above-mentioned TODO (https://github.com/llvm/llvm-project/pull/158276) I'm grateful for your investigation. Your plans offer a promising solution (instead of the hotfix in the previous commit) and it would be very welcome if you could develop them into a PR. Unfortunately, I don't have a deep understanding of this area (invoking Z3 for refutation), but I'll try to help with reviews etc. as far as I can.

---

### Comment 5 - Snape3058

@NagyDonat Thank you for your reply. But I have very limited time this week. I will be back working on this issue next week.
Since this TODO was added as a hotfix, I think I can resolve this bug without worrying about breaking something else.
I will cc the original reviewers introducing the TODO for reviewing.

---

### Comment 6 - Snape3058

I have posted a pr in #168034, which fixes only the example in this issue.
I previously mentioned two other expressions. However, I cannot reproduce them now.
After checking the source code, I think the binary operators are correct and do not need to be fixed.
So the proposed fix only handles the unary operators.

BTW, please note that, during the fix, I also tried expression `-(_Bool)a`, which will still trigger a crash even with the current patch.
For this expression, I think it is a problem with the symbolic expression. We can discuss that in the PR.

---

### Comment 7 - steakhal

> BTW, please note that, during the fix, I also tried expression -(_Bool)a, which will still trigger a crash even with the current patch.
For this expression, I think it is a problem with the symbolic expression. We can discuss that in the PR.

Please bring this up at the PR.
We could also put a new test and XFAIL it to document the presence of a known crash.

---

### Comment 8 - Snape3058

@steakhal 

> Please bring this up at the PR.

Do you mean I should put this into the commit message or the test case?
I have already mentioned that in the discussion of the PR.

---

