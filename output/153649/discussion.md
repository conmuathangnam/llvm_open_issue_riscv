# [clang-tidy] false positive for modernize-type-traits with structured bindings

**Author:** matthew-f
**URL:** https://github.com/llvm/llvm-project/issues/153649
**Status:** Closed
**Labels:** clang:frontend, confirmed
**Closed Date:** 2025-08-16T05:04:32Z

## Body

Hi 

Using an llvm update from a few days `modernize-type-traits` is generating what looks like a false positive. An update from today still exhibits the same problem
```
std::map<int, int> map = {{1, 2}, {3, 4}};

for (const auto& [a, b] : map)
	{
		std::cout << a << b;
	}
```
generates

```
main.cpp:36:20: error: use c++14 style type templates [modernize-type-traits,-warnings-as-errors]
   36 |         for (const auto& [a, b] : map)
      |                           ^
      |                           _t
main.cpp:36:23: error: use c++14 style type templates [modernize-type-traits,-warnings-as-errors]
   36 |         for (const auto& [a, b] : map)
      |                              ^
      |                              _t


```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-tidy

Author: None (matthew-f)

<details>
Hi 

Using an llvm update from a few days `modernize-type-traits` is generating what looks like a false positve.
```
std::map&lt;int, int&gt; map = {{1, 2}, {3, 4}};

for (const auto&amp; [a, b] : map)
	{
		std::cout &lt;&lt; a &lt;&lt; b;
	}
```
generates

```
main.cpp:36:20: error: use c++14 style type templates [modernize-type-traits,-warnings-as-errors]
   36 |         for (const auto&amp; [a, b] : map)
      |                           ^
      |                           _t
main.cpp:36:23: error: use c++14 style type templates [modernize-type-traits,-warnings-as-errors]
   36 |         for (const auto&amp; [a, b] : map)
      |                              ^
      |                              _t


```
</details>


---

### Comment 2 - vbvictor

https://godbolt.org/z/9c7Gab8oc
This is odd TBH, `type-traits` seems unrelated to structured bindings.

---

### Comment 3 - vbvictor

This bug gave 63 FPs on `clang-tools-extra/clang-tidy` which is… very unfortunate.
Need to look into it.

---

### Comment 4 - vbvictor

This must have broke it https://github.com/llvm/llvm-project/pull/147835.

---

### Comment 5 - mizvekov

What looks bizarre is that this is using a matcher on TypeLocs, but there shouldn't be a TypeLoc in there, the user didn't write any types at that point. This could be a fake TypeLoc from some misuse / workaround.

---

### Comment 6 - mizvekov

Another point here is that the matcher didn't previously handle a NestedNameSpecifier of identifier kind.

Now after [#147835](https://github.com/llvm/llvm-project/pull/147835) these kinds of NestedNameSpecifiers are represented as DependentNameTypes, which this matcher handles.

---

### Comment 7 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (matthew-f)

<details>
Hi 

Using an llvm update from a few days `modernize-type-traits` is generating what looks like a false positive. An update from today still exhibits the same problem
```
std::map&lt;int, int&gt; map = {{1, 2}, {3, 4}};

for (const auto&amp; [a, b] : map)
	{
		std::cout &lt;&lt; a &lt;&lt; b;
	}
```
generates

```
main.cpp:36:20: error: use c++14 style type templates [modernize-type-traits,-warnings-as-errors]
   36 |         for (const auto&amp; [a, b] : map)
      |                           ^
      |                           _t
main.cpp:36:23: error: use c++14 style type templates [modernize-type-traits,-warnings-as-errors]
   36 |         for (const auto&amp; [a, b] : map)
      |                              ^
      |                              _t


```
</details>


---

