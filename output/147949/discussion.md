# Assertion failure in CGDeclCXX.cpp / EmitDeclInit "VarDecl must have global or local (in the case of OpenCL) storage!"

**Author:** alexfh
**URL:** https://github.com/llvm/llvm-project/issues/147949
**Status:** Closed
**Labels:** clang:frontend, crash-on-valid, regression:16
**Closed Date:** 2025-07-13T23:02:54Z

## Body

Clang crashes with an assertion failure when compiling `-fmerge-all-constants` some real code. This is an automatically reduced test case (see also https://gcc.godbolt.org/z/n54Tfhzxo):
```
typedef struct {
} Coordinate;
struct Location {
  static Coordinate FromDouble(double);
};
void TestBody() { const Coordinate x{Location::FromDouble(0.77)}; }
```

Stack trace:
```
clang++: /root/llvm-project/llvm/tools/clang/lib/CodeGen/CGDeclCXX.cpp:32: void EmitDeclInit(clang::CodeGen::CodeGenFunction&, const clang::VarDecl&, clang::CodeGen::ConstantAddress): Assertion `(D.hasGlobalStorage() || (D.hasLocalStorage() && CGF.getContext().getLangOpts().OpenCLCPlusPlus)) && "VarDecl must have global or local (in the case of OpenCL) storage!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -fmerge-all-constants <source>
1.	<eof> parser at end of file
2.	<source>:6:6: LLVM IR generation of declaration 'TestBody'
3.	<source>:6:6: Generating code for declaration 'TestBody'
 #0 0x0000000003f8ff58 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f8ff58)
 #1 0x0000000003f8d384 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f8d384)
 #2 0x0000000003ed1c18 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007aeadd042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007aeadd0969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007aeadd042476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007aeadd0287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007aeadd02871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007aeadd039e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x00000000048ae63a clang::CodeGen::CodeGenFunction::EmitCXXGlobalVarDeclInit(clang::VarDecl const&, llvm::GlobalVariable*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48ae63a)
#10 0x00000000045dae74 (anonymous namespace)::ItaniumCXXABI::EmitGuardedInit(clang::CodeGen::CodeGenFunction&, clang::VarDecl const&, llvm::GlobalVariable*, bool) ItaniumCXXABI.cpp:0:0
#11 0x0000000004893e89 clang::CodeGen::CodeGenFunction::AddInitializerToStaticVarDecl(clang::VarDecl const&, llvm::GlobalVariable*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4893e89)
#12 0x000000000489b6b8 clang::CodeGen::CodeGenFunction::EmitStaticVarDecl(clang::VarDecl const&, llvm::GlobalValue::LinkageTypes) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x489b6b8)
#13 0x000000000489f4c5 clang::CodeGen::CodeGenFunction::EmitAutoVarAlloca(clang::VarDecl const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x489f4c5)
#14 0x00000000048a36eb clang::CodeGen::CodeGenFunction::EmitVarDecl(clang::VarDecl const&) (.part.0) CGDecl.cpp:0:0
#15 0x00000000048a3ef7 clang::CodeGen::CodeGenFunction::EmitDecl(clang::Decl const&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48a3ef7)
#16 0x00000000044646cc clang::CodeGen::CodeGenFunction::EmitDeclStmt(clang::DeclStmt const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x44646cc)
#17 0x000000000447a5e2 clang::CodeGen::CodeGenFunction::EmitSimpleStmt(clang::Stmt const*, llvm::ArrayRef<clang::Attr const*>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x447a5e2)
#18 0x0000000004471e71 clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef<clang::Attr const*>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4471e71)
#19 0x000000000447a07c clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&, bool, clang::CodeGen::AggValueSlot) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x447a07c)
#20 0x00000000044e109e clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x44e109e)
#21 0x00000000044f2d94 clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x44f2d94)
#22 0x000000000455dbbd clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x455dbbd)
#23 0x0000000004558bc4 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4558bc4)
#24 0x0000000004559263 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4559263)
#25 0x0000000004563df3 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#26 0x0000000004949450 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#27 0x0000000004938a94 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4938a94)
#28 0x000000000663df74 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x663df74)
#29 0x0000000004945108 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4945108)
#30 0x0000000004c3e825 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c3e825)
#31 0x0000000004bbb9ce clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bbb9ce)
#32 0x0000000004d2fe01 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d2fe01)
#33 0x0000000000d927af cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd927af)
#34 0x0000000000d8938a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#35 0x00000000049ac259 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#36 0x0000000003ed20b4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3ed20b4)
#37 0x00000000049ac86f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#38 0x000000000496e83d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x496e83d)
#39 0x000000000496f8ce clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x496f8ce)
#40 0x0000000004978465 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4978465)
#41 0x0000000000d8ec5f clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd8ec5f)
#42 0x0000000000c43f14 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc43f14)
#43 0x00007aeadd029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#44 0x00007aeadd029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#45 0x0000000000d88e35 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd88e35)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

The crash started happening in Clang 16.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: Alexander Kornienko (alexfh)

<details>
Clang crashes with an assertion failure when compiling `-fmerge-all-constants` some real code. This is an automatically reduced test case (see also https://gcc.godbolt.org/z/n54Tfhzxo):
```
typedef struct {
} Coordinate;
struct Location {
  static Coordinate FromDouble(double);
};
void TestBody() { const Coordinate x{Location::FromDouble(0.77)}; }
```

Stack trace:
```
clang++: /root/llvm-project/llvm/tools/clang/lib/CodeGen/CGDeclCXX.cpp:32: void EmitDeclInit(clang::CodeGen::CodeGenFunction&amp;, const clang::VarDecl&amp;, clang::CodeGen::ConstantAddress): Assertion `(D.hasGlobalStorage() || (D.hasLocalStorage() &amp;&amp; CGF.getContext().getLangOpts().OpenCLCPlusPlus)) &amp;&amp; "VarDecl must have global or local (in the case of OpenCL) storage!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -fmerge-all-constants &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	&lt;source&gt;:6:6: LLVM IR generation of declaration 'TestBody'
3.	&lt;source&gt;:6:6: Generating code for declaration 'TestBody'
 #<!-- -->0 0x0000000003f8ff58 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f8ff58)
 #<!-- -->1 0x0000000003f8d384 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f8d384)
 #<!-- -->2 0x0000000003ed1c18 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007aeadd042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007aeadd0969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007aeadd042476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007aeadd0287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x00007aeadd02871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x00007aeadd039e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x00000000048ae63a clang::CodeGen::CodeGenFunction::EmitCXXGlobalVarDeclInit(clang::VarDecl const&amp;, llvm::GlobalVariable*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48ae63a)
#<!-- -->10 0x00000000045dae74 (anonymous namespace)::ItaniumCXXABI::EmitGuardedInit(clang::CodeGen::CodeGenFunction&amp;, clang::VarDecl const&amp;, llvm::GlobalVariable*, bool) ItaniumCXXABI.cpp:0:0
#<!-- -->11 0x0000000004893e89 clang::CodeGen::CodeGenFunction::AddInitializerToStaticVarDecl(clang::VarDecl const&amp;, llvm::GlobalVariable*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4893e89)
#<!-- -->12 0x000000000489b6b8 clang::CodeGen::CodeGenFunction::EmitStaticVarDecl(clang::VarDecl const&amp;, llvm::GlobalValue::LinkageTypes) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x489b6b8)
#<!-- -->13 0x000000000489f4c5 clang::CodeGen::CodeGenFunction::EmitAutoVarAlloca(clang::VarDecl const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x489f4c5)
#<!-- -->14 0x00000000048a36eb clang::CodeGen::CodeGenFunction::EmitVarDecl(clang::VarDecl const&amp;) (.part.0) CGDecl.cpp:0:0
#<!-- -->15 0x00000000048a3ef7 clang::CodeGen::CodeGenFunction::EmitDecl(clang::Decl const&amp;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48a3ef7)
#<!-- -->16 0x00000000044646cc clang::CodeGen::CodeGenFunction::EmitDeclStmt(clang::DeclStmt const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x44646cc)
#<!-- -->17 0x000000000447a5e2 clang::CodeGen::CodeGenFunction::EmitSimpleStmt(clang::Stmt const*, llvm::ArrayRef&lt;clang::Attr const*&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x447a5e2)
#<!-- -->18 0x0000000004471e71 clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef&lt;clang::Attr const*&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4471e71)
#<!-- -->19 0x000000000447a07c clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&amp;, bool, clang::CodeGen::AggValueSlot) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x447a07c)
#<!-- -->20 0x00000000044e109e clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x44e109e)
#<!-- -->21 0x00000000044f2d94 clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x44f2d94)
#<!-- -->22 0x000000000455dbbd clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x455dbbd)
#<!-- -->23 0x0000000004558bc4 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4558bc4)
#<!-- -->24 0x0000000004559263 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4559263)
#<!-- -->25 0x0000000004563df3 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#<!-- -->26 0x0000000004949450 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#<!-- -->27 0x0000000004938a94 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4938a94)
#<!-- -->28 0x000000000663df74 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x663df74)
#<!-- -->29 0x0000000004945108 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4945108)
#<!-- -->30 0x0000000004c3e825 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c3e825)
#<!-- -->31 0x0000000004bbb9ce clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bbb9ce)
#<!-- -->32 0x0000000004d2fe01 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d2fe01)
#<!-- -->33 0x0000000000d927af cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd927af)
#<!-- -->34 0x0000000000d8938a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->35 0x00000000049ac259 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->36 0x0000000003ed20b4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3ed20b4)
#<!-- -->37 0x00000000049ac86f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->38 0x000000000496e83d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x496e83d)
#<!-- -->39 0x000000000496f8ce clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x496f8ce)
#<!-- -->40 0x0000000004978465 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4978465)
#<!-- -->41 0x0000000000d8ec5f clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd8ec5f)
#<!-- -->42 0x0000000000c43f14 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc43f14)
#<!-- -->43 0x00007aeadd029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->44 0x00007aeadd029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->45 0x0000000000d88e35 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd88e35)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

The crash started happening in Clang 16.
</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Alexander Kornienko (alexfh)

<details>
Clang crashes with an assertion failure when compiling `-fmerge-all-constants` some real code. This is an automatically reduced test case (see also https://gcc.godbolt.org/z/n54Tfhzxo):
```
typedef struct {
} Coordinate;
struct Location {
  static Coordinate FromDouble(double);
};
void TestBody() { const Coordinate x{Location::FromDouble(0.77)}; }
```

Stack trace:
```
clang++: /root/llvm-project/llvm/tools/clang/lib/CodeGen/CGDeclCXX.cpp:32: void EmitDeclInit(clang::CodeGen::CodeGenFunction&amp;, const clang::VarDecl&amp;, clang::CodeGen::ConstantAddress): Assertion `(D.hasGlobalStorage() || (D.hasLocalStorage() &amp;&amp; CGF.getContext().getLangOpts().OpenCLCPlusPlus)) &amp;&amp; "VarDecl must have global or local (in the case of OpenCL) storage!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -fmerge-all-constants &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	&lt;source&gt;:6:6: LLVM IR generation of declaration 'TestBody'
3.	&lt;source&gt;:6:6: Generating code for declaration 'TestBody'
 #<!-- -->0 0x0000000003f8ff58 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f8ff58)
 #<!-- -->1 0x0000000003f8d384 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f8d384)
 #<!-- -->2 0x0000000003ed1c18 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007aeadd042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007aeadd0969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007aeadd042476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007aeadd0287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x00007aeadd02871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x00007aeadd039e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x00000000048ae63a clang::CodeGen::CodeGenFunction::EmitCXXGlobalVarDeclInit(clang::VarDecl const&amp;, llvm::GlobalVariable*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48ae63a)
#<!-- -->10 0x00000000045dae74 (anonymous namespace)::ItaniumCXXABI::EmitGuardedInit(clang::CodeGen::CodeGenFunction&amp;, clang::VarDecl const&amp;, llvm::GlobalVariable*, bool) ItaniumCXXABI.cpp:0:0
#<!-- -->11 0x0000000004893e89 clang::CodeGen::CodeGenFunction::AddInitializerToStaticVarDecl(clang::VarDecl const&amp;, llvm::GlobalVariable*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4893e89)
#<!-- -->12 0x000000000489b6b8 clang::CodeGen::CodeGenFunction::EmitStaticVarDecl(clang::VarDecl const&amp;, llvm::GlobalValue::LinkageTypes) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x489b6b8)
#<!-- -->13 0x000000000489f4c5 clang::CodeGen::CodeGenFunction::EmitAutoVarAlloca(clang::VarDecl const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x489f4c5)
#<!-- -->14 0x00000000048a36eb clang::CodeGen::CodeGenFunction::EmitVarDecl(clang::VarDecl const&amp;) (.part.0) CGDecl.cpp:0:0
#<!-- -->15 0x00000000048a3ef7 clang::CodeGen::CodeGenFunction::EmitDecl(clang::Decl const&amp;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48a3ef7)
#<!-- -->16 0x00000000044646cc clang::CodeGen::CodeGenFunction::EmitDeclStmt(clang::DeclStmt const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x44646cc)
#<!-- -->17 0x000000000447a5e2 clang::CodeGen::CodeGenFunction::EmitSimpleStmt(clang::Stmt const*, llvm::ArrayRef&lt;clang::Attr const*&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x447a5e2)
#<!-- -->18 0x0000000004471e71 clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef&lt;clang::Attr const*&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4471e71)
#<!-- -->19 0x000000000447a07c clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&amp;, bool, clang::CodeGen::AggValueSlot) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x447a07c)
#<!-- -->20 0x00000000044e109e clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x44e109e)
#<!-- -->21 0x00000000044f2d94 clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x44f2d94)
#<!-- -->22 0x000000000455dbbd clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x455dbbd)
#<!-- -->23 0x0000000004558bc4 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4558bc4)
#<!-- -->24 0x0000000004559263 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4559263)
#<!-- -->25 0x0000000004563df3 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#<!-- -->26 0x0000000004949450 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#<!-- -->27 0x0000000004938a94 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4938a94)
#<!-- -->28 0x000000000663df74 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x663df74)
#<!-- -->29 0x0000000004945108 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4945108)
#<!-- -->30 0x0000000004c3e825 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c3e825)
#<!-- -->31 0x0000000004bbb9ce clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bbb9ce)
#<!-- -->32 0x0000000004d2fe01 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d2fe01)
#<!-- -->33 0x0000000000d927af cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd927af)
#<!-- -->34 0x0000000000d8938a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->35 0x00000000049ac259 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->36 0x0000000003ed20b4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3ed20b4)
#<!-- -->37 0x00000000049ac86f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->38 0x000000000496e83d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x496e83d)
#<!-- -->39 0x000000000496f8ce clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x496f8ce)
#<!-- -->40 0x0000000004978465 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4978465)
#<!-- -->41 0x0000000000d8ec5f clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd8ec5f)
#<!-- -->42 0x0000000000c43f14 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc43f14)
#<!-- -->43 0x00007aeadd029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->44 0x00007aeadd029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->45 0x0000000000d88e35 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd88e35)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

The crash started happening in Clang 16.
</details>


---

