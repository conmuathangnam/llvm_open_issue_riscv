# [RISCV] Crash in `detectVLSCCEligibleStruct`

**Author:** michalt
**URL:** https://github.com/llvm/llvm-project/issues/129995
**Status:** Closed
**Labels:** backend:RISC-V, clang:codegen, crash-on-valid
**Closed Date:** 2025-03-24T06:18:12Z

## Body

We found a crash when compiling internal code; creduced repro:

```
> cat galois_field-d4307d.cpp
struct a {
  using b = char __attribute__((vector_size(sizeof(char))));
};
class c {
  using d = a::b;
  d e;

public:
  static c f();
};
class g {
public:
  template <class h> g(h);
  friend g operator^(g, g) { c::f; }
  friend g operator^=(g i, g j) { i ^ j; }
};
template <typename, int> using k = g;
template <typename l> using m = k<l, sizeof(l)>;
void n() {
  void o();
  m<char> p = o ^= p;
}
```

Results in:

```
clang "-cc1" "-triple" "riscv64-unknown-linux-musl" "-emit-obj" "-target-cpu" "generic-rv64" "-target-feature" "+m" "-target-feature" "+a" "-target-feature" "+f" "-target-feature" "+d" "-target-feature" "+c" "-target-feature" "+v" "-target-feature" "-b" "-target-feature" "-e" "-target-feature" "-h" "-target-abi" "lp64d" "-O1" "-std=gnu++20" "-x" "c++" "galois_field-d4307d.cpp"
galois_field-d4307d.cpp:14:30: warning: expression result unused [-Wunused-value]
   14 |   friend g operator^(g, g) { c::f; }
      |                              ^~~~
galois_field-d4307d.cpp:14:36: warning: non-void function does not return a value [-Wreturn-type]
   14 |   friend g operator^(g, g) { c::f; }
      |                                    ^
galois_field-d4307d.cpp:15:42: warning: non-void function does not return a value [-Wreturn-type]
   15 |   friend g operator^=(g i, g j) { i ^ j; }
      |                                          ^
galois_field-d4307d.cpp:21:20: warning: variable 'p' is uninitialized when used within its own initialization [-Wuninitialized]
   21 |   m<char> p = o ^= p;
      |           ~        ^
F0000 00:00:1741242680.668504 1430842 logging.cc:62] assert.h assertion failed at llvm/include/llvm/Support/MathExtras.h:405 in T llvm::divideCeil(U, V) [U = unsigned int, V = unsigned int, T = unsigned int]: Denominator && "Division by zero"
*** Check failure stack trace: ***
    @     0x5610c7648dd9  absl::log_internal::LogMessage::PrepareToDie()
    @     0x5610c7648377  absl::log_internal::LogMessage::SendToLog()
    @     0x5610c764832b  absl::log_internal::LogMessage::Flush()
    @     0x5610bcc4cb45  absl::log_internal::Voidify::operator&&<>()
    @     0x5610c7567eaa  __assert_fail
    @     0x5610b7bc02c1  llvm::divideCeil<>()
    @     0x5610b8f571de  (anonymous namespace)::RISCVABIInfo::detectVLSCCEligibleStruct()
    @     0x5610b8f55fe4  (anonymous namespace)::RISCVABIInfo::classifyArgumentType()
    @     0x5610b8f559b6  (anonymous namespace)::RISCVABIInfo::classifyReturnType()
    @     0x5610b8f54d81  (anonymous namespace)::RISCVABIInfo::computeInfo()
    @     0x5610b7b3d914  clang::CodeGen::CodeGenTypes::arrangeLLVMFunctionInfo()
    @     0x5610b7b3df6d  arrangeLLVMFunctionInfo()
    @     0x5610b7b3dd3e  clang::CodeGen::CodeGenTypes::arrangeFreeFunctionType()
    @     0x5610b8d5f20d  clang::CodeGen::CodeGenTypes::ConvertFunctionTypeInternal()
    @     0x5610b8d5d789  clang::CodeGen::CodeGenTypes::ConvertType()
    @     0x5610b8a44f61  clang::CodeGen::CodeGenModule::GetAddrOfFunction()
    @     0x5610b7ce4ed5  clang::CodeGen::CodeGenModule::getRawFunctionPointer()
    @     0x5610b7c39cfb  clang::CodeGen::CodeGenModule::getFunctionPointer()
    @     0x5610b7ce6b76  EmitFunctionDeclLValue()
    @     0x5610b7cd3ca9  clang::CodeGen::CodeGenFunction::EmitDeclRefLValue()
    @     0x5610b7cd02c6  clang::CodeGen::CodeGenFunction::EmitLValueHelper()
    @     0x5610b7d054ab  clang::CodeGen::CodeGenFunction::EmitLValue()::$_0::operator()()
    @     0x5610b7d05428  llvm::function_ref<>::callback_fn<>()
    @     0x5610ba1c919c  llvm::function_ref<>::operator()()
    @     0x5610be2b4133  clang::runWithSufficientStackSpace()
    @     0x5610be2b3f33  clang::StackExhaustionHandler::runWithSufficientStackSpace()
    @     0x5610b8a24a3d  clang::CodeGen::CodeGenModule::runWithSufficientStackSpace()
    @     0x5610b7cbe553  clang::CodeGen::CodeGenFunction::EmitLValue()
    @     0x5610b7cbdf8d  clang::CodeGen::CodeGenFunction::EmitIgnoredExpr()
    @     0x5610b80543f2  clang::CodeGen::CodeGenFunction::EmitStmt()
    @     0x5610b8062bf4  clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope()
    @     0x5610b89ef9b4  clang::CodeGen::CodeGenFunction::EmitFunctionBody()
    @     0x5610b89f0fd8  clang::CodeGen::CodeGenFunction::GenerateCode()
    @     0x5610b8a41acc  clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition()
    @     0x5610b8a319e1  clang::CodeGen::CodeGenModule::EmitGlobalDefinition()
    @     0x5610b8a1fa13  clang::CodeGen::CodeGenModule::EmitDeferred()
    @     0x5610b8a1fa83  clang::CodeGen::CodeGenModule::EmitDeferred()
    @     0x5610b8a18e1d  clang::CodeGen::CodeGenModule::Release()
    @     0x5610b8e291f5  (anonymous namespace)::CodeGeneratorImpl::HandleTranslationUnit()
    @     0x5610b7b19135  clang::BackendConsumer::HandleTranslationUnit()
    @     0x5610ba606281  clang::ParseAST()
    @     0x5610b9fec3e3  clang::ASTFrontendAction::ExecuteAction()
    @     0x5610b7b1f40d  clang::CodeGenAction::ExecuteAction()
    @     0x5610b9feb9eb  clang::FrontendAction::Execute()
    @     0x5610b9e9d3cb  clang::CompilerInstance::ExecuteAction()
    @     0x5610b7b0e405  clang::ExecuteCompilerInvocation()
    @     0x5610b7add5c6  cc1_main()
    @     0x5610b7acaf66  ExecuteCC1Tool()
    @     0x5610b7ac9773  clang_main()
    @     0x5610b7ac91cb  main
    @     0x7ff9e62753d4  __libc_start_main
    @     0x5610b7ac90aa  _start
PLEASE submit a bug report to http://go/clang-crash-bug and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: clang -cc1 -triple riscv64-unknown-linux-musl -emit-obj -target-cpu generic-rv64 -target-feature +m -target-feature +a -target-feature +f -target-feature +d -target-feature +c -target-feature +v -target-feature -b -target-feature -e -target-feature -h -target-abi lp64d -O1 -std=gnu++20 -x c++ galois_field-d4307d.cpp
1.      <eof> parser at end of file
2.      Per-file LLVM IR generation
3.      galois_field-d4307d.cpp:14:12: Generating code for declaration 'operator^'
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-risc-v

Author: Michal Terepeta (michalt)

<details>
We found a crash when compiling internal code; creduced repro:

```
&gt; cat galois_field-d4307d.cpp
struct a {
  using b = char __attribute__((vector_size(sizeof(char))));
};
class c {
  using d = a::b;
  d e;

public:
  static c f();
};
class g {
public:
  template &lt;class h&gt; g(h);
  friend g operator^(g, g) { c::f; }
  friend g operator^=(g i, g j) { i ^ j; }
};
template &lt;typename, int&gt; using k = g;
template &lt;typename l&gt; using m = k&lt;l, sizeof(l)&gt;;
void n() {
  void o();
  m&lt;char&gt; p = o ^= p;
}
```

Results in:

```
clang "-cc1" "-triple" "riscv64-unknown-linux-musl" "-emit-obj" "-target-cpu" "generic-rv64" "-target-feature" "+m" "-target-feature" "+a" "-target-feature" "+f" "-target-feature" "+d" "-target-feature" "+c" "-target-feature" "+v" "-target-feature" "-b" "-target-feature" "-e" "-target-feature" "-h" "-target-abi" "lp64d" "-O1" "-std=gnu++20" "-x" "c++" "galois_field-d4307d.cpp"
galois_field-d4307d.cpp:14:30: warning: expression result unused [-Wunused-value]
   14 |   friend g operator^(g, g) { c::f; }
      |                              ^~~~
