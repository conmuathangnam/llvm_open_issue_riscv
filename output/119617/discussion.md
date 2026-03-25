# [Clang] FE crash on `std::greater<void>{}(ext_vector_type, ext_vector_type)`

**Author:** aelovikov-intel
**URL:** https://github.com/llvm/llvm-project/issues/119617

## Body

https://godbolt.org/z/P6daoG6jW

```
#include <functional>
using vector_t = int __attribute__((ext_vector_type(16)));
auto foo(vector_t a, vector_t b) {
  return std::greater<void>{}(a, b);
}
```

Fails with
```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++17 <source>
1.	<source>:4:35: current parser token ')'
2.	<source>:3:34: parsing function body 'foo'
3.	<source>:3:34: in compound statement ('{}')
4.	/opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/15.0.0/../../../../include/c++/15.0.0/bits/stl_function.h:524:2: instantiating function definition 'std::greater<>::operator()<int & __attribute__((ext_vector_type(16))), int & __attribute__((ext_vector_type(16)))>'
5.	/opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/15.0.0/../../../../include/c++/15.0.0/type_traits:203:12: instantiating class definition 'std::__and_<std::greater<>::__not_overloaded<int & __attribute__((ext_vector_type(16))), int & __attribute__((ext_vector_type(16)))>, std::is_convertible<int & __attribute__((ext_vector_type(16))), const volatile void *>, std::is_convertible<int & __attribute__((ext_vector_type(16))), const volatile void *>>'
6.	/opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/15.0.0/../../../../include/c++/15.0.0/bits/stl_function.h:566:9: instantiating class definition 'std::greater<>::__not_overloaded<int & __attribute__((ext_vector_type(16))), int & __attribute__((ext_vector_type(16)))>'
  #0 0x0000000003c17778 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3c17778)
  #1 0x0000000003c15484 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3c15484)
  #2 0x0000000003b629e8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
  #3 0x000071bb73242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
  #4 0x0000000006bbd08c LookupMemberExpr(clang::Sema&, clang::LookupResult&, clang::ActionResult<clang::Expr*, true>&, bool&, clang::SourceLocation, clang::CXXScopeSpec&, clang::Decl*, bool, clang::SourceLocation) SemaExprMember.cpp:0:0
  #5 0x0000000006bc071b clang::Sema::BuildMemberReferenceExpr(clang::Expr*, clang::QualType, clang::SourceLocation, bool, clang::CXXScopeSpec&, clang::SourceLocation, clang::NamedDecl*, clang::DeclarationNameInfo const&, clang::TemplateArgumentListInfo const*, clang::Scope const*, clang::Sema::ActOnMemberAccessExtraArgs*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bc071b)
  #6 0x000000000706f44b clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCXXDependentScopeMemberExpr(clang::CXXDependentScopeMemberExpr*) SemaTemplateInstantiate.cpp:0:0
  #7 0x0000000007046bf9 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
  #8 0x00000000070597be clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCallExpr(clang::CallExpr*) SemaTemplateInstantiate.cpp:0:0
  #9 0x0000000007046cee clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
 #10 0x000000000703e097 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
```


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (aelovikov-intel)

<details>
https://godbolt.org/z/P6daoG6jW

```
#include &lt;functional&gt;
using vector_t = int __attribute__((ext_vector_type(16)));
auto foo(vector_t a, vector_t b) {
  return std::greater&lt;void&gt;{}(a, b);
}
```

Fails with
```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++17 &lt;source&gt;
1.	&lt;source&gt;:4:35: current parser token ')'
2.	&lt;source&gt;:3:34: parsing function body 'foo'
3.	&lt;source&gt;:3:34: in compound statement ('{}')
4.	/opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/15.0.0/../../../../include/c++/15.0.0/bits/stl_function.h:524:2: instantiating function definition 'std::greater&lt;&gt;::operator()&lt;int &amp; __attribute__((ext_vector_type(16))), int &amp; __attribute__((ext_vector_type(16)))&gt;'
5.	/opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/15.0.0/../../../../include/c++/15.0.0/type_traits:203:12: instantiating class definition 'std::__and_&lt;std::greater&lt;&gt;::__not_overloaded&lt;int &amp; __attribute__((ext_vector_type(16))), int &amp; __attribute__((ext_vector_type(16)))&gt;, std::is_convertible&lt;int &amp; __attribute__((ext_vector_type(16))), const volatile void *&gt;, std::is_convertible&lt;int &amp; __attribute__((ext_vector_type(16))), const volatile void *&gt;&gt;'
6.	/opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/15.0.0/../../../../include/c++/15.0.0/bits/stl_function.h:566:9: instantiating class definition 'std::greater&lt;&gt;::__not_overloaded&lt;int &amp; __attribute__((ext_vector_type(16))), int &amp; __attribute__((ext_vector_type(16)))&gt;'
  #<!-- -->0 0x0000000003c17778 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3c17778)
  #<!-- -->1 0x0000000003c15484 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3c15484)
  #<!-- -->2 0x0000000003b629e8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
  #<!-- -->3 0x000071bb73242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
  #<!-- -->4 0x0000000006bbd08c LookupMemberExpr(clang::Sema&amp;, clang::LookupResult&amp;, clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, bool&amp;, clang::SourceLocation, clang::CXXScopeSpec&amp;, clang::Decl*, bool, clang::SourceLocation) SemaExprMember.cpp:0:0
  #<!-- -->5 0x0000000006bc071b clang::Sema::BuildMemberReferenceExpr(clang::Expr*, clang::QualType, clang::SourceLocation, bool, clang::CXXScopeSpec&amp;, clang::SourceLocation, clang::NamedDecl*, clang::DeclarationNameInfo const&amp;, clang::TemplateArgumentListInfo const*, clang::Scope const*, clang::Sema::ActOnMemberAccessExtraArgs*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bc071b)
  #<!-- -->6 0x000000000706f44b clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCXXDependentScopeMemberExpr(clang::CXXDependentScopeMemberExpr*) SemaTemplateInstantiate.cpp:0:0
  #<!-- -->7 0x0000000007046bf9 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
  #<!-- -->8 0x00000000070597be clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCallExpr(clang::CallExpr*) SemaTemplateInstantiate.cpp:0:0
  #<!-- -->9 0x0000000007046cee clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->10 0x000000000703e097 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeLocBuilder&amp;, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
```

</details>


---

### Comment 2 - shafik

Looks like this goes back to clang-7: https://godbolt.org/z/5xWfndvve

---

### Comment 3 - frederick-vs-ja

A library-free reduced example ([Godbolt link](https://godbolt.org/z/enYGsGEbb)):
```C++
namespace my {
  template<class T>
  T&& quick_declval() noexcept { static_assert((sizeof(T), false), ""); }

  template<class...>
  using void_t = void;

  template<class T, class U, class = void>
  struct base {};
  template<class T, class U>
  struct base<T, U, void_t<decltype(my::quick_declval<T>().operator>(my::quick_declval<U>()))>> {};

  template<class T, class U, class = void>
  struct result : base<T, U> {};
  template<class T, class U>
  struct result<T, U, void_t<decltype(operator>(my::quick_declval<T>(), my::quick_declval<T>()))>> {};
}

void foo() {
  using vector_t = int __attribute__((ext_vector_type(16)));
  my::result<vector_t&, vector_t&>{};
}
```

> Looks like this goes back to clang-7: https://godbolt.org/z/5xWfndvve

The bug has been existing even earlier as the reduced example caused ICE at least since Clang 3.1. Its later reveal was due to the changes in libstdc++.

---

