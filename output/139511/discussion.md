# Clang++ ICE  mixed array initializer with .y[2] = 'e' and string literal since clang 12

**Author:** mariete1223
**URL:** https://github.com/llvm/llvm-project/issues/139511

## Body

The following minimal C code causes clang++ (LLVM trunk with assertions enabled) to crash with an assertion failure inside castAsArrayTypeUnsafe.

## Assertion

```
clang++: /root/llvm-project/llvm/tools/clang/include/clang/AST/Type.h:8843: const clang::ArrayType* clang::Type::castAsArrayTypeUnsafe() const: Assertion `isa<ArrayType>(CanonicalType)' failed.
```

## Stack dump 

```
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x c++ <source>
1.	<source>:4:25: current parser token ';'
 #0 0x0000000003f8a6d8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f8a6d8)
 #1 0x0000000003f88364 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f88364)
 #2 0x0000000003ecd158 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x0000777cf3e42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x0000777cf3e969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x0000777cf3e42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x0000777cf3e287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x0000777cf3e2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x0000777cf3e39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000007886daf CheckEvaluationResult(CheckEvaluationResultKind, (anonymous namespace)::EvalInfo&, clang::SourceLocation, clang::QualType, clang::APValue const&, clang::Expr::ConstantExprKind, clang::FieldDecl const*, llvm::SmallPtrSet<clang::MaterializeTemporaryExpr const*, 8u>&) ExprConstant.cpp:0:0
#10 0x000000000788684c CheckEvaluationResult(CheckEvaluationResultKind, (anonymous namespace)::EvalInfo&, clang::SourceLocation, clang::QualType, clang::APValue const&, clang::Expr::ConstantExprKind, clang::FieldDecl const*, llvm::SmallPtrSet<clang::MaterializeTemporaryExpr const*, 8u>&) ExprConstant.cpp:0:0
#11 0x0000000007886ad6 CheckEvaluationResult(CheckEvaluationResultKind, (anonymous namespace)::EvalInfo&, clang::SourceLocation, clang::QualType, clang::APValue const&, clang::Expr::ConstantExprKind, clang::FieldDecl const*, llvm::SmallPtrSet<clang::MaterializeTemporaryExpr const*, 8u>&) ExprConstant.cpp:0:0
#12 0x00000000078881f6 CheckConstantExpression((anonymous namespace)::EvalInfo&, clang::SourceLocation, clang::QualType, clang::APValue const&, clang::Expr::ConstantExprKind) ExprConstant.cpp:0:0
#13 0x00000000078fd8ee clang::Expr::EvaluateAsInitializer(clang::APValue&, clang::ASTContext const&, clang::VarDecl const*, llvm::SmallVectorImpl<std::pair<clang::SourceLocation, clang::PartialDiagnostic>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x78fd8ee)
#14 0x000000000771ad32 clang::VarDecl::evaluateValueImpl(llvm::SmallVectorImpl<std::pair<clang::SourceLocation, clang::PartialDiagnostic>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x771ad32)
#15 0x000000000771b0e1 clang::VarDecl::checkForConstantInitialization(llvm::SmallVectorImpl<std::pair<clang::SourceLocation, clang::PartialDiagnostic>>&) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x771b0e1)
#16 0x00000000069ba3eb clang::Sema::CheckCompleteVariableDeclaration(clang::VarDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69ba3eb)
#17 0x00000000069c1562 clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69c1562)
#18 0x0000000006642b6a clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6642b6a)
#19 0x00000000066523e9 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66523e9)
#20 0x000000000661029e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x661029e)
#21 0x0000000006610a59 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6610a59)
#22 0x000000000661837a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x661837a)
#23 0x000000000661931d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x661931d)
#24 0x000000000660b6fa clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x660b6fa)
#25 0x000000000492f0e8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x492f0e8)
#26 0x0000000004c25545 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c25545)
#27 0x0000000004ba41ce clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ba41ce)
#28 0x0000000004d192e9 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d192e9)
#29 0x0000000000da8b7f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda8b7f)
#30 0x0000000000d9ed4a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#31 0x0000000004996179 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#32 0x0000000003ecd5f4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3ecd5f4)
#33 0x000000000499678f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#34 0x000000000495871d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x495871d)
#35 0x00000000049597ae clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49597ae)
#36 0x0000000004961a95 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4961a95)
#37 0x0000000000da4ae8 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda4ae8)
#38 0x0000000000c2a0f4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc2a0f4)
#39 0x0000777cf3e29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#40 0x0000777cf3e29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#41 0x0000000000d9e7f5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd9e7f5)
```

## Program

```
#include <stdio.h>
const struct {
        char y[4];
}a = {.y[2] = 'e',"aaa"};
```

## To reproduce

It occurs since clang 12

https://gcc.godbolt.org/z/Gh3an9P3n

## Comments

### Comment 1 - frederick-vs-ja

This also causes crash in C modes ([Godbolt link](https://gcc.godbolt.org/z/vWq9PWM6e)), but for seemingly different reasons.

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (mariete1223)

<details>
The following minimal C code causes clang++ (LLVM trunk with assertions enabled) to crash with an assertion failure inside castAsArrayTypeUnsafe.

## Assertion

```
clang++: /root/llvm-project/llvm/tools/clang/include/clang/AST/Type.h:8843: const clang::ArrayType* clang::Type::castAsArrayTypeUnsafe() const: Assertion `isa&lt;ArrayType&gt;(CanonicalType)' failed.
```

