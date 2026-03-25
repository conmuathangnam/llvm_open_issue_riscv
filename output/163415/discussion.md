# clang-tidy crash with "LLVM ERROR: Cannot chdir into <sourcefile>"

**Author:** diemoschwarz
**URL:** https://github.com/llvm/llvm-project/issues/163415
**Status:** Open
**Labels:** clang-tidy, crash

## Body

Trying to follow the instructions [here](https://blog.bkryza.com/posts/compile-commands-json-gallery/#xcode), the error message says cannot chdir into a __file__(!?)

```
%/opt/homebrew/Cellar/llvm/21.1.3/bin/clang-tidy -p ../build/xcode/ $PWD/src/maxmubu.c
Error while processing /Users/schwarz/src/mubu-git/maxmubu/src/maxmubu.c.
LLVM ERROR: Cannot chdir into "/Users/schwarz/src/mubu-git/maxmubu/src/imubudatagui.c"!
PLEASE submit a bug report to https://github.com/Homebrew/homebrew-core/issues and include the crash backtrace.
Stack dump:
0.	Program arguments: /opt/homebrew/Cellar/llvm/21.1.3/bin/clang-tidy -p /Users/schwarz/src/mubu-git/build/xcode /Users/schwarz/src/mubu-git/maxmubu/src/maxmubu.c
 #0 0x0000000116cb36dc llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/homebrew/Cellar/llvm/21.1.3/lib/libLLVM.dylib+0x3dff6dc)
 #1 0x0000000116cb3f20 SignalHandler(int, __siginfo*, void*) (/opt/homebrew/Cellar/llvm/21.1.3/lib/libLLVM.dylib+0x3dfff20)
 #2 0x0000000190f516a4 (/usr/lib/system/libsystem_platform.dylib+0x1804ad6a4)
 #3 0x0000000190f1788c (/usr/lib/system/libsystem_pthread.dylib+0x18047388c)
 #4 0x0000000190e20a3c (/usr/lib/system/libsystem_c.dylib+0x18037ca3c)
 #5 0x0000000112f31d48 write_retry(int, char const*, unsigned long) (/opt/homebrew/Cellar/llvm/21.1.3/lib/libLLVM.dylib+0x7dd48)
 #6 0x00000001078375e8 clang::tooling::ClangTool::run(clang::tooling::ToolAction*) (/opt/homebrew/Cellar/llvm/21.1.3/lib/libclang-cpp.dylib+0x1ac75e8)
 #7 0x0000000100d775d8 clang::tidy::runClangTidy(clang::tidy::ClangTidyContext&, clang::tooling::CompilationDatabase const&, llvm::ArrayRef<std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>>>, llvm::IntrusiveRefCntPtr<llvm::vfs::OverlayFileSystem>, bool, bool, llvm::StringRef) (/opt/homebrew/Cellar/llvm/21.1.3/bin/clang-tidy+0x1000035d8)
 #8 0x0000000100da3560 clang::tidy::clangTidyMain(int, char const**) (/opt/homebrew/Cellar/llvm/21.1.3/bin/clang-tidy+0x10002f560)
 #9 0x0000000190b76b98
Abort
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-tidy

Author: diemo (diemoschwarz)

<details>
Trying to follow the instructions [here](https://blog.bkryza.com/posts/compile-commands-json-gallery/#xcode), the error message says cannot chdir into a __file__(!?)

```
%/opt/homebrew/Cellar/llvm/21.1.3/bin/clang-tidy -p ../build/xcode/ $PWD/src/maxmubu.c
Error while processing /Users/schwarz/src/mubu-git/maxmubu/src/maxmubu.c.
LLVM ERROR: Cannot chdir into "/Users/schwarz/src/mubu-git/maxmubu/src/imubudatagui.c"!
PLEASE submit a bug report to https://github.com/Homebrew/homebrew-core/issues and include the crash backtrace.
Stack dump:
0.	Program arguments: /opt/homebrew/Cellar/llvm/21.1.3/bin/clang-tidy -p /Users/schwarz/src/mubu-git/build/xcode /Users/schwarz/src/mubu-git/maxmubu/src/maxmubu.c
 #<!-- -->0 0x0000000116cb36dc llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/homebrew/Cellar/llvm/21.1.3/lib/libLLVM.dylib+0x3dff6dc)
 #<!-- -->1 0x0000000116cb3f20 SignalHandler(int, __siginfo*, void*) (/opt/homebrew/Cellar/llvm/21.1.3/lib/libLLVM.dylib+0x3dfff20)
 #<!-- -->2 0x0000000190f516a4 (/usr/lib/system/libsystem_platform.dylib+0x1804ad6a4)
 #<!-- -->3 0x0000000190f1788c (/usr/lib/system/libsystem_pthread.dylib+0x18047388c)
 #<!-- -->4 0x0000000190e20a3c (/usr/lib/system/libsystem_c.dylib+0x18037ca3c)
 #<!-- -->5 0x0000000112f31d48 write_retry(int, char const*, unsigned long) (/opt/homebrew/Cellar/llvm/21.1.3/lib/libLLVM.dylib+0x7dd48)
 #<!-- -->6 0x00000001078375e8 clang::tooling::ClangTool::run(clang::tooling::ToolAction*) (/opt/homebrew/Cellar/llvm/21.1.3/lib/libclang-cpp.dylib+0x1ac75e8)
 #<!-- -->7 0x0000000100d775d8 clang::tidy::runClangTidy(clang::tidy::ClangTidyContext&amp;, clang::tooling::CompilationDatabase const&amp;, llvm::ArrayRef&lt;std::__1::basic_string&lt;char, std::__1::char_traits&lt;char&gt;, std::__1::allocator&lt;char&gt;&gt;&gt;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::OverlayFileSystem&gt;, bool, bool, llvm::StringRef) (/opt/homebrew/Cellar/llvm/21.1.3/bin/clang-tidy+0x1000035d8)
 #<!-- -->8 0x0000000100da3560 clang::tidy::clangTidyMain(int, char const**) (/opt/homebrew/Cellar/llvm/21.1.3/bin/clang-tidy+0x10002f560)
 #<!-- -->9 0x0000000190b76b98
Abort
```
</details>


---

### Comment 2 - localspook

Can you post the `compile_commands.json`?

---

### Comment 3 - vbvictor

This must be fixed by https://github.com/llvm/llvm-project/pull/156873.
Could you try clang-tidy-22? You probably have to build it from scratch in macos... I only know that we have "development branch" deb packages in https://apt.llvm.org/

---

### Comment 4 - diemoschwarz

Thanks, I was somehow able to generate something that circumvents the error. Here is the original, FWIW: [compile_commands.json.zip](https://github.com/user-attachments/files/22925104/compile_commands.json.zip)

Now I'm stuck at a different error: `LLVM ERROR: unknown module format`
I don't know if this is referring to the compile_commands.json format or to the C++ modules feature that was mentioned [here](https://blog.bkryza.com/posts/compile-commands-json-gallery/#xcode), or something else. Here's the [compile_commands.json.zip](https://github.com/user-attachments/files/22925233/compile_commands.json.zip).   (I apologize in advance if this is no longer llvm-related.)

```
%/opt/homebrew/Cellar/llvm/21.1.3/bin/clang-tidy -p ../build/osx-macho/ $PWD/mubu.process.cpp 
LLVM ERROR: unknown module format
PLEASE submit a bug report to https://github.com/Homebrew/homebrew-core/issues and include the crash backtrace.
Stack dump:
0.	Program arguments: /opt/homebrew/Cellar/llvm/21.1.3/bin/clang-tidy -p /Users/schwarz/src/mubu-git/maxmubu/build/osx-macho /Users/schwarz/src/mubu-git/maxmubu/src/mubu.process.cpp
 #0 0x0000000115ddb6dc llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/homebrew/Cellar/llvm/21.1.3/lib/libLLVM.dylib+0x3dff6dc)
 #1 0x0000000115ddbf20 SignalHandler(int, __siginfo*, void*) (/opt/homebrew/Cellar/llvm/21.1.3/lib/libLLVM.dylib+0x3dfff20)
 #2 0x0000000190f516a4 (/usr/lib/system/libsystem_platform.dylib+0x1804ad6a4)
 #3 0x0000000190f1788c (/usr/lib/system/libsystem_pthread.dylib+0x18047388c)
 #4 0x0000000190e20a3c (/usr/lib/system/libsystem_c.dylib+0x18037ca3c)
 #5 0x0000000112059d48 write_retry(int, char const*, unsigned long) (/opt/homebrew/Cellar/llvm/21.1.3/lib/libLLVM.dylib+0x7dd48)
 #6 0x0000000112059b0c llvm::report_fatal_error(llvm::Twine const&, bool) (/opt/homebrew/Cellar/llvm/21.1.3/lib/libLLVM.dylib+0x7db0c)
 #7 0x000000010682eb00 void std::__1::vector<std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>>, std::__1::allocator<std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>>>>::__init_with_size[abi:nn190102]<std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>> const*, std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>> const*>(std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>> const*, std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>> const*, unsigned long) (/opt/homebrew/Cellar/llvm/21.1.3/lib/libclang-cpp.dylib+0x1996b00)
 #8 0x0000000106840ac4 clang::CompilerInstance::createPreprocessor(clang::TranslationUnitKind) (/opt/homebrew/Cellar/llvm/21.1.3/lib/libclang-cpp.dylib+0x19a8ac4)
 #9 0x00000001068a9328 clang::FrontendAction::BeginSourceFile(clang::CompilerInstance&, clang::FrontendInputFile const&) (/opt/homebrew/Cellar/llvm/21.1.3/lib/libclang-cpp.dylib+0x1a11328)
#10 0x0000000106843d5c clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/homebrew/Cellar/llvm/21.1.3/lib/libclang-cpp.dylib+0x19abd5c)
#11 0x000000010695df34 clang::tooling::FrontendActionFactory::runInvocation(std::__1::shared_ptr<clang::CompilerInvocation>, clang::FileManager*, std::__1::shared_ptr<clang::PCHContainerOperations>, clang::DiagnosticConsumer*) (/opt/homebrew/Cellar/llvm/21.1.3/lib/libclang-cpp.dylib+0x1ac5f34)
#12 0x0000000100137480 clang::tidy::runClangTidy(clang::tidy::ClangTidyContext&, clang::tooling::CompilationDatabase const&, llvm::ArrayRef<std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>>>, llvm::IntrusiveRefCntPtr<llvm::vfs::OverlayFileSystem>, bool, bool, llvm::StringRef)::ActionFactory::runInvocation(std::__1::shared_ptr<clang::CompilerInvocation>, clang::FileManager*, std::__1::shared_ptr<clang::PCHContainerOperations>, clang::DiagnosticConsumer*) (/opt/homebrew/Cellar/llvm/21.1.3/bin/clang-tidy+0x100007480)
#13 0x000000010695dc30 clang::tooling::ToolInvocation::runInvocation(char const*, clang::driver::Compilation*, std::__1::shared_ptr<clang::CompilerInvocation>, std::__1::shared_ptr<clang::PCHContainerOperations>) (/opt/homebrew/Cellar/llvm/21.1.3/lib/libclang-cpp.dylib+0x1ac5c30)
#14 0x000000010695cc0c clang::tooling::ToolInvocation::run() (/opt/homebrew/Cellar/llvm/21.1.3/lib/libclang-cpp.dylib+0x1ac4c0c)
#15 0x000000010695ed14 clang::tooling::ClangTool::run(clang::tooling::ToolAction*) (/opt/homebrew/Cellar/llvm/21.1.3/lib/libclang-cpp.dylib+0x1ac6d14)
#16 0x00000001001335d8 clang::tidy::runClangTidy(clang::tidy::ClangTidyContext&, clang::tooling::CompilationDatabase const&, llvm::ArrayRef<std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>>>, llvm::IntrusiveRefCntPtr<llvm::vfs::OverlayFileSystem>, bool, bool, llvm::StringRef) (/opt/homebrew/Cellar/llvm/21.1.3/bin/clang-tidy+0x1000035d8)
#17 0x000000010015f560 clang::tidy::clangTidyMain(int, char const**) (/opt/homebrew/Cellar/llvm/21.1.3/bin/clang-tidy+0x10002f560)
#18 0x0000000190b76b98
Abort
```

---

### Comment 5 - carlosgalvezp

> This must be fixed by https://github.com/llvm/llvm-project/pull/156873.

I'm not sure, the original compilation database contains this: `"directory" : "/",` which is not empty?

---

### Comment 6 - localspook

For the first crash: the `compile_commands.json` contains entries like:
```json
   {
      "command" : "builtin-ScanDependencies ...",
      "directory" : "/Users/schwarz/src/mubu-git/maxmubu/src/imubudatagui.c",
      "file" : "/Users/schwarz/src/mubu-git/maxmubu/src/maxmubu.c"
   },
```
With a file in the `directory` field, which violates [the spec](https://clang.llvm.org/docs/JSONCompilationDatabase.html) (it seems like all the `builtin-ScanDependencies` calls have this problem). This should probably be reported as a bug against whatever tool generated this (we should still fix the crash though, crashing isn't a good response).

For the second crash: the error seems to be with the format of the precompiled headers (?):

<details>
<summary>Where the error happens</summary>

https://github.com/llvm/llvm-project/blob/16dfd317f38ebfc0bd39a5e20e2a8851daa4f8b8/clang/include/clang/Frontend/CompilerInstance.h#L603-L629

</details>

I see the `compile_commands.json` does contain a lot of PCHs, but I'm out of my depth here, I'm not sure what the underlying issue might be.


---

### Comment 7 - diemoschwarz

FYI, the `compile_commands.json` was generated by xcode 16.4 (with the Apple-provided version of clang) and xcpretty.
I managed to get past the module format error in the homebrew-installed clang by adding `-fno-implicit-modules` and `-fno-modules` to the xcode compile flags directly (there is no build setting in the IDE for disabling C++ modules!).
I agree that the error messages could be more explicit and should ideally not provoke crashes.
Thanks!

---

### Comment 8 - vbvictor

> > This must be fixed by [#156873](https://github.com/llvm/llvm-project/pull/156873).
> 
> I'm not sure, the original compilation database contains this: `"directory" : "/",` which is not empty?

Apologies, rushed with "fixes" a bit. I didn't see the second test-case on my phone which still had `LLVM ERROR: Cannot chdir into` error. Thought the message was completely gone in favor of better diagnostics

---

