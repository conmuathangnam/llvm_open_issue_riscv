# ICE with explicit lambda template template parameters

**Author:** stanford-scs
**URL:** https://github.com/llvm/llvm-project/issues/168969
**Status:** Closed
**Labels:** clang:frontend, obsolete, crash, lambda
**Closed Date:** 2025-11-21T00:50:53Z

## Body

This code generates an internal compiler error with clang 20.1.8.  I won't bother attaching the preprocessed source and other details because the example is completely self-contained and easy to reproduce.  I place this code in the public domain in case you want to incorporate it into a test suite.

```
// clang++ -std=c++20 -c clangice.cc

template<typename T>
using InnerType =
    decltype([]<template<typename> typename Trans, typename Inner>(
                 Trans<Inner> *) -> Inner {
      throw;
    }(static_cast<T *>(nullptr)));

template<typename T> struct Template {};

static_assert(InnerType<Template<bool>>{true});
```

Mostly for the benefit of search indexes, here's the compiler output:

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: clang++ -std=c++20 -c clangice.cc
1.	clangice.cc:12:15: at annotation token
 #0 0x00007fc623eea01d llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/usr/lib/libLLVM.so.20.1+0x8ea01d)
 #1 0x00007fc623ee7b77 llvm::sys::CleanupOnSignal(unsigned long) (/usr/lib/libLLVM.so.20.1+0x8e7b77)
 #2 0x00007fc623dcbb29 (/usr/lib/libLLVM.so.20.1+0x7cbb29)
 #3 0x00007fc622e3e540 (/usr/lib/libc.so.6+0x3e540)
 #4 0x00007fc62d85ed31 (/usr/lib/libclang-cpp.so.20.1+0xa5ed31)
 #5 0x00007fc62d85ee34 clang::CXXRecordDecl::getLambdaCallOperator() const (/usr/lib/libclang-cpp.so.20.1+0xa5ee34)
 #6 0x00007fc62e7ad8f1 clang::Sema::getTemplateInstantiationArgs(clang::NamedDecl const*, clang::DeclContext const*, bool, std::optional<llvm::ArrayRef<clang::TemplateArgument>>, bool, clang::FunctionDecl const*, bool, bool, bool) (/usr/lib/libclang-cpp.so.20.1+0x19ad8f1)
 #7 0x00007fc62e6c964f clang::Sema::CheckTemplateArgumentList(clang::TemplateDecl*, clang::SourceLocation, clang::TemplateArgumentListInfo&, clang::DefaultArguments const&, bool, clang::Sema::CheckTemplateArgumentInfo&, bool, bool*) (/usr/lib/libclang-cpp.so.20.1+0x18c964f)
 #8 0x00007fc62e6cb12b clang::Sema::CheckTemplateIdType(clang::TemplateName, clang::SourceLocation, clang::TemplateArgumentListInfo&) (/usr/lib/libclang-cpp.so.20.1+0x18cb12b)
 #9 0x00007fc62e800c46 (/usr/lib/libclang-cpp.so.20.1+0x1a00c46)