## Stack dump 

```
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x c++ &lt;source&gt;
1.	&lt;source&gt;:4:25: current parser token ';'
 #<!-- -->0 0x0000000003f8a6d8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f8a6d8)
 #<!-- -->1 0x0000000003f88364 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f88364)
 #<!-- -->2 0x0000000003ecd158 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x0000777cf3e42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x0000777cf3e969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x0000777cf3e42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x0000777cf3e287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x0000777cf3e2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x0000777cf3e39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000007886daf CheckEvaluationResult(CheckEvaluationResultKind, (anonymous namespace)::EvalInfo&amp;, clang::SourceLocation, clang::QualType, clang::APValue const&amp;, clang::Expr::ConstantExprKind, clang::FieldDecl const*, llvm::SmallPtrSet&lt;clang::MaterializeTemporaryExpr const*, 8u&gt;&amp;) ExprConstant.cpp:0:0
#<!-- -->10 0x000000000788684c CheckEvaluationResult(CheckEvaluationResultKind, (anonymous namespace)::EvalInfo&amp;, clang::SourceLocation, clang::QualType, clang::APValue const&amp;, clang::Expr::ConstantExprKind, clang::FieldDecl const*, llvm::SmallPtrSet&lt;clang::MaterializeTemporaryExpr const*, 8u&gt;&amp;) ExprConstant.cpp:0:0
#<!-- -->11 0x0000000007886ad6 CheckEvaluationResult(CheckEvaluationResultKind, (anonymous namespace)::EvalInfo&amp;, clang::SourceLocation, clang::QualType, clang::APValue const&amp;, clang::Expr::ConstantExprKind, clang::FieldDecl const*, llvm::SmallPtrSet&lt;clang::MaterializeTemporaryExpr const*, 8u&gt;&amp;) ExprConstant.cpp:0:0
#<!-- -->12 0x00000000078881f6 CheckConstantExpression((anonymous namespace)::EvalInfo&amp;, clang::SourceLocation, clang::QualType, clang::APValue const&amp;, clang::Expr::ConstantExprKind) ExprConstant.cpp:0:0
#<!-- -->13 0x00000000078fd8ee clang::Expr::EvaluateAsInitializer(clang::APValue&amp;, clang::ASTContext const&amp;, clang::VarDecl const*, llvm::SmallVectorImpl&lt;std::pair&lt;clang::SourceLocation, clang::PartialDiagnostic&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x78fd8ee)
#<!-- -->14 0x000000000771ad32 clang::VarDecl::evaluateValueImpl(llvm::SmallVectorImpl&lt;std::pair&lt;clang::SourceLocation, clang::PartialDiagnostic&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x771ad32)
#<!-- -->15 0x000000000771b0e1 clang::VarDecl::checkForConstantInitialization(llvm::SmallVectorImpl&lt;std::pair&lt;clang::SourceLocation, clang::PartialDiagnostic&gt;&gt;&amp;) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x771b0e1)
#<!-- -->16 0x00000000069ba3eb clang::Sema::CheckCompleteVariableDeclaration(clang::VarDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69ba3eb)
#<!-- -->17 0x00000000069c1562 clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69c1562)
#<!-- -->18 0x0000000006642b6a clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6642b6a)
#<!-- -->19 0x00000000066523e9 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66523e9)
#<!-- -->20 0x000000000661029e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x661029e)
#<!-- -->21 0x0000000006610a59 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6610a59)
#<!-- -->22 0x000000000661837a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x661837a)
#<!-- -->23 0x000000000661931d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x661931d)
#<!-- -->24 0x000000000660b6fa clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x660b6fa)
#<!-- -->25 0x000000000492f0e8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x492f0e8)
#<!-- -->26 0x0000000004c25545 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c25545)
#<!-- -->27 0x0000000004ba41ce clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ba41ce)
#<!-- -->28 0x0000000004d192e9 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d192e9)
#<!-- -->29 0x0000000000da8b7f cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda8b7f)
#<!-- -->30 0x0000000000d9ed4a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->31 0x0000000004996179 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->32 0x0000000003ecd5f4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3ecd5f4)
#<!-- -->33 0x000000000499678f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->34 0x000000000495871d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x495871d)
#<!-- -->35 0x00000000049597ae clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49597ae)
#<!-- -->36 0x0000000004961a95 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4961a95)
#<!-- -->37 0x0000000000da4ae8 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda4ae8)
#<!-- -->38 0x0000000000c2a0f4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc2a0f4)
#<!-- -->39 0x0000777cf3e29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->40 0x0000777cf3e29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->41 0x0000000000d9e7f5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd9e7f5)
```

