# LLVM crash on aarch64-linux

**Author:** Tom-Hubrecht
**URL:** https://github.com/llvm/llvm-project/issues/168953
**Status:** Closed
**Labels:** clang-tidy, obsolete, incomplete, crash
**Closed Date:** 2025-11-22T11:19:58Z

## Body

```
Stack dump:
0. Program arguments: /nix/store/qiz8siw1kk1yhxlcazlk4rqb6lfagydc-clang-tools-19.1.7/bin/clang-tidy -header-filter=lix/[^/]+/.*\\.hh -p=/build/source/build/meson/clang-tidy -quiet -load=/nix/store/5f2z74xar6p7gyqbrb9y7kf8bhbchz60-lix-clang-tidy-checks-0.1/lib/liblix-clang-tidy.so --warnings-as-errors=* /build/source/lix/legacy/nix-channel.cc
1. <eof> parser at end of file
2. ASTMatcher: Processing 'lix-charptrcast' against:
   CXXReinterpretCastExpr : </nix/store/pwlmqqj29ykgiklkpz08h7mzygfad3x1-gcc-14.3.0/include/c++/14.3.0/ostream:690:24, col:57>
--- Bound Nodes Begin ---
    reinterpret-cast-expr - { CXXReinterpretCastExpr : </nix/store/pwlmqqj29ykgiklkpz08h7mzygfad3x1-gcc-14.3.0/include/c++/14.3.0/ostream:690:24, col:57> }
--- Bound Nodes End ---
Stack dump without symbol names (ensure you have llvm-symbolizer in your PATH or set the environment var `LLVM_SYMBOLIZER_PATH` to point to it):
0  libLLVM.so.19.1      0x0000ffffebe8f0c8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) + 72
1  libLLVM.so.19.1      0x0000ffffebe8ca5c
2  linux-vdso.so.1      0x0000fffff7ffa970 __kernel_rt_sigreturn + 0
3  libc.so.6            0x0000ffffeaabd258
4  libLLVM.so.19.1      0x0000ffffebe61c84 llvm::raw_svector_ostream::write_impl(char const*, unsigned long) + 68
5  libLLVM.so.19.1      0x0000ffffebe6369c llvm::raw_ostream::write(char const*, unsigned long) + 540
6  libLLVM.so.19.1      0x0000ffffebe25220 llvm::Twine::str[abi:cxx11]() const + 192
7  liblix-clang-tidy.so 0x0000ffffea726588 nix::clang_tidy::CharPtrCastCheck::check(clang::ast_matchers::MatchFinder::MatchResult const&) + 668
8  libclang-cpp.so.19.1 0x0000fffff5135e30
9  libclang-cpp.so.19.1 0x0000fffff5167428 clang::ast_matchers::internal::BoundNodesTreeBuilder::visitMatches(clang::ast_matchers::internal::BoundNodesTreeBuilder::Visitor*) + 200
10 libclang-cpp.so.19.1 0x0000fffff5138194
11 libclang-cpp.so.19.1 0x0000fffff516141c
12 libclang-cpp.so.19.1 0x0000fffff5159498
13 libclang-cpp.so.19.1 0x0000fffff5159930
14 libclang-cpp.so.19.1 0x0000fffff515c7bc
15 libclang-cpp.so.19.1 0x0000fffff5150cb4
16 libclang-cpp.so.19.1 0x0000fffff51525a4
17 libclang-cpp.so.19.1 0x0000fffff5150a10
18 libclang-cpp.so.19.1 0x0000fffff51525a4
19 libclang-cpp.so.19.1 0x0000fffff51526e4
20 libclang-cpp.so.19.1 0x0000fffff516044c
21 libclang-cpp.so.19.1 0x0000fffff51525a4
22 libclang-cpp.so.19.1 0x0000fffff51526e4
23 libclang-cpp.so.19.1 0x0000fffff51608d4
24 libclang-cpp.so.19.1 0x0000fffff51525a4
25 libclang-cpp.so.19.1 0x0000fffff5152ce4 clang::ast_matchers::MatchFinder::matchAST(clang::ASTContext&) + 324
26 libclang-cpp.so.19.1 0x0000fffff6a8e054 clang::MultiplexConsumer::HandleTranslationUnit(clang::ASTContext&) + 52
27 libclang-cpp.so.19.1 0x0000fffff479048c clang::ParseAST(clang::Sema&, bool, bool) + 1132
28 libclang-cpp.so.19.1 0x0000fffff6a57fc8 clang::FrontendAction::Execute() + 232
29 libclang-cpp.so.19.1 0x0000fffff69e8fa8 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) + 1480
30 libclang-cpp.so.19.1 0x0000fffff6c46640 clang::tooling::FrontendActionFactory::runInvocation(std::shared_ptr<clang::CompilerInvocation>, clang::FileManager*, std::shared_ptr<clang::PCHContainerOperations>, clang::DiagnosticConsumer*) + 352
31 clang-tidy           0x0000000000e28ae4
32 libclang-cpp.so.19.1 0x0000fffff6c41ca0 clang::tooling::ToolInvocation::runInvocation(char const*, clang::driver::Compilation*, std::shared_ptr<clang::CompilerInvocation>, std::shared_ptr<clang::PCHContainerOperations>) + 128
33 libclang-cpp.so.19.1 0x0000fffff6c42c3c clang::tooling::ToolInvocation::run() + 1116
34 libclang-cpp.so.19.1 0x0000fffff6c444fc clang::tooling::ClangTool::run(clang::tooling::ToolAction*) + 1692
35 clang-tidy           0x0000000000e29b04 clang::tidy::runClangTidy(clang::tidy::ClangTidyContext&, clang::tooling::CompilationDatabase const&, llvm::ArrayRef<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>>, llvm::IntrusiveRefCntPtr<llvm::vfs::OverlayFileSystem>, bool, bool, llvm::StringRef) + 804
36 clang-tidy           0x00000000005e5148 clang::tidy::clangTidyMain(int, char const**) + 3464
37 libc.so.6            0x0000ffffeaa362b4
38 libc.so.6            0x0000ffffeaa36398 __libc_start_main + 152
39 clang-tidy           0x00000000005dda70 _start + 48
/build/source/lix/legacy/nix-channel.cc: terminated by signal 11
```

