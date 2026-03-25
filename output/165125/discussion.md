# [clang][bytecode] CodeGen crash in presence of `-fexperimental-new-constant-interpreter`

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/165125
**Status:** Open
**Labels:** clang:codegen, crash, clang:bytecode

## Body

Reproducer:
https://godbolt.org/z/zEEEjefv9
```c
void foo(void);
void bar(void);
void baz(void) {
  unsigned long qux[(((unsigned long)(&(bar)) - (unsigned long)(&foo)))];
}
```

Backtrace:
```console
clang: /root/llvm-project/llvm/include/llvm/ADT/APSInt.h:100: int64_t llvm::APSInt::getExtValue() const: Assertion `isRepresentableByInt64() && "Too many bits for int64_t"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -fexperimental-new-constant-interpreter <source>
1.	<eof> parser at end of file
2.	<source>:3:6: LLVM IR generation of declaration 'baz'
3.	<source>:3:6: Generating code for declaration 'baz'
 #0 0x0000000004214d18 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4214d18)
 #1 0x0000000004212144 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4212144)
 #2 0x0000000004156348 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000075f064042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x000075f0640969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x000075f064042476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x000075f0640287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x000075f06402871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x000075f064039e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000004527c7c clang::CodeGen::CGDebugInfo::CreateType(clang::ArrayType const*, llvm::DIFile*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4527c7c)
#10 0x0000000004522c35 clang::CodeGen::CGDebugInfo::CreateTypeNode(clang::QualType, llvm::DIFile*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4522c35)
#11 0x0000000004522ef0 clang::CodeGen::CGDebugInfo::getOrCreateType(clang::QualType, llvm::DIFile*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4522ef0)
#12 0x000000000452668a clang::CodeGen::CGDebugInfo::EmitDeclare(clang::VarDecl const*, llvm::Value*, std::optional<unsigned int>, clang::CodeGen::CGBuilderTy&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x452668a)
#13 0x0000000004ab995c clang::CodeGen::CodeGenFunction::EmitAutoVarAlloca(clang::VarDecl const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4ab995c)
#14 0x0000000004abaeee clang::CodeGen::CodeGenFunction::EmitVarDecl(clang::VarDecl const&) (.part.0) CGDecl.cpp:0:0
#15 0x0000000004abb587 clang::CodeGen::CodeGenFunction::EmitDecl(clang::Decl const&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4abb587)
#16 0x00000000046f7f5c clang::CodeGen::CodeGenFunction::EmitDeclStmt(clang::DeclStmt const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x46f7f5c)
#17 0x000000000470eccb clang::CodeGen::CodeGenFunction::EmitSimpleStmt(clang::Stmt const*, llvm::ArrayRef<clang::Attr const*>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x470eccb)
#18 0x0000000004706381 clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef<clang::Attr const*>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4706381)
#19 0x000000000470e65c clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&, bool, clang::CodeGen::AggValueSlot) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x470e65c)
#20 0x0000000004776c2e clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4776c2e)
#21 0x0000000004788c84 clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4788c84)
#22 0x00000000047f5d80 clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x47f5d80)
#23 0x00000000047f0c44 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x47f0c44)
#24 0x00000000047f1ae0 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x47f1ae0)
#25 0x00000000047fc2f3 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#26 0x0000000004b61ae9 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#27 0x0000000004b50c04 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4b50c04)
#28 0x00000000069011c4 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x69011c4)
#29 0x0000000004b5d4e8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4b5d4e8)
#30 0x0000000004e4a685 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4e4a685)
#31 0x0000000004dcb73e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4dcb73e)
#32 0x0000000004f41c2d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4f41c2d)
#33 0x0000000000dce380 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xdce380)
#34 0x0000000000dc4eea ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#35 0x0000000000dc506d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#36 0x0000000004bc6a79 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#37 0x00000000041567e4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x41567e4)
#38 0x0000000004bc708f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#39 0x0000000004b87fd2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4b87fd2)
#40 0x0000000004b88f7e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4b88f7e)
#41 0x0000000004b90695 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4b90695)
#42 0x0000000000dca8a9 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xdca8a9)
#43 0x0000000000c79c64 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xc79c64)
#44 0x000075f064029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#45 0x000075f064029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#46 0x0000000000dc4985 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xdc4985)
clang: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/zEEEjefv9
```c
void foo(void);
void bar(void);
void baz(void) {
  unsigned long qux[(((unsigned long)(&amp;(bar)) - (unsigned long)(&amp;foo)))];
}
```

