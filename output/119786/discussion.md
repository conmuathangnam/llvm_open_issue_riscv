# Crash under -Wunsafe-buffer-usage-in-libc-call when compiling snprintf()

**Author:** tsepez
**URL:** https://github.com/llvm/llvm-project/issues/119786
**Status:** Closed
**Labels:** clang:frontend, confirmed, crash
**Closed Date:** 2025-02-11T21:03:14Z

## Body

Given a program foo.cc:
```
#include <stdio.h>

constexpr size_t kSize = 10;

int main(int argc, char** argv) {
  char buf[kSize];
  snprintf(buf, kSize, "%c", 0x41);
  return 0;
}
```
compiling with -Wunsafe-buffer-usage-in-libc-call gives

```
$ third_party/llvm-build/Release+Asserts/bin/clang++ -c -emit-llvm -Xclang -disable-llvm-passes -Wunsafe-buffer-usage-in-libc-call  ~/foo.cc
clang++: [redacted]/third_party/llvm/clang/include/clang/AST/APValue.h:468: APSInt &clang::APValue::getInt(): Assertion `isInt() && "Invalid accessor"' failed.
0.	Program arguments: third_party/llvm-build/Release+Asserts/bin/clang++ -c -emit-llvm -Xclang -disable-llvm-passes -Wunsafe-buffer-usage-in-libc-call [redacted]foo.cc
1.	<eof> parser at end of file
#0 0x0000559fa1931108 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (third_party/llvm-build/Release+Asserts/bin/clang+++0x4fc9108)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
clang version 20.0.0git (https://chromium.googlesource.com/external/github.com/llvm/llvm-project 8cb44859cc31929521c09fc6a8add66d53db44de)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: [redacted]/third_party/llvm-build/Release+Asserts/bin
Build config: +assertions
clang++: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: /tmp/foo-82f592.cpp
clang++: note: diagnostic msg: /tmp/foo-82f592.sh
clang++: note: diagnostic msg: 

********************
```

## Comments

### Comment 1 - tsepez

Attempting to add the requested files now.  Getting an error about file type not allowed for .sh and .cpp. 

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Thomas Sepez (tsepez)

<details>
Given a program foo.cc:
```
#include &lt;stdio.h&gt;

constexpr size_t kSize = 10;

int main(int argc, char** argv) {
  char buf[kSize];
  snprintf(buf, kSize, "%c", 0x41);
  return 0;
}
```
compiling with -Wunsafe-buffer-usage-in-libc-call gives

```
$ third_party/llvm-build/Release+Asserts/bin/clang++ -c -emit-llvm -Xclang -disable-llvm-passes -Wunsafe-buffer-usage-in-libc-call  ~/foo.cc
clang++: [redacted]/third_party/llvm/clang/include/clang/AST/APValue.h:468: APSInt &amp;clang::APValue::getInt(): Assertion `isInt() &amp;&amp; "Invalid accessor"' failed.
0.	Program arguments: third_party/llvm-build/Release+Asserts/bin/clang++ -c -emit-llvm -Xclang -disable-llvm-passes -Wunsafe-buffer-usage-in-libc-call [redacted]foo.cc
1.	&lt;eof&gt; parser at end of file
#<!-- -->0 0x0000559fa1931108 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (third_party/llvm-build/Release+Asserts/bin/clang+++0x4fc9108)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
clang version 20.0.0git (https://chromium.googlesource.com/external/github.com/llvm/llvm-project 8cb44859cc31929521c09fc6a8add66d53db44de)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: [redacted]/third_party/llvm-build/Release+Asserts/bin
Build config: +assertions
clang++: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: /tmp/foo-82f592.cpp
clang++: note: diagnostic msg: /tmp/foo-82f592.sh
clang++: note: diagnostic msg: 

********************
```
</details>


---

### Comment 3 - shafik

Confirmed: https://godbolt.org/z/4YTjTdzjP

Assertion:

```console
clang++: /root/llvm-project/clang/include/clang/AST/APValue.h:468:
clang::APValue::APSInt& clang::APValue::getInt():
Assertion `isInt() && "Invalid accessor"' failed.
```

Backtrace:

```console
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -Wunsafe-buffer-usage-in-libc-call <source>
1.	<eof> parser at end of file
 #0 0x0000000003c19758 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3c19758)
 #1 0x0000000003c17464 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3c17464)
 #2 0x0000000003b649c8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x0000733737242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007337372969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x0000733737242476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007337372287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x000073373722871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x0000733737239e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000007276219 clang::ast_matchers::libc_func_matchers::internal::matcher_hasUnsafeSnprintfBufferMatcher::matches(clang::CallExpr const&, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7276219)
#10 0x00000000060f43fc clang::ast_matchers::internal::MatcherInterface<clang::CallExpr>::dynMatches(clang::DynTypedNode const&, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x60f43fc)
#11 0x00000000072f1723 clang::ast_matchers::internal::DynTypedMatcher::matchesNoKindCheck(clang::DynTypedNode const&, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x72f1723)
#12 0x00000000072f1a2b clang::ast_matchers::internal::(anonymous namespace)::VariadicMatcher<&clang::ast_matchers::internal::allOfVariadicOperator(clang::DynTypedNode const&, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*, llvm::ArrayRef<clang::ast_matchers::internal::DynTypedMatcher>)>::dynMatches(clang::DynTypedNode const&, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const ASTMatchersInternal.cpp:0:0
#13 0x00000000072f3051 clang::ast_matchers::internal::(anonymous namespace)::IdDynMatcher::dynMatches(clang::DynTypedNode const&, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const ASTMatchersInternal.cpp:0:0
#14 0x00000000072f1c60 clang::ast_matchers::internal::DynTypedMatcher::matches(clang::DynTypedNode const&, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x72f1c60)
#15 0x00000000072f45c9 clang::ast_matchers::internal::(anonymous namespace)::VariadicMatcher<&clang::ast_matchers::internal::anyOfVariadicOperator(clang::DynTypedNode const&, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*, llvm::ArrayRef<clang::ast_matchers::internal::DynTypedMatcher>)>::dynMatches(clang::DynTypedNode const&, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const ASTMatchersInternal.cpp:0:0
#16 0x00000000072f1723 clang::ast_matchers::internal::DynTypedMatcher::matchesNoKindCheck(clang::DynTypedNode const&, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x72f1723)
#17 0x00000000072f1a2b clang::ast_matchers::internal::(anonymous namespace)::VariadicMatcher<&clang::ast_matchers::internal::allOfVariadicOperator(clang::DynTypedNode const&, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*, llvm::ArrayRef<clang::ast_matchers::internal::DynTypedMatcher>)>::dynMatches(clang::DynTypedNode const&, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const ASTMatchersInternal.cpp:0:0
#18 0x00000000072f3051 clang::ast_matchers::internal::(anonymous namespace)::IdDynMatcher::dynMatches(clang::DynTypedNode const&, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const ASTMatchersInternal.cpp:0:0
#19 0x00000000072f1723 clang::ast_matchers::internal::DynTypedMatcher::matchesNoKindCheck(clang::DynTypedNode const&, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x72f1723)
#20 0x00000000072f1a0e clang::ast_matchers::internal::(anonymous namespace)::VariadicMatcher<&clang::ast_matchers::internal::allOfVariadicOperator(clang::DynTypedNode const&, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*, llvm::ArrayRef<clang::ast_matchers::internal::DynTypedMatcher>)>::dynMatches(clang::DynTypedNode const&, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const ASTMatchersInternal.cpp:0:0
#21 0x00000000072f1c60 clang::ast_matchers::internal::DynTypedMatcher::matches(clang::DynTypedNode const&, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x72f1c60)
#22 0x00000000072f45c9 clang::ast_matchers::internal::(anonymous namespace)::VariadicMatcher<&clang::ast_matchers::internal::anyOfVariadicOperator(clang::DynTypedNode const&, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*, llvm::ArrayRef<clang::ast_matchers::internal::DynTypedMatcher>)>::dynMatches(clang::DynTypedNode const&, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const ASTMatchersInternal.cpp:0:0
#23 0x00000000072f1c60 clang::ast_matchers::internal::DynTypedMatcher::matches(clang::DynTypedNode const&, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x72f1c60)
#24 0x000000000729c2ea clang::ast_matchers::MatchDescendantVisitor::TraverseStmt(clang::Stmt*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x729c2ea)
#25 0x00000000075ed762 clang::DynamicRecursiveASTVisitor::TraverseCompoundStmt(clang::CompoundStmt*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x75ed762)
#26 0x00000000075dfd8e clang::RecursiveASTVisitor<(anonymous namespace)::Impl>::TraverseStmt(clang::Stmt*, llvm::SmallVectorImpl<llvm::PointerIntPair<clang::Stmt*, 1u, bool, llvm::PointerLikeTypeTraits<clang::Stmt*>, llvm::PointerIntPairInfo<clang::Stmt*, 1u, llvm::PointerLikeTypeTraits<clang::Stmt*>>>>*) (.constprop.0) DynamicRecursiveASTVisitor.cpp:0:0
#27 0x00000000075dfed3 clang::DynamicRecursiveASTVisitor::TraverseStmt(clang::Stmt*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x75dfed3)
#28 0x000000000729de21 clang::ast_matchers::internal::matcher_forEachDescendantEvaluatedStmt0Matcher::matches(clang::Stmt const&, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x729de21)
#29 0x00000000060f01fc clang::ast_matchers::internal::MatcherInterface<clang::Stmt>::dynMatches(clang::DynTypedNode const&, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x60f01fc)
#30 0x00000000072f1c60 clang::ast_matchers::internal::DynTypedMatcher::matches(clang::DynTypedNode const&, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x72f1c60)
#31 0x00000000072a97c3 clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor::matchWithFilter(clang::DynTypedNode const&) ASTMatchFinder.cpp:0:0
#32 0x00000000072a9ce9 clang::ast_matchers::MatchFinder::match(clang::DynTypedNode const&, clang::ASTContext&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x72a9ce9)
#33 0x000000000728ec0c findGadgets(clang::Stmt const*, clang::ASTContext&, clang::UnsafeBufferUsageHandler const&, bool, std::vector<std::unique_ptr<(anonymous namespace)::FixableGadget, std::default_delete<(anonymous namespace)::FixableGadget>>, std::allocator<std::unique_ptr<(anonymous namespace)::FixableGadget, std::default_delete<(anonymous namespace)::FixableGadget>>>>&, std::vector<std::unique_ptr<(anonymous namespace)::WarningGadget, std::default_delete<(anonymous namespace)::WarningGadget>>, std::allocator<std::unique_ptr<(anonymous namespace)::WarningGadget, std::default_delete<(anonymous namespace)::WarningGadget>>>>&, (anonymous namespace)::DeclUseTracker&) UnsafeBufferUsage.cpp:0:0
#34 0x000000000729bc0c clang::checkUnsafeBufferUsage(clang::Decl const*, clang::UnsafeBufferUsageHandler&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x729bc0c)
#35 0x000000000715c849 CallableVisitor::VisitFunctionDecl(clang::FunctionDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x715c849)
#36 0x000000000762829d clang::DynamicRecursiveASTVisitor::TraverseFunctionDecl(clang::FunctionDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x762829d)
#37 0x0000000007621f6a clang::RecursiveASTVisitor<(anonymous namespace)::Impl>::TraverseDeclContextHelper(clang::DeclContext*) (.part.0) DynamicRecursiveASTVisitor.cpp:0:0
#38 0x000000000762ccfa clang::DynamicRecursiveASTVisitor::TraverseTranslationUnitDecl(clang::TranslationUnitDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x762ccfa)
#39 0x00000000071626d4 clang::sema::AnalysisBasedWarnings::IssueWarnings(clang::TranslationUnitDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x71626d4)
#40 0x00000000066f9266 clang::Sema::ActOnEndOfTranslationUnit() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66f9266)
#41 0x00000000065649cb clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65649cb)
#42 0x0000000006556e2a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6556e2a)
#43 0x000000000459e458 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x459e458)
#44 0x0000000004856fc9 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4856fc9)
#45 0x00000000047d989e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x47d989e)
#46 0x000000000494267e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x494267e)
#47 0x0000000000cb92df cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xcb92df)
#48 0x0000000000cb0d7a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#49 0x00000000045e2229 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#50 0x0000000003b64e74 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3b64e74)
#51 0x00000000045e281f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#52 0x00000000045a7f3d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x45a7f3d)
#53 0x00000000045a902d clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x45a902d)
#54 0x00000000045b0565 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x45b0565)
#55 0x0000000000cb6123 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xcb6123)
#56 0x0000000000b84564 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xb84564)
#57 0x0000733737229d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#58 0x0000733737229e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#59 0x0000000000cb0825 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xcb0825)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

---

### Comment 4 - shafik

CC @jkorous-apple @ldionne 

---

### Comment 5 - tsepez

The following diff seems to resolve the problem on my local build, namely, that EvaluateAsConstantExpr() was returning a result of kind lvalue (7) which wasn't compatible with getInt()
```
diff --git a/clang/lib/Analysis/UnsafeBufferUsage.cpp b/clang/lib/Analysis/UnsafeBufferUsage.cpp
index bef5fa8624ce..d4dcb5a0b3a5 100644
--- a/clang/lib/Analysis/UnsafeBufferUsage.cpp
+++ b/clang/lib/Analysis/UnsafeBufferUsage.cpp
@@ -928,9 +928,8 @@ AST_MATCHER(CallExpr, hasUnsafeSnprintfBuffer) {
       // The array element type must be compatible with `char` otherwise an
       // explicit cast will be needed, which will make this check unreachable.
       // Therefore, the array extent is same as its' bytewise size.
-      if (Size->EvaluateAsConstantExpr(ER, Ctx)) {
+      if (Size->EvaluateAsInt(ER, Ctx)) {
         APSInt EVal = ER.Val.getInt(); // Size must have integer type
-
         return APSInt::compareValues(EVal, APSInt(CAT->getSize(), true)) != 0;
       }
     }
```

Would this be acceptable?  Let me know, and if so, I'll try to figure out how to upload a patch.  Thanks!

---

### Comment 6 - shafik

@tsepez 

I think maybe the `ValueKind` is not a `Int` but an `LValue` and so maybe `toIntegralConstant()` is the correct way to do this as opposed to `ER.Val.getInt()`.



---

