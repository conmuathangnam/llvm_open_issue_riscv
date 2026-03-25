# Clang crashes while compiling my project

**Author:** ogigato
**URL:** https://github.com/llvm/llvm-project/issues/72880
**Status:** Closed
**Labels:** c++20, clang:frontend, confirmed, crash-on-valid
**Closed Date:** 2025-05-05T16:46:41Z

## Body

CLang version: 16.0.5, using MSVC-like command line. This project compiles ok using MSVC compiler, with the same arguments.

[SourcesAndTrace.zip](https://github.com/llvm/llvm-project/files/13498012/SourcesAndTrace.zip)



## Comments

### Comment 1 - shafik

I can't seem to open the zip file.

---

### Comment 2 - ogigato

Hi, thank you for your interest. Tomorrow morning I will upload a new zip
file. Perhaps it fails because I used 7-zip and deflate64 as compression
algorithm.

Regards,
Óscar.

El El mar, 28 nov 2023 a las 22:48, Shafik Yaghmour <
***@***.***> escribió:

> I can't seem to open the zip file.
>
> —
> Reply to this email directly, view it on GitHub
> <https://github.com/llvm/llvm-project/issues/72880#issuecomment-1830801700>,
> or unsubscribe
> <https://github.com/notifications/unsubscribe-auth/AR72JGNLUMWZL4QDYME5U33YGZLZVAVCNFSM6AAAAAA7TBA5I2VHI2DSMVQWIX3LMV43OSLTON2WKQ3PNVWWK3TUHMYTQMZQHAYDCNZQGA>
> .
> You are receiving this because you authored the thread.Message ID:
> ***@***.***>
>


---

### Comment 3 - ogigato

Hi shafik, I have updated the zip file. I hope you can open it now.

BRs.

---

### Comment 4 - Endilll

Confirmed in Clang 17 and post-18 trunk: https://godbolt.org/z/81qnjez5o
Reduced by me and C-Reduce:
```cpp
struct stChildParam {};

struct RegParam : stChildParam {
  int count = 0;
};

template <typename _TFC>
struct DataGather {
  void AddParameters() {
    RegParam regPar = static_cast<RegParam>(stChildParam{});
  }
};

void f() {
  DataGather<int>{}.AddParameters();
}
```
Backtrace:
```
clang++: /root/llvm-project/clang/include/clang/AST/Type.h:753:
const clang::ExtQualsTypeCommonBase* clang::QualType::getCommonPtr() const:
Assertion `!isNull() && "Cannot retrieve a NULL type pointer"' failed.

PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 <source>
1.	<eof> parser at end of file
2.	<source>:9:8: instantiating function definition 'DataGather<int>::AddParameters'
 #0 0x000000000388a128 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x388a128)
 #1 0x0000000003887e0c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3887e0c)
 #2 0x00000000037d02e8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007ffb19e42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007ffb19e969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007ffb19e42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007ffb19e287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007ffb19e2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007ffb19e39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x00000000062762fc (anonymous namespace)::CastOperation::CastOperation(clang::Sema&, clang::QualType, clang::ActionResult<clang::Expr*, true>) SemaCast.cpp:0:0
#10 0x00000000062832ae clang::Sema::BuildCXXNamedCast(clang::SourceLocation, clang::tok::TokenKind, clang::TypeSourceInfo*, clang::Expr*, clang::SourceRange, clang::SourceRange) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x62832ae)
#11 0x0000000006c38fdf clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCXXNamedCastExpr(clang::CXXNamedCastExpr*) SemaTemplateInstantiate.cpp:0:0
#12 0x0000000006c03d78 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#13 0x0000000006c05be0 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformInitializer(clang::Expr*, bool) (.part.0) SemaTemplateInstantiate.cpp:0:0
#14 0x0000000006c06221 clang::Sema::SubstInitializer(clang::Expr*, clang::MultiLevelTemplateArgumentList const&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c06221)
#15 0x0000000006c4ecaf clang::Sema::InstantiateVariableInitializer(clang::VarDecl*, clang::VarDecl*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c4ecaf)
#16 0x0000000006c9052d clang::Sema::BuildVariableInstantiation(clang::VarDecl*, clang::VarDecl*, clang::MultiLevelTemplateArgumentList const&, llvm::SmallVector<clang::Sema::LateInstantiatedAttribute, 16u>*, clang::DeclContext*, clang::LocalInstantiationScope*, bool, clang::VarTemplateSpecializationDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c9052d)
#17 0x0000000006c99f2b clang::TemplateDeclInstantiator::VisitVarDecl(clang::VarDecl*, bool, llvm::ArrayRef<clang::BindingDecl*>*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c99f2b)
#18 0x0000000006c9ea74 void llvm::function_ref<void ()>::callback_fn<clang::Sema::SubstDecl(clang::Decl*, clang::DeclContext*, clang::MultiLevelTemplateArgumentList const&)::'lambda'()>(long) SemaTemplateInstantiateDecl.cpp:0:0
#19 0x00000000062274e1 clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x62274e1)
#20 0x0000000006c4cb8a clang::Sema::SubstDecl(clang::Decl*, clang::DeclContext*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c4cb8a)
#21 0x0000000006bfd585 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformDeclStmt(clang::DeclStmt*) SemaTemplateInstantiate.cpp:0:0
#22 0x0000000006c3c4a4 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaTemplateInstantiate.cpp:0:0
#23 0x0000000006c4296a clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c4296a)
#24 0x0000000006c93ff1 clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c93ff1)
#25 0x0000000006c9240f clang::Sema::PerformPendingInstantiations(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c9240f)
#26 0x000000000624a6a9 clang::Sema::ActOnEndOfTranslationUnitFragment(clang::Sema::TUFragmentKind) (.part.0) Sema.cpp:0:0
#27 0x000000000624ae82 clang::Sema::ActOnEndOfTranslationUnit() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x624ae82)
#28 0x00000000060d06d1 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x60d06d1)
#29 0x00000000060c341a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x60c341a)
#30 0x0000000004118c48 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4118c48)
#31 0x0000000004389719 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4389719)
#32 0x00000000043081ee clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x43081ee)
#33 0x000000000446940e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x446940e)
#34 0x0000000000c126b6 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc126b6)
#35 0x0000000000c09eca ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#36 0x000000000415a1f9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#37 0x00000000037d0794 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x37d0794)
#38 0x000000000415a7ef clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#39 0x00000000041224e5 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41224e5)
#40 0x0000000004122f4d clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4122f4d)
#41 0x000000000412ae85 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x412ae85)
#42 0x0000000000c0fa3c clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc0fa3c)
#43 0x0000000000b06be4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xb06be4)
#44 0x00007ffb19e29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#45 0x00007ffb19e29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#46 0x0000000000c099ae _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc099ae)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

---

### Comment 5 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (ogigato)

<details>
CLang version: 16.0.5, using MSVC-like command line. This project compiles ok using MSVC compiler, with the same arguments.

[SourcesAndTrace.zip](https://github.com/llvm/llvm-project/files/13498012/SourcesAndTrace.zip)


</details>


---

### Comment 6 - llvmbot


@llvm/issue-subscribers-c-20

Author: None (ogigato)

<details>
CLang version: 16.0.5, using MSVC-like command line. This project compiles ok using MSVC compiler, with the same arguments.

[SourcesAndTrace.zip](https://github.com/llvm/llvm-project/files/13498012/SourcesAndTrace.zip)


</details>


---