Backtrace:
```console
clang: /root/llvm-project/llvm/include/llvm/ADT/APSInt.h:100: int64_t llvm::APSInt::getExtValue() const: Assertion `isRepresentableByInt64() &amp;&amp; "Too many bits for int64_t"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -fexperimental-new-constant-interpreter &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	&lt;source&gt;:3:6: LLVM IR generation of declaration 'baz'
3.	&lt;source&gt;:3:6: Generating code for declaration 'baz'
 #<!-- -->0 0x0000000004214d18 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4214d18)
 #<!-- -->1 0x0000000004212144 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4212144)
 #<!-- -->2 0x0000000004156348 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x000075f064042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x000075f0640969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x000075f064042476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x000075f0640287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x000075f06402871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x000075f064039e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000004527c7c clang::CodeGen::CGDebugInfo::CreateType(clang::ArrayType const*, llvm::DIFile*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4527c7c)
#<!-- -->10 0x0000000004522c35 clang::CodeGen::CGDebugInfo::CreateTypeNode(clang::QualType, llvm::DIFile*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4522c35)
#<!-- -->11 0x0000000004522ef0 clang::CodeGen::CGDebugInfo::getOrCreateType(clang::QualType, llvm::DIFile*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4522ef0)
#<!-- -->12 0x000000000452668a clang::CodeGen::CGDebugInfo::EmitDeclare(clang::VarDecl const*, llvm::Value*, std::optional&lt;unsigned int&gt;, clang::CodeGen::CGBuilderTy&amp;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x452668a)
#<!-- -->13 0x0000000004ab995c clang::CodeGen::CodeGenFunction::EmitAutoVarAlloca(clang::VarDecl const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4ab995c)
#<!-- -->14 0x0000000004abaeee clang::CodeGen::CodeGenFunction::EmitVarDecl(clang::VarDecl const&amp;) (.part.0) CGDecl.cpp:0:0
#<!-- -->15 0x0000000004abb587 clang::CodeGen::CodeGenFunction::EmitDecl(clang::Decl const&amp;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4abb587)
#<!-- -->16 0x00000000046f7f5c clang::CodeGen::CodeGenFunction::EmitDeclStmt(clang::DeclStmt const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x46f7f5c)
#<!-- -->17 0x000000000470eccb clang::CodeGen::CodeGenFunction::EmitSimpleStmt(clang::Stmt const*, llvm::ArrayRef&lt;clang::Attr const*&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x470eccb)
#<!-- -->18 0x0000000004706381 clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef&lt;clang::Attr const*&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4706381)
#<!-- -->19 0x000000000470e65c clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&amp;, bool, clang::CodeGen::AggValueSlot) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x470e65c)
#<!-- -->20 0x0000000004776c2e clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4776c2e)
#<!-- -->21 0x0000000004788c84 clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4788c84)
#<!-- -->22 0x00000000047f5d80 clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x47f5d80)
#<!-- -->23 0x00000000047f0c44 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x47f0c44)
#<!-- -->24 0x00000000047f1ae0 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x47f1ae0)
#<!-- -->25 0x00000000047fc2f3 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#<!-- -->26 0x0000000004b61ae9 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#<!-- -->27 0x0000000004b50c04 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4b50c04)
#<!-- -->28 0x00000000069011c4 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x69011c4)
#<!-- -->29 0x0000000004b5d4e8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4b5d4e8)
#<!-- -->30 0x0000000004e4a685 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4e4a685)
#<!-- -->31 0x0000000004dcb73e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4dcb73e)
#<!-- -->32 0x0000000004f41c2d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4f41c2d)
#<!-- -->33 0x0000000000dce380 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xdce380)
#<!-- -->34 0x0000000000dc4eea ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->35 0x0000000000dc506d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->36 0x0000000004bc6a79 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->37 0x00000000041567e4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x41567e4)
#<!-- -->38 0x0000000004bc708f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->39 0x0000000004b87fd2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4b87fd2)
#<!-- -->40 0x0000000004b88f7e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4b88f7e)
#<!-- -->41 0x0000000004b90695 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4b90695)
#<!-- -->42 0x0000000000dca8a9 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xdca8a9)
#<!-- -->43 0x0000000000c79c64 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xc79c64)
#<!-- -->44 0x000075f064029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->45 0x000075f064029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->46 0x0000000000dc4985 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xdc4985)
clang: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

