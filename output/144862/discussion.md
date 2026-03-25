# Intermittent crash, backtrace provided as requested

**Author:** tonygould
**URL:** https://github.com/llvm/llvm-project/issues/144862
**Status:** Closed
**Labels:** clang-tidy, clang:frontend, crash, needs-reduction
**Closed Date:** 2025-07-21T20:48:42Z

## Body

Sometimes succeeds, sometimes fails with different backtraces

e.g. (1)

```
[build] Error running '/usr/bin/clang-tidy': PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
[build] Stack dump:
[build] 0.	Program arguments: /usr/bin/clang-tidy -p /home/dev/snip/out/build/clang-tidy -header-filter=/home/dev/snip -warnings-as-errors=* --extra-arg-before=--driver-mode=g++ /home/dev/snip/snip.cpp
[build] 1.	<eof> parser at end of file
[build] 2.	ASTMatcher: Matching 'cppcoreguidelines-macro-to-enum' against:
[build] 	ParmVarDecl  : </usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_deque.h:2232:53>
[build]  #0 0x00007f9af2d923bf llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) build-llvm/tools/clang/stage2-bins/llvm/lib/Support/Unix/Signals.inc:723:13
[build]  #1 0x00007f9af2d904f9 llvm::sys::RunSignalHandlers() build-llvm/tools/clang/stage2-bins/llvm/lib/Support/Signals.cpp:106:18
[build]  #2 0x00007f9af2d92b00 SignalHandler build-llvm/tools/clang/stage2-bins/llvm/lib/Support/Unix/Signals.inc:413:1
[build]  #3 0x00007f9af1ab3320 (/lib/x86_64-linux-gnu/libc.so.6+0x45320)
[build]  #4 0x00007f9af1b190d3 _int_free_create_chunk ./malloc/malloc.c:4722:2
[build]  #5 0x00007f9af1b190d3 _int_free_merge_chunk ./malloc/malloc.c:4700:10
[build]  #6 0x00007f9af1b1bd9e cfree ./malloc/malloc.c:3401:3
[build]  #7 0x0000564e26240965 _M_erase /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:0:4
[build]  #8 0x0000564e26240959 _S_left /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:782:45
[build]  #9 0x0000564e26240959 _M_erase /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:1937:21
[build] #10 0x0000564e26240959 _S_left /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:782:45
[build] #11 0x0000564e26240959 _M_erase /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:1937:21
[build] #12 0x0000564e26240959 _S_left /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:782:45
[build] #13 0x0000564e26240959 _M_erase /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:1937:21
[build] #14 0x0000564e26240959 _S_left /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:782:45
[build] #15 0x0000564e26240959 _M_erase /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:1937:21
[build] #16 0x0000564e26240959 _S_left /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:782:45
[build] #17 0x0000564e26240959 _M_erase /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:1937:21
[build] #18 0x0000564e26240959 _S_left /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:782:45
[build] #19 0x0000564e26240959 _M_erase /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:1937:21
[build] #20 0x0000564e2623efb4 clear /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:0:2
[build] #21 0x0000564e2623efb4 clear /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_map.h:1184:14
[build] #22 0x0000564e2623efb4 matchesAncestorOf build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:696:19
[build] #23 0x0000564e25f1d602 matches build-llvm/tools/clang/stage2-bins/clang/include/clang/ASTMatchers/ASTMatchersInternal.h:1675:5
[build] #24 0x0000564e2629fc85 dynMatches build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchersInternal.cpp:0:33
[build] #25 0x0000564e2629d62c clang::ast_matchers::internal::DynTypedMatcher::matches(clang::DynTypedNode const&, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchersInternal.cpp:304:23
[build] #26 0x0000564e2626960e matchWithFilter build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:1064:11
[build] #27 0x0000564e2626c1eb getKind build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/DeclBase.h:447:51
[build] #28 0x0000564e2626c1eb TraverseDecl build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:743:14
[build] #29 0x0000564e2626c1eb TraverseDecl build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:1471:48
[build] #30 0x0000564e26276ab8 TraverseFunctionProtoTypeLoc build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:1309:1
[build] #31 0x0000564e2629bc35 TraverseFunctionHelper build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:2158:5
[build] #32 0x0000564e2626f636 TraverseCXXMethodDecl build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:2223:1
[build] #33 0x0000564e2626c7ef TraverseDecl build-llvm/tools/clang/stage2-bins/build-llvm/tools/clang/stage2-bins/tools/clang/include/clang/AST/DeclNodes.inc:242:1
[build] #34 0x0000564e2626c7ef TraverseDecl build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:1471:48
[build] #35 0x0000564e262717fb TraverseDeclContextHelper build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:1480:7
[build] #36 0x0000564e262717fb TraverseCXXRecordDecl build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:1996:1
[build] #37 0x0000564e2626c36f TraverseDecl build-llvm/tools/clang/stage2-bins/build-llvm/tools/clang/stage2-bins/tools/clang/include/clang/AST/DeclNodes.inc:414:1
[build] #38 0x0000564e2626c36f TraverseDecl build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:1471:48
[build] #39 0x0000564e262731a1 TraverseClassTemplateDecl build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:1883:1
[build] #40 0x0000564e2626c3cf TraverseDecl build-llvm/tools/clang/stage2-bins/build-llvm/tools/clang/stage2-bins/tools/clang/include/clang/AST/DeclNodes.inc:524:1
[build] #41 0x0000564e2626c3cf TraverseDecl build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:1471:48
[build] #42 0x0000564e2626e96b TraverseDeclContextHelper build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:1480:7
[build] #43 0x0000564e2626e96b TraverseNamespaceDecl build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:1616:1
[build] #44 0x0000564e2626c747 TraverseDecl build-llvm/tools/clang/stage2-bins/build-llvm/tools/clang/stage2-bins/tools/clang/include/clang/AST/DeclNodes.inc:170:1
[build] #45 0x0000564e2626c747 TraverseDecl build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:1471:48
[build] #46 0x0000564e262996eb TraverseDeclContextHelper build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:1480:7
[build] #47 0x0000564e2626cb89 TraverseTranslationUnitDecl build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:1580:1
[build] #48 0x0000564e2626c213 TraverseDecl build-llvm/tools/clang/stage2-bins/build-llvm/tools/clang/stage2-bins/tools/clang/include/clang/AST/DeclNodes.inc:24:1
[build] #49 0x0000564e2626c213 TraverseDecl build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:1471:48
[build] #50 0x0000564e2623e825 onEndOfTranslationUnit build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:446:30
[build] #51 0x0000564e2623e825 clang::ast_matchers::MatchFinder::matchAST(clang::ASTContext&) build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:1700:11
[build] #52 0x00007f9afc039fbc operator++ /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_iterator.h:1111:2
[build] #53 0x00007f9afc039fbc clang::MultiplexConsumer::HandleTranslationUnit(clang::ASTContext&) build-llvm/tools/clang/stage2-bins/clang/lib/Frontend/MultiplexConsumer.cpp:297:23
[build] #54 0x00007f9afa18e3d6 __normal_iterator /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_iterator.h:1077:20
[build] #55 0x00007f9afa18e3d6 begin /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_vector.h:874:16
[build] #56 0x00007f9afa18e3d6 finalize<std::vector<std::unique_ptr<clang::TemplateInstantiationCallback, std::default_delete<clang::TemplateInstantiationCallback> >, std::allocator<std::unique_ptr<clang::TemplateInstantiationCallback, std::default_delete<clang::TemplateInstantiationCallback> > > > > build-llvm/tools/clang/stage2-bins/clang/include/clang/Sema/TemplateInstCallback.h:54:16
[build] #57 0x00007f9afa18e3d6 clang::ParseAST(clang::Sema&, bool, bool) build-llvm/tools/clang/stage2-bins/clang/lib/Parse/ParseAST.cpp:183:3
[build] #58 0x00007f9afbffd62c clang::FrontendAction::Execute() build-llvm/tools/clang/stage2-bins/clang/lib/Frontend/FrontendAction.cpp:1073:10
[build] #59 0x00007f9afbf7a0b4 getPtr build-llvm/tools/clang/stage2-bins/llvm/include/llvm/Support/Error.h:279:12
[build] #60 0x00007f9afbf7a0b4 operator bool build-llvm/tools/clang/stage2-bins/llvm/include/llvm/Support/Error.h:239:16
[build] #61 0x00007f9afbf7a0b4 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) build-llvm/tools/clang/stage2-bins/clang/lib/Frontend/CompilerInstance.cpp:1057:23
[build] #62 0x00007f9afc1f16b9 clang::tooling::FrontendActionFactory::runInvocation(std::shared_ptr<clang::CompilerInvocation>, clang::FileManager*, std::shared_ptr<clang::PCHContainerOperations>, clang::DiagnosticConsumer*) build-llvm/tools/clang/stage2-bins/clang/lib/Tooling/Tooling.cpp:465:33
[build] #63 0x0000564e268bdab3 ~__shared_count /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/shared_ptr_base.h:1070:6
[build] #64 0x0000564e268bdab3 ~__shared_ptr /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/shared_ptr_base.h:1524:31
[build] #65 0x0000564e268bdab3 runInvocation build-llvm/tools/clang/stage2-bins/clang-tools-extra/clang-tidy/ClangTidy.cpp:581:7
[build] #66 0x00007f9afc1f1414 ~__shared_count /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/shared_ptr_base.h:1070:6
[build] #67 0x00007f9afc1f1414 ~__shared_ptr /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/shared_ptr_base.h:1524:31
[build] #68 0x00007f9afc1f1414 clang::tooling::ToolInvocation::runInvocation(char const*, clang::driver::Compilation*, std::shared_ptr<clang::CompilerInvocation>, std::shared_ptr<clang::PCHContainerOperations>) build-llvm/tools/clang/stage2-bins/clang/lib/Tooling/Tooling.cpp:440:3
[build] #69 0x00007f9afc1f017f clang::tooling::ToolInvocation::run() build-llvm/tools/clang/stage2-bins/clang/lib/Tooling/Tooling.cpp:425:10
[build] #70 0x00007f9afc1f31c1 clang::tooling::ClangTool::run(clang::tooling::ToolAction*) build-llvm/tools/clang/stage2-bins/clang/lib/Tooling/Tooling.cpp:623:11
[build] #71 0x0000564e268b9d9b clang::tidy::runClangTidy(clang::tidy::ClangTidyContext&, clang::tooling::CompilationDatabase const&, llvm::ArrayRef<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>>, llvm::IntrusiveRefCntPtr<llvm::vfs::OverlayFileSystem>, bool, bool, llvm::StringRef) build-llvm/tools/clang/stage2-bins/clang-tools-extra/clang-tidy/ClangTidy.cpp:0:8
[build] #72 0x0000564e25c39c65 release build-llvm/tools/clang/stage2-bins/llvm/include/llvm/ADT/IntrusiveRefCntPtr.h:232:9
[build] #73 0x0000564e25c39c65 ~IntrusiveRefCntPtr build-llvm/tools/clang/stage2-bins/llvm/include/llvm/ADT/IntrusiveRefCntPtr.h:196:27
[build] #74 0x0000564e25c39c65 clang::tidy::clangTidyMain(int, char const**) build-llvm/tools/clang/stage2-bins/clang-tools-extra/clang-tidy/tool/ClangTidyMain.cpp:681:7
[build] #75 0x00007f9af1a981ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
[build] #76 0x00007f9af1a9828b call_init ./csu/../csu/libc-start.c:128:20
[build] #77 0x00007f9af1a9828b __libc_start_main ./csu/../csu/libc-start.c:347:5
[build] #78 0x0000564e25c34fe5 _start (/usr/bin/clang-tidy+0x6fafe5)
```

e.g. (2)

```
$ clang-tidy --version
Ubuntu LLVM version 18.1.3
  Optimized build.
$ cat .clang-tidy
Checks: >
  *,
  -abseil-*,
  -altera-*,
  -android-*,
  -clang-analyzer-*,
  -darwin-*,
  -google-objc-*,
  -linuxkernel-*,
  -llvmlibc-*,
  -mpi-*,
  -objc-*,
  -openmp-*,
  -zircon-*,
  -boost-use-to-string,
  -CWE-119,
  -cppcoreguidelines-avoid-magic-numbers,
  -cppcoreguidelines-pro-bounds-pointer-arithmetic,
  -fuchsia-default-arguments-calls,
  -fuchsia-default-arguments-declarations,
  -fuchsia-overloaded-operator,
  -google-readability-braces-around-statements,
  -hicpp-braces-around-statements,
  -hicpp-no-array-decay,
  -llvm-header-guard,
  -llvm-include-order,
  -modernize-use-trailing-return-type,
  -readability-braces-around-statements,
  -readability-identifier-length,
  -readability-implicit-bool-conversion,
  -readability-magic-numbers,
  -bugprone-argument-comment,
  -bugprone-bad-signal-to-kill-thread,
  -bugprone-bool-pointer-implicit-conversion,
  -bugprone-easily-swappable-parameters,
  -bugprone-exception-escape,
  -bugprone-implicit-widening-of-multiplication-result,
  -bugprone-lambda-function-name,
  -bugprone-misplaced-operator-in-strlen-in-alloc,
  -bugprone-misplaced-pointer-arithmetic-in-alloc,
  -bugprone-not-null-terminated-result,
  -bugprone-posix-return,
  -bugprone-signal-handler,
  -bugprone-spuriously-wake-up-functions,
  -bugprone-virtual-near-miss,
  -bugprone-unhandled-exception-at-new,
  -bugprone-unchecked-optional-access,
  -cert-con36-c,
  -cert-con54-cpp,
  -hicpp-uppercase-literal-suffix,
  -readability-uppercase-literal-suffix,
  -cert-dcl16-c,
  -cert-dcl21-cpp,
  -cert-dcl58-cpp,
  -cert-env33-c,
  -cert-pos44-c,
  -cert-pos47-c,
  -cert-sig30-c,
  -cert-msc30-c,
  -cert-msc50-cpp,
  -cert-msc32-c,
  -cert-msc51-cpp,
  -cppcoreguidelines-pro-bounds-constant-array-index,
  -fuchsia-multiple-inheritance,
  -fuchsia-statically-constructed-objects,
  -fuchsia-trailing-return,
  -fuchsia-virtual-inheritance,
  -google-build-explicit-make-pair,
  -google-build-using-namespace,
  -google-default-arguments,
  -google-readability-todo,
  -hicpp-named-parameter,
  -readability-named-parameter,
  -llvm-prefer-isa-or-dyn-cast-in-conditionals,
  -llvm-prefer-register-over-unsigned,
  -llvm-qualified-auto,
  -llvm-twine-local,
  -misc-confusable-identifiers,
  -misc-misleading-bidirectional,
  -misc-misleading-identifier,
  -misc-no-recursion,
  -misc-unused-parameters,
  -modernize-replace-disallow-copy-and-assign-macro,
  -modernize-use-nodiscard,
  -performance-inefficient-string-concatenation,
  -readability-isolate-declaration,
  -readability-qualified-auto,
  -readability-suspicious-call-argument,
  -hicpp-use-nullptr,
  -modernize-use-nullptr,
  -cppcoreguidelines-avoid-do-while,
  -misc-const-correctness,
  -readability-redundant-control-flow,
  -google-readability-casting,
  -cppcoreguidelines-pro-type-const-cast,
  -cppcoreguidelines-pro-type-cstyle-cast,
  -readability-container-size-empty,
  -cppcoreguidelines-init-variables,
  -cppcoreguidelines-non-private-member-variables-in-classes,
  -misc-non-private-member-variables-in-classes,
  -readability-avoid-const-params-in-decls,
  -modernize-return-braced-init-list,
  -readability-simplify-boolean-expr,
  -hicpp-signed-bitwise,
  -readability-function-cognitive-complexity,
  -readability-static-accessed-through-instance,
  -modernize-loop-convert,
  -llvm-else-after-return,
  -misc-include-cleaner,
  -readability-convert-member-functions-to-static,
  -hicpp-avoid-c-arrays,
  -hicpp-explicit-conversions,
  -hicpp-special-member-functions,
  -hicpp-noexcept-move,
  -cppcoreguidelines-noexcept-move-operations,
  -cppcoreguidelines-avoid-c-arrays,
  -bugprone-empty-catch,
  -hicpp-member-init,
  -modernize-use-constraints,
  -performance-enum-size,
  -readability-redundant-inline-specifier,
  -readability-redundant-member-init,
  -readability-avoid-return-with-void-value,
  -readability-avoid-nested-conditional-operator,
  -modernize-use-std-numbers,
  -readability-redundant-casting,
  -bugprone-suspicious-include,
  -hicpp-use-emplace,
  -boost-use-ranges,
  -modernize-use-designated-initializers,
  -readability-math-missing-parentheses,
  -modernize-min-max-use-initializer-list,
  -readability-use-std-min-max,
  -modernize-use-ranges,
  -bugprone-crtp-constructor-accessibility,
  -bugprone-suspicious-stringview-data-usage,
  -misc-use-internal-linkage
FormatStyle: "file"
CheckOptions:
  misc-non-private-member-variables-in-classes.IgnoreClassesWithAllMemberVariablesBeingPublic: "true"
  cppcoreguidelines-special-member-functions.AllowSoleDefaultDtor: "true"
```

e.g. (3)

```
[build] Error running '/usr/bin/clang-tidy': PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
[build] Stack dump:
[build] 0.	Program arguments: /usr/bin/clang-tidy -p /home/dev/snip/out/build/clang-tidy -header-filter=/home/dev/snip -warnings-as-errors=* --extra-arg-before=--driver-mode=g++ /home/dev/snip/.cpp
[build] 1.	<eof> parser at end of file
[build] 2.	ASTMatcher: Matching 'hicpp-invalid-access-moved' against:
[build] 	ImplicitCastExpr : </home/dev/snip/out/build/clang-tidy/vcpkg_installed/x64-linux/include/openssl/core_dispatch.h:591:1 <Spelling=line:52:41>>
[build]  #0 0x00007f977cdd63bf llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) build-llvm/tools/clang/stage2-bins/llvm/lib/Support/Unix/Signals.inc:723:13
[build]  #1 0x00007f977cdd44f9 llvm::sys::RunSignalHandlers() build-llvm/tools/clang/stage2-bins/llvm/lib/Support/Signals.cpp:106:18
[build]  #2 0x00007f977cdd6b00 SignalHandler build-llvm/tools/clang/stage2-bins/llvm/lib/Support/Unix/Signals.inc:413:1
[build]  #3 0x00007f977baf7320 (/lib/x86_64-linux-gnu/libc.so.6+0x45320)
[build]  #4 0x00007f977bb5d0d4 _int_free_create_chunk ./malloc/malloc.c:4722:2
[build]  #5 0x00007f977bb5d0d4 _int_free_merge_chunk ./malloc/malloc.c:4700:10
[build]  #6 0x00007f977bb5fd9e cfree ./malloc/malloc.c:3401:3
[build]  #7 0x000056495dde0965 _M_erase /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:0:4
[build]  #8 0x000056495dde0959 _S_left /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:782:45
[build]  #9 0x000056495dde0959 _M_erase /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:1937:21
[build] #10 0x000056495dde0959 _S_left /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:782:45
[build] #11 0x000056495dde0959 _M_erase /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:1937:21
[build] #12 0x000056495dde0959 _S_left /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:782:45
[build] #13 0x000056495dde0959 _M_erase /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:1937:21
[build] #14 0x000056495dde0959 _S_left /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:782:45
[build] #15 0x000056495dde0959 _M_erase /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:1937:21
[build] #16 0x000056495dde0959 _S_left /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:782:45
[build] #17 0x000056495dde0959 _M_erase /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:1937:21
[build] #18 0x000056495dde0959 _S_left /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:782:45
[build] #19 0x000056495dde0959 _M_erase /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:1937:21
[build] #20 0x000056495dddee4c clear /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:0:2
[build] #21 0x000056495dddee4c clear /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_map.h:1184:14
[build] #22 0x000056495dddee4c matchesChildOf build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:675:19
[build] #23 0x000056495d94c1d2 matches build-llvm/tools/clang/stage2-bins/clang/include/clang/ASTMatchers/ASTMatchersInternal.h:1623:5
[build] #24 0x000056495de3d76e clang::ast_matchers::internal::DynTypedMatcher::matchesNoKindCheck(clang::DynTypedNode const&, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchersInternal.cpp:332:23
[build] #25 0x000056495de4066a __find_if<const clang::ast_matchers::internal::DynTypedMatcher *, __gnu_cxx::__ops::_Iter_negate<(lambda at clang/lib/ASTMatchers/ASTMatchersInternal.cpp:394:38)> > /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_algobase.h:2092:8
[build] #26 0x000056495de4066a __find_if_not<const clang::ast_matchers::internal::DynTypedMatcher *, __gnu_cxx::__ops::_Iter_pred<(lambda at clang/lib/ASTMatchers/ASTMatchersInternal.cpp:394:38)> > /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_algo.h:112:14
[build] #27 0x000056495de4066a find_if_not<const clang::ast_matchers::internal::DynTypedMatcher *, (lambda at clang/lib/ASTMatchers/ASTMatchersInternal.cpp:394:38)> /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_algo.h:519:14
[build] #28 0x000056495de4066a all_of<const clang::ast_matchers::internal::DynTypedMatcher *, (lambda at clang/lib/ASTMatchers/ASTMatchersInternal.cpp:394:38)> /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_algo.h:459:24
[build] #29 0x000056495de4066a all_of<llvm::ArrayRef<clang::ast_matchers::internal::DynTypedMatcher> &, (lambda at clang/lib/ASTMatchers/ASTMatchersInternal.cpp:394:38)> build-llvm/tools/clang/stage2-bins/llvm/include/llvm/ADT/STLExtras.h:1732:10
[build] #30 0x000056495de4066a allOfVariadicOperator build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchersInternal.cpp:394:10
[build] #31 0x000056495de4066a dynMatches build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchersInternal.cpp:123:12
[build] #32 0x000056495de3fc85 dynMatches build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchersInternal.cpp:0:33
[build] #33 0x000056495de3d62c clang::ast_matchers::internal::DynTypedMatcher::matches(clang::DynTypedNode const&, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchersInternal.cpp:304:23
[build] #34 0x000056495d7f4641 matches build-llvm/tools/clang/stage2-bins/clang/include/clang/ASTMatchers/ASTMatchersInternal.h:1514:5
[build] #35 0x000056495de3d62c clang::ast_matchers::internal::DynTypedMatcher::matches(clang::DynTypedNode const&, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchersInternal.cpp:304:23
[build] #36 0x000056495de0960e matchWithFilter build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:1064:11
[build] #37 0x000056495de212f1 TraverseStmt build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:1483:48
[build] #38 0x000056495de212f1 TraverseMemberExpr build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:2437:1
[build] #39 0x000056495de1c172 TraverseStmt build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:678:7
[build] #40 0x000056495de3bfa2 ~ASTNodeNotSpelledInSourceScope build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:971:47
[build] #41 0x000056495de3bfa2 TraverseStmt build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:1484:1
[build] #42 0x000056495de3bfa2 TraverseFunctionHelper build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:2198:5
[build] #43 0x000056495de0f586 TraverseFunctionDecl build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:2209:1
[build] #44 0x000056495de0c66f TraverseDecl build-llvm/tools/clang/stage2-bins/build-llvm/tools/clang/stage2-bins/tools/clang/include/clang/AST/DeclNodes.inc:238:1
[build] #45 0x000056495de0c66f TraverseDecl build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:1471:48
[build] #46 0x000056495de0cd7b TraverseDeclContextHelper build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:1480:7
[build] #47 0x000056495de0cd7b TraverseLinkageSpecDecl build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:1568:1
[build] #48 0x000056495de0c5df TraverseDecl build-llvm/tools/clang/stage2-bins/build-llvm/tools/clang/stage2-bins/tools/clang/include/clang/AST/DeclNodes.inc:36:1
[build] #49 0x000056495de0c5df TraverseDecl build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:1471:48
[build] #50 0x000056495de396eb TraverseDeclContextHelper build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:1480:7
[build] #51 0x000056495de0cb89 TraverseTranslationUnitDecl build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:1580:1
[build] #52 0x000056495de0c213 TraverseDecl build-llvm/tools/clang/stage2-bins/build-llvm/tools/clang/stage2-bins/tools/clang/include/clang/AST/DeclNodes.inc:24:1
[build] #53 0x000056495de0c213 TraverseDecl build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:1471:48
[build] #54 0x000056495ddde825 onEndOfTranslationUnit build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:446:30
[build] #55 0x000056495ddde825 clang::ast_matchers::MatchFinder::matchAST(clang::ASTContext&) build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:1700:11
[build] #56 0x00007f978607dfbc operator++ /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_iterator.h:1111:2
[build] #57 0x00007f978607dfbc clang::MultiplexConsumer::HandleTranslationUnit(clang::ASTContext&) build-llvm/tools/clang/stage2-bins/clang/lib/Frontend/MultiplexConsumer.cpp:297:23
[build] #58 0x00007f97841d23d6 __normal_iterator /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_iterator.h:1077:20
[build] #59 0x00007f97841d23d6 begin /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_vector.h:874:16
[build] #60 0x00007f97841d23d6 finalize<std::vector<std::unique_ptr<clang::TemplateInstantiationCallback, std::default_delete<clang::TemplateInstantiationCallback> >, std::allocator<std::unique_ptr<clang::TemplateInstantiationCallback, std::default_delete<clang::TemplateInstantiationCallback> > > > > build-llvm/tools/clang/stage2-bins/clang/include/clang/Sema/TemplateInstCallback.h:54:16
[build] #61 0x00007f97841d23d6 clang::ParseAST(clang::Sema&, bool, bool) build-llvm/tools/clang/stage2-bins/clang/lib/Parse/ParseAST.cpp:183:3
[build] #62 0x00007f978604162c clang::FrontendAction::Execute() build-llvm/tools/clang/stage2-bins/clang/lib/Frontend/FrontendAction.cpp:1073:10
[build] #63 0x00007f9785fbe0b4 getPtr build-llvm/tools/clang/stage2-bins/llvm/include/llvm/Support/Error.h:279:12
[build] #64 0x00007f9785fbe0b4 operator bool build-llvm/tools/clang/stage2-bins/llvm/include/llvm/Support/Error.h:239:16
[build] #65 0x00007f9785fbe0b4 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) build-llvm/tools/clang/stage2-bins/clang/lib/Frontend/CompilerInstance.cpp:1057:23
[build] #66 0x00007f97862356b9 clang::tooling::FrontendActionFactory::runInvocation(std::shared_ptr<clang::CompilerInvocation>, clang::FileManager*, std::shared_ptr<clang::PCHContainerOperations>, clang::DiagnosticConsumer*) build-llvm/tools/clang/stage2-bins/clang/lib/Tooling/Tooling.cpp:465:33
[build] #67 0x000056495e45dab3 ~__shared_count /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/shared_ptr_base.h:1070:6
[build] #68 0x000056495e45dab3 ~__shared_ptr /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/shared_ptr_base.h:1524:31
[build] #69 0x000056495e45dab3 runInvocation build-llvm/tools/clang/stage2-bins/clang-tools-extra/clang-tidy/ClangTidy.cpp:581:7
[build] #70 0x00007f9786235414 ~__shared_count /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/shared_ptr_base.h:1070:6
[build] #71 0x00007f9786235414 ~__shared_ptr /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/shared_ptr_base.h:1524:31
[build] #72 0x00007f9786235414 clang::tooling::ToolInvocation::runInvocation(char const*, clang::driver::Compilation*, std::shared_ptr<clang::CompilerInvocation>, std::shared_ptr<clang::PCHContainerOperations>) build-llvm/tools/clang/stage2-bins/clang/lib/Tooling/Tooling.cpp:440:3
[build] #73 0x00007f978623417f clang::tooling::ToolInvocation::run() build-llvm/tools/clang/stage2-bins/clang/lib/Tooling/Tooling.cpp:425:10
[build] #74 0x00007f97862371c1 clang::tooling::ClangTool::run(clang::tooling::ToolAction*) build-llvm/tools/clang/stage2-bins/clang/lib/Tooling/Tooling.cpp:623:11
[build] #75 0x000056495e459d9b clang::tidy::runClangTidy(clang::tidy::ClangTidyContext&, clang::tooling::CompilationDatabase const&, llvm::ArrayRef<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>>, llvm::IntrusiveRefCntPtr<llvm::vfs::OverlayFileSystem>, bool, bool, llvm::StringRef) build-llvm/tools/clang/stage2-bins/clang-tools-extra/clang-tidy/ClangTidy.cpp:0:8
[build] #76 0x000056495d7d9c65 release build-llvm/tools/clang/stage2-bins/llvm/include/llvm/ADT/IntrusiveRefCntPtr.h:232:9
[build] #77 0x000056495d7d9c65 ~IntrusiveRefCntPtr build-llvm/tools/clang/stage2-bins/llvm/include/llvm/ADT/IntrusiveRefCntPtr.h:196:27
[build] #78 0x000056495d7d9c65 clang::tidy::clangTidyMain(int, char const**) build-llvm/tools/clang/stage2-bins/clang-tools-extra/clang-tidy/tool/ClangTidyMain.cpp:681:7
[build] #79 0x00007f977badc1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
[build] #80 0x00007f977badc28b call_init ./csu/../csu/libc-start.c:128:20
[build] #81 0x00007f977badc28b __libc_start_main ./csu/../csu/libc-start.c:347:5
[build] #82 0x000056495d7d4fe5 _start (/usr/bin/clang-tidy+0x6fafe5)
[build] Segmentation fault
```
e.g. (4)

