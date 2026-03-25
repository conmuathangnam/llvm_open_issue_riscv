# [Clang] Crash in Itanium mangler

**Author:** philnik777
**URL:** https://github.com/llvm/llvm-project/issues/82926

## Body

```c++
template<class _Tp>
using __simd_vector __attribute__((__ext_vector_type__(1))) = _Tp;

template<class _VecT>
using __simd_vector_underlying_type_t
    = decltype([]<class _Tp>(__simd_vector<_Tp>) {}(_VecT {}), 1);

template<class _VecT>
void temp()
{
  [](__simd_vector_underlying_type_t<_VecT>) {};
}

void call()
{
  ::temp<::__simd_vector<char>>();
}
```

This causes Clang to crash, but I'm not really sure why. (https://godbolt.org/z/Tz9vEq4h5)

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Nikolas Klauser (philnik777)

<details>
```c++
template&lt;class _Tp&gt;
using __simd_vector __attribute__((__ext_vector_type__(1))) = _Tp;

template&lt;class _VecT&gt;
using __simd_vector_underlying_type_t
    = decltype([]&lt;class _Tp&gt;(__simd_vector&lt;_Tp&gt;) {}(_VecT {}), 1);

template&lt;class _VecT&gt;
void temp()
{
  [](__simd_vector_underlying_type_t&lt;_VecT&gt;) {};
}

void call()
{
  ::temp&lt;::__simd_vector&lt;char&gt;&gt;();
}
```

This causes Clang to crash, but I'm not really sure why. (https://godbolt.org/z/Tz9vEq4h5)
</details>


---

### Comment 2 - shafik

Confirmed: 

Assertion:

```console
clang++: /root/llvm-project/clang/include/clang/AST/Type.h:753:
const clang::ExtQualsTypeCommonBase* clang::QualType::getCommonPtr() const:
Assertion `!isNull() && "Cannot retrieve a NULL type pointer"' failed.
```

Backtrace:

```console
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++23 <source>
1.	<eof> parser at end of file
2.	<source>:10:6: instantiating function definition 'temp<char __attribute__((ext_vector_type(1)))>'
3.	<source>:7:16: instantiating function definition 'temp()::(anonymous class)::clang++: /root/llvm-project/clang/include/clang/AST/Type.h:753: const clang::ExtQualsTypeCommonBase* clang::QualType::getCommonPtr() const: Assertion `!isNull() && "Cannot retrieve a NULL type pointer"' failed.
 #0 0x00000000038c54a8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x38c54a8)
 #1 0x00000000038c2bfc SignalHandler(int) Signals.cpp:0:0
 #2 0x00007f5753642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x00007f57536969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x00007f5753642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x00007f57536287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x00007f575362871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #7 0x00007f5753639e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #8 0x000000000725aa55 (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x725aa55)
 #9 0x000000000725aaad clang::NamedDecl::printQualifiedName(llvm::raw_ostream&, clang::PrintingPolicy const&) const (.part.0) Decl.cpp:0:0
