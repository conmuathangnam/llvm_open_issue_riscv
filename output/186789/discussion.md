# [clang-tidy] `performance-use-std-move` crash

**Author:** firewave
**URL:** https://github.com/llvm/llvm-project/issues/186789
**Status:** Open
**Labels:** clang-tidy, regression, incomplete, crash

## Body

From https://github.com/danmar/cppcheck/pull/8343.

```
Stack dump:
0.	Program arguments: clang-tidy-23 -p=/home/runner/work/cppcheck/cppcheck/cmake.output -quiet --enable-check-profile --store-check-profile=/tmp/tmpnnlxy74g /home/runner/work/cppcheck/cppcheck/gui/showtypes.cpp
1.	<eof> parser at end of file
2.	ASTMatcher: Matching 'performance-use-std-move' against:
	CXXOperatorCallExpr : </home/runner/work/cppcheck/cppcheck/gui/codeeditor.h:115:9, col:21>
Stack dump without symbol names (ensure you have llvm-symbolizer in your PATH or set the environment var `LLVM_SYMBOLIZER_PATH` to point to it):
0  libLLVM.so.23.0      0x00007faa92355db6 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) + 54
1  libLLVM.so.23.0      0x00007faa92353723 llvm::sys::RunSignalHandlers() + 131
2  libLLVM.so.23.0      0x00007faa92356b54
3  libc.so.6            0x00007faa8cc42520
4  clang-tidy-23        0x0000562f34b95630
5  libclang-cpp.so.23.0 0x00007faa986d7fb3 clang::ast_matchers::internal::DynTypedMatcher::matches(clang::DynTypedNode const&, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const + 243
6  clang-tidy-23        0x0000562f34645564
7  libclang-cpp.so.23.0 0x00007faa986d7fb3 clang::ast_matchers::internal::DynTypedMatcher::matches(clang::DynTypedNode const&, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const + 243
8  clang-tidy-23        0x0000562f3465963a clang::ast_matchers::internal::matcher_hasType1Matcher<clang::Expr, clang::ast_matchers::internal::Matcher<clang::Decl>>::matches(clang::Expr const&, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const + 218
9  libclang-cpp.so.23.0 0x00007faa986d7fb3 clang::ast_matchers::internal::DynTypedMatcher::matches(clang::DynTypedNode const&, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const + 243
10 clang-tidy-23        0x0000562f34659a6d clang::ast_matchers::internal::matcher_hasArgument0Matcher<clang::CXXOperatorCallExpr, unsigned int, clang::ast_matchers::internal::Matcher<clang::Expr>>::matches(clang::CXXOperatorCallExpr const&, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const + 141
11 libclang-cpp.so.23.0 0x00007faa986d80f2 clang::ast_matchers::internal::DynTypedMatcher::matchesNoKindCheck(clang::DynTypedNode const&, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const + 226
12 libclang-cpp.so.23.0 0x00007faa986dc52b
13 libclang-cpp.so.23.0 0x00007faa986db028
14 libclang-cpp.so.23.0 0x00007faa986d7fb3 clang::ast_matchers::internal::DynTypedMatcher::matches(clang::DynTypedNode const&, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const + 243
15 libclang-cpp.so.23.0 0x00007faa986a5d19
16 libclang-cpp.so.23.0 0x00007faa986b43cc
17 libclang-cpp.so.23.0 0x00007faa986ced1a
18 libclang-cpp.so.23.0 0x00007faa986ba992
19 libclang-cpp.so.23.0 0x00007faa986b43da
20 libclang-cpp.so.23.0 0x00007faa986d6621
21 libclang-cpp.so.23.0 0x00007faa986ad903
22 libclang-cpp.so.23.0 0x00007faa986a9887
23 libclang-cpp.so.23.0 0x00007faa986b014b
24 libclang-cpp.so.23.0 0x00007faa986a9daf
25 libclang-cpp.so.23.0 0x00007faa986aa02b
26 libclang-cpp.so.23.0 0x00007faa986a96e4
27 libclang-cpp.so.23.0 0x00007faa986763b2 clang::ast_matchers::MatchFinder::matchAST(clang::ASTContext&) + 610
28 libclang-cpp.so.23.0 0x00007faa99a88cec clang::MultiplexConsumer::HandleTranslationUnit(clang::ASTContext&) + 44
29 libclang-cpp.so.23.0 0x00007faa97cfba69 clang::ParseAST(clang::Sema&, bool, bool) + 633
30 libclang-cpp.so.23.0 0x00007faa99a3ed6b clang::FrontendAction::Execute() + 43
31 libclang-cpp.so.23.0 0x00007faa999b0ea4 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) + 948
32 libclang-cpp.so.23.0 0x00007faa99c40dd9 clang::tooling::FrontendActionFactory::runInvocation(std::shared_ptr<clang::CompilerInvocation>, clang::FileManager*, std::shared_ptr<clang::PCHContainerOperations>, clang::DiagnosticConsumer*) + 601
33 clang-tidy-23        0x0000562f34625fb4
34 libclang-cpp.so.23.0 0x00007faa99c40a64 clang::tooling::ToolInvocation::runInvocation(char const*, clang::driver::Compilation*, std::shared_ptr<clang::CompilerInvocation>, std::shared_ptr<clang::PCHContainerOperations>) + 276
35 libclang-cpp.so.23.0 0x00007faa99c3f89c clang::tooling::ToolInvocation::run() + 1452
36 libclang-cpp.so.23.0 0x00007faa99c42dcf clang::tooling::ClangTool::run(clang::tooling::ToolAction*) + 6271
37 clang-tidy-23        0x0000562f34622458 clang::tidy::runClangTidy(clang::tidy::ClangTidyContext&, clang::tooling::CompilationDatabase const&, llvm::ArrayRef<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>>, llvm::IntrusiveRefCntPtr<llvm::vfs::OverlayFileSystem>, bool, bool, llvm::StringRef, bool) + 1288
38 clang-tidy-23        0x0000562f3460d556 clang::tidy::clangTidyMain(int, char const**) + 10406
39 libc.so.6            0x00007faa8cc29d90
40 libc.so.6            0x00007faa8cc29e40 __libc_start_main + 128
41 clang-tidy-23        0x0000562f34608545 _start + 37
/home/runner/work/cppcheck/cppcheck/gui/showtypes.cpp: terminated by signal 11
```

