# Clang accepts indirectly invoking a consteval function outside of a manifestly constant evaluated context

**Author:** Mathusela
**URL:** https://github.com/llvm/llvm-project/issues/183415

## Body

https://godbolt.org/z/7rG65c13e

The following code is accepted by clang 22.1.0: 

```cpp
consteval void f(int) {}

int main() {
    auto g = []() consteval {
        return [](int x) {
            f(x);
        };
    }();

    int x = 42;
    g(x);
}
```

A PLT call to the wrapped consteval function (`f`) is emitted which invokes a linker error.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Matthew Richardson (Mathusela)

<details>
https://godbolt.org/z/7rG65c13e

The following code is accepted by clang 22.0.1: 

```cpp
consteval void f(int) {}

int main() {
    auto g = []() consteval {
        return [](int x) {
            f(x);
        };
    }();

    int x = 42;
    g(x);
}
```

A PLT call to the wrapped consteval function (`f`) is emitted which invokes a linker error.
</details>


---

### Comment 2 - keinflue

Since Clang 21. Also asserts with assertions enabled (https://godbolt.org/z/bEn7GnoKG):

```
clang++: /root/llvm-project/clang/lib/CodeGen/CGExpr.cpp:6284: clang::CodeGen::RValue clang::CodeGen::CodeGenFunction::EmitCall(clang::QualType, const clang::CodeGen::CGCallee&, const clang::CallExpr*, clang::CodeGen::ReturnValueSlot, llvm::Value*, llvm::CallBase**, const clang::CodeGen::CGFunctionInfo**): Assertion `(!isa_and_present<FunctionDecl>(TargetDecl) || !cast<FunctionDecl>(TargetDecl)->isImmediateFunction()) && "trying to emit a call to an immediate function"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-21.1.0/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-15.2.0 -fcolor-diagnostics -fno-crash-diagnostics -O3 -std=c++23 <source>
1.	<eof> parser at end of file
2.	Per-file LLVM IR generation
3.	<source>:5:16: Generating code for declaration 'main()::(anonymous class)::operator()()::(anonymous class)::operator()'
 #0 0x0000000004019ae8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x4019ae8)
 #1 0x0000000004016f14 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x4016f14)
 #2 0x0000000003f5b698 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007a6a1c842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007a6a1c8969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007a6a1c842476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007a6a1c8287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007a6a1c82871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007a6a1c839e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x00000000043837e7 clang::CodeGen::CodeGenFunction::EmitCall(clang::QualType, clang::CodeGen::CGCallee const&, clang::CallExpr const*, clang::CodeGen::ReturnValueSlot, llvm::Value*, llvm::CallBase**, clang::CodeGen::CGFunctionInfo const**) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x43837e7)
#10 0x00000000043841ed clang::CodeGen::CodeGenFunction::EmitCallExpr(clang::CallExpr const*, clang::CodeGen::ReturnValueSlot, llvm::CallBase**) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x43841ed)
#11 0x00000000043f5454 (anonymous namespace)::ScalarExprEmitter::VisitCallExpr(clang::CallExpr const*) CGExprScalar.cpp:0:0
#12 0x00000000043e9a00 clang::StmtVisitorBase<std::add_pointer, (anonymous namespace)::ScalarExprEmitter, llvm::Value*>::Visit(clang::Stmt*) CGExprScalar.cpp:0:0
#13 0x00000000043ee1ac clang::CodeGen::CodeGenFunction::EmitScalarExpr(clang::Expr const*, bool) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x43ee1ac)
#14 0x000000000435460e clang::CodeGen::CodeGenFunction::EmitAnyExpr(clang::Expr const*, clang::CodeGen::AggValueSlot, bool) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x435460e)
#15 0x0000000004381e0d clang::CodeGen::CodeGenFunction::EmitIgnoredExpr(clang::Expr const*) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x4381e0d)
#16 0x00000000044fb4e7 clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef<clang::Attr const*>) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x44fb4e7)
#17 0x000000000450360c clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&, bool, clang::CodeGen::AggValueSlot) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x450360c)
#18 0x000000000456a68e clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x456a68e)
#19 0x000000000457c384 clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x457c384)
#20 0x00000000045e71dd clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x45e71dd)
#21 0x00000000045e21d4 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x45e21d4)
#22 0x00000000045eeb1c clang::CodeGen::CodeGenModule::EmitDeferred() (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x45eeb1c)
#23 0x00000000045f10a3 clang::CodeGen::CodeGenModule::Release() (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x45f10a3)
#24 0x00000000049d2f4e (anonymous namespace)::CodeGeneratorImpl::HandleTranslationUnit(clang::ASTContext&) ModuleBuilder.cpp:0:0
#25 0x00000000049cf285 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x49cf285)
#26 0x00000000066ca95c clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x66ca95c)
#27 0x00000000049cfa28 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x49cfa28)
#28 0x0000000004cc9f95 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x4cc9f95)
#29 0x0000000004c4688e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x4c4688e)
#30 0x0000000004dbc2e1 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x4dbc2e1)
#31 0x0000000000d93cff cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0xd93cff)
#32 0x0000000000d8a8ba ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#33 0x0000000004a36b79 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#34 0x0000000003f5bb34 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x3f5bb34)
#35 0x0000000004a3718f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#36 0x00000000049f915d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x49f915d)
#37 0x00000000049fa1ee clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x49fa1ee)
#38 0x0000000004a02d85 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x4a02d85)
#39 0x0000000000d9018f clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0xd9018f)
#40 0x0000000000c444d4 main (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0xc444d4)
#41 0x00007a6a1c829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#42 0x00007a6a1c829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#43 0x0000000000d8a365 _start (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0xd8a365)
```

---

### Comment 3 - zyn0217

@cor3ntin 

---

