# [clang] Assertion `D->getTemplateSpecializationKind() != TSK_ImplicitInstantiation' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/169302

## Body

Note: using `-std=c++11 --analyze`

Reproducer:
https://godbolt.org/z/vnMz6oc5T
```cpp
template <typename> struct S;

class Sp {
public:
  template <bool> void M() {}
  template <unsigned> struct I {
    static void IM();
  };
};

template <> struct S<Sp> {
  using F = void (Sp::*)();
  template <bool P> static constexpr F SpM = &Sp::template M<P>;
};

template <bool> constexpr S<Sp>::F S<Sp>::SpM;

template <unsigned X> void Sp::I<X>::IM() {
  using Spec = S<Sp>;
  typename Spec::F E = Spec::template SpM<true>;
}
```

Backtrace:
```console
clang++: warning: argument unused during compilation: '-S' [-Wunused-command-line-argument]
clang++: /root/llvm-project/llvm/tools/clang/include/clang/AST/RecursiveASTVisitor.h:2219: bool clang::RecursiveASTVisitor<Derived>::TraverseVarTemplateSpecializationDecl(clang::VarTemplateSpecializationDecl*) [with Derived = {anonymous}::Impl<false>]: Assertion `D->getTemplateSpecializationKind() != TSK_ImplicitInstantiation' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++11 --analyze <source>
1.	<eof> parser at end of file
 #0 0x00000000042531d8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42531d8)
 #1 0x0000000004250604 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4250604)
 #2 0x0000000004194338 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007e3fbb842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007e3fbb8969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007e3fbb842476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007e3fbb8287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007e3fbb82871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007e3fbb839e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000007b0fccf (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7b0fccf)
#10 0x0000000007b6825c clang::DynamicRecursiveASTVisitorBase<false>::TraverseTemplateInstantiations(clang::VarTemplateDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7b6825c)
#11 0x0000000007b12549 clang::DynamicRecursiveASTVisitorBase<false>::TraverseVarTemplateDecl(clang::VarTemplateDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7b12549)
#12 0x0000000007ace7ec clang::RecursiveASTVisitor<(anonymous namespace)::Impl<false>>::TraverseDeclContextHelper(clang::DeclContext*) (.part.0) DynamicRecursiveASTVisitor.cpp:0:0
#13 0x0000000007b67230 clang::DynamicRecursiveASTVisitorBase<false>::TraverseClassTemplateSpecializationDecl(clang::ClassTemplateSpecializationDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7b67230)
#14 0x0000000006367c99 (anonymous namespace)::AnalysisConsumer::HandleTranslationUnit(clang::ASTContext&) AnalysisConsumer.cpp:0:0
#15 0x0000000006933b1c clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6933b1c)
#16 0x0000000004e8d1e5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e8d1e5)
#17 0x0000000004e0d41e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e0d41e)
#18 0x0000000004f8613d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f8613d)
#19 0x0000000000dc6ea0 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdc6ea0)
#20 0x0000000000dbd91a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#21 0x0000000000dbda9d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#22 0x0000000004c09c79 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#23 0x00000000041947d4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41947d4)
#24 0x0000000004c0a28f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#25 0x0000000004bcac92 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bcac92)
#26 0x0000000004bcbc3e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bcbc3e)
#27 0x0000000004bd3205 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bd3205)
#28 0x0000000000dc32c1 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdc32c1)
#29 0x0000000000c71c44 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc71c44)
#30 0x00007e3fbb829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#31 0x00007e3fbb829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#32 0x0000000000dbd3b5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdbd3b5)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (k-arrows)

<details>
Note: using `-std=c++11 --analyze`

Reproducer:
https://godbolt.org/z/vnMz6oc5T
```cpp
template &lt;typename&gt; struct S;

class Sp {
public:
  template &lt;bool&gt; void M() {}
  template &lt;unsigned&gt; struct I {
    static void IM();
  };
};

template &lt;&gt; struct S&lt;Sp&gt; {
  using F = void (Sp::*)();
  template &lt;bool P&gt; static constexpr F SpM = &amp;Sp::template M&lt;P&gt;;
};

template &lt;bool&gt; constexpr S&lt;Sp&gt;::F S&lt;Sp&gt;::SpM;

template &lt;unsigned X&gt; void Sp::I&lt;X&gt;::IM() {
  using Spec = S&lt;Sp&gt;;
  typename Spec::F E = Spec::template SpM&lt;true&gt;;
}
```