#10 0x00007fc62e806fa1 (/usr/lib/libclang-cpp.so.20.1+0x1a06fa1)
#11 0x00007fc62e80e554 (/usr/lib/libclang-cpp.so.20.1+0x1a0e554)
#12 0x00007fc62e80a92a (/usr/lib/libclang-cpp.so.20.1+0x1a0a92a)
#13 0x00007fc62e7a909e (/usr/lib/libclang-cpp.so.20.1+0x19a909e)
#14 0x00007fc62e7a9208 (/usr/lib/libclang-cpp.so.20.1+0x19a9208)
#15 0x00007fc62e7dd6b8 clang::Sema::SubstParmVarDecl(clang::ParmVarDecl*, clang::MultiLevelTemplateArgumentList const&, int, std::optional<unsigned int>, bool, bool) (/usr/lib/libclang-cpp.so.20.1+0x19dd6b8)
#16 0x00007fc62e7e3ff0 (/usr/lib/libclang-cpp.so.20.1+0x19e3ff0)
#17 0x00007fc62e80abaf (/usr/lib/libclang-cpp.so.20.1+0x1a0abaf)
#18 0x00007fc62e805812 (/usr/lib/libclang-cpp.so.20.1+0x1a05812)
#19 0x00007fc62e7e2c1c (/usr/lib/libclang-cpp.so.20.1+0x19e2c1c)
#20 0x00007fc62e7e764a (/usr/lib/libclang-cpp.so.20.1+0x19e764a)
#21 0x00007fc62e7f89d8 (/usr/lib/libclang-cpp.so.20.1+0x19f89d8)
#22 0x00007fc62e805f07 (/usr/lib/libclang-cpp.so.20.1+0x1a05f07)
#23 0x00007fc62e7a909e (/usr/lib/libclang-cpp.so.20.1+0x19a909e)
#24 0x00007fc62e7a9b3b (/usr/lib/libclang-cpp.so.20.1+0x19a9b3b)
#25 0x00007fc62e7a9c1d clang::Sema::SubstType(clang::QualType, clang::MultiLevelTemplateArgumentList const&, clang::SourceLocation, clang::DeclarationName, bool*) (/usr/lib/libclang-cpp.so.20.1+0x19a9c1d)
#26 0x00007fc62e6cbade clang::Sema::CheckTemplateIdType(clang::TemplateName, clang::SourceLocation, clang::TemplateArgumentListInfo&) (/usr/lib/libclang-cpp.so.20.1+0x18cbade)
#27 0x00007fc62e6cd58e clang::Sema::ActOnTemplateIdType(clang::Scope*, clang::CXXScopeSpec&, clang::SourceLocation, clang::OpaquePtr<clang::TemplateName>, clang::IdentifierInfo const*, clang::SourceLocation, clang::SourceLocation, llvm::MutableArrayRef<clang::ParsedTemplateArgument>, clang::SourceLocation, bool, bool, clang::ImplicitTypenameContext) (/usr/lib/libclang-cpp.so.20.1+0x18cd58e)
#28 0x00007fc62d5e59e4 clang::Parser::AnnotateTemplateIdTokenAsType(clang::CXXScopeSpec&, clang::ImplicitTypenameContext, bool) (/usr/lib/libclang-cpp.so.20.1+0x7e59e4)
#29 0x00007fc62d5f120e clang::Parser::TryAnnotateTypeOrScopeTokenAfterScopeSpec(clang::CXXScopeSpec&, bool, clang::ImplicitTypenameContext) (/usr/lib/libclang-cpp.so.20.1+0x7f120e)
#30 0x00007fc62d5f16e3 clang::Parser::TryAnnotateTypeOrScopeToken(clang::ImplicitTypenameContext) (/usr/lib/libclang-cpp.so.20.1+0x7f16e3)
#31 0x00007fc62d52692c clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) (/usr/lib/libclang-cpp.so.20.1+0x72692c)
#32 0x00007fc62d527625 (/usr/lib/libclang-cpp.so.20.1+0x727625)
#33 0x00007fc62d52eb8b clang::Parser::ParseConstantExpressionInExprEvalContext(clang::Parser::TypeCastState) (/usr/lib/libclang-cpp.so.20.1+0x72eb8b)
#34 0x00007fc62d52ed89 clang::Parser::ParseStaticAssertDeclaration(clang::SourceLocation&) (/usr/lib/libclang-cpp.so.20.1+0x72ed89)
#35 0x00007fc62d564f37 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/usr/lib/libclang-cpp.so.20.1+0x764f37)
#36 0x00007fc62d600356 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/usr/lib/libclang-cpp.so.20.1+0x800356)
#37 0x00007fc62d600eab clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/usr/lib/libclang-cpp.so.20.1+0x800eab)
#38 0x00007fc62d5074a9 clang::ParseAST(clang::Sema&, bool, bool) (/usr/lib/libclang-cpp.so.20.1+0x7074a9)
#39 0x00007fc62f69f23c clang::FrontendAction::Execute() (/usr/lib/libclang-cpp.so.20.1+0x289f23c)
#40 0x00007fc62f625c57 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/usr/lib/libclang-cpp.so.20.1+0x2825c57)
#41 0x00007fc62f70742f clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/lib/libclang-cpp.so.20.1+0x290742f)
#42 0x000055a447483a03 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/usr/bin/clang+++0x14a03)
#43 0x000055a447487781 (/usr/bin/clang+++0x18781)
#44 0x00007fc62f248205 (/usr/lib/libclang-cpp.so.20.1+0x2448205)
#45 0x00007fc623dcbf88 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/usr/lib/libLLVM.so.20.1+0x7cbf88)
#46 0x00007fc62f252a15 (/usr/lib/libclang-cpp.so.20.1+0x2452a15)
#47 0x00007fc62f27fcfb clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/usr/lib/libclang-cpp.so.20.1+0x247fcfb)
#48 0x00007fc62f2800ec clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/usr/lib/libclang-cpp.so.20.1+0x24800ec)
#49 0x00007fc62f284895 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/usr/lib/libclang-cpp.so.20.1+0x2484895)
#50 0x000055a4474899e4 clang_main(int, char**, llvm::ToolContext const&) (/usr/bin/clang+++0x1a9e4)
#51 0x000055a44747a1e0 main (/usr/bin/clang+++0xb1e0)
#52 0x00007fc622e27675 (/usr/lib/libc.so.6+0x27675)
#53 0x00007fc622e27729 __libc_start_main (/usr/lib/libc.so.6+0x27729)
#54 0x000055a44747a245 _start (/usr/bin/clang+++0xb245)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
clang version 20.1.8
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
clang++: note: diagnostic msg: 
```

## Comments

### Comment 1 - EugeneZelenko

Could you please try 21 or `main` branch? https://godbolt.org should be helpful. Only most recent release is maintained.

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (stanford-scs)

<details>
This code generates an internal compiler error with clang 20.1.8.  I won't bother attaching the preprocessed source and other details because the example is completely self-contained and easy to reproduce.  I place this code in the public domain in case you want to incorporate it into a test suite.

```
// clang++ -std=c++20 -c clangice.cc

