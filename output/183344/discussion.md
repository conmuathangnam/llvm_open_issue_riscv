# [Analyzer] 21.1.8 crashes during static analysis of function declaration with both `ownership_returns` and `ownership_takes` attributes

**Author:** jcttrll
**URL:** https://github.com/llvm/llvm-project/issues/183344
**Status:** Closed
**Labels:** clang:static analyzer, confirmed, crash
**Closed Date:** 2026-02-27T12:36:30Z

## Body

Given the following file `crash.c` (a reduced reproduction case):

```c
typedef struct X X;
typedef struct Y Y;

Y* X_to_Y(X* x) __attribute__((ownership_returns(Y))) __attribute__((ownership_takes(X, 1)));

int main() {
	return X_to_Y(0) != 0;
}
```

...`clang --analyze` crashes with:

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: clang --analyze -o /dev/null crash.c
1.      <eof> parser at end of file
2.      While analyzing stack: 
        #0 Calling main
3.      crash.c:7:9: Error evaluating statement
4.      crash.c:7:9: Error evaluating statement
 #0 0x00007f746b3b891f llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/usr/lib/llvm-21/bin/../lib/libLLVM.so.21.1+0x44fc91f)
 #1 0x00007f746b3b61c7 llvm::sys::RunSignalHandlers() (/usr/lib/llvm-21/bin/../lib/libLLVM.so.21.1+0x44fa1c7)
 #2 0x00007f746b2efe87 (/usr/lib/llvm-21/bin/../lib/libLLVM.so.21.1+0x4433e87)
 #3 0x00007f7466954330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x00007f747275a972 clang::ento::MemRegion::getAsOffset() const (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x33da972)
 #5 0x00007f7472792d92 (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x3412d92)
 #6 0x00007f7472766147 clang::ento::ProgramState::bindDefaultInitial(clang::ento::SVal, clang::ento::SVal, clang::LocationContext const*) const (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x33e6147)
 #7 0x00007f74728a9de6 (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x3529de6)
 #8 0x00007f74728b2444 (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x3532444)
 #9 0x00007f74728b116f (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x353116f)
#10 0x00007f74726ef81e clang::ento::CheckerManager::runCheckersForEvalCall(clang::ento::ExplodedNodeSet&, clang::ento::ExplodedNodeSet const&, clang::ento::CallEvent const&, clang::ento::ExprEngine&, clang::ento::EvalCallOptions const&) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x336f81e)
#11 0x00007f747273bf0e clang::ento::ExprEngine::evalCall(clang::ento::ExplodedNodeSet&, clang::ento::ExplodedNode*, clang::ento::CallEvent const&) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x33bbf0e)
#12 0x00007f747273bc97 clang::ento::ExprEngine::VisitCallExpr(clang::CallExpr const*, clang::ento::ExplodedNode*, clang::ento::ExplodedNodeSet&) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x33bbc97)
#13 0x00007f7472719922 clang::ento::ExprEngine::Visit(clang::Stmt const*, clang::ento::ExplodedNode*, clang::ento::ExplodedNodeSet&) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x3399922)
#14 0x00007f7472715b3a clang::ento::ExprEngine::ProcessStmt(clang::Stmt const*, clang::ento::ExplodedNode*) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x3395b3a)
#15 0x00007f74726f6f57 clang::ento::CoreEngine::dispatchWorkItem(clang::ento::ExplodedNode*, clang::ProgramPoint, clang::ento::WorkListUnit const&) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x3376f57)
#16 0x00007f74726f683c clang::ento::CoreEngine::ExecuteWorkList(clang::LocationContext const*, unsigned int, llvm::IntrusiveRefCntPtr<clang::ento::ProgramState const>) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x337683c)
#17 0x00007f74729ff34c (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x367f34c)
#18 0x00007f74729fddc8 (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x367ddc8)
#19 0x00007f74708082c9 clang::ParseAST(clang::Sema&, bool, bool) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x14882c9)
#20 0x00007f74724076d2 clang::FrontendAction::Execute() (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x30876d2)
#21 0x00007f747237e134 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x2ffe134)
#22 0x00007f747248f37c clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x310f37c)
#23 0x000055f89ef4e31b cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/usr/lib/llvm-21/bin/clang+0x1331b)
#24 0x000055f89ef4b024 (/usr/lib/llvm-21/bin/clang+0x10024)
#25 0x00007f7472042172 (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x2cc2172)
#26 0x00007f746b2efb07 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/usr/lib/llvm-21/bin/../lib/libLLVM.so.21.1+0x4433b07)
#27 0x00007f74720419c6 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x2cc19c6)
#28 0x00007f7472004f02 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x2c84f02)
#29 0x00007f74720050ef clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x2c850ef)
#30 0x00007f7472023866 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x2ca3866)
#31 0x000055f89ef4aaf9 clang_main(int, char**, llvm::ToolContext const&) (/usr/lib/llvm-21/bin/clang+0xfaf9)
#32 0x000055f89ef58ef3 main (/usr/lib/llvm-21/bin/clang+0x1def3)
#33 0x00007f74669391ca (/lib/x86_64-linux-gnu/libc.so.6+0x2a1ca)
#34 0x00007f746693928b __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x2a28b)
#35 0x000055f89ef48d25 _start (/usr/lib/llvm-21/bin/clang+0xdd25)
clang: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Ubuntu clang version 21.1.8 (++20251221032922+2078da43e25a-1~exp1~20251221153059.70)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/lib/llvm-21/bin
clang: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang: note: diagnostic msg: /tmp/crash-90c3aa.c
clang: note: diagnostic msg: /tmp/crash-90c3aa.sh
clang: note: diagnostic msg: 

