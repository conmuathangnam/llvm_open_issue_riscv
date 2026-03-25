# Crash in getPackSubstitutedTemplateArgument

**Author:** colavitam
**URL:** https://github.com/llvm/llvm-project/issues/53609
**Status:** Closed
**Labels:** c++17, clang:frontend, confirmed, crash-on-valid
**Closed Date:** 2025-05-27T07:10:25Z

## Body

The following valid code causes a crash in all versions of clang I have tested (`-std=c++17`).
```cpp
template <class, int> struct a;

template <class, class...> struct b;
template <class x, class... y, y... z>
struct b<x, a<y, z>...> {};

template <class... x> struct c: b<x>...  {};

c<int> d;
```

## Comments

### Comment 1 - colavitam

Proposed fix at https://reviews.llvm.org/D119063

---

### Comment 2 - shafik

Confirmed: https://godbolt.org/z/3jWKfsnrf

Assertion:

```console
clang++: /root/llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:1828: 
clang::TemplateArgument getPackSubstitutedTemplateArgument(clang::Sema&, clang::TemplateArgument):
Assertion `S.ArgumentPackSubstitutionIndex < (int)Arg.pack_size()' failed.
```

Backtrace:

```cpp
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++26 <source>
1.	<source>:9:9: current parser token ';'
2.	<source>:7:30: instantiating class definition 'c<int>'
 #0 0x000000000393d208 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x393d208)
 #1 0x000000000393aeec llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x393aeec)
 #2 0x0000000003880fb8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007f975f442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007f975f4969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007f975f442476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007f975f4287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007f975f42871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007f975f439e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000006d6e0f2 getPackSubstitutedTemplateArgument(clang::Sema&, clang::TemplateArgument) (.isra.0) SemaTemplateInstantiate.cpp:0:0
