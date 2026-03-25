# [Clang] Crash in CodeGen when using explicit template parameters in a lambda

**Author:** philnik777
**URL:** https://github.com/llvm/llvm-project/issues/145881

## Body

```
void func() {
  []<class = void> {}();
}
```
causes Clang to crash when in `-std=c++11` mode.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Nikolas Klauser (philnik777)

<details>
```
void func() {
  []&lt;class = void&gt; {}();
}
```
causes Clang to crash when in `-std=c++11` mode.
</details>


---

### Comment 2 - AaronBallman

Confirmed: https://godbolt.org/z/9dbTWWbn1
```
Unexpected placeholder builtin type!
UNREACHABLE executed at /root/llvm-project/llvm/tools/clang/lib/CodeGen/CodeGenTypes.cpp:593!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++11 <source>
1.	<eof> parser at end of file
2.	<source>:1:6: LLVM IR generation of declaration 'func'
3.	<source>:1:6: Generating code for declaration 'func'
 #0 0x0000000003f76ee8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f76ee8)
 #1 0x0000000003f74b74 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f74b74)
 #2 0x0000000003eb9668 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007bf1c3c42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007bf1c3c969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007bf1c3c42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007bf1c3c287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x0000000003ec4daa (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3ec4daa)
 #8 0x0000000004596242 clang::CodeGen::CodeGenTypes::ConvertType(clang::QualType) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4596242)
 #9 0x00000000048289ef clang::CodeGen::CodeGenTypes::arrangeLLVMFunctionInfo(clang::CanQual<clang::Type>, clang::CodeGen::FnInfoOpts, llvm::ArrayRef<clang::CanQual<clang::Type>>, clang::FunctionType::ExtInfo, llvm::ArrayRef<clang::FunctionType::ExtParameterInfo>, clang::CodeGen::RequiredArgs) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48289ef)
#10 0x000000000482b5fe arrangeLLVMFunctionInfo(clang::CodeGen::CodeGenTypes&, bool, llvm::SmallVectorImpl<clang::CanQual<clang::Type>>&, clang::CanQual<clang::FunctionProtoType>) CGCall.cpp:0:0
#11 0x000000000482eb2c clang::CodeGen::CodeGenTypes::arrangeCXXMethodType(clang::CXXRecordDecl const*, clang::FunctionProtoType const*, clang::CXXMethodDecl const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x482eb2c)
#12 0x000000000482ed2b clang::CodeGen::CodeGenTypes::arrangeCXXMethodDeclaration(clang::CXXMethodDecl const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x482ed2b)
#13 0x0000000004307a21 clang::CodeGen::CodeGenFunction::EmitCXXMemberOrOperatorMemberCallExpr(clang::CallExpr const*, clang::CXXMethodDecl const*, clang::CodeGen::ReturnValueSlot, bool, clang::NestedNameSpecifier*, bool, clang::Expr const*, llvm::CallBase**) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4307a21)
#14 0x0000000004308b27 clang::CodeGen::CodeGenFunction::EmitCXXOperatorMemberCallExpr(clang::CXXOperatorCallExpr const*, clang::CXXMethodDecl const*, clang::CodeGen::ReturnValueSlot, llvm::CallBase**) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4308b27)
#15 0x00000000042e3976 clang::CodeGen::CodeGenFunction::EmitCallExpr(clang::CallExpr const*, clang::CodeGen::ReturnValueSlot, llvm::CallBase**) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42e3976)
#16 0x00000000043564d4 (anonymous namespace)::ScalarExprEmitter::VisitCallExpr(clang::CallExpr const*) CGExprScalar.cpp:0:0
#17 0x0000000004349200 clang::StmtVisitorBase<std::add_pointer, (anonymous namespace)::ScalarExprEmitter, llvm::Value*>::Visit(clang::Stmt*) CGExprScalar.cpp:0:0
#18 0x00000000043579ca (anonymous namespace)::ScalarExprEmitter::VisitExprWithCleanups(clang::ExprWithCleanups*) CGExprScalar.cpp:0:0
#19 0x0000000004349daf clang::StmtVisitorBase<std::add_pointer, (anonymous namespace)::ScalarExprEmitter, llvm::Value*>::Visit(clang::Stmt*) CGExprScalar.cpp:0:0
#20 0x000000000434f20c clang::CodeGen::CodeGenFunction::EmitScalarExpr(clang::Expr const*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x434f20c)
#21 0x00000000042b3f5e clang::CodeGen::CodeGenFunction::EmitAnyExpr(clang::Expr const*, clang::CodeGen::AggValueSlot, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42b3f5e)
#22 0x00000000042e162d clang::CodeGen::CodeGenFunction::EmitIgnoredExpr(clang::Expr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42e162d)
#23 0x000000000445b377 clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef<clang::Attr const*>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x445b377)
#24 0x000000000446350c clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&, bool, clang::CodeGen::AggValueSlot) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x446350c)
#25 0x00000000044ca53e clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x44ca53e)
#26 0x00000000044dc2d4 clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x44dc2d4)
#27 0x0000000004546a7d clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4546a7d)
#28 0x0000000004541a84 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4541a84)
#29 0x0000000004542123 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4542123)
#30 0x000000000454cc53 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#31 0x0000000004931820 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#32 0x0000000004920ed4 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4920ed4)
#33 0x0000000006620c34 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6620c34)
#34 0x000000000492d5c8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x492d5c8)
#35 0x0000000004c29745 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c29745)
#36 0x0000000004ba65de clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ba65de)
#37 0x0000000004d1c291 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d1c291)
#38 0x0000000000d8735f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd8735f)
#39 0x0000000000d7df8a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#40 0x0000000004993839 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#41 0x0000000003eb9b04 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3eb9b04)
#42 0x0000000004993e4f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#43 0x00000000049566fd clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49566fd)
#44 0x000000000495778e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x495778e)
#45 0x000000000495f225 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x495f225)
#46 0x0000000000d8386f clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd8386f)
#47 0x0000000000c39614 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc39614)
#48 0x00007bf1c3c29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#49 0x00007bf1c3c29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#50 0x0000000000d7da35 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd7da35)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

---

### Comment 3 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: Nikolas Klauser (philnik777)

<details>
```
void func() {
  []&lt;class = void&gt; {}();
}
```
causes Clang to crash when in `-std=c++11` mode.
</details>


---

### Comment 4 - shafik

Looks like crash goes back to clang-9.

---

### Comment 5 - yronglin

Hi, I have tried to debug this issue. This example use a C++20 syntax in C++11 mode. The Parser emit an warning message and then continue parsing, but in Sema, C++20 and C++11 have two different code paths, this cause the lambda return type is a dependent type, finally CodeGen reject this AST and crash. I have some questions about handle features higher than the current C++ version in Sema: If the user specifies C++11 mode with `-std=c++11` cmd option but uses a C++14/17/20 feature, does this feature should go through the same code path for C++11 mode and C++20 mode in Sema? Otherwise we may encounter some unknown errors. Should we reject them or handle them according to the higher C++ version mode?
```
<source>:2:5: warning: explicit template parameter list for lambdas is a C++20 extension [-Wc++20-extensions]
    2 |   []<class = void> {}();
      |     ^
