# Buffer overflow with excess array initializers in #embed

**Author:** efriedma-quic
**URL:** https://github.com/llvm/llvm-project/issues/128987
**Status:** Closed
**Labels:** c23, clang:frontend, miscompilation, confirmed, embed
**Closed Date:** 2025-03-05T10:04:54Z

## Body

Testcase:

```
void f(int x) {
  int p[8] = {x, x, x, x, x, x, x,
#embed __FILE__ limit(3)
};
}
```

This stores to `p[8]` and `p[9]`, which are past the end of the array.

Related assertion:
```
void f(int x) {
  int p[8] = {x, x, x, x, x, x, x,
#embed __FILE__ limit(10)
};
}
```
```
llvm/include/llvm/Support/Casting.h:109: static bool llvm::isa_impl_cl<clang::ImplicitValueInitExpr, const clang::Expr *>::doit(const From *) [To = clang::ImplicitValueInitExpr, From = const clang::Expr *]: Assertion `Val && "isa<> used on a null pointer"' failed.
```

CC @Fznamznon 

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: Eli Friedman (efriedma-quic)

<details>
Testcase:

```
void f(int x) {
  int p[8] = {x, x, x, x, x, x, x,
#embed __FILE__ limit(3)
};
}
```

This stores to `p[8]` and `p[9]`, which are past the end of the array.

Related assertion:
```
void f(int x) {
  int p[8] = {x, x, x, x, x, x, x,
#embed __FILE__ limit(10)
};
}
```
```
llvm/include/llvm/Support/Casting.h:109: static bool llvm::isa_impl_cl&lt;clang::ImplicitValueInitExpr, const clang::Expr *&gt;::doit(const From *) [To = clang::ImplicitValueInitExpr, From = const clang::Expr *]: Assertion `Val &amp;&amp; "isa&lt;&gt; used on a null pointer"' failed.
```

CC @<!-- -->Fznamznon 
</details>


---

### Comment 2 - Fznamznon

Thanks for reporting this. I think I have a fix. Need to test a bit and I'll push a PR tomorrow. Somehow I was so caught up in improving performance of enormously big arrays that I forgot that small arrays exist.

---

### Comment 3 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Eli Friedman (efriedma-quic)

<details>
Testcase:

```
void f(int x) {
  int p[8] = {x, x, x, x, x, x, x,
#embed __FILE__ limit(3)
};
}
```

This stores to `p[8]` and `p[9]`, which are past the end of the array.

Related assertion:
```
void f(int x) {
  int p[8] = {x, x, x, x, x, x, x,
#embed __FILE__ limit(10)
};
}
```
```
llvm/include/llvm/Support/Casting.h:109: static bool llvm::isa_impl_cl&lt;clang::ImplicitValueInitExpr, const clang::Expr *&gt;::doit(const From *) [To = clang::ImplicitValueInitExpr, From = const clang::Expr *]: Assertion `Val &amp;&amp; "isa&lt;&gt; used on a null pointer"' failed.
```

CC @<!-- -->Fznamznon 
</details>


---