#10 0x0000000006d72842 (anonymous namespace)::TemplateInstantiator::TransformTemplateTypeParmType(clang::TypeLocBuilder&, clang::TemplateTypeParmTypeLoc, bool) SemaTemplateInstantiate.cpp:0:0
#11 0x0000000006d94f1a clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
#12 0x0000000006d9a65d clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformPackExpansionType(clang::TypeLocBuilder&, clang::PackExpansionTypeLoc) SemaTemplateInstantiate.cpp:0:0
#13 0x0000000006d95839 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
#14 0x0000000006d9b71a clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeSourceInfo*) SemaTemplateInstantiate.cpp:0:0
#15 0x0000000006d9b83c clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::QualType) SemaTemplateInstantiate.cpp:0:0
#16 0x0000000006d9dc7b clang::Sema::SubstType(clang::QualType, clang::MultiLevelTemplateArgumentList const&, clang::SourceLocation, clang::DeclarationName) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d9dc7b)
#17 0x0000000006cf80b2 ConvertDeducedTemplateArgument(clang::Sema&, clang::NamedDecl*, clang::DeducedTemplateArgument, clang::NamedDecl*, clang::sema::TemplateDeductionInfo&, bool, llvm::SmallVectorImpl<clang::TemplateArgument>&, llvm::SmallVectorImpl<clang::TemplateArgument>&) SemaTemplateDeduction.cpp:0:0
#18 0x0000000006d0177c _Z31FinishTemplateArgumentDeductionIN5clang38ClassTemplatePartialSpecializationDeclEENSt9enable_ifIXsr23IsPartialSpecializationIT_E5valueENS0_23TemplateDeductionResultEE4typeERNS0_4SemaEPS4_bN4llvm8ArrayRefINS0_16TemplateArgumentEEERNSC_15SmallVectorImplINS0_23DeducedTemplateArgumentEEERNS0_4sema21TemplateDeductionInfoE SemaTemplateDeduction.cpp:0:0
#19 0x0000000006d0338f _ZN4llvm12function_refIFvvEE11callback_fnIZ23DeduceTemplateArgumentsIN5clang38ClassTemplatePartialSpecializationDeclEENSt9enable_ifIXsr23IsPartialSpecializationIT_E5valueENS5_23TemplateDeductionResultEE4typeERNS5_4SemaEPS9_NS_8ArrayRefINS5_16TemplateArgumentEEERNS5_4sema21TemplateDeductionInfoEEUlvE_EEvl SemaTemplateDeduction.cpp:0:0
#20 0x0000000006354461 clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6354461)
#21 0x0000000006d0b302 _Z23DeduceTemplateArgumentsIN5clang38ClassTemplatePartialSpecializationDeclEENSt9enable_ifIXsr23IsPartialSpecializationIT_E5valueENS0_23TemplateDeductionResultEE4typeERNS0_4SemaEPS4_N4llvm8ArrayRefINS0_16TemplateArgumentEEERNS0_4sema21TemplateDeductionInfoE SemaTemplateDeduction.cpp:0:0
#22 0x0000000006d8246d getPatternForClassTemplateSpecialization(clang::Sema&, clang::SourceLocation, clang::ClassTemplateSpecializationDecl*, clang::TemplateSpecializationKind) (.isra.0) SemaTemplateInstantiate.cpp:0:0
#23 0x0000000006dc5d71 clang::Sema::InstantiateClassTemplateSpecialization(clang::SourceLocation, clang::ClassTemplateSpecializationDecl*, clang::TemplateSpecializationKind, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6dc5d71)
#24 0x0000000006e514df void llvm::function_ref<void ()>::callback_fn<clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*)::'lambda'()>(long) SemaType.cpp:0:0
#25 0x0000000006354461 clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6354461)
#26 0x0000000006e5b9b7 clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e5b9b7)
#27 0x0000000006e5bd05 clang::Sema::RequireCompleteType(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e5bd05)
#28 0x00000000066ae3b5 clang::Sema::CheckBaseSpecifier(clang::CXXRecordDecl*, clang::SourceRange, bool, clang::AccessSpecifier, clang::TypeSourceInfo*, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66ae3b5)
#29 0x0000000006da5e5a clang::Sema::SubstBaseSpecifiers(clang::CXXRecordDecl*, clang::CXXRecordDecl*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6da5e5a)
#30 0x0000000006da66f4 clang::Sema::InstantiateClass(clang::SourceLocation, clang::CXXRecordDecl*, clang::CXXRecordDecl*, clang::MultiLevelTemplateArgumentList const&, clang::TemplateSpecializationKind, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6da66f4)
#31 0x0000000006dc5de2 clang::Sema::InstantiateClassTemplateSpecialization(clang::SourceLocation, clang::ClassTemplateSpecializationDecl*, clang::TemplateSpecializationKind, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6dc5de2)
#32 0x0000000006e514df void llvm::function_ref<void ()>::callback_fn<clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*)::'lambda'()>(long) SemaType.cpp:0:0
#33 0x0000000006354461 clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6354461)
#34 0x0000000006e5b9b7 clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e5b9b7)
#35 0x0000000006e5bd05 clang::Sema::RequireCompleteType(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e5bd05)
#36 0x0000000006e5be85 clang::Sema::RequireCompleteType(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e5be85)
#37 0x0000000006566811 clang::Sema::ActOnUninitializedDecl(clang::Decl*) (.part.0) SemaDecl.cpp:0:0
#38 0x0000000006223af4 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6223af4)
#39 0x0000000006232c1a clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6232c1a)
#40 0x00000000061f074e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x61f074e)
#41 0x00000000061f0f43 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x61f0f43)
#42 0x00000000061f8466 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x61f8466)
#43 0x00000000061f930d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x61f930d)
#44 0x00000000061ebf5a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x61ebf5a)
#45 0x00000000041edcb8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41edcb8)
#46 0x000000000446d3c9 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x446d3c9)
#47 0x00000000043f624e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x43f624e)
#48 0x000000000455062e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x455062e)
#49 0x0000000000c45f8c cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc45f8c)
#50 0x0000000000c3f21a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#51 0x0000000004230cd9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#52 0x0000000003881464 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3881464)
#53 0x00000000042312cf clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#54 0x00000000041f7605 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41f7605)
#55 0x00000000041f806d clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41f806d)
#56 0x00000000041ffdf5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41ffdf5)
#57 0x0000000000c43425 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc43425)
#58 0x0000000000b26064 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xb26064)
#59 0x00007f975f429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#60 0x00007f975f429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#61 0x0000000000c3ed0e _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc3ed0e)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

---

### Comment 3 - shafik

> Proposed fix at https://reviews.llvm.org/D119063

@colavitam ping on the fix

---

