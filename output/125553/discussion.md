# ARM NEON builtins involving type __mfp8 seem to be incorrectly declared for armv7 and crash the compiler (clang 20.1.0 rc1)

**Author:** cmeerw
**URL:** https://github.com/llvm/llvm-project/issues/125553

## Body

see https://godbolt.org/z/4a3zbMPG9

```
int i = __builtin_neon_vcvt2_low_bf16_mf8_fpm(0, 0);
```

* armv8 clang complains about incorrect argument types: fine
* x86-64 clang complains about undeclared identifier: fine
* armv7 clang crashes
```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -fno-verbose-asm -S -target arm-linux-gnueabihf --gcc-toolchain=/opt/compiler-explorer/arm/gcc-12.2.0/arm-unknown-linux-gnueabihf --sysroot=/opt/compiler-explorer/arm/gcc-12.2.0/arm-unknown-linux-gnueabihf/arm-unknown-linux-gnueabihf/sysroot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<source>:1:9: current parser token '__builtin_neon_vcvt2_low_bf16_mf8_fpm'
 #0 0x0000000003b10218 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3b10218)
 #1 0x0000000003b0e35c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3b0e35c)
 #2 0x0000000003a5d698 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007187ba042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x0000000006ebaf5f clang::ASTContext::getVectorType(clang::QualType, unsigned int, clang::VectorKind) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6ebaf5f)
 #5 0x0000000006ee81a7 DecodeTypeFromStr(char const*&, clang::ASTContext const&, clang::ASTContext::GetBuiltinTypeError&, bool&, bool) ASTContext.cpp:0:0
 #6 0x0000000006ee8954 clang::ASTContext::GetBuiltinType(unsigned int, clang::ASTContext::GetBuiltinTypeError&, unsigned int*) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6ee8954)
 #7 0x000000000639306e clang::Sema::LazilyCreateBuiltin(clang::IdentifierInfo*, unsigned int, clang::Scope*, bool, clang::SourceLocation) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x639306e)
 #8 0x000000000673501a clang::Sema::LookupBuiltin(clang::LookupResult&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x673501a)
 #9 0x000000000673f369 LookupDirect(clang::Sema&, clang::LookupResult&, clang::DeclContext const*) SemaLookup.cpp:0:0
#10 0x000000000673f509 CppNamespaceLookup(clang::Sema&, clang::LookupResult&, clang::ASTContext&, clang::DeclContext const*, (anonymous namespace)::UnqualUsingDirectiveSet&) (.isra.0) SemaLookup.cpp:0:0
#11 0x000000000672dd5b clang::Sema::CppLookupName(clang::LookupResult&, clang::Scope*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x672dd5b)
#12 0x000000000672ecc5 clang::Sema::LookupName(clang::LookupResult&, clang::Scope*, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x672ecc5)
#13 0x00000000063d75cd clang::Sema::getTypeName(clang::IdentifierInfo const&, clang::SourceLocation, clang::Scope*, clang::CXXScopeSpec*, bool, bool, clang::OpaquePtr<clang::QualType>, bool, bool, bool, clang::ImplicitTypenameContext, clang::IdentifierInfo**) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x63d75cd)
#14 0x0000000006003cbd clang::Parser::TryAnnotateTypeOrScopeTokenAfterScopeSpec(clang::CXXScopeSpec&, bool, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6003cbd)
#15 0x0000000006004014 clang::Parser::TryAnnotateTypeOrScopeToken(clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6004014)
#16 0x0000000006086fbe clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6086fbe)
#17 0x0000000006088faa clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6088faa)
#18 0x0000000006089179 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6089179)
#19 0x00000000060416b8 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x60416b8)
#20 0x000000000605743c clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x605743c)
#21 0x0000000006005d77 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6005d77)
#22 0x0000000006006b8e clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6006b8e)
#23 0x0000000006010492 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6010492)
#24 0x0000000006011d28 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6011d28)
#25 0x0000000006012270 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6012270)
#26 0x0000000006000393 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6000393)
#27 0x00000000044614b5 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x44614b5)
#28 0x0000000004737121 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4737121)
#29 0x00000000046b777b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x46b777b)
#30 0x00000000048229e3 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x48229e3)
#31 0x0000000000d50621 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xd50621)
#32 0x0000000000d48e2d ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#33 0x00000000044acc99 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#34 0x0000000003a5dac3 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3a5dac3)
#35 0x00000000044aceb9 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#36 0x00000000044710dd clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x44710dd)
#37 0x0000000004472071 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4472071)
#38 0x000000000447c3fc clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x447c3fc)
#39 0x0000000000d4d2b1 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xd4d2b1)
#40 0x0000000000c12464 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc12464)
#41 0x00007187ba029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#42 0x00007187ba029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#43 0x0000000000d488c5 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xd488c5)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```

Presumably, that builtin shouldn't be declared for armv7?


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Christof Meerwald (cmeerw)

<details>
see https://godbolt.org/z/4a3zbMPG9

```
int i = __builtin_neon_vcvt2_low_bf16_mf8_fpm(0, 0);
```

