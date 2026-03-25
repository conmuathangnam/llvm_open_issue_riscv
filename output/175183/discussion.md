# clang-tidy: modernize-use-std-format mangles the code inside a macro

**Author:** vadz
**URL:** https://github.com/llvm/llvm-project/issues/175183
**Status:** Closed
**Labels:** clang-tidy, invalid-code-generation, crash
**Closed Date:** 2026-03-25T06:40:35Z

## Body

Here is the minimum example reproducing the problem I could make:

```cpp
#include <string>
#include <stdexcept>

namespace String
{

extern std::string Printf(const char *format, ...);

}

enum SomeEnum
{
    SomeEnum_Good,
    SomeEnum_Bad
};

#define EXCEPTION_MSG(msg) \
    std::runtime_error(msg)

void Check(SomeEnum n) {
    throw EXCEPTION_MSG(String::Printf("Invalid (%d)", n));
}
```

Running clang-tidy-21 with the following config:

```yaml
Checks: '-*,modernize-use-std-format'
CheckOptions:
  modernize-use-std-format.StrFormatLikeFunctions: 'String::Printf'
  modernize-use-std-format.ReplacementFormatFunction: 'fmt::format'
  modernize-use-std-format.FormatHeader: '<fmt/format.h>'
```

produces the following broken "fix":

```
% clang-tidy-21 t.cpp --  
1 warning generated.
t.cpp:21:25: warning: use 'fmt::format' instead of 'Printf' [modernize-use-std-format]
   21 |     throw EXCEPTION_MSG(String::Printf("Invalid (%d)", n));
      |                         ^~~~~~~~~~~~~~ ~~~~~~~~~~~~~~
      |                         fmt::format    )"Invalid ({})" static_cast<int>(
t.cpp:18:24: note: expanded from macro 'EXCEPTION_MSG'
   18 |     std::runtime_error(msg)
      |                        ^
```

Unfortunately I have plenty of `EXCEPTION_MSG()` macro occurrences in my real code (where this macro does something less trivial than above...), so running clang-tidy on the full code base breaks a lot of code.

