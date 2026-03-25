# [clang]Clang assertion failure in constant expression evaluation for template static const member with invalid function call initializer

**Author:** Attacker23
**URL:** https://github.com/llvm/llvm-project/issues/166166

## Body

The following code has been causing Clang to crash since version 17.0.1 and still does in the current trunk (x86‑64).

```cpp
bool A(){return true;}

template<typename T> struct X{static const bool f;};

template<typename T> const bool X<T>::f (A (1));

int main (){
  X<int> a;
  if (!a.f) return 1;
  return 0;
}
```

**Output:**  

```
<source>:5:42: error: no matching function for call to 'A'
    5 | template<typename T> const bool X<T>::f (A (1));
      |                                          ^
<source>:1:6: note: candidate function not viable: requires 0 arguments, but 1 was provided
    1 | bool A(){return true;}
      |      ^
clang++: /root/llvm-project/llvm/tools/clang/lib/AST/ExprConstant.cpp:3567: bool evaluateVarDeclInit({anonymous}::EvalInfo&, const clang::Expr*, const clang::VarDecl*, {anonymous}::CallStackFrame*, unsigned int, clang::APValue*&): Assertion `!VD->mightBeUsableInConstantExpressions(Info.Ctx)' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<source>:9:11: current parser token ')'
2.	<source>:7:12: parsing function body 'main'
3.	<source>:7:12: in compound statement ('{}')
 #0 0x0000000004239d18 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4239d18)
 #1 0x0000000004237144 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4237144)
 #2 0x000000000417b398 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x0000789498a42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x0000789498a969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x0000789498a42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x0000789498a287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x0000789498a2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x0000789498a39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000007bd423e (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7bd423e)