template&lt;typename T&gt;
using InnerType =
    decltype([]&lt;template&lt;typename&gt; typename Trans, typename Inner&gt;(
                 Trans&lt;Inner&gt; *) -&gt; Inner {
      throw;
    }(static_cast&lt;T *&gt;(nullptr)));

template&lt;typename T&gt; struct Template {};

static_assert(InnerType&lt;Template&lt;bool&gt;&gt;{true});
```

Mostly for the benefit of search indexes, here's the compiler output:

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: clang++ -std=c++20 -c clangice.cc
1.	clangice.cc:12:15: at annotation token
 #<!-- -->0 0x00007fc623eea01d llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/usr/lib/libLLVM.so.20.1+0x8ea01d)
 #<!-- -->1 0x00007fc623ee7b77 llvm::sys::CleanupOnSignal(unsigned long) (/usr/lib/libLLVM.so.20.1+0x8e7b77)
 #<!-- -->2 0x00007fc623dcbb29 (/usr/lib/libLLVM.so.20.1+0x7cbb29)
 #<!-- -->3 0x00007fc622e3e540 (/usr/lib/libc.so.6+0x3e540)
 #<!-- -->4 0x00007fc62d85ed31 (/usr/lib/libclang-cpp.so.20.1+0xa5ed31)
 #<!-- -->5 0x00007fc62d85ee34 clang::CXXRecordDecl::getLambdaCallOperator() const (/usr/lib/libclang-cpp.so.20.1+0xa5ee34)
 #<!-- -->6 0x00007fc62e7ad8f1 clang::Sema::getTemplateInstantiationArgs(clang::NamedDecl const*, clang::DeclContext const*, bool, std::optional&lt;llvm::ArrayRef&lt;clang::TemplateArgument&gt;&gt;, bool, clang::FunctionDecl const*, bool, bool, bool) (/usr/lib/libclang-cpp.so.20.1+0x19ad8f1)
 #<!-- -->7 0x00007fc62e6c964f clang::Sema::CheckTemplateArgumentList(clang::TemplateDecl*, clang::SourceLocation, clang::TemplateArgumentListInfo&amp;, clang::DefaultArguments const&amp;, bool, clang::Sema::CheckTemplateArgumentInfo&amp;, bool, bool*) (/usr/lib/libclang-cpp.so.20.1+0x18c964f)
 #<!-- -->8 0x00007fc62e6cb12b clang::Sema::CheckTemplateIdType(clang::TemplateName, clang::SourceLocation, clang::TemplateArgumentListInfo&amp;) (/usr/lib/libclang-cpp.so.20.1+0x18cb12b)
 #<!-- -->9 0x00007fc62e800c46 (/usr/lib/libclang-cpp.so.20.1+0x1a00c46)
#<!-- -->10 0x00007fc62e806fa1 (/usr/lib/libclang-cpp.so.20.1+0x1a06fa1)
#<!-- -->11 0x00007fc62e80e554 (/usr/lib/libclang-cpp.so.20.1+0x1a0e554)
#<!-- -->12 0x00007fc62e80a92a (/usr/lib/libclang-cpp.so.20.1+0x1a0a92a)
#<!-- -->13 0x00007fc62e7a909e (/usr/lib/libclang-cpp.so.20.1+0x19a909e)
#<!-- -->14 0x00007fc62e7a9208 (/usr/lib/libclang-cpp.so.20.1+0x19a9208)
#<!-- -->15 0x00007fc62e7dd6b8 clang::Sema::SubstParmVarDecl(clang::ParmVarDecl*, clang::MultiLevelTemplateArgumentList const&amp;, int, std::optional&lt;unsigned int&gt;, bool, bool) (/usr/lib/libclang-cpp.so.20.1+0x19dd6b8)
#<!-- -->16 0x00007fc62e7e3ff0 (/usr/lib/libclang-cpp.so.20.1+0x19e3ff0)
#<!-- -->17 0x00007fc62e80abaf (/usr/lib/libclang-cpp.so.20.1+0x1a0abaf)
#<!-- -->18 0x00007fc62e805812 (/usr/lib/libclang-cpp.so.20.1+0x1a05812)
#<!-- -->19 0x00007fc62e7e2c1c (/usr/lib/libclang-cpp.so.20.1+0x19e2c1c)
#<!-- -->20 0x00007fc62e7e764a (/usr/lib/libclang-cpp.so.20.1+0x19e764a)
#<!-- -->21 0x00007fc62e7f89d8 (/usr/lib/libclang-cpp.so.20.1+0x19f89d8)
#<!-- -->22 0x00007fc62e805f07 (/usr/lib/libclang-cpp.so.20.1+0x1a05f07)
#<!-- -->23 0x00007fc62e7a909e (/usr/lib/libclang-cpp.so.20.1+0x19a909e)
#<!-- -->24 0x00007fc62e7a9b3b (/usr/lib/libclang-cpp.so.20.1+0x19a9b3b)
#<!-- -->25 0x00007fc62e7a9c1d clang::Sema::SubstType(clang::QualType, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceLocation, clang::DeclarationName, bool*) (/usr/lib/libclang-cpp.so.20.1+0x19a9c1d)
#<!-- -->26 0x00007fc62e6cbade clang::Sema::CheckTemplateIdType(clang::TemplateName, clang::SourceLocation, clang::TemplateArgumentListInfo&amp;) (/usr/lib/libclang-cpp.so.20.1+0x18cbade)
#<!-- -->27 0x00007fc62e6cd58e clang::Sema::ActOnTemplateIdType(clang::Scope*, clang::CXXScopeSpec&amp;, clang::SourceLocation, clang::OpaquePtr&lt;clang::TemplateName&gt;, clang::IdentifierInfo const*, clang::SourceLocation, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::ParsedTemplateArgument&gt;, clang::SourceLocation, bool, bool, clang::ImplicitTypenameContext) (/usr/lib/libclang-cpp.so.20.1+0x18cd58e)
#<!-- -->28 0x00007fc62d5e59e4 clang::Parser::AnnotateTemplateIdTokenAsType(clang::CXXScopeSpec&amp;, clang::ImplicitTypenameContext, bool) (/usr/lib/libclang-cpp.so.20.1+0x7e59e4)
#<!-- -->29 0x00007fc62d5f120e clang::Parser::TryAnnotateTypeOrScopeTokenAfterScopeSpec(clang::CXXScopeSpec&amp;, bool, clang::ImplicitTypenameContext) (/usr/lib/libclang-cpp.so.20.1+0x7f120e)
#<!-- -->30 0x00007fc62d5f16e3 clang::Parser::TryAnnotateTypeOrScopeToken(clang::ImplicitTypenameContext) (/usr/lib/libclang-cpp.so.20.1+0x7f16e3)
#<!-- -->31 0x00007fc62d52692c clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) (/usr/lib/libclang-cpp.so.20.1+0x72692c)
#<!-- -->32 0x00007fc62d527625 (/usr/lib/libclang-cpp.so.20.1+0x727625)
#<!-- -->33 0x00007fc62d52eb8b clang::Parser::ParseConstantExpressionInExprEvalContext(clang::Parser::TypeCastState) (/usr/lib/libclang-cpp.so.20.1+0x72eb8b)
#<!-- -->34 0x00007fc62d52ed89 clang::Parser::ParseStaticAssertDeclaration(clang::SourceLocation&amp;) (/usr/lib/libclang-cpp.so.20.1+0x72ed89)
#<!-- -->35 0x00007fc62d564f37 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/usr/lib/libclang-cpp.so.20.1+0x764f37)
#<!-- -->36 0x00007fc62d600356 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/usr/lib/libclang-cpp.so.20.1+0x800356)
#<!-- -->37 0x00007fc62d600eab clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/usr/lib/libclang-cpp.so.20.1+0x800eab)
#<!-- -->38 0x00007fc62d5074a9 clang::ParseAST(clang::Sema&amp;, bool, bool) (/usr/lib/libclang-cpp.so.20.1+0x7074a9)
#<!-- -->39 0x00007fc62f69f23c clang::FrontendAction::Execute() (/usr/lib/libclang-cpp.so.20.1+0x289f23c)
#<!-- -->40 0x00007fc62f625c57 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/usr/lib/libclang-cpp.so.20.1+0x2825c57)
#<!-- -->41 0x00007fc62f70742f clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/lib/libclang-cpp.so.20.1+0x290742f)
#<!-- -->42 0x000055a447483a03 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/usr/bin/clang+++0x14a03)
#<!-- -->43 0x000055a447487781 (/usr/bin/clang+++0x18781)
#<!-- -->44 0x00007fc62f248205 (/usr/lib/libclang-cpp.so.20.1+0x2448205)
#<!-- -->45 0x00007fc623dcbf88 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/usr/lib/libLLVM.so.20.1+0x7cbf88)
#<!-- -->46 0x00007fc62f252a15 (/usr/lib/libclang-cpp.so.20.1+0x2452a15)
#<!-- -->47 0x00007fc62f27fcfb clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/usr/lib/libclang-cpp.so.20.1+0x247fcfb)
#<!-- -->48 0x00007fc62f2800ec clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/usr/lib/libclang-cpp.so.20.1+0x24800ec)
#<!-- -->49 0x00007fc62f284895 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/usr/lib/libclang-cpp.so.20.1+0x2484895)
#<!-- -->50 0x000055a4474899e4 clang_main(int, char**, llvm::ToolContext const&amp;) (/usr/bin/clang+++0x1a9e4)
#<!-- -->51 0x000055a44747a1e0 main (/usr/bin/clang+++0xb1e0)
#<!-- -->52 0x00007fc622e27675 (/usr/lib/libc.so.6+0x27675)
#<!-- -->53 0x00007fc622e27729 __libc_start_main (/usr/lib/libc.so.6+0x27729)
#<!-- -->54 0x000055a44747a245 _start (/usr/bin/clang+++0xb245)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
clang version 20.1.8
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
clang++: note: diagnostic msg: 
```
</details>


