# [ICE-on-valid] CTAD through alias template causes crash in `TemplateArgument::getNonTypeTemplateArgumentType`

**Author:** ericniebler
**URL:** https://github.com/llvm/llvm-project/issues/129998
**Status:** Closed
**Labels:** clang:frontend, confirmed, crash-on-valid
**Closed Date:** 2025-03-22T14:56:01Z

## Body

The following code, compiled with `-std=c++23` with the latest compiler from trunk, causes clang to ICE. I believe the code is conforming, and gcc trunk accepts it.

```c++
#include <concepts>
#include <cstdint>
#include <limits>
#include <span>
#include <type_traits>
#include <utility>

template <auto Nullopt>
struct inline_optional;

namespace detail
{
  template <auto Type>
  concept _integral_wrapper = std::integral<decltype(Type)>;

  template <class... Ts>
  consteval auto _dynamic_extent_of_helper(Ts...) noexcept
  {
    using integral_t = std::common_type_t<Ts...>;
    if constexpr (std::unsigned_integral<integral_t>)
    {
      return std::numeric_limits<integral_t>::max();
    }
    else
    {
      return std::numeric_limits<integral_t>::min();
    }
  }

  template <class... Ts>
  constexpr auto _dynamic_extent_of = detail::_dynamic_extent_of_helper(+Ts{}...);

  template <class Type, class... Ts>
  constexpr auto _tparam_for = (std::integral<Type> ? inline_optional<_dynamic_extent_of<Ts...>>{} : +Type{});
} // namespace detail

template <auto Value>
using constant = std::integral_constant<decltype(Value), Value>;

template <class Type>
concept integral_wrapper = detail::_integral_wrapper<Type::value> && requires {
  { auto(Type::value) } -> std::same_as<typename Type::value_type>;
};

template <class Type>
concept integral_like = std::integral<Type> || integral_wrapper<Type>;

template <auto Nullopt>
struct inline_optional
{
  using value_type  = decltype(Nullopt);

  inline_optional() = default;

  constexpr inline_optional(value_type v) noexcept
      : value_(v)
  {}

  template <auto OtherNullopt>
  constexpr inline_optional(inline_optional<OtherNullopt> other) noexcept
      : value_(other.has_value() ? static_cast<value_type>(other.value()) : Nullopt)
  {
  }

  constexpr auto operator*() const noexcept -> value_type
  {
    return value_;
  }

  constexpr auto value() const noexcept -> value_type
  {
    return value_;
  }

  constexpr auto has_value() const noexcept -> bool
  {
    return value_ != Nullopt;
  }

  auto operator==(inline_optional const& other) const noexcept -> bool = default;

  value_type value_{Nullopt};
  static constexpr value_type nullopt = Nullopt;
};

struct empty
{
  empty() = default;
  constexpr empty(integral_wrapper auto) noexcept {}
};

template <auto V>
using _typeof = decltype(auto(V));

template <class IndexType, IndexType Dyn>
struct _extents
{
  using index_type = IndexType;

  template <index_type... Extents>
  struct type;

  template <index_type A, index_type B>
  struct type<A, B>
  {
    static constexpr auto i = A.value();
    static constexpr auto j = B.value();
    empty _i, _j;
  };

  template <index_type A>
  struct type<A, Dyn>
  {
    static constexpr int i = A.value();
    empty _i;
    int j;
  };

  template <class... Extents>
  type(Extents...) -> type<(std::integral<Extents> ? Dyn : +Extents{})...>;
};

using extent64_t = inline_optional<detail::_dynamic_extent_of<std::uint64_t>>;

template <extent64_t... Extents>
using extents = _extents<extent64_t, {}>::type<Extents...>;

[[deprecated]] void print(auto) {}

int main()
{
  extents<0, {}> e{{}, 42};
  _extents<extent64_t, {}>::type t{constant<1>{},2};
  extents t2{constant<1>{},2};
}
```

The stack trace:

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++23 <source>
1.	<source>:134:30: current parser token ';'
2.	<source>:131:1: parsing function body 'main'
3.	<source>:131:1: in compound statement ('{}')
 #0 0x0000000003b4d588 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3b4d588)
 #1 0x0000000003b4b6cc llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3b4b6cc)
 #2 0x0000000003a9aa78 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007e0449442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x000000000759c68f clang::TemplateArgument::getNonTypeTemplateArgumentType() const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x759c68f)
 #5 0x0000000006b04c68 checkDeducedTemplateArguments(clang::ASTContext&, clang::DeducedTemplateArgument const&, clang::DeducedTemplateArgument const&, bool) (.part.0) SemaTemplateDeduction.cpp:0:0
 #6 0x0000000006b0e8a4 (anonymous namespace)::PackDeductionScope::finish() (.part.0) SemaTemplateDeduction.cpp:0:0
 #7 0x0000000006b5763f clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, bool, bool, clang::QualType, clang::Expr::Classification, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>)>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6b5763f)
 #8 0x000000000695cdc0 clang::Sema::AddTemplateOverloadCandidate(clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::OverloadCandidateSet&, bool, bool, bool, clang::CallExpr::ADLCallKind, clang::OverloadCandidateParamOrder, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x695cdc0)
 #9 0x000000000674533a clang::Sema::DeduceTemplateSpecializationFromInitializer(clang::TypeSourceInfo*, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::MutableArrayRef<clang::Expr*>)::'lambda0'(bool)::operator()(bool) const SemaInit.cpp:0:0
