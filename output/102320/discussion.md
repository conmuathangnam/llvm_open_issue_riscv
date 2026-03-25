# [clang-18 regression] Compile error in out-of-line definition of constrained class template nested in a class template

**Author:** ozaktash
**URL:** https://github.com/llvm/llvm-project/issues/102320

## Body

The following code doesn't compile on Clang 18 and further:
```cpp
template<typename>
concept Constrained = true;

template <typename T>
class C
{
    template<Constrained>
    class D;
};

template <typename T>
template <Constrained>
class C<T>::D
{
};
```
With the following message:
```
<source>:12:11: error: type constraint differs in template redeclaration
   12 | template <Constrained>
      |           ^
<source>:7:14: note: previous template declaration is here
    7 |     template<Constrained>
      |              ^
1 error generated.
```

It compiles with Clang 17, GCC and MSVC (https://godbolt.org/z/r65o8sPjs).


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (ozaktash)

<details>
The following code doesn't compile on Clang 18 and further:
```cpp
template&lt;typename&gt;
concept Constrained = true;

template &lt;typename T&gt;
class C
{
    template&lt;Constrained&gt;
    class D;
};

template &lt;typename T&gt;
template &lt;Constrained&gt;
class C&lt;T&gt;::D
{
};
```
With the following message:
```
&lt;source&gt;:12:11: error: type constraint differs in template redeclaration
   12 | template &lt;Constrained&gt;
      |           ^
&lt;source&gt;:7:14: note: previous template declaration is here
    7 |     template&lt;Constrained&gt;
      |              ^
1 error generated.
```

It compiles with Clang 17, GCC and MSVC (https://godbolt.org/z/r65o8sPjs).

</details>


---

### Comment 2 - AaronBallman

Somewhat amusingly, Clang 18 fixed an issue present in Clang 17 regarding explicit specializations with similar code: https://godbolt.org/z/e7doYqe1d

---

### Comment 3 - zyn0217

Fixed by #106585.

---

### Comment 4 - MagentaTreehouse

Requesting to reopen this issue as the fix was reverted and the error still reproduces in trunk.

---

### Comment 5 - ilya-biryukov

Another example from #133413, which I'll close as duplicate. Putting it here for reference to increase the chances of this being checked when the fix is in.

```cpp
template <class FooT, class ArgT>
concept CanDo = requires (ArgT arg){
  FooT::Do(arg);
};

struct Arg {

};

template <class T>
class Outer {
 public:
  template <class FooT>
    requires CanDo<FooT, Arg>
  class Inner;
};

template <class T>
template <class FooT>
    requires CanDo<FooT, Arg>
class Outer<T>::Inner {

};
```

---

