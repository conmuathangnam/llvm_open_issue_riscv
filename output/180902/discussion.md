# [clangd] "Show AST" crashes on some code with templates

**Author:** ArcsinX
**URL:** https://github.com/llvm/llvm-project/issues/180902
**Status:** Closed
**Labels:** clangd, crash, regression:22
**Closed Date:** 2026-02-19T22:55:11Z

## Body

The problem can be reproduced on the main branch and looks like a regression (works ok on 21.1.8,  I didn't try other versions)

Code
```cpp
template <typename T>
struct Foo {
  template <typename U>
  struct Bar { static U Value; };
};
template <typename V, typename W>
auto foo() {
  return Foo<V>::template Bar<W>::Value;
}
```

Trace
```
 #0 0x0000555c2b844140 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/llvm-project/build_clang/bin/clangd+0x49f140)
 #1 0x0000555c2b8410cd SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x00007f0737f37520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x0000555c2bdb9832 clang::TemplateName::getKind() const (/llvm-project/build_clang/bin/clangd+0xa14832)
 #4 0x0000555c2ce04ef8 clang::clangd::(anonymous namespace)::DumpVisitor::TraverseTemplateName(clang::TemplateName const&) DumpAST.cpp:0:0
 #5 0x0000555c2ce106a7 clang::RecursiveASTVisitor<clang::clangd::(anonymous namespace)::DumpVisitor>::TraverseTypeLoc(clang::TypeLoc, bool) (.isra.0) DumpAST.cpp:0:0
 #6 0x0000555c2ce17575 clang::RecursiveASTVisitor<clang::clangd::(anonymous namespace)::DumpVisitor>::TraverseNestedNameSpecifierLoc(clang::NestedNameSpecifierLoc) (.isra.0) DumpAST.cpp:0:0
 #7 0x0000555c2ce17f5f clang::clangd::(anonymous namespace)::DumpVisitor::TraverseNestedNameSpecifierLoc(clang::NestedNameSpecifierLoc) DumpAST.cpp:0:0
 #8 0x0000555c2ce09fda clang::RecursiveASTVisitor<clang::clangd::(anonymous namespace)::DumpVisitor>::dataTraverseNode(clang::Stmt*, llvm::SmallVectorImpl<llvm::PointerIntPair<clang::Stmt*, 1u, bool, llvm::PointerLikeTypeTraits<clang::Stmt*>, llvm::PointerIntPairInfo<clang::Stmt*, 1u, llvm::PointerLikeTypeTraits<clang::Stmt*> > > >*) DumpAST.cpp:0:0
 #9 0x0000555c2ce0f031 clang::RecursiveASTVisitor<clang::clangd::(anonymous namespace)::DumpVisitor>::TraverseStmt(clang::Stmt*, llvm::SmallVectorImpl<llvm::PointerIntPair<clang::Stmt*, 1u, bool, llvm::PointerLikeTypeTraits<clang::Stmt*>, llvm::PointerIntPairInfo<clang::Stmt*, 1u, llvm::PointerLikeTypeTraits<clang::Stmt*> > > >*) (.constprop.0) DumpAST.cpp:0:0
#10 0x0000555c2ce18c1c clang::clangd::dumpAST(clang::DynTypedNode const&, clang::syntax::TokenBuffer const&, clang::ASTContext const&) (/llvm-project/build_clang/bin/clangd+0x1a73c1c)
#11 0x0000555c2cd7e151 bool llvm::function_ref<bool (clang::clangd::SelectionTree)>::callback_fn<clang::clangd::ClangdServer::getAST(llvm::StringRef, std::optional<clang::clangd::Range>, llvm::unique_function<void (llvm::Expected<std::optional<clang::clangd::ASTNode> >)>)::'lambda'(llvm::Expected<clang::clangd::InputsAndAST>)::operator()(llvm::Expected<clang::clangd::InputsAndAST>)::'lambda'(clang::clangd::SelectionTree)>(long, clang::clangd::SelectionTree) ClangdServer.cpp:0:0
#12 0x0000555c2cf33bfb clang::clangd::SelectionTree::createEach(clang::ASTContext&, clang::syntax::TokenBuffer const&, unsigned int, unsigned int, llvm::function_ref<bool (clang::clangd::SelectionTree)>) (/llvm-project/build_clang/bin/clangd+0x1b8ebfb)
#13 0x0000555c2cd7ef6c clang::clangd::ClangdServer::getAST(llvm::StringRef, std::optional<clang::clangd::Range>, llvm::unique_function<void (llvm::Expected<std::optional<clang::clangd::ASTNode> >)>)::'lambda'(llvm::Expected<clang::clangd::InputsAndAST>)::operator()(llvm::Expected<clang::clangd::InputsAndAST>) ClangdServer.cpp:0:0
#14 0x0000555c2cd7f461 void llvm::detail::UniqueFunctionBase<void, llvm::Expected<clang::clangd::InputsAndAST> >::CallImpl<clang::clangd::ClangdServer::getAST(llvm::StringRef, std::optional<clang::clangd::Range>, llvm::unique_function<void (llvm::Expected<std::optional<clang::clangd::ASTNode> >)>)::'lambda'(llvm::Expected<clang::clangd::InputsAndAST>)>(void*, llvm::Expected<clang::clangd::InputsAndAST>&) ClangdServer.cpp:0:0
#15 0x0000555c2cfab18a clang::clangd::(anonymous namespace)::ASTWorker::runWithAST(llvm::StringRef, llvm::unique_function<void (llvm::Expected<clang::clangd::InputsAndAST>)>, clang::clangd::TUScheduler::ASTActionInvalidation)::'lambda'()::operator()() TUScheduler.cpp:0:0
#16 0x0000555c2cfa0837 clang::clangd::(anonymous namespace)::ASTWorker::runTask(llvm::StringRef, llvm::function_ref<void ()>) (.constprop.0) TUScheduler.cpp:0:0
#17 0x0000555c2cfa231a clang::clangd::(anonymous namespace)::ASTWorker::run() TUScheduler.cpp:0:0
#18 0x0000555c2d173026 void* llvm::thread::ThreadProxy<std::tuple<clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>)::'lambda0'()> >(void*) Threading.cpp:0:0
#19 0x00007f0737f89ac3 start_thread ./nptl/./nptl/pthread_create.c:442:8
#20 0x00007f073801b8d0 ./misc/../sysdeps/unix/sysv/linux/x86_64/clone3.S:83:0
Signalled during AST worker action: GetAST
```

clang functionality to dump AST (from the same commit) works correctly (i.e. `clang++ -Xclang -ast-dump -fsyntax-only test.cxx` doesn't lead to a crash )

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clangd

Author: Aleksandr Platonov (ArcsinX)

<details>
The problem can be reproduced on the main branch and looks like a regression (works ok on 21.1.8,  I didn't try other versions)

Code
```cpp
template &lt;typename T&gt;
struct Foo {
  template &lt;typename U&gt;
  struct Bar { static U Value; };
};
template &lt;typename V, typename W&gt;
auto foo() {
  return Foo&lt;V&gt;::template Bar&lt;W&gt;::Value;
}
```

Trace
```
 #<!-- -->0 0x0000555c2b844140 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/llvm-project/build_clang/bin/clangd+0x49f140)
 #<!-- -->1 0x0000555c2b8410cd SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->2 0x00007f0737f37520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->3 0x0000555c2bdb9832 clang::TemplateName::getKind() const (/llvm-project/build_clang/bin/clangd+0xa14832)
 #<!-- -->4 0x0000555c2ce04ef8 clang::clangd::(anonymous namespace)::DumpVisitor::TraverseTemplateName(clang::TemplateName const&amp;) DumpAST.cpp:0:0
 #<!-- -->5 0x0000555c2ce106a7 clang::RecursiveASTVisitor&lt;clang::clangd::(anonymous namespace)::DumpVisitor&gt;::TraverseTypeLoc(clang::TypeLoc, bool) (.isra.0) DumpAST.cpp:0:0
 #<!-- -->6 0x0000555c2ce17575 clang::RecursiveASTVisitor&lt;clang::clangd::(anonymous namespace)::DumpVisitor&gt;::TraverseNestedNameSpecifierLoc(clang::NestedNameSpecifierLoc) (.isra.0) DumpAST.cpp:0:0
 #<!-- -->7 0x0000555c2ce17f5f clang::clangd::(anonymous namespace)::DumpVisitor::TraverseNestedNameSpecifierLoc(clang::NestedNameSpecifierLoc) DumpAST.cpp:0:0
 #<!-- -->8 0x0000555c2ce09fda clang::RecursiveASTVisitor&lt;clang::clangd::(anonymous namespace)::DumpVisitor&gt;::dataTraverseNode(clang::Stmt*, llvm::SmallVectorImpl&lt;llvm::PointerIntPair&lt;clang::Stmt*, 1u, bool, llvm::PointerLikeTypeTraits&lt;clang::Stmt*&gt;, llvm::PointerIntPairInfo&lt;clang::Stmt*, 1u, llvm::PointerLikeTypeTraits&lt;clang::Stmt*&gt; &gt; &gt; &gt;*) DumpAST.cpp:0:0
 #<!-- -->9 0x0000555c2ce0f031 clang::RecursiveASTVisitor&lt;clang::clangd::(anonymous namespace)::DumpVisitor&gt;::TraverseStmt(clang::Stmt*, llvm::SmallVectorImpl&lt;llvm::PointerIntPair&lt;clang::Stmt*, 1u, bool, llvm::PointerLikeTypeTraits&lt;clang::Stmt*&gt;, llvm::PointerIntPairInfo&lt;clang::Stmt*, 1u, llvm::PointerLikeTypeTraits&lt;clang::Stmt*&gt; &gt; &gt; &gt;*) (.constprop.0) DumpAST.cpp:0:0
#<!-- -->10 0x0000555c2ce18c1c clang::clangd::dumpAST(clang::DynTypedNode const&amp;, clang::syntax::TokenBuffer const&amp;, clang::ASTContext const&amp;) (/llvm-project/build_clang/bin/clangd+0x1a73c1c)
#<!-- -->11 0x0000555c2cd7e151 bool llvm::function_ref&lt;bool (clang::clangd::SelectionTree)&gt;::callback_fn&lt;clang::clangd::ClangdServer::getAST(llvm::StringRef, std::optional&lt;clang::clangd::Range&gt;, llvm::unique_function&lt;void (llvm::Expected&lt;std::optional&lt;clang::clangd::ASTNode&gt; &gt;)&gt;)::'lambda'(llvm::Expected&lt;clang::clangd::InputsAndAST&gt;)::operator()(llvm::Expected&lt;clang::clangd::InputsAndAST&gt;)::'lambda'(clang::clangd::SelectionTree)&gt;(long, clang::clangd::SelectionTree) ClangdServer.cpp:0:0
#<!-- -->12 0x0000555c2cf33bfb clang::clangd::SelectionTree::createEach(clang::ASTContext&amp;, clang::syntax::TokenBuffer const&amp;, unsigned int, unsigned int, llvm::function_ref&lt;bool (clang::clangd::SelectionTree)&gt;) (/llvm-project/build_clang/bin/clangd+0x1b8ebfb)
#<!-- -->13 0x0000555c2cd7ef6c clang::clangd::ClangdServer::getAST(llvm::StringRef, std::optional&lt;clang::clangd::Range&gt;, llvm::unique_function&lt;void (llvm::Expected&lt;std::optional&lt;clang::clangd::ASTNode&gt; &gt;)&gt;)::'lambda'(llvm::Expected&lt;clang::clangd::InputsAndAST&gt;)::operator()(llvm::Expected&lt;clang::clangd::InputsAndAST&gt;) ClangdServer.cpp:0:0
#<!-- -->14 0x0000555c2cd7f461 void llvm::detail::UniqueFunctionBase&lt;void, llvm::Expected&lt;clang::clangd::InputsAndAST&gt; &gt;::CallImpl&lt;clang::clangd::ClangdServer::getAST(llvm::StringRef, std::optional&lt;clang::clangd::Range&gt;, llvm::unique_function&lt;void (llvm::Expected&lt;std::optional&lt;clang::clangd::ASTNode&gt; &gt;)&gt;)::'lambda'(llvm::Expected&lt;clang::clangd::InputsAndAST&gt;)&gt;(void*, llvm::Expected&lt;clang::clangd::InputsAndAST&gt;&amp;) ClangdServer.cpp:0:0
#<!-- -->15 0x0000555c2cfab18a clang::clangd::(anonymous namespace)::ASTWorker::runWithAST(llvm::StringRef, llvm::unique_function&lt;void (llvm::Expected&lt;clang::clangd::InputsAndAST&gt;)&gt;, clang::clangd::TUScheduler::ASTActionInvalidation)::'lambda'()::operator()() TUScheduler.cpp:0:0
#<!-- -->16 0x0000555c2cfa0837 clang::clangd::(anonymous namespace)::ASTWorker::runTask(llvm::StringRef, llvm::function_ref&lt;void ()&gt;) (.constprop.0) TUScheduler.cpp:0:0
#<!-- -->17 0x0000555c2cfa231a clang::clangd::(anonymous namespace)::ASTWorker::run() TUScheduler.cpp:0:0
#<!-- -->18 0x0000555c2d173026 void* llvm::thread::ThreadProxy&lt;std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::'lambda0'()&gt; &gt;(void*) Threading.cpp:0:0
#<!-- -->19 0x00007f0737f89ac3 start_thread ./nptl/./nptl/pthread_create.c:442:8
#<!-- -->20 0x00007f073801b8d0 ./misc/../sysdeps/unix/sysv/linux/x86_64/clone3.S:83:0
Signalled during AST worker action: GetAST
```

clang functionality to dump AST (from the same commit) works correctly (i.e. `clang++ -Xclang -ast-dump -fsyntax-only test.cxx` doesn't lead to a crash )
</details>


---

### Comment 2 - edror12

I would like to work on this

---

### Comment 3 - edror12

@ArcsinX how did you get this error exactly? `bin/clangd --check=test.cpp` doesn't crash.

---

### Comment 4 - ArcsinX

> [@ArcsinX](https://github.com/ArcsinX) how did you get this error exactly? `bin/clangd --check=test.cpp` doesn't crash.

As I reported in the title, problem in "Show AST" functionality. clangd can parse the file well, but "Show AST" command leads to this crash

<img width="410" height="263" alt="Image" src="https://github.com/user-attachments/assets/eb578f55-c394-46a4-ab39-ad47b4c733e3" />

---

### Comment 5 - HighCommander4

In case it helps, the regressing commit is https://github.com/llvm/llvm-project/commit/ba9d1c41c41d568 (cc @mizvekov as its author).

(This regression window was brought to you by https://github.com/elfshaker/manyclangs.)

---

### Comment 6 - edror12

A PR for a fix was opened just now. @ArcsinX can you confirm that the bug was fixed for you too?

---

