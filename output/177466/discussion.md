# [clangd] crashes with a SIGSEGV when receiving a textDocument/didOpen request where the URI points to a directory instead of a file.

**Author:** mingodad
**URL:** https://github.com/llvm/llvm-project/issues/177466
**Status:** Open
**Labels:** good first issue, clangd, crash

## Body

**Description:**
clangd crashes with a SIGSEGV when receiving a textDocument/didOpen or textDocument/documentSymbol request where the URI points to a directory instead of a file.

**Steps to reproduce:**
Start clangd in LSP mode.
Send the following JSON-RPC request:
code
JSON
{"jsonrpc":"2.0","method":"textDocument/didOpen","params":{"textDocument":{"uri":"file:///","languageId":"c","version":1,"text":""}}}

**Expected result:**
The server should return an error response or ignore the request.

**Actual result:**
clangd crashes.

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
 #0 0x000055676cd8e2f8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/home/mingo/local/clang-20/bin/clangd+0xa4f2f8)
 #1 0x000055676cd8c1ee llvm::sys::RunSignalHandlers() (/home/mingo/local/clang-20/bin/clangd+0xa4d1ee)
 #2 0x000055676cd8ec9d SignalHandler(int) Signals.cpp:0:0
 #3 0x00007f831aff1980 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x12980)
 #4 0x000055676d327160 bool llvm::DenseMapBase<llvm::DenseMap<clang::FileEntryRef, clang::SrcMgr::ContentCache*, llvm::DenseMapInfo<clang::FileEntryRef, void>, llvm::detail::DenseMapPair<clang::FileEntryRef, clang::SrcMgr::ContentCache*>>, clang::FileEntryRef, clang::SrcMgr::ContentCache*, llvm::DenseMapInfo<clang::FileEntryRef, void>, llvm::detail::DenseMapPair<clang::FileEntryRef, clang::SrcMgr::ContentCache*>>::LookupBucketFor<clang::FileEntryRef>(clang::FileEntryRef const&, llvm::detail::DenseMapPair<clang::FileEntryRef, clang::SrcMgr::ContentCache*>*&) (/home/mingo/local/clang-20/bin/clangd+0xfe8160)
 #5 0x000055676d31ef77 llvm::DenseMapBase<llvm::DenseMap<clang::FileEntryRef, clang::SrcMgr::ContentCache*, llvm::DenseMapInfo<clang::FileEntryRef, void>, llvm::detail::DenseMapPair<clang::FileEntryRef, clang::SrcMgr::ContentCache*>>, clang::FileEntryRef, clang::SrcMgr::ContentCache*, llvm::DenseMapInfo<clang::FileEntryRef, void>, llvm::detail::DenseMapPair<clang::FileEntryRef, clang::SrcMgr::ContentCache*>>::operator[](clang::FileEntryRef const&) (/home/mingo/local/clang-20/bin/clangd+0xfdff77)
 #6 0x000055676d31efc7 clang::SourceManager::getOrCreateContentCache(clang::FileEntryRef, bool) (/home/mingo/local/clang-20/bin/clangd+0xfdffc7)
 #7 0x000055676d326799 clang::SourceManagerForFile::SourceManagerForFile(llvm::StringRef, llvm::StringRef) (/home/mingo/local/clang-20/bin/clangd+0xfe7799)
 #8 0x000055677057ebec clang::tooling::(anonymous namespace)::getOffsetAfterTokenSequence(llvm::StringRef, llvm::StringRef, clang::tooling::IncludeStyle const&, llvm::function_ref<unsigned int (clang::SourceManager const&, clang::Lexer&, clang::Token&)>) HeaderIncludes.cpp:0:0
 #9 0x000055677057d867 clang::tooling::HeaderIncludes::HeaderIncludes(llvm::StringRef, llvm::StringRef, clang::tooling::IncludeStyle const&) (/home/mingo/local/clang-20/bin/clangd+0x423e867)