```
[build] 0.	Program arguments: /usr/bin/clang-tidy -p /home/dev/snip/out/build/clang-tidy -header-filter=/home/dev/snip -warnings-as-errors=* --extra-arg-before=--driver-mode=g++ /home/dev/snip.cpp
[build] 1.	<eof> parser at end of file
[build] 2.	ASTMatcher: Matching 'modernize-make-unique' against:
[build] 	CXXMemberCallExpr : </usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/basic_string.h:1354:20, col:31>
[build]  #0 0x00007f881db1c3bf llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) build-llvm/tools/clang/stage2-bins/llvm/lib/Support/Unix/Signals.inc:723:13
[build]  #1 0x00007f881db1a4f9 llvm::sys::RunSignalHandlers() build-llvm/tools/clang/stage2-bins/llvm/lib/Support/Signals.cpp:106:18
[build]  #2 0x00007f881db1cb00 SignalHandler build-llvm/tools/clang/stage2-bins/llvm/lib/Support/Unix/Signals.inc:413:1
[build]  #3 0x00007f881c83d320 (/lib/x86_64-linux-gnu/libc.so.6+0x45320)
[build]  #4 0x00007f881c8a3027 _int_free_create_chunk ./malloc/malloc.c:4720:10
[build]  #5 0x00007f881c8a3027 _int_free_merge_chunk ./malloc/malloc.c:4700:10
[build]  #6 0x00007f881c8a5d9e cfree ./malloc/malloc.c:3401:3
[build]  #7 0x000055b4c3786965 _M_erase /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:0:4
[build]  #8 0x000055b4c3786959 _S_left /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:782:45
[build]  #9 0x000055b4c3786959 _M_erase /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:1937:21
[build] #10 0x000055b4c3786959 _S_left /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:782:45
[build] #11 0x000055b4c3786959 _M_erase /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:1937:21
[build] #12 0x000055b4c3786959 _S_left /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:782:45
[build] #13 0x000055b4c3786959 _M_erase /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:1937:21
[build] #14 0x000055b4c3786959 _S_left /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:782:45
[build] #15 0x000055b4c3786959 _M_erase /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:1937:21
[build] #16 0x000055b4c3786959 _S_left /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:782:45
[build] #17 0x000055b4c3786959 _M_erase /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:1937:21
[build] #18 0x000055b4c3784fb4 clear /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:0:2
[build] #19 0x000055b4c3784fb4 clear /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_map.h:1184:14
[build] #20 0x000055b4c3784fb4 matchesAncestorOf build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:696:19
[build] #21 0x000055b4c31994bf matches build-llvm/tools/clang/stage2-bins/clang/include/clang/ASTMatchers/ASTMatchersInternal.h:1697:5
[build] #22 0x000055b4c37e362c clang::ast_matchers::internal::DynTypedMatcher::matches(clang::DynTypedNode const&, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchersInternal.cpp:304:23
[build] #23 0x000055b4c37e7045 notUnaryOperator build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchersInternal.cpp:384:28
[build] #24 0x000055b4c37e7045 dynMatches build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchersInternal.cpp:123:12
[build] #25 0x000055b4c37e376e clang::ast_matchers::internal::DynTypedMatcher::matchesNoKindCheck(clang::DynTypedNode const&, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchersInternal.cpp:332:23
[build] #26 0x000055b4c37e6585 __find_if<const clang::ast_matchers::internal::DynTypedMatcher *, __gnu_cxx::__ops::_Iter_negate<(lambda at clang/lib/ASTMatchers/ASTMatchersInternal.cpp:394:38)> > /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_algobase.h:2072:8
[build] #27 0x000055b4c37e6585 __find_if_not<const clang::ast_matchers::internal::DynTypedMatcher *, __gnu_cxx::__ops::_Iter_pred<(lambda at clang/lib/ASTMatchers/ASTMatchersInternal.cpp:394:38)> > /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_algo.h:112:14
[build] #28 0x000055b4c37e6585 find_if_not<const clang::ast_matchers::internal::DynTypedMatcher *, (lambda at clang/lib/ASTMatchers/ASTMatchersInternal.cpp:394:38)> /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_algo.h:519:14
[build] #29 0x000055b4c37e6585 all_of<const clang::ast_matchers::internal::DynTypedMatcher *, (lambda at clang/lib/ASTMatchers/ASTMatchersInternal.cpp:394:38)> /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_algo.h:459:24
[build] #30 0x000055b4c37e6585 all_of<llvm::ArrayRef<clang::ast_matchers::internal::DynTypedMatcher> &, (lambda at clang/lib/ASTMatchers/ASTMatchersInternal.cpp:394:38)> build-llvm/tools/clang/stage2-bins/llvm/include/llvm/ADT/STLExtras.h:1732:10
[build] #31 0x000055b4c37e6585 allOfVariadicOperator build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchersInternal.cpp:394:10
[build] #32 0x000055b4c37e6585 dynMatches build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchersInternal.cpp:123:12
[build] #33 0x000055b4c37e5c85 dynMatches build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchersInternal.cpp:0:33
[build] #34 0x000055b4c37e362c clang::ast_matchers::internal::DynTypedMatcher::matches(clang::DynTypedNode const&, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchersInternal.cpp:304:23
[build] #35 0x000055b4c319a641 matches build-llvm/tools/clang/stage2-bins/clang/include/clang/ASTMatchers/ASTMatchersInternal.h:1514:5
[build] #36 0x000055b4c37e362c clang::ast_matchers::internal::DynTypedMatcher::matches(clang::DynTypedNode const&, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchersInternal.cpp:304:23
[build] #37 0x000055b4c37af60e matchWithFilter build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:1064:11
[build] #38 0x000055b4c37ce4a1 TraverseStmt build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:1483:48
[build] #39 0x000055b4c37ce4a1 TraverseBinaryOperator build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:2852:1
[build] #40 0x000055b4c37c1b0c TraverseStmt build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:678:7
[build] #41 0x000055b4c37e1fa2 ~ASTNodeNotSpelledInSourceScope build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:971:47
[build] #42 0x000055b4c37e1fa2 TraverseStmt build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:1484:1
[build] #43 0x000055b4c37e1fa2 TraverseFunctionHelper build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:2198:5
[build] #44 0x000055b4c37b5636 TraverseCXXMethodDecl build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:2223:1
[build] #45 0x000055b4c37b27ef TraverseDecl build-llvm/tools/clang/stage2-bins/build-llvm/tools/clang/stage2-bins/tools/clang/include/clang/AST/DeclNodes.inc:242:1
[build] #46 0x000055b4c37b27ef TraverseDecl build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:1471:48
[build] #47 0x000055b4c37b799b TraverseDeclContextHelper build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:1480:7
[build] #48 0x000055b4c37b799b TraverseClassTemplateSpecializationDecl build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:2026:1
[build] #49 0x000055b4c37b287f TraverseDecl build-llvm/tools/clang/stage2-bins/build-llvm/tools/clang/stage2-bins/tools/clang/include/clang/AST/DeclNodes.inc:418:1
[build] #50 0x000055b4c37b287f TraverseDecl build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:1471:48
[build] #51 0x000055b4c37b9252 TraverseTemplateInstantiations build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:1791:9
[build] #52 0x000055b4c37b9252 TraverseTemplateInstantiations build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:759:50
[build] #53 0x000055b4c37b9252 TraverseClassTemplateDecl build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:1883:1
[build] #54 0x000055b4c37b23cf TraverseDecl build-llvm/tools/clang/stage2-bins/build-llvm/tools/clang/stage2-bins/tools/clang/include/clang/AST/DeclNodes.inc:524:1
[build] #55 0x000055b4c37b23cf TraverseDecl build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:1471:48
[build] #56 0x000055b4c37b496b TraverseDeclContextHelper build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:1480:7
[build] #57 0x000055b4c37b496b TraverseNamespaceDecl build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:1616:1
[build] #58 0x000055b4c37b2747 TraverseDecl build-llvm/tools/clang/stage2-bins/build-llvm/tools/clang/stage2-bins/tools/clang/include/clang/AST/DeclNodes.inc:170:1
[build] #59 0x000055b4c37b2747 TraverseDecl build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:1471:48
[build] #60 0x000055b4c37b496b TraverseDeclContextHelper build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:1480:7
[build] #61 0x000055b4c37b496b TraverseNamespaceDecl build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:1616:1
[build] #62 0x000055b4c37b2747 TraverseDecl build-llvm/tools/clang/stage2-bins/build-llvm/tools/clang/stage2-bins/tools/clang/include/clang/AST/DeclNodes.inc:170:1
[build] #63 0x000055b4c37b2747 TraverseDecl build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:1471:48
[build] #64 0x000055b4c37df6eb TraverseDeclContextHelper build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:1480:7
[build] #65 0x000055b4c37b2b89 TraverseTranslationUnitDecl build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:1580:1
[build] #66 0x000055b4c37b2213 TraverseDecl build-llvm/tools/clang/stage2-bins/build-llvm/tools/clang/stage2-bins/tools/clang/include/clang/AST/DeclNodes.inc:24:1
[build] #67 0x000055b4c37b2213 TraverseDecl build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:1471:48
[build] #68 0x000055b4c3784825 onEndOfTranslationUnit build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:446:30
[build] #69 0x000055b4c3784825 clang::ast_matchers::MatchFinder::matchAST(clang::ASTContext&) build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:1700:11
[build] #70 0x00007f8826dc3fbc operator++ /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_iterator.h:1111:2
[build] #71 0x00007f8826dc3fbc clang::MultiplexConsumer::HandleTranslationUnit(clang::ASTContext&) build-llvm/tools/clang/stage2-bins/clang/lib/Frontend/MultiplexConsumer.cpp:297:23
[build] #72 0x00007f8824f183d6 __normal_iterator /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_iterator.h:1077:20
[build] #73 0x00007f8824f183d6 begin /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_vector.h:874:16
[build] #74 0x00007f8824f183d6 finalize<std::vector<std::unique_ptr<clang::TemplateInstantiationCallback, std::default_delete<clang::TemplateInstantiationCallback> >, std::allocator<std::unique_ptr<clang::TemplateInstantiationCallback, std::default_delete<clang::TemplateInstantiationCallback> > > > > build-llvm/tools/clang/stage2-bins/clang/include/clang/Sema/TemplateInstCallback.h:54:16
[build] #75 0x00007f8824f183d6 clang::ParseAST(clang::Sema&, bool, bool) build-llvm/tools/clang/stage2-bins/clang/lib/Parse/ParseAST.cpp:183:3
[build] #76 0x00007f8826d8762c clang::FrontendAction::Execute() build-llvm/tools/clang/stage2-bins/clang/lib/Frontend/FrontendAction.cpp:1073:10
[build] #77 0x00007f8826d040b4 getPtr build-llvm/tools/clang/stage2-bins/llvm/include/llvm/Support/Error.h:279:12
[build] #78 0x00007f8826d040b4 operator bool build-llvm/tools/clang/stage2-bins/llvm/include/llvm/Support/Error.h:239:16
[build] #79 0x00007f8826d040b4 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) build-llvm/tools/clang/stage2-bins/clang/lib/Frontend/CompilerInstance.cpp:1057:23
[build] #80 0x00007f8826f7b6b9 clang::tooling::FrontendActionFactory::runInvocation(std::shared_ptr<clang::CompilerInvocation>, clang::FileManager*, std::shared_ptr<clang::PCHContainerOperations>, clang::DiagnosticConsumer*) build-llvm/tools/clang/stage2-bins/clang/lib/Tooling/Tooling.cpp:465:33
[build] #81 0x000055b4c3e03ab3 ~__shared_count /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/shared_ptr_base.h:1070:6
[build] #82 0x000055b4c3e03ab3 ~__shared_ptr /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/shared_ptr_base.h:1524:31
[build] #83 0x000055b4c3e03ab3 runInvocation build-llvm/tools/clang/stage2-bins/clang-tools-extra/clang-tidy/ClangTidy.cpp:581:7
[build] #84 0x00007f8826f7b414 ~__shared_count /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/shared_ptr_base.h:1070:6
[build] #85 0x00007f8826f7b414 ~__shared_ptr /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/shared_ptr_base.h:1524:31
[build] #86 0x00007f8826f7b414 clang::tooling::ToolInvocation::runInvocation(char const*, clang::driver::Compilation*, std::shared_ptr<clang::CompilerInvocation>, std::shared_ptr<clang::PCHContainerOperations>) build-llvm/tools/clang/stage2-bins/clang/lib/Tooling/Tooling.cpp:440:3
[build] #87 0x00007f8826f7a17f clang::tooling::ToolInvocation::run() build-llvm/tools/clang/stage2-bins/clang/lib/Tooling/Tooling.cpp:425:10
[build] #88 0x00007f8826f7d1c1 clang::tooling::ClangTool::run(clang::tooling::ToolAction*) build-llvm/tools/clang/stage2-bins/clang/lib/Tooling/Tooling.cpp:623:11
[build] #89 0x000055b4c3dffd9b clang::tidy::runClangTidy(clang::tidy::ClangTidyContext&, clang::tooling::CompilationDatabase const&, llvm::ArrayRef<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>>, llvm::IntrusiveRefCntPtr<llvm::vfs::OverlayFileSystem>, bool, bool, llvm::StringRef) build-llvm/tools/clang/stage2-bins/clang-tools-extra/clang-tidy/ClangTidy.cpp:0:8
[build] #90 0x000055b4c317fc65 release build-llvm/tools/clang/stage2-bins/llvm/include/llvm/ADT/IntrusiveRefCntPtr.h:232:9
[build] #91 0x000055b4c317fc65 ~IntrusiveRefCntPtr build-llvm/tools/clang/stage2-bins/llvm/include/llvm/ADT/IntrusiveRefCntPtr.h:196:27
[build] #92 0x000055b4c317fc65 clang::tidy::clangTidyMain(int, char const**) build-llvm/tools/clang/stage2-bins/clang-tools-extra/clang-tidy/tool/ClangTidyMain.cpp:681:7
[build] #93 0x00007f881c8221ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
[build] #94 0x00007f881c82228b call_init ./csu/../csu/libc-start.c:128:20
[build] #95 0x00007f881c82228b __libc_start_main ./csu/../csu/libc-start.c:347:5
[build] #96 0x000055b4c317afe5 _start (/usr/bin/clang-tidy+0x6fafe5)
[build] Segmentation fault
```

Command line to invoke
```
[build] /usr/bin/cmake -E __run_co_compile --launcher="/usr/bin/cmake;-E;env;CCACHE_SLOPPINESS=pch_defines,time_macros,include_file_mtime,include_file_ctime;/usr/local/bin/ccache" --tidy="/usr/bin/clang-tidy;-p;/home/dev/snip/out/build/clang-tidy;-header-filter=/home/dev/snip;-warnings-as-errors=*;--extra-arg-before=--driver-mode=g++" --source=/home/dev/snip/src/lib/snip.cpp -- /usr/bin/clang++ -DBOOST_CONTAINER_NO_LIB -DBOOST_CONTAINER_STATIC_LINK -DBOOST_CONTEXT_EXPORT="" -DBOOST_CONTEXT_NO_LIB="" -DBOOST_CONTEXT_STATIC_LINK="" -DBOOST_COROUTINE_NO_LIB -DBOOST_COROUTINE_STATIC_LINK -DBOOST_DATE_TIME_NO_LIB -DBOOST_DATE_TIME_STATIC_LINK -DSPDLOG_COMPILED_LIB -DSPDLOG_FMT_EXTERNAL -I/home/dev/snip/src/lib -I/home/dev/snip/src -I/home/dev/snip/out/build/clang-tidy/src/snip/pb -isystem /home/dev/snip/out/build/clang-tidy/vcpkg_installed/x64-linux/include -O3 -DNDEBUG -std=c++20 -fPIC -Wall -Wextra -Xclang -fno-pch-timestamp -Werror -MD -MT src/lib/snip.cpp.o -MF src/lib/snip.o.d -o src/snip.cpp.o -c /home/dev/snip.cpp
```

Sorry, this is commercial code, unable to provide the whole thing, but this is compiling against boost redis. Here are the main 3rd party includes

```c++
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/consign.hpp>
#include <boost/asio/deferred.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/redirect_error.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/asio/use_awaitable.hpp>
#include <boost/asio/use_future.hpp>
#include <boost/redis/config.hpp>
#include <boost/redis/connection.hpp>
#include <chrono>
#include <nlohmann/json.hpp>
#include <memory>
#include <numeric>
#include <ranges>
#include <thread>
#include <tl/to.hpp>
````


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (tonygould)

<details>
Sometimes succeeds, sometimes fails with different backtraces

e.g. (1)

```
[build] Error running '/usr/bin/clang-tidy': PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
[build] Stack dump:
[build] 0.	Program arguments: /usr/bin/clang-tidy -p /home/dev/snip/out/build/clang-tidy -header-filter=/home/dev/snip -warnings-as-errors=* --extra-arg-before=--driver-mode=g++ /home/dev/snip/snip.cpp
[build] 1.	&lt;eof&gt; parser at end of file
[build] 2.	ASTMatcher: Matching 'cppcoreguidelines-macro-to-enum' against:
[build] 	ParmVarDecl  : &lt;/usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_deque.h:2232:53&gt;
[build]  #<!-- -->0 0x00007f9af2d923bf llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) build-llvm/tools/clang/stage2-bins/llvm/lib/Support/Unix/Signals.inc:723:13
[build]  #<!-- -->1 0x00007f9af2d904f9 llvm::sys::RunSignalHandlers() build-llvm/tools/clang/stage2-bins/llvm/lib/Support/Signals.cpp:106:18
[build]  #<!-- -->2 0x00007f9af2d92b00 SignalHandler build-llvm/tools/clang/stage2-bins/llvm/lib/Support/Unix/Signals.inc:413:1
[build]  #<!-- -->3 0x00007f9af1ab3320 (/lib/x86_64-linux-gnu/libc.so.6+0x45320)
[build]  #<!-- -->4 0x00007f9af1b190d3 _int_free_create_chunk ./malloc/malloc.c:4722:2
[build]  #<!-- -->5 0x00007f9af1b190d3 _int_free_merge_chunk ./malloc/malloc.c:4700:10
[build]  #<!-- -->6 0x00007f9af1b1bd9e cfree ./malloc/malloc.c:3401:3
[build]  #<!-- -->7 0x0000564e26240965 _M_erase /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:0:4
[build]  #<!-- -->8 0x0000564e26240959 _S_left /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:782:45
[build]  #<!-- -->9 0x0000564e26240959 _M_erase /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:1937:21
[build] #<!-- -->10 0x0000564e26240959 _S_left /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:782:45
[build] #<!-- -->11 0x0000564e26240959 _M_erase /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:1937:21
[build] #<!-- -->12 0x0000564e26240959 _S_left /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:782:45
[build] #<!-- -->13 0x0000564e26240959 _M_erase /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:1937:21
[build] #<!-- -->14 0x0000564e26240959 _S_left /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:782:45
[build] #<!-- -->15 0x0000564e26240959 _M_erase /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:1937:21
[build] #<!-- -->16 0x0000564e26240959 _S_left /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:782:45
[build] #<!-- -->17 0x0000564e26240959 _M_erase /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:1937:21
[build] #<!-- -->18 0x0000564e26240959 _S_left /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:782:45
[build] #<!-- -->19 0x0000564e26240959 _M_erase /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:1937:21
[build] #<!-- -->20 0x0000564e2623efb4 clear /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:0:2
[build] #<!-- -->21 0x0000564e2623efb4 clear /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_map.h:1184:14
[build] #<!-- -->22 0x0000564e2623efb4 matchesAncestorOf build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:696:19
[build] #<!-- -->23 0x0000564e25f1d602 matches build-llvm/tools/clang/stage2-bins/clang/include/clang/ASTMatchers/ASTMatchersInternal.h:1675:5
[build] #<!-- -->24 0x0000564e2629fc85 dynMatches build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchersInternal.cpp:0:33
[build] #<!-- -->25 0x0000564e2629d62c clang::ast_matchers::internal::DynTypedMatcher::matches(clang::DynTypedNode const&amp;, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchersInternal.cpp:304:23
[build] #<!-- -->26 0x0000564e2626960e matchWithFilter build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:1064:11
[build] #<!-- -->27 0x0000564e2626c1eb getKind build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/DeclBase.h:447:51
[build] #<!-- -->28 0x0000564e2626c1eb TraverseDecl build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:743:14
[build] #<!-- -->29 0x0000564e2626c1eb TraverseDecl build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:1471:48
[build] #<!-- -->30 0x0000564e26276ab8 TraverseFunctionProtoTypeLoc build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:1309:1
[build] #<!-- -->31 0x0000564e2629bc35 TraverseFunctionHelper build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:2158:5
[build] #<!-- -->32 0x0000564e2626f636 TraverseCXXMethodDecl build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:2223:1
[build] #<!-- -->33 0x0000564e2626c7ef TraverseDecl build-llvm/tools/clang/stage2-bins/build-llvm/tools/clang/stage2-bins/tools/clang/include/clang/AST/DeclNodes.inc:242:1
[build] #<!-- -->34 0x0000564e2626c7ef TraverseDecl build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:1471:48
[build] #<!-- -->35 0x0000564e262717fb TraverseDeclContextHelper build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:1480:7
[build] #<!-- -->36 0x0000564e262717fb TraverseCXXRecordDecl build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:1996:1
[build] #<!-- -->37 0x0000564e2626c36f TraverseDecl build-llvm/tools/clang/stage2-bins/build-llvm/tools/clang/stage2-bins/tools/clang/include/clang/AST/DeclNodes.inc:414:1
[build] #<!-- -->38 0x0000564e2626c36f TraverseDecl build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:1471:48
[build] #<!-- -->39 0x0000564e262731a1 TraverseClassTemplateDecl build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:1883:1
[build] #<!-- -->40 0x0000564e2626c3cf TraverseDecl build-llvm/tools/clang/stage2-bins/build-llvm/tools/clang/stage2-bins/tools/clang/include/clang/AST/DeclNodes.inc:524:1
[build] #<!-- -->41 0x0000564e2626c3cf TraverseDecl build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:1471:48
[build] #<!-- -->42 0x0000564e2626e96b TraverseDeclContextHelper build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:1480:7
[build] #<!-- -->43 0x0000564e2626e96b TraverseNamespaceDecl build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:1616:1
[build] #<!-- -->44 0x0000564e2626c747 TraverseDecl build-llvm/tools/clang/stage2-bins/build-llvm/tools/clang/stage2-bins/tools/clang/include/clang/AST/DeclNodes.inc:170:1
[build] #<!-- -->45 0x0000564e2626c747 TraverseDecl build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:1471:48
[build] #<!-- -->46 0x0000564e262996eb TraverseDeclContextHelper build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:1480:7
[build] #<!-- -->47 0x0000564e2626cb89 TraverseTranslationUnitDecl build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:1580:1
[build] #<!-- -->48 0x0000564e2626c213 TraverseDecl build-llvm/tools/clang/stage2-bins/build-llvm/tools/clang/stage2-bins/tools/clang/include/clang/AST/DeclNodes.inc:24:1
[build] #<!-- -->49 0x0000564e2626c213 TraverseDecl build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:1471:48
[build] #<!-- -->50 0x0000564e2623e825 onEndOfTranslationUnit build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:446:30
[build] #<!-- -->51 0x0000564e2623e825 clang::ast_matchers::MatchFinder::matchAST(clang::ASTContext&amp;) build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:1700:11
[build] #<!-- -->52 0x00007f9afc039fbc operator++ /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_iterator.h:1111:2
[build] #<!-- -->53 0x00007f9afc039fbc clang::MultiplexConsumer::HandleTranslationUnit(clang::ASTContext&amp;) build-llvm/tools/clang/stage2-bins/clang/lib/Frontend/MultiplexConsumer.cpp:297:23
[build] #<!-- -->54 0x00007f9afa18e3d6 __normal_iterator /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_iterator.h:1077:20
[build] #<!-- -->55 0x00007f9afa18e3d6 begin /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_vector.h:874:16
[build] #<!-- -->56 0x00007f9afa18e3d6 finalize&lt;std::vector&lt;std::unique_ptr&lt;clang::TemplateInstantiationCallback, std::default_delete&lt;clang::TemplateInstantiationCallback&gt; &gt;, std::allocator&lt;std::unique_ptr&lt;clang::TemplateInstantiationCallback, std::default_delete&lt;clang::TemplateInstantiationCallback&gt; &gt; &gt; &gt; &gt; build-llvm/tools/clang/stage2-bins/clang/include/clang/Sema/TemplateInstCallback.h:54:16
[build] #<!-- -->57 0x00007f9afa18e3d6 clang::ParseAST(clang::Sema&amp;, bool, bool) build-llvm/tools/clang/stage2-bins/clang/lib/Parse/ParseAST.cpp:183:3
[build] #<!-- -->58 0x00007f9afbffd62c clang::FrontendAction::Execute() build-llvm/tools/clang/stage2-bins/clang/lib/Frontend/FrontendAction.cpp:1073:10
[build] #<!-- -->59 0x00007f9afbf7a0b4 getPtr build-llvm/tools/clang/stage2-bins/llvm/include/llvm/Support/Error.h:279:12
[build] #<!-- -->60 0x00007f9afbf7a0b4 operator bool build-llvm/tools/clang/stage2-bins/llvm/include/llvm/Support/Error.h:239:16
[build] #<!-- -->61 0x00007f9afbf7a0b4 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) build-llvm/tools/clang/stage2-bins/clang/lib/Frontend/CompilerInstance.cpp:1057:23
[build] #<!-- -->62 0x00007f9afc1f16b9 clang::tooling::FrontendActionFactory::runInvocation(std::shared_ptr&lt;clang::CompilerInvocation&gt;, clang::FileManager*, std::shared_ptr&lt;clang::PCHContainerOperations&gt;, clang::DiagnosticConsumer*) build-llvm/tools/clang/stage2-bins/clang/lib/Tooling/Tooling.cpp:465:33
[build] #<!-- -->63 0x0000564e268bdab3 ~__shared_count /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/shared_ptr_base.h:1070:6
[build] #<!-- -->64 0x0000564e268bdab3 ~__shared_ptr /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/shared_ptr_base.h:1524:31
[build] #<!-- -->65 0x0000564e268bdab3 runInvocation build-llvm/tools/clang/stage2-bins/clang-tools-extra/clang-tidy/ClangTidy.cpp:581:7
[build] #<!-- -->66 0x00007f9afc1f1414 ~__shared_count /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/shared_ptr_base.h:1070:6
[build] #<!-- -->67 0x00007f9afc1f1414 ~__shared_ptr /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/shared_ptr_base.h:1524:31
[build] #<!-- -->68 0x00007f9afc1f1414 clang::tooling::ToolInvocation::runInvocation(char const*, clang::driver::Compilation*, std::shared_ptr&lt;clang::CompilerInvocation&gt;, std::shared_ptr&lt;clang::PCHContainerOperations&gt;) build-llvm/tools/clang/stage2-bins/clang/lib/Tooling/Tooling.cpp:440:3
[build] #<!-- -->69 0x00007f9afc1f017f clang::tooling::ToolInvocation::run() build-llvm/tools/clang/stage2-bins/clang/lib/Tooling/Tooling.cpp:425:10
[build] #<!-- -->70 0x00007f9afc1f31c1 clang::tooling::ClangTool::run(clang::tooling::ToolAction*) build-llvm/tools/clang/stage2-bins/clang/lib/Tooling/Tooling.cpp:623:11
[build] #<!-- -->71 0x0000564e268b9d9b clang::tidy::runClangTidy(clang::tidy::ClangTidyContext&amp;, clang::tooling::CompilationDatabase const&amp;, llvm::ArrayRef&lt;std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;&gt;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::OverlayFileSystem&gt;, bool, bool, llvm::StringRef) build-llvm/tools/clang/stage2-bins/clang-tools-extra/clang-tidy/ClangTidy.cpp:0:8
[build] #<!-- -->72 0x0000564e25c39c65 release build-llvm/tools/clang/stage2-bins/llvm/include/llvm/ADT/IntrusiveRefCntPtr.h:232:9
[build] #<!-- -->73 0x0000564e25c39c65 ~IntrusiveRefCntPtr build-llvm/tools/clang/stage2-bins/llvm/include/llvm/ADT/IntrusiveRefCntPtr.h:196:27
[build] #<!-- -->74 0x0000564e25c39c65 clang::tidy::clangTidyMain(int, char const**) build-llvm/tools/clang/stage2-bins/clang-tools-extra/clang-tidy/tool/ClangTidyMain.cpp:681:7
[build] #<!-- -->75 0x00007f9af1a981ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
[build] #<!-- -->76 0x00007f9af1a9828b call_init ./csu/../csu/libc-start.c:128:20
[build] #<!-- -->77 0x00007f9af1a9828b __libc_start_main ./csu/../csu/libc-start.c:347:5
[build] #<!-- -->78 0x0000564e25c34fe5 _start (/usr/bin/clang-tidy+0x6fafe5)
```

e.g. (2)

```
$ clang-tidy --version
Ubuntu LLVM version 18.1.3
  Optimized build.
$ cat .clang-tidy
Checks: &gt;
  *,
  -abseil-*,
  -altera-*,
  -android-*,
  -clang-analyzer-*,
  -darwin-*,
  -google-objc-*,
  -linuxkernel-*,
  -llvmlibc-*,
  -mpi-*,
  -objc-*,
  -openmp-*,
  -zircon-*,
  -boost-use-to-string,
  -CWE-119,
  -cppcoreguidelines-avoid-magic-numbers,
  -cppcoreguidelines-pro-bounds-pointer-arithmetic,
  -fuchsia-default-arguments-calls,
  -fuchsia-default-arguments-declarations,
  -fuchsia-overloaded-operator,
  -google-readability-braces-around-statements,
  -hicpp-braces-around-statements,
  -hicpp-no-array-decay,
  -llvm-header-guard,
  -llvm-include-order,
  -modernize-use-trailing-return-type,
  -readability-braces-around-statements,
  -readability-identifier-length,
  -readability-implicit-bool-conversion,
  -readability-magic-numbers,
  -bugprone-argument-comment,
  -bugprone-bad-signal-to-kill-thread,
  -bugprone-bool-pointer-implicit-conversion,
  -bugprone-easily-swappable-parameters,
  -bugprone-exception-escape,
  -bugprone-implicit-widening-of-multiplication-result,
  -bugprone-lambda-function-name,
  -bugprone-misplaced-operator-in-strlen-in-alloc,
  -bugprone-misplaced-pointer-arithmetic-in-alloc,
  -bugprone-not-null-terminated-result,
  -bugprone-posix-return,
  -bugprone-signal-handler,
  -bugprone-spuriously-wake-up-functions,
  -bugprone-virtual-near-miss,
  -bugprone-unhandled-exception-at-new,
  -bugprone-unchecked-optional-access,
  -cert-con36-c,
  -cert-con54-cpp,
  -hicpp-uppercase-literal-suffix,
  -readability-uppercase-literal-suffix,
  -cert-dcl16-c,
  -cert-dcl21-cpp,
  -cert-dcl58-cpp,
  -cert-env33-c,
  -cert-pos44-c,
  -cert-pos47-c,
  -cert-sig30-c,
  -cert-msc30-c,
  -cert-msc50-cpp,
  -cert-msc32-c,
  -cert-msc51-cpp,
  -cppcoreguidelines-pro-bounds-constant-array-index,
  -fuchsia-multiple-inheritance,
  -fuchsia-statically-constructed-objects,
  -fuchsia-trailing-return,
  -fuchsia-virtual-inheritance,
  -google-build-explicit-make-pair,
  -google-build-using-namespace,
  -google-default-arguments,
  -google-readability-todo,
  -hicpp-named-parameter,
  -readability-named-parameter,
  -llvm-prefer-isa-or-dyn-cast-in-conditionals,
  -llvm-prefer-register-over-unsigned,
  -llvm-qualified-auto,
  -llvm-twine-local,
  -misc-confusable-identifiers,
  -misc-misleading-bidirectional,
  -misc-misleading-identifier,
  -misc-no-recursion,
  -misc-unused-parameters,
  -modernize-replace-disallow-copy-and-assign-macro,
  -modernize-use-nodiscard,
  -performance-inefficient-string-concatenation,
  -readability-isolate-declaration,
  -readability-qualified-auto,
  -readability-suspicious-call-argument,
  -hicpp-use-nullptr,
  -modernize-use-nullptr,
  -cppcoreguidelines-avoid-do-while,
  -misc-const-correctness,
  -readability-redundant-control-flow,
  -google-readability-casting,
  -cppcoreguidelines-pro-type-const-cast,
  -cppcoreguidelines-pro-type-cstyle-cast,
  -readability-container-size-empty,
  -cppcoreguidelines-init-variables,
  -cppcoreguidelines-non-private-member-variables-in-classes,
  -misc-non-private-member-variables-in-classes,
  -readability-avoid-const-params-in-decls,
  -modernize-return-braced-init-list,
  -readability-simplify-boolean-expr,
  -hicpp-signed-bitwise,
  -readability-function-cognitive-complexity,
  -readability-static-accessed-through-instance,
  -modernize-loop-convert,
  -llvm-else-after-return,
  -misc-include-cleaner,
  -readability-convert-member-functions-to-static,
  -hicpp-avoid-c-arrays,
  -hicpp-explicit-conversions,
  -hicpp-special-member-functions,
  -hicpp-noexcept-move,
  -cppcoreguidelines-noexcept-move-operations,
  -cppcoreguidelines-avoid-c-arrays,
  -bugprone-empty-catch,
  -hicpp-member-init,
  -modernize-use-constraints,
  -performance-enum-size,
  -readability-redundant-inline-specifier,
  -readability-redundant-member-init,
  -readability-avoid-return-with-void-value,
  -readability-avoid-nested-conditional-operator,
  -modernize-use-std-numbers,
  -readability-redundant-casting,
  -bugprone-suspicious-include,
  -hicpp-use-emplace,
  -boost-use-ranges,
  -modernize-use-designated-initializers,
  -readability-math-missing-parentheses,
  -modernize-min-max-use-initializer-list,
  -readability-use-std-min-max,
  -modernize-use-ranges,
  -bugprone-crtp-constructor-accessibility,
  -bugprone-suspicious-stringview-data-usage,
  -misc-use-internal-linkage
