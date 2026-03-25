# Nested Variadic Expansions in generic lambdas don't work

**Author:** llvmbot
**URL:** https://github.com/llvm/llvm-project/issues/18873

## Body

|  |  |
| --- | --- |
| Bugzilla Link | [18499](https://llvm.org/bz18499) |
| Version | trunk |
| OS | Windows NT |
| Reporter | LLVM Bugzilla Contributor |
| CC | @ricejasonf |

## Extended Description 
This code results in a crash:

```cpp
auto K = [](auto ... OuterArgs) {
      vp([=](auto ... Is) {
          decltype(OuterArgs) OA = OuterArgs;
          return 0;
        }(5)...);
      return 0;
    };
   
 auto M = K('a', ' ', 1, " -- ", 3.14);
```

Assertion failed: !Unexpanded.empty() && "Unable to find unexpanded parameter packs", file \llvm\tools\clang\lib\Sema\SemaTemplateVariadic.cpp, line 281

If this is prioritized as important enough to fix, a patch that is in the works, can be moved up in my queue :)

## Comments

### Comment 1 - 1e4a4844-26d5-49f2-a6f8-5d4460edfaee

Hello, I am certain this is related and would like to see this fixed.

https://godbolt.org/z/2zZ-GW

```cpp
void foo() {
  ([](auto ...x) {
    ([](auto ...y) {
      ([y,x] { }, ...);
    })();
  })();
}
```


Ouptuts: static bool llvm::isa_impl_cl<To, const From*>::doit(const From*) [with To = clang::PackExpansionExpr; From = clang::Expr]: Assertion `Val && "isa<> used on a null pointer"' failed.

Frame of interest: #&#8203;16 0x0000562da341bb4b clang::TemplateArgument::isPackExpansion() const /home/jason/Projects/heavy-compiler/clang/lib/AST/TemplateBase.cpp:211:0

I am able to easily reproduce the OP's assertion failure on my local build, but I found this one while reducing the code. Hopefully it provides some additional insight.

I am running on Ubuntu if that matters.

Please consider fixing this!

---

### Comment 2 - Endilll

The original test case appears to be fixed in Clang 6: https://godbolt.org/z/Kvaxoxq1W
The second test case is still crashing on trunk after Clang 16: https://godbolt.org/z/xa7TM9Tds

---

### Comment 3 - llvmbot

@llvm/issue-subscribers-clang-frontend

---

### Comment 4 - shafik

Assertion:

```console
clang++: /root/llvm-project/llvm/include/llvm/Support/Casting.h:109:
static bool llvm::isa_impl_cl<To, const From*>::doit(const From*) [with To = clang::PackExpansionExpr; From = clang::Expr]: 
Assertion `Val && "isa<> used on a null pointer"' failed.
```

Backtrace:

```console
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++14 <source>
1.	<source>:6:6: current parser token ')'
2.	<source>:1:12: parsing function body 'foo'
3.	<source>:1:12: in compound statement ('{}')
4.	<source>:2:4: instantiating function definition 'foo()::(anonymous class)::operator()<>'
5.	<source>:3:6: instantiating function definition 'foo()::(anonymous class)::operator()()::(anonymous class)::operator()<>'
 #0 0x00000000036c1968 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x36c1968)
 #1 0x00000000036bf62c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x36bf62c)
 #2 0x0000000003608a48 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007fd1018d7420 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x14420)
 #4 0x00007fd10139a00b raise (/lib/x86_64-linux-gnu/libc.so.6+0x4300b)
 #5 0x00007fd101379859 abort (/lib/x86_64-linux-gnu/libc.so.6+0x22859)
 #6 0x00007fd101379729 (/lib/x86_64-linux-gnu/libc.so.6+0x22729)
 #7 0x00007fd10138afd6 (/lib/x86_64-linux-gnu/libc.so.6+0x33fd6)
 #8 0x000000000718a9d3 (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x718a9d3)
 #9 0x000000000718e114 clang::TemplateArgument::isPackExpansion() const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x718e114)
#10 0x00000000069ddae4 clang::Sema::collectUnexpandedParameterPacks(clang::TemplateArgument, llvm::SmallVectorImpl<std::pair<llvm::PointerUnion<clang::TemplateTypeParmType const*, clang::NamedDecl*>, clang::SourceLocation>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69ddae4)
#11 0x0000000006935f11 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCXXFoldExpr(clang::CXXFoldExpr*) SemaTemplateInstantiate.cpp:0:0
#12 0x000000000692df3f clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#13 0x00000000069640cf clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformStmt(clang::Stmt*, clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::StmtDiscardKind) SemaTemplateInstantiate.cpp:0:0
#14 0x0000000006965304 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaTemplateInstantiate.cpp:0:0
#15 0x00000000069685da clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69685da)
#16 0x00000000069b7dab clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69b7dab)
#17 0x0000000005f6c801 clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5f6c801)
#18 0x00000000068a13d2 clang::Sema::DeduceReturnType(clang::FunctionDecl*, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68a13d2)
#19 0x0000000006350912 clang::Sema::DiagnoseUseOfDecl(clang::NamedDecl*, llvm::ArrayRef<clang::SourceLocation>, clang::ObjCInterfaceDecl const*, bool, bool, clang::ObjCInterfaceDecl*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6350912)
#20 0x00000000066e8687 CreateFunctionRefExpr(clang::Sema&, clang::FunctionDecl*, clang::NamedDecl*, clang::Expr const*, bool, clang::SourceLocation, clang::DeclarationNameLoc const&) SemaOverload.cpp:0:0
#21 0x0000000006725a1b clang::Sema::BuildCallToObjectOfClassType(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6725a1b)
#22 0x00000000063c45ee clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x63c45ee)
#23 0x00000000063c645c clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x63c645c)
#24 0x0000000006937a10 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCallExpr(clang::CallExpr*) SemaTemplateInstantiate.cpp:0:0
#25 0x000000000692de98 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#26 0x00000000069640cf clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformStmt(clang::Stmt*, clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::StmtDiscardKind) SemaTemplateInstantiate.cpp:0:0
#27 0x0000000006965304 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaTemplateInstantiate.cpp:0:0
#28 0x00000000069685da clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69685da)
#29 0x00000000069b7dab clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69b7dab)
#30 0x0000000005f6c801 clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5f6c801)
#31 0x00000000068a13d2 clang::Sema::DeduceReturnType(clang::FunctionDecl*, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68a13d2)
#32 0x0000000006350912 clang::Sema::DiagnoseUseOfDecl(clang::NamedDecl*, llvm::ArrayRef<clang::SourceLocation>, clang::ObjCInterfaceDecl const*, bool, bool, clang::ObjCInterfaceDecl*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6350912)
#33 0x00000000066e8687 CreateFunctionRefExpr(clang::Sema&, clang::FunctionDecl*, clang::NamedDecl*, clang::Expr const*, bool, clang::SourceLocation, clang::DeclarationNameLoc const&) SemaOverload.cpp:0:0
#34 0x0000000006725a1b clang::Sema::BuildCallToObjectOfClassType(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6725a1b)
#35 0x00000000063c45ee clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x63c45ee)
#36 0x00000000063c645c clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x63c645c)
#37 0x0000000005e8866d clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult<clang::Expr*, true>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5e8866d)
#38 0x0000000005e7fc11 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5e7fc11)
#39 0x0000000005e82dea clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5e82dea)
#40 0x0000000005e82f89 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5e82f89)
#41 0x0000000005e87c09 clang::Parser::ParseExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5e87c09)
#42 0x0000000005f03097 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5f03097)
#43 0x0000000005ef96ed clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5ef96ed)
#44 0x0000000005efa528 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5efa528)
#45 0x0000000005efb469 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5efb469)
#46 0x0000000005efcdca clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5efcdca)
#47 0x0000000005e2a2a1 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5e2a2a1)
#48 0x0000000005e518e8 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5e518e8)
#49 0x0000000005e1e1bb clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5e1e1bb)
#50 0x0000000005e1e8ef clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (.part.0) Parser.cpp:0:0
#51 0x0000000005e25004 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5e25004)
#52 0x0000000005e2580d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5e2580d)
#53 0x0000000005e25c40 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5e25c40)
#54 0x0000000005e19b32 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5e19b32)
#55 0x0000000004922ef8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4922ef8)
#56 0x0000000004181159 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4181159)
#57 0x000000000410393e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x410393e)
#58 0x000000000426195e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x426195e)
#59 0x0000000000bd52de cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xbd52de)
#60 0x0000000000bcd99a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#61 0x0000000003f646e9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#62 0x0000000003608ef4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3608ef4)
#63 0x0000000003f64cdf clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#64 0x0000000003f2d065 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f2d065)
#65 0x0000000003f2dacd clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f2dacd)
#66 0x0000000003f359f5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f359f5)
#67 0x0000000000bd357c clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xbd357c)
#68 0x0000000000acf4d1 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xacf4d1)
#69 0x00007fd10137b083 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x24083)
#70 0x0000000000bcd47e _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xbcd47e)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

---

### Comment 5 - cor3ntin

@erichkeane @Fznamznon

---

### Comment 6 - Fznamznon

The reproducer in https://github.com/llvm/llvm-project/issues/18873#issuecomment-980927648 is a crash on invalid (though the label says the opposite), both clang (before crash) and gcc report error that `x` is not captured. However once I add capture default to inner lambda, the error goes away but the crash stays with slightly different assertion and backtrace - https://godbolt.org/z/Khjb1e76v


---

### Comment 7 - Endilll

@Fznamznon Feel free to relabel as you see fit. You have more insight than I do anyway.

---

### Comment 8 - zyn0217

I'm working on a [fix](https://github.com/llvm/llvm-project/pull/86265) for [#85667](https://github.com/llvm/llvm-project/issues/85667), and this might also be fixed then.

@Fznamznon I hope my patch doesn't conflict with your work if you're going to submit a patch recently. :-)

---

### Comment 9 - cor3ntin

We still crash on this invalid case https://godbolt.org/z/jd4cTjnnE

---

