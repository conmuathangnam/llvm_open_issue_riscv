# Linter crashes with file including <rerun.hpp>

**Author:** stijn-vanesch
**URL:** https://github.com/llvm/llvm-project/issues/162994
**Status:** Closed
**Labels:** clang-tidy, crash
**Closed Date:** 2025-10-13T13:20:09Z

## Body

The linter crashes during linting code with the include: `#include <rerun.hpp>`.

The version of rerun being used: `librerun-sdk-0.24.1`.

The following backtrace is the result:

```
1.	<eof> parser at end of file
2.	ASTMatcher: Processing 'hicpp-move-const-arg' against:
	CXXConstructExpr : </home/my_project/.pixi/envs/jazzy/include/rerun/archetypes/../result.hpp:34:51, col:74>
--- Bound Nodes Begin ---
    call-move - { CallExpr : </home/my_project/.pixi/envs/jazzy/include/rerun/archetypes/../result.hpp:34:57, col:73> }
    invocation-parm - { ParmVarDecl status : </home/my_project/.pixi/envs/jazzy/include/rerun/archetypes/../error.hpp:117:15, col:36> }
    invocation-parm-type - { QualType : const arrow::Status & }
    receiving-expr - { CXXConstructExpr : </home/my_project/.pixi/envs/jazzy/include/rerun/archetypes/../result.hpp:34:51, col:74> }
--- Bound Nodes End ---
 #0 0x00007b604db2947f llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/home/my_project/.pixi/envs/jazzy/bin/../lib/libLLVM.so.19.1+0xf2947f)
 #1 0x00007b604db26a4b SignalHandler(int) Signals.cpp:0:0
 #2 0x00007b604c842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x000056f13072dcf8 clang::tidy::performance::MoveConstArgCheck::check(clang::ast_matchers::MatchFinder::MatchResult const&) (/home/my_project/.pixi/envs/jazzy/bin/clang-tidy+0x7a1cf8)
 #4 0x00007b605722c26b clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor::MatchVisitor::visitMatch(clang::ast_matchers::BoundNodes const&) ASTMatchFinder.cpp:0:0
 #5 0x00007b605725d591 clang::ast_matchers::internal::BoundNodesTreeBuilder::visitMatches(clang::ast_matchers::internal::BoundNodesTreeBuilder::Visitor*) (/home/my_project/.pixi/envs/jazzy/bin/../lib/libclang-cpp.so.19.1+0x165d591)
 #6 0x00007b605722d6a5 clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor::matchWithFilter(clang::DynTypedNode const&) ASTMatchFinder.cpp:0:0
 #7 0x00007b605724c50f clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor::TraverseStmt(clang::Stmt*, llvm::SmallVectorImpl<llvm::PointerIntPair<clang::Stmt*, 1u, bool, llvm::PointerLikeTypeTraits<clang::Stmt*>, llvm::PointerIntPairInfo<clang::Stmt*, 1u, llvm::PointerLikeTypeTraits<clang::Stmt*>>>>*) (.constprop.0) ASTMatchFinder.cpp:0:0
 #8 0x00007b6057250f19 clang::RecursiveASTVisitor<clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor>::TraverseFunctionHelper(clang::FunctionDecl*) ASTMatchFinder.cpp:0:0
 #9 0x00007b6057255d53 clang::RecursiveASTVisitor<clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor>::TraverseCXXConstructorDecl(clang::CXXConstructorDecl*) ASTMatchFinder.cpp:0:0
#10 0x00007b6057246724 clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor::TraverseDecl(clang::Decl*) ASTMatchFinder.cpp:0:0
#11 0x00007b605724682a clang::RecursiveASTVisitor<clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor>::TraverseDeclContextHelper(clang::DeclContext*) ASTMatchFinder.cpp:0:0
#12 0x00007b6057245502 clang::RecursiveASTVisitor<clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor>::TraverseDecl(clang::Decl*) ASTMatchFinder.cpp:0:0
#13 0x00007b6057246724 clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor::TraverseDecl(clang::Decl*) ASTMatchFinder.cpp:0:0
#14 0x00007b605725679c clang::RecursiveASTVisitor<clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor>::TraverseClassTemplateDecl(clang::ClassTemplateDecl*) ASTMatchFinder.cpp:0:0
#15 0x00007b6057246724 clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor::TraverseDecl(clang::Decl*) ASTMatchFinder.cpp:0:0
#16 0x00007b605724682a clang::RecursiveASTVisitor<clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor>::TraverseDeclContextHelper(clang::DeclContext*) ASTMatchFinder.cpp:0:0
#17 0x00007b6057253a50 clang::RecursiveASTVisitor<clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor>::TraverseNamespaceDecl(clang::NamespaceDecl*) ASTMatchFinder.cpp:0:0
#18 0x00007b6057246724 clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor::TraverseDecl(clang::Decl*) ASTMatchFinder.cpp:0:0
#19 0x00007b605724682a clang::RecursiveASTVisitor<clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor>::TraverseDeclContextHelper(clang::DeclContext*) ASTMatchFinder.cpp:0:0
#20 0x00007b6057255c78 clang::RecursiveASTVisitor<clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor>::TraverseTranslationUnitDecl(clang::TranslationUnitDecl*) ASTMatchFinder.cpp:0:0
#21 0x00007b6057246724 clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor::TraverseDecl(clang::Decl*) ASTMatchFinder.cpp:0:0
#22 0x00007b6057246a7a clang::ast_matchers::MatchFinder::matchAST(clang::ASTContext&) (/home/my_project/.pixi/envs/jazzy/bin/../lib/libclang-cpp.so.19.1+0x1646a7a)
#23 0x00007b6058f0dec8 clang::MultiplexConsumer::HandleTranslationUnit(clang::ASTContext&) (/home/my_project/.pixi/envs/jazzy/bin/../lib/libclang-cpp.so.19.1+0x330dec8)
#24 0x00007b60568809ec clang::ParseAST(clang::Sema&, bool, bool) (/home/my_project/.pixi/envs/jazzy/bin/../lib/libclang-cpp.so.19.1+0xc809ec)
#25 0x00007b6058ecca59 clang::FrontendAction::Execute() (/home/my_project/.pixi/envs/jazzy/bin/../lib/libclang-cpp.so.19.1+0x32cca59)
#26 0x00007b6058e48317 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/home/my_project/.pixi/envs/jazzy/bin/../lib/libclang-cpp.so.19.1+0x3248317)
#27 0x00007b60590fd35b clang::tooling::FrontendActionFactory::runInvocation(std::shared_ptr<clang::CompilerInvocation>, clang::FileManager*, std::shared_ptr<clang::PCHContainerOperations>, clang::DiagnosticConsumer*) (/home/my_project/.pixi/envs/jazzy/bin/../lib/libclang-cpp.so.19.1+0x34fd35b)
#28 0x000056f130943f5e clang::tidy::runClangTidy(clang::tidy::ClangTidyContext&, clang::tooling::CompilationDatabase const&, llvm::ArrayRef<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>>, llvm::IntrusiveRefCntPtr<llvm::vfs::OverlayFileSystem>, bool, bool, llvm::StringRef)::ActionFactory::runInvocation(std::shared_ptr<clang::CompilerInvocation>, clang::FileManager*, std::shared_ptr<clang::PCHContainerOperations>, clang::DiagnosticConsumer*) ClangTidy.cpp:0:0
#29 0x00007b60590f6c46 clang::tooling::ToolInvocation::runInvocation(char const*, clang::driver::Compilation*, std::shared_ptr<clang::CompilerInvocation>, std::shared_ptr<clang::PCHContainerOperations>) (/home/my_project/.pixi/envs/jazzy/bin/../lib/libclang-cpp.so.19.1+0x34f6c46)
#30 0x00007b60590f86b0 clang::tooling::ToolInvocation::run() (/home/my_project/.pixi/envs/jazzy/bin/../lib/libclang-cpp.so.19.1+0x34f86b0)
#31 0x00007b60590fa8b9 clang::tooling::ClangTool::run(clang::tooling::ToolAction*) (/home/my_project/.pixi/envs/jazzy/bin/../lib/libclang-cpp.so.19.1+0x34fa8b9)
#32 0x000056f13094bd7d clang::tidy::runClangTidy(clang::tidy::ClangTidyContext&, clang::tooling::CompilationDatabase const&, llvm::ArrayRef<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>>, llvm::IntrusiveRefCntPtr<llvm::vfs::OverlayFileSystem>, bool, bool, llvm::StringRef) (/home/my_project/.pixi/envs/jazzy/bin/clang-tidy+0x9bfd7d)
#33 0x000056f1301c0dfa clang::tidy::clangTidyMain(int, char const**) (/home/my_project/.pixi/envs/jazzy/bin/clang-tidy+0x234dfa)
#34 0x00007b604c829d90 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:58:16
#35 0x00007b604c829e40 call_init ./csu/../csu/libc-start.c:128:20
#36 0x00007b604c829e40 __libc_start_main ./csu/../csu/libc-start.c:379:5
#37 0x000056f1301b987c _start (/home/my_project/.pixi/envs/jazzy/bin/clang-tidy+0x22d87c)
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-tidy

Author: None (stijn-vanesch)

<details>
The linter crashes during linting code with the include: `#include &lt;rerun.hpp&gt;`.

