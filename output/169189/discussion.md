# clang triggers an "UNREACHABLE executed" assertion (somehow related to typename)

**Author:** pkl97
**URL:** https://github.com/llvm/llvm-project/issues/169189
**Status:** Open
**Labels:** clang:codegen, confirmed, crash, regression:18

## Body

This test program shows the assertion on Godbolt: https://godbolt.org/z/Md3vhYx1j

```cpp
#include <string>

template<typename T>
class Presets final
{
public:
    using PresetMapping=T;
};

class Class
{
public:
    template<typename T>
    void updatePresets(Presets<T>::PresetMapping& p_presets);
    template<typename T>
    Presets<T>::PresetMapping::value_type getVisiblePreset();
};

template<typename T>
void Class::updatePresets(Presets<T>::PresetMapping& /*p_presets*/)
{
    [[maybe_unused]] const typename Presets<T>::PresetMapping::value_type visiblePreset = getVisiblePreset<T>();
}

template<typename T>
Presets<T>::PresetMapping::value_type Class::getVisiblePreset()
{
    return {};
}

int main()
{
    Presets<std::string>::PresetMapping presetMapping;
    Class().updatePresets<std::string>(presetMapping);
    return 0;
}

```


Interestingly, the assertion vanishes if two "typename" keywords are added in lines 16 and 26: https://godbolt.org/z/Kza9z5eYn

This was actually how I found the issue. clang-tidy check readability-redundant-typename reported the two "typename" keywords to be unnecessary. However after their removal the assertion occurred.

MSVC 17.14 (VS 2022) compiles the code with and without the "typename" keyword.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: None (pkl97)

<details>
This test program shows the assertion on Godbolt: https://godbolt.org/z/Md3vhYx1j

Interestingly, the assertion vanishes if two "typename" keywords are added in lines 16 and 26: https://godbolt.org/z/Kza9z5eYn

This was actually how I found the issue. clang-tidy check readability-redundant-typename reported the two "typename" keywords to be unnecessary. However after their removal the assertion occurred.

MSVC 17.14 (VS 2022) compiles the code with and without the "typename" keyword.
</details>


---

### Comment 2 - zwuis

Maybe related: #17283.

---

### Comment 3 - shafik

Goes back to clang-18: https://godbolt.org/z/a9ex8fhEz

unrechable:

```console
non-canonical or dependent type in IR-generation
UNREACHABLE executed at /root/llvm-project/llvm/tools/clang/lib/CodeGen/CodeGenFunction.cpp:259!
```

Backtrace:

```console
 #0 0x00000000042b8698 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42b8698)
 #1 0x00000000042b5ac4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42b5ac4)
 #2 0x00000000041fa898 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007d6757242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007d67572969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007d6757242476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007d67572287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00000000042054ca (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42054ca)
 #8 0x0000000004822e42 clang::CodeGen::CodeGenFunction::getEvaluationKind(clang::QualType) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4822e42)
 #9 0x0000000004698850 clang::CodeGen::CodeGenFunction::EmitScalarExpr(clang::Expr const*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4698850)
#10 0x0000000004b53f81 clang::CodeGen::CodeGenFunction::EmitScalarInit(clang::Expr const*, clang::ValueDecl const*, clang::CodeGen::LValue, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b53f81)
#11 0x0000000004b55e04 clang::CodeGen::CodeGenFunction::EmitExprAsInit(clang::Expr const*, clang::ValueDecl const*, clang::CodeGen::LValue, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b55e04)
#12 0x0000000004b64290 clang::CodeGen::CodeGenFunction::EmitAutoVarInit(clang::CodeGen::CodeGenFunction::AutoVarEmission const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b64290)
#13 0x0000000004b64926 clang::CodeGen::CodeGenFunction::EmitVarDecl(clang::VarDecl const&) (.part.0) CGDecl.cpp:0:0
#14 0x0000000004b64ff7 clang::CodeGen::CodeGenFunction::EmitDecl(clang::Decl const&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b64ff7)
#15 0x00000000047a70cc clang::CodeGen::CodeGenFunction::EmitDeclStmt(clang::DeclStmt const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x47a70cc)
#16 0x00000000047bdedb clang::CodeGen::CodeGenFunction::EmitSimpleStmt(clang::Stmt const*, llvm::ArrayRef<clang::Attr const*>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x47bdedb)
#17 0x00000000047b54f1 clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef<clang::Attr const*>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x47b54f1)
#18 0x00000000047bd90b clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&, bool, clang::CodeGen::AggValueSlot) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x47bd90b)
#19 0x000000000482569e clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x482569e)
#20 0x0000000004836aa4 clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4836aa4)
#21 0x00000000048a45b0 clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48a45b0)
#22 0x000000000489f224 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x489f224)
#23 0x00000000048ac4ec clang::CodeGen::CodeGenModule::EmitDeferred() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48ac4ec)
#24 0x00000000048aeb93 clang::CodeGen::CodeGenModule::Release() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48aeb93)
#25 0x0000000004c08b5e (anonymous namespace)::CodeGeneratorImpl::HandleTranslationUnit(clang::ASTContext&) ModuleBuilder.cpp:0:0
#26 0x0000000004c06575 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c06575)
#27 0x00000000069b311c clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69b311c)
#28 0x0000000004c06d58 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c06d58)
#29 0x0000000004efc015 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4efc015)
#30 0x0000000004e7c07e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e7c07e)
#31 0x0000000004ff5ddd clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ff5ddd)
#32 0x0000000000dccf60 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdccf60)
#33 0x0000000000dc39da ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#34 0x0000000000dc3b5d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#35 0x0000000004c79839 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#36 0x00000000041fad34 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41fad34)
#37 0x0000000004c79e4f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#38 0x0000000004c3a642 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c3a642)
#39 0x0000000004c3b5ee clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c3b5ee)
#40 0x0000000004c42a25 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c42a25)
#41 0x0000000000dc9381 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdc9381)
#42 0x0000000000c74fa4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc74fa4)
#43 0x00007d6757229d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#44 0x00007d6757229e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#45 0x0000000000dc3475 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdc3475)
```

---

### Comment 4 - shafik

@Fznamznon this bisects to 20a05677f9394d4bc9467fe7bc93a4ebd3aeda61

---