#10 0x000055676dd96cc7 clang::clangd::ParsedAST::build(llvm::StringRef, clang::clangd::ParseInputs const&, std::unique_ptr<clang::CompilerInvocation, std::default_delete<clang::CompilerInvocation>>, llvm::ArrayRef<clang::clangd::Diag>, std::shared_ptr<clang::clangd::PreambleData const>) (/home/mingo/local/clang-20/bin/clangd+0x1a57cc7)
#11 0x000055676de33982 clang::clangd::(anonymous namespace)::ASTWorker::generateDiagnostics(std::unique_ptr<clang::CompilerInvocation, std::default_delete<clang::CompilerInvocation>>, clang::clangd::ParseInputs, std::vector<clang::clangd::Diag, std::allocator<clang::clangd::Diag>>) TUScheduler.cpp:0:0
#12 0x000055676de333fa clang::clangd::(anonymous namespace)::ASTWorker::updatePreamble(std::unique_ptr<clang::CompilerInvocation, std::default_delete<clang::CompilerInvocation>>, clang::clangd::ParseInputs, std::shared_ptr<clang::clangd::PreambleData const>, std::vector<clang::clangd::Diag, std::allocator<clang::clangd::Diag>>, clang::clangd::WantDiagnostics)::$_0::operator()() TUScheduler.cpp:0:0
#13 0x000055676de30049 clang::clangd::(anonymous namespace)::ASTWorker::runTask(llvm::StringRef, llvm::function_ref<void ()>) TUScheduler.cpp:0:0
#14 0x000055676de2fb2a void llvm::detail::UniqueFunctionBase<void>::CallImpl<clang::clangd::(anonymous namespace)::ASTWorker::create(llvm::StringRef, clang::clangd::GlobalCompilationDatabase const&, clang::clangd::TUScheduler::ASTCache&, clang::clangd::TUScheduler::HeaderIncluderCache&, clang::clangd::AsyncTaskRunner*, clang::clangd::Semaphore&, clang::clangd::TUScheduler::Options const&, clang::clangd::ParsingCallbacks&)::$_0>(void*) TUScheduler.cpp:0:0
#15 0x000055676df9c2c2 void* llvm::thread::ThreadProxy<std::tuple<clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>)::$_1>>(void*) Threading.cpp:0:0
#16 0x00007f831afe66db start_thread /build/glibc-CVJwZb/glibc-2.27/nptl/pthread_create.c:463:0
#17 0x00007f8319cc561f __clone /build/glibc-CVJwZb/glibc-2.27/misc/../sysdeps/unix/sysv/linux/x86_64/clone.S:97:0
Signalled during AST worker action: Build AST
  Filename: /
  Directory: 
  Command Line: /home/mingo/local/clang-20/bin/clang -xobjective-c++-header -resource-dir=/home/mingo/local/clang-20/lib/clang/20 -- /
  Version: 1
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clangd

Author: Domingo Alvarez Duarte (mingodad)

<details>
**Description:**
clangd crashes with a SIGSEGV when receiving a textDocument/didOpen or textDocument/documentSymbol request where the URI points to a directory instead of a file.

**Steps to reproduce:**
Start clangd in LSP mode.
Send the following JSON-RPC request:
code
JSON
{"jsonrpc":"2.0","method":"textDocument/didOpen","params":{"textDocument":{"uri":"file:///","languageId":"c","version":1,"text":""}}}

**Expected result:**
The server should return an error response or ignore the request.

