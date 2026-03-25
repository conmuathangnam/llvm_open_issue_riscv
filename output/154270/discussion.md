# [clang] unexpected type class: UNREACHABLE executed at ... TreeTransform.h

**Author:** rupprecht
**URL:** https://github.com/llvm/llvm-project/issues/154270
**Status:** Closed
**Labels:** clang:frontend, regression, confirmed, crash-on-valid
**Closed Date:** 2025-08-19T21:32:18Z

## Body

Failure bisects to #147835. Live repro link: https://compiler-explorer.com/z/7TYx3b8bq

Reduced source:
```c++
#include <utility>

template <std::size_t... a, typename b>
bool c(b d, std::index_sequence<a...>) {
    return d(std::integral_constant<long, a>{}...);
}
template <int e, typename b>
bool f(b d) {
    return c(d, std::make_index_sequence<e>{});
}
struct g {
    ~g();
    virtual bool h(const int&) const;
};
template <typename i>
struct j : g {
    j(i);
    bool h(const int& k) const { return l.m(k); }
    i l;
};
template <typename n>
struct o {
    o() { j{*u}; }
    n* u;
};
template <int, typename... p>
struct q {
    static bool m(int) {
        return f<sizeof...(p)>([](auto... a) { return (p::m(a) && ...); });
    }
};
enum { r, s };
void t() { o<q<r, q<s>>>(); }
```

Crash:
```
unexpected type class
UNREACHABLE executed at /root/llvm-project/llvm/tools/clang/lib/Sema/TreeTransform.h:5430!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<eof> parser at end of file
2.	<source>:23:5: instantiating function definition 'o<q<0, q<1>>>::o'
3.	<source>:18:10: instantiating function definition 'j<q<0, q<1>>>::h'
4.	<source>:28:17: instantiating function definition 'q<0, q<1>>::m'
5.	<source>:8:6: instantiating function definition 'f<1, (lambda at <source>:29:32)>'
6.	<source>:4:6: instantiating function definition 'c<0UL, (lambda at <source>:29:32)>'
7.	<source>:29:32: instantiating function definition 'q<0, q<1>>::m(int)::(anonymous class)::operator()<std::integral_constant<long, 0>>'
...
 #8 0x000000000722d32d clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformTypeInObjectScope(clang::TypeLocBuilder&, clang::TypeLoc, clang::QualType, clang::NamedDecl*) SemaTemplateInstantiate.cpp:0:0
 #9 0x000000000722da9f clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformNestedNameSpecifierLoc(clang::NestedNameSpecifierLoc, clang::QualType, clang::NamedDecl*) SemaTemplateInstantiate.cpp:0:0
#10 0x0000000007235c08 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformDependentScopeDeclRefExpr(clang::DependentScopeDeclRefExpr*, bool, clang::TypeSourceInfo**) SemaTemplateInstantiate.cpp:0:0
#11 0x0000000007219b5d clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#12 0x000000000724b2d6 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCallExpr(clang::CallExpr*) SemaTemplateInstantiate.cpp:0:0
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Jordan Rupprecht (rupprecht)

<details>
Failure bisects to #<!-- -->147835. Live repro link: https://compiler-explorer.com/z/7TYx3b8bq

Reduced source:
```c++
#include &lt;utility&gt;

template &lt;std::size_t... a, typename b&gt;
bool c(b d, std::index_sequence&lt;a...&gt;) {
    return d(std::integral_constant&lt;long, a&gt;{}...);
}
template &lt;int e, typename b&gt;
bool f(b d) {
    return c(d, std::make_index_sequence&lt;e&gt;{});
}
struct g {
    ~g();
    virtual bool h(const int&amp;) const;
};
template &lt;typename i&gt;
struct j : g {
    j(i);
    bool h(const int&amp; k) const { return l.m(k); }
    i l;
};
template &lt;typename n&gt;
struct o {
    o() { j{*u}; }
    n* u;
};
template &lt;int, typename... p&gt;
struct q {
    static bool m(int) {
        return f&lt;sizeof...(p)&gt;([](auto... a) { return (p::m(a) &amp;&amp; ...); });
    }
};
enum { r, s };
void t() { o&lt;q&lt;r, q&lt;s&gt;&gt;&gt;(); }
```

Crash:
```
unexpected type class
UNREACHABLE executed at /root/llvm-project/llvm/tools/clang/lib/Sema/TreeTransform.h:5430!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	&lt;source&gt;:23:5: instantiating function definition 'o&lt;q&lt;0, q&lt;1&gt;&gt;&gt;::o'
3.	&lt;source&gt;:18:10: instantiating function definition 'j&lt;q&lt;0, q&lt;1&gt;&gt;&gt;::h'
4.	&lt;source&gt;:28:17: instantiating function definition 'q&lt;0, q&lt;1&gt;&gt;::m'
5.	&lt;source&gt;:8:6: instantiating function definition 'f&lt;1, (lambda at &lt;source&gt;:29:32)&gt;'
6.	&lt;source&gt;:4:6: instantiating function definition 'c&lt;0UL, (lambda at &lt;source&gt;:29:32)&gt;'
7.	&lt;source&gt;:29:32: instantiating function definition 'q&lt;0, q&lt;1&gt;&gt;::m(int)::(anonymous class)::operator()&lt;std::integral_constant&lt;long, 0&gt;&gt;'
...
 #<!-- -->8 0x000000000722d32d clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformTypeInObjectScope(clang::TypeLocBuilder&amp;, clang::TypeLoc, clang::QualType, clang::NamedDecl*) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->9 0x000000000722da9f clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformNestedNameSpecifierLoc(clang::NestedNameSpecifierLoc, clang::QualType, clang::NamedDecl*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->10 0x0000000007235c08 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformDependentScopeDeclRefExpr(clang::DependentScopeDeclRefExpr*, bool, clang::TypeSourceInfo**) SemaTemplateInstantiate.cpp:0:0
#<!-- -->11 0x0000000007219b5d clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->12 0x000000000724b2d6 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCallExpr(clang::CallExpr*) SemaTemplateInstantiate.cpp:0:0
```
</details>


---

### Comment 2 - shafik

CC @mizvekov 

---

