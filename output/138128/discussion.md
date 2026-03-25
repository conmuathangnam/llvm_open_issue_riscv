# [clangd][clang] Crash when parsing code with consecutive commas in function arguments

**Author:** henryhchchc
**URL:** https://github.com/llvm/llvm-project/issues/138128
**Status:** Closed
**Labels:** clang:frontend, crash
**Closed Date:** 2025-05-01T13:12:31Z

## Body

Create a file named `main.cpp` with the following statement.

```c++
 auto foo = E->bar(baz,x,,);
```

Open it with a code editor with clangd enabled and you will see clangd crashes with the following stack trace. It also crashes clang. Looks like a problem in the front-end.

```
I[12:45:32.470] clangd version 20.1.2 (https://github.com/llvm/llvm-project.git 58df0ef89dd64126512e4ee27b4ac3fd8ddf6247)
I[12:45:32.470] Features: linux+asan
I[12:45:32.470] PID: 4512
I[12:45:32.470] Working directory: /tmp/export/input_11
I[12:45:32.470] argv[0]: /src/build/bin/clangd
I[12:45:32.470] Starting LSP over stdin/stdout

...

PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.      /tmp/export/input_11/workspace/main.h:1:70: current parser token ';'
 #0 0x0000000005823eea ___interceptor_backtrace (/src/build/bin/clangd+0x5823eea)
 #1 0x0000000005dfc7e7 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /src/llvm/lib/Support/Unix/Signals.inc:799:13
 #2 0x0000000005dfbbac llvm::sys::RunSignalHandlers() /src/llvm/lib/Support/Signals.cpp:0:5
 #3 0x0000000005dfbbac SignalHandler(int) /src/llvm/lib/Support/Unix/Signals.inc:405:3
 #4 0x00007f8a8da9d730 __restore_rt (/lib64/libc.so.6+0x3e730)
 #5 0x00000000070b0719 clang::TypoCorrectionConsumer::getCurrentCorrection() /src/clang/include/clang/Sema/SemaInternal.h:151:12
 #6 0x00000000070b0719 (anonymous namespace)::TransformTypos::EmitAllDiagnostics(bool) /src/clang/lib/Sema/SemaExprCXX.cpp:8877:50
 #7 0x00000000070b0719 (anonymous namespace)::TransformTypos::Transform(clang::Expr*) /src/clang/lib/Sema/SemaExprCXX.cpp:9125:5
 #8 0x00000000070b0719 clang::Sema::CorrectDelayedTyposInExpr(clang::Expr*, clang::VarDecl*, bool, llvm::function_ref<clang::ActionResult<clang::Expr*, true> (clang::Expr*)>) /src/clang/lib/Sema/SemaExprCXX.cpp:9184:59
 #9 0x000000000744f990 clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) /src/clang/lib/Sema/SemaDecl.cpp:13434:22
#10 0x00000000102ed335 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) /src/clang/lib/Parse/ParseDecl.cpp:0:17
#11 0x00000000102e5f6e clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) /src/clang/lib/Parse/ParseDecl.cpp:2551:7
#12 0x00000000101eb082 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) /src/clang/lib/Parse/Parser.cpp:1244:10
#13 0x00000000101ea230 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) /src/clang/lib/Parse/Parser.cpp:0:12
#14 0x00000000101e7798 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) /src/clang/lib/Parse/Parser.cpp:0:14
#15 0x00000000101e279a clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) /src/clang/lib/Parse/Parser.cpp:0:12
#16 0x00000000101e1619 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) /src/clang/lib/Parse/Parser.cpp:615:23
#17 0x00000000101d2514 clang::ParseAST(clang::Sema&, bool, bool) /src/clang/lib/Parse/ParseAST.cpp:170:5
#18 0x000000000fd2ffe8 clang::CompilerInstance::shouldBuildGlobalModuleIndex() const /src/clang/lib/Frontend/CompilerInstance.cpp:86:11
#19 0x000000000fd2ffe8 clang::FrontendAction::Execute() /src/clang/lib/Frontend/FrontendAction.cpp:1076:10
#20 0x0000000008de2eed llvm::Error::getPtr() const /src/llvm/include/llvm/Support/Error.h:282:12
#21 0x0000000008de2eed llvm::Error::operator bool() /src/llvm/include/llvm/Support/Error.h:242:16
#22 0x0000000008de2eed clang::clangd::ParsedAST::build(llvm::StringRef, clang::clangd::ParseInputs const&, std::unique_ptr<clang::CompilerInvocation, std::default_delete<clang::CompilerInvocation>>, llvm::ArrayRef<clang::clangd::Diag>, std::shared_ptr<clang::clangd::PreambleData const>) /src/clang-tools-extra/clangd/ParsedAST.cpp:713:19
#23 0x0000000008fa4868 clang::clangd::(anonymous namespace)::ASTWorker::generateDiagnostics(std::unique_ptr<clang::CompilerInvocation, std::default_delete<clang::CompilerInvocation>>, clang::clangd::ParseInputs, std::vector<clang::clangd::Diag, std::allocator<clang::clangd::Diag>>) /src/clang-tools-extra/clangd/TUScheduler.cpp:1214:39
#24 0x0000000008fa902a void std::_Destroy_aux<false>::__destroy<clang::clangd::Diag*>(clang::clangd::Diag*, clang::clangd::Diag*) /opt/rh/gcc-toolset-13/root/usr/lib/gcc/x86_64-redhat-linux/13/../../../../include/c++/13/bits/stl_construct.h:162:19
#25 0x0000000008fa902a void std::_Destroy<clang::clangd::Diag*>(clang::clangd::Diag*, clang::clangd::Diag*) /opt/rh/gcc-toolset-13/root/usr/lib/gcc/x86_64-redhat-linux/13/../../../../include/c++/13/bits/stl_construct.h:195:7
#26 0x0000000008fa902a void std::_Destroy<clang::clangd::Diag*, clang::clangd::Diag>(clang::clangd::Diag*, clang::clangd::Diag*, std::allocator<clang::clangd::Diag>&) /opt/rh/gcc-toolset-13/root/usr/lib/gcc/x86_64-redhat-linux/13/../../../../include/c++/13/bits/alloc_traits.h:948:7
#27 0x0000000008fa902a std::vector<clang::clangd::Diag, std::allocator<clang::clangd::Diag>>::~vector() /opt/rh/gcc-toolset-13/root/usr/lib/gcc/x86_64-redhat-linux/13/../../../../include/c++/13/bits/stl_vector.h:735:2
#28 0x0000000008fa902a clang::clangd::(anonymous namespace)::ASTWorker::updatePreamble(std::unique_ptr<clang::CompilerInvocation, std::default_delete<clang::CompilerInvocation>>, clang::clangd::ParseInputs, std::shared_ptr<clang::clangd::PreambleData const>, std::vector<clang::clangd::Diag, std::allocator<clang::clangd::Diag>>, clang::clangd::WantDiagnostics)::$_0::operator()() /src/clang-tools-extra/clangd/TUScheduler.cpp:1148:5
#29 0x0000000008fa6ea0 llvm::function_ref<void ()>::operator()() const /src/llvm/include/llvm/ADT/STLFunctionalExtras.h:0:12
#30 0x0000000008fa6ea0 clang::clangd::(anonymous namespace)::ASTWorker::runTask(llvm::StringRef, llvm::function_ref<void ()>) /src/clang-tools-extra/clangd/TUScheduler.cpp:1328:3
#31 0x0000000008fb0e11 clang::clangd::(anonymous namespace)::ASTWorker::run() /src/clang-tools-extra/clangd/TUScheduler.cpp:0:7
#32 0x0000000008fb0e11 clang::clangd::(anonymous namespace)::ASTWorker::create(llvm::StringRef, clang::clangd::GlobalCompilationDatabase const&, clang::clangd::TUScheduler::ASTCache&, clang::clangd::TUScheduler::HeaderIncluderCache&, clang::clangd::AsyncTaskRunner*, clang::clangd::Semaphore&, clang::clangd::TUScheduler::Options const&, clang::clangd::ParsingCallbacks&)::$_0::operator()() const /src/clang-tools-extra/clangd/TUScheduler.cpp:826:42
#33 0x0000000008fb0e11 void llvm::detail::UniqueFunctionBase<void>::CallImpl<clang::clangd::(anonymous namespace)::ASTWorker::create(llvm::StringRef, clang::clangd::GlobalCompilationDatabase const&, clang::clangd::TUScheduler::ASTCache&, clang::clangd::TUScheduler::HeaderIncluderCache&, clang::clangd::AsyncTaskRunner*, clang::clangd::Semaphore&, clang::clangd::TUScheduler::Options const&, clang::clangd::ParsingCallbacks&)::$_0>(void*) /src/llvm/include/llvm/ADT/FunctionExtras.h:222:12
#34 0x00000000092faab5 clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>)::$_1::operator()() /src/clang-tools-extra/clangd/support/Threading.cpp:103:14
#35 0x00000000092faab5 auto void llvm::thread::GenericThreadProxy<std::tuple<clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>)::$_1>>(void*)::'lambda'(auto&&, auto&&...)::operator()<clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>)::$_1&>(auto&&, auto&&...) const /src/llvm/include/llvm/Support/thread.h:43:11
#36 0x00000000092faab5 auto std::__invoke_impl<void, void llvm::thread::GenericThreadProxy<std::tuple<clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>)::$_1>>(void*)::'lambda'(auto&&, auto&&...), clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>)::$_1&>(std::__invoke_other, void llvm::thread::GenericThreadProxy<std::tuple<clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>)::$_1>>(void*)::'lambda'(auto&&, auto&&...)&&, clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>)::$_1&) /opt/rh/gcc-toolset-13/root/usr/lib/gcc/x86_64-redhat-linux/13/../../../../include/c++/13/bits/invoke.h:61:14
#37 0x00000000092faab5 std::__invoke_result<auto, auto...>::type std::__invoke<void llvm::thread::GenericThreadProxy<std::tuple<clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>)::$_1>>(void*)::'lambda'(auto&&, auto&&...), clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>)::$_1&>(auto&&, auto&&...) /opt/rh/gcc-toolset-13/root/usr/lib/gcc/x86_64-redhat-linux/13/../../../../include/c++/13/bits/invoke.h:96:14
#38 0x00000000092faab5 decltype(auto) std::__apply_impl<void llvm::thread::GenericThreadProxy<std::tuple<clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>)::$_1>>(void*)::'lambda'(auto&&, auto&&...), std::tuple<clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>)::$_1>&, 0ul>(auto&&, std::tuple<clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>)::$_1>&, std::integer_sequence<unsigned long, 0ul>) /opt/rh/gcc-toolset-13/root/usr/lib/gcc/x86_64-redhat-linux/13/../../../../include/c++/13/tuple:2302:14
#39 0x00000000092faab5 decltype(auto) std::apply<void llvm::thread::GenericThreadProxy<std::tuple<clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>)::$_1>>(void*)::'lambda'(auto&&, auto&&...), std::tuple<clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>)::$_1>&>(auto&&, std::tuple<clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>)::$_1>&) /opt/rh/gcc-toolset-13/root/usr/lib/gcc/x86_64-redhat-linux/13/../../../../include/c++/13/tuple:2313:14
#40 0x00000000092faab5 void llvm::thread::GenericThreadProxy<std::tuple<clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>)::$_1>>(void*) /src/llvm/include/llvm/Support/thread.h:41:5
#41 0x00000000092faab5 void* llvm::thread::ThreadProxy<std::tuple<clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>)::$_1>>(void*) /src/llvm/include/llvm/Support/thread.h:55:5
#42 0x00000000058765dd asan_thread_start(void*) crtstuff.c:0:0
#43 0x00007f8a8dae87e2 start_thread (/lib64/libc.so.6+0x897e2)
#44 0x00007f8a8db6d800 __GI___clone3 (/lib64/libc.so.6+0x10e800)
Signalled during AST worker action: Build AST
  Filename: main.h
  Directory: /tmp/export/input_11/workspace
  Command Line: /usr/bin/clang -xobjective-c++-header -resource-dir=/src/build/lib/clang/20 -- /tmp/export/input_11/workspace/main.h
  Version: 1
AddressSanitizer:DEADLYSIGNAL
=================================================================
==4512==ERROR: AddressSanitizer: SEGV on unknown address 0x0000000001b0 (pc 0x0000070b0719 bp 0x7f89f06cb5d0 sp 0x7f89f06cb440 T130)
==4512==The signal is caused by a READ memory access.
==4512==Hint: address points to the zero page.
    #0 0x70b0719 in clang::TypoCorrectionConsumer::getCurrentCorrection() /src/clang/include/clang/Sema/SemaInternal.h:151:12
    #1 0x70b0719 in (anonymous namespace)::TransformTypos::EmitAllDiagnostics(bool) /src/clang/lib/Sema/SemaExprCXX.cpp:8877:50
    #2 0x70b0719 in (anonymous namespace)::TransformTypos::Transform(clang::Expr*) /src/clang/lib/Sema/SemaExprCXX.cpp:9125:5
    #3 0x70b0719 in clang::Sema::CorrectDelayedTyposInExpr(clang::Expr*, clang::VarDecl*, bool, llvm::function_ref<clang::ActionResult<clang::Expr*, true> (clang::Expr*)>) /src/clang/lib/Sema/SemaExprCXX.cpp:9184:59
    #4 0x744f98f in clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) /src/clang/lib/Sema/SemaDecl.cpp:13434:22
    #5 0x102ed334 in clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) /src/clang/lib/Parse/ParseDecl.cpp:2858:17
    #6 0x102e5f6d in clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) /src/clang/lib/Parse/ParseDecl.cpp:2551:7
    #7 0x101eb081 in clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) /src/clang/lib/Parse/Parser.cpp:1244:10
    #8 0x101ea22f in clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) /src/clang/lib/Parse/Parser.cpp:1266:12
    #9 0x101e7797 in clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) /src/clang/lib/Parse/Parser.cpp:1069:14
    #10 0x101e2799 in clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) /src/clang/lib/Parse/Parser.cpp:758:12
    #11 0x101e1618 in clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) /src/clang/lib/Parse/Parser.cpp:608:26
    #12 0x101d2513 in clang::ParseAST(clang::Sema&, bool, bool) /src/clang/lib/Parse/ParseAST.cpp:170:25
    #13 0xfd2ffe7 in clang::FrontendAction::Execute() /src/clang/lib/Frontend/FrontendAction.cpp:1072:3
    #14 0x8de2eec in clang::clangd::ParsedAST::build(llvm::StringRef, clang::clangd::ParseInputs const&, std::unique_ptr<clang::CompilerInvocation, std::default_delete<clang::CompilerInvocation>>, llvm::ArrayRef<clang::clangd::Diag>, std::shared_ptr<clang::clangd::PreambleData const>) /src/clang-tools-extra/clangd/ParsedAST.cpp:713:33
    #15 0x8fa4867 in clang::clangd::(anonymous namespace)::ASTWorker::generateDiagnostics(std::unique_ptr<clang::CompilerInvocation, std::default_delete<clang::CompilerInvocation>>, clang::clangd::ParseInputs, std::vector<clang::clangd::Diag, std::allocator<clang::clangd::Diag>>) /src/clang-tools-extra/clangd/TUScheduler.cpp:1214:39
    #16 0x8fa9029 in clang::clangd::(anonymous namespace)::ASTWorker::updatePreamble(std::unique_ptr<clang::CompilerInvocation, std::default_delete<clang::CompilerInvocation>>, clang::clangd::ParseInputs, std::shared_ptr<clang::clangd::PreambleData const>, std::vector<clang::clangd::Diag, std::allocator<clang::clangd::Diag>>, clang::clangd::WantDiagnostics)::$_0::operator()() /src/clang-tools-extra/clangd/TUScheduler.cpp:1148:5
    #17 0x8fa6e9f in llvm::function_ref<void ()>::operator()() const /src/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
    #18 0x8fa6e9f in clang::clangd::(anonymous namespace)::ASTWorker::runTask(llvm::StringRef, llvm::function_ref<void ()>) /src/clang-tools-extra/clangd/TUScheduler.cpp:1328:3
    #19 0x8fb0e10 in clang::clangd::(anonymous namespace)::ASTWorker::run() /src/clang-tools-extra/clangd/TUScheduler.cpp:1462:7
    #20 0x8fb0e10 in clang::clangd::(anonymous namespace)::ASTWorker::create(llvm::StringRef, clang::clangd::GlobalCompilationDatabase const&, clang::clangd::TUScheduler::ASTCache&, clang::clangd::TUScheduler::HeaderIncluderCache&, clang::clangd::AsyncTaskRunner*, clang::clangd::Semaphore&, clang::clangd::TUScheduler::Options const&, clang::clangd::ParsingCallbacks&)::$_0::operator()() const /src/clang-tools-extra/clangd/TUScheduler.cpp:826:42
    #21 0x8fb0e10 in void llvm::detail::UniqueFunctionBase<void>::CallImpl<clang::clangd::(anonymous namespace)::ASTWorker::create(llvm::StringRef, clang::clangd::GlobalCompilationDatabase const&, clang::clangd::TUScheduler::ASTCache&, clang::clangd::TUScheduler::HeaderIncluderCache&, clang::clangd::AsyncTaskRunner*, clang::clangd::Semaphore&, clang::clangd::TUScheduler::Options const&, clang::clangd::ParsingCallbacks&)::$_0>(void*) /src/llvm/include/llvm/ADT/FunctionExtras.h:222:12
    #22 0x92faab4 in llvm::unique_function<void ()>::operator()() /src/llvm/include/llvm/ADT/FunctionExtras.h:387:12
    #23 0x92faab4 in clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>)::$_1::operator()() /src/clang-tools-extra/clangd/support/Threading.cpp:101:5
    #24 0x92faab4 in auto void llvm::thread::GenericThreadProxy<std::tuple<clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>)::$_1>>(void*)::'lambda'(auto&&, auto&&...)::operator()<clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>)::$_1&>(auto&&, auto&&...) const /src/llvm/include/llvm/Support/thread.h:43:11
    #25 0x92faab4 in auto std::__invoke_impl<void, void llvm::thread::GenericThreadProxy<std::tuple<clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>)::$_1>>(void*)::'lambda'(auto&&, auto&&...), clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>)::$_1&>(std::__invoke_other, void llvm::thread::GenericThreadProxy<std::tuple<clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>)::$_1>>(void*)::'lambda'(auto&&, auto&&...)&&, clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>)::$_1&) /opt/rh/gcc-toolset-13/root/usr/lib/gcc/x86_64-redhat-linux/13/../../../../include/c++/13/bits/invoke.h:61:14
    #26 0x92faab4 in std::__invoke_result<auto, auto...>::type std::__invoke<void llvm::thread::GenericThreadProxy<std::tuple<clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>)::$_1>>(void*)::'lambda'(auto&&, auto&&...), clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>)::$_1&>(auto&&, auto&&...) /opt/rh/gcc-toolset-13/root/usr/lib/gcc/x86_64-redhat-linux/13/../../../../include/c++/13/bits/invoke.h:96:14
    #27 0x92faab4 in decltype(auto) std::__apply_impl<void llvm::thread::GenericThreadProxy<std::tuple<clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>)::$_1>>(void*)::'lambda'(auto&&, auto&&...), std::tuple<clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>)::$_1>&, 0ul>(auto&&, std::tuple<clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>)::$_1>&, std::integer_sequence<unsigned long, 0ul>) /opt/rh/gcc-toolset-13/root/usr/lib/gcc/x86_64-redhat-linux/13/../../../../include/c++/13/tuple:2302:14
    #28 0x92faab4 in decltype(auto) std::apply<void llvm::thread::GenericThreadProxy<std::tuple<clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>)::$_1>>(void*)::'lambda'(auto&&, auto&&...), std::tuple<clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>)::$_1>&>(auto&&, std::tuple<clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>)::$_1>&) /opt/rh/gcc-toolset-13/root/usr/lib/gcc/x86_64-redhat-linux/13/../../../../include/c++/13/tuple:2313:14
    #29 0x92faab4 in void llvm::thread::GenericThreadProxy<std::tuple<clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>)::$_1>>(void*) /src/llvm/include/llvm/Support/thread.h:41:5
    #30 0x92faab4 in void* llvm::thread::ThreadProxy<std::tuple<clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>)::$_1>>(void*) /src/llvm/include/llvm/Support/thread.h:55:5
    #31 0x58765dc in asan_thread_start(void*) crtstuff.c
    #32 0x7f8a8dae87e1 in start_thread (/lib64/libc.so.6+0x897e1) (BuildId: 7a40a22c9a82854f3d66767232ae364a99174860)
    #33 0x7f8a8db6d7ff in __GI___clone3 (/lib64/libc.so.6+0x10e7ff) (BuildId: 7a40a22c9a82854f3d66767232ae364a99174860)

AddressSanitizer can not provide additional info.
SUMMARY: AddressSanitizer: SEGV /src/clang/include/clang/Sema/SemaInternal.h:151:12 in clang::TypoCorrectionConsumer::getCurrentCorrection()
Thread T130 created by T0 here:
    #0 0x585fd95 in pthread_create (/src/build/bin/clangd+0x585fd95) (BuildId: c3ce5beed26c9a8a)
    #1 0x5dfe568 in llvm::llvm_execute_on_thread_impl(void* (*)(void*), void*, std::optional<unsigned int>) /src/llvm/lib/Support/Unix/Threading.inc:96:17
    #2 0x92fa751 in llvm::thread::thread<clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>)::$_1>(std::optional<unsigned int>, clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>)::$_1&&) /src/llvm/include/llvm/Support/thread.h:131:12
    #3 0x92fa751 in clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>) /src/clang-tools-extra/clangd/support/Threading.cpp:107:16
    #4 0x8fb9f5b in clang::clangd::(anonymous namespace)::ASTWorker::create(llvm::StringRef, clang::clangd::GlobalCompilationDatabase const&, clang::clangd::TUScheduler::ASTCache&, clang::clangd::TUScheduler::HeaderIncluderCache&, clang::clangd::AsyncTaskRunner*, clang::clangd::Semaphore&, clang::clangd::TUScheduler::Options const&, clang::clangd::ParsingCallbacks&) /src/clang-tools-extra/clangd/TUScheduler.cpp:825:12
    #5 0x8fb9f5b in clang::clangd::TUScheduler::update(llvm::StringRef, clang::clangd::ParseInputs, clang::clangd::WantDiagnostics) /src/clang-tools-extra/clangd/TUScheduler.cpp:1681:30
    #6 0x8a8637e in clang::clangd::ClangdServer::addDocument(llvm::StringRef, llvm::StringRef, llvm::StringRef, clang::clangd::WantDiagnostics, bool) /src/clang-tools-extra/clangd/ClangdServer.cpp:316:33
    #7 0x89d89f2 in clang::clangd::ClangdLSPServer::onDocumentDidOpen(clang::clangd::DidOpenTextDocumentParams const&) /src/clang-tools-extra/clangd/ClangdLSPServer.cpp:726:11
    #8 0x8a1558f in void clang::clangd::LSPBinder::notification<clang::clangd::DidOpenTextDocumentParams, clang::clangd::ClangdLSPServer>(llvm::StringLiteral, clang::clangd::ClangdLSPServer*, void (clang::clangd::ClangdLSPServer::*)(clang::clangd::DidOpenTextDocumentParams const&))::'lambda'(llvm::json::Value)::operator()(llvm::json::Value) const /src/clang-tools-extra/clangd/LSPBinder.h:153:5
    #9 0x8a1537e in void llvm::detail::UniqueFunctionBase<void, llvm::json::Value>::CallImpl<void clang::clangd::LSPBinder::notification<clang::clangd::DidOpenTextDocumentParams, clang::clangd::ClangdLSPServer>(llvm::StringLiteral, clang::clangd::ClangdLSPServer*, void (clang::clangd::ClangdLSPServer::*)(clang::clangd::DidOpenTextDocumentParams const&))::'lambda'(llvm::json::Value)>(void*, llvm::json::Value&) /src/llvm/include/llvm/ADT/FunctionExtras.h:222:12
    #10 0x8a3cc92 in llvm::unique_function<void (llvm::json::Value)>::operator()(llvm::json::Value) /src/llvm/include/llvm/ADT/FunctionExtras.h:387:12
    #11 0x8a3cc92 in clang::clangd::ClangdLSPServer::MessageHandler::onNotify(llvm::StringRef, llvm::json::Value) /src/clang-tools-extra/clangd/ClangdLSPServer.cpp:219:7
    #12 0x8d76cc7 in clang::clangd::(anonymous namespace)::JSONTransport::handleMessage(llvm::json::Value, clang::clangd::Transport::MessageHandler&) /src/clang-tools-extra/clangd/JSONTransport.cpp:195:18
    #13 0x8d76cc7 in clang::clangd::(anonymous namespace)::JSONTransport::loop(clang::clangd::Transport::MessageHandler&) /src/clang-tools-extra/clangd/JSONTransport.cpp:119:16
    #14 0x8a47cd9 in clang::clangd::ClangdLSPServer::run() /src/clang-tools-extra/clangd/ClangdLSPServer.cpp:1741:25
    #15 0x88915c9 in clang::clangd::clangdMain(int, char**) /src/clang-tools-extra/clangd/tool/ClangdMain.cpp:1049:28
    #16 0x7f8a8da885cf in __libc_start_call_main (/lib64/libc.so.6+0x295cf) (BuildId: 7a40a22c9a82854f3d66767232ae364a99174860)

==4512==ABORTING

```

