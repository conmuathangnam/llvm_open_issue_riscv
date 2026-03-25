# modernize-use-std-print checker crashes when printf contains Chinese

**Author:** HermosMerlin
**URL:** https://github.com/llvm/llvm-project/issues/169198
**Status:** Closed
**Labels:** clang-tidy, crash
**Closed Date:** 2025-11-29T02:36:03Z

## Body

Seems like an issue with modernize-use-std-print. Won't crash after disabling it.

```cpp
#include <cstdio>

int main() {
    printf("你好世界\n");
    return 0;
}
```

Backtrace:
```cpp
Stack dump:
0.	ASTMatcher: Processing 'modernize-use-std-print' against:
	CompoundStmt : </home/eva/code/a.cpp:3:12, line:6:1>
--- Bound Nodes Begin ---
    func_decl - { FunctionDecl printf : </usr/include/stdio.h:363:1, col:56> }
    printf - { CallExpr : </home/eva/code/a.cpp:4:5, col:28> }
--- Bound Nodes End ---
 #0 0x000000000058ff31 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/usr/local/bin/clangd+0x58ff31)
 #1 0x000000000058cc47 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x00007dd9c5845330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #3 0x00000000029c28be clang::tidy::utils::FormatStringConverter::appendFormatText(llvm::StringRef) (/usr/local/bin/clangd+0x29c28be)
 #4 0x00000000029c2931 clang::tidy::utils::FormatStringConverter::finalizeFormatText() (/usr/local/bin/clangd+0x29c2931)
 #5 0x00000000025f4e1a clang::tidy::modernize::UseStdPrintCheck::check(clang::ast_matchers::MatchFinder::MatchResult const&) (/usr/local/bin/clangd+0x25f4e1a)
 #6 0x00000000033b0d7b clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor::MatchVisitor::visitMatch(clang::ast_matchers::BoundNodes const&) ASTMatchFinder.cpp:0:0
 #7 0x00000000033e7fba clang::ast_matchers::internal::BoundNodesTreeBuilder::visitMatches(clang::ast_matchers::internal::BoundNodesTreeBuilder::Visitor*) (/usr/local/bin/clangd+0x33e7fba)
 #8 0x00000000033b2161 clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor::matchWithFilter(clang::DynTypedNode const&) ASTMatchFinder.cpp:0:0
 #9 0x00000000033d6d8f clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor::TraverseStmt(clang::Stmt*, llvm::SmallVectorImpl<llvm::PointerIntPair<clang::Stmt*, 1u, bool, llvm::PointerLikeTypeTraits<clang::Stmt*>, llvm::PointerIntPairInfo<clang::Stmt*, 1u, llvm::PointerLikeTypeTraits<clang::Stmt*>>>>*) (.constprop.0) ASTMatchFinder.cpp:0:0
#10 0x00000000033de994 clang::RecursiveASTVisitor<clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor>::TraverseFunctionHelper(clang::FunctionDecl*) ASTMatchFinder.cpp:0:0
#11 0x00000000033cebf8 clang::RecursiveASTVisitor<clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor>::TraverseDecl(clang::Decl*) ASTMatchFinder.cpp:0:0
#12 0x00000000033d14b2 clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor::TraverseDecl(clang::Decl*) ASTMatchFinder.cpp:0:0
#13 0x00000000033cf943 clang::RecursiveASTVisitor<clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor>::TraverseDecl(clang::Decl*) ASTMatchFinder.cpp:0:0
#14 0x00000000033d14b2 clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor::TraverseDecl(clang::Decl*) ASTMatchFinder.cpp:0:0
#15 0x00000000033d1820 clang::ast_matchers::MatchFinder::matchAST(clang::ASTContext&) (/usr/local/bin/clangd+0x33d1820)
#16 0x0000000001dd3573 clang::clangd::ParsedAST::build(llvm::StringRef, clang::clangd::ParseInputs const&, std::unique_ptr<clang::CompilerInvocation, std::default_delete<clang::CompilerInvocation>>, llvm::ArrayRef<clang::clangd::Diag>, std::shared_ptr<clang::clangd::PreambleData const>) (/usr/local/bin/clangd+0x1dd3573)
#17 0x0000000001e77bfd clang::clangd::(anonymous namespace)::ASTWorker::generateDiagnostics(std::unique_ptr<clang::CompilerInvocation, std::default_delete<clang::CompilerInvocation>>, clang::clangd::ParseInputs, std::vector<clang::clangd::Diag, std::allocator<clang::clangd::Diag>>) TUScheduler.cpp:0:0
#18 0x0000000001e785cd clang::clangd::(anonymous namespace)::ASTWorker::updatePreamble(std::unique_ptr<clang::CompilerInvocation, std::default_delete<clang::CompilerInvocation>>, clang::clangd::ParseInputs, std::shared_ptr<clang::clangd::PreambleData const>, std::vector<clang::clangd::Diag, std::allocator<clang::clangd::Diag>>, clang::clangd::WantDiagnostics)::'lambda'()::operator()() (.part.0) TUScheduler.cpp:0:0
#19 0x0000000001e73e49 clang::clangd::(anonymous namespace)::ASTWorker::runTask(llvm::StringRef, llvm::function_ref<void ()>) (.constprop.0) TUScheduler.cpp:0:0
#20 0x0000000001e74cf2 clang::clangd::(anonymous namespace)::ASTWorker::run() TUScheduler.cpp:0:0
#21 0x00000000020755ae void* llvm::thread::ThreadProxy<std::tuple<clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>)::'lambda0'()>>(void*) Threading.cpp:0:0
#22 0x00007dd9c589caa4 start_thread ./nptl/pthread_create.c:447:8
#23 0x00007dd9c5929c6c clone3 ./misc/../sysdeps/unix/sysv/linux/x86_64/clone3.S:80:0
Signalled during AST worker action: Build AST
  Filename: /home/eva/code/a.cpp
  Directory: /home/eva/code
  Command Line: /usr/local/gcc-16/bin/g++-16 --driver-mode=g++ -I/usr/local/gcc-16/include/c++/16.0.0 -I/usr/local/gcc-16/include/c++/16.0.0/x86_64-linux-gnu -I/usr/local/gcc-16/include/c++/16.0.0/backward -I/usr/local/lib/clang/22/include -I/usr/local/gcc-16/include -I/usr/local/gcc-16/lib/gcc/x86_64-linux-gnu/16.0.0/include-fixed -I/usr/local/include -I/usr/include/x86_64-linux-gnu -I/usr/include -std=gnu++2c -DLX_LOCAL -Wall -xc++ -resource-dir=/usr/local/lib/clang/22 -- /home/eva/code/a.cpp
  Version: 226
```

