# Clang: Failed assertion in nested instantiation

**Author:** katzdm
**URL:** https://github.com/llvm/llvm-project/issues/127421

## Body

While investigating a separate crash in a clang fork, I came across this gnarly nested instantiation that ICE's when assertions are enabled.

https://godbolt.org/z/317rj76cK

I wish I could say that I could reproduce this with fewer than three layers of nested templates - alas, nay.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Daniel M. Katz (katzdm)

<details>
While investigating a separate crash in a clang fork, I came across this gnarly nested instantiation that ICE's when assertions are enabled.

https://godbolt.org/z/317rj76cK

I wish I could say that I could reproduce this with fewer than three layers of nested templates - alas, nay.
</details>


---

### Comment 2 - frederick-vs-ja

IIUC the original example is ill-formed.

```C++
template<typename ToConstruct>
constexpr auto named_construct = []<auto>() consteval {
  static constexpr int to_ret = 0;
  []<auto>() {
     (void) to_ret;
  }.template operator()<1>();
};

auto i =  named_construct<int>();
```

Because
- the first `auto` is neither deduced nor explicitly specified, and
- the lambda expression has no `return` statement, so the return type of the `operator()` would be `void`, which in turn makes the initialization of `i` invalid.

Is this intended?

---

### Comment 3 - zyn0217

Here is a valid example which also crashes since 19. 

https://godbolt.org/z/9fWGrE9Kv

```cpp
template<typename ToConstruct>
constexpr auto named_construct = []<class T>(T) consteval {
  static constexpr int to_ret = 0;
  []<auto>() {
     (void) to_ret;
  }.template operator()<1>();
  return to_ret;
};

auto i = named_construct<int>(42);
```

---

### Comment 4 - zyn0217

This is related to incorrect inner lambda dependency.

Currently, we eagerly substitute into a lambda's body when transforming a lambda expression. When transforming the body of an outer lambda, we encounter an inner lambda and its call expression, which prompts us to transform the callee, i.e. the inner lambda.

At this point, we don't have the template arguments for the inner `<auto>`, so both the original lambda and the transformed lambda expression are supposed to remain dependent. However, thanks to this workaround, the inner lambda becomes non-dependent (in an immediate context).

https://github.com/llvm/llvm-project/blob/6684a5970e74b8b4c0c83361a90e25dae9646db0/clang/lib/Sema/TreeTransform.h#L15364-L15367

As a result, we compute the call expression eagerly and, therein transform the inner lambda's body in a separate instantiation. This separation causes us to lose any local instantiations tracked by the outer lambda transformation.

@mizvekov Your patch gets rid of the dependency workaround so it might help with this too.

---

### Comment 5 - mizvekov

FYI it's helpful to tag #107942 here as I will retest all these issues and eventually mark them as fixed.

---

