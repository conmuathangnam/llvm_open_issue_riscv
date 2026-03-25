# Crash in clang-tidy during `modernize-redundant-void-arg` check.

**Author:** lilyatplyable
**URL:** https://github.com/llvm/llvm-project/issues/165399
**Status:** Open
**Labels:** clang-tidy, incomplete, crash

## Body

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.	Program arguments: /usr/bin/clang-tidy -p=/home/lily/mbox/q/cmake-build-debug /home/lily/mbox/q/src/mould/cfrp/filetted.cppm
1.	<eof> parser at end of file
2.	ASTMatcher: Matching 'modernize-redundant-void-arg' against:
	FunctionDecl std::__terminate : </usr/bin/../lib/gcc/x86_64-redhat-linux/15/../../../../include/c++/15/x86_64-redhat-linux/bits/c++config.h:2262:16, line:2267:3>
 #0 0x00007f1e1082917a llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/lib64/libLLVM.so.20.1+0x22917a)
 #1 0x00007f1e10826104 llvm::sys::RunSignalHandlers() (/lib64/libLLVM.so.20.1+0x226104)
 #2 0x00007f1e108298eb (/lib64/libLLVM.so.20.1+0x2298eb)
 #3 0x00007f1e10028070 __restore_rt (/lib64/libc.so.6+0x1a070)
 #4 0x00007f1e193c4650 (/lib64/libclang-cpp.so.20.1+0x5c4650)
 #5 0x00007f1e193bc933 (/lib64/libclang-cpp.so.20.1+0x5bc933)
 #6 0x00007f1e193c2b2b (/lib64/libclang-cpp.so.20.1+0x5c2b2b)
 #7 0x00007f1e193bca0b (/lib64/libclang-cpp.so.20.1+0x5bca0b)
 #8 0x00007f1e193c4aa6 (/lib64/libclang-cpp.so.20.1+0x5c4aa6)
 #9 0x00007f1e193bc7fc (/lib64/libclang-cpp.so.20.1+0x5bc7fc)
