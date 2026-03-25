# Clang-21 Crash: fatal error: error in backend: can't open file './xxxxxx': No such file or directory

**Author:** hyf751175784
**URL:** https://github.com/llvm/llvm-project/issues/147187
**Status:** Closed
**Labels:** good first issue, clang:frontend, confirmed, crash
**Closed Date:** 2025-07-25T14:20:31Z

## Body

### Description
I encountered a crash in Clang-21 when compiling the following code with compilation options:
```c
int a;

```
Options:
```sh
-Xclang -fsanitize-system-ignorelist=./xxxxxx -fbounds-check -fno-sanitize-link-runtime -fsigned-char 
```

### Clang Version

```sh
$ clang -v
Ubuntu clang version 21.0.0 (++20250501082025+9b1051281e43-1~exp1~20250501082214.882)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/lib/llvm-21/bin
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/11
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/13
Selected GCC installation: /usr/lib/gcc/x86_64-linux-gnu/13
Candidate multilib: .;@m64
Selected multilib: .;@m64
```

### Command
The compilation command used was:
```sh
clang bug.c -Xclang -fsanitize-system-ignorelist=./optfuzz -fbounds-check -fno-sanitize-link-runtime -fsigned-char  -c -o bug.o
```

### Error Message
```sh
clang: warning: argument unused during compilation: '-fbounds-check' [-Wunused-command-line-argument]
fatal error: error in backend: can't open file './optfuzz': No such file or directory
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -Xclang -fsanitize-system-ignorelist=./optfuzz -fbounds-check -fno-sanitize-link-runtime -fsigned-char <source>
 #0 0x0000000003f7af98 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3f7af98)
 #1 0x0000000003f783c4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3f783c4)
 #2 0x0000000003ebd246 llvm::CrashRecoveryContext::HandleExit(int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3ebd246)
 #3 0x0000000003f6f8de llvm::sys::Process::Exit(int, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3f6f8de)
 #4 0x0000000000d8e460 LLVMErrorHandler(void*, char const*, bool) cc1_main.cpp:0:0
 #5 0x0000000003ec8033 llvm::report_fatal_error(llvm::Twine const&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3ec8033)
 #6 0x0000000003ec816c (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3ec816c)
 #7 0x0000000007e99568 clang::SanitizerSpecialCaseList::createOrDie(std::vector<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>, std::allocator<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>>> const&, llvm::vfs::FileSystem&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x7e99568)
 #8 0x0000000007e97fb9 clang::NoSanitizeList::NoSanitizeList(std::vector<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>, std::allocator<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>>> const&, clang::SourceManager&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x7e97fb9)
 #9 0x00000000074c74eb clang::ASTContext::ASTContext(clang::LangOptions&, clang::SourceManager&, clang::IdentifierTable&, clang::SelectorTable&, clang::Builtin::Context&, clang::TranslationUnitKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x74c74eb)
#10 0x0000000004b94382 clang::CompilerInstance::createASTContext() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4b94382)
#11 0x0000000004c2c3cd clang::FrontendAction::BeginSourceFile(clang::CompilerInstance&, clang::FrontendInputFile const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4c2c3cd)
#12 0x0000000004ba490a clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4ba490a)
#13 0x0000000004d191f1 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4d191f1)
#14 0x0000000000d90b9f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xd90b9f)
#15 0x0000000000d8777a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#16 0x00000000049955b9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#17 0x0000000003ebd184 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3ebd184)
#18 0x0000000004995bcf clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#19 0x0000000004957b9d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4957b9d)
#20 0x0000000004958c2e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4958c2e)
#21 0x00000000049617c5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x49617c5)
#22 0x0000000000d8d04f clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xd8d04f)
#23 0x0000000000c42504 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xc42504)
#24 0x0000711292829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#25 0x0000711292829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#26 0x0000000000d87225 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xd87225)
clang: error: clang frontend command failed with exit code 70 (use -v to see invocation)
Compiler returned: 70
```

The issue can also be reproduced on Compiler Explorer:https://godbolt.org/z/oWPP496x5
Please let me know if you need any more details.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Huang Yifan (hyf751175784)

<details>
### Description
I encountered a crash in Clang-21 when compiling the following code with compilation options:
```c
int a;

```
Options:
```sh
-Xclang -fsanitize-system-ignorelist=./optfuzz -fbounds-check -fno-sanitize-link-runtime -fsigned-char 
```

### Clang Version