Backtrace:
```console
clang++: warning: argument unused during compilation: '-S' [-Wunused-command-line-argument]
clang++: /root/llvm-project/llvm/tools/clang/include/clang/AST/RecursiveASTVisitor.h:2219: bool clang::RecursiveASTVisitor&lt;Derived&gt;::TraverseVarTemplateSpecializationDecl(clang::VarTemplateSpecializationDecl*) [with Derived = {anonymous}::Impl&lt;false&gt;]: Assertion `D-&gt;getTemplateSpecializationKind() != TSK_ImplicitInstantiation' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++11 --analyze &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
 #<!-- -->0 0x00000000042531d8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42531d8)
 #<!-- -->1 0x0000000004250604 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4250604)
 #<!-- -->2 0x0000000004194338 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007e3fbb842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007e3fbb8969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007e3fbb842476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007e3fbb8287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x00007e3fbb82871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x00007e3fbb839e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000007b0fccf (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7b0fccf)
#<!-- -->10 0x0000000007b6825c clang::DynamicRecursiveASTVisitorBase&lt;false&gt;::TraverseTemplateInstantiations(clang::VarTemplateDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7b6825c)
#<!-- -->11 0x0000000007b12549 clang::DynamicRecursiveASTVisitorBase&lt;false&gt;::TraverseVarTemplateDecl(clang::VarTemplateDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7b12549)
#<!-- -->12 0x0000000007ace7ec clang::RecursiveASTVisitor&lt;(anonymous namespace)::Impl&lt;false&gt;&gt;::TraverseDeclContextHelper(clang::DeclContext*) (.part.0) DynamicRecursiveASTVisitor.cpp:0:0
#<!-- -->13 0x0000000007b67230 clang::DynamicRecursiveASTVisitorBase&lt;false&gt;::TraverseClassTemplateSpecializationDecl(clang::ClassTemplateSpecializationDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7b67230)
#<!-- -->14 0x0000000006367c99 (anonymous namespace)::AnalysisConsumer::HandleTranslationUnit(clang::ASTContext&amp;) AnalysisConsumer.cpp:0:0
#<!-- -->15 0x0000000006933b1c clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6933b1c)
#<!-- -->16 0x0000000004e8d1e5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e8d1e5)
#<!-- -->17 0x0000000004e0d41e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e0d41e)
#<!-- -->18 0x0000000004f8613d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f8613d)
#<!-- -->19 0x0000000000dc6ea0 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdc6ea0)
#<!-- -->20 0x0000000000dbd91a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->21 0x0000000000dbda9d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->22 0x0000000004c09c79 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->23 0x00000000041947d4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41947d4)
#<!-- -->24 0x0000000004c0a28f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->25 0x0000000004bcac92 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bcac92)
#<!-- -->26 0x0000000004bcbc3e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bcbc3e)
#<!-- -->27 0x0000000004bd3205 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bd3205)
#<!-- -->28 0x0000000000dc32c1 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdc32c1)
#<!-- -->29 0x0000000000c71c44 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc71c44)
#<!-- -->30 0x00007e3fbb829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->31 0x00007e3fbb829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->32 0x0000000000dbd3b5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdbd3b5)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-static-analyzer

Author: None (k-arrows)

<details>
Note: using `-std=c++11 --analyze`

Reproducer:
https://godbolt.org/z/vnMz6oc5T
```cpp
template &lt;typename&gt; struct S;

class Sp {
public:
  template &lt;bool&gt; void M() {}
  template &lt;unsigned&gt; struct I {
    static void IM();
  };
};

template &lt;&gt; struct S&lt;Sp&gt; {
  using F = void (Sp::*)();
  template &lt;bool P&gt; static constexpr F SpM = &amp;Sp::template M&lt;P&gt;;
};

template &lt;bool&gt; constexpr S&lt;Sp&gt;::F S&lt;Sp&gt;::SpM;

template &lt;unsigned X&gt; void Sp::I&lt;X&gt;::IM() {
  using Spec = S&lt;Sp&gt;;
  typename Spec::F E = Spec::template SpM&lt;true&gt;;
}
```

Backtrace:
```console
clang++: warning: argument unused during compilation: '-S' [-Wunused-command-line-argument]
clang++: /root/llvm-project/llvm/tools/clang/include/clang/AST/RecursiveASTVisitor.h:2219: bool clang::RecursiveASTVisitor&lt;Derived&gt;::TraverseVarTemplateSpecializationDecl(clang::VarTemplateSpecializationDecl*) [with Derived = {anonymous}::Impl&lt;false&gt;]: Assertion `D-&gt;getTemplateSpecializationKind() != TSK_ImplicitInstantiation' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++11 --analyze &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
 #<!-- -->0 0x00000000042531d8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42531d8)
 #<!-- -->1 0x0000000004250604 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4250604)
 #<!-- -->2 0x0000000004194338 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007e3fbb842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007e3fbb8969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007e3fbb842476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007e3fbb8287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x00007e3fbb82871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x00007e3fbb839e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000007b0fccf (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7b0fccf)
