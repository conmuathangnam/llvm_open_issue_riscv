# crashed in clang::Preprocessor::EnterMainSourceFile()

**Author:** jongmyeong-choi
**URL:** https://github.com/llvm/llvm-project/issues/139375
**Status:** Closed
**Labels:** clang:frontend, crash
**Closed Date:** 2025-05-15T13:36:29Z

## Body

Stack trace
```
 #0 0x00000001050c55e4 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/Users/cheese/git/llvm-project/build/bin/clang-21+0x1028515e4)
 #1 0x00000001050c36ac llvm::sys::RunSignalHandlers() (/Users/cheese/git/llvm-project/build/bin/clang-21+0x10284f6ac)
 #2 0x00000001050c4bb8 llvm::sys::CleanupOnSignal(unsigned long) (/Users/cheese/git/llvm-project/build/bin/clang-21+0x102850bb8)
 #3 0x000000010501dc34 CrashRecoverySignalHandler(int) (/Users/cheese/git/llvm-project/build/bin/clang-21+0x1027a9c34)
 #4 0x000000019ab57624 (/usr/lib/system/libsystem_platform.dylib+0x1804ab624)
 #5 0x000000019ab1d88c (/usr/lib/system/libsystem_pthread.dylib+0x18047188c)
 #6 0x000000019aa26c60 (/usr/lib/system/libsystem_c.dylib+0x18037ac60)
 #7 0x000000019aa25eec (/usr/lib/system/libsystem_c.dylib+0x180379eec)
 #8 0x0000000107d268c0 clang::Preprocessor::EnterMainSourceFile() (.cold.1) (/Users/cheese/git/llvm-project/build/bin/clang-21+0x1054b28c0)
 #9 0x00000001077ef608 clang::Preprocessor::SetCodeCompletionPoint(clang::FileEntryRef, unsigned int, unsigned int) (/Users/cheese/git/llvm-project/build/bin/clang-21+0x104f7b608)
#10 0x0000000105b5ab88 EnableCodeCompletion(clang::Preprocessor&, llvm::StringRef, unsigned int, unsigned int) (/Users/cheese/git/llvm-project/build/bin/clang-21+0x1032e6b88)
#11 0x0000000105b5a978 clang::CompilerInstance::createCodeCompletionConsumer() (/Users/cheese/git/llvm-project/build/bin/clang-21+0x1032e6978)
#12 0x0000000105bcaea0 clang::ASTFrontendAction::ExecuteAction() (/Users/cheese/git/llvm-project/build/bin/clang-21+0x103356ea0)
#13 0x0000000105bca8f4 clang::FrontendAction::Execute() (/Users/cheese/git/llvm-project/build/bin/clang-21+0x1033568f4)
#14 0x0000000105b5c78c clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/Users/cheese/git/llvm-project/build/bin/clang-21+0x1032e878c)
#15 0x0000000105c44b78 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/Users/cheese/git/llvm-project/build/bin/clang-21+0x1033d0b78)
#16 0x0000000102879dec cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/Users/cheese/git/llvm-project/build/bin/clang-21+0x100005dec)
#17 0x0000000102877ddc ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) (/Users/cheese/git/llvm-project/build/bin/clang-21+0x100003ddc)
#18 0x00000001059bedf8 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::__1::optional<llvm::StringRef>>, std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>>*, bool*) const::$_0>(long) (/Users/cheese/git/llvm-project/build/bin/clang-21+0x10314adf8)
#19 0x000000010501d8ec llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/Users/cheese/git/llvm-project/build/bin/clang-21+0x1027a98ec)
#20 0x00000001059be86c clang::driver::CC1Command::Execute(llvm::ArrayRef<std::__1::optional<llvm::StringRef>>, std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>>*, bool*) const (/Users/cheese/git/llvm-project/build/bin/clang-21+0x10314a86c)
#21 0x0000000105989dac clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/Users/cheese/git/llvm-project/build/bin/clang-21+0x103115dac)
#22 0x0000000105989fb4 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::__1::pair<int, clang::driver::Command const*>>&, bool) const (/Users/cheese/git/llvm-project/build/bin/clang-21+0x103115fb4)
#23 0x00000001059a4970 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::__1::pair<int, clang::driver::Command const*>>&) (/Users/cheese/git/llvm-project/build/bin/clang-21+0x103130970)
#24 0x0000000102876ea0 clang_main(int, char**, llvm::ToolContext const&) (/Users/cheese/git/llvm-project/build/bin/clang-21+0x100002ea0)
#25 0x00000001028839dc main (/Users/cheese/git/llvm-project/build/bin/clang-21+0x10000f9dc)
#26 0x000000019a77eb4c
```

input.cpp
```
int main() {
    std::cout;
    std::c
}
```

Command for reproduce
` ./bin/clang++ -fsyntax-only -Xclang -code-completion-at=input.cpp:5:0 input.cpp --target=x86_64-apple-macos11 --sysroot=/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk`

