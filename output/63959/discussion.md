# clang crash on nested class template specialization

**Author:** Fznamznon
**URL:** https://github.com/llvm/llvm-project/issues/63959

## Body

The following code crashes clang with -std=c++20 option
```
template<typename T>
struct X {
  template<int ct>
  struct impl;
};

template <>
template <int ct>
struct X<int>::impl {
     int f() { return ct; };
 };

void foo() {
    X<int>::impl<17>{}.f();
}
```
Assertion:
```
clang++: /root/llvm-project/clang/lib/Sema/SemaType.cpp:8993: bool clang::Sema::hasAcceptableDefinition(clang::NamedDecl*, clang::NamedDecl**, clang::Sema::AcceptableKind, bool):
Assertion `D && "missing definition for pattern of instantiated definition"' failed.
```
Backtrace:
```

 #0 0x000055704ae9ac1a llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3a9ac1a)
 #1 0x000055704ae9884c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3a9884c)
 #2 0x000055704ade7020 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007f7fad8b4420 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x14420)
 #4 0x00007f7fad38100b raise (/lib/x86_64-linux-gnu/libc.so.6+0x4300b)
 #5 0x00007f7fad360859 abort (/lib/x86_64-linux-gnu/libc.so.6+0x22859)
 #6 0x00007f7fad360729 (/lib/x86_64-linux-gnu/libc.so.6+0x22729)
 #7 0x00007f7fad371fd6 (/lib/x86_64-linux-gnu/libc.so.6+0x33fd6)
 #8 0x000055704dc649bf clang::Sema::hasAcceptableDefinition(clang::NamedDecl*, clang::NamedDecl**, clang::Sema::AcceptableKind, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68649bf)
 #9 0x000055704dc683f3 clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68683f3)
#10 0x000055704dc68ac7 clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6868ac7)
#11 0x000055704dc68e75 clang::Sema::RequireCompleteType(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6868e75)
#12 0x000055704d7a6f0d clang::Sema::BuildCXXTypeConstructExpr(clang::TypeSourceInfo*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x63a6f0d)
#13 0x000055704d82437e clang::Sema::ActOnCXXTypeConstructExpr(clang::OpaquePtr<clang::QualType>, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x642437e)
#14 0x000055704d2a2bb9 clang::Parser::ParseCXXTypeConstructExpression(clang::DeclSpec const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5ea2bb9)
#15 0x000055704d2901e6 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5e901e6)
#16 0x000055704d291ca6 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5e91ca6)
#17 0x000055704d291d48 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5e91d48)
#18 0x000055704d292d49 clang::Parser::ParseExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5e92d49)
#19 0x000055704d2fe56d clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5efe56d)
#20 0x000055704d2f67df clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5ef67df)
#21 0x000055704d2f7597 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5ef7597)
#22 0x000055704d2f84c9 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5ef84c9)
#23 0x000055704d2f8a3b clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5ef8a3b)
#24 0x000055704d247ed8 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5e47ed8)
#25 0x000055704d272d31 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5e72d31)
#26 0x000055704d23e728 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5e3e728)
#27 0x000055704d23f0a6 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (.part.321) Parser.cpp:0:0
#28 0x000055704d24394b clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5e4394b)
#29 0x000055704d245176 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5e45176)
#30 0x000055704d23a22b clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5e3a22b)
#31 0x000055704bf67560 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b67560)
#32 0x000055704b8892e9 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x44892e9)
#33 0x000055704b81dc42 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x441dc42)
#34 0x000055704b960c9f clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4560c9f)
#35 0x0000557048622cc3 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x1222cc3)
#36 0x000055704861ade8 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#37 0x000055704b6912f9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#38 0x000055704ade74a5 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x39e74a5)
#39 0x000055704b693eee clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4293eee)
#40 0x000055704b66060b clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x426060b)
#41 0x000055704b6611e3 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42611e3)
#42 0x000055704b669fd6 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4269fd6)
#43 0x0000557048620a8e clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x1220a8e)
#44 0x0000557048551cb1 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x1151cb1)
#45 0x00007f7fad362083 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x24083)
#46 0x000055704861a4da _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x121a4da)

```
Online demo https://godbolt.org/z/oYajr1E6s .

## Comments

### Comment 1 - llvmbot

@llvm/issue-subscribers-clang-frontend

---

### Comment 2 - shafik

It looks like this crashes back to clang-10

---

### Comment 3 - shafik

Maybe related: https://github.com/llvm/llvm-project/issues/48064

---

### Comment 4 - smcpeak

Slightly different reproducer (no non-type parameters) of presumably the same issue:

```c++
template <class T>
struct Outer {
  template <class U>
  struct Inner;
};

template <>
template <class U>
struct Outer<int>::Inner {};

Outer<int>::Inner<float> i;
```

https://godbolt.org/z/6deETMdM3

It's also perhaps worth noting that the example is closely related to the one in the documentation for [`RedeclarableTemplateDecl::isMemberSpecialization`](https://clang.llvm.org/doxygen/classclang_1_1RedeclarableTemplateDecl.html#a614745d96837c54257c83088908ee794), with the only difference being that the doc example does not instantiate the template.


---

