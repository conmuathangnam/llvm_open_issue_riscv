# [analyzer] Support C++26 structured-binding packs

**Author:** pozemka
**URL:** https://github.com/llvm/llvm-project/issues/182346
**Status:** Open
**Labels:** clang:static analyzer, crash, c++26

## Body

Issue is occuring when analyzing file using compilation database. But without it clang-tidy only complains about missing include files.

`clang-tidy --config-file=../../.clang-tidy -p ../../build/Desktop_clang-Debug  party_in_quest_location2.cpp`

```
Stack dump:
0.      Program arguments: clang-tidy --config-file=../../.clang-tidy -p ../../build/Desktop_clang-Debug party_in_quest_location2.cpp
1.      <eof> parser at end of file
2.      While analyzing stack: 
        #0 Calling boost::pfr::detail::for_each_field_impl(const struct CDS::Attributes &, class (lambda at /home/uni/projects/craftsdwarfship/systems/heroes-ai/party_in_quest_location2.cpp:144:43) &&, std::index_sequence<0UL, 1UL, 2UL, 3UL>, struct std::is_rvalue_reference<const struct CDS::Attributes &>) at line /home/uni/projects/craftsdwarfship/build/Desktop_clang-Debug/vcpkg_installed/x64-linux/include/boost/pfr/detail/for_each_field.hpp:30:9
        #1 Calling boost::pfr::detail::for_each_field(const struct CDS::Attributes &, class (lambda at /home/uni/projects/craftsdwarfship/systems/heroes-ai/party_in_quest_location2.cpp:144:43) &&) at line /home/uni/projects/craftsdwarfship/build/Desktop_clang-Debug/vcpkg_installed/x64-linux/include/boost/pfr/core.hpp:258:12
        #2 Calling boost::pfr::for_each_field(const struct CDS::Attributes &, class (lambda at /home/uni/projects/craftsdwarfship/systems/heroes-ai/party_in_quest_location2.cpp:144:43) &&) at line 144
        #3 Calling CDS::(anonymous namespace)::rewardFight(entt::registry &, const GameState &, SystemContext &, std::span<const entt::entity>, const CombatSimulationState &)::(anonymous class)::operator()(const entt::entity &) at line /usr/bin/../lib64/gcc/x86_64-suse-linux/15/../../../../include/c++/15/bits/invoke.h:63:14
        #4 Calling std::__invoke_impl(__invoke_other, class (lambda at /home/uni/projects/craftsdwarfship/systems/heroes-ai/party_in_quest_location2.cpp:142:31) &, const enum entt::entity &) at line /usr/bin/../lib64/gcc/x86_64-suse-linux/15/../../../../include/c++/15/bits/invoke.h:98:14
        #5 Calling std::__invoke(class (lambda at /home/uni/projects/craftsdwarfship/systems/heroes-ai/party_in_quest_location2.cpp:142:31) &, const enum entt::entity &) at line /usr/bin/../lib64/gcc/x86_64-suse-linux/15/../../../../include/c++/15/bits/ranges_algo.h:190:4
        #6 Calling std::ranges::__for_each_fn::operator()(class __gnu_cxx::__normal_iterator<const enum entt::entity *, class std::vector<enum entt::entity> >, class __gnu_cxx::__normal_iterator<const enum entt::entity *, class std::vector<enum entt::entity> >, class (lambda at /home/uni/projects/craftsdwarfship/systems/heroes-ai/party_in_quest_location2.cpp:142:31), struct std::identity) at line /usr/bin/../lib64/gcc/x86_64-suse-linux/15/../../../../include/c++/15/bits/ranges_algo.h:200:9
        #7 Calling std::ranges::__for_each_fn::operator()(const class std::vector<enum entt::entity> &, class (lambda at /home/uni/projects/craftsdwarfship/systems/heroes-ai/party_in_quest_location2.cpp:142:31), struct std::identity) at line 146
        #8 Calling CDS::(anonymous namespace)::rewardFight(entt::registry &, const GameState &, SystemContext &, std::span<const entt::entity>, const CombatSimulationState &)
3.      /home/uni/projects/craftsdwarfship/build/Desktop_clang-Debug/vcpkg_installed/x64-linux/include/boost/pfr/detail/for_each_field_impl.hpp:47:18: Error evaluating statement
4.      /home/uni/projects/craftsdwarfship/build/Desktop_clang-Debug/vcpkg_installed/x64-linux/include/boost/pfr/detail/for_each_field_impl.hpp:47:18: Error evaluating statement
 #0 0x00007fead48fef39 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/lib64/libLLVM.so.21.1+0x6fef39)
 #1 0x00007fead48fbe13 llvm::sys::RunSignalHandlers() (/lib64/libLLVM.so.21.1+0x6fbe13)
 #2 0x00007fead48ff718 (/lib64/libLLVM.so.21.1+0x6ff718)
 #3 0x00007fead3a42910 __restore_rt (/lib64/libc.so.6+0x42910)
 #4 0x00007feadedd758e clang::ento::ExprEngine::VisitCommonDeclRefExpr(clang::Expr const*, clang::NamedDecl const*, clang::ento::ExplodedNode*, clang::ento::ExplodedNodeSet&) (/lib64/libclang-cpp.so.21.1+0x29d758e)
 #5 0x00007feadedd0d2a clang::ento::ExprEngine::Visit(clang::Stmt const*, clang::ento::ExplodedNode*, clang::ento::ExplodedNodeSet&) (/lib64/libclang-cpp.so.21.1+0x29d0d2a)
 #6 0x00007feadedcdb69 clang::ento::ExprEngine::ProcessStmt(clang::Stmt const*, clang::ento::ExplodedNode*) (/lib64/libclang-cpp.so.21.1+0x29cdb69)
 #7 0x00007feadedaf086 clang::ento::CoreEngine::dispatchWorkItem(clang::ento::ExplodedNode*, clang::ProgramPoint, clang::ento::WorkListUnit const&) (/lib64/libclang-cpp.so.21.1+0x29af086)
 #8 0x00007feadedae93e clang::ento::CoreEngine::ExecuteWorkList(clang::LocationContext const*, unsigned int, llvm::IntrusiveRefCntPtr<clang::ento::ProgramState const>) (/lib64/libclang-cpp.so.21.1+0x29ae93e)
 #9 0x00007feadf0f6fc0 (/lib64/libclang-cpp.so.21.1+0x2cf6fc0)
#10 0x00007feadf0f5331 (/lib64/libclang-cpp.so.21.1+0x2cf5331)
#11 0x00007feadeabbfec clang::MultiplexConsumer::HandleTranslationUnit(clang::ASTContext&) (/lib64/libclang-cpp.so.21.1+0x26bbfec)
#12 0x00007feadcac5fd9 clang::ParseAST(clang::Sema&, bool, bool) (/lib64/libclang-cpp.so.21.1+0x6c5fd9)
#13 0x00007feadea785fd clang::FrontendAction::Execute() (/lib64/libclang-cpp.so.21.1+0x26785fd)
#14 0x00007feade9bd22e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/lib64/libclang-cpp.so.21.1+0x25bd22e)
#15 0x00007feadecbc0fe clang::tooling::FrontendActionFactory::runInvocation(std::shared_ptr<clang::CompilerInvocation>, clang::FileManager*, std::shared_ptr<clang::PCHContainerOperations>, clang::DiagnosticConsumer*) (/lib64/libclang-cpp.so.21.1+0x28bc0fe)
#16 0x000056167c5b835c (/usr/bin/clang-tidy-21+0x74935c)
#17 0x00007feadecbbd9d clang::tooling::ToolInvocation::runInvocation(char const*, clang::driver::Compilation*, std::shared_ptr<clang::CompilerInvocation>, std::shared_ptr<clang::PCHContainerOperations>) (/lib64/libclang-cpp.so.21.1+0x28bbd9d)
#18 0x00007feadecba9dc clang::tooling::ToolInvocation::run() (/lib64/libclang-cpp.so.21.1+0x28ba9dc)
#19 0x00007feadecbdf32 clang::tooling::ClangTool::run(clang::tooling::ToolAction*) (/lib64/libclang-cpp.so.21.1+0x28bdf32)
#20 0x000056167c5b3c36 clang::tidy::runClangTidy(clang::tidy::ClangTidyContext&, clang::tooling::CompilationDatabase const&, llvm::ArrayRef<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>>, llvm::IntrusiveRefCntPtr<llvm::vfs::OverlayFileSystem>, bool, bool, llvm::StringRef) (/usr/bin/clang-tidy-21+0x744c36)
#21 0x000056167bf3d3de clang::tidy::clangTidyMain(int, char const**) (/usr/bin/clang-tidy-21+0xce3de)
#22 0x00007fead3a2b2fb __libc_start_call_main (/lib64/libc.so.6+0x2b2fb)
#23 0x00007fead3a2b3cb __libc_start_main@GLIBC_2.2.5 (/lib64/libc.so.6+0x2b3cb)
#24 0x000056167bf39dc5 _start (/usr/bin/clang-tidy-21+0xcadc5)
/home/uni/projects/craftsdwarfship/systems/heroes-ai/party_in_quest_location2.cpp: terminated by signal 11
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-tidy

Author: Mikhail (pozemka)

<details>
```
Stack dump:
0.      Program arguments: /usr/bin/clang-tidy -p=/home/uni/projects/craftsdwarfship/build/Desktop_clang-Debug --config-file=../../.clang-tidy /home/uni/projects/craftsdwarfship/systems/heroes-ai/party_in_quest_location2.cpp
1.      &lt;eof&gt; parser at end of file
2.      While analyzing stack: 
        #<!-- -->0 Calling boost::pfr::detail::for_each_field_impl(const struct CDS::Attributes &amp;, class (lambda at /home/uni/projects/craftsdwarfship/systems/heroes-ai/party_in_quest_location2.cpp:144:43) &amp;&amp;, std::index_sequence&lt;0UL, 1UL, 2UL, 3UL&gt;, struct std::is_rvalue_reference&lt;const struct CDS::Attributes &amp;&gt;) at line /home/uni/projects/craftsdwarfship/build/Desktop_clang-Debug/vcpkg_installed/x64-linux/include/boost/pfr/detail/for_each_field.hpp:30:9
        #<!-- -->1 Calling boost::pfr::detail::for_each_field(const struct CDS::Attributes &amp;, class (lambda at /home/uni/projects/craftsdwarfship/systems/heroes-ai/party_in_quest_location2.cpp:144:43) &amp;&amp;) at line /home/uni/projects/craftsdwarfship/build/Desktop_clang-Debug/vcpkg_installed/x64-linux/include/boost/pfr/core.hpp:258:12
        #<!-- -->2 Calling boost::pfr::for_each_field(const struct CDS::Attributes &amp;, class (lambda at /home/uni/projects/craftsdwarfship/systems/heroes-ai/party_in_quest_location2.cpp:144:43) &amp;&amp;) at line 144
        #<!-- -->3 Calling CDS::(anonymous namespace)::rewardFight(entt::registry &amp;, const GameState &amp;, SystemContext &amp;, std::span&lt;const entt::entity&gt;, const CombatSimulationState &amp;)::(anonymous class)::operator()(const entt::entity &amp;) at line /usr/bin/../lib64/gcc/x86_64-suse-linux/15/../../../../include/c++/15/bits/invoke.h:63:14
        #<!-- -->4 Calling std::__invoke_impl(__invoke_other, class (lambda at /home/uni/projects/craftsdwarfship/systems/heroes-ai/party_in_quest_location2.cpp:142:31) &amp;, const enum entt::entity &amp;) at line /usr/bin/../lib64/gcc/x86_64-suse-linux/15/../../../../include/c++/15/bits/invoke.h:98:14
        #<!-- -->5 Calling std::__invoke(class (lambda at /home/uni/projects/craftsdwarfship/systems/heroes-ai/party_in_quest_location2.cpp:142:31) &amp;, const enum entt::entity &amp;) at line /usr/bin/../lib64/gcc/x86_64-suse-linux/15/../../../../include/c++/15/bits/ranges_algo.h:190:4
        #<!-- -->6 Calling std::ranges::__for_each_fn::operator()(class __gnu_cxx::__normal_iterator&lt;const enum entt::entity *, class std::vector&lt;enum entt::entity&gt; &gt;, class __gnu_cxx::__normal_iterator&lt;const enum entt::entity *, class std::vector&lt;enum entt::entity&gt; &gt;, class (lambda at /home/uni/projects/craftsdwarfship/systems/heroes-ai/party_in_quest_location2.cpp:142:31), struct std::identity) at line /usr/bin/../lib64/gcc/x86_64-suse-linux/15/../../../../include/c++/15/bits/ranges_algo.h:200:9
        #<!-- -->7 Calling std::ranges::__for_each_fn::operator()(const class std::vector&lt;enum entt::entity&gt; &amp;, class (lambda at /home/uni/projects/craftsdwarfship/systems/heroes-ai/party_in_quest_location2.cpp:142:31), struct std::identity) at line 146
        #<!-- -->8 Calling CDS::(anonymous namespace)::rewardFight(entt::registry &amp;, const GameState &amp;, SystemContext &amp;, std::span&lt;const entt::entity&gt;, const CombatSimulationState &amp;)