#10 0x0000000006745cb2 clang::Sema::DeduceTemplateSpecializationFromInitializer(clang::TypeSourceInfo*, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::MutableArrayRef<clang::Expr*>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6745cb2)
#11 0x00000000063bba08 clang::Sema::deduceVarTypeFromInitializer(clang::VarDecl*, clang::DeclarationName, clang::QualType, clang::TypeSourceInfo*, clang::SourceRange, bool, clang::Expr*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x63bba08)
#12 0x00000000063d84bb clang::Sema::DeduceVariableDeclarationType(clang::VarDecl*, bool, clang::Expr*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x63d84bb)
#13 0x0000000006400a8c clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6400a8c)
#14 0x0000000006079fee clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6079fee)
#15 0x000000000608f76c clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x608f76c)
#16 0x000000000609228a clang::Parser::ParseSimpleDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, bool, clang::Parser::ForRangeInit*, clang::SourceLocation*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x609228a)
#17 0x000000000609278f clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x609278f)
#18 0x0000000006147ea4 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6147ea4)
#19 0x00000000061487fd clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61487fd)
#20 0x0000000006149b14 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6149b14)
#21 0x000000000614acca clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x614acca)
#22 0x0000000006045bf6 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6045bf6)
#23 0x000000000608fbcf clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x608fbcf)
#24 0x000000000603e1b7 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x603e1b7)
#25 0x000000000603efce clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x603efce)
#26 0x00000000060488d2 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x60488d2)
#27 0x000000000604a168 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x604a168)
#28 0x000000000603880a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x603880a)
#29 0x00000000044c8ff5 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x44c8ff5)
#30 0x00000000047a3241 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x47a3241)
#31 0x0000000004723e7b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4723e7b)
#32 0x0000000004890233 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4890233)
#33 0x0000000000d63321 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xd63321)
#34 0x0000000000d5bb2d ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#35 0x0000000004515569 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#36 0x0000000003a9aea3 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3a9aea3)
#37 0x0000000004515789 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#38 0x00000000044d95ad clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x44d95ad)
#39 0x00000000044da541 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x44da541)
#40 0x00000000044e497c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x44e497c)
#41 0x0000000000d5ffe1 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xd5ffe1)
#42 0x0000000000c21914 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc21914)
#43 0x00007e0449429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#44 0x00007e0449429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#45 0x0000000000d5b5c5 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xd5b5c5)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```

Demo:
https://godbolt.org/z/9WsPov173


EDIT: With assertions enabled, I get:

```
Can't get a NULL template argument here
UNREACHABLE executed at /root/llvm-project/clang/lib/Sema/SemaTemplateDeduction.cpp:2894!
```

https://github.com/llvm/llvm-project/blob/6e7e46cafeccab761d31e6404ceb0cdef4c18bd4/clang/lib/Sema/SemaTemplateDeduction.cpp#L2894


## Comments

### Comment 1 - frederick-vs-ja

