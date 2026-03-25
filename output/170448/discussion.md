# [clang][DebugInfo] Assertion `SizeInBits >= OffsetInBits - FragmentOffset && "size underflow"' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/170448
**Status:** Open
**Labels:** debuginfo, confirmed, crash

## Body

Reproducer:
https://godbolt.org/z/5bjqq7ord
```c
void foo(void) {
  char arr[0x100000000];
  char *ptr = arr + ~0u/2;
  *ptr = 42;
}
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/lib/CodeGen/AsmPrinter/DwarfExpression.cpp:542: bool llvm::DwarfExpression::addExpression(llvm::DIExpressionCursor&&, llvm::function_ref<bool(unsigned int, llvm::DIExpressionCursor&)>): Assertion `SizeInBits >= OffsetInBits - FragmentOffset && "size underflow"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x c -std=c89 -O1 -g <source>
1.	<eof> parser at end of file
2.	Code generation
3.	Running pass 'Function Pass Manager' on module '<source>'.
4.	Running pass 'X86 Assembly Printer' on function '@foo'
 #0 0x0000000004284618 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4284618)
 #1 0x0000000004281a44 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4281a44)
 #2 0x00000000041c5448 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x0000707e82442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x0000707e824969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x0000707e82442476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x0000707e824287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x0000707e8242871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x0000707e82439e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x000000000521891a llvm::DwarfExpression::addExpression(llvm::DIExpressionCursor&&, llvm::function_ref<bool (unsigned int, llvm::DIExpressionCursor&)>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x521891a)
#10 0x00000000052189b5 llvm::DwarfExpression::addExpression(llvm::DIExpressionCursor&&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x52189b5)
#11 0x000000000527dc81 llvm::DwarfCompileUnit::applyConcreteDbgVariableAttributes(llvm::Loc::Single const&, llvm::DbgVariable const&, llvm::DIE&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x527dc81)
#12 0x000000000527d228 llvm::DwarfCompileUnit::constructVariableDIE(llvm::DbgVariable&, llvm::LexicalScope const&, llvm::DIE*&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x527d228)
#13 0x0000000005283972 llvm::DwarfCompileUnit::createAndAddScopeChildren(llvm::LexicalScope*, llvm::DIE&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5283972)
#14 0x0000000005287226 llvm::DwarfCompileUnit::constructSubprogramScopeDIE(llvm::DISubprogram const*, llvm::Function const&, llvm::LexicalScope*, llvm::MCSymbol*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5287226)
#15 0x0000000005211bc9 llvm::DwarfDebug::endFunctionImpl(llvm::MachineFunction const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5211bc9)
#16 0x00000000051d5427 llvm::DebugHandlerBase::endFunction(llvm::MachineFunction const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x51d5427)
#17 0x00000000051ce8aa llvm::AsmPrinter::emitFunctionBody() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x51ce8aa)
#18 0x0000000002a5a4f4 llvm::X86AsmPrinter::runOnMachineFunction(llvm::MachineFunction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x2a5a4f4)
#19 0x0000000003536099 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (.part.0) MachineFunctionPass.cpp:0:0
#20 0x0000000003b7d856 llvm::FPPassManager::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3b7d856)
#21 0x0000000003b7dc01 llvm::FPPassManager::runOnModule(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3b7dc01)
#22 0x0000000003b7e46f llvm::legacy::PassManagerImpl::run(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3b7e46f)
#23 0x0000000004543306 clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4543306)
#24 0x0000000004bd4207 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bd4207)
#25 0x000000000697194c clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x697194c)
#26 0x0000000004bd4668 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bd4668)
#27 0x0000000004ebfd05 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ebfd05)
#28 0x0000000004e4006e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e4006e)
#29 0x0000000004fb8d6d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4fb8d6d)
#30 0x0000000000dc8450 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdc8450)
#31 0x0000000000dbeeca ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#32 0x0000000000dbf04d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#33 0x0000000004c3ce59 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#34 0x00000000041c58e4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41c58e4)
#35 0x0000000004c3d46f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#36 0x0000000004bfdc62 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bfdc62)
#37 0x0000000004bfec0e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bfec0e)
#38 0x0000000004c06045 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c06045)
#39 0x0000000000dc4871 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdc4871)
#40 0x0000000000c73414 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc73414)
#41 0x0000707e82429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#42 0x0000707e82429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#43 0x0000000000dbe965 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdbe965)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-debuginfo

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/5bjqq7ord
```c
void foo(void) {
  char arr[0x100000000];
  char *ptr = arr + ~0u/2;
  *ptr = 42;
}
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/lib/CodeGen/AsmPrinter/DwarfExpression.cpp:542: bool llvm::DwarfExpression::addExpression(llvm::DIExpressionCursor&amp;&amp;, llvm::function_ref&lt;bool(unsigned int, llvm::DIExpressionCursor&amp;)&gt;): Assertion `SizeInBits &gt;= OffsetInBits - FragmentOffset &amp;&amp; "size underflow"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x c -std=c89 -O1 -g &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	Code generation
3.	Running pass 'Function Pass Manager' on module '&lt;source&gt;'.
4.	Running pass 'X86 Assembly Printer' on function '@<!-- -->foo'
 #<!-- -->0 0x0000000004284618 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4284618)
 #<!-- -->1 0x0000000004281a44 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4281a44)
 #<!-- -->2 0x00000000041c5448 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x0000707e82442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x0000707e824969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x0000707e82442476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x0000707e824287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x0000707e8242871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x0000707e82439e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x000000000521891a llvm::DwarfExpression::addExpression(llvm::DIExpressionCursor&amp;&amp;, llvm::function_ref&lt;bool (unsigned int, llvm::DIExpressionCursor&amp;)&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x521891a)