3.      /home/uni/projects/craftsdwarfship/build/Desktop_clang-Debug/vcpkg_installed/x64-linux/include/boost/pfr/detail/for_each_field_impl.hpp:47:18: Error evaluating statement
4.      /home/uni/projects/craftsdwarfship/build/Desktop_clang-Debug/vcpkg_installed/x64-linux/include/boost/pfr/detail/for_each_field_impl.hpp:47:18: Error evaluating statement
 #<!-- -->0 0x00007fdf482fef39 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/lib64/libLLVM.so.21.1+0x6fef39)
 #<!-- -->1 0x00007fdf482fbe13 llvm::sys::RunSignalHandlers() (/lib64/libLLVM.so.21.1+0x6fbe13)
 #<!-- -->2 0x00007fdf482ff718 (/lib64/libLLVM.so.21.1+0x6ff718)
 #<!-- -->3 0x00007fdf47442910 __restore_rt (/lib64/libc.so.6+0x42910)
 #<!-- -->4 0x00007fdf527d758e clang::ento::ExprEngine::VisitCommonDeclRefExpr(clang::Expr const*, clang::NamedDecl const*, clang::ento::ExplodedNode*, clang::ento::ExplodedNodeSet&amp;) (/lib64/libclang-cpp.so.21.1+0x29d758e)
 #<!-- -->5 0x00007fdf527d0d2a clang::ento::ExprEngine::Visit(clang::Stmt const*, clang::ento::ExplodedNode*, clang::ento::ExplodedNodeSet&amp;) (/lib64/libclang-cpp.so.21.1+0x29d0d2a)
 #<!-- -->6 0x00007fdf527cdb69 clang::ento::ExprEngine::ProcessStmt(clang::Stmt const*, clang::ento::ExplodedNode*) (/lib64/libclang-cpp.so.21.1+0x29cdb69)
 #<!-- -->7 0x00007fdf527af086 clang::ento::CoreEngine::dispatchWorkItem(clang::ento::ExplodedNode*, clang::ProgramPoint, clang::ento::WorkListUnit const&amp;) (/lib64/libclang-cpp.so.21.1+0x29af086)
 #<!-- -->8 0x00007fdf527ae93e clang::ento::CoreEngine::ExecuteWorkList(clang::LocationContext const*, unsigned int, llvm::IntrusiveRefCntPtr&lt;clang::ento::ProgramState const&gt;) (/lib64/libclang-cpp.so.21.1+0x29ae93e)
 #<!-- -->9 0x00007fdf52af6fc0 (/lib64/libclang-cpp.so.21.1+0x2cf6fc0)
