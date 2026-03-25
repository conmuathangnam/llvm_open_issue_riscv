# [Clang] Clang crash when applying __builtin_classify_type to a pointer-to-member of a function template

**Author:** Attacker23
**URL:** https://github.com/llvm/llvm-project/issues/175589

## Body

The following code causes crash on x86-64 clang since 6.0.0, but it doesn’t crash on 12–16:


### Minimal Reproduction

```cpp
struct C {
  template<class T>
  void f() {}
};

static_assert(__builtin_classify_type(&C::f));
```
Clang crashes when __builtin_classify_type is used on &C::f, where f is a member function template
Instead of rejecting the expression as ill-formed (as GCC/EDG do), Clang goes into constant evaluation of __builtin_classify_type’s argument and crashes while trying to treat &C::f as a valid pointer in a constant expression.

### Compiler Explorer

**https://godbolt.org/z/KasWqvsvd**

### Compiler Output

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<eof> parser at end of file
#0 0x0000000003d649e8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3d649e8)
#1 0x0000000003d623bc llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3d623bc)
#2 0x0000000003ca74b8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
#3 0x000072eb5a442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
#4 0x00000000074f9d98 clang::Expr::EvalResult::isGlobalLValue() const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x74f9d98)
#5 0x000000003c6b4a40
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```



## Comments

### Comment 1 - shafik

Assertions trunk is almost always the most useful point of reference for backtraces, assertions and other diagnostics: https://godbolt.org/z/jzWTx741z

Unreachable:

```console
unexpected placeholder type
UNREACHABLE executed at /root/llvm-project/llvm/tools/clang/lib/AST/ExprConstant.cpp:15378!
```

Backtrace:

```console
 #0 0x000000000430abe8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x430abe8)
 #1 0x0000000004308014 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4308014)
 #2 0x000000000424aa08 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x0000735f5b442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x0000735f5b4969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x0000735f5b442476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x0000735f5b4287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x000000000425557a (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x425557a)
 #8 0x0000000007d249b7 (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7d249b7)
 #9 0x0000000007dc452b (anonymous namespace)::IntExprEvaluator::VisitBuiltinCallExpr(clang::CallExpr const*, unsigned int) ExprConstant.cpp:0:0
#10 0x0000000007dc8e09 (anonymous namespace)::IntExprEvaluator::VisitCallExpr(clang::CallExpr const*) ExprConstant.cpp:0:0
#11 0x0000000007d70af8 clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#12 0x0000000007d4a5ae Evaluate(clang::APValue&, (anonymous namespace)::EvalInfo&, clang::Expr const*) ExprConstant.cpp:0:0
#13 0x0000000007d52f48 EvaluateAsBooleanCondition(clang::Expr const*, bool&, (anonymous namespace)::EvalInfo&) ExprConstant.cpp:0:0
#14 0x0000000007db4ec3 (anonymous namespace)::IntExprEvaluator::VisitCastExpr(clang::CastExpr const*) ExprConstant.cpp:0:0
#15 0x0000000007d70bf1 clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#16 0x0000000007d4a5ae Evaluate(clang::APValue&, (anonymous namespace)::EvalInfo&, clang::Expr const*) ExprConstant.cpp:0:0
#17 0x0000000007d4aeab EvaluateAsRValue((anonymous namespace)::EvalInfo&, clang::Expr const*, clang::APValue&) ExprConstant.cpp:0:0
#18 0x0000000007d50377 clang::Expr::EvaluateAsRValue(clang::Expr::EvalResult&, clang::ASTContext const&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7d50377)
#19 0x0000000006fb8421 clang::Sema::VerifyIntegerConstantExpression(clang::Expr*, llvm::APSInt*, clang::Sema::VerifyICEDiagnoser&, clang::AllowFoldKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6fb8421)
#20 0x0000000006fb8f62 clang::Sema::VerifyIntegerConstantExpression(clang::Expr*, llvm::APSInt*, unsigned int, clang::AllowFoldKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6fb8f62)
#21 0x0000000006ec9ec6 clang::Sema::BuildStaticAssertDeclaration(clang::SourceLocation, clang::Expr*, clang::Expr*, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ec9ec6)
#22 0x0000000006ab35e6 clang::Parser::ParseStaticAssertDeclaration(clang::SourceLocation&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ab35e6)
#23 0x0000000006aa1166 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6aa1166)
#24 0x0000000006a5dc2c clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a5dc2c)
#25 0x0000000006a5eb15 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a5eb15)
#26 0x0000000006a3fb9a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a3fb9a)
#27 0x0000000004c6daf8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c6daf8)
#28 0x0000000004f66ee5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f66ee5)
#29 0x0000000004ee6d1e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ee6d1e)
#30 0x000000000506158d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x506158d)
#31 0x0000000000de8a9e cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde8a9e)
#32 0x0000000000ddf46a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#33 0x0000000000ddf5ed int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#34 0x0000000004ce0b39 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#35 0x000000000424aea4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x424aea4)
#36 0x0000000004ce114f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#37 0x0000000004ca1ae2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ca1ae2)
#38 0x0000000004ca2a8e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ca2a8e)
#39 0x0000000004ca9ee5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ca9ee5)
#40 0x0000000000de4e11 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde4e11)
#41 0x0000000000c8f104 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc8f104)
#42 0x0000735f5b429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#43 0x0000735f5b429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#44 0x0000000000ddef05 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xddef05)
```

Maybe related: https://github.com/llvm/llvm-project/issues/111462

---

### Comment 2 - shafik

Is this fuzzing generate?

---

### Comment 3 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (Attacker23)

<details>
The following code causes crash on x86-64 clang since 6.0.0, but it doesn’t crash on 12–16:


### Minimal Reproduction

```cpp
struct C {
  template&lt;class T&gt;
  void f() {}
};

static_assert(__builtin_classify_type(&amp;C::f));
```
Clang crashes when __builtin_classify_type is used on &amp;C::f, where f is a member function template
Instead of rejecting the expression as ill-formed (as GCC/EDG do), Clang goes into constant evaluation of __builtin_classify_type’s argument and crashes while trying to treat &amp;C::f as a valid pointer in a constant expression.

### Compiler Explorer

**https://godbolt.org/z/KasWqvsvd**

### Compiler Output

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
#<!-- -->0 0x0000000003d649e8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3d649e8)
#<!-- -->1 0x0000000003d623bc llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3d623bc)
#<!-- -->2 0x0000000003ca74b8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
#<!-- -->3 0x000072eb5a442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
#<!-- -->4 0x00000000074f9d98 clang::Expr::EvalResult::isGlobalLValue() const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x74f9d98)
#<!-- -->5 0x000000003c6b4a40
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```


</details>


---

### Comment 4 - Attacker23

> Is this fuzzing generate?

@shafik It started as a fuzzing-generated input, and I minimized it to the current form. Got it about using the assertions trunk for backtraces, thank you for the explanation and the example link.

---