#10 0x0000000007bd4c0b findCompleteObject((anonymous namespace)::EvalInfo&, clang::Expr const*, clang::AccessKinds, (anonymous namespace)::LValue const&, clang::QualType) ExprConstant.cpp:0:0
#11 0x0000000007be8a55 handleLValueToRValueConversion((anonymous namespace)::EvalInfo&, clang::Expr const*, clang::QualType, (anonymous namespace)::LValue const&, clang::APValue&, bool) ExprConstant.cpp:0:0
#12 0x0000000007c488a4 (anonymous namespace)::IntExprEvaluator::VisitCastExpr(clang::CastExpr const*) ExprConstant.cpp:0:0
#13 0x0000000007c012eb clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#14 0x0000000007bf06de Evaluate(clang::APValue&, (anonymous namespace)::EvalInfo&, clang::Expr const*) ExprConstant.cpp:0:0
#15 0x0000000007bf8638 EvaluateAsBooleanCondition(clang::Expr const*, bool&, (anonymous namespace)::EvalInfo&) ExprConstant.cpp:0:0
#16 0x0000000007c031aa (anonymous namespace)::IntExprEvaluator::VisitUnaryOperator(clang::UnaryOperator const*) ExprConstant.cpp:0:0
#17 0x0000000007c0267b clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#18 0x0000000007bf06de Evaluate(clang::APValue&, (anonymous namespace)::EvalInfo&, clang::Expr const*) ExprConstant.cpp:0:0
#19 0x0000000007bf0fdb EvaluateAsRValue((anonymous namespace)::EvalInfo&, clang::Expr const*, clang::APValue&) ExprConstant.cpp:0:0
#20 0x0000000007bf4c0e clang::Expr::EvaluateForOverflow(clang::ASTContext const&) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7bf4c0e)
#21 0x0000000006b83d36 clang::Sema::CheckForIntOverflow(clang::Expr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b83d36)
#22 0x0000000006bc65fc clang::Sema::CheckCompletedExpr(clang::Expr*, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bc65fc)
#23 0x0000000006f69257 clang::Sema::ActOnFinishFullExpr(clang::Expr*, clang::SourceLocation, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f69257)
#24 0x0000000006eb6550 clang::Sema::ActOnCondition(clang::Scope*, clang::SourceLocation, clang::Expr*, clang::Sema::ConditionKind, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6eb6550)
#25 0x00000000069d7abc clang::Parser::ParseCXXCondition(clang::ActionResult<clang::Stmt*, true>*, clang::SourceLocation, clang::Sema::ConditionKind, bool, clang::Parser::ForRangeInfo*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69d7abc)
#26 0x0000000006a3f82b clang::Parser::ParseParenExprOrCondition(clang::ActionResult<clang::Stmt*, true>*, clang::Sema::ConditionResult&, clang::SourceLocation, clang::Sema::ConditionKind, clang::SourceLocation&, clang::SourceLocation&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a3f82b)
#27 0x0000000006a446d6 clang::Parser::ParseIfStatement(clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a446d6)
#28 0x0000000006a4207a clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a4207a)
#29 0x0000000006a43089 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a43089)
#30 0x0000000006a4b816 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a4b816)
#31 0x0000000006a4c02a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a4c02a)
#32 0x00000000069529fb clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69529fb)
#33 0x0000000006988edd clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6988edd)
#34 0x00000000069461ae clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69461ae)
#35 0x000000000694694f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x694694f)
#36 0x000000000694e203 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x694e203)
#37 0x000000000694f115 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x694f115)
#38 0x000000000693079a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x693079a)
#39 0x0000000004b84e58 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b84e58)
#40 0x0000000004e72a45 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e72a45)
#41 0x0000000004df384e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4df384e)
#42 0x0000000004f6b21d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f6b21d)
#43 0x0000000000dd0670 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdd0670)
#44 0x0000000000dc71da ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#45 0x0000000000dc735d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#46 0x0000000004bee6f9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#47 0x000000000417b834 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x417b834)
#48 0x0000000004beed0f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#49 0x0000000004bafaa2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bafaa2)
#50 0x0000000004bb0a4e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bb0a4e)
#51 0x0000000004bb8185 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bb8185)
#52 0x0000000000dccb99 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdccb99)
#53 0x0000000000c7bb64 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc7bb64)
#54 0x0000789498a29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#55 0x0000789498a29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#56 0x0000000000dc6c75 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdc6c75)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

see: https://godbolt.org/z/zssGzo1f1

**Conditions required to trigger the crash:**

1. Template  
2. Const static member  
3. Invalid function call in initializer (signature mismatch or otherwise invalid)  
4. A context that forces constant evaluation, such as:  
   * `if (~a.f)`  
   * `switch (+a.f)`  
   * `int arr[a.f];`

---

**Additional cases that also crash:**

```cpp
bool A(int i) { return true; }// also crashes

template<typename T> struct X { static const bool f; };

template<typename T> const bool X<T>::f(A());

int main() {
  X<int> a;
  if (~a.f) return 1;
  return 0;
}
```
https://godbolt.org/z/7cr5o3q5s

and:

```cpp
void A() { return true; }// also crashes

template<typename T> struct X { static const bool f; };

template<typename T> const bool X<T>::f(A());

int main() {
  X<int> a;
  if (!a.f) return 1;
  return 0;
}
```

https://godbolt.org/z/8hrhrYGd1

**Another example:**

```cpp
void A() { return true; }

template<typename T>
struct X {
  inline static const int f = A();
};

int main() {
  X<int> a;
  int arr[a.f]; // also crashes
}
```
https://godbolt.org/z/Y9qddaz4z

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (Attacker23)

<details>
The following code has been causing Clang to crash since version 17.0.1 and still does in the current trunk (x86‑64).

```cpp
bool A(){return true;}

template&lt;typename T&gt; struct X{static const bool f;};

template&lt;typename T&gt; const bool X&lt;T&gt;::f (A (1));

int main (){
  X&lt;int&gt; a;
  if (!a.f) return 1;
  return 0;
}
```

**Output:**  