FormatStyle: "file"
CheckOptions:
  misc-non-private-member-variables-in-classes.IgnoreClassesWithAllMemberVariablesBeingPublic: "true"
  cppcoreguidelines-special-member-functions.AllowSoleDefaultDtor: "true"
```

e.g. (3)

```
[build] Error running '/usr/bin/clang-tidy': PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
[build] Stack dump:
[build] 0.	Program arguments: /usr/bin/clang-tidy -p /home/dev/snip/out/build/clang-tidy -header-filter=/home/dev/snip -warnings-as-errors=* --extra-arg-before=--driver-mode=g++ /home/dev/snip/.cpp
[build] 1.	&lt;eof&gt; parser at end of file
[build] 2.	ASTMatcher: Matching 'hicpp-invalid-access-moved' against:
[build] 	ImplicitCastExpr : &lt;/home/dev/snip/out/build/clang-tidy/vcpkg_installed/x64-linux/include/openssl/core_dispatch.h:591:1 &lt;Spelling=line:52:41&gt;&gt;
[build]  #<!-- -->0 0x00007f977cdd63bf llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) build-llvm/tools/clang/stage2-bins/llvm/lib/Support/Unix/Signals.inc:723:13
[build]  #<!-- -->1 0x00007f977cdd44f9 llvm::sys::RunSignalHandlers() build-llvm/tools/clang/stage2-bins/llvm/lib/Support/Signals.cpp:106:18
[build]  #<!-- -->2 0x00007f977cdd6b00 SignalHandler build-llvm/tools/clang/stage2-bins/llvm/lib/Support/Unix/Signals.inc:413:1
[build]  #<!-- -->3 0x00007f977baf7320 (/lib/x86_64-linux-gnu/libc.so.6+0x45320)
[build]  #<!-- -->4 0x00007f977bb5d0d4 _int_free_create_chunk ./malloc/malloc.c:4722:2
[build]  #<!-- -->5 0x00007f977bb5d0d4 _int_free_merge_chunk ./malloc/malloc.c:4700:10
[build]  #<!-- -->6 0x00007f977bb5fd9e cfree ./malloc/malloc.c:3401:3
[build]  #<!-- -->7 0x000056495dde0965 _M_erase /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:0:4
[build]  #<!-- -->8 0x000056495dde0959 _S_left /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:782:45
[build]  #<!-- -->9 0x000056495dde0959 _M_erase /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:1937:21
[build] #<!-- -->10 0x000056495dde0959 _S_left /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:782:45
[build] #<!-- -->11 0x000056495dde0959 _M_erase /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:1937:21
[build] #<!-- -->12 0x000056495dde0959 _S_left /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:782:45
[build] #<!-- -->13 0x000056495dde0959 _M_erase /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:1937:21
[build] #<!-- -->14 0x000056495dde0959 _S_left /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:782:45
[build] #<!-- -->15 0x000056495dde0959 _M_erase /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:1937:21
[build] #<!-- -->16 0x000056495dde0959 _S_left /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:782:45
[build] #<!-- -->17 0x000056495dde0959 _M_erase /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:1937:21
[build] #<!-- -->18 0x000056495dde0959 _S_left /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:782:45
[build] #<!-- -->19 0x000056495dde0959 _M_erase /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:1937:21
[build] #<!-- -->20 0x000056495dddee4c clear /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:0:2
[build] #<!-- -->21 0x000056495dddee4c clear /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_map.h:1184:14
[build] #<!-- -->22 0x000056495dddee4c matchesChildOf build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:675:19
[build] #<!-- -->23 0x000056495d94c1d2 matches build-llvm/tools/clang/stage2-bins/clang/include/clang/ASTMatchers/ASTMatchersInternal.h:1623:5
[build] #<!-- -->24 0x000056495de3d76e clang::ast_matchers::internal::DynTypedMatcher::matchesNoKindCheck(clang::DynTypedNode const&amp;, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchersInternal.cpp:332:23
[build] #<!-- -->25 0x000056495de4066a __find_if&lt;const clang::ast_matchers::internal::DynTypedMatcher *, __gnu_cxx::__ops::_Iter_negate&lt;(lambda at clang/lib/ASTMatchers/ASTMatchersInternal.cpp:394:38)&gt; &gt; /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_algobase.h:2092:8
[build] #<!-- -->26 0x000056495de4066a __find_if_not&lt;const clang::ast_matchers::internal::DynTypedMatcher *, __gnu_cxx::__ops::_Iter_pred&lt;(lambda at clang/lib/ASTMatchers/ASTMatchersInternal.cpp:394:38)&gt; &gt; /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_algo.h:112:14
[build] #<!-- -->27 0x000056495de4066a find_if_not&lt;const clang::ast_matchers::internal::DynTypedMatcher *, (lambda at clang/lib/ASTMatchers/ASTMatchersInternal.cpp:394:38)&gt; /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_algo.h:519:14
[build] #<!-- -->28 0x000056495de4066a all_of&lt;const clang::ast_matchers::internal::DynTypedMatcher *, (lambda at clang/lib/ASTMatchers/ASTMatchersInternal.cpp:394:38)&gt; /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_algo.h:459:24
[build] #<!-- -->29 0x000056495de4066a all_of&lt;llvm::ArrayRef&lt;clang::ast_matchers::internal::DynTypedMatcher&gt; &amp;, (lambda at clang/lib/ASTMatchers/ASTMatchersInternal.cpp:394:38)&gt; build-llvm/tools/clang/stage2-bins/llvm/include/llvm/ADT/STLExtras.h:1732:10
[build] #<!-- -->30 0x000056495de4066a allOfVariadicOperator build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchersInternal.cpp:394:10
[build] #<!-- -->31 0x000056495de4066a dynMatches build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchersInternal.cpp:123:12
[build] #<!-- -->32 0x000056495de3fc85 dynMatches build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchersInternal.cpp:0:33
[build] #<!-- -->33 0x000056495de3d62c clang::ast_matchers::internal::DynTypedMatcher::matches(clang::DynTypedNode const&amp;, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchersInternal.cpp:304:23
[build] #<!-- -->34 0x000056495d7f4641 matches build-llvm/tools/clang/stage2-bins/clang/include/clang/ASTMatchers/ASTMatchersInternal.h:1514:5
[build] #<!-- -->35 0x000056495de3d62c clang::ast_matchers::internal::DynTypedMatcher::matches(clang::DynTypedNode const&amp;, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchersInternal.cpp:304:23
[build] #<!-- -->36 0x000056495de0960e matchWithFilter build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:1064:11
[build] #<!-- -->37 0x000056495de212f1 TraverseStmt build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:1483:48
[build] #<!-- -->38 0x000056495de212f1 TraverseMemberExpr build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:2437:1
[build] #<!-- -->39 0x000056495de1c172 TraverseStmt build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:678:7
[build] #<!-- -->40 0x000056495de3bfa2 ~ASTNodeNotSpelledInSourceScope build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:971:47
[build] #<!-- -->41 0x000056495de3bfa2 TraverseStmt build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:1484:1
[build] #<!-- -->42 0x000056495de3bfa2 TraverseFunctionHelper build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:2198:5
[build] #<!-- -->43 0x000056495de0f586 TraverseFunctionDecl build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:2209:1
[build] #<!-- -->44 0x000056495de0c66f TraverseDecl build-llvm/tools/clang/stage2-bins/build-llvm/tools/clang/stage2-bins/tools/clang/include/clang/AST/DeclNodes.inc:238:1
[build] #<!-- -->45 0x000056495de0c66f TraverseDecl build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:1471:48
[build] #<!-- -->46 0x000056495de0cd7b TraverseDeclContextHelper build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:1480:7
[build] #<!-- -->47 0x000056495de0cd7b TraverseLinkageSpecDecl build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:1568:1
[build] #<!-- -->48 0x000056495de0c5df TraverseDecl build-llvm/tools/clang/stage2-bins/build-llvm/tools/clang/stage2-bins/tools/clang/include/clang/AST/DeclNodes.inc:36:1
[build] #<!-- -->49 0x000056495de0c5df TraverseDecl build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:1471:48
[build] #<!-- -->50 0x000056495de396eb TraverseDeclContextHelper build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:1480:7
[build] #<!-- -->51 0x000056495de0cb89 TraverseTranslationUnitDecl build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:1580:1
[build] #<!-- -->52 0x000056495de0c213 TraverseDecl build-llvm/tools/clang/stage2-bins/build-llvm/tools/clang/stage2-bins/tools/clang/include/clang/AST/DeclNodes.inc:24:1
[build] #<!-- -->53 0x000056495de0c213 TraverseDecl build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:1471:48
[build] #<!-- -->54 0x000056495ddde825 onEndOfTranslationUnit build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:446:30
[build] #<!-- -->55 0x000056495ddde825 clang::ast_matchers::MatchFinder::matchAST(clang::ASTContext&amp;) build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:1700:11
[build] #<!-- -->56 0x00007f978607dfbc operator++ /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_iterator.h:1111:2
[build] #<!-- -->57 0x00007f978607dfbc clang::MultiplexConsumer::HandleTranslationUnit(clang::ASTContext&amp;) build-llvm/tools/clang/stage2-bins/clang/lib/Frontend/MultiplexConsumer.cpp:297:23
[build] #<!-- -->58 0x00007f97841d23d6 __normal_iterator /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_iterator.h:1077:20
[build] #<!-- -->59 0x00007f97841d23d6 begin /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_vector.h:874:16
[build] #<!-- -->60 0x00007f97841d23d6 finalize&lt;std::vector&lt;std::unique_ptr&lt;clang::TemplateInstantiationCallback, std::default_delete&lt;clang::TemplateInstantiationCallback&gt; &gt;, std::allocator&lt;std::unique_ptr&lt;clang::TemplateInstantiationCallback, std::default_delete&lt;clang::TemplateInstantiationCallback&gt; &gt; &gt; &gt; &gt; build-llvm/tools/clang/stage2-bins/clang/include/clang/Sema/TemplateInstCallback.h:54:16
[build] #<!-- -->61 0x00007f97841d23d6 clang::ParseAST(clang::Sema&amp;, bool, bool) build-llvm/tools/clang/stage2-bins/clang/lib/Parse/ParseAST.cpp:183:3
[build] #<!-- -->62 0x00007f978604162c clang::FrontendAction::Execute() build-llvm/tools/clang/stage2-bins/clang/lib/Frontend/FrontendAction.cpp:1073:10
[build] #<!-- -->63 0x00007f9785fbe0b4 getPtr build-llvm/tools/clang/stage2-bins/llvm/include/llvm/Support/Error.h:279:12
[build] #<!-- -->64 0x00007f9785fbe0b4 operator bool build-llvm/tools/clang/stage2-bins/llvm/include/llvm/Support/Error.h:239:16
[build] #<!-- -->65 0x00007f9785fbe0b4 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) build-llvm/tools/clang/stage2-bins/clang/lib/Frontend/CompilerInstance.cpp:1057:23
[build] #<!-- -->66 0x00007f97862356b9 clang::tooling::FrontendActionFactory::runInvocation(std::shared_ptr&lt;clang::CompilerInvocation&gt;, clang::FileManager*, std::shared_ptr&lt;clang::PCHContainerOperations&gt;, clang::DiagnosticConsumer*) build-llvm/tools/clang/stage2-bins/clang/lib/Tooling/Tooling.cpp:465:33
[build] #<!-- -->67 0x000056495e45dab3 ~__shared_count /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/shared_ptr_base.h:1070:6
[build] #<!-- -->68 0x000056495e45dab3 ~__shared_ptr /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/shared_ptr_base.h:1524:31
[build] #<!-- -->69 0x000056495e45dab3 runInvocation build-llvm/tools/clang/stage2-bins/clang-tools-extra/clang-tidy/ClangTidy.cpp:581:7
[build] #<!-- -->70 0x00007f9786235414 ~__shared_count /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/shared_ptr_base.h:1070:6
[build] #<!-- -->71 0x00007f9786235414 ~__shared_ptr /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/shared_ptr_base.h:1524:31
[build] #<!-- -->72 0x00007f9786235414 clang::tooling::ToolInvocation::runInvocation(char const*, clang::driver::Compilation*, std::shared_ptr&lt;clang::CompilerInvocation&gt;, std::shared_ptr&lt;clang::PCHContainerOperations&gt;) build-llvm/tools/clang/stage2-bins/clang/lib/Tooling/Tooling.cpp:440:3
[build] #<!-- -->73 0x00007f978623417f clang::tooling::ToolInvocation::run() build-llvm/tools/clang/stage2-bins/clang/lib/Tooling/Tooling.cpp:425:10
[build] #<!-- -->74 0x00007f97862371c1 clang::tooling::ClangTool::run(clang::tooling::ToolAction*) build-llvm/tools/clang/stage2-bins/clang/lib/Tooling/Tooling.cpp:623:11
[build] #<!-- -->75 0x000056495e459d9b clang::tidy::runClangTidy(clang::tidy::ClangTidyContext&amp;, clang::tooling::CompilationDatabase const&amp;, llvm::ArrayRef&lt;std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;&gt;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::OverlayFileSystem&gt;, bool, bool, llvm::StringRef) build-llvm/tools/clang/stage2-bins/clang-tools-extra/clang-tidy/ClangTidy.cpp:0:8
[build] #<!-- -->76 0x000056495d7d9c65 release build-llvm/tools/clang/stage2-bins/llvm/include/llvm/ADT/IntrusiveRefCntPtr.h:232:9
[build] #<!-- -->77 0x000056495d7d9c65 ~IntrusiveRefCntPtr build-llvm/tools/clang/stage2-bins/llvm/include/llvm/ADT/IntrusiveRefCntPtr.h:196:27
[build] #<!-- -->78 0x000056495d7d9c65 clang::tidy::clangTidyMain(int, char const**) build-llvm/tools/clang/stage2-bins/clang-tools-extra/clang-tidy/tool/ClangTidyMain.cpp:681:7
[build] #<!-- -->79 0x00007f977badc1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
[build] #<!-- -->80 0x00007f977badc28b call_init ./csu/../csu/libc-start.c:128:20
[build] #<!-- -->81 0x00007f977badc28b __libc_start_main ./csu/../csu/libc-start.c:347:5
[build] #<!-- -->82 0x000056495d7d4fe5 _start (/usr/bin/clang-tidy+0x6fafe5)
[build] Segmentation fault
```
e.g. (4)

```
[build] 0.	Program arguments: /usr/bin/clang-tidy -p /home/dev/snip/out/build/clang-tidy -header-filter=/home/dev/snip -warnings-as-errors=* --extra-arg-before=--driver-mode=g++ /home/dev/snip.cpp
[build] 1.	&lt;eof&gt; parser at end of file
[build] 2.	ASTMatcher: Matching 'modernize-make-unique' against:
[build] 	CXXMemberCallExpr : &lt;/usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/basic_string.h:1354:20, col:31&gt;
[build]  #<!-- -->0 0x00007f881db1c3bf llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) build-llvm/tools/clang/stage2-bins/llvm/lib/Support/Unix/Signals.inc:723:13
[build]  #<!-- -->1 0x00007f881db1a4f9 llvm::sys::RunSignalHandlers() build-llvm/tools/clang/stage2-bins/llvm/lib/Support/Signals.cpp:106:18
[build]  #<!-- -->2 0x00007f881db1cb00 SignalHandler build-llvm/tools/clang/stage2-bins/llvm/lib/Support/Unix/Signals.inc:413:1
[build]  #<!-- -->3 0x00007f881c83d320 (/lib/x86_64-linux-gnu/libc.so.6+0x45320)
[build]  #<!-- -->4 0x00007f881c8a3027 _int_free_create_chunk ./malloc/malloc.c:4720:10
[build]  #<!-- -->5 0x00007f881c8a3027 _int_free_merge_chunk ./malloc/malloc.c:4700:10
[build]  #<!-- -->6 0x00007f881c8a5d9e cfree ./malloc/malloc.c:3401:3
[build]  #<!-- -->7 0x000055b4c3786965 _M_erase /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:0:4
[build]  #<!-- -->8 0x000055b4c3786959 _S_left /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:782:45
[build]  #<!-- -->9 0x000055b4c3786959 _M_erase /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:1937:21
[build] #<!-- -->10 0x000055b4c3786959 _S_left /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:782:45
[build] #<!-- -->11 0x000055b4c3786959 _M_erase /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:1937:21
[build] #<!-- -->12 0x000055b4c3786959 _S_left /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:782:45
[build] #<!-- -->13 0x000055b4c3786959 _M_erase /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:1937:21
[build] #<!-- -->14 0x000055b4c3786959 _S_left /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:782:45
[build] #<!-- -->15 0x000055b4c3786959 _M_erase /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:1937:21
[build] #<!-- -->16 0x000055b4c3786959 _S_left /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:782:45
[build] #<!-- -->17 0x000055b4c3786959 _M_erase /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:1937:21
[build] #<!-- -->18 0x000055b4c3784fb4 clear /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:0:2
[build] #<!-- -->19 0x000055b4c3784fb4 clear /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_map.h:1184:14
[build] #<!-- -->20 0x000055b4c3784fb4 matchesAncestorOf build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:696:19
[build] #<!-- -->21 0x000055b4c31994bf matches build-llvm/tools/clang/stage2-bins/clang/include/clang/ASTMatchers/ASTMatchersInternal.h:1697:5
[build] #<!-- -->22 0x000055b4c37e362c clang::ast_matchers::internal::DynTypedMatcher::matches(clang::DynTypedNode const&amp;, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchersInternal.cpp:304:23
[build] #<!-- -->23 0x000055b4c37e7045 notUnaryOperator build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchersInternal.cpp:384:28
[build] #<!-- -->24 0x000055b4c37e7045 dynMatches build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchersInternal.cpp:123:12
[build] #<!-- -->25 0x000055b4c37e376e clang::ast_matchers::internal::DynTypedMatcher::matchesNoKindCheck(clang::DynTypedNode const&amp;, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchersInternal.cpp:332:23
[build] #<!-- -->26 0x000055b4c37e6585 __find_if&lt;const clang::ast_matchers::internal::DynTypedMatcher *, __gnu_cxx::__ops::_Iter_negate&lt;(lambda at clang/lib/ASTMatchers/ASTMatchersInternal.cpp:394:38)&gt; &gt; /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_algobase.h:2072:8
[build] #<!-- -->27 0x000055b4c37e6585 __find_if_not&lt;const clang::ast_matchers::internal::DynTypedMatcher *, __gnu_cxx::__ops::_Iter_pred&lt;(lambda at clang/lib/ASTMatchers/ASTMatchersInternal.cpp:394:38)&gt; &gt; /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_algo.h:112:14
[build] #<!-- -->28 0x000055b4c37e6585 find_if_not&lt;const clang::ast_matchers::internal::DynTypedMatcher *, (lambda at clang/lib/ASTMatchers/ASTMatchersInternal.cpp:394:38)&gt; /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_algo.h:519:14
[build] #<!-- -->29 0x000055b4c37e6585 all_of&lt;const clang::ast_matchers::internal::DynTypedMatcher *, (lambda at clang/lib/ASTMatchers/ASTMatchersInternal.cpp:394:38)&gt; /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_algo.h:459:24
[build] #<!-- -->30 0x000055b4c37e6585 all_of&lt;llvm::ArrayRef&lt;clang::ast_matchers::internal::DynTypedMatcher&gt; &amp;, (lambda at clang/lib/ASTMatchers/ASTMatchersInternal.cpp:394:38)&gt; build-llvm/tools/clang/stage2-bins/llvm/include/llvm/ADT/STLExtras.h:1732:10
[build] #<!-- -->31 0x000055b4c37e6585 allOfVariadicOperator build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchersInternal.cpp:394:10
[build] #<!-- -->32 0x000055b4c37e6585 dynMatches build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchersInternal.cpp:123:12
[build] #<!-- -->33 0x000055b4c37e5c85 dynMatches build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchersInternal.cpp:0:33
[build] #<!-- -->34 0x000055b4c37e362c clang::ast_matchers::internal::DynTypedMatcher::matches(clang::DynTypedNode const&amp;, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchersInternal.cpp:304:23
[build] #<!-- -->35 0x000055b4c319a641 matches build-llvm/tools/clang/stage2-bins/clang/include/clang/ASTMatchers/ASTMatchersInternal.h:1514:5
[build] #<!-- -->36 0x000055b4c37e362c clang::ast_matchers::internal::DynTypedMatcher::matches(clang::DynTypedNode const&amp;, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchersInternal.cpp:304:23
[build] #<!-- -->37 0x000055b4c37af60e matchWithFilter build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:1064:11
[build] #<!-- -->38 0x000055b4c37ce4a1 TraverseStmt build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:1483:48
[build] #<!-- -->39 0x000055b4c37ce4a1 TraverseBinaryOperator build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:2852:1
[build] #<!-- -->40 0x000055b4c37c1b0c TraverseStmt build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:678:7
[build] #<!-- -->41 0x000055b4c37e1fa2 ~ASTNodeNotSpelledInSourceScope build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:971:47
[build] #<!-- -->42 0x000055b4c37e1fa2 TraverseStmt build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:1484:1
[build] #<!-- -->43 0x000055b4c37e1fa2 TraverseFunctionHelper build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:2198:5
[build] #<!-- -->44 0x000055b4c37b5636 TraverseCXXMethodDecl build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:2223:1
[build] #<!-- -->45 0x000055b4c37b27ef TraverseDecl build-llvm/tools/clang/stage2-bins/build-llvm/tools/clang/stage2-bins/tools/clang/include/clang/AST/DeclNodes.inc:242:1
[build] #<!-- -->46 0x000055b4c37b27ef TraverseDecl build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:1471:48
[build] #<!-- -->47 0x000055b4c37b799b TraverseDeclContextHelper build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:1480:7
[build] #<!-- -->48 0x000055b4c37b799b TraverseClassTemplateSpecializationDecl build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:2026:1
[build] #<!-- -->49 0x000055b4c37b287f TraverseDecl build-llvm/tools/clang/stage2-bins/build-llvm/tools/clang/stage2-bins/tools/clang/include/clang/AST/DeclNodes.inc:418:1
[build] #<!-- -->50 0x000055b4c37b287f TraverseDecl build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:1471:48
[build] #<!-- -->51 0x000055b4c37b9252 TraverseTemplateInstantiations build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:1791:9
[build] #<!-- -->52 0x000055b4c37b9252 TraverseTemplateInstantiations build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:759:50
[build] #<!-- -->53 0x000055b4c37b9252 TraverseClassTemplateDecl build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:1883:1
[build] #<!-- -->54 0x000055b4c37b23cf TraverseDecl build-llvm/tools/clang/stage2-bins/build-llvm/tools/clang/stage2-bins/tools/clang/include/clang/AST/DeclNodes.inc:524:1
[build] #<!-- -->55 0x000055b4c37b23cf TraverseDecl build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:1471:48
[build] #<!-- -->56 0x000055b4c37b496b TraverseDeclContextHelper build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:1480:7
[build] #<!-- -->57 0x000055b4c37b496b TraverseNamespaceDecl build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:1616:1
[build] #<!-- -->58 0x000055b4c37b2747 TraverseDecl build-llvm/tools/clang/stage2-bins/build-llvm/tools/clang/stage2-bins/tools/clang/include/clang/AST/DeclNodes.inc:170:1
[build] #<!-- -->59 0x000055b4c37b2747 TraverseDecl build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:1471:48
[build] #<!-- -->60 0x000055b4c37b496b TraverseDeclContextHelper build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:1480:7
[build] #<!-- -->61 0x000055b4c37b496b TraverseNamespaceDecl build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:1616:1
[build] #<!-- -->62 0x000055b4c37b2747 TraverseDecl build-llvm/tools/clang/stage2-bins/build-llvm/tools/clang/stage2-bins/tools/clang/include/clang/AST/DeclNodes.inc:170:1
[build] #<!-- -->63 0x000055b4c37b2747 TraverseDecl build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:1471:48
[build] #<!-- -->64 0x000055b4c37df6eb TraverseDeclContextHelper build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:1480:7
[build] #<!-- -->65 0x000055b4c37b2b89 TraverseTranslationUnitDecl build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:1580:1
[build] #<!-- -->66 0x000055b4c37b2213 TraverseDecl build-llvm/tools/clang/stage2-bins/build-llvm/tools/clang/stage2-bins/tools/clang/include/clang/AST/DeclNodes.inc:24:1
[build] #<!-- -->67 0x000055b4c37b2213 TraverseDecl build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:1471:48
[build] #<!-- -->68 0x000055b4c3784825 onEndOfTranslationUnit build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:446:30
[build] #<!-- -->69 0x000055b4c3784825 clang::ast_matchers::MatchFinder::matchAST(clang::ASTContext&amp;) build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:1700:11
[build] #<!-- -->70 0x00007f8826dc3fbc operator++ /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_iterator.h:1111:2
[build] #<!-- -->71 0x00007f8826dc3fbc clang::MultiplexConsumer::HandleTranslationUnit(clang::ASTContext&amp;) build-llvm/tools/clang/stage2-bins/clang/lib/Frontend/MultiplexConsumer.cpp:297:23
[build] #<!-- -->72 0x00007f8824f183d6 __normal_iterator /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_iterator.h:1077:20
[build] #<!-- -->73 0x00007f8824f183d6 begin /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_vector.h:874:16
[build] #<!-- -->74 0x00007f8824f183d6 finalize&lt;std::vector&lt;std::unique_ptr&lt;clang::TemplateInstantiationCallback, std::default_delete&lt;clang::TemplateInstantiationCallback&gt; &gt;, std::allocator&lt;std::unique_ptr&lt;clang::TemplateInstantiationCallback, std::default_delete&lt;clang::TemplateInstantiationCallback&gt; &gt; &gt; &gt; &gt; build-llvm/tools/clang/stage2-bins/clang/include/clang/Sema/TemplateInstCallback.h:54:16
[build] #<!-- -->75 0x00007f8824f183d6 clang::ParseAST(clang::Sema&amp;, bool, bool) build-llvm/tools/clang/stage2-bins/clang/lib/Parse/ParseAST.cpp:183:3
[build] #<!-- -->76 0x00007f8826d8762c clang::FrontendAction::Execute() build-llvm/tools/clang/stage2-bins/clang/lib/Frontend/FrontendAction.cpp:1073:10
[build] #<!-- -->77 0x00007f8826d040b4 getPtr build-llvm/tools/clang/stage2-bins/llvm/include/llvm/Support/Error.h:279:12
[build] #<!-- -->78 0x00007f8826d040b4 operator bool build-llvm/tools/clang/stage2-bins/llvm/include/llvm/Support/Error.h:239:16
[build] #<!-- -->79 0x00007f8826d040b4 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) build-llvm/tools/clang/stage2-bins/clang/lib/Frontend/CompilerInstance.cpp:1057:23
[build] #<!-- -->80 0x00007f8826f7b6b9 clang::tooling::FrontendActionFactory::runInvocation(std::shared_ptr&lt;clang::CompilerInvocation&gt;, clang::FileManager*, std::shared_ptr&lt;clang::PCHContainerOperations&gt;, clang::DiagnosticConsumer*) build-llvm/tools/clang/stage2-bins/clang/lib/Tooling/Tooling.cpp:465:33
[build] #<!-- -->81 0x000055b4c3e03ab3 ~__shared_count /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/shared_ptr_base.h:1070:6
[build] #<!-- -->82 0x000055b4c3e03ab3 ~__shared_ptr /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/shared_ptr_base.h:1524:31
[build] #<!-- -->83 0x000055b4c3e03ab3 runInvocation build-llvm/tools/clang/stage2-bins/clang-tools-extra/clang-tidy/ClangTidy.cpp:581:7
[build] #<!-- -->84 0x00007f8826f7b414 ~__shared_count /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/shared_ptr_base.h:1070:6
[build] #<!-- -->85 0x00007f8826f7b414 ~__shared_ptr /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/shared_ptr_base.h:1524:31
[build] #<!-- -->86 0x00007f8826f7b414 clang::tooling::ToolInvocation::runInvocation(char const*, clang::driver::Compilation*, std::shared_ptr&lt;clang::CompilerInvocation&gt;, std::shared_ptr&lt;clang::PCHContainerOperations&gt;) build-llvm/tools/clang/stage2-bins/clang/lib/Tooling/Tooling.cpp:440:3
[build] #<!-- -->87 0x00007f8826f7a17f clang::tooling::ToolInvocation::run() build-llvm/tools/clang/stage2-bins/clang/lib/Tooling/Tooling.cpp:425:10
[build] #<!-- -->88 0x00007f8826f7d1c1 clang::tooling::ClangTool::run(clang::tooling::ToolAction*) build-llvm/tools/clang/stage2-bins/clang/lib/Tooling/Tooling.cpp:623:11
[build] #<!-- -->89 0x000055b4c3dffd9b clang::tidy::runClangTidy(clang::tidy::ClangTidyContext&amp;, clang::tooling::CompilationDatabase const&amp;, llvm::ArrayRef&lt;std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;&gt;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::OverlayFileSystem&gt;, bool, bool, llvm::StringRef) build-llvm/tools/clang/stage2-bins/clang-tools-extra/clang-tidy/ClangTidy.cpp:0:8
[build] #<!-- -->90 0x000055b4c317fc65 release build-llvm/tools/clang/stage2-bins/llvm/include/llvm/ADT/IntrusiveRefCntPtr.h:232:9
[build] #<!-- -->91 0x000055b4c317fc65 ~IntrusiveRefCntPtr build-llvm/tools/clang/stage2-bins/llvm/include/llvm/ADT/IntrusiveRefCntPtr.h:196:27
[build] #<!-- -->92 0x000055b4c317fc65 clang::tidy::clangTidyMain(int, char const**) build-llvm/tools/clang/stage2-bins/clang-tools-extra/clang-tidy/tool/ClangTidyMain.cpp:681:7
[build] #<!-- -->93 0x00007f881c8221ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
[build] #<!-- -->94 0x00007f881c82228b call_init ./csu/../csu/libc-start.c:128:20
[build] #<!-- -->95 0x00007f881c82228b __libc_start_main ./csu/../csu/libc-start.c:347:5
[build] #<!-- -->96 0x000055b4c317afe5 _start (/usr/bin/clang-tidy+0x6fafe5)
[build] Segmentation fault
```

Command line to invoke
```
[build] /usr/bin/cmake -E __run_co_compile --launcher="/usr/bin/cmake;-E;env;CCACHE_SLOPPINESS=pch_defines,time_macros,include_file_mtime,include_file_ctime;/usr/local/bin/ccache" --tidy="/usr/bin/clang-tidy;-p;/home/dev/snip/out/build/clang-tidy;-header-filter=/home/dev/snip;-warnings-as-errors=*;--extra-arg-before=--driver-mode=g++" --source=/home/dev/snip/src/lib/snip.cpp -- /usr/bin/clang++ -DBOOST_CONTAINER_NO_LIB -DBOOST_CONTAINER_STATIC_LINK -DBOOST_CONTEXT_EXPORT="" -DBOOST_CONTEXT_NO_LIB="" -DBOOST_CONTEXT_STATIC_LINK="" -DBOOST_COROUTINE_NO_LIB -DBOOST_COROUTINE_STATIC_LINK -DBOOST_DATE_TIME_NO_LIB -DBOOST_DATE_TIME_STATIC_LINK -DSPDLOG_COMPILED_LIB -DSPDLOG_FMT_EXTERNAL -I/home/dev/snip/src/lib -I/home/dev/snip/src -I/home/dev/snip/out/build/clang-tidy/src/snip/pb -isystem /home/dev/snip/out/build/clang-tidy/vcpkg_installed/x64-linux/include -O3 -DNDEBUG -std=c++20 -fPIC -Wall -Wextra -Xclang -fno-pch-timestamp -Werror -MD -MT src/lib/snip.cpp.o -MF src/lib/snip.o.d -o src/snip.cpp.o -c /home/dev/snip.cpp
```

Sorry, this is commercial code, unable to provide the whole thing, but this is compiling against boost redis. Here are the main 3rd party includes

```c++
#include &lt;boost/asio/co_spawn.hpp&gt;
#include &lt;boost/asio/consign.hpp&gt;
#include &lt;boost/asio/deferred.hpp&gt;
#include &lt;boost/asio/detached.hpp&gt;
#include &lt;boost/asio/io_context.hpp&gt;
#include &lt;boost/asio/redirect_error.hpp&gt;
#include &lt;boost/asio/signal_set.hpp&gt;
#include &lt;boost/asio/use_awaitable.hpp&gt;
#include &lt;boost/asio/use_future.hpp&gt;
#include &lt;boost/redis/config.hpp&gt;
#include &lt;boost/redis/connection.hpp&gt;
#include &lt;chrono&gt;
#include &lt;nlohmann/json.hpp&gt;
#include &lt;memory&gt;
#include &lt;numeric&gt;
#include &lt;ranges&gt;
#include &lt;thread&gt;
#include &lt;tl/to.hpp&gt;
````

