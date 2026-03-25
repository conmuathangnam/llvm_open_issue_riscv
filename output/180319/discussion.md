# clang: recusive lambdas in templates can segfault by exausting stack

**Author:** ribbon-otter
**URL:** https://github.com/llvm/llvm-project/issues/180319

## Body

Compiling the following code causes a stack overflow on `clang++  std=c++23`
```c++
struct foo_tag {};

template<class T> struct foo {
	using tag = foo_tag;
	T run;
};

template<class T> concept isFoo = requires(T a) {a.run();};

//function to construct very complex type with lambda
template<int i, class T> auto complexify(T a) requires isFoo<T> {
	if constexpr (i > 0) {
		return complexify<i-1>(foo{ [a]{
			return 1+a.run();
		}});
	} else return a;
}

//base case
template<int i> auto complexify(int a) {
	return complexify<i-1>(foo{ [a]{
		return a;
	}});
}

int main() {
	auto a = complexify<389>(1);
}
// vim: ts=2 sw=2
```

Output that the main version of clang outputs:
[output.txt](https://github.com/user-attachments/files/25141859/output.txt)
and the files it wishes me to attach:
[clang-bug-c2e6e3.sh](https://github.com/user-attachments/files/25141857/clang-bug-c2e6e3.sh)
[clang-bug-c2e6e3.cpp](https://github.com/user-attachments/files/25141858/clang-bug-c2e6e3.cpp)

by adjusting `complexify<398>(t)` to be a slightly smaller (for example, `381` on my machine + build), I get a warning about the stack being nearly exhausted. 
```
clang-bug.cpp:3:26: warning: stack nearly exhausted; compilation time may suffer, and crashes due to stack overflow
      are likely [-Wstack-exhausted]
```

g++ compiles it without issue. (though it has a related bug ([Bug 120124](https://gcc.gnu.org/bugzilla/show_bug.cgi?id=120124)) where it crashes there is an error message that involves this; which is how I found this clang bug by accident)

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (ribbon-otter)

<details>
Compiling the following code causes a stack overflow on `clang++  std=c++23`
```c++
struct foo_tag {};

template&lt;class T&gt; struct foo {
	using tag = foo_tag;
	T run;
};

template&lt;class T&gt; concept isFoo = requires(T a) {a.run();};

//function to construct very complex type with lambda
template&lt;int i, class T&gt; auto complexify(T a) requires isFoo&lt;T&gt; {
	if constexpr (i &gt; 0) {
		return complexify&lt;i-1&gt;(foo{ [a]{
			return 1+a.run();
		}});
	} else return a;
}

//base case
template&lt;int i&gt; auto complexify(int a) {
	return complexify&lt;i-1&gt;(foo{ [a]{
		return a;
	}});
}

int main() {
	auto a = complexify&lt;389&gt;(1);
}
// vim: ts=2 sw=2
```

Output that the main version of clang outputs:
[output.txt](https://github.com/user-attachments/files/25141859/output.txt)
and the files it wishes me to attach:
[clang-bug-c2e6e3.sh](https://github.com/user-attachments/files/25141857/clang-bug-c2e6e3.sh)
[clang-bug-c2e6e3.cpp](https://github.com/user-attachments/files/25141858/clang-bug-c2e6e3.cpp)

by adjusting `complexify&lt;398&gt;(t)` to be a slightly smaller (for example, `381` on my machine + build), I get a warning about the stack being nearly exhausted. 
```
clang-bug.cpp:3:26: warning: stack nearly exhausted; compilation time may suffer, and crashes due to stack overflow
      are likely [-Wstack-exhausted]
```

g++ compiles it without issue. (though it has a related bug ([Bug 120124](https://gcc.gnu.org/bugzilla/show_bug.cgi?id=120124)) where it crashes there is an error message that involves this; which is how I found this clang bug by accident)
</details>


---