The version of rerun being used: `librerun-sdk-0.24.1`.

The following backtrace is the result:

```
1.	&lt;eof&gt; parser at end of file
2.	ASTMatcher: Processing 'hicpp-move-const-arg' against:
	CXXConstructExpr : &lt;/home/my_project/.pixi/envs/jazzy/include/rerun/archetypes/../result.hpp:34:51, col:74&gt;
--- Bound Nodes Begin ---
    call-move - { CallExpr : &lt;/home/my_project/.pixi/envs/jazzy/include/rerun/archetypes/../result.hpp:34:57, col:73&gt; }
    invocation-parm - { ParmVarDecl status : &lt;/home/my_project/.pixi/envs/jazzy/include/rerun/archetypes/../error.hpp:117:15, col:36&gt; }
    invocation-parm-type - { QualType : const arrow::Status &amp; }
    receiving-expr - { CXXConstructExpr : &lt;/home/my_project/.pixi/envs/jazzy/include/rerun/archetypes/../result.hpp:34:51, col:74&gt; }
--- Bound Nodes End ---
 #<!-- -->0 0x00007b604db2947f llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/home/my_project/.pixi/envs/jazzy/bin/../lib/libLLVM.so.19.1+0xf2947f)
 #<!-- -->1 0x00007b604db26a4b SignalHandler(int) Signals.cpp:0:0
 #<!-- -->2 0x00007b604c842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->3 0x000056f13072dcf8 clang::tidy::performance::MoveConstArgCheck::check(clang::ast_matchers::MatchFinder::MatchResult const&amp;) (/home/my_project/.pixi/envs/jazzy/bin/clang-tidy+0x7a1cf8)
 #<!-- -->4 0x00007b605722c26b clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor::MatchVisitor::visitMatch(clang::ast_matchers::BoundNodes const&amp;) ASTMatchFinder.cpp:0:0
 #<!-- -->5 0x00007b605725d591 clang::ast_matchers::internal::BoundNodesTreeBuilder::visitMatches(clang::ast_matchers::internal::BoundNodesTreeBuilder::Visitor*) (/home/my_project/.pixi/envs/jazzy/bin/../lib/libclang-cpp.so.19.1+0x165d591)
 #<!-- -->6 0x00007b605722d6a5 clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor::matchWithFilter(clang::DynTypedNode const&amp;) ASTMatchFinder.cpp:0:0
 #<!-- -->7 0x00007b605724c50f clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor::TraverseStmt(clang::Stmt*, llvm::SmallVectorImpl&lt;llvm::PointerIntPair&lt;clang::Stmt*, 1u, bool, llvm::PointerLikeTypeTraits&lt;clang::Stmt*&gt;, llvm::PointerIntPairInfo&lt;clang::Stmt*, 1u, llvm::PointerLikeTypeTraits&lt;clang::Stmt*&gt;&gt;&gt;&gt;*) (.constprop.0) ASTMatchFinder.cpp:0:0
 #<!-- -->8 0x00007b6057250f19 clang::RecursiveASTVisitor&lt;clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor&gt;::TraverseFunctionHelper(clang::FunctionDecl*) ASTMatchFinder.cpp:0:0
 #<!-- -->9 0x00007b6057255d53 clang::RecursiveASTVisitor&lt;clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor&gt;::TraverseCXXConstructorDecl(clang::CXXConstructorDecl*) ASTMatchFinder.cpp:0:0
#<!-- -->10 0x00007b6057246724 clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor::TraverseDecl(clang::Decl*) ASTMatchFinder.cpp:0:0
#<!-- -->11 0x00007b605724682a clang::RecursiveASTVisitor&lt;clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor&gt;::TraverseDeclContextHelper(clang::DeclContext*) ASTMatchFinder.cpp:0:0
#<!-- -->12 0x00007b6057245502 clang::RecursiveASTVisitor&lt;clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor&gt;::TraverseDecl(clang::Decl*) ASTMatchFinder.cpp:0:0
#<!-- -->13 0x00007b6057246724 clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor::TraverseDecl(clang::Decl*) ASTMatchFinder.cpp:0:0
#<!-- -->14 0x00007b605725679c clang::RecursiveASTVisitor&lt;clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor&gt;::TraverseClassTemplateDecl(clang::ClassTemplateDecl*) ASTMatchFinder.cpp:0:0
#<!-- -->15 0x00007b6057246724 clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor::TraverseDecl(clang::Decl*) ASTMatchFinder.cpp:0:0
#<!-- -->16 0x00007b605724682a clang::RecursiveASTVisitor&lt;clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor&gt;::TraverseDeclContextHelper(clang::DeclContext*) ASTMatchFinder.cpp:0:0
#<!-- -->17 0x00007b6057253a50 clang::RecursiveASTVisitor&lt;clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor&gt;::TraverseNamespaceDecl(clang::NamespaceDecl*) ASTMatchFinder.cpp:0:0
#<!-- -->18 0x00007b6057246724 clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor::TraverseDecl(clang::Decl*) ASTMatchFinder.cpp:0:0
#<!-- -->19 0x00007b605724682a clang::RecursiveASTVisitor&lt;clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor&gt;::TraverseDeclContextHelper(clang::DeclContext*) ASTMatchFinder.cpp:0:0
#<!-- -->20 0x00007b6057255c78 clang::RecursiveASTVisitor&lt;clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor&gt;::TraverseTranslationUnitDecl(clang::TranslationUnitDecl*) ASTMatchFinder.cpp:0:0
#<!-- -->21 0x00007b6057246724 clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor::TraverseDecl(clang::Decl*) ASTMatchFinder.cpp:0:0
#<!-- -->22 0x00007b6057246a7a clang::ast_matchers::MatchFinder::matchAST(clang::ASTContext&amp;) (/home/my_project/.pixi/envs/jazzy/bin/../lib/libclang-cpp.so.19.1+0x1646a7a)
#<!-- -->23 0x00007b6058f0dec8 clang::MultiplexConsumer::HandleTranslationUnit(clang::ASTContext&amp;) (/home/my_project/.pixi/envs/jazzy/bin/../lib/libclang-cpp.so.19.1+0x330dec8)
#<!-- -->24 0x00007b60568809ec clang::ParseAST(clang::Sema&amp;, bool, bool) (/home/my_project/.pixi/envs/jazzy/bin/../lib/libclang-cpp.so.19.1+0xc809ec)
#<!-- -->25 0x00007b6058ecca59 clang::FrontendAction::Execute() (/home/my_project/.pixi/envs/jazzy/bin/../lib/libclang-cpp.so.19.1+0x32cca59)
#<!-- -->26 0x00007b6058e48317 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/home/my_project/.pixi/envs/jazzy/bin/../lib/libclang-cpp.so.19.1+0x3248317)
#<!-- -->27 0x00007b60590fd35b clang::tooling::FrontendActionFactory::runInvocation(std::shared_ptr&lt;clang::CompilerInvocation&gt;, clang::FileManager*, std::shared_ptr&lt;clang::PCHContainerOperations&gt;, clang::DiagnosticConsumer*) (/home/my_project/.pixi/envs/jazzy/bin/../lib/libclang-cpp.so.19.1+0x34fd35b)
#<!-- -->28 0x000056f130943f5e clang::tidy::runClangTidy(clang::tidy::ClangTidyContext&amp;, clang::tooling::CompilationDatabase const&amp;, llvm::ArrayRef&lt;std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;&gt;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::OverlayFileSystem&gt;, bool, bool, llvm::StringRef)::ActionFactory::runInvocation(std::shared_ptr&lt;clang::CompilerInvocation&gt;, clang::FileManager*, std::shared_ptr&lt;clang::PCHContainerOperations&gt;, clang::DiagnosticConsumer*) ClangTidy.cpp:0:0
#<!-- -->29 0x00007b60590f6c46 clang::tooling::ToolInvocation::runInvocation(char const*, clang::driver::Compilation*, std::shared_ptr&lt;clang::CompilerInvocation&gt;, std::shared_ptr&lt;clang::PCHContainerOperations&gt;) (/home/my_project/.pixi/envs/jazzy/bin/../lib/libclang-cpp.so.19.1+0x34f6c46)
#<!-- -->30 0x00007b60590f86b0 clang::tooling::ToolInvocation::run() (/home/my_project/.pixi/envs/jazzy/bin/../lib/libclang-cpp.so.19.1+0x34f86b0)
#<!-- -->31 0x00007b60590fa8b9 clang::tooling::ClangTool::run(clang::tooling::ToolAction*) (/home/my_project/.pixi/envs/jazzy/bin/../lib/libclang-cpp.so.19.1+0x34fa8b9)
#<!-- -->32 0x000056f13094bd7d clang::tidy::runClangTidy(clang::tidy::ClangTidyContext&amp;, clang::tooling::CompilationDatabase const&amp;, llvm::ArrayRef&lt;std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;&gt;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::OverlayFileSystem&gt;, bool, bool, llvm::StringRef) (/home/my_project/.pixi/envs/jazzy/bin/clang-tidy+0x9bfd7d)
#<!-- -->33 0x000056f1301c0dfa clang::tidy::clangTidyMain(int, char const**) (/home/my_project/.pixi/envs/jazzy/bin/clang-tidy+0x234dfa)
#<!-- -->34 0x00007b604c829d90 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:58:16
#<!-- -->35 0x00007b604c829e40 call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->36 0x00007b604c829e40 __libc_start_main ./csu/../csu/libc-start.c:379:5
#<!-- -->37 0x000056f1301b987c _start (/home/my_project/.pixi/envs/jazzy/bin/clang-tidy+0x22d87c)
```
</details>


---

### Comment 2 - localspook

This looks like a duplicate of #111450 (the crash in that issue is even on the same rerun code). That issue was fixed in clang-tidy 20; are you using an earlier version? If so, can you try testing with a newer one?

---

### Comment 3 - vbvictor

By `libLLVM.so.19.1` I assume it's clang-tidy-19 indeed

---

### Comment 4 - stijn-vanesch

Indeed! Using clang-tidy version 20 has solved this crash.

---

