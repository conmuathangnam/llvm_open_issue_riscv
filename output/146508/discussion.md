# Clang SISEGV in addAssociatedClassesAndNamespaces since clang 9.0

**Author:** mariete1223
**URL:** https://github.com/llvm/llvm-project/issues/146508

## Body

I have not been able to reduce it more. The original version without reducing gave the error addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&, clang::QualType) SemaLookup.cpp:0:0, repeatedly until stack exhaustion, the reduced one does not seem to give any feedback, so both versions will be posted.

## Stack dump:

```
#0 0x00005c1e0215318f llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/usr/local/bin/clang-21+0x3c4a18f)
  #1 0x00005c1e02150bd4 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
  #2 0x0000723f94a83420 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x14420)
  #3 0x00005c1e04ab0330 addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&, clang::QualType) SemaLookup.cpp:0:0
  #4 0x00005c1e04ab2eea addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&, clang::TemplateArgument const&) SemaLookup.cpp:0:0
  #5 0x00005c1e04ab2f38 addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&, clang::TemplateArgument const&) SemaLookup.cpp:0:0
  #6 0x00005c1e04ab08bf addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&, clang::QualType) SemaLookup.cpp:0:0
  #7 0x00005c1e04ab2eea addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&, clang::TemplateArgument const&) SemaLookup.cpp:0:0
  #8 0x00005c1e04ab2f38 addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&, clang::TemplateArgument const&) SemaLookup.cpp:0:0
  #9 0x00005c1e04ab08bf addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&, clang::QualType) SemaLookup.cpp:0:0
 #10 0x00005c1e04ab2eea addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&, clang::TemplateArgument const&) SemaLookup.cpp:0:0
 #11 0x00005c1e04ab2f38 addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&, clang::TemplateArgument const&) SemaLookup.cpp:0:0
 #12 0x00005c1e04ab08bf addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&, clang::QualType) SemaLookup.cpp:0:0
 #13 0x00005c1e04ab2eea addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&, clang::TemplateArgument const&) SemaLookup.cpp:0:0
 #14 0x00005c1e04ab2f38 addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&, clang::TemplateArgument const&) SemaLookup.cpp:0:0
 #15 0x00005c1e04ab08bf addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&, clang::QualType) SemaLookup.cpp:0:0
 #16 0x00005c1e04ab2eea addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&, clang::TemplateArgument const&) SemaLookup.cpp:0:0
 #17 0x00005c1e04ab2f38 addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&, clang::TemplateArgument const&) SemaLookup.cpp:0:0
 #18 0x00005c1e04ab08bf addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&, clang::QualType) SemaLookup.cpp:0:0
 #19 0x00005c1e04ab2eea addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&, clang::TemplateArgument const&) SemaLookup.cpp:0:0
 #20 0x00005c1e04ab2f38 addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&, clang::TemplateArgument const&) SemaLookup.cpp:0:0
 #21 0x00005c1e04ab08bf addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&, clang::QualType) SemaLookup.cpp:0:0
 #22 0x00005c1e04ab2eea addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&, clang::TemplateArgument const&) SemaLookup.cpp:0:0
 #23 0x00005c1e04ab2f38 addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&, clang::TemplateArgument const&) SemaLookup.cpp:0:0
 #24 0x00005c1e04ab08bf addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&, clang::QualType) SemaLookup.cpp:0:0
 #25 0x00005c1e04ab2eea addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&, clang::TemplateArgument const&) SemaLookup.cpp:0:0
 #26 0x00005c1e04ab2f38 addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&, clang::TemplateArgument const&) SemaLookup.cpp:0:0
 #27 0x00005c1e04ab08bf addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&, clang::QualType) SemaLookup.cpp:0:0
 #28 0x00005c1e04ab2eea addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&, clang::TemplateArgument const&) SemaLookup.cpp:0:0
 #29 0x00005c1e04ab2f38 addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&, clang::TemplateArgument const&) SemaLookup.cpp:0:0
 #30 0x00005c1e04ab08bf addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&, clang::QualType) SemaLookup.cpp:0:0
 #31 0x00005c1e04ab2eea addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&, clang::TemplateArgument const&) SemaLookup.cpp:0:0
 #32 0x00005c1e04ab2f38 addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&, clang::TemplateArgument const&) SemaLookup.cpp:0:0
 #33 0x00005c1e04ab08bf addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&, clang::QualType) SemaLookup.cpp:0:0
 #34 0x00005c1e04ab2eea addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&, clang::TemplateArgument const&) SemaLookup.cpp:0:0
 #35 0x00005c1e04ab2f38 addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&, clang::TemplateArgument const&) SemaLookup.cpp:0:0
 #36 0x00005c1e04ab08bf addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&, clang::QualType) SemaLookup.cpp:0:0
 #37 0x00005c1e04ab2eea addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&, clang::TemplateArgument const&) SemaLookup.cpp:0:0
 #38 0x00005c1e04ab2f38 addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&, clang::TemplateArgument const&) SemaLookup.cpp:0:0
 #39 0x00005c1e04ab08bf addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&, clang::QualType) SemaLookup.cpp:0:0
 #40 0x00005c1e04ab2eea addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&, clang::TemplateArgument const&) SemaLookup.cpp:0:0
 #41 0x00005c1e04ab2f38 addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&, clang::TemplateArgument const&) SemaLookup.cpp:0:0
 #42 0x00005c1e04ab08bf addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&, clang::QualType) SemaLookup.cpp:0:0
...
#254 0x00005c1e04ab2f38 addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&, clang::TemplateArgument const&) SemaLookup.cpp:0:0
#255 0x00005c1e04ab08bf addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&, clang::QualType) SemaLookup.cpp:0:0
```

