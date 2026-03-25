# [clang] Crash on initialization

**Author:** cardigan1008
**URL:** https://github.com/llvm/llvm-project/issues/106180

## Body

I compile this code with a crash:

```c
a = {{}, 1};
```

Assertion:

```console
/root/llvm-project/clang/include/clang/AST/Expr.h:5091: clang::Expr* clang::InitListExpr::getInit(unsigned int):
Assertion `Init < getNumInits() && "Initializer access out of range!"' failed.
```

The details can be found here: https://godbolt.org/z/Yc9nvE5de

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Yunbo Ni (cardigan1008)

<details>
I compile this code with a crash:

```c
a = {{}, 1};
```

The crash is: clang: /root/llvm-project/clang/include/clang/AST/Expr.h:5091: clang::Expr* clang::InitListExpr::getInit(unsigned int): Assertion `Init &lt; getNumInits() &amp;&amp; "Initializer access out of range!"' failed.

The details can be found here: https://godbolt.org/z/Yc9nvE5de
</details>


---

### Comment 2 - shafik

https://godbolt.org/z/6soTscqd6

Backtrace:

```console
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<source>:1:12: current parser token ';'
 #0 0x0000000003b123c8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3b123c8)
 #1 0x0000000003b100bc llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3b100bc)
 #2 0x0000000003a58af8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x0000721f79842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x0000721f798969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x0000721f79842476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x0000721f798287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x0000721f7982871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x0000721f79839e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000006d1f3f4 clang::InitListExpr::getInit(unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6d1f3f4)
#10 0x0000000006d318a5 (anonymous namespace)::InitListChecker::CheckExplicitInitList(clang::InitializedEntity const&, clang::InitListExpr*, clang::QualType&, clang::InitListExpr*, bool) (.constprop.0) SemaInit.cpp:0:0
#11 0x0000000006d31bee (anonymous namespace)::InitListChecker::InitListChecker(clang::Sema&, clang::InitializedEntity const&, clang::InitListExpr*, clang::QualType&, bool, bool, bool, llvm::SmallVectorImpl<clang::QualType>*) SemaInit.cpp:0:0
#12 0x0000000006d33ccb clang::InitializationSequence::Perform(clang::Sema&, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::MutableArrayRef<clang::Expr*>, clang::QualType*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6d33ccb)
#13 0x00000000068dce51 clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x68dce51)
#14 0x000000000656b9da clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x656b9da)
#15 0x000000000657a22b clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x657a22b)
#16 0x000000000653992e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x653992e)
#17 0x000000000653a0de clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x653a0de)
#18 0x0000000006540727 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6540727)
#19 0x00000000065415fd clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x65415fd)
#20 0x0000000006541aa0 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6541aa0)
#21 0x0000000006534ab2 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6534ab2)
#22 0x000000000442a558 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x442a558)
#23 0x00000000046b9999 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x46b9999)
#24 0x000000000463cf8e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x463cf8e)
#25 0x00000000047a31de clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x47a31de)
#26 0x0000000000ca71af cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xca71af)
#27 0x0000000000ca03fa ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#28 0x000000000446d7f9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#29 0x0000000003a58fa4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3a58fa4)
#30 0x000000000446ddef clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#31 0x0000000004433da5 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4433da5)
#32 0x000000000443480d clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x443480d)
#33 0x000000000443c445 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x443c445)
#34 0x0000000000ca45a5 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xca45a5)
#35 0x0000000000b78974 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xb78974)
#36 0x0000721f79829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#37 0x0000721f79829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#38 0x0000000000c9feae _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xc9feae)
clang: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

---