**Actual result:**
clangd crashes.

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
 #<!-- -->0 0x000055676cd8e2f8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/home/mingo/local/clang-20/bin/clangd+0xa4f2f8)
 #<!-- -->1 0x000055676cd8c1ee llvm::sys::RunSignalHandlers() (/home/mingo/local/clang-20/bin/clangd+0xa4d1ee)
 #<!-- -->2 0x000055676cd8ec9d SignalHandler(int) Signals.cpp:0:0
 #<!-- -->3 0x00007f831aff1980 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x12980)
 #<!-- -->4 0x000055676d327160 bool llvm::DenseMapBase&lt;llvm::DenseMap&lt;clang::FileEntryRef, clang::SrcMgr::ContentCache*, llvm::DenseMapInfo&lt;clang::FileEntryRef, void&gt;, llvm::detail::DenseMapPair&lt;clang::FileEntryRef, clang::SrcMgr::ContentCache*&gt;&gt;, clang::FileEntryRef, clang::SrcMgr::ContentCache*, llvm::DenseMapInfo&lt;clang::FileEntryRef, void&gt;, llvm::detail::DenseMapPair&lt;clang::FileEntryRef, clang::SrcMgr::ContentCache*&gt;&gt;::LookupBucketFor&lt;clang::FileEntryRef&gt;(clang::FileEntryRef const&amp;, llvm::detail::DenseMapPair&lt;clang::FileEntryRef, clang::SrcMgr::ContentCache*&gt;*&amp;) (/home/mingo/local/clang-20/bin/clangd+0xfe8160)
 #<!-- -->5 0x000055676d31ef77 llvm::DenseMapBase&lt;llvm::DenseMap&lt;clang::FileEntryRef, clang::SrcMgr::ContentCache*, llvm::DenseMapInfo&lt;clang::FileEntryRef, void&gt;, llvm::detail::DenseMapPair&lt;clang::FileEntryRef, clang::SrcMgr::ContentCache*&gt;&gt;, clang::FileEntryRef, clang::SrcMgr::ContentCache*, llvm::DenseMapInfo&lt;clang::FileEntryRef, void&gt;, llvm::detail::DenseMapPair&lt;clang::FileEntryRef, clang::SrcMgr::ContentCache*&gt;&gt;::operator[](clang::FileEntryRef const&amp;) (/home/mingo/local/clang-20/bin/clangd+0xfdff77)
 #<!-- -->6 0x000055676d31efc7 clang::SourceManager::getOrCreateContentCache(clang::FileEntryRef, bool) (/home/mingo/local/clang-20/bin/clangd+0xfdffc7)
 #<!-- -->7 0x000055676d326799 clang::SourceManagerForFile::SourceManagerForFile(llvm::StringRef, llvm::StringRef) (/home/mingo/local/clang-20/bin/clangd+0xfe7799)
 #<!-- -->8 0x000055677057ebec clang::tooling::(anonymous namespace)::getOffsetAfterTokenSequence(llvm::StringRef, llvm::StringRef, clang::tooling::IncludeStyle const&amp;, llvm::function_ref&lt;unsigned int (clang::SourceManager const&amp;, clang::Lexer&amp;, clang::Token&amp;)&gt;) HeaderIncludes.cpp:0:0
 #<!-- -->9 0x000055677057d867 clang::tooling::HeaderIncludes::HeaderIncludes(llvm::StringRef, llvm::StringRef, clang::tooling::IncludeStyle const&amp;) (/home/mingo/local/clang-20/bin/clangd+0x423e867)
