# Clang frontend C++ crash on invalid code when using `typeid` 

**Author:** brutalsavage
**URL:** https://github.com/llvm/llvm-project/issues/63242
**Status:** Closed
**Labels:** clang:frontend, crash-on-invalid, confirmed, regression:16
**Closed Date:** 2026-02-26T06:31:38Z

## Body

To quickly reproduce: https://gcc.godbolt.org/z/Ens9e5xnn 

```cpp
#include <type_traits>
#include <typeinfo>

struct scoped {
    enum class scoped2 {
        RED,
        YELLOW,
        GREEN
    };
};
template<auto N>
struct scoped_struct {
    void f() {
        class scoped2 e = scoped::scoped2::RED;
        typeid(e); //remove this = no crash
    }
};
int main() {
    constexpr scoped_struct<scoped::scoped2::RED> s{};
    s.f();
}
```

Compiling the above invalid code crashes clang `clang++ -x c++ -std=c++23`, crashes locally using clang-17.0 (a10019a), also on trunk (see godbolt link)

Note: removing the `typeid(e);` leads to no crash.




## Comments

### Comment 1 - llvmbot

@llvm/issue-subscribers-clang-frontend

---

### Comment 2 - shafik

Confirmed: https://gcc.godbolt.org/z/T41qG6WTc

Assertion:

```console
clang++: /root/llvm-project/clang/include/clang/AST/DeclCXX.h:452:
clang::CXXRecordDecl::DefinitionData& clang::CXXRecordDecl::data() const:
Assertion `DD && "queried property of class with no definition"' failed.
```

Backtrace:

```console
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<eof> parser at end of file
2.	<source>:13:10: instantiating function definition 'scoped_struct<scoped::scoped2::RED>::f'
 #0 0x000055684a30824f llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3c2824f)
 #1 0x000055684a305fbc llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3c25fbc)
 #2 0x000055684a24f3b8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007fe05821a420 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x14420)
 #4 0x00007fe057ce700b raise (/lib/x86_64-linux-gnu/libc.so.6+0x4300b)
 #5 0x00007fe057cc6859 abort (/lib/x86_64-linux-gnu/libc.so.6+0x22859)
 #6 0x00007fe057cc6729 (/lib/x86_64-linux-gnu/libc.so.6+0x22729)
 #7 0x00007fe057cd7fd6 (/lib/x86_64-linux-gnu/libc.so.6+0x33fd6)
 #8 0x000055684d5ccc86 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCXXTypeidExpr(clang::CXXTypeidExpr*) SemaTemplateInstantiate.cpp:0:0
 #9 0x000055684d59b078 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#10 0x000055684d5d390f clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformStmt(clang::Stmt*, clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::StmtDiscardKind) SemaTemplateInstantiate.cpp:0:0
#11 0x000055684d5d4b79 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaTemplateInstantiate.cpp:0:0
#12 0x000055684d5d7cce clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ef7cce)
#13 0x000055684d628838 clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f48838)
#14 0x000055684d626c3f clang::Sema::PerformPendingInstantiations(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f46c3f)
#15 0x000055684cbd4d20 clang::Sema::ActOnEndOfTranslationUnitFragment(clang::Sema::TUFragmentKind) (.part.0) Sema.cpp:0:0
#16 0x000055684cbd53ea clang::Sema::ActOnEndOfTranslationUnit() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64f53ea)
#17 0x000055684ca674b3 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x63874b3)
#18 0x000055684ca5b0ba clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x637b0ba)
#19 0x000055684b55b598 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e7b598)
#20 0x000055684adabc79 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x46cbc79)
#21 0x000055684ad30316 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4650316)
#22 0x000055684ae8e0b6 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x47ae0b6)
#23 0x00005568477b116d cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x10d116d)
#24 0x00005568477ace6a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#25 0x000055684ab98c1d void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#26 0x000055684a24f8c0 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3b6f8c0)
#27 0x000055684ab9923f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#28 0x000055684ab60a1c clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4480a1c)
#29 0x000055684ab614ad clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x44814ad)
#30 0x000055684ab6958d clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x448958d)
#31 0x00005568477af3ca clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x10cf3ca)
#32 0x00005568476b62b5 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xfd62b5)
#33 0x00007fe057cc8083 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x24083)
#34 0x00005568477a7c4e _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x10c7c4e)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

---