#<!-- -->10 0x00000000052189b5 llvm::DwarfExpression::addExpression(llvm::DIExpressionCursor&amp;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x52189b5)
#<!-- -->11 0x000000000527dc81 llvm::DwarfCompileUnit::applyConcreteDbgVariableAttributes(llvm::Loc::Single const&amp;, llvm::DbgVariable const&amp;, llvm::DIE&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x527dc81)
#<!-- -->12 0x000000000527d228 llvm::DwarfCompileUnit::constructVariableDIE(llvm::DbgVariable&amp;, llvm::LexicalScope const&amp;, llvm::DIE*&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x527d228)
#<!-- -->13 0x0000000005283972 llvm::DwarfCompileUnit::createAndAddScopeChildren(llvm::LexicalScope*, llvm::DIE&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5283972)
#<!-- -->14 0x0000000005287226 llvm::DwarfCompileUnit::constructSubprogramScopeDIE(llvm::DISubprogram const*, llvm::Function const&amp;, llvm::LexicalScope*, llvm::MCSymbol*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5287226)
#<!-- -->15 0x0000000005211bc9 llvm::DwarfDebug::endFunctionImpl(llvm::MachineFunction const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5211bc9)
#<!-- -->16 0x00000000051d5427 llvm::DebugHandlerBase::endFunction(llvm::MachineFunction const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x51d5427)
#<!-- -->17 0x00000000051ce8aa llvm::AsmPrinter::emitFunctionBody() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x51ce8aa)
#<!-- -->18 0x0000000002a5a4f4 llvm::X86AsmPrinter::runOnMachineFunction(llvm::MachineFunction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x2a5a4f4)
#<!-- -->19 0x0000000003536099 llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (.part.0) MachineFunctionPass.cpp:0:0
#<!-- -->20 0x0000000003b7d856 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3b7d856)
#<!-- -->21 0x0000000003b7dc01 llvm::FPPassManager::runOnModule(llvm::Module&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3b7dc01)
#<!-- -->22 0x0000000003b7e46f llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3b7e46f)
#<!-- -->23 0x0000000004543306 clang::emitBackendOutput(clang::CompilerInstance&amp;, clang::CodeGenOptions&amp;, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;, std::unique_ptr&lt;llvm::raw_pwrite_stream, std::default_delete&lt;llvm::raw_pwrite_stream&gt;&gt;, clang::BackendConsumer*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4543306)
#<!-- -->24 0x0000000004bd4207 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bd4207)
#<!-- -->25 0x000000000697194c clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x697194c)
#<!-- -->26 0x0000000004bd4668 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bd4668)
#<!-- -->27 0x0000000004ebfd05 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ebfd05)
#<!-- -->28 0x0000000004e4006e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e4006e)
#<!-- -->29 0x0000000004fb8d6d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4fb8d6d)
#<!-- -->30 0x0000000000dc8450 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdc8450)
#<!-- -->31 0x0000000000dbeeca ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->32 0x0000000000dbf04d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->33 0x0000000004c3ce59 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->34 0x00000000041c58e4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41c58e4)
#<!-- -->35 0x0000000004c3d46f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->36 0x0000000004bfdc62 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bfdc62)
#<!-- -->37 0x0000000004bfec0e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bfec0e)
#<!-- -->38 0x0000000004c06045 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c06045)
#<!-- -->39 0x0000000000dc4871 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdc4871)
#<!-- -->40 0x0000000000c73414 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc73414)
#<!-- -->41 0x0000707e82429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->42 0x0000707e82429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->43 0x0000000000dbe965 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdbe965)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

### Comment 2 - k-arrows

Goes back to clang-19.1.0:
https://godbolt.org/z/G9x6K94T9

---

### Comment 3 - OCHyams

It looks like the `DW_OP_LLVM_fragment` generated for the `DIExpression` for the split variable (after SROA) has a bit-offset of 17179869176, `!DIExpression(DW_OP_LLVM_fragment, 17179869176, 8)`.

The [code](https://github.com/llvm/llvm-project/blob/main/llvm/lib/CodeGen/AsmPrinter/DwarfExpression.cpp#L536) around the assert is truncating the fragment offset and size uints from 64 to 32 bits. I'm not 100% sure how much code needs updating looking at it.

Not sure why it started happening in Clang 19 specifically.

---