**System information**

Output of `clangd --version`:
```
clangd version 22.0.0git (https://github.com/llvm/llvm-project 226765b60cc54e03386f38874a177c906f5aa6e7)
Features: linux
Platform: x86_64-unknown-linux-gnu; target=x86_64-linux-gnu
```

Editor/LSP plugin: VS Code
Operating system: Ubuntu 24.04 noble(on the Windows Subsystem for Linux)


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-tidy

Author: None (HermosMerlin)

<details>
Seems like an issue with modernize-use-std-print. Won't crash after disabling it.

```cpp
#include &lt;cstdio&gt;

int main() {
    printf("你好世界\n");
    return 0;
}
```

Backtrace:
```cpp
Stack dump:
0.	ASTMatcher: Processing 'modernize-use-std-print' against:
	CompoundStmt : &lt;/home/eva/code/a.cpp:3:12, line:6:1&gt;
--- Bound Nodes Begin ---
    func_decl - { FunctionDecl printf : &lt;/usr/include/stdio.h:363:1, col:56&gt; }
    printf - { CallExpr : &lt;/home/eva/code/a.cpp:4:5, col:28&gt; }
--- Bound Nodes End ---
 #<!-- -->0 0x000000000058ff31 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/usr/local/bin/clangd+0x58ff31)
 #<!-- -->1 0x000000000058cc47 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->2 0x00007dd9c5845330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->3 0x00000000029c28be clang::tidy::utils::FormatStringConverter::appendFormatText(llvm::StringRef) (/usr/local/bin/clangd+0x29c28be)
 #<!-- -->4 0x00000000029c2931 clang::tidy::utils::FormatStringConverter::finalizeFormatText() (/usr/local/bin/clangd+0x29c2931)
 #<!-- -->5 0x00000000025f4e1a clang::tidy::modernize::UseStdPrintCheck::check(clang::ast_matchers::MatchFinder::MatchResult const&amp;) (/usr/local/bin/clangd+0x25f4e1a)
 #<!-- -->6 0x00000000033b0d7b clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor::MatchVisitor::visitMatch(clang::ast_matchers::BoundNodes const&amp;) ASTMatchFinder.cpp:0:0
 #<!-- -->7 0x00000000033e7fba clang::ast_matchers::internal::BoundNodesTreeBuilder::visitMatches(clang::ast_matchers::internal::BoundNodesTreeBuilder::Visitor*) (/usr/local/bin/clangd+0x33e7fba)
 #<!-- -->8 0x00000000033b2161 clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor::matchWithFilter(clang::DynTypedNode const&amp;) ASTMatchFinder.cpp:0:0
 #<!-- -->9 0x00000000033d6d8f clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor::TraverseStmt(clang::Stmt*, llvm::SmallVectorImpl&lt;llvm::PointerIntPair&lt;clang::Stmt*, 1u, bool, llvm::PointerLikeTypeTraits&lt;clang::Stmt*&gt;, llvm::PointerIntPairInfo&lt;clang::Stmt*, 1u, llvm::PointerLikeTypeTraits&lt;clang::Stmt*&gt;&gt;&gt;&gt;*) (.constprop.0) ASTMatchFinder.cpp:0:0
#<!-- -->10 0x00000000033de994 clang::RecursiveASTVisitor&lt;clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor&gt;::TraverseFunctionHelper(clang::FunctionDecl*) ASTMatchFinder.cpp:0:0
#<!-- -->11 0x00000000033cebf8 clang::RecursiveASTVisitor&lt;clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor&gt;::TraverseDecl(clang::Decl*) ASTMatchFinder.cpp:0:0
#<!-- -->12 0x00000000033d14b2 clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor::TraverseDecl(clang::Decl*) ASTMatchFinder.cpp:0:0
#<!-- -->13 0x00000000033cf943 clang::RecursiveASTVisitor&lt;clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor&gt;::TraverseDecl(clang::Decl*) ASTMatchFinder.cpp:0:0
#<!-- -->14 0x00000000033d14b2 clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor::TraverseDecl(clang::Decl*) ASTMatchFinder.cpp:0:0
#<!-- -->15 0x00000000033d1820 clang::ast_matchers::MatchFinder::matchAST(clang::ASTContext&amp;) (/usr/local/bin/clangd+0x33d1820)
#<!-- -->16 0x0000000001dd3573 clang::clangd::ParsedAST::build(llvm::StringRef, clang::clangd::ParseInputs const&amp;, std::unique_ptr&lt;clang::CompilerInvocation, std::default_delete&lt;clang::CompilerInvocation&gt;&gt;, llvm::ArrayRef&lt;clang::clangd::Diag&gt;, std::shared_ptr&lt;clang::clangd::PreambleData const&gt;) (/usr/local/bin/clangd+0x1dd3573)
#<!-- -->17 0x0000000001e77bfd clang::clangd::(anonymous namespace)::ASTWorker::generateDiagnostics(std::unique_ptr&lt;clang::CompilerInvocation, std::default_delete&lt;clang::CompilerInvocation&gt;&gt;, clang::clangd::ParseInputs, std::vector&lt;clang::clangd::Diag, std::allocator&lt;clang::clangd::Diag&gt;&gt;) TUScheduler.cpp:0:0
#<!-- -->18 0x0000000001e785cd clang::clangd::(anonymous namespace)::ASTWorker::updatePreamble(std::unique_ptr&lt;clang::CompilerInvocation, std::default_delete&lt;clang::CompilerInvocation&gt;&gt;, clang::clangd::ParseInputs, std::shared_ptr&lt;clang::clangd::PreambleData const&gt;, std::vector&lt;clang::clangd::Diag, std::allocator&lt;clang::clangd::Diag&gt;&gt;, clang::clangd::WantDiagnostics)::'lambda'()::operator()() (.part.0) TUScheduler.cpp:0:0
#<!-- -->19 0x0000000001e73e49 clang::clangd::(anonymous namespace)::ASTWorker::runTask(llvm::StringRef, llvm::function_ref&lt;void ()&gt;) (.constprop.0) TUScheduler.cpp:0:0
#<!-- -->20 0x0000000001e74cf2 clang::clangd::(anonymous namespace)::ASTWorker::run() TUScheduler.cpp:0:0
#<!-- -->21 0x00000000020755ae void* llvm::thread::ThreadProxy&lt;std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::'lambda0'()&gt;&gt;(void*) Threading.cpp:0:0
#<!-- -->22 0x00007dd9c589caa4 start_thread ./nptl/pthread_create.c:447:8
#<!-- -->23 0x00007dd9c5929c6c clone3 ./misc/../sysdeps/unix/sysv/linux/x86_64/clone3.S:80:0
Signalled during AST worker action: Build AST
  Filename: /home/eva/code/a.cpp
  Directory: /home/eva/code
  Command Line: /usr/local/gcc-16/bin/g++-16 --driver-mode=g++ -I/usr/local/gcc-16/include/c++/16.0.0 -I/usr/local/gcc-16/include/c++/16.0.0/x86_64-linux-gnu -I/usr/local/gcc-16/include/c++/16.0.0/backward -I/usr/local/lib/clang/22/include -I/usr/local/gcc-16/include -I/usr/local/gcc-16/lib/gcc/x86_64-linux-gnu/16.0.0/include-fixed -I/usr/local/include -I/usr/include/x86_64-linux-gnu -I/usr/include -std=gnu++2c -DLX_LOCAL -Wall -xc++ -resource-dir=/usr/local/lib/clang/22 -- /home/eva/code/a.cpp
  Version: 226
```

