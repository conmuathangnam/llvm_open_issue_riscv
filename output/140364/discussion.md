# [clang] Assertion `IdentLoc.isValid() && "Invalid TargetName location."' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/140364

## Body

Reproducer:
https://godbolt.org/z/4xE5nP3jz
```cpp
template <int> class A {};
template <int N> class B : A<N> {
  using A<N>::operator typename A<N>::X;
};
B<0> b;
```

The original reproducer of this issue is the following:
https://github.com/gcc-mirror/gcc/blob/master/gcc/testsuite/g++.dg/template/using13.C

Diagnostics:
```console
<source>:3:39: error: no type named 'X' in 'A<0>'
    3 |   using A<N>::operator typename A<N>::X;
      |                        ~~~~~~~~~~~~~~~^
<source>:5:6: note: in instantiation of template class 'B<0>' requested here
    5 | B<0> b;
      |      ^
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/Sema/SemaDeclCXX.cpp:13334: clang::NamedDecl* clang::Sema::BuildUsingDeclaration(clang::Scope*, clang::AccessSpecifier, clang::SourceLocation, bool, clang::SourceLocation, clang::CXXScopeSpec&, clang::DeclarationNameInfo, clang::SourceLocation, const clang::ParsedAttributesView&, bool, bool): Assertion `IdentLoc.isValid() && "Invalid TargetName location."' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<source>:5:7: current parser token ';'
2.	<source>:2:24: instantiating class definition 'B<0>'
 #0 0x0000000003f9bf28 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f9bf28)
 #1 0x0000000003f99bb4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f99bb4)
 #2 0x0000000003ede7e8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000073722a042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x000073722a0969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x000073722a042476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x000073722a0287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x000073722a02871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x000073722a039e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000006a92558 clang::Sema::BuildUsingDeclaration(clang::Scope*, clang::AccessSpecifier, clang::SourceLocation, bool, clang::SourceLocation, clang::CXXScopeSpec&, clang::DeclarationNameInfo, clang::SourceLocation, clang::ParsedAttributesView const&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a92558)
#10 0x000000000729c459 clang::Decl* clang::TemplateDeclInstantiator::instantiateUnresolvedUsingDecl<clang::UnresolvedUsingValueDecl>(clang::UnresolvedUsingValueDecl*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x729c459)
#11 0x00000000072161ab clang::Sema::InstantiateClass(clang::SourceLocation, clang::CXXRecordDecl*, clang::CXXRecordDecl*, clang::MultiLevelTemplateArgumentList const&, clang::TemplateSpecializationKind, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x72161ab)
#12 0x0000000007238ad8 clang::Sema::InstantiateClassTemplateSpecialization(clang::SourceLocation, clang::ClassTemplateSpecializationDecl*, clang::TemplateSpecializationKind, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7238ad8)
#13 0x00000000072b677e void llvm::function_ref<void ()>::callback_fn<clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*)::'lambda'()>(long) SemaType.cpp:0:0
#14 0x0000000007f3ba31 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7f3ba31)
#15 0x00000000072bf94a clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x72bf94a)
#16 0x00000000072bff45 clang::Sema::RequireCompleteType(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x72bff45)
#17 0x00000000072c00c5 clang::Sema::RequireCompleteType(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x72c00c5)
#18 0x00000000069efe63 clang::Sema::ActOnUninitializedDecl(clang::Decl*) (.part.0) SemaDecl.cpp:0:0
#19 0x000000000666a35e clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x666a35e)
#20 0x000000000667a609 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x667a609)
#21 0x00000000066381be clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66381be)
#22 0x0000000006638979 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6638979)
#23 0x000000000664028a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x664028a)
#24 0x000000000664122d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x664122d)
#25 0x00000000066335fa clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66335fa)
#26 0x00000000049478f8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49478f8)
#27 0x0000000004c3f825 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c3f825)
#28 0x0000000004bbe4ee clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bbe4ee)
#29 0x0000000004d32309 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d32309)
#30 0x0000000000daceaf cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdaceaf)
#31 0x0000000000da307a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#32 0x00000000049aebe9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#33 0x0000000003edec84 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3edec84)
#34 0x00000000049af1ff clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#35 0x0000000004970fcd clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4970fcd)
#36 0x000000000497205e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x497205e)
#37 0x000000000497a345 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x497a345)
#38 0x0000000000da8e18 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda8e18)
#39 0x0000000000c2e464 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc2e464)
#40 0x000073722a029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#41 0x000073722a029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#42 0x0000000000da2b25 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda2b25)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/4xE5nP3jz
```cpp
template &lt;int&gt; class A {};
template &lt;int N&gt; class B : A&lt;N&gt; {
  using A&lt;N&gt;::operator typename A&lt;N&gt;::X;
};
B&lt;0&gt; b;
```
</details>


---

### Comment 2 - shafik

Goes back to clang-2.8: https://godbolt.org/z/M9bn9qTYG

---

