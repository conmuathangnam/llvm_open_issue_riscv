# [clang] -fsanitize=undefined causes Assertion `vlaSize && "no size for VLA!"' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/163032
**Status:** Open
**Labels:** clang:codegen, crash

## Body

Reproducer:
https://godbolt.org/z/x8s5GhbE8
```cpp
void foo(unsigned short i) {
  short a[i];
  auto bar = [&iv = a](short value) { iv[0] = 0; };
  bar(0);
}
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/CodeGen/CodeGenFunction.cpp:2420: clang::CodeGen::CodeGenFunction::VlaSizePair clang::CodeGen::CodeGenFunction::getVLASize(const clang::VariableArrayType*): Assertion `vlaSize && "no size for VLA!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -fsanitize=undefined <source>
1.	<eof> parser at end of file
2.	Per-file LLVM IR generation
3.	<source>:3:14: Generating code for declaration 'foo(unsigned short)::(anonymous class)::operator()'
 #0 0x00000000041e9638 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41e9638)
 #1 0x00000000041e6a64 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41e6a64)
 #2 0x000000000412b3b8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000076bcc1642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x000076bcc16969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x000076bcc1642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x000076bcc16287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x000076bcc162871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x000076bcc1639e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000004752e21 clang::CodeGen::CodeGenFunction::getVLASize(clang::VariableArrayType const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4752e21)
#10 0x000000000453cb7c getArrayIndexingBound(clang::CodeGen::CodeGenFunction&, clang::Expr const*, clang::QualType&, clang::LangOptionsBase::StrictFlexArraysLevelKind) (.constprop.0) CGExpr.cpp:0:0
#11 0x000000000453d2ed clang::CodeGen::CodeGenFunction::EmitArraySubscriptExpr(clang::ArraySubscriptExpr const*, bool)::'lambda'(bool)::operator()(bool) const CGExpr.cpp:0:0
#12 0x0000000004547478 clang::CodeGen::CodeGenFunction::EmitArraySubscriptExpr(clang::ArraySubscriptExpr const*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4547478)
#13 0x0000000004547ad3 clang::CodeGen::CodeGenFunction::EmitCheckedLValue(clang::Expr const*, clang::CodeGen::CodeGenFunction::TypeCheckKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4547ad3)
#14 0x0000000004559091 clang::CodeGen::CodeGenFunction::EmitBinaryOperatorLValue(clang::BinaryOperator const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4559091)
#15 0x000000000455f943 clang::CodeGen::CodeGenFunction::EmitLValueHelper(clang::Expr const*, clang::CodeGen::KnownNonNull_t) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x455f943)
#16 0x000000000455fd97 void llvm::function_ref<void ()>::callback_fn<clang::CodeGen::CodeGenFunction::EmitLValue(clang::Expr const*, clang::CodeGen::KnownNonNull_t)::'lambda'()>(long) CGExpr.cpp:0:0
#17 0x00000000081aeb91 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x81aeb91)
#18 0x0000000004527e03 clang::CodeGen::CodeGenFunction::EmitLValue(clang::Expr const*, clang::CodeGen::KnownNonNull_t) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4527e03)
#19 0x000000000455800b clang::CodeGen::CodeGenFunction::EmitIgnoredExpr(clang::Expr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x455800b)
#20 0x00000000046d7f87 clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef<clang::Attr const*>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x46d7f87)
#21 0x00000000046e021c clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&, bool, clang::CodeGen::AggValueSlot) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x46e021c)
#22 0x000000000474879e clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x474879e)
#23 0x000000000475a7c4 clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x475a7c4)
#24 0x00000000047c73d0 clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x47c73d0)
#25 0x00000000047c2284 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x47c2284)
#26 0x00000000047cf12c clang::CodeGen::CodeGenModule::EmitDeferred() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x47cf12c)
#27 0x00000000047d17d3 clang::CodeGen::CodeGenModule::Release() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x47d17d3)
#28 0x0000000004b31ebe (anonymous namespace)::CodeGeneratorImpl::HandleTranslationUnit(clang::ASTContext&) ModuleBuilder.cpp:0:0
#29 0x0000000004b2dec5 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b2dec5)
#30 0x00000000068c62bc clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68c62bc)
#31 0x0000000004b2e6a8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b2e6a8)
#32 0x0000000004e1a3a5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e1a3a5)
#33 0x0000000004d9bade clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d9bade)
#34 0x0000000004f116bd clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f116bd)
#35 0x0000000000dbfa50 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdbfa50)
#36 0x0000000000db65ba ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#37 0x0000000000db673d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#38 0x0000000004b97839 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#39 0x000000000412b854 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x412b854)
#40 0x0000000004b97e4f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#41 0x0000000004b59162 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b59162)
#42 0x0000000004b5a10e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b5a10e)
#43 0x0000000004b61845 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b61845)
#44 0x0000000000dbbf79 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdbbf79)
#45 0x0000000000c6bf74 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc6bf74)
#46 0x000076bcc1629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#47 0x000076bcc1629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#48 0x0000000000db6055 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb6055)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/x8s5GhbE8
```cpp
void foo(unsigned short i) {
  short a[i];
  auto bar = [&amp;iv = a](short value) { iv[0] = 0; };
  bar(0);
}
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/CodeGen/CodeGenFunction.cpp:2420: clang::CodeGen::CodeGenFunction::VlaSizePair clang::CodeGen::CodeGenFunction::getVLASize(const clang::VariableArrayType*): Assertion `vlaSize &amp;&amp; "no size for VLA!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -fsanitize=undefined &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	Per-file LLVM IR generation
3.	&lt;source&gt;:3:14: Generating code for declaration 'foo(unsigned short)::(anonymous class)::operator()'
 #<!-- -->0 0x00000000041e9638 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41e9638)
 #<!-- -->1 0x00000000041e6a64 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41e6a64)
 #<!-- -->2 0x000000000412b3b8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x000076bcc1642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x000076bcc16969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x000076bcc1642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x000076bcc16287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x000076bcc162871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x000076bcc1639e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000004752e21 clang::CodeGen::CodeGenFunction::getVLASize(clang::VariableArrayType const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4752e21)