#<!-- -->10 0x000055676dd96cc7 clang::clangd::ParsedAST::build(llvm::StringRef, clang::clangd::ParseInputs const&amp;, std::unique_ptr&lt;clang::CompilerInvocation, std::default_delete&lt;clang::CompilerInvocation&gt;&gt;, llvm::ArrayRef&lt;clang::clangd::Diag&gt;, std::shared_ptr&lt;clang::clangd::PreambleData const&gt;) (/home/mingo/local/clang-20/bin/clangd+0x1a57cc7)
#<!-- -->11 0x000055676de33982 clang::clangd::(anonymous namespace)::ASTWorker::generateDiagnostics(std::unique_ptr&lt;clang::CompilerInvocation, std::default_delete&lt;clang::CompilerInvocation&gt;&gt;, clang::clangd::ParseInputs, std::vector&lt;clang::clangd::Diag, std::allocator&lt;clang::clangd::Diag&gt;&gt;) TUScheduler.cpp:0:0
#<!-- -->12 0x000055676de333fa clang::clangd::(anonymous namespace)::ASTWorker::updatePreamble(std::unique_ptr&lt;clang::CompilerInvocation, std::default_delete&lt;clang::CompilerInvocation&gt;&gt;, clang::clangd::ParseInputs, std::shared_ptr&lt;clang::clangd::PreambleData const&gt;, std::vector&lt;clang::clangd::Diag, std::allocator&lt;clang::clangd::Diag&gt;&gt;, clang::clangd::WantDiagnostics)::$_0::operator()() TUScheduler.cpp:0:0
#<!-- -->13 0x000055676de30049 clang::clangd::(anonymous namespace)::ASTWorker::runTask(llvm::StringRef, llvm::function_ref&lt;void ()&gt;) TUScheduler.cpp:0:0
#<!-- -->14 0x000055676de2fb2a void llvm::detail::UniqueFunctionBase&lt;void&gt;::CallImpl&lt;clang::clangd::(anonymous namespace)::ASTWorker::create(llvm::StringRef, clang::clangd::GlobalCompilationDatabase const&amp;, clang::clangd::TUScheduler::ASTCache&amp;, clang::clangd::TUScheduler::HeaderIncluderCache&amp;, clang::clangd::AsyncTaskRunner*, clang::clangd::Semaphore&amp;, clang::clangd::TUScheduler::Options const&amp;, clang::clangd::ParsingCallbacks&amp;)::$_0&gt;(void*) TUScheduler.cpp:0:0
#<!-- -->15 0x000055676df9c2c2 void* llvm::thread::ThreadProxy&lt;std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&gt;&gt;(void*) Threading.cpp:0:0
#<!-- -->16 0x00007f831afe66db start_thread /build/glibc-CVJwZb/glibc-2.27/nptl/pthread_create.c:463:0
#<!-- -->17 0x00007f8319cc561f __clone /build/glibc-CVJwZb/glibc-2.27/misc/../sysdeps/unix/sysv/linux/x86_64/clone.S:97:0
Signalled during AST worker action: Build AST
  Filename: /
  Directory: 
  Command Line: /home/mingo/local/clang-20/bin/clang -xobjective-c++-header -resource-dir=/home/mingo/local/clang-20/lib/clang/20 -- /
  Version: 1
