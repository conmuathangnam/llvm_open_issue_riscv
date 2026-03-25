# [clangd] SIGSEGV when replaying specific LSP sequence in Decl.h and ExternalASTSource.h (affiliated with reprodution scripts)

**Author:** hongtaihu
**URL:** https://github.com/llvm/llvm-project/issues/184820
**Status:** Open
**Labels:** clangd, crash

## Body

There are two crashes triggered by a specific LSP message sequence.(Geranted by a Fuzzer)
Their patterns look like that of https://github.com/llvm/llvm-project/issues/180403, maybe they share similar root causes which involves race conditions under multi-thread environment, but I am not certain whether they are duplications or not.


Environment

- clangd commit: https://github.com/llvm/llvm-project/commit/aff5afc48df63615053b2432da198a4932435c3f
- OS: Ubuntu 24.04
- Build: Release
- clangd version 23.0.0git (https://github.com/llvm/llvm-project.git https://github.com/llvm/llvm-project/commit/aff5afc48df63615053b2432da198a4932435c3f)

Steps to Reproduce

in the folder, 

```
python3 replay.py <path/to/clangd> <timeout>
```

## ASAN log

affiliated in reprodution scripts,

There are two types of output under the same input on both Decl.h:2778 and ExternalASTSource.h:363, in short:

```sh
==7476==ERROR: AddressSanitizer: use-after-poison on address 0x521000453fb0 at pc 0x5f506b488062 bp 0x7aba7107aef0 sp 0x7aba7107aee0
READ of size 8 at 0x521000453fb0 thread T12
    #0 0x5f506b488061 in clang::FunctionDecl::parameters() /home/ubuntu2404/llvm-project/clang/include/clang/AST/Decl.h:2778
    #1 0x5f506b488061 in clang::Sema::ActOnStartOfFunctionDef(clang::Scope*, clang::Decl*, clang::SkipBodyInfo*, clang::Sema::FnBodyKind) /home/ubuntu2404/llvm-project/clang/lib/Sema/SemaDecl.cpp:16261
    #2 0x5f506b539f76 in clang::Sema::ActOnStartOfFunctionDef(clang::Scope*, clang::Declarator&, llvm::MutableArrayRef<clang::TemplateParameterList*>, clang::SkipBodyInfo*, clang::Sema::FnBodyKind) /home/ubuntu2404/llvm-project/clang/lib/Sema/SemaDecl.cpp:15923
    #3 0x5f50737bbd2f in clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) /home/ubuntu2404/llvm-project/clang/lib/Parse/Parser.cpp:1332
    #4 0x5f50738dd019 in clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) /home/ubuntu2404/llvm-project/clang/lib/Parse/ParseDecl.cpp:2265
    #5 0x5f507379ca96 in clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) /home/ubuntu2404/llvm-project/clang/lib/Parse/Parser.cpp:1148
    #6 0x5f507379effa in clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) /home/ubuntu2404/llvm-project/clang/lib/Parse/Parser.cpp:1170
    #7 0x5f50737c365f in clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) /home/ubuntu2404/llvm-project/clang/lib/Parse/Parser.cpp:998
    #8 0x5f50737c8421 in clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) /home/ubuntu2404/llvm-project/clang/lib/Parse/Parser.cpp:706
    #9 0x5f507374763d in clang::ParseAST(clang::Sema&, bool, bool) /home/ubuntu2404/llvm-project/clang/lib/Parse/ParseAST.cpp:170
    #10 0x5f507346fe49 in clang::FrontendAction::Execute() /home/ubuntu2404/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1322
    #11 0x5f506d671ed4 in clang::clangd::ParsedAST::build(llvm::StringRef, clang::clangd::ParseInputs const&, std::unique_ptr<clang::CompilerInvocation, std::default_delete<clang::CompilerInvocation> >, llvm::ArrayRef<clang::clangd::Diag>, std::shared_ptr<clang::clangd::PreambleData const>) /home/ubuntu2404/llvm-project/clang-tools-extra/clangd/ParsedAST.cpp:714
    #12 0x5f506d934384 in generateDiagnostics /home/ubuntu2404/llvm-project/clang-tools-extra/clangd/TUScheduler.cpp:1214
    #13 0x5f506d9377db in operator() /home/ubuntu2404/llvm-project/clang-tools-extra/clangd/TUScheduler.cpp:1147
    #14 0x5f506d938176 in operator() /home/ubuntu2404/llvm-project/clang-tools-extra/clangd/TUScheduler.cpp:1135
    #15 0x5f506d8eca0f in llvm::unique_function<void ()>::operator()() /home/ubuntu2404/llvm-project/llvm/include/llvm/ADT/FunctionExtras.h:364
    #16 0x5f506d8eca0f in void llvm::function_ref<void ()>::callback_fn<llvm::unique_function<void ()> >(long) /home/ubuntu2404/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46
    #17 0x5f506d8fdbf0 in llvm::function_ref<void ()>::operator()() const /home/ubuntu2404/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69
    #18 0x5f506d8fdbf0 in runTask /home/ubuntu2404/llvm-project/clang-tools-extra/clangd/TUScheduler.cpp:1327
    #19 0x5f506d923eb4 in run /home/ubuntu2404/llvm-project/clang-tools-extra/clangd/TUScheduler.cpp:1461
    #20 0x5f506df32ff8 in llvm::unique_function<void ()>::operator()() /home/ubuntu2404/llvm-project/llvm/include/llvm/ADT/FunctionExtras.h:364
    #21 0x5f506df32ff8 in operator() /home/ubuntu2404/llvm-project/clang-tools-extra/clangd/support/Threading.cpp:101
    #22 0x5f506df32ff8 in operator()<clang::clangd::AsyncTaskRunner::runAsync(const llvm::Twine&, llvm::unique_function<void()>)::<lambda()>&> /home/ubuntu2404/llvm-project/llvm/include/llvm/Support/thread.h:46
    #23 0x5f506df32ff8 in __invoke_impl<void, llvm::thread::GenericThreadProxy<std::tuple<clang::clangd::AsyncTaskRunner::runAsync(const llvm::Twine&, llvm::unique_function<void()>)::<lambda()> > >(void*)::<lambda(auto:5&&, auto:6&& ...)>, clang::clangd::AsyncTaskRunner::runAsync(const llvm::Twine&, llvm::unique_function<void()>)::<lambda()>&> /usr/include/c++/13/bits/invoke.h:61
    #24 0x5f506df32ff8 in __invoke<llvm::thread::GenericThreadProxy<std::tuple<clang::clangd::AsyncTaskRunner::runAsync(const llvm::Twine&, llvm::unique_function<void()>)::<lambda()> > >(void*)::<lambda(auto:5&&, auto:6&& ...)>, clang::clangd::AsyncTaskRunner::runAsync(const llvm::Twine&, llvm::unique_function<void()>)::<lambda()>&> /usr/include/c++/13/bits/invoke.h:96
    #25 0x5f506df32ff8 in __apply_impl<llvm::thread::GenericThreadProxy<std::tuple<clang::clangd::AsyncTaskRunner::runAsync(const llvm::Twine&, llvm::unique_function<void()>)::<lambda()> > >(void*)::<lambda(auto:5&&, auto:6&& ...)>, std::tuple<clang::clangd::AsyncTaskRunner::runAsync(const llvm::Twine&, llvm::unique_function<void()>)::<lambda()> >&, 0> /usr/include/c++/13/tuple:2302
    #26 0x5f506df32ff8 in apply<llvm::thread::GenericThreadProxy<std::tuple<clang::clangd::AsyncTaskRunner::runAsync(const llvm::Twine&, llvm::unique_function<void()>)::<lambda()> > >(void*)::<lambda(auto:5&&, auto:6&& ...)>, std::tuple<clang::clangd::AsyncTaskRunner::runAsync(const llvm::Twine&, llvm::unique_function<void()>)::<lambda()> >&> /usr/include/c++/13/tuple:2313
    #27 0x5f506df32ff8 in GenericThreadProxy<std::tuple<clang::clangd::AsyncTaskRunner::runAsync(const llvm::Twine&, llvm::unique_function<void()>)::<lambda()> > > /home/ubuntu2404/llvm-project/llvm/include/llvm/Support/thread.h:44
    #28 0x5f506df32ff8 in ThreadProxy<std::tuple<clang::clangd::AsyncTaskRunner::runAsync(const llvm::Twine&, llvm::unique_function<void()>)::<lambda()> > > /home/ubuntu2404/llvm-project/llvm/include/llvm/Support/thread.h:62
    #29 0x7aba8125ea41 in asan_thread_start ../../../../src/libsanitizer/asan/asan_interceptors.cpp:234
    #30 0x7aba80a9caa3 in start_thread nptl/pthread_create.c:447
    #31 0x7aba80b29c6b in clone3 ../sysdeps/unix/sysv/linux/x86_64/clone3.S:78

```

```sh
==10347==ERROR: AddressSanitizer: use-after-poison on address 0x5210003e37b8 at pc 0x59a64c204c5e bp 0x7fe3fd27a850 sp 0x7fe3fd27a840
READ of size 1 at 0x5210003e37b8 thread T12
    #0 0x59a64c204c5d in clang::LazyOffsetPtr<clang::Stmt, unsigned long, &clang::ExternalASTSource::GetExternalDeclStmt>::GetLSB() const /home/ubuntu2404/llvm-project/clang/include/clang/AST/ExternalASTSource.h:363
    #1 0x59a64c204c5d in clang::LazyOffsetPtr<clang::Stmt, unsigned long, &clang::ExternalASTSource::GetExternalDeclStmt>::isOffset() const /home/ubuntu2404/llvm-project/clang/include/clang/AST/ExternalASTSource.h:417
    #2 0x59a64c204c5d in clang::LazyOffsetPtr<clang::Stmt, unsigned long, &clang::ExternalASTSource::GetExternalDeclStmt>::operator bool() const /home/ubuntu2404/llvm-project/clang/include/clang/AST/ExternalASTSource.h:409
    #3 0x59a64c204c5d in clang::FunctionDecl::doesThisDeclarationHaveABody() const /home/ubuntu2404/llvm-project/clang/include/clang/AST/Decl.h:2327
    #4 0x59a64c204c5d in clang::FunctionDecl::isThisDeclarationADefinition() const /home/ubuntu2404/llvm-project/clang/include/clang/AST/Decl.h:2316
    #5 0x59a64c204c5d in clang::FunctionDecl::isDefined(clang::FunctionDecl const*&, bool) const /home/ubuntu2404/llvm-project/clang/lib/AST/Decl.cpp:3249
    #6 0x59a64fb36214 in clang::Sema::CheckForFunctionRedefinition(clang::FunctionDecl*, clang::FunctionDecl const*, clang::SkipBodyInfo*) /home/ubuntu2404/llvm-project/clang/lib/Sema/SemaDecl.cpp:16013
    #7 0x59a64fc3b80d in clang::Sema::ActOnStartOfFunctionDef(clang::Scope*, clang::Decl*, clang::SkipBodyInfo*, clang::Sema::FnBodyKind) /home/ubuntu2404/llvm-project/clang/lib/Sema/SemaDecl.cpp:16192
    #8 0x59a64fcedf76 in clang::Sema::ActOnStartOfFunctionDef(clang::Scope*, clang::Declarator&, llvm::MutableArrayRef<clang::TemplateParameterList*>, clang::SkipBodyInfo*, clang::Sema::FnBodyKind) /home/ubuntu2404/llvm-project/clang/lib/Sema/SemaDecl.cpp:15923
    #9 0x59a657f6fd2f in clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) /home/ubuntu2404/llvm-project/clang/lib/Parse/Parser.cpp:1332
    #10 0x59a658091019 in clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) /home/ubuntu2404/llvm-project/clang/lib/Parse/ParseDecl.cpp:2265
    #11 0x59a657f50a96 in clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) /home/ubuntu2404/llvm-project/clang/lib/Parse/Parser.cpp:1148
    #12 0x59a657f52ffa in clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) /home/ubuntu2404/llvm-project/clang/lib/Parse/Parser.cpp:1170
    #13 0x59a657f7765f in clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) /home/ubuntu2404/llvm-project/clang/lib/Parse/Parser.cpp:998
    #14 0x59a657f7c421 in clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) /home/ubuntu2404/llvm-project/clang/lib/Parse/Parser.cpp:706
    #15 0x59a657efb63d in clang::ParseAST(clang::Sema&, bool, bool) /home/ubuntu2404/llvm-project/clang/lib/Parse/ParseAST.cpp:170
    #16 0x59a657c23e49 in clang::FrontendAction::Execute() /home/ubuntu2404/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1322
    #17 0x59a651e25ed4 in clang::clangd::ParsedAST::build(llvm::StringRef, clang::clangd::ParseInputs const&, std::unique_ptr<clang::CompilerInvocation, std::default_delete<clang::CompilerInvocation> >, llvm::ArrayRef<clang::clangd::Diag>, std::shared_ptr<clang::clangd::PreambleData const>) /home/ubuntu2404/llvm-project/clang-tools-extra/clangd/ParsedAST.cpp:714
    #18 0x59a6520e8384 in generateDiagnostics /home/ubuntu2404/llvm-project/clang-tools-extra/clangd/TUScheduler.cpp:1214
    #19 0x59a6520eb7db in operator() /home/ubuntu2404/llvm-project/clang-tools-extra/clangd/TUScheduler.cpp:1147
    #20 0x59a6520ec176 in operator() /home/ubuntu2404/llvm-project/clang-tools-extra/clangd/TUScheduler.cpp:1135
    #21 0x59a6520a0a0f in llvm::unique_function<void ()>::operator()() /home/ubuntu2404/llvm-project/llvm/include/llvm/ADT/FunctionExtras.h:364
    #22 0x59a6520a0a0f in void llvm::function_ref<void ()>::callback_fn<llvm::unique_function<void ()> >(long) /home/ubuntu2404/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46
    #23 0x59a6520b1bf0 in llvm::function_ref<void ()>::operator()() const /home/ubuntu2404/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69
    #24 0x59a6520b1bf0 in runTask /home/ubuntu2404/llvm-project/clang-tools-extra/clangd/TUScheduler.cpp:1327
    #25 0x59a6520d7eb4 in run /home/ubuntu2404/llvm-project/clang-tools-extra/clangd/TUScheduler.cpp:1461
    #26 0x59a6526e6ff8 in llvm::unique_function<void ()>::operator()() /home/ubuntu2404/llvm-project/llvm/include/llvm/ADT/FunctionExtras.h:364
    #27 0x59a6526e6ff8 in operator() /home/ubuntu2404/llvm-project/clang-tools-extra/clangd/support/Threading.cpp:101
    #28 0x59a6526e6ff8 in operator()<clang::clangd::AsyncTaskRunner::runAsync(const llvm::Twine&, llvm::unique_function<void()>)::<lambda()>&> /home/ubuntu2404/llvm-project/llvm/include/llvm/Support/thread.h:46
    #29 0x59a6526e6ff8 in __invoke_impl<void, llvm::thread::GenericThreadProxy<std::tuple<clang::clangd::AsyncTaskRunner::runAsync(const llvm::Twine&, llvm::unique_function<void()>)::<lambda()> > >(void*)::<lambda(auto:5&&, auto:6&& ...)>, clang::clangd::AsyncTaskRunner::runAsync(const llvm::Twine&, llvm::unique_function<void()>)::<lambda()>&> /usr/include/c++/13/bits/invoke.h:61
    #30 0x59a6526e6ff8 in __invoke<llvm::thread::GenericThreadProxy<std::tuple<clang::clangd::AsyncTaskRunner::runAsync(const llvm::Twine&, llvm::unique_function<void()>)::<lambda()> > >(void*)::<lambda(auto:5&&, auto:6&& ...)>, clang::clangd::AsyncTaskRunner::runAsync(const llvm::Twine&, llvm::unique_function<void()>)::<lambda()>&> /usr/include/c++/13/bits/invoke.h:96
    #31 0x59a6526e6ff8 in __apply_impl<llvm::thread::GenericThreadProxy<std::tuple<clang::clangd::AsyncTaskRunner::runAsync(const llvm::Twine&, llvm::unique_function<void()>)::<lambda()> > >(void*)::<lambda(auto:5&&, auto:6&& ...)>, std::tuple<clang::clangd::AsyncTaskRunner::runAsync(const llvm::Twine&, llvm::unique_function<void()>)::<lambda()> >&, 0> /usr/include/c++/13/tuple:2302
    #32 0x59a6526e6ff8 in apply<llvm::thread::GenericThreadProxy<std::tuple<clang::clangd::AsyncTaskRunner::runAsync(const llvm::Twine&, llvm::unique_function<void()>)::<lambda()> > >(void*)::<lambda(auto:5&&, auto:6&& ...)>, std::tuple<clang::clangd::AsyncTaskRunner::runAsync(const llvm::Twine&, llvm::unique_function<void()>)::<lambda()> >&> /usr/include/c++/13/tuple:2313
    #33 0x59a6526e6ff8 in GenericThreadProxy<std::tuple<clang::clangd::AsyncTaskRunner::runAsync(const llvm::Twine&, llvm::unique_function<void()>)::<lambda()> > > /home/ubuntu2404/llvm-project/llvm/include/llvm/Support/thread.h:44
    #34 0x59a6526e6ff8 in ThreadProxy<std::tuple<clang::clangd::AsyncTaskRunner::runAsync(const llvm::Twine&, llvm::unique_function<void()>)::<lambda()> > > /home/ubuntu2404/llvm-project/llvm/include/llvm/Support/thread.h:62
    #35 0x7fe40d45ea41 in asan_thread_start ../../../../src/libsanitizer/asan/asan_interceptors.cpp:234
    #36 0x7fe40cc9caa3 in start_thread nptl/pthread_create.c:447
    #37 0x7fe40cd29c6b in clone3 ../sysdeps/unix/sysv/linux/x86_64/clone3.S:78
```


[archive1.tar.gz](https://github.com/user-attachments/files/25772672/archive1.tar.gz)

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clangd

Author: Macro Terra (hongtaihu)

<details>
There are two crashes triggered by a specific LSP message sequence.(Geranted by a Fuzzer)
Their patterns look like that of https://github.com/llvm/llvm-project/issues/180403, maybe they share similar root causes which involves race conditions under multi-thread environment, but I am not certain whether they are duplications or not.


Environment

- clangd commit: https://github.com/llvm/llvm-project/commit/aff5afc48df63615053b2432da198a4932435c3f
- OS: Ubuntu 24.04
- Build: Release
- clangd version 23.0.0git (https://github.com/llvm/llvm-project.git https://github.com/llvm/llvm-project/commit/aff5afc48df63615053b2432da198a4932435c3f)

Steps to Reproduce

in the folder, 

```
python3 replay.py &lt;path/to/clangd&gt; &lt;timeout&gt;
```

## ASAN log

affiliated in reprodution scripts,

There are two types of output under the same input on both Decl.h:2778 and ExternalASTSource.h:363, in short:

```sh
==7476==ERROR: AddressSanitizer: use-after-poison on address 0x521000453fb0 at pc 0x5f506b488062 bp 0x7aba7107aef0 sp 0x7aba7107aee0
READ of size 8 at 0x521000453fb0 thread T12
    #<!-- -->0 0x5f506b488061 in clang::FunctionDecl::parameters() /home/ubuntu2404/llvm-project/clang/include/clang/AST/Decl.h:2778
    #<!-- -->1 0x5f506b488061 in clang::Sema::ActOnStartOfFunctionDef(clang::Scope*, clang::Decl*, clang::SkipBodyInfo*, clang::Sema::FnBodyKind) /home/ubuntu2404/llvm-project/clang/lib/Sema/SemaDecl.cpp:16261
    #<!-- -->2 0x5f506b539f76 in clang::Sema::ActOnStartOfFunctionDef(clang::Scope*, clang::Declarator&amp;, llvm::MutableArrayRef&lt;clang::TemplateParameterList*&gt;, clang::SkipBodyInfo*, clang::Sema::FnBodyKind) /home/ubuntu2404/llvm-project/clang/lib/Sema/SemaDecl.cpp:15923
    #<!-- -->3 0x5f50737bbd2f in clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) /home/ubuntu2404/llvm-project/clang/lib/Parse/Parser.cpp:1332
    #<!-- -->4 0x5f50738dd019 in clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) /home/ubuntu2404/llvm-project/clang/lib/Parse/ParseDecl.cpp:2265
    #<!-- -->5 0x5f507379ca96 in clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) /home/ubuntu2404/llvm-project/clang/lib/Parse/Parser.cpp:1148
    #<!-- -->6 0x5f507379effa in clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) /home/ubuntu2404/llvm-project/clang/lib/Parse/Parser.cpp:1170
    #<!-- -->7 0x5f50737c365f in clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) /home/ubuntu2404/llvm-project/clang/lib/Parse/Parser.cpp:998
    #<!-- -->8 0x5f50737c8421 in clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) /home/ubuntu2404/llvm-project/clang/lib/Parse/Parser.cpp:706
    #<!-- -->9 0x5f507374763d in clang::ParseAST(clang::Sema&amp;, bool, bool) /home/ubuntu2404/llvm-project/clang/lib/Parse/ParseAST.cpp:170
    #<!-- -->10 0x5f507346fe49 in clang::FrontendAction::Execute() /home/ubuntu2404/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1322
    #<!-- -->11 0x5f506d671ed4 in clang::clangd::ParsedAST::build(llvm::StringRef, clang::clangd::ParseInputs const&amp;, std::unique_ptr&lt;clang::CompilerInvocation, std::default_delete&lt;clang::CompilerInvocation&gt; &gt;, llvm::ArrayRef&lt;clang::clangd::Diag&gt;, std::shared_ptr&lt;clang::clangd::PreambleData const&gt;) /home/ubuntu2404/llvm-project/clang-tools-extra/clangd/ParsedAST.cpp:714
    #<!-- -->12 0x5f506d934384 in generateDiagnostics /home/ubuntu2404/llvm-project/clang-tools-extra/clangd/TUScheduler.cpp:1214
    #<!-- -->13 0x5f506d9377db in operator() /home/ubuntu2404/llvm-project/clang-tools-extra/clangd/TUScheduler.cpp:1147
    #<!-- -->14 0x5f506d938176 in operator() /home/ubuntu2404/llvm-project/clang-tools-extra/clangd/TUScheduler.cpp:1135
    #<!-- -->15 0x5f506d8eca0f in llvm::unique_function&lt;void ()&gt;::operator()() /home/ubuntu2404/llvm-project/llvm/include/llvm/ADT/FunctionExtras.h:364
    #<!-- -->16 0x5f506d8eca0f in void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;llvm::unique_function&lt;void ()&gt; &gt;(long) /home/ubuntu2404/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46
    #<!-- -->17 0x5f506d8fdbf0 in llvm::function_ref&lt;void ()&gt;::operator()() const /home/ubuntu2404/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69
    #<!-- -->18 0x5f506d8fdbf0 in runTask /home/ubuntu2404/llvm-project/clang-tools-extra/clangd/TUScheduler.cpp:1327
    #<!-- -->19 0x5f506d923eb4 in run /home/ubuntu2404/llvm-project/clang-tools-extra/clangd/TUScheduler.cpp:1461
    #<!-- -->20 0x5f506df32ff8 in llvm::unique_function&lt;void ()&gt;::operator()() /home/ubuntu2404/llvm-project/llvm/include/llvm/ADT/FunctionExtras.h:364
    #<!-- -->21 0x5f506df32ff8 in operator() /home/ubuntu2404/llvm-project/clang-tools-extra/clangd/support/Threading.cpp:101
    #<!-- -->22 0x5f506df32ff8 in operator()&lt;clang::clangd::AsyncTaskRunner::runAsync(const llvm::Twine&amp;, llvm::unique_function&lt;void()&gt;)::&lt;lambda()&gt;&amp;&gt; /home/ubuntu2404/llvm-project/llvm/include/llvm/Support/thread.h:46
    #<!-- -->23 0x5f506df32ff8 in __invoke_impl&lt;void, llvm::thread::GenericThreadProxy&lt;std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(const llvm::Twine&amp;, llvm::unique_function&lt;void()&gt;)::&lt;lambda()&gt; &gt; &gt;(void*)::&lt;lambda(auto:5&amp;&amp;, auto:6&amp;&amp; ...)&gt;, clang::clangd::AsyncTaskRunner::runAsync(const llvm::Twine&amp;, llvm::unique_function&lt;void()&gt;)::&lt;lambda()&gt;&amp;&gt; /usr/include/c++/13/bits/invoke.h:61
    #<!-- -->24 0x5f506df32ff8 in __invoke&lt;llvm::thread::GenericThreadProxy&lt;std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(const llvm::Twine&amp;, llvm::unique_function&lt;void()&gt;)::&lt;lambda()&gt; &gt; &gt;(void*)::&lt;lambda(auto:5&amp;&amp;, auto:6&amp;&amp; ...)&gt;, clang::clangd::AsyncTaskRunner::runAsync(const llvm::Twine&amp;, llvm::unique_function&lt;void()&gt;)::&lt;lambda()&gt;&amp;&gt; /usr/include/c++/13/bits/invoke.h:96
    #<!-- -->25 0x5f506df32ff8 in __apply_impl&lt;llvm::thread::GenericThreadProxy&lt;std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(const llvm::Twine&amp;, llvm::unique_function&lt;void()&gt;)::&lt;lambda()&gt; &gt; &gt;(void*)::&lt;lambda(auto:5&amp;&amp;, auto:6&amp;&amp; ...)&gt;, std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(const llvm::Twine&amp;, llvm::unique_function&lt;void()&gt;)::&lt;lambda()&gt; &gt;&amp;, 0&gt; /usr/include/c++/13/tuple:2302
    #<!-- -->26 0x5f506df32ff8 in apply&lt;llvm::thread::GenericThreadProxy&lt;std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(const llvm::Twine&amp;, llvm::unique_function&lt;void()&gt;)::&lt;lambda()&gt; &gt; &gt;(void*)::&lt;lambda(auto:5&amp;&amp;, auto:6&amp;&amp; ...)&gt;, std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(const llvm::Twine&amp;, llvm::unique_function&lt;void()&gt;)::&lt;lambda()&gt; &gt;&amp;&gt; /usr/include/c++/13/tuple:2313
    #<!-- -->27 0x5f506df32ff8 in GenericThreadProxy&lt;std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(const llvm::Twine&amp;, llvm::unique_function&lt;void()&gt;)::&lt;lambda()&gt; &gt; &gt; /home/ubuntu2404/llvm-project/llvm/include/llvm/Support/thread.h:44
    #<!-- -->28 0x5f506df32ff8 in ThreadProxy&lt;std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(const llvm::Twine&amp;, llvm::unique_function&lt;void()&gt;)::&lt;lambda()&gt; &gt; &gt; /home/ubuntu2404/llvm-project/llvm/include/llvm/Support/thread.h:62
    #<!-- -->29 0x7aba8125ea41 in asan_thread_start ../../../../src/libsanitizer/asan/asan_interceptors.cpp:234
    #<!-- -->30 0x7aba80a9caa3 in start_thread nptl/pthread_create.c:447
    #<!-- -->31 0x7aba80b29c6b in clone3 ../sysdeps/unix/sysv/linux/x86_64/clone3.S:78

```

```sh
==10347==ERROR: AddressSanitizer: use-after-poison on address 0x5210003e37b8 at pc 0x59a64c204c5e bp 0x7fe3fd27a850 sp 0x7fe3fd27a840
READ of size 1 at 0x5210003e37b8 thread T12
    #<!-- -->0 0x59a64c204c5d in clang::LazyOffsetPtr&lt;clang::Stmt, unsigned long, &amp;clang::ExternalASTSource::GetExternalDeclStmt&gt;::GetLSB() const /home/ubuntu2404/llvm-project/clang/include/clang/AST/ExternalASTSource.h:363
    #<!-- -->1 0x59a64c204c5d in clang::LazyOffsetPtr&lt;clang::Stmt, unsigned long, &amp;clang::ExternalASTSource::GetExternalDeclStmt&gt;::isOffset() const /home/ubuntu2404/llvm-project/clang/include/clang/AST/ExternalASTSource.h:417
    #<!-- -->2 0x59a64c204c5d in clang::LazyOffsetPtr&lt;clang::Stmt, unsigned long, &amp;clang::ExternalASTSource::GetExternalDeclStmt&gt;::operator bool() const /home/ubuntu2404/llvm-project/clang/include/clang/AST/ExternalASTSource.h:409
    #<!-- -->3 0x59a64c204c5d in clang::FunctionDecl::doesThisDeclarationHaveABody() const /home/ubuntu2404/llvm-project/clang/include/clang/AST/Decl.h:2327
    #<!-- -->4 0x59a64c204c5d in clang::FunctionDecl::isThisDeclarationADefinition() const /home/ubuntu2404/llvm-project/clang/include/clang/AST/Decl.h:2316
    #<!-- -->5 0x59a64c204c5d in clang::FunctionDecl::isDefined(clang::FunctionDecl const*&amp;, bool) const /home/ubuntu2404/llvm-project/clang/lib/AST/Decl.cpp:3249
    #<!-- -->6 0x59a64fb36214 in clang::Sema::CheckForFunctionRedefinition(clang::FunctionDecl*, clang::FunctionDecl const*, clang::SkipBodyInfo*) /home/ubuntu2404/llvm-project/clang/lib/Sema/SemaDecl.cpp:16013
    #<!-- -->7 0x59a64fc3b80d in clang::Sema::ActOnStartOfFunctionDef(clang::Scope*, clang::Decl*, clang::SkipBodyInfo*, clang::Sema::FnBodyKind) /home/ubuntu2404/llvm-project/clang/lib/Sema/SemaDecl.cpp:16192
    #<!-- -->8 0x59a64fcedf76 in clang::Sema::ActOnStartOfFunctionDef(clang::Scope*, clang::Declarator&amp;, llvm::MutableArrayRef&lt;clang::TemplateParameterList*&gt;, clang::SkipBodyInfo*, clang::Sema::FnBodyKind) /home/ubuntu2404/llvm-project/clang/lib/Sema/SemaDecl.cpp:15923
    #<!-- -->9 0x59a657f6fd2f in clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) /home/ubuntu2404/llvm-project/clang/lib/Parse/Parser.cpp:1332
    #<!-- -->10 0x59a658091019 in clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) /home/ubuntu2404/llvm-project/clang/lib/Parse/ParseDecl.cpp:2265
    #<!-- -->11 0x59a657f50a96 in clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) /home/ubuntu2404/llvm-project/clang/lib/Parse/Parser.cpp:1148
    #<!-- -->12 0x59a657f52ffa in clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) /home/ubuntu2404/llvm-project/clang/lib/Parse/Parser.cpp:1170
    #<!-- -->13 0x59a657f7765f in clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) /home/ubuntu2404/llvm-project/clang/lib/Parse/Parser.cpp:998
    #<!-- -->14 0x59a657f7c421 in clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) /home/ubuntu2404/llvm-project/clang/lib/Parse/Parser.cpp:706
    #<!-- -->15 0x59a657efb63d in clang::ParseAST(clang::Sema&amp;, bool, bool) /home/ubuntu2404/llvm-project/clang/lib/Parse/ParseAST.cpp:170
    #<!-- -->16 0x59a657c23e49 in clang::FrontendAction::Execute() /home/ubuntu2404/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1322
    #<!-- -->17 0x59a651e25ed4 in clang::clangd::ParsedAST::build(llvm::StringRef, clang::clangd::ParseInputs const&amp;, std::unique_ptr&lt;clang::CompilerInvocation, std::default_delete&lt;clang::CompilerInvocation&gt; &gt;, llvm::ArrayRef&lt;clang::clangd::Diag&gt;, std::shared_ptr&lt;clang::clangd::PreambleData const&gt;) /home/ubuntu2404/llvm-project/clang-tools-extra/clangd/ParsedAST.cpp:714
    #<!-- -->18 0x59a6520e8384 in generateDiagnostics /home/ubuntu2404/llvm-project/clang-tools-extra/clangd/TUScheduler.cpp:1214
    #<!-- -->19 0x59a6520eb7db in operator() /home/ubuntu2404/llvm-project/clang-tools-extra/clangd/TUScheduler.cpp:1147
    #<!-- -->20 0x59a6520ec176 in operator() /home/ubuntu2404/llvm-project/clang-tools-extra/clangd/TUScheduler.cpp:1135
    #<!-- -->21 0x59a6520a0a0f in llvm::unique_function&lt;void ()&gt;::operator()() /home/ubuntu2404/llvm-project/llvm/include/llvm/ADT/FunctionExtras.h:364
    #<!-- -->22 0x59a6520a0a0f in void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;llvm::unique_function&lt;void ()&gt; &gt;(long) /home/ubuntu2404/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46
    #<!-- -->23 0x59a6520b1bf0 in llvm::function_ref&lt;void ()&gt;::operator()() const /home/ubuntu2404/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69
    #<!-- -->24 0x59a6520b1bf0 in runTask /home/ubuntu2404/llvm-project/clang-tools-extra/clangd/TUScheduler.cpp:1327
    #<!-- -->25 0x59a6520d7eb4 in run /home/ubuntu2404/llvm-project/clang-tools-extra/clangd/TUScheduler.cpp:1461
    #<!-- -->26 0x59a6526e6ff8 in llvm::unique_function&lt;void ()&gt;::operator()() /home/ubuntu2404/llvm-project/llvm/include/llvm/ADT/FunctionExtras.h:364
    #<!-- -->27 0x59a6526e6ff8 in operator() /home/ubuntu2404/llvm-project/clang-tools-extra/clangd/support/Threading.cpp:101
    #<!-- -->28 0x59a6526e6ff8 in operator()&lt;clang::clangd::AsyncTaskRunner::runAsync(const llvm::Twine&amp;, llvm::unique_function&lt;void()&gt;)::&lt;lambda()&gt;&amp;&gt; /home/ubuntu2404/llvm-project/llvm/include/llvm/Support/thread.h:46
    #<!-- -->29 0x59a6526e6ff8 in __invoke_impl&lt;void, llvm::thread::GenericThreadProxy&lt;std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(const llvm::Twine&amp;, llvm::unique_function&lt;void()&gt;)::&lt;lambda()&gt; &gt; &gt;(void*)::&lt;lambda(auto:5&amp;&amp;, auto:6&amp;&amp; ...)&gt;, clang::clangd::AsyncTaskRunner::runAsync(const llvm::Twine&amp;, llvm::unique_function&lt;void()&gt;)::&lt;lambda()&gt;&amp;&gt; /usr/include/c++/13/bits/invoke.h:61
    #<!-- -->30 0x59a6526e6ff8 in __invoke&lt;llvm::thread::GenericThreadProxy&lt;std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(const llvm::Twine&amp;, llvm::unique_function&lt;void()&gt;)::&lt;lambda()&gt; &gt; &gt;(void*)::&lt;lambda(auto:5&amp;&amp;, auto:6&amp;&amp; ...)&gt;, clang::clangd::AsyncTaskRunner::runAsync(const llvm::Twine&amp;, llvm::unique_function&lt;void()&gt;)::&lt;lambda()&gt;&amp;&gt; /usr/include/c++/13/bits/invoke.h:96
    #<!-- -->31 0x59a6526e6ff8 in __apply_impl&lt;llvm::thread::GenericThreadProxy&lt;std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(const llvm::Twine&amp;, llvm::unique_function&lt;void()&gt;)::&lt;lambda()&gt; &gt; &gt;(void*)::&lt;lambda(auto:5&amp;&amp;, auto:6&amp;&amp; ...)&gt;, std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(const llvm::Twine&amp;, llvm::unique_function&lt;void()&gt;)::&lt;lambda()&gt; &gt;&amp;, 0&gt; /usr/include/c++/13/tuple:2302
    #<!-- -->32 0x59a6526e6ff8 in apply&lt;llvm::thread::GenericThreadProxy&lt;std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(const llvm::Twine&amp;, llvm::unique_function&lt;void()&gt;)::&lt;lambda()&gt; &gt; &gt;(void*)::&lt;lambda(auto:5&amp;&amp;, auto:6&amp;&amp; ...)&gt;, std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(const llvm::Twine&amp;, llvm::unique_function&lt;void()&gt;)::&lt;lambda()&gt; &gt;&amp;&gt; /usr/include/c++/13/tuple:2313
    #<!-- -->33 0x59a6526e6ff8 in GenericThreadProxy&lt;std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(const llvm::Twine&amp;, llvm::unique_function&lt;void()&gt;)::&lt;lambda()&gt; &gt; &gt; /home/ubuntu2404/llvm-project/llvm/include/llvm/Support/thread.h:44
    #<!-- -->34 0x59a6526e6ff8 in ThreadProxy&lt;std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(const llvm::Twine&amp;, llvm::unique_function&lt;void()&gt;)::&lt;lambda()&gt; &gt; &gt; /home/ubuntu2404/llvm-project/llvm/include/llvm/Support/thread.h:62
    #<!-- -->35 0x7fe40d45ea41 in asan_thread_start ../../../../src/libsanitizer/asan/asan_interceptors.cpp:234
    #<!-- -->36 0x7fe40cc9caa3 in start_thread nptl/pthread_create.c:447
    #<!-- -->37 0x7fe40cd29c6b in clone3 ../sysdeps/unix/sysv/linux/x86_64/clone3.S:78
```


[archive1.tar.gz](https://github.com/user-attachments/files/25772672/archive1.tar.gz)
</details>


---

### Comment 2 - HighCommander4

A general note on fuzzer-generated bug reports in the clangd component:

If a crash is occurring in parser code (e.g. with `clang::ParseAST` on the stack), it would be helpful (and is likely to lead to the bug report being actioned sooner) to check whether the crash reproduces by providing the same input to `clang` with appropriate flags (and if so, filing the bug in the `clang` component rather than `clangd`).

The vast majority of crashes triggered by clangd in parser code are bugs in the parser itself (with only a small minority being related to clangd's particular ways of using the parser APIs which makes them not reproducible with a `clang` invocation) and are not clangd-specific at all. Clangd in this respect is just a convenient (for fuzzers, I guess) way of triggering parser codepaths.

---

### Comment 3 - hongtaihu

> A general note on fuzzer-generated bug reports in the clangd component:
> 
> If a crash is occurring in parser code (e.g. with `clang::ParseAST` on the stack), it would be helpful (and is likely to lead to the bug report being actioned sooner) to check whether the crash reproduces by providing the same input to `clang` with appropriate flags (and if so, filing the bug in the `clang` component rather than `clangd`).
> 
> The vast majority of crashes triggered by clangd in parser code are bugs in the parser itself (with only a small minority being related to clangd's particular ways of using the parser APIs which makes them not reproducible with a `clang` invocation) and are not clangd-specific at all. Clangd in this respect is just a convenient (for fuzzers, I guess) way of triggering parser codepaths.

Thanks for the detailed explanation!

I tried some furthermore reproduction and here are some discoveries.

## An easier way to reproduce

``` cpp
#include <cassert>

template <class InitFunc>
  InitResult static_cast_mutated<unsigned int>(access)(InitFunc&& init) noexcept(noexcept([] noexcept(noexcept(Baz())){ auto&& [a] = std::declval<InitFunc>(); }())) {
    auto res = check_guard<Impl>(&guard_object, init);
    data++;
    return res;
  }>
bool can_convert(Tp) { return true; }>
template<int has_unambiguous_public_base> struct Baz : virtual public Foo {
  Baz()  noexcept(noexcept(int>())){}
  Baz(int x) : Foo((x * 1)) {}
  pure();
};
++alignof(__cxa_exception)xornew (NewMeta) 
                               void{(*data & 0x0f000000) >> 24bitorkOurExceptionClass                    & get_vendor_and_language,__unw_set_reg(cursor, (unw_regnum_t)(UNW_ARM_R0 + regno),
                           *(unw_word_t *)valuep) == UNW_ESUCCESS
                 ? _UVRSR_OK
                 : _UVRSR_FAILED}

```
Then use the command:

```sh
clangd --check=main.hpp
```

Hopefully, it crashes, and the ASAN log looks like this:

```sh
I[17:08:44.150] clangd version 23.0.0git (https://github.com/llvm/llvm-project.git aff5afc48df63615053b2432da198a4932435c3f)
...
==362911==ERROR: AddressSanitizer: heap-use-after-free on address 0x50d0000048a8 at pc 0x5dd5cf7c9c2a bp 0x7ffe7d32cf00 sp 0x7ffe7d32cef0
READ of size 8 at 0x50d0000048a8 thread T0
    #0 0x5dd5cf7c9c29 in clang::OpaquePtr<clang::TemplateName>::get() const /home/ubuntu2404/llvm-project/clang/include/clang/Sema/Ownership.h:82
    #1 0x5dd5cf7c9c29 in clang::Sema::GetNameFromUnqualifiedId(clang::UnqualifiedId const&) /home/ubuntu2404/llvm-project/clang/lib/Sema/SemaDecl.cpp:6104
    #2 0x5dd5cf9450fe in clang::Sema::GetNameForDeclarator(clang::Declarator&) /home/ubuntu2404/llvm-project/clang/lib/Sema/SemaDecl.cpp:5993
    #3 0x5dd5cf9450fe in clang::Sema::HandleDeclarator(clang::Scope*, clang::Declarator&, llvm::MutableArrayRef<clang::TemplateParameterList*>) /home/ubuntu2404/llvm-project/clang/lib/Sema/SemaDecl.cpp:6439
    #4 0x5dd5cf949f5d in clang::Sema::ActOnStartOfFunctionDef(clang::Scope*, clang::Declarator&, llvm::MutableArrayRef<clang::TemplateParameterList*>, clang::SkipBodyInfo*, clang::Sema::FnBodyKind) /home/ubuntu2404/llvm-project/clang/lib/Sema/SemaDecl.cpp:15922
    #5 0x5dd5d7bcbd2f in clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) /home/ubuntu2404/llvm-project/clang/lib/Parse/Parser.cpp:1332
    #6 0x5dd5d7ced019 in clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) /home/ubuntu2404/llvm-project/clang/lib/Parse/ParseDecl.cpp:2265
    #7 0x5dd5d8071f48 in clang::Parser::ParseDeclarationAfterTemplate(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo&, clang::ParsingDeclRAIIObject&, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) /home/ubuntu2404/llvm-project/clang/lib/Parse/ParseTemplate.cpp:228
    #8 0x5dd5d80a299c in clang::Parser::ParseTemplateDeclarationOrSpecialization(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) /home/ubuntu2404/llvm-project/clang/lib/Parse/ParseTemplate.cpp:147
    #9 0x5dd5d80a3b55 in clang::Parser::ParseDeclarationStartingWithTemplate(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&) /home/ubuntu2404/llvm-project/clang/lib/Parse/ParseTemplate.cpp:43
    #10 0x5dd5d7cf22db in clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) /home/ubuntu2404/llvm-project/clang/lib/Parse/ParseDecl.cpp:1889
    #11 0x5dd5d7bd38e9 in clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) /home/ubuntu2404/llvm-project/clang/lib/Parse/Parser.cpp:944
    #12 0x5dd5d7bd8421 in clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) /home/ubuntu2404/llvm-project/clang/lib/Parse/Parser.cpp:706
    #13 0x5dd5d7bd99de in clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) /home/ubuntu2404/llvm-project/clang/lib/Parse/Parser.cpp:595
    #14 0x5dd5d7b57575 in clang::ParseAST(clang::Sema&, bool, bool) /home/ubuntu2404/llvm-project/clang/lib/Parse/ParseAST.cpp:169
    #15 0x5dd5d787fe49 in clang::FrontendAction::Execute() /home/ubuntu2404/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1322
    #16 0x5dd5d1a81ed4 in clang::clangd::ParsedAST::build(llvm::StringRef, clang::clangd::ParseInputs const&, std::unique_ptr<clang::CompilerInvocation, std::default_delete<clang::CompilerInvocation> >, llvm::ArrayRef<clang::clangd::Diag>, std::shared_ptr<clang::clangd::PreambleData const>) /home/ubuntu2404/llvm-project/clang-tools-extra/clangd/ParsedAST.cpp:714
    #17 0x5dd5d10afb09 in buildAST /home/ubuntu2404/llvm-project/clang-tools-extra/clangd/tool/Check.cpp:265
    #18 0x5dd5d10b830b in clang::clangd::check(llvm::StringRef, clang::clangd::ThreadsafeFS const&, clang::clangd::ClangdLSPServer::Options const&) /home/ubuntu2404/llvm-project/clang-tools-extra/clangd/tool/Check.cpp:525
    #19 0x5dd5d1084c76 in clang::clangd::clangdMain(int, char**) /home/ubuntu2404/llvm-project/clang-tools-extra/clangd/tool/ClangdMain.cpp:1028
    #20 0x7bef4602a1c9 in __libc_start_call_main ../sysdeps/nptl/libc_start_call_main.h:58
    #21 0x7bef4602a28a in __libc_start_main_impl ../csu/libc-start.c:360
    #22 0x5dd5cb483294 in _start (/home/ubuntu2404/llvm-project/build_asan/bin/clangd+0xf80294) (BuildId: d48805c6bdc26e44a606e626f54f60fc155fda5d)

0x50d0000048a8 is located 24 bytes inside of 136-byte region [0x50d000004890,0x50d000004918)
freed by thread T0 here:
    #0 0x7bef468fc4d8 in free ../../../../src/libsanitizer/asan/asan_malloc_linux.cpp:52
    #1 0x5dd5d7b9cdd0 in clang::TemplateIdAnnotation::Destroy() /home/ubuntu2404/llvm-project/clang/include/clang/Sema/ParsedTemplate.h:224
    #2 0x5dd5d7b9cdd0 in clang::Parser::DestroyTemplateIds() /home/ubuntu2404/llvm-project/clang/lib/Parse/Parser.cpp:584
    #3 0x5dd5d8030373 in clang::Parser::MaybeDestroyTemplateIds() /home/ubuntu2404/llvm-project/clang/include/clang/Parse/Parser.h:7842
    #4 0x5dd5d8030373 in clang::Parser::MaybeDestroyTemplateIds() /home/ubuntu2404/llvm-project/clang/include/clang/Parse/Parser.h:7837
    #5 0x5dd5d8030373 in clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) /home/ubuntu2404/llvm-project/clang/lib/Parse/ParseStmt.cpp:81
    #6 0x5dd5d8036592 in clang::Parser::ParseCompoundStatementBody(bool) /home/ubuntu2404/llvm-project/clang/lib/Parse/ParseStmt.cpp:1173
    #7 0x5dd5d7e68b71 in clang::Parser::ParseLambdaExpressionAfterIntroducer(clang::LambdaIntroducer&) /home/ubuntu2404/llvm-project/clang/lib/Parse/ParseExprCXX.cpp:1485
    #8 0x5dd5d7e6d397 in clang::Parser::ParseLambdaExpression() /home/ubuntu2404/llvm-project/clang/lib/Parse/ParseExprCXX.cpp:693
    #9 0x5dd5d7dcb21a in clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypoCorrectionTypeBehavior, bool, bool*) /home/ubuntu2404/llvm-project/clang/lib/Parse/ParseExpr.cpp:1595
    #10 0x5dd5d7dd1521 in clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) /home/ubuntu2404/llvm-project/clang/lib/Parse/ParseExpr.cpp:588
    #11 0x5dd5d7dd181b in clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) /home/ubuntu2404/llvm-project/clang/lib/Parse/ParseExpr.cpp:90
    #12 0x5dd5d7dcaf5a in clang::Parser::ParseExpression(clang::TypoCorrectionTypeBehavior) /home/ubuntu2404/llvm-project/clang/lib/Parse/ParseExpr.cpp:48
    #13 0x5dd5d7dcaf5a in clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypoCorrectionTypeBehavior, bool, bool*) /home/ubuntu2404/llvm-project/clang/lib/Parse/ParseExpr.cpp:1517
    #14 0x5dd5d7dd1521 in clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) /home/ubuntu2404/llvm-project/clang/lib/Parse/ParseExpr.cpp:588
    #15 0x5dd5d7e0a2bc in clang::Parser::ParseConstantExpressionInExprEvalContext(clang::TypoCorrectionTypeBehavior) /home/ubuntu2404/llvm-project/clang/lib/Parse/ParseExpr.cpp:127
    #16 0x5dd5d7d50745 in clang::Parser::tryParseExceptionSpecification(bool, clang::SourceRange&, llvm::SmallVectorImpl<clang::OpaquePtr<clang::QualType> >&, llvm::SmallVectorImpl<clang::SourceRange>&, clang::ActionResult<clang::Expr*, true>&, llvm::SmallVector<clang::Token, 4u>*&) /home/ubuntu2404/llvm-project/clang/lib/Parse/ParseDeclCXX.cpp:3983
    #17 0x5dd5d7d0c85e in clang::Parser::ParseFunctionDeclarator(clang::Declarator&, clang::ParsedAttributes&, clang::BalancedDelimiterTracker&, bool, bool) /home/ubuntu2404/llvm-project/clang/lib/Parse/ParseDecl.cpp:7294
    #18 0x5dd5d7d173d9 in clang::Parser::ParseDirectDeclarator(clang::Declarator&) /home/ubuntu2404/llvm-project/clang/lib/Parse/ParseDecl.cpp:6875
    #19 0x5dd5d7cf718a in clang::Parser::ParseDeclaratorInternal(clang::Declarator&, void (clang::Parser::*)(clang::Declarator&)) /home/ubuntu2404/llvm-project/clang/lib/Parse/ParseDecl.cpp:6439
    #20 0x5dd5d98fcc63 in llvm::function_ref<void ()>::operator()() const /home/ubuntu2404/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69
    #21 0x5dd5d98fcc63 in clang::runWithSufficientStackSpace(llvm::function_ref<void ()>, llvm::function_ref<void ()>) /home/ubuntu2404/llvm-project/clang/include/clang/Basic/Stack.h:49
    #22 0x5dd5d98fcc63 in clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) /home/ubuntu2404/llvm-project/clang/lib/Basic/StackExhaustionHandler.cpp:20
    #23 0x5dd5d7ce8ea9 in clang::Parser::ParseDeclarator(clang::Declarator&) /home/ubuntu2404/llvm-project/clang/lib/Parse/ParseDecl.cpp:6304
    #24 0x5dd5d7ce8ea9 in clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) /home/ubuntu2404/llvm-project/clang/lib/Parse/ParseDecl.cpp:2147
    #25 0x5dd5d8071f48 in clang::Parser::ParseDeclarationAfterTemplate(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo&, clang::ParsingDeclRAIIObject&, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) /home/ubuntu2404/llvm-project/clang/lib/Parse/ParseTemplate.cpp:228
    #26 0x5dd5d80a299c in clang::Parser::ParseTemplateDeclarationOrSpecialization(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) /home/ubuntu2404/llvm-project/clang/lib/Parse/ParseTemplate.cpp:147
    #27 0x5dd5d80a3b55 in clang::Parser::ParseDeclarationStartingWithTemplate(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&) /home/ubuntu2404/llvm-project/clang/lib/Parse/ParseTemplate.cpp:43
    #28 0x5dd5d7cf22db in clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) /home/ubuntu2404/llvm-project/clang/lib/Parse/ParseDecl.cpp:1889
    #29 0x5dd5d7bd38e9 in clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) /home/ubuntu2404/llvm-project/clang/lib/Parse/Parser.cpp:944
    #30 0x5dd5d7bd8421 in clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) /home/ubuntu2404/llvm-project/clang/lib/Parse/Parser.cpp:706
    #31 0x5dd5d7bd99de in clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) /home/ubuntu2404/llvm-project/clang/lib/Parse/Parser.cpp:595
    #32 0x5dd5d7b57575 in clang::ParseAST(clang::Sema&, bool, bool) /home/ubuntu2404/llvm-project/clang/lib/Parse/ParseAST.cpp:169
    #33 0x5dd5d787fe49 in clang::FrontendAction::Execute() /home/ubuntu2404/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1322
    #34 0x5dd5d1a81ed4 in clang::clangd::ParsedAST::build(llvm::StringRef, clang::clangd::ParseInputs const&, std::unique_ptr<clang::CompilerInvocation, std::default_delete<clang::CompilerInvocation> >, llvm::ArrayRef<clang::clangd::Diag>, std::shared_ptr<clang::clangd::PreambleData const>) /home/ubuntu2404/llvm-project/clang-tools-extra/clangd/ParsedAST.cpp:714
    #35 0x5dd5d10afb09 in buildAST /home/ubuntu2404/llvm-project/clang-tools-extra/clangd/tool/Check.cpp:265
    #36 0x5dd5d10b830b in clang::clangd::check(llvm::StringRef, clang::clangd::ThreadsafeFS const&, clang::clangd::ClangdLSPServer::Options const&) /home/ubuntu2404/llvm-project/clang-tools-extra/clangd/tool/Check.cpp:525

previously allocated by thread T0 here:
    #0 0x7bef468fd9c7 in malloc ../../../../src/libsanitizer/asan/asan_malloc_linux.cpp:69
    #1 0x5dd5d809398c in llvm::safe_malloc(unsigned long) /home/ubuntu2404/llvm-project/llvm/include/llvm/Support/MemAlloc.h:26
    #2 0x5dd5d809398c in clang::TemplateIdAnnotation::Create(clang::SourceLocation, clang::SourceLocation, clang::IdentifierInfo const*, clang::OverloadedOperatorKind, clang::OpaquePtr<clang::TemplateName>, clang::TemplateNameKind, clang::SourceLocation, clang::SourceLocation, llvm::ArrayRef<clang::ParsedTemplateArgument>, bool, llvm::SmallVectorImpl<clang::TemplateIdAnnotation*>&) /home/ubuntu2404/llvm-project/clang/include/clang/Sema/ParsedTemplate.h:210
    #3 0x5dd5d809398c in clang::Parser::AnnotateTemplateIdToken(clang::OpaquePtr<clang::TemplateName>, clang::TemplateNameKind, clang::CXXScopeSpec&, clang::SourceLocation, clang::UnqualifiedId&, bool, bool) /home/ubuntu2404/llvm-project/clang/lib/Parse/ParseTemplate.cpp:1169
    #4 0x5dd5d7e53601 in clang::Parser::ParseOptionalCXXScopeSpecifier(clang::CXXScopeSpec&, clang::OpaquePtr<clang::QualType>, bool, bool, bool*, bool, clang::IdentifierInfo const**, bool, bool, bool) /home/ubuntu2404/llvm-project/clang/lib/Parse/ParseExprCXX.cpp:545
    #5 0x5dd5d7cf83d7 in clang::Parser::ParseDeclaratorInternal(clang::Declarator&, void (clang::Parser::*)(clang::Declarator&)) /home/ubuntu2404/llvm-project/clang/lib/Parse/ParseDecl.cpp:6367
    #6 0x5dd5d98fcc63 in llvm::function_ref<void ()>::operator()() const /home/ubuntu2404/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69
    #7 0x5dd5d98fcc63 in clang::runWithSufficientStackSpace(llvm::function_ref<void ()>, llvm::function_ref<void ()>) /home/ubuntu2404/llvm-project/clang/include/clang/Basic/Stack.h:49
    #8 0x5dd5d98fcc63 in clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) /home/ubuntu2404/llvm-project/clang/lib/Basic/StackExhaustionHandler.cpp:20
    #9 0x5dd5d7ce8ea9 in clang::Parser::ParseDeclarator(clang::Declarator&) /home/ubuntu2404/llvm-project/clang/lib/Parse/ParseDecl.cpp:6304
    #10 0x5dd5d7ce8ea9 in clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) /home/ubuntu2404/llvm-project/clang/lib/Parse/ParseDecl.cpp:2147
    #11 0x5dd5d8071f48 in clang::Parser::ParseDeclarationAfterTemplate(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo&, clang::ParsingDeclRAIIObject&, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) /home/ubuntu2404/llvm-project/clang/lib/Parse/ParseTemplate.cpp:228
    #12 0x5dd5d80a299c in clang::Parser::ParseTemplateDeclarationOrSpecialization(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) /home/ubuntu2404/llvm-project/clang/lib/Parse/ParseTemplate.cpp:147
    #13 0x5dd5d80a3b55 in clang::Parser::ParseDeclarationStartingWithTemplate(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&) /home/ubuntu2404/llvm-project/clang/lib/Parse/ParseTemplate.cpp:43
    #14 0x5dd5d7cf22db in clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) /home/ubuntu2404/llvm-project/clang/lib/Parse/ParseDecl.cpp:1889
    #15 0x5dd5d7bd38e9 in clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) /home/ubuntu2404/llvm-project/clang/lib/Parse/Parser.cpp:944
    #16 0x5dd5d7bd8421 in clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) /home/ubuntu2404/llvm-project/clang/lib/Parse/Parser.cpp:706
    #17 0x5dd5d7bd99de in clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) /home/ubuntu2404/llvm-project/clang/lib/Parse/Parser.cpp:595
    #18 0x5dd5d7b57575 in clang::ParseAST(clang::Sema&, bool, bool) /home/ubuntu2404/llvm-project/clang/lib/Parse/ParseAST.cpp:169
    #19 0x5dd5d787fe49 in clang::FrontendAction::Execute() /home/ubuntu2404/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1322
    #20 0x5dd5d1a81ed4 in clang::clangd::ParsedAST::build(llvm::StringRef, clang::clangd::ParseInputs const&, std::unique_ptr<clang::CompilerInvocation, std::default_delete<clang::CompilerInvocation> >, llvm::ArrayRef<clang::clangd::Diag>, std::shared_ptr<clang::clangd::PreambleData const>) /home/ubuntu2404/llvm-project/clang-tools-extra/clangd/ParsedAST.cpp:714
    #21 0x5dd5d10afb09 in buildAST /home/ubuntu2404/llvm-project/clang-tools-extra/clangd/tool/Check.cpp:265
    #22 0x5dd5d10b830b in clang::clangd::check(llvm::StringRef, clang::clangd::ThreadsafeFS const&, clang::clangd::ClangdLSPServer::Options const&) /home/ubuntu2404/llvm-project/clang-tools-extra/clangd/tool/Check.cpp:525
    #23 0x5dd5d1084c76 in clang::clangd::clangdMain(int, char**) /home/ubuntu2404/llvm-project/clang-tools-extra/clangd/tool/ClangdMain.cpp:1028
    #24 0x7bef4602a1c9 in __libc_start_call_main ../sysdeps/nptl/libc_start_call_main.h:58
    #25 0x7bef4602a28a in __libc_start_main_impl ../csu/libc-start.c:360
    #26 0x5dd5cb483294 in _start (/home/ubuntu2404/llvm-project/build_asan/bin/clangd+0xf80294) (BuildId: d48805c6bdc26e44a606e626f54f60fc155fda5d)

SUMMARY: AddressSanitizer: heap-use-after-free /home/ubuntu2404/llvm-project/clang/include/clang/Sema/Ownership.h:82 in clang::OpaquePtr<clang::TemplateName>::get() const
Shadow bytes around the buggy address:
  0x50d000004600: fa fa fa fa 00 00 00 00 00 00 00 00 00 00 00 00
  0x50d000004680: 00 00 00 00 00 00 fa fa fa fa fa fa fa fa 00 00
  0x50d000004700: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x50d000004780: fa fa fa fa fa fa fa fa 00 00 00 00 00 00 00 00
  0x50d000004800: 00 00 00 00 00 00 00 00 00 fa fa fa fa fa fa fa
=>0x50d000004880: fa fa fd fd fd[fd]fd fd fd fd fd fd fd fd fd fd
  0x50d000004900: fd fd fd fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x50d000004980: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x50d000004a00: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x50d000004a80: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x50d000004b00: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
Shadow byte legend (one shadow byte represents 8 application bytes):
  Addressable:           00
  Partially addressable: 01 02 03 04 05 06 07 
  Heap left redzone:       fa
  Freed heap region:       fd
  Stack left redzone:      f1
  Stack mid redzone:       f2
  Stack right redzone:     f3
  Stack after return:      f5
  Stack use after scope:   f8
  Global redzone:          f9
  Global init order:       f6
  Poisoned by user:        f7
  Container overflow:      fc
  Array cookie:            ac
  Intra object redzone:    bb
  ASan internal:           fe
  Left alloca redzone:     ca
  Right alloca redzone:    cb
==362911==ABORTING
```

Besides, I also tried with the command:
```sh
clang -fsyntax-only
```
``` sh
clang -x c++-header -fsyntax-only
```
but it did not crash.



---

### Comment 4 - ArcsinX

> ## An easier way to reproduce
> ``` cpp
> #include <cassert>
>
> template <class InitFunc>
>  InitResult static_cast_mutated<unsigned int>(access)(InitFunc&& init) noexcept(noexcept([] noexcept(noexcept(Baz())){ auto&& [a] = std::declval<InitFunc>(); }())) {
>    auto res = check_guard<Impl>(&guard_object, init);
>    data++;
>    return res;
>  }>
> bool can_convert(Tp) { return true; }>
> template<int has_unambiguous_public_base> struct Baz : virtual public Foo {
>  Baz()  noexcept(noexcept(int>())){}
>  Baz(int x) : Foo((x * 1)) {}
>  pure();
> };
> ++alignof(__cxa_exception)xornew (NewMeta) 
>                               void{(*data & 0x0f000000) >> 24bitorkOurExceptionClass                    & get_vendor_and_language,__unw_set_reg(cursor, (unw_regnum_t)(UNW_ARM_R0 + regno),
>                           *(unw_word_t *)valuep) == UNW_ESUCCESS
>                 ? _UVRSR_OK
>                 : _UVRSR_FAILED}
> ```
> Then use the command:
> 
> clangd --check=main.hpp
> Hopefully, it crashes, and the ASAN log looks like this:
> 
> I[17:08:44.150] clangd version 23.0.0git (https://github.com/llvm/llvm-project.git aff5afc48df63615053b2432da198a4932435c3f)

I tried to reproduce using this example and exactly the same clangd version and for me `clangd --check=main.hpp` doesn't crash

```
I[15:45:05.523] clangd version 23.0.0git (https://github.com/llvm/llvm-project.git aff5afc48df63615053b2432da198a4932435c3f)
I[15:45:05.523] Features: linux+grpc
I[15:45:05.523] PID: 156090
I[15:45:05.523] Working directory: ...
I[15:45:05.523] argv[0]: bin/clangd
I[15:45:05.523] argv[1]: --check=~/main.hpp
I[15:45:05.523] Entering check mode (no LSP server)
I[15:45:05.523] Testing on source file ...main.hpp
I[15:45:05.523] Loading compilation database...
I[15:45:05.523] Failed to find compilation database for ...main.hpp
I[15:45:05.523] Generic fallback command is: [...] .../llvm-project/build_clang/bin/clang -resource-dir=.../llvm-project/build_clang/lib/clang/23 -- .../main.hpp
I[15:45:05.523] Parsing command...
I[15:45:05.524] internal (cc1) args are: -cc1 -triple x86_64-unknown-linux-gnu -fsyntax-only -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name main.hpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=... -fcoverage-compilation-dir=... -resource-dir ...llvm-project/build_clang/lib/clang/23 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/x86_64-linux-gnu/c++/11 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/backward -internal-isystem ...llvm-project/build_clang/lib/clang/23/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -fdeprecated-macro -ferror-limit 19 -fmessage-length=214 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -no-round-trip-args -faddrsig -fdwarf2-cfi-asm -x c++-header ...main.hpp
I[15:45:05.524] Building preamble...
I[15:45:05.535] Built preamble of size 314084 for file ...main.hpp version null in 0.01 seconds
I[15:45:05.535] Indexing headers...
I[15:45:05.541] Building AST...
E[15:45:05.547] [unknown_typename] Line 4: unknown type name 'InitResult'
E[15:45:05.547] [unknown_typename] Line 4: unknown type name 'access'
E[15:45:05.547] [undeclared_var_use] Line 4: use of undeclared identifier 'Baz'
E[15:45:05.547] [no_member] Line 4: no member named 'declval' in namespace 'std'
E[15:45:05.547] [ref_non_value] Line 4: 'InitFunc' does not refer to a value
E[15:45:05.547] [expected_expression] Line 4: expected expression
E[15:45:05.547] [lambda_unevaluated_operand] Line 4: lambda expression in an unevaluated operand
E[15:45:05.547] [function_template_partial_spec] Line 4: function template partial specialization is not allowed
E[15:45:05.547] [undeclared_var_use] Line 5: use of undeclared identifier 'Impl'
E[15:45:05.547] [undeclared_var_use] Line 5: use of undeclared identifier 'guard_object'
E[15:45:05.547] [undeclared_var_use] Line 5: use of undeclared identifier 'init'
E[15:45:05.547] [undeclared_var_use] Line 6: use of undeclared identifier 'data'
E[15:45:05.547] [expected_unqualified_id] Line 8: expected unqualified-id
E[15:45:05.547] [expected_unqualified_id] Line 9: expected unqualified-id
E[15:45:05.547] [expected_unqualified_id] Line 15: expected unqualified-id
I[15:45:05.547] Indexing AST...
I[15:45:05.548] Building inlay hints
I[15:45:05.548] Building semantic highlighting
I[15:45:05.548] Testing features at each token (may be slow in large files)
E[15:45:05.556]     tweak: ExpandDeducedType ==> FAIL: Could not deduce type for 'auto' type
I[15:45:05.557] All checks completed, 16 errors
```

---

### Comment 5 - hongtaihu

> > ## An easier way to reproduce
> > #include <cassert>
> > 
> > template <class InitFunc>
> >  InitResult static_cast_mutated<unsigned int>(access)(InitFunc&& init) noexcept(noexcept([] noexcept(noexcept(Baz())){ auto&& [a] = std::declval<InitFunc>(); }())) {
> >    auto res = check_guard<Impl>(&guard_object, init);
> >    data++;
> >    return res;
> >  }>
> > bool can_convert(Tp) { return true; }>
> > template<int has_unambiguous_public_base> struct Baz : virtual public Foo {
> >  Baz()  noexcept(noexcept(int>())){}
> >  Baz(int x) : Foo((x * 1)) {}
> >  pure();
> > };
> > ++alignof(__cxa_exception)xornew (NewMeta) 
> >                               void{(*data & 0x0f000000) >> 24bitorkOurExceptionClass                    & get_vendor_and_language,__unw_set_reg(cursor, (unw_regnum_t)(UNW_ARM_R0 + regno),
> >                           *(unw_word_t *)valuep) == UNW_ESUCCESS
> >                 ? _UVRSR_OK
> >                 : _UVRSR_FAILED}
> >     
> >       
> >     
> > 
> >       
> >     
> > 
> >     
> >   
> > Then use the command:
> > clangd --check=main.hpp
> > Hopefully, it crashes, and the ASAN log looks like this:
> > I[17:08:44.150] clangd version 23.0.0git (https://github.com/llvm/llvm-project.git [aff5afc](https://github.com/llvm/llvm-project/commit/aff5afc48df63615053b2432da198a4932435c3f))
> 
> I tried to reproduce using this example and exactly the same clangd version and for me `clangd --check=main.hpp` doesn't crash
> 
> ```
> I[15:45:05.523] clangd version 23.0.0git (https://github.com/llvm/llvm-project.git aff5afc48df63615053b2432da198a4932435c3f)
> I[15:45:05.523] Features: linux+grpc
> I[15:45:05.523] PID: 156090
> I[15:45:05.523] Working directory: ...
> I[15:45:05.523] argv[0]: bin/clangd
> I[15:45:05.523] argv[1]: --check=~/main.hpp
> I[15:45:05.523] Entering check mode (no LSP server)
> I[15:45:05.523] Testing on source file ...main.hpp
> I[15:45:05.523] Loading compilation database...
> I[15:45:05.523] Failed to find compilation database for ...main.hpp
> I[15:45:05.523] Generic fallback command is: [...] .../llvm-project/build_clang/bin/clang -resource-dir=.../llvm-project/build_clang/lib/clang/23 -- .../main.hpp
> I[15:45:05.523] Parsing command...
> I[15:45:05.524] internal (cc1) args are: -cc1 -triple x86_64-unknown-linux-gnu -fsyntax-only -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name main.hpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=... -fcoverage-compilation-dir=... -resource-dir ...llvm-project/build_clang/lib/clang/23 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/x86_64-linux-gnu/c++/11 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/backward -internal-isystem ...llvm-project/build_clang/lib/clang/23/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -fdeprecated-macro -ferror-limit 19 -fmessage-length=214 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -no-round-trip-args -faddrsig -fdwarf2-cfi-asm -x c++-header ...main.hpp
> I[15:45:05.524] Building preamble...
> I[15:45:05.535] Built preamble of size 314084 for file ...main.hpp version null in 0.01 seconds
> I[15:45:05.535] Indexing headers...
> I[15:45:05.541] Building AST...
> E[15:45:05.547] [unknown_typename] Line 4: unknown type name 'InitResult'
> E[15:45:05.547] [unknown_typename] Line 4: unknown type name 'access'
> E[15:45:05.547] [undeclared_var_use] Line 4: use of undeclared identifier 'Baz'
> E[15:45:05.547] [no_member] Line 4: no member named 'declval' in namespace 'std'
> E[15:45:05.547] [ref_non_value] Line 4: 'InitFunc' does not refer to a value
> E[15:45:05.547] [expected_expression] Line 4: expected expression
> E[15:45:05.547] [lambda_unevaluated_operand] Line 4: lambda expression in an unevaluated operand
> E[15:45:05.547] [function_template_partial_spec] Line 4: function template partial specialization is not allowed
> E[15:45:05.547] [undeclared_var_use] Line 5: use of undeclared identifier 'Impl'
> E[15:45:05.547] [undeclared_var_use] Line 5: use of undeclared identifier 'guard_object'
> E[15:45:05.547] [undeclared_var_use] Line 5: use of undeclared identifier 'init'
> E[15:45:05.547] [undeclared_var_use] Line 6: use of undeclared identifier 'data'
> E[15:45:05.547] [expected_unqualified_id] Line 8: expected unqualified-id
> E[15:45:05.547] [expected_unqualified_id] Line 9: expected unqualified-id
> E[15:45:05.547] [expected_unqualified_id] Line 15: expected unqualified-id
> I[15:45:05.547] Indexing AST...
> I[15:45:05.548] Building inlay hints
> I[15:45:05.548] Building semantic highlighting
> I[15:45:05.548] Testing features at each token (may be slow in large files)
> E[15:45:05.556]     tweak: ExpandDeducedType ==> FAIL: Could not deduce type for 'auto' type
> I[15:45:05.557] All checks completed, 16 errors
> ```

Maybe it only crashes when ASAN is applied? Mine didn't crash either when using the non-ASAN version.
```sh
$ /home/ubuntu2404/llvm-project/build/bin/clangd -check=main.hpp
I[21:18:18.498] clangd version 23.0.0git (https://github.com/llvm/llvm-project.git aff5afc48df63615053b2432da198a4932435c3f)
I[21:18:18.498] Features: linux
I[21:18:18.498] PID: 407431
I[21:18:18.498] Working directory: /home/ubuntu2404/lsp_fuzz_latest/lsp-fuzz/repro_packages_v3/id_27_time_19585_exec_284040/workspace
I[21:18:18.498] argv[0]: /home/ubuntu2404/llvm-project/build/bin/clangd
I[21:18:18.498] argv[1]: -check=main.hpp
I[21:18:18.498] Entering check mode (no LSP server)
I[21:18:18.498] Testing on source file /home/ubuntu2404/lsp_fuzz_latest/lsp-fuzz/repro_packages_v3/id_27_time_19585_exec_284040/workspace/main.hpp
I[21:18:18.498] Loading compilation database...
I[21:18:18.499] Failed to find compilation database for /home/ubuntu2404/lsp_fuzz_latest/lsp-fuzz/repro_packages_v3/id_27_time_19585_exec_284040/workspace/main.hpp
I[21:18:18.499] Generic fallback command is: [/home/ubuntu2404/lsp_fuzz_latest/lsp-fuzz/repro_packages_v3/id_27_time_19585_exec_284040/workspace] /usr/lib/llvm-18/bin/clang -resource-dir=/home/ubuntu2404/llvm-project/build/lib/clang/23 -- /home/ubuntu2404/lsp_fuzz_latest/lsp-fuzz/repro_packages_v3/id_27_time_19585_exec_284040/workspace/main.hpp
I[21:18:18.499] Parsing command...
I[21:18:18.502] internal (cc1) args are: -cc1 -triple x86_64-unknown-linux-gnu -fsyntax-only -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name main.hpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/home/ubuntu2404/lsp_fuzz_latest/lsp-fuzz/repro_packages_v3/id_27_time_19585_exec_284040/workspace -fcoverage-compilation-dir=/home/ubuntu2404/lsp_fuzz_latest/lsp-fuzz/repro_packages_v3/id_27_time_19585_exec_284040/workspace -resource-dir /home/ubuntu2404/llvm-project/build/lib/clang/23 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/x86_64-linux-gnu/c++/13 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/backward -internal-isystem /home/ubuntu2404/llvm-project/build/lib/clang/23/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -fdeprecated-macro -ferror-limit 19 -fmessage-length=149 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -no-round-trip-args -faddrsig -fdwarf2-cfi-asm -x c++-header /home/ubuntu2404/lsp_fuzz_latest/lsp-fuzz/repro_packages_v3/id_27_time_19585_exec_284040/workspace/main.hpp
I[21:18:18.502] Building preamble...
I[21:18:18.522] Built preamble of size 316172 for file /home/ubuntu2404/lsp_fuzz_latest/lsp-fuzz/repro_packages_v3/id_27_time_19585_exec_284040/workspace/main.hpp version null in 0.02 seconds
I[21:18:18.522] Indexing headers...
I[21:18:18.528] Building AST...
E[21:18:18.533] [unknown_typename] Line 4: unknown type name 'InitResult'
E[21:18:18.533] [unknown_typename] Line 4: unknown type name 'access'
E[21:18:18.533] [undeclared_var_use] Line 4: use of undeclared identifier 'Baz'
E[21:18:18.533] [no_member] Line 4: no member named 'declval' in namespace 'std'
E[21:18:18.533] [ref_non_value] Line 4: 'InitFunc' does not refer to a value
E[21:18:18.533] [expected_expression] Line 4: expected expression
E[21:18:18.533] [lambda_unevaluated_operand] Line 4: lambda expression in an unevaluated operand
E[21:18:18.534] [function_template_partial_spec] Line 4: function template partial specialization is not allowed
E[21:18:18.534] [undeclared_var_use] Line 5: use of undeclared identifier 'Impl'
E[21:18:18.534] [undeclared_var_use] Line 5: use of undeclared identifier 'guard_object'
E[21:18:18.534] [undeclared_var_use] Line 5: use of undeclared identifier 'init'
E[21:18:18.534] [undeclared_var_use] Line 6: use of undeclared identifier 'data'
E[21:18:18.534] [expected_unqualified_id] Line 8: expected unqualified-id
E[21:18:18.534] [expected_unqualified_id] Line 9: expected unqualified-id
E[21:18:18.534] [expected_unqualified_id] Line 15: expected unqualified-id
I[21:18:18.534] Indexing AST...
I[21:18:18.534] Building inlay hints
I[21:18:18.534] Building semantic highlighting
I[21:18:18.534] Testing features at each token (may be slow in large files)
I[21:18:18.535] All checks completed, 15 errors
```

Besides, my local clangd build uses GCC 13 rather than GCC 11, so the diagnostic output is slightly different. For example, the **ExpandDeducedType** tweak failure in your log does not appear on my system. 
```
"E[15:45:05.556]     tweak: ExpandDeducedType ==> FAIL: Could not deduce type for 'auto' type"
```


---

### Comment 6 - hongtaihu

> > > ## An easier way to reproduce
> > > #include 
> > > template 
> > > InitResult static_cast_mutated(access)(InitFunc&& init) noexcept(noexcept([] noexcept(noexcept(Baz())){ auto&& [a] = std::declval(); }())) {
> > > auto res = check_guard(&guard_object, init);
> > > data++;
> > > return res;
> > > }>
> > > bool can_convert(Tp) { return true; }>
> > > template struct Baz : virtual public Foo {
> > > Baz()  noexcept(noexcept(int>())){}
> > > Baz(int x) : Foo((x * 1)) {}
> > > pure();
> > > };
> > > ++alignof(__cxa_exception)xornew (NewMeta)
> > > void{(*data & 0x0f000000) >> 24bitorkOurExceptionClass                    & get_vendor_and_language,__unw_set_reg(cursor, (unw_regnum_t)(UNW_ARM_R0 + regno),
> > > *(unw_word_t *)valuep) == UNW_ESUCCESS
> > > ? _UVRSR_OK
> > > : _UVRSR_FAILED}
> > > Then use the command:
> > > clangd --check=main.hpp
> > > Hopefully, it crashes, and the ASAN log looks like this:
> > > I[17:08:44.150] clangd version 23.0.0git (https://github.com/llvm/llvm-project.git [aff5afc](https://github.com/llvm/llvm-project/commit/aff5afc48df63615053b2432da198a4932435c3f))
> > 
> > 
> > I tried to reproduce using this example and exactly the same clangd version and for me `clangd --check=main.hpp` doesn't crash
> > ```
> > I[15:45:05.523] clangd version 23.0.0git (https://github.com/llvm/llvm-project.git aff5afc48df63615053b2432da198a4932435c3f)
> > I[15:45:05.523] Features: linux+grpc
> > I[15:45:05.523] PID: 156090
> > I[15:45:05.523] Working directory: ...
> > I[15:45:05.523] argv[0]: bin/clangd
> > I[15:45:05.523] argv[1]: --check=~/main.hpp
> > I[15:45:05.523] Entering check mode (no LSP server)
> > I[15:45:05.523] Testing on source file ...main.hpp
> > I[15:45:05.523] Loading compilation database...
> > I[15:45:05.523] Failed to find compilation database for ...main.hpp
> > I[15:45:05.523] Generic fallback command is: [...] .../llvm-project/build_clang/bin/clang -resource-dir=.../llvm-project/build_clang/lib/clang/23 -- .../main.hpp
> > I[15:45:05.523] Parsing command...
> > I[15:45:05.524] internal (cc1) args are: -cc1 -triple x86_64-unknown-linux-gnu -fsyntax-only -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name main.hpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=... -fcoverage-compilation-dir=... -resource-dir ...llvm-project/build_clang/lib/clang/23 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/x86_64-linux-gnu/c++/11 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/backward -internal-isystem ...llvm-project/build_clang/lib/clang/23/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -fdeprecated-macro -ferror-limit 19 -fmessage-length=214 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -no-round-trip-args -faddrsig -fdwarf2-cfi-asm -x c++-header ...main.hpp
> > I[15:45:05.524] Building preamble...
> > I[15:45:05.535] Built preamble of size 314084 for file ...main.hpp version null in 0.01 seconds
> > I[15:45:05.535] Indexing headers...
> > I[15:45:05.541] Building AST...
> > E[15:45:05.547] [unknown_typename] Line 4: unknown type name 'InitResult'
> > E[15:45:05.547] [unknown_typename] Line 4: unknown type name 'access'
> > E[15:45:05.547] [undeclared_var_use] Line 4: use of undeclared identifier 'Baz'
> > E[15:45:05.547] [no_member] Line 4: no member named 'declval' in namespace 'std'
> > E[15:45:05.547] [ref_non_value] Line 4: 'InitFunc' does not refer to a value
> > E[15:45:05.547] [expected_expression] Line 4: expected expression
> > E[15:45:05.547] [lambda_unevaluated_operand] Line 4: lambda expression in an unevaluated operand
> > E[15:45:05.547] [function_template_partial_spec] Line 4: function template partial specialization is not allowed
> > E[15:45:05.547] [undeclared_var_use] Line 5: use of undeclared identifier 'Impl'
> > E[15:45:05.547] [undeclared_var_use] Line 5: use of undeclared identifier 'guard_object'
> > E[15:45:05.547] [undeclared_var_use] Line 5: use of undeclared identifier 'init'
> > E[15:45:05.547] [undeclared_var_use] Line 6: use of undeclared identifier 'data'
> > E[15:45:05.547] [expected_unqualified_id] Line 8: expected unqualified-id
> > E[15:45:05.547] [expected_unqualified_id] Line 9: expected unqualified-id
> > E[15:45:05.547] [expected_unqualified_id] Line 15: expected unqualified-id
> > I[15:45:05.547] Indexing AST...
> > I[15:45:05.548] Building inlay hints
> > I[15:45:05.548] Building semantic highlighting
> > I[15:45:05.548] Testing features at each token (may be slow in large files)
> > E[15:45:05.556]     tweak: ExpandDeducedType ==> FAIL: Could not deduce type for 'auto' type
> > I[15:45:05.557] All checks completed, 16 errors
> > ```
> 
> Maybe it only crashes when ASAN is applied? Mine didn't crash either when using the non-ASAN version.
> 
> $ /home/ubuntu2404/llvm-project/build/bin/clangd -check=main.hpp
> I[21:18:18.498] clangd version 23.0.0git (https://github.com/llvm/llvm-project.git aff5afc48df63615053b2432da198a4932435c3f)
> I[21:18:18.498] Features: linux
> I[21:18:18.498] PID: 407431
> I[21:18:18.498] Working directory: /home/ubuntu2404/lsp_fuzz_latest/lsp-fuzz/repro_packages_v3/id_27_time_19585_exec_284040/workspace
> I[21:18:18.498] argv[0]: /home/ubuntu2404/llvm-project/build/bin/clangd
> I[21:18:18.498] argv[1]: -check=main.hpp
> I[21:18:18.498] Entering check mode (no LSP server)
> I[21:18:18.498] Testing on source file /home/ubuntu2404/lsp_fuzz_latest/lsp-fuzz/repro_packages_v3/id_27_time_19585_exec_284040/workspace/main.hpp
> I[21:18:18.498] Loading compilation database...
> I[21:18:18.499] Failed to find compilation database for /home/ubuntu2404/lsp_fuzz_latest/lsp-fuzz/repro_packages_v3/id_27_time_19585_exec_284040/workspace/main.hpp
> I[21:18:18.499] Generic fallback command is: [/home/ubuntu2404/lsp_fuzz_latest/lsp-fuzz/repro_packages_v3/id_27_time_19585_exec_284040/workspace] /usr/lib/llvm-18/bin/clang -resource-dir=/home/ubuntu2404/llvm-project/build/lib/clang/23 -- /home/ubuntu2404/lsp_fuzz_latest/lsp-fuzz/repro_packages_v3/id_27_time_19585_exec_284040/workspace/main.hpp
> I[21:18:18.499] Parsing command...
> I[21:18:18.502] internal (cc1) args are: -cc1 -triple x86_64-unknown-linux-gnu -fsyntax-only -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name main.hpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/home/ubuntu2404/lsp_fuzz_latest/lsp-fuzz/repro_packages_v3/id_27_time_19585_exec_284040/workspace -fcoverage-compilation-dir=/home/ubuntu2404/lsp_fuzz_latest/lsp-fuzz/repro_packages_v3/id_27_time_19585_exec_284040/workspace -resource-dir /home/ubuntu2404/llvm-project/build/lib/clang/23 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/x86_64-linux-gnu/c++/13 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/backward -internal-isystem /home/ubuntu2404/llvm-project/build/lib/clang/23/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -fdeprecated-macro -ferror-limit 19 -fmessage-length=149 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -no-round-trip-args -faddrsig -fdwarf2-cfi-asm -x c++-header /home/ubuntu2404/lsp_fuzz_latest/lsp-fuzz/repro_packages_v3/id_27_time_19585_exec_284040/workspace/main.hpp
> I[21:18:18.502] Building preamble...
> I[21:18:18.522] Built preamble of size 316172 for file /home/ubuntu2404/lsp_fuzz_latest/lsp-fuzz/repro_packages_v3/id_27_time_19585_exec_284040/workspace/main.hpp version null in 0.02 seconds
> I[21:18:18.522] Indexing headers...
> I[21:18:18.528] Building AST...
> E[21:18:18.533] [unknown_typename] Line 4: unknown type name 'InitResult'
> E[21:18:18.533] [unknown_typename] Line 4: unknown type name 'access'
> E[21:18:18.533] [undeclared_var_use] Line 4: use of undeclared identifier 'Baz'
> E[21:18:18.533] [no_member] Line 4: no member named 'declval' in namespace 'std'
> E[21:18:18.533] [ref_non_value] Line 4: 'InitFunc' does not refer to a value
> E[21:18:18.533] [expected_expression] Line 4: expected expression
> E[21:18:18.533] [lambda_unevaluated_operand] Line 4: lambda expression in an unevaluated operand
> E[21:18:18.534] [function_template_partial_spec] Line 4: function template partial specialization is not allowed
> E[21:18:18.534] [undeclared_var_use] Line 5: use of undeclared identifier 'Impl'
> E[21:18:18.534] [undeclared_var_use] Line 5: use of undeclared identifier 'guard_object'
> E[21:18:18.534] [undeclared_var_use] Line 5: use of undeclared identifier 'init'
> E[21:18:18.534] [undeclared_var_use] Line 6: use of undeclared identifier 'data'
> E[21:18:18.534] [expected_unqualified_id] Line 8: expected unqualified-id
> E[21:18:18.534] [expected_unqualified_id] Line 9: expected unqualified-id
> E[21:18:18.534] [expected_unqualified_id] Line 15: expected unqualified-id
> I[21:18:18.534] Indexing AST...
> I[21:18:18.534] Building inlay hints
> I[21:18:18.534] Building semantic highlighting
> I[21:18:18.534] Testing features at each token (may be slow in large files)
> I[21:18:18.535] All checks completed, 15 errors
> Besides, my local clangd build uses GCC 13 rather than GCC 11, so the diagnostic output is slightly different. For example, the **ExpandDeducedType** tweak failure in your log does not appear on my system.
> 
> ```
> "E[15:45:05.556]     tweak: ExpandDeducedType ==> FAIL: Could not deduce type for 'auto' type"
> ```

I think  it's the ASAN that detects the use‑after‑free.

---