## Comments

### Comment 1 - AaronBallman

Closing as this issue appears to have been fixed on Clang trunk: https://godbolt.org/z/67xYbT6Gz -- if you're still able to reproduce on trunk, please reopen the issue with more details.

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Henry Chu (henryhchchc)

<details>
Create a file named `main.cpp` with the following statement.

```c++
 auto foo = E-&gt;bar(baz,x,,);
```

Open it with a code editor with clangd enabled and you will see clangd crashes with the following stack trace. It also crashes clang. Looks like a problem in the front-end.

```
I[12:45:32.470] clangd version 20.1.2 (https://github.com/llvm/llvm-project.git 58df0ef89dd64126512e4ee27b4ac3fd8ddf6247)
I[12:45:32.470] Features: linux+asan
I[12:45:32.470] PID: 4512
I[12:45:32.470] Working directory: /tmp/export/input_11
I[12:45:32.470] argv[0]: /src/build/bin/clangd
I[12:45:32.470] Starting LSP over stdin/stdout

...

PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.      /tmp/export/input_11/workspace/main.h:1:70: current parser token ';'
 #<!-- -->0 0x0000000005823eea ___interceptor_backtrace (/src/build/bin/clangd+0x5823eea)
 #<!-- -->1 0x0000000005dfc7e7 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /src/llvm/lib/Support/Unix/Signals.inc:799:13
 #<!-- -->2 0x0000000005dfbbac llvm::sys::RunSignalHandlers() /src/llvm/lib/Support/Signals.cpp:0:5
 #<!-- -->3 0x0000000005dfbbac SignalHandler(int) /src/llvm/lib/Support/Unix/Signals.inc:405:3
 #<!-- -->4 0x00007f8a8da9d730 __restore_rt (/lib64/libc.so.6+0x3e730)
 #<!-- -->5 0x00000000070b0719 clang::TypoCorrectionConsumer::getCurrentCorrection() /src/clang/include/clang/Sema/SemaInternal.h:151:12
 #<!-- -->6 0x00000000070b0719 (anonymous namespace)::TransformTypos::EmitAllDiagnostics(bool) /src/clang/lib/Sema/SemaExprCXX.cpp:8877:50
 #<!-- -->7 0x00000000070b0719 (anonymous namespace)::TransformTypos::Transform(clang::Expr*) /src/clang/lib/Sema/SemaExprCXX.cpp:9125:5
 #<!-- -->8 0x00000000070b0719 clang::Sema::CorrectDelayedTyposInExpr(clang::Expr*, clang::VarDecl*, bool, llvm::function_ref&lt;clang::ActionResult&lt;clang::Expr*, true&gt; (clang::Expr*)&gt;) /src/clang/lib/Sema/SemaExprCXX.cpp:9184:59
 #<!-- -->9 0x000000000744f990 clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) /src/clang/lib/Sema/SemaDecl.cpp:13434:22
#<!-- -->10 0x00000000102ed335 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) /src/clang/lib/Parse/ParseDecl.cpp:0:17
#<!-- -->11 0x00000000102e5f6e clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) /src/clang/lib/Parse/ParseDecl.cpp:2551:7
#<!-- -->12 0x00000000101eb082 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) /src/clang/lib/Parse/Parser.cpp:1244:10
#<!-- -->13 0x00000000101ea230 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) /src/clang/lib/Parse/Parser.cpp:0:12
#<!-- -->14 0x00000000101e7798 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) /src/clang/lib/Parse/Parser.cpp:0:14
#<!-- -->15 0x00000000101e279a clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) /src/clang/lib/Parse/Parser.cpp:0:12
#<!-- -->16 0x00000000101e1619 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) /src/clang/lib/Parse/Parser.cpp:615:23
#<!-- -->17 0x00000000101d2514 clang::ParseAST(clang::Sema&amp;, bool, bool) /src/clang/lib/Parse/ParseAST.cpp:170:5
#<!-- -->18 0x000000000fd2ffe8 clang::CompilerInstance::shouldBuildGlobalModuleIndex() const /src/clang/lib/Frontend/CompilerInstance.cpp:86:11
#<!-- -->19 0x000000000fd2ffe8 clang::FrontendAction::Execute() /src/clang/lib/Frontend/FrontendAction.cpp:1076:10
#<!-- -->20 0x0000000008de2eed llvm::Error::getPtr() const /src/llvm/include/llvm/Support/Error.h:282:12
#<!-- -->21 0x0000000008de2eed llvm::Error::operator bool() /src/llvm/include/llvm/Support/Error.h:242:16
#<!-- -->22 0x0000000008de2eed clang::clangd::ParsedAST::build(llvm::StringRef, clang::clangd::ParseInputs const&amp;, std::unique_ptr&lt;clang::CompilerInvocation, std::default_delete&lt;clang::CompilerInvocation&gt;&gt;, llvm::ArrayRef&lt;clang::clangd::Diag&gt;, std::shared_ptr&lt;clang::clangd::PreambleData const&gt;) /src/clang-tools-extra/clangd/ParsedAST.cpp:713:19
#<!-- -->23 0x0000000008fa4868 clang::clangd::(anonymous namespace)::ASTWorker::generateDiagnostics(std::unique_ptr&lt;clang::CompilerInvocation, std::default_delete&lt;clang::CompilerInvocation&gt;&gt;, clang::clangd::ParseInputs, std::vector&lt;clang::clangd::Diag, std::allocator&lt;clang::clangd::Diag&gt;&gt;) /src/clang-tools-extra/clangd/TUScheduler.cpp:1214:39
#<!-- -->24 0x0000000008fa902a void std::_Destroy_aux&lt;false&gt;::__destroy&lt;clang::clangd::Diag*&gt;(clang::clangd::Diag*, clang::clangd::Diag*) /opt/rh/gcc-toolset-13/root/usr/lib/gcc/x86_64-redhat-linux/13/../../../../include/c++/13/bits/stl_construct.h:162:19
#<!-- -->25 0x0000000008fa902a void std::_Destroy&lt;clang::clangd::Diag*&gt;(clang::clangd::Diag*, clang::clangd::Diag*) /opt/rh/gcc-toolset-13/root/usr/lib/gcc/x86_64-redhat-linux/13/../../../../include/c++/13/bits/stl_construct.h:195:7
#<!-- -->26 0x0000000008fa902a void std::_Destroy&lt;clang::clangd::Diag*, clang::clangd::Diag&gt;(clang::clangd::Diag*, clang::clangd::Diag*, std::allocator&lt;clang::clangd::Diag&gt;&amp;) /opt/rh/gcc-toolset-13/root/usr/lib/gcc/x86_64-redhat-linux/13/../../../../include/c++/13/bits/alloc_traits.h:948:7
#<!-- -->27 0x0000000008fa902a std::vector&lt;clang::clangd::Diag, std::allocator&lt;clang::clangd::Diag&gt;&gt;::~vector() /opt/rh/gcc-toolset-13/root/usr/lib/gcc/x86_64-redhat-linux/13/../../../../include/c++/13/bits/stl_vector.h:735:2
#<!-- -->28 0x0000000008fa902a clang::clangd::(anonymous namespace)::ASTWorker::updatePreamble(std::unique_ptr&lt;clang::CompilerInvocation, std::default_delete&lt;clang::CompilerInvocation&gt;&gt;, clang::clangd::ParseInputs, std::shared_ptr&lt;clang::clangd::PreambleData const&gt;, std::vector&lt;clang::clangd::Diag, std::allocator&lt;clang::clangd::Diag&gt;&gt;, clang::clangd::WantDiagnostics)::$_0::operator()() /src/clang-tools-extra/clangd/TUScheduler.cpp:1148:5
#<!-- -->29 0x0000000008fa6ea0 llvm::function_ref&lt;void ()&gt;::operator()() const /src/llvm/include/llvm/ADT/STLFunctionalExtras.h:0:12
#<!-- -->30 0x0000000008fa6ea0 clang::clangd::(anonymous namespace)::ASTWorker::runTask(llvm::StringRef, llvm::function_ref&lt;void ()&gt;) /src/clang-tools-extra/clangd/TUScheduler.cpp:1328:3
#<!-- -->31 0x0000000008fb0e11 clang::clangd::(anonymous namespace)::ASTWorker::run() /src/clang-tools-extra/clangd/TUScheduler.cpp:0:7
#<!-- -->32 0x0000000008fb0e11 clang::clangd::(anonymous namespace)::ASTWorker::create(llvm::StringRef, clang::clangd::GlobalCompilationDatabase const&amp;, clang::clangd::TUScheduler::ASTCache&amp;, clang::clangd::TUScheduler::HeaderIncluderCache&amp;, clang::clangd::AsyncTaskRunner*, clang::clangd::Semaphore&amp;, clang::clangd::TUScheduler::Options const&amp;, clang::clangd::ParsingCallbacks&amp;)::$_0::operator()() const /src/clang-tools-extra/clangd/TUScheduler.cpp:826:42
#<!-- -->33 0x0000000008fb0e11 void llvm::detail::UniqueFunctionBase&lt;void&gt;::CallImpl&lt;clang::clangd::(anonymous namespace)::ASTWorker::create(llvm::StringRef, clang::clangd::GlobalCompilationDatabase const&amp;, clang::clangd::TUScheduler::ASTCache&amp;, clang::clangd::TUScheduler::HeaderIncluderCache&amp;, clang::clangd::AsyncTaskRunner*, clang::clangd::Semaphore&amp;, clang::clangd::TUScheduler::Options const&amp;, clang::clangd::ParsingCallbacks&amp;)::$_0&gt;(void*) /src/llvm/include/llvm/ADT/FunctionExtras.h:222:12
#<!-- -->34 0x00000000092faab5 clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1::operator()() /src/clang-tools-extra/clangd/support/Threading.cpp:103:14
#<!-- -->35 0x00000000092faab5 auto void llvm::thread::GenericThreadProxy&lt;std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&gt;&gt;(void*)::'lambda'(auto&amp;&amp;, auto&amp;&amp;...)::operator()&lt;clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&amp;&gt;(auto&amp;&amp;, auto&amp;&amp;...) const /src/llvm/include/llvm/Support/thread.h:43:11
#<!-- -->36 0x00000000092faab5 auto std::__invoke_impl&lt;void, void llvm::thread::GenericThreadProxy&lt;std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&gt;&gt;(void*)::'lambda'(auto&amp;&amp;, auto&amp;&amp;...), clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&amp;&gt;(std::__invoke_other, void llvm::thread::GenericThreadProxy&lt;std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&gt;&gt;(void*)::'lambda'(auto&amp;&amp;, auto&amp;&amp;...)&amp;&amp;, clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&amp;) /opt/rh/gcc-toolset-13/root/usr/lib/gcc/x86_64-redhat-linux/13/../../../../include/c++/13/bits/invoke.h:61:14
#<!-- -->37 0x00000000092faab5 std::__invoke_result&lt;auto, auto...&gt;::type std::__invoke&lt;void llvm::thread::GenericThreadProxy&lt;std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&gt;&gt;(void*)::'lambda'(auto&amp;&amp;, auto&amp;&amp;...), clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&amp;&gt;(auto&amp;&amp;, auto&amp;&amp;...) /opt/rh/gcc-toolset-13/root/usr/lib/gcc/x86_64-redhat-linux/13/../../../../include/c++/13/bits/invoke.h:96:14
#<!-- -->38 0x00000000092faab5 decltype(auto) std::__apply_impl&lt;void llvm::thread::GenericThreadProxy&lt;std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&gt;&gt;(void*)::'lambda'(auto&amp;&amp;, auto&amp;&amp;...), std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&gt;&amp;, 0ul&gt;(auto&amp;&amp;, std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&gt;&amp;, std::integer_sequence&lt;unsigned long, 0ul&gt;) /opt/rh/gcc-toolset-13/root/usr/lib/gcc/x86_64-redhat-linux/13/../../../../include/c++/13/tuple:2302:14
#<!-- -->39 0x00000000092faab5 decltype(auto) std::apply&lt;void llvm::thread::GenericThreadProxy&lt;std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&gt;&gt;(void*)::'lambda'(auto&amp;&amp;, auto&amp;&amp;...), std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&gt;&amp;&gt;(auto&amp;&amp;, std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&gt;&amp;) /opt/rh/gcc-toolset-13/root/usr/lib/gcc/x86_64-redhat-linux/13/../../../../include/c++/13/tuple:2313:14
#<!-- -->40 0x00000000092faab5 void llvm::thread::GenericThreadProxy&lt;std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&gt;&gt;(void*) /src/llvm/include/llvm/Support/thread.h:41:5
#<!-- -->41 0x00000000092faab5 void* llvm::thread::ThreadProxy&lt;std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&gt;&gt;(void*) /src/llvm/include/llvm/Support/thread.h:55:5
#<!-- -->42 0x00000000058765dd asan_thread_start(void*) crtstuff.c:0:0
#<!-- -->43 0x00007f8a8dae87e2 start_thread (/lib64/libc.so.6+0x897e2)
#<!-- -->44 0x00007f8a8db6d800 __GI___clone3 (/lib64/libc.so.6+0x10e800)
Signalled during AST worker action: Build AST
  Filename: main.h
  Directory: /tmp/export/input_11/workspace
  Command Line: /usr/bin/clang -xobjective-c++-header -resource-dir=/src/build/lib/clang/20 -- /tmp/export/input_11/workspace/main.h
  Version: 1
AddressSanitizer:DEADLYSIGNAL
=================================================================
==4512==ERROR: AddressSanitizer: SEGV on unknown address 0x0000000001b0 (pc 0x0000070b0719 bp 0x7f89f06cb5d0 sp 0x7f89f06cb440 T130)
==4512==The signal is caused by a READ memory access.
==4512==Hint: address points to the zero page.
    #<!-- -->0 0x70b0719 in clang::TypoCorrectionConsumer::getCurrentCorrection() /src/clang/include/clang/Sema/SemaInternal.h:151:12
    #<!-- -->1 0x70b0719 in (anonymous namespace)::TransformTypos::EmitAllDiagnostics(bool) /src/clang/lib/Sema/SemaExprCXX.cpp:8877:50
    #<!-- -->2 0x70b0719 in (anonymous namespace)::TransformTypos::Transform(clang::Expr*) /src/clang/lib/Sema/SemaExprCXX.cpp:9125:5
    #<!-- -->3 0x70b0719 in clang::Sema::CorrectDelayedTyposInExpr(clang::Expr*, clang::VarDecl*, bool, llvm::function_ref&lt;clang::ActionResult&lt;clang::Expr*, true&gt; (clang::Expr*)&gt;) /src/clang/lib/Sema/SemaExprCXX.cpp:9184:59
    #<!-- -->4 0x744f98f in clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) /src/clang/lib/Sema/SemaDecl.cpp:13434:22
    #<!-- -->5 0x102ed334 in clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) /src/clang/lib/Parse/ParseDecl.cpp:2858:17
    #<!-- -->6 0x102e5f6d in clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) /src/clang/lib/Parse/ParseDecl.cpp:2551:7
    #<!-- -->7 0x101eb081 in clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) /src/clang/lib/Parse/Parser.cpp:1244:10
    #<!-- -->8 0x101ea22f in clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) /src/clang/lib/Parse/Parser.cpp:1266:12
    #<!-- -->9 0x101e7797 in clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) /src/clang/lib/Parse/Parser.cpp:1069:14
    #<!-- -->10 0x101e2799 in clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) /src/clang/lib/Parse/Parser.cpp:758:12
    #<!-- -->11 0x101e1618 in clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) /src/clang/lib/Parse/Parser.cpp:608:26
    #<!-- -->12 0x101d2513 in clang::ParseAST(clang::Sema&amp;, bool, bool) /src/clang/lib/Parse/ParseAST.cpp:170:25
    #<!-- -->13 0xfd2ffe7 in clang::FrontendAction::Execute() /src/clang/lib/Frontend/FrontendAction.cpp:1072:3
    #<!-- -->14 0x8de2eec in clang::clangd::ParsedAST::build(llvm::StringRef, clang::clangd::ParseInputs const&amp;, std::unique_ptr&lt;clang::CompilerInvocation, std::default_delete&lt;clang::CompilerInvocation&gt;&gt;, llvm::ArrayRef&lt;clang::clangd::Diag&gt;, std::shared_ptr&lt;clang::clangd::PreambleData const&gt;) /src/clang-tools-extra/clangd/ParsedAST.cpp:713:33
    #<!-- -->15 0x8fa4867 in clang::clangd::(anonymous namespace)::ASTWorker::generateDiagnostics(std::unique_ptr&lt;clang::CompilerInvocation, std::default_delete&lt;clang::CompilerInvocation&gt;&gt;, clang::clangd::ParseInputs, std::vector&lt;clang::clangd::Diag, std::allocator&lt;clang::clangd::Diag&gt;&gt;) /src/clang-tools-extra/clangd/TUScheduler.cpp:1214:39
    #<!-- -->16 0x8fa9029 in clang::clangd::(anonymous namespace)::ASTWorker::updatePreamble(std::unique_ptr&lt;clang::CompilerInvocation, std::default_delete&lt;clang::CompilerInvocation&gt;&gt;, clang::clangd::ParseInputs, std::shared_ptr&lt;clang::clangd::PreambleData const&gt;, std::vector&lt;clang::clangd::Diag, std::allocator&lt;clang::clangd::Diag&gt;&gt;, clang::clangd::WantDiagnostics)::$_0::operator()() /src/clang-tools-extra/clangd/TUScheduler.cpp:1148:5
    #<!-- -->17 0x8fa6e9f in llvm::function_ref&lt;void ()&gt;::operator()() const /src/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
    #<!-- -->18 0x8fa6e9f in clang::clangd::(anonymous namespace)::ASTWorker::runTask(llvm::StringRef, llvm::function_ref&lt;void ()&gt;) /src/clang-tools-extra/clangd/TUScheduler.cpp:1328:3
    #<!-- -->19 0x8fb0e10 in clang::clangd::(anonymous namespace)::ASTWorker::run() /src/clang-tools-extra/clangd/TUScheduler.cpp:1462:7
    #<!-- -->20 0x8fb0e10 in clang::clangd::(anonymous namespace)::ASTWorker::create(llvm::StringRef, clang::clangd::GlobalCompilationDatabase const&amp;, clang::clangd::TUScheduler::ASTCache&amp;, clang::clangd::TUScheduler::HeaderIncluderCache&amp;, clang::clangd::AsyncTaskRunner*, clang::clangd::Semaphore&amp;, clang::clangd::TUScheduler::Options const&amp;, clang::clangd::ParsingCallbacks&amp;)::$_0::operator()() const /src/clang-tools-extra/clangd/TUScheduler.cpp:826:42
    #<!-- -->21 0x8fb0e10 in void llvm::detail::UniqueFunctionBase&lt;void&gt;::CallImpl&lt;clang::clangd::(anonymous namespace)::ASTWorker::create(llvm::StringRef, clang::clangd::GlobalCompilationDatabase const&amp;, clang::clangd::TUScheduler::ASTCache&amp;, clang::clangd::TUScheduler::HeaderIncluderCache&amp;, clang::clangd::AsyncTaskRunner*, clang::clangd::Semaphore&amp;, clang::clangd::TUScheduler::Options const&amp;, clang::clangd::ParsingCallbacks&amp;)::$_0&gt;(void*) /src/llvm/include/llvm/ADT/FunctionExtras.h:222:12
    #<!-- -->22 0x92faab4 in llvm::unique_function&lt;void ()&gt;::operator()() /src/llvm/include/llvm/ADT/FunctionExtras.h:387:12
    #<!-- -->23 0x92faab4 in clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1::operator()() /src/clang-tools-extra/clangd/support/Threading.cpp:101:5
    #<!-- -->24 0x92faab4 in auto void llvm::thread::GenericThreadProxy&lt;std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&gt;&gt;(void*)::'lambda'(auto&amp;&amp;, auto&amp;&amp;...)::operator()&lt;clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&amp;&gt;(auto&amp;&amp;, auto&amp;&amp;...) const /src/llvm/include/llvm/Support/thread.h:43:11
    #<!-- -->25 0x92faab4 in auto std::__invoke_impl&lt;void, void llvm::thread::GenericThreadProxy&lt;std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&gt;&gt;(void*)::'lambda'(auto&amp;&amp;, auto&amp;&amp;...), clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&amp;&gt;(std::__invoke_other, void llvm::thread::GenericThreadProxy&lt;std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&gt;&gt;(void*)::'lambda'(auto&amp;&amp;, auto&amp;&amp;...)&amp;&amp;, clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&amp;) /opt/rh/gcc-toolset-13/root/usr/lib/gcc/x86_64-redhat-linux/13/../../../../include/c++/13/bits/invoke.h:61:14
    #<!-- -->26 0x92faab4 in std::__invoke_result&lt;auto, auto...&gt;::type std::__invoke&lt;void llvm::thread::GenericThreadProxy&lt;std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&gt;&gt;(void*)::'lambda'(auto&amp;&amp;, auto&amp;&amp;...), clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&amp;&gt;(auto&amp;&amp;, auto&amp;&amp;...) /opt/rh/gcc-toolset-13/root/usr/lib/gcc/x86_64-redhat-linux/13/../../../../include/c++/13/bits/invoke.h:96:14
    #<!-- -->27 0x92faab4 in decltype(auto) std::__apply_impl&lt;void llvm::thread::GenericThreadProxy&lt;std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&gt;&gt;(void*)::'lambda'(auto&amp;&amp;, auto&amp;&amp;...), std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&gt;&amp;, 0ul&gt;(auto&amp;&amp;, std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&gt;&amp;, std::integer_sequence&lt;unsigned long, 0ul&gt;) /opt/rh/gcc-toolset-13/root/usr/lib/gcc/x86_64-redhat-linux/13/../../../../include/c++/13/tuple:2302:14
    #<!-- -->28 0x92faab4 in decltype(auto) std::apply&lt;void llvm::thread::GenericThreadProxy&lt;std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&gt;&gt;(void*)::'lambda'(auto&amp;&amp;, auto&amp;&amp;...), std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&gt;&amp;&gt;(auto&amp;&amp;, std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&gt;&amp;) /opt/rh/gcc-toolset-13/root/usr/lib/gcc/x86_64-redhat-linux/13/../../../../include/c++/13/tuple:2313:14
    #<!-- -->29 0x92faab4 in void llvm::thread::GenericThreadProxy&lt;std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&gt;&gt;(void*) /src/llvm/include/llvm/Support/thread.h:41:5
    #<!-- -->30 0x92faab4 in void* llvm::thread::ThreadProxy&lt;std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&gt;&gt;(void*) /src/llvm/include/llvm/Support/thread.h:55:5
    #<!-- -->31 0x58765dc in asan_thread_start(void*) crtstuff.c
    #<!-- -->32 0x7f8a8dae87e1 in start_thread (/lib64/libc.so.6+0x897e1) (BuildId: 7a40a22c9a82854f3d66767232ae364a99174860)
    #<!-- -->33 0x7f8a8db6d7ff in __GI___clone3 (/lib64/libc.so.6+0x10e7ff) (BuildId: 7a40a22c9a82854f3d66767232ae364a99174860)

AddressSanitizer can not provide additional info.
SUMMARY: AddressSanitizer: SEGV /src/clang/include/clang/Sema/SemaInternal.h:151:12 in clang::TypoCorrectionConsumer::getCurrentCorrection()
Thread T130 created by T0 here:
    #<!-- -->0 0x585fd95 in pthread_create (/src/build/bin/clangd+0x585fd95) (BuildId: c3ce5beed26c9a8a)
    #<!-- -->1 0x5dfe568 in llvm::llvm_execute_on_thread_impl(void* (*)(void*), void*, std::optional&lt;unsigned int&gt;) /src/llvm/lib/Support/Unix/Threading.inc:96:17
    #<!-- -->2 0x92fa751 in llvm::thread::thread&lt;clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&gt;(std::optional&lt;unsigned int&gt;, clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&amp;&amp;) /src/llvm/include/llvm/Support/thread.h:131:12
    #<!-- -->3 0x92fa751 in clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;) /src/clang-tools-extra/clangd/support/Threading.cpp:107:16
    #<!-- -->4 0x8fb9f5b in clang::clangd::(anonymous namespace)::ASTWorker::create(llvm::StringRef, clang::clangd::GlobalCompilationDatabase const&amp;, clang::clangd::TUScheduler::ASTCache&amp;, clang::clangd::TUScheduler::HeaderIncluderCache&amp;, clang::clangd::AsyncTaskRunner*, clang::clangd::Semaphore&amp;, clang::clangd::TUScheduler::Options const&amp;, clang::clangd::ParsingCallbacks&amp;) /src/clang-tools-extra/clangd/TUScheduler.cpp:825:12
    #<!-- -->5 0x8fb9f5b in clang::clangd::TUScheduler::update(llvm::StringRef, clang::clangd::ParseInputs, clang::clangd::WantDiagnostics) /src/clang-tools-extra/clangd/TUScheduler.cpp:1681:30
    #<!-- -->6 0x8a8637e in clang::clangd::ClangdServer::addDocument(llvm::StringRef, llvm::StringRef, llvm::StringRef, clang::clangd::WantDiagnostics, bool) /src/clang-tools-extra/clangd/ClangdServer.cpp:316:33
    #<!-- -->7 0x89d89f2 in clang::clangd::ClangdLSPServer::onDocumentDidOpen(clang::clangd::DidOpenTextDocumentParams const&amp;) /src/clang-tools-extra/clangd/ClangdLSPServer.cpp:726:11
    #<!-- -->8 0x8a1558f in void clang::clangd::LSPBinder::notification&lt;clang::clangd::DidOpenTextDocumentParams, clang::clangd::ClangdLSPServer&gt;(llvm::StringLiteral, clang::clangd::ClangdLSPServer*, void (clang::clangd::ClangdLSPServer::*)(clang::clangd::DidOpenTextDocumentParams const&amp;))::'lambda'(llvm::json::Value)::operator()(llvm::json::Value) const /src/clang-tools-extra/clangd/LSPBinder.h:153:5
    #<!-- -->9 0x8a1537e in void llvm::detail::UniqueFunctionBase&lt;void, llvm::json::Value&gt;::CallImpl&lt;void clang::clangd::LSPBinder::notification&lt;clang::clangd::DidOpenTextDocumentParams, clang::clangd::ClangdLSPServer&gt;(llvm::StringLiteral, clang::clangd::ClangdLSPServer*, void (clang::clangd::ClangdLSPServer::*)(clang::clangd::DidOpenTextDocumentParams const&amp;))::'lambda'(llvm::json::Value)&gt;(void*, llvm::json::Value&amp;) /src/llvm/include/llvm/ADT/FunctionExtras.h:222:12
    #<!-- -->10 0x8a3cc92 in llvm::unique_function&lt;void (llvm::json::Value)&gt;::operator()(llvm::json::Value) /src/llvm/include/llvm/ADT/FunctionExtras.h:387:12
    #<!-- -->11 0x8a3cc92 in clang::clangd::ClangdLSPServer::MessageHandler::onNotify(llvm::StringRef, llvm::json::Value) /src/clang-tools-extra/clangd/ClangdLSPServer.cpp:219:7
    #<!-- -->12 0x8d76cc7 in clang::clangd::(anonymous namespace)::JSONTransport::handleMessage(llvm::json::Value, clang::clangd::Transport::MessageHandler&amp;) /src/clang-tools-extra/clangd/JSONTransport.cpp:195:18
    #<!-- -->13 0x8d76cc7 in clang::clangd::(anonymous namespace)::JSONTransport::loop(clang::clangd::Transport::MessageHandler&amp;) /src/clang-tools-extra/clangd/JSONTransport.cpp:119:16
    #<!-- -->14 0x8a47cd9 in clang::clangd::ClangdLSPServer::run() /src/clang-tools-extra/clangd/ClangdLSPServer.cpp:1741:25
    #<!-- -->15 0x88915c9 in clang::clangd::clangdMain(int, char**) /src/clang-tools-extra/clangd/tool/ClangdMain.cpp:1049:28
    #<!-- -->16 0x7f8a8da885cf in __libc_start_call_main (/lib64/libc.so.6+0x295cf) (BuildId: 7a40a22c9a82854f3d66767232ae364a99174860)

==4512==ABORTING

```
</details>


---

