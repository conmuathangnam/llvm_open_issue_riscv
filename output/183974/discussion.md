# [clang] Crash in C mode with -fblocks after invalid global __block declaration

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/183974

## Body

Reproducer:
https://godbolt.org/z/xnP4ox8rE
```c
__block int x;

int x[0];
```

Backtrace:
```console
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x c -fblocks <source>
1.	<eof> parser at end of file
 #0 0x000000000440ac38 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x440ac38)
 #1 0x0000000004408084 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4408084)
 #2 0x0000000004347e88 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007e318c642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x0000000000ca287a clang::Sema::CheckCompleteVariableDeclaration(clang::VarDecl*) (.cold) SemaDecl.cpp:0:0
 #5 0x0000000006d85f19 clang::Sema::ActOnEndOfTranslationUnit() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d85f19)
 #6 0x0000000006beda9a clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6beda9a)
 #7 0x0000000006bcefda clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bcefda)
 #8 0x0000000004ee4ef8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ee4ef8)
 #9 0x00000000051eb615 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x51eb615)
#10 0x00000000051698fe clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x51698fe)
#11 0x00000000052e72dd clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x52e72dd)
#12 0x0000000000df063e cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdf063e)
#13 0x0000000000de6faa ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#14 0x0000000000de712d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#15 0x0000000004f59659 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#16 0x00000000043482c4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x43482c4)
#17 0x0000000004f59c6f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#18 0x0000000004f19952 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f19952)
#19 0x0000000004f1a8fe clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f1a8fe)
#20 0x0000000004f22645 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f22645)
#21 0x0000000000dec9ae clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdec9ae)
#22 0x0000000000ca4d0a main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xca4d0a)
#23 0x00007e318c629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#24 0x00007e318c629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#25 0x0000000000de6a45 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde6a45)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
Compiler returned: 139
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/xnP4ox8rE
```c
__block int x;

int x[0];
```

Backtrace:
```console
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x c -fblocks &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
 #<!-- -->0 0x000000000440ac38 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x440ac38)
 #<!-- -->1 0x0000000004408084 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4408084)
 #<!-- -->2 0x0000000004347e88 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007e318c642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x0000000000ca287a clang::Sema::CheckCompleteVariableDeclaration(clang::VarDecl*) (.cold) SemaDecl.cpp:0:0
 #<!-- -->5 0x0000000006d85f19 clang::Sema::ActOnEndOfTranslationUnit() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d85f19)
 #<!-- -->6 0x0000000006beda9a clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6beda9a)
 #<!-- -->7 0x0000000006bcefda clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bcefda)
 #<!-- -->8 0x0000000004ee4ef8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ee4ef8)
 #<!-- -->9 0x00000000051eb615 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x51eb615)
#<!-- -->10 0x00000000051698fe clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x51698fe)
#<!-- -->11 0x00000000052e72dd clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x52e72dd)
#<!-- -->12 0x0000000000df063e cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdf063e)
#<!-- -->13 0x0000000000de6faa ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->14 0x0000000000de712d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->15 0x0000000004f59659 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->16 0x00000000043482c4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x43482c4)
#<!-- -->17 0x0000000004f59c6f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->18 0x0000000004f19952 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f19952)
#<!-- -->19 0x0000000004f1a8fe clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f1a8fe)
#<!-- -->20 0x0000000004f22645 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f22645)
#<!-- -->21 0x0000000000dec9ae clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdec9ae)
#<!-- -->22 0x0000000000ca4d0a main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xca4d0a)
#<!-- -->23 0x00007e318c629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->24 0x00007e318c629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->25 0x0000000000de6a45 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde6a45)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
Compiler returned: 139
```
</details>


---

### Comment 2 - k-arrows

Goes back to clang-13:
https://godbolt.org/z/5h8he7c8r

---

### Comment 3 - TPPPP72

I believe a feasible solution is to add additional checks to these two code blocks.

https://github.com/llvm/llvm-project/blob/b872179bebe7a9aa42163fdb3334ab9e388c9dfd/clang/lib/Sema/SemaObjC.cpp#L1707-L1714

---

