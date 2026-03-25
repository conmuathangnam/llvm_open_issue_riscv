# Clang instantiates function templates inside a discarded statement

**Author:** seha-bot
**URL:** https://github.com/llvm/llvm-project/issues/140449
**Status:** Closed
**Labels:** clang:frontend
**Closed Date:** 2025-05-20T05:22:03Z

## Body

According to [[temp.inst]/5](https://eel.is/c++draft/temp.inst#5), a function template will be instantiated if its definition is required or if it changes the semantics of the program. A definition is required for a function if its name is odr-used outside a discarded statement [[basic.def.odr]/12](https://eel.is/c++draft/basic.def.odr#12.sentence-1).

However, if we write code such as:

```c++
template <typename T>
int f() {
    T *ptr;
    return 0;
}

int main() {
    if constexpr (false) {
        int x = f<int &>();  // OK, but clang errors
    }
}
```

clang attempts to instantiate `f<int &>` which forms an invalid type and the compilation fails. The instantiation should not have happened in the first place. `f<int &>` is inside a discarded statement, so its definition is not required.

Reproduction: https://godbolt.org/z/K9azcbYsc
There is one more example in that link which passes correctly just to show that this behavior is also inconsistent in clang. This is a regression because it worked fine in earlier versions of clang as shown in that link.

## Comments

### Comment 1 - cor3ntin

This is the expected behavior.  `main` is not an enclosing templated entity, so instantiation happen unconditionally.

That being said, this question comes up frequently enough that I think we need a diagnostic to explain this case.

Ideally I think we would make it ill-formed at the standard level.

https://eel.is/c++draft/stmt.if#2.sentence-3

---

### Comment 2 - HolyBlackCat

@cor3ntin If it should be instantiated unconditionally, I wonder why clang doesn't error on `g()` in the godbolt example above.

---

### Comment 3 - cor3ntin

Because it's still a discarded statement, so the rule from basic.odr mentioned in the first message still applies. however the "entities are not instantiated" bit does not.


---

### Comment 4 - HolyBlackCat

Sorry, I don't follow.

In the godbolt link, why does `f()` error but not `g()`? If both are instantiated, then at face value both should error? This is what I'm asking.

I've posted a stackoverflow question about this: https://stackoverflow.com/q/79627471/2752075

---

### Comment 5 - michael-kenzel

> This is the expected behavior. `main` is not an enclosing templated entity, so instantiation happen unconditionally.

Shouldn't the fact that it's not in an enclosing templated entity only mean that the statement itself is instantiated? Since it's instantiated, it has to be well-formed. But the statement is still a discarded statement, so any odr-use of an entity within the statement still doesn't require a definition to exist. And, thus, the template shouldn't be instantiated?

---

### Comment 6 - cor3ntin

> Sorry, I don't follow.
> 
> In the godbolt link, why does `f()` error but not `g()`? If both are instantiated, then at face value both should error? This is what I'm asking.
> 
> I've posted a stackoverflow question about this: https://stackoverflow.com/q/79627471/2752075


The expression g<int>() is well formed and doesn't require a definition to exists. and because that expression is in a discarded statement it is not odr used and a definition is never needed so never instantiated. if g had a deduced return type for example, which would require us to instantiate the definition, you would get an error.


---

### Comment 7 - michael-kenzel

> In the godbolt link, why does `f()` error but not `g()`? If both are instantiated, then at face value both should error? This is what I'm asking.

The difference seems to come down to the fact that `f()` is being used in an initializer. It's just a guess, but I'd guess that maybe clang is doing some trial constant evaluation and that's what actually causes the instantiation?

---

### Comment 8 - cor3ntin

Turns out there was a bug afterwards!

---

### Comment 9 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (seha-bot)

<details>
According to [[temp.inst]/5](https://eel.is/c++draft/temp.inst#<!-- -->5), a function template will be instantiated if its definition is required or if it changes the semantics of the program. A definition is required for a function if its name is odr-used outside a discarded statement [[basic.def.odr]/12](https://eel.is/c++draft/basic.def.odr#<!-- -->12.sentence-1).

However, if we write code such as:

```c++
template &lt;typename T&gt;
int f() {
    T *ptr;
    return 0;
}

int main() {
    if constexpr (false) {
        int x = f&lt;int &amp;&gt;();  // OK, but clang errors
    }
}
```

clang attempts to instantiate `f&lt;int &amp;&gt;` which forms an invalid type and the compilation fails. The instantiation should not have happened in the first place. `f&lt;int &amp;&gt;` is inside a discarded statement, so its definition is not required.

Reproduction: https://godbolt.org/z/K9azcbYsc
There is one more example in that link which passes correctly just to show that this behavior is also inconsistent in clang. This is a regression because it worked fine in earlier versions of clang as shown in that link.
</details>


---