As an aside (not sure if it's worth opening a separate issue for this?), please note that clang-tidy didn't generate the code adding `#include <fmt/format.h>` here, which is almost certainly also related to the fact that the function being replaced is inside a macro, as it did add the header to other files in the project.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-tidy

Author: VZ (vadz)

<details>
Here is the minimum example reproducing the problem I could make:

```cpp
#include &lt;string&gt;
#include &lt;stdexcept&gt;

namespace String
{

extern std::string Printf(const char *format, ...);

}

enum SomeEnum
{
    SomeEnum_Good,
    SomeEnum_Bad
};

#define EXCEPTION_MSG(msg) \
    std::runtime_error(msg)

void Check(SomeEnum n) {
    throw EXCEPTION_MSG(String::Printf("Invalid (%d)", n));
}
```

Running clang-tidy-21 with the following config:

```yaml
Checks: '-*,modernize-use-std-format'
CheckOptions:
  modernize-use-std-format.StrFormatLikeFunctions: 'String::Printf'
  modernize-use-std-format.ReplacementFormatFunction: 'fmt::format'
  modernize-use-std-format.FormatHeader: '&lt;fmt/format.h&gt;'
```

produces the following broken "fix":

```
% clang-tidy-21 t.cpp --  
1 warning generated.
t.cpp:21:25: warning: use 'fmt::format' instead of 'Printf' [modernize-use-std-format]
   21 |     throw EXCEPTION_MSG(String::Printf("Invalid (%d)", n));
      |                         ^~~~~~~~~~~~~~ ~~~~~~~~~~~~~~
      |                         fmt::format    )"Invalid ({})" static_cast&lt;int&gt;(
t.cpp:18:24: note: expanded from macro 'EXCEPTION_MSG'
   18 |     std::runtime_error(msg)
      |                        ^
```

Unfortunately I have plenty of `EXCEPTION_MSG()` macro occurrences in my real code (where this macro does something less trivial than above...), so running clang-tidy on the full code base breaks a lot of code.

As an aside (not sure if it's worth opening a separate issue for this?), please note that clang-tidy didn't generate the code adding `#include &lt;fmt/format.h&gt;` here, which is almost certainly also related to the fact that the function being replaced is inside a macro, as it did add the header to other files in the project.
</details>


---

### Comment 2 - zeyi2

The problem is actually worse... It will crash clang-tidy with assertion enabled.

```
m@n:~/.../projects/llvm-project$ ./build/bin/clang-tidy repro.cpp -checks='-*,modernize-use-std-format' -config="{CheckOptions: {modernize-use-std-format.StrFormatLikeFunctions: 'String::Printf', modernize-use-std-format.ReplacementFormatFunction: 'fmt::format', modernize-use-std-format.FormatHeader: '<fmt/format.h>'}}" --
/usr/include/c++/15.2.1/optional:1172: constexpr _Tp* std::optional<_Tp>::operator->() [with _Tp = clang::Token]: Assertion 'this->_M_is_engaged()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: ./build/bin/clang-tidy repro.cpp -checks=-*,modernize-use-std-format "-config={CheckOptions: {modernize-use-std-format.StrFormatLikeFunctions: 'String::Printf', modernize-use-std-format.ReplacementFormatFunction: 'fmt::format', modernize-use-std-format.FormatHeader: '<fmt/format.h>'}}" --
1.      <eof> parser at end of file
2.      ASTMatcher: Processing 'modernize-use-std-format' against:
        CallExpr : </home/mitchell/Documents/projects/llvm-project/repro.cpp:21:11 <Spelling=col:25>, col:11 <Spelling=col:57>>
--- Bound Nodes Begin ---
    func_decl - { FunctionDecl String::Printf : </home/mitchell/Documents/projects/llvm-project/repro.cpp:7:1, col:50> }
    strformat - { CallExpr : </home/mitchell/Documents/projects/llvm-project/repro.cpp:21:11 <Spelling=col:25>, col:11 <Spelling=col:57>> }
--- Bound Nodes End ---
 #0 0x00007f09890b7208 __interceptor_backtrace /usr/src/debug/gcc/gcc/libsanitizer/sanitizer_common/sanitizer_common_interceptors.inc:4423:28
 #1 0x000055dad8fb62a1 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /home/mitchell/Documents/projects/llvm-project/llvm/lib/Support/Unix/Signals.inc:846:3
 #2 0x000055dad8fa7a4a llvm::sys::RunSignalHandlers() /home/mitchell/Documents/projects/llvm-project/llvm/lib/Support/Signals.cpp:108:20
 #3 0x000055dad8fa9eb5 SignalHandler(int, siginfo_t*, void*) /home/mitchell/Documents/projects/llvm-project/llvm/lib/Support/Unix/Signals.inc:429:14
 #4 0x00007f098883e4d0 (/usr/lib/libc.so.6+0x3e4d0)
 #5 0x00007f098889890c (/usr/lib/libc.so.6+0x9890c)
 #6 0x00007f098883e3a0 raise (/usr/lib/libc.so.6+0x3e3a0)
 #7 0x00007f098882557a abort (/usr/lib/libc.so.6+0x2557a)
 #8 0x00007f0988c9a41f (/usr/lib/libstdc++.so.6+0x9a41f)
 #9 0x000055dac4f0c79e llvm::isa_impl_cl<clang::Expr, clang::Stmt const*>::doit(clang::Stmt const*) /usr/include/c++/15.2.1/optional:1172:2
#10 0x000055dac4f0c79e llvm::isa_impl_wrap<clang::Expr, clang::Stmt const*, clang::Stmt const*>::doit(clang::Stmt const* const&) /home/mitchell/Documents/projects/llvm-project/llvm/include/llvm/Support/Casting.h:137:41
#11 0x000055dac4f0c79e llvm::isa_impl_wrap<clang::Expr, clang::Stmt const* const, clang::Stmt const*>::doit(clang::Stmt const* const&) /home/mitchell/Documents/projects/llvm-project/llvm/include/llvm/Support/Casting.h:129:13
#12 0x000055dac4f0c79e llvm::CastIsPossible<clang::Expr, clang::Stmt const*, void>::isPossible(clang::Stmt const* const&) /home/mitchell/Documents/projects/llvm-project/llvm/include/llvm/Support/Casting.h:257:62
#13 0x000055dac4f0c79e llvm::CastInfo<clang::Expr, clang::Stmt const* const, void>::isPossible(clang::Stmt const* const&) /home/mitchell/Documents/projects/llvm-project/llvm/include/llvm/Support/Casting.h:509:38
#14 0x000055dac4f0c79e bool llvm::isa<clang::Expr, clang::Stmt const*>(clang::Stmt const* const&) /home/mitchell/Documents/projects/llvm-project/llvm/include/llvm/Support/Casting.h:548:47
#15 0x000055dac4f0c79e decltype(auto) llvm::cast<clang::Expr, clang::Stmt const>(clang::Stmt const*) /home/mitchell/Documents/projects/llvm-project/llvm/include/llvm/Support/Casting.h:572:3
#16 0x000055dac4f0c79e clang::DynTypedNode::DynCastPtrConverter<clang::Expr, clang::Stmt>::getUnchecked(clang::ASTNodeKind, void const*) /home/mitchell/Documents/projects/llvm-project/clang/include/clang/AST/ASTTypeTraits.h:428:22
#17 0x000055dac4f0c79e clang::DynTypedNode::DynCastPtrConverter<clang::Expr, clang::Stmt>::get(clang::ASTNodeKind, void const*) /home/mitchell/Documents/projects/llvm-project/clang/include/clang/AST/ASTTypeTraits.h:423:29
#18 0x000055dac4f0c79e clang::Expr const* clang::DynTypedNode::get<clang::Expr>() const /home/mitchell/Documents/projects/llvm-project/clang/include/clang/AST/ASTTypeTraits.h:278:33
#19 0x000055dac4f0c79e clang::Expr const* clang::ast_matchers::internal::BoundNodesMap::getNodeAs<clang::Expr>(llvm::StringRef) const /home/mitchell/Documents/projects/llvm-project/clang/include/clang/ASTMatchers/ASTMatchersInternal.h:233:29
#20 0x000055dac4f0c79e clang::Expr const* clang::ast_matchers::BoundNodes::getNodeAs<clang::Expr>(llvm::StringRef) const /home/mitchell/Documents/projects/llvm-project/clang/include/clang/ASTMatchers/ASTMatchers.h:119:37
#21 0x000055dac4f0c79e operator() /home/mitchell/Documents/projects/llvm-project/clang-tools-extra/clang-tidy/utils/FormatStringConverter.cpp:770:71
#22 0x000055dac4f0c79e operator()<__gnu_cxx::__normal_iterator<const clang::ast_matchers::BoundNodes*, std::vector<clang::ast_matchers::BoundNodes> > > /usr/include/c++/15.2.1/bits/predefined_ops.h:318:23
#23 0x000055dac4f0c79e __find_if<__gnu_cxx::__normal_iterator<const clang::ast_matchers::BoundNodes*, std::vector<clang::ast_matchers::BoundNodes> >, __gnu_cxx::__ops::_Iter_pred<clang::tidy::utils::FormatStringConverter::applyFixes(clang::DiagnosticBuilder&, clang::SourceManager&)::<lambda(const clang::ast_matchers::BoundNodes&)> > > /usr/include/c++/15.2.1/bits/stl_algobase.h:2095:42
#24 0x000055dac4f0c79e find_if<__gnu_cxx::__normal_iterator<const clang::ast_matchers::BoundNodes*, std::vector<clang::ast_matchers::BoundNodes> >, clang::tidy::utils::FormatStringConverter::applyFixes(clang::DiagnosticBuilder&, clang::SourceManager&)::<lambda(const clang::ast_matchers::BoundNodes&)> > /usr/include/c++/15.2.1/bits/stl_algo.h:3921:28
#25 0x000055dac4f0c79e clang::tidy::utils::FormatStringConverter::applyFixes(clang::DiagnosticBuilder&, clang::SourceManager&) (.cold) /home/mitchell/Documents/projects/llvm-project/clang-tools-extra/clang-tidy/utils/FormatStringConverter.cpp:765:25
#26 0x000055dac757c773 std::_Optional_base_impl<llvm::StringRef, std::_Optional_base<llvm::StringRef, true, true>>::_M_is_engaged() const /usr/include/c++/15.2.1/optional:613:58
#27 0x000055dac757c773 std::optional<llvm::StringRef>::operator bool() const /usr/include/c++/15.2.1/optional:1205:35
#28 0x000055dac757c773 clang::tidy::modernize::UseStdFormatCheck::check(clang::ast_matchers::MatchFinder::MatchResult const&) /home/mitchell/Documents/projects/llvm-project/clang-tools-extra/clang-tidy/modernize/UseStdFormatCheck.cpp:100:7
#29 0x000055dad1b0ba35 clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor::MatchVisitor::visitMatch(clang::ast_matchers::BoundNodes const&) /home/mitchell/Documents/projects/llvm-project/clang/lib/ASTMatchers/ASTMatchFinder.cpp:1272:20
#30 0x000055dad1c1ea45 std::_Rb_tree<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>, std::pair<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const, clang::DynTypedNode>, std::_Select1st<std::pair<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const, clang::DynTypedNode>>, std::less<void>, std::allocator<std::pair<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const, clang::DynTypedNode>>>::_M_begin_node() const /usr/include/c++/15.2.1/bits/stl_tree.h:1383:12
#31 0x000055dad1c1ea45 std::_Rb_tree<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>, std::pair<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const, clang::DynTypedNode>, std::_Select1st<std::pair<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const, clang::DynTypedNode>>, std::less<void>, std::allocator<std::pair<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const, clang::DynTypedNode>>>::~_Rb_tree() /usr/include/c++/15.2.1/bits/stl_tree.h:1609:17
#32 0x000055dad1c1ea45 std::map<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>, clang::DynTypedNode, std::less<void>, std::allocator<std::pair<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>> const, clang::DynTypedNode>>>::~map() /usr/include/c++/15.2.1/bits/stl_map.h:337:7
#33 0x000055dad1c1ea45 clang::ast_matchers::internal::BoundNodesMap::~BoundNodesMap() /home/mitchell/Documents/projects/llvm-project/clang/include/clang/ASTMatchers/ASTMatchersInternal.h:214:7
#34 0x000055dad1c1ea45 clang::ast_matchers::BoundNodes::~BoundNodes() /home/mitchell/Documents/projects/llvm-project/clang/include/clang/ASTMatchers/ASTMatchers.h:111:7
#35 0x000055dad1c1ea45 clang::ast_matchers::internal::BoundNodesTreeBuilder::visitMatches(clang::ast_matchers::internal::BoundNodesTreeBuilder::Visitor*) /home/mitchell/Documents/projects/llvm-project/clang/lib/ASTMatchers/ASTMatchersInternal.cpp:104:31
#36 0x000055dad1b15064 clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor::matchWithFilter(clang::DynTypedNode const&) /home/mitchell/Documents/projects/llvm-project/clang/include/clang/ASTMatchers/ASTMatchersInternal.h:284:13
#37 0x000055dad1bb9534 matchDispatch /home/mitchell/Documents/projects/llvm-project/clang/lib/ASTMatchers/ASTMatchFinder.cpp:1090:48
#38 0x000055dad1bb9534 match<clang::Stmt> /home/mitchell/Documents/projects/llvm-project/clang/lib/ASTMatchers/ASTMatchFinder.cpp:731:18
#39 0x000055dad1bb9534 clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor::TraverseStmt(clang::Stmt*, llvm::SmallVectorImpl<llvm::PointerIntPair<clang::Stmt*, 1u, bool, llvm::PointerLikeTypeTraits<clang::Stmt*>, llvm::PointerIntPairInfo<clang::Stmt*, 1u, llvm::PointerLikeTypeTraits<clang::Stmt*>>>>*) /home/mitchell/Documents/projects/llvm-project/clang/lib/ASTMatchers/ASTMatchFinder.cpp:1547:8
#40 0x000055dad1bbdcad clang::RecursiveASTVisitor<clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor>::TraverseAtomicExpr(clang::AtomicExpr*, llvm::SmallVectorImpl<llvm::PointerIntPair<clang::Stmt*, 1u, bool, llvm::PointerLikeTypeTraits<clang::Stmt*>, llvm::PointerIntPairInfo<clang::Stmt*, 1u, llvm::PointerLikeTypeTraits<clang::Stmt*>>>>*) /home/mitchell/Documents/projects/llvm-project/clang/include/clang/AST/RecursiveASTVisitor.h:2597:1
#41 0x000055dad1bb06e2 clang::RecursiveASTVisitor<clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor>::dataTraverseNode(clang::Stmt*, llvm::SmallVectorImpl<llvm::PointerIntPair<clang::Stmt*, 1u, bool, llvm::PointerLikeTypeTraits<clang::Stmt*>, llvm::PointerIntPairInfo<clang::Stmt*, 1u, llvm::PointerLikeTypeTraits<clang::Stmt*>>>>*) /home/mitchell/Documents/projects/llvm-project/build/tools/clang/include/clang/AST/StmtNodes.inc:780:1
#42 0x000055dad1bb8913 dataTraverseNode /home/mitchell/Documents/projects/llvm-project/clang/lib/ASTMatchers/ASTMatchFinder.cpp:593:66
#43 0x000055dad1bb8913 clang::RecursiveASTVisitor<clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor>::TraverseStmt(clang::Stmt*, llvm::SmallVectorImpl<llvm::PointerIntPair<clang::Stmt*, 1u, bool, llvm::PointerLikeTypeTraits<clang::Stmt*>, llvm::PointerIntPairInfo<clang::Stmt*, 1u, llvm::PointerLikeTypeTraits<clang::Stmt*>>>>*) /home/mitchell/Documents/projects/llvm-project/clang/include/clang/AST/RecursiveASTVisitor.h:695:7
#44 0x000055dad1bb955b ~ASTNodeNotSpelledInSourceScope /home/mitchell/Documents/projects/llvm-project/clang/lib/ASTMatchers/ASTMatchFinder.cpp:972:7
#45 0x000055dad1bb955b clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor::TraverseStmt(clang::Stmt*, llvm::SmallVectorImpl<llvm::PointerIntPair<clang::Stmt*, 1u, bool, llvm::PointerLikeTypeTraits<clang::Stmt*>, llvm::PointerIntPairInfo<clang::Stmt*, 1u, llvm::PointerLikeTypeTraits<clang::Stmt*>>>>*) /home/mitchell/Documents/projects/llvm-project/clang/lib/ASTMatchers/ASTMatchFinder.cpp:1549:1
#46 0x000055dad1bd715d clang::RecursiveASTVisitor<clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor>::TraverseFunctionHelper(clang::FunctionDecl*) /home/mitchell/Documents/projects/llvm-project/clang/include/clang/AST/RecursiveASTVisitor.h:2386:5
#47 0x000055dad1ba6389 TraverseFunctionDecl /home/mitchell/Documents/projects/llvm-project/clang/include/clang/AST/RecursiveASTVisitor.h:2397:1
#48 0x000055dad1ba6389 clang::RecursiveASTVisitor<clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor>::TraverseDecl(clang::Decl*) /home/mitchell/Documents/projects/llvm-project/build/tools/clang/include/clang/AST/DeclNodes.inc:256:1
#49 0x000055dad1ba91e9 clang::ast_matchers::internal::ASTChildrenNotSpelledInSourceScope::~ASTChildrenNotSpelledInSourceScope() /home/mitchell/Documents/projects/llvm-project/clang/include/clang/ASTMatchers/ASTMatchersInternal.h:824:5
#50 0x000055dad1ba91e9 clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor::TraverseDecl(clang::Decl*) /home/mitchell/Documents/projects/llvm-project/clang/lib/ASTMatchers/ASTMatchFinder.cpp:1537:1
#51 0x000055dad1ba9839 clang::RecursiveASTVisitor<clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor>::TraverseDeclContextHelper(clang::DeclContext*) /home/mitchell/Documents/projects/llvm-project/clang/include/clang/AST/RecursiveASTVisitor.h:1637:7
#52 0x000055dad1be6146 clang::RecursiveASTVisitor<clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor>::TraverseTranslationUnitDecl(clang::TranslationUnitDecl*) /home/mitchell/Documents/projects/llvm-project/clang/include/clang/AST/RecursiveASTVisitor.h:1745:1
#53 0x000055dad1ba7f33 clang::RecursiveASTVisitor<clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor>::TraverseDecl(clang::Decl*) /home/mitchell/Documents/projects/llvm-project/build/tools/clang/include/clang/AST/DeclNodes.inc:24:1
#54 0x000055dad1ba91e9 clang::ast_matchers::internal::ASTChildrenNotSpelledInSourceScope::~ASTChildrenNotSpelledInSourceScope() /home/mitchell/Documents/projects/llvm-project/clang/include/clang/ASTMatchers/ASTMatchersInternal.h:824:5
#55 0x000055dad1ba91e9 clang::ast_matchers::internal::(anonymous namespace)::MatchASTVisitor::TraverseDecl(clang::Decl*) /home/mitchell/Documents/projects/llvm-project/clang/lib/ASTMatchers/ASTMatchFinder.cpp:1537:1
#56 0x000055dad1c08bc2 onEndOfTranslationUnit /home/mitchell/Documents/projects/llvm-project/clang/lib/ASTMatchers/ASTMatchFinder.cpp:444:39
#57 0x000055dad1c08bc2 clang::ast_matchers::MatchFinder::matchAST(clang::ASTContext&) /home/mitchell/Documents/projects/llvm-project/clang/lib/ASTMatchers/ASTMatchFinder.cpp:1785:33
#58 0x000055dacd032f79 __gnu_cxx::__normal_iterator<std::unique_ptr<clang::ASTConsumer, std::default_delete<clang::ASTConsumer>>*, std::vector<std::unique_ptr<clang::ASTConsumer, std::default_delete<clang::ASTConsumer>>, std::allocator<std::unique_ptr<clang::ASTConsumer, std::default_delete<clang::ASTConsumer>>>>>::operator++() /usr/include/c++/15.2.1/bits/stl_iterator.h:1103:4
#59 0x000055dacd032f79 clang::MultiplexConsumer::HandleTranslationUnit(clang::ASTContext&) /home/mitchell/Documents/projects/llvm-project/clang/lib/Frontend/MultiplexConsumer.cpp:345:25
#60 0x000055dacd284c58 clang::ParseAST(clang::Sema&, bool, bool) /home/mitchell/Documents/projects/llvm-project/clang/lib/Parse/ParseAST.cpp:183:34
#61 0x000055dacced36e0 clang::FrontendAction::Execute() /home/mitchell/Documents/projects/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1317:38
#62 0x000055daccc44332 llvm::Error::getPtr() const /home/mitchell/Documents/projects/llvm-project/llvm/include/llvm/Support/Error.h:278:42
#63 0x000055daccc44332 llvm::Error::operator bool() /home/mitchell/Documents/projects/llvm-project/llvm/include/llvm/Support/Error.h:241:22
#64 0x000055daccc44332 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) /home/mitchell/Documents/projects/llvm-project/clang/lib/Frontend/CompilerInstance.cpp:1005:42
#65 0x000055daca9c4ea9 clang::tooling::FrontendActionFactory::runInvocation(std::shared_ptr<clang::CompilerInvocation>, clang::FileManager*, std::shared_ptr<clang::PCHContainerOperations>, clang::DiagnosticConsumer*) /home/mitchell/Documents/projects/llvm-project/clang/lib/Tooling/Tooling.cpp:460:24
#66 0x000055daca81aa80 std::__shared_count<(__gnu_cxx::_Lock_policy)2>::~__shared_count() /usr/include/c++/15.2.1/bits/shared_ptr_base.h:1068:6
#67 0x000055daca81aa80 std::__shared_ptr<clang::CompilerInvocation, (__gnu_cxx::_Lock_policy)2>::~__shared_ptr() /usr/include/c++/15.2.1/bits/shared_ptr_base.h:1531:7
#68 0x000055daca81aa80 std::shared_ptr<clang::CompilerInvocation>::~shared_ptr() /usr/include/c++/15.2.1/bits/shared_ptr.h:175:11
#69 0x000055daca81aa80 clang::tidy::runClangTidy(clang::tidy::ClangTidyContext&, clang::tooling::CompilationDatabase const&, llvm::ArrayRef<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>>, llvm::IntrusiveRefCntPtr<llvm::vfs::OverlayFileSystem>, bool, bool, llvm::StringRef, bool)::ActionFactory::runInvocation(std::shared_ptr<clang::CompilerInvocation>, clang::FileManager*, std::shared_ptr<clang::PCHContainerOperations>, clang::DiagnosticConsumer*) /home/mitchell/Documents/projects/llvm-project/clang-tools-extra/clang-tidy/ClangTidy.cpp:633:50
#70 0x000055daca9a16ed std::__shared_count<(__gnu_cxx::_Lock_policy)2>::~__shared_count() /usr/include/c++/15.2.1/bits/shared_ptr_base.h:1068:6
#71 0x000055daca9a16ed std::__shared_ptr<clang::CompilerInvocation, (__gnu_cxx::_Lock_policy)2>::~__shared_ptr() /usr/include/c++/15.2.1/bits/shared_ptr_base.h:1531:7
#72 0x000055daca9a16ed std::shared_ptr<clang::CompilerInvocation>::~shared_ptr() /usr/include/c++/15.2.1/bits/shared_ptr.h:175:11
#73 0x000055daca9a16ed clang::tooling::ToolInvocation::runInvocation(char const*, clang::driver::Compilation*, std::shared_ptr<clang::CompilerInvocation>, std::shared_ptr<clang::PCHContainerOperations>) /home/mitchell/Documents/projects/llvm-project/clang/lib/Tooling/Tooling.cpp:438:31
#74 0x000055daca9aff71 clang::tooling::ToolInvocation::run() /home/mitchell/Documents/projects/llvm-project/clang/lib/Tooling/Tooling.cpp:423:23
#75 0x000055daca9bafd3 clang::tooling::ClangTool::run(clang::tooling::ToolAction*) /home/mitchell/Documents/projects/llvm-project/clang/lib/Tooling/Tooling.cpp:633:7
#76 0x000055daca846b7e clang::tidy::runClangTidy(clang::tidy::ClangTidyContext&, clang::tooling::CompilationDatabase const&, llvm::ArrayRef<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>>, llvm::IntrusiveRefCntPtr<llvm::vfs::OverlayFileSystem>, bool, bool, llvm::StringRef, bool) /home/mitchell/Documents/projects/llvm-project/clang-tools-extra/clang-tidy/ClangTidy.cpp:656:28
#77 0x000055dac5618770 llvm::IntrusiveRefCntPtr<llvm::vfs::OverlayFileSystem>::release() /home/mitchell/Documents/projects/llvm-project/llvm/include/llvm/ADT/IntrusiveRefCntPtr.h:232:9
#78 0x000055dac5618770 llvm::IntrusiveRefCntPtr<llvm::vfs::OverlayFileSystem>::~IntrusiveRefCntPtr() /home/mitchell/Documents/projects/llvm-project/llvm/include/llvm/ADT/IntrusiveRefCntPtr.h:196:34
#79 0x000055dac5618770 clang::tidy::clangTidyMain(int, char const**) /home/mitchell/Documents/projects/llvm-project/clang-tools-extra/clang-tidy/tool/ClangTidyMain.cpp:746:19
#80 0x00007f0988827635 (/usr/lib/libc.so.6+0x27635)
#81 0x00007f09888276e9 __libc_start_main (/usr/lib/libc.so.6+0x276e9)
#82 0x000055dac55f5125 _start (./build/bin/clang-tidy+0x3720125)
Aborted                    ./build/bin/clang-tidy repro.cpp -checks='-*,modernize-use-std-format' -config="{CheckOptions: {modernize-use-std-format.StrFormatLikeFunctions: 'String::Printf', modernize-use-std-format.ReplacementFormatFunction: 'fmt::format', modernize-use-std-format.FormatHeader: '<fmt/format.h>'}}" --

```

---

### Comment 3 - zeyi2

Hi, FYI: I can no longer repro the invalid `)"Invalid ({})" static_cast<int>(` fix-it on the latest trunk:

https://clang-tidy.godbolt.org/z/K4GfGY9jE

It is possible that this part of the issue is already fixed, I'll open a PR for the missing `#include` soon.

---

### Comment 4 - zeyi2

Based on https://github.com/llvm/llvm-project/issues/175183#issuecomment-4118313370 and https://github.com/llvm/llvm-project/pull/188247, I believe the issue is now fixed. So I'm closing it. If it doesn't work for your codebase, feel free to reopen it :)

---