galois_field-d4307d.cpp:14:36: warning: non-void function does not return a value [-Wreturn-type]
   14 |   friend g operator^(g, g) { c::f; }
      |                                    ^
galois_field-d4307d.cpp:15:42: warning: non-void function does not return a value [-Wreturn-type]
   15 |   friend g operator^=(g i, g j) { i ^ j; }
      |                                          ^
galois_field-d4307d.cpp:21:20: warning: variable 'p' is uninitialized when used within its own initialization [-Wuninitialized]
   21 |   m&lt;char&gt; p = o ^= p;
      |           ~        ^
F0000 00:00:1741242680.668504 1430842 logging.cc:62] assert.h assertion failed at llvm/include/llvm/Support/MathExtras.h:405 in T llvm::divideCeil(U, V) [U = unsigned int, V = unsigned int, T = unsigned int]: Denominator &amp;&amp; "Division by zero"
*** Check failure stack trace: ***
    @     0x5610c7648dd9  absl::log_internal::LogMessage::PrepareToDie()
    @     0x5610c7648377  absl::log_internal::LogMessage::SendToLog()
    @     0x5610c764832b  absl::log_internal::LogMessage::Flush()
    @     0x5610bcc4cb45  absl::log_internal::Voidify::operator&amp;&amp;&lt;&gt;()
    @     0x5610c7567eaa  __assert_fail
    @     0x5610b7bc02c1  llvm::divideCeil&lt;&gt;()
    @     0x5610b8f571de  (anonymous namespace)::RISCVABIInfo::detectVLSCCEligibleStruct()
    @     0x5610b8f55fe4  (anonymous namespace)::RISCVABIInfo::classifyArgumentType()
    @     0x5610b8f559b6  (anonymous namespace)::RISCVABIInfo::classifyReturnType()
    @     0x5610b8f54d81  (anonymous namespace)::RISCVABIInfo::computeInfo()
    @     0x5610b7b3d914  clang::CodeGen::CodeGenTypes::arrangeLLVMFunctionInfo()
    @     0x5610b7b3df6d  arrangeLLVMFunctionInfo()
    @     0x5610b7b3dd3e  clang::CodeGen::CodeGenTypes::arrangeFreeFunctionType()
    @     0x5610b8d5f20d  clang::CodeGen::CodeGenTypes::ConvertFunctionTypeInternal()
    @     0x5610b8d5d789  clang::CodeGen::CodeGenTypes::ConvertType()
    @     0x5610b8a44f61  clang::CodeGen::CodeGenModule::GetAddrOfFunction()
    @     0x5610b7ce4ed5  clang::CodeGen::CodeGenModule::getRawFunctionPointer()
    @     0x5610b7c39cfb  clang::CodeGen::CodeGenModule::getFunctionPointer()
    @     0x5610b7ce6b76  EmitFunctionDeclLValue()
    @     0x5610b7cd3ca9  clang::CodeGen::CodeGenFunction::EmitDeclRefLValue()
    @     0x5610b7cd02c6  clang::CodeGen::CodeGenFunction::EmitLValueHelper()
    @     0x5610b7d054ab  clang::CodeGen::CodeGenFunction::EmitLValue()::$_0::operator()()
    @     0x5610b7d05428  llvm::function_ref&lt;&gt;::callback_fn&lt;&gt;()
    @     0x5610ba1c919c  llvm::function_ref&lt;&gt;::operator()()
    @     0x5610be2b4133  clang::runWithSufficientStackSpace()
    @     0x5610be2b3f33  clang::StackExhaustionHandler::runWithSufficientStackSpace()
    @     0x5610b8a24a3d  clang::CodeGen::CodeGenModule::runWithSufficientStackSpace()
    @     0x5610b7cbe553  clang::CodeGen::CodeGenFunction::EmitLValue()
    @     0x5610b7cbdf8d  clang::CodeGen::CodeGenFunction::EmitIgnoredExpr()
    @     0x5610b80543f2  clang::CodeGen::CodeGenFunction::EmitStmt()
    @     0x5610b8062bf4  clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope()
    @     0x5610b89ef9b4  clang::CodeGen::CodeGenFunction::EmitFunctionBody()
    @     0x5610b89f0fd8  clang::CodeGen::CodeGenFunction::GenerateCode()
    @     0x5610b8a41acc  clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition()
    @     0x5610b8a319e1  clang::CodeGen::CodeGenModule::EmitGlobalDefinition()
    @     0x5610b8a1fa13  clang::CodeGen::CodeGenModule::EmitDeferred()
    @     0x5610b8a1fa83  clang::CodeGen::CodeGenModule::EmitDeferred()
    @     0x5610b8a18e1d  clang::CodeGen::CodeGenModule::Release()
    @     0x5610b8e291f5  (anonymous namespace)::CodeGeneratorImpl::HandleTranslationUnit()
    @     0x5610b7b19135  clang::BackendConsumer::HandleTranslationUnit()
    @     0x5610ba606281  clang::ParseAST()
    @     0x5610b9fec3e3  clang::ASTFrontendAction::ExecuteAction()
    @     0x5610b7b1f40d  clang::CodeGenAction::ExecuteAction()
    @     0x5610b9feb9eb  clang::FrontendAction::Execute()
    @     0x5610b9e9d3cb  clang::CompilerInstance::ExecuteAction()
    @     0x5610b7b0e405  clang::ExecuteCompilerInvocation()
    @     0x5610b7add5c6  cc1_main()
    @     0x5610b7acaf66  ExecuteCC1Tool()
    @     0x5610b7ac9773  clang_main()
    @     0x5610b7ac91cb  main
    @     0x7ff9e62753d4  __libc_start_main
    @     0x5610b7ac90aa  _start