---

### Comment 3 - stanford-scs

Yeah, works with 21.1.0, still broken with 20.1.0.  If 20 is no longer maintained then you can close this bug.

[godbolt](https://godbolt.org/#g:!((g:!((g:!((h:codeEditor,i:(filename:'1',fontScale:14,fontUsePx:'0',j:1,lang:c%2B%2B,selection:(endColumn:48,endLineNumber:12,positionColumn:48,positionLineNumber:12,selectionStartColumn:48,selectionStartLineNumber:12,startColumn:48,startLineNumber:12),source:'//+clang%2B%2B+-std%3Dc%2B%2B20+-c+clangice.cc%0A%0Atemplate%3Ctypename+T%3E%0Ausing+InnerType+%3D%0A++++decltype(%5B%5D%3Ctemplate%3Ctypename%3E+typename+Trans,+typename+Inner%3E(%0A+++++++++++++++++Trans%3CInner%3E+*)+-%3E+Inner+%7B%0A++++++throw%3B%0A++++%7D(static_cast%3CT+*%3E(nullptr)))%3B%0A%0Atemplate%3Ctypename+T%3E+struct+Template+%7B%7D%3B%0A%0Astatic_assert(InnerType%3CTemplate%3Cbool%3E%3E%7Btrue%7D)%3B'),l:'5',n:'0',o:'C%2B%2B+source+%231',t:'0')),k:33.333333333333336,l:'4',n:'0',o:'',s:0,t:'0'),(g:!((h:compiler,i:(compiler:clang2010,filters:(b:'0',binary:'1',binaryObject:'1',commentOnly:'0',debugCalls:'1',demangle:'0',directives:'0',execute:'1',intel:'0',libraryCode:'0',trim:'1',verboseDemangling:'0'),flagsViewOpen:'1',fontScale:14,fontUsePx:'0',j:1,lang:c%2B%2B,libs:!(),options:'-std%3Dc%2B%2B20',overrides:!(),selection:(endColumn:1,endLineNumber:1,positionColumn:1,positionLineNumber:1,selectionStartColumn:1,selectionStartLineNumber:1,startColumn:1,startLineNumber:1),source:1),l:'5',n:'0',o:'+x86-64+clang+20.1.0+(Editor+%231)',t:'0')),k:33.333333333333336,l:'4',n:'0',o:'',s:0,t:'0'),(g:!((h:output,i:(compilerName:'x86-64+clang+20.1.0',editorid:1,fontScale:14,fontUsePx:'0',j:1,wrap:'1'),l:'5',n:'0',o:'Output+of+x86-64+clang+20.1.0+(Compiler+%231)',t:'0')),k:33.33333333333333,l:'4',n:'0',o:'',s:0,t:'0')),l:'2',n:'0',o:'',t:'0')),version:4)

---