</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-tidy

Author: None (tonygould)

<details>
Sometimes succeeds, sometimes fails with different backtraces

e.g. (1)

```
[build] Error running '/usr/bin/clang-tidy': PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
[build] Stack dump:
[build] 0.	Program arguments: /usr/bin/clang-tidy -p /home/dev/snip/out/build/clang-tidy -header-filter=/home/dev/snip -warnings-as-errors=* --extra-arg-before=--driver-mode=g++ /home/dev/snip/snip.cpp
[build] 1.	&lt;eof&gt; parser at end of file
[build] 2.	ASTMatcher: Matching 'cppcoreguidelines-macro-to-enum' against:
[build] 	ParmVarDecl  : &lt;/usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_deque.h:2232:53&gt;
[build]  #<!-- -->0 0x00007f9af2d923bf llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) build-llvm/tools/clang/stage2-bins/llvm/lib/Support/Unix/Signals.inc:723:13
[build]  #<!-- -->1 0x00007f9af2d904f9 llvm::sys::RunSignalHandlers() build-llvm/tools/clang/stage2-bins/llvm/lib/Support/Signals.cpp:106:18
[build]  #<!-- -->2 0x00007f9af2d92b00 SignalHandler build-llvm/tools/clang/stage2-bins/llvm/lib/Support/Unix/Signals.inc:413:1
[build]  #<!-- -->3 0x00007f9af1ab3320 (/lib/x86_64-linux-gnu/libc.so.6+0x45320)
[build]  #<!-- -->4 0x00007f9af1b190d3 _int_free_create_chunk ./malloc/malloc.c:4722:2
[build]  #<!-- -->5 0x00007f9af1b190d3 _int_free_merge_chunk ./malloc/malloc.c:4700:10
[build]  #<!-- -->6 0x00007f9af1b1bd9e cfree ./malloc/malloc.c:3401:3
[build]  #<!-- -->7 0x0000564e26240965 _M_erase /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:0:4
[build]  #<!-- -->8 0x0000564e26240959 _S_left /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:782:45
[build]  #<!-- -->9 0x0000564e26240959 _M_erase /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:1937:21
[build] #<!-- -->10 0x0000564e26240959 _S_left /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:782:45
[build] #<!-- -->11 0x0000564e26240959 _M_erase /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:1937:21
[build] #<!-- -->12 0x0000564e26240959 _S_left /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:782:45
[build] #<!-- -->13 0x0000564e26240959 _M_erase /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:1937:21
[build] #<!-- -->14 0x0000564e26240959 _S_left /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:782:45
[build] #<!-- -->15 0x0000564e26240959 _M_erase /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:1937:21
[build] #<!-- -->16 0x0000564e26240959 _S_left /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:782:45
[build] #<!-- -->17 0x0000564e26240959 _M_erase /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:1937:21
[build] #<!-- -->18 0x0000564e26240959 _S_left /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:782:45
[build] #<!-- -->19 0x0000564e26240959 _M_erase /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:1937:21
[build] #<!-- -->20 0x0000564e2623efb4 clear /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:0:2
[build] #<!-- -->21 0x0000564e2623efb4 clear /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_map.h:1184:14
[build] #<!-- -->22 0x0000564e2623efb4 matchesAncestorOf build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:696:19
[build] #<!-- -->23 0x0000564e25f1d602 matches build-llvm/tools/clang/stage2-bins/clang/include/clang/ASTMatchers/ASTMatchersInternal.h:1675:5
[build] #<!-- -->24 0x0000564e2629fc85 dynMatches build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchersInternal.cpp:0:33
[build] #<!-- -->25 0x0000564e2629d62c clang::ast_matchers::internal::DynTypedMatcher::matches(clang::DynTypedNode const&amp;, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchersInternal.cpp:304:23
[build] #<!-- -->26 0x0000564e2626960e matchWithFilter build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:1064:11
[build] #<!-- -->27 0x0000564e2626c1eb getKind build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/DeclBase.h:447:51
[build] #<!-- -->28 0x0000564e2626c1eb TraverseDecl build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:743:14
[build] #<!-- -->29 0x0000564e2626c1eb TraverseDecl build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:1471:48
[build] #<!-- -->30 0x0000564e26276ab8 TraverseFunctionProtoTypeLoc build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:1309:1
[build] #<!-- -->31 0x0000564e2629bc35 TraverseFunctionHelper build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:2158:5
[build] #<!-- -->32 0x0000564e2626f636 TraverseCXXMethodDecl build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:2223:1
[build] #<!-- -->33 0x0000564e2626c7ef TraverseDecl build-llvm/tools/clang/stage2-bins/build-llvm/tools/clang/stage2-bins/tools/clang/include/clang/AST/DeclNodes.inc:242:1
[build] #<!-- -->34 0x0000564e2626c7ef TraverseDecl build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:1471:48
[build] #<!-- -->35 0x0000564e262717fb TraverseDeclContextHelper build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:1480:7
[build] #<!-- -->36 0x0000564e262717fb TraverseCXXRecordDecl build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:1996:1
[build] #<!-- -->37 0x0000564e2626c36f TraverseDecl build-llvm/tools/clang/stage2-bins/build-llvm/tools/clang/stage2-bins/tools/clang/include/clang/AST/DeclNodes.inc:414:1
[build] #<!-- -->38 0x0000564e2626c36f TraverseDecl build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:1471:48
[build] #<!-- -->39 0x0000564e262731a1 TraverseClassTemplateDecl build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:1883:1
[build] #<!-- -->40 0x0000564e2626c3cf TraverseDecl build-llvm/tools/clang/stage2-bins/build-llvm/tools/clang/stage2-bins/tools/clang/include/clang/AST/DeclNodes.inc:524:1
[build] #<!-- -->41 0x0000564e2626c3cf TraverseDecl build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:1471:48
[build] #<!-- -->42 0x0000564e2626e96b TraverseDeclContextHelper build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:1480:7
[build] #<!-- -->43 0x0000564e2626e96b TraverseNamespaceDecl build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:1616:1
[build] #<!-- -->44 0x0000564e2626c747 TraverseDecl build-llvm/tools/clang/stage2-bins/build-llvm/tools/clang/stage2-bins/tools/clang/include/clang/AST/DeclNodes.inc:170:1
[build] #<!-- -->45 0x0000564e2626c747 TraverseDecl build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:1471:48
[build] #<!-- -->46 0x0000564e262996eb TraverseDeclContextHelper build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:1480:7
[build] #<!-- -->47 0x0000564e2626cb89 TraverseTranslationUnitDecl build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:1580:1
[build] #<!-- -->48 0x0000564e2626c213 TraverseDecl build-llvm/tools/clang/stage2-bins/build-llvm/tools/clang/stage2-bins/tools/clang/include/clang/AST/DeclNodes.inc:24:1
[build] #<!-- -->49 0x0000564e2626c213 TraverseDecl build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:1471:48
[build] #<!-- -->50 0x0000564e2623e825 onEndOfTranslationUnit build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:446:30
[build] #<!-- -->51 0x0000564e2623e825 clang::ast_matchers::MatchFinder::matchAST(clang::ASTContext&amp;) build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:1700:11
[build] #<!-- -->52 0x00007f9afc039fbc operator++ /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_iterator.h:1111:2
[build] #<!-- -->53 0x00007f9afc039fbc clang::MultiplexConsumer::HandleTranslationUnit(clang::ASTContext&amp;) build-llvm/tools/clang/stage2-bins/clang/lib/Frontend/MultiplexConsumer.cpp:297:23
[build] #<!-- -->54 0x00007f9afa18e3d6 __normal_iterator /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_iterator.h:1077:20
[build] #<!-- -->55 0x00007f9afa18e3d6 begin /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_vector.h:874:16
[build] #<!-- -->56 0x00007f9afa18e3d6 finalize&lt;std::vector&lt;std::unique_ptr&lt;clang::TemplateInstantiationCallback, std::default_delete&lt;clang::TemplateInstantiationCallback&gt; &gt;, std::allocator&lt;std::unique_ptr&lt;clang::TemplateInstantiationCallback, std::default_delete&lt;clang::TemplateInstantiationCallback&gt; &gt; &gt; &gt; &gt; build-llvm/tools/clang/stage2-bins/clang/include/clang/Sema/TemplateInstCallback.h:54:16
[build] #<!-- -->57 0x00007f9afa18e3d6 clang::ParseAST(clang::Sema&amp;, bool, bool) build-llvm/tools/clang/stage2-bins/clang/lib/Parse/ParseAST.cpp:183:3
[build] #<!-- -->58 0x00007f9afbffd62c clang::FrontendAction::Execute() build-llvm/tools/clang/stage2-bins/clang/lib/Frontend/FrontendAction.cpp:1073:10
[build] #<!-- -->59 0x00007f9afbf7a0b4 getPtr build-llvm/tools/clang/stage2-bins/llvm/include/llvm/Support/Error.h:279:12
[build] #<!-- -->60 0x00007f9afbf7a0b4 operator bool build-llvm/tools/clang/stage2-bins/llvm/include/llvm/Support/Error.h:239:16
[build] #<!-- -->61 0x00007f9afbf7a0b4 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) build-llvm/tools/clang/stage2-bins/clang/lib/Frontend/CompilerInstance.cpp:1057:23
[build] #<!-- -->62 0x00007f9afc1f16b9 clang::tooling::FrontendActionFactory::runInvocation(std::shared_ptr&lt;clang::CompilerInvocation&gt;, clang::FileManager*, std::shared_ptr&lt;clang::PCHContainerOperations&gt;, clang::DiagnosticConsumer*) build-llvm/tools/clang/stage2-bins/clang/lib/Tooling/Tooling.cpp:465:33
[build] #<!-- -->63 0x0000564e268bdab3 ~__shared_count /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/shared_ptr_base.h:1070:6
[build] #<!-- -->64 0x0000564e268bdab3 ~__shared_ptr /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/shared_ptr_base.h:1524:31
[build] #<!-- -->65 0x0000564e268bdab3 runInvocation build-llvm/tools/clang/stage2-bins/clang-tools-extra/clang-tidy/ClangTidy.cpp:581:7
[build] #<!-- -->66 0x00007f9afc1f1414 ~__shared_count /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/shared_ptr_base.h:1070:6
[build] #<!-- -->67 0x00007f9afc1f1414 ~__shared_ptr /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/shared_ptr_base.h:1524:31
[build] #<!-- -->68 0x00007f9afc1f1414 clang::tooling::ToolInvocation::runInvocation(char const*, clang::driver::Compilation*, std::shared_ptr&lt;clang::CompilerInvocation&gt;, std::shared_ptr&lt;clang::PCHContainerOperations&gt;) build-llvm/tools/clang/stage2-bins/clang/lib/Tooling/Tooling.cpp:440:3
[build] #<!-- -->69 0x00007f9afc1f017f clang::tooling::ToolInvocation::run() build-llvm/tools/clang/stage2-bins/clang/lib/Tooling/Tooling.cpp:425:10
[build] #<!-- -->70 0x00007f9afc1f31c1 clang::tooling::ClangTool::run(clang::tooling::ToolAction*) build-llvm/tools/clang/stage2-bins/clang/lib/Tooling/Tooling.cpp:623:11
[build] #<!-- -->71 0x0000564e268b9d9b clang::tidy::runClangTidy(clang::tidy::ClangTidyContext&amp;, clang::tooling::CompilationDatabase const&amp;, llvm::ArrayRef&lt;std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;&gt;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::OverlayFileSystem&gt;, bool, bool, llvm::StringRef) build-llvm/tools/clang/stage2-bins/clang-tools-extra/clang-tidy/ClangTidy.cpp:0:8
[build] #<!-- -->72 0x0000564e25c39c65 release build-llvm/tools/clang/stage2-bins/llvm/include/llvm/ADT/IntrusiveRefCntPtr.h:232:9
[build] #<!-- -->73 0x0000564e25c39c65 ~IntrusiveRefCntPtr build-llvm/tools/clang/stage2-bins/llvm/include/llvm/ADT/IntrusiveRefCntPtr.h:196:27
[build] #<!-- -->74 0x0000564e25c39c65 clang::tidy::clangTidyMain(int, char const**) build-llvm/tools/clang/stage2-bins/clang-tools-extra/clang-tidy/tool/ClangTidyMain.cpp:681:7
[build] #<!-- -->75 0x00007f9af1a981ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
[build] #<!-- -->76 0x00007f9af1a9828b call_init ./csu/../csu/libc-start.c:128:20
[build] #<!-- -->77 0x00007f9af1a9828b __libc_start_main ./csu/../csu/libc-start.c:347:5
[build] #<!-- -->78 0x0000564e25c34fe5 _start (/usr/bin/clang-tidy+0x6fafe5)
```

e.g. (2)

```
$ clang-tidy --version
Ubuntu LLVM version 18.1.3
  Optimized build.
$ cat .clang-tidy
Checks: &gt;
  *,
  -abseil-*,
  -altera-*,
  -android-*,
  -clang-analyzer-*,
  -darwin-*,
  -google-objc-*,
  -linuxkernel-*,
  -llvmlibc-*,
  -mpi-*,
  -objc-*,
  -openmp-*,
  -zircon-*,
  -boost-use-to-string,
  -CWE-119,
  -cppcoreguidelines-avoid-magic-numbers,
  -cppcoreguidelines-pro-bounds-pointer-arithmetic,
  -fuchsia-default-arguments-calls,
  -fuchsia-default-arguments-declarations,
  -fuchsia-overloaded-operator,
  -google-readability-braces-around-statements,
  -hicpp-braces-around-statements,
  -hicpp-no-array-decay,
  -llvm-header-guard,
  -llvm-include-order,
  -modernize-use-trailing-return-type,
  -readability-braces-around-statements,
  -readability-identifier-length,
  -readability-implicit-bool-conversion,
  -readability-magic-numbers,
  -bugprone-argument-comment,
  -bugprone-bad-signal-to-kill-thread,
  -bugprone-bool-pointer-implicit-conversion,
  -bugprone-easily-swappable-parameters,
  -bugprone-exception-escape,
  -bugprone-implicit-widening-of-multiplication-result,
  -bugprone-lambda-function-name,
  -bugprone-misplaced-operator-in-strlen-in-alloc,
  -bugprone-misplaced-pointer-arithmetic-in-alloc,
  -bugprone-not-null-terminated-result,
  -bugprone-posix-return,
  -bugprone-signal-handler,
  -bugprone-spuriously-wake-up-functions,
  -bugprone-virtual-near-miss,
  -bugprone-unhandled-exception-at-new,
  -bugprone-unchecked-optional-access,
  -cert-con36-c,
  -cert-con54-cpp,
  -hicpp-uppercase-literal-suffix,
  -readability-uppercase-literal-suffix,
  -cert-dcl16-c,
  -cert-dcl21-cpp,
  -cert-dcl58-cpp,
  -cert-env33-c,
  -cert-pos44-c,
  -cert-pos47-c,
  -cert-sig30-c,
  -cert-msc30-c,
  -cert-msc50-cpp,
  -cert-msc32-c,
  -cert-msc51-cpp,
  -cppcoreguidelines-pro-bounds-constant-array-index,
  -fuchsia-multiple-inheritance,
  -fuchsia-statically-constructed-objects,
  -fuchsia-trailing-return,
  -fuchsia-virtual-inheritance,
  -google-build-explicit-make-pair,
  -google-build-using-namespace,
  -google-default-arguments,
  -google-readability-todo,
  -hicpp-named-parameter,
  -readability-named-parameter,
  -llvm-prefer-isa-or-dyn-cast-in-conditionals,
  -llvm-prefer-register-over-unsigned,
  -llvm-qualified-auto,
  -llvm-twine-local,
  -misc-confusable-identifiers,
  -misc-misleading-bidirectional,
  -misc-misleading-identifier,
  -misc-no-recursion,
  -misc-unused-parameters,
  -modernize-replace-disallow-copy-and-assign-macro,
  -modernize-use-nodiscard,
  -performance-inefficient-string-concatenation,
  -readability-isolate-declaration,
  -readability-qualified-auto,
  -readability-suspicious-call-argument,
  -hicpp-use-nullptr,
  -modernize-use-nullptr,
  -cppcoreguidelines-avoid-do-while,
  -misc-const-correctness,
  -readability-redundant-control-flow,
  -google-readability-casting,
  -cppcoreguidelines-pro-type-const-cast,
  -cppcoreguidelines-pro-type-cstyle-cast,
  -readability-container-size-empty,
  -cppcoreguidelines-init-variables,
  -cppcoreguidelines-non-private-member-variables-in-classes,
  -misc-non-private-member-variables-in-classes,
  -readability-avoid-const-params-in-decls,
  -modernize-return-braced-init-list,
  -readability-simplify-boolean-expr,
  -hicpp-signed-bitwise,
  -readability-function-cognitive-complexity,
  -readability-static-accessed-through-instance,
  -modernize-loop-convert,
  -llvm-else-after-return,
  -misc-include-cleaner,
  -readability-convert-member-functions-to-static,
  -hicpp-avoid-c-arrays,
  -hicpp-explicit-conversions,
  -hicpp-special-member-functions,
  -hicpp-noexcept-move,
  -cppcoreguidelines-noexcept-move-operations,
  -cppcoreguidelines-avoid-c-arrays,
  -bugprone-empty-catch,
  -hicpp-member-init,
  -modernize-use-constraints,
  -performance-enum-size,
  -readability-redundant-inline-specifier,
  -readability-redundant-member-init,
  -readability-avoid-return-with-void-value,
  -readability-avoid-nested-conditional-operator,
  -modernize-use-std-numbers,
  -readability-redundant-casting,
  -bugprone-suspicious-include,
  -hicpp-use-emplace,
  -boost-use-ranges,
  -modernize-use-designated-initializers,
  -readability-math-missing-parentheses,
  -modernize-min-max-use-initializer-list,
  -readability-use-std-min-max,
  -modernize-use-ranges,
  -bugprone-crtp-constructor-accessibility,
  -bugprone-suspicious-stringview-data-usage,
  -misc-use-internal-linkage
FormatStyle: "file"
CheckOptions:
  misc-non-private-member-variables-in-classes.IgnoreClassesWithAllMemberVariablesBeingPublic: "true"
  cppcoreguidelines-special-member-functions.AllowSoleDefaultDtor: "true"
```

e.g. (3)

```
[build] Error running '/usr/bin/clang-tidy': PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
[build] Stack dump:
[build] 0.	Program arguments: /usr/bin/clang-tidy -p /home/dev/snip/out/build/clang-tidy -header-filter=/home/dev/snip -warnings-as-errors=* --extra-arg-before=--driver-mode=g++ /home/dev/snip/.cpp
[build] 1.	&lt;eof&gt; parser at end of file
[build] 2.	ASTMatcher: Matching 'hicpp-invalid-access-moved' against:
[build] 	ImplicitCastExpr : &lt;/home/dev/snip/out/build/clang-tidy/vcpkg_installed/x64-linux/include/openssl/core_dispatch.h:591:1 &lt;Spelling=line:52:41&gt;&gt;
[build]  #<!-- -->0 0x00007f977cdd63bf llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) build-llvm/tools/clang/stage2-bins/llvm/lib/Support/Unix/Signals.inc:723:13
[build]  #<!-- -->1 0x00007f977cdd44f9 llvm::sys::RunSignalHandlers() build-llvm/tools/clang/stage2-bins/llvm/lib/Support/Signals.cpp:106:18
[build]  #<!-- -->2 0x00007f977cdd6b00 SignalHandler build-llvm/tools/clang/stage2-bins/llvm/lib/Support/Unix/Signals.inc:413:1
[build]  #<!-- -->3 0x00007f977baf7320 (/lib/x86_64-linux-gnu/libc.so.6+0x45320)
[build]  #<!-- -->4 0x00007f977bb5d0d4 _int_free_create_chunk ./malloc/malloc.c:4722:2
[build]  #<!-- -->5 0x00007f977bb5d0d4 _int_free_merge_chunk ./malloc/malloc.c:4700:10
[build]  #<!-- -->6 0x00007f977bb5fd9e cfree ./malloc/malloc.c:3401:3
[build]  #<!-- -->7 0x000056495dde0965 _M_erase /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:0:4
[build]  #<!-- -->8 0x000056495dde0959 _S_left /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:782:45
[build]  #<!-- -->9 0x000056495dde0959 _M_erase /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:1937:21
[build] #<!-- -->10 0x000056495dde0959 _S_left /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:782:45
[build] #<!-- -->11 0x000056495dde0959 _M_erase /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:1937:21
[build] #<!-- -->12 0x000056495dde0959 _S_left /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:782:45
[build] #<!-- -->13 0x000056495dde0959 _M_erase /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:1937:21
[build] #<!-- -->14 0x000056495dde0959 _S_left /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:782:45
[build] #<!-- -->15 0x000056495dde0959 _M_erase /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:1937:21
[build] #<!-- -->16 0x000056495dde0959 _S_left /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:782:45
[build] #<!-- -->17 0x000056495dde0959 _M_erase /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:1937:21
[build] #<!-- -->18 0x000056495dde0959 _S_left /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:782:45
[build] #<!-- -->19 0x000056495dde0959 _M_erase /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:1937:21
[build] #<!-- -->20 0x000056495dddee4c clear /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:0:2
[build] #<!-- -->21 0x000056495dddee4c clear /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_map.h:1184:14
[build] #<!-- -->22 0x000056495dddee4c matchesChildOf build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:675:19
[build] #<!-- -->23 0x000056495d94c1d2 matches build-llvm/tools/clang/stage2-bins/clang/include/clang/ASTMatchers/ASTMatchersInternal.h:1623:5
[build] #<!-- -->24 0x000056495de3d76e clang::ast_matchers::internal::DynTypedMatcher::matchesNoKindCheck(clang::DynTypedNode const&amp;, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchersInternal.cpp:332:23
[build] #<!-- -->25 0x000056495de4066a __find_if&lt;const clang::ast_matchers::internal::DynTypedMatcher *, __gnu_cxx::__ops::_Iter_negate&lt;(lambda at clang/lib/ASTMatchers/ASTMatchersInternal.cpp:394:38)&gt; &gt; /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_algobase.h:2092:8
[build] #<!-- -->26 0x000056495de4066a __find_if_not&lt;const clang::ast_matchers::internal::DynTypedMatcher *, __gnu_cxx::__ops::_Iter_pred&lt;(lambda at clang/lib/ASTMatchers/ASTMatchersInternal.cpp:394:38)&gt; &gt; /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_algo.h:112:14
[build] #<!-- -->27 0x000056495de4066a find_if_not&lt;const clang::ast_matchers::internal::DynTypedMatcher *, (lambda at clang/lib/ASTMatchers/ASTMatchersInternal.cpp:394:38)&gt; /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_algo.h:519:14
[build] #<!-- -->28 0x000056495de4066a all_of&lt;const clang::ast_matchers::internal::DynTypedMatcher *, (lambda at clang/lib/ASTMatchers/ASTMatchersInternal.cpp:394:38)&gt; /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_algo.h:459:24
[build] #<!-- -->29 0x000056495de4066a all_of&lt;llvm::ArrayRef&lt;clang::ast_matchers::internal::DynTypedMatcher&gt; &amp;, (lambda at clang/lib/ASTMatchers/ASTMatchersInternal.cpp:394:38)&gt; build-llvm/tools/clang/stage2-bins/llvm/include/llvm/ADT/STLExtras.h:1732:10
[build] #<!-- -->30 0x000056495de4066a allOfVariadicOperator build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchersInternal.cpp:394:10
[build] #<!-- -->31 0x000056495de4066a dynMatches build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchersInternal.cpp:123:12
[build] #<!-- -->32 0x000056495de3fc85 dynMatches build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchersInternal.cpp:0:33
[build] #<!-- -->33 0x000056495de3d62c clang::ast_matchers::internal::DynTypedMatcher::matches(clang::DynTypedNode const&amp;, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchersInternal.cpp:304:23
[build] #<!-- -->34 0x000056495d7f4641 matches build-llvm/tools/clang/stage2-bins/clang/include/clang/ASTMatchers/ASTMatchersInternal.h:1514:5
[build] #<!-- -->35 0x000056495de3d62c clang::ast_matchers::internal::DynTypedMatcher::matches(clang::DynTypedNode const&amp;, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchersInternal.cpp:304:23
[build] #<!-- -->36 0x000056495de0960e matchWithFilter build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:1064:11
[build] #<!-- -->37 0x000056495de212f1 TraverseStmt build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:1483:48
[build] #<!-- -->38 0x000056495de212f1 TraverseMemberExpr build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:2437:1
[build] #<!-- -->39 0x000056495de1c172 TraverseStmt build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:678:7
[build] #<!-- -->40 0x000056495de3bfa2 ~ASTNodeNotSpelledInSourceScope build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:971:47
[build] #<!-- -->41 0x000056495de3bfa2 TraverseStmt build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:1484:1
[build] #<!-- -->42 0x000056495de3bfa2 TraverseFunctionHelper build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:2198:5
[build] #<!-- -->43 0x000056495de0f586 TraverseFunctionDecl build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:2209:1
[build] #<!-- -->44 0x000056495de0c66f TraverseDecl build-llvm/tools/clang/stage2-bins/build-llvm/tools/clang/stage2-bins/tools/clang/include/clang/AST/DeclNodes.inc:238:1
[build] #<!-- -->45 0x000056495de0c66f TraverseDecl build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:1471:48
[build] #<!-- -->46 0x000056495de0cd7b TraverseDeclContextHelper build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:1480:7
[build] #<!-- -->47 0x000056495de0cd7b TraverseLinkageSpecDecl build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:1568:1
[build] #<!-- -->48 0x000056495de0c5df TraverseDecl build-llvm/tools/clang/stage2-bins/build-llvm/tools/clang/stage2-bins/tools/clang/include/clang/AST/DeclNodes.inc:36:1
[build] #<!-- -->49 0x000056495de0c5df TraverseDecl build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:1471:48
[build] #<!-- -->50 0x000056495de396eb TraverseDeclContextHelper build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:1480:7
[build] #<!-- -->51 0x000056495de0cb89 TraverseTranslationUnitDecl build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:1580:1
[build] #<!-- -->52 0x000056495de0c213 TraverseDecl build-llvm/tools/clang/stage2-bins/build-llvm/tools/clang/stage2-bins/tools/clang/include/clang/AST/DeclNodes.inc:24:1
[build] #<!-- -->53 0x000056495de0c213 TraverseDecl build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:1471:48
[build] #<!-- -->54 0x000056495ddde825 onEndOfTranslationUnit build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:446:30
[build] #<!-- -->55 0x000056495ddde825 clang::ast_matchers::MatchFinder::matchAST(clang::ASTContext&amp;) build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:1700:11
[build] #<!-- -->56 0x00007f978607dfbc operator++ /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_iterator.h:1111:2
[build] #<!-- -->57 0x00007f978607dfbc clang::MultiplexConsumer::HandleTranslationUnit(clang::ASTContext&amp;) build-llvm/tools/clang/stage2-bins/clang/lib/Frontend/MultiplexConsumer.cpp:297:23
[build] #<!-- -->58 0x00007f97841d23d6 __normal_iterator /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_iterator.h:1077:20
[build] #<!-- -->59 0x00007f97841d23d6 begin /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_vector.h:874:16
[build] #<!-- -->60 0x00007f97841d23d6 finalize&lt;std::vector&lt;std::unique_ptr&lt;clang::TemplateInstantiationCallback, std::default_delete&lt;clang::TemplateInstantiationCallback&gt; &gt;, std::allocator&lt;std::unique_ptr&lt;clang::TemplateInstantiationCallback, std::default_delete&lt;clang::TemplateInstantiationCallback&gt; &gt; &gt; &gt; &gt; build-llvm/tools/clang/stage2-bins/clang/include/clang/Sema/TemplateInstCallback.h:54:16
[build] #<!-- -->61 0x00007f97841d23d6 clang::ParseAST(clang::Sema&amp;, bool, bool) build-llvm/tools/clang/stage2-bins/clang/lib/Parse/ParseAST.cpp:183:3
[build] #<!-- -->62 0x00007f978604162c clang::FrontendAction::Execute() build-llvm/tools/clang/stage2-bins/clang/lib/Frontend/FrontendAction.cpp:1073:10
[build] #<!-- -->63 0x00007f9785fbe0b4 getPtr build-llvm/tools/clang/stage2-bins/llvm/include/llvm/Support/Error.h:279:12
[build] #<!-- -->64 0x00007f9785fbe0b4 operator bool build-llvm/tools/clang/stage2-bins/llvm/include/llvm/Support/Error.h:239:16
[build] #<!-- -->65 0x00007f9785fbe0b4 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) build-llvm/tools/clang/stage2-bins/clang/lib/Frontend/CompilerInstance.cpp:1057:23
[build] #<!-- -->66 0x00007f97862356b9 clang::tooling::FrontendActionFactory::runInvocation(std::shared_ptr&lt;clang::CompilerInvocation&gt;, clang::FileManager*, std::shared_ptr&lt;clang::PCHContainerOperations&gt;, clang::DiagnosticConsumer*) build-llvm/tools/clang/stage2-bins/clang/lib/Tooling/Tooling.cpp:465:33
[build] #<!-- -->67 0x000056495e45dab3 ~__shared_count /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/shared_ptr_base.h:1070:6
[build] #<!-- -->68 0x000056495e45dab3 ~__shared_ptr /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/shared_ptr_base.h:1524:31
[build] #<!-- -->69 0x000056495e45dab3 runInvocation build-llvm/tools/clang/stage2-bins/clang-tools-extra/clang-tidy/ClangTidy.cpp:581:7
[build] #<!-- -->70 0x00007f9786235414 ~__shared_count /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/shared_ptr_base.h:1070:6
[build] #<!-- -->71 0x00007f9786235414 ~__shared_ptr /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/shared_ptr_base.h:1524:31
[build] #<!-- -->72 0x00007f9786235414 clang::tooling::ToolInvocation::runInvocation(char const*, clang::driver::Compilation*, std::shared_ptr&lt;clang::CompilerInvocation&gt;, std::shared_ptr&lt;clang::PCHContainerOperations&gt;) build-llvm/tools/clang/stage2-bins/clang/lib/Tooling/Tooling.cpp:440:3
[build] #<!-- -->73 0x00007f978623417f clang::tooling::ToolInvocation::run() build-llvm/tools/clang/stage2-bins/clang/lib/Tooling/Tooling.cpp:425:10
[build] #<!-- -->74 0x00007f97862371c1 clang::tooling::ClangTool::run(clang::tooling::ToolAction*) build-llvm/tools/clang/stage2-bins/clang/lib/Tooling/Tooling.cpp:623:11
[build] #<!-- -->75 0x000056495e459d9b clang::tidy::runClangTidy(clang::tidy::ClangTidyContext&amp;, clang::tooling::CompilationDatabase const&amp;, llvm::ArrayRef&lt;std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;&gt;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::OverlayFileSystem&gt;, bool, bool, llvm::StringRef) build-llvm/tools/clang/stage2-bins/clang-tools-extra/clang-tidy/ClangTidy.cpp:0:8
[build] #<!-- -->76 0x000056495d7d9c65 release build-llvm/tools/clang/stage2-bins/llvm/include/llvm/ADT/IntrusiveRefCntPtr.h:232:9
[build] #<!-- -->77 0x000056495d7d9c65 ~IntrusiveRefCntPtr build-llvm/tools/clang/stage2-bins/llvm/include/llvm/ADT/IntrusiveRefCntPtr.h:196:27
[build] #<!-- -->78 0x000056495d7d9c65 clang::tidy::clangTidyMain(int, char const**) build-llvm/tools/clang/stage2-bins/clang-tools-extra/clang-tidy/tool/ClangTidyMain.cpp:681:7
[build] #<!-- -->79 0x00007f977badc1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
[build] #<!-- -->80 0x00007f977badc28b call_init ./csu/../csu/libc-start.c:128:20
[build] #<!-- -->81 0x00007f977badc28b __libc_start_main ./csu/../csu/libc-start.c:347:5
[build] #<!-- -->82 0x000056495d7d4fe5 _start (/usr/bin/clang-tidy+0x6fafe5)
[build] Segmentation fault
```
e.g. (4)