**System information**

Output of `clangd --version`:
```
clangd version 22.0.0git (https://github.com/llvm/llvm-project 226765b60cc54e03386f38874a177c906f5aa6e7)
Features: linux
Platform: x86_64-unknown-linux-gnu; target=x86_64-linux-gnu
```

Editor/LSP plugin: VS Code
Operating system: Ubuntu 24.04 noble(on the Windows Subsystem for Linux)

</details>


---

### Comment 2 - zeyi2

Thanks for the information! I'm looking into it right now :)

Also, this modification produces the expected diagnostic from `modernize-use-std-print` and does not crash:

```c++
#include <cstdio>

int main() {
  printf("%s\n", u8"你好世界");
  return 0;
}

```
```
+ clang-tidy -p /llvm-project/repro '-checks=-*,modernize-use-std-print' /llvm-project/repro/a.cpp -- -std=gnu++2c
2 warnings generated.
/llvm-project/repro/a.cpp:6:3: warning: use 'std::println' instead of 'printf' [modernize-use-std-print]
    2 | 
    3 | int main() {
    4 |   printf("%s\n", u8"你好世界");
      |   ^~~~~~ ~~~~~~
      |   std::println "{}"
```

If this check is important for you, you can temporarily apply this workaround while we continue working on a proper fix.

---