#<!-- -->10 0x000000000453cb7c getArrayIndexingBound(clang::CodeGen::CodeGenFunction&amp;, clang::Expr const*, clang::QualType&amp;, clang::LangOptionsBase::StrictFlexArraysLevelKind) (.constprop.0) CGExpr.cpp:0:0
#<!-- -->11 0x000000000453d2ed clang::CodeGen::CodeGenFunction::EmitArraySubscriptExpr(clang::ArraySubscriptExpr const*, bool)::'lambda'(bool)::operator()(bool) const CGExpr.cpp:0:0
#<!-- -->12 0x0000000004547478 clang::CodeGen::CodeGenFunction::EmitArraySubscriptExpr(clang::ArraySubscriptExpr const*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4547478)
#<!-- -->13 0x0000000004547ad3 clang::CodeGen::CodeGenFunction::EmitCheckedLValue(clang::Expr const*, clang::CodeGen::CodeGenFunction::TypeCheckKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4547ad3)
#<!-- -->14 0x0000000004559091 clang::CodeGen::CodeGenFunction::EmitBinaryOperatorLValue(clang::BinaryOperator const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4559091)
#<!-- -->15 0x000000000455f943 clang::CodeGen::CodeGenFunction::EmitLValueHelper(clang::Expr const*, clang::CodeGen::KnownNonNull_t) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x455f943)
#<!-- -->16 0x000000000455fd97 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::CodeGen::CodeGenFunction::EmitLValue(clang::Expr const*, clang::CodeGen::KnownNonNull_t)::'lambda'()&gt;(long) CGExpr.cpp:0:0
#<!-- -->17 0x00000000081aeb91 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x81aeb91)
#<!-- -->18 0x0000000004527e03 clang::CodeGen::CodeGenFunction::EmitLValue(clang::Expr const*, clang::CodeGen::KnownNonNull_t) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4527e03)
#<!-- -->19 0x000000000455800b clang::CodeGen::CodeGenFunction::EmitIgnoredExpr(clang::Expr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x455800b)
#<!-- -->20 0x00000000046d7f87 clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef&lt;clang::Attr const*&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x46d7f87)
#<!-- -->21 0x00000000046e021c clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&amp;, bool, clang::CodeGen::AggValueSlot) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x46e021c)
#<!-- -->22 0x000000000474879e clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x474879e)
#<!-- -->23 0x000000000475a7c4 clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x475a7c4)
#<!-- -->24 0x00000000047c73d0 clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x47c73d0)
#<!-- -->25 0x00000000047c2284 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x47c2284)
#<!-- -->26 0x00000000047cf12c clang::CodeGen::CodeGenModule::EmitDeferred() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x47cf12c)
#<!-- -->27 0x00000000047d17d3 clang::CodeGen::CodeGenModule::Release() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x47d17d3)
#<!-- -->28 0x0000000004b31ebe (anonymous namespace)::CodeGeneratorImpl::HandleTranslationUnit(clang::ASTContext&amp;) ModuleBuilder.cpp:0:0
#<!-- -->29 0x0000000004b2dec5 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b2dec5)
#<!-- -->30 0x00000000068c62bc clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68c62bc)
#<!-- -->31 0x0000000004b2e6a8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b2e6a8)
#<!-- -->32 0x0000000004e1a3a5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e1a3a5)
#<!-- -->33 0x0000000004d9bade clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d9bade)
#<!-- -->34 0x0000000004f116bd clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f116bd)
#<!-- -->35 0x0000000000dbfa50 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdbfa50)
#<!-- -->36 0x0000000000db65ba ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->37 0x0000000000db673d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->38 0x0000000004b97839 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->39 0x000000000412b854 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x412b854)
#<!-- -->40 0x0000000004b97e4f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->41 0x0000000004b59162 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b59162)
#<!-- -->42 0x0000000004b5a10e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b5a10e)
#<!-- -->43 0x0000000004b61845 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b61845)
#<!-- -->44 0x0000000000dbbf79 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdbbf79)
#<!-- -->45 0x0000000000c6bf74 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc6bf74)
#<!-- -->46 0x000076bcc1629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->47 0x000076bcc1629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->48 0x0000000000db6055 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb6055)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

### Comment 2 - k-arrows

Goes back to clang-6.0.0:
https://godbolt.org/z/G9rK9GhY4

---

