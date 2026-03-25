# crashes when compiling __annotation(L"...") with -fms-extensions

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/184318
**Status:** Open
**Labels:** clang:codegen, crash

## Body

Reproducer:
https://godbolt.org/z/37Wc9MP8v
```cpp
void foo() {
  __annotation(L"a1");
}
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/CodeGen/CGBuiltin.cpp:5373: clang::CodeGen::RValue clang::CodeGen::CodeGenFunction::EmitBuiltinExpr(clang::GlobalDecl, unsigned int, const clang::CallExpr*, clang::CodeGen::ReturnValueSlot): Assertion `Str->getCharByteWidth() == 2' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -fms-extensions <source>
1.	<eof> parser at end of file
2.	<source>:1:6: LLVM IR generation of declaration 'foo'
3.	<source>:1:6: Generating code for declaration 'foo'
 #0 0x000000000440a568 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x440a568)
 #1 0x00000000044079b4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x44079b4)
 #2 0x00000000043477b8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000077e392242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x000077e3922969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x000077e392242476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x000077e3922287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x000077e39222871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x000077e392239e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000004db7fa6 clang::CodeGen::CodeGenFunction::EmitBuiltinExpr(clang::GlobalDecl, unsigned int, clang::CallExpr const*, clang::CodeGen::ReturnValueSlot) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4db7fa6)
#10 0x00000000048d29de clang::CodeGen::CodeGenFunction::EmitCallExpr(clang::CallExpr const*, clang::CodeGen::ReturnValueSlot, llvm::CallBase**) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48d29de)
#11 0x0000000004957b64 (anonymous namespace)::ScalarExprEmitter::VisitCallExpr(clang::CallExpr const*) CGExprScalar.cpp:0:0
#12 0x000000000494d2f0 clang::StmtVisitorBase<std::add_pointer, (anonymous namespace)::ScalarExprEmitter, llvm::Value*>::Visit(clang::Stmt*) CGExprScalar.cpp:0:0
#13 0x0000000004952d0c clang::CodeGen::CodeGenFunction::EmitScalarExpr(clang::Expr const*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4952d0c)
#14 0x00000000048b262e clang::CodeGen::CodeGenFunction::EmitAnyExpr(clang::Expr const*, clang::CodeGen::AggValueSlot, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48b262e)
#15 0x00000000048cb59d clang::CodeGen::CodeGenFunction::EmitIgnoredExpr(clang::Expr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48cb59d)
#16 0x0000000004a7b507 clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef<clang::Attr const*>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4a7b507)
#17 0x0000000004a83e6b clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&, bool, clang::CodeGen::AggValueSlot) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4a83e6b)
#18 0x0000000004aecf66 clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4aecf66)
#19 0x0000000004afeda4 clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4afeda4)
#20 0x0000000004b6daf0 clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b6daf0)
#21 0x0000000004b687c4 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b687c4)
#22 0x0000000004b68fa3 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b68fa3)
#23 0x0000000004b74693 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#24 0x0000000004ee6a39 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#25 0x0000000004ed7e44 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ed7e44)
#26 0x0000000006bcf6c4 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bcf6c4)
#27 0x0000000004ee47d8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ee47d8)
#28 0x00000000051eb175 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x51eb175)
#29 0x00000000051693ce clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x51693ce)
#30 0x00000000052e6e2d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x52e6e2d)
#31 0x0000000000df0afe cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdf0afe)
#32 0x0000000000de746a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#33 0x0000000000de75ed int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#34 0x0000000004f58f39 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#35 0x0000000004347bf4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4347bf4)
#36 0x0000000004f5954f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#37 0x0000000004f19232 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f19232)
#38 0x0000000004f1a1de clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f1a1de)
#39 0x0000000004f21f25 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f21f25)
#40 0x0000000000dece6e clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdece6e)
#41 0x0000000000ca503a main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xca503a)
#42 0x000077e392229d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#43 0x000077e392229e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#44 0x0000000000de6f05 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde6f05)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/37Wc9MP8v
```cpp
void foo() {
  __annotation(L"a1");
}
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/CodeGen/CGBuiltin.cpp:5373: clang::CodeGen::RValue clang::CodeGen::CodeGenFunction::EmitBuiltinExpr(clang::GlobalDecl, unsigned int, const clang::CallExpr*, clang::CodeGen::ReturnValueSlot): Assertion `Str-&gt;getCharByteWidth() == 2' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -fms-extensions &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	&lt;source&gt;:1:6: LLVM IR generation of declaration 'foo'
3.	&lt;source&gt;:1:6: Generating code for declaration 'foo'
 #<!-- -->0 0x000000000440a568 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x440a568)
 #<!-- -->1 0x00000000044079b4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x44079b4)
 #<!-- -->2 0x00000000043477b8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x000077e392242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x000077e3922969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x000077e392242476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x000077e3922287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x000077e39222871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x000077e392239e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000004db7fa6 clang::CodeGen::CodeGenFunction::EmitBuiltinExpr(clang::GlobalDecl, unsigned int, clang::CallExpr const*, clang::CodeGen::ReturnValueSlot) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4db7fa6)