```
</details>


---

### Comment 2 - llvmbot


Hi!

This issue may be a good introductory issue for people new to working on LLVM. If you would like to work on this issue, your first steps are:

1. Check that no other contributor is working on this issue. If someone is assigned to the issue or claimed to be working on it, ping the person. After one week without a response, the assignee may be changed.
1. Leave a comment indicating that you are working on the issue, or just create a [pull request](https://github.com/llvm/llvm-project/pulls) after following the steps below. [Mention](https://docs.github.com/en/issues/tracking-your-work-with-issues/linking-a-pull-request-to-an-issue) this issue in the description of the pull request.
1. Fix the issue locally.
1. [Run the test suite](https://llvm.org/docs/TestingGuide.html#unit-and-regression-tests) locally. Remember that the subdirectories under `test/` create fine-grained testing targets, so you can e.g. use `make check-clang-ast` to only run Clang's AST tests.
1. Create a Git commit.
1. Run [`git clang-format HEAD~1`](https://clang.llvm.org/docs/ClangFormat.html#git-integration) to format your changes.
1. Open a [pull request](https://github.com/llvm/llvm-project/pulls) to the [upstream repository](https://github.com/llvm/llvm-project) on GitHub. Detailed instructions can be found [in GitHub's documentation](https://docs.github.com/en/pull-requests/collaborating-with-pull-requests/proposing-changes-to-your-work-with-pull-requests/creating-a-pull-request). [Mention](https://docs.github.com/en/issues/tracking-your-work-with-issues/linking-a-pull-request-to-an-issue) this issue in the description of the pull request.

If you have any further questions about this issue, don't hesitate to ask via a comment in the thread below.



---

### Comment 3 - llvmbot


@llvm/issue-subscribers-good-first-issue

Author: Domingo Alvarez Duarte (mingodad)

<details>
**Description:**
clangd crashes with a SIGSEGV when receiving a textDocument/didOpen or textDocument/documentSymbol request where the URI points to a directory instead of a file.

**Steps to reproduce:**
Start clangd in LSP mode.
Send the following JSON-RPC request:
code
JSON
{"jsonrpc":"2.0","method":"textDocument/didOpen","params":{"textDocument":{"uri":"file:///","languageId":"c","version":1,"text":""}}}

**Expected result:**
The server should return an error response or ignore the request.

**Actual result:**
clangd crashes.

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
 #<!-- -->0 0x000055676cd8e2f8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/home/mingo/local/clang-20/bin/clangd+0xa4f2f8)
 #<!-- -->1 0x000055676cd8c1ee llvm::sys::RunSignalHandlers() (/home/mingo/local/clang-20/bin/clangd+0xa4d1ee)
 #<!-- -->2 0x000055676cd8ec9d SignalHandler(int) Signals.cpp:0:0
 #<!-- -->3 0x00007f831aff1980 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x12980)
 #<!-- -->4 0x000055676d327160 bool llvm::DenseMapBase&lt;llvm::DenseMap&lt;clang::FileEntryRef, clang::SrcMgr::ContentCache*, llvm::DenseMapInfo&lt;clang::FileEntryRef, void&gt;, llvm::detail::DenseMapPair&lt;clang::FileEntryRef, clang::SrcMgr::ContentCache*&gt;&gt;, clang::FileEntryRef, clang::SrcMgr::ContentCache*, llvm::DenseMapInfo&lt;clang::FileEntryRef, void&gt;, llvm::detail::DenseMapPair&lt;clang::FileEntryRef, clang::SrcMgr::ContentCache*&gt;&gt;::LookupBucketFor&lt;clang::FileEntryRef&gt;(clang::FileEntryRef const&amp;, llvm::detail::DenseMapPair&lt;clang::FileEntryRef, clang::SrcMgr::ContentCache*&gt;*&amp;) (/home/mingo/local/clang-20/bin/clangd+0xfe8160)
 #<!-- -->5 0x000055676d31ef77 llvm::DenseMapBase&lt;llvm::DenseMap&lt;clang::FileEntryRef, clang::SrcMgr::ContentCache*, llvm::DenseMapInfo&lt;clang::FileEntryRef, void&gt;, llvm::detail::DenseMapPair&lt;clang::FileEntryRef, clang::SrcMgr::ContentCache*&gt;&gt;, clang::FileEntryRef, clang::SrcMgr::ContentCache*, llvm::DenseMapInfo&lt;clang::FileEntryRef, void&gt;, llvm::detail::DenseMapPair&lt;clang::FileEntryRef, clang::SrcMgr::ContentCache*&gt;&gt;::operator[](clang::FileEntryRef const&amp;) (/home/mingo/local/clang-20/bin/clangd+0xfdff77)
 #<!-- -->6 0x000055676d31efc7 clang::SourceManager::getOrCreateContentCache(clang::FileEntryRef, bool) (/home/mingo/local/clang-20/bin/clangd+0xfdffc7)
 #<!-- -->7 0x000055676d326799 clang::SourceManagerForFile::SourceManagerForFile(llvm::StringRef, llvm::StringRef) (/home/mingo/local/clang-20/bin/clangd+0xfe7799)
 #<!-- -->8 0x000055677057ebec clang::tooling::(anonymous namespace)::getOffsetAfterTokenSequence(llvm::StringRef, llvm::StringRef, clang::tooling::IncludeStyle const&amp;, llvm::function_ref&lt;unsigned int (clang::SourceManager const&amp;, clang::Lexer&amp;, clang::Token&amp;)&gt;) HeaderIncludes.cpp:0:0
 #<!-- -->9 0x000055677057d867 clang::tooling::HeaderIncludes::HeaderIncludes(llvm::StringRef, llvm::StringRef, clang::tooling::IncludeStyle const&amp;) (/home/mingo/local/clang-20/bin/clangd+0x423e867)
#<!-- -->10 0x000055676dd96cc7 clang::clangd::ParsedAST::build(llvm::StringRef, clang::clangd::ParseInputs const&amp;, std::unique_ptr&lt;clang::CompilerInvocation, std::default_delete&lt;clang::CompilerInvocation&gt;&gt;, llvm::ArrayRef&lt;clang::clangd::Diag&gt;, std::shared_ptr&lt;clang::clangd::PreambleData const&gt;) (/home/mingo/local/clang-20/bin/clangd+0x1a57cc7)
#<!-- -->11 0x000055676de33982 clang::clangd::(anonymous namespace)::ASTWorker::generateDiagnostics(std::unique_ptr&lt;clang::CompilerInvocation, std::default_delete&lt;clang::CompilerInvocation&gt;&gt;, clang::clangd::ParseInputs, std::vector&lt;clang::clangd::Diag, std::allocator&lt;clang::clangd::Diag&gt;&gt;) TUScheduler.cpp:0:0
#<!-- -->12 0x000055676de333fa clang::clangd::(anonymous namespace)::ASTWorker::updatePreamble(std::unique_ptr&lt;clang::CompilerInvocation, std::default_delete&lt;clang::CompilerInvocation&gt;&gt;, clang::clangd::ParseInputs, std::shared_ptr&lt;clang::clangd::PreambleData const&gt;, std::vector&lt;clang::clangd::Diag, std::allocator&lt;clang::clangd::Diag&gt;&gt;, clang::clangd::WantDiagnostics)::$_0::operator()() TUScheduler.cpp:0:0
#<!-- -->13 0x000055676de30049 clang::clangd::(anonymous namespace)::ASTWorker::runTask(llvm::StringRef, llvm::function_ref&lt;void ()&gt;) TUScheduler.cpp:0:0
#<!-- -->14 0x000055676de2fb2a void llvm::detail::UniqueFunctionBase&lt;void&gt;::CallImpl&lt;clang::clangd::(anonymous namespace)::ASTWorker::create(llvm::StringRef, clang::clangd::GlobalCompilationDatabase const&amp;, clang::clangd::TUScheduler::ASTCache&amp;, clang::clangd::TUScheduler::HeaderIncluderCache&amp;, clang::clangd::AsyncTaskRunner*, clang::clangd::Semaphore&amp;, clang::clangd::TUScheduler::Options const&amp;, clang::clangd::ParsingCallbacks&amp;)::$_0&gt;(void*) TUScheduler.cpp:0:0
#<!-- -->15 0x000055676df9c2c2 void* llvm::thread::ThreadProxy&lt;std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&gt;&gt;(void*) Threading.cpp:0:0
#<!-- -->16 0x00007f831afe66db start_thread /build/glibc-CVJwZb/glibc-2.27/nptl/pthread_create.c:463:0
#<!-- -->17 0x00007f8319cc561f __clone /build/glibc-CVJwZb/glibc-2.27/misc/../sysdeps/unix/sysv/linux/x86_64/clone.S:97:0
Signalled during AST worker action: Build AST
  Filename: /
  Directory: 
  Command Line: /home/mingo/local/clang-20/bin/clang -xobjective-c++-header -resource-dir=/home/mingo/local/clang-20/lib/clang/20 -- /
  Version: 1
```
</details>