## Program

```
#include &lt;stdio.h&gt;
const struct {
        char y[4];
}a = {.y[2] = 'e',"aaa"};
```

## To reproduce

It occurs since clang 12

https://gcc.godbolt.org/z/Gh3an9P3n
</details>


---

### Comment 3 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: None (mariete1223)

<details>
The following minimal C code causes clang++ (LLVM trunk with assertions enabled) to crash with an assertion failure inside castAsArrayTypeUnsafe.

## Assertion

```
clang++: /root/llvm-project/llvm/tools/clang/include/clang/AST/Type.h:8843: const clang::ArrayType* clang::Type::castAsArrayTypeUnsafe() const: Assertion `isa&lt;ArrayType&gt;(CanonicalType)' failed.
```

## Stack dump 

```
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x c++ &lt;source&gt;
1.	&lt;source&gt;:4:25: current parser token ';'
 #<!-- -->0 0x0000000003f8a6d8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f8a6d8)
 #<!-- -->1 0x0000000003f88364 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f88364)
 #<!-- -->2 0x0000000003ecd158 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x0000777cf3e42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x0000777cf3e969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x0000777cf3e42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x0000777cf3e287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x0000777cf3e2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x0000777cf3e39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000007886daf CheckEvaluationResult(CheckEvaluationResultKind, (anonymous namespace)::EvalInfo&amp;, clang::SourceLocation, clang::QualType, clang::APValue const&amp;, clang::Expr::ConstantExprKind, clang::FieldDecl const*, llvm::SmallPtrSet&lt;clang::MaterializeTemporaryExpr const*, 8u&gt;&amp;) ExprConstant.cpp:0:0