```
[build] 0.	Program arguments: /usr/bin/clang-tidy -p /home/dev/snip/out/build/clang-tidy -header-filter=/home/dev/snip -warnings-as-errors=* --extra-arg-before=--driver-mode=g++ /home/dev/snip.cpp
[build] 1.	&lt;eof&gt; parser at end of file
[build] 2.	ASTMatcher: Matching 'modernize-make-unique' against:
[build] 	CXXMemberCallExpr : &lt;/usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/basic_string.h:1354:20, col:31&gt;
[build]  #<!-- -->0 0x00007f881db1c3bf llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) build-llvm/tools/clang/stage2-bins/llvm/lib/Support/Unix/Signals.inc:723:13
[build]  #<!-- -->1 0x00007f881db1a4f9 llvm::sys::RunSignalHandlers() build-llvm/tools/clang/stage2-bins/llvm/lib/Support/Signals.cpp:106:18
[build]  #<!-- -->2 0x00007f881db1cb00 SignalHandler build-llvm/tools/clang/stage2-bins/llvm/lib/Support/Unix/Signals.inc:413:1
[build]  #<!-- -->3 0x00007f881c83d320 (/lib/x86_64-linux-gnu/libc.so.6+0x45320)
[build]  #<!-- -->4 0x00007f881c8a3027 _int_free_create_chunk ./malloc/malloc.c:4720:10
[build]  #<!-- -->5 0x00007f881c8a3027 _int_free_merge_chunk ./malloc/malloc.c:4700:10
[build]  #<!-- -->6 0x00007f881c8a5d9e cfree ./malloc/malloc.c:3401:3
[build]  #<!-- -->7 0x000055b4c3786965 _M_erase /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:0:4
[build]  #<!-- -->8 0x000055b4c3786959 _S_left /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:782:45
[build]  #<!-- -->9 0x000055b4c3786959 _M_erase /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:1937:21
[build] #<!-- -->10 0x000055b4c3786959 _S_left /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:782:45
[build] #<!-- -->11 0x000055b4c3786959 _M_erase /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:1937:21
[build] #<!-- -->12 0x000055b4c3786959 _S_left /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:782:45
[build] #<!-- -->13 0x000055b4c3786959 _M_erase /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:1937:21
[build] #<!-- -->14 0x000055b4c3786959 _S_left /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:782:45
[build] #<!-- -->15 0x000055b4c3786959 _M_erase /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:1937:21
[build] #<!-- -->16 0x000055b4c3786959 _S_left /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:782:45
[build] #<!-- -->17 0x000055b4c3786959 _M_erase /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:1937:21
[build] #<!-- -->18 0x000055b4c3784fb4 clear /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_tree.h:0:2
[build] #<!-- -->19 0x000055b4c3784fb4 clear /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_map.h:1184:14
[build] #<!-- -->20 0x000055b4c3784fb4 matchesAncestorOf build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:696:19
[build] #<!-- -->21 0x000055b4c31994bf matches build-llvm/tools/clang/stage2-bins/clang/include/clang/ASTMatchers/ASTMatchersInternal.h:1697:5
[build] #<!-- -->22 0x000055b4c37e362c clang::ast_matchers::internal::DynTypedMatcher::matches(clang::DynTypedNode const&amp;, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchersInternal.cpp:304:23
[build] #<!-- -->23 0x000055b4c37e7045 notUnaryOperator build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchersInternal.cpp:384:28
[build] #<!-- -->24 0x000055b4c37e7045 dynMatches build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchersInternal.cpp:123:12
[build] #<!-- -->25 0x000055b4c37e376e clang::ast_matchers::internal::DynTypedMatcher::matchesNoKindCheck(clang::DynTypedNode const&amp;, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchersInternal.cpp:332:23
[build] #<!-- -->26 0x000055b4c37e6585 __find_if&lt;const clang::ast_matchers::internal::DynTypedMatcher *, __gnu_cxx::__ops::_Iter_negate&lt;(lambda at clang/lib/ASTMatchers/ASTMatchersInternal.cpp:394:38)&gt; &gt; /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_algobase.h:2072:8
[build] #<!-- -->27 0x000055b4c37e6585 __find_if_not&lt;const clang::ast_matchers::internal::DynTypedMatcher *, __gnu_cxx::__ops::_Iter_pred&lt;(lambda at clang/lib/ASTMatchers/ASTMatchersInternal.cpp:394:38)&gt; &gt; /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_algo.h:112:14
[build] #<!-- -->28 0x000055b4c37e6585 find_if_not&lt;const clang::ast_matchers::internal::DynTypedMatcher *, (lambda at clang/lib/ASTMatchers/ASTMatchersInternal.cpp:394:38)&gt; /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_algo.h:519:14
[build] #<!-- -->29 0x000055b4c37e6585 all_of&lt;const clang::ast_matchers::internal::DynTypedMatcher *, (lambda at clang/lib/ASTMatchers/ASTMatchersInternal.cpp:394:38)&gt; /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_algo.h:459:24
[build] #<!-- -->30 0x000055b4c37e6585 all_of&lt;llvm::ArrayRef&lt;clang::ast_matchers::internal::DynTypedMatcher&gt; &amp;, (lambda at clang/lib/ASTMatchers/ASTMatchersInternal.cpp:394:38)&gt; build-llvm/tools/clang/stage2-bins/llvm/include/llvm/ADT/STLExtras.h:1732:10
[build] #<!-- -->31 0x000055b4c37e6585 allOfVariadicOperator build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchersInternal.cpp:394:10
[build] #<!-- -->32 0x000055b4c37e6585 dynMatches build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchersInternal.cpp:123:12
[build] #<!-- -->33 0x000055b4c37e5c85 dynMatches build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchersInternal.cpp:0:33
[build] #<!-- -->34 0x000055b4c37e362c clang::ast_matchers::internal::DynTypedMatcher::matches(clang::DynTypedNode const&amp;, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchersInternal.cpp:304:23
[build] #<!-- -->35 0x000055b4c319a641 matches build-llvm/tools/clang/stage2-bins/clang/include/clang/ASTMatchers/ASTMatchersInternal.h:1514:5
[build] #<!-- -->36 0x000055b4c37e362c clang::ast_matchers::internal::DynTypedMatcher::matches(clang::DynTypedNode const&amp;, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchersInternal.cpp:304:23
[build] #<!-- -->37 0x000055b4c37af60e matchWithFilter build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:1064:11
[build] #<!-- -->38 0x000055b4c37ce4a1 TraverseStmt build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:1483:48
[build] #<!-- -->39 0x000055b4c37ce4a1 TraverseBinaryOperator build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:2852:1
[build] #<!-- -->40 0x000055b4c37c1b0c TraverseStmt build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:678:7
[build] #<!-- -->41 0x000055b4c37e1fa2 ~ASTNodeNotSpelledInSourceScope build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:971:47
[build] #<!-- -->42 0x000055b4c37e1fa2 TraverseStmt build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:1484:1
[build] #<!-- -->43 0x000055b4c37e1fa2 TraverseFunctionHelper build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:2198:5
[build] #<!-- -->44 0x000055b4c37b5636 TraverseCXXMethodDecl build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:2223:1
[build] #<!-- -->45 0x000055b4c37b27ef TraverseDecl build-llvm/tools/clang/stage2-bins/build-llvm/tools/clang/stage2-bins/tools/clang/include/clang/AST/DeclNodes.inc:242:1
[build] #<!-- -->46 0x000055b4c37b27ef TraverseDecl build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:1471:48
[build] #<!-- -->47 0x000055b4c37b799b TraverseDeclContextHelper build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:1480:7
[build] #<!-- -->48 0x000055b4c37b799b TraverseClassTemplateSpecializationDecl build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:2026:1
[build] #<!-- -->49 0x000055b4c37b287f TraverseDecl build-llvm/tools/clang/stage2-bins/build-llvm/tools/clang/stage2-bins/tools/clang/include/clang/AST/DeclNodes.inc:418:1
[build] #<!-- -->50 0x000055b4c37b287f TraverseDecl build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:1471:48
[build] #<!-- -->51 0x000055b4c37b9252 TraverseTemplateInstantiations build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:1791:9
[build] #<!-- -->52 0x000055b4c37b9252 TraverseTemplateInstantiations build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:759:50
[build] #<!-- -->53 0x000055b4c37b9252 TraverseClassTemplateDecl build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:1883:1
[build] #<!-- -->54 0x000055b4c37b23cf TraverseDecl build-llvm/tools/clang/stage2-bins/build-llvm/tools/clang/stage2-bins/tools/clang/include/clang/AST/DeclNodes.inc:524:1
[build] #<!-- -->55 0x000055b4c37b23cf TraverseDecl build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:1471:48
[build] #<!-- -->56 0x000055b4c37b496b TraverseDeclContextHelper build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:1480:7
[build] #<!-- -->57 0x000055b4c37b496b TraverseNamespaceDecl build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:1616:1
[build] #<!-- -->58 0x000055b4c37b2747 TraverseDecl build-llvm/tools/clang/stage2-bins/build-llvm/tools/clang/stage2-bins/tools/clang/include/clang/AST/DeclNodes.inc:170:1
[build] #<!-- -->59 0x000055b4c37b2747 TraverseDecl build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:1471:48
[build] #<!-- -->60 0x000055b4c37b496b TraverseDeclContextHelper build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:1480:7
[build] #<!-- -->61 0x000055b4c37b496b TraverseNamespaceDecl build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:1616:1
[build] #<!-- -->62 0x000055b4c37b2747 TraverseDecl build-llvm/tools/clang/stage2-bins/build-llvm/tools/clang/stage2-bins/tools/clang/include/clang/AST/DeclNodes.inc:170:1
[build] #<!-- -->63 0x000055b4c37b2747 TraverseDecl build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:1471:48
[build] #<!-- -->64 0x000055b4c37df6eb TraverseDeclContextHelper build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:1480:7
[build] #<!-- -->65 0x000055b4c37b2b89 TraverseTranslationUnitDecl build-llvm/tools/clang/stage2-bins/clang/include/clang/AST/RecursiveASTVisitor.h:1580:1
[build] #<!-- -->66 0x000055b4c37b2213 TraverseDecl build-llvm/tools/clang/stage2-bins/build-llvm/tools/clang/stage2-bins/tools/clang/include/clang/AST/DeclNodes.inc:24:1
[build] #<!-- -->67 0x000055b4c37b2213 TraverseDecl build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:1471:48
[build] #<!-- -->68 0x000055b4c3784825 onEndOfTranslationUnit build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:446:30
[build] #<!-- -->69 0x000055b4c3784825 clang::ast_matchers::MatchFinder::matchAST(clang::ASTContext&amp;) build-llvm/tools/clang/stage2-bins/clang/lib/ASTMatchers/ASTMatchFinder.cpp:1700:11
[build] #<!-- -->70 0x00007f8826dc3fbc operator++ /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_iterator.h:1111:2
[build] #<!-- -->71 0x00007f8826dc3fbc clang::MultiplexConsumer::HandleTranslationUnit(clang::ASTContext&amp;) build-llvm/tools/clang/stage2-bins/clang/lib/Frontend/MultiplexConsumer.cpp:297:23
[build] #<!-- -->72 0x00007f8824f183d6 __normal_iterator /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_iterator.h:1077:20
[build] #<!-- -->73 0x00007f8824f183d6 begin /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stl_vector.h:874:16
[build] #<!-- -->74 0x00007f8824f183d6 finalize&lt;std::vector&lt;std::unique_ptr&lt;clang::TemplateInstantiationCallback, std::default_delete&lt;clang::TemplateInstantiationCallback&gt; &gt;, std::allocator&lt;std::unique_ptr&lt;clang::TemplateInstantiationCallback, std::default_delete&lt;clang::TemplateInstantiationCallback&gt; &gt; &gt; &gt; &gt; build-llvm/tools/clang/stage2-bins/clang/include/clang/Sema/TemplateInstCallback.h:54:16
[build] #<!-- -->75 0x00007f8824f183d6 clang::ParseAST(clang::Sema&amp;, bool, bool) build-llvm/tools/clang/stage2-bins/clang/lib/Parse/ParseAST.cpp:183:3
[build] #<!-- -->76 0x00007f8826d8762c clang::FrontendAction::Execute() build-llvm/tools/clang/stage2-bins/clang/lib/Frontend/FrontendAction.cpp:1073:10
[build] #<!-- -->77 0x00007f8826d040b4 getPtr build-llvm/tools/clang/stage2-bins/llvm/include/llvm/Support/Error.h:279:12
[build] #<!-- -->78 0x00007f8826d040b4 operator bool build-llvm/tools/clang/stage2-bins/llvm/include/llvm/Support/Error.h:239:16
[build] #<!-- -->79 0x00007f8826d040b4 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) build-llvm/tools/clang/stage2-bins/clang/lib/Frontend/CompilerInstance.cpp:1057:23
[build] #<!-- -->80 0x00007f8826f7b6b9 clang::tooling::FrontendActionFactory::runInvocation(std::shared_ptr&lt;clang::CompilerInvocation&gt;, clang::FileManager*, std::shared_ptr&lt;clang::PCHContainerOperations&gt;, clang::DiagnosticConsumer*) build-llvm/tools/clang/stage2-bins/clang/lib/Tooling/Tooling.cpp:465:33
[build] #<!-- -->81 0x000055b4c3e03ab3 ~__shared_count /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/shared_ptr_base.h:1070:6
[build] #<!-- -->82 0x000055b4c3e03ab3 ~__shared_ptr /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/shared_ptr_base.h:1524:31
[build] #<!-- -->83 0x000055b4c3e03ab3 runInvocation build-llvm/tools/clang/stage2-bins/clang-tools-extra/clang-tidy/ClangTidy.cpp:581:7
[build] #<!-- -->84 0x00007f8826f7b414 ~__shared_count /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/shared_ptr_base.h:1070:6
[build] #<!-- -->85 0x00007f8826f7b414 ~__shared_ptr /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/shared_ptr_base.h:1524:31
[build] #<!-- -->86 0x00007f8826f7b414 clang::tooling::ToolInvocation::runInvocation(char const*, clang::driver::Compilation*, std::shared_ptr&lt;clang::CompilerInvocation&gt;, std::shared_ptr&lt;clang::PCHContainerOperations&gt;) build-llvm/tools/clang/stage2-bins/clang/lib/Tooling/Tooling.cpp:440:3
[build] #<!-- -->87 0x00007f8826f7a17f clang::tooling::ToolInvocation::run() build-llvm/tools/clang/stage2-bins/clang/lib/Tooling/Tooling.cpp:425:10
[build] #<!-- -->88 0x00007f8826f7d1c1 clang::tooling::ClangTool::run(clang::tooling::ToolAction*) build-llvm/tools/clang/stage2-bins/clang/lib/Tooling/Tooling.cpp:623:11
[build] #<!-- -->89 0x000055b4c3dffd9b clang::tidy::runClangTidy(clang::tidy::ClangTidyContext&amp;, clang::tooling::CompilationDatabase const&amp;, llvm::ArrayRef&lt;std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;&gt;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::OverlayFileSystem&gt;, bool, bool, llvm::StringRef) build-llvm/tools/clang/stage2-bins/clang-tools-extra/clang-tidy/ClangTidy.cpp:0:8
[build] #<!-- -->90 0x000055b4c317fc65 release build-llvm/tools/clang/stage2-bins/llvm/include/llvm/ADT/IntrusiveRefCntPtr.h:232:9
[build] #<!-- -->91 0x000055b4c317fc65 ~IntrusiveRefCntPtr build-llvm/tools/clang/stage2-bins/llvm/include/llvm/ADT/IntrusiveRefCntPtr.h:196:27
[build] #<!-- -->92 0x000055b4c317fc65 clang::tidy::clangTidyMain(int, char const**) build-llvm/tools/clang/stage2-bins/clang-tools-extra/clang-tidy/tool/ClangTidyMain.cpp:681:7
[build] #<!-- -->93 0x00007f881c8221ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
[build] #<!-- -->94 0x00007f881c82228b call_init ./csu/../csu/libc-start.c:128:20
[build] #<!-- -->95 0x00007f881c82228b __libc_start_main ./csu/../csu/libc-start.c:347:5
[build] #<!-- -->96 0x000055b4c317afe5 _start (/usr/bin/clang-tidy+0x6fafe5)
[build] Segmentation fault
```

Command line to invoke
```
[build] /usr/bin/cmake -E __run_co_compile --launcher="/usr/bin/cmake;-E;env;CCACHE_SLOPPINESS=pch_defines,time_macros,include_file_mtime,include_file_ctime;/usr/local/bin/ccache" --tidy="/usr/bin/clang-tidy;-p;/home/dev/snip/out/build/clang-tidy;-header-filter=/home/dev/snip;-warnings-as-errors=*;--extra-arg-before=--driver-mode=g++" --source=/home/dev/snip/src/lib/snip.cpp -- /usr/bin/clang++ -DBOOST_CONTAINER_NO_LIB -DBOOST_CONTAINER_STATIC_LINK -DBOOST_CONTEXT_EXPORT="" -DBOOST_CONTEXT_NO_LIB="" -DBOOST_CONTEXT_STATIC_LINK="" -DBOOST_COROUTINE_NO_LIB -DBOOST_COROUTINE_STATIC_LINK -DBOOST_DATE_TIME_NO_LIB -DBOOST_DATE_TIME_STATIC_LINK -DSPDLOG_COMPILED_LIB -DSPDLOG_FMT_EXTERNAL -I/home/dev/snip/src/lib -I/home/dev/snip/src -I/home/dev/snip/out/build/clang-tidy/src/snip/pb -isystem /home/dev/snip/out/build/clang-tidy/vcpkg_installed/x64-linux/include -O3 -DNDEBUG -std=c++20 -fPIC -Wall -Wextra -Xclang -fno-pch-timestamp -Werror -MD -MT src/lib/snip.cpp.o -MF src/lib/snip.o.d -o src/snip.cpp.o -c /home/dev/snip.cpp
```

Sorry, this is commercial code, unable to provide the whole thing, but this is compiling against boost redis. Here are the main 3rd party includes

```c++
#include &lt;boost/asio/co_spawn.hpp&gt;
#include &lt;boost/asio/consign.hpp&gt;
#include &lt;boost/asio/deferred.hpp&gt;
#include &lt;boost/asio/detached.hpp&gt;
#include &lt;boost/asio/io_context.hpp&gt;
#include &lt;boost/asio/redirect_error.hpp&gt;
#include &lt;boost/asio/signal_set.hpp&gt;
#include &lt;boost/asio/use_awaitable.hpp&gt;
#include &lt;boost/asio/use_future.hpp&gt;
#include &lt;boost/redis/config.hpp&gt;
#include &lt;boost/redis/connection.hpp&gt;
#include &lt;chrono&gt;
#include &lt;nlohmann/json.hpp&gt;
#include &lt;memory&gt;
#include &lt;numeric&gt;
#include &lt;ranges&gt;
#include &lt;thread&gt;
#include &lt;tl/to.hpp&gt;
````

</details>


---

### Comment 3 - zwuis

Could you please try 20 or `main` branch?

---

### Comment 4 - tonygould

will do, I will need to spin up a new container, rebuild the dependencies etc will take the computer a little while to turn the cogs then I will get back to you

---

### Comment 5 - tonygould

It took a while to upgrade my whole project... Strangely, multiple build of the file that had failed before now succeed (5 in a row, which is inconsistent with the hypothesis that clang-tidy 20 fails on this translation unit). OTOH, I see crashes elsewhere in the code base.

For example, e.g. on a single file which is simply `#include <boost/redis/src.hpp>`. I get the below. As you can tell, this is simply including the boost redis src file that is necessary to link to  boost redis.

Apologies, it has taken me a lot  longer than expected to upgrade + get something building, and not sure I can spend a lot more time on this. I appreciate the effort that has gone into clang-tidy, and apologies in advance if I can't spend more time on this, but I hope that the stack traces might help improved the tool.

`
[1/2] Building CXX object src/lib/snip2/CMakeFiles/snip2.dir/RedisSrc.cpp.o
FAILED: src/lib/snip2/CMakeFiles/snip2.dir/RedisSrc.cpp.o 
/usr/bin/cmake -E __run_co_compile --launcher="/usr/bin/cmake;-E;env;CCACHE_SLOPPINESS=pch_defines,time_macros,include_file_mtime,include_file_ctime;/usr/local/bin/ccache" --tidy="/usr/bin/clang-tidy;-p;/home/dev/snip/out/build/clang-tidy;-header-filter=/home/dev/snip;-warnings-as-errors=*;--extra-arg-before=--driver-mode=g++" --source=/home/dev/sip/src/lib/snip2/RedisSrc.cpp -- /usr/bin/clang++ -DBOOST_CONTAINER_NO_LIB -DBOOST_CONTAINER_STATIC_LINK -DBOOST_CONTEXT_EXPORT="" -DBOOST_CONTEXT_NO_LIB="" -DBOOST_CONTEXT_STATIC_LINK="" -DBOOST_DATE_TIME_NO_LIB -DBOOST_DATE_TIME_STATIC_LINK -DSPDLOG_FWRITE_UNLOCKED -DSPDLOG_USE_STD_FORMAT -I/home/dev/snip/src/lib -I/home/dev/snip/src/interface -I/home/dev/snip/out/build/clang-tidy/src/lib/snip2/idl/pb -isystem /home/dev/snip/out/build/clang-tidy/vcpkg_installed/x64-linux/include -isystem /home/dev/snip/out/build/clang-tidy/_deps/spdlog-src/include -O3 -DNDEBUG -std=c++20 -fPIC -Wall -Wextra -Xclang -fno-pch-timestamp -Werror -MD -MT src/lib/snip2/CMakeFiles/snip2.dir/RedisSrc.cpp.o -MF src/lib/snip2/CMakeFiles/snip2.dir/RedisSrc.cpp.o.d -o src/lib/snip2/CMakeFiles/snip2.dir/RedisSrc.cpp.o -c /home/dev/snip/src/lib/snip2/RedisSrc.cpp
Error running '/usr/bin/clang-tidy': *** stack smashing detected ***: terminated
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.      Program arguments: /usr/bin/clang-tidy -p /home/dev/snip/analytics/src/lib/snip2/RedisSrc.cpp
1.      <eof> parser at end of file
2.      ASTMatcher: Matching 'misc-unused-using-decls' against:
        FunctionProtoTypeLoc : </usr/bin/../lib/gcc/x86_64-linux-gnu/14/../../../../include/c++/14/tuple:2463:21, line:2464:42>
 #0 0x00007f299091fd1f llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/usr/lib/llvm-20/bin/../lib/libLLVM.so.20.1+0x101dd1f)
 #1 0x00007f299091da29 llvm::sys::RunSignalHandlers() (/usr/lib/llvm-20/bin/../lib/libLLVM.so.20.1+0x101ba29)
 #2 0x00007f2990920430 (/usr/lib/llvm-20/bin/../lib/libLLVM.so.20.1+0x101e430)
 #3 0x00007f298f388650 (/lib/x86_64-linux-gnu/libc.so.6+0x45650)
 #4 0x00007f298f3e6f5c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x00007f298f3e6f5c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x00007f298f3e6f5c pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x00007f298f38859e raise ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x00007f298f36b902 abort ./stdlib/abort.c:81:7
 #9 0x00007f298f36c76c _IO_acquire_lock_fct ./libio/libioP.h:979:6
#10 0x00007f298f36c76c _IO_peekc_locked ./libio/peekc.c:37:3
#11 0x00007f298f4841c9 (/lib/x86_64-linux-gnu/libc.so.6+0x1411c9)
#12 0x00007f298f485444 (/lib/x86_64-linux-gnu/libc.so.6+0x142444)
#13 0x00007f2999963f95 (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x169ef95)
#14 0x00007f2999963f48 clang::ast_matchers::internal::DynTypedMatcher::matches(clang::DynTypedNode const&, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x169ef48)
#15 0x00007f299992a35f (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x166535f)
#16 0x00007f2999962449 (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x169d449)
#17 0x00007f2999930d96 (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x166bd96)
#18 0x00007f299992df91 (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x1668f91)
#19 0x00007f29999346f0 (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x166f6f0)
#20 0x00007f299992db71 (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x1668b71)
#21 0x00007f29999301eb (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x166b1eb)
#22 0x00007f299992dfd9 (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x1668fd9)
#23 0x00007f299992e3ab (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x16693ab)
#24 0x00007f299992d9b5 (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x16689b5)
#25 0x00007f29998fdd00 clang::ast_matchers::MatchFinder::matchAST(clang::ASTContext&) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x1638d00)
#26 0x00007f299aff843c clang::MultiplexConsumer::HandleTranslationUnit(clang::ASTContext&) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x2d3343c)
#27 0x00007f29990294d9 clang::ParseAST(clang::Sema&, bool, bool) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xd644d9)
#28 0x00007f299afb67f2 clang::FrontendAction::Execute() (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x2cf17f2)
#29 0x00007f299af2f744 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x2c6a744)
#30 0x00007f299b1c24dd clang::tooling::FrontendActionFactory::runInvocation(std::shared_ptr<clang::CompilerInvocation>, clang::FileManager*, std::shared_ptr<clang::PCHContainerOperations>, clang::DiagnosticConsumer*) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x2efd4dd)
#31 0x000055fb15a82fdf (/usr/bin/clang-tidy+0x90efdf)
#32 0x00007f299b1c2234 clang::tooling::ToolInvocation::runInvocation(char const*, clang::driver::Compilation*, std::shared_ptr<clang::CompilerInvocation>, std::shared_ptr<clang::PCHContainerOperations>) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x2efd234)
#33 0x00007f299b1c0ea9 clang::tooling::ToolInvocation::run() (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x2efbea9)
#34 0x00007f299b1c41eb clang::tooling::ClangTool::run(clang::tooling::ToolAction*) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x2eff1eb)
#35 0x000055fb15a7e932 clang::tidy::runClangTidy(clang::tidy::ClangTidyContext&, clang::tooling::CompilationDatabase const&, llvm::ArrayRef<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>>, llvm::IntrusiveRefCntPtr<llvm::vfs::OverlayFileSystem>, bool, bool, llvm::StringRef) (/usr/bin/clang-tidy+0x90a932)
#36 0x000055fb153d75f4 clang::tidy::clangTidyMain(int, char const**) (/usr/bin/clang-tidy+0x2635f4)
#37 0x00007f298f36d3b8 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#38 0x00007f298f36d47b call_init ./csu/../csu/libc-start.c:128:20
#39 0x00007f298f36d47b __libc_start_main ./csu/../csu/libc-start.c:347:5
#40 0x000055fb153d29c5 _start (/usr/bin/clang-tidy+0x25e9c5)
Subprocess aborted`

---

### Comment 6 - vbvictor

I tried locally on commit 74687180dde07312521db09c6f6454fe9d1e5662, and It didn't crash.

```cpp
// gh144862.cpp
#include <boost/redis/src.hpp>

int main() {}
```
`./bin/clang-tidy --checks=-*,misc-unused-using-decls gh144862.cpp`

I couldn't find any related commits of fixing this crash in `misc-unused-using-decls` using `git log`.
There were a couple of commits improving performance, maybe it was fixed somewhere there.

---

### Comment 7 - vbvictor

Oh, I could get the crush on "bugprone-crtp-constructor-accessibility" but it seems you have this rule disabled.
Commit 74687180dde07312521db09c6f6454fe9d1e5662, file:

```cpp
#include "boost/redis/src.hpp" // 1.88

int main() {}
```

crash:

```
Error while trying to load a compilation database:
Could not auto-detect compilation database for file "gh.cpp"
No compilation database found in /usr/include/boost/redis or any parent directory
fixed-compilation-database: Error while opening fixed database: No such file or directory
json-compilation-database: Error while opening JSON database: No such file or directory
Running without flags.
clang-tidy: /home/victor/llvm/llvm-project/clang/include/clang/Basic/SourceManager.h:509: const ExpansionInfo &clang::SrcMgr::SLocEntry::getExpansion() const: Assertion `isExpansion() && "Not a macro expansion SLocEntry!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.      Program arguments: /home/victor/llvm/llvm-project/build/bin/clang-tidy --checks=* --extra-arg=-fcxx-exceptions --extra-arg=-frtti gh.cpp
1.      <eof> parser at end of file
2.      ASTMatcher: Processing 'bugprone-crtp-constructor-accessibility' against:
        ClassTemplateSpecializationDecl boost::date_time::base_time : </usr/include/boost/date_time/time.hpp:45:3, line:204:3>
--- Bound Nodes Begin ---
    crtp - { ClassTemplateSpecializationDecl boost::date_time::base_time : </usr/include/boost/date_time/time.hpp:45:3, line:204:3> }
    derived - { CXXRecordDecl boost::posix_time::ptime : </usr/include/boost/date_time/posix_time/ptime.hpp:32:3, line:71:3> }