Found with Lix CI ( https://buildkite.com/organizations/lix-project/pipelines/lix/builds/6239/jobs/019aa33d-4d10-4dde-b1d8-2ac859630701/log )


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-tidy

Author: Tom Hubrecht (Tom-Hubrecht)

<details>
```
Stack dump:
0. Program arguments: /nix/store/qiz8siw1kk1yhxlcazlk4rqb6lfagydc-clang-tools-19.1.7/bin/clang-tidy -header-filter=lix/[^/]+/.*\\.hh -p=/build/source/build/meson/clang-tidy -quiet -load=/nix/store/5f2z74xar6p7gyqbrb9y7kf8bhbchz60-lix-clang-tidy-checks-0.1/lib/liblix-clang-tidy.so --warnings-as-errors=* /build/source/lix/legacy/nix-channel.cc
1. &lt;eof&gt; parser at end of file
2. ASTMatcher: Processing 'lix-charptrcast' against:
   CXXReinterpretCastExpr : &lt;/nix/store/pwlmqqj29ykgiklkpz08h7mzygfad3x1-gcc-14.3.0/include/c++/14.3.0/ostream:690:24, col:57&gt;
--- Bound Nodes Begin ---
    reinterpret-cast-expr - { CXXReinterpretCastExpr : &lt;/nix/store/pwlmqqj29ykgiklkpz08h7mzygfad3x1-gcc-14.3.0/include/c++/14.3.0/ostream:690:24, col:57&gt; }
--- Bound Nodes End ---
Stack dump without symbol names (ensure you have llvm-symbolizer in your PATH or set the environment var `LLVM_SYMBOLIZER_PATH` to point to it):
0  libLLVM.so.19.1      0x0000ffffebe8f0c8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) + 72
1  libLLVM.so.19.1      0x0000ffffebe8ca5c
2  linux-vdso.so.1      0x0000fffff7ffa970 __kernel_rt_sigreturn + 0
3  libc.so.6            0x0000ffffeaabd258
4  libLLVM.so.19.1      0x0000ffffebe61c84 llvm::raw_svector_ostream::write_impl(char const*, unsigned long) + 68
5  libLLVM.so.19.1      0x0000ffffebe6369c llvm::raw_ostream::write(char const*, unsigned long) + 540
6  libLLVM.so.19.1      0x0000ffffebe25220 llvm::Twine::str[abi:cxx11]() const + 192
7  liblix-clang-tidy.so 0x0000ffffea726588 nix::clang_tidy::CharPtrCastCheck::check(clang::ast_matchers::MatchFinder::MatchResult const&amp;) + 668
8  libclang-cpp.so.19.1 0x0000fffff5135e30
9  libclang-cpp.so.19.1 0x0000fffff5167428 clang::ast_matchers::internal::BoundNodesTreeBuilder::visitMatches(clang::ast_matchers::internal::BoundNodesTreeBuilder::Visitor*) + 200
10 libclang-cpp.so.19.1 0x0000fffff5138194
11 libclang-cpp.so.19.1 0x0000fffff516141c
12 libclang-cpp.so.19.1 0x0000fffff5159498
13 libclang-cpp.so.19.1 0x0000fffff5159930
14 libclang-cpp.so.19.1 0x0000fffff515c7bc
15 libclang-cpp.so.19.1 0x0000fffff5150cb4
16 libclang-cpp.so.19.1 0x0000fffff51525a4
17 libclang-cpp.so.19.1 0x0000fffff5150a10
18 libclang-cpp.so.19.1 0x0000fffff51525a4
19 libclang-cpp.so.19.1 0x0000fffff51526e4
20 libclang-cpp.so.19.1 0x0000fffff516044c
21 libclang-cpp.so.19.1 0x0000fffff51525a4
22 libclang-cpp.so.19.1 0x0000fffff51526e4
23 libclang-cpp.so.19.1 0x0000fffff51608d4
24 libclang-cpp.so.19.1 0x0000fffff51525a4
25 libclang-cpp.so.19.1 0x0000fffff5152ce4 clang::ast_matchers::MatchFinder::matchAST(clang::ASTContext&amp;) + 324
26 libclang-cpp.so.19.1 0x0000fffff6a8e054 clang::MultiplexConsumer::HandleTranslationUnit(clang::ASTContext&amp;) + 52
27 libclang-cpp.so.19.1 0x0000fffff479048c clang::ParseAST(clang::Sema&amp;, bool, bool) + 1132
28 libclang-cpp.so.19.1 0x0000fffff6a57fc8 clang::FrontendAction::Execute() + 232
29 libclang-cpp.so.19.1 0x0000fffff69e8fa8 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) + 1480
30 libclang-cpp.so.19.1 0x0000fffff6c46640 clang::tooling::FrontendActionFactory::runInvocation(std::shared_ptr&lt;clang::CompilerInvocation&gt;, clang::FileManager*, std::shared_ptr&lt;clang::PCHContainerOperations&gt;, clang::DiagnosticConsumer*) + 352
31 clang-tidy           0x0000000000e28ae4
32 libclang-cpp.so.19.1 0x0000fffff6c41ca0 clang::tooling::ToolInvocation::runInvocation(char const*, clang::driver::Compilation*, std::shared_ptr&lt;clang::CompilerInvocation&gt;, std::shared_ptr&lt;clang::PCHContainerOperations&gt;) + 128
33 libclang-cpp.so.19.1 0x0000fffff6c42c3c clang::tooling::ToolInvocation::run() + 1116
34 libclang-cpp.so.19.1 0x0000fffff6c444fc clang::tooling::ClangTool::run(clang::tooling::ToolAction*) + 1692
35 clang-tidy           0x0000000000e29b04 clang::tidy::runClangTidy(clang::tidy::ClangTidyContext&amp;, clang::tooling::CompilationDatabase const&amp;, llvm::ArrayRef&lt;std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;&gt;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::OverlayFileSystem&gt;, bool, bool, llvm::StringRef) + 804
36 clang-tidy           0x00000000005e5148 clang::tidy::clangTidyMain(int, char const**) + 3464
37 libc.so.6            0x0000ffffeaa362b4
38 libc.so.6            0x0000ffffeaa36398 __libc_start_main + 152
39 clang-tidy           0x00000000005dda70 _start + 48
/build/source/lix/legacy/nix-channel.cc: terminated by signal 11
```

Found with Lix CI ( https://buildkite.com/organizations/lix-project/pipelines/lix/builds/6239/jobs/019aa33d-4d10-4dde-b1d8-2ac859630701/log )

</details>


---

### Comment 2 - EugeneZelenko

Could you please try 21 or `main` branch? Only most recent release is maintained.


---

### Comment 3 - Tom-Hubrecht

It indeeds works correctly with 21, sorry for the bother

---

