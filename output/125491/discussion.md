# if constexpr code not ignored inside an immediately invoked lambda

**Author:** Toeger
**URL:** https://github.com/llvm/llvm-project/issues/125491

## Body

### Minimal reproducible example
```cpp
template <class... Ts>
struct Type_list {
	private:
	template <int index, class... Args>
	struct Get_at;
	template <class T, class... Args>
	struct Get_at<0, T, Args...> {
		using Type = T;
	};
	template <int index, class T, class... Args>
	struct Get_at<index, T, Args...> {
		using Type = typename Get_at<index - 1, Args...>::Type;
	};

	public:
	template <int index>
	using at = typename Get_at<index, Ts...>::Type;
	constexpr static auto size = sizeof...(Ts);
};

template <class F>
struct CI {};
template <class R, class... PArgs>
struct CI<R (*)(PArgs...)> {
	using Args = Type_list<PArgs...>;
};

template <class Function>
constexpr bool is_viable_source = []<class Properties_list>(Properties_list) {
	if constexpr (Properties_list::size != Properties_list::size) {
		static_assert(false);
		return sizeof(typename CI<Function>::Args::template at<0>) > 0;
	}
	return false;
}(Type_list<>{});

int main() {
	return is_viable_source<void (*)()>;
}

```
https://godbolt.org/z/o3co4j31e

### Expected behavior
Successfully builds executable equivalent to `int main() {}`.

### Actual behavior: Compilation error
```
<source>:17:2: error: implicit instantiation of undefined template 'Type_list<>::Get_at<0>'
   17 |         using at = typename Get_at<index, Ts...>::Type;
      |         ^
<source>:32:55: note: in instantiation of template type alias 'at' requested here
   32 |                 return sizeof(typename CI<Function>::Args::template at<0>) > 0;
      |                                                                     ^
<source>:29:78: note: while substituting into a lambda expression here
   29 | constexpr bool is_viable_source = []<class Properties_list>(Properties_list) {
      |                                                                              ^
<source>:38:9: note: in instantiation of variable template specialization 'is_viable_source<void (*)()>' requested here
   38 |         return is_viable_source<void (*)()>;
      |                ^
<source>:5:9: note: template is declared here
    5 |         struct Get_at;
      |                ^
1 error generated.
```

### Affected versions
Clang 18, clang 19 and clang trunk
C++23 and above (it seems the explicit lambda template argument is required to reproduce, which is a C++23 feature)
Gcc compiles the code successfully.

### Reasoning why clang is wrong
The error is caused by code inside an `if constexpr` that evaluates to `false`, thus clang should not have attempted to compile the code or ignored the error. Clang partially agrees that the `if constexpr` evaluates to `false` and that the code should not be compiled since it does not complain about the `static_assert(false);`.

### Workaround
The code compiles after replacing the immediately invoked lambda with a helper function: <https://godbolt.org/z/6P5qToWWT>
This also applies to my ungolfed code.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (Toeger)

<details>
### Minimal reproducible example
```cpp
template &lt;class... Ts&gt;
struct Type_list {
	private:
	template &lt;int index, class... Args&gt;
	struct Get_at;
	template &lt;class T, class... Args&gt;
	struct Get_at&lt;0, T, Args...&gt; {
		using Type = T;
	};
	template &lt;int index, class T, class... Args&gt;
	struct Get_at&lt;index, T, Args...&gt; {
		using Type = typename Get_at&lt;index - 1, Args...&gt;::Type;
	};

	public:
	template &lt;int index&gt;
	using at = typename Get_at&lt;index, Ts...&gt;::Type;
	constexpr static auto size = sizeof...(Ts);
};

template &lt;class F&gt;
struct CI {};
template &lt;class R, class... PArgs&gt;
struct CI&lt;R (*)(PArgs...)&gt; {
	using Args = Type_list&lt;PArgs...&gt;;
};

template &lt;class Function&gt;
constexpr bool is_viable_source = []&lt;class Properties_list&gt;(Properties_list) {
	if constexpr (Properties_list::size != Properties_list::size) {
		static_assert(false);
		return sizeof(typename CI&lt;Function&gt;::Args::template at&lt;0&gt;) &gt; 0;
	}
	return false;
}(Type_list&lt;&gt;{});

int main() {
	return is_viable_source&lt;void (*)()&gt;;
}

```
https://godbolt.org/z/o3co4j31e

### Expected behavior
Successfully builds executable equivalent to `int main() {}`.

### Actual behavior: Compilation error
```
&lt;source&gt;:17:2: error: implicit instantiation of undefined template 'Type_list&lt;&gt;::Get_at&lt;0&gt;'
   17 |         using at = typename Get_at&lt;index, Ts...&gt;::Type;
      |         ^
&lt;source&gt;:32:55: note: in instantiation of template type alias 'at' requested here
   32 |                 return sizeof(typename CI&lt;Function&gt;::Args::template at&lt;0&gt;) &gt; 0;
      |                                                                     ^
&lt;source&gt;:29:78: note: while substituting into a lambda expression here
   29 | constexpr bool is_viable_source = []&lt;class Properties_list&gt;(Properties_list) {
      |                                                                              ^
&lt;source&gt;:38:9: note: in instantiation of variable template specialization 'is_viable_source&lt;void (*)()&gt;' requested here
   38 |         return is_viable_source&lt;void (*)()&gt;;
      |                ^
&lt;source&gt;:5:9: note: template is declared here
    5 |         struct Get_at;
      |                ^
1 error generated.
```

### Affected versions
Clang 18, clang 19 and clang trunk
C++23 and above (it seems the explicit lambda template argument is required to reproduce, which is a C++23 feature)
Gcc compiles the code successfully.

### Reasoning why clang is wrong
The error is caused by code inside an `if constexpr` that evaluates to `false`, thus clang should not have attempted to compile the code or ignored the error. Clang partially agrees that the `if constexpr` evaluates to `false` and that the code should not be compiled since it does not complain about the `static_assert(false);`.


</details>


---

### Comment 2 - shafik

I think this is https://github.com/llvm/llvm-project/issues/55890

CC @cor3ntin 

---

### Comment 3 - cor3ntin

@erichkeane do we have an issue to track "Erich needs to work on delayed body instantiation"?

---

### Comment 4 - erichkeane

> [@erichkeane](https://github.com/erichkeane) do we have an issue to track "Erich needs to work on delayed body instantiation"?

Probably... i just don't know it .

EDIT: Think it is this one: https://github.com/llvm/llvm-project/issues/58872

Thats the one that caused me to talk about it initially. I put together a 'doesn't work at all' 1st pass patch here (https://reviews.llvm.org/D138148).  I'm reasonably confident that the Phab review + "make sure we instantiate it everywhere we actually call it" would be enough to get that feature done correctly.

---