#<!-- -->10 0x00007fdf52af5331 (/lib64/libclang-cpp.so.21.1+0x2cf5331)
#<!-- -->11 0x00007fdf524bbfec clang::MultiplexConsumer::HandleTranslationUnit(clang::ASTContext&amp;) (/lib64/libclang-cpp.so.21.1+0x26bbfec)
#<!-- -->12 0x00007fdf504c5fd9 clang::ParseAST(clang::Sema&amp;, bool, bool) (/lib64/libclang-cpp.so.21.1+0x6c5fd9)
#<!-- -->13 0x00007fdf524785fd clang::FrontendAction::Execute() (/lib64/libclang-cpp.so.21.1+0x26785fd)
#<!-- -->14 0x00007fdf523bd22e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/lib64/libclang-cpp.so.21.1+0x25bd22e)
#<!-- -->15 0x00007fdf526bc0fe clang::tooling::FrontendActionFactory::runInvocation(std::shared_ptr&lt;clang::CompilerInvocation&gt;, clang::FileManager*, std::shared_ptr&lt;clang::PCHContainerOperations&gt;, clang::DiagnosticConsumer*) (/lib64/libclang-cpp.so.21.1+0x28bc0fe)
#<!-- -->16 0x0000560aff04635c (/usr/bin/clang-tidy+0x74935c)
#<!-- -->17 0x00007fdf526bbd9d clang::tooling::ToolInvocation::runInvocation(char const*, clang::driver::Compilation*, std::shared_ptr&lt;clang::CompilerInvocation&gt;, std::shared_ptr&lt;clang::PCHContainerOperations&gt;) (/lib64/libclang-cpp.so.21.1+0x28bbd9d)
#<!-- -->18 0x00007fdf526ba9dc clang::tooling::ToolInvocation::run() (/lib64/libclang-cpp.so.21.1+0x28ba9dc)
#<!-- -->19 0x00007fdf526bdf32 clang::tooling::ClangTool::run(clang::tooling::ToolAction*) (/lib64/libclang-cpp.so.21.1+0x28bdf32)
#<!-- -->20 0x0000560aff041c36 clang::tidy::runClangTidy(clang::tidy::ClangTidyContext&amp;, clang::tooling::CompilationDatabase const&amp;, llvm::ArrayRef&lt;std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;&gt;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::OverlayFileSystem&gt;, bool, bool, llvm::StringRef) (/usr/bin/clang-tidy+0x744c36)
#<!-- -->21 0x0000560afe9cb3de clang::tidy::clangTidyMain(int, char const**) (/usr/bin/clang-tidy+0xce3de)
#<!-- -->22 0x00007fdf4742b2fb __libc_start_call_main (/lib64/libc.so.6+0x2b2fb)
#<!-- -->23 0x00007fdf4742b3cb __libc_start_main@<!-- -->GLIBC_2.2.5 (/lib64/libc.so.6+0x2b3cb)
#<!-- -->24 0x0000560afe9c7dc5 _start (/usr/bin/clang-tidy+0xcadc5)
/home/uni/projects/craftsdwarfship/systems/heroes-ai/party_in_quest_location2.cpp: terminated by signal 11
```
</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-static-analyzer

Author: Mikhail (pozemka)

<details>
Issue is occuring when analyzing file using compilation database. But without it clang-tidy only complains about missing include files.

`clang-tidy --config-file=../../.clang-tidy -p ../../build/Desktop_clang-Debug  party_in_quest_location2.cpp`

```
Stack dump:
0.      Program arguments: clang-tidy --config-file=../../.clang-tidy -p ../../build/Desktop_clang-Debug party_in_quest_location2.cpp
1.      &lt;eof&gt; parser at end of file
2.      While analyzing stack: 
        #<!-- -->0 Calling boost::pfr::detail::for_each_field_impl(const struct CDS::Attributes &amp;, class (lambda at /home/uni/projects/craftsdwarfship/systems/heroes-ai/party_in_quest_location2.cpp:144:43) &amp;&amp;, std::index_sequence&lt;0UL, 1UL, 2UL, 3UL&gt;, struct std::is_rvalue_reference&lt;const struct CDS::Attributes &amp;&gt;) at line /home/uni/projects/craftsdwarfship/build/Desktop_clang-Debug/vcpkg_installed/x64-linux/include/boost/pfr/detail/for_each_field.hpp:30:9
        #<!-- -->1 Calling boost::pfr::detail::for_each_field(const struct CDS::Attributes &amp;, class (lambda at /home/uni/projects/craftsdwarfship/systems/heroes-ai/party_in_quest_location2.cpp:144:43) &amp;&amp;) at line /home/uni/projects/craftsdwarfship/build/Desktop_clang-Debug/vcpkg_installed/x64-linux/include/boost/pfr/core.hpp:258:12
        #<!-- -->2 Calling boost::pfr::for_each_field(const struct CDS::Attributes &amp;, class (lambda at /home/uni/projects/craftsdwarfship/systems/heroes-ai/party_in_quest_location2.cpp:144:43) &amp;&amp;) at line 144
        #<!-- -->3 Calling CDS::(anonymous namespace)::rewardFight(entt::registry &amp;, const GameState &amp;, SystemContext &amp;, std::span&lt;const entt::entity&gt;, const CombatSimulationState &amp;)::(anonymous class)::operator()(const entt::entity &amp;) at line /usr/bin/../lib64/gcc/x86_64-suse-linux/15/../../../../include/c++/15/bits/invoke.h:63:14
        #<!-- -->4 Calling std::__invoke_impl(__invoke_other, class (lambda at /home/uni/projects/craftsdwarfship/systems/heroes-ai/party_in_quest_location2.cpp:142:31) &amp;, const enum entt::entity &amp;) at line /usr/bin/../lib64/gcc/x86_64-suse-linux/15/../../../../include/c++/15/bits/invoke.h:98:14
        #<!-- -->5 Calling std::__invoke(class (lambda at /home/uni/projects/craftsdwarfship/systems/heroes-ai/party_in_quest_location2.cpp:142:31) &amp;, const enum entt::entity &amp;) at line /usr/bin/../lib64/gcc/x86_64-suse-linux/15/../../../../include/c++/15/bits/ranges_algo.h:190:4
        #<!-- -->6 Calling std::ranges::__for_each_fn::operator()(class __gnu_cxx::__normal_iterator&lt;const enum entt::entity *, class std::vector&lt;enum entt::entity&gt; &gt;, class __gnu_cxx::__normal_iterator&lt;const enum entt::entity *, class std::vector&lt;enum entt::entity&gt; &gt;, class (lambda at /home/uni/projects/craftsdwarfship/systems/heroes-ai/party_in_quest_location2.cpp:142:31), struct std::identity) at line /usr/bin/../lib64/gcc/x86_64-suse-linux/15/../../../../include/c++/15/bits/ranges_algo.h:200:9
        #<!-- -->7 Calling std::ranges::__for_each_fn::operator()(const class std::vector&lt;enum entt::entity&gt; &amp;, class (lambda at /home/uni/projects/craftsdwarfship/systems/heroes-ai/party_in_quest_location2.cpp:142:31), struct std::identity) at line 146
        #<!-- -->8 Calling CDS::(anonymous namespace)::rewardFight(entt::registry &amp;, const GameState &amp;, SystemContext &amp;, std::span&lt;const entt::entity&gt;, const CombatSimulationState &amp;)