--- Bound Nodes End ---
 #0 0x00005a09b1fe658d llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /home/victor/llvm/llvm-project/llvm/lib/Support/Unix/Signals.inc:804:11
 #1 0x00005a09b1fe6acb PrintStackTraceSignalHandler(void*) /home/victor/llvm/llvm-project/llvm/lib/Support/Unix/Signals.inc:888:1
 #2 0x00005a09b1fe4bef llvm::sys::RunSignalHandlers() /home/victor/llvm/llvm-project/llvm/lib/Support/Signals.cpp:105:5
 #3 0x00005a09b1fe71a9 SignalHandler(int, siginfo_t*, void*) /home/victor/llvm/llvm-project/llvm/lib/Support/Unix/Signals.inc:418:7
 #4 0x0000787ce7245330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #5 0x0000787ce729eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #6 0x0000787ce729eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #7 0x0000787ce729eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #8 0x0000787ce724527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #9 0x0000787ce72288ff abort ./stdlib/abort.c:81:7
#10 0x0000787ce722881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#11 0x0000787ce723b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#12 0x00005a09b21f7cf8 clang::SrcMgr::SLocEntry::getExpansion() const /home/victor/llvm/llvm-project/clang/include/clang/Basic/SourceManager.h:0:5
#13 0x00005a09b2f727f1 clang::SourceManager::isMacroArgExpansion(clang::SourceLocation, clang::SourceLocation*) const /home/victor/llvm/llvm-project/clang/lib/Basic/SourceManager.cpp:1031:32
#14 0x00005a09b2f72c0d clang::SourceManager::getTopMacroCallerLoc(clang::SourceLocation) const /home/victor/llvm/llvm-project/clang/lib/Basic/SourceManager.cpp:1001:3
#15 0x00005a09b635a5d5 clang::edit::Commit::canInsert(clang::SourceLocation, clang::edit::FileOffset&) /home/victor/llvm/llvm-project/clang/lib/Edit/Commit.cpp:233:12
#16 0x00005a09b635a4cb clang::edit::Commit::insert(clang::SourceLocation, llvm::StringRef, bool, bool) /home/victor/llvm/llvm-project/clang/lib/Edit/Commit.cpp:54:46
#17 0x00005a09b4bf55ff mergeFixits(llvm::ArrayRef<clang::FixItHint>, clang::SourceManager const&, clang::LangOptions const&, llvm::SmallVectorImpl<clang::FixItHint>&) /home/victor/llvm/llvm-project/clang/lib/Frontend/DiagnosticRenderer.cpp:0:16
#18 0x00005a09b4bf4eb8 clang::DiagnosticRenderer::emitDiagnostic(clang::FullSourceLoc, clang::DiagnosticsEngine::Level, llvm::StringRef, llvm::ArrayRef<clang::CharSourceRange>, llvm::ArrayRef<clang::FixItHint>, llvm::PointerUnion<clang::Diagnostic const*, clang::StoredDiagnostic const*>) /home/victor/llvm/llvm-project/clang/lib/Frontend/DiagnosticRenderer.cpp:105:20
#19 0x00005a09b30d5504 clang::tidy::ClangTidyDiagnosticConsumer::HandleDiagnostic(clang::DiagnosticsEngine::Level, clang::Diagnostic const&) /home/victor/llvm/llvm-project/clang-tools-extra/clang-tidy/ClangTidyDiagnosticConsumer.cpp:443:3
#20 0x00005a09b2ef76dc clang::DiagnosticsEngine::Report(clang::DiagnosticsEngine::Level, clang::Diagnostic const&) /home/victor/llvm/llvm-project/clang/lib/Basic/Diagnostic.cpp:668:11
#21 0x00005a09b2ef7a1b clang::DiagnosticsEngine::ProcessDiag(clang::DiagnosticBuilder const&) /home/victor/llvm/llvm-project/clang/lib/Basic/Diagnostic.cpp:749:3
#22 0x00005a09b2ef7b29 clang::DiagnosticsEngine::EmitDiagnostic(clang::DiagnosticBuilder const&, bool) /home/victor/llvm/llvm-project/clang/lib/Basic/Diagnostic.cpp:770:13
#23 0x00005a09b20bce58 clang::DiagnosticBuilder::Emit() /home/victor/llvm/llvm-project/clang/include/clang/Basic/Diagnostic.h:1284:28
#24 0x00005a09b20bbf89 clang::DiagnosticBuilder::~DiagnosticBuilder() /home/victor/llvm/llvm-project/clang/include/clang/Basic/Diagnostic.h:1319:26
#25 0x00005a09b3350b7f clang::tidy::bugprone::CrtpConstructorAccessibilityCheck::check(clang::ast_matchers::MatchFinder::MatchResult const&) /home/victor/llvm/llvm-project/clang-tools-extra/clang-tidy/bugprone/CrtpConstructorAccessibilityCheck.cpp:174:3
#26 0x00005a09b30cfede clang::tidy::ClangTidyCheck::run(clang::ast_matchers::MatchFinder::MatchResult const&) /home/victor/llvm/llvm-project/clang-tools-extra/clang-tidy/ClangTidyCheck.cpp:47:1
#27 0x00005a09b2d6d438 clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor::MatchVisitor::visitMatch(clang::ast_matchers::BoundNodes const&) /home/victor/llvm/llvm-project/clang/lib/ASTMatchers/ASTMatchFinder.cpp:1273:7
#28 0x00005a09b2e1cd01 clang::ast_matchers::internal::BoundNodesTreeBuilder::visitMatches(clang::ast_matchers::internal::BoundNodesTreeBuilder::Visitor*) /home/victor/llvm/llvm-project/clang/lib/ASTMatchers/ASTMatchersInternal.cpp:103:5
#29 0x00005a09b2d6cfaf clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor::matchWithFilter(clang::DynTypedNode const&) /home/victor/llvm/llvm-project/clang/lib/ASTMatchers/ASTMatchFinder.cpp:1069:7
#30 0x00005a09b2d6ccb2 clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor::matchDispatch(clang::Decl const*) /home/victor/llvm/llvm-project/clang/lib/ASTMatchers/ASTMatchFinder.cpp:1088:5
#31 0x00005a09b2d6cabd void clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor::match<clang::Decl>(clang::Decl const&) /home/victor/llvm/llvm-project/clang/lib/ASTMatchers/ASTMatchFinder.cpp:733:3
#32 0x00005a09b2d6f8d4 clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor::TraverseDecl(clang::Decl*) /home/victor/llvm/llvm-project/clang/lib/ASTMatchers/ASTMatchFinder.cpp:1493:3
#33 0x00005a09b2de96a9 clang::RecursiveASTVisitor<clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor>::TraverseTemplateInstantiations(clang::ClassTemplateDecl*) /home/victor/llvm/llvm-project/clang/include/clang/AST/RecursiveASTVisitor.h:1884:9
#34 0x00005a09b2de9503 clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor::TraverseTemplateInstantiations(clang::ClassTemplateDecl*) /home/victor/llvm/llvm-project/clang/lib/ASTMatchers/ASTMatchFinder.cpp:761:50
#35 0x00005a09b2d7948e clang::RecursiveASTVisitor<clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor>::TraverseClassTemplateDecl(clang::ClassTemplateDecl*) /home/victor/llvm/llvm-project/clang/include/clang/AST/RecursiveASTVisitor.h:1976:1
#36 0x00005a09b2d70517 clang::RecursiveASTVisitor<clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor>::TraverseDecl(clang::Decl*) /home/victor/llvm/llvm-project/build/tools/clang/include/clang/AST/DeclNodes.inc:542:1
#37 0x00005a09b2d6f8e1 clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor::TraverseDecl(clang::Decl*) /home/victor/llvm/llvm-project/clang/lib/ASTMatchers/ASTMatchFinder.cpp:1494:3
#38 0x00005a09b2dcf236 clang::RecursiveASTVisitor<clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor>::TraverseDeclContextHelper(clang::DeclContext*) /home/victor/llvm/llvm-project/clang/include/clang/AST/RecursiveASTVisitor.h:1558:7
#39 0x00005a09b2d73b49 clang::RecursiveASTVisitor<clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor>::TraverseNamespaceDecl(clang::NamespaceDecl*) /home/victor/llvm/llvm-project/clang/include/clang/AST/RecursiveASTVisitor.h:1701:1
#40 0x00005a09b2d6fdc7 clang::RecursiveASTVisitor<clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor>::TraverseDecl(clang::Decl*) /home/victor/llvm/llvm-project/build/tools/clang/include/clang/AST/DeclNodes.inc:188:1
#41 0x00005a09b2d6f8e1 clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor::TraverseDecl(clang::Decl*) /home/victor/llvm/llvm-project/clang/lib/ASTMatchers/ASTMatchFinder.cpp:1494:3
#42 0x00005a09b2dcf236 clang::RecursiveASTVisitor<clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor>::TraverseDeclContextHelper(clang::DeclContext*) /home/victor/llvm/llvm-project/clang/include/clang/AST/RecursiveASTVisitor.h:1558:7
#43 0x00005a09b2d73b49 clang::RecursiveASTVisitor<clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor>::TraverseNamespaceDecl(clang::NamespaceDecl*) /home/victor/llvm/llvm-project/clang/include/clang/AST/RecursiveASTVisitor.h:1701:1
#44 0x00005a09b2d6fdc7 clang::RecursiveASTVisitor<clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor>::TraverseDecl(clang::Decl*) /home/victor/llvm/llvm-project/build/tools/clang/include/clang/AST/DeclNodes.inc:188:1
#45 0x00005a09b2d6f8e1 clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor::TraverseDecl(clang::Decl*) /home/victor/llvm/llvm-project/clang/lib/ASTMatchers/ASTMatchFinder.cpp:1494:3
#46 0x00005a09b2dcf236 clang::RecursiveASTVisitor<clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor>::TraverseDeclContextHelper(clang::DeclContext*) /home/victor/llvm/llvm-project/clang/include/clang/AST/RecursiveASTVisitor.h:1558:7
#47 0x00005a09b2d709b8 clang::RecursiveASTVisitor<clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor>::TraverseTranslationUnitDecl(clang::TranslationUnitDecl*) /home/victor/llvm/llvm-project/clang/include/clang/AST/RecursiveASTVisitor.h:1665:1
#48 0x00005a09b2d6fa1f clang::RecursiveASTVisitor<clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor>::TraverseDecl(clang::Decl*) /home/victor/llvm/llvm-project/build/tools/clang/include/clang/AST/DeclNodes.inc:24:1
#49 0x00005a09b2d6f8e1 clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor::TraverseDecl(clang::Decl*) /home/victor/llvm/llvm-project/clang/lib/ASTMatchers/ASTMatchFinder.cpp:1494:3
#50 0x00005a09b2cebd82 clang::RecursiveASTVisitor<clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor>::TraverseAST(clang::ASTContext&) /home/victor/llvm/llvm-project/clang/include/clang/AST/RecursiveASTVisitor.h:192:5
#51 0x00005a09b2cebbec clang::ast_matchers::MatchFinder::matchAST(clang::ASTContext&) /home/victor/llvm/llvm-project/clang/lib/ASTMatchers/ASTMatchFinder.cpp:1723:11
#52 0x00005a09b2d6f37e clang::ast_matchers::internal::(anonymous namespace)::MatchASTConsumer::HandleTranslationUnit(clang::ASTContext&) /home/victor/llvm/llvm-project/clang/lib/ASTMatchers/ASTMatchFinder.cpp:1586:3
#53 0x00005a09b4c4bccc clang::MultiplexConsumer::HandleTranslationUnit(clang::ASTContext&) /home/victor/llvm/llvm-project/clang/lib/Frontend/MultiplexConsumer.cpp:338:23
#54 0x00005a09b4eb708b clang::ParseAST(clang::Sema&, bool, bool) /home/victor/llvm/llvm-project/clang/lib/Parse/ParseAST.cpp:190:12
#55 0x00005a09b4c005e9 clang::ASTFrontendAction::ExecuteAction() /home/victor/llvm/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1341:1
#56 0x00005a09b4c00036 clang::FrontendAction::Execute() /home/victor/llvm/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1225:7
#57 0x00005a09b4b1e553 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) /home/victor/llvm/llvm-project/clang/lib/Frontend/CompilerInstance.cpp:1055:23
#58 0x00005a09b309cc33 clang::tooling::FrontendActionFactory::runInvocation(std::shared_ptr<clang::CompilerInvocation>, clang::FileManager*, std::shared_ptr<clang::PCHContainerOperations>, clang::DiagnosticConsumer*) /home/victor/llvm/llvm-project/clang/lib/Tooling/Tooling.cpp:463:14
#59 0x00005a09b30bae49 clang::tidy::runClangTidy(clang::tidy::ClangTidyContext&, clang::tooling::CompilationDatabase const&, llvm::ArrayRef<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>>, llvm::IntrusiveRefCntPtr<llvm::vfs::OverlayFileSystem>, bool, bool, llvm::StringRef)::ActionFactory::runInvocation(std::shared_ptr<clang::CompilerInvocation>, clang::FileManager*, std::shared_ptr<clang::PCHContainerOperations>, clang::DiagnosticConsumer*) /home/victor/llvm/llvm-project/clang-tools-extra/clang-tidy/ClangTidy.cpp:577:37
#60 0x00005a09b309cafb clang::tooling::ToolInvocation::runInvocation(char const*, clang::driver::Compilation*, std::shared_ptr<clang::CompilerInvocation>, std::shared_ptr<clang::PCHContainerOperations>) /home/victor/llvm/llvm-project/clang/lib/Tooling/Tooling.cpp:438:18
#61 0x00005a09b309bcb8 clang::tooling::ToolInvocation::run() /home/victor/llvm/llvm-project/clang/lib/Tooling/Tooling.cpp:423:3
#62 0x00005a09b309de22 clang::tooling::ClangTool::run(clang::tooling::ToolAction*) /home/victor/llvm/llvm-project/clang/lib/Tooling/Tooling.cpp:621:11
#63 0x00005a09b30b6f98 clang::tidy::runClangTidy(clang::tidy::ClangTidyContext&, clang::tooling::CompilationDatabase const&, llvm::ArrayRef<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>>, llvm::IntrusiveRefCntPtr<llvm::vfs::OverlayFileSystem>, bool, bool, llvm::StringRef) /home/victor/llvm/llvm-project/clang-tools-extra/clang-tidy/ClangTidy.cpp:598:8
#64 0x00005a09b310c515 clang::tidy::clangTidyMain(int, char const**) /home/victor/llvm/llvm-project/clang-tools-extra/clang-tidy/tool/ClangTidyMain.cpp:713:7
#65 0x00005a09b1c468d2 main /home/victor/llvm/llvm-project/clang-tools-extra/clang-tidy/tool/ClangTidyToolMain.cpp:20:3
#66 0x0000787ce722a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#67 0x0000787ce722a28b call_init ./csu/../csu/libc-start.c:128:20
#68 0x0000787ce722a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#69 0x00005a09b1c467e5 _start (/home/victor/llvm/llvm-project/build/bin/clang-tidy+0x419a7e5)
Aborted (core dumped)
```


---

### Comment 8 - tonygould

hey thanks for all the investigation!

As I said, I'm up against it now on some hard-deadlines at work but my sense having built on different clang /clang-tidy versions is that's there's an underlying bug which reveals itself in different circumstances, possibly a "Heisenbug" (I hate the term, sorry, but you know what I mean). In the original report I saw multiple different kinds of backtraces. Then I didn't see the error on clang-tidy-20 but I did see it on the different file where I include boost/redis/src.hpp. I've also seen crashes in vanilla clang on various compiles -- but I'm trying to get my day job done so if I can rebuild and it works, I tend to move on. Thanks again.

---

### Comment 9 - tonygould

Btw I'm liking that clang-tidy 20 is picking up new stuff, e.g. spotting `auto` should be `const auto&` in contexts such as lambdas which it didn't before. This is a huge benefit of clang-tidy in my opinion, spotting performance issues where it's easy for me or a colleague to forget an ampersand here or there :)

---

### Comment 10 - vbvictor

> there's an underlying bug which reveals itself in different circumstances

I've seen an issue about a Heisenbug, though no work was done because of lack of reproduces, I guess.

It would be great if you could make a debug build of clang-tidy from `main` branch and use it for some time in your project to see if it crashes or not (when you would have free time, of course). If it crashed - we could potentially see an assertion.

---

### Comment 11 - tonygould

I have just two weeks left in my current job, but so far as I am able I will help. Is there an idiot's guide as to how I can build clang-tidy debug? This issue has given me a kick to upgrade our build system to ubuntu 25.04 / gcc14 / clang20.

---

### Comment 12 - vbvictor

You could follow these steps assuming you have `ninja`:

1. Run configure in `llvm-project` directory, adjust defines for your system: 
```
cmake -DCMAKE_BUILD_TYPE:STRING=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -DCMAKE_C_COMPILER:FILEPATH=/usr/bin/clang -DCMAKE_CXX_COMPILER:FILEPATH=/usr/bin/clang++ -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_FLAGS=-g -DLLVM_ENABLE_ASSERTIONS=ON "-DLLVM_ENABLE_PROJECTS='clang;clang-tools-extra'" -DLLVM_USE_LINKER=lld -DLLVM_PARALLEL_COMPILE_JOBS=24 -DLLVM_PARALLEL_LINK_JOBS=24 -DLLVM_PARALLEL_TABLEGEN_JOBS=24 -DLLVM_TARGETS_TO_BUILD='X86' -S llvm -B build -G Ninja
```
2. Build:
```
cmake --build build --config Debug --target clang-tidy
```

---

### Comment 13 - vbvictor

After that, you could use `-clang-tidy-binary` flag in `run-clang-tidy.py` to specify freshly build clang-tidy.
I can't recommend any `cmake install` commands since I haven't tested it myself, but they can be found in https://llvm.org/docs/GettingStarted.html

---

### Comment 14 - tonygould

Thank you! My project is building -- an order of magnitude slower :') -- will let you know if I see any issues. (Your build instructions worked perfectly, and to install I simply replaced the link `/usr/bin/clang-tidy` with a link to the one I'd built.)

---

### Comment 15 - tonygould

(I just picked the latest llvm commit, which happens to be the below, hope ok
commit 4af96a9d83335b3b59f3441af47c879c7a9eb183 (grafted, HEAD -> main)
Author: Momchil Velikov <momchil.velikov@arm.com>
Date:   Mon Jun 23 09:28:33 2025 +0100)

---

### Comment 16 - tonygould

@vbvictor have done several complete rebuilds using clang-tidy. This version from yesterday seems remarkably stable compared to previous ones. I wonder, is it worth me trying a sanitize build if it's possible to build that? I've found with our own code bases that certain intermittent crashes are most likely to show up and give a back trace from a crash in a sanitize build. If so, how would I modify the build instructions in your [post above](https://github.com/llvm/llvm-project/issues/144862#issuecomment-2992498022) to get a sanitize build?

---

### Comment 17 - zwuis

> how would I modify the build instructions in your [post above](https://github.com/llvm/llvm-project/issues/144862#issuecomment-2992498022) to get a sanitize build?

You can specific CMAKE variable `LLVM_USE_SANITIZER`. See <https://llvm.org/docs/CMake.html>.

---

### Comment 18 - tonygould

Sanitize immediately trips :)

```
/usr/bin/cmake -E __run_co_compile --launcher="/usr/bin/cmake;-E;env;CCACHE_SLOPPINESS=pch_defines,time_macros,include_file_mtime,include_file_ctime;/usr/local/bin/ccache" --tidy="/usr/bin/clang-tidy;-p;/home/dev/snip/out/build/clang-tidy;-header-filter=/home/dev/snip/src;-warnings-as-errors=*;--extra-arg-before=--driver-mode=g++" --source=/home/dev/snip/src/lib/database/kDatabaseClient.cpp -- /usr/bin/clang++ -DSPDLOG_COMPILED_LIB -DSPDLOG_USE_STD_FORMAT -I/home/dev/snip/src/lib -I/home/dev/snip/src/interface -I/home/dev/snip/out/build/clang-tidy/src/lib/database-service/idl/pb -isystem /home/dev/snip/out/build/clang-tidy/vcpkg_installed/x64-linux/include -isystem /home/dev/snip/out/build/clang-tidy/_deps/spdlog-src/include -O3 -DNDEBUG -std=c++23 -fPIC -Wall -Wextra -Xclang -fno-pch-timestamp -Werror -MD -MT src/lib/database/CMakeFiles/database.dir/kDatabaseClient.cpp.o -MF src/lib/database/CMakeFiles/database.dir/kDatabaseClient.cpp.o.d -o src/lib/database/CMakeFiles/database.dir/kDatabaseClient.cpp.o -c /home/dev/snip/src/lib/database/kDatabaseClient.cpp
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.      Program arguments: /usr/bin/clang-tidy -p /home/dev/snip/out/build/clang-tidy -header-filter=/home/dev/snip/src -warnings-as-errors=* --extra-arg-before=--driver-mode=g++ /home/dev/snip/src/lib/database/kDatabaseClient.cpp
1.      /home/dev/snip/out/build/clang-tidy/vcpkg_installed/x64-linux/include/absl/log/absl_log.h:79:2: current parser token 'define'
2.      /home/dev/snip/out/build/clang-tidy/vcpkg_installed/x64-linux/include/absl/log/internal/vlog_config.h:41:1: parsing namespace 'absl'
 #0 0x0000556aa4caac1a ___interceptor_backtrace (/usr/bin/clang-tidy+0x57b0c1a)
 #1 0x0000556aa4e1ea08 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /home/dev/llvm-project/llvm/lib/Support/Unix/Signals.inc:804:13
 #2 0x0000556aa4e18eb9 llvm::sys::RunSignalHandlers() /home/dev/llvm-project/llvm/lib/Support/Signals.cpp:0:5
 #3 0x0000556aa4e203e4 SignalHandler(int, siginfo_t*, void*) /home/dev/llvm-project/llvm/lib/Support/Unix/Signals.inc:0:3
 #4 0x00007f8479184810 (/lib/x86_64-linux-gnu/libc.so.6+0x45810)
 #5 0x0000556aa4de26ab llvm::raw_ostream::~raw_ostream() /home/dev/llvm-project/llvm/lib/Support/raw_ostream.cpp:75:29
 #6 0x0000556aa4e0c162 toVector /home/dev/llvm-project/llvm/lib/Support/Twine.cpp:35:1
 #7 0x0000556aa4e0c162 llvm::Twine::toStringRef(llvm::SmallVectorImpl<char>&) const /home/dev/llvm-project/llvm/include/llvm/ADT/Twine.h:497:7
 #8 0x0000556aa4e0c15a toVector /home/dev/llvm-project/llvm/lib/Support/Twine.cpp:35:1
 #9 0x0000556aa4e0c15a llvm::Twine::toStringRef(llvm::SmallVectorImpl<char>&) const /home/dev/llvm-project/llvm/include/llvm/ADT/Twine.h:497:7
#10 0x0000556aa4e0bc7c str /home/dev/llvm-project/llvm/include/llvm/ADT/StringRef.h:232:12
#11 0x0000556aa4e0bc7c llvm::Twine::str[abi:cxx11]() const /home/dev/llvm-project/llvm/lib/Support/Twine.cpp:29:27
#12 0x0000556aa50b931f clang::tidy::ClangTidyContext::diag(llvm::StringRef, clang::SourceLocation, llvm::StringRef, clang::DiagnosticIDs::Level) /home/dev/llvm-project/clang-tools-extra/clang-tidy/ClangTidyDiagnosticConsumer.cpp:0:53
#13 0x0000556aa693e2f4 clang::tidy::ClangTidyCheck::diag(clang::SourceLocation, llvm::StringRef, clang::DiagnosticIDs::Level) /home/dev/llvm-project/clang-tools-extra/clang-tidy/ClangTidyCheck.cpp:28:3
#14 0x0000556aa7048448 operator<< /home/dev/llvm-project/clang/include/clang/Basic/Diagnostic.h:1348:6
#15 0x0000556aa7048448 operator<<<llvm::StringRef> /home/dev/llvm-project/clang/include/clang/Basic/Diagnostic.h:1300:8
#16 0x0000556aa7048448 clang::tidy::cppcoreguidelines::MacroUsageCheck::warnMacro(clang::MacroDirective const*, llvm::StringRef) /home/dev/llvm-project/clang-tools-extra/clang-tidy/cppcoreguidelines/MacroUsageCheck.cpp:100:38
#17 0x0000556aa7049398 clang::tidy::cppcoreguidelines::(anonymous namespace)::MacroUsageCallbacks::MacroDefined(clang::Token const&, clang::MacroDirective const*) /home/dev/llvm-project/clang-tools-extra/clang-tidy/cppcoreguidelines/MacroUsageCheck.cpp:56:9
#18 0x0000556aabd6588a clang::PPChainedCallbacks::MacroDefined(clang::Token const&, clang::MacroDirective const*) /home/dev/llvm-project/clang/include/clang/Lex/PPCallbacks.h:678:5
#19 0x0000556aabd76d3a getLangOpts /home/dev/llvm-project/clang/include/clang/Lex/Preprocessor.h:1218:51
#20 0x0000556aabd76d3a clang::Preprocessor::HandleDefineDirective(clang::Token&, bool) /home/dev/llvm-project/clang/lib/Lex/PPDirectives.cpp:3315:8
#21 0x0000556aabd7956f clang::Preprocessor::HandleDirective(clang::Token&) /home/dev/llvm-project/clang/lib/Lex/PPDirectives.cpp:0:14
#22 0x0000556aabcd52f5 clang::Lexer::LexTokenInternal(clang::Token&, bool) /home/dev/llvm-project/clang/lib/Lex/Lexer.cpp:4528:7
#23 0x0000556aabccc075 clang::Lexer::Lex(clang::Token&) /home/dev/llvm-project/clang/lib/Lex/Lexer.cpp:3743:21
#24 0x0000556aabe0781c clang::Preprocessor::Lex(clang::Token&) /home/dev/llvm-project/clang/lib/Lex/Preprocessor.cpp:879:3
#25 0x0000556aa98e70df clang::Parser::ConsumeBrace() /home/dev/llvm-project/clang/include/clang/Parse/Parser.h:661:12
#26 0x0000556aa98fda4b clang::BalancedDelimiterTracker::consumeClose() /home/dev/llvm-project/clang/include/clang/Parse/RAIIObjectsForParser.h:483:9
#27 0x0000556aa99ad371 clang::Parser::ParseInnerNamespace(llvm::SmallVector<clang::Parser::InnerNamespaceInfo, 4u> const&, unsigned int, clang::SourceLocation&, clang::ParsedAttributes&, clang::BalancedDelimiterTracker&) /home/dev/llvm-project/clang/lib/Parse/ParseDeclCXX.cpp:0:13
#28 0x0000556aa99aa5e4 clang::Parser::ParseNamespace(clang::DeclaratorContext, clang::SourceLocation&, clang::SourceLocation) /home/dev/llvm-project/clang/lib/Parse/ParseDeclCXX.cpp:0:3
#29 0x0000556aa9a04a0a clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) /home/dev/llvm-project/clang/lib/Parse/ParseDecl.cpp:0:0
#30 0x0000556aa98f0dc2 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) /home/dev/llvm-project/clang/lib/Parse/Parser.cpp:946:14
#31 0x0000556aa98ed5c4 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) /home/dev/llvm-project/clang/lib/Parse/Parser.cpp:0:12
#32 0x0000556aa98ded2f clang::ParseAST(clang::Sema&, bool, bool) /home/dev/llvm-project/clang/lib/Parse/ParseAST.cpp:169:5
#33 0x0000556aa923134c clang::FrontendAction::Execute() /home/dev/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1225:10
#34 0x0000556aa90f9d67 getPtr /home/dev/llvm-project/llvm/include/llvm/Support/Error.h:281:12
#35 0x0000556aa90f9d67 operator bool /home/dev/llvm-project/llvm/include/llvm/Support/Error.h:241:16
#36 0x0000556aa90f9d67 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) /home/dev/llvm-project/clang/lib/Frontend/CompilerInstance.cpp:1055:23
#37 0x0000556aa4f7d815 clang::tooling::FrontendActionFactory::runInvocation(std::shared_ptr<clang::CompilerInvocation>, clang::FileManager*, std::shared_ptr<clang::PCHContainerOperations>, clang::DiagnosticConsumer*) /home/dev/llvm-project/clang/lib/Tooling/Tooling.cpp:463:33
#38 0x0000556aa509e28b clang::tidy::runClangTidy(clang::tidy::ClangTidyContext&, clang::tooling::CompilationDatabase const&, llvm::ArrayRef<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>>, llvm::IntrusiveRefCntPtr<llvm::vfs::OverlayFileSystem>, bool, bool, llvm::StringRef)::ActionFactory::runInvocation(std::shared_ptr<clang::CompilerInvocation>, clang::FileManager*, std::shared_ptr<clang::PCHContainerOperations>, clang::DiagnosticConsumer*) /home/dev/llvm-project/clang-tools-extra/clang-tidy/ClangTidy.cpp:0:37
#39 0x0000556aa4f7d3e4 clang::tooling::ToolInvocation::runInvocation(char const*, clang::driver::Compilation*, std::shared_ptr<clang::CompilerInvocation>, std::shared_ptr<clang::PCHContainerOperations>) /home/dev/llvm-project/clang/lib/Tooling/Tooling.cpp:438:18
#40 0x0000556aa4f7a9d2 clang::tooling::ToolInvocation::run() /home/dev/llvm-project/clang/lib/Tooling/Tooling.cpp:0:10
#41 0x0000556aa4f814c9 clang::tooling::ClangTool::run(clang::tooling::ToolAction*) /home/dev/llvm-project/clang/lib/Tooling/Tooling.cpp:621:11
#42 0x0000556aa508fab6 clang::tidy::runClangTidy(clang::tidy::ClangTidyContext&, clang::tooling::CompilationDatabase const&, llvm::ArrayRef<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>>, llvm::IntrusiveRefCntPtr<llvm::vfs::OverlayFileSystem>, bool, bool, llvm::StringRef) /home/dev/llvm-project/clang-tools-extra/clang-tidy/ClangTidy.cpp:599:23
#43 0x0000556aa4d51e05 clang::tidy::clangTidyMain(int, char const**) /home/dev/llvm-project/clang-tools-extra/clang-tidy/tool/ClangTidyMain.cpp:713:7
#44 0x00007f8479169338 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#45 0x00007f84791693fb call_init ./csu/../csu/libc-start.c:128:20
#46 0x00007f84791693fb __libc_start_main ./csu/../csu/libc-start.c:347:5
#47 0x0000556aa4c61905 _start (/usr/bin/clang-tidy+0x5767905)
AddressSanitizer:DEADLYSIGNAL
=================================================================
==12012==ERROR: AddressSanitizer: SEGV on unknown address 0x03ea00002eec (pc 0x7f84791e40bc bp 0x7f8478d979f0 sp 0x7f8478d979b0 T0)
==12012==The signal is caused by a READ memory access.
    #0 0x7f84791e40bc in __pthread_kill_implementation nptl/pthread_kill.c:44:76
    #1 0x7f84791e40bc in __pthread_kill_internal nptl/pthread_kill.c:89:10
    #2 0x7f84791e40bc in pthread_kill nptl/pthread_kill.c:100:10
    #3 0x7f84791846dd in raise signal/../sysdeps/posix/raise.c:26:13
    #4 0x556aa4e2042b in SignalHandler(int, siginfo_t*, void*) /home/dev/llvm-project/llvm/lib/Support/Unix/Signals.inc
    #5 0x7f847918480f  (/lib/x86_64-linux-gnu/libc.so.6+0x4580f) (BuildId: 467f544f15035abef911999cbc14489edd0555ab)
    #6 0x556aa4de26aa in llvm::raw_ostream::~raw_ostream() /home/dev/llvm-project/llvm/lib/Support/raw_ostream.cpp:75:29
    #7 0x556aa4e0c161 in toVector /home/dev/llvm-project/llvm/lib/Support/Twine.cpp:35:1
    #8 0x556aa4e0c161 in llvm::Twine::toStringRef(llvm::SmallVectorImpl<char>&) const /home/dev/llvm-project/llvm/include/llvm/ADT/Twine.h:497:7
    #9 0x556aa4e0c159 in print /home/dev/llvm-project/llvm/lib/Support/Twine.cpp:166:3
    #10 0x556aa4e0c159 in toVector /home/dev/llvm-project/llvm/lib/Support/Twine.cpp:34:3
    #11 0x556aa4e0c159 in llvm::Twine::toStringRef(llvm::SmallVectorImpl<char>&) const /home/dev/llvm-project/llvm/include/llvm/ADT/Twine.h:497:7
    #12 0x556aa4e0bc7b in llvm::Twine::str[abi:cxx11]() const /home/dev/llvm-project/llvm/lib/Support/Twine.cpp:29:10
    #13 0x556aa50b931e in clang::tidy::ClangTidyContext::diag(llvm::StringRef, clang::SourceLocation, llvm::StringRef, clang::DiagnosticIDs::Level) /home/dev/llvm-project/clang-tools-extra/clang-tidy/ClangTidyDiagnosticConsumer.cpp:180:53
    #14 0x556aa693e2f3 in clang::tidy::ClangTidyCheck::diag(clang::SourceLocation, llvm::StringRef, clang::DiagnosticIDs::Level) /home/dev/llvm-project/clang-tools-extra/clang-tidy/ClangTidyCheck.cpp:28:19
    #15 0x556aa7048447 in clang::tidy::cppcoreguidelines::MacroUsageCheck::warnMacro(clang::MacroDirective const*, llvm::StringRef) /home/dev/llvm-project/clang-tools-extra/clang-tidy/cppcoreguidelines/MacroUsageCheck.cpp:100:5
    #16 0x556aa7049397 in clang::tidy::cppcoreguidelines::(anonymous namespace)::MacroUsageCallbacks::MacroDefined(clang::Token const&, clang::MacroDirective const*) /home/dev/llvm-project/clang-tools-extra/clang-tidy/cppcoreguidelines/MacroUsageCheck.cpp:54:14
    #17 0x556aabd65889 in clang::PPChainedCallbacks::MacroDefined(clang::Token const&, clang::MacroDirective const*) /home/dev/llvm-project/clang/include/clang/Lex/PPCallbacks.h:677:12
    #18 0x556aabd76d39 in clang::Preprocessor::HandleDefineDirective(clang::Token&, bool) /home/dev/llvm-project/clang/lib/Lex/PPDirectives.cpp:3309:16
    #19 0x556aabd7956e in clang::Preprocessor::HandleDirective(clang::Token&) /home/dev/llvm-project/clang/lib/Lex/PPDirectives.cpp:1348:14
    #20 0x556aabcd52f4 in clang::Lexer::LexTokenInternal(clang::Token&, bool) /home/dev/llvm-project/clang/lib/Lex/Lexer.cpp:4526:7
    #21 0x556aabccc074 in clang::Lexer::Lex(clang::Token&) /home/dev/llvm-project/clang/lib/Lex/Lexer.cpp:3739:24
    #22 0x556aabe0781b in clang::Preprocessor::Lex(clang::Token&) /home/dev/llvm-project/clang/lib/Lex/Preprocessor.cpp:879:11
    #23 0x556aa98e70de in clang::Parser::ConsumeBrace() /home/dev/llvm-project/clang/include/clang/Parse/Parser.h:660:8
    #24 0x556aa98fda4a in clang::BalancedDelimiterTracker::consumeClose() /home/dev/llvm-project/clang/include/clang/Parse/RAIIObjectsForParser.h:483:18
    #25 0x556aa99ad370 in clang::Parser::ParseInnerNamespace(llvm::SmallVector<clang::Parser::InnerNamespaceInfo, 4u> const&, unsigned int, clang::SourceLocation&, clang::ParsedAttributes&, clang::BalancedDelimiterTracker&) /home/dev/llvm-project/clang/lib/Parse/ParseDeclCXX.cpp:242:13
    #26 0x556aa99aa5e3 in clang::Parser::ParseNamespace(clang::DeclaratorContext, clang::SourceLocation&, clang::SourceLocation) /home/dev/llvm-project/clang/lib/Parse/ParseDeclCXX.cpp:215:3
    #27 0x556aa9a04a09 in clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) /home/dev/llvm-project/clang/lib/Parse/ParseDecl.cpp
    #28 0x556aa98f0dc1 in clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) /home/dev/llvm-project/clang/lib/Parse/Parser.cpp:946:14
    #29 0x556aa98ed5c3 in clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) /home/dev/llvm-project/clang/lib/Parse/Parser.cpp:745:12
    #30 0x556aa98ded2e in clang::ParseAST(clang::Sema&, bool, bool) /home/dev/llvm-project/clang/lib/Parse/ParseAST.cpp:170:20
    #31 0x556aa923134b in clang::FrontendAction::Execute() /home/dev/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1221:3
    #32 0x556aa90f9d66 in clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) /home/dev/llvm-project/clang/lib/Frontend/CompilerInstance.cpp:1055:33
    #33 0x556aa4f7d814 in clang::tooling::FrontendActionFactory::runInvocation(std::shared_ptr<clang::CompilerInvocation>, clang::FileManager*, std::shared_ptr<clang::PCHContainerOperations>, clang::DiagnosticConsumer*) /home/dev/llvm-project/clang/lib/Tooling/Tooling.cpp:463:33
    #34 0x556aa509e28a in clang::tidy::runClangTidy(clang::tidy::ClangTidyContext&, clang::tooling::CompilationDatabase const&, llvm::ArrayRef<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>>, llvm::IntrusiveRefCntPtr<llvm::vfs::OverlayFileSystem>, bool, bool, llvm::StringRef)::ActionFactory::runInvocation(std::shared_ptr<clang::CompilerInvocation>, clang::FileManager*, std::shared_ptr<clang::PCHContainerOperations>, clang::DiagnosticConsumer*) /home/dev/llvm-project/clang-tools-extra/clang-tidy/ClangTidy.cpp:577:37
    #35 0x556aa4f7d3e3 in clang::tooling::ToolInvocation::runInvocation(char const*, clang::driver::Compilation*, std::shared_ptr<clang::CompilerInvocation>, std::shared_ptr<clang::PCHContainerOperations>) /home/dev/llvm-project/clang/lib/Tooling/Tooling.cpp:438:18
    #36 0x556aa4f7a9d1 in clang::tooling::ToolInvocation::run() /home/dev/llvm-project/clang/lib/Tooling/Tooling.cpp:423:10
    #37 0x556aa4f814c8 in clang::tooling::ClangTool::run(clang::tooling::ToolAction*) /home/dev/llvm-project/clang/lib/Tooling/Tooling.cpp:621:23
    #38 0x556aa508fab5 in clang::tidy::runClangTidy(clang::tidy::ClangTidyContext&, clang::tooling::CompilationDatabase const&, llvm::ArrayRef<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>>, llvm::IntrusiveRefCntPtr<llvm::vfs::OverlayFileSystem>, bool, bool, llvm::StringRef) /home/dev/llvm-project/clang-tools-extra/clang-tidy/ClangTidy.cpp:598:8
    #39 0x556aa4d51e04 in clang::tidy::clangTidyMain(int, char const**) /home/dev/llvm-project/clang-tools-extra/clang-tidy/tool/ClangTidyMain.cpp:713:7
    #40 0x7f8479169337 in __libc_start_call_main csu/../sysdeps/nptl/libc_start_call_main.h:58:16
    #41 0x7f84791693fa in __libc_start_main csu/../csu/libc-start.c:360:3
    #42 0x556aa4c61904 in _start (/home/dev/llvm-project/build-sanitize/bin/clang-tidy+0x5767904) (BuildId: b25f5b1f12d9549f057f21cc730e8c25a2f66948)

