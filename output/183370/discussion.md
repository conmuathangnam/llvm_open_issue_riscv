# [19 regression] Wrong overload resolution rules for CTAD in copy-initialization allowing double user-defined conversion

**Author:** zygoloid
**URL:** https://github.com/llvm/llvm-project/issues/183370

## Body

Testcase:

```c++
struct Wrap { Wrap(int); };
template <bool> struct Class { Class(int); };
Class(Wrap) -> Class<true>;
Class c = 0;
```

This is invalid. The CTAD rules say that we are supposed to form a class whose constructors are the guides of `Class` and try initializing it:

```c++
struct GuideClass {
  GuideClass(Wrap);
  // ... other constructors ...
};
GuideClass c = 0;
```

But this fails due to [over.best.ics]/4.4, because it would invoke two user-defined conversions (one from `int` to `Wrap` and another from `Wrap` to `GuideClass`). So CTAD should fail here.

Clang 18 and earlier got this right; Clang 19 onwards incorrectly deduces `Class` -> `Class<true>`.

## Comments

### Comment 1 - zygoloid

For what it's worth, it looks like other compilers also get this wrong.

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Richard Smith (zygoloid)

<details>
Testcase:

```c++
struct Wrap { Wrap(int); };
template &lt;bool&gt; struct Class { Class(int); };
Class(Wrap) -&gt; Class&lt;true&gt;;
Class c = 0;
```

This is invalid. The CTAD rules say that we are supposed to form a class whose constructors are the guides of `Class` and try initializing it:

```c++
struct GuideClass {
  GuideClass(Wrap);
  // ... other constructors ...
};
GuideClass c = 0;
```

But this fails due to [over.best.ics]/4.4, because it would invoke two user-defined conversions (one from `int` to `Wrap` and another from `Wrap` to `GuideClass`). So CTAD should fail here.

Clang 18 and earlier got this right; Clang 19 onwards incorrectly deduces `Class` -&gt; `Class&lt;true&gt;`.
</details>


---

### Comment 3 - zygoloid

Perhaps a more interesting example where Clang 18 accepted and Clang 19 onwards rejects valid:

```c++
struct Wrap { Wrap(int); };

template<typename T = void> struct A {
  A(Wrap);
};

template<> struct A<int> {
  A(int);
};

A(...) -> A<int>;

int *p;
A a = 0;
```

Here we should use the `A(...)` guide, not the `A(Wrap)` guide from the constructor, because the constructor can't be used for this initialization.

---

### Comment 4 - zygoloid

Looks like this was broken by #94752 for #62925 (@spaits), which removed all the user-defined conversion suppression when it looks like it should only have removed the suppression for list-initialization. We're going to need a more nuanced test here to properly implement https://eel.is/c++draft/over.best.ics#general-4 for CTAD.

---

### Comment 5 - spaits

Sorry for causing the regression. I am happy to take a look at this when I have some time in the next few weeks.

---

