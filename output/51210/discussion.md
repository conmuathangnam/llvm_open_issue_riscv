# Clang crashes with broken program containing _Complex int and fixed-point literal

**Author:** 009b8ea0-6754-41bc-becf-d830f82bdd65
**URL:** https://github.com/llvm/llvm-project/issues/51210

## Body

|  |  |
| --- | --- |
| Bugzilla Link | [51868](https://llvm.org/bz51868) |
| Version | trunk |
| OS | Linux |
| CC | @zygoloid |

## Extended Description 
Commit: d261d4cf559d96eb4e45d8699df2557e2998fc88

Compiling the following broken program:

```cpp
f() { for (_Complex int a_1 0.5r / a_2
```

with:
```console
$ clang -ffixed-point foo.c
```
gives:
```
foo.c:1:1: warning: type specifier missing, defaults to 'int' [-Wimplicit-int]
f() { for (_Complex int a_1 0.5r / a_2
^
foo.c:1:29: error: expected ';' in 'for' statement specifier
f() { for (_Complex int a_1 0.5r / a_2
                            ^
getIntegerRank(): not a built-in integer
UNREACHABLE executed at /clang/lib/AST/ASTContext.cpp:6402!
PLEASE submit a bug report to https://bugs.llvm.org/ and include the crash backtrace, preprocessed source, and associated run script.
```
If e.g. switching to a floating-point literal the crash is not seen.

## Comments

### Comment 1 - Endilll

Still crashing as of post-17 trunk: https://godbolt.org/z/c5e9Wdza7
```
<source>:1:29: error: expected ';' in 'for' statement specifier
    1 | f() { for (_Complex int a_1 0.5r / a_2
      |                             ^

getIntegerRank(): not a built-in integer
UNREACHABLE executed at /root/llvm-project/clang/lib/AST/ASTContext.cpp:7091!

PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x c -Wno-implicit-int -Wno-implicit-function-declaration -ffixed-point <source>
1.	<eof> parser at end of file
2.	<source>:1:5: parsing function body 'f'
3.	<source>:1:5: in compound statement ('{}')
 #0 0x00000000036afc28 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x36afc28)
 #1 0x00000000036ad8ec llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x36ad8ec)
 #2 0x00000000035f6d08 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007fe0ed65c420 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x14420)
 #4 0x00007fe0ed11f00b raise (/lib/x86_64-linux-gnu/libc.so.6+0x4300b)
 #5 0x00007fe0ed0fe859 abort (/lib/x86_64-linux-gnu/libc.so.6+0x22859)
 #6 0x0000000003601fba (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3601fba)
 #7 0x0000000006cdc370 clang::ASTContext::getIntegerTypeOrder(clang::QualType, clang::QualType) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6cdc370)
 #8 0x0000000006cdc496 clang::ASTContext::getIntegerTypeOrder(clang::QualType, clang::QualType) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6cdc496)
 #9 0x0000000006395495 clang::Sema::UsualArithmeticConversions(clang::ActionResult<clang::Expr*, true>&, clang::ActionResult<clang::Expr*, true>&, clang::SourceLocation, clang::Sema::ArithConvKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6395495)
#10 0x000000000639e287 clang::Sema::CheckMultiplyDivideOperands(clang::ActionResult<clang::Expr*, true>&, clang::ActionResult<clang::Expr*, true>&, clang::SourceLocation, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x639e287)
#11 0x00000000063a5256 clang::Sema::CreateBuiltinBinOp(clang::SourceLocation, clang::BinaryOperatorKind, clang::Expr*, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x63a5256)
#12 0x00000000063a69dc clang::Sema::BuildBinOp(clang::Scope*, clang::SourceLocation, clang::BinaryOperatorKind, clang::Expr*, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x63a69dc)
#13 0x00000000064bd9be clang::TreeTransform<(anonymous namespace)::TransformTypos>::TransformBinaryOperator(clang::BinaryOperator*) SemaExprCXX.cpp:0:0
#14 0x00000000064a83f2 (anonymous namespace)::TransformTypos::RecursiveTransformLoop(clang::Expr*, bool&) SemaExprCXX.cpp:0:0
#15 0x00000000064aa46d clang::Sema::CorrectDelayedTyposInExpr(clang::Expr*, clang::VarDecl*, bool, llvm::function_ref<clang::ActionResult<clang::Expr*, true> (clang::Expr*)>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64aa46d)
#16 0x00000000064ab9a5 clang::Sema::ActOnFinishFullExpr(clang::Expr*, clang::SourceLocation, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64ab9a5)
#17 0x00000000063a327e clang::Sema::ActOnCondition(clang::Scope*, clang::SourceLocation, clang::Expr*, clang::Sema::ConditionKind, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x63a327e)
#18 0x0000000005ee8ecb clang::Parser::ParseForStatement(clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5ee8ecb)
#19 0x0000000005ee3b65 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5ee3b65)
#20 0x0000000005ee5128 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5ee5128)
#21 0x0000000005ee6069 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5ee6069)
#22 0x0000000005ee798a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5ee798a)
#23 0x0000000005e15151 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5e15151)
#24 0x0000000005e3c748 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5e3c748)
#25 0x0000000005e0906b clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5e0906b)
#26 0x0000000005e0979f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (.part.0) Parser.cpp:0:0
#27 0x0000000005e0feb4 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5e0feb4)
#28 0x0000000005e106bd clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5e106bd)
#29 0x0000000005e10af0 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5e10af0)
#30 0x0000000005e049e2 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5e049e2)
#31 0x000000000490f578 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x490f578)
#32 0x0000000004177f89 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4177f89)
#33 0x00000000040f818e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x40f818e)
#34 0x00000000042561fe clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42561fe)
#35 0x0000000000bd6ea0 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xbd6ea0)
#36 0x0000000000bcf0ca ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#37 0x0000000003f59309 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#38 0x00000000035f71b4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x35f71b4)
#39 0x0000000003f598ff clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#40 0x0000000003f21c85 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f21c85)
#41 0x0000000003f226ed clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f226ed)
#42 0x0000000003f2a615 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f2a615)
#43 0x0000000000bd4cac clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xbd4cac)
#44 0x0000000000ad14a1 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xad14a1)
#45 0x00007fe0ed100083 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x24083)
#46 0x0000000000bcebae _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xbcebae)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
```

---

### Comment 2 - llvmbot

@llvm/issue-subscribers-c

---

### Comment 3 - llvmbot

@llvm/issue-subscribers-clang-frontend

---

### Comment 4 - AaronBallman

Note, the crash is specific to the typo correction to `a_1`: https://godbolt.org/z/zGzcG91bP

---

### Comment 5 - ecnelises

A much clearer reproducer:

```c
// compile with -ffixed-point
_Complex int b;
_Complex int a = 0.5r / b;
```

Similar backtrace:
```
0  clang-18                 0x0000000106e20e18 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) + 56
1  clang-18                 0x0000000106e1ef38 llvm::sys::RunSignalHandlers() + 112
2  clang-18                 0x0000000106e203e8 llvm::sys::CleanupOnSignal(unsigned long) + 232
3  clang-18                 0x0000000106d79400 CrashRecoverySignalHandler(int) + 156
4  libsystem_platform.dylib 0x0000000181271a24 _sigtramp + 56
5  libsystem_pthread.dylib  0x0000000181242cc0 pthread_kill + 288
6  libsystem_c.dylib        0x0000000181152a50 abort + 180
7  clang-18                 0x0000000106d83ccc llvm::install_out_of_memory_new_handler() + 0
8  clang-18                 0x0000000109495994 clang::ASTContext::getIntegerRank(clang::Type const*) const + 924
9  clang-18                 0x000000010949618c clang::ASTContext::getIntegerTypeOrder(clang::QualType, clang::QualType) const + 152
10 clang-18                 0x0000000108d85790 handleComplexIntConversion(clang::Sema&, clang::ActionResult<clang::Expr*, true>&, clang::ActionResult<clang::Expr*, true>&, clang::QualType, clang::QualType, bool) + 720
11 clang-18                 0x0000000108d8500c clang::Sema::UsualArithmeticConversions(clang::ActionResult<clang::Expr*, true>&, clang::ActionResult<clang::Expr*, true>&, clang::SourceLocation, clang::Sema::ArithConvKind) + 1488
```

BTW,

```c
// compile with -ffixed-point
_Complex int a = 0.5r; // This fails in trunk, but Clang 17 is okay
```

---

### Comment 6 - AaronBallman

> A much clearer reproducer:
> 
> // compile with -ffixed-point
> _Complex int b;
> _Complex int a = 0.5r / b;
> 
> Similar backtrace:
> 
> ```
> 0  clang-18                 0x0000000106e20e18 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) + 56
> 1  clang-18                 0x0000000106e1ef38 llvm::sys::RunSignalHandlers() + 112
> 2  clang-18                 0x0000000106e203e8 llvm::sys::CleanupOnSignal(unsigned long) + 232
> 3  clang-18                 0x0000000106d79400 CrashRecoverySignalHandler(int) + 156
> 4  libsystem_platform.dylib 0x0000000181271a24 _sigtramp + 56
> 5  libsystem_pthread.dylib  0x0000000181242cc0 pthread_kill + 288
> 6  libsystem_c.dylib        0x0000000181152a50 abort + 180
> 7  clang-18                 0x0000000106d83ccc llvm::install_out_of_memory_new_handler() + 0
> 8  clang-18                 0x0000000109495994 clang::ASTContext::getIntegerRank(clang::Type const*) const + 924
> 9  clang-18                 0x000000010949618c clang::ASTContext::getIntegerTypeOrder(clang::QualType, clang::QualType) const + 152
> 10 clang-18                 0x0000000108d85790 handleComplexIntConversion(clang::Sema&, clang::ActionResult<clang::Expr*, true>&, clang::ActionResult<clang::Expr*, true>&, clang::QualType, clang::QualType, bool) + 720
> 11 clang-18                 0x0000000108d8500c clang::Sema::UsualArithmeticConversions(clang::ActionResult<clang::Expr*, true>&, clang::ActionResult<clang::Expr*, true>&, clang::SourceLocation, clang::Sema::ArithConvKind) + 1488
> ```
> 
> BTW,
> 
> // compile with -ffixed-point
> _Complex int a = 0.5r; // This fails in trunk, but Clang 17 is okay

This is actually a different bug, I think. This stack trace does not have any typo correction involved.

---

### Comment 7 - AaronBallman

An even smaller reproducer for the typo correction related crash is:
```
int main(void) {
  _Complex int a_1;
   0.5r / a_2;
}
```

---