Reduced example (https://godbolt.org/z/c569s48hx, https://godbolt.org/z/va3vjW3jf)

```C++
struct converible_to_one {
    constexpr operator int() const noexcept { return 1; }
};

template <int... Extents>
struct class_template {
    class_template() = default;
    constexpr class_template(auto&&...) noexcept {}
};

template <class... Extents>
class_template(Extents...) -> class_template<(true ? 0 : +Extents{})...>;

template <int... Extents>
using alias_template = class_template<Extents...>;

class_template var1{converible_to_one{}, 2}; // OK
alias_template var2{converible_to_one{}, 2};
```

Looks like an issue specific to CTAD for alias templates. CC @hokein.

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Eric Niebler (ericniebler)

<details>
The following code, compiled with `-std=c++23` with the latest compiler from trunk, causes clang to ICE. I believe the code is conforming, and gcc trunk accepts it.

```c++
#include &lt;concepts&gt;
#include &lt;cstdint&gt;
#include &lt;limits&gt;
#include &lt;span&gt;
#include &lt;type_traits&gt;
#include &lt;utility&gt;

template &lt;auto Nullopt&gt;
struct inline_optional;

namespace detail
{
  template &lt;auto Type&gt;
  concept _integral_wrapper = std::integral&lt;decltype(Type)&gt;;

  template &lt;class... Ts&gt;
  consteval auto _dynamic_extent_of_helper(Ts...) noexcept
  {
    using integral_t = std::common_type_t&lt;Ts...&gt;;
    if constexpr (std::unsigned_integral&lt;integral_t&gt;)
    {
      return std::numeric_limits&lt;integral_t&gt;::max();
    }
    else
    {
      return std::numeric_limits&lt;integral_t&gt;::min();
    }
  }

  template &lt;class... Ts&gt;
  constexpr auto _dynamic_extent_of = detail::_dynamic_extent_of_helper(+Ts{}...);

  template &lt;class Type, class... Ts&gt;
  constexpr auto _tparam_for = (std::integral&lt;Type&gt; ? inline_optional&lt;_dynamic_extent_of&lt;Ts...&gt;&gt;{} : +Type{});
} // namespace detail

template &lt;auto Value&gt;
using constant = std::integral_constant&lt;decltype(Value), Value&gt;;

template &lt;class Type&gt;
concept integral_wrapper = detail::_integral_wrapper&lt;Type::value&gt; &amp;&amp; requires {
  { auto(Type::value) } -&gt; std::same_as&lt;typename Type::value_type&gt;;
};

template &lt;class Type&gt;
concept integral_like = std::integral&lt;Type&gt; || integral_wrapper&lt;Type&gt;;

template &lt;auto Nullopt&gt;
struct inline_optional
{
  using value_type  = decltype(Nullopt);

  inline_optional() = default;

  constexpr inline_optional(value_type v) noexcept
      : value_(v)
  {}

  template &lt;auto OtherNullopt&gt;
  constexpr inline_optional(inline_optional&lt;OtherNullopt&gt; other) noexcept
      : value_(other.has_value() ? static_cast&lt;value_type&gt;(other.value()) : Nullopt)
  {
  }

  constexpr auto operator*() const noexcept -&gt; value_type
  {
    return value_;
  }

  constexpr auto value() const noexcept -&gt; value_type
  {
    return value_;
  }

  constexpr auto has_value() const noexcept -&gt; bool
  {
    return value_ != Nullopt;
  }

  auto operator==(inline_optional const&amp; other) const noexcept -&gt; bool = default;

  value_type value_{Nullopt};
  static constexpr value_type nullopt = Nullopt;
};

struct empty
{
  empty() = default;
  constexpr empty(integral_wrapper auto) noexcept {}
};

template &lt;auto V&gt;
using _typeof = decltype(auto(V));

template &lt;class IndexType, IndexType Dyn&gt;
struct _extents
{
  using index_type = IndexType;

  template &lt;index_type... Extents&gt;
  struct type;

  template &lt;index_type A, index_type B&gt;
  struct type&lt;A, B&gt;
  {
    static constexpr auto i = A.value();
    static constexpr auto j = B.value();
    empty _i, _j;
  };

  template &lt;index_type A&gt;
  struct type&lt;A, Dyn&gt;
  {
    static constexpr int i = A.value();
    empty _i;
    int j;
  };

  template &lt;class... Extents&gt;
  type(Extents...) -&gt; type&lt;(std::integral&lt;Extents&gt; ? Dyn : +Extents{})...&gt;;
};

using extent64_t = inline_optional&lt;detail::_dynamic_extent_of&lt;std::uint64_t&gt;&gt;;

template &lt;extent64_t... Extents&gt;
using extents = _extents&lt;extent64_t, {}&gt;::type&lt;Extents...&gt;;

[[deprecated]] void print(auto) {}

int main()
{
  extents&lt;0, {}&gt; e{{}, 42};
  _extents&lt;extent64_t, {}&gt;::type t{constant&lt;1&gt;{},2};
  extents t2{constant&lt;1&gt;{},2};
}
```

The stack trace:

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++23 &lt;source&gt;
1.	&lt;source&gt;:134:30: current parser token ';'
2.	&lt;source&gt;:131:1: parsing function body 'main'
3.	&lt;source&gt;:131:1: in compound statement ('{}')
 #<!-- -->0 0x0000000003b4d588 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3b4d588)
 #<!-- -->1 0x0000000003b4b6cc llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3b4b6cc)
 #<!-- -->2 0x0000000003a9aa78 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007e0449442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x000000000759c68f clang::TemplateArgument::getNonTypeTemplateArgumentType() const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x759c68f)
 #<!-- -->5 0x0000000006b04c68 checkDeducedTemplateArguments(clang::ASTContext&amp;, clang::DeducedTemplateArgument const&amp;, clang::DeducedTemplateArgument const&amp;, bool) (.part.0) SemaTemplateDeduction.cpp:0:0
 #<!-- -->6 0x0000000006b0e8a4 (anonymous namespace)::PackDeductionScope::finish() (.part.0) SemaTemplateDeduction.cpp:0:0
 #<!-- -->7 0x0000000006b5763f clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, bool, bool, bool, clang::QualType, clang::Expr::Classification, llvm::function_ref&lt;bool (llvm::ArrayRef&lt;clang::QualType&gt;)&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6b5763f)
 #<!-- -->8 0x000000000695cdc0 clang::Sema::AddTemplateOverloadCandidate(clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::OverloadCandidateSet&amp;, bool, bool, bool, clang::CallExpr::ADLCallKind, clang::OverloadCandidateParamOrder, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x695cdc0)
 #<!-- -->9 0x000000000674533a clang::Sema::DeduceTemplateSpecializationFromInitializer(clang::TypeSourceInfo*, clang::InitializedEntity const&amp;, clang::InitializationKind const&amp;, llvm::MutableArrayRef&lt;clang::Expr*&gt;)::'lambda0'(bool)::operator()(bool) const SemaInit.cpp:0:0
#<!-- -->10 0x0000000006745cb2 clang::Sema::DeduceTemplateSpecializationFromInitializer(clang::TypeSourceInfo*, clang::InitializedEntity const&amp;, clang::InitializationKind const&amp;, llvm::MutableArrayRef&lt;clang::Expr*&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6745cb2)
#<!-- -->11 0x00000000063bba08 clang::Sema::deduceVarTypeFromInitializer(clang::VarDecl*, clang::DeclarationName, clang::QualType, clang::TypeSourceInfo*, clang::SourceRange, bool, clang::Expr*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x63bba08)
#<!-- -->12 0x00000000063d84bb clang::Sema::DeduceVariableDeclarationType(clang::VarDecl*, bool, clang::Expr*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x63d84bb)
#<!-- -->13 0x0000000006400a8c clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6400a8c)
#<!-- -->14 0x0000000006079fee clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6079fee)
#<!-- -->15 0x000000000608f76c clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x608f76c)
#<!-- -->16 0x000000000609228a clang::Parser::ParseSimpleDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, bool, clang::Parser::ForRangeInit*, clang::SourceLocation*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x609228a)
#<!-- -->17 0x000000000609278f clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x609278f)
#<!-- -->18 0x0000000006147ea4 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6147ea4)
#<!-- -->19 0x00000000061487fd clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61487fd)
#<!-- -->20 0x0000000006149b14 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6149b14)
#<!-- -->21 0x000000000614acca clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x614acca)
#<!-- -->22 0x0000000006045bf6 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6045bf6)
#<!-- -->23 0x000000000608fbcf clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x608fbcf)
#<!-- -->24 0x000000000603e1b7 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x603e1b7)
#<!-- -->25 0x000000000603efce clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x603efce)
#<!-- -->26 0x00000000060488d2 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x60488d2)
#<!-- -->27 0x000000000604a168 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x604a168)
#<!-- -->28 0x000000000603880a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x603880a)
#<!-- -->29 0x00000000044c8ff5 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x44c8ff5)
#<!-- -->30 0x00000000047a3241 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x47a3241)
#<!-- -->31 0x0000000004723e7b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4723e7b)
#<!-- -->32 0x0000000004890233 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4890233)
#<!-- -->33 0x0000000000d63321 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xd63321)
#<!-- -->34 0x0000000000d5bb2d ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->35 0x0000000004515569 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->36 0x0000000003a9aea3 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3a9aea3)
#<!-- -->37 0x0000000004515789 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->38 0x00000000044d95ad clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x44d95ad)
#<!-- -->39 0x00000000044da541 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x44da541)
#<!-- -->40 0x00000000044e497c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x44e497c)
#<!-- -->41 0x0000000000d5ffe1 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xd5ffe1)
#<!-- -->42 0x0000000000c21914 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc21914)
#<!-- -->43 0x00007e0449429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->44 0x00007e0449429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->45 0x0000000000d5b5c5 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xd5b5c5)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```

Demo:
https://godbolt.org/z/9WsPov173


EDIT: With assertions enabled, I get:

```
Can't get a NULL template argument here
UNREACHABLE executed at /root/llvm-project/clang/lib/Sema/SemaTemplateDeduction.cpp:2894!
```

https://github.com/llvm/llvm-project/blob/6e7e46cafeccab761d31e6404ceb0cdef4c18bd4/clang/lib/Sema/SemaTemplateDeduction.cpp#L2894

</details>


---

### Comment 3 - cor3ntin

@hokein 

---

### Comment 4 - MagentaTreehouse

Looks like a variant of #127539.

---

