# clang crash due to missing typename for dependent type

**Author:** pfent
**URL:** https://github.com/llvm/llvm-project/issues/174301

## Body

The following minimized reproducer produces a clang frontend crash:

```cpp
template <class T>
struct Tester {
   struct Inner {
      using T2 = int;
   };
   static void test();
};

template <class T>
/*typename*/ Tester<T>::Inner::T2 getInnerT2() { return {}; }

template <class T>
void Tester<T>::test() {
   auto _ = getInnerT2<T>();
}

template struct Tester<int>;
```

https://compiler-explorer.com/z/jnqf8fP68

Assertion:
```
clang++: /root/llvm-project/llvm/tools/clang/lib/AST/ASTContext.cpp:2428:
clang::TypeInfo clang::ASTContext::getTypeInfoImpl(const clang::Type*) const:
Assertion `!A->getDeducedType().isNull() && "cannot request the size of an undeduced or dependent auto type"' failed.
```

Note that this only crashes when omitting the `typename` from `getInnerT2()`. With explicit `typename`, the code compiles.


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: Philipp Fent (pfent)

<details>
The following minimized reproducer produces a clang frontend crash:

```cpp
template &lt;class T&gt;
struct Tester {
   struct Inner {
      using T2 = int;
   };
   static void test();
};

template &lt;class T&gt;
/*typename*/ Tester&lt;T&gt;::Inner::T2 getInnerT2() { return {}; }

template &lt;class T&gt;
void Tester&lt;T&gt;::test() {
   auto _ = getInnerT2&lt;T&gt;();
}

template struct Tester&lt;int&gt;;
```

https://compiler-explorer.com/z/jnqf8fP68

Assertion:
```
clang++: /root/llvm-project/llvm/tools/clang/lib/AST/ASTContext.cpp:2428:
clang::TypeInfo clang::ASTContext::getTypeInfoImpl(const clang::Type*) const:
Assertion `!A-&gt;getDeducedType().isNull() &amp;&amp; "cannot request the size of an undeduced or dependent auto type"' failed.
```

Note that this only crashes when omitting the `typename` from `getInnerT2()`. With explicit `typename`, the code compiles.

</details>


---

### Comment 2 - zwuis

This is valid in C++20.

Maybe related: #17283.

---

### Comment 3 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Philipp Fent (pfent)

<details>
The following minimized reproducer produces a clang frontend crash:

```cpp
template &lt;class T&gt;
struct Tester {
   struct Inner {
      using T2 = int;
   };
   static void test();
};

template &lt;class T&gt;
/*typename*/ Tester&lt;T&gt;::Inner::T2 getInnerT2() { return {}; }

template &lt;class T&gt;
void Tester&lt;T&gt;::test() {
   auto _ = getInnerT2&lt;T&gt;();
}

template struct Tester&lt;int&gt;;
```

https://compiler-explorer.com/z/jnqf8fP68

Assertion:
```
clang++: /root/llvm-project/llvm/tools/clang/lib/AST/ASTContext.cpp:2428:
clang::TypeInfo clang::ASTContext::getTypeInfoImpl(const clang::Type*) const:
Assertion `!A-&gt;getDeducedType().isNull() &amp;&amp; "cannot request the size of an undeduced or dependent auto type"' failed.
```

Note that this only crashes when omitting the `typename` from `getInnerT2()`. With explicit `typename`, the code compiles.

</details>


---

### Comment 4 - shafik

It can be helpful when reporting crashes and bugs to see how far back they go, to know if this is a regression. 

This started w/ clang-16: https://compiler-explorer.com/z/ezzxe6qvs

Backtrace:

```console
 #0 0x0000000004308648 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4308648)
 #1 0x0000000004305a74 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4305a74)
 #2 0x000000000424a518 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007d7570642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007d75706969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007d7570642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007d75706287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007d757062871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007d7570639e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x00000000079a81cd clang::ASTContext::getTypeInfoImpl(clang::Type const*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x79a81cd)