## Program slightly reduced:

```
#include <tuple>
#include <iostream>

template <typename F>
auto apply(F&& f) ->
    decltype(apply(std::forward<F>(f)))
{
}
template <typename F, typename... Args>
auto apply(F&& f, Args&&... args) ->
    decltype(apply(std::forward<F>(f),
                  std::forward_as_tuple(std::forward<Args>(args)...)))
{
    return apply(
        std::forward<F>(f)
    );
}
int main() {
    apply(
        [](int x) {
            return x;
        },
        10
    );

}
```

Original:

```
#include <tuple>
#include <iostream>
template <typename F, typename S, size_t... Is>
auto apply(F&& f, S&& t, std::index_sequence<Is...> is) ->
    decltype(std::apply(std::forward<F>(f), std::forward<S>(t), is...))
{
    return std::apply(std::forward<F>(f), std::forward<S>(t), is...);
}
template <typename F, typename... Args>
auto apply(F&& f, Args&&... args) ->
    decltype(apply(std::forward<F>(f),
                  std::forward_as_tuple(std::forward<Args>(args)...),
                  std::make_index_sequence<sizeof...(Args)>{}))
{
    return apply(
        std::forward<F>(f),
        std::forward_as_tuple(std::forward<Args>(args)...),
        std::make_index_sequence<sizeof...(Args)>{}
    );
}
int main() {
    std::vector<int> vec{1, 2, 3, 4, 5, 6, 7};
    apply(
        [&vec](int x) {
            vec.push_back(x * 2);
        },
        10
    );
}
```

To quickly reproduce:
https://godbolt.org/z/vx3T89eax

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (mariete1223)

<details>
I have not been able to reduce it more. The original version without reducing gave the error addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&amp;, clang::QualType) SemaLookup.cpp:0:0, repeatedly until stack exhaustion, the reduced one does not seem to give any feedback, so both versions will be posted.

## Stack dump:

```
#<!-- -->0 0x00005c1e0215318f llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/usr/local/bin/clang-21+0x3c4a18f)
  #<!-- -->1 0x00005c1e02150bd4 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
  #<!-- -->2 0x0000723f94a83420 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x14420)
  #<!-- -->3 0x00005c1e04ab0330 addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&amp;, clang::QualType) SemaLookup.cpp:0:0
  #<!-- -->4 0x00005c1e04ab2eea addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&amp;, clang::TemplateArgument const&amp;) SemaLookup.cpp:0:0
  #<!-- -->5 0x00005c1e04ab2f38 addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&amp;, clang::TemplateArgument const&amp;) SemaLookup.cpp:0:0
  #<!-- -->6 0x00005c1e04ab08bf addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&amp;, clang::QualType) SemaLookup.cpp:0:0
  #<!-- -->7 0x00005c1e04ab2eea addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&amp;, clang::TemplateArgument const&amp;) SemaLookup.cpp:0:0
  #<!-- -->8 0x00005c1e04ab2f38 addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&amp;, clang::TemplateArgument const&amp;) SemaLookup.cpp:0:0
  #<!-- -->9 0x00005c1e04ab08bf addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&amp;, clang::QualType) SemaLookup.cpp:0:0
 #<!-- -->10 0x00005c1e04ab2eea addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&amp;, clang::TemplateArgument const&amp;) SemaLookup.cpp:0:0
 #<!-- -->11 0x00005c1e04ab2f38 addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&amp;, clang::TemplateArgument const&amp;) SemaLookup.cpp:0:0
 #<!-- -->12 0x00005c1e04ab08bf addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&amp;, clang::QualType) SemaLookup.cpp:0:0
 #<!-- -->13 0x00005c1e04ab2eea addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&amp;, clang::TemplateArgument const&amp;) SemaLookup.cpp:0:0
 #<!-- -->14 0x00005c1e04ab2f38 addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&amp;, clang::TemplateArgument const&amp;) SemaLookup.cpp:0:0
 #<!-- -->15 0x00005c1e04ab08bf addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&amp;, clang::QualType) SemaLookup.cpp:0:0
 #<!-- -->16 0x00005c1e04ab2eea addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&amp;, clang::TemplateArgument const&amp;) SemaLookup.cpp:0:0
 #<!-- -->17 0x00005c1e04ab2f38 addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&amp;, clang::TemplateArgument const&amp;) SemaLookup.cpp:0:0
 #<!-- -->18 0x00005c1e04ab08bf addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&amp;, clang::QualType) SemaLookup.cpp:0:0
 #<!-- -->19 0x00005c1e04ab2eea addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&amp;, clang::TemplateArgument const&amp;) SemaLookup.cpp:0:0
 #<!-- -->20 0x00005c1e04ab2f38 addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&amp;, clang::TemplateArgument const&amp;) SemaLookup.cpp:0:0
 #<!-- -->21 0x00005c1e04ab08bf addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&amp;, clang::QualType) SemaLookup.cpp:0:0
 #<!-- -->22 0x00005c1e04ab2eea addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&amp;, clang::TemplateArgument const&amp;) SemaLookup.cpp:0:0
 #<!-- -->23 0x00005c1e04ab2f38 addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&amp;, clang::TemplateArgument const&amp;) SemaLookup.cpp:0:0
 #<!-- -->24 0x00005c1e04ab08bf addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&amp;, clang::QualType) SemaLookup.cpp:0:0
 #<!-- -->25 0x00005c1e04ab2eea addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&amp;, clang::TemplateArgument const&amp;) SemaLookup.cpp:0:0
 #<!-- -->26 0x00005c1e04ab2f38 addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&amp;, clang::TemplateArgument const&amp;) SemaLookup.cpp:0:0
 #<!-- -->27 0x00005c1e04ab08bf addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&amp;, clang::QualType) SemaLookup.cpp:0:0
 #<!-- -->28 0x00005c1e04ab2eea addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&amp;, clang::TemplateArgument const&amp;) SemaLookup.cpp:0:0
 #<!-- -->29 0x00005c1e04ab2f38 addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&amp;, clang::TemplateArgument const&amp;) SemaLookup.cpp:0:0
 #<!-- -->30 0x00005c1e04ab08bf addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&amp;, clang::QualType) SemaLookup.cpp:0:0
 #<!-- -->31 0x00005c1e04ab2eea addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&amp;, clang::TemplateArgument const&amp;) SemaLookup.cpp:0:0
 #<!-- -->32 0x00005c1e04ab2f38 addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&amp;, clang::TemplateArgument const&amp;) SemaLookup.cpp:0:0
 #<!-- -->33 0x00005c1e04ab08bf addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&amp;, clang::QualType) SemaLookup.cpp:0:0
 #<!-- -->34 0x00005c1e04ab2eea addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&amp;, clang::TemplateArgument const&amp;) SemaLookup.cpp:0:0
 #<!-- -->35 0x00005c1e04ab2f38 addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&amp;, clang::TemplateArgument const&amp;) SemaLookup.cpp:0:0
 #<!-- -->36 0x00005c1e04ab08bf addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&amp;, clang::QualType) SemaLookup.cpp:0:0
 #<!-- -->37 0x00005c1e04ab2eea addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&amp;, clang::TemplateArgument const&amp;) SemaLookup.cpp:0:0
 #<!-- -->38 0x00005c1e04ab2f38 addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&amp;, clang::TemplateArgument const&amp;) SemaLookup.cpp:0:0
 #<!-- -->39 0x00005c1e04ab08bf addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&amp;, clang::QualType) SemaLookup.cpp:0:0
 #<!-- -->40 0x00005c1e04ab2eea addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&amp;, clang::TemplateArgument const&amp;) SemaLookup.cpp:0:0
 #<!-- -->41 0x00005c1e04ab2f38 addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&amp;, clang::TemplateArgument const&amp;) SemaLookup.cpp:0:0
 #<!-- -->42 0x00005c1e04ab08bf addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&amp;, clang::QualType) SemaLookup.cpp:0:0
...
#<!-- -->254 0x00005c1e04ab2f38 addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&amp;, clang::TemplateArgument const&amp;) SemaLookup.cpp:0:0
#<!-- -->255 0x00005c1e04ab08bf addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&amp;, clang::QualType) SemaLookup.cpp:0:0
```