```
&lt;source&gt;:5:42: error: no matching function for call to 'A'
    5 | template&lt;typename T&gt; const bool X&lt;T&gt;::f (A (1));
      |                                          ^
&lt;source&gt;:1:6: note: candidate function not viable: requires 0 arguments, but 1 was provided
    1 | bool A(){return true;}
      |      ^
clang++: /root/llvm-project/llvm/tools/clang/lib/AST/ExprConstant.cpp:3567: bool evaluateVarDeclInit({anonymous}::EvalInfo&amp;, const clang::Expr*, const clang::VarDecl*, {anonymous}::CallStackFrame*, unsigned int, clang::APValue*&amp;): Assertion `!VD-&gt;mightBeUsableInConstantExpressions(Info.Ctx)' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics &lt;source&gt;
1.	&lt;source&gt;:9:11: current parser token ')'
2.	&lt;source&gt;:7:12: parsing function body 'main'
3.	&lt;source&gt;:7:12: in compound statement ('{}')
 #<!-- -->0 0x0000000004239d18 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4239d18)
 #<!-- -->1 0x0000000004237144 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4237144)
 #<!-- -->2 0x000000000417b398 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x0000789498a42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x0000789498a969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x0000789498a42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x0000789498a287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x0000789498a2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x0000789498a39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000007bd423e (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7bd423e)
#<!-- -->10 0x0000000007bd4c0b findCompleteObject((anonymous namespace)::EvalInfo&amp;, clang::Expr const*, clang::AccessKinds, (anonymous namespace)::LValue const&amp;, clang::QualType) ExprConstant.cpp:0:0
#<!-- -->11 0x0000000007be8a55 handleLValueToRValueConversion((anonymous namespace)::EvalInfo&amp;, clang::Expr const*, clang::QualType, (anonymous namespace)::LValue const&amp;, clang::APValue&amp;, bool) ExprConstant.cpp:0:0
#<!-- -->12 0x0000000007c488a4 (anonymous namespace)::IntExprEvaluator::VisitCastExpr(clang::CastExpr const*) ExprConstant.cpp:0:0
#<!-- -->13 0x0000000007c012eb clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool&gt;::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#<!-- -->14 0x0000000007bf06de Evaluate(clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Expr const*) ExprConstant.cpp:0:0
#<!-- -->15 0x0000000007bf8638 EvaluateAsBooleanCondition(clang::Expr const*, bool&amp;, (anonymous namespace)::EvalInfo&amp;) ExprConstant.cpp:0:0
#<!-- -->16 0x0000000007c031aa (anonymous namespace)::IntExprEvaluator::VisitUnaryOperator(clang::UnaryOperator const*) ExprConstant.cpp:0:0
#<!-- -->17 0x0000000007c0267b clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool&gt;::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#<!-- -->18 0x0000000007bf06de Evaluate(clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Expr const*) ExprConstant.cpp:0:0
#<!-- -->19 0x0000000007bf0fdb EvaluateAsRValue((anonymous namespace)::EvalInfo&amp;, clang::Expr const*, clang::APValue&amp;) ExprConstant.cpp:0:0
#<!-- -->20 0x0000000007bf4c0e clang::Expr::EvaluateForOverflow(clang::ASTContext const&amp;) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7bf4c0e)
#<!-- -->21 0x0000000006b83d36 clang::Sema::CheckForIntOverflow(clang::Expr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b83d36)
#<!-- -->22 0x0000000006bc65fc clang::Sema::CheckCompletedExpr(clang::Expr*, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bc65fc)
#<!-- -->23 0x0000000006f69257 clang::Sema::ActOnFinishFullExpr(clang::Expr*, clang::SourceLocation, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f69257)
#<!-- -->24 0x0000000006eb6550 clang::Sema::ActOnCondition(clang::Scope*, clang::SourceLocation, clang::Expr*, clang::Sema::ConditionKind, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6eb6550)
#<!-- -->25 0x00000000069d7abc clang::Parser::ParseCXXCondition(clang::ActionResult&lt;clang::Stmt*, true&gt;*, clang::SourceLocation, clang::Sema::ConditionKind, bool, clang::Parser::ForRangeInfo*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69d7abc)
#<!-- -->26 0x0000000006a3f82b clang::Parser::ParseParenExprOrCondition(clang::ActionResult&lt;clang::Stmt*, true&gt;*, clang::Sema::ConditionResult&amp;, clang::SourceLocation, clang::Sema::ConditionKind, clang::SourceLocation&amp;, clang::SourceLocation&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a3f82b)
#<!-- -->27 0x0000000006a446d6 clang::Parser::ParseIfStatement(clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a446d6)
#<!-- -->28 0x0000000006a4207a clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a4207a)
#<!-- -->29 0x0000000006a43089 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a43089)
#<!-- -->30 0x0000000006a4b816 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a4b816)
#<!-- -->31 0x0000000006a4c02a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a4c02a)
#<!-- -->32 0x00000000069529fb clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69529fb)
#<!-- -->33 0x0000000006988edd clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6988edd)
#<!-- -->34 0x00000000069461ae clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69461ae)
#<!-- -->35 0x000000000694694f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x694694f)
#<!-- -->36 0x000000000694e203 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x694e203)
#<!-- -->37 0x000000000694f115 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x694f115)
#<!-- -->38 0x000000000693079a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x693079a)
#<!-- -->39 0x0000000004b84e58 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b84e58)
#<!-- -->40 0x0000000004e72a45 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e72a45)
#<!-- -->41 0x0000000004df384e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4df384e)
#<!-- -->42 0x0000000004f6b21d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f6b21d)
#<!-- -->43 0x0000000000dd0670 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdd0670)
#<!-- -->44 0x0000000000dc71da ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->45 0x0000000000dc735d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->46 0x0000000004bee6f9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->47 0x000000000417b834 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x417b834)
#<!-- -->48 0x0000000004beed0f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->49 0x0000000004bafaa2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bafaa2)
#<!-- -->50 0x0000000004bb0a4e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bb0a4e)
#<!-- -->51 0x0000000004bb8185 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bb8185)
#<!-- -->52 0x0000000000dccb99 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdccb99)
#<!-- -->53 0x0000000000c7bb64 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc7bb64)
#<!-- -->54 0x0000789498a29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->55 0x0000789498a29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->56 0x0000000000dc6c75 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdc6c75)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