#10 0x00000000079a53b7 clang::ASTContext::getTypeInfo(clang::Type const*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x79a53b7)
#11 0x00000000079aa22b clang::ASTContext::getPreferredTypeAlign(clang::Type const*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x79aa22b)
#12 0x00000000079aae8b clang::ASTContext::getDeclAlign(clang::Decl const*, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x79aae8b)
#13 0x0000000004bc0d19 clang::CodeGen::CodeGenFunction::EmitAutoVarAlloca(clang::VarDecl const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bc0d19)
#14 0x0000000004bc771b clang::CodeGen::CodeGenFunction::EmitVarDecl(clang::VarDecl const&) (.part.0) CGDecl.cpp:0:0
#15 0x0000000004bc7df7 clang::CodeGen::CodeGenFunction::EmitDecl(clang::Decl const&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bc7df7)
#16 0x000000000480468c clang::CodeGen::CodeGenFunction::EmitDeclStmt(clang::DeclStmt const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x480468c)
#17 0x000000000481b4fd clang::CodeGen::CodeGenFunction::EmitSimpleStmt(clang::Stmt const*, llvm::ArrayRef<clang::Attr const*>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x481b4fd)
#18 0x0000000004812a61 clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef<clang::Attr const*>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4812a61)
#19 0x000000000481b05b clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&, bool, clang::CodeGen::AggValueSlot) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x481b05b)
#20 0x000000000488309e clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x488309e)
#21 0x00000000048944f4 clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48944f4)
#22 0x00000000049022d0 clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49022d0)
#23 0x00000000048fcf44 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48fcf44)
#24 0x00000000048fd753 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48fd753)
#25 0x0000000004908ce3 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#26 0x0000000004c6c4c9 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#27 0x0000000004c5cff4 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c5cff4)
#28 0x000000000766501a clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x766501a)
#29 0x00000000075a3d2f clang::Sema::InstantiateClassMembers(clang::SourceLocation, clang::CXXRecordDecl*, clang::MultiLevelTemplateArgumentList const&, clang::TemplateSpecializationKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x75a3d2f)
#30 0x00000000075a44d7 clang::Sema::InstantiateClassTemplateSpecializationMembers(clang::SourceLocation, clang::ClassTemplateSpecializationDecl*, clang::TemplateSpecializationKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x75a44d7)
#31 0x000000000746bf80 clang::Sema::ActOnExplicitInstantiation(clang::Scope*, clang::SourceLocation, clang::SourceLocation, unsigned int, clang::SourceLocation, clang::CXXScopeSpec const&, clang::OpaquePtr<clang::TemplateName>, clang::SourceLocation, clang::SourceLocation, llvm::MutableArrayRef<clang::ParsedTemplateArgument>, clang::SourceLocation, clang::ParsedAttributesView const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x746bf80)
#32 0x0000000006ab69c4 clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ab69c4)
#33 0x0000000006a898ee clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a898ee)
#34 0x0000000006b577a1 clang::Parser::ParseDeclarationAfterTemplate(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo&, clang::ParsingDeclRAIIObject&, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b577a1)
#35 0x0000000006b5834c clang::Parser::ParseExplicitInstantiation(clang::DeclaratorContext, clang::SourceLocation, clang::SourceLocation, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b5834c)
#36 0x0000000006b651c8 clang::Parser::ParseDeclarationStartingWithTemplate(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b651c8)
#37 0x0000000006a90640 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a90640)
#38 0x0000000006a4d03c clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a4d03c)
#39 0x0000000006a4df25 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a4df25)
#40 0x0000000006a2efaa clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a2efaa)
#41 0x0000000004c698d8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c698d8)
#42 0x0000000004f600a5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f600a5)
#43 0x0000000004ee025e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ee025e)
#44 0x000000000505a42d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x505a42d)
#45 0x0000000000de722c cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde722c)
#46 0x0000000000dddc7a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#47 0x0000000000ddddfd int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#48 0x0000000004cdbfa9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#49 0x000000000424a9b4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x424a9b4)
#50 0x0000000004cdc5bf clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#51 0x0000000004c9d0a2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c9d0a2)
#52 0x0000000004c9e04e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c9e04e)
#53 0x0000000004ca54a5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ca54a5)
#54 0x0000000000de3621 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde3621)
#55 0x0000000000c8df54 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc8df54)
#56 0x00007d7570629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#57 0x00007d7570629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#58 0x0000000000ddd715 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xddd715)
```

---