```sh
$ clang -v
Ubuntu clang version 21.0.0 (++20250501082025+9b1051281e43-1~exp1~20250501082214.882)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/lib/llvm-21/bin
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/11
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/13
Selected GCC installation: /usr/lib/gcc/x86_64-linux-gnu/13
Candidate multilib: .;@<!-- -->m64
Selected multilib: .;@<!-- -->m64
```

### Command
The compilation command used was:
```sh
clang bug.c -Xclang -fsanitize-system-ignorelist=./optfuzz -fbounds-check -fno-sanitize-link-runtime -fsigned-char  -c -o bug.o
```

### Error Message
```sh
clang: warning: argument unused during compilation: '-fbounds-check' [-Wunused-command-line-argument]
fatal error: error in backend: can't open file './optfuzz': No such file or directory
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -Xclang -fsanitize-system-ignorelist=./optfuzz -fbounds-check -fno-sanitize-link-runtime -fsigned-char &lt;source&gt;
 #<!-- -->0 0x0000000003f7af98 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3f7af98)
 #<!-- -->1 0x0000000003f783c4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3f783c4)
 #<!-- -->2 0x0000000003ebd246 llvm::CrashRecoveryContext::HandleExit(int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3ebd246)
 #<!-- -->3 0x0000000003f6f8de llvm::sys::Process::Exit(int, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3f6f8de)
 #<!-- -->4 0x0000000000d8e460 LLVMErrorHandler(void*, char const*, bool) cc1_main.cpp:0:0
 #<!-- -->5 0x0000000003ec8033 llvm::report_fatal_error(llvm::Twine const&amp;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3ec8033)
 #<!-- -->6 0x0000000003ec816c (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3ec816c)
 #<!-- -->7 0x0000000007e99568 clang::SanitizerSpecialCaseList::createOrDie(std::vector&lt;std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;, std::allocator&lt;std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;&gt;&gt; const&amp;, llvm::vfs::FileSystem&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x7e99568)
 #<!-- -->8 0x0000000007e97fb9 clang::NoSanitizeList::NoSanitizeList(std::vector&lt;std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;, std::allocator&lt;std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;&gt;&gt; const&amp;, clang::SourceManager&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x7e97fb9)
 #<!-- -->9 0x00000000074c74eb clang::ASTContext::ASTContext(clang::LangOptions&amp;, clang::SourceManager&amp;, clang::IdentifierTable&amp;, clang::SelectorTable&amp;, clang::Builtin::Context&amp;, clang::TranslationUnitKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x74c74eb)
#<!-- -->10 0x0000000004b94382 clang::CompilerInstance::createASTContext() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4b94382)
#<!-- -->11 0x0000000004c2c3cd clang::FrontendAction::BeginSourceFile(clang::CompilerInstance&amp;, clang::FrontendInputFile const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4c2c3cd)
#<!-- -->12 0x0000000004ba490a clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4ba490a)
#<!-- -->13 0x0000000004d191f1 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4d191f1)
#<!-- -->14 0x0000000000d90b9f cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xd90b9f)
#<!-- -->15 0x0000000000d8777a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->16 0x00000000049955b9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->17 0x0000000003ebd184 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3ebd184)
#<!-- -->18 0x0000000004995bcf clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->19 0x0000000004957b9d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4957b9d)
#<!-- -->20 0x0000000004958c2e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4958c2e)
#<!-- -->21 0x00000000049617c5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x49617c5)
#<!-- -->22 0x0000000000d8d04f clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xd8d04f)
#<!-- -->23 0x0000000000c42504 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xc42504)
#<!-- -->24 0x0000711292829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->25 0x0000711292829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->26 0x0000000000d87225 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xd87225)
clang: error: clang frontend command failed with exit code 70 (use -v to see invocation)
Compiler returned: 70
```

The issue can also be reproduced on Compiler Explorer:https://godbolt.org/z/oWPP496x5
Please let me know if you need any more details.
</details>


---

### Comment 2 - hstk30-hw

Do you have the file `-fsanitize-system-ignorelist=./optfuzz` ？

---

### Comment 3 - hyf751175784

> Do you have the file `-fsanitize-system-ignorelist=./optfuzz` ？

This crash can be triggered when compiling with certain combinations of options even without any input file.



---

### Comment 4 - shafik

I don't think we should crash if the files does not exist, we should emit a diagnostic of some kind.

---

### Comment 5 - AaronBallman

> I don't think we should crash if the files does not exist, we should emit a diagnostic of some kind.

Agreed, this should be handled via a diagnostic IMO.

---

### Comment 6 - llvmbot


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

### Comment 7 - llvmbot


@llvm/issue-subscribers-good-first-issue