#10 0x00007f1e193bf69b (/lib64/libclang-cpp.so.20.1+0x5bf69b)
#11 0x00007f1e193bcd9b (/lib64/libclang-cpp.so.20.1+0x5bcd9b)
#12 0x00007f1e193bd64b (/lib64/libclang-cpp.so.20.1+0x5bd64b)
#13 0x00007f1e193bc727 (/lib64/libclang-cpp.so.20.1+0x5bc727)
#14 0x00007f1e193ba5c1 clang::ParentMapContext::ParentMap::ParentMap(clang::ASTContext&) (/lib64/libclang-cpp.so.20.1+0x5ba5c1)
#15 0x00007f1e193ba6da clang::ParentMapContext::getParents(clang::DynTypedNode const&) (/lib64/libclang-cpp.so.20.1+0x5ba6da)
#16 0x00007f1e19b20775 (/lib64/libclang-cpp.so.20.1+0xd20775)
#17 0x0000555e447fc35f (/usr/bin/clang-tidy+0x9035f)
#18 0x00007f1e19b9612d clang::ast_matchers::internal::DynTypedMatcher::matches(clang::DynTypedNode const&, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const (/lib64/libclang-cpp.so.20.1+0xd9612d)
#19 0x00007f1e19b9a739 (/lib64/libclang-cpp.so.20.1+0xd9a739)
#20 0x00007f1e19b9612d clang::ast_matchers::internal::DynTypedMatcher::matches(clang::DynTypedNode const&, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const (/lib64/libclang-cpp.so.20.1+0xd9612d)
#21 0x00007f1e19b9af6e (/lib64/libclang-cpp.so.20.1+0xd9af6e)
#22 0x00007f1e19b963e2 clang::ast_matchers::internal::DynTypedMatcher::matchesNoKindCheck(clang::DynTypedNode const&, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const (/lib64/libclang-cpp.so.20.1+0xd963e2)
#23 0x00007f1e19b9a566 (/lib64/libclang-cpp.so.20.1+0xd9a566)
#24 0x00007f1e19b98ee2 (/lib64/libclang-cpp.so.20.1+0xd98ee2)
#25 0x00007f1e19b9612d clang::ast_matchers::internal::DynTypedMatcher::matches(clang::DynTypedNode const&, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const (/lib64/libclang-cpp.so.20.1+0xd9612d)
#26 0x00007f1e19b565f5 (/lib64/libclang-cpp.so.20.1+0xd565f5)
#27 0x00007f1e19b58413 (/lib64/libclang-cpp.so.20.1+0xd58413)
#28 0x00007f1e19b5905b (/lib64/libclang-cpp.so.20.1+0xd5905b)
#29 0x00007f1e19b5863f (/lib64/libclang-cpp.so.20.1+0xd5863f)
#30 0x00007f1e19b5ab8b (/lib64/libclang-cpp.so.20.1+0xd5ab8b)
#31 0x00007f1e19b5884f (/lib64/libclang-cpp.so.20.1+0xd5884f)
#32 0x00007f1e19b58bab (/lib64/libclang-cpp.so.20.1+0xd58bab)
#33 0x00007f1e19b5843f (/lib64/libclang-cpp.so.20.1+0xd5843f)
#34 0x00007f1e19b1f879 clang::ast_matchers::MatchFinder::matchAST(clang::ASTContext&) (/lib64/libclang-cpp.so.20.1+0xd1f879)
#35 0x00007f1e1b75e2ec clang::MultiplexConsumer::HandleTranslationUnit(clang::ASTContext&) (/lib64/libclang-cpp.so.20.1+0x295e2ec)
#36 0x00007f1e19012d49 clang::ParseAST(clang::Sema&, bool, bool) (/lib64/libclang-cpp.so.20.1+0x212d49)
#37 0x00007f1e1b7191af clang::FrontendAction::Execute() (/lib64/libclang-cpp.so.20.1+0x29191af)
#38 0x00007f1e1b67ba24 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/lib64/libclang-cpp.so.20.1+0x287ba24)
#39 0x00007f1e1b981285 clang::tooling::FrontendActionFactory::runInvocation(std::shared_ptr<clang::CompilerInvocation>, clang::FileManager*, std::shared_ptr<clang::PCHContainerOperations>, clang::DiagnosticConsumer*) (/lib64/libclang-cpp.so.20.1+0x2b81285)
#40 0x0000555e44e9a1dc (/usr/bin/clang-tidy+0x72e1dc)
#41 0x00007f1e1b980efa clang::tooling::ToolInvocation::runInvocation(char const*, clang::driver::Compilation*, std::shared_ptr<clang::CompilerInvocation>, std::shared_ptr<clang::PCHContainerOperations>) (/lib64/libclang-cpp.so.20.1+0x2b80efa)
#42 0x00007f1e1b97f7a5 clang::tooling::ToolInvocation::run() (/lib64/libclang-cpp.so.20.1+0x2b7f7a5)
#43 0x00007f1e1b9833f1 clang::tooling::ClangTool::run(clang::tooling::ToolAction*) (/lib64/libclang-cpp.so.20.1+0x2b833f1)
#44 0x0000555e44e9356a clang::tidy::runClangTidy(clang::tidy::ClangTidyContext&, clang::tooling::CompilationDatabase const&, llvm::ArrayRef<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>>, llvm::IntrusiveRefCntPtr<llvm::vfs::OverlayFileSystem>, bool, bool, llvm::StringRef) (/usr/bin/clang-tidy+0x72756a)
#45 0x0000555e4477a530 clang::tidy::clangTidyMain(int, char const**) (/usr/bin/clang-tidy+0xe530)
#46 0x00007f1e10011575 __libc_start_call_main (/lib64/libc.so.6+0x3575)
#47 0x00007f1e10011628 __libc_start_main@GLIBC_2.2.5 (/lib64/libc.so.6+0x3628)
#48 0x0000555e44777475 _start (/usr/bin/clang-tidy+0xb475)
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-tidy

Author: Lily Penrose (lilyatplyable)