#<!-- -->10 0x000000000788684c CheckEvaluationResult(CheckEvaluationResultKind, (anonymous namespace)::EvalInfo&amp;, clang::SourceLocation, clang::QualType, clang::APValue const&amp;, clang::Expr::ConstantExprKind, clang::FieldDecl const*, llvm::SmallPtrSet&lt;clang::MaterializeTemporaryExpr const*, 8u&gt;&amp;) ExprConstant.cpp:0:0
#<!-- -->11 0x0000000007886ad6 CheckEvaluationResult(CheckEvaluationResultKind, (anonymous namespace)::EvalInfo&amp;, clang::SourceLocation, clang::QualType, clang::APValue const&amp;, clang::Expr::ConstantExprKind, clang::FieldDecl const*, llvm::SmallPtrSet&lt;clang::MaterializeTemporaryExpr const*, 8u&gt;&amp;) ExprConstant.cpp:0:0
#<!-- -->12 0x00000000078881f6 CheckConstantExpression((anonymous namespace)::EvalInfo&amp;, clang::SourceLocation, clang::QualType, clang::APValue const&amp;, clang::Expr::ConstantExprKind) ExprConstant.cpp:0:0
#<!-- -->13 0x00000000078fd8ee clang::Expr::EvaluateAsInitializer(clang::APValue&amp;, clang::ASTContext const&amp;, clang::VarDecl const*, llvm::SmallVectorImpl&lt;std::pair&lt;clang::SourceLocation, clang::PartialDiagnostic&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x78fd8ee)
#<!-- -->14 0x000000000771ad32 clang::VarDecl::evaluateValueImpl(llvm::SmallVectorImpl&lt;std::pair&lt;clang::SourceLocation, clang::PartialDiagnostic&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x771ad32)
#<!-- -->15 0x000000000771b0e1 clang::VarDecl::checkForConstantInitialization(llvm::SmallVectorImpl&lt;std::pair&lt;clang::SourceLocation, clang::PartialDiagnostic&gt;&gt;&amp;) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x771b0e1)
#<!-- -->16 0x00000000069ba3eb clang::Sema::CheckCompleteVariableDeclaration(clang::VarDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69ba3eb)
#<!-- -->17 0x00000000069c1562 clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69c1562)
#<!-- -->18 0x0000000006642b6a clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6642b6a)
#<!-- -->19 0x00000000066523e9 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66523e9)
#<!-- -->20 0x000000000661029e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x661029e)
#<!-- -->21 0x0000000006610a59 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6610a59)
#<!-- -->22 0x000000000661837a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x661837a)
#<!-- -->23 0x000000000661931d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x661931d)
#<!-- -->24 0x000000000660b6fa clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x660b6fa)
#<!-- -->25 0x000000000492f0e8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x492f0e8)
#<!-- -->26 0x0000000004c25545 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c25545)
#<!-- -->27 0x0000000004ba41ce clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ba41ce)
#<!-- -->28 0x0000000004d192e9 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d192e9)
#<!-- -->29 0x0000000000da8b7f cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda8b7f)
#<!-- -->30 0x0000000000d9ed4a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->31 0x0000000004996179 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->32 0x0000000003ecd5f4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3ecd5f4)
#<!-- -->33 0x000000000499678f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->34 0x000000000495871d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x495871d)
#<!-- -->35 0x00000000049597ae clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49597ae)
#<!-- -->36 0x0000000004961a95 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4961a95)
#<!-- -->37 0x0000000000da4ae8 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda4ae8)
#<!-- -->38 0x0000000000c2a0f4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc2a0f4)
#<!-- -->39 0x0000777cf3e29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->40 0x0000777cf3e29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->41 0x0000000000d9e7f5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd9e7f5)
```

## Program

```
#include &lt;stdio.h&gt;
const struct {
        char y[4];
}a = {.y[2] = 'e',"aaa"};
```

## To reproduce

It occurs since clang 12

https://gcc.godbolt.org/z/Gh3an9P3n
</details>


---

### Comment 4 - Mr-Anyone

Interestingly, Clang rejects both of these initializations, while GCC accepts the first one. 
```c
char a[10]  = {[2] = 'e', "aaa"};   // gcc accepts this char a = "aaa"
char b[10]  =  {[0] = 'e',  "aaa"}; // gcc rejects this
```
https://gcc.godbolt.org/z/evr3Wdjzo

---