==12012==Register values:
rax = 0x0000000000000000  rbx = 0x0000000000002eec  rcx = 0x00007f84791e40bc  rdx = 0x000000000000000b  
rdi = 0x0000000000002eec  rsi = 0x0000000000002eec  rbp = 0x00007f8478d979f0  rsp = 0x00007f8478d979b0  
 r8 = 0x00000aad56028072   r9 = 0x0000556ab014039f  r10 = 0x000000001044e839  r11 = 0x0000000000000246  
r12 = 0x000000000000000b  r13 = 0x00007b84774790f0  r14 = 0x0000000000000016  r15 = 0x00000f710ee87200  
AddressSanitizer can not provide additional info.
SUMMARY: AddressSanitizer: SEGV nptl/pthread_kill.c:44:76 in __pthread_kill_implementation
==12012==ABORTING
ninja: build stopped: subcommand failed.
CompletedProcess(args=['/bin/bash', PosixPath('/home/dev/snip/buildscripts/linbuild.sh')], returncode=1)```

Again, is intermittent, have rebuilt several times and have not seen this again.

---

### Comment 19 - tonygould

also, not relevant to the crashes I'm seeing, on other files I get UB san

```
/home/dev/llvm-project/clang-tools-extra/clang-tidy/readability/UseNumericLimitsCheck.cpp:69:64: runtime error: negation of -9223372036854775808 cannot be represented in type 'long'; cast to an unsigned type to negate this value to itself
SUMMARY: UndefinedBehaviorSanitizer: undefined-behavior /home/dev/llvm-project/clang-tools-extra/clang-tidy/readability/UseNumericLimitsCheck.cpp:69:64 ```

---

### Comment 20 - vbvictor

> also, not relevant to the crashes I'm seeing, on other files I get UB san

Yes, this patch is already reverted on main because `*-san` builders caught this problem too.

---

### Comment 21 - tonygould

again not relevant to the crashes but saw memory leaks

```
==54850==ERROR: LeakSanitizer: detected memory leaks

Direct leak of 648 byte(s) in 3 object(s) allocated from:
    #0 0x55e17b7645f8 in malloc (/home/dev/llvm-project/build-sanitize/bin/clang-tidy+0x580b5f8) (BuildId: b25f5b1f12d9549f057f21cc730e8c25a2f66948)
    #1 0x55e17b8357a2 in safe_malloc /home/dev/llvm-project/llvm/include/llvm/Support/MemAlloc.h:26:18
    #2 0x55e17b8357a2 in llvm::SmallVectorBase<unsigned int>::grow_pod(void*, unsigned long, unsigned long) /home/dev/llvm-project/llvm/lib/Support/SmallVector.cpp:151:15
    #3 0x55e17f8193b5 in grow_pod /home/dev/llvm-project/llvm/include/llvm/ADT/SmallVector.h:139:11
    #4 0x55e17f8193b5 in grow /home/dev/llvm-project/llvm/include/llvm/ADT/SmallVector.h:526:41
    #5 0x55e17f8193b5 in llvm::SmallVectorImpl<clang::TemplateArgument>::operator=(llvm::SmallVectorImpl<clang::TemplateArgument> const&) /home/dev/llvm-project/llvm/include/llvm/ADT/SmallVector.h:1037:11
    #6 0x55e181b256cf in operator= /home/dev/llvm-project/llvm/include/llvm/ADT/SmallVector.h:1243:25
    #7 0x55e181b256cf in operator= /home/dev/llvm-project/clang/include/clang/AST/ASTConcept.h:36:7
    #8 0x55e181b256cf in clang::MakeDeductionFailureInfo(clang::ASTContext&, clang::TemplateDeductionResult, clang::sema::TemplateDeductionInfo&) /home/dev/llvm-project/clang/lib/Sema/SemaOverload.cpp:804:25
    #9 0x55e181b5bb2b in AddTemplateOverloadCandidateImmediately(clang::Sema&, clang::OverloadCandidateSet&, clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, bool, bool, bool, clang::CallExpr::ADLCallKind, clang::OverloadCandidateParamOrder, bool) /home/dev/llvm-project/clang/lib/Sema/SemaOverload.cpp:8035:11
    #10 0x55e181b7cde3 in AddTemplateOverloadCandidate /home/dev/llvm-project/clang/lib/Sema/SemaOverload.cpp:11222:3
    #11 0x55e181b7cde3 in clang::OverloadCandidateSet::InjectNonDeducedTemplateCandidates(clang::Sema&) /home/dev/llvm-project/clang/lib/Sema/SemaOverload.cpp:11244:7
    #12 0x55e181b4d6f6 in clang::OverloadCandidateSet::BestViableFunction(clang::Sema&, clang::SourceLocation, clang::OverloadCandidate*&) /home/dev/llvm-project/clang/lib/Sema/SemaOverload.cpp:11338:3
    #13 0x55e181b9475d in clang::Sema::BuildOverloadedCallExpr(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) /home/dev/llvm-project/clang/lib/Sema/SemaOverload.cpp:14799:20
    #14 0x55e1812b4136 in clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) /home/dev/llvm-project/clang/lib/Sema/SemaExpr.cpp:6659:16
    #15 0x55e1812efb58 in clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) /home/dev/llvm-project/clang/lib/Sema/SemaExpr.cpp:6515:7
    #16 0x55e1820f1f3f in RebuildCallExpr /home/dev/llvm-project/clang/lib/Sema/TreeTransform.h:2868:22
    #17 0x55e1820f1f3f in clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCallExpr(clang::CallExpr*) /home/dev/llvm-project/clang/lib/Sema/TreeTransform.h:13421:23
    #18 0x55e182125492 in clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformDecltypeType(clang::TypeLocBuilder&, clang::DecltypeTypeLoc) /home/dev/llvm-project/clang/lib/Sema/TreeTransform.h:6826:31
    #19 0x55e1820b8676 in TransformFunctionProtoType<(lambda at /home/dev/llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:3102:9)> /home/dev/llvm-project/clang/lib/Sema/TreeTransform.h:6453:33
    #20 0x55e1820b8676 in TransformFunctionProtoType<(lambda at /home/dev/llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:3102:9)> /home/dev/llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:2600:21
    #21 0x55e1820b8676 in clang::Sema::SubstFunctionDeclType(clang::TypeSourceInfo*, clang::MultiLevelTemplateArgumentList const&, clang::SourceLocation, clang::DeclarationName, clang::CXXRecordDecl*, clang::Qualifiers, bool) /home/dev/llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:3100:27
    #22 0x55e1821ba5e0 in clang::TemplateDeclInstantiator::SubstFunctionType(clang::FunctionDecl*, llvm::SmallVectorImpl<clang::ParmVarDecl*>&) /home/dev/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:5006:38
    #23 0x55e1821b4b5d in clang::TemplateDeclInstantiator::VisitFunctionDecl(clang::FunctionDecl*, clang::TemplateParameterList*, clang::TemplateDeclInstantiator::RewriteKind) /home/dev/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:2667:27
    #24 0x55e18228fc00 in operator() /home/dev/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:4637:27
    #25 0x55e18228fc00 in void llvm::function_ref<void ()>::callback_fn<clang::Sema::SubstDecl(clang::Decl*, clang::DeclContext*, clang::MultiLevelTemplateArgumentList const&)::$_0>(long) /home/dev/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
    #26 0x55e1805815c0 in operator() /home/dev/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
    #27 0x55e1805815c0 in runWithSufficientStackSpace /home/dev/llvm-project/clang/include/clang/Basic/Stack.h:49:7
    #28 0x55e1805815c0 in clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) /home/dev/llvm-project/clang/lib/Basic/StackExhaustionHandler.cpp:20:3
    #29 0x55e1821d25fb in clang::Sema::SubstDecl(clang::Decl*, clang::DeclContext*, clang::MultiLevelTemplateArgumentList const&) /home/dev/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:4636:3
    #30 0x55e181f0aba1 in clang::Sema::FinishTemplateArgumentDeduction(clang::FunctionTemplateDecl*, llvm::SmallVectorImpl<clang::DeducedTemplateArgument>&, unsigned int, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, llvm::SmallVectorImpl<clang::Sema::OriginalCallArg> const*, bool, bool, llvm::function_ref<bool (bool)>) /home/dev/llvm-project/clang/lib/Sema/SemaTemplateDeduction.cpp:3971:7
    #31 0x55e181febcd4 in operator() /home/dev/llvm-project/clang/lib/Sema/SemaTemplateDeduction.cpp:4681:14
    #32 0x55e181febcd4 in void llvm::function_ref<void ()>::callback_fn<clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, bool, bool, clang::QualType, clang::Expr::Classification, bool, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>, bool)>)::$_2>(long) /home/dev/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
    #33 0x55e1805815c0 in operator() /home/dev/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
    #34 0x55e1805815c0 in runWithSufficientStackSpace /home/dev/llvm-project/clang/include/clang/Basic/Stack.h:49:7
    #35 0x55e1805815c0 in clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) /home/dev/llvm-project/clang/lib/Basic/StackExhaustionHandler.cpp:20:3
    #36 0x55e181f129f4 in clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, bool, bool, clang::QualType, clang::Expr::Classification, bool, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>, bool)>) /home/dev/llvm-project/clang/lib/Sema/SemaTemplateDeduction.cpp:4680:3
    #37 0x55e181b5b753 in AddTemplateOverloadCandidateImmediately(clang::Sema&, clang::OverloadCandidateSet&, clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, bool, bool, bool, clang::CallExpr::ADLCallKind, clang::OverloadCandidateParamOrder, bool) /home/dev/llvm-project/clang/lib/Sema/SemaOverload.cpp:7997:42
    #38 0x55e181b59e83 in clang::Sema::AddTemplateOverloadCandidate(clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::OverloadCandidateSet&, bool, bool, bool, clang::CallExpr::ADLCallKind, clang::OverloadCandidateParamOrder, bool) /home/dev/llvm-project/clang/lib/Sema/SemaOverload.cpp:8067:5
    #39 0x55e181b937a7 in AddOverloadedCallCandidate(clang::Sema&, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::OverloadCandidateSet&, bool, bool) /home/dev/llvm-project/clang/lib/Sema/SemaOverload.cpp:14216:7
    #40 0x55e181b93243 in clang::Sema::AddOverloadedCallCandidates(clang::UnresolvedLookupExpr*, llvm::ArrayRef<clang::Expr*>, clang::OverloadCandidateSet&, bool) /home/dev/llvm-project/clang/lib/Sema/SemaOverload.cpp:14270:5
    #41 0x55e181b93bdd in clang::Sema::buildOverloadedCallSet(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::OverloadCandidateSet*, clang::ActionResult<clang::Expr*, true>*) /home/dev/llvm-project/clang/lib/Sema/SemaOverload.cpp:14576:3
    #42 0x55e181b945bd in clang::Sema::BuildOverloadedCallExpr(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) /home/dev/llvm-project/clang/lib/Sema/SemaOverload.cpp:14788:7
    #43 0x55e1812b4136 in clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) /home/dev/llvm-project/clang/lib/Sema/SemaExpr.cpp:6659:16
    #44 0x55e1812efb58 in clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) /home/dev/llvm-project/clang/lib/Sema/SemaExpr.cpp:6515:7

Direct leak of 648 byte(s) in 3 object(s) allocated from:
    #0 0x55e17b7645f8 in malloc (/home/dev/llvm-project/build-sanitize/bin/clang-tidy+0x580b5f8) (BuildId: b25f5b1f12d9549f057f21cc730e8c25a2f66948)
    #1 0x55e17b8357a2 in safe_malloc /home/dev/llvm-project/llvm/include/llvm/Support/MemAlloc.h:26:18
    #2 0x55e17b8357a2 in llvm::SmallVectorBase<unsigned int>::grow_pod(void*, unsigned long, unsigned long) /home/dev/llvm-project/llvm/lib/Support/SmallVector.cpp:151:15
    #3 0x55e17f8193b5 in grow_pod /home/dev/llvm-project/llvm/include/llvm/ADT/SmallVector.h:139:11
    #4 0x55e17f8193b5 in grow /home/dev/llvm-project/llvm/include/llvm/ADT/SmallVector.h:526:41
    #5 0x55e17f8193b5 in llvm::SmallVectorImpl<clang::TemplateArgument>::operator=(llvm::SmallVectorImpl<clang::TemplateArgument> const&) /home/dev/llvm-project/llvm/include/llvm/ADT/SmallVector.h:1037:11
    #6 0x55e181b256cf in operator= /home/dev/llvm-project/llvm/include/llvm/ADT/SmallVector.h:1243:25
    #7 0x55e181b256cf in operator= /home/dev/llvm-project/clang/include/clang/AST/ASTConcept.h:36:7
    #8 0x55e181b256cf in clang::MakeDeductionFailureInfo(clang::ASTContext&, clang::TemplateDeductionResult, clang::sema::TemplateDeductionInfo&) /home/dev/llvm-project/clang/lib/Sema/SemaOverload.cpp:804:25
    #9 0x55e181b5bb2b in AddTemplateOverloadCandidateImmediately(clang::Sema&, clang::OverloadCandidateSet&, clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, bool, bool, bool, clang::CallExpr::ADLCallKind, clang::OverloadCandidateParamOrder, bool) /home/dev/llvm-project/clang/lib/Sema/SemaOverload.cpp:8035:11
    #10 0x55e181b7cde3 in AddTemplateOverloadCandidate /home/dev/llvm-project/clang/lib/Sema/SemaOverload.cpp:11222:3
    #11 0x55e181b7cde3 in clang::OverloadCandidateSet::InjectNonDeducedTemplateCandidates(clang::Sema&) /home/dev/llvm-project/clang/lib/Sema/SemaOverload.cpp:11244:7
    #12 0x55e181b4d6f6 in clang::OverloadCandidateSet::BestViableFunction(clang::Sema&, clang::SourceLocation, clang::OverloadCandidate*&) /home/dev/llvm-project/clang/lib/Sema/SemaOverload.cpp:11338:3
    #13 0x55e181b9475d in clang::Sema::BuildOverloadedCallExpr(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) /home/dev/llvm-project/clang/lib/Sema/SemaOverload.cpp:14799:20
    #14 0x55e1812b4136 in clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) /home/dev/llvm-project/clang/lib/Sema/SemaExpr.cpp:6659:16
    #15 0x55e1812efb58 in clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) /home/dev/llvm-project/clang/lib/Sema/SemaExpr.cpp:6515:7
    #16 0x55e1820f1f3f in RebuildCallExpr /home/dev/llvm-project/clang/lib/Sema/TreeTransform.h:2868:22
    #17 0x55e1820f1f3f in clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCallExpr(clang::CallExpr*) /home/dev/llvm-project/clang/lib/Sema/TreeTransform.h:13421:23
    #18 0x55e182125492 in clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformDecltypeType(clang::TypeLocBuilder&, clang::DecltypeTypeLoc) /home/dev/llvm-project/clang/lib/Sema/TreeTransform.h:6826:31
    #19 0x55e1820b8676 in TransformFunctionProtoType<(lambda at /home/dev/llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:3102:9)> /home/dev/llvm-project/clang/lib/Sema/TreeTransform.h:6453:33
    #20 0x55e1820b8676 in TransformFunctionProtoType<(lambda at /home/dev/llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:3102:9)> /home/dev/llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:2600:21
    #21 0x55e1820b8676 in clang::Sema::SubstFunctionDeclType(clang::TypeSourceInfo*, clang::MultiLevelTemplateArgumentList const&, clang::SourceLocation, clang::DeclarationName, clang::CXXRecordDecl*, clang::Qualifiers, bool) /home/dev/llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:3100:27
    #22 0x55e1821ba5e0 in clang::TemplateDeclInstantiator::SubstFunctionType(clang::FunctionDecl*, llvm::SmallVectorImpl<clang::ParmVarDecl*>&) /home/dev/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:5006:38
    #23 0x55e1821b4b5d in clang::TemplateDeclInstantiator::VisitFunctionDecl(clang::FunctionDecl*, clang::TemplateParameterList*, clang::TemplateDeclInstantiator::RewriteKind) /home/dev/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:2667:27
    #24 0x55e18228fc00 in operator() /home/dev/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:4637:27
    #25 0x55e18228fc00 in void llvm::function_ref<void ()>::callback_fn<clang::Sema::SubstDecl(clang::Decl*, clang::DeclContext*, clang::MultiLevelTemplateArgumentList const&)::$_0>(long) /home/dev/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
    #26 0x55e1805815c0 in operator() /home/dev/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
    #27 0x55e1805815c0 in runWithSufficientStackSpace /home/dev/llvm-project/clang/include/clang/Basic/Stack.h:49:7
    #28 0x55e1805815c0 in clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) /home/dev/llvm-project/clang/lib/Basic/StackExhaustionHandler.cpp:20:3
    #29 0x55e1821d25fb in clang::Sema::SubstDecl(clang::Decl*, clang::DeclContext*, clang::MultiLevelTemplateArgumentList const&) /home/dev/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:4636:3
    #30 0x55e181f0aba1 in clang::Sema::FinishTemplateArgumentDeduction(clang::FunctionTemplateDecl*, llvm::SmallVectorImpl<clang::DeducedTemplateArgument>&, unsigned int, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, llvm::SmallVectorImpl<clang::Sema::OriginalCallArg> const*, bool, bool, llvm::function_ref<bool (bool)>) /home/dev/llvm-project/clang/lib/Sema/SemaTemplateDeduction.cpp:3971:7
    #31 0x55e181febcd4 in operator() /home/dev/llvm-project/clang/lib/Sema/SemaTemplateDeduction.cpp:4681:14
    #32 0x55e181febcd4 in void llvm::function_ref<void ()>::callback_fn<clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, bool, bool, clang::QualType, clang::Expr::Classification, bool, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>, bool)>)::$_2>(long) /home/dev/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
    #33 0x55e1805815c0 in operator() /home/dev/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
    #34 0x55e1805815c0 in runWithSufficientStackSpace /home/dev/llvm-project/clang/include/clang/Basic/Stack.h:49:7
    #35 0x55e1805815c0 in clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) /home/dev/llvm-project/clang/lib/Basic/StackExhaustionHandler.cpp:20:3
    #36 0x55e181f129f4 in clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, bool, bool, clang::QualType, clang::Expr::Classification, bool, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>, bool)>) /home/dev/llvm-project/clang/lib/Sema/SemaTemplateDeduction.cpp:4680:3
    #37 0x55e181b5b753 in AddTemplateOverloadCandidateImmediately(clang::Sema&, clang::OverloadCandidateSet&, clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, bool, bool, bool, clang::CallExpr::ADLCallKind, clang::OverloadCandidateParamOrder, bool) /home/dev/llvm-project/clang/lib/Sema/SemaOverload.cpp:7997:42
    #38 0x55e181b59e83 in clang::Sema::AddTemplateOverloadCandidate(clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::OverloadCandidateSet&, bool, bool, bool, clang::CallExpr::ADLCallKind, clang::OverloadCandidateParamOrder, bool) /home/dev/llvm-project/clang/lib/Sema/SemaOverload.cpp:8067:5
    #39 0x55e181b937a7 in AddOverloadedCallCandidate(clang::Sema&, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::OverloadCandidateSet&, bool, bool) /home/dev/llvm-project/clang/lib/Sema/SemaOverload.cpp:14216:7
    #40 0x55e181b939b4 in clang::Sema::AddOverloadedCallCandidates(clang::LookupResult&, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::OverloadCandidateSet&) /home/dev/llvm-project/clang/lib/Sema/SemaOverload.cpp:14284:5
    #41 0x55e181bd12f0 in DiagnoseTwoPhaseLookup(clang::Sema&, clang::SourceLocation, clang::CXXScopeSpec const&, clang::LookupResult&, clang::OverloadCandidateSet::CandidateSetKind, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::CXXRecordDecl**) /home/dev/llvm-project/clang/lib/Sema/SemaOverload.cpp:14325:15
    #42 0x55e181b95ec6 in BuildRecoveryCallExpr /home/dev/llvm-project/clang/lib/Sema/SemaOverload.cpp:14479:7
    #43 0x55e181b95ec6 in FinishOverloadedCallExpr(clang::Sema&, clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, clang::OverloadCandidateSet*, clang::OverloadCandidate**, clang::OverloadingResult, bool) /home/dev/llvm-project/clang/lib/Sema/SemaOverload.cpp:14696:27
    #44 0x55e181b948ad in clang::Sema::BuildOverloadedCallExpr(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) /home/dev/llvm-project/clang/lib/Sema/SemaOverload.cpp:14845:10
    #45 0x55e1812b4136 in clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) /home/dev/llvm-project/clang/lib/Sema/SemaExpr.cpp:6659:16

SUMMARY: AddressSanitizer: 1296 byte(s) leaked in 6 allocation(s).```

---

### Comment 22 - vbvictor

As odd as it sounds, but these memory leaks may be intentional, please see https://github.com/llvm/llvm-project/issues/143129


---

### Comment 23 - tonygould

makes sense, and just to note, I get a crash with stock clang tidy 20 on the same file
(
```
Error running '/usr/bin/clang-tidy': malloc(): invalid next->prev_inuse (unsorted)
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.      Program arguments: /usr/bin/clang-tidy -p /home/dev/kwantcentral/analytics/out/build/clang-tidy -header-filter=/home/dev/kwantcentral/analytics/src -warnings-as-errors=* --extra-arg-before=--driver-mode=g++ /home/dev/kwantcentral/analytics/src/lib/database-service/kRing.cpp
1.      <eof> parser at end of file
2.      ASTMatcher: Processing 'bugprone-use-after-move' against:
        CXXMemberCallExpr : </home/dev/kwantcentral/analytics/out/build/clang-tidy/vcpkg_installed/x64-linux/include/nlohmann/json.hpp:2077:19, col:73>
--- Bound Nodes Begin ---
    arg - { DeclRefExpr : </home/dev/kwantcentral/analytics/out/build/clang-tidy/vcpkg_installed/x64-linux/include/nlohmann/json.hpp:2077:69> }
    call-move - { CallExpr : </home/dev/kwantcentral/analytics/out/build/clang-tidy/vcpkg_installed/x64-linux/include/nlohmann/json.hpp:2077:47, col:72> }
    containing-func - { CXXMethodDecl nlohmann::basic_json<>::at : </home/dev/kwantcentral/analytics/out/build/clang-tidy/vcpkg_installed/x64-linux/include/nlohmann/json.hpp:2069:5, line:2083:5> }
    move-decl - { FunctionDecl std::forward : </usr/bin/../lib/gcc/x86_64-linux-gnu/14/../../../../include/c++/14/bits/move.h:70:5, line:72:39> }
    moving-call - { CXXMemberCallExpr : </home/dev/kwantcentral/analytics/out/build/clang-tidy/vcpkg_installed/x64-linux/include/nlohmann/json.hpp:2077:19, col:73> }