3.      /home/uni/projects/craftsdwarfship/build/Desktop_clang-Debug/vcpkg_installed/x64-linux/include/boost/pfr/detail/for_each_field_impl.hpp:47:18: Error evaluating statement
4.      /home/uni/projects/craftsdwarfship/build/Desktop_clang-Debug/vcpkg_installed/x64-linux/include/boost/pfr/detail/for_each_field_impl.hpp:47:18: Error evaluating statement
 #<!-- -->0 0x00007fead48fef39 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/lib64/libLLVM.so.21.1+0x6fef39)
 #<!-- -->1 0x00007fead48fbe13 llvm::sys::RunSignalHandlers() (/lib64/libLLVM.so.21.1+0x6fbe13)
 #<!-- -->2 0x00007fead48ff718 (/lib64/libLLVM.so.21.1+0x6ff718)
 #<!-- -->3 0x00007fead3a42910 __restore_rt (/lib64/libc.so.6+0x42910)
 #<!-- -->4 0x00007feadedd758e clang::ento::ExprEngine::VisitCommonDeclRefExpr(clang::Expr const*, clang::NamedDecl const*, clang::ento::ExplodedNode*, clang::ento::ExplodedNodeSet&amp;) (/lib64/libclang-cpp.so.21.1+0x29d758e)
 #<!-- -->5 0x00007feadedd0d2a clang::ento::ExprEngine::Visit(clang::Stmt const*, clang::ento::ExplodedNode*, clang::ento::ExplodedNodeSet&amp;) (/lib64/libclang-cpp.so.21.1+0x29d0d2a)
 #<!-- -->6 0x00007feadedcdb69 clang::ento::ExprEngine::ProcessStmt(clang::Stmt const*, clang::ento::ExplodedNode*) (/lib64/libclang-cpp.so.21.1+0x29cdb69)
 #<!-- -->7 0x00007feadedaf086 clang::ento::CoreEngine::dispatchWorkItem(clang::ento::ExplodedNode*, clang::ProgramPoint, clang::ento::WorkListUnit const&amp;) (/lib64/libclang-cpp.so.21.1+0x29af086)
 #<!-- -->8 0x00007feadedae93e clang::ento::CoreEngine::ExecuteWorkList(clang::LocationContext const*, unsigned int, llvm::IntrusiveRefCntPtr&lt;clang::ento::ProgramState const&gt;) (/lib64/libclang-cpp.so.21.1+0x29ae93e)
 #<!-- -->9 0x00007feadf0f6fc0 (/lib64/libclang-cpp.so.21.1+0x2cf6fc0)