* armv8 clang complains about incorrect argument types: fine
* x86-64 clang complains about undeclared identifier: fine
* armv7 clang crashes
```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -fno-verbose-asm -S -target arm-linux-gnueabihf --gcc-toolchain=/opt/compiler-explorer/arm/gcc-12.2.0/arm-unknown-linux-gnueabihf --sysroot=/opt/compiler-explorer/arm/gcc-12.2.0/arm-unknown-linux-gnueabihf/arm-unknown-linux-gnueabihf/sysroot -fcolor-diagnostics -fno-crash-diagnostics &lt;source&gt;
1.	&lt;source&gt;:1:9: current parser token '__builtin_neon_vcvt2_low_bf16_mf8_fpm'
 #<!-- -->0 0x0000000003b10218 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3b10218)
 #<!-- -->1 0x0000000003b0e35c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3b0e35c)
 #<!-- -->2 0x0000000003a5d698 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007187ba042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x0000000006ebaf5f clang::ASTContext::getVectorType(clang::QualType, unsigned int, clang::VectorKind) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6ebaf5f)
 #<!-- -->5 0x0000000006ee81a7 DecodeTypeFromStr(char const*&amp;, clang::ASTContext const&amp;, clang::ASTContext::GetBuiltinTypeError&amp;, bool&amp;, bool) ASTContext.cpp:0:0
 #<!-- -->6 0x0000000006ee8954 clang::ASTContext::GetBuiltinType(unsigned int, clang::ASTContext::GetBuiltinTypeError&amp;, unsigned int*) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6ee8954)
 #<!-- -->7 0x000000000639306e clang::Sema::LazilyCreateBuiltin(clang::IdentifierInfo*, unsigned int, clang::Scope*, bool, clang::SourceLocation) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x639306e)
 #<!-- -->8 0x000000000673501a clang::Sema::LookupBuiltin(clang::LookupResult&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x673501a)
 #<!-- -->9 0x000000000673f369 LookupDirect(clang::Sema&amp;, clang::LookupResult&amp;, clang::DeclContext const*) SemaLookup.cpp:0:0
#<!-- -->10 0x000000000673f509 CppNamespaceLookup(clang::Sema&amp;, clang::LookupResult&amp;, clang::ASTContext&amp;, clang::DeclContext const*, (anonymous namespace)::UnqualUsingDirectiveSet&amp;) (.isra.0) SemaLookup.cpp:0:0
#<!-- -->11 0x000000000672dd5b clang::Sema::CppLookupName(clang::LookupResult&amp;, clang::Scope*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x672dd5b)
#<!-- -->12 0x000000000672ecc5 clang::Sema::LookupName(clang::LookupResult&amp;, clang::Scope*, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x672ecc5)
#<!-- -->13 0x00000000063d75cd clang::Sema::getTypeName(clang::IdentifierInfo const&amp;, clang::SourceLocation, clang::Scope*, clang::CXXScopeSpec*, bool, bool, clang::OpaquePtr&lt;clang::QualType&gt;, bool, bool, bool, clang::ImplicitTypenameContext, clang::IdentifierInfo**) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x63d75cd)
#<!-- -->14 0x0000000006003cbd clang::Parser::TryAnnotateTypeOrScopeTokenAfterScopeSpec(clang::CXXScopeSpec&amp;, bool, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6003cbd)
#<!-- -->15 0x0000000006004014 clang::Parser::TryAnnotateTypeOrScopeToken(clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6004014)
#<!-- -->16 0x0000000006086fbe clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6086fbe)
#<!-- -->17 0x0000000006088faa clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6088faa)
#<!-- -->18 0x0000000006089179 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6089179)
#<!-- -->19 0x00000000060416b8 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x60416b8)
#<!-- -->20 0x000000000605743c clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x605743c)
#<!-- -->21 0x0000000006005d77 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6005d77)
#<!-- -->22 0x0000000006006b8e clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6006b8e)
#<!-- -->23 0x0000000006010492 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6010492)
#<!-- -->24 0x0000000006011d28 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6011d28)
#<!-- -->25 0x0000000006012270 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6012270)
#<!-- -->26 0x0000000006000393 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6000393)
#<!-- -->27 0x00000000044614b5 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x44614b5)
#<!-- -->28 0x0000000004737121 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4737121)
#<!-- -->29 0x00000000046b777b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x46b777b)
#<!-- -->30 0x00000000048229e3 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x48229e3)
#<!-- -->31 0x0000000000d50621 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xd50621)
#<!-- -->32 0x0000000000d48e2d ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->33 0x00000000044acc99 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->34 0x0000000003a5dac3 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3a5dac3)
#<!-- -->35 0x00000000044aceb9 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->36 0x00000000044710dd clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x44710dd)
#<!-- -->37 0x0000000004472071 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4472071)
#<!-- -->38 0x000000000447c3fc clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x447c3fc)
#<!-- -->39 0x0000000000d4d2b1 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xd4d2b1)
#<!-- -->40 0x0000000000c12464 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc12464)
#<!-- -->41 0x00007187ba029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->42 0x00007187ba029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->43 0x0000000000d488c5 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xd488c5)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```

Presumably, that builtin shouldn't be declared for armv7?

</details>


---