********************
```
Requested files are attached.

[crash-90c3aa.sh](https://github.com/user-attachments/files/25552713/crash-90c3aa.sh)
[crash-90c3aa.c](https://github.com/user-attachments/files/25552714/crash-90c3aa.c)

## Comments

### Comment 1 - EugeneZelenko

Could you please try 22 or `main` branch? https://godbolt.org should be helpful. Only most recent release is maintained.


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-static-analyzer

Author: Jonathan Cottrill (jcttrll)

<details>
Given the following file `crash.c` (a reduced reproduction case):
```
typedef struct X X;
typedef struct Y Y;

Y* X_to_Y(X* x) __attribute__((ownership_returns(Y))) __attribute__((ownership_takes(X, 1)));

int main() {
	return X_to_Y(0) != 0;
}
```
...`clang --analyze` crashes with:
```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: clang --analyze -o /dev/null crash.c
1.      &lt;eof&gt; parser at end of file
2.      While analyzing stack: 
        #<!-- -->0 Calling main
3.      crash.c:7:9: Error evaluating statement
4.      crash.c:7:9: Error evaluating statement
 #<!-- -->0 0x00007f746b3b891f llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/usr/lib/llvm-21/bin/../lib/libLLVM.so.21.1+0x44fc91f)
 #<!-- -->1 0x00007f746b3b61c7 llvm::sys::RunSignalHandlers() (/usr/lib/llvm-21/bin/../lib/libLLVM.so.21.1+0x44fa1c7)
 #<!-- -->2 0x00007f746b2efe87 (/usr/lib/llvm-21/bin/../lib/libLLVM.so.21.1+0x4433e87)
 #<!-- -->3 0x00007f7466954330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->4 0x00007f747275a972 clang::ento::MemRegion::getAsOffset() const (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x33da972)
 #<!-- -->5 0x00007f7472792d92 (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x3412d92)
 #<!-- -->6 0x00007f7472766147 clang::ento::ProgramState::bindDefaultInitial(clang::ento::SVal, clang::ento::SVal, clang::LocationContext const*) const (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x33e6147)
 #<!-- -->7 0x00007f74728a9de6 (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x3529de6)
 #<!-- -->8 0x00007f74728b2444 (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x3532444)
 #<!-- -->9 0x00007f74728b116f (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x353116f)
