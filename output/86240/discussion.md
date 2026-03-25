# Stack overflow in clang-query 17, ItaniumMangle.cpp

**Author:** ksromanov
**URL:** https://github.com/llvm/llvm-project/issues/86240
**Status:** Closed
**Labels:** clang:frontend, confirmed, crash-on-valid
**Closed Date:** 2026-02-27T19:18:12Z

## Body

On our machines (Linux) clang-tidy (llvm 17) crashes when processing

```c++
template <class RET> struct Context {
  using Ptr = Context<RET> *;
};

template <typename Callable> void scheduleCoro(Callable &&coro_function) {
  [coro_function{coro_function}](
      typename Context<decltype(coro_function())>::Ptr ctx) -> int {
    return 0;
  };
}
```

To reproduce

```
clang-query-17 a.cpp 
```

with stack dump

```
...
#238 0x000000000095b3e8 (anonymous namespace)::CXXNameMangler::mangleTemplateArg(clang::TemplateArgument, bool) ItaniumMangle.cpp:0:0
#239 0x000000000095b837 (anonymous namespace)::CXXNameMangler::mangleTemplateArgs(clang::TemplateName, llvm::ArrayRef<clang::TemplateArgument>) ItaniumMangle.cpp:0:0
#240 0x000000000096205f (anonymous namespace)::CXXNameMangler::manglePrefix(clang::NestedNameSpecifier*) ItaniumMangle.cpp:0:0
#241 0x0000000000949566 (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) ItaniumMangle.cpp:0:0
#242 0x0000000000953f7d (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) ItaniumMangle.cpp:0:0
#243 0x000000000095eed0 (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#244 0x000000000095d1ca (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
#245 0x000000000095dc2d (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#246 0x000000000095e8f2 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool)::'lambda0'(clang::NamedDecl const*)::operator()(clang::NamedDecl const*) const ItaniumMangle.cpp:0:0
#247 0x0000000000957a1e (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#248 0x0000000000956421 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#249 0x0000000000949478 (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) ItaniumMangle.cpp:0:0
#250 0x000000000095b3e8 (anonymous namespace)::CXXNameMangler::mangleTemplateArg(clang::TemplateArgument, bool) ItaniumMangle.cpp:0:0
#251 0x000000000095b837 (anonymous namespace)::CXXNameMangler::mangleTemplateArgs(clang::TemplateName, llvm::ArrayRef<clang::TemplateArgument>) ItaniumMangle.cpp:0:0
#252 0x000000000096205f (anonymous namespace)::CXXNameMangler::manglePrefix(clang::NestedNameSpecifier*) ItaniumMangle.cpp:0:0
#253 0x0000000000949566 (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) ItaniumMangle.cpp:0:0
#254 0x0000000000953f7d (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) ItaniumMangle.cpp:0:0
#255 0x000000000095eed0 (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
...
```



## Comments

### Comment 1 - EugeneZelenko

Could you please try 18 or `main` branch?

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Konstantin Romanov (ksromanov)

<details>
On our machines (Linux) clang-tidy (llvm 17) crashes when processing

```c++
template &lt;class RET&gt; struct Context {
  using Ptr = Context&lt;RET&gt; *;
};

template &lt;typename Callable&gt; void scheduleCoro(Callable &amp;&amp;coro_function) {
  [coro_function{coro_function}](
      typename Context&lt;decltype(coro_function())&gt;::Ptr ctx) -&gt; int {
    return 0;
  };
}
```

To reproduce

```
clang-query-17 a.cpp 
```

with stack dump

```
...
#<!-- -->238 0x000000000095b3e8 (anonymous namespace)::CXXNameMangler::mangleTemplateArg(clang::TemplateArgument, bool) ItaniumMangle.cpp:0:0
#<!-- -->239 0x000000000095b837 (anonymous namespace)::CXXNameMangler::mangleTemplateArgs(clang::TemplateName, llvm::ArrayRef&lt;clang::TemplateArgument&gt;) ItaniumMangle.cpp:0:0
#<!-- -->240 0x000000000096205f (anonymous namespace)::CXXNameMangler::manglePrefix(clang::NestedNameSpecifier*) ItaniumMangle.cpp:0:0
#<!-- -->241 0x0000000000949566 (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) ItaniumMangle.cpp:0:0
#<!-- -->242 0x0000000000953f7d (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) ItaniumMangle.cpp:0:0
#<!-- -->243 0x000000000095eed0 (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#<!-- -->244 0x000000000095d1ca (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector&lt;llvm::StringRef, 4u&gt; const*) ItaniumMangle.cpp:0:0
#<!-- -->245 0x000000000095dc2d (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#<!-- -->246 0x000000000095e8f2 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool)::'lambda0'(clang::NamedDecl const*)::operator()(clang::NamedDecl const*) const ItaniumMangle.cpp:0:0
#<!-- -->247 0x0000000000957a1e (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#<!-- -->248 0x0000000000956421 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#<!-- -->249 0x0000000000949478 (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) ItaniumMangle.cpp:0:0
#<!-- -->250 0x000000000095b3e8 (anonymous namespace)::CXXNameMangler::mangleTemplateArg(clang::TemplateArgument, bool) ItaniumMangle.cpp:0:0
#<!-- -->251 0x000000000095b837 (anonymous namespace)::CXXNameMangler::mangleTemplateArgs(clang::TemplateName, llvm::ArrayRef&lt;clang::TemplateArgument&gt;) ItaniumMangle.cpp:0:0
#<!-- -->252 0x000000000096205f (anonymous namespace)::CXXNameMangler::manglePrefix(clang::NestedNameSpecifier*) ItaniumMangle.cpp:0:0
#<!-- -->253 0x0000000000949566 (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) ItaniumMangle.cpp:0:0
#<!-- -->254 0x0000000000953f7d (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) ItaniumMangle.cpp:0:0
#<!-- -->255 0x000000000095eed0 (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
...
```