---

### Comment 4 - HighCommander4

This seems like a good first issue for someone looking to get into contributing to clangd.

I'm happy to provide more concrete starting points if someone expresses interest in working on this.

---

### Comment 5 - HighCommander4

@mingodad Are you aware of a published client which is affected by this?

---

### Comment 6 - dhr412

Hi, Could I work on this issue?

---

### Comment 7 - HighCommander4

> Hi, Could I work on this issue?

Sure.

The first step would be to build clangd locally, following the instructions at https://github.com/llvm/llvm-project/tree/main/clang-tools-extra/clangd#building-and-testing-clangd.

For this issue, I would recommend a debug build, which is done by using `-DCMAKE_BUILD_TYPE=Debug` rather than `-DCMAKE_BUILD_TYPE=Release` in the cmake command line.

The next step after that would be to reproduce the crash. If @mingodad points us to a client to reproduce with, we can use that.

---

### Comment 8 - mingodad

bellow is the [quickjs](https://bellard.org/quickjs/) script that I was using when clangd crashed, the command to execute it is `qjs clangd-query-bug.qjs test.c` notice that `test.c` doesn't need to exists to see the crash it, seems that the key trigger is when clangd receives this `{"jsonrpc":"2.0","method":"textDocument/didOpen","params":{"textDocument":{"uri":"file:///","languageId":"c","version":1,"text":""}}}`.
```javascript
import * as std from "std";
import * as os from "os";

const SymbolKind = {
    File: 1, Module: 2, Namespace: 3, Package: 4, Class: 5,
    Method: 6, Property: 7, Field: 8, Constructor: 9, Enum: 10,
    Interface: 11, Function: 12, Variable: 13, Constant: 14,
    Struct: 23, EnumMember: 22
};

const SIGTERM = 15;

class ClangdClient {
    static KIND_LABELS = {
        [SymbolKind.File]: "file", [SymbolKind.Class]: "struct",
        [SymbolKind.Enum]: "enum", [SymbolKind.Interface]: "typedef",
        [SymbolKind.Function]: "func", [SymbolKind.Variable]: "var",
        [SymbolKind.Struct]: "struct", [SymbolKind.Constant]: "const"
    };

    constructor(filePath) {
        const realPathRes = os.realpath(filePath);
        this.absPath = Array.isArray(realPathRes) ? realPathRes[0] : realPathRes;
        this.uri = `file://${this.absPath.startsWith('/') ? '' : '/'}${this.absPath}`;
        this.requestId = 0;

        const pipeIn = os.pipe();
        const pipeOut = os.pipe();

        this.pid = os.exec(["clangd", "--log=error", "--check-locations=false", "--header-insertion=never"], {
            stdin: pipeIn[0], stdout: pipeOut[1], stderr: std.err.fileno(), block: false
        });

        os.close(pipeIn[0]);
        os.close(pipeOut[1]);
        this.stdin = std.fdopen(pipeIn[1], "w");
        this.stdout = std.fdopen(pipeOut[0], "r");
    }

    _decodeUTF8(buf) {
        let out = "", i = 0;
        while (i < buf.length) {
            let c = buf[i++];
            if (c < 0x80) out += String.fromCharCode(c);
            else if (c < 0xE0) out += String.fromCharCode(((c & 0x1F) << 6) | (buf[i++] & 0x3F));
            else if (c < 0xF0) out += String.fromCharCode(((c & 0x0F) << 12) | ((buf[i++] & 0x3F) << 6) | (buf[i++] & 0x3F));
        }
        return out;
    }

    _send(method, params, isRequest = true) {
        const msg = { jsonrpc: "2.0", method: method, params: params };
        if (isRequest) msg.id = ++this.requestId;
        const jsonMsg = JSON.stringify(msg);
        let byteLen = 0;
        for (let i = 0; i < jsonMsg.length; i++) {
            let c = jsonMsg.charCodeAt(i);
            byteLen += (c < 0x80) ? 1 : (c < 0x800) ? 2 : 3;
        }
        this.stdin.printf("Content-Length: %d\r\n\r\n%s", byteLen, jsonMsg);
        this.stdin.flush();
        return isRequest ? this.requestId : null;
    }

    _read() {
        let line = this.stdout.getline();
        while (line !== null && !line.startsWith("Content-Length:")) line = this.stdout.getline();
        if (line === null) return null;
        const length = parseInt(line.split(":")[1].trim());
        this.stdout.getline();
        const buf = new Uint8Array(length);
        const readLen = this.stdout.read(buf.buffer, 0, length);
        try { return JSON.parse(this._decodeUTF8(buf.slice(0, readLen))); } catch (e) { return null; }
    }

    init() {
        this._send("initialize", {
            rootUri: `file://${os.getcwd()[0]}`,
            capabilities: { textDocument: {
                hover: { contentFormat: ["plaintext"] },
                documentSymbol: { hierarchicalDocumentSymbolSupport: true }
            } }
        });
        let initId = this.requestId;
        while(true) { let res = this._read(); if (res && res.id === initId) break; }
        this._send("initialized", {}, false);
        this._send("textDocument/didOpen", {
            textDocument: { uri: this.uri, languageId: "c", version: 1, text: std.loadFile(this.absPath) || "" }
        }, false);
    }

    getHover(line, character) {
        const reqId = this._send("textDocument/hover", {
            textDocument: { uri: this.uri },
            position: { line: line, character: character }
        });
        while (true) {
            const res = this._read();
            if (res && res.id === reqId) return res.result?.contents?.value || res.result?.contents || "";
            if (!res) break;
        }
    }

    getSymbols() {
        this.init();
        const reqId = this._send("textDocument/documentSymbol", { textDocument: { uri: this.uri } });
        while (true) {
            const res = this._read();
            if (res && res.id === reqId) return res.result || [];
        }
    }

    isAnonymous(name) {
        return !name || name.includes("anonymous") || name.includes("unnamed") || name.startsWith("(");
    }

    toLLM(symbols, depth = 0) {
        let lines = [];
        for (let i = 0; i < symbols.length; i++) {
            const s = symbols[i];
            const next = symbols[i + 1];
            const indent = "  ".repeat(depth);
            if (!s.range) continue;

            const startLine = s.range.start.line;
            const endLine = s.range.end.line;
            const loc = (endLine - startLine) + 1;
            let kind = ClangdClient.KIND_LABELS[s.kind] || "sym";

            // 1. Tag declarations vs definitions
            if (s.kind === SymbolKind.Function && loc === 1) kind = "decl";

            // 2. Merge Anonymous Structs with variables
            if (this.isAnonymous(s.name) && (s.kind === SymbolKind.Struct || s.kind === SymbolKind.Class)) {
                if (next && next.range && next.range.start.line === s.range.start.line) {
                    const nextLoc = next.range.end.line - next.range.start.line + 1;
                    lines.push(`${indent}[STRUCT] ${next.name} (L${startLine+1}-${next.range.end.line+1}, ${nextLoc} LOC)`);
                    if (s.children) lines.push(this.toLLM(s.children, depth + 1));
                    i++; continue;
                }
            }

            // 3. Strict Documentation Filtering (Human prose only)
            let docStr = "";
            const charPos = s.selectionRange ? s.selectionRange.start.character : s.range.start.character;
            const raw = this.getHover(startLine, charPos);
            if (raw) {
                docStr = raw.split("\n")
                    .map(l => l.trim())
                    .filter(l => {
                        // Reject all boilerplate
                        if (!l || l.startsWith("---") || l.startsWith("```") || l.includes("→")) return false;
                        if (l.startsWith("Type:") || l.startsWith("Parameters:") || l.startsWith("Value =")) return false;
                        if (l.includes("aka ") || l.includes("Offset:") || l.includes("Size:")) return false;
                        if (l.startsWith("- `")) return false; // Filter list of parameters
                        // Reject if it's just repeating the code
                        if (l.includes(s.name) && (l.includes("(") || l.includes("*") || l.includes(";"))) return false;
                        return true;
                    })
                    .join(" ");
            }

            const detail = s.detail ? `: ${s.detail.replace(s.name, "").trim()}` : "";
            const isBlock = (s.kind === SymbolKind.Function || s.kind === SymbolKind.Struct || s.kind === SymbolKind.Enum || s.children);

            // 4. Clean formatting
            let head = `[${kind.toUpperCase()}] ${s.name}`;
            let range = (isBlock && loc > 1) ? `(L${startLine+1}-${endLine+1}, ${loc} LOC)` : `(L${startLine+1})`;

            lines.push(`${indent}${head} ${range}${detail}`);
            if (docStr && docStr.length > 5) {
                lines.push(`${indent}  > Doc: ${docStr.substring(0, 200)}`);
            }

            if (s.children && s.children.length > 0) {
                lines.push(this.toLLM(s.children, depth + 1));
            }
        }
        return lines.join("\n");
    }

    close() { os.kill(this.pid, SIGTERM); }
}

function main() {
    const args = scriptArgs.slice(1);
    let filePath = args.find(a => !a.startsWith("-"));
    if (!filePath) { std.err.puts("Usage: qjs script.js <file>\n"); std.exit(1); }

    const client = new ClangdClient(filePath);
    client.init();
    const symbols = client.getSymbols();

    std.out.puts(`FILE STRUCTURE: ${client.absPath}\n${"=".repeat(40)}\n`);
    std.out.puts(client.toLLM(symbols) + "\n");
    client.close();
}
main();

//clang-20-env qjs "clangd-query-bug.qjs" test.c
```

---

