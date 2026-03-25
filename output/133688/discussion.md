# [Clang][Sema] Stack overflow when trying to get the layout of a class with a member with a nested dependency

**Author:** alejandro-alvarez-sonarsource
**URL:** https://github.com/llvm/llvm-project/issues/133688

## Body

There is an assertion error when built with assertions enabled:

```
clang++: /root/build/tools/clang/include/clang/AST/TypeNodes.inc:77: clang::TypeInfo clang::ASTContext::getTypeInfoImpl(const clang::Type*) const: Assertion `!T->isDependentType() && "should not see dependent types here"' failed.
```

The reproducer:

```cpp
template <typename T> auto id(const T &obj) -> decltype(obj.identifier());

template <typename T> struct ContainedId {
  auto operator()(const T &obj) -> decltype(id(obj));
};

template <class T, class = decltype(ContainedId<T>{}.operator()(T{}))>
class Container {};

class Contained {
public:
  int identifier();
};

class Collection {
  Container<Contained> m_changesToPersist;
};

Collection collection;
```

This is a regression from clang 17: https://godbolt.org/z/x5ezKnf79

I have bisected this, and I think it was introduced by #75456

Note that the crash happens because `identifier` can not be resolved due to const-nedness, adding a `const` makes the problem go away and the code compiles.

This is a detail, just making it possible not to resolve `identifier` (by renaming, deleting, etc.) is enough to trigger the crash.

The two levels of indirection are relevant: `Container<Contained> -> ContainedId<Contained> -> id`

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Alejandro Álvarez Ayllón (alejandro-alvarez-sonarsource)

<details>
There is an assertion error when built with assertions enabled:

```
clang++: /root/build/tools/clang/include/clang/AST/TypeNodes.inc:77: clang::TypeInfo clang::ASTContext::getTypeInfoImpl(const clang::Type*) const: Assertion `!T-&gt;isDependentType() &amp;&amp; "should not see dependent types here"' failed.
```

The reproducer:

```cpp
template &lt;typename T&gt; auto id(const T &amp;obj) -&gt; decltype(obj.identifier());

template &lt;typename T&gt; struct ContainedId {
  auto operator()(const T &amp;obj) -&gt; decltype(id(obj));
};

template &lt;class T, class = decltype(ContainedId&lt;T&gt;{}.operator()(T{}))&gt;
class Container {};

class Contained {
public:
  int identifier();
};

class Collection {
  Container&lt;Contained&gt; m_changesToPersist;
};

Collection collection;
```

This is a regression from clang 17: https://godbolt.org/z/x5ezKnf79

I have bisected this, and I think it was introduced by #<!-- -->75456

Note that the crash happens because `identifier` can not be resolved due to const-nedness, adding a `const` makes the problem go away and the code compiles.

This is a detail, just making it possible not to resolve `identifier` (by renaming, deleting, etc.) is enough to trigger the crash.

The two levels of indirection are relevant: `Container&lt;Contained&gt; -&gt; ContainedId&lt;Contained&gt; -&gt; id`
</details>


---

### Comment 2 - shafik

CC @Fznamznon 

---