PLEASE submit a bug report to http://go/clang-crash-bug and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: clang -cc1 -triple riscv64-unknown-linux-musl -emit-obj -target-cpu generic-rv64 -target-feature +m -target-feature +a -target-feature +f -target-feature +d -target-feature +c -target-feature +v -target-feature -b -target-feature -e -target-feature -h -target-abi lp64d -O1 -std=gnu++20 -x c++ galois_field-d4307d.cpp
1.      &lt;eof&gt; parser at end of file
2.      Per-file LLVM IR generation
3.      galois_field-d4307d.cpp:14:12: Generating code for declaration 'operator^'
```
</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: Michal Terepeta (michalt)

<details>
We found a crash when compiling internal code; creduced repro:

```
&gt; cat galois_field-d4307d.cpp
struct a {
  using b = char __attribute__((vector_size(sizeof(char))));
};
class c {
  using d = a::b;
  d e;

public:
  static c f();
};
class g {
public:
  template &lt;class h&gt; g(h);
  friend g operator^(g, g) { c::f; }
  friend g operator^=(g i, g j) { i ^ j; }
};
template &lt;typename, int&gt; using k = g;
template &lt;typename l&gt; using m = k&lt;l, sizeof(l)&gt;;
void n() {
  void o();
  m&lt;char&gt; p = o ^= p;
}
```

Results in:

```
clang "-cc1" "-triple" "riscv64-unknown-linux-musl" "-emit-obj" "-target-cpu" "generic-rv64" "-target-feature" "+m" "-target-feature" "+a" "-target-feature" "+f" "-target-feature" "+d" "-target-feature" "+c" "-target-feature" "+v" "-target-feature" "-b" "-target-feature" "-e" "-target-feature" "-h" "-target-abi" "lp64d" "-O1" "-std=gnu++20" "-x" "c++" "galois_field-d4307d.cpp"
galois_field-d4307d.cpp:14:30: warning: expression result unused [-Wunused-value]
   14 |   friend g operator^(g, g) { c::f; }
      |                              ^~~~