[input-55332e.cpp.txt](https://github.com/user-attachments/files/20139199/input-55332e.cpp.txt)
[input-55332e.sh.txt](https://github.com/user-attachments/files/20139198/input-55332e.sh.txt)






## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Jongmyeong Choi (jongmyeong-choi)

<details>
Stack trace
```
 #<!-- -->0 0x00000001050c55e4 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/Users/cheese/git/llvm-project/build/bin/clang-21+0x1028515e4)
 #<!-- -->1 0x00000001050c36ac llvm::sys::RunSignalHandlers() (/Users/cheese/git/llvm-project/build/bin/clang-21+0x10284f6ac)
 #<!-- -->2 0x00000001050c4bb8 llvm::sys::CleanupOnSignal(unsigned long) (/Users/cheese/git/llvm-project/build/bin/clang-21+0x102850bb8)
 #<!-- -->3 0x000000010501dc34 CrashRecoverySignalHandler(int) (/Users/cheese/git/llvm-project/build/bin/clang-21+0x1027a9c34)
 #<!-- -->4 0x000000019ab57624 (/usr/lib/system/libsystem_platform.dylib+0x1804ab624)
 #<!-- -->5 0x000000019ab1d88c (/usr/lib/system/libsystem_pthread.dylib+0x18047188c)
 #<!-- -->6 0x000000019aa26c60 (/usr/lib/system/libsystem_c.dylib+0x18037ac60)
 #<!-- -->7 0x000000019aa25eec (/usr/lib/system/libsystem_c.dylib+0x180379eec)
 #<!-- -->8 0x0000000107d268c0 clang::Preprocessor::EnterMainSourceFile() (.cold.1) (/Users/cheese/git/llvm-project/build/bin/clang-21+0x1054b28c0)
 #<!-- -->9 0x00000001077ef608 clang::Preprocessor::SetCodeCompletionPoint(clang::FileEntryRef, unsigned int, unsigned int) (/Users/cheese/git/llvm-project/build/bin/clang-21+0x104f7b608)
#<!-- -->10 0x0000000105b5ab88 EnableCodeCompletion(clang::Preprocessor&amp;, llvm::StringRef, unsigned int, unsigned int) (/Users/cheese/git/llvm-project/build/bin/clang-21+0x1032e6b88)
#<!-- -->11 0x0000000105b5a978 clang::CompilerInstance::createCodeCompletionConsumer() (/Users/cheese/git/llvm-project/build/bin/clang-21+0x1032e6978)
#<!-- -->12 0x0000000105bcaea0 clang::ASTFrontendAction::ExecuteAction() (/Users/cheese/git/llvm-project/build/bin/clang-21+0x103356ea0)
#<!-- -->13 0x0000000105bca8f4 clang::FrontendAction::Execute() (/Users/cheese/git/llvm-project/build/bin/clang-21+0x1033568f4)
#<!-- -->14 0x0000000105b5c78c clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/Users/cheese/git/llvm-project/build/bin/clang-21+0x1032e878c)
#<!-- -->15 0x0000000105c44b78 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/Users/cheese/git/llvm-project/build/bin/clang-21+0x1033d0b78)
#<!-- -->16 0x0000000102879dec cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/Users/cheese/git/llvm-project/build/bin/clang-21+0x100005dec)
#<!-- -->17 0x0000000102877ddc ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) (/Users/cheese/git/llvm-project/build/bin/clang-21+0x100003ddc)
#<!-- -->18 0x00000001059bedf8 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::__1::optional&lt;llvm::StringRef&gt;&gt;, std::__1::basic_string&lt;char, std::__1::char_traits&lt;char&gt;, std::__1::allocator&lt;char&gt;&gt;*, bool*) const::$_0&gt;(long) (/Users/cheese/git/llvm-project/build/bin/clang-21+0x10314adf8)
#<!-- -->19 0x000000010501d8ec llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/Users/cheese/git/llvm-project/build/bin/clang-21+0x1027a98ec)
#<!-- -->20 0x00000001059be86c clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::__1::optional&lt;llvm::StringRef&gt;&gt;, std::__1::basic_string&lt;char, std::__1::char_traits&lt;char&gt;, std::__1::allocator&lt;char&gt;&gt;*, bool*) const (/Users/cheese/git/llvm-project/build/bin/clang-21+0x10314a86c)
#<!-- -->21 0x0000000105989dac clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/Users/cheese/git/llvm-project/build/bin/clang-21+0x103115dac)
#<!-- -->22 0x0000000105989fb4 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::__1::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/Users/cheese/git/llvm-project/build/bin/clang-21+0x103115fb4)
#<!-- -->23 0x00000001059a4970 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::__1::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/Users/cheese/git/llvm-project/build/bin/clang-21+0x103130970)
#<!-- -->24 0x0000000102876ea0 clang_main(int, char**, llvm::ToolContext const&amp;) (/Users/cheese/git/llvm-project/build/bin/clang-21+0x100002ea0)
#<!-- -->25 0x00000001028839dc main (/Users/cheese/git/llvm-project/build/bin/clang-21+0x10000f9dc)
#<!-- -->26 0x000000019a77eb4c
```

input.cpp
```
int main() {
    std::cout;
    std::c
}
```

Command for reproduce
` ./bin/clang++ -fsyntax-only -Xclang -code-completion-at=input.cpp:5:0 input.cpp --target=x86_64-apple-macos11 --sysroot=/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk`

[input-55332e.cpp.txt](https://github.com/user-attachments/files/20139199/input-55332e.cpp.txt)
[input-55332e.sh.txt](https://github.com/user-attachments/files/20139198/input-55332e.sh.txt)





</details>


---

### Comment 2 - naveen-seth

I tried reproducing the issue using a debug build of trunk and ran into the following assert:

https://github.com/llvm/llvm-project/blob/9eeae5a5de41b22d31d9037be2acc45dd7af4192/clang/lib/Lex/Preprocessor.cpp#L398

Simplified reproducer:

```bash
clang++ -cc1 -code-completion-at=input.cpp:1:0 input.cpp
# or 
clang++ -cc1 -code-completion-at=input.cpp:0:1 input.cpp
```

Clang seems to use 1-based indexing for source-location arguments and the crash is caused because Clang does not reject the invalid column-number in `-code-completion-at=input.cpp:5:0`.

The PR below will cause Clang to reject the command-line input instead, same as it does for other bad inputs for column- or line-number.

---