#<!-- -->10 0x00007feadf0f5331 (/lib64/libclang-cpp.so.21.1+0x2cf5331)
#<!-- -->11 0x00007feadeabbfec clang::MultiplexConsumer::HandleTranslationUnit(clang::ASTContext&amp;) (/lib64/libclang-cpp.so.21.1+0x26bbfec)
#<!-- -->12 0x00007feadcac5fd9 clang::ParseAST(clang::Sema&amp;, bool, bool) (/lib64/libclang-cpp.so.21.1+0x6c5fd9)
#<!-- -->13 0x00007feadea785fd clang::FrontendAction::Execute() (/lib64/libclang-cpp.so.21.1+0x26785fd)
#<!-- -->14 0x00007feade9bd22e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/lib64/libclang-cpp.so.21.1+0x25bd22e)
#<!-- -->15 0x00007feadecbc0fe clang::tooling::FrontendActionFactory::runInvocation(std::shared_ptr&lt;clang::CompilerInvocation&gt;, clang::FileManager*, std::shared_ptr&lt;clang::PCHContainerOperations&gt;, clang::DiagnosticConsumer*) (/lib64/libclang-cpp.so.21.1+0x28bc0fe)
#<!-- -->16 0x000056167c5b835c (/usr/bin/clang-tidy-21+0x74935c)
#<!-- -->17 0x00007feadecbbd9d clang::tooling::ToolInvocation::runInvocation(char const*, clang::driver::Compilation*, std::shared_ptr&lt;clang::CompilerInvocation&gt;, std::shared_ptr&lt;clang::PCHContainerOperations&gt;) (/lib64/libclang-cpp.so.21.1+0x28bbd9d)
#<!-- -->18 0x00007feadecba9dc clang::tooling::ToolInvocation::run() (/lib64/libclang-cpp.so.21.1+0x28ba9dc)
#<!-- -->19 0x00007feadecbdf32 clang::tooling::ClangTool::run(clang::tooling::ToolAction*) (/lib64/libclang-cpp.so.21.1+0x28bdf32)
#<!-- -->20 0x000056167c5b3c36 clang::tidy::runClangTidy(clang::tidy::ClangTidyContext&amp;, clang::tooling::CompilationDatabase const&amp;, llvm::ArrayRef&lt;std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;&gt;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::OverlayFileSystem&gt;, bool, bool, llvm::StringRef) (/usr/bin/clang-tidy-21+0x744c36)
#<!-- -->21 0x000056167bf3d3de clang::tidy::clangTidyMain(int, char const**) (/usr/bin/clang-tidy-21+0xce3de)
#<!-- -->22 0x00007fead3a2b2fb __libc_start_call_main (/lib64/libc.so.6+0x2b2fb)
#<!-- -->23 0x00007fead3a2b3cb __libc_start_main@<!-- -->GLIBC_2.2.5 (/lib64/libc.so.6+0x2b3cb)
#<!-- -->24 0x000056167bf39dc5 _start (/usr/bin/clang-tidy-21+0xcadc5)
/home/uni/projects/craftsdwarfship/systems/heroes-ai/party_in_quest_location2.cpp: terminated by signal 11
```
</details>


---

### Comment 3 - EugeneZelenko

Could you please try 22 release candidate or `main` branch? https://godbolt.org should be helpful. Only most recent release is maintained.

Please provide reproducer, preferably on https://godbolt.org.


---

### Comment 4 - steakhal

Yea, without a reproducer we will close this issue, I'm sure.

---

### Comment 5 - pozemka

Understandable. I will try to reproduce and check with latest clang-tidy.

---

### Comment 6 - pozemka

I couldn't reproduce it using Compiler Explorer https://clang-tidy.godbolt.org/z/crnroTz1Y

However it does reproduces locally with 22 release candidate:
```
Stack dump:
0.      Program arguments: /home/uni/bin/LLVM-22.1.0-rc3-Linux-X64/bin/clang-tidy --config-file=../../.clang-tidy -p ../../build/Desktop_clang-Debug repro.cpp
1.      <eof> parser at end of file
2.      While analyzing stack: 
        #0 Calling boost::pfr::detail::for_each_field_impl(struct (anonymous namespace)::Attributes &, class (lambda at /home/uni/projects/craftsdwarfship/systems/heroes-ai/repro.cpp:34:43) &&, std::index_sequence<0UL, 1UL>, struct std::is_rvalue_reference<struct (anonymous namespace)::Attributes &>) at line /home/uni/projects/craftsdwarfship/build/Desktop_clang-Debug/vcpkg_installed/x64-linux/include/boost/pfr/detail/for_each_field.hpp:30:9
        #1 Calling boost::pfr::detail::for_each_field(struct (anonymous namespace)::Attributes &, class (lambda at /home/uni/projects/craftsdwarfship/systems/heroes-ai/repro.cpp:34:43) &&) at line /home/uni/projects/craftsdwarfship/build/Desktop_clang-Debug/vcpkg_installed/x64-linux/include/boost/pfr/core.hpp:258:12
        #2 Calling boost::pfr::for_each_field(struct (anonymous namespace)::Attributes &, class (lambda at /home/uni/projects/craftsdwarfship/systems/heroes-ai/repro.cpp:34:43) &&) at line 34
        #3 Calling repro()::(lambda)::operator()()