#10 0x000000000725bbdd clang::FunctionDecl::getNameForDiagnostic(llvm::raw_ostream&, clang::PrintingPolicy const&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x725bbdd)
#11 0x000000000724f623 clang::PrettyDeclStackTraceEntry::print(llvm::raw_ostream&) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x724f623)
#12 0x00000000038349f7 PrintCurStackTrace(llvm::raw_ostream&) PrettyStackTrace.cpp:0:0
#13 0x00000000038c318c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x38c318c)
#14 0x000000000380b668 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
#15 0x00007f5753642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
#16 0x00007f57536969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
#17 0x00007f5753642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
#18 0x00007f57536287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
#19 0x00007f575362871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
#20 0x00007f5753639e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
#21 0x00000000074ea626 (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#22 0x00000000074d1054 (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
#23 0x00000000074d18a9 (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
#24 0x00000000074d26ca (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#25 0x00000000074e941d (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#26 0x00000000074ed3be (anonymous namespace)::ItaniumMangleContextImpl::mangleCXXName(clang::GlobalDecl, llvm::raw_ostream&) ItaniumMangle.cpp:0:0
#27 0x00000000075118a3 clang::MangleContext::mangleName(clang::GlobalDecl, llvm::raw_ostream&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x75118a3)
#28 0x0000000003c93833 getMangledNameImpl(clang::CodeGen::CodeGenModule&, clang::GlobalDecl, clang::NamedDecl const*, bool) CodeGenModule.cpp:0:0
#29 0x0000000003c967ac clang::CodeGen::CodeGenModule::getMangledName(clang::GlobalDecl) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3c967ac)
#30 0x0000000003cc09fc clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3cc09fc)
#31 0x0000000003cca671 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#32 0x000000000415a626 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#33 0x000000000414b028 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x414b028)
#34 0x0000000006d02d17 clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d02d17)
#35 0x0000000006289b51 clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6289b51)
#36 0x0000000006be3602 clang::Sema::DeduceReturnType(clang::FunctionDecl*, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6be3602)
#37 0x000000000667d18a clang::Sema::DiagnoseUseOfDecl(clang::NamedDecl*, llvm::ArrayRef<clang::SourceLocation>, clang::ObjCInterfaceDecl const*, bool, bool, clang::ObjCInterfaceDecl*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x667d18a)
#38 0x0000000006a283f7 CreateFunctionRefExpr(clang::Sema&, clang::FunctionDecl*, clang::NamedDecl*, clang::Expr const*, bool, clang::SourceLocation, clang::DeclarationNameLoc const&) SemaOverload.cpp:0:0
#39 0x0000000006a65556 clang::Sema::BuildCallToObjectOfClassType(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a65556)
#40 0x00000000066f0ace clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66f0ace)
#41 0x00000000066f28ac clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66f28ac)
#42 0x0000000006c7eb10 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCallExpr(clang::CallExpr*) SemaTemplateInstantiate.cpp:0:0
#43 0x0000000006c72d66 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#44 0x0000000006c80cc0 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformBinaryOperator(clang::BinaryOperator*) SemaTemplateInstantiate.cpp:0:0
#45 0x0000000006c72ba9 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#46 0x0000000006c82120 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
#47 0x0000000006c8834a clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeSourceInfo*) SemaTemplateInstantiate.cpp:0:0
#48 0x0000000006c88469 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::QualType) SemaTemplateInstantiate.cpp:0:0
#49 0x0000000006c8a8eb clang::Sema::SubstType(clang::QualType, clang::MultiLevelTemplateArgumentList const&, clang::SourceLocation, clang::DeclarationName) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c8a8eb)
#50 0x0000000006b1e11a clang::Sema::CheckTemplateIdType(clang::TemplateName, clang::SourceLocation, clang::TemplateArgumentListInfo&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b1e11a)
#51 0x0000000006c9018d clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformTemplateSpecializationType(clang::TypeLocBuilder&, clang::TemplateSpecializationTypeLoc, clang::TemplateName) SemaTemplateInstantiate.cpp:0:0
#52 0x0000000006c82e02 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
#53 0x0000000006c9914d clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformElaboratedType(clang::TypeLocBuilder&, clang::ElaboratedTypeLoc) SemaTemplateInstantiate.cpp:0:0
#54 0x0000000006c821e5 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
#55 0x0000000006c8834a clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeSourceInfo*) SemaTemplateInstantiate.cpp:0:0
#56 0x0000000006c91d3a clang::Sema::SubstType(clang::TypeSourceInfo*, clang::MultiLevelTemplateArgumentList const&, clang::SourceLocation, clang::DeclarationName, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c91d3a)
#57 0x0000000006c9b0b5 clang::Sema::SubstParmVarDecl(clang::ParmVarDecl*, clang::MultiLevelTemplateArgumentList const&, int, std::optional<unsigned int>, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c9b0b5)
#58 0x0000000006c9c050 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformFunctionTypeParams(clang::SourceLocation, llvm::ArrayRef<clang::ParmVarDecl*>, clang::QualType const*, clang::FunctionType::ExtParameterInfo const*, llvm::SmallVectorImpl<clang::QualType>&, llvm::SmallVectorImpl<clang::ParmVarDecl*>*, clang::Sema::ExtParameterInfoBuilder&, unsigned int*) SemaTemplateInstantiate.cpp:0:0
#59 0x0000000006ca21f4 clang::QualType clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformFunctionProtoType<clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformLambdaExpr(clang::LambdaExpr*)::'lambda0'(clang::TypeLocBuilder&, clang::FunctionProtoTypeLoc)::operator()(clang::TypeLocBuilder&, clang::FunctionProtoTypeLoc) const::'lambda'(clang::FunctionProtoType::ExceptionSpecInfo&, bool&)>(clang::TypeLocBuilder&, clang::FunctionProtoTypeLoc, clang::CXXRecordDecl*, clang::Qualifiers, clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformLambdaExpr(clang::LambdaExpr*)::'lambda0'(clang::TypeLocBuilder&, clang::FunctionProtoTypeLoc)::operator()(clang::TypeLocBuilder&, clang::FunctionProtoTypeLoc) const::'lambda'(clang::FunctionProtoType::ExceptionSpecInfo&, bool&)) (.constprop.0) SemaTemplateInstantiate.cpp:0:0
#60 0x0000000006c71b2e clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformLambdaExpr(clang::LambdaExpr*) SemaTemplateInstantiate.cpp:0:0
#61 0x0000000006c72f7f clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#62 0x0000000006caaa8f clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformStmt(clang::Stmt*, clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::StmtDiscardKind) SemaTemplateInstantiate.cpp:0:0
#63 0x0000000006cab864 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaTemplateInstantiate.cpp:0:0
#64 0x0000000006cb1daa clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6cb1daa)
#65 0x0000000006d03211 clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d03211)
#66 0x0000000006d0162f clang::Sema::PerformPendingInstantiations(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d0162f)
#67 0x00000000062acce9 clang::Sema::ActOnEndOfTranslationUnitFragment(clang::Sema::TUFragmentKind) (.part.0) Sema.cpp:0:0
#68 0x00000000062ad4c2 clang::Sema::ActOnEndOfTranslationUnit() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x62ad4c2)
#69 0x0000000006132eb1 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6132eb1)
#70 0x0000000006125a8a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6125a8a)
#71 0x0000000004158248 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4158248)
#72 0x00000000043d2619 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x43d2619)
#73 0x000000000435118e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x435118e)
#74 0x00000000044b47ce clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x44b47ce)
#75 0x0000000000c184f6 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc184f6)
#76 0x0000000000c0fcea ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#77 0x0000000004199859 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#78 0x000000000380bb14 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x380bb14)
#79 0x0000000004199e4f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#80 0x0000000004161ad5 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4161ad5)
#81 0x000000000416253d clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x416253d)
#82 0x000000000416a475 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x416a475)
#83 0x0000000000c1587c clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc1587c)
#84 0x0000000000b0b864 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xb0b864)
#85 0x00007f5753629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#86 0x00007f5753629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#87 0x0000000000c0f7ae _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc0f7ae)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

---

### Comment 3 - shafik

Looks like a regression from clang-16 to clang-17: https://godbolt.org/z/YE3cod3P5

---

### Comment 4 - shafik

CC @zygoloid could this be because of 4b163e343cfa54c8d55c9da73c70d58f55ea9df2 

---

### Comment 5 - zygoloid

Maybe, but it looks more likely to be a lambdas-in-unevaluated-operands issue.

---

### Comment 6 - shafik

Looks like this also triggers the same problem:  https://godbolt.org/z/z3Y8Pe7YW

```cpp
void f() {
    [] -> decltype([](auto){}(1))
}
```

h/t: 
https://discord.com/channels/636084430946959380/636732781086638081/1261801308734619669

CC @cor3ntin @Endilll 

---

### Comment 7 - shafik

ping @cor3ntin 

---

### Comment 8 - zyn0217

@mizvekov would the lambda refactoring patch also fix it?

---

### Comment 9 - mizvekov

> @mizvekov would the lambda refactoring patch also fix it?

Not currently, neither the original example, nor the one from discord.
I'd have to take a closer look later if it's a missing piece or otherwise related.

---

### Comment 10 - shafik

@mizvekov any luck w/ this one?

---