#<!-- -->10 0x00007f74726ef81e clang::ento::CheckerManager::runCheckersForEvalCall(clang::ento::ExplodedNodeSet&amp;, clang::ento::ExplodedNodeSet const&amp;, clang::ento::CallEvent const&amp;, clang::ento::ExprEngine&amp;, clang::ento::EvalCallOptions const&amp;) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x336f81e)
#<!-- -->11 0x00007f747273bf0e clang::ento::ExprEngine::evalCall(clang::ento::ExplodedNodeSet&amp;, clang::ento::ExplodedNode*, clang::ento::CallEvent const&amp;) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x33bbf0e)
#<!-- -->12 0x00007f747273bc97 clang::ento::ExprEngine::VisitCallExpr(clang::CallExpr const*, clang::ento::ExplodedNode*, clang::ento::ExplodedNodeSet&amp;) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x33bbc97)
#<!-- -->13 0x00007f7472719922 clang::ento::ExprEngine::Visit(clang::Stmt const*, clang::ento::ExplodedNode*, clang::ento::ExplodedNodeSet&amp;) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x3399922)
#<!-- -->14 0x00007f7472715b3a clang::ento::ExprEngine::ProcessStmt(clang::Stmt const*, clang::ento::ExplodedNode*) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x3395b3a)
#<!-- -->15 0x00007f74726f6f57 clang::ento::CoreEngine::dispatchWorkItem(clang::ento::ExplodedNode*, clang::ProgramPoint, clang::ento::WorkListUnit const&amp;) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x3376f57)
#<!-- -->16 0x00007f74726f683c clang::ento::CoreEngine::ExecuteWorkList(clang::LocationContext const*, unsigned int, llvm::IntrusiveRefCntPtr&lt;clang::ento::ProgramState const&gt;) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x337683c)
#<!-- -->17 0x00007f74729ff34c (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x367f34c)
#<!-- -->18 0x00007f74729fddc8 (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x367ddc8)
#<!-- -->19 0x00007f74708082c9 clang::ParseAST(clang::Sema&amp;, bool, bool) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x14882c9)
#<!-- -->20 0x00007f74724076d2 clang::FrontendAction::Execute() (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x30876d2)
#<!-- -->21 0x00007f747237e134 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x2ffe134)
#<!-- -->22 0x00007f747248f37c clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x310f37c)
#<!-- -->23 0x000055f89ef4e31b cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/usr/lib/llvm-21/bin/clang+0x1331b)
#<!-- -->24 0x000055f89ef4b024 (/usr/lib/llvm-21/bin/clang+0x10024)
#<!-- -->25 0x00007f7472042172 (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x2cc2172)
#<!-- -->26 0x00007f746b2efb07 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/usr/lib/llvm-21/bin/../lib/libLLVM.so.21.1+0x4433b07)
#<!-- -->27 0x00007f74720419c6 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x2cc19c6)
#<!-- -->28 0x00007f7472004f02 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x2c84f02)
#<!-- -->29 0x00007f74720050ef clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x2c850ef)
#<!-- -->30 0x00007f7472023866 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x2ca3866)
#<!-- -->31 0x000055f89ef4aaf9 clang_main(int, char**, llvm::ToolContext const&amp;) (/usr/lib/llvm-21/bin/clang+0xfaf9)
#<!-- -->32 0x000055f89ef58ef3 main (/usr/lib/llvm-21/bin/clang+0x1def3)
#<!-- -->33 0x00007f74669391ca (/lib/x86_64-linux-gnu/libc.so.6+0x2a1ca)
#<!-- -->34 0x00007f746693928b __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x2a28b)
#<!-- -->35 0x000055f89ef48d25 _start (/usr/lib/llvm-21/bin/clang+0xdd25)
clang: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Ubuntu clang version 21.1.8 (++20251221032922+2078da43e25a-1~exp1~20251221153059.70)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/lib/llvm-21/bin
clang: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang: note: diagnostic msg: /tmp/crash-90c3aa.c
clang: note: diagnostic msg: /tmp/crash-90c3aa.sh
clang: note: diagnostic msg: 

********************
```
Requested files are attached.

[crash-90c3aa.sh](https://github.com/user-attachments/files/25552713/crash-90c3aa.sh)
[crash-90c3aa.c](https://github.com/user-attachments/files/25552714/crash-90c3aa.c)
</details>


---

### Comment 3 - steakhal

Also crashes on trunk https://godbolt.org/z/6v7W8K3Eh

---

### Comment 4 - steakhal

The fix will land in clang-21.1.1 - backport is alreaady in progress.

---

