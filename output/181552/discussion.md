# [clang-tidy] Crash on latest main while running `readability-container-size-empty`

**Author:** vbvictor
**URL:** https://github.com/llvm/llvm-project/issues/181552
**Status:** Closed
**Labels:** clang-tidy, crash
**Closed Date:** 2026-02-16T04:21:59Z

## Body

While running latest clang-tidy over clang-tools-extra got crush:

```
[ 48/463][136.1s] build/bin/clang-tidy -p=build/ /home/user/repos/llvm-project/clang-tools-extra/clang-tidy/ClangTidy.cpp
clang-tidy: /home/user/repos/llvm-project/clang/include/clang/AST/Decl.h:302: llvm::StringRef clang::NamedDecl::getName() const: Assertion `Name.isIdentifier() && "Name is not a simple identifier"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: build/bin/clang-tidy -p=build/ /home/user/repos/llvm-project/clang-tools-extra/clang-tidy/ClangTidy.cpp
1.      <eof> parser at end of file
2.      ASTMatcher: Matching 'readability-container-size-empty' against:
        CXXMemberCallExpr : </home/user/repos/llvm-project/clang/include/clang/Sema/SemaConcept.h:119:7, col:60>
Stack dump without symbol names (ensure you have llvm-symbolizer in your PATH or set the environment var `LLVM_SYMBOLIZER_PATH` to point to it):
0  clang-tidy 0x000055c5bed4e560 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) + 240
1  clang-tidy 0x000055c5bed4b23f llvm::sys::RunSignalHandlers() + 47
2  clang-tidy 0x000055c5bed4b392
3  libc.so.6  0x000076373ac42520
4  libc.so.6  0x000076373ac969fc pthread_kill + 300
5  libc.so.6  0x000076373ac42476 raise + 22
6  libc.so.6  0x000076373ac287f3 abort + 211
7  libc.so.6  0x000076373ac2871b
8  libc.so.6  0x000076373ac39e96
9  clang-tidy 0x000055c5bb2b2d06
10 clang-tidy 0x000055c5bd20b944 clang::ast_matchers::internal::DynTypedMatcher::matches(clang::DynTypedNode const&, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const + 244
11 clang-tidy 0x000055c5bd20f0ab
12 clang-tidy 0x000055c5bd20baab clang::ast_matchers::internal::DynTypedMatcher::matchesNoKindCheck(clang::DynTypedNode const&, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const + 235
13 clang-tidy 0x000055c5bd20bcfb
14 clang-tidy 0x000055c5bd20b944 clang::ast_matchers::internal::DynTypedMatcher::matches(clang::DynTypedNode const&, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const + 244
15 clang-tidy 0x000055c5bd20f0ab
16 clang-tidy 0x000055c5bd20f75d
17 clang-tidy 0x000055c5bd20b944 clang::ast_matchers::internal::DynTypedMatcher::matches(clang::DynTypedNode const&, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const + 244
18 clang-tidy 0x000055c5bd1d641a
19 clang-tidy 0x000055c5bd1e3652
20 clang-tidy 0x000055c5bd1e3b8e
21 clang-tidy 0x000055c5bd1e3509
22 clang-tidy 0x000055c5bd1f3836
23 clang-tidy 0x000055c5bd1f4686
24 clang-tidy 0x000055c5baf77cb2
25 clang-tidy 0x000055c5bd20baab clang::ast_matchers::internal::DynTypedMatcher::matchesNoKindCheck(clang::DynTypedNode const&, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const + 235
26 clang-tidy 0x000055c5bd20bbe5
27 clang-tidy 0x000055c5bd20f75d
28 clang-tidy 0x000055c5bd20b944 clang::ast_matchers::internal::DynTypedMatcher::matches(clang::DynTypedNode const&, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const + 244
29 clang-tidy 0x000055c5baabf20c
30 clang-tidy 0x000055c5bd20b944 clang::ast_matchers::internal::DynTypedMatcher::matches(clang::DynTypedNode const&, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const + 244
31 clang-tidy 0x000055c5bd1da49b
32 clang-tidy 0x000055c5bd209425
33 clang-tidy 0x000055c5bd20a2ee
34 clang-tidy 0x000055c5bd1fba17
35 clang-tidy 0x000055c5bd1fbd8d
36 clang-tidy 0x000055c5bd200eb7
37 clang-tidy 0x000055c5bd206593
38 clang-tidy 0x000055c5bd1f6227
39 clang-tidy 0x000055c5bd1f6d6a
40 clang-tidy 0x000055c5bd1f5ecb
41 clang-tidy 0x000055c5bd1f6227
42 clang-tidy 0x000055c5bd1f6d6a
43 clang-tidy 0x000055c5bd1f4c7e
44 clang-tidy 0x000055c5bd1f6227
45 clang-tidy 0x000055c5bd1f6d6a
46 clang-tidy 0x000055c5bd20635b
47 clang-tidy 0x000055c5bd1f6227
48 clang-tidy 0x000055c5bd1f6801 clang::ast_matchers::MatchFinder::matchAST(clang::ASTContext&) + 1281
49 clang-tidy 0x000055c5bc02ea20 clang::MultiplexConsumer::HandleTranslationUnit(clang::ASTContext&) + 48
50 clang-tidy 0x000055c5bc0c5f7c clang::ParseAST(clang::Sema&, bool, bool) + 1164
51 clang-tidy 0x000055c5bbfdbdc9 clang::FrontendAction::Execute() + 57
52 clang-tidy 0x000055c5bbf580c5 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) + 997
53 clang-tidy 0x000055c5bb6edf2a clang::tooling::FrontendActionFactory::runInvocation(std::shared_ptr<clang::CompilerInvocation>, clang::FileManager*, std::shared_ptr<clang::PCHContainerOperations>, clang::DiagnosticConsumer*) + 442
54 clang-tidy 0x000055c5bb68cbfb
55 clang-tidy 0x000055c5bb6e66f5 clang::tooling::ToolInvocation::runInvocation(char const*, clang::driver::Compilation*, std::shared_ptr<clang::CompilerInvocation>, std::shared_ptr<clang::PCHContainerOperations>) + 133
56 clang-tidy 0x000055c5bb6e90b0 clang::tooling::ToolInvocation::run() + 1248
57 clang-tidy 0x000055c5bb6eb71b clang::tooling::ClangTool::run(clang::tooling::ToolAction*) + 4139
58 clang-tidy 0x000055c5bb69733f clang::tidy::runClangTidy(clang::tidy::ClangTidyContext&, clang::tooling::CompilationDatabase const&, llvm::ArrayRef<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>>, llvm::IntrusiveRefCntPtr<llvm::vfs::OverlayFileSystem>, bool, bool, llvm::StringRef, bool) + 1311
59 clang-tidy 0x000055c5baa7cf76 clang::tidy::clangTidyMain(int, char const**) + 8166
60 libc.so.6  0x000076373ac29d90
61 libc.so.6  0x000076373ac29e40 __libc_start_main + 128
62 clang-tidy 0x000055c5baa73065 _start + 37
/home/user/repos/llvm-project/clang-tools-extra/clang-tidy/ClangTidy.cpp: terminated by signal 6
```

Happening probably here:
https://github.com/llvm/llvm-project/blob/f26e8595c3073fafc822bfcdd3ca9dec32ab573b/clang-tools-extra/clang-tidy/readability/ContainerContainsCheck.cpp#L94-L95

Probably a regression after https://github.com/llvm/llvm-project/pull/175121 

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-tidy

Author: Baranov Victor (vbvictor)

<details>
While running latest clang-tidy over clang-tools-extra got crush:

```
[ 48/463][136.1s] build/bin/clang-tidy -p=build/ /home/user/repos/llvm-project/clang-tools-extra/clang-tidy/ClangTidy.cpp
clang-tidy: /home/user/repos/llvm-project/clang/include/clang/AST/Decl.h:302: llvm::StringRef clang::NamedDecl::getName() const: Assertion `Name.isIdentifier() &amp;&amp; "Name is not a simple identifier"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: build/bin/clang-tidy -p=build/ /home/user/repos/llvm-project/clang-tools-extra/clang-tidy/ClangTidy.cpp
1.      &lt;eof&gt; parser at end of file
2.      ASTMatcher: Matching 'readability-container-size-empty' against:
        CXXMemberCallExpr : &lt;/home/user/repos/llvm-project/clang/include/clang/Sema/SemaConcept.h:119:7, col:60&gt;
Stack dump without symbol names (ensure you have llvm-symbolizer in your PATH or set the environment var `LLVM_SYMBOLIZER_PATH` to point to it):
0  clang-tidy 0x000055c5bed4e560 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) + 240
1  clang-tidy 0x000055c5bed4b23f llvm::sys::RunSignalHandlers() + 47
2  clang-tidy 0x000055c5bed4b392
3  libc.so.6  0x000076373ac42520
4  libc.so.6  0x000076373ac969fc pthread_kill + 300
5  libc.so.6  0x000076373ac42476 raise + 22
6  libc.so.6  0x000076373ac287f3 abort + 211
7  libc.so.6  0x000076373ac2871b
8  libc.so.6  0x000076373ac39e96
9  clang-tidy 0x000055c5bb2b2d06
10 clang-tidy 0x000055c5bd20b944 clang::ast_matchers::internal::DynTypedMatcher::matches(clang::DynTypedNode const&amp;, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const + 244
11 clang-tidy 0x000055c5bd20f0ab
12 clang-tidy 0x000055c5bd20baab clang::ast_matchers::internal::DynTypedMatcher::matchesNoKindCheck(clang::DynTypedNode const&amp;, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const + 235
13 clang-tidy 0x000055c5bd20bcfb
14 clang-tidy 0x000055c5bd20b944 clang::ast_matchers::internal::DynTypedMatcher::matches(clang::DynTypedNode const&amp;, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const + 244
15 clang-tidy 0x000055c5bd20f0ab
16 clang-tidy 0x000055c5bd20f75d
17 clang-tidy 0x000055c5bd20b944 clang::ast_matchers::internal::DynTypedMatcher::matches(clang::DynTypedNode const&amp;, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const + 244
18 clang-tidy 0x000055c5bd1d641a
19 clang-tidy 0x000055c5bd1e3652
20 clang-tidy 0x000055c5bd1e3b8e
21 clang-tidy 0x000055c5bd1e3509
22 clang-tidy 0x000055c5bd1f3836
23 clang-tidy 0x000055c5bd1f4686
24 clang-tidy 0x000055c5baf77cb2
25 clang-tidy 0x000055c5bd20baab clang::ast_matchers::internal::DynTypedMatcher::matchesNoKindCheck(clang::DynTypedNode const&amp;, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const + 235
26 clang-tidy 0x000055c5bd20bbe5
27 clang-tidy 0x000055c5bd20f75d
28 clang-tidy 0x000055c5bd20b944 clang::ast_matchers::internal::DynTypedMatcher::matches(clang::DynTypedNode const&amp;, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const + 244
29 clang-tidy 0x000055c5baabf20c
30 clang-tidy 0x000055c5bd20b944 clang::ast_matchers::internal::DynTypedMatcher::matches(clang::DynTypedNode const&amp;, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const + 244
31 clang-tidy 0x000055c5bd1da49b
32 clang-tidy 0x000055c5bd209425
33 clang-tidy 0x000055c5bd20a2ee
34 clang-tidy 0x000055c5bd1fba17
35 clang-tidy 0x000055c5bd1fbd8d
36 clang-tidy 0x000055c5bd200eb7
37 clang-tidy 0x000055c5bd206593
38 clang-tidy 0x000055c5bd1f6227
39 clang-tidy 0x000055c5bd1f6d6a
40 clang-tidy 0x000055c5bd1f5ecb
41 clang-tidy 0x000055c5bd1f6227
42 clang-tidy 0x000055c5bd1f6d6a
43 clang-tidy 0x000055c5bd1f4c7e
44 clang-tidy 0x000055c5bd1f6227
45 clang-tidy 0x000055c5bd1f6d6a
46 clang-tidy 0x000055c5bd20635b
47 clang-tidy 0x000055c5bd1f6227
48 clang-tidy 0x000055c5bd1f6801 clang::ast_matchers::MatchFinder::matchAST(clang::ASTContext&amp;) + 1281
49 clang-tidy 0x000055c5bc02ea20 clang::MultiplexConsumer::HandleTranslationUnit(clang::ASTContext&amp;) + 48
50 clang-tidy 0x000055c5bc0c5f7c clang::ParseAST(clang::Sema&amp;, bool, bool) + 1164
51 clang-tidy 0x000055c5bbfdbdc9 clang::FrontendAction::Execute() + 57
52 clang-tidy 0x000055c5bbf580c5 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) + 997
53 clang-tidy 0x000055c5bb6edf2a clang::tooling::FrontendActionFactory::runInvocation(std::shared_ptr&lt;clang::CompilerInvocation&gt;, clang::FileManager*, std::shared_ptr&lt;clang::PCHContainerOperations&gt;, clang::DiagnosticConsumer*) + 442
54 clang-tidy 0x000055c5bb68cbfb
55 clang-tidy 0x000055c5bb6e66f5 clang::tooling::ToolInvocation::runInvocation(char const*, clang::driver::Compilation*, std::shared_ptr&lt;clang::CompilerInvocation&gt;, std::shared_ptr&lt;clang::PCHContainerOperations&gt;) + 133
56 clang-tidy 0x000055c5bb6e90b0 clang::tooling::ToolInvocation::run() + 1248
57 clang-tidy 0x000055c5bb6eb71b clang::tooling::ClangTool::run(clang::tooling::ToolAction*) + 4139
58 clang-tidy 0x000055c5bb69733f clang::tidy::runClangTidy(clang::tidy::ClangTidyContext&amp;, clang::tooling::CompilationDatabase const&amp;, llvm::ArrayRef&lt;std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;&gt;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::OverlayFileSystem&gt;, bool, bool, llvm::StringRef, bool) + 1311
59 clang-tidy 0x000055c5baa7cf76 clang::tidy::clangTidyMain(int, char const**) + 8166
60 libc.so.6  0x000076373ac29d90
61 libc.so.6  0x000076373ac29e40 __libc_start_main + 128
62 clang-tidy 0x000055c5baa73065 _start + 37
/home/user/repos/llvm-project/clang-tools-extra/clang-tidy/ClangTidy.cpp: terminated by signal 6
```

Happening probably here:
https://github.com/llvm/llvm-project/blob/f26e8595c3073fafc822bfcdd3ca9dec32ab573b/clang-tools-extra/clang-tidy/readability/ContainerContainsCheck.cpp#L94-L95

Probably a regression after https://github.com/llvm/llvm-project/pull/175121 
</details>


---

### Comment 2 - zeyi2

Reduced to:

```c++
// build/bin/clang-tidy -checks='-*,readability-container-size-empty' repro.cpp -- -std=c++20
struct SmallBitVector {
  int size() const { return 0; }
  bool empty() const { return true; }
  ~SmallBitVector() {}
};

struct NormalizedConstraint {
  union {
    SmallBitVector Indexes;
  };
  ~NormalizedConstraint() {
    Indexes.~SmallBitVector();
  }
};
```

---

### Comment 3 - zeyi2

I'll take this issue if no one is already working on it :)

---

### Comment 4 - vbvictor

> I'll take this issue if no one is already working on it :)

I'm definitely not working on it rn

---