#<!-- -->10 0x00000000048d29de clang::CodeGen::CodeGenFunction::EmitCallExpr(clang::CallExpr const*, clang::CodeGen::ReturnValueSlot, llvm::CallBase**) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48d29de)
#<!-- -->11 0x0000000004957b64 (anonymous namespace)::ScalarExprEmitter::VisitCallExpr(clang::CallExpr const*) CGExprScalar.cpp:0:0
#<!-- -->12 0x000000000494d2f0 clang::StmtVisitorBase&lt;std::add_pointer, (anonymous namespace)::ScalarExprEmitter, llvm::Value*&gt;::Visit(clang::Stmt*) CGExprScalar.cpp:0:0
#<!-- -->13 0x0000000004952d0c clang::CodeGen::CodeGenFunction::EmitScalarExpr(clang::Expr const*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4952d0c)
#<!-- -->14 0x00000000048b262e clang::CodeGen::CodeGenFunction::EmitAnyExpr(clang::Expr const*, clang::CodeGen::AggValueSlot, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48b262e)
#<!-- -->15 0x00000000048cb59d clang::CodeGen::CodeGenFunction::EmitIgnoredExpr(clang::Expr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48cb59d)
#<!-- -->16 0x0000000004a7b507 clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef&lt;clang::Attr const*&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4a7b507)
#<!-- -->17 0x0000000004a83e6b clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&amp;, bool, clang::CodeGen::AggValueSlot) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4a83e6b)
#<!-- -->18 0x0000000004aecf66 clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4aecf66)
#<!-- -->19 0x0000000004afeda4 clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4afeda4)
#<!-- -->20 0x0000000004b6daf0 clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b6daf0)
#<!-- -->21 0x0000000004b687c4 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b687c4)
#<!-- -->22 0x0000000004b68fa3 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b68fa3)
#<!-- -->23 0x0000000004b74693 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#<!-- -->24 0x0000000004ee6a39 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#<!-- -->25 0x0000000004ed7e44 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ed7e44)
#<!-- -->26 0x0000000006bcf6c4 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bcf6c4)
#<!-- -->27 0x0000000004ee47d8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ee47d8)
#<!-- -->28 0x00000000051eb175 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x51eb175)
#<!-- -->29 0x00000000051693ce clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x51693ce)
#<!-- -->30 0x00000000052e6e2d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x52e6e2d)
#<!-- -->31 0x0000000000df0afe cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdf0afe)
#<!-- -->32 0x0000000000de746a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->33 0x0000000000de75ed int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->34 0x0000000004f58f39 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->35 0x0000000004347bf4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4347bf4)
#<!-- -->36 0x0000000004f5954f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->37 0x0000000004f19232 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f19232)
#<!-- -->38 0x0000000004f1a1de clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f1a1de)
#<!-- -->39 0x0000000004f21f25 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f21f25)
#<!-- -->40 0x0000000000dece6e clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdece6e)
#<!-- -->41 0x0000000000ca503a main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xca503a)
#<!-- -->42 0x000077e392229d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->43 0x000077e392229e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->44 0x0000000000de6f05 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde6f05)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
Compiler returned: 134
```
</details>


---

### Comment 2 - k-arrows

Goes back to clang-6:
https://godbolt.org/z/cvMsv4M5f

---