Will provide a minimal reproducer later.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-tidy

Author: Oliver Stöneberg (firewave)

<details>
From https://github.com/danmar/cppcheck/pull/8343.

```
Stack dump:
0.	Program arguments: clang-tidy-23 -p=/home/runner/work/cppcheck/cppcheck/cmake.output -quiet --enable-check-profile --store-check-profile=/tmp/tmpnnlxy74g /home/runner/work/cppcheck/cppcheck/gui/showtypes.cpp
1.	&lt;eof&gt; parser at end of file
2.	ASTMatcher: Matching 'performance-use-std-move' against:
	CXXOperatorCallExpr : &lt;/home/runner/work/cppcheck/cppcheck/gui/codeeditor.h:115:9, col:21&gt;
Stack dump without symbol names (ensure you have llvm-symbolizer in your PATH or set the environment var `LLVM_SYMBOLIZER_PATH` to point to it):
0  libLLVM.so.23.0      0x00007faa92355db6 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) + 54
1  libLLVM.so.23.0      0x00007faa92353723 llvm::sys::RunSignalHandlers() + 131
2  libLLVM.so.23.0      0x00007faa92356b54
3  libc.so.6            0x00007faa8cc42520
4  clang-tidy-23        0x0000562f34b95630
5  libclang-cpp.so.23.0 0x00007faa986d7fb3 clang::ast_matchers::internal::DynTypedMatcher::matches(clang::DynTypedNode const&amp;, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const + 243
6  clang-tidy-23        0x0000562f34645564
7  libclang-cpp.so.23.0 0x00007faa986d7fb3 clang::ast_matchers::internal::DynTypedMatcher::matches(clang::DynTypedNode const&amp;, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const + 243
8  clang-tidy-23        0x0000562f3465963a clang::ast_matchers::internal::matcher_hasType1Matcher&lt;clang::Expr, clang::ast_matchers::internal::Matcher&lt;clang::Decl&gt;&gt;::matches(clang::Expr const&amp;, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const + 218
9  libclang-cpp.so.23.0 0x00007faa986d7fb3 clang::ast_matchers::internal::DynTypedMatcher::matches(clang::DynTypedNode const&amp;, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const + 243
10 clang-tidy-23        0x0000562f34659a6d clang::ast_matchers::internal::matcher_hasArgument0Matcher&lt;clang::CXXOperatorCallExpr, unsigned int, clang::ast_matchers::internal::Matcher&lt;clang::Expr&gt;&gt;::matches(clang::CXXOperatorCallExpr const&amp;, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const + 141
11 libclang-cpp.so.23.0 0x00007faa986d80f2 clang::ast_matchers::internal::DynTypedMatcher::matchesNoKindCheck(clang::DynTypedNode const&amp;, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const + 226
12 libclang-cpp.so.23.0 0x00007faa986dc52b
13 libclang-cpp.so.23.0 0x00007faa986db028
14 libclang-cpp.so.23.0 0x00007faa986d7fb3 clang::ast_matchers::internal::DynTypedMatcher::matches(clang::DynTypedNode const&amp;, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const + 243
15 libclang-cpp.so.23.0 0x00007faa986a5d19
16 libclang-cpp.so.23.0 0x00007faa986b43cc
17 libclang-cpp.so.23.0 0x00007faa986ced1a
18 libclang-cpp.so.23.0 0x00007faa986ba992
19 libclang-cpp.so.23.0 0x00007faa986b43da
20 libclang-cpp.so.23.0 0x00007faa986d6621
21 libclang-cpp.so.23.0 0x00007faa986ad903
22 libclang-cpp.so.23.0 0x00007faa986a9887
23 libclang-cpp.so.23.0 0x00007faa986b014b
24 libclang-cpp.so.23.0 0x00007faa986a9daf
25 libclang-cpp.so.23.0 0x00007faa986aa02b
26 libclang-cpp.so.23.0 0x00007faa986a96e4
27 libclang-cpp.so.23.0 0x00007faa986763b2 clang::ast_matchers::MatchFinder::matchAST(clang::ASTContext&amp;) + 610
28 libclang-cpp.so.23.0 0x00007faa99a88cec clang::MultiplexConsumer::HandleTranslationUnit(clang::ASTContext&amp;) + 44
29 libclang-cpp.so.23.0 0x00007faa97cfba69 clang::ParseAST(clang::Sema&amp;, bool, bool) + 633
30 libclang-cpp.so.23.0 0x00007faa99a3ed6b clang::FrontendAction::Execute() + 43
31 libclang-cpp.so.23.0 0x00007faa999b0ea4 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) + 948
32 libclang-cpp.so.23.0 0x00007faa99c40dd9 clang::tooling::FrontendActionFactory::runInvocation(std::shared_ptr&lt;clang::CompilerInvocation&gt;, clang::FileManager*, std::shared_ptr&lt;clang::PCHContainerOperations&gt;, clang::DiagnosticConsumer*) + 601
33 clang-tidy-23        0x0000562f34625fb4
34 libclang-cpp.so.23.0 0x00007faa99c40a64 clang::tooling::ToolInvocation::runInvocation(char const*, clang::driver::Compilation*, std::shared_ptr&lt;clang::CompilerInvocation&gt;, std::shared_ptr&lt;clang::PCHContainerOperations&gt;) + 276
35 libclang-cpp.so.23.0 0x00007faa99c3f89c clang::tooling::ToolInvocation::run() + 1452
36 libclang-cpp.so.23.0 0x00007faa99c42dcf clang::tooling::ClangTool::run(clang::tooling::ToolAction*) + 6271
37 clang-tidy-23        0x0000562f34622458 clang::tidy::runClangTidy(clang::tidy::ClangTidyContext&amp;, clang::tooling::CompilationDatabase const&amp;, llvm::ArrayRef&lt;std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;&gt;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::OverlayFileSystem&gt;, bool, bool, llvm::StringRef, bool) + 1288
38 clang-tidy-23        0x0000562f3460d556 clang::tidy::clangTidyMain(int, char const**) + 10406
39 libc.so.6            0x00007faa8cc29d90
40 libc.so.6            0x00007faa8cc29e40 __libc_start_main + 128
41 clang-tidy-23        0x0000562f34608545 _start + 37
/home/runner/work/cppcheck/cppcheck/gui/showtypes.cpp: terminated by signal 11
```

Will provide a minimal reproducer later.
</details>


---

### Comment 2 - firewave

This started with version 23 (specific label is still missing).

---

### Comment 3 - zeyi2

CC @serge-sans-paille 

---

### Comment 4 - firewave

To be specific about the version:
```
Get:1 https://apt.llvm.org/jammy llvm-toolchain-jammy/main amd64 clang-tidy-23 amd64 1:23~++20260310031814+cc38e4232c01-1~exp1~20260310151933.3015 [2280 kB]
```

---

### Comment 5 - serge-sans-paille

@firewave can you check if https://github.com/llvm/llvm-project/pull/186704 fixes the issue?

---

### Comment 6 - firewave

> [@firewave](https://github.com/firewave) can you check if [#186704](https://github.com/llvm/llvm-project/pull/186704) fixes the issue?

I do not have a local build to test with so we have to wait for the repo to be updated.

I will still provide a reproducer later to make sure it was not just an accidental drive-by fix hiding a different issue. Godbolt might also be updated earlier than the repo.

---