#<!-- -->10 0x0000000007b6825c clang::DynamicRecursiveASTVisitorBase&lt;false&gt;::TraverseTemplateInstantiations(clang::VarTemplateDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7b6825c)
#<!-- -->11 0x0000000007b12549 clang::DynamicRecursiveASTVisitorBase&lt;false&gt;::TraverseVarTemplateDecl(clang::VarTemplateDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7b12549)
#<!-- -->12 0x0000000007ace7ec clang::RecursiveASTVisitor&lt;(anonymous namespace)::Impl&lt;false&gt;&gt;::TraverseDeclContextHelper(clang::DeclContext*) (.part.0) DynamicRecursiveASTVisitor.cpp:0:0
#<!-- -->13 0x0000000007b67230 clang::DynamicRecursiveASTVisitorBase&lt;false&gt;::TraverseClassTemplateSpecializationDecl(clang::ClassTemplateSpecializationDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7b67230)
#<!-- -->14 0x0000000006367c99 (anonymous namespace)::AnalysisConsumer::HandleTranslationUnit(clang::ASTContext&amp;) AnalysisConsumer.cpp:0:0
#<!-- -->15 0x0000000006933b1c clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6933b1c)
#<!-- -->16 0x0000000004e8d1e5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e8d1e5)
#<!-- -->17 0x0000000004e0d41e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e0d41e)
#<!-- -->18 0x0000000004f8613d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f8613d)
#<!-- -->19 0x0000000000dc6ea0 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdc6ea0)
#<!-- -->20 0x0000000000dbd91a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->21 0x0000000000dbda9d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->22 0x0000000004c09c79 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->23 0x00000000041947d4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41947d4)
#<!-- -->24 0x0000000004c0a28f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->25 0x0000000004bcac92 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bcac92)
#<!-- -->26 0x0000000004bcbc3e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bcbc3e)
#<!-- -->27 0x0000000004bd3205 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bd3205)
#<!-- -->28 0x0000000000dc32c1 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdc32c1)
#<!-- -->29 0x0000000000c71c44 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc71c44)
#<!-- -->30 0x00007e3fbb829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->31 0x00007e3fbb829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->32 0x0000000000dbd3b5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdbd3b5)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

### Comment 3 - k-arrows

This is trunk only:
https://godbolt.org/z/7j4oazY78

---

### Comment 4 - steakhal

Thanks for the report. I think this is related to #110899 I'd like to invite @hvdijk to have a look at this.
I don't think this assertion has anything to do with the Clang Static Analyzer.

---

### Comment 5 - hvdijk

> Thanks for the report. I think this is related to [#110899](https://github.com/llvm/llvm-project/pull/110899) I'd like to invite [@hvdijk](https://github.com/hvdijk) to have a look at this.

Ah, that's worrying. The assertion I added in #110899 was only making explicit an assumption that had been made implicitly in #81642, which wasn't held at the time, which I thought had since been fixed, but apparently hasn't. The comment before the assert says that the `if` is meant to be skipped for implicit instantiations, so the assert explicitly checks inside the assert that the we are not dealing with an implicit instantiation. The comment says we can use `getTemplateArgsAsWritten()` to distinguish, but that hasn't proved reliable. @sdkrystian Can we make `getTemplateArgsAsWritten()` a reliable way to distinguish or do we need to stop using that to distinguish?

> I don't think this assertion has anything to do with the Clang Static Analyzer.

Agreed, although it's possible it only hits when the analyzer is used, the actual problem is in Clang itself.

---

### Comment 6 - steakhal

@hvdijk Thanks for having a look. This confirms my theory, that its not a bug in the CSA.

---

### Comment 7 - shafik

@hvdijk is this something you can fix? I am not sure how active @sdkrystian is. It would be good to fix soon since this is indeed a regression.

---

### Comment 8 - hvdijk

> [@hvdijk](https://github.com/hvdijk) is this something you can fix?

There are three possible things we can do to avoid the error:
1. Revert #81642. This is almost certainly a solution that would be rejected in review, since we're a year and a half after that PR.
2. Revert #110899. This "fixes" the assertion failure by removing the assertion, but keeping the code that relies on what the assertion was checking and silently doing the wrong thing and hoping for the best.
3. Fix #81642. This is IMO the best solution, but needs someone familiar with this code to clarify things.

I would be able to do option 1, but I suspect people will prefer that I don't.
I would be able to do option 2, but I personally consider that worse than leaving it crashing.
I am not at the moment able to do option 3 without significant effort, or help from others.

---

### Comment 9 - shafik

CC @erichkeane @sdkrystian 

---