galois_field-d4307d.cpp:14:36: warning: non-void function does not return a value [-Wreturn-type]
   14 |   friend g operator^(g, g) { c::f; }
      |                                    ^
galois_field-d4307d.cpp:15:42: warning: non-void function does not return a value [-Wreturn-type]
   15 |   friend g operator^=(g i, g j) { i ^ j; }
      |                                          ^
galois_field-d4307d.cpp:21:20: warning: variable 'p' is uninitialized when used within its own initialization [-Wuninitialized]
   21 |   m&lt;char&gt; p = o ^= p;
      |           ~        ^
F0000 00:00:1741242680.668504 1430842 logging.cc:62] assert.h assertion failed at llvm/include/llvm/Support/MathExtras.h:405 in T llvm::divideCeil(U, V) [U = unsigned int, V = unsigned int, T = unsigned int]: Denominator &amp;&amp; "Division by zero"
*** Check failure stack trace: ***
    @     0x5610c7648dd9  absl::log_internal::LogMessage::PrepareToDie()
    @     0x5610c7648377  absl::log_internal::LogMessage::SendToLog()
    @     0x5610c764832b  absl::log_internal::LogMessage::Flush()
    @     0x5610bcc4cb45  absl::log_internal::Voidify::operator&amp;&amp;&lt;&gt;()
    @     0x5610c7567eaa  __assert_fail
    @     0x5610b7bc02c1  llvm::divideCeil&lt;&gt;()
    @     0x5610b8f571de  (anonymous namespace)::RISCVABIInfo::detectVLSCCEligibleStruct()
    @     0x5610b8f55fe4  (anonymous namespace)::RISCVABIInfo::classifyArgumentType()
    @     0x5610b8f559b6  (anonymous namespace)::RISCVABIInfo::classifyReturnType()
    @     0x5610b8f54d81  (anonymous namespace)::RISCVABIInfo::computeInfo()
    @     0x5610b7b3d914  clang::CodeGen::CodeGenTypes::arrangeLLVMFunctionInfo()
    @     0x5610b7b3df6d  arrangeLLVMFunctionInfo()
    @     0x5610b7b3dd3e  clang::CodeGen::CodeGenTypes::arrangeFreeFunctionType()
    @     0x5610b8d5f20d  clang::CodeGen::CodeGenTypes::ConvertFunctionTypeInternal()
    @     0x5610b8d5d789  clang::CodeGen::CodeGenTypes::ConvertType()
    @     0x5610b8a44f61  clang::CodeGen::CodeGenModule::GetAddrOfFunction()
    @     0x5610b7ce4ed5  clang::CodeGen::CodeGenModule::getRawFunctionPointer()
    @     0x5610b7c39cfb  clang::CodeGen::CodeGenModule::getFunctionPointer()
    @     0x5610b7ce6b76  EmitFunctionDeclLValue()
    @     0x5610b7cd3ca9  clang::CodeGen::CodeGenFunction::EmitDeclRefLValue()
    @     0x5610b7cd02c6  clang::CodeGen::CodeGenFunction::EmitLValueHelper()
    @     0x5610b7d054ab  clang::CodeGen::CodeGenFunction::EmitLValue()::$_0::operator()()
    @     0x5610b7d05428  llvm::function_ref&lt;&gt;::callback_fn&lt;&gt;()
    @     0x5610ba1c919c  llvm::function_ref&lt;&gt;::operator()()
    @     0x5610be2b4133  clang::runWithSufficientStackSpace()
    @     0x5610be2b3f33  clang::StackExhaustionHandler::runWithSufficientStackSpace()
    @     0x5610b8a24a3d  clang::CodeGen::CodeGenModule::runWithSufficientStackSpace()
    @     0x5610b7cbe553  clang::CodeGen::CodeGenFunction::EmitLValue()
    @     0x5610b7cbdf8d  clang::CodeGen::CodeGenFunction::EmitIgnoredExpr()
    @     0x5610b80543f2  clang::CodeGen::CodeGenFunction::EmitStmt()
    @     0x5610b8062bf4  clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope()
    @     0x5610b89ef9b4  clang::CodeGen::CodeGenFunction::EmitFunctionBody()
    @     0x5610b89f0fd8  clang::CodeGen::CodeGenFunction::GenerateCode()
    @     0x5610b8a41acc  clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition()
    @     0x5610b8a319e1  clang::CodeGen::CodeGenModule::EmitGlobalDefinition()
    @     0x5610b8a1fa13  clang::CodeGen::CodeGenModule::EmitDeferred()
    @     0x5610b8a1fa83  clang::CodeGen::CodeGenModule::EmitDeferred()
    @     0x5610b8a18e1d  clang::CodeGen::CodeGenModule::Release()
    @     0x5610b8e291f5  (anonymous namespace)::CodeGeneratorImpl::HandleTranslationUnit()
    @     0x5610b7b19135  clang::BackendConsumer::HandleTranslationUnit()
    @     0x5610ba606281  clang::ParseAST()
    @     0x5610b9fec3e3  clang::ASTFrontendAction::ExecuteAction()
    @     0x5610b7b1f40d  clang::CodeGenAction::ExecuteAction()
    @     0x5610b9feb9eb  clang::FrontendAction::Execute()
    @     0x5610b9e9d3cb  clang::CompilerInstance::ExecuteAction()
    @     0x5610b7b0e405  clang::ExecuteCompilerInvocation()
    @     0x5610b7add5c6  cc1_main()
    @     0x5610b7acaf66  ExecuteCC1Tool()
    @     0x5610b7ac9773  clang_main()
    @     0x5610b7ac91cb  main
    @     0x7ff9e62753d4  __libc_start_main
    @     0x5610b7ac90aa  _start
PLEASE submit a bug report to http://go/clang-crash-bug and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: clang -cc1 -triple riscv64-unknown-linux-musl -emit-obj -target-cpu generic-rv64 -target-feature +m -target-feature +a -target-feature +f -target-feature +d -target-feature +c -target-feature +v -target-feature -b -target-feature -e -target-feature -h -target-abi lp64d -O1 -std=gnu++20 -x c++ galois_field-d4307d.cpp
1.      &lt;eof&gt; parser at end of file
2.      Per-file LLVM IR generation
3.      galois_field-d4307d.cpp:14:12: Generating code for declaration 'operator^'
```
</details>


---

### Comment 3 - michalt

I think this was fixed by 55f86cf02336e0a1bce81403296cce6d4cfbb1e4 🙂 

---