Author: Huang Yifan (hyf751175784)

<details>
### Description
I encountered a crash in Clang-21 when compiling the following code with compilation options:
```c
int a;

```
Options:
```sh
-Xclang -fsanitize-system-ignorelist=./xxxxxx -fbounds-check -fno-sanitize-link-runtime -fsigned-char 
```

### Clang Version

```sh
$ clang -v
Ubuntu clang version 21.0.0 (++20250501082025+9b1051281e43-1~exp1~20250501082214.882)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/lib/llvm-21/bin
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/11
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/13
Selected GCC installation: /usr/lib/gcc/x86_64-linux-gnu/13
Candidate multilib: .;@<!-- -->m64
Selected multilib: .;@<!-- -->m64
```

### Command
The compilation command used was:
```sh
clang bug.c -Xclang -fsanitize-system-ignorelist=./optfuzz -fbounds-check -fno-sanitize-link-runtime -fsigned-char  -c -o bug.o
```

### Error Message
```sh
clang: warning: argument unused during compilation: '-fbounds-check' [-Wunused-command-line-argument]
fatal error: error in backend: can't open file './optfuzz': No such file or directory
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -Xclang -fsanitize-system-ignorelist=./optfuzz -fbounds-check -fno-sanitize-link-runtime -fsigned-char &lt;source&gt;
 #<!-- -->0 0x0000000003f7af98 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3f7af98)
 #<!-- -->1 0x0000000003f783c4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3f783c4)
 #<!-- -->2 0x0000000003ebd246 llvm::CrashRecoveryContext::HandleExit(int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3ebd246)
 #<!-- -->3 0x0000000003f6f8de llvm::sys::Process::Exit(int, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3f6f8de)
 #<!-- -->4 0x0000000000d8e460 LLVMErrorHandler(void*, char const*, bool) cc1_main.cpp:0:0
 #<!-- -->5 0x0000000003ec8033 llvm::report_fatal_error(llvm::Twine const&amp;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3ec8033)
 #<!-- -->6 0x0000000003ec816c (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3ec816c)
 #<!-- -->7 0x0000000007e99568 clang::SanitizerSpecialCaseList::createOrDie(std::vector&lt;std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;, std::allocator&lt;std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;&gt;&gt; const&amp;, llvm::vfs::FileSystem&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x7e99568)
 #<!-- -->8 0x0000000007e97fb9 clang::NoSanitizeList::NoSanitizeList(std::vector&lt;std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;, std::allocator&lt;std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;&gt;&gt; const&amp;, clang::SourceManager&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x7e97fb9)
 #<!-- -->9 0x00000000074c74eb clang::ASTContext::ASTContext(clang::LangOptions&amp;, clang::SourceManager&amp;, clang::IdentifierTable&amp;, clang::SelectorTable&amp;, clang::Builtin::Context&amp;, clang::TranslationUnitKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x74c74eb)
#<!-- -->10 0x0000000004b94382 clang::CompilerInstance::createASTContext() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4b94382)
#<!-- -->11 0x0000000004c2c3cd clang::FrontendAction::BeginSourceFile(clang::CompilerInstance&amp;, clang::FrontendInputFile const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4c2c3cd)
#<!-- -->12 0x0000000004ba490a clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4ba490a)
#<!-- -->13 0x0000000004d191f1 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4d191f1)
#<!-- -->14 0x0000000000d90b9f cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xd90b9f)
#<!-- -->15 0x0000000000d8777a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->16 0x00000000049955b9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->17 0x0000000003ebd184 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3ebd184)
#<!-- -->18 0x0000000004995bcf clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->19 0x0000000004957b9d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4957b9d)
#<!-- -->20 0x0000000004958c2e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4958c2e)
#<!-- -->21 0x00000000049617c5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x49617c5)
#<!-- -->22 0x0000000000d8d04f clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xd8d04f)
#<!-- -->23 0x0000000000c42504 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xc42504)
#<!-- -->24 0x0000711292829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->25 0x0000711292829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->26 0x0000000000d87225 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xd87225)
clang: error: clang frontend command failed with exit code 70 (use -v to see invocation)
Compiler returned: 70
```

The issue can also be reproduced on Compiler Explorer:https://godbolt.org/z/oWPP496x5
Please let me know if you need any more details.
</details>


---

### Comment 8 - woruyu

Hello! I'm happy to work on this issue. Thank you!@hstk30-hw

---

### Comment 9 - woruyu

@hstk30-hw This is my first time working on Clang frontend code — any suggestions or feedback on this issue would be very appreciated.

---