<details>
```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.	Program arguments: /usr/bin/clang-tidy -p=/home/lily/mbox/q/cmake-build-debug /home/lily/mbox/q/src/mould/cfrp/filetted.cppm
1.	&lt;eof&gt; parser at end of file
2.	ASTMatcher: Matching 'modernize-redundant-void-arg' against:
	FunctionDecl std::__terminate : &lt;/usr/bin/../lib/gcc/x86_64-redhat-linux/15/../../../../include/c++/15/x86_64-redhat-linux/bits/c++config.h:2262:16, line:2267:3&gt;
 #<!-- -->0 0x00007f1e1082917a llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/lib64/libLLVM.so.20.1+0x22917a)
 #<!-- -->1 0x00007f1e10826104 llvm::sys::RunSignalHandlers() (/lib64/libLLVM.so.20.1+0x226104)
 #<!-- -->2 0x00007f1e108298eb (/lib64/libLLVM.so.20.1+0x2298eb)
 #<!-- -->3 0x00007f1e10028070 __restore_rt (/lib64/libc.so.6+0x1a070)
 #<!-- -->4 0x00007f1e193c4650 (/lib64/libclang-cpp.so.20.1+0x5c4650)
 #<!-- -->5 0x00007f1e193bc933 (/lib64/libclang-cpp.so.20.1+0x5bc933)
 #<!-- -->6 0x00007f1e193c2b2b (/lib64/libclang-cpp.so.20.1+0x5c2b2b)
 #<!-- -->7 0x00007f1e193bca0b (/lib64/libclang-cpp.so.20.1+0x5bca0b)
 #<!-- -->8 0x00007f1e193c4aa6 (/lib64/libclang-cpp.so.20.1+0x5c4aa6)
 #<!-- -->9 0x00007f1e193bc7fc (/lib64/libclang-cpp.so.20.1+0x5bc7fc)
#<!-- -->10 0x00007f1e193bf69b (/lib64/libclang-cpp.so.20.1+0x5bf69b)
#<!-- -->11 0x00007f1e193bcd9b (/lib64/libclang-cpp.so.20.1+0x5bcd9b)
#<!-- -->12 0x00007f1e193bd64b (/lib64/libclang-cpp.so.20.1+0x5bd64b)
#<!-- -->13 0x00007f1e193bc727 (/lib64/libclang-cpp.so.20.1+0x5bc727)
#<!-- -->14 0x00007f1e193ba5c1 clang::ParentMapContext::ParentMap::ParentMap(clang::ASTContext&amp;) (/lib64/libclang-cpp.so.20.1+0x5ba5c1)
#<!-- -->15 0x00007f1e193ba6da clang::ParentMapContext::getParents(clang::DynTypedNode const&amp;) (/lib64/libclang-cpp.so.20.1+0x5ba6da)
#<!-- -->16 0x00007f1e19b20775 (/lib64/libclang-cpp.so.20.1+0xd20775)
#<!-- -->17 0x0000555e447fc35f (/usr/bin/clang-tidy+0x9035f)
#<!-- -->18 0x00007f1e19b9612d clang::ast_matchers::internal::DynTypedMatcher::matches(clang::DynTypedNode const&amp;, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const (/lib64/libclang-cpp.so.20.1+0xd9612d)
#<!-- -->19 0x00007f1e19b9a739 (/lib64/libclang-cpp.so.20.1+0xd9a739)
#<!-- -->20 0x00007f1e19b9612d clang::ast_matchers::internal::DynTypedMatcher::matches(clang::DynTypedNode const&amp;, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const (/lib64/libclang-cpp.so.20.1+0xd9612d)
#<!-- -->21 0x00007f1e19b9af6e (/lib64/libclang-cpp.so.20.1+0xd9af6e)
#<!-- -->22 0x00007f1e19b963e2 clang::ast_matchers::internal::DynTypedMatcher::matchesNoKindCheck(clang::DynTypedNode const&amp;, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const (/lib64/libclang-cpp.so.20.1+0xd963e2)
#<!-- -->23 0x00007f1e19b9a566 (/lib64/libclang-cpp.so.20.1+0xd9a566)
#<!-- -->24 0x00007f1e19b98ee2 (/lib64/libclang-cpp.so.20.1+0xd98ee2)
#<!-- -->25 0x00007f1e19b9612d clang::ast_matchers::internal::DynTypedMatcher::matches(clang::DynTypedNode const&amp;, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const (/lib64/libclang-cpp.so.20.1+0xd9612d)
#<!-- -->26 0x00007f1e19b565f5 (/lib64/libclang-cpp.so.20.1+0xd565f5)
#<!-- -->27 0x00007f1e19b58413 (/lib64/libclang-cpp.so.20.1+0xd58413)
#<!-- -->28 0x00007f1e19b5905b (/lib64/libclang-cpp.so.20.1+0xd5905b)
#<!-- -->29 0x00007f1e19b5863f (/lib64/libclang-cpp.so.20.1+0xd5863f)
#<!-- -->30 0x00007f1e19b5ab8b (/lib64/libclang-cpp.so.20.1+0xd5ab8b)
#<!-- -->31 0x00007f1e19b5884f (/lib64/libclang-cpp.so.20.1+0xd5884f)
#<!-- -->32 0x00007f1e19b58bab (/lib64/libclang-cpp.so.20.1+0xd58bab)
#<!-- -->33 0x00007f1e19b5843f (/lib64/libclang-cpp.so.20.1+0xd5843f)
#<!-- -->34 0x00007f1e19b1f879 clang::ast_matchers::MatchFinder::matchAST(clang::ASTContext&amp;) (/lib64/libclang-cpp.so.20.1+0xd1f879)
#<!-- -->35 0x00007f1e1b75e2ec clang::MultiplexConsumer::HandleTranslationUnit(clang::ASTContext&amp;) (/lib64/libclang-cpp.so.20.1+0x295e2ec)
#<!-- -->36 0x00007f1e19012d49 clang::ParseAST(clang::Sema&amp;, bool, bool) (/lib64/libclang-cpp.so.20.1+0x212d49)
#<!-- -->37 0x00007f1e1b7191af clang::FrontendAction::Execute() (/lib64/libclang-cpp.so.20.1+0x29191af)
#<!-- -->38 0x00007f1e1b67ba24 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/lib64/libclang-cpp.so.20.1+0x287ba24)
#<!-- -->39 0x00007f1e1b981285 clang::tooling::FrontendActionFactory::runInvocation(std::shared_ptr&lt;clang::CompilerInvocation&gt;, clang::FileManager*, std::shared_ptr&lt;clang::PCHContainerOperations&gt;, clang::DiagnosticConsumer*) (/lib64/libclang-cpp.so.20.1+0x2b81285)
#<!-- -->40 0x0000555e44e9a1dc (/usr/bin/clang-tidy+0x72e1dc)
#<!-- -->41 0x00007f1e1b980efa clang::tooling::ToolInvocation::runInvocation(char const*, clang::driver::Compilation*, std::shared_ptr&lt;clang::CompilerInvocation&gt;, std::shared_ptr&lt;clang::PCHContainerOperations&gt;) (/lib64/libclang-cpp.so.20.1+0x2b80efa)
#<!-- -->42 0x00007f1e1b97f7a5 clang::tooling::ToolInvocation::run() (/lib64/libclang-cpp.so.20.1+0x2b7f7a5)
#<!-- -->43 0x00007f1e1b9833f1 clang::tooling::ClangTool::run(clang::tooling::ToolAction*) (/lib64/libclang-cpp.so.20.1+0x2b833f1)
#<!-- -->44 0x0000555e44e9356a clang::tidy::runClangTidy(clang::tidy::ClangTidyContext&amp;, clang::tooling::CompilationDatabase const&amp;, llvm::ArrayRef&lt;std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;&gt;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::OverlayFileSystem&gt;, bool, bool, llvm::StringRef) (/usr/bin/clang-tidy+0x72756a)
#<!-- -->45 0x0000555e4477a530 clang::tidy::clangTidyMain(int, char const**) (/usr/bin/clang-tidy+0xe530)
#<!-- -->46 0x00007f1e10011575 __libc_start_call_main (/lib64/libc.so.6+0x3575)
#<!-- -->47 0x00007f1e10011628 __libc_start_main@<!-- -->GLIBC_2.2.5 (/lib64/libc.so.6+0x3628)
#<!-- -->48 0x0000555e44777475 _start (/usr/bin/clang-tidy+0xb475)
```
</details>


---

### Comment 2 - zwuis

Could you please try 21 Release or `main` branch?

Please provide reproducer.

---