3.      /home/uni/projects/craftsdwarfship/build/Desktop_clang-Debug/vcpkg_installed/x64-linux/include/boost/pfr/detail/for_each_field_impl.hpp:47:18: Error evaluating statement
4.      /home/uni/projects/craftsdwarfship/build/Desktop_clang-Debug/vcpkg_installed/x64-linux/include/boost/pfr/detail/for_each_field_impl.hpp:47:18: Error evaluating statement
 #0 0x000055d3f440b097 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/home/uni/bin/LLVM-22.1.0-rc3-Linux-X64/bin/clang-tidy+0x46a5097)
 #1 0x000055d3f440bbcf SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x00007fe2eda42910 __restore_rt (/lib64/libc.so.6+0x42910)
 #3 0x000055d3f57d26ed clang::ento::ExprEngine::VisitCommonDeclRefExpr(clang::Expr const*, clang::NamedDecl const*, clang::ento::ExplodedNode*, clang::ento::ExplodedNodeSet&) (/home/uni/bin/LLVM-22.1.0-rc3-Linux-X64/bin/clang-tidy+0x5a6c6ed)
 #4 0x000055d3f57cd74f clang::ento::ExprEngine::Visit(clang::Stmt const*, clang::ento::ExplodedNode*, clang::ento::ExplodedNodeSet&) (/home/uni/bin/LLVM-22.1.0-rc3-Linux-X64/bin/clang-tidy+0x5a6774f)
 #5 0x000055d3f57cb072 clang::ento::ExprEngine::ProcessStmt(clang::Stmt const*, clang::ento::ExplodedNode*) (/home/uni/bin/LLVM-22.1.0-rc3-Linux-X64/bin/clang-tidy+0x5a65072)
 #6 0x000055d3f57b54dc clang::ento::CoreEngine::HandlePostStmt(clang::CFGBlock const*, unsigned int, clang::ento::ExplodedNode*) (/home/uni/bin/LLVM-22.1.0-rc3-Linux-X64/bin/clang-tidy+0x5a4f4dc)
 #7 0x000055d3f57b4cb9 clang::ento::CoreEngine::dispatchWorkItem(clang::ento::ExplodedNode*, clang::ProgramPoint, clang::ento::WorkListUnit const&) (/home/uni/bin/LLVM-22.1.0-rc3-Linux-X64/bin/clang-tidy+0x5a4ecb9)
 #8 0x000055d3f57b43a5 clang::ento::CoreEngine::ExecuteWorkList(clang::LocationContext const*, unsigned int, llvm::IntrusiveRefCntPtr<clang::ento::ProgramState const>) (/home/uni/bin/LLVM-22.1.0-rc3-Linux-X64/bin/clang-tidy+0x5a4e3a5)
 #9 0x000055d3f55a4f3c clang::ento::ExprEngine::ExecuteWorkList(clang::LocationContext const*, unsigned int) AnalysisConsumer.cpp:0:0