## Program slightly reduced:

```
#include &lt;tuple&gt;
#include &lt;iostream&gt;

template &lt;typename F&gt;
auto apply(F&amp;&amp; f) -&gt;
    decltype(apply(std::forward&lt;F&gt;(f)))
{
}
template &lt;typename F, typename... Args&gt;
auto apply(F&amp;&amp; f, Args&amp;&amp;... args) -&gt;
    decltype(apply(std::forward&lt;F&gt;(f),
                  std::forward_as_tuple(std::forward&lt;Args&gt;(args)...)))
{
    return apply(
        std::forward&lt;F&gt;(f)
    );
}
int main() {
    apply(
        [](int x) {
            return x;
        },
        10
    );

}
```

Original:

```
#include &lt;tuple&gt;
#include &lt;iostream&gt;
template &lt;typename F, typename S, size_t... Is&gt;
auto apply(F&amp;&amp; f, S&amp;&amp; t, std::index_sequence&lt;Is...&gt; is) -&gt;
    decltype(std::apply(std::forward&lt;F&gt;(f), std::forward&lt;S&gt;(t), is...))
{
    return std::apply(std::forward&lt;F&gt;(f), std::forward&lt;S&gt;(t), is...);
}
template &lt;typename F, typename... Args&gt;
auto apply(F&amp;&amp; f, Args&amp;&amp;... args) -&gt;
    decltype(apply(std::forward&lt;F&gt;(f),
                  std::forward_as_tuple(std::forward&lt;Args&gt;(args)...),
                  std::make_index_sequence&lt;sizeof...(Args)&gt;{}))
{
    return apply(
        std::forward&lt;F&gt;(f),
        std::forward_as_tuple(std::forward&lt;Args&gt;(args)...),
        std::make_index_sequence&lt;sizeof...(Args)&gt;{}
    );
}
int main() {
    std::vector&lt;int&gt; vec{1, 2, 3, 4, 5, 6, 7};
    apply(
        [&amp;vec](int x) {
            vec.push_back(x * 2);
        },
        10
    );
}
```

To quickly reproduce:
https://godbolt.org/z/vx3T89eax
</details>


---

### Comment 2 - shafik

I don't think this is a bug, it looks like infinite recursion, everyone fails on this: https://godbolt.org/z/EhdzY95Ke

---

### Comment 3 - zygoloid

We should ideally be able to detect this via an instantiation depth limit and produce a proper diagnostic rather than crashing. It looks like we're also missing a `runWithSufficientStackSpace` call on this recursive path.

---