</details>


---

### Comment 3 - dtcxzyw

> Could you please try 18 or `main` branch?

https://godbolt.org/z/rchYTKaab

---

### Comment 4 - ksromanov

> Could you please try 18 or `main` branch?

Does reproduce on both 18 and main branches.

---

### Comment 5 - ksromanov

If I understand it right, the root cause is inside Itanium C++ ABI — the mangling standard does not support recursive types. In Hindley-Milner type system they use type variables, so it is possible to express the type that takes itself as an argument:

```ocaml
type 'a funct =
      Run of (('a funct) option -> unit)
```

here `'a` — alpha is a type variable. However I couldn't find any presence of type variables in [Itanium ABI/mangling](https://itanium-cxx-abi.github.io/cxx-abi/abi.html#mangling).

In the snippet that we are interested in, they have:

```
<prefix> ::= ....
             ::= <template-prefix> <template-args>  # class template specialization
             ....

 <template-args> ::= I <template-arg>+ E

  <template-arg> ::= <type>                                             # type or template
                 ::= X <expression> E                                   # expression
                 ::= <expr-primary>                                     # simple expressions
                 ::= J <template-arg>* E                                # argument pack
```

It is just a regular tree structure, without any type variables.

Also `ItaniumMangle.cpp` contains the comment

```c++
      // FIXME: GCC does not appear to mangle the template arguments when
      // the template in question is a dependent template name. Should we
      // emulate that badness?
```

https://github.com/llvm/llvm-project/blob/a3efc53f168b1451803a40075201c3490d6e3928/clang/lib/AST/ItaniumMangle.cpp#L1299

Perhaps it is not _badness_, it is just a way to avoid infinite recursion in the absence of the support for recursive types in Itanium C++ ABI. Of course, it is not the best choice, and perhaps someone can come up with a better proposal.

---

### Comment 6 - ccotter

@ksromanov and I uncovered some more details:

This code compiles with clang trunk

```c++
template <class RET> struct Context {
  using Ptr = Context<RET> *;
};

template <typename Callable> void templated(Callable &&a) {
  [b{a}](
      typename Context<decltype(a())>::Ptr ctx) -> int { // HERE
    return 0;
  };
}
```

But, when changing the line with `// HERE` to use `decltype(b())` instead, clang crashes with the stack overflow.

```c++
        typename Context<decltype(b())>::Ptr ctx) -> int {
```

Meanwhile, GCC rejects this code with `<source>:14:33: error: there are no arguments to 'b' that depend on a template parameter, so a declaration of 'b' must be available [-fpermissive]`. MSVC permits this code though!

https://godbolt.org/z/zW4n445cM

Once instantiating `templated`, GCC and MSVC both fail to compile with `buggy` (in all cases, Clang stack overflows).

In rejecting the `buggy` function with a compilation error, GCC appears to not see any lambda capture names while parsing the lambda function parameters. We should consult the spec to confirm whether parsing the lambda parameters can "see" any names defined as lambda captures. If not, then the fix would be to have Clang behave accordingly.

|-|Template definition of `buggy`|Instantiation of `buggy`|
|--|--|--|
|Clang|Stack overflow|Stack overflow|
|GCC|Compiler error|Compiler error|
|MSVC|Accepts the code|Compiler error|

---

### Comment 7 - ccotter

git bisect shows 93d7002dc4644b0a6f15a998dff0d55c72012e87 is what broke things (well, it fixed other things too). Before this commit, clang compiles https://godbolt.org/z/zW4n445cM without stack overflowing.

Reading P2036R3, it looks like the code from the original post should be rejected since `coro_function` is used in the parameter list.

cc @cor3ntin 

---

### Comment 8 - MagentaTreehouse

I'm almost certain that I found an even earlier report of the same issue: #63271

---