#10 0x000055d3f55a3534 (anonymous namespace)::AnalysisConsumer::HandleCode(clang::Decl*, unsigned int, clang::ento::ExprEngine::InliningModes, llvm::DenseSet<clang::Decl const*, llvm::DenseMapInfo<clang::Decl const*, void>>*) AnalysisConsumer.cpp:0:0
#11 0x000055d3f55a2170 (anonymous namespace)::AnalysisConsumer::HandleTranslationUnit(clang::ASTContext&) AnalysisConsumer.cpp:0:0
#12 0x000055d3f5a76e81 clang::MultiplexConsumer::HandleTranslationUnit(clang::ASTContext&) (/home/uni/bin/LLVM-22.1.0-rc3-Linux-X64/bin/clang-tidy+0x5d10e81)
#13 0x000055d3f328740d clang::ParseAST(clang::Sema&, bool, bool) (/home/uni/bin/LLVM-22.1.0-rc3-Linux-X64/bin/clang-tidy+0x352140d)
#14 0x000055d3f41e79cd clang::FrontendAction::Execute() (/home/uni/bin/LLVM-22.1.0-rc3-Linux-X64/bin/clang-tidy+0x44819cd)
#15 0x000055d3f41e765b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/home/uni/bin/LLVM-22.1.0-rc3-Linux-X64/bin/clang-tidy+0x448165b)
#16 0x000055d3f446dbba clang::tooling::FrontendActionFactory::runInvocation(std::shared_ptr<clang::CompilerInvocation>, clang::FileManager*, std::shared_ptr<clang::PCHContainerOperations>, clang::DiagnosticConsumer*) (/home/uni/bin/LLVM-22.1.0-rc3-Linux-X64/bin/clang-tidy+0x4707bba)
#17 0x000055d3f44b4236 clang::tidy::runClangTidy(clang::tidy::ClangTidyContext&, clang::tooling::CompilationDatabase const&, llvm::ArrayRef<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>>, llvm::IntrusiveRefCntPtr<llvm::vfs::OverlayFileSystem>, bool, bool, llvm::StringRef, bool)::ActionFactory::runInvocation(std::shared_ptr<clang::CompilerInvocation>, clang::FileManager*, std::shared_ptr<clang::PCHContainerOperations>, clang::DiagnosticConsumer*) ClangTidy.cpp:0:0
#18 0x000055d3f446d82e clang::tooling::ToolInvocation::runInvocation(char const*, clang::driver::Compilation*, std::shared_ptr<clang::CompilerInvocation>, std::shared_ptr<clang::PCHContainerOperations>) (/home/uni/bin/LLVM-22.1.0-rc3-Linux-X64/bin/clang-tidy+0x470782e)
#19 0x000055d3f446c532 clang::tooling::ToolInvocation::run() (/home/uni/bin/LLVM-22.1.0-rc3-Linux-X64/bin/clang-tidy+0x4706532)
#20 0x000055d3f446feff clang::tooling::ClangTool::run(clang::tooling::ToolAction*) (/home/uni/bin/LLVM-22.1.0-rc3-Linux-X64/bin/clang-tidy+0x4709eff)
#21 0x000055d3f44ae942 clang::tidy::runClangTidy(clang::tidy::ClangTidyContext&, clang::tooling::CompilationDatabase const&, llvm::ArrayRef<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>>, llvm::IntrusiveRefCntPtr<llvm::vfs::OverlayFileSystem>, bool, bool, llvm::StringRef, bool) (/home/uni/bin/LLVM-22.1.0-rc3-Linux-X64/bin/clang-tidy+0x4748942)
#22 0x000055d3f43147e0 clang::tidy::clangTidyMain(int, char const**) (/home/uni/bin/LLVM-22.1.0-rc3-Linux-X64/bin/clang-tidy+0x45ae7e0)
#23 0x00007fe2eda2b2fb __libc_start_call_main (/lib64/libc.so.6+0x2b2fb)
#24 0x00007fe2eda2b3cb __libc_start_main@GLIBC_2.2.5 (/lib64/libc.so.6+0x2b3cb)
#25 0x000055d3f433c9d5 _start (/home/uni/bin/LLVM-22.1.0-rc3-Linux-X64/bin/clang-tidy+0x45d69d5)
```

---

### Comment 7 - pozemka

I also managed to get backtrace from gdb:
```
Program received signal SIGSEGV, Segmentation fault.
0x000055555afc06ed in clang::ento::ExprEngine::VisitCommonDeclRefExpr(clang::Expr const*, clang::NamedDecl const*, clang::ento::ExplodedNode*, clang::ento::ExplodedNodeSet&) ()
(gdb) bt
#0  0x000055555afc06ed in clang::ento::ExprEngine::VisitCommonDeclRefExpr(clang::Expr const*, clang::NamedDecl const*, clang::ento::ExplodedNode*, clang::ento::ExplodedNodeSet&) ()
#1  0x000055555afbb74f in clang::ento::ExprEngine::Visit(clang::Stmt const*, clang::ento::ExplodedNode*, clang::ento::ExplodedNodeSet&) ()
#2  0x000055555afb9072 in clang::ento::ExprEngine::ProcessStmt(clang::Stmt const*, clang::ento::ExplodedNode*) ()
#3  0x000055555afa34dc in clang::ento::CoreEngine::HandlePostStmt(clang::CFGBlock const*, unsigned int, clang::ento::ExplodedNode*) ()
#4  0x000055555afa2cb9 in clang::ento::CoreEngine::dispatchWorkItem(clang::ento::ExplodedNode*, clang::ProgramPoint, clang::ento::WorkListUnit const&) ()
#5  0x000055555afa23a5 in clang::ento::CoreEngine::ExecuteWorkList(clang::LocationContext const*, unsigned int, llvm::IntrusiveRefCntPtr<clang::ento::ProgramState const>) ()
#6  0x000055555ad92f3c in clang::ento::ExprEngine::ExecuteWorkList(clang::LocationContext const*, unsigned int) ()
#7  0x000055555ad91534 in (anonymous namespace)::AnalysisConsumer::HandleCode(clang::Decl*, unsigned int, clang::ento::ExprEngine::InliningModes, llvm::DenseSet<clang::Decl const*, llvm::DenseMapInfo<clang::Decl const*, void> >*) ()
#8  0x000055555ad90170 in (anonymous namespace)::AnalysisConsumer::HandleTranslationUnit(clang::ASTContext&) ()
#9  0x000055555b264e81 in clang::MultiplexConsumer::HandleTranslationUnit(clang::ASTContext&) ()
#10 0x0000555558a7540d in clang::ParseAST(clang::Sema&, bool, bool) ()
#11 0x00005555599d59cd in clang::FrontendAction::Execute() ()
#12 0x00005555599d565b in clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) ()
#13 0x0000555559c5bbba in clang::tooling::FrontendActionFactory::runInvocation(std::shared_ptr<clang::CompilerInvocation>, clang::FileManager*, std::shared_ptr<clang::PCHContainerOperations>, clang::DiagnosticConsumer*) ()
#14 0x0000555559ca2236 in clang::tidy::runClangTidy(clang::tidy::ClangTidyContext&, clang::tooling::CompilationDatabase const&, llvm::ArrayRef<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > >, llvm::IntrusiveRefCntPtr<llvm::vfs::OverlayFileSystem>, bool, bool, llvm::StringRef, bool)::ActionFactory::runInvocation(std::shared_ptr<clang::CompilerInvocation>, clang::FileManager*, std::shared_ptr<clang::PCHContainerOperations>, clang::DiagnosticConsumer*) ()
#15 0x0000555559c5b82e in clang::tooling::ToolInvocation::runInvocation(char const*, clang::driver::Compilation*, std::shared_ptr<clang::CompilerInvocation>, std::shared_ptr<clang::PCHContainerOperations>) ()
#16 0x0000555559c5a532 in clang::tooling::ToolInvocation::run() ()
#17 0x0000555559c5deff in clang::tooling::ClangTool::run(clang::tooling::ToolAction*) ()
#18 0x0000555559c9c942 in clang::tidy::runClangTidy(clang::tidy::ClangTidyContext&, clang::tooling::CompilationDatabase const&, llvm::ArrayRef<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > >, llvm::IntrusiveRefCntPtr<llvm::vfs::OverlayFileSystem>, bool, bool, llvm::StringRef, bool) ()
#19 0x0000555559b027e0 in clang::tidy::clangTidyMain(int, char const**) ()
#20 0x00007ffff782b2fb in __libc_start_call_main (main=main@entry=0x555559afecf0 <main>, argc=argc@entry=5, 
    argv=argv@entry=0x7fffffffd558) at ../sysdeps/nptl/libc_start_call_main.h:58
#21 0x00007ffff782b3cb in __libc_start_main_impl (main=0x555559afecf0 <main>, argc=5, argv=0x7fffffffd558, init=<optimized out>, 
    fini=<optimized out>, rtld_fini=<optimized out>, stack_end=0x7fffffffd548) at ../csu/libc-start.c:360