```

https://github.com/llvm/llvm-project/blob/21d5017a05d1f57ee54b20cb982eea263dd855d9/clang/lib/Sema/SemaLambda.cpp#L931-L933
https://github.com/llvm/llvm-project/blob/21d5017a05d1f57ee54b20cb982eea263dd855d9/clang/lib/Sema/SemaExpr.cpp#L83-L87

---

### Comment 6 - shafik

Note, this does not crash in C++14 mode, so there is some difference between how we handle C++11 and C++14: https://godbolt.org/z/d7ohPGGbs

Maybe looking at

8205a814a691b 

may help to see what was missed.

---

### Comment 7 - yronglin

Should we use a flag in `FunctionScopeInfo` instead of `getLangOpts().CPlusPlus14` to determine whether to perform return type deduction?(https://github.com/llvm/llvm-project/commit/2a7d481faf5464458a8dbe40beddb094eb54b2a8#diff-0c72f2a03b93148c901ed5cb9a7da141806ffd8efca60335af9d22ba9500a5d5R2581). We should have warned users during the parse phase that newer C++ features are being used.

---

### Comment 8 - yronglin

CC @cor3ntin 

---

### Comment 9 - zwuis

Looks like a duplicate of #56013.

---

### Comment 10 - philnik777

Is there any update on this? It's biting us again and again in libc++.

---

