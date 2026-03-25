# [clang] crash on lambda capturing *this with this parameter

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/154054
**Status:** Closed
**Labels:** clang:frontend, confirmed, crash, lambda, regression:19
**Closed Date:** 2025-08-19T22:37:50Z

## Body

Reproducer:
https://godbolt.org/z/ecK6YYrM1
```cpp
struct S {
  int x;
  auto byval() {
    return [*this](this auto) { return this->x; };
  }
};

int main() {
  S s{ 42 };

  if ( s.byval()() != 42)
    __builtin_abort();
}
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/AST/ExprConstant.cpp:17135: bool clang::Expr::EvaluateAsInt(clang::Expr::EvalResult&, const clang::ASTContext&, clang::Expr::SideEffectsKind, bool) const: Assertion `!isValueDependent() && "Expression evaluator can't be called on a dependent expression."' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics --std=c++23 <source>
1.	<eof> parser at end of file
2.	<source>:8:5: LLVM IR generation of declaration 'main'
3.	<source>:8:5: Generating code for declaration 'main'
 #0 0x0000000004028478 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4028478)
 #1 0x00000000040258a4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x40258a4)
 #2 0x0000000003f6a0d8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000078959a442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x000078959a4969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x000078959a442476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x000078959a4287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x000078959a42871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x000078959a439e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x000000000793cfea (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x793cfea)
#10 0x000000000456b644 clang::CodeGen::CodeGenFunction::ConstantFoldsToSimpleInteger(clang::Expr const*, bool&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x456b644)
#11 0x0000000004502c7c clang::CodeGen::CodeGenFunction::EmitIfStmt(clang::IfStmt const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4502c7c)
#12 0x00000000044fbaae clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef<clang::Attr const*>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x44fbaae)
#13 0x00000000045039ec clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&, bool, clang::CodeGen::AggValueSlot) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x45039ec)
#14 0x000000000456ae3e clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x456ae3e)
#15 0x000000000457ca84 clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x457ca84)
#16 0x00000000045e7edd clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x45e7edd)
#17 0x00000000045e2ea4 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x45e2ea4)
#18 0x00000000045e3de0 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x45e3de0)
#19 0x00000000045ee123 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#20 0x00000000049cec70 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#21 0x00000000049be144 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49be144)
#22 0x00000000066da4e4 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66da4e4)
#23 0x00000000049ca7e8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49ca7e8)
#24 0x0000000004cbedb5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4cbedb5)
#25 0x0000000004c3af5e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c3af5e)
#26 0x0000000004db06d1 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4db06d1)
#27 0x0000000000db082f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb082f)
#28 0x0000000000da74da ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#29 0x0000000004a321d9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#30 0x0000000003f6a574 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f6a574)
#31 0x0000000004a327ef clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#32 0x00000000049f4a8d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49f4a8d)
#33 0x00000000049f5b1e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49f5b1e)
#34 0x00000000049fd565 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49fd565)
#35 0x0000000000dacce5 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdacce5)
#36 0x0000000000c60ed4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc60ed4)
#37 0x000078959a429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#38 0x000078959a429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#39 0x0000000000da6f85 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda6f85)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/ecK6YYrM1
```cpp
struct S {
  int x;
  auto byval() {
    return [*this](this auto) { return this-&gt;x; };
  }
};

int main() {
  S s{ 42 };

  if ( s.byval()() != 42)
    __builtin_abort();
}
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/AST/ExprConstant.cpp:17135: bool clang::Expr::EvaluateAsInt(clang::Expr::EvalResult&amp;, const clang::ASTContext&amp;, clang::Expr::SideEffectsKind, bool) const: Assertion `!isValueDependent() &amp;&amp; "Expression evaluator can't be called on a dependent expression."' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics --std=c++23 &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	&lt;source&gt;:8:5: LLVM IR generation of declaration 'main'
3.	&lt;source&gt;:8:5: Generating code for declaration 'main'
 #<!-- -->0 0x0000000004028478 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4028478)
 #<!-- -->1 0x00000000040258a4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x40258a4)
 #<!-- -->2 0x0000000003f6a0d8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x000078959a442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x000078959a4969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x000078959a442476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x000078959a4287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x000078959a42871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x000078959a439e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x000000000793cfea (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x793cfea)
#<!-- -->10 0x000000000456b644 clang::CodeGen::CodeGenFunction::ConstantFoldsToSimpleInteger(clang::Expr const*, bool&amp;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x456b644)
#<!-- -->11 0x0000000004502c7c clang::CodeGen::CodeGenFunction::EmitIfStmt(clang::IfStmt const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4502c7c)
#<!-- -->12 0x00000000044fbaae clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef&lt;clang::Attr const*&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x44fbaae)
#<!-- -->13 0x00000000045039ec clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&amp;, bool, clang::CodeGen::AggValueSlot) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x45039ec)
#<!-- -->14 0x000000000456ae3e clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x456ae3e)
#<!-- -->15 0x000000000457ca84 clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x457ca84)
#<!-- -->16 0x00000000045e7edd clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x45e7edd)
#<!-- -->17 0x00000000045e2ea4 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x45e2ea4)
#<!-- -->18 0x00000000045e3de0 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x45e3de0)
#<!-- -->19 0x00000000045ee123 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#<!-- -->20 0x00000000049cec70 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#<!-- -->21 0x00000000049be144 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49be144)
#<!-- -->22 0x00000000066da4e4 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66da4e4)
#<!-- -->23 0x00000000049ca7e8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49ca7e8)
#<!-- -->24 0x0000000004cbedb5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4cbedb5)
#<!-- -->25 0x0000000004c3af5e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c3af5e)
#<!-- -->26 0x0000000004db06d1 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4db06d1)
#<!-- -->27 0x0000000000db082f cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb082f)
#<!-- -->28 0x0000000000da74da ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->29 0x0000000004a321d9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->30 0x0000000003f6a574 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f6a574)
#<!-- -->31 0x0000000004a327ef clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->32 0x00000000049f4a8d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49f4a8d)
#<!-- -->33 0x00000000049f5b1e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49f5b1e)
#<!-- -->34 0x00000000049fd565 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49fd565)
#<!-- -->35 0x0000000000dacce5 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdacce5)
#<!-- -->36 0x0000000000c60ed4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc60ed4)
#<!-- -->37 0x000078959a429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->38 0x000078959a429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->39 0x0000000000da6f85 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda6f85)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

### Comment 2 - k-arrows

Goes back to clang-19.1.0:
https://godbolt.org/z/6WesYvfzj

---

### Comment 3 - shafik

@Sirraide git bisect points to 38824f285f1459cb890337d2df1a3cafd3fd109d

---

### Comment 4 - Sirraide

> [@Sirraide](https://github.com/Sirraide) git bisect points to [38824f2](https://github.com/llvm/llvm-project/commit/38824f285f1459cb890337d2df1a3cafd3fd109d)

At a glance, I’m guessing there’s something wrong with how `[*this](this auto) { return this->x; }` is instantiated: there is a flag that marks `this` as dependent if `this` is captured by value in a lambda with a dependent explicit object parameter; maybe it isn’t being cleared when we instantiate the template?

---

### Comment 5 - Sirraide

Possibly? reduced version (https://godbolt.org/z/ra8nnfvMP):
```c++
struct s {
    auto f() { return [*this](this auto) { return this; }; }
};

void f() {
  s().f()();
}
```
‘Possibly’ because the stack trace and crash are different.

---