#22 0x0000555559b2a9d5 in _start ()
```

---

### Comment 8 - steakhal

The stacktrace itself is not too useful. Could you attach the preprocessed file and the exact tidy invocation?

---

### Comment 9 - pozemka

Preprocessed file:
[repro_preprocessed.cpp](https://github.com/user-attachments/files/25427106/repro_preprocessed.cpp)

Source file:
[repro.cpp](https://github.com/user-attachments/files/25427115/repro.cpp)

tidy invocation:
```
~/bin/LLVM-22.1.0-rc3-Linux-X64/bin/clang-tidy --config="{Checks: '*, -abseil-*, -altera-*, -android-*, -fuchsia-*, -google-*, -llvm*, -modernize-use-trailing-return-type, -modernize-concat-nested-namespaces, -zircon-*, -readability-else-after-return, -readability-static-accessed-through-instance, -readability-avoid-const-params-in-decls, -readability-uppercase-literal-suffix, -hicpp-uppercase-literal-suffix, -hicpp-signed-bitwise, -cppcoreguidelines-non-private-member-variables-in-classes, -cppcoreguidelines-pro-type-vararg, -misc-unused-parameters, -misc-include-cleaner, -misc-non-private-member-variables-in-classes, -misc-no-recursion, -misc-use-anonymous-namespace, -misc-use-internal-linkage ', WarningsAsErrors: '', HeaderFilterRegex: '', FormatStyle: none, CheckOptions: [{key: readability-identifier-length.MinimumVariableNameLength, value: 2}, {key: readability-identifier-length.IgnoredVariableNames, value: x|y|z|id}, {key: readability-identifier-length.IgnoredParameterNames, value: x|y|z|id}, {key: readability-identifier-length.IgnoredLoopCounterNames, value: '^[xyzijk_]$'}, {key: hicpp-signed-bitwise.IgnorePositiveIntegerLiterals, value: true}, {key: bugprone-easily-swappable-parameters.MinimumLength, value: 3}]}" -p ../../build/Desktop_clang-Debug repro.cpp
```


---

### Comment 10 - EugeneZelenko

@pozemka: Since crash happens in Static Analyzer, you could enable only `clang-analyzer-*`. And ideally narrow down for single check.

---

### Comment 11 - steakhal

The crash is coming from the core. I'll take it from here.

---

### Comment 12 - pozemka

I've narrowed invokation a bit.
OK:
```
~/bin/LLVM-22.1.0-rc3-Linux-X64/bin/clang-tidy --config="{Checks: 'bugprone-no-escape'}" -p ../../build/Desktop_clang-Debug repro.cpp
```
CRASH:
```
~/bin/LLVM-22.1.0-rc3-Linux-X64/bin/clang-tidy --config="{Checks: 'clang-analyzer-core.BitwiseShift'}" -p ../../build/Desktop_clang-Debug repro.cpp
```
But it seems It is impossible to narrow to the single check: #59588
```
~/bin/LLVM-22.1.0-rc3-Linux-X64/bin/clang-tidy --config="{Checks: 'clang-analyzer-core.BitwiseShift'}" -p ../../build/Desktop_clang-Debug --list-checks repro.cpp 
Enabled checks:
    clang-analyzer-core.BitwiseShift
    clang-analyzer-core.CallAndMessage
    clang-analyzer-core.DivideZero
    clang-analyzer-core.DynamicTypePropagation
    clang-analyzer-core.FixedAddressDereference
    clang-analyzer-core.NonNullParamChecker
    clang-analyzer-core.NonnilStringConstants
    clang-analyzer-core.NullDereference
    clang-analyzer-core.NullPointerArithm
    clang-analyzer-core.StackAddressEscape
    clang-analyzer-core.UndefinedBinaryOperatorResult
    clang-analyzer-core.VLASize
    clang-analyzer-core.builtin.AssumeModeling
    clang-analyzer-core.builtin.BuiltinFunctions
    clang-analyzer-core.builtin.NoReturnFunctions
    clang-analyzer-core.uninitialized.ArraySubscript
    clang-analyzer-core.uninitialized.Assign
    clang-analyzer-core.uninitialized.Branch
    clang-analyzer-core.uninitialized.CapturedBlockVariable
    clang-analyzer-core.uninitialized.NewArraySize
    clang-analyzer-core.uninitialized.UndefReturn
```

---

### Comment 13 - pozemka

And smaller preprocessed source with only include is boost/pfr/core.hpp:

[repro_preprocessed.cpp](https://github.com/user-attachments/files/25437686/repro_preprocessed.cpp)

[repro.cpp](https://github.com/user-attachments/files/25437713/repro.cpp)

---

### Comment 14 - steakhal

Here is the reduced version by claude:
https://godbolt.org/z/b53aobcK1 (crashing on main)
```c++
// Minimal reproducer for crash in ExprEngine::VisitCommonDeclRefExpr
// when visiting a pack-indexed structured binding (C++26 pack bindings).
//
// RUN: %clang_analyze_cc1 -std=c++26 --target=x86_64-pc-linux-gnu -verify %s
//
// Crash: assertion "dyn_cast on a non-existent value" in Casting.h
// because BD->getBinding() is null for a pack BindingDecl in a dependent
// context, and the code does not guard against this before calling dyn_cast.

template <class = void>
int templated_fn() {
    struct S { int a; };
    auto &&[... members] = S{12};
    return members...[0];
}

void repro() {
    templated_fn();
}
```

The facts are true. We don't support pack indexing from C++26.
It shouldn't be too hard to implement pack indexing properly. I wonder if I could get claude to do it.
I'll try.

---

### Comment 15 - steakhal

Thanks for the report btw.
There is no way to workaround this issue, unless you can avoid using pack-indexing like this here.

---

### Comment 16 - pozemka

🤔 I'm using `set(CMAKE_CXX_STANDARD 23)` for now. And reproducer code looks nothing like my case. 
Is something inside boost::pfr causing it?

---

### Comment 17 - steakhal

I tried implementing structured-binding pack indexing, and I could make it work but I'm blocked on issue #182691.

---