--- Bound Nodes End ---
malloc(): invalid next->prev_inuse (unsorted)
```
)

---

### Comment 24 - tonygould

> As odd as it sounds, but these memory leaks may be intentional, please see [#143129](https://github.com/llvm/llvm-project/issues/143129)

Actually, it is unproductive to have to keep switching clang-tidy whenever I hit the file that is associated with this arena allocation. Maybe I'm missing something, but as it stands I can't run my sanitized clang-tidy over my whole project.

Just to check, what options would be useful for clang-tidy sanitizer? From https://llvm.org/docs/CMake.html.

> LLVM_USE_SANITIZER:STRING
> Define the sanitizer used to build LLVM binaries and tests. Possible values are Address, Memory, MemoryWithOrigins, Undefined, Thread, DataFlow, and Address;Undefined. Defaults to empty string.

I currently have `Address;Undefined`. I was under the impression that in general, I can separate address and leak sanitizers, but I don't see that option here. In any case, happy to build with anything you might recommend as useful in tracking down the failures I have been seeing (or showing they don't occur in the latest clang-tidy src).


---

### Comment 25 - 5chmidti

Given that this is a crash, could you reduce the input? At some point the code should be so small that it would not contain any proprietary code anymore.
You can use creduce or it's faster sibling cvise and to ensure the flakyness does not mean the wrong thing is reduced, maybe just repeat the clang-tidy call 15 times or so in the interestingness script? So copy the compile command from the compile-commands.json and then run clang-tidy a few times in the  interestingness test.

(A preprocessed input is best but might not work)

---

### Comment 26 - tonygould

Following code causes a crash pretty reliably using release build of latest clang-tidy, but it feels like a different kind of crash from that I see elsewhere (and also this is the file that has the arena leaks so can't use sanitizer)

```
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/consign.hpp>
#include <boost/asio/deferred.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/redirect_error.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/asio/use_awaitable.hpp>
#include <boost/asio/use_future.hpp>
#include <boost/redis/config.hpp>
#include <boost/redis/connection.hpp>
#include <chrono>
#include <nlohmann/json.hpp>
#include <numeric>
#include <ranges>
#include <thread>
```
(boost 1.88)

Error is `<eof> parser at end of file` . The matcher & backtrace varies. Some examples below.

```
0.      Program arguments: /usr/bin/clang-tidy -p /home/dev/kwantcentral/analytics/out/build/clang-tidy -header-filter=/home/dev/kwantcentral/analytics/src -warnings-as-errors=* --extra-arg-before=--driver-mode=g++ 
1.      <eof> parser at end of file
2.      ASTMatcher: Matching 'bugprone-use-after-move' against:
        ImplicitCastExpr : </usr/bin/../lib/gcc/x86_64-linux-gnu/14/../../../../include/c++/14/bits/atomic_base.h:269:37>
 #0 0x000055a60ee99ad8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/usr/bin/clang-tidy+0x1974ad8)
 #1 0x000055a60ee977a5 llvm::sys::RunSignalHandlers() (/usr/bin/clang-tidy+0x19727a5)
 #2 0x000055a60ee9a471 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x00007f38c2551810 (/lib/x86_64-linux-gnu/libc.so.6+0x45810)
 #4 0x00007f38c25bd7b5 _int_free_create_chunk ./malloc/malloc.c:4756:6
 #5 0x00007f38c25bd7b5 _int_free_merge_chunk ./malloc/malloc.c:4742:10
 #6 0x00007f38c25c08ab cfree ./malloc/malloc.c:3479:3
 #7 0x000055a60f6b4bf5 std::_Rb_tree<clang::ast_matchers::internal::(anonymous namespace)::MatchKey, std::pair<clang::ast_matchers::internal::(anonymous namespace)::MatchKey const, clang::ast_matchers::internal::(anonymous namespace)::MemoizedMatchResult>, std::_Select1st<std::pair<clang::ast_matchers::internal::(anonymous namespace)::MatchKey const, clang::ast_matchers::internal::(anonymous namespace)::MemoizedMatchResult>>, std::less<clang::ast_matchers::internal::(anonymous namespace)::MatchKey>, std::allocator<std::pair<clang::ast_matchers::internal::(anonymous namespace)::MatchKey const, clang::ast_matchers::internal::(anonymous namespace)::MemoizedMatchResult>>>::_M_erase(std::_Rb_tree_node<std::pair<clang::ast_matchers::internal::(anonymous namespace)::MatchKey const, clang::ast_matchers::internal::(anonymous namespace)::MemoizedMatchResult>>*) ASTMatchFinder.cpp:0:0
 #8 0x000055a60f6b4be9 std::_Rb_tree<clang::ast_matchers::internal::(anonymous namespace)::MatchKey, std::pair<clang::ast_matchers::internal::(anonymous namespace)::MatchKey const, clang::ast_matchers::internal::(anonymous namespace)::MemoizedMatchResult>, std::_Select1st<std::pair<clang::ast_matchers::internal::(anonymous namespace)::MatchKey const, clang::ast_matchers::internal::(anonymous namespace)::MemoizedMatchResult>>, std::less<clang::ast_matchers::internal::(anonymous namespace)::MatchKey>, std::allocator<std::pair<clang::ast_matchers::internal::(anonymous namespace)::MatchKey const, clang::ast_matchers::internal::(anonymous namespace)::MemoizedMatchResult>>>::_M_erase(std::_Rb_tree_node<std::pair<clang::ast_matchers::internal::(anonymous namespace)::MatchKey const, clang::ast_matchers::internal::(anonymous namespace)::MemoizedMatchResult>>*) ASTMatchFinder.cpp:0:0
 #9 0x000055a60f6b4be9 std::_Rb_tree<clang::ast_matchers::internal::(anonymous namespace)::MatchKey, std::pair<clang::ast_matchers::internal::(anonymous namespace)::MatchKey const, clang::ast_matchers::internal::(anonymous namespace)::MemoizedMatchResult>, std::_Select1st<std::pair<clang::ast_matchers::internal::(anonymous namespace)::MatchKey const, clang::ast_matchers::internal::(anonymous namespace)::MemoizedMatchResult>>, std::less<clang::ast_matchers::internal::(anonymous namespace)::MatchKey>, std::allocator<std::pair<clang::ast_matchers::internal::(anonymous namespace)::MatchKey const, clang::ast_matchers::internal::(anonymous namespace)::MemoizedMatchResult>>>::_M_erase(std::_Rb_tree_node<std::pair<clang::ast_matchers::internal::(anonymous namespace)::MatchKey const, clang::ast_matchers::internal::(anonymous namespace)::MemoizedMatchResult>>*) ASTMatchFinder.cpp:0:0
#10 0x000055a60f6b4be9 std::_Rb_tree<clang::ast_matchers::internal::(anonymous namespace)::MatchKey, std::pair<clang::ast_matchers::internal::(anonymous namespace)::MatchKey const, clang::ast_matchers::internal::(anonymous namespace)::MemoizedMatchResult>, std::_Select1st<std::pair<clang::ast_matchers::internal::(anonymous namespace)::MatchKey const, clang::ast_matchers::internal::(anonymous namespace)::MemoizedMatchResult>>, std::less<clang::ast_matchers::internal::(anonymous namespace)::MatchKey>, std::allocator<std::pair<clang::ast_matchers::internal::(anonymous namespace)::MatchKey const, clang::ast_matchers::internal::(anonymous namespace)::MemoizedMatchResult>>>::_M_erase(std::_Rb_tree_node<std::pair<clang::ast_matchers::internal::(anonymous namespace)::MatchKey const, clang::ast_matchers::internal::(anonymous namespace)::MemoizedMatchResult>>*) ASTMatchFinder.cpp:0:0
#11 0x000055a60f6b4be9 std::_Rb_tree<clang::ast_matchers::internal::(anonymous namespace)::MatchKey, std::pair<clang::ast_matchers::internal::(anonymous namespace)::MatchKey const, clang::ast_matchers::internal::(anonymous namespace)::MemoizedMatchResult>, std::_Select1st<std::pair<clang::ast_matchers::internal::(anonymous namespace)::MatchKey const, clang::ast_matchers::internal::(anonymous namespace)::MemoizedMatchResult>>, std::less<clang::ast_matchers::internal::(anonymous namespace)::MatchKey>, std::allocator<std::pair<clang::ast_matchers::internal::(anonymous namespace)::MatchKey const, clang::ast_matchers::internal::(anonymous namespace)::MemoizedMatchResult>>>::_M_erase(std::_Rb_tree_node<std::pair<clang::ast_matchers::internal::(anonymous namespace)::MatchKey const, clang::ast_matchers::internal::(anonymous namespace)::MemoizedMatchResult>>*) ASTMatchFinder.cpp:0:0
#12 0x000055a60f6b4be9 std::_Rb_tree<clang::ast_matchers::internal::(anonymous namespace)::MatchKey, std::pair<clang::ast_matchers::internal::(anonymous namespace)::MatchKey const, clang::ast_matchers::internal::(anonymous namespace)::MemoizedMatchResult>, std::_Select1st<std::pair<clang::ast_matchers::internal::(anonymous namespace)::MatchKey const, clang::ast_matchers::internal::(anonymous namespace)::MemoizedMatchResult>>, std::less<clang::ast_matchers::internal::(anonymous namespace)::MatchKey>, std::allocator<std::pair<clang::ast_matchers::internal::(anonymous namespace)::MatchKey const, clang::ast_matchers::internal::(anonymous namespace)::MemoizedMatchResult>>>::_M_erase(std::_Rb_tree_node<std::pair<clang::ast_matchers::internal::(anonymous namespace)::MatchKey const, clang::ast_matchers::internal::(anonymous namespace)::MemoizedMatchResult>>*) ASTMatchFinder.cpp:0:0
#13 0x000055a60f6b4be9 std::_Rb_tree<clang::ast_matchers::internal::(anonymous namespace)::MatchKey, std::pair<clang::ast_matchers::internal::(anonymous namespace)::MatchKey const, clang::ast_matchers::internal::(anonymous namespace)::MemoizedMatchResult>, std::_Select1st<std::pair<clang::ast_matchers::internal::(anonymous namespace)::MatchKey const, clang::ast_matchers::internal::(anonymous namespace)::MemoizedMatchResult>>, std::less<clang::ast_matchers::internal::(anonymous namespace)::MatchKey>, std::allocator<std::pair<clang::ast_matchers::internal::(anonymous namespace)::MatchKey const, clang::ast_matchers::internal::(anonymous namespace)::MemoizedMatchResult>>>::_M_erase(std::_Rb_tree_node<std::pair<clang::ast_matchers::internal::(anonymous namespace)::MatchKey const, clang::ast_matchers::internal::(anonymous namespace)::MemoizedMatchResult>>*) ASTMatchFinder.cpp:0:0
#14 0x000055a60f6b3289 clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor::matchesChildOf(clang::DynTypedNode const&, clang::ASTContext&, clang::ast_matchers::internal::DynTypedMatcher const&, clang::ast_matchers::internal::BoundNodesTreeBuilder*, clang::ast_matchers::internal::ASTMatchFinder::BindKind) ASTMatchFinder.cpp:0:0
#15 0x000055a60f945e5d clang::ast_matchers::internal::ForEachMatcher<clang::Stmt, clang::Stmt>::matches(clang::Stmt const&, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const UseAfterMoveCheck.cpp:0:0
#16 0x000055a60f70fa86 clang::ast_matchers::internal::DynTypedMatcher::matchesNoKindCheck(clang::DynTypedNode const&, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const (/usr/bin/clang-tidy+0x21eaa86)
#17 0x000055a60f713cb2 clang::ast_matchers::internal::(anonymous namespace)::VariadicMatcher<&clang::ast_matchers::internal::allOfVariadicOperator(clang::DynTypedNode const&, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*, llvm::ArrayRef<clang::ast_matchers::internal::DynTypedMatcher>)>::dynMatches(clang::DynTypedNode const&, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const ASTMatchersInternal.cpp:0:0
#18 0x000055a60f71273e clang::ast_matchers::internal::(anonymous namespace)::IdDynMatcher::dynMatches(clang::DynTypedNode const&, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const ASTMatchersInternal.cpp:0:0
#19 0x000055a60f70f975 clang::ast_matchers::internal::DynTypedMatcher::matches(clang::DynTypedNode const&, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const (/usr/bin/clang-tidy+0x21ea975)
#20 0x000055a60f6de5dc clang::ast_matchers::internal::TraversalMatcher<clang::Stmt>::matches(clang::Stmt const&, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const ASTMatchFinder.cpp:0:0
#21 0x000055a60f70f975 clang::ast_matchers::internal::DynTypedMatcher::matches(clang::DynTypedNode const&, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const (/usr/bin/clang-tidy+0x21ea975)
#22 0x000055a60f6dc995 clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor::matchWithFilter(clang::DynTypedNode const&) ASTMatchFinder.cpp:0:0
#23 0x000055a60f6f30fa clang::RecursiveASTVisitor<clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor>::TraverseMemberExpr(clang::MemberExpr*, llvm::SmallVectorImpl<llvm::PointerIntPair<clang::Stmt*, 1u, bool, llvm::PointerLikeTypeTraits<clang::Stmt*>, llvm::PointerIntPairInfo<clang::Stmt*, 1u, llvm::PointerLikeTypeTraits<clang::Stmt*>>>>*) ASTMatchFinder.cpp:0:0
#24 0x000055a60f6ecfd8 clang::RecursiveASTVisitor<clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor>::TraverseStmt(clang::Stmt*, llvm::SmallVectorImpl<llvm::PointerIntPair<clang::Stmt*, 1u, bool, llvm::PointerLikeTypeTraits<clang::Stmt*>, llvm::PointerIntPairInfo<clang::Stmt*, 1u, llvm::PointerLikeTypeTraits<clang::Stmt*>>>>*) ASTMatchFinder.cpp:0:0
#25 0x000055a60f70e41c clang::RecursiveASTVisitor<clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor>::TraverseFunctionHelper(clang::FunctionDecl*) ASTMatchFinder.cpp:0:0
#26 0x000055a60f6e27d3 clang::RecursiveASTVisitor<clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor>::TraverseCXXMethodDecl(clang::CXXMethodDecl*) ASTMatchFinder.cpp:0:0
#27 0x000055a60f6df529 clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor::TraverseDecl(clang::Decl*) ASTMatchFinder.cpp:0:0
#28 0x000055a60f6e4a1b clang::RecursiveASTVisitor<clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor>::TraverseCXXRecordDecl(clang::CXXRecordDecl*) ASTMatchFinder.cpp:0:0
#29 0x000055a60f6dfa51 clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor::TraverseDecl(clang::Decl*) ASTMatchFinder.cpp:0:0
#30 0x000055a60f6e1c5b clang::RecursiveASTVisitor<clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor>::TraverseNamespaceDecl(clang::NamespaceDecl*) ASTMatchFinder.cpp:0:0
#31 0x000055a60f6df751 clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor::TraverseDecl(clang::Decl*) ASTMatchFinder.cpp:0:0
#32 0x000055a60f6dfcab clang::RecursiveASTVisitor<clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor>::TraverseTranslationUnitDecl(clang::TranslationUnitDecl*) ASTMatchFinder.cpp:0:0
#33 0x000055a60f6df3a1 clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor::TraverseDecl(clang::Decl*) ASTMatchFinder.cpp:0:0
#34 0x000055a60f6b1d8c clang::ast_matchers::MatchFinder::matchAST(clang::ASTContext&) (/usr/bin/clang-tidy+0x218cd8c)
#35 0x000055a6104818ac clang::MultiplexConsumer::HandleTranslationUnit(clang::ASTContext&) (/usr/bin/clang-tidy+0x2f5c8ac)
#36 0x000055a610675fe9 clang::ParseAST(clang::Sema&, bool, bool) (/usr/bin/clang-tidy+0x3150fe9)
#37 0x000055a6103fb19d clang::FrontendAction::Execute() (/usr/bin/clang-tidy+0x2ed619d)
#38 0x000055a610362e64 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/usr/bin/clang-tidy+0x2e3de64)
#39 0x000055a60ef273a6 clang::tooling::FrontendActionFactory::runInvocation(std::shared_ptr<clang::CompilerInvocation>, clang::FileManager*, std::shared_ptr<clang::PCHContainerOperations>, clang::DiagnosticConsumer*) (/usr/bin/clang-tidy+0x1a023a6)
#40 0x000055a60ef90d4b clang::tidy::runClangTidy(clang::tidy::ClangTidyContext&, clang::tooling::CompilationDatabase const&, llvm::ArrayRef<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>>, llvm::IntrusiveRefCntPtr<llvm::vfs::OverlayFileSystem>, bool, bool, llvm::StringRef)::ActionFactory::runInvocation(std::shared_ptr<clang::CompilerInvocation>, clang::FileManager*, std::shared_ptr<clang::PCHContainerOperations>, clang::DiagnosticConsumer*) ClangTidy.cpp:0:0
#41 0x000055a60ef27101 clang::tooling::ToolInvocation::runInvocation(char const*, clang::driver::Compilation*, std::shared_ptr<clang::CompilerInvocation>, std::shared_ptr<clang::PCHContainerOperations>) (/usr/bin/clang-tidy+0x1a02101)
#42 0x000055a60ef25c2d clang::tooling::ToolInvocation::run() (/usr/bin/clang-tidy+0x1a00c2d)
#43 0x000055a60ef29297 clang::tooling::ClangTool::run(clang::tooling::ToolAction*) (/usr/bin/clang-tidy+0x1a04297)
#44 0x000055a60ef8ad64 clang::tidy::runClangTidy(clang::tidy::ClangTidyContext&, clang::tooling::CompilationDatabase const&, llvm::ArrayRef<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>>, llvm::IntrusiveRefCntPtr<llvm::vfs::OverlayFileSystem>, bool, bool, llvm::StringRef) (/usr/bin/clang-tidy+0x1a65d64)
#45 0x000055a60ee6720d clang::tidy::clangTidyMain(int, char const**) (/usr/bin/clang-tidy+0x194220d)
#46 0x00007f38c2536338 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#47 0x00007f38c25363fb call_init ./csu/../csu/libc-start.c:128:20
#48 0x00007f38c25363fb __libc_start_main ./csu/../csu/libc-start.c:347:5
#49 0x000055a60ee621e5 _start (/usr/bin/clang-tidy+0x193d1e5)
Bus error```

```
0.      Program arguments: /usr/bin/clang-tidy -p /home/dev/kwantcentral/analytics/out/build/clang-tidy -header-filter=/home/dev/kwantcentral/analytics/src -warnings-as-errors=* --extra-arg-before=--driver-mode=g++ 
1.      <eof> parser at end of file
2.      ASTMatcher: Matching 'cppcoreguidelines-pro-bounds-array-to-pointer-decay' against:
        ImplicitCastExpr : </home/dev/kwantcentral/analytics/out/build/clang-tidy/vcpkg_installed/x64-linux/include/boost/asio/buffer.hpp:2540:36>
malloc(): invalid next->prev_inuse (unsorted)
Subprocess aborted
ninja: build stopped: subcommand failed.```

```
0.      Program arguments: /usr/bin/clang-tidy -p /home/dev/kwantcentral/analytics/out/build/clang-tidy -header-filter=/home/dev/kwantcentral/analytics/src -warnings-as-errors=* --extra-arg-before=--driver-mode=g++ /home/dev/kwantcentral/analytics/src/lib/database-service/kRing.cpp
1.      <eof> parser at end of file
2.      ASTMatcher: Matching 'cppcoreguidelines-pro-bounds-array-to-pointer-decay' against:
        ImplicitCastExpr : </home/dev/kwantcentral/analytics/out/build/clang-tidy/vcpkg_installed/x64-linux/include/boost/asio/detail/scoped_ptr.hpp:57:13>
 #0 0x00005626b2307ad8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/usr/bin/clang-tidy+0x1974ad8)
 #1 0x00005626b23057a5 llvm::sys::RunSignalHandlers() (/usr/bin/clang-tidy+0x19727a5)
 #2 0x00005626b2308471 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x00007f00243f7810 (/lib/x86_64-linux-gnu/libc.so.6+0x45810)
 #4 0x00007f0024462247 unlink_chunk ./malloc/malloc.c:1618:6
 #5 0x00007f002446247b malloc_consolidate ./malloc/malloc.c:4933:6
 #6 0x00007f0024464238 _int_malloc ./malloc/malloc.c:4099:9
 #7 0x00007f00244662d2 __libc_malloc ./malloc/malloc.c:3407:7
 #8 0x00007f00246c08c4 operator new(unsigned long) (/lib/x86_64-linux-gnu/libstdc++.so.6+0xc18c4)
 #9 0x00005626b2b216be clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor::matchesAncestorOf(clang::DynTypedNode const&, clang::ASTContext&, clang::ast_matchers::internal::DynTypedMatcher const&, clang::ast_matchers::internal::BoundNodesTreeBuilder*, clang::ast_matchers::internal::ASTMatchFinder::AncestorMatchMode) ASTMatchFinder.cpp:0:0
#10 0x00005626b2dade8a clang::ast_matchers::internal::HasAncestorMatcher<clang::Stmt, clang::Stmt>::matches(clang::Stmt const&, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const UseAfterMoveCheck.cpp:0:0
#11 0x00005626b2b7d975 clang::ast_matchers::internal::DynTypedMatcher::matches(clang::DynTypedNode const&, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const (/usr/bin/clang-tidy+0x21ea975)
#12 0x00005626b2df00db clang::tidy::cppcoreguidelines::(anonymous namespace)::internal::matcher_isInsideOfRangeBeginEndStmtMatcher::matches(clang::Stmt const&, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const ProBoundsArrayToPointerDecayCheck.cpp:0:0
#13 0x00005626b2b7d975 clang::ast_matchers::internal::DynTypedMatcher::matches(clang::DynTypedNode const&, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const (/usr/bin/clang-tidy+0x21ea975)
#14 0x00005626b2b826c5 clang::ast_matchers::internal::(anonymous namespace)::VariadicMatcher<&clang::ast_matchers::internal::notUnaryOperator(clang::DynTypedNode const&, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*, llvm::ArrayRef<clang::ast_matchers::internal::DynTypedMatcher>)>::dynMatches(clang::DynTypedNode const&, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const ASTMatchersInternal.cpp:0:0
#15 0x00005626b2b7da86 clang::ast_matchers::internal::DynTypedMatcher::matchesNoKindCheck(clang::DynTypedNode const&, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const (/usr/bin/clang-tidy+0x21eaa86)
#16 0x00005626b2b81c3e clang::ast_matchers::internal::(anonymous namespace)::VariadicMatcher<&clang::ast_matchers::internal::allOfVariadicOperator(clang::DynTypedNode const&, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*, llvm::ArrayRef<clang::ast_matchers::internal::DynTypedMatcher>)>::dynMatches(clang::DynTypedNode const&, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const ASTMatchersInternal.cpp:0:0
#17 0x00005626b2b8073e clang::ast_matchers::internal::(anonymous namespace)::IdDynMatcher::dynMatches(clang::DynTypedNode const&, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const ASTMatchersInternal.cpp:0:0
#18 0x00005626b2b7d975 clang::ast_matchers::internal::DynTypedMatcher::matches(clang::DynTypedNode const&, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const (/usr/bin/clang-tidy+0x21ea975)
#19 0x00005626b2b4c5dc clang::ast_matchers::internal::TraversalMatcher<clang::Stmt>::matches(clang::Stmt const&, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const ASTMatchFinder.cpp:0:0
#20 0x00005626b2b7d975 clang::ast_matchers::internal::DynTypedMatcher::matches(clang::DynTypedNode const&, clang::ast_matchers::internal::ASTMatchFinder*, clang::ast_matchers::internal::BoundNodesTreeBuilder*) const (/usr/bin/clang-tidy+0x21ea975)
#21 0x00005626b2b4a995 clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor::matchWithFilter(clang::DynTypedNode const&) ASTMatchFinder.cpp:0:0
#22 0x00005626b2b5d19e clang::RecursiveASTVisitor<clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor>::TraverseUnaryOperator(clang::UnaryOperator*, llvm::SmallVectorImpl<llvm::PointerIntPair<clang::Stmt*, 1u, bool, llvm::PointerLikeTypeTraits<clang::Stmt*>, llvm::PointerIntPairInfo<clang::Stmt*, 1u, llvm::PointerLikeTypeTraits<clang::Stmt*>>>>*) ASTMatchFinder.cpp:0:0
#23 0x00005626b2b5af7e clang::RecursiveASTVisitor<clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor>::TraverseStmt(clang::Stmt*, llvm::SmallVectorImpl<llvm::PointerIntPair<clang::Stmt*, 1u, bool, llvm::PointerLikeTypeTraits<clang::Stmt*>, llvm::PointerIntPairInfo<clang::Stmt*, 1u, llvm::PointerLikeTypeTraits<clang::Stmt*>>>>*) ASTMatchFinder.cpp:0:0
#24 0x00005626b2b7c41c clang::RecursiveASTVisitor<clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor>::TraverseFunctionHelper(clang::FunctionDecl*) ASTMatchFinder.cpp:0:0
#25 0x00005626b2b507d3 clang::RecursiveASTVisitor<clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor>::TraverseCXXMethodDecl(clang::CXXMethodDecl*) ASTMatchFinder.cpp:0:0
#26 0x00005626b2b4d529 clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor::TraverseDecl(clang::Decl*) ASTMatchFinder.cpp:0:0
#27 0x00005626b2b52a1b clang::RecursiveASTVisitor<clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor>::TraverseCXXRecordDecl(clang::CXXRecordDecl*) ASTMatchFinder.cpp:0:0
#28 0x00005626b2b4da51 clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor::TraverseDecl(clang::Decl*) ASTMatchFinder.cpp:0:0
#29 0x00005626b2b54111 clang::RecursiveASTVisitor<clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor>::TraverseClassTemplateDecl(clang::ClassTemplateDecl*) ASTMatchFinder.cpp:0:0
#30 0x00005626b2b4daf9 clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor::TraverseDecl(clang::Decl*) ASTMatchFinder.cpp:0:0
#31 0x00005626b2b4fc5b clang::RecursiveASTVisitor<clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor>::TraverseNamespaceDecl(clang::NamespaceDecl*) ASTMatchFinder.cpp:0:0
#32 0x00005626b2b4d751 clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor::TraverseDecl(clang::Decl*) ASTMatchFinder.cpp:0:0
#33 0x00005626b2b4fc5b clang::RecursiveASTVisitor<clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor>::TraverseNamespaceDecl(clang::NamespaceDecl*) ASTMatchFinder.cpp:0:0
#34 0x00005626b2b4d751 clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor::TraverseDecl(clang::Decl*) ASTMatchFinder.cpp:0:0
#35 0x00005626b2b4fc5b clang::RecursiveASTVisitor<clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor>::TraverseNamespaceDecl(clang::NamespaceDecl*) ASTMatchFinder.cpp:0:0
#36 0x00005626b2b4d751 clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor::TraverseDecl(clang::Decl*) ASTMatchFinder.cpp:0:0
#37 0x00005626b2b4dcab clang::RecursiveASTVisitor<clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor>::TraverseTranslationUnitDecl(clang::TranslationUnitDecl*) ASTMatchFinder.cpp:0:0
#38 0x00005626b2b4d3a1 clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor::TraverseDecl(clang::Decl*) ASTMatchFinder.cpp:0:0
#39 0x00005626b2b1fd8c clang::ast_matchers::MatchFinder::matchAST(clang::ASTContext&) (/usr/bin/clang-tidy+0x218cd8c)
#40 0x00005626b38ef8ac clang::MultiplexConsumer::HandleTranslationUnit(clang::ASTContext&) (/usr/bin/clang-tidy+0x2f5c8ac)
#41 0x00005626b3ae3fe9 clang::ParseAST(clang::Sema&, bool, bool) (/usr/bin/clang-tidy+0x3150fe9)
#42 0x00005626b386919d clang::FrontendAction::Execute() (/usr/bin/clang-tidy+0x2ed619d)
#43 0x00005626b37d0e64 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/usr/bin/clang-tidy+0x2e3de64)
#44 0x00005626b23953a6 clang::tooling::FrontendActionFactory::runInvocation(std::shared_ptr<clang::CompilerInvocation>, clang::FileManager*, std::shared_ptr<clang::PCHContainerOperations>, clang::DiagnosticConsumer*) (/usr/bin/clang-tidy+0x1a023a6)
#45 0x00005626b23fed4b clang::tidy::runClangTidy(clang::tidy::ClangTidyContext&, clang::tooling::CompilationDatabase const&, llvm::ArrayRef<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>>, llvm::IntrusiveRefCntPtr<llvm::vfs::OverlayFileSystem>, bool, bool, llvm::StringRef)::ActionFactory::runInvocation(std::shared_ptr<clang::CompilerInvocation>, clang::FileManager*, std::shared_ptr<clang::PCHContainerOperations>, clang::DiagnosticConsumer*) ClangTidy.cpp:0:0
#46 0x00005626b2395101 clang::tooling::ToolInvocation::runInvocation(char const*, clang::driver::Compilation*, std::shared_ptr<clang::CompilerInvocation>, std::shared_ptr<clang::PCHContainerOperations>) (/usr/bin/clang-tidy+0x1a02101)
#47 0x00005626b2393c2d clang::tooling::ToolInvocation::run() (/usr/bin/clang-tidy+0x1a00c2d)
#48 0x00005626b2397297 clang::tooling::ClangTool::run(clang::tooling::ToolAction*) (/usr/bin/clang-tidy+0x1a04297)
#49 0x00005626b23f8d64 clang::tidy::runClangTidy(clang::tidy::ClangTidyContext&, clang::tooling::CompilationDatabase const&, llvm::ArrayRef<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>>, llvm::IntrusiveRefCntPtr<llvm::vfs::OverlayFileSystem>, bool, bool, llvm::StringRef) (/usr/bin/clang-tidy+0x1a65d64)
#50 0x00005626b22d520d clang::tidy::clangTidyMain(int, char const**) (/usr/bin/clang-tidy+0x194220d)
#51 0x00007f00243dc338 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#52 0x00007f00243dc3fb call_init ./csu/../csu/libc-start.c:128:20
#53 0x00007f00243dc3fb __libc_start_main ./csu/../csu/libc-start.c:347:5
#54 0x00005626b22d01e5 _start (/usr/bin/clang-tidy+0x193d1e5)
Segmentation fault```


---

### Comment 27 - tonygould

Is there anything more I can do on this issue? (I will shortly be unable to do more)

In summary, building with debug asserts didn't show anything, sanitizers did, and there's a minimal reproducible example in the post above , using publicly available includes.

---

### Comment 28 - AaronBallman

> Is there anything more I can do on this issue? (I will shortly be unable to do more)

It would be super helpful if you were able to reduce the bug down to just the code needed to cause the crash without any includes. Each include brings in a *ton* of extra source code which is often entirely unrelated to the crash but complicates our ability to step through the logic in a debugger to get to the root cause of the issue. Removing the includes and getting it down to just the bit of code that's tripping up the compiler can save us a ton of time. (There are tools which can help with this, like creduce, but it can still be a bit of a slog so it's fine if you're not able to reduce the issue further.)

---

### Comment 29 - 5chmidti

Beyond the lib versions of the libraries included, I don't think there is more you can help with within the short amount of time. You've already provided reproducer snippets, thanks for narrowing things down so far, so to look into this we just need to experiment a bit to reproduce this. The backtraces help with narrowing things down (could be an assumption in the matchers is broken? But then it shouldn't be so flaky...).

---

### Comment 30 - tonygould

OK, I'm not sure I can cut down the code beyond the includes. The complexity of the includes are intrinsic to the crashes, I think. But I'll see if I can reduce the number of includes. It's a combinatorial problem, obviously, but we do include nlohmann json in other translation units so it may be that this is independent of the issue and can be removed.

Related, is https://github.com/llvm/llvm-project/pull/128150. I know strictly speaking it's working around the problem that is buried in 100s of ks of boost code included in my translation unit, but if we could turn matchers off for system header files somehow, it would not only have a hugely positive  impact on integrating clang-tidy with medium to large projects, it would also likely make stability issues of the kind I'm seeing go from 2-3 sigma to 3-4 sigma frequency. Is there any way I can turn on @carlosgalvezp 's changes on latest llvm code that I'm building and see if it fixes the stability issues I'm seeing?

---