see: https://godbolt.org/z/zssGzo1f1

**Conditions required to trigger the crash:**

1. Template  
2. Const static member  
3. Invalid function call in initializer (signature mismatch or otherwise invalid)  
4. A context that forces constant evaluation, such as:  
   * `if (~a.f)`  
   * `switch (+a.f)`  
   * `int arr[a.f];`

---

**Additional cases that also crash:**

```cpp
bool A(int i) { return true; }// also crashes

template&lt;typename T&gt; struct X { static const bool f; };

template&lt;typename T&gt; const bool X&lt;T&gt;::f(A());

int main() {
  X&lt;int&gt; a;
  if (~a.f) return 1;
  return 0;
}
```
https://godbolt.org/z/7cr5o3q5s

and:

```cpp
void A() { return true; }// also crashes

template&lt;typename T&gt; struct X { static const bool f; };

template&lt;typename T&gt; const bool X&lt;T&gt;::f(A());

int main() {
  X&lt;int&gt; a;
  if (!a.f) return 1;
  return 0;
}
```

https://godbolt.org/z/8hrhrYGd1

**Another example:**

```cpp
void A() { return true; }

template&lt;typename T&gt;
struct X {
  inline static const int f = A();
};

int main() {
  X&lt;int&gt; a;
  int arr[a.f]; // also crashes
}
```
https://godbolt.org/z/Y9qddaz4z
</details>


---

